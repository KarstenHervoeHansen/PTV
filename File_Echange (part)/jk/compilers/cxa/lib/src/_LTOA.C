/*
 *	Version : @(#)_ltoa.c	1.4	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _ltoa.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for _ltoa() routine                        *
**                 This routine converts the given long argument to an    *
**                 ascii string (the buffer is given as an argument).     *
**                 This routine is used by _doprint() routines.           *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <limits.h>


char *
_ltoa( char *p, unsigned long num, unsigned char radix )
{
	register unsigned int i;
	register char *q;
	register char *hold;

	hold = p;
	q = p + 15 + 2;

	*--q = '\0';		/* be sure to terminate the string with a NULL character	*/

	do
	{
		i = (num % radix) + '0';
		if( i > '9' )
			i+='A'-'0'-10;
		*--q = i;
	} while( num /= radix );

	for( ; (*p++ = *q++); )	/* move the string to the left, including the NULL character	*/
		;

	return( hold );
}
