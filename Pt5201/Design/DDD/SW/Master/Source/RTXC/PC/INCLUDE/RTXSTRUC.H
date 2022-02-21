/* rtxstruc.h - RTXC internal table data structures */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _RTXSTRUC_H
#define _RTXSTRUC_H

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#include "rtxcopts.h"

typedef short KSNUM;

/* RTXC stack frame on interrupt */

typedef struct frame
{

#ifdef CPU386
   unsigned long edi; /* end of 80286/386/486 pushad */
   unsigned long esi;
   unsigned long ebp;
   unsigned long esp; /* bogus value, not used */
   unsigned long ebx;
   unsigned long edx;
   unsigned long ecx;
   KSNUM ks_stk *pksnum; /* start of 80286/386/486 pushad */
#ifdef KS_STK_NEAR /* { */
   unsigned short pad;
#endif /* } KS_STK_NEAR */
#else
   unsigned short di; /* end of 80286/386/486 pusha */
   unsigned short si;
   unsigned short bp;
   unsigned short sp; /* bogus value, not used */
   unsigned short bx;
   unsigned short dx;
#ifdef KS_STK_NEAR /* { */
   unsigned short cx;
#endif /* } KS_STK_NEAR */
   KSNUM ks_stk *pksnum; /* start of 80286/386/486 pusha */
#endif

   unsigned short es;
   unsigned short ds;
   void (far *pc)(void); /* Program Counter at time of KS or interrupt */
   unsigned short ccr;   /* Condition Code Register */

} FRAME;

/* RTXC task control block */
typedef struct tcb
{
   struct tcb near *flink; /* note, flink MUST be first element in tcb */
   struct tcb near *blink;
   struct clkblk ks_clk *pclkblk;
   TASK task;
   PRIORITY priority;
   struct frame *sp;      /* current stack frame pointer */
   TSTATE status;
   void (far *pc_t0)(void);    /* initial - entry point address */
   char ks_stk *stackbase;
   size_t stacksize;
#ifdef TIME_SLICE /* { */
   TICKS tslice;          /* current time slice remaining */
   TICKS newslice;        /* reset value for time slice, 0 = no time slice */
#endif /* } TIME_SLICE */

#ifdef HAS_INQTASK_ARG /* { */
   void *arg;             /* argument "passed" to (*pc_t0)() on execute */
#endif /* } HAS_INQTASK_ARG */

#ifdef FPU /* { */
   struct fpregs near *fpregs; /* optional fpu context */
   int fpumode;          /* dynamic fpu requirement, 1=fpu in use */
#endif /* } FPU */
} TCB;

/* RTXC const task control block */
typedef struct ktcb
{
   void (far *pc_t0)(void); /* initial - entry point address */
   char ks_stk *stackbase; /* initial - stack base */
   size_t stacksize;       /* initial - stack size */
   PRIORITY priority;      /* initial - priority */
#ifdef FPU /* { */
   struct fpregs near *fpregs; /* optional fpu context */
#endif /* } FPU */
} KTCB;

#ifdef FPU /* { */
/* FPU (or any other extended) context  */

typedef struct fpregs
{
       /* 80x87 hardware context follows */
   unsigned short control; /* control register */
   unsigned short status;  /* status register */
   unsigned short tag;     /* tag register */
   void (far *ip)(void); /* instruction pointer */
   void far *op;         /* operand pointer */
   char st0[10];         /* fpu data reg 0 */
   char st1[10];         /* fpu data reg 1 */
   char st2[10];         /* fpu data reg 2 */
   char st3[10];         /* fpu data reg 3 */
   char st4[10];         /* fpu data reg 4 */
   char st5[10];         /* fpu data reg 5 */
   char st6[10];         /* fpu data reg 6 */
   char st7[10];         /* fpu data reg 7 */
} FPREGS;

#endif /* } FPU */

/* RTXC queue header */
typedef struct qheader
{
   char *base;         /* volatile version of qkheader.base    */
   size_t width;       /* volatile version of qkheader.width   */
   int depth;          /* volatile version of qkheader.depth   */
   int putndx;         /* internal use, initially = depth - 1  */
   int curndx;         /* internal use, initially = 0          */
#ifdef QUEUE_WAITERS /* { */
   TCB near *waiters;  /* priority list of waiting tasks       */
   TCB near *dummy;    /* reserved - must follow *waiters      */
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
   char *base;         /* &queue body */
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
   TCB near *waiters;  /* root of linked waiting tasks */
   TCB near *dummy;    /* reserved - must follow *waiters */
#endif /* } PARTITION_WAITERS */

#ifdef DYNAMIC_PARTS /* { */
   MAP map;            /* dynamic partition number */
   char *addr;         /* address of block of memory to use for partition */
#endif /* } DYNAMIC_PARTS */

#ifdef CBUG /* { */
               /* RTXCbug use */
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
   TCB near *waiters; /* priority list of waiting tasks */
   TCB near *dummy;   /* reserved - must follow *waiters */
#endif /* } MAILBOX_WAITERS */

#ifdef MAILBOX_SEMAS /* { */
   SEMA nesema; /* mailbox not empty semaphore, initially undefined */
#endif /* } MAILBOX_SEMAS */

#ifdef CBUG /* { */
          /* RTXCbug use */
   unsigned int count; /* statistics - no. of messages sent */
#endif /* } CBUG */
} MHEADER;

#if 0
#if(__BORLANDC__ == 0x400)
#pragma option -a2
#elif(__BORLANDC__ == 0x500)
#pragma option -a2
#else
#pragma option -a
#endif
#endif
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
   OBJNUM objid;    /* object id */
} CLKBLK;

#if 0
#if(__BORLANDC__ == 0x400)
#pragma option -a.
#elseif(__BORLANDC__ == 0x500)
#pragma option -a2
#else
#pragma option -a
#endif
#endif

/* RTXC resource header */
typedef struct rheader
{
   TCB near *owner;   /* current task ownership, 0=no owner */
   int level;         /* no. of nested locks */

#ifdef RESOURCE_WAITERS /* { */
   TCB near *waiters; /* priority list of waiters, 0=no waiters */
   TCB near *dummy;   /* reserved - must follow *waiters */
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

/* End of file - rtxstruc.h */
