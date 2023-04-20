/*
 *	Version : @(#)strspn.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strspn.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for strspn() routine                       *
**                 Returns the length of the prefix of the first string   *
**                 containing only characters out of the second string    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <string.h>


size_t
strspn( register const char *cs, register const char *ct )
{
	register size_t i = 0;
	register const char *search;

	for( ; *cs; ++cs )
	{
		for( search=ct; *search && *search!=*cs; ++search )
			;	/* look if character from cs is also in ct */

		if( !*search )	/* must be the same character, not NULL	   */
			break;	/* so it is not the same character, stop   */

		++i;		/* increment prefix counter		   */
	}

	return( i );		/* return number of characters matching	   */
}
