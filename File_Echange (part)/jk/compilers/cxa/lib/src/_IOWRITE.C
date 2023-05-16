/*
 *	Version : @(#)_iowrite.c	1.4	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _iowrite.c                                             *
**                                                                        *
**  DESCRIPTION :  Source file for _iowrite() routine                     *
**                 Low level output routine. Is used by all printing      *
**                 routines.                                              *
**                 This routine is included in three versions. Selection  *
**		   of the used routine depends on the definition of the   *
**		   following macros:					  *
**		   _NOSIMIO	If defined a framework for _iowrite is	  *
**		                used that should be customized.           *
**				If not defined the simulated I/O feature  *
**				of the CrossView debugger is used. 	  *
**				(default)	  			  *
**		   _NONBUFFERED	This macro can be defined when simulated  *
**				I/O is used. When it is defined simulated *
**				I/O will not be buffered. This will slow  *
**				down the simulated I/O but is useful when *
**				e.g. binary I/O is done.                  *
**				If the macro is not defined (default) the *
**				simulated I/O is buffer in an 80 	  *
**				character wide line buffer.		  *
**									  *
**                 On entry of _iowrite, it is known that 'fout' is a	  *
**                 file opened for writing, and is not in error yet.	  *
**                 This routine should write the output for the given	  *
**                 file handle (in the standard version only 'stdout' or  *
**                 'stderr') to the required hardware.			  *
**                 Whenever an error occurs, 'EOF' should be returned	  *
**                 and fout->_flag should be set to the proper error	  *
**                 value						  *
**                 i.e.		_IOEOF on end of FILE			  *
**                 		_IOERR on any other error		  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>
#include <simio.h>


#if _NOSIMIO
/*
 * Framework for _iowrite function.
 */
int
_iowrite( int c, FILE * fout )
{
	c; fout;	/* prevent warning 'parameter not used' */

	return( EOF );	/* return EOF on error ('c' when correct) */
}

#else /* !_NOSIMIO */ 


/* when using simulated output with CrossView 
 *
 * CrossView needs to know the stream number, therefore
 *  the pointer is converted to the stream number.
 *  The following stream numbers are already in use:
 *    0  stdin
 *    1  stdout
 *    2  stderr
 *  255  reserved
 */

#ifdef _NONBUFFERED

/*
 * _iowrite for non buffered simulated I/O
 */
int
_iowrite( int c, FILE * fout )
{

	_simo( fout->_file, (char*)&c, 1 );

        return( c );    /* return written character on success */
}

#else  /* !_NONBUFFERED */

#define _IOBUFSIZE	80

static int prv_sr;

/*
 * _iowrite for line buffered simulated I/O
 *
 * Notes:
 * o The third argument of _simo() must be the same for each call,
 *   because xvw caches this information.
 * o Output to stdout and stderr is buffered in the same buffer. This
 *   will cause problems when writing to both streams at the same time.
 */
int
_iowrite( int c, FILE * fout )
{
	static	char	outbuf[_IOBUFSIZE+1];
	static	int	i;

	prv_sr = c;	/* just use it */

	if ( fout == stdout  ||  fout == stderr )
	{
		outbuf[i++] = c;
		if ( c == '\n' || i == _IOBUFSIZE )
		{	
			outbuf[i] = '\0';
			_simo( fout->_file, outbuf, _IOBUFSIZE );
			i = 0;
		}
	}
	else
	{
		_simo( fout->_file, (char*)&c, 1 );
	}

        return( c );    /* return written character on success */
}



#endif /* _NONBUFFERED */
#endif /* _NOSIMIO */
