/* periphal.h - define the peripherals in the application */

/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/*
 * NOTE: These definitions must match those in the isrs.inc file.
*/

/* ---------------------------------------- */
/* #define the CLK if timers are needed     */
/* #undef the CLK if timers are not needed  */
/* ---------------------------------------- */
#define CLK

#ifdef CLK
#define TIMER_0     /* timer 0 - #define or #undef */
#define TIMER_2     /* timer 2 - RTXC time base - #define or #undef */

#ifdef TIMER_0
#define TIMER0INTLVL 0x0E    /* defines priority level for timer 0 */
#endif /* } TIMER_0 */

#ifdef TIMER_2
#define TIMER2INTLVL 0x0C    /* defines priority level for timer 2 */
#endif /* } TIMER_2 */

#endif /* } CLK */

/* end of periphal.h */
