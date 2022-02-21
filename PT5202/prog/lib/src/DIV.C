/*
 *	Version : @(#)div.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  div.c                                                  *
**                                                                        *
**  DESCRIPTION :  Source file for div() routine                          *
**                 Returns a structure containing the quotient and        *
**                 remainder of num divided by denom                      *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>

div_t
div( int numer, int denom )
{
	div_t result;

	result.quot = numer / denom;
	result.rem  = numer % denom;
	return( result );
}
