$CASE
$LOCALS
; *** 
;     This file has been modified by
;     Embedded System Products, Inc. to
;     be used with their RTXC kernel.
; ***
; @(#)start.asm 1.9 7/22/97
;	Header file for XA type processors
;	--------------------------------------
;
		NAME START

%IF(%EQS(%_MODEL,m) OR %EQS(%_MODEL,l))THEN(
	%DEFINE(GCALL)(FCALL)
	%DEFINE(GJMP)(FJMP)
        %DEFINE(CODE_SEG_DEF)(LIBRT_PR     SEGMENT HCODE INSEGMENT)
)ELSE(
	%DEFINE(GCALL)(CALL)
	%DEFINE(GJMP)(JMP)
        %DEFINE(CODE_SEG_DEF)(LIBRT_PR     SEGMENT CODE)
)FI

;
%CODE_SEG_DEF
                RSEG    LIBRT_PR
  ;
		PUBLIC	__EXIT		; address to jump to on 'exit()'
		PUBLIC	__START		; start label, mentioned in description file (.dsc)
					; and by C-compiler, don't remove it

$INCLUDE (RTXCOPTS.INC)

EXTRN %KSSTKTYPE(_nullstak)

;	------------------- SYSTEM STARTUP CODE ----------------------------

__START:
	MOV.B	SCR,#B_SCR		; set System Configuration Register
					; disable watchdog timer
	MOV.B	WDCON,#00H		; set WD control register to clear WDRUN
	MOV.B	WFEED1,#0A5H		; do watchdog feed part 1
	MOV.B	WFEED2,#05AH		; do watchdog feed part 2

;	---------------- Appended by TK PTV   23/7-99 ------------------------
	MOV.B	BCR,#006H

	MOV.B	BTRH,#0BAH		; KEN WM0=1 REQUIRED TO BE 1
;	MOV.B	BTRH,#088H
	MOV.B	BTRL,#0EAH		; KEN WM0=1 REQUIRED TO BE 1
;	MOV.B	BTRL,#068H
;	-------------------------- Append end --------------------------------


%IF(%EQS(%_MODEL,l))THEN(
        MOV.B   DS,#SEG(__DS)      ; initialize stack pointer
)FI
	MOV.W	R7,#SOF(__lc_es)	; initialize stack pointer

%IF(%EQS(%_MODEL,l))THEN(
	MOV.B	SSEL,#07FH		; default indirect access via ES
)FI
					; now clear BITADDR segments
	MOV.W	R0,#020H		; start address bit space
	MOV.W	R1,#010H		; number of bit words
_clrb:	MOV.W	[R0+],#00H		; clear bit word
	DJNZ.W	R1,_clrb		; loop until end of bit segment

	%GCALL	__INITSEG		; initialized data segments

	MOV.W   R0, #0			; set argc to 0
%IF(%EQS(%_MODEL,l))THEN(
	MOV.W   R2, #0			;
        MOV.W   R3, #0			; set argv[] to 0
)ELSE(
	MOV.W   R1, #0			; set argv[] to 0
)FI

;
;       Load sp with RTXC null stack pointer
;
%IF(%EQS(%_MODEL,l))THEN(
	MOV.B   DS,#SEG(_nullstak)      ; initialize stack pointer
)FI
	MOV.W   R7,#SOF(_nullstak)        ; initialize stack pointer

	%GCALL  _main                   ; call C program

	MOVS.W	R0,#00H			; set exit status 0
	%GJMP	_exit			; jump to exit(0) function

__EXIT:
	; exit() function jumps to this place. When it is required to restart
	; the program, R7 should be initialized again (if applicable).
	; The exit() function causes normal program termination to occur. First, all 
	; functions registered by the atexit() function are called in the reverse 
	; order. Next, all open streams with unwritten buffered data are flushed, all 
	; open streams are closed and all files created by the tmpfile() function are 
	; removed. The status value passed to exit is returned in R0.

	SETB	PD			; Power down

;	------------- AVOID ILLEGAL NULL POINTERS ------------------------------

		DSEG AT 00H
	DS	1
	
;	------------- CONFIGURATION BITS ---- SCR ------------------------------

_PT1	EQU	0	; Peripheral Clock
_PT0	EQU	0	; 0 0	oscillator/4
			; 0 1	oscillator/16
			; 1 0	oscillator/64
			; 1 1	reserved 

_CM	EQU	0	; Only "native" mode XA is supported

%IF(%EQS(%_MODEL,m) OR %EQS(%_MODEL,l))THEN(
_PZ	EQU	0	; Large memory mode
)ELSE(
_PZ	EQU	1	; Page zero mode
)FI

B_SCR	EQU	( 0    SHL 7 ) OR \
		( 0    SHL 6 ) OR \
		( 0    SHL 5 ) OR \
		( 0    SHL 4 ) OR \
		( _PT1 SHL 3 ) OR \
		( _PT0 SHL 2 ) OR \
		( _CM  SHL 1 ) OR \
		( _PZ  SHL 0 )

;	------------- CONFIGURATION BITS ---- RESET PSW --------------------------

_SM	EQU	1	; 1 = System mode
			; 0 = User mode

_TM	EQU	0	; 1 = Trace Mode
			; 0 = XA debugging feature are disabled

_RS	EQU	0	; Register bank number [0..3]

_IM	EQU	15	; Interrupt priority (0-15) 15 indicates highest priority

_C	EQU	0	; Carry flag 
_AC	EQU	0	; Auxilary carry
_V	EQU	0	; Overflow flag
_N	EQU	0	; Negative flag
_Z	EQU	0	; Zero flag

B_PSWH	EQU	( _SM  SHL 7 ) OR \
		( _TM  SHL 6 ) OR \
		( _RS  SHL 4 ) OR \
		( _IM  SHL 0 )

B_PSWL	EQU	( _C   SHL 7 ) OR \
		( _AC  SHL 6 ) OR \
		( 0    SHL 5 ) OR \
		( 0    SHL 4 ) OR \
		( 0    SHL 3 ) OR \
		( _V   SHL 2 ) OR \
		( _N   SHL 1 ) OR \
		( _Z   SHL 0 )

B_PSW	EQU	( B_PSWH SHL 8 ) OR ( B_PSWL )

;	----------------------- RESET VECTOR -------------------------------

RESET_VECTOR	SEGMENT CODE		; Reset vector of the application: locator
	RSEG	RESET_VECTOR		; description file contains absolute address
	DW      B_PSW			; set initail PSW
%IF(%EQS(%_MODEL,m) OR %EQS(%_MODEL,l))THEN(
	DW	__RST			; Chain to the startup code 

RST_VECTOR	SEGMENT CODE		; Reset vector allocated in segment 0
        RSEG    RST_VECTOR		;
__RST:	FJMP    __START			; Jump to the startup code 

DSSEG@DS	SEGMENT	DATA JOIN NOCLEAR
	RSEG	DSSEG@DS		; Empty segment only used by startup code
	PUBLIC	__DS			; and interrupt functions to determine the
__DS:					; default data segment number.

	EXTRN	HCODE( _main )		; start label user C program
	EXTRN	HCODE( _exit )		; start label exit() function		
	EXTRN 	HCODE( __INITSEG )	; label copy-table init function
)ELSE(
	DW      __START			; Vector to the startup code 

	EXTRN	CODE( _main )		; start label user C program
	EXTRN	CODE( _exit )		; start label exit() function	
	EXTRN	CODE( __INITSEG )	; label copy-table init function
)FI

	EXTRN	IDATA( __lc_bs )	; begin of stack label, used by xvwxa
	EXTRN	IDATA( __lc_es )	; end of stack label

	END
