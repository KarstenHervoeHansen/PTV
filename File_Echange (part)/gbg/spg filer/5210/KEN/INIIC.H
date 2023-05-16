/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1996							*/
/*	Project:		PT5210 VariTime digital sync generator, front plate			*/
/*	Module:		INIIC.H																		*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960703																		*/
/*	Rev. date:	971211, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 970112: Added IICErrorCode																*/
/***************************************************************************/

#define NoError 			0
#define AddressTimeout	5
#define DataError			6

extern UC IICErrorCode;

extern UC Rd_Front_IIC( UC Address);
extern bit Wr_Front_IIC( UC Address, UC Data);
extern bit Wr_Front_IIC2( UC Address, UC Data, UC Data2);
