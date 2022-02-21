/* rtxcbug.c */

/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <stdarg.h>
#include <string.h>

#include "rtxcapi.h"
#include "tstate.h"

#include "rtxcbug.h"

#include "cqueue.h"
#include "cres.h"
#include "csema.h"
#include "ctask.h"
#include "sysopts.h"

/* CONFIG */
#define STKLOGIC      /* indicates task stack limit support */

#define SCREENSIZE 25 /* 25 for PC, else usually 24 for most terminals */

#define NULLTCB  ((TCB near *)0)
#define NULLCLK  ((CLKBLK ks_clk *)0)
#ifndef NULL
#define NULL     ((void *)0)
#endif

#define NAMMAX   8 /* max # chars in task/queue/etc names */

#define SELFTASK ((TASK)0)

#define YES      1
#define NO       0

#define ESC     27
#define BS       8
#define DEL    127

#ifdef STKLOGIC /* { */
#ifdef CBUG /* { */
static void cbugstack(void);
#endif /* } CBUG */
extern char near rtxctos[];
#endif /* } STKLOGIC */

extern SSTATE near semat[];
extern const char semaname[][NAMMAX+1];
extern const SEMA near nsemas;
extern SEMA near siglist[];

extern TCB rtxtcb[];
extern const char taskname[][NAMMAX+1];
extern const TASK near ntasks;
#ifdef DYNAMIC_TASKS /* { */
extern const TASK near dntasks;
#endif /* } DYNAMIC_TASKS */

#ifdef HAS_MAILBOXES /* { */
extern MHEADER near mheader[];
extern const char mboxname[][NAMMAX+1];
extern const MBOX near nmboxes;
#endif /* } HAS_MAILBOXES */

#ifdef HAS_PARTITIONS /* { */
extern PHEADER near pheader[];
extern const char partname[][NAMMAX+1];
extern const MAP near nparts;
#ifdef DYNAMIC_PARTS /* { */
extern const MAP near dnparts;
#endif /* } DYNAMIC_PARTS */
#endif /* } HAS_PARTITIONS */

#ifdef HAS_QUEUES /* { */
extern QHEADER near qheader[];
extern const char queuename[][NAMMAX+1];
extern const QUEUE near nqueues;
#endif /* } HAS_QUEUES */

#ifdef HAS_RESOURCES /* { */
extern RHEADER near rheader[];
extern const char resname[][NAMMAX+1];
extern const RESOURCE near nres;
#endif /* } HAS_RESOURCES */

extern CLKBLK ks_clk * near clkqptr;
extern TICKS near rtctick;
extern time_t near rtctime;
extern const int near ntmrs;
extern const int near clktick;

extern char near isrmax;

#ifdef CBUG /* { */
extern char near brkflag;


static void cbugmain(void);
static void cbugclock(void);
#ifdef HAS_MAILBOXES
static void cbugmbox(void);
#endif
#ifdef HAS_PARTITIONS
static void cbugpart(void);
#endif
#ifdef HAS_QUEUES
static void cbugqueue(void);
#endif
#ifdef HAS_RESOURCES
static void cbugres(void);
#endif
static void cbugsema(void);
static void cbugtask(void);
static void cbugzero(void);
static void cbugregs(void);


static char *xtaskname(TASK task);
#ifdef HAS_PARTITIONS
static char *xpartname(MAP map);
#endif
static void showtime(OBJTYPE objtype, TASK task, OBJNUM objid);

static void taskmgr(void);

static TASK gettask(void);
static PRIORITY getpriority(void);
#ifdef TIME_SLICE
static TICKS getslice(void);
#endif

static void inputs(char *); /* get '\r' terminated line from CBUGIQ */

static void outputs(const char *); /* := outputs(buff) */

static void outputf(const char *, ...); /* := formatv + output */
extern int formatv(char *, char *, va_list);

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

extern int atoi(const char *);
extern int strcmp(const char *, const char *);

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

extern char near cbugflag; /* clock ISR ignore flag, initially = 0 */

static TICKS et;

static void cbugrtxc(void);                    /* RTXC object menu function */
static void menumain(void);                    /* main menu                 */
static void menurtxc(void);   /* rtxc menu for "H" help option in rtxc menu */
int goflag;           /* scope is range of local menues, used for GO option */
#endif /* } CBUG */

void sgnlcbug(void);
void rtxcbug(void);

#ifndef CBUG /* { no RTXCbug wanted, but leave RTXCbug task defined */
void rtxcbug(void)
{
   KS_terminate(SELFTASK);
}
#else /* } CBUG { */

void rtxcbug(void)
{
#ifdef BSS_NOT_ZERO
//   cbugflag = 0; /* reset */
#endif

#ifdef AUTOCBUG
      /* allow for auto entry to CBUG before any tasks start */
   KS_signal(CBUGSEMA);
#endif

   for (;;)
   {
      KS_elapse(&et); /* initialize time stamp */

      KS_wait(CBUGSEMA); /* wait for Cbug event */

      et = KS_elapse(&et); /* capture time since last event */

#if CBUGRES
      KS_lockw(CBUGRES); /* lock and wait for availability */
      KS_unlock(CBUGRES); /* else deadlock since next will block world */
#endif

      cbugflag = 1; /* set bug active, in case not already set */

       /* block everybody except self */
#ifdef DYNAMIC_TASKS
      KS_block(1, ntasks + dntasks);
#else
      KS_block(1, ntasks);
#endif

      /* unblock i/o drivers */
      KS_unblock(CBUGIDRV, CBUGIDRV);
      KS_unblock(CBUGODRV, CBUGODRV);

      cbugmain();

      cbugflag = 0; /* disable bug on exit */
   }
}

static void cbugmain(void)
{
   char string[40];

   outputs("\n** RTXCbug - ");
   outputs(KS_version());

   for (;;)
   {
      menumain();

      outputs("\nRTXCbug> ");

      inputs(string);

      switch(string[0])
      {
         case 'K':
         case 'k':
            goflag = 0;
            cbugrtxc();
            if(goflag)
              return;
            break;

         case 'X':
         case 'x':
         case 'G':
         case 'g':
            outputs("\nReturning from RTXCBug\n");

         /* release all task blocks */
#ifdef DYNAMIC_TASKS
         KS_unblock(1, ntasks + dntasks);
#else
         KS_unblock(1, ntasks);
#endif
         return;

         default:
            outputs("\nBad command\n"); /* clean console line */
            break;
      }
   }
}

static void cbugrtxc(void)
{
   char string[40];

   for (;;)
   {
      outputs("\nRTXCbug - RTXC Objects> ");

      inputs(string);

      switch(string[0])
      {
         case 'T':
         case 't':
            cbugtask();
            break;

#ifdef HAS_QUEUES
         case 'Q':
         case 'q':
            cbugqueue();
            break;
#endif

         case 'S':
         case 's':
            cbugsema();
            break;

#ifdef HAS_RESOURCES
         case 'R':
         case 'r':
            cbugres();
            break;
#endif

#ifdef HAS_PARTITIONS
         case 'P':
         case 'p':
            cbugpart();
            break;
#endif

#ifdef HAS_MAILBOXES
         case 'M':
         case 'm':
            cbugmbox();
            break;
#endif

         case 'C':
         case 'c':
            cbugclock();
            break;

         case '$':
            taskmgr();
            break;

         case '!':
           /* block everybody except self */
#ifdef DYNAMIC_TASKS
           KS_block(1, ntasks + dntasks);
#else
           KS_block(1, ntasks);
#endif

           /* unblock i/o drivers */
           KS_unblock(CBUGIDRV, CBUGIDRV);
           KS_unblock(CBUGODRV, CBUGODRV);

           break;

#ifdef STKLOGIC
         case 'K':
         case 'k':
            cbugstack();
            break;
#endif

         case 'Z': /* reset statistics */
         case 'z':
            cbugzero();
            break;

         case '#': /* display task registers */
            cbugregs();
            break;

         case 'H':
         case 'h':
            menurtxc();
            break;

         case 'X':
         case 'x':
         case 'G':
         case 'g':
            outputs("\nReturning from RTXCBug\n");

            /* release all task blocks */
#ifdef DYNAMIC_TASKS
            KS_unblock(1, ntasks + dntasks);
#else
            KS_unblock(1, ntasks);
#endif
            goflag = 1;
            return;

         case 'U':
         case 'u':
             return;

         default:
            outputs("\nBad command\n"); /* clean console line */
            break;
      }
   }
}

static void cbugtask(void)
{
   int l;
   OBJNUM i, j;
   int priority;
   char string[16];
#if defined(MAILBOX_SEMAS) || defined(QUEUE_SEMAS)
   OBJNUM k;
#endif
#if defined(PARTITION_WAITERS) || \
    defined(MAILBOX_WAITERS)   || \
    defined(QUEUE_WAITERS)     || \
    defined(RESOURCE_WAITERS)
   TCB near *nexttask;
#endif

   outputs("\n** Task Snapshot **\n");
#ifdef FPU
   outputs(  "  #   Name  Priority  FPU State \n");
#else
   outputs(  "  #   Name  Priority  State \n");
#endif
          /* 123 12345678  123     12345678 */
#ifdef DYNAMIC_TASKS
   for (i = 1, l = 1; i <= ntasks + dntasks; i++, l++)
#else
   for (i = 1, l = 1; i <= ntasks; i++, l++)
#endif
   {
      if (l > SCREENSIZE - 4) /* "more" logic */
      {
         l = 1;
         outputs("-- More --");
         inputs(string);
         if (string[0] == 'X' || string[0] == 'x')
            break;
      }

      priority = rtxtcb[i].priority;
      outputf("(I3,X1,S8,X2,UI3,X5,N)", &i, xtaskname(i), &priority);

#ifdef FPU
      if (rtxtcb[i].fpumode)
         outputs(" Y  ");
      else
         outputs(" N  ");
#endif

      if (rtxtcb[i].status & SUSPFLG)
         outputs("SUSPENDED ");

      if (rtxtcb[i].status & INACTIVE)
         outputs("INACTIVE ");

      if (rtxtcb[i].status & SEMAPHORE_WAIT)
      {
         outputs("Semaphore ");  /* now find associated sema(s) */
         for (j = 1; j <= nsemas; j++)
         {
            if ((semat[j] == i))
            {
               outputf("(X1,S8,N)", &semaname[j][0]);
               showtime(TIMER_OBJ, i, j);
#ifdef SEMAPHORE_TIMEOUTS
               showtime(SEMAPHORE_OBJ, i, j);
#endif

#ifdef QUEUE_SEMAS
               for (k = 1; k <= nqueues; k++)
               {
                  if (j == qheader[k].esema)
                     outputf("(H'<QE> ',S8,N)", &queuename[k][0]);

                  if (j == qheader[k].fsema)
                     outputf("(H'<QF> ',S8,N)", &queuename[k][0]);

                  if (j == qheader[k].nesema)
                     outputf("(H'<QNE> ',S8,N)", &queuename[k][0]);

                  if (j == qheader[k].nfsema)
                     outputf("(H'<QNF> ',S8,N)", &queuename[k][0]);
               }
#endif

#ifdef MAILBOX_SEMAS
               for (k = 1; k <= nmboxes; k++)
               {
                  if (j == mheader[k].nesema)
                     outputf("(H'<MBXNE> ',S8,N)", &mboxname[k][0]);
               }
#endif
            }
         }
      }

#ifdef QUEUE_WAITERS
      if (rtxtcb[i].status & QUEUE_WAIT)
      {
         for (j = 1; j <= nqueues; j++) /* check all queues */
         {
            nexttask = qheader[j].waiters;
            while (nexttask != NULLTCB) /* by walking list of waiters */
            {
               if (nexttask->task == i)
               {
                  if (qheader[j].curndx == 0)
                     outputs("QueueEmpty");
                  else
                     outputs("QueueFull ");
                  outputf("(X1,S8,N)", &queuename[j][0]);
#ifdef QUEUE_TIMEOUTS
                  showtime(QUEUE_OBJ, i, j);
#endif
               }
               nexttask = nexttask->flink;
            }
         }
      }
#endif

#ifdef MAILBOX_WAITERS
      if (rtxtcb[i].status & MSG_WAIT)
      {
         outputs("Mailbox    ");
         for (j = 1; j <= nmboxes; j++)
         {
            nexttask = mheader[j].waiters;
            while (nexttask != NULLTCB)
            {
               if (nexttask->task == i) /* if task i is waiting on mbox j */
               {
                  outputf("(S8,N)", &mboxname[j][0]);
#ifdef MAILBOX_TIMEOUTS
                  showtime(MAILBOX_OBJ, i, j);
#endif
               }
               nexttask = nexttask->flink;
            }
         }
      }
#endif

      if (rtxtcb[i].status & DELAY_WAIT)
      {
         outputs("DELAY              "); /* now find time remaining */
         showtime(DELAY_OBJ, i, i);
      }

#ifdef RESOURCE_WAITERS
      if (rtxtcb[i].status & RESOURCE_WAIT)
      {
         outputs("Resource   ");             /* find associated resource */
         for (j = 1; j <= nres; j++)  /* by checking all resources */
         {
            nexttask = rheader[j].waiters;
            while (nexttask != NULLTCB) /* by walking list of waiters */
            {
               if (nexttask->task == i) /* if task i is waiting on res j */
               {
                  outputf("(S8,N)", &resname[j][0]); /* note resource */
#ifdef RESOURCE_TIMEOUTS
                  showtime(RESOURCE_OBJ, i, j);
#endif
               }
               nexttask = nexttask->flink;
            }
         }
      }
#endif

#ifdef PARTITION_WAITERS
      if (rtxtcb[i].status & PARTITION_WAIT)
      {
         outputs("Partition  ");           /* find associated partition */
         for (j = 1; j <= nparts; j++) /* by checking each partition */
         {
            nexttask = pheader[j].waiters;
            while (nexttask != NULLTCB) /* by walking list of waiters */
            {
               if (nexttask->task == i) /* if task i waiting on partition j */
               {
                  outputf("(S8,N)", xpartname(j)); /* note partition */
#ifdef PARTITION_TIMEOUTS
                  showtime(PARTITION_OBJ, i, j);
#endif
               }
               nexttask = nexttask->flink;
            }
         }
      }
#endif

      if (rtxtcb[i].status == BLOCK_WAIT) /* ready but only blocked by Cbug */
          outputs("-READY ");

      if (rtxtcb[i].status == 0)
         outputs("READY");

#ifdef TIME_SLICE
   if (rtxtcb[i].newslice) /* if time slicing enabled for task */
   {
      outputf("(X1,H'(',I4,H'/',I4,H' ticks)',N)", &rtxtcb[i].tslice,
                                                   &rtxtcb[i].newslice);
   }
#endif

      outputs("\n");
   }
}

#ifdef HAS_QUEUES
static void cbugqueue(void)
{
   int l;
   OBJNUM i;
   char string[16];
#ifdef QUEUE_WAITERS
   TCB near *nexttask;
#endif
#ifdef QUEUE_SEMAS
   SEMA sema;
#endif

   outputs("\n** Queue Snapshot **");
   outputs("\n  #   Name  Current/Depth Worst Count Waiters\n");
         /*  123 12345678  12345/12345 12345 12345 12345678 12345678 */
   for (i = 1, l = 1; i <= nqueues; i++, l++)
   {
      if (l > SCREENSIZE - 4)
      {
         l = 1;
         outputs("-- More --");
         inputs(string);
         if (string[0] == 'X' || string[0] == 'x')
            break;
      }

      outputf("(I3,X1,S8,X2,I5,H'/',2(I5,X1),UI5,N)",
         &i, &queuename[i][0], &qheader[i].curndx,
         &qheader[i].depth, &qheader[i].worst, &qheader[i].count);

#ifdef QUEUE_WAITERS
      nexttask = qheader[i].waiters;
      while (nexttask != NULLTCB) /* by walking list of waiters */
      {
         outputf("(X1,S8,N)", xtaskname(nexttask->task));
         nexttask = nexttask->flink;
      }
#endif

#ifdef QUEUE_SEMAS
      if ( ((sema = qheader[i].esema) != 0) && (semat[sema] > 0) )
         outputf("(X1,S8,H'<E>',N)", &semaname[sema][0]);

      if ( ((sema = qheader[i].fsema) != 0) && (semat[sema] > 0) )
         outputf("(X1,S8,H'<F>',N)", &semaname[sema][0]);

      if ( ((sema = qheader[i].nesema) != 0) && (semat[sema] > 0) )
         outputf("(X1,S8,H'<NE>',N)", &semaname[sema][0]);

      if ( ((sema = qheader[i].nfsema) != 0) && (semat[sema] > 0) )
         outputf("(X1,S8,H'<NF>',N)", &semaname[sema][0]);
#endif

      outputs("\n");
   }
}
#endif

static void cbugsema(void)
{
   int l;
   OBJNUM i;
   TASK task;
   char string[16];
#if defined(QUEUE_SEMAS) || defined(MAILBOX_SEMAS)
   OBJNUM k;
#endif

   outputs("\n** Semaphore Snapshot **");
   outputs("\n  #   Name  State  Waiter\n");
         /*  123 12345678 1234 12345678 */
         /*      xxxxxxxx DONE          */
         /*      xxxxxxxx PEND          */
         /*      xxxxxxxx WAIT xxxxxxxx */

   for (i = 1, l = 1; i <= nsemas; i++, l++)
   {
      if (l > SCREENSIZE - 4)
      {
         l = 1;
         outputs("-- More --");
         inputs(string);
         if (string[0] == 'X' || string[0] == 'x')
            break;
      }
      outputf("(I3,X1,S8,X1,N)", &i, &semaname[i][0]);
      switch((signed int)semat[i])
      {
         case (signed int)SEMA_DONE:
            outputs("DONE\n");
            break;

         case (signed int)SEMA_PENDING:
            outputs("PEND\n");
            break;

         default:
            task = semat[i];
            outputf("(H'WAIT ',S8,N)", xtaskname(task));
#ifdef SEMAPHORE_TIMEOUTS
            showtime(SEMAPHORE_OBJ, task, i);
#endif

#ifdef QUEUE_SEMAS
            for (k = 1; k <= nqueues; k++)
            {
               if (i == qheader[k].esema)
                  outputf("(H' <QE> ',S8,N)", &queuename[k][0]);

               if (i == qheader[k].fsema)
                  outputf("(H' <QF> ',S8,N)", &queuename[k][0]);

               if (i == qheader[k].nesema)
                  outputf("(H' <QNE> ',S8,N)", &queuename[k][0]);

               if (i == qheader[k].nfsema)
                  outputf("(H' <QNF> ',S8,N)", &queuename[k][0]);
            }
#endif

#ifdef MAILBOX_SEMAS
            for (k = 1; k <= nmboxes; k++)
            {
               if (i  == mheader[k].nesema)
                  outputf("(H' <MBXNE> ',S8,N)", &mboxname[k][0]);
            }
#endif
            outputs("\n");
            break;
      }
   }
}

#ifdef HAS_RESOURCES
static void cbugres(void)
{
   int l;
   OBJNUM i;
   char string[16];
#ifdef RESOURCE_WAITERS
   TCB near *nexttask;
#endif

   outputs("\n** Resource Snapshot **");
   outputs("\n  #   Name   Count Conflicts   Owner   Waiters\n");
          /* 123 12345678 12345   12345    12345678 12345678 12345678 */

   for (i = 1, l = 1; i <= nres; i++, l++)
   {
      if (l > SCREENSIZE - 4)
      {
          l = 1;
          outputs("-- More --");
          inputs(string);
          if (string[0] == 'X' || string[0] == 'x')
             break;
      }

#ifdef PRIORITY_INVERSION
      outputf("(I3,X1,S8,X1,UI5,X3,UI5,N)", &i, &resname[i][0],
           &rheader[i].count, &rheader[i].conflict);
      if (rheader[i].resattr == PRIORITY_INVERSION_ON)
         outputs(" ** ");
      else
         outputs("    ");
#else
      outputf("(I3,X1,S8,X1,UI5,X3,UI5,X4,N)", &i, &resname[i][0],
           &rheader[i].count, &rheader[i].conflict);
#endif

      if (rheader[i].owner != NULLTCB)
      {
         outputf("(S8,N)", xtaskname(rheader[i].owner->task));

#ifdef RESOURCE_WAITERS
         nexttask = rheader[i].waiters;
         while (nexttask != NULLTCB)
         {
            outputf("(X1,S8,N)", xtaskname(nexttask->task));
            nexttask = nexttask->flink;
         }
#endif
      }
      outputs("\n");
   }
}
#endif

#ifdef HAS_PARTITIONS
static void cbugpart(void)
{
   int j, l;
   OBJNUM i;
   struct xmap *next;
   char string[16];
#ifdef PARTITION_WAITERS
   TCB near *nexttask;
#endif

   outputs("\n** Partition Snapshot **");
   outputs("\n  #   Name   Avail/Total Worst Count  Bytes Waiters\n");
          /* 123 12345678 12345/12345 12345 12345 123456 12345678 1345678 */

#ifdef DYNAMIC_PARTS
   for (i = 1, l = 1; i <= nparts + dnparts; i++, l++)
#else
   for (i = 1, l = 1; i <= nparts; i++, l++)
#endif
   {
      j = 0;
      next = pheader[i].next;
      while (next)
      {
         next = next->link;
         j++;
      }

#ifdef DYNAMIC_PARTS
      /* if dynamic partitions, free ones will be linked together ... */
      /* therefore, the above counting of links will not give the number */
      /* of blocks in a dynamic partition if it is free since free ones */
      /* have no blocks, so adjust count if necessary */
      if (i > nparts)
         if (pheader[i].size == 0)
            j = 0;
#endif

      if (l > SCREENSIZE - 4)
      {
         l = 1;
         outputs("-- More --");
         inputs(string);
         if (string[0] == 'X' || string[0] == 'x')
            break;
      }
      outputf("(I3,X1,S8,X1,I5,H'/',3(UI5,X1),UI6,N)",
          &i, xpartname(i), &j, &pheader[i].count,
          &pheader[i].worst, &pheader[i].usage, &pheader[i].size);

#ifdef PARTITION_WAITERS
      nexttask = pheader[i].waiters;
      while (nexttask != NULLTCB)
      {
         outputf("(X1,S8,N)", xtaskname(nexttask->task));
         nexttask = nexttask->flink;
      }
#endif

      outputs("\n");
   }
}
#endif

#ifdef HAS_MAILBOXES
static void cbugmbox(void)
{
   int j, l;
   OBJNUM i;
   char string[16];
   MHEADER *pmh;
   RTXCMSG *next;
#ifdef MAILBOX_WAITERS
   TCB near *nexttask;
#endif

   outputs("\n** Mailbox Snapshot **");
   outputs("\n  #   Name   Current Count Waiters\n");
          /* 123 12345678  12345  12345 12345678 12345678 */

   for (i = 1, l = 1, pmh = &mheader[1]; i <= nmboxes;
        i++,   l++,   pmh++)
   {
      j = 0;
      /* count # messages in list */
      next = pmh->link;
      while (next != NULL)
      {
         next = next->link;
         j++;
      }

      if (l > SCREENSIZE - 4)
      {
         l = 1;
         outputs("-- More --");
         inputs(string);
         if (string[0] == 'X' || string[0] == 'x')
            break;
      }

      outputf("(I3,X1,S8,2(X2,UI5),N)",
          &i, &mboxname[i][0], &j, &mheader[i].count);

#ifdef MAILBOX_WAITERS
      nexttask = pmh->waiters;
      while (nexttask != NULLTCB)
      {
         outputf("(X1,S8,N)", xtaskname(nexttask->task));
         nexttask = nexttask->flink;
      }
#endif

      outputs("\n");
   }
}
#endif

static void cbugclock(void)
{
   int l, k;
   CLKBLK ks_clk *next;
   char string[16];
   long time, m;

   outputs("\n** Clock Snapshot **\n\n");
   k = 1000 / clktick;
   outputf("(H'Clock rate is',I5,H' Hz, Tick interval is',I4,H' ms, ',N)",
       &k, &clktick);

   outputf("(H'Maximum of',I3,H' timers\nTick timer is',I9,N)",
       &ntmrs, &rtctick);
   outputf("(H', ET is ',I9,H' ticks, RTC time is ',L9,//N)", &et,&rtctime);

   outputs("   Time        Cyclic   Task   Timer      Object\n");
   outputs(" Remaining      Value   Name   Type        Name\n");
        /* 1234567890 1234567890 12345678 xxxxxxxxxx xxxxxxxx */

   time = 0;
   l = 1;
   next = clkqptr;

   while (next != NULLCLK) /* walk timer list */
   {
      time = next->remain - rtctick;

      time *= clktick;

      if (l > SCREENSIZE - 4)
      {
         l = 1;
         outputs("-- More --");
         inputs(string);
         if (string[0] == 'X' || string[0] == 'x')
            break;
      }
      l++;

      m = (long)(next->recycle * clktick);
      outputf("(2(L10,X1),S8,X1,N)", &time, &m, xtaskname(next->task));

      switch(next->objtype)
      {
        case TIMER_OBJ:
            outputf("(H'Timer      ',S8/N)", &semaname[next->objid][0]);
            break;

         case DELAY_OBJ:
            outputf("(H'Delay      ',S8/N)", xtaskname(next->objid));
            break;

#ifdef SEMAPHORE_TIMEOUTS
         case SEMAPHORE_OBJ:
            outputf("(H'Semaphore  ',S8/N)", &semaname[next->objid][0]);
            break;
#endif

#ifdef MAILBOX_TIMEOUTS
         case MAILBOX_OBJ:
            outputf("(H'Mailbox    ',S8/N)", &mboxname[next->objid][0]);
            break;
#endif

#ifdef PARTITION_TIMEOUTS
         case PARTITION_OBJ:
            outputf("(H'Partition  ',S8/N)", xpartname(next->objid));
            break;
#endif

#ifdef QUEUE_TIMEOUTS
         case QUEUE_OBJ:
            outputs("Queue");
            if (qheader[next->objid].curndx == 0)
               outputs("Empty");
            else
               outputs("Full ");
            outputf("(X1,S8/N)", &queuename[next->objid][0]);
            break;
#endif

#ifdef RESOURCE_TIMEOUTS
         case RESOURCE_OBJ:
            outputf("(H'Resource   ',S8/N)", &resname[next->objid][0]);
            break;
#endif

         default:
            outputs("Unknown timer type\n");
            break;
      }
      next = next->flink;
   }
}

/* zero all statistics */
static void cbugzero(void)
{
   OBJNUM i;

#ifdef HAS_PARTITIONS
#ifdef DYNAMIC_PARTS
   for (i = 1; i <= nparts + dnparts; i++)
#else
   for (i = 1; i <= nparts; i++)
#endif
      pheader[i].usage = pheader[i].worst = 0;
#endif

#ifdef HAS_QUEUES
   for (i = 1; i <= nqueues; i++)
      qheader[i].count = qheader[i].worst = 0;
#endif

#ifdef HAS_RESOURCES
   for (i = 1; i <= nres; i++)
      rheader[i].count = rheader[i].conflict = 0;
#endif

#ifdef HAS_MAILBOXES
   for (i = 1; i <= nmboxes; i++)
      mheader[i].count = 0;
#endif

   isrmax = 0;

   outputs("\n"); /* clear console line */
}

static void cbugregs(void)
{
   TASK task;
   FRAME *frame;
#ifdef LARGE_MEM_MODEL
   void (* pc1)(void);
#endif
#ifdef FPU /* { */
   FPREGS *fpregs;
   double temp;
   long double *st;
   int i;
#endif /* } FPU */

   if ( (task = gettask()) != 0 )
   {
      if (task == RTXCBUG)
      {
         outputs("Register set of current task is meaningless\n");
         return;
      }

      if ( (frame = rtxtcb[task].sp) == NULL)
      {
         outputs("Task Register set is undefined\n");
         return;
      }

      outputs("** Task Register Snapshot **\n");

      outputf("(R16,Z-,H' CS=',UI4,N)",   (unsigned int *)(&frame->pc)+1);
      outputf("(R16,Z-,H' IP=',UI4,N)",   (unsigned int *)(&frame->pc)+0);
      outputf("(R16,Z-,H' SS=',UI4,N)",   (unsigned int *)&frame+1);
#ifndef CPU386
      outputf("(R16,Z-,H' SP=',UI4,N)",   (unsigned int *)&frame+0);
      outputf("(R16,Z-,H' BP=',UI4,N)",   &frame->bp);
#else /* 386 mode - bcc 3.1 only - yet only 16 bits */
      outputf("(R16,Z-,H' SP=',UI4,N)",   (unsigned short *)&frame+0);
      outputf("(R16,Z-,H' BP=',UI4,N)",   &frame->ebp); /* show only short */
#endif
      outputf("(R16,Z-,H' DS=',UI4,N)",   &frame->ds);
      outputf("(R16,Z-,H' ES=',UI4,N)",   &frame->es);
      outputf("(R16,Z-,H' CCR=',UI4,/N)", &frame->ccr);

#ifndef CPU386
      outputf("(R16,Z-,H' AX=',UI4,N)",   &frame->pksnum);
      outputf("(R16,Z-,H' BX=',UI4,N)",   &frame->bx);
#ifdef KS_STK_NEAR /* { */
      outputf("(R16,Z-,H' CX=',UI4,N)",   &frame->cx);
#else
      outputf("(R16,Z-,H' CX=',UI4,N)",   (unsigned short *)&frame->pksnum+1);
#endif /* } KS_STK_NEAR */
      outputf("(R16,Z-,H' DX=',UI4,N)",   &frame->dx);
      outputf("(R16,Z-,H' SI=',UI4,N)",   &frame->si);
      outputf("(R16,Z-,H' DI=',UI4,/N)",  &frame->di);
#else /* 386 mode - bcc 3.1 only */
      outputf("(R16,Z-,H' EAX=',UL8,N)",   &frame->pksnum);
      outputf("(R16,Z-,H' EBX=',UL8,N)",   &frame->ebx);
      outputf("(R16,Z-,H' ECX=',UL8,N)",   &frame->ecx);
      outputf("(R16,Z-,H' EDX=',UL8,N)",   &frame->edx);
      outputf("(R16,Z-,H' ESI=',UL8,N)",   &frame->esi);
      outputf("(R16,Z-,H' EDI=',UL8,/N)",  &frame->edi);
#endif

#ifdef FPU
      if ( (fpregs = rtxtcb[task].fpregs) == NULL)
                        return;

      outputs("\n** Floating Point Register Snapshot **\n");

      outputf("(R16,Z-,H' Control Word = ',UI4,N)",  &fpregs->control);
      outputf("(R16,Z-,H' FIP=',UL8,/N)",            &fpregs->ip);
      outputf("(R16,Z-,H' Status Word  = ',UI4,N)",  &fpregs->status);
      outputf("(R16,Z-,H' FOO=',UL8,/N)",            &fpregs->op);
      outputf("(R16,Z-,H' Tag Word     = ',UI4,/N)", &fpregs->tag);

      st = (long double *)&fpregs->st0;
      for (i = 0; i < 8; i++, st++)
      {
         outputf("(' ST',I1,' - ',N)", &i);
         if(((fpregs->tag >> (2 * i)) & 3) == 3)
            outputs("Empty\n");
         else if(((fpregs->tag >> (2 * i)) & 3) == 2)
            outputs("Special\n");
         else if(((fpregs->tag >> (2 * i)) & 3) == 1)
            outputs("Zero\n");
         else
         {
            temp = *st;
            outputf("(D16.6,/N)", &temp);
         }
      }
#endif   /* } FPU */
   }
   else
      outputs("Undefined Task\n");
}

static TASK gettask(void)
{
   TASK task;
   char string[16]; /* 12345678\0 */

   outputs("\nTask (# or name)> ");
   inputs(string);
   outputs("\n");

   if (string[0] == '-') /* if first char is '-' */
      return(-1); /* assume -1 = ALL for block/unblock */

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      task = atoi(string);

#ifdef DYNAMIC_TASKS
      if ( (task < 1) || (task > ntasks + dntasks) )
#else
      if ( (task < 1) || (task > ntasks) )
#endif
         task = 0;
      return(task);
   }
   else
   {
#ifdef DYNAMIC_TASKS
      for (task = 1; task <= ntasks + dntasks; task++)
#else
      for (task = 1; task <= ntasks; task++)
#endif
      {
         if (!strcmp(string, xtaskname(task)))
            return(task);
      }
      return(0); /* not found */
   }
}

static PRIORITY getpriority(void)
{
   PRIORITY priority;
   char string[16]; /* 123\0 */

   outputs("Priority> ");
   inputs(string);
   outputs("\n");

   priority = atoi(string);

   return(priority);
}

#ifdef TIME_SLICE
static TICKS getslice(void)
{
   TICKS tslice;
   char string[16]; /* 123\0 */

   outputs("Time slice (in ticks)> ");
   inputs(string);
   outputs("\n");

   tslice = atoi(string);

   return(tslice);
}
#endif

/* extended mode debug functions start here */
static void taskmgr(void)
{
   char string[16];
   TASK task;
   PRIORITY priority;

   outputs("\n");
   for (;;)
   {
      outputs("\n$RTXCbug> ");
      inputs(string);
      switch(string[0])
      {
         case 'S': /* suspend */
         case 's':
            task = gettask();
            if ( (task != SELFTASK) && (task != RTXCBUG) )
               KS_suspend(task);
            break;

         case 'R': /* resume */
         case 'r':
            task = gettask();
            if (task != 0)
               KS_resume(task);
            break;

         case 'T': /* terminate */
         case 't':
            task = gettask();
            if (task != 0)
               KS_terminate(task);
            break;

         case 'C': /* change priority */
         case 'c':
            task = gettask();
#ifdef DYNAMIC_TASKS
            if ( (task != 0) && (task <= ntasks + dntasks) )
#else
            if ( (task != 0) && (task <= ntasks) )
#endif
            {
               priority = getpriority();
               KS_defpriority(task, priority);
            }
            break;

         case 'E': /* execute */
         case 'e':
            task = gettask();
            /* note - no defense against executing CBUG, etc */
            if (task != SELFTASK)
               KS_execute(task);
            break;

         case 'B': /* block */
         case 'b':
            task = gettask();
            if (task == -1) /* special case - everybody blocked! */
            {
               cbugflag = 1;           /* in case clock was running */

               /* block all except CBUG */
#ifdef DYNAMIC_TASKS
               KS_block(1, ntasks + dntasks);
#else
               KS_block(1, ntasks);
#endif

               /* unblock i/o drivers */
               KS_unblock(CBUGIDRV, CBUGIDRV);
               KS_unblock(CBUGODRV, CBUGODRV);

               break;
            }
#ifdef DYNAMIC_TASKS
            if ( (task != 0) && (task <= ntasks + dntasks) )
#else
            if ( (task != 0) && (task <= ntasks) )
#endif
               KS_block(task, task);
            break;

         case 'U': /* unblock */
         case 'u':
            task = gettask();
            if (task == -1) /* special case - everybody unblocked */
            {
                   /* unblock all - ! CAUTION ! */
#ifdef DYNAMIC_TASKS
               KS_unblock(1, ntasks + dntasks);
#else
               KS_unblock(1, ntasks);
#endif
               cbugflag = 0;               /* more caution */
               break;
            }
#ifdef DYNAMIC_TASKS
            if ( (task != 0) && (task <= ntasks + dntasks) )
#else
            if ( (task != 0) && (task <= ntasks) )
#endif
               KS_unblock(task, task);
            break;

#ifdef TIME_SLICE
         case '/': /* time slice (divide time) */
            task = gettask();
#ifdef DYNAMIC_TASKS
            if ( (task != 0) && (task <= ntasks + dntasks) )
#else
            if ( (task != 0) && (task <= ntasks) )
#endif
            {
               KS_defslice(task, getslice());
            }
            break;
#endif

         case 'H': /* Help */
         case 'h':
            outputs("\n");
            outputs("S - Suspend\n");
            outputs("R - Resume\n");
            outputs("T - Terminate\n");
            outputs("E - Execute\n");
            outputs("C - Change task priority\n");
            outputs("B - Block (-1=All)\n");
            outputs("U - Unblock (-1=ALL)\n");
#ifdef TIME_SLICE
            outputs("/ - Time slice\n");
#endif
            outputs("H - Help\n");
            outputs("X - Exit Task Manager Mode\n");
            break;

         case 'X': /* exit */
         case 'x':
            outputs("\nRe-entering RTXCbug mode\n");
            return;

         default:
            outputs("\nBad command\n");
            break;
      }
   }
}

#ifdef STKLOGIC
static void cbugstack(void)
{
   int i, l;
   size_t max, max1, j, used;
   char string[16];
   int  *p;
   SEMA near *psema;

   outputs("\n** Stack Snapshot **\n");
   outputs(  "  #   Task    Size  Used  Spare\n");
       /*    123 12345678  1234  1234  1234*/

#ifdef DYNAMIC_TASKS
   for (i = 1, l = 1; i <= ntasks + dntasks; i++, l++)
#else
   for (i = 1, l = 1; i <= ntasks; i++, l++)
#endif
   {
      if (l > SCREENSIZE - 4)
      {
         l = 1;
         outputs("-- More --");
         inputs(string);
         if (string[0] == 'X' || string[0] == 'x')
            break;
      }

      max = rtxtcb[i].stacksize;
      max1 = max / sizeof(int);
      for (j = 0, p = (int  *)rtxtcb[i].stackbase; j < max1; j++, p++)
      {
         if (*p != STACK_FILL) /* look for 1st use of stack (non-init) */
            break;
      }
      used = (max1 - j) * sizeof(int);
      j *= sizeof(int);

      /* note - use of UI6 implies "size_t" is defined as "unsigned int" */
      outputf("(I3,X1,S8,3(UI6),/N)", &i, xtaskname(i), &max, &used, &j);
   }

   if (l > SCREENSIZE - 4)
   {
      outputs("-- More --");
      inputs(string);
      if (string[0] == 'X' || string[0] == 'x')
         return;
   }

#ifdef HAS_KSTACK
   max = RTXCSTKSZ;
   max1 = max / sizeof(int);
   for (j = 0, p = (int  *)&rtxctos[0]; j < max1; j++, p++)
   {
      if (*p != STACK_FILL)
         break;
   }
   used = (max1 - j) * sizeof(int);
   j *= sizeof(int);

   outputf("(/H'RTXC Kernel ',3(UI6),//N)", &max, &used, &j);
#endif

   outputf("(H'Worst case interrupt nesting =',B2,/N)", &isrmax);

   /* first zero semaphore marks the End of "used" area */
   for (j = 0, psema = &siglist[0]; *psema; j++, psema++)
        ;
   outputf("(H'Worst case signal list size =',I3,/N)", &j);
}
#endif

/* build input string the hard way - when gets() not supported */
/* note - no check is made for input string longer than buffer */
static void inputs(char *p)
{
   char *q, c, b;
   int cnt, getflg;

   getflg = YES;
   q = p;
   cnt = 1;
   while (getflg)
   {
      KS_dequeuew(CBUGIQ, &c);
      if (c != '\r')
      {
         if (c != ESC)
         {
            if (c == BS)
            {
               if (cnt > 1)
               {
                  --cnt;
                  --q;
               }
            }

            KS_enqueuew(CBUGOQ, &c);
            if ( (c == BS) || (c == DEL) )
            {
               b = ' ';
               KS_enqueuew(CBUGOQ, &b);
               KS_enqueuew(CBUGOQ, &c);
            }
            if ( (c >= 32) && (c <= 126) ) /* printable */
            {
               *q++ = c;
               cnt++;
            }
         }
         else
         {
            *p = c;
            cnt = 1;
            getflg = NO;
         }
      }
      else
         getflg = NO;
   }

   if (c == '\r')
   {
      KS_enqueuew(CBUGOQ, &c);
      *q = '\0';
   }
}

/* routine to "print" string to RTXCbug CONsole */
static void outputs(const char *string)
{
#if CBUGRES /* lock only if non-zero resource specified */
   KS_lockw(CBUGRES);
#endif

   while (*string != '\0') /* till end of string mark */
      KS_enqueuew(CBUGOQ, (void *)string++);

#if CBUGRES /* unlock only if non-zero resource specified */
   KS_unlock(CBUGRES);
#endif
}

/* variable # argument version of sprintf() followed by puts() */
static void outputf(const char *fmt, ...)
{
   va_list arg_ptr;        /* argument pointer */
   static char buffer[82];

   va_start(arg_ptr, fmt); /* init arg_ptr to point to 1st unnamed arg */

   formatv((char *)fmt, &buffer[0], arg_ptr);

   outputs(buffer);

   va_end(arg_ptr);
}

/*
 * general routine for forcing entry into RTXCbug
 * callable from anywhere by anybody including user tasks
 *
 */
void sgnlcbug(void)
{
   cbugflag = 1; /* set bug active flag - suppress clock service */

   KS_signal(CBUGSEMA);
}

/*
 * show corresponding timeout value for objtype, objid and task match
*/
static void showtime(OBJTYPE objtype, TASK task, OBJNUM objid)
{
   CLKBLK ks_clk *next = clkqptr;
   long time; /* note - use long not TICKS since used for time in ms */

   while (next != NULLCLK) /* walk timer list looking for matches */
   {
      if ( (next->objtype == objtype) &&
           (next->objid == objid) &&
           (next->task == task) )
      {
         time = next->remain - rtctick;
         time *= clktick;

         outputf("(L10,H' ms',N)", &time); /* output time */
      }
      next = next->flink;
   }
}

static char *xtaskname(TASK task)
{
#if 0
   int i;
#endif
   static char name[10];

   if (task <= ntasks)
      return((char *)&taskname[task][0]);
#ifdef DYNAMIC_TASKS
   else
   {
#if 0
      i = task; /* move to int type to avoid TASK typedef as char, etc. */
      formatv("(H'TASK',Z-,I4,N)", &name[0], &i);
#endif
      strcpy(name, "Unknown");
      return(&name[0]);
   }
#endif
}

#ifdef HAS_PARTITIONS
static char *xpartname(MAP map)
{
#if 0
   int i;
#endif
   static char name[10];

   if (map <= nparts)
      return((char *)&partname[map][0]);
#ifdef DYNAMIC_PARTS
   else
   {
#if 0
      i = map; /* move to int type to avoid MAP typedef as char, etc. */
      formatv("(H'PART',Z-,I4,N)", &name[0], &i);
#endif
      strcpy(name, "Unknown");
      return(&name[0]);
   }
#endif
}
#endif

static void menumain(void)
{
    outputs("\n\n");
    outputs("  K - RTXC\n");
    outputs("  G - Go to Multitasking Mode\n");
    outputs("  X - Exit RTXCbug\n");
}

static void menurtxc(void)
{
    outputs("\n");

    outputs("T - Tasks\n");
#ifdef HAS_MAILBOXES
    outputs("M - Mailboxes\n");
#endif
#ifdef HAS_PARTITIONS
    outputs("P - Partitions\n");
#endif
#ifdef HAS_QUEUES
    outputs("Q - Queues\n");
#endif
#ifdef HAS_RESOURCES
    outputs("R - Resources\n");
#endif
    outputs("S - Semaphores\n");
    outputs("C - Clock / Timers\n");
#ifdef STKLOGIC
    outputs("K - Stack Limits\n");
#endif
#if defined(HAS_PARTITIONS) || \
    defined(HAS_QUEUES) || \
    defined(HAS_RESOURCES)
    outputs("Z - Zero Partition/Queue/Resource Statistics\n");
#endif
    outputs("$ - Enter Task Manager Mode\n");
    outputs("# - Task Registers\n");
    outputs("G - Go to Multitasking Mode\n");
    outputs("H - Help\n");
    outputs("U - Return To Main Menu\n");
    outputs("X - Exit RTXCbug\n");
}
#endif /* } CBUG */

/* End of file - rtxcbug.c */
