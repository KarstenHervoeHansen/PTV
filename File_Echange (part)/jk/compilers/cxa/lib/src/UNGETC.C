/*
 *	Version : @(#)ungetc.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  ungetc.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for _ungetc() routine                      *
**                 pushes one character from given stream back.           *
**                 returns EOF when unget is not possible                 *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>

extern int _ungetc[];	/* defined in _iob.c */

int
ungetc( int c, FILE * fin )
{
	if ( c == EOF )
		return( EOF );

	if ( (fin->_flag & _IOSTRG) )
	{
		/* We are not able to check if we pass
		 * the beginning of the real string,
		 * but if the character is right,
		 * assume 'ungetc()' is correct.
		 */
		if( *(fin->_ptr - 1) != c )
			return( EOF );		/* Failure */

		fin->_ptr--;	/* Pointer one position back		*/
		fin->_cnt++;	/* One more character in the string	*/
		return( c );	/* OK					*/
	}

	/* unbuffered I/O, try the '_ungetc' array */
	if ( (fin->_flag & _IONBF) )
	{
		if( _ungetc[ fileno( fin ) ] )
			return( EOF );	/* impossible to do an 'ungetc' */
		_ungetc[ fileno( fin ) ] = c + 1;
			/* otherwise we cannot push back a 'NULL' byte */
		return( c );
	}

	if ( (fin->_flag & _IOREAD)==0 || fin->_ptr <= fin->_base )
	{	/* Not open for reading, or empty/incorrect buffer */
		if ( fin->_ptr == fin->_base && fin->_cnt==0 )
		{	/* empty buffer */
			fin->_ptr++;
		}
		else
			return( EOF );
	}

	fin->_cnt++;
	*--fin->_ptr = c;
	fin->_flag &= ~_IOEOF;	/* ungetc successfull, clear EOF indicator */
	return( c );
}
