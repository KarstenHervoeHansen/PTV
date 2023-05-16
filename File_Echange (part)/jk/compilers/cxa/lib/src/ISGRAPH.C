/*
 *	Version : @(#)isgraph.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  isgraph.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for isgraph() routine                      *
**                 Returns non zero if given character is a graphic       *
**                 character.                                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <ctype.h>

#undef	isgraph

int
isgraph( int c )
{
	return( (_ctype_+1)[c] & (__P|__U|__L|__N) );
}
