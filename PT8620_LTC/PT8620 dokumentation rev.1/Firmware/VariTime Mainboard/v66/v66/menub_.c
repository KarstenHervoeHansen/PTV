/***************************************************************************/
/*  Copyright   ProTeleVision Technologies A/S, BRONDBY 1998               */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:     MENUB.C                                                    */
/*  Author:     Kim Engedahl, DEV                                          */
/*  Org. date:  27.07.2005: JK                                             */
/*  Rev. date:                                                             */
/*  Status:     Version xx                                                 */
/*                                                                         */
/*  This was introduced because of too big size of module menu.c, which   */
/*  was increased due to new menu intems introduced (TLG and HDTPG).       */
/*  Linker gave an error due to too big module.                            */
/***************************************************************************/
#include <xa.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mas.h"

#include "menutree.h"
#include "menu.h"
#include "menuhlp.h"
#include "message.h"
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

UC RecByte=0;

static char tbuff[80];     /* temporary buffer used for printing messages */
/* for test purposes                           */
near UC DLTPGNoNdx;                         //     -"-     DLTPG sub menu item
near UC DLTPGsubmenuNdx[MaxDLTPGUnits];     //     -"-     DLTPG menu item

near UC DLTPGTextMenuNdx[MaxDLTPGUnits];    //     -"-     DLTPG text 2ndmenu item
near UC DLTPGAudioMenuNdx[MaxDLTPGUnits];   //     -"-     DLTPG audio 2ndmenu item

// Temporary variables PRIVATE NOT overlayable

extern char TextBuffer[32];      // Temp. text buffers // var 17 lang

extern UC ErrorCode;             // Temporary error code, 8 BIT
//  ErrorType + ErrorNo

extern near UC UcharVal;                // Temp. unsigned char value
extern near UC UcharVal1;        //  do.
extern near UC UcharVal2;        //  do.
extern near UC UcharVal3;        //  do.
extern near UC UcharVal4;		 //  do.


extern near UI UintVal;          // Temp. unsigned integer value

extern near int IntVal;          // Temp. integer value

extern near UL UlongVal;         // Temp. unsigned long value
extern near long longVal1;

extern UC* near UcharPtr;        // Temp. pointer to unsigned char

extern code char* near CodePtr;  // Temp. pointer to text arrays in CODE
extern char* near TxtPtr;        // Temp. pointer to text arrays in DATA


UC   FindNextDL(char, char *);
UL UC2bcd( UC ival);
UC bcd2UC( UC ival);

char TB[33];


char TimeBuffer[]= {0, 12, 0,0};
int l=0;



/***************************** PT8603 **************************************/
/***************************************************************************/
/*  PT8603SDITPGTextDown                                           MENU.C  */
/*                                                                         */
/* Author:   JK                                                            */
/* Revised:  20.11.2006                                                    */
/*                                                                         */
/* Purpose:  Go to the selected item in the SDI-TPGx/TEXT submenu in case  */
/*           of PT8603 (this is PT8603 specific function)                  */
/*                                                                         */
/* Remarks:  This function in enterd each time user has selected TEXT on   */
/*           SDITPG menu and board is PT8603.  See specs03.txt             */
/* Returns:  Nothing.                                                      */
/* Updates:  TBD                                                           */
/***************************************************************************/
void PT8603SDITPGTextDown()
{
	UC ndx = BaseNdx - SDITPG1Item, itemNdx = SDITPGTextNdx[ndx];

	WriteCodeLN1(0, SecondmnuHdrTxt);      // Write second menu header
	WriteCode1(SlashDotHdrTxt);            //  on line 1 = "../";

	WriteCode1(PT8603TPGtext_menu[itemNdx].ItemTxt);

	ClearRestLN1();

	switch (itemNdx)
	{
	case PT8603EditItem:                  // Prepare for text editing
		UcharVal1 = 0;
		UcharVal2 = 2;
		UcharVal  = UcharVal1;
		// Copy first string to temp. buffer
		strcpy(TextBuffer, SDITPGConfig[ndx].Text[UcharVal]);

		if (UintVal = strlen(TextBuffer))  // Position in Text string
			UintVal--;
		break;
	case PT8603OnOffItem:
		UcharVal = SDITPGConfig[ndx].TextEnable;     // Get text ON/OFF
		break;
	case PT8603MovementItem:
		UcharVal = SDITPGConfig[ndx].TextMovement;   // Get SDI text movement
		break;
	case PT8603TextPosItem:
		UcharVal  = SDITPGConfig[ndx].TextPos[0][0];   // Get SDI text X pos
		UcharVal1 = SDITPGConfig[ndx].TextPos[0][1];   // Get SDI text Y pos
		break;
	}
	PrevPtr      = Ptr;                          // Store old menu-pointer
	Ptr          = &PT8603TPG_submenu[itemNdx];  // Point to selected item
	FuncState    = 1;                            // Prepare for item statemachine
	FuncTriggers = 0;
	FuncSave     = FuncEscape = FALSE;
	SelectModeOn = TRUE;
}

/***************************************************************************/
/*  PT8603TextEditSelect                                           MENU.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980224                                    */
/* Revised:   990305                                                       */
/*                                                                         */
/*  Function: Edit a text in a SDI-TPG.                                    */
/*  Remarks:  SDI-TPGx/TEXT MENU function.                                 */
/*          Cursor position on line 2 is updated.                          */
/*          UcharVal     LineNumber 0-2                                    */
/*          UcharVal1    Min. LineNumber                                   */
/*          UcharVal2    Max. LineNumber                                   */
/*          UcharVal3    Text Enable ON/OFF (Not used here)                */
/*          UintVal      Position in text buffer                           */
/*  Returns:    --                                                         */
/*  Updates:    TBD                                                        */
/*  This function is taken from PT5230 Master and modified                 */
/***************************************************************************/
void PT8603TextEditSelect()
{
	UC tmp;
	UC ndx = BaseNdx - SDITPG1Item;

	if (FuncEscape)
	{
		CharBlinkLN2(0, OFF);           // Switch off possible blinking

		SelectModeOn = FALSE;           // Clear select-mode

		Ptr = PrevPtr;                  // Set pointer to previous menu
		Ptr->DispFunct();               // Display previous menu
		return;
	}

	if (FuncSave)
	{
		// If textinfo has changed..
		FuncSave = FALSE;
		ActivePresetNo = 0;       // Clear possible preset
		// Save the new text
		strcpy(SDITPGConfig[ndx].Text[UcharVal], TextBuffer);

		// Save text enable information
		//SDIConfig.TextOn &= ~(1 << UcharVal);
		//SDIConfig.TextOn |= (UcharVal3 << UcharVal);

		tmp = (UcharVal % 3) + 1;          // Calculate line number

		// Transmit text line enable
		//ErrorCode = TrxSDITPGTextLineEnable(ndx, tmp, UcharVal3);

		// Transmit text line string
		ErrorCode = TrxSDITPGText(ndx, tmp, TextBuffer);

		if (ErrorCode)
		{
			GotoErrorStatus(ErrorCode);
			return;
		}
	}
	// Select line to edit
	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal-- == UcharVal1 )
				UcharVal = UcharVal2;
		}                                // ..button pressed is DOWN
		else
		{
			if (UcharVal++ == UcharVal2)
				UcharVal = UcharVal1;
		}                          // Copy selected text to temp.
		//  text buffer
		strcpy(TextBuffer, SDITPGConfig[ndx].Text[UcharVal]);

		if (UintVal = strlen(TextBuffer))    // Find position in string
			UintVal--;
		FuncState = 1;
	}
	// Select position in TextBuffer
	if (FuncTriggers & 0xC0)
	{
		// If LEFT/RIGHT button pressed..
		if (FuncTriggers & 0x40)
		{
			// If button pressed is LEFT..
			TextBuffer[UintVal] = 0;// Delete character

			if (UintVal)            // Decrement pointer to last
				UintVal--;            //  character
		}
		else
		{
			// ..button pressed is RIGHT
			tmp = 31;               // Max. 32 characters in PT8602/3; 15 orig

			if ((UintVal < tmp) && (UintVal < strlen(TextBuffer)))
				UintVal++;
		}
	}
	// Select character in TextBuffer
	if (FuncTriggers & 0x30)
	{
		// If UP/DOWN button pressed..
		if (TextBuffer[UintVal] == 0) // if we are at the end of string....
		{
			// If text insert..
			TextBuffer[UintVal]   = TextBuffer[UintVal-1]; //  set new character = previous
			TextBuffer[UintVal+1] = 0;
		}

		if (TextBuffer[UintVal] == 0)
			TextBuffer[UintVal] = '-';

		/* FindNextASCII(UC Direction,   UC LimitedType, char ASCIIChar) */
		TextBuffer[UintVal] = FindNextASCII((FuncTriggers & 0x10), TRUE, TextBuffer[UintVal]);
	}

	CodePtr = PT8603TextEditArrTxt[UcharVal % 3]; /* L1, L2 or L3 */

	WriteCodeLN2(0, CodePtr);        // Write PT8603 text on line 2
	ClearRestLN2();

	WriteTxtLN2(2, ": ");            // 5 orig

	/* As we now have text with max length of 32 chars, we must implement horizontal */
	/* scroll of text displayed on the second line, otherwise there is no place for  */
	/* all <SAVE><ESC>.  Displayed text will have max 24 chars visible, if user      */
	/* enters more chars, text will be scrolled left, so that last visible char is   */
	/* at position (24+4).  Variable tmp is used to hold character number which is   */
	/* visible on the left side of displayed text.                                   */

	if (strlen(TextBuffer) > 24)    /* if text is longer than 24 chars....           */
		tmp = strlen(TextBuffer) - 24;/* displayed char on left side is tmp            */
	else
		tmp = 0;                      /* otherwise, 0-th char is displayed on left side*/


	/**********  Display text  **************************************/
	WriteTxtLN2(4, &TextBuffer[tmp]);
	/****************************************************************/

	WriteTypeDK_SAVE_ESC(FuncState);         // Write the SAVE ESC text

	switch (FuncState)
	{
	case 1:
	case 3:
		WriteItemArrows(4, strlen(&TextBuffer[tmp])); // 7 orig
		CharBlinkLN2(0, OFF);          // Switch OFF blinking
		break;

	case 7:
	case 8:
		WriteItemBrackets(4, strlen(&TextBuffer[tmp])); // 7 orig
		CharBlinkLN2(4 + UintVal - tmp, ON); // Switch ON blinking at; 7 orig
		break;                               // edited character
	}
	FuncTriggers = 0;                  // Reset the button-selection
}


/***************************************************************************/
/* PT8603TextOnOffSelect                                           MENU.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   05.10.2004                                                   */
/*                                                                         */
/*  Function:                                                              */
/*                                                                         */
/***************************************************************************/
void PT8603TextOnOffSelect()
{
	UC ndx = BaseNdx - SDITPG1Item;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			ActivePresetNo = 0;

			SDITPGConfig[ndx].TextEnable = UcharVal;

			if (ErrorCode = TrxSDITPGTextLineEnable(ndx, 1, UcharVal))
			{
				GotoErrorStatus(ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;             // Clear select-mode

		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (UcharVal == ON)        //  toggle between ON & OFF
			UcharVal = OFF;
		else
			UcharVal = ON;

		if (UcharVal == SDITPGConfig[ndx].TextEnable)  // If actual TextOn..
			FuncState = 1;                   //  remove SAVE option
	}

	WriteCodeLN2(0, SelectHdrTxt);

	CodePtr = OffOnArrTxt[UcharVal];    // Get text for on/off selection...
	WriteCode2(CodePtr);

	ClearRestLN2();

	WriteType1_SAVE_ESC(FuncState);

	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(8, Cstrlen(CodePtr));  // Mark selected item

	FuncTriggers = 0;                  // Reset the button-selection
}

/***************************************************************************/
/* PT8603TextMovementSelect                                        MENU.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   05.10.2004                                                   */
/*                                                                         */
/*  Function:                                                              */
/*                                                                         */
/***************************************************************************/
void PT8603TextMovementSelect()
{
	UC ndx = BaseNdx - SDITPG1Item;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			ActivePresetNo = 0;

			SDITPGConfig[ndx].TextMovement = UcharVal;

			ErrorCode = SndInt(SDITPGUnit[ndx].Address, "SH", SDITPGConfig[ndx].TextMovement);  // new for 8603: text move

			if (ErrorCode)
			{
				GotoErrorStatus(ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;             // Clear select-mode

		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (UcharVal == ON)        //  toggle between ON & OFF
			UcharVal = OFF;
		else
			UcharVal = ON;

		if (UcharVal == SDITPGConfig[ndx].TextMovement)  // If actual mov..
			FuncState = 1;                   //  remove SAVE option
	}

	WriteCodeLN2(0, SelectHdrTxt);

	CodePtr = OffOnArrTxt[UcharVal];    // Get text for selected system
	WriteCode2(CodePtr);

	ClearRestLN2();

	WriteType1_SAVE_ESC(FuncState);

	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(8, Cstrlen(CodePtr));  // Mark selected item

	FuncTriggers = 0;                  // Reset the button-selection
}


/***************************************************************************/
/*  PT8603TextPosSelect                                            MENU.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 000508                                    */
/* Revised:    000608                                                      */
/*                                                                         */
/*  Function:  Select the text positioning in a SDI-TPG.                   */
/*  Remarks:    SDI-TPGx/TEXT MENU function.                               */
/*          Cursor position on line 2 is updated.                          */
/*  Returns:    --                                                         */
/*  Updates:    TBD                                                        */
/*  UcharVal  = SDITPGConfig[ndx].TextPos[0][0];   // Get SDI text X pos   */
/*  UcharVal1 = SDITPGConfig[ndx].TextPos[0][1];   // Get SDI text Y pos   */
/*                                                                         */
/***************************************************************************/
void PT8603TextPosSelect()
{
	UC patternType;
	UC ndx = BaseNdx - SDITPG1Item;
	UC Xpos, Ypos, tmp;


	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If text position has changed..
			ActivePresetNo = 0;
			// Save new text positioning
			SDITPGConfig[ndx].TextPos[0][0] = UcharVal;
			SDITPGConfig[ndx].TextPos[0][1] = UcharVal1;
		}
		else
		{
			// ..restore text style
			if ((UcharVal != SDITPGConfig[ndx].TextPos[0][0]) ||
			        (UcharVal1 != SDITPGConfig[ndx].TextPos[0][1]))
			{
				Xpos = SDITPGConfig[ndx].TextPos[0][0];
				Ypos = SDITPGConfig[ndx].TextPos[0][1];

				if (SDITPGConfig[ndx].System == SDI625)
					tmp = 22*Ypos+8;
				else
					tmp = 18*Ypos+6;

				ErrorCode = SndInt3(SDITPGUnit[ndx].Address, "SJ", (UI)1, (UI)(4 + Xpos * 2), (UI)tmp);

				if (ErrorCode)
				{
					GotoErrorStatus(ErrorCode);
					return;
				}
			}
		}
		SelectModeOn = FALSE;            // Clear select-mode

		Ptr = PrevPtr;                  // Set pointer to previous menu
		Ptr->DispFunct();                // Display previous menu
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If button pressed is UP/DOWN..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal++ == PT8603XPosMax)
				UcharVal = PT8603XPosMin;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal-- == PT8603XPosMin)
				UcharVal = PT8603XPosMax;
		}
		// Save new text positioning
		if ((UcharVal == SDITPGConfig[ndx].TextPos[0][0]) &&
		        (UcharVal1 == SDITPGConfig[ndx].TextPos[0][0]))
			FuncState = 1; // if unchanged, remove SAVE option...

		Xpos = UcharVal;
		Ypos = UcharVal1;

		if (SDITPGConfig[ndx].System == SDI625)
			tmp = 22*Ypos+8;
		else
			tmp = 18*Ypos+6;

		ErrorCode = SndInt3(SDITPGUnit[ndx].Address, "SJ", (UI)1, (UI)(4 + Xpos * 2), (UI)tmp);

		if (ErrorCode)
		{
			GotoErrorStatus(ErrorCode);
			return;
		}
	}

	if (FuncTriggers & 0x0C)
	{
		// If button UP/DOWN pressed..
		if (FuncTriggers & 0x04)
		{
			// If button pressed is UP..
			if (UcharVal1++ == PT8603YPosMax)
				UcharVal1 = PT8603YPosMin;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal1-- == PT8603YPosMin)
				UcharVal1 = PT8603YPosMax;
		}
		// Save new text positioning
		if ((UcharVal == SDITPGConfig[ndx].TextPos[0][0]) &&
		        (UcharVal1 == SDITPGConfig[ndx].TextPos[0][1]))
			FuncState = 2;

		Xpos = UcharVal;
		Ypos = UcharVal1;

		if (SDITPGConfig[ndx].System == SDI625)
			tmp = 22*Ypos+8;
		else
			tmp = 18*Ypos+6;

		ErrorCode = SndInt3(SDITPGUnit[ndx].Address, "SJ", (UI)1, (UI)(4 + Xpos * 2), (UI)tmp);

		if (ErrorCode)
		{
			GotoErrorStatus(ErrorCode);
			return;
		}
	}

	WriteTxtLN2(0, "X: ");              // Write line 2 header
	sprintf(TextBuffer, "%+3d", UcharVal);  // Convert coordinate to ascii
	WriteTxt2(TextBuffer);              // Write coordinate

	ClearToPosLN2(10);

	WriteTxt2("Y: ");                       // Convert coordinate to ascii
	sprintf(TextBuffer, "%+3d", UcharVal1);
	WriteTxt2(TextBuffer);              // Write coordinate

	ClearRestLN2();

	WriteType2_SAVE_ESC(FuncState);

	switch (FuncState)
	{
	case 1:
	case 5:
		WriteItemArrows(3, 3);        // Mark selected item
		break;

	case 2:
	case 4:                        // Mark selected item
		WriteItemArrows(13, 3);
		break;
	}
	FuncTriggers = 0;               // Reset button-selection
}
/************************** PT8603 *****************************/


/***************************************************************************/
/*  AnlBlkMenuShow                                    MENU.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980119                          */
/* Revised:    980508                                       */
/*                                                  */
/*  Function:  Display the items in the BLACK-BURST menu.            */
/*  Remarks:    Cursor position on line 1 & 2 are updated.            */
/*  Returns:    --                                        */
/*  Updates:    AnlBlkNdx                                  */
/***************************************************************************/
void AnlBlkMenuShow()
{
	register int i, j, last, NoOfBBs;

	WriteCodeLN1(0, MenuHdrTxt);
	WriteCode1(base_menu[BaseNdx].ItemTxt);
	WriteCode1(CoConfigureTxt);           // Write menu headline
	ClearRestLN1();                  // Clear rest of line 1

	AnlBlkNdx = Ptr->ItemNumber;          // Update BB unit index, ie. no.

	WriteCodeLN2(0, SubmnuHdrTxt);        // Write header for sub-menu line
	// Calculate no. of BB units
	for (NoOfBBs=0, i=BB1Item; i < BB8Item+1; i++)
		if (AnlBlkUnit[i].Present)
			NoOfBBs++;

	if (NoOfBBs > 6)
	{
		// If 6 or more BB units..
		i = last = BB7Item;                //  two screens are required
		(AnlBlkNdx > BB6Item) ? (last = BB8Item+1) : (i = BB1Item);
	}
	else
	{
		// ..otherwise only 1 screen
		i = 0;
		last = BB8Item+1;
	}

	for (j=0;  i < last; i++)
	{
		// Display active screen
		if (AnlBlkUnit[i].Present)
		{
			if ((i == BB2Item) && (AnlBlkUnit[BB2Item].HWType == PT8604))
			{
				WriteCode2(BBMulTxt);          // Write item text
				WriteTxt2("  ");               // Jump to next position

				if (i < AnlBlkNdx)            // Calculate item marker
					j += (Cstrlen(BBMulTxt) + 2);
			}
			else
			{
				WriteCode2(anlblk_menu[i].ItemTxt);// Write item text
				WriteTxt2("  ");                   // Jump to next position

				if (i < AnlBlkNdx)               // Calculate item marker
					j += (Cstrlen(anlblk_menu[i].ItemTxt) + 2);
			}
		}
	}
	ClearRestLN2();                  // Clear reset of line 2

	i = Ptr->ItemConfig;                // Get line1&2 icons/dots info

	if (NoOfBBs > 6 )                // If 6 or more BB units..
		i += MORE_;                    //  add 'more' dots to line 2

	WriteArrowsField(i);              // Write line 1&2 icons/dots

	// Get length of text to mark
	if ((AnlBlkNdx == BB2Item) && (AnlBlkUnit[BB2Item].HWType == PT8604))
		i = Cstrlen(BBMulTxt);
	else
		i = Cstrlen(Ptr->ItemTxt);

	WriteItemArrows(j+8, (UC) i);        // Mark selected item
}

/***************************************************************************/
/* DLTPGMenuShow                                                    menu.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 15.02.2005                             */
/* Revised:  21.02.2005                                                    */
/*           26.07.2005                                                    */
/*                                                                         */
/* Function: To display DLTPG menu, according to number of generators      */
/*           present in PT5230, which may be up to 6.                      */
/* Params:   None                                                          */
/* Remarks:  This function displays menu in form:                          */
/*           MENU   : DLTPG, configure                                     */
/*           SUBMENU: <DL1>  <DL2>  <DL3>  <DL4>  <DL5>  <DL6>             */
/*           At present there may be up to 3 DLTPG boards mounted,         */
/*           each containing 4 independent generators                      */
/*           New specification:                                            */
/*           Board mounted at bbu1_addr has generators 1 and 2.            */
/*           Board mounted at bbu3_addr has generators 3 and 4.            */
/*           Board mounted at bbu2_addr has generators 5 and 6      .      */
/*           Upon entry we have:                                           */
/*                              Ptr = &DLTPG_menu[DLTPGNoNdx];             */
/*           While this menu is displayed:                                 */
/*           When we move LEF : Ptr = Ptr->PrevItem;                       */
/*           When we move RGHT: Ptr = Ptr->NextItem;                       */
/*           This function updates:   DLTPGNoNdx = Ptr->ItemNumber;        */
/*           When we move DOWN, DLTPGNoNdx contains number of selecerd     */
/*           generator.                                                    */
/*                                                                         */
/* Returns:  Nothing                                                       */
/***************************************************************************/
void DLTPGMenuShow()
{
	register int i, j, last, first, NoOfDLTPGs;
	UC first_board;
	UC second_board = 0;

	WriteCodeLN1(0, MenuHdrTxt);
	WriteCode1(base_menu[BaseNdx].ItemTxt);
	WriteCode1(CoConfigureTxt);           // Write menu headline
	ClearRestLN1();

	DLTPGNoNdx = Ptr->ItemNumber;          // Update DLTPG unit index, ie. no.

	WriteCodeLN2(0, SubmnuHdrTxt);

	for (NoOfDLTPGs = 0, i = DLTPG1Item; i < DLTPG6Item + 1; i++)
		if (DLTPGUnit[i].Present)
			NoOfDLTPGs++;              // Calculate no. of DLTPG units


	if (DLTPGUnit[0].Present)
	{
		first_board = 1;
		if (DLTPGUnit[2].Present)
			second_board = 2;
		else if (DLTPGUnit[2].Present)
			second_board = 3;
	}
	else if (DLTPGUnit[2].Present)
	{
		first_board = 2;
		if (DLTPGUnit[4].Present)
			second_board = 3;
	}
	else if (DLTPGUnit[4].Present)
		first_board = 3;

	switch (NoOfDLTPGs)
	{
	case 2:
		switch (first_board)
		{
		case 1:
			first = DLTPG1Item;
			last  = DLTPG2Item + 1;
			break;
		case 2:
			first = DLTPG3Item;
			last  = DLTPG4Item + 1;
			break;
		case 3:
			first = DLTPG5Item;
			last  = DLTPG6Item + 1;
			break;
		}
		break;
	case 4:
		switch (first_board)
		{
		case 1:                  // second board can be 2 or 3....
			if (second_board == 2)
			{
				first = DLTPG1Item;
				last  = DLTPG4Item + 1;
			}
			else
			{
				first = DLTPG1Item;
				last  = DLTPG6Item + 1;
			}
			break;
		case 2:                  // second board can only be 3....
			first = DLTPG3Item;
			last  = DLTPG6Item + 1;
			break;
		}
		break;
	case 6:
		first = DLTPG1Item;
		last  = DLTPG6Item + 1;
		break;
	default:
		break;
	}
	j = 0;

	for (i = first;  i < last; i++)
	{
		// Display active screen
		if (DLTPGUnit[i].Present)
		{
			WriteCode2(DLTPG_menu[i].ItemTxt);// Write item text
			WriteTxt2("  ");                  // Jump to next position

			if (i < DLTPGNoNdx)               // Calculate item marker
				j += (Cstrlen(DLTPG_menu[i].ItemTxt) + 2);
		}
	}
	ClearRestLN2();               // Clear reset of line 2

	i = Ptr->ItemConfig;          // Get line1&2 icons/dots info

	//if (NoOfDLTPGs > 4 )          // If 4 or more DLTPG units..
	//i += MORE_;                 //  add 'more' dots to line 2

	WriteArrowsField(i);             // Write line 1&2 icons/dots
	i = Cstrlen(Ptr->ItemTxt);       // Get length of text to mark
	WriteItemArrows(j + 8, (UC) i);  // Mark selected item
}

/***************************************************************************/
/* DLTPGDown                                                        menu.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, DEV, 15.02.2005                             */
/* Revised:  21.02.2005                                                    */
/*                                                                         */
/* Function: To set Ptr to apropriate member of DLTPG_submenu[] using      */
/*           DLTPGsubmenuNdx[DLTPGNoNdx]                                   */
/*                                                                         */
/* Params:   None                                                          */
/* Remarks:  This function is entered when user presses DOWN key in while  */
/*           in DLTPGMenuShow(), thus selecting desired generator          */
/*                                                                         */
/* Returns:  Nothing                                                       */
/***************************************************************************/
void DLTPGDown()
{
	Ptr = &DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];
}

void UpToDLTPG()
{
	Ptr = &DLTPG_menu[DLTPGNoNdx];    // Restore selected DLTPG item i.e.
}                                 // generator number.

void UpToDLTPGsub()
{
	Ptr = &DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];    // Restore selected DLTPG item i.e.
}

/***************************************************************************/
/* void DLTPGSubMenuShow(void)                                     MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 21.02.2005                                                     */
/*                                                                         */
/* Function: To display DLTPG submenu, which is as follows:                */
/*           2NDMNU: <PATT>  SYSTEM  AUDIO  TIMING                         */
/* Remarks:                                                                */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Updates:     TBD                                                        */
/***************************************************************************/
void DLTPGSubMenuShow()
{
	register int i, j;

	WriteCodeLN1(0, SubmnuHdrTxt);
	WriteCode1(base_menu[BaseNdx].ItemTxt);
	WriteChar1('/');

	CodePtr = DLTPG_menu[DLTPGNoNdx].ItemTxt;
	WriteCode1(CodePtr);
	WriteCode1(CoSelectTxt);
	ClearRestLN1();
	WriteCodeLN2(0, SecondmnuHdrTxt);

	DLTPGsubmenuNdx[DLTPGNoNdx] = Ptr->ItemNumber;

	i = j = DLTPGAudioItem;              // Find window

	if (DLTPGsubmenuNdx[DLTPGNoNdx] < DLTPGAudioItem)   // If item < AUDIO..
		i = DLTPGPatternItem;              //  Pattern, Text & System
	else
		j = DLTPGTextItem+1;               //  EMB.Audio, Timing & Text

	for (; i < j; i++)
	{
		ClearToPosLN2(DLTPG_submenu[i].ItemPos); // Write item position
		WriteCode2(DLTPG_submenu[i].ItemTxt);    // Write item text
	}

	ClearRestLN2();
	i = Ptr->ItemConfig;                  // Get line 1&2 icons/dots
	if (Flags.NormalLockOn)               // If normal LOCK is enabled..
		if (DLTPGsubmenuNdx[DLTPGNoNdx] != DLTPGPatternItem)  // If not pattern menu..
			i = (i | P_) & ~D_;             //  add padlock, remove down  arrow.
	WriteArrowsField(i);                // Write line 1&2 icons/dots
	WriteItemArrows(Ptr->ItemPos, Cstrlen(Ptr->ItemTxt));  // Mark selected item
}

/*
enum DLTPGItems
  {
  DLTPGPatternItem = 0, DLTPGSystemItem, DLTPGTimingItem \
  DLTPGAudioItem, DLTPGTextItem
  };
*/
/***************************************************************************/
/* void DLTPGSubDown(void)                                         MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 21.02.2005                                                     */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in DLTPG submenu, which is as follows:         */
/*           2NDMNU: <PATT> SYSTEM  TIMING TEXT   AUDIO                    */
/*                                                                         */
/* Remarks: For current generator (DLTPGNoNdx), DLTPGsubmenuNdx[DLTPGNoNdx]*/
/*          holds the  highlighted item nr, i.e. PATT, TIMING, etc         */
/*          and preparations are made accordingly,                         */
/*          before DLTPG_sub_submenu  is entered.                          */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGSubDown()
{
	WriteCodeLN1(0, SecondmnuHdrTxt);
	WriteCode1(SlashDotHdrTxt);
	WriteCode1(DLTPG_menu[DLTPGNoNdx].ItemTxt);
	WriteChar1('/');
	WriteCode1(DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]].ItemTxt);

	PrevPtr = Ptr;
	//Ptr     = &DLTPG_sub_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];

	switch (DLTPGsubmenuNdx[DLTPGNoNdx])
	{
	case DLTPGPatternItem:
		WriteCode1(CoSelectTxt);
		UcharVal  = DLTPGConfig[DLTPGNoNdx].Pattern;
		Ptr       = &DLTPG_sub_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];// Point to selected item
		FuncState = 1;
		break;
	case DLTPGTimingItem:
		WriteCode1(CoEditDelayTxt);
		UcharVal    = DLTPGConfig[DLTPGNoNdx].System;
		tlg_samples = DLTPGConfig[DLTPGNoNdx].Delay;
		stepNdx     = stepNdxTLGMin;
		stepVal     = stepsizeTLGTable[dltpg_fmt_table[UcharVal].sys_clk][stepNdx];
		Ptr         = &DLTPG_sub_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];
		FuncState   = 2;
		break;
	case DLTPGTextItem:
		/* Point to TextMenu: Edit, Style, Pos, Clock for current DL generator number */
		Ptr       = &DLTPGtext_menu[DLTPGTextMenuNdx[DLTPGNoNdx]];
		FuncState = 1;
		break;
	case DLTPGSystemItem:
		WriteCode1(CoSelectTxt);
		UcharVal  = DLTPGConfig[DLTPGNoNdx].System;
		Ptr       = &DLTPG_sub_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];// Point to selected item
		FuncState = 1;
		break;
	case DLTPGAudioItem:
		/* Point to AudioMenu: Signal  Level  Group for current DL generator number */
		Ptr       = &DLTPGaudio_menu[DLTPGAudioMenuNdx[DLTPGNoNdx]];
		FuncState = 1;
		break;
	}
	ClearRestLN1();
	FuncTriggers = 0;
	FuncSave     = FuncEscape = FALSE;
	SelectModeOn = TRUE;
}

/***************************************************************************/
/* void DLTPGPatternSelect()                                       MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 03.03.2005                                                     */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in DLTPG submenu, which is as follows:         */
/*           2NDMNU: <PATT> SYSTEM TIMING                                  */
/*           having PATT selected.                                         */
/*                                                                         */
/*           This function writes Line 2 menu:                             */
/*           <PAT0>                      ESC         OR                    */
/*           <PAT1>      LEVELS          ESC                               */
/*           Only PATs 0 - 4 have attributes which may be modified.        */
/* Remarks:  On entry:                                                     */
/*           UcharVal = DLTPGConfig[DLTPGNoNdx].Pattern;                   */
/*                Ptr = &DLTPG_sub_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];   */
/* Returns: Nothing                                                        */
/* Notes:                                                                  */
/*
    first 5 have attributes, others do not.
enum HDPatterns
  {
  HDColorbar = 0, HDCombination, HDWindow, HDWhite,  HDCrosshatch,
  HDCheckField,   HDPluge,       HDLumRamp, HDClapBoard,
  HDBlack
  };
 */
/***************************************************************************/
void DLTPGPatternSelect()
{
	UC board;

	if (FuncState == 13) // FuncState = 13 means that MODIFY item selected..
	{
		// See comments below ....
		WriteCodeLN1(0, SecondmnuHdrTxt);   // Write menu header on line 1
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LevelsTxt);
		WriteTxt1(", ");
		CodePtr = HDTPGPattArrTxt[UcharVal];  // Get text for selected pattern
		WriteCode1(CodePtr);                  // Write pattern text on line 1
		ClearRestLN1();                       // Clear rest of line 1
		// Get pattern attribute to UcharVal2    UcharVal = Pattern
		UcharVal2 = DLTPGConfig[DLTPGNoNdx].Attrib[UcharVal];

		FuncState    = 1;
		FuncTriggers = 0;
		FuncSave     = FuncEscape = FALSE;

		Ptr = &DLTPGpattern_modify_menu;   // Set pointer to MODIFY menu
		Ptr->DispFunct();                  // Display HDTPGPatternModify() menu
		return;                            // execute function HDTPGPatternModify()
	}

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			ActivePresetNo = 0;
			DLTPGConfig[DLTPGNoNdx].Pattern = UcharVal;
		}
		else   // is ESC ..restore HDTPG pattern
		{
			/* If pattern has attributes, they are taken from HDTPGConfig and */
			/* transmitted, otherwise attribute = 0 is transmitted.           */
			ErrorCode = TrxDLTPGPattern(DLTPGNoNdx, DLTPGConfig[DLTPGNoNdx].Pattern);
			if (ErrorCode)
			{
				board = FindDLTPGBoard(DLTPGNoNdx);
				GotoErrorStatus(256*board + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;
		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}
	/*************** PATTERN UP/DOWN ********************/
	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal++ == HDBlack)
				UcharVal = HDColorbar;
		}
		else
		{
			// ..else previous pattern
			if (UcharVal-- == HDColorbar)
				UcharVal = HDBlack;
		}

		if (UcharVal == DLTPGConfig[DLTPGNoNdx].Pattern)
		{
			// If current pattern..
			if (FuncState > 5)                  //  remove SAVE option
				FuncState = 6;
			else
				FuncState = 1;
		}
		/************************* UP/DOWN ***************************/

		/* If pattern has attributes, they are taken from HDTPGConfig and */
		/* transmitted, otherwise attribute = 0 is transmitted.           */
		ErrorCode = TrxDLTPGPattern(DLTPGNoNdx, UcharVal);
		if (ErrorCode)
		{
			board = FindDLTPGBoard(DLTPGNoNdx);

			GotoErrorStatus(256*board + ErrorCode);
			return;
		}
	} // if PATTERN UP/DOWN

	/* If HAS ATTRIBS (= "COLOURBAR", OR COMBINATION...etc    */
	/* Code below decides which Patterns will have attributes */
	/* i.e. which will have <MODIFY> option.                  */
	if ((UcharVal >= HDColorbar) && (UcharVal < HDCrosshatch))
	{
		if (FuncState == 1)
			FuncState = 6;
		if (FuncState == 3)
			FuncState = 9;
	}
	else
	{
		if (FuncState == 6)
			FuncState = 1;
		if (FuncState == 9)
			FuncState = 3;
	}
	CodePtr = HDTPGPattArrTxt[UcharVal];    // Get text for selected pattern
	WriteCharLN2(0, ' '  );                 // No header for line 2
	WriteCode2(CodePtr);                    // Write the selected pattern
	ClearRestLN2();                         // Clear rest of line 2
	WriteType7b_SAVE_ESC(FuncState);        // Write the SAVE ESC texct
	switch (FuncState )
	{
	case 1:
	case 3:
	case 6:
	case 9:                      // Mark selected item
		WriteItemArrows(1, Cstrlen(CodePtr));
		break;
	}
	FuncTriggers = 0;
}

/***************************************************************************/
/* void DLTPGPatternModify()                                       MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 03.03.2005                                                     */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in pattern select submenu, which is as follows:*/
/*            WINDOW       <LEVELS>         ESC                            */
/*           having MODIFY selected and then on subsequent UP/DOWN key     */
/*           presses.                                                      */
/*                                                                         */
/* Remarks:                                                                */
/*           When this function is invoked, the first line on display has  */
/*           already been written.                                         */
/*                                                                         */
/*           UcharVal2 holds current attributes                            */
/*           UcharVal  halds current pattern                               */
/*           When SAVE, this function saves Pattern in DLTPGConfig.        */
/* Returns:  Nothing                                                       */
/* Notes:    When this function saves or escapes, we must return to        */
/*           displaying patterns,(DLTPGPatternSelect),                     */
/*           and not to DLTPGSubMenuShow()                                 */
/***************************************************************************/
void DLTPGPatternModify()
{
	UC board;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If interface has changed..
			ActivePresetNo = 0;
			DLTPGConfig[DLTPGNoNdx].Attrib[UcharVal]  = UcharVal2;
			DLTPGConfig[DLTPGNoNdx].Pattern           = UcharVal; // save also pattern in Config

			ErrorCode = TrxDLTPGPattern(DLTPGNoNdx, UcharVal);

			ErrorCode = TrxDLTPGAttributes(DLTPGNoNdx, UcharVal2);
			if (ErrorCode)
			{
				board = FindDLTPGBoard(DLTPGNoNdx);
				GotoErrorStatus(256*board + ErrorCode);
				return;
			}
			SelectModeOn = FALSE;
			Ptr = PrevPtr;
			Ptr->DispFunct();       // rerutn to DLTPG_submenu
			return;
		}
		else                      // If escape ..restore old attribs.....
		{
			ErrorCode = TrxDLTPGAttributes(DLTPGNoNdx, DLTPGConfig[DLTPGNoNdx].Attrib[UcharVal]);
			if (ErrorCode)
			{
				board = FindDLTPGBoard(DLTPGNoNdx);
				GotoErrorStatus(256*board + ErrorCode);
				return;
			}
			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			WriteCode1(DLTPG_menu[DLTPGNoNdx].ItemTxt);
			WriteChar1('/');
			WriteCode1(DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]].ItemTxt);
			ClearRestLN1();                       // Clear rest of line 1

			if (UcharVal == DLTPGConfig[DLTPGNoNdx].Pattern) // If current system..
				FuncState = 7;                                //  remove SAVE option
			else
				FuncState = 10;

			FuncTriggers = 0;
			FuncSave     = FuncEscape = FALSE;

			Ptr   = &DLTPG_sub_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];
			Ptr->DispFunct();
			return;              // return to SYSTEM menu
		}
	} // if (FuncSave || FuncEscape)

	if (FuncTriggers & 0x03)    // If UP/DOWN button pressed..
	{
		UcharVal2 = FindHDAttribute(FuncTriggers, UcharVal, UcharVal2);

		if (UcharVal2 == DLTPGConfig[DLTPGNoNdx].Attrib[UcharVal])  // If current attrib
			FuncState = 1;                                            //  remove SAVE option

		ErrorCode = TrxDLTPGAttributes(DLTPGNoNdx, UcharVal2);
		if (ErrorCode)
		{
			board = FindDLTPGBoard(DLTPGNoNdx);

			GotoErrorStatus(256*board + ErrorCode);
			return;
		}
	}

	ClearRestLN2();
	WriteType2_SAVE_ESC(FuncState);                   // Write the ESC text
	/* Point tot the right table with attrib text */
	switch (UcharVal)
	{
	case HDColorbar:
	case HDCombination:
		CodePtr = HDTPGACBttribArrTxt[UcharVal2];     // Get attribute text
		break;
	case HDWhite:
	case HDWindow:
		CodePtr = HDTPGAWindowttribArrTxt[UcharVal2]; // Get attribute text
		break;
	case HDCrosshatch: /* This is not necessary, HDCrosshatch: no attrib */
		CodePtr = HDTPGAChttribArrTxt[UcharVal2];     // Get attribute text
		break;
	}
	WriteCodeLN2(1, CodePtr);                 // Write attribute

	ClearRestLN2();
	WriteType1_SAVE_ESC(FuncState);        // Write the SAVE ESC text
	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(1, Cstrlen(CodePtr));// Mark selected item
	FuncTriggers = 0;                      // Reset the button-selection
}

/***************************************************************************/
/* void DLTPGTimingSelect()                                        MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 03.03.2005                                                     */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in DLTPG submenu, which is as follows:         */
/*           2NDMNU:  PATT  TEXT  SYSTEM  EDH  AUDIO  <TIMING>             */
/*           having TIMING selected.                                       */
/*                                                                         */
/* Remarks:  UcharVal contains System here                                 */
/*                                                                         */
/* Returns: Nothing                                                        */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGTimingSelect()
{
	UC board;

	/* if system is OFF... do nothing. */
	if (DLTPGConfig[DLTPGNoNdx].System == 0)
	{
		CharBlinkLN2(0, OFF);
		SelectModeOn = FALSE;
		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If delay has changed..
			ActivePresetNo = 0;
			DLTPGConfig[DLTPGNoNdx].Delay = tlg_samples;  // Save new DL delay
		}
		else                    /* If ESC, transmit DLTPGConfig[DLTPGNoNdx].Delay */
		{
			ErrorCode = TrxDLTPGSystem(DLTPGNoNdx,
			                           DLTPGConfig[DLTPGNoNdx].System,
			                           DLTPGConfig[DLTPGNoNdx].Delay,
			                           GenlockConfig[ActiveGenlockNo].Delay);
			if (ErrorCode)
			{
				board = FindDLTPGBoard(DLTPGNoNdx);

				GotoErrorStatus(256*board + ErrorCode);
				return;
			}
		}
		CharBlinkLN2(0, OFF);
		SelectModeOn = FALSE;
		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}
//HDTPGTimeAdjust(DLTPGNoNdx, UcharVal, DLTPGConfig[DLTPGNoNdx].Delay, 1);
	TLGTimeAdjust(DLTPGNoNdx, UcharVal, DLTPGConfig[DLTPGNoNdx].Delay, 1);
	FuncTriggers = 0;
}

/***************************************************************************/
/* void DLTPGSystemSelect()                                        MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 03.03.2005                                                     */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in DLTPG submenu, which is as follows:         */
/*           2NDMNU:  PATT  TEXT  <SYSTEM>  EDH  AUDIO  TIMING             */
/*           having SYSTEM selected.                                       */
/*                                                                         */
/* Remarks: When browsing systems, they are not transmitted to board.      */
/*          They are transmitted only when saved.                          */
/*                                                                         */
/* Returns: Nothing                                                        */
/* Notes:   FuncState = 13 means that consecutive Left or Right buttons    */
/*          will display System Attributes menu.                           */
/*          UcharVal  contains System                                      */
/***************************************************************************/
void DLTPGSystemSelect()
{
	UC board;

	if (FuncState == 13) // FuncState = 13 means that Interface attribs are to be displayed..
	{
		// See comments below ....
		WriteCodeLN1(0, SecondmnuHdrTxt);   // Write menu header on line 1
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(INTERFACETxt);
		WriteTxt1(", ");
		CodePtr = DLTPGSystemArrTxt[UcharVal];// Get text for selected system
		WriteCode1(CodePtr);                  // Write pattern text on line 1
		ClearRestLN1();                       // Clear rest of line 1
		//     UcharVal = Pattern
		UcharVal2 = DLTPGConfig[DLTPGNoNdx].Interface[UcharVal]; // Get system attribute to UcharVal2

		FuncState    = 1;
		FuncTriggers = 0;
		FuncSave     = FuncEscape = FALSE;

		Ptr = &DLTPGsystem_modify_menu;    // Set pointer to MODIFY menu
		Ptr->DispFunct();                  // Display HDTPGPatternModify() menu
		return;                            // execute function HDTPGPatternModify()
	}

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If System has changed..
			ActivePresetNo = 0;            // Clear possible preset
			DLTPGConfig[DLTPGNoNdx].System = UcharVal;    // Save new System

			/* recalculate new delay on the basis of delay for old system */
			DLTPGConfig[DLTPGNoNdx].Delay = adjust_hd_samples(DLTPGConfig[DLTPGNoNdx].Delay, UcharVal, dltpg_fmt_table);

			ErrorCode = TrxDLTPGSystem(DLTPGNoNdx,
			                           DLTPGConfig[DLTPGNoNdx].System,
			                           DLTPGConfig[DLTPGNoNdx].Delay,
			                           GenlockConfig[ActiveGenlockNo].Delay);
			if (ErrorCode)
			{
				board = FindDLTPGBoard(DLTPGNoNdx);

				GotoErrorStatus(256*board + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;          // Clear select-mode
		Ptr = PrevPtr;                 // Set pointer to previous menu
		Ptr->DispFunct();              // Display previous menu
		return;
	}
	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal-- == 0)
				UcharVal = MAX_DL_SYSTEMS - 1;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal++ == (MAX_DL_SYSTEMS - 1))
				UcharVal = 0;
		}
		if (UcharVal == DLTPGConfig[DLTPGNoNdx].System) // If actual format..
			FuncState = 1;                                //  remove SAVE option
		/* JK change, do NOT transmit System, only when saved... */
	}

	/* If HAS INTERF (= "1", OR 2...etc    */
	/* Code below decides which systems will have interfaces  */
	/* i.e. which will have the "..." option.                 */
	if ((UcharVal > 0) && (UcharVal < 14)) /* if have interface */
	{
		if (FuncState == 1)
			FuncState = 6;
		if (FuncState == 3)
			FuncState = 9;
	}
	else                                   /* if NOT have interface */
	{
		if (FuncState == 6)
			FuncState = 1;
		if (FuncState == 9)
			FuncState = 3;
	}

	CodePtr = DLTPGSystemArrTxt[UcharVal];     // Get text for selection
	WriteCharLN2(0, ' '  );                 // No header for line 2
	WriteCode2(CodePtr);                    // Write the selected pattern
	ClearRestLN2();                         // Clear rest of line 2
	WriteType7a_SAVE_ESC(FuncState);         // Write the SAVE ESC texct INTERFACETxt

	switch (FuncState )
	{
	case 1:
	case 3:
	case 6:
	case 9:                      // Mark selected item
		WriteItemArrows(1, Cstrlen(CodePtr));
		break;
	}
	FuncTriggers = 0;
}

/***************************************************************************/
/* void DLTPGInterfaceModify()                                     MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 03.03.2005                                                     */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Left or Right key (+more), which is as follows:               */
/*            <XXX>   <INTERFACE>         ESC                              */
/*            <XXX>   <INTERFACE>  SAVE   ESC                              */
/*             XXX    <INTERFACE>        <ESC>                             */
/*             XXX    <INTERFACE>  SAVE  <ESC>                             */
/*                                                                         */
/* Remarks:                                                                */
/*           When this function is invoked, the first line on display has  */
/*           already been written.                                         */
/*                                                                         */
/*           UcharVal2 holds current interface                             */
/*           UcharVal  hOlds current system                                */
/*           When SAVE, this function saves System in DLTPGConfig.         */
/* Returns:  Nothing                                                       */
/* Notes:    When this function  escapes, we must return to                */
/*           displaying System,(DLTPGSystemSelect),                        */
/*           and not to DLTPGSubMenuShow()                                 */
/*           This function implements NON STANDARD BEHAVIOUR               */
/*                                                                         */
/*           Only systems 1...13 have interface                            */
/***************************************************************************/
void DLTPGInterfaceModify()
{
	UC board;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If interface has changed..
			ActivePresetNo = 0;
			DLTPGConfig[DLTPGNoNdx].Interface[UcharVal]  = UcharVal2;
			DLTPGConfig[DLTPGNoNdx].System               = UcharVal; // save also System in Config

			ErrorCode = TrxDLTPGSystem(DLTPGNoNdx,
			                           UcharVal,
			                           DLTPGConfig[DLTPGNoNdx].Delay,
			                           GenlockConfig[ActiveGenlockNo].Delay);

			ErrorCode = TrxDLTPGInterface(DLTPGNoNdx, UcharVal2);
			if (ErrorCode)
			{
				board = FindDLTPGBoard(DLTPGNoNdx);
				GotoErrorStatus(256*board + ErrorCode);
				return;
			}
			SelectModeOn = FALSE;
			Ptr = PrevPtr;
			Ptr->DispFunct();       // rerutn to DLTPG_submenu
			return;
		}
		else                      // If escape ..restore old pattern and old attribs.....
		{
			//  UC TrxDLTPGInterface(UC ndx, UC inter)
			ErrorCode = TrxDLTPGInterface(DLTPGNoNdx, DLTPGConfig[DLTPGNoNdx].Interface[UcharVal]);
			if (ErrorCode)
			{
				board = FindDLTPGBoard(DLTPGNoNdx);
				GotoErrorStatus(256*board + ErrorCode);
				return;
			}
			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			WriteCode1(DLTPG_menu[DLTPGNoNdx].ItemTxt);
			WriteChar1('/');
			WriteCode1(DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]].ItemTxt);
			ClearRestLN1();                       // Clear rest of line 1

			if (UcharVal == DLTPGConfig[DLTPGNoNdx].System) // If current system..
				FuncState = 7;                                //  remove SAVE option
			else
				FuncState = 10;

			FuncTriggers = 0;
			FuncSave     = FuncEscape = FALSE;

			Ptr   = &DLTPG_sub_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]];
			Ptr->DispFunct();
			return;              // return to SYSTEM menu
		}
	} // if (FuncSave || FuncEscape)

	if (FuncTriggers & 0x03)    // If UP/DOWN button pressed..
	{
		UcharVal2 = FindHDInterface(FuncTriggers, UcharVal, UcharVal2);

		if (UcharVal2 == DLTPGConfig[DLTPGNoNdx].Interface[UcharVal])  // If current interface
			FuncState = 1;                                               //  remove SAVE option
	}

	//WriteCodeLN2(0, ModifyHdrTxt);
	ClearRestLN2();
	WriteType2_SAVE_ESC(FuncState);                   // Write the ESC text

	/* Point to the right table with attrib text */
	CodePtr = DLTPGSystInterTxt[UcharVal2];           // Get interface text
	WriteCodeLN2(1, CodePtr);

	ClearRestLN2();
	WriteType1_SAVE_ESC(FuncState);        // Write the SAVE ESC text
	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(1, Cstrlen(CodePtr));// Mark selected item
	FuncTriggers = 0;                      // Reset the button-selection
}

/***************************************************************************/
/* void DLTPGTextMenuShow(void)                                    MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in DLTPG submenu, which is as follows:         */
/*           2NDMNU:  PATT  <TEXT>  SYSTEM  TIMING                         */
/*           having TEXT selected.                                         */
/*           This function displays text menu for current DL generator:    */
/*             <EDIT> STYLE POS COLOR                                      */
/*                                                                         */
/* Remarks:  For current generator DLTPGTextMenuNdx[DLTPGNoNdx]            */
/*           holds selected item nr, i.e. EDIT, STYLE, etc. in text menu.  */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGTextMenuShow()
{
	UC tmp;
	register int i, j;

	WriteCodeLN1(0, ThirdmnuHdrTxt);
	WriteCode1(SlashDotHdrTxt);
	WriteCode1(DLTPG_menu[DLTPGNoNdx].ItemTxt); // DL9
	WriteChar1('/');
	WriteCode1(DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]].ItemTxt); // TEXT
	WriteCode1(CoSelectTxt);                                        // , select
	ClearRestLN1();

	WriteCodeLN2(0, ThirdmnuHdrTxt);

	DLTPGTextMenuNdx[DLTPGNoNdx] = Ptr->ItemNumber; // Update ../TEXT menu index

	i = j = ColorItem; // 4

	if (DLTPGtext_menu[DLTPGTextMenuNdx[DLTPGNoNdx]].ItemNumber < ColorItem)
		i = EditItem; // 0
	else
		j = BackgroundItem + 1; // 6

	for (; i < j; i++)
	{
		ClearToPosLN2(DLTPGtext_menu[i].ItemPos); // Find item column position
		WriteCode2(DLTPGtext_menu[i].ItemTxt);    // Write item texts
	}

	ClearRestLN2();

	i = Ptr->ItemConfig;

	WriteArrowsField(i);
	WriteItemArrows(Ptr->ItemPos, Cstrlen(Ptr->ItemTxt));
}


/***************************************************************************/
/* void DLTPGTextDown(void)                                        MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in TEXT submenu with one of the following:     */
/*           EDIT  STYLE  POS  COLOR selected                              */
/*           The selected item is in DLTPGTextMenuNdx[DLTPGNoNdx]          */
/*                                                                         */
/* Remarks:  This function prepares some global variables according to     */
/*           what was selected in the TEXT submenu for use by functions    */
/*           which actually perform required actions:                      */
/*                void DLTPGTextEditSelect()                               */
/*                void DLTPGTextStyleSelect()                              */
/*                void DLTPGTextPosSelect()                                */
/*                void DLTPGTextColorSelect()                              */
/*           In this function:                                             */
/*             Ptr = DLTPGtext_submenu[DLTPGTextMenuNdx[DLTPGNoNdx]]       */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGTextDown()
{
	UC itemNdx = DLTPGTextMenuNdx[DLTPGNoNdx];

	WriteCodeLN1(0, ThirdmnuHdrTxt);        // Write menu headline
	WriteCode1(SlashDotHdrTxt);
	WriteCode1(DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]].ItemTxt);
	WriteChar1('/');
	WriteCode1(DLTPGtext_menu[itemNdx].ItemTxt);
	WriteCode1(CoSelectTxt);
	ClearRestLN1();

	switch (DLTPGTextMenuNdx[DLTPGNoNdx])
	{
	case HDTPGTextEditItem:                  // Prepare for text editing
		UcharVal  = 0;
		UcharVal1 = 0;
		UcharVal2 = 2;
		strcpy(TextBuffer, DLTPGConfig[DLTPGNoNdx].Text[UcharVal]);
		// Find text enable information
		UcharVal3 = (DLTPGConfig[DLTPGNoNdx].TextEnable & (1 << UcharVal)) > 0;
		if (UintVal = strlen(TextBuffer))  // Position in Text string
			UintVal--;
		break;
	case HDTPGTextScaleItem:          // Prepare for scale selection
		UcharVal  = DLTPGConfig[DLTPGNoNdx].TextScale;
		UcharVal1 = 0;
		break;
	case HDTPGTextPosItem:            // Prepare for position selection
		UcharVal  = DLTPGConfig[DLTPGNoNdx].TextPos[0];
		UcharVal1 = DLTPGConfig[DLTPGNoNdx].TextPos[1];
		break;
	case HDTPGTextMovementItem:        // Prepare for MOV selection
		UcharVal  = DLTPGConfig[DLTPGNoNdx].TextMovement;
		UcharVal1 = 0;
		break;
	case HDTPGTextColorItem:          // Prepare for COLOR selection
		UcharVal  = DLTPGConfig[DLTPGNoNdx].TextColor & 0x0F;
		UcharVal1 = 0;
		break;
	case HDTPGTextBAckgroundItem:     // Prepare for BACKGROUND selection
		UcharVal  = DLTPGConfig[DLTPGNoNdx].TextColor >> 4;
		UcharVal1 = 0;
		break;
	}
	PrevPtr =  Ptr;
	Ptr     = &DLTPGtext_submenu[DLTPGTextMenuNdx[DLTPGNoNdx]]; // Point to selected item
	FuncState    = 1;
	FuncTriggers = 0;
	FuncSave     = FuncEscape = FALSE;
	SelectModeOn = TRUE;
}

/***************************************************************************/
/* void DLTPGTextEditSelect(void)                                  MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in TEXT submenu:                               */
/*              <EDIT>  STYLE   POS   CLOCK                                */
/*           with EDIT selected.                                           */
/*           This function writes Line 2 menu:                             */
/*           LINE1:<abc>            <Off>       ESC                        */
/*           This function is entered each time user presses UP or DOWN    */
/*           key or changs text, putting or removing SAVE  option as       */
/*           necessary.                                                    */
/*                                                                         */
/* Remarks:  On entry:                                                     */
/*            UcharVal   = 0; Line nr                                      */
/*            UcharVal1  = 0; Min line                                     */
/*            UcharVal2  = 2; Max line (3 lines of text)                   */
/*            TextBuffer = DLTPGConfig[DLTPGNoNdx].Text[0]);               */
/*            UcharVal3  = Text enable info for line 0                     */
/*            UintVal    = strlen(TextBuffer)                              */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGTextEditSelect()
{
	UC tmp;
	UC board;

	if (FuncEscape)
	{
		CharBlinkLN2(0, OFF);   // Switch off possible blinking
		SelectModeOn = FALSE;   // Clear select-mode
		Ptr = PrevPtr;          // Set pointer to previous menu
		Ptr->DispFunct();       // Display previous menu
		return;
	}

	if (FuncSave)
	{
		// If textinfo has changed..
		FuncSave       = FALSE;
		ActivePresetNo = 0;       // Clear possible preset
		// Save the new text
		strcpy(DLTPGConfig[DLTPGNoNdx].Text[UcharVal], TextBuffer);
		// Save text enable information
		DLTPGConfig[DLTPGNoNdx].TextEnable &= ~(1 << UcharVal);
		DLTPGConfig[DLTPGNoNdx].TextEnable |= (UcharVal3 << UcharVal);

		//tmp = (UcharVal % 3) + 1; // Calculate line number
		// Transmit text line enable

		ErrorCode = TrxDLTPGText(DLTPGNoNdx, &DLTPGConfig[DLTPGNoNdx]);
		/************************************************************************/
		/* Note: It is impossible to develop proper communiction error handling */
		/*       here as to do so would require having working DL board which   */
		/*       does not give Level Error.                                     */
		/*       A button must be pressed before we go to status!               */
		/************************************************************************/
		if (ErrorCode)
		{
			board = FindDLTPGBoard(DLTPGNoNdx);
			ErrorStatus = 0;
			GotoErrorStatus(256*board + ErrorCode);
			//return;
		}
	}
	// Select line to edit
	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal-- == UcharVal1 )
				UcharVal = UcharVal2;
		}                                // ..button pressed is DOWN
		else
		{
			if (UcharVal++ == UcharVal2)
				UcharVal = UcharVal1;
		}                          // Copy selected text to temp.
		//  text buffer
		strcpy(TextBuffer, DLTPGConfig[DLTPGNoNdx].Text[UcharVal]);
		// Find text enable information
		UcharVal3 = (DLTPGConfig[DLTPGNoNdx].TextEnable & (1 << UcharVal)) > 0;

		if (UintVal = strlen(TextBuffer))    // Find position in string
			UintVal--;

		FuncState = 1;
	}
	// Select textline ON/OFF
	if (FuncTriggers & 0x0C)
	{
		// If UP/DOWN button pressed..
		UcharVal3 ^= ON;          // Toggle selection

		if (UcharVal3 == (DLTPGConfig[DLTPGNoNdx].TextEnable & (1 << UcharVal)) > 0)
			if (!strcmp(DLTPGConfig[DLTPGNoNdx].Text[UcharVal], TextBuffer))
				FuncState = 2;
	}
	// Select position in TextBuffer
	if (FuncTriggers & 0xC0)     // If LEFT/RIGHT button pressed..
	{
		if (FuncTriggers & 0x40)   // If button pressed is LEFT..
		{
			if (UintVal)        /* Algor is changed here compared to orig 8633 */
			{
				/***********************************************/
				TextBuffer[UintVal] = 0; // Delete character
				UintVal--;               // Decrement pointer to last character
			}
		}
		else
		{
			// ..button pressed is RIGHT
			tmp = 15;                 // Max. 16 characters in PT8632/3
			if ((UintVal < tmp) && (UintVal < strlen(TextBuffer)))
				UintVal++;
		}
	}
	if (FuncTriggers & 0x30)
	{
		// If UP/DOWN button pressed..
		if (TextBuffer[UintVal] == 0)
		{
			// If text insert..
			TextBuffer[UintVal] = TextBuffer[UintVal-1]; //  set new character = previous
			TextBuffer[UintVal+1] = 0;
		}
		tmp = GetNext((FuncTriggers & 0x10), TextBuffer[UintVal]);
		TextBuffer[UintVal] = tmp;

		if (!strcmp(DLTPGConfig[DLTPGNoNdx].Text[UcharVal], TextBuffer))
			if (UcharVal3 == (DLTPGConfig[DLTPGNoNdx].TextEnable & (1 << UcharVal)) > 0)
				FuncState = 8;
	}
	CodePtr = TPGTextEditArrTxt[UcharVal % 3];
	WriteCodeLN2(0, CodePtr);        // Write header on line 2
	ClearRestLN2();                  // Clear rest of line 2
	WriteTxtLN2(5, ": ");
	WriteTxtLN2(7, TextBuffer);              // Write edited text on line 2
	WriteCodeLN2(25, OffOnArrTxt[UcharVal3]);// Write on/off on line 2
	WriteType12_SAVE_ESC(FuncState);         // Write the SAVE ESC text
	switch (FuncState)
	{
	case 1:
	case 4:                          // Mark selected item
		WriteItemArrows(7, strlen(TextBuffer));
		CharBlinkLN2(0, OFF);          // Switch OFF blinking
		break;
	case 2:
	case 5:                          // Mark selected item
		WriteItemArrows(25, Cstrlen(OffOnArrTxt[UcharVal3]));
		CharBlinkLN2(0, OFF);          // Switch OFF blinking
		break;
	case 8:
	case 9:                          // Mark selected item
		WriteItemBrackets(7, strlen(TextBuffer));
		CharBlinkLN2(7 + UintVal, ON); // Switch OFF blinking at
		break;                         //  editing character
	}
	FuncTriggers = 0;                  // Reset the button-selection
}

/***************************************************************************/
/* void DLTPGTextStyleSelect(void)                                 MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in TEXT submenu:                               */
/*              EDIT  <SCALE>  POS   COLOR                                 */
/*           with SCALE selected.                                          */
/*           This function writes Line 2 menu:                             */
/*           SELECT:<1>                      ESC                           */
/*           This function is entered each time user presses UP or DOWN    */
/*           key or changs text, putting or removing SAVE  option as       */
/*           necessary.                                                    */
/*                                                                         */
/* Remarks:  ON entry:                                                     */
/*           UcharVal  = DLTPGConfig[DLTPGNoNdx].TextScale;                */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGTextScaleSelect()
{
	UC ndx = DLTPGNoNdx;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			ActivePresetNo = 0;
			DLTPGConfig[ndx].TextScale = UcharVal;  // Save new text scale
		}
		else
		{
			// ..restore text scale
			if (ErrorCode = TrxDLTPGTextScale(ndx, DLTPGConfig[ndx].TextScale))
			{
				GotoErrorStatus(256*ndx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;

		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal-- == ScaleOne)
				UcharVal = ScaleFour;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal++ == ScaleFour)
				UcharVal = ScaleOne;
		}
		if (DLTPGConfig[ndx].TextScale == UcharVal)
			FuncState = 1;               //  if current scale, remove SAVE option

		if (ErrorCode = TrxDLTPGTextScale(ndx, UcharVal))
		{
			GotoErrorStatus(256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = HDTextScaleArrTxt[UcharVal];  // Get text for selected level
	WriteCodeLN2(0, SelectHdrTxt);
	WriteCode2(CodePtr);
	ClearRestLN2();
	WriteType1_SAVE_ESC(FuncState);

	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(8, Cstrlen(CodePtr));// Mark selected item
	FuncTriggers = 0;
}


/***************************************************************************/
/* void DLTPGTextPosSelect(void)                                   MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in TEXT submenu:                               */
/*              EDIT   STYLE   <POS>   COLOR                               */
/*           with POS selected.                                            */
/*           This function writes Line 2 menu:                             */
/*           X:< +0>   Y:  +0                   ESC                        */
/*           This function is entered each time user presses UP or DOWN    */
/*           key or changes position, putting or removing SAVE  option as  */
/*           necessary.                                                    */
/*                                                                         */
/* Remarks:  On entry:                                                     */
/*           UcharVal  = DLTPGConfig[DLTPGNoNdx].TextPos[0];               */
/*           UcharVal1 = DLTPGConfig[DLTPGNoNdx].TextPos[1];               */
/*                                                                         */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGTextPosSelect()
{

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If text position has changed..
			ActivePresetNo = 0;
			DLTPGConfig[DLTPGNoNdx].TextPos[0] = UcharVal;
			DLTPGConfig[DLTPGNoNdx].TextPos[1] = UcharVal1;
		}
		else
		{
			// ..restore text style
			if (ErrorCode = TrxDLTPGTextPos(DLTPGNoNdx,
			                                DLTPGConfig[DLTPGNoNdx].TextPos[0],
			                                DLTPGConfig[DLTPGNoNdx].TextPos[1]))
			{
				GotoErrorStatus(256*DLTPGNoNdx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;

		Ptr = PrevPtr;
		Ptr->DispFunct();  // Display previous menu
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If button pressed is UP/DOWN..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal++ == HDXPosMax)
				UcharVal = HDXPosMin;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal-- == HDXPosMin)
				UcharVal = HDXPosMax;
		}
		if ((UcharVal == DLTPGConfig[DLTPGNoNdx].TextPos[0]) &&
		        (UcharVal1 == DLTPGConfig[DLTPGNoNdx].TextPos[1]))
			FuncState = 1;
		// Transmit new X position
		if (ErrorCode = TrxDLTPGTextPos(DLTPGNoNdx, UcharVal, UcharVal1))
		{
			GotoErrorStatus(256*DLTPGNoNdx + ErrorCode);
			return;
		}
	}

	if (FuncTriggers & 0x0C)
	{
		// If button UP/DOWN pressed..
		if (FuncTriggers & 0x04)
		{
			// If button pressed is UP..
			if (UcharVal1++ == HDYPosMax)
				UcharVal1 = HDYPosMin;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal1-- == HDYPosMin)
				UcharVal1 = HDYPosMax;
		}
		if ((UcharVal == DLTPGConfig[DLTPGNoNdx].TextPos[0]) &&
		        (UcharVal1 == DLTPGConfig[DLTPGNoNdx].TextPos[1]))
			FuncState = 2;
		if (ErrorCode = TrxDLTPGTextPos(DLTPGNoNdx, UcharVal, UcharVal1))
		{
			GotoErrorStatus(256*DLTPGNoNdx + ErrorCode);
			return;
		}
	}
	WriteTxtLN2(0, "X: ");                  // Write line 2 header
	sprintf(TextBuffer, "%+3d", UcharVal);  // Convert coordinate to ascii
	WriteTxt2(TextBuffer);
	ClearToPosLN2(10);
	WriteTxt2("Y: ");                       // Convert coordinate to ascii
	sprintf(TextBuffer, "%+3d", UcharVal1);
	WriteTxt2(TextBuffer);
	ClearRestLN2();
	WriteType2_SAVE_ESC(FuncState);        // Write the SAVE ESC text

	switch (FuncState)
	{
	case 1:
	case 5:
		WriteItemArrows(3, 3);
		break;
	case 2:
	case 4:
		WriteItemArrows(13, 3);
		break;
	}
	FuncTriggers = 0;                    // Reset button-selection
}

/***************************************************************************/
/* void DLTPGTextColorSelect(void)                                 MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in TEXT submenu:                               */
/*              EDIT  STYLE  POS <COLOR> BACKGROUND                        */
/*           with COLOR selected.                                          */
/*           This function writes Line 2 menu:                             */

/*           This function is entered each time user presses UP or DOWN    */
/*           key or changs text, putting or removing SAVE  option as       */
/*           necessary.                                                    */
/*                                                                         */
/* Remarks:  On entry:                                                     */
/*                 UcharVal  = DLTPGConfig[DLTPGNoNdx].TextColor & 0x0F;   */
/*                 UcharVal1 = 0;                                          */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:    DLTPGConfig[DLTPGNoNdx].TextColor member carries text color   */
/*           and text background info in the following manner:             */
/*                                                                         */
/*           |X|R|G|B|X|R|G|B| .TextColor byte                             */
/*                                                                         */
/*           Bits 0-2 represent text RGB value, bits 4-6 represent         */
/*           background RGB value.                                         */
/***************************************************************************/
void DLTPGTextColorSelect()
{
	UC ndx = DLTPGNoNdx;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			ActivePresetNo = 0;
			DLTPGConfig[ndx].TextColor &= 0xF0;      // Zero text color
			DLTPGConfig[ndx].TextColor |= UcharVal;  // Save new text color
		}
		else
		{
			// ..restore text color
			if (ErrorCode = TrxDLTPGTextColor(ndx, DLTPGConfig[ndx].TextColor))
			{
				GotoErrorStatus(256*ndx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;
		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01) // If button pressed is UP..
		{
			if (UcharVal-- == ColorBlack)
				UcharVal = ColorWhite;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal++ == ColorWhite)
				UcharVal = ColorBlack;
		}
		if ((DLTPGConfig[ndx].TextColor & 0x0F) == UcharVal)
			FuncState = 1;               //  if current color, remove SAVE option

		UcharVal1  = DLTPGConfig[ndx].TextColor;
		UcharVal1 &= 0xF0;             // zero color part...
		UcharVal1 |= UcharVal;         // place new color....
		if (ErrorCode = TrxDLTPGTextColor(ndx, UcharVal1))
		{
			GotoErrorStatus(256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = HDTextColorArrTxt[UcharVal];  // Get text for selected color
	WriteCodeLN2(0, SelectHdrTxt);
	WriteCode2(CodePtr);
	ClearRestLN2();
	WriteType1_SAVE_ESC(FuncState);

	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(8, Cstrlen(CodePtr));// Mark selected item
	FuncTriggers = 0;
}

/***************************************************************************/
/* void DLTPGTextBackgroundSelect(void)                            MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in TEXT submenu:                               */
/*              EDIT  STYLE  POS  COLOR <BACKGROUND>                       */
/*           with BACKGROUND selected.                                     */
/*           This function writes Line 2 menu:                             */

/*           This function is entered each time user presses UP or DOWN    */
/*           key or changs text, putting or removing SAVE  option as       */
/*           necessary.                                                    */
/*                                                                         */
/* Remarks:  On entry:                                                     */
/*                 UcharVal  = DLTPGConfig[DLTPGNoNdx].TextColor >> 4;     */
/*                 UcharVal1 = 0;                                          */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:    DLTPGConfig[DLTPGNoNdx].TextColor member carries text color   */
/*           and text background info in the following manner:             */
/*                                                                         */
/*           |X|R|G|B|X|R|G|B| .TextColor byte                             */
/*                                                                         */
/*           Bits 0-2 represent text RGB value, bits 4-6 represent         */
/*           background RGB value.                                         */
/***************************************************************************/
void DLTPGTextBackgroundSelect()
{
	UC ndx = DLTPGNoNdx;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			ActivePresetNo = 0;
			UcharVal = UcharVal << 4;
			DLTPGConfig[ndx].TextColor &= 0x0F;      // Zero background nibble
			DLTPGConfig[ndx].TextColor |= UcharVal;  // Save new text background
		}
		else
		{
			// ..restore text color
			if (ErrorCode = TrxDLTPGTextColor(ndx, DLTPGConfig[ndx].TextColor))
			{
				GotoErrorStatus(256*ndx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;
		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01) // If button pressed is UP..
		{
			if (UcharVal-- == ColorBlack)
				UcharVal = ColorWhite;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal++ == ColorWhite)
				UcharVal = ColorBlack;
		}
		if ((DLTPGConfig[ndx].TextColor >> 4) == UcharVal)
			FuncState = 1;               //  if current color, remove SAVE option

		UcharVal1  = DLTPGConfig[ndx].TextColor;
		UcharVal1 &= 0x0F;             // zero background part...
		UcharVal1 |= UcharVal << 4;    // place new background....

		if (ErrorCode = TrxDLTPGTextColor(ndx, UcharVal1))
		{
			GotoErrorStatus(256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = HDTextColorArrTxt[UcharVal];  // Get text for selected level
	WriteCodeLN2(0, SelectHdrTxt);
	WriteCode2(CodePtr);
	ClearRestLN2();
	WriteType1_SAVE_ESC(FuncState);

	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(8, Cstrlen(CodePtr));// Mark selected item
	FuncTriggers = 0;
}




/***************************************************************************/
/* void DLTPGTextMovementSelect(void)                              MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  17.02.2006                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in TEXT submenu:                               */
/*              EDIT   STYLE    POS   COLOR  <MOVEMENT>                    */
/*           with MOVEMENT selected.                                       */
/*           This function writes Line 2 menu:                             */
/*                                                                         */
/*           This function is entered each time user presses UP or DOWN    */
/*           key or changs text, putting or removing SAVE  option as       */
/*           necessary.                                                    */
/*                                                                         */
/* Remarks:  On entry:                                                     */
/*                 UcharVal  = DLTPGConfig[DLTPGNoNdx].TextMOvement        */
/*                 UcharVal1 = 0;                                          */
/*                                                                         */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGTextMovementSelect()
{
	UC ndx = DLTPGNoNdx;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			ActivePresetNo = 0;
			DLTPGConfig[ndx].TextMovement = UcharVal;  // Save new text movement
		}
		else
		{
			// ..restore text movement
			if (ErrorCode = TrxDLTPGTextMovement(ndx, DLTPGConfig[ndx].TextMovement))
			{
				GotoErrorStatus(256*ndx + ErrorCode);
				return;
			}
		}
		SelectModeOn = FALSE;
		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01) // If button pressed is UP..
		{
			if (UcharVal-- == TextMovementNone)
				UcharVal = TextMovementBoth;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal++ == TextMovementBoth)
				UcharVal = TextMovementNone;
		}
		if (DLTPGConfig[ndx].TextMovement == UcharVal)
			FuncState = 1;               //  if current color, remove SAVE option

		if (ErrorCode = TrxDLTPGTextMovement(ndx, UcharVal))
		{
			GotoErrorStatus(256*ndx + ErrorCode);
			return;
		}
	}

	CodePtr = HDTextMovArrTxt[UcharVal];  // Get text for selected level
	WriteCodeLN2(0, SelectHdrTxt);
	WriteCode2(CodePtr);
	ClearRestLN2();
	WriteType1_SAVE_ESC(FuncState);

	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(8, Cstrlen(CodePtr));// Mark selected item
	FuncTriggers = 0;
}

/***************************************************************************/
/* void DLTPGAudioMenuShow(void)                                   MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in DLTPG submenu, which is as follows:         */
/*           2NDMNU:  PATT  TEXT  SYSTEM  EDH  <AUDIO>  TIMING             */
/*           having AUDIO selected.                                        */
/*           This function displays audio menu for current DL generator:   */
/*             <SIGNAL>  LEVEL  GROUP                                      */
/*                                                                         */
/* Remarks:  For current generator DLTPGAudioMenuNdx[DLTPGNoNdx]           */
/*           holds selected item nr, i.e. SIGNAL, LEVEL, etc. in audio     */
/*           menu.                                                         */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGAudioMenuShow()
{
	register int i, j;

	WriteCodeLN1(0, ThirdmnuHdrTxt);
	WriteCode1(SlashDotHdrTxt);
	WriteCode1(DLTPG_menu[DLTPGNoNdx].ItemTxt);
	WriteChar1('/');
	WriteCode1(DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]].ItemTxt);
	WriteCode1(CoSelectTxt);                 // Write sub-menu headline
	ClearRestLN1();

	DLTPGAudioMenuNdx[DLTPGNoNdx] = Ptr->ItemNumber; // Update ../EMB.AUDIO menu index
	WriteCodeLN2(0, ThirdmnuHdrTxt);

	i = j = DLTPGAudioClickItem; // 3

	if (DLTPGaudio_menu[DLTPGAudioMenuNdx[DLTPGNoNdx]].ItemNumber < DLTPGAudioClickItem)
		i = DLTPGAudioSignalItem; // 0
	else
		j = DLTPGAudioClickItem + 1; // 5

	for (; i < j; i++)
	{
		ClearToPosLN2(DLTPGaudio_menu[i].ItemPos);  // Find item column position
		WriteCode2(DLTPGaudio_menu[i].ItemTxt);     // Write item texts
	}

	ClearRestLN2();
	i = Ptr->ItemConfig;

	WriteArrowsField(i);
	WriteItemArrows(Ptr->ItemPos, Cstrlen(Ptr->ItemTxt));
}

#if 0
enum DLTPGAudioItems
{
	DLTPGAudioSignalItem = 0, DLTPGAudioLevelItem, DLTPGAudioGroupItem,\
	DLTPGAudioClickItem, DLTPGAudioTimingItem
};
#endif

/***************************************************************************/
/* void DLTPGAudioDown(void)                                       MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in AUDIO submenu with one of the following:    */
/*              SIGNAL  LEVEL  GROUP  CLICK OFFSET  TIMING                 */
/*           selected                                                      */
/*           The selected item is in DLTPGAudioMenuNdx[DLTPGNoNdx]         */
/*                                                                         */
/* Remarks:  This function prepares some global variables according to     */
/*           what was selected in the AUDIO submenu for use by functions   */
/*           which actually perform required actions:                      */
/*                void DLTPGAudioSignalSelect()                            */
/*                void DLTPGAudioLevelSelect()                             */
/*                void DLTPGAudioGroupSelect()                             */
/*           and writes Line 1 header according to                         */
/*           DLTPGAudioMenuNdx[DLTPGNoNdx]                                 */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGAudioDown()
{
	UC itemNdx = DLTPGAudioMenuNdx[DLTPGNoNdx];

	WriteCodeLN1(0, ThirdmnuHdrTxt);        // Write menu headline
	WriteCode1(SlashDotHdrTxt);
	WriteCode1(DLTPG_submenu[DLTPGsubmenuNdx[DLTPGNoNdx]].ItemTxt);
	WriteChar1('/');
	WriteCode1(DLTPGaudio_menu[itemNdx].ItemTxt);
	WriteCode1(CoSelectTxt);
	ClearRestLN1();

	switch (itemNdx)
	{
	case DLTPGAudioSignalItem:
		UcharVal = DLTPGConfig[DLTPGNoNdx].AudioSignal;
		break;
	case DLTPGAudioLevelItem:
		UcharVal = DLTPGConfig[DLTPGNoNdx].AudioLevel;
		break;
	case DLTPGAudioClickItem:
		IntVal = DLTPGConfig[DLTPGNoNdx].AudioClick;
		break;
	}
	PrevPtr = &DLTPGaudio_menu   [DLTPGAudioMenuNdx[DLTPGNoNdx]]; // Ptr; // Store old menu-pointer
	Ptr     = &DLTPGaudio_submenu[DLTPGAudioMenuNdx[DLTPGNoNdx]]; // Point to selected item

	FuncState    = 1;   // Prepare for item state-machine
	FuncTriggers = 0;
	FuncSave     = FuncEscape = FALSE;
	SelectModeOn = TRUE;
}

/***************************************************************************/
/* void DLTPGAudioSignalSelect(void)                               MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in AUDIO submenu:                              */
/*             <SIGNAL> LEVEL  GROUP  CLICK OFFSET  TIMING                 */
/*           with SIGNAL selected.                                         */
/*           This function writes Line 2 menu:                             */
/*           SIGNAL: <Off>       ESC                                       */
/*           This function is entered each time user presses UP or DOWN    */
/*           key, putting or removing SAVE  option as necessary.           */
/*                                                                         */
/* Remarks:                                                                */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGAudioSignalSelect()
{
	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If audio signal has changed..
			ActivePresetNo = 0;
			DLTPGConfig[DLTPGNoNdx].AudioSignal = UcharVal;
		}
		else
		{
			// If escape ..restore audio signal
			if (UcharVal != DLTPGConfig[DLTPGNoNdx].AudioSignal)
				if (ErrorCode = TrxDLTPGAudioSignal(DLTPGNoNdx, DLTPGConfig[DLTPGNoNdx].AudioSignal))
				{
					GotoErrorStatus(256*DLTPGNoNdx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;
		Ptr = PrevPtr;
		Ptr->DispFunct();
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal-- == HDSilence)
				UcharVal = HDAudioOff;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal++ == HDAudioOff)
				UcharVal = HDSilence;
		}

		if (UcharVal == DLTPGConfig[DLTPGNoNdx].AudioSignal)  // If current signal
			FuncState = 1;                                      //  remove SAVE option
		if (ErrorCode = TrxDLTPGAudioSignal(DLTPGNoNdx, UcharVal))
		{
			GotoErrorStatus(256*DLTPGNoNdx + ErrorCode);
			return;
		}
	}
	CodePtr = HDAudioSignalArrTxt[UcharVal];   // Get text for selected signal
	WriteCodeLN2(0, SignalHdrTxt);             // Write header for line 2
	WriteCode2(CodePtr);                       // Write selected text
	ClearRestLN2();                            // Clear rest of line 2
	WriteType1_SAVE_ESC(FuncState);            // Write the SAVE ESC text
	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(8, Cstrlen(CodePtr));    // Mark selected item
	FuncTriggers = 0;                          // Reset the button-selection
}

/***************************************************************************/
/* void DLTPGAudioLevelSelect(void)                                MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  22.02.2005                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in AUDIO submenu:                              */
/*              SIGNAL <LEVEL> GROUP  CLICK OFFSET  TIMING                 */
/*           with LEVEL selected.                                          */
/*           This function writes Line 2 menu:                             */
/*           LEVEL: <Silence>       ESC                                    */
/*           This function is entered each time user presses UP or DOWN    */
/*           key, putting or removing SAVE  option as necessary.           */
/*                                                                         */
/* Remarks:                                                                */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGAudioLevelSelect()
{
	//code UC* tablePtr;

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If audio level has changed..
			ActivePresetNo = 0;
			DLTPGConfig[DLTPGNoNdx].AudioLevel = UcharVal;
		}
		else
		{
			// ..restore audio level
			if (UcharVal != DLTPGConfig[DLTPGNoNdx].AudioLevel)
				if (ErrorCode = TrxDLTPGAudioLevel(DLTPGNoNdx, DLTPGConfig[DLTPGNoNdx].AudioLevel))
				{
					GotoErrorStatus(256*DLTPGNoNdx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;  // Clear select-mode
		Ptr = PrevPtr;         // Set pointer to previous menu
		Ptr->DispFunct();      // Display previous menu
		return;
	}
	//tablePtr = PT8641AudioLevelTable;

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (UcharVal-- == HDLevel0)
				UcharVal = HDLevelM60;
		}
		else
		{
			// ..button pressed is DOWN
			if (UcharVal++ == HDLevelM60)
				UcharVal = HDLevel0;
		}

		if (UcharVal == HDTPGConfig[HDTPGNoNdx].AudioLevel)  // If current level
			FuncState = 1;                                     //  remove SAVE option
		if (ErrorCode = TrxHDTPGAudioLevel(HDTPGNoNdx, UcharVal))
		{
			GotoErrorStatus(256*HDTPGNoNdx + ErrorCode);
			return;
		}
	}
	//CodePtr = HDAudioLevelArrTxt[UcharVal];  // Get text for selected level
	WriteCodeLN2(0, LevelHdrTxt);
	//WriteCode2(CodePtr);
	if (UcharVal)
		sprintf(TextBuffer, "-%d dBFS", UcharVal);
	else
		sprintf(TextBuffer, " %d dBFS", UcharVal);
	WriteTxt2(TextBuffer);
	ClearRestLN2();
	WriteType1_SAVE_ESC(FuncState);
	if ((FuncState == 1) || (FuncState == 3))
		//WriteItemArrows(7, Cstrlen(CodePtr));// Mark selected item
		WriteItemArrows(7, strlen(TextBuffer));// Mark selected item
	FuncTriggers = 0;
}


/***************************************************************************/
/* void DLTPGAudioClickSelect(void)                                MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  23.03.2006                                                    */
/*                                                                         */
/* Function: This function is entered in response to user pressing the     */
/*           Down key while in AUDIO submenu:                              */
/*              SIGNAL  LEVEL  GROUP <CLICK OFFSET> TIMING                 */
/*           with CLICK OFFSET selected.                                   */
/*           This function writes Line 2 menu:                             */
/*           OFFSET: <000>       ESC                                       */
/*           This function is entered each time user presses UP or DOWN    */
/*           key, putting or removing SAVE  option as necessary.           */
/*                                                                         */
/* Remarks:                                                                */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:                                                                  */
/***************************************************************************/
void DLTPGAudioClickSelect()
{

	if (FuncSave || FuncEscape)
	{
		if (FuncSave)
		{
			// If Click has changed..
			ActivePresetNo = 0;
			DLTPGConfig[DLTPGNoNdx].AudioClick = IntVal;
		}
		else
		{
			if (IntVal != DLTPGConfig[DLTPGNoNdx].AudioClick)
				if (ErrorCode = TrxDLTPGAudioClick(DLTPGNoNdx, DLTPGConfig[DLTPGNoNdx].AudioClick))
				{
					GotoErrorStatus(256*DLTPGNoNdx + ErrorCode);
					return;
				}
		}
		SelectModeOn = FALSE;

		Ptr = PrevPtr;
		Ptr->DispFunct();     // Display previous menu
		return;
	}

	if (FuncTriggers & 0x03)
	{
		// If UP/DOWN button pressed..
		if (FuncTriggers & 0x01)
		{
			// If button pressed is UP..
			if (++IntVal > ClickHigh)      // If max. click reached..
				IntVal = ClickLow;           //  .. roll-over to min. value
		}
		else
		{
			// ..button pressed is DOWN
			if (--IntVal < ClickLow)       // If min. click reached..
				IntVal = ClickHigh;          //  .. roll-over to max. value
		}

		if (IntVal == DLTPGConfig[DLTPGNoNdx].AudioClick)   // If current click
			FuncState = 1;                                    //  remove SAVE option

		if (ErrorCode = TrxDLTPGAudioClick(DLTPGNoNdx, IntVal))
		{
			GotoErrorStatus(256*DLTPGNoNdx + ErrorCode);
			return;
		}
	}

	WriteCodeLN2(0, OffsetHdrTxt);        // Write line 2 header
	sprintf(TextBuffer, "%+4d", IntVal);  // Convert click to ascii
	Cstrcat(TextBuffer, mSTxt);           // Add prefix mS
	WriteTxt2(TextBuffer);                // Write click value
	ClearRestLN2();
	WriteType1_SAVE_ESC(FuncState);       // Write the SAVE ESC text

	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(8, strlen(TextBuffer));  // Mark selected item

	FuncTriggers = 0;     // Reset button selectors
}

UC FindNextDL(char present, char * next)
{
	UC i;

	for (i = present; i < MaxDLTPGUnits; i++)
	{
		if (DLTPGUnit[i].Present)
		{
			*next = i;
			return (1);
		}
	}
	return (0);
}

/***************************************************************************/
/* UC FindHDInterface(UC Triggers, UC system, UC interface)                */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 27.03.2006                                                     */
/*                                                                         */
/* Function: This function returns next value of interface based on which  */
/*           key (UP or DOWN) is pressed.                                  */
/*                                                                         */
/* Args:     UC Triggers: function triggers, indicating which (UP/DOWN)    */
/*                        button was pressed.                              */
/*           UC system :  system for which next interface is to be found.  */
/*           UC pattern:  current interface                                */
/* Remarks:                                                                */
/*                                                                         */
/*                                                                         */
/* Returns: Valid interface or 0xFF.                                       */
/* Notes:                                                                  */
/***************************************************************************/
UC FindHDInterface(UC Triggers, UC system, UC interface)
{

	if ((system > 0) && (system < 14))
	{
		if (Triggers & 0x01)         // If button pressed is UP..
		{
			if (interface++ == 5)      // check if equal and then increment...
				interface = 0;
		}
		else                         // ..button pressed is DOWN
		{
			if (interface-- == 0)
				interface = 5;
		}
		return (interface);
	}
	return (0);
}

/***************************************************************************/
/* LTCMenuShow                                  MENUB.C            */
/*                                                               */
/* Author:    Jens K Hansen, DEV, 26/9/2007                              */
/*                                                             */
/*  Function:  Display 1st line in LTC menu and prepare state machine.     */
/*  Remarks:    LTC MENU function.                                   */
/*          Cursor position on line 1 is updated.                      */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void LTCMenuShow()
{
	WriteCodeLN1(0, SubmnuHdrTxt);
	WriteCode1(LTCTxt);
	WriteCode1(CoSelectTxt);

	ClearRestLN1();


	// sprintf(tbuff, "test");
	//	   Udr_PutNChar(tbuff);
	//safety FuncState select, if not valid state
	if((FuncState>15) || (FuncState<10))
		FuncState=10;

	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;        // Set select-mode
}

/***************************************************************************/
/* LTCSelect                                  MENUB.C            */
/*                                                               */
/* Author:    Jens K Hansen, DEV, 26/9/2007                              */
/*                                                             */
/*  Function:  Select submenues for LTC generator              */
/*  Remarks:    LTC MENU function.                                   */
/*            Cursor position on line 2 is updated.                    */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void LTCSelect()
{
	register int length;

	// Handling of selections
	switch(FuncState)
	{
		//wrap around to the right
	case 16:
		FuncState=10;
		break;

		//wrap arount to the left
	case 19:
		FuncState=15;
		break;

		// Up pressed on ESC
	case 95:
		FuncEscape=TRUE;
		break;

		//If up/down pressed on LTC select
	case 20:
	case 90:
		//switch LTC
		if (LTCNo == 0)
			LTCNo = 1;
		else
			LTCNo = 0;

		FuncState=10;
		break;

		//if offset selected:
	case 21:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCArrTxt[LTCNo]);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCOffsetItem].ItemTxt);
		WriteCode1(CoEditDelayTxt);
		ClearRestLN1();

		longVal1=LTCConfig[LTCNo].Offset;

		FuncState = 10;
		Ptr = &LTC_menu[LTCOffsetItem];
		Ptr -> DispFunct();
		return;

		//if format selected:
	case 22:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCArrTxt[LTCNo]);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCFormatItem].ItemTxt);
		WriteCode1(CoSelectTxt);

		ClearRestLN1();

		//set temporary var to selected LTC's format
		UcharVal1=LTCConfig[LTCNo].Format;

		FuncState = 10;
		Ptr = &LTC_menu[LTCFormatItem];
		Ptr -> DispFunct();
		return;

		//if time selected:
	case 23:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCArrTxt[LTCNo]);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
		WriteCode1(CoSelectTxt);

		ClearRestLN1();

		FuncState = 10;
		Ptr = &LTC_menu[LTCTimeItem];
		Ptr -> DispFunct();
		return;

		//if sync selected:
	case 24:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(base_menu[BaseNdx].ItemTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCSyncConfirmItem].ItemTxt);
		WriteChar1(' ');
		WriteCode1(LTCTxt);
		WriteChar1(' ');
		WriteCode1(LTCSyncTxt);

		WriteCode1(CoSelectTxt);

		ClearRestLN1();
		FuncState = 10;
		Ptr = &LTC_menu[LTCSyncConfirmItem];
		Ptr -> DispFunct();
		return;

		//If up/down pressed on illegal positions, reset
	case 91:
		FuncState=11;
		break;
	case 92:
		FuncState=12;
		break;
	case 93:
		FuncState=13;
		break;
	case 94:
		FuncState=14;
		break;
	case 25:
		FuncState=15;
		break;
	}

	if (FuncSave || FuncEscape)
	{
		if(FuncState==95)
		{
			Ptr = PrevPtr;                  // Set pointer to previous menu
			Ptr->DispFunct();               // Display previous menu
			return;
		}

		FuncSave = FALSE;
	}

	//Write "<LTC X><OFFSET><FORMAT><TIME><SYNC><ESC>"
	CodePtr = LTCArrTxt[LTCNo];    // Get text for selection
	length = Cstrlen(CodePtr);       // Calculate length of string
	WriteCharLN2(0, ' ');
	WriteCode2(CodePtr);

	ClearToPosLN2(8);
	WriteCode2(OffsetTxt);

	ClearToPosLN2(16);
	WriteCode2(LTCFormatTxt);

	ClearToPosLN2(24);
	WriteCode2(TimeTxt);

	ClearToPosLN2(30);
	WriteCode2(LTCSyncTxt);

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);

	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(1, Cstrlen(LTCArrTxt[LTCNo]));
		WriteArrowsField(L_+U_+R_+D_);
		break;

	case 11:
		WriteItemArrows(8, Cstrlen(OffsetTxt));
		WriteArrowsField(L_+R_+D_);
		break;

	case 12:
		WriteItemArrows(16, Cstrlen(FormatTxt));
		WriteArrowsField(L_+R_+D_);
		break;

	case 13:
		WriteItemArrows(24, Cstrlen(TimeTxt));
		WriteArrowsField(L_+R_+D_);
		break;

	case 14:
		WriteItemArrows(30, Cstrlen(LTCSyncTxt));
		WriteArrowsField(L_+R_+D_);
		break;

	default:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
}

/***************************************************************************/
/* LTCOffsetSet                                 MENUB.C            */
/*                                                               */
/* Author:    Jens K Hansen, DEV, 26/9/2007                              */
/*                                                             */
/*  Function:  Edit the individual offsets for LTCs              */
/*  Remarks:    LTC MENU function.                                   */
/*            Cursor position on line 2 is updated.                    */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void LTCOffsetSet()
{
	register int length;
	char LTCOffsetStr[12]; //var static?
	double offset;

	switch(FuncState)
	{
		// Wrap to the right cases
	case 12:
		FuncState=10;
		break;
	case 43:
		FuncState=40;
		break;
	case 78:
		FuncState=70;
		break;

		// wrap to the left cases
	case 19:
		FuncState=11;
		break;
	case 49:
		FuncState=42;
		break;
	case 79:
		FuncState=77;
		break;

		// down pressed on <esc>
	case 21:
		FuncState=11;
		break;
		// down pressed on <ok>
	case 51:
		FuncState=41;
		break;
		// down pressed on <esc>
	case 52:
		FuncState=42;
		break;

		// Up pressed on <esc>
	case 32:
	case 91:
		FuncEscape=TRUE;
		break;

		// Up pressed on <OK>
	case 31:
		FuncState=41;
		break;

		// If up pressed on offset
	case 90:
	case 30:
		longVal1++;

		if(longVal1>74250000)
			longVal1=-74250000;

		FuncState=40;
		break;

		// If down pressed on offset
	case 20:
	case 50:
		longVal1--;

		if(longVal1<-74250000)
			longVal1=74250000;

		FuncState=40;
		break;
	}

	//individuel ciffer editing
	if((FuncState>=60) && (FuncState<70))
	{
		FuncState+=10;
		switch(FuncState)
		{
		case 77:
			longVal1+=1;
			break;
		case 76:
			longVal1+=15;
			break;
		case 75:
			longVal1+=149;
			break;
		case 74:
			longVal1+=1485;
			break;
		case 73:
			longVal1+=14850;
			break;
		case 72:
			longVal1+=148500;
			break;
		case 71:
			longVal1+=1485000;
			break;
		case 70:
			longVal1+=14850000;
			break;
		}

		if(longVal1>74250000)
			longVal1=-74250000;
	}

	if((FuncState>=80) && (FuncState<90))
	{
		FuncState-=10;

		switch(FuncState)
		{
		case 77:
			longVal1-=1;
			break;
		case 76:
			longVal1-=15;
			break;
		case 75:
			longVal1-=149;
			break;
		case 74:
			longVal1-=1485;
			break;
		case 73:
			longVal1-=14850;
			break;
		case 72:
			longVal1-=148500;
			break;
		case 71:
			longVal1-=1485000;
			break;
		case 70:
			longVal1-=14850000;
			break;
		}

		if(longVal1<-74250000)
			longVal1=74250000;
	}

	//execute handling
	if (FuncSave)
	{
		//individual ciffer editing On/Off
		if((FuncState == 10) || (FuncState == 40))
			FuncState=70;
		else if((FuncState>=70) && (FuncState<80))
			FuncState=40;

		//if save
		if(FuncState==41)
		{
			//save modified variable
			LTCConfig[LTCNo].Offset=longVal1;

			//send data to GPS module
			if(LTCNo==0)
			{
				IIC1Write2(d2ram_addr, 0x07, (unsigned char)(longVal1));
				IIC1Write2(d2ram_addr, 0x08, (unsigned char)(longVal1>>8));
				IIC1Write2(d2ram_addr, 0x09, (unsigned char)(longVal1>>16));
				IIC1Write2(d2ram_addr, 0x0A, (unsigned char)(longVal1>>24));
			}
			else
			{
				IIC1Write2(d2ram_addr, 0x1B, (unsigned char)(longVal1));
				IIC1Write2(d2ram_addr, 0x1C, (unsigned char)(longVal1>>8));
				IIC1Write2(d2ram_addr, 0x1D, (unsigned char)(longVal1>>16));
				IIC1Write2(d2ram_addr, 0x1E, (unsigned char)(longVal1>>24));
			}

			//return to top menu
			FuncState=11;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}

		FuncSave = FALSE;
	}

	if (FuncEscape)
	{
		//If escape from menu
		if((FuncState==91) || (FuncState==32))
		{
			FuncState=11;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}
		FuncEscape=FALSE;
	}

	//Write "OFFSET: <XXXXXXXXXX.X>ns       <OK><ESC>"

	WriteCodeLN2(0, OffsetTxt);
	WriteChar2(':');

	ClearToPosLN2(9);

	//Convert 148.5 MHz clockcycles to ns
	offset=longVal1/0.1485;
	sprintf(LTCOffsetStr, "%012.1f", offset);

	//if positive, add sign
	if(longVal1>=0)
		LTCOffsetStr[0]='+';

	WriteTxt2(LTCOffsetStr);
	WriteChar2(' ');
	WriteCode2(LTCNSTxt);

	if(FuncState>39)
	{
		ClearToPosLN2(31);
		WriteCode2(OKTxt);
	}

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);

	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
	case 40:
		WriteItemArrows(9, 12);
		WriteArrowsField(L_+U_+R_+D_);
		break;

	case 41:
		WriteItemArrows(31, Cstrlen(OKTxt));
		WriteArrowsField(L_+R_+E_);
		break;

	case 11:
	case 42:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+U_+R_);
		break;

	default:
		WriteItemArrows(9, 12);
		WriteArrowsField(L_+U_+R_+D_);
		break;
	}

	if((FuncState>=70) && (FuncState<80))
		CharBlinkLN2((10+(FuncState-70)), ON);
	else
		CharBlinkLN2(0, OFF);

	FuncTriggers = 0;                  // Reset the button-selection
}

/***************************************************************************/
/* LTCFormatSet                                 MENUB.C            */
/*                                                               */
/* Author:    Jens K Hansen, DEV, 26/9/2007                              */
/*                                                             */
/*  Function:  Select LTC system                       */
/*  Remarks:    LTC MENU function.                                   */
/*            Cursor position on line 2 is updated.                    */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void LTCFormatSet()
{
	switch(FuncState)
	{
		// Wrap to the right - cases
	case 13:
		FuncState=10;
		break;
	case 54:
		FuncState=50;
		break;

		// wrap to the left
	case 19:
		FuncState=12;
		break;
	case 59:
		FuncState=53;
		break;

		// down pressed on <esc>
	case 22:
		FuncState=12;
		break;
		// down pressed on <ok>
	case 62:
		FuncState=52;
		break;
		// down pressed on <esc>
	case 63:
		FuncState=53;
		break;

		// Up pressed on <esc>
	case 43:
	case 92:
		FuncEscape=TRUE;
		break;
		// Up pressed on <OK>
	case 42:
		FuncState=52;
		break;

		// If up pressed on <FORMAT>
	case 90:
	case 40:
		if ((UcharVal1--)==0)
			UcharVal1=4;

		FuncState=50;
		break;

		// If down pressed on <FORMAT>
	case 20:
	case 60:
		if ((UcharVal1++)==4)
			UcharVal1=0;

		FuncState=50;
		break;

		// If up pressed on <SYNCMODE>
	case 91:
		FuncState=11;
		break;
	case 41:
		FuncState=51;
		break;

		// If down pressed on <SYNCMODE>
	case 21:
	case 61:
		WriteCodeLN1(0, ThirdmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCArrTxt[LTCNo]);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCModeItem].ItemTxt);
		WriteCode1(CoEditTxt);
		ClearRestLN1();

		//set temporary dropframe mode to current LTC dropframe mode
		UcharVal=LTCConfig[LTCNo].DFMode;
		UcharVal1=LTCConfig[LTCNo].DFHour;
		UcharVal2=LTCConfig[LTCNo].DFMin;

		FuncState = 10;
		Ptr = &LTC_menu[LTCModeItem];
		Ptr -> DispFunct();
		return;
	}

	//if save
	if (FuncSave)
	{
		if(FuncState==52)
		{
			//save modified variable
			LTCConfig[LTCNo].Format=UcharVal1;

			//send data to GPS module
			if(LTCNo==0)
				IIC1Write2(d2ram_addr, 0x0B, UcharVal1);
			else
				IIC1Write2(d2ram_addr, 0x1F, UcharVal1);

			//return to top menu
			FuncState=12;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();                // Display previous menu
			return;
		}
		FuncSave = FALSE;
	}

	//if escape
	if (FuncEscape)
	{
		if(FuncState==92 || FuncState==43)
		{
			FuncState=12;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();                // Display previous menu
			return;
		}

		FuncEscape = FALSE;
	}

	//Write "FORMAT: <24.00  FPS><SYNCMODE> <OK><ESC>"
	WriteCodeLN2(0, LTCFormatTxt);
	WriteChar2(':');

	ClearToPosLN2(9);
	WriteCode2(LTCFormatArrTxt[UcharVal1]);

	ClearToPosLN2(21);
	WriteCode2(LTCSyncTxt);
	WriteCode2(LTCModeTxt);

	if(FuncState>39)
	{
		ClearToPosLN2(32);
		WriteCode2(OKTxt);
	}

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);

	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
	case 50:
		WriteItemArrows(9, 10);
		WriteArrowsField(L_+U_+R_+D_);
		break;

	case 11:
	case 51:
		WriteItemArrows(21, 8);
		WriteArrowsField(L_+R_+D_);
		break;

	case 52:
		WriteItemArrows(32, Cstrlen(OKTxt));
		WriteArrowsField(L_+E_+R_);
		break;

	default:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
}

/***************************************************************************/
/* LTCTimeSet                                 MENUB.C            */
/*                                                               */
/* Author:    Jens K Hansen, DEV, 26/9/2007                              */
/*                                                             */
/*  Function:  Select time submenues                     */
/*  Remarks:    LTC MENU function.                                   */
/*            Cursor position on line 2 is updated.                    */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void LTCTimeSet()
{
	switch(FuncState)
	{
		// Wrap to the right
	case 13:
		FuncState=10;
		break;

		// wrap to the left
	case 19:
		FuncState=12;
		break;

		//down pressed on <TIMEZONE>
	case 20:
		WriteCodeLN1(0, ThirdmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCArrTxt[LTCNo]);
		WriteChar1('/');
		WriteCode1(LTCTimezoneTxt);
		WriteCode1(CoEditTxt);

		ClearRestLN1();

		UcharVal1=LTCConfig[LTCNo].TimezoneHour+12;
		UcharVal=LTCConfig[LTCNo].TimezoneMin;

		FuncState = 10;
		Ptr = &LTC_menu[LTCTimezoneItem];
		Ptr -> DispFunct();
		return;

		//down pressed on <DAYLIGHT>
	case 21:
		WriteCodeLN1(0, ThirdmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCArrTxt[LTCNo]);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCDaylightItem].ItemTxt);
		WriteCode1(CoSelectTxt);

		ClearRestLN1();

		UcharVal=LTCConfig[LTCNo].DaylightMode;
		UcharVal1=LTCConfig[LTCNo].DaylightState;

		FuncState = 10;
		Ptr = &LTC_menu[LTCDaylightItem];
		Ptr -> DispFunct();
		return;

		//Up pressed on <TIMEZONE> or <DAYLIGHT>
	case 90:
		FuncState=10;
		break;
	case 91:
		FuncState=11;
		break;

		// Down pressed on <esc>
	case 22:
		FuncState=12;
		break;

		// Up pressed on <esc>
	case 92:
		FuncEscape=TRUE;
		break;
	}

	//If escape from menu
	if (FuncEscape)
	{
		if(FuncState==92)
		{
			FuncState=13;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}

		FuncEscape = FALSE;
	}

	//Write "2NDMNU:  TIMEZONE  DAYLIGHT       <ESC>"
	WriteCodeLN2(0, ThirdmnuHdrTxt);

	ClearToPosLN2(9);
	WriteCode2(LTCTimezoneTxt);

	ClearToPosLN2(19);
	WriteCode2(LTCDaylightTxt);

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);
	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(9, 8);
		WriteArrowsField(L_+R_+D_);
		break;
	case 11:
		WriteItemArrows(19, 8);
		WriteArrowsField(L_+R_+D_);
		break;

	default:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
}

/***************************************************************************/
/* LTCFormatSet                                 MENUB.C            */
/*                                                               */
/* Author:    Jens K Hansen, DEV, 26/9/2007                              */
/*                                                             */
/*  Function:  Select LTC Dropframe updatemode                 */
/*  Remarks:    LTC MENU function.                                   */
/*            Cursor position on line 2 is updated.                    */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void LTCModeSet()
{
	static char HourString[2];
	static char MinString[2];

	//sprintf(tbuff,"Uchar=%d",UcharVal);


	//  DebugStr(tbuff);


	if(TimeCodeUnit.Present)
		LTCNo=0;

	switch(FuncState)
	{
		// Wrap to the right cases
	case 14:
		FuncState=10;
		break;
	case 55:
		FuncState=50;
		break;

		// wrap to the left cases
	case 19:
		FuncState=13;
		break;
	case 59:
		FuncState=54;
		break;

		//Switch mode down
	case 20:
	case 60:
		if((UcharVal++)==2)
			UcharVal=0;

		FuncState=50;
		break;

		//Switch mode up
	case 40:
	case 90:
		if((UcharVal--)==0)
			UcharVal=2;

		FuncState=50;
		break;

		//down pressed on hours
	case 21:
	case 61:
		if((UcharVal1--)==0)
			UcharVal1=23;

		FuncState=51;
		break;

		//up pressed on hours
	case 41:
	case 91:
		if((UcharVal1++)==23)
			UcharVal1=0;

		FuncState=51;
		break;

		//if down pressed on mins
	case 22:
	case 62:
		UcharVal2-=1;
		if(UcharVal2>=60)
			UcharVal2=50;

		FuncState=52;
		break;

		//if up pressed on mins
	case 42:
	case 92:
		UcharVal2+=1;
		if(UcharVal2==60)
			UcharVal2=0;

		FuncState=52;
		break;

		//down pressed on <ok>
	case 23:
		FuncState=13;
		break;
	case 63:
		FuncState=53;
		break;
	case 64:
		FuncState=54;
		break;

		//Up pressed on <esc>
	case 44:
	case 93:
		FuncEscape=TRUE;
		break;
	case 43:
		FuncState=53;
		break;
	}

	if (FuncEscape)
	{
		if(FuncState==93 || FuncState==44)
		{
			if(TimeCodeUnit.Present)
			{
				//WriteCode1(LTCTxt);
				FuncState=12;
				UcharVal1=LTCConfig[0].Format;
				LTCMenuShow();
				Ptr = &LTC8620_menu[LTCSelectItem];	// Set pointer to previous menu
				Ptr-> DispFunct();			   // Display previous menu
				return;

			}
			else
			{
				WriteCodeLN1(0, SecondmnuHdrTxt);
				WriteCode1(SlashDotHdrTxt);

				WriteCode1(LTCArrTxt[LTCNo]);
				WriteChar1('/');
				WriteCode1(LTC_menu[LTCFormatItem].ItemTxt);
				WriteCode1(CoSelectTxt);

				ClearRestLN1();

				FuncState = 11;
				//Set temp variable to current LTC format
				UcharVal1=LTCConfig[LTCNo].Format;
				Ptr =&LTC_menu[LTCFormatItem];

				Ptr -> DispFunct();
				return;
			}
		}

		FuncEscape=FALSE;
	}

	if(FuncSave)
	{
		if(FuncState==53)
		{
			LTCConfig[LTCNo].DFMode=UcharVal;
			LTCConfig[LTCNo].DFHour=UcharVal1;
			LTCConfig[LTCNo].DFMin=UcharVal2;

			//send data to GPS module
			if(LTCNo==0)
			{
				if(TimeCodeUnit.Present){
				if(GenlockConfig[GPSGenlock].System==4)					
				IIC1Write2(d2ram_addr, 0x0B, 1);

				if(GenlockConfig[GPSGenlock].System==5)					
				IIC1Write2(d2ram_addr, 0x0B, 3);
					}
				IIC1Write2(d2ram_addr, 0x0C, UcharVal);
				IIC1Write2(d2ram_addr, 0x0D, UcharVal1);
				IIC1Write2(d2ram_addr, 0x0E, UcharVal2);
				if(!IIC1ErrorCode){

				sprintf(tbuff,"Uchar=%d Uchar=%d Uchar=%d",UcharVal,UcharVal1,UcharVal2);
				
				
				DebugStr(tbuff);
					}
				
			}
			else
			{
				IIC1Write2(d2ram_addr, 0x20, UcharVal);
				IIC1Write2(d2ram_addr, 0x21, UcharVal1);
				IIC1Write2(d2ram_addr, 0x22, UcharVal2);
			}

			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			if(TimeCodeUnit.Present)
				WriteCode1(LTCTxt);
			else
				WriteCode1(LTCArrTxt[LTCNo]);
			WriteChar1('/');
			WriteCode1(LTC_menu[LTCFormatItem].ItemTxt);
			WriteCode1(CoSelectTxt);

			ClearRestLN1();

			FuncState = 11;
			//Set temp variable to current LTC format
			UcharVal1=LTCConfig[LTCNo].Format;


			if(TimeCodeUnit.Present)
			{
				FuncState=12;
				LTCMenuShow();
				Ptr = &LTC8620_menu[LTCSelectItem];   // Set pointer to previous menu
			}
			else
				Ptr = &LTC_menu[LTCFormatItem];



			Ptr -> DispFunct();
			return;
		}

		FuncSave = FALSE;
	}

	//Write "SYNCMODE:<MODE> TIME:<HH:MM>   <OK><ESC>"
	WriteCodeLN2(0,LTCSyncTxt);
	WriteCode2(LTCModeTxt);
	WriteChar2(':');

	ClearToPosLN2(10);
	WriteCode2(LTCModeArrTxt[UcharVal]);

	ClearToPosLN2(16);
	WriteCode2(TimeTxt);
	WriteChar2(':');

	sprintf(HourString, "%02i", (int)UcharVal1);
	sprintf(MinString, "%02i", (int)UcharVal2);

	ClearToPosLN2(22);
	WriteTxt2(HourString);
	WriteCharLN2(24, ':');
	WriteTxt2(MinString);

	if(FuncState>39)
	{
		ClearToPosLN2(31);
		WriteCode2(OKTxt);
	}

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);
	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
	case 50:
		WriteItemArrows(10, 4);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 11:
	case 51:
		WriteItemArrows(22, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 12:
	case 52:
		WriteItemArrows(25, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 43:
	case 53:
		WriteItemArrows(31, 2);
		WriteArrowsField(L_+R_+E_);
		break;
	default:
		WriteItemArrows(36, 3);
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
}

void LTCTimezoneSet()
{
	static char String[3];

	if(TimeCodeUnit.Present)
		LTCNo=0;



	switch(FuncState)
	{
		// Wrap to the right
	case 13:
		FuncState=10;
		break;
	case 54:
		FuncState=50;
		break;
		// wrap to the left
	case 19:
		FuncState=12;
		break;
	case 59:
		FuncState=53;
		break;

		//down pressed on hours
	case 20:
	case 60:
		if((UcharVal1--)==0)
			UcharVal1=24;

		if(UcharVal1==0)
			UcharVal=0;

		FuncState=50;
		break;
		//up pressed on hours
	case 40:
	case 90:
		if((UcharVal1++)==24)
			UcharVal1=0;

		if((UcharVal1>12) && (UcharVal==226)) //hvis wrap fra - til + (226 = -30)
			UcharVal=30;

		if(UcharVal1==24)
			UcharVal=0;

		FuncState=50;
		break;


		//down pressed on mins
	case 61:
	case 21:
		if (UcharVal==0)
			UcharVal=30;
		else
			UcharVal=0;

		if(UcharVal1==0 || UcharVal1==24)
			UcharVal=0;

		if ((UcharVal1==12) && (UcharVal==30))
			UcharVal = 226; //(226= -30 when signed)

		FuncState=51;
		break;

		//up pressed on mins
	case 41:
	case 91:
		if (UcharVal==0)
			UcharVal=30;
		else
			UcharVal=0;

		if(UcharVal1==0 || UcharVal1==24)
			UcharVal=0;

		FuncState=51;
		break;

		//down pressed on <esc>
	case 22:
		FuncState=12;
		break;
	case 63:
		FuncState=53;
		break;
		// down pressed on <ok>
	case 62:
		FuncState = 52;
		break;
		//Up pressed on <esc>
	case 43:
	case 92:
		FuncEscape=TRUE;
		break;
		//Up pressed on <OK>
	case 42:
		FuncState=52;
		break;
	}

	//save
	if (FuncSave)
	{
		if(FuncState==52)
		{
			//save variables in LTC struct
			LTCConfig[LTCNo].TimezoneHour=(int)(UcharVal1-12);
			LTCConfig[LTCNo].TimezoneMin=UcharVal;

			//send data to GPS module
			if(LTCNo==0)
			{
				IIC1Write2(d2ram_addr, 0x0F, UcharVal1-12);
				IIC1Write2(d2ram_addr, 0x10, UcharVal);
			}
			else
			{
				IIC1Write2(d2ram_addr, 0x23, UcharVal1-12);
				IIC1Write2(d2ram_addr, 0x24, UcharVal);
			}

			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			if(TimeCodeUnit.Present){
				
				WriteCode1(LTCTxt);
				}
			else
				WriteCode1(LTCArrTxt[LTCNo]);
			WriteChar1('/');
			WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
			WriteCode1(CoSelectTxt);

			ClearRestLN1();

			FuncState = 10;



			Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCTimeItem] : &LTC_menu[LTCTimeItem];
			Ptr -> DispFunct();
			return;
		}

		//escape
		FuncSave = FALSE;
	}

	if (FuncEscape)
	{
		if((FuncState==92) || (FuncState==43))
		{
			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);

			if(TimeCodeUnit.Present)
				WriteCode1(LTCTxt);
			else
				WriteCode1(LTCArrTxt[LTCNo]);
			WriteChar1('/');
			WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
			WriteCode1(CoSelectTxt);

			ClearRestLN1();

			FuncState = 10;
			Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCTimeItem] : &LTC_menu[LTCTimeItem];

			Ptr -> DispFunct();
			return;

		}

		FuncEscape = FALSE;
	}

	//Write "TIMEZONE: <SHH:MM>              <OK><ESC>" (S=sign)

	WriteCodeLN2(0, LTCTimezoneTxt);
	WriteChar2(':');

	sprintf(String, "%+03i", (int)UcharVal1-12);

	//handle -30
	if((UcharVal1==12) && (UcharVal==226))
		String[0]='-';

	ClearToPosLN2(11);
	WriteTxt2(String);

	if((UcharVal1<13) && (UcharVal==226))
		sprintf(String, "%02u", (int)30);
	else
		sprintf(String, "%02u", (int)UcharVal);

	WriteCharLN2(14, ':');
	WriteTxt2(String);

	if(FuncState>39)
	{
		ClearToPosLN2(31);
		WriteCode2(OKTxt);
	}

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);
	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
	case 50:
		WriteItemArrows(11, 3);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 11:
	case 51:
		WriteItemArrows(15, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 52:
		WriteItemArrows(31, 2);
		WriteArrowsField(L_+E_+R_);
		break;
	default:
		WriteItemArrows(36, 3);
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
}

void LTCDaylightSet()
{

	if(TimeCodeUnit.Present)
		LTCNo=0;


	switch(FuncState)
	{
		// Wrap to the right
	case 15:
		FuncState=10;
		break;
	case 56:
		FuncState=50;
		break;
		// wrap to the left
	case 19:
		FuncState=14;
		break;
	case 59:
		FuncState=55;
		break;

		//down pressed on mode
	case 20:
	case 60:
		if ((UcharVal--)==0)
			UcharVal=2;

		FuncState=50;
		break;
		//up pressed on modes
	case 40:
	case 90:
		if ((UcharVal++)==2)
			UcharVal=0;

		FuncState=50;
		break;

		//switch daylight saving on/off
	case 21:
	case 41:
	case 61:
	case 91:
		if(UcharVal1==0)
			UcharVal1=1;
		else
			UcharVal1=0;

		FuncState=51;
		break;

		//down pressed on start
	case 22:
	case 62:
		WriteCodeLN1(0, ForthmnuHdrTxt);
		WriteCodeLN1(7, SlashDotHdrTxt);
		if(TimeCodeUnit.Present)
			WriteCode1(LTCTxt);
		else
			WriteCode1(LTCArrTxt[LTCNo]);
		WriteChar1('/');
		WriteCode1(LTCDaylightTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCStartItem].ItemTxt);
		WriteCode1(CoEditTxt);

		ClearRestLN1();

		UcharVal=LTCConfig[LTCNo].DaylightStartMonth;
		UcharVal1=LTCConfig[LTCNo].DaylightStartDay;
		UcharVal2=LTCConfig[LTCNo].DaylightStartHour;

		//no day 0 in a month
		if(UcharVal1==0)
			UcharVal1=1;

		FuncState = 11;

		Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCStartItem] : &LTC_menu[LTCStartItem];


		Ptr -> DispFunct();
		return;

		//up pressed on start
	case 42:
		FuncState=52;
		break;
	case 92:
		FuncState=12;
		break;

		//down pressed on end
	case 23:
	case 63:
		WriteCodeLN1(0, ForthmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		if(TimeCodeUnit.Present)
			WriteCode1(LTCTxt);
		else
			WriteCode1(LTCArrTxt[LTCNo]);
		WriteChar1('/');
		WriteCode1(LTCDaylightTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCEndItem].ItemTxt);
		WriteCode1(CoEditTxt);

		ClearRestLN1();

		//save settings in unit
		UcharVal=LTCConfig[LTCNo].DaylightEndMonth;
		UcharVal1=LTCConfig[LTCNo].DaylightEndDay;
		UcharVal2=LTCConfig[LTCNo].DaylightEndHour;

		//no day 0 in a month
		if(UcharVal1==0)
			UcharVal1=1;

		FuncState = 11;
		Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCEndItem] : &LTC_menu[LTCEndItem];

		Ptr -> DispFunct();
		return;
	case 43:
		FuncState=53;
		break;
	case 93:
		FuncState=13;
		break;

		//down pressed on <esc>
	case 24:
		FuncState=14;
		break;
	case 65:
		FuncState=55;
		break;

		//down pressed on <ok>
	case 64:
		FuncState = 54;
		break;
		//Up pressed on <esc>
	case 45:
	case 94:
		FuncEscape=TRUE;
		break;
		//Up pressed on <OK>
	case 44:

		FuncState=54;
		break;
	}

	//if escape
	if (FuncEscape)
	{
		if((FuncState==94) || (FuncState==45))
		{
			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(base_menu[BaseNdx].ItemTxt);
			WriteChar1('/');
			WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
			WriteCode1(CoSelectTxt);

			ClearRestLN1();

			FuncState = 11;

			Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCTimeItem] : &LTC_menu[LTCTimeItem];


			Ptr -> DispFunct();
			return;
		}

		FuncEscape = FALSE;
	}

	//if save
	if (FuncSave)
	{
		if(FuncState==54)
		{
			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(base_menu[BaseNdx].ItemTxt);
			WriteChar1('/');
			WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
			WriteCode1(CoSelectTxt);

			ClearRestLN1();

			LTCConfig[LTCNo].DaylightMode=UcharVal;
			LTCConfig[LTCNo].DaylightState=UcharVal1;

			//send data to GPS module
			if(LTCNo==0)
				IIC1Write2(d2ram_addr, 0x11, UcharVal);
			else
				IIC1Write2(d2ram_addr, 0x25, UcharVal);

			//send daylight savings mode
			UcharVal1=0;

			if(LTCConfig[0].DaylightState==0)
				UcharVal1|=2;
			else
				UcharVal1|=1;

			if(LTCConfig[1].DaylightState==0)
				UcharVal1|=32;
			else
				UcharVal1|=16;

			IIC1Write2(d2ram_addr, 0x38, UcharVal1);

			FuncState = 11;

			Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCTimeItem] : &LTC_menu[LTCTimeItem];


			Ptr -> DispFunct();
			return;
		}

		FuncSave = FALSE;
	}

	//Write "MODE:<MODE>DST:< ON><STARTXEND><OK><ESC>"
	WriteCodeLN2(0, LTCModeTxt);
	WriteChar2(':');

	ClearToPosLN2(6);
	WriteCode2(LTCModeArrTxt[UcharVal]);

	ClearToPosLN2(11);
	WriteCode2(LTCDSTTxt);

	WriteChar2(' ');

	if(UcharVal1==0)
		WriteCode2(OffTxt);
	else
		WriteCode2(OnTxt);

	ClearToPosLN2(21);
	WriteCode2(LTCStartTxt);


	ClearToPosLN2(27);
	WriteCode2(LTCEndTxt);

	if(FuncState>39)
	{
		ClearToPosLN2(32);
		WriteCode2(OKTxt);
	}

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);
	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
	case 50:
		WriteItemArrows(6, 4);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 11:
	case 51:
		WriteItemArrows(16, 3);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 12:
	case 52:
		WriteItemArrows(21, 5);
		WriteArrowsField(L_+R_+D_);
		break;
	case 13:
	case 53:
		WriteItemArrows(27, 3);
		WriteArrowsField(L_+R_+D_);
		break;
	case 54:
		WriteItemArrows(32, 2);
		WriteArrowsField(L_+E_+R_);
		break;
	default:
		WriteItemArrows(36, 3);
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
}

void LTCStartSet()
{
	static char String[2];
	switch(FuncState)
	{
		// Wrap to the right
	case 14:
		FuncState=10;
		break;
	case 55:
		FuncState=50;
		break;
		// wrap to the left
	case 19:
		FuncState=13;
		break;
	case 59:
		FuncState=54;
		break;
		//Switch month down
	case 20:
	case 60:
		if ( (UcharVal--)==0)
			UcharVal=11;

		//if the selected day, is not included in the month
		if (UcharVal1>month_days[UcharVal])
			UcharVal1=month_days[UcharVal];

		FuncState=50;
		break;
		//Switch month up
	case 40:
	case 90:
		if ( (UcharVal++)==11)
			UcharVal=0;

		//if the selected day, is not included in the month
		if (UcharVal1>month_days[UcharVal])
			UcharVal1=month_days[UcharVal];

		FuncState=50;
		break;
		//down pressed on day
	case 21:
	case 61:
		if ( (UcharVal1--)==1)
			UcharVal1=month_days[UcharVal];

		FuncState=51;
		break;
		//up pressed on day
	case 41:
	case 91:
		if ( (UcharVal1++)==month_days[UcharVal])
			UcharVal1=1;

		FuncState=51;
		break;
		//if down pressed on hour
	case 22:
	case 62:
		if((UcharVal2--)==0)
			UcharVal2=23;

		FuncState=52;
		break;
		//if up pressed on hour
	case 42:
	case 92:
		if((UcharVal2++)==23)
			UcharVal2=0;

		FuncState=52;
		break;
		//down pressed on <esc>
	case 23:
		FuncState=13;
		break;
	case 64:
		FuncState=54;
		break;
		// down pressen on <ok>
	case 63:
		FuncState = 53;
		break;
		//Up pressed on <ok>
	case 44:
	case 93:
		FuncEscape=TRUE;
		break;
	case 43:
		FuncState=53;
		break;
	}

	if (FuncEscape)
	{
		if((FuncState==93) || (FuncState==44))
		{
			WriteCodeLN1(0, ThirdmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			if(TimeCodeUnit.Present)
				WriteCode1(LTCTxt);
			else
				WriteCode1(LTCArrTxt[LTCNo]);

			WriteChar1('/');
			WriteCode1(LTC_menu[LTCDaylightItem].ItemTxt);
			WriteCode1(CoSelectTxt);

			ClearRestLN1();

			UcharVal=LTCConfig[LTCNo].DaylightMode;
			UcharVal1=LTCConfig[LTCNo].DaylightState;

			FuncState = 12;

			Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCDaylightItem] : &LTC_menu[LTCDaylightItem];


			Ptr -> DispFunct();
			return;
		}

		FuncEscape = FALSE;
	}

	if (FuncSave)
	{
		if(FuncState==53)
		{
			WriteCodeLN1(0, ThirdmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			if(TimeCodeUnit.Present)
				WriteCode1(LTCTxt);
			else
				WriteCode1(LTCArrTxt[LTCNo]);

			WriteChar1('/');
			WriteCode1(LTC_menu[LTCDaylightItem].ItemTxt);
			WriteCode1(CoSelectTxt);

			ClearRestLN1();

			//save settings in unit
			LTCConfig[LTCNo].DaylightStartMonth=UcharVal;
			LTCConfig[LTCNo].DaylightStartDay=UcharVal1;
			LTCConfig[LTCNo].DaylightStartHour=UcharVal2;

			//send data to GPS module
			if(LTCNo==0)
			{
				IIC1Write2(d2ram_addr, 0x12, UcharVal);
				IIC1Write2(d2ram_addr, 0x13, UcharVal1);
				IIC1Write2(d2ram_addr, 0x14, UcharVal2);
			}
			else
			{
				IIC1Write2(d2ram_addr, 0x26, UcharVal);
				IIC1Write2(d2ram_addr, 0x27, UcharVal1);
				IIC1Write2(d2ram_addr, 0x28, UcharVal2);
			}

			//set new menu variables
			UcharVal=LTCConfig[LTCNo].DaylightMode;
			UcharVal1=LTCConfig[LTCNo].DaylightState;

			FuncState = 12;
			Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCDaylightItem] : &LTC_menu[LTCDaylightItem];
			Ptr -> DispFunct();
			return;
		}

		FuncSave = FALSE;
	}

	//Write "START DATE: MM/DD HOUR: HH     <OK><ESC>"
	WriteCodeLN2(0, LTCStartTxt);
	WriteChar2(' ');
	WriteCode2(LTCDateTxt);
	WriteCharLN2(10, ':');

	ClearToPosLN2(12);

	sprintf(String, "%02i", (int)UcharVal+1);
	WriteTxt2(String);

	WriteChar2('/');

	sprintf(String, "%02i", (int)UcharVal1);
	WriteTxt2(String);

	ClearToPosLN2(19);
	WriteCode2(LTCHourTxt);
	WriteChar2(':');
	ClearToPosLN2(25);

	sprintf(String, "%02i", (int)UcharVal2);
	WriteTxt2(String);

	if(FuncState>39)
	{
		ClearToPosLN2(31);
		WriteCode2(OKTxt);
	}

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);
	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
	case 50:
		WriteItemArrows(12, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 11:
	case 51:
		WriteItemArrows(15, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 12:
	case 52:
		WriteItemArrows(25, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 53:
		WriteItemArrows(31, 2);
		WriteArrowsField(L_+R_+E_);
		break;
	default:
		WriteItemArrows(36, 3);
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
}

void LTCEndSet()
{
	static char String[2];

	switch(FuncState)
	{
		// Wrap to the right
	case 14:
		FuncState=10;
		break;
	case 55:
		FuncState=50;
		break;
		// wrap to the left
	case 19:
		FuncState=13;
		break;
	case 59:
		FuncState=54;
		break;
		//Switch month down
	case 20:
	case 60:
		if ( (UcharVal--)==0)
			UcharVal=11;

		//if the selected day, is not included in the month
		if (UcharVal1>month_days[UcharVal])
			UcharVal1=month_days[UcharVal];

		FuncState=50;
		break;
		//Switch month up
	case 40:
	case 90:
		if ( (UcharVal++)==11)
			UcharVal=0;

		//if the selected day, is not included in the month
		if (UcharVal1>month_days[UcharVal])
			UcharVal1=month_days[UcharVal];

		FuncState=50;
		break;
		//down pressed on day
	case 21:
	case 61:
		if ( (UcharVal1--)==1)
			UcharVal1=month_days[UcharVal];

		FuncState=51;
		break;
		//up pressed on day
	case 41:
	case 91:
		if ( (UcharVal1++)==month_days[UcharVal])
			UcharVal1=1;

		FuncState=51;
		break;
		//if down pressed on hour
	case 22:
	case 62:
		if((UcharVal2--)==0)
			UcharVal2=23;

		FuncState=52;
		break;
		//if up pressed on hour
	case 42:
	case 92:
		if((UcharVal2++)==23)
			UcharVal2=0;

		FuncState=52;
		break;
		//down pressed on <esc>
	case 23:
		FuncState=13;
		break;
	case 64:
		FuncState=54;
		break;
		// down pressen on <ok>
	case 63:
		FuncState = 53;
		break;
		//Up pressed on <ok>
	case 44:
	case 93:
		FuncEscape=TRUE;
		break;
	case 43:
		FuncState=53;
		break;
	}

	if (FuncEscape)
	{
		if((FuncState==93) || (FuncState==44))
		{
			WriteCodeLN1(0, ThirdmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			if(TimeCodeUnit.Present)
				WriteCode1(LTCTxt);
			else
				WriteCode1(LTCArrTxt[LTCNo]);

			WriteChar1('/');
			WriteCode1(LTC_menu[LTCDaylightItem].ItemTxt);
			WriteCode1(CoSelectTxt);
			ClearRestLN1();

			//recall settings for daylight mode
			UcharVal=LTCConfig[LTCNo].DaylightMode;
			UcharVal1=LTCConfig[LTCNo].DaylightState;

			FuncState = 13;
			Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCDaylightItem] : &LTC_menu[LTCDaylightItem];
			Ptr -> DispFunct();
			return;
		}

		FuncEscape = FALSE;
	}

	if (FuncSave)
	{
		if(FuncState==53)
		{
			WriteCodeLN1(0, ThirdmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			if(TimeCodeUnit.Present)
				WriteCode1(LTCTxt);
			else
				WriteCode1(LTCArrTxt[LTCNo]);

			WriteChar1('/');
			WriteCode1(LTC_menu[LTCDaylightItem].ItemTxt);
			WriteCode1(CoSelectTxt);
			ClearRestLN1();

			//Save variables in struct
			LTCConfig[LTCNo].DaylightEndMonth=UcharVal;
			LTCConfig[LTCNo].DaylightEndDay=UcharVal1;
			LTCConfig[LTCNo].DaylightEndHour=UcharVal2;

			//send data to GPS module
			if(LTCNo==0)
			{
				IIC1Write2(d2ram_addr, 0x15, UcharVal);
				IIC1Write2(d2ram_addr, 0x16, UcharVal1);
				IIC1Write2(d2ram_addr, 0x17, UcharVal2);
			}
			else
			{
				IIC1Write2(d2ram_addr, 0x29, UcharVal);
				IIC1Write2(d2ram_addr, 0x2A, UcharVal1);
				IIC1Write2(d2ram_addr, 0x2B, UcharVal2);
			}

			//recall settings for daylight mode
			UcharVal=LTCConfig[LTCNo].DaylightMode;
			UcharVal1=LTCConfig[LTCNo].DaylightState;

			FuncState = 13;
			Ptr = TimeCodeUnit.Present? &LTC8620_menu[LTCDaylightItem] : &LTC_menu[LTCDaylightItem];
			Ptr -> DispFunct();
			return;
		}

		FuncSave = FALSE;
	}

	//Write "END DATE: MM/DD   HOUR: HH     <OK><ESC>"
	WriteCodeLN2(0, LTCEndTxt);
	WriteChar2(' ');
	WriteCode2(LTCDateTxt);
	WriteCharLN2(8, ':');

	ClearToPosLN2(10);

	sprintf(String, "%02i", (int)UcharVal+1);
	WriteTxt2(String);

	WriteChar2('/');

	sprintf(String, "%02i", (int)UcharVal1);
	WriteTxt2(String);

	ClearToPosLN2(19);
	WriteCode2(LTCHourTxt);
	WriteChar2(':');
	ClearToPosLN2(25);

	sprintf(String, "%02i", (int)UcharVal2);
	WriteTxt2(String);

	if(FuncState>39)
	{
		ClearToPosLN2(31);
		WriteCode2(OKTxt);
	}

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);
	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
	case 50:
		WriteItemArrows(10, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 11:
	case 51:
		WriteItemArrows(13, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 12:
	case 52:
		WriteItemArrows(25, 2);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	case 53:
		WriteItemArrows(31, 2);
		WriteArrowsField(L_+R_+E_);
		break;
	default:
		WriteItemArrows(36, 3);
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
	FuncSave=FALSE;
	FuncEscape=FALSE;
}


void LTCDaylightConfirm()
{
	switch(FuncState)
	{
		// Wrap to the right
	case 12:
		FuncState=10;
		break;
		// wrap to the left
	case 19:
		FuncState=11;
		break;
		//Switch down
	case 20:
		FuncState=10;
		break;
	case 21:
		FuncState=11;
		break;
		//Switch up
	case 90:
		FuncState=10;
		break;
	case 91:
		FuncState=11;
		break;
	}

	if (FuncSave)
	{
		//If save <OK>, return to prev menu, and send confirmation
		if(FuncState==10)
		{
			ClearRestLN1();

			//send data to GPS module
			if(LTC_request&3)     //If LTC A requested
				IIC1Write2(d2ram_addr, 50, 1);

			else if (LTC_request&48)  //If LTC B requested
				IIC1Write2(d2ram_addr, 51, 1);

			//return to top menu
			FuncState=10;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}

		//If <ESC>, return to prev menu, and send not confirmed
		if(FuncState==11)
		{
			ClearRestLN1();

			//send data to GPS module
			if(LTC_request&3)     //If LTC A requested
				IIC1Write2(d2ram_addr, 50, 0);

			else if (LTC_request&48)  //If LTC B requested
				IIC1Write2(d2ram_addr, 51, 0);

			//return to top menu
			FuncState=10;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}

		FuncSave = FALSE;
	}

	//Write "CONFIRM DAYLIGHT XXX:  <OK>  <ESC>      "
	WriteCodeLN2(0, ConfirmTxt);
	WriteChar2(' ');
	WriteCodeLN2(0, LTCDaylightTxt);
	WriteChar2(' ');

	if(LTC_request&1)
		WriteCode2(OnTxt);
	else if(LTC_request&2)
		WriteCode2(OffTxt);
	else if(LTC_request&16)
		WriteCode2(OnTxt);
	else if(LTC_request&32)
		WriteCode2(OffTxt);

	WriteChar2(':');

	ClearToPosLN2(24);
	WriteCode2(OKTxt);

	ClearToPosLN2(30);
	WriteCode2(ESCTxt);

	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(24, 2);
		WriteArrowsField(L_+R_+E_);
		break;
	case 11:
		WriteItemArrows(30, 3);
		WriteArrowsField(L_+R_+E_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
	FuncSave=FALSE;
	FuncEscape=FALSE;
}

void LTCSyncConfirm()
{
	switch(FuncState)
	{
		// Wrap to the right
	case 12:
		FuncState=10;
		break;
		// wrap to the left
	case 19:
		FuncState=11;
		break;
		//Switch down
	case 20:
		FuncState=10;
		break;
	case 21:
		FuncState=11;
		break;
		//Switch up
	case 90:
		FuncState=10;
		break;
	case 91:
		FuncState=11;
		break;
	}

	if (FuncSave || FuncEscape)
	{
		//If save <OK>, return to prev menu, and send confirmation
		if(FuncState==10)
		{
			//if entered from confirm sync
			if(LTC_sync_request&34)
			{
				if (LTCNo==0)
					IIC1Write2(d2ram_addr, 54, 1); //Send '1' to GPS data register 54 (confirm syncrequest) LTC a
				//if request for sync on LTC B
				else
					IIC1Write2(d2ram_addr, 55, 1); //Send '1' to GPS data register 54 (confirm syncrequest) LTC b
			}

			//else, manual sync
			else
			{
				//send data to GPS module
				if(LTCNo==0)      //If LTC A sync
					IIC1Write2(d2ram_addr, 52, 1);

				else          //If LTC B sync
					IIC1Write2(d2ram_addr, 53, 1);
			}

			//return to top menu
			FuncState=10;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}

		//If <ESC>, return to prev menu, and send not confirmed
		if(FuncState==11)
		{
			//if request for sync on LTC A
			if (LTCNo==0)
				IIC1Write2(d2ram_addr, 54, 0);  //Send '0' to GPS data register 54 (decline syncrequest) LTC a

			//if request for sync on LTC B
			else
				IIC1Write2(d2ram_addr, 55, 0);  //Send '0' to GPS data register 54 (decline syncrequest) LTC b

			//return to top menu
			FuncState=10;
			LTCMenuShow();
			Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}

		FuncSave = FALSE;
	}

	//Write "SYNC LTC X:  <OK>  <ESC>               "
	WriteCodeLN2(0, LTCSyncTxt);
	WriteChar2(' ');
	WriteCodeLN2(0, LTCArrTxt[LTCNo]);
	WriteChar2(':');

	ClearToPosLN2(31);
	WriteCode2(OKTxt);

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);

	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(31, 2);
		WriteArrowsField(L_+R_+E_);
		break;
	case 11:
		WriteItemArrows(36, 3);
		WriteArrowsField(L_+R_+E_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
	FuncSave=FALSE;
	FuncEscape=FALSE;
}

//menu with no actual function, beside waiting for user to read message, without disturbing display
void LTCMessageMenu()
{
	if((FuncState!=10) || FuncSave || FuncEscape)
	{
		FuncState = 10;
		LTCMenuShow();
		Ptr = &LTC_menu[LTCSelectItem];   // Set pointer to previous menu
		Ptr-> DispFunct();               // Display previous menu
	}
}












// NTP module addresses
// 0: IP address
// 1: Subnet address
// 2: Gateway address
// 3: DNS address
// 4: DHCP enable/disable address
// 5: Telnet address
// 6: Reset password address
// 7: KU Address

void NTPMenuShow()
{
	WriteCodeLN1(0, SubmnuHdrTxt);
	WriteCode1(NetworkTxt);
	WriteCode1(CoSelectTxt);

	ClearRestLN1();

	//safety FuncState select, if not valid state
	if((FuncState>12) || (FuncState<10))
		FuncState=10;

	FuncTriggers = 0;
	FuncSave = FuncEscape = FALSE;

	SelectModeOn = TRUE;        // Set select-mode
}

/***************************************************************************/
/* NTPSelectMenu                                  MENUB.C            */
/*                                                               */
/*                                                             */
/*  Function:  Select submenues for NTP server              */
/*  Remarks:    NTP MENU function.                                   */
/*            Cursor position on line 2 is updated.                    */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void NTPSelectMenu()
{
	// Handling of selections
	switch(FuncState)
	{
		//wrap around to the right
	case 13:
		FuncState=10;
		break;

		//wrap arount to the left
	case 19:
		FuncState=12;
		break;

		// Up pressed on ESC
	case 92:
		FuncEscape=TRUE;
		break;

		//if Ethernet configuration selected:
	case 20:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(NetworkTxt);
		WriteChar1('/');
		WriteCode1(ETHConfigTxt);
		ClearRestLN1();

		FuncState = 10;
		Ptr = &NTP_MENU[NTPETHERConfigItem];
		Ptr -> DispFunct();
		return;

		//if configuration selected:
	case 21:
		// read values

		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(NetworkTxt);
		WriteChar1('/');
		WriteCode1(ConfigTxt);
		ClearRestLN1();

		FuncState = 10;
		Ptr = &NTP_MENU[NTPConfigItem];
		Ptr -> DispFunct();
		return;

		//If up/down pressed on illegal positions, reset
	case 90:
		FuncState=10;
		break;
	case 91:
		FuncState=11;
		break;
	case 22:
		FuncState=12;
		break;
	}

	if (FuncSave || FuncEscape)
	{
		if(FuncState==92)
		{
			FuncState = 10;
			Ptr = PrevPtr;                  // Set pointer to previous menu
			Ptr->DispFunct();               // Display previous menu
			return;
		}

		FuncSave = FALSE;
	}

	//Write "<ETHERNET> <CONFIG>                <ESC>"
	WriteCharLN2(0, ' ');
	WriteCode2(ETHConfigTxt);

	ClearToPosLN2(12);
	WriteCode2(ConfigTxt);

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);

	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(1, Cstrlen(ETHConfigTxt));
		WriteArrowsField(L_+R_+D_);
		break;

	case 11:
		WriteItemArrows(12, Cstrlen(ConfigTxt));
		WriteArrowsField(L_+R_+D_);
		break;

	default:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+U_+R_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
}


void NTPETHERConfigMenu()
{
	switch(FuncState)
	{
		// Wrap to the right
	case 17:
		FuncState=10;
		break;
	case 43: // On DHCP
		FuncState=40;
		break;

		// wrap to the left
	case 19:
		FuncState=16;
		break;
	case 49:
		FuncState=42; // On DHCP
		break;

		//Switch downs
	case 20: // down on DHCP
		// read current status
		if(IIC1Write(ntp_addr, 0x86))
		{
			//sprintf(&TextBuffer, "Wrote 0x86\n");
			//Udr_PutNChar(TextBuffer);
			NTPConfig.DHCP = IIC1Read(ntp_addr); //IIC1ReadN(ntp_addr, 1, (UC *)NTPConfig.DHCP);
			//sprintf(&TextBuffer, "Read %x\n", NTPConfig.DHCP);
			//Udr_PutNChar(TextBuffer);
		}
		if(NTPConfig.DHCP==1)
			FuncState=40;
		else
			FuncState=41;
		break;
	case 21:
		// read actual IP address

		if(IIC1Write(ntp_addr, 0x82))
			IIC1ReadN(ntp_addr, 4, (UC *)NTPConfig.IP_addr);
		else
			memset(NTPConfig.IP_addr, 0, 4);

		UcharVal=NTPConfig.IP_addr[3];
		UcharVal1=NTPConfig.IP_addr[2];
		UcharVal2=NTPConfig.IP_addr[1];
		UcharVal3=NTPConfig.IP_addr[0];
		UcharVal4=0;

		NTPAddrModifyMenuHelper(0); // modify IP
		return;
	case 22:
		if(IIC1Write(ntp_addr, 0x83))
			IIC1ReadN(ntp_addr, 4, (UC *)NTPConfig.subnet_addr);
		else
			memset(NTPConfig.subnet_addr, 0, 4);

		UcharVal=NTPConfig.subnet_addr[3];
		UcharVal1=NTPConfig.subnet_addr[2];
		UcharVal2=NTPConfig.subnet_addr[1];
		UcharVal3=NTPConfig.subnet_addr[0];
		UcharVal4=1;
		NTPAddrModifyMenuHelper(1); // modify subnet mask
		return;
	case 23:
		if(IIC1Write(ntp_addr, 0x84))
			IIC1ReadN(ntp_addr, 4, (UC *)NTPConfig.gateway);
		else
			memset(NTPConfig.gateway, 0, 4);

		UcharVal=NTPConfig.gateway[3];
		UcharVal1=NTPConfig.gateway[2];
		UcharVal2=NTPConfig.gateway[1];
		UcharVal3=NTPConfig.gateway[0];
		UcharVal4=2;
		NTPAddrModifyMenuHelper(2); // modify gateway
		return;

	case 24: // down on MAC addr
		FuncState=70;
		break;
	case 25: // down on Netfinder name
		FuncState=75;
		break;
	case 26: // down on ESC
		FuncState=16;
		break;

	case 50:  // Down on DHCP enable/disable
		FuncState=40;
		break;
	case 51:
		FuncState=41;
		break;
	case 52:
		FuncState=42;
		break;

		//Switch ups
	case 30:  // Up on DHCP enable/disable
		FuncState=40;
		break;
	case 31:
		FuncState=41;
		break;
	case 32: // up on ESC from DHCP menu
		FuncState=10;
		break;

	case 90:
		FuncState=10;
		break;
	case 91:
		FuncState=11;
		break;
	case 92:
		FuncState=12;
		break;
	case 93:
		FuncState=13;
		break;
	case 94:
		FuncState=14;
		break;
	case 95:
		FuncState=15;
		break;
	case 96:
		FuncEscape=TRUE;
		break;

	case 71: // left/right, down/up on mac + netfinder
	case 79:
	case 60:
	case 80:
		FuncState=14;
		break;

	case 74:
	case 76:
	case 65:
	case 85:
		FuncState=15;
		break;

	}

	if (FuncSave)
	{
		if((FuncState==40)||(FuncState==41)) // Press exec on DHCP enable/disable
		{
			if(FuncState==40)
			{
				IIC1Write2(ntp_addr, 4, 0x01);
				NTPConfig.DHCP=1;
			}
			else
			{
				IIC1Write2(ntp_addr, 4, 0x00);
				NTPConfig.DHCP=0;

				// when DHCP disabled, jump to set ip menu
				// read actual IP address
				if(IIC1Write(ntp_addr, 0x82))
					IIC1ReadN(ntp_addr, 4, (UC *)NTPConfig.IP_addr);
				else
					memset(NTPConfig.IP_addr, 0, 4);

				UcharVal=NTPConfig.IP_addr[3];
				UcharVal1=NTPConfig.IP_addr[2];
				UcharVal2=NTPConfig.IP_addr[1];
				UcharVal3=NTPConfig.IP_addr[0];
				UcharVal4=0;

				NTPAddrModifyMenuHelper(0); // modify IP
				return;
			}

			FuncState=10;
		}
		else if(FuncState==70)
			FuncState=14;
		else if(FuncState==75)
			FuncState=15;

	}

	if (FuncEscape)
	{
		NTPMenuShow();
		Ptr = &NTP_MENU[NTPSelectItem];   // Set pointer to previous menu
		Ptr-> DispFunct();               // Display previous menu
		return;
	}

	//Write "<DHCP><IP ADDR><SUBNET MASK><GATEWAY>..."
	if(FuncState<14) // first part
	{
		WriteCharLN2(0, ' ');
		WriteCode2(DHCPTxt);

		ClearToPosLN2(7);
		WriteCode2(IPAddrTxt);

		ClearToPosLN2(16);
		WriteCode2(SUBNETTxt);

		ClearToPosLN2(29);
		WriteCode2(GATEWAYTxt);

		ClearRestLN2();
	}
	//  "<MAC ADDR><NETFINDER>              <ESC>"
	else if(FuncState<18) // second part
	{
		WriteCharLN2(0, ' ');
		WriteCode2(MACTxt);

		ClearToPosLN2(11);
		WriteCode2(NetfinderTxt);

		ClearToPosLN2(36);
		WriteCode2(ESCTxt);

		ClearRestLN2();
	}
	//Or    " DHCP: <on> <off>                  <ESC>"
	else if(FuncState<43) // DHCP part
	{
		WriteCharLN2(0, ' ');
		WriteCode2(DHCPTxt);
		WriteCharLN2(5, ':');

		ClearToPosLN2(8);
		WriteCode2(OnTxt);

		ClearToPosLN2(13);
		WriteCode2(OffTxt);

		ClearToPosLN2(36);
		WriteCode2(ESCTxt);

		ClearRestLN2();
	}
	//Or    " MAC ADDR: xx:xx:xx:xx:xx:xx       <ESC>"
	else if(FuncState==70) // Mac address part
	{
		if(IIC1Write(ntp_addr, 0x81))
			if(IIC1ReadN(ntp_addr, 6, (UC*) tbuff))
				sprintf(&TextBuffer, "%02x:%02x:%02x:%02x:%02x:%02x",
				        (UC) tbuff[0], (UC) tbuff[1], (UC) tbuff[2], (UC) tbuff[3], (UC) tbuff[4], (UC) tbuff[5]);
			else
				sprintf(&TextBuffer, "%s", NoneTxt);
		else
			sprintf(&TextBuffer, "%s", NoneTxt);

		WriteCharLN2(0, ' ');
		WriteCode2(MACTxt);
		WriteCharLN2(9, ':');

		ClearToPosLN2(11);

		WriteTxt2(TextBuffer);

		ClearToPosLN2(36);
		WriteCode2(ESCTxt);

		ClearRestLN2();

		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+U_+D_+E_);
	}
	else if(FuncState==75)
	{
		// read netfinder name
		if(IIC1Write(ntp_addr, 0x88))
			if(IIC1ReadN(ntp_addr, 32, (UC*) tbuff))
				sprintf(&TextBuffer, "%s", tbuff);
			else
				sprintf(&TextBuffer, "%s", NoneTxt);
		else
			sprintf(&TextBuffer, "%s", NoneTxt);

		WriteCharLN2(0, ' ');

		WriteTxt2(TextBuffer);

		ClearToPosLN2(36);
		WriteCode2(ESCTxt);

		ClearRestLN2();

		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+U_+D_+E_);
	}


	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(1, Cstrlen(DHCPTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 11:
		WriteItemArrows(7, Cstrlen(IPAddrTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 12:
		WriteItemArrows(16, Cstrlen(SUBNETTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 13:
		WriteItemArrows(29, Cstrlen(GATEWAYTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 14:
		WriteItemArrows(1, Cstrlen(MACTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 15:
		WriteItemArrows(11, Cstrlen(NetfinderTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 16:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+U_);
		break;
		// DHCP Arrows
	case 40:
		WriteItemArrows(8, Cstrlen(OnTxt));
		WriteArrowsField(L_+R_+E_);
		break;
	case 41:
		WriteItemArrows(13, Cstrlen(OffTxt));
		WriteArrowsField(L_+R_+E_);
		break;
	case 42:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+U_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
	FuncSave=FALSE;
	FuncEscape=FALSE;
}

void NTPConfigMenu()
{
	switch(FuncState)
	{
		// Wrap to the right
	case 14:
		FuncState=10;
		break;
	case 53:
		FuncState=51;
		break;
	case 58:
		FuncState=55;
		break;

		// wrap to the left
	case 19:
		FuncState=13;
		break;
	case 50:
		FuncState=52;
		break;
	case 54:
		FuncState=57;
		break;

		//Switch downs
	case 20:
		// read current status
		if(IIC1Write(ntp_addr, 0x87))
		{
			//IIC1ReadN(ntp_addr, 1, (UC *) NTPConfig.telnet);
			NTPConfig.telnet = IIC1Read(ntp_addr);
		}
		if(NTPConfig.telnet==1)
			FuncState=55;
		else
			FuncState=56;

		break;

	case 21: // down on port
		FuncState = 25;
		break;

	case 22: // down to reset password
		FuncState=51;
		break;

	case 23: // Down on escape
		FuncState=13;
		break;

		// Down on Telnet on/off + reset passwords
	case 61:
		FuncState=51;
		break;
	case 62:
		FuncState=52;
		break;
	case 65:
		FuncState=55;
		break;
	case 66:
		FuncState=56;
		break;
	case 67:
		FuncState=57;
		break;

		//Switch ups
	case 90: // up on telnet
		FuncState=10;
		break;
	case 91: // up on port
		FuncState=11;
		break;
	case 92: // up on pass reset
		FuncState = 12;
		break;
	case 93:  // up on escape
		FuncEscape=TRUE;
		break;

		// up on reset pass + telnet on/off
	case 41:
		FuncState=51;
		break;
	case 42:
		FuncState=52;
		break;
	case 45:
		FuncState=55;
		break;
	case 46:
		FuncState=56;
		break;
	case 47:
		FuncState=10;
		break;

		//up+down+left+right on port
	case 15:
	case 24:
	case 26:
	case 35:
		FuncState=11;
		break;


	}

	//Write "<TELNET><PORT><RESET PASSWORD>  <ESC>..."
	if(FuncState<14)
	{
		WriteCharLN2(0, ' ');
		WriteCode2(TelnetTxt);

		ClearToPosLN2(9);
		WriteCode2(PortTxt);

		ClearToPosLN2(15);
		WriteCode2(ResetPasswordTxt);

		ClearToPosLN2(36);
		WriteCode2(ESCTxt);

		ClearRestLN2();
	}
	// Port menu
	else if(FuncState<26)
	{
		// Write " PORT:xxxxx                        <ESC>"
		WriteCharLN2(0, ' ');
		WriteCode2(PortTxt);
		WriteCharLN2(5, ':');

		if(IIC1Write(ntp_addr, 0x80))
			if(IIC1ReadN(ntp_addr, 5, (UC*) tbuff))
			{
				tbuff[5]=0;
				sprintf(&TextBuffer, "%s", tbuff);
			}
			else
				sprintf(&TextBuffer, "%s", NoneTxt);
		else
			sprintf(&TextBuffer, "%s", NoneTxt);

		WriteTxt2(TextBuffer);

		ClearToPosLN2(36);
		WriteCode2(ESCTxt);

		ClearRestLN2();
	}
	// Reset password menu
	else if(FuncState<55)
	{
		//Write " RESET PASSWORD             <OK><ESC>..."
		WriteCharLN2(0, ' ');
		WriteCode2(ResetPasswordTxt);

		ClearToPosLN2(29);
		WriteCode2(OKTxt);

		ClearToPosLN2(36);
		WriteCode2(ESCTxt);

		ClearRestLN2();
	}
	else
	{
		//Write " TELNET: <on> <off>                <ESC>"
		WriteCharLN2(0, ' ');
		WriteCode2(TelnetTxt);
		WriteCharLN2(7, ':');

		ClearToPosLN2(10);
		WriteCode2(OnTxt);

		ClearToPosLN2(15);
		WriteCode2(OffTxt);

		ClearToPosLN2(36);
		WriteCode2(ESCTxt);

		ClearRestLN2();
	}

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(1, Cstrlen(TelnetTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 11:
		WriteItemArrows(9, Cstrlen(PortTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 12:
		WriteItemArrows(15, Cstrlen(ResetPasswordTxt));
		WriteArrowsField(L_+R_+D_);
		break;
	case 13:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+U_);
		break;
	case 25:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+U_+D_+E_);
		break;
	case 51:
		WriteItemArrows(29, Cstrlen(OKTxt));
		WriteArrowsField(L_+R_+E_);
		break;
	case 52:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+E_);
		break;
	case 55:
		WriteItemArrows(10, Cstrlen(OnTxt));
		WriteArrowsField(L_+R_+E_);
		break;
	case 56:
		WriteItemArrows(15, Cstrlen(OffTxt));
		WriteArrowsField(L_+R_+E_);
		break;
	case 57:
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+U_);
		break;
	}

	if (FuncSave)
	{
		switch(FuncState)
		{
		case 51: //reset password
			IIC1Write2(ntp_addr, 6, 0x01);

			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			WriteCode1(NetworkTxt);
			WriteChar1('/');
			WriteCode1(ConfigTxt);
			ClearRestLN1();

			FuncState = 12;
			Ptr = &NTP_MENU[NTPConfigItem];
			Ptr -> DispFunct();
			return;

		case 52: //don't reset password
			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			WriteCode1(NetworkTxt);
			WriteChar1('/');
			WriteCode1(ConfigTxt);
			ClearRestLN1();

			FuncState = 12;
			Ptr = &NTP_MENU[NTPConfigItem];
			Ptr -> DispFunct();
			return;

		case 55: // Telnet on
			IIC1Write2(ntp_addr, 5, 0x01);
			NTPConfig.telnet=1;

			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			WriteCode1(NetworkTxt);
			WriteChar1('/');
			WriteCode1(ConfigTxt);
			ClearRestLN1();

			FuncState = 10;
			Ptr = &NTP_MENU[NTPConfigItem];
			Ptr -> DispFunct();
			return;

		case 56: // Telnet off
			IIC1Write2(ntp_addr, 5, 0x00);
			NTPConfig.telnet=0;

			WriteCodeLN1(0, SecondmnuHdrTxt);
			WriteCode1(SlashDotHdrTxt);
			WriteCode1(NetworkTxt);
			WriteChar1('/');
			WriteCode1(ConfigTxt);
			ClearRestLN1();

			FuncState = 10;
			Ptr = &NTP_MENU[NTPConfigItem];
			Ptr -> DispFunct();
			return;

		case 25:	// out of "port" menu
			FuncState = 11;
			Ptr = &NTP_MENU[NTPConfigItem];
			Ptr -> DispFunct();
			return;

		}
	}

	if (FuncEscape)
	{
		if(FuncState==93) //true escape from menu
		{
			NTPMenuShow();
			FuncState=11;
			Ptr = &NTP_MENU[NTPSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}
	}

	FuncTriggers = 0;                  // Reset the button-selection
	FuncSave=FALSE;
	FuncEscape=FALSE;
}


// Displays correct 1st line, and sets UCharVal0-3 to correct IP num
void NTPAddrModifyMenuHelper(char addrnum)
{
	// write " 3RDMNU: ../SUBNET MASK, modify         " //or IP, DNS, gateway
	WriteCodeLN1(0, ThirdmnuHdrTxt);
	WriteCode1(SlashDotHdrTxt);

	switch(addrnum)
	{
	case 0: //IP addr
		WriteCode1(IPAddrTxt);
		break;
	case 1: //subnet mask
		WriteCode1(SUBNETTxt);
		break;
	case 2: //Gateway
		WriteCode1(GATEWAYTxt);
		break;
	case 3: //DNS server
		WriteCode1(DNSTxt);
		break;
	}

	WriteCode1(CoModifyTxt);
	ClearRestLN1();

	FuncSave=0;
	FuncEscape=0;
	FuncState = 71;
	Ptr = &NTP_MENU[NTPSetAddrItem];
	Ptr -> DispFunct();
}


// Addr modify states
//       0  0  0.  0  0  0.  0  0  0.  0  0  0  ok   esc
//State  10 11 12  13 14 15  40 41 42  43 44 45 70   71
//
void NTPAddrModifyMenu()
{
	switch(FuncState)
	{
		//up on IP
	case 90:
		UcharVal+=100;
		FuncState=10;
		break;
	case 91:
		UcharVal+=10;
		FuncState=11;
		break;
	case 92:
		UcharVal++;
		FuncState=12;
		break;
	case 93:
		UcharVal1+=100;
		FuncState=13;
		break;
	case 94:
		UcharVal1+=10;
		FuncState=14;
		break;
	case 95:
		UcharVal1++;
		FuncState=15;
		break;
	case 30:
		UcharVal2+=100;
		FuncState=40;
		break;
	case 31:
		UcharVal2+=10;
		FuncState=41;
		break;
	case 32:
		UcharVal2++;
		FuncState=42;
		break;
	case 33:
		UcharVal3+=100;
		FuncState=43;
		break;
	case 34:
		UcharVal3+=10;
		FuncState=44;
		break;
	case 35:
		UcharVal3++;
		FuncState=45;
		break;


		//down on IP
	case 20:
		UcharVal-=100;
		FuncState=10;
		break;
	case 21:
		UcharVal-=10;
		FuncState=11;
		break;
	case 22:
		UcharVal--;
		FuncState=12;
		break;
	case 23:
		UcharVal1-=100;
		FuncState=13;
		break;
	case 24:
		UcharVal1-=10;
		FuncState=14;
		break;
	case 25:
		UcharVal1--;
		FuncState=15;
		break;
	case 50:
		UcharVal2-=100;
		FuncState=40;
		break;
	case 51:
		UcharVal2-=10;
		FuncState=41;
		break;
	case 52:
		UcharVal2--;
		FuncState=42;
		break;
	case 53:
		UcharVal3-=100;
		FuncState=43;
		break;
	case 54:
		UcharVal3-=10;
		FuncState=44;
		break;
	case 55:
		UcharVal3--;
		FuncState=45;
		break;

		// down on ok/esc
	case 80:
		FuncState=70;
		break;
	case 81:
		FuncState=71;
		break;

		// up on ok/esc
	case 60:
		FuncState=70;
		break;
	case 61:
		FuncState=71;
		FuncEscape=TRUE;
		break;

		// wrap right
	case 16:
		FuncState=40;
		break;
	case 46:
		FuncState=70;
		break;
	case 72:
		if(NTPConfig.DHCP==1)
			FuncState=71;
		else
			FuncState=10;
		break;

		//wrap left
	case 19:
		FuncState=71;
		break;
	case 70:
		if(NTPConfig.DHCP==1)
			FuncState=71;

		break;

	case 79:
		FuncState=45;
		break;
	case 49:
		FuncState=15;
		break;
	}

	// write " 000.000.000.000             <OK>  <ESC>"
	WriteCharLN2(0, ' ');
	sprintf(&TextBuffer, "%03d.%03d.%03d.%03d", UcharVal, UcharVal1, UcharVal2, UcharVal3);
	WriteTxt2(TextBuffer);

	// only ok, when not DCHP on
	if(NTPConfig.DHCP==0)
	{
		ClearToPosLN2(29);
		WriteCode2(OKTxt);
	}

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);

	ClearRestLN2();

	//Arrow fields for all IP digits
	WriteArrowsField(L_+R_+U_+D_);

	switch(FuncState)
	{
	case 10:
		CharBlinkLN2(1, ON);
		break;
	case 11:
		CharBlinkLN2(2, ON);
		break;
	case 12:
		CharBlinkLN2(3, ON);
		break;
	case 13:
		CharBlinkLN2(5, ON);
		break;
	case 14:
		CharBlinkLN2(6, ON);
		break;
	case 15:
		CharBlinkLN2(7, ON);
		break;
	case 40:
		CharBlinkLN2(9, ON);
		break;
	case 41:
		CharBlinkLN2(10, ON);
		break;
	case 42:
		CharBlinkLN2(11, ON);
		break;
	case 43:
		CharBlinkLN2(13, ON);
		break;
	case 44:
		CharBlinkLN2(14, ON);
		break;
	case 45:
		CharBlinkLN2(15, ON);
		break;

		// OK/ESC
	case 70:
		CharBlinkLN2(0, OFF);
		WriteItemArrows(29, Cstrlen(OKTxt));
		WriteArrowsField(L_+R_+E_);
		break;

	case 71:
		CharBlinkLN2(0, OFF);
		WriteItemArrows(36, Cstrlen(ESCTxt));
		WriteArrowsField(L_+R_+U_+E_);
		break;

	default:
		CharBlinkLN2(0, OFF);
		break;
	}

	if( (FuncSave || FuncEscape) && (FuncState>=70))
	{
		if(FuncSave)
			NTPSaveAddr();

		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(NetworkTxt);
		WriteChar1('/');
		WriteCode1(ETHConfigTxt);
		ClearRestLN1();

		FuncState = 11+UcharVal4;
		FuncTriggers = 0;                  // Reset the button-selection
		FuncSave=FALSE;
		FuncEscape=FALSE;
		Ptr = &NTP_MENU[NTPETHERConfigItem];
		Ptr -> DispFunct();
		return;
	}

	FuncTriggers = 0;                  // Reset the button-selection
	FuncSave=FALSE;
	FuncEscape=FALSE;
}

// Saves the address being edited
void NTPSaveAddr()
{
	char sendbuffer[5];

	// write to module
	// UCharVal is globally defined
	sendbuffer[0]=UcharVal4;
	sendbuffer[1]=UcharVal3;
	sendbuffer[2]=UcharVal2;
	sendbuffer[3]=UcharVal1;
	sendbuffer[4]=UcharVal;

	IIC1WriteN(ntp_addr, sizeof(sendbuffer), sendbuffer);

	// write to internal ram
	switch(UcharVal4)
	{
	case 0:
		NTPConfig.IP_addr[0]=sendbuffer[1];
		NTPConfig.IP_addr[1]=sendbuffer[2];
		NTPConfig.IP_addr[2]=sendbuffer[3];
		NTPConfig.IP_addr[3]=sendbuffer[4];
		break;
	case 1:
		NTPConfig.subnet_addr[0]=sendbuffer[1];
		NTPConfig.subnet_addr[1]=sendbuffer[2];
		NTPConfig.subnet_addr[2]=sendbuffer[3];
		NTPConfig.subnet_addr[3]=sendbuffer[4];
		break;
	case 2:
		NTPConfig.gateway[0]=sendbuffer[1];
		NTPConfig.gateway[1]=sendbuffer[2];
		NTPConfig.gateway[2]=sendbuffer[3];
		NTPConfig.gateway[3]=sendbuffer[4];
		break;
	case 3:
		NTPConfig.dns[0]=sendbuffer[1];
		NTPConfig.dns[1]=sendbuffer[2];
		NTPConfig.dns[2]=sendbuffer[3];
		NTPConfig.dns[3]=sendbuffer[4];
		break;
	}
}


void SetLTCTime()
{
	IIC1WriteN(LTC_addr, 4, TimeBuffer);
	IIC1Write2(LTC_addr, 0x03,0x08);
	//sprintf(tbuff,"IIC1ErrorCode=%x\n\r",IIC1ErrorCode);
	//				  Udr_PutNChar(tbuff);

	/*for(l = 0 ; l < 4;l++){

			RecByte=IIC1RdChar( LTC_addr, l);
			sprintf(tbuff,"Reg=%d , Byte=%x ,IIC1ErrorCode=%x\n\r",l,RecByte,IIC1ErrorCode);
						  Udr_PutNChar(tbuff);

	}*/

	/*for(l = 5 ; l < 8;l++){

			RecByte=IIC1RdChar( LTC_addr, l);
			sprintf(tbuff,"Reg=%d , Byte=%x ,IIC1ErrorCode=%x\n\r",l,RecByte,IIC1ErrorCode);
						  Udr_PutNChar(tbuff);

	}


		*/
	//waitms(100);
	//sprintf(tbuff,"IIC1ErrorCode=%x\n\r",IIC1ErrorCode);
	//	  Udr_PutNChar(tbuff);


}

void GetRTCTime()
{
	IIC1WriteN(LTC_addr, 4, TimeBuffer);
	IIC1Write2(LTC_addr, 0x03,0x08);
	//sprintf(tbuff,"IIC1ErrorCode=%x\n\r",IIC1ErrorCode);
	//				  Udr_PutNChar(tbuff);

	/*for(l = 0 ; l < 4;l++){

			RecByte=IIC1RdChar( LTC_addr, l);
			sprintf(tbuff,"Reg=%d , Byte=%x ,IIC1ErrorCode=%x\n\r",l,RecByte,IIC1ErrorCode);
						  Udr_PutNChar(tbuff);

	}*/

	for(l = 5 ; l < 8; l++)
	{

		RecByte=IIC1RdChar( LTC_addr, l);
		sprintf(tbuff,"Reg=%d , Byte=%x ,IIC1ErrorCode=%x\n\r",l,RecByte,IIC1ErrorCode);
		Udr_PutNChar(tbuff);

	}



	//waitms(100);
	//sprintf(tbuff,"IIC1ErrorCode=%x\n\r",IIC1ErrorCode);
	//	  Udr_PutNChar(tbuff);


}

/***************************************************************************/
/* DateTimeRefSelect								  MENU.C  */
/*													*/
/* Author:	  Kim Engedahl, DEV, 980121 						 */
/* Revised:    980730										*/
/*													*/
/*	Function:  Select time reference for the Time Clock Interface.		*/
/*	Remarks:	--										  */
/*	Returns:	--										  */
/*	Updates:	TBD 									 */
/***************************************************************************/
void LTC8620RefSelect()
{

	UC tmp;



	if (FuncSave || FuncEscape)
	{
		if (FuncSave)   			   // If reference has changed..
		{
			ActivePresetNo = 0;			 // Clear a possible preset
			// Save new reference
			TCGConfig.Reference = UcharVal;


			if(UcharVal==iNTSC)
				TCGConfig.System[Internal]=1;

			if(UcharVal==iPAL)
				TCGConfig.System[Internal]=0;



			if(UcharVal==LTC_GPS_Input)
			{
				if(GenlockConfig[GPSGenlock].System==4)
					TCGConfig.System[GPSGenlock]=0;


				if(GenlockConfig[GPSGenlock].System==5)
					TCGConfig.System[GPSGenlock]=1;

			}

			
			ChooseTCGRef(TCGConfig.Reference);

		
		}

		SelectModeOn = FALSE;			 // Clear select-mode

		FuncState = 10;
		Ptr = &time_menu[TimeTimeItem];
		Ptr->DispFunct();				 // Display previous menu
		return;
	}

	if (FuncTriggers & 0x03)  		  // If UP/DOWN button pressed..
	{

	/*if(!GPSGenlockUnit.Present && UcharVal == LTC_GPS_Input)
			UcharVal = LTC_VITC_Input;*/



	sprintf(tbuff,"*UcharVal=%d*",LTC_VITC_Input);
			DebugStr(tbuff);
			sprintf(tbuff,"*UcharVal=%d*",iPAL);
						DebugStr(tbuff);
						sprintf(tbuff,"*UcharVal=%d*",iNTSC);
									DebugStr(tbuff);
									sprintf(tbuff,"*UcharVal=%d*\n",LTC_GPS_Input);
												DebugStr(tbuff);


		sprintf(tbuff,"*UcharVal=%d*",UcharVal);
														DebugStr(tbuff);

			
		if (FuncTriggers & 0x01)  		  // If button pressed is UP..
		{

			/*if(GPSGenlockUnit.Present){
				if (UcharVal-- == LTC_VITC_Input)
				UcharVal = LTC_GPS_Input;}
				
			else{*/
				if (UcharVal-- == LTC_VITC_Input)
					UcharVal = iNTSC;
				//}

		}
		else  						// ..button pressed is DOWN
		{

			/*if(GPSGenlockUnit.Present){
				if (UcharVal++ == LTC_GPS_Input)
					UcharVal = LTC_VITC_Input;
				}
			else{*/
				if (UcharVal++ == iNTSC)
				UcharVal = LTC_VITC_Input;
				//}

			
		}

		
			
		// If currentreference..
		sprintf(tbuff,"*UcharVal=%d*\n",UcharVal);
		
			DebugStr(tbuff);

		
		if (UcharVal == TCGConfig.Reference)
			FuncState = 1;				//	remove SAVE option
	}


	if(FuncState == 1)
	{



		UcharVal = TCGConfig.Reference;
	}
	CodePtr = TimeRefArrTxt[UcharVal];	// Get text for selected pattern



	WriteCodeLN2(0, ReferenceHdrTxt); 	 // Write line 2 header
	WriteCode2(CodePtr);				// Write the selected pattern


	ClearRestLN2();				   // Clear rest of line 2

	WriteType1_SAVE_ESC(FuncState);		 // Write the SAVE ESC texct

	if ((FuncState == 1) || (FuncState == 3))
		WriteItemArrows(11, Cstrlen(CodePtr));// Mark selected item

	FuncTriggers = 0; 				 // Reset button selectors
}





/***************************************************************************/
/* LTC8620Select                                  MENUB.C            */
/*                                                               */
/* Author:    Mikkel Melin, IPU, 24/3-14                              */
/*                                                             */
/*  Function:  Select submenues for LTC generator              */
/*  Remarks:    LTC MENU function.                                   */
/*            Cursor position on line 2 is updated.                    */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void LTC8620Select()
{
	register int length;
	sprintf(tbuff,"*FuncState=%d*",FuncState);
	LTCNo=0;

	DebugStr(tbuff);
	// Handling of selections
	switch(FuncState)
	{
		//wrap around to the right
	case 13:
		FuncState=10;
		break;

		//wrap arount to the left
	case 19:
		FuncState=12;
		break;

		// Up pressed on ESC
	case 95:
		FuncEscape=TRUE;
		break;



//if reference selected:
	case 20:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
		WriteCode1(CoSelectTxt);

		ClearRestLN1();

		FuncState = 10;
		Ptr = &time_menu[TimeTimeItem];
		Ptr -> DispFunct();
		return;

		//if time selected:
	case 21:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
		WriteCode1(CoSelectTxt);

		ClearRestLN1();

		FuncState = 10;
		Ptr = &LTC8620_menu[LTCTimeItem];
		Ptr -> DispFunct();
		return;


		//if sync selected:
	case 22:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(base_menu[BaseNdx].ItemTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCSyncConfirmItem].ItemTxt);
		WriteChar1(' ');
		WriteCode1(LTCTxt);
		WriteChar1(' ');
		WriteCode1(LTCSyncTxt);

		WriteCode1(CoSelectTxt);

		ClearRestLN1();
		FuncState = 10;
		Ptr = &LTC8620_menu[LTCSyncItem];
		Ptr -> DispFunct();
		return;


	}

	if (FuncSave || FuncEscape)
	{
		if(FuncState==95)
		{
			Ptr = PrevPtr;                  // Set pointer to previous menu
			Ptr->DispFunct();               // Display previous menu
			return;
		}

		FuncSave = FALSE;
	}

	//Write "<REFERENCE><TIME><SYNC>"
	WriteCodeLN2(0, SubmnuHdrTxt);

	WriteCodeLN2(8, ReferenceTxt);
	ClearToPosLN2(20);


	WriteCodeLN2(20, TimeTxt);
	ClearToPosLN2(27);


	WriteCodeLN2(27, LTCSyncTxt);
	ClearRestLN2();


//	sprintf(tbuff,"FuncState=%d\n",FuncState);


	//  DebugStr(tbuff);


	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(8, Cstrlen(ReferenceTxt));
		WriteArrowsField(L_+U_+R_+D_);
		break;

	case 11:
		WriteItemArrows(20, Cstrlen(TimeTxt));
		WriteArrowsField(L_+U_+R_+D_);
		break;

	case 12:
		WriteItemArrows(27, Cstrlen(LTCSyncTxt));
		WriteArrowsField(L_+U_+R_+D_);
		break;

	default :
		WriteItemArrows(8, Cstrlen(ReferenceTxt));
		WriteArrowsField(L_+U_+R_+D_);
		break;

	}

	FuncTriggers = 0;                  // Reset the button-selection
}

/*void LTC8620Down(){

  Ptr = &BB_menu[BBNdx[AnlBlkNdx]];    // Restore unit BBn's last item
}*/


/***************************************************************************/
/* LTCTimeSet                                 MENUB.C            */
/*                                                               */
/* Author:    Jens K Hansen, DEV, 26/9/2007                              */
/*                                                             */
/*  Function:  Select time submenues                     */
/*  Remarks:    LTC MENU function.                                   */
/*            Cursor position on line 2 is updated.                    */
/*  Returns:    --                                                 */
/*  Updates:    --                                                 */
/***************************************************************************/
void LTC8620TimeSet()
{

	//sprintf(tbuff,"FuncState=%d 86020",FuncState);


	//  DebugStr(tbuff);

	switch(FuncState)
	{
		// Wrap to the right
	case 12:
		FuncState=10;
		break;

		// wrap to the left
	case 19:
		FuncState=12;
		break;

		//down pressed on <TIMEZONE>
	case 20:
		WriteCodeLN1(0, ThirdmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCTxt);
		WriteChar1('/');
		WriteCode1(LTCTimezoneTxt);
		WriteCode1(CoEditTxt);

		ClearRestLN1();

		UcharVal1=LTCConfig[0].TimezoneHour+12;
		UcharVal=LTCConfig[0].TimezoneMin;

		FuncState = 10;
		Ptr = &LTC8620_menu[LTCTimezoneItem];
		Ptr -> DispFunct();
		return;

		//down pressed on <DAYLIGHT>
	case 21:
		WriteCodeLN1(0, ThirdmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCDaylightItem].ItemTxt);
		WriteCode1(CoSelectTxt);

		ClearRestLN1();

		UcharVal=LTCConfig[0].DaylightMode;
		UcharVal1=LTCConfig[0].DaylightState;

		FuncState = 10;
		Ptr = &LTC8620_menu[LTCDaylightItem];
		Ptr -> DispFunct();
		return;

		//Up pressed on <TIMEZONE> or <DAYLIGHT>
	case 90:
		Ptr = &LTC8620_menu[0];
		sprintf(tbuff,"up T",FuncState);



		DebugStr(tbuff);
		FuncState=11;
		Ptr -> DispFunct();
		return;

	case 91:
		Ptr = &LTC8620_menu[0];
		FuncState=11;
		Ptr -> DispFunct();
		return;



	}



	//Write "2NDMNU:  TIMEZONE  DAYLIGHT       <ESC>"
	WriteCodeLN2(0, ThirdmnuHdrTxt);

	ClearToPosLN2(9);
	WriteCode2(LTCTimezoneTxt);

	ClearToPosLN2(19);
	WriteCode2(LTCDaylightTxt);
	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(9, 8);
		WriteArrowsField(L_+R_+D_+U_);
		break;
	default:
		WriteItemArrows(19, 8);
		WriteArrowsField(L_+R_+D_+U_);
		break;

	}

	FuncTriggers = 0;                  // Reset the button-selection

//sprintf(tbuff,"FuncState=%d \n",FuncState);


	//	DebugStr(tbuff);


}

void UpToLTC8620()
{

	Ptr = &LTC8620_menu[LTCTimeItem];
}



void LTC8620SyncConfirm()
{
	sprintf(tbuff,"save=%d esc=%d",FuncSave,FuncEscape);


	DebugStr(tbuff);

	switch(FuncState)
	{
		// Wrap to the right
	case 12:
		FuncState=10;
		break;
		// wrap to the left
	case 19:
		FuncState=11;
		break;
		//Switch down
	case 20:
		FuncState=10;
		break;
	case 21:
		FuncState=11;
		break;
		//Switch up
	case 90:
		FuncState=10;
		break;
	case 91:
		FuncState=11;
		break;
	}

	if (FuncSave || FuncEscape)
	{
		//If save <OK>, return to prev menu, and send confirmation
		if(FuncState==10)
		{
			//if entered from confirm sync
			if(LTC_sync_request&34)
			{
			
					IIC1Write2(d2ram_addr, 54, 1); //Send '1' to GPS data register 54 (confirm syncrequest) LTC a
				
			}

			//else, manual sync
			else
			{
				//send data to GPS module
				//LTC A sync
				
				IIC1Write2(d2ram_addr, 52, 1);
				
				TCG_sync_request=0;
				ctrl1temp=ctrl1;
						ctrl1=ctrl1|0x04;
        IIC1Write2(LTC_addr, ctrl1_reg,ctrl1);
				
				

			}

			//return to top menu
			FuncState=12;
			LTCMenuShow();
			Ptr = &LTC8620_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}

		//If <ESC>, return to prev menu, and send not confirmed
		if(FuncState==11)
		{
			//request for sync on LTC A
			IIC1Write2(d2ram_addr, 54, 0);  //Send '0' to GPS data register 54 (decline syncrequest) LTC a


			//return to top menu
			FuncState=12;
			LTCMenuShow();
			Ptr = &LTC8620_menu[LTCSelectItem];   // Set pointer to previous menu
			Ptr-> DispFunct();               // Display previous menu
			return;
		}

		FuncSave = FALSE;
	}

	//Write "SYNC LTC X:  <OK>  <ESC>               "
	WriteCodeLN2(0, LTCSyncTxt);
	WriteChar2(' ');
	WriteCodeLN2(5, LTCTxt);
	WriteChar2(':');

	ClearToPosLN2(31);
	WriteCode2(OKTxt);

	ClearToPosLN2(36);
	WriteCode2(ESCTxt);

	ClearRestLN2();

	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(31, 2);
		WriteArrowsField(L_+R_+E_);
		break;
	case 11:
		WriteItemArrows(36, 3);
		WriteArrowsField(L_+R_+E_);
		break;
	}

	FuncTriggers = 0;                  // Reset the button-selection
	FuncSave=FALSE;
	FuncEscape=FALSE;
}



void LTC8620Sync()
{
	LTCNo=0;
	sprintf(tbuff,"FuncState=%d\n",FuncState);


	DebugStr(tbuff);

	switch(FuncState)
	{
	case 12:
		FuncState=10;
		break;
	case 19:
		FuncState=11;
		break;


	case 90:
		Ptr = &LTC8620_menu[0];

		FuncState=12;
		Ptr -> DispFunct();
		return;

	case 91:
		Ptr = &LTC8620_menu[0];
		FuncState=12;
		Ptr -> DispFunct();
		return;

		// If down pressed on <SYNCMODE>
	case 20:
	case 60:
		WriteCodeLN1(0, ThirdmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCModeItem].ItemTxt);
		WriteCode1(CoEditTxt);
		ClearRestLN1();

		//set temporary dropframe mode to current LTC dropframe mode
		UcharVal=LTCConfig[LTCNo].DFMode;
		UcharVal1=LTCConfig[LTCNo].DFHour;
		UcharVal2=LTCConfig[LTCNo].DFMin;

		FuncState = 10;
		Ptr = &LTC8620_menu[LTCModeItem];
		Ptr -> DispFunct();
		return;

		// If down pressed on <SYNC>
	case 21:
	case 61:
		WriteCodeLN1(0, ThirdmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCSyncConfirmItem].ItemTxt);
		WriteChar1(' ');
		WriteCode1(LTCTxt);
		WriteChar1(' ');
		WriteCode1(LTCSyncTxt);

		WriteCode1(CoSelectTxt);
		ClearRestLN1();

		FuncState = 10;
		Ptr = &LTC8620_menu[LTCSyncConfirmItem];
		Ptr -> DispFunct();
		return;
	}


	WriteCharLN2(0,' ');
	// WriteCode2(LTCSyncTxt);

	WriteCodeLN2(1, LTCSyncTxt);

	WriteCodeLN2(5, LTCModeTxt);


	ClearToPosLN2(11);

	WriteCodeLN2(11, LTCSyncTxt);
	//ClearToPosLN2(27);

	ClearRestLN2();

	// Show selection brackets, and option arrows

	switch (FuncState)
	{
	case 10:
		WriteItemArrows(1, 8);
		WriteArrowsField(L_+U_+R_+D_);
		break;

	case 11:
		WriteItemArrows(11, Cstrlen(LTCSyncTxt));
		WriteArrowsField(L_+U_+R_+D_);
		break;




	}



	/*switch (FuncState) {
	  case 10:
	  case 50:
	    WriteItemArrows(1, 8);
	    WriteArrowsField(L_+U_+R_+D_);
	    break;

	  case 11:
	  case 51:
	    WriteItemArrows(21, 8);
	    WriteArrowsField(L_+R_+D_);
	    break;

	  case 52:
	WriteItemArrows(32, Cstrlen(OKTxt));
	    WriteArrowsField(L_+E_+R_);
	    break;

	  default:
	    WriteItemArrows(36, Cstrlen(ESCTxt));
	    WriteArrowsField(L_+U_+R_);
	    break;
	}*/
	FuncSave=FALSE; // Reset enter button 
	FuncTriggers = 0;                  // Reset the button-selection
}


/***************************************************************************/
/* DateTimeMenuShow								  MENU.C  */
/* 												 */
/* Author:    Kim Engedahl, DEV, 980121						  */
/* Revised:	980507										 */
/* 												 */
/*  Function:	Display the items in the CONFIG/DATE-TIME submenu.		  */
/*  Remarks:	 Cursor position on line 2 is updated.				  */
/*  Returns:	 -- 									   */
/*  Updates:	 TBD									  */
/***************************************************************************/
void TimeMenuShow()
{

	register int i, j;
	sprintf(tbuff,"FuncState=%d",FuncState);


	DebugStr(tbuff);

	switch(FuncState)
	{
		// Wrap to the right
	case 12:
		FuncState=10;
		break;

		// wrap to the left
	case 19:
		FuncState=12;
		break;

	case 90:

		if(GPSGenlockUnit.Present)
		Ptr = &LTC8620_menu[0];
		else
			Ptr = &base_menu[LTCItem];
		FuncState=10;
		Ptr -> DispFunct();
		return;
	case 91:
		if(GPSGenlockUnit.Present)
		Ptr = &LTC8620_menu[0];
		else
			Ptr = &base_menu[LTCItem];

		
		FuncState=10;

		Ptr -> DispFunct();
		return;



		//if reference selected:
	case 20:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
		WriteCode1(CoSelectTxt);

		ClearRestLN1();
		UcharVal = TimeClockConfig.Reference;	   // Get reference
		FuncState = 1;					// Prepare for item state-machine
		FuncTriggers = 0;
		FuncSave = FuncEscape = FALSE;

		SelectModeOn = TRUE;			  // Set select-mode
		Ptr = &time_submenu[TimeRefItem];
		Ptr -> DispFunct();
		return;
		//if time selected:
	case 21:
		WriteCodeLN1(0, SecondmnuHdrTxt);
		WriteCode1(SlashDotHdrTxt);
		WriteCode1(LTCTxt);
		WriteChar1('/');
		WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
		WriteCode1(CoSelectTxt);

		ClearRestLN1();




		FuncState = 1;					// Prepare for item state-machine
		FuncTriggers = 0;
		FuncSave = FuncEscape = FALSE;

		SelectModeOn = TRUE;			  // Set select-mode

		Ptr = &time_submenu[TimeTimeItem];
		Ptr -> DispFunct();
		return;



	}

	WriteCodeLN1(0, ThirdmnuHdrTxt);
	WriteCode1(SlashDotHdrTxt);
	WriteCode1(LTCTxt);
	WriteChar1('/');
	WriteCode1(LTC_menu[LTCTimeItem].ItemTxt);
	WriteCode1(CoSelectTxt);

	ClearRestLN1();

	WriteCodeLN2(0, SubmnuHdrTxt);

	WriteCodeLN2(8, ReferenceTxt);
	ClearToPosLN2(20);


	WriteCodeLN2(20, TimeTxt);


	ClearRestLN2();					  // Clear rest of line 2


	// Show selection brackets, and option arrows
	switch (FuncState)
	{
	case 10:
		WriteItemArrows(8, Cstrlen(ReferenceTxt));
		WriteArrowsField(L_+U_+R_+D_);
		break;

	case 11:
		WriteItemArrows(20, Cstrlen(TimeTxt));
		WriteArrowsField(L_+U_+R_+D_);
		break;



	default :
		WriteItemArrows(8, Cstrlen(ReferenceTxt));
		WriteArrowsField(L_+U_+R_+D_);
		break;

	}





}
/***************************************************************************/
/* DateTimeTimeSelect								MENU.C	*/
/*												   */
/* Author:	 Kim Engedahl, DEV, 980121							*/
/* Revised:	  980916									   */
/*												   */
/*  Function:  Select time-format and/or set time in time clock option	 */
/* Remarks:	  When exiting the edited date, the date will be reset if it  */
/*		   is not valid.								*/
/*  Returns:    --										 */
/*  Updates:    TBD										*/
/***************************************************************************/
/***************************************************************************/
 /* DateTimeTimeSelect								  MENU.C  */
 /* 												 */
 /* Author:    Kim Engedahl, DEV, 980121						  */
 /* Revised:	980916										 */
 /* 												 */
 /*  Function:	Select time-format and/or set time in time clock option    */
 /* Remarks:	When exiting the edited date, the date will be reset if it	*/
 /* 		 is not valid.								  */
 /*  Returns:	 -- 									   */
 /*  Updates:	 TBD									  */
 /***************************************************************************/
 void RTCTimeSelect(){
	 UC tmp;
	 UC regcount;
	 UC TimeArray[5]; //[set_hh_reg,hh,mm,ss]
   UC RecByte[4];
   if (FuncState==1)
	   FuncState++;

		   
	   
	 
	   if (FuncSave || FuncEscape) {
		 if (FuncSave) {				// If time has changed..
		   ActivePresetNo = 0;			  // Clear a possible preset
	 
		   //TimeClockConfig.Format &= 0xFE;	// Save new time format
		   //TimeClockConfig.Format |= UcharVal;
	 
		   //TimeClockConfig.Time = UlongVal;  // Save new time

		   //UlongVal=bcd2UC((UC) UlongVal&0xFF);


		   TimeArray[0]=(UC) set_hh; //[set_hh_reg,hh,mm,ss]
		   TimeArray[1]=bcd2UC((UC) ((UlongVal>>16) &0xFF));
		   TimeArray[2]=bcd2UC((UC) ((UlongVal>>8) &0xFF));
		   TimeArray[3]=bcd2UC((UC) ((UlongVal) &0xFF));

			if((TCGConfig.System[Internal])){//NTSC
			TCGConfig.Reference=iNTSC;
				TimeArray[4]=0x88;
		   }
				   else{
			   TimeArray[4]=0x08;

		   TCGConfig.Reference=iPAL;
					   }

				   
		   IIC1WriteN(LTC_addr, 5, TimeArray);
		   
		   
		 }
	 
		 CharBlinkLN2(0, OFF);			  // Switch off possible blinking
	 
		 SelectModeOn = FALSE;			  // Clear select-mode
	 
		 FuncState = 11;
		Ptr = &time_menu[TimeTimeItem];
		 Ptr->DispFunct();				  // Display time menu
		 return;
	   }
								   // Select line to edit
	   
								   // Select position in TextBuffer
	   if (FuncTriggers & 0xC0) {
		   // If LEFT/RIGHT button pressed..

			
		 if (FuncTriggers & 0x40) { 	   // If button pressed is LEFT..
		   if (UcharVal1-- == 0)
			 UcharVal1 = 7;
	 
		   if ((UcharVal1 == 2) || (UcharVal1 == 5))
			 UcharVal1--;
		 }
		 else { 					 // ..button pressed is RIGHT
		   if (UcharVal1++ == 7)
			 UcharVal1 = 0;
	 
		   if ((UcharVal1 == 2) || (UcharVal1 == 5))
			 UcharVal1++;
		 }
		   }



								   

								   // Select character in TextBuffer
	   if (FuncTriggers & 0x30) {		   // If UP/DOWN button pressed..
	 
	   
	 

		 UlongVal = AdjustTime(UcharVal1, (FuncTriggers & 0x10), Hour24Format, UlongVal);


		 TimeFormat(Hour24Format,(UlongVal), 0, tbuff);


	   

		   
		/* if (UlongVal == TimeClockConfig.Time)	  // If current format..
		   if (UcharVal == (TimeClockConfig.Format & 0x01))
			 FuncState = 8;    */					 //  remove SAVE option
	   }
	 



   if(FuncState<4){
	   UlongVal=GetTimeLong();
	   TimeFormat(Hour24Format,(UlongVal), 0, tbuff);

	   }
	 
	   WriteCodeLN2(0, TimeHdrTxt); 		 // Write line 2 header
	   ClearToPosLN2(17);
	 
	   WriteTxt2(tbuff);			   // Write edited text on line 2
	   ClearRestLN2();					// Clear rest of line 2
	 
	   WriteType13_SAVE_ESC(FuncState); 	 // Write the SAVE ESC text
	 
	   switch (FuncState){
		 case 1:
		 case 4:					  // Mark selected item
		   WriteItemArrows(6, Cstrlen(CodePtr));
		   CharBlinkLN2(0, OFF);		  // Switch OFF blinking
		   break;
	 
		 case 2:
		 case 5:					  // Mark selected item
		   UcharVal1 = 0;
	 
		   if (!ValidTime(UlongVal)) {
			 //UlongVal = TimeClockConfig.Time;
			 TimeFormat(UcharVal, UlongVal, 0, TextBuffer);
	 
			 WriteTxtLN2(17, TextBuffer);	 // Write edited text on line 2
		   }
	 
		   WriteItemArrows(17, strlen(tbuff));
		   CharBlinkLN2(0, OFF);		  // Switch OFF blinking
		   break;
	 
		 case 8:
		 case 9:					  // Mark selected item
		   WriteItemBrackets(17, strlen(tbuff));
		   CharBlinkLN2(17 + UcharVal1, ON);  // Switch OFF blinking at
		   break;					 //  editing character
	   }
	 
	   FuncTriggers = 0;				  // Reset the button-selection
   }

 


UL UC2bcd( UC ival)
{
	return (UL) (((ival / 10) << 4) | (ival % 10));
}


UC bcd2UC( UC ival)
{
	return ((ival>>4)*10)+ival%16;
}

