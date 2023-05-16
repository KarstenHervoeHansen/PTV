/*
 *	Version : @(#)gets.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  gets.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for gets() routine                         *
**                 Reads all characters from standard input until a       *
**                 newline is found, the newline is replaced by a NULL    *
**                 character.                                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

char *
gets( char * s )
{
	register int c;
	register char *cs;

	cs = s;
	while ( (c = fgetc( stdin )) != '\n' && c != EOF )	/* read until newline or end of file */
		*cs++ = c;

	if ( c == EOF )			/* end of file ? */
		return( NULL );

	*cs = '\0';			/* strip the '\n' character */
	return( s );
}
