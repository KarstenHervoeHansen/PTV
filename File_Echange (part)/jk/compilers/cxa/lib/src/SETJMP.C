/*
 *	Version : @(#)setjmp.c	1.5
 */

/**************************************************************************
**                                                                        *
**  FILE        :  _setjmp.asm                                            *
**                                                                        *
**  DESCRIPTION :  Source file for setjmp function                        *
**		   The setjmp function saves its calling environment in   *
**		   its jmp_buf for later use by the longjmp function.     *
**		   If the return is from a direct invocation, the setjmp  *
**		   function returns zero. If the return is from a call to *
**		   the longjmp, the setjmp function retunrs a nonzero	  *
**		   value.						  *
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

int setjmp( jmp_buf env )
{
 	env;	/* to prevent warning W068: parameter "env" not used	*/

#pragma	asm
#if _MODEL == 'l'	
	MOV.B	ES,R1L		;
#endif	
	MOV.W	[R0+],R7	; save SSP/USP in 'env' buffer
	MOV.W	[R0+],[R7+]	; save PC in 'env' buffer
#if _MODEL == 'l' || _MODEL == 'm'
	MOV.W	[R0+],[R7+]	; save PC in 'env' buffer 
	SUB.W	R7,#04H		; correct stack pointer
#else
	SUB.W	R7,#02H		; correct stack pointer				      
#endif				      
#pragma	endasm

	return( 0 );
}
