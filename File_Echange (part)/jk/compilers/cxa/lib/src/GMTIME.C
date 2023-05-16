/*
 *	Version : @(#)gmtime.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  gmtime.c                                               *
**                                                                        *
**  DESCRIPTION :  The gmtime function converts the calendar time pointed *
**		   to by t into a broken-down time, expressed as          *
**		   Coordinated Universal Time (UTC).                      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <time.h>

short _month_offs( unsigned int, unsigned int );

struct tm * gmtime( const time_t *t )
{
	unsigned int 	year;
	unsigned long	days,
			i;
	time_t		secs;
	static struct tm ts;

	secs = *t + _BIAS_TIME;
	days = secs / 86400;

	ts.tm_wday  = (days + 1) % 7;	/* days since Sunday        */

	year = days / 365;
	if( year )			/* correct for leap year    */
	{
		i = ((year - 1) / 4) + 365L * year;
		if( days < i )
			year--;
		days -= i;
	}

	ts.tm_year  = year;		/* years since 1900         */
	ts.tm_yday  = days;		/* day of the year          */

	if( year )
		year &= 3;
	else
		year = 1;		/* 1900 not a leap year     */

	for( ts.tm_mon = 12; 		/* month of the year        */
	     days < ( i = _month_offs( year, --ts.tm_mon ) ); ) ;

	ts.tm_mday  = days - i + 1;	/* day of the month         */
	secs 	   %= 86400;
	ts.tm_hour  = secs / 3600;	/* hour of the day          */
	secs	   %= 3600;
	ts.tm_min   = secs / 60;	/* minutes after the hour   */
	ts.tm_sec   = secs % 60;	/* seconds after the minute */
	ts.tm_isdst = 0;		/* time zone not in effect  */

	return( &ts );
}


short _month_offs( unsigned int y, unsigned int mon )
{
	static const short _moffs[2][12] =
	{ /*   jan  feb  mar  apr  may  jun  jul  aug  sep  oct  nov  dec */
	    {  0,   31,  60,  91,  121, 152, 182, 213, 244, 274, 305, 335 },
	    {  0,   31,  59,  90,  120, 151, 181, 212, 243, 273, 304, 334 }
	};

	return( _moffs[y?1:0][mon] );	/* y != 0 indicates leap year */
}


