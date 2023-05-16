/*
 *	Version : @(#)asin.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  asin.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for asin() routine                         *
**                 Computes the 'arcsinus' of the given value.            *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<math.h>
#include	<errno.h>

#define	PIO2	1.570796326794896619

double
asin( double arg )
{
	int	negative;
	double	temp;

	negative = 0;

	if( arg < 0.0 )
	{
		arg = -arg;
		negative = 1;
	}
	
	if( arg > 1.0 )
	{
		errno = EDOM;
		return( 0.0 );
	}

	temp = sqrt( 1.0 - arg * arg );
	if( arg > 0.7 )
		temp = PIO2 - atan( temp / arg );
	else
		temp = atan( arg / temp );

	if( !negative )
		return( temp );
	else
		return( -temp );
}
