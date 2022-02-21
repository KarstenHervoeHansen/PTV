; isrs.asm - assembly interrupt service routines for 8051XA peripherals
;
; RTXC    Version 3.2
; Copyright (c) 1986-1997.
; Embedded System Products, Inc.
; ALL RIGHTS RESERVED
;
; Modified by KEN-PTV 28/01-00
;  Application code used throughout this module
; Modified by TK-PTV 22/20-99
;  No conditional code, all interrupts are coded. Enable them
;  through respestive call.
;===============================================================================
$INCLUDE (rtxcopts.inc)
$INCLUDE (isrs.inc)

; claim the peripheral's interrupt vector
; the next line after the vector claim must
; be the first instruction of the interrupt service routine
;
CSEG AT 084H
DW	((SYSMODE+RTXCINTLVL) SHL 8), isvctimer0
CSEG AT 08CH
DW	((SYSMODE+RTXCINTLVL) SHL 8), isvctimer1
CSEG AT 090H
DW	((SYSMODE+RTXCINTLVL) SHL 8), isvctimer2

CSEG AT 0A4H
DW	((SYSMODE+RTXCINTLVL) SHL 8), isvcsio0o
CSEG AT 0A0H
DW	((SYSMODE+RX0INTLVL) SHL 8), isvcsio0i

CSEG AT 0ACH
DW	((SYSMODE+RTXCINTLVL) SHL 8), isvcsio1o
CSEG AT 0A8H
DW	((SYSMODE+RTXCINTLVL) SHL 8), isvcsio1i

CSEG AT 080H
DW	((SYSMODE+EX0INTLVL) SHL 8), isvcex0
CSEG AT 088H
DW	((SYSMODE+EX1INTLVL) SHL 8), isvcex1

CSEG AT 0114H
DW	((SYSMODE+RTXCINTLVL) SHL 8), isvcswr6

CSEG AT 0118H
DW	((SYSMODE+RTXCINTLVL) SHL 8), isvcswr7


%ISRS_CODESEG_DEF
	RSEG ISRS_CODE

$INCLUDE (saveregs.inc)

	EXTRN %KSSTKTYPE( _rtxcstak)
	EXTRN %DATATYPE( _isrcnt)
	EXTRN %DATATYPE( _intlvl)
	EXTRN %CODETYPE( isrexit)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; serial 0 interrupt service                               ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	EXTRN %CODETYPE( _intsio0o)
	EXTRN %DATATYPE( _RxOutput)
	EXTRN %DATATYPE( _RxInput)
	EXTRN HDATA( _RxBuffer)


	public isvcsio0o
isvcsio0o:
	; save the world of the interrupted
	saveregs

	; set PSWH to the priority of the interrupt
	mov.b		PSWH, #SYSMODE+TX0INTLVL

	; call interrupt handler
	%GCALL	_intsio0o

	; goto isr exit code
	%GJMP		isrexit


	public	isvcsio0i
isvcsio0i:
	; save register set on the interrupted's frame or stack
	; R1 first, R0 last
	push		R1, R0

	; store the Segment SELect register and the Extra Segment
	; register in the interrupted's frame
	mov.b		R1L, SSEL
	mov.b		R1H, ES
	push		R1

	CLR			RI_0

	MOV.B		R0L, _RxOutput        
	SUB.B		R0L, _RxInput
	OR.B		R0L, R0L

	BNE			isvcsio0i_1
	SETB		SWR6

isvcsio0i_1:
	ADDS.B	_RxInput, #01H
	MOV.B		R0L, _RxInput
	DIVU.B	R0L, #040H
	MOV.B		_RxInput, R0H
	MOV.B		R0L, R0H
	MOVS.B	R0H, #00H

	MOV.B		R1L, S0BUF

	SETB		R0SEG
	MOV.B		ES, #SEG( _RxBuffer)
	MOV.B		[R0+SOF( _RxBuffer)], R1L

	; restore the Extra Segment and the Segment SELect register
	; off the stack.
	pop     R1
	mov.b		ES, R1H
	mov.b   SSEL, R1L

	; restore the register set
	pop     R0, R1

	;return from interrupt
	reti


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; serial 1 interrupt service                               ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	EXTRN %CODETYPE( _int232o)
	EXTRN %CODETYPE( _int232i)

   public isvcsio1o
isvcsio1o:
   ; save the world of the interrupted
   saveregs

   ; set PSWH to the priority of the interrupt
   mov.b     PSWH, #SYSMODE+TX1INTLVL

   ; call interrupt handler
   %GCALL   _int232o

	; goto isr exit code
   %GJMP       isrexit


	public isvcsio1i
isvcsio1i:
   ; save the world of the interrupted
   saveregs

   ; set PSWH to the priority of the interrupt
   mov.b     PSWH, #SYSMODE+RX1INTLVL

   ; call interrupt handler
   %GCALL      _int232i

   ; goto isr exit code
   %GJMP       isrexit


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; clk interrupt service                                    ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	public isvctimer0
isvctimer0:
   ; push/save all registers being used here
   ; perform private interrupt operations here
   ; pop/restore all registers being used here

   ; DO NOT USE! USED AS COUNTER FOR COLORBAR GENERATOR
   reti


	public isvctimer1
isvctimer1:
   ; push/save all registers being used here
   ; perform private interrupt operations here
   ; pop/restore all registers being used here

   ; DO NOT USE! USED AS BAUD RATE GENERATOR FOR SIO1
   reti

	public isvctimer2
isvctimer2:
   ; push/save all registers being used here
   ; perform private interrupt operations here
   ; pop/restore all registers being used here

   ; DO NOT USE! USED AS BAUD RATE GENERATOR FOR SIO0
   reti


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; ext0 interrupt service                                   ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	EXTRN %CODETYPE( _CBS1S0Lines)
	EXTRN %DATATYPE( _TimeTickCounter)
	EXTRN %DATATYPE( _LineOffset)
	EXTRN %DATATYPE( _NewFieldPulse);

	public	isvcex0
isvcex0:
	; save register set on the interrupted's frame or stack
	; R6 first, R0 last

	push		R6, R5, R4, R3, R2, R1, R0

	; load and save the segment select and data segment
	; registers in the interrupted's frame.
	mov.b		R6L, SSEL
	mov.b		R6H, DS
	push		R6

	; store the Extra and Code Segment registers in the interrupted's frame
	mov.b		R6L, ES
	mov.b		R6H, CS
	push		R6
 
	; perform private interrupt operations here
	mov			R6, _TimeTickCounter

	djnz		R6, isvcex0_lbl1

	mov			R6, #0125		;reload time tick counter (PAL)
	setb		SWR7				;Time for time tick

isvcex0_lbl1:
	mov			_TimeTickCounter, R6

	JBC			TF0, DelayedField
	JBC			IE1, LoadFieldTimer

	%GCALL	_CBS1S0Lines

	; restore the Extra and Code Segment registers off the stack.
	pop			R6
	mov.b		CS, R6H
	mov.b		ES, R6L

	; restore the Data Segment and Segment SELect register off the stack.
	pop			R6
	mov.b		DS, R6H
	mov.b		SSEL, R6L

	; restore the register set
	pop			R0, R1, R2, R3, R4, R5, R6

	;return from interrupt
	reti

DelayedField:
	%GCALL	_CBS1S0Lines

	; restore the Extra and Code Segment registers off the stack.
	pop       R6
	mov.b     CS, R6H
	mov.b     ES, R6L

	; restore the Data Segment and Segment SELect register off the stack.
	pop       R6
	mov.b     DS, R6H
	mov.b     SSEL, R6L

	; restore the register set
	pop       R0, R1, R2, R3, R4, R5, R6

	;return from interrupt
	reti

LoadFieldTimer:
	setb			_NewFieldPulse

	mov				R6, _LineOffset

	mov.b			TH0, R6H
	mov.b			TL0, R6L

	%GCALL	_CBS1S0Lines

	; restore the Extra and Code Segment registers off the stack.
	pop       R6
	mov.b     CS, R6H
	mov.b     ES, R6L

	; restore the Data Segment and Segment SELect register off the stack.
	pop       R6
	mov.b     DS, R6H
	mov.b     SSEL, R6L

	; restore the register set
	pop       R0, R1, R2, R3, R4, R5, R6
	;return from interrupt
	reti


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; ext1 interrupt service                                   ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	public isvcex1
isvcex1:
   ; save register set on the interrupted's frame or stack
   ; R6 first, R0 last

   push      R6, R5, R4, R3, R2, R1, R0

   ; load and save the segment select and data segment
   ; registers in the interrupted's frame.
   mov.b     R6L, SSEL
   mov.b     R6H, DS
   push      R6

   ; store the Extra and Code Segment registers in the interrupted's frame
   mov.b     R6L, ES
   mov.b     R6H, CS
   push      R6
 
   ; perform private interrupt operations here

   ; restore the Extra and Code Segment registers off the stack.
   pop       R6
   mov.b     CS, R6H
   mov.b     ES, R6L

   ; restore the Data Segment and Segment SELect register off the stack.
   pop       R6
   mov.b     DS, R6H
   mov.b     SSEL, R6L

   ; restore the register set
   pop       R0, R1, R2, R3, R4, R5, R6

   ;return from interrupt
   reti


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; SWR6 interrupt service                                   ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	EXTRN %CODETYPE( _intsio0i)

	public isvcswr6
isvcswr6:
   ; save the world of the interrupted
   saveregs

   ; set PSWH to the priority of the interrupt
   mov.b     PSWH, #SYSMODE+6

   ; call interrupt handler
   %GCALL      _intsio0i

   ; goto isr exit code
   %GJMP       isrexit


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; SWR7 interrupt service                                   ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	EXTRN %CODETYPE( _ticktimer)

	public isvcswr7
isvcswr7:
   ; save the world of the interrupted
   saveregs

   ; set PSWH to the priority of the interrupt
   mov.b     PSWH, #SYSMODE+7

   ; call interrupt handler
   %GCALL      _ticktimer

	; goto isr exit code
   %GJMP       isrexit

; end of isrs.asm
