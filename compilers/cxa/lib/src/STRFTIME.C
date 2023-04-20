/*
 *	Version : @(#)strftime.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strftime.c                                             *
**                                                                        *
**  DESCRIPTION :  The strftime function places characters into the array *
**		   pointed to by s as controlled by the string pointed    *
**		   to by format.                                          *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <time.h>
#include <stdlib.h>
#include <string.h>

extern char * _tzget( int );

/* local time values for an English-speaking country */
static const char _ampm_nm[2][3] =
	{ "AM", "PM" };
static const char _swday_nm[7][4] =
	{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *_lwday_nm[7] =
	{ "Sunday", "Monday", "Tuesday", "Wednesday",
	  "Thursday", "Friday", "Saturday" };
static const char _smon_nm[12][4] =
	{ "Jan", "Feb", "Mar", "Apr", "May", "Jun",
	  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
static const char *_lmon_nm[12] =
	{ "January", "February", "March", "April",
	  "May", "June", "July", "August",
	  "September", "October", "November", "December" };
static const char *_fmt_nm[3] =
	{ "%a %b %d %H:%M:%S %Y", "%a %b %d, %Y", "%H:%M:%S" };

static char * strnum( char *, int, int );

size_t strftime( char *s, size_t maxsize, const char *format,
		 const struct tm *t )
{
	const	char *ptr = format;
	const	char *fmtmp = NULL;
	const	char *p;
	size_t	length = strlen( format );
	size_t	lntmp;
	size_t	characters = 0;
	size_t	n;
	char	buf[20];

	lntmp = length;		/* suppress superfluous warning message */
	for( ;; )	/* parse the format string */
	{
		format = ptr;		/* set to current location */

		if( !format )
			goto format_end;

		for( ;; )		/* scan for '%' or '\0' */
		{
			if( *ptr == '\0' )
				break;

			if( length--, *ptr++ == '%' )
				break;
		}

		if( ptr > format )	/* copy any literal text */
		{
			n = ptr - format - (*ptr == '\0' ? 0 : 1);

			characters += n;

			if( n > 0 && maxsize >= characters )
			{
				memcpy( s, format, n );
				s += n;
			}
		}

		if( *ptr )		/* do the conversion */
		{
			length--;	/* decrement format length */

			switch( *ptr++ )
			{               /* switch on conversion specifier */
			case 'a':	/* put short weekday name */
				p = _swday_nm[t->tm_wday];
				break;
			case 'A':	/* put full weekday name */
				p = _lwday_nm[t->tm_wday];
				break;
			case 'b':	/* put short month name */
				p = _smon_nm[t->tm_mon];
				break;
			case 'B':	/* put full month name */
				p = _lmon_nm[t->tm_mon];
				break;
			case 'c':      /* put date and time */
				p = _fmt_nm[0];
				goto new_format;
			case 'd':	/* put day of month, from 1 */
				p = strnum( buf,t->tm_mday, 2 );
				break;
			case 'H':	/* put hour of 24-hour day */
				p = strnum( buf, t->tm_hour, 2 );
				break;
			case 'I':	/* put hour of 12-hour day */
				p = strnum( buf, t->tm_hour % 12, 2 );
				break;
			case 'j':	/* put day of year, from 1 */
				p = strnum( buf, t->tm_yday + 1, 3 );
				break;
			case 'm':	/* put month of year, from 1 */
				p = strnum( buf, t->tm_mon + 1, 2 );
				break;
			case 'M':	/* put minutes after the hour */
				p = strnum( buf, t->tm_min, 2 );
				break;
			case 'p':	/* put AM/PM */
				p = _ampm_nm[(t->tm_hour >= 12)];
				break;
			case 'S':	/* put seconds after the minute */
				p = strnum( buf, t->tm_sec, 2 );
				break;
			case 'U':	/* put Sunday week of the year, sunday from 0 */
				p = strnum( buf, ((t->tm_yday-((t->tm_wday+7)%7)+12)/7-1), 2 );
				break;
			case 'w':	/* put day of week, from Sunday */
				p = strnum( buf, t->tm_wday, 1);
				break;
			case 'W':	/* put Monday week of the year, monday from 1 */
				p = strnum( buf, ((t->tm_yday-((t->tm_wday+6)%7)+12)/7-1), 2 );
				break;
			case 'x':	/* put date */
				p = _fmt_nm[1];
				goto new_format;
			case 'X':	/* put time */
				p = _fmt_nm[2];
new_format:
				if( fmtmp == NULL ) /* save current format */
				{
					fmtmp = ptr;
					ptr = p;
					lntmp = length;
					length = strlen( p );
					p = NULL;
				}
				break;
			case 'y':	/* put year of the century */
				p = strnum( buf, (t->tm_year % 100), 2 );
				break;
			case 'Y':	/* put year */
				p = strnum( buf, (t->tm_year + 1900), 4 );
				break;
			case 'Z':	/* put time zone name */
				p = _tzget( t->tm_isdst > 0 );
				break;
			case '%':	/* put "%" */
				buf[0] = '%'; buf[1] = '\0'; p = buf;
				break;
			default:	/* unknown format, print it */
				buf[0] = *(ptr -1); buf[1] = '\0'; p = buf;
				break;
			}

			if( p )
			{
				n = strlen( p );

				if( n > 0 ) /* put string ? */
				{
					characters += n;

					if( maxsize >= characters )
					{
						memcpy( s, p, n );
						s += n;
					}
				}
			}
		}

		if( length == 0 && fmtmp == NULL )	/* format end */
		{
format_end:
			if( maxsize >= (characters + 1) )
				*s = '\0';	/* null termination */
			else
				characters = 0;	/* maxsize exceeded */

			return( characters );
		}
		else if( length == 0 )	/* restore previous format */
		{
			ptr = fmtmp;
			fmtmp = NULL;
			length = lntmp;
		}
	}
}

static char * strnum( char *s, int value, int length )
{
	if( value < 0 )
		value = 0;

	s += length;
	*s = '\0';	/* null termination */

	while( length-- )
	{
		*--s = value % 10 + '0';
		value /= 10;
	}

	return (s);
}
