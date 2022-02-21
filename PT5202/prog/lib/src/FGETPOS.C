/*
 *	@(#)fgetpos.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fgetpos.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for fgetpos() routine                      *
**                 Stores the current value of the file position          *
**                 indicator for the stream pointed by 'file' in the      *
**                 object pointed to by 'pos'.                            *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
#include <errno.h>

int
fgetpos( FILE * file, fpos_t * pos )
{
	/* 'file' points to the open stream                        */
	/* 'pos' points to object which contains the file position */
	fpos_t value;

	value = _lseek( file, 0, SEEK_CUR );
	if( value == EOF )
	{
		errno = ERR_POS;
		return( -1 );	/* return non zero */
	}
	* pos = value;
	return( 0 );		/* success */
}
