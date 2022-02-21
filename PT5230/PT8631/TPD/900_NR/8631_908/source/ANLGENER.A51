$NOMOD51      ;Disable the predefined 8051 register set

$INCLUDE( REG320.INC)   ;Include the predefined 80C320 register set
$INCLUDE( MACROS.INC)   ;Include user defined macros

;/************************************************************************
;* Copyright ProTeleVision Technologies A/S, BRONDBY 1998                *
;* Project: PT8631 Analog Test Pattern generator                         *
;* Module:  ANLGENER.A51                                                 *
;* Author:  Kim Engedahl, DEV                                            *
;* Org.  :  980306                                                       *
;* Rev.  :  000705, KEn, DEV                                             *
;* Status:  Version 1.1 (the DALLAS 80C320 running at 19.6608MHz)        *
;*                                                                       *
;* Changes:                                                              *
;* 13.01.2005: Window10, Window15 and Window20 patterns now cover whole  *
;              screen.  Only this file is affected (JK)                  *
;* 000705: ALL protection has been removed                               *
;* 990421: Only HW type & HW version tested for protection               *
;* 990420: Added Window 10% & 15kHz Bl/Wh in PAL/NTSC                    *
;* 990420: Added Philips 4:3 & 16:9 in NTSC                              *
;* 980223: Released in SW ver. 2.2                                       *
;* 981002: Released in SW ver. 2.0                                       *
;* 980916: Added 16:9 CrossHatch in PAL/NTSC and WhiteCircle 4:3/16:9    *
;*         in PAL.                                                       *
;* 980903: Added 16:9 pattern in PAL                                     *
;* 980603: Released in SW ver. 1.0                                       *
;************************************************************************/

NAME    ANLGENER

EXTRN CODE( RS232Intr, RS232Init, CharHandler, TestInputBuffer)
EXTRN CODE( Reset_DAC, Read_EEPROM)

EXTRN CODE( G1_F1L22, G2_F1L22, G3_F1L22, G4_F1L22, G5_F1L22, G6_F1L22)
EXTRN CODE( G7_F1L22, G8_F1L22, G9_F1L22, G10_F1L22, G11_F1L22, G12_F1L22)
EXTRN CODE( G13_F1L22, G14_F1L22, G15_F1L22)

EXTRN CODE( M1_F1L20, M2_F1L20, M3_F1L20, M4_F1L20, M5_F1L20, M6_F1L20)
EXTRN CODE( M7_F1L20, M8_F1L20, M9_F1L20)

EXTRN CODE( CB75GreyPattG, EBUCBRedPattG, Win10PattG, Win15PattG, Win20PattG)
EXTRN CODE( Win100PattG, CrossPattG, PLUGEPattG, SafePattG, FSWaveG, VMT01G)
EXTRN CODE( CrossPatt16x9G)

EXTRN CODE( CBSMPTEPattM, Win10PattM, Win15PattM, Win20PattM, Win100PattM)
EXTRN CODE( CrossPattM, PLUGEPattM, SafePattM, FSWaveM, CrossPatt16x9M)

EXTRN CODE( GOffsetReset, GeneratorType, ClearOSDLine)


PUBLIC  StatusPort
PUBLIC  GenericMSBPort, LineAdrPort, FinePortH, FinePortL
PUBLIC  ScHPhasePort, SCFreqLSBPort, SCFreqMSBPort
PUBLIC  TextPort

PUBLIC  NewPattern, PatternNo, PatternAttr, PatternPtr
PUBLIC  Complex, ComplexPtr, ComplexCnt

PUBLIC  GenlockOffset, UserOffset
PUBLIC  OffsetCal, LineOffset, CoarseOffset, FineOffset

PUBLIC  UserScHOffset, ScHOffset, ScHPhaseCal
PUBLIC  DACLevel, DACOffset

PUBLIC  Status
PUBLIC  Status_SYSSEL, Status_TXT_1, Status_TXT_0

PUBLIC  TmpBit, CoarseGTMax
PUBLIC  PALSystem, NTSCSystem

PUBLIC  OSDTextLN1, OSDTextLN2, OSDTextLN3
PUBLIC  TimeText, DateText

PUBLIC  XPosLine1, YPosLine1
PUBLIC  XPosLine2, YPosLine2
PUBLIC  XPosLine3, YPosLine3

PUBLIC  XPosTime
PUBLIC  XPosDate

PUBLIC  TextLN1Enable, Line1Enable, Line1On
PUBLIC  TextLN2Enable, Line2Enable, Line2On
PUBLIC  TextLN3Enable, Line3Enable, Line3On
PUBLIC  ClockEnable

PUBLIC  TextStyle, ClockStyle, LineStatus

PUBLIC Line1Cnt, Line2Cnt, Line3Cnt

PUBLIC  ErrorStatus, Buffer_OV, Illegal_Del, Illegal_Cmd, Illegal_Par
PUBLIC  IIC_CheckSum, IIC_NoAck

PUBLIC  TmpBuffer, TextTable

PUBLIC  HWRunning

;************************************************************************
  CSEG  AT 00H    ;00H is address for Reset vector
  LJMP  Begin

  CSEG  AT 03H    ;03H is address for External Interrupt 0,
  LJMP  RelLineCntIntr

  CSEG  AT 0BH    ;0BH is address for Timer 0 Overflow
  RETI

  CSEG  AT 13H    ;13H is address for External Interrupt 1,
  RETI

  CSEG  AT 1BH    ;1BH is address for Timer 1 Overflow
  RET

  CSEG  AT 23H    ;23H is address for Serial interrupt
  LJMP  RS232Intr

;************************************************************************
?STACK  SEGMENT IDATA   ; ?STACK goes into IDATA RAM.
  RSEG  ?STACK    ; switch to ?STACK segment.
  DS  20    ; reserve your stack space

;************************************************************************
;Bank 0 is the generel bank, NOT TO BE USED DIRECTLY
;Bank 1 is reserved for field-/line-/counter-interrupt
;Bank 2 is reserved for RS232-interrupt
;Bank 3 is used for general purpose RAM

Global_BITDATA  SEGMENT DATA BITADDRESSABLE
    RSEG  Global_BITDATA

Status:   DS  1   ;Status port and it's bit definition
Status_S0 BIT Status.0  ;S1S0: line selector for field blanking
Status_S1 BIT Status.1  ; etc.
Status_SYSSEL BIT Status.2  ;0: PAL-G, 1: NTSC
Status_TXT_0  BIT Status.3  ;1: In lines with text on
Status_TXT_1  BIT Status.4  ;
;Not used BIT Status.5  ;
;No Name  BIT Status.6  ;Set to 1 in ( line 1, field1)
Status_Field1 BIT Status.7  ;Low in field 1, 1 in field 2

ErrorStatus:  DS  1   ;
Buffer_OV BIT ErrorStatus.0 ;Input buffer overrun
Illegal_Del BIT ErrorStatus.1 ;Illegal delimiter
Illegal_Cmd BIT ErrorStatus.2 ;Illegal command
Illegal_Par BIT ErrorStatus.3 ;Illegal parameter
IIC_NoAck BIT ErrorStatus.4 ;No acknowledge from IIC Circuit
IIC_CheckSum  BIT ErrorStatus.5 ;Checksum error reading from EEPROM
;Not used BIT ErrorStatus.6
;Not Used BIT ErrorStatus.7

LineStatus: DS  1   ;
Line1On   BIT LineStatus.0  ;1: Active lines for text insertion
Line2On   BIT LineStatus.1  ;1: Active lines for text insertion
Line3On   BIT LineStatus.2  ;1: Active lines for text insertion
Line1Enable BIT LineStatus.3  ;1: User text line 1 ON
Line2Enable BIT LineStatus.4  ;1: User text line 2 ON
Line3Enable BIT LineStatus.5  ;1: User text line 3 ON
TmpBit    BIT LineStatus.6  ;Temporary bit. Used in ANLCMD.A51
HWRunning BIT LineStatus.7

LineStatus1:  DS  1   ;
TextLN1Enable BIT LineStatus1.0 ;
TextLN2Enable BIT LineStatus1.1 ;
TextLN3Enable BIT LineStatus1.2 ;
FieldCnt1 BIT LineStatus1.3 ;
FieldCnt2 BIT LineStatus1.4 ;
FieldCnt3 BIT LineStatus1.5 ;
;Not used BIT LineStatus1.6 ;
ClockEnable BIT LineStatus1.7

;************************************************************************
Global_BIT  SEGMENT BIT
    RSEG  Global_BIT

PALSystem:  DBIT  1   ;0: PAL-G  1: PAL-G with ID
NTSCSystem: DBIT  1   ;0: NTSC  1: NTSC without piedestal
NewPattern: DBIT  1   ;1: New pattern received

UpdOffsetFlag:  DBIT  1   ;1: Delayed field interrupt has
          ; occurred -> update line offset

CoarseGTMax:  DBIT  1   ;1: CoarseOffset >= Max. value

;************************************************************************
Global_DATA SEGMENT DATA
    RSEG  Global_DATA

PatternNo:  DS  1   ;User selected pattern type.
PatternAttr:  DS  1   ;User selected pattern attribute.
PatternPtr: DS  2   ;Pointer to pattern table, (MACROS.INC).

TextStyle:  DS  1   ;Text style information
ClockStyle: DS  1   ;Clock style information

ComplexPtr: DS  2   ;Pointer to complex linenumber.
ComplexCnt: DS  1   ;Counter for displaing complex lines.

Line1Cnt: DS  2   ;Counter used for text line 1 insertion
Line2Cnt: DS  2   ;Counter used for text line 2 insertion
Line3Cnt: DS  2   ;Counter used for text line 3 insertion

NFrameCnt:  DS  1   ;

XPosLine1:  DS  1   ;Line 1 X-position.
YPosLine1:  DS  2   ;Line 1 Y-position. Actual start is +1.
XPosLine2:  DS  1   ;Line 2 X-position.
YPosLine2:  DS  2   ;Line 2 Y-position. Actual start is +1.
XPosLine3:  DS  1   ;Line 3 X-position.
YPosLine3:  DS  2   ;Line 3 Y-position. Actual start is +1.

XPosTime: DS  1   ;Start of time horizontal position
XPosDate: DS  1   ;Start of date horizontal position

OffsetCal:  DS  4   ;Calibration offset
ScHPhaseCal:  DS  2   ;ScH-Phase calibration

UserOffset: DS  4   ;User offset
GenlockOffset:  DS  4   ;Genlock offset
UserScHOffset:  DS  2   ;User ScH-Phase offset

DACLevel: DS  1   ;ONLY used during DAC-calibraion &
DACOffset:  DS  1   ; DAC-readout

LineOffset: DS  2   ;Calculated line offset
CoarseOffset: DS  2   ;Calculated coarse offset (n*74ns)
FineOffset: DS  2   ;Calculated fine offset, (n*0.145ns)
ScHOffset:  DS  2   ;Calculated ScHPhase offset

TmpBuffer:  DS  11    ;Temporary array

          ;Port references, etc.
StatusPort  EQU 08000H    ;

GenericMSBPort  EQU 09000H    ;ALWAYS write this port FIRST

LineAdrPort EQU 0C000H    ;Port to write new linenumber for the
          ; patterntype

CoarsePort  EQU 0A000H    ;Port to write coarse offset < 860
          ;A001 is also used

FinePortL EQU 0B008H    ;Port to write fine offset
FinePortH EQU 0B009H    ; do.

ScHPhasePort  EQU 0D006H    ;Port to write ScH-Phase

SCFreqLSBPort EQU 0D000H    ;Address of 16 LSB Subcarrier frequency
SCFreqMSBPort EQU 0D001H    ;Address of 16 MSB Subcarrier frequency

NCOFreqPort EQU 0B000H    ;Address of 13.5 MHz NCO (LSB of 32bit)

TextPort  EQU 0E000H    ;Address of textport  
          ;Status bits:         TXT_1 TXT_0
          ;Line 0: 0-07FH        0     0
          ;Line 1: 080H-0FFH     0     1
          ;Line 2: 100H-17FH     1     0
          ;Line 3: 180H-1FFH     1     1

          ;Variables placed in dual port RAM
OSDTextLN1  EQU TextPort+0200H  ;Text on line 1, 21 bytes
OSDTextLN2  EQU OSDTextLN1+21 ;Text on line 2, 21 bytes
OSDTextLN3  EQU OSDTextLN2+21 ;Text on line 3, 21 bytes

TimeText  EQU OSDTextLN3+21 ;8+1 bytes for time
DateText  EQU TimeText+9  ;8+1 bytes for date
          ;NOTE!! RxBuffer in RS232.A51 occupies
          ; TextPort+( 0300H->0380H)

Complex   EQU DateText+9  ;Reserve 24 bytes for complex pattern

          ;Definitions for register bank 1
absR0   EQU 008H    ;No. of lines with same S1S0
absR1   EQU 009H    ;S1S0 pattern

absR2   EQU 00AH    ;Temporary register

absR3   EQU 00BH    ;LineRoutinePtr
absR4   EQU 00CH    ; do. +1

absR5   EQU 00DH    ;Not Used / PatternPtr
absR6   EQU 00EH    ;LineType / PatternPtr+1

absR7   EQU 00FH    ;Reg. to push ACC

;************************************************************************
Global_TABLES SEGMENT CODE
    RSEG  Global_TABLES

;************************************************************************
;* Data arranged as:
;*       <lines>, xxxxxzyyb
;* where 
;*   <lines> = number of lines to next update
;*   xxxxx=linetype
;*       z=field, ( 0 in field1, 1 in field2)
;*       yy = S1S0
;************************************************************************
S1S0_M: 
  ;Frame 1
  DB    3,038H,   3,048H,   3,038H,  11,068H, 242,070H
  DB    1,0FAH,   2,03CH,   1,044H,   2,04CH,   1,05CH
  DB    2,03CH,   1,054H,  10,06CH,   1,075H, 242,0FCH
  ;Frame 2
  DB    3,038H,   3,048H,   3,038H,  11,068H, 242,070H
  DB    1,0FAH,   2,03CH,   1,044H,   2,04CH,   1,05CH
  DB    2,03CH,   1,054H,  10,06CH,   1,075H, 242,0FCH
  DB    0

S1S0_G:
  ;Frame 1
  DB    2,010H,   1,020H,   2,000H,   1,033H,   1,078H
  DB   15,030H,   1,0F9H, 255,0F8H,  31,0F8H,   1,0FBH
  DB    2,000H,   1,00CH,   2,014H,   2,004H,   1,01CH
  DB    1,034H,   16,034H, 255,0FCH,  31,0FCH,  1,0FFH
  DB    1,0FEH,   2,004H
  ;Frame 2
  DB    2,010H,   1,020H,   2,000H,   1,030H,  16,030H
  DB    1,0F9H, 255,0F8H,  31,0F8H,   1,0F8H,   2,000H
  DB    1,00CH,   2,014H,   2,004H,   1,01CH,   1,037H
  DB   16,034H, 255,0FCH,  31,0FCH,   1,0FCH,   1,0FEH
  DB    2,004H
  ;Frame 3
  DB    2,010H,   1,020H,   2,000H,   1,033H,  16,030H
  DB    1,0F9H, 255,0F8H,  31,0F8H,   1,0FBH,   2,000H
  DB    1,00CH,   2,014H,   2,004H,   1,01CH,   1,034H
  DB   16,034H, 255,0FCH,  31,0FCH,   1,0FFH,   1,0FEH
  DB    2,004H
  ;Frame 4
  DB    2,010H,   1,020H,   2,000H,   1,030H,  16,030H
  DB    1,0F9H, 255,0F8H,  31,0F8H,   1,0F8H,   2,000H
  DB    1,00CH,   2,014H,   2,004H,   1,01CH,   1,037H
  DB   16,034H, 255,0FCH,  31,0FCH,   1,0FCH,   1,0FEH
  DB    2,004H
  DB    0

          ; NOTE! # is used for clock colon
          ; NOTE! $ is used for date hyphen
TextTable: 
  ;   spc     !               #       $                 &       '
  DB  1, 94,  1, 93,  1, 0,   1, 237, 1, 238, 1, 0,   2, 65,  1, 158

  ; (       )       *       +       ,       -       .       /
  DB  1, 235, 1, 236, 1, 0,   2, 99,  1, 92,  2, 95,  1, 91,  2, 161

  ; 0       1       2       3       4       5       6       7
  DB  2, 69,  2, 71,  2, 73,  2, 75,  2, 77,  2, 79,  2, 81,  2, 83

  ; 8       9       :       ;       <       =       >       ?
  DB  2, 85,  2, 87,  2, 89,  1, 0,   1, 0,   2, 159, 1, 0,   1, 0

  ; A       B       C       D       E       F       G
  DB  1, 0,   2, 1,   2, 3,   2, 5,   2, 7,   2, 9,   2, 11,  2, 13

  ; H       I       J       K       L       M       N       O
  DB  2, 15,  1, 17,  2, 18,  2, 20,  2, 22,  3, 24,  2, 27,  2, 29

  ; P       Q       R       S       T       U       V       W
  DB  2, 31,  2, 33,  2, 35,  2, 37,  2, 39,  2, 41,  2, 43,  3, 45

  ; X       Y       Z                                       _
  DB  2, 48,  2, 50,  2, 52,  1, 0,   1, 0,   1, 0,   1, 0,   2, 97

  ; `       a       b       c       d       e       f       g  
  DB  1, 0,   2, 101, 2, 109, 2, 111, 2, 113, 2, 115, 1, 117, 2, 118

  ; h       i       j       k       l       m       n       o  
  DB  2, 105, 1, 120, 1, 121, 2, 122, 1, 124, 3, 125, 2, 128, 2, 107

  ; p       q       r       s       t       u       v       w  
  DB  2, 130, 2, 132, 2, 134, 2, 136, 2, 138, 2, 140, 2, 142, 3, 144

  ; x       y       z       {       |       }       ~          
  DB  2, 103, 2, 147, 2, 149, 1, 0,   1, 0,   1, 0,   1, 0,   1, 0

  ; €              ‚       ƒ       „       …       †       ‡
  DB  2,171,  2, 221, 2, 203, 2, 197, 2, 193, 2, 199, 2, 156, 2, 201

  ; ˆ       ‰       Š       ‹       Œ              Ž         
  DB  2, 205, 2, 207, 2, 209, 1, 228, 1, 227, 1, 0,   2, 61,  2, 59

  ;        ‘       ’       “       ”       •       –       —  
  DB  2, 173, 3, 151, 3,  54, 2, 217, 2, 213, 1, 0,   2, 231, 1, 0

  ; ˜       ™       š       ›       œ              ž       Ÿ  
  DB  1, 0,   2, 63,  2, 191, 2, 154, 1, 0,   2, 57,  1, 0,   1, 0

  ;         ¡       ¢       £       ¤       ¥       ¦       §  
  DB  2, 195, 1, 226, 2, 215, 2, 189, 2, 211, 2, 181, 1, 0,   1, 0

  ; ¨       ©       ª       «       ¬       ­       ®       ¯  
  DB  1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0

  ; °       ±       ²       ³       ´       µ       ¶       ·  
  DB  1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   2, 163, 2, 165, 2, 167

  ; ¸       ¹       º       »       ¼       ½       ¾       ¿  
  DB  1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0

  ; À       Á       Â       Ã       Ä       Å       Æ       Ç  
  DB  1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   2, 169

  ; È       É       Ê       Ë       Ì       Í       Î       Ï  
  DB  1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0

  ; Ð       Ñ       Ò       Ó       Ô       Õ       Ö       ×  
  DB  1, 0,   1, 0,   2, 175, 2, 177, 2, 179, 1, 0,   1, 223, 1, 224

  ; Ø       Ù       Ú       Û       Ü       Ý       o       ß  
  DB  1, 225, 1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0

  ; à       á       â       ã       ä       å       æ       ç  
  DB  2, 185, 2,  67, 2, 187, 1, 0,   2, 219, 2, 183, 1, 0,   1, 0

  ; è       é       ê       ë      
  DB  1, 0,   2, 233, 2, 229, 1, 0

;************************************************************************
;* Name:  -----
;* Function:  Main routine for the analog composite generator
;* Revision:  1.0
;* Date:  971208, KEn
;* Input: -
;* Output:  -
;* Destroys:  -
;* Calls  -
;* Time:  -
;************************************************************************
D1GenerMain SEGMENT CODE
    RSEG  D1GenerMain
    USING 0

Begin:
  MOV SP, #?STACK-1 

  MOV R1, #8
ZeroLoop:     ;Reset internal RAM, ie. clear 248 bytes
  MOV @R1, A    ; of internal RAM, ie. DO NOT CLEAR the first
  INC R1    ; 8 bytes.
  CJNE  R1, #0, ZeroLoop

        ;Initialise the timer, counters and the serial
        ; interface.
  MOV TMOD, #055H ;Timer 0: 16 bit counter, external trigger
        ;Timer 1: 16 bit counter, external trigger

  MOV T2CON, #034H  ;Timer 2: baud rate generator, 76.8 kBaud

  MOV RCAP2L, #0F8H ;Reload value 65528, ( 0FFF8H).
  MOV RCAP2H, #0FFH

  MOV SCON0, #0F0H  ;Serial control: Mode 3, using timer 2
        ; 9-bit receive/transmit

  MOV TCON, #055H ;Start Counter0, Counter 1 and set edge
        ; triggered interrupt for the external
        ; interrupt

  SETB  TR2   ;Start timer 2

        ;  TSTXTX
        ;  2 1100
  MOV IP, #00000001B  ;Field- & line-interrupt's highest priopity
        ;Field, V-pulse = X1
        ;Line, H-pulse = X0
        ;Delayed field = T1
        ;Line-counter = T0

  LCALL RS232Init   ;Initialise RS232 buffers etc.

  ;****************************************************************
  ;Initialize 13.5 MHz NCO to 040000000H. GenericMSBPort used to
  ;synchronize something in the NCO.
  ;NOTE! After this initialization, GenericMSBPort MUST ALWAYS
  ;contain 0, ZERO, when communicating with the NCO.
  ;****************************************************************
  MOV A, #01
  MOV DPTR, #GenericMSBPort
  MOVX  @DPTR, A

  CLR A
  MOV DPTR, #NCOFreqPort
  MOVX  @DPTR, A

  INC DPTR
  MOVX  @DPTR, A

  INC DPTR
  MOVX  @DPTR, A

  MOV A, #040H
  INC DPTR
  MOVX  @DPTR, A

  CLR A
  MOV DPTR, #GenericMSBPort
  MOVX  @DPTR, A

  ;****************************************************************
  ;Initialise subcarrier NCO to PAL-G
  ;****************************************************************
  MOV A, #015H
  MOV DPTR, #GenericMSBPort
  MOVX  @DPTR, A

  MOV A, #096H
  MOV DPTR, #SCFreqLSBPort
  MOVX  @DPTR, A

  MOV A, #054H
  MOV DPTR, #GenericMSBPort
  MOVX  @DPTR, A

  MOV A, #013H
  MOV DPTR, #SCFreqMSBPort
  MOVX  @DPTR, A

  ;****************************************************************
  ;Initialize GenericMSBPort
  ;****************************************************************
  CLR A
  MOV DPTR, #GenericMSBPort
  MOVX  @DPTR, A

  ;****************************************************************
  ;Initialize ScHPhase, (requires GenericMSBPort = 0)
  ;****************************************************************
  MOV DPTR, #ScHPhasePort
  MOVX  @DPTR, A

  ;****************************************************************
  ;Initialize coarseoffset, (requires GenericMSBPort = 0)
  ;****************************************************************
  MOV DPTR, #CoarsePort
  MOVX  @DPTR, A

  CLR CoarseGTMax
  ;****************************************************************
  ;Initialize fineoffset (+1 is LSB), (requires GenericMSBPort = 0)
  ;****************************************************************
  CLR A
  MOV DPTR, #FinePortL  ;NB! LSB MUST be loaded before MSB
  MOVX  @DPTR, A

  MOV DPTR, #FinePortH
  MOVX  @DPTR, A

  ;****************************************************************
  ;Initialize pattern line address, (requires GenericMSBPort = 0)
  ;****************************************************************
  MOV DPTR, #LineAdrPort
  MOVX  @DPTR, A

  ;****************************************************************
  ;Initialize text RAM, ALL 256 bytes
  ;****************************************************************
  SETB  TmpBit

  MOV DPTR, #StatusPort+1 ;Writing to StatusPort+1 enables reset-
  MOVX  @DPTR, A    ; ting the first 256 bytes in text RAM

  MOV DPTR, #TextPort
TextRAMReset:
  MOV R0, #00     ;Clear the 255 bytes
TextRAMLoop:
  MOVX  @DPTR, A
  INC DPTR
  DJNZ  R0, TextRAMLoop

  MOV DPTR, #StatusPort ;Writing to StatusPort enables reset-
  MOVX  @DPTR, A    ; ting the next 256 bytes in text RAM

  MOV DPTR, #TextPort+0100H

  JBC TmpBit, TextRAMReset

  ;****************************************************************
  ;Initialize DAC/EEPROM
  ;****************************************************************
  LCALL Reset_DAC   ;Dummy IIC initializer
  LCALL Reset_DAC

  MOV A, #GeneratorType ;Dummy Read
  MOV R2, #02     ;
  MOV R1, #TmpBuffer
  LCALL Read_EEPROM

  ;****************************************************************
  ;Initialize time-/date-/OSD-strings
  ;****************************************************************
  MOV TextStyle, #FreeTextStyle

  MOV R0, #18
  MOV DPTR, #TimeText
  MOV A, #'0'
TimeTextLoop:
  MOVX  @DPTR, A
  INC DPTR
  DJNZ  R0, TimeTextLoop

  MOV A, #':'     ;Set separator in time
  MOV DPTR, #TimeText+2
  MOVX  @DPTR, A
  MOV DPTR, #TimeText+5
  MOVX  @DPTR, A

  MOV A, #'-'     ;Set separator in date
  MOV DPTR, #DateText+2
  MOVX  @DPTR, A
  MOV DPTR, #DateText+5
  MOVX  @DPTR, A

  CLR A
  MOV DPTR, #TimeText+8 ;Set NULL terminator in time string
  MOVX  @DPTR, A

  MOV DPTR, #DateText+8 ;Set NULL terminator in date string
  MOVX  @DPTR, A

  MOV DPTR, #OSDTextLN1 ;Set NULL terminator in text string 1
  MOVX  @DPTR, A
  MOV DPTR, #OSDTextLN2 ;Set NULL terminator in text string 2
  MOVX  @DPTR, A
  MOV DPTR, #OSDTextLN3 ;Set NULL terminator in text string 3
  MOVX  @DPTR, A

  MOV YPosLine1, #HIGH( 17) ;
; MOV Line1Cnt, #HIGH( 17)  ;
  MOV YPosLine1+1,  #LOW( 17) ;
; MOV Line1Cnt+1,  #LOW( 17)  ;

  MOV YPosLine2,  #HIGH( 37)  ;
  MOV YPosLine2+1,  #LOW( 37) ;

  MOV YPosLine3,  #HIGH( 57)  ;
  MOV YPosLine3+1,  #LOW( 57) ;

  ;****************************************************************
  ;Initialize register 1, ie. interrupt variables
  ;****************************************************************
  MOV absR0, #01    ;Reset S1S0 table pointer
  MOV absR1, #61    ; to G system, (ie. offset from M)

  MOV absR3, #HIGH( G1_F1L22)
  MOV absR4, #LOW( G1_F1L22)

  MOV absR6, #LOW( BlackBurst_G)

  ;****************************************************************
  ;Initialize status register
  ;The initial generator setup really doesn't matter as a complete
  ;setup wil be received from the master immediately after power-up,
  ;patterntype should be set at black burst though
  ;****************************************************************
  CLR A
  MOV LineStatus, A   ;Disable text line 1, 2, 3
  MOV LineStatus1, A  

  MOV Status, A   ;PAL system
  MOV DPTR, #StatusPort
  MOVX  @DPTR, A

  CLR PALSystem   ;PAL-G without ID
  MOV PatternNo, #BlackBurst  ;
  SETB  Status_SYSSEL
  LCALL GOffsetReset    ;Initialize offset etc

        ;  TSTXTX
        ;  2 1100
  MOV IE, #10010001B    ;Enable external-interrupts

  SETB  HWRunning   ;Enable generator card
Main:
  LCALL TestInputBuffer

  JC  NewCharReceived
  SJMP  Main

NewCharReceived:
  LCALL CharHandler
  SJMP  Main

;************************************************************************
;* Name:  RelLineCntIntr
;* Function:  Relative Line counter interrupt code, NOT TO BE USED!!
;* Revision:  2.0
;* Date:  980529, KEn
;* Input: -
;* Output:  -
;* Destroys:  -
;* Calls  -
;* Time:  -
;* Remarks: 19.6608MHz
;*    80C320:  4 oscillatorcycles/cycle -> 203 ns/cycle
;************************************************************************
RelLineCntIntrCode  SEGMENT CODE
  RSEG  RelLineCntIntrCode
  USING 1     ;SAME BANK AS DelayedFieldIntrCode

RelLineCntIntr:
  PUSH  PSW     ;+2
  PUSH  DPL     ;+2
  PUSH  DPH     ;+2
  PUSH  DPS     ;+2
  MOV DPS, #0     ;+3

  MOV PSW, #08H   ;+3 ALWAYS use register bank 1
  MOV R7, A     ;+1 Push accumulator

  JNB IE1, TestFieldUpdIntr ;+4 If HW field interrupt..
  CLR IE1     ;+2  reset field interrupt

  MOV TH1, LineOffset   ;+3  load offset to delayed field
  MOV TL1, LineOffset+1 ;+3

TestFieldUpdIntr:
  JBC TF1, TempFieldUpdIntr ;+4 If SW field interrupt service intr.
          ;   otherwise..
  LCALL UpdTextLines    ;+n 

  DJNZ  R0, NoS1S0Update  ;+3 If time to update S1S0..

  MOV DPTR, #S1S0_M   ;+3 Load basis pointer for Table Entry

  MOV A, R1     ;+1
  MOVC  A, @A+DPTR    ;+3 Get no. of lines to next S1S0 upd.

  JNZ  StoreNoOfLines   ;+3 If ( NoOfLines == 0)...
          ;    table entry is out of bound
  JB  Status_SYSSEL, TFULbl1  ;+4  reset table pointer to NTSC...
  MOV A, #61      ;+2  or PAL
TFULbl1:
  MOV R1, A     ;+1
  MOVC  A, @A+DPTR    ;+3 Get no. of lines to next S1S0 upd.

StoreNoOfLines:
  MOV R0, A     ;+1 Save no. of lines in R0

  INC R1      ;+1 Update pointer to next table entry

  MOV A, R1     ;+1
  MOVC  A, @A+DPTR    ;+3 Get linetype for next R0 lines
  MOV R2, A     ;+1 Save linetype information in R2

  INC R1      ;+1 Update pointer to next table entry

  MOV C, ACC.2    ;+2 Store field type in carry
  ANL A, #03      ;+2 Isolate S1S0
  MOV ACC.6, C    ;+2 Recall field type from carry

  ANL Status, #0BCH   ;+3 Remove S1S0 & field information
  ORL Status, A   ;+2 Update status: S1S0 & field bit

  JBC TF0, PatternUpdate  ;+4

  MOV A, Status   ;+2 Update status port with S1S0
  MOV DPTR, #StatusPort ;+3
  MOVX  @DPTR, A    ;+3

  MOV A, R2     ;+1 Linetype F8 -> 1F: DO NOT UPDATE
  SWAP  A     ;+1
  RL  A     ;+1
  ANL A, #01FH    ;+2 Acc. = Linetype

  CJNE  A, #01FH, FindLineType  ;+4

  MOV A, R7     ;+1 Linetype = 01FH, ie. DO NOT UPDATE
  POP DPS     ;+2
  POP DPH     ;+2
  POP DPL     ;+2
  POP PSW     ;+2
  RETI        ;+4

TempFieldUpdIntr:
  LJMP  FieldUpdIntr    ;+4 Jump to interrupt routine handler

FindLineType:
  JB  Status_SYSSEL, UpdLineType  ;+4
  JB  PALSystem, UpdLineType    ;+4 If system is PAL-ID..

  CJNE  A, #00FH, UpdLineType ;+4 If line 7, ie. line with PAL-ID..
  MOV A, #BlackBurst_G  ;+2  reset line to black burst
UpdLineType:
  MOV DPTR, #LineAdrPort  ;+3 NOTE: GenericMSBPort MUST contain 0
  MOVX  @DPTR, A    ;+3 Write update to line port

  MOV A, R7     ;+1
  POP DPS     ;+2
  POP DPH     ;+2
  POP DPL     ;+2
  POP PSW     ;+2
  RETI        ;+4

NoS1S0Update:
  JBC TF0, PatternUpdate  ;+4
  JBC UpdOffsetFlag, UpdOffset;+4

  MOV A, R7     ;+1
  POP DPS     ;+2
  POP DPH     ;+2
  POP DPL     ;+2
  POP PSW     ;+2
  RETI        ;+4

PatternUpdate:
  MOV DPH, R3     ;+2 Update pointer to routine
  MOV DPL, R4     ;+2

  CLR A     ;+1 
  JMP @A+DPTR     ;+3 Jump to interrupt routine

UpdOffset:        ;Update lineoffset every F4/F8
  ;NOTE: GenericMSBPort MUST contain 0
  ;AND should ALWAYS be set to ZERO
  ;before writing the FineOffset

  MOV A, FineOffset+1   ;+2
  MOV DPTR, #FinePortL  ;+3 NB! LSB MUST be loaded before MSB
  MOVX  @DPTR, A    ;+3

  MOV A, FineOffset   ;+2
  MOV DPTR, #FinePortH  ;+3
  MOVX  @DPTR, A    ;+3

  MOV A, CoarseOffset   ;+2
  MOV DPTR, #GenericMSBPort ;+3
  MOVX  @DPTR, A    ;+3

  MOV A, CoarseOffset+1 ;+2
  MOV DPTR, #CoarsePort ;+3
  JNB CoarseGTMax, LoadCoarseOffset ;+4
  INC DPTR      ;+3
LoadCoarseOffset:
  MOVX  @DPTR, A    ;+3

  CLR A     ;+1 NOTE: GenericMSBPort MUST contain 0
  MOV DPTR, #GenericMSBPort ;+3  AND should ALWAYS be set to ZERO
  MOVX  @DPTR, A    ;+3  before writing the FineOffset

  MOV A, R7     ;+1
  POP DPS     ;+2
  POP DPH     ;+2
  POP DPL     ;+2
  POP PSW     ;+2
  RETI        ;+4

;************************************************************************
;* Name:  UpdTextLines
;* Function:  
;* Revision:  1.0 (the DALLAS 80C320)
;* Date:  980327, KEn
;* Input: -
;* Output:  -
;* Destroys:  -
;* Calls  -
;* Time:  ?
;************************************************************************
UpdTextLines:       ;Text line 1
  MOV A, Line1Cnt+01H   ;+2
  DEC Line1Cnt+01H    ;+2
  JNZ ?UTL1_06    ;+3
  DEC Line1Cnt    ;+2
?UTL1_06:
  DEC A     ;+1
  ORL A, Line1Cnt   ;+2
  JNZ ?UTL2_00    ;+3

  JNB Line1On, ?UTL1_02 ;+4

  JB  Status_SYSSEL, ?UTL1_06A;+4 If selected system is PAL..
  MOV Line1Cnt, #01H    ;+3  next update in 296/297 lines
  MOV A, #029H    ;+2 
  SJMP  ?UTL1_06B   ;+3

?UTL1_06A:        ;   ..else system is NTSC
  MOV Line1Cnt, #00H    ;+3  next update in 247/246 lines
  MOV A, #0F7H    ;+2

?UTL1_06B:
  MOV C, FieldCnt1    ;+2
  SUBB  A, #0     ;+2
  MOV Line1Cnt+01H, A   ;+2

?UTL1_06C:
  CPL FieldCnt1   ;+2
  CLR Status_TXT_0    ;+2

  SJMP  ?UTL1_03    ;+3
?UTL1_02:
  MOV Line1Cnt+01H, #010H ;+3 Line1Cnt = 16

  JNB Line1Enable, ?UTL1_03 ;+4
  SETB  Status_TXT_0    ;+2
?UTL1_03:
  CPL Line1On     ;+2

  MOV A, Status   ;+2 Update status port with text
  MOV DPTR, #StatusPort ;+3
  MOVX  @DPTR, A    ;+3
;---------------------------------------;
?UTL2_00:       ;Text line 2
  MOV A, Line2Cnt+01H   ;+2
  DEC Line2Cnt+01H    ;+2
  JNZ ?UTL2_06    ;+3
  DEC Line2Cnt    ;+2
?UTL2_06:
  DEC A     ;+1
  ORL A, Line2Cnt   ;+2
  JNZ ?UTL3_00    ;+3

  JNB Line2On, ?UTL2_02 ;+4

  JB  Status_SYSSEL, ?UTL2_06A;+4 If selected system is PAL..
  MOV Line2Cnt, #01H    ;+3  next update in 296/297 lines
  MOV A, #029H    ;+2 
  SJMP  ?UTL2_06B   ;+3

?UTL2_06A:        ;   ..else system is NTSC
  MOV Line2Cnt, #00H    ;+3  next update in 247/246 lines
  MOV A, #0F7H    ;+2

?UTL2_06B:
  MOV C, FieldCnt2    ;+2
  SUBB  A, #0     ;+2
  MOV Line2Cnt+01H, A ;+2

  CPL FieldCnt2   ;+2
  CLR Status_TXT_1    ;+2

  SJMP  ?UTL2_03    ;+3
?UTL2_02:
  MOV Line2Cnt+01H, #010H ;+3 Line2Cnt = 16

  JNB Line2Enable, ?UTL2_03 ;+4
  SETB  Status_TXT_1    ;+2
?UTL2_03:
  CPL Line2On     ;+2

  MOV A, Status   ;+2 Update status port with text
  MOV DPTR, #StatusPort ;+3
  MOVX  @DPTR, A    ;+3
;---------------------------------------;
?UTL3_00:       ;Text line 3
  MOV A, Line3Cnt+01H   ;+2
  DEC Line3Cnt+01H    ;+2
  JNZ ?UTL3_06    ;+3
  DEC Line3Cnt    ;+2
?UTL3_06:
  DEC A     ;+1
  ORL A, Line3Cnt   ;+2
  JNZ ?UTL_END    ;+3

  JNB Line3On, ?UTL3_02 ;+4

  JB  Status_SYSSEL, ?UTL3_06A;+4 If selected system is PAL..
  MOV Line3Cnt, #01H    ;+3  next update in 296/297 lines
  MOV A, #029H    ;+2 
  SJMP  ?UTL3_06B   ;+3

?UTL3_06A:        ;   ..else system is NTSC
  MOV Line3Cnt, #00H    ;+3  next update in 247/246 lines
  MOV A, #0F7H    ;+2

?UTL3_06B:
  MOV C, FieldCnt3    ;+2
  SUBB  A, #0     ;+2
  MOV Line3Cnt+01H, A   ;+2

  CPL FieldCnt3   ;+2
  ANL Status, #0E7H   ;+2 Status_TXT_0 = Status_TXT_1 = 0

  SJMP  ?UTL3_03    ;+3
?UTL3_02:
  MOV Line3Cnt+01H, #010H ;+3 Line3Cnt = 16

  JNB Line3Enable, ?UTL3_03 ;+4
  ORL Status, #018H   ;+2 Status_TXT_0 = Status_TXT_1 = 1
?UTL3_03:
  CPL Line3On     ;+2

  MOV A, Status   ;+2 Update status port with text
  MOV DPTR, #StatusPort ;+3
  MOVX  @DPTR, A    ;+3
?UTL_END:
  RET

;************************************************************************
;* Name:  DelFieldIntr
;* Function:  Delayed field interrupt
;* Revision:  2.1
;* Date:  980925, KEn
;* Input: -
;* Output:  -
;* Destroys:  -
;* Calls  -
;* Time:  Worst case: 53 cycles + 5 cycles, (ie. PAL-G)
;************************************************************************
DelayedFieldIntrCode  SEGMENT CODE
  RSEG  DelayedFieldIntrCode
  USING 1

FieldUpdIntr:
  SETB  UpdOffsetFlag   ;+1 Flag for update of line offset

  MOV A, Status   ;+1
  SETB  ACC.7     ;+1 Set bit 7 in ONE line ONLY every
  MOV DPTR, #StatusPort ;+2  4 fields
  MOVX  @DPTR, A    ;+2

  MOV A, ScHOffset    ;+1
  MOV DPTR, #GenericMSBPort ;+2
  MOVX  @DPTR, A    ;+2

  MOV A, ScHOffset+1    ;+1
  MOV DPTR, #ScHPhasePort ;+2
  MOVX  @DPTR, A    ;+2

  CLR A     ;+1 NOTE: GenericMSBPort MUST contain 0
  MOV DPTR, #GenericMSBPort ;+2
  MOVX  @DPTR, A    ;+2

  MOV R0, #01     ;+1 Reset S1S0 table

  MOV Line1Cnt, YPosLine1   ;+3 Reload line 1 Y-coordinate
  MOV Line1Cnt+01H, YPosLine1+01H ;+3

  MOV Line2Cnt, YPosLine2   ;+3 Reload line 2 Y-coordinate
  MOV Line2Cnt+01H, YPosLine2+01H ;+3

  MOV Line3Cnt, YPosLine3   ;+3 Reload line 3 Y-coordinate
  MOV Line3Cnt+01H, YPosLine3+01H ;+3

  ANL LineStatus, #0F8H ;+2 Clear Line1On, Line2On, Line3On

  JB  Status_SYSSEL, DelFieldIntr_M
  LJMP  DelFieldIntr_G

;**************************************************************************
;* NTSC SYSTEM
;* Worst case timing: 53 cycles + 3 cycles
;**************************************************************************
DelFieldIntr_M:
  ORL LineStatus1, #038H  ;+2 SETB FieldCnt1, 2, 3

  MOV R1, #00     ;+1 Offset from S1S0_M

  MOV TH0, #HIGH( -21)  ;+2 Next active pattern interrupt
  MOV TL0, #LOW( -21)   ;+2  in line 20

  MOV A, PatternNo    ;+1
  MOV DPTR, #PatternTable_M ;+2
  JMP @A+DPTR     ;+2

PatternTable_M:
  LJMP  Pattern0_M    ;+2 SMPTE Colorbar
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, EBU Colorbar
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, Colorbar 100%
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, Colorbar 75%+grey
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, Colorbar 75%+red
  LJMP  Pattern5_M    ;+2 Window 15%
  LJMP  Pattern6_M    ;+2 Window 20%
  LJMP  Pattern7_M    ;+2 Window 100%
  LJMP  Pattern8_M    ;+2 Crosshatch
  LJMP  Pattern9_M    ;+2 PLUGE
  LJMP  Pattern10_M    ;+2 Safe Area
  LJMP  Pattern11_M    ;+2 Shallowramp
  LJMP  Pattern12_M    ;+2 Multiburst / CCIR18
  LJMP  Pattern13_M    ;+2 Red 75%
  LJMP  Pattern14_M    ;+2 Blackburst
  LJMP  Pattern15_M    ;+2 Philips
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, Philips wo/APAL
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, CCIR17
  LJMP  Pattern18_M    ;+2 Multipulse
  LJMP  Pattern19_M    ;+2 Sinxx
  LJMP  Pattern20_M    ;+2 Luminance Sweep
  LJMP  Pattern21_M    ;+2 FCC CBar
  LJMP  Pattern22_M    ;+2 NTC7 Combination
  LJMP  Pattern23_M    ;+2 FCC Mulitburst
  LJMP  Pattern24_M    ;+2 50% Flat Field
  LJMP  Pattern25_M    ;+2 Flat 100%
  LJMP  Pattern26_M    ;+2 Field Square Wave
  LJMP  Pattern27_M    ;+2 0.1Hz Bl/Wh
  LJMP  Pattern28_M    ;+2 Luminance Ramp
  LJMP  Pattern29_M    ;+2 Modulated Ramp
  LJMP  Pattern30_M    ;+2 Staircase 5-step
  LJMP  Pattern31_M    ;+2 Modulated stair, 5-step
  LJMP  Pattern32_M    ;+2 Staircase 10-step
  LJMP  Pattern33_M    ;+2 Pulse & Bar
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, CCIR330
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, CCIR331
  LJMP  Pattern36_M    ;+2 FCC Composite
  LJMP  Pattern37_M    ;+2 NTC7 Composite
  LJMP  Pattern38_M    ;+2 250kHz
  LJMP  Pattern14_M    ;+2 ONLY USED in 625, VMT 01
  LJMP  Pattern14_M    ;+2 Custom pattern 1
  LJMP  Pattern14_M    ;+2 Custom pattern 2
  LJMP  Pattern14_M    ;+2 Custom pattern 3
  LJMP  Pattern14_M    ;+2 Custom pattern 4
  LJMP  Pattern14_M    ;+2 Custom pattern 5
  LJMP  Pattern14_M    ;+2 Custom pattern 6
  LJMP  Pattern14_M    ;+2 Custom pattern 7
  LJMP  Pattern14_M    ;+2 Custom pattern 8
  LJMP  Pattern14_M    ;+2 ONLY USED IN 625, FuBK 4:3
  LJMP  Pattern14_M    ;+2 ONLY USED IN 625, FuBK 16:9
  LJMP  Pattern50_M    ;+2 Philips 16:9
  LJMP  Pattern51_M    ;+2 Crosshatch 16:9
  LJMP  Pattern14_M    ;+2 ONLY USED IN 625, WhiteCircle 4:3
  LJMP  Pattern14_M    ;+2 ONLY USED IN 625, WhiteCircle 16:9
  LJMP  Pattern54_M    ;+2 Window 10%
  LJMP  Pattern55_M    ;+2 15kHz Bl/Wh

;**************************************************************************
Pattern0_M:        ;SMPTE Colorbar, (525 only)
  MOV  R3, #HIGH( M3_F1L20)  ;+6/+10
  MOV  R4, #LOW( M3_F1L20)

  JBC  NewPattern, NewPattern0_M
  LJMP  FieldUpdIntrEnd

NewPattern0_M:
  MOV  PatternPtr, #HIGH( CBSMPTEPattM)
  MOV  PatternPtr+1, #LOW( CBSMPTEPattM)

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern5_M:        ;Window 15%
  MOV  R3, #HIGH( M2_F1L20)  ;+6/+10
  MOV  R4, #LOW( M2_F1L20)

  JBC  NewPattern, NewPattern5_M
  LJMP  FieldUpdIntrEnd

NewPattern5_M:
  MOV  PatternPtr, #HIGH( Win15PattM)
  MOV  PatternPtr+1, #LOW( Win15PattM)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern6_M:        ;Window 20%
  MOV  R3, #HIGH( M2_F1L20)  ;+6/+10
  MOV  R4, #LOW( M2_F1L20)

  JBC  NewPattern, NewPattern6_M
  LJMP  FieldUpdIntrEnd

NewPattern6_M:
  MOV  PatternPtr, #HIGH( Win20PattM)
  MOV  PatternPtr+1, #LOW( Win20PattM)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern7_M:        ;Window 100%
  MOV  R3, #HIGH( M2_F1L20)  ;+6/+10
  MOV  R4, #LOW( M2_F1L20)

  JBC  NewPattern, NewPattern7_M
  LJMP  FieldUpdIntrEnd

NewPattern7_M:
  MOV  PatternPtr, #HIGH( Win100PattM)
  MOV  PatternPtr+1, #LOW( Win100PattM)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern8_M:        ;Cross hatch-4:3
  MOV  R3, #HIGH( M6_F1L20)  ;+6/+10
  MOV  R4, #LOW( M6_F1L20)

  JBC  NewPattern, NewPattern8_M
  LJMP  FieldUpdIntrEnd

NewPattern8_M:
  MOV  PatternPtr, #HIGH( CrossPattM)
  MOV  PatternPtr+1, #LOW( CrossPattM)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern9_M:        ;PLUGE
  MOV  R3, #HIGH( M4_F1L20)  ;+6/+10
  MOV  R4, #LOW( M4_F1L20)

  JBC  NewPattern, NewPattern9_M
  LJMP  FieldUpdIntrEnd

NewPattern9_M:
  MOV  PatternPtr, #HIGH( PLUGEPattM)
  MOV  PatternPtr+1, #LOW( PLUGEPattM)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern10_M:        ;Safe Area
  MOV  R3, #HIGH( M5_F1L20)  ;+6/+10
  MOV  R4, #LOW( M5_F1L20)

  JBC  NewPattern, NewPattern10_M
  LJMP  FieldUpdIntrEnd

NewPattern10_M:
  MOV  PatternPtr, #HIGH( SafePattM)
  MOV  PatternPtr+1, #LOW( SafePattM)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern11_M:        ;Shallowramp
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern11_M
  LJMP  FieldUpdIntrEnd

NewPattern11_M:
  MOV  R6, #ShallowRamp_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern12_M:        ;Multiburst
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern12_M
  LJMP  FieldUpdIntrEnd

NewPattern12_M:
  MOV  R6, #FCC_Multiburst_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern13_M:        ;75% Red
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern13_M
  LJMP  FieldUpdIntrEnd

NewPattern13_M:
  MOV  R6, #Red75_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern14_M:        ;Black burst
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern14_M
  LJMP  FieldUpdIntrEnd

NewPattern14_M:
  MOV  R6, #Sync_Burst_Setup_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern15_M:        ;Philips
  MOV  R3, #HIGH( M8_F1L20)  ;+/+
  MOV  R4, #LOW( M8_F1L20)

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern18_M:        ;Multipulse
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern18_M
  LJMP  FieldUpdIntrEnd

NewPattern18_M:
  MOV  R6, #Multipulse_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern19_M:        ;Sinxx
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern19_M
  LJMP  FieldUpdIntrEnd

NewPattern19_M:
  MOV  R6, #Sinx42_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern20_M:        ;Luminance Sweep

  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern20_M
  LJMP  FieldUpdIntrEnd

NewPattern20_M:
  MOV  R6, #LineSweep_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern21_M:        ;FCC CBar
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern21_M
  LJMP  FieldUpdIntrEnd

NewPattern21_M:
  MOV  R6, #FCC_CB_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern22_M:        ;NTC7 Combination
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern22_M
  LJMP  FieldUpdIntrEnd

NewPattern22_M:
  MOV  R6, #NTC7Combination_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern23_M:        ;FCC Mulitburst
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern23_M
  LJMP  FieldUpdIntrEnd

NewPattern23_M:
  MOV  R6, #FCC_Multiburst_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern24_M:        ;50% Flat Field
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern24_M
  LJMP  FieldUpdIntrEnd

NewPattern24_M:
  MOV  R6, #WhiteLine50_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern25_M:        ;Flat 100%
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern25_M
  LJMP  FieldUpdIntrEnd

NewPattern25_M:
  MOV  R6, #WhiteLine_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern26_M:        ;Field Square Wave
  MOV  R3, #HIGH( M7_F1L20)  ;+6/+10
  MOV  R4, #LOW( M7_F1L20)

  JBC  NewPattern, NewPattern26_M
  LJMP  FieldUpdIntrEnd

NewPattern26_M:
  MOV  PatternPtr, #HIGH( FSWaveM)
  MOV  PatternPtr+1, #LOW( FSWaveM)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern27_M:        ;0.1Hz Bl/Wh
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern27_M

          ;150*f/4 = 10.0000000
  MOV  A, NFrameCnt
  CJNE  A, #151, ?P27_M1  ;If ( NFrameCnt++ > 150)
  CLR  A
?P27_M1:
  INC  A
  MOV  NFrameCnt, A

  CLR  C
  SUBB  A, #76
  JC  ?P27_M2      ;If NFrameCnt > 75

  MOV  R6, #WhiteLine_M
  LJMP  FieldUpdIntrEnd
?P27_M2:
  MOV  R6, #Sync_Burst_Setup_M
  LJMP  FieldUpdIntrEnd

NewPattern27_M:
  MOV  R6, #Sync_Burst_Setup_M
  MOV  NFrameCnt, #1

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern28_M:        ;Luminance Ramp
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern28_M
  LJMP  FieldUpdIntrEnd

NewPattern28_M:
  MOV  R6, #LuminanceRamp_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern29_M:        ;Modulated Ramp
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern29_M
  LJMP  FieldUpdIntrEnd

NewPattern29_M:
  MOV  R6, #ModRampY_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern30_M:        ;Staircase 5-step
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern30_M
  LJMP  FieldUpdIntrEnd

NewPattern30_M:
  MOV  R6, #GreyScale5_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern31_M:        ;Modulated stair, 5-step
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern31_M
  LJMP  FieldUpdIntrEnd

NewPattern31_M:
  MOV  R6, #ModGreyScale5_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern32_M:        ;Staircase 10-step
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern32_M
  LJMP  FieldUpdIntrEnd

NewPattern32_M:
  MOV  R6, #GreyScale10_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern33_M:        ;Pulse & Bar
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern33_M
  LJMP  FieldUpdIntrEnd

NewPattern33_M:
  MOV  R6, #T2T20BarInv2T_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern36_M:        ;FCC Composite
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern36_M
  LJMP  FieldUpdIntrEnd

NewPattern36_M:
  MOV  R6, #FCCComposite_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern37_M:        ;NTC7 Composite
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern37_M
  LJMP  FieldUpdIntrEnd

NewPattern37_M:
  MOV  R6, #NTC7Composite_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern38_M:        ;250kHz
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern38_M
  LJMP  FieldUpdIntrEnd

NewPattern38_M:
  MOV  R6, #kHz250_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern50_M:        ;Philips 16:9
  MOV  R3, #HIGH( M9_F1L20)  ;+/+
  MOV  R4, #LOW( M9_F1L20)

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern51_M:        ;Cross hatch-16:9
  MOV  R3, #HIGH( M6_F1L20)  ;+6/+10
  MOV  R4, #LOW( M6_F1L20)

  JBC  NewPattern, NewPattern51_M
  LJMP  FieldUpdIntrEnd

NewPattern51_M:
  MOV  PatternPtr, #HIGH( CrossPatt16x9M)
  MOV  PatternPtr+1, #LOW( CrossPatt16x9M)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern54_M:        ;Window 10%
  MOV  R3, #HIGH( M2_F1L20)  ;+6/+10
  MOV  R4, #LOW( M2_F1L20)

  JBC  NewPattern, NewPattern54_M
  LJMP  FieldUpdIntrEnd

NewPattern54_M:
  MOV  PatternPtr, #HIGH( Win10PattM)
  MOV  PatternPtr+1, #LOW( Win10PattM)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern55_M:        ;165kHz Bl/Wh
  MOV  R3, #HIGH( M1_F1L20)  ;+6/+7
  MOV  R4, #LOW( M1_F1L20)

  JBC  NewPattern, NewPattern55_M
  LJMP  FieldUpdIntrEnd

NewPattern55_M:
  MOV  R6, #kHz15BlWh_M
  LJMP  FieldUpdIntrEnd

;**************************************************************************
;* PAL-G/PAL-G with ID SYSTEM
;* Worst case timing: 53 cycles + 5 cycle
;**************************************************************************
DelFieldIntr_G:
  ANL  LineStatus1, #0C7H  ;+2 CLR FieldCnt1, 2, 3

  MOV  R1, #61      ;+1 Offset from S1S0_M

  MOV  TH0, #HIGH( -23)  ;+2 Next active pattern interrupt
  MOV  TL0, #LOW( -23)    ;+2  in line 22

  MOV  A, PatternNo    ;+1
  MOV  DPTR, #SetPatternTable  ;+2
  JMP  @A+DPTR      ;+2

SetPatternTable:
  LJMP  Pattern14_G    ;+2 ONLY USED in 525, BlackBurst
  LJMP  Pattern1_G    ;+2 EBU Colorbar
  LJMP  Pattern2_G    ;+2 Colorbar 100%
  LJMP  Pattern3_G    ;+2 Colorbar 75%+grey
  LJMP  Pattern4_G    ;+2 Colorbar 75%+red
  LJMP  Pattern5_G    ;+2 Window 15%
  LJMP  Pattern6_G    ;+2 Window 20%
  LJMP  Pattern7_G    ;+2 Window 100%
  LJMP  Pattern8_G    ;+2 Crosshatch
  LJMP  Pattern9_G    ;+2 PLUGE
  LJMP  Pattern10_G    ;+2 Safe Area
  LJMP  Pattern11_G    ;+2 Shallowramp
  LJMP  Pattern12_G    ;+2 Multiburst/CCIR18
  LJMP  Pattern13_G    ;+2 Red 75%
  LJMP  Pattern14_G    ;+2 Blackburst
  LJMP  Pattern15_G    ;+2 Philips
  LJMP  Pattern16_G    ;+2 Philips wo/APAL
  LJMP  Pattern17_G    ;+2 CCIR17
  LJMP  Pattern18_G    ;+2 Multipulse
  LJMP  Pattern19_G    ;+2 Sincxx
  LJMP  Pattern20_G    ;+2 Luminance Sweep
  LJMP  Pattern14_G    ;+2 ONLY USED in 525, FCC CBar
  LJMP  Pattern14_G    ;+2 ONLY USED in 525, NTC7 Combination
  LJMP  Pattern14_G    ;+2 ONLY USED in 525, FCC Mulitburst
  LJMP  Pattern24_G    ;+2 50% Flat Field
  LJMP  Pattern25_G    ;+2 Flat 100%        /**************** Flat100 *********/
  LJMP  Pattern26_G    ;+2 Field Square Wave
  LJMP  Pattern27_G    ;+2 0.1Hz Bl/Wh
  LJMP  Pattern28_G    ;+2 Luminance Ramp
  LJMP  Pattern29_G    ;+2 Modulated Ramp
  LJMP  Pattern30_G    ;+2 Staircase 5-step
  LJMP  Pattern31_G    ;+2 Modulated stair, 5-step
  LJMP  Pattern32_G    ;+2 Staircase 10-step
  LJMP  Pattern33_G    ;+2 Pulse & Bar
  LJMP  Pattern34_G    ;+2 CCIR330
  LJMP  Pattern35_G    ;+2 CCIR331
  LJMP  Pattern14_G    ;+2 ONLY USED in 525, FCC Composite
  LJMP  Pattern14_G    ;+2 ONLY USED in 525, NTC7 Composite
  LJMP  Pattern38_G    ;+2 250kHz
  LJMP  Pattern39_G    ;+2 VMT 01
  LJMP  Pattern14_G    ;+2 Custom pattern 1
  LJMP  Pattern14_G    ;+2 Custom pattern 2
  LJMP  Pattern14_G    ;+2 Custom pattern 3
  LJMP  Pattern14_G    ;+2 Custom pattern 4
  LJMP  Pattern14_G    ;+2 Custom pattern 5
  LJMP  Pattern14_G    ;+2 Custom pattern 6
  LJMP  Pattern14_G    ;+2 Custom pattern 7
  LJMP  Pattern14_G    ;+2 Custom pattern 8
  LJMP  Pattern48_G    ;+2 FuBK 4:3
  LJMP  Pattern49_G    ;+2 FuBK 16:9
  LJMP  Pattern50_G    ;+2 Philips 16:9
  LJMP  Pattern51_G    ;+2 Crosshatch 16:9
  LJMP  Pattern52_G    ;+2 WhiteCircle 4:3
  LJMP  Pattern53_G    ;+2 WhiteCircle 16:9
  LJMP  Pattern54_G    ;+2 Window 10%
  LJMP  Pattern55_G    ;+2 15kHz Bl/Wh

;**************************************************************************
Pattern1_G:        ;EBU Colorbar, (625 only)
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)


  JBC  NewPattern, NewPattern1_G
  LJMP  FieldUpdIntrEnd

NewPattern1_G:
  MOV  R6, #EBUCB_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern2_G:        ;100% Colorbar, (625 only)
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern2_G
  LJMP  FieldUpdIntrEnd

NewPattern2_G:
  MOV  R6, #CB100_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern3_G:        ;75% Colorbar+grey, (625 only)
  MOV  R3, #HIGH( G3_F1L22)  ;+6/+10
  MOV  R4, #LOW( G3_F1L22)

  JBC  NewPattern, NewPattern3_G
  LJMP  FieldUpdIntrEnd

NewPattern3_G:
  MOV  PatternPtr, #HIGH( CB75GreyPattG)
  MOV  PatternPtr+1, #LOW( CB75GreyPattG)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern4_G:        ;EBU Colorbar+Red, (625 only)
  MOV  R3, #HIGH( G3_F1L22)  ;+6/+10
  MOV  R4, #LOW( G3_F1L22)

  JBC  NewPattern, NewPattern4_G
  LJMP  FieldUpdIntrEnd

NewPattern4_G:
  MOV  PatternPtr, #HIGH( EBUCBRedPattG)
  MOV  PatternPtr+1, #LOW( EBUCBRedPattG)
  LJMP  FieldUpdIntrEnd

;/**************************************************************************/
Pattern5_G:                  ;Window 15%     ;/******** Window15 ***********/
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+10
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern5_G
  LJMP  FieldUpdIntrEnd

NewPattern5_G:
  MOV  R6, #Window15_G                       ;/* Window15 uses Window15_G */
  LJMP  FieldUpdIntrEnd

;/**************************************************************************/
Pattern6_G:                  ;Window 20%     ;/********* Window20 **********/
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+10
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern6_G
  LJMP  FieldUpdIntrEnd

NewPattern6_G:
  MOV  R6, #Window20_G ;                      /* Window20 uses Window20_G */
  LJMP  FieldUpdIntrEnd

;/**************************************************************************/
Pattern7_G:                  ;Window 100%     ;/*** Window100 (no change)***/
  MOV  R3, #HIGH( G2_F1L22)  ;+6/+10
  MOV  R4, #LOW( G2_F1L22)

  JBC  NewPattern, NewPattern7_G
  LJMP  FieldUpdIntrEnd

NewPattern7_G:
  MOV  PatternPtr, #HIGH( Win100PattG)
  MOV  PatternPtr+1, #LOW( Win100PattG)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern8_G:        ;Crosshatch-4:3
  MOV  R3, #HIGH( G6_F1L22)  ;+6/+10
  MOV  R4, #LOW( G6_F1L22)

  JBC  NewPattern, NewPattern8_G
  LJMP  FieldUpdIntrEnd

NewPattern8_G:
  MOV  PatternPtr, #HIGH( CrossPattG)
  MOV  PatternPtr+1, #LOW( CrossPattG)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern9_G:        ;PLUGE
  MOV  R3, #HIGH( G4_F1L22)  ;+6/+10
  MOV  R4, #LOW( G4_F1L22)

  JBC  NewPattern, NewPattern9_G
  LJMP  FieldUpdIntrEnd

NewPattern9_G:
  MOV  PatternPtr, #HIGH( PLUGEPattG)
  MOV  PatternPtr+1, #LOW( PLUGEPattG)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern10_G:        ;Safe Area
  MOV  R3, #HIGH( G5_F1L22)  ;+6/+10
  MOV  R4, #LOW( G5_F1L22)  

  JBC  NewPattern, NewPattern10_G
  LJMP  FieldUpdIntrEnd

NewPattern10_G:
  MOV  PatternPtr, #HIGH( SafePattG)
  MOV  PatternPtr+1, #LOW( SafePattG)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern11_G:        ;Shallowramp
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern11_G
  LJMP  FieldUpdIntrEnd

NewPattern11_G:
  MOV  R6, #ShallowRamp_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern12_G:        ;Multiburst / CCIR18
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern12_G
  LJMP  FieldUpdIntrEnd

NewPattern12_G:
  MOV  R6, #_CCIR18_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern13_G:        ;75% Red
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern13_G
  LJMP  FieldUpdIntrEnd

NewPattern13_G:
  MOV  R6, #Red75_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern14_G:        ;Black burst
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern14_G
  LJMP  FieldUpdIntrEnd

NewPattern14_G:
  MOV  R6, #BlackBurst_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern15_G:        ;Philips
Pattern16_G:        ;Philips wo/APAL
  MOV  A, PatternAttr
  JNB  ACC.0, Pattern16woAPAL_G

  MOV  R3, #HIGH( G7_F1L22)  ;+/+
  MOV  R4, #LOW( G7_F1L22)

  LJMP  FieldUpdIntrEnd

Pattern16woAPAL_G:
  MOV  R3, #HIGH( G8_F1L22)  ;+/+
  MOV  R4, #LOW( G8_F1L22)

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern17_G:        ;CCIR17
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern17_G
  LJMP  FieldUpdIntrEnd

NewPattern17_G:
  MOV  R6, #CCIR17_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern18_G:        ;Multipulse
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern18_G
  LJMP  FieldUpdIntrEnd

NewPattern18_G:
  MOV  R6, #Multipulse_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern19_G:        ;Sinxx
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern19_G
  LJMP  FieldUpdIntrEnd

NewPattern19_G:
  MOV  R6, #Sinc_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern20_G:        ;Luminance sweep
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern20_G
  LJMP  FieldUpdIntrEnd

NewPattern20_G:
  MOV  R6, #LineSweep_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern24_G:        ;50% Flat Field
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern24_G
  LJMP  FieldUpdIntrEnd

NewPattern24_G:
  MOV  R6, #WhiteLine50_G
  LJMP  FieldUpdIntrEnd

;/**************************************************************************/
Pattern25_G:        ;Flat 100%     ;/*************** Flat100 *************/
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern25_G
  LJMP  FieldUpdIntrEnd

NewPattern25_G:
  MOV  R6, #WhiteLine_G ;/* Flat100 uses WhiteLine_G */
  LJMP  FieldUpdIntrEnd

;/**************************************************************************/
Pattern26_G:        ;Field Square Wave
  MOV  R3, #HIGH( G9_F1L22)  ;+6/+10
  MOV  R4, #LOW( G9_F1L22)

  JBC  NewPattern, NewPattern26_G
  LJMP  FieldUpdIntrEnd

NewPattern26_G:
  MOV  PatternPtr, #HIGH( FSWaveG)
  MOV  PatternPtr+1, #LOW( FSWaveG)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern27_G:        ;0.1Hz Bl/Wh
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern27_G

          ;62*f/8 = 9.92
  MOV  A, NFrameCnt
  CJNE  A, #63, ?P27_G1    ;If ( NFrameCnt++ > 62)
  CLR  A
?P27_G1:
  INC  A
  MOV  NFrameCnt, A

  CLR  C
  SUBB  A, #32
  JC  ?P27_G2      ;If NFrameCnt > 31

  MOV  R6, #WhiteLine_G
  LJMP  FieldUpdIntrEnd
?P27_G2:
  MOV  R6, #BlackBurst_G
  LJMP  FieldUpdIntrEnd

NewPattern27_G:
  MOV  R6, #BlackBurst_G
  MOV  NFrameCnt, #1

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern28_G:        ;Luminance Ramp
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern28_G
  LJMP  FieldUpdIntrEnd

NewPattern28_G:
  MOV  R6, #LuminanceRamp_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern29_G:        ;Modulated Ramp
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern29_G
  LJMP  FieldUpdIntrEnd

NewPattern29_G:
  MOV  R6, #ModRampY_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern30_G:        ;Staircase 5-step
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern30_G
  LJMP  FieldUpdIntrEnd

NewPattern30_G:
  MOV  R6, #GreyScale5_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern31_G:        ;Modulated stair, 5-step
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern31_G
  LJMP  FieldUpdIntrEnd

NewPattern31_G:
  MOV  R6, #ModGreyScale5_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern32_G:        ;Staircase 10-step
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern32_G
  LJMP  FieldUpdIntrEnd

NewPattern32_G:
  MOV  R6, #GreyScale10_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern33_G:        ;Pulse & Bar
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern33_G
  LJMP  FieldUpdIntrEnd

NewPattern33_G:
  MOV  R6, #T2T20BarInv2T_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern34_G:        ;CCIR330
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern34_G
  LJMP  FieldUpdIntrEnd

NewPattern34_G:
  MOV  R6, #CCIR330_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern35_G:        ;CCIR331
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern35_G
  LJMP  FieldUpdIntrEnd

NewPattern35_G:
  MOV  R6, #CCIR331_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern38_G:        ;250kHz
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern38_G
  LJMP  FieldUpdIntrEnd

NewPattern38_G:
  MOV  R6, #kHz250_G
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern39_G:        ;VMT 01
  MOV  R3, #HIGH( G10_F1L22)  ;+6/+10
  MOV  R4, #LOW( G10_F1L22)

  JBC  NewPattern, NewPattern39_G
  LJMP  FieldUpdIntrEnd

NewPattern39_G:
  MOV  PatternPtr, #HIGH( VMT01G)
  MOV  PatternPtr+1, #LOW( VMT01G)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern48_G:        ;FuBK-4:3
  MOV  R3, #HIGH( G12_F1L22)  ;+/+
  MOV  R4, #LOW( G12_F1L22)

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern49_G:        ;FuBK-16:9
  MOV  R3, #HIGH( G13_F1L22)  ;+/+
  MOV  R4, #LOW( G13_F1L22)

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern50_G:        ;Philips-16:9
  MOV  R3, #HIGH( G11_F1L22)  ;+/+
  MOV  R4, #LOW( G11_F1L22)

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern51_G:        ;Crosshatch-16:9
  MOV  R3, #HIGH( G6_F1L22)  ;+6/+10
  MOV  R4, #LOW( G6_F1L22)

  JBC  NewPattern, NewPattern51_G
  LJMP  FieldUpdIntrEnd

NewPattern51_G:
  MOV  PatternPtr, #HIGH( CrossPatt16x9G)
  MOV  PatternPtr+1, #LOW( CrossPatt16x9G)
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern52_G:        ;WhiteCircle-4:3
  MOV  R3, #HIGH( G14_F1L22)  ;+/+
  MOV  R4, #LOW( G14_F1L22)

  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern53_G:        ;WhiteCircle-16:9
  MOV  R3, #HIGH( G15_F1L22)  ;+/+
  MOV  R4, #LOW( G15_F1L22)

  LJMP  FieldUpdIntrEnd

;/******** Cahnged here to have full screen with Window10  ******************/
Pattern54_G:                 ;Window 10%      ;/****** Window10 *************/
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+10 ;
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern54_G
  LJMP  FieldUpdIntrEnd

NewPattern54_G:
  MOV  R6, #Window10_G                      ;/* Window10 uses Window10_G */
  LJMP  FieldUpdIntrEnd

;**************************************************************************
Pattern55_G:        ;15kHz Bl/Wh
  MOV  R3, #HIGH( G1_F1L22)  ;+6/+7
  MOV  R4, #LOW( G1_F1L22)

  JBC  NewPattern, NewPattern55_G
  LJMP  FieldUpdIntrEnd

NewPattern55_G:
  MOV  R6, #kHz15BlWh_g
  LJMP  FieldUpdIntrEnd

;**************************************************************************
FieldUpdIntrEnd:
  MOV  A, R7      ;+1
  POP  DPS      ;+2
  POP  DPH      ;+2
  POP  DPL      ;+2
  POP  PSW      ;+2
  RETI        ;+2
END
