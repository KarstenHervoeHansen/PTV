/*
 *	@(#)_flsbuf.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _flsbuf.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for _flsbuf() routine                      *
**                 This routine makes sure the buffering of file I/O will *
**                 work as expected. I/O to strings is not done here, but *
**                 in the routine 'fputc()'                               *
**                 returns 'EOF' on error                                 *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

int
_flsbuf( int c, register FILE *iop )
{
	register char *	base;
	register int	n = 0;	/* test value of written character */
	int		i;	/* just a loop counter */

	if (iop->_flag & _IORW)
	{
		iop->_flag |= _IOWRT;	/* Set writable flag */
		iop->_flag &= ~_IOEOF;	/* Reset EOF flag */
	}

	/* check if file is writable */
	if ( !(iop->_flag & _IOWRT) )
	{
		iop->_flag |= _IOERR;
		return( EOF );
	}

tryagain:
	/* unbuffered I/O */
	if ( iop->_flag & _IONBF)
	{
		if( _iowrite( c, iop ) != c )	/* write direct */
		{
			iop->_flag |= _IOERR;
			iop->_cnt = 0;
			return(EOF);
		}
		iop->_cnt = 0;
		return( c );
	}

	/* Buffered I/O */

	/* Is no buffer assigned to this I/O ? */
	if ((base = iop->_base) == NULL)
	{
		if ( (iop == stdout) ||	/* is this stdout ?	*/
		     (iop == stderr) )	/* or stderr ?		*/
		{			/* yes, try unbuffered I/O */
			iop->_flag |= _IONBF;
			goto tryagain;
		}
		/* No buffer, not 'stdout' or 'stderr' -> problem */
		/* No buffer is assigned, the user should do this */
		/* by creating his own '_iob.c' or assigning a    */
		/* buffer with 'setbuf()' or 'setvbuf()'          */
		iop->_flag |= _IOERR;
		return( EOF );
	}

	if( iop->_flag & _IOLBF )	/* Line buffering ? */
	{
		if( !iop->_ptr )	/* Make sure '_ptr' is filled, '_cnt' will always be '0' */
			iop->_ptr = iop->_base;
		iop->_cnt = 0;

		if( (iop->_ptr - iop->_base) < iop->_bufsiz )	/* character still fits in buffer */
			*iop->_ptr++ = c;

		/* while buffer not full and not eol, just return character written to buffer */
		if( !( ((iop->_ptr - iop->_base) == iop->_bufsiz) || (c == '\n') ) )
			return( c );
	}

	/* We have a buffer assigned */
	if( iop->_ptr != NULL && (i = (iop->_ptr - base)) > 0)
	{	/* buffer present and buffered I/O requested */
		/* So, buffer is Full ! */
		if( _write( iop, base, i ) != i )	/* returns 'i' when OK */
		{
			iop->_flag |= _IOERR;
			if( iop->_bufsiz )
				iop->_cnt = iop->_bufsiz;
			else
				iop->_cnt = BUFSIZ;	/* Completely empty buffer */
			iop->_ptr = iop->_base;
			return( EOF );
		}
	}

	iop->_ptr = iop->_base;	/* Return to base immediately */

	/* Line buffering, do not assign anything to '_cnt' field	*/
	/* 'c' is always written into buffer already, or outputted	*/
	if( !(iop->_flag & _IOLBF) )
	{
		if( iop->_bufsiz )
			iop->_cnt = iop->_bufsiz-1;
		else
			iop->_cnt = BUFSIZ-1;	/* buffer will be filled with one character */
		*iop->_ptr++ = c;	/* write last character */
	}
	return( c );
}
