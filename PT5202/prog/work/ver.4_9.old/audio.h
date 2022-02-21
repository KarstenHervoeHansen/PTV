/****************************************************************************/
/* MODULE:                                                                  */
/*   audio.h - Audio																											  */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*  NONE                                                                    */
/*                                                                          */
/* TASKS:                                                                   */
/*  NONE                                                                    */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#ifndef _AUDIO_H
#define _AUDIO_H

#define NoOfAudioOutputs	2

enum AudioOutputType			{	AnalogAudio = 0, AESEBUAudio };

enum AudioSystem					{	AudioPAL = 0,	AudioNTSC };

enum AudioToneType 				{	S500Hz = 0,	S1kHz, SEBU1kHz, S8kHz };

enum AESEBULevelType			{	M0dBFS = 0,	M9dBFS,	M12dBFS, M15dBFS, M16dBFS,\
														M18dBFS, M20dBFS, AESEBUSilence };

enum AnalogLevelType			{	P12dBm = 0, P10dBm,	P8dBm, P7dBm, P6dBm, P5dBm, P4dBm,\
														P3dBm, P2dBm, P1dBm, M0dBm, M1dBm, M2dBm, M3dBm, M4dBm,\
														M5dBm, M6dBm, M7dBm, M8dBm, M9dBm, M10dBm, M11dBm,\
														M12dBm, M15dBm, M18dBm, M20dBm, M24dBm, M27dBm, M30dBm,\
														M33dBm, M36dBm, AnalogSilence };

enum AudioFreqType				{	f441kHz = 0, f48kHz };

enum AudioClickType 			{	click1s = 1, click3s = 3 };

enum PotMeterTypes				{	CBPotMeter = 0,	AudioPotMeter };

enum AnalogAudioChannels	{	AnlAudioLeftChannel = 0,	AnlAudioRightChannel };

typedef struct {
	UL Date;
	UC AnalogLeftChannel;
	UC AnalogRightChannel;
} AudioCalibrationObject;

typedef struct {
	int System;
	int Signal;
	int Level;
	float Timing;
	int Frequency;
 	int Click;
} AudioObject;

extern int AudioOutput;
extern AudioObject AudioSetup[NoOfAudioOutputs];

extern int Write_AD8402( UC potmeter, UC channel, UC data);

extern int WriteAnlAudioDACLevel( UC left, UC right);

extern void AudioDefaults( void);
extern int AudioInit( void);
extern int AudioUpdate( void);
extern UC AudioStatus( void);

extern int SetAudioOutput( int output);
extern int SetAudioSystem( int output, int system);
extern int SetAudioSignal( int output, int signal);
extern int SetAudioLevel( int output, int level);
extern int SetAudioTiming( int output, float timing);
extern int SetAudioWordClock( int output, int freqquency);
extern int SetAudioClick( int output, int click);

#endif