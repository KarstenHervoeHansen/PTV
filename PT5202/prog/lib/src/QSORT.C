/*
 *	Version : @(#)qsort.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  qsort.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for qsort() routine                        *
**                 Sorts an array of objects in ascending order.          *
**                 Afterwards this array can be used by the routine       *
**                 bsearch.                                               *
**                 A comparison routine should be given by the user.      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

/* qsort routine (ANSI) */
#include <stdlib.h>

/* Swap two elements of the 'base' array, each element is 'size' bytes large */
static void
swap( void *base, size_t sizep, int first, int second )
{
	register char	temp;
	register char	* one;
	register char	* two;
	register size_t	size = sizep;		/* To speed up access to the value */

	one = (char *)(base) + first*size;	/* pointer to 'first' element in 'base' array */
	two = (char *)(base) + second*size;	/* pointer to 'second' element in 'base' array */

	while( size-- > 0 )
	{
		temp = one[size];
		one[size] = two[size];
		two[size] = temp;
	}
}

/* The qsort routine */
void
qsort( void *base, size_t np, size_t sizep, int (*cmp)(const void *, const void *) )
{
	register int	i;
	register int	last;
	register size_t	size = sizep;	/* To speed up access to the value */
	register size_t	n = np;		/* To speed up access to the value */

	if( n <= 1 )	/* Do nothing when array smaller than two elements */
		return;

	swap( base, size, 0, n/2 );
	last = 0;
	for( i=1; i<n; i++ )
		if( cmp( ((char *)base)+i*size, (char *)base ) < 0 )
			swap( base, size, ++last, i );
	swap( base, size, 0, last );
	qsort( base, last, size, cmp );
	qsort( (char *)(base)+size*(last+1), n-(last+1), size, cmp );
}
