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

enum AudioOutputType			{	AnalogAudio = 0, AESEBUAudio };

enum AudioSystem					{	AudioPAL = 0,	AudioNTSC };

enum AudioToneType 				{	S500Hz = 0,	S1kHz, SEBU1kHz, S8kHz };

enum AESEBULevelType			{	M0dBFS = 0,	M9dBFS,	M12dBFS, M15dBFS, M16dBFS,\
														M18dBFS, M21dBFS, AESEBUSilence };

enum AnalogLevelType			{	P6dBm = 0, P4dBm,	P2dBm, M0dBm, M2dBm, M4dBm, M6dBm,\
														M8dBm, M10dBm, M12dBm, M14dBm, M16dBm, M18dBm, M20dBm,\
														M22dBm, M24dBm, M26dBm, M28dBm, M30dBm, M32dBm, M34dBm,\
														M36dBm, M38dBm, M40dBm, M42dBm, M44dBm, M46dBm, M48dBm,\
														M50dBm,	M52dBm,	M54dBm,	AnalogSilence };

enum AudioFreqType				{	f441kHz = 0, f48kHz };

enum AudioClickType 			{	click1s = 1, click3s = 3 };

enum PotMeterTypes				{	CBPotMeter = 0,	AudioPotMeter };

typedef struct {
	UC System;
	UC Signal;
	UC Level;
	UL Timing;
	UC Frequency;
	UC Click;
} AudioObject;

extern AudioOutput;
extern AudioObject AudioSetup[2];

extern int Write_AD8402( UC potmeter, UC channel, UC data);

extern int AudioInit( void);
extern int AudioStatus( void);

extern int SetAudioOutput( int output);
extern int SetAudioSystem( int system);
extern int SetAudioSignal( int signal);
extern int SetAudioLevel( int level);
extern int SetAudioTiming( int timing);
extern int SetAudioWordClock( int freqquency);
extern int SetAudioClick( int click);

#endif