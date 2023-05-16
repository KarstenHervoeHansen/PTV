/*
 *	Version : @(#)ceil.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  ceil.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for ceil() routine                         *
**                 Rounds the value to the nearest integer value, not     *
**                 less than the given value                              *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include	<math.h>

double
ceil( double d )
{
	return( -floor( -d ) );
}
