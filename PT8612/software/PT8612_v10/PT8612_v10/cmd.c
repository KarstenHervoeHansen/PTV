/***************************************************************************/
/*  Copyright DK-Audio A/S, 2005                                           */
/*  Project:  PT8641 HDTPG (PT8612), using 8051F320 Cygnal                 */
/*            processor.                                                   */
/*  File  :   cmd.C                                                        */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     01.03.2005                                                   */
/*  Status:   Version 8.0                                                  */
/*  Purpose:  This module implements serial communication routines used to */
/*            communicate with PT5300 Master.  It also contains two        */
/*            software SPI functions for communication with the FPGA.      */
/*            The following command set is implemented:                    */
/*                                                                         */
/* Changes:                                                                */
/* 27.05.2008: Changed SW version to 10, to match new SW bundle with FPGA  */	
/* 27.06.2007: Mainframe sends correct Sysetm byte (Mainframe ver 4.9)     */
/*             Version of this software is now 90.                         */
/* 26.01.2007: Querrying FPGA in Manual Mode implemented. Addresses in     */
/*             FPGA param RAM: 128 - 133 may now be querried (Ver 80).     */
/*             see comments in ManualMode().                               */
/* 17.08.2006: Manual Mode implemented (direct R/W to FPGA settings RAM)   */
/*             (Version 70)                                                */
/* 29.06.2006: JTAGMode (programming FPGA via JTAG) implemented.           */
/* 14.06.2006: Uart Tx made open drain    (version 50)(there was no v.40)  */
/* 21.02.2006: Processor changed to F320  (version 30)                     */
/*                                                                         */
/*           To issue these commands from an external PC connected to      */
/*           Master, we use the following FACTORY commands:                */
/*           :FACT:V24C:ADDR BBU_x;COMM 'UE?';   to get HW version         */
/*           :FACT:V24C:ADDR BBU_x;COMM 'UB',1,'string'; to write u.s 1    */
/*           :FACT:V24C:ADDR BBU_x;COMM 'UC?',1; to get user string 1      */
/*           ... etc.                                                      */
/*           We use BBU_x because the HD after it is found to be           */
/*           installed, gets its address changed to bbux_addr              */
/*                                                                         */
/*      KU number is read by master as string as follows:                  */
/*      ascPtr = RecStr(address, "UR", ProductKUNo_addr/10, &error);       */
/*      where ProductKUNo_addr/10 = 2.                                     */
/*      KU number is stored as user string nr. 2.                          */
/*                                                                         */
/*      When 5300 Master changes generator Pattern, or System, or Delay,   */
/*      or Attributes, it is done in one go using TrxHDTPGSystem() function*/
/*      from unit_drv.c.  This function sends the mentioned params in the  */
/*      following sequence:                                                */
/*                                                                         */
/* SndInt2(HDTPGUnit[ndx].Address, "UL", (UI)ndx, (UI)system);             */
/* SndInt3(HDTPGUnit[ndx].Address, "UM", (UI)ndx, (UI)pattern, (UI)attrib);*/
/* SndULUL(HDTPGUnit[ndx].Address, "UK", (UL)ndx, (UL)tmp); // delay       */
/*                                                                         */
/***************************************************************************/
#pragma WARNINGLEVEL (1) /* Lists only those warnings which may generate */
                         /* incorrect code. Default is 2.                */

#include <C8051F320.h>
#include <stdio.h>
#include "define.h"
#include <string.h>
#include "cmd.h"
#include "ser_spi.h"
#include "eeprom.h"
#include "serial.h"
#include "micro.h"


#define PT8641_COMMAND_PREFIX  'U'    /* First letter of PT8641 command */
                                      /* set */

/* User text start address in onboard EEPROM: 10 blocks of 11 byte (10 data + checksum) */
#define USER_TEXT              0x0B

#define SWVERSION              100
#define HWVERSION              00
#define HWINFO                 12 /* This generatoar is called PT8612 */

/*                   HD-SDI PT8612                                  */
/* In the FPGA, each parameter has a location in RAM, at particular */
/* address.  Changing a setting (parameter) involves sending        */
/* genrator number, value and its address, using defins below       */
/*       Symbol          Value                Setting               */
/*   -------------------------------------------------------------- */
#define   SYS_ADDR           0         /* System                    */
#define   PAT_ADDR           1         /* Pattern, Attribs          */
#define   DEL_ADDR           3         /* Delay   MSB...LSB         */
#define   CLI_ADDR           7         /* Audio Click               */
#define   SIG_ADDR           9         /* Audio Signal              */
#define   LEV_ADDR          10         /* Audio Level               */

#define   HOR_ADDR          11         /* Text Position  Horizontal */

#define   VER_ADDR          13         /* Text Position  Vertical   */

#define   SCA_ADDR          15         /* Text Scale                */
#define   COL_ADDR          16         /* Text Color                */
#define   SIZ_ADDR_H        17         /* Text Bitmap Horizontal    */
#define   SIZ_ADDR_V        18         /* Text Bitmap Horizontal    */
#define   MOV_ADDR          19         /* Text Movement             */
#define   TXTONOFF_ADDR     20         /* Text ON / OFF             */

/* Addresses below contain now some status in FPGA.  When querried  */
/* on these addresses, the FPGA returns values @ these addresses.   */
/* On all other RAM addresses the FPGA does not return any values.  */
/* This means that you can not querry DEL_ADDR for example and      */
/* expect to get current delay.                                     */

#define   STAT_CHAN1        128
#define   STAT_CHAN2        129
#define   STAT_CHAN3        130
#define   STAT_CHAN4        131
#define   STAT_PLL          132
#define   STAT_GENL         133
/********************************************************************/
#define   TXT_ADDR          0x1000     /* Text Bitmap  4096         */

#define   TXT_ADDR_HI       0x10
#define   TXT_ADDR_LO       0x00

#define NOOFCOMMANDS 21

code struct COMMAND_STRUCT CommandTable[NOOFCOMMANDS] =
  {
  /* Housekeeping functions                     */
  'U', 'A', /* UnitAddressCommand()          0  */
  'U', 'B', /* WriteUserTextCommand()        1  */
  'U', 'C', /* ReadUserTextCommand()         2  */
  'U', 'D', /* ReadSWVersionCommand()        3  */
  'U', 'E', /* ReadHWVersionCommand()        4  */
  'U', 'F', /* ReadHWInformationCommand()    5  */
  /* Settings functions                         */
  'U', 'G', /* GenSystemCommand()            6  */
  'U', 'H', /* GenPatternCommand()           7  */
  'U', 'I', /* GenAttribCommand()            8  */
  'U', 'J', /* GenDelayCommand()             9  */
  'U', 'K', /* GenAudioClickTimeCommand()    10 */
  'U', 'L', /* GenAudioSignalCommand()       11 */
  'U', 'M', /* GenAudioLevelCommand()        12 */

  'U', 'P', /* GenTextPosCommand()           13 */
  'U', 'Q', /* GenTextScaleCommand()         14 */
  'U', 'R', /* GenTextColorCommand()         15 */
  'U', 'T', /* GenTextMovCommand()           16 */
  'U', 'U', /* GenTextOnOffCommand()         17 */
  'U', 'Z', /* GenTextCommand()              18 */
  'U', 'N', /* JTAGMode()                    19 */
  'U', 'O'  /* ManualMode()                  20 */
  };

#include "io.c"


#if 1
#include "font.c"   /* text font, in separae file */
#endif

static UC TextBuffer[3][17];
static UC text_line_no = 0;

extern bit  transparent; /* indicates to serial interrupt routine */
                         /* that transparent mode is set and data */
                         /* for FPGA programming is comming       */
void (*uploader)();      /* function pointer declaration          */

/***************************************************************************/
/*  CommandExecute                                                 RS232.C */
/*                                                                         */
/* Author:    Janusz Kuzminski                                             */
/* Revised:   06.01.2006                                                   */
/*                                                                         */
/*  Function:  Decode and execute the received RS232 command.              */
/*  Remarks:   The commands are no longer executed via function pointers   */
/*             as this made it impossible to use overlaying under compila- */
/*             tion.  This function checks if 2-nd command code is equal   */
/*             to one of second commands codes in CommandTable[] and if it */
/*             is, appropriate function is executed.                       */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void CommandExecute()
  {
  UC i;

  for (i = 0; i < NOOFCOMMANDS; i++)
    if (Cmd[1] == CommandTable[i].Cmd2)
      break;

  switch (i)
    {
    case 0:
      UnitAddressCommand();
      break;
    case 1:
      WriteUserTextCommand();
      break;
    case 2:
      ReadUserTextCommand();
      break;
    case 3:
      ReadSWVersionCommand();
      break;
    case 4:
      ReadHWVersionCommand();
      break;
    case 5:
      ReadHWInformationCommand();
      break;
    case 6:
      GenSystemCommand(SYS_ADDR);
      break;
    case 7:
      GenPatternCommand(PAT_ADDR);
      break;
    case 8:
      GenAttribCommand(PAT_ADDR + 1);
      break;
    case 9:
      GenDelayCommand(DEL_ADDR);
      break;
    case 10:
      GenAudioClickTimeCommand(CLI_ADDR);
      break;
    case 11:
      GenAudioSignalCommand(SIG_ADDR);
      break;
    case 12:
      GenAudioLevelCommand(LEV_ADDR);
      break;
    case 13:
      GenTextPosCommand(HOR_ADDR);
      break;
    case 14:
      GenTextScaleCommand(SCA_ADDR);
      break;
    case 15:
      GenTextColorCommand(COL_ADDR);
      break;
    case 16:
      GenTextMovCommand(MOV_ADDR);
      break;
    case 17:
      GenTextOnOffCommand(TXTONOFF_ADDR);
      break;
    case 18:
      GenTextCommand(TXT_ADDR);
      break;
    case 19:
      JTAGMode();
      break;
    case 20:
      ManualMode();
      break;

    default:
      break;
    }
  }    

/******************************************************************
* NAME       void ManualMode(void)
* PURPOSE    To provide direct (manual) access to FPGA settings
*            memory and make a querry (querry works only with addresses
*            STAT_CHAN1 to STAT_GENL, because it is so implemented
*            in FPGA.
*            (UO command)
* 
* ARGUMENTS  None
*
* RETURNS    Nothing
*
* EXTERNS    None
*
* NOTES:     This function sends to the FPGA:
*            - address, contained in Parameter[1]
*            - data,    contained in Parameter[2]
*            allowing to change any setting, without using normal
*            commands defined in CommandTable[].  It is to be 
*            used in development stage, using the :FACTORY SCPI
*            functions of the form:
*
*            :FACT:V24C:ADDR BBU_1;
*             - direct all further communication to 8641 (PT8612) 
*               mounted at XA1,
*            :FACT:V24C:COMM 'UO',1,0,0;
*             - invokes this function with 1 in Parameter[0]:GENERATOR
*                                          0 in Paramerer[1]:ADDRESS
*                                          0 in Paramerer[2]:DATA
*            i.e. after the send function returns, generator 1
*            in FPGA will have 0 placed at address 0 in in its
*            settings RAM, thus system will be st to OFF.
*
*            Generator may be 1, 2, 4 or 8.  First 4 bits define
*            generator, by placing '1' at appropriate position.
*            In that way the byte containing generator number can
*            be used as simple selector in FPGA code.
*
*            If the address is STAT_CHAN1 to STAT_GENL, this function allows to 
*            make querry.  Note that normally the querry of a 
*            parameter is not possible, because querry may not
*            contain parameters (querry with parameters gives 
*            syntax error).  Only in this function, the querry
*            is implemented as follows:
*            If the address is in the above range, the FPGA is 
*            querried and the answer (1 byte) is placed in a static
*            dat variable.  Subsequent querry ('UO?') will send the 
*            result in dat to the user (as hex value).

*           The syntax is:
*           :FACT:V24C:COMM 'UO',Gen,Addr,0; (0 is dummy data)
*           :FACT:V24C:COMM 'UO?';           (standard querry)
*           This function's querry feature requires FPGA software
*           capable of 2 way communication.
*           Also note that when changin system via this function,
*           you must use values from hd_sys_table[], because
*           mainframe does it in that manner.
*           hd_sys_table[] is defined in unit_drv.c.
*           This function does not perform validation of arguments
*           arguments are used as suppllied by the user.
*           Note that when changing settings using this function
*           does not change settings in PT5300 Master.  This is 
*           also true when using the :FACT:V24C:COMM system.
******************************************************************/
void ManualMode(void)
  {
  UI addr;
  UC tmpb[10];
  static UC dat;

  switch (CmdDelimiter)
    {
    case ';': /* set */
      addr = (UI)Parameter[1];
      dat  = (UC)Parameter[2];
      receive_byte((UC)Parameter[0], addr, &dat);

    case '?': /* querry */
      /* send data in static variable dat */
      sprintf(tmpb, "0x%bX", (UC)dat);
      WriteString(tmpb);
      LED = ~LED;
      break;

    default:
      break;
    }
  }


/******************************************************************
* NAME       void JTAGMode(void)
* PURPOSE    To program FPGA with data sent from PT5300 Master
*            via serial interface. (UN command)
* 
* ARGUMENTS  None
*
* RETURNS    Nothing
*
* EXTERNS    transparent (flag indicating to normal serial interrupt
*                         routine that another routine is to be 
*                         used)
*
* NOTES:     This command does:
*            Reprograms UART to 115200 Baud and P2 for FPGA JTAG
*            functions:
*                       P2.7: TDO : input
*                       P2.6: TMS : output
*                       P2.5: TCK : output
*                       P2.4: TDI : output
*
*            Sets transparent flag to TRUE.  This means that 
*            serial interrupt routine calls serint0, which is 
*            separate JTAG programmer serial interrupt routine.
*
*            Runs xsvfExecute() function, which is JTAG programmer
*            routine and uses separate functions to fetch and send
*            characters (in module serial.c).
*
*            Data for xsvfExecute() are sent by the   DKPLAY.EXE
*            (running on a PC) to PT5300 Master, which operates in
*            transparent mode, i.e. each incoming character is sent
*            to PT8612 and each character sent by PT8612 is sent to
*            the PC.
*
******************************************************************/
void JTAGMode(void)
  {
  UC i;
  UC cnt;
  UC buff[60];

  ES0 = 0;

  delay();
  delay();
  delay();
  RI0 = 0;
  TI0 = 0;

  transparent = TRUE;

  /*----------------------------------------------------------------*/
  /*       Timers Configuration 115200 BR @ 11.059200 MHz           */
  /*----------------------------------------------------------------*/

  CKCON = 0x08;     /* Clock Control Register: Timer1 uses SYSCLK   */
  TL0   = 0x00;     /* Timer 0 Low Byte                             */
  TH0   = 0xFF;     /* Timer 0 High Byte                            */
  TL1   = 0x00;     /* Timer 1 Low Byte                             */
  TH1   = 0xD0;     /* BaudRate = 115200                            */
  TMOD  = 0x20;     /* Timer Mode Register                          */
  TCON  = 0x00;     /* Timer Control Register                       */
  /*----------------------------------------------------------------*/
  /*                UART0 Configuration                             */
  /*----------------------------------------------------------------*/
  SCON0 = 0x10;     /* Serial Port Control Register                 */
  PCON  = 0x00;     /* Power Control Register                       */

  TR1 = 1;          /* start timer1                                 */

  serinit();     /* zeroes pointers for new serial routine */

  /*config();*/
  XBR0 = 0x01;  // Crossbar Register 1
  XBR1 = 0x40;  // Crossbar Register 2 (weak pull-ups globally enabled)

/* New values for JTAG signals from P2 */
  P2MDOUT = 0xFC;   /* Output configuration for P2 (0x7C)           */
  P2MDIN  = 0xFC;   /* Input configuration for P2                   */
  P2SKIP  = 0x00;   /* Port 2 Crossbar Skip Register                */

  serinit();/* zeroes both pointers for new serial routine          */

  LED = 0;
  flush();  /* zeroes both pointers to serial buffer and fills the  */
            /* buffer with 0 */

  RI0 = 0;
  TI0 = 0;
  ES0 = 1;       /* Enable serial interrupt    */
  EA  = 1;
  cnt = 0;

#if 1
  while (1)
    {
    cnt++;
    flush();  /* zeroes both pointers to serial buffer and fills the  */
              /* buffer with 0 */
    while (1)
      {
      if (getch(&i))       /* wait for START from SERVER (transmits START) */
        {                  /* removes  START sent by master */
        /* The flush() below is not in order to flush, but to zero rx_outptr */
        flush(); 
        break;
        }
      else
        continue;
      }
  
    LED = 0;
    i = xsvfExecute();
    LED = 0;
  
    if (i != 0)
      {
      putch(ERR);
      LED = 0;
      }
    else
      {
      putch(SUCCESS);
      LED = 0;
      }
    break;
    }
#endif

  delay();
  delay();
  RI0 = 0;
  TI0 = 0;
  ES0 = 0;       /* Disable serial interrupt   */
  EA  = 0;
  uploader = (void code *) 0x3C00;
  uploader();
  }

/******************************************************************
* NAME       void GenSystemCommand(void)
* PURPOSE    To set format for given generator (UG command)
* 
* ARGUMENTS  UI addr
*
* RETURNS    Nothing
*
* EXTERNS    None
*
* NOTES: 
*         In Master software, the format is sent as follows:
*          'UL'<ul>,<ul>
*          
*  error = SndULUL(HDTPGUnit[subgen].Address, "UL", (UL)gen, (UL)fmt);
*          
*          i.e. generator nr and format are sent (no info about delay)
*          but delay is ALWAYS sent after format and attributes.
*          There is no check of validity of both parameters.
*          Potentialy dangerous!!

******************************************************************/
void GenSystemCommand(UI addr)
  {
  UC sys;

  switch (CmdDelimiter)
    {
    case ';': /* set system */
      /* sys = hd_sys_table[(int)Parameter[1]]; */
      /* Mainframe sends the correct value now instead of index */
      sys = (UC)Parameter[1];
      send((UC)Parameter[0], addr, &sys, 1);
      break;
    case '?': /* querry */
    default:
      break;
    }
  }


/******************************************************************
* NAME       void GenPatternCommand(void)
* PURPOSE    To set pattern for given generator (UH command)
* 
* ARGUMENTS  None
*
* RETURNS    Nothing
*
* EXTERNS    None
*
* NOTES: 
*            In Master software, the pattern is sent as follows:
*             'UH'<UI>,<UI>
*          
*      error = SndInt2(HDTPGUnit[ndx].Address, "UH", (UI)gen, (UI)pattern);
*          
*          i.e. generator nr, pattern and attributes are sent (no info about delay)
*          but delay is ALWAYS sent after format.
*          There is no check of validity of both parameters.
*          Potentialy dangerous!! 
*
*          void send(UC gen, UI addr, UC* dat, int cnt)
******************************************************************/
void GenPatternCommand(UI addr)
  {
  UC pat;

  switch (CmdDelimiter)
    {
    case ';':
      pat = (UC) Parameter[1];
      send((UC)Parameter[0], addr, &pat, 1);
      break;
    default:
      break;
    }
  }

/*   (UI command)                  */
void GenAttribCommand(UI addr)
  {
  UC at;

  switch (CmdDelimiter)
    {
    case ';':
      at = (UC)Parameter[1]; /* get text attributes */
      send((UC)Parameter[0], addr, &at, 1);
      break;
    default:
      break;
    }
  }


/******************************************************************
* NAME       void GenDelayCommand(void)
* PURPOSE    To set delay for given generator (UJ command)
* 
* ARGUMENTS  None
*
* RETURNS    Nothing
*
* EXTERNS    None
*
* NOTES: 
*            In Master software, the delay is sent as follows:
*             'UK' <ul>,<ul>
*          
*  error = SndULUL(HDTPGUnit[ndx].Address, "UK", (UL)ndx, (UL)tmp); /* delay
*          
*          i.e. generator nr and delay are sent (no info about format)
*          There is no check of validity of both parameters.
*          Potentialy dangerous!! 
*
* Problem: On the FGA side the protocol was changed so that only 3
*          bytes per delay are used and therefore received.  Because
*          our protocol transmits MSB first, the LSB byte of delay
*          parameter was lost.  So this function must send the 
*          following bytes of Delay parameter:
*          BYTE2 then BYTE1 and then BYTE0.  BYTE3 must not be sent!
*          Discovered on 20.04.2006.
*          We will use the following union to tackle this problem,
*          which is defined in define.h:
            unions for unsigned long and unsigned int
            to access these data types bytewise
            Note that for Kiel C compiler, charval[0]
            carries MSB and charval[3] carries LSB.
            This is the opposite of how TC does it.

              typedef union xx
                  {
                  long longval;
                  int  intgval[2];
                  UC   charval[4];
                  }  ul;
              
          charval[0] = BYTE3 = MSB
          charval[1] = BYTE2 =  .
          charval[2] = BYTE1 =  .
          charval[3] = BYTE0 = LSB   0x5AA30C

******************************************************************/
void GenDelayCommand(UI addr)
  {
  ul del;

  switch (CmdDelimiter)
    {
    case ';':
      del.longval = (UL)Parameter[1];
      send((UC)Parameter[0], addr, (UC*)&del.charval[1], 3);

      LED = ~LED;

      break;
    default:
      break;
    }
  }

/* (UK command)                               */
void GenAudioClickTimeCommand(UI addr)
  {
  UI click;

  switch (CmdDelimiter)
    {
    case ';':
      click = (UI)Parameter[1]; /* get click *was UC: WRONG!!!!*/
      send((UC)Parameter[0], addr, (UC*)&click, 2);
      break;
    default:
      break;
    }
  }

/* (UL command)                               */
void GenAudioSignalCommand(UI addr)
  {
  UC sig;

  switch (CmdDelimiter)
    {
    case ';':
      sig = (UC)Parameter[1]; /* get sig */
      send((UC)Parameter[0], addr, (UC*)&sig, 1);
      break;
    default:
      break;
    }
  }

/* (UM command)                             */
void GenAudioLevelCommand(UI addr)
  {
  UC lev;

  switch (CmdDelimiter)
    {
    case ';':
      lev = (UC)Parameter[1]; /* get sig */
      send((UC)Parameter[0], addr, (UC*)&lev, 1);
      break;
    default:
      break;
    }
  }


/******************************************************************
* NAME       void GenTextCommand(UI addr)
* PURPOSE    The purpose of this function is to receive 3 lines 
*            of text from the PT5300, to compose text bitmap and 
*            transfer it to the FPGA. (UZ command)
*
* ARGUMENTS  UI addr - address of FPGA chunk RAM where the bitmap
*                      is to be stored.
* RETURNS    Nothing
* EXTERNS    None
*
* NOTES: 
*            In Master software, the text is sent as follows:
*             'UZ'<uc>,<string>
*            i.e. command, gen_number and text.
*              <uc> carries generator number.
*              The <uc> is available here as
*              Parameter[0].  The <string> is available in global
*              buffer String.  
*            The master software sends all 3 lines of equal length,
*            lines shorter than the longest one are padded with 
*            spaces.
*            No info about line number is sent, but lines are always
*            sent in sequence, 1, 2 and 3.  We use global variable 
*            text_line_no to hold control over actual line number.
*
    while(*Str)
      {
      UC StrValue = *Str++;
      UC Font     = Font57[line_nr + (7 * (StrValue - ' '))];
      if (line_nr == 7)
        Font = 0;    

******************************************************************/
void GenTextCommand(UI addr)
  {
  UC gen;
  UI i;
  UC *Str;
  UC Font;
  UC line_nr;
  UC width;
  UC heigth;

  UC weigth;
  UC first;
  UC last;

  weigth = 0;

#if 0
  if (gen == 1)
    LED = ~LED;  
#endif

  gen    = (UC)Parameter[0];
  

  strcpy(TextBuffer[text_line_no], String);
  text_line_no++;
  if (text_line_no != 3)
    return;

  text_line_no = 0;

  /* calculate and send text size hor: in pixels, ver: in pixels */
  width  = strlen(TextBuffer[0]) * 8;

#if 1
  /* Lines with spaces only shall not be displayed */

  if (is_text(TextBuffer[0]))
    weigth |= 1;
  if (is_text(TextBuffer[1]))
    weigth |= 2;
  if (is_text(TextBuffer[2]))
    weigth |= 4;

  switch (weigth)
    {
    case 0:      /* spaces on all lines, send heigth = 0 and return */
      i = 0;
      send(gen, TXTONOFF_ADDR, &i, 1);    /* send TEXT OFF  */
      return;
      break;
    case 1:      /* only line 0 has text... */
      heigth = 9;
      first  = 0;
      last   = 1;
      break;
    case 2:      /* only line 1 has text... */
      heigth = 9;
      first  = 1;
      last   = 2;
      break;
    case 3:      /* line 0 and 1 have text... */
      heigth = 17;
      first  =  0;
      last   =  2;
      break;
    case 4:      /* only line 2 has text... */
      heigth = 9;
      first  = 2;
      last   = 3;
      break;
    case 5:      /* line 0 and 2 have text... line in between shal be removed...*/
      strcpy(TextBuffer[1], TextBuffer[2]);
      heigth = 17;
      first  =  0;
      last   =  2;
      break;
    case 6:      /* line 1 and 2 have text... */
      heigth = 17;
      first  =  1;
      last   =  3;
      break;
    case 7:      /* all lines have text... */
      heigth = 25;
      first  =  0;
      last   =  3;
      break;
    }
#else
      first  = 0;
      last   = 3;
      heigth = 25;
#endif

  send(gen, SIZ_ADDR_H, &width,  1);  /* send TEXT SIZE */
  send(gen, SIZ_ADDR_V, &heigth, 1);  /* send TEXT SIZE */

  /* Proto: void send(UC gen, UI addr, UC* dat, int cnt) */

  CS = 0;
  spi_transfer(gen, 8);
  spi_transfer(TXT_ADDR_HI, 8);       /* send TEXT ADDRESS MSB */
  spi_transfer(TXT_ADDR_LO, 8);       /* send TEXT ADDRESS LSB */

  /* compose the bitmap       */
  /* send one scan line blank */
  for (i = 0; i < strlen(TextBuffer[0]); i++)
    spi_transfer(0, 8);

  for (i = first; i < last; i++)              /* for 3 lines of text... */
    for (line_nr = 0; line_nr < 8; line_nr++) /* and 8 scan lines...    */
      {
      Str = &TextBuffer[i];
      while (*Str) /* compose the string itself */
        {
        Font = Font57[line_nr + (7 * (*Str++ - ' '))];
        Font = Font << 1;
        if (line_nr == 7)
          Font = 0;    
        spi_transfer(Font, 8);
        }
      }
  CS = 1;
  }

#if 1
/******************************************************************
* NAME       UC is_text(UC *buff)
* PURPOSE    To test if supplied string contains only spaces
* 
* ARGUMENTS  UC *buff: buffer with text
*
* RETURNS    1 - if text contains text
*            0 - if text contains only spaces
*
* EXTERNS    None
*
* NOTES: 
******************************************************************/
UC is_text(UC *buff)
  {
  
  while (*buff)
    {
    if (*buff != ' ')
      return (1);
    buff++;
    }
  return (0);
  }
#endif



/* (UQ command)                          */
void GenTextScaleCommand(UI addr)
  {
  UC scal;

  switch (CmdDelimiter)
    {
    case ';':
      scal = (UC)Parameter[1]; /* get scale */
      send((UC)Parameter[0], addr, (UC*)&scal, 1);
      break;
    default:
      break;
    }
  }

/* (UP command)                           */
void GenTextPosCommand(UI addr)
  {
  UI pos[2];

  switch (CmdDelimiter)
    {
    case ';':
      pos[0] = (UI)Parameter[1]; /* get X */
      pos[1] = (UI)Parameter[2]; /* get Y */
      send((UC)Parameter[0], addr, (UC*)&pos, 4);
      break;
    default:
      break;
    }
  }

/* (UR command)                            */
void GenTextColorCommand(UI addr)
  {
  UC col;

  switch (CmdDelimiter)
    {
    case ';':
      col = (UC)Parameter[1]; /* get color */
      send((UC)Parameter[0], addr, (UC*)&col, 1);
      break;
    default:
      break;
    }
  }


/* (UT command)                             */
void GenTextMovCommand(UI addr)
  {
  UC tm;

  switch (CmdDelimiter)
    {
    case ';':
      tm = (UC)Parameter[1]; /* get text movement */
      send((UC)Parameter[0], addr, (UC*)&tm, 1);
      break;
    default:
      break;
    }
  }

/* (UU command)                            */
void GenTextOnOffCommand(UI addr)
  {
  UC tm;

  switch (CmdDelimiter)
    {
    case ';':
      tm = (UC)Parameter[1]; /* get text onoff */
      send((UC)Parameter[0], addr, (UC*)&tm, 1);
      break;
    default:
      break;
    }
  }
/*******************************************************************
 * NAME       void WriteUserTextCommand()
 * PURPOSE    To write text string sent by 5300 Master into eeprom.
 *            (UB command)
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     10 characters are written, plus checksum.
 *            
 *             :FACT:V24C:ADDR BBU_1;COMM 'UB',2,'000000';
 *******************************************************************/
void WriteUserTextCommand()
  {

  if ( CmdDelimiter == ';')
    {
    if ((UC)Parameter[0] < 10)
      PageWriteE2prom(CONROL_BYTE, USER_TEXT + (11 * (UC)Parameter[0]), String, 10);
    }
  }

/*******************************************************************
 * NAME       void ReadUserTextCommand()
 * PURPOSE    To read string from eeprom and transmit it to 5300 Master.
 *            (UC command)
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     There is place for 10 strings, each containing 11
 *            characters (last one is checksum) in the eeprom.
 *            Parameter[0] contains nr of string to read.
 *               :FACT:V24C:ADDR BBU_1;COMM 'UC?',2;
 *******************************************************************/
void ReadUserTextCommand()
  {
  if (CmdDelimiter == '?')
    {
    if ((UC)Parameter[0] < 10)
      {
      SeqReadE2prom(CONROL_BYTE, USER_TEXT + (11 * (UC)Parameter[0]), String, 10);
      String[10] = '\0';
      WriteString(String); /* WriteString() adds quotes */
      }
    }
  }

/***************************************************************************/
/*  void ReadSWVersionCommand(void)                                        */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   17.07.2002                                                   */
/*                                                                         */
/*  Function: To return software version (UD command)                      */
/*                                                                         */
/*  Returns:  Nothing                                                      */
/*  Notes:                                                                 */
/*            8051F226 board                                               */
/***************************************************************************/
void ReadSWVersionCommand()
  {

  switch (CmdDelimiter)
    {
    case ';':
      break;
    case '?':
      WriteVal(SWVERSION);
      break;
    default:
      break;
    }
  }

/***************************************************************************/
/*  ReadHWVersionCommand                                                   */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    990304, KEn                                                 */
/* Function: To return hardware version (UE command)                       */
/***************************************************************************/
void ReadHWVersionCommand()
  {

  if (CmdDelimiter == '?')
    {

    WriteVal(HWVERSION);
    }
  }

/***************************************************************************/
/*  ReadHWInformationCommand                                               */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 971120                                    */
/* Revised:    990304, KEn                                                 */
/* Function: To return hardware information (UF command)                   */
/***************************************************************************/
void ReadHWInformationCommand()
  {

  if (CmdDelimiter == '?')
    {

    WriteVal(HWINFO);
    }
  }

/***************************************************************************/
/*  UnitAddressCommand                                                     */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:   980601, KEn                                                  */
/*                                                                         */
/*  Function: Set/read the unit slave address (UA command)                 */
/*                                                                         */
/*  Remarks:  This function is invoked during FindDynamicOptions procedure */
/*            in Master startup sequence, where master sends UA command    */
/*            with apropriate conn_addr parameter.  This function changes  */
/*            MySlaveAddress from DefaultHDTPG_addr to con_addr and echoes */
/*            the conn_addr back, so the master knows that board is mounted*/
/*                                                                         */
/*               Send 'set unit address to con_addr' command               */
/*            SndChar(DefaultHDTPG_addr, "UA", conn_addr);                 */
/*                                                                         */
/***************************************************************************/
void UnitAddressCommand(void)
  {

  switch (CmdDelimiter)
    {
    case ';':
      MySlaveAddress = (UC)Parameter[0];  /* Parameter[0] holds bbux_addr  */
      WriteChar(MySlaveAddress);          /* echo MySlaveAddress to Master */
      break;

    case '?':
      WriteVal((UL) MySlaveAddress);
      break;
    }
  }

