/*
 *	Version : @(#)_open.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _open.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for _open() routine                        *
**                 Opens a file in given mode.                            *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

FILE *
_open( char * file, char * mode, FILE * iop )
{
	if ( iop == NULL )
		return( NULL );

	while( *mode )
	{
		switch ( *mode )
		{
		case 'w':
		case 'a':
			/* NOTE : the append mode is treated as a normal */
			/*        write mode                             */
			iop->_flag |= _IOWRT;
			break;

		case 'r':
			iop->_flag |= _IOREAD;
			break;
	
		case 'b':	/* No difference between 'binary' and 'text' */
				/* modes */
		case '+':
			break;
		default:
			return( NULL );
		}
		if( (iop->_flag & (_IOWRT|_IOREAD)) == (_IOWRT|_IOREAD) )
			iop->_flag |= _IORW;

		mode ++;
	}

	iop->_cnt = 0;
	iop->_ptr = NULL;
	iop->_base = NULL;
	/* iop->_file was already filled */

	if ( _fopen( file, iop ) == EOF )
	{
		iop->_flag = 0;
		return( NULL );
	}

	return( iop );
}
