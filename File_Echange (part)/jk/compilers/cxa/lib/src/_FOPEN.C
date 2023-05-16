/*
 *	@(#)_fopen.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _fopen.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for _fopen() routine                       *
**                 Low level routine. Is used by the 'fopen()/freopen()'  *
**                 routines. This routine should be customised.           *
**                 The given stream should be properly opened.            *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>

int
_fopen( char * file, FILE * iop )
{
	/* 'file' points to the filename to open                        */
	/* 'iop' points to the new '_iob' structure                     */
	/*                                                              */
	/* At this point, the modes in which the file should be opened  */
	/* can be found in 'file->_flag'                                */
	/* On error, EOF should be returned                             */
	/* This routine may not change the buffering mode of the file	*/
	/* (by default, a file is opened fully buffered)		*/
	/* in the standard version, 'stdin', 'stdout' and 'stderr'	*/
	/* are the only, unbuffered streams, which are already open at  */
	/* startup.							*/

	file; iop;	/* suppress warning "parameter not used" */

	return( 1 );	/* return 'success' */
}
