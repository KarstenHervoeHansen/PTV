/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		RS232.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	981002, KEn, DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*			void RS232Init();																	*/
/*			void CharHandler();																*/
/*			void CommandExecute();															*/
/*			bit TestInputBuffer();															*/
/*			void WriteChar( char TxChar);													*/
/*			void WriteEndOfTx( char checksum);											*/
/*			UC WriteString( char* Str);													*/
/*			UC WriteValue( UL value);														*/
/*																									*/
/* Changes:																						*/
/* 981002: Released in SW ver. 3.0														*/
/* 980603: Released in SW ver. 2.0														*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>

#include "version.h"

#include "d1gener.h"
#include "rs232.h"
#include "d1cmd.h"

code char TypeTable[NoInASCII] = {
	/*   0 */	'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'D', 'R',
	/*  12 */	'R', 'D', 'A', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R',
	/*  24 */	'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'D', 'Q', 'R',
	/*  36 */	'D', 'R', 'R', 'R', 'R', 'R', 'R', 'N', 'S', 'N', 'N', 'R',
	/*  48 */	'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'N', 'R', 'D',
	/*  60 */	'R', 'R', 'R', 'D', 'R', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	/*  72 */	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	/*  84 */	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'R', 'R', 'R', 'R', 'R',
	/*  96 */	'R', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	/* 108 */	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A',
	/* 120 */	'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'
};

code struct COMMAND_STRUCT CommandTable[NoOfCommands] = {
	'S', 'A', UnitAddressCommand,
	'S', 'G', GOffsetCommand,
	'S', 'M', MOffsetCommand,
	'S', 'Q', ReadSystemCommand,
	'S', 'B', ReadOffsetCommand,
	'S', 'O', GenlockOffsetCommand,
	'S', 'P', PatternCommand,
	'S', 'E', EDHCommand,
	'S', 'U', AudioSignalCommand,
	'S', 'L', AudioLevelCommand,
	'S', 'C', AudioGroupCommand,
	'S', 'N', TextCommand,
	'S', 'F', TextONCommand,
	'S', 'J', TextPositionCommand,
	'S', 'D', TextStyleCommand,
	'S', 'S', ReadStatusCommand,
	'S', 'X', GPhaseCommand,
	'S', 'Y', MPhaseCommand,
	'S', 'Z', ResetCommand,
	'S', 'W', WriteUserTextCommand,
	'S', 'R', ReadUserTextCommand,
	'S', 'I', ReadSWVersionCommand,
	'S', 'V', ReadHWVersionCommand,
	'S', 'T', ReadHWInformationCommand,
	'B', 'T', TimeCommand,
	'B', 'D', DateCommand
};
									// Variables used to handle the input buffer
									// Receiver buffer located in Dual port RAM
static char RxBuffer[RxBufferLen] @ 0xB2C4;
									// NOTE! The Dual port RAM is also used in
									//  D1GENER.C for the string

static UC RxInputSize;		// Var. to hold the number of characters in buffer
static UC RxState;			// Receiver interrupt state machine
static UC RxChkSum;			// Calculated receiver checksum

static UC RxOutput;			// Pointer to the NEXT received char in the buffer
									//  input buffer
static UC RxInput;			// Pointer to the next FREE space in the input buffer

									// Variables used when decoding the input buffer
static UC BufState;			// Char handler state machine
static UC Char;				// Char read from the input buffer
static UC CharType;			// Received character type

static char CmdNdx;			// Command index
static char StrNdx;			// String index
static char ParamNdx;		// Parameter index

UC ParamCnt;					// Number of numeric parameters received
UC StrCnt;						// Number of string parameters received

static char Cmd[4];			// Decoded command
char CmdDelimiter;			// Command delimiter for the decoded command

char String[StringLen];		// String parameter
UL Parameter[3];				// Three unsigned long parameters

									// Variables used during transmission
static UC Tx_ACK_Buffer;	// Buffer to hold acknowledge byte
static bit Tx_ACK_Pending;	//
static bit Tx_BUSY;			//
static bit Tx_S0BUF_Empty;	//

static bit Broadcast;		// 1: Time/Date is being transferred

/***************************************************************************/
/*	RS232Intr															 			 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	Transmitter/receiver interrupt handler.							*/
/*	Remarks:		DO NOT CALL THIS FUNCTION!!! IT'S AN INTERRUPT ONLY			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
banked interrupt void RS232Intr( void) {

	static char RxChar, RxCharType;

	while ( RI0) {								// If receiver interrupt..
		if ( S0M2) {
			if ( S0BUF == 0x90) {			// If broadcast address..
				Broadcast = TRUE;				// prepare to receive WITHOUT an
				S0M2 = 0;						//  acknowledge in the end

				RxState = DataState;			// Initialize state machine
				RxChkSum = 0;					// Reset Checksum
			}
			else
				if ( S0BUF == MyAddress) {	// If MyAddress address..
					S0M2 = 0;					// prepare to receive WITH an
													//  acknowledge in the end
					RxState = DataState;		// Initialize state machine
					RxChkSum = 0;				// Reset Checksum
				}
		}
		else {
			RxChar = S0BUF;					// Read the character

			if ( RxState == DelState) {	// Last byte, ie. checksum received?..

				S0M2 = 1;						// Inciates that the next received byte
													//  will be an address

				if (( RxChkSum & 0x7F) - RxChar)
					Tx_ACK_Buffer = 0x81;	// Checksum ERROR
				else
					Tx_ACK_Buffer = 0x80;	// Checksum is OK

				RxChar = Tx_ACK_Buffer;

				if ( !Broadcast) {			// DO NOT TRANSMIT ACKNOWLEDGE FOR A
													//  BROADCAST MESSAGE!!!!
					if ( Tx_BUSY)
						Tx_ACK_Pending = TRUE;
					else {
						Tx_S0BUF_Empty = FALSE;
						S0BUF = Tx_ACK_Buffer;
					}
				}
				else
					Broadcast = FALSE;
			}
			else {								// .. else we're still receiving data
				RxCharType = TypeTable[RxChar];

				if ( RxCharType == 'Q')
					RxState ^= XORValue;		// Toggle between Data- and QuoteState
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

		RI0 = 0;									// Reset the interrupt
	}

	if ( TI0) {									// If transmitter interrupt
		if ( Tx_ACK_Pending) {				// If transmitter acknowledge pending ..
			Tx_ACK_Pending = FALSE;
			S0BUF = Tx_ACK_Buffer;			// transmit acknowledge
		}
		else
			Tx_S0BUF_Empty = TRUE;

		TI0 = 0;									// Reset the interrupt
	}
}

/***************************************************************************/
/*	RS232Init															 	  	  	 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	Initialize the RS232 communication.									*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void RS232Init() {

	RxInputSize = 0;				// Number of characters in input buffer

	RxInput = 0;					// Pointer to NEXT character in input buffer
	RxOutput = 0;					// Pointer to FIRST character in output buffer

	RxState = IdleState;			// Receiver state machine

	Char = 0;						// Character read from output buffer
	CharType = 0;					// Type of character read from output buffer
	BufState = State_0;			// Command handler state machine

	Broadcast = FALSE;			//

	Tx_ACK_Pending = FALSE;
	Tx_BUSY = FALSE;
	Tx_S0BUF_Empty = TRUE;
}

/***************************************************************************/
/*	CharHandler															 	  	  	 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	RS232 command handler.													*/
/*	Remarks:		NOTE!  The broadcast messages are handled specially.			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void CharHandler() {

	RxInputSize--;						// Decrement number of bytes in input buffer

											// Get next char from output buffer
	Char = RxBuffer[ RxOutput = ++RxOutput % RxBufferLen];
										
	CharType = TypeTable[Char];	// Determine type of the received character

	switch ( BufState) {

		case State_0:							// ***** BUFFER STATE 0 ******
			if ( CharType == 'A') {
				BufState++;

				Cmd[CmdNdx = 0] = Char;
				ParamNdx = 0;
				ParamCnt = StrCnt = 0;
				Parameter[0] = Parameter[1] = Parameter[2] = 0;
				return;
			}

			if ( CharType != 'D')
				BufState = State_7;
			return;

		case State_1:							// ***** BUFFER STATE 1 ******
			switch ( CharType) {
				case 'A':
					Cmd[++CmdNdx] = Char;
					return;

				case 'D':
					CmdDelimiter = Char;
					BufState = State_6;
					return;

				case 'Q':
					StrCnt = 1;

					StrNdx = -1;
					BufState = State_3;
					return;

				case 'N':
					ParamCnt++;

					Parameter[ParamNdx = 0] = Char - '0';
					BufState = State_2;
					return;

				default:
					BufState = State_7;
			}
			break;

		case State_2:							// ***** BUFFER STATE 2 ******
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

		case State_3:							// ***** BUFFER STATE 3 ******
			if ( CharType == 'Q') {
				String[++StrNdx] = '\0';
				BufState++;
				return;
			}

			String[++StrNdx] = Char;
			break;

		case State_4:							// ***** BUFFER STATE 4 ******
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

		case State_5:							// ***** BUFFER STATE 5 ******
			switch ( CharType) {
				case 'Q':
					StrNdx = -1;
					BufState = State_3;
					break;

				case 'N':
					ParamCnt++;

					Parameter[++ParamNdx] = Char - '0';
					BufState = State_2;
					break;

				default:
					BufState = State_7;
					return;
			}
			break;

		case State_6:							// ***** BUFFER STATE 6 ******
			if ( Char == 0x80) {

				if ( Cmd[0] == 'B') {
					if ( Cmd[1] == 'D')
						DateCommand();
					else			
						if ( Cmd[1] == 'T')
							TimeCommand();
					else
						ErrorStatus |= Illegal_Cmd;
				}
				else
					if ( Cmd[0] == 'S')
						CommandExecute();
				else
					ErrorStatus |= Illegal_Cmd;			
			}

			BufState = State_0;
			return;

		case State_7:
			if ( CharType == 'D')
				BufState++;
			return;			

		case State_8:
			BufState = State_0;				// Reset state-machine 
			return;			
	}
}

/***************************************************************************/
/*	CommandExecute													 	  		  	 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	Decode and execute the received RS232 command.					*/
/*	Remarks:		NOTE! The broadcast messages are NOT decoded in this 			*/
/*					function, UNLESS they are called as NORMAL commands.			*/
/*					Is is assumed the the first character HAS been tested AND	*/
/*					found valid BEFORE entering this function.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void CommandExecute() {

	register UI i;

	for ( i = 0; i < NoOfCommands; i++)
		if ( Cmd[1] == CommandTable[i].Cmd2) {
			if ( GeneratorRunning)
				CommandTable[i].Exec();
			else
#if PT8633Version
				if (( Cmd[0] == 'S') && 
					 (( Cmd[1] == 'Z') || ( Cmd[1] == 'V') || ( Cmd[1] == 'A')))
					CommandTable[i].Exec();
#else
				if (( Cmd[0] == 'S') && (( Cmd[1] == 'Z') || ( Cmd[1] == 'V')))
					CommandTable[i].Exec();
#endif
			return;
		}

	ErrorStatus |= Illegal_Cmd;
}

/***************************************************************************/
/*	TestInputBuffer													 	  	  	 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*	Function:	Test if any more date in the input buffer.						*/
/*	Remarks:		--																				*/
/*	Returns:		TRUE: If any more data, otherwise FALSE.							*/
/*	Updates:		--																				*/
/***************************************************************************/
bit TestInputBuffer() {

	if ( RxInput - RxOutput)
		return( TRUE);

	return( FALSE);
}

/***************************************************************************/
/*	WriteChar															 	  	  	 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	Write one character to the RS232 output buffer.					*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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
/*	WriteEndOfTx														 	  	  	 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	Write delimiter and checksum to the RS232 output buffer.		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void WriteEndOfTx( char checksum) {

	WriteChar( ';');
	WriteChar(( checksum + ';') & 0x7F);
}

/***************************************************************************/
/*	WriteString															 	  	  	 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	Write a string terminated by NULL to theRS232 output buffer.*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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
/*	WriteValue															 	  	  	 RS232.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970217	 											*/
/* Revised:		980601, KEn			     													*/
/*																									*/
/*	Function:	Write a value to the RS232 output buffer.							*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
UC WriteValue( UL value) {
	
	UC checksum = 0, i;
	UC tmp, ValOut = FALSE;
	UL divider = 1000000000;

	for ( i = 0; i < 10; i++) {
	   if ( tmp = ( value / divider) % 10)
			ValOut = TRUE;

		if ( ValOut || ( i == 9)) {
			tmp += '0';
			WriteChar( tmp);
			checksum += tmp;
		}	

		divider /= 10;
	}

	return( checksum);
}
