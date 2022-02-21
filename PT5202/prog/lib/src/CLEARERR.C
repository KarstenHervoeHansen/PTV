/*
 *	Version : @(#)clearerr.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  clearerr.c                                             *
**                                                                        *
**  DESCRIPTION :  Source file for clearerr() routine                     *
**                 Clears any error on the stream                         *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>

#undef clearerr

void
clearerr( FILE * stream )
{
	stream->_flag &= ~(_IOEOF|_IOERR);
}
