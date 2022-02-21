NAME CALCULAT

;************************************************************************
;* Filename  : CALCULAT.A51            *
;* Function  : Calculation & communication subroutines.    *
;* Author  : St.A.A/K.Engedahl          *
;* Latest revision:  94-12-06          *
;************************************************************************

;************************************************************************
;* 941206:                *
;* No updates                *
;* 940615:                *
;* No updates                *
;************************************************************************

$INCLUDE (REG51.INC)
$INCLUDE (PORT_DEF.ICL)
$INCLUDE (DEFIRAM.ICL)

CALCULATE SEGMENT CODE

PUBLIC  STORE_FLOAT, RECALL_FLOAT
PUBLIC  EE_TO_RS232, EE_R_W, EE_READ, EE_WRITE, LEARN_OFFSET

EXTRN  CODE( STTXAD, TXBYTE, STRXAD, RXLBYT, STOP)
EXTRN  CODE( WRITC, MAINLOOP, OUTPUT_RESULT)

EXTRN   CODE( ?C_FPMUL, ?C_FPDIV, ?C_FCASTI, ?C_LPUSH, ?C_FPADD)
EXTRN  CODE( ?C_FPSUB, ?C_CASTF, ?C_LPULL)

  RSEG  CALCULATE

;******************************************************************************

EE_READ:

;  Reads ( R2 ) bytes from EýPROM address ( ACC++) to 
;  internal RAM address ( @R1++)

  SETB  EE_W_CONTROL
  SJMP  EE_R_W


;******************************************************************************
EE_WRITE:
 
;  Writes ( R2 ) bytes to EýPROM address ( ACC++) from 
;  internal RAM address ( @R1++)

  CLR  EE_W_CONTROL

;******************************************************************************
EE_R_W:

  CLR  EE_TO_RS

;******************************************************************************
EE_TO_RS232:
  MOV  workR0, A

  JB  EE_W_CONTROL, EE_WRITE1

  JNB  EEPROM_PROTECT, EE_WRITE1

  MOV  A, EEPROM_PAGE
  SUBB  A, WR_PROTECT_HIGH
  JZ  CHECK_LOW_ADR
  JNC  EE_WRITE1

CHECK_LOW_ADR:
  MOV  A, workR0
  CLR  C
  SUBB  A, WR_PROTECT_LOW
  JZ  INC_RET      ; and address is below or equal to
  JNC  EE_WRITE1    ; write protected area then

INC_RET:        ; return without writing
  RET

EE_WRITE1:
  MOV  A, workR0
  MOV  B, A

NO_RESPONSE:
  MOV  R5, #20

STOP_LOOP:
  CLR  SCL_PIN
  LCALL  STOP
  DJNZ  R5, STOP_LOOP

EE_LOOP:
  MOV  A, EEPROM_PAGE  
  RL  A
  ANL  A, NUM_PAGES    ;Find page in EýPROM
  ORL  A, #EEPROM_IIC_ADR  ;IIC address of EýPROM
  LCALL  STTXAD
  JC  NO_RESPONSE
  MOV  A, B
  LCALL  TXBYTE
  JB  EE_W_CONTROL, READING
  MOV  A, @R1
  LCALL  TXBYTE
  JMP  EE_FORBI

READING:
  LCALL  STOP
  MOV  A, EEPROM_PAGE
  ANL  A, NUM_PAGES    ;Find page in EýPROM
  ORL  A, #EEPROM_IIC_ADR  ;IIC address of EýPROM
  LCALL  STRXAD
  LCALL  RXLBYT
  JNB  EE_TO_RS, NORM_EE
  PUSH  B
  LCALL  WRITC
  POP  B
  JMP  EE_FORBI

NORM_EE:
  MOV  @R1, A

EE_FORBI:
  LCALL  STOP
  INC  R1
  MOV  A, B
  ADD  A, #01      ; Increment address
  MOV  B, A
  JNC  EE_FORBI_NEXT    ;If LOW(address) from 0FFH to 00H then
  INC  EEPROM_PAGE    ; increment HIGH(address) "page"
  MOV  A, NUM_PAGES
  ANL  EEPROM_PAGE, A

EE_FORBI_NEXT:
  DJNZ  R2, EE_LOOP
  RET                                        

                                                         

;******************************************************************************
LEARN_OFFSET:

;  Learn voltage offset measured in darkness (no light AT ALL )
;
;  Input   :  Measured voltages in Dx_VALUES
;             Fixed gain in REF_GAIN
;  Output  :  Voltage offset stored in EýPROM

  INC  AVERAGE_COUNT
  MOV  A, AVERAGE_COUNT
  CJNE  A, #NO_OF_AVERAGE, ADD_NEW_VALUES
  JMP  WRITE_OFFSET_TO_EEPROM

ADD_NEW_VALUES:
  CLR  C
  SUBB  A, #NO_OF_STABILIZE
  JNC  ADD_CONTINUE
  JMP  MAINLOOP

ADD_CONTINUE:
  MOV  NEXT_AVERAGE, #D1_AVERAGE
  MOV  NEXT_SENSOR, #D1_VALUE

ADD_CONTINUE_1:
  MOV  R1, NEXT_SENSOR
  LCALL  RECALL_FLOAT

  INC  R1
  INC  R1
  MOV  NEXT_SENSOR, R1

  LCALL  ?C_LPUSH

  MOV  R1, NEXT_AVERAGE
  LCALL  RECALL_FLOAT

  LCALL  ?C_FPADD

  MOV  R1, NEXT_AVERAGE
  LCALL  STORE_FLOAT

  INC  R1
  MOV  NEXT_AVERAGE, R1

  MOV  A, NEXT_AVERAGE
  CLR  C
  SUBB  A, #03CH
  JNZ  ADD_CONTINUE_1

  JMP  MAINLOOP

WRITE_OFFSET_TO_EEPROM:

  MOV  TEMP_VALUE+3, #DIVIDE_CONST_1
  MOV  TEMP_VALUE+2, #DIVIDE_CONST_0
  CLR  A
  MOV  TEMP_VALUE+1, A
  MOV  TEMP_VALUE, A    ;TEMP_VALUE = ( float) NO_OF_AVERAGE;

  MOV  A, REF_GAIN
  RL  A
  ADD  A, #OFFSET_ADR

  MOV  workR1, A    ;Store EýPROM offset address
  MOV  workR3, #03    ;Prepare for three channels

  MOV  R1, #D1_AVERAGE
  MOV  NEXT_SENSOR, #D1_VALUE

DIVIDE_NEXT_CHANNEL:
  LCALL  RECALL_FLOAT

  INC  R1
  MOV  NEXT_AVERAGE, R1

  MOV  A, dirR7
  MOV  C, ACC.7
  MOV  OFFSET_SIGN, C
  CLR  ACC.7
  MOV  dirR7, A

  LCALL  ?C_LPUSH

  MOV  R1, #TEMP_VALUE
  LCALL  RECALL_FLOAT

  LCALL  ?C_FPDIV
  LCALL  ?C_CASTF

  LCALL  ?C_LPUSH

  MOV  A, R6
  MOV  R4, A
  MOV  A, R7
  MOV  R5, A
  MOV  A, R4  
  LCALL  ?C_FCASTI

  MOV  C, OFFSET_SIGN
  JNC  DONT_ADD_32768

  LCALL  ?C_LPUSH

  MOV  dirR7, #47H  ;if offset < 0 add 32768
  CLR  A
  MOV  dirR6, A
  MOV  dirR5, A
  MOV  dirR4, A

  LCALL  ?C_FPADD

DONT_ADD_32768:
  MOV  R1, NEXT_SENSOR
  LCALL  STORE_FLOAT

  MOV  A, NEXT_SENSOR
  ADD  A, #05
  MOV  NEXT_SENSOR, A
  
  LCALL  ?C_LPULL

  MOV  workR5, dirR7    ;LOW 
  MOV  A, dirR6    ;HIGH

  MOV  C, OFFSET_SIGN
  MOV  ACC.7, C
  MOV  workR6, A

  MOV  EEPROM_PAGE, #00

  CLR  EE_W_CONTROL

  MOV  R2, #02H
  MOV  R1, #workR5
  MOV  A, workR1
  LCALL  EE_TO_RS232    ;Store mean value in EýPROM

  MOV  A, workR1
  ADD  A, #0CH
  MOV  workR1, A  

  MOV  R1, NEXT_AVERAGE
  MOV  R2, NEXT_SENSOR

  DJNZ  workR3, DIVIDE_NEXT_CHANNEL

  MOV  A, REF_GAIN
  INC  A
  MOV  REF_GAIN, A

  CJNE  A, #NO_OF_GAIN_CONSTANTS, NEXT_GAIN

  CLR  MODE_MO
  CLR  MODE_FG
  SETB  EE_W_CONTROL

  MOV  R2, #18      ;Reload Correction matrix
  MOV  A, #K_MATRIX_EEPROM
  MOV  R1, #K_MATRIX_RAM
  LCALL  EE_TO_RS232 

  JMP  OUTPUT_RESULT

NEXT_GAIN:
  MOV  R1, #010H    ;Clear working registers
  MOV  R0, #030H

CLEAR_WORK_AREA:
  MOV  @R0, #00H
  INC  R0
  DJNZ  R1, CLEAR_WORK_AREA

  JMP  OUTPUT_RESULT

;******************************************************************************
STORE_FLOAT:
;  Stores floating point value ( R4-R7) in ( @R1++ )

;  R7:  SEEEEEEE    @R1+3
;  R6:  EMMMMMMM    @R1+2
;  R5:  MMMMMMMM    @R1+1
;  R4:  MMMMMMMM    @R1

  MOV  @R1, dirR4
  INC  R1
  MOV  @R1, dirR5
  INC  R1
  MOV  @R1, dirR6
  INC  R1
  MOV  @R1, dirR7
  RET

RECALL_FLOAT:
;  Recalls floating point value from ( @R1++) to ( R4-R7)

  MOV  dirR4, @R1
  INC  R1
  MOV  dirR5, @R1
  INC  R1
  MOV  dirR6, @R1
  INC  R1
  MOV  dirR7, @R1
  RET
END
