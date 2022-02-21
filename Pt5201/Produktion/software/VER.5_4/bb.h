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

typedef struct 
  {
	UL Date;

	UL PhaseG;
	UI ScHPhaseG;

	UL PhaseM;
	UI ScHPhaseM;

	UC DACGainG;
	UC DACOffsetG;
	UC DACLevelG;

	UC DACGainM;
	UC DACOffsetM;
	UC DACLevelM;
  } BBCalibrationObject;

typedef struct {
	UC System;
	UL Delay;
	int ScHPhase;
} BBObject;

extern BBObject BBSetup[NoOfBBOutputs];

extern volatile int BBControlPort;

extern void BBDefaults( void);
extern int BBInit( void);
extern int BBVersion( char *result);
extern int BBUpdate( void);

extern int SetBBSystem( int output, int system);
extern int SetBBDelay(  int output, UL delay);
extern int SetBBScHPhase(  int output, int schphase);

#endif