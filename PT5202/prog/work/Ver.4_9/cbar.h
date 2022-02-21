/****************************************************************************/
/* MODULE:                                                                  */
/*   cbar.h - ColorBar generator																					  */
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

#ifndef _CBAR_H
#define _CBAR_H

enum CBPatterns					{	SMPTECBar = 0, EBUCBar,	FCCCBar, EBUCBarITU, CBar100,\
													CBarGrey75,	CBarRed75, Red75, Multiburst, CCIR18,\
													Window10, Window15, Window20, Window100, BlWh15kHz,\
													White100, Black, CheckField,	DigitalGrey,\
													Stair5, Stair10, Crosshatch, PLUGE };
	
enum EmbAudioSignals		{ EmbAudioOff = 0, EmbAudioSilence, EmbAudioStereo1kHz };

typedef struct {
	UL Date;

	UL PhaseG;
	UC ScHPhaseG;

	UL PhaseM;
	UC ScHPhaseM;

	UC GainG;
	UC ChromaGainG;	

	UC GainM;
	UC ChromaGainM;
} CBCalibrationObject;

typedef struct {
	UC System;
	UI Pattern;
	UL Delay;
	int ScHPhase;
	UC EmbAudioSignal;
} CBObject;

extern CBObject CBSetup;

int ValidateCBPattern( UC system, UI *pattern);
int SRAMVideoLoad( UC signal, UC bank);
int SRAMSoundLoad( UC signal);
int WriteCBChromaGain( UC gain);
int WriteCBGain( UC gain);
int ValidateCBPattern( UC system, UI *pattern);
int SetCBEmbAudioSignal( UC signal);
int SetCBScHPhase( int schphase);
int SetCBDelay( UL delay);
int SetCBSystem( UC system);
int SetCBPattern( UI pattern);
int CBUpdate( void);
int CBInit( void);
void CBDefaults( void);
void CBLineInterrupt( void);


#endif