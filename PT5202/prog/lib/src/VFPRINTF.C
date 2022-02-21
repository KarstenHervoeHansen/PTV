/*
 *	Version : @(#)vfprintf.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  vfprintf.c                                             *
**                                                                        *
**  DESCRIPTION :  Source file for vfprintf() routine                     *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdarg.h>
#include <stdio.h>

extern int	_doprint( const char *, va_list, FILE * );

int
vfprintf( FILE *stream, const char *format, va_list arg )
{
	register int chars;

	chars = _doprint( format, arg, stream );

	/* return number of characters written */
	return( ferror(stream)? EOF: chars );
}
