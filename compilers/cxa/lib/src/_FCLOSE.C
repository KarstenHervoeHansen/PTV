/*
 *	@(#)_fclose.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _fclose.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for _fclose() routine                      *
**                 Low level routine. Is used by the 'fclose()' routine.  *
**                 This routine should be customised.                     *
**                 The given stream should be properly closed, any buffer *
**                 is already flushed.                                    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>

int
_fclose( FILE * file )
{
	/* At this point, it is known that 'file' is already flushed    */
	/* This routine should do whatever neccessary to close the given*/
	/* stream.                                                      */
	/* (in the standard version, 'stdin', 'stdout' and 'stderr'     */
	/* are the only, unbuffered streams).                           */
	/* Whenever an error occurs, 'EOF' should be returned		*/
	/* and fout->_flag should be set to the proper error value	*/
	/* i.e.		_IOEOF on end of FILE				*/
	/*		_IOERR on any other error			*/

	file;		/* suppress warning "parameter not used" */

	return( 0 );	/* return 'success' */
}
