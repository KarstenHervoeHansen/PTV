/*
 *	Version : @(#)strcspn.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strcspn.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for strcspn() routine                      *
**                 Returns the length of the prefix in the string,        *
**                 consisting out of characters not present in the        *
**                 second string.                                         *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <string.h>

size_t
strcspn( register const char *cs, register const char *ct )
{
	register size_t i=0;		/* holds length of prefix */
	register const char *search;

	for( ; *cs; cs++ )
	{
		for( search=ct; *search && *search!=*cs; search++ )
			;	/* look if character from cs is also in ct */

		if( *search )	/* when not '\0', a character has matched  */
			break;	/* it is the same character, stop	   */

		i++;			/* one more non match found	   */
	}

	return( i );
}
