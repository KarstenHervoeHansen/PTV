/***************************************************************************/
/*  Copyright DK-Audio A/S, 2005                                           */
/*  Project:  PT8641 HDTPG (PT8612), using 8051F320 Cygnal                 */
/*            processor.                                                   */
/*  Module:   ser_spi.C                                                    */
/*  Author:   Jnausz Kuzminski                                             */
/*  Date:     01.03.2006                                                   */
/*  Status:   Version 2.0                                                  */
/*  Purpose:  This module implements serial communication routines used to */
/*            communicate with PT5300 Master.  It also contains two        */
/*            software SPI functions for communication with the FPGA.      */
/***************************************************************************/
#pragma WARNINGLEVEL (1) /* Lists only those warnings which may generate   */
                         /* incorrect code. Default is 2.                  */
/***************************************************************************/

#include <C8051F320.h>
#include "define.h"
#include <string.h>
#include "ser_spi.h"
#include "cmd.h"
#include "eeprom.h"
#include "serial.h"

#define IDDLESTATE     0x00
#define DATASTATE      0x01
#define QUOTESTATE     0x02    /* MUST be DATASTATE XOR 03H */
#define DELSTATE       0x03

#define XORValue       0x03

/* use the condition below to allow debugging.  If DefaultHDTPG_addr = 0x13,
   then PT8641 program will work properly without the need to reset the Master
*/
#if 1
#define DefaultHDTPG_addr     (UC)0xF5    /* default SLAVE address for PT8641 */
#else
#define DefaultHDTPG_addr     (UC)0x11
#endif

#define PT8641_COMMAND_PREFIX  'U'    /* First letter of PT8641 command */
                                      /* set */

enum CommandStates
  { 
  State_0 = 0,
  State_1,
  State_2,
  State_3,
  State_4,
  State_5,
  State_6,
  State_7,
  State_8
  };

#define NoInASCII     128

/* In the table below:
   D is delimiter and has following values:
                                           0x0A - LF
                                           0x0D - CR
                                           0x24 - $
                                           0x3B - ;
                                           0x3F - ?

   S is separator                          0x2C - ,

   N is ASCII number:
                                           0x2B - +
                                           0x2D - -
                                           0x2E - .
                                           0x30 - 0
                                           0x31 - 1
                                           ........
                                           0x39 - 9

   A is ASCII letter:                      0x41 - 0x4A and
                                           0x61 - 0x7F

   Q is quote                              0x22

   R is all the rest, control character.

   So, the table below maps all ASCII chars to a set of chartypes.
*/
code char TypeTable[NoInASCII] =
   {
  /*           0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
  /* 0x00 */  'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'D', 'R', 'R', 'D', 'A', 'R',
  /* 0x10 */  'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
  /* 0x20 */  'R', 'D', 'Q', 'R', 'D', 'R', 'R', 'R', 'R', 'R', 'R', 'N', 'S', 'N', 'N', 'R',
  /* 0x30 */  'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'R', 'D', 'R', 'R', 'R', 'D',
  /* 0x40 */  'R', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /* 0x50 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'R', 'R', 'R', 'R', 'R',
  /* 0x60 */  'R', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /* 0x70 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'
  };

UC RxState;
UC RxChkSum;

/* These 2 are out and in pointers to RxBuffer */
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

#define RXRLEN      50

char RxBuffer[RXRLEN];
char String[10]; /* used to hold string, if sent by master */
UL   Parameter[3];

UC   Tx_ACK_Buffer;
bit  Tx_ACK_Pending;
bit  Tx_BUSY;          /* What is the difference between Tx_BUSY and Tx_SBUF_Empty */
bit  Tx_SBUF_Empty;

bit  Broadcast;

bit  transparent;

#include "io.c"

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
  MySlaveAddress   = DefaultHDTPG_addr; /* default SLAVE address for PT8641 */
  CS          = 1;
  RxInput     = 0; 
  RxOutput    = 0;
  RxState     = IDDLESTATE;
  Char        = 0;
  CharType    = 0;
  BufState    = State_0;
  Broadcast   = FALSE;
  Tx_BUSY     = FALSE;
  Tx_ACK_Pending = FALSE;
  Tx_SBUF_Empty  = TRUE;
  transparent    = FALSE;
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
/*
Multiprocessor Communications

Modes 2 and 3 support multiprocessor communication 
between a master processor and one or more slave 
processors by special use of the ninth data bit. 
When a master processor wants to transmit to one 
or more slaves, it first sends an address byte to 
select the target(s). An address byte differs from
a data byte in that its ninth bit is logic 1; 
in a data byte, the ninth bit is always set to logic 0.

Setting the MCE0 bit (SCON.5) of a slave processor 
configures its UART such that when a stop bit is 
received, the UART will generate an interrupt only 
if the ninth bit is logic one (RB8 = 1) signifying 
an address byte has been received. In the UART's
interrupt handler, software will compare the received 
address with the slave's  assigned 8-bit address. 
If the addresses match, the slave will clear its MCE0 bit 
to enable interrupts on the reception of the following 
data byte(s). 

Slaves that weren't addressed leave their MCE0 bits 
set and do not generate interrupts on the reception 
of the following data bytes, thereby ignoring the data. 
Once the entire message is received, the addressed slave 
resets its MCE0 bit to ignore all transmissions until it 
receives the next address byte.

Multiple addresses can be assigned to a single slave 
and/or a single address can be assigned to multiple slaves,
thereby enabling inbroadcastls transmissions to more than 
one slave simultaneously. The master processor can be
configured to receive all transmissions or a protocol can 
be implemented such that the master/slave role is
temporarily reversed to enable half-duplex transmission 
between the original master and slave(s).
*/
/***************************************************************************/
void RS232break(void) interrupt 4
  {
  static char RxChar, RxCharType;

  if (transparent)
    {
    serint0();
    }
  else
    {
    while (RI0)
      {
      if (MCE0)
        {
        if (SBUF0 == 0x90) /* Broadcast: all slaves listen, 0x90 must be bradcast address */
          {
          Broadcast = TRUE; /* prepare to receive WITHOUT an acknowledge in the end */
  
          MCE0       = 0;
          RxState   = DATASTATE;
          RxChkSum  = 0;
          }
        else
          if (SBUF0 == MySlaveAddress) /* Only this slave listens */
            {
            MCE0      = 0;             /* Cancel multiprocessor comms */
            RxState  = DATASTATE;
            RxChkSum = 0;
            }
        } /* if (MCE0) */
      else
        {
        RxChar = SBUF0;
        if (RxState == DELSTATE)
          {
          MCE0 = 1;                    /* End of data, Enable multiprocessor comms */
          if ((RxChkSum & 0x7F) - RxChar)
            {
            Tx_ACK_Buffer = 0x81;  /* checksum error */
            }
          else
            {
            Tx_ACK_Buffer = 0x80;  /* checksum OK    */
            }
          RxChar = Tx_ACK_Buffer;  /******************/
          if (!Broadcast)      /* DO NOT TRANSMIT ACKNOWLEDGE FOR A BROADCAST MESSAGE!!!!*/
            {
            if (Tx_BUSY)
              Tx_ACK_Pending = TRUE;/* Signal that ACK will be sent later */
            else
              {
              Tx_SBUF_Empty = FALSE;
              SBUF0 = Tx_ACK_Buffer; /* Send ACK = checksum_state */
              }
            }
          else /* Broadcast: do nothing */
            Broadcast = FALSE;
          } /* if (RxState == DELSTATE) */
        else
          { 
          RxCharType = TypeTable[RxChar];
          if (RxCharType == 'Q')
            RxState ^= XORValue;
          else
            {
            if (RxState != 0x02)     /* ???  (State_2 ?) or (QUOTESTATE = 0x02) */
              {
              if (RxCharType == 'D') /* D is delimiter  */
                RxState = DELSTATE;  /* DELSTATE = 0x03 */
              }
            }
          }
        RxBuffer[RxInput = ++RxInput % RXRLEN] = RxChar; /* store char */
        RxChkSum += RxChar;
        } /* if (!MCE0) */
      RI0 = 0;
      }
  
    if (TI0)
      {
      if (Tx_ACK_Pending)
        {
        Tx_ACK_Pending = FALSE;
        SBUF0 = Tx_ACK_Buffer;
        }
      else
        Tx_SBUF_Empty = TRUE;
      TI0 = 0;
      }
    }
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
  SBUF0 = TxChar;
  while (!Tx_SBUF_Empty)
    ;
  Tx_BUSY = FALSE;
  }

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

/******************************************************************
 * NAME       void WriteString(UC *Str)
 * PURPOSE    To transmit string to master
 * 
 * ARGUMENTS  UC *Str - NULL terminated string
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None
 *
 * NOTES:     Text of the string is transmitted in quotes.
 *            Checksum includes these quotes.
 ******************************************************************/
void WriteString(UC *Str)
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
  Char = RxBuffer[RxOutput = ++RxOutput % RXRLEN];
                    
  CharType = TypeTable[Char];

  switch (BufState)
    {
    case State_0:
      if (CharType == 'A') /* Ascii letter */
        {
        BufState++;
        Cmd[0] = Char;
        ParamNdx = 0;
        Parameter[0] = Parameter[1] = Parameter[2] = 0;
        return;
        }
      if (CharType != 'D') /* Delimiter */
        BufState = State_7;
      return;
    case State_1:
      switch (CharType)
        {
        case 'A': /* Ascii letter */
          Cmd[1] = Char;
          return;
        case 'D': /* Delimiter    */
          CmdDelimiter = Char;
          BufState = State_6;
          return;
        case 'Q': /* Quote        */
          StrNdx = -1;
          BufState = State_3;
          return;
        case 'N': /* Number       */
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
        case 'N': /* Number       */
          Parameter[ParamNdx] = 10*Parameter[ParamNdx] + Char - '0';
          return;
        case 'S': /* Separator    */
          BufState = State_5;
          return;
        case 'D': /* Delimiter    */
          CmdDelimiter = Char;
          BufState = State_6;
          return;
        default:
          BufState = State_7;
        }
      break;
    case State_3:
      if (CharType == 'Q') /* Quote     */
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
        case 'Q': /* Quote     */
          String[StrNdx] = Char;
          BufState = State_3;
          break;
        case 'S': /* Separator */
          BufState++;
          break;
        case 'D': /* Delimiter */
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
        case 'Q':  /* Quote     */
          StrNdx = -1;
          BufState = State_3;
          break;
        case 'N': /* Number    */
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
        if (Cmd[0] == PT8641_COMMAND_PREFIX)  /* if (Cmd[0] == 'U') */
          CommandExecute();
        }
      BufState = State_0;
      return;
    case State_7:
      if (CharType == 'D') /* Delimiter */
        BufState++;
      return;      
    case State_8:
      BufState = State_0;
      return;      
    }
  }

/******************************************************************
 * NAME       UC spi_transfer (UC spi_byte, UC cnt)
 * PURPOSE    To transfer data bits using SPI protocol
 * 
 * ARGUMENTS  UC spi_byte - data byte  to be transferred
 *            UC cnt      - nr of bits to be transferred
 *
 * RETURNS    byte received simultaneously 
 *
 * EXTERNS    None
 *
 * NOTES:     This function wil serialize cnt bits of spi_byte
 *            and send them via MOSI, MSB bit first.
 *            If cnt is less than 8, make sure that the bits to 
 *            send are already shifted to the left before 
 *            calling this function.
 *            Slave select signals are not handled by this function.
 ******************************************************************/
UC spi_transfer (UC spi_byte, UC cnt)
  {
  UC i;

  for (i = 0; i < cnt; i++)
    {
    SCK = 0x01;

    if (spi_byte & 0x80)
      MOSI = 1;
    else
      MOSI = 0;
    spi_byte = spi_byte << 1;
    SCK = 0x00;
    spi_byte |= MISO;
    }
  return (spi_byte);
  }

/******************************************************************
 * NAME       void send(UC gen, UI addr, UC* dat, int cnt)
 * PURPOSE    To send data packet to FPGA.
 *
 * ARGUMENTS  UC gen  - generator number
 *            ui addr - FPGA chunk RAM storage address
 *            UC *dat - pointer to data
 *            int cnt - data length
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None
 *
 * NOTES:     
 ******************************************************************/
void send(UC gen, UI addr, UC* dat, int cnt)
  {
  UC i;
  ui j;

  CS = 0;
  j.intval = addr;
  spi_transfer(gen, 8);          /* generator number */
  spi_transfer(j.bytes[0], 8);   /* address MSB      */
  spi_transfer(j.bytes[1], 8);   /* address LSB      */

  for (i = 0; i < cnt; i++)
    {
    spi_transfer(*(dat++), 8);
    }
  CS = 1;
  }



/******************************************************************
 * NAME       void receive_byte(UC gen, UI addr, UC* dat)
 * PURPOSE    To receive data byte from FPGA.
 *
 * ARGUMENTS  UC gen  - generator number
 *            ui addr - FPGA chunk RAM storage address
 *            UC *dat - pointer to received data
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None
 *
 * NOTES:     This function works with the following addresses only:
 *
 *              #define   STAT_CHAN1        128
 *              #define   STAT_CHAN2        129
 *              #define   STAT_CHAN3        130
 *              #define   STAT_CHAN4        131
 *              #define   STAT_PLL          132
 *              #define   STAT_GENL         133
 *            due to such implementation of the communication 
 *            process in the FPGA.  All other addresses return
 *            some dummy data.
 *******************************************************************/
void receive_byte(UC gen, UI addr, UC* dat)
  {
  UC i;
  ui j;
  UC tmp;

  tmp = *dat;

  CS = 0;
  j.intval = addr;
  spi_transfer(gen, 8);          /* generator number */
  spi_transfer(j.bytes[0], 8);   /* address MSB      */
  spi_transfer(j.bytes[1], 8);   /* address LSB      */

  *dat = spi_transfer(tmp, 8);
  CS = 1;
  }
