/**************************************************************************
**                                                                        *
**  FILE        :  vsprintf.c                                             *
**                                                                        *
**  DESCRIPTION :  Source file for vsprintf() routine                     *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <limits.h>

extern int	_doprint( const char *, va_list, FILE * );

int
vsprintf( char *s, const char *format, va_list arg )
{
	register int	chars;
	struct _iobuf	_strbuf;

	_strbuf._flag = _IOWRT|_IOSTRG;
	_strbuf._ptr = s;
	_strbuf._cnt = INT_MAX;
	chars = _doprint( format, arg, &_strbuf );
	fputc('\0', &_strbuf);	/* terminate string with a NULL byte */

	return( chars );	/* return number of characters written */
}
