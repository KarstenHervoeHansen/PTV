/* periphal.h - define the peripherals in the application */

/*
 *   RTXC    
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
#undef CLK

#ifdef CLK
#undef TIMER_0     /* timer 0 - #define or #undef */
#undef TIMER_2     /* timer 2 - RTXC time base - #define or #undef */

#ifdef TIMER_0
#define TIMER0INTLVL 0x0E    /* defines priority level for timer 0 */
#endif /* } TIMER_0 */

#ifdef TIMER_2
#define TIMER2INTLVL 0x0C    /* defines priority level for timer 2 */
#endif /* } TIMER_2 */

#endif /* } CLK */

/* ---------------------------------------- */
/* #define the SIO if serial is needed      */
/* #undef the SIO if timers is not needed   */
/* ---------------------------------------- */
#define SIO

#ifdef SIO
#define SIO_U0       /* Serial I/O 1 - #define or #undef */

#ifdef SIO_U0 /* { */
#define RX0INTLVL 0x0E   /* defines priority level for receive  0 */
#define TX0INTLVL 0x0B   /* defines priority level for transmit 0 */
#endif /* } SIO_U0 */

#define SIO_U1       /* Serial I/O 1 - #define or #undef */

#ifdef SIO_U1 /* { */
#define RX1INTLVL 0x0A   /* defines priority level for receive  1 */
#define TX1INTLVL 0x09   /* defines priority level for transmit 1 */
#endif /* } SIO_U1 */

#endif /* } SIO */

/* ---------------------------------------- */
/* #define the EXINT0 if external interrupt is needed      */
/* #undef the EXINT0 if timers is not needed   */
/* ---------------------------------------- */
#define EXINT0
#define EXINT1

#ifdef EXINT0

#define EX0INTLVL 0x0F   /* defines priority level for external interrupt 0 */

#endif /* } EXINT0 */

#ifdef EXINT1

#define EX1INTLVL 0x0F   /* defines priority level for external interrupt 1 */

#endif /* } EXINT1 */

/* end of periphal.h */
