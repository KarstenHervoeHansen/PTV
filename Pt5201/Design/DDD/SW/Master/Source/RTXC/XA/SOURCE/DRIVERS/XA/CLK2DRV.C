/* clk2drv.c - model CLK driver task for 8XC251 using software timer 2 */

/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/


#include "rtxcapi.h"
#include "periphal.h"

#ifdef CLK /* { */
#ifdef TIMER_2 /* { */

#include "enable.h"

#include "cclock.h" /* CLKTICK */

/*-----------------------------------------------------------------*/
/* A clock period is the number of ticks (CLKTICK) per one
   millisecond (.001).                                             */
#define PERIOD (CLKTICK * .001)
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/* The XTAL frequency is 20 MHz.                             	   */
// Next line commented out by TK PTV. Instead use crystal.h
//#define CRYSTAL 20E6 /* Change to match processor speed as needed */
#include "crystal.h"
/*-----------------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/* The timer register is incremented once every peripheral cycle
   which is once every four oscillator cycles.                     */
#define DIVIDE_DOWN 4
/*-----------------------------------------------------------------*/

/* Formula for calculating reload value for Timer 2 */
#define RESET_TIME (0x0FFFF - (unsigned short)(PERIOD / (1 / (CRYSTAL / DIVIDE_DOWN ))))
#define HI_RESET (RESET_TIME >> 8) & 0xFF
#define LO_RESET (RESET_TIME & 0xFF)

long timer2cnt = 0;

FRAME ks_stk * timer2c(FRAME ks_stk * frame)
{
	SWR7=0;
   timer2cnt++;
   TF2 = 0;
   KS_ISRtick();
   return(KS_ISRexit(frame, 0 ));
}

/* initialization code for clock */
void timer2_start(void)
{
   SWE |= 0x40;
}

#else
void timer2_start(void)
{
   KS_nop();
}
#endif /* end of TIMER_2 */

#else
void timer2_start(void)
{
   KS_nop();
}
#endif /* end of CLK */

/* end of clk2drv.c */
