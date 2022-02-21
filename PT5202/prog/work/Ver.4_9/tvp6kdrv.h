/****************************************************************************/
/* MODULE:                                                                  */
/*   tvp6kdrv.h - TI TVP6000 Multimedie driver															*/
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

#ifndef _TVP6KDRV_H
#define _TVP6KDRV_H

#define TVP6K_IIC_address			0x40
#define	C_PHASE_address				0x5A

extern int tvp6k_init(  void);
extern int tvp6k_system(  int system);

#endif