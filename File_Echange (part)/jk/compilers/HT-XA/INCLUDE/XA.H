/*
 *	XA G3 SFR definitions
 *	Extracted directly from Philips documentation
 */

#ifndef	__xa_h__
#define	__xa_h__

extern unsigned int	__trap(unsigned char, ...);
#pragma inline(__trap)

static unsigned short	PSW	@ 0x400;	/* Program status word */
static unsigned char	PSWL	@ 0x400;	/* Program status word (low byte) */
static unsigned char	PSWH	@ 0x401;	/* Program status word (high byte) */
static unsigned char	PSW51	@ 0x402;	/* 80C51 compatible PSW */
static unsigned char	SSEL	@ 0x403;	/* Segment selection register */
static unsigned char	PCON	@ 0x404;	/* Power control register */
static unsigned char	TCON	@ 0x410;	/* Timer 0 and 1 control register */
static unsigned char	TSTAT	@ 0x411;	/* Timer 0 and 1 extended status */
static unsigned char	T2CON	@ 0x418;	/* Timer 2 control register */
static unsigned char	T2MOD	@ 0x419;	/* Timer 2 mode control */
static unsigned char	WDCON	@ 0x41F;	/* Watchdog control register */
static unsigned char	S0CON	@ 0x420;	/* Serial port 0 control register */
static unsigned char	S0STAT	@ 0x421;	/* Serial port 0 extended status */
static unsigned char	S1CON	@ 0x424;	/* Serial port 1 control register */
static unsigned char	S1STAT	@ 0x425;	/* Serial port 1 extended status */
static unsigned char	IEL	@ 0x426;	/* Interrupt enable low byte */
static unsigned char	IEH	@ 0x427;	/* Interrupt enable high byte */
static unsigned char	SWR	@ 0x42A;	/* Software Interrupt Request */
static unsigned char	P0	@ 0x430;	/* Port 0 */
static unsigned char	P1	@ 0x431;	/* Port 1 */
static unsigned char	P2	@ 0x432;	/* Port 2 */
static unsigned char	P3	@ 0x433;	/* Port3 */
static unsigned char	SCR	@ 0x440;	/* System configuration register */
static unsigned char	DS	@ 0x441;	/* Data segment */
static unsigned char	ES	@ 0x442;	/* Extra segment */
static unsigned char	CS	@ 0x443;	/* Code segment */
static unsigned char	TL0	@ 0x450;	/* Timer 0 low byte */
static unsigned char	TH0	@ 0x451;	/* Timer 0 high byte */
static unsigned char	TL1	@ 0x452;	/* Timer 1 low byte */
static unsigned char	TH1	@ 0x453;	/* Timer 1 high byte */
static unsigned char	RTL0	@ 0x454;	/* Timer 0 extended reload, low byte */
static unsigned char	RTH0	@ 0x455;	/* Timer 0 extended reload, high byte */
static unsigned char	RTL1	@ 0x456;	/* Timer 1 extended reload, low byte */
static unsigned char	RTH1	@ 0x457;	/* Timer 1 extended reload, high byte */
static unsigned char	TL2	@ 0x458;	/* Timer 2 low byte */
static unsigned char	TH2	@ 0x459;	/* Timer 2 high byte */
static unsigned char	T2CAPL	@ 0x45A;	/* Timer 2 capture register, low byte */
static unsigned char	T2CAPH	@ 0x45B;	/* Timer 2 capture register, high byte */
static unsigned char	TMOD	@ 0x45C;	/* Timer 0 and 1 mode register */
static unsigned char	WFEED1	@ 0x45D;	/* Watchdog feed 1 */
static unsigned char	WFEED2	@ 0x45E;	/* Watchdog feed 2 */
static unsigned char	WDL	@ 0x45F;	/* Watchdog timer reload */
static unsigned char	S0BUF	@ 0x460;	/* Serial port 0 buffer register */
static unsigned char	S0ADDR	@ 0x461;	/* Serial port 0 address register */
static unsigned char	S0ADEN	@ 0x462;	/* Serial port 0 address enable register */
static unsigned char	S1BUF	@ 0x464;	/* Serial port 1 buffer register */
static unsigned char	S1ADDR	@ 0x465;	/* Serial port 1 address register */
static unsigned char	S1ADEN	@ 0x466;	/* Serial port 1 address enable register */
static unsigned char	BTRL	@ 0x468;	/* Bus timing register high byte */
static unsigned char	BTRH	@ 0x469;	/* Bus timing register low byte */
static unsigned char	BCR	@ 0x46A;	/* Bus configuration register */
static unsigned char	P0CFGA	@ 0x470;	/* Port 0 configuration A */
static unsigned char	P1CFGA	@ 0x471;	/* Port 1 configuration A */
static unsigned char	P2CFGA	@ 0x472;	/* Port 2 configuration A */
static unsigned char	P3CFGA	@ 0x473;	/* Port 3 configuration A */
static unsigned char	SWE	@ 0x47A;	/* Software Interrupt Enable */
static unsigned char	IPA0	@ 0x4A0;	/* Interrupt priority 0 */
static unsigned char	IPA1	@ 0x4A1;	/* Interrupt priority 1 */
static unsigned char	IPA2	@ 0x4A2;	/* Interrupt priority 2 */
static unsigned char	IPA4	@ 0x4A4;	/* Interrupt priority 4 */
static unsigned char	IPA5	@ 0x4A5;	/* Interrupt priority 5 */
static unsigned char	P0CFGB	@ 0x4F0;	/* Port 0 configuration B */
static unsigned char	P1CFGB	@ 0x4F1;	/* Port 1 configuration B */
static unsigned char	P2CFGB	@ 0x4F2;	/* Port 2 configuration B */
static unsigned char	P3CFGB	@ 0x4F3;	/* Port 3 configuration B */

static bit		ETI1	@ 0x33B;		/* TX interrupt enable 1 */
static bit		ERI1	@ 0x33A;		/* RX interrupt enable 1 */
static bit		ETI0	@ 0x339;		/* TX interrupt enable 0 */
static bit		ERI0	@ 0x338;		/* RX interrupt enable 0 */
static bit		EA	@ 0x337;		/* global int. enable */
static bit		ET2	@ 0x334;		/* timer 2 interrupt */
static bit		ET1	@ 0x333;		/* timer 1 interrupt */
static bit		EX1	@ 0x332;		/* external interrupt 1 */
static bit		ET0	@ 0x331;		/* timer 0 interrupt */
static bit		EX0	@ 0x330;		/* external interrupt 0 */
static bit		PD	@ 0x221;		/* power down */
static bit		IDL	@ 0x220;
static bit		SM	@ 0x20F;
static bit		TM	@ 0x20E;
static bit		RS1	@ 0x20D;
static bit		RS0	@ 0x20C;
static bit		IM3	@ 0x20B;
static bit		IM2	@ 0x20A;
static bit		IM1	@ 0x209;
static bit		IM0	@ 0x208;
static bit		S0M0	@ 0x307;
static bit		S0M1	@ 0x306;
static bit		S0M2	@ 0x305;
static bit		R0EN	@ 0x304;
static bit		T0B8	@ 0x303;
static bit		R0B8	@ 0x302;
static bit		TI0	@ 0x301;		/* serial port 0 tx ready */
static bit		RI0	@ 0x300;		/* serial port 0 rx ready */
static bit		FE0	@ 0x30B;
static bit		BR0	@ 0x30A;
static bit		OE0	@ 0x309;
static bit		STINT0	@ 0x308;
static bit		S1M0	@ 0x327;
static bit		S1M1	@ 0x326;
static bit		S1M2	@ 0x325;
static bit		R1EN	@ 0x324;
static bit		T1B8	@ 0x323;
static bit		R1B8	@ 0x322;
static bit		TI1	@ 0x321;		/* serial port 0 tx ready */
static bit		RI1	@ 0x320;		/* serial port 0 rx ready */
static bit		FE1	@ 0x32B;
static bit		BR1	@ 0x32A;
static bit		OE1	@ 0x329;
static bit		STINT1	@ 0x328;
static bit		SWR7	@ 0x356;
static bit		SWR6	@ 0x355;
static bit		SWR5	@ 0x354;
static bit		SWR4	@ 0x353;
static bit		SWR3	@ 0x352;
static bit		SWR2	@ 0x351;
static bit		SWR1	@ 0x350;
static bit		TF2	@ 0x2C7;
static bit		EXF2	@ 0x2C6;
static bit		RCLK0	@ 0x2C5;
static bit		TCLK0	@ 0x2C4;
static bit		RCLK1	@ 0x2CD;
static bit		TCLK1	@ 0x2CC;
static bit		EXEN2	@ 0x2C3;
static bit		TR2	@ 0x2C2;
static bit		CT2	@ 0x2C1;
static bit		CPRL2	@ 0x2C0;
static bit		T2OE	@ 0x2C9;
static bit		DCEN	@ 0x2C8;
static bit		TF1	@ 0x287;
static bit		TR1	@ 0x286;
static bit		TF0	@ 0x285;
static bit		TR0	@ 0x284;
static bit		IE1	@ 0x283;
static bit		IT1	@ 0x282;
static bit		IE0	@ 0x281;
static bit		IT0	@ 0x280;

static bit		T1OE	@ 0x28A;
static bit		T0OE	@ 0x288;
static bit		PRE2	@ 0x2FF;
static bit		PRE1	@ 0x2FE;
static bit		PRE0	@ 0x2FD;
static bit		WDRUN	@ 0x2FA;
static bit		WDTOF	@ 0x2F9;
static bit		WDMOD	@ 0x2F8;

static bit		WR1	@ 0x388;
static bit		RXD1	@ 0x38C;
static bit		TXD1	@ 0x38D;
static bit		T2	@ 0x38E;
static bit		T2EX	@ 0x38F;
static bit		RXD0	@ 0x398;
static bit		TXD0	@ 0x399;
static bit		INT0	@ 0x39A;
static bit		INT1	@ 0x39B;
static bit		T0	@ 0x39C;
static bit		T1	@ 0x39D;
static bit		WR	@ 0x39E;
static bit		RD	@ 0x39F;

/*
 * Interrupt-related
 */


/* Vectors */
				/* Exception vectors. */
#define	IV_BRKPT	0x04	/* breakpoint vector */
#define	IV_TRACE	0x08	/* Trace mode bit set */
#define	IV_STKOVER	0x0C	/* stack overflow */
#define	IV_DIVZERO	0x10	/* divide by zero */
#define	IV_IRET		0x14	/* user mode IRET */

#define IV_TRI0		0x40	/* Trap interrupt vectors. */
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
#define	IV_EX0		0x80	/* Event interrupt vectors. */
#define	IV_T0		0x84
#define	IV_EX1		0x88
#define	IV_T1		0x8C
#define	IV_T2		0x90
#define	IV_RI0		0xA0
#define	IV_TI0		0xA4
#define	IV_RI1		0xA8
#define	IV_TI1		0xAC
#define	IV_SWI1		0x100	/* Software interrupt vectorss */
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

#endif	/* __xa_h__ */
