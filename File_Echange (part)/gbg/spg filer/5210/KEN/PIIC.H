/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1996							*/
/*	Project:		PT5210 VariTime digital sync generator, front plate			*/
/*	Module:		PIIC.H																		*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960215																		*/
/*	Rev. date:	971211, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 970821 Altered Timeout & DataError definitions									*/
/* 970112 Added IIC1ErrorCode & IIC2ErrorCode										*/
/* 961219 Added routines for read and write EEPROM									*/
/***************************************************************************/

#define NoError 			0

#define TxIICTimeout		1
#define TxIICDataError	2

#define RxIICTimeout		3
#define RxIICDataError	4

extern UC IIC1ErrorCode;

extern UC IIC1_Read( UC Address);
extern bit IIC1_Write( UC Address, UC Data);
extern bit IIC1_Write2( UC Address,UC Data, UC Data2);
extern bit IIC1_WrStr( UC Address, UC subadr, char* s);
extern bit IIC1_RdStr( UC Address, UC subadr, char *s);


extern UC IIC2ErrorCode;

extern UC IIC2_Read( UC Address);
extern bit IIC2_Write( UC Address, UC Data);
extern bit IIC2_Write2( UC Address,UC Data, UC Data2);
extern bit IIC2_WrStr( UC Address, UC subadr, char *s);
extern bit IIC2_RdStr( UC Address, UC subadr, char *s);
extern bit IIC2_WrLongInt( UC Address, UC subadr, UL a, UI b);
extern UL IIC2_RdLong( UC Address, UC subadr);
extern UI IIC2_RdInt( UC Address, UC subadr);
