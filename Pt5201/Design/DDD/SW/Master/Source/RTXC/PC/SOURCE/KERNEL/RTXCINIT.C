/* rtxcinit.c - RTXC initialization */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "rtxcopts.h"

#ifdef VECTOR_BASED /* { */

extern void interrupt far isvcrtx(void); /* 1st level interrupt entry point */
extern void interrupt (far * far getvect(int))(void);
extern void far setvect(int, void interrupt (far *)(void));
void interrupt (far *oldisr)(void); /* saved isr for RTXC vector */

#endif /* } VECTOR_BASED */

extern void near clockinit(void); /* link clock blocks */
extern void near semainit(void);  /* pend semaphores */
extern void near taskinit(void);  /* set tasks inactive */

#ifdef HAS_MAILBOXES
extern void near mboxinit(void);  /* init mailboxes empty */
#endif /* } HAS_MAILBOXES */

#ifdef HAS_QUEUES
extern void near queueinit(void); /* init width/depth/base/semas */
#endif /* } HAS_QUEUES */

#ifdef HAS_PARTITIONS
extern void near partinit(void);  /* link partition blocks */
#endif /* } HAS_PARTITIONS */

#ifdef HAS_RESOURCES
extern void near resinit(void);   /* clear resources */
#endif /* } HAS RESOURCES */

/*
 * RTXC internal data structure initialization
 *
 * note: rtxcinit() must be called by main().
*/

/*
 *       rtxcinit() is large model, it calls other initialization
 *       routines as small/compact models since all is in same
 *       code segment, RTXC_TEXT.
 *
*/

void far rtxcinit(void)
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

#ifdef VECTOR_BASED /* { */
   oldisr = getvect(RTXC_VECTOR); /* save old RTXC isr */
   setvect(RTXC_VECTOR, isvcrtx); /* claim RTXC vector */
#endif /* } VECTOR_BASED */

}

void far rtxcstop(void)
{

#ifdef VECTOR_BASED /* { */
   setvect(RTXC_VECTOR, oldisr); /* restore original RTXC isr */
#endif /* } VECTOR_BASED */

}

/* End of file - rtxcinit.c */
