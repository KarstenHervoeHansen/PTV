/****************************************************************************/
/* MODULE:                                                                  */
/*  genlock.c - Genlock generator                                           */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*	int GenlockInit( void)																									*/
/*	int GenlockVersion( char* result)																				*/
/*	int GenlockUpdate( void)																								*/
/*	int SetGenlockSystem( int system)																				*/
/*	int SetGenlockDelay( UL delay)																					*/	
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
#include "instru.h"
#include "cbar.h"

GenlockObject GenlockSetup;

/**************************************************************************/
/* GenlockDefaults																							GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000607																		*/
/* Revised:	 000607, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void GenlockDefaults( void)
{
	Settings.GenlockSetup.System = GenlockSetup.System = Internal;
	Settings.GenlockSetup.Delay = GenlockSetup.Delay = 0;

	NV_Store( &Settings, SettingsPtr, sizeof( Settings));
}

/**************************************************************************/
/* GenlockInit					 																			  GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000914, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int GenlockInit( void)
{
	sio0SndCommand( 12, SPG_V24_address, "GZ;");

	sio0SndCommand( 20, SPG_V24_address, "GV%lu;", Calibration.GenlockCalibration.HPHZeroG);
	sio0SndCommand( 20, SPG_V24_address, "GF%lu;", Calibration.GenlockCalibration.PhaseG);

	sio0SndCommand( 20, SPG_V24_address, "GY%lu;", Calibration.GenlockCalibration.HPHZeroM);
	sio0SndCommand( 20, SPG_V24_address, "GG%lu;", Calibration.GenlockCalibration.PhaseM);

	sio0SndCommand( 20, SPG_V24_address, "GX%u;", Calibration.GenlockCalibration.DACUserValue);

	GenlockSetup.System = Settings.GenlockSetup.System;
	GenlockSetup.Delay = Settings.GenlockSetup.Delay;

	return( 0);
}

/**************************************************************************/
/* GenlockVersion				 																			  GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000504																		*/
/* Revised:	 000603, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int GenlockVersion( char* result)
{
	return( sio0SndRequest( 12, SPG_V24_address, result, "GI?"));
}

/**************************************************************************/
/* GenlockUpdate				 																			  GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000413																		*/
/* Revised:	 000509, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int GenlockUpdate( void)
{
	SetGenlockSystem( GenlockSetup.System);
  SetGenlockDelay( GenlockSetup.Delay);

	return( OK);
}

/**************************************************************************/
/* SetGenlockSystem			 																			  GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000619, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetGenlockSystem( int system)
{
	char cmd[7];

	switch ( system)
	{
		case Internal:
			strcpy( cmd, "GM0;");
			break;

		case MHzLock10:
			strcpy( cmd, "GM2;");
			break;

		case BurstLockPAL:
			strcpy( cmd, "GM8;");
			break;

		case BurstLockNTSC:
			strcpy( cmd, "GM10;");
			break;

		default:
			return( FAIL);
	}

	Settings.ActivePreset = 0;

	Settings.GenlockSetup.System = GenlockSetup.System = system;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	LEDSet( GenlockLED, false);
	
	return( sio0SndCommand( 12, SPG_V24_address, "%s", cmd));
}

/**************************************************************************/
/* SetGenlockDelay				 																			GENLOCK.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000731, KEn, DEV     																				*/
/*																																				*/
/* Function: Broadcast common delay to BB and CBar												*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetGenlockDelay( UL delay)
{
	Settings.ActivePreset = 0;

	Settings.GenlockSetup.Delay = GenlockSetup.Delay = delay;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	sio0SndCommand( 20, BB_V24_address, "HO%lu;", delay);
	sio0SndCommand( 20, BB_V24_address, "JO%lu;", delay);
	sio0SndCommand( 20, BB_V24_address, "KO%lu;", delay);

	SetCBDelay( CBSetup.Delay);	// The delay routine also takes the genlock
															//  offset into account.																			
	return( OK);
}
