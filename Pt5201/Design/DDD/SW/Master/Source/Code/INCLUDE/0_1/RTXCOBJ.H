#ifndef _RTXCOBJ_H_
 #define _RTXCOBJ_H_

 #include "rtxcapi.h"
 #include "ctask.h"
 #include "csema.h"
 #include "cres.h"
 #include "cqueue.h"
 #include "cpart.h"
 #include "cmbox.h"
 #include "cclock.h"
 #ifdef _RTIO_API_H_  /* { _RTIO_API_H_ */
  #include "cchnls.h"
  #include "ccntlr.h"
  #include "cdevice.h"
 #endif

//#include "stack.h"

 #define THIS 0

/***** Dynamic Memmory allocation ********/
extern void* new(long size);
extern void delete(void* addr);
extern void Sleep(int time);

extern long HeapAlloc;


typedef struct TEvent {
  /* Public: */
  	const int	ObjectID;
	void		(*TEvent)(struct TEvent *this, char *Name);
	void		(*_TEvent)(struct TEvent *this);
	int		(*Wait)(struct TEvent *this, long *TimeOut);
	void		(*Set)(struct TEvent *this);
	void		(*Clear)(struct TEvent *this);
	int		(*IsSet)(struct TEvent *this);
	const SEMA	ID;
	char * const	Name;
  /* Private: */
} TEvent;

extern TEvent	*newTEvent(char *Name);

extern void	deleteTEvent(TEvent *Event);




typedef struct TEventList {
  /* Public: */
  	const int	ObjectID;
	void		(*TEventList)(struct TEventList *this);
	void		(*_TEventList)(struct TEventList *this);
	int		(*Append)(struct TEventList *this, TEvent *Event);
	void		(*Clear)(struct TEventList *this, TEvent *Event);
	int		(*InList)(struct TEventList *this, TEvent *Event);
	TEvent		*(*Wait)(struct TEventList *this, long *TimeOut);
  /* Private: */
	const char	Align[96];
} TEventList;

TEventList	*newTEventList(void);

void deleteTEventList(TEventList *EventList);




typedef struct TResource {
  /* Public: */
  	const int	ObjectID;
	void		(*TResource)(struct TResource *this, char *Name);
	void		(*_TResource)(struct TResource *this);
	void		(*Lock)(struct TResource *this);
	void		(*Unlock)(struct TResource *this);
	int		(*Owner)(struct TResource *this);
	const RESOURCE	ID;
	char * const	Name;
  /* Private: */
} TResource;

extern TResource *newTResource(char *Name);

extern void	deleteTResource(TResource *Resource);




typedef struct TThread {
  /* Public: */
  	const int	ObjectID;
	void		(*TThread)(struct TThread *this, int Priority, void (*entry)(void));
	void		(*Execute)(struct TThread *this);
	void		(*Terminate)(struct TThread *this);
	void		(*Suspend)(struct TThread *this, int Time);
	void		(*Resume)(struct TThread *this);
	int		(*GetPriority)(struct TThread *this);
	void		(*SetPriority)(struct TThread *this, int Prio);
	TASK		ID;
	char		Status;
  /* Private: */
	char		*Stack;
} TThread;

extern TThread	*newTThread(int Priority, void (*entry)(void));

extern void 	deleteTThread(TThread *Thread);




typedef struct TTimer {
  /* Public: */
	const int	ObjectID;
	void		(*TTimer)(struct TTimer *this);
	void		(*_TTimer)(struct TTimer *this);
	void		(*SetPeriod)(struct TTimer *this, int Time);
	int		(*GetPeriod)(struct TTimer *this);
	void		(*Tick)(struct TTimer *this, int Time);
	TEvent		*TimeOut;
  /* Private: */
	short		Period;
	CLKBLK		*Handle;

} TTimer;

extern TTimer *newTTimer(void);

extern void deleteTTimer(TTimer *Timer);



typedef struct TStream {
  /* Public: */
	const int	ObjectID;
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

extern TStream *newTStream(int Width, int Depth);

extern void deleteTStream(TStream* Stream);



#endif
