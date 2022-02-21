/*
 *	Version : @(#)strncpy.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strncpy.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for strncpy() routine                      *
**                 Copies two strings to eachother. If the string copied  *
**                 is too short, it will be NULL padded.                  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <string.h>


char *
strncpy( register char * s1, register const char * s2, register size_t n )
{
	register char *os1;

	os1 = s1;
	for ( ; n>0 && *s2; --n )
		*s1++ = *s2++;

	for ( ; n-- > 0; )
		*s1++ = '\0';
	return(os1);
}
