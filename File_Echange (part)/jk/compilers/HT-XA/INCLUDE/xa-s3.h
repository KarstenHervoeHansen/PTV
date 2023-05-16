/*** XA-S3 SFR definitions for HiTech compiler ***/

#define	__xa_h__

extern unsigned int	__trap(unsigned char, ...);
#pragma inline(__trap)

static unsigned char PSWL   @ 0x400;   /* Program status word (low byte) */
static unsigned char PSWH   @ 0x401;   /* Program status word (high byte) */
static unsigned char PSW51  @ 0x402;   /* 80C51 compatible PSW */
static unsigned char SSEL   @ 0x403;   /* Segment selection register */
static unsigned char PCON   @ 0x404;   /* Power control register */
static unsigned char TCON   @ 0x410;   /* Timer 0 and 1 control register */
static unsigned char TSTAT  @ 0x411;   /* Timer 0 and 1 extended status */
static unsigned char T2CON  @ 0x418;   /* Timer 2 control register */
static unsigned char T2MOD  @ 0x419;   /* Timer 2 mode control */
static unsigned char CCON   @ 0x41A;   /* PCA counter control */
static unsigned char WDCON  @ 0x41F;   /* Watchdog control register */
static unsigned char S0CON  @ 0x420;   /* Serial port 0 control register */
static unsigned char S0STAT @ 0x421;   /* Serial port 0 extended status */
static unsigned char S1CON  @ 0x424;   /* Serial port 1 control register */
static unsigned char S1STAT @ 0x425;   /* Serial port 1 extended status */
static unsigned char IEL    @ 0x426;   /* Interrupt enable low byte */
static unsigned char IEH    @ 0x427;   /* Interrupt enable high byte */
static unsigned char SWR    @ 0x42A;   /* Software Interrupt Request */
static unsigned char I2CON  @ 0x42C;   /* I2C control register */
static unsigned char IELB   @ 0x42E;   /* Int. enable extension low byte */
static unsigned char P0     @ 0x430;   /* Port 0 */
static unsigned char P1     @ 0x431;   /* Port 1 */
static unsigned char P2     @ 0x432;   /* Port 2 */
static unsigned char P3     @ 0x433;   /* Port 3 */
static unsigned char P4     @ 0x434;   /* Port 4 */
static unsigned char P5     @ 0x435;   /* Port 5 */
static unsigned char P6     @ 0x436;   /* Port 6 */
static unsigned char ADCON  @ 0x43E;   /* A/D control register */
static unsigned char ADCS   @ 0x43F;   /* A/D channel select register */
static unsigned char SCR    @ 0x440;   /* System configuration register */
static unsigned char DS     @ 0x441;   /* Data segment */
static unsigned char ES     @ 0x442;   /* Extra segment */
static unsigned char CS     @ 0x443;   /* Code segment */
static unsigned char TL0    @ 0x450;   /* Timer 0 low byte */
static unsigned char TH0    @ 0x451;   /* Timer 0 high byte */
static unsigned char TL1    @ 0x452;   /* Timer 1 low byte */
static unsigned char TH1    @ 0x453;   /* Timer 1 high byte */
static unsigned char RTL0   @ 0x454;   /* Timer 0 reload, low byte */
static unsigned char RTH0   @ 0x455;   /* Timer 0 reload, high byte */
static unsigned char RTL1   @ 0x456;   /* Timer 1 reload, low byte */
static unsigned char RTH1   @ 0x457;   /* Timer 1 reload, high byte */
static unsigned char TL2    @ 0x458;   /* Timer 2 low byte */
static unsigned char TH2    @ 0x459;   /* Timer 2 high byte */
static unsigned char T2CAPL @ 0x45A;   /* Timer 2 capture reg, low byte */
static unsigned char T2CAPH @ 0x45B;   /* Timer 2 capture reg, high byte */
static unsigned char TMOD   @ 0x45C;   /* Timer 0 and 1 mode control */
static unsigned char WFEED1 @ 0x45D;   /* Watchdog feed 1 */
static unsigned char WFEED2 @ 0x45E;   /* Watchdog feed 2 */
static unsigned char WDL    @ 0x45F;   /* Watchdog timer reload */
static unsigned char S0BUF  @ 0x460;   /* uart 0 buffer register */
static unsigned char S0ADDR @ 0x461;   /* uart 0 address register */
static unsigned char S0ADEN @ 0x462;   /* uart 0 address enable register */
static unsigned char RSTSRC @ 0x463;   /* Reset source register */
static unsigned char S1BUF  @ 0x464;   /* uart 1 buffer register */
static unsigned char S1ADDR @ 0x465;   /* uart 1 address register */
static unsigned char S1ADEN @ 0x466;   /* uart 1 address enable register */
static unsigned char BTRL   @ 0x468;   /* Bus timing register low byte */
static unsigned char BTRH   @ 0x469;   /* Bus timing register high byte */
static unsigned char BCR    @ 0x46A;   /* Bus configuration register */
static unsigned char I2STAT @ 0x46C;   /* I2C status register */
static unsigned char I2DAT  @ 0x46D;   /* I2C data register */
static unsigned char I2ADDR @ 0x46E;   /* I2C address register */
static unsigned char P0CFGA @ 0x470;   /* Port 0 configuration A */
static unsigned char P1CFGA @ 0x471;   /* Port 1 configuration A */
static unsigned char P2CFGA @ 0x472;   /* Port 2 configuration A */
static unsigned char P3CFGA @ 0x473;   /* Port 3 configuration A */
static unsigned char P4CFGA @ 0x474;   /* Port 4 configuration A */
static unsigned char P5CFGA @ 0x475;   /* Port 5 configuration A */
static unsigned char P6CFGA @ 0x476;   /* Port 6 configuration A */
static unsigned char SWE    @ 0x47A;   /* Software Interrupt Enable */
static unsigned char CL     @ 0x48A;   /* PCA counter low byte */
static unsigned char CH     @ 0x48B;   /* PCA counter high byte */
static unsigned char CMOD   @ 0x490;   /* PCA mode control */
static unsigned char CCAPM0 @ 0x491;   /* PCA module 0 mode */
static unsigned char CCAPM1 @ 0x492;   /* PCA module 1 mode */
static unsigned char CCAPM2 @ 0x493;   /* PCA module 2 mode */
static unsigned char CCAPM3 @ 0x494;   /* PCA module 3 mode */
static unsigned char CCAPM4 @ 0x495;   /* PCA module 4 mode */
static unsigned char CCAP0L @ 0x496;   /* PCA module 0 capture low byte */
static unsigned char CCAP0H @ 0x497;   /* PCA module 0 capture high byte */
static unsigned char CCAP1L @ 0x498;   /* PCA module 1 capture low byte */
static unsigned char CCAP1H @ 0x499;   /* PCA module 1 capture high byte */
static unsigned char CCAP2L @ 0x49A;   /* PCA module 2 capture low byte */
static unsigned char CCAP2H @ 0x49B;   /* PCA module 2 capture high byte */
static unsigned char CCAP3L @ 0x49C;   /* PCA module 3 capture low byte */
static unsigned char CCAP3H @ 0x49D;   /* PCA module 3 capture high byte */
static unsigned char CCAP4L @ 0x49E;   /* PCA module 4 capture low byte */
static unsigned char CCAP4H @ 0x49F;   /* PCA module 4 capture high byte */
static unsigned char IPA0   @ 0x4A0;   /* Interrupt priority A0 */
static unsigned char IPA1   @ 0x4A1;   /* Interrupt priority A1 */
static unsigned char IPA2   @ 0x4A2;   /* Interrupt priority A2 */
static unsigned char IPA3   @ 0x4A3;   /* Interrupt priority A3 */
static unsigned char IPA4   @ 0x4A4;   /* Interrupt priority A4 */
static unsigned char IPA5   @ 0x4A5;   /* Interrupt priority A5 */
static unsigned char IPB0   @ 0x4A8;   /* Interrupt priority B0 */
static unsigned char IPB1   @ 0x4A9;   /* Interrupt priority B1 */
static unsigned char IPB2   @ 0x4AA;   /* Interrupt priority B2 */
static unsigned char ADRSH0 @ 0x4B0;   /* A/D high byte result channel 0 */
static unsigned char ADRSH1 @ 0x4B1;   /* A/D high byte result channel 1 */
static unsigned char ADRSH2 @ 0x4B2;   /* A/D high byte result channel 2 */
static unsigned char ADRSH3 @ 0x4B3;   /* A/D high byte result channel 3 */
static unsigned char ADRSH4 @ 0x4B4;   /* A/D high byte result channel 4 */
static unsigned char ADRSH5 @ 0x4B5;   /* A/D high byte result channel 5 */
static unsigned char ADRSH6 @ 0x4B6;   /* A/D high byte result channel 6 */
static unsigned char ADRSH7 @ 0x4B7;   /* A/D high byte result channel 7 */
static unsigned char ADRSL  @ 0x4B8;   /* A/D low 2 bits of result */
static unsigned char ADCFG  @ 0x4B9;   /* A/D timing configuration */
static unsigned char P0CFGB @ 0x4F0;   /* Port 0 configuration B */
static unsigned char P1CFGB @ 0x4F1;   /* Port 1 configuration B */
static unsigned char P2CFGB @ 0x4F2;   /* Port 2 configuration B */
static unsigned char P3CFGB @ 0x4F3;   /* Port 3 configuration B */
static unsigned char P4CFGB @ 0x4F4;   /* Port 4 configuration B */
static unsigned char P5CFGB @ 0x4F5;   /* Port 5 configuration B */
static unsigned char P6CFGB @ 0x4F6;   /* Port 6 configuration B */


static bit Z       @ 0x200;     /* PSW bits */
static bit N       @ 0x201;
static bit V       @ 0x202;
static bit AC      @ 0x206;
static bit C       @ 0x207;
static bit IM0     @ 0x208;
static bit IM1     @ 0x209;
static bit IM2     @ 0x20A;
static bit IM3     @ 0x20B;
static bit RS0     @ 0x20C;
static bit RS1     @ 0x20D;
static bit TM      @ 0x20E;
static bit SM      @ 0x20F;
static bit P       @ 0x210;
static bit F1      @ 0x211;
static bit F0      @ 0x215;

static bit R0SEG   @ 0x218;     /* SSEL bits */
static bit R1SEG   @ 0x219;
static bit R2SEG   @ 0x21A;
static bit R3SEG   @ 0x21B;
static bit R4SEG   @ 0x21C;
static bit R5SEG   @ 0x21D;
static bit R6SEG   @ 0x21E;
static bit ESWEN   @ 0x21F;

static bit IDL     @ 0x220;     /* idle mode */
static bit PD      @ 0x221;     /* power down */

static bit IT0     @ 0x280;     /* Interrupt 0 level or edge select */
static bit IE0     @ 0x281;     /* Interrupt 0 flag */
static bit IT1     @ 0x282;     /* Interrupt 1 level or edge select */
static bit IE1     @ 0x283;     /* Interrupt 1 flag */
static bit TR0     @ 0x284;     /* Timer 0 run control */
static bit TF0     @ 0x285;     /* Timer 0 overflow flag */
static bit TR1     @ 0x286;     /* Timer 1 run control */
static bit TF1     @ 0x287;     /* Timer 1 overflow flag */
static bit T0OE    @ 0x288;     /* T0 pin control */
static bit T1OE    @ 0x28A;     /* T1 pin control */

static bit CPRL2   @ 0x2C0;     /* Timer 2 capture/reload flag */
static bit CT2     @ 0x2C1;     /* Timer 2 timer/counter select */
static bit TR2     @ 0x2C2;     /* Timer 2 run control */
static bit EXEN2   @ 0x2C3;     /* Timer 2 external enable bit */
static bit TCLK0   @ 0x2C4;     /* Timer 2 transmit clock flag */
static bit RCLK0   @ 0x2C5;     /* Timer 2 receive clock flag */
static bit EXF2    @ 0x2C6;     /* Timer 2 external flag */
static bit TF2     @ 0x2C7;     /* Timer 2 overflow flag */
static bit DCEN    @ 0x2C8;     /* Timer 2 count direction control */
static bit T2OE    @ 0x2C9;     /* T2 pin control */
static bit TCLK1   @ 0x2CC;     /* Transmit clock flag */
static bit RCLK1   @ 0x2CD;     /* Receive clock flag */

static bit CCF0    @ 0x2D0;     /* PCA channel 0 interrupt flag */
static bit CCF1    @ 0x2D1;     /* PCA channel 1 interrupt flag */
static bit CCF2    @ 0x2D2;     /* PCA channel 2 interrupt flag */
static bit CCF3    @ 0x2D3;     /* PCA channel 3 interrupt flag */
static bit CCF4    @ 0x2D4;     /* PCA channel 4 interrupt flag */
static bit CR      @ 0x2D6;     /* PCA counter run control bit */
static bit CF      @ 0x2D7;     /* PCA counter overflow flag */

static bit WDTOF   @ 0x2F9;     /* watchdog overflow flag */
static bit WDRUN   @ 0x2FA;     /* watchdog run */
static bit PRE0    @ 0x2FD;     /* watchdog prescaler bit 0 */
static bit PRE1    @ 0x2FE;     /* watchdog prescaler bit 1 */
static bit PRE2    @ 0x2FF;     /* watchdog prescaler bit 2 */

static bit RI_0    @ 0x300;     /* UART0 receive interrupt flag */
static bit TI_0    @ 0x301;     /* UART0 transmit interrupt flag */
static bit RB8_0   @ 0x302;     /* UART0 mode 2/3 9th received bit */
static bit TB8_0   @ 0x303;     /* UART0 mode 2/3 9th transmit bit */
static bit REN_0   @ 0x304;     /* UART0 receive enable */
static bit SM2_0   @ 0x305;     /* UART0 mode select bit 2 */
static bit SM1_0   @ 0x306;     /* UART0 mode select bit 1 */
static bit SM0_0   @ 0x307;     /* UART0 mode select bit 0 */
static bit STINT0  @ 0x308;     /* UART0 status interrupt enable */
static bit OE0     @ 0x309;     /* UART0 overrun error flag */
static bit BR0     @ 0x30A;     /* UART0 break detect flag */
static bit FE0     @ 0x30B;     /* UART0 framing error flag */
static bit ERR0    @ 0x30C;

static bit RI_1    @ 0x320;     /* UART1 receive interrupt flag */
static bit TI_1    @ 0x321;     /* UART1 transmit interrupt flag */
static bit RB8_1   @ 0x322;     /* UART1 mode 2/3 9th received bit */
static bit TB8_1   @ 0x323;     /* UART1 mode 2/3 9th transmit bit */
static bit REN_1   @ 0x324;     /* UART1 receive enable */
static bit SM2_1   @ 0x325;     /* UART1 mode select bit 2 */
static bit SM1_1   @ 0x326;     /* UART1 mode select bit 1 */
static bit SM0_1   @ 0x327;     /* UART1 mode select bit 0 */
static bit STINT1  @ 0x328;     /* UART1 status interrupt enable */
static bit OE1     @ 0x329;     /* UART1 overrun error flag */
static bit BR1     @ 0x32A;     /* UART1 break detect flag */
static bit FE1     @ 0x32B;     /* UART1 framing error flag */
static bit ERR1    @ 0x32C;

static bit EX0     @ 0x330;     /* enable external interrupt 0 */
static bit ET0     @ 0x331;     /* enable timer 0 interrupt */
static bit EX1     @ 0x332;     /* enable external interrupt 1 */
static bit ET1     @ 0x333;     /* enable timer 1 interrupt */
static bit ET2     @ 0x334;     /* enable timer 2 interrupt */
static bit EPC     @ 0x335;     /* enable PCA interrupt */
static bit EAD     @ 0x336;     /* enable ADC interrupt */
static bit EA      @ 0x337;     /* global int. enable */
static bit ERI0    @ 0x338;     /* RX 0 interrupt enable */
static bit ETI0    @ 0x339;     /* TX 0 interrupt enable */
static bit ERI1    @ 0x33A;     /* RX 1 interrupt enable */
static bit ETI1    @ 0x33B;     /* TX 1 interrupt enable */

static bit SWR1    @ 0x350;     /* Software interrupt bits */
static bit SWR2    @ 0x351;
static bit SWR3    @ 0x352;
static bit SWR4    @ 0x353;
static bit SWR5    @ 0x354;
static bit SWR6    @ 0x355;
static bit SWR7    @ 0x356;

static bit CR0     @ 0x360;     /* I2C rate control bit 0 */
static bit CR1     @ 0x361;     /* I2C rate control bit 1 */
static bit AA      @ 0x362;     /* I2C assert acknowledge */
static bit SI      @ 0x363;     /* I2C serial interrupt flag */
static bit STO     @ 0x364;     /* I2C stop flag */
static bit STA     @ 0x365;     /* I2C start flag */
static bit ENA     @ 0x366;     /* I2C enable */
static bit CR2     @ 0x367;     /* I2C rate control bit 2 */

static bit EC0     @ 0x370;     /* enable PCA channel 0 interrupt */
static bit EC1     @ 0x371;     /* enable PCA channel 1 interrupt */
static bit EC2     @ 0x372;     /* enable PCA channel 2 interrupt */
static bit EC3     @ 0x373;     /* enable PCA channel 3 interrupt */
static bit EC4     @ 0x374;     /* enable PCA channel 4 interrupt */
static bit EI2     @ 0x375;     /* enable I2C interrupt */

static bit P0_0    @ 0x380;     /* Port 0 bits */
static bit P0_1    @ 0x381;
static bit P0_2    @ 0x382;
static bit P0_3    @ 0x383;
static bit P0_4    @ 0x384;
static bit P0_5    @ 0x385;
static bit P0_6    @ 0x386;
static bit P0_7    @ 0x387;

static bit P1_0    @ 0x388;     /* Port 1 bits */
static bit P1_1    @ 0x389;
static bit P1_2    @ 0x38A;
static bit P1_3    @ 0x38B;
static bit P1_4    @ 0x38C;
static bit P1_5    @ 0x38D;
static bit P1_6    @ 0x38E;
static bit P1_7    @ 0x38F;

static bit P2_0    @ 0x390;     /* Port 2 bits */
static bit P2_1    @ 0x391;
static bit P2_2    @ 0x392;
static bit P2_3    @ 0x393;
static bit P2_4    @ 0x394;
static bit P2_5    @ 0x395;
static bit P2_6    @ 0x396;
static bit P2_7    @ 0x397;

static bit P3_0    @ 0x398;     /* Port 3 bits */
static bit P3_1    @ 0x399;
static bit P3_2    @ 0x39A;
static bit P3_3    @ 0x39B;
static bit P3_4    @ 0x39C;
static bit P3_5    @ 0x39D;
static bit P3_6    @ 0x39E;
static bit P3_7    @ 0x39F;

static bit P4_0    @ 0x3A0;     /* Port 4 bits */
static bit P4_1    @ 0x3A1;
static bit P4_2    @ 0x3A2;
static bit P4_3    @ 0x3A3;
static bit P4_4    @ 0x3A4;
static bit P4_5    @ 0x3A5;
static bit P4_6    @ 0x3A6;
static bit P4_7    @ 0x3A7;

static bit P5_0    @ 0x3A8;     /* Port 5 bits */
static bit P5_1    @ 0x3A9;
static bit P5_2    @ 0x3AA;
static bit P5_3    @ 0x3AB;
static bit P5_4    @ 0x3AC;
static bit P5_5    @ 0x3AD;
static bit P5_6    @ 0x3AE;
static bit P5_7    @ 0x3AF;

static bit P6_0    @ 0x3B0;     /* Port 6 bits */
static bit P6_1    @ 0x3B1;

static bit ADINT   @ 0x3F0;     /* A/D conversion ready interrupt flag */
static bit ADSST   @ 0x3F1;     /* A/D start and status */
static bit ADMOD   @ 0x3F2;     /* A/D mode select */
static bit ADRES   @ 0x3F3;     /* A/D 8 or 10 bit mode select */

static bit ADCS0   @ 0x3F8;     /* A/D channel 0 select bit */
static bit ADCS1   @ 0x3F9;     /* A/D channel 1 select bit */
static bit ADCS2   @ 0x3FA;     /* A/D channel 2 select bit */
static bit ADCS3   @ 0x3FB;     /* A/D channel 3 select bit */
static bit ADCS4   @ 0x3FC;     /* A/D channel 4 select bit */
static bit ADCS5   @ 0x3FD;     /* A/D channel 5 select bit */
static bit ADCS6   @ 0x3FE;     /* A/D channel 6 select bit */
static bit ADCS7   @ 0x3FF;     /* A/D channel 7 select bit */


/* Exception vectors */
#define	IV_BRKPT	0x04	/* breakpoint vector */
#define	IV_TRACE	0x08	/* Trace mode bit set */
#define	IV_STKOVER	0x0C	/* stack overflow */
#define	IV_DIVZERO	0x10	/* divide by zero */
#define	IV_IRET		0x14	/* user mode IRET */

/* Trap interrupt vectors */
#define IV_TRI0		0x40
#define IV_TRI1		0x44
#define IV_TRI2		0x48
#define IV_TRI3		0x4C
#define IV_TRI4		0x50
#define IV_TRI5		0x54
#define IV_TRI6		0x58
#define IV_TRI7		0x5C
#define IV_TRI8		0x60
#define IV_TRI9		0x64
#define IV_TRI10	0x68
#define IV_TRI11	0x6C
#define IV_TRI12	0x70
#define IV_TRI13	0x74
#define IV_TRI14	0x78
#define IV_TRI15	0x7C

/* Event interrupt vectors */
#define	IV_EX0		0x80	/* External interrupt 0 */
#define	IV_T0		0x84	/* Timer 0 interrupt */
#define	IV_EX1		0x88	/* External interrupt 1 */
#define	IV_T1		0x8C	/* Timer 1 interrupt */
#define	IV_T2		0x90	/* Timer 2 interrupt */
#define	IV_PCA		0x94	/* PCA interrupt */
#define	IV_ADC		0x98	/* A/D interrupt */
#define	IV_RI0		0xA0	/* Serial port 0 Rx */
#define	IV_TI0		0xA4	/* Serial port 0 Tx */
#define	IV_RI1		0xA8	/* Serial port 1 Rx */
#define	IV_TI1		0xAC	/* Serial port 1 Tx */
#define	IV_PCA0		0xC0	/* PCA channel 0 */
#define	IV_PCA1		0xC4	/* PCA channel 1 */
#define	IV_PCA2		0xC8	/* PCA channel 2 */
#define	IV_PCA3		0xCC	/* PCA channel 3 */
#define	IV_PCA4		0xD0	/* PCA channel 4 */
#define	IV_I2C		0xD4	/* i2c interrupt */

/* Software interrupt vectors */
#define	IV_SWI1		0x100
#define	IV_SWI2		0x104
#define	IV_SWI3		0x108
#define	IV_SWI4		0x10C
#define	IV_SWI5		0x110
#define	IV_SWI6		0x114
#define	IV_SWI7		0x118

/* PSW Values for interrupt vectors */
#define	IV_PSW		0x8F00	/* System mode, high priority, bank 0 */
#define	IV_SYSTEM	0x8000
#define	IV_PRI00	0x0000	/* priorities 0 - 15 */
#define	IV_PRI01	0x0100
#define	IV_PRI02	0x0200
#define	IV_PRI03	0x0300
#define	IV_PRI04	0x0400
#define	IV_PRI05	0x0500
#define	IV_PRI06	0x0600
#define	IV_PRI07	0x0700
#define	IV_PRI08	0x0800
#define	IV_PRI09	0x0900
#define	IV_PRI10	0x0A00
#define	IV_PRI11	0x0B00
#define	IV_PRI12	0x0C00
#define	IV_PRI13	0x0D00
#define	IV_PRI14	0x0E00
#define	IV_PRI15	0x0F00

#define	IV_BANK0	0x0000
#define	IV_BANK1	0x1000
#define	IV_BANK2	0x2000
#define	IV_BANK3	0x3000
