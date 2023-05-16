/*
 *	Version : @(#)fwrite.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fwrite.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for fwrite() routine                       *
**                 Writes a number of objects to stdout                   *
**                                                                        *
**  REMARKS     :  fwrite is only allowed for stdout and stderr.          *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

size_t
fwrite( const void *ptr, size_t size, size_t nobj, FILE *stream )
{
	register size_t nr;
	register size_t objs;

	for( objs=0; objs < nobj; objs++ )
	{
		for( nr=0; nr <size; nr++ )
		{
			fputc( *(char *)ptr, stream );
			ptr = (char *)ptr + 1;
		}
		if( ferror( stream ) )
			break;
	}
	return( objs );
}
