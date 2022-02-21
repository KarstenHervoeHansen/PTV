/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		SERIAL1.H																	*/
/*	Author:		Preben Christiansen/Kim Engedahl, DEV								*/
/*	Org. date:	960903																		*/
/*	Rev. date:	980616, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/* Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

#define V24Ok				0

#define TxV24Timeout		1
#define TxV24Checksum	2

#define RxV24Timeout		3
#define RxV24Checksum	4

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
extern void SndBroadcast( char* A, char *st);

extern UI RecInt( UC address, char* cmd, UC val, UC* error);
extern UL RecLong( UC address, char* cmd, UC val, UC* error);
extern char* RecAsc( UC address, char* cmd, UC val, UC* error);
extern char* RecStr( UC address, char* cmd, UC val, UC* error);
