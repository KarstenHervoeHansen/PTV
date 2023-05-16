/*
 *	Version : @(#)toupper.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  toupper.c                                              *
**                                                                        *
**  DESCRIPTION :  Source file for toupper() routine                      *
**                 Converts given character to uppercase.                 *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <ctype.h>


int
toupper( int c )
{
	return( islower( c ) ? _toupper( c ) : c );
}
