/*
 *	Version : @(#)labs.c	1.4
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _labs.c                                                *
**                                                                        *
**  DESCRIPTION :  Source file for labs() routine                         *
**                 Returns the abs value of the signed long argument.     *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <stdlib.h>

long
labs( long arg ) 
{
	if ( arg < 0 )
		arg = -arg;

	return( arg );
}
