/* clkdrv.c - CLK interrupt service functions - PC version */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <dos.h>

#include "rtxcapi.h"
#include "cclock.h"
//#include "rtxstdio.h"

/* Clock control block */
//extern const CLKCB_Struct CLKdcb;

#define PIC 0x20 /* Programmable Interrupt Controller address */
#define EOI 0x20 /* End Of Interrupt code */

#define CLKINT 0x08  /* intercept DOS TOD timer interrupt */

#define CHL_0  0x40  /* clk channel 0 */
#define CLKREG 0x43  /* clk register */
#define CLK_FACTR ((1193180L / CLKRATE) + 1 )   /* see CCLOCK.H ms */

void clkstart(void);              /* take over from DOS */
void clkstop(void);               /* prepare for returning to DOS */

#ifdef __cplusplus /* { */
extern "C" {

static void interrupt (far *old)(...);
extern void interrupt isvcclk(...); /* clock interrupt service prologue */

}
#else
static void interrupt (far *old)();
extern void interrupt isvcclk(); /* clock interrupt service prologue */
#endif /* } __cplusplus */

                                 /* located in KERNEL\RTXCASM.ASM */

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

FRAME *clkc(FRAME *frame)
{
   outportb(PIC, EOI);    /* clear interrupt */

   KS_ISRtick(); /* bump tick counter */
   return(KS_ISRexit(frame, (SEMA)0));
}
#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */


/* clock driver start hook */
void clkstart(void)
{
   unsigned int clk_factr = (unsigned int)(CLK_FACTR);
   old = getvect(CLKINT);    /* save old CLK vector */
   setvect(CLKINT, isvcclk); /* claim CLK vector */

   outportb(CLKREG, 0x36);  /* channel 0 2 byte counter, mode 3, binary */
   outportb(CHL_0, (unsigned int) clk_factr);       /* LSB of rate */
   outportb(CHL_0, (unsigned int)(clk_factr >> 8)); /* MSB of rate */
}

/* clock driver stop hook */
void clkstop(void)
{
   setvect(CLKINT, old); /* restore old CLK vector */

           /* restore clock to 18.2 Hz */
   outportb(CLKREG, 0x36);  /* channel 0 2 byte counter, mode 3, binary */
   outportb(CHL_0,  (unsigned int)0xffff);        /* LSB */
   outportb(CHL_0, ((unsigned int)0xffff) >> 8);  /* MSB */
}

/* End of file - clkdrv.c */
