/*
 *	@(#)ftell.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  ftell.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for ftell() routine                        *
**                 Returns the current value of the file position         *
**                 indicator for the stream pointed by 'file'.            *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
#include <errno.h>

long
ftell( FILE * file )
{
	long value;

	value = _lseek( file, 0, SEEK_CUR );
	if( value == EOF )
	{
		errno = ERR_POS;
		return( -1 );	/* return non-zero */
	}

	return( value );		/* success */
}
