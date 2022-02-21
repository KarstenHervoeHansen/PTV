; isvcrtxc.asm

;
;   RTXC    Version 3.2
;   Copyright (c) 1986-1997.
;   Embedded System Products, Inc.
;   ALL RIGHTS RESERVED
;
;
;   RTXC assembly code - for the 8051XA
;
;=============================================================================
$CASE

$INCLUDE (RTXCOPTS.INC)

ISVCRTXC_DATA SEGMENT %KSSTKTYPE CLEAR

PSWOFFSET       equ     012h

	RSEG ISVCRTXC_DATA
;
; Reserve memory for the Kernel stack and Nulltask stack
;
	public _nulltos
	public _nullstak
_nulltos:
	ds      NULLSTKSZ
_nullstak:
	public _rtxctos
	public _rtxcstak
_rtxctos:
	ds      RTXCSTKSZ
_rtxcstak:

EXTRN %DATATYPE(_isrcnt)
EXTRN %DATATYPE(_intlvl)

EXTRN %CODETYPE(_rtxc)

CSEG AT 040H
DW   8000H+(RTXCINTLVL SHL 8),isvcrtx

%KRNL_CODESEG_DEF
       RSEG ISVCRTXC_CODE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; C to Assembly call to trap to kernel                     ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	public _ks
_ks:
	trap #RTXC_TRAPNUM
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                          ;
; interrupt service for RTXC Kernel Service using trap     ;
;                                                          ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
       public isvcrtx
isvcrtx:
    ; save register set on the interrupted's frame or stack
    ; R6 first, R0 last
    push      R6,R5,R4,R3,R2,R1,R0

    ; load and save the segment select and data segment
    ; registers in the interrupted's frame. Then set them
    ; to page zero (0).
    mov.b     R6L,SSEL
    mov.b     R6H,DS
    push      R6
%IF(%EQS(%_MODEL,l))THEN(
    mov.b     SSEL,#7Fh
)ELSE(
    mov.b     SSEL,#0
)FI
    mov.b     DS,#0

    ; store the Extra and Code Segment registers in the interrupted's frame.
    mov.b     R6L,ES
    mov.b     R6H,CS
    push      R6

    ; force the interrupt level count to one.
    ; Should always come in as zero (0).
%IF(%EQS(%CORE_IRAMDATA,1))THEN(     ; declared as type DATA
    movs.b      _isrcnt,#01h
)ELSE(
%IF(%EQS(%_MODEL,l))THEN(
    mov.b      ES, #SEG( _isrcnt )
    mov.w      R0, #SOF( _isrcnt )
    mov.b      [R0],#01h
)ELSE(       ; < 64K data default models when declared IDATA (indirect)
    mov.w      R0, #_isrcnt
    mov.b      [R0],#01h
)FI
)FI   ; end of if CORE_IRAMDATA

    ; pass the frame pointer or stack pointer as an argument to the kernel.
%IF( %EQS(%_MODEL,l) AND %NES(%_KS_STK_NEAR,1) )THEN(
    mov.b     R1L, DS
    xor.b     R1H, R1H
)FI
    mov.w     R0,R7

    ; switch to the kernel stack
    mov.w     R7,#_rtxcstak

    ; restore the processor priority level to the
    ; global task interrupt priority level
%IF(%EQS(%CORE_IRAMDATA,1))THEN(     ; declared as type DATA
    mov.b      R6L,_intlvl
)ELSE(
%IF(%EQS(%_MODEL,l))THEN(
    mov.b     ES, #SEG( _intlvl )
    mov.w     R6, #SOF( _intlvl )
    mov.b     R6L, [R6]
)ELSE(         ; < 64K data default models when declared IDATA (indirect)
    mov.w     R6, #_intlvl
    mov.b     R6L, [R6]
)FI
)FI   ; end of if CORE_IRAMDATA

    mov.b      PSWH,R6L

    ; call the RTXC kernel
    %GCALL     _rtxc                   ;call kernel

    ; exit the RTXC kernel by restoring the highest priority task
ksexit:

    ;restore the SP with the pointer to task frame or stack
%IF( %EQS(%_MODEL,l) AND %NES(%_KS_STK_NEAR,1) )THEN(
    mov.b     DS, R1L
)FI
    mov.w     R7, R0

    ; replace the highest priority task's copy of the PSW in its frame
    ; with the global task interrupt priority level stored in _intlvl.
    mov.w     R0,[R7+PSWOFFSET]       ;get psw of current task
    and.w     R0,#0F0FFh              ;set ppl to zero
%IF(%EQS(%CORE_IRAMDATA,1))THEN(     ; declared as type DATA
    or.b      R0H,_intlvl             ;OR in current task ppl
)ELSE(
%IF(%EQS(%_MODEL,l))THEN(
    mov.b     ES, #SEG( _intlvl )
    mov.w     R6, #SOF( _intlvl )
    mov.b     R6L, [R6]
    or.b      R0H, R6L                ;OR in current task ppl
)ELSE(         ; < 64K data default models when declared IDATA (indirect)
    mov.w     R6, #_intlvl
    mov.b     R6L, [R6]
    or.b      R0H, R6L                ;OR in current task ppl
)FI
)FI   ; end of if CORE_IRAMDATA

    mov.w     [R7+PSWOFFSET],R0       ;put in task stack frame

    ; force the interrupt level count to zero (0).
    ; Should always be one here.
%IF(%EQS(%CORE_IRAMDATA,1))THEN(     ; declared as type DATA
    movs.b    _isrcnt,#0h
)ELSE(
%IF(%EQS(%_MODEL,l))THEN(
    mov.b      ES, #SEG( _isrcnt )
    mov.w      R6, #SOF( _isrcnt )
    mov.b      [R6],#0h
)ELSE(         ; < 64K data default models when declared IDATA (indirect)
    mov.w      R6, #_isrcnt
    mov.b      [R6],#0h
)FI
)FI  ; end of if CORE_IRAMDATA

    ; restore the Extra and Code Segment registers off the stack.
    pop       R6
    mov.b     CS,R6H
    mov.b     ES,R6L

    ; restore the Data Segment and Segment SELect register off the stack.
    pop       R6
    mov.b     DS,R6H
    mov.b     SSEL,R6L

    ; restore the register set
    pop       R0,R1,R2,R3,R4,R5,R6

    ;return from interrupt
    reti

      public isrexit
isrexit:

    ;restore the SP with the pointer to task frame or stack
%IF( %EQS(%_MODEL,l) AND %NES(%_KS_STK_NEAR,1) )THEN(
    mov.b     DS, R1L
)FI
    mov.w     R7, R0

    ; decrement the interrupt level count
%IF(%EQS(%CORE_IRAMDATA,1))THEN(     ; declared as type DATA
    adds.b    _isrcnt,#-1
)ELSE(
%IF(%EQS(%_MODEL,l))THEN(
    mov.b      ES, #SEG( _isrcnt )
    mov.w      R0, #SOF( _isrcnt )
    adds.b     [R0],#-1h
)ELSE(         ; < 64K data default models when declared IDATA (indirect)
    mov.w      R0, #_isrcnt
    adds.b     [R0],#-1h
)FI
)FI    ; end of if CORE_IRAMDATA

    ; restore the Extra and Code Segment registers off the stack.
    pop       R6
    mov.b     CS,R6H
    mov.b     ES,R6L

    ; restore the Data Segment and Segment SELect register off the stack.
    pop       R6
    mov.b     DS,R6H
    mov.b     SSEL,R6L

    ; restore the register set
    pop       R0,R1,R2,R3,R4,R5,R6

    ;return from interrupt
    reti

;
; interrupt enable
;
      public   _ks_enable
_ks_enable:
      push    R0
      mov.b   R0L,PSWH
      and.b   R0L,#0F0h
%IF(%EQS(%CORE_IRAMDATA,1))THEN(     ; declared as type DATA
      or.b    R0L,_intlvl
)ELSE(
      push    R6
%IF(%EQS(%_MODEL,l))THEN(
      mov.b   ES, #SEG( _intlvl )
      mov.w   R6, #SOF( _intlvl )
      or.b    R0L, [R6]                ;OR in current task ppl
)ELSE(         ; < 64K data default models when declared IDATA (indirect)
      mov.w   R6, #_intlvl
      or.b    R0L, [R6]                ;OR in current task ppl
)FI
      pop     R6
)FI   ; end of if CORE_IRAMDATA

      mov.b   PSWH,R0L
      pop     R0
      ret

;
; interrupt disable
;
      public   _ks_disable
_ks_disable:
      mov.b     PSWH,#SYSMODE+RTXCINTLVL  ;disable interrupts
      ret

      public   _pushpswh
_pushpswh:
      mov.b     [R0],PSWH             ;save current psw
      ret

      public   _poppswh
_poppswh:
      mov.b      PSWH,R0L             ;restore psw
      ret

      public   _popnull
_popnull:
      ret

;
; Routine to allow, allow a task to change the processor priority level
; at which tasks run.
;
; As distributed, all RTXC tasks begin execution at processor priority
; level 0 (TASK_INT_LEVEL defined in RTXCOPTS.H).  If a level change is
; required, a task MUST change levels (up and down) by calling setintlvl().
; This will change the processor priority level at which all tasks run.
; The global variable "intlvl" defines the PPL at which tasks run.
;
; char setintlvl(char level);  /* pass and return level = 0-15 */
;
;
	public  _setintlvl
_setintlvl:
	; save the values of these registers on the stack
	push    R5
	push    R6

	; get current level of psw and preserve only the priority level to return
	mov.b   R6L,PSWH
	and.b   R6L,#0Fh

	; Mask out interrupts at level 15, store the new interrupt level
	; and the force supervisor mode in the global variable _intlvl
	or.b    PSWH,#0Fh
	or.b    R0L,#SYSMODE
%IF(%EQS(%CORE_IRAMDATA,1))THEN(           ; declared as type DATA
	mov.b   _intlvl,R0L
)ELSE(
%IF(%EQS(%_MODEL,l))THEN(
	mov.b  ES, #SEG( _intlvl )
	mov.w  R5, #SOF( _intlvl )
	mov.b   [R5], R0L
)ELSE(         ; < 64K data default models when declared IDATA (indirect)
	mov.w  R5, #_intlvl
	mov.b   [R5], R0L
)FI
)FI    ; end of if CORE_IRAMDATA

	; take the PSWH value and AND out the priority level
	; then set the new priority level
	mov.b   R5L,PSWH             ;reread psw
	and.b   R5L,#0F0h            ;preserve all but priority level
	or.b    R5L,R0L              ;set psw to new level
	mov.b   PSWH,R5L             ;store new psw into sfr

	; return old priority level (IM3,IM2,IM1,IM0)
	mov.b   R0L,R6L

	; restore the values of these registers off the stack
	pop     R6
	pop     R5

	ret

;
; char getintlvl(void);  /* returns level 0-15 */
;
	public  _getintlvl

_getintlvl:
	mov.b   R0L,PSWH              ;get psw
	and.b   R0L,#0Fh              ;preserve only the priority level
	ret

       END

; end of isvcrtxc.asm 
