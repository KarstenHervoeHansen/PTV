/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MENUHLP.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960628																		*/
/*	Rev. date:	971215, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
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
/*		void AnlBlkItemLeft();																*/
/*		void AnlBlkItemRight();																*/
/*		void SDIBlkItemLeft();																*/
/*		void SDIBlkItemRight();																*/
/*		void ItemLeft();																		*/
/*		void ItemRight();																		*/
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
/*																									*/
/*	Changes:																						*/
/* 971215: State tabel not correct in ExecButType8(): State 6 didn't result*/
/*				in state 1 when pressing EXECUTE											*/
/* 970923: Added Type 11 state machine													*/
/* 970725: In EscFunction(): FuncSave is now cleared while FuncEsc is set	*/
/* 970721: Error in WriteType8_SAVE_ESC: UP/DOWN was not displayed in 		*/
/*			   state 1 & 3																		*/
/* 970127: Error in LeftButType7 corrected											*/
/* 961229: Added StatusItemLeft & StatusItemRight									*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>

#include "52sync.h"
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
/* Revised:		970105				     													*/
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

	WriteTxtLN1( 36, "    ");

	switch ( No & 0x0A) {
		case D_:
			WriteCGChar( 1, 0x07);
			WriteCharLN1( 37, 1);			// DOWN
			break;

		case U_:
			WriteCGChar( 1, 0x06);
			WriteCharLN1( 37, 1);			// UP
			break;

		case D_+U_:
			WriteCGChar( 1, 0x02);
			WriteCharLN1( 37, 1);			// UP/DOWN
			break;
	}

	if ( No & L_) {
		WriteCGChar( 0, 0x01);				// LEFT
		WriteCharLN1( 36, 0);				// LEFT
	}

	if ( No & R_) {
		WriteCGChar( 2, 0x03);				// RIGHT
		WriteCharLN1( 38, 2);				// RIGHT
	}

	if ( No & P_) {
		WriteCGChar( 4, 0x05);				// Lock Icon
		WriteCharLN1( 39, 4);				// PADLOCK
	}

	if ( No & E_) {
		WriteCGChar( 3, 0x04);				// Execute Icon
		WriteCharLN1( 39, 3);				// EXECUTE
	}

	if ( No & MORE_)
		WriteCodeLN2( 37, ThreeDotsTxt);
}

/***************************************************************************/
/*	StatusItemLeft															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970225				     													*/
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
			case StatusAnalogSignalItem:
				if ( !AnalogSignalUnit.Present)
					Found = FALSE;
				break;

			case StatusSDISignalItem:
				if ( !SDISignalUnit.Present)
					Found = FALSE;
				break;

			case StatusAudioItem:
				if ( !AES_EBUUnit.Present)
					Found = FALSE;
				break;
		}
	} while ( !Found);
}

/***************************************************************************/
/*	StatusItemRight														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970225				     													*/
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
			case StatusAnalogSignalItem:
				if ( !AnalogSignalUnit.Present)
					Found = FALSE;
				break;

			case StatusSDISignalItem:
				if ( !SDISignalUnit.Present)
					Found = FALSE;
				break;

			case StatusAudioItem:
				if ( !AES_EBUUnit.Present)
					Found = FALSE;
				break;
		}
	} while ( !Found);
}

/***************************************************************************/
/*	BaseItemLeft															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970225				     													*/
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
			case SDIBlackItem:
				if ( !SDIBlkUnit[SB34Item].Present &&
					  !SDIBlkUnit[SB78Item].Present &&
					  !SDIBlkUnit[SB56Item].Present)
					Found = FALSE;
				break;

			case AnalogSignalItem:
				if ( !AnalogSignalUnit.Present)
					Found = FALSE;
				break;

			case SDISignalItem:
				if ( !SDISignalUnit.Present)
					Found = FALSE;
				break;

			case AES_EBUItem:
				if ( !AES_EBUUnit.Present)
					Found = FALSE;
				break;

			case TestItem:
				Found = ( UC) TestModeEnable;
				break;

			default:
				Found = TRUE;
				break;
		}
	} while ( !Found);
}

/***************************************************************************/
/*	BaseItemRight															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970225				     													*/
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
			case SDIBlackItem:
				if ( !SDIBlkUnit[SB34Item].Present &&
					  !SDIBlkUnit[SB78Item].Present &&
					  !SDIBlkUnit[SB56Item].Present)
					Found = FALSE;
				break;

			case AnalogSignalItem:
				if ( !AnalogSignalUnit.Present)
					Found = FALSE;
				break;

			case SDISignalItem:
				if ( !SDISignalUnit.Present)
					Found = FALSE;
				break;

			case AES_EBUItem:
				if ( !AES_EBUUnit.Present)
					Found = FALSE;
				break;

			case TestItem:
				Found = ( UC) TestModeEnable;
				break;
		}
	} while ( !Found);
}

/***************************************************************************/
/*	AnlBlkItemLeft															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Select previous item in the ANALOG BLACK MENU					*/
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
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Select next item in the ANALOG BLACK MENU							*/
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
/*	SDIBlkItemLeft															 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Select previous item in the SDI BLACK MENU						*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void SDIBlkItemLeft() {

	do {
		Ptr = Ptr->PrevItem;
	} while ( !SDIBlkUnit[Ptr->ItemNumber].Present);
}

/***************************************************************************/
/*	SDIBlkItemRight														 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Select next item in the SDI BLACK MENU								*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void SDIBlkItemRight() {

	do {
		Ptr = Ptr->NextItem;
	} while ( !SDIBlkUnit[Ptr->ItemNumber].Present);
}

/***************************************************************************/
/*	ItemLeft																	 	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Select previous item in the menu-/submenu-system				*/
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
/* Author:		Kim Engedahl, DEV, 940401	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Select next item in the menu-/submenu-system						*/
/*	Remarks:		This function does NOT apply to the BASE-menu					*/
/*	Returns:		--																				*/
/*	Updates:		Ptr																			*/
/***************************************************************************/
void ItemRight() {

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
/* Revised:		970908				     													*/
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
	ClearRestLN2();

	WriteCodeLN2( 36, ESCTxt);

	if ( FuncState > 2)
		WriteCodeLN2( 30, SAVETxt);

	switch ( FuncState) {
		case 1:
		case 3:
			WriteArrowsField( L_+D_+U_+R_);

			Vertical_Repeat = TRUE;
			break;

		case 4:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			Vertical_Repeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			Vertical_Repeat = FALSE;
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
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <XY SAVE ESC> display 	*/
/*					handling																		*/
/*	Remarks:		Part of <XY SAVE ESC> display handling								*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		Vertical_Repeat															*/
/***************************************************************************/
void WriteType2_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);
	ClearRestLN2();

	WriteCodeLN2( 36, ESCTxt);

	if ( FuncState > 3)
		WriteCodeLN2( 30, SAVETxt);

	switch ( FuncState) {
		case 3:
		case 7:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			Vertical_Repeat = FALSE;
			break;

		case 6:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);
			break;

		default:
			WriteArrowsField( L_+D_+U_+R_);

			Vertical_Repeat = TRUE;
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
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the OK and ESC text in the <OK ESC> display	handling */
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		--																				*/
/***************************************************************************/
void WriteType3_OK_ESC( char FuncState) {

	WriteAddressLN2( 32);
	ClearRestLN2();

	WriteCodeLN2( 31, OKTxt);
	WriteCodeLN2( 36, ESCTxt);

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
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the ESC text in the <INSTANT ADJUST LEVEL> display	*/
/*					handling																		*/
/*	Remarks:		Part of <INSTANT ADJUST LEVEL> display handling					*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		Vertical_Repeat															*/
/***************************************************************************/
void WriteType4_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 36);
	ClearRestLN2();

	WriteCodeLN2( 36, ESCTxt);

	switch ( FuncState) {
		case 1:
			WriteArrowsField( L_+D_+U_+R_);
			Vertical_Repeat = TRUE;
			break;

		case 2:
			WriteArrowsField( L_+U_+R_);
			WriteItemArrows( 36, Cstrlen( ESCTxt));

			Vertical_Repeat = FALSE;
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
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the ESC text in the <EXECUTE TEXT ESC> display			*/
/*					handling																		*/
/*	Remarks:		Part of <EXECUTE TEXT ESC> display handling						*/
/*					Reference: PM5689 NICAM Monitor, Menu structure document		*/
/*	Returns:    --																				*/
/* Updates:		--																				*/
/***************************************************************************/
void WriteType5_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 36);
	ClearRestLN2();

	WriteCodeLN2( 36, ESCTxt);

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
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <SAVE ESC> display 		*/
/*					handling																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		Vertical_Repeat															*/
/***************************************************************************/
void WriteType6_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);
	ClearRestLN2();

	WriteCodeLN2( 36, ESCTxt);

	if (( FuncState > 4) && ( FuncState < 10) || ( FuncState == 11))
		WriteCodeLN2( 30, SAVETxt);

	switch ( FuncState) {
		case 1:
		case 2:
		case 5:
		case 6:
			WriteArrowsField( L_+D_+U_+R_);

			Vertical_Repeat = TRUE;
			break;

		case 3:
		case 7:
		case 10:
		case 11:
			WriteArrowsField( L_+D_+U_+R_+E_);

			Vertical_Repeat = TRUE;
			break;

		case 8:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			Vertical_Repeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			Vertical_Repeat = FALSE;
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
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <USER TEXT SAVE ESC>		*/
/*					display handling															*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		Vertical_Repeat															*/
/***************************************************************************/
void WriteType7_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);
	ClearRestLN2();

	WriteCodeLN2( 36, ESCTxt);

	if (( FuncState >= 4) && ( FuncState <= 7) || ( FuncState == 9))
		WriteCodeLN2( 30, SAVETxt);

	switch ( FuncState) {
		case 1:
		case 4:
			WriteArrowsField( L_+R_+E_);

			Vertical_Repeat = FALSE;
			break;

		case 2:
		case 5:
			WriteArrowsField( L_+D_+U_+R_);

			Vertical_Repeat = TRUE;
			break;

		case 8:
		case 9:
			WriteArrowsField( L_+D_+U_+R_+E_);

			Vertical_Repeat = TRUE;
			break;

		case 6:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			Vertical_Repeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			Vertical_Repeat = FALSE;
			break;
	}
}

/***************************************************************************/
/*	UpButType7																	  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the UP-button in the <SAVE ESC> state machine			*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    -																				*/
/* Updates:		Ptr, FuncState and FuncTriggers										*/
/***************************************************************************/
void UpButType7() {

	switch ( FuncState) {
		case 2:
			FuncState = 5;										// Break IS missing
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x04;  // inc yy
			break;

		case 3:
		case 7:
			FuncEscape = TRUE;
			break;

		case 8:
			FuncState = 9;										// Break IS missing
		case 9:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x01;  // inc xx
			break;
	}
}

/***************************************************************************/
/*	DownButType7																  MENUHLP.C	*/
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
void DownButType7() {

	switch ( FuncState) {
		case 2:
			FuncState = 5;										// Break IS missing
		case 5:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x08;  // dec yy
			break;

		case 8:
			FuncState = 9;										// Break IS missing
		case 9:
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x02;  // dec xx
			break;
	}
}

/***************************************************************************/
/*	LeftButType7																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the LEFT-button in the <XY EDIT SAVE ESC> state machine*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void LeftButType7() {
	switch ( FuncState) {
		case 1:
			FuncState = 3;
			break;

		case 4:
			FuncState = 7;
			break;

		case 8:
		case 9:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x40;	// right rr
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
/*	Function:	Handle the RIGHT-button in the <XY EDIT SAVE ESC> state		*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    Nothing																		*/
/* Updates:		FuncState																	*/
/***************************************************************************/
void RightButType7() {
	switch ( FuncState) {
		case 3:
			FuncState = 1;
			break;

		case 7:
			FuncState = 4;
			break;

		case 8:
		case 9:
			FuncTriggers = ( FuncTriggers & 0x3F) | 0x80;	// left rr
			break;

		default:
			FuncState++;
			break;
	}
}

/***************************************************************************/
/*	ExecButType7																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
/*																									*/
/*	Function:	Handle the EXECUTE-button in the <XY EDIT SAVE ESC> state 	*/
/*					machine																		*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*	Returns:    --																				*/
/* Updates:		FuncSave																		*/
/***************************************************************************/
void ExecButType7() {
	switch ( FuncState){
		case 1:
			FuncState = 8;
			break;

		case 4:
			FuncState = 9;
			break;

		case 6:
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
/*	WriteType8_SAVE_ESC														  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970624	    										*/
/* Revised:		970721				     													*/
/*																									*/
/*	Function:	Handle the SAVE and ESC text in the <USER TEXT SAVE ESC>		*/
/*					display handling															*/
/*	Remarks:		Part of <XY EDIT SAVE ESC> display handling						*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		Vertical_Repeat															*/
/***************************************************************************/
void WriteType8_SAVE_ESC( char FuncState) {

	WriteAddressLN2( 30);
	ClearRestLN2();

	WriteCodeLN2( 36, ESCTxt);

	if (( FuncState >= 3) && ( FuncState <= 5) || ( FuncState == 7))
		WriteCodeLN2( 30, SAVETxt);

	switch ( FuncState) {
		case 1:
		case 3:
		case 6:
		case 7:
			WriteArrowsField( L_+D_+U_+R_+E_);

			Vertical_Repeat = TRUE;
			break;

		case 4:
			WriteItemArrows( 30, Cstrlen( SAVETxt));
			WriteArrowsField( L_+R_+E_);

			Vertical_Repeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			Vertical_Repeat = FALSE;
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
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x01;  // inc xx
			break;

		case 2:
		case 5:
			FuncEscape = TRUE;
			break;

		case 6:
			FuncState++;
		case 7:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x04;  // inc yy
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
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x02;  /* dec xx */
			break;

		case 6:
			FuncState++;
		case 7:
			FuncTriggers = ( FuncTriggers & 0xFC) | 0x08;  // dec yy
			break;
	}
}

/***************************************************************************/
/*	LeftButType8																  MENUHLP.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970623	    										*/
/* Revised:		970623				     													*/
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
/* Revised:		970624				     													*/
/*																									*/
/*	Function:	Handle the XX, YY, TIME, SAVE and ESC text in the				*/
/*					<XY TIME SAVE ESC> display handling									*/
/*	Remarks:		Part of <XY TIME SAVE ESC> display handling						*/
/*					Cursor postion on line 2 is updated									*/
/*	Returns:    --																				*/
/* Updates:		Vertical_Repeat															*/
/***************************************************************************/
void WriteType9_SAVE_ESC( char FuncState) {

	WriteCodeLN2( 21, SYSTxt);
	WriteCodeLN2( 26, TimeTxt);

	if ( FuncState > 4)
		WriteCodeLN2( 32, OKTxt);

	WriteCodeLN2( 36, ESCTxt);

	switch ( FuncState) {
		case 1:
		case 5:
			WriteArrowsField( L_+U_+R_+D_);

			Vertical_Repeat = FALSE;
			break;

		case 2:
		case 6:
			WriteItemArrows( 21, Cstrlen( SYSTxt));
			WriteArrowsField( L_+R_+D_);

			Vertical_Repeat = FALSE;
			break;

		case 3:
		case 7:
			WriteItemArrows( 26, Cstrlen( TimeTxt));
			WriteArrowsField( L_+R_+D_);

			Vertical_Repeat = FALSE;
			break;

		case 8:
			WriteItemArrows( 32, Cstrlen( OKTxt));
			WriteArrowsField( L_+R_+E_);

			Vertical_Repeat = FALSE;
			break;

		default:
			WriteItemArrows( 36, Cstrlen( ESCTxt));
			WriteArrowsField( L_+U_+R_);

			Vertical_Repeat = TRUE;
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
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x01; 	 // inc xx
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
			FuncTriggers = ( FuncTriggers & 0xF3) | 0x02;  // dec xx
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
/* Revised:		970908				     													*/
/*																									*/
/*	Function:	Handle the OK and ESC text in the <OK ESC> display	handling */
/*	Remarks:		Part of <OK ESC> display handling									*/
/*	Returns:    --																				*/
/* Updates:		--																				*/
/***************************************************************************/
void WriteType10_OK_ESC( char FuncState) {

	WriteAddressLN2( 32);
	ClearRestLN2();

	WriteCodeLN2( 31, OKTxt);
	WriteCodeLN2( 36, ESCTxt);

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
/* Revised:		970923				     													*/
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

