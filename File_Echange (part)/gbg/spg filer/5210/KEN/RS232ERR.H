/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		RS232ERR.H																	*/
/*	Author:		Kim Engedahl, VDE															*/
/*	Org. date:	961124																		*/
/*	Rev. date:	971212, KEn VDE															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 970630 Added UpdateESRRegister() & UpdateSTBRegister()						*/
/*			 Added UpdateQUESTRegister() & UpdateOPERRegister()					*/
/* 970627 Added Status Reporting bytes													*/
/* 970129 Added errorcode HardwareNotPresentErr										*/
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
