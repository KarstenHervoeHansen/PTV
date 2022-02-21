NAME COM_EXEC

;****************************************************************
;* Filename  : COM_EXEC.A51          *
;* Function  : RS232 Command interpreter subroutines.  *
;* Author  : St.A.A/K.Engedahl        *
;* Latest revision:  97-09-05        *
;****************************************************************

;****************************************************************
SOFTWARE_RELEASE  EQU  35    ;VERSION 3.5  *
;****************************************************************

;****************************************************************
;* 970905:              *
;* New version: 3.4. Bug in file: PM39MAIN.A51
;* 970617:              *
;* Added SET_BAUDRATE command. Changed revision to 3.3    *
;* 940906:              *
;* No updates, but changed revision to 3.2      *
;* 940906:              *
;* Added new command: MB for MODE_BARCO        *
;* Added new command: CS for CALCULATE_CHECKSUM      *
;* 940615:              *
;* Added in FIX_GAIN_SELECT:          *
;*   Release of the Fixed Gain Command by writing FG255;  *
;****************************************************************

$INCLUDE ( REG51.INC)
$INCLUDE ( PORT_DEF.ICL)
$INCLUDE ( DEFIRAM.ICL)

;****************************************************************
;Check for valid char
;Valid : a-z : 61h-7Ah
;        A-Z : 41h-5Ah
;        0-9 : 30h-39h
;        '?',',',';',<CR>,<ESC>
;        3Fh,2Ch,3Bh,0Dh ,1Bh
; Alfas (a-z & A-Z) and '?' are stored in
; command-regs. if third alfa then delete command
; Digit is handled (if numerics allowed otherwise delete)
; After deleted commands state is : Waiting for delimiter
; <CR>,',' and ';' are all handled as command delimiter
; <ESC> goes to ready state (deletes and terminates)
;Output  ACC.0 Indicates alfa (a-z,A-Z,?). R2 converted to uppercase
;        ACC.1 Indicates digit
;        ACC.2 Indicates Skip buffer (ESCape)
;        ACC.3 Indicates Delimiter
;****************************************************************

COM_EXEC SEGMENT CODE

PUBLIC  INTEGRATION_TIME_OUT, CHAR_HANDLER

EXTRN CODE( EE_R_W, EE_WRITE, EE_TO_RS232, WRITC, VALUE_OUT, COMMA_OUT, CR_OUT)

  RSEG  COM_EXEC

CHAR_HANDLER:
  CJNE  R2, #'A', $+3
  JC  CHECK_FOR_SINGLES
  ANL  dirR2, #11011111B  ;Clearing bit 5 converts 
          ;lowcase to capital
  MOV  A, R2
  SUBB  A, #'Z'+1
  JNC  IGNORE

ALFA_FOUND:
  MOV  A, #1      ; Code for alfas
  LJMP  CHAR_STATE_MACHINE

CHECK_FOR_SINGLES:
  MOV  A, R2
  CLR  C
  SUBB  A, #0DH
  JZ  CR_FOUND

  SUBB  A, #1BH-0DH
  JZ  RETURN_0    ;ESCape found

  SUBB  A, #2CH-1BH
  JZ  COMMA_FOUND

  SUBB  A, #3BH-2CH
  JZ  SEMICOLON_FOUND

  SUBB  A, #3FH-3BH
  JZ  ALFA_FOUND

FIND_NUMBER:
  MOV  A, R2
  CLR  C
  SUBB  A, #'0'
  JC  IGNORE

  SUBB  A, #'9'-'0'+1
  MOV  A, #2      ;Code for number
  JC  CHAR_STATE_MACHINE
  RET

RETURN_0:
  CLR  MODE_RX_ACTIVE

IGNORE:
  RET

CR_FOUND:
SEMICOLON_FOUND:
COMMA_FOUND:
  MOV  A, #8      ;Code for delimiter

;  Start of state machine for received chars
;  At entry ACC indicates type of char :
;    ACC.0 Indicates alfa (a-z,A-Z,?). R2 converted to uppercase
;    ACC.1 Indicates digit
;    ACC.3 Indicates Delimiter

CHAR_STATE_MACHINE:
  JNB  MODE_RX_ACTIVE, STATE_0
  JB  MODE_RX_2, STATE_4
  JNB  MODE_RX_1, STATE_1

STATE_2:
  JB  ACC.3, RX_EXECUTE
  JB  ACC.0, RX_ERROR    ;No alfa allowed

NUM_HANDLER:        ;Now num : Handle number
  MOV  A, INPUT_NUMBER    ;One byte version
  MOV  B, #10
  MUL  AB
  MOV  INPUT_NUMBER+1, B
  XCH  A, R2
  ANL  A, #00001111B
  ADD  A, R2
  MOV  INPUT_NUMBER, A
  MOV  A,#00
  ADDC  A, INPUT_NUMBER+1
  MOV  INPUT_NUMBER+1, A
  RET

STATE_0:
  JB  ACC.3, STATE_0_END  ;No action if delimiter
  JNB  ACC.0, RX_ERROR    ;If not alfa then DIGIT :Error

STATE_0_ALFA:
        MOV     RX_REG,R2               ;Store char in Rx-register
        SETB    MODE_RX_ACTIVE          ;Change to state1
        CLR     MODE_RX_1
        CLR     MODE_RX_2

STATE_0_END:
        RET

STATE_4:
  JB  ACC.3,RX_Execute  ;Wait for delimiter

RX_ERROR:
  MOV  RX_REG, #0    ;Error : clear command
  SETB  MODE_RX_ACTIVE    ;
  SETB  MODE_RX_1    ; and change to wait-state.
  SETB  MODE_RX_2
  RET

RX_EXECUTE:
  CLR  MODE_RX_ACTIVE
  MOV  DPTR, #COMMAND_TABLE
  MOV  A, RX_REG
  MOV  B, #05
  MUL  AB
;  ANL  A, #01111100B    ;Mask to max 32 entries
  JMP  @A+DPTR

STATE_1:
  JB  ACC.3, RETURN_0
  JNB  ACC.0, RX_ERROR

  MOV  INPUT_NUMBER, #0  ;Init number;2nd alfa
  MOV  INPUT_NUMBER+1, #0
  SETB  MODE_RX_1    ;Change to state 2

COMMAND_CHECK:

;  Init number to NO_NUMBER;
;  Check command;
;  If ( ok)
;     RX_REG := Command number;
;  If ( number allowed for command)
;     change to state 2;
;  else
;     change to state 4;
;  If ( not OK)
;     RX_REG := 0 change to state 4 (jmp rx-error);

  MOV  DPTR, #COMMAND_TABLE-2
  MOV  R1, #0      ;R1 is Command-counter

LOOP:  INC  R1
  MOV  A, R1
  MOV  B, #05
  MUL  AB
  MOVC  A, @A+DPTR
  JZ  RX_ERROR    ;Zero byte is end-of-table indicator
  CJNE  A, RX_REG, LOOP

  MOV  A, R1
  MOV  B, #05
  MUL  AB
  INC  A
  MOVC  A, @A+DPTR
  CJNE  A, dirR2, LOOP

  MOV  RX_REG, R1
  RET

COMMAND_TABLE:
  LJMP  DUMMY_COMMAND  ;Command number 0 is reserved as no-command
  DB  'MS'    ;Command number 1 spelling
  LJMP  MEASURE_STOP  ;Command number 1 Execute-address
  DB  'MC'    ;   -      -    2 spelling etc.
  LJMP  MEASURE_CONTINOUS
  DB  'TM'
  LJMP  TAKE_MEASURE
  DB  'F?'
  LJMP  INTEGRATION_TIME_OUT
  DB  'SM'
  LJMP  EEPROM_WRITE
  DB  'RM'
  LJMP  EEPROM_READ
  DB  'MA'
  LJMP  EEPROM_ADDRESS
  DB      'FG'
  LJMP    FIX_GAIN_SELECT
  DB  'I?'
  LJMP  REQUEST_IDENTITY
  DB  'MO'
  LJMP  MEASURE_OFFSET
  DB  'SO'
  LJMP  SHOW_ONLY_OFFSET
  DB  'ST'
  LJMP  SHOW_ONLY_COUNT
  DB  'NR'
  LJMP  NORMAL_RUN
  DB  'RN'
  LJMP  READ_NAME
  DB  'SI'
  LJMP  SET_INTEGRATION_TIME
  DB  'XY'
  LJMP  SET_XYZ_OUTPUT
  DB  'MX'
  LJMP  CLEAR_XYZ_OUTPUT
  DB  'WR'
  LJMP  WRITE_PROTECTION
  DB  'MB'
  LJMP  SET_BARCO_OUTPUT
  DB  'CS'
  LJMP  CALCULATE_CHECKSUM
  DB  'SB'
  LJMP  SET_BAUDRATE

COMMAND_END:  
  DB  0    ;End indicator

;******************************************************************************

MEASURE_CONTINOUS:
  SETB  MODE_MC

ERASE_STATE:
  CLR  A
  MOV  PROT_STATE, A

;******************************************************************************

DUMMY_COMMAND:
  RET

;******************************************************************************

TAKE_MEASURE:
  SETB  MODE_TM    ;And change to single-cycle
  CLR  MODE_MC
  JMP  ERASE_STATE

;******************************************************************************

MEASURE_STOP:
  CLR  MODE_MC
  MOV  PROT_STATE, #01H
  RET

;******************************************************************************

EEPROM_ADDRESS:
  MOV  EEPROM_ADR, INPUT_NUMBER
  MOV  SM_RM_PAGE, INPUT_NUMBER+1
  JMP  ERASE_STATE

;******************************************************************************

EEPROM_WRITE:
  CLR  EE_W_CONTROL
  MOV  R7, INPUT_NUMBER
  JMP  EEPROM_COMMON

;******************************************************************************

EEPROM_READ:
  SETB  EE_W_CONTROL

;******************************************************************************

EEPROM_COMMON:
  MOV  R2, #1
  MOV  A, EEPROM_ADR
  MOV  EEPROM_PAGE, SM_RM_PAGE

  MOV  R1, #dirR7
  LCALL  EE_R_W

  MOV  A, EEPROM_ADR     ;Auto-increment address
  ADD  A,#01
  JNC  NO_NEW_PAGE

  MOV  EEPROM_ADR, A    ;Increment page
  INC  SM_RM_PAGE
  JMP  NEW_PAGE

NO_NEW_PAGE:
  MOV  EEPROM_ADR, A

NEW_PAGE:
  JNB  EE_W_CONTROL, WRITING

  CLR  A
  MOV  dirR4, A
  MOV  dirR5, A
  MOV  dirR6, A
  MOV  PROT_STATE, A

  CLR  DECI_OUT
  CLR  DECIMAL1_BIT
  CLR  DECIMAL2_BIT

  JMP  VALUE_OUT
WRITING:
  JMP  ERASE_STATE

;******************************************************************************

INTEGRATION_TIME_OUT:
;  Output present integration time to RS232

  MOV  R7, INT_TIME

VERSIO_FORMAT:
  SETB  DECIMAL1_BIT

  CLR  DECIMAL2_BIT
  CLR  COMMA_BIT
  CLR     DECI_OUT

  CLR  A
  MOV  dirR6, A
  MOV  dirR5, A
  MOV  dirR4, A
  MOV  PROT_STATE, A

  JMP  VALUE_OUT

;******************************************************************************

FIX_GAIN_SELECT:
;  if (( gain >= 0) && ( gain < 7))
;     Force GAIN-selection;
;  else {
;    if ( gain == 255)
;      Remove forced GAIN-selection;
;  }

  CLR  C
  MOV  A, INPUT_NUMBER
  SUBB  A, #06
  JC  FIX_GAIN_OK

  CLR  C
  MOV  A, INPUT_NUMBER
  SUBB  A, #255
  JNZ  END_FIX_GAIN_SELECTION  ;If ( input == 255)
  CLR  MODE_FG      ;  remove forced GAIN-selection.

END_FIX_GAIN_SELECTION:
  JMP  ERASE_STATE

FIX_GAIN_OK:
  MOV  REF_GAIN, INPUT_NUMBER
  SETB  MODE_FG
  JMP  ERASE_STATE

;******************************************************************************

REQUEST_IDENTITY:
;  Send ID-string to output together with software-version;

  SETB  EE_W_CONTROL
  SETB  EE_TO_RS

  MOV  R2, #29      ;Number of Caracters in eeprom
  MOV  EEPROM_PAGE, #00  ;Identity always in page 0.
  MOV  A, #ID_STRING_ADR

  LCALL  EE_TO_RS232
  LCALL  COMMA_OUT

  MOV  R7, #SOFTWARE_RELEASE  ;Version
  JMP  VERSIO_FORMAT

;******************************************************************************

MEASURE_OFFSET:

;  Routine uses RAM containing Correction matrix. This area is restored
;  at the end of the routine

  SETB  MODE_MO
  SETB  MODE_FG
  CLR  MODE_MC
  CLR  MODE_XY

  MOV  R1, #010H
  MOV  R0, #030H
  CLR  A

CLEAR_WORK_AREA:
  MOV  @R0, A
  INC  R0
  DJNZ  R1, CLEAR_WORK_AREA
  
  MOV  AVERAGE_COUNT, A
  MOV  REF_GAIN, A    ;Start offset measurement with GAIN=1

  MOV  INT_TIME, #DEF_INT_TIME  ;Force integration-time to 50.0ms
  JMP  ERASE_STATE

;******************************************************************************

NORMAL_RUN:
  CLR  MODE_ST
  CLR  MODE_SO
  CLR  MODE_SI
  CLR  MODE_FG
  CLR  MODE_MO

  SETB  EE_W_CONTROL

  MOV  R2, #18      ;Reload Correction matrix
  MOV  A, #K_MATRIX_EEPROM
  MOV  R1, #K_MATRIX_RAM
  LCALL  EE_TO_RS232 

CLEAR_XYZ_OUTPUT:
;  Force output to "old" sensor xyz-output, (ie. MX-mode)

  CLR  MODE_XY
  CLR  MODE_BARCO

  JMP  ERASE_STATE

;******************************************************************************

SHOW_ONLY_OFFSET:
;  Force output offset-values for present GAIN;

  SETB  MODE_SO
  CLR  MODE_ST
  CLR  MODE_XY

  JMP  ERASE_STATE

;******************************************************************************

SHOW_ONLY_COUNT:
;  Force output direct counts from A/D-converter;

  CLR  MODE_SO
  CLR  MODE_XY
  SETB  MODE_ST
  JMP  ERASE_STATE

;******************************************************************************

SET_INTEGRATION_TIME:
;  if (( SENSOR_VERSION == INDUSTRIAL_HEAD) ||
;     ( SENSOR_VERSION == SMALL_HEAD)) {
;     if (( integration_time < 5ms) || ( integration_time > 50ms))
;        ;
;     else
;        Set new integration time;
;  }

  MOV  A, SENSOR_VERSION
  CJNE  A, #INDUSTRIAL_HEAD, SIT_TEST_SMALL_HEAD
  JMP  SIT_CONTINUE

SIT_TEST_SMALL_HEAD:
  CJNE  A, #SMALL_HEAD, END_SET_INTEGRATION_TIME
  
SIT_CONTINUE:
  CLR  C
  MOV  A, INPUT_NUMBER
  SUBB  A, #25
  JC  END_SET_INTEGRATION_TIME

  CLR  C
  MOV  A, INPUT_NUMBER
  SUBB  A, #251
  JNC  END_SET_INTEGRATION_TIME  

  SETB  MODE_SI
  MOV  USER_INT_TIME, INPUT_NUMBER
  MOV  INT_TIME, INPUT_NUMBER

END_SET_INTEGRATION_TIME:

  JMP  ERASE_STATE

;******************************************************************************

SET_XYZ_OUTPUT:
;  Force output to CIE 1931 XYZ-values;

  SETB  MODE_XY
  CLR  MODE_BARCO

  JMP  ERASE_STATE

;******************************************************************************

SET_BARCO_OUTPUT:
;  Force output to CIE 1931 XYZ-values AND BARCO/THOMA output-format;

  SETB  MODE_XY
  SETB  MODE_BARCO
  JMP  ERASE_STATE

;******************************************************************************

READ_NAME:
  SETB  EE_W_CONTROL
  CLR  COMMA_BIT

  MOV  workR2, #00    ;Initialize CHECKSUM to ZERO.

  MOV  workR3, INPUT_NUMBER  ;Read number of characters to transmit.
  MOV  A, workR3

  JNZ  READ_N_LOOP    ;Check to prevent reading more than
  INC  workR3      ; 255 characters if number_new =0
          ; (this is an error condition).
READ_N_LOOP:
  MOV  R2, #1

  MOV  A, EEPROM_ADR
  MOV  EEPROM_PAGE, SM_RM_PAGE

  MOV  R1, #dirR7
  LCALL  EE_R_W

  MOV  A, EEPROM_ADR     ;Auto-increment address
  ADD  A,#01
  JNC  NO_NEW_PAGE_CS

  MOV  EEPROM_ADR, A    ;Increment page
  INC  SM_RM_PAGE

  CLR  C
  MOV  A, NUM_PAGES
  RRC  A
  ANL  A, SM_RM_PAGE
  MOV  SM_RM_PAGE, A

  JMP  NEW_PAGE_CS

NO_NEW_PAGE_CS:
  MOV  EEPROM_ADR, A

NEW_PAGE_CS:
  MOV  A, dirR7  ;Write read BYTE to RS232
  LCALL  WRITC

  ADD  A, workR2
  MOV  workR2, A  ;Add byte to CHECKSUM.

  CLR  C
  MOV  A, workR3
  SUBB  A, #01
  MOV  workR3, A

  JNZ  READ_N_LOOP

  MOV  A, workR2
  LCALL  WRITC    ;Write CHECKSUM to RS232

  MOV  A, #0DH
  LCALL  WRITC    ;Write <LINEFEED> to RS232

  JMP  ERASE_STATE

;******************************************************************************

WRITE_PROTECTION:

  MOV  A, INPUT_NUMBER
  JZ  PROTECTION_ON

PROTECTION_OFF:
;  if ( 1. COMMAND = MS;) {
;     if ( 2. COMMAND = WR79;)
;        if ( 3. COMMAND = WR102;)
;           if( 4. COMMAND = WR102;)
;              WRITE PROTECTTION OFF;
;  }
;  else
;     FORGET IT (WO)MAN!!

  CJNE  A, #'O', CHECK_F

  MOV  A, PROT_STATE
  CJNE  A, #01, ERASE_ALL

  MOV  PROT_STATE, #02
  RET

CHECK_F:
  CJNE  A, #'f', ERASE_ALL

  MOV  A, PROT_STATE
  CJNE  A, #02, CHECK_STATE3

  MOV  PROT_STATE, #03
  RET

CHECK_STATE3:
  CJNE  A, #03, ERASE_ALL

  CLR  EEPROM_PROTECT

  MOV  A, #0FFH
  MOV  TEMP_VALUE, A
  MOV  TEMP_VALUE+1, A

  MOV  R2, #02
  MOV  R1, #TEMP_VALUE
  MOV  A, #WR_PROTECT_INFO
  LCALL  EE_WRITE

ERASE_ALL:
  JMP  ERASE_STATE

PROTECTION_ON:
;  if ( COMMAND = WR0)
;     WRITE PROTECTION ON;

  CLR  EEPROM_PROTECT

  CLR  A
  MOV  TEMP_VALUE, A
  MOV  TEMP_VALUE+1, A

  MOV  R2, #02
  MOV  R1, #TEMP_VALUE
  MOV  A, #WR_PROTECT_INFO
  LCALL  EE_WRITE

  SETB  EEPROM_PROTECT
  RET

;******************************************************************************

CALCULATE_CHECKSUM:
;  workR0, workR1 contains CHECKSUM
;  workR2, workR3 contains number of bytes

  CLR  A
  MOV  workR0, A
  MOV  workR0+1, A
  MOV  workR2, A
  MOV  workR2+1, A

LABEL_01:
  MOV  DPL, workR2+1
  MOV  DPH, workR2

  CLR  A
  MOVC  A, @A+DPTR

  ADD  A, workR0+1
  MOV  workR0+1, A

  CLR  A
  ADDC  A, workR0
  MOV  workR0, A

  INC  workR2+1
  MOV  A, workR2+1

  JNZ  LABEL_02
  INC  workR2

LABEL_02:
  CLR  A
  CJNE  A, workR2+1, LABEL_01
  MOV  A, workR2
  CJNE  A, #010H, LABEL_01

  MOV  A, workR0
  LCALL  WRITC

  MOV  A, workR0+1
  LCALL  WRITC

  LCALL  CR_OUT

  JMP  ERASE_STATE

;******************************************************************************

SET_BAUDRATE:
;  if ( SENSOR_VERSION == SMALL_HEAD)
;     set new baudrate, (48, 96, 192)

  MOV   A, SENSOR_VERSION
  CJNE  A, #SMALL_HEAD, END_SET_BAUDRATE

  MOV   A, INPUT_NUMBER
  CJNE  A, #048, TEST_BAUDRATE_9600
  MOV   TEMP_VALUE, #Baud_4800
  JMP   WRITE_BAUDRATE
  
TEST_BAUDRATE_9600:
  CJNE  A, #096, TEST_BAUDRATE_19200
  MOV   TEMP_VALUE, #Baud_9600
  JMP   WRITE_BAUDRATE

TEST_BAUDRATE_19200:
  CJNE  A, #0192, END_SET_BAUDRATE
  MOV   TEMP_VALUE, #Baud_19200

WRITE_BAUDRATE:
  MOV   TH1, TEMP_VALUE    ;Load new baudrate

  CLR   A                  ;Save PROTECTION information
  MOV   C, EEPROM_PROTECT
  RRC   A
  MOV   TEMP_VALUE+1, A

  CLR   EEPROM_PROTECT    ;Temporarily disable witre protection

  MOV   R2, #01           ;Save baudrate information
  MOV   R1, #TEMP_VALUE
  MOV   A, #BAUDRATE_INFO
  LCALL EE_WRITE

  MOV  A, TEMP_VALUE+1    ;Restore PROTECTION information
  RLC  A
  MOV  EEPROM_PROTECT, C

END_SET_BAUDRATE:
  RET

END
