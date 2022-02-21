/* rtxcbug.c - for the advanced and extended Libraries only */

/*
 *   RTXCio    
 *   Copyright (c) 
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

#ifdef RTXCIO /* { RTXCio */
#include "cclock.h"
#endif /* } RTXCio */

/* CONFIG */
#define STKLOGIC      /* indicates task stack limit support */

#define SCREENSIZE 25 /* 25 for PC, else usually 24 for most terminals */

#define MAXTIME (~(TICKS)0)

#define NULLTCB  ((TCB ks_tbl *)0)
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

#ifdef STKLOGIC
static void cbugstack(void);
extern char ks_stkx rtxctos[];
#endif

extern SSTATE ks_tblx semat[];
extern const char semaname[][NAMMAX+1];
extern const SEMA  nsemas;
extern SEMA ks_datax siglist[];

extern TCB ks_tblx rtxtcb[];
extern const char taskname[][NAMMAX+1];
extern const TASK  ntasks;
#ifdef DYNAMIC_TASKS
extern const TASK  dntasks;
#endif

#ifdef HAS_MAILBOXES
extern MHEADER ks_tblx mheader[];
extern const char mboxname[][NAMMAX+1];
extern const MBOX  nmboxes;
#endif

#ifdef HAS_PARTITIONS
extern PHEADER ks_tblx pheader[];
extern const char partname[][NAMMAX+1];
extern const MAP  nparts;
#ifdef DYNAMIC_PARTS
extern const MAP  dnparts;
#endif
#endif

#ifdef HAS_QUEUES
extern QHEADER ks_tblx qheader[];
extern const char queuename[][NAMMAX+1];
extern const QUEUE  nqueues;
#endif

#ifdef HAS_RESOURCES
extern RHEADER ks_tblx rheader[];
extern const char resname[][NAMMAX+1];
extern const RESOURCE  nres;
#endif

extern CLKBLK ks_clk * ks_data clkqptr; /* ptr to first active timer, initially NULLCLK */
extern volatile TICKS ks_data rtctick; /* no. of ticks since hw reset, initially 0 */

#ifdef HAS_INQTIME /* { */
extern volatile time_t ks_data rtctime; /* running seconds maintained by clock driver */
#endif /* } HAS_INQTIME */

extern const int  ntmrs;
extern const int  clktick;

#ifdef RTXCIO /* { RTXCio */
/*--------------------------------------------------------------------------*/
/* RTXCio - information for system level debugging                          */
/*--------------------------------------------------------------------------*/
extern CCB_ENTRY * chnls_free;                   /* Pointer to list - free  */
						 /* channel control blocks  */
extern CCB_ENTRY * chnls_active;                 /* Pointer to list - active*/
						 /* channel control blocks  */
extern CCB_ENTRY   channel_table[];              /* channel control block   */
						 /* table                   */
extern CNTLR_UNIT * cntlr_loc_table[];           /* controller locator table*/
extern PDU_HEADER * logical_dev_tbl[];           /* logical to physical     */
						 /* device table            */
extern CNTLR_UNIT   cntlr_table[];               /* table of controller     */
						 /* blocks                  */
extern const char devname[][NAMMAX+1];
extern const char cntlrname[][NAMMAX+1];

extern const char * IO_version(void);
/*--------------------------------------------------------------------------*/
#endif /* } RTXCio */

extern char isrmax;

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
static void showtime(OBJTYPE objtype, TASK task, int objid);

static void taskmgr(void);

static TASK gettask(void);
static PRIORITY getpriority(void);
#ifdef TIME_SLICE
static TICKS getslice(void);
#endif

#ifdef RTXCIO /* { RTXCio */
/*--------------------------------------------------------------------------*/
/* RTXCio - information functions                                           */
/*--------------------------------------------------------------------------*/
static void cbugcntlrs(void);
static void cbugdevs(void);
static void cbugchnls(void);
void cbugwrite( CHNLID, BLKNO, BUFFADD, BYTECNT );
void cbugread( CHNLID, BLKNO, BUFFADD, BYTECNT );
/*--------------------------------------------------------------------------*/
#endif /* } RTXCio */
static void inputs(char *); /* get '\r' terminated line from CBUGIQ */
static void outputs(const char *); /* := outputs(buff) */
static void outputf(const char *, ...); /* := formatv + output */
extern int formatv(const char *, char *, va_list);
extern int atoi(const char *);

#if _MODEL == 'm' || _MODEL == 'l'
extern unsigned long swap_pc_t0(unsigned long);
#endif

extern char cbugflag; /* clock ISR ignore flag, initially = 0 */

static char buffer[80]; /* used for formatting */
static TICKS et;

void sgnlcbug(void);
void rtxcbug(void);

static void cbugrtxc(void);                    /* RTXC object menu function */
static void cbugrtio(void);                  /* RTXCio object menu function */
static void menumain(void);                                    /* main menu */
static void menurtxc(void);   /* rtxc menu for "H" help option in rtxc menu */
static void menurtio(void);   /* rtio menu for "H" help option in rtio menu */
int goflag;           /* scope is range of local menues, used for GO option */

#ifndef CBUG /* { no RTXCbug wanted, but leave RTXCbug task defined */
void rtxcbug(void)
{
   KS_terminate(SELFTASK);
}
#else /* } CBUG { */

void rtxcbug(void)
{
#ifdef RTXCIO  /* { RTXCio */

#if defined(CBUG_ASYNCH_IO) || \
       defined(CBUG_TIMEOUT_IO)  /* { CBUG_ASYNCH_IO | SYNCH_IO */
   IORC ret_code;
#endif  /* } ASYNCH_IO | SYNCH_IO */

   cbug_in_chnlid = IO_chnl_open( (DEVID)CBUG_IN_PORT, IO_DIRECT,
		  READ_IO, NULLIOINFO );

   cbug_out_chnlid = IO_chnl_open( (DEVID)CBUG_OUT_PORT, IO_DIRECT,
		   WRITE_IO, NULLIOINFO );

#ifdef CBUG_ASYNCH_IO  /* { ASYNCH_IO */
   ret_code = IO_dev_open( (DEVID)CBUG_OUT_PORT, NULLIOINFO );
   while( ret_code < 0 )
      ret_code = IO_dev_open( (DEVID)CBUG_OUT_PORT, NULLIOINFO );

   ret_code = IO_dev_open( (DEVID)CBUG_IN_PORT, NULLIOINFO );
   while( ret_code < 0 )
      ret_code = IO_dev_open( (DEVID)CBUG_IN_PORT, NULLIOINFO );
#endif  /* } ASYNCH_IO */

#ifdef CBUG_SYNCH_IO  /* { SYNCH_IO */
   IO_dev_openw( (DEVID)CBUG_OUT_PORT, NULLIOINFO );

   IO_dev_openw( (DEVID)CBUG_IN_PORT, NULLIOINFO );
#endif  /* } SYNCH_IO */

#ifdef CBUG_TIMEOUT_IO  /* { TIMEOUT_IO */
   ret_code = IO_dev_opent( (DEVID)CBUG_OUT_PORT, NULLIOINFO, 5/CLKTICK );
   while( ret_code < 0 )
      ret_code = IO_dev_opent( (DEVID)CBUG_OUT_PORT, NULLIOINFO, 5/CLKTICK );

   ret_code = IO_dev_opent( (DEVID)CBUG_IN_PORT, NULLIOINFO, 5/CLKTICK );
   while( ret_code < 0 )
      ret_code = IO_dev_opent( (DEVID)CBUG_IN_PORT, NULLIOINFO, 5/CLKTICK );
#endif /* } TIMEOUT_IO */

#endif   /* } RTXCio */

#ifdef BSS_NOT_ZERO
   cbugflag = 0; /* reset */
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

#ifdef RTXCIO  /* { RTXCio */
      /* RTXCio - lock the serial i/o resource to ensure bug input */
      KS_lock( SIORES );
#endif   /* } RTXCio */

      cbugflag = 1; /* set bug active, in case not already set */

       /* block everybody except self */
#ifdef DYNAMIC_TASKS
      KS_block(1, ntasks + dntasks);
#else
      KS_block(1, ntasks);
#endif

#ifndef RTXCIO  /* { RTXCio */
      /* unblock i/o drivers */
      KS_unblock(CBUGIDRV, CBUGIDRV);
      KS_unblock(CBUGODRV, CBUGODRV);
#endif   /* } RTXCio */

      cbugmain();

      cbugflag = 0; /* disable bug on exit */
   }
}

static void cbugmain(void)
{
   char string[40];

   outputs("\n** RTXCbug - ");
   outputs(KS_version());

#ifdef RTXCIO  /* { RTXCio */
   outputs("\n           - ");
   outputs(IO_version());
#endif   /* } RTXCio */

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

#ifdef RTXCIO  /* { RTXCio */
         case 'I':
         case 'i':
            goflag = 0;
            cbugrtio();
	    if(goflag)
                  return;
            break;
#endif   /* } RTXCio */

         case 'X':
         case 'x':
         case 'G':
         case 'g':
            outputs("\nReturning from RTXCBug\n");

#ifdef RTXCIO /* { */
            /* RTXCio - unlock the serial i/o resource */
            KS_unlock( SIORES );
#endif /* } */

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
      outputs("\nRTXCbug - RTXC Objects > ");

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

#ifndef RTXCIO  /* { RTXCio */
           /* unblock i/o drivers */
           KS_unblock(CBUGIDRV, CBUGIDRV);
           KS_unblock(CBUGODRV, CBUGODRV);
#endif   /* } RTXCio */

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

#ifdef RTXCIO /* { */
            /* RTXCio - unlock the serial i/o resource */
            KS_unlock( SIORES );
#endif /* } */

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

static void cbugrtio(void)
{
   char string[40];

   for (;;)
   {
      outputs("\nRTXCbug - RTXCio Objects > ");

      inputs(string);

      switch(string[0])
      {

#ifdef RTXCIO /* { */
         case 'D':
         case 'd':
            cbugdevs();
            break;

         case 'C':
         case 'c':
            cbugcntlrs();
            break;

         case 'L':
         case 'l':
            cbugchnls();
            break;
#endif /* } RTXCIO */

         case 'H':
         case 'h':
            menurtio();
            break;

         case 'X':
         case 'x':
         case 'G':
         case 'g':
            outputs("\nReturning from RTXCBug\n");

#ifdef RTXCIO /* { */
            /* RTXCio - unlock the serial i/o resource */
            KS_unlock( SIORES );
#endif /* } */

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
   int i, j, l;
   int priority;
   char string[16];
#if defined(MAILBOX_SEMAS) || defined(QUEUE_SEMAS)
   int k;
#endif
#if defined(PARTITION_WAITERS) || \
    defined(MAILBOX_WAITERS)   || \
    defined(QUEUE_WAITERS)     || \
    defined(RESOURCE_WAITERS)
   TCB ks_tbl *nexttask;
#endif

   outputs("\n** Task Snapshot **\n");
   outputs(  "  #   Name  Priority  State\n");
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
   int i, l;
   char string[16];
#ifdef QUEUE_WAITERS
   TCB ks_tbl *nexttask;
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
   int i, l;
   TASK task;
   char string[16];
#if defined(QUEUE_SEMAS) || defined(MAILBOX_SEMAS)
   int k;
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
      switch(semat[i])
      {
         case SEMA_DONE:
            outputs("DONE\n");
            break;

         case SEMA_PENDING:
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
   int i, l;
   char string[16];
#ifdef RESOURCE_WAITERS
   TCB ks_tbl *nexttask;
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
   int i, j, l;
   struct xmap *next;
   char string[16];
#ifdef PARTITION_WAITERS
   TCB ks_tbl *nexttask;
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
   int i, j, l;
   char string[16];
   MHEADER ks_tblx *pmh;
   RTXCMSG *next;
#ifdef MAILBOX_WAITERS
   TCB ks_tbl *nexttask;
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
       &ntmrs, (void *)&rtctick);
   outputf("(H', ET is ',I9,H' ticks, RTC time is ',L9,//N)", (void *)&et,(void *)&rtctime);

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
   int i;

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
   FRAME ks_stk *frame;
#if _MODEL == 'm' || _MODEL == 'l'
   unsigned long pc1;
#endif

   if ( (task = gettask()) != 0 )
   {
      if (task == RTXCBUG)
      {
         outputs("Register set of current task is meaningless\n");
	 return;
      }

      if ( (frame = (FRAME ks_stk *)rtxtcb[task].sp) == NULL)
      {
         outputs("Task Register set is undefined\n");
	 return;
      }

      outputs("** Task Register Snapshot **\n");
   outputs(
  "   0    1    2    3    4    5    6    7\n");
/* R 1234 1234 1234 1234 1234 1234 1234 1234 */
/* PC 1234 PSW 1234                       */

#if _MODEL != 'l'  /* { */
      outputf("(H'R ',R16,Z-,8(UI4,X1),/N)",
          &frame->pksnum, &frame->R1, &frame->R2, &frame->R3,
          &frame->R4, &frame->R5, &frame->R6, &frame);
#else
#ifdef KS_STK_NEAR  /* { */
      outputf("(H'R ',R16,Z-,8(UI4,X1),/N)",
          (void *)&frame->pksnum, (void *)&frame->R1,
          (void *)&frame->R2, (void *)&frame->R3,
          (void *)&frame->R4, (void *)&frame->R5,
          (void *)&frame->R6, &frame);
#else
      outputf("(H'R ',R16,Z-,UI8,X1,6(UI4,X1),/N)",
       &frame->pksnum, &frame->R2, &frame->R3,
          &frame->R4, &frame->R5, &frame->R6, &frame);
/*       outputf("(H'',R16,Z-,6(UI4,X1),/N)", &frame->R2, &frame->R3,
          &frame->R4, &frame->R5, &frame->R6, &frame); */
#endif    /* } KS_STK_NEAR */
#endif /* } _MODEL != 'l' */

#if _MODEL == 'm' || _MODEL == 'l'
      pc1 = (unsigned long)frame->pc;
      pc1 = swap_pc_t0(pc1);
      outputf("(R16,Z-,H'PC ',UL8,H' PSW ',UI4/N)", &pc1, (void *)&frame->psw);
#else
      outputf("(R16,Z-,H'PC ',UI4,H' PSW ',UI4/N)", &frame->pc, &frame->psw);
#endif

      outputf("(R16,Z-,H'DS ',UB2,H' ES ',UB2/N)", (void *)&frame->ds, (void *)&frame->es);
      outputf("(R16,Z-,H'SSEL ',UB2,H' CS ',UB2/N)", (void *)&frame->ssel, (void *)&frame->cs);
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
   PRIORITY priority = 0;
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
   TICKS tslice = 0;
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

#ifndef RTXCIO /* { */
               /* unblock i/o drivers */
               KS_unblock(CBUGIDRV, CBUGIDRV);
               KS_unblock(CBUGODRV, CBUGODRV);
#endif /* } */

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
   short int ks_stk *p;
   SEMA ks_tbl *psema;

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
      max1 = max / 2;
      for
       (j = 0, p = (short int ks_stk *)rtxtcb[i].stackbase; j < max1; j++, p++)
      {
            if (*p != STACK_FILL) /* look for 1st use of stack (non-init) */
            break;
      }
      used = (max1 - j) * 2;
      j *= 2;

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

   max = RTXCSTKSZ;
   max1 = max / 2;
   for (j = 0, p = (short int ks_stk *)&rtxctos[0]; j < max1; j++, p++)
   {
         if (*p != STACK_FILL)
         break;
   }
   used = (max1 - j) * 2;
   j *= 2;
   
   outputf("(/H'RTXC Kernel ',3(UI6),//N)", &max, &used, &j);

   outputf("(H'Worst case interrupt nesting =',B2,/N)", &isrmax);

   /* first zero semaphore marks the End of "used" area */
   for (j = 0, psema = &siglist[0]; *psema; j++, psema++)
        ;
   outputf("(H'Worst case signal list size =',I3,/N)", &j);
}
#endif

#ifdef RTXCIO   /* { */
/*--------------------------------------------------------------------------*/
/* RTXCio - information functions                                           */
/*--------------------------------------------------------------------------*/
static void cbugcntlrs(void)
{
   int i, l;
   char string[16];

   outputs("\n** RTXCIO - Controller Snapshot **\n");
   outputs("\n  #    Name     Address  Number_of_units   Size_of_units\n");
      /* 123  12345678  12345678        123             123         */

   for (i = 1, l = 1; i <= NCNTLRS; i++, l++)
   {
    if( cntlr_loc_table[i] != NULLCNTLR )
    {
        if (l > SCREENSIZE - 4)
        {
            l = 1;
            outputs("-- More --");
            inputs(string);
            if (string[0] == 'X' || string[0] == 'x')
            break;
        }
        outputf("(I3,X2,S8,X2,I8,X8,I3,X13,I3,N)",
            &i, &cntlrname[i][0], &(cntlr_loc_table[i]->cntlr_address),
            &(cntlr_loc_table[i]->number_of_units), &(cntlr_loc_table[i]->size_of_units) );
    outputs("\n");
    }
   }
}

static void cbugdevs(void)
{
   int i, l;
   char string[16];
   char *attr, *last_oper, *lost_data;

   outputs("\n** RTXCIO - Device Snapshot **\n");
   outputs("\n  Device     Unit Cntlr     Resource  Semaphore Attr   Last      Lost Bytes\n");
     outputs("  #  Name      #   Name      Name      Name            Operation Data Transfered\n");
           /*123 12345678 123  12345678  12345678  12345678  12345  123456789  123   12345*/

   for (i = 1, l = 1; i <= NLDEVS; i++, l++)
   {
    if( logical_dev_tbl[i] != NULLUHEADER )
    {
        if (l > SCREENSIZE - 4)
        {
            l = 1;
            outputs("-- More --");
            inputs(string);
            if (string[0] == 'X' || string[0] == 'x')
            break;
        }

        switch( logical_dev_tbl[i]->dev_attribute )
        {
            case ATTB_READ:
                attr = " READ";
                break;
            case ATTB_WRITE:
                attr = "WRITE";
                break;
            case ATTB_RDWR:
                attr = "RD/WR";
                break;
            default:
                attr = " NONE";
                break;
        }

        switch( logical_dev_tbl[i]->last_io_oper )
        {
            case IO_NONE:
                last_oper = "     NONE";
                break;
            case IO_READ:
                last_oper = "     READ";
                break;
            case IO_WRITE:
                last_oper = "    WRITE";
                break;
            case IO_DEV_OPEN:
                last_oper = " DEV_OPEN";
                break;
            case IO_DEV_CLOSE:
                last_oper = "DEV_CLOSE";
                break;
            case IO_CONTROL:
                last_oper = "  CONTROL";
                break;
            default:
                last_oper = "     NONE";
                break;
        }

        if(logical_dev_tbl[i]->lost_data)
            lost_data = "YES";
        else
            lost_data = " NO";

        outputf("(I3,X1,S8,X1,I3,X2,S8,X2,S8,X2,S8,X2,S5,X2,S9,X2,S3,X3,I5,N)",&i,
            &devname[i],
            &logical_dev_tbl[i]->dev_unitno,
            &cntlrname[logical_dev_tbl[i]->dev_cntlr->cntlr_number][0],
            &resname[logical_dev_tbl[i]->dev_res][0],
            &semaname[logical_dev_tbl[i]->dev_sema][0],
            attr,
            last_oper,
            lost_data,
            &logical_dev_tbl[i]->bytes_transfered);
    outputs("\n");
    }
   }
}

static void  cbugchnls(void)
{
   int i, j, l;
   CCB_ENTRY *next;
   char string[16];
   char *iocmmd, *iometh;

   outputs("\n** RTXCIO - Channel Snapshot **\n");
   outputs("\n  #   Device   I/O Command  I/O Method \n");
      /* 123  12345678        12345    12345678 */

   j = 0;
   next = chnls_free;
   while (next)
   {
     next = next->flink;
     j++;
   }

   for (i = 1, l = 1; i <= NCHNLS; i++, l++)
   {

      if( channel_table[i].blink != NULLCCBE )
      {
        if (l > SCREENSIZE - 4)
        {
            l = 1;
            outputs("-- More --");
            inputs(string);
            if (string[0] == 'X' || string[0] == 'x')
                break;
        }

        switch( channel_table[i].io_command )
        {
            case READ_IO:
                iocmmd = " READ";
                break;
            case WRITE_IO:
                iocmmd = "WRITE";
                break;
            case RDWR_IO:
                iocmmd = "RD/WR";
                break;
            default:
                iocmmd = " NONE";
                break;
        }

        if( channel_table[i].io_method == IO_DIRECT )
            iometh = "  DIRECT";
        else
            iometh = "BUFFERED";

        outputf("(I3,X2,S8,X8,S5,X4,S8,N)",
            &channel_table[i].chnl_number,
            &devname[channel_table[i].io_deviceno][0],
            iocmmd, iometh );
      outputs("\n");
      }
   }
   outputs("The number of available channels: ");
   outputf("(I3,N)", &j );
   outputs("\n");
}
#endif   /* } RTXCio */

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
#ifdef RTXCIO /* { RTXCio */
      cbugread( cbug_in_chnlid, CBUG_BLOCK, (BUFFADD)&c, 1 );
#else /* use standard RTXCbug */
      KS_dequeuew(CBUGIQ, &c);
#endif        /* } RTXCio */
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

#ifdef RTXCIO /* { RTXCio */
        cbugwrite( cbug_out_chnlid, CBUG_BLOCK, (BUFFADD)&c, 1 );
#else /* use standard RTXCbug */
            KS_enqueuew(CBUGOQ, &c);
#endif        /* } RTXCio */
            if ( (c == BS) || (c == DEL) )
            {
               b = ' ';
#ifdef RTXCIO /* { RTXCio */
        cbugwrite( cbug_out_chnlid, CBUG_BLOCK, (BUFFADD)&b, 1 );
        cbugwrite( cbug_out_chnlid, CBUG_BLOCK, (BUFFADD)&c, 1 );
#else /* use standard RTXCbug */
               KS_enqueuew(CBUGOQ, &b);
               KS_enqueuew(CBUGOQ, &c);
#endif        /* } RTXCio */
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
#ifdef RTXCIO /* { RTXCio */
    cbugwrite( cbug_out_chnlid, CBUG_BLOCK, (BUFFADD)&c, 1 );
#else /* use standard RTXCbug */
      KS_enqueuew(CBUGOQ, &c);
#endif        /* } RTXCio */
      *q = '\0';
   }
}

/* routine to "print" string to RTXCbug CONsole */
static void outputs(const char *string)
{
#if CBUGRES /* lock only if non-zero resource specified */
   KS_lockw(CBUGRES);
#endif

#ifdef RTXCIO /* { RTXCio */
    cbugwrite( cbug_out_chnlid, CBUG_BLOCK, (BUFFADD)string, (BYTECNT)strlen(string) );
#else /* use standard RTXCbug */
   while (*string != '\0') /* till end of string mark */
      KS_enqueuew(CBUGOQ, string++);
#endif        /* } RTXCio */

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

   formatv(fmt, &buffer[0], arg_ptr);

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
static void showtime(OBJTYPE objtype, TASK task, int objid)
{
   CLKBLK ks_clk *next = clkqptr;
   long time; /* note - use long not TICKS since used for time in ms */

   while (next != NULLCLK) /* walk timer list looking for matches */
   {
      if ( (next->objtype == objtype) &&
           (next->objid == objid) &&
           (next->task == task) )
      {
         if (next->remain > rtctick)
            time = next->remain - rtctick;
         else
            time = next->remain + (MAXTIME - rtctick) + 1;
         time *= clktick;

         outputf("(L10,H' ms',N)", &time); /* output time */
      }
      next = next->flink;
   }
}

static char *xtaskname(TASK task)
{
   int i;
   static char name[10];

   if (task <= ntasks)
      return((char *)&taskname[task][0]);
#ifdef DYNAMIC_TASKS
   else
   {
      i = task; /* move to int type to avoid TASK typedef as char, etc. */
#if 0
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
   int i;
   static char name[10];

   if (map <= nparts)
      return((char *)&partname[map][0]);
#ifdef DYNAMIC_PARTS
   else
   {
      i = map; /* move to int type to avoid MAP typedef as char, etc. */
#if 0
      formatv("(H'PART',Z-,I4,N)", &name[0], &i);
#endif
      strcpy(name, "Unknown");
      return(&name[0]);
   }
#endif
}
#endif

#ifdef RTXCIO  /* { RTXCio */
void cbugwrite( CHNLID chnlid, BLKNO block, BUFFADD trans_buff, BYTECNT trans_count )
{
#if defined(CBUG_ASYNCH_IO) || \
    defined(CBUG_TIMEOUT_IO)  /* { CBUG_ASYNCH_IO | SYNCH_IO */
    IORC ret_code;
#endif  /* } ASYNCH_IO | SYNCH_IO */

#ifdef CBUG_ASYNCH_IO
    ret_code = IO_write( chnlid, block, trans_buff, trans_count);
    while( ret_code < 0 )
        ret_code = IO_write( chnlid, block, trans_buff, trans_count);
    IO_dev_wait( chnlid );
#endif

#ifdef CBUG_SYNCH_IO
    IO_writew( chnlid, block, trans_buff, trans_count);
#endif

#ifdef CBUG_TIMEOUT_IO
    ret_code = IO_writet( chnlid, block, trans_buff, trans_count, 5/CLKTICK );
    while( ret_code < 0 )
        ret_code = IO_writet( chnlid, block, trans_buff, trans_count, 5/CLKTICK );
#endif

}
void cbugread( CHNLID chnlid, BLKNO block, BUFFADD trans_buff, BYTECNT trans_count )
{

#if defined(CBUG_ASYNCH_IO) || \
    defined(CBUG_TIMEOUT_IO)  /* { CBUG_ASYNCH_IO | SYNCH_IO */
    IORC ret_code;
#endif  /* } ASYNCH_IO | SYNCH_IO */

#ifdef CBUG_ASYNCH_IO
    ret_code = IO_read( chnlid, block, trans_buff, trans_count);
    while( ret_code < 0 )
        ret_code = IO_read( chnlid, block, trans_buff, trans_count);
    IO_dev_wait( chnlid );
#endif

#ifdef CBUG_SYNCH_IO
    IO_readw( chnlid, block, trans_buff, trans_count);
#endif

#ifdef CBUG_TIMEOUT_IO
    ret_code = IO_readt( chnlid, block, trans_buff, trans_count, 5/CLKTICK );
    while( ret_code < 0 )
        ret_code = IO_readt( chnlid, block, trans_buff, trans_count, 5/CLKTICK );
#endif
}
#endif  /* } RTXCio */

static void menumain(void)
{
    outputs("\n\n");
    outputs("  K - RTXC\n");
#ifdef RTXCIO  /* { RTXCio */
    outputs("  I - RTXCio\n");
#endif   /* } RTXCio */
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

static void menurtio(void)
{
    outputs("\n");

    outputs("L - channeLs\n");
    outputs("C - Controllers\n");
    outputs("D - Devices\n");
    outputs("G - Go to Multitasking Mode\n");
    outputs("H - Help\n");
    outputs("U - Return To Main Menu\n");
    outputs("X - Exit RTXCbug\n");
}
#endif /* } CBUG */
