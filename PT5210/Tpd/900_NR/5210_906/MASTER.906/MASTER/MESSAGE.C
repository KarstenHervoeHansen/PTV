/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MESSAGE.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	970202																		*/
/*	Rev. date:	971212, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void MessageHandling( UC messageno);											*/
/* 	void SetupRS232GotoLocalMessage();												*/
/*		void RS232GotoLocalMessage();														*/
/*		void WriteErrorLN2( UI errorcode);												*/
/*		void DisplayErrorLine( UI error, UC timer);									*/
/*		void GotoErrorStatus( UI error);													*/
/*		void ReadSWVersion( char* string);												*/
/*                                                                         */
/* Changes:                                                                */
/* 971212 Added errormessage ErrorInDataFromRS232									*/
/* 971023 All requests are replaced by calls to RecInt, RecLong or RecChar	*/
/* 971021 SndRequest automatically inserts the questionmark.					*/
/* 971009 Cleanup																				*/
/* 970911 ReadSWVersion altered to GetPT5210SWVersion								*/
/* 970821 Major redesign of WriteErrorLN2(), DisplayErrorLine() & Goto-		*/
/*			  ErrorStatus()																	*/
/* 970725 In GotoErrorStatus(): If MenuModeOn and/or SelectModeOn when		*/
/*			  entering this routine, the menu/select display will now be		*/
/*			  terminated nicely.																*/
/* 970724 Added feed_dog update in DisplayErrorLine()								*/
/* 970723 Added SetupRS232GotoLocalMessage()											*/
/*        Major change: Ptr->DispShow() & Ptr->DispUpdate has now been		*/
/*			  now been replaced by one function: Ptr->DispFunct();				*/
/* 970722 In RS232GotoLocalMessage: reference now made to PrevPtr				*/
/* 970226 Added timer parameter to DisplayErrorLine								*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xa.h>

#include "mas.h"

#include <ctype.h>

#include "52sync.h"
#include "sw5210.h"
#include "message.h"
#include "menu.h"
#include "menuhlp.h"
#include "menutree.h"
#include "disp_drv.h"
#include "text.h"
#include "piic.h"
#include "serial1.h"
#include "keyb_drv.h"

/***************************************************************************/
/* MessageHandling														     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971212				     													*/
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
			WriteCode2( SerialRemoteOperatingTxt);
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
/* Revised:		970723				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		--																				*/
/*	Updates:																						*/
/***************************************************************************/
void RS232GotoLocalMessage() {

	if ( FuncSave || FuncEscape) {

		if ( FuncSave)							// If OK selected..
			SerialRemoteActive = FALSE;	//  goto local mode

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
/* Revised:		970821				     													*/
/*																									*/
/*	Function:	Write an error text based on <errorcode> on line 2, LN2Pos 	*/
/*	Remarks:	 	Translation of the 32-bit parameter, error:						*/
/*					16MSB				8 BIT		8 LSB											*/
/*					level detector	[index]	ErrorType ErrorCode						*/
/*	Returns:		--																				*/
/*	Updates:																						*/
/***************************************************************************/
void WriteErrorLN2( UC LN2Pos, UL error) {

	UI LevelError;
	UC errorindex, errortype, errorcode;
	char tmpbuf[4];
	code char* TxtPtr;

	LevelError = ( UI) ( error >> 16);

	errorindex = ( UC) (( error & 0x0000FF00) >> 8);
	errorcode = ( UC) ( error & 0x000000FF);

	WriteCodeLN2( LN2Pos, ErrorNoHdrTxt);				// Write error text header

	sprintf( tmpbuf, "%03d", ( UI) errorcode );
	WriteTxtLN2( LN2Pos+2, tmpbuf);						// Write error number

	errortype = ( errorcode / 10) * 10;
	errorcode -= errortype;

	if ( errortype == SystemErrorType) {				// If any system error..

		switch ( errorcode) {
			case LevelDetectorError:

				WriteCodeLN2( LN2Pos+8, LevelDetectorTxt);

				TxtPtr = UnknownOptionTxt;					// Default -> Unknown Option

				if (( LevelError & 0x0100) == 0) {		// SDI Signal Generator
					if ( SDISignalUnit.Present)
						TxtPtr = SDISignalTxt;
				}

				if (( LevelError & 0x0080) == 0) {		// BB6/SB56
					if ( AnlBlkUnit[BB6Item].Present)
						TxtPtr = BBArrTxt[BB6Item];
					else {
						if ( SDIBlkUnit[SB56Item].Present)
							TxtPtr = SBArrTxt[SB56Item];
					}
				}

				if (( LevelError & 0x0040) == 0) {		// BB5/SB56
					if ( AnlBlkUnit[BB5Item].Present)
						TxtPtr = BBArrTxt[BB5Item];
					else {
						if ( SDIBlkUnit[SB56Item].Present)
							TxtPtr = SBArrTxt[SB56Item];
					}
				}

				if (( LevelError & 0x0020) == 0) {		// BB8/SB78/Analog signal
					if ( AnlBlkUnit[BB8Item].Present)
						TxtPtr = BBArrTxt[BB8Item];
					else {
						if ( SDIBlkUnit[SB78Item].Present)
							TxtPtr = SBArrTxt[SB78Item];
						else {
							if ( AnalogSignalUnit.Present)
								TxtPtr = AnalogSignalTxt;
						}
					}
				}

				if (( LevelError & 0x0010) == 0) {		// BB7/SB78/Analog signal
					if ( AnlBlkUnit[BB7Item].Present)
						TxtPtr = BBArrTxt[BB7Item];		// BB
					else {
						if ( SDIBlkUnit[SB78Item].Present)
							TxtPtr = SBArrTxt[SB78Item];	// SB78
						else {
							if ( AnalogSignalUnit.Present)
								TxtPtr = AnalogSignalTxt;	// Analog unit
						}
					}
				}

				if (( LevelError & 0x0008) == 0) {		// BB4/SB34
					if ( AnlBlkUnit[BB4Item].Present)
						TxtPtr = BBArrTxt[BB4Item];
					else {
						if ( SDIBlkUnit[SB34Item].Present)
							TxtPtr = SBArrTxt[SB34Item];
					}
				}

				if (( LevelError & 0x0004) == 0) {		// BB3/BB-Par/SB34
					if ( AnlBlkUnit[BB3Item].Present)
						TxtPtr = BBArrTxt[BB3Item];
					else {
						if ( Connectors[BB34Conn].HWType == PT8604)
							TxtPtr = BBMulTxt;				// Laves om til BB-Par
						else {
							if ( SDIBlkUnit[SB34Item].Present)
								TxtPtr = SBArrTxt[SB34Item];
						}
					}
				}

				if (( LevelError & 0x0002) == 0) 			// BB2
					TxtPtr = BBArrTxt[BB2Item];

				if (( LevelError & 0x0001) == 0) 			// BB1
					TxtPtr = BBArrTxt[BB1Item];

				WriteCode2( TxtPtr);
				break;

			case ConfigurationError:
				WriteCodeLN2( LN2Pos+8, ConfigurationErrorTxt);
				break;

			case MultipleError:
				WriteCodeLN2( LN2Pos+8, MultipleErrorTxt);
				break;

			case MultipleFailure:
				WriteCodeLN2( LN2Pos+8, MultipleFailureTxt);
				break;

			case PowerN5Failure:
				WriteCodeLN2( LN2Pos+8, PowerFailureTxt);
				WriteCode2( VoltageN5Txt);
				break;

			case Power12Failure:
				WriteCodeLN2( LN2Pos+8, PowerFailureTxt);
				WriteCode2( Voltage12Txt);
				break;

			case TemperatureFailure:
				WriteCodeLN2( LN2Pos+8, TemperatureFailureTxt);
				break;

			case TemperatureWarning:
				WriteCodeLN2( LN2Pos+8, TemperatureWarningTxt);
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
		WriteCodeLN2( LN2Pos+8, TxtPtr);

		switch ( errortype) {
			case AnlBlkErrorType:
				TxtPtr = BBArrTxt[errorindex];
				break;

			case SDIBlkErrorType:
				TxtPtr = SBArrTxt[errorindex];
				break;

			case AES_EBUErrorType:
				TxtPtr = AudioTxt;
				break;

			case AnlSignalErrorType:
				TxtPtr = AnalogSignalTxt;
				break;

			case SDISignalErrorType:
				TxtPtr = SDISignalTxt;
				break;

			case SDIGenlockErrorType:
				TxtPtr = SDIGenlockTxt;
				break;

			case TimeCodeErrorType:
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
				TxtPtr = LCD_ADConvTxt;
				break;

			case TempADCErrorType:
				TxtPtr = TEMP_ADConvTxt;
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

		WriteCode2( TxtPtr);
	}
}

/***************************************************************************/
/* DisplayErrorLine														     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970821				     													*/
/*																									*/
/*	Function:	Display an error on line 2 without disturbing pointer Ptr	*/
/*	Remarks:																						*/
/*	Returns:		--																				*/
/*	Updates:																						*/
/***************************************************************************/
void DisplayErrorLine( UI error, UC timer) {

	UL ErrorVar;

	if ( ErrorStatus && ( ErrorStatus < 10))	// If system error type active..
		if ( error >= ErrorStatus)					//  only higher priority errors
			return;										//  are allowed

	ErrorStatus = error;							// Update error status
	ErrorVar = ( UL) 0x10000*LevelStatus + ( UL) ErrorStatus;

	if ( ++ErrorQueuePtr > NoInErrorQueue-1)	// If next queue entry GT. max..
		ErrorQueuePtr = 0;							//  set pointer to 0

	ErrorQueue[ErrorQueuePtr] = ErrorVar;	// Save extended error status

	CharBlinkLN2( 0, OFF);						// Switch off possible blinking

	Warning_LED = ON;								// Errors wil switch ON Warning LED
	Update_LEDs();									// Update LEDs

	WriteErrorLN2( 0, ErrorVar);				// Write error in display
	ClearRestLN2();

	if ( timer) {									// If time enabled..
		DisplayTimer = timer;					//  display error line for timer*20ms

		while ( DisplayTimer)
			feed_dog;
	}
}

/***************************************************************************/
/* GotoErrorStatus														     MESSAGE.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970821				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		index:8 error:8															*/
/*	Returns:		--																				*/
/*	Updates:																						*/
/***************************************************************************/
void GotoErrorStatus( UI error) {

	UL ErrorVar;

	if ( ErrorStatus && ( ErrorStatus < 10))	// If system error type active..
		if ( error >= ErrorStatus)
			return;

	if ( MenuModeOn) {			// If in menu mode..
		if ( SelectModeOn) {		// If in selection mode..
			Ptr->EscapeFct();		//  be sure to terminate NICELY!!
			Ptr->DispFunct();
		}

		UpToStatus();				// Update ptr to status-display
		Ptr->DispFunct();			// Display status-display
	}

	ErrorStatus = error;							// Update error status
	ErrorVar = (UL) 0x10000*LevelStatus + ( UL) ErrorStatus;

	if ( ++ErrorQueuePtr > NoInErrorQueue-1)	// If next queue entry GT. max..
		ErrorQueuePtr = 0;							//  set pointer to 0

	ErrorQueue[ErrorQueuePtr] = ErrorVar;	// Save extended error status

	ErrorWindowOn = TRUE;

	CharBlinkLN2( 0, OFF);						// Switch off possible blinking

	Warning_LED = ON;								// Errors wil switch ON Warning LED
	Update_LEDs();									// Update LEDs

	WriteErrorLN2( 0, ErrorVar);				// Write error in display
	ClearRestLN2();

	WriteArrowsField( L_+D_+U_+R_+E_);

	StatusNdx = StatusErrorItem;				// Force menupointer to STATUS-
	Ptr = &status_menu[StatusNdx];			//  ERROR display
}

/***************************************************************************/
/* GetPT5210SWVersion												 	     MESSAGE.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971023				     													*/
/*																									*/
/*	Function:	Return the SW version of the main- & oscillator-board			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		string which contains the version as: xx.x-yy.y					*/
/***************************************************************************/
void GetPT5210SWVersion( char* string) {

	UC tmp, error;

	tmp = ( UC) RecInt( sync_addr, "GI", 0xFF, &error);

	if ( error)
		tmp = 0;

	sprintf( string, "%04.1f-%04.1f\0", PT5210SWVersion/10.0, tmp/10.0);
}
