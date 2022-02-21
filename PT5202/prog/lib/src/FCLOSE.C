/*
 *	@(#)fclose.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fclose.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for fclose() routine                       *
**                 Flushes the buffer for given stream, then closes the   *
**                 stream.                                                *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>

int
fclose( FILE *iop )
{
	register r;

	r = EOF;
	if (iop->_flag & (_IOREAD|_IOWRT|_IORW)
	    && (iop->_flag & _IOSTRG) == 0)
	{
		r = fflush( iop );			/* First clear the buffer */
		if( iop->_flag & _IOMYBUF )		/* Did we assign a buffer to this stream ? */
			free( iop->_base );

		if ( _fclose( iop ) < 0 )		/* Call the low level (customised) routine to do all necessary	*/
			r = EOF;			/* hardware actions						*/

		if (iop->_flag & (_IOMYBUF|_IONBF))	/* No buffer assigned anymore */
			iop->_base = NULL;
	}
	iop->_flag &= ~(_IOREAD|_IOWRT|_IORW);		/* Do NOT touch buffering mode ! */
	iop->_cnt = 0;
	iop->_ptr = NULL;
	return( r );
}
