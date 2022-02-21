/*
 *	@(#)_filbuf.c	1.4
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _filbuf.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for _filbuf() routine                      *
**                 This routine makes sure the buffering of file I/O will *
**                 work as expected.                                      *
**                 returns 'EOF' on error                                 *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

extern int _ungetc[];	/* defined in _iob.c */

int
_filbuf( FILE * fin )
{
	unsigned int i;

	if ( fin->_flag & _IORW )	/* Make sure the 'read' flag is set */
		fin->_flag |= _IOREAD;

	if ( (fin->_flag & _IOREAD) == 0 || fin->_flag & _IOSTRG )
	{	    
	        fin->_cnt = 0;
		return( EOF ); /* when reading a string, string is empty ! */
	}	

tryagain:
	if ( fin->_base == NULL )	/* No buffer assigned to stream */
	{
		if ( fin->_flag & _IONBF )
		{	/* unbuffered I/O, just read one character */
			/* Maybe the value is in '_ungetc' */
			int smallbuf;

			if ( _ungetc[ fileno( fin ) ] )
			{
				smallbuf = _ungetc[ fileno( fin ) ] - 1;
				_ungetc[ fileno( fin ) ] = 0;
			}
			else
				smallbuf = _ioread( fin );

			if ( smallbuf > 0 )
				return( smallbuf & 0377 );

			if ( smallbuf == EOF )
			{
				fin->_flag |= _IOEOF;	/* EOF found */
				/* Close the 'read' mode */
				if ( fin->_flag & _IORW )
					fin->_flag &= ~_IOREAD;
			}
			else
				fin->_flag |= _IOERR;

			fin->_cnt = 0;
			return( EOF );			/* Error */
		}
		if ( fin->_base == NULL )
		{		/* Buffered mode, but no buffer assigned ? */
			fin->_flag |= _IONBF;
			goto tryagain;
		}
	}

	fin->_ptr = fin->_base;				/* Reset buffer */

	/* Fill the buffer */
	if( fin->_bufsiz )
		i = fin->_bufsiz;
	else
		i = BUFSIZ;
	if( fin->_flag & _IOLBF )	/* line bufferring ? */
	{
		fin->_cnt = 0;
		for( ; i > 0; i-- )
		{
			int c;
			if( (c = _ioread( fin ) ) == EOF )
				break;
			fin->_cnt ++;
			*fin->_ptr++ = c;
			/* maximum fill when eol is found */
			if( c == '\n' )
				break;
		}
		fin->_ptr = fin->_base;				/* Reset buffer again */
	}
	else	/* Full buffering */
	{
		fin->_cnt = _read( fin, fin->_base, i );
	}

	if ( --fin->_cnt < 0 )		/* Still nothing in the buffer ? */
	{
		if ( fin->_cnt == EOF )
		{
			fin->_flag |= _IOEOF;		/* EOF found */
			/* Close the 'read' mode */
			if ( fin->_flag & _IORW )
				fin->_flag &= ~_IOREAD;
		}
		else
			fin->_flag |= _IOERR;

		fin->_cnt = 0;
		return( EOF );				/* Error */
	}

	/* Return first character in the buffer */
	/* '_cnt' was already decremented */
	return( *fin->_ptr++ & 0377 );
}
