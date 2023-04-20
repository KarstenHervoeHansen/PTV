/*
 *	Version : @(#)fprintf.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fprintf.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for fprintf() routine                      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdarg.h>
#include <stdio.h>

extern int	_doprint( const char *, va_list, FILE * );

int
fprintf( FILE *stream, const char *format, ... )
{
	va_list ap;
	register int chars;

	va_start( ap, format );
	chars = _doprint( format, ap, stream );
	va_end( ap );

	/* return number of characters written */
	return( ferror(stream) ? EOF : chars );
}
