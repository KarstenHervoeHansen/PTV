; HCOUNT	SEGMENT CODE
; HISTORY:
; 960129 Fieldjobs() moved to fieldint.
; 940610 stop line no. modified
; 940608 datawin target now 11 us to 19 us from sync
; 940524 trmask and trampli added.

; extrn code	(fieldjobs)

EXTRN 	lineno, fieldno, detmask, v24on, trmask, trampli
EXTRN 	port1tab, port2tab
EXTRN	testmode

PSECT HSTART

ORG 000Bh
;	ljmp	hint


; RSEG	HDATA
; index:	DS	1

PSECT	HCOUNT

hint:
	PUSH    ACC
	PUSH	B
        PUSH    DPH
        PUSH    DPL
	PUSH    PSW
	MOV     PSW,#08H

	mov	a,r5		;get presaved port2 value
	MOV     DPTR,#08000H
	MOVX    @DPTR,A		;write port2 value (with START OF DATAWIN)
	mov	r6,a		;save value

	MOV     A,lineno	;get line no. to get port1 value
	ADD     A,#LOW port1tab	;offset by table start

;NOW MAKE A BREAK TO STOP DATAWIN ON PORT2
	xch	a,r6		;save acc and get port2 value
	anl	a,#0fbh		;value for stopping datawin
;	MOV     DPTR,#08000H
	mov	r5,a		;delay
	mov	r5,a		;delay
	mov	r5,a		;delay
	mov	r5,a		;delay
	mov	r5,a		;delay

	MOVX    @DPTR,A		;STOP DATAWIN
	
	mov	a,r6		;get port2 value back
	MOV     DPL,A
	CLR     A
	ADDC    A,#HIGH port1tab
	MOV     DPH,A
	MOVX    A,@DPTR		;get port1 value
	orl	a,detmask	;insert det. positions
	mov	dptr,#0c000h	;write to port
	movx	@dptr,a


; PREPARE NEXT LINE
	MOV     A,lineno		;get line no.
	inc	a
	ADD     A,#LOW port2tab	;offset by table start
	MOV     DPL,A
	CLR     A
	ADDC    A,#HIGH port2tab
	MOV     DPH,A
	MOVX    A,@DPTR		;get port2 value
	orl	a,v24on		;add bit for V24-out control
	orl	a,trmask	;set mode (play-address)
	orl	a,trampli	;set amplitude
	jnb	testmode,notst	;set testmode
	setb	acc.0
notst:
	MOV     R5,a		;save it until next line
	MOV     A,lineno	;stop if line 23 (line=17)
	CJNE    A,#17,ck36
	CLR     TR0
;	setb	fieldjob
	sjmp	finish
ck36:
	CJNE    A,#35,morelin		;F2: stop if line 313+23 (line=35)
	CLR     TR0
;	setb	fieldjob
finish:				;stop transmit from line 23 (336)
	mov	r1,#20		;wait 1/2 line
	djnz	r1,$
	mov	a,r5
	MOV     DPTR,#08000H
	MOVX    @DPTR,A		;write port2 value (with start of datawin)
	MOV     lineno,#0	;reset line no. <960129a

	POP     PSW
        POP     DPL
        POP     DPH
	POP	B
	POP     ACC
;	lcall	fieldjobs
	RETI    
	
morelin:
	INC     lineno		;next line
	POP     PSW
        POP     DPL
        POP     DPH
	POP	B
	POP     ACC
	RETI    
end