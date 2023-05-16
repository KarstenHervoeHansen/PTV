/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		RS232PAR.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960713																		*/
/*	Rev. date:	980609, KEn DEV															*/
/*	Status:		Version 2.1																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*																									*/
/*	Changes:																						*/
/* 980608 In CmdSyntax(): It is now possible to enter semicolon in state0	*/
/* 971212 ResponseReceived is now a UC													*/
/* 970723 Added bit: NewCmdExecuted. Used to signal that the LCD display	*/
/*			 should be updated. This is necessary if the RS232 command will	*/
/*			 alter any information shown in the status display						*/
/* 970303 Added handling of parity-, framing-, break-error & overrun-error	*/
/* 970224 Forgot to reset CmdExecute at beginning of CmdHandler				*/
/* 970203 Strings can now distinguish between capital & small letters		*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>

#include "mas.h"

#include "52sync.h"
#include "serial1.h"
#include "util.h"
#include "rs232par.h"
#include "rs232err.h"
#include "rs232cmd.h"
#include "xaexprt.h"

#define StateP0		0
#define StateP1		StateP0+1
#define StateP2		StateP1+1
#define StateP3		StateP2+1
#define StateP4		StateP3+1
#define StateP5		StateP4+1
#define StateP6		StateP5+1
#define StateP7		StateP6+1
#define StateP8		StateP7+1

#define StateC0		StateP8+1

#define StateD0		StateC0+1
#define StateD1		StateD0+1
#define StateD2		StateD1+1
#define StateD3		StateD2+1
#define StateD4		StateD3+1
#define StateD5		StateD4+1
#define StateD6		StateD5+1
#define StateD7		StateD6+1

#define StateS0		StateD7+1
#define StateS1		StateS0+1
#define StateS2		StateS1+1
#define StateS3		StateS2+1

#define StateDA0		StateS3+1
#define StateDA1		StateDA0+1
#define StateDA2		StateDA1+1
#define StateDA3		StateDA2+1
#define StateDA4		StateDA3+1
#define StateDA5		StateDA4+1

#define StateA0		StateDA5+1

#define StateE0		StateA0+1

#define StateERROR	StateE0+1

#define iswhitespace(c)		(((c) != 0x0A) && ((c) <= 0x20))
#define isoctaldigit(c)		(((c) >= '0') && ((c) <= '7'))
#define isbinarydigit(c)	(((c) == '0') || ((c) == '1'))
#define iscolon(c)			((c) == ':')
#define isasterix(c)			((c) == '*')
#define issemicolon(c)		((c) == ';')
#define isquestionmark(c)	((c) == '?')
#define isunderscore(c)		((c) == '_')
#define isplussign(c)		((c) == '+')
#define isminussign(c)		((c) == '-')
#define isslash(c)			((c) == '/')
#define isdoubledagger(c)	((c) == '#')
#define isdot(c)				((c) == '.')
#define iscomma(c)			((c) == ',')
#define isquote(c)			((c) == '\'')
#define isdoublequote(c)	((c) == '\"')
#define isnewline(c)			((c) == 0x0A)		// Newline is Linefeed

enum CmdStates	{ Idle = 0, NextLevel, Ready, PMUS, PMT, Error };

bit NewCmdExecuted;
bit CmdRequest, CmdExecute;

UC ResponseReceived;

char Cmd[512];
UI CmdNdx;

UI NumSuffix;

UC ParameterType;

static code struct CMD_STRUCT* CmdPtr = &Subsystem[0];
static code struct CMD_STRUCT* CmdLevelPtr = &Subsystem[0];

static UC State = StateP0;

static UI NoOfBytes;
static UC NoOfDigits;

/***************************************************************************/
/*	CmdSyntax															 	    RS232PAR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980609				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		Command state																*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
char CmdSyntax( char c) {

	char c_str;

	c_str = c;
	c = toupper( c);

	if ( c < 0) {
		switch ( c) {
			case UDR_OVERRUN:
				SCPIError = InputBufferOverrun;
				break;

			case UDR_PARITY:
				SCPIError = ParityErrInMessage;
				break;

			case UDR_FRAMING:
				SCPIError = FramingErrInMessage;
				break;

			default:
				SCPIError = CommunicationErr;
				break;
		}
		State = StateERROR;
	}

	switch ( State) {
		case StateP0:							 // COMMAND PROGRAM DATA
			ParameterType = P_NoParameter;
			SCPIError = NoErr;
			CmdExecute = CmdRequest = FALSE;

			if ( iswhitespace(c) || issemicolon(c))
				break;

			if ( isalpha(c)) {
				Cmd[CmdNdx = 0] = c;
				NumSuffix = 0;
				State = StateP2;
				CmdPtr = CmdLevelPtr;
				break;
			}

			if ( iscolon(c)) {
				State = StateP1;
				CmdPtr = CmdLevelPtr = &Subsystem[0];
				break;
			}

			if ( isasterix(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateP3;
				CmdLevelPtr = CmdPtr;
				CmdPtr = &Mandated[0];
				break;
			}

			if ( isnewline(c)) {
				Cmd[CmdNdx = 0] = 0;
				State = StateP0;
				CmdPtr = CmdLevelPtr = &Subsystem[0];
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateP1:
			if ( isalpha(c)) {
				Cmd[CmdNdx = 0] = c;
				NumSuffix = 0;
				State = StateP2;
				break;
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateP2:
			if ( iswhitespace(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP6;
				return( Ready);
			}

			if ( isalpha(c) || isunderscore(c) || isdigit(c)) {
				CmdLevelPtr = CmdPtr;

				if ( CmdNdx < 11) {
					Cmd[++CmdNdx] = c;
					if ( isdigit(c)) {
						NumSuffix = 10*NumSuffix + ( c-'0');
						break;
					}
					NumSuffix = 0;
					NoOfDigits = CmdNdx + 1;
					break;
				}

				SCPIError = PrgmMnemonicTooLong;
				State = StateERROR;
				break;
			}

			if ( isquestionmark(c)) {
				State = StateP5;
				CmdRequest = TRUE;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMUS);
			}

			if ( iscolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP1;
				return( NextLevel);
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateP3:
			if ( isalpha(c)) {
				Cmd[CmdNdx = 0] = c;
				NumSuffix = 0;
				State = StateP4;
				break;
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateP4:
			if ( iswhitespace(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP6;
				return( Ready);
			}

			if ( isalpha(c) || isunderscore(c) || isdigit(c)) {
				if ( CmdNdx < 11) {
					Cmd[++CmdNdx] = c;
					break;
				}
				SCPIError = PrgmMnemonicTooLong;
				State = StateERROR;
				break;
			}

			if ( isquestionmark(c)) {
				State = StateP5;
				CmdRequest = TRUE;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMUS);
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateP5:
			if ( iswhitespace(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP6;
				return( Ready);
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMUS);
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateP6:
			if ( iswhitespace(c)) {
				State = StateP6;
				break;
			}

			if ( isalpha(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateC0;
				break;
			}

			if ( isdigit(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateD2;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMUS);
			}

			if ( isplussign(c) || isminussign(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateD0;
				break;
			}

			if ( isdot(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateD1;
				break;
			}

			if ( isdoubledagger(c)) {
				State = StateDA0;
				break;
			}

			if ( isdoublequote(c)) {
				CmdNdx = 0;
				State = StateS2;
				break;
			}

			if ( isquote(c)) {
				CmdNdx = 0;
				State = StateS0;
				break;
			}

			if ((c) == '(') {
				State = StateE0;
				break;
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateP7:
			if ( iswhitespace(c)) {
				State = StateP7;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMUS);
			}

			if ( iscomma(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP8;
				return( Ready);
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateP8:
			if ( iswhitespace(c)) {
				State = StateP8;
				break;
			}

			if ( isalpha(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateC0;
				break;
			}

			if ( isdigit(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateD2;
				break;
			}

			if ( isplussign(c) || isminussign(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateD0;
				break;
			}

			if ( isdot(c)) {
				Cmd[CmdNdx = 0] = c;
				State = StateD1;
				break;
			}

			if ( isdoubledagger(c)) {
				State = StateDA0;
				break;
			}

			if ( isdoublequote(c)) {
				CmdNdx = 0;
				State = StateS2;
				break;
			}

			if ( isquote(c)) {
				CmdNdx = 0;
				State = StateS0;
				break;
			}

			if ( (c) == '(') {
				State = StateE0;
				break;
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateC0:							 // CHARACTER PROGRAM DATA
			if ( iswhitespace(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP7;
				ParameterType = P_Char;
				break;
			}

			if ( isalpha(c) || isunderscore(c) || isdigit(c)) {
				if ( CmdNdx < 11) {
					Cmd[++CmdNdx] = c;
					break;
				}
				SCPIError = PrgmMnemonicTooLong;
				State = StateERROR;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_Char;
				return( PMUS);
			}

			if ( iscomma(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_Char;
				return( Ready);
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_Char;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateD0:							 // NUMERIC DECIMAL PROGRAM DATA
			if ( isdigit(c)) {
				Cmd[++CmdNdx] = c;
				State = StateD2;
				break;
			}

			if ( isdot(c)) {
				Cmd[++CmdNdx] = c;
				State = StateD1;
				break;
			}

			SCPIError = InvalidCharInNumber;
			State = StateERROR;
			break;

		case StateD1:
			if ( isdigit(c)) {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					State = StateD3;
					break;
				}
				SCPIError = TooManyDigits;
				State = StateERROR;
				break;
			}

			SCPIError = InvalidCharInNumber;
			State = StateERROR;
			break;

		case StateD2:
			if ( iswhitespace(c)) {
				State = StateD4;
				ParameterType = P_DecNum;
				break;
			}

			if ( isdigit(c)) {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					break;
				}
				SCPIError = TooManyDigits;
				State = StateERROR;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_DecNum;
				return( PMUS);
			}

			if ( isdot(c)) {
				if ( ++CmdNdx < 255) {
					Cmd[CmdNdx] = c;
					State = StateD3;
					break;
				}
				SCPIError = TooManyDigits;
				State = StateERROR;
				break;
			}

			if ( iscomma(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_DecNum;
				return( Ready);
			}

			if ( (c) == 'E') {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					State = StateD5;
					break;
				}
				SCPIError = TooManyDigits;					// TBD
				State = StateERROR;
				break;
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_DecNum;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateD3:
			if ( iswhitespace(c)) {
				State = StateD4;
				ParameterType = P_DecNum;
				break;
			}

			if ( isdigit(c)) {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					State = StateD3;
					break;
				}
				SCPIError = TooManyDigits;
				State = StateERROR;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_DecNum;
				return( PMUS);
			}

			if ( iscomma(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_DecNum;
				return( Ready);
			}

			if ( (c) == 'E') {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					State = StateD5;
					break;
				}
				SCPIError = TooManyDigits;			// TBD
				State = StateERROR;
				break;
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_DecNum;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateD4:
			if ( iswhitespace(c)) {
				State = StateD4;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMUS);
			}

			if ( iscomma(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP8;
				return( Ready);
			}

			if ( (c) == 'E') {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					State = StateD5;
					break;
				}
				SCPIError = TooManyDigits;			// TBD
				State = StateERROR;
				break;
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateD5:
			if ( iswhitespace(c)) {
				State = StateD5;
				break;
			}

			if ( isdigit(c)) {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					State = StateD7;
					break;
				}
				SCPIError = TooManyDigits;			// TBD
				State = StateERROR;
				break;
			}

			if ( isplussign(c) || isminussign(c)) {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					State = StateD6;
					break;
				}
				SCPIError = TooManyDigits;			// TBD
				State = StateERROR;
				break;
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateD6:
			if ( isdigit(c)) {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					State = StateD7;
					break;
				}
				SCPIError = TooManyDigits;			// TBD
				State = StateERROR;
				break;
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateD7:
			if ( iswhitespace(c)) {
				State = StateP7;
				ParameterType = P_DecNum;
				break;
			}

			if ( isdigit(c)) {
				if ( CmdNdx < 255) {
					Cmd[++CmdNdx] = c;
					break;
				}
				SCPIError = TooManyDigits;			// TBD
				State = StateERROR;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_DecNum;
				return( PMUS);
			}

			if ( iscomma(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_DecNum;
				return( Ready);
			}

			if ( isnewline(c)) {
				Cmd[++CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_DecNum;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateS0:							 // STRING PROGRAM DATA
			if ( isquote(c)) {
				State = StateS1;
				break;
			}

			Cmd[CmdNdx++] = c_str;
			break;

		case StateS1:
			if ( iswhitespace(c)) {
				State = StateP7;
				ParameterType = P_String;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_String;
				return( PMUS);
			}

			if ( isquote(c)) {
				Cmd[CmdNdx++] = c;
				State = StateS0;
				break;
			}

			if ( iscomma(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_String;
				return( Ready);
			}

			if ( isnewline(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_String;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateS2:
			if ( isdoublequote(c)) {
				State = StateS3;
				break;
			}

			Cmd[CmdNdx++] = c_str;
			break;

		case StateS3:
			if ( iswhitespace(c)) {
				State = StateP7;
				ParameterType = P_String;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_String;
				return( PMUS);
			}

			if ( isdoublequote(c)) {
				Cmd[CmdNdx++] = c;
				State = StateS2;
				break;
			}

			if ( iscomma(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_String;
				return( Ready);
			}

			if ( isnewline(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_String;
				return( PMT);
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateDA0:							 // NON-DECIMAL/ARBITRARY PROGRAM DATA
			if ( (c) == 'B') {
				CmdNdx = 0;
				State = StateDA3;
				break;
			}

			if ( (c) == 'Q') {
				CmdNdx = 0;
				State = StateDA2;
				break;
			}

			if ( (c) == 'H') {
				CmdNdx = 0;
				State = StateDA1;
				break;
			}

			if ( (c) == '0') {
				CmdNdx = 0;
				State = StateDA5;
				break;
			}

			if ( isdigit(c)) {
				NoOfDigits = c - '0';
				NoOfBytes = 0;
				CmdNdx = 0;
				State = StateDA4;
				break;
			}

			SCPIError = InvalidChar;
			State = StateERROR;
			break;

		case StateDA1:
			if ( isxdigit(c)) {
				if ( CmdNdx < 255) {				// TBD SHOULD NOT GENERATE AN ERR
					Cmd[CmdNdx++] = c;
					break;
				}
//				SCPIError = TooManyDigits;
//				State = StateERROR;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_HexDecNum;
				return( PMUS);
			}

			if ( iswhitespace(c)) {
				State = StateP7;
				ParameterType = P_HexDecNum;
				break;
			}

			if ( iscomma(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_HexDecNum;
				return( Ready);
			}

			if ( isnewline(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_HexDecNum;
				return( PMT);
			}

			SCPIError = InvalidCharInNumber;
			State = StateERROR;
			break;

		case StateDA2:
			if ( isoctaldigit(c)) {
				if ( CmdNdx < 255) {					// TBD NOT ERROR CODES
					Cmd[CmdNdx++] = c;
					break;
				}
//				SCPIError = ParameterOverflowErr;
  //				State = StateERROR;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_OctDecNum;
				return( PMUS);
			}

			if ( iswhitespace(c)) {
				State = StateP7;
				ParameterType = P_OctDecNum;
				break;
			}

			if ( iscomma(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_OctDecNum;
				return( Ready);
			}

			if ( isnewline(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_OctDecNum;
				return( PMT);
			}

			SCPIError = InvalidCharInNumber;
			State = StateERROR;
			break;

		case StateDA3:
			if ( isbinarydigit(c)) {
				if ( CmdNdx < 255) {					// TBD NOT ERROR CODES
					Cmd[CmdNdx++] = c;
					break;
				}
//				SCPIError = ParameterOverflowErr;
//				State = StateERROR;
				break;
			}

			if ( issemicolon(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_BinDecNum;
				return( PMUS);
			}

			if ( iswhitespace(c)) {
				State = StateP7;
				ParameterType = P_BinDecNum;
				break;
			}

			if ( iscomma(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP8;
				ParameterType = P_BinDecNum;
				return( Ready);
			}

			if ( isnewline(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_BinDecNum;
				return( PMT);
			}

			SCPIError = InvalidCharInNumber;
			State = StateERROR;
			break;

		case StateDA4:
			if ( NoOfDigits) {
				if ( isdigit(c)) {
					NoOfDigits--;
					NoOfBytes = 10*NoOfBytes + ( c - '0');
					break;
				}
				SCPIError = InvalidBlockData;
				State = StateERROR;
				break;
			}

			if ( CmdNdx < NoOfBytes) {
				Cmd[CmdNdx++] = c_str;
			}
			else {
				if ( issemicolon(c)) {
					Cmd[CmdNdx] = 0;
					State = StateP0;
					ParameterType = P_Blockn;
					return( PMUS);
				}

				if ( iswhitespace(c)) {
					State = StateP7;
					ParameterType = P_Blockn;
					break;
				}

				if ( iscomma(c)) {
					Cmd[CmdNdx] = 0;
					State = StateP8;
					ParameterType = P_Blockn;
					return( Ready);
				}

				if ( isnewline(c)) {
					Cmd[CmdNdx] = 0;
					State = StateP0;
					ParameterType = P_Blockn;
					return( PMT);
				}

				SCPIError = InvalidBlockData;
				State = StateERROR;
				break;
			}
			break;

		case StateDA5:
			if ( isnewline(c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				ParameterType = P_Block0;
				return( PMT);
			}

			Cmd[CmdNdx++] = c_str;
			break;

		case StateA0:							 // ASCII DATA ONLY FOR A RESPONSE
			if ( isnewline( c)) {
				Cmd[CmdNdx] = 0;
				State = StateP0;
				return( PMT);
			}

			Cmd[CmdNdx++] = c_str;
			ParameterType = P_ASCII;
			break;

		case StateE0:
			if ( (c) == ')') {
				State = StateP7;
				ParameterType = P_Block0;
			}
			break;

		case StateERROR:						 // ERROR
			if ( issemicolon(c)) {
				State = StateP0;
				return( PMUS);
			}

			if ( isnewline(c)) {
				State = StateP0;
				return( PMT);
			}
			break;
	}

	if ( State == StateERROR)
		return( Error);

	return( Idle);
}

/***************************************************************************/
/*	CmdHandler															 	    RS232PAR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		-------																		*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void CmdHandler( char c) {

	UC CmdState;
	UL DecVar;

	switch ( CmdState = CmdSyntax( c)) {
		case Idle:
			break;

		case Error:
			break;

		default:									// ie. NextLevel, Ready, PMUS and PMT
			if ( ParameterType == P_NoParameter) {

				if ( NumSuffix)
					Cmd[NoOfDigits] = 0;
				else
					NumSuffix = 1;

				while ( CmdPtr) {
					if (( !Cstrcmp( Cmd, CmdPtr->LongName)) ||
						 ( !Cstrcmp( Cmd, CmdPtr->ShortName))) {

						if ( CmdPtr->Suffix != NONE)
							CmdPtr->Suffix();
						else {
							if ( NumSuffix > 1)
								SCPIError = HdrSuffixOutOfRange;
						}

						if ( CmdState == NextLevel)
							CmdPtr = CmdPtr->NextLevel;
						break;
					}
					CmdPtr = CmdPtr->SameLevel;
				}
			}
			else {
				switch ( ParameterType) {
					case P_BinDecNum:
						btol( Cmd, &DecVar);
						_ultoa( DecVar, Cmd);
						ParameterType = P_DecNum;
						break;

					case P_OctDecNum:
						otol( Cmd, &DecVar);
						_ultoa( DecVar, Cmd);
						ParameterType = P_DecNum;
						break;

					case P_HexDecNum:
						htol( Cmd, &DecVar);
						_ultoa( DecVar, Cmd);
						ParameterType = P_DecNum;
						break;
				}
			}

			if ( CmdState != NextLevel) {				// ie. Ready, PMUS and PMT

				if ( SCPIError == NoErr) {

					if ( CmdState != Ready)
						CmdExecute = TRUE;
					else
						CmdExecute = FALSE;

					if (( CmdPtr->Function != NONE) && CmdPtr)
						CmdPtr->Function();
					else {
						if ( Cmd[0] != '\x0')
							SCPIError = SyntaxErr;
					}
				}

				if ( CmdState == PMT)
					CmdPtr = CmdLevelPtr = &Subsystem[0];	// Reset level pointers

				if ( CmdState != Ready) {					// ie. PMUS and PMT
					if ( SCPIError) {							// If any errors has occured
						SaveErrorEventQueue( SCPIError);	//  save the error
						SCPIError = NoErr;
					}
					else
						NewCmdExecuted = TRUE;				// Command has succesfully
				}													//  been executed
			}
			break;
	}
}

/***************************************************************************/
/*	ResponseHandler													 	    RS232PAR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970218	    										*/
/* Revised:		971212				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void ResponseHandler( char* str) {

	ResponseReceived = 0;
	CmdExecute = CmdRequest = FALSE;

	Udr_PutNChar( str);
	Udr_PutNewline();						// Transmit command initialize donwload

	if ( !Cstrcmp( str, IDNResponseCmdTxt)) {
		State = StateA0;
		CmdPtr = &Response[0];

		ParameterType = P_NoParameter;

		strcpy( Cmd, str);

		CmdPtr->Function();
	}
	else {
		if ( !Cstrncmp( str, DownloadResponseCmdTxt,\
													 Cstrlen( DownloadResponseCmdTxt))) {
			State = StateP6;
			CmdPtr = &SystemLevel1[4];

			strcpy( Cmd, str);

			ParameterType = P_NoParameter;
			CmdPtr->Function();

			Cmd[0] = str[ strlen( str)- 1];
			Cmd[1] = 0;

			ParameterType = P_DecNum;
			CmdPtr->Function();
		}
	}
}
