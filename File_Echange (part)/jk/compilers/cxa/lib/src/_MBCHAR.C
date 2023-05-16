/*
 *	Version : @(#)_mbchar.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _mbchar.c						  *
**                                                                        *
**  DESCRIPTION :  Skeletons for multibyte character functions.		  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>

int
mblen( const char *s, size_t n )
{
	return( 0 );
}

int
mbtowc( wchar_t *pwc, const char *s, size_t n )
{
	return( 0 );
}

int
wctomb( char *s, wchar_t wchar )
{
	return( 0 );
}

size_t
mbstowcs( wchar_t *pwcs, const char *s, size_t n )
{
	return( 0 );
}

size_t
wcstombs( char *s, const wchar_t *pwcs, size_t n )
{
	return( 0 );
}
