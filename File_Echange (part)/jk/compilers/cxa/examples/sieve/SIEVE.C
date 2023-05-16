/*	@(#)sieve.c	1.2
 *
 * Eratosthenes Sieve Prime Number Program in C
 * Byte, januari 1983
 *
 */

/*
 * Compile with -DPRINT for result printing.
 *  
 * Compile with -DMEASURE_TIME for time measurement.
 *
 */

#include <stdio.h>

#ifdef MEASURE_TIME
#include "time.h"
unsigned	Begin_Time,End_Time;
#endif

#define	true	1
#define	false	0
#define	size	8190

char flags[size + 1];

void
main( void )
{
	int	i,prime,k,count,iter;
	
#ifdef PRINT
	printf( "10 iterations\n" );
#ifdef MEASURE_TIME
	Begin_Time = time ( NULL );
#endif
#endif
	for( iter = 1; iter <= 10; iter++ )	/* do program 10 times	*/
	{
		count = 0;			/* prime counter	*/
		for( i = 0; i <= size; i++ )	/* set all flags true	*/
			flags[i] = true;
		for( i = 0; i <= size; i++ )
		{
			if( flags[i] )		/* found a prime	*/
			{
				prime = i + i + 3; /* twice index + 3	*/
				for(k=i+prime; k<=size; k+=prime)
					flags[k] = false;
						/* kill all multiples	*/
				count++;	/* primes found		*/
			}
		}
	}
#ifdef PRINT
#ifdef MEASURE_TIME
	End_Time = time ( NULL );
#endif
	printf( "%d primes.\n", count ); /* primes found on 10th pass	*/
#ifdef MEASURE_TIME
	printf( "Time: %d msec.\n", End_Time - Begin_Time );
#endif
#endif
}







