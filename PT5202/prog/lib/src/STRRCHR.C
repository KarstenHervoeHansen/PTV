/*
 *	Version : @(#)strrchr.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strrchr.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for strrchr() routine                      *
**                 Search the string for the last occurrence of the       *
**                 given character.                                       *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <string.h>


char *
strrchr( register const char *cs, int c )
{
	register const char *os = cs;

	while( *os )
	{
		os++;
	}

	/* os points to last real 'character' before NULL byte */
	do
	{
		if( *os == (char)c )
		{
			return( (char *)os );
		}
	}
	while( os-- != cs );	/* Post decrement, so actual comparison is done one character to the left */

	return( NULL );
}
