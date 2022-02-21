/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		IIC.H																			*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960215																		*/
/*	Rev. date:	980720, KEn, DEV															*/
/*	Status:		Version 1.1																	*/
/*																									*/
/*	Changes:																						*/
/* 980720: Added IIC1WriteN()																*/
/* 980703: Added IIC1ReadN()																*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

#define NoIICError 		0

#define TxIICTimeout		1
#define TxIICDataError	2

#define RxIICTimeout		3
#define RxIICDataError	4


extern UC IICErrorCode;

extern UC RdFrontIIC( UC Address);
extern bit WrFrontIIC( UC Address, UC Data);
extern bit WrFrontIIC2( UC Address, UC Data, UC Data2);


extern UC IIC1ErrorCode;

extern UC IIC1Read( UC Address);
extern bit IIC1ReadN( UC Address, UC n, UC* s);
extern bit IIC1Write( UC Address, UC Data);
extern bit IIC1Write2( UC Address,UC Data, UC Data2);
extern bit IIC1WriteN( UC Address, UC noOfBytes, UC* Data);
extern bit IIC1WrStr( UC Address, UC subadr, char* s);
extern bit IIC1RdStr( UC Address, UC subadr, char *s);


extern UC IIC2ErrorCode;

extern UC IIC2Read( UC Address);
extern bit IIC2Write( UC Address, UC Data);
extern bit IIC2Write2( UC Address,UC Data, UC Data2);
extern bit IIC2WrStr( UC Address, UC subadr, char *s);
extern bit IIC2RdStr( UC Address, UC subadr, char *s);
extern bit IIC2WrLongInt( UC Address, UC subadr, UL a, UI b);
extern UL IIC2RdLong( UC Address, UC subadr);
extern UI IIC2RdInt( UC Address, UC subadr);
