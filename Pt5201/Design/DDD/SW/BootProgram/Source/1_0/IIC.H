/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		IIC.H																			*/
/*	Author:		Preben Christiansen, Kim Engedahl, DEV								*/
/*	Org. date:	960215																		*/
/*	Rev. date:	980902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/

#define NoIICError 		0

#define TxIICTimeout		1
#define TxIICDataError	2

#define RxIICTimeout		3
#define RxIICDataError	4


extern byte IICErrorCode;

extern byte RdFrontIIC( byte Address);
extern _bit WrFrontIIC( byte Address, byte Data);
extern _bit WrFrontIIC2( byte Address, byte Data, byte Data2);
