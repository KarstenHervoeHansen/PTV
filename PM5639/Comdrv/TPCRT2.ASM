;******************************************************
;		    TPCRT2.ASM 5.08
;		Secondary level	routines
;	 Copyright (c) TurboPower Software 1987.
; Portions copyright (c) Sunny Hill Software 1985, 1986
;     and used under license to	TurboPower Software
;		 All rights reserved.
;******************************************************


	INCLUDE	TPCOMMON.ASM

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
	EXTRN	ScreenHeight : BYTE		;Current height	of display
	EXTRN	CurrentDisplay : BYTE		;Current display type
	EXTRN	EnhancedDisplay	: BYTE		;Type of enhanced display
						;installed, if any
	EXTRN	InTextMode : BYTE		;False if in graphics mode
	EXTRN	TextAttr : BYTE			;Current video attribute
	EXTRN	NormalAttr : BYTE;		;Attribute for NormVideo
	EXTRN	CheckSnow : BYTE		;If true, check	for retrace
	EXTRN	CtrlBreakFlag :	BYTE		;True if ^Break	pressed
	EXTRN	VideoSegment : WORD		;Segment of Video Memory
	EXTRN	VirtualSegment : WORD		;Segment of Video Memory--alt
	EXTRN	BufLen : WORD			;Max length of string for Read
	EXTRN	ExitProc : DWORD		;Turbo's ExitProc pointer
	EXTRN	SaveInt1B : DWORD		;Previous INT 1Bh handler
	EXTRN	DetectMultitasking : BYTE	;If True, automatically	checks
						;  for multitasker
	EXTRN	MultitaskingOn : BYTE		;True if running under
						;  multitasker
	EXTRN	IsCompaq : BYTE			;True if system	is a COMPAQ
	EXTRN	BiosScroll : BYTE		;If False, use special scroll
						;  routine
	EXTRN	NextChar : BYTE			;Used by KeyPressed/ReadKey
	EXTRN	OneMS :	WORD			;Loop count for	1 ms delay

DATA	ENDS

;******************************************************	Code

CODE	SEGMENT	BYTE PUBLIC

	ASSUME	CS:CODE, DS:DATA

	;standard CRT unit routines

	PUBLIC	TextColor, TextBackground, LowVideo, HighVideo,	NormVideo
	PUBLIC	Delay, KeyPressed, GotoXY, WhereX, WhereY, TextMode, Window
	PUBLIC	ClrScr

	;extensions

	PUBLIC	ReadKeyWord, GotoXYabs,	GetCrtMode
	PUBLIC	Font8x8Selected, SelectFont8x8

	;routines in TPCRT.ASM

	EXTRN	ReadCursorPrim : NEAR
	EXTRN	SetCursorPrim :	NEAR
	EXTRN	GetCursorPrim :	NEAR
	EXTRN	GetCrtModePrim : NEAR
	EXTRN	ScrollUpPrim : NEAR
	EXTRN	ScrollDownPrim : NEAR
	EXTRN	AdapterCheck : NEAR
	EXTRN	DelayMS	: NEAR
	EXTRN	GetCharAttr : NEAR
	EXTRN	SetWindowPrim :	NEAR
	EXTRN	FullWindow : NEAR
	EXTRN	GetAttribute : NEAR

	;routines in TPCRT.PAS

	EXTRN	CrtTest	: NEAR

;******************************************************	LowVideo

;procedure LowVideo;
;Select	low intensity

LowVideo	PROC FAR

	AND	TextAttr,01110111b	;Intensity bit off, blink bit off
	RET

LowVideo	ENDP

;******************************************************	HighVideo

;procedure HighVideo;
;Select	high intensity

HighVideo	PROC FAR

	OR	TextAttr,00001000b	;Intensity bit on
	RET

HighVideo	ENDP

;******************************************************	NormVideo

;procedure NormVideo;
;Select	video attribute	used at	start of program

NormVideo	PROC FAR

	MOV	AL,NormalAttr		;AL = Normal attribute
	MOV	TextAttr,AL		;Current attribute = Normal attribute
	RET

NormVideo	ENDP

;******************************************************	TextColor

;procedure TextColor(Color : Byte);
;Set foreground	color for screen writes

NewColor	EQU	BYTE PTR SS:[BX+4]

TextColor	PROC FAR

	StackFrame
	MOV	AL,NewColor		;New foreground	color into AL
	TEST	AL,11110000b		;See if	blink requested
	JZ	SkipBlink		;If not, don't set blink bit
	AND	AL,00001111b		;Only blink and	foreground bits
	OR	AL,Blink		;Set blink bit

SkipBlink:
	AND	TextAttr,01110000b	;Retain	current	background color
	OR	TextAttr,AL		;Merge in new foreground color
	CALL	CrtTest			;test for presence of CRT unit
	RET	2

TextColor	ENDP

;******************************************************	TextBackground

;procedure TextBackground(Color	: Byte);
;Set background	color for screen writes

TextBackground	PROC FAR

	StackFrame
	MOV	AL,NewColor		;New foreground	color into AL
	AND	AL,00000111b		;Only bottom three bits	are significant
	MOV	CL,4			;Move bottom 4 bits into top 4 bits
	SHL	AL,CL
	AND	TextAttr,10001111b	;Retain	current	blink/foreground
	OR	TextAttr,AL		;Merge in new background color
	CALL	CrtTest			;test for presence of CRT unit
	RET	2

TextBackground	ENDP

;******************************************************	KeyPressed

;function KeyPressed : Boolean;
;Return	true if	a key has been pressed

KeyPressed	PROC FAR

	CMP	NextChar,0		;See if	a character is waiting
	JA	KeyFound		;If so,	we're done
	KbdCall	1			;Character ready service
	MOV	AL,False		;Assume	false
	JZ	KeyExit			;No keypress if	zero flag is set

KeyFound:
	MOV	AL,True			;AL = True

KeyExit:
	RET

KeyPressed	ENDP

;******************************************************	ReadKeyWord

;function ReadKeyWord :	Word;
;Waits for keypress, then returns scan and character codes together in AX

ReadKeyWord	PROC FAR

ReadKeyWordStart:
	INT	28h			;Help keep popups from locking out
					;other TSR's
	KbdCall	1			;Key pressed function
	JZ	ReadKeyWordStart	;If no keypress, loop

	KbdCall	0			;Read next character service
	RET

ReadKeyWord	ENDP

;******************************************************	GotoXY

;procedure GotoXY(X, Y : Byte);
;Move cursor to	column X, row Y, relative to Window

XGoto	EQU	BYTE PTR SS:[BX+6]
YGoto	EQU	BYTE PTR SS:[BX+4]

GotoXY	PROC FAR

	StackFrame
	MOV	DH,YGoto		;DH = new row
	DEC	DH			;Now in	0..24 format
	ADD	DH,WindMin.YLow		;Account for window
	JB	ExitXY			;Exit if number	is negative
	CMP	DH,WindMax.YHigh	;out of	window?
	JA	ExitXY			;If so,	exit
	MOV	DL,XGoto		;DL = new column
	DEC	DL			;Now in	0..79 format
	ADD	DL,WindMin.XLow		;Account for window
	JB	ExitXY			;Exit if number	is negative
	CMP	DL,WindMax.XHigh	;out of	window?
	JA	ExitXY			;If so,	exit
	CALL	SetCursorPrim		;Primitive routine to move the cursor
ExitXY:
	CALL	CrtTest			;test for presence of CRT unit
	RET	4

GotoXY	ENDP

;******************************************************	GotoXYAbs

;procedure GotoXYAbs(X,	Y : Byte);
;Move cursor to	column X, row Y. No error checking done.

GotoXYAbs	PROC FAR

	StackFrame
	MOV	DH,YGoto		;DH = new row
	DEC	DH			;Now in	0..24 format
	MOV	DL,XGoto		;DL = new column
	DEC	DL			;Now in	0..79 format
	CALL	SetCursorPrim		;Primitive routine to move the cursor
	RET	4

GotoXYAbs	ENDP

;******************************************************	WhereX

;function WhereX : Byte;
;Return	column coordinate of cursor, relative to Window

WhereX	PROC FAR

	CALL	GetCursorPrim		;Get current column into DL
	SUB	DL,WindMin.XLow		;Subtract XLow
	INC	DL			;Adjust	for 1..80 format
	SetZero	AH			;Clear AH
	MOV	AL,DL			;Result	into AL
	RET

WhereX	ENDP

;******************************************************	WhereY

;function WhereY : Byte;
;Return	row coordinate of cursor, relative to Window

WhereY	PROC FAR

	CALL	GetCursorPrim		;Get current row into DH
	SUB	DH,WindMin.YLow		;Subtract YLow
	INC	DH			;Adjust	for 1..25 format
	SetZero	AH			;Clear AH
	MOV	AL,DH			;Result	into AL
	RET

WhereY	ENDP

;******************************************************	Font8x8Selected

;function Font8x8Selected : Boolean;
;Return	True if	EGA or VGA is active and in 8x8	font (43-line /	50-line	mode)

Font8x8Selected	PROC FAR

	CALL	GetCrtModePrim		;Get current video mode
	CALL	AdapterCheck		;Get adapter type
	SetZero	AX			;Assume	false
	TEST	LastMode,Font8x8	;8x8 font bit set?
	JZ	FSexit			;if not, exit
	INC	AX			;AX = 1
FSExit:	RET

Font8x8Selected	ENDP

;******************************************************	SelectFont8x8

;procedure SelectFont8x8(On : Boolean);
;Toggle	8x8 font on or off. Does not reset Window() or clear the screen!

FontOn		EQU	BYTE PTR SS:[BX+4]
EmulationFlag	EQU	BYTE PTR ES:[DI]

SelectFont8x8	PROC FAR

	CALL	AdapterCheck		;Check display type
	StackFrame			;Set up	stack frame
	MOV	CH,FontOn		;CH = FontOn
	MOV	CL,CurrentDisplay	;CL = CurrentDisplay
	CMP	CL,EGA			;EGA or	VGA?
	JB	SFDone			;Done if not
	MOV	AX,1112h		;AX = $1112 (turns it on for EGA or VGA)
	CMP	CH,True			;Turn it on?
	JE	SFGo			;if so,	go
	DEC	AX			;AX = $1111 (turns it off on EGA)
	CMP	CL,EGA			;is it an EGA?
	JE	SFGo			;if so,	go
	MOV	AX,1114h		;else, AX = $1114 (turns it off	on VGA)
SFGo:
	SetZero	BL			;BL = 0
	VideoPrim			;Call BIOS

	;turn cursor emulation on or off
	MOV	AX,40h			;AX = $40
	MOV	ES,AX			;ES = $40
	MOV	DI,87h			;ES:DI => Emulation flag
	SHR	CH,1			;Turning Font on?
	JC	EmulationOff		;If so,	turn emulation off
	AND	EmulationFlag,0FEh	;turns emulation on
	MOV	CX,0607h		;cursor	scan lines
	JMP	SHORT SFSetCursor	;reset the cursor

EmulationOff:
	OR	EmulationFlag,1		;turns emulation off
	MOV	CX,0507h		;cursor	scan lines

SFSetCursor:
	CMP	CurrentMode,7		;In mono mode?
	JE	SFCheck			;Exit if so, else...
	VideoCall	1		;Set cursor scan lines

SFCheck:
	CALL	AdapterCheck		;Call primitive	routine	to reset vars
	MOV	AH,12h			;select	alternate print	screen routine
	MOV	BL,20h
	VideoPrim
SFDone:	RET	2

SelectFont8x8	ENDP

;******************************************************	GetCrtMode

;procedure GetCrtMode :	Byte;
;Return	the current video mode in AX

GetCrtMode	PROC FAR

	CALL	GetCrtModePrim		;Call primitive	routine
	SetZero	AH			;AH = 0, result	in AL
	RET

GetCrtMode	ENDP

;******************************************************	TextMode

;procedure TextMode(Mode : Word);
;Set the current video mode

TModeW	EQU	WORD PTR SS:[BX+4]

TextMode	PROC FAR

	StackFrame
	MOV	AX,40h			;AX = $40
	MOV	ES,AX			;ES = $40
	AND	BYTE PTR ES:[87h],0FEh	;turn cursor emulation on
	MOV	AX,TModeW		;AX = Mode
	PUSH	AX			;Save mode
	VideoCall	0		;Set video mode	service
	POP	AX			;Reload	AH
	TEST	AX,Font8x8		;Is Font8x8 requested?
	JZ	TMnoFont		;No font change	if it isn't
	MOV	AL,True			;AL = True
	PUSH	AX			;Pass True as the parameter
	CALL	SelectFont8x8		;Call the font change routine
TMnoFont:
	CALL	GetCrtModePrim		;Make sure variables are up-to-date
	CALL	AdapterCheck		;In case we switched in/out of 8x8 mode
	CALL	FullWindow		;set window coordinates
	CMP	InTextMode,False	;are we	still in text mode?
	JNE	TMexit			;if not, we're finished
	CALL	GetAttribute		;re-initialize attribute variables
TMexit:
	RET	2

TextMode	ENDP

;******************************************************	Window

;procedure Window(XLow,	YLow, XHigh, YHigh : Byte);
;Set current window coordinates

WXLow	EQU	BYTE PTR SS:[BX+10]
WYLow	EQU	BYTE PTR SS:[BX+8]
WXHigh	EQU	BYTE PTR SS:[BX+6]
WYHigh	EQU	BYTE PTR SS:[BX+4]

Window	PROC FAR

	CALL	CrtTest			;test for presence of CRT unit
	StackFrame
	MOV	DL,WXLow		;DL = XLow
	MOV	DH,WXHigh		;DH = XHigh
	MOV	CL,WYLow		;CL = YLow
	MOV	CH,WYHigh		;CH = YHigh
	DEC	DL			;Convert all to	BIOS format
	DEC	DH
	DEC	CL
	DEC	CH
	CMP	CL,CH			;YLow >	YHigh?
	JA	WinExit			;If so,	exit
	CMP	DL,DH			;XLow >	XHigh?
	JA	WinExit			;If so,	exit
	CMP	DH,BYTE	PTR ScreenWidth	;XHigh >= screen width?
	JAE	WinExit			;If so,	exit
	CMP	CH,ScreenHeight		;YHigh >= screen height?
	JAE	WinExit			;If so,	exit
	CALL	SetWindowPrim		;Call primitive	routine
	MOV	DX,WindMin		;Load XLow and YLow into DX
	CALL	SetCursorPrim		;Move cursor into window
WinExit:
	RET	8

Window	ENDP

;******************************************************	ClrScr

;procedure ClrScr;
;Clear the current window

ClrScr	PROC FAR

	MOV	BH,TextAttr		;BH = current attribute
	MOV	CX,WindMin		;CX has	XLow,YLow
	MOV	DX,WindMax		;DX has	XHigh,YHigh

	SetZero	AL			;AL = 0
	CALL	ScrollUpPrim		;Call scroll window up primitive
	MOV	DX,WindMin		;DX has	coordinates of top left	corner
	CALL	SetCursorPrim		;Call routine to set cursor
	CALL	CrtTest			;test for presence of CRT unit
	RET

ClrScr	ENDP

;******************************************************	Delay

;procedure Delay(MS: Word);
;Delay for MS milliseconds

MSecs	EQU	WORD PTR SS:[BX+4]

Delay	PROC FAR

	StackFrame
	MOV	DX,MSecs		;DX = MS
	OR	DX,DX			;Do nothing if MS = 0
	JZ	DelayExit
	SetZero	DI			;ES:DI points to dummy address
	MOV	ES,DI			; which	won't change
	MOV	AL,ES:[DI]		;AL has	the value there

DelayLoop:
	MOV	CX,OneMS		;loop count into CX
	CALL	DelayMS			;delay for one MS
	DEC	DX			;decrement counter
	JNZ	DelayLoop		;repeat	if not 0

DelayExit:
	RET	2

Delay	ENDP


CODE	ENDS

	END
