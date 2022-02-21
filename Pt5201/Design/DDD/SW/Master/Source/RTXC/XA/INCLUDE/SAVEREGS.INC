; saveregs.inc - RTXC interrupt save register macro 8051XA

;
;   RTXC    
;   Copyright (c) 
;   Embedded System Products, Inc.
;   ALL RIGHTS RESERVED
;

$ GEN

saveregs MACRO
    ; save register set on the interrupted's frame or stack
    ; R6 first, R0 last
    push      R6,R5,R4,R3,R2,R1,R0

    ; load and save the segment select and data segment
    ; registers in the interrupted's frame.
    mov.b     R6L,SSEL
    mov.b     R6H,DS
    push      R6
%IF(%EQS(%_MODEL,l))THEN(
    mov.b     SSEL,#7Fh
)ELSE(
    mov.b     SSEL,#0
)FI
    mov.b     DS,#0

    ; store the Extra and Code Segment registers in the interrupted's frame
    mov.b     R6L,ES
    mov.b     R6H,CS
    push      R6

    ; pass the frame pointer or stack pointer as an argument to the kernel
%IF( %EQS(%_MODEL,l) AND %NES(%_KS_STK_NEAR,1) )THEN(
    mov.b     R1L, DS
    xor.b     R1H, R1H
)FI
    mov.w     R0,R7

    ; compare the interrupt level count to zero (0) to determine stack usage
%IF(%EQS(%CORE_IRAMDATA,1))THEN(     ; declared as type DATA
    cmp.b     _isrcnt,#0             ;if nesting level != 0
)ELSE(
%IF(%EQS(%_MODEL,l))THEN(
    mov.b      ES, #SEG( _isrcnt )
    mov.w      R6, #SOF( _isrcnt )
    cmp.b      [R6],#0             ;if nesting level != 0
)ELSE(           ; < 64K data default models when declared IDATA (indirect)
    mov.w      R6, #_isrcnt
    cmp.b      [R6],#0             ;if nesting level != 0
)FI
)FI   ; end of if CORE_IRAMDATA

    bne       ^noswitch              ; don't switch, use current stack
    mov.w     R7,#_rtxcstak          ;load kernel stack into sp

^noswitch:
    ; increment the interrupt level count by one.
%IF(%EQS(%CORE_IRAMDATA,1))THEN(     ; declared as type DATA
    adds.b    _isrcnt,#1h
)ELSE(  ; already loaded R6 with _isrcnt
    adds.b     [R6],#1h
)FI  ; end of if CORE_IRAMDATA

    ENDM

; end of saveregs.inc
