/*
 *	@(#)fsetpos.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fsetpos.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for fsetpos() routine                      *
**                 Sets the file position indicator for the stream        *
**                 pointed by 'file' according to the value of the object *
**                 pointed to by 'pos'.                                   *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
#include <errno.h>

extern int _ungetc[];	/* defined in _iob.c */

int
fsetpos( FILE * file, const fpos_t * pos )
{
	fpos_t	value;

	fflush( file );	/* Make sure all is written to the file */
	value = _lseek( file, * pos, SEEK_SET );
	if( value == EOF )
	{
		errno = ERR_POS;
		return( -1 );	/* return non-zero */
	}

	file->_flag &= ~(_IOEOF);	/* clear EOF indicator */
	_ungetc[ fileno( file ) ] = 0;	/* Undo the effect of 'ungetc' */
	return( 0 );			/* success */
}
