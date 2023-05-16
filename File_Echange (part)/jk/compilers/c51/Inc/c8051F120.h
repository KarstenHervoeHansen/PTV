/*---------------------------------------------------------------------------
; Copyright (C) 2003 CYGNAL INTEGRATED PRODUCTS, INC.
; All rights reserved.
;
;
; FILE NAME: C8051F120.H
; TARGET MCUs: C8051F120, F121, F122, F123, F124, F125, F126, F127
; DESCRIPTION: Register/bit definitions for the C8051F120 product family.
;
; REVISION 1.6
;
;---------------------------------------------------------------------------*/

/*  BYTE Registers  */

sfr P0            = 0x80; /* PORT 0 LATCH */
sfr SP            = 0x81; /* STACK POINTER */
sfr DPL           = 0x82; /* DATA POINTER LOW BYTE */
sfr DPH           = 0x83; /* DATA POINTER HIGH BYTE */
sfr SFRPAGE       = 0x84; /* SFR PAGE SELECT */
sfr SFRNEXT       = 0x85; /* SFR STACK NEXT PAGE */
sfr SFRLAST       = 0x86; /* SFR STACK LAST PAGE */
sfr PCON          = 0x87; /* POWER CONTROL */
sfr FLSTAT        = 0x88; /* FLASH STATUS */
sfr CPT0CN        = 0x88; /* COMPARATOR 0 CONTROL */
sfr CPT1CN        = 0x88; /* COMPARATOR 1 CONTROL */
sfr TCON          = 0x88; /* TIMER/COUNTER CONTROL */
sfr TMOD          = 0x89; /* TIMER/COUNTER MODE */
sfr CPT0MD        = 0x89; /* COMPARATOR 0 CONFIGURATION */
sfr CPT1MD        = 0x89; /* COMPARATOR 1 CONFIGURATION */
sfr PLL0CN        = 0x89; /* PLL CONTROL */
sfr OSCICN        = 0x8A; /* INTERNAL OSCILLATOR CONTROL */
sfr TL0           = 0x8A; /* TIMER/COUNTER 0 LOW BYTE */
sfr OSCICL        = 0x8B; /* INTERNAL OSCILLATOR CALIBRATION */
sfr TL1           = 0x8B; /* TIMER/COUNTER 1 LOW BYTE */
sfr OSCXCN        = 0x8C; /* EXTERNAL OSCILLATOR CONTROL */
sfr TH0           = 0x8C; /* TIMER/COUNTER 0 HIGH BYTE */
sfr TH1           = 0x8D; /* TIMER/COUNTER 1 HIGH BYTE */
sfr PLL0DIV       = 0x8D; /* PLL DIVIDER */
sfr CKCON         = 0x8E; /* CLOCK CONTROL */
sfr PLL0MUL       = 0x8E; /* PLL MULTIPLIER */
sfr PSCTL         = 0x8F; /* FLASH WRITE/ERASE CONTROL */
sfr PLL0FLT       = 0x8F; /* PLL FILTER */
sfr P1            = 0x90; /* PORT 1 LATCH */
sfr SSTA0         = 0x91; /* UART 0 STATUS */
sfr MAC0BL        = 0x91; /* MAC0 B REGISTER LOW BYTE */
sfr MAC0BH        = 0x92; /* MAC0 B REGISTER HIGH BYTE */
sfr MAC0ACC0      = 0x93; /* MAC0 ACCUMULATOR BYTE 0 */
sfr MAC0ACC1      = 0x94; /* MAC0 ACCUMULATOR BYTE 1 */
sfr MAC0ACC2      = 0x95; /* MAC0 ACCUMULATOR BYTE 2 */
sfr SFRPGCN       = 0x96; /* SFR PAGE CONTROL */
sfr MAC0ACC3      = 0x96; /* MAC0 ACCUMULATOR BYTE 3 */
sfr MAC0OVR       = 0x97; /* MAC0 ACCUMULATOR OVERFLOW BYTE */
sfr CLKSEL        = 0x97; /* SYSTEM CLOCK SELECT */
sfr SCON0         = 0x98; /* UART 0 CONTROL */
sfr SCON1         = 0x98; /* UART 1 CONTROL */
sfr SBUF0         = 0x99; /* UART 0 DATA BUFFER */
sfr SBUF1         = 0x99; /* UART 1 DATA BUFFER */
sfr SPI0CFG       = 0x9A; /* SPI CONFIGURATION */
sfr CCH0MA        = 0x9A; /* CACHE MISS ACCUMULATOR */
sfr SPI0DAT       = 0x9B; /* SPI DATA */
sfr P4MDOUT       = 0x9C; /* PORT 4 OUTPUT MODE CONFIGURATION */
sfr P5MDOUT       = 0x9D; /* PORT 5 OUTPUT MODE CONFIGURATION */
sfr SPI0CKR       = 0x9D; /* SPI CLOCK RATE CONTROL */
sfr P6MDOUT       = 0x9E; /* PORT 6 OUTPUT MODE CONFIGURATION */
sfr P7MDOUT       = 0x9F; /* PORT 7 OUTPUT MODE CONFIGURATION */
sfr P2            = 0xA0; /* PORT 2 LATCH */
sfr EMI0TC        = 0xA1; /* EMIF TIMING CONTROL */
sfr CCH0CN        = 0xA1; /* CACHE CONTROL */
sfr EMI0CN        = 0xA2; /* EMIF CONTROL */
sfr CCH0TN        = 0xA2; /* CACHE TUNING */
sfr EMI0CF        = 0xA3; /* EMIF CONFIGURATION */
sfr CCH0LC        = 0xA3; /* CACHE LOCK */
sfr P0MDOUT       = 0xA4; /* PORT 0 OUTPUT MODE CONFIGURATION */
sfr P1MDOUT       = 0xA5; /* PORT 1 OUTPUT MODE CONFIGURATION */
sfr P2MDOUT       = 0xA6; /* PORT 2 OUTPUT MODE CONFIGURATION */
sfr P3MDOUT       = 0xA7; /* PORT 3 OUTPUT MODE CONFIGURATION */
sfr IE            = 0xA8; /* INTERRUPT ENABLE */
sfr SADDR0        = 0xA9; /* UART 0 SLAVE ADDRESS */
sfr P1MDIN        = 0xAD; /* PORT 1 INPUT MODE */
sfr P3            = 0xB0; /* PORT 3 LATCH */
sfr PSBANK        = 0xB1; /* FLASH BANK SELECT */
sfr FLACL         = 0xB7; /* FLASH ACCESS LIMIT */
sfr FLSCL         = 0xB7; /* FLASH SCALE */
sfr IP            = 0xB8; /* INTERRUPT PRIORITY */
sfr SADEN0        = 0xB9; /* UART 0 SLAVE ADDRESS MASK */
sfr AMX0CF        = 0xBA; /* ADC0 MULTIPLEXER CONFIGURATION */
sfr AMX2CF        = 0xBA; /* ADC2 MULTIPLEXER CONFIGURATION */
sfr AMX0SL        = 0xBB; /* ADC0 MULTIPLEXER CHANNEL SELECT */
sfr AMX2SL        = 0xBB; /* ADC2 MULTIPLEXER CHANNEL SELECT */
sfr ADC0CF        = 0xBC; /* ADC0 CONFIGURATION */
sfr ADC2CF        = 0xBC; /* ADC2 CONFIGURATION */
sfr ADC0L         = 0xBE; /* ADC0 DATA WORD LOW BYTE */
sfr ADC2          = 0xBE; /* ADC2DATA WORD */
sfr ADC0H         = 0xBF; /* ADC0 DATA WORD HIGH BYTE */
sfr MAC0STA       = 0xC0; /* MAC0 STATUS */
sfr SMB0CN        = 0xC0; /* SMBUS CONTROL */
sfr MAC0AL        = 0xC1; /* MAC0 A REGISTER LOW BYTE */
sfr SMB0STA       = 0xC1; /* SMBUS STATUS */
sfr MAC0AH        = 0xC2; /* MAC0 A REGISTER HIGH BYTE */
sfr SMB0DAT       = 0xC2; /* SMBUS DATA */
sfr MAC0CF        = 0xC3; /* MAC0 CONFIGURATION REGISTER */
sfr SMB0ADR       = 0xC3; /* SMBUS SLAVE ADDRESS */
sfr ADC0GTL       = 0xC4; /* ADC0 GREATER-THAN LOW BYTE */
sfr ADC2GT        = 0xC4; /* ADC2 GREATER-THAN */
sfr ADC0GTH       = 0xC5; /* ADC0 GREATER-THAN HIGH BYTE */
sfr ADC0LTL       = 0xC6; /* ADC0 LESS-THAN LOW BYTE */
sfr ADC2LT        = 0xC6; /* ADC2 LESS-THAN */
sfr ADC0LTH       = 0xC7; /* ADC0 LESS-THAN HIGH BYTE */
sfr P4            = 0xC8; /* PORT 4 LATCH */
sfr TMR2CN        = 0xC8; /* TIMER/COUNTER 2 CONTROL */
sfr TMR3CN        = 0xC8; /* TIMER 3 CONTROL */
sfr TMR4CN        = 0xC8; /* TIMER/COUNTER 4 CONTROL */
sfr TMR2CF        = 0xC9; /* TIMER/COUNTER 2 CONFIGURATION */
sfr TMR3CF        = 0xC9; /* TIMER 3 CONFIGURATION */
sfr TMR4CF        = 0xC9; /* TIMER/COUNTER 4 CONFIGURATION */
sfr RCAP2L        = 0xCA; /* TIMER/COUNTER 2 CAPTURE/RELOAD LOW BYTE */
sfr RCAP3L        = 0xCA; /* TIMER 3 CAPTURE/RELOAD LOW BYTE */
sfr RCAP4L        = 0xCA; /* TIMER/COUNTER 4 CAPTURE/RELOAD LOW BYTE */
sfr RCAP2H        = 0xCB; /* TIMER/COUNTER 2 CAPTURE/RELOAD HIGH BYTE */
sfr RCAP3H        = 0xCB; /* TIMER 3 CAPTURE/RELOAD HIGH BYTE */
sfr RCAP4H        = 0xCB; /* TIMER/COUNTER 4 CAPTURE/RELOAD HIGH BYTE */
sfr TMR2L         = 0xCC; /* TIMER/COUNTER 2 LOW BYTE */
sfr TMR3L         = 0xCC; /* TIMER 3 LOW BYTE */
sfr TMR4L         = 0xCC; /* TIMER/COUNTER 4 LOW BYTE */
sfr TMR2H         = 0xCD; /* TIMER/COUNTER 2 HIGH BYTE */
sfr TMR3H         = 0xCD; /* TIMER 3 HIGH BYTE */
sfr TMR4H         = 0xCD; /* TIMER/COUNTER 4 HIGH BYTE */
sfr MAC0RNDL      = 0xCE; /* MAC0 ROUNDING REGISTER LOW BYTE */
sfr MAC0RNDH      = 0xCF; /* MAC0 ROUNDING REGISTER HIGH BYTE */
sfr SMB0CR        = 0xCF; /* SMBUS CLOCK RATE */
sfr PSW           = 0xD0; /* PROGRAM STATUS WORD */
sfr REF0CN        = 0xD1; /* VOLTAGE REFERENCE CONTROL */
sfr DAC0L         = 0xD2; /* DAC0 LOW BYTE */
sfr DAC1L         = 0xD2; /* DAC1 LOW BYTE */
sfr DAC0H         = 0xD3; /* DAC0 HIGH BYTE */
sfr DAC1H         = 0xD3; /* DAC1 HIGH BYTE */
sfr DAC0CN        = 0xD4; /* DAC0 CONTROL */
sfr DAC1CN        = 0xD4; /* DAC1 CONTROL */
sfr P5            = 0xD8; /* PORT 5 LATCH */
sfr PCA0CN        = 0xD8; /* PCA CONTROL */
sfr PCA0MD        = 0xD9; /* PCA MODE */
sfr PCA0CPM0      = 0xDA; /* PCA MODULE 0 MODE */
sfr PCA0CPM1      = 0xDB; /* PCA MODULE 1 MODE REGISTER */
sfr PCA0CPM2      = 0xDC; /* PCA MODULE 2 MODE */
sfr PCA0CPM3      = 0xDD; /* PCA MODULE 3 MODE */
sfr PCA0CPM4      = 0xDE; /* PCA MODULE 4 MODE */
sfr PCA0CPM5      = 0xDF; /* PCA MODULE 5 MODE */
sfr ACC           = 0xE0; /* ACCUMULATOR */
sfr XBR0          = 0xE1; /* PORT I/O CROSSBAR CONTROL 0 */
sfr PCA0CPL5      = 0xE1; /* PCA MODULE 5 CAPTURE/COMPARE LOW BYTE */
sfr PCA0CPH5      = 0xE2; /* PCA MODULE 5 CAPTURE/COMPARE HIGH BYTE */
sfr XBR1          = 0xE2; /* PORT I/O CROSSBAR CONTROL 1 */
sfr XBR2          = 0xE3; /* PORT I/O CROSSBAR CONTROL 2 */
sfr EIE1          = 0xE6; /* EXTENDED INTERRUPT ENABLE 1 */
sfr EIE2          = 0xE7; /* EXTENDED INTERRUPT ENABLE 2 */
sfr ADC0CN        = 0xE8; /* ADC0 CONTROL */
sfr ADC2CN        = 0xE8; /* ADC2 CONTROL */
sfr P6            = 0xE8; /* PORT 6 LATCH */
sfr PCA0CPL2      = 0xE9; /* PCA MODULE 2 CAPTURE/COMPARE LOW BYTE */
sfr PCA0CPH2      = 0xEA; /* PCA MODULE 2 CAPTURE/COMPARE HIGH BYTE */
sfr PCA0CPL3      = 0xEB; /* PCA MODULE 3 CAPTURE/COMPARE LOW BYTE */
sfr PCA0CPH3      = 0xEC; /* PCA MODULE 3 CAPTURE/COMPARE HIGH BYTE */
sfr PCA0CPL4      = 0xED; /* PCA MODULE 4 CAPTURE/COMPARE LOW BYTE */
sfr PCA0CPH4      = 0xEE; /* PCA MODULE 4 CAPTURE/COMPARE HIGH BYTE */
sfr RSTSRC        = 0xEF; /* RESET SOURCE */
sfr B             = 0xF0; /* B REGISTER */
sfr EIP1          = 0xF6; /* EXTERNAL INTERRUPT PRIORITY 1 */
sfr EIP2          = 0xF7; /* EXTERNAL INTERRUPT PRIORITY 2 */
sfr P7            = 0xF8; /* PORT 7 LATCH */
sfr SPI0CN        = 0xF8; /* SPI CONTROL */
sfr PCA0L         = 0xF9; /* PCA COUNTER LOW BYTE */
sfr PCA0H         = 0xFA; /* PCA COUNTER HIGH BYTE */
sfr PCA0CPL0      = 0xFB; /* PCA MODULE 0 CAPTURE/COMPARE LOW BYTE */
sfr PCA0CPH0      = 0xFC; /* PCA MODULE 0 CAPTURE/COMPARE HIGH BYTE */
sfr PCA0CPL1      = 0xFD; /* PCA MODULE 1 CAPTURE/COMPARE LOW BYTE */
sfr PCA0CPH1      = 0xFE; /* PCA MODULE 1 CAPTURE/COMPARE HIGH BYTE */
sfr WDTCN         = 0xFF; /* WATCHDOG TIMER CONTROL */


/* Bit Definitions */

/* TCON 0x88 */
sbit TF1 =        0x8F ;  /* TIMER 1 OVERFLOW FLAG */
sbit TR1 =        0x8E ;  /* TIMER 1 ON/OFF CONTROL */
sbit TF0 =        0x8D ;  /* TIMER 0 OVERFLOW FLAG */
sbit TR0 =        0x8C ;  /* TIMER 0 ON/OFF CONTROL */
sbit IE1 =        0x8B ;  /* EXT. INTERRUPT 1 EDGE FLAG */
sbit IT1 =        0x8A ;  /* EXT. INTERRUPT 1 TYPE */
sbit IE0 =        0x89 ;  /* EXT. INTERRUPT 0 EDGE FLAG */
sbit IT0 =        0x88 ;  /* EXT. INTERRUPT 0 TYPE */

/* CPT0CN 0x88 */
sbit CP0EN =      0x8F ;  /* COMPARATOR 0 ENABLE */
sbit CP0OUT =     0x8E ;  /* COMPARATOR 0 OUTPUT */
sbit CP0RIF =     0x8D ;  /* COMPARATOR 0 RISING EDGE INTERRUPT */
sbit CP0FIF =     0x8C ;  /* COMPARATOR 0 FALLING EDGE INTERRUPT */
sbit CP0HYP1 =    0x8B ;  /* COMPARATOR 0 POSITIVE HYSTERISIS 1 */
sbit CP0HYP0 =    0x8A ;  /* COMPARATOR 0 POSITIVE HYSTERISIS 0 */
sbit CP0HYN1 =    0x89 ;  /* COMPARATOR 0 NEGATIVE HYSTERISIS 1 */
sbit CP0HYN0 =    0x88 ;  /* COMPARATOR 0 NEGATIVE HYSTERISIS 0 */

/* CPT1CN 0x88 */
sbit CP1EN =      0x8F ;  /* COMPARATOR 1 ENABLE */
sbit CP1OUT =     0x8E ;  /* COMPARATOR 1 OUTPUT */
sbit CP1RIF =     0x8D ;  /* COMPARATOR 1 RISING EDGE INTERRUPT */
sbit CP1FIF =     0x8C ;  /* COMPARATOR 1 FALLING EDGE INTERRUPT */
sbit CP1HYP1 =    0x8B ;  /* COMPARATOR 1 POSITIVE HYSTERISIS 1 */
sbit CP1HYP0 =    0x8A ;  /* COMPARATOR 1 POSITIVE HYSTERISIS 0 */
sbit CP1HYN1 =    0x89 ;  /* COMPARATOR 1 NEGATIVE HYSTERISIS 1 */
sbit CP1HYN0 =    0x88 ;  /* COMPARATOR 1 NEGATIVE HYSTERISIS 0 */

/* FLSTAT 0x88 */
sbit FLHBUSY =    0x88 ;  /* FLASH BUSY */

/* SCON0 0x98 */
sbit SM00 =       0x9F ;  /* UART 0 MODE 0 */
sbit SM10 =       0x9E ;  /* UART 0 MODE 1 */
sbit SM20 =       0x9D ;  /* UART 0 MULTIPROCESSOR EN */
sbit REN0 =       0x9C ;  /* UART 0 RX ENABLE */
sbit TB80 =       0x9B ;  /* UART 0 TX BIT 8 */
sbit RB80 =       0x9A ;  /* UART 0 RX BIT 8 */
sbit TI0 =        0x99 ;  /* UART 0 TX INTERRUPT FLAG */
sbit RI0 =        0x98 ;  /* UART 0 RX INTERRUPT FLAG */

/* SCON1 0x98 */
sbit S1MODE =     0x9F ;  /* UART 1 MODE */
sbit MCE1 =       0x9D ;  /* UART 1 MCE */
sbit REN1 =       0x9C ;  /* UART 1 RX ENABLE */
sbit TB81 =       0x9B ;  /* UART 1 TX BIT 8 */
sbit RB81 =       0x9A ;  /* UART 1 RX BIT 8 */
sbit TI1 =        0x99 ;  /* UART 1 TX INTERRUPT FLAG */
sbit RI1 =        0x98 ;  /* UART 1 RX INTERRUPT FLAG */

/* IE 0xA8 */
sbit EA =         0xAF ;  /* GLOBAL INTERRUPT ENABLE */
sbit ET2  =       0xAD ;  /* TIMER 2 INTERRUPT ENABLE */
sbit ES0  =       0xAC ;  /* UART0 INTERRUPT ENABLE */
sbit ET1 =        0xAB ;  /* TIMER 1 INTERRUPT ENABLE */
sbit EX1 =        0xAA ;  /* EXTERNAL INTERRUPT 1 ENABLE */
sbit ET0 =        0xA9 ;  /* TIMER 0 INTERRUPT ENABLE */
sbit EX0 =        0xA8 ;  /* EXTERNAL INTERRUPT 0 ENABLE */

/* IP 0xB8 */
sbit PT2 =        0xBD ;  /* TIMER 2 PRIORITY */
sbit PS =         0xBC ;  /* SERIAL PORT PRIORITY */
sbit PT1 =        0xBB ;  /* TIMER 1 PRIORITY */
sbit PX1 =        0xBA ;  /* EXTERNAL INTERRUPT 1 PRIORITY */
sbit PT0 =        0xB9 ;  /* TIMER 0 PRIORITY */
sbit PX0 =        0xB8 ;  /* EXTERNAL INTERRUPT 0 PRIORITY */

/* SMB0CN 0xC0 */
sbit BUSY =       0xC7 ;  /* SMBUS 0 BUSY */
sbit ENSMB =      0xC6 ;  /* SMBUS 0 ENABLE */
sbit STA  =       0xC5 ;  /* SMBUS 0 START FLAG */
sbit STO =        0xC4 ;  /* SMBUS 0 STOP FLAG */
sbit SI =         0xC3 ;  /* SMBUS 0 INTERRUPT PENDING FLAG */
sbit AA =         0xC2 ;  /* SMBUS 0 ASSERT/ACKNOWLEDGE FLAG */
sbit SMBFTE =     0xC1 ;  /* SMBUS 0 FREE TIMER ENABLE */
sbit SMBTOE =     0xC0 ;  /* SMBUS 0 TIMEOUT ENABLE */

/* MAC0STA 0xC0 */
sbit MAC0HO =     0xC3 ;  /* MAC0 HARD OVERFLOW */
sbit MAC0Z =      0xC2 ;  /* MAC0 ZERO */
sbit MAC0SO =     0xC1 ;  /* MAC0 SOFT OVERFLOW */
sbit MAC0N =      0xC0 ;  /* MAC0 NEGATIVE */

/* TMR2CN 0xC8 */
sbit TF2 =        0xCF ;  /* TIMER 2 OVERFLOW FLAG */
sbit EXF2 =       0xCE ;  /* TIMER 2 EXTERNAL FLAG */
sbit EXEN2 =      0xCB ;  /* TIMER 2 EXTERNAL ENABLE FLAG */
sbit TR2 =        0xCA ;  /* TIMER 2 ON/OFF CONTROL */
sbit CT2 =        0xC9 ;  /* TIMER 2 COUNTER SELECT */
sbit CPRL2 =      0xC8 ;  /* TIMER 2 CAPTURE SELECT */

/* TMR3CN 0xC8 */
sbit TF3 =        0xCF ;  /* TIMER 3 OVERFLOW FLAG */
sbit EXF3 =       0xCE ;  /* TIMER 3 EXTERNAL FLAG */
sbit EXEN3 =      0xCB ;  /* TIMER 3 EXTERNAL ENABLE FLAG */
sbit TR3 =        0xCA ;  /* TIMER 3 ON/OFF CONTROL */
sbit CT3 =        0xC9 ;  /* TIMER 3 COUNTER SELECT */
sbit CPRL3 =      0xC8 ;  /* TIMER 3 CAPTURE SELECT */

/* TMR4CN 0xC8 */
sbit TF4 =        0xCF ;  /* TIMER 4 OVERFLOW FLAG */
sbit EXF4 =       0xCE ;  /* TIMER 4 EXTERNAL FLAG */
sbit EXEN4 =      0xCB ;  /* TIMER 4 EXTERNAL ENABLE FLAG */
sbit TR4 =        0xCA ;  /* TIMER 4 ON/OFF CONTROL */
sbit CT4 =        0xC9 ;  /* TIMER 4 COUNTER SELECT */
sbit CPRL4 =      0xC8 ;  /* TIMER 4 CAPTURE SELECT */

/* PSW 0xD0 */
sbit CY =         0xD7 ;  /* CARRY FLAG */
sbit AC =         0xD6 ;  /* AUXILIARY CARRY FLAG */
sbit F0 =         0xD5 ;  /* USER FLAG 0 */
sbit RS1 =        0xD4 ;  /* REGISTER BANK SELECT 1 */
sbit RS0 =        0xD3 ;  /* REGISTER BANK SELECT 0 */
sbit OV =         0xD2 ;  /* OVERFLOW FLAG */
sbit F1 =         0xD1 ;  /* USER FLAG 1 */
sbit P =          0xD0 ;  /* ACCUMULATOR PARITY FLAG */

/* PCA0CN 0xD8 */
sbit CF =         0xDF ;  /* PCA 0 COUNTER OVERFLOW FLAG */
sbit CR =         0xDE ;  /* PCA 0 COUNTER RUN CONTROL BIT */
sbit CCF5 =       0xDD ;  /* PCA 0 MODULE 5 INTERRUPT FLAG */
sbit CCF4 =       0xDC ;  /* PCA 0 MODULE 4 INTERRUPT FLAG */
sbit CCF3 =       0xDB ;  /* PCA 0 MODULE 3 INTERRUPT FLAG */
sbit CCF2 =       0xDA ;  /* PCA 0 MODULE 2 INTERRUPT FLAG */
sbit CCF1 =       0xD9 ;  /* PCA 0 MODULE 1 INTERRUPT FLAG */
sbit CCF0 =       0xD8 ;  /* PCA 0 MODULE 0 INTERRUPT FLAG */

/* ADC0CN 0xE8 */
sbit AD0EN  =     0xEF ;  /* ADC 0 ENABLE */
sbit AD0TM =      0xEE ;  /* ADC 0 TRACK MODE */
sbit AD0INT =     0xED ;  /* ADC 0 EOC INTERRUPT FLAG */
sbit AD0BUSY =    0xEC ;  /* ADC 0 BUSY FLAG */
sbit AD0CM1   =   0xEB ;  /* ADC 0 CONVERT START MODE BIT 1 */
sbit AD0CM0 =     0xEA ;  /* ADC 0 CONVERT START MODE BIT 0 */
sbit AD0WINT =    0xE9 ;  /* ADC 0 WINDOW INTERRUPT FLAG */
sbit AD0LJST =    0xE8 ;  /* ADC 0 RIGHT JUSTIFY DATA BIT */

/* ADC2CN 0xE8 */
sbit AD2EN  =     0xEF ;  /* ADC 2 ENABLE */
sbit AD2TM =      0xEE ;  /* ADC 2 TRACK MODE */
sbit AD2INT =     0xED ;  /* ADC 2 EOC INTERRUPT FLAG */
sbit AD2BUSY =    0xEC ;  /* ADC 2 BUSY FLAG */
sbit AD2CM2 =     0xEB ;  /* ADC 2 CONVERT START MODE BIT 2 */
sbit AD2CM1 =     0xEA ;  /* ADC 2 CONVERT START MODE BIT 1 */
sbit AD2CM0 =     0xE9 ;  /* ADC 2 CONVERT START MODE BIT 0 */
sbit AD2WINT =    0xE8 ;  /* ADC 2 WINDOW INTERRUPT FLAG */

/* SPI0CN 0xF8 */
sbit SPIF =       0xFF ;  /* SPI 0 INTERRUPT FLAG */
sbit WCOL =       0xFE ;  /* SPI 0 WRITE COLLISION FLAG */
sbit MODF =       0xFD ;  /* SPI 0 MODE FAULT FLAG */
sbit RXOVRN =     0xFC ;  /* SPI 0 RX OVERRUN FLAG */
sbit NSSMD1 =     0xFB ;  /* SPI 0 SLAVE SELECT MODE 1 */
sbit NSSMD0 =     0xFA ;  /* SPI 0 SLAVE SELECT MODE 0 */
sbit TXBMT =      0xF9 ;  /* SPI 0 TX BUFFER EMPTY FLAG */
sbit SPIEN =      0xF8 ;  /* SPI 0 SPI ENABLE */


/* SFR PAGE DEFINITIONS */

#define CONFIG_PAGE       0x0F    /* SYSTEM AND PORT CONFIGURATION PAGE */
#define LEGACY_PAGE       0x00    /* LEGACY SFR PAGE */
#define TIMER01_PAGE      0x00    /* TIMER 0 AND TIMER 1 */
#define CPT0_PAGE         0x01    /* COMPARATOR 0 */
#define CPT1_PAGE         0x02    /* COMPARATOR 1 */
#define UART0_PAGE        0x00    /* UART 0 */
#define UART1_PAGE        0x01    /* UART 1 */
#define SPI0_PAGE         0x00    /* SPI 0 */
#define EMI0_PAGE         0x00    /* EXTERNAL MEMORY INTERFACE */
#define ADC0_PAGE         0x00    /* ADC 0 */
#define ADC2_PAGE         0x02    /* ADC 2 */
#define SMB0_PAGE         0x00    /* SMBUS 0 */
#define TMR2_PAGE         0x00    /* TIMER 2 */
#define TMR3_PAGE         0x01    /* TIMER 3 */
#define TMR4_PAGE         0x02    /* TIMER 4 */
#define DAC0_PAGE         0x00    /* DAC 0 */
#define DAC1_PAGE         0x01    /* DAC 1 */
#define PCA0_PAGE         0x00    /* PCA 0 */
#define PLL0_PAGE         0x0F    /* PLL 0 */
#define MAC0_PAGE         0x03    /* MAC 0 */
