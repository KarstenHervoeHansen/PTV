/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		SERIAL1.H																	*/
/*	Author:		Preben Christiansen/Kim Engedahl, DEV								*/
/*	Org. date:	960903																		*/
/*	Rev. date:	971126, KEn DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/* Changes:																						*/
/* 971126 Added parameter val to RecInt() & RecLong								*/
/* 971031 Added RecStr()																	*/
/* 971030 Added SndInt3()																	*/
/* 971027 Modified RecAsc. Removed SndRequest										*/
/* 971024 Added SndV24Checksum, SndV24Timeout, RecV24Checksum and 			*/
/*			 SndV24Timeout. Removed SndRequest_n, SndAsc_n (KEn)					*/
/* 971023 Added RecInt, RecLong & RecAsc, (KEn)										*/
/* 971022 Removed TxV24 and RxV24 definitions, (KEn)								*/
/* 970821 Altered Timeout & Checksum definitions, (KEn)							*/
/* 970218 Added SndInt2 function (KEn)													*/
/* 970115 Added v24ok, v24checksum & v24timeout, (KEn)							*/
/* 961209 Changed AnswerCheck from bit to UC											*/
/* 961111 Added SndRequest_n(), SndSpecial(), SndAsc_n()							*/
/* 961101 Added UC AnswerBuf[]; bit AnswerCheck(void); near UC rstate;		*/
/***************************************************************************/

#define UI unsigned int
#define UC unsigned char
#define UL unsigned long

#define v24ok				0
#define v24checksum		1
#define v24timeout		2

#define SndV24Checksum	1
#define SndV24Timeout	2
#define RecV24Checksum	3
#define RecV24Timeout	4

extern void Serial1_init( void);
extern interrupt void Serial1_int( void);

extern UC SndChar( UC adr, char* A, UC x) ;
extern UC SndInt( UC adr, char* A, UI x);
extern UC SndInt2( UC adr, char *A, UI x, UI y);
extern UC SndInt3( UC adr, char *A, UI x, UI y, UI z);
extern UC SndLong( UC adr, char* A,UL x);
extern UC SndVoid( UC adr, char* A);
extern UC SndAsc( UC adr, char* A, UC n, const char *st);
extern UC SndSpecial( UC adr, char* A, char C);

extern UI RecInt( UC address, char* cmd, UC val, UC* error);
extern UL RecLong( UC address, char* cmd, UC val, UC* error);
extern char* RecAsc( UC address, char* cmd, UC val, UC* error);
extern char* RecStr( UC address, char* cmd, UC val, UC* error);
