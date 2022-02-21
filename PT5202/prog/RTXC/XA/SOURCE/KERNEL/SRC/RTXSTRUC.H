/* rtxstruc.h - RTXC internal table data structures */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _RTXSTRUC_H
#define _RTXSTRUC_H

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#include "rtxcopts.h"

/* RTXC stack frame on interrupt */

typedef struct frame
{
  /* store the Code, Extra and Data Segment registers and the
     Segment SELect registers */
  unsigned char es;
  unsigned char cs;
  unsigned char ssel;
  unsigned char ds;
  /* rename R0 to be the frame pointer argument passed to the RTXC kernel. */
  KSNUM ks_stk *pksnum;
  /* store the rest of the global registers */
#if _MODEL != 'l'  /* { */
  unsigned int R1;
#else
#ifdef KS_STK_NEAR  /* { */
  unsigned int R1;
#endif /* } KS_STK_NEAR */
#endif /* } _MODEL */
  unsigned int R2;
  unsigned int R3;
  unsigned int R4;
  unsigned int R5;
  unsigned int R6;
  /* storage place for interrupt processing procedures */
  unsigned int psw;
  void (* pc)(void);       /* initial - entry point address */
} FRAME;

/* RTXC task control block */
typedef struct tcb
{
   struct tcb ks_tbl *flink; /* note, flink MUST be first element in tcb */
   struct tcb ks_tbl *blink;
   struct clkblk ks_clk *pclkblk;
   TASK task;
   PRIORITY priority;
   struct frame ks_stk *sp; /* current stack frame pointer */
   TSTATE status;
   void (*pc_t0)(void);     /* initial - entry point address */
   char ks_stk *stackbase;
   size_t stacksize;
#ifdef TIME_SLICE /* { */
   TSLICE tslice;           /* current time slice remaining */
   TSLICE newslice;         /* reset value for time slice, 0 = no time slice */
#endif /* } TIME_SLICE */

#ifdef HAS_INQTASK_ARG /* { */
   void *arg;             /* argument "passed" to (*pc_t0)() on execute */
#endif /* } HAS_INQTASK_ARG */

#ifdef FPU /* { */
   struct fpregs ks_tbl *fpregs; /* optional fpu context */
   int fpumode;          /* dynamic fpu requirement, 1=fpu in use */
#endif /* } FPU */
} TCB;

/* RTXC const task control block */
typedef struct ktcb
{
   void (*pc_t0)(void); /* initial - entry point address */
   char ks_stk *stackbase; /* initial - stack base */
   size_t stacksize;       /* initial - stack size */
   PRIORITY priority;      /* initial - priority */
#ifdef FPU /* { */
   struct fpregs ks_tbl *fpregs; /* optional fpu context */
#endif /* } FPU */
} KTCB;

#ifdef FPU /* { */
/* FPU (or any other extended) context  */

typedef struct fpregs
{
   int errno;                /* RTL errno */
   int randx;                /* random number seed */
} FPREGS;

#endif /* } FPU */

/* RTXC queue header */
typedef struct qheader
{
   char ks_queue *base;         /* volatile version of qkheader.base    */
   size_t width;       /* volatile version of qkheader.width   */
   int depth;          /* volatile version of qkheader.depth   */
   int putndx;         /* internal use, initially = depth - 1  */
   int curndx;         /* internal use, initially = 0          */
#ifdef QUEUE_WAITERS /* { */
   TCB ks_tbl *waiters; /* priority list of waiting tasks       */
   TCB ks_tbl *dummy;   /* reserved - must follow *waiters      */
#endif /* } QUEUE_WAITERS */

#ifdef QUEUE_SEMAS /* { */
   SEMA fsema;         /* queue full semaphore, initially PENDing      */
   SEMA nesema;        /* queue not empty semaphore, initially PENDing */
   SEMA esema;         /* queue empty semaphore, initially DONE        */
   SEMA nfsema;        /* queue not full semaphore, initially DONE     */
#endif /* } QUEUE_SEMAS */

#ifdef CBUG /* { */
               /* RTXCbug use */
   unsigned int count; /* statistics - count */
   int worst;          /* statistics - worst case full level */
#endif /* } CBUG */
} QHEADER;

/* RTXC const queue header */
typedef struct qkheader
{
   char ks_queue *base; /* &queue body */
   size_t width;       /* width in bytes */
   int depth;          /* maximum depth in entries (not bytes) */
} QKHEADER;

/* RTXC memory block format */
struct xmap
{
   struct xmap *link;
};

/* RTXC partition headers */
typedef struct pheader
{
   struct xmap *next;  /* root of free pool list */
   size_t size;        /* no. of bytes in blocks  */
   int count;          /* initial no. blocks in map */

#ifdef PARTITION_WAITERS /* { */
   TCB ks_tbl *waiters;       /* root of linked waiting tasks */
   TCB ks_tbl *dummy;         /* reserved - must follow *waiters */
#endif /* } PARTITION_WAITERS */

#ifdef DYNAMIC_PARTS /* { */
   MAP map;            /* dynamic partition number */
   char *addr;         /* address of block of memory to use for partition */
#endif /* } DYNAMIC_PARTS */

#ifdef CBUG /* { */
                /* RTXCBUG use */
   int cur;            /* statistics - current no. of used blocks */
   int worst;          /* statistics - worst case low mark   */
   unsigned int usage; /* statistics - no. of KS_free() calls to date */
#endif /* } CBUG */
} PHEADER;

/* RTXC const partition headers */
typedef struct pkheader
{
   struct xmap *next; /* root of free pool list  */
   size_t size;       /* no. of bytes in blocks  */
   int count;         /* initial no. blocks in map */
} PKHEADER;

/* RTXC message */
typedef struct rtxcmsg
{
   struct rtxcmsg *link;
   TASK task;         /* sending task no. */
   PRIORITY priority; /* message priority */
   SEMA sema; /* response semaphore */
} RTXCMSG;

/* RTXC mailbox header */
typedef struct mheader
{
   RTXCMSG *link;
#ifdef MAILBOX_WAITERS /* { */
   TCB ks_tbl *waiters; /* priority list of waiting tasks */
   TCB ks_tbl *dummy;   /* reserved - must follow *waiters */
#endif /* } MAILBOX_WAITERS */

#ifdef MAILBOX_SEMAS /* { */
   SEMA nesema; /* mailbox not empty semaphore, initially undefined */
#endif /* } MAILBOX_SEMAS */

#ifdef CBUG /* { */
          /* RTXCbug use */
   unsigned int count; /* statistics - no. of messages sent */
#endif /* } CBUG */
} MHEADER;

/* RTXC clock block */
typedef struct clkblk
{
   struct clkblk ks_clk *flink;
   struct clkblk ks_clk *blink;
   TICKS remain;  /* delta time before timer expiration */
   TICKS recycle; /* cyclic amount if non-zero */
   TASK task;     /* associated task no. */
   char state;    /* TIMER_DONE or TIMER_ACTIVE */
   OBJTYPE objtype; /* object type */
   KOBJNUM objid;     /* object id */
} CLKBLK;

/* RTXC resource header */
typedef struct rheader
{
   TCB ks_tbl *owner;  /* current task ownership, 0=no owner */
   unsigned char level;     /* no. of nested locks */

#ifdef RESOURCE_WAITERS /* { */
   TCB ks_tbl *waiters; /* priority list of waiters, 0=no waiters */
   TCB ks_tbl *dummy;   /* reserved - must follow *waiters */
#ifdef PRIORITY_INVERSION /* { */
   PRIORITY priority; /* original owner's priority iff inversion */
   RESATTR resattr;   /* priority inversion option switch */
#endif /* } PRIORITY_INVERSION */
#endif /* } RESOURCE_WAITERS */
#ifdef CBUG /* { */
          /* RTXCbug use */
   unsigned int count;     /* statistics - no. of locks performed */
   unsigned int conflict;  /* statistics - no. resource conflicts */
#endif /* } CBUG */
} RHEADER;

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _RTXSTRUC_H */

/* end of rtxstruc.h */
