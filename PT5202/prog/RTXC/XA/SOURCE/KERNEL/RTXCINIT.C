/* rtxcinit.c - RTXC initialization */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "rtxcopts.h"

extern void clockinit(void); /* link clock blocks */
extern void semainit(void);  /* pend semaphores */
extern void taskinit(void);  /* set tasks inactive */

#ifdef HAS_MAILBOXES
extern void mboxinit(void);  /* init mailboxes empty */
#endif /* } HAS_MAILBOXES */

#ifdef HAS_QUEUES
extern void queueinit(void); /* init width/depth/base/semas */
#endif /* } HAS_QUEUES */

#ifdef HAS_PARTITIONS
extern void partinit(void);  /* link partition blocks */
#endif /* } HAS_PARTITIONS */

#ifdef HAS_RESOURCES
extern void resinit(void);   /* clear resources */
#endif /* } HAS RESOURCES */

void rtxcinit(void);


/*
 * RTXC internal data structure initialization
 *
 * note: rtxcinit() must be called by main().
*/

void rtxcinit(void)
{
   clockinit(); /* thread timer control blocks */

   semainit();  /* pend semaphores */

   taskinit();  /* initialize task state info */

#ifdef HAS_MAILBOXES
   mboxinit();  /* initialize mailboxes */
#endif /* } HAS_MAILBOXES */

#ifdef HAS_PARTITIONS
   partinit();  /* thread partition blocks */
#endif /* } HAS_PARTITIONS */

#ifdef HAS_QUEUES
   queueinit(); /* initialize queue headers */
#endif /* } HAS_QUEUES */

#ifdef HAS_RESOURCES
   resinit();   /* initialize resources */
#endif /* } HAS_RESOURCES */

}

/* end of rtxcinit.c */
