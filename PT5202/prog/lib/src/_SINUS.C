/*
 *	Version : @(#)_sinus.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _sinus.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for _sinus() routine                       *
**                 Does the actual computation of the 'sin()' and 'cos()' *
**                 routines                                               *
**                                                                        *
**	Coefficients double are #3368 from Hart & Cheney (16.25D).        *
**	Coefficients single are #3362 from Hart & Cheney ( 8.28D).        *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <math.h>

#ifndef _SINGLE_FP
#define	TWOOPI	 0.63661977236758134308
#define	P0	 0.2078236841696101220215102e6
#define	P1	-0.765864156388469493709081e5
#define	P2	 0.70641360814006880884734e4
#define	P3	-0.2378593245781215847583e3
#define	P4	 0.28078274176220686085e1
#define	Q0	 0.1323046665086493066402123e6
#define	Q1	 0.5651686795316917682421e4
#define	Q2	 0.1089998110371290528265e3
#else
#define	TWOOPI	 0.63661977236758134308
#define	P0	 0.52930152776255e3
#define	P1	-0.17389497132272e3
#define	P2	 0.1042302058487e2
#define	Q0	 0.33696381989527e3
#define	Q1	 0.2786575519992e2
#endif

double
_sinus( double x, int quad )
{
	double	f;
	double	y;
	int	k;

	if( x < 0.0 )	/* Make sure X gets a positive number */
	{
		x = -x;
		quad = quad + 2;
	}

	x = x * TWOOPI;		/* underflow ? */

	if( x > 32764.0 )
	{
		y = modf( x, &f );
		/* From here, we don't need the original 'x' anymore
		 * So we just use it for other purposes
		 */
		x = f;
		x = x + quad;
		modf( 0.25 * x, &f );
		quad = x - 4.0 * f;
	}
	else
	{
		k = x;
		y = x - k;
		quad = ( quad + k ) & 03;
	}
	if ( quad & 0x01 )	/* Quadrant 1 or 3 */
		y = 1 - y;
	if( quad > 1 )
		y = -y;

	x = y * y;	/* y-square */

#ifndef _SINGLE_FP
	return(
		( ( ( ( ( P4 * x + P3 ) * x + P2 ) * x + P1 ) * x + P0 ) * y ) /
			( ( ( x + Q2 ) * x + Q1 ) * x + Q0 )
	      );
#else	  
	return( 
		( ( ( P2 * x + P1 ) * x + P0 ) * y ) /
		( ( x + Q1 ) * x + Q0 )
	);
#endif	      
}
