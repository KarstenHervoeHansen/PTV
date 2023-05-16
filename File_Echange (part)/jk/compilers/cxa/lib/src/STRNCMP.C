/*
 *	Version : @(#)strncmp.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strncmp.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for strncmp() routine                      *
**                 Compares two strings to eachother.                     *
**                 Returns : < 0  -- s1<s2                                *
**                           = 0  -- s1=s2                                *
**                           > 0  -- s1>s2                                *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <string.h>


int
strncmp( register const char * s1, register const char * s2, size_t n )
{
	while( n-- )
	{
		if( *s1 != *s2 )
			return(*s1 - *s2);

		if ( !*s1 )
			break;
		s2++;
		s1++;
	}
	return( 0 );
}
