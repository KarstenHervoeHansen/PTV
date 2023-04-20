/*
 *	Version : @(#)atan2.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  atan2.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for atan2() routine                        *
**                 Computes the 'arctangent' for the given value          *
**                 (return value in radians)                              *
**	           argument in the range [-pi,pi].                        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>

#define	PIO2	1.570796326794896619231e0
extern double _atan( double );

double
atan2( double arg1, double arg2 )
{
	if( ( arg1 + arg2 ) == arg1 )
	{
		if( arg1 >= 0.0 )
			return( PIO2 );
		else
			return( -PIO2 );
	}
	else if( arg2 < 0.0 )
	{
		if( arg1 >= 0.0 )
			return( PIO2 + PIO2 - _atan( -arg1 / arg2 ) );
		else
			return( -PIO2 - PIO2 + _atan( arg1 / arg2 ) );
	}
	else if( arg1 > 0.0 )
	{
		return( _atan( arg1 / arg2 ) );
	}
	else
	{
		return( -_atan( -arg1 / arg2 ) );
	}
}
