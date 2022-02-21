; isrs.inc - 8051XA peripheral options

;
;   RTXCio
;   Copyright (c) 
;   Embedded System Products, Inc.
;   ALL RIGHTS RESERVED
;

;
; CLOCK support
;
%DEFINE(CLK)(0)         ; 1=clk supported in application

%IF(%CLK)THEN(
%DEFINE(TIMER_0)(1)     ; 1=timer 0 supported in application
%DEFINE(TIMER_2)(0)     ; 1=timer 2 - RTXC time base supported in application

%IF(%TIMER_0)THEN(
;
; Timer 0 priority level, must match level in PERIPHAL.H
;
TIMER0INTLVL  EQU  0Eh  ; clk interrupt level,
                        ;  (User may change interrupt level at which
                        ;   clock interrupts.) cref CLK0DRV.C & ISRS.ASM
)FI  ; end of TIMER_0

%IF(%TIMER_2)THEN(
;
; Timer 2 priority level, must match level in PERIPHAL.H
;
TIMER2INTLVL  EQU  0Ch  ; clk interrupt level,
                        ;  (User may change interrupt level at which
                        ;   clock interrupts.) cref CLK2DRV.C & ISRS.ASM
)FI  ; end of TIMER_2
)FI  ; end of CLK

;=============================================================================
; serial support
;
%DEFINE(SIO)(1)         ; 1=serial supported in application

%IF(%SIO)THEN(
%DEFINE(SIO_0)(1)       ; 1=Serial I/O 0 supported in application
%DEFINE(SIO_1)(1)       ; 1=Serial I/O 1 supported in application

%IF(%SIO_0)THEN(
;
; receive 0 and transmit 0 priority levels, must match level in PERIPHAL.H
;
RX0INTLVL  EQU  0Eh     ; receive interrupt level
                        ; (User may change interrupt level at which
                        ;  SCI interrupts.) cref SCIDRV.C & ISRS.ASM
TX0INTLVL  EQU  0Bh     ; transmit interrupt level
                        ; (User may change interrupt level at which
                        ;  SCI interrupts.) cref SCIDRV.C & ISRS.ASM
)FI  ; end if SIO_0

%IF(%SIO_1)THEN(
;
; receive 1 and transmit 1 priority levels, must match level in PERIPHAL.H
;
RX1INTLVL  EQU  0Ah     ; receive interrupt level
                        ; (User may change interrupt level at which
                        ;  SCI interrupts.) cref SCIDRV.C & ISRS.ASM
TX1INTLVL  EQU  09h     ; transmit interrupt level
                        ; (User may change interrupt level at which
                        ;  SCI interrupts.) cref SCIDRV.C & ISRS.ASM
)FI  ; end if SIO_1
)FI  ; end if SIO

;=============================================================================
; external interrupt
;
%DEFINE(EXINT0)(1)      ; 1=external interrupt 0 supported in application
%DEFINE(EXINT1)(1)      ; 1=external interrupt 1 supported in application

%IF(%EXINT0)THEN(
;
; external interrupt 0, must match level in PERIPHAL.H
;
EX0INTLVL  EQU  0Fh     ; external interrupt 0 level

)FI  ; end if EXINT0

%IF(%EXINT1)THEN(
;
; external interrupt 1, must match level in PERIPHAL.H
;
EX1INTLVL  EQU  0Fh     ; external interrupt 1 level

)FI  ; end if EXINT1

; end of isrs.inc
