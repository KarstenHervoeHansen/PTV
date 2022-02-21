; isrs.asm - assembly interrupt service routines for 8051XA peripherals
;
;   RTXC    Version 3.2
;   Copyright (c) 1986-1997.
;   Embedded System Products, Inc.
;   ALL RIGHTS RESERVED
;
;===============================================================================
$INCLUDE (rtxcopts.inc)
$INCLUDE (isrs.inc)

; claim the peripheral's interrupt vector
; the next line after the vector claim must
; be the first instruction of the interrupt service routine
;
%IF(%CLK)THEN(
%IF(%TIMER_0)THEN(
CSEG AT 084H
DW   ((SYSMODE+RTXCINTLVL) SHL 8),isvctimer0
)FI

%IF(%TIMER_2)THEN(
CSEG AT 090H
DW   ((SYSMODE+RTXCINTLVL) SHL 8),isvctimer2
)FI
)FI  ; end of if CLK

%IF(%SIO)THEN(
%IF(%SIO_0)THEN(
CSEG AT 0A4H
DW   ((SYSMODE+RTXCINTLVL) SHL 8),isvcsio0o
CSEG AT 0A0H
DW   ((SYSMODE+RTXCINTLVL) SHL 8),isvcsio0i
)FI
)FI  ; end of if SIO


%ISRS_CODESEG_DEF
       RSEG ISRS_CODE

$INCLUDE (saveregs.inc)

%IF(%SIO)THEN(
	EXTRN %KSSTKTYPE(_rtxcstak)
	EXTRN %DATATYPE(_isrcnt)
	EXTRN %DATATYPE(_intlvl)
	EXTRN %CODETYPE(isrexit)
)ELSE(
%IF(%CLK)THEN(
	EXTRN IDATA(_rtxcstak)
	EXTRN %DATATYPE(_isrcnt)
	EXTRN %DATATYPE(_intlvl)
	EXTRN %CODETYPE(isrexit)
)FI
)FI  ; end of if SIO


%IF(%SIO)THEN(
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; serial interrupt service                                 ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
%IF(%SIO_0)THEN(

	EXTRN %CODETYPE(_sio0oc)
	EXTRN %CODETYPE(_sio0ic)

	public isvcsio0o
isvcsio0o:
    ; save the world of the interrupted
    saveregs

    ; set PSWH to the priority of the interrupt
    mov.b     PSWH,#SYSMODE+TX0INTLVL

    ; call interrupt handler
    %GCALL      _sio0oc

    ; goto isr exit code
    %GJMP       isrexit

	public isvcsio0i
isvcsio0i:
    ; save the world of the interrupted
    saveregs

    ; set PSWH to the priority of the interrupt
    mov.b     PSWH,#SYSMODE+RX0INTLVL

    ; call interrupt handler
    %GCALL      _sio0ic

    ; goto isr exit code
    %GJMP       isrexit

)FI  ; end of if SIO_0
)FI  ; end of if SIO

%IF(%CLK)THEN(
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; clk interrupt service                                    ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
%IF(%TIMER_0)THEN(

	public isvctimer0

isvctimer0:
       ; push/save all registers being used here
       ; perform private interrupt operations here
       ; pop/restore all registers being used here
       reti

)FI  ; end of if TIMER_0

%IF(%TIMER_2)THEN(

	EXTRN %CODETYPE(_timer2c)
	public isvctimer2

isvctimer2:
    ; save the world of the interrupted
    saveregs

    ; set PSWH to the priority of the interrupt
    mov.b     PSWH,#SYSMODE+TIMER2INTLVL

    ; call interrupt handler
    %GCALL      _timer2c

    ; goto isr exit code
    %GJMP       isrexit

)FI  ; end of if TIMER_2
)FI  ; end of if CLK

; end of isrs.asm
