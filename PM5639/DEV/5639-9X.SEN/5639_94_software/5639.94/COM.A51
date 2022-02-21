NAME COMMUNICATION

;************************************************************************
;* Filename  : COM.A51            *
;* Function  : Communication subroutines.        *
;* Author  : St.A.A/K.Engedahl          *
;* Latest revision:  94-12-06          *
;************************************************************************

;************************************************************************
;* 941206:                *
;* No updates                *
;* 940906:                *
;* Added BARCO output format.            *
;* 940615:                *
;* Added in VALUE_OUT:              *
;*  routine has been optimized regarding to SPEED, which means the  *
;*  code is a bit longer than necessary. ALL references to IO_BUFFER  *
;*  and IO_BUFFER+1 have been removed, (they are simply NOT used).  *
;************************************************************************


$INCLUDE ( REG51.INC)
$INCLUDE ( DEFIRAM.ICL)
$INCLUDE ( PORT_DEF.ICL)

COMMUNICATION  SEGMENT CODE

PUBLIC  VALUE_OUT, BYTE_OUT
PUBLIC  STRXAD, STTXAD, TXBYTE, RXFBYT, RXLBYT, STOP
PUBLIC  WRITC, OUT_STRING, COMMA_OUT, CR_OUT, MINUS_OUT, ASCII_SEND

EXTRN  CODE( ?C_CASTF)

  RSEG  COMMUNICATION

;******************************************************************************
VALUE_OUT:

;  DECI_OUT = '1' means PHILIPS floating point output:
;  MODE_BARCO = '1' AND DECI_OUT = '1' means BARCO, (ie. THOMA) 
;  floating output
;
;  This procedure converts a floating point ( 0 <= value < 2^32 ) to 
;  ascii-string with 12 digits.
;  During value_out only IO_BUFFER+2 -> +5 are used
;
;  Input is  R4 (MSB), R5, R6, R7 (LSB) which is in floating point
;  Original value has to be multiplied with 100 to get one/two decimals.
;
;  IO_BUFFER+2   +3   +4   +5
;            9   99   99   ab
;  In this NEW optimized version, (940615), ALL references to IO_BUFFER
;  and IO_BUFFER+1 are REMOVED.
;
;  if ( value < 99999aa)   OUTPUT FORMAT = 99999
;  if ( value <  9999aa)   OUTPUT FORMAT = 9999.a
;  if ( value <   999aa)   OUTPUT FORMAT = 999.ab
;  if ( value <    99aa)   OUTPUT FORMAT = 099.ab
;  if ( value <     9aa)   OUTPUT FORMAT = 009.ab
;  if ( value <      aa)   OUTPUT FORMAT = 000.ab
;  if ( value <       a)   OUTPUT FORMAT = 000.0b
;
;
;  For PHILIPS output format:
;  if ( BUFFER+2 != 0)
;     /* No decimals */
;  else {
;     /* Output one decimal */
;     if ( HIGH( BUFFER+3) != 0)
;        ;
;     else {
;        /* Output two decimals */
;  }
;
;  For BARCO/THOMA output-format:
;  if ((BUFFER+2 != 0 ) || (HIGH(BUFFER+3) != 0) || (LOW(BUFFER+3) > 5))
;     /* output overflow, ie. RGB*   0 *   0 *   0 *<CR><LF> */
;  else {
;     if ( LOW(BUFFER+3) != 0) {
;        /* output;  RGB*xx.xx*yy.yy*zz.zz*<CR><LF> */
;        /* where xxxx = 999a */
;     }
;     else {
;        /* output;  RGB*xxxx *yyyy *zzzz *<CR><LF> */
;        /* where xxxx = 99aa */
;     }
;  }
;------------------------------------------------------------------------------
;
;  DECI_OUT = '0' means binary output
;
;  This procedure converts a unsigned long ( 0 <= value < 2^32 ) to ascii-
;  string with 12 digits.
;
;  Input is  R4 (MSB), R5, R6, R7 (LSB) which is in binary format
;  DECIMAL1_BIT = 1: Shows one decimal
;  DECIMAL2_BIT = 1: Shows two decimals
;
;  Generel:
;  R0, R1 are destroyed
;  IO_BUFFER+2 -> IO_BUFFER+5 are destroyed

  JNB  DECI_OUT, DONT_CAST_OUTPUT

  LCALL  ?C_CASTF

DONT_CAST_OUTPUT:
  CLR  A               ;Clear output area

  MOV  IO_BUFFER+2, A  ;Clearing the IO_BUFFER this way takes LESS
  MOV  IO_BUFFER+3, A  ; TIME, than making an indexed register clear.
  MOV  IO_BUFFER+4, A
  MOV  IO_BUFFER+5, A

  MOV  R0, #32
BIN_LOOP:
  MOV  A, R7    ;Shift input left to get Most Significant
  RLC  A    ; Bit into carry
  MOV  R7, A

  MOV  A, R6
  RLC  A
  MOV  R6, A

  MOV  A, R5
  RLC  A
  MOV  R5, A

  MOV  A, R4
  RLC  A
  MOV  R4, A

NEXT_ADJUST:
  MOV  A, IO_BUFFER+5
  ADDC  A, IO_BUFFER+5
  DA  A
  MOV  IO_BUFFER+5, A

  MOV  A, IO_BUFFER+4
  ADDC  A, IO_BUFFER+4
  DA  A
  MOV  IO_BUFFER+4, A

  MOV  A, IO_BUFFER+3
  ADDC  A, IO_BUFFER+3
  DA  A
  MOV  IO_BUFFER+3, A

  MOV  A, IO_BUFFER+2
  ADDC  A, IO_BUFFER+2
  DA  A
  MOV  IO_BUFFER+2, A

  DJNZ  R0, BIN_LOOP    ;Loop for 32 bits

  JB  DECI_OUT, PHILIPS_OR_BARCO
  JMP  BYTE_OUT

PHILIPS_OR_BARCO:

; Output format is PHILIPS or BARCO/THOMA

  CLR  DECIMAL1_BIT
  CLR  DECIMAL2_BIT    ;No decimals

  JB  MODE_BARCO, OUTPUT_BARCO_FORMAT

  JMP  OUTPUT_PHILIPS_FORMAT

OUTPUT_BARCO_FORMAT:

  MOV  A, workR2
  CJNE  A, #D1_VALUE, START_SEQ  ;If new measurement

  MOV  A, #'R'      ;  start by transmitting: RGB*
  LCALL  WRITC

  MOV  A, #'G'
  LCALL  WRITC

  MOV  A, #'B'
  LCALL  WRITC

  LCALL  ASTERIX_OUT  

START_SEQ:
  JB  SENSOR_OVERFLOW, DISPLAY_OVERFLOW

  MOV  A, IO_BUFFER+3
  ANL  A, #11110000B
  ADD  A, IO_BUFFER+2

  JNZ  DISPLAY_OVERFLOW

  MOV  A, IO_BUFFER+3
  ANL  A, #00001111B
  SUBB  A, #05
  JNC  DISPLAY_OVERFLOW

  SJMP  START_SEQ1

DISPLAY_OVERFLOW:
  MOV  A, #00
  MOV  IO_BUFFER+3, A
  MOV  IO_BUFFER+4, A
  MOV  IO_BUFFER+5, A

START_SEQ1:
  SETB  DECIMAL2_BIT    ;Assume value >= 99.99, ie.
          ; output decimal-point.

  MOV  A, IO_BUFFER+3    ;If LOW( BUFFER+3) != 0
  ANL  A, #00001111B    ; value >= 99.99
  JNZ  START_SEQ2    ;else

  ADD  A, #00AH    ; convert '0' (ZERO) to ' ' (space)
  ORL  IO_BUFFER+3, A

  CLR  DECIMAL2_BIT    ;Reset decimal-point, ie. don't display

  MOV  A, IO_BUFFER+4    ;If ( HIGH( BUFFER+4) == 0)
  ANL  A, #11110000B    ; convert '0' (ZERO) to ' ' (space)
  JNZ  START_SEQ2

  ADD  A, #0A0H
  ORL  IO_BUFFER+4, A

  MOV  A, IO_BUFFER+4    ;If ( LOW( BUFFER+4) == 0)
  ANL  A, #00001111B    ; convert '0' (ZERO) to ' ' (space)
  JNZ  START_SEQ2

  ADD  A, #00AH
  ORL  IO_BUFFER+4, A

  MOV  A, IO_BUFFER+5    ;If ( HIGH( BUFFER+5) == 0)
  ANL  A, #11110000B    ; convert '0' (ZERO) to ' ' (space)
  JNZ  START_SEQ2

  ADD  A, #0A0H
  ORL  IO_BUFFER+5, A

START_SEQ2:
  JNB  DECIMAL2_BIT, BARCO_NXT1

  MOV  A, IO_BUFFER+3    ;ACC eq. LOW( BUFFER+3)
  ANL  A, #00001111B
  LCALL  ASCII_SEND    ;Output LOW( BUFFER+3)

BARCO_NXT1:
  MOV  A, IO_BUFFER+4    ;ACC eq. HIGH( BUFFER+4)
  SWAP  A
  ANL  A, #00001111B
  LCALL  ASCII_SEND    ;Output HIGH( BUFFER+4)

  JNB  DECIMAL2_BIT, BARCO_NXT2
  LCALL  DOT_OUT

BARCO_NXT2:
  MOV  A, IO_BUFFER+4    ;ACC eq. LOW( BUFFER+4)
  ANL  A, #00001111B
  LCALL  ASCII_SEND    ;Output LOW( BUFFER+4)

  MOV  A, IO_BUFFER+5    ;ACC eq. HIGH( BUFFER+5)
  SWAP  A
  ANL  A, #00001111B
  LCALL  ASCII_SEND    ;Output HIGH( BUFFER+5)

  JB  DECIMAL2_BIT, BARCO_NXT3  ;If value < 100
            ; output LOW( BUFFER+5)

  MOV  A, IO_BUFFER+5    ;ACC eq. LOW( BUFFER+5)
  ANL  A, #00001111B
  LCALL  ASCII_SEND    ;Output LOW( BUFFER+5)

  MOV  A, #' ';    ;Output SPACE.
  LCALL  WRITC

BARCO_NXT3:
  JB  COMMA_BIT, BARCO_NXT4  ;If last value
  LCALL  ASTERIX_OUT    ; output an ASTERIX, <CR> and <LF>.

  LCALL  CR_OUT

  MOV  A, #0AH
  LCALL  WRITC

  JMP  CR_OUT

BARCO_NXT4:        ;else
  JMP  ASTERIX_OUT    ; output an ASTERIX.

;******************************************************************************
OUTPUT_PHILIPS_FORMAT:
  MOV  A, IO_BUFFER+2
  ANL  A, #00001111B
  JZ  SET_ONE_DECIMAL

  LCALL  ASCII_SEND    ;Send LOW( IO_BUFFER+2)
  JMP  NEXT_DIGIT

SET_ONE_DECIMAL:
  SETB  DECIMAL1_BIT

NEXT_DIGIT:
  MOV  A, IO_BUFFER+3
  SWAP  A
  ANL  A, #00001111B

  JNZ  NEXT_DIGIT_1
  JNB  DECIMAL1_BIT, NEXT_DIGIT_1

  CLR  DECIMAL1_BIT
  SETB  DECIMAL2_BIT

  JMP  NEXT_DIGIT_2

NEXT_DIGIT_1:
  LCALL  ASCII_SEND    ;Send HIGH( IO_BUFFER+3)

NEXT_DIGIT_2:
  MOV  A, IO_BUFFER+3
  LCALL  ASCII_SEND    ;Send LOW( IO_BUFFER+3)  

  MOV  A, IO_BUFFER+4
  SWAP  A
  LCALL  ASCII_SEND    ;Send HIGH( IO_BUFFER+4)

  MOV  A, IO_BUFFER+4
  LCALL  ASCII_SEND    ;Send LOW( IO_BUFFER+4)

  MOV  C, DECIMAL1_BIT
  ORL  C, DECIMAL2_BIT
  JNC  NO_MORE_DIGITS

  LCALL  DOT_OUT      ;Show decimal point

  MOV  A, IO_BUFFER+5
  SWAP  A
  LCALL  ASCII_SEND    ;Send HIGH( IO_BUFFER+5)

  JNB  DECIMAL2_BIT, NO_MORE_DIGITS

  MOV  A, IO_BUFFER+5
  LCALL  ASCII_SEND    ;Send LOW( IO_BUFFER+5)

NO_MORE_DIGITS:
  JNB  COMMA_BIT, END_VALUE_OUT

  JMP  COMMA_OUT

END_VALUE_OUT:
  JMP  CR_OUT

;******************************************************************************
BYTE_OUT:
  MOV  A, IO_BUFFER+4
  LCALL  ASCII_SEND    ;Send LOW( IO_BUFFER+4)

  JNB  DECIMAL2_BIT, NOT_TWO_DECIMALS
  LCALL  DOT_OUT      ;Show decimal point

NOT_TWO_DECIMALS:
  MOV  A, IO_BUFFER+5
  SWAP  A
  LCALL  ASCII_SEND    ;Send HIGH( IO_BUFFER+5)

  JNB  DECIMAL1_BIT, NOT_ONE_DECIMAL
  LCALL  DOT_OUT      ;Show decimal point

NOT_ONE_DECIMAL:
  MOV  A, IO_BUFFER+5
  LCALL  ASCII_SEND    ;Send LOW( IO_BUFFER+5)

  JMP  NO_MORE_DIGITS

;******************************************************************************
IIC_ROUTINES:
;  Time used : Max 88 usec
;  txbyte -5 usec = 83
;  Stop uses 8 usec.


STRXAD:  CLR  SDA_PIN    ;(1)
  SETB  ACC.0    ;(1)
  SJMP  TXSTAD    ;(2)

STTXAD:  CLR  SDA_PIN    ;(1)
  CLR  ACC.0    ;(1)

TXSTAD:  CLR  SCL_PIN    ;(1)

TXBYTE:  CLR  F0    ;(1)

BYTE:  MOV  R5, #8    ;(1)
  RLC  A    ;(1)  usec before loop
LOOP:  JB  F0, FORBI_1  ;(2)
  MOV  SDA_PIN, C  ;(2)

FORBI_1:
  SETB  SCL_PIN    ;(1)
  JNB  SCL_PIN, $  ;(2) (++)
  JNB     F0, FORBI_2  ;(2)
  MOV  C, SDA_PIN  ;(1)
FORBI_2:
  RLC  A    ;(1)
  CLR  SCL_PIN    ;(1)
  DJNZ  R5, LOOP  ;(2) (8 *  usec in loop)
  JB  F0, FORBI_3  ;(2)
  SETB  C    ;(1)
FORBI_3:
  MOV  SDA_PIN, C  ;(2)
  NOP      ;(1)
  SETB  SCL_PIN    ;(1)
  JNB  SCL_PIN, $  ;(2) (++)
  MOV  C, SDA_PIN  ;(1)
  CLR  SCL_PIN    ;(1)
  SETB  SDA_PIN    ;(1)
  RET      ;(2) (9 usec after loop)

RXLBYT:  SETB  C    ;(1)
  SETB  F0    ;(1)
  JMP  BYTE    ;(2)

RXFBYT:  CLR  C    ;(1)
  SETB  F0    ;(1)
  SJMP  BYTE    ;(2)

STOP:  CLR  SDA_PIN    ;(1)
  SETB  SCL_PIN    ;(1)
  JNB  SCL_PIN, $  ;(2) (++)
  NOP      ;(1)
  SETB  SDA_PIN    ;(1)
  RET                  ;(2) (stop uses 8 usec)

;******************************************************************************
OUT_STRING:
  MOV  A, @R1    ;R1 pointing to string, R3 is number of chars

  LCALL  WRITC
  INC  R1
  DJNZ  R3, OUT_STRING

  JB  COMMA_BIT, COMMA_OUT

CR_OUT:
  MOV  A, #0DH
  JMP  WRITC  

DOT_OUT:
  MOV  A, #'.'
  JMP  WRITC  

MINUS_OUT:
  MOV  A, #'-'
  JMP  WRITC

COMMA_OUT:
  MOV  A, #','
  JMP  WRITC

ASTERIX_OUT:
  MOV  A, #'*'
  JMP  WRITC

ASCII_SEND:      ;Send the ASCII-character for numbers 0-9
  ANL  A, #00001111B  ; OR space if ACC=00AH
  ADD  A, #30H

  CJNE  A, #3AH,WRITC
  MOV  A, #' '
WRITC:
  JB  BUSY, $
  SETB  BUSY
  MOV  SBUF, A
  RET
END
