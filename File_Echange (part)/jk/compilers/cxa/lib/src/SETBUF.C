/*
 *	@(#)setbuf.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  setbuf.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for setbuf() routine                       *
**                 Adds a buffer to the given stream                      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>

void
setbuf( FILE * iop, char *buf )
{
	if( (iop->_flag & _IOMYBUF) && iop->_base )	/* System allocated buffer ? */
		free( iop->_base );

	iop->_flag &= ~(_IOMYBUF|_IONBF|_IOFBF|_IOLBF);	/* Flags off */

	if ( (iop->_base = buf) == NULL )
	{
		iop->_flag |= _IONBF;			/* Buffering off */
		iop->_bufsiz = 0;
	}
	else
	{
		iop->_flag |= _IOFBF;			/* Buffering on */
		iop->_bufsiz = BUFSIZ;
	}
	iop->_ptr = NULL;
	iop->_cnt = 0;					/* Empty buffer yet */
}
