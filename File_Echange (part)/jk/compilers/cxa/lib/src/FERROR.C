/*
 *	Version : @(#)ferror.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  ferror.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for ferror() routine                       *
**                 Checks if the stream has an error                      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>

#undef ferror

int
ferror( FILE * stream )
{
	return( (stream->_flag & _IOERR) ? 1 : 0 );
}
