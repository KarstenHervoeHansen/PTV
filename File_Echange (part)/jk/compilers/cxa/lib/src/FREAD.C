/*
 *	Version : @(#)fread.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fread.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for fread() routine                        *
**                 Reads a number of objects from the given stream        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

size_t
fread( void *ptr, size_t size, size_t nobj, FILE *stream )
{
	register size_t nr;
	register size_t objs;

	for( objs=0; objs < nobj; objs++ )
	{
		for( nr=0; nr <size; nr++ )
		{
			*(char *)ptr = (char)fgetc( stream );
			if( *(char *)ptr == EOF )
				return( objs );
			ptr = (char *)ptr + 1;
		}
	}
	return( objs );
}
