/*
 *	Version : @(#)bsearch.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _bsearch.c                                             *
**                                                                        *
**  DESCRIPTION :  Source file for bsearch() routine                      *
**                 Searches an array for a matching structure.            *
**                 A comparison routine should be given by the user.      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>

void *
bsearch( const void *key, const void *base, size_t n, size_t size,
	 int (*cmp)( const void *keyval, const void *datum )  )
{
	register int result;

	if( n==0 )			/* not found */
		return( NULL );

	result = cmp( key, (char *)(base)+size*(n/2) );
	if( result == 0 )	/* found one */
		return( (char *)(base)+size*(n/2) );
	if( result < 0 )
		return( bsearch( key, base, n/2, size, cmp ) );
	/* result > 0 */
	return( bsearch( key, (char *)(base)+size*((n/2)+1), n-((n/2)+1), size, cmp ) );
}

