/*
 *	Version : @(#)atan.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  atan.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for atan() routine                         *
**                 Computes the 'arctangent' for the given value          *
**                 (return value in radians)                              *
**	           argument in the range [-pi/2,pi/2].                    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>

extern double _atan( double );

/*
	atan makes its argument positive and
	calls the inner routine _atan.
*/
double
atan( arg )
double arg;
{
	if( arg > 0.0 )
		return( _atan( arg ) );
	else
		return( -_atan( -arg ) );
}
