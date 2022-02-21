/*
 *	Version : @(#)iscntrl.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  iscntrl.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for iscntrl() routine                      *
**                 Returns non zero if given character is a control       *
**                 character.                                             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <ctype.h>

#undef	iscntrl

int
iscntrl( int c )
{
	return( (_ctype_+1)[c] & (__C|__W) );
}
