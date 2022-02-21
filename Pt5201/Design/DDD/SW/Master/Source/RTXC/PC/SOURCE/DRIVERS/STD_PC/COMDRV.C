/* comdrv.c - PC COM driver task, interrupt driven, full-duplex */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1998.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <dos.h>
#include "enable.h"

#include "rtxcapi.h"

#include "comdrv.h"
#include "rtxstdio.h"

/* Console Device Control Block */
extern CONCB_Struct CONCdcb[];
#define PORT 1 /* index for CONCdcb */

/* CONFIG */
#ifdef __cplusplus /* { */
extern "C" {

static void interrupt (*old)(...);
extern void interrupt isvccom(...); /* assembly level COM svc */

}
#else
static void interrupt (*old)();
extern void interrupt isvccom(); /* assembly level COM svc */
#endif /* } __cplusplus */


#ifdef CBUG
extern void sgnlcbug(void);
#endif

static void comstart(void);

void comstop(void);

static char ichar; /* last char read for input */

/*
/* interrupt service for COM ports on IBM PC serial card
/*
/* Note, more than one interrupt may be pending (TX done, RX ready) at a
/* given time, so you must service "both" interrupts before exiting this
/* routine.  The USART will not re-request an interrupt if you only clear
/* one of them.  Thus the nasty logic for checking which one caused the
/* interrupt and then another check for duplicate interrupts.  Note also
/* that it may require two semaphores to be signaled (COMISEM and COMOSEM).
/* The KS_ISRsignal() call performs this secondary semaphore signal.
*/

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

FRAME *comc(FRAME *frame)
{
   unsigned char status;

   status = inportb(UART_INT_ID_REG); /* read hw status */
   if (status == RX_DATA_READY) /* RX is higher priority w/in USART */
   {
      ichar = inportb(UART_RBR); /* read char, clear USART interrupt */
      status = inportb(UART_INT_ID_REG); /* re-read hw status */
      if (status == TX_BUFF_EMPTY)
         KS_ISRsignal(CONCdcb[PORT].osema); /* signal char output semaphore */
      outportb(PIC_EOI_ADDR, EOI); /* clear PIC interrupt */
      return(KS_ISRexit(frame, CONCdcb[PORT].isema)); /* signal char input semaphore */
   }
   else
   {
      if (status == TX_BUFF_EMPTY)
      {
         status = inportb(UART_INT_ID_REG); /* re-read hw status */
         if (status == RX_DATA_READY)
         {
            ichar = inportb(UART_RBR); /* read char, clear USART interrupt */
            KS_ISRsignal(CONCdcb[PORT].isema); /* signal char input semaphore */
         }
         outportb(PIC_EOI_ADDR, EOI); /* clear PIC interrupt */
         return(KS_ISRexit(frame, CONCdcb[PORT].osema)); /* exit signalling char output sema */
      }
   }
   outportb(PIC_EOI_ADDR, EOI); /* clear interrupt */
   return(KS_ISRexit(frame, (SEMA)0));
}
#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */


/* COM driver start hook */
static void comstart(void)
{
   unsigned char mask;

   old = getvect(COMINT);    /* save COM vector */
   setvect(COMINT, isvccom); /* claim COM vector */

   outportb(UART_MCR, COM_MCR);

   outportb(UART_LCR, inportb(UART_LCR) & ~DLAB);

   inportb(UART_RBR); /* dummy read to clear any TX interrupt */
   inportb(UART_INT_ID_REG); /* clear any RX interrupt */
   outportb(UART_INT_EN_REG, RX_INT_ENABLE | TX_INT_ENABLE); /* enable TX/RX int */

   mask = inportb(PIC_MASK_ADDR);
   outportb(PIC_MASK_ADDR, mask & COM_INT_ENABLE); /* enable com interrupts */
}

/* COM driver stop hook - called before returning to DOS */
void comstop(void)
{
   unsigned char mask;

   outportb(UART_INT_EN_REG, INT_DISABLE); /* disable TX/RX int */

   mask = inportb(PIC_MASK_ADDR);
   outportb(PIC_MASK_ADDR, mask | COM_INT_DISABLE); /* disable com interrupts */

   setvect(COMINT, old); /* restore COM vector */
}

/* TASK */
void comidrv(void) /* input driver for COM channel */
{
   comstart();
   for (;;)
   {
      KS_wait(CONCdcb[PORT].isema); /* wait on keystroke */
#ifdef CBUG
      if (ichar == '!')
         sgnlcbug();     /* wake up Cbug */
      else
         KS_enqueuew(CONCdcb[PORT].inq, &ichar); /* queue char for later processing */
      KS_enqueuew(CONCdcb[PORT].inq, &ichar); /* echo to output queue */
#else
      KS_enqueuew(CONCdcb[PORT].inq, &ichar); /* queue char for later processing */
#endif
   }
}

/* TASK */
void comodrv(void) /* output driver for COM channel */
{
   char ochar;

   KS_wait(CONCdcb[PORT].osema); /* for initialization caused INT */

   for (;;)
   {
      KS_dequeuew(CONCdcb[PORT].outq, &ochar);   /* get next char from queue */
      if (ochar == '\n')
      {
         outportb(UART_TBR, '\r');    /* output to usart */
         KS_wait(CONCdcb[PORT].osema);          /* wait for it to "go" out */

         outportb(UART_TBR, ochar);    /* output to usart */
      }
      else
         outportb(UART_TBR, ochar);    /* output to usart */
      KS_wait(CONCdcb[PORT].osema);          /* wait for it to "go" out */
   }
}

/* End of file - comdrv.c */
