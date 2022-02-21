/***************************************************************************/
/*  Copyright   DK-Audio A/S, 2002                                         */
/*  Project:    PT5202 Digital Video Generator                             */
/*  Module:     MNUINTER.C                                                 */
/*  Purpse:     Interface between 5201 LCD/Keyb menu system and 5201       */
/*  Author:     JK                                                         */
/*  Org. date:  02.05.2002                                                 */
/*  Rev. date:                                                             */
/*  Notes:      This module provides functions to actually set 5201 params */
/*              It contains the following functions:                       */
/*              void do_AnlBlkConfig(BBObject *, UC, UC);
/*              void do_GenlockConfig(GenlockObject *, UC);
/*              void do_SdiConfig(CBObject *, UC);
/*              void do_AnalogConfig(AudioObject *, UC);
/*              void do_AesebuConfig(AudioObject *, UC);
/*              void do_RecallPreset(UC);
/*              void do_StorePreset(UC);
/*              void do_NamePreset(UC, char *);
/*              void do_Calibration(UC);
/*              void do_calibration_save(void);
/*              void do_Audio_Kind(UC);
/*              void do_FactoryReset(UC);
/*              void init_configs(UC *);
/*              which implement updating of genrator parameters            */
/*              using functions found in original 5201 software            */
/*              Functions here are called                                  */
/*              from menutree.c module, which implements the LCD/Keyb.     */
/*              menu system.                                               */
/*                                                                         */
/***************************************************************************/

#include "define.h"
#include <stdio.h>
#include <string.h>
#include "rs232.h"
#include "rs232cmd.h"
#include "instru.h"
#include "nv_drv.h"
#include "bb.h"
#include "cbar.h"
#include "audio.h"
#include "menutree.h"
#include "mnuinter.h"
#include "nv_drv.h"


code char AUTHOR[]        = "NONE";
code char DATE[]          = "01-01-01";

extern UC             ActivePresetNo; // Currently active preset, (0 = empty)

/* existing structures to hold orinal settings, holding current 5201 settings */
extern BBObject       BBSetup[3];
extern CBObject       CBSetup;
extern AudioObject    AudioSetup[2];
extern GenlockObject  GenlockSetup;

/* temporary object to hold original settings in order to restore them in case
   ESC is commanded                                                             */
extern BBObject       BBOrig[3];
extern CBObject       CBOrig;
extern AudioObject    AudioOrig[2];
extern GenlockObject  GenlockOrig;

extern char            OSDTextBuffer[16];


extern NvPointer PresetPtr[NoOfPresets];

/* These two handle copy and cat from code memory */
extern char* Cstrcat(register char* to, register code char* from);
extern char* Cstrcpy(register char* to, register code char* from); 

extern void genlock_adjust(UC);
extern void calibrate_on(void);
extern void calibrate_off(void);
extern void calibrate_store(void);
extern void store_system(int);
  
/* NAME         void do_Calibration(UC which)
 * PURPOSE      To implement the 5201 calibration function
 * ARGUMENTS    UC which: calibration adjustment parameter,
 *              MAX, MIN, UP or DOWN
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        calibrate_store has to be implemented, requires SAVE
 *              function.  
 */  
void do_Calibration(UC which)
  {
  calibrate_on();
  genlock_adjust(which);
  calibrate_off();
  }
 
void do_calibration_save(void)
  {
  calibrate_store();
  }
  

/* NAME         void do_StorePreset(UC which)
 * PURPOSE      To implement the 5201 'store to preset' function
 * ARGUMENTS    UC which: preset number
 *
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_StorePreset(UC which)
  {

  StorePreset(which);
    //sleep( 2);    // This delay is necessaray in order for the instrument to
                                        //  store ALL the variables,
  }

/* NAME         void do_RecallPreset(UC which)
 * PURPOSE      To implement the 5201 'recall preset' function
 * ARGUMENTS    UC which: preset number
 *
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_RecallPreset(UC which, UC * audio_index)
  {
  RecallPreset(which);
  *audio_index = (UC)AudioOutput;
  }

/* NAME         void do_NamePreset(UC which, char * name)
 * PURPOSE      To implement the 5201 'name a preset' function
 * ARGUMENTS    UC which: preset number
 *              char *Name: new name 
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_NamePreset(UC which, char *Name)
  {
  PresetObject prst;

  NV_Recall(&prst, PresetPtr[which], sizeof(PresetObject));
  strcpy(prst.Name, Name);
  NV_Store(&prst, PresetPtr[which], sizeof(PresetObject));
  }
  
/* NAME         void do_AnlBlkConfig(BBObject * data, UC which, UC what)
 * PURPOSE      To implement setting of black burst params
 * ARGUMENTS    BBObject * data - pointer to black burst data stucture
 *              UC which - which BB (0, 1 or 2)
 *              UC what  - what to update (system, timing, ScH)
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
  
void do_AnlBlkConfig(BBObject * data, UC which, UC what)
  {
  switch (what)
    {
    case BBTimingItem:
      SetBBDelay(which, (UL) data->Delay);           
      break;
    case BBSystemItem:
      SetBBSystem(which, data->System);
      break;
    case BBScHPhaseItem:
      SetBBScHPhase(which, data->ScHPhase);
      break;
    }
  }

/* NAME         void do_GenlockConfig(GenlockObject * data, UC what)
 * PURPOSE      To implement setting of genlock params
 * ARGUMENTS    GenlockObject * data - pointer to genlock data stucture
 *              UC what  - what to update (system, timing)
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_GenlockConfig(GenlockObject * data, UC what)
  {
  switch (what)
    {
    case GenlockTimingItem:
      SetGenlockDelay(data->Delay);
      break;
    case GenlockSystemItem:
      SetGenlockSystem(data->System);
      break;
    }
  }

/* NAME         void do_SdiConfig(CBObject * data, UC what)
 * PURPOSE      To implement setting of SDI params
 * ARGUMENTS    CBObject * data - pointer to SDI data stucture
 *              UC what  - what to update (system, timing, etc)
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_SdiConfig(CBObject * data, UC what)
  {
  switch (what) 
    {
    case SdiTimingItem:
      SetCBDelay(data->Delay);
      break;
    case SdiSystemItem:
      SetCBSystem(data->System);
      break;
    case SdiAnalogItem:
      SetCBScHPhase(data->ScHPhase);
      break;
    case SdiPatternItem:
      SetCBPattern(data->Pattern);
      break;
    case SdiEmbeddedItem:
      SetCBEmbAudioSignal(data->EmbAudioSignal);
      break;
    case SdiTextItem:
      SetCBText(data->TextData.Text);
      break;
    case SdiMovementItem:
      SetCBMovement(data->TextData.TextMovement);
      break;
    }
  }

void do_SdiText(UC data)
  {
  SetCBTextOnoff(data);
  }

/* NAME         void do_AnalogConfig(AudioObject * data, UC what)
 * PURPOSE      To implement setting of analog audio params
 * ARGUMENTS    AudioObject * data - pointer to analog audio data stucture
 *              UC what  - what to update (signal, level, etc)
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_AnalogConfig(AudioObject * data, UC what)
  {
  switch (what)
    {
    case AnalogAudioLevelItem:
      SetAudioLevel(AnalogAudio, data->Level);
      break;
    case AnalogAudioSignalItem:
      SetAudioSignal( AnalogAudio, data->Signal);
      break;
    case AnalogAudioClickItem:
      SetAudioClick(AnalogAudio, data->Click);
      break;
    }
  }
/* NAME         void do_AesebuConfig(AudioObject * data, UC what)
 * PURPOSE      To implement setting of digital audio params
 * ARGUMENTS    AudioObject * data - pointer to digital audio data stucture
 *              UC what  - what to update (signal, level, etc)
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_AesebuConfig(AudioObject * data, UC what)
  {
  switch (what)
    {
    case DigitalAudioTimingItem:
      SetAudioTiming(AESEBUAudio, data->Timing);
      break;
    case DigitalAudioLevelItem:
      SetAudioLevel(AESEBUAudio, data->Level);
      break;
    case DigitalAudioSignalItem:
      SetAudioSignal(AESEBUAudio, data->Signal);
      break;
    case DigitalAudioClickItem:
      SetAudioClick(AESEBUAudio, data->Click);
      break;
    case DigitalAudioFrequencyItem:
      SetAudioWordClock(AESEBUAudio, data->Frequency);
      break;
    case DigitalAudioSystemItem:
      SetAudioSystem( AESEBUAudio, data->System);
      break;
    }
  }

/* NAME         void do_Audio_Kind(UC which)
 * PURPOSE      To implement the audio output command
 * ARGUMENTS    UC which  - which audio: analog or AES/EBU
 *              
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_Audio_Kind(UC which)
  {
  SetAudioOutput(which); 
  }

/* NAME         void do_FactoryReset(UC which)
 * PURPOSE      To implement the factory reset command
 * ARGUMENTS    UC which  - which system
 *              
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        
 */  
void do_FactoryReset(UC which)
  {
  store_system(which);
  }

/* NAME         void init_configs(void)
 * PURPOSE      To initialize structures used in menu system
 * ARGUMENTS    None
 *              
 * RETURNS      Nothing
 *
 * EXTERNS      None
 * NOTES        The menu system uses two sets of settings: one which reflects original
                settings of 5202, before menu was used (xxOrigxx) and one used to hold
                present settings commanded by the user (xxSetupxx) using menus.
 */  

void init_configs(UC * audio_index)
  {

  copy_presets();
  // copy text from color bar setup to OSDTextBuffer
  strcpy(OSDTextBuffer, CBSetup.TextData.Text);

  *audio_index = (UC)AudioOutput; //  !!!!!!!!!!!!!! error?
  }



void do_cal_store(void)
  {
  calibrate_store( );
  }

void do_cal_enable_off(void)
  {
  calibrate_off();
  }

void do_cal_enable_on(void)
  {
  calibrate_on();
  }

void send_up(void)
  {
  genlock_adjust(CUP);
  }

void send_down(void)
  {
  genlock_adjust(CDOWN);
  }

void send_max(void)
  {
  genlock_adjust(CMAX);
  }

void send_min(void)
  {
  genlock_adjust(CMIN);
  }

// end of mnuinter.c


#if 0
  UC i;
  
  for (i = 0; i < 3; i++)
    {
    BBOrig[i].System   = BBSetup[i].System;
    BBOrig[i].ScHPhase = BBSetup[i].ScHPhase;
    BBOrig[i].Delay    = BBSetup[i].Delay;
    }

  GenlockOrig.System         = GenlockSetup.System;
  GenlockOrig.Delay          = GenlockSetup.Delay;


  //SRAMVideoTextLoad();


  CBOrig.System              = CBSetup.System;
  CBOrig.Delay               = CBSetup.Delay;
  CBOrig.Pattern             = CBSetup.Pattern;
  CBOrig.EmbAudioSignal      = CBSetup.EmbAudioSignal;
  CBOrig.ScHPhase            = CBSetup.ScHPhase;

  CBOrig.TextData.TextEnable = CBSetup.TextData.TextEnable;
  strcpy(CBOrig.TextData.Text, CBSetup.TextData.Text);
  CBOrig.TextData.TextMovement = CBSetup.TextData.TextMovement;

  
  AudioOrig[AESEBUAudio].System        = AudioSetup[AESEBUAudio].System; // genlock values used here, PAL = 1, NTSC = 2
  AudioOrig[AESEBUAudio].Timing        = AudioSetup[AESEBUAudio].Timing; 
  AudioOrig[AESEBUAudio].Click         = AudioSetup[AESEBUAudio].Click;
  AudioOrig[AESEBUAudio].Frequency     = AudioSetup[AESEBUAudio].Frequency;
  AudioOrig[AESEBUAudio].Signal        = AudioSetup[AESEBUAudio].Signal;
  AudioOrig[AESEBUAudio].Level         = AudioSetup[AESEBUAudio].Level;

/**************************************************************************/
  AudioOrig[AnalogAudio].Click         = AudioSetup[AnalogAudio].Click;
  AudioOrig[AnalogAudio].Signal        = AudioSetup[AnalogAudio].Signal;
  AudioOrig[AnalogAudio].Level         = AudioSetup[AnalogAudio].Level;
/**************************************************************************/
#endif

