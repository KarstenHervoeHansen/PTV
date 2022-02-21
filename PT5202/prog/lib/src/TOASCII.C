/*
 *	Version : @(#)toascii.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  toascii.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for toascii() routine                      *
**                 Converts the given character to ascii                  *
**                 (Non ANSI function)                                    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <ctype.h>

#undef	toascii


int
toascii( int c )
{
	return( c & 0177 );
}
