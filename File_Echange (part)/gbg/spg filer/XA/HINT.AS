; HCOUNT	SEGMENT CODE
; HISTORY:
; 960129 Fieldjobs() moved to fieldint.
; 940610 stop line no. modified
; 940608 datawin target now 11 us to 19 us from sync
; 940524 trmask and trampli added.

; extrn code	(fieldjobs)

EXTRN 	_lineno, _detmask, _v24on, _trmask, _trampli
EXTRN 	_port1tab, _port2tab
EXTRN	_testmode
TR0	EQU	0x284

PSECT text	;,bit

ORG 000Bh
;	ljmp	hint


PSECT	text,align=2

hint:
           PUSH.B  ACC              
           PUSH.B  R6H              
           PUSH.B  R6L              
;           PUSH.B  PSW              
;           MOV.B   PSW,#08H         

           MOV.B   R4L,R2H          ;get presaved port2 value
           MOV.W   R6,#08000H       
           MOVX.B  [R6],R4L         ;write port2 value (with START OF DATAWIN)
           MOV.B   R3L,R4L          ;save value

           MOV.B   R4L,_lineno       ;get line no. to get port1 value
           ADD.B   R4L,#LOW _port1tab ;offset by table start

;NOW MAKE A BREAK TO STOP DATAWIN ON PORT2
           XCH.B   R4L,R3L          ;save acc and get port2 value
           AND.B   R4L,#0fbh        ;value for stopping datawin
           MOV.B   R2H,R4L          ;delay
           MOV.B   R2H,R4L          ;delay
           MOV.B   R2H,R4L          ;delay
           MOV.B   R2H,R4L          ;delay
           MOV.B   R2H,R4L          ;delay

           MOVX.B  [R6],R4L         ;STOP DATAWIN
	
           MOV.B   R4L,R3L          ;get port2 value back
           MOV.B   R6L,R4L          
           MOVS    R4L,#0           
           ADDC.B  R4L,#HIGH _port1tab 
           MOV.B   R6H,R4L          
           MOVX.B  R4L,[R6]         ;get port1 value
           OR.B    R4L,_detmask      ;insert det. positions
           MOV.W   R6,#0c000h       ;write to port
           MOVX.B  [R6],R4L         


; PREPARE NEXT LINE
           MOV.B   R4L,_lineno       ;get line no.
           ADDS.B  R4L,#1           
           ADD.B   R4L,#LOW _port2tab ;offset by table start
           MOV.B   R6L,R4L          
           MOVS    R4L,#0           
           ADDC.B  R4L,#HIGH _port2tab 
           MOV.B   R6H,R4L          
           MOVX.B  R4L,[R6]         ;get port2 value
           OR.B    R4L,_v24on        ;add bit for V24-out control
           OR.B    R4L,_trmask       ;set mode (play-address)
           OR.B    R4L,_trampli      ;set amplitude
           JNB     _testmode,notst   ;set testmode
           SETB    R4L.0            
;********* WARNING : Please double check the bit maps above
notst:
	   MOV.B   R2H,R4L          ;save it until next line
           MOV.B   R4L,_lineno       ;stop if line 23 (line=17)
           CJNE.B  R4L,#17,ck36     
           CLR     TR0              
;********* WARNING : Please double check the bit maps above
           BR      finish           
ck36:
           CJNE.B  R4L,#35,morelin  ;F2: stop if line 313+23 (line=35)
           CLR     TR0              
;********* WARNING : Please double check the bit maps above
;	setb	fieldjob
finish:				;stop transmit from line 23 (336)
           MOV.B   R0H,#20          ;wait 1/2 line
XA_ADJUST_0000: DJNZ.B  R0H,$       
           MOV.B   R4L,R2H          
           MOV.W   R6,#08000H       
           MOVX.B  [R6],R4L         ;write port2 value (with start of datawin)
           MOV.B   _lineno,#0        ;reset line no. <960129a

;           POP.B   PSW              
           POP.B   R6L              
           POP.B   R6H              
           POP.B   ACC              
           RETI                     
	
morelin:
           ADDS.B  _lineno,#1        ;next line
;           POP.B   PSW              
           POP.B   R6L              
           POP.B   R6H              
           POP.B   ACC              
           RETI                     
end
