/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		MESSAGE.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980208																		*/
/*	Rev. date:	981026, KEn, DEV															*/
/*	Status:		Version 1.2																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void MessageHandling( UC messageno);											*/
/* 	void SetupRS232GotoLocalMessage();												*/
/*		void RS232GotoLocalMessage();														*/
/*		void WriteErrorLN2( UI errorcode);												*/
/*		void GetErrorString( UL error, char* tmpbuf)									*/
/*		void DisplayErrorLine( UI error, UC timer);									*/
/*		void GotoErrorStatus( UI error);													*/
/*		void ReadSWVersion( char* string);												*/
/*                                                                         */
/* Changes:                                                                */
/* 991026: Rewritten void WriteErrorLN2( UC LN2Pos, UL error)					*/
/*			 Introduced new function to handle writing of an error				*/
/* 981005: Cleanup in leveldetector errors in function WriteErrorLN2()		*/ 
/* 980915: Error in MessgeHandling(): Local lockout for serial remote not	*/
/*			   displayed correctly.															*/
/* 980616: Released as 4008 002 06812													*/
/* 980611: Update LevelStatus handling with 6 audio levels for PT8635		*/
/* 980609: In DisplayErrorLine() & GotoErrorStatus() the system error:		*/
/*				Level Detector ALWAYS will pass											*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xa.h>

#include "mas.h"

#include <ctype.h>

#include "sw5230.h"
#include "message.h"
#include "menu.h"
#include "menuhlp.h"
#include "menutree.h"
#include "disp_drv.h"
#include "text.h"
#include "iic.h"
#include "serial1.h"
#include "tables.h"
#include "keyb_drv.h"
#include "util.h"

/***************************************************************************/
/* MessageHandling														     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980915				     													*/
/*																									*/
/*	Function:	Display a message text in display, and start message timer	*/
/*	Remarks:																						*/
/*	Returns:		--																				*/
/*	Updates:																						*/
/***************************************************************************/
void MessageHandling( UC messageno) {

	switch ( messageno) {
		case ParallelLockoutMessage:
			WriteTxtLN1( 0, "M(001): ");
			WriteCode1( ParallelRemoteOperatingTxt);
			ClearRestLN1();

			WriteCodeLN2( 0, LocalLockoutTxt);
			ClearRestLN2();

			MessageWindowOn = ON;
			MessageCntDwn = 15;				// 15*200ms = 3s
			break;

		case SerialLockoutMessage:
			WriteTxtLN1( 0, "M(003): ");
			WriteCode1( SerialRemoteOperatingTxt);
			ClearRestLN1();

			WriteCodeLN2( 0, LocalLockoutTxt);
			ClearRestLN2();

			MessageWindowOn = ON;
			MessageCntDwn = 15;				// 15*200ms = 3s
			break;

		case NoResponseFromRS232:
			WriteTxtLN1( 0, "E(160): ");
			WriteCode1( NoResponseFromTxt);
			WriteCode1( RS232Txt);
			ClearRestLN1();
			WriteArrowsField( U_+D_+R_+L_+E_);	// Write icons/dots on line 1&2

			WriteCodeLN2( 0, CheckCableTxt);
			ClearRestLN2();
			break;

		case ErrorDownloadingFromRS232:
			WriteTxtLN1( 0, "E(161): ");
			WriteCode1( ErrorDownloadingFromTxt);
			WriteCode1( RS232Txt);
			ClearRestLN1();
			WriteArrowsField( U_+D_+R_+L_+E_);	// Write icons/dots on line 1&2

			WriteCodeLN2( 0, ChecksumErrorReceivingTxt);
			ClearRestLN2();
			break;

		case ErrorInDataFromRS232:
			WriteTxtLN1( 0, "E(162): ");
			WriteCode1( ErrorDownloadingFromTxt);
			WriteCode1( RS232Txt);
			ClearRestLN1();
			WriteArrowsField( U_+D_+R_+L_+E_);	// Write icons/dots on line 1&2

			WriteCodeLN2( 0, DataNotCompatibleTxt);
			ClearRestLN2();
			break;

		default:
			break;
	}
}

/***************************************************************************/
/* SetupRS232GotoLocalMessage											     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970723	    										*/
/* Revised:		970723				     													*/
/*																									*/
/*	Function:	Prepare for the "Goto Local?"	display and state machine		*/
/*	Remarks:																						*/
/*	Returns:		--																				*/
/*	Updates:																						*/
/***************************************************************************/
void SetupRS232GotoLocalMessage() {

	if ( MenuModeOn) {					// If in menu mode..
		if ( SelectModeOn) {				// If in selection mode ..
			Ptr->EscapeFct();				//  be sure to terminate NICELY!!
			Ptr->DispFunct();
		}

		UpToStatus();						// Update ptr to status-display
		Ptr->DispFunct();					// Display status-display
	}
	PrevPtr = Ptr;							// Save old status display which is
												//  ALWAYS a status display

	FuncState = 2;							// Prepare for state machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;
}

/***************************************************************************/
/* RS232GotoLocalMessage												     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980415				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		--																				*/
/*	Updates:																						*/
/***************************************************************************/
void RS232GotoLocalMessage() {

	if ( FuncSave || FuncEscape) {

		if ( FuncSave)	{						// If OK selected..
			SerialRemoteActive = FALSE;	//  goto local mode
			RemoteLED = OFF;
			UpdateLEDs();
		}
		MenuModeOn = SelectModeOn = OFF;

		FuncSave = FuncEscape = FALSE;	// This is normally done in the function
													//  pointed to by PrevPtr. In this case
													//  it MUST be reset manually 'cause
													//  PrevPtr points to a StatusDisplay

		Ptr = PrevPtr;							// Restore old status display pointer
		Ptr->DispFunct();						//  which ALWAYS is a status display
		return;
	}

	MenuModeOn = SelectModeOn = ON;

	WriteTxtLN1( 0, "M(002): ");
	WriteCode1( SerialRemoteOperatingTxt);
	ClearRestLN1();

	WriteTxtLN2( 0, "Goto local ?");
	ClearRestLN2();

	WriteType10_OK_ESC( FuncState);			// Write the SAVE ESC text

	FuncTriggers = 0;								// Reset the button-selection
}

/***************************************************************************/
/* WriteErrorLN2															     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		981005				     													*/
/*																									*/
/*	Function:	Write an error text based on <errorcode> on line 2, LN2Pos 	*/
/*	Remarks:	 	Translation of the 32-bit parameter, error:						*/
/*					16MSB				8 BIT		8 LSB											*/
/*					level detector	[index]	ErrorType ErrorCode						*/
/* 				Option PT8601 SHOULD only be placed in upper space TXG2, ie.*/
/*					it is ONLY tested in this place.										*/
/*					Level detectors:															*/
/*									01  02/03  31v1  31v2  32/33 08/09/39				*/
/*						BB1																		*/
/*						BB2																		*/
/*						BBU_1A	X			 	 X		 X	    X 		X					*/
/*						BBU_1B				 			 X	    X			X					*/
/*						BBU_2A	X				 X		 X	    X			X					*/
/*						BBU_2B							 X	    X			X					*/
/*						BBU_3A												X					*/
/*						BBU_3B												X					*/
/*						SDI				X					    X								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void WriteErrorLN2( UC LN2Pos, UL error) {

	char tmpbuf[40];

	WriteCodeLN2( LN2Pos, ErrorNoHdrTxt);				// Write error text header

	sprintf( tmpbuf, "%03d", ( UC) ( error & 0x000000FF) );
	WriteTxtLN2( LN2Pos+2, tmpbuf);						// Write error number

	GetErrorString( error, tmpbuf);

	WriteTxtLN2( LN2Pos+8, tmpbuf);
}

/***************************************************************************/
/* GetErrorString															     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 991026	    										*/
/* Revised:		991026				     													*/
/*																									*/
/*	Function:	Compose and return an error string with the actual error		*/
/*	Remarks:	 	Translation of the 32-bit parameter, error:						*/
/*					16MSB				8 BIT		8 LSB											*/
/*					level detector	[index]	ErrorType ErrorCode						*/
/*	Returns:		--																				*/
/*	Updates:																						*/
/***************************************************************************/
void GetErrorString( UL error, char* tmpbuf) {

	UI LevelError;
	UC errorindex, errortype, errorcode;
	code char* TxtPtr;

	FeedTheWatchdog;

	LevelError = ( UI) ( error >> 16);

	errorindex = ( UC) (( error & 0x0000FF00) >> 8);
	errorcode = ( UC) ( error & 0x000000FF);

	errortype = ( errorcode / 10) * 10;
	errorcode -= errortype;

	if ( errortype == SystemErrorType) {				// If any system error..

		switch ( errorcode) {
			case LevelDetectorError:

				Cstrcpy( tmpbuf, LevelDetectorTxt);

				TxtPtr = UnknownOptionTxt;					// Default: Unknown Option

				if ( AESEBUUnit.Present && ( AESEBUUnit.HWType == PT8635)) {

					if (( LevelError & 0x4000) == 0) 	// PT8635 BNC Word #2
						TxtPtr = AESEBUWord2Txt;

					if (( LevelError & 0x2000) == 0) 	// PT8635 BNC Word #1
						TxtPtr = AESEBUWord1Txt;

					if (( LevelError & 0x1000) == 0) 	// PT8635 XLR generator #2
						TxtPtr = AESEBUXLR2Txt;

					if (( LevelError & 0x0800) == 0) 	// PT8635 XLR generator #1
						TxtPtr = AESEBUXLR1Txt;

					if (( LevelError & 0x0400) == 0) 	// PT8635 BNC generator #2
						TxtPtr = AESEBUBNC2Txt;

					if (( LevelError & 0x0200) == 0) 	// PT8635 BNC generator #1
						TxtPtr = AESEBUBNC1Txt;
				}

				if (( LevelError & 0x0100) == 0) 		// SDI-TPG1 Generator
					if ( SDITPGUnit[0].Present)
						TxtPtr = SDITPG1Txt;

																	// BBU_3B connector
				if (( LevelError & 0x0080) == 0) { 		// TSG4, BB6

					if ( AnlBlkUnit[BB6Item].Present)
						TxtPtr = BBArrTxt[BB6Item];
					else
						if ( SDITSGUnit[2].Present)
							TxtPtr = SDITSG4Txt;
				}

																	// BBU_3A connector
				if (( LevelError & 0x0040) == 0) {		// TSG4, BB5

					if ( AnlBlkUnit[BB5Item].Present)
						TxtPtr = BBArrTxt[BB5Item];
					else
						if ( SDITSGUnit[2].Present)
							TxtPtr = SDITSG4Txt;
				}

																	// BBU_2B connector
				if (( LevelError & 0x0020) == 0) {		// BB8, ATPG2V2, ATPG5V2,
																	// TSG2, STPG2, STPG5
					if ( AnlBlkUnit[BB8Item].Present)
						TxtPtr = BBArrTxt[BB8Item];
					else
						if ( AnlTPGUnit[0].Present || AnlTPGUnit[1].Present) {
							if ( AnlTPGUnit[0].Present && AnlTPGUnit[1].Present)
								TxtPtr = AnalogTPG5Txt;
							else
								TxtPtr = AnalogTPG2Txt;
						}
					else
						if ( SDITSGUnit[0].Present)
							TxtPtr = SDITSG2Txt;
					else
						if ( SDITPGUnit[1].Present || SDITPGUnit[2].Present) {
							if ( SDITPGUnit[1].Present && SDITPGUnit[2].Present)
								TxtPtr = SDITPG5Txt;
							else
								TxtPtr = SDITPG2Txt;
						}
				}

																	// BBU_2A connector
				if (( LevelError & 0x0010) == 0) {		// BB7, ATPG2, ATPG5, TSG2,
																	// STPG2, STPG5,
					if ( AnlBlkUnit[BB7Item].Present)
						TxtPtr = BBArrTxt[BB7Item];
					else
						if ( AnlTPGUnit[0].Present || AnlTPGUnit[1].Present) {
							if ( AnlTPGUnit[0].Present && AnlTPGUnit[1].Present)
								TxtPtr = AnalogTPG5Txt;
							else
								TxtPtr = AnalogTPG2Txt;
						}
					else
						if ( SDITSGUnit[0].Present)
							TxtPtr = SDITSG2Txt;
					else
						if ( SDITPGUnit[1].Present || SDITPGUnit[2].Present) {
							if ( SDITPGUnit[1].Present && SDITPGUnit[2].Present)
								TxtPtr = SDITPG5Txt;
							else
								TxtPtr = SDITPG2Txt;
						}
				}

																	// BBU_1B connector
				if (( LevelError & 0x0008) == 0) {		// BB4, ATPG2, TSG3, STPG2
																	// BB+Mul
					if ( AnlBlkUnit[BB4Item].Present)
						TxtPtr = BBArrTxt[BB4Item];
					else
						if ( AnlBlkUnit[BB2Item].HWType == PT8604)
							TxtPtr = BBMulTxt;
					else
						if ( AnlTPGUnit[0].Present)
							TxtPtr = AnalogTPG2Txt;
					else
						if ( SDITSGUnit[1].Present)
							TxtPtr = SDITSG3Txt;
					else
						if ( SDITPGUnit[1].Present)
							TxtPtr = SDITPG2Txt;
				}

																	// BBU_1A connector
				if (( LevelError & 0x0004) == 0) {		// BB3, ATPG2, TSG3, STPG2
																	// BB+Mul
					if ( AnlBlkUnit[BB3Item].Present)
						TxtPtr = BBArrTxt[BB3Item];
					else
						if ( AnlBlkUnit[BB2Item].HWType == PT8604)
							TxtPtr = BBMulTxt;
					else
						if ( AnlTPGUnit[0].Present)
							TxtPtr = AnalogTPG2Txt;
					else
						if ( SDITSGUnit[1].Present)
							TxtPtr = SDITSG3Txt;
					else
						if ( SDITPGUnit[1].Present)
							TxtPtr = SDITPG2Txt;
				}

				if (( LevelError & 0x0002) == 0) 			// BB2
					TxtPtr = BBArrTxt[BB2Item];

				if (( LevelError & 0x0001) == 0) 			// BB1
					TxtPtr = BBArrTxt[BB1Item];

				Cstrcat( tmpbuf, TxtPtr);
				break;

			case ConfigurationError:
				Cstrcpy( tmpbuf, ConfigurationErrorTxt);
				break;

			case MultipleError:
				Cstrcpy( tmpbuf, MultipleErrorTxt);
				break;

			case MultipleFailure:
				Cstrcpy( tmpbuf, MultipleFailureTxt);
				break;

			case PowerN5Failure:
				Cstrcpy( tmpbuf, PowerFailureTxt);
				Cstrcat( tmpbuf, VoltageN5Txt);
				break;

			case Power12Failure:
				Cstrcpy( tmpbuf, PowerFailureTxt);
				Cstrcat( tmpbuf, Voltage12Txt);
				break;

			case TemperatureFailure:
				Cstrcpy( tmpbuf, TemperatureFailureTxt);
				break;

			case TemperatureWarning:
				Cstrcpy( tmpbuf, TemperatureWarningTxt);
				break;
		}
	}
	else {
		switch ( errorcode) {
			case 0:
				TxtPtr = GeneralFailureTxt;
				break;

			case 1:
				TxtPtr = NoContactToTxt;
				break;

			case 2:
				TxtPtr = ErrorWritingToTxt;
				break;

			case 3:
				TxtPtr = NoResponseFromTxt;
				break;

			case 4:
				TxtPtr = ErrorReadingFromTxt;
				break;

			case ChecksumError:
				TxtPtr = ChecksumErrorTxt;
				break;

			default:
				TxtPtr = GeneralFailureTxt;
				break;
		}
		Cstrcpy( tmpbuf, TxtPtr);

		switch ( errortype) {
			case AnlBlkErrorType:
				TxtPtr = BBArrTxt[errorindex];
				break;

			case SDITSGErrorType:
				TxtPtr = base_menu[SDITSG2Item + errorindex].ItemTxt;
				break;

			case AESEBUErrorType:
				TxtPtr = AESEBU_menu[errorindex].ItemTxt;
				break;

			case AnlTPGErrorType:
				TxtPtr = base_menu[AnlTPG2Item + errorindex].ItemTxt;
				break;

			case SDITPGErrorType:
				TxtPtr = base_menu[SDITPG1Item + errorindex].ItemTxt;
				break;

			case SDIGenlockErrorType:
				TxtPtr = SDIGenlockTxt;
				break;

			case TimeClockErrorType:
				TxtPtr = TimeCodeTxt;
				break;

			case OSCErrorType:
				TxtPtr = OSCTxt;
				break;

			case ROMMemoryErrorType:
				TxtPtr = ROMMemoryTxt;
				break;

			case RAMMemoryErrorType:
				TxtPtr = RAMMemoryTxt;
				break;

			case KeyboardErrorType:
				TxtPtr = KeyboardTxt;
				break;

			case LCD_ADCErrorType:
				TxtPtr = LCDADConvTxt;
				break;

			case TempADCErrorType:
				TxtPtr = TEMPADConvTxt;
				break;

			case VoltageADCErrorType:
				TxtPtr = VoltageTxt;
				break;

			case LevelDetectorErrorType:
				TxtPtr = LevelDetTxt;
				break;

			default:
				TxtPtr = UnknownOptionTxt;
				break;
		}

		Cstrcat( tmpbuf, TxtPtr);
	}
}

/***************************************************************************/
/* DisplayErrorLine														     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980610				     													*/
/*																									*/
/*	Function:	Display an error on line 2 without disturbing pointer Ptr	*/
/*	Remarks:		This function is ONLY used during power up. It will record 	*/
/*					ALL errors. It WILL record any error the number of times		*/
/*					it occurs.																	*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DisplayErrorLine( UI error, UC timer) {

	UL ErrorVar;

	ErrorStatus = error;							// Update error status
	ErrorVar = ( UL) 0x10000*LevelStatus + ( UL) ErrorStatus;

	if ( ++ErrorQueuePtr > NoInErrorQueue-1)	// If next queue entry GT. max..
		ErrorQueuePtr = 0;							//  set pointer to 0

	ErrorQueue[ErrorQueuePtr] = ErrorVar;	// Save extended error status

	CharBlinkLN2( 0, OFF);						// Switch off possible blinking

	UpdateLEDs();									// Update LEDs

	WriteErrorLN2( 0, ErrorVar);				// Write error in display
	ClearRestLN2();

	if ( timer) {									// If time enabled..
		DisplayTimer = timer;					//  display error line for timer*20ms

		while ( DisplayTimer)
			FeedTheWatchdog;
	}
}

/***************************************************************************/
/* GotoErrorStatus														     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980610				     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		index:8 error:8															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void GotoErrorStatus( UI error) {

	UL ErrorVar;

	if ( ErrorStatus && ( ErrorStatus < 10)) {	// If system error type active.
		if ( error > ErrorStatus)						//  only higher priority errors
			return;											//  OR level errors are allowed

		if ( error == ErrorStatus)
			if ( error != ( SystemErrorType + LevelDetectorError))
				return;
	}

	if ( MenuModeOn) {							// If in menu mode..
		if ( SelectModeOn) {						// If in selection mode..
			Ptr->EscapeFct();						//  be sure to terminate NICELY!!
			Ptr->DispFunct();
		}

		UpToStatus();								// Update ptr to status-display
		Ptr->DispFunct();							// Display status-display
	}

	ErrorStatus = error;								// Update error status
	ErrorVar = (UL) 0x10000*LevelStatus + ( UL) ErrorStatus;

	if ( ++ErrorQueuePtr > NoInErrorQueue-1)	// If next queue entry GT. max..
		ErrorQueuePtr = 0;							//  set pointer to 0

	ErrorQueue[ErrorQueuePtr] = ErrorVar;	// Save extended error status

	ErrorWindowOn = TRUE;

	CharBlinkLN2( 0, OFF);						// Switch off possible blinking

	UpdateGrpLEDs( NoGroup);					// Switch OFF ALL goup LEDs
	UpdateLEDs();									// Update LEDs

	WriteErrorLN2( 0, ErrorVar);				// Write error in display
	ClearRestLN2();

	WriteArrowsField( L_+D_+U_+R_+E_);

	StatusNdx = StatusErrorItem;				// Force menupointer to STATUS-
	Ptr = &status_menu[StatusNdx];			//  ERROR display
}

/***************************************************************************/
/* GetPT5230SWVersion												 	     MESSAGE.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980208	    										*/
/* Revised:		980215				     													*/
/*																									*/
/*	Function:	Return the SW version of the main- & oscillator-board			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		string which contains the version as: xx.x-yy.y					*/
/***************************************************************************/
void GetPT5230SWVersion( char* string) {

	UC tmp, error;

	tmp = ( UC) RecInt( sync_addr, "GI", 0xFF, &error);

	if ( error)
		tmp = 0;

	sprintf( string, "%04.1f-%04.1f\0", PT5230SWVersion/10.0, tmp/10.0);
}
