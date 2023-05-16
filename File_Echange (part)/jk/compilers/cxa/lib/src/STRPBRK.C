/*
 *	Version : @(#)strpbrk.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strpbrk.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for strpbrk() routine                      *
**                 Searches the string for the occurrence of any          *
**                 character out of the second string.                    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>
#include <string.h>

char *
strpbrk( register const char *cs, register const char *ct )
{
	register const char *search;

	do
	{
		for( search=ct; *search && *search!=*cs; search++ )
			;	/* look if character from cs is also in ct */

		if( *search )		/* if found the character in ct, */
			return( (char *)cs );	/* return pointer	 */
	} while( *cs++ );

	return( NULL );			/* no matching character found	   */
}
