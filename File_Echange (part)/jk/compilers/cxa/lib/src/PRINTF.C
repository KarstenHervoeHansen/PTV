/*
 *	Version : @(#)printf.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  printf.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for printf() routine                       *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdarg.h>
#include <stdio.h>

extern int	_doprint( const char *, va_list, FILE * );

int
printf( const char *format, ... )
{
	va_list ap;
	register int chars;

	va_start( ap, format );
	chars = _doprint( format, ap, stdout );
	va_end( ap );

	/* return number of characters written */
	return( ferror(stdout)? EOF: chars );
}
