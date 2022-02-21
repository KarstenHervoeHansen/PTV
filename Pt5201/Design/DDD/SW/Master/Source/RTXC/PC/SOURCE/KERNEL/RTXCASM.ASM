    TITLE   rtxcasm.asm
    NAME    rtxcasm

;
;   RTXC    Version 3.2
;   Copyright (c) 1986-1997.
;   Embedded System Products, Inc.
;   ALL RIGHTS RESERVED
;

MASM51                      ; MASM mode is default,
                            ; MASM51 enables 5.1 extensions

INCLUDE saveregs.inc        ; register save macros

IF CPU EQ 0
    .8087
ELSE ; CPU EQ 2 or 3
    .286p
    .287
ENDIF

EXTRN _isrcnt:BYTE      ; interrupt nesting counter, declared in isrc.c

RTXC_TEXT SEGMENT BYTE PUBLIC 'CODE'
RTXC_TEXT ENDS

_DATA SEGMENT WORD PUBLIC 'DATA'
_DATA ENDS

_BSS SEGMENT WORD PUBLIC 'BSS'
_BSS ENDS

;;_CONST SEGMENT WORD PUBLIC 'CONST'
;;_CONST ENDS

DGROUP GROUP _DATA, _BSS
;DGROUP GROUP _DATA, _CONST, _BSS,
ASSUME CS: RTXC_TEXT, DS: DGROUP, SS: DGROUP, ES: DGROUP

_DATA SEGMENT

;
; kernel stack area
;

PUBLIC _rtxcstk, _rtxctos
_rtxctos  DB  RTXCSTKSZ-4 DUP (0) ; kernel stack area, 0 to allow for checks
                             ; must be defined in _DATA segment
_rtxcstk  DD  0              ; ss:sp - must be adjacent to rtxctos
_DATA ENDS

IFDEF __HUGE__ ; {
EXTRN _rtxc:FAR     ; rtxc function decoder and task dispatcher
ELSE ; } __HUGE__ {
EXTRN _rtxc:NEAR    ; rtxc function decoder and task dispatcher
ENDIF ; } __HUGE__

RTXC_TEXT SEGMENT

DEFSEG DW SEG _isrcnt          ; NOTE: must be in CODE segment
;                              ; used in context save logic

;
; void far isvcrtx(void) - interrupt service for RTXC software interrupt
;
PUBLIC _isvcrtx
_isvcrtx PROC FAR
        fastsave        ; save minimal C context, see macro for details
        call    _rtxc   ; FRAME * rtxc(FRAME ks_stk *) equivalent
        cexit           ; fall through to cexit
                        ; note: stack does not need adjustment
                        ;       since ss:sp will be overridden anyway
_isvcrtx ENDP

IF FPU EQ 1
;
; 80x87 hardware register context save area
;
fpregs STRUC  ; see rtxstruc.h for corresponding C structure definition
;
; example of extended context for error handling
;   note: must match FPREGS in rtxstruc.h
;
; errno     dw ? ; task image of RTL errno
; _doserrno dw ? ; task image of RTL _doserrno
;
control dw ?   ; control register */
status  dw ?   ; status register */
tag     dw ?   ; tag register */
ip      dd ?   ; instruction pointer */
op      dd ?   ; operand pointer */
st0     dt ?   ; fpu data reg 0 */
st1     dt ?   ; fpu data reg 1 */
st2     dt ?   ; fpu data reg 2 */
st3     dt ?   ; fpu data reg 3 */
st4     dt ?   ; fpu data reg 4 */
st5     dt ?   ; fpu data reg 5 */
st6     dt ?   ; fpu data reg 6 */
st7     dt ?   ; fpu data reg 7 */
fpregs ENDS

PUBLIC _fpuswap

IFDEF __HUGE__ ; {
; void cdecl huge fpuswap(FPREGS huge *old, FPREGS huge *new);
_fpuswap PROC FAR
    arg old:DWORD,new:DWORD
ELSE ; } __HUGE__ {
; void cdecl near fpuswap(FPREGS near *old, FPREGS near *new);
_fpuswap PROC
    arg old:WORD,new:WORD
ENDIF ; } __HUGE__

;
; note, this code is for BCC -f87/-f287 switch.  It is for a real 80x87,
;       not the Borland fp emulator.
;
; note, entire routine is performed with interrupts disabled
;

IF CPU EQ 0
        push    bp         ; make frame
        mov     bp,sp
ELSE ; CPU EQ 2 or 3
        enter   WORD PTR 0,0
ENDIF

IFDEF __HUGE__ ; HUGE {
        les     bx,DWORD PTR [old]  ; get old (save) address passed on stack
        cmp     bx,0      ; check for 32 bit NULL ptr
        jne     fpsave
        mov     cx,es
        cmp     cx,0
        je      fprstor
fpsave:
        fsave   es:control.[bx] ; save current owner's fpu regs
fprstor:
        les     bx,DWORD PTR [new]  ; get new (restore) address passed on stack
        frstor  es:control.[bx] ; restore new owner's fpu regs
ELSE ; } HUGE {
        mov     bx,WORD PTR [old] ; get old (save) address passed on stack
        cmp     bx,0      ; quick check for no reg save area (null task)
        je      fprstor
        fsave   control.[bx] ; save current owner's fpu regs
fprstor:
        mov     bx,WORD PTR [new] ; get new (restore) address passed on stack
        frstor  control.[bx] ; restore new owner's fpu regs
ENDIF ; } HUGE

IF CPU EQ 0
        mov     sp,bp
        pop     bp
ELSE ; CPU EQ 2 or 3
        leave
ENDIF
        fwait              ; wait around for frstor to complete
        ret
_fpuswap ENDP
ENDIF                      ; IF FPU EQ 1


;
; the following three functions, ut_enable, ut_disable, and ut_nop
; are optional.  they are provided in case the inline equivalents are
; not desired.  note, all three functions are declared as near functions.
; If standard _enable or enable() was used, then the far version found
; in cl.lib would be used.
;
PUBLIC _ut_enable
; void cdecl near ut_enable(void);
_ut_enable PROC near
        sti
        ret
_ut_enable ENDP

PUBLIC _ut_disable
; void cdecl near ut_disable(void);
_ut_disable PROC near
        cli
        ret
_ut_disable ENDP

PUBLIC _ut_nop
; void cdecl near ut_nop(void);
_ut_nop PROC near
        ret
_ut_nop ENDP

PUBLIC _getflags
; int cdecl near getflags(void);
_getflags PROC far
        pushf
		pop   ax
        ret
_getflags ENDP

RTXC_TEXT ENDS

END

; End of file - rtxcasm.asm
