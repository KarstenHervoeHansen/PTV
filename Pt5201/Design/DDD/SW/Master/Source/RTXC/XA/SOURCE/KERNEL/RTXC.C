/* rtxc.c - RTXC entry point from isvcrtx() */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/


/*****************************************************************************/
/* Changes made to the kernel : */

/*    3/6-99 the following changes is made by TK-PTV */

/*	KS_deftime(long)	corrected for reset of tick counter. */

/*	KS_inqtime()		appended reading of tick count in variable
				rtctimetick after invocation of KS_inqtime().*/

/*****************************************************************************/


#include "typedef.h"
#include "rtxstruc.h"
#include "rtxcarg.h"
#include "tstate.h"
#include "enable.h"

extern void * memcpy(void *, const void *, size_t);

static void ks_memcpyFN(void ks_queue *, const void          *, size_t);
static void ks_memcpyNF(void          *, const void ks_queue *, size_t);

#if _MODEL == 'm' || _MODEL == 'l'
unsigned long swap_pc_t0(unsigned long);
#endif /* } _MODEL */

/* state of CLKBLKs */
#define TIMER_ACTIVE    1 /* timer counting down */
#define TIMER_DONE      0 /* timer already expired */

#define NULL     ((void *)0)
#define NULLTCB  ((TCB ks_tbl *)0)
#define NULLMAP  ((PHEADER ks_tbl *)0)
#define NULLCLK  ((CLKBLK ks_clk *)0)
#define NULLSEMA ((SEMA)0)
#define NULLFUNC ((void (*)(void))0)
#define NULLARG  ((void *)0)
#ifdef FPU /* { */
#define NULLFPREGS ((FPREGS ks_tbl *)0)
#endif /* } FPU */

#define SELFTASK ((TASK)0)

/* stash sema in siglist */
#define SIGNAL(sema) PUSHPSWH; DISABLE; *++semaput = sema; POPPSWH

extern TCB ks_tblx rtxtcb[];
extern const KTCB rtxktcb[];
extern const TASK ntasks;
#ifdef DYNAMIC_TASKS /* { */
extern const TASK dntasks;
#endif /* } DYNAMIC_TASKS */

extern SSTATE ks_tblx semat[];
extern SEMA ks_datax siglist[];
extern const SEMA nsemas;
#ifdef BSS_NOT_ZERO /* { */
extern const int siglistsize;
#endif /* } BSS_NOT_ZERO */

extern CLKBLK ks_clkx clkq[];
extern const int ntmrs;

#ifdef HAS_MAILBOXES /* { */
extern MHEADER ks_tblx mheader[];
extern const MBOX nmboxes;
#endif /* } HAS_MAILBOXES */

#ifdef HAS_PARTITIONS /* { */
extern PHEADER ks_tblx pheader[];
extern const PKHEADER pkheader[];
extern const MAP nparts;
#ifdef DYNAMIC_PARTS /* { */
extern const MAP dnparts;
#endif /* } DYNAMIC_PARTS */
#endif /* } HAS_PARTITIONS */

#ifdef HAS_QUEUES /* { */
extern QHEADER ks_tblx qheader[];
extern const QKHEADER qkheader[];
extern const QUEUE nqueues;
#endif /* } HAS_QUEUES */

#ifdef HAS_RESOURCES /* { */
extern RHEADER ks_tblx rheader[];
extern const RESOURCE nres;
#endif /* } HAS_RESOURCES */

TCB ks_tbl * core_iramdata hipritsk; /* highest priority task */
TCB ks_tbl * core_iramdata nsrttcb;  /* list of tcbs pending scheduling */

volatile SEMA ks_data * core_iramdata semaput;

SEMA ks_data * core_iramdata semaget;

#ifdef DYNAMIC_TASKS /* { */
TCB ks_tbl * ks_data dtcbfl; /* head of tcb free list,initially &rtxtcb[NTASKS+1] */
#endif /* } DYNAMIC_TASKS */

#ifdef HAS_PARTITIONS /* { */
#ifdef DYNAMIC_PARTS /* { */
PHEADER ks_tbl * ks_data dphfl; /* head of partition header free list */
                                   /* initially &pheader[NPARTS+1] */
static void dn_defpart(PHEADER ks_tbl *pph, KSNUM ks_stk *p2);
#endif /* } DYNAMIC_PARTS */

void * KS_ISRalloc(MAP);
#endif /* } HAS_PARTITIONS */

volatile TICKS ks_data rtctick; /* no. of ticks since hw reset, initially 0 */

#ifdef HAS_INQTIME /* { */
volatile time_t ks_data rtctime; /* running seconds maintained by clock driver */
#endif /* } HAS_INQTIME */

CLKBLK ks_clk * ks_data clkqptr; /* ptr to first active timer, initially NULLCLK */
CLKBLK ks_clk * ks_data clkqfl;  /* ptr to timer free list, initially &clkq[0] */
#ifdef TIME_SLICE /* { */
TCB ks_tbl * core_iramdata sliceup;
#endif /* } TIME_SLICE */
extern const int clkrate;

#ifdef HAS_INQTIME /* { */
static int ks_data ratecnt; /* clkrate counter (0 -> clkrate-1) */
int rtctimetick;
#endif /* } HAS_INQTIME */

#ifdef FPU /* { */
TCB ks_tbl * ks_data fputask; /* current owner of FPU */
extern void fpuswap(FPREGS ks_tbl *oldregs, FPREGS ks_tbl *newregs);
#endif /* } FPU */

#ifdef CBUG /* { */
char cbugflag; /* RTXCbug active flag */
char isrmax;  /* worst case interrupt depth, initially = 0 */
extern char ks_stkx rtxctos[];
void stkinit(short ks_stk *, size_t);
#endif /* } CBUG */

char core_iramdata isrcnt; /* count of "levels" of interrupts, initially = 0 */
unsigned char core_iramdata intlvl; /* current processor priority level for
                                       tasks. Initialized in startup code */

static volatile char ks_data t_expired;

static FRAME ks_stk *postem(void);

#ifdef HAS_MAILBOXES /* { */
static void insert_message(MHEADER ks_tbl *pmh, RTXCMSG *prtxcmsg);
static RTXCMSG *remove_message(MHEADER ks_tbl *pmh, TASK task);
#endif /* } HAS_MAILBOXES */

static CLKBLK ks_clk *get_clkblk(void);
static void insert_timer(CLKBLK ks_clk *pclkblk);
static void unlink_timer(CLKBLK ks_clk *pclkblk);

#if defined(MAILBOX_WAITERS)   || \
    defined(PARTITION_WAITERS) || \
    defined(QUEUE_WAITERS)     || \
    defined(RESOURCE_WAITERS) /* { */
#define COMBO_WAIT (MSG_WAIT | PARTITION_WAIT | QUEUE_WAIT | RESOURCE_WAIT)
static void porder(TCB ks_tbl *ptcb);
static void reorder_waiters(TCB ks_tbl *ptcb);
static void fwd_insert(TCB ks_tbl *ptcb);
static void bwd_insert(TCB ks_tbl *ptcb);
#endif /* } - MAILBOX_ || PARTITION_ || QUEUE_ || RESOURCE_WAITERS */

static void chgpriority(TCB ks_tbl *ptcb, PRIORITY priority);

#ifdef PRIORITY_INVERSION /* { */
/*
 *
 * the following variable defines the initial conditions of all
 * PRIORITY_INVERSION resource attributes. Setting to _OFF is
 * default. Use KS_defres() at runtime to set individual resources to _ON.
 *
*/
#define PRIORITY_INVERSION_T0 PRIORITY_INVERSION_OFF
#endif /* } PRIORITY_INVERSION */

/* choice of function (smaller) code or inline (faster) */
#ifdef INLINE_MACRO_EXPANSION /* { */
#define UNLINK_HIPRITSK() hipritsk = hipritsk->flink; \
                          hipritsk->blink = (TCB ks_tbl *)&hipritsk

#define UPDATE_SEMA(sema) if ((sema) != NULLSEMA) \
                          { \
                             psstate = (SSTATE ks_tbl *)&semat[(sema)]; \
                             \
                             if (*psstate == SEMA_PENDING) \
                                *psstate = SEMA_DONE; \
                             else if (*psstate != SEMA_DONE) \
                             { \
                                SIGNAL((sema)); \
                             } \
                          }
#else /* } INLINE_MACRO_EXPANSION { */
#define UNLINK_HIPRITSK() unlink_hipritsk()
static void unlink_hipritsk(void)
{
   hipritsk = hipritsk->flink;
   hipritsk->blink = (TCB ks_tbl *)&hipritsk;
}
#define UPDATE_SEMA(sema) if ((sema) != NULLSEMA) update_sema((sema))
static void update_sema(SEMA sema)
{
   unsigned char pswh;  /* local variable for saving processor interrupt state */
   SSTATE ks_tbl *psstate;

   psstate = (SSTATE ks_tbl *)&semat[sema];

   if (*psstate == SEMA_PENDING)
      *psstate = SEMA_DONE;
   else if (*psstate != SEMA_DONE)
   {
      SIGNAL(sema);
   }
}
#endif /* } INLINE_MACRO_EXPANSION */

/* local function prototypes */
FRAME ks_stk * rtxc(FRAME ks_stk *);
FRAME ks_stk * KS_ISRexit(FRAME ks_stk *, SEMA);
void KS_ISRsignal(SEMA);
int KS_ISRtick(void);
void taskinit(void);
void semainit(void);
void clockinit(void);
#ifdef HAS_RESOURCES /* { */
void resinit(void);
#endif /* } HAS_RESOURCES */
#ifdef HAS_MAILBOXES /* { */
void mboxinit(void);
#endif /* } HAS_MAILBOXES */
#ifdef HAS_PARTITIONS /* { */
void partinit(void);
#endif /* } HAS_PARTITIONS */
#ifdef HAS_QUEUES /* { */
void queueinit(void);
#endif /* } HAS_QUEUES */

/* entry point for all kernel directives */
FRAME ks_stk * rtxc(FRAME ks_stk *p)
{
   unsigned char pswh;  /* local variable for saving processor interrupt state */
   KSNUM ks_stk *p2 = p->pksnum;
   FRAME ks_stk *frame;
   TCB ks_tbl *ptcb;
   TASK task;
   CLKBLK ks_clk *pclkblk;
   TICKS tcnt; /* local copy of rtctick */
   SEMA sema;
   const SEMA *semalist;
   SSTATE ks_tbl *sema_ptr = (SSTATE ks_tbl *)0;
   KSNUM ks_stk *p2a;

#ifdef HAS_WAITM /* { */
   const SEMA *semalist2;
#endif /* } HAS_WAITM */

#ifdef HAS_BLOCK /* { */
   TASK endtask;
#endif /* } HAS_BLOCK */

#ifdef HAS_MAILBOXES /* { */
   MHEADER ks_tbl *pmh;
   RTXCMSG *prtxcmsg;
#endif /* } HAS_MAILBOXES */

#ifdef HAS_PARTITIONS /* { */
   PHEADER ks_tbl *pph;
   struct xmap *q;
#endif /* } HAS_PARTITIONS */

#ifdef HAS_QUEUES /* { */
   QHEADER ks_tbl *pqh;
   int qindex, depth;
   size_t width;
#ifdef HAS_DEFQUEUE /* { */
   int cursz;
#endif /* } HAS_DEFQUEUE */
#endif /* } HAS_QUEUES */

#ifdef HAS_RESOURCES /* { */
   RHEADER ks_tbl *prh;
#endif /* } HAS_RESOURCES */

#ifdef FPU /* { */
   FPREGS ks_tbl *fpregs;
#endif /* } FPU */

#if _MODEL == 'm' || _MODEL == 'l'
   unsigned long pc1;
#endif /* } _MODEL */

#ifdef INLINE_MACRO_EXPANSION /* { */
   SSTATE ks_tbl *psstate;
#endif /* } INLINE_MACRO_EXPANSION */

   hipritsk->sp = (FRAME ks_stk *)p; /* always save &stk frame */

/* force the compiler to generate a JUMP TABLE - deterministic */
#pragma optimize t

   switch(*p2)
   {
/************************/
      case RTXC_SIGNAL:
/************************/
         sema = ((struct sarg ks_stk *)p2)->sema;
         sema_ptr = (SSTATE ks_tbl *)&semat[sema];

         if (*sema_ptr == SEMA_DONE)
            ((struct sarg ks_stk *)p2)->ksrc = RC_MISSED_EVENT;
         else if (*sema_ptr == SEMA_PENDING)
            *sema_ptr = SEMA_DONE;
         else
         {
            SIGNAL(sema);
         }
         break;

#ifdef HAS_SIGNALM /* { */
/************************/
      case RTXC_SIGNALM:
/************************/
         for (semalist = ((struct sargm ks_stk *)p2)->list;
            (sema = *semalist) != NULLSEMA; semalist++)
         {
            sema_ptr = (SSTATE ks_tbl *)&semat[sema];
            if (*sema_ptr == SEMA_DONE)
               ;
            else if (*sema_ptr == SEMA_PENDING)
               *sema_ptr = SEMA_DONE;
            else
            {
               SIGNAL(sema);
            }
         }
         break;
#endif /* } HAS_SIGNALM */

/************************/
      case RTXC_WAIT:
/************************/
         sema = ((struct sarg ks_stk *)p2)->sema;
         sema_ptr = (SSTATE ks_tbl *)&semat[sema];

         if (*sema_ptr == SEMA_PENDING)
         {
            ptcb = hipritsk; /* save for later */

#ifdef SEMAPHORE_TIMEOUTS /* { */
            if ( ((struct sarg ks_stk *)p2)->ticks) /* if timeout active */
            {
               pclkblk = ptcb->pclkblk = ((struct sarg ks_stk *)p2)->pclkblk;
               pclkblk->remain = ((struct sarg ks_stk *)p2)->ticks;
               pclkblk->recycle = (TICKS)0; /* non-cyclic */

               /* mark object type */
               pclkblk->objtype = SEMAPHORE_OBJ;
               pclkblk->objid = sema;
               pclkblk->task = ptcb->task;

               /* insert timer */
               insert_timer(pclkblk);
               ((struct sarg ks_stk *)p2)->ksrc = RC_TIMEOUT;
            }
#endif /* } SEMAPHORE_TIMEOUTS */

            /* put sema into wait state */
            *sema_ptr = (SSTATE)ptcb->task;

            ptcb->status = SEMAPHORE_WAIT;

            UNLINK_HIPRITSK(); /* unlink first tcb */
            break;
         }

         if (*sema_ptr == SEMA_DONE) /* if sema already done */
         {
            *sema_ptr = SEMA_PENDING;  /* set sema pending */
         }
         else     /* else sema was already in a wait state */
         {
            /* if here, application task design flaw */
               /* since no corresponding semaphore */
            ((struct sarg ks_stk *)p2)->ksrc = RC_WAIT_CONFLICT;
         }
         break;

#ifdef HAS_WAITM /* { */
/************************/
      case RTXC_WAITM:
/************************/
         semalist = ((struct sargm ks_stk *)p2)->list;
         while (*semalist != NULLSEMA) /* while not at end of waitm list */
         {
            sema_ptr = (SSTATE ks_tbl *)&semat[*semalist]; /* get ptr to sema */

            if (*sema_ptr == SEMA_PENDING)
            {
               *sema_ptr = (SSTATE)hipritsk->task; /* set sema waiting */
               semalist++;                          /* bump list ptr */
            }
            else /* found sema in WAIT or DONE state */
               break;
         }

         if (*semalist == NULLSEMA) /* all semas now in WAIT state */
         {
            hipritsk->status = SEMAPHORE_WAIT; /* mark status SEMAPHORE_WAIT */

            UNLINK_HIPRITSK(); /* unlink first tcb */
            break;            /* force task switch */
         }

         if (*sema_ptr == SEMA_DONE) /* found a sema in list in DONE state */
         {
            /* return sema to user */
            ((struct sargm ks_stk *)p2)->sema = *semalist;

               /* set all semas in list to PEND state */
            for ( semalist2 = ((struct sargm ks_stk *)p2)->list;
                  semalist2 <= semalist;
                  semalist2++)
               semat[*semalist2] = SEMA_PENDING;
         }
         else /* semaphore was found already in a WAIT state */
         {
               /* if arrive here, application task design flaw */
                 /* since no corresponding semaphore waiting */
            hipritsk->status = SEMAPHORE_WAIT;

            UNLINK_HIPRITSK(); /* unlink first tcb */
         }
         break;
#endif /* } HAS_WAITM */

#ifdef HAS_PEND /* { */
/************************/
      case RTXC_PEND:
/************************/
         sema = ((struct sarg ks_stk *)p2)->sema;
         sema_ptr = (SSTATE ks_tbl *)&semat[sema];

         if (*sema_ptr == SEMA_DONE)
            *sema_ptr = SEMA_PENDING;
         break;
#endif /* } HAS_PEND */

#ifdef HAS_PENDM /* { */
/************************/
      case RTXC_PENDM:
/************************/
         for (semalist = ((struct sargm ks_stk *)p2)->list;
              (sema = *semalist) != NULLSEMA; semalist++)
         {
            sema_ptr = (SSTATE ks_tbl *)&semat[sema];
            if (*sema_ptr == SEMA_DONE)
               *sema_ptr = SEMA_PENDING;
         }
         break;
#endif /* } HAS_PENDM */

#ifdef HAS_SUSPEND /* { */
/************************/
      case RTXC_SUSPEND:
/************************/
         if ( (task = ((struct targ ks_stk *)p2)->task) == SELFTASK)
            ptcb = hipritsk;
         else
         {
            ptcb = (TCB ks_tbl *)&rtxtcb[task];
            if (ptcb->status != READY)
            {
               ptcb->status |= SUSPFLG; /* mark suspended */
               break;
            }
         }
         ptcb->status |= SUSPFLG; /* mark suspended */

         ptcb->flink->blink = ptcb->blink; /* general task unlink */
         ptcb->blink->flink = ptcb->flink;

         break;
#endif /* } HAS_SUSPEND */

#ifdef HAS_RESUME /* { */
/************************/
      case RTXC_RESUME:
/************************/
         ptcb = (TCB ks_tbl *)&rtxtcb[((struct targ ks_stk *)p2)->task];

         if (ptcb->status != READY)
         {
               /* clear suspended and test for ready now */
             if ( (ptcb->status &= ~SUSPFLG) == READY)
             {
                 ptcb->flink = nsrttcb;
                 nsrttcb = ptcb;
             }
         }
         break;
#endif /* } HAS_RESUME */

#ifdef HAS_PARTITIONS /* { */
#ifdef HAS_ALLOC /* { */
/************************/
      case RTXC_ALLOC:
/************************/
         pph = (PHEADER ks_tbl *)&pheader[((struct parg ks_stk *)p2)->map];

         /* Get 1st free block. If none available, thread waiter */
         if ( (q = (struct xmap *)KS_ISRalloc(((struct parg ks_stk *)p2)->map))
                                       == NULL)
         {
#ifdef PARTITION_WAITERS /* { */
            if ( ((struct parg ks_stk *)p2)->wait)
            {
               ptcb = hipritsk; /* save hipritsk for later use */

               ptcb->status = PARTITION_WAIT;

               porder((TCB ks_tbl *)&pph->waiters);

#ifdef PARTITION_TIMEOUTS /* { */
               if ( ((struct parg ks_stk *)p2)->ticks)
               {
                  ptcb->pclkblk = ((struct parg ks_stk *)p2)->pclkblk;
                  pclkblk = ptcb->pclkblk;
                  pclkblk->remain = ((struct parg ks_stk *)p2)->ticks;
                  pclkblk->recycle = (TICKS)0;

                  /* mark object type */
                  pclkblk->objtype = PARTITION_OBJ;
                  pclkblk->objid = ((struct parg ks_stk *)p2)->map;
                  pclkblk->task = ptcb->task;

                  /* insert timer */
                  insert_timer(pclkblk);

                  ((struct parg ks_stk *)p2)->ksrc = RC_TIMEOUT;
               }
#endif /* } PARTITION_TIMEOUTS */
            }
#endif /* } PARTITION_WAITERS */
         }
         ((struct parg ks_stk *)p2)->val = (char *)q;
         break;
#endif /* } HAS_ALLOC */

#ifdef HAS_FREE /* { */
/************************/
      case RTXC_FREE:
/************************/
         q = (struct xmap *)((struct parg ks_stk *)p2)->val;

         pph = (PHEADER ks_tbl *)&pheader[((struct parg ks_stk *)p2)->map];

#ifdef CBUG /* { */
         pph->usage++; /* increment no. frees */
#endif /* } CBUG */

#ifdef PARTITION_WAITERS /* { */
         if ( (ptcb = pph->waiters) != NULLTCB) /* if any waiters */
         {
            /* pass ptr directly from freeing task to allocating task */
            p2a = ((FRAME ks_stk *)(ptcb->sp))->pksnum;
            ((struct parg ks_stk *)p2a)->val = (char *)q;

            /* remove first waiter from list */
            if ( (pph->waiters = ptcb->flink) != NULLTCB)
               ptcb->flink->blink = (TCB ks_tbl *)&pph->waiters;

            /* "resume" first waiter */
            if ( (ptcb->status &= ~PARTITION_WAIT) == READY)
            {
               /* insert waiter into READY list */
               ptcb->flink = nsrttcb;
               nsrttcb = ptcb;
            }

#ifdef PARTITION_TIMEOUTS /* { */
            /* cleanup any pending timeout */
            if ( (pclkblk = ptcb->pclkblk) != NULLCLK)
            {
               /* mark waiter GOOD */
               ((struct parg ks_stk *)p2a)->ksrc = RC_GOOD;

               unlink_timer(pclkblk);
               ptcb->pclkblk = NULLCLK;
            }
#endif /* } PARTITION_TIMEOUTS */
         }
         else
#endif /* } PARTITION_WAITERS */
         {
            /* put block back on free list */
            PUSHPSWH; /* save current processor priority level */
            DISABLE;
            q->link = pph->next;
            pph->next = q;
            POPPSWH;  /* restore current PPL */

#ifdef CBUG /* { */
            pph->cur--;   /* track current no. used */
#endif /* } CBUG */
         }
         break;
#endif /* } HAS_FREE */

#ifdef HAS_CREATE_PART /* { */
/************************/
      case RTXC_CREATE_PART:
/************************/
         if ( (pph = (PHEADER ks_tbl *)dphfl) != NULLMAP)
         {
            /* allocate pheader by removing 1st from free list */
            dphfl = (PHEADER ks_tbl *)pph->next;

            /* put map number in arg packet */
            ((struct parg ks_stk *)p2)->map = pph->map;

            /* use header to define partition */
            dn_defpart(pph, p2);
         }
         else  /* pheader not available, return 0 */
            ((struct parg ks_stk *)p2)->map = (MAP)0;

         break;
#endif /* } HAS_CREATE_PART */

#ifdef HAS_DEFPART /* { */
/************************/
      case RTXC_DEFPART:
/************************/
         pph = (PHEADER ks_tbl *)&pheader[((struct parg ks_stk *)p2)->map];
         dn_defpart(pph, p2);  /* use header to define partition */

         break;
#endif /* } HAS_DEFPART */

#ifdef HAS_ALLOC_PART /* { */
/************************/
      case RTXC_ALLOC_PART:
/************************/
         if ( (pph = (PHEADER ks_tbl *)dphfl) != NULLMAP)
         {
            /* allocate pheader by removing 1st from free list */
            dphfl = (PHEADER ks_tbl *)pph->next;

            /* put map number in arg packet */
            ((struct parg ks_stk *)p2)->map = pph->map;
         }
         else /* pheader not available, return 0 */
            ((struct parg ks_stk *)p2)->map = (MAP)0;

         break;
#endif /* } HAS_ALLOC_PART */

#ifdef HAS_FREE_PART /* { */
/************************/
      case RTXC_FREE_PART:
/************************/
         pph = (PHEADER ks_tbl *)&pheader[((struct parg ks_stk *)p2)->map];

         /* reset dimension and stats */
         pph->size = 0;
         pph->count = 0;
#ifdef PARTITION_WAITERS /* { */
         pph->waiters = NULLTCB;
         pph->dummy = NULLTCB;
#endif /* } PARTITION_WAITERS */

#ifdef CBUG /* { */
         pph->cur = 0;
         pph->worst = 0;
         pph->usage = 0;
#endif /* } CBUG */

         /* re-insert pheader into pheader free list for dynamic partitions */
         pph->next = (struct xmap *)dphfl;
         dphfl = (PHEADER ks_tbl *)pph;

         ((struct parg ks_stk *)p2)->addr = pph->addr;

         break;
#endif /* } HAS_FREE_PART */

#endif /* } HAS_PARTITIONS */

#ifdef HAS_RESOURCES /* { */
#ifdef HAS_LOCK /* { */
/************************/
      case RTXC_LOCK:
/************************/
         prh = (RHEADER ks_tbl *)&rheader[((struct larg ks_stk *)p2)->resource];

#ifdef CBUG /* { */
         prh->count++; /* update statistics */
#endif /* } CBUG */

         if (prh->owner == NULLTCB)  /* if resource not currently owned */
         {
            prh->owner = hipritsk; /* lock resource with task */
            prh->level = 1;         /* set nesting level = 1 */
            ((struct larg ks_stk *)p2)->ksrc = RC_GOOD;
            break;
         }

         if (prh->owner == hipritsk) /* if nested lock */
         {
            prh->level++; /* bump nesting level only */
                   /* indicate NESTED lock */
            ((struct larg ks_stk *)p2)->ksrc = RC_NESTED;
            break;
         }

#ifdef CBUG /* { */
         prh->conflict++; /* count resource bottlenecks */
#endif /* } CBUG */

#ifdef RESOURCE_WAITERS /* { */
         /* if wait on resource not available */
         if ( ((struct larg ks_stk *)p2)->wait)
         {
#ifdef PRIORITY_INVERSION /* { */
            if (prh->resattr == PRIORITY_INVERSION_ON)
            {
               /* if owner is lower priority than requestor */
               if ( (ptcb = prh->owner)->priority > hipritsk->priority)
               {
                  /* priority inversion is necessary */

                  /* save owner's priority only for first conflict */
                  if (prh->priority == (PRIORITY)0)
                     prh->priority = ptcb->priority;

                  /* but always elevate owner's priority to hipri->priority */
                  ptcb->priority = hipritsk->priority;

                  /* change priority - whatever it takes */
                  if (ptcb->status == READY)
                  {
                     /* unlink owner task from RUN list */
                     ptcb->flink->blink = ptcb->blink;
                     ptcb->blink->flink = ptcb->flink;

                     /* re-insert task */
                     ptcb->flink = nsrttcb;
                     nsrttcb = ptcb;
                  }
                  else
                  {
                     /* if task being changed is in a WAITER list */
                     if (ptcb->status & COMBO_WAIT)
                     {
                        /*
                         * need to re-order within partition, queue, resource,
                         * or mailbox header
                        */
                        reorder_waiters(ptcb);
                     }
                     else
                     {
                        /* task is suspended, terminated or blocked */
                        /* priority change above is all that is needed */
                     }
                  }
               }
            }
#endif /* } PRIORITY_INVERSION */

            ptcb = hipritsk; /* save hipritsk for later use */

            ptcb->status = RESOURCE_WAIT; /* mark task waiting */

            /* thread waiting task in priority order */
            porder((TCB ks_tbl *)&prh->waiters);

#ifdef RESOURCE_TIMEOUTS /* { */
            if ( ((struct larg ks_stk *)p2)->ticks) /* if timeout active */
            {
               ptcb->pclkblk = ((struct larg ks_stk *)p2)->pclkblk;
               pclkblk = ptcb->pclkblk;
               pclkblk->remain = ((struct larg ks_stk *)p2)->ticks;
               pclkblk->recycle = (TICKS)0;

               /* mark object type */
               pclkblk->objtype = RESOURCE_OBJ;
               pclkblk->objid = ((struct larg ks_stk *)p2)->resource;
               pclkblk->task = ptcb->task;

               /* insert timer */
               insert_timer(pclkblk);

               ((struct larg ks_stk *)p2)->ksrc = RC_TIMEOUT;
            }
#endif /* } RESOURCE_TIMEOUTS */
         }
         else
#endif /* } RESOURCE_WAITERS */
         {
                   /* indicate lock failure */
            ((struct larg ks_stk *)p2)->ksrc = RC_BUSY;
         }
         break;
#endif /* } HAS_LOCK */

#ifdef HAS_UNLOCK /* { */
/************************/
      case RTXC_UNLOCK:
/************************/
         prh = (RHEADER ks_tbl *)&rheader[((struct larg ks_stk *)p2)->resource];

         if (prh->owner == NULLTCB) /* if no owner, then NOP */
         {
            ((struct larg ks_stk *)p2)->ksrc = RC_GOOD;
            break;
         }

         if (prh->owner != hipritsk) /* if hipritsk not owner, then NOP */
         {
            ((struct larg ks_stk *)p2)->ksrc = RC_BUSY;
            break;
         }

         if (--prh->level) /* if more nesting remaining, then NOP */
         {
            ((struct larg ks_stk *)p2)->ksrc = RC_NESTED;
            break;
         }

#ifdef RESOURCE_WAITERS /* { */
#ifdef PRIORITY_INVERSION /* { */
         if (prh->resattr == PRIORITY_INVERSION_ON)
         {
            if (prh->priority != (PRIORITY)0)
            {
               /* return owner task to pre-inversion priority
                *
                * note - owner task may have manually changed its priority
                *        unbeknownst to kernel
               */

               /* iff relative priorities are changed then re-prioritize */
               if ( (hipritsk->priority = prh->priority) >
                    hipritsk->flink->priority)
               {
                  ptcb = hipritsk; /* save for later */

                  /* remove self from READY list (ALWAYS first in list) */
                  UNLINK_HIPRITSK();

                  /* re-insert task */
                  ptcb->flink = nsrttcb;
                  nsrttcb = ptcb;
               }

               prh->priority = 0; /* reset priority inversion history flag */
            }
         }
#endif /* } PRIORITY_INVERSION */

         /* if any task is waiting for the resource */
         if ( (ptcb = prh->waiters) != NULLTCB)
         {
            /* remove first waiter from list */
            if ( (prh->waiters = ptcb->flink) != NULLTCB)
               ptcb->flink->blink = (TCB ks_tbl *)&prh->waiters;

                 /* "resume" first waiter */
            if ( (ptcb->status &= ~RESOURCE_WAIT) == READY)
            {
               /* insert waiter into READY list */
               ptcb->flink = nsrttcb;
               nsrttcb = ptcb;
            }

            prh->owner = ptcb; /* mark resource owned */
            prh->level = 1;

#ifdef RESOURCE_TIMEOUTS /* { */
            /* cleanup any pending timeout */
            if ( (pclkblk = ptcb->pclkblk) != NULLCLK)
            {
               p2a = ((FRAME ks_stk *)(ptcb->sp))->pksnum;
               ((struct larg ks_stk *)p2a)->ksrc = RC_GOOD;

               unlink_timer(pclkblk);
               ptcb->pclkblk = NULLCLK;
            }
#endif /* } RESOURCE_TIMEOUTS */
         }
         else
#endif /* } RESOURCE_WAITERS */
         {
            prh->owner = NULLTCB; /* mark resource not owned */
            ((struct larg ks_stk *)p2)->ksrc = RC_GOOD;
         }
         break;
#endif /* } HAS_UNLOCK */

#ifdef HAS_INQRES /* { */
/************************/
      case RTXC_INQRES:
/************************/
         prh = (RHEADER ks_tbl *)&rheader[((struct larg ks_stk *)p2)->resource];

         /* return owner task or 0 if none (0 set in API since faster) */
         if (prh->owner != NULLTCB)
            ((struct larg ks_stk *)p2)->task = prh->owner->task;
         break;
#endif /* } HAS_INQRES */

#ifdef PRIORITY_INVERSION /* { */
#ifdef HAS_DEFRES /* { */
/************************/
      case RTXC_DEFRES:
/************************/
         prh = (RHEADER ks_tbl *)&rheader[((struct larg ks_stk *)p2)->resource];

         /* note: ksrc initialized to RC_GOOD in API */

         if (prh->owner == NULLTCB)
            prh->resattr = ((struct larg ks_stk *)p2)->resattr;
         else
            ((struct larg ks_stk *)p2)->ksrc = RC_BUSY;
         break;
#endif /* } HAS_DEFRES */
#endif /* } PRIORITY_INVERSION */

#endif /* } HAS_RESOURCES */

#ifdef HAS_BLOCK /* { */
/************************/
      case RTXC_BLOCK:
/************************/
         /* handle block world (all tasks) */
         if ( (endtask = ((struct blkarg ks_stk *)p2)->endtask) == SELFTASK)
            endtask = hipritsk->task; /* -1 could block null task 0 */

         if ( (task = ((struct blkarg ks_stk *)p2)->starttask) == SELFTASK)
            task = hipritsk->task; /* +1 could block past last task */

         for (ptcb = (TCB ks_tbl *)&rtxtcb[task]; task <= endtask; task++, ptcb++)
         {
            if (hipritsk->task == task) /* never block self */
               continue;

            if (ptcb->status == READY)
            {
               ptcb->flink->blink = ptcb->blink; /* general unlink */
               ptcb->blink->flink = ptcb->flink;
            }

            ptcb->status |= BLOCK_WAIT;
         }
         break;
#endif /* } HAS_BLOCK */

#ifdef HAS_UNBLOCK /* { */
/************************/
      case RTXC_UNBLOCK:
/************************/
         if ( (endtask = ((struct blkarg ks_stk *)p2)->endtask) == SELFTASK)
            endtask = hipritsk->task; /* -1 could unblock null task */

         if ( (task = ((struct blkarg ks_stk *)p2)->starttask) == SELFTASK)
            task = hipritsk->task; /* +1 could unblock past last task */

         for (ptcb = (TCB ks_tbl *)&rtxtcb[task]; task <= endtask; task++, ptcb++)
         {
            if (ptcb->status == READY)
               continue;

            if ( (ptcb->status &= ~BLOCK_WAIT) == READY)
            {
               ptcb->flink = nsrttcb;
               nsrttcb = ptcb;
            }
         }
         break;
#endif /* } HAS_UNBLOCK */

#ifdef HAS_ALLOC_TIMER /* { */
/************************/
      case RTXC_ALLOC_TIMER:
/************************/
         ((struct clkarg ks_stk *)p2)->pclkblk = get_clkblk();
         break;
#endif /* } HAS_ALLOC_TIMER */

#ifdef HAS_FREE_TIMER /* { */
/************************/
      case RTXC_FREE_TIMER:
/************************/
         pclkblk = ((struct clkarg ks_stk *)p2)->pclkblk;

         if (pclkblk->state == TIMER_ACTIVE)
         {
            unlink_timer(pclkblk);
         }

         pclkblk->flink = clkqfl; /* insert block at front of free list */
         clkqfl = pclkblk;
         break;
#endif /* } HAS_FREE_TIMER */

#ifdef HAS_STOP_TIMER /* { */
/************************/
      case RTXC_STOP_TIMER:
/************************/
         /* fall through to common code */
#endif /* } HAS_STOP_TIMER */
#ifdef HAS_RESTART_TIMER /* { */
/************************/
      case RTXC_RESTART_TIMER:
/************************/
         /* fall through to common code */
#endif /* } HAS_RESTART_TIMER */
#ifdef HAS_START_TIMER /* { */
/************************/
      case RTXC_START_TIMER:
/************************/

         if ( (pclkblk = ((struct clkarg ks_stk *)p2)->pclkblk) == NULLCLK)
         {
            if (*p2 != RTXC_START_TIMER)
            {
#ifdef HAS_RESTART_TIMER /* { */
               /* no clkblk sent for RTXC_STOP_TIMER or RTXC_RESTART_TIMER */
#else /* } HAS_RESTART_TIMER { */
               /* no clkblk sent for RTXC_STOP_TIMER */
#endif /* } HAS_RESTART_TIMER */
               ((struct clkarg ks_stk *)p2)->ksrc = RC_TIMER_ILLEGAL;
               break;
            }
            else /* is RTXC_START_TIMER */
            {
               if ( (pclkblk = get_clkblk()) == NULLCLK)
                  break; /* no timer available, NULLCLK returned */
               else
                  ((struct clkarg ks_stk *)p2)->pclkblk = pclkblk;
            }
         }
         else /* clkblk was sent */
         {
            if (*p2 != RTXC_START_TIMER)
            {
               if (pclkblk->task != hipritsk->task)
               {
#ifdef HAS_RESTART_TIMER /* { */
                  /* attempt to STOP or RESTART someone else's timer */
#else /* } HAS_RESTART_TIMER { */
                  /* attempt to STOP someone else's timer */
#endif /* } HAS_RESTART_TIMER */
                  ((struct clkarg ks_stk *)p2)->ksrc = RC_TIMER_ILLEGAL;
                  break;
               }
            }
         }

         if (pclkblk->state == TIMER_DONE)
         {
            if (*p2 == RTXC_STOP_TIMER)
            {
               ((struct clkarg ks_stk *)p2)->ksrc = RC_TIMER_INACTIVE;
               break;
            }
         }
         else   /* stop active timer */
         {
            unlink_timer(pclkblk);

            if (*p2 == RTXC_STOP_TIMER)
               break;
         }

#ifdef HAS_RESTART_TIMER /* { */
         /* at this point, the function is START_TIMER or RESTART_TIMER */
#else /* } HAS_RESTART_TIMER { */
         /* at this point, the function should be START_TIMER */
#endif /* } HAS_RESTART_TIMER */

         if (*p2 == RTXC_START_TIMER)
         {
            /* if special case of zero initial and recycle times ... */
            /* signal semaphore and leave */
            if ( ((struct clkarg ks_stk *)p2)->ticks == (TICKS)0 &&
                 ((struct clkarg ks_stk *)p2)->period == (TICKS)0 )
            {
               pclkblk->task = hipritsk->task;
               SIGNAL(((struct clkarg ks_stk *)p2)->sema);
               break;
            }

            /* if zero initial time and non-zero recycle time ... */
            /* start timer with recycle time & signal semaphore */
            if ( ((struct clkarg ks_stk *)p2)->ticks == (TICKS)0 &&
                 ((struct clkarg ks_stk *)p2)->period != (TICKS)0 )
            {
               ((struct clkarg ks_stk *)p2)->ticks =
                           ((struct clkarg ks_stk *)p2)->period;
               SIGNAL(((struct clkarg ks_stk *)p2)->sema);
            }
         }
#ifdef HAS_RESTART_TIMER /* { */
         else  /* is RTXC_RESTART_TIMER */
         {
            /* get sema from clkblk & put in arg struct */
            ((struct clkarg ks_stk *)p2)->sema = pclkblk->objid;
         }
#endif /* } HAS_RESTART_TIMER */

         pclkblk->remain = ((struct clkarg ks_stk *)p2)->ticks;
         pclkblk->recycle = ((struct clkarg ks_stk *)p2)->period;

         /* mark object type */
         pclkblk->objtype = TIMER_OBJ;
         pclkblk->task = hipritsk->task;

         /* force Sema pending in case it was already DONE */
         if ( (semat[pclkblk->objid = ((struct clkarg ks_stk *)p2)->sema]
                            == SEMA_DONE)
                                 &&
              ( ((struct clkarg ks_stk *)p2)->period == (TICKS)0) )
            semat[pclkblk->objid] = SEMA_PENDING;

         /* insert timer */
         insert_timer(pclkblk);

         break;
#endif /* } HAS_START_TIMER */

#ifdef HAS_DELAY /* { */
/************************/
      case RTXC_DELAY:
/************************/
         if ( ((task = ((struct delayarg ks_stk *)p2)->task) == SELFTASK)  ||
                (task == hipritsk->task) )  /* delaying self */
         {
            if ( ((struct delayarg ks_stk *)p2)->ticks == 0)
               break;   /* zero time for self = NOP */
            ptcb = hipritsk;
            task = ptcb->task;
            pclkblk = ((struct delayarg ks_stk *)p2)->pclkblk;
         }
         else  /* current task is delaying another task */
         {
            ptcb = (TCB ks_tbl *)&rtxtcb[task];

            if (ptcb->status & DELAY_WAIT)
            {
               /* if task is already delayed ... */
               /* get current clkblk and unlink from active timers */
               pclkblk = ptcb->pclkblk;
               unlink_timer(pclkblk);

               if ( ((struct delayarg ks_stk *)p2)->ticks == 0)
               {
                  /* if 0 delay, clear DELAY_WAIT flag ... */
                  if ( (ptcb->status &= ~DELAY_WAIT) == READY)
                  {
                     /* ... if task is now READY, put in ready list */
                     ptcb->flink = nsrttcb;
                     nsrttcb = ptcb;
                  }
                  break;
               }
            }
            else  /* task was not already delayed */
            {
               /* if 0 ticks (un-delay) or if task not READY, then NOP */
               if ( (((struct delayarg ks_stk *)p2)->ticks == 0) ||
                                       (ptcb->status != READY) )
                  break;

               /* allocate space on task's personal stack for clkblk */
               pclkblk = (CLKBLK ks_clk *)(ptcb->sp) - 1;
            }
         }

         ptcb->pclkblk = pclkblk; /* put pointer to clkblk in tcb */

         if (ptcb->status == READY)
         {
               /* general unlink from tcb READY list */
            ptcb->flink->blink = ptcb->blink;
            ptcb->blink->flink = ptcb->flink;
         }

         ptcb->status |= DELAY_WAIT;

         pclkblk->remain = ((struct delayarg ks_stk *)p2)->ticks;
         pclkblk->recycle = (TICKS)0;

         /* mark object type */
         pclkblk->objtype = DELAY_OBJ;
         pclkblk->objid = task;
         pclkblk->task = task;

         /* insert timer */
         insert_timer(pclkblk);
         break;
#endif /* } HAS_DELAY */

#ifdef HAS_ELAPSE /* { */
/************************/
      case RTXC_ELAPSE:
/************************/
         PUSHPSWH; /* save current processor priority level */
         DISABLE;
         tcnt = rtctick; /* snapshot rtctick */
         POPPSWH;  /* restore current PPL */
         ((struct etarg ks_stk *)p2)->val = tcnt -
                                *(((struct etarg ks_stk *)p2)->stamp);
         *(((struct etarg ks_stk *)p2)->stamp) = tcnt;
         break;
#endif /* } HAS_ELAPSE */

#ifdef HAS_INQTIMER /* { */
/************************/
      case RTXC_INQTIMER:
/************************/
         pclkblk = ((struct clkarg ks_stk *)p2)->pclkblk;

         if (pclkblk->state == TIMER_ACTIVE)
         {
            PUSHPSWH; /* save current processor priority level */
            DISABLE; /* prevent changing rtctick while reading */
            ((struct clkarg ks_stk *)p2)->ticks = pclkblk->remain - rtctick;
            POPPSWH;
         }
         else
            ((struct clkarg ks_stk *)p2)->ticks = (TICKS)0;
         break;
#endif /* } HAS_INQTIMER */

#ifdef HAS_INQTIME /* { */
/************************/
      case RTXC_INQTIME:
/************************/
         PUSHPSWH; /* save current processor priority level */
         DISABLE; /* prevent changing while reading */
         ((struct timearg ks_stk *)p2)->time = rtctime;
	 rtctimetick=ratecnt;
         POPPSWH;
         break;
#endif /* } HAS_INQTIME */

#ifdef HAS_DEFTIME /* { */
/************************/
      case RTXC_DEFTIME:
/************************/
         PUSHPSWH; /* save current processor priority level */
         DISABLE; /* prevent changing while writing */
         rtctime = ((struct timearg ks_stk *)p2)->time;
	 ratecnt=0;
         POPPSWH;
         break;
#endif /* } HAS_DEFTIME */

#ifdef HAS_MAILBOXES /* { */
#ifdef HAS_SEND /* { */
/************************/
      case RTXC_SEND:
/************************/
         if ( ((struct msgarg ks_stk *)p2)->priority == (PRIORITY)0)
         {
            /* priority = 0 yields sender's priority */
            ((struct msgarg ks_stk *)p2)->prtxcmsg->priority =
               hipritsk->priority;
         }
         else
         {
            ((struct msgarg ks_stk *)p2)->prtxcmsg->priority =
               ((struct msgarg ks_stk *)p2)->priority;
         }

         ((struct msgarg ks_stk *)p2)->prtxcmsg->sema =
            ((struct msgarg ks_stk *)p2)->sema;

         ((struct msgarg ks_stk *)p2)->prtxcmsg->task = hipritsk->task;

         pmh = (MHEADER ks_tbl *)&mheader[((struct msgarg ks_stk *)p2)->mbox];

#ifdef MAILBOX_SEMAS /* { */
         /* if was empty and there is a not_empty semaphore defined */
         if (pmh->link == NULL)
         {
            UPDATE_SEMA(pmh->nesema);
         }
#endif /* } MAILBOX_SEMAS */

#ifdef MAILBOX_WAITERS /* { */
         if ( (ptcb = pmh->waiters) != NULLTCB) /* if a task waiting */
         {
            /* pass data directly from sender to receiver */
            p2a = ((FRAME ks_stk *)(ptcb->sp))->pksnum;

            /* if receiving from anybody (0) or a specific task match */
            if ( (((struct msgarg ks_stk *)p2a)->task == (TASK)0) ||
                 (((struct msgarg ks_stk *)p2a)->task == hipritsk->task) )
            {
               /* pass &message directly to receiver */
               ((struct msgarg ks_stk *)p2a)->prtxcmsg =
                  ((struct msgarg ks_stk *)p2)->prtxcmsg;

#ifdef CBUG /* { */
               pmh->count++;
#endif /* } CBUG */

               /* remove tcb from waiter's list */
               if ( (pmh->waiters = ptcb->flink) != NULLTCB)
                  ptcb->flink->blink = (TCB ks_tbl *)&pmh->waiters;

               /* clear MSG WAIT in receiving task and check if runnable */
               if ( (ptcb->status &= ~MSG_WAIT) == READY)
               {
                  /* insert waiter into READY list */
                  ptcb->flink = nsrttcb;
                  nsrttcb = ptcb;
               }

#ifdef MAILBOX_TIMEOUTS /* { */
               /* cleanup any pending timeout */
               if ( (pclkblk = ptcb->pclkblk) != NULLCLK)
               {
                  /* mark waiter GOOD */
                  ((struct msgarg ks_stk *)p2a)->ksrc = RC_GOOD;

                  unlink_timer(pclkblk);
                  ptcb->pclkblk = NULLCLK;
               }
#endif /* } MAILBOX_TIMEOUTS */
            }
            else
            {
               insert_message(pmh, ((struct msgarg ks_stk *)p2)->prtxcmsg);
            }
         }
         else
#endif /* } MAILBOX_WAITERS */
         {
            insert_message(pmh, ((struct msgarg ks_stk *)p2)->prtxcmsg);
         }

         sema = ((struct msgarg ks_stk *)p2)->sema;

#ifdef MAILBOX_WAITERS /* { */
         /* if need to wait on response (sema) */
         if ( ((struct msgarg ks_stk *)p2)->wait)
         {
            ptcb = hipritsk; /* save for later */

            semat[sema] = (SSTATE)ptcb->task;

            ptcb->status = SEMAPHORE_WAIT;

            UNLINK_HIPRITSK(); /* unlink first tcb */

#ifdef SEMAPHORE_TIMEOUTS /* { */
            if ( ((struct msgarg ks_stk *)p2)->ticks) /* if timeout active */
            {
               ptcb->pclkblk = ((struct msgarg ks_stk *)p2)->pclkblk;
               pclkblk = ptcb->pclkblk;
               pclkblk->remain = ((struct msgarg ks_stk *)p2)->ticks;
               pclkblk->recycle = (TICKS)0;

               /* mark object type */
               pclkblk->objtype = SEMAPHORE_OBJ;
               pclkblk->objid = sema;
               pclkblk->task = ptcb->task;

               /* insert timer */
               insert_timer(pclkblk);

               ((struct msgarg ks_stk *)p2)->ksrc = RC_TIMEOUT;
            }
#endif /* } SEMAPHORE_TIMEOUTS */
         }
         else
#endif /* } MAILBOX_WAITERS */
         {
            if (sema != NULLSEMA)
               semat[sema] = SEMA_PENDING;
         }
         break;
#endif /* } HAS_SEND */

#ifdef HAS_RECEIVE /* { */
/************************/
      case RTXC_RECEIVE:
/************************/
         pmh = (MHEADER ks_tbl *)&mheader[((struct msgarg ks_stk *)p2)->mbox];

         /* if no messages are available */
         if ( (prtxcmsg = remove_message(pmh,
                            ((struct msgarg ks_stk *)p2)->task)) == NULL)
         {
#ifdef MAILBOX_WAITERS /* { */
            if ( ((struct msgarg ks_stk *)p2)->wait)
            {
               ptcb = hipritsk; /* save for later */

               ptcb->status = MSG_WAIT; /* set MSG_WAIT in status word */

               /* thread waiting task in priority order */
               porder((TCB ks_tbl *)&pmh->waiters);

#ifdef MAILBOX_TIMEOUTS /* { */
               if ( ((struct msgarg ks_stk *)p2)->ticks) /* if timeout active*/
               {
                  ptcb->pclkblk = ((struct msgarg ks_stk *)p2)->pclkblk;
                  pclkblk = ptcb->pclkblk;
                  pclkblk->remain = ((struct msgarg ks_stk *)p2)->ticks;
                  pclkblk->recycle = (TICKS)0;

                  /* mark object type */
                  pclkblk->objtype = MAILBOX_OBJ;
                  pclkblk->objid = ((struct msgarg ks_stk *)p2)->mbox;
                  pclkblk->task = ptcb->task;

                  /* insert timer */
                  insert_timer(pclkblk);

                  ((struct msgarg ks_stk *)p2)->ksrc = RC_TIMEOUT;
               }
#endif /* } MAILBOX_TIMEOUTS */
            }
#endif /* } MAILBOX_WAITERS */
         }
         else /* message returned to caller */
         {
#ifdef CBUG /* { */
            pmh->count++;
#endif /* } CBUG */

#ifdef MAILBOX_SEMAS /* { */
            if (pmh->nesema != NULLSEMA)
            {
               /* if now empty */
               if (pmh->link == NULL)
                  semat[pmh->nesema] = SEMA_PENDING; /* reset */
               else
                  semat[pmh->nesema] = SEMA_DONE; /* force not empty */
            }
#endif /* } MAILBOX_SEMAS */
         }

         /* return msg to caller */
         ((struct msgarg ks_stk *)p2)->prtxcmsg = prtxcmsg;
         break;
#endif /* } HAS_RECEIVE */
#endif /* } HAS_MAILBOXES */

#ifdef HAS_QUEUES /* { */
#ifdef HAS_ENQUEUE /* { */
/************************/
      case RTXC_ENQUEUE:
/************************/
         pqh = (QHEADER ks_tbl *)&qheader[((struct qarg ks_stk *)p2)->queue];

         if ( (depth = pqh->depth) == pqh->curndx) /* queue was already full */
         {
#ifdef ENQUEUE_WAITERS /* { */
            if ( ((struct qarg ks_stk *)p2)->wait)
            {
               ptcb = hipritsk; /* save hipritsk for later */

               ptcb->status = QUEUE_WAIT; /* set QUEUE_WAIT in status */

               /* thread waiting task in priority order */
               porder((TCB ks_tbl *)&pqh->waiters);

#ifdef ENQUEUE_TIMEOUTS /* { */
               if ( ((struct qarg ks_stk *)p2)->ticks) /* if timeout active */
               {
                  ptcb->pclkblk = ((struct qarg ks_stk *)p2)->pclkblk;
                  pclkblk = ptcb->pclkblk;
                  pclkblk->remain = ((struct qarg ks_stk *)p2)->ticks;
                  pclkblk->recycle = (TICKS)0;

                  /* mark object type */
                  pclkblk->objtype = QUEUE_OBJ;
                  pclkblk->objid = ((struct qarg ks_stk *)p2)->queue;
                  pclkblk->task = ptcb->task;

                  /* insert timer */
                  insert_timer(pclkblk);
               }
#endif /* } ENQUEUE_TIMEOUTS */
            }
#endif /* } ENQUEUE_WAITERS */
            break;
         }

         ((struct qarg ks_stk *)p2)->ksrc = RC_GOOD;

         width = pqh->width;

#ifdef DEQUEUE_WAITERS /* { */
         /* if queue is empty && if there are waiters */
         if ( (pqh->curndx == 0) && ((ptcb = pqh->waiters) != NULLTCB) )
         {
            /* move data directly to first waiter */

            /* remove highest priority waiter (1st) */
            if ( (pqh->waiters = ptcb->flink) != NULLTCB)
                 ptcb->flink->blink = (TCB ks_tbl *)&pqh->waiters;

            if ( (ptcb->status &= ~QUEUE_WAIT) == READY)
            {
               ptcb->flink = nsrttcb;
               nsrttcb = ptcb;
            }

#ifdef DEQUEUE_TIMEOUTS /* { */
            /* cleanup any pending timeout */
            if ( (pclkblk = ptcb->pclkblk) != NULLCLK)
            {
               unlink_timer(pclkblk);
               ptcb->pclkblk = NULLCLK;
            }
#endif /* } DEQUEUE_TIMEOUTS */

            p2a = ((FRAME ks_stk *)(ptcb->sp))->pksnum;

            ((struct qarg ks_stk *)p2a)->ksrc = RC_GOOD;

#ifdef QUEUE_MEMCPY /* { */
            if (width)
               memcpy( ((struct qarg ks_stk *)p2a)->data,
                       ((struct qarg ks_stk *)p2)->data,
                       width);
#else /* } QUEUE_MEMCPY { */
            switch(width)
            {
               case 0:
                  break;

               case sizeof(char):
                  *(char *)(((struct qarg ks_stk *)p2a)->data) =
                              *(char *)(((struct qarg ks_stk *)p2)->data);
                  break;

               case sizeof(short):
                  *(short *)(((struct qarg ks_stk *)p2a)->data) =
                              *(short *)(((struct qarg ks_stk *)p2)->data);
                  break;

               case sizeof(long):
                  *(long *)(((struct qarg ks_stk *)p2a)->data) =
                              *(long *)(((struct qarg ks_stk *)p2)->data);
                  break;

               default:
                  memcpy( ((struct qarg ks_stk *)p2a)->data,
                          ((struct qarg ks_stk *)p2)->data,
                          width);
                  break;
            }
#endif /* } QUEUE_MEMCPY */

#ifdef CBUG /* { */
            pqh->count++; /* increment total no. of enqueues to date */
#endif /* } CBUG */

#ifdef QUEUE_SEMAS /* { */
            /* queue was empty - process not empty sema for consumer */
            UPDATE_SEMA(pqh->nesema);

            /* queue was empty - process not full sema for producer */
            if (pqh->nfsema != NULLSEMA)
                  semat[pqh->nfsema] = SEMA_DONE;
#endif /* } QUEUE_SEMAS */
            break;
         }
#endif /* } DEQUEUE_WAITERS */

         /* move data into queue */
         pqh->curndx++;
         if (++pqh->putndx == depth)
            pqh->putndx = 0;  /* recycle index */

         qindex = pqh->putndx; /* local for speed */

#ifdef QUEUE_MEMCPY /* { */
         if (width)
            ks_memcpyFN(pqh->base + (qindex * width),
                   ((struct qarg ks_stk *)p2)->data, width);
#else /* } QUEUE_MEMCPY { */
         switch(width)
         {
            case 0:
               break;

            case sizeof(char):
               *(char ks_queue *)(pqh->base + qindex) =
                                  *(char *)(((struct qarg ks_stk *)p2)->data);
               break;

            case sizeof(short):
               *(short ks_queue *)(pqh->base + (qindex << 1)) =
                                  *(short *)(((struct qarg ks_stk *)p2)->data);
               break;
            case sizeof(long):
               *(long ks_queue *)(pqh->base + (qindex << 2)) =
                                  *(long *)(((struct qarg ks_stk *)p2)->data);
               break;

            default:
               ks_memcpyFN(pqh->base + (qindex * width),
                       ((struct qarg ks_stk *)p2)->data,
                       width);
               break;
         }
#endif /* } QUEUE_MEMCPY */

#ifdef CBUG /* { */
         pqh->count++; /* increment total no. of enqueues to date */
         if (pqh->curndx > pqh->worst) /* check for new worst case full condition */
            pqh->worst = pqh->curndx;
#endif /* } CBUG */

#ifdef QUEUE_SEMAS /* { */
         if (pqh->curndx == depth)               /* queue is now full */
         {
            if (pqh->nfsema != NULLSEMA)
               semat[pqh->nfsema] = SEMA_PENDING; /*set not full sema pending*/

            UPDATE_SEMA(pqh->fsema);
         }
         else                               /* still room in queue */
         {
            if (pqh->nfsema != NULLSEMA)
               semat[pqh->nfsema] = SEMA_DONE;
         }

         if (pqh->curndx == 1) /* if was empty */
         {
            if (pqh->esema != NULLSEMA)
               semat[pqh->esema] = SEMA_PENDING;

            UPDATE_SEMA(pqh->nesema);
         }
#endif /* } QUEUE_SEMAS */
         break;
#endif /* } HAS_ENQUEUE */

#ifdef HAS_DEQUEUE /* { */
/************************/
      case RTXC_DEQUEUE:
/************************/
         pqh = (QHEADER ks_tbl *)&qheader[((struct qarg ks_stk *)p2)->queue];

         if (pqh->curndx == 0)  /* empty queue */
         {
#ifdef DEQUEUE_WAITERS /* { */
            if ( ((struct qarg ks_stk *)p2)->wait)
            {
               ptcb = hipritsk; /* save copy of hipritsk */

               ptcb->status = QUEUE_WAIT; /* set QUEUE_WAIT in status */

               /* thread waiting task in priority order */
               porder((TCB ks_tbl *)&pqh->waiters);

#ifdef DEQUEUE_TIMEOUTS /* { */
               if ( ((struct qarg ks_stk *)p2)->ticks) /* if timeout active */
               {
                  ptcb->pclkblk = ((struct qarg ks_stk *)p2)->pclkblk;
                  pclkblk = ptcb->pclkblk;
                  pclkblk->remain = ((struct qarg ks_stk *)p2)->ticks;
                  pclkblk->recycle = (TICKS)0;

                  /* mark object type */
                  pclkblk->objtype = QUEUE_OBJ;
                  pclkblk->objid = ((struct qarg ks_stk *)p2)->queue;
                  pclkblk->task = ptcb->task;

                  /* insert timer */
                  insert_timer(pclkblk);
               }

#endif /* } DEQUEUE_TIMEOUTS */
            }

#endif /* } DEQUEUE_WAITERS */
            break;
         }

         /* queue not empty, get entry */

         ((struct qarg ks_stk *)p2)->ksrc = RC_GOOD;

         qindex = pqh->putndx - --pqh->curndx;
         if (qindex < 0)
            qindex += pqh->depth;  /* wrap around */

#ifdef QUEUE_MEMCPY /* { */
         if ( (width = pqh->width) != 0)
            ks_memcpyNF( ((struct qarg ks_stk *)p2)->data,
                    pqh->base + (qindex * width),
                    width);
#else /* } QUEUE_MEMCPY { */
         switch(width = pqh->width)
         {
            case 0:
               break;

            case sizeof(char):
               *(char *)(((struct qarg ks_stk *)p2)->data) =
                               *(char ks_queue *)(pqh->base + qindex);
               break;

            case sizeof(short):
               *(short *)(((struct qarg ks_stk *)p2)->data) =
                               *(short ks_queue *)(pqh->base + (qindex << 1));
               break;

            case sizeof(long):
               *(long *)(((struct qarg ks_stk *)p2)->data) =
                               *(long ks_queue *)(pqh->base + (qindex << 2));
               break;

            default:
               ks_memcpyNF( ((struct qarg ks_stk *)p2)->data,
                       pqh->base + (qindex * width),
                       width);
               break;
         }
#endif /* } QUEUE_MEMCPY */

#ifdef QUEUE_SEMAS /* { */
         if (pqh->curndx == 0)  /* just dequeued last item (now empty) */
         {
            if (pqh->nesema != NULLSEMA)
               semat[pqh->nesema] = SEMA_PENDING; /* pend not empty sema */

            UPDATE_SEMA(pqh->esema);
         }
         else /* there are still more entries in queue */
         {
            if (pqh->nesema != NULLSEMA)
               semat[pqh->nesema] = SEMA_DONE; /* set not empty sema done */
         }
#endif /* } QUEUE_SEMAS */

#ifdef ENQUEUE_WAITERS /* { */
         if (pqh->curndx == pqh->depth - 1)  /* queue was full before deq */
         {
            /* insert first waiter into insert list */
            if ( (ptcb = pqh->waiters) != NULLTCB)
            {
               /* remove highest priority waiter */
               if ( (pqh->waiters = ptcb->flink) != NULLTCB)
                  ptcb->flink->blink = (TCB ks_tbl *)&pqh->waiters;

               if ( (ptcb->status &= ~QUEUE_WAIT) == READY)
               {
                  ptcb->flink = nsrttcb;
                  nsrttcb = ptcb;
               }

#ifdef ENQUEUE_TIMEOUTS /* { */
               /* cleanup any pending timeout */
               if ( (pclkblk = ptcb->pclkblk) != NULLCLK)
               {
                  unlink_timer(pclkblk);
                  ptcb->pclkblk = NULLCLK;
               }
#endif /* } ENQUEUE_TIMEOUTS */

               /* perform enqueue for waiter */

               pqh->curndx++;
               if (++pqh->putndx == pqh->depth)
                  pqh->putndx = 0;  /* recycle index */

               qindex = pqh->putndx; /* local for speed */

               p2a = ((FRAME ks_stk *)(ptcb->sp))->pksnum;

               ((struct qarg ks_stk *)p2a)->ksrc = RC_GOOD;

#ifdef QUEUE_MEMCPY /* { */
               if (width)
                  ks_memcpyFN(pqh->base + (qindex * width),
                         ((struct qarg ks_stk *)p2a)->data,
                         width);
#else /* } QUEUE_MEMCPY { */
               switch(width)
               {
                  case 0:
                     break;

                  case sizeof(char):
                     *(char  ks_queue *)(pqh->base + qindex) =
                                 *(char *)(((struct qarg ks_stk *)p2a)->data);
                     break;

                  case sizeof(short):
                     *(short  ks_queue *)(pqh->base + (qindex << 1)) =
                                 *(short *)(((struct qarg ks_stk *)p2a)->data);
                     break;

                  case sizeof(long):
                     *(long  ks_queue *)(pqh->base + (qindex << 2)) =
                                 *(long *)(((struct qarg ks_stk *)p2a)->data);
                     break;

                  default:
                     ks_memcpyFN(pqh->base + (qindex * width),
                             ((struct qarg ks_stk *)p2a)->data,
                             width);
                    break;
               }
#endif /* } QUEUE_MEMCPY */

#ifdef CBUG /* { */
               pqh->count++; /* increment total no. of enqueues to date */
               /* already past worst case */
#endif /* } CBUG */

#ifdef QUEUE_SEMAS /* { */
               if (pqh->curndx == pqh->depth) /* queue is now full */
               {
                 if (pqh->nfsema != NULLSEMA)
                    semat[pqh->nfsema] = SEMA_PENDING; /*pend not full sema*/

                 UPDATE_SEMA(pqh->fsema);
               }
               else                               /* still room in queue */
               {
                  if (pqh->nfsema != NULLSEMA)
                     semat[pqh->nfsema] = SEMA_DONE;
               }
#endif /* } QUEUE_SEMAS */

#ifdef QUEUE_SEMAS /* { */
               if (pqh->curndx == 1) /* if was empty */
               {
                  UPDATE_SEMA(pqh->nesema);
               }
#endif /* } QUEUE_SEMAS */
            }
            else
            {
#ifdef QUEUE_SEMAS /* { */
               if (pqh->fsema != NULLSEMA)
                  semat[pqh->fsema] = SEMA_PENDING;

               UPDATE_SEMA(pqh->nfsema);

#endif /* } QUEUE_SEMAS */
            }
         }
         else
#endif /* } ENQUEUE_WAITERS */
         {
#ifdef QUEUE_SEMAS /* { */

            UPDATE_SEMA(pqh->nfsema);

#endif /* } QUEUE_SEMAS */
         }
         break;
#endif /* } HAS_DEQUEUE */
#endif /* } HAS_QUEUES */

#ifdef HAS_DEFQUEUE /* { */
/************************/
      case RTXC_DEFQUEUE:
/************************/
         pqh = (QHEADER ks_tbl *)&qheader[((struct qdefarg ks_stk *)p2)->queue];

         pqh->base   = ((struct qdefarg ks_stk *)p2)->base;
         pqh->width  = ((struct qdefarg ks_stk *)p2)->width;
         pqh->depth  = ((struct qdefarg ks_stk *)p2)->depth;
         cursz = ((struct qdefarg ks_stk *)p2)->current_size;

         if ( (cursz < 0) || (cursz > pqh->depth) )
            ((struct qdefarg ks_stk *)p2)->ksrc = RC_ILLEGAL_QUEUE_SIZE;

         else if (cursz == 0)
         {
            pqh->putndx = pqh->depth - 1; /* purge queue */
            pqh->curndx = 0;
         }

         else
         {
            pqh->curndx = cursz;
            pqh->putndx = cursz - 1;
         }
         break;
#endif /* } HAS_DEFQUEUE */

#ifdef QUEUE_SEMAS /* { */
#ifdef HAS_DEFQSEMA /* { */
/************************/
      case RTXC_DEFQSEMA:
/************************/
         pqh = (QHEADER ks_tbl *)&qheader[((struct qdefarg ks_stk *)p2)->queue];

         switch(((struct qdefarg ks_stk *)p2)->qcond)
         {
            /* note: semaphore is 0 == undefqsema() and harmless */
            case QNE:
               pqh->nesema = ((struct qdefarg ks_stk *)p2)->sema;
               if (pqh->curndx != 0) /* if not empty */
                  semat[pqh->nesema] = SEMA_DONE;
               else
                  semat[pqh->nesema] = SEMA_PENDING;
               break;

            case QNF:
               pqh->nfsema = ((struct qdefarg ks_stk *)p2)->sema;
               if (pqh->curndx != pqh->depth) /* if not full */
                  semat[pqh->nfsema] = SEMA_DONE;
               else
                  semat[pqh->nfsema] = SEMA_PENDING;
               break;

            case QE:
               pqh->esema = ((struct qdefarg ks_stk *)p2)->sema;
               if (pqh->curndx == 0) /* if empty */
                  semat[pqh->esema] = SEMA_DONE;
               else
                  semat[pqh->esema] = SEMA_PENDING;
               break;

            case QF:
               pqh->fsema = ((struct qdefarg ks_stk *)p2)->sema;
               if (pqh->curndx == pqh->depth) /* if full */
                  semat[pqh->fsema] = SEMA_DONE;
               else
                  semat[pqh->fsema] = SEMA_PENDING;
               break;
         }
         break;
#endif /* } HAS_DEFQSEMA */
#endif /* } QUEUE_SEMAS */

#ifdef MAILBOX_SEMAS /* { */
#ifdef HAS_DEFMBOXSEMA /* { */
/************************/
      case RTXC_DEFMBOXSEMA:
/************************/
         pmh = (MHEADER ks_tbl *)&mheader[((struct msgarg ks_stk *)p2)->mbox];

         /* note: semaphore is 0 == undefmboxsema() and harmless */
         pmh->nesema = ((struct msgarg ks_stk *)p2)->sema;

         if (pmh->link == NULL)
             semat[pmh->nesema] = SEMA_PENDING;
         else
             semat[pmh->nesema] = SEMA_DONE;
         break;
#endif /* } HAS_DEFMBOXSEMA */
#endif /* } MAILBOX_SEMAS */

#ifdef HAS_PURGEQUEUE /* { */
/************************/
      case RTXC_PURGEQUEUE:
/************************/
         pqh = (QHEADER ks_tbl *)&qheader[((struct qarg ks_stk *)p2)->queue];

         pqh->putndx = pqh->depth - 1;
         qindex = pqh->curndx; /* save original size for later use */
         pqh->curndx = 0;

#ifdef QUEUE_SEMAS /* { */
         UPDATE_SEMA(pqh->esema);

#endif /* } QUEUE_SEMAS */

         depth = pqh->depth;

         /* if queue was full */
         if (qindex == depth)
         {
#ifdef QUEUE_SEMAS /* { */
            UPDATE_SEMA(pqh->nfsema);

#endif /* } QUEUE_SEMAS */

            width = pqh->width;

#ifdef QUEUE_WAITERS /* { */
            /* might have enq or deq waiters to process */
            while ( (ptcb = pqh->waiters) != NULLTCB)
            {
               /* remove highest priority waiter (1st) */
               if ( (pqh->waiters = ptcb->flink) != NULLTCB)
                  ptcb->flink->blink = (TCB ks_tbl *)&pqh->waiters;

               if ( (ptcb->status &= ~QUEUE_WAIT) == READY)
               {
                  ptcb->flink = nsrttcb;
                  nsrttcb = ptcb;
               }

#ifdef QUEUE_TIMEOUTS /* { */
               /* cleanup any pending timeout */
               if ( (pclkblk = ptcb->pclkblk) != NULLCLK)
               {
                  unlink_timer(pclkblk);
                  ptcb->pclkblk = NULLCLK;
               }
#endif /* } QUEUE_TIMEOUTS */

               p2a = ((FRAME ks_stk *)(ptcb->sp))->pksnum;

               ((struct qarg ks_stk *)p2a)->ksrc = RC_GOOD;

               /* move data into queue */
               pqh->curndx++;
               if (++pqh->putndx == depth)
                  pqh->putndx = 0;  /* recycle index */

               qindex = pqh->putndx;

#ifdef QUEUE_MEMCPY /* { */
               if (width)
                  ks_memcpyFN(pqh->base + (qindex * width),
                         ((struct qarg ks_stk *)p2a)->data,
                         width);
#else /* } QUEUE_MEMCPY { */
               switch(width)
               {
                  case 0:
                     break;

                  case sizeof(char):
                     *(char ks_queue *)(pqh->base + qindex) =
                               *(char *)(((struct qarg ks_stk *)p2a)->data);
                     break;

                  case sizeof(short):
                     *(short ks_queue *)(pqh->base + (qindex << 1)) =
                               *(short *)(((struct qarg ks_stk *)p2a)->data);
                     break;

                  case sizeof(long):
                     *(long ks_queue *)(pqh->base + (qindex << 2)) =
                               *(long *)(((struct qarg ks_stk *)p2a)->data);
                     break;

                  default:
                     ks_memcpyFN(pqh->base + (qindex * width),
                           ((struct qarg ks_stk *)p2a)->data,
                           width);
                     break;
               }
#endif /* } QUEUE_MEMCPY */

#ifdef CBUG /* { */
               pqh->count++; /* increment total no. of enqueues to date */
               if (pqh->curndx > pqh->worst) /* check for new worst case */
                  pqh->worst = pqh->curndx;
#endif /* } CBUG */

#ifdef QUEUE_SEMAS /* { */
               /* queue was empty - process not empty sema for consumer */
               if ( pqh->curndx == 1)
               {
                  UPDATE_SEMA(pqh->nesema);
               }

#endif /* } QUEUE_SEMAS */

            } /* end of while */
#endif /* } QUEUE_WAITERS */

         }
         break;
#endif /* } HAS_PURGEQUEUE */

/************************/
      case RTXC_EXECUTE:
/************************/
         ptcb = (TCB ks_tbl *)&rtxtcb[task = ((struct targ ks_stk *)p2)->task];

         /* if task is already executing, then stop and restart */
         if (ptcb->status == READY)
         {
            ptcb->flink->blink = ptcb->blink; /* general unlink */
            ptcb->blink->flink = ptcb->flink;
         }

         /* initialize task stack pointer */
         frame = ptcb->sp = (FRAME ks_stk *)(ptcb->stackbase +
                                   ptcb->stacksize - sizeof(FRAME));

         frame->pc = ptcb->pc_t0; /* initialize program counter */

         frame->psw = 0x8000; /* init status register at task startup */

#if _MODEL == 'l'
         frame->ssel = 0x7f;
#else
         frame->ssel = 0x00;
#endif /* } _MODEL */

         frame->ds = 0x00;
         frame->es = 0x00;
         frame->cs = 0x00;

#ifdef TIME_SLICE /* { */
         ptcb->tslice = (TSLICE)0;
         ptcb->newslice = (TSLICE)0;
#endif /* } TIME_SLICE */

#ifdef SYNC_START /* { */
         if (hipritsk->task == 0)
            ptcb->status = BLOCK_WAIT; /* mark task blocked */
         else
            ptcb->status = READY; /* mark task runnable */
#else /* } SYNC_START { */
         ptcb->status = READY; /* mark task runnable */
#endif /* } SYNC_START */

#ifdef FPU /* { */
          /* if fpregs area allocated */
         if ( (fpregs = rtxtcb[task].fpregs) != NULLFPREGS)
         {
                  /* init fpu to known state */

            /* processor dependent fpu initialization goes here */
                       /* fpregs->? = ?; */

            ptcb->fpumode = 1;         /* mark task as fpu user */
         }
#ifdef BSS_NOT_ZERO /* { */
         else
         {
            ptcb->fpumode = 0;         /* mark task as non fpu user */
         }
#endif /* } BSS_NOT_ZERO */
#endif /* } FPU */

#ifdef SYNC_START /* { */
         if (ptcb->status == READY)
         {
            /* insert new task into list for later */
            ptcb->flink = nsrttcb;
            nsrttcb = ptcb;
         }
#else /* } SYNC_START { */
         /* insert new task into list for later */
         ptcb->flink = nsrttcb;
         nsrttcb = ptcb;
#endif /* } SYNC_START */

         break;

#ifdef HAS_DEFTASK /* { */
/************************/
      case RTXC_DEFTASK:
/************************/
         if ( (task = ((struct deftaskarg ks_stk *)p2)->task) == SELFTASK)
         {
            ((struct deftaskarg ks_stk *)p2)->ksrc = RC_ILLEGAL_TASK;
            break;
         }

         ptcb = (TCB ks_tbl *)&rtxtcb[task];

         /* if task is "active", then abort deftask operation */
         if ( (ptcb->status & INACTIVE) != INACTIVE)
         {
            ((struct deftaskarg ks_stk *)p2)->ksrc = RC_ACTIVE_TASK;
            break;
         }

         ptcb->priority =  ((struct deftaskarg ks_stk *)p2)->priority;

         if ( ((struct deftaskarg ks_stk *)p2)->stackbase) /* new stack */
         {
            ptcb->stackbase = ((struct deftaskarg ks_stk *)p2)->stackbase;
            ptcb->stacksize = ((struct deftaskarg ks_stk *)p2)->stacksize;
         }

#if _MODEL == 'm' || _MODEL == 'l'
         pc1 = (unsigned long)((struct deftaskarg ks_stk *)p2)->entry;
         ptcb->pc_t0 = (void (*)(void))swap_pc_t0(pc1);
#else
         ptcb->pc_t0 = ((struct deftaskarg ks_stk *)p2)->entry;
#endif /* } _MODEL */

         ptcb->pclkblk = NULLCLK;

#ifdef HAS_INQTASK_ARG /* { */
         ptcb->arg = NULLARG;
#endif /* } HAS_INQTASK_ARG */

#ifdef TIME_SLICE /* { */
         ptcb->tslice = (TSLICE)0;
         ptcb->newslice = (TSLICE)0;
#endif /* } TIME_SLICE */

         break;
#endif /* } HAS_DEFTASK */

#ifdef HAS_INQTASK_ARG /* { */
/************************/
      case RTXC_INQTASK_ARG:
/************************/
         if ( (task = ((struct deftaskarg ks_stk *)p2)->task) == SELFTASK)
            task = hipritsk->task;
         ((struct deftaskarg ks_stk *)p2)->arg = rtxtcb[task].arg;
         break;
#endif /* } HAS_INQTASK_ARG */

#ifdef HAS_DEFTASK_ARG /* { */
/************************/
      case RTXC_DEFTASK_ARG:
/************************/
         if ( (task = ((struct deftaskarg ks_stk *)p2)->task) == SELFTASK)
            task = hipritsk->task;
         rtxtcb[task].arg = ((struct deftaskarg ks_stk *)p2)->arg;
         break;
#endif /* } HAS_DEFTASK_ARG */

#ifdef HAS_ALLOC_TASK /* { */
/************************/
      case RTXC_ALLOC_TASK:
/************************/
         if ( (ptcb = dtcbfl) != NULLTCB) /* allocate tcb */
         {
            dtcbfl = ptcb->flink; /* by removing 1st from free list */
            ((struct targ ks_stk *)p2)->task = ptcb->task;
         }
         else
            ((struct targ ks_stk *)p2)->task = (TASK)0;
         break;
#endif /* } HAS_ALLOC_TASK */

#ifdef HAS_TERMINATE /* { */
/************************/
      case RTXC_TERMINATE:
/************************/
         if ( (task = ((struct targ ks_stk *)p2)->task) == SELFTASK)
         {
            ptcb = hipritsk;
            task = hipritsk->task;
         }
         else
            ptcb = (TCB ks_tbl *)&rtxtcb[task];

#ifdef TIME_SLICE /* { */
         ptcb->tslice = (TSLICE)0;
         ptcb->newslice = (TSLICE)0;
#endif /* } TIME_SLICE */

         /* if task has a timeout timer */
         if ( (pclkblk = ptcb->pclkblk) != NULLCLK)
         {
            /* if timeout is still active */
            if (pclkblk->state == TIMER_ACTIVE)
            {
               unlink_timer(pclkblk);
            }
            ptcb->pclkblk = NULLCLK;
         }

         if (ptcb->status & SEMAPHORE_WAIT)
         {
            p2a = ptcb->sp->pksnum;

            if ( (*p2a == RTXC_WAIT)
#ifdef HAS_MAILBOXES /* { */
                                     || (*p2a == RTXC_SEND)
#endif /* } HAS_MAILBOXES */
                                                            )
            {
               sema = ((struct sarg ks_stk *)p2a)->sema;
               semat[sema] = SEMA_PENDING;
            }
            else /* must be RTXC_WAITM - set all semas PENDing */
            {
               for (semalist = ((struct sargm ks_stk *)p2a)->list;
                  (sema = *semalist) != NULLSEMA; semalist++)
               {
                  semat[sema] = SEMA_PENDING;
               }
            }
         }

         if ( (ptcb->status == READY)
#if defined(MAILBOX_WAITERS)   || \
    defined(PARTITION_WAITERS) || \
    defined(RESOURCE_WAITERS)  || \
    defined(QUEUE_WAITERS) /* { */
           || (ptcb->status & COMBO_WAIT)
#endif /* } - MAILBOX_ || PARTITION_ || QUEUE_ || RESOURCE_WAITERS */
                                      )
         {
            /* unlink task from a WAITER or the READY list */
                  /* (all cases have same format) */
            if((ptcb->blink->flink = ptcb->flink) != NULLTCB)
               ptcb->flink->blink = ptcb->blink;
         }

         if (task <= ntasks)
            ptcb->priority = rtxktcb[task].priority; /* reset priority */
#ifdef DYNAMIC_TASKS /* { */
         else
         {
            ptcb->priority = NULLTASK_PRIORITY - 1; /* reset priority */

            /* re-insert tcb into tcb free list for dynamic tasks */
                        /* unless already inactive */
#if 0 /* NOT IMPLEMENTED - since no way to free a never started task */
            if ( (ptcb->status & INACTIVE) != INACTIVE)
#endif
            {
               ptcb->flink = dtcbfl;
               dtcbfl = ptcb;
            }
         }
#endif /* } DYNAMIC_TASKS */

#ifdef FPU /* { */
         if ( (ptcb->fpumode == 1) && (ptcb == fputask) )
         {
            fputask = &rtxtcb[0]; /* release ownership of fpu */
         }
#endif /* } FPU */

         ptcb->status = INACTIVE;

         break;
#endif /* } HAS_TERMINATE */

#ifdef HAS_YIELD /* { */
/************************/
      case RTXC_YIELD:
/************************/
         /* yield is NOP unless next READY task at same priority */
         if (hipritsk->flink->priority != hipritsk->priority)
            break;

         ((struct targ ks_stk *)p2)->ksrc = RC_GOOD;

         ptcb = hipritsk; /* save */

#ifdef TIME_SLICE /* { */
         ptcb->tslice = ptcb->newslice; /* reset time allotment */
#endif /* } TIME_SLICE */

         UNLINK_HIPRITSK(); /* unlink first tcb */

         /* re-insert task */
         ptcb->flink = nsrttcb;
         nsrttcb = ptcb;

         break;
#endif /* } HAS_YIELD */

#ifdef HAS_DEFPRIORITY /* { */
/************************/
      case RTXC_DEFPRIORITY:
/************************/
         /* limit priority to reasonable value, else real low priority */
         if ( (((struct targ ks_stk *)p2)->priority < 1) ||
              (((struct targ ks_stk *)p2)->priority >= NULLTASK_PRIORITY) )
                 ((struct targ ks_stk *)p2)->priority = NULLTASK_PRIORITY - 1;

         if ( (task = ((struct targ ks_stk *)p2)->task) == SELFTASK)
            ptcb = hipritsk;
         else
            ptcb = (TCB ks_tbl *)&rtxtcb[task];

         chgpriority(ptcb, ((struct targ ks_stk *)p2)->priority);
         break;
#endif /* } HAS_DEFPRIORITY */

#ifdef HAS_USER /* { */
/************************/
      case RTXC_USER:
/************************/
         /* call function passing &arg packet */
         ((struct userarg ks_stk *)p2)->val =
            (*((struct userarg ks_stk *)p2)->fun)
            (((struct userarg ks_stk *)p2)->arg);
         break;
#endif /* } HAS_USER */

#ifdef HAS_NOP /* { */
/************************/
      case RTXC_NOP:
/************************/
         break;
#endif /* } HAS_NOP */

/************************/
      default:
/************************/
         break;
   }

/* end if forcing the compiler to generate a JUMP TABLE - deterministic */
#pragma endoptimize

   PUSHPSWH; /* save current processor priority level */
   DISABLE;
   if ( (semaput == semaget) && (nsrttcb == NULLTCB) && (t_expired == 0) )
   {
#ifdef FPU /* { */
        if ( (hipritsk->fpumode == 1) && (fputask != hipritsk) )
        {
             /* swap fpu regs */
             fpuswap(fputask->fpregs, hipritsk->fpregs);
             fputask = hipritsk; /* update new user of fpu */
        }
#endif /* } FPU */
        return(hipritsk->sp);
   }
   else
   {
       POPPSWH;
       return(postem());
   }
}

#ifdef HAS_MAILBOXES /* { */
/* thread message into mailbox chain in priority as specified by sender */
/************************/
static void insert_message(MHEADER ks_tbl *pmh, RTXCMSG *prtxcmsg)
/************************/
{
   RTXCMSG *prev, *next;
   PRIORITY priority;

   /* see if the degenerate case exists, i.e., the mailbox is empty */
   if ( (next = pmh->link) == NULL)
      pmh->link = prtxcmsg;
   else /* mailbox is not empty, search for insertion point */
   {
      priority = prtxcmsg->priority;

      prev = (RTXCMSG *)pmh;

      while ( ((next = prev->link) != NULL) && (next->priority <= priority) )
         prev = next;

      prev->link = prtxcmsg;
   }

   prtxcmsg->link = next;
}

/* remove message from task's mailbox */
/************************/
static RTXCMSG *remove_message(MHEADER ks_tbl *pmh, TASK task)
/************************/
{
   RTXCMSG *lptr, *nptr;

   if ( (nptr = pmh->link) == NULL)
      return(NULL);

   if ( (task == (TASK)0) || (nptr->task == task) )
   {
      pmh->link = nptr->link;
      return(nptr);
   }

   while (nptr->link != NULL)
   {
      lptr = nptr;
      nptr = lptr->link;
      if (nptr->task == task)
      {
         lptr->link = nptr->link;
         return(nptr);
      }
   }

   return(NULL);
}
#endif /* } HAS_MAILBOXES */

#if _MODEL == 'm' || _MODEL == 'l'
unsigned long swap_pc_t0(unsigned long temppc)
{
    unsigned long low, high;

    low = temppc & 0x00ffff;         /* low to go high */
    high =  temppc & 0x0ffff0000;    /* high to go low */
    temppc = (low << 16) + (high >> 16);
    return(temppc);
}
#endif /* } _MODEL */

/* clear TCB critical info and initialize task status list */
/************************/
void taskinit(void)
/************************/
{
   register TASK i;
   register TCB ks_tbl *ptcb;

#if _MODEL == 'm' || _MODEL == 'l'
   unsigned long pc1;
#endif /* } _MODEL */

#ifdef BSS_NOT_ZERO /* { */
#ifdef CBUG /* { */
   cbugflag = 0;
#endif /* } CBUG */
#endif /* } BSS_NOT_ZERO */

#ifdef FPU /* { */
   fputask =
#endif /* } FPU */
   hipritsk = (TCB ks_tbl *)&rtxtcb[0];

   hipritsk->blink = (TCB ks_tbl *)&hipritsk;

   hipritsk->priority = NULLTASK_PRIORITY;

#ifdef BSS_NOT_ZERO /* { */
   hipritsk->task = 0;
   hipritsk->flink = NULLTCB;
   hipritsk->status = READY; /* mark null task READY */
   nsrttcb = NULLTCB;
#ifdef FPU /* { */
   hipritsk->fpregs = NULLFPREGS;
   hipritsk->fpumode = 0;  /* all other tasks set in execute */
#endif /* } FPU */
#endif /* } BSS_NOT_ZERO */

   for (i = 1, ptcb = (TCB ks_tbl *)&rtxtcb[1]; i <= ntasks; i++, ptcb++)
   {
      ptcb->status = INACTIVE; /* mark each task inactive */
      ptcb->task = i; /* set each task no. */

#ifdef BSS_NOT_ZERO /* { */
      ptcb->pclkblk = NULLCLK;
#endif /* } BSS_NOT_ZERO */

      ptcb->priority = rtxktcb[i].priority; /* initial priority */

#ifdef FPU /* { */
      ptcb->fpregs = rtxktcb[i].fpregs;
#ifdef BSS_NOT_ZERO /* { */
      ptcb->fpumode = 0;
#endif /* } BSS_NOT_ZERO */
#endif /* } FPU */

#ifdef TIME_SLICE /* { */
#ifdef BSS_NOT_ZERO /* { */
      ptcb->tslice = (TSLICE)0;
      ptcb->newslice = (TSLICE)0;
#endif /* } BSS_NOT_ZERO */
#endif /* } TIME_SLICE */

      /* copy stackbase, stacksize and entry point from KTCB to TCB */
      ptcb->stackbase = (char ks_stk *)rtxktcb[i].stackbase;
      ptcb->stacksize = rtxktcb[i].stacksize;
#ifdef CBUG
      stkinit((short ks_stk *)ptcb->stackbase, ptcb->stacksize);
#endif /* } CBUG */

#if _MODEL == 'm' || _MODEL == 'l'
      pc1 = (unsigned long)rtxktcb[i].pc_t0;
      ptcb->pc_t0 = (void (*)(void))swap_pc_t0(pc1);
#else
      ptcb->pc_t0 = rtxktcb[i].pc_t0;
#endif /* } _MODEL */

#ifdef HAS_INQTASK_ARG /* { */
#ifdef BSS_NOT_ZERO /* { */
      ptcb->arg = NULLARG;
#endif /* } BSS_NOT_ZERO */
#endif /* } HAS_INQTASK_ARG */
   }

#ifdef DYNAMIC_TASKS /* { */
   /* thread dynamic task TCB free pool - possibly dntasks == 0 */
   if (dntasks)
     dtcbfl = (TCB ks_tbl *)&rtxtcb[ntasks + 1];
   else
     dtcbfl = NULLTCB;

   for (i = 1, ptcb = dtcbfl; i <= dntasks; i++, ptcb++)
   {
      ptcb->flink = ptcb + 1; /* insert tcb into linked list */

      ptcb->status = INACTIVE; /* mark each task inactive */
      ptcb->task = ntasks + i; /* set each task no. */

#ifdef BSS_NOT_ZERO /* { */
      ptcb->pclkblk = NULLCLK;
#endif /* } BSS_NOT_ZERO */

      ptcb->priority = NULLTASK_PRIORITY - 1; /* initial priority (low) */

#ifdef FPU /* { */
#ifdef BSS_NOT_ZERO /* { */
      ptcb->fpregs = NULLFPREGS;
      ptcb->fpumode = 0;
#endif /* } BSS_NOT_ZERO */
#endif /* } FPU */

#ifdef TIME_SLICE /* { */
#ifdef BSS_NOT_ZERO /* { */
      ptcb->tslice = (TSLICE)0;
      ptcb->newslice = (TSLICE)0;
#endif /* } BSS_NOT_ZERO */
#endif /* } TIME_SLICE */

#ifdef HAS_INQTASK_ARG /* { */
#ifdef BSS_NOT_ZERO /* { */
      ptcb->arg = NULLARG;
#endif /* } BSS_NOT_ZERO */
#endif /* } HAS_INQTASK_ARG */
   }

   if (dntasks)
   {
      --ptcb;
      ptcb->flink = NULLTCB; /* null last link */
   }
#endif /* } DYNAMIC_TASKS */

#ifdef CBUG
   stkinit((short ks_stk *)&rtxctos[0], RTXCSTKSZ); /* init kernel stack */
#endif /* } CBUG */

#ifdef BSS_NOT_ZERO /* { */
   isrcnt = 0;
#ifdef CBUG /* { */
   isrmax = 0;
#endif /* } CBUG */

#endif /* } BSS_NOT_ZERO */

}

#ifdef HAS_RESOURCES /* { */
/* startup - initialize resource structures */
/************************/
void resinit(void)
/************************/
{
   register RESOURCE i;
   register RHEADER ks_tbl *prh;

   for (i = 1, prh = (RHEADER ks_tbl *)&rheader[1]; i <= nres; i++, prh++)
   {
#ifdef BSS_NOT_ZERO /* { */
      prh->owner = NULLTCB;

#ifdef RESOURCE_WAITERS /* { */
      prh->waiters = NULLTCB;
      prh->dummy = NULLTCB;
#ifdef PRIORITY_INVERSION /* { */
      prh->priority = 0;
#endif /* } PRIORITY_INVERSION */
#endif /* } RESOURCE_WAITERS */

      prh->level = 0;

#ifdef CBUG /* { */
      prh->count = 0;
      prh->conflict = 0;
#endif /* } CBUG */
#endif /* } BSS_NOT_ZERO */

#ifdef PRIORITY_INVERSION /* { */
      prh->resattr = PRIORITY_INVERSION_T0;
#endif /* } PRIORITY_INVERSION */
   }
}
#endif /* } HAS_RESOURCES */

#ifdef HAS_MAILBOXES /* { */
/* startup - initialize mailbox structures */
/************************/
void mboxinit(void)
/************************/
{
#ifdef BSS_NOT_ZERO /* { */
   register MBOX i;
   register MHEADER ks_tbl *pmh;

   for (i = 1, pmh = (MHEADER ks_tbl *)&mheader[1]; i <= nmboxes; i++, pmh++)
   {
      pmh->link = NULL;

#ifdef MAILBOX_WAITERS /* { */
      pmh->waiters = NULLTCB;
      pmh->dummy = NULLTCB;
#endif /* } MAILBOX_WAITERS */

#ifdef MAILBOX_SEMAS /* { */
      pmh->nesema = 0;
#endif /* } MAILBOX_SEMAS */

#ifdef CBUG /* { */
      pmh->count = 0;
#endif /* } CBUG */
   }
#endif /* } BSS_NOT_ZERO */
}
#endif /* } HAS_MAILBOXES */

#ifdef HAS_PARTITIONS /* { */
/* startup partition linking logic */
/************************/
void partinit(void)
/************************/
{
   MAP i, k;
   int j;
   char **next;
   register PHEADER ks_tbl *pph;
   register const PKHEADER *ppkh;

   for (i = 1, ppkh = &pkheader[1], pph = (PHEADER ks_tbl *)&pheader[1]; i <= nparts;
        i++,   ppkh++,              pph++)
   {
      pph->next = ppkh->next;

      if ( (next = (char **)pph->next) != NULL)
      {
         for (j = 1; j < ppkh->count; j++, next = (char **)*next)
            *next = (char *)next + ppkh->size;

#ifdef BSS_NOT_ZERO /* { */
         *next = NULL; /* null last link */
#endif /* } BSS_NOT_ZERO */
      }

      pph->size = ppkh->size;
      pph->count = ppkh->count;

#ifdef BSS_NOT_ZERO /* { */
#ifdef PARTITION_WAITERS /* { */
      pph->waiters = NULLTCB;
      pph->dummy = NULLTCB;
#endif /* } PARTITION_WAITERS */

#ifdef CBUG /* { */
      pph->cur = 0;
      pph->worst = 0;
      pph->usage = 0;
#endif /* } CBUG */
#endif /* } BSS_NOT_ZERO */
   }

#ifdef DYNAMIC_PARTS /* { */
   /* thread dynamic partition PHEADER free pool - possibly dnparts == 0 */
   if (dnparts)
      dphfl = (PHEADER ks_tbl *)&pheader[nparts + 1];
   else
      dphfl = NULLMAP;

   for (i = 1, k = nparts + 1, pph = (PHEADER ks_tbl *)dphfl;
        i <= dnparts; i++, k++, pph++)
   {

      /* insert pheader into linked list */
      pph->next = (struct xmap *)&pheader[k + 1];

      pph->map = k;        /* set map number for return to definer */

#ifdef BSS_NOT_ZERO /* { */
      pph->size = 0;
      pph->count = 0;

#ifdef PARTITION_WAITERS /* { */
      pph->waiters = NULLTCB;
      pph->dummy = NULLTCB;
#endif /* } PARTITION_WAITERS */

#ifdef CBUG /* { */
      pph->cur = 0;
      pph->worst = 0;
      pph->usage = 0;
#endif /* } CBUG */
#endif /* } BSS_NOT_ZERO */
   }

   if (dnparts)
   {
      pph--;
      pph->next = NULL; /* null last link */
   }
#endif /* } DYNAMIC_PARTS */

}
#endif /* } HAS_PARTITIONS */

#ifdef HAS_QUEUES /* { */
/* queue startup support */
/************************/
void queueinit(void)
/************************/
{
   QUEUE i;
   register QHEADER ks_tbl *pqh;
   register const QKHEADER *pqkh;

   for (i = 1, pqkh = &qkheader[1], pqh = (QHEADER ks_tbl *)&qheader[1]; i <= nqueues;
        i++,   pqkh++,              pqh++)
   {
      pqh->base = pqkh->base;
      pqh->width = pqkh->width;
      pqh->depth = pqkh->depth;
      pqh->putndx = pqh->depth - 1;
#ifdef BSS_NOT_ZERO /* { */
      pqh->curndx = 0;

#ifdef QUEUE_SEMAS /* { */
      pqh->nesema = 0;
      pqh->fsema = 0;
      pqh->nfsema = 0;
      pqh->esema = 0;
#endif /* } QUEUE_SEMAS */

#ifdef QUEUE_WAITERS /* { */
      pqh->waiters = NULLTCB;
      pqh->dummy = NULLTCB;
#endif /* } QUEUE_WAITERS */

#ifdef CBUG /* { */
      pqh->count = 0;
      pqh->worst = 0;
#endif /* } CBUG */
#endif /* } BSS_NOT_ZERO */
   }
}
#endif /* } HAS_QUEUES */

/* startup all semaphores as PENDing */
/************************/
void semainit(void)
/************************/
{
   register SEMA i;
   register SSTATE ks_tbl *s;
#ifdef BSS_NOT_ZERO /* { */
   int j;
#endif /* } BSS_NOT_ZERO */

   for (i = 1, s = (SSTATE ks_tbl *)&semat[1]; i <= nsemas; i++, s++)
      *s = SEMA_PENDING;

   semaput = (SEMA ks_data *)&siglist[-1]; /* init empty sema deque */
   semaget = (SEMA ks_data *)&siglist[-1]; /* init empty sema deque */

#ifdef BSS_NOT_ZERO /* { */
   for (j = 0; j < siglistsize; j++)
      siglist[j] = 0;

#endif /* } BSS_NOT_ZERO */
}

/* link all clock blocks into free list */
/************************/
void clockinit(void)
/************************/
{
   register int i;
   register CLKBLK ks_clk *link;

   /* note, RTXCgen guarantees at least 1 timer */

   link = clkqfl = (CLKBLK ks_clk *)&clkq[0];

   for (i = 1; i < ntmrs; i++, link++)
      link->flink = link + 1;

#ifdef BSS_NOT_ZERO /* { */
   link->flink = NULLCLK;

   clkqptr = NULLCLK;

   rtctick = (TICKS)0;

   t_expired = 0;

#ifdef HAS_INQTIME /* { */
   rtctime = (time_t)0;
   ratecnt = 0;
#endif /* } HAS_INQTIME */

#ifdef TIME_SLICE /* { */
   sliceup = NULLTCB;
#endif /* } TIME_SLICE */

#endif /* } BSS_NOT_ZERO */
}

#ifdef HAS_PARTITIONS /* { */
#ifdef HAS_ALLOC /* { */
void * KS_ISRalloc(MAP map)
{
   unsigned char pswh;  /* local variable for saving processor priority level */
   PHEADER ks_tbl *pph;
   struct xmap *q;

   pph = (PHEADER ks_tbl *)&pheader[map];

   PUSHPSWH;   /* save current processor priority level */
   DISABLE;   /* in case called from isr */
              /* can also be called from RTXC_ALLOC code */

   if ( (q = pph->next) != NULL) /* if any avail, return 1st */
   {
      pph->next = q->link; /* unlink 1st one from list */
#ifdef CBUG /* { */
      if (++pph->cur > pph->worst) /* check for new worst case */
         pph->worst = pph->cur;          /* usage level */
#endif /* } CBUG */
   }
   POPPSWH;

   return(q);
}
#endif /* } HAS_ALLOC */
#endif /* } HAS_PARTITIONS */

void KS_ISRsignal(SEMA sema)
{
   unsigned char pswh;  /* local variable for saving processor priority level */

   /* save current processor priority level, disable interrupts, */
   /* stash the semaphore and restore PPL */
   SIGNAL(sema);
}

/* rtxc standard interrupt exit logic */
/************************/
FRAME ks_stk * KS_ISRexit(FRAME ks_stk *frame, SEMA sema)
/************************/
{
   FRAME ks_stk *frptr;
   unsigned char pswh;  /* local variable for saving processor priority level */

#ifdef CBUG /* { */
   if (isrcnt > isrmax) /* audit worst case interrupt depth */
      isrmax = isrcnt;
#endif /* } CBUG */

   PUSHPSWH; /* save current processor priority level */
   DISABLE; /* turn off interrupts briefly while checking nest level */

   if (sema != NULLSEMA) /* if non-zero sema passed */
      *++semaput = sema; /* put sema in post list */

   if (isrcnt == 1)                /* if 1st nest level, ... */
                                   /* return via postem() */
   {
      hipritsk->sp = frame;

      /* finished processing interrupt, .... */
      /* restore to task PPL before calling postem() */
      ENABLE;

      frptr = postem();
      frptr->psw = (frptr->psw & 0xF0FF) | (intlvl << 8);
      return(frptr);
   }
   else /* return to rtxc or isr, postem() will be performed later */
   {
      return(frame); /* return to rtxc or isr */
   }
}

/************************/
static FRAME ks_stk *postem(void) /* returns with interrupts disabled */
/************************/
{
   unsigned char pswh;  /* local variable for saving processor priority level */
   TICKS tcnt, xx;
   SEMA sema;
   TASK task;
   SSTATE ks_tbl *sema_ptr;
   TCB ks_tbl *ptcb;
   TCB ks_tbl *prev;
   KSNUM ks_stk *p2a;
   CLKBLK ks_clk *pclkblk;
#ifdef HAS_WAITM /* { */
   const SEMA *list;
#endif /* } HAS_WAITM */
#ifdef RESOURCE_TIMEOUTS /* { */
   KSNUM ks_stk *p2;
#ifdef PRIORITY_INVERSION /* { */
    RESOURCE resource;
    RHEADER ks_tbl *prh;
    PRIORITY priority;
#endif /* } PRIORITY_INVERSION */
#endif /* } RESOURCE_TIMEOUTS */

   for (;;)
   {
      PUSHPSWH;    /* save current processor priority level */
      DISABLE;

      if (t_expired) /* if timer expired with last clock interrupt ... */
      {
         t_expired = 0;
         tcnt = rtctick; /* local copy of rtctick */
         POPPSWH;

/*
 * xx is used in the following statement because some compilers can't
 * properly handle the arithmetic when negative values are involved
*/
         while ( (clkqptr != NULLCLK) && ((xx = tcnt-clkqptr->remain) >= 0) )
         {
            pclkblk = clkqptr; /* save address of expired timer */

            PUSHPSWH;    /* save current processor priority level */
            DISABLE;
            /* unlink expired timer with interrupts disabled */
            if ( (clkqptr = clkqptr->flink) != NULLCLK)
                clkqptr->blink = (CLKBLK ks_clk *)&clkqptr;
            POPPSWH;

            pclkblk->state = TIMER_DONE;

/* force the compiler to generate a JUMP TABLE - deterministic */
#pragma optimize t

            switch(pclkblk->objtype)
            {
               case TIMER_OBJ:
                  SIGNAL(pclkblk->objid); /* put semaphore in signal list */

                  if (pclkblk->recycle)         /* if timer is cyclic */
                  {
                     pclkblk->remain = pclkblk->recycle; /* reset counts */

                     /* insert clk timer */
                     insert_timer(pclkblk);
                  }
                  break;

               case DELAY_OBJ:
                  ptcb = (TCB ks_tbl *)&rtxtcb[pclkblk->task];

                  ptcb->pclkblk = NULLCLK;

                  if ( (ptcb->status &= ~DELAY_WAIT) == READY)
                  {
                     /* insert task into insert list */
                     ptcb->flink = nsrttcb;
                     nsrttcb = ptcb;
                  }
                  break;

#ifdef SEMAPHORE_TIMEOUTS /* { */
               case SEMAPHORE_OBJ: /* KS_waitt() and KS_sendt() */
                  semat[pclkblk->objid] = SEMA_PENDING;

                  ptcb = (TCB ks_tbl *)&rtxtcb[pclkblk->task];

                  ptcb->pclkblk = NULLCLK;

                  if ( (ptcb->status &= ~SEMAPHORE_WAIT) == READY)
                  {
                     /* insert task into insert list */
                     ptcb->flink = nsrttcb;
                     nsrttcb = ptcb;
                  }
                  break;
#endif /* } SEMAPHORE_TIMEOUTS */

#ifdef QUEUE_TIMEOUTS /* { */
               case QUEUE_OBJ:     /* KS_enqueuet() and KS_dequeuet() */
                  /* fall into common code */
#endif /* } QUEUE_TIMEOUTS */

#ifdef PARTITION_TIMEOUTS /* { */
               case PARTITION_OBJ: /* KS_alloct() */
                  /* fall into common code */
#endif /* } PARTITION_TIMEOUTS */

#ifdef RESOURCE_TIMEOUTS /* { */
               case RESOURCE_OBJ:  /* KS_lockt() */
                  /* fall into common code */
#endif /* } RESOURCE_TIMEOUTS */

#ifdef MAILBOX_TIMEOUTS /* { */
               case MAILBOX_OBJ:   /* KS_receivet() */
                  /* fall into common code */
#endif /* } MAILBOX_TIMEOUTS */

#if defined(MAILBOX_TIMEOUTS)   || \
    defined(PARTITION_TIMEOUTS) || \
    defined(QUEUE_TIMEOUTS)     || \
    defined(RESOURCE_TIMEOUTS) /* { */
                  /* common code */
                  ptcb = (TCB ks_tbl *)&rtxtcb[pclkblk->task];

#ifdef RESOURCE_TIMEOUTS /* { */
                  if (pclkblk->objtype == RESOURCE_OBJ)
                  {
                     /* mark KS_lockt() failure */
                     p2 = ptcb->sp->pksnum;

                     ((struct larg ks_stk *)p2)->ksrc = RC_TIMEOUT;

#ifdef PRIORITY_INVERSION /* { */
                     resource = ((struct larg ks_stk *)p2)->resource;
                     prh = (RHEADER ks_tbl *)&rheader[resource];

                     /* if task that owns resource is priority inverted */
                     if (prh->priority != (PRIORITY)0)
                     {
                        /*
                         * then determine if the task was inverted because
                         * of the lockt() that just timed-out. If so, then
                         * determine what priority to change to.  The new
                         * priority will be either the original or that of
                         * the next task waiting on the resource (if any).
                        */

                        if (prh->waiters == ptcb)
                        {
                           if (prh->waiters->flink != NULLTCB)
                           {
                              priority = prh->waiters->flink->priority;
                              if (priority > prh->priority)
                                 priority = prh->priority;
                           }
                           else
                              priority = prh->priority;

                           chgpriority(prh->owner, priority);
                        }
                     }
#endif /* } PRIORITY_INVERSION */
                  }
#endif /* } RESOURCE_TIMEOUTS */

                  ptcb->pclkblk = NULLCLK;

                  /* remove tcb from 2-way waiter list */
                  if ( (ptcb->blink->flink = ptcb->flink) != NULLTCB)
                     ptcb->flink->blink = ptcb->blink;

                  if ( (ptcb->status &= ~COMBO_WAIT) == READY)
                  {
                     /* insert task into insert list */
                     ptcb->flink = nsrttcb;
                     nsrttcb = ptcb;
                  }
                  break;
#endif /* } - MAILBOX_ || PARTITION_ || QUEUE_ || RESOURCE_TIMEOUTS */

               default: /* serious trouble here - undefined type */
                  break;
            }
/* end if forcing the compiler to generate a JUMP TABLE - deterministic */
#pragma endoptimize
         }

#ifdef TIME_SLICE /* { */
         if (sliceup != NULLTCB)
         {
            if (sliceup->status == READY)
            {
               if (sliceup->priority == sliceup->flink->priority)
               {
                  /* yield sliceup to next task in list */

                  /* unlink tcb */
                  sliceup->flink->blink = sliceup->blink;
                  sliceup->blink->flink = sliceup->flink;

                  /* re-insert task */
                  sliceup->flink = nsrttcb;
                  nsrttcb = sliceup;
               }
            }
            sliceup = NULLTCB;
         }
#endif /* } TIME_SLICE */

         PUSHPSWH;    /* save current processor priority level */
         DISABLE;

      }

      /* check to see if any semaphores to process */
      while (semaput != semaget)
      {
         POPPSWH;

         sema = *++semaget;     /* get sema from siglist ... */
         sema_ptr = (SSTATE ks_tbl *)&semat[sema];

         if ( (*sema_ptr == SEMA_PENDING) || (*sema_ptr == SEMA_DONE) )
            *sema_ptr = SEMA_DONE;
         else /* sema was found in wait state */
         {
            task = *sema_ptr; /* extract waiting task no. */

            *sema_ptr = SEMA_PENDING; /* reset semaphore pending */

            ptcb = (TCB ks_tbl *)&rtxtcb[task];

            p2a = ((FRAME ks_stk *)(ptcb->sp))->pksnum;

#ifdef SEMAPHORE_TIMEOUTS /* { */
            /* if timer associated with semaphore */
            if ( ((pclkblk = ptcb->pclkblk) != NULLCLK) &&
                  (pclkblk->objtype == SEMAPHORE_OBJ) &&
                  (pclkblk->objid == sema) )
            {
               ((struct sarg ks_stk *)p2a)->ksrc = RC_GOOD;

               unlink_timer(pclkblk);
               ptcb->pclkblk = NULLCLK;
            }
#endif /* } SEMAPHORE_TIMEOUTS */

#ifdef HAS_WAITM /* { */
            if ( ((struct sarg ks_stk *)p2a)->ksnum == RTXC_WAITM)
            {
               /* pass sema back via task stack arg packet */
               ((struct sarg ks_stk *)p2a)->sema = sema;

               for (list = ((struct sargm ks_stk *)p2a)->list;
                    *list != NULLSEMA; list++)
               {
                  sema_ptr = (SSTATE ks_tbl *)&semat[*list]; /* get ptr to sema */
                  if ( (*sema_ptr != SEMA_PENDING) &&
                       (*sema_ptr != SEMA_DONE) )
                     *sema_ptr = SEMA_PENDING;
               }
            }
#endif /* } HAS_WAITM */

            if ( (ptcb->status &= ~SEMAPHORE_WAIT) == READY)
            {
               ptcb->flink = nsrttcb;
               nsrttcb = ptcb;
            }
         }
         PUSHPSWH;    /* save current processor priority level */
         DISABLE;
      }
      /* loop exited with interrupts disabled, all semaphores processed */

      semaput = (SEMA ks_data *)&siglist[-1]; /* reset sema signal list empty */
      semaget = (SEMA ks_data *)&siglist[-1]; /* reset sema signal list empty */

      POPPSWH;

      /* process task list now with interrupts on */
      while ( (ptcb = nsrttcb) != NULLTCB)
      {
         /* unlink first tcb in thread and insert in READY list */
         nsrttcb = ptcb->flink;

         /* insert ptcb in priority order in READY list */
         prev = (TCB ks_tbl *)&hipritsk;

         while (ptcb->priority >= prev->flink->priority)
            prev = prev->flink;

         ptcb->flink = prev->flink;
         ptcb->blink = prev;
         prev->flink->blink = ptcb;
         prev->flink = ptcb;
      }

      PUSHPSWH;    /* save current processor priority level */
      DISABLE;

      /* check to see if any semaphores were posted while processing tasks */
      if ((semaput == semaget) && (t_expired == 0))
         break;    /* if not, then all done - exit as fast as possible */
      else
      {
         POPPSWH; /* else, enable interrupts and back to top of loop */
      }
   }

#ifdef FPU /* { */
   if ( (hipritsk->fpumode == 1) && (fputask != hipritsk) )
   {
      /* swap fpu regs */
      fpuswap(fputask->fpregs, hipritsk->fpregs);
      fputask = hipritsk; /* update new user of fpu */
   }
#endif /* } FPU */

   return(hipritsk->sp); /* exit to hipritsk via tcb.sp */
}

/************************/
static CLKBLK ks_clk *get_clkblk(void)
/************************/
{
   CLKBLK ks_clk *pclkblk;

   if ( (pclkblk = clkqfl) != NULLCLK) /* allocate timer block */
   {
      clkqfl = pclkblk->flink; /* by removing 1st from free list */
      pclkblk->state = TIMER_DONE;
   }
   return(pclkblk);
}

/*
 *    routine to "process" a tick from clock isr
 *
 * sets t_expired and returns 1(true) if timer expires or time slice has
 * occurred
 *
*/
int KS_ISRtick(void)
{
   TICKS xx;

#ifdef CBUG /* { */
   if (cbugflag) /* no clock ticks while RTXCbug active */
      return(0);    /* just return */
#endif /* } CBUG */

   rtctick++; /* update tick counter */

#ifdef HAS_INQTIME /* { */
   if (++ratecnt >= clkrate)
   {
      ratecnt = 0; /* reset rate counter (0 - CLKRATE-1) */
      rtctime++; /* update second counter */
   }
#endif /* } HAS_INQTIME */

#ifdef TIME_SLICE /* { */
   if (hipritsk->newslice) /* if time slicing enabled for running task */
   {
      if (--hipritsk->tslice == (TSLICE)0) /* if time slice is up */
      {
         sliceup = hipritsk; /* save tcb of task that timed out */
         sliceup->tslice = sliceup->newslice; /* reset next slice amount */
         t_expired = 1;
         return(1);
      }
   }
#endif /* } TIME_SLICE */

/*
 * xx is used in the following statement because some compilers can't
 * properly handle the arithmetic when negative values are involved
 */
   if ( (clkqptr != NULLCLK) && ((xx = rtctick - clkqptr->remain) >= 0) )
   {
      t_expired = 1;
      return(1);
   }

   return(0);
}

/*
 * initialize task stacks
 *
 * called by taskinit() and KS_deftask()
 *
*/
#ifdef CBUG /* { */
/************************/
void stkinit(short ks_stk *stackbase, size_t stacksize)
/************************/
{
   int count, j;

   count = stacksize / sizeof(short);

   for (j = 0; j < count; j++)
        *stackbase++ = STACK_FILL;
}
#endif /* } CBUG */

/*
 * insert timer into timer list
 *
 * called by rtxc() and by postem()
*/
/************************/
static void insert_timer(CLKBLK ks_clk *pclkblk)
/************************/
{
   TICKS xx;
   CLKBLK ks_clk *lclkptr;
   CLKBLK ks_clk *nclkptr;

   pclkblk->state = TIMER_ACTIVE;

   lclkptr = (CLKBLK ks_clk *)&clkqptr;

   /* find point of insertion */
/*
 * xx is used in the following statement because some compilers can't
 * properly handle the arithmetic when negative values are involved
 */
   while ( ((nclkptr = lclkptr->flink) != NULLCLK) &&
                  (pclkblk->remain >= (xx = nclkptr->remain - rtctick)) )
      lclkptr = nclkptr;

   /* insert timer between lptr and nptr */
   pclkblk->remain += rtctick;
   pclkblk->flink = nclkptr;
   pclkblk->blink = lclkptr;
   lclkptr->flink = pclkblk;
   if (nclkptr != NULLCLK)
      nclkptr->blink = pclkblk;
}

/*
 * unlink timer from timer list
*/
/************************/
static void unlink_timer(CLKBLK ks_clk *pclkblk)
/************************/
{
   unsigned char pswh;  /* local variable for saving processor priority level */

   pclkblk->state = TIMER_DONE;  /* mark timer done */

   PUSHPSWH;    /* save current processor priority level */
   DISABLE;    /* disable interrutps */
   /* simply remove block from list */
   if ( (pclkblk->blink->flink = pclkblk->flink) != NULLCLK)
   {
      pclkblk->flink->blink = pclkblk->blink;
   }
   POPPSWH;
}

#if defined(MAILBOX_WAITERS)   || \
    defined(PARTITION_WAITERS) || \
    defined(QUEUE_WAITERS)     || \
    defined(RESOURCE_WAITERS)  /* { */
/*
 * insert tcb in priority order
 *
 * unlink hipritsk from READY list
 * and insert hipritsk TCB in priority list (doubly linked)
 * used by mailboxes, partitions, queues, and resources
*/
/************************/
static void porder(TCB ks_tbl *prev)
/************************/
{
   TCB ks_tbl *next;
   TCB ks_tbl *save;
   PRIORITY priority;

   save = hipritsk;
   priority = save->priority;

   /* remove task from READY list (it is ALWAYS first in list) */
   UNLINK_HIPRITSK();

   /* search and insert in priority order */
   while ( ((next = prev->flink) != NULLTCB) &&
          (next->priority <= priority) )
      prev = next;

   prev->flink = save;
   save->blink = prev;

   if ( (save->flink = next) != NULLTCB)
      next->blink = save;
}

/*
 *
 * reorder TCB in respective WAITER list due to priority change
 *
*/
static void reorder_waiters(TCB ks_tbl *ptcb)
{
   /*
    *
    * The cases get a little nastier here than with simply reordering the
    * READY list since there may be no tasks of lower and/or higher priority
    * in the list.  Each case is handled separately (and optimized for speed).
    *
   */

   /* if task is the first waiter in the list */
   if (ptcb->blink->blink == NULLTCB)
   {
      /* if (first and) also last waiter in list */
      if (ptcb->flink == NULLTCB)
      {
         /* fast return since no shuffling necessary */
         return;
      }

      /* if (first and) still higher priority than next waiter */
      if (ptcb->priority <= ptcb->flink->priority)
      {
         /* fast return since no shuffling necessary */
         return;
      }

      /* insert by walking forward */
      fwd_insert(ptcb);
      return;
   }

   /* if last waiter in list (and not also first) */
   if (ptcb->flink == NULLTCB)
   {
      /* if (last and) still lower priority than next to last */
      if (ptcb->blink->priority <= ptcb->priority)
      {
         /* fast return since no shuffling necessary */
         return;
      }

      /* insert by walking backwards */
      bwd_insert(ptcb);
      return;
   }

   /* else task is somewhere in middle of list */

   /* if relative priorities are unchanged */
   if ( (ptcb->blink->priority <= ptcb->priority) &&
        (ptcb->priority <= ptcb->flink->priority) )
   {
      /* fast return since no shuffling necessary */
      return;
   }

   /* if new priority < left-side */
   if (ptcb->priority < ptcb->blink->priority)
   {
      bwd_insert(ptcb);
   }
   else
   {
      fwd_insert(ptcb);
   }
}

/*
 *
 * remove TCB from respective WAITER list and re-insert in forward direction
 *
*/
static void fwd_insert(TCB ks_tbl *ptcb)
{
   TCB ks_tbl *prev;
   TCB ks_tbl *next;
   PRIORITY priority;

   priority = ptcb->priority;

   prev = ptcb->flink;

   /* unlink task from respective WAITER list */
   ptcb->flink->blink = ptcb->blink;
   ptcb->blink->flink = ptcb->flink;

   while ( ((next = prev->flink) != NULLTCB) &&
          (next->priority <= priority) )
      prev = next;

   prev->flink = ptcb;
   ptcb->blink = prev;

   if ( (ptcb->flink = next) != NULLTCB)
      next->blink = ptcb;
}

/*
 *
 * remove TCB from respective WAITER list and re-insert in backward direction
 *
*/
static void bwd_insert(TCB ks_tbl *ptcb)
{
   TCB ks_tbl *prev;
   TCB ks_tbl *next;
   PRIORITY priority;

   priority = ptcb->priority;

   prev = ptcb->blink;

   /* unlink task from respective WAITER list */
   if ((ptcb->blink->flink = ptcb->flink) != NULLTCB )
        ptcb->flink->blink = ptcb->blink;

   for (next = prev->blink;
      (next->blink != NULLTCB) && (priority < next->priority);
      prev = next, next = prev->blink)
      ; /* yes - a null loop is intended here */

   prev->blink = ptcb;
   ptcb->flink = prev;

   ptcb->blink = next;
   next->flink = ptcb;
}
#endif /* } - MAILBOX_ || PARTITION_ || QUEUE_ || RESOURCE_WAITERS */

/*
 *
 * change priority of a task
 *
*/
static void chgpriority(TCB ks_tbl *ptcb, PRIORITY priority)
{
   /* set new priority */
   ptcb->priority = priority;

   /* if current task is changing its own priority */
   if (hipritsk->task == ptcb->task)
   {
      /* if relative priorities are unchanged */
      if (ptcb->priority <= ptcb->flink->priority)
      {
         /* fast return since no shuffling necessary */
         return;
      }

      /* remove task from READY list (it is ALWAYS first in list) */
      UNLINK_HIPRITSK();

      /* re-insert task */
      ptcb->flink = nsrttcb;
      nsrttcb = ptcb;

      return;
   }

   /* if task being changed is RUNnable */
   if (ptcb->status == READY)
   {
      /* if relative priorities are unchanged */
      if ( (ptcb->blink->priority <= ptcb->priority) &&
           (ptcb->priority <= ptcb->flink->priority) )
      {
         /* fast return since no shuffling necessary */
         return;
      }

      /* else unlink task from RUN list */
      ptcb->flink->blink = ptcb->blink;
      ptcb->blink->flink = ptcb->flink;

      /* re-insert task */
      ptcb->flink = nsrttcb;
      nsrttcb = ptcb;

      return;
   }

#if defined(MAILBOX_WAITERS)   || \
    defined(PARTITION_WAITERS) || \
    defined(QUEUE_WAITERS)     || \
    defined(RESOURCE_WAITERS)  /* { */

   /* if task being changed is in a WAITER list */
   if (ptcb->status & COMBO_WAIT)
   {
      reorder_waiters(ptcb);
   }
#endif /* } - MAILBOX_ || PARTITION_ || QUEUE_ || RESOURCE_WAITERS */

   return;
}

#ifdef HAS_PARTITIONS /* { */
#ifdef DYNAMIC_PARTS /* { */
static void dn_defpart(PHEADER ks_tbl *pph, KSNUM ks_stk *p2)
{
   size_t count, blksize;
   char **next;
   int j;

   pph->addr = ((struct parg ks_stk *)p2)->addr;
   blksize = ((struct parg ks_stk *)p2)->size;
   count = ((struct parg ks_stk *)p2)->nblocks;

   pph->next = (struct xmap *)pph->addr;

   next = (char **)pph->next;
   for (j = 1; j < count; j++, next = (char **)*next)
      *next = (char *)next + blksize;

   *next = NULL; /* null last link */
   pph->size = blksize;
   pph->count = count;

#ifdef PARTITION_WAITERS /* { */
   pph->waiters = NULLTCB;
   pph->dummy = NULLTCB;
#endif /* } PARTITION_WAITERS */

#ifdef CBUG /* { */
   pph->cur = 0;
   pph->worst = 0;
   pph->usage = 0;
#endif /* } CBUG */

   return;
}
#endif /* } DYNAMIC_PARTS */
#endif /* } HAS_PARTITIONS */

static void ks_memcpyFN(void ks_queue * d1, const void * s1, size_t n)
{
  register char ks_queue * d;
  register const char * s;

   s = s1;
   d = d1;
   while (n--)
      *d++ = *s++;
}

static void ks_memcpyNF(void * d1, const void ks_queue * s1, size_t n)
{
  register char * d;
  register const char ks_queue * s;

   s = s1;
   d = d1;
   while (n--)
      *d++ = *s++;
}

/* end of rtxc.c */
