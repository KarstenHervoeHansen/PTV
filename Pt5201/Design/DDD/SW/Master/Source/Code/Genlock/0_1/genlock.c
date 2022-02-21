/****************************************************************************/
/* MODULE:                                                                  */
/*  genlock.c - Genlock generator                                           */
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
#include "genlock.h"
#include "tables.h"
#include "led_hw.h"
#include "sio0drv.h"

GenlockObject GenlockSetup;

/**************************************************************************/
/* SetGenlockSystem			 																			  GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000405, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void GenlockInit( void)
{
	sio0SndCommand( 12, SPG_V24_address, "GZ;");
}

/**************************************************************************/
/* SetGenlockSystem			 																			  GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000405, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetGenlockSystem( int system)
{
	char cmd[8];

	GenlockSetup.System = system;

	LEDSet( GenlockLED, false);

	switch ( system)
	{
		case MHzLock10:
			strcpy( cmd, "GM2");
			break;

		case BurstLockPAL:
			strcpy( cmd, "GM8");
			break;

		case BurstLockNTSC:
			strcpy( cmd, "GM10");
			break;

		default:
			strcpy( cmd, "GM0");
			break;
	}

	sio0SndCommand( 12, SPG_V24_address, "%s;", cmd);

/*#define sio0NoError				(int)0
#define sio0NoAcknowledge	(int)1
#define sio0TxCksumError	(int)3

#define sio0RxCksumError	(int)2
#define sio0RxTimeout			(int)4

#define sio0UnknownError	(int)5
*/
	return( OK);
}

/**************************************************************************/
/* SetGenlockDelay				 																			GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000331, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetGenlockDelay( UL delay)
{
	GenlockSetup.Delay = delay;

	return( OK);
}
