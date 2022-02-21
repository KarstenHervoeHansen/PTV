/* rtxcmain.c */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/*** 15/4-99 TK.    Added ptvmain call.   ***/

#include "sysopts.h"

#include "enable.h"
#include "rtxcapi.h"


#ifdef PTV
extern void ptvmain(void);
extern void ptvend(void);
#endif


/* CONFIG */
#ifdef KA_SUPPORT
unsigned long pointer_bias = 0xFFFFFFFFL;
#endif

#define BRKINT 0x23  /* BRK (^C) interrupt */
#define DVZINT 0x00  /* Div by zero interrupt */

#define BRK /* indicates ^C aborts RTXC program */
#undef DVZ /* indicates divide by zero aborts RTXC program */

extern void interrupt (far * far getvect(int))(void);
extern void far setvect(int, void interrupt (far *)(void));

extern void rtxcinit(void); /* initialize RTXC */

extern void clkstart(void);	
extern void clkstop(void);

/* driver stop functions restore vectors, etc. */

extern void rtxcstop(void);


volatile char near brkflag; /* non-zero if BREAK hit or aborted */
TASK near brktask; /* break task # */

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#ifdef BRK
extern void interrupt isvcbrk(); /* break svc */
static void interrupt (*oldbrk)();
#endif

#ifdef DVZ
extern void interrupt isvcdvz(); /* dvz svc */
static void interrupt (*olddvz)();
#endif

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

static void stop(void);

int main(void)
{
   int i;

#ifdef KA_SUPPORT
	pointer_bias = ((((unsigned long)_DS) << 16) & 0xFFFF0000L);
#endif

#ifdef BRK
   oldbrk = getvect(BRKINT); /* save old ^C vector */
   setvect(BRKINT, isvcbrk); /* claim ^C vector from DOS */
#endif

#ifdef DVZ
   olddvz = getvect(DVZINT); /* save old /0 vector */
   setvect(DVZINT, isvcdvz); /* claim /0 vector from DOS */
#endif

   /*
    * user supplied startup code for board specific initialization
    * should be inserted here
   */

   rtxcinit(); /* initialize RTXC */

   clkstart();	/* initialize and start the clock driver */

   ENABLE;

   ptvmain();

   for (;;) /* loop till brkflag changes or forever */
   {
      if (brkflag)
      {
         break;
      }
   }

   /*
    * user supplied shutdown code should be inserted here
    * typically, the null task never stops, but it may
    * be aborted in some circumstances where it is desirable
    * to stop multi-tasking.
   */

   ptvend();

   clkstop();	/* stop the clock driver and restore state */

   rtxcstop();

   stop(); /* normal shutdown */

   return(1);
}

/* restore system before returning to DOS */
static void stop(void)
{
#ifdef BRK
   setvect(BRKINT, oldbrk); /* restore std ^C interrupt handler */
#endif

#ifdef DVZ
   setvect(DVZINT, olddvz); /* restore std divide by zero handler */
#endif
}

/* End of file - rtxcmain.c */
