/*
 *	Version : @(#)feof.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  feof.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for feof() routine                         *
**                 Checks if the stream has reached the end               *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>

#undef feof

int
feof( FILE * stream )
{
	return( (stream->_flag & _IOEOF) ? 1 : 0 );
}
