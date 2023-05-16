/*
 *	@(#)fflush.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fflush.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for fflush() routine                       *
**                 Flushes an output stream, the buffer is completely     *
**                 empty afterwards.                                      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

int
fflush( register FILE *iop )
{
	register char *base;
	register n;

	/* Buffered I/O and writable AND buffer present AND is not empty */
	if (	(iop->_flag & (_IONBF|_IOWRT)) == _IOWRT &&
		(base = iop->_base) != NULL && (n = iop->_ptr - base) > 0
	   )
	{
		iop->_ptr = base;	/* Back to start of buffer */

		/* Line buffering, '_cnt' MUST remain '0' */
		if( iop->_bufsiz && !(iop->_flag & _IOLBF) )
			iop->_cnt = iop->_bufsiz;	/* complete buffer empty */

		if( _write( iop, base, n ) != n )	/* returns 'n' when ok */
		{
			iop->_flag |= _IOERR;
			return( EOF );
		}
	}
	return( 0 );
}
