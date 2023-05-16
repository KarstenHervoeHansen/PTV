/*
 *	@(#)fseek.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fseek.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for fseek() routine                        *
**                 Sets the file position indicator for the stream        *
**                 pointed by 'file' by adding the value 'offset' to      *
**                 the position indicated by whence                       *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
extern int _ungetc[];	/* defined in _iob.c */

int
fseek( FILE * file, long offset, int whence )
{
	fpos_t	value;

	fflush( file );	/* Make sure all is written to the file */
	value = _lseek( file, offset, whence );
	if( value == EOF )
		return( -1 );	/* return non-zero */

	file->_flag &= ~(_IOEOF);	/* clear EOF indicator */
	_ungetc[ fileno( file ) ] = 0;	/* Undo the effect of 'ungetc' */
	return( 0 );			/* success */
}
