/*
 *	@(#)_lseek.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _lseek.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for _lseek() routine                       *
**                 Stores the current value of the file position          *
**                 indicator for the stream pointed by 'file' in the      *
**                 object pointed to by 'pos'.                            *
**                                                                        *
**  This routine should be customised.                                    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>

long
_lseek( FILE * iop, long offset, int whence )
{
	/* 'iop' is the file assigned to the file						*/
	/* 'whence' can contain :								*/
	/*	SEEK_SET, set the file pointer to 'offset' from the beginning of the file	*/
	/*	SEEK_CUR, set the file pointer to 'offset' from the current position		*/
	/*	SEEK_END, set the file pointer to 'offset' after the end of the file		*/
	/*											*/
	/* Flushing of the stream is already done, the only actions of this routine is to	*/
	/* place the filepointer to the requested position					*/
	/* and return the new position or EOF on error						*/

	iop; offset; whence;	/* suppress warnings 'parameter not used' */

	return( EOF );		/* error */
}
