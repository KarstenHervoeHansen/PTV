/*
 *	Version : @(#)isspace.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  isspace.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for isspace() routine                      *
**                 Returns non zero if given character is a whitespace    *
**                 character.                                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <ctype.h>

#undef	isspace

int
isspace( int c )
{
	return( (_ctype_+1)[c] & (__S|__W) );
}
