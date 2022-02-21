/* rtxcgen.h */

/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
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

#define STKLOGIC          /* indicates stk limit table support in RTXCbug */

#define TASKSTKSZ 64     /* default task stack size */
#define RTXCSTKSZ 256     /* RTXC kernel stack size */


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
switch,  int size (short or long), and structure packing options.  If you are
using a cross-compiler, you should update the corresponding SIZEOF entries in
order for RTXCgen to accurately report  RAM  and  ROM  requirements  for  the
system tables and variable space.  The sizes.exe program in this directory is
provided to aid in figuring out the right SIZEOF numbers.

*/
#undef SMALL
#undef MEDIUM
#define LARGE

/* task related */
#define ROMTCB 1 /* 1 indicates romable tcb, else 0 */


#ifdef SMALL /* { */
#define BASIC_TCB  18
#define BASIC_KTCB  7
#endif  /* } SMALL */

#ifdef MEDIUM  /* { */
#define BASIC_TCB  20
#define BASIC_KTCB  9
#endif  /* } MEDIUM */

#ifdef LARGE  /* { */
#define BASIC_TCB  30
#define BASIC_KTCB  11
#endif  /* } LARGE */

#ifdef TIME_SLICE  /* { */
#define H_TS  4
#else
#define H_TS  0
#endif  /* } TIME_SLICE */

#ifdef HAS_INQTASK_ARG  /* { */
#ifdef LARGE  /* { */
#define H_IQA  4
#else
#define H_IQA  2
#endif  /* } LARGE */
#else
#define H_IQA  0
#endif  /* } HAS_INQTASK_ARG */

#ifdef FPU  /* { */
#ifdef LARGE  /* { */
#define H_FPU_TCB  6
#define H_FPU_KTCB 4
#else
#define H_FPU_TCB  4
#define H_FPU_KTCB  2
#endif  /* } LARGE */
#else
#define H_FPU_TCB  0
#define H_FPU_KTCB  0
#endif  /* } FPU */

#define SIZEOF_TCB   (BASIC_TCB + H_TS + H_IQA + H_FPU_TCB)
#define SIZEOF_KTCB  (BASIC_KTCB + H_FPU_KTCB)
#define SIZEOF_FPREGS  4


#ifdef HAS_QUEUES  /* { */

/* queue related */
#define ROMQH 1          /* 1 indicates romable queue header, else 0 */

#ifdef LARGE  /* { */
#define BASIC_QH  12
#define BASIC_KQH  8
#else
#define BASIC_QH  10
#define BASIC_KQH  6
#endif  /* } LARGE */

#ifdef QUEUE_WAITERS   /* { */
#ifdef LARGE  /* { */
#define H_QW  8
#else
#define H_QW  4
#endif  /* } LARGE */
#else
#define H_QW  0
#endif  /* } QUEUE_WAITERS */

#ifdef QUEUE_SEMAS  /* { */
#define H_QS  4
#else
#define H_QS  0
#endif  /* } QUEUE_SEMAS */

#ifdef CBUG   /* { */
#define H_QCB  4
#else
#define H_QCB  0
#endif  /* } CBUG */

#define SIZEOF_QH    (BASIC_QH + H_QW + H_QS + H_QCB)
#define SIZEOF_KQH   BASIC_KQH

#else

#define SIZEOF_QH    0
#define SIZEOF_KQH   0
#define ROMQH 0          /* 1 indicates romable queue header, else 0 */
#endif  /* } HAS_QUEUES */

#ifdef HAS_PARTITIONS  /* { */
/* partition related */
#define ROMPH 1          /* 1 indicates romable partition header, else 0 */

#ifdef LARGE  /* { */
#define BASIC_PH  8
#define BASIC_KPH 8
#else
#define BASIC_PH  6
#define BASIC_KPH 6
#endif  /* } LARGE */

#ifdef PARTITION_WAITERS  /* { */
#ifdef LARGE  /* { */
#define H_PW  8
#else
#define H_PW  4
#endif  /* } LARGE */
#else
#define H_PW  0
#endif  /* } PARTITION_WAITERS */

#ifdef DYNAMIC_PARTS /* { */
#ifdef LARGE  /* { */
#define H_DP  5
#else
#define H_DP  3
#endif  /* } LARGE */
#else
#define H_DP  0
#endif /* } DYNAMIC_PARTS */

#ifdef CBUG  /* { */
#define H_PCB  6
#else
#define H_PCB  0
#endif  /* } CBUG */

#define SIZEOF_PH    (BASIC_PH + H_PW + H_PCB)
#define SIZEOF_KPH   BASIC_KPH

#else

#define SIZEOF_PH    0
#define SIZEOF_KPH   0
#define ROMPH 0          /* 1 indicates romable partition header, else 0 */
#endif  /* } HAS_PARTITIONS */

#ifdef HAS_MAILBOXES  /* { */

#ifdef LARGE  /* { */
#define BASIC_MH  4
#else
#define BASIC_MH  2
#endif  /* } LARGE */

#ifdef MAILBOX_WAITERS  /* { */
#ifdef LARGE  /* { */
#define H_MW  8
#else
#define H_MW  4
#endif  /* } LARGE */
#else
#define H_MW  0
#endif  /* } MAILBOX_WAITERS */

#ifdef MAILBOX_SEMAS   /* { */
#define H_MS  1
#else
#define H_MS  0
#endif  /* } MAILBOX_SEMAS */

#ifdef CBUG  /* { */
#define H_MCB  2
#else
#define H_MCB  0
#endif  /* } CBUG */

#define SIZEOF_MH    (BASIC_MH + H_MW + H_MS + H_MCB)

#else

#define SIZEOF_MH    0

#endif  /* } HAS_MAILBOX */

#ifdef HAS_RESOURCES  /* { */

#ifdef LARGE  /* { */
#define BASIC_RH  5
#else
#define BASIC_RH  3
#endif  /* } LARGE */

#ifdef RESOURCE_WAITERS  /* { */
#ifdef LARGE  /* { */
#define H_RW  8
#else
#define H_RW  4
#endif  /* } LARGE */
#ifdef PRIORITY_INVERSION  /* { */
#define PI  3
#else
#define PI  0
#endif  /* } PRIORITY_INVERSION */
#else
#define H_RW  0
#define PI  0
#endif /* } RESOURCE_WAITERS */

#ifdef CBUG   /* { */
#define H_RCB  4
#else
#define H_RCB  0
#endif  /* } CBUG */

#define SIZEOF_RH    (BASIC_RH + H_RW + PI + H_RCB)

#else

#define SIZEOF_RH    0

#endif  /* } HAS_RESOURCE */

/* semaphore related */
#define SIZEOF_SEMA       1  /* := sizeof(SEMA) */
#define SIZEOF_SSTATE     1  /* := sizeof(SSTATE) */

/* clock related */

#ifdef LARGE  /* { */
#define SIZEOF_CLKBLK 18 /* := sizeof(struct clkblk) */
#else
#define SIZEOF_CLKBLK 14 /* := sizeof(struct clkblk) */
#endif  /* } LARGE */

#ifdef RTXCIO    /* } RTXCio */
#define SIZEOF_CHANNELS   18
#define SIZEOF_DEVICE     15
#define SIZEOF_CNTLR      30
#endif /* } RTXCio */

#endif /* } _RTXCGEN_H */
