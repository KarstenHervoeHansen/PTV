/*
 *	Version : @(#)strstr.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strstr.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for strstr() routine                       *
**                 Return a pointer to the first occurrence of string     *
**                 two in the first string.                               *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>
#include <string.h>


char *
strstr( register const char *cs, register const char *ct )
{
	register const char *search;
	register const char *search2;

	if( *ct )
	{
		for( ; *cs; cs++ )
		{
			for(	search=ct, search2=cs;
				*search && *search==*search2;
				++search, ++search2
			   )
				;		/* look if string ct is in cs	*/

			if( !*search )		/* if found the string in ct,	*/
				return( (char *)cs );	/* return pointer	*/
		}
	}
	else
		return( (char *)cs );

	return( NULL );			/* no matching string found	*/
}
