/*
 *	@(#)assert.h	1.2
 */

/**************************************************************************
**                                                                        *
**  FILE        :  assert.h                                               *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes and macros to add         *
**                 diagnostics to programs                                *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#undef assert

#ifndef	_STDIO_H
#include <stdio.h>	/* prototype for 'printf' */
#endif  /* _STDIO_H */
#ifndef	_STDLIB_H
#include <stdlib.h>	/* prototype for 'exit' */
#endif  /* _STDLIB_H */

#ifndef NDEBUG

#define assert( _expr )								\
	( (void)( !(_expr) ?							\
	  printf( "Assertion failed: (" #_expr ") file %s, line %d\n",		\
	  	   __FILE__, __LINE__ ),exit( 1 ) : (void)0			\
	))

#else

#define	assert( _expr )		((void)0)

#endif  /* NDEBUG */
