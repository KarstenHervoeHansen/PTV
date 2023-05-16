/*
 *	Version : @(#)log.c	1.5
 */

/**************************************************************************
**                                                                        *
**  FILE        :  log.c                                                  *
**                                                                        *
**  DESCRIPTION :  Source file for log() routine                          *
**                 Computes the 'natural logarithm' for the given value   *
**                                                                        *
**        The double coefficients are #2704 from Hart & Cheney. (16.65D)  *
**        The single coefficients are #2701 from Hart & Cheney. ( 8.48D)  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <errno.h>
#include <math.h>

#define	LOG2	 0.693147180559945309e0
#define	SQRTO2	 0.707106781186547524e0
#ifndef _SINGLE_FP
#define	P0	-0.90174691662040536328986e2
#define	P1	 0.934639006428585382474e2
#define	P2	-0.18327870372215593212e2
#define	Q0	-0.45087345831020305748486e2
#define	Q1	 0.61761065598471302843e2
#define	Q2	-0.20733487895513939345e2
#else
#define	P0	-0.331355617479e1
#define	P1	 0.89554061525e0
#define	Q0	-0.165677797691e1
#endif

double
log( double arg )
{
	double	zsq;
	int	exp;

	if( arg <= 0.0 )
	{
	        if ( arg == 0.0 )
		{		    
		    errno = ERANGE;
		}
		else
		{
		    errno = EDOM;
		}				    
		return( -HUGE_VAL );
	}
	arg = frexp( arg, &exp );
	if( arg < SQRTO2 )
	{
		arg = 2 * arg;
		exp = exp - 1;
	}

	arg = ( arg - 1 ) / ( arg + 1 );
	zsq = arg * arg;

#ifndef _SINGLE_FP
	return(
		(
			( ( P2 * zsq + P1 ) * zsq + P0 ) /
			( ( ( zsq + Q2 ) * zsq + Q1 ) * zsq + Q0 )
		) * arg + exp * LOG2
	       );
#else
	return(
		(
			( P1 * zsq + P0 ) /
			( zsq + Q0 )
		) * arg + exp * LOG2
	       );
#endif	       
}
