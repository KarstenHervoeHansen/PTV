/*
 *	Version : @(#)tolower.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  tolower.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for tolower() routine                      *
**                 Converts given string to lower case.                   *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <ctype.h>


int
tolower( int c )
{
	return( isupper( c ) ? _tolower( c ) : c );
}
