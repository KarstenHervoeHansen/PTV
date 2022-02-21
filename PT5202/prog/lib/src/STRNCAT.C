/*
 *	Version : @(#)strncat.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strncat.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for strncat() routine                      *
**                 Catenates two strings to eachother. A maximum length   *
**                 can be given.                                          *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <string.h>

char *
strncat( register char * s1, register const char * s2, register size_t n)
{
	register char *os1;

	os1 = s1;
	if( n-- )
	{
		while (*s1++)
			;
		--s1;

		while( *s1++ = *s2++ )
		{
			if( !n-- )
			{
				*s1 = '\0';
				break;
			}
		}
	}

	return(os1);
}
