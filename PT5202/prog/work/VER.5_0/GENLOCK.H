/****************************************************************************/
/* MODULE:                                                                  */
/*   genlock.h - Genlock generator	 																			  */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*  NONE                                                                    */
/*                                                                          */
/* TASKS:                                                                   */
/*  NONE: 20.11.01 - GenlockCalibrationObject MOIDFIED: JK                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#ifndef _GENLOCK_H
#define _GENLOCK_H

#define SPG_V24_address	0x15

enum GenlockSystem			{ Internal = 0, BurstLockPAL, BurstLockNTSC, MHzLock10 };

typedef struct 
  {
	UL Date;
 
	UL DACFactDate;
	UL DACUserDate;

	UI DACFactValue;
	UI DACUserValue;

	UL HPHZeroG;
	UL PhaseG;

	UL HPHZeroM;
	UL PhaseM;
	int DACOffset; // JK
  } GenlockCalibrationObject;

typedef struct
  {
	UC System;
	UL Delay;
  } GenlockObject;

extern GenlockObject GenlockSetup;

extern void GenlockDefaults( void);
extern int GenlockInit( void);
extern int GenlockVersion( char* result);
extern int GenlockUpdate( void);

extern int SetGenlockSystem( int system);
extern int SetGenlockDelay(  UL delay);

#endif