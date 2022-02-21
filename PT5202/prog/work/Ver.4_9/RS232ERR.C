/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		RS232ERR.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960713																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void ResetErrorEventQueue();														*/
/*		bit UpdateQUESTRegister();															*/
/*		bit UpdateOPERRegister();															*/
/*		UC UpdateESRRegister();																*/
/*		void UpdateSTBRegister();															*/
/*		int ReadErrorEventQueue();															*/
/*		void SaveErrorEventQueue( int ErrorType);										*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

/***************************************************************************/
// Include files:

//#include <xa.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//#include "mas.h"

#include "define.h"
#include "rs232err.h"

char code NoErrTxt[]							= "No error";
char code CmdErrTxt[]						= "Command error";
char code InvalidCharTxt[]					= "Invalid character";
char code SyntaxErrTxt[]					= "Syntax error";
char code InvalidSeparatorTxt[]			= "Invalid separator";
char code DataTypeErrTxt[]					= "Data type error";
char code ParNotAllowedTxt[]				= "Parameter not allowed";
char code MissingParTxt[]					= "Missing parameter";
char code CmdHeaderErrTxt[]				= "Command header error";
char code HdrSeparatorErrTxt[]			= "Header separator error";
char code PrgmMnemonicTooLongTxt[]		= "Program mnemonic too long";
char code UndefinedHeaderTxt[]			= "Undefined header";
char code HdrSuffixOutOfRangeTxt[]		= "Header suffix out of range";

char code NumericDataErrTxt[]				= "Numeric data error";
char code InvalidCharInNumberTxt[]		= "Invalid character in number";
char code ExponentTooLargeTxt[]			= "Exponent too large";
char code TooManyDigitsTxt[]				= "Too many digits";
char code NumericDataNotAllowedTxt[]	= "Numeric data not allowed";
char code SuffixErrTxt[]					= "Suffix error";
char code InvalidSuffixErrTxt[]			= "Invalid suffix";
char code SuffixTooLongTxt[]				= "Suffix too long";
char code SuffixNotAllowedTxt[]			= "Suffix not allowed";

char code CharErrTxt[]						= "Character data error";
char code InvalidCharDataTxt[]			= "Invalid character data";
char code CharDataTooLongTxt[]			= "Character data too long";
char code CharDataNotAllowedTxt[]		= "Character data not allowed";

char code StringDataErrTxt[]				= "String data error";
char code InvalidStringDataTxt[]			= "Invalid string data";
char code StringDataNotAllowedTxt[]		= "String data not allowed";

char code BlockDataErrTxt[]				= "Block data error";
char code InvalidBlockDataTxt[]			= "Invalid block data";
char code BlockDataNotAllowedTxt[]		= "Block data not allowed";

char code ExpressionErrTxt[]				= "Expression error";

char code ExecErrTxt[]						= "Execution error";
char code CmdProtectedTxt[]				= "Command protected";

char code ParErrTxt[]						= "Parameter error";
char code SettingsErrTxt[]					= "Settings conflict";
char code DataOutOfRangeTxt[]				= "Data out of range";
char code TooMuchDataTxt[]					= "Too much data";
char code IllegalParValueTxt[]			= "Illegal parameter value";

char code InvalidVersionTxt[]				= "Invalid version";
char code HardwareMissingTxt[]			= "Hardware Missing";

char code DeviceSpecificErrTxt[]			= "Device-specific error";
char code QueueOverflowTxt[]				= "Queue overflow";
char code CommunicationErrTxt[]			= "Communication error";
char code ParityErrMessageTxt[]			= "Parity error in program message";
char code FramingErrMessageTxt[]			= "Framing error in program message";
char code InputBufferOverrunTxt[]		= "Input buffer overrun";

char code QueryErrTxt[]						= "Query error";
char code QueryInterruptedTxt[]			= "Query INTERRUPTED";
char code QueryUnterminatedTxt[]			= "Query UNTERMINATED";
char code QueryDeadlockedTxt[]			= "Query DEADLOCKED";


struct ERROR_STRUCT code ErrorTxt_Array[LastErrorCode] = {
	{    0, NoErrTxt },						// NoErr
	{ -100, CmdErrTxt },						// CmdErr
	{ -101, InvalidCharTxt },				// InvalidChar
	{ -102, SyntaxErrTxt },					// SyntaxErr
	{ -103, InvalidSeparatorTxt },		// InvalidSeparator
	{ -104, DataTypeErrTxt },				// DataTypeErr
	{ -108, ParNotAllowedTxt },			// ParNotAllowed
	{ -109, MissingParTxt },				// Missingpar
	{ -110, CmdHeaderErrTxt },				// CmdHeaderErr
	{ -111, HdrSeparatorErrTxt },			// HdrSeparatorErr
	{ -112, PrgmMnemonicTooLongTxt },	// PrgmMnemonicTooLong
	{ -113, UndefinedHeaderTxt },			// UndefinedHeader
	{ -114, HdrSuffixOutOfRangeTxt },	// HdrSufficOutOfrange

	{ -120, NumericDataErrTxt },			// NumericDataErr
	{ -121, InvalidCharInNumberTxt },	//	InvalidCharinNUmber
	{ -123, ExponentTooLargeTxt },		// ExponentTooLarge
	{ -124, TooManyDigitsTxt },			// TooManyDigits
	{ -125, NumericDataNotAllowedTxt },	// NumericDataNotAllowed
	{ -130, SuffixErrTxt },					// SuffixErr
	{ -131, InvalidSuffixErrTxt },		// InvalidSuffixErr
	{ -134, SuffixTooLongTxt },			// SuffixTooLong
	{ -138, SuffixNotAllowedTxt },		// SuffixNotAllowed

	{ -140, CharErrTxt },					// CharErr

	{ -150, StringDataErrTxt },			// StringDataErr
	{ -151, InvalidStringDataTxt },		// InvalidStringData
	{ -158, StringDataNotAllowedTxt },	// StringDataNotAllowed

	{ -160, BlockDataErrTxt },				// BlockDataErr
	{ -161, InvalidBlockDataTxt },		// InvalidBlockData

	{ -170, ExpressionErrTxt },			// ExpressionErr

	{ -200, ExecErrTxt },					// ExecErr
	{ -203, CmdProtectedTxt },				// CmdProtected

	{ -220, ParErrTxt },						// ParErr
	{ -221, SettingsErrTxt },						// SettingsErr
	{ -222, DataOutOfRangeTxt },			// DataOutOfRange
	{ -223, TooMuchDataTxt },				// TooMuchData
	{ -224, IllegalParValueTxt },			// IllegalParValue

	{ -233, InvalidVersionTxt },			// InvalidVersion

	{ -241, HardwareMissingTxt },			// HardwareMissing

	{ -300, DeviceSpecificErrTxt },		// DeviceSpecificErr
	{ -350, QueueOverflowTxt },			// QueueOverflow
	{ -360, CommunicationErrTxt },		// CommunicationErr
	{ -361, ParityErrMessageTxt },		// ParityErrMessage
	{ -362, FramingErrMessageTxt },		// FramingErrMessage
	{ -363, InputBufferOverrunTxt },		// InputBufferOverrunErr
	{ -400, QueryErrTxt },					// QueryErr
	{ -410, QueryInterruptedTxt },		// QueryInterrupted
	{ -420, QueryUnterminatedTxt },		// QueryUnterminated
	{ -430, QueryDeadlockedTxt }			// QueryDeadlocked
};

UC STBReg;										// Status Byte
UC SREReg;										// Service Request Enable Register

UC ESRReg;										// Event Status Register
UC ESEReg; 										// Event Status Enable Register

UI QuestConditionReg;						// Questionable Condition Register
UI QuestNtransReg;							// Questionable NTransition Register
UI QuestPtransReg;							// Questionable PTransition Register
UI QuestEventReg;								// Questionable Event Register
UI QuestEnableReg;							// Questionable Enable Register

UI OperConditionReg;							// Operation Condition Register
UI OperNtransReg;								// Operation NTransition Register
UI OperPtransReg;								// Operation PTransition Register
UI OperEventReg;								// Operation Event Register
UI OperEnableReg;								// Operation Enable Register

int SCPIError;									// Current SCPI error

static int ErrorEventQueue[QUEUE_SIZE];

static int QueueHead = 0;
static int QueueTail = 0;

/***************************************************************************/
/*	ResetErrorEventQueue												 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Clear all entries in the error/event queue						*/
/*	Remarks:		Note! It is only the pointers which is reset						*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void ResetErrorEventQueue() {

	QueueHead = QueueTail = 0;

	UpdateSTBRegister();
}

/***************************************************************************/
/*	ReadErrorEventQueue												 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Read the OLDEST entry in the error queue							*/
/*	Remarks:		-																				*/
/*	Returns:		Error code if queue > 0, otherwise 0, ie. No error				*/
/*	Updates:		-																				*/
/***************************************************************************/
int ReadErrorEventQueue() {

	register int tmp = NoErr;

	if ( QueueHead != QueueTail) {					// If Queue is not empty

		QueueTail = ( QueueTail+1) % QUEUE_SIZE;

		tmp = ErrorEventQueue[QueueTail];
	}

	UpdateSTBRegister();

	return( tmp);
}

/***************************************************************************/
/*	SaveErrorEventQueue												 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Save the new error code in the error queue						*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SaveErrorEventQueue( int ErrorType) {

	if ((( QueueHead+1) % QUEUE_SIZE) != QueueTail) { // If queue not full

		QueueHead = ( ++QueueHead) % QUEUE_SIZE;

		ErrorEventQueue[QueueHead] = ErrorType;
	}
	else
		ErrorEventQueue[QueueHead] = QueueOverflow;

	UpdateSTBRegister();
}

/***************************************************************************/
/*	UpdateQUESTRegister												 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Update the content of the Questionable status register		*/
/*	Remarks:		-																				*/
/*	Returns:		A sum bit																	*/
/*	Updates:		-																				*/
/***************************************************************************/
bit UpdateQUESTRegister() {

	QuestConditionReg=0;				// 1. Condition, ie. directly hardware etc.
	QuestPtransReg=0;					// 2. Positive transition
	QuestNtransReg=0;					// 3. Negative transition
	QuestEventReg=0;					// 4. Event section
	QuestEnableReg=0;					// 5. Enable section

	return( FALSE);
}

/***************************************************************************/
/*	UpdateOPERRegister												 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Update the content of the Operation status register			*/
/*	Remarks:		-																				*/
/*	Returns:		A sum bit																	*/
/*	Updates:		-																				*/
/***************************************************************************/
bit UpdateOPERRegister() {

	OperConditionReg=0;				// 1. Condition, ie. directly hardware etc.
	OperPtransReg=0;					// 2. Positive transition
	OperNtransReg=0;					// 3. Negative transition
	OperEventReg=0;					// 4. Event section
	OperEnableReg=0;					// 5. Enable section

	return( FALSE);
}

/***************************************************************************/
/*	UpdateESRRegister													 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Update the content of the ESR register								*/
/*	Remarks:		-																				*/
/*	Returns:		The content of the ESR register										*/
/*	Updates:		-																				*/
/***************************************************************************/
UC UpdateESRRegister() {
										// Bit 7: Power On
										// Bit 6: User request
										// Bit 5: Command error
										// Bit 4: Execution error
										// Bit 3: Device dependent error
										// Bit 2: Queue error
										// Bit 1: Request control
										// Bit 0: Operation complete
	return( ESRReg);
}

/***************************************************************************/
/*	UpdateSTBRegister													 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Update the content of the Status Byte register					*/
/*	Remarks:		This command also updates the ESR register, the	operation-	*/
/*					register and the questionable register.							*/
/*	Returns:		-																				*/
/*	Updates:		STBReg, ESRReg, QUESTReg & OPERReg									*/
/***************************************************************************/
void UpdateSTBRegister() {

	STBReg &= 0x07;							// Clear the MSS-, ESR- & MAV-bit &
													//  operation & questionable bits
	if ( UpdateOPERRegister())
		STBReg |= 0x80;

	if ( STBReg & SREReg & 0xBF)			//	Calculate the MSS bit(6) in STB
		STBReg |= 0x40;

	if ( UpdateESRRegister() & ESEReg)	//	Calculate the ESB bit(5) in STB
		STBReg |= 0x20;

	if ( QueueHead != QueueTail)			//	Calculate the MAV bit(4) in STB
		STBReg |= 0x10;

	if ( UpdateQUESTRegister())
		STBReg |= 0x08;
}
