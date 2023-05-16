/*
 *	Version : @(#)getc.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :   getc.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for getc() routine                         *
**                 Reads one character from input.                        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>

#undef getc

int
getc( FILE * stream )
{
	if( stream->_flag & _IONBF && --stream->_cnt >= 0 )
	{
		return( (int)(*(stream)->_ptr++ & 0xFF ) );
	}
	else
	{
		return( _filbuf( stream ) );
	}
}
