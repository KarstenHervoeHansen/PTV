/****************************************************************************/
/* MODULE:                                                                  */
/*  audio.c - Audio 									                                 	    */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES: Audio-Defaults: Levels changed to -18 (PAL) and -20 (NTSC)        */
/* UPDATES: SetAudioClick() call added in SetAudioSignal, as the click      */
/*          rate was updated incorrecly 14.03.02 JK                         */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include "reg8051.h"
#include "define.h"
#include "rtxcobj.h"
#include "audio.h"
#include "led_hw.h"
#include "tables.h"
#include "instru.h"
													
/**************************************************************************/
// The following definitions are based upon the document be PF:
//  "I/O-ports for controlling the audio generator in PT5201."

// Audio Mode Select Port
#define AUDIOMODE			0x000F
#define CLICKPERIOD		0x0100

#define	AESEBU500HZ48KHZ 					0
#define	AESEBU1000HZ48KHZ					1
#define	AESEBU1000HZCLICK48KHZ		2
#define	AESEBU8000HZ48KHZ					3
 
#define	AESEBU500HZ441KHZ					4
#define	AESEBU1000HZ441KHZ				5
#define	AESEBU1000HZCLICK441KHZ		6
#define	AESEBU8000HZ441KHZ				7

#define	ANALOG500HZ								8
#define	ANALOG1000HZ							9
#define	ANALOG1000HZCLICK					10
#define	ANALOG8000HZ							11

#define AESEBUF48KHZOFFSET				0
#define AESEBUF441KHZOFFSET				4
#define ANALOGAUDIOOFFSET					8

// Audio Level Select Port
#define AESEBULEVEL		0x0007
#define ANALOGLEVEL		0x001F
#define AUDIOLEVEL		AESEBULEVEL | ANALOGLEVEL

// Audio Control Select Port
#define POTMETERS			0x000F
#define POT_CLK				0x0001
#define POT_SDI				0x0002
#define POT_CSAU			0x0004
#define POT_CSCB			0x0008
#define SELAESEBU			0x0010
#define SELANALOG			0x0020
#define NTSCPHASE			0x0040
#define RELAYTYPE			0x0080
#define PLLLOCK				0x0100
#define VIDEOLOCK			0x0200

// End of definitions
/**************************************************************************/

code UI TimingValues[] = {
	0x001,	// 0.0us
	0x016,	// 0.8us
	0x02B,  // 1.6us
	0x041, 	// 2.4us
	0x056,	// 3.2us
	0x06C,	// 4.0us
	0x082,  // 4.8us
	0x097, 	// 5.6us
	0x0AD,	// 6.4us
	0x0C2,	// 7.2us
	0x0D8, 	// 8.0us
	0x0EE, 	// 8.8us
	0x103,	// 9.6us
	0x119,	// 10.4us
	0x12E, 	// 11.2us
	0x144, 	// 12.0us
	0x15A,	// 12.8us
	0x16F,	// 13.6us
	0x185, 	// 14.4us
	0x19A, 	// 15.2us
	0x1B0,	// 16.0us
	0x1C6,	// 16.8us
	0x1DB, 	// 17.6us
	0x1F1, 	// 18.4us
	0x206,	// 19.2us
	0x21C		// 20.0us
};

int AudioOutput;
AudioObject AudioSetup[NoOfAudioOutputs];

volatile int AudioModePort _at(0x70000); 				// Port defined in PLD
volatile int AudioLevelPort _at(0x70002); 	 		// Port defined in PLD
volatile int AudioTimingPort _at(0x70004); 			// Port defined in PLD
volatile int AudioControlPort _at(0x70006);  		// Port defined in PLD

static int AudioMode;
static int AudioLevel;
static int AudioTiming;
static int AudioControl;

/**************************************************************************/
/* Write_AD8402																								 	  AUDIO.C */
/*																																				*/
/* Author:	 Peter Frederiksen/Kim Engedahl, DEV, 000329									*/
/* Revised:	 000603, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 The Altera PLD V8 should be loaded before calling this 			*/
/*					 function.		 																								*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int Write_AD8402( UC potmeter, UC channel, UC data)
{
	UI d, n;

	if ( channel > 1)
		return( FAIL);

	switch ( potmeter)
	{
		case CBPotMeter:
			AudioControl &= ~POT_CSCB;
			break;

		case AudioPotMeter:
			AudioControl &= ~POT_CSAU;
			break;

		default:
			return( FAIL);
	}

	AudioControlPort = AudioControl;

	d = ( channel * 0x100) + data;

	for ( n = 0x200; n > 0; n /= 2)
	{
		AudioControl  &= ~POT_CLK;
		AudioControlPort = AudioControl;
		
		if ( d & n)
			AudioControl |= POT_SDI;
		else
			AudioControl &= ~POT_SDI;

		AudioControlPort = AudioControl;

		AudioControl |= POT_CLK;
		AudioControlPort = AudioControl;
	}

	AudioControl |= ( POT_CLK | POT_SDI | POT_CSAU | POT_CSCB);
	AudioControlPort = AudioControl;

	return( OK);
}

/**************************************************************************/
/* WriteAnlAudioDACLevel																			 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000504																		*/
/* Revised:	 000603, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int WriteAnlAudioDACLevel( UC left, UC right)
{	
	Write_AD8402( AudioPotMeter, AnlAudioLeftChannel, left);
	Write_AD8402( AudioPotMeter, AnlAudioRightChannel, right);

	return( OK);
}

/**************************************************************************/
/* AudioDefaults																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000526																		*/
/* Revised:	 000617, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:  Level on Analog and AES Audio changed to SILENCE: JK         */
/* Updates:  Level on Analog and AES Audio changed to -20dBFS and 0dBu: JK*/
/* Updates:  level is -18 for PAL and -20 for NTSC                        */
/**************************************************************************/
void AudioDefaults( void)
  {	
	Settings.AudioOutput = AudioOutput = AESEBUAudio;

	if ( Calibration.ResetSystem < NTSC_US)
	  {
		Settings.AudioSetup[AESEBUAudio].System = AudioSetup[AESEBUAudio].System = AudioPAL;
	  Settings.AudioSetup[AESEBUAudio].Level = AudioSetup[AESEBUAudio].Level = M18dBFS;
		}
	else
	  {
		Settings.AudioSetup[AESEBUAudio].System = AudioSetup[AESEBUAudio].System = AudioNTSC;
	  Settings.AudioSetup[AESEBUAudio].Level = AudioSetup[AESEBUAudio].Level = M20dBFS;
		}

	Settings.AudioSetup[AESEBUAudio].Signal = AudioSetup[AESEBUAudio].Signal = S1kHz;
	Settings.AudioSetup[AESEBUAudio].Timing = AudioSetup[AESEBUAudio].Timing = TimingValues[0];
	Settings.AudioSetup[AESEBUAudio].Click = AudioSetup[AESEBUAudio].Click = click3s;
	Settings.AudioSetup[AESEBUAudio].Frequency = AudioSetup[AESEBUAudio].Frequency = f48kHz;

	Settings.AudioSetup[AnalogAudio].Signal = AudioSetup[AnalogAudio].Signal = S1kHz;
	Settings.AudioSetup[AnalogAudio].Level = AudioSetup[AnalogAudio].Level = M0dBm; 
	Settings.AudioSetup[AnalogAudio].Click = AudioSetup[AnalogAudio].Click = click3s;

	NV_Store( &Settings, SettingsPtr, sizeof( Settings));
}

/**************************************************************************/
/* AudioInit																									 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000617, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int AudioInit( void)
  {	
	WriteAnlAudioDACLevel(Calibration.AudioCalibration.AnalogLeftChannel,
										    Calibration.AudioCalibration.AnalogRightChannel);

	AudioSetup[AESEBUAudio].System = Settings.AudioSetup[AESEBUAudio].System;
	AudioSetup[AESEBUAudio].Signal = Settings.AudioSetup[AESEBUAudio].Signal;
	AudioSetup[AESEBUAudio].Level = Settings.AudioSetup[AESEBUAudio].Level;
	AudioSetup[AESEBUAudio].Timing = Settings.AudioSetup[AESEBUAudio].Timing;
	AudioSetup[AESEBUAudio].Click = Settings.AudioSetup[AESEBUAudio].Click;
	AudioSetup[AESEBUAudio].Frequency = Settings.AudioSetup[AESEBUAudio].Frequency;

	AudioSetup[AnalogAudio].Signal = Settings.AudioSetup[AnalogAudio].Signal;
	AudioSetup[AnalogAudio].Level = Settings.AudioSetup[AnalogAudio].Level;
	AudioSetup[AnalogAudio].Click = Settings.AudioSetup[AnalogAudio].Click;

	// NOTE!! WordClock is NOT defined for the analog audio, hence it is necessary
	//  to ensure that either of the front WordClock LEDs is switched on if
	//  the PT5201 is to start in analog audio mode, i.e. the wordclock LEDs is
	//  updated with the state of the AESEBU audio wordclock even though analog audio
	//  is selected.
	LEDSet(WordClk441LED, AudioSetup[AESEBUAudio].Frequency == f441kHz);	
	LEDSet(WordClk48LED, AudioSetup[AESEBUAudio].Frequency == f48kHz);

	return(0);
  }

/**************************************************************************/
/* AudioUpdate																								 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000628, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int AudioUpdate( void)
  {
 	SetAudioOutput( Settings.AudioOutput);

	return( OK);
  }

/**************************************************************************/
/* AudioStatus																								 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000628, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
UC AudioStatus( void)
{
	return(( AudioControlPort >> 8) && 0xFC);
 }

/**************************************************************************/
/* SetAudioOutput																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioOutput( int output)
  {
	Settings.ActivePreset = 0;

	LEDSet( AnalogAudioLED, output == AnalogAudio);
	LEDSet( AESEBUAudioLED, output == AESEBUAudio);

	Settings.AudioOutput = AudioOutput = output;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( output == AnalogAudio)
		AudioControl |= SELANALOG;			// Select analog output
	else
		AudioControl |= SELAESEBU;			// Select AES/EBU output

	AudioControlPort = AudioControl; 	// Select output mode. The output selection
																	 	//  requires the generation of a 50ms pulse.

	Sleep( 50);											 	// Sleep for 50 ms

	AudioControl &= ~( SELANALOG + SELAESEBU);	// End of 50ms pulse

																				// Update output variable and LEDs
	//LEDSet( AnalogAudioLED, output == AnalogAudio);
	//LEDSet( AESEBUAudioLED, output == AESEBUAudio);

	SetAudioSystem( output, AudioSetup[output].System);
	SetAudioSignal( output, AudioSetup[output].Signal);
	SetAudioLevel( output, AudioSetup[output].Level);
	SetAudioTiming( output, AudioSetup[output].Timing);
	SetAudioWordClock( output, AudioSetup[output].Frequency);
	SetAudioClick( output, AudioSetup[output].Click);

	return( OK);
}

/**************************************************************************/
/* SetAudioSystem																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 Only possible if the selected audio output is AES/EBU				*/
/* Returns:	 FAIL if the selected output is different from AES/EBU,				*/
/*					 otherwise is returns OK.																			*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioSystem( int output, int system)
{
	int tmp = AudioControl;

	if ( output != AESEBUAudio)
		return( FAIL);

	Settings.ActivePreset = 0;

	Settings.AudioSetup[output].System = AudioSetup[output].System = system;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( system == AudioPAL)
		tmp &= ~NTSCPHASE;
	else
		tmp |= NTSCPHASE;

	if ( output == AudioOutput)
	{
		AudioControl = tmp;
		AudioControlPort = AudioControl;
	}

	return( OK);
}

/**************************************************************************/
/* SetAudioSignal																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:	 FAIL if signal is not valid, otherwise OK.										*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioSignal( int output, int signal)
  {
	int tmp;

	if ( signal > S8kHz)
		return( FAIL);

	Settings.ActivePreset = 0;

	Settings.AudioSetup[output].Signal = AudioSetup[output].Signal = signal;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( output == AnalogAudio)
		tmp = ANALOGAUDIOOFFSET;				// Analog signal offset
	else
	  {
		if ( AudioSetup[output].Frequency == f441kHz)
			tmp = AESEBUF441KHZOFFSET;		// AESEBU 44.1kHz signal offset
		else
			tmp = AESEBUF48KHZOFFSET;			// AESEBU 48kHz signal offset
	  }

	tmp += signal;
	tmp += CLICKPERIOD * AudioSetup[output].Click;

	if ( output == AudioOutput)
	  {
		AudioMode = tmp;
		AudioModePort = AudioMode;
	  }
  SetAudioClick(output, Settings.AudioSetup[output].Click);
	return( OK);
  }

/**************************************************************************/
/* SetAudioLevel																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:	 FAIL if level is invalid, otherwise it returns OK.				 		*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioLevel( int output, int level)
{
	if ( output == AESEBUAudio)
	{
		if ( level > AESEBUSilence)
			return( FAIL);
	}
	else
	{
		if ( level > AnalogSilence)
			return( FAIL);
	}

	Settings.ActivePreset = 0;

	Settings.AudioSetup[output].Level = AudioSetup[output].Level = level;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( output == AudioOutput)
	{
		AudioLevel &= ~AUDIOLEVEL;
		AudioLevel = level;
		AudioLevelPort = AudioLevel;
	}

	return( OK);
}

/**************************************************************************/
/* SetAudioTiming																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioTiming( int output, float timing)
{
	int tmp;

	Settings.ActivePreset = 0;

	Settings.AudioSetup[output].Timing = AudioSetup[output].Timing = timing;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( output == AudioOutput)
	{
		tmp = (int)(( timing / 0.8) + 0.5 - ( 1*( timing < 0)));

		if ( tmp < 0)
			tmp += 26;
				
		AudioTiming = TimingValues[tmp];
		AudioTimingPort = AudioTiming;
	}

	return( OK);
}

/**************************************************************************/
/* SetAudioWordClock																					 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 WordClock and audio mode is integrated. This function is 		*/
/*					 only valid for the AES/EBU output.											 			*/
/* Returns:	 FAIL if click and is invalid, otherwise it returns OK.				*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioWordClock( int output, int frequency)
{
	int tmp;

	if ( output != AESEBUAudio)
		return( FAIL);

	Settings.ActivePreset = 0;

	Settings.AudioSetup[output].Frequency = AudioSetup[output].Frequency = frequency;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( frequency == f441kHz)
		tmp = AESEBUF441KHZOFFSET;
	else
		tmp = AESEBUF48KHZOFFSET;

	tmp += AudioSetup[output].Signal;
	tmp += CLICKPERIOD*AudioSetup[output].Click;

	if ( output == AudioOutput)
	{
		AudioMode = tmp;
		AudioModePort = AudioMode;

		LEDSet( WordClk441LED, frequency == f441kHz);	
		LEDSet( WordClk48LED, frequency == f48kHz);
	}

	AudioSetup[AudioOutput].Frequency = frequency;

	return( OK);
}

/**************************************************************************/
/* SetAudioClick																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:  FAIL if audio output is invalid, otherwise it returns OK.		*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioClick( int output, int click)
{
	int tmp = AudioSetup[output].Signal;

	if ( output > AESEBUAudio)
		return( FAIL);

	Settings.ActivePreset = 0;

	Settings.AudioSetup[output].Click = AudioSetup[output].Click = click;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( click == click1s)
		tmp += CLICKPERIOD;

	if ( output == AnalogAudio)
		tmp += ANALOGAUDIOOFFSET;					// Analog signal offset
	else
	{
		if ( AudioSetup[output].Frequency == f441kHz)
			tmp += AESEBUF441KHZOFFSET;			// AESEBU 44.1kHz signal offset
		else
			tmp += AESEBUF48KHZOFFSET;	 		// AESEBU 48kHz signal offset
	}

	if ( output == AudioOutput)
	{
		AudioMode = tmp;
		AudioModePort = AudioMode;
	}

	return( OK);
}

