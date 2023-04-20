/*
 *	Version : @(#)sscanf.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  sscanf.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for sscanf() routine                       *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern int _doscan( const char *fmt, va_list ap, FILE *fin );

int
sscanf( const char *s, const char *format, ... )
{
	va_list ap;
	register int items;
	struct _iobuf	_strbuf;

	va_start( ap, format );
	_strbuf._flag = _IOREAD|_IOSTRG;
	_strbuf._ptr = (char *)s;
	_strbuf._cnt = strlen( s );
	items = _doscan( format, ap, &_strbuf );
	va_end( ap );

	return( items );	/* return number of characters read */
}
