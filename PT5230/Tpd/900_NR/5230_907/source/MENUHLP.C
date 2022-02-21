/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		MENUHLP.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980122																		*/
/*	Rev. date:	990305 KEn, DEV															*/
/*	Status:		Version 1.2																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*		void WriteTestArrows( UC NoOfArrows);											*/
/*		void WriteItemArrows( UC start, UC length);									*/
/*		void WriteItemBrackets( UC start, UC length);								*/
/*		void WriteArrowsField( UC No);													*/
/*		void StatusItemLeft();																*/
/*		void StatusItemRight();																*/
/*		void BaseItemLeft();																	*/
/*		void BaseItemRight();																*/
/*		void ItemLeft();																		*/
/*		void ItemRight();																		*/
/*		void AnlBlkItemLeft();																*/
/*		void AnlBlkItemRight();																*/
/*		void SDITPGAudioItemLeft();														*/
/*		void SDITPGAudioItemRight();														*/
/*		void AnlTPGTextItemLeft();															*/
/*		void AnlTPGTextItemRight();														*/
/*		void SDITPGTextItemLeft();															*/
/*		void SDITPGTextItemRight();														*/
/*		void AESEBUnItemLeft();																*/
/*		void AESEBUnItemRight();                                             */
/*		void ConfigItemLeft();																*/
/*		void ConfigItemRight();																*/
/*		void WriteType1_SAVE_ESC( char FuncState);									*/
/*		void UpButType1();																	*/
/*		void DownButType1();																	*/
/*		void LeftButType1();																	*/
/*		void RightButType1();																*/
/*		void ExecButType1();																	*/
/*		void WriteType2_SAVE_ESC( char FuncState);									*/
/*		void UpButType2();																	*/
/*		void DownButType2();																	*/
/*		void LeftButType2();																	*/
/*		void RightButType2();																*/
/*		void ExecButType2();																	*/
/*		void WriteType3_OK_ESC( char FuncState);										*/
/*		void UpButType3();																	*/
/*		void DownButType3();																	*/
/*		void LeftButType3();																	*/
/*		void RightButType3();																*/
/*		void ExecButType3();																	*/
/*		void WriteType4_SAVE_ESC( char FuncState);									*/
/*		void UpButType4();																	*/
/*		void DownButType4();																	*/
/*		void LeftButType4();																	*/
/*		void RightButType4();																*/
/*		void WriteType5_SAVE_ESC( char FuncState);									*/
/*		void UpButType5();																	*/
/*		void DownButType5();																	*/
/*		void LeftButType5();																	*/
/*		void RightButType5();																*/
/*		void ExecButType5();																	*/
/*		void WriteType6_SAVE_ESC( char FuncState);									*/
/*		void UpButType6();																	*/
/*		void DownButType6();																	*/
/*		void LeftButType6();																	*/
/*		void RightButType6();																*/
/*		void ExecButType6();																	*/
/*		void WriteType7_SAVE_ESC( char FuncState);									*/
/*		void UpButType7();																	*/
/*		void DownButType7();																	*/
/*		void LeftButType7();																	*/
/*		void RightButType7();																*/
/*		void ExecButType7();																	*/
/*		void WriteType8_SAVE_ESC( char FuncState);									*/
/*		void UpButType8();																	*/
/*		void DownButType8();																	*/
/*		void LeftButType8();																	*/
/*		void RightButType8();																*/
/*		void ExecButType8();																	*/
/*		void WriteType9_SAVE_ESC( char FuncState);									*/
/*		void UpButType9();																	*/
/*		void DownButType9();																	*/
/*		void LeftButType9();																	*/
/*		void RightButType9();																*/
/*		void ExecButType9();																	*/
/*		void WriteType10_SAVE_ESC( char FuncState);									*/
/*		void UpButType10();																	*/
/*		void LeftButType10();																*/
/*		void RightButType10();																*/
/*		void ExecButType10();																*/
/*		void WriteType11_OK_FAIL( char FuncState);									*/
/*		void UpButType11();																	*/
/*		void ExecButType11();																*/
/*		void WriteType12_SAVE_ESC( char FuncState);									*/
/*		void UpButType12();																	*/
/*		void DownButType12();																*/
/*		void LeftButType12();																*/
/*		void RightButType12();																*/
/*		void ExecButType12();																*/
/*		void WriteType13_SAVE_ESC( char FuncState);									*/
/*		void UpButType13();																	*/
/*		void DownButType13();																*/
/*		void LeftButType13();																*/
/*		void RightButType13();																*/
/*		void ExecButType13();																*/
/*																									*/
/*	Changes:																						*/
/* 990305: Change in LeftButType8()														*/
/* 980915: Updated state machine for Type13											*/
/* 980826: Optimized WriteArrowsField()												*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>

#include "mas.h"
#include "menu.h"
#include "menuhlp.h"
#include "menutree.h"
#include "disp_drv.h"
#include "text.h"
#include "keyb_drv.h"
#include "util.h"

// Variables for the state machine(s):

UC FuncState;				// Current state in state machine
UC FuncTriggers;			// Bit 1,0:  01-> X++, 10 -> X--
								// Bit 3,2:  01-> Y++, 10 -> Y--
								// Bit 5,4:  01-> Z++, 10 -> Z--
								// Bit 7,6:  01-> R++, 10 -> R--

bit FuncSave;				// if 1, do a (controlled) save
bit FuncEscape;			// if 1, do a (controlled) escape

/***************************************************************************/
/*	WriteTestArrows														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Write number (<4) of "right"-arrows eg. >>>> on line 2		*/
/*	Remarks:		Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void WriteTestArrows( UC NoOfArrows) {

	register int i;

	for ( i = 1; i <= NoOfArrows; i++)
		WriteChar2( '>');

	for ( ; i <= 4; i++)
		WriteChar2( ' ');
}

/***************************************************************************/
/*	WriteItemArrows														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Hightlight selected item , ie. write arrows, (<txt>),			*/
/*					around the item.															*/
/*	Remarks:		Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void WriteItemArrows( UC start, UC length) {

	WriteCharLN2( start - 1, '<');
	WriteCharLN2( start + length, '>');
}

/***************************************************************************/
/*	WriteItemBrackets														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Hightlight selected item , ie. write brackets, ([txt]),		*/
/*					around the item.															*/
/*	Remarks:		Cursor position on line 2 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void WriteItemBrackets( UC start, UC length) {

	WriteCharLN2( start - 1, '[');
	WriteCharLN2( start + length, ']');
}

/***************************************************************************/
/*	WriteArrowsField														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		980826				     													*/
/*																									*/
/*	Function:	Indicates the keyboard-selection, (ie. keys that can be		*/
/*					used), the upper right corner of the display, (ie. line 1	*/
/*					pos 36-39). Also displays a possible padlock						*/
/*	Remarks:		Special characters, which are not included in the standard	*/
/*					character set, are written to the eight user definable 		*/
/*					characters in the display-controller								*/
/*					Cursor position on line 1 is updated								*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void WriteArrowsField( UC No) {

	char tmpChar;

	// Array Definition:
		// Error =inverse '!' 	0
		// Arrow LEFT 				1
		// Arrow UP/DOWN 			2
		// Arrow RIGHT 			3
		// Execute icon 			4
		// Lock icon 				5
		// Arrow UP 				6
		// Arrow DOWN 				7
		// Not Equal icon 		8

	// Character defintion as respect to addresses
		// Adr 0: Arrow LEFT
		// Adr 1: Arrow UP/DOWN or arrow UP or arrow DOWN
		// Adr 2: Arrow RIGHT
		// Adr 3: Execute icon
		// Adr 4: Lock icon
		// Adr 5: Not Equal icon
		// Adr 6: Used in local routines
		//			 ConfigLCDContrastSelect Bar10/10 Adr. 26
		// Adr 7: Used in local routines

	if ( No & L_) {
		WriteCGChar( 0, 0x01);				// LEFT
		tmpChar = 0;
	}
	else
		tmpChar = ' ';

	WriteCharLN1( 36, tmpChar);


	switch ( No & 0x0A) {
		case D_:
			WriteCGChar( 1, 0x07);
			tmpChar = 1;						// DOWN
			break;

		case U_:
			WriteCGChar( 1, 0x06);
			tmpChar = 1;						// UP
			break;

		case D_+U_:
			WriteCGChar( 1, 0x02);
			tmpChar = 1;						// UP/DOWN
			break;

		default:
			tmpChar = ' ';
			break;
	}

	WriteCharLN1( 37, tmpChar);


	if ( No & R_) {
		WriteCGChar( 2, 0x03);				// RIGHT
		tmpChar = 2;
	}
	else
		tmpChar = ' ';

	WriteCharLN1( 38, tmpChar);


	if ( No & P_) {
		WriteCGChar( 4, 0x05);				// Lock Icon
		tmpChar = 4;
	}

	if ( No & E_) {
		WriteCGChar( 3, 0x04);				// Execute Icon
		tmpChar = 3;
	}
	else
		tmpChar = ' ';

	WriteCharLN1( 39, tmpChar);


	if ( No & MORE_)
		WriteCodeLN2( 37, ThreeDotsTxt);
}

/***************************************************************************/
/*	StatusItemLeft															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980504				     													*/
/*																									*/
/*	Function:	Select previous item in the STATUS MENU							*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void StatusItemLeft() {

	register int Found;

	do {
		Ptr = Ptr->PrevItem;

		Found = TRUE;

		switch ( Ptr->ItemNumber) {
			case StatusAnlTPG2Item:
				if ( !AnlTPGUnit[0].Present)
					Found = FALSE;
				break;

			case StatusAnlTPG5Item:
				if ( !AnlTPGUnit[1].Present)
					Found = FALSE;
				break;

			case StatusSDITSG2Item:
				if ( !SDITSGUnit[0].Present)
					Found = FALSE;
				break;

			case StatusSDITSG3Item:
				if ( !SDITSGUnit[1].Present)
					Found = FALSE;
				break;

			case StatusSDITSG4Item:
				if ( !SDITSGUnit[2].Present)
					Found = FALSE;
				break;

			case StatusSDITPG1Item:
				if ( !SDITPGUnit[0].Present)
					Found = FALSE;
				break;

			case StatusSDITPG2Item:
				if ( !SDITPGUnit[1].Present)
					Found = FALSE;
				break;

			case StatusSDITPG5Item:
				if ( !SDITPGUnit[2].Present)
					Found = FALSE;
				break;

			case StatusAESEBU1Item:
				if ( !AESEBUUnit.Present)
					Found = FALSE;
				break;

			case StatusAESEBU2Item:
				if ( !AESEBUUnit.Present || ( AESEBUUnit.HWType == PT8605))
					Found = FALSE;
				break;

			case StatusDateTimeItem:
				if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637))
					Found = FALSE;
				break;
		}
	} while ( !Found);
}

/***************************************************************************/
/*	StatusItemRight														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980504				     													*/
/*																									*/
/*	Function:	Select next item in the STATUS MENU									*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void StatusItemRight() {

	register int Found;

	do {
		Ptr = Ptr->NextItem;

		Found = TRUE;

		switch ( Ptr->ItemNumber) {
			case StatusAnlTPG2Item:
				if ( !AnlTPGUnit[0].Present)
					Found = FALSE;
				break;

			case StatusAnlTPG5Item:
				if ( !AnlTPGUnit[1].Present)
					Found = FALSE;
				break;

			case StatusSDITSG2Item:
				if ( !SDITSGUnit[0].Present)
					Found = FALSE;
				break;

			case StatusSDITSG3Item:
				if ( !SDITSGUnit[1].Present)
					Found = FALSE;
				break;

			case StatusSDITSG4Item:
				if ( !SDITSGUnit[2].Present)
					Found = FALSE;
				break;

			case StatusSDITPG1Item:
				if ( !SDITPGUnit[0].Present)
					Found = FALSE;
				break;

			case StatusSDITPG2Item:
				if ( !SDITPGUnit[1].Present)
					Found = FALSE;
				break;

			case StatusSDITPG5Item:
				if ( !SDITPGUnit[2].Present)
					Found = FALSE;
				break;

			case StatusAESEBU1Item:
				if ( !AESEBUUnit.Present)
					Found = FALSE;
				break;

			case StatusAESEBU2Item:
				if ( !AESEBUUnit.Present || ( AESEBUUnit.HWType == PT8605))
					Found = FALSE;
				break;

			case StatusDateTimeItem:
				if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637))
					Found = FALSE;
				break;
		}
	} while ( !Found);
}

/***************************************************************************/
/*	BaseItemLeft															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980125	    										*/
/* Revised:		980125				     													*/
/*																									*/
/*	Function:	Select previous item in the BASE MENU								*/
/*	Remarks:		A special test for the existence of the TEST-MENU is			*/
/*					performed																	*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void BaseItemLeft() {

	register int Found;

	do {
		Ptr = Ptr->PrevItem;

		Found = TRUE;

		switch ( Ptr->ItemNumber) {
			case AnlTPG2Item:
				if ( !AnlTPGUnit[0].Present)
					Found = FALSE;
				break;

			case AnlTPG5Item:
				if ( !AnlTPGUnit[1].Present)
					Found = FALSE;
				break;

			case SDITSG2Item:
				if ( !SDITSGUnit[0].Present)
					Found = FALSE;
				break;

			case SDITSG3Item:
				if ( !SDITSGUnit[1].Present)
					Found = FALSE;
				break;

			case SDITSG4Item:
				if ( !SDITSGUnit[2].Present)
					Found = FALSE;
				break;

			case SDITPG1Item:
				if ( !SDITPGUnit[0].Present)
					Found = FALSE;
				break;

			case SDITPG2Item:
				if ( !SDITPGUnit[1].Present)
					Found = FALSE;
				break;

			case SDITPG5Item:
				if ( !SDITPGUnit[2].Present)
					Found = FALSE;
				break;

			case AESEBUItem:
				if ( !AESEBUUnit.Present)
					Found = FALSE;
				break;

			case TestItem:
				Found = ( UC) TestModeEnable;
				break;
		}
	} while ( !Found);
}

/***************************************************************************/
/*	BaseItemRight															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980122				     													*/
/*																									*/
/*	Function:	Select next item in the BASE MENU									*/
/*	Remarks:		A special test for the existence of the TEST-MENU is			*/
/*					performed																	*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void BaseItemRight() {

	register int Found;

	do {
		Ptr = Ptr->NextItem;

		Found = TRUE;

		switch ( Ptr->ItemNumber) {
			case AnlTPG2Item:
				if ( !AnlTPGUnit[0].Present)
					Found = FALSE;
				break;

			case AnlTPG5Item:
				if ( !AnlTPGUnit[1].Present)
					Found = FALSE;
				break;

			case SDITSG2Item:
				if ( !SDITSGUnit[0].Present)
					Found = FALSE;
				break;

			case SDITSG3Item:
				if ( !SDITSGUnit[1].Present)
					Found = FALSE;
				break;

			case SDITSG4Item:
				if ( !SDITSGUnit[2].Present)
					Found = FALSE;
				break;

			case SDITPG1Item:
				if ( !SDITPGUnit[0].Present)
					Found = FALSE;
				break;

			case SDITPG2Item:
				if ( !SDITPGUnit[1].Present)
					Found = FALSE;
				break;

			case SDITPG5Item:
				if ( !SDITPGUnit[2].Present)
					Found = FALSE;
				break;

			case AESEBUItem:
				if ( !AESEBUUnit.Present)
					Found = FALSE;
				break;

			case TestItem:
				Found = ( UC) TestModeEnable;
				break;
		}
	} while ( !Found);
}

/***************************************************************************/
/*	ItemLeft																	 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980122				     													*/
/*																									*/
/*	Function:	Select previous item in the general menu-/submenu-system		*/
/*	Remarks:		This function does NOT apply to the BASE-menu					*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void ItemLeft() {

	Ptr = Ptr->PrevItem;
}

/***************************************************************************/
/*	ItemRight																 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980122				     													*/
/*																									*/
/*	Function:	Select next item in the general menu-/submenu-system			*/
/*	Remarks:		This function does NOT apply to the BASE-menu					*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void ItemRight() {

	Ptr = Ptr->NextItem;
}

/***************************************************************************/
/*	AnlBlkItemLeft															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980122				     													*/
/*																									*/
/*	Function:	Select previous item in the BLACK-BURST MENU						*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AnlBlkItemLeft() {

	do {
		Ptr = Ptr->PrevItem;
	} while ( !AnlBlkUnit[Ptr->ItemNumber].Present);
}

/***************************************************************************/
/*	AnlBlkItemRight														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980122	    										*/
/* Revised:		980122				     													*/
/*																									*/
/*	Function:	Select next item in the BLACK-BURST menu.							*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AnlBlkItemRight() {

	do {
		Ptr = Ptr->NextItem;
	} while ( !AnlBlkUnit[Ptr->ItemNumber].Present);
}

/***************************************************************************/
/*	SDITPGAudioItemLeft													 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		980504				     													*/
/*																									*/
/*	Function:	Select previous item in the SDI-TPGx/EMB.AUDIO menu.			*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void SDITPGAudioItemLeft() {

	Ptr = Ptr->PrevItem;

	if ( Ptr->ItemNumber == SDITPGAudioGroupItem)
		if ( SDITPGUnit[BaseNdx - SDITPG1Item].HWType != PT8633)
			Ptr = Ptr->PrevItem;
}

/***************************************************************************/
/*	SDITPGAudioItemRight													 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		980504				     													*/
/*																									*/
/*	Function:	Select next item in the SDI-TPGx/EMB.AUDIO menu.				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void SDITPGAudioItemRight() {

	Ptr = Ptr->NextItem;

	if ( Ptr->ItemNumber == SDITPGAudioGroupItem)
		if ( SDITPGUnit[BaseNdx - SDITPG1Item].HWType != PT8633)
			Ptr = Ptr->NextItem;
}

/***************************************************************************/
/*	AnlTPGTextItemLeft													 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		980929				     													*/
/*																									*/
/*	Function:	Select previous item in the ANALOG-TPGx/TEXT menu.				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AnlTPGTextItemLeft() {

	Ptr = Ptr->PrevItem;

	if ( Ptr->ItemNumber == TPGTextClockItem)
		if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637) ||
			( AnlTPGUnit[BaseNdx-AnlTPG2Item].HWType == PT8601))
				Ptr = Ptr->PrevItem;
}

/***************************************************************************/
/*	AnlTPGTextItemRight													 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		980929				     													*/
/*																									*/
/*	Function:	Select next item in the ANALOG-TPGx/TEXT menu.					*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AnlTPGTextItemRight() {

	Ptr = Ptr->NextItem;

	if ( Ptr->ItemNumber == TPGTextClockItem)
		if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637) ||
			( AnlTPGUnit[BaseNdx-AnlTPG2Item].HWType == PT8601))
				Ptr = Ptr->NextItem;
}

/***************************************************************************/
/*	SDITPGTextItemLeft													 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		980929				     													*/
/*																									*/
/*	Function:	Select previous item in the SDI-TPGx/TEXT menu.					*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void SDITPGTextItemLeft() {

	Ptr = Ptr->PrevItem;

	if ( Ptr->ItemNumber == TPGTextClockItem)
		if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637) ||
			( SDITPGUnit[BaseNdx-SDITPG1Item].HWType < PT8632))
				Ptr = Ptr->PrevItem;
}

/***************************************************************************/
/*	SDITPGTextItemRight													 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980224	    										*/
/* Revised:		980504				     													*/
/*																									*/
/*	Function:	Select next item in the SDI-TPGx/TEXT menu.						*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void SDITPGTextItemRight() {

	Ptr = Ptr->NextItem;

	if ( Ptr->ItemNumber == TPGTextClockItem)
		if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637) ||
			( SDITPGUnit[BaseNdx-SDITPG1Item].HWType < PT8632))
				Ptr = Ptr->NextItem;
}

/***************************************************************************/
/*	AESEBUnItemLeft														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980216	    										*/
/* Revised:		980216				     													*/
/*																									*/
/*	Function:	Select previous item in the AESEBU MENU							*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AESEBUnItemLeft() {

	Ptr = Ptr->PrevItem;

	if ( Ptr->ItemNumber == AESEBU2Item)
		if ( AESEBUUnit.HWType == PT8605)
			Ptr = Ptr->PrevItem;
}

/***************************************************************************/
/*	AESEBUnItemRight														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980216	    										*/
/* Revised:		980216				     													*/
/*																									*/
/*	Function:	Select next item in the AESEBU MENU									*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void AESEBUnItemRight() {

	Ptr = Ptr->NextItem;

	if ( Ptr->ItemNumber == AESEBU2Item)
		if ( AESEBUUnit.HWType == PT8605)
			Ptr = Ptr->NextItem;
}

/***************************************************************************/
/*	ConfigItemLeft															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980129	    										*/
/* Revised:		980504				     													*/
/*																									*/
/*	Function:	Select previous item in the CONFIG MENU							*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void ConfigItemLeft() {

	Ptr = Ptr->PrevItem;

	if ( Ptr->ItemNumber == ConfigDateTimeItem)
		if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637))
			Ptr = Ptr->PrevItem;
}

/***************************************************************************/
/*	ConfigItemRight														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980129	    										*/
/* Revised:		980504				     													*/
/*																									*/
/*	Function:	Select next item in the CONFIG MENU									*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void ConfigItemRight() {

	Ptr = Ptr->NextItem;

	if ( Ptr->ItemNumber == ConfigDateTimeItem)
		if ( !TimeClockUnit.Present || ( TimeClockUnit.HWType != PT8637))
			Ptr = Ptr->NextItem;
}

/***************************************************************************/
/*	EscFunction																 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970725				     													*/
/*																									*/
/*	Function:	Function to terminate a menu/selection without using the		*/
/*					 ESC-item in the menu													*/
/*	Remarks:		FuncSave MUST be set to FALSE, as the EscFunction may be 	*/
/*					 called from within a save function, see MENU.C51				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void EscFunction() {

	FuncSave = FALSE;
	FuncEscape = TRUE;
}

/***************************************************************************/
/*	WriteType1_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <SAVE ESC> display 		*/
/*					handling																		*/
/*	Remarks:		Part of <SAVE ESC> display handling									*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		Vertical_Repeat															*/
/***************************************************************************/
void WriteType1_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);

	if ( FuncState > 2)
		WriteCode2( SAVETxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
		case 3:
			WriteArrowsField( L_+D_+U_+R_);

			VerticalRepeat = TRUE;
			break;

		case 4:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			VerticalRepeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = FALSE;
			break;
	}
}

/***************************************************************************/
/*	UpButType1																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <SAVE ESC> state machine			*/
/*	Remarks:		Part of <SAVE ESC> display handling									*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType1() {

	switch ( FuncState) {
		case 1:
			FuncState = 3;								  		// Break IS missing
		case 3:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;
			break;

		case 2:
		case 5:
			FuncEscape = TRUE;
			return;
	}
}

/***************************************************************************/
/*	DownButType1																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <SAVE ESC> state machine		*/
/*	Remarks:		Part of <SAVE ESC> display handling									*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType1() {

	switch ( FuncState) {
		case 1:
			FuncState = 3;										// Break IS missing
		case 3:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;
			break;
	}
}

/***************************************************************************/
/*	LeftButType1																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <SAVE ESC> state machine		*/
/*	Remarks:		Part of <SAVE ESC> display handling									*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType1() {
	switch ( FuncState) {
		case 3:
			FuncState++;										// Break IS missing
		case 1:
			FuncState++;
			break;

		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType1																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <SAVE ESC> state machine		*/
/*	Remarks:		Part of <SAVE ESC> display handling									*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType1() {
	switch (FuncState) {
		case 5:
			FuncState--;										// Break IS missing
		case 2:
			FuncState--;
			break;

		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType1																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <SAVE ESC> state machine	*/
/*	Remarks:		Part of <SAVE ESC> display handling									*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType1() {
	if ( FuncState == 4)
		FuncSave = TRUE;
}

/***************************************************************************/
/*	WriteType2_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <XY SAVE ESC> display 	*/
/*					handling																		*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType2_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);

	if ( FuncState > 3)
		WriteCode2( SAVETxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 3:
		case 7:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = FALSE;
			break;

		case 6:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);
			break;

		default:
			WriteArrowsField( L_+D_+U_+R_);

			VerticalRepeat = TRUE;
			break;
	}
}

/***************************************************************************/
/*	UpButType2																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <XY SAVE ESC> state machine		*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType2() {
	switch ( FuncState) {
		case 1:
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;
			break;

		case 2:
			FuncState = 4;										// Break IS missing
		case 4:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x04;
			break;

		case 3:
		case 7:
			FuncEscape = TRUE;
			return;
	}
}

/***************************************************************************/
/*	DownButType2																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <XY SAVE ESC> state machine	*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType2() {
	switch ( FuncState) {
		case 1:
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;
			break;

		case 2:
		case 4:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x08;
			FuncState = 4;
			break;
	}
}

/***************************************************************************/
/*	LeftButType2																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <XY SAVE ESC> state machine	*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType2() {
	switch ( FuncState) {
		case 1:
		case 5:
			FuncState++;										// Break IS missing
		case 4:
			FuncState++;
			break;

		case 6:
			FuncState--;										// Break IS missing
		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType2																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <XY SAVE ESC> state machine	*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType2() {
	switch (FuncState) {
		case 3:
		case 7:
			FuncState--;										// Break IS missing
		case 5:
			FuncState--;
			break;

		case 4:
			FuncState++;										// Break IS missing
		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType2																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <XY SAVE ESC> state machine*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncSave													*/
/***************************************************************************/
void ExecButType2() {
	if ( FuncState == 6)
		FuncSave = FuncState = 1;
}

/***************************************************************************/
/*	WriteType3_OK_ESC															  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the OK and ESC text in the <OK ESC> display	handling */
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		--																				*/
/***************************************************************************/
void WriteType3_OK_ESC( char FuncState) {

	WriteCodeLN2( 31, OKTxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
			WriteArrowsField( L_+U_+R_+D_);
			break;

		case 2:
			WriteItemArrows( 31, Cstrlen( OKTxt));
			WriteArrowsField( L_+R_+E_);
			break;

		case 3:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);
			break;
	}
}

/***************************************************************************/
/*	UpButType3																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <OK ESC> state machine				*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		Ptr																			*/
/***************************************************************************/
void UpButType3() {

	switch ( FuncState)  {
		case 1:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;
			break;

		case 3:
			FuncEscape = TRUE;
			break;
	}
}

/***************************************************************************/
/*	DownButType3																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <OK ESC> state machine			*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		Ptr																			*/
/***************************************************************************/
void DownButType3() {

	if ( FuncState == 1)
		FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;
}

/***************************************************************************/
/* LeftButType3																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <OK ESC> state machine			*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType3() {
	if ( FuncState == 1)
		FuncState = 3;
	else
		FuncState--;
}

/***************************************************************************/
/*	RightButType3																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <OK ESC> state					*/
/*					machine																		*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType3() {
	if ( FuncState == 3)
		FuncState = 1;
	else
		FuncState++;
}

/***************************************************************************/
/*	ExecButType3																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <OK ESC> state	machine		*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType3() {

	if ( FuncState == 2)
		FuncSave = TRUE;
}

/***************************************************************************/
/*	WriteType4_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the ESC text in the <INSTANT ADJUST LEVEL> display	*/
/*					handling																		*/
/*	Remarks:		Part of <INSTANT ADJUST LEVEL> display handling					*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType4_SAVE_ESC( char FuncState) {

	WriteCodeLN2( 36, ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
			WriteArrowsField( L_+D_+U_+R_);
			VerticalRepeat = TRUE;
			break;

		case 2:
			WriteArrowsField( L_+U_+R_);
			WriteItemArrows( 36, Cstrlen( ESCTxt));

			VerticalRepeat = FALSE;
			break;
	}
}

/***************************************************************************/
/*	UpButType4																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <INSTANT ADJUST LEVEL> state		*/
/*					machine																		*/
/*	Remarks:		Part of <INSTANT ADJUST LEVEL> display handling					*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		Ptr and FuncTriggers														*/
/***************************************************************************/
void UpButType4() {
	switch ( FuncState) {
		case 1:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;
			break;

		case 2:
			FuncEscape = TRUE;
			break;
	}
}

/***************************************************************************/
/*	DownButType4																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <INSTANT ADJUST LEVEL> state	*/
/*					machine																		*/
/*	Remarks:		Part of <INSTANT ADJUST LEVEL> display handling					*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncTriggers																*/
/***************************************************************************/
void DownButType4() {
	if ( FuncState == 1)
		FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;
}

/***************************************************************************/
/*	LeftButType4																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <INSTANT ADJUST LEVEL> state	*/
/*					machine																		*/
/*	Remarks:		Part of <INSTANT ADJUST LEVEL> display handling					*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType4() {
	switch ( FuncState) {
		case 1:
			FuncState++;
			break;

		case 2:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType4																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <INSTANT ADJUST LEVEL> state	*/
/*					machine																		*/
/*	Remarks:		Part of <INSTANT ADJUST LEVEL> display handling					*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType4() {
	switch ( FuncState) {
		case 1:
			FuncState++;
			break;

		case 2:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	WriteType5_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the ESC text in the <EXECUTE TEXT ESC> display			*/
/*					handling																		*/
/*	Remarks:		Part of <EXECUTE TEXT ESC> display handling						*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		--																				*/
/***************************************************************************/
void WriteType5_SAVE_ESC( char FuncState) {

	WriteCodeLN2( 36, ESCTxt);
	ClearRestLN2();

	switch ( FuncState) {
		case 1:
			WriteArrowsField( L_+U_+D_+R_);
			break;

		case 2:
			WriteArrowsField( L_+U_+R_);
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			break;
	}
}

/***************************************************************************/
/*	UpButType5																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <EXECUTE TEXT ESC> state machine*/
/*	Remarks:		Part of <EXECUTE TEXT ESC> display handling						*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncTriggers and FuncSave												*/
/***************************************************************************/
void UpButType5() {

	switch ( FuncState) {
		case 1:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;
			break;

		case 2:
			FuncEscape = TRUE;
			break;
	}
}

/***************************************************************************/
/*	DownButType5																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <EXECUTE TEXT ESC> state		*/
/*					machine																		*/
/*	Remarks:		Part of <EXECUTE TEXT ESC> display handling						*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncTriggers																*/
/***************************************************************************/
void DownButType5() {

	if ( FuncState == 1)
		FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;
}

/***************************************************************************/
/*	LeftButType5																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <EXECUTE TEXT ESC> state		*/
/*					machine																		*/
/*	Remarks:		Part of <EXECUTE TEXT ESC> display handling						*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType5() {

	if ( FuncState == 1)
		FuncState = 2;
	else
		FuncState = 1;
}

/***************************************************************************/
/*	RightButType5																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <EXECUTE TEXT ESC> state		*/
/*					machine																		*/
/*	Remarks:		Part of <EXECUTE TEXT ESC> display handling						*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:   	--																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType5() {

	if ( FuncState == 1)
		FuncState = 2;
	else
		FuncState = 1;
}

/***************************************************************************/
/*	ExecButType5																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <EXECUTE TEXT ESC> state	*/
/*					machine																		*/
/*	Remarks:		Part of <EXECUTE TEXT ESC> display handling						*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType5() {

	if ( FuncState == 1)
		FuncSave = TRUE;
}

/***************************************************************************/
/*	WriteType6_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <SAVE ESC> display 		*/
/*					handling																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType6_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);

	if (( FuncState > 4) && ( FuncState < 10) || ( FuncState == 11))
		WriteCode2( SAVETxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
		case 2:
		case 5:
		case 6:
			WriteArrowsField( L_+D_+U_+R_);

			VerticalRepeat = TRUE;
			break;

		case 3:
		case 7:
		case 10:
		case 11:
			WriteArrowsField( L_+D_+U_+R_+E_);

			VerticalRepeat = TRUE;
			break;

		case 8:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			VerticalRepeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = FALSE;
			break;
	}
}

/***************************************************************************/
/*	UpButType6																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <SAVE ESC> state machine			*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType6() {

	switch ( FuncState) {
		case 1:
			FuncState = 5;										// Break IS missing
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;  // inc xx
			break;

		case 2:
			FuncState = 6;										// Break IS missing
		case 6:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x04;  // inc yy
			break;

		case 3:
			FuncState = 7;										// Break IS missing
		case 7:
			FuncTriggers = ( FuncTriggers & 0xCF) | 0x10;  // inc zz
			break;

		case 4:
		case 9:
			FuncEscape = TRUE;
			break;

		case 10:
			FuncState = 11;									// Break IS missing
		case 11:
			FuncTriggers = ( FuncTriggers & 0xCF) | 0x10;  // inc zz
			break;
	}
}

/***************************************************************************/
/*	DownButType6																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <XY EDIT SAVE ESC> state machine*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType6() {

	switch ( FuncState) {
		case 1:
			FuncState = 5;										// Break IS missing
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;  // dec xx
			break;

		case 2:
			FuncState = 6;										// Break IS missing
		case 6:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x08;  // dec yy
			break;

		case 3:
			FuncState = 7;										// Break IS missing
		case 7:
			FuncTriggers = ( FuncTriggers & 0xCF) | 0x20;  // dec zz
			break;

		case 10:
			FuncState = 11;									// Break IS missing
		case 11:
			FuncTriggers = ( FuncTriggers & 0xCF) | 0x20;  // dec zz
			break;
	}
}

/***************************************************************************/
/*	LeftButType6																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <XY EDIT SAVE ESC> state machine*/
/*	Syntax:     void LeftButType6();														*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType6() {
	switch ( FuncState) {
		case 1:
			FuncState = 4;
			break;

		case 5:
			FuncState = 9;
			break;

		case 10:
		case 11:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x40;	// left rr
			break;

		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType6																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <XY EDIT SAVE ESC> state		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType6() {
	switch ( FuncState) {
		case 4:
			FuncState = 1;
			break;

		case 9:
			FuncState = 5;
			break;

		case 10:
		case 11:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x80;	// right rr
			break;

		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType6																  MENUHLP.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <XY EDIT SAVE ESC> state	*/
/*					machine																		*/
/*	Syntax:     void ExecButType1();														*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    Nothing																		*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType6() {
	switch ( FuncState){
		case 3:
			FuncState = 10;
			break;

		case 7:
			FuncState = 11;
			break;

		case 8:
			FuncSave = TRUE;
			break;

		case 10:
			FuncState = 3;
			break;

		case 11:
			FuncState = 7;
			break;
	}
}

/***************************************************************************/
/*	WriteType7_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the EDIT, SAVE and ESC text in the <EDIT SAVE ESC>	*/
/*					display handling															*/
/*	Remarks:		Part of <EDIT SAVE ESC> display handling							*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType7_SAVE_ESC( char FuncState) {

	if ( FuncState > 5)
		WriteCodeLN2( 22, MODIFYTxt);

	WriteAddressLN2( 30);

	if ((( FuncState >= 3) && ( FuncState <= 5)) ||
		 (( FuncState >= 9) && ( FuncState <= 12)))
		WriteCode2( SAVETxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
		case 3:
		case 6:
		case 9:
			WriteArrowsField( L_+D_+U_+R_);

			VerticalRepeat = TRUE;
			break;

		case 2:
		case 5:
		case 8:
		case 12:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = FALSE;
			break;

		case 4:
		case 11:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			VerticalRepeat = FALSE;
			break;

		case 7:
		case 10:
			WriteItemArrows( 22, Cstrlen( MODIFYTxt));
			WriteArrowsField( L_+D_+R_);

			VerticalRepeat = FALSE;
			break;
	}
}

/***************************************************************************/
/*	UpButType7																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <EDIT SAVE ESC> state machine	*/
/*	Remarks:		Part of <EDIT SAVE ESC> display handling							*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType7() {

	switch ( FuncState) {
		case 1:
			FuncState = 3;									// Break IS missing
		case 3:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;  // inc xx
			break;

		case 2:
		case 5:
		case 8:
		case 12:
			FuncEscape = TRUE;
			break;

		case 6:
			FuncState = 9;									// Break IS missing
		case 9:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;  // inc xx
			break;
	}
}

/***************************************************************************/
/*	DownButType7																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <EDIT SAVE ESC> state machine */
/*	Remarks:		Part of <EDIT SAVE ESC> display handling							*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType7() {

	switch ( FuncState) {
		case 1:
			FuncState = 3;										// Break IS missing
		case 3:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;  // dec xx
			break;

		case 6:
			FuncState = 9;										// Break IS missing
		case 9:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;  // dec xx
			break;

		case 7:
		case 10:
			FuncState = 13;
			break;
	}
}

/***************************************************************************/
/*	LeftButType7																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <EDIT SAVE ESC> state machine	*/
/*	Remarks:		Part of <EDIT SAVE ESC> display handling							*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType7() {

	switch ( FuncState) {
		case 1:
			FuncState = 2;
			break;

		case 3:
			FuncState = 5;
			break;

		case 6:
			FuncState = 8;
			break;

		case 9:
			FuncState = 12;
			break;

		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType7																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <EDIT SAVE ESC> state machine*/
/*	Remarks:		Part of <EDIT SAVE ESC> display handling							*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType7() {

	switch ( FuncState) {
		case 2:
			FuncState = 1;
			break;

		case 5:
			FuncState = 3;
			break;

		case 8:
			FuncState = 6;
			break;

		case 12:
			FuncState = 9;
			break;

		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType7																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980213	    										*/
/* Revised:		980213				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <EDIT SAVE ESC> state 		*/
/*					machine																		*/
/*	Remarks:		Part of <EDIT SAVE ESC> display handling							*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType7() {

	switch ( FuncState){
		case 4:
		case 11:
			FuncSave = TRUE;
			break;
	}
}

/***************************************************************************/
/*	WriteType8_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970624	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <USER TEXT SAVE ESC>		*/
/*					display handling															*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType8_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);

	if (( FuncState >= 3) && ( FuncState <= 5) || ( FuncState == 7))
		WriteCode2( SAVETxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
		case 3:
		case 6:
		case 7:
			WriteArrowsField( L_+D_+U_+R_+E_);

			VerticalRepeat = TRUE;
			break;

		case 4:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			VerticalRepeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = FALSE;
			break;
	}
}

/***************************************************************************/
/*	UpButType8																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <SAVE ESC> state machine			*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType8() {

	switch ( FuncState) {
		case 1:
		case 3:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;  // inc xx
			break;

		case 2:
		case 5:
			FuncEscape = TRUE;
			break;

		case 6:
			FuncState++;
		case 7:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x04;  // inc yy
			break;
	}
}

/***************************************************************************/
/*	DownButType8																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <XY EDIT SAVE ESC> state 		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType8() {

	switch ( FuncState) {
		case 1:
		case 3:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;  /* dec xx */
			break;

		case 6:
			FuncState++;
		case 7:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x08;  // dec yy
			break;
	}
}

/***************************************************************************/
/*	LeftButType8																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <XY EDIT SAVE ESC> state 		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType8() {
	switch ( FuncState) {
		case 1:
			FuncState++;
			break;

		case 3:
			FuncState = 5;
			break;

		case 6:
			FuncState++;
		case 7:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x40;	// right rr
			break;

		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType8																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <XY EDIT SAVE ESC> state 		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType8() {
	switch ( FuncState) {
		case 2:
			FuncState--;
			break;

		case 5:
			FuncState = 3;
			break;

		case 6:
		case 7:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x80;	// left rr
			break;

		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType8																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		971215				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <XY EDIT SAVE ESC> state	*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType8() {
	switch ( FuncState){
		case 1:
			FuncState = 6;
			break;

		case 3:
			FuncState = 7;
			break;

		case 4:
			FuncSave = TRUE;
			break;

		case 6:
			FuncState = 1;
			break;

		case 7:
			FuncState = 3;
			break;
	}
}

/***************************************************************************/
/*	WriteType9_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970624	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the XX, YY, TIME, SAVE and ESC text in the				*/
/*					<XY TIME SAVE ESC> display handling									*/
/*	Remarks:		Part of <XY TIME SAVE ESC> display handling						*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType9_SAVE_ESC( char FuncState) {

	WriteCodeLN2( 19, SYSTxt);

	ClearToPosLN2( 24);
	WriteCode2( TimingTxt);

	if ( FuncState > 4) {
		ClearToPosLN2( 32);
		WriteCode2( OKTxt);
	}

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
		case 5:
			WriteArrowsField( L_+U_+R_+D_);

			VerticalRepeat = FALSE;
			break;

		case 2:
		case 6:
			WriteItemArrows( 19, Cstrlen( SYSTxt));
			WriteArrowsField( L_+R_+D_);

			VerticalRepeat = FALSE;
			break;

		case 3:
		case 7:
			WriteItemArrows( 24, Cstrlen( TimingTxt));
			WriteArrowsField( L_+R_+D_);

			VerticalRepeat = FALSE;
			break;

		case 8:
			WriteItemArrows( 32, Cstrlen( OKTxt));
			WriteArrowsField( L_+R_+E_);

			VerticalRepeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = TRUE;
			break;
	}
}

/***************************************************************************/
/*	UpButType9																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <SAVE ESC> state machine			*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType9() {

	switch ( FuncState) {
		case 1:
			FuncState = 5;
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01; 	 // inc xx
			break;

		case 4:
		case 9:
			FuncEscape = TRUE;
			break;
	}
}

/***************************************************************************/
/*	DownButType9																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <XY EDIT SAVE ESC> state 		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    Nothing																		*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType9() {

	switch ( FuncState) {
		case 1:
			FuncState = 5;
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;  // dec xx
			break;

		case 2:
		case 3:
		case 6:
		case 7:
			FuncState += 30;
			break;
	}
}

/***************************************************************************/
/*	LeftButType9																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <XY EDIT SAVE ESC> state		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    Nothing																		*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType9() {

	switch ( FuncState) {
		case 1:
			FuncState += 3;
			break;

		case 5:
			FuncState += 4;
			break;

		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType9																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <XY EDIT SAVE ESC> state		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType9() {
	switch ( FuncState) {
		case 4:
			FuncState -= 3;
			break;

		case 9:
			FuncState -= 4;
			break;

		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType9																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <XY EDIT SAVE ESC> state	*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType9() {

	if ( FuncState == 8)
		FuncSave = TRUE;
}

/***************************************************************************/
/*	WriteType10_OK_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the OK and ESC text in the <OK ESC> display	handling */
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		--																				*/
/***************************************************************************/
void WriteType10_OK_ESC( char FuncState) {

	WriteCodeLN2( 31, OKTxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
			WriteItemArrows( 31, Cstrlen( OKTxt));
			WriteArrowsField( L_+R_+E_);
			break;

		case 2:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);
			break;
	}
}

/***************************************************************************/
/*	UpButType10																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <OK ESC> state machine				*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		Ptr																			*/
/***************************************************************************/
void UpButType10() {

	if ( FuncState == 2)
		FuncEscape = TRUE;
}

/***************************************************************************/
/* LeftButType10																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <OK ESC> state machine			*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType10() {

	if ( FuncState == 1)
		FuncState = 2;
	else
		FuncState = 1;
}

/***************************************************************************/
/*	RightButType10																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <OK ESC> state					*/
/*					machine																		*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType10() {

	if ( FuncState == 1)
		FuncState = 2;
	else
		FuncState = 1;
}

/***************************************************************************/
/*	ExecButType10																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970401	    										*/
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <OK ESC> state	machine		*/
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    Nothing																		*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType10() {

	if ( FuncState == 1)
		FuncSave = TRUE;
}

/***************************************************************************/
/*	WriteType11_OK_FAIL													     MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970923	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <OK ESC> state	machine			*/
/*	Remarks:	  	Part of <DIAGNOSE> display handling									*/
/*	Returns:		--																				*/
/*	Updates:		FuncSave																		*/
/***************************************************************************/
void WriteType11_OK_FAIL( char FuncState) {

	switch ( FuncState) {
		case 2:
			WriteArrowsField( U_+E_);
			WriteCodeLN2( 35, OKTxt);
			ClearRestLN2();
			break;

		case 3:
			WriteArrowsField( U_+E_);
			WriteCodeLN2( 35, FAILTxt);
			ClearRestLN2();
			break;

		case 4:
			WriteArrowsField( U_+E_);
			break;

		default:
			WriteArrowsField( U_);
			break;
	}
}

/***************************************************************************/
/*	UpButType11																     MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970923	    										*/
/* Revised:		970923				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <OK ESC> state	machine			*/
/*	Remarks:	  	Part of <DIAGNOSE> display handling									*/
/*	Returns:		--																				*/
/*	Updates:		FuncSave																		*/
/***************************************************************************/
void UpButType11() {

	FuncEscape = TRUE;
}

/***************************************************************************/
/*	ExecButType11															     MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970923	    										*/
/* Revised:		970923				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <OK ESC> state	machine		*/
/*	Remarks:	  	Part of <DIAGNOSE> display handling									*/
/*	Returns:		--																				*/
/*	Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType11() {

	if ( FuncState > 1)
		FuncSave = 1;
}

/***************************************************************************/
/*	WriteType12_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980302	    										*/
/* Revised:		980827				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <USER TEXT SAVE ESC>		*/
/*					display handling															*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType12_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);

	if (( FuncState >= 4) && ( FuncState <= 7) || ( FuncState == 9))
		WriteCode2( SAVETxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 1:
		case 4:
		case 8:
		case 9:
			WriteArrowsField( L_+D_+U_+R_+E_);

			VerticalRepeat = TRUE;
			break;

		case 6:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			VerticalRepeat = FALSE;
			break;

		case 3:
		case 7:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = FALSE;
			break;

		default:
			WriteArrowsField( L_+D_+U_+R_);

			VerticalRepeat = FALSE;
			break;
	}
}

/***************************************************************************/
/*	UpButType12																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980302	    										*/
/* Revised:		980302				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <SAVE ESC> state machine			*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType12() {

	switch ( FuncState) {
		case 1:
			FuncState = 4;
		case 4:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;  // inc xx
			break;

		case 2:
			FuncState = 5;
		case 5:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x04;  // inc yy
			break;

		case 8:
			FuncState = 9;
		case 9:
			FuncTriggers = ( FuncTriggers & 0xCF) | 0x10;  // inc zz
			break;

		case 3:
		case 7:
			FuncEscape = TRUE;
			break;
	}
}

/***************************************************************************/
/*	DownButType12																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980302	    										*/
/* Revised:		980302				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <XY EDIT SAVE ESC> state 		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType12() {

	switch ( FuncState) {
		case 1:
			FuncState = 4;
		case 4:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;  // dec xx
			break;

		case 2:
			FuncState = 5;
		case 5:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x08;  // dec yy
			break;

		case 8:
			FuncState = 9;
		case 9:
			FuncTriggers = ( FuncTriggers & 0xCF) | 0x20;  // dec zz
			break;
	}
}

/***************************************************************************/
/*	LeftButType12																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980302	    										*/
/* Revised:		980302				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <XY EDIT SAVE ESC> state 		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType12() {

	switch ( FuncState) {
		case 1:
			FuncState = 3;
			break;

		case 4:
			FuncState = 7;
			break;

		case 8:
			FuncState = 9;
		case 9:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x40;	// left rr
			break;

		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType12																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980302	    										*/
/* Revised:		980302				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <XY EDIT SAVE ESC> state 		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType12() {

	switch ( FuncState) {
		case 3:
			FuncState = 1;
			break;

		case 7:
			FuncState = 4;
			break;

		case 8:
			FuncState = 9;
		case 9:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x80;	// right rr
			break;

		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType12																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980302	    										*/
/* Revised:		980302				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <XY EDIT SAVE ESC> state	*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType12() {

	switch ( FuncState){
		case 1:
			FuncState = 8;
			break;

		case 4:
			FuncState = 9;
			break;

		case 6:
			FuncState = 1;
			FuncSave = TRUE;
			break;

		case 8:
			FuncState = 1;
			break;

		case 9:
			FuncState = 4;
			break;
	}
}

/***************************************************************************/
/*	WriteType13_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980302	    										*/
/* Revised:		980916				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <FORMAT ADJUST SAVE ESC>*/
/*					display handling															*/
/*	Remarks:		Part of <FORMAT ADJUST SAVE ESC> display handling				*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType13_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);

	if ((( FuncState >= 4) && ( FuncState <= 7)) || ( FuncState == 9))
		WriteCode2( SAVETxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 6:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
		case 2:
		case 5:
			WriteArrowsField( L_+R_+E_);

			VerticalRepeat = FALSE;
			break;

		case 3:
		case 7:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = FALSE;
			break;

		case 8:
		case 9:
			WriteArrowsField( L_+D_+U_+R_+E_);

			VerticalRepeat = TRUE;
			break;

		default:
			WriteArrowsField( L_+D_+U_+R_);

			VerticalRepeat = FALSE;
			break;
	}
}

/***************************************************************************/
/*	UpButType13																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980916	    										*/
/* Revised:		980916  				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <FORMAT ADJUST SAVE ESC> state	*/
/*					machine.																		*/
/*	Remarks:		Part of <FORMAT ADJUST SAVE ESC> display handling				*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType13() {

	switch ( FuncState) {
		case 1:
			FuncState = 4;
		case 4:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;  // inc xx
			break;

		case 8:
			FuncState = 9;
		case 9:
			FuncTriggers = ( FuncTriggers & 0xCF) | 0x10;  // inc zz
			break;

		case 3:
		case 7:
			FuncEscape = TRUE;
			break;
	}
}

/***************************************************************************/
/*	DownButType13																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980916	    										*/
/* Revised:		980916				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <FORMAT ADJUST SAVE ESC>		*/
/*					state machine.																*/
/*	Remarks:		Part of <FORMAT ADJUST SAVE ESC> display handling				*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType13() {

	switch ( FuncState) {
		case 1:
			FuncState = 4;
		case 4:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;  // dec xx
			break;

		case 8:
			FuncState = 9;
		case 9:
			FuncTriggers = ( FuncTriggers & 0xCF) | 0x20;  // dec zz
			break;
	}
}

/***************************************************************************/
/*	LeftButType13																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980916	    										*/
/* Revised:		980916				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <FORMAT ADJUST SAVE ESC>		*/
/*					state machine.																*/
/*	Remarks:		Part of <FORMAT ADJUST SAVE ESC> display handling				*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType13() {

	switch ( FuncState) {
		case 1:
			FuncState = 3;
			break;

		case 4:
			FuncState = 7;
			break;

		case 8:
			FuncState = 9;
		case 9:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x40;	// left rr
			break;

		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType13																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980916	    										*/
/* Revised:		980916				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <FORMAT ADJUST SAVE ESC>		*/
/*					state machine.																*/
/*	Remarks:		Part of <FORMAT ADJUST SAVE ESC> display handling				*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType13() {

	switch ( FuncState) {
		case 3:
			FuncState = 1;
			break;

		case 7:
			FuncState = 4;
			break;

		case 8:
			FuncState = 9;
		case 9:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x80;	// right rr
			break;

		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType13																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980916	    										*/
/* Revised:		980916				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <FORMAT ADJUST SAVE ESC>	*/
/*					state machine.																*/
/*	Remarks:		Part of <FORMAT ADJUST SAVE ESC> display handling				*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType13() {

	switch ( FuncState){
		case 2:
			FuncState = 8;
			break;

		case 5:
			FuncState = 9;
			break;

		case 6:
			FuncState = 1;
			FuncSave = TRUE;
			break;

		case 8:
			FuncState = 2;
			break;

		case 9:
			FuncState = 5;
			break;
	}
}

/***************************************************************************/
/*	WriteType14_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000508				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <XY SAVE ESC> display 	*/
/*					handling																		*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*	Returns:    --																				*/
/* Updates:		VerticalRepeat																*/
/***************************************************************************/
void WriteType14_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);

	if ( FuncState > 3)
		WriteCode2( SAVETxt);

	ClearToPosLN2( 36);
	WriteCode2( ESCTxt);

	ClearRestLN2();

	switch ( FuncState) {
		case 3:
		case 7:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			VerticalRepeat = FALSE;
			break;

		case 6:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);
			break;

		default:
			WriteArrowsField( L_+D_+U_+R_);

			VerticalRepeat = TRUE;
			break;
	}
}

/***************************************************************************/
/*	UpButType14																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000508				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <XY SAVE ESC> state machine		*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*	Returns:    --																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType14() {
	switch ( FuncState) {
		case 1:
			FuncState = 5;										// Break IS missing
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x01;
			break;

		case 2:
			FuncState = 4;										// Break IS missing
		case 4:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x04;
			break;

		case 3:
		case 7:
			FuncEscape = TRUE;
			return;
	}
}

/***************************************************************************/
/*	DownButType14																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000508				     													*/
/*																									*/
/*	Function:	Handle the DOWN-button in the <XY SAVE ESC> state machine	*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncTriggers												*/
/***************************************************************************/
void DownButType14() {
	switch ( FuncState) {
		case 1:
			FuncState = 5;
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x02;
			break;

		case 2:
			FuncState = 4;
		case 4:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x08;
			break;
	}
}

/***************************************************************************/
/*	LeftButType14																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000508				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <XY SAVE ESC> state machine	*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType14() {
	switch ( FuncState) {
		case 1:
		case 5:
			FuncState++;										// Break IS missing
		case 4:
			FuncState++;
			break;

		case 6:
			FuncState--;										// Break IS missing
		default:
			FuncState--;
			break;
	}
}

/***************************************************************************/
/*	RightButType14																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000508				     													*/
/*																									*/
/*	Function:	Handle the RIGHT-button in the <XY SAVE ESC> state machine	*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType14() {
	switch (FuncState) {
		case 3:
		case 7:
			FuncState--;										// Break IS missing
		case 5:
			FuncState--;
			break;

		case 4:
			FuncState++;										// Break IS missing
		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType14																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000508	    										*/
/* Revised:		000508				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <XY SAVE ESC> state machine*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*	Returns:    --																				*/
/* Updates:		FuncState and FuncSave													*/
/***************************************************************************/
void ExecButType14() {

	if ( FuncState == 6)
		FuncSave = FuncState = 1;
}
