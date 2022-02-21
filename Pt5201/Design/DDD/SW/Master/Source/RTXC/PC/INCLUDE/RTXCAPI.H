/* rtxcapi.h - RTXC interface */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _RTXCAPI_H
#define _RTXCAPI_H

#include "typedef.h"
#include "rtxstruc.h"

//#ifdef __cplusplus /* { */
//extern "C" {
//#endif /* } __cplusplus */

/************************/
/****** SEMAPHORES ******/
/************************/
KSRC KSAPI KS_wait(SEMA);

#ifdef HAS_WAITT /* { */
KSRC KSAPI KS_waitt(SEMA, TICKS);
#endif /* } HAS_WAITT */

#ifdef HAS_WAITM /* { */
SEMA KSAPI KS_waitm(const SEMA *);
#endif /* } HAS_WAITM */

KSRC KSAPI KS_signal(SEMA);

#ifdef HAS_SIGNALM /* { */
void KSAPI KS_signalm(const SEMA *);
#endif /* } HAS_SIGNALM */

#ifdef HAS_PEND /* { */
void KSAPI KS_pend(SEMA);
#endif /* } HAS_PEND */

#ifdef HAS_PENDM /* { */
void KSAPI KS_pendm(const SEMA *);
#endif /* } HAS_PENDM */

#ifdef HAS_INQSEMA /* { */
SSTATE KSAPI KS_inqsema(SEMA);
#endif /* } HAS_INQSEMA */

/***********************/
/****** MAILBOXES ******/
/***********************/
#ifdef HAS_MAILBOXES /* { */

void KSAPI KS_send(MBOX, RTXCMSG *, PRIORITY, SEMA);

#ifdef HAS_SENDW /* { */
void KSAPI KS_sendw(MBOX, RTXCMSG *, PRIORITY, SEMA);
#endif /* } HAS_SENDW */

#ifdef HAS_SENDT /* { */
KSRC KSAPI KS_sendt(MBOX, RTXCMSG *, PRIORITY, SEMA, TICKS);
#endif /* } HAS_SENDT */

RTXCMSG * KSAPI KS_receive(MBOX, TASK);

#ifdef HAS_RECEIVEW /* { */
RTXCMSG * KSAPI KS_receivew(MBOX, TASK);
#endif /* } HAS_RECEIVEW */

#ifdef HAS_RECEIVET /* { */
RTXCMSG * KSAPI KS_receivet(MBOX, TASK, TICKS, KSRC *);
#endif /* } HAS_RECEIVET */

#ifdef HAS_ACK /* { */
void KSAPI KS_ack(RTXCMSG *);
#endif /* } HAS_ACK */

#ifdef HAS_DEFMBOXSEMA /* { */
void KSAPI KS_defmboxsema(MBOX, SEMA);
#endif /* } HAS_DEFMBOXSEMA */

#endif /* } HAS_MAILBOXES */

/************************/
/****** PARTITIONS ******/
/************************/
#ifdef HAS_PARTITIONS /* { */

void * KSAPI KS_alloc(MAP);

#ifdef HAS_ALLOCW /* { */
void * KSAPI KS_allocw(MAP);
#endif /* } HAS_ALLOCW */

#ifdef HAS_ALLOCT /* { */
void * KSAPI KS_alloct(MAP, TICKS, KSRC *);
#endif /* } HAS_ALLOCT */

void KSAPI KS_free(MAP, void *);

#ifdef HAS_INQMAP /* { */
size_t KSAPI KS_inqmap(MAP);
#endif /* } HAS_INQMAP */

#ifdef HAS_CREATE_PART /* { */
MAP KSAPI KS_create_part(void *, size_t, size_t);
#endif /* } HAS_CREATE_PART */

#ifdef HAS_ALLOC_PART /* { */
MAP KSAPI KS_alloc_part(void);
#endif /* } HAS_ALLOC_PART */

#ifdef HAS_DEFPART /* { */
void KSAPI KS_defpart(MAP, void *, size_t, size_t);
#endif /* } HAS_DEFPART */

#ifdef HAS_FREE_PART /* { */
void * KSAPI KS_free_part(MAP);
#endif /* } HAS_FREE_PART */

#endif /* } HAS_PARTITIONS */

/********************/
/****** QUEUES ******/
/********************/
#ifdef HAS_QUEUES /* { */

KSRC KSAPI KS_enqueue(QUEUE, void *);

#ifdef HAS_ENQUEUEW /* { */
void KSAPI KS_enqueuew(QUEUE, void *);
#endif /* } HAS_ENQUEUEW */

#ifdef HAS_ENQUEUET /* { */
KSRC KSAPI KS_enqueuet(QUEUE, void *, TICKS);
#endif /* } HAS_ENQUEUET */

KSRC KSAPI KS_dequeue(QUEUE, void *);

#ifdef HAS_DEQUEUEW /* { */
void KSAPI KS_dequeuew(QUEUE, void *);
#endif /* } HAS_DEQUEUEW */

#ifdef HAS_DEQUEUET /* { */
KSRC KSAPI KS_dequeuet(QUEUE, void *, TICKS);
#endif /* } HAS_DEQUEUET */

#ifdef HAS_DEFQUEUE /* { */
KSRC KSAPI KS_defqueue(QUEUE, size_t, int, char *, int);
#endif /* } HAS_DEFQUEUE */

#ifdef HAS_DEFQSEMA /* { */
void KSAPI KS_defqsema(QUEUE, SEMA, QCOND);
#endif /* } HAS_DEFQSEMA */

#ifdef HAS_INQQUEUE /* { */
int KSAPI KS_inqqueue(QUEUE);
#endif /* } HAS_INQQUEUE */

#ifdef HAS_PURGEQUEUE /* { */
void KSAPI KS_purgequeue(QUEUE);
#endif /* } HAS_PURGEQUEUE */

#endif /* } HAS_QUEUES */

/**********************/
/***** RESOURCES ******/
/**********************/
#ifdef HAS_RESOURCES /* { */

KSRC KSAPI KS_lock(RESOURCE);

#ifdef HAS_LOCKW /* { */
KSRC KSAPI KS_lockw(RESOURCE);
#endif /* } HAS_LOCKW */

#ifdef HAS_LOCKT /* { */
KSRC KSAPI KS_lockt(RESOURCE, TICKS);
#endif /* } HAS_LOCKT */

KSRC KSAPI KS_unlock(RESOURCE);

#ifdef HAS_INQRES /* { */
TASK KSAPI KS_inqres(RESOURCE);
#endif /* } HAS_INQRES */

#ifdef PRIORITY_INVERSION /* { */
#ifdef HAS_DEFRES /* { */
KSRC KSAPI KS_defres(RESOURCE, RESATTR);
#endif /* } HAS_DEFRES */
#endif /* } PRIORITY_INVERSION */

#endif /* } HAS_RESOURCES */

/***********************/
/****** TASK MGMT ******/
/***********************/
void KSAPI KS_execute(TASK);

#ifdef HAS_TERMINATE /* { */
void KSAPI KS_terminate(TASK);
#endif /* } HAS_TERMINATE */

#ifdef HAS_SUSPEND /* { */
void KSAPI KS_suspend(TASK);
#endif /* } HAS_SUSPEND */

#ifdef HAS_RESUME /* { */
void KSAPI KS_resume(TASK);
#endif /* } HAS_RESUME */

#ifdef HAS_YIELD /* { */
KSRC KSAPI KS_yield(void);
#endif /* } HAS_YIELD */

#ifdef HAS_BLOCK /* { */
void KSAPI KS_block(TASK, TASK);
#endif /* } HAS_BLOCK */

#ifdef HAS_UNBLOCK /* { */
void KSAPI KS_unblock(TASK, TASK);
#endif /* } HAS_UNBLOCK */

#ifdef HAS_INQTASK /* { */
TASK KSAPI KS_inqtask(void);
#endif /* } HAS_INQTASK */

#ifdef HAS_INQPRIORITY /* { */
PRIORITY KSAPI KS_inqpriority(TASK);
#endif /* } HAS_INQPRIORITY */

#ifdef HAS_DEFPRIORITY /* { */
void KSAPI KS_defpriority(TASK, PRIORITY);
#endif /* } HAS_DEFPRIORITY */

#ifdef HAS_DEFTASK /* { */
KSRC KSAPI KS_deftask(TASK, PRIORITY, char ks_stk *, size_t,
                     void (far *)(void));
#endif /* } HAS_DEFTASK */

#ifdef HAS_INQTASK_ARG /* { */
void * KSAPI KS_inqtask_arg(TASK);
#endif /* } HAS_INQTASK_ARG */

#ifdef HAS_DEFTASK_ARG /* { */
void KSAPI KS_deftask_arg(TASK, void *);
#endif /* } HAS_DEFTASK_ARG */

#ifdef HAS_ALLOC_TASK /* { */
TASK KSAPI KS_alloc_task(void);
#endif /* } HAS_ALLOC_TASK */

/***********************/
/******  TIMERS   ******/
/***********************/
#ifdef HAS_ALLOC_TIMER /* { */
CLKBLK ks_clk * KSAPI KS_alloc_timer(void);
#endif /* } HAS_ALLOC_TIMER */

#ifdef HAS_FREE_TIMER /* { */
void KSAPI KS_free_timer(CLKBLK ks_clk *);
#endif /* } HAS_FREE_TIMER */

#ifdef HAS_START_TIMER /* { */
CLKBLK ks_clk * KSAPI KS_start_timer(CLKBLK ks_clk *, TICKS, TICKS, SEMA);
#endif /* } HAS_START_TIMER */

#ifdef HAS_STOP_TIMER /* { */
KSRC KSAPI KS_stop_timer(CLKBLK ks_clk *);
#endif /* } HAS_STOP_TIMER */

#ifdef HAS_RESTART_TIMER /* { */
KSRC KSAPI KS_restart_timer(CLKBLK ks_clk *, TICKS, TICKS);
#endif /* } HAS_RESTART_TIMER */

#ifdef HAS_DELAY /* { */
void KSAPI KS_delay(TASK, TICKS);
#endif /* } HAS_DELAY */

#ifdef HAS_ELAPSE /* { */
TICKS KSAPI KS_elapse(TICKS *);
#endif /* } HAS_ELAPSE */

#ifdef HAS_INQTIMER /* { */
TICKS KSAPI KS_inqtimer(CLKBLK ks_clk *);
#endif /* } HAS_INQTIMER */

/**********************/
/**** SPECIAL SVCS ****/
/**********************/
int KSAPI KS_user(int (*)(void *), void *);

#ifdef HAS_NOP /* { */
void KSAPI KS_nop(void);
#endif /* } HAS_NOP */

const char * KSAPI KS_version(void);

#ifdef HAS_INQTIME /* { */
time_t KSAPI KS_inqtime(void);
#endif /* } HAS_INQTIME */

#ifdef HAS_DEFTIME /* { */
void KSAPI KS_deftime(time_t);
#endif /* } HAS_DEFTIME */

#ifdef TIME_SLICE /* { */
void KSAPI KS_defslice(TASK, TICKS);
#ifdef HAS_INQSLICE /* { */
TICKS KSAPI KS_inqslice(TASK);
#endif /* } HAS_INQSLICE */
#endif /* } TIME_SLICE */

#ifdef HAS_PARTITIONS /* { */
void * KSAPI KS_ISRalloc(MAP);
#endif /* } HAS_PARTITIONS */

/* required ISR functions */
FRAME * KSAPI KS_ISRexit(FRAME *, SEMA);
void KSAPI KS_ISRsignal(SEMA);
int  KSAPI KS_ISRtick(void);

#ifdef FP_EMU /* { */
void KSAPI init_FPU(TASK task);
#endif /* } FP_EMU */

//#ifdef __cplusplus /* { */
//}
//#endif /* } __cplusplus */

#endif /* } _RTXCAPI_H */

/* End of file - rtxcapi.h */
