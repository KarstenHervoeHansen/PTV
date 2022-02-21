NAME  PM39MAIN

;************************************************************************
;* Filename    : PM39MAIN.A51            *
;* Function    : Main program for PM5639 Measure Head      *
;* Author      : St.A.A/K.Engedahl          *
;* Latest revision:  97-09-05          *
;* Related files: ADC.A51, CALCULAT.A51, COM_EXEC.A51      *
;*      COM.A51, DEFIRAM.ICL, PORT_DEF.ICL      *
;*                  *
;*    Min  Typ  Max (cycles)        *
;* ?C_FPADD  100  220  285          *
;* ?C_FPSUB  100  220  205          *
;* ?C_FPMUL  352  375  384          *
;* ?C_FPDIV  1020  1150  1210          *
;* ?C_CASTF                *
;* ?C_FCASTI                *
;* ?C_LPULL                *
;* ?C_LPUSH                *
;************************************************************************

;************************************************************************
;* 970905:                *
;* Bug in reading the baudrate information, (SMALL SENSOR ONLY)    *
;* 970617:                *
;* A new sensor type SMALL_HEAD has been introduced.      *
;* 940906:                *
;* Timer constants corrected to new crystal of 11.0592MHz.    *
;* Reference to bit GAIN_UNSTABLE has been removed,       *
;*  (eq. ANALOG_CONTROL bit 6).            *
;* Added in the initialization routine:          *
;*  Sensor starts measuring IMMEDIATELY outputting data in BARCO/THOMA  *
;*   format.                *
;* Changed in OVERFLOW_OUTPUT:            *
;*  Minus is not transmitted in BARCO/THOMA output-format.    *
;* 940628:                *
;* Moved hardware gain adjustement to ADC.A51        *
;* 940615:                *
;* Added in the initialization routine:          *
;* Three new constants for 4800, 9600 and 19200 baud at 11,0592MHz  *
;*  ie. 0F4H->(4800baud), 0FAH->(9600baud) and 0FDH->(19200baud)  *
;* The two jumpers, P0.5,P0.6 are scanned according to:      *
;*   baud-rate     P0.6  P0.5            *
;*    4800          0     0            *
;*    9600          0     1            *
;*   19200          1     0            *
;*    4800          1     1  ( just in case anything screws up).  *
;*                  *
;* Added in initialization routine:          *
;* Reading information in EýPROM about whether to transmit the    *
;*  integration constant or not, (at this point in time this    *
;*  function is ONLY used in connection with PM5639/914 PC Software  *
;*  for Brgge).              *
;* Added in SENSOR_OUTPUT:            *
;*  In XYZ-mode the integration time constant is NEVER sent. In MX-mode  *
;*  it is the content of the bit TRX_INT_CONST, (which is reset/set  *
;*  according to the content of the EýPROM address 95), which decides  *
;*  whether to sent this constant or not.        *
;************************************************************************


$INCLUDE ( REG51.INC)
$INCLUDE ( DEFIRAM.ICL)    ;Defines memory layout (Bytes and Bits)
$INCLUDE ( PORT_DEF.ICL)  ;Defines Port-locations and constants

PM39MAIN SEGMENT CODE

PUBLIC  BEGIN, RS232_INTERRUPT, COMMA_BIT, BUSY, MAINLOOP, OUTPUT_RESULT

EXTRN  CODE( MEASURE, INTEGRATION_TIME_OUT, LEARN_OFFSET)
EXTRN  CODE( VALUE_OUT, CHAR_HANDLER, MINUS_OUT)
EXTRN  CODE( STORE_FLOAT, RECALL_FLOAT, EE_TO_RS232)

EXTRN   CODE( ?C_FPMUL, ?C_FCASTI, ?C_LPUSH, ?C_FPADD)
EXTRN  CODE( ?C_FPSUB, ?C_CASTF, ?C_LPULL, ?C_FPDIV)

TIMER_10MS  EQU  02400H  ;According to crystal val. 11.0592MHz
TIMER_40MS  EQU  09000H

  CSEG

  ORG  00H  ;Reset vector
  JMP  BEGIN

  ORG  03H  ;Interrupt from External Interrupt 0
  RETI

  ORG  0BH  ;Interrupt from Timer 0 Overflow
  RETI

  ORG  13H  ;Interrupt from External Interrupt 1
  RETI

  ORG  1BH  ;Interrupt from Timer 1 Overflow
  RETI

  ORG  23H  ;Interrupt from RI+TI
  JMP  RS232_INTERRUPT

  RSEG  PM39MAIN

BEGIN:
  MOV  SP, #STACK_START-1  ;Main-program starts at 01DH (Hex)
  CLR  A      ; (setup during Linking).
  MOV  R1, #8

ZERO_LOOP:
  MOV  @R1, A
  INC  R1
  CJNE  R1, #STACK_START, ZERO_LOOP

;Variables below are cleared by ZERO_LOOP

;  MOV  D1_GAIN, A
;  MOV  D2_GAIN, A
;  MOV  D3_GAIN, A
;  MOV  IP, A    ;No priorities among interrupts

  MOV   P1, A    ;Zero integrate, select RED channel, RED-gain 0
  MOV   P2, A    ;GREEN- & BLUE-gain 0
  
  MOV   A, P0
  ANL   A, #060H
  CJNE  A, #020H, TEST_19200_BAUD  ;Test if 9600 baud

  MOV   TH1, #BAUD_9600
  SJMP  END_OF_BAUD_RATE_SETTING

TEST_19200_BAUD:  
  CJNE  A, #040H, SET_4800_BAUD    ;Test if 19200 baud

  MOV   TH1, #BAUD_19200
  SJMP  END_OF_BAUD_RATE_SETTING

SET_4800_BAUD:  
  MOV   TH1, #BAUD_4800

END_OF_BAUD_RATE_SETTING:
  ORL  PCON, #080H
  MOV  SCON, #0D8H
  MOV  TMOD, #021H
  MOV  TCON, #055H

  MOV  INT_TIME, #DEF_INT_TIME  ;Default to 50.0 ms integration-time

;************************************************************************

INIT_PM5639:
  SETB  EE_W_CONTROL    ;Read # bytes from EýPROM
  CLR  EE_TO_RS    ;Do NOT send to RS232

  MOV  NUM_PAGES, #00000110B

GET_SENSOR_VERSION:
  MOV  R2, #1      ;Get version of sensor
  MOV  A, #INTERN_ID_EEPROM
  MOV  R1, #TEMP_VALUE
  LCALL  EE_TO_RS232

;  if (( temp_value >= 0) && ( temp_value < 16))
;     SENSOR_VERSION = CRT_head;
;  else if (( temp_value >= 16) && ( temp_value < 32))
;     SENSOR_VERSION = PROJ_head;
;  else {
;     if (( temp_value >= 32) && ( temp_value < 56))
;        SENSOR_VERSION = INDUSTRIAL_head;
;     else
;        SENSOR_VERSION = SMALL_head;
;  }

  MOV  SENSOR_VERSION, #CRT_HEAD

  MOV  A, TEMP_VALUE
  CLR  C
  SUBB  A, #10H
  JC  GET_PROTECT_INFO

  MOV  SENSOR_VERSION, #PROJ_HEAD
  SUBB  A, #10H
  JC  GET_PROTECT_INFO

  MOV  SENSOR_VERSION, #INDUSTRIAL_HEAD
  SUBB  A, #18H
  JC  GET_PROTECT_INFO

  MOV  SENSOR_VERSION, #SMALL_HEAD

  MOV  R2, #1      ;Get information about the baudrate
  MOV  A, #BAUDRATE_INFO
  MOV  R1, #TEMP_VALUE
  LCALL  EE_TO_RS232

;  if (( INFO <> BAUD_9600) && ( INFO <> BAUD_9600))
;     set baudrate = BAUD_4800

  MOV  TH1, TEMP_VALUE

  MOV  A, TEMP_VALUE
  SUBB  A, #Baud_9600
  JZ  GET_PROTECT_INFO

  MOV  A, TEMP_VALUE
  SUBB  A, #Baud_19200
  JZ  GET_PROTECT_INFO

  MOV  TH1, #Baud_4800

GET_PROTECT_INFO:
  MOV  IE, #10010000B    ;Enable General- and serial-interrupt

  MOV  R2, #02      ;Info of write protected area
  MOV  A, #WR_PROTECT_ADR  ; 0-WR_PROTECT_ADR
  MOV  R1, #TEMP_VALUE    ; read from EýPROM location
  LCALL  EE_TO_RS232    ; WR_PROTECT_INFO (LOW), +1 (HIGH)

  MOV  WR_PROTECT_LOW, TEMP_VALUE
  MOV  WR_PROTECT_HIGH, TEMP_VALUE+1

  SETB  EEPROM_PROTECT    ;Disable writing in protected area

  MOV  R2, #02      ;Info of write protection ON/OFF
  MOV  A, #WR_PROTECT_INFO  
  MOV  R1, #TEMP_VALUE    ; read from EýPROM location
  LCALL  EE_TO_RS232    ; WR_PROTECT_INFO (LOW), +1 (HIGH)

  MOV  A, TEMP_VALUE    ;WR_PROTECT_INFO +0, +1 MUST contain
  ANL  A, TEMP_VALUE+1    ; 0FFFFH to initialize writing in
  CJNE  A, #0FFH, INIT_CONTINUE  ; protected area

  CLR  EEPROM_PROTECT    ;Enable writing in protected area

INIT_CONTINUE:
  MOV  R2, #18      ;Get Correction matrix
  MOV  A, #K_MATRIX_EEPROM
  MOV  R1, #K_MATRIX_RAM
  LCALL  EE_TO_RS232 

  MOV  R2, #1      ;Get information about transmitting the
  MOV  A, #TRX_INT_CONST_ADR  ; integration constant.
  MOV  R1, #TEMP_VALUE
  LCALL  EE_TO_RS232

  SETB  TRX_INT_CONST
  MOV  A, TEMP_VALUE
  CJNE  A, #000H, SET_OUTPUT_MODE ;If (data[TRX_INT_CONST_ADR] == 0)

  CLR  TRX_INT_CONST    ; do NOT transmit the integration
          ; constant
SET_OUTPUT_MODE:
  SETB  MODE_XY      ;Start-up in CIE XYZ output-mode
  SETB  MODE_BARCO    ;Start-up in BARCO/THOMA output-format
  SETB  MODE_MC

  JMP  START_POINT

;************************************************************************

DUMMY_INT:
  RETI

RS232_INTERRUPT:
  CLR  BUSY

  JB  RI, RECEIVER_INTERRUPT
  CLR  TI
  RETI

RECEIVER_INTERRUPT:
  CLR  RI

  ANL  P1, #03FH    ;Zero integrate

  MOV  ?C_DSTKLEVEL, #00  ;Special floating-point clear

  MOV  SP, #STACK_START-1
  LCALL  DUMMY_INT

  MOV  R2, SBUF
        LCALL  CHAR_HANDLER  ;Perform any action caused by received char.

;************************************************************************

START_POINT:  

  CLR  OVERFLOW_ERROR
  CLR  COMMA_BIT

  CPL  WD_TRIG_PIN
  JB  MODE_RX_ACTIVE, START_POINT

;************************************************************************

;Force AD-converter to well-defined state

  ANL  P1, #03CH  ;Select RED-chan. Zero integrate

  MOV  A, TMOD
  ANL  A, #11110111B
  MOV  TMOD, A

  MOV  TH0, #HIGH( TIMER_10MS)
  MOV  TL0, #LOW( TIMER_10MS)

  CLR  TF_0
  SETB  TR_0

  JNB  TF_0, $    ;Zero integrate for app. 5 ms

  ORL  P1, #080H  ;Set Auto-zero,  (B=1, A=0)

  MOV  TL0, #LOW( TIMER_40MS)
  MOV  TH0, #HIGH( TIMER_40MS)

  CLR  TF_0
  JNB  TF_0, $    ;Auto Zero for app. 50 ms

  CLR  TR_0
  CLR  TF_0

;************************************************************************

MAINLOOP:
  CLR  SENSOR_OVERFLOW

  JB  MODE_FG, CLEAR_FAST_GAIN

  MOV  A, GAIN_BYTE
  ANL  A, #00001110B
  JNZ  SOME_GAIN_UNSTABLE

CLEAR_FAST_GAIN:
  CLR  FAST_GAIN      ;All channel-gains are stable

SOME_GAIN_UNSTABLE:  
  ANL  GAIN_BYTE, #11110001B  ;Clear RED-, GREEN- & BLUE-unstable

  ANL  ANALOG_CONTROL+1, #00111000B
  ORL  ANALOG_CONTROL+1, #01000000B  ;Select RED channel
            ;Set bits for integrate
   MOV  ACTUAL_SENSOR, #D1_VALUE
  LCALL  MEASURE

MEAS_GREEN:
  ANL  ANALOG_CONTROL+1, #00111000B
  ORL  ANALOG_CONTROL+1, #01000001B  ;Select GREEN channel
            ;Set bits for integrate
  MOV  ACTUAL_SENSOR, #D2_VALUE
  LCALL  MEASURE  

MEAS_BLUE:
  ANL  ANALOG_CONTROL+1, #00111000B
  ORL  ANALOG_CONTROL+1, #01000010B  ;Select BLUE channel
            ;Set bits for integrate
  MOV  ACTUAL_SENSOR, #D3_VALUE
  LCALL  MEASURE  

MEAS_END:
  MOV  A, GAIN_BYTE
  ANL  A, #00001110B
  JNZ  MAINLOOP

  JB  FAST_GAIN, MAINLOOP

NOT_ERROR:
  JNB  MODE_MO, NO_OFFSET
  JMP  LEARN_OFFSET

MAINLOOP_TEMP:
  JMP  MAINLOOP

NO_OFFSET:
  JB  MODE_MC, OUTPUT_RESULT
  JNB  MODE_TM, MAINLOOP_TEMP

;************************************************************************

OUTPUT_RESULT:
  CLR  MODE_TM
  SETB  COMMA_BIT
  SETB  DECI_OUT

  MOV  workR2, #D1_VALUE

  JB  SENSOR_OVERFLOW, OVERFLOW_OUTPUT

  JB  MODE_XY, OUTPUT_XYZ
  JMP  SENSOR_OUTPUT

OVERFLOW_OUTPUT:
  ;If BARCO/THOMA output-format:
  ; Do not output minus if overflow, the routine value_out, (COM.A51),
  ;  will handle this condition.

  JB  MODE_BARCO, NO_MINUS  
  LCALL  MINUS_OUT

NO_MINUS:
  MOV  D1_VALUE+3, #3FH
  MOV  D1_VALUE+2, #80H
  CLR  A
  MOV  D1_VALUE+1, A
  MOV  D1_VALUE, A    ;D1_VALUE = 1.00 -> -1.00

  JMP  SENSOR_OUTPUT

;************************************************************************
;  CIE 1931 XYZ-coordinates

;  X = ( D1_VALUE*K_A + D2_VALUE*K_B + D3_VALUE*K_C)*1E-5*200/INT_TIME;
;  Y = ( D1_VALUE*K_D + D2_VALUE*K_E + D3_VALUE*K_F)*1E-5*200/INT_TIME;
;  Z = ( D1_VALUE*K_G + D2_VALUE*K_H + D3_VALUE*K_J)*1E-5*200/INT_TIME;

;  NB !!! 200/INT_TIME is to compensate for different integration times.

OUTPUT_XYZ:
  MOV  workR0, #K_MATRIX_RAM-2

NEXT_ADDEND:
  MOV  workR1, #D1_VALUE
  LCALL  MUL_ELEMENT

  LCALL  ?C_LPUSH

  MOV  workR1, #D2_VALUE
  LCALL  MUL_ELEMENT

  LCALL  ?C_FPADD

  LCALL  ?C_LPUSH

  MOV  workR1, #D3_VALUE
  LCALL  MUL_ELEMENT

  LCALL  ?C_FPADD

  LCALL  ?C_LPUSH

  MOV  dirR7, #040H
  MOV  dirR6, #000H
  MOV  dirR5, #000H
  MOV  dirR4, #000H

  LCALL  ?C_FPMUL    ;Multiply with 200*1E-4*100;
                     ;For output multiply with 100
  LCALL  ?C_LPUSH

  MOV  R4, #00
  MOV  R5, INT_TIME
  CLR  A
  LCALL  ?C_FCASTI

  LCALL  ?C_FPDIV    ;Divide INT_TIME ( integration time)

  LCALL  VALUE_OUT

  MOV  A, workR2
  ADD  A, #05
  MOV  workR2, A

  CLR  C
  SUBB  A, #D2_VALUE
  JZ  DONT_CLEAR_COMMA

  CLR  COMMA_BIT

DONT_CLEAR_COMMA:

  MOV  A, workR2
  CJNE  A, #D3_VALUE+5, NEXT_ADDEND

  JMP  MAINLOOP

;************************************************************************
MUL_ELEMENT:
;  This routine multiplies the sensors with the Correction-matrix.

  MOV  R0, workR0
  INC  R0
  INC  R0
  MOV  workR0, R0

  MOV  dirR4, @R0    ;K_ELEMENT
  INC  R0
  MOV  dirR5, @R0

  MOV  A, dirR4
  LCALL  ?C_FCASTI    ;signed int K_ELEMENT

  LCALL  ?C_LPUSH

  MOV  R1, workR1
  LCALL  RECALL_FLOAT

  LCALL  ?C_FPMUL    ;( float) Dx_VALUE*K_ELEMENT

  RET

;************************************************************************

SENSOR_OUTPUT:

;  This routine output the sensor-readings as the direct readings 
;  from the sensors only compensated by gain_factors/offsets.

  MOV  R1, workR2
  LCALL  RECALL_FLOAT

  LCALL  ?C_LPUSH

  MOV  dirR7, #042H
  MOV  dirR6, #0C8H
  CLR  A
  MOV  dirR5, A
  MOV  dirR4, A
  LCALL  ?C_FPMUL    ;Multiply with 100 to get decimals

  LCALL  VALUE_OUT

  MOV  A, workR2
  ADD  A, #05
  MOV  workR2, A

  CJNE  A, #D3_VALUE, SENSOR_OUTPUT_1

  JB  MODE_XY, CLEAR_COMMA_BIT
  JB  TRX_INT_CONST, SENSOR_OUTPUT_1

CLEAR_COMMA_BIT:
  CLR  COMMA_BIT

SENSOR_OUTPUT_1:
  CJNE  A, #D3_VALUE+5, SENSOR_OUTPUT

  JB  MODE_XY, SENSOR_OUTPUT_END
  JNB  TRX_INT_CONST, SENSOR_OUTPUT_END

  LCALL  INTEGRATION_TIME_OUT

SENSOR_OUTPUT_END:
  JMP  MAINLOOP
END
