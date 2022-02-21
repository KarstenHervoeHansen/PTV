/****************************************************************************/
/* MODULE:                                                                  */
/*  audio.c - Audio 									                                 	    */
/****************************************************************************/
/* FUNCTIONS:                                                               */
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

#include "define.h"
#include "audio.h"
#include "led_hw.h"
#include "tables.h"
														
#define AUDIOMODE			0x000F
#define CLICKPERIOD		0x0100

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

int AudioOutput;
AudioObject AudioSetup[2];

volatile UI AudioModePort _at(0x70000); 				// Port defined in PLD
volatile UI AudioLevelPort _at(0x70002); 				// Port defined in PLD
volatile UI AudioTimingPort _at(0x70004); 			// Port defined in PLD
volatile UI AudioControlPort _at(0x70006); 			// Port defined in PLD

static UI AudioMode;
static UI AudioLevel;
static UI AudioTiming;
static UI AudioControl;

/**************************************************************************/
/* Write_AD8402																								 	  AUDIO.C */
/*																																				*/
/* Author:	 Peter Frederiksen/Kim Engedahl, DEV, 000329									*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 The Altera PLD V8 should be loaded before calling this 			*/
/*						function.		 																								*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int Write_AD8402( UC potmeter, UC channel, UC data)
{
	UI d, n;

	if ( channel > 1)
		return( FAIL);

	AudioControl |= ( POT_CLK | POT_SDI | POT_CSAU | POT_CSCB);

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
/* AudioInit																									 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int AudioInit( void)
{	
	AudioMode = 1;					// AES/EBU, 1000Hz, 48kHz, click 3 second
	AudioLevel = 0x0018;		// 0dBFS(AES/EBU), 0dBm(ANALOG);
	AudioTiming = 0;
	AudioControl = 0;				// PAL Lock

	AudioOutput = AESEBUAudio;

	AudioSetup[AESEBUAudio].System = PAL;
	AudioSetup[AESEBUAudio].Signal = S1kHz;
	AudioSetup[AESEBUAudio].Level = 0;
	AudioSetup[AESEBUAudio].Timing = 0;
	AudioSetup[AESEBUAudio].Click = click3s;
	AudioSetup[AESEBUAudio].Frequency = f48kHz;

	AudioSetup[AnalogAudio].Signal = S1kHz;
	AudioSetup[AnalogAudio].Level = 0;
	AudioSetup[AnalogAudio].Click = click3s;

	return( OK);
}

/**************************************************************************/
/* AudioStatus																								 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int AudioStatus( void)
{

	return( OK);
}

/**************************************************************************/
/* SetAudioOutput																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioOutput( int output)
{
	switch ( output)
	{
		case AnalogAudio:
			if ( AudioOutput == AnalogAudio)
				return( OK);												// Already analog mode
				
			AudioMode = 8;												// Analog signal offset
			break;

		case AESEBUAudio:
			if ( AudioOutput == AESEBUAudio)
				return( OK);												// Already AESEBU mode

			if ( AudioSetup[AESEBUAudio].Frequency == f441kHz)
				AudioMode = 4;
			else
				AudioMode = 0;
			break;

		default:
			return( FAIL);
	}

	AudioMode += AudioSetup[output].Signal;
	AudioMode += 0x100*AudioSetup[output].Click;

	AudioOutput = output;

	AudioModePort = AudioMode;

	LEDSet( AnalogAudioLED, output == AnalogAudio);
	LEDSet( AESEBUAudioLED, output == AESEBUAudio);

	return( OK);
}

/**************************************************************************/
/* SetAudioSystem																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioSystem( int system)
{
	if ( AudioOutput != AESEBUAudio)
		return( FAIL);

	if ( system == NTSC)
		AudioControl |= NTSCPHASE;
	else
		AudioControl &= ~NTSCPHASE;

	AudioSetup[AESEBUAudio].System = system;

	AudioControlPort = AudioControl;

	return( OK);
}

/**************************************************************************/
/* SetAudioSignal																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioSignal( int signal)
{
	int tmp;

	AudioSetup[AudioOutput].Signal = signal;

	if ( AudioOutput == AnalogAudio)
		AudioMode = 8;												// Analog signal offset
	else
	{
		if ( AudioSetup[AESEBUAudio].Frequency == f441kHz)
			AudioMode = 4;
		else
			AudioMode = 0;
	}

	AudioMode += AudioSetup[AudioOutput].Signal;
	AudioMode += 0x100*AudioSetup[AudioOutput].Click;

	AudioModePort = AudioMode;

	return( OK);
}

/**************************************************************************/
/* SetAudioLevel																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000402, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioLevel( int level)
{
	AudioSetup[AudioOutput].Level = level;

	return( OK);
}

/**************************************************************************/
/* SetAudioTiming																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000402, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioTiming( int timing)
{
	AudioSetup[AudioOutput].Timing = timing;

	return( OK);
}

/**************************************************************************/
/* SetAudioWordClock																					 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 WordClock and audio mode is integrated												*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioWordClock( int frequency)
{
	if ( AudioOutput != AESEBUAudio)
		return( FAIL);

	switch ( frequency)
	{
		case f441kHz:
			if ( AudioSetup[AudioOutput].Signal >= AESEBU500HZ441KHZ )
				return( OK);									// Already 44.1kHz

			AudioMode += 4;
			break;

		case f48kHz:
			if ( AudioSetup[AudioOutput].Signal <= AESEBU8000HZ48KHZ)
				return( OK);									// Already 48kHz

			AudioMode -= 4;
			break;

		default:
			return( FAIL);
	}

	AudioSetup[AudioOutput].Frequency = frequency;

	AudioModePort = AudioMode;

	LEDSet( WordClk441LED, frequency == f441kHz);
	LEDSet( WordClk48LED, frequency == f48kHz);

	return( OK);
}

/**************************************************************************/
/* SetAudioClick																							 	  AUDIO.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000402																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetAudioClick( int click)
{
	switch ( click)
	{
		case click3s:
			AudioMode &= ~CLICKPERIOD;
			break;

		case click1s:
			AudioMode |= CLICKPERIOD;
			break;

		default:
			return( FAIL);
	}

	AudioSetup[AudioOutput].Click = click;

	AudioModePort = AudioMode;

	return( OK);
}
