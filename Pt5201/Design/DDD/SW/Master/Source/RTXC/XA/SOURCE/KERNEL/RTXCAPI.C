/* rtxcapi.c - RTXC application program interface to kernel directives */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <cxa.h>

#include "rtxcapi.h"    /* function prototypes */
#include "typedef.h"
#include "rtxstruc.h"
#include "rtxcarg.h"
#include "enable.h"

#define NULL        ((void *)0)
#define NULLCLK     ((CLKBLK ks_clk *)0)
#define NULLTCB     ((TCB ks_tbl *)0)
#define SELFTASK    ((TASK)0)

extern SSTATE ks_tblx semat[];
extern TCB ks_tblx rtxtcb[];
extern TCB ks_tbl * core_iramdata hipritsk; /* highest priority task */

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#if 1
#define KS(p) ks((void ks_stk *)&p); /* traditional model for rtxc entry,  */
extern void ks(void ks_stk *);     /* C to assembly function call           */
#else
#if 0
_inline void KS( void ks_stk * p)
{
#pragma asm
     trap #0
#pragma endasm
}
#else
#define KS(p) ks((void ks_stk *)(ks_stk)&p); /* traditional model for rtxc entry,  */
extern void ks(void ks_stk *);     /* C to assembly function call           */
#endif
#endif

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

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
extern TCB ks_tbl * ks_tbl fputask; /* current owner of FPU */
#endif /* } FPU */

#ifdef HAS_RESOURCES /* { */
#ifdef FASTLOCK /* { */
extern RHEADER ks_tblx rheader[];
#endif /* } FASTLOCK */
#endif /* } HAS_RESOURCES */

#ifdef HAS_PARTITIONS /* { */
extern PHEADER ks_tblx pheader[];
#endif /* } HAS_PARTITIONS */

#ifdef HAS_QUEUES /* { */
extern QHEADER ks_tblx qheader[];
#endif /* } HAS_QUEUES */

#ifdef CBUG
extern void stkinit(short ks_stk *, size_t);
#endif /* } CBUG */

/*******************************************************/
#ifdef HAS_BLOCK /* { */
void KS_block(TASK starttask, TASK endtask)
{
   struct blkarg p;

   p.ksnum = RTXC_BLOCK;
   p.starttask = starttask;
   p.endtask = endtask;

   KS(p);
}
#endif /* } HAS_BLOCK */

/*******************************************************/
const char * KS_version(void)
{
   return((const char *)"RTXC v3.2d XA-G3/Tasking v2.0 Mar-26-98 KEY:22361");
}

/*******************************************************/
#ifdef HAS_NOP /* { */
void KS_nop(void)
{
   struct targ p;

   p.ksnum = RTXC_NOP;
   KS(p);
}
#endif /* } HAS_NOP */

/*******************************************************/
#ifdef HAS_ALLOC /* { */
void * KS_alloc(MAP map)
{
   struct parg p;
#ifdef FASTALLOC /* { */
   PHEADER ks_tbl *pph;
   unsigned char pswh;  /* local variable for saving processor priority level */

   pph = (PHEADER ks_tbl *)&pheader[map];

   PUSHPSWH;  /* save processor priority level */
   DISABLE;  /* disable interrupts */

   if ( (p.val = (char *)pph->next) != NULL ) /* if any available */
   {
      pph->next = ((struct xmap *)p.val)->link; /* unlink 1st from list */
#ifdef CBUG /* { */
      if (++pph->cur > pph->worst) /* check for new worst case */
         pph->worst = pph->cur;          /* useage level */
#endif /* } CBUG */
   }
   POPPSWH;  /* restore processor priority level */
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
CLKBLK ks_clk * KS_alloc_timer(void)
{
   struct clkarg p;

   p.ksnum = RTXC_ALLOC_TIMER;
   KS(p);
   return(p.pclkblk);
}
#endif /* } HAS_ALLOC_TIMER */

/*******************************************************/
#ifdef HAS_DEFPRIORITY /* { */
void KS_defpriority(TASK task, PRIORITY priority)
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
void KS_defslice(TASK task, TICKS ticks)
{
   TCB ks_tbl *ptcb;

   if (task == SELFTASK)
      ptcb = hipritsk;
   else
      ptcb = (TCB ks_tbl *)&rtxtcb[task];

   /* if slicing is just becoming active */
   if (ptcb->newslice == (TSLICE)0)
   {
      ptcb->tslice = ticks;
   }
   ptcb->newslice = ticks;
}
#endif /* } TIME_SLICE */

/*******************************************************/
#ifdef HAS_DELAY /* { */
void KS_delay(TASK task, TICKS ticks)
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
KSRC KS_dequeue(QUEUE queue, void *data)
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
KSRC KS_enqueue(QUEUE queue, void *data)
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
void KS_execute(TASK task)
{
   struct targ p;

   p.ksnum = RTXC_EXECUTE;
   p.task = task;
   KS(p);
}

/*******************************************************/
#ifdef HAS_DEFTASK /* { */
KSRC KS_deftask(TASK task, PRIORITY priority,
                char ks_stk *stackbase, size_t stacksize,
                void (*entry)(void))
{
   struct deftaskarg p;

#ifdef CBUG
   stkinit((short ks_stk *)stackbase, stacksize);
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
TASK KS_alloc_task(void)
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
void KS_free(MAP map, void *val)
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
MAP KS_alloc_part(void)
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
void KS_defpart(MAP map, void *addr, size_t blksize, size_t nblocks)
{
   struct parg p;

   p.ksnum = RTXC_DEFPART;
   p.map = map;
   p.addr = addr;
   p.size = blksize;
   p.nblocks = nblocks;
   KS(p);
}
#endif /* } HAS_DEFPART */

/*******************************************************/
#ifdef HAS_FREE_PART /* { */
void * KS_free_part(MAP map)
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
void KS_free_timer(CLKBLK ks_clk *pclkblk)
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
KSRC KS_lock(RESOURCE resource)
{
   struct larg p;
#ifdef FASTLOCK /* { */
   RHEADER ks_tbl *prh;
   unsigned char pswh;  /* local variable for saving processor priority level */

   prh = (RHEADER ks_tbl *)&rheader[resource];

   PUSHPSWH;  /* save processor priority level */
   DISABLE;

   if (prh->owner == NULLTCB)  /* if resource not currently owned */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->owner = hipritsk; /* lock resource with task # */
      prh->level = 1;         /* set nesting level = 1 */
      POPPSWH;  /* restore processor priority level */
      return(RC_GOOD);        /* mark successful */
   }

   if (prh->owner == hipritsk) /* if already owned by caller */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->level++;           /* bump nesting level */
      POPPSWH;  /* restore processor priority level */
      return(RC_NESTED);      /* mark successful */
   }
   POPPSWH;  /* restore processor priority level */
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
void KS_pend(SEMA sema)
{
   struct sarg p;

   p.ksnum = RTXC_PEND;
   p.sema = sema;
   KS(p);
}
#endif /* } HAS_PEND */

/*******************************************************/
#ifdef HAS_RECEIVE /* { */
RTXCMSG * KS_receive(MBOX mbox, TASK task)
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
void KS_resume(TASK task)
{
   struct targ p;

   p.ksnum = RTXC_RESUME;
   p.task = task;
   KS(p);
}
#endif /* } HAS_RESUME */

/*******************************************************/
#ifdef HAS_SEND /* { */
void KS_send(MBOX mbox, RTXCMSG *prtxcmsg, PRIORITY priority, SEMA sema)
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
KSRC KS_signal(SEMA sema)
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
void KS_ack(RTXCMSG *prtxcmsg)
{

   KS_signal(prtxcmsg->sema);

}
#endif /* } HAS_ACK */

/*******************************************************/
#ifdef HAS_STOP_TIMER /* { */
KSRC KS_stop_timer(CLKBLK ks_clk *pclkblk)
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
CLKBLK ks_clk * KS_start_timer(CLKBLK ks_clk *pclkblk, TICKS ticks,
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
void KS_suspend(TASK task)
{
   struct targ p;

   p.ksnum = RTXC_SUSPEND;
   p.task = task;
   KS(p);
}
#endif /* } HAS_SUSPEND */

/*******************************************************/
#ifdef HAS_TERMINATE /* { */
void KS_terminate(TASK task)
{
   struct targ p;

   p.ksnum = RTXC_TERMINATE;
   p.task = task;
   KS(p);
}
#endif /* } HAS_TERMINATE */

/*******************************************************/
#ifdef HAS_UNBLOCK /* { */
void KS_unblock(TASK starttask, TASK endtask)
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
KSRC KS_unlock(RESOURCE resource)
{
   struct larg p;
#ifdef FASTLOCK /* { */
   RHEADER ks_tbl *prh;
   unsigned char pswh;  /* local variable for saving processor priority level */

   prh = (RHEADER ks_tbl *)&rheader[resource];

   PUSHPSWH;  /* save processor priority level */
   DISABLE;

   if (prh->owner != hipritsk) /* if hipritsk not owner, then nop */
   {
      POPPSWH; /* fast return */
      return(RC_BUSY);
   }

   if (prh->level > 1) /* if nesting level > 1 */
   {
      prh->level--;   /* decrement nesting level */
      POPPSWH;              /* fast return */
      return(RC_NESTED);
   }

#ifdef RESOURCE_WAITERS /* { */
   if (prh->waiters == NULLTCB) /* if no tasks are waiting */
   {
      prh->level = 0;  /* zero nesting level */
      prh->owner = NULLTCB; /* mark resource not owned */
      POPPSWH;              /* fast return */
      return(RC_GOOD);
   }
#endif /* } RESOURCE_WAITERS */

   POPPSWH;              /* fast return */
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
int KS_user(int (*fun)(void *), void *arg)
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
KSRC KS_wait(SEMA sema)
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
KSRC KS_yield(void)
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
void * KS_allocw(MAP map)
{
   struct parg p;
#ifdef FASTALLOC /* { */
   PHEADER ks_tbl *pph;
   unsigned char pswh;  /* local variable for saving processor priority level */

   pph = (PHEADER ks_tbl *)&pheader[map];

   PUSHPSWH;  /* save processor priority level */
   DISABLE;

   if ( (p.val = (char *)pph->next) != NULL ) /* if any available */
   {
      pph->next = ((struct xmap *)p.val)->link; /* unlink 1st from list */
#ifdef CBUG /* { */
      if (++pph->cur > pph->worst) /* check for new worst case */
         pph->worst = pph->cur;          /* useage level */
#endif /* } CBUG */
      POPPSWH;
      return(p.val);
   }
   POPPSWH;
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
MAP KS_create_part(void *addr, size_t blksize, size_t nblocks)
{
   struct parg p;

   p.ksnum = RTXC_CREATE_PART;
   p.addr = addr;
   p.size = blksize;
   p.nblocks = nblocks;
   KS(p);

   /* return map # (0 = no dynamic partition headers available) */
   return(p.map);
}
#endif /* } HAS_CREATE_PART */

/*******************************************************/
#ifdef HAS_INQRES /* { */
TASK KS_inqres(RESOURCE resource)
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
KSRC KS_defres(RESOURCE resource, RESATTR resattr)
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
void * KS_inqtask_arg(TASK task)
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
void KS_deftask_arg(TASK task, void *arg)
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
void KS_defmboxsema(MBOX mbox, SEMA sema)
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
void KS_defqsema(QUEUE queue, SEMA sema, QCOND qcond)
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
void KS_dequeuew(QUEUE queue, void *data)
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
TICKS KS_elapse(TICKS *stamp)
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
void KS_enqueuew(QUEUE queue, void *data)
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
size_t KS_inqmap(MAP map)
{
   return(pheader[map].size);
}
#endif /* } HAS_INQMAP */

/*******************************************************/
#ifdef HAS_LOCKW /* { */
KSRC KS_lockw(RESOURCE resource)
{
   struct larg p;
#ifdef FASTLOCK /* { */
   RHEADER ks_tbl *prh;
   unsigned char pswh;  /* local variable for saving processor priority level */

   prh = (RHEADER ks_tbl *)&rheader[resource];

   PUSHPSWH;  /* save processor priority level */
   DISABLE;

   if (prh->owner == NULLTCB)  /* if resource not currently owned */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->owner = hipritsk; /* lock resource with task # */
      prh->level = 1;         /* set nesting level = 1 */
      POPPSWH;                 /* fast return */
      return(RC_GOOD);
   }

   if (prh->owner == hipritsk) /* if already owned by caller */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->level++;           /* bump nesting level */
      POPPSWH;                 /* fast return */
      return(RC_NESTED);
   }
   POPPSWH;
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
void KS_purgequeue(QUEUE queue)
{
   struct qarg p;

   p.ksnum = RTXC_PURGEQUEUE;
   p.queue = queue;
   KS(p);
}
#endif /* } HAS_PURGEQUEUE */

/*******************************************************/
#ifdef HAS_RECEIVEW /* { */
RTXCMSG * KS_receivew(MBOX mbox, TASK task)
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
void KS_sendw(MBOX mbox, RTXCMSG *prtxcmsg, PRIORITY priority, SEMA sema)
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
SEMA KS_waitm(const SEMA *semalist)
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
time_t KS_inqtime(void)
{
   struct timearg p;

   p.ksnum = RTXC_INQTIME;
   KS(p);
   return(p.time);
}
#endif /* } HAS_INQTIME */

/*******************************************************/
#ifdef HAS_DEFTIME /* { */
void KS_deftime(time_t time)
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
TICKS KS_inqtimer(CLKBLK ks_clk *pclkblk)
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
void * KS_alloct(MAP map, TICKS ticks, KSRC *pksrc)
{
   struct parg p;
   CLKBLK clkblk;
#ifdef FASTALLOC /* { */
   PHEADER ks_tbl *pph;
   unsigned char pswh;  /* local variable for saving processor priority level */

   pph = (PHEADER ks_tbl *)&pheader[map];

   PUSHPSWH;  /* save processor priority level */
   DISABLE;

   if ( (p.val = (char *)pph->next) != NULL ) /* if any available */
   {
      pph->next = ((struct xmap *)p.val)->link; /* unlink 1st from list */
#ifdef CBUG /* { */
      if (++pph->cur > pph->worst) /* check for new worst case */
         pph->worst = pph->cur;          /* useage level */
#endif /* } CBUG */
      POPPSWH;
      *pksrc = RC_GOOD;
      return(p.val);
   }
   POPPSWH;
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
KSRC KS_defqueue(QUEUE queue, size_t width, int depth,
		 void ks_queue *base, int current_size)
{
   struct qdefarg p;

   p.ksnum = RTXC_DEFQUEUE;
   p.queue = queue;
   p.width = width;
   p.depth = depth;
   p.base = (char ks_queue *)base;
   p.current_size = current_size;
   p.ksrc = RC_GOOD;
   KS(p);
   return(p.ksrc);
}
#endif /* } HAS_DEFQUEUE */

/*******************************************************/
#ifdef HAS_DEQUEUET /* { */
KSRC KS_dequeuet(QUEUE queue, void *data, TICKS ticks)
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
KSRC KS_enqueuet(QUEUE queue, void *data, TICKS ticks)
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
PRIORITY KS_inqpriority(TASK task)
{
   if (task == SELFTASK)
      return(hipritsk->priority);
   else
      return(rtxtcb[task].priority);
}
#endif /* } HAS_INQPRIORITY */

/*******************************************************/
#ifdef HAS_INQQUEUE /* { */
int KS_inqqueue(QUEUE queue)
{
   return(qheader[queue].curndx);
}
#endif /* } HAS_INQQUEUE */

/*******************************************************/
#ifdef HAS_INQSEMA /* { */
SSTATE KS_inqsema(SEMA sema)
{
   return(semat[sema]);
}
#endif /* } HAS_INQSEMA */

/*******************************************************/
#ifdef HAS_INQTASK /* { */
TASK KS_inqtask(void)
{
   return(hipritsk->task);
}
#endif /* } HAS_INQTASK */

/*******************************************************/
#ifdef HAS_LOCKT /* { */
KSRC KS_lockt(RESOURCE resource, TICKS ticks)
{

   struct larg p;
   CLKBLK clkblk;
#ifdef FASTLOCK /* { */
   RHEADER ks_tbl *prh;
   unsigned char pswh;  /* local variable for saving processor priority level */

   prh = (RHEADER ks_tbl *)&rheader[resource];

   PUSHPSWH;  /* save processor priority level */
   DISABLE;

   if (prh->owner == NULLTCB)  /* if resource not currently owned */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->owner = hipritsk; /* lock resource with task # */
      prh->level = 1;         /* set nesting level = 1 */
      POPPSWH;                 /* fast return */
      return(RC_GOOD);        /* mark successful */
   }

   if (prh->owner == hipritsk) /* if already owned by caller */
   {
#ifdef CBUG /* { */
      prh->count++;           /* update statistics */
#endif /* } CBUG */
      prh->level++;           /* bump nesting level */
      POPPSWH;                 /* fast return */
      return(RC_NESTED);      /* mark successful */
   }
   POPPSWH;
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
void KS_pendm(const SEMA *semalist)
{
   struct sargm p;

   p.ksnum = RTXC_PENDM;
   p.list = semalist;
   KS(p);
}
#endif /* } HAS_PENDM */

/*******************************************************/
#ifdef HAS_RECEIVET /* { */
RTXCMSG * KS_receivet(MBOX mbox, TASK task, TICKS ticks, KSRC *pksrc)
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
KSRC KS_restart_timer(CLKBLK ks_clk *pclkblk, TICKS ticks, TICKS period)
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
KSRC KS_sendt(MBOX mbox, RTXCMSG *prtxcmsg, PRIORITY priority, SEMA sema,
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
void KS_signalm(const SEMA *semalist)
{
   struct sargm p;

   p.ksnum = RTXC_SIGNALM;
   p.list = semalist;
   KS(p);
}
#endif /* } HAS_SIGNALM */

/*******************************************************/
#ifdef HAS_WAITT /* { */
KSRC KS_waitt(SEMA sema, TICKS ticks)
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
TICKS KS_inqslice(TASK task)
{
   if (task == SELFTASK)
      task = hipritsk->task;

   return(rtxtcb[task].newslice);
}
#endif /* } HAS_INQSLICE */
#endif /* } TIME_SLICE */
        
#endif /* } RTXC_EL */

/* end of rtxcapi.c */
