/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		RS232ERR.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990521																		*/
/*	Rev. date:	990902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void ResetErrorEventQueue();														*/
/*		int ReadErrorEventQueue();															*/
/*		void SaveErrorEventQueue( int ErrorType);										*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/
#include "rs232err.h"

_rom char NoErrTxt[]							= "No error";
_rom char CmdErrTxt[]						= "Command error";
_rom char InvalidCharTxt[]					= "Invalid character";
_rom char SyntaxErrTxt[]					= "Syntax error";
_rom char InvalidSeparatorTxt[]			= "Invalid separator";
_rom char DataTypeErrTxt[]					= "Data type error";
_rom char ParNotAllowedTxt[]				= "Parameter not allowed";
_rom char MissingParTxt[]					= "Missing parameter";
_rom char CmdHeaderErrTxt[]				= "Command header error";
_rom char HdrSeparatorErrTxt[]			= "Header separator error";
_rom char PrgmMnemonicTooLongTxt[]		= "Program mnemonic too long";
_rom char UndefinedHeaderTxt[]			= "Undefined header";
_rom char HdrSuffixOutOfRangeTxt[]		= "Header suffix out of range";

_rom char NumericDataErrTxt[]				= "Numeric data error";
_rom char InvalidCharInNumberTxt[]		= "Invalid character in number";
_rom char ExponentTooLargeTxt[]			= "Exponent too large";
_rom char TooManyDigitsTxt[]				= "Too many digits";
_rom char NumericDataNotAllowedTxt[]	= "Numeric data not allowed";
_rom char SuffixErrTxt[]					= "Suffix error";
_rom char InvalidSuffixErrTxt[]			= "Invalid suffix";
_rom char SuffixTooLongTxt[]				= "Suffix too long";
_rom char SuffixNotAllowedTxt[]			= "Suffix not allowed";

_rom char CharErrTxt[]						= "Character data error";
_rom char InvalidCharDataTxt[]			= "Invalid character data";
_rom char CharDataTooLongTxt[]			= "Character data too long";
_rom char CharDataNotAllowedTxt[]		= "Character data not allowed";

_rom char StringDataErrTxt[]				= "String data error";
_rom char InvalidStringDataTxt[]			= "Invalid string data";
_rom char StringDataNotAllowedTxt[]		= "String data not allowed";

_rom char BlockDataErrTxt[]				= "Block data error";
_rom char InvalidBlockDataTxt[]			= "Invalid block data";
_rom char BlockDataNotAllowedTxt[]		= "Block data not allowed";

_rom char ExpressionErrTxt[]				= "Expression error";

_rom char ExecErrTxt[]						= "Execution error";
_rom char CmdProtectedTxt[]				= "Command protected";

_rom char ParErrTxt[]						= "Parameter error";
_rom char DataOutOfRangeTxt[]				= "Data out of range";
_rom char TooMuchDataTxt[]					= "Too much data";
_rom char IllegalParValueTxt[]			= "Illegal parameter value";

_rom char InvalidVersionTxt[]				= "Invalid version";
_rom char HardwareMissingTxt[]			= "Hardware Missing";

_rom char DeviceSpecificErrTxt[]			= "Device-specific error";
_rom char QueueOverflowTxt[]				= "Queue overflow";
_rom char CommunicationErrTxt[]			= "Communication error";
_rom char ParityErrMessageTxt[]			= "Parity error in program message";
_rom char FramingErrMessageTxt[]			= "Framing error in program message";
_rom char InputBufferOverrunTxt[]		= "Input buffer overrun";

_rom char QueryErrTxt[]						= "Query error";
_rom char QueryInterruptedTxt[]			= "Query INTERRUPTED";
_rom char QueryUnterminatedTxt[]			= "Query UNTERMINATED";
_rom char QueryDeadlockedTxt[]			= "Query DEADLOCKED";


_rom struct ERROR_STRUCT ErrorTxt_Array[LastErrorCode] = {
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

_near int SCPIError;	  						// Current SCPI error

static _near int ErrorEventQueue[QUEUE_SIZE];

static _near char QueueHead = 0;
static _near char QueueTail = 0;

/***************************************************************************/
/*	ResetErrorEventQueue												 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 90521	    										*/
/* Revised:		990521				     													*/
/*																									*/
/*	Function:	Clear all entries in the error/event queue						*/
/*	Remarks:		Note! It is only the pointers which is reset						*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void ResetErrorEventQueue( void) {

	QueueHead = QueueTail = 0;
}

/***************************************************************************/
/*	ReadErrorEventQueue												 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990521				     													*/
/*																									*/
/*	Function:	Read the OLDEST entry in the error queue							*/
/*	Remarks:		-																				*/
/*	Returns:		Error _rom if queue > 0, otherwise 0, ie. No error				*/
/*	Updates:		-																				*/
/***************************************************************************/
int ReadErrorEventQueue( void) {

	register int tmp = NoErr;

	if ( QueueHead != QueueTail) {					// If Queue is not empty

		QueueTail = ( QueueTail+1) % QUEUE_SIZE;

		tmp = ErrorEventQueue[QueueTail];
	}

	return( tmp);
}

/***************************************************************************/
/*	SaveErrorEventQueue												 	    RS232ERR.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990521	    										*/
/* Revised:		990521				     													*/
/*																									*/
/*	Function:	Save the new error _rom in the error queue						*/
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
}
