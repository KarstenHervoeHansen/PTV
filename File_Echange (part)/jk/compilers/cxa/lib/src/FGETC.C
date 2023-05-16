/*
 *	Version : @(#)fgetc.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fgetc.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for fgetc() routine                        *
**                 Reads one character from input.                        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>

int
fgetc( FILE *stream )
{
	/* Read on string is treated correct in 'getc' too, so we do not */
	/* do it here							 */
	return( getc( stream ) );
}
