/*
 *	Version : @(#)sin.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  sin.c                                                  *
**                                                                        *
**  DESCRIPTION :  Source file for sin() routine                          *
**                 Computes the 'sinus' for the given angle (in radians)  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>

extern double _sinus( double, int );

double
sin( double arg )
{
	return( _sinus( arg, 0 ) );
}
