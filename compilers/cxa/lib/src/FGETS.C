/*
 *	Version : @(#)fgets.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fgets.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for fgets() routine                        *
**                 Reads all characters from the given stream until a     *
**                 EOF or newline is found                                *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

char *
fgets( char * s, int n, FILE *stream )
{
	register int c;
	register char *cs;

	c = !EOF;	/* initialise for n <= 0 */
	cs = s;
	while ( (--n > 0) && ((c = fgetc( stream )) != EOF) )
	{	/* read until end of file or until 'n-1' characters are read */
		*cs++ = c;
		if ( c == '\n' )	/* Read until newline is found */
			break;
	}

	if ( c == EOF )			/* end of file ? */
		return(  NULL );

	*cs = '\0';			/* append a '\0' character */
	return( s );
}
