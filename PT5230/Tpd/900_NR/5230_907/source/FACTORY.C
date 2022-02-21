/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 2000						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		FACTORY.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980121																		*/
/*	Rev. date:	000515, KEn, DEV															*/
/*	Status:		Version 1.3																	*/
/*																									*/
/*	This module contains NO functions:													*/
/*																									*/
/*	Changes:																						*/
/* 000508: Added support for text positioning										*/
/* 991025: "Master reset" is no onger supported										*/
/* 980917: Reset of analog TPG altered.												*/
/* 980708: Altered reset of Time Clock Interface.									*/
/* 980702: TimeClockConfig resets to VITCTimeInput instead of LTCTimeInput	*/
/* 980616: Released as 4008 002 06812.													*/
/* 980514: Released as 4008 002 06811.													*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <stdlib.h>
#include <string.h>
#include <xa.h>

#include "mas.h"
#include "factory.h"
#include "tables.h"
#include "cont_drv.h"
#include "xaexprt.h"
#include "menutree.h"
#include "util.h"
#include "iic.h"
#include "rs232err.h"

code ANLBLK_STRUCT AnlBlkConfigFactory[MaxAnlBlkUnits] = {
	{ PAL, 0, 0},						// Black Burst 1
	{ PAL, 0, 0},						// Black Burst 2
	{ PAL, 0, 0},						// Black Burst 3
	{ PAL, 0, 0},						// Black Burst 4
	{ PAL, 0, 0},						// Black Burst 5
	{ PAL, 0, 0},						// Black Burst 6
	{ PAL, 0, 0},						// Black Burst 7
	{ PAL, 0, 0}						// Black Burst 8
};

code ANLTPG_STRUCT AnlTPGConfigFactory[MaxAnlTPGUnits] = {
											// ANALOG-TPG2
	{ PAL,
	  AnlEBUCBar,
	  { 3, 19, 0, 0 },
	  { AnlEBUCBar, AnlLumSweep, AnlWindow15,
		 AnlFieldSquareWave, AnlRamp, AnlCrosshatch
	  },
	  0x18,								// Standard text: OFF, Complex text: ON
	  { TPGStandardTextStyle, TPGComplexTextStyle },
	  {{ 1,1}, { 1,1}},
	  "ANALOG1",
	  "ANALOG2",
	  "ANALOG3",
	  "PTV",
	  "PT5230",
	  0,
	  0
	},
											// ANALOG-TPG5
	{ PAL,
	  AnlEBUCBar,
	  { 3, 19, 0, 0 },
	  { AnlEBUCBar, AnlLumSweep, AnlWindow15,
		 AnlFieldSquareWave, AnlRamp, AnlCrosshatch
	  },
	  0x18,								// Standard text: OFF, Complex text: ON
	  { TPGStandardTextStyle, TPGComplexTextStyle },
	  {{ 1,1}, { 1,1}},
	  "ANALOG1",
	  "ANALOG2",
	  "ANALOG3",
	  "PTV",
	  "PT5230",
	  0,
	  0
	}
};

code SDITSG_STRUCT SDITSGConfigFactory[MaxSDITSGUnits] = {
											// SDI-TSG2
	{ SDI625,
	  SDIBlack,
	  { SDIEBUCBar, SDIMultiburst, SDIWindow15,
		 SDICheckField, SDIStair5, SDICrosshatch
	  },
	  OFF,								//EDH insertion
	  TSGAudioOff,
	  TSGAudioSilence,
	  0
	},
											// SDI-TSG3
	{ SDI625,
	  SDIBlack,
	  { SDIEBUCBar, SDIMultiburst, SDIWindow15,
		 SDICheckField, SDIStair5, SDICrosshatch
	  },
	  OFF,								// EDH insertion
	  TSGAudioOff,
	  TSGAudioSilence,
	  0
	},
											// SDI-TSG4
	{ SDI625,
	  SDIBlack,
	  { SDIEBUCBar, SDIMultiburst, SDIWindow15,
		 SDICheckField, SDIStair5, SDICrosshatch
	  },
	  OFF,								//EDH insertion
	  TSGAudioOff,
	  TSGAudioSilence,
	  0
	}
};

code SDITPG_STRUCT SDITPGConfigFactory[MaxSDITPGUnits] = {
											// SDI-TPG1
	{ SDI625,
	  SDIEBUCBar,
	  { 2, 18, 0, 0 },
	  { SDIEBUCBar, SDIMultiburst, SDIWindow15,
		 SDICheckField, SDIShallowRamp, SDICrosshatch
	  },
	  0x18,								// Standard text: OFF, Complex text: ON
	  { TPGStandardTextStyle, TPGComplexTextStyle },
	  {{ 1,1}, { 1,1}},
	  "DIGITAL1",
	  "DIGITAL2",
	  "DIGITAL3",
	  "PTV",
	  "PT5230",
	  OFF,								// EDH insertion
	  TPGAudioOff,
	  TPGAudioSilence,
	  TPGAudioGroup1,
	  0
	},
											// SDI-TPG2
	{ SDI625,
	  SDIEBUCBar,
	  { 2, 18, 0, 0 },
	  { SDIEBUCBar, SDIMultiburst, SDIWindow15,
		 SDICheckField, SDIShallowRamp, SDICrosshatch
	  },
	  0x18,								// Standard text: OFF, Complex text: ON
	  { TPGStandardTextStyle, TPGComplexTextStyle },
	  {{ 1,1}, { 1,1}},
	  "DIGITAL1",
	  "DIGITAL2",
	  "DIGITAL3",
	  "PTV",
	  "PT5230",
	  OFF,								// EDH insertion
	  TPGAudioOff,
	  TPGAudioSilence,
	  TPGAudioGroup1,
	  0
	},
											// SDI-TPG5
	{ SDI625,
	  SDIEBUCBar,
	  { 2, 18, 0, 0 },
	  { SDIEBUCBar, SDIMultiburst, SDIWindow15,
		 SDICheckField, SDIShallowRamp, SDICrosshatch
	  },
	  0x18,								// Standard text: OFF, Complex text: ON
	  { TPGStandardTextStyle, TPGComplexTextStyle },
	  {{ 1,1}, { 1,1}},
	  "DIGITAL1",
	  "DIGITAL2",
	  "DIGITAL3",
	  "PTV",
	  "PT5230",
	  OFF,								// EDH insertion
	  TPGAudioOff,
	  TPGAudioSilence,
	  TPGAudioGroup1,
	  0
	}
};

code AESEBU_STRUCT AESEBUConfigFactory[MaxAESEBUUnits] = {
	{ AESEBUStereo800Hz, AESEBUSilence, AESEBUPAL },	// AES/EBU 1
	{ AESEBUStereo800Hz, AESEBUSilence, AESEBUPAL }		// AES/EBU 2
};

code GENLOCK_STRUCT GenlockConfigFactory[MaxGenlockUnits] = {
	{ BurstLockPAL, 0 },				// Genlock A
	{ BurstLockPAL, 0 },				// Genlock B
	{ BurstLockPAL, 0 },				// Loop through
	{ SDI625Lock, 0 },				// Option PT8607 SDI Genlock
	{ NA, 0 },							// Internal genlock
	{ NA, 0 }							// Option PT8610 Internal OCXO genlock
};

code TIMECLOCK_STRUCT TimeClockConfigFactory = {
	0,										// TCI Control
	0,										// TCI Status
	VITCTimeInput,						// Time reference
	0x980501,							// Date, ( always organized as YY/MM/DD)
	0x080000,							// Time, ( always organized as HH/MM/SS)
	Hour24Format+2*DMYFormat,		//
};

code RS232_STRUCT RS232ConfigFactory = {
	UDR_BAUD_9600,						// Baudrate
	UDR_DATA8,							// Databit
	UDR_NONE,							// Parity
	UDR_RTS_CTS							// Handshake
};

/***************************************************************************/
/* FactoryPreset																  FACTORY.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980214	    										*/
/* Revised:		980509				     													*/
/*																									*/
/*	Function:	Prepare generator for a factory preset.							*/
/*	Remarks:	  	--																				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FactoryPreset() {

	register int i;

	for ( i = 0; i < 16; i++) {
		if ( i % 2 )
			RAMCheckArea[i] = 0xAA;
		else
			RAMCheckArea[i] = 0x55;
	}

	// CONFIGURE PRESET 1 & 3-8 TO PAL

	for ( i = 0; i < MaxAnlBlkUnits; i++) {
		Cmemcpy( &Preset[0].AnlBlkStore[i], &AnlBlkConfigFactory[i], sizeof( AnlBlkConfig[0]));
		Cmemcpy( &AnlBlkConfig[i], &AnlBlkConfigFactory[i], sizeof( AnlBlkConfig[0]));
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxAnlTPGUnits; i++) {
		Cmemcpy( &Preset[0].AnlTPGStore[i], &AnlTPGConfigFactory[i], sizeof( AnlTPGConfig[0]));
		Cmemcpy( &AnlTPGConfig[i], &AnlTPGConfigFactory[i], sizeof( AnlTPGConfig[0]));
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxSDITSGUnits; i++) {
		Cmemcpy( &Preset[0].SDITSGStore[i], &SDITSGConfigFactory[i], sizeof( SDITSGConfig[0]));
		Cmemcpy( &SDITSGConfig[i], &SDITSGConfigFactory[i], sizeof( SDITSGConfig[0]));
	}

	for ( i = 0; i < MaxSDITPGUnits; i++) {
		Cmemcpy( &Preset[0].SDITPGStore[i], &SDITPGConfigFactory[i], sizeof( SDITPGConfig[0]));
		Cmemcpy( &SDITPGConfig[i], &SDITPGConfigFactory[i], sizeof( SDITPGConfig[0]));
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxAESEBUUnits; i++) {
		Cmemcpy( &Preset[0].AESEBUStore[i], &AESEBUConfigFactory[i], sizeof( AESEBUConfig[0]));
		Cmemcpy( &AESEBUConfig[i], &AESEBUConfigFactory[i], sizeof( AESEBUConfig[0]));
	}

	FeedTheWatchdog;

	for ( i = 0; i < MaxGenlockUnits; i++) {
		Cmemcpy( &Preset[0].GenlockStore[i], &GenlockConfigFactory[i], sizeof( GenlockConfig[0]));
		Cmemcpy( &GenlockConfig[i], &GenlockConfigFactory[i], sizeof( GenlockConfig[0]));
	}

	Cmemcpy( &Preset[0].TimeClockStore, &TimeClockConfigFactory, sizeof( TimeClockConfig));
	Cmemcpy( &TimeClockConfig, &TimeClockConfigFactory, sizeof( TimeClockConfig));

	Preset[0].ActiveGenlockNo = ActiveGenlockNo = Internal;
	Preset[0].ExternalGenlockNo = ExternalGenlockNo = GenlockA;

	strcpy( Preset[0].Name, "PRESET  ");

	for ( i = 1; i < NoOfPresets; i++) {
		Preset[0].Name[7] = '1' + i;
		memcpy( &Preset[i], &Preset[0], sizeof( Preset[0]));
	}

	Preset[0].Name[7] = '1';

	// CONFIGURE PRESET 2 TO NTSC, IE. CHANGE ONLY THE DIFFERENCES

	FeedTheWatchdog;

	for ( i = 0; i < MaxAnlBlkUnits; i++)
		Preset[1].AnlBlkStore[i].System = NTSC;

	for ( i = 0; i < MaxAnlTPGUnits; i++) {
		Preset[1].AnlTPGStore[i].System = NTSC;
		Preset[1].AnlTPGStore[i].Pattern = AnlSMPTECBar;
	}

	for ( i = 0; i < MaxSDITSGUnits; i++) {
		Preset[1].SDITSGStore[i].System = SDI525;
		Preset[1].SDITSGStore[i].Pattern = SDISMPTECBar;
	}

	for ( i = 0; i < MaxSDITPGUnits; i++) {
		Preset[1].SDITPGStore[i].System = SDI525;
		Preset[1].SDITPGStore[i].Pattern = SDISMPTECBar;
	}

	for ( i = 0; i < MaxAESEBUUnits; i++)
		Preset[1].AESEBUStore[i].Timing = AESEBUNTSCPhase1;

	for ( i = 0; i < SDIGenlock; i++)
		Preset[1].GenlockStore[i].System = BurstLockNTSC;

	Preset[1].GenlockStore[SDIGenlock].System = SDI525Lock;

	Cmemcpy( &RS232Config, &RS232ConfigFactory, sizeof( RS232Config));

	LCDContrast = DefContrast;

	ActiveOutputNo = AnlBlkItem;		// AnlBlkItem is NOT part of output
	ActivePresetNo = 0;

	Flags.NormalLockOn = OFF;
	Flags.PanelLockOn = OFF;
	Flags.DateTimeLockOn = OFF;
	Flags.DownloadLockOn = OFF;
	Flags.DiagnoseLockOn = OFF;

	Flags.AutoESCEnable = ON;
	Flags.AutoESCToStatus = ON;

	PasswordState = ON;

	IIC2Write2( mainram1_addr, RAMCheckOK_addr, TRUE);

	ResetErrorEventQueue();						// Reset RS232 error queue
}
