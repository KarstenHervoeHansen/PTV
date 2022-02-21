/****************************************************************************/
/* MODULE:                                                                  */
/*  bb.c - Black burst generators                                           */
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
#include <string.h>

#include "define.h"        // Standard PTV defines
#include "bb.h"
#include "tables.h"
#include "led_hw.h"
#include "sio0drv.h"

BBObject BBSetup[NoOfBBOutputs];

/**************************************************************************/
/* BBInit																											       BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int BBInit( void)
{
	int i;

	for ( i = BBOutput1; i <= BBOutput3; i++)
	{
		BBSetup[i].System = PAL;
		BBSetup[i].Delay = 0;
		BBSetup[i].ScHPhase = 0;

		SetBBSystem( i, BBSetup[i].System);
	}
}

/**************************************************************************/
/* SetBBSystem																								       BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetBBSystem( UC output, int system)
{
	char cmd[3];
	UI PALLED, NTSCLED;

	cmd[2] = '\0';

	switch ( system)
	{
		case PAL:
			cmd[1] = 'G';
			break;

		case PAL_PALID:
			cmd[1] = 'K';
			break;

		case NTSC:
			cmd[1] = 'M';
			break;

		default:
			return( FAIL);
	}

	switch ( output)
	{
		case BBOutput1:
			cmd[0] = 'H';

			PALLED = BB1PALLED;
			NTSCLED = BB1NTSCLED;
			break;

		case BBOutput2:
			cmd[0] = 'J';

			PALLED = BB2PALLED;
			NTSCLED = BB2NTSCLED;
			break;

		case BBOutput3:
			cmd[0] = 'K';

			PALLED = BB3PALLED;
			NTSCLED = BB3NTSCLED;
			break;

		default:
			return( FAIL);
	}

	LEDSet( PALLED, system < NTSC);
	LEDSet( NTSCLED, system >= NTSC);

	BBSetup[output].System = system;

	return( sio0SndCommand( 20, BB_V24_address, "%s%d;", cmd, BBSetup[output].Delay));
}

/**************************************************************************/
/* SetBBDelay																										     BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetBBDelay( UC output, UL delay)
{
	char cmd[3];

	cmd[2] = '\0';

	switch ( output)
	{
		case BBOutput1:
			cmd[0] = 'H';
			break;

		case BBOutput2:
			cmd[0] = 'J';
			break;

		case BBOutput3:
			cmd[0] = 'K';
			break;

		default:
			return( FAIL);
	}

	switch ( BBSetup[output].System)
	{
		case PAL:
			cmd[1] = 'G';
			break;

		case PAL_PALID:
			cmd[1] = 'K';
			break;

		case NTSC:
			cmd[1] = 'M';
			break;

		default:
			return( FAIL);
	}

	BBSetup[output].Delay = delay;

	return( sio0SndCommand( 20, BB_V24_address, "%s%d;", cmd, BBSetup[output].Delay));
}

/**************************************************************************/
/* SetBBScHPhase																									   BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetBBScHPhase( UC output, int schphase)
{
	char cmd[3];
	long tmp = schphase;

	if (( schphase < ScHPhaseLow) || ( schphase > ScHPhaseHigh))
		return( FAIL);

	switch ( output)
	{
		case BBOutput1:
			strcpy( cmd, "HH");
			break;

		case BBOutput2:
			strcpy( cmd, "JH");
			break;

		case BBOutput3:
			strcpy( cmd, "KH");
			break;

		default:
			return( FAIL);
	}

	BBSetup[output].ScHPhase = schphase;

	if ( schphase < 0)								// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 2048;
	tmp /= 360;												// tmp = ( ScHPhase*2048)/360;
														
	return( sio0SndCommand( 12, BB_V24_address, "%s%d;", cmd, (int) tmp));
}

