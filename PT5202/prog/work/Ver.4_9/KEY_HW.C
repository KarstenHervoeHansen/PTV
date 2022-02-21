/****************************************************************************/
/* MODULE:                                                                  */
/*  key_hw.c - Keyboard driver                                              */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*	int KeyboardGet( UC *scancode);																					*/
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#include "reg8051.h"
#include "define.h"        // Standard PTV defines
#include "key_hw.h"
#include "keycodes.h"

volatile UI PushButtonPort _at(0x6a010); 	// Port defined in PLD

/**************************************************************************/
/* KeyboardGet																								   KEY_HW.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000208																		*/
/* Revised:	 000321, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int KeyboardGet( UC *scancode)
{
	if ( PushButtonPort & 0x01)
		*scancode = KeyNone;
	else
		*scancode = KeyExecute;

	return( OK);
}
