/*
 *	Version : @(#)fputc.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fputc.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for fputc() routine                        *
**                 Put one character to the given stream                  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>

int
fputc( int c, FILE *stream )
{
	if( (stream->_flag & (_IOWRT|_IOSTRG)) == (_IOWRT|_IOSTRG) )
	{	/* write to string, handled here */
		/* Do not use 'putc()' macro here */
		/* No range checking, just write to the string */
		*(stream->_ptr)++ = (char)c;
		return( c );
	}
	else
	{
		return( putc( c, stream ) );
	}
}
