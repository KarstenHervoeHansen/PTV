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
													CBarGrey75,	CBarRed75, Red75, Multiburst, Window10,\
													Window15, Window20, Window100, BlWh15kHz, White100,\
													BlackBurst, CheckField,	DigitalGrey, Stair5, Stair10,\
													Crosshatch, PLUGE };
	
enum EmbAudioSignals		{ EmbAudioOff = 0, EmbAudioStereo1kHz };

typedef struct {
	UC System;
	UI Pattern;
	UL Delay;
	int ScHPhase;
	UC EmbAudioSignal;
} CBObject;

extern CBObject CBSetup;

extern void BBSPGReset( bool SPGReset, bool BBReset, bool SPGPrg);
extern int SRAMVideoLoad( int signal, UC bank);
extern void CBInit( void);
extern void CBS1S0Lines( void);

extern int SetCBPattern( UI pattern);
extern int SetCBSystem( UC system);
extern int SetCBDelay( UL delay);
extern int SetCBScHPhase( int schphase);
extern int SetCBEmbAudioSignal( UC signal);

extern int ValidCBPattern( UC system, UI *pattern);

#endif