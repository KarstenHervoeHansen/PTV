/****************************************************************************/
/* MODULE:                                                                  */
/*  unitprg.c - 							                                              */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
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
#include "define.h"        // Standard PTV defines
#include "unitprg.h"

volatile UI BBSPGResetPort _at( 0x6a018);		// Port defined in PLD

/**************************************************************************/
/* BBSPGReset																									  UNITPRG.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000330																		*/
/* Revised:	 000330, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void BBSPGReset( bool SPGReset, bool BBReset, bool SPGPrg)
{
	UI tmp = 3;

	if ( SPGReset)
		tmp &= 0xFE;

	if ( BBReset)
		tmp &= 0xFD;

	if ( SPGPrg)
		tmp |= 0x04;

	BBSPGResetPort = tmp;
}

/**************************************************************************/
/* BBSPGCommunication																					  UNITPRG.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000407																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void BBSPGCommunication( int unit)
{
	// Stop the world
}
