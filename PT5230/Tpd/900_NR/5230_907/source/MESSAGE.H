/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		MESSAGE.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	970202																		*/
/*	Rev. date:	991026 KEn, DEV															*/
/*	Status:		Version 1.1																	*/
/*																									*/
/*	Changes:																						*/
/* 991026: Added function GetErrorString( UL error, char* tmpbuf)				*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

#define SystemErrorType 	  		  	0
#define AnlBlkErrorType		 	 		10
#define SDITSGErrorType		 	 		20
#define AESEBUErrorType	 	 			30
#define AnlTPGErrorType	 	 			40
#define SDITPGErrorType	 	 			50
#define SDIGenlockErrorType 	 		60
#define TimeClockErrorType	 	 		70
#define OSCErrorType			 	 		80
#define ROMMemoryErrorType	 	  		90
#define RAMMemoryErrorType				100
#define KeyboardErrorType				110
#define LCD_ADCErrorType				120
#define TempADCErrorType 				130
#define VoltageADCErrorType 			140
#define LevelDetectorErrorType		150


// SYSTEM ERRORS
#define MultipleFailure					1
#define PowerN5Failure					2
#define Power12Failure					3
#define TemperatureFailure				4

#define TemperatureWarning				5
#define LevelDetectorError				6
#define ConfigurationError				7
#define MultipleError					8

// NORMAL ERRORS
#define ChecksumError					5

#define ParallelLockoutMessage		1
#define SerialOperatingMessage		2
#define SerialLockoutMessage			3
#define NoResponseFromRS232			4
#define ErrorDownloadingFromRS232	5
#define ErrorInDataFromRS232			6

#define ErrorDisplayTimeout	50			// Equals 1 second

extern void MessageHandling( UC messageno);
extern void SetupRS232GotoLocalMessage();
extern void RS232GotoLocalMessage();
extern void WriteErrorLN2( UC LN2Pos, UL error);
extern void GetErrorString( UL error, char* tmpbuf);
extern void DisplayErrorLine( UI error, UC timer);
extern void GotoErrorStatus( UI error);
extern void GetPT5230SWVersion( char* string);
