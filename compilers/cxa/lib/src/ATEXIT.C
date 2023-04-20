/*
 *	Version : @(#)atexit.c	1.3	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  atexit.c                                               *
**                                                                        *
**  DESCRIPTION :  Source file for atexit() routine.                      *
**                 Registers the functions (with a maximum of 32)         *
**                 which should be executed on 'exit'.                    *
**                 The routine '_doexit()' is used by the 'exit()'        *
**                 routine to call all necessary functions.               *
**                 The library module exit.asm has to be assembled with   *
**                 -DDO_ATEXIT to actually call the function.             *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>

#define	MAXFUNC		32	/* Nr of functions which can be executed at 'exit' (ANSI --> 32) */
typedef	void (* FUNCPTR)( void );

static unsigned char	_atexitnr = 0;
static FUNCPTR		_atexitarr[MAXFUNC];

/* Store the routine to execute at 'exit' */
int
atexit( FUNCPTR ptr )
{
	if( _atexitnr == MAXFUNC )
		return( 1 );

	_atexitarr[_atexitnr++] = ptr;
	return( 0 );
}

/* Execute all stored functions, called by 'exit()' */
void
_doexit( void )
{
	register unsigned char i;

	for( i = _atexitnr; i > 0; i-- )
	{
		_atexitarr[i-1]();	/* Call the stored function */
	}
}


