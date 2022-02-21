/* rtxcarg.h - RTXC argument interface */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/* KS opcode numbers used in rtxc() and C interface library */

#ifndef _RTXCARG_H
#define _RTXCARG_H

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

typedef enum
{
   /* leave 0 "open" to catch bogus calls */
   RTXC_NOP = 0,

   RTXC_WAIT,
   RTXC_SIGNAL,
#ifdef HAS_PEND /* { */
   RTXC_PEND,
#else /* } HAS_PEND */
   q8q_xxx1,
#endif /* } HAS_PEND */

#ifdef HAS_MAILBOXES /* { */
   RTXC_SEND,
   RTXC_RECEIVE,
#else /* } HAS_MAILBOXES */
   q8q_xxx2,
   q8q_xxx3,
#endif /* } HAS_MAILBOXES */

#ifdef HAS_PARTITIONS /* { */
   RTXC_ALLOC,
   RTXC_FREE,
#ifdef HAS_CREATE_PART /* { */
   RTXC_CREATE_PART,
#else /* } HAS_CREATE_PART */
   q8q_xxx6,
#endif /* } HAS_CREATE_PART */
#ifdef HAS_ALLOC_PART /* { */
   RTXC_ALLOC_PART,
#else /* } HAS_ALLOC_PART */
   q8q_xxx7,
#endif /* } HAS_ALLOC_PART */
#ifdef HAS_DEFPART /* { */
   RTXC_DEFPART,
#else /* } HAS_DEFPART */
   q8q_xxx8,
#endif /* } HAS_DEFPART */
#ifdef HAS_FREE_PART /* { */
   RTXC_FREE_PART,
#else /* } HAS_FREE_PART */
   q8q_xxx9,
#endif /* } HAS_FREE_PART */
#else /* } HAS_PARTITIONS */
   q8q_xxx4,
   q8q_xxx5,
   q8q_xxx6,
   q8q_xxx7,
   q8q_xxx8,
   q8q_xxx9,
#endif /* } HAS_PARTITIONS */

#ifdef HAS_QUEUES /* { */
   RTXC_ENQUEUE,
   RTXC_DEQUEUE,
#else /* } HAS_QUEUES */
   q8q_xxx10,
   q8q_xxx11,
#endif /* } HAS_QUEUES */

#ifdef HAS_RESOURCES /* { */
   RTXC_LOCK,
   RTXC_UNLOCK,
#else /* } HAS_RESOURCES */
   q8q_xxx12,
   q8q_xxx13,
#endif /* } HAS_RESOURCES */

#ifdef HAS_ALLOC_TIMER /* { */
   RTXC_ALLOC_TIMER,
#else /* } HAS_ALLOC_TIMER */
   q8q_xxx14,
#endif /* } HAS_ALLOC_TIMER */

#ifdef HAS_FREE_TIMER /* { */
   RTXC_FREE_TIMER,
#else /* } HAS_FREE_TIMER */
   q8q_xxx15,
#endif /* } HAS_FREE_TIMER */

#ifdef HAS_START_TIMER /* { */
   RTXC_START_TIMER,
#else /* } HAS_START_TIMER */
   q8q_xxx16,
#endif /* } HAS_START_TIMER */

#ifdef HAS_STOP_TIMER /* { */
   RTXC_STOP_TIMER,
#else /* } HAS_STOP_TIMER */
   q8q_xxx17,
#endif /* } HAS_STOP_TIMER */

#ifdef HAS_DELAY /* { */
   RTXC_DELAY,
#else /* } HAS_DELAY */
   q8q_xxx18,
#endif /* } HAS_DELAY */

   RTXC_EXECUTE,

#ifdef HAS_DEFTASK /* { */
   RTXC_DEFTASK,
#else /* } HAS_DEFTASK */
   q8q_xxx19,
#endif /* } HAS_DEFTASK */

#ifdef HAS_ALLOC_TASK /* { */
   RTXC_ALLOC_TASK,
#else /* } HAS_ALLOC_TASK */
   q8q_xxx20,
#endif /* } HAS_ALLOC_TASK */

#ifdef HAS_TERMINATE /* { */
   RTXC_TERMINATE,
#else /* } HAS_TERMINATE */
   q8q_xxx21,
#endif /* } HAS_TERMINATE */

#ifdef HAS_SUSPEND /* { */
   RTXC_SUSPEND,
#else /* } HAS_SUSPEND */
   q8q_xxx22,
#endif /* } HAS_SUSPEND */

#ifdef HAS_RESUME /* { */
   RTXC_RESUME,
#else /* } HAS_RESUME */
   q8q_xxx23,
#endif /* } HAS_RESUME */

#ifdef HAS_DEFPRIORITY /* { */
   RTXC_DEFPRIORITY,
#else /* } HAS_DEFPRIORITY */
   q8q_xxx24,
#endif /* } HAS_DEFPRIORITY */

#ifdef HAS_YIELD /* { */
   RTXC_YIELD,
#else /* } HAS_YIELD */
   q8q_xxx25,
#endif /* } HAS_YIELD */

#ifdef HAS_BLOCK /* { */
   RTXC_BLOCK,
#else /* } HAS_BLOCK */
   q8q_xxx26,
#endif /* } HAS_BLOCK */

#ifdef HAS_UNBLOCK /* { */
   RTXC_UNBLOCK,
#else /* } HAS_UNBLOCK */
   q8q_xxx27,
#endif /* } HAS_UNBLOCK */

/* RTXC_AL */
/*******************************************************/

#ifdef HAS_DEFMBOXSEMA /* { */
   RTXC_DEFMBOXSEMA,
#else /* } HAS_DEFMBOXSEMA */
   q8q_xxx28,
#endif /* } HAS_DEFMBOXSEMA */

#ifdef HAS_PURGEQUEUE /* { */
   RTXC_PURGEQUEUE,
#else /* } HAS_PURGEQUEUE */
   q8q_xxx29,
#endif /* } HAS_PURGEQUEUE */

#ifdef HAS_DEFQSEMA /* { */
   RTXC_DEFQSEMA,
#else /* } HAS_DEFQSEMA */
   q8q_xxx30,
#endif /* } HAS_DEFQSEMA */

#ifdef HAS_WAITM /* { */
   RTXC_WAITM,
#else /* } HAS_WAITM */
   q8q_xxx31,
#endif /* } HAS_WAITM */

#ifdef HAS_ELAPSE /* { */
   RTXC_ELAPSE,
#else /* } HAS_ELAPSE */
   q8q_xxx32,
#endif /* } HAS_ELAPSE */

#ifdef HAS_INQTIME /* { */
   RTXC_INQTIME,
#else /* } HAS_INQTIME */
   q8q_xxx33,
#endif /* } HAS_INQTIME */

#ifdef HAS_DEFTIME /* { */
   RTXC_DEFTIME,
#else /* } HAS_DEFTIME */
   q8q_xxx34,
#endif /* } HAS_DEFTIME */

#ifdef HAS_RESOURCES /* { */
#ifdef HAS_INQRES /* { */
   RTXC_INQRES,
#else /* } HAS_INQRES */
   q8q_xxx35,
#endif /* } HAS_INQRES */
#ifdef HAS_DEFRES /* { */
   RTXC_DEFRES,
#else /* } HAS_DEFRES */
   q8q_xxx36,
#endif /* } HAS_DEFRES */
#else /* } HAS_RESOURCES */
   q8q_xxx35,
   q8q_xxx36,
#endif /* } HAS_RESOURCES */

#ifdef HAS_INQTASK_ARG /* { */
   RTXC_INQTASK_ARG,
#else /* } HAS_INQTASK_ARG */
   q8q_xxx37,
#endif /* } HAS_INQTASK_ARG */

#ifdef HAS_DEFTASK_ARG /* { */
   RTXC_DEFTASK_ARG,
#else /* } HAS_DEFTASK_ARG */
   q8q_xxx38,
#endif /* } HAS_DEFTASK_ARG */

/* end RTXC_AL */

/* RTXC_EL */
/*******************************************************/

#ifdef HAS_INQTIMER /* { */
   RTXC_INQTIMER,
#else /* } HAS_INQTIMER */
   q8q_xxx39,
#endif /* } HAS_INQTIMER */

#ifdef HAS_SIGNALM /* { */
   RTXC_SIGNALM,
#else /* } HAS_SIGNALM */
   q8q_xxx40,
#endif /* } HAS_SIGNALM */

#ifdef HAS_PENDM /* { */
   RTXC_PENDM,
#else /* } HAS_PENDM */
   q8q_xxx41,
#endif /* } HAS_PENDM */

#ifdef HAS_RESTART_TIMER /* { */
   RTXC_RESTART_TIMER,
#else /* } HAS_RESTART_TIMER */
   q8q_xxx42,
#endif /* } HAS_RESTART_TIMER */

#ifdef HAS_DEFQUEUE /* { */
   RTXC_DEFQUEUE,
#else /* } HAS_DEFQUEUE */
   q8q_xxx43,
#endif /* } HAS_DEFQUEUE */

/* end RTXC_EL */

/*******************************************************/

   RTXC_USER    /* user extensions */

/* additional RTXC functions may be added here by user */
} RTXC_KSNUM;

/* RTXC argument packets */

/* semaphore directives */
struct sarg /* semaphore argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   SEMA sema;
   TICKS ticks;
   CLKBLK ks_clk *pclkblk;
};

/* multiple semaphore directives */
struct sargm /* multiple semaphores argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   SEMA sema;
   const SEMA *list;
};

#ifdef HAS_QUEUES /* { */
/* queue directives */
struct qarg /* queue argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   SEMA sema;
   QUEUE queue;
   void *data;
#ifdef QUEUE_WAITERS /* { */
   KBOOL wait;
#endif /* } QUEUE_WAITERS */
#ifdef QUEUE_TIMEOUTS /* { */
   TICKS ticks;
   CLKBLK ks_clk *pclkblk;
#endif /* } QUEUE_TIMEOUTS */
};

/* define queue directives */
struct qdefarg /* queue definition argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   QUEUE queue;
   char ks_queue *base;
   size_t width;
   int depth;
   int current_size;
   SEMA sema;
#ifdef QUEUE_SEMAS /* { */
   QCOND qcond;
#endif /* } QUEUE_SEMAS */
};
#endif /* } HAS_QUEUES */

#ifdef HAS_PARTITIONS /* { */
struct parg /* partition argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   MAP map;
   void *val;
   size_t size;
#ifdef PARTITION_WAITERS /* { */
   KBOOL wait;
#endif /* } PARTITION_WAITERS */
#ifdef PARTITION_TIMEOUTS /* { */
   TICKS ticks;
   CLKBLK ks_clk *pclkblk;
#endif /* } PARTITION_TIMEOUTS */
#ifdef DYNAMIC_PARTS /* { */
   char *addr;
   size_t nblocks;
#endif /* } DYNAMIC_PARTS */
};
#endif /* } HAS_PARTITIONS */

#ifdef HAS_MAILBOXES /* { */
struct msgarg /* message argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   SEMA sema;
   MBOX mbox;
   TASK task;
   PRIORITY priority;
   RTXCMSG *prtxcmsg; /* ptr to msg overhead */
   KBOOL wait; /* 0 - nowait, 1 - wait: used by SENDW, too */
#ifdef MAILBOX_TIMEOUTS /* { */
   TICKS ticks;
   CLKBLK ks_clk *pclkblk;
#endif /* } MAILBOX_TIMEOUTS */
};
#endif /* } HAS_MAILBOXES */

struct targ /* task control argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   TASK task;
   PRIORITY priority;
};

struct clkarg /* clock argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   SEMA sema;
   TICKS ticks;
   TICKS period;
   CLKBLK ks_clk *pclkblk;
};

struct timearg /* Time argument packet */
{
   KSNUM ksnum;
   time_t time;
};

struct delayarg /* delay argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   TASK task;
   TICKS ticks;
   CLKBLK ks_clk *pclkblk;
};

struct etarg /* elapse argument packet */
{
   KSNUM ksnum;
   TICKS *stamp;
   TICKS val;
};

struct blkarg /* block task(s) argument packet */
{
   KSNUM ksnum;
   TASK starttask;
   TASK endtask;
};

#ifdef HAS_RESOURCES /* { */
struct larg /* lock resource argument packet */
{
   KSNUM ksnum;
   KSRC ksrc;
   RESOURCE resource;
#ifdef RESOURCE_WAITERS /* { */
   KBOOL wait;
#endif /* } RESOURCE_WAITERS */
#ifdef RESOURCE_TIMEOUTS /* { */
   TICKS ticks;
   CLKBLK ks_clk *pclkblk;
#endif /* } RESOURCE_TIMEOUTS */
#ifdef HAS_INQRES /* { */
   TASK task;
#endif /* } HAS_INQRES */
#ifdef HAS_DEFRES /* { */
#ifdef PRIORITY_INVERSION /* { */
   RESATTR resattr;
#endif /* } PRIORITY_INVERSION */
#endif /* } HAS_DEFRES */
};
#endif /* } HAS_RESOURCES */

struct userarg /* user defined ESR */
{
   KSNUM ksnum;
   void *arg;   /* passed to (*fun)() */
   int (*fun)(void *);
   int val; /* returned from (*fun)() */
};

#ifdef HAS_DEFTASK /* { */
struct deftaskarg
{
   KSNUM ksnum;
   KSRC ksrc;
   TASK task;
   PRIORITY priority;
   char ks_stk *stackbase;
   size_t stacksize;
   void (*entry)(void);
   void *arg;
};
#endif /* } HAS_DEFTASK */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _RTXCARG_H */

/* end of rtxcarg.h */
