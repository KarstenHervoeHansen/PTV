;******************************************************
;		   TPCRT.ASM 5.08
;		Low level CRT routines
;	 Copyright (c) TurboPower Software 1987.
; Portions copyright (c) Sunny Hill Software 1985, 1986
;     and used under license to	TurboPower Software
;		 All rights reserved.
;******************************************************

	INCLUDE	TPCOMMON.ASM

;******************************************************	Equates

;structure of a	Turbo FIB (File	Interface Block)

FHandle		EQU	(WORD PTR 0)
FMode		EQU	(WORD PTR 2)
FBufSize	EQU	(WORD PTR 4)
FPrivate	EQU	(WORD PTR 6)
FBufPos		EQU	(WORD PTR 8)
FBufEnd		EQU	(WORD PTR 10)
FBufPtr		EQU	(DWORD PTR 12)
FOpenProc	EQU	(DWORD PTR 16)
FInOutProc	EQU	(DWORD PTR 20)
FFlushProc	EQU	(DWORD PTR 24)
FCloseProc	EQU	(DWORD PTR 28)
FUserData	EQU	(BYTE PTR 32)
FName		EQU	(BYTE PTR 48)
FBuffer		EQU	(BYTE PTR 128)

;FMode constants

FMClosed	=	0D7B0h
FMInput		=	0D7B1h
FMOutput	=	0D7B2h
FMInOut		=	0D7B3h

;equates for referencing pointers to FIB's on the stack

TextFile	EQU	DWORD PTR SS:[BX+4]
TextFileBP	EQU	[BP+6]
TFPtr		EQU	ES:[DI]

;keys /	control	characters

Null		=	0
CtrlA		=	1
CtrlC		=	3
CtrlD		=	4
CtrlF		=	6
Bell		=	7
BkSp		=	8
LineFeed	=	10
Return		=	13
CR		=	13
CtrlR		=	18
CtrlS		=	19
CtrlX		=	24
CtrlZ		=	26
Escape		=	27
CtrlBkSp	=	127

;******************************************************	Macros

Write		MACRO	Char
		MOV	AL, Char	;Al has	character to write
		CALL	WriteChar	;Write it
		ENDM

WriteLn		MACRO
		Write	CR		;Write the carriage return
		Write	LineFeed	;Write the line	feed
		ENDM

;******************************************************	Data

DATA	SEGMENT	BYTE PUBLIC

	;Pascal	variables

	EXTRN	CheckBreak : BYTE		;Check for ^Break?
	EXTRN	CheckEof : BYTE			;Check for ^Z on Read/Ln?
	EXTRN	DirectVideo : BYTE		;If false, use BIOS
	EXTRN	WindMin	: WORD			;Min. XY coordinates
	EXTRN	WindMax	: WORD			;Max. XY coordinates
	EXTRN	LastMode : WORD			;Current video mode
	EXTRN	CurrentMode : BYTE		;Current video mode
	EXTRN	CurrentPage : BYTE		;Current video page
	EXTRN	ScreenWidth : BYTE		;Current width of display
	EXTRN	VirtualWidth : BYTE		;Current width of virtual display
	EXTRN	ScreenHeight : BYTE		;Current height	of display
	EXTRN	CurrentHeight :	BYTE		;Current height	of display-1
	EXTRN	CurrentDisplay : BYTE		;Current display type
	EXTRN	EnhancedDisplay	: BYTE		;Type of enhanced display
						;installed, if any
	EXTRN	WhichHerc : BYTE		;Type of Herc card installed
	EXTRN	InTextMode : BYTE		;False if in graphics mode
	EXTRN	TextAttr : BYTE			;Current video attribute
	EXTRN	TextChar : BYTE			;background character
	EXTRN	NormalAttr : BYTE;		;Attribute for NormVideo
	EXTRN	CheckSnow : BYTE		;If true, check	for retrace
	EXTRN	CtrlBreakFlag :	BYTE		;True if ^Break	pressed
	EXTRN	VideoSegment : WORD		;Segment of Video Memory
	EXTRN	VirtualSegment : WORD		;Segment of Video Memory--alt
	EXTRN	BufLen : WORD			;Max length of string for Read
	EXTRN	ExitProc : DWORD		;Turbo's ExitProc pointer
	EXTRN	Save1B : DWORD			;Previous INT 1Bh handler
	EXTRN	DetectMultitasking : BYTE	;If True, automatically	checks
						;  for multitasker
	EXTRN	MultitaskingOn : BYTE		;True if running under
						;  multitasker
	EXTRN	IsCompaq : BYTE			;True if system	is a COMPAQ
	EXTRN	BiosScroll : BYTE		;If False, use special scroll
						;  routine
	EXTRN	DisplayOverride	: BYTE		;override auto-detection
	EXTRN	NextChar : BYTE			;Used by KeyPressed/ReadKey
	EXTRN	OneMS :	WORD			;Loop count for	1 ms delay
	EXTRN	SaveExitProc :DWORD		;Saves old ExitProc

DATA	ENDS

;******************************************************	Code

CODE	SEGMENT	BYTE PUBLIC

	ASSUME	CS:CODE, DS:DATA

	;Pascal	routines

	EXTRN	WhichHercCard :	NEAR	;Herc detection	routine

	;standard CRT unit routines

	PUBLIC	ReadKey, InitCrt, AssignCrt

	;extensions

	PUBLIC	ReInitCrt

	;low-level routines

	PUBLIC	ReadCursorPrim,	SetCursorPrim, GetCursorPrim, GetCrtModePrim
	PUBLIC	ScrollUpPrim, ScrollDownPrim, AdapterCheck, DelayMS
	PUBLIC	GetCharAttr, SetWindowPrim, FullWindow,	GetAttribute

	;data in code segment

	OurDS	DW	DATA		;Init'd by EXE loader to value of DS

AdapterTable1	LABEL BYTE
	DB	UnknownD	;0 = no	display
	DB	Mono		;1 = mono
	DB	CGA		;2 = CGA
	DB	UnknownD	;3 = reserved
	DB	EGA		;4 = EGA w/color
	DB	EGA		;5 = EGA w/mono
	DB	PGC		;6 = Professional Graphics controller
	DB	VGA		;7 = VGA w/mono
	DB	VGA		;8 = VGA w/color
	DB	UnknownD	;9 = reserved
	DB	MCGA		;A = MCGA w/digital color
	DB	MCGA		;B = MCGA w/mono
	DB	MCGA		;C = MCGA w/analog color

AdapterTable2	LABEL BYTE
	DB	Unenhanced	;0 = not enhanced
	DB	Unenhanced	;1 = "
	DB	Unenhanced	;2 = "
	DB	Unenhanced	;3 = "
	DB	EGA		;4 = EGA w/color
	DB	EGA		;5 = EGA w/mono
	DB	PGC		;6 = Professional Graphics controller
	DB	VGA		;7 = VGA w/mono
	DB	VGA		;8 = VGA w/color
	DB	Unenhanced	;9 = not enhanced
	DB	MCGA		;A = MCGA w/digital color
	DB	MCGA		;B = MCGA w/mono
	DB	MCGA		;C = MCGA w/analog color

;******************************************************	AdapterCheck

JunkValue	=	0FFFFh

AdapterCheck	PROC NEAR

	MOV	WP ScreenHeight,25	;Assume	25-line	mode
	MOV	WP CurrentHeight,24
	AND	WP LastMode,00FFh	;Clear high byte of LastMode

	MOV	AL,DisplayOverride	;check for override
	CMP	AL,-1			;-1 means no override
	JE	ACauto			;if -1,	use auto detect
	CMP	AL,VGA			;is it in Mono..VGA?
	JA	ACauto			;if not, use auto detect
	MOV	CurrentDisplay,AL	;use the override
	MOV	EnhancedDisplay,AL
	CMP	AL,MCGA			;is MCGA or higher indicated?
	JAE	ACgetheight		;if so,	check height
	MOV	EnhancedDisplay,0	;not enhanced
	MOV	CurrentDisplay,CGA	;assume	CGA
	CMP	CurrentMode,7		;Current mode =	7?
	JNE	ACexit1			;if not, done
	MOV	CurrentDisplay,Mono	;else, mono
ACexit1:
	RET				;done
ACgetHeight:
	JMP	SHORT ACheight

	;test for VGA, MCGA
ACauto:
	VidCallAX	1A00h		;Video display combination
	CMP	AL,1Ah			;AL = $1A signals valid	call
	JNE	ACega			;Check for EGA if invalid

	;get current display type
	MOV	DX,BX			;move result from BX into DX
	CMP	DL,0Ch			;DL should be in range 1..C
	JA	ACega
	MOV	AL,DL			;translate into	our terms
	MOV	BX,Offset AdapterTable1
	XLAT	BYTE PTR CS:[0]
	CMP	AL,UnknownD		;check for unknown adapter
	JE	ACega			;use EGA test after all
	MOV	CurrentDisplay,AL	;save result

	;get enhanced display type
	MOV	AL,DL			;assume	one display
	OR	DH,DH			;DH of 0 means one display
	JZ	ACone
	CMP	DH,0Ch			;else DH should	be in range 1..C
	JA	ACone
	CMP	DH,DL			;if two, enhanced has higher value
	JB	ACone
	MOV	AL,DH
ACone:	MOV	BX,Offset AdapterTable2
	XLAT	BYTE PTR CS:[0]
	MOV	EnhancedDisplay,AL	;save result
	JMP	SHORT ACHeight		;Check the height

	;test for EGA
ACega:	MOV	CurrentDisplay,EGA	;Assume	EGA
	MOV	EnhancedDisplay,EGA
	MOV	BX,0FF10h		;Return	EGA information
	MOV	CX,JunkValue		;Load CX with junk value
	VidCallAX	1200h		;Alternate function select
	SetZero	AL			;AL = 0
	CMP	CX,JunkValue		;CX unchanged?
	JE	ACnotEnhanced		;If so,	not an enhanced	display
	CMP	BH,1			;BH should be 0	or 1
	JA	ACnotEnhanced		;Mono or CGA if	it isn't

	;See if	EGA/VGA	is the active monitor
	CMP	BH,1			;mono display?
	JE	ACMono			;If so,	use mono check
	CMP	CurrentMode,7		;In mono mode?
	JE	ACPlain			;If so,	we're not on the EGA/VGA/MCGA
	JMP	SHORT ACHeight		;Else check height
ACMono:
	CMP	CurrentMode,7		;Current mode =	7?
	JNE	ACPlain			;Exit if not

	;check the height of the display
ACHeight:
	MOV	CX,40h			;CX = $40
	MOV	ES,CX			;ES = $40
	MOV	CL,ES:[84h]		;CL = Number of	rows - 1
	MOV	CurrentHeight,CL	;Set CurrentHeight and ScreenHeight
	INC	CL
	MOV	ScreenHeight,CL
	CMP	CL,25			;Is it 25-line mode?
	JE	ACexit			;If so,	done
	OR	WP LastMode,Font8x8	;Set Font8x8 bit
	CMP	CL,43			;Is it 43-line mode?
	JE	ACexit			;if so,	done
	CMP	CL,50			;Is it 50-line mode?
	JE	ACexit			;if so,	done
	AND	WP LastMode,00FFh	;Clear Font8x8 bit of LastMode
ACexit:
	RET

ACnotEnhanced:
	MOV	EnhancedDisplay,0	;No enhanced display installed

ACPlain:
	CMP	CurrentDisplay,EGA	;Set to	EGA?
	JNE	ACExit			;Exit if not
	MOV	CurrentDisplay,CGA	;Assume	CGA
	CMP	CurrentMode,7		;Mono mode
	JNE	ACExit			;Done if not
	MOV	CurrentDisplay,Mono	;Else, Mono
	RET

AdapterCheck	ENDP

;******************************************************	BreakCheck

;Check for ^Break

BreakCheck	PROC NEAR

	CMP	CtrlBreakFlag,False	;Break flag set?
	JE	BreakExit		;Done if not
	MOV	CtrlBreakFlag,False	;Reset break flag

FlushKbdBuffer:
	KbdCall	1			;Key pressed function
	JZ	BreakCtrlC		;If no keypress, continue
	KbdCall	0			;Read next character
	JMP	SHORT FlushKbdBuffer	;Repeat

BreakCtrlC:
	Write	'^'			;Write the caret
	Write	'C'			;Write the 'C'
	WriteLn				;Write a CR/LF
	INT	23h			;Abort

BreakExit:
	RET

BreakCheck	ENDP

;******************************************************	ReadKey

;function ReadKey : Char;
;Read a	character from the keyboard

ReadKey	PROC FAR

	MOV	AL,NextChar		;AL = NextChar
	MOV	NextChar,0		;NextChar = 0
	OR	AL,AL			;See if	NextChar = 0
	JNZ	ReadKeyExit		;If not, we're done

ReadKeyStart:
	INT	28h			;Help keep popups from locking out
					;other TSR's
	KbdCall	1			;Key pressed function
	JZ	ReadKeyStart		;If no keypress, loop

	KbdCall	0			;Read next char	service
	OR	AL,AL			;Check for AL =	0
	JNZ	ReadKeyExit		;Exit if AL <> 0

	MOV	NextChar,AH		;Save extended scan code
	OR	AH,AH			;Check for scan	code of	0 (^Break)
	JNZ	ReadKeyExit		;Exit if AH <> 0
	MOV	AL,CtrlC		;Else, AL = ^C

ReadKeyExit:
	CALL	BreakCheck		;Check for break if necessary
	RET

ReadKey	ENDP

;******************************************************	SetCursorPrim

;Primitive routine to set cursor coordinates
;On entry, DH has row (Y), DL has column (X)

SetCursorPrim	PROC NEAR

	CMP	DirectVideo,False	;Direct	video flag on?
	JE	SCNotDirect		;If not, use BIOS call
	CMP	MultitaskingOn,True	;Multitasking flag on?
	JE	SCNotDirect		;If so,	use BIOS call

	;Set cursor position in	BIOS data area
	MOV	BX,0040h		;BX = $40
	MOV	ES,BX			;ES = $40
	MOV	BL,CurrentPage		;BL = CurrentPage, BH =	0
	SHL	BX,1			;Multiply by 2
	MOV	ES:50h[BX],DX		;DX has	cursor position

	;program the 6845
	SetZero	AH
	MOV	AL,DH			;AL = Row
	MUL	ScreenWidth		;AX = Row * width of screen
	SetZero	DH			;DX = Column (in DL, DH	is 0)
	ADD	AX,DX			;AX = (Row * width) + Column
	SHL	AX,1			;Multiply by 2
	ADD	AX,ES:[4Eh]		;DX has	starting offset	of video memory
	SHR	AX,1			;Divide	by 2
	MOV	BX,AX			;BX = (Row * width) + Column
	MOV	DX,ES:[63h]		;DX has	address	of 6845	($03D4/$03B4)

	MOV	AL,14			;Select	cursor address (high) register
	OUT	DX,AL			;Write the byte
	NullJump			;Wait
	MOV	AL,BH			;Get high byte of word
	INC	DX			;Next register ($03D5/$03B5)
	OUT	DX,AL			;Write the byte

	NullJump			;Wait
	DEC	DX			;Previous register ($03D4/$03B4)

	MOV	AL,15			;Select	cursor address (low) register
	OUT	DX,AL			;Write the byte
	NullJump			;Wait
	MOV	AL,BL			;Get low byte of word
	INC	DX			;Next register ($03D5/$03B5)
	OUT	DX,AL			;Write the byte
	NullJump			;Wait
	RET

SCNotDirect:
	;Set cursor position with BIOS call
	MOV	BH,CurrentPage		;BH = current display page
	VideoCall	2		;Set cursor service
	RET

SetCursorPrim	ENDP

;******************************************************	GetCursorPrim

;Primitive routine to get cursor coordinates
;On exit, DH has row, DL has column, CH	has starting scan line,	CL has ending

GetCursorPrim	PROC NEAR

	CMP	DirectVideo,False	;Direct	video flag on?
	JE	GCNotDirect		;If not, use BIOS call
	CMP	MultitaskingOn,True	;Multitasking flag on?
	JE	GCNotDirect		;If so,	use BIOS call

	;get cursor position from BIOS data area
	MOV	BX,0040h		;BX = $40
	MOV	ES,BX			;ES = $40
	MOV	BL,CurrentPage		;BL = CurrentPage, BH =	0
	SHL	BX,1			;Multiply by 2
	MOV	DX,ES:50h[BX]		;DX has	cursor position	for CurrentPage
	MOV	CX,ES:60h		;CX has	cursor shape
	RET

GCNotDirect:
	;get cursor position with BIOS call
	MOV	BH,CurrentPage		;BH = current display page
	VideoCall	3		;Read cursor service
	RET

GetCursorPrim	ENDP

;******************************************************	ReadCursorPrim

;Primitive routine to get cursor size and position
;On exit, DH has row, DL has column, AH	has starting scan line,	AL has ending

ReadCursorPrim	PROC NEAR

	CALL	GetCursorPrim
	MOV	AX,CX			;Scan lines from CX into AX
	RET

ReadCursorPrim	ENDP

;******************************************************	DelayMS

;Delay one millisecond

DelayMS	PROC NEAR

	MOV	BX,4
DMSdec:	DEC	BX
	JNZ	DMSdec
	CMP	AL,ES:[DI]
	LOOPE	DelayMS
	RET

DelayMS	ENDP

;******************************************************	CrtExitProc

;Invoked when a	program	is ending. Restore INT 1B vector.

CrtExitProc	PROC FAR

	GetPtr	SaveExitProc		;ES:DI points to old ExitProc
	SetPtr	ExitProc, ES, DI	;Restore old ExitProc
	MOV	CtrlBreakFlag,False	;Reset CtrlBreakFlag
	PUSH	DS			;Save DS
	LDS	DX,Save1B		;DS:DX has saved vector
	MOV	AX,251Bh		;AH = Set interrupt vector service
					;AL = interrupt	vector
	INT	21h			;Call DOS
	POP	DS			;Restore DS
	RET

CrtExitProc	ENDP

;******************************************************	Int1B

;Handles INT 1Bh, ^Break interrupt

Int1B	PROC NEAR

	PUSH	DS			;Save DS
	PUSH	AX			;Save AX
	MOV	DS,CS:OurDS		;Restore our DS
	MOV	AL,CheckBreak		;If CheckBreak is True...
	MOV	CtrlBreakFlag,AL	;CtrlBreakFlag will be True also
	POP	AX			;Restore AX
	POP	DS			;Restore DS
	IRET

Int1B	ENDP

;******************************************************	GetCharAttr

;Get current character and attribute
;Returns attribute in AH, character in AL

GetCharAttr	PROC NEAR

	MOV	BH,CurrentPage		;BH = current video page
	VideoCall	8		;Read char and attribute service
	RET

GetCharAttr	ENDP

;******************************************************	GetAttribute

;Get current attribute at cursor

GetAttribute	PROC NEAR

	CALL	GetCharAttr		;Get char and attribute
	AND	AH,01111111b		;Mask out the blink bit
	MOV	TextAttr,AH		;Attribute at cursor is	in AH
	MOV	NormalAttr,AH		;Attribute for NormVideo
	RET

GetAttribute	ENDP

;******************************************************	GetCrtModePrim

;Return	the current video mode in AL, initialize most global screen variables

GetCrtModePrim	PROC NEAR

	VideoCall	0Fh		;Get video mode	service
	AND	AL,7Fh			;mask out high bit, sometimes set on EGA
	MOV	CurrentMode,AL		;Save video mode
	MOV	ScreenWidth,AH		;Save width
	MOV	VirtualWidth,AH
	MOV	CurrentPage,BH		;Save page
	MOV	InTextMode,True		;Assume	text mode
	SetZero	DX			;DX = 0
	MOV	CX,0B000h		;Assume	mono card
	CMP	AL,7			;In monochrome mode?
	JE	GetCrtExit		;If so,	exit

	;get the segment of video memory for the current page

	MOV	CX,40h			;CX = $40
	MOV	ES,CX			;ES = $40
	MOV	CX,ES:[4Eh]		;CX = MemW[$40:$4E]
	SHR	CX,1			;Convert offset	to paragraph
	SHR	CX,1
	SHR	CX,1
	SHR	CX,1
	ADD	CX,0B800h		;Color card

	;make sure we're not in graphics mode
	CMP	AL,3			;In mode 0-3?
	JNA	GetCrtExit		;If so,	in text	mode
	MOV	InTextMode,False	;Else, graphics	mode

GetCrtExit:
	MOV	VideoSegment,CX		;Save video segment
	MOV	VirtualSegment,CX
	CMP	DetectMultitasking,True	;Check for multitasker?
	JE	GCcheck			;If so,	do the check
	MOV	MultitaskingOn,False	;Else, turn the	flag off
	RET

	;check for multitasker
GCcheck:
	MOV	ES,CX			;Put video segment in ES
	SetZero	DI			;DI = 0
	PUSH	BP			;Save BP
	VidCallAX	0FE00h		;Get segment of	virtual	screen
	POP	BP			;Restore BP
	SetZero	AL			;AL = False
	MOV	BX,ES			;Return	value into BX
	CMP	BX,VideoSegment		;Same as before?
	JE	GCno			;If so,	not multitasking
	MOV	VideoSegment,BX		;Save segment
	MOV	VirtualSegment,BX
	INC	AL			;AL = True
GCno:
	MOV	MultitaskingOn,AL	;Save multitasking flag
	MOV	AL,CurrentMode		;Return	current	mode in	AL
	RET

GetCrtModePrim	ENDP

;******************************************************	SetWindowPrim

;Primitive routine to set current window coordinates
;On entry, the new Y coordinates must be in CX,	X coordinates in DX

SetWindowPrim	PROC NEAR

	MOV	WindMin.XLow,DL		;Set XLow
	MOV	WindMax.XHigh,DH	;Set XHigh
	MOV	WindMin.YLow,CL		;Set YLow
	MOV	WindMax.YHigh,CH	;Set YHigh
	RET

SetWindowPrim	ENDP

;******************************************************	FullWindow

;Makes the entire screen the window

FullWindow	PROC NEAR

	SetZero	DL			;XLow =	0
	MOV	CL,DL			;YLow =	0
	MOV	DH,ScreenWidth		;DH = ScreenWidth-1
	DEC	DH
	MOV	CH,CurrentHeight	;CH = ScreenHeight-1 = CurrentHeight
	CALL	SetWindowPrim		;Call primitive	routine
	RET

FullWindow	ENDP

;******************************************************	ScrollUpPrim

;Primitive routine to scroll a window up, with optional	snow/flicker prevention

COMMENT	|

On entry:
	BH has the video attribute to use as filler
	CH has top row of window
	CL has left column
	DH has bottom row
	DL has right column
	AL has number of lines to scroll, AL = 0 means clear the whole window
|

SPcharsPerRow	EQU	BYTE PTR [BP-1]	;# of char/attr	pairs per window row
SProwsToMove	EQU	BYTE PTR [BP-2]	;# of rows to move
SProwsToFill	EQU	BYTE PTR [BP-3]	;# of rows to fill
SPattr		EQU	BYTE PTR [BP-4]	;attribute to fill with
SPchar		EQU	BYTE PTR [BP-5]	;char to fill with

ScrollUpPrim	PROC NEAR

	CMP	BiosScroll,False	;Use special scroll routine?
	JE	SUspecial		;If not, use BIOS
	CMP	TextChar,' '		;Use ours if TextChar <> ' '
	JNE	SUspecial
	VideoCall	06h		;Scroll	window up service
	RET

SUspecial:
	CLD				;Go forward
	PUSH	DS			;Save DS
	StackFrameBP
	SUB	SP,5			;Make room for locals
	MOV	SPattr,BH		;Save attribute
	MOV	AH,TextChar		;Save character
	MOV	SPchar,AH
	MOV	TextChar,' '		;Reset TextChar

	;Get rows in window into AH
	MOV	AH,DH			;AH = bottom row
	SUB	AH,CH			;AH = bottom row - top row
	INC	AH			;AH = Succ(bottom row -	top row)

	;Get rows to scroll into AL
	CMP	AL,AH			;AL > AH?
	JNA	SUcheckZero		;If not, check for AL =	0
	MOV	AL,AH			;Else, AL = AH
	JMP	SHORT SUnotWhole	;Continue
SUcheckZero:
	OR	AL,AL			;Is AL 0?
	JNZ	SUnotWhole		;If not, use actual value
	MOV	AL,AH			;Else, scroll entire window
SUnotWhole:
	MOV	SProwsToFill,AL		;Save #	of rows	to fill
	SUB	AH,AL			;AH = #	of rows	to move
	MOV	SProwsToMove,AH		;Save #	of rows	to move

	;Get # of chars	per row
	SetZero	AH			;AH = 0
	MOV	AL,DL			;AL = right column
	SUB	AL,CL			;AL = right column - left column
	INC	AL			;AL = Succ(right column	- left column)
	MOV	SPcharsPerRow,AL	;Save #	of char/attr pairs per row
	MOV	SI,AX			;Save in SI too

	;Get starting value for	DI
	MOV	BX,WP ScreenWidth	;BX = Chars per	screen row
	MOV	AL,CH			;AX = top row
	MUL	BX			;AX = row * current width
	SetZero	CH			;CH = 0, CX = LeftCol
	ADD	AX,CX			;AX = (row * current width) + col
	SHL	AX,1			;account for attribute bytes
	MOV	DI,AX			;DI has	starting offset	for top	left
					; corner of window

	SHL	SI,1			;SI = bytes per	window row
	SHL	BX,1			;BX = Video words per screen row
	MOV	CX,BX			;CX = Video words per screen row
	SUB	BX,SI			;BX has	delta to add when looping

	SetZero	AH			;AH = 0
	MOV	AL,SProwsToFill		;AX = #	of rows	to fill
	MUL	CX			;AX = rows to fill * words/screen row
	MOV	SI,DI			;SI = DI
	ADD	SI,AX			;Point SI to first row to move

	MOV	AL,CheckSnow		;Grab this before changing DS
	MOV	DX,VideoSegment		;DX = VideoSegment
	MOV	ES,DX			;ES = VideoSegment
	MOV	DS,DX			;DS = VideoSegment
	SetZero	CH			;CH = 0

	SHR	AL,1			;Snow checking on?
	JNC	SUfullSpeed		;Skip next check if not
	CMP	DH,0B8h			;See if	we're moving/filling CGA memory
	JNE	SUfullSpeed		;If not, don't wait for retrace

	;Move screen data (with	snow prevention)
	MOV	DX,03DAh		;Point DX to CGA status	port
	CMP	SProwsToMove,0		;Any rows to move?
	JE	SUstartFillWait		;If not, start filling
SUmoveWait:
	MOV	CL,SPcharsPerRow	;CX = #	of char/attr pairs to move
	WordMoveNoSnow			;Move CX words,	preventing snow
	ADD	DI,BX			;Point DI to next row
	ADD	SI,BX			;Point SI to next row
	DEC	SProwsToMove		;Decrement # of	rows to	move
	JNZ	SUmoveWait		;If not	zero, repeat

	;Fill with blanks (with	snow prevention)
SUstartFillWait:
	MOV	SI,BX			;Delta into SI
	MOV	BH,SPattr		;Attribute into	BH
	MOV	BL,SPchar		;Filler	into BL
SUfillWait:
	MOV	CL,SPcharsPerRow	;CX = #	of char/attr pairs to fill
SUfillNext:
	WaitForRetrace			;Wait for opportunity to write
	WordToCGA	BX		;Move the word
	LOOP	SUfillNext		;Fill next video word
	ADD	DI,SI			;Point DI to next row
	DEC	SProwsToFill		;Decrement # of	rows to	fill
	JNZ	SUfillWait		;Repeat	if not zero
	JMP	SHORT SUExit		;All done

	;Move screen data (no snow prevention)

SUfullSpeed:
	MOV	DL,SPcharsPerRow	;DL = #	of char/attr pairs to move
	MOV	DH,SProwsToMove		;DH = #	of rows	to move
	MOV	AH,SPattr		;Attribute into	AH
	MOV	AL,SPchar		;Filler	into AL
	OR	DH,DH			;Any rows to move?
	JZ	SUstartFillNoWait	;If not, start filling
SUmoveNoWait:
	MOV	CL,DL			;CX = #	of char/attr pairs to move
	REP	MOVSW			;Move 'em
	ADD	DI,BX			;Point DI to next row
	ADD	SI,BX			;Point SI to next row
	DEC	DH			;Decrement # of	rows to	move
	JNZ	SUmoveNoWait		;If not	zero, repeat

	;Fill with blanks (no snow prevention)

SUstartFillNoWait:
	MOV	DH,SProwsToFill		;DH = #	of rows	to fill
SUfillNoWait:
	MOV	CL,DL			;CX = #	of char/attr pairs to move
	REP	STOSW			;Fill this row
	ADD	DI,BX			;Point DI to next row
	DEC	DH			;Decrement # of	rows to	fill
	JNZ	SUfillNoWait		;Repeat	if not zero

SUExit:
	MOV	SP,BP			;Restore SP
	POP	BP			;Restore BP
	POP	DS			;Restore DS
	RET				;Return

ScrollUpPrim	ENDP

;******************************************************	ScrollDownPrim

;Primitive routine to scroll a window down, with optional snow/flicker prevention

COMMENT	|

On entry:
	BH has the video attribute to use as filler
	CH has top row of window
	CL has left column
	DH has bottom row
	DL has right column
	AL has number of lines to scroll, AL = 0 means clear the whole window
|

ScrollDownPrim	PROC NEAR

	CMP	BiosScroll,False	;Use special scroll routine?
	JE	SDspecial		;If not, use BIOS
	CMP	TextChar,' '		;Use ours if TextChar <> ' '
	JNE	SDspecial
	VideoCall	07h		;Scroll	window down service
	RET

SDspecial:
	CLD				;Go forward
	PUSH	DS			;Save DS
	StackFrameBP
	SUB	SP,5			;Make room for locals
	MOV	SPattr,BH		;Save attribute
	MOV	AH,TextChar		;Save character
	MOV	SPchar,AH
	MOV	TextChar,' '		;Reset TextChar

	;Get rows in window into BL
	MOV	BL,DH			;BL = bottom row
	SUB	BL,CH			;BL = bottom row - top row
	INC	BL			;BL = Succ(bottom row -	top row)

	;Get rows to scroll into AL
	CMP	AL,BL			;AL > BL?
	JNA	SDcheckZero		;If not, check for AL =	0
	MOV	AL,BL			;Else, AL = AH
	JMP	SHORT SDnotWhole	;Continue
SDcheckZero:
	OR	AL,AL			;Is AL 0?
	JNZ	SDnotWhole		;If not, use actual value
	MOV	AL,BL			;Else, scroll entire window
SDnotWhole:
	MOV	SProwsToFill,AL		;Save #	of rows	to fill
	SUB	BL,AL			;BL = #	of rows	to move
	MOV	SProwsToMove,BL		;Save #	of rows	to move

	;Get # of chars	per row
	SetZero	AH			;AH = 0
	MOV	AL,DL			;AL = right column
	SUB	AL,CL			;AL = right column - left column
	INC	AL			;AL = Succ(right column	- left column)
	MOV	SPcharsPerRow,AL	;Save #	of char/attr pairs per row
	MOV	SI,AX			;Save in SI too

	;Get starting value for	SI
	MOV	AL,DH			;AX = bottom row
	MOV	BX,WP ScreenWidth	;BX = Chars per	screen row
	MUL	BX			;AX = row * current width
	SetZero	CH			;CH = 0, CX = LeftCol
	ADD	AX,CX			;AX = (row * current width) + col
	SHL	AX,1			;account for attribute bytes
	MOV	DI,AX			;SI has	starting offset	for last row

	SHL	SI,1			;SI = bytes per	window row
	SHL	BX,1			;BX = Video words per screen row
	MOV	CX,BX			;CX = Video words per screen row
	ADD	BX,SI			;BX has	delta to subtract when looping

	SetZero	AH			;AH = 0
	MOV	AL,SProwsToFill		;AX = #	of rows	to fill
	MUL	CX			;AX = rows to fill * words/screen row
	MOV	SI,DI			;SI = DI
	SUB	SI,AX			;Point SI to first row to move

	MOV	AL,CheckSnow		;Grab this before changing DS
	MOV	DX,VideoSegment		;DX = VideoSegment
	MOV	ES,DX			;ES = VideoSegment
	MOV	DS,DX			;DS = VideoSegment
	SetZero	CH			;CH = 0

	SHR	AL,1			;Snow checking on?
	JNC	SDfullSpeed		;Skip next check if not
	CMP	DH,0B8h			;See if	we're moving/filling CGA memory
	JNE	SDfullSpeed		;If not, don't wait for retrace

	;Move screen data (with	snow prevention)
	MOV	DX,03DAh		;Point DX to CGA status	port
	CMP	SProwsToMove,0		;Any rows to move?
	JE	SDstartFillWait		;If not, start filling
SDmoveWait:
	MOV	CL,SPcharsPerRow	;CX = #	of char/attr pairs to move
	WordMoveNoSnow			;Move CX words,	preventing snow
	SUB	DI,BX			;Point DI to previous row
	SUB	SI,BX			;Point SI to previous row
	DEC	SProwsToMove		;Decrement # of	rows to	move
	JNZ	SDmoveWait		;If not	zero, repeat

	;Fill with blanks (with	snow prevention)
SDstartFillWait:
	MOV	SI,BX			;Delta into SI
	MOV	BH,SPattr		;Attribute into	BH
	MOV	BL,SPchar		;Filler	into BL
SDfillWait:
	MOV	CL,SPcharsPerRow	;CX = #	of char/attr pairs to fill
SDfillNext:
	WaitForRetrace			;Wait for opportunity to write
	MOV	AX,BX			;Get word back into AX
	STOSW				;Store the word
	STI				;Allow interrupts
	LOOP	SDfillNext		;Fill next video word
	SUB	DI,SI			;Point DI to previous row
	DEC	SProwsToFill		;Decrement # of	rows to	fill
	JNZ	SDfillWait		;Repeat	if not zero
	JMP	SHORT SDExit		;All done

	;Move screen data (no snow prevention)

SDfullSpeed:
	MOV	DL,SPcharsPerRow	;DL = #	of char/attr pairs to move
	MOV	DH,SProwsToMove		;DH = #	of rows	to move
	MOV	AH,SPattr		;Attribute into	AH
	MOV	AL,SPchar		;Filler	into AL
	OR	DH,DH			;Any rows to move?
	JZ	SDstartFillNoWait	;If not, start filling
SDmoveNoWait:
	MOV	CL,DL			;CX = #	of char/attr pairs to move
	REP	MOVSW			;Move 'em
	SUB	DI,BX			;Point DI to previous row
	SUB	SI,BX			;Point SI to previous row
	DEC	DH			;Decrement # of	rows to	move
	JNZ	SDmoveNoWait		;If not	zero, repeat

	;Fill with blanks (no snow prevention)

SDstartFillNoWait:
	MOV	DH,SProwsToFill		;DH = #	of rows	to fill
SDfillNoWait:
	MOV	CL,DL			;CX = #	of char/attr pairs to move
	REP	STOSW			;Fill this row
	SUB	DI,BX			;Point DI to previous row
	DEC	DH			;Decrement # of	rows to	fill
	JNZ	SDfillNoWait		;Repeat	if not zero

SDExit:
	MOV	SP,BP			;Restore SP
	POP	BP			;Restore BP
	POP	DS			;Restore DS
	RET				;Return

ScrollDownPrim	ENDP

;******************************************************	CrtNop

;Do-nothing file handling procedure
;On entry, a pointer to	a FileRec is on	the stack

CrtNop	PROC FAR

	SetZero	AX			;Signal	AOK
	RET	4

CrtNop	ENDP

;******************************************************	OutputChar

;On entry, AL has character to write

OutputChar	PROC NEAR

	PUSH	DI			;Save DI
	MOV	CL,AL			;CL has	character
	MOV	CH,TextAttr		;CH has	TextAttr
	CMP	DirectVideo,True	;Write directly	to video memory?
	JNE	BiosOutput		;If not, use BIOS
	CMP	InTextMode,True		;In text mode?
	JNE	BiosOutput		;If not, use BIOS

	CMP	MultitaskingOn,True	;Multitasking?
	JNE	OCgetCursor		;if not, don't need BIOS call

	;get cursor position with BIOS call
	PUSH	CX			;Save CX
	MOV	BH,CurrentPage		;BH = current display page
	VideoCall	3		;Read cursor service
	POP	CX			;Restore CX
	JMP	SHORT OCcalcAddress	;Ready to calculate address

	;get cursor position from BIOS data area into DX
OCgetCursor:
	MOV	BX,0040h		;BX = $40
	MOV	ES,BX			;ES = $40
	MOV	BL,CurrentPage		;BL = CurrentPage, BH =	0
	SHL	BX,1			;Multiply by 2
	MOV	DX,ES:50h[BX]		;DX has	cursor position	for CurrentPage

	;calculate address in video memory
OCcalcAddress:
	MOV	AL,DH			;AL = row
	SetZero	DH			;DH = 0	(DX = column)
	MUL	ScreenWidth		;AL = (row * width)
	MOV	DI,AX			;DI = (row * width)
	ADD	DI,DX			;DI = (row * width) + column
	SHL	DI,1			;Account for attribute bytes
	MOV	ES,VideoSegment		;ES has	segment	of video memory

	;determine which method	of output to use
	CMP	CheckSnow,False		;Snow checking off?
	JE	OutputStore		;If so,	go

	;Use the following only	for color cards	when retrace checking is on
	MOV	DX,03DAh		;Point DX to CGA status	port
	WaitForRetrace			;Wait for an opportunity to write

OutputStore:
	WordToCGA	CX		;Write the word
	JMP	SHORT OutputExit	;Return

	;Write with a BIOS call

BiosOutput:
	MOV	AL,CL			;AL = character
	MOV	BL,CH			;BL = Video attribute
	MOV	BH,CurrentPage		;BH = Current video page
	MOV	CX,1			;CX = number of	chars to repeat
	VideoCall	9		;Write character and attribute service

OutputExit:
	POP	DI			;Restore DI
	RET

OutputChar	ENDP

;******************************************************	DoLineFeed

DoLineFeed	PROC NEAR

	INC	DH			;Next row
	CMP	DH,WindMax.YHigh	;Still inside window?
	JBE	DLFexit			;If so,	go
	DEC	DH			;Back up to current row
	PUSH	BX			;save registers
	PUSH	CX
	PUSH	DI
	PUSH	SI
	PUSH	ES
	PUSH	DX
	MOV	BH,TextAttr		;Filler	character
	MOV	CX,WindMin		;Top row and left column
	MOV	DX,WindMax		;Bottom	row and	right column
	MOV	AL,1			;AL = 1	(lines to scroll)
	CALL	ScrollUpPrim		;Call scroll window up primitive
	POP	DX
	POP	ES			;Restore registers
	POP	SI
	POP	DI
	POP	CX
	POP	BX
DLFexit:
	RET

DoLineFeed	ENDP

;******************************************************	WriteChar

;Write one character
;On entry, AL has character to write

WriteChar	PROC NEAR

	;Save registers

	PUSH	DI
	PUSH	SI
	PUSH	BX
	PUSH	CX
	PUSH	DX
	PUSH	ES

	;Get current position of cursor

	PUSH	AX
	CALL	GetCursorPrim
	POP	AX

	;Check for special characters

	CMP	AL,CR			;Carriage return?
	JE	CarriageReturn
	CMP	AL,LineFeed		;Line feed?
	JE	WCLineFeed
	CMP	AL,BkSp			;Backspace?
	JE	BackSpace
	CMP	AL,Bell			;Bell (^G)?
	JE	RingBell

	;Normal	character

	PUSH	DX			;Save DX
	CALL	OutputChar		;Write the character
	POP	DX			;Restore DX

	;Now move the cursor

	INC	DL			;Cursor	to next	column
	CMP	DL,WindMax.XHigh	;Still inside window?
	JBE	WCSetCursor		;If so,	move cursor and	exit
	MOV	DL,WindMin.XLow		;Else, do a 'carriage return'...
	JMP	SHORT WCLineFeed	;... and 'line feed'

RingBell:
	VideoCall	0Eh		;Write character as TTY
	JMP	SHORT WCExit		;Done

BackSpace:
	CMP	DL,WindMin.XLow		;Cursor	at start of this column?
	JE	WCExit			;If so,	do nothing
	DEC	DL			;Else, previous	column
	JMP	SHORT WCSetCursor	;Move cursor and exit

CarriageReturn:
	MOV	DL,WindMin.XLow		;First column in window
	JMP	SHORT WCSetCursor	;Move cursor and exit

WCLineFeed:
	CALL	DoLineFeed		;generate a line feed

WCSetCursor:
	CALL	SetCursorPrim
WCExit:
	POP	ES			;Restore registers
	POP	DX
	POP	CX
	POP	BX
	POP	SI
	POP	DI
	RET

WriteChar	ENDP

;******************************************************	FlushString

FlushString	PROC NEAR

	PUSH	CX			;save CX
	MOV	CX,DI			;get number of characters to write
	SUB	CX,SI
	JCXZ	FSexit			;done if CX = 0

	PUSH	DX			;save registers	used
	PUSH	DI
	PUSH	DS
	PUSH	ES

	;calculate address in video memory

	MOV	AL,BH			;AL = row
	SetZero	BH			;BH = 0	(BX = column)
	MUL	ScreenWidth		;AL = (row * width)
	MOV	DI,AX			;DI = (row * width)
	ADD	DI,BX			;DI = (row * width) + column
	SHL	DI,1			;Account for attribute bytes

	MOV	AH,TextAttr		;AH has	attribute
	MOV	AL,CheckSnow		;AL = CheckSnow

	MOV	BX,VideoSegment		;BX = VideoSegment
	PUSH	ES			;DS:SI points to string
	POP	DS
	MOV	ES,BX			;ES has	segment	of video memory

	;determine which method	of output to use
	CLD				;go forward
	OR	AL,AL			;Snow checking off?
	JZ	FSfast			;If so,	go fast
	MOV	DX,03DAh		;Point DX to CGA status	port

	;Use the following only	for color cards	when retrace checking is on
FSgetNext:
	LODSB				;Load next character into AL
	MOV	BX,AX			;Store video word in BX
	WaitForRetrace			;Wait for an opportunity to write
	WordToCGA	BX		;Move the word
	LOOP	FSgetNext		;Get next character
	JMP	SHORT FSdone		;Done

FSfast:
	LODSB				;load next character
	STOSW				;write it
	LOOP	FSfast			;get next character

FSdone:
	POP	ES			;restore registers
	POP	DS
	POP	DI
	POP	DX
FSexit:
	POP	CX
	RET

FlushString	ENDP

;******************************************************	WriteString

;on entry: ES:DI points	to start of string, CX has its length

WriteString	PROC NEAR

	PUSH	ES
	PUSH	DI
	PUSH	CX
	CALL	GetCursorPrim		;Get cursor coordinates	into DX
	POP	CX
	POP	DI
	POP	ES
	MOV	BX,DX			;save them in BX
	MOV	SI,DI			;ES:SI points to start of string

WSnext:
	MOV	AL,ES:[DI]		;next character	into AL
	CMP	AL,CR			;check for control characters
	JA	WSplain
	JE	WScr
	CMP	AL,LineFeed
	JE	WSlinefeed
	CMP	AL,BkSp
	JE	WSbksp
	CMP	AL,Bell
	JE	WSbell

WSplain:
	INC	DI			;point to next character
	INC	DL			;next column
	CMP	DL,WindMax.XHigh	;past end of window?
	JBE	WSskip			;if not, skip to next character

	CALL	FlushString		;else, flush string
	CALL	DoLineFeed		;do a line feed
	MOV	DL,WindMin.XLow		;DL has	first column
	JMP	SHORT WSnoAdvance

WSbell:
	CALL	FlushString		;flush the string
	Write	Bell			;sound the bell
	JMP	SHORT WSadvance

WSbksp:
	CALL	FlushString		;flush the string
	CMP	DL,WindMin.XLow		;already at first column?
	JE	WSadvance		;if so,	don't back up
	DEC	DL			;else, previous	column
	JMP	SHORT WSadvance

WSlinefeed:
	CALL	FlushString		;flush the string
	CALL	DoLineFeed		;do a line feed
	JMP	SHORT WSadvance

WScr:
	CALL	FlushString		;flush the string
	MOV	DL,WindMin.XLow		;DL has	first column

WSadvance:
	INC	DI			;point to next character

WSnoAdvance:
	MOV	SI,DI			;SI = DI
	MOV	BX,DX			;save cursor position

WSskip:
	LOOP	WSnext			;do it again
	CALL	FlushString		;flush the string
	CALL	SetCursorPrim		;position the cursor
	RET

WriteString	ENDP

;******************************************************	WriteCrt

;On entry, a pointer to	an FIB is on the stack

WriteCrt	PROC FAR

	StackFrame
	GetPtr	TextFile		;ES:DI points to file
	MOV	CX,TFPtr.FBufPos	;CX = number of	chars to write
	JCXZ	WCrtExit		;Exit if string	empty
	SUB	TFPtr.FBufPos,CX	;Back up pointer to start of string
	GetPtr	TFPtr.FBufPtr		;Point ES:DI to	the start of the buffer
	MOV	TextChar,' '		;Force TextChar	to ' '

	CMP	DirectVideo,True	;Write directly	to screen?
	JNE	WriteNext		;nope, write character at a time
	CMP	InTextMode,True
	JNE	WriteNext

	CALL	WriteString		;else, write string at a time
	JMP	SHORT WCrtExit		;we're done

WriteNext:
	Write	TFPtr			;Write the next	character
	INC	DI			;point to next char
	LOOP	WriteNext		;Next character

WCrtExit:
	CALL	BreakCheck		;Check for ^Break
	SetZero	AX			;Signal	AOK
	RET	4

WriteCrt	ENDP

;******************************************************	ReadCrt

;On entry, a pointer to	an FIB is on the stack

;constants
CrLf		=	0A0Dh

COMMENT	|
  Register usage:
    BX always points to	the next available slot	in the text buffer
    CX contains	a loop count (0	or 1). When CX = 0, LOOP instructions
      set CX to	FFFFh, so the loop gets	executed until some other
      condition	is met.
|

ReadCrt	PROC FAR

	StackFrameBP
	GetPtr	TextFileBP		;ES:DI points to FileRec
	MOV	SI,TFPtr.FBufPos	;SI = BufPos
	MOV	CX,TFPtr.FBufSize	;CX = BufSize
	DEC	CX
	DEC	CX			;CX = 126
	PUSH	CX			;Save this for later
	SetZero	BX			;Start at beginning of the buffer
	GetPtr	TFPtr.FBufPtr		;ES:DI points to BufPtr^
	MOV	AX,BufLen		;AX = BufLen
	OR	AX,AX			;AX = 0?
	JZ	ResetBufLen		;If so,	set it to the default

	CMP	AX,CX			;BufLen	> BufSize?
	JNA	ReadNext		;If not, proceed

ResetBufLen:
	MOV	Buflen,CX		;BufLen	= 126

ReadNext:
	MOV	NextChar,Null		;Filter	keys with extended scan	codes
	CALL	ReadKey			;Get next character into AL
	MOV	CX,1			;Loop counter =	1
	CMP	AL,BkSp			;Backspace?
	JE	ReadBkSp
	CMP	AL,CtrlS		;^S? (=	Backspace)
	JE	ReadBkSp
	CMP	AL,CtrlD		;^D? (Restore one character)
	JE	ReadRestore
	DEC	CX			;BkSp loop counter = 0
	CMP	AL,Escape		;Escape? (Delete line)
	JE	ReadBkSp
	CMP	AL,CtrlBkSp		;^BkSp?	(Delete	line)
	JE	ReadBkSp
	CMP	AL,CtrlA		;^A? (Delete line)
	JE	ReadBkSp
	CMP	AL,CtrlF		;^F? (Restore whole line)
	JE	ReadRestore
	CMP	AL,CtrlZ		;^Z? (close Input?)
	JE	ReadEofCheck		;If so,	see if we should close shop
	CMP	AL,Return		;Enter key?
	JE	ReadCrlf		;If so,	write a	CrLf and stop
	CMP	AL,' '			;Regular ASCII character?
	JB	ReadNext		;If not, continue

	;Process the character
	CMP	BX,BufLen		;Line full?
	JE	ReadNext		;If so,	don't add to it
	MOV	TFPtr[BX],AL		;Add character to end of buffer
	CALL	WriteChar		;Write the character
	INC	BX			;Increment the buffer pointer
	CMP	BX,SI			;BX > BufPos?
	JNA	ReadNext		;If not, continue
	MOV	SI,BX			;Else set BufPos to BX
	JMP	SHORT ReadNext

ReadExit:
	GetPtr	TextFileBP		;ES:DI points to FileRec
	POP	CX			;Get back default Buflen setting
	MOV	BufLen,CX		;Reset BufLen to default
	SetZero	AX			;No I/O	error
	MOV	TFPtr.FBufPos,AX	;Reset BufPos to 0 for next read
	MOV	TFPtr.FBufEnd,BX	;FBufEnd has length of the string
	ExitCode	4

ReadRestore:
	CMP	BX,SI			;Does BX point to end of buffer?
	JE	ReadNext		;If so,	do nothing
	MOV	AL,TFPtr[BX]		;Next character	into AL
	CMP	AL,' '			;Is it a control character?
	JB	ReadNext		;If so,	do nothing
	CALL	WriteChar		;Write it
	INC	BX			;Point to next character in buffer
	LOOP	ReadRestore		;Repeat	if CX was 0 on entry
ReadJump:
	JMP	SHORT ReadNext

ReadEofCheck:
	CMP	CheckEof,False		;CheckEof false?
	JE	ReadNext		;If so,	continue
	MOV	TFPtr[BX],AL		;Add character to end of buffer
	INC	BX			;Increment BX
	JMP	SHORT ReadExit		;And exit

ReadBkSp:
	OR	BX,BX			;BX = 0?
	JZ	ReadJump		;If so,	don't back up
	Write	BkSp			;Move the cursor back
	Write	' '			;Overwrite the last character
	Write	BkSp			;Move the cursor back again
	DEC	BX			;Decrement buffer pointer
	LOOP	ReadBkSp		;Repeat	if CX was 0 on entry
	JMP	ReadNext		;Continue

ReadCrlf:
	WriteLn				;Write a CR/LF
	MOV	WP TFPtr[BX],CrLf	;Add the CrLf to the buffer
	INC	BX
	INC	BX
	JMP	SHORT ReadExit		;And exit

ReadCrt	ENDP

;******************************************************	OpenCrt

;Open input or output files
;Parameter is a	pointer	to an FIB

COMMENT	|
   Register usage:
   AX has offset of the	Read/Write procedure
   BX			Flush
   CX			Close
|

OpenCrt	PROC FAR

	StackFrame
	GetPtr	TextFile		;ES:DI points to FileRec

	;assume	we're opening for input
	MOV	CX,OFFSET CrtNop	;CX = Ofs(CrtNop)
	MOV	BX,CX			;BX = Ofs(CrtNop)
	MOV	AX,OFFSET ReadCrt	;AX = Ofs(ReadCrt)
	CMP	TFPtr.FMode,FMInput	;Opening for input?
	JE	OpenIt			;If so,	AX-BX-CX are correct

	;nope, opening for output
	MOV	TFPtr.FMode,FMOutput	;Open for output
	MOV	AX,OFFSET WriteCrt	;AX = Ofs(WriteCrt)
	MOV	BX,AX			;BX = Ofs(WriteCrt) (Flush with	write)

OpenIt:
	SetPtr	TFPtr.FInOutProc, CS, AX	;Set up	Read/Write routine
	SetPtr	TFPtr.FFlushProc, CS, BX	;Set up	Flush routine
	SetPtr	TFPtr.FCloseProc, CS, CX	;Do nothing with a Close()
	SetZero	AX			;Signal	success
	RET	4

OpenCrt	ENDP

;******************************************************	AssignCrt

;procedure AssignCrt(var F : Text);
;Parameter is a	pointer	to an FIB

AssignCrt	PROC FAR

	StackFrame
	GetPtr	TextFile		;ES:DI points to F
	MOV	BX, OFFSET OpenCrt	;BX has	offset of OpenCrt
	SetPtr	TFPtr.FOpenProc, CS, BX	;Open with OpenCrt
	MOV	TFPtr.FMode,FMClosed	;File is closed
	MOV	TFPtr.FName,0		;No filename
	LEA	AX,TFPtr.FBuffer	;Set up	file buffer
	MOV	TFPtr.FBufPtr.Segm,ES	 ;Initialize record
	MOV	TFPtr.FBufPtr.Ofst,AX
	MOV	AX,128			;AX = Default buffersize
	MOV	TFPtr.FBufSize,AX	;Store Buffer size
	DEC	AX
	DEC	AX			;AX = Buffer size - 2
	MOV	BufLen,AX		;BufLen	= Buffer size -	2
	RET	4

AssignCrt	ENDP

;******************************************************	ReInitCrt

;procedure ReInitCrt;

;Reinitialize CRT unit's internal variables. For TSR's or programs with
;DOS shells.

ReInitCrt	PROC FAR

	;save current video mode
	CALL	GetCrtModePrim		;Current video mode into AL

	;check for EGA/VGA/etc.
	CALL	AdapterCheck		;Check for EGA
	CALL	WhichHercCard		;Check for Herc	card type
	MOV	WhichHerc,AL		;save it in WhichHerc

	;set retrace checking variable
	MOV	CheckSnow,False		;Assume	false
	CMP	IsCompaq,True		;Is it a Compaq?
	JE	SetWindow		;If so,	won't need to wait
	CMP	CurrentDisplay,CGA	;CGA installed?
	JNE	SetWindow		;If not, won't need to wait
	INC	CheckSnow		;Else, may have	to wait	when writing
					; to the color card
SetWindow:
	CALL	FullWindow		;set window coordinates
	RET

ReInitCrt	ENDP

;******************************************************	InitCrt

;procedure InitCrt;
;Initialization	stuff

InitCrt	PROC NEAR

	;Run test to see if DirectVideo	will cause problems
	MOV	DirectVideo,False	;try it	with DirectVideo false
	CALL	GetCursorPrim		;Get current coordinates into DX
	PUSH	DX			;save result on	stack
	MOV	DirectVideo,True	;try it	with DirectVideo on
	CALL	GetCursorPrim		;Get current coordinates into DX
	POP	AX			;first result into AX
	CMP	AX,DX			;results the same?
	JE	DirectVideoOK		;if so,	DirectVideo=True presumably OK
	MOV	DirectVideo,False	;else, force it	to false

DirectVideoOK:
	SetZero	AL			;AL = 0
	MOV	NextChar,AL		;NextChar = 0
	MOV	CtrlBreakFlag,AL	;CtrlBreakFlag = False
	MOV	WP ScreenWidth,80	;ScreenWidth = 80
	MOV	WP VirtualWidth,80
	CALL	ReinitCrt		;Init code shared w/ ReInitCrt

	CALL	GetAttribute		;get current attribute at cursor

	;set up	our ExitProc routine
	MOV	AX, OFFSET CrtExitProc	;AX has	offset of CrtExitProc
	GetPtr	ExitProc		;ES:DI points to ExitProc^
	SetPtr	SaveExitProc, ES, DI	;Save current value of ExitProc
	SetPtr	ExitProc, CS, AX	;Replace with address of CrtExitProc

	;take over INT 1Bh
	MOV	AX,351Bh		;AH = Get interrupt vector service
					;AL = interrupt	vector
	INT	21h			;Call DOS
	SetPtr	Save1B,	ES, BX		;Save pointer returned in ES:BX
	MOV	DX,OFFSET Int1B		;DX = Ofs(Int1B)
	PUSH	DS			;Save DS
	MOV	AX,CS			;AX = CS
	MOV	DS,AX			;DS = CS
	MOV	AX,251Bh		;AH = Set interrupt vector service
					;AL = interrupt	vector
	INT	21h			;Call DOS
	POP	DS			;Restore DS

	;set up	delay count
	MOV	AX,40h			;AX = $40
	MOV	ES,AX			;ES = $40
	MOV	DI,6Ch			;ES:DI => low word of BIOS timer count
	MOV	AL,ES:[DI]		;AL has	first byte there
	STI
WaitForChange:
	CMP	AL,ES:[DI]		;wait for the byte to change
	JZ	WaitForChange

	MOV	AL,ES:[DI]		;see how many times DelayMS has	to
	MOV	CX,0FFFFh		;loop before the timer count changes
	CALL	DelayMS

	MOV	AX,55			;now calculate OneMS
	XCHG	CX,AX
	NOT	AX
	XOR	DX,DX
	DIV	CX
	MOV	OneMS,AX		;AX has	OneMS
	RET

InitCrt	ENDP

CODE	ENDS

	END

