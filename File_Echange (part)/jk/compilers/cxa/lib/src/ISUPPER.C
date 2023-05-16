/*
 *	Version : @(#)isupper.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  isupper.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for isupper() routine                      *
**                 Returns non zero if given character is an uppercase    *
**                 character.                                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <ctype.h>

#undef	isupper

int
isupper( int c )
{
	return( (_ctype_+1)[c] & (__U) );
}
