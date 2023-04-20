/*
 *	@(#)_read.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _read.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for _read() routine                        *
**                 Reads a block of characters from the given stream      *
**                 returns nr of read characters                          *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

size_t
_read( FILE * fin, char * base, size_t size )
{
	int	c;
	size_t	cnt = 0;

	/* At this point, it is known that 'fin' is a file opened for	*/
	/* reading, and is not in error yet.				*/
	/* This routine should read the input for the given filehandle  */
	/* from the required hardware.					*/
	/* when nothing is read, '0' should be returned, meaning EOF    */
	/* is found                                                     */
	/* On any other error, the routine should return non zero       */
	/* Note that when a read is done on 'stdin', this routine       */
	/* should do the scanning, and whenever neccessary, do the      */
	/* echoing to stdout                                            */

	for( ; size; size--,cnt++ )
	{
		/* _ioread will echo to stdout when neccessary */
		if( (c = _ioread( fin )) == EOF )
			break;	/* Nothing more read */
		*base++ = c;
	}

	return( cnt );
}
