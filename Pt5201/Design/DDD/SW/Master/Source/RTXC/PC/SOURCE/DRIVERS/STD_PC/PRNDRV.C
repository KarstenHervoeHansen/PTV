/* prndrv.c - PC parallel printer driver task, interrupt driven */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1998.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <dos.h>

#include "rtxcapi.h"
#include "prndrv.h"  /* selects which PRN (LPT1 vs LPT2) */
#include "rtxstdio.h"

extern const PRNCB_Struct PRNdcb;

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

extern void interrupt isvcprn(); /* asm level PRN svc */

#ifndef NO_INTS
static void interrupt (*old)();
#endif

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

static void prnstart(void);
void prnstop(void);

static int near expect = 0; /* indicates LPT interrupt expected */

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/* PRN interrupt handler */
FRAME *prnc(FRAME *frame)
{
   outportb(PIC_EOI_ADDR, EOI);    /* clear interrupt */

   if (!expect) /* ignore SEL, Power ON/OFF, and other spurious ints */
      return(KS_ISRexit(frame, 0));

   expect = 0; /* reset expect flag */

   return(KS_ISRexit(frame, PRNdcb.sema));
}

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

/* start printer driver hook */
static void prnstart(void)
{
#ifndef NO_INTS
   unsigned int mask;

   old = getvect(PRNINT);
   setvect(PRNINT, isvcprn);

   mask = inportb(PIC_MASK_ADDR);
   outportb(PIC_MASK_ADDR, mask & PRN_INT_ENABLE);

   outportb(CONTROL, INIT_RTXC);       /* ints enabled, port initialized */
#endif
}

/* stop printer driver hook */
void prnstop(void)
{
#ifndef NO_INTS
   unsigned int mask;

   mask = inportb(PIC_MASK_ADDR);
   outportb(PIC_MASK_ADDR, mask & PRN_INT_DISABLE); /*disable PRN interrupts*/
   outportb(CONTROL, INIT_DOS);  /* stop printer interrupts */

   setvect(PRNINT, old); /* restore to DOS interrupt handler */
#endif
}

/* TASK */
void prndrv(void)
{
   unsigned int status;
   unsigned char ochar;

   prnstart(); /* claim interrupt and configure parallel card */
   for (;;)
   {
      KS_dequeuew(PRNdcb.queue, &ochar);
      while (( (status = inportb(STATUS)) & PRN_READY ) != PRN_READY )
      {
         if (status & NO_PAPER)
            KS_delay(0, PAPER_TMOUT);
         else if (~status & PRN_OFF)
            KS_delay(0, OFF_TMOUT);
         else  /* since printer not ready */
            ;   /* KS_delay(0, CHAR_TMOUT); */
      }
      outportb(DATA_OUT, ochar); /* write char to latch */

      expect = 1; /* set flag for interrupt service */

      /* now toggle bit 0 in control reg to output strobe data */
      outportb(CONTROL, SEND_CHAR);
      outportb(CONTROL, NORM_MODE);

#ifdef NO_INTS
      KS_delay(SELFTASK, 2); /* short delay, at least 2 tocks */
#else
      KS_wait(PRNdcb.sema);   /* wait for char done signal */
#endif
   }
}

/* End of file - prndrv.c */
