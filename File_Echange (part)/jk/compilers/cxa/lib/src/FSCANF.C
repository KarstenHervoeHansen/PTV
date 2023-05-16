/*
 *	Version : @(#)fscanf.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fscanf.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for fscanf() routine                       *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdarg.h>
#include <stdio.h>

extern int _doscan( const char *fmt, va_list ap, FILE *fin );

int
fscanf( FILE *stream, const char *format, ... )
{
	va_list ap;
	register int items;

	va_start( ap, format );
	items = _doscan( format, ap, stream );
	va_end( ap );
	return( ferror(stream) ? EOF : items ); /* return number of characters read */
}
