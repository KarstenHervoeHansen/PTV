/*
 *	Version : @(#)longjmp.c	1.5
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _longjmp.asm                                           *
**                                                                        *
**  DESCRIPTION :  Source file for longjmp function                       *
**		   The longjmp function restores the environment saved by *
**		   the most recent invocation of the program, with the    *
**		   corresponding jmp_buf argument. The longjmp function   *
**		   cause setjmp to return "val". The		 	  *
**		   longjmp function cannot cause setjmp to return the	  *
**		   value 0. If "val" is 0, then setjmp returns 1.	  *
**									  *
**		   Offset description of the 				  *	
**		   jmp_buf environment variable				  *
**									  *
**	model TINY/SMALL:						  *
**		   000000 stack pointer SSP/USP				  *
**		   000001 return address PC (16-bits)			  *
**									  *
**	model MEDIUM/LARGE:						  *
**		   000000 stack pointer SSP/USP				  *
**		   000001 return address PC (hi-byte) 			  *
**		   000002 return address PC (low-order 16-bits)		  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/
#include	<setjmp.h>

void longjmp( jmp_buf env, int val )
{
 	env;	/* to prevent warning W068: parameter "env" not used	*/
	val;    /* to prevent warning W068: parameter "val" not used	*/

#pragma	asm
#if _MODEL == 'l'	
	MOV.B	ES,R1L		;
#endif	
	MOV.W	R7,[R0+]	; restore SP from 'env' buffer
	MOV.W	[R7+],[R0+]	; restore PC from 'env' buffer
#if _MODEL == 'l' || _MODEL == 'm'
	MOV.W	[R7+],[R0+]	; restore PC from 'env' buffer
	SUB.W	R7,#04H		; correct stack pointer
#else
	SUB.W	R7,#02H		; correct stack pointer
#endif
#if _MODEL == 'l'
	MOV.W	R0,R2		; val
#else
	MOV.W	R0,R1		; val
#endif
	BNE	_end		; return( val );
	MOV.W	R0,#01H		; else return( 1 );
_end:
#pragma	endasm

}
