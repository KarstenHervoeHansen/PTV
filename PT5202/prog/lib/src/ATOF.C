/*
 *	Version : @(#)atof.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  atof.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for atof() routine                         *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>

double
atof( register const char *p )
{
	return( strtod( p, NULL ) );
}
