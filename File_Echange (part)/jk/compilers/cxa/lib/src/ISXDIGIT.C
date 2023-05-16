/*
 *	Version : @(#)isxdigit.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  isxdigit.c                                             *
**                                                                        *
**  DESCRIPTION :  Source file for isxdigit() routine                     *
**                 Returns non zero if given character is a hexadecimal   *
**                 character.                                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <ctype.h>

#undef	isxdigit

int
isxdigit( int c )
{
	return( (_ctype_+1)[c] & (__N|__X) );
}
