/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		RS232ERR.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990521																		*/
/*	Rev. date:	990902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/
#ifndef _RS232ERR_H
#define _RS232ERR_H

// SCPI Error _roms:

// Abbreviations:
// Cmd = Command
// Par = Parameter
// Err = Error
// Prgm = Program
// Char = Character
// Hdr = Header
// Exec = Execution

enum ErrorCodes { NoErr = 0,
						CmdErr,
						InvalidChar,
						SyntaxErr,
						InvalidSeparator,
						DataTypeErr,
						ParNotAllowed,
						MissingPar,
						CmdHeaderErr,
						HdrSeparatorErr,
						PrgmMnemonicTooLong,
						UndefinedHeader,
						HdrSuffixOutOfRange,
						NumericDataErr,
						InvalidCharInNumber,
						ExponentTooLarge,
						TooManyDigits,
						NumericDataNotAllowed,
						SuffixErr,
						InvalidErr,
						SuffixTooLong,
						SuffixNotAllowed,
						CharErr,
						StringDataErr,
						InvalidStringData,
						StringDataNotAllowed,
						BlockDataErr,
						InvalidBlockData,
						ExpressionErr,
						ExecErr,
						CmdProtected,
						ParErr,
						DataOutOfRange,
						TooMuchData,
						IllegalParValue,
						InvalidVersion,
						HardwareMissing,
						DeviceSpecificErr,
						QueueOverflow,
						CommunicationErr,
						ParityErrInMessage,
						FramingErrInMessage,
						InputBufferOverrun,
						QueryErr,
						QueryInterrupted,
						QueryUnterminated,
						QueryDeadlocked,
						LastErrorCode
};

#define QUEUE_SIZE	16

typedef struct ERROR_STRUCT {
	int ErrorNo;
	_rom char* ErrorTxt;
};

extern _rom struct ERROR_STRUCT ErrorTxt_Array[LastErrorCode];

extern _near int SCPIError;

extern void ResetErrorEventQueue( void);
extern int ReadErrorEventQueue( void);
extern void SaveErrorEventQueue( int ErrorType);

#endif