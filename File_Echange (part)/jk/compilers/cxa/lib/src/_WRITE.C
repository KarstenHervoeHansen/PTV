/*
 *	@(#)_write.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _write.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for _write() routine                       *
**                 Write a block of data to the given file                *
**                 As a working example, this routine just calls the low  *
**                 level routine to write one character, but a faster     *
**                 implementation is often possible.                      *
**                 returns number of characters correctly written.	  *
**                                                                        *
**  This routine should be customised.                                    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

size_t
_write( FILE * iop, char * base, size_t size )
{
	size_t	cnt = 0;

	for( ; size--; base++, cnt++ )
	{	/* Each character must be correctly written */
		if( _iowrite( *base, iop ) != *base )
			break;
	}
	return( cnt );	/* nr of characters correctly written */
}
