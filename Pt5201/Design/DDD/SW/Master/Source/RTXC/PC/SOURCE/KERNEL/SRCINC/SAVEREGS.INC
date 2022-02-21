;   TITLE   saveregs.inc

;
;   RTXC    Version 3.2
;   Copyright (c) 1986-1997.
;   Embedded System Products, Inc.
;   ALL RIGHTS RESERVED
;

;
; RTXC register save macros
;                                                                                            0

        include ..\kernel\rtxcopts.inc

saveregs MACRO
    LOCAL skip,eom
        push    ds   ; save ds on stack
        push    es   ; save es on stack

IF CPU EQ 0
        push    ax  ; start of "pusha"
        push    cx
        push    dx
        push    bx
        push    sp  ; sp - not really used
        push    bp
        push    si
        push    di  ; end of "pusha"
ENDIF
IF CPU EQ 2
        pusha
ENDIF
IF CPU EQ 3
        PUSHSTATE
        .386
        pushad
        POPSTATE
ENDIF

        mov     ds,cs:DEFSEG ; force ds to SEG _isrcnt
        inc     _isrcnt ; track interrupt level
        cmp     _isrcnt,1 ; if not at 1st level
        jne     skip       ; then no stack switch
        mov     si,OFFSET DGROUP:_rtxcstk ; else
        mov     ax,SEG    DGROUP:_rtxcstk
        mov     es,ax
        mov     es:2[si],ss   ; save current ss:sp on new RTXCSTK
        mov     es:0[si],sp
        mov     ss,ax
        mov     sp,si
        jmp     eom
skip:
        push    ss    ; pass ss:sp on stack
        mov     ax,sp ; compatible 80286/8088 push sp sequence
        add     ax,2  ; note: push sp executes differently on 8088 vs 80286
        push    ax    ;   same code will run on 8088/80286
eom:
        cld          ; clear direction for C routines
    ENDM

;
; fast C register save macros - used only by kernel _ks
;
; this may vary from one compiler to another
;
; skip saving volatile C registers (es,ax,cx,dx,bx,sp) and adjust
; the frame as needed
;

fastsave MACRO
        push    ds   ; save ds on stack

IFDEF __HUGE__ ; {
        mov     ds,cs:DEFSEG ; force ds to SEG _isrcnt
ENDIF ; } __HUGE__

        inc     _isrcnt ; track interrupt level (now := 1)

IF CPU EQ 0
        push    es

                       ; start of pusha
        push    ss
        push    cx
;       push    dx
;       push    bx
;       push    sp
        sub     sp,6  ; faster than 3 x push
        push    bp
        push    si
        push    di
;                       ; end of pusha
ELSE ; CPU EQ 2 or 3
        push    es

IF CPU EQ 2
IF ks_stk_far EQ 1
        mov ax, ss    ; store stack segment in ax, stack offset is in cx
ENDIF
        pusha
ENDIF
IF CPU EQ 3
        PUSHSTATE
        .386
        pushad
        POPSTATE
ENDIF
ENDIF

IF ks_stk_far EQ 1
        mov     si,OFFSET DGROUP:_rtxcstk
        mov     ax,SEG    DGROUP:_rtxcstk
        mov     es,ax
        mov     es:2[si],ss   ; save current ss:sp on new RTXCSTK
        mov     es:0[si],sp
        mov     ss,ax
        mov     sp,si
ENDIF

IF ks_stk_near EQ 1
        mov     si,OFFSET DGROUP:_rtxcstk
        mov     [si],sp
        mov     sp,si
ENDIF

        sti         ; enable interrupts
    ENDM


;
; this is the normal exit path from the kernel
; it is performed with interrupts disabled
; cexit is a macro, to be called after calls to all C interrupt handlers
;
; DS is also known to be == DGROUP
;
cexit MACRO
        dec     _isrcnt  ; decrement isr level
        mov     sp,ax    ; restore ss:sp from returned value dx:ax
        mov     ss,dx

IF CPU EQ 0
        pop     di      ; start of "popa"
        pop     si
        pop     bp
;       add     sp,2    ; ignore mid push sp
        pop     bx      ; this is faster than "add sp,2", bx overwritten
        pop     bx
        pop     dx
        pop     cx
        pop     ax      ; end of "popa"
ENDIF
IF CPU EQ 2
        popa
ENDIF
IF CPU EQ 3
        PUSHSTATE
        .386
        popad
        POPSTATE
ENDIF

        pop     es
        pop     ds

        iret            ; note: 16 bit ccr even in 386 mode
   ENDM

; End of file - saveregs.inc
