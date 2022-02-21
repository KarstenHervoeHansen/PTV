/* clkdrv.c - model CLK driver task for XA using software timer 0 */

/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "periphal.h"

#ifdef CLK  /* { */
#ifdef TIMER_0  /* { */

#include "rtxcapi.h"
#include "enable.h"

#include "cclock.h" /* CLKTICK */

// Next line commented out by TK PTV. Instead use crystal.h
//#define CRYSTAL 20E6 /* Change to match processor speed as needed */
#include "crystal.h"

#define PERIOD (CLKTICK / 3 * .001)

/* Formula for calculating reload value for Timer 0 */
#define RESET_TIME (unsigned short)(PERIOD / (1 / (CRYSTAL / 4)))

#define HI_RESET (-RESET_TIME >> 8) & 0xFF
#define LO_RESET (-RESET_TIME & 0xFF)

long timer0cnt = 0;

FRAME ks_stk * timer0c(FRAME ks_stk * frame)
{
   timer0cnt++;
   return(KS_ISRexit(frame, 0));    /* no sema to post, just return */
}


/* initialization code for clock */
void timer0_start(void)
{
   /* Initialize Timer/Counter 0 */
   /* Timer/Counter Control Register */
   TCON |= 0x10;     /* BIT VAL    DESCR */
                     /* TF1  0   
                      * TR1  0
                      * TF0  0
                      * TR0  1   Timer 0 run control bit
                      * IE1  0
                      * IT1  0
                      * IE0  0
                      * IT0  0
                      */

   TH0 = HI_RESET;
   TL0 = LO_RESET;
   RTH0 = HI_RESET;
   RTL0 = LO_RESET;

   /* Interrupt Enable Register */
   IEL |= 0x82;       /* BIT VAL    DESCR */
                     /* EA   1   Enable all interrupts
                      * --
                      * ET2  0
                      * ES   0
                      * ET1  0
                      * EX1  0
                      * ET0  1   Enable Timer/Counter interrupt
                      * EX0  0
                      */

   IPA0 |= TIMER0INTLVL << 4;       /* Set timer 0 priority */
}
#else
void timer0_start(void)
{
}
#endif  /* } TIMER_0 */

#else
void timer0_start(void)
{
}
#endif  /* } CLK */

/* end of clkdrv.c */
