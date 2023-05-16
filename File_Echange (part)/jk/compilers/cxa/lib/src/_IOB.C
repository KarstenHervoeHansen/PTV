/*
 *	Version : @(#)_iob.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _iob.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file with _iob structure definitions            *
**                 Default, only the streams 'stdin', 'stdout' and        *
**                 'stderr' are supported (unbuffered).                   *
**                 When buffered I/O, or more than the supported streams  *
**                 are neccessary, the size of the '_iob' array should    *
**                 be changed. Also the buffering options should be       *
**                 changed to the users needs. Note that all I/O will     *
**                 pass the routines '_ioread'/'_iowrite'                 *
**                 Also space for 'unbuffered ungetc()' is made           *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

FILE	_iob[_NFILE] = {
	{ NULL, 0, NULL, _IOREAD+_IONBF, 0, NULL },	/* stdin   */
	{ NULL, 0, NULL, _IOWRT +_IONBF, 1, NULL },	/* stdout  */
	{ NULL, 0, NULL, _IOWRT +_IONBF, 2, NULL },	/* stderr  */
	{ NULL, 0, NULL, 0,              3, NULL },	/* stduser */
};

int	_ungetc[_NFILE];			/* For all files   */


