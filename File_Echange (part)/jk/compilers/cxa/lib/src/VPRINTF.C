/*
 *	Version : @(#)vprintf.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  vprintf.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for vprintf() routine                      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdarg.h>
#include <stdio.h>

extern int	_doprint( const char *, va_list, FILE * );

int
vprintf( const char *format, va_list arg )
{
	register int chars;

	chars = _doprint( format, arg, stdout );

	/* return number of characters written */
	return( ferror(stdout)? EOF: chars );
}
