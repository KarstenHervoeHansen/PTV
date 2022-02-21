TITLE isrs.asm
NAME  isrs

;
;   RTXC    Version 3.2
;   Copyright (c) 1986-1997.
;   Embedded System Products, Inc.
;   ALL RIGHTS RESERVED
;

;
; IBM PC interrupt service logic
;

MASM51                      ; MASM mode is default,
                            ; MASM51 enables 5.1 extensions

IF CPU EQ 2
   .286p
ENDIF

INCLUDE saveregs.inc

INCLUDE isrs.inc

EXTRN _rtxcstk:BYTE ; saveregs use

RTXC_TEXT SEGMENT BYTE PUBLIC 'CODE'
RTXC_TEXT ENDS

_CONST SEGMENT WORD PUBLIC 'CONST'
_CONST ENDS

_DATA SEGMENT WORD PUBLIC 'DATA'
_DATA ENDS

_BSS SEGMENT WORD PUBLIC 'BSS'
_BSS ENDS

DGROUP GROUP _CONST, _BSS, _DATA

ASSUME CS: RTXC_TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP

EXTRN _isrcnt:BYTE ; nesting level counter

IF clk eq 1
EXTRN _clkc:FAR
ENDIF

EXTRN _int232:FAR

IF prn EQ 1
EXTRN _prnc:FAR
ENDIF

IF prtsc EQ 1
EXTRN _prtscc:FAR
ENDIF

IF dvz EQ 1
EXTRN _dvzc:FAR
ENDIF

RTXC_TEXT SEGMENT

DEFSEG DW SEG _isrcnt          ; NOTE: must be in CODE segment
                                ; used in context save logic

;
; interrupt service for periodic clock interrupt
;
IF clk EQ 1
       PUBLIC  _isvcclk
_isvcclk PROC FAR
        saveregs      ; save register set on stack
        call    _clkc ; call C interrupt handler
        cexit         ; goto common exit logic
_isvcclk ENDP
ENDIF

;
; interrupt service for ^C (BREAK) from PC keyboard
;
IF brk EQ 1
EXTRN _brkflag:BYTE
       PUBLIC  _isvcbrk
_isvcbrk PROC FAR
        push    ds
        push    bx  ; make room for isrcnt++ & setup DS

IFDEF __HUGE__ ; {
        mov     bx,SEG _isrcnt
ELSE ; } HUGE {
        mov     bx,SEG DGROUP:_isrcnt
ENDIF ; } HUGE

        mov     ds,bx

IFDEF __HUGE__ ; {
        mov     _brkflag,1 ;set flag to be recognized by null task
ELSE ; } HUGE {
        mov     DGROUP:_brkflag,1 ;set flag to be recognized by null task
ENDIF ; } HUGE

        pop     bx
        pop     ds
        iret
_isvcbrk ENDP
ENDIF

;
; interrupt service for COM interrupt
;
       PUBLIC  _isvccom
_isvccom PROC FAR
        saveregs
        call    _int232
        cexit
_isvccom ENDP

;
; interrupt service for PRN (LPT) interrupt
;
IF prn EQ 1
       PUBLIC  _isvcprn
_isvcprn PROC FAR
        saveregs
        call    _prnc
        cexit
_isvcprn ENDP
ENDIF

;
; interrupt service for PrtSc (Print Screen) interrupt
;
IF prtsc EQ 1
       PUBLIC  _isvcprtsc
_isvcprtsc PROC FAR
        saveregs
        call    _prtscc
        cexit
_isvcprtsc ENDP
ENDIF

;
; interrupt service for divide-by-zero interrupt
;
IF dvz EQ 1
       PUBLIC  _isvcdvz
_isvcdvz PROC FAR
        saveregs
        call    _dvzc
        cexit
_isvcdvz ENDP
ENDIF

;
; add more interrupt service code here
;

RTXC_TEXT ENDS

END

; End of file - isrs.asm
