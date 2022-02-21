/****************************************************************************/
/* MODULE:                                                                  */
/*   unitprg.h - 																				 */
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

#ifndef _UNITPRG_H
#define _UNITPRG_H


enum units							{ BBUnit = 0, SPGUnit };

extern void BBSPGReset( bool SPGReset, bool BBReset, bool SPGPrg);
extern void BBSPGCommunication( int unit);

#endif