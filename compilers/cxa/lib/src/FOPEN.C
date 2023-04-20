/*
 *	@(#)fopen.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fopen.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for fopen() routine                        *
**                 Searches for an empty stream and opens it again        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>

FILE *
fopen( const char * file, const char * mode )
{
	FILE * iop = NULL;
	int i;

	for( i=0; i<_NFILE; i++ )
	{
		if( !(_iob[i]._flag & (_IOREAD|_IOWRT|_IORW)) )
		{
			iop = &_iob[i];
			iop->_file = i;
			break;
		}
	}
	if ( !iop )	/* cannot find any unused _iob[] structure */
		return( NULL );

	if( !_open( (char *)file, (char *)mode, iop ) )
		return( NULL );	/* Error */

	/* open has success */
	iop->_flag &= ~(_IOERR|_IOEOF);	/* Clear error and end of file indicators */
	iop->_base = malloc( BUFSIZ );	/* Try to get full buffering on this file */
	if( iop->_base )
	{	/* administrate we allocated the buffer */
		iop->_flag |= _IOMYBUF|_IOFBF;
		iop->_bufsiz = BUFSIZ;
	}
	else	/* Malloc without success, just do unbuffered I/O */
	{
		iop->_flag |= _IONBF;
	}

	return( iop );
}

