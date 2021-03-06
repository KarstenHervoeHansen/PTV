/****************************************************************************/
/* MODULE:                                                                  */
/*  bb.c - Black burst generators                                           */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*	void BBDefaults( void)																									*/	
/*	int BBInit( void)																												*/	
/*	int BBVersion( char *result)																						*/	
/*	int BBUpdate( void)																											*/
/*	int SetBBSystem( int output, int system)																*/
/*	int SetBBDelay( int output, UL delay)																		*/
/*	int SetBBScHPhase( int output, int schphase)														*/
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
#include "instru.h"

code char BBOutputCmd[3] = { 'H', 'J', 'K' };
code char BBSystemCmd[4] = { 'G', 'K', 'M', 'L' };

BBObject BBSetup[NoOfBBOutputs];

volatile int BBControlPort _at(0x70008); 		// Port defined in SPG/AUDIO PLD

/**************************************************************************/
/* BBDefaults																								     	CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000606																		*/
/* Revised:	 000617, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void BBDefaults( void)
{
	int i;

	for ( i = BBOutput1; i <= BBOutput3; i++)
	{
		Settings.BBSetup[i].System = BBSetup[i].System = Calibration.ResetSystem;
		Settings.BBSetup[i].Delay = BBSetup[i].Delay = 0;
		Settings.BBSetup[i].ScHPhase = BBSetup[i].ScHPhase = 0;
	}

	NV_Store( &Settings, SettingsPtr, sizeof( Settings));
}

/**************************************************************************/
/* BBInit																											       BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000914, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int BBInit( void)
{
	char cmd;
	int i;

	for ( i = BBOutput1; i <= BBOutput3; i++)
	{
		cmd = BBOutputCmd[i];

		sio0SndCommand( 20, BB_V24_address, "%cX%lu;", cmd, Calibration.BBCalibration[i].PhaseG);
		sio0SndCommand( 20, BB_V24_address, "%cT%u;", cmd, Calibration.BBCalibration[i].ScHPhaseG);

		sio0SndCommand( 20, BB_V24_address, "%cY%lu;", cmd, Calibration.BBCalibration[i].PhaseM);
		sio0SndCommand( 20, BB_V24_address, "%cU%u;", cmd, Calibration.BBCalibration[i].ScHPhaseM);

		sio0SndCommand( 20, BB_V24_address, "%cE%u;", cmd, Calibration.BBCalibration[i].DACGain);
		sio0SndCommand( 20, BB_V24_address, "%cC%u;", cmd, Calibration.BBCalibration[i].DACOffset);
		sio0SndCommand( 20, BB_V24_address, "%cD%u;", cmd, Calibration.BBCalibration[i].DACLevel);

		BBSetup[i].System = Settings.BBSetup[i].System;
		BBSetup[i].Delay = Settings.BBSetup[i].Delay;
		BBSetup[i].ScHPhase = Settings.BBSetup[i].ScHPhase;
	}

	return( 0);
}

/**************************************************************************/
/* BBVersion					 																				       BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000504																		*/
/* Revised:	 000604, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int BBVersion( char *result)
{
	return( sio0SndRequest( 12, BB_V24_address, result, "HI?"));
}

/**************************************************************************/
/* BBUpdate																										       BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000413																		*/
/* Revised:	 000606, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int BBUpdate( void)
{
	int i;

	for ( i = BBOutput1; i <= BBOutput3; i++)
	{
		SetBBSystem( i, BBSetup[i].System);			// SetBBSystem ALSO sets the delay
		SetBBScHPhase( i, BBSetup[i].ScHPhase);
	}

	return( OK);
}

/**************************************************************************/
/* SetBBSystem																								       BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000731, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 Due to an errorneous BB SW the ScHPhase MUST be transmitted	*/
/*					 when changing system.																				*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetBBSystem( int output, int system)
{
	UI PALLED, NTSCLED;
	long tmp;

	Settings.ActivePreset = 0;

	Settings.BBSetup[output].System = BBSetup[output].System = system;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( output == BBOutput1)
	{
		PALLED = BB1PALLED;
		NTSCLED = BB1NTSCLED;
	}
	else
	{
		if ( output == BBOutput2)
		{
			PALLED = BB2PALLED;
			NTSCLED = BB2NTSCLED;
		}
		else
		{
			PALLED = BB3PALLED;
			NTSCLED = BB3NTSCLED;
		}
	}

	LEDSet( PALLED, system < NTSC);
	LEDSet( NTSCLED, system >= NTSC);

	sio0SndCommand( 20, BB_V24_address, "%c%c%lu;", BBOutputCmd[output],
						BBSystemCmd[system], BBSetup[output].Delay);

	tmp = BBSetup[output].ScHPhase;

	if ( tmp < 0)												// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 2048;
	tmp /= 360;													// tmp = ( ScHPhase*2048)/360;
														
	return( sio0SndCommand( 12, BB_V24_address, "%cH%u;",
						 BBOutputCmd[output], ( int) tmp));
}

/**************************************************************************/
/* SetBBDelay																										     BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000731, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetBBDelay( int output, UL delay)
{
	Settings.ActivePreset = 0;

	Settings.BBSetup[output].Delay = BBSetup[output].Delay = delay;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	return( sio0SndCommand( 20, BB_V24_address, "%c%c%lu;", BBOutputCmd[output],
						BBSystemCmd[BBSetup[output].System], delay));
}

/**************************************************************************/
/* SetBBScHPhase																									   BB.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000731, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetBBScHPhase( int output, int schphase)
{
	long tmp = ( long) schphase;

	Settings.ActivePreset = 0;

	Settings.BBSetup[output].ScHPhase = BBSetup[output].ScHPhase = schphase;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( schphase < 0)								// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 2048;
	tmp /= 360;												// tmp = ( ScHPhase*2048)/360;
														
	return( sio0SndCommand( 12, BB_V24_address, "%cH%u;",
						 BBOutputCmd[output], ( int) tmp));
}

