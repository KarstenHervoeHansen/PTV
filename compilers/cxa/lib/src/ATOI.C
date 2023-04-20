/*
 *	Version : @(#)atoi.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  atoi.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for atoi() routine                         *
**                 Converts given string to an int value                  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>

#define	FALSE	0
#define	TRUE	1

int
atoi( const char * str )
{
	register int result = 0;
	register unsigned char ch;
	char neg = FALSE;

	do
	{
		ch = *str++;

	} while( ((ch >= 9) && (ch <= 13)) || (ch == ' ') );	/* white space */

	if( ch == '-' )			/* check for '-' or '+' */
		neg = TRUE;
	else if( ch != '+' )
		str--;

	for ( ;; )
	{
		ch = *str++;
		
		if ( ch < '0' || ch > '9' )
			break;		/* end of string reached */

		result = 10 * result + ((neg) ? -(ch - '0') : (ch - '0'));
	}

	return( result );
}
