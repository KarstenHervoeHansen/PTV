/* rtxcmain.c - C main() module */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/*** 15/4-99 TK.    Added ptvmain call.   ***/

#include "reg8051.h"
#include "sysopts.h"

#include "rtxcapi.h"
#include "enable.h"
#include "periphal.h"
#include "ext0drv.h"
#include "plddrv.h"

extern void ptvmain(void);

/* CONFIG */

extern void rtxcinit(void);         // initialize RTXC

extern void ticktimer_start(void);  // initialize tick timer

extern char setintlvl(char);        // function to set task PPL

/* C main() module -- THE NULLTASK */
void main(void)
{ 
#ifndef PTV
  int i;
#endif

/* We make check here if PLD is initialized correctly.  If pld_init() returns
   FAIL, we stop here and do nothing. :JK change.
*/
  if (pld_init() == FAIL)
    {
    for (;;)
      ;            // do nothing
    }
  //pld_init();								// initialize PLD (audio/SPG)
  ext0_init();             	// initialize tick Timer source
  ticktimer_start();       	// initialize tick Timer

  // kernel initialization
  rtxcinit();              	// initialize RTXC

  /* set task processor priority level in global variables */
  /* and enable interrupts at task PPL */
  setintlvl(TASK_INT_LEVEL);

  /* enable interrupts */
  EA = 1;

  ptvmain();

  for (;;) /* loop forever (null task) */
  {
  }

  /*
   * user supplied shutdown code should be inserted here
   * typically, the null task never stops, but it may
   * be aborted in some circumstances where it is desirable
   * to stop multi-tasking.
  */
 }

/* end of rtxcmain.c */
