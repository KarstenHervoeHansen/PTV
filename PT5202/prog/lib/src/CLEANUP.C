/*
 *	Version : @(#)cleanup.c	1.4	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  cleanup.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for cleanup() routine                      *
**                 Flushes all streams, and closes all streams            *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<stdio.h>

void
_cleanup( void )
{
	register int	i;

	for( i = 0; i < _NFILE; i++ )
		fclose( &_iob[i] );
}
