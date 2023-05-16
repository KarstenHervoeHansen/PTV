/*
 *	Version : @(#)memchr.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  memchr.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for memchr() routine                       *
**                 Searches for a character in a string.                  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <string.h>

void *
memchr( register const void *cs, register int c, register size_t n )
{
	while ( n-- )
	{
		if ( *(char *)cs == (char)c )	/* found character ?	*/
			return( (void *)cs );
		cs = (char *)cs + 1;
	};

	return( NULL );			/* character not found		*/
}
