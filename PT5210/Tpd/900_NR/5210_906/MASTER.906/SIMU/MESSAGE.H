/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MESSAGE.H																	*/
/*	Author:		Kim Engedahl, VDE															*/
/*	Org. date:	970202																		*/
/*	Rev. date:	971212 KEn VDE																*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 971212 Added message ErrorInDataFromRS232											*/
/* 970911 ReadSWVersion altered to GetPT5210SWVersion								*/
/* 970825 Added system failures															*/
/* 970821 Added new error definitions													*/
/* 970723 Added SetupRS232GotoLocalMessage()											*/
/* 970226 Added timer parameter to DisplayErrorLine								*/
/***************************************************************************/

#define SystemErrorType 	  		  	0
#define AnlBlkErrorType		 	 		10
#define SDIBlkErrorType		 	 		20
#define AES_EBUErrorType	 	 		30
#define AnlSignalErrorType	 	 		40
#define SDISignalErrorType	 	 		50
#define SDIGenlockErrorType 	 		60
#define TimeCodeErrorType	 	 		70
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
extern void DisplayErrorLine( UI error, UC timer);
extern void GotoErrorStatus( UI error);
extern void GetPT5210SWVersion( char* string);
