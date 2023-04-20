/*
 *	Version : @(#)clock.c	1.5	
 */

/**************************************************************************
**                                                                        *
**  FILE        :  clock.c                                                *
**                                                                        *
**  DESCRIPTION :  The clock function returns the current processor time. *
**		   To determine the time in seconds, the value returned   *
**		   by the clock function should be divided by the value   *
**		   of the macro CLOCKS_PER_SEC, defined in <time.h>.      * 
**									  *
**		   Currently interfaces with the cyle counter of the      *
**		   XVW simulator. The cycle counter is mapped at SFR      *
**		   address 0x600.					  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include <time.h>

clock_t	clock ( void )
{
#pragma asm
    MOV.B	0600H,#01H	; latch cycle counter
    MOV.B	R0L,0600H 	; read cycle counter
    MOV.B	R0H,0601H 	;    
    MOV.B	R1L,0602H 	;     
    MOV.B	R1H,0603H 	; 
#pragma endasm
}
