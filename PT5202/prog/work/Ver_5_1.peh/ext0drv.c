/****************************************************************************/
/* MODULE:                                                                  */
/*  ext0drv.c - external interrupt 0 driver                                 */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
    void ext0_init(void);
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
#include <periphal.h>
#include "ext0drv.h"


/**************************************************************************/
/* ext0_init	 																								 EXT0_DRV.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000208																		*/
/* Revised:		000321, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void ext0_init( void)
{
  IPA0 = (IPA0 & 0xF0) | EX0INTLVL;

  IT0 = 1;   // Set to specify falling edge triggered ext. interrupt
  EX0 = 1;   // Enable external 0 interrupt
}
