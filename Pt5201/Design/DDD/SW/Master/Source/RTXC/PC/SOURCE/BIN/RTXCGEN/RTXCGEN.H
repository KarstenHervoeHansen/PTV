/* rtxcgen.h */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products
 *   ALL RIGHTS RESERVED
*/

#ifndef _RTXCGEN_H
#define _RTXCGEN_H

/*

The variable, SIGLISTSIZE, should be set to the worst case size.  The size is
defined  by the maximum number  of semaphores that can  be signalled during a
single RTXC system  call.  The  term is the  sum of the  number of semaphores
that can signalled from the interrupt  service  level  via  isrc()  plus  the
maximum number of semaphores that can  be  signalled  during  a  single  RTXC
kernel call.  Typically, the worst case SIGLISTSIZE is 6 - 8.

Unusually  large numbers (>  8) may be  necessary in the  following cases: 1)
when multiple semaphores  are  signalled  from  the  same  interrupt  service
routine and there is a  possibility  of  nested  interrupts  each  signalling
multiple semaphores,  2)  the  use  of  KS_signalm()  with  a  long  list  of
semaphores to signal in a single call, and 3) the use of semaphores to detect
queue empty, not empty, full, not full and mailbox not empty conditions.  The
worst case queue  use  term  can  be  4  semaphores  when  a  full  queue  is
KS_purgequeue()ed.   In the  worst case  KS_purgequeue() scenario,  the queue
becomes not full and then  empty by  the purge  and then  not empty  and full
again as it  is refilled  by the  multiple waiters  who were  already blocked
waiting on the queue to become non-full.

Setting  SIGLISTSIZE too low can  cause runtime problems that  can be ** VERY
HARD ** to detect, therefore the default setting of 16 is outrageously large.
RTXCbug provides a means of monitoring the worst case SIGLISTSIZE via the 'K'
command.

*/

#define SIGLISTSIZE 16    /* worst case sema signal list size - tunable */

#define STKLOGIC         /* indicates stk limit table support in RTXCbug */

/* task related - tunable per system */

#define TASKSTKSZ 512     /* default task stack size */



/*

The screen  size variable is used to determine  the number of lines to scroll
when in page  mode displays.  A  PC screen usually has  25 lines but terminal
based systems will typically have 24.  Define SCREENSIZE to fit your system.

*/

#define SCREENSIZE 25 /* 25=PC, 24=most real terminals */


/*

Numerous sizeof()s terms are used by  RTXCgen  in  calculating  RAM  and  ROM
requirements for RTXC system  tables  and  variable  space.    Note,  if  the
compiler that generates code for target is not the same as the host compiler,
the numbers  defined by the  sizeof(struct) calculations are  bogus, thus the
manually calculated values  as seen below.   The actual  structure sizes will
vary based  on various typedefs,  pointer sizes (near/far),  FPU switch, CBUG
switch, int  size (short or long), and  structure  packing  options.  If  you
are using a  cross-compiler,  you  should  update  the  corresponding  SIZEOF
entries in order  for RTXCgen to  accurately report RAM  and ROM requirements
for the system  tables and  variable space.   The  sizes.exe program  in this
directory is provided to aid in figuring out the right SIZEOF numbers.

*/


/* task related */
#define ROMTCB 1 /* 1 indicates romable tcb, else 0 */

#define SIZEOF_TCB sizeof(struct tcb)
#define SIZEOF_KTCB sizeof(struct ktcb)
#define SIZEOF_FPREGS sizeof(struct fpregs)


/* mailbox related */
#define SIZEOF_MH  sizeof(struct mheader)


/* partition related */
#define ROMPH 1 /* 1 indicates romable partition header, else 0 */

#define SIZEOF_PH  sizeof(struct pheader)
#define SIZEOF_KPH sizeof(struct pkheader)


/* queue related */
#define ROMQH 1 /* 1 indicates romable queue header, else 0 */

#define SIZEOF_QH  sizeof(struct qheader)
#define SIZEOF_KQH sizeof(struct qkheader)


/* resource related */
#define SIZEOF_RH sizeof(struct rheader)


/* clock related */
#define SIZEOF_CLKBLK sizeof(struct clkblk)


/* semaphore related */
#define SIZEOF_SEMA sizeof(SEMA)
#define SIZEOF_SSTATE sizeof(SSTATE)

#endif /* } _RTXCGEN_H */

/* End of file - rtxcgen.h */