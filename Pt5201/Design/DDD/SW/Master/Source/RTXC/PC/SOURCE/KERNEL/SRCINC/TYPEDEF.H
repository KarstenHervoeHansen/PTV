/* typedef.h - RTXC typedef definitions */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#define ks_clk ks_stk

/* application level typedefs */

#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#define STACK_FILL	22359

#define STK_ALIGNTYPE short far

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#include "rtxcopts.h"

#ifndef _SIZE_T /* per Borland 3.x conventions */
#define _SIZE_T_
typedef unsigned size_t;
#endif

/*

    The following typedefs, MBOX, PARTITION, QUEUE, RESOURCE, SEMA, and TASK,
could  be defined as char, short, int or  long.  Char is typically better for
microcontrollers, else int is preferable.  If char is used for TASK, you must
make  sure that the default char mode for the compiler is signed char because
unsigned  char will not work for SSTATE=TASK.  Alternatively, specify "signed
char" in the typedef.

*/

typedef int OBJNUM;

typedef OBJNUM MBOX;

typedef OBJNUM MAP;

typedef OBJNUM QUEUE;

typedef OBJNUM RESOURCE;

typedef OBJNUM TASK;
typedef OBJNUM SEMA;

/* maximum timer length limited by largest possible value of type TICKS */

typedef long TICKS;

/* provide time_t iff missing from compiler */
#ifndef _TIME_T /* per Borland 3.x convention */
#define _TIME_T
typedef long time_t; /* seconds since Jan 1, 1901 */
#endif

typedef TASK PRIORITY; /* used for task + message priority */

#define NULLTASK_PRIORITY 0x7f /* lowest priority supported */

/* system level typedefs */

typedef unsigned short TSTATE; /* at least 16 bits */

typedef TASK SSTATE; /* must be signed, see SEMA_PENDING below */

/* semaphore states */
#define SEMA_PENDING ((SSTATE)-1)
#define SEMA_DONE    ((SSTATE)0)

#ifdef QUEUE_SEMAS /* { */
typedef enum
{
   QNE = 0, QNF, QF, QE
} QCOND;
#endif /* } QUEUE_SEMAS */

/* set of rtxc return codes */
typedef enum
{
   RC_GOOD = 0,
   RC_WAIT_CONFLICT,
   RC_NO_TIMERS,
   RC_TIMER_INACTIVE,
   RC_TIMER_ILLEGAL,
   RC_TIMEOUT,
#ifdef HAS_QUEUES /* { */
   RC_QUEUE_FULL,
   RC_QUEUE_EMPTY,
#ifdef HAS_DEFQUEUE /* { */
   RC_ILLEGAL_QUEUE_SIZE,
#else /* } HAS_DEFQUEUE  */
   q8q_yyy3,
#endif /* } HAS_DEFQUEUE */
#else /* } HAS_QUEUES  */
   q8q_yyy1,
   q8q_yyy2,
   q8q_yyy3,
#endif /* } HAS_QUEUES */
#ifdef HAS_RESOURCES /* { */
   RC_BUSY,
   RC_NESTED,
#else /* } HAS_RESOURCES */
   q8q_yyy4,
   q8q_yyy5,
#endif /* } HAS_RESOURCES */
#ifdef HAS_DEFTASK /* { */
   RC_ILLEGAL_TASK,
   RC_ACTIVE_TASK,
#else /* } HAS_DEFTASK */
   q8q_yyy6,
   q8q_yyy7,
#endif /* } HAS_DEFTASK */
#ifdef HAS_YIELD /* { */
   RC_NO_YIELD,
#else /* } HAS_YIELD */
   q8q_yyy8,
#endif /* } HAS_YIELD */
   RC_MISSED_EVENT
} KSRC;

/* timer types */
typedef enum
{
   DELAY_OBJ = 0,

#ifdef SEMAPHORE_TIMEOUTS /* { */
   SEMAPHORE_OBJ,
#else /* } SEMAPHORE_TIMEOUTS */
   q8q_zzz1,
#endif /* } SEMAPHORE_TIMEOUTS */

#ifdef PARTITION_TIMEOUTS /* { */
   PARTITION_OBJ,
#else /* } PARTITION_TIMEOUTS */
   q8q_zzz2,
#endif /* } PARTITION_TIMEOUTS */

#ifdef QUEUE_TIMEOUTS /* { */
   QUEUE_OBJ,
#else /* } QUEUE_TIMEOUTS */
   q8q_zzz3,
#endif /* } QUEUE_TIMEOUTS */

#ifdef RESOURCE_TIMEOUTS /* { */
   RESOURCE_OBJ,
#else /* } RESOURCE_TIMEOUTS */
   q8q_zzz4,
#endif /* } RESOURCE_TIMEOUTS */

#ifdef MAILBOX_TIMEOUTS /* { */
   MAILBOX_OBJ,
#else /* } MAILBOX_TIMEOUTS */
   q8q_zzz5,
#endif /* } MAILBOX_TIMEOUTS */

   TIMER_OBJ
} OBJTYPE;

#ifdef HAS_RESOURCES /* { */
#ifdef PRIORITY_INVERSION /* { */
typedef enum
{
   PRIORITY_INVERSION_OFF = 0,
   PRIORITY_INVERSION_ON
} RESATTR;
#endif /* } PRIORITY_INVERSION */
#endif /* } HAS_RESOURCES */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _TYPEDEF_H */

/* End of file - typedef.h */
