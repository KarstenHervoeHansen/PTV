/*
 *	Version : @(#)memcpy.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  memcpy.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for memcpy() routine                       *
**                 Copies data from default memory to default memory      *
**                 Does not check for overlapping spaces.                 *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <string.h>


void *
memcpy( void *s, register const void *ct, register size_t n )
{
	register char *so = s;

	for( ; n--; *so++=*(char *)ct, ct = (char *)ct + 1 )
		;
	return( (void *)s );
}
