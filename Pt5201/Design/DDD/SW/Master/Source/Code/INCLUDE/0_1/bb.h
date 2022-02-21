/****************************************************************************/
/* MODULE:                                                                  */
/*   bb.h - Black burst generators																				  */
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

#ifndef _BB_H
#define _BB_H

#define BB_V24_address	0x34

#define NoOfBBOutputs	3

enum BBOutputs					{ BBOutput1 = 0,	BBOutput2, BBOutput3 };

typedef struct {
	UC System;
	UL Delay;
	int ScHPhase;
} BBObject;

extern BBObject BBSetup[NoOfBBOutputs];

extern int BBInit( void);

extern int SetBBSystem( UC output, int system);
extern int SetBBDelay(  UC output, UL delay);
extern int SetBBScHPhase(  UC output, int schphase);

#endif