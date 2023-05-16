; se ogs† si.84...
; PSECT-ERKL’RING. SKAL forefindes.
	PSECT	text,class=CODE
;	PSECT	text,align=2

; GLOBALE VARIABLE
	global	_fdmul

; SIGNATUR. SKAL forefindes -se si.86
SIGNAT	_fdmul,1234

; EKSTERNE
	ekstrn  _var1, _stor3


; BETINGET KOMPILERING
#ifdef	LARGE_MODEL
rasize	equ	4
#else
rasize	equ	2
#endif

; KONSTANTER
quot	equ	rasize
prod	equ	quot


; MACRO ERKL’RING.
eksemp	macro	arg
	global	_printf
	savereg	<r0,r1,r2,r3,r4,r5,r6,r7>
	push	#999f
	call	_printf
	pop	0
	restreg	<r7,r6,r5,r4,r3,r2,r1,r0>
	psect	data
999:	dcb	'arg',10,0
	psect	text
	endm

fdmul:

jumpeks:
	cmp	expl,expr	;compare exponents
	bcs	5f		;expl is lower, so branch
	beq	6f		;equal, so just return

5:
	mov	r0,#0
	mov	[r7+mantl+0],r0
	ret
6:
	call	ddiv
	mov	[r4+0],r0
	pop	r4
	ret

; OPTIONAL END
end
