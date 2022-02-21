/***************************************************************************/
/*  Copyright DK-Audio A/S, 2003                                           */
/*  Project:  PT8640 Trilevel HDTV generator, using 8051F231 Cygnal        */
/*            processor.                                                   */
/*  Module:   RS232.C                                                      */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     15.10.2003                                                   */
/*  Status:   Version 1.0                                                  */
/*  Purpose:  This module implements serial communication routines used to */
/*            communicate with PT5230 Master.  The following command set   */
/*            is implemented:                                              */
/*               Function           Command       Response                 */
/*                                                                         */           
/*             Set unit address      TA <uc>;       <uc>;                  */
/*             Read unit address     TA ?           <uc>;                  */
/*                                                                         */
/*                                                                         */
/*             Write User Text       TW <uc>,<str[<11]>                    */
/*                                                                         */
/*             Read User Text        TR <uc>?                              */
/*                                                                         */
/*             Read HW vers          TH ?           <ui>;                  */
/*                                                                         */
/*             Read SW vers          TI ?           <ui>;                  */
/*                                                                         */
/*             Read HW info          TT ?           <ui>;                  */
/*                                                                         */
/*             Read TLG samples      TJ ?           <ul>;                  */
/*                                                                         */
/*             The above command set is similar to that of PT9633 SDI TPG  */ 
/*             described in commands.doc.                                  */
/*                                                                         */
/* Changes:                                                                */
/* 15.10.03: This module is taken from 8633 source code, as is.  It was    */
/*           modified only to fit 8051F231 definitions.                    */
/*  Note:    F231 has 8K Flash and 255 bytes of RAM and no  XRAM           */
/*                                                                         */
/*           To issue these commands from an external PC connected to      */
/*           Master, we use the following FACTORY commands:                */
/*           :FACT:V24C:ADDR BBU_x;COMM 'TH?';   to get HW version         */
/*           :FACT:V24C:ADDR BBU_x;COMM 'TW',1,'string'; to write u.s 1    */
/*           :FACT:V24C:ADDR BBU_x;COMM 'TR?',1; to get user string 1      */
/*           ... etc.                                                      */
/*           We use BBU_x because the TLG after it is found to be          */
/*           installed, gets its address changed to bbux_addr              */
/*                                                                         */
/*      KU number is read by master as string as follows:                  */
/*      ascPtr = RecStr(address, "HR", ProductKUNo_addr/10, &error);       */
/*      where ProductKUNo_addr/10 = 2.                                     */
/*      KU number is stored as user string nr. 2.                          */
/***************************************************************************/


#include "f200.h"
#include "define.h"
#include "rs232.h"
#include "spi_mod1.h"
#include "eeprom.h"


#define IdleState              0x00
#define DataState              0x01
#define QuoteState             0x02    /* MUST be DataState XOR 03H */
#define DelState               0x03

/* use the condition below to allow debugging.  If DefaultTLG_addr = 0x13,
   then TLG program will work properly without the need to reset the Master
*/
#if 1
#define DefaultTLG_addr        (UC)0xF4    /* default SLAVE address for TLG */
#else
#define DefaultTLG_addr        (UC)0x12
#endif

#define PT8640_COMMAND_PREFIX  'T'    /* First letter of PT8640 command */
                                      /* set */

  /* User text start address: 10 blocks of 11 byte ( 10 data + checksum) */
#define USER_TEXT              0x0B

#define SWVERSION              10
#define HWVERSION               0
#define HWINFO                 11

code char TypeTable[NoInASCII] =
   {
  /*   0 */  'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'D', 'R',
  /*  12 */  'R', 'D', 'A', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
  /*  24 */  'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'D', 'Q', 'R',
  /*  36 */  'D', 'R', 'R', 'R', 'R', 'R', 'R', 'N', 'S', 'N', 'N', 'R',
  /*  48 */  'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'R', 'D',
  /*  60 */  'R', 'R', 'R', 'D', 'R', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /*  72 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /*  84 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'R', 'R', 'R', 'R', 'R',
  /*  96 */  'R', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /* 108 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /* 120 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'
  };

#define NOOFCOMMANDS 8

code struct COMMAND_STRUCT CommandTable[NOOFCOMMANDS] =
  {
  'T', 'A', UnitAddressCommand,
  'T', 'W', WriteUserTextCommand,
  'T', 'R', ReadUserTextCommand,
  'T', 'I', ReadSWVersionCommand,
  'T', 'H', ReadHWVersionCommand,
  'T', 'T', ReadHWInformationCommand,
  'T', 'K', GeneratorDelayCommand, /* The last two are new and general */
  'T', 'L', GeneratorFormatCommand /* commands to tackle format and    */
  };                               /* delay for all generators         */



UC RxState;
UC RxChkSum;

UC RxOutput;
UC RxInput;

UC BufState;
UC Char;
UC CharType;

char StrNdx;
char ParamNdx;

UC   MySlaveAddress;

char Cmd[2];
char CmdDelimiter;

#define RXRLEN      20

char idata RxBuffer[RXRLEN];
char idata String[15];
UL   Parameter[3];

UC   Tx_ACK_Buffer;
bit  Tx_ACK_Pending;
bit  Tx_BUSY;
bit  Tx_SBUF_Empty;

bit  Broadcast;
sbit LED = P2^4;

/* extern CURR_PARAMS gen_params; */
extern CURR_PARAMS tlgs[4];
extern bit         changed;
extern UC          current_generator;
extern ul          tlg_samples;

/***************************************************************************/
/*  RS232Init                                                      RS232.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    980601, KEn                                                 */
/*                                                                         */
/*  Function:  Initialize the RS232 communication.                         */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void RS232Init()
  {
  MySlaveAddress   = DefaultTLG_addr; /* default SLAVE address for TLG */
  RxInput     = 0; 
  RxOutput    = 0;
  RxState     = IdleState;
  Char        = 0;
  CharType    = 0;
  BufState    = State_0;
  Broadcast   = FALSE;
  Tx_BUSY     = FALSE;
  Tx_ACK_Pending = FALSE;
  Tx_SBUF_Empty  = TRUE;
  }


/***************************************************************************/
/*  CharHandler                                                    RS232.C */
/*                                                                         */
/* Author:     Kim Engedahl, DEV, 970217                                   */
/* Revised:    980601, KEn                                                 */
/*                                                                         */
/*  Function:  RS232 command handler.                                      */
/*  Remarks:   NOTE!  The broadcast messages are handled specially.        */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void CharHandler()
  {

  Char = RxBuffer[ RxOutput = ++RxOutput % RXRLEN];
                    
  CharType = TypeTable[Char];

  switch (BufState)
    {
    case State_0:
      if (CharType == 'A')
        {
        BufState++;
        Cmd[0] = Char;
        ParamNdx = 0;
        Parameter[0] = Parameter[1] = Parameter[2] = 0;
        return;
        }
      if (CharType != 'D')
        BufState = State_7;
      return;
    case State_1:
      switch (CharType)
        {
        case 'A':
          Cmd[1] = Char;
          return;
        case 'D':
          CmdDelimiter = Char;
          BufState = State_6;
          return;
        case 'Q':
          StrNdx = -1;
          BufState = State_3;
          return;
        case 'N':
          Parameter[ParamNdx = 0] = Char - '0';
          BufState = State_2;
          return;
        default:
          BufState = State_7;
        }
      break;
    case State_2:
      switch (CharType)
        {
        case 'N':
          Parameter[ParamNdx] = 10*Parameter[ParamNdx] + Char - '0';
          return;
        case 'S':
          BufState = State_5;
          return;
        case 'D':
          CmdDelimiter = Char;
          BufState = State_6;
          return;
        default:
          BufState = State_7;
        }
      break;
    case State_3:
      if (CharType == 'Q')
        {
        String[++StrNdx] = '\0';
        BufState++;
        return;
        }
      String[++StrNdx] = Char;
      break;
    case State_4:
      switch (CharType)
        {
        case 'Q':
          String[StrNdx] = Char;
          BufState = State_3;
          break;
        case 'S':
          BufState++;
          break;
        case 'D':
          CmdDelimiter = Char;
          BufState = State_6;
          break;
        default:
          BufState = State_7;
          return;
        }
      break;
    case State_5:
      switch (CharType)
        {
        case 'Q':
          StrNdx = -1;
          BufState = State_3;
          break;
        case 'N':
          Parameter[++ParamNdx] = Char - '0';
          BufState = State_2;
          break;
        default:
          BufState = State_7;
          return;
        }
      break;
    case State_6:
      if (Char == 0x80) /* this means that there was no chksum error */
        {
        if (Cmd[0] == PT8640_COMMAND_PREFIX)  /* if (Cmd[0] == 'T') */
          CommandExecute();
        }
      BufState = State_0;
      return;
    case State_7:
      if (CharType == 'D')
        BufState++;
      return;      
    case State_8:
      BufState = State_0;
      return;      
    }
  }
 
/***************************************************************************/
/*  CommandExecute                                                 RS232.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    980601, KEn                                                 */
/*                                                                         */
/*  Function:  Decode and execute the received RS232 command.              */
/*  Remarks:   NOTE! The broadcast messages are NOT decoded in this        */
/*             function, UNLESS they are called as NORMAL commands.        */
/*             Is is assumed the the first character HAS been tested AND   */
/*             found valid BEFORE entering this function.                  */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void CommandExecute()
  {
  UI i;

  for (i = 0; i < NOOFCOMMANDS; i++)
    if (Cmd[1] == CommandTable[i].Cmd2)
      {
      CommandTable[i].Exec();
      return;
      }
  }

/***************************************************************************/
/*  WriteChar                                                      RS232.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    980601, KEn                                                 */
/*                                                                         */
/*  Function:  Write one character to the RS232 output buffer.             */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void WriteChar(char TxChar)
  {
  Tx_BUSY = TRUE;
  while (!Tx_SBUF_Empty)
    ;
  Tx_SBUF_Empty = FALSE;
  SBUF = TxChar;
  while (!Tx_SBUF_Empty)
    ;
  Tx_BUSY = FALSE;
  }

#if 1
/******************************************************************
 * NAME       void WriteString(UC idata *Str)
 * PURPOSE    To transmit string to master
 * 
 * ARGUMENTS  UC idata *Str - NULL terminated string
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None
 *
 * NOTES:     Text of the string is transmitted in quotes.
 *            Checksum includes these quotes.
 ******************************************************************/
void WriteString(UC idata *Str)
  {
  UC checksum = 2*'\"';

  WriteChar('\"');
  
  while (*Str)
    {
    WriteChar(*Str);
    checksum += *Str++;
    }
  WriteChar('\"');
  WriteChar(';');
  WriteChar((checksum + ';') & 0x7F);

  }
#endif
/***************************************************************************/
/*  WriteVal                                                       RS232.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    980601, KEn                                                 */
/*                                                                         */
/*  Function:  Write a value to the RS232 output buffer.                   */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void WriteVal(UL value)
  {
  UC checksum, i;
  UC tmp, ValOut;
  UL divider;

  ValOut = FALSE;
  checksum = 0;

  divider = 1000000000;

  for (i = 0; i < 10; i++)
     {
     if (tmp = (value / divider) % 10)
       ValOut = TRUE;
     if (ValOut || (i == 9))
       {
       tmp += '0';
       WriteChar(tmp);
       checksum += tmp;
       }  
    divider /= 10;
    }
  WriteChar(';');
  WriteChar((checksum + ';') & 0x7F);
  }

/***************************************************************************/
/*  TestInputBuffer                                                RS232.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:    980601, KEn                                                 */
/*  Function:  Test if any more date in the input buffer.                  */
/*  Remarks:    --                                                         */
/*  Returns:    TRUE: If any more data, otherwise FALSE.                   */
/*  Updates:    --                                                         */
/***************************************************************************/
bit TestInputBuffer()
  {
  if (RxInput - RxOutput)
    return(TRUE);
  return(FALSE);
  }



/*******************************************************************
 * NAME       void WriteUserTextCommand()
 * PURPOSE    To write text string sent by 5230 Master into eeprom.
 *            < TW > command
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     10 characters are written, plus checksum.
 *            
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
 * PURPOSE    To read string from eeprom and transmit it to 5230 Master.
 *            < TR > command
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     There is place for 10 strings, each containing 11
 *            characters (last one is checksum) in the eeprom.
 *            Parameter[0] contains nr of string to read.
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
/*  Function: To return software version (TI command)                      */
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
#if 1
      WriteVal(SWVERSION);
#endif
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
/* Function: To return hardware version (TH command)                       */
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
/* Function: To return hardware information (TT command)                   */
/***************************************************************************/
void ReadHWInformationCommand()
  {

  if (CmdDelimiter == '?')
    {

    WriteVal(HWINFO);
    }
  }

/* GeneratorDelayCommand(): sets delay for x generator.
   TK <uc>,<ul>
   In Master software, the delay is sent as follows:

   error = SndULUL(TLGUnit[gen].Address, cmdBuffer, (UL)gen, tmp);

   i.e. generator nr and delay are sent (no info about format)
*/
void GeneratorDelayCommand(void)
  {
  switch (CmdDelimiter)
    {
    case ';':
      tlgs[(UC)Parameter[0]].delay.longval = Parameter[1];
      current_generator = (UC)Parameter[0];
      changed           = 1;
#if 0              /* GeneratorDelayCommand() will signal change          */
      LED = ~LED;  
#endif
      break;
    default:
      break;
    }
  }
/* GeneratorFormatCommand(): sets format for x generator.
   TL <uc>,<uc>
   In Master software, the delay is sent as follows:

   error = SndULUL(TLGUnit[subgen].Address, "TL", (UL)subgen, (UL)fmt);

   i.e. generator nr and format are sent (no info about delay)
*/
void GeneratorFormatCommand(void)
  {
  switch (CmdDelimiter)
    {
    case ';':
      tlgs[(UC)Parameter[0]].format = (UC) Parameter[1];
#if 0              /* GeneratorDelayCommand() will signal change          */
      LED = ~LED;  
#endif
      break;
    default:
      break;
    }
  }

/***************************************************************************/
/*  UnitAddressCommand                                                     */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:   980601, KEn                                                  */
/*                                                                         */
/*  Function: Set/read the unit slave address (TA command)                 */
/*  Remarks:                                                               */
/*                                                                         */
/***************************************************************************/
void UnitAddressCommand(void)
  {

  switch (CmdDelimiter)
    {
    case ';':
      MySlaveAddress = (UC)Parameter[0];  /* Parameter[0] holds bbux_addr  */
#if 0
      if (MySlaveAddress == 0x13)
        LED = ~LED; 
#endif
      WriteChar(MySlaveAddress);      /* echo MySlaveAddress to Master */
      WriteChar(MySlaveAddress);      /* echo MySlaveAddress to Master */
      break;

    case '?':
      WriteVal((UL) MySlaveAddress);
      break;
    }
  }
/***************************************************************************/
/*  RS232break                                                     RS232.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970217                                    */
/* Revised:   980601, KEn                                                  */
/*                                                                         */
/*  Function:  Transmitter/receiver interrupt handler.                     */
/*  Remarks:   DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void RS232break(void) interrupt 4
  {
  static char RxChar, RxCharType;

  while (RI)
    {
    if (SM2)
      {
      if (SBUF == 0x90)
        {
        Broadcast = TRUE;
        SM2       = 0;
        RxState   = DataState;
        RxChkSum  = 0;
        }
      else
        if (SBUF == MySlaveAddress)
          {
          SM2      = 0;
          RxState  = DataState;
          RxChkSum = 0;
          }
      }
    else
      {
      RxChar = SBUF;
      if (RxState == DelState)
        {
        SM2 = 1;
        if ((RxChkSum & 0x7F) - RxChar)
          {
          Tx_ACK_Buffer = 0x81;          /* checksum error */
          }
        else
          {
          Tx_ACK_Buffer = 0x80;          /* checksum OK    */
          }
        RxChar = Tx_ACK_Buffer;
        if (!Broadcast)
          {
          if (Tx_BUSY)
            Tx_ACK_Pending = TRUE;
          else
            {
            Tx_SBUF_Empty = FALSE;
            SBUF = Tx_ACK_Buffer;
            }
          }
        else
          Broadcast = FALSE;
        }
      else
        { 
        RxCharType = TypeTable[RxChar];
        if (RxCharType == 'Q')
          RxState ^= XORValue;
        else
          {
          if (RxState != 0x02)
            {
            if (RxCharType == 'D')
              RxState = DelState;
            }
          }
        }
      RxBuffer[ RxInput = ++RxInput % RXRLEN] = RxChar;
      RxChkSum += RxChar;
      }
    RI = 0;
    }

  if (TI)
    {
    if (Tx_ACK_Pending)
      {
      Tx_ACK_Pending = FALSE;
      SBUF = Tx_ACK_Buffer;
      }
    else
      Tx_SBUF_Empty = TRUE;
    TI = 0;
    }
  }

