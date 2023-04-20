/*
 *	Version : @(#)_ioread.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _ioread.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for _ioread() routine                      *
**                 Low level input routine. Used by all scanning routines *
**                 This routine is included in three versions. Selection  *
**		   of the used routine depends on the definition of the   *
**		   following macros:					  *
**		   _NOSIMIO	If defined a framework for _ioread is	  *
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
**                 On entry of _ioread, it is known that 'fin' is a file  *
**		   opened for reading, and is not in error yet.		  *
**                 This routine should read the input for the given 	  *
**		   file handle (in the standard version only 'stdin') from*
**		   the required hardware.				  *
** 		   Whenever an error occurs, 'EOF' should be returned     *
** 		   and fin->_flag should be set to the proper error value *
** 		   i.e.		_IOEOF on end of FILE			  *
** 				_IOERR on any other error		  *
** 		   Note that when a read is done on 'stdin', this routine *
** 		   should do the scanning, and whenever necessary, do the *
** 		   echoing to stdout.					  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
#include <simio.h>
#include <string.h>

#ifdef _NOSIMIO
/*
 * Framework for _ioread function.
 */
int
_ioread( FILE * fin )
{
	register int c;

	/* read character into c */
	
	if( fin == stdin )
	{	/* when reading 'stdin', echo to 'stdout' */
		_iowrite( c, stdout );
	}

	return( c );	/* return read character on success */
}

#else /* !_NOSIMIO */

/* When using simulated input with CrossView:
 *
 *  CrossView needs to know the stream number, therefore
 *  the pointer is converted to the stream number.
 *  The following stream numbers are already in use:
 *    0  stdin
 *    1  stdout
 *    2  stderr
 *  255  reserved
 */

#ifdef _NONBUFFERED

/*
 * _ioread for non buffered simulated I/O
 */
int
_ioread( FILE * fin )
{
	int inbuf = EOF;

	_simi( fin->_file, (char*)&inbuf, 1 );

	return( inbuf );
}



#else 	/* !_NONBUFFERED */

#define _IOBUFSIZE	80

static int	prv_sr;

/*
 * _ioread for line buffered simulated I/O
 */
int
_ioread( FILE * fin )
{
	static	char	inbuf[_IOBUFSIZE];
	static	int	cnt = 0;
	static	char	*p;
	int	c = EOF;

	prv_sr = c;	/* just use it */

        if ( fin == stdin )
        {
		if ( !cnt )
		{
			inbuf[0] = 0;	// xvw will not clear buffer on EOF
			_simi( fin->_file, inbuf, _IOBUFSIZE );
			p = inbuf;
			cnt = strlen(inbuf);
		}
		if ( cnt > 0 )
		{
			c = *p++;
			cnt--;
		}
        }
	else
	{
		_simi( fin->_file, (char*)&c, 1 );
	}

	return ( c );
}

#endif  /* _NONBUFFERED */
#endif	/* _NOSIMIO */
