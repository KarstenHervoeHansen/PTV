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

extern int WriteCBGain( UC gain);
extern int WriteCBChromaGain( UC gain);

extern void CBDefaults( void);
extern int CBInit( void);
extern int CBUpdate( void);

extern int SetCBPattern( UI pattern);
extern int SetCBSystem( UC system);
extern int SetCBDelay( UL delay);
extern int SetCBScHPhase( int schphase);
extern int SetCBEmbAudioSignal( UC signal);

#endif