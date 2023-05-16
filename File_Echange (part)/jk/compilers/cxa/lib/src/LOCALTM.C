/*
 *	Version : @(#)localtm.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  localtm.c                                              *
**                                                                        *
**  DESCRIPTION :  The localtime function converts the calendar time      *
**		   pointed to by t into a broken-down time, expressed as  *
**		   localtime.                                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <time.h>
#include <stdlib.h>

extern char * _tzget( int );

struct tm * localtime( const time_t *t )
{
	long tzoff;
	char *endptr;
	time_t secs = *t;
	struct tm *ts;

	tzoff = strtol( _tzget( 2 ), &endptr, 10 );

	if( tzoff <= -(60*13) || tzoff >= (60*13) )
		tzoff = 0;

	secs += (tzoff * 60);

	ts = gmtime( &secs );	/* get broken-down time expressed */
				/* as Coordinate Universal Time   */

	ts->tm_isdst = -1;	/* Daylight Saving Time not available */

	return( ts );
}
