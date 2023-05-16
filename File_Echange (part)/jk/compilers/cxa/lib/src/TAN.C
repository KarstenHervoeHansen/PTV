/*
 *	Version : @(#)tan.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  tan.c                                                  *
**                                                                        *
**  DESCRIPTION :  Source file for tan() routine                          *
**                 Computes the 'tangent' for the given angle (in radians)*
**                                                                        *
**      Coefficients double are #4285 from Hart & Cheney. (19.74D)        *
**      Coefficients single are #4282 from Hart & Cheney. ( 7.85D)        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>
#include <errno.h>

#define	INVPI	  1.27323954473516268
#ifndef _SINGLE_FP
#define	P0	 -0.1306820264754825668269611177e+5
#define	P1	  0.1055970901714953193602353981e+4
#define	P2	 -0.1550685653483266376941705728e+2
#define	P3	  0.3422554387241003435328470489e-1
#define	P4	  0.3386638642677172096076369e-4
#define	Q0	 -0.1663895238947119001851464661e+5
#define	Q1	  0.4765751362916483698926655581e+4
#define	Q2	 -0.1555033164031709966900124574e+3
#else
#define	P0	  0.21242445758263e3
#define	P1	 -0.1255329742424e2
#define	Q0	  0.27046722349399e3
#define	Q1	 -0.7159606050466e2
#endif

double
tan( double arg )
{
	double	x;
	char	sign;
	char	flag;

	flag = 0;
	sign = 1;
	if( arg < 0.0 )
	{
		arg = -arg;
		sign = -1;
	}
	arg = arg * INVPI;   /* overflow? */
	x = modf( arg, &arg );	/* From here we don't need the value of 'arg' anymore */
				/* So we use 'arg' for other purposes */
	switch( (int)arg % 4 )
	{
	case 1:
		x = 1.0 - x;
		flag = 1;
		break;
	case 2:
		sign = -sign;
		flag = 1;
		break;
	case 3:
		x = 1.0 - x;
		sign = -sign;
		break;
	}

	arg = x * x;
#ifndef _SINGLE_FP
	arg = ( ( ( ( ( P4 * arg + P3 ) * arg + P2 ) * arg + P1 ) * arg + P0 ) * x ) /
			( ( ( arg + Q2 ) * arg + Q1 ) * arg + Q0 );
#else
	arg = ( ( P1 * arg + P0 ) * x ) /
		( ( arg + Q1 ) * arg + Q0 );
#endif

	if( flag == 1 )
	{
		if( arg == 0.0 )
		{
			errno = ERANGE;
			if ( sign > 0 )
				return( HUGE_VAL );
			return( -HUGE_VAL );
		}
		arg = 1.0 / arg;
	}

	if( sign == 1 )
		return( arg );
	else
		return( -arg );
}
