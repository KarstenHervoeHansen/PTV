/*
 *	@(#)setvbuf.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  setvbuf.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for setvbuf() routine                      *
**                 Adds a buffer to the given stream                      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>

int
setvbuf( FILE * iop, char * buf, int mode, size_t size )
{
	if( ! (iop->_flag & (_IOREAD|_IOWRT|_IORW)) )		/* File not open */
		return( 1 );

	if( (mode & (_IONBF|_IOLBF|_IOFBF)) == _IONBF )
	{
		if( buf )			/* No buffering request, but a buffer is specified */
			return( 1 );
	}
	else if	( !((mode & (_IONBF|_IOLBF|_IOFBF)) == _IOLBF) &&
		  !((mode & (_IONBF|_IOLBF|_IOFBF)) == _IOFBF)
		)	/* Illegal buffering compination requested */
		return( 1 );

	/* 'free' a previously allocated buffer */
	if( (iop->_flag & _IOMYBUF) && iop->_base ) 
		free( iop->_base );

	iop->_flag &= ~(_IOMYBUF|_IONBF|_IOFBF|_IOLBF);	/* Flags off */

	if( !(mode & _IONBF) && !buf )		/* Buffering request, no buffer or size specified */
	{
		if( size )
		{
			buf = malloc( size );
			if( !buf )
			{
				iop->_flag |= _IONBF;	/* Things can go on .. */
				return( 1 );
			}
			iop->_flag |= _IOMYBUF;
		}
		else	/* Buffering request, but buffer size is '0', just act like nobuffering */
			mode |= _IONBF;
	}

	if( mode & _IONBF )
	{
		iop->_flag |= _IONBF;
		iop->_base = NULL;
		iop->_bufsiz = 0;
	}
	else	/* 'buf' is present */
	{
		iop->_flag |= (mode & (_IOFBF|_IOLBF));
		iop->_base = buf;
		iop->_bufsiz = size;
	}

	iop->_ptr = NULL;
	iop->_cnt = 0;	/* Empty buffer yet */
	return( 0 );
}
