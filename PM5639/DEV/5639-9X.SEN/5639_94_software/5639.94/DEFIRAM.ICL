;************************************************************************
;* Filename  : DEFIRAM.ICL            *
;* Function  : Definition of constants & addresses.      *
;* Author  : St.A.A/K.Engedahl          *
;* Latest revision:  97-06-17          *
;************************************************************************

;************************************************************************
;* 970617:                *
;* Added new sensor type SMALL_HEAD and BAUDRATE_INFO address in EýPROM  *
;* 940806:                *
;* Added new bit: MODE_BARCO.            *
;* 940615:                *
;* Added three constants for baudrate.          *
;************************************************************************

;**************************** CONSTANTS ETC. ****************************

Baud_4800    EQU  0F4H ; 244
Baud_9600    EQU  0FAH ; 250
Baud_19200   EQU  0FDH ; 253

RED_GAIN_UNSTABLE    EQU  00000011B
GREEN_GAIN_UNSTABLE  EQU  00000101B
BLUE_GAIN_UNSTABLE   EQU  00001001B

DEF_INT_TIME   EQU  250  ;Default integration time: 50ms
DEF_FAST_GAIN  EQU  25   ;Fast gain integration time: 5.0ms

LIMIT_HIGH  EQU  ( 42000 / DEF_INT_TIME)   ;Constant used in gain shift
LIMIT_LOW   EQU  ( 7000  / DEF_INT_TIME)   ;Constant used in gain shift

NO_OF_STABILIZE  EQU  10

AVERAGE_DIVIDE   EQU  32

NO_OF_AVERAGE  EQU  ( NO_OF_STABILIZE + AVERAGE_DIVIDE)
                      ;The first NO_OF_STABILIZE measurements are
                      ; used ONLY for calibration purpose and NOT
                      ; used in the final average-calculation

DIVIDE_CONST_1  EQU  042H   ;  8 = 2^3 = 4100
DIVIDE_CONST_0  EQU  000H   ; 16 = 2^4 = 4180
                            ; 32 = 2^5 = 4200
                            ; 64 = 2^6 = 4280
                            ;Constant MUST be equal to AVERAGE_DIVIDE

NO_OF_GAIN_CONSTANTS  EQU  6

CRT_HEAD              EQU  00  ;Code for CRT head
PROJ_HEAD             EQU  01
INDUSTRIAL_HEAD       EQU  02
SMALL_HEAD            EQU  03

EEPROM_IIC_ADR  EQU  0A0H   ;EýPROM address on IýC-bus

CALIB_ADR   EQU  002H       ;Start of 36 byte gain-calibration data
OFFSET_ADR  EQU  079H       ;Start of 36 byte voltage offset area

WR_PROTECT_ADR  EQU  036H   ;Location in EýPROM where address
                            ;of write-protected area resides, (2 bytes)
                            ;adr 0 to WR_PROTECT_HIGH, WR_PROTECT_LOW

WR_PROTECT_INFO  EQU  026H  ;Location in EýPROM where information
                            ;of write-protection ON/OFF resides,
                            ;(2 bytes)

BAUDRATE_INFO  EQU  028H    ;Location in EýPROM where information
                            ;of baudrate for SMALL sensor resides,
                            ;(1 byte)

ID_STRING_ADR    EQU  03EH    ;Location in EýPROM where 25 bytes id-string
                              ; resides

K_MATRIX_EEPROM  EQU  067H    ;Address in EýPROM indicating start of
                              ; K-matrix in EýPROM

INTERN_ID_EEPROM  EQU  03DH   ;Location in EýPROM where version
                              ; resides

TRX_INT_CONST_ADR  EQU  05FH  ;Location in EýPROM where information
                              ; about whether to transmit, (=FFH),
                              ; the integration constant, or not
                              ; (=00). At the moment the display unit
                              ; ALWAYS need this constant.

;************************************************************************

;*********** BANK 0 REGISTERS USED AS CONSTANTS AND ADRESSES ************

;***Register bank 0

dirR0    EQU  0
dirR1    EQU  1
dirR2    EQU  2
dirR3    EQU  3
dirR4    EQU  4
dirR5    EQU  5
dirR6    EQU  6
dirR7    EQU  7

;REG_VALUE  EQU  dirR6  ;Using r5,r6,r7 as value-register

;***Register bank 1 & 2, ( used by floating point).

?C_LACC3  EQU  00BH
?C_LACC2  EQU  00CH
?C_LACC1  EQU  00DH
?C_FACMAN EQU  00DH
?C_LACC0  EQU  00EH
?C_FACEXP EQU  00FH

?C_FACTMP     EQU  015H
?C_DSTKLEVEL  EQU  016H

;***Register bank 3

workR0    EQU  018H  ;Register bank 3 are generel working registers
workR1    EQU  019H
workR2    EQU  01AH
workR3    EQU  01BH
workR4    EQU  01CH
workR5    EQU  01DH
workR6    EQU  01EH
workR7    EQU  01FH

;************************************************************************

;***************** BYTES IN UPPER MEMORY (030H-07FH) ********************

K_MATRIX_RAM  DATA  030H

;  a b c
;  K =  d e f
;  g h j

K_A    DATA  030H  ;( low)
                   ;( high)
K_B    DATA  032H
K_C    DATA  034H
K_D    DATA  036H
K_E    DATA  038H
K_F    DATA  03AH
K_G    DATA  03CH
K_H    DATA  03EH
K_J    DATA  040H  ;( low)
                   ;( high)

D1_AVERAGE     DATA  030H  ;Note memory-mapped with K-matrix
D2_AVERAGE     DATA  034H  ; -"-
D3_AVERAGE     DATA  038H  ; -"-
AVERAGE_COUNT  DATA  03CH  ; -"-
NEXT_SENSOR    DATA  03DH  ; -"-
NEXT_AVERAGE   DATA  03EH  ; -"-

D1_VALUE       DATA  042H  ;4 bytes for value in floating point
D1_GAIN        DATA  046H  ;actual gain for channel

D2_VALUE       DATA  047H  ;4 bytes for value in floating point
D2_GAIN        DATA  04BH  ;actual gain for channel

D3_VALUE       DATA  04CH  ;4 bytes for value in floating point
D3_GAIN        DATA  050H  ;actual gain for channel

GAIN_OFFSET    DATA  051H  ;Address contains 2*gain+12*channel for

ACTUAL_SENSOR  DATA  052H  ;Address of actual sensor being measured

INT_TIME       DATA  053H  ;Integration time in 1/5ms
USER_INT_TIME  DATA  054H  ;             -"-           set by user

REF_GAIN       DATA  055H  ;Reference gain used during MO or FG

EEPROM_PAGE    DATA  056H  ;Page to use when reading/writing EýPROM
EEPROM_ADR     DATA  057H  ;Address to use when reading/writing to EýPROM

NUM_PAGES      DATA  058H  ;Number of pages in EýPROM adapted for IIC
                           ;address i.e. ((num_pages-1)/2)

SM_RM_PAGE     DATA  059H  ;Page read from command "MA n".

WR_PROTECT_LOW DATA  05AH  ;Contains low address, from EýPROM, off write
                           ;protected area, (from 0 till WR_PROTECT_ADR)

WR_PROTECT_HIGH DATA  05BH ;Contains high address of above.

PROT_STATE     DATA  05CH  ;Register used to enable/disable write-
                           ; protection.

SENSOR_VERSION  DATA  05DH  ;Contains info of which sensor:
                            ; 0 = CRT
                            ; 1 = PROJECTOR
                            ; 2 = INDUTRIAL
                            ; 3 = SMALL
                    
INT_LOAD_HIGH  DATA  05EH
INT_LOAD_LOW   DATA  05FH

STACK_START    DATA  070H  ;Area reserved for stack

;************************************************************************

;******************* BYTES IN LOWER RAM (020H-02FH) *********************

;?C_FACBIT      DATA  020H  ;Used by floating point routines

ANALOG_CONTROL  DATA  021H  ;2 bytes for Analog control
MODE            DATA  023H  ;
GAIN_BYTE       DATA  024H  ;
CONTROL_BYTE    DATA  025H  ;2 bytes for control bit

IO_BUFFER       DATA  02AH  ; 6 bytes of in-/output buffer

RX_REG          EQU  IO_BUFFER     ;1 byte for receiving command
INPUT_NUMBER    EQU  IO_BUFFER+1   ;2 bytes for input number

TEMP_VALUE      EQU  02CH  ;Temporary storage for values

;************************************************************************

;**************** ADRESSABLE BITS IN RAM (020H-02FH) ********************

;***1st BIT-BYTE AT 020H:  ?C_FACBIT

;Reserved for floating point
;This area CANNOT be controlled by user ( OR WHAT )

FPERROR      BIT  00H
FPUNDERFLOW  BIT  01H
FPOVERFLOW   BIT  02H
FPNANFLAG    BIT  03H
?C_FCMPFLAG  BIT  04H
?C_FACBTM2   BIT  05H
?C_FACBTM    BIT  06H
?C_FACSGN    BIT  07H

;***2nd BIT-BYTE AT 021H:  ANALOG_CONTROL

GREEN_GAIN_0  BIT  08H
GREEN_GAIN_1  BIT  09H
GREEN_GAIN_2  BIT  0AH
BLUE_GAIN_0   BIT  0BH
BLUE_GAIN_1   BIT  0CH
BLUE_GAIN_2   BIT  0DH
;NOT USED     BIT  0EH
AD_SIGN       BIT  0FH

;***3rd BIT-BYTE AT 022H:  ANALOG_CONTROL + 1

CHANNEL_BIT_0  BIT  10H
CHANNEL_BIT_1  BIT  11H
CHANNEL_BIT_2  BIT  12H
RED_GAIN_0     BIT  13H
RED_GAIN_1     BIT  14H
RED_GAIN_2     BIT  15H
ADCO_BIT_A     BIT  16H
ADCO_BIT_B     BIT  17H

;***4th BIT-BYTE AT 023H:  MODE

MODE_TM    BIT  18H  ;Set to request single measure. Auto-delete.
MODE_MC    BIT  19H  ;if 'one' measure continous (Ignore TM-bit)
MODE_ST    BIT  1AH  ;Show direct count from A/D-converter
MODE_SO    BIT  1BH  ;Show voltage-offset values.
MODE_SI    BIT  1CH  ;Set integration time
MODE_FG    BIT  1DH  ;Set fixed gain
MODE_MO    BIT  1EH  ;If 'one' then offset measurement at 250ms
MODE_XY    BIT  1FH  ;If 'one' then output CIE 1931 XYZ values
                     ; otherwise behave as 'old' CRT version.

;***5th BIT-BYTE AT 024H:  GAIN_SIGN_BYTE

FAST_GAIN       BIT  20H
RED_UNSTABLE    BIT  21H
GREEN_UNSTABLE  BIT  22H
BLUE_UNSTABLE   BIT  23H
OVERFLOW_ERROR  BIT  24H  ;Overflow flag when A/D-comparator is out
                          ; of range ( >2**16us ).
MODE_RX_1       BIT  25H  ;Bit 1 of receiver state-machine
MODE_RX_2       BIT  26H  ;Bit 2 of receiver state-machine
MODE_RX_ACTIVE  BIT  27H  ;'1' : Receiver-statemachine active

;***6th BIT-BYTE AT 025H  CONTROL_BYTE

DECI_OUT        BIT  28H  ;Bit controls decimal output/byte-output
DECIMAL1_BIT    BIT  29H  ;Controls if 1 decimals in output
DECIMAL2_BIT    BIT  2AH  ;Controls if 2 decimals in output
COMMA_BIT       BIT  2BH  ;
OFFSET_SIGN     BIT  2CH  ;Sign of offset-value
EEPROM_PROTECT  BIT  2DH  ;If address 038H-039H in EýPROM contains
                          ; 0FFFFH then writing in EýPROM is ALWAYS
                          ; permitted. Checked during initialization
EE_W_CONTROL    BIT  2EH  ;Used internal in EýPROM read/write
EE_TO_RS        BIT  2FH  ;1: Tranfer EýPROM data direct to RS232


;***7th BIT-BYTE AT 026H  CONTROL_BYTE + 1

BUSY            BIT  30H
SENSOR_OVERFLOW BIT  31H
TRX_INT_CONST   BIT  32H
MODE_BARCO      BIT  33H
