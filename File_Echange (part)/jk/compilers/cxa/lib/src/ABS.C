/*
 *	Version : @(#)abs.c	1.4
 */

/**************************************************************************
**                                                                        *
**  FILE          :  _abs.c                                               *
**                                                                        *
**  DESCRIPTION   :  Source file for abs() routine                        *
**                   returns absolute value of signed integer argument    *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdlib.h>

int
abs( int arg )
{
	if ( arg < 0 )
		arg = -arg;

	return( arg );
}
