/* rtxcapi.c - RTXC application program interface to kernel directives */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "typedef.h"
#include "rtxstruc.h"
#include "rtxcarg.h"
#include "enable.h"

#define NULL        ((void *)0)
#define NULLCLK     ((CLKBLK ks_clk *)0)
#define NULLTCB     ((TCB near *)0)

#define SELFTASK    ((TASK)0)

extern SSTATE near semat[];
extern TCB near rtxtcb[];
extern TCB near * near hipritsk; /* highest priority task */

/* choices are near or far stack - each with choice of vector or call based */

   /* inline code is faster and supported on Turbo and Borland C++ */

#ifdef VECTOR_BASED /* { (default) vector based, traditional RTXC entry method */

#define ENTER_KERNEL __asm int RTXC_VECTOR;

#else /* } VECTOR_BASED { */

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */
extern void isvcrtx(void); /* reference for call based */
#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#define ENTER_KERNEL __asm pushf;    \
                     __asm cli;      \
                     __asm call FAR ptr isvcrtx;

#endif /* } VECTOR_BASED */

#ifdef KS_STK_FAR /* { (default) far ks_stk model for Borland C++ */

#ifdef CPU386 /* { */
#define STORE_ARG_PACKET __asm mov ax, ss; \
                         __asm shl eax, 16; \
                         __asm lea ax,p;
#else /* } CPU386 { */
#define STORE_ARG_PACKET __asm lea cx,p;
#endif /* } CPU386 */

#else /* } KS_STK_FAR { near ks_stk model - requires tlink with c0fl.obj */

#ifdef CPU386 /* { */
#define STORE_ARG_PACKET __asm lea eax, p;
#else /* } CPU386 { */
#define STORE_ARG_PACKET __asm lea ax,p;
#endif /* } CPU386 */

#endif /* } KS_STK_FAR */

#define KS(p) STORE_ARG_PACKET \
              ENTER_KERNEL

/*
 * NOTE: The next two FAST switches improve performance at the
 *       expense of a considerable increase in the size of the
 *       rtxcapi module considerably.
*/

/* user defined option for speed optimized KS_lockw()/KS_unlock() */
#undef FASTLOCK

/* user defined option for speed optimized KS_alloc()/KS_allocw() */
#undef FASTALLOC

#ifdef FPU /* { */

extern TCB near * near fputask; /* current owner of FPU */

#endif /* } FPU */

#ifdef HAS_RESOURCES /* { */
#ifdef FASTLOCK /* { */
extern RHEADER near rheader[];
#endif /* } FASTLOCK */
#endif /* } HAS_RESOURCES */

#ifdef HAS_PARTITIONS /* { */
extern PHEADER near pheader[];
#endif /* } HAS_PARTITIONS */

#ifdef HAS_QUEUES /* { */
extern QHEADER near qheader[];
#endif /* } HAS_QUEUES */

extern void far stkinit(int ks_stk *, size_t);

/*******************************************************/
#ifdef HAS_BLOCK /* { */
void KSAPI KS_block(TASK starttask, TASK endtask)
{
   struct blkarg p;

   p.ksnum = RTXC_BLOCK;
   p.starttask = starttask;
   p.endtask = endtask;
   KS(p);
}
#endif /* } HAS_BLOCK */

/*******************************************************/
const char * KSAPI KS_version(void)
{
   return("RTXC 3.2c for x86 - Borland v5.0x Oct-29-97 Key: 22359");
}

/*******************************************************/
#ifdef HAS_NOP /* { */
void KSAPI KS_nop(void)
{
   struct targ p;

   p.ksnum = RTXC_NOP;
   KS(p);
}
#endif /* } HAS_NOP */

/*******************************************************/
#ifdef HAS_ALLOC /* { */
void * KSAPI KS_alloc(MAP map)
{
   struct parg p;
#ifdef FASTALLOC /* { */
   PHEADER near *pph;
   FLAG_STORAGE;

   pph = &pheader[map];

	SAVE_FLAGS;
   DISABLE;

   if ( (p.val = (char *)pph->next) != NULL ) /* if any available */
   {
      pph->next = ((struct xmap *)p.val)->link; /* unlink 1st from list */
#ifdef CBUG /* { */
      if (++pph->cur > pph->worst) /* check for new worst case */
         pph->worst = pph->cur;          /* useage level */
#endif /* } CBUG */
   }
   RESTORE_FLAGS;
#else /* } FASTALLOC { */
   p.ksnum = RTXC_ALLOC;
   p.map = map;
#ifdef PARTITION_WAITERS /* { */
   p.wait = 0;
#endif /* } PARTITION_WAITERS */
   KS(p);
#endif /* } FASTALLOC */
   return(p.val);
}
#endif /* } HAS_ALLOC */

/*******************************************************/
#ifdef HAS_ALLOC_TIMER /* { */
CLKBLK ks_clk * KSAPI KS_alloc_timer(void)
{
   struct clkarg p;

   p.ksnum = RTXC_ALLOC_TIMER;
   KS(p);
   return(p.pclkblk);
}
#endif /* } HAS_ALLOC_TIMER */

/*******************************************************/
#ifdef HAS_DEFPRIORITY /* { */
void KSAPI KS_defpriority(TASK task, PRIORITY priority)
{
   struct targ p;

   p.ksnum = RTXC_DEFPRIORITY;
   p.task = task;
   p.priority = priority;
   KS(p);
}
#endif /* } HAS_DEFPRIORITY */

/*******************************************************/
#ifdef TIME_SLICE /* { */
void KSAPI KS_defslice(TASK task, TICKS ticks)
{
   TCB near *ptcb;

   if (task == SELFTASK)
      ptcb = hipritsk;
   else
      ptcb = &rtxtcb[task];

   /* if slicing is just becoming active */
   if (ptcb->newslice == 0)
   {
      ptcb->tslice = ticks;
   }
   ptcb->newslice = ticks;
}
#endif /* } TIME_SLICE */

/*******************************************************/
#ifdef HAS_DELAY /* { */
void KSAPI KS_delay(TASK task, TICKS ticks)
{
   struct delayarg p;
   CLKBLK clkblk;

   p.ksnum = RTXC_DELAY;
   p.task = task;
   p.ticks = ticks;
   p.pclkblk = (CLKBLK ks_clk *)&clkblk;

   KS(p);
}
#endif /* } HAS_DELAY */

/*******************************************************/
#ifdef HAS_DEQUEUE /* { */
KSRC KSAPI KS_dequeue(QUEUE queue, void *data)
{
   struct qarg p;

   p.ksnum = RTXC_DEQUEUE;
   p.queue = queue;
   p.data = data;
#ifdef QUEUE_WAITERS /* { */
   p.wait = 0;
#endif /* } QUEUE_WAITERS */
   p.ksrc = RC_QUEUE_EMPTY;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_DEQUEUE */

/*******************************************************/
#ifdef HAS_ENQUEUE /* { */
KSRC KSAPI KS_enqueue(QUEUE queue, void *data)
{
   struct qarg p;

   p.ksnum = RTXC_ENQUEUE;
   p.queue = queue;
   p.data = data;
#ifdef QUEUE_WAITERS /* { */
   p.wait = 0;
#endif /* } QUEUE_WAITERS */
   p.ksrc = RC_QUEUE_FULL;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_ENQUEUE */

/*******************************************************/
void KSAPI KS_execute(TASK task)
{
   struct targ p;

   p.ksnum = RTXC_EXECUTE;
   p.task = task;
   KS(p);
}

/*******************************************************/
#ifdef HAS_DEFTASK /* { */
KSRC KSAPI KS_deftask(TASK task, PRIORITY priority,
                char ks_stk *stackbase, size_t stacksize,
                void (far *entry)(void))
{
   struct deftaskarg p;

#ifdef CBUG /* { */
   stkinit((int ks_stk *)stackbase, stacksize);
#endif /* } CBUG */

   p.ksnum = RTXC_DEFTASK;
   p.task = task;
   p.priority = priority;
   p.stackbase = stackbase;
   p.stacksize = stacksize;
   p.entry = entry;
   p.ksrc = RC_GOOD;
   KS(p);

   return(p.ksrc);
}
#endif /* } HAS_DEFTASK */

/*******************************************************/
#ifdef HAS_ALLOC_TASK /* { */
TASK KSAPI KS_alloc_task(void)
{
   struct targ p;

   p.ksnum = RTXC_ALLOC_TASK;
   KS(p);

   /* return task # (0 = no tasks available) */
   return(p.task);
}
#endif /* } HAS_ALLOC_TASK */

/*******************************************************/
#ifdef HAS_FREE /* { */
void KSAPI KS_free(MAP map, void *val)
{
   struct parg p;

   p.ksnum = RTXC_FREE;
   p.map = map;
   p.val = val;
   KS(p);
}
#endif /* } HAS_FREE */

/*******************************************************/
#ifdef HAS_ALLOC_PART /* { */
MAP KSAPI KS_alloc_part(void)
{
   struct parg p;

   p.ksnum = RTXC_ALLOC_PART;

   KS(p);

   /* return map # (0 = no dynamic partition headers available) */
   return(p.map);
}
#endif /* } HAS_ALLOC_PART */

/*******************************************************/
#ifdef HAS_DEFPART /* { */
void KSAPI KS_defpart(MAP map, void *addr, size_t blksize, size_t nblocks)
{
   struct parg p;

   p.ksnum = RTXC_DEFPART;
   p.map = map;
   p.addr = (char *)addr;
   p.size = blksize;
   p.nblocks = nblocks;
   KS(p);
}
#endif /* } HAS_DEFPART */

/*******************************************************/
#ifdef HAS_FREE_PART /* { */
void * KSAPI KS_free_part(MAP map)
{
   struct parg p;

   p.ksnum = RTXC_FREE_PART;
   p.map = map;
   KS(p);
   return(p.addr);
}
#endif /* } HAS_FREE_PART */

/*******************************************************/
#ifdef HAS_FREE_TIMER /* { */
void KSAPI KS_free_timer(CLKBLK ks_clk *pclkblk)
{
   struct clkarg p;

   p.ksnum = RTXC_FREE_TIMER;
   if ( (p.pclkblk = pclkblk) != NULLCLK)
   {
      KS(p);
   }
}
#endif /* } HAS_FREE_TIMER */

/*******************************************************/
#ifdef HAS_LOCK /* { */
KSRC KSAPI KS_lock(RESOURCE resource)
{
   struct larg p;
#ifdef FASTLOCK /* { */
   RHEADER near *prh;
   FLAG_STORAGE;

   prh = &rheader[resource];

	SAVE_FLAGS;
   DISABLE;

   if (prh->owner == NULLTCB)  /* if resource not currently owned */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->owner = hipritsk; /* lock resource with task # */
      prh->level = 1;         /* set nesting level = 1 */
      RESTORE_FLAGS;                 /* fast return */
      return(RC_GOOD);        /* mark successful */
   }

   if (prh->owner == hipritsk) /* if already owned by caller */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->level++;           /* bump nesting level */
      RESTORE_FLAGS;                 /* fast return */
      return(RC_NESTED);      /* mark successful */
   }
   RESTORE_FLAGS;
#endif /* } FASTLOCK */

   p.ksnum = RTXC_LOCK;
   p.resource = resource;
#ifdef RESOURCE_WAITERS /* { */
   p.wait = 0;
#endif /* } RESOURCE_WAITERS */
   p.ksrc = RC_GOOD;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_LOCK */

/*******************************************************/
#ifdef HAS_PEND /* { */
void KSAPI KS_pend(SEMA sema)
{
   struct sarg p;

   p.ksnum = RTXC_PEND;
   p.sema = sema;
   KS(p);
}
#endif /* } HAS_PEND */

/*******************************************************/
#ifdef HAS_RECEIVE /* { */
RTXCMSG * KSAPI KS_receive(MBOX mbox, TASK task)
{
   struct msgarg p;

   p.ksnum = RTXC_RECEIVE;
   p.mbox = mbox;
   p.task = task;
#ifdef MAILBOX_WAITERS /* { */
   p.wait = 0;
#endif /* } MAILBOX_WAITERS */
   KS(p);
   return(p.prtxcmsg);
}
#endif /* } HAS_RECEIVE */

/*******************************************************/
#ifdef HAS_RESUME /* { */
void KSAPI KS_resume(TASK task)
{
   struct targ p;

   p.ksnum = RTXC_RESUME;
   p.task = task;
   KS(p);
}
#endif /* } HAS_RESUME */

/*******************************************************/
#ifdef HAS_SEND /* { */
void KSAPI KS_send(MBOX mbox, RTXCMSG *prtxcmsg, PRIORITY priority, SEMA sema)
{
   struct msgarg p;

   p.ksnum = RTXC_SEND;
   p.mbox = mbox;
   p.prtxcmsg = prtxcmsg;
   p.priority = priority;
   p.sema = sema;
   p.wait = 0;
   KS(p);
}
#endif /* } HAS_SEND */

/*******************************************************/
KSRC KSAPI KS_signal(SEMA sema)
{
   struct sarg p;

   p.ksnum = RTXC_SIGNAL;
   p.sema = sema;
   p.ksrc = RC_GOOD;
   KS(p);
   return(p.ksrc);
}

/*******************************************************/
#ifdef HAS_ACK /* { */
void KSAPI KS_ack(RTXCMSG *prtxcmsg)
{

   KS_signal(prtxcmsg->sema);

}
#endif /* } HAS_ACK */

/*******************************************************/
#ifdef HAS_STOP_TIMER /* { */
KSRC KSAPI KS_stop_timer(CLKBLK ks_clk *pclkblk)
{
   struct clkarg p;

   p.ksnum = RTXC_STOP_TIMER;
   p.pclkblk = pclkblk;
   p.ksrc = RC_GOOD;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_STOP_TIMER */

/*******************************************************/
#ifdef HAS_START_TIMER /* { */
CLKBLK ks_clk * KSAPI KS_start_timer(CLKBLK ks_clk *pclkblk, TICKS ticks,
                                    TICKS period, SEMA sema)
{
   struct clkarg p;

   p.ksnum = RTXC_START_TIMER;
   p.pclkblk = pclkblk;
   p.ticks = ticks;
   p.period = period;
   p.sema = sema;
   KS(p);
   return(p.pclkblk);
}
#endif /* } HAS_START_TIMER */

/*******************************************************/
#ifdef HAS_SUSPEND /* { */
void KSAPI KS_suspend(TASK task)
{
   struct targ p;

   p.ksnum = RTXC_SUSPEND;
   p.task = task;
   KS(p);
}
#endif /* } HAS_SUSPEND */

/*******************************************************/
#ifdef HAS_TERMINATE /* { */
void KSAPI KS_terminate(TASK task)
{
   struct targ p;

   p.ksnum = RTXC_TERMINATE;
   p.task = task;
   KS(p);
}
#endif /* } HAS_TERMINATE */

/*******************************************************/
#ifdef HAS_UNBLOCK /* { */
void KSAPI KS_unblock(TASK starttask, TASK endtask)
{
   struct blkarg p;

   p.ksnum = RTXC_UNBLOCK;
   p.starttask = starttask;
   p.endtask = endtask;
   KS(p);
}
#endif /* } HAS_UNBLOCK */

/*******************************************************/
#ifdef HAS_UNLOCK /* { */
KSRC KSAPI KS_unlock(RESOURCE resource)
{
   struct larg p;
#ifdef FASTLOCK /* { */
   RHEADER near *prh;
   FLAG_STORAGE;

   prh = &rheader[resource];

	SAVE_FLAGS;
   DISABLE;

   if (prh->owner != hipritsk) /* if hipritsk not owner, then nop */
   {
      RESTORE_FLAGS; /* fast return */
      return(RC_BUSY);
   }

   if (prh->level > 1) /* if nesting level > 1 */
   {
      prh->level--;   /* decrement nesting level */
      RESTORE_FLAGS;              /* fast return */
      return(RC_NESTED);
   }

#ifdef RESOURCE_WAITERS /* { */
   if (prh->waiters == NULLTCB) /* if no tasks are waiting */
   {
      prh->level = 0;  /* zero nesting level */
      prh->owner = NULLTCB; /* mark resource not owned */
      RESTORE_FLAGS;              /* fast return */
      return(RC_GOOD);
   }
#endif /* } RESOURCE_WAITERS */

   RESTORE_FLAGS;              /* fast return */
#endif /* } FASTLOCK */

   p.ksnum = RTXC_UNLOCK;
   p.ksrc = RC_GOOD;
   p.resource = resource;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_UNLOCK */

/*******************************************************/
#ifdef HAS_USER /* { */
int KSAPI KS_user(int (*fun)(void *), void *arg)
{
   struct userarg p;

   p.ksnum = RTXC_USER;
   p.fun = fun;
   p.arg = arg;
   KS(p);
   return(p.val);
}
#endif /* } HAS_USER */

/*******************************************************/
KSRC KSAPI KS_wait(SEMA sema)
{
   struct sarg p;

   p.ksnum = RTXC_WAIT;
   p.sema = sema;
   p.ticks = (TICKS)0;
   p.ksrc = RC_GOOD;
   KS(p);
   return(p.ksrc);
}

/*******************************************************/
#ifdef HAS_YIELD /* { */
KSRC KSAPI KS_yield(void)
{
   struct targ p;

   p.ksnum = RTXC_YIELD;
   p.ksrc = RC_NO_YIELD;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_YIELD */

#ifdef RTXC_AL /* { */
/*******************************************************/
/*              ADVANCED RTXC LIBRARY                  */
/*******************************************************/
#ifdef HAS_ALLOCW /* { */
void * KSAPI KS_allocw(MAP map)
{
   struct parg p;
#ifdef FASTALLOC /* { */
   PHEADER near *pph;
   FLAG_STORAGE;

   pph = &pheader[map];

	SAVE_FLAGS;
   DISABLE;

   if ( (p.val = (char *)pph->next) != NULL ) /* if any available */
   {
      pph->next = ((struct xmap *)p.val)->link; /* unlink 1st from list */
#ifdef CBUG /* { */
      if (++pph->cur > pph->worst) /* check for new worst case */
         pph->worst = pph->cur;          /* useage level */
#endif /* } CBUG */
      RESTORE_FLAGS;
      return(p.val);
   }
   RESTORE_FLAGS;
#endif /* } FASTALLOC */

   p.ksnum = RTXC_ALLOC;
   p.map = map;
   p.wait = 1;
#ifdef PARTITION_TIMEOUTS /* { */
   p.ticks = (TICKS)0; /* indicate NO timeout */
#endif /* } PARTITION_TIMEOUTS */
   KS(p);
   return(p.val);
}
#endif /* } HAS_ALLOCW */

/*******************************************************/
#ifdef HAS_CREATE_PART /* { */
MAP KSAPI KS_create_part(void *addr, size_t blksize, size_t nblocks)
{
   struct parg p;

   p.ksnum = RTXC_CREATE_PART;
   p.addr = (char *)addr;
   p.size = blksize;
   p.nblocks = nblocks;
   KS(p);

   /* return map # (0 = no dynamic partition headers available) */
   return(p.map);
}
#endif /* } HAS_CREATE_PART */

/*******************************************************/
#ifdef HAS_INQRES /* { */
TASK KSAPI KS_inqres(RESOURCE resource)
{
   struct larg p;

   p.ksnum = RTXC_INQRES;
   p.resource = resource;
   p.task = (TASK)0; /* initialize in case no owner exists */
   KS(p);
   return(p.task);
}
#endif /* } HAS_INQRES */

/*******************************************************/
#ifdef PRIORITY_INVERSION /* { */
#ifdef HAS_DEFRES /* { */
KSRC KSAPI KS_defres(RESOURCE resource, RESATTR resattr)
{
   struct larg p;

   p.ksnum = RTXC_DEFRES;
   p.resource = resource;
   p.resattr = resattr;
   p.ksrc = RC_GOOD;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_DEFRES */
#endif /* } PRIORITY_INVERSION */

/*******************************************************/
#ifdef HAS_INQTASK_ARG /* { */
void * KSAPI KS_inqtask_arg(TASK task)
{
   struct deftaskarg p;

   p.ksnum = RTXC_INQTASK_ARG;
   p.task = task;
   KS(p);
   return(p.arg);
}
#endif /* } HAS_INQTASK_ARG */

/*******************************************************/
#ifdef HAS_DEFTASK_ARG /* { */
void KSAPI KS_deftask_arg(TASK task, void *arg)
{
   struct deftaskarg p;

   p.ksnum = RTXC_DEFTASK_ARG;
   p.task = task;
   p.arg = arg;
   KS(p);
}
#endif /* } HAS_DEFTASK_ARG */

/*******************************************************/
#ifdef HAS_DEFMBOXSEMA /* { */
/* mailbox event management */
void KSAPI KS_defmboxsema(MBOX mbox, SEMA sema)
{
   struct msgarg p;

   p.ksnum = RTXC_DEFMBOXSEMA;
   p.mbox = mbox;
   p.sema = sema;
   KS(p);
}
#endif /* } HAS_DEFMBOXSEMA */

/*******************************************************/
#ifdef HAS_DEFQSEMA /* { */
/* queue event management */
void KSAPI KS_defqsema(QUEUE queue, SEMA sema, QCOND qcond)
{
   struct qdefarg p;

   p.ksnum = RTXC_DEFQSEMA;
   p.queue = queue;
   p.sema = sema;
   p.qcond = qcond;
   KS(p);
}
#endif /* } HAS_DEFQSEMA */

/*******************************************************/
#ifdef HAS_DEQUEUEW /* { */
void KSAPI KS_dequeuew(QUEUE queue, void *data)
{
   struct qarg p;

   p.ksnum = RTXC_DEQUEUE;
   p.queue = queue;
   p.data = data;
#ifdef QUEUE_TIMEOUTS /* { */
   p.ticks = (TICKS)0;
#endif /* } QUEUE_TIMEOUTS */
   p.wait = 1;
   KS(p);
}
#endif /* } HAS_DEQUEUEW */

/*******************************************************/
#ifdef HAS_ELAPSE /* { */
TICKS KSAPI KS_elapse(TICKS *stamp)
{
   struct etarg p;

   p.ksnum = RTXC_ELAPSE;
   p.stamp = stamp;
   KS(p);
   return(p.val);
}
#endif /* } HAS_ELAPSE */

/*******************************************************/
#ifdef HAS_ENQUEUEW /* { */
void KSAPI KS_enqueuew(QUEUE queue, void *data)
{
   struct qarg p;

   p.ksnum = RTXC_ENQUEUE;
   p.queue = queue;
   p.data = data;
#ifdef QUEUE_TIMEOUTS /* { */
   p.ticks = (TICKS)0;
#endif /* } QUEUE_TIMEOUTS */
   p.wait = 1;
   KS(p);
}
#endif /* } HAS_ENQUEUEW */

/*******************************************************/
#ifdef HAS_INQMAP /* { */
size_t KSAPI KS_inqmap(MAP map)
{
   return(pheader[map].size);
}
#endif /* } HAS_INQMAP */

/*******************************************************/
#ifdef HAS_LOCKW /* { */
KSRC KSAPI KS_lockw(RESOURCE resource)
{
   struct larg p;
#ifdef FASTLOCK /* { */
   RHEADER near *prh;
   FLAG_STORAGE;

   prh = &rheader[resource];

	SAVE_FLAGS;
   DISABLE;

   if (prh->owner == NULLTCB)  /* if resource not currently owned */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->owner = hipritsk; /* lock resource with task # */
      prh->level = 1;         /* set nesting level = 1 */
      RESTORE_FLAGS;                 /* fast return */
      return(RC_GOOD);
   }

   if (prh->owner == hipritsk) /* if already owned by caller */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->level++;           /* bump nesting level */
      RESTORE_FLAGS;                 /* fast return */
      return(RC_NESTED);
   }
   RESTORE_FLAGS;
#endif /* } FASTLOCK */

   p.ksnum = RTXC_LOCK;
   p.resource = resource;
   p.wait = 1;
   p.ksrc = RC_GOOD;
#ifdef RESOURCE_TIMEOUTS /* { */
   p.ticks = (TICKS)0;
#endif /* } RESOURCE_TIMEOUTS */
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_LOCKW */

/*******************************************************/
#ifdef HAS_PURGEQUEUE /* { */
void KSAPI KS_purgequeue(QUEUE queue)
{
   struct qarg p;

   p.ksnum = RTXC_PURGEQUEUE;
   p.queue = queue;
   KS(p);
}
#endif /* } HAS_PURGEQUEUE */

/*******************************************************/
#ifdef HAS_RECEIVEW /* { */
RTXCMSG * KSAPI KS_receivew(MBOX mbox, TASK task)
{
   struct msgarg p;

   p.ksnum = RTXC_RECEIVE;
   p.mbox = mbox;
   p.task = task;
   p.prtxcmsg = (RTXCMSG *)0; /* preset in case timeout */
   p.wait = 1;
#ifdef MAILBOX_TIMEOUTS /* { */
   p.ticks = (TICKS)0;
#endif /* } MAILBOX_TIMEOUTS */
   KS(p);
   return(p.prtxcmsg);
}
#endif /* } HAS_RECEIVEW */

/*******************************************************/
#ifdef HAS_SENDW /* { */
void KSAPI KS_sendw(MBOX mbox, RTXCMSG *prtxcmsg, PRIORITY priority, SEMA sema)
{
   struct msgarg p;

   p.ksnum = RTXC_SEND;
   p.mbox = mbox;
   p.prtxcmsg = prtxcmsg;
   p.priority = priority;
   p.sema = sema;
   p.wait = 1;
#ifdef MAILBOX_TIMEOUTS /* { */
   p.ticks = (TICKS)0; /* indicate NO timeout */
#endif /* } MAILBOX_TIMEOUTS */
   KS(p);
}
#endif /* } HAS_SENDW */

/*******************************************************/
#ifdef HAS_WAITM /* { */
SEMA KSAPI KS_waitm(const SEMA *semalist)
{
   struct sargm p;

   p.ksnum = RTXC_WAITM;
   p.list = semalist;
   KS(p);
   return(p.sema);
}
#endif /* } HAS_WAITM */

/*******************************************************/
#ifdef HAS_INQTIME /* { */
time_t KSAPI KS_inqtime(void)
{
   struct timearg p;

   p.ksnum = RTXC_INQTIME;
   KS(p);
   return(p.time);
}
#endif /* } HAS_INQTIME */

/*******************************************************/
#ifdef HAS_DEFTIME /* { */
void KSAPI KS_deftime(time_t time)
{
   struct timearg p;

   p.ksnum = RTXC_DEFTIME;
   p.time = time;
   KS(p);
}
#endif /* } HAS_DEFTIME */

#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */
/*******************************************************/
/* EXTENDED RTXC LIBRARY */
/*******************************************************/
#ifdef HAS_INQTIMER /* { */
TICKS KSAPI KS_inqtimer(CLKBLK ks_clk *pclkblk)
{
   struct clkarg p;

   p.ksnum = RTXC_INQTIMER;
   p.pclkblk = pclkblk;
   KS(p);
   return(p.ticks);
}
#endif /* } HAS_INQTIMER */

/*******************************************************/
#ifdef HAS_ALLOCT /* { */
void * KSAPI KS_alloct(MAP map, TICKS ticks, KSRC *pksrc)
{
   struct parg p;
   CLKBLK clkblk;
#ifdef FASTALLOC /* { */
   PHEADER near *pph;
   FLAG_STORAGE;

   pph = &pheader[map];

	SAVE_FLAGS;
   DISABLE;

   if ( (p.val = (char *)pph->next) != NULL ) /* if any available */
   {
      pph->next = ((struct xmap *)p.val)->link; /* unlink 1st from list */
#ifdef CBUG /* { */
      if (++pph->cur > pph->worst) /* check for new worst case */
         pph->worst = pph->cur;          /* useage level */
#endif /* } CBUG */
      RESTORE_FLAGS;
      *pksrc = RC_GOOD;
      return(p.val);
   }
   RESTORE_FLAGS;
#endif /* } FASTALLOC */

   p.ksnum = RTXC_ALLOC;
   p.map = map;
   p.wait = 1;
   p.ticks = ticks;
   p.ksrc = RC_GOOD;
   p.pclkblk = (CLKBLK ks_clk *)&clkblk;
   KS(p);
   *pksrc = p.ksrc;
   return(p.val);
}
#endif /* } HAS_ALLOCT */

/*******************************************************/
#ifdef HAS_DEFQUEUE /* { */
KSRC KSAPI KS_defqueue(QUEUE queue, size_t width, int depth,
                      char *base, int current_size)
{
   struct qdefarg p;

   p.ksnum = RTXC_DEFQUEUE;
   p.queue = queue;
   p.width = width;
   p.depth = depth;
   p.base = base;
   p.current_size = current_size;
   p.ksrc = RC_GOOD;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_DEFQUEUE */

/*******************************************************/
#ifdef HAS_DEQUEUET /* { */
KSRC KSAPI KS_dequeuet(QUEUE queue, void *data, TICKS ticks)
{
   struct qarg p;
   CLKBLK clkblk;

   p.ksnum = RTXC_DEQUEUE;
   p.queue = queue;
   p.data = data;
   p.ticks = ticks;
   p.wait = 1;
   p.ksrc = RC_TIMEOUT;
   p.pclkblk = (CLKBLK ks_clk *)&clkblk;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_DEQUEUET */

/*******************************************************/
#ifdef HAS_ENQUEUET /* { */
KSRC KSAPI KS_enqueuet(QUEUE queue, void *data, TICKS ticks)
{
   struct qarg p;
   CLKBLK clkblk;

   p.ksnum = RTXC_ENQUEUE;
   p.queue = queue;
   p.data = data;
   p.ticks = ticks;
   p.wait = 1;
   p.ksrc = RC_TIMEOUT;
   p.pclkblk = (CLKBLK ks_clk *)&clkblk;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_ENQUEUET */

/*******************************************************/
#ifdef HAS_INQPRIORITY /* { */
PRIORITY KSAPI KS_inqpriority(TASK task)
{
   if (task == SELFTASK)
      return(hipritsk->priority);
   else
      return(rtxtcb[task].priority);
}
#endif /* } HAS_INQPRIORITY */

/*******************************************************/
#ifdef HAS_INQQUEUE /* { */
int KSAPI KS_inqqueue(QUEUE queue)
{
   return(qheader[queue].curndx);
}
#endif /* } HAS_INQQUEUE */

/*******************************************************/
#ifdef HAS_INQSEMA /* { */
SSTATE KSAPI KS_inqsema(SEMA sema)
{
   return(semat[sema]);
}
#endif /* } HAS_INQSEMA */

/*******************************************************/
#ifdef HAS_INQTASK /* { */
TASK KSAPI KS_inqtask(void)
{
   return(hipritsk->task);
}
#endif /* } HAS_INQTASK */

/*******************************************************/
#ifdef HAS_LOCKT /* { */
KSRC KSAPI KS_lockt(RESOURCE resource, TICKS ticks)
{
   struct larg p;
   CLKBLK clkblk;
#ifdef FASTLOCK /* { */
   RHEADER near *prh;
   FLAG_STORAGE;

   prh = &rheader[resource];

	SAVE_FLAGS;
   DISABLE;

   if (prh->owner == NULLTCB)  /* if resource not currently owned */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->owner = hipritsk; /* lock resource with task # */
      prh->level = 1;         /* set nesting level = 1 */
      RESTORE_FLAGS;                 /* fast return */
      return(RC_GOOD);        /* mark successful */
   }

   if (prh->owner == hipritsk) /* if already owned by caller */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->level++;           /* bump nesting level */
      RESTORE_FLAGS;                 /* fast return */
      return(RC_NESTED);      /* mark successful */
   }
   RESTORE_FLAGS;
#endif /* } FASTLOCK */

   p.ksnum = RTXC_LOCK;
   p.resource = resource;
   p.wait = 1;
   p.ticks = ticks;
   p.ksrc = RC_GOOD;
   p.pclkblk = (CLKBLK ks_clk *)&clkblk;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_LOCKT */

/*******************************************************/
#ifdef HAS_PENDM /* { */
void KSAPI KS_pendm(const SEMA *semalist)
{
   struct sargm p;

   p.ksnum = RTXC_PENDM;
   p.list = semalist;
   KS(p);
}
#endif /* } HAS_PENDM */

/*******************************************************/
#ifdef HAS_RECEIVET /* { */
RTXCMSG * KSAPI KS_receivet(MBOX mbox, TASK task, TICKS ticks, KSRC *pksrc)
{
   struct msgarg p;
   CLKBLK clkblk;

   p.ksnum = RTXC_RECEIVE;
   p.mbox = mbox;
   p.task = task;
   p.prtxcmsg = (RTXCMSG *)0; /* preset in case timeout */
   p.wait = 1;
   p.ticks = ticks;
   p.ksrc = RC_GOOD;
   p.pclkblk = (CLKBLK ks_clk *)&clkblk;
   KS(p);
   *pksrc = p.ksrc;
   return(p.prtxcmsg);
}
#endif /* } HAS_RECEIVET */

/*******************************************************/
#ifdef HAS_RESTART_TIMER /* { */
KSRC KSAPI KS_restart_timer(CLKBLK ks_clk *pclkblk, TICKS ticks, TICKS period)
{
   struct clkarg p;

   p.ksnum = RTXC_RESTART_TIMER;
   p.pclkblk = pclkblk;
   p.ticks = ticks;
   p.period = period;
   p.ksrc = RC_GOOD;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_RESTART_TIMER */

/*******************************************************/
#ifdef HAS_SENDT /* { */
KSRC KSAPI KS_sendt(MBOX mbox, RTXCMSG *prtxcmsg, PRIORITY priority, SEMA sema,
                   TICKS ticks)
{
   struct msgarg p;
   CLKBLK clkblk;

   p.ksnum = RTXC_SEND;
   p.mbox = mbox;
   p.prtxcmsg = prtxcmsg;
   p.priority = priority;
   p.sema = sema;
   p.wait = 1;
   p.ticks = ticks;
   p.ksrc = RC_GOOD;
   p.pclkblk = (CLKBLK ks_clk *)&clkblk;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_SENDT */

/*******************************************************/
#ifdef HAS_SIGNALM /* { */
void KSAPI KS_signalm(const SEMA *semalist)
{
   struct sargm p;

   p.ksnum = RTXC_SIGNALM;
   p.list = semalist;
   KS(p);
}
#endif /* } HAS_SIGNALM */

/*******************************************************/
#ifdef HAS_WAITT /* { */
KSRC KSAPI KS_waitt(SEMA sema, TICKS ticks)
{
   struct sarg p;
   CLKBLK clkblk;

   p.ksnum = RTXC_WAIT;
   p.sema = sema;
   p.ticks = ticks;
   p.ksrc = RC_GOOD;
   p.pclkblk = (CLKBLK ks_clk *)&clkblk;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_WAITT */

/*******************************************************/
#ifdef TIME_SLICE /* { */
#ifdef HAS_INQSLICE /* { */
TICKS KSAPI KS_inqslice(TASK task)
{
   if (task == SELFTASK)
      task = hipritsk->task;

   return(rtxtcb[task].newslice);
}
#endif /* } HAS_INQSLICE */
#endif /* } TIME_SLICE */

#endif /* } RTXC_EL */

#ifdef FP_EMU /* { */

/* Borland Floating Emulation Library */
/*
 * The Borland emulation library allocates 272 bytes on the current stack
 * to maintain the floating point control structure and a floating point
 * stack.  The fpu control structure is 68 bytes is located 32 bytes off
 * ss:0x0.  The fpu stack is 204 bytes and is located after the fpu control
 * structure at ss:0x64 (0x20 + 0x44).  The floating point emulation
 * structure is only initialized once during startup.  This initial
 * condition is used to initialize every task which requires floating point
 * support.  The init_FPU routine copies this initialized structure from the
 * NULL task to the calling task (init_FPU must be called by each tasking
 * which requires floating point support).
 *
 * We have not determined what information Borland stores in the first
 * 32 bytes.  The first 32 bytes are copied along with the fpu control
 * structure and stack.
 *
*/
extern void far * far memcpy(void far *, const void far *, unsigned int);
extern char far _emu[]; /* emulation register area - defined by Borland */
extern unsigned _fpstklen; /* defined by Borland - value is 0xCC */

#define FPU_STACK_OFFSET  0x20 /* offset of fpu control struct from ss:0x0 */
#define FPU_STRUCT_SIZE   0x44 /* size of fpu control struct               */

void KSAPI init_FPU(TASK task)
{
   if (task == SELFTASK)
     task = hipritsk->task;
   memcpy(rtxtcb[task].stackbase, &_emu[0] - FPU_STACK_OFFSET,
   		FPU_STACK_OFFSET + FPU_STRUCT_SIZE + _fpstklen);
}
#endif /* } FP_EMU */

/* End of file - rtxcapi.c */
