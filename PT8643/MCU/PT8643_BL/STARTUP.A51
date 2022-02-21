$NOMOD51

;------------------------------------------------------------------------------
;  This file is part of the C51 Compiler package
;  Copyright (c) 1988-2002 Keil Elektronik GmbH and Keil Software, Inc.
;------------------------------------------------------------------------------
;  STARTUP.A51:  This code is executed after processor reset.
;
;  To translate this file use A51 with the following invocation:
;
;     A51 STARTUP.A51
;
;  To link the modified STARTUP.OBJ file to your application use the following
;  BL51 invocation:
;
;     BL51 <your object file list>, STARTUP.OBJ <controls>
;
;------------------------------------------------------------------------------
;
;  User-defined Power-On Initialization of Memory
;
;  With the following EQU statements the initialization of memory
;  at processor reset can be defined:
;
;               ; the absolute start-address of IDATA memory is always 0
IDATALEN        EQU     80H     ; the length of IDATA memory in bytes.
;
XDATASTART      EQU     0H      ; the absolute start-address of XDATA memory
XDATALEN        EQU     0H      ; the length of XDATA memory in bytes.
;
PDATASTART      EQU     0H      ; the absolute start-address of PDATA memory
PDATALEN        EQU     0H      ; the length of PDATA memory in bytes.
;
;  Notes:  The IDATA space overlaps physically the DATA and BIT areas of the
;          8051 CPU. At minimum the memory space occupied from the C51 
;          run-time routines must be set to zero.
;------------------------------------------------------------------------------
;
;  Reentrant Stack Initilization
;
;  The following EQU statements define the stack pointer for reentrant
;  functions and initialized it:
;
;  Stack Space for reentrant functions in the SMALL model.
IBPSTACK        EQU     0       ; set to 1 if small reentrant is used.
IBPSTACKTOP     EQU     0FFH+1  ; set top of stack to highest location+1.
;
;  Stack Space for reentrant functions in the LARGE model.      
XBPSTACK        EQU     0       ; set to 1 if large reentrant is used.
XBPSTACKTOP     EQU     0FFFFH+1; set top of stack to highest location+1.
;
;  Stack Space for reentrant functions in the COMPACT model.    
PBPSTACK        EQU     0       ; set to 1 if compact reentrant is used.
PBPSTACKTOP     EQU     0FFFFH+1; set top of stack to highest location+1.
;
;------------------------------------------------------------------------------
;
;  Page Definition for Using the Compact Model with 64 KByte xdata RAM
;
;  The following EQU statements define the xdata page used for pdata
;  variables. The EQU PPAGE must conform with the PPAGE control used
;  in the linker invocation.
;
PPAGEENABLE     EQU     0       ; set to 1 if pdata object are used.
;
PPAGE           EQU     0       ; define PPAGE number.
;
PPAGE_SFR       DATA    0A0H    ; SFR that supplies uppermost address byte
;               (most 8051 variants use P2 as uppermost address byte)
;
;------------------------------------------------------------------------------

; Standard SFR Symbols 
ACC     DATA    0E0H
B       DATA    0F0H
SP      DATA    81H
DPL     DATA    82H
DPH     DATA    83H
PCA0MD  DATA    0D9h

                NAME    ?C_STARTUP


?C_C51STARTUP   SEGMENT   CODE
?STACK          SEGMENT   IDATA

                RSEG    ?STACK
                DS      1

                EXTRN CODE (?C_START)
                PUBLIC  ?C_STARTUP

;------------------------------
; Interrupt Vector Redirection
;------------------------------
	
; Specify the base address of the TCP/IP Library
BASE_ADDRESS  EQU 0x2400

; Reset Vector @ 0x0000
		CSEG	AT	000h
?C_STARTUP:	LJMP	STARTUP1

; Interrupt 0  @ 0x0003						External Interrupt 0.
		CSEG	AT	003h
		LJMP BASE_ADDRESS + 0x0003

; Interrupt 1  @ 0x000B						Timer 0 Overflow.
		CSEG	AT	00Bh
		LJMP BASE_ADDRESS + 0x000B

; Interrupt 2  @ 0x0013						External Interrupt 1.
		CSEG	AT	013h
		LJMP BASE_ADDRESS + 0x0013

; Interrupt 3  @ 0x001B						Timer 1 Overflow.
		CSEG	AT	01Bh
		LJMP BASE_ADDRESS + 0x001B

; Interrupt 4  @ 0x0023   				UART0.
//		CSEG	AT	023h
//		LJMP BASE_ADDRESS + 0x0023

; Interrupt 5  @ 0x002B						Timer 2.
//		CSEG	AT	02Bh
//		LJMP BASE_ADDRESS + 0x002B

; Interrupt 6  @ 0x0033						SPI Interface.
		CSEG	AT	033h
		LJMP BASE_ADDRESS + 0x0033

; Interrupt 7  @ 0x003B						SMBUS Interface.
//		CSEG	AT	03Bh
//		LJMP BASE_ADDRESS + 0x003B

; Interrupt 8  @ 0x0043						ADC0 Window Comparator.
		CSEG	AT	043h
		LJMP BASE_ADDRESS + 0x0043

; Interrupt 9  @ 0x004B						Programmable Counter Array.
		CSEG	AT	04Bh
		LJMP BASE_ADDRESS + 0x004B

; Interrupt 10 @ 0x0053						Comparator 0 Falling Edge.
		CSEG	AT	053h
		LJMP BASE_ADDRESS + 0x0053

; Interrupt 11 @ 0x005B						Comparator Rising Edge.
		CSEG	AT	05Bh
		LJMP BASE_ADDRESS + 0x005B

; Interrupt 12 @ 0x0063						Comparator 1 Falling Edge.
		CSEG	AT	063h
		LJMP BASE_ADDRESS + 0x0063

; Interrupt 13 @ 0x006B						Comparator 1 Rising Edge.
		CSEG	AT	06Bh
		LJMP BASE_ADDRESS + 0x006B

; Interrupt 14 @ 0x0073						Timer 3.
		CSEG	AT	073h
		LJMP BASE_ADDRESS + 0x0073

; Interrupt 15 @ 0x007B						ADC0 End of Conversion.
		CSEG	AT	07Bh
		LJMP BASE_ADDRESS + 0x007B

; Interrupt 16 @ 0x0083						Timer 4.
		CSEG	AT	083h
		LJMP BASE_ADDRESS + 0x0083

; Interrupt 17 @ 0x008B						ADC2 Window Comparator.
		CSEG	AT	08Bh
		LJMP BASE_ADDRESS + 0x008B

; Interrupt 18 @ 0x0093						ADC2 End of Conversion.
		CSEG	AT	093h
		LJMP BASE_ADDRESS + 0x0093

; Interrupt 19 @ 0x009B						Reserved.
		CSEG	AT	09Bh
		LJMP BASE_ADDRESS + 0x009B

; Interrupt 20 @ 0x00A3						UART1.
		CSEG	AT	0A3h
		LJMP BASE_ADDRESS + 0x00A3

;------------------------------
; Bootloader Startup Code
;------------------------------

                RSEG    ?C_C51STARTUP

STARTUP1:
		
		; Disable Watchdog Timer
 MOV PCA0MD , #000h	
//ANL		PCA0MD,   #0BFH		; disable WDT
		; Initialize RAM (startup.a51 unmodified beyond this point)
IF IDATALEN <> 0
                MOV     R0,#IDATALEN - 1
                CLR     A
IDATALOOP:      MOV     @R0,A
                DJNZ    R0,IDATALOOP
ENDIF

IF XDATALEN <> 0
                MOV     DPTR,#XDATASTART
                MOV     R7,#LOW (XDATALEN)
  IF (LOW (XDATALEN)) <> 0
                MOV     R6,#(HIGH (XDATALEN)) +1
  ELSE
                MOV     R6,#HIGH (XDATALEN)
  ENDIF
                CLR     A
XDATALOOP:      MOVX    @DPTR,A
                INC     DPTR
                DJNZ    R7,XDATALOOP
                DJNZ    R6,XDATALOOP
ENDIF

IF PPAGEENABLE <> 0
                MOV     PPAGE_SFR,#PPAGE
ENDIF

IF PDATALEN <> 0
                MOV     R0,#LOW (PDATASTART)
                MOV     R7,#LOW (PDATALEN)
                CLR     A
PDATALOOP:      MOVX    @R0,A
                INC     R0
                DJNZ    R7,PDATALOOP
ENDIF

IF IBPSTACK <> 0
EXTRN DATA (?C_IBP)

                MOV     ?C_IBP,#LOW IBPSTACKTOP
ENDIF

IF XBPSTACK <> 0
EXTRN DATA (?C_XBP)

                MOV     ?C_XBP,#HIGH XBPSTACKTOP
                MOV     ?C_XBP+1,#LOW XBPSTACKTOP
ENDIF

IF PBPSTACK <> 0
EXTRN DATA (?C_PBP)
                MOV     ?C_PBP,#LOW PBPSTACKTOP
ENDIF

                MOV     SP,#?STACK-1
; This code is required if you use L51_BANK.A51 with Banking Mode 4
; EXTRN CODE (?B_SWITCH0)
;               CALL    ?B_SWITCH0      ; init bank mechanism to code bank 0
                LJMP    ?C_START

                END
