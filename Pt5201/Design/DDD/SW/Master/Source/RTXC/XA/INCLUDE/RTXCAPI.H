/* rtxcapi.h - RTXC interface */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _RTXCAPI_H
#define _RTXCAPI_H

#include "typedef.h"
#include "rtxstruc.h"

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/************************/
/****** SEMAPHORES ******/
/************************/
KSRC KS_wait(SEMA);

#ifdef HAS_WAITT /* { */
KSRC KS_waitt(SEMA, TICKS);
#endif /* } HAS_WAITT */

#ifdef HAS_WAITM /* { */
SEMA KS_waitm(const SEMA *);
#endif /* } HAS_WAITM */

KSRC KS_signal(SEMA);

#ifdef HAS_SIGNALM /* { */
void KS_signalm(const SEMA *);
#endif /* } HAS_SIGNALM */

#ifdef HAS_PEND /* { */
void KS_pend(SEMA);
#endif /* } HAS_PEND */

#ifdef HAS_PENDM /* { */
void KS_pendm(const SEMA *);
#endif /* } HAS_PENDM */

#ifdef HAS_INQSEMA /* { */
SSTATE KS_inqsema(SEMA);
#endif /* } HAS_INQSEMA */

/***********************/
/****** MAILBOXES ******/
/***********************/
#ifdef HAS_MAILBOXES /* { */

void KS_send(MBOX, RTXCMSG *, PRIORITY, SEMA);

#ifdef HAS_SENDW /* { */
void KS_sendw(MBOX, RTXCMSG *, PRIORITY, SEMA);
#endif /* } HAS_SENDW */

#ifdef HAS_SENDT /* { */
KSRC KS_sendt(MBOX, RTXCMSG *, PRIORITY, SEMA, TICKS);
#endif /* } HAS_SENDT */

RTXCMSG * KS_receive(MBOX, TASK);

#ifdef HAS_RECEIVEW /* { */
RTXCMSG * KS_receivew(MBOX, TASK);
#endif /* } HAS_RECEIVEW */

#ifdef HAS_RECEIVET /* { */
RTXCMSG * KS_receivet(MBOX, TASK, TICKS, KSRC *);
#endif /* } HAS_RECEIVET */

#ifdef HAS_ACK /* { */
void KS_ack(RTXCMSG *);
#endif /* } HAS_ACK */

#ifdef HAS_DEFMBOXSEMA /* { */
void KS_defmboxsema(MBOX, SEMA);
#endif /* } HAS_DEFMBOXSEMA */

#endif /* } HAS_MAILBOXES */

/************************/
/****** PARTITIONS ******/
/************************/
#ifdef HAS_PARTITIONS /* { */

void * KS_alloc(MAP);

#ifdef HAS_ALLOCW /* { */
void * KS_allocw(MAP);
#endif /* } HAS_ALLOCW */

#ifdef HAS_ALLOCT /* { */
void * KS_alloct(MAP, TICKS, KSRC *);
#endif /* } HAS_ALLOCT */

void KS_free(MAP, void *);

#ifdef HAS_INQMAP /* { */
size_t KS_inqmap(MAP);
#endif /* } HAS_INQMAP */

#ifdef HAS_CREATE_PART /* { */
MAP KS_create_part(void *, size_t, size_t);
#endif /* } HAS_CREATE_PART */

#ifdef HAS_ALLOC_PART /* { */
MAP KS_alloc_part(void);
#endif /* } HAS_ALLOC_PART */

#ifdef HAS_DEFPART /* { */
void KS_defpart(MAP, void *, size_t, size_t);
#endif /* } HAS_DEFPART */

#ifdef HAS_FREE_PART /* { */
void * KS_free_part(MAP);
#endif /* } HAS_FREE_PART */

#endif /* } HAS_PARTITIONS */

/********************/
/****** QUEUES ******/
/********************/
#ifdef HAS_QUEUES /* { */

KSRC KS_enqueue(QUEUE, void *);

#ifdef HAS_ENQUEUEW /* { */
void KS_enqueuew(QUEUE, void *);
#endif /* } HAS_ENQUEUEW */

#ifdef HAS_ENQUEUET /* { */
KSRC KS_enqueuet(QUEUE, void *, TICKS);
#endif /* } HAS_ENQUEUET */

KSRC KS_dequeue(QUEUE, void *);

#ifdef HAS_DEQUEUEW /* { */
void KS_dequeuew(QUEUE, void *);
#endif /* } HAS_DEQUEUEW */

#ifdef HAS_DEQUEUET /* { */
KSRC KS_dequeuet(QUEUE, void *, TICKS);
#endif /* } HAS_DEQUEUET */

#ifdef HAS_DEFQUEUE /* { */
KSRC KS_defqueue(QUEUE, size_t, int, void ks_queue *, int);
#endif /* } HAS_DEFQUEUE */

#ifdef HAS_DEFQSEMA /* { */
void KS_defqsema(QUEUE, SEMA, QCOND);
#endif /* } HAS_DEFQSEMA */

#ifdef HAS_INQQUEUE /* { */
int KS_inqqueue(QUEUE);
#endif /* } HAS_INQQUEUE */

#ifdef HAS_PURGEQUEUE /* { */
void KS_purgequeue(QUEUE);
#endif /* } HAS_PURGEQUEUE */

#endif /* } HAS_QUEUES */

/**********************/
/***** RESOURCES ******/
/**********************/
#ifdef HAS_RESOURCES /* { */

KSRC KS_lock(RESOURCE);

#ifdef HAS_LOCKW /* { */
KSRC KS_lockw(RESOURCE);
#endif /* } HAS_LOCKW */

#ifdef HAS_LOCKT /* { */
KSRC KS_lockt(RESOURCE, TICKS);
#endif /* } HAS_LOCKT */

KSRC KS_unlock(RESOURCE);

#ifdef HAS_INQRES /* { */
TASK KS_inqres(RESOURCE);
#endif /* } HAS_INQRES */

#ifdef PRIORITY_INVERSION /* { */
#ifdef HAS_DEFRES /* { */
KSRC KS_defres(RESOURCE, RESATTR);
#endif /* } HAS_DEFRES */
#endif /* } PRIORITY_INVERSION */

#endif /* } HAS_RESOURCES */

/***********************/
/****** TASK MGMT ******/
/***********************/
void KS_execute(TASK);

#ifdef HAS_TERMINATE /* { */
void KS_terminate(TASK);
#endif /* } HAS_TERMINATE */

#ifdef HAS_SUSPEND /* { */
void KS_suspend(TASK);
#endif /* } HAS_SUSPEND */

#ifdef HAS_RESUME /* { */
void KS_resume(TASK);
#endif /* } HAS_RESUME */

#ifdef HAS_YIELD /* { */
KSRC KS_yield(void);
#endif /* } HAS_YIELD */

#ifdef HAS_BLOCK /* { */
void KS_block(TASK, TASK);
#endif /* } HAS_BLOCK */

#ifdef HAS_UNBLOCK /* { */
void KS_unblock(TASK, TASK);
#endif /* } HAS_UNBLOCK */

#ifdef HAS_INQTASK /* { */
TASK KS_inqtask(void);
#endif /* } HAS_INQTASK */

#ifdef HAS_INQPRIORITY /* { */
PRIORITY KS_inqpriority(TASK);
#endif /* } HAS_INQPRIORITY */

#ifdef HAS_DEFPRIORITY /* { */
void KS_defpriority(TASK, PRIORITY);
#endif /* } HAS_DEFPRIORITY */

#ifdef HAS_DEFTASK /* { */
KSRC KS_deftask(TASK, PRIORITY, ks_stk char *, size_t, void (*)(void));
#endif /* } HAS_DEFTASK */

#ifdef HAS_INQTASK_ARG /* { */
void * KS_inqtask_arg(TASK);
#endif /* } HAS_INQTASK_ARG */

#ifdef HAS_DEFTASK_ARG /* { */
void KS_deftask_arg(TASK, void *);
#endif /* } HAS_DEFTASK_ARG */

#ifdef HAS_ALLOC_TASK /* { */
TASK KS_alloc_task(void);
#endif /* } HAS_ALLOC_TASK */

/***********************/
/******  TIMERS   ******/
/***********************/
#ifdef HAS_ALLOC_TIMER /* { */
CLKBLK ks_clk * KS_alloc_timer(void);
#endif /* } HAS_ALLOC_TIMER */

#ifdef HAS_FREE_TIMER /* { */
void KS_free_timer(CLKBLK ks_clk *);
#endif /* } HAS_FREE_TIMER */

#ifdef HAS_START_TIMER /* { */
CLKBLK ks_clk * KS_start_timer(CLKBLK ks_clk *, TICKS, TICKS, SEMA);
#endif /* } HAS_START_TIMER */

#ifdef HAS_STOP_TIMER /* { */
KSRC KS_stop_timer(CLKBLK ks_clk *);
#endif /* } HAS_STOP_TIMER */

#ifdef HAS_RESTART_TIMER /* { */
KSRC KS_restart_timer(CLKBLK ks_clk *, TICKS, TICKS);
#endif /* } HAS_RESTART_TIMER */

#ifdef HAS_DELAY /* { */
void KS_delay(TASK, TICKS);
#endif /* } HAS_DELAY */

#ifdef HAS_ELAPSE /* { */
TICKS KS_elapse(TICKS *);
#endif /* } HAS_ELAPSE */

#ifdef HAS_INQTIMER /* { */
TICKS KS_inqtimer(CLKBLK ks_clk *);
#endif /* } HAS_INQTIMER */

/**********************/
/**** SPECIAL SVCS ****/
/**********************/
int KS_user(int (*)(void *), void *);

#ifdef HAS_NOP /* { */
void KS_nop(void);
#endif /* } HAS_NOP */

const char * KS_version(void);

#ifdef HAS_INQTIME /* { */
time_t KS_inqtime(void);
#endif /* } HAS_INQTIME */

#ifdef HAS_DEFTIME /* { */
void KS_deftime(time_t);
#endif /* } HAS_DEFTIME */

#ifdef TIME_SLICE /* { */
void KS_defslice(TASK, TICKS);
#ifdef HAS_INQSLICE /* { */
TICKS KS_inqslice(TASK);
#endif /* } HAS_INQSLICE */
#endif /* } TIME_SLICE */

#ifdef HAS_PARTITIONS /* { */
void * KS_ISRalloc(MAP);
#endif /* } HAS_PARTITIONS */

/* required ISR functions */
FRAME ks_stk * KS_ISRexit(FRAME ks_stk *, SEMA);
void KS_ISRsignal(SEMA);
int KS_ISRtick(void);

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _RTXCAPI_H */

/* end of rtxcapi.h */
