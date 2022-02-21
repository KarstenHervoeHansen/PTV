/****************************************************************************/
/* MODULE:                                                                  */
/*   instru.h - structures and enums for instrument control task            */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
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


#ifndef _INSTRU_H
#define _INSTRU_H

#ifndef NvPointer
#include "nv_drv.h"
#endif

#ifndef GenlockSetup
#include "genlock.h"
#endif

#ifndef BBSetup
#include "bb.h"
#endif

#ifndef CBSetup
#include "cbar.h"
#endif

#ifndef AudioSetup
#include "audio.h"
#endif

#define NoOfPresets							4

#define MaxPresetNameLength			16+1
#define MaxPresetAuthorLength		6+1

typedef struct
{
	char											Company[33];
	char											Type[33];
	char											KUNumber[9];
	char											SWRevision[9];
	char											NC12[13];
	UC												ResetSystem;
	UL												ProdDate;
	UL												UpdateDate;
	GenlockCalibrationObject	GenlockCalibration;
	BBCalibrationObject				BBCalibration[NoOfBBOutputs];
	CBCalibrationObject				CBCalibration;
	AudioCalibrationObject		AudioCalibration;
} CalibrationObject;

typedef struct
{
	char					Name[MaxPresetNameLength];
	char					Author[MaxPresetAuthorLength];
	UL						Date;
	GenlockObject GenlockSetup;
	BBObject 			BBSetup[NoOfBBOutputs];
	CBObject 			CBSetup;
	int 					AudioOutput;
	AudioObject 	AudioSetup[NoOfAudioOutputs];
} PresetObject;

typedef struct
{
	int						ActivePreset;
	GenlockObject GenlockSetup;
	BBObject 			BBSetup[NoOfBBOutputs];
	CBObject 			CBSetup;
	int 					AudioOutput;
	AudioObject 	AudioSetup[NoOfAudioOutputs];
} SettingsObject;

extern void Instrument_Control_Task( void);
extern TThread *InstrumentControl;

extern int ResetInstrument( void);
extern int StorePreset( int no);
extern int RecallPreset( int no);

extern NvPointer PresetPtr[NoOfPresets];
extern NvPointer SettingsPtr;
extern NvPointer CalibrationPtr;

extern SettingsObject Settings;
extern CalibrationObject Calibration;

#endif