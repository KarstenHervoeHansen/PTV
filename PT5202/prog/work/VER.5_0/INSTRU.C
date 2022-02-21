/****************************************************************************/
/* MODULE:                                                                  */
/*  instru.c - Instrument control task                                      */
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "define.h"
#include "rs232.h"
#include "sio0drv.h"
#include "tim0drv.h"
#include "iic.h"
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
#include "disp_02.h"

#include "instru.h"
#include "interp.h"
#include "menutree.h"

#define  AUTO_ESC_TIMEOUT             120

#define  AUTO_STATUS_UPDATE_TIMEOUT   120

NvPointer SettingsPtr;
NvPointer PresetPtr[NoOfPresets];
NvPointer CalibrationPtr;

SettingsObject Settings;
CalibrationObject Calibration;

UC GenlockStatus;
UC AutoESCTimer = 0;     // Timer for auto return to status display

code char CalibrationFailTxt[]    = "Calibration Recall Fail...";
code char ResetTxt[]              = "Calibration Recall Fail...";


/**/
extern _near int rec_timeout;
extern UC status_is_displayed;
extern UC rs232_touched;

/**************************************************************************/
/* Instrument_Control_Task                                   INSTRU.C     */
/* Author:              Kim Engedahl, DEV, 000321                         */
/* Revised:             001219, KEn, DEV                                  */
/* Function:                                                              */
/**************************************************************************/
void Instrument_Control_Task(void)
  {
  bool Test1LEDFlag = FALSE;
  char  i;
  int tmp;
  char result[10];

  UnitPgmMode = 0;     // IMPORTANT!!! see module UNITPRG.C

  tim0_init();         // init the T0 counter for field

  tvp6k_init();        // init the TI TVP6000 Multimedie chip
  front_init();
  keyscan_init();
  NV_Init();           // init the NV RAM

                       // init the calibraion values
  CalibrationPtr = NV_AllocP( sizeof( CalibrationObject));

  if ( NV_Recall( &Calibration, CalibrationPtr, sizeof( Calibration)) == FAIL)
    {
    WriteCodeLN1(0, CalibrationFailTxt);
    ClearRestLN1();
    SystemsLEDSet( ErrorLEDs, 0);
    waitms( 1000);
    // error reading calibration values
    }


  // init the presets
  for ( i = 0; i < NoOfPresets; i++)
    PresetPtr[i] = NV_AllocP( sizeof( PresetObject));

  // init the last selected values
  SettingsPtr = NV_AllocP( sizeof( SettingsObject));


  if ( NV_Recall( &Settings, SettingsPtr, sizeof( Settings)) == FAIL)
    {
    WriteCodeLN1(0, ResetTxt);
    ClearRestLN1();
    ResetInstrument();
    }
  else
    {
    // NOTE!! The Init()-routines WILL clear
    //  Settings.ActivePreset!!!!!!!!!!

    tmp = Settings.ActivePreset;

    GenlockInit();     // restore the genlock settings.
    CBInit();          // restore the colorbar generator settings
    BBInit();          // restore the black burst settings
    AudioInit();       // restore the audio settings

    GenlockUpdate();   // update the genlock setup.
    CBUpdate();        // update the colorbar generator (PAL,EBU ColorBar)
    BBUpdate();        // update the black burst setups
    AudioUpdate();     // update the audio setup

    Settings.ActivePreset = tmp;

    if ( Settings.ActivePreset)
      RecallPreset( tmp);
    }

  /* rec_timeout is general timer variable */
  rec_timeout = 7812;    // This is decremented in CBLineInterrupt each 64 uS
  menu_init();  // init menu system: menutree.c

	/* MAIN loop start */
	for (;;)
    {
    check_keys();      // check if key pressed on LCD display/keypad
    Interpreter();     // check for remote command
    keyscan();         // check if reset button pressed
    if (rec_timeout < 0)
      {
      sio0SndRequest(12, SPG_V24_address, result, "GS?");
      GenlockStatus = (UC) atoi(result);
      if (GenlockSetup.System == MHzLock10)
        GenlockStatus &= 0x02;
      else
        GenlockStatus &= 0x01;
      LEDSet(GenlockLED, GenlockStatus > 0);
      Test1LEDFlag ^= 1;                      /* Togge L28 ON/OFF */
      TestLEDSet(Test1LED, Test1LEDFlag);
      rec_timeout = 7812;
      AutoESCTimer++;
      // if status is displayed, then display must be updated each second
#if 1
      if ((AutoESCTimer % 2) == 0)
        update_status();
      // AutoESCTimer is used to display status on LCD after key have not
      // been touched during 2 minutes
      if (AutoESCTimer > AUTO_ESC_TIMEOUT)
        {
        ret2status();
        AutoESCTimer = 0;
        }
      if (rs232_touched)
        {
        if(!status_is_displayed)
          {
          rs232_touched = 0;
          ret2status();
          AutoESCTimer = 0;
          }
        }
#endif
      }
    }
  }
  

/**************************************************************************/
/* ResetInstrument                                           INSTRU.C     */
/* Author:              Kim Engedahl, DEV, 000411                         */
/* Revised:             000914, KEn, DEV                                  */
/* Function:                                                              */
/**************************************************************************/
int ResetInstrument( void)
  {
  BBDefaults();         
  CBDefaults();         
  AudioDefaults();          
  GenlockDefaults();

  BBInit();                // init the black burst setups
  CBInit();                // init the colorbar generator (PAL,EBU ColorBar)
  AudioInit();             // init the audio setup, (this includes an AudioTimer)
  GenlockInit();  // Calibration.Genl -> SPG

  GenlockUpdate(); // Set Genlock System, Delay
  CBUpdate();              // update the colorbar generator (PAL,EBU ColorBar)
  BBUpdate();              // update the black burst setups
  AudioUpdate();           // update the audio setup

  Settings.ActivePreset = 0;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));
  menu_init();  // init menu system: menutree.c  
  
  return( OK);
  }

/**************************************************************************/
/* StorePreset                                               INSTRU.C     */
/* Author:              Kim Engedahl, DEV, 000411                         */
/* Revised:             000618, KEn, DEV                                  */
/* Function:                                                              */
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
/* RecallPreset                                              INSTRU.C     */
/* Author:              Kim Engedahl, DEV, 000411                         */
/* Revised:             000618, KEn, DEV                                  */
/* Function:                                                              */
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

    GenlockUpdate();                // update the genlock setup


    // Recall black burst setup from preset
    for ( i = 0; i < NoOfBBOutputs; i++)
      memcpy( &BBSetup[i], &Preset.BBSetup[i], sizeof( BBSetup[0]));

    BBUpdate();   // update the black burst setups


    // Recall Colorbar setup from preset
    memcpy( &CBSetup, &Preset.CBSetup, sizeof( CBSetup));

    CBUpdate();   // update the colorbar generator (PAL,EBU ColorBar)

    // Recall currently selected audio output from preset
    AudioOutput = Preset.AudioOutput;

    // Recall audio setup from preset
    for ( i = 0; i < NoOfAudioOutputs; i++)
      memcpy( &AudioSetup[i], &Preset.AudioSetup[i], sizeof( AudioSetup[0]));

    SetAudioOutput( AudioOutput);  // update the audio setup

    Settings.ActivePreset = no;
    NV_Store( &Settings, SettingsPtr, sizeof( Settings));

    return( OK);
    }
  return( FAIL);
  }
