/****************************************************************************/
/* MODULE:                                                                  */
/*   genlock.h - Genlock generator	 																			  */
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

#ifndef _GENLOCK_H
#define _GENLOCK_H

#define SPG_V24_address	0x15

enum GenlockSystem			{ Internal = 0, BurstLockPAL, BurstLockNTSC, MHzLock10 };

typedef struct {
	UC System;
	UL Delay;
} GenlockObject;

extern GenlockObject GenlockSetup;

extern void GenlockInit( void);

extern int SetGenlockSystem( int system);
extern int SetGenlockDelay(  UL delay);

#endif