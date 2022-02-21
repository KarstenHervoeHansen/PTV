/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		MENU.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980119																		*/
/*	Rev. date:	000608, KEn, DEV															*/
/*	Status:		Version 1.7																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*																									*/
/*	Changes:																						*/
/* 000508: Added support for text positioning										*/
/* 991028: Error in DownloadFromSPG() corrected.									*/
/* 991025: Error in DiagnoseErrorQueueReSET(): LevelStatus was not reset	*/
/*				to enable a static error to be displayed again.						*/
/* 990420: Update of AnlTPGPatternSelect() to support MODIFY of PHILIPS		*/
/*				patterns in NTSC.																*/
/* 990308: Implemented ID of PT8632/10 in DiagnoseConfig()						*/
/* 990305: SDITPGPatternModify(): attrib is now checked in tables.c			*/
/*			  In ConfigLockSelect(): Header on line was SELECT: it should be	*/
/*				LOCK:																				*/
/*			  Text editing bug corrected in Anl/SDITPGText & PresetName			*/
/* 990305: AnlTPGPatternModify(): attrib is now checked in tables.c			*/
/* 981126: Error in cetain menus when auto escape is enabled and goes into	*/
/*				action.																			*/
/* 980929: xxTPGTextMenuShow(): Do NOT display CLOCK if PT8601/02/03			*/
/* 980918: StatusDateTimeShow() updated.												*/
/* 980917: AnlTPGPattsernSelect() & AnlTPGPatternModify() updated to handle*/
/*				new complex signals.															*/
/* 980916: New state machine for DateTimeDateSelect(), DateTimeTimeSelect()*/
/* 980915: Added in StatusGenlockShow(): New status for burstlock.			*/
/* 980825: Status??Update changed to Status??Show									*/
/* 980723: Updated StatusDateTimeUpdate()												*/
/* 980708: Updated Time Clock menu functions											*/
/* 980616: Released as 4008 002 06812													*/
/* 980523: Limited no of characters in ...TPGTextEditSelect() to 8 for the	*/
/*				PT860x series and to 16 for the PT863x series.						*/
/*			  Redesigned GroupButtonsSelect().											*/
/* 980521: BB options was not tested.													*/
/* 980519: In Anl/SDI-TPGTextEditSelect(): SAVE did not disappear when 		*/
/*				toggling the text line ON/OFF												*/
/* 980518: In GroupButtonSelect(): When using buttons in AnlTPG pattern		*/
/*				select menu, the display was not updated.								*/
/* 980515: Released as 4008 002 06811													*/
/* 980515: In AESEBUnMenuShow(): The items texts was not shown for PT8635.	*/
/*			  The Hi-TECH compiler can NOT handle the construction :				*/
/*					for ( i = A; i <= B + ( HWtype == PT8635)							*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "mas.h"

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
#include "iic.h"
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

// Temporary variables PUBLIC

near UC StatusNdx;							 // Last active display item
near UC BaseNdx;								 //     -"-     base-menu item
near UC AnlBlkNdx;							 //     -"-     analog black menu item
near UC BBNdx[MaxAnlBlkUnits];			 //     -"-     black-burst menu item
near UC AnlTPGNdx[MaxAnlTPGUnits];		 //     -"-     analog TPG menu item
near UC AnlTPGTextNdx[MaxAnlTPGUnits];	 //     -"-     analog-TPG text 2ndmenu item
near UC SDITSGNdx[MaxSDITSGUnits];		 //     -"-     SDI-TSG menu item
near UC SDITSGAudioNdx[MaxSDITSGUnits]; //     -"-     SDI-TSG audio 2ndmenu item
near UC SDITPGNdx[MaxSDITPGUnits];		 //     -"-     SDI-TPG menu item
near UC SDITPGTextNdx[MaxSDITPGUnits];	 //     -"-     SDI-TPG text 2ndmenu item
near UC SDITPGAudioNdx[MaxSDITPGUnits]; //     -"-     SDI-TPG audio 2ndmenu item
near UC AESEBUnNdx;	  						 //     -"-     AES-EBU out item
near UC AESEBUNdx[MaxAESEBUUnits];		 //     -"-     audio item
near UC PresetNdx;			  				 //     -"-     preset item
near UC ConfigNdx;	  						 //     -"-     config item
near UC DateTimeNdx;	 						 //     -"-     date/time item
near UC RS232Ndx;	  							 //     -"-     RS232 item
near UC DiagnoseNdx; 						 //     -"-     diagnose item


// Temporary variables used specific in the download functions

UL ResponseKUNo;
UC ResponseSWVersion;


// Temporary variables PRIVATE NOT overlayable

static char TextBuffer[17];			// Temp. text buffers

static UC ErrorCode;						// Temporary error code, 8 BIT
												//  ErrorType + ErrorNo

static near UC UcharVal;				// Temp. unsigned char value
static near UC UcharVal1;				//  do.
static near UC UcharVal2;				//  do.
static near UC UcharVal3;				//  do.

static near UI UintVal;					// Temp. unsigned integer value

static near int IntVal;					// Temp. integer value

static near UL UlongVal;				// Temp. unsigned long value

static UC* near UcharPtr;				// Temp. pointer to unsigned char

static code char* near CodePtr;		// Temp. pointer to text arrays in CODE
static char* near TxtPtr;				// Temp. pointer to text arrays in DATA

static STATUS_FLAGS TmpFlags;


// Temporary variables used specific in the timing functions

static near long samples;
static near long stepVal;
static UC stepNdx;


/***************************************************************************/
/*	GroupButtonSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980206	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handles the front plate group button key's to the pattern	*/
/*					menus in the TSG/TPG's.													*/
/*	Remarks:		The buttons only work in the status display and in the		*/
/*					pattern selection menus.												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void GroupButtonSelect( UC group) {

	UC ndx, HWtype, system, pattern;

	if ( Flags.PanelLockOn) 					// If PANEL LOCK is enabled..
		return;										//  return immediately

														// If status display or select mode.
	if ( !MenuModeOn || ( MenuModeOn && SelectModeOn)) {

		if ( !MenuModeOn)
			ndx = Ptr->ItemNumber;
		else
			ndx = BaseNdx+1;						// Transform BaseNdx to StatusNdx

		switch ( ndx) {
			case StatusAnlTPG2Item:
			case StatusAnlTPG5Item:
				if ( !MenuModeOn || ( Ptr == &AnlTPG_submenu[AnlTPGPatternItem])) {

					ndx -= StatusAnlTPG2Item;

					HWtype = AnlTPGUnit[ndx].HWType;

					system = AnlTPGConfig[ndx].System;
					pattern = AnlTPGConfig[ndx].Pattern;

															// If group already selected..
															//  find new pattern and group
					if ( group == AnlFindGroup( pattern))
						pattern = AnlNextGroupPattern( HWtype, ndx, system, group, pattern);
					else {
															// Validate pattern in group
						pattern = AnlValidateGroupPattern( HWtype, ndx, system, group,\
															AnlTPGConfig[ndx].Memory[group]);

						if ( pattern == NoPattern)	// If no pattern in group
							break;						//  return immediately
					}

					UpdateGrpLEDs( group);

					AnlTPGConfig[ndx].Memory[group] = pattern;
					AnlTPGConfig[ndx].Pattern = pattern;

					ErrorCode = TrxAnlTPGPattern( ndx, pattern, AnlTPGAttrib( ndx, pattern));

					if ( !ErrorCode)
						ErrorCode = ConfigureAnlTPGText( ndx, pattern);

					if ( ErrorCode)
						GotoErrorStatus( 256*ndx + ErrorCode);
					else {
						if ( Ptr == &AnlTPG_submenu[AnlTPGPatternItem]) {
							UcharVal = pattern;

							if ( AnlFindPatternType( pattern) == ComplexPattern)
								FuncState = 6;
							else
								FuncState = 1;
						}
					}
				}
				break;

			case StatusSDITSG2Item:
			case StatusSDITSG3Item:
			case StatusSDITSG4Item:
				if ( !MenuModeOn || ( Ptr == &SDITSG_submenu[SDITSGPatternItem])) {

					ndx -= StatusSDITSG2Item;

					HWtype = SDITSGUnit[ndx].HWType;

					system = SDITSGConfig[ndx].System;
					pattern = SDITSGConfig[ndx].Pattern;

															// If group already selected..
															//  find new pattern and group
					if ( group == SDIFindGroup( pattern))
						pattern = SDINextGroupPattern( HWtype, ndx, system, group, pattern);
					else {
															// Validate pattern in group
						pattern = SDIValidateGroupPattern( HWtype, ndx, system, group,\
															SDITSGConfig[ndx].Memory[group]);

						if ( pattern == NoPattern)	// If no pattern in group
							break;						//  return immediately
					}

					UpdateGrpLEDs( group);

					SDITSGConfig[ndx].Memory[group] = pattern;
					SDITSGConfig[ndx].Pattern = pattern;

					if ( ErrorCode = TrxSDITSGPattern( ndx, pattern))
						GotoErrorStatus( 256*ndx + ErrorCode);
					else {
						if (  Ptr == &SDITSG_submenu[SDITSGPatternItem]) {
							UcharVal = pattern;

							FuncState = 1;
						}
					}
				}
				break;

			case StatusSDITPG1Item:
			case StatusSDITPG2Item:
			case StatusSDITPG5Item:
				if ( !MenuModeOn || ( Ptr == &SDITPG_submenu[SDITPGPatternItem])) {

					ndx -= StatusSDITPG1Item;

					HWtype = SDITPGUnit[ndx].HWType;

					system = SDITPGConfig[ndx].System;
					pattern = SDITPGConfig[ndx].Pattern;

															// If group already selected..
															//  find new pattern and group
					if ( group == SDIFindGroup( pattern))
						pattern = SDINextGroupPattern( HWtype, ndx, system, group, pattern);
					else {
															// Validate pattern in group
						pattern = SDIValidateGroupPattern( HWtype, ndx, system, group,\
															SDITPGConfig[ndx].Memory[group]);

						if ( pattern == NoPattern)	// If no pattern in group
							break;						//  return immediately
					}

					UpdateGrpLEDs( group);

					SDITPGConfig[ndx].Memory[group] = pattern;
					SDITPGConfig[ndx].Pattern = pattern;

					ErrorCode = TrxSDITPGPattern( ndx, pattern, SDITPGAttrib( ndx, pattern));

					if ( !ErrorCode)
						ErrorCode = ConfigureSDITPGText( ndx, pattern);

					if ( ErrorCode)
						GotoErrorStatus( 256*ndx + ErrorCode);
					else {
						if (  Ptr == &SDITPG_submenu[SDITPGPatternItem]) {
							UcharVal = pattern;

							if ( SDIFindPatternType( pattern) == ComplexPattern)
								FuncState = 6;
							else
								FuncState = 1;
						}
					}
				}
				break;
		}
	}
}

/***************************************************************************/
/*	PresetButtonSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980129	    										*/
/* Revised:		980503				     													*/
/*																									*/
/*	Function:	Handles the front plate PRESET button which is a shortcut	*/
/*					key to the PRESET/RECALL menu.										*/
/*	Remarks:		If NOT in menu-mode, the PRESET button will activate the		*/
/*					PRESET RECALL menu. When in the PRESET RECALL menu, pressing*/
/*					the PRESET button will function as the UP button.				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, BaseNdx, PresetNdx, MenuModeOn, SelectModeOn				*/
/***************************************************************************/
void PresetButtonSelect() {

	if ( Flags.PanelLockOn) 					// If PANEL LOCK is enabled..
		return;										//  return immediately

	if ( MenuModeOn && ( Ptr == &preset_submenu[PresetRecallItem]))
		Ptr->UpBut();
	else {
		if ( MenuModeOn && SelectModeOn) {	// If in selection mode ..
			Ptr->EscapeFct();						// Be sure to terminate NICELY
			Ptr->DispFunct();
		}

		if ( !MenuModeOn)							// If in a status display..
			UpdateGrpLEDs( NoGroup);			// Switch OFF all group LEDs,

		MenuModeOn = SelectModeOn = ON;		// Set menu- & select-mode

		BaseNdx = PresetItem;					// Select PRESET MENU

		PresetNdx = PresetRecallItem;			// Select PRESET RECALL SUBMENU
		Ptr = &preset_menu[PresetNdx];		//  and upadte pointer to it

		PresetDown();
	}
}

/***************************************************************************/
/*	OutputButtonSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980129	    										*/
/* Revised:		980503				     													*/
/*																									*/
/*	Function:	Handles the front plate OUTPUT button which is a shortcut 	*/
/*					key to the ../PATTERN selectinos in ANALOG-TPGx, SDI-TSGx &	*/
/*					SDI-TPGx.																	*/
/*	Remarks:		This button has NO function if no options are installed		*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void OutputButtonSelect() {

	UC tmp;

	if ( Flags.PanelLockOn) 					// If PANEL LOCK is enabled..
		return;										//  return immediately

														// If a option is installed..
	if ( FindNextTSGTPG( ActiveOutputNo) > AnlBlkItem) {

		if ( MenuModeOn && SelectModeOn) {	// If in selection mode ..
			Ptr->EscapeFct();						//  be sure to terminate NICELY
			Ptr->DispFunct();
		}

		MenuModeOn = SelectModeOn = ON;		// Set menu- & select-mode

		BaseNdx = ActiveOutputNo = FindNextTSGTPG( ActiveOutputNo);

		switch ( BaseNdx) {
			case AnlTPG2Item:									// Analog-TPG2
			case AnlTPG5Item:									// Analog-TPG5
				tmp = ActiveOutputNo - AnlTPG2Item;

				AnlTPGNdx[tmp] = AnlTPGPatternItem;		// Select ANALOG-TPGx
				Ptr = &AnlTPG_menu[AnlTPGPatternItem];//  PATTERN SUBMENU

				AnlTPGDown();
				break;

			case SDITSG2Item:									// SDI-TSG2
			case SDITSG3Item:									// SDI-TSG3
			case SDITSG4Item:									// SDI-TSG4
				tmp = ActiveOutputNo - SDITSG2Item;

				SDITSGNdx[tmp] = SDITSGPatternItem;		// Select SDI-TSGx
				Ptr = &SDITSG_menu[SDITSGPatternItem];	//  PATTERN SUBMENU

				SDITSGDown();
				break;

			case SDITPG1Item:									// SDI-TPG1
			case SDITPG2Item:									// SDI-TPG2
			case SDITPG5Item:									// SDI-TPG5
				tmp = ActiveOutputNo - SDITPG1Item;

				SDITPGNdx[tmp] = SDITPGPatternItem;		// Select SDI-TPGx
				Ptr = &SDITPG_menu[SDITPGPatternItem];	//  PATTERN SUBMENU

				SDITPGDown();
				break;
		}
	}
}

/***************************************************************************/
/*	StatusPresetShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980825				     													*/
/*																									*/
/*	Function:	Display the name and number of a selected preset.				*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusPresetShow() {

	WriteCodeLN1( 0, StatusPresetTxt);			// Write line 1 text
	ClearRestLN1();									// Clear rest of line 1

	if ( ActivePresetNo) {
		WriteCodeLN2( 0, PresetHdrTxt);
		WriteCharLN2( 6, ActivePresetNo + '0');

		WriteTxtLN2( 9, Preset[ActivePresetNo-1].Name);
	}
	else
		WriteCodeLN2( 0, NoPresetActiveTxt);

	ClearRestLN2();									// Clear rest of line 2

	UpdateGrpLEDs( NoGroup);						// Switch OFF all group LEDs

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusGenlockShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980915				     													*/
/*																									*/
/*	Function:	Display current genlock selection and status.					*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusGenlockShow() {

	WriteCodeLN1( 0, GenlockHdrTxt);
	WriteCode1( GenlockInputArrTxt[ActiveGenlockNo]);
	ClearRestLN1();									// Clear rest on line 1

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

		if ( UnlockedLED)
			WriteCode2( UNLOCKEDTxt);
		else {
			if ( GenlockConfig[ActiveGenlockNo].System < SyncLock625) {
				if ( burstLockOn)
					WriteCode2( GENLOCKEDTxt);
				else
					WriteCode2( SYNCLOCKEDTxt);
			}
			else
				WriteCode2( GENLOCKEDTxt);
		}
	}

	ClearRestLN2();									// Clear rest of line 2

	UpdateGrpLEDs( NoGroup);						// Switch OFF all group LEDs

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusAnlTPGShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980825				     													*/
/*																									*/
/*	Function:	Display current Analog-TPGx selection.								*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusAnlTPGShow() {

	UC patterntype, textenable, ndx = Ptr->ItemNumber - StatusAnlTPG2Item;

	WriteCodeLN1( 0, AnlTPGHdrArrTxt[ndx]);
	WriteCode1( AnlPattArrTxt[AnlTPGConfig[ndx].Pattern]);

	ClearRestLN1();									// Clear rest of line 1

	WriteCodeLN2( 0, SystemHdrTxt);				// Write System information
	WriteCode2( AnlSystemArrTxt[AnlTPGConfig[ndx].System]);

	ClearToPosLN2( 20);								// Write text information
	patterntype = AnlFindPatternType( AnlTPGConfig[ndx].Pattern);
	textenable = AnlTPGConfig[ndx].TextEnable;

	if ( patterntype == ComplexPattern)
		textenable &= 0x18;
	else
		textenable &= 0x07;

	WriteCode2( MinusPlusArrTxt[( UC) ( textenable || (AnlTPGConfig[ndx].TextStyle[patterntype] & 0xF0))]);
	WriteCode2( TextTxt);

	ClearRestLN2();									// Clear rest of line 2

															// Update group LEDs
	UpdateGrpLEDs( AnlFindGroup( AnlTPGConfig[ndx].Pattern));

	WriteArrowsField( Ptr->ItemConfig);			// Write line 1&2 icons/dots
}

/***************************************************************************/
/*	StatusSDITSGShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980825				     													*/
/*																									*/
/*	Function:	Display current SDI-TSGx selection.									*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusSDITSGShow() {

	UC ndx = Ptr->ItemNumber - StatusSDITSG2Item;

	WriteCodeLN1( 0, SDITSGHdrArrTxt[ndx]);
	WriteCode1( SDIPattArrTxt[SDITSGConfig[ndx].Pattern]);

	ClearRestLN1();									// Clear rest of line 1

	WriteCodeLN2( 0, SystemHdrTxt);				// Write system information
	WriteCode2( SDISystemArrTxt[SDITSGConfig[ndx].System]);

	ClearToPosLN2( 27);								// Write audio information
	WriteCode2( MinusPlusArrTxt[( UC) ( SDITSGConfig[ndx].AudioSignal > TSGAudioOff)]);
	WriteCode2( AudioTxt);

	ClearToPosLN2( 35);								// Write EDH information
	WriteCode2( MinusPlusArrTxt[( UC) ( SDITSGConfig[ndx].EDH)]);
	WriteCode2( EDHTxt);

	ClearRestLN2();									// Clear rest of line 2

															// Update group LEDs
	UpdateGrpLEDs( SDIFindGroup( SDITSGConfig[ndx].Pattern));

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusSDITPGShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980825				     													*/
/*																									*/
/*	Function:	Display current SDI-TPGx selection.									*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusSDITPGShow() {

	UC patterntype, textenable, ndx = Ptr->ItemNumber - StatusSDITPG1Item;

	WriteCodeLN1( 0, SDITPGHdrArrTxt[ndx]);
	WriteCode1( SDIPattArrTxt[SDITPGConfig[ndx].Pattern]);

	ClearRestLN1();									// Clear rest of line 1

	WriteCodeLN2( 0, SystemHdrTxt);				// Write system information
	WriteCode2( SDISystemArrTxt[SDITPGConfig[ndx].System]);

	ClearToPosLN2( 20);								// Write text information
	patterntype = SDIFindPatternType( SDITPGConfig[ndx].Pattern);
	textenable = SDITPGConfig[ndx].TextEnable;

	if ( patterntype == ComplexPattern)
		textenable &= 0x18;
	else
		textenable &= 0x07;

	WriteCode2( MinusPlusArrTxt[( UC) ( textenable || (SDITPGConfig[ndx].TextStyle[patterntype] & 0xF0))]);
	WriteCode2( TextTxt);

	ClearToPosLN2( 27);								// Write audio information
	WriteCode2( MinusPlusArrTxt[( UC) ( SDITPGConfig[ndx].AudioSignal > TPGAudioOff)]);
	WriteCode2( AudioTxt);

	ClearToPosLN2( 35);								// Write EDH information
	WriteCode2( MinusPlusArrTxt[( UC) ( SDITPGConfig[ndx].EDH)]);
	WriteCode2( EDHTxt);

	ClearRestLN2();									// Clear rest of line 2

															// Update group LEDs
	UpdateGrpLEDs( SDIFindGroup( SDITPGConfig[ndx].Pattern));

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusAESEBUShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980825			     														*/
/*																									*/
/*	Function:	Display current AES/EBU selection.									*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusAESEBUShow() {

	UC ndx = Ptr->ItemNumber - StatusAESEBU1Item;;

	WriteCodeLN1( 0, AESEBUHdrArrTxt[ndx]);
	WriteCode1( AESEBUSignalArrTxt[AESEBUConfig[ndx].Signal]);
	ClearRestLN1();

	WriteCodeLN2( 0, LevelHdrTxt);

	if ( AESEBUConfig[ndx].Signal == AESEBUWordClock) {
		WriteCode2( HyphenTxt);

		ClearToPosLN2( 17);

		WriteCode2( TimingHdrTxt);
		WriteCode2( HyphenTxt);
	}
	else {
		WriteCode2( AESEBULevelArrTxt[AESEBUConfig[ndx].Level]);

		ClearToPosLN2( 17);

		WriteCode2( TimingHdrTxt);
		WriteCode2( AESEBUTimingArrTxt[AESEBUConfig[ndx].Timing]);
	}

	ClearRestLN2();									// Clear rest of line 2

	UpdateGrpLEDs( NoGroup);						// Switch OFF all group LEDs

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusDateTimeShow															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980918				     													*/
/*																									*/
/*	Function:	Display current DATE-TIME selection and status.					*/
/*	Remarks:		Only displayed if the PT8637 Time Clock Option is mounted	*/
/*					Cursor position on line 1 and 2 are updated						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusDateTimeShow() {

	WriteCodeLN1( 0, DateHdrTxt);

	DateFormat(( TimeClockConfig.Format >> 1), TimeClockConfig.Date,\
																					0, TextBuffer);
	WriteTxt1( TextBuffer);
	ClearToPosLN1( 19);

	WriteCode1( TimeHdrTxt);
	TimeFormat(( TimeClockConfig.Format & 0x01), TimeClockConfig.Time,\
																					0, TextBuffer);
	WriteTxt1( TextBuffer);
	ClearToPosLN1( 36);

															// Write selected reference
	WriteCodeLN2( 0, RefHdrTxt);
	WriteCode2( DateTimeRefArrTxt[ TimeClockConfig.Reference]);

	ClearToPosLN2( 22);

	WriteCode2( StatusHdrTxt);						// Write status information

	switch ( TimeClockConfig.Reference) {
		case VITCTimeInput:
			if ( ActiveGenlockNo >= Internal)
				WriteCode2( NOGENLOCKTxt);
			else {
				if ( ActiveGenlockNo == SDIGenlock)
					WriteCode2( NOSIGNALTxt);
				else {
					if ( UnlockedLED)
						WriteCode2( UNLOCKEDTxt);
					else {
						if (( TimeClockConfig.TCIStatus & 0x0C) == 0x0C) {
							if ( TimeClockConfig.TCIStatus & 0x10)
								WriteCode2( VITCNTSCTxt);
							else
								WriteCode2( VITCPALTxt);
						}
						else
							WriteCode2( NOVITCTxt);
					}
				}
			}
			break;

		case LTCTimeInput:
			if ( !( TimeClockConfig.TCIStatus & 0x04))
				WriteCode2( NOSIGNALTxt);
			else {
				if ( TimeClockConfig.TCIStatus & 0x08) {
					if ( TimeClockConfig.TCIStatus & 0x10)
						WriteCode2( LTCNTSCTxt);
					else
						WriteCode2( LTCPALTxt);	
				}
				else
					WriteCode2( NOLTCTxt);
			}
			break;

		case Reference1HzInput:
			switch (( TimeClockConfig.TCIStatus & 0x0C) >> 2) {
				case 0:
					WriteCode2( NOSIGNALTxt);
					break;

				case 1:
					WriteCode2( TOOFASTTxt);
					break;

				case 2:
					WriteCode2( TOOSLOWTxt);
					break;

				case 3:
					WriteCode2( PRESENTTxt);
					break;
			}
			break;

		default:											//  VideoFieldTimeInput
			WriteCode2( HyphenTxt);	
			break;
	}
	ClearRestLN2();									// Clear rest of line 2

	UpdateGrpLEDs( NoGroup);						// Switch OFF all group LEDs

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/* StatusErrorShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980825				     													*/
/*																									*/
/*	Function:	If any active errors, display error from top of errorqueue	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void StatusErrorShow() {

	WriteCodeLN1(0, StatusErrorStatusTxt);
	ClearRestLN1();

	if ( ErrorStatus)
		WriteErrorLN2( 0, ErrorQueue[ErrorQueuePtr]);
	else {
		if ( ErrorQueue[ErrorQueuePtr])
			WriteCodeLN2( 0, NoWarningsTxt);
		else
			WriteCodeLN2( 0, NoErrorDetectedTxt);
	}

	ClearRestLN2();

	UpdateGrpLEDs( NoGroup);						// Switch OFF all group LEDs

	WriteArrowsField( Ptr->ItemConfig);			// Write icons/dots on line 1&2
}

/***************************************************************************/
/*	StatusDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Switch to menumode.														*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, StatusNdx, MenuModeOn												*/
/***************************************************************************/
void StatusDown(){

	StatusNdx = Ptr->ItemNumber;			// Save active Status-Display pointer

	MenuModeOn = TRUE;						// Set menu-mode

	UpdateGrpLEDs( NoGroup);				// Switch OFF all group LEDs

	Ptr = &base_menu[BaseNdx];				// Restore Base-Menu pointer
}

/***************************************************************************/
/*	UpToStatus																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Switch to status mode and show the last active status			*/
/*					display.																		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, MenuModeOn															*/
/***************************************************************************/
void UpToStatus(){

	MenuModeOn = FALSE;						// Clear menu-mode

	Ptr = &status_menu[StatusNdx];		// Restore Display-Status pointer
}

/***************************************************************************/
/*	FindSDIUnit																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980125	    										*/
/* Revised:		980126				     													*/
/*																									*/
/*	Function:	Find the SDI unit which is number <no> is the base menu		*/
/*	Remarks:		BaseMenuShow() help function											*/
/*	Returns:		0 if less units than <no> otherwise	the ItemNumber in the	*/
/*					base menu list																*/
/*	Updates:		--																				*/
/***************************************************************************/
UC FindSDIUnit( UC no) {

	register int i;

	for ( i = 0; i < 3; i++)
		if ( SDITSGUnit[i].Present)
			if ( !--no)
				return( SDITSG2Item + i);

	for ( i = 0; i < 3; i++)
		if ( SDITPGUnit[i].Present)
			if ( !--no)
				return( SDITPG1Item + i);

	return( 0);
}

/***************************************************************************/
/*	BaseMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the BASE menu.									*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void BaseMenuShow(){

	UC NoOfTXGs, NoOfSDITXGs, NoOfAnlTPGs;
	UC AESEBUPresent;
	UC DisplayType, spacing;

	code struct MENU_ITEM* tmpPtr;

	register int i, j;
	int number, start;

	WriteCodeLN1( 0, PT5230HeadlineTxt);
	ClearRestLN1();

	BaseNdx = Ptr->ItemNumber;							// Save Base Menu index

	WriteCodeLN2( 0, MenuHdrTxt);						// Write header for menu line

	AESEBUPresent = AESEBUUnit.Present;

	for ( NoOfAnlTPGs = i = 0; i < 2; i++)
		NoOfAnlTPGs += AnlTPGUnit[i].Present;

	for ( NoOfSDITXGs = i = 0; i < 3; i++)
		NoOfSDITXGs += SDITSGUnit[i].Present + SDITPGUnit[i].Present;

	NoOfTXGs = NoOfAnlTPGs + NoOfSDITXGs;

	tmpPtr = Ptr;						// Save menu pointer temporarily

											// The following DisplayType settings are
											//  based on the doc: Operating Description
	switch ( NoOfTXGs) {
		case 0:
		case 1:
			switch ( NoOfTXGs + AESEBUPresent) {
				case 0:
					if ( BaseNdx < PresetItem)
						DisplayType = 1;
					else
						DisplayType = 2;
					break;

				case 1:
					if ( BaseNdx < GenlockItem)
						DisplayType = 1;
					else
						if ( BaseNdx < TestItem)
							DisplayType = 3;
					else
						DisplayType = 5;
					break;

				case 2:
					if ( BaseNdx < AESEBUItem)
						DisplayType = 1;
					else
						if ( BaseNdx < ConfigItem)
							DisplayType = 6;
					else
						DisplayType = 4;
					break;
			}
			break;

		case 2:
			if ( NoOfAnlTPGs == 2) {
				if ( BaseNdx < AnlTPG5Item)
					DisplayType = 1;
				else {
					if ( !AESEBUPresent) {
						if ( BaseNdx < PresetItem)
							DisplayType = 7;
						else
							DisplayType = 2;
					}
					else {
						if ( BaseNdx < GenlockItem)
							DisplayType = 7;
						else
							if ( BaseNdx < TestItem)
								DisplayType = 3;
						else
							DisplayType = 5;
					}
				}
			}
			else
				if ( NoOfAnlTPGs) {
					if ( BaseNdx < AnlTPG5Item+1)
						DisplayType = 1;
					else {
						if ( !AESEBUPresent) {
							if ( BaseNdx < ConfigItem)
								DisplayType = 10;
							else
								DisplayType = 4;
						}
						else {
							if ( BaseNdx < PresetItem)
								DisplayType = 10;
							else
								DisplayType = 2;
						}
					}
				}
			else {
				if ( BaseNdx < FindSDIUnit( 2))
					DisplayType = 1;
				else {
					if ( !AESEBUPresent) {
						if ( BaseNdx < ConfigItem)
							DisplayType = 11;
						else
							DisplayType = 4;
					}
					else {
						if ( BaseNdx < PresetItem)
							DisplayType = 11;
						else
							DisplayType = 2;
					}
				}
			}
			break;

		case 3:
			if ( NoOfAnlTPGs == 2) {
				if ( BaseNdx < AnlTPG5Item)
					DisplayType = 1;
				else {
					if ( !AESEBUPresent) {
						if ( BaseNdx < GenlockItem)
							DisplayType = 7;
						else
							if ( BaseNdx < TestItem)
								DisplayType = 3;
						else
							DisplayType = 5;
					}
					else {
						if ( BaseNdx < AESEBUItem)
							DisplayType = 7;
						else
							if ( BaseNdx < ConfigItem)
								DisplayType = 6;
						else
							DisplayType = 4;
					}
				}
			}
			else
				if ( NoOfAnlTPGs) {
					if ( BaseNdx < AnlTPG5Item+1)
						DisplayType = 1;
					else {
						if ( !AESEBUPresent) {
							if ( BaseNdx < PresetItem)
								DisplayType = 8;
							else
								DisplayType = 2;
						}
						else {
							if ( BaseNdx < GenlockItem)
								DisplayType = 8;
							else
								if ( BaseNdx < TestItem)
									DisplayType = 3;
							else
								DisplayType = 5;
						}
					}
				}
			else {
				if ( BaseNdx < FindSDIUnit( 2))
					DisplayType = 1;
				else {
					if ( !AESEBUPresent) {
						if ( BaseNdx < PresetItem)
							DisplayType = 9;
						else
							DisplayType = 2;
					}
					else {
						if ( BaseNdx < GenlockItem)
							DisplayType = 9;
						else
							if ( BaseNdx < TestItem)
								DisplayType = 3;
						else
							DisplayType = 5;
					}
				}
			}
			break;

		case 4:
			if ( NoOfAnlTPGs) {
				if ( BaseNdx < FindSDIUnit( 1))
					DisplayType = 1;
				else {
					if ( !AESEBUPresent) {
						if ( BaseNdx < GenlockItem)
							DisplayType = 8;
						else
							if ( BaseNdx < TestItem)
								DisplayType = 3;
						else
							DisplayType = 5;
					}
					else {
						if ( BaseNdx < AESEBUItem)
							DisplayType = 8;
						else
							if ( BaseNdx < ConfigItem)
								DisplayType = 6;
						else
							DisplayType = 4;
					}
				}
			}
			else {
				if ( BaseNdx < FindSDIUnit( 2))
					DisplayType = 1;
				else {
					if ( !AESEBUPresent) {
						if ( BaseNdx < GenlockItem)
							DisplayType = 9;
						else
							if ( BaseNdx < TestItem)
								DisplayType = 3;
						else
							DisplayType = 5;
					}
					else {
						if ( BaseNdx < AESEBUItem)
							DisplayType = 9;
						else
							if ( BaseNdx < ConfigItem)
								DisplayType = 6;
						else
							DisplayType = 4;
					}
				}
			}
			break;
	}

	spacing = 3;								// Normal spacing between items is 3

	switch ( DisplayType) {
		case 1:
			Ptr = &base_menu[AnlBlkItem];
			start = AnlBlkItem;
			number = 2;
			break;

		case 2:
			Ptr = &base_menu[PresetItem];
			start = PresetItem;
			number = 2 + TestModeEnable;
			break;

		case 3:
			Ptr = &base_menu[GenlockItem];
			start = GenlockItem;
			number = 3;
			break;

		case 4:
			Ptr = &base_menu[ConfigItem];
			start = ConfigItem;
			number = 1 + TestModeEnable;
			break;

		case 5:
			Ptr = &base_menu[TestItem];
			start = TestItem;
			number = 1;
			break;

		case 6:
			Ptr = &base_menu[AESEBUItem];
			start = AESEBUItem;
			number = 3;
			break;

		case 7:
			Ptr = &base_menu[AnlTPG5Item];
			start = AnlTPG5Item;
			number = 2;
			break;

		case 8:
		case 9:
			Ptr = &base_menu[start = FindSDIUnit( DisplayType-7)];
			spacing = 2;
			number = 3;
			break;

		case 10:
		case 11:
			Ptr = &base_menu[start = FindSDIUnit( DisplayType-9)];
			number = 3;
			break;
	}

	for ( j = 8, i = start; i < start+number; i++) {
		WriteCode2( Ptr->ItemTxt);

		if ( spacing == 2)
			WriteTxt2( "  ");
		else
			WriteTxt2( "   ");

		if ( Ptr->ItemNumber < BaseNdx )
			j += ( spacing + Cstrlen( Ptr->ItemTxt));

		BaseItemRight();
	}

	ClearRestLN2();					// Clear rest of line 2

	Ptr = tmpPtr;						// Restore menu pointer

	i = Ptr->ItemConfig;				// Get info on line 1&2 icons/dots

											// If PANEL LOCK enabled..
											//  include a pad-lock and remove the
											//  down arrow except for the CONFIG menu.
	if (( Flags.PanelLockOn || Flags.NormalLockOn ) && ( BaseNdx != ConfigItem)) {
		if ( Flags.PanelLockOn)
			i = ( i | P_) & ~D_;
		else {
			switch ( BaseNdx) {
				case AnlBlkItem:
				case GenlockItem:
					i = ( i | P_) & ~D_;
					break;
			}
		}
	}

	WriteArrowsField( i);								// Write line 1&2 icons/dots

	WriteItemArrows( j, Cstrlen( Ptr->ItemTxt));	// Mark selected item
}

/***************************************************************************/
/*	BaseDown																			  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Go to the selected item in the BASE MENU.							*/
/*	Remarks:		BASE MENU function.														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void BaseDown(){

	UC ndx;

	if ( Flags.PanelLockOn)						// If PANEL LOCK is enabled..
		if ( BaseNdx != ConfigItem)			//  only CONFIG MENU is alllowed
			return;

	switch ( BaseNdx) {
		case AnlBlkItem:							// If BLACK-BURST MENU selected..
			if ( !Flags.NormalLockOn)
				Ptr = &anlblk_menu[AnlBlkNdx];
			break;

		case AnlTPG2Item:							// If ANALOG-TPGx MENU selected..
		case AnlTPG5Item:
			ndx = BaseNdx-AnlTPG2Item;
			Ptr = &AnlTPG_menu[AnlTPGNdx[ndx]];
			break;

		case SDITSG2Item:							// If SDI-TSGx selected..
		case SDITSG3Item:
		case SDITSG4Item:
			ndx = BaseNdx - SDITSG2Item;
			Ptr = &SDITSG_menu[SDITSGNdx[ndx]];
			break;

		case SDITPG1Item:							// If SDI-TPGx selected..
		case SDITPG2Item:
		case SDITPG5Item:
			ndx = BaseNdx - SDITPG1Item;
			Ptr = &SDITPG_menu[SDITPGNdx[ndx]];
			break;

		case AESEBUItem:							// If AES-EBU MENU selected..
			Ptr = &AESEBUn_menu[AESEBUnNdx];
			break;

		case GenlockItem:							// If GENLOCK MENU selected..
			if ( !Flags.NormalLockOn) {		//  if NORMAL LOCK disabled..

				PrevPtr = Ptr;						//   Store old menu-pointer
				Ptr = &genlock_menu;				//   Point to genlock-menu

				UcharVal = ActiveGenlockNo;	//   Restore selected genlock input
														//   Restore selected genlock system
				UcharVal1 = GenlockConfig[UcharVal].System;

				GenlockInputShow(); 				//   /Prepare/display the menu
			}
			break;

		case PresetItem:							// If PRESET MENU selected..
			Ptr = &preset_menu[PresetNdx];
			break;

		case ConfigItem:							// If CONFIG MENU selected..
			Ptr = &config_menu[ConfigNdx];
			break;

		case TestItem:								// If TEST MENU selected..
			break;
	}
}

/***************************************************************************/
/*	UpToBase																			  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980126				     													*/
/*																									*/
/*	Function:	Go to the BASE MENU and restore last selected item				*/
/*	Remarks:		BASE MENU function														*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToBase(){

	UpdateGrpLEDs( NoGroup);					// Switch OFF group LEDs

	Ptr = &base_menu[BaseNdx];					// Restore Base menu item
}

/***************************************************************************/
/*	AnlBlkMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the BLACK-BURST menu.						*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		AnlBlkNdx																	*/
/***************************************************************************/
void AnlBlkMenuShow(){

	register int i, j, last, NoOfBBs;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( CoConfigureTxt);			 		// Write menu headline
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

			if (( i == BB2Item) && ( AnlBlkUnit[BB2Item].HWType == PT8604)) {
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

	WriteArrowsField( i);							// Write line 1&2 icons/dots

															// Get length of text to mark
	if (( AnlBlkNdx == BB2Item) && ( AnlBlkUnit[BB2Item].HWType == PT8604))
		i = Cstrlen( BBMulTxt);
	else
		i = Cstrlen( Ptr->ItemTxt);

	WriteItemArrows( j+8, (UC) i);				// Mark selected item
}

/***************************************************************************/
/*	AnlBlkDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Go to the selected item in the BLACK-BURST menu.				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AnlBlkDown(){

	Ptr = &BB_menu[BBNdx[AnlBlkNdx]];		// Restore unit BBn's last item
}

/***************************************************************************/
/* UpToAnlBlk																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Go to the BLACK-BURST menu and restore last selected item.	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToAnlBlk(){

	Ptr = &anlblk_menu[AnlBlkNdx];		// Restore selected BLACK-BURST item.
}

/***************************************************************************/
/*	BBMenuShow																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the BLACK-BURST/BB submenu.				*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		BBNdx[AnlBlkNdx]															*/
/***************************************************************************/
void BBMenuShow(){

	register int i;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');

	if (( AnlBlkNdx == BB2Item) && ( AnlBlkUnit[BB2Item].HWType == PT8604))
		CodePtr = BBMulTxt;
	else
		CodePtr = anlblk_menu[AnlBlkNdx].ItemTxt;

	WriteCode1( CodePtr);

	WriteCode1( CoSelectTxt);						// Write sub-menu headline

	ClearRestLN1();

	BBNdx[AnlBlkNdx] = Ptr->ItemNumber;			// Update Black Burst menu index

	WriteCodeLN2( 0, SecondmnuHdrTxt);			// Write line 2 header

	for ( i = BBSystemItem; i <= BBScHPhaseItem; i++) {
		ClearToPosLN2( BB_menu[i].ItemPos);		// Find item column position
		WriteCode2( BB_menu[i].ItemTxt);			// Write item texts
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write line 1&2 icons/dots
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	BBDown																			  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Go to the selected item in the BLACK-BURST/BBx submenu.		*/
/*	Remarks:		Cursor position on line 1 is updated. 								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void BBDown(){

	WriteCodeLN1( 0, SecondmnuHdrTxt);
	WriteCode1( SlashDotHdrTxt);

	if (( AnlBlkNdx == BB2Item) && ( AnlBlkUnit[BB2Item].HWType == PT8604))
		WriteCode1( BBMulTxt);
	else
		WriteCode1( anlblk_menu[AnlBlkNdx].ItemTxt);

	WriteChar1( '/');
	WriteCode1( BB_menu[BBNdx[AnlBlkNdx]].ItemTxt);

	switch ( BBNdx[AnlBlkNdx]) {
		case BBSystemItem:
			WriteCode1( CoSelectTxt);

			UcharVal = AnlBlkConfig[AnlBlkNdx].System;	// Get BBn system
			break;

		case BBTimingItem:
			WriteCode1( CoEditDelayTxt);

			UcharVal = AnlBlkConfig[AnlBlkNdx].System;	// Get BBn system
			samples = AnlBlkConfig[AnlBlkNdx].Delay;		// Get BBn delay

			stepNdx = stepNdxAnalogMin;
			stepVal = stepsizeAnalogTable[stepNdx];		// Get step size
			break;

		case BBScHPhaseItem:
			WriteCode1( CoEditTxt);

			IntVal = AnlBlkConfig[AnlBlkNdx].ScHPhase;	// Get BBn ScH-Phase
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
/* Revised:		980428				     													*/
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

															// Test if delay is valid
															//  in the new system, (AFTER
															//  the system is saved)
			AnlValidateSetting( AnlBlkUnit[AnlBlkNdx].HWType, AnlBlkNdx);

			if ( ErrorCode = TrxAnlBlkSystem( AnlBlkNdx, UcharVal,\
													AnlBlkConfig[AnlBlkNdx].Delay,\
													AnlBlkConfig[AnlBlkNdx].ScHPhase)) {
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

	CodePtr = AnlSystemArrTxt[UcharVal];		// Get text for selection

	WriteCodeLN2( 0, SystemHdrTxt);				// Write header on line 2
	WriteCode2( CodePtr);							// Write selected text on line 2

	ClearRestLN2();                       	 	// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	BBTimingSelect																 	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Set the delay in a Black Burst Generator.							*/
/*	Remarks:		BLACK-BURST SUBMENU function.											*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:	   TBD																			*/
/***************************************************************************/
void BBTimingSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {										// If delay has changed..
			ActivePresetNo = 0;								// Clear possible reset

			AnlBlkConfig[AnlBlkNdx].Delay = samples;	// Save new BBn delay
		}
																	// Transmit delay to BBn
																	//  MUST be .Delay
		TrxAnlBlkDelay( AnlBlkNdx, UcharVal, AnlBlkConfig[AnlBlkNdx].Delay);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	AnlTimeAdjust( AnlBlkItem, UcharVal, AnlBlkConfig[AnlBlkNdx].Delay);

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	BBScHPhaseSelect															 	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980208				     													*/
/*																									*/
/*	Function:	Setting the ScH-phase value in a black burst generator.		*/
/*	Remarks:		BLACK-BURST SUBMENU function											*/
/*					Instant update of ScH-Phase. The ScH-Phase is not updated	*/
/*					if the menu is left without altering the value.					*/
/*	Returns:		--																				*/
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
/*	AnlTPGMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the ANALOG-TPGx menu.						*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGMenuShow(){

	register int i, j;
	UC ndx = BaseNdx - AnlTPG2Item;

	WriteCodeLN1( 0, MenuHdrTxt);					// Write menu header on line 1
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( CoConfigureTxt);
	ClearRestLN1();									// Clear rest of line 1

	AnlTPGNdx[ndx] = Ptr->ItemNumber;			// Update Analog menu index

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for line 2

	i = j = AnlTPGTimingItem;						// Find menu window for item

	if ( AnlTPGNdx[ndx] <  AnlTPGTimingItem)	// If item < TIMING
		i = AnlTPGPatternItem;						//  Pattern, Text & System window
	else                                      // else
		j = AnlTPGScHPhaseItem+1;              //  Timing & ScH-Phase window

	for ( ; i < j; i++) {
		ClearToPosLN2( AnlTPG_menu[i].ItemPos);// Write item position
		WriteCode2( AnlTPG_menu[i].ItemTxt);	// Write item text
	}

	ClearRestLN2();										// Clear rest of line 2

	i = Ptr->ItemConfig;									// Get line 1&2 icons/dots

	if ( Flags.NormalLockOn )							// If normal lock is ENABLED..
		if ( AnlTPGNdx[ndx] != AnlTPGPatternItem)	// If not pattern menu..
			i = ( i | P_) & ~D_; 						//  add padlock, remove down
																//  arrow.

	WriteArrowsField( i);								// Write line 1&2 icons/dots
																// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	AnlTPGDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		981126				     													*/
/*																									*/
/*	Function:	Go to the selected item in the ANALOG-TPGx menu.				*/
/*	Remarks:		Cursor position on line 1&2 are updated. 							*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGDown(){

	UC ndx = BaseNdx - AnlTPG2Item, itemNdx = AnlTPGNdx[ndx];

	if ( Flags.NormalLockOn)						// If normal LOCK is enabled..
		if ( itemNdx != AnlTPGPatternItem)		// If not pattern menu..
			return;										//  return immediately

	WriteCodeLN1( 0, SubmnuHdrTxt);				// Write submenu header on line 1
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( AnlTPG_menu[itemNdx].ItemTxt);

															// DO NOT MOVE NEXT TWO LINES!!
	PrevPtr = Ptr;                		  		// Store old menu-pointer
	Ptr = &AnlTPG_submenu[itemNdx];				// Point to selected item

	switch ( itemNdx) {
		case AnlTPGPatternItem:                // Prepare for pattern selection
			WriteCode1( CoSelectTxt);           // Write additional line 1 text
															// Get pattern
			UcharVal = AnlTPGConfig[ndx].Pattern;
															// Determine pattern type
			UcharVal1 = AnlFindPatternType( UcharVal);
															// Switch on group LED
			UpdateGrpLEDs( AnlFindGroup( UcharVal));

			FuncState = 1;								// Prepare for item statemachine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case AnlTPGTextItem:							// Prepare for ../TEXT menu
			Ptr = &AnlTPGtext_menu[AnlTPGTextNdx[ndx]];
			break;

		case AnlTPGSystemItem:                 // Prepare for system selection
			WriteCode1( CoSelectTxt);				// Write additional line1 text

			UcharVal = AnlTPGConfig[ndx].System;// Get system

			FuncState = 1;								// Prepare for item statemachine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case AnlTPGTimingItem:						// Prepare for timing selection
			WriteCode1( CoEditTxt);             // Write additional line 1 text

			UcharVal = AnlTPGConfig[ndx].System;// Get system
			samples = AnlTPGConfig[ndx].Delay;	// Get delay

			stepNdx = stepNdxAnalogMin;         	// Get step for analog timing
			stepVal = stepsizeAnalogTable[stepNdx];// Get step size

			FuncState = 1;								// Prepare for item statemachine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case AnlTPGScHPhaseItem:					// Prepare for ScH-Phase selection
			IntVal = AnlTPGConfig[ndx].ScHPhase;// Get ScH-Phase

			FuncState = 1;								// Prepare for item statemachine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;
	}

	ClearRestLN1();									// Clear rest of line 1
}

/***************************************************************************/
/*	AnlTPGPatternSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Select test-pattern in an Analog-TPG.								*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*					Instant update implemented												*/
/*					MODIFY is only available when a complex pattern has been		*/
/*					selected. PT8601 do NOT contain any complex patterns,	hence */
/*					this field will never be displayed									*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGPatternSelect() {

	UC ndx = BaseNdx - AnlTPG2Item;

	if ( FuncState == 13) {						// If MODIFY item selected..

		WriteCodeLN1( 0, SecondmnuHdrTxt);	// Write menu header on line 1
		WriteCode1( SlashDotHdrTxt);
		WriteCode1( MODIFYTxt);
		WriteTxt1( ", ");
		CodePtr = AnlPattArrTxt[UcharVal];	// Get text for selected pattern
		WriteCode1( CodePtr);               // Write pattern text on line 1
		ClearRestLN1();							// Clear rest of line 1

														// Get pattern attribute
		UcharPtr = &AnlTPGConfig[ndx].Attrib[UcharVal-AnlPhilips4x3];
		UcharVal2 = *UcharPtr;

		if ( AnlTPGConfig[ndx].System < NTSC )
			UcharVal3 = AttribAntiPAL;
		else
			UcharVal3 = AttribPLUGE;

		FuncState = 1;								// Prepare for item statemachine
		FuncTriggers = 0;
		FuncSave = FuncEscape = FALSE;

		Ptr = &AnlTPGpattern_menu;	  			// Set pointer to MODIFY menu
		Ptr->DispFunct();						 	// Display next menu
		return;
	}

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If pattern has changed..
			ActivePresetNo = 0;						// Clear a possible preset
															// Save new pattern
			AnlTPGConfig[ndx].Pattern = UcharVal;
		}
		else {											// ..restore ANALOG-TPGx pattern
			if ( UcharVal != AnlTPGConfig[ndx].Pattern) {
				UcharVal = AnlTPGConfig[ndx].Pattern;

				ErrorCode = TrxAnlTPGPattern( ndx, UcharVal, AnlTPGAttrib( ndx, UcharVal));

				if ( !ErrorCode)
					ErrorCode = ConfigureAnlTPGText( ndx, UcharVal);

				if ( ErrorCode) {
					GotoErrorStatus( 256*ndx + ErrorCode);
					return;
				}
			}
		}
		UpdateGrpLEDs( NoGroup);					// Switch OFF group LEDs

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..

		do {
			if ( FuncTriggers & 0x01) {         // If next pattern..
				if ( UcharVal-- == AnlSMPTECBar)
					UcharVal = AnlVMT01;
			}
			else {										// ..else previous pattern
				if ( UcharVal++ == AnlVMT01)
					UcharVal = AnlSMPTECBar;
			}
		} while( UcharVal != AnlValidatePattern( AnlTPGUnit[ndx].HWType, ndx,\
													 AnlTPGConfig[ndx].System, UcharVal));

		UpdateGrpLEDs( AnlFindGroup( UcharVal));

		if ( UcharVal == AnlTPGConfig[ndx].Pattern) {	// If current pattern..
			if ( FuncState > 5)									//  remove SAVE option
				FuncState = 6;
			else
				FuncState = 1;
		}
																// Transmit pattern selection
		ErrorCode = TrxAnlTPGPattern( ndx, UcharVal, AnlTPGAttrib( ndx, UcharVal));

		if ( !ErrorCode)
			ErrorCode = ConfigureAnlTPGText( ndx, UcharVal);

		if ( ErrorCode) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	if (( UcharVal >= AnlPhilips4x3) && ( UcharVal <= AnlFuBK16x9)) {
		if ( FuncState == 1)
			FuncState = 6;

		if ( FuncState == 3)
			FuncState = 9;
	}
	else {
		if ( FuncState == 6)
			FuncState = 1;

		if ( FuncState == 9)
			FuncState = 3;
	}

	CodePtr = AnlPattArrTxt[UcharVal];			// Get text for selected pattern

	WriteCharLN2( 0, ' '	);							// No header on line 2
	WriteCode2( CodePtr);							// Write the selected pattern
	ClearRestLN2();									// Clear rest of line 2

	WriteType7_SAVE_ESC( FuncState);				// Write the SAVE ESC texct

	switch ( FuncState ) {
		case 1:
		case 3:
		case 6:
		case 9:											// Mark selected item
			WriteItemArrows( 1, Cstrlen( CodePtr));
			break;
	}

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	AnlTPGPatternModify															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990305	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Modify selected test-pattern in an Analog-TPG					*/
/*	Remarks:		ANALOG-TPGx SUBMENU function											*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented												*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGPatternModify() {

	UC tmp, ndx = BaseNdx - AnlTPG2Item;
	UC bitPos;

	if ( FuncEscape) {
															// Restore pattern attribute
		if ( UcharVal2 != *UcharPtr)				//  if not saved
			if ( ErrorCode = TrxAnlTPGPattern( ndx, UcharVal, *UcharPtr)) {
				GotoErrorStatus( 256*ndx + ErrorCode);
				return;
			}

		WriteCodeLN1( 0, SubmnuHdrTxt);				 // Write previous submenu
		WriteCode1( base_menu[BaseNdx].ItemTxt);	 //  on line 1
		WriteChar1( '/');
		WriteCode1( AnlTPG_menu[AnlTPGNdx[ndx]].ItemTxt);
		WriteCode1( CoSelectTxt);						 // Write additional line 1
																 //  text

		if ( UcharVal == AnlTPGConfig[ndx].Pattern)// If current pattern..
			FuncState = 7;                          //  remove SAVE option
		else
			FuncState = 10;

		FuncTriggers = 0;
		FuncSave = FuncEscape = FALSE;

		Ptr = &AnlTPG_submenu[AnlTPGNdx[ndx]];		 // Point to previous item
		Ptr->DispFunct();									 // Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..

		do {
			if ( FuncTriggers & 0x01) {         // If next attribute..
				if ( UcharVal3-- == AttribAntiPAL)
					UcharVal3 = AttribCircles;
			}
			else {										// ..else previous attribute
				if ( UcharVal3++ == AttribCircles)
					UcharVal3 = AttribAntiPAL;
			}
		} while( UcharVal3 != AnlValidateAttrib( AnlTPGUnit[ndx].HWType, ndx,\
									 AnlTPGConfig[ndx].System, UcharVal, UcharVal3));
	}

	bitPos = 1 << UcharVal3;						// Find attribute bit position

	if ( FuncSave) {									// If SAVE executed
		FuncSave = FALSE;

		*UcharPtr &= ~bitPos;						// Clear present attribute
		*UcharPtr |= ( bitPos & UcharVal2);    // Save new attribute
	}

	if ( FuncTriggers & 0x0C) {					// If button UP/DOWN pressed..
		UcharVal2 ^= bitPos;							//  toggle attribute selection

		if ( !( bitPos & ( UcharVal2^*UcharPtr)))
			FuncState = 2;
															// Transmit pattern selection
		if ( ErrorCode = TrxAnlTPGPattern( ndx, UcharVal, UcharVal2)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	if ( FuncTriggers & 0x03) {					// DO NOT MOVE TO THE ONE ABOVE
		if ( !( bitPos & ( UcharVal2^*UcharPtr)))
			FuncState = 1;
		else
			FuncState = 5;
	}

	tmp = (( bitPos & UcharVal2) == bitPos);		// Attribute state

	WriteCodeLN2( 0, ModifyHdrTxt);					// Write line 2 header
	ClearRestLN2();

	WriteType2_SAVE_ESC( FuncState);					// Write the ESC text

	CodePtr = AttribArrTxt[UcharVal3];				// Get attribute text

	WriteCodeLN2( 8, CodePtr);							// Write attribute
	WriteCodeLN2( 24, OffOnArrTxt[tmp]);			// Wtite attribute state

	switch ( FuncState) {
		case 1:
		case 5:
			WriteItemArrows( 8, Cstrlen( CodePtr));// Mark selected item
			break;

		case 2:
		case 4:												// Mark selected item
			WriteItemArrows( 24, Cstrlen( OffOnArrTxt[tmp]));
			break;
	}

	FuncTriggers = 0;										// Reset button-selection
}

/***************************************************************************/
/*	AnlTPGSystemSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980503				     													*/
/*																									*/
/*	Function:	Select TV system for an Analog-TPG.									*/
/*	Remarks:		ANALOG-TPGx MENU function.												*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGSystemSelect(){

	UC pattern, ndx = BaseNdx - AnlTPG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If system has changed...
			ActivePresetNo = 0;  					// Clear possible preset

			AnlTPGConfig[ndx].System = UcharVal;// Save the updated system

															// Test if delay/pattern is valid
															//  in the new system, (AFTER
															//  the system is saved)
			AnlValidateSetting( AnlTPGUnit[ndx].HWType, ndx);

			pattern = AnlTPGConfig[ndx].Pattern;// Get validated pattern

			if ( ErrorCode = TrxAnlTPGSystem( ndx, UcharVal, AnlTPGConfig[ndx].Delay,\
												  pattern, AnlTPGAttrib( ndx, pattern))) {
				GotoErrorStatus( 256*ndx + ErrorCode);
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

		if ( UcharVal == AnlTPGConfig[ndx].System)	// If current system..
			FuncState = 1;										//  remove SAVE option
	}

	CodePtr = AnlSystemArrTxt[UcharVal];		// Get text for selected system

	WriteCodeLN2( 0, SystemHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr); 							// Write selected system text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	AnlTPGTimingSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 9801211	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Set the delay for an Analog-TPG.											*/
/*	Remarks:		ANALOG-TPGx MENU function.												*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:	   TBD																			*/
/***************************************************************************/
void AnlTPGTimingSelect(){

	UC ndx = BaseNdx - AnlTPG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                 		// If delay has changed..
			ActivePresetNo = 0;						// Clear possible preset

			AnlTPGConfig[ndx].Delay = samples;	// Save updated delay
		}
															// Transmit delay to AnlTPGn
															// MUST be .Delay
		TrxAnlTPGDelay( ndx, UcharVal, AnlTPGConfig[ndx].Delay);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Adjust timing
	AnlTimeAdjust( BaseNdx, UcharVal, AnlTPGConfig[ndx].Delay);

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	AnlTPGScHPhaseSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Setting ScH-phase value in a Analog-TPG.							*/
/*	Remarks:		ANALOG-TPGx MENU function.											  	*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGScHPhaseSelect(){

	UC ndx = BaseNdx - AnlTPG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {									// If ScH-Phase has changed..
			ActivePresetNo = 0;							// Clear possible preset

			AnlTPGConfig[ndx].ScHPhase = IntVal;	// Save new ScH-Phase
		}
		else {												// ..restore ScH-Phase
			if ( IntVal != AnlTPGConfig[ndx].ScHPhase)
				if ( ErrorCode = TrxAnlTPGScHPhase( ndx, AnlTPGConfig[ndx].ScHPhase)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
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

		if ( IntVal == AnlTPGConfig[ndx].ScHPhase)	// If current ScH-Phase..
			FuncState = 1;										//  remove SAVE option

																	// Transmit ScH-Phase
		if ( ErrorCode = TrxAnlTPGScHPhase( ndx, IntVal)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
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
/* UpToAnlTPG																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980303	    										*/
/* Revised:		980303				     													*/
/*																									*/
/*	Function:	Go to the ANALOG-TPGx MENU and restore the last selected		*/
/*					item.																			*/
/*	Remarks:		ANALOG-TPGx SUBMENU function.											*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToAnlTPG(){
													// Restore selected ANALOG-TPGx unit
	Ptr = &AnlTPG_menu[AnlTPGNdx[BaseNdx-AnlTPG2Item]];
}

/***************************************************************************/
/*	AnlTPGTextMenuShow															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980303	    										*/
/* Revised:		000510				     													*/
/*																									*/
/*	Function:	Display the items in the ANALOG-TPGx/TEXT submenu.	  			*/
/*	Remarks:		This submenu will have 2 or 3 items depending on the PT8637.*/
/*					The PT8601 only supports the EDIT item.							*/
/*					Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGTextMenuShow(){

	register int i;
	UC patternType, tmp, ndx = BaseNdx - AnlTPG2Item;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( AnlTPG_menu[AnlTPGNdx[ndx]].ItemTxt);
	WriteCode1( CoConfigureTxt);						// Write sub-menu headline
	ClearRestLN1();										// Clear rest of line 1

	AnlTPGTextNdx[ndx] = Ptr->ItemNumber;			// Update ../TEXT menu index

	WriteCodeLN2( 0, SecondmnuHdrTxt);				// Write line 2 header

	tmp = TPGTextPosItem;

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637))
		tmp += ( AnlTPGUnit[ndx].HWType == PT8631);

	for ( i = TPGTextEditItem; i <= tmp; i++) {
		ClearToPosLN2( AnlTPGtext_menu[i].ItemPos);	// Find item column position
		WriteCode2( AnlTPGtext_menu[i].ItemTxt);		// Write item texts
	}

	ClearRestLN2();										// Clear rest of line 2

	i = Ptr->ItemConfig;

	if ( AnlTPGUnit[ndx].HWType == PT8601) {
		if ( AnlTPGTextNdx[ndx] != TPGTextEditItem)	// Only text editing is
			i &= ~D_;                              	//  available in PT8601
	}
	else {
		patternType = AnlFindPatternType( AnlTPGConfig[ndx].Pattern);

		if ( AnlTPGTextNdx[ndx] == TPGTextPosItem)
			if (( AnlTPGConfig[ndx].TextStyle[patternType] & 0x0F) != TPGFreeTextStyle)
				i &= ~D_;									// Textstyle is not allowed
																//  for the standard patterns
	}

	WriteArrowsField( i);							// Write line 1&2 icons/dots
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	AnlTPGTextDown																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980303	    										*/
/* Revised:		000510				     													*/
/*																									*/
/*	Function:	Go to the selected item in the ANALOG-TPGx/TEXT submenu.		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGTextDown(){

	UC patternType, ndx = BaseNdx - AnlTPG2Item, itemNdx = AnlTPGTextNdx[ndx];

	patternType = AnlFindPatternType( AnlTPGConfig[ndx].Pattern);

	if ( SDITPGUnit[ndx].HWType == PT8601) {
		if ( itemNdx != TPGTextEditItem)		  	// Only text editing is
			return;                  			  	//  available in PT8601
	}
	else {
		if ( AnlTPGTextNdx[ndx] == TPGTextPosItem)
			if (( AnlTPGConfig[ndx].TextStyle[patternType] & 0x0F) != TPGFreeTextStyle)
				return;
	}

	WriteCodeLN1( 0, SecondmnuHdrTxt);			// Write second menu header
	WriteCode1( SlashDotHdrTxt);				  	//  on line 1
	WriteCode1( AnlTPGtext_menu[itemNdx].ItemTxt);

	if ( patternType == ComplexPattern)
		CodePtr = CoComplexPatternTxt;
	else
		CodePtr = CoStandardPatternTxt;

	WriteCode1( CodePtr);
	ClearRestLN1();									// Clear rest of line 1

	switch ( itemNdx) {
		case TPGTextEditItem:					   // Prepare for text editing
			if ( AnlTPGUnit[ndx].HWType == PT8601)
				UcharVal1 = UcharVal2 = 0; 	   // Array start/stop
			else {
				if ( patternType == ComplexPattern) {
					UcharVal1 = 3;					   // Array start
					UcharVal2 = 4;       		   // Array stop
				}
				else {
					UcharVal1 = 0; 			      // Array start
					UcharVal2 = 2;          	  	// Array stop
				}
			}
			UcharVal = UcharVal1;
															// Copy string to temp. buffer
			strcpy( TextBuffer, AnlTPGConfig[ndx].Text[UcharVal]);

															// Find text enable information
			UcharVal3 = ( AnlTPGConfig[ndx].TextEnable & ( 1 << UcharVal)) > 0;

			if ( UintVal = strlen( TextBuffer))	// Position in Text string
				UintVal--;
			break;

		case TPGTextStyleItem:                 // Prepare for style selection
			UcharVal1 = patternType;
															// Mask out style
			UcharVal = AnlTPGConfig[ndx].TextStyle[UcharVal1] & 0x0F;
			break;

		case TPGTextPosItem:       	          // Prepare for position selection
			UcharVal = AnlTPGConfig[ndx].TextPos[patternType][0];
			UcharVal1 = AnlTPGConfig[ndx].TextPos[patternType][1];
			break;

		case TPGTextClockItem:					   // Prepare for date/time selection
			UcharVal1 = patternType;
															// Mask out date/time
			UcharVal = AnlTPGConfig[ndx].TextStyle[UcharVal1] >> 4;
			break;
	}

	PrevPtr = Ptr;										// Store old menu-pointer
															// Point to selected item
	Ptr = &AnlTPGtext_submenu[itemNdx];

	FuncState = 1;										// Prepare for item statemachine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Set select-mode
}

/***************************************************************************/
/*	AnlTPGTextEditSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980303	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Edit a text in a Analog-TPG.											*/
/*	Remarks:		ANALOG-TPGx/TEXT MENU function.										*/
/*					Cursor position on line 2 is updated.								*/
/*					UcharVal 		LineNumber 0-4											*/
/*					UcharVal1		Min. LineNumber                              */
/*					UcharVal2		Max. LineNumber                              */
/*					UcharVal3      Text Enable ON/OFF                           */
/*					UintVal			Position in text buffer								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGTextEditSelect(){

	UC tmp, ndx = BaseNdx - AnlTPG2Item;

	if ( FuncEscape) {
		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncSave) {								  	// If textinfo has changed
		FuncSave = FALSE;
		ActivePresetNo = 0;						  	// Clear possible preset
															// Save the new text
		strcpy( AnlTPGConfig[ndx].Text[UcharVal], TextBuffer);

															// Save text enable information
		AnlTPGConfig[ndx].TextEnable &= ~( 1 << UcharVal);
		AnlTPGConfig[ndx].TextEnable |= ( UcharVal3 << UcharVal);

		tmp = ( UcharVal % 3) + 1;					// Calculate line number

															// Transmit text line enable
		ErrorCode = TrxAnlTPGTextLineEnable( ndx, tmp, UcharVal3);

		if ( !ErrorCode) 								// Transmit text line string
			ErrorCode = TrxAnlTPGText( ndx, tmp, TextBuffer);

		if ( ErrorCode) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}
															// Select line to edit
	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
				if ( UcharVal-- == UcharVal1 )
					UcharVal = UcharVal2;
		}													// ..button pressed is DOWN
		else {
				if ( UcharVal++ == UcharVal2)
					UcharVal = UcharVal1;
		}													// Copy selected text to temp.
															//  text buffer
		strcpy( TextBuffer, AnlTPGConfig[ndx].Text[UcharVal]);

															// Find text enable information
		UcharVal3 = ( AnlTPGConfig[ndx].TextEnable & ( 1 << UcharVal)) > 0;

		if ( UintVal = strlen( TextBuffer))		// Find position in string
			UintVal--;

		FuncState = 1;
	}
															// Select textline ON/OFF
	if ( FuncTriggers & 0x0C) {					// If UP/DOWN button pressed..

		UcharVal3 ^= ON;								// Toggle selection

		if ( UcharVal3 == ( AnlTPGConfig[ndx].TextEnable & ( 1 << UcharVal)) > 0)
			if ( !strcmp( AnlTPGConfig[ndx].Text[UcharVal], TextBuffer))
				FuncState = 2;
	}
															// Select position in TextBuffer
	if ( FuncTriggers & 0xC0) {					// If LEFT/RIGHT button pressed..

		if ( FuncTriggers & 0x40) {				// If button pressed is LEFT..

			TextBuffer[UintVal] = 0;				// Delete character

			if ( UintVal)                       // Decrement pointer to last
				UintVal--;								//  character
		}
		else {											// ..button pressed is RIGHT
			if ( AnlTPGUnit[ndx].HWType == PT8601)
				tmp = 7;									// Max. 8 characters in PT8601
			else
				tmp = 15;								// Max. 16 characters in PT8631

			if (( UintVal < tmp) && ( UintVal < strlen( TextBuffer)))
				UintVal++;
		}
	}
															// Select character in TextBuffer
	if ( FuncTriggers & 0x30) {					// If UP/DOWN button pressed..

		if ( TextBuffer[UintVal] == 0) {			// If text insert..
															//  set new character = previous
			TextBuffer[UintVal] = TextBuffer[UintVal-1];
			TextBuffer[UintVal+1] = 0;
		}

		TextBuffer[UintVal] = GetNextTextASCII( AnlTPGUnit[ndx].HWType,\
										 ( FuncTriggers & 0x10), TextBuffer[UintVal]);

		if ( !strcmp( AnlTPGConfig[ndx].Text[UcharVal], TextBuffer))
			if ( UcharVal3 == ( AnlTPGConfig[ndx].TextEnable & ( 1 << UcharVal)) > 0)
				FuncState = 8;
	}

	CodePtr = TPGTextEditArrTxt[UcharVal % 3];

	WriteCodeLN2( 0, CodePtr);						// Write header on line 2
	ClearRestLN2();									// Clear rest of line 2

	WriteTxtLN2( 5, ": ");
	WriteTxtLN2( 7, TextBuffer);					// Write edited text on line 2
	WriteCodeLN2( 25, OffOnArrTxt[UcharVal3]);// Write on/off on line 2

	WriteType12_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 4:											// Mark selected item
			WriteItemArrows( 7, strlen( TextBuffer));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 2:
		case 5:											// Mark selected item
			WriteItemArrows( 25, Cstrlen( OffOnArrTxt[UcharVal3]));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 8:
		case 9:											// Mark selected item
			WriteItemBrackets( 7, strlen( TextBuffer));
			CharBlinkLN2( 7 + UintVal, ON);		// Switch OFF blinking at
			break;										//  editing character
	}

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	AnlTPGTextStyleSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980303	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Select the text style in a Analog-TPG.								*/
/*	Remarks:		ANALOG-TPGx/TEXT MENU function.										*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGTextStyleSelect(){

	UC ndx = BaseNdx - AnlTPG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                       // If text style has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new text style
			AnlTPGConfig[ndx].TextStyle[UcharVal1] &= 0xF0;
			AnlTPGConfig[ndx].TextStyle[UcharVal1] |= UcharVal;
		}
		else {											// ..restore text style
			if ( UcharVal != ( AnlTPGConfig[ndx].TextStyle[UcharVal1] & 0x0F))
				if ( ErrorCode = TrxAnlTPGTextStyle( ndx, AnlTPGConfig[ndx].Pattern,
											 AnlTPGConfig[ndx].TextStyle[UcharVal1])) {
					GotoErrorStatus( 256*ndx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( AnlFindPatternType( AnlTPGConfig[ndx].Pattern) == StandardPattern) {
			if ( UcharVal == TPGStandardTextStyle)
				UcharVal = TPGFreeTextStyle;
			else
				UcharVal = TPGStandardTextStyle;
		}
		else {
			if ( FuncTriggers & 0x01) {				// If button pressed is UP..
				if ( UcharVal-- == TPGFreeTextStyle)
					UcharVal = TPGComplexTextStyle;
			}
			else {											// ..button pressed is DOWN
				if ( UcharVal++ == TPGComplexTextStyle)
					UcharVal = TPGFreeTextStyle;
			}
		}
																		// If current style
		if ( UcharVal == ( AnlTPGConfig[ndx].TextStyle[UcharVal1] & 0x0F))
			FuncState = 1;											//  remove SAVE option

																		// Transmit new style
		if ( ErrorCode = TrxAnlTPGTextStyle( ndx, AnlTPGConfig[ndx].Pattern,
				  ( AnlTPGConfig[ndx].TextStyle[UcharVal1] & 0xF0) | UcharVal)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = TPGTextStyleArrTxt[UcharVal];	// Get text for selected level

	WriteCodeLN2( 0, SelectHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	AnlTPGTextPosSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Select the text positioning in a Analog-TPG.						*/
/*	Remarks:		ANL-TPGx/TEXT MENU function.											*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGTextPosSelect(){

	UC patternType, ndx = BaseNdx - AnlTPG2Item;

	patternType = AnlFindPatternType( AnlTPGConfig[ndx].Pattern);

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                       // If text position has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new text positioning
			AnlTPGConfig[ndx].TextPos[patternType][0] = UcharVal;
			AnlTPGConfig[ndx].TextPos[patternType][1] = UcharVal1;
		}
		else {											// ..restore text style
			if (( UcharVal != AnlTPGConfig[ndx].TextPos[patternType][0]) ||
				 ( UcharVal1 != AnlTPGConfig[ndx].TextPos[patternType][1]))

				if ( ErrorCode = TrxAnlTPGTextPos( ndx,
															  AnlTPGConfig[ndx].TextStyle[patternType],
															  UcharVal, UcharVal1)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
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
			if ( UcharVal++ == AnlXPosMax)
				UcharVal = AnlXPosMin;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal-- == AnlXPosMin)
				UcharVal = AnlXPosMax;
		}
															// Save new text positioning
		if (( UcharVal == AnlTPGConfig[ndx].TextPos[patternType][0]) &&
			 ( UcharVal1 == AnlTPGConfig[ndx].TextPos[patternType][1]))
			FuncState = 1;
																		// Transmit new X position
		if ( ErrorCode = TrxAnlTPGTextPos( ndx,
													  AnlTPGConfig[ndx].TextStyle[patternType],
													  UcharVal, UcharVal1)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	if ( FuncTriggers & 0x0C) {			// If button UP/DOWN pressed..
		if ( FuncTriggers & 0x04) {				// If button pressed is UP..
			if ( UcharVal1++ == AnlYPosMax)
				UcharVal1 = AnlYPosMin;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal1-- == AnlYPosMin)
				UcharVal1 = AnlYPosMax;
		}
															// Save new text positioning
		if (( UcharVal == AnlTPGConfig[ndx].TextPos[patternType][0]) &&
			 ( UcharVal1 == AnlTPGConfig[ndx].TextPos[patternType][1]))
			FuncState = 2;
																		// Transmit new Y position
		if ( ErrorCode = TrxAnlTPGTextPos( ndx,
													  AnlTPGConfig[ndx].TextStyle[patternType],
													  UcharVal, UcharVal1)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	WriteTxtLN2( 0, "X: ");							// Write line 2 header
	sprintf( TextBuffer, "%+3d", UcharVal);	// Convert coordinate to ascii
	WriteTxt2( TextBuffer);							// Write coordinate

	ClearToPosLN2( 10);								// Clear to position

	WriteTxt2( "Y: ");                     	// Convert coordinate to ascii
	sprintf( TextBuffer, "%+3d", UcharVal1);
	WriteTxt2( TextBuffer);							// Write coordinate

	ClearRestLN2();									// Clear rest of line 2

	WriteType2_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	switch ( FuncState) {
		case 1:
		case 5:
			WriteItemArrows( 3, 3);						// Mark selected item
			break;

		case 2:
		case 4:												// Mark selected item
			WriteItemArrows( 13, 3);
			break;
	}

	FuncTriggers = 0;										// Reset button-selection
}

/***************************************************************************/
/*	AnlTPGTextClockSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980303	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Select clock information in an Analog-TPG.						*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTPGTextClockSelect(){

	UC ndx = BaseNdx - AnlTPG2Item;

	if ( FuncSave || FuncEscape){
		if ( FuncSave) {                       // If text clock has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new text style
			AnlTPGConfig[ndx].TextStyle[UcharVal1] &= 0x0F;
			AnlTPGConfig[ndx].TextStyle[UcharVal1] |= ( UcharVal << 4);
		}
		else {											// ..restore text clock
			if ( UcharVal != ( AnlTPGConfig[ndx].TextStyle[UcharVal1] >> 4))
				if ( ErrorCode = TrxAnlTPGTextStyle( ndx,
									 AnlTPGConfig[ndx].Pattern,
									 AnlTPGConfig[ndx].TextStyle[UcharVal1])) {
					GotoErrorStatus( 256*ndx + ErrorCode);
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
			if ( UcharVal-- == TPGTextClockNone)
				UcharVal = TPGTextClockTimeDate;

			if ( UcharVal == TPGTextClockDate)
				UcharVal--;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal++ == TPGTextClockTimeDate)
				UcharVal = TPGTextClockNone;

			if ( UcharVal == TPGTextClockDate)
				UcharVal++;
		}

																			// If current clock
		if ( UcharVal == AnlTPGConfig[ndx].TextStyle[UcharVal1] >> 4)
			FuncState = 1;							 					//  remove SAVE option

																		// Transmit new style
		if ( ErrorCode = TrxAnlTPGTextStyle( ndx,
										AnlTPGConfig[ndx].Pattern,
									 ( AnlTPGConfig[ndx].TextStyle[UcharVal1] & 0x0F) | ( UcharVal << 4))) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = TPGTextClockArrTxt[UcharVal];	// Get text for selected clock

	WriteCodeLN2( 0, SelectHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	SDITSGMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the SDI-TSGx menu.							*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITSGMenuShow(){

	register int i, j;
	UC ndx = BaseNdx - SDITSG2Item;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( CoConfigureTxt);					// Write header for line 1
	ClearRestLN1();									// Clear rest of line 1

	SDITSGNdx[ndx] = Ptr->ItemNumber;			// Update SDI-TSGx menu index

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for line 2

	i = j = SDITSGAudioItem;						// Find window

	if ( SDITSGNdx[ndx] < SDITSGAudioItem)		// If item < EMB.Audio..
		i = SDITSGPatternItem;						//  Pattern, System & EDH window
	else                                      // else
		j = SDITSGTimingItem+1;						//  EMB.Audio & Time window

	for ( ; i < j; i++) {
		ClearToPosLN2( SDITSG_menu[i].ItemPos);// Write item position
		WriteCode2( SDITSG_menu[i].ItemTxt);	// Write item text
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write line 1&2 icons/dots
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	SDITSGDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		981126				     													*/
/*																									*/
/*	Function:	Go to the selected item in the SDI-TSGx menu.					*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITSGDown(){

	UC ndx = BaseNdx - SDITSG2Item, itemNdx = SDITSGNdx[ndx];

	if ( Flags.NormalLockOn)						// If normal LOCK is enabled..
		if ( itemNdx != SDITSGPatternItem)		//  only pattern is allowed
			return;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( SDITSG_menu[itemNdx].ItemTxt);// Write headline

														// DO NOT MOVE THE NEXT TWO LINES!!
	PrevPtr = Ptr;                		  	// Store old menu-pointer
	Ptr = &SDITSG_submenu[itemNdx];			// Point to selected item

	switch ( itemNdx) {
		case SDITSGPatternItem:
			WriteCode1( CoSelectTxt);				// Write ', select

			UcharVal = SDITSGConfig[ndx].Pattern;  // Get pattern

			UpdateGrpLEDs( SDIFindGroup( UcharVal));

			FuncState = 1;								// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case SDITSGSystemItem:
			WriteCode1( CoSelectTxt);				// Write ', select'

			UcharVal = SDITSGConfig[ndx].System;	// Get system

			FuncState = 1;								// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case SDITSGEDHItem:
			WriteCode1( CoSelectTxt);				// Write ', select'

			UcharVal = SDITSGConfig[ndx].EDH;	// Get EDH setting

			FuncState = 1;								// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case SDITSGAudioItem:						// Prepare for ../AUDIO menu
			Ptr = &SDITSGaudio_menu[SDITSGAudioNdx[ndx]];
			break;

		case SDITSGTimingItem:
			WriteCode1( CoEditDelayTxt);				// Write ', edit delay'

			UcharVal = SDITSGConfig[ndx].System;	// Get system
			samples = SDITSGConfig[ndx].Delay;		// Get delay

			stepNdx = stepNdxSDIMin;
			stepVal = stepsizeSDITable[stepNdx];	// Get step size

			FuncState = 1;								// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;
	}

	ClearRestLN1();									// Clear rest of line 1
}

/***************************************************************************/
/*	SDITSGPatternSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980222				     													*/
/*																									*/
/*	Function:	Select pattern in a SDI-TSG.											*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:	  TBD																				*/
/***************************************************************************/
void SDITSGPatternSelect(){

	UC ndx = BaseNdx - SDITSG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If pattern has changed..
			ActivePresetNo = 0;						// Clear a possible preset

															// Save new selection
			SDITSGConfig[ndx].Pattern = UcharVal;
		}
		else {											// ..restore SDI-TSGx pattern
			if ( UcharVal != SDITSGConfig[ndx].Pattern)

				UcharVal = SDITSGConfig[ndx].Pattern;

				if ( ErrorCode = TrxSDITSGPattern( ndx, UcharVal)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
					return;
				}
		}

		UpdateGrpLEDs( NoGroup);					// Switch OFF group LEDs

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..

		do {
			if ( FuncTriggers & 0x01) {         // If next pattern..
				if ( UcharVal-- == SDISMPTECBar)
					UcharVal = SDIVMT01;
			}
			else {										// ..else previous pattern
				if ( UcharVal++ == SDIVMT01)
					UcharVal = SDISMPTECBar;
			}
		} while( UcharVal != SDIValidatePattern( SDITSGUnit[ndx].HWType, ndx,\
													 SDITSGConfig[ndx].System, UcharVal));

		UpdateGrpLEDs( SDIFindGroup( UcharVal));

		if ( UcharVal == SDITSGConfig[ndx].Pattern)	// If current pattern..
			FuncState = 1;										//  remove SAVE option

															// Transmit pattern selection
		if ( ErrorCode = TrxSDITSGPattern( ndx, UcharVal)) {
			GotoErrorStatus( 256*SDITSGNdx[ndx] + ErrorCode);
			return;
		}
	}

	CodePtr = SDIPattArrTxt[UcharVal];			// Get text for selected pattern

	WriteCharLN2( 0, ' ');							// NO header for line 2
	WriteCode2( CodePtr);							// Write the selected pattern

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC texct

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 1, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	SDITSGSystemSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980504				     													*/
/*																									*/
/*	Function:	Select TV system in a SDI-TSG											*/
/*	Remarks:		SDI-TSGx SUBMENU function.												*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITSGSystemSelect(){

	UC ndx = BaseNdx - SDITSG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {									// If system has changed..
			ActivePresetNo = 0;							// Clear possible preset

			SDITSGConfig[ndx].System = UcharVal;	// Save new SDI-TSG pattern

																// Test if pattern/delay is
																//  valid in new system
			SDIValidateSetting( SDITSGUnit[ndx].HWType, ndx);

																// Transmit the new system
			if ( ErrorCode = TrxSDITSGSystem( ndx, UcharVal, SDITSGConfig[ndx].Delay,\
															SDITSGConfig[ndx].Pattern)) {
				GotoErrorStatus( 256*ndx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal == SDI625)					//  toggle TV system
			UcharVal = SDI525;
		else
			UcharVal = SDI625;

		if ( UcharVal == SDITSGConfig[ndx].System)	// If current system..
			FuncState = 1;										//  remove SAVE option
	}

	CodePtr = SDISystemArrTxt[UcharVal];				// Get text for selection

	WriteCodeLN2( 0, SystemHdrTxt);						// Write header on line 2
	WriteCode2( CodePtr);									// Write text on line 2

	ClearRestLN2();											// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);						// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));		// Mark selected item

	FuncTriggers = 0;											// Reset button selectors
}

/***************************************************************************/
/*	SDITSGEDHSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Swith EDH on/off in a SDI-TSG											*/
/*	Remarks:		SDI-TSGx SUBMENU function.												*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITSGEDHSelect(){

	UC ndx = BaseNdx - SDITSG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If EDH has changed..
			ActivePresetNo = 0;						// Clear possible preset

			SDITSGConfig[ndx].EDH = UcharVal;	// Save new EDH

															// Transmit new EDH setting
			if ( ErrorCode = TrxSDITSGEDH( ndx, UcharVal)) {
				GotoErrorStatus( 256*ndx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal == ON)							//  toggle EDH insertion on/off
			UcharVal = OFF;
		else
			UcharVal = ON;

		if ( UcharVal == SDITSGConfig[ndx].EDH)	// If current EDH..
			FuncState = 1;									//  remove SAVE option
	}

	CodePtr = OffOnArrTxt[UcharVal];					// Get text for EDH selection

	WriteCodeLN2( 0, EDHHdrTxt);						// Write line 2 header
	WriteCode2( CodePtr);                  		// Write text for selection

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 15, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset button selectors
}

/***************************************************************************/
/*	SDITSGTimingSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Set the delay for a SDI-TSG.											*/
/*	Remarks:		SDI-TSGx SUBMENU function.												*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITSGTimingSelect(){

	UC ndx = BaseNdx - SDITSG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If delay has changed..
			ActivePresetNo = 0;						//  Clear possible preset

			SDITSGConfig[ndx].Delay = samples;	// Save new delay
		}
															// Transmit delay to SDI-TSGn
															// MUST be .Delay
		TrxSDITSGDelay( ndx, UcharVal, SDITSGConfig[ndx].Delay);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Adjust timing
	SDITimeAdjust( BaseNdx, UcharVal, SDITSGConfig[ndx].Delay);

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/*	SDITSGAudioMenuShow															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the SDI-TSGx/EMB.AUDIO submenu.			*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITSGAudioMenuShow(){

	register int i;
	UC ndx = BaseNdx - SDITSG2Item;

	WriteCodeLN1( 0, SubmnuHdrTxt);				// Write sub-menu headline
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');

	WriteCode1( SDITSG_menu[SDITSGNdx[ndx]].ItemTxt);

	WriteCode1( CoSelectTxt);
	ClearRestLN1();									// Clear rest of line 1

	SDITSGAudioNdx[ndx] = Ptr->ItemNumber;		// Update ../EMB.AUDIO menu index

	WriteCodeLN2( 0, SecondmnuHdrTxt);			// Write line 2 header

	for ( i = SDITSGAudioSignalItem; i <= SDITSGAudioLevelItem; i++) {
		ClearToPosLN2( SDITSGaudio_menu[i].ItemPos);	// Find item column position
		WriteCode2( SDITSGaudio_menu[i].ItemTxt);		// Write item texts
	}

	ClearRestLN2();										// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);				// Write line 1&2 icons/dots
																// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	SDITSGAudioDown																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Go to the selected item in the SDI-TSGx/EMB.AUDIO submenu.	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITSGAudioDown(){

	UC ndx = BaseNdx - SDITSG2Item;

	WriteCodeLN1( 0, SecondmnuHdrTxt);					// Write menu headline
	WriteCode1( SlashDotHdrTxt);
	WriteCode1( SDITSG_menu[SDITSGNdx[ndx]].ItemTxt);
	WriteChar1( '/');
	WriteCode1( SDITSGaudio_menu[SDITSGAudioNdx[ndx]].ItemTxt);
	WriteCode1( CoSelectTxt);

	switch ( SDITSGAudioNdx[ndx]) {
		case SDITSGAudioSignalItem:
			UcharVal = SDITSGConfig[ndx].AudioSignal;		// Get audio signal
			break;

		case SDITSGAudioLevelItem:
			UcharVal = SDITSGConfig[ndx].AudioLevel;		// Get audio level
			break;
	}

	ClearRestLN1();											// Clear rest of line 1

	PrevPtr = Ptr;												// Store old menu-pointer
	Ptr = &SDITSGaudio_submenu[SDITSGAudioNdx[ndx]];// Point to selected item

	FuncState = 1;									// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;							// Set select-mode
}

/***************************************************************************/
/* UpToSDITSG																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Go to the SDI-TSGx MENU and restore the last selected	item	*/
/*	Remarks:		SDI-TSGx MENU function													*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToSDITSG(){
														// Restore selected SDI-TSGx unit
	Ptr = &SDITSG_menu[SDITSGNdx[BaseNdx-SDITSG2Item]];
}

/***************************************************************************/
/*	SDITSGAudioSignalSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980505				     													*/
/*																									*/
/*	Function:	Select embedded audio signal in a SDI-TSG.						*/
/*	Remarks:		SDI-TSGx 2ND MENU function.											*/
/*					Instant update implemented.											*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																				*/
/***************************************************************************/
void SDITSGAudioSignalSelect(){

	UC ndx = BaseNdx - SDITSG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                   		// If audio signal has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new audio selection
			SDITSGConfig[ndx].AudioSignal = UcharVal;
		}
		else {											// ..restore audio signal
			if ( UcharVal != SDITSGConfig[ndx].AudioSignal)
				if ( ErrorCode = TrxSDITSGAudioSignal( ndx, SDITSGConfig[ndx].AudioSignal)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
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
			if ( UcharVal-- == TSGAudioOff)
				UcharVal = TSGAudioStereo1kHz;
		}
		else												// ..button pressed is DOWN
			if ( UcharVal++ == TSGAudioStereo1kHz)
				UcharVal = TSGAudioOff;

		if ( UcharVal == SDITSGConfig[ndx].AudioSignal)	// If current audio signal..
			FuncState = 1;											//  remove SAVE option

																		// Transmit audio signal
		if ( ErrorCode = TrxSDITSGAudioSignal( ndx, UcharVal)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = TSGAudioSignalArrTxt[UcharVal];		// Get text for selected
																//	audio signal

	WriteCodeLN2( 0, SignalHdrTxt);					// Write header for line 2
	WriteCode2( CodePtr);								// Write selected text

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/*	SDITSGAudioLevelSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980505				     													*/
/*																									*/
/*	Function:	Select embedded audio level in a SDI-TSG.							*/
/*	Remarks:		SDI-TSGx 2NDMENU function.												*/
/*					Instant update implemented.											*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITSGAudioLevelSelect(){

	UC ndx = BaseNdx - SDITSG2Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                       // If audio level has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new audio level
			SDITSGConfig[ndx].AudioLevel = UcharVal;
		}
		else {											// ..restore audio level
			if ( UcharVal != SDITSGConfig[ndx].AudioLevel)
				if ( ErrorCode = TrxSDITSGAudioLevel( ndx, SDITSGConfig[ndx].AudioLevel)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03)   // If UP/DOWN button pressed..
		{													// Note! Only silence is available in PT8609
		if ( SDITSGUnit[ndx].HWType == PT8639)
      {
			if ( FuncTriggers & 0x01)
        {			                  // If button pressed is UP..
				if ( UcharVal-- == TSGAudioSilence)
					UcharVal = TSGAudioLevel18dB;
			  }
			else											// ..button pressed is DOWN
				if ( UcharVal++ == TSGAudioLevel18dB)
					UcharVal = TSGAudioSilence;
  		}

		if (UcharVal == SDITSGConfig[ndx].AudioLevel) // If current audio level..
			FuncState = 1;										  //  remove SAVE option
																	  // Transmit audio level
		if ( ErrorCode = TrxSDITSGAudioLevel( ndx, UcharVal)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = TSGAudioLevelArrTxt[UcharVal];		// Get text selection audio
																//  level

	WriteCodeLN2( 0, LevelHdrTxt);					// Write line 2 header
	WriteCode2( CodePtr);								// Write selected text

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 7, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/* SDITPGMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the SDI-TPGx menu.							*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGMenuShow(){

	register int i, j;
	UC ndx = BaseNdx - SDITPG1Item;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( CoConfigureTxt);						// Write header for line 1
	ClearRestLN1();										// Clear rest of line 1

	SDITPGNdx[ndx] = Ptr->ItemNumber;				// Update SDI-TPGx menu index

	WriteCodeLN2( 0, SubmnuHdrTxt);					// Write header for line 2

	i = j = SDITPGEDHItem;								// Find window

	if ( SDITPGNdx[ndx] < SDITPGEDHItem)			// If item < EDH..
		i = SDITPGPatternItem;							//  Pattern, Text & System
	else                                     	 	// else
		j = SDITPGTimingItem+1;							//  EDH, EMB.Audio & Timing

	for ( ; i < j; i++) {
		ClearToPosLN2( SDITPG_menu[i].ItemPos);	// Write item position
		WriteCode2( SDITPG_menu[i].ItemTxt);		// Write item text
	}

	ClearRestLN2();										// Clear rest of line 2

	i = Ptr->ItemConfig;									// Get line 1&2 icons/dots

	if ( Flags.NormalLockOn) 							// If normal LOCK is enabled..
		if ( SDITPGNdx[ndx] != SDITPGPatternItem)	// If not pattern menu..
			i = ( i | P_) & ~D_; 						//  add padlock, remove down
																//  arrow.

	WriteArrowsField( i);								// Write line 1&2 icons/dots
																// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* SDITPGDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		981126				     													*/
/*																									*/
/*	Function:	Go to the selected item in the SDI-TPGx menu.					*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGDown(){

	UC ndx = BaseNdx - SDITPG1Item, itemNdx = SDITPGNdx[ndx];

	if ( Flags.NormalLockOn)						// If normal LOCK is enabled..
		if ( itemNdx != SDITPGPatternItem)		// If not pattern menu..
			return;                             //  return immediately

	WriteCodeLN1( 0, SubmnuHdrTxt);				// Write submenu headline
	WriteCode1( base_menu[BaseNdx].ItemTxt);	//  on line 1
	WriteChar1( '/');
	WriteCode1( SDITPG_menu[itemNdx].ItemTxt);
															// DO NOT MOVE NEXT TWO LINES!!
	PrevPtr = Ptr;                		  		// Store old menu-pointer
	Ptr = &SDITPG_submenu[itemNdx];				// Point to selected item

	switch ( itemNdx) {
		case SDITPGPatternItem:						// Prepare for pattern selection
			WriteCode1( CoSelectTxt);				// Write additional line 1 text

			UcharVal = SDITPGConfig[ndx].Pattern;	// Get pattern
																// Determine patterntype
			UcharVal1 = SDIFindPatternType( UcharVal);
																// Switch on group LED
			UpdateGrpLEDs( SDIFindGroup( UcharVal));

			FuncState = 1;								// Prepare for item statemachine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case SDITPGTextItem:							// Prepare for ../TEXT menu
			Ptr = &SDITPGtext_menu[SDITPGTextNdx[ndx]];
			break;

		case SDITPGSystemItem:                 // Prepare for system selection
			WriteCode1( CoSelectTxt);				// Write additional line 1 text

			UcharVal = SDITPGConfig[ndx].System;// Get system

			FuncState = 1;								// Prepare for item statemachine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case SDITPGEDHItem:                    // Prepare for EDH selection
			WriteCode1( CoSelectTxt);				// Write additional line 1 text

			UcharVal = SDITPGConfig[ndx].EDH;	// Get EDH setting

			FuncState = 1;								// Prepare for item statemachine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;

		case SDITPGAudioItem:						// Prepare for ../AUDIO MENU
			Ptr = &SDITPGaudio_menu[SDITPGAudioNdx[ndx]];
			break;

		case SDITPGTimingItem:						// Prepare for timing selection
			WriteCode1( CoEditDelayTxt);			// Write additional line 1 text

			UcharVal = SDITPGConfig[ndx].System;// Get system
			samples = SDITPGConfig[ndx].Delay;	// Get delay

			stepNdx = stepNdxSDIMin;
			stepVal = stepsizeSDITable[stepNdx];// Get step size

			FuncState = 1;								// Prepare for item statemachine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;						// Set select-mode
			break;
	}

	ClearRestLN1();									// Clear rest of line 1
}

/***************************************************************************/
/* SDITPGPatternSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Select pattern in a SDI-TPG.											*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*					MODIFY is only available when a complex pattern has been		*/
/*					selected. PT8602/03 do NOT contain any complex patterns,		*/
/*					hence this field will never be displayed.							*/
/*	Returns:		--																				*/
/*	Updates:	   TBD																			*/
/***************************************************************************/
void SDITPGPatternSelect(){

	UC ndx = BaseNdx - SDITPG1Item;

	if ( FuncState == 13) {						// If MODIFY item selected..

		WriteCodeLN1( 0, SecondmnuHdrTxt);	// Write menu header on line 1
		WriteCode1( SlashDotHdrTxt);
		WriteCode1( MODIFYTxt);
		WriteTxt1( ", ");
		CodePtr = SDIPattArrTxt[UcharVal];	// Get text for selected pattern
		WriteCode1( CodePtr);               // Write pattern text on line 1
		ClearRestLN1();							// Clear rest of line 1

														// Get pattern attribute
		UcharPtr = &SDITPGConfig[ndx].Attrib[UcharVal-SDIPhilips4x3];
		UcharVal2 = *UcharPtr;
														// Find the modifications available
														//  to the current pattern
		if ( SDITPGConfig[ndx].System == SDI525)
			UcharVal3 = AttribPLUGE;
		else
			UcharVal3 = AttribAntiPAL;

		FuncState = 1;								// Prepare for item statemachine
		FuncTriggers = 0;
		FuncSave = FuncEscape = FALSE;

		Ptr = &SDITPGpattern_menu;	  			// Set pointer to MODIFY menu
		Ptr->DispFunct();						 	// Display next menu
		return;
	}

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If pattern has changed..
			ActivePresetNo = 0;						// Clear a possible preset
															// Save new pattern
			SDITPGConfig[ndx].Pattern = UcharVal;
		}
		else {											// ..restore SDI-TPGx pattern
			if ( UcharVal != SDITPGConfig[ndx].Pattern) {
				UcharVal = SDITPGConfig[ndx].Pattern;

				ErrorCode = TrxSDITPGPattern( ndx, UcharVal, SDITPGAttrib( ndx, UcharVal));

				if ( !ErrorCode)
					ErrorCode = ConfigureSDITPGText( ndx, UcharVal);

				if ( ErrorCode) {
					GotoErrorStatus( 256*ndx + ErrorCode);
					return;
				}
			}
		}

		UpdateGrpLEDs( NoGroup);					// Switch OFF group LEDs

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..

		do {
			if ( FuncTriggers & 0x01) {         // If next pattern..
				if ( UcharVal-- == SDISMPTECBar)
					UcharVal = SDIVMT01;
			}
			else {										// ..else previous pattern
				if ( UcharVal++ == SDIVMT01)
					UcharVal = SDISMPTECBar;
			}
		} while( UcharVal != SDIValidatePattern( SDITPGUnit[ndx].HWType, ndx,\
													 SDITPGConfig[ndx].System, UcharVal));

		UpdateGrpLEDs( SDIFindGroup( UcharVal));

		if ( UcharVal == SDITPGConfig[ndx].Pattern) {	// If current pattern..
			if ( FuncState > 5)									//  remove SAVE option
				FuncState = 6;
			else
				FuncState = 1;
		}
																// Transmit pattern selection
		ErrorCode = TrxSDITPGPattern( ndx, UcharVal, SDITPGAttrib( ndx, UcharVal));

		if ( !ErrorCode)
			ErrorCode = ConfigureSDITPGText( ndx, UcharVal);

		if ( ErrorCode) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	if (( UcharVal >= SDIPhilips4x3) && ( UcharVal <= SDIFuBK16x9)) {
		if ( FuncState == 1)
			FuncState = 6;

		if ( FuncState == 3)
			FuncState = 9;
	}
	else {
		if ( FuncState == 6)
			FuncState = 1;

		if ( FuncState == 9)
			FuncState = 3;
	}

	CodePtr = SDIPattArrTxt[UcharVal];			// Get text for selected pattern

	WriteCharLN2( 0, ' '	);							// No header for line 2
	WriteCode2( CodePtr);							// Write the selected pattern

	ClearRestLN2();									// Clear rest of line 2

	WriteType7_SAVE_ESC( FuncState);				// Write the SAVE ESC texct

	switch ( FuncState ) {
		case 1:
		case 3:
		case 6:
		case 9:											// Mark selected item
			WriteItemArrows( 1, Cstrlen( CodePtr));
			break;
	}

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/* SDITPGPatternModify															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Modify selected test-pattern in a SDI-TPG.						*/
/*	Remarks:		SDI-TPGx SUBMENU function.												*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:	   TBD																			*/
/***************************************************************************/
void SDITPGPatternModify(){

	UC tmp, ndx = BaseNdx - SDITPG1Item;
	UC bitPos;

	if ( FuncEscape)
    {	             // Restore pattern attribute
		if ( UcharVal2 != *UcharPtr)				//  if not saved
			if ( ErrorCode = TrxSDITPGPattern( ndx, UcharVal, *UcharPtr))
        {
				GotoErrorStatus( 256*ndx + ErrorCode);
				return;
			  }
		WriteCodeLN1( 0, SubmnuHdrTxt);				 // Write previous submenu
		WriteCode1( base_menu[BaseNdx].ItemTxt);	 //  on line 1
		WriteChar1( '/');
		WriteCode1( SDITPG_menu[SDITPGNdx[ndx]].ItemTxt);
		WriteCode1( CoSelectTxt);						 // Write additional line 1 text
		if ( UcharVal == SDITPGConfig[ndx].Pattern)// If current pattern..
			FuncState = 7;                          //  remove SAVE option
		else
			FuncState = 10;

		FuncTriggers = 0;
		FuncSave = FuncEscape = FALSE;

		Ptr = &SDITPG_submenu[SDITPGNdx[ndx]];		 // Point to previous item
		Ptr->DispFunct();									 // Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..

		do {
			if ( FuncTriggers & 0x01) {         // If next attribute..
				if ( UcharVal3-- == AttribAntiPAL)
					UcharVal3 = AttribCircles;
			}
			else {										// ..else previous attribute
				if ( UcharVal3++ == AttribCircles)
					UcharVal3 = AttribAntiPAL;
			}
		} while( UcharVal3 != SDIValidateAttrib( SDITPGUnit[ndx].HWType, ndx,\
									 SDITPGConfig[ndx].System, UcharVal, UcharVal3));
	}

	bitPos = 1 << UcharVal3;						// Find attribute bit position

	if ( FuncSave) {									// If SAVE executed
		FuncSave = FALSE;

		*UcharPtr &= ~bitPos;						// Clear present attribute
		*UcharPtr |= ( bitPos & UcharVal2);    // Save new attribute
	}

	if ( FuncTriggers & 0x0C) {					// If button UP/DOWN pressed..
		UcharVal2 ^= bitPos;							//  toggle attribute selection

		if ( !( bitPos & ( UcharVal2^*UcharPtr)))
			FuncState = 2;
															// Transmit pattern selection
		if ( ErrorCode = TrxSDITPGPattern( ndx, UcharVal, UcharVal2)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	if ( FuncTriggers & 0x03) {					// DO NOT MOVE TO THE ONE ABOVE
		if ( !( bitPos & ( UcharVal2^*UcharPtr)))
			FuncState = 1;
		else
			FuncState = 5;
	}

	tmp = (( bitPos & UcharVal2) == bitPos);		// Attribute state

	WriteCodeLN2( 0, ModifyHdrTxt);					// Write line 2 header
	ClearRestLN2();

	WriteType2_SAVE_ESC( FuncState);					// Write the ESC text

	CodePtr = AttribArrTxt[UcharVal3];				// Get attribute text

	WriteCodeLN2( 8, CodePtr);							// Write attribute
	WriteCodeLN2( 24, OffOnArrTxt[tmp]);			// Wtite attribute state

	switch ( FuncState) {
		case 1:
		case 5:
			WriteItemArrows( 8, Cstrlen( CodePtr));// Mark selected item
			break;

		case 2:
		case 4:												// Mark selected item
			WriteItemArrows( 24, Cstrlen( OffOnArrTxt[tmp]));
			break;
	}

	FuncTriggers = 0;										// Reset button-selection
}

/***************************************************************************/
/* SDITPGSystemSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980304				     													*/
/*																									*/
/*	Function:	Select TV system for a SDI-TPG.										*/
/*	Remarks:		SDI-TPGx MENU function.													*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGSystemSelect(){

	UC pattern, ndx = BaseNdx - SDITPG1Item;

	if ( FuncSave || FuncEscape)
    {
		if ( FuncSave)
      {									// If system has changed..
			ActivePresetNo = 0;							// Clear possible preset

			SDITPGConfig[ndx].System = UcharVal;	// Save new system

															// Test if delay/pattern is valid
															//  in the new system, (AFTER
															//  the system is saved)
			SDIValidateSetting( SDITPGUnit[ndx].HWType, ndx);
															// Get validated pattern
			pattern = SDITPGConfig[ndx].Pattern;

			if ( ErrorCode = TrxSDITPGSystem( ndx, UcharVal, SDITPGConfig[ndx].Delay,\
												  pattern, SDITPGAttrib( ndx, pattern)))
        {
				GotoErrorStatus( 256*ndx + ErrorCode);
				return;
			  }
		  }
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	  }

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal == SDI625)					//  toggle TV system
			UcharVal = SDI525;
		else
			UcharVal = SDI625;

		if ( UcharVal == SDITPGConfig[ndx].System)	// If current system..
			FuncState = 1;										//  remove SAVE option
	}

	CodePtr = SDISystemArrTxt[UcharVal];			// Get text for selection

	WriteCodeLN2( 0, SystemHdrTxt);					// Write header on line 2
	WriteCode2( CodePtr);								// Write text on line 2

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset button selectors
}

/***************************************************************************/
/* SDITPGEDHSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980304				     													*/
/*																									*/
/*	Function:	Set EDH insertion on/off in a SDI-TPG.								*/
/*	Remarks:		SDI-TPGx SUBMENU function.												*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGEDHSelect(){

	UC ndx = BaseNdx - SDITPG1Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {									// If EDH has changed..
			ActivePresetNo = 0;							// Clear possible preset

			SDITPGConfig[ndx].EDH = UcharVal;		// Save new EDH

																// Transmit new EDH setting
			if ( ErrorCode = TrxSDITPGEDH( ndx, UcharVal)) {
				GotoErrorStatus( 256*ndx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal == ON)							//  toggle EDH insertion on/off
			UcharVal = OFF;
		else
			UcharVal = ON;

		if ( UcharVal == SDITPGConfig[ndx].EDH)	// If current EDH..
			FuncState = 1;									//  remove SAVE option
	}

	CodePtr = OffOnArrTxt[UcharVal];					// Get text for EDH selection

	WriteCodeLN2( 0, EDHHdrTxt);						// Write line 2 header
	WriteCode2( CodePtr);                  		// Write text for selection
	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 15, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset button selectors
}

/***************************************************************************/
/* SDITPGTimingSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Set timing for a SDI-TPG.												*/
/*	Remarks:		SDI-TPGx SUBMENU function.												*/
/*					Cursor position on line 2 is updated								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGTimingSelect(){

	UC ndx = BaseNdx - SDITPG1Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If delay has changed..
			ActivePresetNo = 0;						// Clear possible preset

			SDITPGConfig[ndx].Delay = samples;	// Save new delay
		}
															// MUST be .Delay
		TrxSDITPGDelay( ndx, UcharVal, SDITPGConfig[ndx].Delay);

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Adjust timing
	SDITimeAdjust( BaseNdx, UcharVal, SDITPGConfig[ndx].Delay);

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/* UpToSDITPG																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980304				     													*/
/*																									*/
/*	Function:	Go to the SDI-TPGx MENU and restore the last selected item.	*/
/*	Remarks:		SDI-TPGx SUBMENU function.												*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToSDITPG(){
														// Restore selected SDI-TPGx unit
	Ptr = &SDITPG_menu[SDITPGNdx[BaseNdx-SDITPG1Item]];
}

/***************************************************************************/
/*	SDITPGTextMenuShow															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		000510				     													*/
/*																									*/
/*	Function:	Display the items in the SDI-TPGx/TEXT submenu.		  			*/
/*	Remarks:		This submenu will have 3 or 4 items depending on the PT8637.*/
/*					The PT8602/03 only supports the EDIT item.						*/
/*					Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGTextMenuShow(){

	UC patternType, tmp, ndx = BaseNdx - SDITPG1Item;
	register int i;

	WriteCodeLN1( 0, SubmnuHdrTxt);         	 	 // Write submenu header on
	WriteCode1( base_menu[BaseNdx].ItemTxt);		 //  line 1
	WriteChar1( '/');
	WriteCode1( SDITPG_menu[SDITPGNdx[ndx]].ItemTxt);
	WriteCode1( CoConfigureTxt);
	ClearRestLN1();										// Clear rest of line 1

	SDITPGTextNdx[ndx] = Ptr->ItemNumber;			// Update ../TEXT menu index

	WriteCodeLN2( 0, SecondmnuHdrTxt);				// Write line 2 header

	tmp = TPGTextPosItem;

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637))
		tmp += ( SDITPGUnit[ndx].HWType > PT8631);

	for ( i = TPGTextEditItem; i <= tmp; i++) {
		ClearToPosLN2( SDITPGtext_menu[i].ItemPos); // Find item column position
		WriteCode2( SDITPGtext_menu[i].ItemTxt);	  // Write item texts
	}

	ClearRestLN2();										// Clear rest of line 2

	i = Ptr->ItemConfig;									// Get line 1&2 icons/dots

	switch ( SDITPGUnit[ndx].HWType) {				// Determing HW type
		case PT8602:                              // Only text editing is
		case PT8603:										//  available in PT8602/03
			if ( SDITPGTextNdx[ndx] != TPGTextEditItem)
				i &= ~D_;
			break;

		default:
			patternType = SDIFindPatternType( SDITPGConfig[ndx].Pattern);

			if ( SDITPGTextNdx[ndx] == TPGTextPosItem)
				if (( SDITPGConfig[ndx].TextStyle[patternType] & 0x0F) != TPGFreeTextStyle)
					i &= ~D_;								// Textpositioning is only allowed
			break;											//  for the free text style
	}
	WriteArrowsField( i);								// Write line 1&2 icons/dots
																// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	SDITPGTextDown																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		000510				     													*/
/*																									*/
/*	Function:	Go to the selected item in the SDI-TPGx/TEXT submenu.			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGTextDown(){

	UC patternType, ndx = BaseNdx - SDITPG1Item, itemNdx = SDITPGTextNdx[ndx];

	patternType = SDIFindPatternType( SDITPGConfig[ndx].Pattern);

	if ( SDITPGUnit[ndx].HWType < PT8632) {
		if ( itemNdx != TPGTextEditItem)		 	   // Only text editing is
			return;                  				   //  available in PT8602/03
	}
	else {
		if ( SDITPGTextNdx[ndx] == TPGTextPosItem)
			if (( SDITPGConfig[ndx].TextStyle[patternType] & 0x0F) != TPGFreeTextStyle)
				return;
	}

	WriteCodeLN1( 0, SecondmnuHdrTxt);				// Write second menu header
	WriteCode1( SlashDotHdrTxt);						//  on line 1
	WriteCode1( SDITPGtext_menu[itemNdx].ItemTxt);

	if ( patternType == ComplexPattern)
		CodePtr = CoComplexPatternTxt;
	else
		CodePtr = CoStandardPatternTxt;

	WriteCode1( CodePtr);
	ClearRestLN1();										// Clear rest of line 1

	switch ( itemNdx) {
		case TPGTextEditItem:       		   		// Prepare for text editing
			if ( SDITPGUnit[ndx].HWType < PT8632) {
				UcharVal1 = UcharVal2 = 0;         	// Only one text line is
			}													//  available in PT8602/03
			else {
				if ( patternType == ComplexPattern) {
					UcharVal1 = 3;
					UcharVal2 = 4;
				}
				else {
					UcharVal1 = 0;
					UcharVal2 = 2;
				}
			}
			UcharVal = UcharVal1;
														// Copy string to temp. buffer
			strcpy( TextBuffer, SDITPGConfig[ndx].Text[UcharVal]);

														// Find text enable information
			UcharVal3 = ( SDITPGConfig[ndx].TextEnable & ( 1 << UcharVal)) > 0;

			if ( UintVal = strlen( TextBuffer))	// Position in Text string
				UintVal--;
			break;

		case TPGTextStyleItem:					// Prepare for style selection
			UcharVal1 = patternType;
														// Mask out style
			UcharVal = SDITPGConfig[ndx].TextStyle[UcharVal1] & 0x0F;
			break;

		case TPGTextPosItem:						// Prepare for position selection
			UcharVal = SDITPGConfig[ndx].TextPos[patternType][0];
			UcharVal1 = SDITPGConfig[ndx].TextPos[patternType][1];
			break;

		case TPGTextClockItem:              // Prepare for date/time selection
			UcharVal1 = patternType;
														// Mask out date/time
			UcharVal = SDITPGConfig[ndx].TextStyle[UcharVal1] >> 4;
			break;
	}

	PrevPtr = Ptr;									// Store old menu-pointer
														// Point to selected item
	Ptr = &SDITPGtext_submenu[itemNdx];

	FuncState = 1;									// Prepare for item statemachine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;							// Set select-mode
}

/***************************************************************************/
/*	SDITPGTextEditSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Edit a text in a SDI-TPG.												*/
/*	Remarks:		SDI-TPGx/TEXT MENU function.											*/
/*					Cursor position on line 2 is updated.								*/
/*					UcharVal 		LineNumber 0-4											*/
/*					UcharVal1		Min. LineNumber                              */
/*					UcharVal2		Max. LineNumber                              */
/*					UcharVal3      Text Enable ON/OFF                           */
/*					UintVal			Position in text buffer								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGTextEditSelect(){

	UC tmp, ndx = BaseNdx - SDITPG1Item;

	if ( FuncEscape) {
		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncSave) {								  	// If textinfo has changed..
		FuncSave = FALSE;
		ActivePresetNo = 0;						  	// Clear possible preset
															// Save the new text
		strcpy( SDITPGConfig[ndx].Text[UcharVal], TextBuffer);

															// Save text enable information
		SDITPGConfig[ndx].TextEnable &= ~( 1 << UcharVal);
		SDITPGConfig[ndx].TextEnable |= ( UcharVal3 << UcharVal);

		tmp = ( UcharVal % 3) + 1;					// Calculate line number

															// Transmit text line enable
		ErrorCode = TrxSDITPGTextLineEnable( ndx, tmp, UcharVal3);

		if ( !ErrorCode) 								// Transmit text line string
			ErrorCode = TrxSDITPGText( ndx, tmp, TextBuffer);

		if ( ErrorCode) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}
															// Select line to edit
	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
				if ( UcharVal-- == UcharVal1 )
					UcharVal = UcharVal2;
		}													// ..button pressed is DOWN
		else {
				if ( UcharVal++ == UcharVal2)
					UcharVal = UcharVal1;
		}													// Copy selected text to temp.
															//  text buffer
		strcpy( TextBuffer, SDITPGConfig[ndx].Text[UcharVal]);

															// Find text enable information
		UcharVal3 = ( SDITPGConfig[ndx].TextEnable & ( 1 << UcharVal)) > 0;

		if ( UintVal = strlen( TextBuffer))		// Find position in string
			UintVal--;

		FuncState = 1;
	}
															// Select textline ON/OFF
	if ( FuncTriggers & 0x0C) {					// If UP/DOWN button pressed..

		UcharVal3 ^= ON;								// Toggle selection

		if ( UcharVal3 == ( SDITPGConfig[ndx].TextEnable & ( 1 << UcharVal)) > 0)
			if ( !strcmp( SDITPGConfig[ndx].Text[UcharVal], TextBuffer))
				FuncState = 2;
	}
															// Select position in TextBuffer
	if ( FuncTriggers & 0xC0) {					// If LEFT/RIGHT button pressed..

		if ( FuncTriggers & 0x40) {				// If button pressed is LEFT..

			TextBuffer[UintVal] = 0;				// Delete character

			if ( UintVal)                       // Decrement pointer to last
				UintVal--;								//  character
		}
		else {											// ..button pressed is RIGHT
			if ( SDITPGUnit[ndx].HWType < PT8632)
				tmp = 7;									// Max. 8 characters in PT8602/3
			else
				tmp = 15;								// Max. 16 characters in PT8632/3

			if (( UintVal < tmp) && ( UintVal < strlen( TextBuffer)))
				UintVal++;
		}
	}
															// Select character in TextBuffer
	if ( FuncTriggers & 0x30) {					// If UP/DOWN button pressed..
		if ( TextBuffer[UintVal] == 0) {			// If text insert..
															//  set new character = previous
			TextBuffer[UintVal] = TextBuffer[UintVal-1];
			TextBuffer[UintVal+1] = 0;
		}

		TextBuffer[UintVal] = GetNextTextASCII( SDITPGUnit[ndx].HWType,\
										 ( FuncTriggers & 0x10), TextBuffer[UintVal]);

		if ( !strcmp( SDITPGConfig[ndx].Text[UcharVal], TextBuffer))
			if ( UcharVal3 == ( SDITPGConfig[ndx].TextEnable & ( 1 << UcharVal)) > 0)
				FuncState = 8;
	}

	CodePtr = TPGTextEditArrTxt[UcharVal % 3];

	WriteCodeLN2( 0, CodePtr);						// Write header on line 2
	ClearRestLN2();									// Clear rest of line 2

	WriteTxtLN2( 5, ": ");
	WriteTxtLN2( 7, TextBuffer);					// Write edited text on line 2
	WriteCodeLN2( 25, OffOnArrTxt[UcharVal3]);// Write on/off on line 2

	WriteType12_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 4:											// Mark selected item
			WriteItemArrows( 7, strlen( TextBuffer));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 2:
		case 5:											// Mark selected item
			WriteItemArrows( 25, Cstrlen( OffOnArrTxt[UcharVal3]));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 8:
		case 9:											// Mark selected item
			WriteItemBrackets( 7, strlen( TextBuffer));
			CharBlinkLN2( 7 + UintVal, ON);		// Switch OFF blinking at
			break;										//  editing character
	}

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	SDITPGTextStyleSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Select the text style in a SDI-TPG.									*/
/*	Remarks:		SDI-TPGx/TEXT MENU function.											*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGTextStyleSelect(){

	UC ndx = BaseNdx - SDITPG1Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                       // If text style has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new text style
			SDITPGConfig[ndx].TextStyle[UcharVal1] &= 0xF0;
			SDITPGConfig[ndx].TextStyle[UcharVal1] |= UcharVal;
		}
		else {											// ..restore text style
			if ( UcharVal != ( SDITPGConfig[ndx].TextStyle[UcharVal1] & 0x0F))
				if ( ErrorCode = TrxSDITPGTextStyle( ndx, 
												SDITPGConfig[ndx].Pattern,
												SDITPGConfig[ndx].TextStyle[UcharVal1])) {
					GotoErrorStatus( 256*ndx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( SDIFindPatternType( SDITPGConfig[ndx].Pattern) == StandardPattern) {
			if ( UcharVal == TPGStandardTextStyle)
				UcharVal = TPGFreeTextStyle;
			else
				UcharVal = TPGStandardTextStyle;
		}
		else {
			if ( FuncTriggers & 0x01) {				// If button pressed is UP..
				if ( UcharVal-- == TPGFreeTextStyle)
					UcharVal = TPGComplexTextStyle;
			}
			else {											// ..button pressed is DOWN
				if ( UcharVal++ == TPGComplexTextStyle)
					UcharVal = TPGFreeTextStyle;
			}
		}
																		// If current style
		if ( UcharVal == ( SDITPGConfig[ndx].TextStyle[UcharVal1] & 0x0F))
			FuncState = 1;											//  remove SAVE option

																		// Transmit new style
		if ( ErrorCode = TrxSDITPGTextStyle( ndx, SDITPGConfig[ndx].Pattern,
				  ( SDITPGConfig[ndx].TextStyle[UcharVal1] & 0xF0) | UcharVal)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = TPGTextStyleArrTxt[UcharVal];	// Get text for selected level

	WriteCodeLN2( 0, SelectHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	SDITPGTextPosSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Select the text positioning in a SDI-TPG.							*/
/*	Remarks:		SDI-TPGx/TEXT MENU function.											*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGTextPosSelect(){

	UC patternType, ndx = BaseNdx - SDITPG1Item;

	patternType = SDIFindPatternType( SDITPGConfig[ndx].Pattern);

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                       // If text position has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new text positioning
			SDITPGConfig[ndx].TextPos[patternType][0] = UcharVal;
			SDITPGConfig[ndx].TextPos[patternType][1] = UcharVal1;
		}
		else {											// ..restore text style
			if (( UcharVal != SDITPGConfig[ndx].TextPos[patternType][0]) ||
			    ( UcharVal1 != SDITPGConfig[ndx].TextPos[patternType][1]))
				if ( ErrorCode = TrxSDITPGTextPos( ndx,
															  SDITPGConfig[ndx].TextStyle[patternType],
															  UcharVal, UcharVal1)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
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

			if ( UcharVal++ == SDIXPosMax)
				UcharVal = SDIXPosMin;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal-- == SDIXPosMin)
				UcharVal = SDIXPosMax;
		}
															// Save new text positioning
		if (( UcharVal == SDITPGConfig[ndx].TextPos[patternType][0]) &&
			 ( UcharVal1 == SDITPGConfig[ndx].TextPos[patternType][1]))
			FuncState = 1;
																		// Transmit new X position
		if ( ErrorCode = TrxSDITPGTextPos( ndx,
													  SDITPGConfig[ndx].TextStyle[patternType],
													  UcharVal, UcharVal1)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	if ( FuncTriggers & 0x0C) {			// If button UP/DOWN pressed..
		if ( FuncTriggers & 0x04) {				// If button pressed is UP..
			if ( UcharVal1++ == SDIYPosMax)
				UcharVal1 = SDIYPosMin;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal1-- == SDIYPosMin)
				UcharVal1 = SDIYPosMax;
		}
															// Save new text positioning
		if (( UcharVal == SDITPGConfig[ndx].TextPos[patternType][0]) &&
			 ( UcharVal1 == SDITPGConfig[ndx].TextPos[patternType][1]))
			FuncState = 2;
																		// Transmit new Y position
		if ( ErrorCode = TrxSDITPGTextPos( ndx,
													  SDITPGConfig[ndx].TextStyle[patternType],
													  UcharVal, UcharVal1)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	WriteTxtLN2( 0, "X: ");							// Write line 2 header
	sprintf( TextBuffer, "%+3d", UcharVal);	// Convert coordinate to ascii
	WriteTxt2( TextBuffer);							// Write coordinate

	ClearToPosLN2( 10);								// Clear to position

	WriteTxt2( "Y: ");                     	// Convert coordinate to ascii
	sprintf( TextBuffer, "%+3d", UcharVal1);
	WriteTxt2( TextBuffer);							// Write coordinate

	ClearRestLN2();									// Clear rest of line 2

	WriteType2_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	switch ( FuncState) {
		case 1:
		case 5:
			WriteItemArrows( 3, 3);						// Mark selected item
			break;

		case 2:
		case 4:												// Mark selected item
			WriteItemArrows( 13, 3);
			break;
	}

	FuncTriggers = 0;										// Reset button-selection
}

/***************************************************************************/
/*	SDITPGTextClockSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 9802242	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Select clock information in a SDI-TPG.								*/
/*	Remarks:		SDI-TPGx/TEXT MENU function.											*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGTextClockSelect(){

	UC ndx = BaseNdx - SDITPG1Item;

	if ( FuncSave || FuncEscape){
		if ( FuncSave) {                       // If text clock has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new text style
			SDITPGConfig[ndx].TextStyle[UcharVal1] &= 0x0F;
			SDITPGConfig[ndx].TextStyle[UcharVal1] |= ( UcharVal << 4);
		}
		else {											// ..restore text clock
			if ( UcharVal != ( SDITPGConfig[ndx].TextStyle[UcharVal1] >> 4))
				if ( ErrorCode = TrxSDITPGTextStyle( ndx, 
											SDITPGConfig[ndx].Pattern,
											SDITPGConfig[ndx].TextStyle[UcharVal1])) {
					GotoErrorStatus( 256*ndx + ErrorCode);
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
			if ( UcharVal-- == TPGTextClockNone)
				UcharVal = TPGTextClockTimeDate;

			if ( UcharVal == TPGTextClockDate)
				UcharVal--;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal++ == TPGTextClockTimeDate)
				UcharVal = TPGTextClockNone;

			if ( UcharVal == TPGTextClockDate)
				UcharVal++;
		}

																		// If current clock
		if ( UcharVal == SDITPGConfig[ndx].TextStyle[UcharVal1] >> 4)
			FuncState = 1;							 				//  remove SAVE option

																		// Transmit new style
		if ( ErrorCode = TrxSDITPGTextStyle( ndx,\
							SDITPGConfig[ndx].Pattern,
						 ( SDITPGConfig[ndx].TextStyle[UcharVal1] & 0x0F) | ( UcharVal << 4))) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = TPGTextClockArrTxt[UcharVal];	// Get text for selected clock

	WriteCodeLN2( 0, SelectHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	SDITPGAudioMenuShow															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the SDI-TPGx/EMB.AUDIO submenu.		  	*/
/*	Remarks:		This submenu will have 2 or 3 items depending on the TPG.	*/
/*					Cursor position on line 1 and line 2 is updated.				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGAudioMenuShow(){

	register int i, last;
	UC ndx = BaseNdx - SDITPG1Item;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( SDITPG_menu[SDITPGNdx[ndx]].ItemTxt);
	WriteCode1( CoSelectTxt);						// Write sub-menu headline
	ClearRestLN1();									// Clear rest of line 1

	SDITPGAudioNdx[ndx] = Ptr->ItemNumber;		// Update ../EMB.AUDIO menu index

	WriteCodeLN2( 0, SecondmnuHdrTxt);			// Write line 2 header

	if ( SDITPGUnit[ndx].HWType == PT8633) 	// If PT8633..
		last = SDITPGAudioGroupItem;       	 	//  group is available
	else													// ..else
		last = SDITPGAudioLevelItem;				//  group is NOT available

	for ( i = SDITPGAudioSignalItem; i <= last; i++) {
		ClearToPosLN2( SDITPGaudio_menu[i].ItemPos);	// Find item column position
		WriteCode2( SDITPGaudio_menu[i].ItemTxt);		// Write item texts
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write line 1&2 icons/dots
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	SDITPGAudioDown																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Go to the selected item in the SDI-TPGx/EMB.AUDIO submenu.	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGAudioDown(){

	UC ndx = BaseNdx - SDITPG1Item, itemNdx = SDITPGAudioNdx[ndx];

	WriteCodeLN1( 0, SecondmnuHdrTxt);				// Write menu headline
	WriteCode1( SlashDotHdrTxt);
	WriteCode1( SDITPG_menu[SDITPGNdx[ndx]].ItemTxt);
	WriteChar1( '/');
	WriteCode1( SDITPGaudio_menu[itemNdx].ItemTxt);
	WriteCode1( CoSelectTxt);
	ClearRestLN1();										// Clear rest of line 1

	switch ( itemNdx) {
		case SDITPGAudioSignalItem:
			UcharVal = SDITPGConfig[ndx].AudioSignal;
			break;

		case SDITPGAudioLevelItem:
			UcharVal = SDITPGConfig[ndx].AudioLevel;
			break;

		case SDITPGAudioGroupItem:
			UcharVal = SDITPGConfig[ndx].AudioGroup;
			break;
	}

	PrevPtr = Ptr;										// Store old menu-pointer
															// Point to selected item
	Ptr = &SDITPGaudio_submenu[SDITPGAudioNdx[ndx]];

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Set select-mode
}

/***************************************************************************/
/*	SDITPGAudioSignalSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Select embedded audio signal in a SDI-TPG.						*/
/*	Remarks:		SDI-TPGx/EMB.AUDIO MENU function.									*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGAudioSignalSelect(){

	UC ndx = BaseNdx - SDITPG1Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                   		// If audio signal has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new audio selection
			SDITPGConfig[ndx].AudioSignal = UcharVal;
		}
		else {											// ..restore audio signal
			if ( UcharVal != SDITPGConfig[ndx].AudioSignal)
				if ( ErrorCode = TrxSDITPGAudioSignal( ndx, SDITPGConfig[ndx].AudioSignal)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
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
			if ( UcharVal-- == TPGAudioOff)
				UcharVal = TPGAudioDual;
		}
		else													// ..button pressed is DOWN
			if ( UcharVal++ == TPGAudioDual)
				UcharVal = TPGAudioOff;

		if ( UcharVal == SDITPGConfig[ndx].AudioSignal)	// If current signal
			FuncState = 1;											//  remove SAVE option

																		// Transmit signal
		if ( ErrorCode = TrxSDITPGAudioSignal( ndx, UcharVal)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = TPGAudioSignalArrTxt[UcharVal];	// Get text for selected signal

	WriteCodeLN2( 0, SignalHdrTxt);				// Write header for line 2
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/*	SDITPGAudioLevelSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980512				     													*/
/*																									*/
/*	Function:	Select embedded audio level in a SDI-TPG.							*/
/*	Remarks:		SDI-TPGx/EMB.AUDIO MENU function.									*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGAudioLevelSelect(){

	UC ndx = BaseNdx - SDITPG1Item;
	code UC* tablePtr;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                       // If audio level has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new audio level
			SDITPGConfig[ndx].AudioLevel = UcharVal;
		}
		else {											// ..restore audio level
			if ( UcharVal != SDITPGConfig[ndx].AudioLevel)
				if ( ErrorCode = TrxSDITPGAudioLevel( ndx, SDITPGConfig[ndx].AudioLevel)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) 					// If UP/DOWN button pressed..
    {
		tablePtr = FindAudioLevelTable( SDITPGUnit[ndx].HWType);

		if ( FuncTriggers & 0x01) 				// If button pressed is UP..
      {
			do
        {
				if ( UcharVal-- == TPGAudioSilence)
					UcharVal = TPGAudioLevel20dB;
			  } while ( tablePtr[UcharVal] == NoAudioLevel);
		  }
		else
      {											// ..button pressed is DOWN
			do
        {
				if ( UcharVal++ == TPGAudioLevel20dB)
					UcharVal = TPGAudioSilence;
			  } while ( tablePtr[UcharVal] == NoAudioLevel);
		  }

		if ( UcharVal == SDITPGConfig[ndx].AudioLevel)	// If current level
			FuncState = 1;							 				//  remove SAVE option

																		// Transmit level
		if ( ErrorCode = TrxSDITPGAudioLevel( ndx, UcharVal)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = TPGAudioLevelArrTxt[UcharVal];	// Get text for selected level

	WriteCodeLN2( 0, LevelHdrTxt);				// Write line 2 header
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 7, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	SDITPGAudioGroupSelect														  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Select embedded audio group in a SDI-TPG.							*/
/*	Remarks:		SDI-TPGx/EMB.AUDIO MENU function.									*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITPGAudioGroupSelect(){

	UC ndx = BaseNdx - SDITPG1Item;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {                   		// If audio group has changed..
			ActivePresetNo = 0;						// Clear possible preset

															// Save new audio group
			SDITPGConfig[ndx].AudioGroup = UcharVal;
		}
		else {											// ..restore audio group
			if ( UcharVal != SDITPGConfig[ndx].AudioGroup)
				if ( ErrorCode = TrxSDITPGAudioGroup( ndx, SDITPGConfig[ndx].AudioGroup)) {
					GotoErrorStatus( 256*ndx + ErrorCode);
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
			if ( UcharVal-- == TPGAudioGroup1)
				UcharVal = TPGAudioGroup4;
		}
		else												// ..button pressed is DOWN
			if ( UcharVal++ == TPGAudioGroup4)
				UcharVal = TPGAudioGroup1;

		if ( UcharVal == SDITPGConfig[ndx].AudioGroup)	// If current group
			FuncState = 1;											//  remove SAVE option

																		// Transmit audio group
		if ( ErrorCode = TrxSDITPGAudioGroup( ndx, UcharVal)) {
			GotoErrorStatus( 256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = TPGAudioGroupArrTxt[UcharVal];	// Get text for selected group

	WriteCodeLN2( 0, GroupHdrTxt);				// Write header for line 2
	WriteCode2( CodePtr);							// Write selected text

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 7, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/*	AESEBUnMenuShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980214	    										*/
/* Revised:		980515				     													*/
/*																									*/
/*	Function:	Display the items in the AES-EBU menu.								*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		AESEBUnNdx																	*/
/***************************************************************************/
void AESEBUnMenuShow(){

	register int i;
	UC last;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( CoConfigureTxt);					// Write menu headline
	ClearRestLN1();									// Clear rest of line 1

	AESEBUnNdx = Ptr->ItemNumber;					// Update BB unit index, ie. no.

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for sub-menu line

	if ( AESEBUUnit.HWType == PT8635) 			// If PT8635..
		last = AESEBU2Item;	       	 			//  two audio generators
	else													// ..else
		last = AESEBU1Item;							//  only one audio generator

	for ( i = AESEBU1Item; i <= last; i++) {
		ClearToPosLN2( AESEBUn_menu[i].ItemPos);	// Find item column position
		WriteCode2( AESEBUn_menu[i].ItemTxt);		// Write item texts
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write line 1&2 icons/dots
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/*	AESEBUnDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Go to the selected item in the AES-EBU menu.						*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AESEBUnDown(){

	Ptr = &AESEBU_menu[AESEBUNdx[AESEBUnNdx]]; // Restore last AESEBUn item
}

/***************************************************************************/
/* UpToAESEBUn																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980119	    										*/
/* Revised:		980214				     													*/
/*																									*/
/*	Function:	Go to the AES-EBU MENU and restore the last selected item.	*/
/*	Remarks:		AES-EBU MENU function.													*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToAESEBUn(){

	Ptr = &AESEBUn_menu[AESEBUnNdx];		// Restore selected AESEBU menu
}

/***************************************************************************/
/* AESEBUMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Display the items in the AES-EBU/AES-EBUx submenu.				*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																				*/
/***************************************************************************/
void AESEBUMenuShow(){

	register int i;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( AESEBUn_menu[AESEBUnNdx].ItemTxt);
	WriteCode1( CoConfigureTxt);					// Write sub-menu headline
	ClearRestLN1();

	AESEBUNdx[AESEBUnNdx] = Ptr->ItemNumber;	// Update AESEBU unit index

	WriteCodeLN2( 0, SecondmnuHdrTxt);			// Write header for sub-menu line

	for ( i = AESEBUSignalItem; i <= AESEBUTimingItem; i++) {
		ClearToPosLN2( AESEBU_menu[i].ItemPos);	// Find item column position
		WriteCode2( AESEBU_menu[i].ItemTxt);		// Write item texts
	}

	ClearRestLN2();									// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);			// Write line 1&2 icons/dots
															// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* AESEBUDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Go to the selected item in the AES-EBU/AES-EBUx menu.			*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																				*/
/***************************************************************************/
void AESEBUDown(){

	if ( AESEBUNdx[AESEBUnNdx] > AESEBUSignalItem) {
		if ( Flags.NormalLockOn || ( AESEBUConfig[AESEBUnNdx].Signal == AESEBUWordClock))
			return;
	}

	WriteCodeLN1( 0, SecondmnuHdrTxt);					// Write menu headline
	WriteCode1( SlashDotHdrTxt);
	WriteCode1( AESEBUn_menu[AESEBUnNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( AESEBU_menu[AESEBUNdx[AESEBUnNdx]].ItemTxt);
	WriteCode1( CoSelectTxt);

	switch ( AESEBUNdx[AESEBUnNdx]) {
		case AESEBUSignalItem:
			UcharVal = AESEBUConfig[AESEBUnNdx].Signal;
			break;

		case AESEBULevelItem:
			UcharVal = AESEBUConfig[AESEBUnNdx].Level;
			break;

		case AESEBUTimingItem:
			UcharVal = AESEBUConfig[AESEBUnNdx].Timing;
			break;
	}

	ClearRestLN1();									// Clear rest of line 1

	PrevPtr = Ptr;										// Store old menu-pointer
															// Point to selected item
	Ptr = &AESEBU_submenu[AESEBUNdx[AESEBUnNdx]];

	FuncState = 1;										// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;								// Clear select-mode
}

/***************************************************************************/
/* AESEBUSignalSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980512				     													*/
/*																									*/
/*	Function:	Select the audio signal for a serial AES-EBU generator.		*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AESEBUSignalSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If audio signal has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new audio signal
			AESEBUConfig[AESEBUnNdx].Signal = UcharVal;
		}
		else {											// ..restore audio signal
			if ( UcharVal != AESEBUConfig[AESEBUnNdx].Signal)
				if ( ErrorCode = AESEBUUnitDrv( AESEBUnNdx, AESEBUConfig[AESEBUnNdx].Signal,\
						 AESEBUConfig[AESEBUnNdx].Level,	AESEBUConfig[AESEBUnNdx].Timing)) {
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
			if ( UcharVal-- == AESEBUStereo800Hz)
				UcharVal = AESEBUWordClock;
		}
		else													// ..button pressed is DOWN
			if ( UcharVal++ == AESEBUWordClock)
				UcharVal = AESEBUStereo800Hz;

		if ( UcharVal == AESEBUConfig[AESEBUnNdx].Signal)	// If current signal
			FuncState = 1;												//  remove SAVE option
																// Transmit audio signal
		if ( ErrorCode = AESEBUUnitDrv( AESEBUnNdx, UcharVal, AESEBUConfig[AESEBUnNdx].Level, AESEBUConfig[AESEBUnNdx].Timing)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AESEBUSignalArrTxt[UcharVal];		// Get text for selection

	WriteCodeLN2( 0, SignalHdrTxt);					// Write header on line 2
	WriteCode2( CodePtr);								// Write text on line 2

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/* AESEBULevelSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980512				     													*/
/*																									*/
/*	Function:	Set the audio level in a serial AES-EBU generator.				*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*					-14 dB is not allowed in PT8635.										*/
/*					-15 dB is not allowed in PT8605.										*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AESEBULevelSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If audio level has changed..
			ActivePresetNo = 0;						// Clear possible preset

			AESEBUConfig[AESEBUnNdx].Level = UcharVal;		// Save new audio level
		}
		else {											// ..restore audio level
			if ( UcharVal != AESEBUConfig[AESEBUnNdx].Level)
				if ( ErrorCode = AESEBUUnitDrv( AESEBUnNdx, AESEBUConfig[AESEBUnNdx].Signal, AESEBUConfig[AESEBUnNdx].Level,\
																	AESEBUConfig[AESEBUnNdx].Timing)) {
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
			if ( UcharVal-- == AESEBUSilence)
				UcharVal = AESEBULevel20dB;

			if ( AESEBUUnit.HWType == PT8635) {
				if ( UcharVal == AESEBULevel14dB)
					UcharVal--;
			}
			else {
				if ( UcharVal == AESEBULevel15dB)
					UcharVal--;
			}
		}
		else {												// ..button pressed is DOWN
			if ( UcharVal++ == AESEBULevel20dB)
				UcharVal = AESEBUSilence;

			if ( AESEBUUnit.HWType == PT8635) {
				if ( UcharVal == AESEBULevel14dB)
					UcharVal++;
			}
			else {
				if ( UcharVal == AESEBULevel15dB)
					UcharVal++;
			}
		}

		if ( UcharVal == AESEBUConfig[AESEBUnNdx].Level)		// If current level
			FuncState = 1;									//  remove SAVE option
																// Transmit audio level
		if ( ErrorCode = AESEBUUnitDrv( AESEBUnNdx, AESEBUConfig[AESEBUnNdx].Signal, UcharVal, AESEBUConfig[AESEBUnNdx].Timing)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AESEBULevelArrTxt[UcharVal];		// Get text for selected level

	WriteCodeLN2( 0, LevelHdrTxt);					// Write header on line 2
	WriteCode2( CodePtr);								// Write text on line 2

	ClearRestLN2();										// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);					// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 7, Cstrlen( CodePtr));	// Mark selected item

	FuncTriggers = 0;										// Reset the button-selection
}

/***************************************************************************/
/* AESEBUTimingSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980512				     													*/
/*																									*/
/*	Function:	Set the timing in a serial AES-EBU generator.					*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*					Instant update implemented.											*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AESEBUTimingSelect(){

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If timing has changed..
			ActivePresetNo = 0;						// Clear possible preset

			AESEBUConfig[AESEBUnNdx].Timing = UcharVal;		// Store new audio timing
		}
		else {											// ..restore AES/EBU timing
			if ( UcharVal != AESEBUConfig[AESEBUnNdx].Timing)
				if ( ErrorCode = AESEBUUnitDrv( AESEBUnNdx, AESEBUConfig[AESEBUnNdx].Signal, AESEBUConfig[AESEBUnNdx].Level,\
																	AESEBUConfig[AESEBUnNdx].Timing)) {
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
			if ( UcharVal-- == AESEBUPAL)
				UcharVal = AESEBUNTSCPhase5;
		}
		else													// ..button pressed is DOWN
			if ( UcharVal++ == AESEBUNTSCPhase5)
				UcharVal = AESEBUPAL;

		if ( UcharVal == AESEBUConfig[AESEBUnNdx].Timing) // If current timing..
			FuncState = 1;											  //  remove SAVE option

																	// Transmit AES/EBU timing
		if ( ErrorCode = AESEBUUnitDrv( AESEBUnNdx, AESEBUConfig[AESEBUnNdx].Signal, AESEBUConfig[AESEBUnNdx].Level, UcharVal)) {
			GotoErrorStatus( ErrorCode);
			return;
		}
	}

	CodePtr = AESEBUTimingArrTxt[UcharVal];		// Get text for selected phase

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
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Display 1st line in GENLOCK menu and prepare state machine.	*/
/*	Remarks:		GENLOCK MENU function.													*/
/*					Cursor position on line 1 is updated. 								*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void GenlockInputShow() {

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( CoSelectInputTxt);

	ClearRestLN1();

	FuncState = 1;						// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;				// Set select-mode
}

/***************************************************************************/
/* GenlockInputSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Setting the input of the genlock signal.							*/
/*	Remarks:		GENLOCK MENU function.													*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
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
				WriteCode1( CoSelectTxt);

				ClearRestLN1();

				Ptr = &genlocksys_menu;		 	// Update pointer
				Ptr->DispFunct();			 	//  and display
				return;
			}
			else
				FuncState -= 30;
			break;

		case 33:
		case 37:									 	// If TIMING selected..
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
				WriteCode1( genlocktiming_menu.ItemTxt);
				WriteCode1( CoEditDelayTxt);

				ClearRestLN1();

				Ptr = &genlocktiming_menu;	// Update pointer to genlock timing
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
		SelectModeOn = FALSE;						// Clear select-mode

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

	WriteCharLN2( 0, ' ');						// No header on line 2
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
			WriteItemArrows( 1, length);			// Mark selected item
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
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Setting the system of the genlock input.							*/
/*	Remarks:		GENLOCK MENU function.													*/
/*					Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
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

		GenlockInputShow();							// Display GENLOCK line 1, (this
															//  call sets FuncState = 1)
		if ( UcharVal == ActiveGenlockNo)		// FuncState MUST be set AFTER
			FuncState = 2;								//  GenlockInputShow()
		else
			FuncState = 6;

		Ptr = &genlock_menu;							// Update pointer to signal menu
		Ptr->DispFunct();								//  and display it
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal == SDIGenlock) {			// If SDI genlock selected..
			if ( UcharVal1 == SDI625Lock)			//  only 625 and 525 is available
				UcharVal1 = SDI525Lock;
			else
				UcharVal1 = SDI625Lock;
		}
		else {											// .. A,B or A-B genlock
			if ( FuncTriggers & 0x01) {			// If button pressed is UP..
				if ( UcharVal1-- == BurstLockPAL)
					UcharVal1 = MHzLock10;

				if ( UcharVal1 == SDI525Lock)
					UcharVal1 = SyncLock525;
			}
			else {										// ..button pressed is DOWN
				if ( UcharVal1++ == MHzLock10)
					UcharVal1 = BurstLockPAL;

				if ( UcharVal1 == SDI625Lock)
					UcharVal1 = MHzLock358;
			}
		}
															// If current genlock system..
		if ( UcharVal1 == GenlockConfig[UcharVal].System)
			FuncState = 1;								//  remove SAVE option
	}

	CodePtr = GenlockSignalArrTxt[UcharVal1];	// Get text for selection

	WriteCodeLN2( 0, SystemHdrTxt);				// Write header on line 2
	WriteCode2( CodePtr);							// Write active signal on line 2

	ClearRestLN2();                        	// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* GenlockTimingSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Setting the timing of the genlock input/system.					*/
/*	Remarks:		GENLOCK MENU MENU function.											*/
/*					Cursor position on line 2 is updated.								*/
/*					Instant update implemented when editing the active input.	*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void GenlockTimingSelect(){

	UC tmpSystem;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If delay has changed..
			ActivePresetNo = 0;						// Clear possible preset
															// Save new delay
			GenlockConfig[UcharVal].Delay = samples;
		}

		if ( UcharVal == ActiveGenlockNo)
			TrxGenlockDelay( GenlockConfig[UcharVal].Delay);  // MUST be .Delay

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		GenlockInputShow();							// Display GENLOCK line 1, (this
															//  call sets FuncState = 1)
		if ( UcharVal == ActiveGenlockNo)		// FuncState MUST be set AFTER
			FuncState = 3;								//  GenlockInputShow()
		else
			FuncState = 7;

		Ptr = &genlock_menu;
		Ptr->DispFunct();
		return;
	}

	if ( UcharVal == SDIGenlock) {
		if ( UcharVal1 == SDI625Lock)
			tmpSystem = SDI625;
		else
			tmpSystem = SDI525;

		SDITimeAdjust( GenlockItem, tmpSystem, GenlockConfig[UcharVal].Delay);
	}
	else {
		if (( UcharVal1 == BurstLockPAL) || ( UcharVal1 == SyncLock625))
			tmpSystem = PAL;
		else
			tmpSystem = NTSC;

		AnlTimeAdjust( GenlockItem, tmpSystem, GenlockConfig[UcharVal].Delay);
	}

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* PresetMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980509				     													*/
/*																									*/
/*	Function:	Display the items in the PRESET menu.								*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		PresetNdx																	*/
/***************************************************************************/
void PresetMenuShow(){

	register int i;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( CoSelectFunctionTxt);			// Display the headline

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
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Go to the selected item in the PRESET menu.						*/
/*	Remarks:		Cursor position on line 1 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
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
			WriteCode1( CoSelectTxt);
			break;

		case PresetNameItem:
			WriteCode1( CoEditNameTxt);

			strcpy( TextBuffer, Preset[UcharVal].Name);

			if ( UintVal = strlen( TextBuffer)) // Position in Text string
				UintVal--;
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
/* Revised:		980514				     													*/
/*																									*/
/*	Function:	Select a complete preset to recall 									*/
/*	Remarks:		PRESET MENU function														*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void PresetRecallSelect(){

	if ( FuncSave || FuncEscape) {

		if ( FuncSave) {
			ActivePresetNo = UcharVal + 1;

			if ( ErrorCode = ConfigurePreset( UcharVal)) {
				GotoErrorStatus( ErrorCode);
				return;
			}
														// Set pointer to status menu
			Ptr = &status_menu[StatusNdx = StatusPresetItem];
			MenuModeOn = FALSE;					// Clear menu-mode
		}
		else
			Ptr = PrevPtr;							// Set pointer to previous menu

		SelectModeOn = FALSE;					// Clear select-mode

		Ptr->DispFunct();							// Display previous menu
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
	WriteCharLN2( 6, '1' + UcharVal);			// This DESTROYS line 2 position

	WriteTxtLN2( 9, TxtPtr);						// Write selected text on line 2

	ClearRestLN2();                     		// Clear rest of line 2

	WriteType3_OK_ESC( FuncState);				// Write the SAVE ESC text

	if ( FuncState == 1)
		WriteItemArrows( 9, strlen( TxtPtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* PresetStoreSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980322				     													*/
/*																									*/
/*	Function:	Store a setup in preset													*/
/*	Remarks:		PRESET MENU function														*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, UcharVal, FuncSave, FunState and FuncTriggers				*/
/***************************************************************************/
void PresetStoreSelect(){

	register int i;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {

			ActivePresetNo = UcharVal + 1;

			for ( i = 0; i < MaxAnlBlkUnits; i++)
				memcpy( &Preset[UcharVal].AnlBlkStore[i], &AnlBlkConfig[i], sizeof( AnlBlkConfig[0]));

			for ( i = 0; i < MaxAnlTPGUnits; i++)
				memcpy( &Preset[UcharVal].AnlTPGStore[i], &AnlTPGConfig[i], sizeof( AnlTPGConfig[0]));

			for ( i = 0; i < MaxSDITSGUnits; i++)
				memcpy( &Preset[UcharVal].SDITSGStore[i], &SDITSGConfig[i], sizeof( SDITSGConfig[0]));

			for ( i = 0; i < MaxSDITPGUnits; i++)
				memcpy( &Preset[UcharVal].SDITPGStore[i], &SDITPGConfig[i], sizeof( SDITPGConfig[0]));

			for ( i = 0; i < MaxAESEBUUnits; i++)
				memcpy( &Preset[UcharVal].AESEBUStore[i], &AESEBUConfig[i], sizeof( AESEBUConfig[0]));

			for ( i = 0; i < MaxGenlockUnits; i++)
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
	WriteCharLN2( 5, '1' + UcharVal);			// This DESTROYS line 2 position

	WriteTxtLN2( 8, TxtPtr);						// Write selected text on line 2

	ClearRestLN2();									// Clear rest of line 2

	WriteType3_OK_ESC( FuncState);				// Write the SAVE ESC text

	if ( FuncState == 1)
		WriteItemArrows( 8, strlen( TxtPtr));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* PresetNameSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		AUDIO-SIGNAL MENU function												*/
/*					Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
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

		if ( UintVal = strlen( TextBuffer))		// Find position in string
			UintVal--;

		FuncState = 1;
	}
														// Select TextBuffer position
	if ( FuncTriggers & 0xC0) {				// If LEFT/RIGHT button pressed..
		if ( FuncTriggers & 0x40) {				// If button pressed is LEFT..
			TextBuffer[UintVal] = 0;

			if ( UintVal)
				UintVal--;
		}
		else {											// ..button pressed is RIGHT
			if (( UintVal < 15) && ( UintVal < strlen( TextBuffer)))
				UintVal++;
		}
	}
														// Select character for TextBuffer
	if ( FuncTriggers & 0x0C) {				// If UP/DOWN button pressed..
		if ( TextBuffer[UintVal] == 0) {		// If text insert..
														//  set new character = previous
			TextBuffer[UintVal] = TextBuffer[UintVal-1];
			TextBuffer[UintVal+1] = 0;
		}

		TextBuffer[UintVal] = GetNextPresetASCII(( FuncTriggers & 0x04),
																		 TextBuffer[UintVal]);

		if ( !strcmp( Preset[UcharVal].Name, TextBuffer))
			FuncState = 6;
	}

	WriteCodeLN2( 0, NameHdrTxt);					// Write header on line 2
	WriteCharLN2( 4, '1'+UcharVal);				// This DESTROYS line 2 position

	WriteTxtLN2( 7, TextBuffer);					// Write selected text on line 2

	ClearRestLN2();									// Clear rest of line 2

	WriteType8_SAVE_ESC( FuncState);				// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 3:											// Mark selected item
			WriteItemArrows( 7, strlen( TextBuffer));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 6:
		case 7:											// Mark selected item
			WriteItemBrackets( 7, strlen( TextBuffer));
			CharBlinkLN2( 7+UintVal, ON);			// Switch OFF blinking at
			break;										//  editing character
	}

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* ConfigMenuShow																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Display the items in the CONFIG menu.								*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void ConfigMenuShow(){

	register int i, j;
	UC number, spacing, start;
	code struct MENU_ITEM* tmpPtr;

	WriteCodeLN1( 0, MenuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteCode1( CoSelectFunctionTxt);			// Write the headline

	ClearRestLN1();

	ConfigNdx = Ptr->ItemNumber;

	WriteCodeLN2( 0, SubmnuHdrTxt);				// Write header for menu line

	tmpPtr = Ptr;										// Save menupointer temporarily

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {
		spacing = 3;									// Spacing between items

		if ( ConfigNdx < ConfigLCDContItem) {
			Ptr = &config_menu[ConfigDateTimeItem];
			start = ConfigDateTimeItem;
			number = 3;
		}
		else {
			if ( ConfigNdx < ConfigRS232Item) {
				Ptr = &config_menu[ConfigLCDContItem];
				start = ConfigLCDContItem;
				number = 2;
			}
			else {
				Ptr = &config_menu[ConfigRS232Item];
				start = ConfigRS232Item;
				number = 2;
			}
		}
	}
	else {
		spacing = 2;									// Spacing between items

		if ( ConfigNdx < ConfigDownloadItem) {
			Ptr = &config_menu[ConfigLockItem];
			start = ConfigLockItem;
			number = 3;
		}
		else {
			Ptr = &config_menu[ConfigDownloadItem];
			start = ConfigDownloadItem;
			number = 3;
		}
	}

	for ( j = 8, i = start; i < start+number; i++) {
		WriteCode2( Ptr->ItemTxt);

		if ( spacing == 2)
			WriteTxt2( "  ");
		else
			WriteTxt2( "   ");

		if ( Ptr->ItemNumber < ConfigNdx )
			j += ( spacing + Cstrlen( Ptr->ItemTxt));

		ConfigItemRight();
	}

	ClearRestLN2();										// Clear rest of line 2

	Ptr = tmpPtr;											// Restore menu pointer

	i = Ptr->ItemConfig;									// Get line1&2 icons/dots info

	if ( Flags.PanelLockOn || Flags.NormalLockOn) {
		if ( ConfigNdx != ConfigLockItem)
			i = ( i | P_) & ~D_;
	}
	else {
		if ( Flags.DateTimeLockOn && ( ConfigNdx == ConfigDateTimeItem))
				i = ( i | P_) & ~D_;

		if ( Flags.DownloadLockOn && ( ConfigNdx == ConfigDownloadItem))
				i = ( i | P_) & ~D_;

		if ( Flags.DiagnoseLockOn && ( ConfigNdx == ConfigDiagnoseItem))
				i = ( i | P_) & ~D_;
	}
	WriteArrowsField( i);							// Write icons/dots on line 1&2
															//  Mark selected item
	WriteItemArrows( j, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* ConfigDown																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980509				     													*/
/*																									*/
/*	Function:	Go the the selected item in the CONFIG menu.						*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																				*/
/***************************************************************************/
void ConfigDown(){

	if (( Flags.PanelLockOn || Flags.NormalLockOn) && ( ConfigNdx != ConfigLockItem))
		return;

	if ( Flags.DateTimeLockOn && ( ConfigNdx == ConfigDateTimeItem))
		return;

	if ( Flags.DownloadLockOn && ( ConfigNdx == ConfigDownloadItem))
		return;

	if ( Flags.DiagnoseLockOn && ( ConfigNdx == ConfigDiagnoseItem))
		return;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( config_menu[ConfigNdx].ItemTxt);

	PrevPtr = Ptr;								// Store old menu-pointer setting
													// DO NOT MOVE THIS LINE ANYWHERE ELSE
	switch ( ConfigNdx) {
		case ConfigDateTimeItem:
			Ptr = &datetime_menu[DateTimeNdx];
			break;

		case ConfigLockItem:
			UcharVal = NormalLock;			// Begin selection with Normal Lock

			Ptr = &config_submenu[ConfigNdx];

			TmpFlags = Flags;					// Save flags setting temporarily

			FuncState = 1;						// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;				// Set select mode
			break;

		case ConfigAutoESCItem:
			WriteCode1( CoSelectTxt);

			Ptr = &config_submenu[ConfigNdx];

			TmpFlags = Flags;					// Save flags setting temporarily

			FuncState = 1;						// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;				// Set select mode
			break;

		case ConfigLCDContItem:
			UcharVal1 = MaxContrast;
			UcharVal2 = MinContrast;

			Ptr = &config_submenu[ConfigNdx];

			FuncState = 1;						// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;				// Set select mode
			break;

		case ConfigDownloadItem:
			WriteCode1( CoSelectTxt);

			UcharVal = DwnPresetNo;

			Ptr = &config_submenu[ConfigNdx];

			TmpFlags = Flags;					// Save flags setting temporarily

			FuncState = 1;						// Prepare for item state-machine
			FuncTriggers = 0;
			FuncSave = FuncEscape = FALSE;

			SelectModeOn = TRUE;				// Set select mode
			break;

		case ConfigRS232Item:
			Ptr = &RS232_menu[RS232Ndx];
			break;

		case ConfigDiagnoseItem:
			WriteCode1( CoSelectTxt);

			Ptr = &diagnose_menu[DiagnoseNdx];
			break;
	}

	ClearRestLN1();							// Clear rest of line 1
}

/***************************************************************************/
/* UpToConfig																		  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		960102				     													*/
/*																									*/
/*	Function:	Select the CONFIG MENU and restore the last selected item	*/
/*	Remarks:		CONFIG MENU function														*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void UpToConfig(){

	Ptr = &config_menu[ConfigNdx];
}

/***************************************************************************/
/* ConfigLockSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the display containing the LOCK functions				*/
/*	Remarks:		CONFIG/LOCK MENU function												*/
/*					Cursor position on line 1&2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void ConfigLockSelect(){

	if ( FuncEscape) {

		SelectModeOn = FALSE;						// Clear select mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If button pressed is UP/DOWN..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..

			if ( UcharVal-- == NormalLock)
				UcharVal = DiagnoseLock;

			if ( UcharVal == DateTimeLock)
				if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637))
					UcharVal--;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal++ == DiagnoseLock)
				UcharVal = NormalLock;

			if ( UcharVal == DateTimeLock)
				if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637))
					UcharVal++;
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

		case DateTimeLock:							// If DATE-TIME LOCK selected..
			if ( FuncSave) {
				FuncSave = FALSE;
				Flags.DateTimeLockOn = TmpFlags.DateTimeLockOn;
			}
			UcharVal1 = ( UC) Flags.DateTimeLockOn;

			if ( FuncTriggers & 0x0C) {			// If button UP/DOWN pressed..
				TmpFlags.DateTimeLockOn ^= 0x01;	//  toggle selection

				if ( TmpFlags.DateTimeLockOn == UcharVal1)
					FuncState = 2;
			}

			UcharVal2 = ( UC) TmpFlags.DateTimeLockOn;
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

	CodePtr = LockArrTxt[UcharVal];

	WriteCode1( CodePtr);						// Get display text for selection

	WriteChar1( '(');
	WriteCode1( OffOnArrTxt[UcharVal1]);
	WriteChar1( ')');										// Write the ON/OFF text

	ClearRestLN1();										// Clear rest of line 1

	WriteCodeLN2( 0, LockHdrTxt);						// Write LINE 2 header
	ClearRestLN2();

	WriteType2_SAVE_ESC( FuncState);					// Write the ESC text

	WriteCodeLN2( 6, CodePtr);
	WriteCodeLN2( 21, OffOnArrTxt[ UcharVal2]);

	switch ( FuncState) {
		case 1:
		case 5:
			WriteItemArrows( 6, Cstrlen( CodePtr));// Mark selected item
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
/* Revised:		980512				     													*/
/*																									*/
/*	Function:	Handle the display containing the AUDIO functions				*/
/*	Remarks:		Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		Ptr, LCDContrast, UcharVal1, FuncSave, FunState,				*/
/*					FuncTriggers, ErrorCode and SelectModeOn							*/
/***************************************************************************/
void ConfigLCDContSelect(){

	register int i;

	if ( FuncEscape) {

		SelectModeOn = FALSE;						// Clear select mode

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
	if ( ErrorCode = SendContrast( LCDContrast)) {
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
/* DateTimeMenuShow																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Display the items in the CONFIG/DATE-TIME submenu.				*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void DateTimeMenuShow() {

	register int i, j;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteCode1( CoSelectTxt);

	ClearRestLN1();									// Clear rest of line 1

	DateTimeNdx = Ptr->ItemNumber;				// Update DATE-TIME Menu index

	WriteCodeLN2( 0, SecondmnuHdrTxt);			// Write header for menu line

	i = j = DateTimeOffsetItem;

	if ( DateTimeNdx < DateTimeOffsetItem)
		i = DateTimeDateItem;
	else
		j++;

	for ( ; i < j; i++) {
		ClearToPosLN2( datetime_menu[i].ItemPos);	// Write item position
		WriteCode2( datetime_menu[i].ItemTxt);		// Write item text
	}

	ClearRestLN2();										// Clear rest of line 2

	WriteArrowsField( Ptr->ItemConfig);				// Write line 1&2 icons/dots
																// Mark selected item
	WriteItemArrows( Ptr->ItemPos, Cstrlen( Ptr->ItemTxt));
}

/***************************************************************************/
/* DateTimeDown																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980727				     													*/
/*																									*/
/*	Function:	Go to the selected item in the DATE-TIME menu.					*/
/*	Remarks:		Cursor position on line 1 & 2 are updated.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DateTimeDown() {

	WriteCodeLN1( 0, SecondmnuHdrTxt);						// Write menu headline
	WriteCode1( SlashDotHdrTxt);
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( datetime_menu[DateTimeNdx].ItemTxt);

	switch ( DateTimeNdx) {
		case DateTimeDateItem:
			WriteCode1( CoModifyTxt);

			UcharVal = TimeClockConfig.Format >> 1;		// Get date format
			UlongVal = TimeClockConfig.Date;

			DateFormat( UcharVal, UlongVal, 0, TextBuffer);
			break;

		case DateTimeTimeItem:
			WriteCode1( CoModifyTxt);

			UcharVal = TimeClockConfig.Format & 0x01;		// Get time format
			UlongVal = TimeClockConfig.Time;

			TimeFormat( UcharVal, UlongVal, 0, TextBuffer);
			break;

		case DateTimeRefItem:
			UcharVal = TimeClockConfig.Reference;			// Get reference
			break;

		case DateTimeOffsetItem:
			WriteCode1( CoModifyTxt);

			UcharVal = 0;
			UlongVal = TimeClockConfig.Offset;				// Get offset

			TimeOffsetFormat( UlongVal, TextBuffer);
			break;
	}

	ClearRestLN1();								// Clear rest of line 1

	PrevPtr = Ptr;									// Store old menu-pointer
	Ptr = &datetime_submenu[DateTimeNdx];	// Point to selected item

	FuncState = 1;									// Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;							// Set select-mode
}

/***************************************************************************/
/* DateTimeDateSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980916			     														*/
/*																									*/
/*	Function:	Select date-format and/or set date in time clock option		*/
/* Remarks:		When exiting the edited time, the time will be reset if it	*/
/*					is not valid.																*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void DateTimeDateSelect() {

	UC tmp;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If date has changed..
			ActivePresetNo = 0;						// Clear a possible preset

			TimeClockConfig.Format &= 0x01;		// Save new date format
			TimeClockConfig.Format |= ( UcharVal << 1);

			TimeClockConfig.Date = UlongVal;		// Save new date

															// Select TCI date register
			tmp = TimeClockConfig.TCIControl & 0x1F;
			tmp |= ( TCIDateRegister << 5);

			TrxTimeClockData( tmp, 3, UlongVal);// Update TCI
		}

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Select line to edit
	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( UcharVal-- == DMYFormat)
				UcharVal = YMDFormat;
		}													// ..button pressed is DOWN
		else {
			if ( UcharVal++ == YMDFormat)
				UcharVal = DMYFormat;
		}

		DateFormat( UcharVal, UlongVal, 0, TextBuffer);

		if ( UcharVal == TimeClockConfig.Format >> 1)	// If current format..
			if ( UlongVal == TimeClockConfig.Date)
				FuncState = 1;										//  remove SAVE option
	}
															// Select position in TextBuffer
	if ( FuncTriggers & 0xC0) {					// If LEFT/RIGHT button pressed..
		if ( FuncTriggers & 0x40) {				// If button pressed is LEFT..
			if ( UcharVal1-- == 0)
				UcharVal1 = 7;

			if (( UcharVal1 == 2) || ( UcharVal1 == 5))
				UcharVal1--;
		}
		else {											// ..button pressed is RIGHT
			if ( UcharVal1++ == 7)
				UcharVal1 = 0;

			if (( UcharVal1 == 2) || ( UcharVal1 == 5))
				UcharVal1++;
		}
	}
															// Select character in TextBuffer
	if ( FuncTriggers & 0x30) {					// If UP/DOWN button pressed..

		UlongVal = AdjustDate( UcharVal1, ( FuncTriggers & 0x10), UcharVal, UlongVal);

		DateFormat( UcharVal, UlongVal, 0, TextBuffer);

		if ( UlongVal == TimeClockConfig.Date)				// If current date..
			if ( UcharVal == TimeClockConfig.Format >> 1)
				FuncState = 8;                   			//  remove SAVE option
	}

	CodePtr = DateFormatArrTxt[UcharVal];		// Get text for selected format

	WriteCodeLN2( 0, DateHdrTxt);					// Write line 2 header
	WriteCode2( CodePtr);							// Write the selected pattern
	ClearToPosLN2( 17);

	WriteTxt2( TextBuffer);							// Write edited text on line 2
	ClearRestLN2();									// Clear rest of line 2

	WriteType13_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 4:											// Mark selected item
			WriteItemArrows( 6, Cstrlen( CodePtr));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 2:
		case 5:											// Mark selected item
			UcharVal1 = 0;

			if ( !ValidDate( UlongVal)) {
				UlongVal = TimeClockConfig.Date;
				DateFormat( UcharVal, UlongVal, 0, TextBuffer);
				WriteTxtLN2( 17, TextBuffer);		// Write edited text on line 2
			}

			WriteItemArrows( 17, strlen( TextBuffer));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 8:
		case 9:											// Mark selected item
			WriteItemBrackets( 17, strlen( TextBuffer));
			CharBlinkLN2( 17 + UcharVal1, ON);	// Switch OFF blinking at
			break;										//  editing character
	}

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* DateTimeTimeSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980916				     													*/
/*																									*/
/*	Function:	Select time-format and/or set time in time clock option		*/
/* Remarks:		When exiting the edited date, the date will be reset if it	*/
/*					is not valid.																*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void DateTimeTimeSelect() {

	UC tmp;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If time has changed..
			ActivePresetNo = 0;						// Clear a possible preset

			TimeClockConfig.Format &= 0xFE;		// Save new time format
			TimeClockConfig.Format |= UcharVal;

			TimeClockConfig.Time = UlongVal;		// Save new time

															// Select TCI time register
			tmp = TimeClockConfig.TCIControl & 0x1F;
			tmp |= ( TCITimeRegister << 5);

			TrxTimeClockData( tmp, 3, UlongVal);// Update TCI
		}

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Select line to edit
	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( UcharVal == Hour12Format)
			UcharVal = Hour24Format;
		else
			UcharVal = Hour12Format;

		TimeFormat( UcharVal, UlongVal, 0, TextBuffer);

		if ( UcharVal == ( TimeClockConfig.Format & 0x01))	// If current format..
			if ( UlongVal == TimeClockConfig.Time)
				FuncState = 1;											//  remove SAVE option
	}
															// Select position in TextBuffer
	if ( FuncTriggers & 0xC0) {					// If LEFT/RIGHT button pressed..
		if ( FuncTriggers & 0x40) {				// If button pressed is LEFT..
			if ( UcharVal1-- == 0)
				UcharVal1 = 7;

			if (( UcharVal1 == 2) || ( UcharVal1 == 5))
				UcharVal1--;
		}
		else {											// ..button pressed is RIGHT
			if ( UcharVal1++ == 7)
				UcharVal1 = 0;

			if (( UcharVal1 == 2) || ( UcharVal1 == 5))
				UcharVal1++;
		}
	}
															// Select character in TextBuffer
	if ( FuncTriggers & 0x30) {					// If UP/DOWN button pressed..

		UlongVal = AdjustTime( UcharVal1, ( FuncTriggers & 0x10), UcharVal, UlongVal);

		TimeFormat( UcharVal, UlongVal, 0, TextBuffer);

		if ( UlongVal == TimeClockConfig.Time)				// If current format..
			if ( UcharVal == ( TimeClockConfig.Format & 0x01))
				FuncState = 8;                  				//  remove SAVE option
	}

	CodePtr = TimeFormatArrTxt[UcharVal];		// Get text for selected format

	WriteCodeLN2( 0, TimeHdrTxt);					// Write line 2 header
	WriteCode2( CodePtr);							// Write the selected pattern
	ClearToPosLN2( 17);

	WriteTxt2( TextBuffer);							// Write edited text on line 2
	ClearRestLN2();									// Clear rest of line 2

	WriteType13_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	switch ( FuncState){
		case 1:
		case 4:											// Mark selected item
			WriteItemArrows( 6, Cstrlen( CodePtr));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 2:
		case 5:											// Mark selected item
			UcharVal1 = 0;

			if ( !ValidTime( UlongVal)) {
				UlongVal = TimeClockConfig.Time;
				TimeFormat( UcharVal, UlongVal, 0, TextBuffer);

				WriteTxtLN2( 17, TextBuffer);		// Write edited text on line 2
			}

			WriteItemArrows( 17, strlen( TextBuffer));
			CharBlinkLN2( 0, OFF);					// Switch OFF blinking
			break;

		case 8:
		case 9:											// Mark selected item
			WriteItemBrackets( 17, strlen( TextBuffer));
			CharBlinkLN2( 17 + UcharVal1, ON);	// Switch OFF blinking at
			break;										//  editing character
	}

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* DateTimeRefSelect																  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980730				     													*/
/*																									*/
/*	Function:	Select time reference for the Time Clock Interface.			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void DateTimeRefSelect() {

	UC tmp;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If reference has changed..
			ActivePresetNo = 0;						// Clear a possible preset
															// Save new reference
			TimeClockConfig.Reference = UcharVal;

															// Select TCI time register
			tmp = TimeClockConfig.TCIControl & 0x1F;
			tmp |= ( TCITimeRefRegister << 5);
															// Update TCI
			TrxTimeClockData( tmp, 1, (UL) UcharVal);
		}

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) {				// If button pressed is UP..
			if ( UcharVal-- == VITCTimeInput)
				UcharVal = VideoFieldFreqInput;
		}
		else {											// ..button pressed is DOWN
			if ( UcharVal++ == VideoFieldFreqInput)
				UcharVal = VITCTimeInput;
		}
															// If currentreference..
		if ( UcharVal == TimeClockConfig.Reference)
			FuncState = 1;								//  remove SAVE option
	}

	CodePtr = DateTimeRefArrTxt[UcharVal];		// Get text for selected pattern

	WriteCodeLN2( 0, ReferenceHdrTxt);			// Write line 2 header
	WriteCode2( CodePtr);							// Write the selected pattern

	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);				// Write the SAVE ESC texct

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 11, Cstrlen( CodePtr));// Mark selected item

	FuncTriggers = 0;									// Reset button selectors
}

/***************************************************************************/
/* DateTimeOffsetSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980121	    										*/
/* Revised:		980127				     													*/
/*																									*/
/*	Function:	Select the time offset for the Time Clock Interface			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void DateTimeOffsetSelect() {

	UC tmp;

	if ( FuncSave || FuncEscape) {
		if ( FuncSave) {								// If time has changed..
			ActivePresetNo = 0;						// Clear a possible preset

			TimeClockConfig.Offset = UlongVal;	// Save new offset

															// Select TCI time offset register
			tmp = TimeClockConfig.TCIControl & 0x1F;
			tmp |= ( TCIOffsetRegister << 5);

			TrxTimeClockData( tmp, 3, UlongVal);// Update TCI
		}

		CharBlinkLN2( 0, OFF);						// Switch off possible blinking

		SelectModeOn = FALSE;						// Clear select-mode

		Ptr = PrevPtr;									// Set pointer to previous menu
		Ptr->DispFunct();								// Display previous menu
		return;
	}
															// Select character in TextBuffer
	if ( FuncTriggers & 0x03) {					// If UP/DOWN button pressed..

		UlongVal = AdjustTimeOffset( UcharVal, ( FuncTriggers & 0x01), UlongVal);
		TimeOffsetFormat( UlongVal, TextBuffer);

		if ( UlongVal == TimeClockConfig.Offset)		// If current offset..
			FuncState = 1;                  				//  remove SAVE option
	}

	WriteCodeLN2( 0, OffsetHdrTxt);				// Write line 2 header

	WriteTxt2( TextBuffer);							// Write edited text on line 2
	ClearRestLN2();									// Clear rest of line 2

	WriteType1_SAVE_ESC( FuncState);			// Write the SAVE ESC text

	if (( FuncState == 1) || ( FuncState == 3))
		WriteItemArrows( 8, strlen( TextBuffer));	// Mark selected item

	FuncTriggers = 0;									// Reset the button-selection
}

/***************************************************************************/
/* DownloadTypeSelect															  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980509				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
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
	WriteCode1( CoSelectTxt);
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
/* Revised:		970328				     													*/
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
/* Revised:		991103				     													*/
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

				for ( i = 1; i < 7; i++) {

					TextBuffer[16] = '0' + i;
					ResponseHandler( TextBuffer);

					UserTimer = 750;					// 15 second timeout
					while ( !ResponseReceived && UserTimer) {
						FeedTheWatchdog;

						while (( RS232Char = Udr_InChar()) != UDR_EOF)
							CmdHandler(( char) RS232Char);
					}

					if ( !ResponseReceived)
						i = 8;
					else {
						UserTimer = 20;				// Delay to allow recipient to
						while( UserTimer)				//  update it's memory
							FeedTheWatchdog;
					}
				}
			}
			else {
				TextBuffer[16] = '1' + UcharVal;
				ResponseHandler( TextBuffer);

				UserTimer = 750;						// 15 second timeout
				while ( !ResponseReceived && UserTimer) {
					FeedTheWatchdog;

					while (( RS232Char = Udr_InChar()) != UDR_EOF)
						CmdHandler(( char) RS232Char);
				}
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
				while( UserTimer && !ButtonReady())
					FeedTheWatchdog;
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
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Display the items in the CONFIG/RS232 submenu.					*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void RS232MenuShow(){

	register int i, j;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteCode1( CoSelectTxt);

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
/* Revised:		980508				     													*/
/*																									*/
/*	Function:	Go to the selected item in the CONFIG/RS232 submenu.			*/
/*	Remarks:		Cursor position on line 1 is updated.								*/
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
	WriteCode1( CoSelectTxt);

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
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Display the items in the CONFIG/DIAGNOSE submenu.				*/
/*	Remarks:		Cursor position on line 2 is updated.								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void DiagnoseMenuShow(){

	register int i, j;

	WriteCodeLN1( 0, SubmnuHdrTxt);
	WriteCode1( base_menu[BaseNdx].ItemTxt);
	WriteChar1( '/');
	WriteCode1( config_menu[ConfigNdx].ItemTxt);
	WriteCode1( CoSelectTxt);						// Write sub-menu headline

	ClearRestLN1();

	DiagnoseNdx = Ptr->ItemNumber;

	WriteCodeLN2( 0, SelectHdrTxt);				// Write header for menu line

	i = j = DiagDisplayItem;						// Write DIAGNOSE-SUBMENU items

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
/* Revised:		980519				     													*/
/*																									*/
/*	Function:	Execute the selected item in the CONFIG/DIAGNOSE submenu.	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
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

			UcharVal = FindUnit( UP, 3);     // Find first unit to test

			if ( UcharVal <= 3)					// If no units to test..
				UcharVal = 0;						//
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
			UcharVal1 = 0;							// TRUE if right button pressed
			UcharVal2 = 0;							// No of buttons pressed
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
/* Revised:		980324				     													*/
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
							UcharVal1= TestAnlBlkUnit( 0);	// BB1&2
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
/* Revised:		980521				     													*/
/*																									*/
/*	Function:	Test the installed options.											*/
/*	Remarks:		IntVal holds the timer value, ie. the arrows.					*/
/*					UcharVal holds the unit number to test, (0 is No options..)	*/
/*					UcharVal1 holds the result of the single options test.		*/
/*					UcharVal2 holds the global result of the options test.		*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseOptionsTest(){

	UC tmp, HWType, HWVersion;

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
					case 0:										// Options test: setup
						WriteCodeLN2( 0, TestingTxt);

						switch ( UcharVal ) {
							case 4:	 							// BB34 in TSG3
							case 6:	 							// BB56 in TSG4
							case 8:	 							// BB78 in TSG2
								tmp = UcharVal - 2;

								HWVersion = AnlBlkUnit[tmp].HWVersion;
								HWType = AnlBlkUnit[tmp].HWType;
								break;

							case 10:								// AnlTPG
							case 11:
								tmp = UcharVal - 10;

								HWVersion = AnlTPGUnit[tmp].HWVersion;
								HWType = AnlTPGUnit[tmp].HWType;
								break;

							case 12:								// SDITSG
							case 13:
							case 14:
								tmp = UcharVal - 12;

								HWVersion = SDITSGUnit[tmp].HWVersion;
								HWType = SDITSGUnit[tmp].HWType;
								break;

							case 15:								// SDITPG
							case 16:
							case 17:
								tmp = UcharVal - 15;

								HWVersion = SDITPGUnit[tmp].HWVersion;
								HWType = SDITPGUnit[tmp].HWType;
								break;

							case 18:								// AES-EBU
								HWVersion = AESEBUUnit.HWVersion;
								HWType = AESEBUUnit.HWType;
								break;

							case 19:								// SDI Genlock
								HWVersion = SDIGenlockUnit.HWVersion;
								HWType = SDIGenlockUnit.HWType;
								break;

							case 20:								// Internal 2
								HWVersion = Int2GenlockUnit.HWVersion;
								HWType = Int2GenlockUnit.HWType;
								break;

							case 21:								// TimeClock
								HWVersion = TimeClockUnit.HWVersion;
								HWType = TimeClockUnit.HWType;
								break;
						}

						sprintf( TextBuffer, "PT86%02d", HWType);

						if ( HWVersion)
							sprintf( &TextBuffer[6], "/9%02d", HWVersion);

						WriteTxt2( TextBuffer);

						switch ( UcharVal ) {
							case 4: 	 							// BB34 in TSG3
							case 6: 	 							// BB56 in TSG4
							case 8:	 							// BB78 in TSG2
								if ( UcharVal == 4)
									CodePtr = SDITSGArrTxt[1];
								else
									if ( UcharVal == 6)
										CodePtr = SDITSGArrTxt[2];
								else
									CodePtr = SDITSGArrTxt[0];

								WriteTxt2( " in ");
								WriteCode2( CodePtr);
								break;

							case 10:								// ANL-TPG2
							case 11:                      // ANL-TPG5
								WriteTxt2( " in ");
								WriteCode2( AnlTPGArrTxt[UcharVal - 10]);
								break;

							case 12:								// SDI-TSG2
							case 13:								// SDI-TSG3
							case 14:                      // SDI-TSG4
								WriteTxt2( " in ");
								WriteCode2( SDITSGArrTxt[UcharVal - 12]);
								break;

							case 15:								// SDI-TPG1
							case 16:                      // SDI-TPG2
							case 17:                      // SDI-TPG5
								WriteTxt2( " in ");
								WriteCode2( SDITPGArrTxt[UcharVal - 15]);
								break;
						}
						ClearRestLN2();

						WriteCharLN2( 35, '>');
						break;

					case 1:										// Options test 1:
						switch ( UcharVal ) {
							case 4:								// BB34
							case 6:								// BB56
							case 8:								// BB78
																	// BB options starts at 2
								UcharVal1 = TestAnlBlkUnit( UcharVal-2);
								break;

							case 10:								// AnlTPG
							case 11:
								UcharVal1 = TestAnlTPGUnit( UcharVal-10);
								break;

							case 12:								// SDITSG
							case 13:
							case 14:
								UcharVal1 = TestSDITSGUnit( UcharVal-12);
								break;

							case 15:								// SDITPG
							case 16:
							case 17:
								UcharVal1 = TestSDITPGUnit( UcharVal-15);
								break;

							case 18:								// AES-EBU
								UcharVal1 = TestAESEBUUnit( 0);
								break;

							case 19:								// SDI Genlock
								UcharVal1 = TestSDIGenlockUnit();
								break;

							case 20:								// Internal 2
								UcharVal1 = TestHighPrecisionOCXUnit();
								break;

							case 21:								// TimeClock
								UcharVal1 = TestTimeClockUnit();
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

						UcharVal = FindUnit( UP, UcharVal);
						break;

					case 6:								// Options test: ALL result
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

				if ( UcharVal) {
					if ( IntVal < 5)
						IntVal++;
					else
						IntVal = 0;
				}
				else {
					UcharVal = FindUnit( UP, 3);     // Find first unit to test
					IntVal++;
				}
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
/* Revised:		980509				     													*/
/*																									*/
/*	Function:	Test the RS232 connection via a loopback conector.				*/
/*	Remarks:		IntVal holds the timer value, ie. the arrows						*/
/*					UcharVal holds the information about the loopback connector	*/
/*					UcharVal1 holds the result of the RS232 test						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseRS232Test(){

	register int i;

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
/* Revised:		980509				     													*/
/*																									*/
/*	Function:	Test the display.															*/
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
/* Author:		Kim Engedahl, DEV, 980212	    										*/
/* Revised:		980509				     													*/
/*																									*/
/*	Function:	Test the keyboard.														*/
/*	Remarks:		IntVal holds the timer value, ie. the arrows.					*/
/*					UcharVal holds the button to be pressed.							*/
/*					UcharVal1 is TRUE if the right button has been pressed.		*/
/*					UcharVal2 holds the number of buttons pressed.					*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseKeyboardTest(){

	UC ButtonPressed;

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

	if ( ButtonCommand != NOButton)
		ButtonPressed = ButtonCommand;
	else
		ButtonPressed = FALSE;

										// If update signal or correct button pressed..
	if ( UpdateSignal || ButtonPressed) {
		UpdateSignal = FALSE;

		if ( FuncState == 1) {
			switch( IntVal) {
				case 0:									// Button test: setup
					WriteCodeLN2( 0, PressButtonHdrTxt);
					WriteCode2( ButtonArrTxt[UcharVal-1]);
					ClearRestLN2();

					WriteCharLN2( 35, '>');
					break;

				default:									// DO NOT MOVE THIS CASE!!!!!
					if ( ButtonPressed) {			// If button pressed..
						IntVal = 5;						//  immediately leave test mode

						switch ( ButtonPressed) {
							case LEFTButton:
								UcharVal1 = ( UcharVal == 1);
								break;

							case DOWNButton:
								UcharVal1 = ( UcharVal == 2);
								break;

							case RIGHTButton:
								UcharVal1 = ( UcharVal == 3);
								break;

							case EXECButton:
								UcharVal1 = ( UcharVal == 4);
								break;

							case CBARButton:
								UcharVal1 = ( UcharVal == 5);
								break;

							case MBURSTButton:
								UcharVal1 = ( UcharVal == 6);
								break;

							case WINDOWButton:
								UcharVal1 = ( UcharVal == 7);
								break;

							case PRESETButton:
								UcharVal1 = ( UcharVal == 8);
								break;

							case SPECIALButton:
								UcharVal1 = ( UcharVal == 9);
								break;

							case LINEARITYButton:
								UcharVal1 = ( UcharVal == 10);
								break;

							case PATTERNButton:
								UcharVal1 = ( UcharVal == 11);
								break;

							case OUTPUTButton:
								UcharVal1 = ( UcharVal == 12);
								break;

							default:
								UcharVal1 = 0;
								break;
						}
					}
					else {								// otherwise..
						WriteChar2( '>');				//  display next arrow
						break;
					}
															// break IS missing!!
				case 5:
					if ( UcharVal1) {
						UcharVal2++;
						WriteCodeLN2( 35, OKTxt);
					}
					else
						WriteCodeLN2( 35, FAILTxt);

					ClearRestLN2();
					break;

				case 6:
					UcharVal++;							// Prepare for next button

					if ( UcharVal > 12) {			// If no more buttons..
						if ( UcharVal2 < 12)			//  If not all buttons pressed.
							FuncState = 3;          //   display FAIL text..
						else								//  otherwise..
							FuncState = 2;				//   display OK text

						WriteCodeLN2( 0, TestingButtonsCompletedTxt);
						ClearRestLN2();

						UcharVal = 1;					// Prepare for button 1
						UcharVal2 = 0;					// Reset all buttons pressed
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
/* Revised:		980509				     													*/
/*																									*/
/*	Function:	Test the ROM/RAM memory.												*/
/*	Remarks:		IntVal holds the timer value, ie. the arrows.					*/
/*					UcharVal holds the result of the PROM check.						*/
/*					UcharVal1 holds the result of the RAM check.						*/
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
					UcharVal1 = DiagnoseRAMCheckOK();

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
/* Revised:		990308				     													*/
/*																									*/
/*	Function:	Display the configuration, (menu: DIAGNOSE/CONFIGUATION)		*/
/*	Remarks:		Only options detected during power-up will be displayed.		*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseConfigTest(){

	UC tmp, HWType, HWVersion, SWVersion, HWInformation;
	UL KUNumber;

	if ( FuncEscape) {

		Flags = TmpFlags;							// Restore flags

		SelectModeOn = FALSE;					// Clear select-mode

		Ptr = PrevPtr;								// Set pointer to previous menu
		Ptr->DispFunct();							// Display previous menu
		return;
	}

	if ( FuncTriggers & 0x03) {				// If UP/DOWN button pressed..
		if ( FuncTriggers & 0x01) 				// If button pressed is UP..
			UcharVal = FindUnit( UP, UcharVal);
		else											// ..button pressed is DOWN
			UcharVal = FindUnit( DOWN, UcharVal);
	}

	switch ( UcharVal ) {
		case 0:										// MainBoard
			Cstrcpy( TextBuffer, MainConnTxt);
			WriteTxtLN2( 0, TextBuffer);

			strcpy( TextBuffer, "PT5230");

			KUNumber = MainBoard.KUNumber;
			SWVersion = MainBoard.SWVersion;
			HWVersion = MainBoard.HWVersion;
			HWType = MainBoard.HWType;
			break;

		case 1:										// OSCBoard
			Cstrcpy( TextBuffer, OSCConnTxt);
			WriteTxtLN2( 0, TextBuffer);

			Cstrcpy( TextBuffer, HyphenTxt);

			KUNumber = SPGBoard.KUNumber;
			SWVersion = SPGBoard.SWVersion;
			HWVersion = SPGBoard.HWVersion;
			HWType = SPGBoard.HWType;
			break;

		case 2:										// AnlBlkBurst
		case 4:
		case 6:
		case 8:
			tmp = UcharVal - 2;

			Cstrcpy( TextBuffer, BBConnTxt);

			TextBuffer[4] = '1' + tmp;
			TextBuffer[5] = TextBuffer[4]+1;

			WriteTxtLN2( 0, TextBuffer);

			if ( UcharVal < 4)
				Cstrcpy( TextBuffer, HyphenTxt);

			KUNumber = AnlBlkUnit[tmp].KUNumber;
			SWVersion = AnlBlkUnit[tmp].SWVersion;
			HWVersion = AnlBlkUnit[tmp].HWVersion;
			HWType = AnlBlkUnit[tmp].HWType;
			break;

		case 10:										// AnlTPG
		case 11:
			tmp = UcharVal - 10;

			Cstrcpy( TextBuffer, ATPGConnTxt);

			( tmp == 0) ? ( TextBuffer[5] = '2') : (	TextBuffer[5] = '5');

			WriteTxtLN2( 0, TextBuffer);

			KUNumber = AnlTPGUnit[tmp].KUNumber;
			SWVersion = AnlTPGUnit[tmp].SWVersion;
			HWVersion = AnlTPGUnit[tmp].HWVersion;
			HWType = AnlTPGUnit[tmp].HWType;
			break;

		case 12:										// SDITSG
		case 13:
		case 14:
			tmp = UcharVal - 12;

			Cstrcpy( TextBuffer, STSGConnTxt);
			TextBuffer[5] = '2' + tmp;

			WriteTxtLN2( 0, TextBuffer);

			KUNumber = SDITSGUnit[tmp].KUNumber;
			SWVersion = SDITSGUnit[tmp].SWVersion;
			HWVersion = SDITSGUnit[tmp].HWVersion;
			HWType = SDITSGUnit[tmp].HWType;
			break;

		case 15:										// SDITPG
		case 16:
		case 17:
			tmp = UcharVal - 15;

			Cstrcpy( TextBuffer, STPGConnTxt);

			if ( UcharVal < 17)
				TextBuffer[5] = '1' + tmp;
			else
				TextBuffer[5] = '5';

			WriteTxtLN2( 0, TextBuffer);

			KUNumber = SDITPGUnit[tmp].KUNumber;
			SWVersion = SDITPGUnit[tmp].SWVersion;
			HWVersion = SDITPGUnit[tmp].HWVersion;
			HWType = SDITPGUnit[tmp].HWType;
			HWInformation = SDITPGUnit[tmp].HWInformation;
			break;

		case 18:										// AES-EBU
			Cstrcpy( TextBuffer, AESEBUConnTxt);
			WriteTxtLN2( 0, TextBuffer);

			KUNumber = AESEBUUnit.KUNumber;
			SWVersion = AESEBUUnit.SWVersion;
			HWVersion = AESEBUUnit.HWVersion;
			HWType = AESEBUUnit.HWType;
			break;

		case 19:										// SDI Genlock
			Cstrcpy( TextBuffer, SDIGenlockConnTxt);
			WriteTxtLN2( 0, TextBuffer);

			KUNumber = SDIGenlockUnit.KUNumber;
			SWVersion = SDIGenlockUnit.SWVersion;
			HWVersion = SDIGenlockUnit.HWVersion;
			HWType = SDIGenlockUnit.HWType;
			break;

		case 20:										// Internal 2
			Cstrcpy( TextBuffer, Int2GenlockConnTxt);
			WriteTxtLN2( 0, TextBuffer);

			KUNumber = Int2GenlockUnit.KUNumber;
			SWVersion = Int2GenlockUnit.SWVersion;
			HWVersion = Int2GenlockUnit.HWVersion;
			HWType = Int2GenlockUnit.HWType;
			break;

		case 21:										// TimeClock
			Cstrcpy( TextBuffer, TimeConnTxt);
			WriteTxtLN2( 0, TextBuffer);

			KUNumber = TimeClockUnit.KUNumber;
			SWVersion = TimeClockUnit.SWVersion;
			HWVersion = TimeClockUnit.HWVersion;
			HWType = TimeClockUnit.HWType;
			break;
	}

	if ( UcharVal > 3)									// Mainboard, Oscillator
		sprintf( TextBuffer, "PT86%02d", HWType);	//   & BB12 are special

	WriteTxt2( TextBuffer);

	if ( HWVersion) {
		sprintf( TextBuffer, "/9%02d", HWVersion);
		WriteTxt2( TextBuffer);
	}
	else {
		if (( HWType == PT8632) && ( HWInformation == 1))
			WriteTxt2( "/10");
	}

	sprintf( TextBuffer, ", KU%06.0f, ", ( float) KUNumber);
	WriteTxt2( TextBuffer);

	if ( SWVersion)
		sprintf( TextBuffer, "%04.1f", ( float) SWVersion/10.0);
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
/* Revised:		980509				     													*/
/*																									*/
/*	Function:	Display the errorqueue.													*/
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
/* Revised:		991025				     													*/
/*																									*/
/*	Function:	Reset the errorqueue.													*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void DiagnoseErrorQueueReset(){

	register int i;

	if ( FuncSave || FuncEscape) {

		if ( FuncSave) {
			for( i = 0 ; i < NoInErrorQueue; i++)
				ErrorQueue[i] = 0;

			LevelStatus = 0;					// Reset last detected level error

			ErrorStatus = 0;
			ErrorQueuePtr = NoInErrorQueue-1;
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
/* AnlTimeAdjust																	  MENU.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 960621	    										*/
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Update the delay in the analog units.								*/
/*	Remarks:		The parameter <item> is based the items: AnlBlkItem,			*/
/*					AnlTPG2Item-AnlTPG5Item & GenlockItem								*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void AnlTimeAdjust( UC item, UC system, long delay) {

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

	if ( system < NTSC) {						// If system is PAL..
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
		if ( delay == samples) {
			if ( FuncState == 11)
				FuncState--;
			else
				FuncState -= 4;
		}

		switch ( item) {
			case AnlBlkItem:
				TrxAnlBlkDelay( AnlBlkNdx, system, samples);
				break;

			case AnlTPG2Item:
			case AnlTPG5Item:
				TrxAnlTPGDelay( item - AnlTPG2Item, system, samples);
				break;

			case GenlockItem:
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

	AnlSamplesToFLT( system, samples, &sign, &field, &line, &time);
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
/* Revised:		980507				     													*/
/*																									*/
/*	Function:	Update the delay in the digital units.								*/
/*	Remarks:		The parameter <item> is based on the items:						*/
/*					SDITSG2Item-SDITSG4Item, SDITPG1-SDITPG5Item & GenlockItem.	*/
/*	Returns:		--																				*/
/*	Updates:		TBD																			*/
/***************************************************************************/
void SDITimeAdjust( UC item, UC system, long delay) {

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

	if ( system == SDI625) {					// If system is SDI625..
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
		if ( delay == samples) {
			if ( FuncState == 11)
				FuncState--;
			else
				FuncState -= 4;
		}

		switch ( item) {
			case SDITSG2Item:
			case SDITSG3Item:
			case SDITSG4Item:
				TrxSDITSGDelay( item - SDITSG2Item, system, samples);
				break;

			case SDITPG1Item:
			case SDITPG2Item:
			case SDITPG5Item:
				TrxSDITPGDelay( item - SDITPG1Item, system, samples);
				break;

			case GenlockItem:
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

	SDISamplesToFLT( system, samples, &sign, &field, &line, &time);
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
