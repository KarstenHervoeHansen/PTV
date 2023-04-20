/*
 *	Version : @(#)floor.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  floor.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for floor() routine                        *
**                 Rounds value to the largest integer value not greater  *
**                 than the given value                                   *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<math.h>

double
floor( double d )
{
	if ( d < 0.0 )
	{
		if( modf( -d, &d ) != 0.0 )	/* When fractional part is not 0.0 */
			d += 1.0;		/* Round up */
		d = -d;
	}
	else
		modf( d, &d );

	return( d );
}
