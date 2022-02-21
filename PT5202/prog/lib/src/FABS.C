/*
 *	Version : @(#)fabs.c	1.5	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  fabs.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for fabs() routine                         *
**                 Returns the absolute value of the given 'double'       *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<math.h>
#include	<float.h>

double	fabs( double arg )
{
#if _SINGLE_FP
	return copysignf( arg, 1.0 );
#else
	return copysign( arg, 1.0 );
#endif
}
