;****************************************************************
;* Filename	: PORT_DEF.ICL					*
;* Function	: Definition of 87C51 Hardware-pins		*
;* Author	: St.A.A/K.Engedahl				*
;* Latest revision:	94-06-15				*
;****************************************************************

;* 940615:
;* Added two new definitions, BaudRate_0 and BaudRate_1
;****************************************************************


RS232_IN_PIN	EQU	P3.0	;RS232 receive pin.
RS232_OUT_PIN	EQU	P3.1	;RS232 transmit pin.
AD_COMP_PIN	EQU	P3.2	;A/D-converter comparator output.
;		EQU	P3.3	;
;		EQU	P3.4	;
;		EQU	P3.5	;
;		EQU	P3.6	;
;		EQU	P3.7	;

ANA_CONTR_PORT	EQU	P1	;Analog control is collected in port 1+2

SCL_PIN		EQU	P0.0	;IýC-bus clock line.
SDA_PIN		EQU	P0.1	;IýC-bus data line.
WD_TRIG_PIN	EQU	P0.2	;Trigger to watch-dog-circuit
;		EQU	P0.3
;		EQU	P0.4
BaudRate_0	EQU	P0.5
BaudRate_1	EQU	P0.6
;		EQU	P0.7

EDGE_OR_LEV_0	BIT	TCON.0	;
EDGE_ON_INT0	BIT	TCON.1	;
EDGE_OR_LEV_1	BIT	TCON.2	;
EDGE_ON_INT1	BIT	TCON.3	;
TR_0		BIT	TCON.4	; 
TF_0		BIT	TCON.5	; 
TR_1		BIT	TCON.6	;
TF_1		BIT	TCON.7	;
