/****************************************************************************/
/* MODULE:                                                                  */
/*  instru.c - Instrument control task                                      */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
    void instru(void);
/*                                                                          */
/* NOTES:                                                                   */
/*  Receives command messages form user control tasks, processes them and   */                               
/*  replies according to command.                                           */
/*                                                                          */
/* EDITOR:                                                                  */
/*  Tasking tabs 3 5																												*/
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "eventdef.h"
#include "rtxcobj.h"
#include "rs232.h"
#include "sio0drv.h"
#include "tim0drv.h"
#include "i2c_drv.h"
#include "tvp6kdrv.h"
#include "nv_drv.h"
#include "led_hw.h"
#include "genlock.h"
#include "bb.h"
#include "cbar.h"
#include "audio.h"
#include "keyscan.h"
#include "front.h"
#include "unitprg.h"

#include "instru.h"
#include "interp.h"

TTimer*	 InstruTimer;
TThread* InstrumentControl;
TThread* InterpreterThread;
TThread* KeyScanThread;
TThread* FrontThread;

NvPointer SettingsPtr;
NvPointer PresetPtr[NoOfPresets];
NvPointer CalibrationPtr;

SettingsObject Settings;
CalibrationObject Calibration;

/**************************************************************************/
/* Instrument_Control_Task																		   INSTRU.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000321																		*/
/* Revised:		000914, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void Instrument_Control_Task( void)
{
	/* VAR */
 	bool Test1LEDFlag = FALSE;
	char result[10], i;
	int tmp;
	UC status;

  /* BEGIN */
	UnitPgmMode = 0;						// IMPORTANT!!! see module UNITPRG.C

  rs232_init();				  			// init the rs232 driver
  sio0_init();				 	 			// init the sio0 driver
	tim0_init();								// init the T0 counter for field
  InitBus( IIC_PTV);					// init the IIC bus
	tvp6k_init();								// init the TI TVP6000 Multimedie chip

	NV_Init();									// init the NV RAM

															// init the calibraion values
	CalibrationPtr = NV_AllocP( sizeof( CalibrationObject));

	if ( NV_Recall( &Calibration, CalibrationPtr, sizeof( Calibration)) == FAIL)
	{
		SystemsLEDSet( ErrorLEDs, 0);

		Sleep( 1000);
		// error reading calibration values
	}

															// init the presets
	for ( i = 0; i < NoOfPresets; i++)
		PresetPtr[i] = NV_AllocP( sizeof( PresetObject));

															// init the last selected values
	SettingsPtr = NV_AllocP( sizeof( SettingsObject));

	if ( NV_Recall( &Settings, SettingsPtr, sizeof( Settings)) == FAIL)
		ResetInstrument();
	else
	{
																// NOTE!! The Init()-routines WILL clear
																//  Settings.ActivePreset!!!!!!!!!!
		tmp = Settings.ActivePreset;

		GenlockInit();	 						// restore the genlock settings.
		CBInit();										// restore the colorbar generator settings
		BBInit();										// restore the black burst settings
		AudioInit();								// restore the audio settings

		GenlockUpdate();	 			 		// update the genlock setup.
		CBUpdate();							 		// update the colorbar generator (PAL,EBU ColorBar)
		BBUpdate();							 		// update the black burst setups
		AudioUpdate();					 		// update the audio setup

		Settings.ActivePreset = tmp;

		if ( Settings.ActivePreset)
			RecallPreset( tmp);
	}


  KeyScanThread = newTThread( 3, keyscan);
	FrontThread = newTThread( 4, front);
	InterpreterThread = newTThread( 5, Interpreter);

  KeyScanThread->Execute( KeyScanThread);
	FrontThread->Execute( FrontThread);
  InterpreterThread->Execute( InterpreterThread);

  InstruTimer = newTTimer();
  InstruTimer->SetPeriod( InstruTimer, 250);

  for(;;)
	{
		InstruTimer->TimeOut->Wait( InstruTimer->TimeOut, 0);
		
		if ( !GenlockSetup.System == Internal)
		{
			sio0SndRequest( 12, SPG_V24_address, result, "GS?");

			status = (UC) atoi( result);

 			if ( GenlockSetup.System == MHzLock10)
				LEDSet( GenlockLED, ( status & 0x02) > 0);
			else
				LEDSet( GenlockLED, ( status & 0x01) > 0);
		}

		Test1LEDFlag ^= 1;
		TestLEDSet( Test1LED, Test1LEDFlag);
  }
}

/**************************************************************************/
/* ResetInstrument																						   INSTRU.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000411																		*/
/* Revised:		000914, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int ResetInstrument( void)
{
	BBDefaults();				 			// 
	CBDefaults();				 			//
	AudioDefaults();				 	//
	GenlockDefaults();				// 

	BBInit();						 			// init the black burst setups
	CBInit();						 			// init the colorbar generator (PAL,EBU ColorBar)
	AudioInit();				 			// init the audio setup, (this includes an AudioTimer)
	GenlockInit();	 		 			// init the genlock setup.

	GenlockUpdate();	 		 		// update the genlock setup.
	CBUpdate();						 		// update the colorbar generator (PAL,EBU ColorBar)
	BBUpdate();						 		// update the black burst setups
	AudioUpdate();				 		// update the audio setup

	Settings.ActivePreset = 0;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	return( OK);
}

/**************************************************************************/
/* StorePreset																								   INSTRU.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000411																		*/
/* Revised:		000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int StorePreset( int no)
{
	int i;
	PresetObject Preset;

	if (( no == 0) || ( no > NoOfPresets))
		return( FAIL);

	// Copy genlock setup to preset
	memcpy( &Preset.GenlockSetup, &GenlockSetup, sizeof( GenlockSetup));

	// Copy black burst setup to preset
	for ( i = 0; i < NoOfBBOutputs; i++)
		memcpy( &Preset.BBSetup[i], &BBSetup[i], sizeof( BBSetup[0]));

	// Copy Colorbar setup to preset
	memcpy( &Preset.CBSetup, &CBSetup, sizeof( CBSetup));

	// Copy currently selected audio output to preset
	Preset.AudioOutput = AudioOutput;

	// Copy audio setup to preset
	for ( i = 0; i < NoOfAudioOutputs; i++)
		memcpy( &Preset.AudioSetup[i], &AudioSetup[i], sizeof( AudioSetup[0]));

	// Store preset in setected preset number
	NV_Store( &Preset, PresetPtr[no-1], sizeof( Preset));

	Settings.ActivePreset = no;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	return( OK);
}

/**************************************************************************/
/* RecallPreset																								   INSTRU.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000411																		*/
/* Revised:		000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int RecallPreset( int no)
{
	int i;
	PresetObject Preset;

	if (( no == 0) || ( no > NoOfPresets))
		return( FAIL);

	if ( NV_Recall( &Preset, PresetPtr[no-1], sizeof( Preset)))
	{
		// Recall genlock setup from preset
		memcpy( &GenlockSetup, &Preset.GenlockSetup, sizeof( GenlockSetup));

		GenlockUpdate();		// update the genlock setup


		// Recall black burst setup from preset
		for ( i = 0; i < NoOfBBOutputs; i++)
			memcpy( &BBSetup[i], &Preset.BBSetup[i], sizeof( BBSetup[0]));

		BBUpdate();					// update the black burst setups


		// Recall Colorbar setup from preset
		memcpy( &CBSetup, &Preset.CBSetup, sizeof( CBSetup));

		CBUpdate();					// update the colorbar generator (PAL,EBU ColorBar)


		// Recall currently selected audio output from preset
		AudioOutput = Preset.AudioOutput;

		// Recall audio setup from preset
		for ( i = 0; i < NoOfAudioOutputs; i++)
			memcpy( &AudioSetup[i], &Preset.AudioSetup[i], sizeof( AudioSetup[0]));
	
		SetAudioOutput( AudioOutput);		// update the audio setup

		Settings.ActivePreset = no;
		NV_Store( &Settings, SettingsPtr, sizeof( Settings));

		return( OK);
	}

	return( FAIL);
}


















