/* rtxcopts.h - RTXC compile time options */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _RTXCOPTS_H
#define _RTXCOPTS_H

#include "binding.h"    /* RTXC binding options */

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/************************************************************************

    define either RTXC_EL, RTXC_AL, or RTXC_BL below

***********************************************************************/
#define RTXC_EL /* indicates RTXC extended library support */
#undef RTXC_AL /* indicates RTXC advanced library support */
#undef RTXC_BL /* indicates RTXC basic library support */

/************************************************************************/

/************************************************************************

                  TIME_SLICE specification

The use of TIME_SLICEing provides for the automatic task switching by RTXC of
tasks  that are running at  the same task priority.   RTXC will automatically
switch to  the "next" runnable task  at the same priority  when a task's time
slice has expired.    On  each  clock  tick  interrupt,  the  current  task's
remaining time slice value is decremented.  If the value is exhausted (== 0),
then if the next ready task is at the same priority, then the current running
task is "moved to the end" of the Ready task list with respect to other tasks
at  the its priority.  The extra space overhead to support time slicing is an
extra two  members (TICK sized) in  each Task Control Block.   The extra time
overhead to support time slicing is minimal.

***********************************************************************/
#define TIME_SLICE /* select #define or #undef */

/************************************************************************

                  DYNAMIC_TASKS specification

The specification of DYNAMIC_TASKS adds support for the dynamic "creation" of
tasks at runtime.   Under RTXC,  the task  creation operation  is a  two step
operation.   First, RTXC assigns  a Task ID in  response to a KS_alloc_task()
directive.  Next, a  KS_deftask() directive  must be  executed to  define the
task's  entry point and priority, stack address  and size, and a single "void
*" argument.  Like with static tasks, a task is started using KS_execute().

***********************************************************************/
#define DYNAMIC_TASKS /* select #define or #undef */

/************************************************************************

                  DYNAMIC_PARTS specification

The specification of DYNAMIC_PARTS adds support for the dynamic "creation" of
partitions at  runtime.  Under RTXC, the  partition creation operation can be
handled in  two ways.

1.   A one step operation, KS_create_part()  allocates a partition header and
defines a  partition according  to parameters  supplied by  the caller.   The
caller must supply a pointer to  a  block  of  memory  to  be  used  for  the
partition,  the size of each block in the partition, and the number of blocks
in  the partition.   KS_create_part() will return  a MAP number  that must be
used when using KS_free_part() to free the allocated partition header.  NOTE:
KS_create_part() is not available in the Basic Library configuration.

2.  A two step operation  involves  calling  KS_alloc_part()  to  allocate  a
partition header.   KS_alloc_part() returns a MAP  number which is used along
with the  memory block pointer, size  of blocks and number  of blocks to call
KS_defpart().

***********************************************************************/
#define DYNAMIC_PARTS /* select #define or #undef */

/******* sanity check for api library **************************/

#ifdef RTXC_EL /* { */
#define RTXC_AL
#endif /* } RTXC_EL */

#ifdef RTXC_AL /* { */
#define RTXC_BL
#endif /* } RTXC_AL */

#define HAS_MAILBOXES   /* choose #define or #undef */
#define HAS_PARTITIONS  /* choose #define or #undef */
#define HAS_QUEUES      /* choose #define or #undef */
#define HAS_RESOURCES   /* choose #define or #undef */

/*************************************************/
/*   Define the Mailbox Services to be included  */
/*************************************************/

#ifdef HAS_MAILBOXES /* { */
#define HAS_ACK       /* choose #define or #undef */
#define HAS_RECEIVE   /* REQUIRED */
#define HAS_SEND      /* REQUIRED */

#ifdef RTXC_AL /* { */
#define HAS_RECEIVEW  /* choose #define or #undef */
#define HAS_SENDW     /* choose #define or #undef */
#define HAS_DEFMBOXSEMA  /* choose #define or #undef */
#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */
#define HAS_RECEIVET  /* choose #define or #undef */
#define HAS_SENDT     /* choose #define or #undef */
#endif /* } RTXC_EL */

#endif /* } HAS_MAILBOXES */

/*************************************************/
/*  Define the Partition Services to be included */
/*************************************************/

#ifdef HAS_PARTITIONS /* { */

#define HAS_ALLOC    /* REQUIRED */
#define HAS_FREE     /* REQUIRED */

#ifdef RTXC_AL /* { */
#define HAS_ALLOCW   /* choose #define or #undef */
#define HAS_INQMAP   /* choose #define or #undef */

#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */
#define HAS_ALLOCT   /* choose #define or #undef */
#endif /* } RTXC_EL */

#ifdef DYNAMIC_PARTS /* { */
#define HAS_ALLOC_PART   /* must have HAS_ALLOC_PART --- and --- */
#define HAS_DEFPART      /* HAS_DEFPART */
#ifdef RTXC_AL /* { */
                        /* --- or --- */
#define HAS_CREATE_PART  /* must have HAS_CREATE_PART, may have all 3 */
#endif /* } RTXC_AL */
#define HAS_FREE_PART    /* REQUIRED */
#endif /* } DYNAMIC_PARTS */

#endif /* } HAS_PARTITIONS */

/*************************************************/
/*    Define the Queue Services to be included   */
/*************************************************/

#ifdef HAS_QUEUES /* { */

#define HAS_DEQUEUE    /* REQUIRED */
#define HAS_ENQUEUE    /* REQUIRED */

#ifdef RTXC_AL /* { RTXC_AL */
#define HAS_DEQUEUEW   /* choose #define or #undef */
#define HAS_ENQUEUEW   /* choose #define or #undef */
#define HAS_PURGEQUEUE /* choose #define or #undef */
#define HAS_DEFQSEMA   /* choose #define or #undef */
#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */
#define HAS_DEQUEUET   /* choose #define or #undef */
#define HAS_ENQUEUET   /* choose #define or #undef */
#define HAS_INQQUEUE   /* choose #define or #undef */
#define HAS_DEFQUEUE   /* choose #define or #undef */
#endif /* } RTXC_EL */

#endif /* } HAS_QUEUES */

/*************************************************/
/*  Define the Resource Services to be included  */
/*************************************************/

#ifdef HAS_RESOURCES /* { */

#define HAS_LOCK    /* REQUIRED */
#define HAS_UNLOCK  /* REQUIRED */

#ifdef RTXC_AL /* { */
#define HAS_LOCKW   /* choose #define or #undef */
#define HAS_INQRES /* choose #define or #undef */
#define PRIORITY_INVERSION  /* choose #define or #undef */
#define HAS_DEFRES /* choose #define or #undef */
#endif /* } RTXC_AL */

#ifdef RTXC_EL  /* { */
#define HAS_LOCKT    /* choose #define or #undef */
#endif /* } RTXC_EL */

#endif /* } HAS_RESOURCES */

/*************************************************/
/*  Define the Semaphore Services to be included */
/*************************************************/

#define HAS_PEND       /* choose #define or #undef */

#ifdef RTXC_AL /* { */
#define HAS_WAITM      /* choose #define or #undef */
#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */
#define HAS_INQSEMA    /* choose #define or #undef */
#define HAS_PENDM      /* choose #define or #undef */
#define HAS_SIGNALM    /* choose #define or #undef */
#define HAS_WAITT      /* choose #define or #undef */
#endif /* } RTXC_EL */

/*************************************************/
/*     Define the Task Services to be included   */
/*************************************************/

#define HAS_TERMINATE  /* choose #define or #undef */

#define HAS_SUSPEND    /* choose #define or #undef */
#define HAS_RESUME     /* choose #define or #undef */

#define HAS_BLOCK      /* choose #define or #undef */
#define HAS_UNBLOCK    /* choose #define or #undef */

#define HAS_DEFPRIORITY  /* choose #define or #undef */
#define HAS_YIELD        /* choose #define or #undef */

#define HAS_DEFTASK      /* choose #define or #undef */

#ifdef RTXC_AL /* { */
#define HAS_INQTASK_ARG  /* choose #define or #undef */
#define HAS_DEFTASK_ARG  /* choose #define or #undef, iff INQTASK_ARG */
#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */
#define HAS_INQTASK      /* choose #define or #undef */
#define HAS_INQPRIORITY  /* choose #define or #undef */
#endif /* } RTXC_EL */

/*************************************************/
/*    Define the Timer Services to be included   */
/*************************************************/

#define HAS_DELAY          /* choose #define or #undef */
#define HAS_ALLOC_TIMER    /* choose #define or #undef */
#define HAS_FREE_TIMER     /* choose #define or #undef */
#define HAS_START_TIMER    /* choose #define or #undef */
#define HAS_STOP_TIMER     /* choose #define or #undef */

#ifdef RTXC_AL /* { */
#define HAS_ELAPSE         /* choose #define or #undef */
#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */
#define HAS_RESTART_TIMER  /* choose #define or #undef */
#define HAS_INQTIMER       /* choose #define or #undef */
#endif /* } RTXC_EL */

/*************************************************/
/*   Define the Special Services to be included  */
/*************************************************/

#define HAS_USER       /* choose #define or #undef */
#define HAS_NOP        /* choose #define or #undef */

#ifdef RTXC_AL /* { */
#define HAS_DEFTIME    /* choose #define or #undef */
#define HAS_INQTIME    /* choose #define or #undef */
#endif /* } RTXC_AL */

/*************************************************/
/*   Define the Other features to be included    */
/*************************************************/

#ifdef TIME_SLICE /* { */

#define HAS_DEFSLICE   /* REQUIRED */

#ifdef RTXC_EL /* { */
#define HAS_INQSLICE   /* choose #define or #undef */
#endif /* } RTXC_EL */

#endif /* } TIME_SLICE */

#ifdef DYNAMIC_TASKS /* { */
#define HAS_ALLOC_TASK   /* REQUIRED */
#endif /* } DYNAMIC_TASKS */

/*******************************************************/
/*                                                     */
/*             Start of sanity checks here             */
/*                                                     */
/*        !! DO NOT CHANGE THESE CHECKS BELOW !!       */
/*                                                     */
/*******************************************************/
#ifdef HAS_MAILBOXES /* { */

#ifdef RTXC_AL /* { */

#ifdef HAS_RECEIVEW /* { */
#define MAILBOX_WAITERS
#endif /* } HAS_RECEIVEW */

#ifdef HAS_DEFMBOXSEMA /* { */
#define MAILBOX_SEMAS
#endif /* } HAS_DEFMBOXSEMA */

#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */

#ifdef HAS_RECEIVET /* { */
#define MAILBOX_TIMEOUTS
#ifndef MAILBOX_WAITERS /* { */
#define MAILBOX_WAITERS
#endif /* } MAILBOX_WAITERS */
#endif /* } HAS_RECEIVET */

#endif /* } RTXC_EL */

#endif /* } HAS_MAILBOXES */

/*******************************************************/
#ifdef HAS_PARTITIONS /* { */

#ifdef RTXC_AL /* { */

#ifdef HAS_ALLOCW /* { */
#define PARTITION_WAITERS
#endif /* } HAS_ALLOCW */

#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */

#ifdef HAS_ALLOCT /* { */
#define PARTITION_TIMEOUTS
#ifndef PARTITION_WAITERS /* { */
#define PARTITION_WAITERS
#endif /* } PARTITION_WAITERS */
#endif /* } HAS_ALLOCT */

#endif /* } RTXC_EL */

#endif /* } HAS_PARTITIONS */

/*******************************************************/
#ifdef HAS_QUEUES /* { */

#ifdef RTXC_AL /* { */

#ifdef HAS_ENQUEUEW /* { */
#define ENQUEUE_WAITERS
#endif /* } HAS_ENQUEUEW */
#ifdef HAS_DEQUEUEW /* { */
#define DEQUEUE_WAITERS
#endif /* } HAS_DEQUEUEW */

#if defined (ENQUEUE_WAITERS) || defined (DEQUEUE_WAITERS) /* { */
#define QUEUE_WAITERS
#endif /* - } (ENQUEUE_WAITERS || DEQUEUE_WAITERS) */

#ifdef HAS_DEFQSEMA /* { */
#define QUEUE_SEMAS
#endif /* } HAS_DEFQSEMA */

#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */

#ifdef HAS_ENQUEUET /* { */
#define ENQUEUE_TIMEOUTS
#endif /* } HAS_ENQUEUET */
#ifdef HAS_DEQUEUET /* { */
#define DEQUEUE_TIMEOUTS
#endif /* } HAS_DEQUEUET */

#ifdef ENQUEUE_TIMEOUTS /* { */
#ifndef ENQUEUE_WAITERS /* { */
#define ENQUEUE_WAITERS
#endif /* } ENQUEUE_WAITERS */
#endif /* } ENQUEUE_TIMEOUTS */

#ifdef DEQUEUE_TIMEOUTS /* { */
#ifndef DEQUEUE_WAITERS /* { */
#define DEQUEUE_WAITERS
#endif /* } DEQUEUE_WAITERS */
#endif /* } DEQUEUE_TIMEOUTS */

#ifndef QUEUE_WAITERS /* { */
#if defined (ENQUEUE_WAITERS) || defined (DEQUEUE_WAITERS) /* { */
#define QUEUE_WAITERS
#endif /* - } (ENQUEUE_WAITERS || DEQUEUE_WAITERS) */
#endif /* } QUEUE_WAITERS */

#if defined (ENQUEUE_TIMEOUTS) || defined (DEQUEUE_TIMEOUTS) /* { */
#define QUEUE_TIMEOUTS
#endif /* - } (ENQUEUE_TIMEOUTS || DEQUEUE_TIMEOUTS) */

#endif /* } RTXC_EL */

#endif /* } HAS_QUEUES */

/*******************************************************/
#ifdef HAS_RESOURCES /* { */

#ifdef RTXC_AL /* { */

#ifdef HAS_LOCKW /* { */
#define RESOURCE_WAITERS
#endif /* } HAS_LOCKW */
#endif /* } RTXC_AL */

#ifdef RTXC_EL /* { */

#ifdef HAS_LOCKT /* { */
#define RESOURCE_TIMEOUTS
#ifndef RESOURCE_WAITERS /* { */
#define RESOURCE_WAITERS
#endif /* } RESOURCE_WAITERS */
#endif /* } HAS_LOCKT */

#endif /* } RTXC_EL */

#endif /* } HAS_RESOURCES */

/*******************************************************/
#ifdef RTXC_EL /* { */

#ifdef HAS_WAITT /* { */
#define SEMAPHORE_TIMEOUTS  /* choose #define or #undef */
#endif /* } HAS_WAITT */

#endif /* } RTXC_EL */

/*******************************************************/
#ifdef RTXC_EL /* { */

#ifdef HAS_STOP_TIMER /* { */
#ifndef HAS_START_TIMER /* { */
#define HAS_START_TIMER
#endif /* } HAS_START_TIMER */
#endif /* } HAS_STOP_TIMER */

#ifdef HAS_RESTART_TIMER /* { */
#ifndef HAS_START_TIMER /* { */
#define HAS_START_TIMER
#endif /* } HAS_START_TIMER */
#endif /* } HAS_RESTART_TIMER */

#endif /* } RTXC_EL */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _RTXCOPTS_H */

/* End of file - rtxcopts.h */
