/*
 *	Version : @(#)acos.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  acos.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for acos() routine                         *
**                 Computes the 'arccosinus' of the given value.          *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<math.h>
#include	<errno.h>

#define	PIO2	1.570796326794896619

double
acos( double arg )
{
	if( ( arg > 1.0 ) || ( arg < -1.0 ) )
	{
		errno = EDOM;
		return( 0.0 );
	}

	return( PIO2 - asin( arg ) );
}
