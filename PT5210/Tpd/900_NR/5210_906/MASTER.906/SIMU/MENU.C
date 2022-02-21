/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MENU.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960713																		*/
/*	Rev. date:	980113, KEn DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*																									*/
/*	Changes:																						*/
/* 980113: /906 version. In SDIAudioSignalSelect() only Off and Stereo 1kHz*/
/*				are allowed.																	*/
/* 971008: /906 version. Philips motion pattern removed							*/
/* 980106: /906 version. Added /906 in apparatus readout DiagnoseConfigTest*/
/* 971215: /906 version. Removed Text information in SDI Status Display, 	*/
/*				StatusSDISignalUpdate()														*/
/* 971215: /906 version. TextBuffer increased to 17 characters. Altered		*/
/*				SDIDown() & SDITextSelect(). Altered call to CheckTextASCII		*/
/* 971212: Minor changes																	*/
/* 971027: Finishing DiagnoseMainTest()												*/
/* 971024: Finishing DiagnoseOptionsTest()											*/
/* 971020: In DiagnoseErrorQueueReset: The brackets in line 1 is removed	*/
/* 971016: Enhanced all diagnose functions but mainly DiagnoseRS232Test()	*/
/* 971010: Added parameters in call to Udr_InitializeUart()						*/
/* 970924: removed UpToDiagnose()														*/
/* 970918: Implementing Diagnose functions 											*/
/* 970909: In DiagnoseConfigTest(): Update to also display OSC Connector	*/
/* 970827: In StatusErrorUpdate(): Every time the routine was executed		*/
/*			  the present error was added to the errorqueue. WRONG!!!			*/
/* 970723: Major change: Ptr->DispShow() & Ptr->DispUpdate has now been		*/
/*			  now been replaced by one function: Ptr->DispFunct();				*/
/* 970627: In ConfigLockSelect(): SAVE now disappears when the present 		*/
/*			  selection pops up, just as all the other menus.						*/
/* 970626: In aaTimeAdjust(): GenlockTiming is ALWAYS transmitted to the	*/
/*			  generators whether the system is genlocked or not.					*/
/* 970625: GenlockTimeSelect(). Value was not restored when selecting ESC 	*/
/*			  in Genlock Time menu															*/
/* 970303: Error in SDITimeAdjust corrected. Pressing genlock button in		*/
/*			  genlock time or sys will exit to Genlock input						*/
/* 970226: RS232 menu items has been changed											*/
/* 970223: Major updates especially to the Download menus						*/
/* 970217: Recall of a preset now returns display to status preset display	*/
/* 970211: Added Diagnose version and configuration								*/
/* 970205: Remove temporarily access to SPG download. Added new texts		*/
/* 970201: Added diagnose test functions												*/
/* 970107: Implementation of error handling											*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mas.h"

#include "52sync.h"
#include "menu.h"
#include "menuhlp.h"
#include "message.h"
#include "menutree.h"
#include "timing.h"
#include "disp_drv.h"
#include "cont_drv.h"
#include "keyb_drv.h"
#include "text.h"
#include "tables.h"
#include "util.h"
#include "serial1.h"
#include "unit_drv.h"
#include "piic.h"
#include "rs232par.h"
#include "rs232err.h"
#include "xaexprt.h"
#include "xadrivr.h"


// Definitions and constants

#define stepNdxAnalogMax		5
#define stepNdxAnalogMin		0

code long stepsizeAnalogTable[6] = {
	1, 7, 69, 691, 6910, 69100
};

#define stepNdxSDIMax		3
#define stepNdxSDIMin		0

code long stepsizeSDITable[4] = {
	256, 768, 6912, 69120
};

#define AnalogSignalMask   1
#define SDISignalMask      2
#define AES_EBUSignalMask  4
#define SDIBlkMask			8

// Temporary variables PUBLIC

near UC StatusNdx;					// Last active Display item
near UC BaseNdx;						//     -"-     base-menu item
near UC AnlBlkNdx;					//     -"-     analog black menu item
near UC BBNdx[NoOfAnlBlkUnits];	//     -"-     black-burst menu items
near UC SDIBlkNdx;					//     -"-     SDI black menu item
near UC SBNdx[NoOfSDIBlkUnits];	//     -"-     serial-black menu items
near UC AnlSigNdx;					//     -"-     analog-signal menu item
near UC SDISigNdx;					//     -"-     digital-signal menu item
near UC AES_EBUNdx;	  				//     -"-     AES-EBU out item
near UC PresetNdx;	  				//     -"-     Preset item
near UC ConfigNdx;	  				//     -"-     Config item
near UC RS232Ndx;	  					//     -"-     RS232 item
near UC DiagnoseNdx; 				//     -"-     Diagnose item


// Temporary variables used specific in the download functions

UL ResponseKUNo;
UC ResponseSWVersion;


// Temporary variables PRIVATE NOT overlayable

static char TextBuffer[17];		// Temporary text buffer

static near UC UcharVal;			// Temporary unsigned char value
static near UC UcharVal1;			//  do.
static near UC UcharVal2;			//  do.

static near int IntVal;				// Temporary integer value

static code char* near CodePtr;	// Pointer used for text arrays in CODE
static char* near TxtPtr;			// Pointer used for text arrays in DATA

static struct STATUS_FLAGS TmpFlags;


// Temporary variables used specific in the timing functions

static near long samples;
static near long stepVal;
static UC stepNdx;


/***************************************************************************/
/*	OutputButtonSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970304				     													*/
/*																									*/
/*	Function:	Handles the front plate OUTPUT button which is a shortcut 	*/
/*					key to the signal menus in Analog-, SDI-Signal and AES-EBU.	*/
/*	Remarks:																						*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, AnlSigNdx, SDISigNdx, AES_EBUNdx, ActiveOutputNo,		*/
/*					MenuModeOn																	*/
/***************************************************************************/
void OutputButtonSelect(){

	if ( !Flags.PanelLockOn) {						// If PANEL LOCK is disabled

															//  if a option is installed..
		if ( AnalogSignalUnit.Present || SDISignalUnit.Present ||
			  AES_EBUUnit.Present) {

			if ( MenuModeOn && SelectModeOn) {	// If in selection mode ..
				Ptr->EscapeFct();						// Be sure to terminate NICELY
				Ptr->DispFunct();
			}

			MenuModeOn = SelectModeOn = ON;		// Set menu- & select-mode

			switch ( ActiveOutputNo) {
				case AnalogSignalItem:
					if ( SDISignalUnit.Present)
						ActiveOutputNo = SDISignalItem;
					else {
						if ( AES_EBUUnit.Present)
							ActiveOutputNo = AES_EBUItem;
					}
					break;

				case SDISignalItem:
					if ( AES_EBUUnit.Present)
						ActiveOutputNo = AES_EBUItem;
					else {
						if ( AnalogSignalUnit.Present)
							ActiveOutputNo = AnalogSignalItem;
					}
					break;

				case AES_EBUItem:
					if ( AnalogSignalUnit.Present)
						ActiveOutputNo = AnalogSignalItem;
					else {
						if ( SDISignalUnit.Present)
							ActiveOutputNo = SDISignalItem;
					}
					break;
			}

			switch ( BaseNdx = ActiveOutputNo) {
				case AnalogSignalItem:					// Analog Signal
					AnlSigNdx = AnalogPatternItem;	// Select ANALOG-SIGNAL
					Ptr = &anlsig_menu[AnlSigNdx];	//  PATTERN SUBMENU

					AnalogDown();
					break;

				case SDISignalItem:						// SDI Signal
					SDISigNdx = SDIPatternItem;		// Select SDI-SIGNAL
					Ptr = &SDIsig_menu[SDISigNdx];	//  PATTERN SUBMENU

					SDIDown();
					break;

				case AES_EBUItem:							// AES-EBU
					AES_EBUNdx = AES_EBUSignalItem;	// Select AES-EBU SIGNAL
					Ptr = &AES_EBU_menu[AES_EBUNdx];	//  SUBMENU

					AES_EBUDown();
					break;
			}
		}
	}
}

/***************************************************************************/
/*	PresetButtonSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970304				     													*/
/*																									*/
/*	Function:	Handles the front plate PRESET button which is a shortcut	*/
/*					key to the RECALL PRESET menu											*/
/*	Remarks:		If NOT in menu-mode, the PRESET button will activate the		*/
/*					PRESET RECALL menu. When in the PRESET RECALL menu, pressing*/
/*					the PRESET button will function as the UP button				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, PrevPtr, BaseNdx, PresetNdx, MenuModeOn						*/
/***************************************************************************/
void PresetButtonSelect(){

	if ( !Flags.PanelLockOn) {						// If PANEL LOCK is disabled..

		if ( MenuModeOn && ( Ptr == &preset_submenu[PresetRecallItem]))
			Ptr->UpBut();
		else {
			if ( MenuModeOn && SelectModeOn) {	// If in selection mode ..
				Ptr->EscapeFct();						// Be sure to terminate NICELY
				Ptr->DispFunct();
			}

			MenuModeOn = SelectModeOn = ON;		// Set menu- & select-mode

			BaseNdx = PresetItem;					// Select PRESET MENU

			PresetNdx = PresetRecallItem;			// Select PRESET RECALL SUBMENU
			PrevPtr = Ptr = &preset_menu[PresetNdx];

			PresetDown();
		}
	}
}

/***************************************************************************/
/*	GenlockButtonSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970304				     													*/
/*																									*/
/*	Function:	Handles the front plate GENLOCK button which toggles 			*/
/*					between internal genlock and last active external genlock	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		ActiveGenlockNo, MenuModeOn, UcharVal, FuncState				*/
/***************************************************************************/
void GenlockButtonSelect(){

	if ( !Flags.PanelLockOn) {						// If PANEL LOCK is disabled..

		if ( ActiveGenlockNo == Internal)
			ActiveGenlockNo = ExternalGenlockNo;
		else
			ActiveGenlockNo = Internal;

		if ( MenuModeOn) {
			if (( Ptr == &genlocksys_menu) || ( Ptr == &genlocktime_menu)) {
				Ptr = &genlock_menu;				//   Point to genlock-menu

				UcharVal = ActiveGenlockNo;
				UcharVal1 = GenlockConfig[UcharVal].System;

				FuncState = 1;						//   Prepare for item state-machine

				GenlockInputShow(); 				//   Display the menu
			}
			else {
				if ( Ptr == &genlock_menu) {
					UcharVal = ActiveGenlockNo;
					UcharVal1 = GenlockConfig[UcharVal].System;

					FuncState = 1;
				}
			}
		}
															// Update the genlock unit
		if ( ErrorCode = GenlockUnitDrv( ActiveGenlockNo,\
										 GenlockConfig[ActiveGenlockNo].System)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}
}

/***************************************************************************/
/*	StatusPresetUpdate															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970205				     													*/
/*																									*/
/*	Function:	Display the name and number of a selected preset				*/
/*	Remarks:		STATUS DISPLAY function													*/
/*					Cursor position on line 1 and 2 are updated						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusPresetUpdate() {

	WriteCodeLN1( 0, Status_PresetTxt);			// Write line 1 text
	ClearRestLN1();									// Clear rest of line 1

	if ( ActivePresetNo) {
		WriteCodeLN2( 0, PresetHdrTxt);
		WriteCharLN2( 8, ActivePresetNo + '0');

		WriteTxtLN2( 11, Preset[ActivePresetNo-1].Name);
	}
	else
		WriteCodeLN2( 0, NoPresetActiveTxt);

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusGenlockUpdate															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display actual Genlock status, ie. selected by user			*/
/*	Remarks:		STATUS DISPLAY function													*/
/*					Cursor position on line 1 and 2 are updated						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusGenlockUpdate() {

	WriteCodeLN1( 0, GenlockHdrTxt);
	WriteCode1( GenlockInputArrTxt[ActiveGenlockNo]);
	ClearRestLN1();

	WriteCodeLN2( 0, SignalHdrTxt);

	if ( ActiveGenlockNo >= Internal) {			// If internal genlock..
		WriteCode2( HyphenTxt);						// No signal information

		ClearToPosLN2( 20);

		WriteCode2( StatusHdrTxt);
		WriteCode2( HyphenTxt);						// No status information
	}
	else {												// ..external genlock active
		WriteCode2( GenlockSignalArrTxt[GenlockConfig[ActiveGenlockNo].System]);

		ClearToPosLN2( 20);

		WriteCode2( StatusHdrTxt);

		if ( Unlocked_LED)
			WriteCode2( UNLOCKEDTxt);
		else
			WriteCode2( GENLOCKEDTxt);
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusAnalogSignalUpdate													  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display actual Analog Signal status, ie. selected by user	*/
/*	Remarks:		STATUS DISPLAY function													*/
/*					Cursor position on line 1 and 2 are updated						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusAnalogSignalUpdate() {

	WriteCodeLN1( 0, AnalogSignalHdrTxt);
	WriteCode1( AnalogPatternArrTxt[AnalogConfig.Pattern]);
	ClearRestLN1();

	WriteCodeLN2( 0, SystemHdrTxt);
	WriteCode2( AnaTVSystemArrTxt[AnalogConfig.System]);

	ClearToPosLN2( 22);

	if ( AnalogConfig.TextOn) {
		WriteCode2( TextHdrTxt);
		WriteTxt2( AnalogConfig.TextInfo);
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusSDISignalUpdate														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971217				     													*/
/*																									*/
/*	Function:	Display actual SDI Signal status, ie. selected by user		*/
/*	Remarks:		STATUS DISPLAY function													*/
/*					Cursor position on line 1 and 2 are updated						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusSDISignalUpdate() {

	WriteCodeLN1(0, SDISignalHdrTxt);
	WriteCode1( SDIPatternArrTxt[SDIConfig.Pattern]);
	ClearRestLN1();

	WriteCodeLN2( 0, SystemHdrTxt);
	WriteCode2( SDITVSystemArrTxt[SDIConfig.System]);

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusAudioUpdate																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display actual AES-EBU status, ie. selected by user			*/
/*	Remarks:		STATUS DISPLAY function													*/
/*					Cursor position on line 1 and 2 is updated						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusAudioUpdate() {

	WriteCodeLN1(0, AES_EBUHdrTxt);
	WriteCode1( AES_EBUSignalArrTxt[AES_EBUConfig.Signal]);
	ClearRestLN1();

	WriteCodeLN2( 0, LevelHdrTxt);

	if ( AES_EBUConfig.Signal == Audio48kHz) {
		WriteCode2( HyphenTxt);

		ClearToPosLN2( 17);

		WriteCode2( TimingHdrTxt);
		WriteCode2( HyphenTxt);
	}
	else {
		WriteCode2( AES_EBULevelArrTxt[AES_EBUConfig.Level]);

		ClearToPosLN2( 17);

		WriteCode2( TimingHdrTxt);
		WriteCode2( AES_EBUTimingArrTxt[AES_EBUConfig.Timing]);
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/* StatusErrorUpdate																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970827				     													*/
/*																									*/
/*	Function:	If any active errors, display error from top of errorqueue	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusErrorUpdate() {

	WriteCodeLN1(0, Status_ErrorStatusTxt);
	ClearRestLN1();

	if ( ErrorStatus) 								// Write error in display
		WriteErrorLN2( 0, ErrorQueue[ErrorQueuePtr]);
	else
		WriteCodeLN2( 0, NoWarningsTxt);

	ClearRestLN2();

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display BASE MENU highlighting the LAST selected menu			*/
/*	Remarks:		STATUS DISPLAY function													*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, StatusNdx, MenuModeOn												*/
/***************************************************************************/
void StatusDown(){

	StatusNdx = Ptr->ItemNumber;			// Save active display item

	MenuModeOn = TRUE;						// Set menu-mode

	Ptr = &base_menu[BaseNdx];				// Restore Base-Menu pointer
}

/***************************************************************************/
/*	UpToStatus																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Select the STATUS DISPLAY and display the last active 		*/
/*					status display																*/
/*	Remarks:		STATUS DISPLAY function													*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, MenuModeOn															*/
/***************************************************************************/
void UpToStatus(){

	MenuModeOn = FALSE;						// Clear menu-mode

	Ptr = &status_menu[StatusNdx];		// Restore Display-Status pointer
}

/***************************************************************************/
/*	BaseMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display the items in the BASE-MENU									*/
/*	Remarks:		BASE MENU function														*/
/*					Cursor position on line 1 and line 2 are updated				*/
/*	Returns:		Nothing																		*/
/*	Updates:		BaseNdx, PrevPtr															*/
/*					FuncSave, FuncTriggers and FuncState								*/
/***************************************************************************/
void BaseMenuShow(){

	register int i, j;
	int number, start;
	code struct MENU_ITEM* TmpPtr;

	WriteCodeLN1( 0, PT5210HeadlineTxt);
	ClearRestLN1();

	BaseNdx = Ptr->ItemNumber;							// Save Base Menu index

	WriteCodeLN2( 0, MenuHdrTxt);						// Write header for menu line

	TmpPtr = Ptr;

	i = 0;

	if ( SDIBlkUnit[SB34Item].Present || SDIBlkUnit[SB56Item].Present ||\
		  SDIBlkUnit[SB78Item].Present )
		i |= SDIBlkMask;

	if ( AES_EBUUnit.Present)
		i |= AES_EBUSignalMask;

	if ( AnalogSignalUnit.Present)
		i |= AnalogSignalMask;

	if ( SDISignalUnit.Present)
		i |= SDISignalMask;

	switch ( i) {				//Base menu screens according to present units
		case 0:
			if ( BaseNdx < PresetItem) {
				Ptr = &base_menu[AnlBlackItem];
				start = AnlBlackItem;
				number = 2;
			}
			else {
				Ptr = &base_menu[PresetItem];
				start = PresetItem;
				number = 2 + TestModeEnable;
			}
			break;

		case 1:
		case 2:
		case 4:
		case 8:
			if ( BaseNdx < GenlockItem) {
				Ptr = &base_menu[AnlBlackItem];
				start = AnlBlackItem;
				number = 2;
			}
			else {
				if ( BaseNdx == TestItem) {
					Ptr = &base_menu[TestItem];
					start = TestItem;
					number = 1;
				}
				else {
					Ptr = &base_menu[GenlockItem];
					start = GenlockItem;
					number = 3;
				}
			}
			break;

		case 3:
		case 10:
			if ( BaseNdx < SDISignalItem) {
				Ptr = &base_menu[AnlBlackItem];
				start = AnlBlackItem;
				number = 2;
			}
			else {
				if ( BaseNdx < PresetItem) {
					Ptr = &base_menu[SDISignalItem];
					start = SDISignalItem;
					number = 2;
				}
				else {
					Ptr = &base_menu[PresetItem];
					start = PresetItem;
					number = 2 + TestModeEnable;
				}
			}
			break;

		case 5:
		case 6:
		case 12:
			if ( BaseNdx < AES_EBUItem) {
				Ptr = &base_menu[AnlBlackItem];
				start = AnlBlackItem;
				number = 2;
			}
			else {
				if ( BaseNdx < ConfigItem) {
					Ptr = &base_menu[AES_EBUItem];
					start = AES_EBUItem;
					number = 3;
				}
				else {
					Ptr = &base_menu[ConfigItem];
					start = ConfigItem;
					number = 1 + TestModeEnable;
				}
			}
			break;

		case 7:
		case 14:
			if ( BaseNdx < SDISignalItem) {
				Ptr = &base_menu[AnlBlackItem];
				start = AnlBlackItem;
				number = 2;
			}
			else {
				if ( BaseNdx < GenlockItem) {
					Ptr = &base_menu[SDISignalItem];
					start = SDISignalItem;
					number = 2;
				}
				else {
					if ( BaseNdx < TestItem) {
						Ptr = &base_menu[GenlockItem];
						start = GenlockItem;
						number = 3;
					}
					else {
						Ptr = &base_menu[TestItem];
						start = TestItem;
						number = 1;
					}
				}
			}
			break;

		case 9:
			if ( BaseNdx < AnalogSignalItem) {
				Ptr = &base_menu[AnlBlackItem];
				start = AnlBlackItem;
				number = 2;
			}
			else {
				if ( BaseNdx < PresetItem) {
					Ptr = &base_menu[AnalogSignalItem];
					start = AnalogSignalItem;
					number = 2;
				}
				else {
					Ptr = &base_menu[PresetItem];
					start = PresetItem;
					number = 2 + TestModeEnable;
				}
			}
			break;

		case 11:
		case 13:
			if ( BaseNdx < AnalogSignalItem) {
				Ptr = &base_menu[AnlBlackItem];
				start = AnlBlackItem;
				number = 2;
			}
			else {
				if ( BaseNdx < GenlockItem) {
					Ptr = &base_menu[AnalogSignalItem];
					start = AnalogSignalItem;
					number = 2;
				}
				else {
					if ( BaseNdx < TestItem) {
						Ptr = &base_menu[GenlockItem];
						start = GenlockItem;
						number = 3;
					}
					else {
						Ptr = &base_menu[TestItem];
						start = TestItem;
						number = 1;
					}
				}
			}
			break;

		case 15:
			if ( BaseNdx < AnalogSignalItem) {
				Ptr = &base_menu[AnlBlackItem];
				start = AnlBlackItem;
				number = 2;
			}
			else {
				if ( BaseNdx < AES_EBUItem) {
					Ptr = &base_menu[AnalogSignalItem];
					start = AnalogSignalItem;
					number = 2;
				}
				else {
					if ( BaseNdx < ConfigItem) {
						Ptr = &base_menu[AES_EBUItem];
						start = AES_EBUItem;
						number = 3;
					}
					else {
						Ptr = &base_menu[ConfigItem];
						start = ConfigItem;
						number = 1 + TestModeEnable;
					}
				}
			}
			break;
	}

	for ( j = 8, i = start; i < start+number; i++) {
			WriteCode2( Ptr->ItemTxt);
			WriteTxt2( "   ");

			if ( Ptr->ItemNumber < BaseNdx )
				j += ( 3+ Cstrlen( Ptr->ItemTxt));

			BaseItemRight();
	}

	ClearRestLN2();					// Clear rest of line 2

	Ptr = TmpPtr;

	i = Ptr->ItemConfig;				// Get info on line 1&2 icons/dots

											// If PANEL LOCK enabled..
											//  include a pad-lock and remove the
											//  down arrow except for CONFIG-MENU
	if (( Flags.PanelLockOn || Flags.NormalLockOn ) && ( BaseNdx != ConfigItem)) {
		if ( Flags.PanelLockOn)
			i = ( i | P_) & ~D_;
		else {
			switch ( BaseNdx) {
				case AnalogSignalItem:
				case SDISignalItem:
				case AES_EBUItem:
				case PresetItem:
					break;

				default:
					i = ( i | P_) & ~D_;
					break;
			}
		}
	}
	WriteArrowsField( i);							// Write icons/dots on line 1&2
															// Mark selected item
	WriteItemArrows( j, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	BaseDown																			  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the BASE MENU						*/
/*	Remarks:		BASE MENU function														*/
/*	Returns:		--																				*/
/*	Updates:		Ptr	  TBD TBD TBD														*/
/***************************************************************************/
void BaseDown(){

	if ( Flags.PanelLockOn)						// If PANEL LOCK enabled..
		if ( BaseNdx != ConfigItem)			//  only CONFIG MENU is alllowed
			return;

	switch ( BaseNdx) {
		case AnlBlackItem:						// If ANALOG-BLACK MENU selected..
			if ( !Flags.NormalLockOn)			//  if NORMAL LOCK disabled..
				Ptr = &anlblk_menu[AnlBlkNdx];//   update menu-pointer to last
			break;									//   active item

		case SDIBlackItem:						// If SDI-BLACK&CB MENU selected..
			if ( !Flags.NormalLockOn)			//  if NORMAL LOCK disabled..
				Ptr = &SDIblk_menu[SDIBlkNdx];//   update menu-pointer to last
			break;									//   active item

		case AnalogSignalItem:					// If ANALOG-SIGNAL MENU selected..
			Ptr = &anlsig_menu[AnlSigNdx];	//  update menu-pointer to last
			break;									//  active item

		case SDISignalItem:						// If SDI-SIGNAL MENU selected..
			Ptr = &SDIsig_menu[SDISigNdx];	//  update menu-pointer to last
			break;									//  active item

		case AES_EBUItem:							// If AES-EBU MENU selected..
			Ptr = &AES_EBU_menu[AES_EBUNdx];	//  update menu-pointer to last
			break;									//  active item

		case GenlockItem:							// If GENLOCK MENU selected..
			if ( !Flags.NormalLockOn) {		//  if NORMAL LOCK disabled..

				PrevPtr = Ptr;						//   Store old menu-pointer
				Ptr = &genlock_menu;				//   Point to genlock-menu

				UcharVal = ActiveGenlockNo;	//   Restore selected genlock input
														//   Restore selected genlock system
				UcharVal1 = GenlockConfig[UcharVal].System;

				FuncState = 1;						//   Prepare for item state-machine
				FuncTriggers = 0;
				FuncSave = FuncEscape = FALSE;

				GenlockInputShow(); 				//   Display the menu
			}
			break;

		case PresetItem:							// If PRESET MENU selected..
			Ptr = &preset_menu[PresetNdx];	//  update menu-pointer to last
			break;									//  active item

		case ConfigItem:							// If CONFIG MENU selected..
			Ptr = &config_menu[ConfigNdx];	//  update menu-pointer to last
			break;									//  active item

		case TestItem:								// If TEST MENU selected..
			break;									//  TBD
	}
}

/***************************************************************************/
/*	UpToBase																			  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Select the BASE MENU and restore last selected item			*/
/*	Remarks:		BASE MENU function														*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToBase(){

	Ptr = &base_menu[BaseNdx];					// Restore Base menu item
}

/***************************************************************************/
/*	AnlBlkMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display the items in the ANALOG-BLACK MENU						*/
/*	Remarks:		ANALOG-BLACK MENU function												*/
/*					Cursor position on line 1 and line 2 are updated				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AnlBlkMenuShow(){

	register int i, j, last, NoOfBBs;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( Configure_Txt);					// Write menu headline
	ClearRestLN1();									// Clear rest of line 1

	AnlBlkNdx = Ptr->ItemNumber;					// Update BB unit index, ie. no.

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for sub-menu line
															// Calculate no. of BB units
	for ( NoOfBBs=0, i=BB1Item; i < BB8Item+1; i++)
		if ( AnlBlkUnit[i].Present)
			NoOfBBs++;

	if ( NoOfBBs > 6) {									// If 6 or more BB units..
		i = last = BB7Item;								//  two screens are required
		( AnlBlkNdx > BB6Item) ? ( last = BB8Item+1) : ( i = BB1Item);
	}
	else {													// ..otherwise only 1 screen
		i = 0;
		last = BB8Item+1;
	}

	for ( j=0;  i < last; i++) {						// Display active screen
		if ( AnlBlkUnit[i].Present) {

			if (( i == BB2Item) && ( Connectors[BB34Conn].HWType == PT8604)) {
				WriteCode2( BBMulTxt);					// Write item text
				WriteTxt2( "  ");                   // Jump to next position

				if ( i < AnlBlkNdx)						// Calculate item marker
					j += ( Cstrlen( BBMulTxt) + 2);
			}
			else {
				WriteCode2( anlblk_menu[i].ItemTxt);// Write item text
				WriteTxt2( "  ");                   // Jump to next position

				if ( i < AnlBlkNdx)						   // Calculate item marker
					j += ( Cstrlen( anlblk_menu[i].ItemTxt) + 2);
			}
		}
	}
	ClearRestLN2();									// Clear reset of line 2

	i = Ptr->ItemConfig;								// Get line1&2 icons/dots info

	if ( NoOfBBs > 6 )								// If 6 or more BB units..
		i += MORE_;										//  add 'more' dots to line 2

	WriteArrowsField( i);							// Write icons/dots on line 1&2

	if (( AnlBlkNdx == BB2Item) && ( Connectors[BB34Conn].HWType == PT8604))
		WriteItemArrows( j+8, Cstrlen( BBMulTxt));		// Mark selected item
	else
		WriteItemArrows( j+8, Cstrlen( Ptr->ItemTxt));	// Mark selected item
}

/***************************************************************************/
/*	AnlBlkDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the ANALOG-BLACK MENU			*/
/*	Remarks:		ANALOG-BLACK MENU function												*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AnlBlkDown(){

	Ptr = &BB_menu[BBNdx[AnlBlkNdx]];		// Restore unit BBn's last item
}

/***************************************************************************/
/* UpToAnlBlk																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Select the ANALOG-BLACK MENU and restore the last selected	*/
/*					item																		   */
/*	Remarks:		ANALOG-BLACK MENU function												*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToAnlBlk(){

	Ptr = &anlblk_menu[AnlBlkNdx];			// Restore selected BB unit
}

/***************************************************************************/
/*	BBMenuShow																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display the items in the BLACK-BURST SUBMENU						*/
/*	Remarks:		BLACK-BURST SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void BBMenuShow(){

	register int i;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');

	if (( AnlBlkNdx == BB2Item) && ( Connectors[BB34Conn].HWType == PT8604))
		WriteCode1( BBMulTxt);
	else
		WriteCode1( anlblk_menu[AnlBlkNdx].ItemTxt);

	WriteCode1( Select_Txt);						// Write sub-menu headline
	ClearRestLN1();

	BBNdx[AnlBlkNdx] = Ptr->ItemNumber;			// Update Black Burst menu index

	WriteCodeLN2( 0, SecondmnuHdrTxt);			// Write line 2 header

	for ( i = BBSystemItem; i <= BBCopyItem; i++) {
		ClearToPosLN2( BB_menu[i].ItemPos);		// Find item column position
		WriteCode2( BB_menu[i].ItemTxt);			// Write item texts
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	BBDown																			  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 950301	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the BLACK BURST SUBMENU		*/
/*	Remarks:		BLACK BURST SUBMENU function											*/
/*					Cursor position on line 1 is updated 								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, PrevPtr, FloatVal, IntVal, UcharVal,							*/
/*					stepNdx, stepVal, samples												*/
/*					FuncState, FuncTriggers, FuncSave, FuncEscape					*/
/***************************************************************************/
void BBDown(){

	WriteCodeLN1( 0, SecondmnuHdrTxt);
	WriteCode1( SlashDotHdrTxt);

	if (( AnlBlkNdx == BB2Item) && ( Connectors[BB34Conn].HWType == PT8604))
		WriteCode1( BBMulTxt);
	else
		WriteCode1( anlblk_menu[AnlBlkNdx].ItemTxt);

	WriteChar1( '/');
	WriteCode1( BB_menu[BBNdx[AnlBlkNdx]].ItemTxt);

	switch ( BBNdx[AnlBlkNdx]) {
		case BBSystemItem:
			WriteCode1( Select_Txt);

			UcharVal = AnlBlkConfig[AnlBlkNdx].System;	// Get BBn system
			break;

		case BBTimeItem:
			WriteCode1( Edit_DelayTxt);

			UcharVal = AnlBlkConfig[AnlBlkNdx].System;	// Get BBn system
			samples = AnlBlkConfig[AnlBlkNdx].Delay;		// Get BBn delay

			stepNdx = stepNdxAnalogMin;
			stepVal = stepsizeAnalogTable[stepNdx];		// Get step size
			break;

		case BBScHPhaseItem:
			WriteCode1( Edit_Txt);

			IntVal = AnlBlkConfig[AnlBlkNdx].ScHPhase;	// Get BBn ScH-Phase
			break;

		case BBCopyItem:
			WriteCode1( Select_Txt);

			UcharVal = AnlBlkNdx;
			break;
	}

	ClearRestLN1();								// Clear rest of line 1

	PrevPtr = Ptr;									// Store old menu-pointer
	Ptr = &BB_submenu[BBNdx[AnlBlkNdx]];	// Point to selected item

	FuncState = 1;									// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;							// Set select-mode
}

/***************************************************************************/
/*	BBSystemSelect																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960107				     													*/
/*																									*/
/*	Function:	Sets the TV system in a black burst generator.					*/
/*	Remarks:		BLACK-BURST SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, CodePtr, UcharVal, FuncSave, FuncEscape, FuncState,		*/
/*				  	FunState, FuncTriggers, SelectModeOn, ActivepresetNo			*/
/***************************************************************************/
void BBSystemSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If system has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new BB system
			AnlBlkConfig[AnlBlkNdx].System = UcharVal;

			if ( ErrorCode = TrxAnlBlkSystem( AnlBlkNdx, UcharVal)) {
				GotoErrorStatus( 256*AnlBlkNdx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( UcharVal-- == PAL)
				UcharVal = NTSC;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal++ == NTSC)
				UcharVal = PAL;
		}
															// If actual system..
		if ( UcharVal == AnlBlkConfig[AnlBlkNdx].System)
			FuncState = 1;								//  remove SAVE option
	}

	CodePtr = AnaTVSystemArrTxt[UcharVal];		// Get text for selection

	WriteCodeLN2( 0, SystemHdrTxt);				// Write header on line 2
	WriteCode2( CodePtr);							// Write selected text on line 2

	ClearRestLN2();                       	 	// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	BBTimeSelect																 	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Handle the display containing the BLACK-BURST-SUBMENU			*/
/*	Remarks:		BLACK-BURST SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, UcharVal, CharVal, FuncSave, FunState and FuncTriggers	*/
/***************************************************************************/
void BBTimeSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {										// If delay has changed..
			ActivePresetNo = 0;								// Clear possible reset

			AnlBlkConfig[AnlBlkNdx].Delay = samples;	// Save new BBn delay
		}
																	// Transmit delay to BBn
		TrxAnlBlkDelay( AnlBlkNdx, AnlBlkConfig[AnlBlkNdx].Delay);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	AnalogTimeAdjust( 0, UcharVal, AnlBlkConfig[AnlBlkNdx].Delay);

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	BBScHPhaseSelect															 	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960107				     													*/
/*																									*/
/*	Function:	Setting the ScH-phase value in a black burst generator.		*/
/*	Remarks:		BLACK-BURST SUBMENU function											*/
/*					Instant update of ScH-Phase. The ScH-Phase is not updated	*/
/*					if the menu is left without altering the value.					*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, FloatVal, IntVal, TextBuffer, FuncSave,	FuncState		*/
/*					FuncTriggers, SelectModeOn, ActivePresetNo						*/
/***************************************************************************/
void BBScHPhaseSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) { 								// If ScHPhase has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new BB ScH-Phase
			AnlBlkConfig[AnlBlkNdx].ScHPhase = IntVal;
		}
		else {
			if ( IntVal != AnlBlkConfig[AnlBlkNdx].ScHPhase)
				if ( ErrorCode = TrxAnlBlkScHPhase( AnlBlkNdx, AnlBlkConfig[AnlBlkNdx].ScHPhase)) {
					GotoErrorStatus( 256*AnlBlkNdx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( ++IntVal > ScHPhaseHigh)			// If max. ScH-Phase reached..
				IntVal = ScHPhaseLow;				//  .. roll-over to min. value
		}
		else {											// ..button pressed is DOWN
			if ( --IntVal < ScHPhaseLow)			// If min. ScH-Phase reached..
				IntVal = ScHPhaseHigh;	 			//  .. roll-over to max. value
		}
															// If actual ScH-Phase..
		if ( IntVal == AnlBlkConfig[AnlBlkNdx].ScHPhase)
			FuncState = 1;								//  remove SAVE option

															// Transmit ScH-Phase
		if ( ErrorCode = TrxAnlBlkScHPhase( AnlBlkNdx, IntVal)) {
			GotoErrorStatus( 256*AnlBlkNdx + ErrorCode);
			return;
		}
	}

	WriteCodeLN2( 0, ScHPhaseHdrTxt);			// Write line 2 header
															// Convert phase to ascii
	sprintf( TextBuffer, "%+4d", IntVal);

	Cstrcat( TextBuffer, degTxt);					// Add prefix deg

	WriteTxt2( TextBuffer);							// Write phase value

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 11, strlen( TextBuffer));	// Mark selected item

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	BBCopySelect																 	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Handle the display containing the BLACK BURST SUBMENU copy	*/
/*					function 																	*/
/*	Remarks:		BLACK BURST SUBMENU function											*/
/*					Cursor position on line 2 are updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, CodePtr, UcharVal, FuncSave, FunState	 					*/
/*					FuncTriggers, SelectModeOn, ActivePresetNo						*/
/***************************************************************************/
void BBCopySelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If OK to copy..
			ActivePresetNo = 0;						// Clear a possible preset

			memcpy( &AnlBlkConfig[AnlBlkNdx], &AnlBlkConfig[UcharVal], sizeof( AnlBlkConfig[0]));

			if ( ErrorCode = TrxAnlBlkSystem( AnlBlkNdx, AnlBlkConfig[AnlBlkNdx].System)) {
				GotoErrorStatus( 256*AnlBlkNdx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			do {
				if ( UcharVal-- == BB1Item)
					UcharVal = BB8Item;
			} while ( !AnlBlkUnit[UcharVal].Present);
		}
		else												// ..button pressed is DOWN
			do {
				if ( UcharVal++ == BB8Item)
					UcharVal = BB1Item;
			} while ( !AnlBlkUnit[UcharVal].Present);
	}

	CodePtr = BBArrTxt[UcharVal];					// Get text for selection

	WriteCodeLN2( 0, BlackCopyHdrTxt);			// Write line 2 header
	WriteCode2( CodePtr);							// Write text on line 2

	ClearRestLN2();

	WriteType3_OK_ESC( FuncState);				// Write the OK ESC text

	if ( FuncState == 1)
		WriteItemArrows( 19, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	SDIBlkMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960619	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display the items in the DIGITAL-BLACK MENU						*/
/*	Remarks:		DIGITAL-BLACK MENU function											*/
/*					Cursor position on line 1 and line 2 are updated				*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SDIBlkMenuShow(){

	register int i, j;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( Configure_Txt);					// Write sub-menu headline

	ClearRestLN1();

	SDIBlkNdx = Ptr->ItemNumber;					// Update SB unit index, ie. no.

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for menu line

	for ( j = 0, i = SB34Item; i <= SB78Item; i++) {

		if ( SDIBlkUnit[i].Present) {
			WriteCode2( SDIblk_menu[i].ItemTxt);	// Write item text
			WriteTxt2( "  ");								// Set next column position

			if ( i < SDIBlkNdx)
				j += ( 2 + Cstrlen( SDIblk_menu[i].ItemTxt));
		}
	}
	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2

	WriteItemArrows( j+8, Cstrlen( Ptr->ItemTxt));	// Mark selected item/
}

/***************************************************************************/
/*	SDIBlkDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960622	    										*/
/* Revised:		960912				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the DIGITAL-BLACK MENU			*/
/*	Remarks:		DIGITAL-BLACK MENU function											*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void SDIBlkDown(){

	Ptr = &SB_menu[SBNdx[SDIBlkNdx]];
}

/***************************************************************************/
/*	UpToSDIBlk																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960619	    										*/
/* Revised:		960912				     													*/
/*																									*/
/*	Function:	Select the DIGITAL-BLACK MENU and restore the last selected	*/
/*					item																		   */
/*	Remarks:		DIGITAL-BLACK MENU function											*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToSDIBlk(){

	Ptr = &SDIblk_menu[SDIBlkNdx];
}

/***************************************************************************/
/*	SBMenuShow																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960912				     													*/
/*																									*/
/*	Function:	Handle the display containing the SERIAL-BLACK SUBMENU		*/
/*	Remarks:		BLACK MENU function														*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SBMenuShow(){

	register int i, j;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( SDIblk_menu[SDIBlkNdx].ItemTxt);
	WriteCode1( Select_Txt);						// Write sub-menu headline
	ClearRestLN1();

	SBNdx[SDIBlkNdx] = Ptr->ItemNumber;			// Update serial black index

	WriteCodeLN2( 0, SecondmnuHdrTxt);

	i = j = SBAudioItem;								// Find window

	if ( SBNdx[SDIBlkNdx] < SBAudioItem)		// If item < EMB.Audio..
		i = SBPatternItem;							//  Pattern, System & EDH window
	else                                      // else
		j = SBCopyItem+1;								//  EMB.Audio & Time window

	for ( ; i < j; i++) {
		ClearToPosLN2( SB_menu[i].ItemPos);		// Write item position
		WriteCode2( SB_menu[i].ItemTxt);			// Write item text
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	SBDown																			  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960912				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the SERIAL BLACK SUBMENU		*/
/*	Remarks:		SERIAL BLACK MENU function												*/
/*					Cursor position on line 1 is					 						*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, PrevPtr, FloatVal, IntVal, UcharVal,							*/
/*					stepNdx, stepVal, samples												*/
/*					FuncState, FuncTriggers, FuncSave, FuncEscape					*/
/***************************************************************************/
void SBDown(){

	WriteCodeLN1( 0, SecondmnuHdrTxt);
	WriteCode1( SlashDotHdrTxt);
	WriteCode1( SDIblk_menu[SDIBlkNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( SB_menu[SBNdx[SDIBlkNdx]].ItemTxt);

	switch ( SBNdx[SDIBlkNdx]) {
		case SBPatternItem:
			WriteCode1( Select_Txt);							// Write ', select'

			UcharVal = SDIBlkConfig[SDIBlkNdx].Pattern;	// Get SBn pattern
			UcharVal1 = SDIBlkConfig[SDIBlkNdx].System;	// Get SBn system
			break;

		case SBSystemItem:
			WriteCode1( Select_Txt);							// Write ', select'

			UcharVal = SDIBlkConfig[SDIBlkNdx].System;	// Get SBn system
			break;

		case SBEDHItem:
			WriteCode1( Select_Txt);							// Write ', select'

			UcharVal = SDIBlkConfig[SDIBlkNdx].EDH;		// Get SBn EDH
			break;

		case SBAudioItem:
			WriteCode1( Select_Txt);   						// Write ', select'

			UcharVal = SDIBlkConfig[SDIBlkNdx].EMBAudio;	// Get SBn audio
			break;

		case SBTimeItem:
			WriteCode1( Edit_DelayTxt);						// Write ', edit delay'

			UcharVal = SDIBlkConfig[SDIBlkNdx].System;	// Get SBn system
			samples = SDIBlkConfig[SDIBlkNdx].Delay;		// Get SBn delay

			stepNdx = stepNdxSDIMin;
			stepVal = stepsizeSDITable[stepNdx];			// Get step size
			break;

		case SBCopyItem:
			WriteCode1( Select_Txt);				// Write ', select'

			UcharVal = SDIBlkNdx;
			break;
	}

	ClearRestLN1();									// Clear rest of line 1

	PrevPtr = Ptr;                		  		// Store old menu-pointer
	Ptr = &SB_submenu[SBNdx[SDIBlkNdx]];		// Point to selected item

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Set select-mode
}

/***************************************************************************/
/*	SBPatternSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970106				     													*/
/*																									*/
/*	Function:	Select the test-pattern in the SERIAL-BLACK generator			*/
/*	Remarks:		SERIAL-BLACK MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SBPatternSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If pattern has changed
			ActivePresetNo = 0;						// Clear a possible preset
															// Save new selection
			SDIBlkConfig[SDIBlkNdx].Pattern = UcharVal;
		}
		else {											// ..restore SB pattern
			if ( UcharVal != SDIBlkConfig[SDIBlkNdx].Pattern)
				if ( ErrorCode = TrxSDIBlkPattern( SDIBlkNdx, SDIBlkConfig[SDIBlkNdx].Pattern)) {
					GotoErrorStatus( 256*SDIBlkNdx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal1 == SDI625) {				// If TV system is 625..
			if ( FuncTriggers & 0x01) {			// If button pressed is UP..
				if ( UcharVal-- == SBBlack)
					UcharVal = SBCBar100;
			}
			else {										// ..else buttons is DOWN
				if ( UcharVal++ == SBCBar100)
					UcharVal = SBBlack;
			}
		}
		else {											// ..else TV system is SDI525
			if ( UcharVal == SBSMPTECBar)
				UcharVal = SBBlack;
			else
				UcharVal = SBSMPTECBar;
		}
															// If actual pattern..
		if ( UcharVal == SDIBlkConfig[SDIBlkNdx].Pattern)
			FuncState = 1;								//  remove SAVE option
															// Transmit pattern selection
		if ( ErrorCode = TrxSDIBlkPattern( SDIBlkNdx, UcharVal)) {
			GotoErrorStatus( 256*SDIBlkNdx + ErrorCode);
			return;
		}
	}

	CodePtr = SBPatternArrTxt[UcharVal];		// Get text for selected pattern

	WriteCodeLN2( 0, PatternHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);							// Write the selected pattern

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC texct

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 9, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	SBSystemSelect																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Setting the TV system in the serial black generator			*/
/*	Remarks:		SERIAL-BLACK SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, CodePtr, UcharVal, FuncSave, FuncEscape, FuncState,		*/
/*				  	FunState, FuncTriggers													*/
/***************************************************************************/
void SBSystemSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {										// If system has changed..
			ActivePresetNo = 0;								// Clear possible preset
																	// Save new SB system
			SDIBlkConfig[SDIBlkNdx].System = UcharVal;

			if ( ErrorCode = TrxSDIBlkSystem( SDIBlkNdx, UcharVal)) {
				GotoErrorStatus( 256*SDIBlkNdx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( UcharVal == SDI625)              //  toggle TV system
			UcharVal = SDI525;
		else
			UcharVal = SDI625;
															// If actual system..
		if ( UcharVal == SDIBlkConfig[SDIBlkNdx].System)
			FuncState = 1;							//  remove SAVE option
	}

	CodePtr = SDITVSystemArrTxt[UcharVal];		// Get text for selection

	WriteCodeLN2( 0, SystemHdrTxt);				// Write header on line 2
	WriteCode2( CodePtr);							// Write text on line 2

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	SBEDHSelect																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Swith EDH on/off in the serial black generator.					*/
/*	Remarks:		SERIAL-BLACK SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, CodePtr, UcharVal, FuncSave, FuncEscape, FunState		*/
/*					and FuncTriggers															*/
/***************************************************************************/
void SBEDHSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If EDH has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new SB EDH
			SDIBlkConfig[SDIBlkNdx].EDH = UcharVal;

			if ( ErrorCode = TrxSDIBlkEDH( SDIBlkNdx, UcharVal)) {
				GotoErrorStatus( 256*SDIBlkNdx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( UcharVal == ON)						//  toggle EDH alarm on/off
			UcharVal = OFF;
		else
			UcharVal = ON;
															// If actual EDH..
		if ( UcharVal == SDIBlkConfig[SDIBlkNdx].EDH)
			FuncState = 1;							//  remove SAVE option
	}

	CodePtr = OffOnArrTxt[UcharVal];					// Get text for EDH selection

	WriteCodeLN2( 0, SBEDHHdrTxt);					// Write line 2 header
	WriteCode2( CodePtr);                  		// Write text for selection

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 15, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset button selectors
}

/***************************************************************************/
/*	SBAudioSelect																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Select the embedded audio signal in the serial black			*/
/*					generator.																	*/
/*	Remarks:		SERIAL-BLACK SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SBAudioSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {									// If EMB Audio has changed..
			ActivePresetNo = 0;							// Clear possible preset
																// Save new SB EMB Audio
			SDIBlkConfig[SDIBlkNdx].EMBAudio = UcharVal;

			if ( ErrorCode = TrxSDIBlkAudio( SDIBlkNdx, UcharVal)) {
				GotoErrorStatus( 256*SDIBlkNdx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal == EMBAudioOff)				//  toggle between Off & Silence
			UcharVal = EMBAudioSilence;
		else
			UcharVal = EMBAudioOff;
															// If actual EMB Audio..
		if ( UcharVal == SDIBlkConfig[SDIBlkNdx].EMBAudio)
			FuncState = 1;								//  remove SAVE option
	}

	CodePtr = EMBAudioArrTxt[UcharVal];			// Get text for selection audio

	WriteCodeLN2( 0, SBAudioHdrTxt);					// Write line 2 header
	WriteCode2( CodePtr);								// Write selected text

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 16, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset button selectors
}

/***************************************************************************/
/*	SBTimeSelect																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960912				     													*/
/*																									*/
/*	Function:	Handle the display containing the SERIAL-BLACK copy function*/
/*	Remarks:		SERIAL-BLACK SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, CharVal, FuncSave, FunState and FuncTriggers	*/
/***************************************************************************/
void SBTimeSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {
			ActivePresetNo = 0;					// Delay has changed, clear preset

			SDIBlkConfig[SDIBlkNdx].Delay = samples;		// Save new SB delay
		}
																		// MUST be .Delay
		TrxSDIBlkDelay( SDIBlkNdx, SDIBlkConfig[SDIBlkNdx].Delay);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;					// Clear select-mode

		Ptr = PrevPtr;								// Set pointer to previous menu
		Ptr->DispFunct();							// Display previous menu
		return;
	}

	SDITimeAdjust( 0, UcharVal, SDIBlkConfig[SDIBlkNdx].Delay);

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	SBCopySelect																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Handle the display containing the SERIAL BLACK SUBMENU copy	*/
/*					function																		*/
/*	Remarks:		SERIAL BLACK SUBMENU function											*/
/*					Cursor position on line 1 and 2 are updated						*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, CodePtr, UcharVal, FuncSave, FunState, FuncTriggers		*/
/***************************************************************************/
void SBCopySelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If OK to copy..
			ActivePresetNo = 0;						// Clear possible preset

			memcpy( &SDIBlkConfig[SDIBlkNdx], &SDIBlkConfig[UcharVal], sizeof( SDIBlkConfig[0]));

															// Transmit system
			ErrorCode = TrxSDIBlkSystem( SDIBlkNdx, SDIBlkConfig[SDIBlkNdx].System);

			if ( ErrorCode == 0)						// Transmit EDH
				ErrorCode = TrxSDIBlkEDH( SDIBlkNdx, SDIBlkConfig[SDIBlkNdx].EDH);

			if ( ErrorCode == 0)						// Transmit EMB Audio
				TrxSDIBlkAudio( SDIBlkNdx, SDIBlkConfig[SDIBlkNdx].EMBAudio);

			if ( ErrorCode) {
				GotoErrorStatus( 256*SDIBlkNdx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {						// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {					// If button pressed is UP..
			do {
				if ( UcharVal-- == SB34Item)
					UcharVal = SB78Item;
			} while ( !SDIBlkUnit[UcharVal].Present);
		}
		else													// ..button pressed is DOWN
			do {
				if ( UcharVal++ == SB78Item)
					UcharVal = SB34Item;
			} while ( !SDIBlkUnit[UcharVal].Present);
	}

	CodePtr = SBArrTxt[UcharVal];						// Get text for selection

	WriteCodeLN2( 0, BlackCopyHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);								// Write selection on line 2

	ClearRestLN2();										// Clear rest of line 2

	WriteType3_OK_ESC( FuncState);					// Write the OK ESC text

	if ( FuncState == 1)
		WriteItemArrows( 19, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset button selectors
}

/***************************************************************************/
/*	AnalogMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960912				     													*/
/*																									*/
/*	Function:	Handle the display containing the ANALOG MENU functions		*/
/*	Remarks:		ANALOG MENU function														*/
/*					Cursor position on line 1 and 2 are updated						*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AnalogMenuShow(){

	register int i, j;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( Configure_Txt);					// Write header for line 1
	ClearRestLN1();

	AnlSigNdx = Ptr->ItemNumber;					// Update Analog menu index

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for line 2

	i = j = AnalogTimeItem;							// Find menu window for item

	if ( AnlSigNdx < AnalogTimeItem)          // If item < TIME
		i = AnalogPatternItem;						//  Pattern, Text & System window
	else                                      // else
		j = AnalogScHPhaseItem+1;              //  Time & ScH-Phase window

	for ( ; i < j; i++) {
		ClearToPosLN2( anlsig_menu[i].ItemPos);	// Write item position
		WriteCode2( anlsig_menu[i].ItemTxt);		// Write item text
	}

	ClearRestLN2();								// Clear rest of line 2

	i = Ptr->ItemConfig;							// Get info on line 1&2 icons/dots

	if ( Flags.NormalLockOn )						// If normal lock is ENABLED..
		if ( AnlSigNdx != AnalogPatternItem)	//  and if NOT in pattern menu..
			i = ( i | P_) & ~D_; 					//  include the pad-lock and
															//  remove the down-arrow.

	WriteArrowsField( i);							// Write icons/dots on line 1&2
															//  Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	AnalogDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960912				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the ANALOG-SIGNAL MENU			*/
/*	Remarks:		ANALOG-SIGNAL MENU function											*/
/*					Cursor position on line 1 and 2 are updated 						*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, PrevPtr, IntVal, UcharVal, UcharVal1,						*/
/*					UcharVal2, TextBuffer													*/
/*					stepNdx, stepVal, samples												*/
/*					FuncState, FuncTriggers, FuncSave, FuncEscape					*/
/***************************************************************************/
void AnalogDown(){

	if ( Flags.NormalLockOn)						// If normal LOCK is enabled..
		if ( AnlSigNdx != AnalogPatternItem)	//  only analog pattern is
			return;										//  allowed

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( anlsig_menu[AnlSigNdx].ItemTxt);		// Write headline

	switch ( AnlSigNdx) {
		case AnalogPatternItem:
			UcharVal = AnalogConfig.Pattern;				// Get analog pattern
			UcharVal1 = AnalogConfig.System;				// Get analog system
			break;

		case AnalogTextItem:
			WriteCode1( Edit_Txt);
																	// Get analog text
			strcpy( TextBuffer, AnalogConfig.TextInfo);

			UcharVal = 0;
			UcharVal1 = AnalogConfig.TextOn;				// Get text ON/OFF
			break;

		case AnalogSystemItem:
			WriteCode1( Select_Txt);

			UcharVal = AnalogConfig.System;				// Get analog system
			break;

		case AnalogTimeItem:
			WriteCode1( Edit_Txt);

			UcharVal = AnalogConfig.System;				// Get analog system
			samples = AnalogConfig.Delay;					// Get analog delay

			stepNdx = stepNdxAnalogMin;
			stepVal = stepsizeAnalogTable[stepNdx];	// Get step size
			break;

		case AnalogScHPhaseItem:
			IntVal = AnalogConfig.ScHPhase;		// Get analog ScH-Phase
			break;
	}

	ClearRestLN1();									// Clear rest of line 1

	PrevPtr = Ptr;										// Store old menu-pointet
	Ptr = &anlsig_submenu[AnlSigNdx];			// Point to selected item

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Set select-mode
}

/***************************************************************************/
/*	AnalogPatternSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Select the test-pattern in the analog test signal generator	*/
/*	Remarks:		ANALOG-SIGNAL MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AnalogPatternSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If pattern has changed..
			ActivePresetNo = 0;						// Clear possible preset

			AnalogConfig.Pattern = UcharVal;		// Save new analog pattern
		}
		else {											// ..restore pattern
			if ( UcharVal != AnalogConfig.Pattern)
				if ( ErrorCode = TrxAnalogPattern( AnalogConfig.Pattern)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal1 < NTSC) {					// If system is PAL..
			if ( FuncTriggers & 0x01) {			// If button pressed is UP..
				if ( UcharVal-- == EBUCBar)
					UcharVal = BlackBurst;
			}
			else {										// .. button pressed is DOWN
				if ( UcharVal++ == BlackBurst)
					UcharVal = EBUCBar;
			}
		}
		else {											// .. system is NTSC
			if ( FuncTriggers & 0x01) {			// If button pressed is UP..
				if ( UcharVal-- == SMPTECBar)
					UcharVal = BlackBurst;

				if ( UcharVal == CBar75Red)
					UcharVal = SMPTECBar;
			}
			else {										// .. button pressed is DOWN
				if ( UcharVal++ == BlackBurst)
					UcharVal = SMPTECBar;

				if ( UcharVal == EBUCBar)
					UcharVal = Window15;
			}
		}

		if ( UcharVal == AnalogConfig.Pattern)	// If actual pattern..
			FuncState = 1;								//  remove SAVE option

															// Transmit pattern selection
		if ( ErrorCode = TrxAnalogPattern( UcharVal))	{
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AnalogPatternArrTxt[UcharVal];	// Get text for selected pattern

	WriteCodeLN2( 0, PatternHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);							// Write text for selected pattern

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC texct

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 9, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	AnalogTextSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971215				     													*/
/*																									*/
/*	Function:	Insert user defined text in the analog test signal generator*/
/*	Remarks:		ANALOG-SIGNAL MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, UcharVal1, FuncSave, FunState, FuncTriggers	*/
/***************************************************************************/
void AnalogTextSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If text has changed
			ActivePresetNo = 0;						// Clear possible preset
															// Save the new text
			strcpy( AnalogConfig.TextInfo, TextBuffer);

			AnalogConfig.TextOn = UcharVal1;		// Save text-insertion info
															// Transmit Text info to unit
			if ( ErrorCode = TrxAnalogText( AnalogConfig.TextOn, AnalogConfig.TextInfo)) {
				GotoErrorStatus( ErrorCode);
				return;
			}
		}
		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Select TextBuffer position
	if ( FuncTriggers & 0xC0) {					// If LEFT/RIGHT button pressed..
		if ( FuncTriggers & 0x40) {				// If button pressed is LEFT..
			if( UcharVal-- == 0)
				UcharVal = 7;
		}
		else {											// ..button pressed is RIGHT
			if( UcharVal++ == 7)
				UcharVal = 0;
		}
	}
															// Select character for position
	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..

		TextBuffer[UcharVal] =\
				FindNextASCII(( FuncTriggers & 0x01), 1, TextBuffer[UcharVal]);

															// If actual text & mode
		if (( !strcmp( AnalogConfig.TextInfo, TextBuffer)) &&
			 ( UcharVal1 == AnalogConfig.TextOn))
			 FuncState = 8;							//  remove SAVE option
	}

	if ( FuncTriggers & 0x0C) {					// If UP/DOWN selected..
		if ( UcharVal1 == ON)						//  toggle selection in ON/OFF
			UcharVal1 = OFF;
		else
			UcharVal1 = ON;
															// If actual text & mode
		if (( !strcmp( AnalogConfig.TextInfo, TextBuffer)) &&
			 ( UcharVal1 == AnalogConfig.TextOn))
			 FuncState = 2;							//  remove SAVE option
	}

	CodePtr = OffOnArrTxt[ UcharVal1];		// Get text for the on/off selection

	WriteCodeLN2( 0, UserTextHdrTxt);		// Write line 2 header
	WriteTxt2( TextBuffer);						// Write user text

	ClearToPosLN2( 23);							// Clear line 2 to on/off position
	WriteCode2( CodePtr);						// Write the on/off text

	ClearRestLN2();								// Clear rest of line 2

	WriteType7_SAVE_ESC( FuncState);			// Write the SAVE ESC texct

	switch ( FuncState){
		case 1:
		case 4:
			WriteItemArrows( 11, strlen( TextBuffer));	// Mark selected item
			CharBlinkLN2( 0, OFF);								// Switch OFF blinking
			break;

		case 2:
		case 5:
			WriteItemArrows( 23, Cstrlen( CodePtr));		// Mark selected item
			break;

		case 8:
		case 9:
			WriteItemBrackets( 11, strlen( TextBuffer));	// Mark selected item
			CharBlinkLN2( 11+UcharVal, ON);					// Switch on blinking at
			break;													//  editing character
	}

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/*	AnalogSystemSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Setting the TV system for the analog test signal generator.	*/
/*	Remarks:		ANALOG-SIGNAL MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		CodePtr, Ptr, UcharVal, FuncSave, FuncEscape, 					*/
/*					FunState and FuncTriggers												*/
/***************************************************************************/
void AnalogSystemSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If system has changed
			ActivePresetNo = 0;  					// Clear possible preset

			AnalogConfig.System = UcharVal;		// Save the updated system

			if ( ErrorCode = TrxAnalogSystem( UcharVal)) {
				GotoErrorStatus( ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {			// If button pressed is UP..
			if ( UcharVal-- == PAL)
				UcharVal = NTSC;
		}
		else {										// ..button pressed is DOWN
			if ( UcharVal++ == NTSC)
				UcharVal = PAL;
		}

		if ( UcharVal == AnalogConfig.System)  // If actual system..
			FuncState = 1;							//  remove SAVE option
	}

	CodePtr = AnaTVSystemArrTxt[UcharVal];		// Get text for selected system

	WriteCodeLN2( 0, SystemHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr); 							// Write selected system text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	AnalogTimeSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Setting the TV system for the analog signal generator.		*/
/*	Remarks:		ANALOG-SIGNAL MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AnalogTimeSelect(){

	if ( FuncSave || FuncEscape) {

		if ( FuncSave) {                 		// If delay has changed..
			ActivePresetNo = 0;						// Clear possible preset

			AnalogConfig.Delay = samples;			// Save updated delay
		}
		TrxAnalogDelay( AnalogConfig.Delay);	// MUST be .Delay

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	AnalogTimeAdjust( 1, UcharVal, AnalogConfig.Delay);

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	AnalogScHPhaseSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Setting the ScH-phase value in the analog test signal 		*/
/*					generator.																	*/
/*	Remarks:		ANALOG-SIGNAL MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, IntVal, CharVal, TextBuffer, FuncSave, FunState and		*/
/*					FuncTriggers																*/
/***************************************************************************/
void AnalogScHPhaseSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {									// If ScHPhase has changed..
			ActivePresetNo = 0;							// Clear possible preset

			AnalogConfig.ScHPhase = IntVal;			// Save new ScH-Phase
		}
		else {												// ..restore ScH-Phase
			if ( IntVal != AnalogConfig.ScHPhase)
				if ( ErrorCode = TrxAnalogScHPhase( AnalogConfig.ScHPhase)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( ++IntVal > ScHPhaseHigh)			// If max. ScH-Phase reached..
				IntVal = ScHPhaseLow;				//  .. roll-over to min. value
		}
		else {											// ..button pressed is DOWN
			if ( --IntVal < ScHPhaseLow)			// If min. ScH-Phase reached..
				IntVal = ScHPhaseHigh;	 			//  .. roll-over to max. value
		}

		if ( IntVal == AnalogConfig.ScHPhase)	// If actual ScH-Phase..
			FuncState = 1;								//  remove SAVE option
															// Transmit ScH-Phase
		if ( ErrorCode = TrxAnalogScHPhase( IntVal)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	WriteCodeLN2( 0, ScHPhaseHdrTxt);			// Write line 2 header
															// Convert phase to ascii
	sprintf( TextBuffer, "%+4d", IntVal);

	Cstrcat( TextBuffer, degTxt);					// Add prefix deg

	WriteTxt2( TextBuffer);							// Write phase value

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 11, strlen( TextBuffer));	// Mark selected item

	FuncTriggers = 0;									// Reset button selection
}

/***************************************************************************/
/* SDIMenuShow																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Handle the display containing the SDI SIGNAL functions		*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SDIMenuShow(){

	register int i, j;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( Configure_Txt);					// Display the headline

	ClearRestLN1();									// Clear rest of line 1

	SDISigNdx = Ptr->ItemNumber;					// Update SDI-SIGNAL menu index

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for menu line

															// Find menu window for item
	if ( SDISigNdx < SDIEDHItem) {				// If item < EDH..
		i = SDIPatternItem;							//  Pattern, text & system window
		j = SDIEDHItem;
	}
	else {
		i = j = SDIAudioLevelItem;

		if ( SDISigNdx < SDIAudioLevelItem)		// If item < audio level
			i = SDIEDHItem;							//  EDH & EMB Audio Signal window
		else                                   // else
			j = SDITimeItem+1;						//  EMD Audio level & time window
	}

	for ( ; i < j; i++) {
		ClearToPosLN2( SDIsig_menu[i].ItemPos);	// Write item position
		WriteCode2( SDIsig_menu[i].ItemTxt);		// Write item text
	}

	ClearRestLN2();										// Clear rest of line 2

	i = Ptr->ItemConfig;									// Get line1&2 icons/dots info

												// If NORMAL LOCK enabled..
												//  include a pad-lock and remove the
												//  down arrow except for PATTERN-SUBMENU
	if ( Flags.NormalLockOn ) {
		if ( SDISigNdx != SDIPatternItem)
			i = ( i | P_) & ~D_;
	}

	WriteArrowsField( i);							// Write icons/dots on line 1&2
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* SDIDown																			  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		961217				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the SDI-SIGNAL MENU				*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, UcharVal2, FuncSave, FunState, FuncTriggers	*/
/***************************************************************************/
void SDIDown(){

	if ( Flags.NormalLockOn)							// If NORMAL LOCK is enabled..
		if ( SDISigNdx != SDIPatternItem)			//  only SDI pattern is
			return;											//  allowed.

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( SDIsig_menu[SDISigNdx].ItemTxt);		// Write headline

	switch ( SDISigNdx) {
		case SDIPatternItem:
			WriteCode1( Select_Txt);

			UcharVal = SDIConfig.Pattern;					// Get SDI pattern
			UcharVal1 = SDIConfig.System;					// Get SDI system

			if ( Connectors[SDISConn].HWType == PT8602)
				UcharVal2 = SDIBlack;
			else {
				if ( Connectors[SDISConn].HWVersion == 5)
					UcharVal2 = SDIPhilipswoAPAL;
				else
					UcharVal2 = SDIRed75;
			}
			break;

		case SDITextItem:
			WriteCode1( Edit_TextTxt);

			strcpy( TextBuffer, SDIConfig.TextInfo);  // Get SDI text-info

			if ( UcharVal = strlen( TextBuffer))
				UcharVal--;

			UcharVal1 = SDIConfig.TextOn;					// Get text ON/OFF

			if ( Connectors[SDISConn].HWVersion == 5)
				UcharVal2 = 15;								// Line: max 16 characters
			else
				UcharVal2 = 7;									// Line: max 8 characters
			break;

		case SDISystemItem:
			WriteCode1( Select_Txt);

			UcharVal = SDIConfig.System;					// Get SDI system
			break;

		case SDIEDHItem:
			WriteCode1( Select_Txt);

			UcharVal = SDIConfig.EDH;						// Get SDI EDH ON/OFF
			break;

		case SDIAudioSignalItem:
			UcharVal = SDIConfig.AudioSignal;			// Get SDI Audio signal
			break;

		case SDIAudioLevelItem:
			UcharVal = SDIConfig.AudioLevel;				// Get SDI audio level
			break;

		case SDITimeItem:
			WriteCode1( Edit_DelayTxt);

			UcharVal = SDIConfig.System;

			stepNdx = stepNdxSDIMin;
			stepVal = stepsizeSDITable[stepNdx];		// Get step size

			samples = SDIConfig.Delay;						// Get SDI delay
			break;
	}

	ClearRestLN1();									// Clear rest of line 1

	PrevPtr = Ptr;										// Store old menu-pointer
	Ptr = &SDIsig_submenu[SDISigNdx];			// Point to selected item

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Set select-mode
}

/***************************************************************************/
/* SDIPatternSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960107				     													*/
/*																									*/
/*	Function:	Select the test-pattern in the SDI test signal generator		*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SDIPatternSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If pattern has changed
			ActivePresetNo = 0;						// Clear possible preset

			SDIConfig.Pattern = UcharVal;			// Save updated selection
		}
		else {											// ..restore pattern
			if ( UcharVal != SDIConfig.Pattern)
				if ( ErrorCode = TrxSDIPattern( SDIConfig.Pattern)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..

		if ( UcharVal1 == SDI625) {				// If system is SDI625..
			if ( FuncTriggers & 0x01) {			// If button pressed is UP..
				if ( UcharVal-- == SDIEBUCBar)
					UcharVal = UcharVal2;
			}
			else {										// ..button pressed is DOWN
				if ( UcharVal++ == UcharVal2)
					UcharVal = SDIEBUCBar;
			}
		}
		else {											// ..system is SDI525
			if ( FuncTriggers & 0x01) {			// If button pressed is UP..
				if ( UcharVal-- == SDISMPTECBar)
					UcharVal = UcharVal2;

				if ( UcharVal == SDICBar75Red)
					UcharVal = SDISMPTECBar;
			}
			else {										// ..button pressed is DOWN
				if ( UcharVal++ == UcharVal2)
					UcharVal = SDISMPTECBar;

				if ( UcharVal == SDIEBUCBar)
					UcharVal = SDICheck;
			}
		}

		if ( UcharVal == SDIConfig.Pattern)		// If actual pattern..
			FuncState = 1;								//  remove SAVE option
															// Transmit pattern selection
		if ( ErrorCode = TrxSDIPattern( UcharVal)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = SDIPatternArrTxt[UcharVal];		// Get text for selected pattern

	WriteCodeLN2( 0, PatternHdrTxt);      		// Write line 2 header
	WriteCode2( CodePtr);							// Write the selected pattern

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 9, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* SDITextSelect																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971217				     													*/
/*																									*/
/*	Function:	Insert user defined text in the SDI test signal generator	*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, UcharVal1, FuncSave, FunState, FuncTriggers	*/
/***************************************************************************/
void SDITextSelect(){

	UC txtlen;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {     						// If text has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save the new text
			strcpy( SDIConfig.TextInfo, TextBuffer);
															// Save text-insertion info
			SDIConfig.TextOn = UcharVal1;

			if ( ErrorCode = TrxSDIText( SDIConfig.TextOn, SDIConfig.TextInfo)) {
				GotoErrorStatus( ErrorCode);
				return;
			}
		}
		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Select TextBuffer position
	if ( FuncTriggers & 0xC0) {					// If LEFT/RIGHT button pressed..
		if ( FuncTriggers & 0x40) {				// If button pressed is LEFT..
			TextBuffer[UcharVal] = 0;				// Delete character
			if ( UcharVal)
				UcharVal--;
		}
		else {											// ..button pressed is RIGHT
			if (( UcharVal < UcharVal2) && ( UcharVal < strlen( TextBuffer)))
				UcharVal++;
		}
	}
															// Select character for position
	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..

		if	( TextBuffer[UcharVal] == 0) {
			TextBuffer[UcharVal] = ' ';
			TextBuffer[UcharVal+1] = 0;
		}

		if ( Connectors[SDISConn].HWVersion == 5)
			TextBuffer[UcharVal] =\
					 FindNextASCII(( FuncTriggers & 0x01), 2, TextBuffer[UcharVal]);
		else
			TextBuffer[UcharVal] =\
					 FindNextASCII(( FuncTriggers & 0x01), 1, TextBuffer[UcharVal]);

															// If actual text & mode..
		if (( !strcmp( SDIConfig.TextInfo, TextBuffer)) &&
			 ( UcharVal1 == SDIConfig.TextOn))
			FuncState = 8;								//  remove SAVE OPTION
	}

	if ( FuncTriggers & 0x0C) {					// If UP/DOWN selected..
		if ( UcharVal1 == ON)						//  toggle selection in ON/OFF
			UcharVal1 = OFF;
		else
			UcharVal1 = ON;
															// If actual text & mode
		if (( !strcmp( SDIConfig.TextInfo, TextBuffer)) &&
			 ( UcharVal1 == SDIConfig.TextOn))
			 FuncState = 2;							//  remove SAVE option
	}

	WriteCGChar( 5, 0x0C);						// Download return icon

	CodePtr = OffOnArrTxt[UcharVal1];		// Get text for the on/off selection

	WriteCodeLN2( 0, TextHdrTxt);				// Write line 2 header

	if (( txtlen = strlen( TextBuffer)) > 16)
		WriteTxt2( &TextBuffer[txtlen-16]);	// Write partial user text
	else
		WriteTxt2( TextBuffer);					// Write whole user text

	WriteChar2( 5);								// Write return icon

	ClearToPosLN2( 25);							// Clear line 2 to on/off position
	WriteCode2( CodePtr);						// Write the on/off text

	ClearRestLN2();								// Clear rest of line 2

	WriteType7_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 4:                                      // Mark selected item
			if ( txtlen > 16)
				WriteItemArrows( 6, 17);
			else
				WriteItemArrows( 6, txtlen+1);
			CharBlinkLN2( 0, OFF);							// Switch OFF blinking
			break;

		case 2:
		case 5:                                      // Mark selected item
			WriteItemArrows( 25, Cstrlen( CodePtr));
			CharBlinkLN2( 0, OFF);							// Switch OFF blinking
			break;

		case 8:
		case 9:                          	        	// Mark selected item
			if ( txtlen > 16) {
				WriteItemBrackets( 6, 17);
				CharBlinkLN2( 22 + UcharVal - txtlen, ON);
			}
			else {
				WriteItemBrackets( 6, txtlen + 1);
				CharBlinkLN2( 6 + UcharVal, ON);			// Switch on blinking at
			}
			break;												//  editing character
	}

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/* SDISystemSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960107				     													*/
/*																									*/
/*	Function:	Setting the TV system for the SDI test signal generator.		*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SDISystemSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {							// If system has changed..
			ActivePresetNo = 0;					// Clear possible preset

			SDIConfig.System = UcharVal;		// Save updated system selection

			if ( ErrorCode = TrxSDISystem( UcharVal)) {
				GotoErrorStatus( ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( UcharVal == SDI625)				//  toggle between SDI525 & SDI625
			UcharVal = SDI525;
		else
			UcharVal = SDI625;

		if ( UcharVal == SDIConfig.System)	// If actual system..
			FuncState = 1;							//  remove SAVE option
	}

	CodePtr = SDITVSystemArrTxt[UcharVal];		// Get text for selected system

	WriteCodeLN2( 0, SystemHdrTxt);				// Write header on line 2
	WriteCode2( CodePtr);							// Write text for selected system

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* SDIEDHSelect																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960107				     													*/
/*																									*/
/*	Function:	Swithc EDH on/off in the SDI test signal generator				*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		CodePtr, Ptr, UcharVal, FuncSave, FuncEscape, FunState,		*/
/*					and FuncTriggers															*/
/***************************************************************************/
void SDIEDHSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If EDH has changed..
			ActivePresetNo = 0;						// Clear possible preset

			SDIConfig.EDH = UcharVal;				// Save new EDH selection

			if ( ErrorCode = TrxSDIEDH( UcharVal)) {
				GotoErrorStatus( ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( UcharVal == ON)						//  toggle EDH on/off
			UcharVal = OFF;
		else
			UcharVal = ON;

		if ( UcharVal == SDIConfig.EDH)		// If actual EDH..
			FuncState = 1;							//  remove SAVE option
	}

	CodePtr = OffOnArrTxt[UcharVal];				// Get ON/OFF text for selection

	WriteCodeLN2( 0, SBEDHHdrTxt);				// Write header on line 2
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 15, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset button selection
}

/***************************************************************************/
/* SDIAudioSignalSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980113				     													*/
/*																									*/
/*	Function:	Select the embedded audio signal for ths SDI test signal		*/
/*					generator.																	*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented 											*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SDIAudioSignalSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                   		// If audio signal has changed..
			ActivePresetNo = 0;						// Clear possible preset

			SDIConfig.AudioSignal = UcharVal;	// Save new audio selection
		}
		else {											// ..restore audio signal
			if ( UcharVal != SDIConfig.AudioSignal)
				if ( ErrorCode = TrxSDIAudioSignal( SDIConfig.AudioSignal)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {						// If UP/DOWN button pressed..

		if ( Connectors[SDISConn].HWVersion == 5) {
			if ( UcharVal == AudioSignalOff)
				UcharVal = AudioStereo1kHz;
			else
				UcharVal = AudioSignalOff;
		}
		else {
			if ( FuncTriggers & 0x01) {				// If button pressed is UP..
				if ( UcharVal-- == AudioSignalOff)
					UcharVal = AudioDual;
			}
			else {											// ..button pressed is DOWN
				if ( UcharVal++ == AudioDual)
					UcharVal = AudioSignalOff;
			}
		}

		if ( UcharVal == SDIConfig.AudioSignal)	// If actual audio signal..
			FuncState = 1;									//  remove SAVE option
																// Transmit audio signal
		if ( ErrorCode = TrxSDIAudioSignal( UcharVal)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AES_EBUSignalArrTxt[UcharVal];	// Get text for selected audio signal

	WriteCodeLN2( 0, SignalHdrTxt);				// Write header for line 2
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* SDIAudioLevelSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:	Set the audio level for the embedded audio in the SDI test	*/
/*					signal generator.															*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SDIAudioLevelSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                       // If audio level has changed..
			ActivePresetNo = 0;						// Clear possible preset

			SDIConfig.AudioLevel = UcharVal;		// Save new audio level
		}
		else {											// ..restore audio level
			if ( UcharVal != SDIConfig.AudioLevel)
				if ( ErrorCode = TrxSDIAudioLevel( SDIConfig.AudioLevel)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( UcharVal-- == AudioSilence)
				UcharVal = AudioLevel20dB;
		}
		else												// ..button pressed is DOWN
			if ( UcharVal++ == AudioLevel20dB)
				UcharVal = AudioSilence;

		if ( UcharVal == SDIConfig.AudioLevel)	// If actual audio level..
			FuncState = 1;								//  remove SAVE option

														  // Transmit audio level
		if ( ErrorCode = TrxSDIAudioLevel( UcharVal)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AES_EBULevelArrTxt[UcharVal];	// Get text selection audio level

	WriteCodeLN2( 0, LevelHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 7, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* SDITimeSelect																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Handle the display containing the SDI-SIGNAL functions		*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SDITimeSelect(){

	if ( FuncSave || FuncEscape) {

		if ( FuncSave) {								// If delay has changed..
			ActivePresetNo = 0;						// Clear possible preset

			SDIConfig.Delay = samples;				// Save new delay
		}
		TrxSDIDelay( SDIConfig.Delay);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	SDITimeAdjust( 1, UcharVal, SDIConfig.Delay);

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* AES_EBUMenuShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960112				     													*/
/*																									*/
/*	Function:	Handle the display containing the AES-EBU functions			*/
/*	Remarks:		AES-EBU MENU function													*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AES_EBUMenuShow(){

	register int i;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( Configure_OutputTxt);			// Display the headline

	ClearRestLN1();									// Clear rest of line 1

	AES_EBUNdx = Ptr->ItemNumber;					// Update AES-EBU Menu index

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for menu line

	for ( i = AES_EBUSignalItem; i <= AES_EBUTimingItem; i++) {
		ClearToPosLN2( AES_EBU_menu[i].ItemPos);	// Clear to item position
		WriteCode2( AES_EBU_menu[i].ItemTxt);		// Write item text
	}

	ClearRestLN2();					// Clear rest of line 2

	i = Ptr->ItemConfig;				// Get line1&2 icons/dots info

	if ( AES_EBUNdx > AES_EBUSignalItem) {
		if ( AES_EBUConfig.Signal != Audio48kHz)
			i += D_;
	}
											// If NORMAL LOCK enabled..
											//  include a pad-lock and remove the
											//  down arrow except for PATTERN-SUBMENU
	if ( Flags.NormalLockOn ) {
		if ( AES_EBUNdx != AES_EBUSignalItem)
			i = ( i | P_) & ~D_;
	}

	WriteArrowsField( i);						// Write icons/dots on line 1&2
														//  Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* AES_EBUDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960112				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the AES_EBUMENU					*/
/*	Remarks:		AES-EBU MENU function													*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AES_EBUDown(){

	if ( AES_EBUNdx > AES_EBUSignalItem) {
		if ( Flags.NormalLockOn || ( AES_EBUConfig.Signal == Audio48kHz))
			return;
	}

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( AES_EBU_menu[AES_EBUNdx].ItemTxt);	//Write headline

	switch ( AES_EBUNdx) {
		case AES_EBUSignalItem:
			WriteCode1( Select_Txt);
			UcharVal = AES_EBUConfig.Signal;
			break;

		case AES_EBULevelItem:
			WriteCode1( Select_LevelTxt);
			UcharVal = AES_EBUConfig.Level;
			break;

		case AES_EBUTimingItem:
			WriteCode1( Select_Txt);
			UcharVal = AES_EBUConfig.Timing;
			break;
	}

	ClearRestLN1();									// Clear rest of line 1

	PrevPtr = Ptr;										// Store old menu-pointer
	Ptr = &AES_EBU_submenu[AES_EBUNdx];			// Point to selected item

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Clear select-mode
}

/***************************************************************************/
/* AES_EBUSignalSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960112				     													*/
/*																									*/
/*	Function:	Select the audio signal for the serial AES-EBU generator		*/
/*	Remarks:		AES-EBU SIGNAL MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AES_EBUSignalSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If audio signal has changed..
			ActivePresetNo = 0;						// Clear possible preset

			AES_EBUConfig.Signal = UcharVal;		// Save new audio signal
		}
		else {											// ..restore audio signal
			if ( UcharVal != AES_EBUConfig.Signal)
				if ( ErrorCode = AES_EBUUnitDrv( AES_EBUConfig.Signal, AES_EBUConfig.Level,\
																AES_EBUConfig.Timing)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {						// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {					// If button pressed is UP..
			if ( UcharVal-- == AudioStereo800Hz)
				UcharVal = Audio48kHz;
		}
		else													// ..button pressed is DOWN
			if ( UcharVal++ == Audio48kHz)
				UcharVal = AudioStereo800Hz;

		if ( UcharVal == AES_EBUConfig.Signal)		// If actual signal
			FuncState = 1;									//  remove SAVE option
																// Transmit audio signal
		if ( ErrorCode = AES_EBUUnitDrv( UcharVal, AES_EBUConfig.Level, AES_EBUConfig.Timing)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AES_EBUSignalArrTxt[UcharVal];		// Get text for selection

	WriteCodeLN2( 0, SignalHdrTxt);					// Write header on line 2
	WriteCode2( CodePtr);								// Write text on line 2

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/* AES_EBULevelSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960112				     													*/
/*																									*/
/*	Function:	Set the audio level in the serial AES-EBU generator			*/
/*	Remarks:		AES-EBU SIGNAL MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, UcharVal1, FuncSave, FunState and				*/
/*					FuncTriggers																*/
/***************************************************************************/
void AES_EBULevelSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If audio level has changed..
			ActivePresetNo = 0;						// Clear possible preset

			AES_EBUConfig.Level = UcharVal;		// Save new audio level
		}
		else {											// ..restore audio level
			if ( UcharVal != AES_EBUConfig.Level)
				if ( ErrorCode = AES_EBUUnitDrv( AES_EBUConfig.Signal, AES_EBUConfig.Level,\
																	AES_EBUConfig.Timing)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {						// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {					// If button pressed is UP..
			if ( UcharVal-- == AudioSilence)
				UcharVal = AudioLevel20dB;
		}
		else													// ..button pressed is DOWN
			if ( UcharVal++ == AudioLevel20dB)
				UcharVal = AudioSilence;

		if ( UcharVal == AES_EBUConfig.Level)		// If actual level
			FuncState = 1;									//  remove SAVE option
																// Transmit audio level
		if ( ErrorCode = AES_EBUUnitDrv( AES_EBUConfig.Signal, UcharVal, AES_EBUConfig.Timing)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AES_EBULevelArrTxt[UcharVal];		// Get text for selected level

	WriteCodeLN2( 0, LevelHdrTxt);					// Write header on line 2
	WriteCode2( CodePtr);								// Write text on line 2

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 7, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/* AES_EBUTimingSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960112				     													*/
/*																									*/
/*	Function:	Set the AES-EBU timing in the serial AES-EBU generator.		*/
/*	Remarks:		AES-EBU SIGNAL MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AES_EBUTimingSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If timing has changed..
			ActivePresetNo = 0;						// Clear possible preset

			AES_EBUConfig.Timing = UcharVal;		// Store new audio timing
		}
		else {											// ..restore AES/EBU timing
			if ( UcharVal != AES_EBUConfig.Timing)
				if ( ErrorCode = AES_EBUUnitDrv( AES_EBUConfig.Signal, AES_EBUConfig.Level,\
																	AES_EBUConfig.Timing)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {						// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01){						// If button pressed is UP..
			if ( UcharVal-- == AudioPAL)
				UcharVal = AudioNTSCPhase5;
		}
		else													// ..button pressed is DOWN
			if ( UcharVal++ == AudioNTSCPhase5)
				UcharVal = AudioPAL;

		if ( UcharVal == AES_EBUConfig.Timing)		// If actual AES/EBU timing..
			FuncState = 1;									//  remove SAVE option
																// Transmit AES/EBU timing
		if ( ErrorCode = AES_EBUUnitDrv( AES_EBUConfig.Signal, AES_EBUConfig.Level, UcharVal)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AES_EBUTimingArrTxt[UcharVal];		// Get text for selected phase

	WriteCodeLN2( 0, TimingHdrTxt);					// Write header on line 2
	WriteCode2( CodePtr);								// Write text on line 2

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text
	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/* GenlockInputShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Switch to the GENLOCK INPUT MENU										*/
/*	Remarks:		GENLOCK INPUT MENU function											*/
/*					Cursor position on line 1 is updated 								*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void GenlockInputShow() {

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( Select_InputTxt);

	ClearRestLN1();
}

/***************************************************************************/
/* GenlockInputSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Setting the input of the genelock signal							*/
/*	Remarks:		GENLOCK MENU function													*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void GenlockInputSelect(){

	register int length;

	switch ( FuncState) {
		case 32:
		case 36:										 // If SYS selected..
			if ( UcharVal < Internal) {
				FuncState = 1;						 // Prepare for item state-machine
				FuncTriggers = 0;
				FuncSave = FuncEscape = FALSE;

				WriteCodeLN1( 0, SubmnuHdrTxt);
				WriteCode1( base_menu[BaseNdx].ItemTxt);
				WriteChar1( '/');
				WriteCode1( genlocksys_menu.ItemTxt);
				WriteCode1( Select_Txt);

				ClearRestLN1();

				Ptr = &genlocksys_menu;		 	// Update pointer
				Ptr->DispFunct();			 	//  and display
				return;
			}
			else
				FuncState -= 30;
			break;

		case 33:
		case 37:									 	// If TIME selected..
			if (( UcharVal < Internal) && ( UcharVal1 < MHzLock358)) {
				if ( UcharVal == SDIGenlock)
					stepVal = stepsizeSDITable[stepNdx = stepNdxSDIMin];
				else
					stepVal = stepsizeAnalogTable[stepNdx = stepNdxAnalogMin];

				samples = GenlockConfig[UcharVal].Delay;

				FuncState = 1;						// Prepare for item state-machine
				FuncTriggers = 0;
				FuncSave = FuncEscape = FALSE;

				WriteCodeLN1( 0, SubmnuHdrTxt);
				WriteCode1( base_menu[BaseNdx].ItemTxt);
				WriteChar1( '/');
				WriteCode1( genlocktime_menu.ItemTxt);
				WriteCode1( Edit_DelayTxt);

				ClearRestLN1();

				Ptr = &genlocktime_menu;		// Update pointer to genlock time
				Ptr->DispFunct();				//  and display it
				return;
			}
			else
				FuncState -= 30;
			break;
	}

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If genlock input has changed..
			ActivePresetNo = 0;						// Clear possible preset

			ActiveGenlockNo = UcharVal;			// Save new Genlock input

			if ( UcharVal < Internal)				// If new input isn't internal..
				ExternalGenlockNo = UcharVal;		//  update also external no
															// Update genlock unit
															// Update the genlock unit
			if ( ErrorCode = GenlockUnitDrv( UcharVal,\
														 GenlockConfig[UcharVal].System)) {
				GotoErrorStatus( ErrorCode);
				return;
			}
		}

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01){					// If button pressed is UP..
			if ( UcharVal-- == GenlockA)
				UcharVal = Internal2;

			switch ( UcharVal) {
				case SDIGenlock:
					if ( !SDIGenlockUnit.Present)
						UcharVal--;
					break;
				case Internal2:
					if ( !Int2GenlockUnit.Present)
						UcharVal--;
					break;
			}
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal++ == Internal2)
				UcharVal = GenlockA;

			switch ( UcharVal) {
				case SDIGenlock:
					if ( !SDIGenlockUnit.Present)
						UcharVal++;
					break;
				case Internal2:
					if ( !Int2GenlockUnit.Present)
						UcharVal = GenlockA;
					break;
			}
		}

		UcharVal1 = GenlockConfig[UcharVal].System;

		if ( UcharVal == ActiveGenlockNo)		// If actual genlock..
			FuncState = 1;								//  remove OK option
	}

	CodePtr = GenlockInputArrTxt[UcharVal];	// Get text for selection
	length = Cstrlen( CodePtr);					// Calculate length of string

	WriteCodeLN2( 0, InpHdrTxt);					// Write header on line 2
	WriteCode2( CodePtr);							// Write selected input on line 2

	if ( UcharVal < Internal) {
		WriteChar2( ' ');
															// Get signal for selected input
		CodePtr = GenlockSignalArrTxt[UcharVal1];
		length += Cstrlen( CodePtr)+1;			// Add length of this string

		WriteCode2( CodePtr);						// Write active signal on line 2
	}

	ClearRestLN2();                        	// Clear rest of line 2

	WriteType9_SAVE_ESC( FuncState);

	switch ( FuncState) {
		case 1:
		case 5:
			WriteItemArrows( 5, length);			// Mark selected item
			break;
		case 3:
		case 7:
			if ( UcharVal1 >= MHzLock358)
				WriteArrowsField( L_+R_);
		case 2:
		case 6:
			if ( UcharVal >= Internal)
				WriteArrowsField( L_+R_);
			break;
	}

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* GenlockSystemSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Setting the system of the genlock input							*/
/*	Remarks:		GENLOCK MENU function													*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void GenlockSystemSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                       // If genlock system changed..
			ActivePresetNo = 0;						// Clear possible preset

			GenlockConfig[UcharVal].System = UcharVal1;

			if ( UcharVal == ActiveGenlockNo) {
															// Update the genlock unit
				if ( ErrorCode = GenlockUnitDrv( UcharVal, UcharVal1)) {
					GotoErrorStatus( ErrorCode);
					return;
				}
			}
		}

		if ( UcharVal == ActiveGenlockNo)
			FuncState = 2;
		else
			FuncState = 6;

		GenlockInputShow();						// Display genlock input line 1

		FuncTriggers = 0;							// Prepare for item state-machine
		FuncSave = FuncEscape = FALSE;

		Ptr = &genlock_menu;						// Update pointer to signal menu
		Ptr->DispFunct();						//  and display it
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( UcharVal == SDIGenlock) {		// If SDI genlock selected..
			if ( UcharVal1 == SDI625Lock)		//  only 625 and 525 is available
				UcharVal1 = SDI525Lock;
			else
				UcharVal1 = SDI625Lock;
		}
		else {										// .. A,B or A-B genlock
			if ( FuncTriggers & 0x01) {		// If button pressed is UP..
				if ( UcharVal1-- == BurstLockPAL)
					UcharVal1 = MHzLock10;

				if ( UcharVal1 == SDI525Lock)
					UcharVal1 = SyncLock525;
			}
			else {									// ..button pressed is DOWN
				if ( UcharVal1++ == MHzLock10)
					UcharVal1 = BurstLockPAL;

				if ( UcharVal1 == SDI625Lock)
					UcharVal1 = MHzLock358;
			}
		}
															// If actual genlock system..
		if ( UcharVal1 == GenlockConfig[UcharVal].System)
			FuncState = 1;								//  remove SAVE option
	}

	CodePtr = GenlockSignalArrTxt[UcharVal1];	// Get text for selection

	WriteCodeLN2( 0, SystemHdrTxt);				// Write header on line 2
	WriteCode2( CodePtr);							// Write active signal on line 2

	ClearRestLN2();                        	// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* GenlockTimeSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960625				     													*/
/*																									*/
/*	Function:	Switch to the GENLOCK SIGNAL MENU									*/
/*	Remarks:		GENLOCK MENU MENU function												*/
/*					Cursor position on line 1 and 2 are updated 						*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, UcharVal1 and UcharVal2								*/
/***************************************************************************/
void GenlockTimeSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {									// If delay has changed..
			ActivePresetNo = 0;							// Clear possible preset
																// Save new delay
			GenlockConfig[UcharVal].Delay = samples;
		}
		else													// MUST be .Delay
			TrxGenlockDelay( GenlockConfig[UcharVal].Delay);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		if ( UcharVal == ActiveGenlockNo)
			FuncState = 3;
		else
			FuncState = 7;

		GenlockInputShow();						// Display genlock signal line 1

		FuncTriggers = 0;							//   Prepare for item state-machine
		FuncSave = FuncEscape = FALSE;

		Ptr = &genlock_menu;
		Ptr->DispFunct();
		return;
	}

	if ( UcharVal == SDIGenlock) {
		if ( UcharVal1 == SDI625Lock)
			SDITimeAdjust( 2, SDI625, GenlockConfig[UcharVal].Delay);
		else
			SDITimeAdjust( 2, SDI525, GenlockConfig[UcharVal].Delay);
	}
	else {
		if (( UcharVal1 == BurstLockPAL) || ( UcharVal1 == SyncLock625))
			AnalogTimeAdjust( 2, PAL, GenlockConfig[UcharVal].Delay);
		else
			AnalogTimeAdjust( 2, NTSC, GenlockConfig[UcharVal].Delay);
	}

	FuncTriggers = 0;								// Reset the button-selection
}

/***************************************************************************/
/* PresetMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Handle the display containing the SDI-SIGNAL functions		*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void PresetMenuShow(){

	register int i;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( Select_FunctionTxt);				// Display the headline

	ClearRestLN1();									// Clear rest of line 1

	PresetNdx = Ptr->ItemNumber;					// Update Preset Menu index

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for menu line

	for ( i = PresetRecallItem; i <= PresetNameItem; i++) {
		ClearToPosLN2( preset_menu[i].ItemPos);	// Write item position
		WriteCode2( preset_menu[i].ItemTxt);		// Write item text
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* PresetDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the PRESET MENU					*/
/*	Remarks:		PRESET MENU function														*/
/*					Cursor position on line 1 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, TextBuffer, UcharVal, UcharVal1, UcharVal2, FuncSave,	*/
/*					FunState and FuncTriggers												*/
/***************************************************************************/
void PresetDown(){

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( preset_menu[PresetNdx].ItemTxt);	// Write headline

	if ( ActivePresetNo)							// If preset is active..
		UcharVal = ActivePresetNo - 1;		//  select currently active preset
	else
		UcharVal = 0;								// ..otherwise select preset ONE

	switch ( PresetNdx) {
		case PresetRecallItem:
		case PresetStoreItem:
			WriteCode1( Select_Txt);
			break;

		case PresetNameItem:
			WriteCode1( Edit_NameTxt);

			strcpy( TextBuffer, Preset[UcharVal].Name);

			UcharVal1 = 0;								// Position in Text string
			break;
	}

	ClearRestLN1();									// Clear rest of line 1

	PrevPtr = Ptr;										// Store old menu-pointer
	Ptr = &preset_submenu[PresetNdx];			// Point to selected item

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Set select-mode
}

/***************************************************************************/
/* PresetRecallSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960217				     													*/
/*																									*/
/*	Function:	Select a complete preset to recall 									*/
/*	Remarks:		PRESET MENU function														*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void PresetRecallSelect(){

	if ( FuncSave || FuncEscape) {
		SelectModeOn = FALSE;						// Clear select-mode

		if ( FuncSave) {
			ActivePresetNo = UcharVal + 1;

			if ( ErrorCode = ConfigurePreset( UcharVal)) {
				GotoErrorStatus( ErrorCode);
				return;
			}
															// Set pointer to status menu
			Ptr = &status_menu[StatusNdx = StatusPresetItem];
			Ptr->DispFunct();						// Display previous menu
		}
		else {
			Ptr = PrevPtr;								// Set pointer to previous menu
			Ptr->DispFunct();							// Display previous menu
		}
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01){					// If button pressed is UP..
			if ( UcharVal++ == NoOfPresets-1)
				UcharVal = 0;
		}
		else												// ..button pressed is DOWN
			if ( UcharVal-- == 0)
				UcharVal = NoOfPresets-1;
	}

	TxtPtr = Preset[UcharVal].Name;				// Get text for selection

	WriteCodeLN2( 0, RecallHdrTxt);				// Write header on line 2
	WriteCharLN2( 8, '1' + UcharVal);			// This DESTROYS line 2 position

	WriteTxtLN2( 12, TxtPtr);						// Write selected text on line 2

	ClearRestLN2();                     		// Clear rest of line 2

	WriteType3_OK_ESC( FuncState);				// Write the SAVE ESC text

	if ( FuncState == 1)
		WriteItemArrows( 12, strlen( TxtPtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* PresetStoreSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Store a setup in preset													*/
/*	Remarks:		PRESET MENU function														*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void PresetStoreSelect(){

	register int i;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {

			ActivePresetNo = UcharVal + 1;

			for ( i = 0; i < NoOfAnlBlkUnits; i++)
				memcpy( &Preset[UcharVal].AnlBlkStore[i], &AnlBlkConfig[i], sizeof( AnlBlkConfig[0]));

			for ( i = 0; i < NoOfSDIBlkUnits; i++)
				memcpy( &Preset[UcharVal].SDIBlkStore[i], &SDIBlkConfig[i], sizeof( SDIBlkConfig[0]));

			memcpy( &Preset[UcharVal].AnalogStore, &AnalogConfig, sizeof( AnalogConfig));
			memcpy( &Preset[UcharVal].SDIStore, &SDIConfig, sizeof( SDIConfig));
			memcpy( &Preset[UcharVal].AES_EBUStore, &AES_EBUConfig, sizeof( AES_EBUConfig));

			for ( i = 0; i < NoOfGenlockUnits; i++)
				memcpy( &Preset[UcharVal].GenlockStore[i], &GenlockConfig[i], sizeof( GenlockConfig[0]));

			Preset[UcharVal].ActiveGenlockNo = ActiveGenlockNo;
			Preset[UcharVal].ExternalGenlockNo = ExternalGenlockNo;
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01){					// If button pressed is UP..
			if ( UcharVal++ == NoOfPresets-1)
				UcharVal = 0;
		}
		else												// ..button pressed is DOWN
			if ( UcharVal-- == 0)
				UcharVal = NoOfPresets-1;
	}

	TxtPtr = Preset[UcharVal].Name;				// Get text for selection

	WriteCodeLN2( 0, StoreHdrTxt);				// Write header on line 2
	WriteCharLN2( 7, '1' + UcharVal);			// This DESTROYS line 2 position

	WriteTxtLN2( 11, TxtPtr);						// Write selected text on line 2

	ClearRestLN2();									// Clear rest of line 2

	WriteType3_OK_ESC( FuncState);				// Write the SAVE ESC text

	if ( FuncState == 1)
		WriteItemArrows( 11, strlen( TxtPtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* PresetNameSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		961215				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void PresetNameSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave)
			strcpy( Preset[UcharVal].Name, TextBuffer);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Select preset to name
	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( UcharVal++ == NoOfPresets-1)
				UcharVal = 0;
		}													// ..button pressed is DOWN
		else {
			if ( UcharVal-- == 0 )
				UcharVal = NoOfPresets-1;
		}												// Copy selected preset name to
														//  temporary text buffer
		strcpy( TextBuffer, Preset[UcharVal].Name);

		FuncState = 1;
	}
														// Select TextBuffer position
	if ( FuncTriggers & 0xC0) {				// If LEFT/RIGHT button pressed..
		if ( FuncTriggers & 0x40) {			// If button pressed is LEFT..
			if( UcharVal1-- == 0)
				UcharVal1 = 15;
		}
		else {										// ..button pressed is RIGHT
			if( UcharVal1++ == 15)
				UcharVal1 = 0;
		}
	}
														// Select character for TextBuffer
	if ( FuncTriggers & 0x0C) {				// If UP/DOWN button pressed..

		TextBuffer[UcharVal1] =\
			 FindNextASCII(( FuncTriggers & 0x04), 0, TextBuffer[UcharVal1]);

		if ( !strcmp( Preset[UcharVal].Name, TextBuffer))
			FuncState = 6;
	}

	WriteCodeLN2( 0, NameHdrTxt);					// Write header on line 2
	WriteCharLN2( 6, '1'+UcharVal);				// This DESTROYS line 2 position

	WriteTxtLN2( 10, TextBuffer);					// Write selected text on line 2

	ClearRestLN2();									// Clear rest of line 2

	WriteType8_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 3:											// Mark selected item
			WriteItemArrows( 10, strlen( TextBuffer));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 6:
		case 7:											// Mark selected item
			WriteItemBrackets( 10, strlen( TextBuffer));
			CharBlinkLN2( 10+UcharVal1, ON);		// Switch OFF blinking at
			break;										//  editing character
	}

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* ConfigMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display the items in the BLACK-BURST SUBMENU						*/
/*	Remarks:		BLACK-BURST SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void ConfigMenuShow(){

	register int i, j;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( Select_FunctionTxt);				// Write the headline

	ClearRestLN1();

	ConfigNdx = Ptr->ItemNumber;

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for menu line

	if ( ConfigNdx < ConfigDownloadItem) {
		i = ConfigLockItem;
		j = ConfigDownloadItem;
	}
	else {
		i = ConfigDownloadItem;
		j = ConfigDiagnoseItem+1;
	}

	for ( ; i < j; i++) {
		ClearToPosLN2( config_menu[i].ItemPos);	// Write item position
		WriteCode2( config_menu[i].ItemTxt);		// Write item text
	}
	ClearRestLN2();										// Clear rest of line 2

	i = Ptr->ItemConfig;									// Get line1&2 icons/dots info

	if ( Flags.PanelLockOn || Flags.NormalLockOn) {
		if ( ConfigNdx != ConfigLockItem)
			i = ( i | P_) & ~D_;
	}
	else {
		if ( Flags.DiagnoseLockOn && ( ConfigNdx == ConfigDiagnoseItem))
				i = ( i | P_) & ~D_;

		if ( Flags.DownloadLockOn && ( ConfigNdx == ConfigDownloadItem))
				i = ( i | P_) & ~D_;
	}
	WriteArrowsField( i);							// Write icons/dots on line 1&2
															//  Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* ConfigDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Handle the display containing the SDI			 functions		*/
/*	Remarks:		SDI-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void ConfigDown(){

	if (( Flags.PanelLockOn || Flags.NormalLockOn) && ( ConfigNdx != ConfigLockItem))
		return;

	if ( Flags.DiagnoseLockOn && ( ConfigNdx == ConfigDiagnoseItem))
		return;

	if ( Flags.DownloadLockOn && ( ConfigNdx == ConfigDownloadItem))
		return;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( config_menu[ConfigNdx].ItemTxt);

	PrevPtr = Ptr;								// Store old menu-pointer setting
													// DO NOT MOVE THIS LINE ANYWHERE ELSE
	switch ( ConfigNdx) {
		case ConfigLockItem:
			UcharVal = NormalLock;

			Ptr = &config_submenu[ConfigNdx];
			break;

		case ConfigAutoESCItem:
			WriteCode1( Select_Txt);

			Ptr = &config_submenu[ConfigNdx];
			break;

		case ConfigLCDContItem:
			UcharVal1 = Max_Contrast;
			UcharVal2 = Min_Contrast;

			Ptr = &config_submenu[ConfigNdx];
			break;

		case ConfigDownloadItem:
			WriteCode1( Select_Txt);

			UcharVal = DwnPresetNo;

			Ptr = &config_submenu[ConfigNdx];
			break;

		case ConfigRS232Item:
			Ptr = &RS232_menu[RS232Ndx];
			break;

		case ConfigDiagnoseItem:
			WriteCode1( Select_Txt);

			Ptr = &diagnose_menu[DiagnoseNdx];
			break;
	}
	ClearRestLN1();									// Clear rest of line 1

	TmpFlags = Flags;

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Set select mode
}

/***************************************************************************/
/* UpToConfig																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Select the CONFIG MENU and restore the last selected item	*/
/*	Remarks:		CONFIG MENU function														*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToConfig(){

	Ptr = &config_menu[ConfigNdx];
}

/***************************************************************************/
/* ConfigLockSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960627				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void ConfigLockSelect(){

	if ( FuncEscape) {

		SelectModeOn = FALSE;						// Clear select mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..
		if (( FuncTriggers & 0x03) == 1) {		// If button pressed is UP..
			if ( UcharVal-- == NormalLock)
				UcharVal = DiagnoseLock;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal++ == DiagnoseLock)
				UcharVal = NormalLock;
		}
	}

	switch ( UcharVal) {
		case NormalLock:								// If NORMAL LOCK selected..
			if ( FuncSave){
				FuncSave = FALSE;
				Flags.NormalLockOn = TmpFlags.NormalLockOn;
			}

			UcharVal1 = ( UC) Flags.NormalLockOn;

			if ( FuncTriggers & 0x0C) {			// If button UP/DOWN pressed..
				TmpFlags.NormalLockOn ^= 0x01;	//  toggle selection

				if ( TmpFlags.NormalLockOn == UcharVal1)
					FuncState = 2;
			}

			UcharVal2 = ( UC) TmpFlags.NormalLockOn;
			break;

		case PanelLock:								// If PANEL LOCK selected..
			if ( FuncSave) {
				FuncSave = FALSE;
				Flags.PanelLockOn = TmpFlags.PanelLockOn;
			}
			UcharVal1 = ( UC) Flags.PanelLockOn;

			if ( FuncTriggers & 0x0C) {			// If button UP/DOWN pressed..
				TmpFlags.PanelLockOn ^= 0x01;		//  toggle selection

				if ( TmpFlags.PanelLockOn == UcharVal1)
					FuncState = 2;
			}

			UcharVal2 = ( UC) TmpFlags.PanelLockOn;
			break;

		case DownloadLock:							// If DOWNLOAD LOCK selected..
			if ( FuncSave) {
				FuncSave = FALSE;
				Flags.DownloadLockOn = TmpFlags.DownloadLockOn;
			}
			UcharVal1 = ( UC) Flags.DownloadLockOn;

			if ( FuncTriggers & 0x0C) {			// If button UP/DOWN pressed..
				TmpFlags.DownloadLockOn ^= 0x01;	//  toggle selection

				if ( TmpFlags.DownloadLockOn == UcharVal1)
					FuncState = 2;
			}

			UcharVal2 = ( UC) TmpFlags.DownloadLockOn;
			break;

		case DiagnoseLock:							// If DIAGNOSE LOCK selected..
			if ( FuncSave) {
				FuncSave = FALSE;
				Flags.DiagnoseLockOn = TmpFlags.DiagnoseLockOn;
			}
			UcharVal1 = ( UC) Flags.DiagnoseLockOn;

			if ( FuncTriggers & 0x0C) {			// If button UP/DOWN pressed..
				TmpFlags.DiagnoseLockOn ^= 0x01;	//  toggle selection

				if ( TmpFlags.DiagnoseLockOn == UcharVal1)
					FuncState = 2;
			}

			UcharVal2 = ( UC) TmpFlags.DiagnoseLockOn;
			break;
	}

	if ( FuncTriggers & 0x03) {				// DO NOT MOVE TO THE ONE ABOVE
		if ( UcharVal2 == UcharVal1)			// !!!!!!!!!!!!!
			FuncState = 1;
		else
			FuncState = 5;
	}

	WriteTxtLN1( 19, ", ");						// Write headline and actual value

	WriteCode1( LockArrTxt[UcharVal]);		// Get display text for selection

	WriteChar1( '(');
	WriteCode1( OffOnArrTxt[UcharVal1]);
	WriteChar1( ')');										// Write the ON/OFF text

	ClearRestLN1();										// Clear rest of line 1

	WriteCodeLN2( 0, SelectHdrTxt);					// Write LINE 2 header
	ClearRestLN2();

	WriteType2_SAVE_ESC( FuncState);					// Write the ESC text

	WriteCodeLN2( 8, LockArrTxt[UcharVal]);
	WriteCodeLN2( 21, OffOnArrTxt[ UcharVal2]);

	switch ( FuncState) {
		case 1:
		case 5:												// Mark selected item
			WriteItemArrows( 8, Cstrlen( LockArrTxt[UcharVal]));
			break;

		case 2:
		case 4:												// Mark selected item
			WriteItemArrows( 21, Cstrlen( OffOnArrTxt[ UcharVal2]));
			break;
	}

	FuncTriggers = 0;										// Reset button-selection
}

/***************************************************************************/
/* ConfigAutoESCSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void ConfigAutoESCSelect(){

	if ( FuncSave || FuncEscape) {

		if ( FuncSave)
			Flags.AutoESCEnable = TmpFlags.AutoESCEnable;	// Save selection

		SelectModeOn = FALSE;						// Clear select mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button UP/DOWN pressed..
		TmpFlags.AutoESCEnable ^= 0x01;			//  toggle selection

		if ( TmpFlags.AutoESCEnable == Flags.AutoESCEnable)
			FuncState = 1;
	}
																// get text for selection
	CodePtr = OffOnArrTxt[( UC) TmpFlags.AutoESCEnable];

	WriteCodeLN2( 0, AutoReturnStatusHdrTxt);
	WriteCode2( CodePtr);

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 23, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset button selection
}

/***************************************************************************/
/* ConfigLCDContSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void ConfigLCDContSelect(){

	register int i;

	if ( FuncEscape) {

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	WriteCGChar( 6, 0x1A);							// Download the 10/10 bar to the
															//  display controller

	if ( FuncTriggers & 0x03) {					// If button UP/DOWN pressed..
		if (( FuncTriggers & 0x03) == 1) {		// If button UP pressed..
			if ( LCDContrast++ == UcharVal1)
				LCDContrast = UcharVal1;
		}
		else												// ..button DOWN pressed
			if ( LCDContrast-- == UcharVal2)
				LCDContrast = UcharVal2;
	}
															// Send contrast to immediately
	if ( ErrorCode = Send_Contrast( LCDContrast)) {
		GotoErrorStatus( LCD_ADCErrorType + ErrorCode);
		return;
	}

	if ( FuncState == 1)
		WriteCharLN2( 0, '<');
	else
		WriteCharLN2( 0, '|');

	for ( i = 0; i < LCDContrast; i++)
		WriteChar2( 6);

	for ( i = LCDContrast; i < UcharVal1; i++)
		WriteChar2( '_');

	if ( FuncState == 1)
		WriteChar2( '>');
	else
		WriteChar2( '|');

	ClearRestLN2();									// Clear rest of line 2

	WriteType4_SAVE_ESC( FuncState);				// Write the ESC text

	FuncTriggers = 0;									// Reset button-selection
}

/***************************************************************************/
/* DownloadTypeSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970223				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void DownloadTypeSelect(){

	UC tmp;

	if ( FuncSave || FuncEscape) {

		Flags.AutoESCToStatus = TRUE;				// Auto escape to status menu

		if ( FuncSave) {
			FuncTriggers = 0;							// Prepare for item state-machine
			FuncSave = FuncEscape = FALSE;		// NOTE! FuncState should NOT be
															//  initialized

			Ptr = &download_submenu;				// Point to DownloadTestSPG menu
			Ptr->DispFunct();							// Display menu
		}
		else {
			Flags = TmpFlags;							// Restore flags

			SelectModeOn = FALSE;					// Clear select mode

			Ptr = PrevPtr;								// Set pointer to previous menu
			Ptr->DispFunct();							// Display previous menu
		}
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..
		if ( FuncTriggers & 0x01){					// If button pressed is UP..
//			if ( UcharVal++ == DwnCompleteSPG)	// Removed
			if ( UcharVal++ == DwnAllPreset)
				UcharVal = DwnPresetNo;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal-- == DwnPresetNo)
				UcharVal = DwnAllPreset;
//				UcharVal = DwnCompleteSPG;			// Remove
		}
	}

	if ( UcharVal < DwnAllPreset)	{				// Get text for selection
		tmp = 1;
		CodePtr = DownloadArrTxt[DwnPresetNo];
	}
	else {
		tmp = 0;
		CodePtr = DownloadArrTxt[UcharVal-DwnAllPreset+1];
	}

	tmp += Cstrlen( CodePtr);

	WriteCodeLN2( 0, DownloadHdrTxt);			// Write header on line 2
	WriteCode2( CodePtr);							// Write selected input on line 2

	if ( UcharVal < DwnAllPreset)
		WriteChar2( UcharVal + '1');

	ClearRestLN2();                        	// Clear rest of line 2

	WriteType3_OK_ESC( FuncState);

	if ( FuncState == 1)
		WriteItemArrows( 10, tmp);					// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* UpToDownload																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970223				     													*/
/*																									*/
/*	Function:	Return to download type select menu									*/
/*	Remarks:		DOWNLOAD SUBMENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToDownload(){

	WriteCodeLN1( 0, SubmnuHdrTxt);		// Write line 1 header and selection
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteCode1( Select_Txt);
	ClearRestLN1();

	Flags = TmpFlags;							// Restore flags

	FuncState = 1;								// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	Ptr = &config_submenu[ConfigNdx];
}

/***************************************************************************/
/* DownloadTestSPG																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970223				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the DOWNLOAD-SUBMENU			*/
/*	Remarks:		DOWNLOAD SUBMENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void DownloadTestSPG(){

	int RS232Char;

	if ( FuncSave || FuncEscape) {

		if ( FuncEscape || !ResponseReceived) {
			UpToDownload();
			Flags.AutoESCToStatus = FALSE;	// Auto escape to download menu
		}
		else {
			FuncState = 2;							// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			Ptr = &download_2ndmenu;			// Prepare for final download menu
		}
		Ptr->DispFunct();
		return;
	}

	strcpy( TextBuffer, "*IDN?");				// Find a SPG at the other side
	ResponseHandler( TextBuffer);				//  and get the KU number

	UserTimer = 40;
	while ( !ResponseReceived && UserTimer)
		while (( RS232Char = Udr_InChar()) != UDR_EOF)
			CmdHandler(( char) RS232Char);


	if ( ResponseReceived == 0xFF) {

		WriteCodeLN1( 0, DownloadTxt);		 // Write header on line 2
		WriteChar1( ' ');
		WriteCode1( CodePtr);					 // Write selected input on line 2

		if ( UcharVal < DwnAllPreset)
			WriteChar1( UcharVal + '1');

		WriteCode1( FromKUTxt);

		_ultoa( ResponseKUNo, TextBuffer);
		WriteTxt1( TextBuffer);
		ClearRestLN1();

		WriteCodeLN2( 0, PressExecuteToStartTxt);
		ClearRestLN2();
	}
	else
		MessageHandling( NoResponseFromRS232);

	WriteArrowsField( R_+U_+D_+L_+E_);

	AutoESCTimer = AutoESCTimeOut - 10;			// Prepare a 10 sec timeout
	AutoESCSignal = FALSE;							//  for this menu

	Flags.AutoESCEnable = TRUE;
}

/***************************************************************************/
/* DownloadFromSPG																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971212				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the DOWNLOAD-SUBMENU			*/
/*	Remarks:		DOWNLOAD SUBMENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void DownloadFromSPG(){

	int RS232Char, i;

	if ( FuncSave || FuncEscape) {

		if ( FuncSave) {

			Flags.AutoESCEnable = FALSE;			// Disable auto escape

			WriteArrowsField( U_);

			WriteCodeLN2( 0, WarningDownloadInTxt);
			ClearRestLN2();

			WriteCodeLN2( 36, ESCTxt);
			WriteItemArrows( 36, 3);

			strcpy( TextBuffer, ":SYST:PRES:DOWN  ");

			if ( UcharVal == DwnAllPreset) {

				for ( i = 1; i < 9; i++) {

					TextBuffer[16] = '0' + i;
					ResponseHandler( TextBuffer);

					UserTimer = 40;
					while ( !ResponseReceived && UserTimer)
						while (( RS232Char = Udr_InChar()) != UDR_EOF)
							CmdHandler(( char) RS232Char);

					if ( !ResponseReceived)
						i = 8;
					else {
						UserTimer = 20;				// Delay to allow recipient to
						while( UserTimer)				//  update it's memory
							feed_dog;
					}
				}
			}
			else {
				TextBuffer[16] = '1' + UcharVal;
				ResponseHandler( TextBuffer);

				UserTimer = 40;
				while ( !ResponseReceived && UserTimer)
					while (( RS232Char = Udr_InChar()) != UDR_EOF)
						CmdHandler(( char) RS232Char);
			}

			if ( ResponseReceived < 0xFF) {
				switch ( ResponseReceived) {
					case InvalidBlockData:
						MessageHandling( ErrorDownloadingFromRS232);
						break;

					case InvalidVersion:
						MessageHandling( ErrorInDataFromRS232);
						break;

					default:
						MessageHandling( NoResponseFromRS232);
						break;
				}

				UserTimer = 100;
				while( UserTimer && !Button_Ready())
					feed_dog;
			}
		}
		UpToDownload();
		Ptr->DispFunct();
		return;
	}

	WriteCodeLN2( 0, WarningAboutToAlterTxt);
	ClearRestLN2();

	WriteType10_OK_ESC( FuncState);

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* RS232MenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		970226				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void RS232MenuShow(){

	register int i, j;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteCode1( Select_Txt);

	ClearRestLN1();									// Clear rest of line 1

	RS232Ndx = Ptr->ItemNumber;					// Update RS232 Menu index

	WriteCodeLN2( 0, SecondmnuHdrTxt);			// Write header for menu line

	i = j = RS232HandshakeItem;

	if ( RS232Ndx < RS232HandshakeItem)
		i = RS232BaudrateItem;
	else
		j++;

	for ( ; i < j ; i++) {
		ClearToPosLN2( RS232_menu[i].ItemPos);	// Write item position
		WriteCode2( RS232_menu[i].ItemTxt);		// Write item text
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2

															//  Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* RS232Down																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		920226				     													*/
/*																									*/
/*	Function:	Switch to the selected item in the RS232-SUBMENU				*/
/*	Remarks:		RS232 SUBMENU function													*/
/*					Cursor position on line 1 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, PrevPtr, UcharVal, FuncSave, FunState, FuncTriggers		*/
/*					SelectModeOn																*/
/***************************************************************************/
void RS232Down(){

	WriteCodeLN1( 0, SecondmnuHdrTxt);
	WriteCode1( SlashDotHdrTxt);
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( RS232_menu[RS232Ndx].ItemTxt);		// Write headline
	WriteCode1( Select_Txt);

	switch ( RS232Ndx) {
		case RS232BaudrateItem:
			UcharVal = RS232Config.Baudrate;
			break;

		case RS232DatabitItem:
			UcharVal = RS232Config.Databit;
			break;

		case RS232ParityItem:
			UcharVal = RS232Config.Parity;
			break;

		case RS232HandshakeItem:
			UcharVal = RS232Config.Handshake;
			break;
	}
	ClearRestLN1();									// Clear rest of line 1

	PrevPtr = Ptr;										// Store old menu-pointer
	Ptr = &RS232_submenu[RS232Ndx];				// Point to selected item

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Set select-mode ON
}

/***************************************************************************/
/* RS232BaudrateSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		961010				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		RS232 INTERFACE MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, CodePtr, FuncSave, FunState, FuncTriggers,					*/
/*					SelectModeOn																*/
/***************************************************************************/
void RS232BaudrateSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If baudrate has changed..
			RS232Config.Baudrate = UcharVal;		// Save new baudrate info
															// Re-initialize UART
			Udr_InitializeUart( RS232Config.Baudrate, RS232Config.Databit,\
											RS232Config.Parity, RS232Config.Handshake);
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( UcharVal++ == UDR_BAUD_9600)
				UcharVal = UDR_BAUD_300;
		}
		else												// ..button pressed is DOWN
			if ( UcharVal-- == UDR_BAUD_300)
				UcharVal = UDR_BAUD_9600;

		if ( UcharVal == RS232Config.Baudrate)
			FuncState = 1;
	}

	CodePtr = RS232BaudrateArrTxt[UcharVal];	// Get text for selection

	WriteCodeLN2( 0, BaudrateHdrTxt);			// Write header on line 2
	WriteCode2( CodePtr);							// Write selected text on line 2

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 11, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* RS232DatabitSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		961010				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		RS232 INTERFACE MENU function											*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, CodePtr, FuncSave, FunState, FuncTriggers,					*/
/*					SelectModeOn																*/
/***************************************************************************/
void RS232DatabitSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If baudrate has changed..
			RS232Config.Databit = UcharVal;		// Save new data bit info
															// Re-initialize UART
			Udr_InitializeUart( RS232Config.Baudrate, RS232Config.Databit,\
											RS232Config.Parity, RS232Config.Handshake);
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..

		if ( UcharVal == UDR_DATA8)
			UcharVal = UDR_DATA7;
		else
			UcharVal = UDR_DATA8;

		if ( UcharVal == RS232Config.Databit)
			FuncState = 1;
	}

	WriteCodeLN2( 0, DatabitHdrTxt);				// Write header on line 2
	WriteChar2( '0' + UcharVal);
	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 10, 1);					// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* RS232ParitySelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		961010				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void RS232ParitySelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave)	{								// If parity info has changed..
			RS232Config.Parity = UcharVal;  		// Save new parity info
															// Re-initialize UART
			Udr_InitializeUart( RS232Config.Baudrate, RS232Config.Databit,\
												RS232Config.Parity, RS232Config.Handshake);
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( UcharVal-- == UDR_NONE)
				UcharVal = UDR_EVEN;
		}
		else												// ..button pressed is DOWN
			if ( UcharVal++ == UDR_EVEN)
				UcharVal = UDR_NONE;

		if ( UcharVal == RS232Config.Parity)
			FuncState = 1;
	}

	CodePtr = RS232ParityArrTxt[UcharVal];		// Get text for selected parity

	WriteCodeLN2( 0, ParityHdrTxt);				// Write header on line 2
	WriteCode2( CodePtr);							// Write selected text on line 2

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* RS232HandshakeSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		961010				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void RS232HandshakeSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If handshake has changed
			RS232Config.Handshake = UcharVal;	// Save new handshake method
															// Re-initialize UART
			Udr_InitializeUart( RS232Config.Baudrate, RS232Config.Databit,\
												RS232Config.Parity, RS232Config.Handshake);
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..

		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( UcharVal-- == UDR_XON_XOFF)
				UcharVal = UDR_RTS_CTS;
		}
		else												// ..button pressed is DOWN
			if ( UcharVal++ == UDR_RTS_CTS)
				UcharVal = UDR_XON_XOFF;

		if ( UcharVal == RS232Config.Handshake)
			FuncState = 1;
	}

	CodePtr = RS232HandshakeArrTxt[UcharVal];	// Get text for selection

	WriteCodeLN2( 0, HandshakeHdrTxt);			// Write header on line 2
	WriteCode2( CodePtr);							// Write selected text on line 2

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC texct

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 11, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* DiagnoseMenuShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Display the items in the DIAGNOSE SUBMENU							*/
/*	Remarks:		DIAGNOSE SUBMENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void DiagnoseMenuShow(){

	register int i, j;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteCode1( Select_Txt);						// Write sub-menu headline

	ClearRestLN1();

	WriteCodeLN2( 0, SelectHdrTxt);				// Write header for menu line

	DiagnoseNdx = Ptr->ItemNumber;
															// Write DIAGNOSE-SUBMENU items
	i = j = DiagDisplayItem;

	if ( DiagnoseNdx < DiagDisplayItem)
		i = DiagMainItem;
	else {
		i = j = DiagConfigItem;

		if ( DiagnoseNdx < DiagConfigItem)
			i = DiagDisplayItem;
		else
			j = DiagErrorQueueItem+1;
	}

	for ( ; i < j; i++) {
		ClearToPosLN2( diagnose_menu[i].ItemPos);		// Write item position
		WriteCode2( diagnose_menu[i].ItemTxt);			// Write item text
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
															//  Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* DiagnoseDown																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971017				     													*/
/*																									*/
/*	Function:	Execute the selected item in the DIAGNOSE-SUBMENU				*/
/*	Remarks:		DIAGNOSE function															*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, PrevPtr, AutoEscEnable, IntVal, UcharVal, UcharVal1,	*/
/*					UcharVal2, TmpFlags, Flags, FuncState, FuncSave, FuncEscape,*/
/*					FuncTrigger, SelectModeOn, UpdateTimer, UpdateFrequency, 	*/
/*					UpdateSignal and UpdateEnable											*/
/***************************************************************************/
void DiagnoseDown() {

	WriteCodeLN1( 0, SecondmnuHdrTxt);		// Write secondmenu headline
	WriteCode1( SlashDotHdrTxt);
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( diagnose_menu[DiagnoseNdx].ItemTxt);

	ClearRestLN1();								// Clear rest of line 1

	PrevPtr = Ptr;									// Store old menu-pointer
	Ptr = &diagnose_submenu[DiagnoseNdx];	// Point to selected item

	switch ( DiagnoseNdx) {
		case DiagMainItem:						// Initialize the main test
			UpdateEnable = TRUE;					// Enable the update function
			UpdateSignal = TRUE;					// SW initiate a update sequence

			UpdateFrequency = 4;					// Update every 0.8 second
			UpdateTimer = UpdateFrequency;	// Intialize updatetimer

			UcharVal = 0;							// Start testing the mainboard
			UcharVal2 = FALSE;					// Set OK flag for global test
			break;

		case DiagOptionsItem:					// Initialize the options test
			UpdateEnable = TRUE;					// Enable the update function
			UpdateSignal = TRUE;					// SW initiate a update sequence

			UpdateFrequency = 4;					// Update every 0.8 second
			UpdateTimer = UpdateFrequency;	// Intialize updatetimer

														// Find number of options installed
			for ( UcharVal = BB34Conn; UcharVal <= TimeConn; UcharVal++)
				if ( Connectors[UcharVal].Present)
					break;

			if ( UcharVal > TimeConn)			// If no options present..
				UcharVal = 0;						//  clear UcharVal ie. no of options

			UcharVal2 = FALSE;					// Set OK flag for global test
			break;

		case DiagRS232Item:						// Initialize the ext. RS232 test
			UpdateEnable = TRUE;					// Enable the update function
			UpdateSignal = TRUE;					// SW initiate a update sequence

			UpdateFrequency = 4;					// Update every 0.8 second
			UpdateTimer = UpdateFrequency;	// Intialize updatetimer
														// Test if connector is present
			UcharVal = ( UC) Udr_RS232LoopbackTest();
			break;

		case DiagDisplayItem:					// Initialise the display test
			UpdateEnable = TRUE;					// Enable the update function
			UpdateSignal = TRUE;					// SW initiate a update sequence

			UpdateFrequency = 8;					// Update every 1.6 second
			UpdateTimer = UpdateFrequency;	// Intialize updatetimer
			break;

		case DiagKeyboardItem:					// Initialize the keyboard test
			UpdateEnable = TRUE;					// Enable the update function
			UpdateSignal = TRUE;					// SW initiate a update sequence

			UpdateFrequency = 4;					// Update every 0.8 second
			UpdateTimer = UpdateFrequency;	// Intialize updatetimer

			UcharVal = 1;							// Button selector
			UcharVal1 = 0;							// Buttons pressed
			break;

		case DiagMemItem:							// Initialize the main. memory test
			UpdateEnable = TRUE;					// Enable the update function
			UpdateSignal = TRUE;					// SW initiate a update sequence

			UpdateFrequency = 4;					// Update every 0.8 second
			UpdateTimer = UpdateFrequency;	// Intialize updatetimer
			break;

		case DiagConfigItem:						// Display the hardware configuration
		case DiagErrorQueueItem:				// Display the error queue
			UcharVal = 0;							//
			break;
	}
	IntVal = 0;										// Update timing counter

	TmpFlags = Flags;								// Temporarily save flag information
	Flags.AutoESCEnable = OFF;					// Disable the auto escape function

	FuncState = 1;									// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;							// Set select-mode
}

/***************************************************************************/
/* DiagnoseMainTest																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971027				     													*/
/*																									*/
/*	Function:	Handle the display containing the Mainbaord diagnose test	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, SelectModeOn, UpdateSignal, UpdateEnable, FuncSave,		*/
/*					FunState, IntVal															*/
/***************************************************************************/
void DiagnoseMainTest(){

	if ( FuncEscape) {
		SelectModeOn = FALSE;			// Clear select-mode
		UpdateEnable = FALSE;			// Disable update function

		Flags = TmpFlags;					// Restore original auto escape function

		Ptr = PrevPtr;						// Set pointer to previous menu
		Ptr->DispFunct();					// Display previous menu
		return;
	}

	if ( FuncSave) {						// If execute pressed in FuncState > 1..
		FuncSave = FALSE;					//
		FuncState = 1;						// Set state machine at state 1

		UpdateSignal = TRUE;				// Reinitialize update timer & signal
		UpdateTimer = UpdateFrequency;
	}

	if ( UpdateSignal) {					// Pressing a key do NOT cause an update
		UpdateSignal = FALSE;

		if ( FuncState == 1) {
			switch( IntVal) {
				case 0:						// Mainboard test setup
					switch ( UcharVal) {							// Header text
						case 0:	
							WriteCodeLN2( 0, TestingMainboardTxt);
							break;

						case 1:	
							WriteCodeLN2( 0, TestingBlackburstTxt);
							break;

						case 2:	
							WriteCodeLN2( 0, TestingSPGBoardTxt);
							break;
					}
					ClearRestLN2();							

					WriteCharLN2( 35, '>');
					break;

				case 1:						// Mainboard test 1
					switch ( UcharVal) {
						case 0:
							UcharVal1 = TestMainboard();
							break;

						case 1:
							UcharVal1= TestAnlBlkUnit( BB12Conn);
							break;

						case 2:
							UcharVal1 = TestSPGUnit();					
							break;
					}
					WriteChar2( '>');
					break;

				case 5:
					if ( UcharVal1 == 0)			// If NO errors in tested unit
						FuncState = 2;				//  display OK text
					else {							// otherwise..
						FuncState = 3;				//  display error text
						UcharVal2 = TRUE;			//  and remember the error
					}
					UcharVal++;						// Prepare for the next test

					ClearRestLN2();
					break;

				case 6:
					if ( UcharVal2 == 0)			// If NO errors in the tests
						FuncState = 2;				//  display OK text..
					else								// otherwise..
						FuncState = 3;				//  display FAIL text

					UcharVal = 0;
					UcharVal2 = FALSE;			// Prepare OK flag for global test

					WriteCodeLN2( 0, TestingMainCompletedTxt);
					ClearRestLN2();
					break;

				default:
					WriteChar2( '>');
					break;
			}

			if ( UcharVal < 3) {
				if ( IntVal < 5)
					IntVal++;
				else
					IntVal = 0;
			}
			else
				IntVal++;
		}
		WriteType11_OK_FAIL( FuncState);				// Write the OK or FAIL text
	}
}

/***************************************************************************/
/* DiagnoseOptionsTest															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971024				     													*/
/*																									*/
/*	Function:	Handle the display containing the Optionsdiagnose test		*/
/*	Remarks:		IntVal holds the timer value, ie. the arrows						*/
/*					UcharVal holds the number of options installed					*/
/*					UcharVal1 holds the result of the single options test			*/
/*					UcharVal2 holds the global result of the options test			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseOptionsTest(){

	UC tmpHWType, tmpHWVersion;

	if ( FuncEscape) {
		SelectModeOn = FALSE;			// Clear select-mode
		UpdateEnable = FALSE;			// Disable update function

		Flags = TmpFlags;					// Restore original auto escape function

		Ptr = PrevPtr;						// Set pointer to previous menu
		Ptr->DispFunct();					// Display previous menu
		return;
	}

	if ( FuncSave) {						// If execute pressed in FuncState > 1..
		FuncSave = FALSE;					//
		FuncState = 1;						// Set state machine at state 1

		UpdateSignal = TRUE;				// Reinitialize update timer & signal
		UpdateTimer = UpdateFrequency;
	}

	if ( UpdateSignal) {					// Pressing a key do NOT cause an update
		UpdateSignal = FALSE;

		if ( UcharVal) {					// If one or more options are present..
			if ( FuncState == 1) {
				switch( IntVal) {
					case 0:					// Options test: setup
						WriteCodeLN2( 0, TestingTxt);

						tmpHWType = Connectors[UcharVal].HWType;
						tmpHWVersion = Connectors[UcharVal].HWVersion;

						sprintf( TextBuffer, "PT86%02d", tmpHWType);

						if ( tmpHWVersion)
							sprintf( &TextBuffer[6], "/9%02d", tmpHWVersion);

						WriteTxt2( TextBuffer);

						switch ( UcharVal) {
							case BB78Conn:
								if ( tmpHWType == PT8601) {
									WriteTxt2( " in [ANLS]");
									break;
								}										// Break IS missing!!

							case BB34Conn:
								if ( tmpHWType == PT8604) {
									WriteTxt2( " in [BBMU]");
									break;
								}										// Break IS missing!!

							case BB56Conn:
								WriteTxt2( " in ");

								Cstrcpy( TextBuffer, ConnectorArrTxt[UcharVal]);

								if ( tmpHWType == PT8609)
									TextBuffer[1] = 'S';

								WriteTxt2( TextBuffer);
								break;
						}
						ClearRestLN2();

						WriteCharLN2( 35, '>');
						break;

					case 1:									// Options test 1:
						switch ( Connectors[UcharVal].HWType) {
							case PT8601:					// Analog Test signal generator
								UcharVal1 = TestAnalogSignalUnit();
								break;

							case PT8602:					// SDI Test signal generators
							case PT8603:
								UcharVal1 = TestSDISignalUnit();
								break;

							case PT8604:					// Multi black burst module
								UcharVal1 = TestBBMulUnit();
								break;

							case PT8605:					// AES/EBU audio generator
								UcharVal1 = TestAES_EBUUnit();
								break;

							case PT8606:					// SDI Genlock
								UcharVal1 = TestSDIGenlockUnit();
								break;

							case PT8607:					// Timecode module
								UcharVal1 = TestTimecodeUnit();
								break;

							case PT8608:					// Black burst generator
								UcharVal1 = TestAnlBlkUnit( UcharVal);
								break;

							case PT8609:					// SDI Black generator
								UcharVal1 = TestSDIBlkUnit( UcharVal);
								break;

							case PT8610:					// High precision oscillator
								UcharVal1 = TestHighPrecisionOCXUnit();
								break;
						}
						WriteChar2( '>');
						break;

					case 5:								// Options test: single result
						if ( UcharVal1 == 0)			// If NO errors in option
							FuncState = 2;				//  display OK text
						else {							// otherwise..
							FuncState = 3;				//  display error text
							UcharVal2 = TRUE;			//  and remember the error
						}
						ClearRestLN2();

						for ( UcharVal++; UcharVal <= TimeConn; UcharVal++)
							if ( Connectors[UcharVal].Present)
								break;
						break;

					case 6:								// Options test: ALL result	
						for ( UcharVal = BB34Conn; UcharVal <= TimeConn; UcharVal++)
							if ( Connectors[UcharVal].Present)
								break;

						if ( UcharVal2 == FALSE)	// If NO errors in options
							FuncState = 2;				//  display OK text..
						else								// otherwise..
							FuncState = 3;				//  display FAIL text

						UcharVal2 = FALSE;			// Prepare OK flag for global test

						WriteCodeLN2( 0, TestingOptionsCompletedTxt);
						ClearRestLN2();
						break;

					default:
						WriteChar2( '>');
						break;
				}

				if ( UcharVal <= TimeConn) {
					if ( IntVal < 5)
						IntVal++;
					else
						IntVal = 0;
				}
				else
					IntVal++;
			}

			WriteType11_OK_FAIL( FuncState);			// Write the OK or FAIL text
		}
		else {								// ..otherwise write no options.. text
			WriteCodeLN2( 0, NoOptionsDetectedTxt);
			ClearRestLN2();

			WriteArrowsField( U_);
		}
	}
}

/***************************************************************************/
/* DiagnoseRS232Test																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971017				     													*/
/*																									*/
/*	Function:	Handle the display containing the external RS232 test			*/
/*	Remarks:		IntVal holds the timer value, ie. the arrows						*/
/*					UcharVal holds the information about the loopback connector	*/
/*					UcharVal1 holds the result of the RS232 test						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseRS232Test(){

	UC i;

	if ( FuncEscape) {
		RS232DiagnoseMode = FALSE;		// Clear RS232 diagnose flag
												// Re-initialize UART
		Udr_InitializeUart( RS232Config.Baudrate, RS232Config.Databit,\
												RS232Config.Parity, RS232Config.Handshake);

		SelectModeOn = FALSE;			// Clear select-mode
		UpdateEnable = FALSE;			// Disable update function

		Flags = TmpFlags;					// Restore original auto escape function

		Ptr = PrevPtr;						// Set pointer to previous menu
		Ptr->DispFunct();					// Display previous menu
		return;
	}

	if ( FuncSave) {						// If execute pressed in FuncState > 1..
		FuncSave = FALSE;
		FuncState = 1;						// Set state machine at state 1

		UpdateSignal = TRUE;				// Reinitialize update timer & signal
		UpdateTimer = UpdateFrequency;
												// Test if connector is (still) present
		UcharVal = ( UC) Udr_RS232LoopbackTest();
	}

	if ( UpdateSignal) {					// Pressing a key do NOT cause an update
		UpdateSignal = FALSE;

		if ( UcharVal) {					// If loopback connector is present..
			RS232DiagnoseMode = TRUE;	//  start testing

			if ( FuncState == 1) {
				switch( IntVal) {
					case 0:							// RS232 comm. test: setup
						WriteCodeLN2( 0, TestingRS232CommTxt);	// Header text
						ClearRestLN2();

						UcharVal1 = 1;				// Error flag, 1 = OK

						WriteCharLN2( 35, '>');
						break;

					case 1:							// RS232 comm. test 1: 300 baud
						UcharVal1 = RS232CheckOK( UDR_BAUD_300);
						WriteChar2( '>');
						break;

					case 2:							// RS232 comm. test 2: 600 baud
						if ( UcharVal1)
							UcharVal1 = RS232CheckOK( UDR_BAUD_600);
						WriteChar2( '>');
						break;

					case 3:							// RS232 comm. test 3: 1200 - 2400
						if ( UcharVal1)
							for ( i = UDR_BAUD_1200; i < UDR_BAUD_4800; i++)
								if (( UcharVal1 = RS232CheckOK( i)) == 0)
									break;
						WriteChar2( '>');
						break;

					case 4:							// RS232 comm. test 4: 4800-19200
						if ( UcharVal1)
							for ( i = UDR_BAUD_4800; i <= UDR_BAUD_19200; i++)
								if (( UcharVal1 = RS232CheckOK( i)) == 0)
									break;
						WriteChar2( '>');
						break;

					case 5:							// RS232 comm. test: result
						WriteCodeLN2( 0, TestingRS232CommTxt);	// Header text

						if ( UcharVal1)			// If NO errors in communication
							FuncState = 2;			//  display OK text
						else 							// otherwise..
							FuncState = 3;			//  display error text

						ClearRestLN2();
						break;
				}

				if ( IntVal < 5)
					IntVal++;
				else
					IntVal = 0;
			}
		}
		else {										// ..otherwise ask for connector
			FuncState = 4;

			WriteCodeLN2( 0, LoopbackConnectorTxt);
			ClearRestLN2();
		}

		WriteType11_OK_FAIL( FuncState);		// Write the OK or FAIL text
	}
}

/***************************************************************************/
/* DiagnoseDisplayTest															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971007				     													*/
/*																									*/
/*	Function:	Handle the display containing the display test					*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseDisplayTest(){

	if ( FuncEscape) {
		SelectModeOn = FALSE;			// Clear select-mode
		UpdateEnable = FALSE;			// Disable update function

		Flags = TmpFlags;					// Restore original auto escape function

		Ptr = PrevPtr;						// Set pointer to previous menu
		Ptr->DispFunct();					// Display previous menu
		return;
	}

	if ( UpdateSignal) {					// Pressing a key do NOT cause an update
		UpdateSignal = FALSE;

		if ( IntVal) {
			IntVal = FALSE;
																// Write 40 characters on line1
			WriteCodeLN1( 0, DiagnoseDisplayLN1Txt);
		}
		else {
			IntVal = TRUE;

			WriteCodeLN1( 0, SecondmnuHdrTxt);		// Write display headline
			WriteCode1( SlashDotHdrTxt);
			WriteCode1( config_menu[ConfigNdx].ItemTxt);
			WriteChar1( '/');
			WriteCode1( diagnose_menu[DiagnoseNdx].ItemTxt);
			ClearRestLN1();

			WriteArrowsField( Ptr->ItemConfig);		// Display keyboard-selection
		}

		WriteCodeLN2( 0, DiagnoseDisplayLN2Txt);	// Write 40 characters on line2
	}
}

/***************************************************************************/
/* DiagnoseKeyboardTest															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971016				     													*/
/*																									*/
/*	Function:	Handle the display containing the keyboard test					*/
/*	Remarks:		IntVal holds the timer value, ie. the arrows						*/
/*					UcharVal holds the button to be pressed							*/
/*					UcharVal1 holds the result of an button being pressed			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseKeyboardTest(){

	if ( FuncEscape) {
		SelectModeOn = FALSE;			// Clear select-mode
		UpdateEnable = FALSE;			// Disable update function

		Flags = TmpFlags;					// Restore original auto escape function

		Ptr = PrevPtr;						// Set pointer to previous menu
		Ptr->DispFunct();					// Display previous menu
		return;
	}

	if ( FuncSave) {						// If execute pressed in FuncState > 1..
		FuncSave = FALSE;					//
		FuncState = 1;						// Set state machine at state 1

		UpdateSignal = TRUE;				// Reinitialize update timer & signal
		UpdateTimer = UpdateFrequency;
	}

	switch ( Button_Command) {
		case LEFT_Button:
			if ( UcharVal == 0x01)
				UcharVal1 |= UcharVal;
			break;

		case DOWN_Button:
			if ( UcharVal == 0x02)
				UcharVal1 |= UcharVal;
			break;

		case RIGHT_Button:
			if ( UcharVal == 0x04) 
				UcharVal1 |= UcharVal;
			break;

		case EXEC_Button:
			if ( UcharVal == 0x08) 
				UcharVal1 |= UcharVal;
			break;

		case PRESET_Button:
			if ( UcharVal == 0x10)
				UcharVal1 |= UcharVal;
			break;

		case OUTPUT_Button:
			if ( UcharVal == 0x20)
				UcharVal1 |= UcharVal;
			break;

		case GENLOCK_Button:
			if ( UcharVal == 0x40)
				UcharVal1 |= UcharVal;
			break;
	}
										// If update signal or correct button pressed..
	if ( UpdateSignal || ( UcharVal & UcharVal1)) {
		UpdateSignal = FALSE;

		if ( FuncState == 1) {
			switch( IntVal) {
				case 0:									// Button test: setup
					WriteCodeLN2( 0, PressButtonHdrTxt);
					WriteCode2( ButtonArrTxt[BitPos( UcharVal)-1]);
					ClearRestLN2();

					WriteCharLN2( 35, '>');
					break;

				default:									// DO NOT MOVE THIS CASE!!!!!
					if ( UcharVal & UcharVal1)		// If button pressed..
						IntVal = 5;						//  immediately leave test mode
					else {								// otherwise..
						WriteChar2( '>');				//  display next arrow
						break;
					}		
															// break IS missing!!
				case 5:
					if ( UcharVal & UcharVal1)
						WriteCodeLN2( 35, OKTxt);
					else
						WriteCodeLN2( 35, FAILTxt);

					ClearRestLN2();
					break;

				case 6:
					UcharVal <<= 1;					// Prepare for next button

					if ( UcharVal > 0x40) {			// If no mnore buttons..
						if ( UcharVal1 - 0x7F)		//  If not all buttons pressed.
							FuncState = 3;          //   display FAIL text..
						else								//  otherwise..
							FuncState = 2;				//   display OK text

						WriteCodeLN2( 0, TestingButtonsCompletedTxt);
						ClearRestLN2();

						UcharVal = 1;					// Prepare for button 1
						UcharVal1 = 0;					// Reset buttons pressed
					}
					break;
			}

			if ( IntVal < 6)
				IntVal++;
			else
				IntVal = 0;
		}
		WriteType11_OK_FAIL( FuncState);			// Write the OK or FAIL text
	}
}

/***************************************************************************/
/* DiagnoseMemoryTest															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971016				     													*/
/*																									*/
/*	Function:	Handle the display containing the mainboard memory test		*/
/*	Remarks:		IntVal holds the timer value, ie. the arrows						*/
/*					UcharVal holds the result of the PROM check						*/
/*					UcharVal1 holds the result of the RAM check						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseMemoryTest(){

	UI CSum;

	if ( FuncEscape) {
		SelectModeOn = FALSE;			// Clear select-mode
		UpdateEnable = FALSE;			// Disable update function

		Flags = TmpFlags;					// Restore original auto escape function

		Ptr = PrevPtr;						// Set pointer to previous menu
		Ptr->DispFunct();					// Display previous menu
		return;
	}

	if ( FuncSave) {						// If execute pressed in FuncState > 1..
		FuncSave = FALSE;					//
		FuncState = 1;						// Set state machine at state 1

		UpdateSignal = TRUE;				// Reinitialize update timer & signal
		UpdateTimer = UpdateFrequency;
	}

	if ( UpdateSignal) {					// Pressing a key do NOT cause an update
		UpdateSignal = FALSE;

		if ( FuncState == 1) {
			switch( IntVal) {
				case 0:									// ROM memory test: setup
					WriteCodeLN2( 0, TestingMemoryHdrTxt);	// Header text ROM test
					WriteCode2( ROMTxt);
					ClearRestLN2();

					WriteCharLN2( 35, '>');
					break;

				case 1:									// ROM memory test: ROM test
					UcharVal = ROMCheckOK( &CSum);
					sprintf( TextBuffer, "%04X", CSum);

					WriteChar2( '>');
					break;

				case 5:									// ROM memory test: Result
					if ( UcharVal) {					// If NO errors in ROM
						FuncState = 2;					//  display OK text
						WriteTxtLN2( 22, "CSUM:");
						WriteTxt2( TextBuffer);
					}
					else {								// otherwise..
						WriteErrorLN2( 0, ROMMemoryErrorType + ChecksumError);
						FuncState = 4;					//  display error text
					}

					ClearRestLN2();
					break;

				case 6:									// RAM memory test: setup
					WriteCodeLN2( 0, TestingMemoryHdrTxt);	// Header text RAM test
					WriteCode2( RAMTxt);
					ClearRestLN2();

					WriteCharLN2( 35, '>');
					break;

				case 7:									// RAM memory test: RAM test
					UcharVal1 = RAMCheckOK();

					WriteChar2( '>');
					break;

				case 11:									// RAM memory test: result
					if ( UcharVal1)					// If NO errors in RAM
						FuncState = 2;					//  display OK text
					else {								// otherwise..
						WriteErrorLN2( 0, RAMMemoryErrorType);
						FuncState = 4;					//  display General Failure text
					}

					ClearRestLN2();
					break;

				case 12:									// ROM/RAM memory test: result
					if ( UcharVal && UcharVal1)	// If NO errors in ROM or RAM..
						FuncState = 2;					//  display OK text..
					else									// otherwise..
						FuncState = 3;					//  display FAIL text

					WriteCodeLN2( 0, TestingMemoryCompletedTxt);
					ClearRestLN2();
					break;

				default:
					WriteChar2( '>');
					break;
			}

			if ( IntVal < 12)
				IntVal++;
			else
				IntVal = 0;
		}
		WriteType11_OK_FAIL( FuncState);			// Write the OK or FAIL text
	}
}

/***************************************************************************/
/* DiagnoseConfigTest															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980106				     													*/
/*																									*/
/*	Function:	Display the configuration in menu: DIAGNOSE/CONFIGUATION		*/
/*	Remarks:		Only options detected during power-up will be displayed		*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseConfigTest(){

	if ( FuncEscape) {

		Flags = TmpFlags;							// Restore flags

		SelectModeOn = FALSE;					// Clear select-mode

		Ptr = PrevPtr;								// Set pointer to previous menu
		Ptr->DispFunct();							// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {			// If button pressed is UP..
			do {
				if ( UcharVal++ == TimeConn)
					UcharVal = MainConn;
			} while ( !Connectors[UcharVal].Present);
		}
		else {										// ..button pressed is DOWN
			do {
				if( UcharVal-- == MainConn)
					UcharVal = TimeConn;
			} while ( !Connectors[UcharVal].Present);
		}
	}

	Cstrcpy( TextBuffer, ConnectorArrTxt[UcharVal]);

	switch ( UcharVal) {
		case BB78Conn:
			if ( Connectors[UcharVal].HWType == PT8601) {
				TextBuffer[1] = 'A';
				TextBuffer[2] = 'N';
				TextBuffer[3] = 'L';
				TextBuffer[4] = 'S';
				break;
			}											// Break IS missing!!

		case BB34Conn:
			if ( Connectors[UcharVal].HWType == PT8604) {
				TextBuffer[3] = 'M';
				TextBuffer[4] = 'U';
				break;
			}											// Break IS missing!!

		case BB56Conn:
			if ( Connectors[UcharVal].HWType == PT8609)
				TextBuffer[1] = 'S';
			break;
	}

	WriteTxtLN2( 0, TextBuffer);
	WriteTxt2( ": ");

	switch ( UcharVal) {
		case MainConn:
			strcpy( TextBuffer, "PT5210/906");
			break;

		case OSCConn:
		case BB12Conn:
			Cstrcpy( TextBuffer, HyphenTxt);
			break;

		default:
			sprintf( TextBuffer, "PT86%02d", Connectors[UcharVal].HWType);
			break;
	}

	WriteTxt2( TextBuffer);

	if ( Connectors[UcharVal].HWVersion) {
		sprintf( TextBuffer, "/9%02d", Connectors[UcharVal].HWVersion);
		WriteTxt2( TextBuffer);
	}

	sprintf( TextBuffer, ", KU%06.0f, ", ( float) Connectors[UcharVal].KUNumber);
	WriteTxt2( TextBuffer);

	if ( Connectors[UcharVal].SWVersion)
		sprintf( TextBuffer, "%04.1f", ( float) Connectors[UcharVal].SWVersion/10.0);
	else
		strcpy( TextBuffer, "NA");

	WriteTxt2( TextBuffer);

	ClearRestLN2();								// Clear rest of line 2

	WriteType5_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	FuncTriggers = 0;								// Reset the button-selection
}

/***************************************************************************/
/* DiagnoseErrorQueueTest														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		971020				     													*/
/*																									*/
/*	Function:	Handle the display containing the error queue buffer			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseErrorQueueTest(){

	if ( FuncSave || FuncEscape) {

		if ( FuncSave) {
			WriteAddressLN1( 31);				// Clear the square brackets with
			ClearRestLN1();						//  the error queue number

			FuncState = 2;							// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			Ptr = &reset_menu;					// DO NOT ALTER PrevPtr!!!!!!!!!!!!
			Ptr->DispFunct();						// Display reset menu
			return;
		}

		Flags = TmpFlags;

		SelectModeOn = FALSE;					// Clear select-mode

		Ptr = PrevPtr;								// Set pointer to previous menu
		Ptr->DispFunct();							// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {			// If button pressed is LEFT..
			if( UcharVal++ == NoInErrorQueue-1)
				UcharVal = 0;
		}
		else {										// ..button pressed is RIGHT
			if( UcharVal-- == 0)
				UcharVal = NoInErrorQueue-1;
		}
	}

	WriteTxtLN1( 31, "[ ]");
	WriteCharLN1( 32, '1' + UcharVal);

	if ( ErrorQueue[UcharVal])
		WriteErrorLN2( 0, ErrorQueue[UcharVal]);
	else
		WriteCodeLN2( 0, NoErrorDetectedTxt);

	ClearRestLN2();								// Clear rest of line 2

	WriteType5_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	if ( FuncState == 1) {
		WriteCGChar( 3, 0x04);					// Execute Icon
		WriteCharLN1( 39, 3);					// EXECUTE
	}

	FuncTriggers = 0;								// Reset the button-selection
}

/***************************************************************************/
/* DiagnoseErrorQueueReset														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970306	    										*/
/* Revised:		971016				     													*/
/*																									*/
/*	Function:	Handle the display to reset the error queue						*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseErrorQueueReset(){

	UC i;

	if ( FuncSave || FuncEscape) {

		if ( FuncSave) {
			for( i = 0 ; i < NoInErrorQueue; i++)
				ErrorQueue[i] = 0;

			ErrorStatus = 0;
			ErrorQueuePtr = NoInErrorQueue-1;

			Warning_LED = OFF;
			Update_LEDs();
		}

		Ptr = &diagnose_submenu[DiagErrorQueueItem];

		FuncState = 1;								// Prepare for item state-machine
		FuncTriggers = 0;
		FuncSave = FuncEscape = FALSE;

		Ptr->DispFunct();							// Display previous menu
		return;
	}

	WriteCodeLN2( 0, ResetErrorQueueTxt);
	ClearRestLN2();

	WriteType10_OK_ESC( FuncState);			// Write the SAVE ESC text

	FuncTriggers = 0;								// Reset the button-selection
}

/***************************************************************************/
/* AnalogTimeAdjust																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960626				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void AnalogTimeAdjust( UC unit, UC System, long Delay) {

	int line;
	char field, sign;
	float time;
	int lineTst;

														// Time, ie. T, adjustement..
	if ( FuncTriggers & 0x30) {				// If button pressed is UP/DOWN..
		if ( FuncTriggers & 0x10) 				// If button pressed is UP ..
			samples += stepVal;
		else
			samples -= stepVal;
	}

	if ( System < NTSC) {						// If system is PAL..
														// Line, ie. H, adjstement..
		if ( FuncTriggers & 0x0C) {			// If button pressed is UP/DOWN..
			if ( FuncTriggers & 0x04) 			// If button pressed is UP..
				samples += (long) 1728*256;
			else
				samples -= (long) 1728*256;
		}
														// Field, ie. V, adjustement ..
		if ( FuncTriggers & 0x03) {			// If button pressed is UP/DOWN..

			lineTst = ( samples / ((long) 1728*256)) % 625;

			if ( FuncTriggers & 0x01) { 		// If button pressed is UP..
				if ( lineTst < 313)
					samples += (long) 313*1728*256;
				else
					samples += (long) 312*1728*256;
			}
			else {
				if ( lineTst < 313)
					samples -= (long) 312*1728*256;
				else
					samples -= (long) 313*1728*256;
			}
		}

		if ( samples < 0)
			samples += (long) 4*625*1728*256;
		else
			samples %= (long) 4*625*1728*256;
	}
	else {											// otherwise system is NTSC
														// Line, ie. H, adjustement..
		if ( FuncTriggers & 0x0C) {			// If button pressed is UP/DOWN..
			if ( FuncTriggers & 0x04) 			// If button pressed is UP..
				samples += (long) 1716*256;
			else
				samples -= (long) 1716*256;
		}
														// Field, ie. V, adjustement..
		if ( FuncTriggers & 0x03) {			// If button pressed is UP/DOWN..

			lineTst = ( samples / ((long) 1716*256)) % 525;

			if ( FuncTriggers & 0x01) { 		// If button pressed is UP..
				if ( lineTst < 263)
					samples += (long) 263*1716*256;
				else
					samples += (long) 262*1716*256;
			}
			else {
				if ( lineTst < 263)
					samples -= (long) 262*1716*256;
				else
					samples -= (long) 263*1716*256;
			}
		}
		if ( samples < 0)
			samples += (long) 2*525*1716*256;
		else
			samples %= (long) 2*525*1716*256;

	}
														// If time ie T, is being updated..
	if ( FuncTriggers & 0xC0) {				// If button pressed is RIGHT/LEFT..
		if ( FuncTriggers & 0x40) {			// If button pressed is LEFT..
			if ( stepNdx++ == stepNdxAnalogMax)
				stepNdx = stepNdxAnalogMin;
		}
		else { 										// ..button pressed is RIGHT
			if ( stepNdx-- == stepNdxAnalogMin)
				stepNdx = stepNdxAnalogMax;
		}
		stepVal = stepsizeAnalogTable[stepNdx];	// Get step size from index
	}

	if ( FuncTriggers & 0x3F) {
		if ( Delay == samples) {
			if ( FuncState == 11)
				FuncState--;
			else
				FuncState -= 4;
		}

		switch ( unit) {
			case 0:
				TrxAnlBlkDelay( AnlBlkNdx, samples);
				break;

			case 1:
				TrxAnalogDelay( samples);
				break;

			case 2:
				if (( ActiveGenlockNo == UcharVal) &&
					 ( GenlockConfig[ActiveGenlockNo].System == UcharVal1))

					if ( ErrorCode = TrxGenlockDelay( samples)) {
						GotoErrorStatus( ErrorCode);
						return;
					}
				break;

			default:
				break;
		}
	}

	AnalogSamplesToFLT( System, samples, &sign, &field, &line, &time);
	WriteTimingFLT( sign, field, line, time);

	WriteType6_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 5:
			WriteItemArrows( 3, 2);				// Highlight selection
			break;

		case 2:
		case 6:
			WriteItemArrows( 10, 4);			// Highlight selection
			break;

		case 3:
		case 7:
			WriteItemArrows( 19, 8);			// Highlight selection
			CharBlinkLN2( 0, OFF);				// Switch on blinking at
														// Reset step index and stepVal
			stepVal = stepsizeAnalogTable[stepNdx = stepNdxAnalogMin];
			break;

		case 10:
		case 11:
			WriteItemBrackets( 19, 8);				// Highlight selection

			if ( stepNdx == stepNdxAnalogMin)
				CharBlinkLN2( 26-stepNdx, ON);	// Switch on blinking
			else
				CharBlinkLN2( 25-stepNdx, ON);
			break;
	}
}

/***************************************************************************/
/* SDITimeAdjust																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960626				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		Nothing																		*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void SDITimeAdjust( UC unit, UC System, long Delay) {

	int line;
	char field, sign;
	float time;
	int lineTst;
														// Time, ie. T, adjustement..
	if ( FuncTriggers & 0x30) {				// If button pressed is UP/DOWN..
		if ( FuncTriggers & 0x10) 				// If button pressed is UP ..
			samples += stepVal;
		else
			samples -= stepVal;
	}

	if ( System == SDI625) {					// If system is SDI625..
														// Line, ie. H, adjstement..
		if ( FuncTriggers & 0x0C) {			// If button pressed is UP/DOWN..
			if ( FuncTriggers & 0x04) 			// If button pressed is UP..
				samples += (long) 1728*256;
			else
				samples -= (long) 1728*256;
		}
														// Field, ie. V, adjustement ..
		if ( FuncTriggers & 0x03) {			// If button pressed is UP/DOWN..

			lineTst = ( samples / ((long) 1728*256)) % 625;
			if ( FuncTriggers & 0x01) { 		// If button pressed is UP..

				if ( lineTst < 312)
					samples += (long) 312*1728*256;
				else
					samples += (long) 313*1728*256;
			}
			else {
				if ( lineTst < 312)
					samples -= (long) 313*1728*256;
				else
					samples -= (long) 312*1728*256;
			}
		}

		if ( samples < 0)
			samples += (long) 625*1728*256;
		else
			samples %= (long) 625*1728*256;
	}
	else {											// otherwise system is SDI525
														// Line, ie. H, adjustement..
		if ( FuncTriggers & 0x0C) {			// If button pressed is UP/DOWN..
			if ( FuncTriggers & 0x04) 			// If button pressed is UP..
				samples += (long) 1716*256;
			else
				samples -= (long) 1716*256;
		}
														// Field, ie. V, adjustement..
		if ( FuncTriggers & 0x03) {			// If button pressed is UP/DOWN..

			lineTst = ( samples / ((long) 1716*256)) % 525;

			if ( FuncTriggers & 0x01) { 		// If button pressed is UP..
				if ( lineTst < 262)
					samples += (long) 262*1716*256;
				else
					samples += (long) 263*1716*256;
			}
			else {
				if ( lineTst < 262)
					samples -= (long) 263*1716*256;
				else
					samples -= (long) 262*1716*256;
			}
		}
		if ( samples < 0)
			samples += (long) 525*1716*256;
		else
			samples %= (long) 525*1716*256;

	}
														// If time ie T, is being updated..
	if ( FuncTriggers & 0xC0) {				// If button pressed is RIGHT/LEFT..
		if ( FuncTriggers & 0x40) {			// If button pressed is LEFT..
			if ( stepNdx++ == stepNdxSDIMax)
				stepNdx = stepNdxSDIMin;
		}
		else { 												// ..button pressed is RIGHT
			if ( stepNdx-- == stepNdxSDIMin)
				stepNdx = stepNdxSDIMax;
		}
		stepVal = stepsizeSDITable[stepNdx];		// Get step size from index
	}

	if ( FuncTriggers & 0x3F) {
		if ( Delay == samples) {
			if ( FuncState == 11)
				FuncState--;
			else
				FuncState -= 4;
		}

		switch ( unit) {
			case 0:
				TrxSDIBlkDelay( SDIBlkNdx, samples);
				break;

			case 1:
				TrxSDIDelay( samples);
				break;

			case 2:
				if (( ActiveGenlockNo == UcharVal) &&
					 ( GenlockConfig[ActiveGenlockNo].System == UcharVal1))

					if ( ErrorCode = TrxGenlockDelay( samples)) {
						GotoErrorStatus( ErrorCode);
						return;
					}
				break;

			default:
				break;
		}
	}

	SDISamplesToFLT( System, samples, &sign, &field, &line, &time);
	WriteTimingFLT( sign, field, line, time);

	WriteType6_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 5:
			WriteItemArrows( 3, 2);				// Highlight selection
			break;

		case 2:
		case 6:
			WriteItemArrows( 10, 4);			// Highlight selection
			break;

		case 3:
		case 7:
			WriteItemArrows( 19, 8);			// Highlight selection
			CharBlinkLN2( 0, OFF);				// Switch on blinking at
														// Reset step index and stepVal
			stepVal = stepsizeSDITable[stepNdx = stepNdxSDIMin];
			break;

		case 10:
		case 11:
			WriteItemBrackets( 19, 8);				// Highlight selection

			CharBlinkLN2( 23-stepNdx, ON);
			break;
	}
}
