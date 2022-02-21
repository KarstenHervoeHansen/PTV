/*
 *	Version : @(#)log10.c	1.4	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  log10.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for log10() routine                        *
**                 Computes the 'logarithm' for the given value           *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <errno.h>
#include <math.h>

#define	LN10	 2.302585092994045684

double
log10( double arg )
{
	double	log_result;

	log_result	= log( arg );

	/*
	 * Here we can't test on errno == ERANGE since errno is not reset
	 * on success. Also do not reset errno here first, since that
	 * is not expected behavior of a math function.
	 */
	if ( log_result == -HUGE_VAL )
	{
		return log_result;
	}

	return log_result / LN10;
}
