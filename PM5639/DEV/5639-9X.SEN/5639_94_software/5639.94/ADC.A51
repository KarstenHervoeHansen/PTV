NAME ADC

;************************************************************************
;* Filename  : ADC.A51            *
;* Function  : A/D-Measure subroutines.        *
;* Author  : St.A.A/K.Engedahl          *
;* Latest revision:  94-12-06          *
;************************************************************************

;************************************************************************
;* 941206:                *
;* No updates                *
;* 940906:                *
;* Reference to bit GAIN_UNSTABLE has been removed,       *
;*  (eq. ANALOG_CONTROL bit 6).            *
;* 940628:                *
;*  Added immediate gain change after decision about changing gain  *
;* 940615:                *
;* This module hasn't been changed, BUT, it should be noted that all  *
;*  the timing now relates to a 11,0592MHz crystal as opposed to the  *
;*  old 12MHz crystal. Therefore the microseconds mentioned in the  *
;*  source below should be translated to machine cycles.    *
;************************************************************************

$INCLUDE ( REG51.INC)
$INCLUDE ( PORT_DEF.ICL)
$INCLUDE ( DEFIRAM.ICL)

PUBLIC  MEASURE, WAIT_TIMER_0

EXTRN  CODE( EE_READ, RECALL_FLOAT, STORE_FLOAT)

EXTRN  CODE( ?C_FPMUL, ?C_FCASTI, ?C_CASTF, ?C_FPADD, ?C_FPSUB)
EXTRN  CODE( ?C_LPUSH, ?C_LPULL)

  ADC SEGMENT CODE

  RSEG ADC

;******************************************************************************
MEASURE:

;  CALCULATE VALUE OF OVERHEAD.
;
;  T1 + 200*INT_TIME - OVERHEAD + X + T2 + (255 -(X + 41)) + T3
;       := 200*INT_TIME
;
;  OVERHEAD := T1 + T2 + 255 - 41 + T3 
;           := 21 + 6 + 255 - 41 + 14
;           := 256

OVERHEAD  EQU  256  ;Delay equal to the number of integration-
        ; cycles minus overhead, to maintain an
        ; accurate number of integration cycles.
  POP     DPH
  POP     DPL

  MOV  INT_TIME, #DEF_INT_TIME

  JNB  MODE_SI, TEST_FAST_GAIN
  MOV  INT_TIME, USER_INT_TIME

TEST_FAST_GAIN:
  JNB  FAST_GAIN, NOT_FAST_GAIN

  MOV  INT_TIME, #DEF_FAST_GAIN

NOT_FAST_GAIN:

  MOV  P1, ANALOG_CONTROL+1  ;Load AD-converter port

  MOV  A, INT_TIME  ;(1) Convert to microsec by multiplying by 100

  MOV  B, #2*100  ;(2) Set timer to twice the integration time
  MUL  AB    ;(4) Mul allways clears carry
  SUBB  A, #LOW(OVERHEAD)
        ;(1) Subtract overhead (LOW byte)
  CPL  A    ;(1) and complement before loading timer.
  MOV  TL0, A    ;(1)
  MOV  INT_LOAD_LOW, A  ;(1) Save info about integration-time, (LOW).

  MOV  A, B    ;(1)
  SUBB  A, #HIGH(OVERHEAD) ;(1) Subtract overhead (HIGH byte)
  CPL  A    ;(1) and complement before loading timer.
  MOV  TH0, A    ;(1)
  MOV  INT_LOAD_HIGH, A;(1) Save info about integration-time, (HIGH).

  MOV  A, TMOD    ;(1)
  ANL  A, #11110111B  ;(1)
  MOV  TMOD, A    ;(1)

  CLR  TF_0    ;(1)
  SETB  TR_0    ;(1)

  ;----------------------> T1 (integrate_start -> timer on) := 21 cycles

;******************************************************************************
CALC_GAIN_OFFSET_VALUE:
;  Calculate where to find the offset base on the actual sensor and gain:
;  GAIN_OFFSET = ( 2 * gain) + ( 12 * sensor)

  MOV  A, ANALOG_CONTROL+1
  ANL  A, #00000011B    ;Find actual sensor
  MOV  B, #12
  MUL  AB
  MOV  GAIN_OFFSET, A    ;GAIN_OFFSET = 12*sensor

  MOV  A, ACTUAL_SENSOR
  ADD  A, #04
  MOV  R1, A
  MOV  A, @R1      ;A = gain for sensor being measured

  MOV  C, MODE_FG
  ORL  C, MODE_MO

  JNC  ADD_ACTUAL_GAIN    ;if ( FIX_GAIN || MEASURE_OFFSET)
  MOV  A, REF_GAIN    ;   A = reference gain;

ADD_ACTUAL_GAIN:
  RL  A
  ADD  A, GAIN_OFFSET
  MOV  GAIN_OFFSET, A    ;GAIN_OFFSET = 2*gain + 12*sensor

  JB  FAST_GAIN, WAIT_TIMER_0

;******************************************************************************
LOAD_OFFSET:

;  TEMP_VALUE = ( float) fabs( OFFSET[gain,channel]/500) * INT_TIME * 2);
;
;  if ( OFFSET[gain,channel] > 0)
;     OFFSET_SIGN = 0;
;  else
;     OFFSET_SIGN = 1;
;
;  NOTE!!  LOAD_OFFSET are not used during MEAS_OFFSET and FIX_GAIN_TEST
;    but calculated anyhow.
;

;  MOV  A, GAIN_OFFSET  ;Excess code: ACC already equals GAIN_OFFSET
  ADD  A, #OFFSET_ADR

  MOV  EEPROM_PAGE, #00
  MOV  R1, #workR5
  MOV  R2, #02
  LCALL  EE_READ      ;Load OFFSET[gain,sensor] from EýPROM

  MOV  A, workR6
  MOV  C, ACC.7
  MOV  OFFSET_SIGN, C
  CLR  ACC.7      ;OFFSET_SIGN = sign of OFFSET

  MOV  R4, A
  MOV  R5, workR5
  CLR  A      ;OFFSET = unsigned int OFFSET;
  LCALL  ?C_FCASTI    ;OFFSET = ( float) OFFSET;

  LCALL  ?C_LPUSH

  MOV  dirR7, #03BH
  MOV  dirR6, #003H
  MOV  dirR5, #012H
  MOV  dirR4, #097H

  LCALL  ?C_FPMUL    ;OFFSET = OFFSET * ( 1/500.0);

  LCALL  ?C_LPUSH

  MOV  A, INT_TIME
  MOV  B, #02
  MUL  AB
  MOV  dirR5, A
  MOV  dirR4, B
  CLR  A      ;INT_TIME = unsigned int 2*INT_TIME;
  LCALL  ?C_FCASTI    ;INT_TIME = ( float) INT_TIME;
  LCALL  ?C_FPMUL    ;OFFSET = OFFSET*(1/500)*INT_TIME*2

  MOV  R1, #TEMP_VALUE
  LCALL  STORE_FLOAT

;******************************************************************************
WAIT_TIMER_0:

;  WAIT for timer overrun then switch to Reference Integrate.
;  Calculate timing from actual timer value, 
;  and insert NOPs to have exact timing.

  JNB  TF_0, $    

  MOV  A, TL0
  ADD  A, #41    ;(1)
  CPL  A    ;(1)
  RRC  A    ;(1)
  JNC  $+3    ;(2)
  NOP
  MOV  R1, A    ;(1)
  DJNZ  R1, $    ;
  ;----------------------> T2 (timer_read -> exec delay) = 6us

  MOV  C, AD_COMP_PIN  ;(1) Test signal polarity
  MOV  AD_SIGN, C  ;(2) AD_SIGN='1': Positive input signal

  CLR     TR_0           ;(1) Stop timer

  MOV  TL0, INT_LOAD_LOW  ;(2)
  MOV  TH0, INT_LOAD_HIGH  ;(2)

  MOV  A, TMOD    ;(1) START Timer_0
  ORL  A, #00001000B  ;(1) MODE : Gated by external pin.
  MOV  TMOD, A    ;(1)

  ORL  P1, #0C0H  ;(3)
  ;----------------------> T3 (delay -> Reference Integrate ) = 14us

  SETB    TR_0    ;Start Reference Integration
  CLR  TF_0    ;Use TF as overflow-indicator
  
;******************************************************************************
WT_CONV_END:

;  Wait for AD-comparator to go low, ( or overflow to occur)
;  Only update of watchdog is allowed

  JB  TF_0, RI_STOPPED

  CPL  WD_TRIG_PIN      ;Update watchdog
  JB  AD_COMP_PIN, WT_CONV_END  

  CLR  TR0        ;Stop timer 0, ( just in case)

  CLR  C
  MOV  A, TL0
  SUBB  A, INT_LOAD_LOW
  MOV   TL0, A

  MOV  A, TH0
  SUBB  A, INT_LOAD_HIGH
  MOV  TH0, A

RI_STOPPED:

;  AD-comparator low or overflow in timer_0
;  Store timer values in Dx_VALUE++
;  Start Zero Integration

  CLR  TR_0    ;Stop timer 0

  MOV  R1, ACTUAL_SENSOR

  MOV  @R1, TL0
  MOV  workR1, TL0
  INC  R1
  MOV  @R1, TH0    ;Store count from timer 0 in Dx_VALUE++
  MOV  workR0, TH0

  MOV  C, TF_0      ;Store overflow flag from timer 0;
  MOV  OVERFLOW_ERROR, C

  JNC  START_ZERO_INTEGRATION

  SETB  SENSOR_OVERFLOW    ;Save info about overflow till ALL
          ; sensors have been measured.

START_ZERO_INTEGRATION:

  ANL  P1, #03FH  ;Start ZERO integration

  MOV  A, TMOD
  ANL  A, #11110111B
  MOV  TMOD, A

;  ZERO Integrate for 20% of INT_TIME:
;  0.2*INT_TIME*2*100 = INT_TIME*40

  MOV  A, INT_TIME
  MOV  B, #40
  MUL  AB

  CPL  A
  MOV  TL0, A

  MOV  A, B
  CPL  A
  MOV  TH0, A
  
  CLR  TF_0
  SETB  TR_0

  JNB  TF_0, $    ;Wait for ZERO INTEGRATION to end.

  ORL  P1, #080H  ;Start AUTO ZERO

  CLR  TR_0
  CLR  TF_0

;  AUTO ZERO for 80% of INT_TIME:
;  0.8*INT_TIME*100*2 = 160 * INT_TIME

  MOV  A, INT_TIME
  MOV  B, #160
  MUL  AB

  CPL  A
  MOV  TL0, A

  MOV  A, B
  CPL  A
  MOV  TH0, A

  SETB  TR_0

;******************************************************************************
ADJUST_NEXT_GAIN:

  INC  R1
  INC  R1
  INC  R1

CALC_LIMITS:
  MOV  A, INT_TIME
  MOV  B, #LIMIT_HIGH
  MUL  AB

  MOV  workR2, B
  MOV  workR3, A

  MOV  A, INT_TIME
  MOV  B, #LIMIT_LOW
  MUL  AB

  MOV  workR4, B
  MOV  workR5, A

CALCULATE_NEXT_GAIN:

  CLR  C
  MOV  A, workR3
  SUBB  A, workR1
  MOV  A, workR2
  SUBB  A, workR0    ;HIGH_LIMIT - COUNT_VALUE

  MOV  A, @R1
  ORL  C, OVERFLOW_ERROR
  JC  G_DOWN

  CLR  C
  MOV  A, workR5
  SUBB  A, workR1
  MOV  A, workR4
  SUBB  A, workR0    ;LOW_LIMIT - COUNT_VALUE

  MOV  A, @R1
  JC  G_STAY

G_UP:   INC     A
G_STAY: INC     A
G_DOWN:  
  ANL  A, #00000111B
  ADD  A, #GAIN_TAB-PC_1
  MOVC  A, @A+PC
PC_1:

;Check for change in gain (Used to supress output untill stable)

  JNB  MODE_FG, NORMAL_GAIN
  MOV  A, REF_GAIN

NORMAL_GAIN:
  XCH  A, @R1
  ANL  A, #00000111B
  XRL  A, @R1

  JZ  SAME_GAIN

;The unstable gain should be changed IMMEDIATELY

  MOV  A, ANALOG_CONTROL+1
  ANL  A, #00000011B

  CJNE  A, #0, TEST_GREEN_CHANNEL

;RED_CHANNEL
  ANL  ANALOG_CONTROL+1, #11000011B
  MOV  A, D1_GAIN
  SWAP  A
  RR  A
  ANL  A, #00111000B
  ORL  ANALOG_CONTROL+1, A    ;Set gain for channel RED

  ORL  GAIN_BYTE, #RED_GAIN_UNSTABLE
  JMP  SAME_GAIN

TEST_GREEN_CHANNEL:
  CJNE  A, #1, BLUE_CHANNEL

;GREEN_CHANNEL
  ANL  ANALOG_CONTROL, #11111000B

  MOV  A, D2_GAIN
  ANL  A, #00000111B
  ORL  ANALOG_CONTROL, A    ;Set gain for channel GREEN
  MOV  P2, ANALOG_CONTROL  

  ORL  GAIN_BYTE, #GREEN_GAIN_UNSTABLE
  JMP  SAME_GAIN

BLUE_CHANNEL:
  ANL  ANALOG_CONTROL, #11000111B
  MOV  A, D3_GAIN
  SWAP  A
  RR  A
  ANL  A, #00111000B
  ORL  ANALOG_CONTROL, A    ;Set gain for channel BLUE
  MOV  P2, ANALOG_CONTROL

  ORL  GAIN_BYTE, #BLUE_GAIN_UNSTABLE

SAME_GAIN:
  JMP  FORBITABEL

GAIN_TAB:
  DB  0
  DB  0
  DB  1
  DB  2
  DB  3
  DB  4
  DB  5
  DB  5
  DB  5

FORBITABEL:
  JNB  OVERFLOW_ERROR, CALCULATE_VIN
  JMP  VIN_ERROR

;******************************************************************************
CALCULATE_VIN:

  MOV  R1, ACTUAL_SENSOR

  MOV  dirR5, @R1
  INC  R1
  MOV  dirR4, @R1
  CLR  A      ;AD_COUNT = unsigned int AD_COUNT;
  LCALL  ?C_FCASTI    ;AD_COUNT = ( float) AD_COUNT;

  MOV  C, MODE_ST    ; if (( Show Counter-mode) ||
  ORL  C, MODE_SO    ;     ( Show Offset-mode) ||
  ORL  C, MODE_MO    ;     ( Measure Offset-modee))
  JC  DONT_CALCULATE_VIN  ; do NOT calculate input voltage

  LCALL  ?C_LPUSH

  MOV  R1, #TEMP_VALUE
  LCALL  RECALL_FLOAT

  MOV  C, AD_SIGN
  CPL  C

  JB  OFFSET_SIGN, NEGATIVE_OFFSET

POSITIVE_OFFSET:

;  fabs(( float) ( AD_COUNT)) pushed.
;  fabs(( float) ( OFFSET)) in R7-R4
;  sign of AD_COUNT in CARRY
;  sign of OFFSET in R7-R4
;
;  if ( AD_SIGN)
;     Vin = fabs( Vin) + fabs( Voffset);
;  else {
;     if (( fabs( voffset) - fabs( vin)) > 0)
;        Vin = fabs( Vin) - fabs( Voffset);
;     else
;        Vin = 0;
;  }

  JNC  TEST_POS_AD_SIGN

  LCALL  ?C_FPADD

  JMP  WRITE_VIN

TEST_POS_AD_SIGN:
  LCALL  ?C_FPSUB    ; fabs( Voffset) - fabs( Vin)

  MOV  A, dirR7
  JB  ACC.7, VIN_ERROR

  JMP  WRITE_VIN

DONT_CALCULATE_VIN:
  JNB  MODE_MO, TEST_SO_ST

  MOV  A, dirR7    ;Do an offset_measurement
  MOV  C, AD_SIGN    ;ie. DO NOT manipulate input values
  CPL  C
  MOV  ACC.7, C
  MOV  dirR7, A

  JMP  WRITE_VIN

TEST_SO_ST:
  JB  MODE_ST, LOAD_GAIN_FACTOR

  MOV  R1, #TEMP_VALUE    ;Show Offset-mode
  LCALL  RECALL_FLOAT    ;ie. copy offset values to output

  JMP  LOAD_GAIN_FACTOR

VIN_ERROR_PULL:
  LCALL  ?C_LPULL

VIN_ERROR:
  CLR  A
  MOV  dirR7, A
  MOV  dirR6, A
  MOV  dirR5, A
  MOV  dirR4, A

  JMP  WRITE_VIN
  
NEGATIVE_OFFSET:

;  fabs( ( float) ( AD_COUNT)) pushed.
;  fabs( ( float) ( OFFSET)) in R7-R4
;  sign of AD_COUNT in CARRY
;  sign of OFFSET in OFFSET_SIGN
;
;  if ( AD_SIGN && OFFSET_SIGN) {
;     if (( fabs( Vin) - fabs( Voffset)) > 0)
;        Vin = fabs( Vin) - fabs( Voffset);
;     else
;        Vin = 0:
;  }
;  else
;     Vin = 0;

  ANL  C, OFFSET_SIGN
  JNC  VIN_ERROR_PULL

  LCALL  ?C_FPSUB    ;fabs( Voffset) - fabs( Vin)

  MOV  A, dirR7
  JNB  ACC.7, VIN_ERROR

  CLR  ACC.7
  MOV  dirR7, A

WRITE_VIN:
  MOV  R1, #TEMP_VALUE
  LCALL  STORE_FLOAT

;******************************************************************************
LOAD_GAIN_FACTOR:
  MOV  C, MODE_ST    ; if (( Show Counter-mode) ||
  ORL  C, MODE_SO    ;     ( Show Offset-mode) ||
  ORL  C, MODE_MO    ;     ( Measure Offset-modee))
  JC  ADC_END      ; do NOT use gain correction

  MOV  EEPROM_PAGE, #00

  MOV  A, GAIN_OFFSET
  ADD  A, #CALIB_ADR

  MOV  R2, #2
  MOV  R1, #workR5
  LCALL  EE_READ      ;Load GAIN_FACT[gain,sensor]

  MOV  dirR5, workR5
  MOV  dirR4, workR6
  CLR  A      ;GAIN_FACT = unsigned int GAIN_FACT;
  LCALL  ?C_FCASTI    ;GAIN_FACT = ( float) GAIN_FACT;

  LCALL  ?C_LPUSH

  MOV  dirR7, #037H
  MOV  dirR6, #080H
  CLR  A
  MOV  dirR5, A
  MOV  dirR4, A

  LCALL  ?C_FPMUL    ;GAIN_FACT = ( GAIN_FACT*1/(2^16));

  MOV  A, ACTUAL_SENSOR
  ADD  A, #04
  MOV  R1, A
  MOV  A, @R1

  MOV  workR0, A    ;Store actual GAIN temporarily
  JZ  DONT_ADJUST_GAIN  ;if ( GAIN = 0) ADJUST = 1;
  CLR  C
  SUBB  A, #01
  JZ  DONT_ADJUST_GAIN  ;if ( GAIN = 1) ADJUST = 1;

  LCALL  ?C_LPUSH

  CLR  C
  MOV  A, #040H
  SUBB  A, workR0

  MOV  dirR7, A
  MOV  dirR6, #080H    ;if ( GAIN = 2) ADJUST = 1/4;
  CLR  A      ;if ( GAIN = 3) ADJUST = 1/16;
  MOV  dirR5, A    ;if ( GAIN = 4) ADJUST = 1/64;
  MOV  dirR4, A    ;if ( GAIN = 5) ADJUST = 1/256;

  LCALL  ?C_FPMUL    ;GAIN_FACTOR = GAIN_FACTOR * ADJUST;

DONT_ADJUST_GAIN:

  LCALL  ?C_LPUSH    ;GAIN_FACTOR stored in dirR7 to dirR4

  MOV  R1, #TEMP_VALUE
  LCALL  RECALL_FLOAT

  LCALL  ?C_FPMUL    ;VIN = VIN[offset] * GAIN_FACTOR[gain];

;******************************************************************************
ADC_END:
  MOV  R1, ACTUAL_SENSOR
  LCALL  STORE_FLOAT

  JNB  TF_0, $    ;Wait for AUTO ZERO to end

  CLR  TR_0
  CLR  TF_0

  CPL  WD_TRIG_PIN  ;Update watch-dog

  CLR     A
  JMP     @A+DPTR
END
