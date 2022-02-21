/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		RS232ERR.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	961124																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

// SCPI Error codes:

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

#define TransmitChecksumErr	-381
#define TransmitTimeout			-382
#define ReceiveChecksumErr		-383
#define ReceiveTimeout			-384

#define IICAddressTimeout		-385
#define IICDataError				-386

#define QUEUE_SIZE	32

typedef struct ERROR_STRUCT {
	int ErrorNo;
	code char* ErrorTxt;
};

extern code struct ERROR_STRUCT ErrorTxt_Array[LastErrorCode];

extern UC STBReg;								// Status Byte
extern UC SREReg;								// Service Request Enable Register

extern UC ESRReg;								// Event Status Register
extern UC ESEReg; 							// Event Status Enable Register

extern UI QuestConditionReg;				// Questionable Condition Register
extern UI QuestNtransReg;					// Questionable NTransition Register
extern UI QuestPtransReg;					// Questionable PTransition Register
extern UI QuestEventReg;					// Questionable Event Register
extern UI QuestEnableReg;					// Questionable Enable Register

extern UI OperConditionReg;				// Operation Condition Register
extern UI OperNtransReg;					// Operation NTransition Register
extern UI OperPtransReg;					// Operation PTransition Register
extern UI OperEventReg;						// Operation Event Register
extern UI OperEnableReg;					// Operation Enable Register

extern int SCPIError;

extern void ResetErrorEventQueue();
extern int ReadErrorEventQueue();
extern void SaveErrorEventQueue( int ErrorType);
extern bit UpdateQUESTRegister();
extern bit UpdateOPERRegister();
extern UC UpdateESRRegister();
extern void UpdateSTBRegister();
