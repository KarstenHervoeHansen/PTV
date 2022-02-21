/****************************************************************************/ 
/* MODULE:                                                                  */
/*   RtxcObj.c - Methodes for:                                              */
/*               TTimer objects created 5/5-99 by. Tomas D. Kappelgaard.    */
/*               TThread objects created x/5-99 by. Tomas D. Kappelgaard.   */
/*               TStream objects created 20/10-99 by. Tomas D. Kappelgaard. */
/*                                                                          */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*               void *Timer_Create(SEMA Event);                            */
/*               void Timer_Period_Set(TTimer Timer, int Time);             */
/*               int Timer_Period_Get(TTimer Timer);                        */
/*               void Timer_Tick(TTimer Timer, int Time);                   */
/*               void * Thread_Create(int Priority, void (far *entry)(void))*/
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
/* Edit log SAA 991005 */
/* In line 369(++)                                                   */   
/*      removed '&' from address of array of chars (name IS pointer) */
/* In Object EventList, function Wait : removed Timer part if        */
/*                paarmeter *TimeOut is NULL pointer                 */





/*
 *   PTV software for PT5765
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include <stdlib.h>
//#include "rtxcobj.h"
#include "define.h"
#include "rtxcapi.h"
#include "ctask.h"
#include "cclock.h"
#include "cpart.h"
#include "csema.h"
#include "cres.h"
#include "cqueue.h"
#include "stack.h"
#include "objectid.h"
//#include "evlog.h"


/**************** new and delete *****************/
long HeapAlloc;

int malloc2(void* arg)
{
 HeapAlloc+=((*(unsigned long*)arg)+4+15)&0xFFF0;
 *(void**)arg=malloc(*(unsigned int*)arg);
 return(0);
}
void* new(long size)
{
 KS_user(malloc2,&size);
// if (!size)
//  LogEvent(EventOutOfMemmory,0);
 return((void*)size);
} 

int free2(void* addr)
{
 free(addr);
 return(0);
}
void delete(void* addr)
{
 KS_user(free2,addr);
} 


void Sleep(int time)
{
 KS_delay(0,time/CLKTICK);
}	

/**************** TEvent Object *****************/
typedef struct TEvent {
  /* Public: */
  	int	ObjectID;
	void	(*TEvent)(struct TEvent *this, char *Name);
	void	(*_TEvent)(struct TEvent *this);
	int	(*Wait)(struct TEvent *this, long *TimeOut);
	void	(*Set)(struct TEvent *this);
	void	(*Clear)(struct TEvent *this);
	int	(*IsSet)(struct TEvent *this);
	SEMA	ID;
	char *	Name;
  /* Private: */
} TEvent;


struct TEvent	TEventPartition[DNSEMAS];
MAP		TEventMap=0;


void	TEvent_TEvent(struct TEvent *this, char *Name)
{

 struct TEvent *Ptr;

 if(this>(struct TEvent *)0)
 {
  Ptr=this;
  this->ID=NSEMAS+1;
  while(Ptr!=&TEventPartition[0])
  {
   this->ID++;
   Ptr--;
  }
 this->ObjectID=TEventID;
 this->Name=Name;
 }
}


void	TEvent__TEvent(struct TEvent *this)
{
 if(this>(struct TEvent *)0)
  this->ID=0;
}


int	TEvent_Waitt(struct TEvent *this, long *TimeOut)
{

 int ret;

 if(this==0)
  return(0);
 if(TimeOut==0)
 {
  KS_wait(this->ID);
  return(-1);
 }
 if(*TimeOut<0)
  return(-1);
 if(*TimeOut>CLKTICK)
 {
  ret=KS_waitt(this->ID,*TimeOut/CLKTICK);
  if(ret==RC_GOOD)
   return(-1);
  else if(ret==RC_TIMEOUT)
  {
   *TimeOut=0;
   return(-1);
  }
  else 
   return(0);
 }
 KS_wait(this->ID);
 return(-1);
}


void	TEvent_Signal(struct TEvent *this)
{
 if(this>(struct TEvent *)0)
 {
  KS_signal(this->ID);
 }
}


void	TEvent_Pend(struct TEvent *this)
{
 if(this>(struct TEvent *)0)
 {
  KS_pend(this->ID);
 }
}


int	TEvent_InqSema(struct TEvent *this)
{
 if(this>(struct TEvent *)0)
 {
  return(KS_inqsema(this->ID));
 }
 else
 {
  return(0);
 }
}



TEvent	*newTEvent(char *Name)
{

/* VAR */
 TEvent *Event;

/* BEGIN */
 if(TEventMap==0)
 {
  TEventMap=KS_create_part(&TEventPartition,sizeof(struct TEvent),DNSEMAS);  /* Create event map */
  if(TEventMap==0)
   return(0);
 }
 Event=KS_alloc(TEventMap);
 if(Event==0)
 {
//  LogEvent(EventNoMoreEvents,0);
  return(0);	
 }
 Event->TEvent=TEvent_TEvent;
 Event->_TEvent=TEvent__TEvent;
 Event->Wait=TEvent_Waitt;
 Event->Set=TEvent_Signal;
 Event->Clear=TEvent_Pend;
 Event->IsSet=TEvent_InqSema;
 Event->TEvent(Event,Name);
 if(Event->ID>(NSEMAS+DNSEMAS+1))
 {
  KS_free(TEventMap,Event);
  return(0);
 }
 return(Event);
}


void deleteTEvent(TEvent *Event)
{
 Event->_TEvent(Event);
 KS_free(TEventMap,Event);
}

/**************** TResource Object *****************/
typedef struct TResource {
  /* Public: */
  	int	ObjectID;
	void	(*TResource)(struct TResource *this, char *Name);
	void	(*_TResource)(struct TResource *this);
	void	(*Lock)(struct TResource *this);
	void	(*Unlock)(struct TResource *this);
	int	(*Owner)(struct TResource *this);
	RESOURCE ID;
	char *	Name;
  /* Private: */
} TResource;


struct TResource TResourcePartition[DNRES];
MAP		 TResourceMap=0;


void	TResource_TResource(struct TResource *this, char *Name)
{

 struct TResource *Ptr;

 if(this>(struct TResource *)0)
 {
  Ptr=this;
  this->ID=NRES+1;
  while(Ptr!=&TResourcePartition[0])
  {
   this->ID++;
   Ptr--;
  }
 this->ObjectID=TResourceID;
 this->Name=Name;
 }
}


void	TResource__TResource(struct TResource *this)
{
 if(this>(struct TResource *)0)
  this->ID=0;
}


void	TResource_Lock(struct TResource *this)
{
 if(this>(struct TResource *)0)
 {
  KS_lockw(this->ID);
 }
}


void	TResource_Unlock(struct TResource *this)
{
 if(this>(struct TResource *)0)
 {
  KS_unlock(this->ID);
 }
}


int	TResource_Owner(struct TResource *this)
{
 if(this>(struct TResource *)0)
 {
  return(KS_inqres(this->ID));
 }
 else
 {
  return(0);
 }
}



TResource *newTResource(char *Name)
{

/* VAR */
 TResource *Resource;

/* BEGIN */
 if(TResourceMap==0)
 {
  TResourceMap=KS_create_part(&TResourcePartition,sizeof(struct TResource),DNRES);  /* Create Resource map */
  if(TResourceMap==0)
   return(0);
 }
 Resource=KS_alloc(TResourceMap);
 if(Resource==0)
 {
//  LogEvent(EventNoMoreResources,0);
  return(0);	
 }
 Resource->TResource=TResource_TResource;
 Resource->_TResource=TResource__TResource;
 Resource->Lock=TResource_Lock;
 Resource->Unlock=TResource_Unlock;
 Resource->Owner=TResource_Owner;
 Resource->TResource(Resource,Name);
 if(Resource->ID>(NRES+DNRES+1))
 {
  KS_free(TResourceMap,Resource);
  return(0);
 }
 return(Resource);
}


void deleteTResource(TResource *Resource)
{
 Resource->_TResource(Resource);
 KS_free(TResourceMap,Resource);
}



/**************** TThread Object *****************/
typedef struct TThread {
  /* Public: */
  	int	ObjectID;
	void	(*TThread)(struct TThread *this, int Priority, void (*entry)(void));
	void	(*Execute)(struct TThread *this);
	void	(*Terminate)(struct TThread *this);
	void	(*Suspend)(struct TThread *this, int Time);
	void	(*Resume)(struct TThread *this);
	int	(*GetPriority)(struct TThread *this);
	void	(*SetPriority)(struct TThread *this, int Prio);
	TASK	ID;
	char	Status;
  /* Private: */
	char 	*Stack;
} TThread;

char far stk[DNTASKS][STACK_SIZE];

void	TThread_TThread(struct TThread *this, int Priority, void (*entry)(void))
{
 if(this>(void *)0)
 {
  this->ID=KS_alloc_task();
  if(this->ID>0)
  {
   this->ObjectID=TThreadID;
   this->Stack=stk[this->ID];
   KS_deftask(this->ID,Priority,this->Stack,STACK_SIZE,entry);
  }
 }
}


void	TThread_Execute(struct TThread *this)
{
 KS_execute(this->ID);
}


void	TThread__TThread(struct TThread *this)
{
 if(this->ID>0)
 {
  KS_terminate(this->ID);
 }

}


void	TThread_Suspend(struct TThread *this, int Time)
{
 if(this->ID>0)
 {
  if(Time<0)
  {
   KS_suspend(this->ID);
  }
  else
  {
   KS_delay(this->ID,Time/CLKTICK);
  }
 }
}


void	TThread_Resume(struct TThread *this)
{
 if(this->ID>0)
 {
  KS_resume(this->ID);
 }
}


int	TThread_GetPriority(struct TThread *this)
{
 if(this->ID>0)
  return(KS_inqpriority(this->ID));
 return(0);
}


void	TThread_SetPriority(struct TThread *this, int Prio)
{
 if(this->ID>0)
  KS_defpriority(this->ID,Prio);
}


TThread		TThreadPartition[DNTASKS];
MAP		TThreadMap=0;



TThread	*newTThread(int Priority, void (*entry)(void))
{

/* VAR */
 TThread *Thread;

/* BEGIN */
 if(TThreadMap==0)
 {
  TThreadMap=KS_create_part(&TThreadPartition,sizeof(struct TThread),DNTASKS);  /* Create stack map */
  if(TThreadMap==0)
   return(0);
 }
 Thread=KS_alloc(TThreadMap);
 if(Thread==(void *)0)
 {
//  LogEvent(EventNoMoreThreads,0);
  return(0);	
 }
 Thread->TThread=TThread_TThread;
 Thread->Execute=TThread_Execute;
 Thread->Terminate=TThread__TThread;
 Thread->Suspend=TThread_Suspend;
 Thread->Resume=TThread_Resume;
 Thread->GetPriority=TThread_GetPriority;
 Thread->SetPriority=TThread_SetPriority;
 Thread->TThread(Thread,Priority,entry);
 if(Thread->ID==0)
 {
  KS_free(TThreadMap,Thread);
  return(0);
 }
 return(Thread);
}


void deleteTThread(TThread *Thread)
{
 Thread->Terminate(Thread);
 KS_free(TThreadMap,Thread);
}




/********** TTimer Object **************/
typedef struct TTimer {
  /* Public: */
	int	ObjectID;
	void	(*TTimer)(struct TTimer *this);
	void	(*_TTimer)(struct TTimer *this);
	void	(*SetPeriod)(struct TTimer *this, int Time);
	int	(*GetPeriod)(struct TTimer *this);
	void	(*Tick)(struct TTimer *this, int Time);
	TEvent	*TimeOut;
  /* Private: */
	short	Period;
	CLKBLK	*Handle;

} TTimer;
  

void TTimer_TTimer(struct TTimer *this)
{
 if(this>(void *)0)
 { 
  this->Handle=KS_alloc_timer();
  this->TimeOut=newTEvent(0);
  this->Period=0;
  this->ObjectID=TTimerID;
 }
}


void TTimer__TTimer(struct TTimer *this)
{
 if(this>(void *)0)
 { 
  deleteTEvent(this->TimeOut);
  KS_free_timer(this->Handle);
 }
}


void TTimer_SetPeriod(struct TTimer *this, int Time)
{
 if(this->Period==0)
 {
  if(Time!=0)
  {
   KS_start_timer(this->Handle,(TICKS)Time/CLKTICK,
                 (TICKS)Time/CLKTICK,this->TimeOut->ID);	/* start cyclic timer */
  }
 }
 else
 {
  if(Time==0)
  {
   KS_stop_timer(this->Handle);
  }
  else   						/* stop cyclic timer */
  { 
   KS_restart_timer(this->Handle,(TICKS)Time/CLKTICK,
                    (TICKS)Time/CLKTICK);		/* restart cyclic timer */
  }
 }
 this->Period=Time;
}



int TTimer_GetPeriod(struct TTimer *this)
{
 return(this->Period);
}



void TTimer_Tick(struct TTimer *this, int Time)
{
 if(this->Period==0)
 {
//  this->TimeOut->Set(this->TimeOut);	/* signal update */
  KS_start_timer(this->Handle,(TICKS)Time/CLKTICK,0,this->TimeOut->ID);
                                             /* single shot timeout */
 }
 else
 {
  KS_restart_timer(this->Handle,(TICKS)Time/CLKTICK,
                  (TICKS)(this->Period)/CLKTICK);   /* restart cyclic timer */
 }
}


//static void Timer_RealThread(void);


//int Timer_Thread_Create(void * Timer,int Priority, void (far *entry)(void))
//{

/* VAR */

/* BEGIN */
// ((struct TimerObject *)Timer)->Thread=Thread_Create(Priority,Timer_RealThread);
// if(((struct TimerObject *)Timer)->Thread==0)
//  return(0);
// ((struct TimerObject *)Timer)->TimeOutFunc=entry;
// KS_deftask_arg(((struct TimerObject *)Timer)->Thread->Num,Timer);
// Thread_Execute(((struct TimerObject *)Timer)->Thread);
// return(-1);
//}



//static void Timer_RealThread(void)
//{
 /* VAR */

 /* BEGIN */
// for(;;)
// {
//  KS_wait(((struct TimerObject *)KS_inqtask_arg(0))->TimeOutEvent);

//  ((struct TimerObject *)KS_inqtask_arg(0))->TimeOutFunc();
// } /* END for */
//} /* END task */



static MAP TTimerMap=0;
static TTimer TTimerPartition[NTMRS];



TTimer *newTTimer(void)
{
 
/* VAR */
 TTimer *Timer;

/* BEGIN */
 if(TTimerMap==0)
 {
  TTimerMap=KS_create_part(&TTimerPartition,sizeof(struct TTimer),NTMRS);  /* Create Timer map */
  if(TTimerMap==0)
   return(0);
 }
 Timer=KS_alloc(TTimerMap);
 if(Timer==(void *)0)
 {
//  LogEvent(EventNoMoreTimers,0);
  return(0);	
 }
 Timer->TTimer=TTimer_TTimer;
 Timer->_TTimer=TTimer__TTimer;
 Timer->SetPeriod=TTimer_SetPeriod;
 Timer->GetPeriod=TTimer_GetPeriod;
 Timer->Tick=TTimer_Tick;
 Timer->TTimer(Timer);
 if(Timer->Handle==(CLKBLK *)0)
 {
  KS_free(TTimerMap,Timer);
  return(0);
 }
 return(Timer);
}


void deleteTTimer(TTimer *Timer)
{
 Timer->_TTimer(Timer);
 KS_free(TTimerMap,Timer);
}




/**************** TEventList Object *****************/
typedef struct TEventList {
  /* Public: */
  	int	ObjectID;
	void	(*TEventList)(struct TEventList *this);
	void	(*_TEventList)(struct TEventList *this);
	int	(*Append)(struct TEventList *this, TEvent *Event);
	void	(*Clear)(struct TEventList *this, TEvent *Event);
	int	(*InList)(struct TEventList *this, TEvent *Event);
	TEvent	*(*Wait)(struct TEventList *this, long *TimeOut);
  /* Private: */
	TEvent	*Events[16];
	SEMA	RTXCList[16];
} TEventList;


struct TEventList TEventListPartition[TEVENTLIST_COUNT];
MAP		TEventListMap=0;


void	TEventList_TEventList(struct TEventList *this)
{
 if(this>(struct TEventList *)0)
 {
  this->ObjectID=TEventListID;
 }
}


void	TEventList__TEventList(struct TEventList *this)
{
 if(this>(struct TEventList *)0)
  this->ObjectID=0;
}


int	TEventList_Append(struct TEventList *this, TEvent *Event)
{

 TEvent **Ptr;

 Ptr=&(this->Events[0]);
 if(this==0)
  return(0);
 if(Event==0)
  return(0);
 while(*Ptr!=0)
 {
  if(*Ptr==Event)
   return(-1);
  Ptr++;
 }
 if(Ptr>=&(this->Events[15]))
  return(0);
 *Ptr=Event;
 *(Ptr+1)=0;
 return(-1);
}


void	TEventList_Clear(struct TEventList *this, TEvent *Event)
{

 char found=0;
 TEvent **Ptr;
 
 Ptr=&(this->Events[0]);
 if(this==0)
  return;
 if(Event==0)
  return;
 while(*Ptr!=0)
 {
  if(*Ptr==Event)
   found=-1;
  if(found==-1)
   *Ptr=*(Ptr+1);
  Ptr++;
 }
}


int	TEventList_InList(struct TEventList *this, TEvent *Event)
{
 TEvent **Ptr;
 
 if(this==(void *)0)
  return(0);
 if(Event==0)
  return(0);
 Ptr=&(this->Events[0]);
 while(*Ptr!=0)
 {
  if(*Ptr==Event)
   return(-1);
  Ptr++;
 }
 return(0);
}


TEvent	*TEventList_Wait(struct TEventList *this, long *TimeOut)
{

 SEMA	cause;
 int	index=0;
 TTimer	*TMOTimer;

 TMOTimer=0;
 if(this==0)
 {
  *TimeOut=-1;			  // Allocation error.
  return(0);
 }

// Added test for NULL pointer @ Timeout SAA 991005
// TMOTimer has value zero

 if(TimeOut != 0)
 {
  if(*TimeOut<0)
  {
   *TimeOut=-1;			  // Allocation error.
   return(0);
  }
  if(*TimeOut>CLKTICK)		 // Allocate timer if nessesary.
  {
   TMOTimer=newTTimer();
   if(TMOTimer==0)
   {
    *TimeOut=-1;			  // Allocation error.
    return(0);
   }
   TMOTimer->Tick(TMOTimer,*TimeOut); // Set Timeout.
   if(this->Append(this,TMOTimer->TimeOut)==0)
   {
    *TimeOut=-1;			  // Append Error.
    return(0);
   }
  }
 }
 while(this->Events[index]!=0)	 // Generate RTXC SEMA-list.
 {
  this->RTXCList[index]=this->Events[index]->ID;
  index++;
 }
 this->RTXCList[index]=0;


 cause=KS_waitm(this->RTXCList); // Perform actual wait.


 index=0;
 while(this->Events[index]!=0)	 // Track event-souce from SEMA-list.
 {
  if(this->RTXCList[index]==cause)
   break;
  index++;
 }

 if(TMOTimer!=0)		  // TimeOut was used.
 {
  if(this->Events[index]==TMOTimer->TimeOut)
   *TimeOut=0;			  // TimeOut.
  *TimeOut=CLKTICK*KS_inqtimer(TMOTimer->Handle);
  this->Clear(this,TMOTimer->TimeOut);
  deleteTTimer(TMOTimer);
 }  
 
 return(this->Events[index]);
}



TEventList	*newTEventList(void)
{

/* VAR */
 TEventList *EventList;

/* BEGIN */
 if(TEventListMap==0)
 {
  TEventListMap=KS_create_part(&TEventListPartition,
                               sizeof(struct TEventList),TEVENTLIST_COUNT);
  if(TEventListMap==0)
   return(0);
 }
 EventList=KS_alloc(TEventListMap);
 if(EventList==0)
  return(0);
 EventList->TEventList=TEventList_TEventList;
 EventList->_TEventList=TEventList__TEventList;
 EventList->Append=TEventList_Append;
 EventList->Clear=TEventList_Clear;
 EventList->InList=TEventList_InList;
 EventList->Wait=TEventList_Wait;
 EventList->TEventList(EventList);
 return(EventList);
}


void deleteTEventList(TEventList *EventList)
{
 EventList->_TEventList(EventList);
 KS_free(TEventListMap,EventList);
}




typedef struct TStream {
  /* Public: */
	int		ObjectID;
	void		(*TStream)(struct TStream *this, int width, int depth);
	void		(*_TStream)(struct TStream *this);
	char*		(*Get)(struct TStream *this, void* dest);
	void		(*Put)(struct TStream *this, void* Source);
	QUEUE		ID;
  /* Private: */
	int		Width;
	int		Depth;
	char*		Handle;
} TStream;


struct TStream TStreamPartition[DNQUEUES];
MAP		 TStreamMap=0;


void	TStream_TStream(struct TStream *this, int width, int depth)
{

 struct TStream *Ptr;

 if(this>(struct TStream *)0)
 {
  Ptr=this;
  this->ID=NQUEUES+1;
  while(Ptr!=&TStreamPartition[0])
  {
   this->ID++;
   Ptr--;
  }
  this->ObjectID=TStreamID;
  this->Width=width;
  this->Depth=depth;
  this->Handle=new(this->Width*this->Depth);
  if (this->Handle)
   KS_defqueue(this->ID,this->Width,this->Depth,this->Handle,0);
 }
}


void		TStream__TStream(struct TStream *this)
{
 if(this>(struct TStream *)0)
 {
  this->ID=0;
  if (this->Handle)
   delete(this->Handle);
 }
}


char*		TStream_Get(struct TStream *this, void* Dest)
{
 if(this>(struct TStream *)0)
 {
  KS_dequeuew(this->ID,Dest);
  return(Dest);
 }
 return(0);
}	


void		TStream_Put(struct TStream *this, void* Source)
{
 if(this>(struct TStream *)0)
 {
  KS_enqueuew(this->ID,Source);
 }
}	



TStream *newTStream(int Width, int Depth)
{

/* VAR */
 TStream *Stream;

/* BEGIN */
 if(TStreamMap==0)
 {
  TStreamMap=KS_create_part(&TStreamPartition,
                               sizeof(struct TStream),DNQUEUES);
  if(TStreamMap==0)
   return(0);
 }
 Stream=KS_alloc(TStreamMap);
 if(Stream==0)
 {
//  LogEvent(EventNoMoreStreams,0);
  return(0);
 }
 Stream->TStream=TStream_TStream;
 Stream->_TStream=TStream__TStream;
 Stream->Put=TStream_Put;
 Stream->Get=TStream_Get;
 Stream->TStream(Stream,Width,Depth);
 if(Stream->ID>(NQUEUES+DNQUEUES+1))
 {
  KS_free(TStreamMap,Stream);
  return(0);
 }
 return(Stream);
}


void deleteTStream(TStream* Stream)
{
 Stream->_TStream(Stream);
 KS_free(TStreamMap,Stream);
}


