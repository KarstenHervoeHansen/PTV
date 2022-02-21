/*
 *	Version : @(#)rand.c	1.6	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  rand.c                                                 *
**                                                                        *
**  DESCRIPTION :  Source file for rand()  and srand() routines           *
**                 Pseudo random routines with reproducable results.      *
**                                                                        *
**  REMARKS     : The algorithm used is defined by the ANSI DRAFT         *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <stdlib.h>

static unsigned long	next = 1;

int
rand( void )
{
	next = next * 1103515245 + 12345;
	return (unsigned int)(next/65536) % 32768;
}

void
srand( unsigned int seed )
{
	next = seed;
}
