/****************************************************************************/
/* MODULE:                                                                  */
/*  tim0drv.c - timer/counter 0 driver                                 			*/
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include "define.h"
#include "tim0drv.h"

/* =================== */
/* TMOD - bit settings */
/* ------------------- */
/* TIMER 0             */
/* ------------------- */
#define T0_GATE (0 << 3)   /* disable gate control */
#define T0_CT   (1 << 2)   /* cleared for timer operation */
#define T0_M1   (0 << 1)   /* 16-bit auto-reload */
#define T0_M0   (1 << 0)   /* 16-bit auto-reload */

#define T0_TMOD  (T0_GATE | T0_CT | T0_M1 | T0_M0)

/* ------------------- */
/* TIMER 1             */
/* ------------------- */
#define T1_GATE (0 << 7)   /* not applicable - OR in a zero (0) */
#define T1_CT   (0 << 6)   /* not applicable - OR in a zero (0) */
#define T1_M1   (0 << 5)   /* not applicable - OR in a zero (0) */
#define T1_M0   (0 << 4)   /* not applicable - OR in a zero (0) */

#define T1_TMOD  (T1_GATE | T1_CT | T1_M1 | T1_M0)

#define xTMOD  (T0_TMOD | T1_TMOD)
/* =================== */

/* =================== */
/* TCON - bit settings */
/* ------------------- */
/* TIMER 0             */
/* ------------------- */
#define T0_TF0  (0 << 5)   /* overflow flag - manipulated by hardware */
#define T0_TR0  (1 << 4)   /* run control bit - enabled */
#define T0_IE0  (0 << 1)   /* not applicable - OR in a zero (0) */
#define T0_IT0  (0 << 0)   /* not applicable - OR in a zero (0) */

#define T0_CON (T0_TF0 | T0_TR0 | T0_IE0 | T0_IT0)


/* ------------------- */
/* TIMER 1             */
/* ------------------- */
#define T1_TF1  (0 << 7)   /* not applicable - OR in a zero (0) */
#define T1_TR1  (0 << 6)   /* not applicable - OR in a zero (0) */
#define T1_IE1  (0 << 3)   /* interrupt edge flag */
#define T1_IT1  (1 << 2)   /* interrupt type control bit - enabled */

#define T1_CON (T1_TF1 | T1_TR1 | T1_IE1 | T1_IT1)

#define xTCON (T0_CON | T1_CON)
/* =================== */

/**************************************************************************/
/* tim0_init	 																								 TIM0_DRV.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000330																		*/
/* Revised:		000330, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void tim0_init( void)
{
  /* Timer/Counter Mode Control Register */
  TMOD &= 0xF0;   /* clear timer0 settings */
  TMOD |= xTMOD;
  
  /* Timer/Counter Control Register */
  TCON &= 0xCB;   /* clear timer0 settings */
  TCON |= xTCON;
}
