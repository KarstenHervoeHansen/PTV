/*
 *	Version : @(#)cos.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  cos.c                                                  *
**                                                                        *
**  DESCRIPTION :  Source file for cos() routine                          *
**                 Computes the 'cosinus' for the given angle (in radians)*
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>

extern double _sinus( double, int );

double
cos( double arg )
{
	if( arg < 0.0 )
		arg = -arg;

	return( _sinus( arg, 1 ) );
}
