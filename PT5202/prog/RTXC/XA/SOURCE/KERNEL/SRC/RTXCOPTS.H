/* rtxcopts.h - RTXC compile time options */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _RTXCOPTS_H
#define _RTXCOPTS_H

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/************************************************************************

    define either RTXC_EL, RTXC_AL, or RTXC_BL below

***********************************************************************/
#define RTXC_EL /* indicates RTXC extended library support */
#undef RTXC_AL /* indicates RTXC advanced library support */
#undef RTXC_BL /* indicates RTXC basic library support */

/************************************************************************
    RTXCBUG support includes RTXCbug support in kernel and system tables
***********************************************************************/
#define CBUG /* select #define or #undef */

/************************************************************************

Extended context support, e.g., floating point, stdin/out/stderr, memory
management, graphics context or bank switching, includes support for
switching extended contexts within kernel and system tables.

NOTE: The Tasking CXA compiler has a fully reentrant floating point library.
      Therefore, this switch should not be defined.

***********************************************************************/
#undef FPU                      /* select #define if extended context */

/************************************************************************
    kernel stack

Define HAS_KSTACK for those processor/bindings which support a kernel stack.
Default is defined. Also define size of kernel stack.

***********************************************************************/
#define HAS_KSTACK
#define RTXCSTKSZ  256   /* must match size in RTXCOPTS.INC */
			 /* and should be a multiple of 2 */

/************************************************************************
       CORE Internal RAM

This switch is to define 12 bytes of kernel data into the first 1k bytes of
RAM.  Having this switch defined to _near allows the kernel to execute
considerably faster at a minimal cost.  If this data is not desired to be in
the 1k bytes, then #undef CORE_IRAMDATA.

**********************************************************************/
#define CORE_IRAMDATA                         /* alternate - fastest */

/************************************************************************
    kernel stack model (ks_stk) specification

The kernel stack model specification is implemented using a RTXC specific
"ks_stk" keyword.  The "ks_stk" keyword is substituted with either _near, or
NIL by the C preprocessor.  The "ks_stk" keyword is used in rtxc.c in
qualifying the pointer to the RTXC argument packet passed to rtxc(), in
rtxcapi.c in passing the above mentioned pointer, and in rtxcbug.c when
checking stack sizes for overflow.  This allows the kernel in the LARGE
memory model to access the stacks as 16 bit data.

The only downside to the use of a _near stack is that all stacks plus all
other _near data including literal char strings must fit into low 64 kbytes.
This may be a problem with large applications with lots of tasks (each task
stack typically requires 256 - 1024 bytes) and there are lots of _near data
items.

NOTE1: When the kernel assigns a pointer to this data the address is
      truncated and the following warning is issued:
	    "W 517: conversion of long address to short address"
      These warnings should be ignored.

NOTE2: This switch must match the setting of the switch of the same name
       in the RTXCOPTS.INC module.

NOTE3:This switch is supported only in the Tasking LARGE memory model.

***********************************************************************/
#undef KS_STK_NEAR                              /* #define or #undef */

/************************************************************************
    kernel tables (ks_tbl) specification

The kernel tables specification is implemented using a RTXC specific "ks_tbl"
keyword.  The "ks_tbl" keyword is substituted with either _near, or NIL by
the C preprocessor.  The "ks_tbl" keyword is used in rtxc.c in qualifying the
pointer to the RTXC kernel tabular data.  This allows the kernel in the LARGE
memory model to access the tabular data through 16 bit pointers as 16 bit
data.

The only downside to the use of _near tables is that all tables plus all
other _near data including literal char strings must fit into low 64 kbytes.
This may be a problem with large applications with lots of tasks (each task
stack typically requires 256 - 1024 bytes) and there are lots of _near data
items.

NOTE1:When the kernel assigns a pointer to this data the address is
      truncated and the following warning is issued:
	    "W 517: conversion of long address to short address"
      These warnings should be ignored.

NOTE2:This switch is supported only in the Tasking LARGE memory model.

***********************************************************************/
#undef KS_TBL_NEAR                              /* #define or #undef */

/************************************************************************
    kernel data (ks_data) specification

The kernel data specification is implemented using a RTXC specific "ks_data"
keyword.  The "ks_data" keyword is substituted with either _near, or NIL by
the C preprocessor.  The "ks_data" keyword is used in rtxc.c in qualifying
the pointer to frequently accessed RTXC kernel data.  This allows the kernel
in the LARGE memory model to access this data through 16 bit pointers as 16
bit data.

The only downside to the use of _near frequently accessed RTXC kernel data is
that all this data plus all other _near data including literal char strings
must fit into low 64 kbytes.  This may be a problem with large applications
with lots of tasks (each task stack typically requires 256 - 1024 bytes) and
there are lots of _near data items.

NOTE: When the kernel assigns a pointer to this data the address is
      truncated and the following warning is issued:
	    "W 517: conversion of long address to short address"
      These warnings should be ignored.

NOTE2:This switch is supported only in the Tasking LARGE memory model.

***********************************************************************/
#undef KS_DATA_NEAR                             /* #define or #undef */

/************************************************************************
    RTXCINTLVL

Defines processor priority levels for kernel.  This interrupt priority level
is the ceiling at which interrupts are disabled while in the kernel.  All
interrupts at or below the ceiling will be disabled for intervals during
kernel processing.  All interrupts above the ceiling are never disabled and
may be proceesed only outside the scope of the kernel.

NOTE: this must match the setting of the switch of the same name in
      RTXCOPTS.INC

    TASK_INT_LEVEL

Defines the task processor priority level.  This is the floor interrupt level
at which the kernel will execute.  All tasks will execute at this interrupts
level.

***********************************************************************/
#define RTXCINTLVL 0x0D          /* defines priority level for kernel */

#define TASK_INT_LEVEL   0                       /* define as desired */

/************************************************************************
	HAS_INLINE_TRAP

This switch is not supported.  Leave this switch undefined.  This switch
provides the means of trapping into the kernel in an inline manner.  At this
time only the C to assembly function call entry is supported.

***********************************************************************/
#undef HAS_INLINE_TRAP  /* select #define or #undef */

#ifdef HAS_INLINE_TRAP
#define KRNL_TRAP_NUM  0   /* trap # to get into the kernel */
#endif /* } HAS_INLINE_TRAP */

/************************************************************************
    INLINE interrupt control

The INLINE_INTS option allows for inline code generation for the ENABLE and
DISABLE macros which control the enabling and disabling of interrupts.
Inline code is typically faster than the alternative (explicit function
calls).  If the compiler supports inline options, it should usually be used.

***********************************************************************/
#define INLINE_INTS /* select #define or #undef */

/************************************************************************
    INLINE_MACRO_EXPANSION option

The INLINE_MACRO_EXPANSION option allows for selecting expansion of various C
macros used in the kernel as inline C code versus function calls.  The inline
code is typically faster than the alternative (explicit function calls).  The
penalty for using macro expansion is that the RTXC kernel object code is
slightly larger.  The size should only be a concern in small microcontroller
or extremely ROM-limited environments.

***********************************************************************/
#define INLINE_MACRO_EXPANSION /* select #define or #undef */

/************************************************************************
    explicit initialization (zero) of critical uninitialized data items

The BSS_NOT_ZERO option allows for explicit initialization (zeroing) of
uninitialized data areas.  In particular, critical tables and variables used
by RTXC will be cleared as part of the rtxcinit() procedures.  The net effect
is slightly slower and larger initialiation code.  This option is #defined
when using ROM based systems where the RAM contents is unknown or when RTXC
applications are required to be hot-restartable.

***********************************************************************/
#define BSS_NOT_ZERO /* #define of #undef this variable */

/****************************************************************************
                    Queue copy option - QUEUE_MEMCPY

The QUEUE_MEMCPY compile time variable determines (defined) whether a
memcpy() operation will be performed for all queue data movement operations
or (undefined) whether all 1, 2, and 4 byte wide queue data movement
operations will be performed using char, short and long data type assignments
and memcpy() will be performed for all other queue width sizes.  Defining the
QUEUE_MEMCPY switch will produce slightly slower code for 1, 2 and 4 byte
wide queues, but the code will be significantly smaller.  This switch is most
valuable in microcontrollers where code size is critical and in RISC
processors where maximum inline code is preferred.  Note, some compilers even
generate inline code for the memcpy call.

****************************************************************************/
#undef QUEUE_MEMCPY /* choose #define or #undef */

/************************************************************************
                  TIME_SLICE specification

The use of TIME_SLICEing provides for the automatic task switching by RTXC of
tasks that are running at the same task priority.  RTXC will automatically
switch to the "next" runnable task at the same priority when a task's time
slice has expired.  On each clock tick interrupt, the current task's
remaining time slice value is decremented.  If the value is exhausted (== 0),
then if the next ready task is at the same priority, then the current running
task is "moved to the end" of the Ready task list with respect to other tasks
at the its priority.  The extra space overhead to support time slicing is an
extra two members (TICK sized) in each Task Control Block.  The extra time
overhead to support time slicing is minimal.

***********************************************************************/
#define TIME_SLICE /* select #define or #undef */

/************************************************************************

                  DYNAMIC_TASKS specification

The specification of DYNAMIC_TASKS adds support for the dynamic "creation" of
tasks at runtime.  Under RTXC, the taSk creation operation is a two step
operation.  First, RTXC assigns a Task ID in response to a KS_alloc_task()
directive.  Next, a KS_deftask() directive must be executed to define the
task's entry point and priority, stack address and size, and a single "void
*" argument.  Like with static tasks, a task is started using KS_execute().

***********************************************************************/
#define DYNAMIC_TASKS /* select #define or #undef */

/************************************************************************

                  DYNAMIC_PARTS specification

The specification of DYNAMIC_PARTS adds support for the dynamic "creation" of
partitions at runtime.  Under RTXC, the partition creation operation can be
handled in two ways.

1.  A one step operation, KS_create_part() allocates a partition header and
defines a partition according to parameters supplied by the caller.  The
caller must supply a pointer to a block of memory to be used for the
partition, the size of each block in the partition, and the number of blocks
in the partition.  KS_create_part() will return a MAP number that must be
used when using KS_free_part to free the allocated partition header.  NOTE:
KS_create_part() is not available in the Basic Library configuration.

2.  A two step operation involves calling KS_alloc_part() to allocate a
partition header.  KS_alloc_part() returns a MAP number which is used along
with the memory block pointer, size of blocks and number of blocks to call
KS_defpart().

***********************************************************************/
#define DYNAMIC_PARTS /* select #define or #undef */

/************************************************************************
                  synchronized task start up specification

In the standard distribution, the main() function, which serves as the NULL
task, calls KS_execute() for each task that is in the start list.  Each task,
when it is executed, will run until it blocks for some reason and then main()
will start the next task.  The use of the SYNC_START option allows all tasks
that are defined in the start list to be initialized but not actually
started.  If the calling task is the NULL task and SYNC_START is defined,
KS_execute() will set the task status to BLOCKED_WAIT instead of READY.  In
this mode, after all tasks are executed, main() will use KS_unblock() to
unblock all tasks.  Multitasking will then begin.

***********************************************************************/
#undef SYNC_START /* select #define or #undef */

/************************************************************************
	ks_nosaveregs

This is for future use.  Leave this switch set to nothing.  This switch when
applied to the rtxc(), interrupt handlers, and task functions will prevent
the compiler from pushing and popping unnecessary registers upon entry and
exit, respectively.  As a result, the code executes quicker and more
efficeintly.  Set this variable to "c_task" when wanting the optimization.

***********************************************************************/
#define ks_nosaveregs                     /* leave defined as nothing */

/**********************************************************************/

/************************************************************************
	ks_queue

This switch is to define to the data type of the bodies for your applications
RTXC queues.  If the queue bodies are located as far then define "ks_queue"
as far.  Else #undef it.

***********************************************************************/
#define ks_queue                                /* default is nothing */


/***********************************************************************

The SYSMODE switch should always be defined and equal to 0x080.  This defines
the system mode.  User mode is not supported.

**********************************************************************/
#define SYSMODE 0x80

/***********************************************************************
		  end of user defined switches
***********************************************************************/

#ifdef CORE_IRAMDATA
#define core_iramdata _near
#else
#define core_iramdata
#endif /* } CORE_IRAMDATA */

#ifdef KS_STK_NEAR
#define ks_stk _near  /* stacks _near */
#else
#define ks_stk        /* stacks default (_far) */
#endif /* } KS_STK_NEAR */

#ifdef KS_TBL_NEAR
#define ks_tbl _near  /* kernel tables _near */
#else
#define ks_tbl        /* kernel tables default (_far)  */
#endif /* } KS_TBL_NEAR */

#ifdef KS_DATA_NEAR
#define ks_data _near /* kernel data _near */
#else
#define ks_data       /* kernel data default (_far)  */
#endif /* } KS_DATA_NEAR */

/***********************************************************************
  NOT YET SUPPORTED!!!!

       These kernel defined keywords are used to show simularity among
its data type and the data type of its pointers.  The linker wants them
externally declared as they are declared (default).  However, the kernel
wants them as _near.  All pointers are defined to _near objects.
Therefore, you get warnings of pointer trunctations from long to short.

We could not guarantee that the kernel fooled data (ks_stk, ks_clk, ks_tbl
and ks_data) would get loaded in the 1st 64K of data.  The linker/locator
would fill page zero data space and tranfser the rest to page one data
space.  By swapping the attributes of the data(Y8) and xshort(Y3) cluster,
we were able to load the kernel _near data out of the first 1K of page zero
and into the 1st 64k of page zero.  However, we could not guarantee that
the true _near data would get loaded in front of the kernel fooled _near
data or for that matter any other data.

NOTE: At this time the following kernel keywords are defined as nothing.

***********************************************************************/
#define ks_stkx   /* stacks            "" (default) or "near" (Huge=far) */
#define ks_clkx   /* kernel clock data "" (default) or "near" (Huge=far) */
#define ks_tblx   /* kernel tables     "" (default) or "near" (Huge=far) */
#define ks_datax  /* kernel data       "" (default) or "near" (Huge=far) */

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

/* end of rtxcopts.h */
