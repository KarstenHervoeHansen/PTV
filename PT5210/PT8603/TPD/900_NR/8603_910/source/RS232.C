/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PT8607 SDI Digital test signal generator              */
/*  Module:    RS232.C                                    */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  960703                                    */
/*  Rev. date:  980929, KEn  DEV                              */
/*  Status:    Version 1.1                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*      void RS232Init();                                  */
/*      void CharHandler();                                */
/*      void Command_Execute();                              */
/*      bit TestInputBuffer();                              */
/*      void WriteChar( char TxChar);                          */
/*      void WriteEndOfTx( char checksum);                      */
/*      UC WriteString( char* Str);                          */
/*      UC WriteValue( UL value);                            */
/*                                                  */
/* Changes:                                            */
/* 980929: Added new command 'SV'                            */
/*  970501: WriteValue has been rewritten                        */
/***************************************************************************/

/***************************************************************************/
/* Include files:                                        */

#include <xa.h>

#include "d1gener.h"
#include "rs232.h"
#include "d1cmd.h"
#include "d1util.h"

code char TypeTable[NoInASCII] = {
  /*   0 */  'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'D', 'R',
  /*  12 */  'R', 'D', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
  /*  24 */  'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'D', 'Q', 'R',
  /*  36 */  'D', 'R', 'R', 'R', 'R', 'R', 'R', 'N', 'S', 'N', 'N', 'R',
  /*  48 */  'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'R', 'D',
  /*  60 */  'R', 'R', 'R', 'D', 'R', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /*  72 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /*  84 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'R', 'R', 'R', 'R', 'R',
  /*  96 */  'R', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /* 108 */  'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
  /* 120 */  'A', 'A', 'A', 'R', 'R', 'R', 'R', 'R'
};

code struct COMMAND_STRUCT Command_Table[NoOfCommands] = {
  'S', 'A', Unit_Address_Command,
  'S', 'G', G_Offset_Command,
  'S', 'M', M_Offset_Command,
  'S', 'Q', Read_System_Command,
  'S', 'B', Read_Offset_Command,
  'S', 'O', Genlock_Offset_Command,
  'S', 'P', Pattern_Command,
  'S', 'E', EDH_Command,
  'S', 'U', Audio_Signal_Command,
  'S', 'L', Audio_Level_Command,
  'S', 'F', Text_ON_Command,
  'S', 'N', Text_Command,
  'S', 'S', Read_Status_Command,
  'S', 'X', G_Phase_Command,
  'S', 'Y', M_Phase_Command,
  'S', 'Z', Reset_Command,
  'S', 'W', Write_User_Text_Command,
  'S', 'R', Read_User_Text_Command,
  'S', 'I', Read_SW_Version_Command,
  'S', 'V', Read_HW_Version_Command,
  'S', 'T', Read_HW_Information_Command,
};

                  // Variables used to handle the input buffer
char RxBuffer[RxBufferLen];// Receiver buffer
UC RxInputSize;        // Var. to hold the number of characters in buffer
UC RxState;            // Receiver interrupt state machine
UC RxChkSum;          // Calculated receiver checksum

UC RxOutput;          // Pointer to the NEXT received char in the buffer
                  //  input buffer
UC RxInput;            // Pointer to the next FREE space in the input buffer

                  // Variables used when decoding the input buffer
UC BufState;          // Char handler state machine
UC Char;              // Char read from the input buffer
UC CharType;          // Received character type

char CmdNdx;          // Command index
char StrNdx;          // String index
char ParamNdx;          // Parameter index

char Cmd[4];          // Decoded command
char CmdDelimiter;      // Command delimiter for the decoded command

char String[StringLen];    // String parameter
UL Parameter[3];        // Three unsigned long parameters

                  // Variables used during transmission
UC Tx_ACK_Buffer;        // Buffer to hold acknowledge byte
UC TxChkSum;          // Checksum for the transmission

static bit Tx_ACK_Pending;  //
static bit Tx_BUSY;      //
static bit Tx_S0BUF_Empty;  //

/***************************************************************************/
/*  RS232Intr                                      RS232.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY      */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
banked interrupt void RS232Intr( void) {

  static char RxChar, RxCharType;

  while ( RI0) {                // If receiver interrupt ..

    if ( S0M2) {
      S0M2 = 0;

      RxState = DataState;        // Initialize state machine
      RxChkSum = 0;            // Reset Checksum
    }
    else {
      RxChar = S0BUF;          // Read the character

      if ( RxState == DelState) {  // Last byte, ie. checksum received ?..

        S0M2 = 1;

        if (( RxChkSum & 0x7F) - RxChar)
          Tx_ACK_Buffer = 0x81;  // Checksum ERROR
        else
          Tx_ACK_Buffer = 0x80;  // Checksum is OK

        RxChar = Tx_ACK_Buffer;

        if ( Tx_BUSY)
          Tx_ACK_Pending = TRUE;
        else {
          Tx_S0BUF_Empty = FALSE;
          S0BUF = Tx_ACK_Buffer;
        }
      }
      else {                // .. else we're still receiving data
        RxCharType = TypeTable[RxChar];

        if ( RxCharType == 'Q')
          RxState ^= XORValue;    // Toggle between Data- and QuoteState
        else {
          if ( RxState != QuoteState) {
            if ( RxCharType == 'D')
              RxState = DelState;
          }
        }
      }

      RxBuffer[ RxInput = ++RxInput % RxBufferLen] = RxChar;
      RxChkSum += RxChar;

      if ( ++RxInputSize > RxBufferLen)
        ErrorStatus |= Buffer_OV;
    }
    RI0 = 0;                  // Reset the interrupt
  }

  if ( TI0) {                  // If transmitter interrupt
    if ( Tx_ACK_Pending) {        // If transmitter acknowledge pending ..
      Tx_ACK_Pending = FALSE;
      S0BUF = Tx_ACK_Buffer;      // transmit acknowledge
    }
    else
      Tx_S0BUF_Empty = TRUE;

    TI0 = 0;                  // Reset the interrupt
  }
}

/***************************************************************************/
/*  RS232Init                                          RS232.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void RS232Init() {

  RxInputSize = 0;        // Number of characters in input buffer

  RxInput = 0;          // Pointer to NEXT character in input buffer
  RxOutput = 0;          // Pointer to FIRST character in output buffer

  RxState = IdleState;      // Receiver state machine

  Char = 0;            // Character read from output buffer
  CharType = 0;          // Type of character read from output buffer
  BufState = State_0;      // Command handler state machine

  Tx_ACK_Pending = FALSE;
  Tx_BUSY = FALSE;
  Tx_S0BUF_Empty = TRUE;
}

/***************************************************************************/
/*  CharHandler                                          RS232.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void CharHandler() {

  RxInputSize--;            // Decrement number of bytes in input buffer

                      // Get next char from output buffer
  Char = RxBuffer[ RxOutput = ++RxOutput % RxBufferLen];
                    
  CharType = TypeTable[Char];  // Determine type of the received character

  switch ( BufState) {

    case State_0:          // ***** BUFFER STATE 0 ******
      if ( CharType == 'A') {
        BufState++;

        Cmd[CmdNdx = 0] = Char;
        return;
      }

      if ( CharType != 'D')
        BufState = State_7;
      return;

    case State_1:          // ***** BUFFER STATE 1 ******
      switch ( CharType) {
        case 'A':
          Cmd[++CmdNdx] = Char;
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

    case State_2:          // ***** BUFFER STATE 2 ******
      switch ( CharType) {
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

    case State_3:          // ***** BUFFER STATE 3 ******
      if ( CharType == 'Q') {
        String[++StrNdx] = '\0';
        BufState++;
        return;
      }

      String[++StrNdx] = Char;
      break;

    case State_4:          // ***** BUFFER STATE 4 ******
      switch ( CharType) {
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

    case State_5:          // ***** BUFFER STATE 5 ******
      switch ( CharType) {
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

    case State_6:          // ***** BUFFER STATE 6 ******
      if ( Char == 0x80)
        Command_Execute();

      BufState = State_0;
      return;

    case State_7:
      if ( CharType == 'D')
        BufState++;
      return;      

    case State_8:
      BufState = State_0;    // Reset state-machine 
      return;      
  }
}

/***************************************************************************/
/*  Command_Execute                                      RS232.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void Command_Execute() {

  register int i;

  for ( i = 0; i < NoOfCommands; i++)
    if ( Cmd[0] == Command_Table[i].Cmd1)
      break;

  if ( i < NoOfCommands)
    for ( ; i< NoOfCommands; i++) {
      if ( Cmd[1] == Command_Table[i].Cmd2)
        break;        //Command found
    }

  if ( i < NoOfCommands) {
    if ( GeneratorRunning)
      Command_Table[i].Exec();
    else
      if (( Cmd[0] == 'S') && (( Cmd[1] == 'Z') || ( Cmd[1] == 'V')))
        Command_Table[i].Exec();
  }
  else
    ErrorStatus |= Illegal_Cmd;
}

/***************************************************************************/
/*  TestInputBuffer                                       RS232.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
bit TestInputBuffer() {

  if ( RxInput - RxOutput)
    return( TRUE);

  return( FALSE);
}

/***************************************************************************/
/*  WriteChar                                          RS232.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void WriteChar( char TxChar) {

  Tx_BUSY = TRUE;

  while ( !Tx_S0BUF_Empty)
    ;

  Tx_S0BUF_Empty = FALSE;

  S0BUF = TxChar;

  while ( !Tx_S0BUF_Empty)
    ;

  Tx_BUSY = FALSE;
}

/***************************************************************************/
/*  WriteEndOfTx                                        RS232.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
void WriteEndOfTx( char checksum) {

  WriteChar( ';');
  WriteChar(( checksum + ';') & 0x7F);
}

/***************************************************************************/
/*  WriteString                                          rs232.c */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970217, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
UC WriteString( char* Str) {

  UC checksum = 2*'\"';

  WriteChar( '\"');
  
  while ( *Str) {
    WriteChar( *Str);
    checksum += *Str++;
  }

  WriteChar( '\"');

  return( checksum);
}

/***************************************************************************/
/*  WriteValue                                          RS232.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970217                         */
/* Revised:    970501, KEn                                     */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:    --                                        */
/***************************************************************************/
UC WriteValue( UL value)
  {
  UC checksum = 0, i;
  UC tmp, ValOut = FALSE;
  UL divider = 1000000000;

  for ( i = 0; i < 10; i++)
    {
    if ( tmp = ( value / divider) % 10)
      ValOut = TRUE;

    if ( ValOut || ( i == 9))
      {
      tmp += '0';
      WriteChar( tmp);
      checksum += tmp;
      }  
    divider /= 10;
    }
  return( checksum);
  }
