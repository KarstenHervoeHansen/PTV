/*
 *	@(#)tmpfile.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  tmpfile.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for tmpfile() routine                      *
**                 Creates a 'unique' file.                               *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdio.h>
#include <string.h>

int	rand( void );

FILE *
tmpfile( void )
{
	/* 's' buffer for temporary file name		*/
	/* NOTE: the created file MUST be removed	*/
	/* when the program terminates normally		*/
	/* F.e. add some 'remove' routine to the 	*/
	/* atexit() list				*/

	static	char	tmpname[ L_tmpnam ];

	tmpnam( tmpname );

	return( fopen( tmpname, "wb+" ) );
}
