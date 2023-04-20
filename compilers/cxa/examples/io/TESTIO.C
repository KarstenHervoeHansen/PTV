/*
 *      Version : @(#)testio.c	1.3	25 Sep 1996
 */

/**************************************************************************
**                                                                        *
**  FILE        :  testio.c                                               *
**                                                                        *
**  DESCRIPTION :  Program to test _ioread() and _iowrite()               *
**                 implementation.                                        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdio.h>

#define	MXLINE	80
static	char	line[ MXLINE ];

void
main( void )
{
	char	c;
	int	i;
	long	l;
	int	yn;

	do
	{
		printf( "Please enter a (decimal) number:\n" );
		fgets( line, MXLINE, stdin );
		if ( sscanf( line, "%ld", &l ) != 1 )
		{
			printf( "Invalid input ...\n" );
			continue;
		}
		printf( "Number: %ld\n", l );

		printf( "Please enter characters:\n" );
		for ( i = 0; (i < MXLINE-1) && (c = getchar()) != '\n'; i++ )
		{
			line[i] = c;
		}
		line[i] = '\0';
		printf( "Input was: \"%s\"\n", line );

		printf( "Again ? (y/n)\n" );
		yn = getchar();
		getchar();				// read '\n'
	}
	while ( yn == 'y' );
	

	printf( "End of program.\n" );
}
