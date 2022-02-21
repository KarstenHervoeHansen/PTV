
#ifndef __BORLANDC__
	#define _ROM_	code
#else
#define _ROM_
	#define _inline
	#define code
	#define _at(abs_add)
	#define _sfrbit  char
	#define _atbit(Add,bit7)


	extern char ET1;
	 
	extern char EA;
	extern char BCR;
	extern char BTRH;
	extern char BTRL;
	extern char CS;
	extern char DS;
	extern char ES;
	extern char IEH;
	extern char IEL;
	extern char IPA0;
	extern char IPA1;
	extern char IPA2;
	extern char IPA4;
	extern char IPA5;
	extern char P0;
	extern char P1;
	extern char P2;
	extern char P3;
	extern char P0CFGA;
	extern char P1CFGA;
	extern char P2CFGA;
	extern char P3CFGA;
	extern char P0CFGB;
	extern char P1CFGB;
	extern char P2CFGB;
	extern char P3CFGB;
	extern char PCON;
	extern char PSW51;
	extern char PSWH;
	extern char PSWL;
	extern char RTH0;
	extern char RTH1;
	extern char RTL0;
	extern char RTL1;
	extern char S0ADDR;
	extern char S0ADEN;
	extern char S0BUF;
	extern char S0CON;
	extern char S0STAT;
	extern char S1ADDR;
	extern char S1ADEN;
	extern char S1BUF;
	extern char S1CON;
	extern char S1STAT;
	extern char SCR;
	extern char SSEL;
	extern char TH0;
	extern char TH1;
	extern char SWE;
	extern char TL0;
	extern char SWR;
	extern char   TL1;
	extern char   T2CON;
	extern char   TMOD;
	extern char   T2MOD;
	extern char   TSTAT;
	extern char   TH2;
	extern char   WDCON;
	extern char   TL2;
	extern char   WDL;
	extern char   T2CAPH;
	extern char   WFEED1;
	extern char   T2CAPL;
	extern char   WFEED2;
	extern char   TCON;
	extern char    A0;
	extern char    A1;
	extern char    A2;
	extern char    A3;
	extern char    AC;
	extern char    AD0;
	extern char    AD1;
	extern char    AD2;
	extern char    AD3;
	extern char    AD4;
	extern char    AD5;
	extern char    AD6;
	extern char    AD7;
	extern char    BR0;
	extern char    BR1;
	extern char    C;
	extern char    CP_RL2;
	extern char    C_T2	;
	extern char    DCEN;
	extern char    EA;
	extern char    ERI0;
	extern char    ERI1;
	extern char    ESWEN;
	extern char    ET0;
	extern char    ET1;
	extern char    ET2;
	extern char    ETI0;
	extern char    ETI1;
	extern char    EX0;
	extern char    EX1;
	extern char    EXEN2;
	extern char    EXF2;
	extern char    FE0;
	extern char    FE1;
	extern char    IDL;
	extern char    IE0;
	extern char    IE1;
	extern char    IM0;
	extern char    IM1;
	extern char    IM2;
	extern char    IM3;
	extern char    INT0;
	extern char    INT1;
	extern char    IT0;
	extern char    IT1;
	extern char    N;
	extern char    OE0;
	extern char    OE1;
	extern char    P2_0;
	extern char    P2_1;
	extern char    P2_2;
	extern char    P2_3;
	extern char    P2_4;
	extern char    P2_5;
	extern char    P2_6;
	extern char    P2_7;
	extern char    PD;
	extern char    PRE0;
	extern char    PRE1;
	extern char    PRE2;
	extern char    R0SEG;
	extern char    R1SEG;
	extern char    R2SEG;
	extern char    R3SEG;
	extern char    R4SEG;
	extern char    R5SEG;
	extern char    R6SEG;
	extern char    RB8_0;
	extern char    RB8_1;
	extern char    RCLK0;
	extern char    RCLK1;
	extern char    RD;
	extern char    REN_0;
	extern char    REN_1;
	extern char    RI_0;
	extern char    RI_1;
	extern char    RS0;
	extern char    RS1;
	extern char    RXD0;
	extern char    RXD1;
	extern char    SM;
	extern char    SM0_0;
	extern char    SM0_1;
	extern char    SM1_0;
	extern char    SM1_1;
	extern char    SM2_0;
	extern char    SM2_1;
	extern char    STINT0;
	extern char    STINT1;
	extern char    SWR1;
	extern char    SWR2;
	extern char    SWR3;
	extern char    SWR4;
	extern char    SWR5;
	extern char    SWR6;
	extern char    SWR7;
	extern char    T0;
	extern char    T0OE;
	extern char    T1;
	extern char    T1OE;
	extern char    T2;
	extern char    T2EX;
	extern char    T2OE;
	extern char    TB8_0;
	extern char    TB8_1;
	extern char    TCLK0;
	extern char    TCLK1;
	extern char    TF0;
	extern char    TF1;
	extern char    TF2;
	extern char    TI_0;
	extern char    TI_1;
	extern char    TM;
	extern char    TR0;
	extern char    TR1;
	extern char    TR2;
	extern char    TXD0;
	extern char    TXD1;
	extern char    V;
	extern char    WDRUN;
	extern char    WDTOF;
	extern char    WR;
	extern char    WRH;
	extern char    Z;

#endif


