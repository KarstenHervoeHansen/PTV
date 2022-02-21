/*
 *	@(#)strerror.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  strerror.c                                             *
**                                                                        *
**  DESCRIPTION :  Source file for strerror() routine                     *
**		   Maps the error number in 'errnum' to an error message  *
**                 string.                                                *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <errno.h>
#include <string.h>

char	*
strerror( int errcode )
{
	errcode;
	return( 0 );
}
