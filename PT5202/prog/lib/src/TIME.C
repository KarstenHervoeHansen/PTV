/*
 *	Version : @(#)time.c	1.4	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  time.c                                                 *
**                                                                        *
**  DESCRIPTION :  The time function determines the current calendar      *
**		   time in msecs, and stores that value in the location   *
**		   pointed to by timer. It also returns this value.       *
**		   The _stime function sets the current calendar time.    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <time.h>

static time_t _initial;

time_t
time( time_t *timer )
{
	/* milli seconds elapsed since 1 January 1970 */
	time_t _elapsed = _initial + (clock() / (CLOCKS_PER_SEC / 1000));

	if ( timer )
	{
		*timer = _elapsed;
	}
	return _elapsed;
}

void
_stime( time_t *s )
{
	_initial = *s - (clock() / (CLOCKS_PER_SEC / 1000));
	/* initial is current time s - cpu time elapsed. */
}
