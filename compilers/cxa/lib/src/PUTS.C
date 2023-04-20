/*
 *	Version : @(#)puts.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  puts.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for puts() routine                         *
**                 Writes a complete string to standard output            *
**                 The string is terminated by a newline.                 *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

int
puts( const char * s )
{
	register int c;

	while ( c = *s++ )			/* write until NULL character */
		if( fputc( c, stdout ) != c )
			return( EOF );		/* error */
	/* complete with a newline character */
	return( fputc( '\n', stdout ) == '\n' ? 0 : EOF );
}
