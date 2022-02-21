/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		XADRIVR.H																	*/
/*	Author:		P.H.Seerden, Application note: AN95040								*/
/*	Org. date:	950105, P.H.Seerden, Philips Semiconductors B.V.				*/
/*	Rev. date:	990902, KEn	DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module consists a number of 'local' declarations of the XA51 		*/
/*	driver package.																			*/
/*																									*/
/*	History:																						*/
/***************************************************************************/
#ifndef _XADRIVR_H
#define _XADRIVR_H

extern _near char rxBufHead;
extern _near char rxBufTail;
extern _near char txBufHead;
extern _near char txBufTail;

extern char Udr_ReceiveByte( char *c);
extern byte Udr_FreeSpaceInRxBuf( void);
extern char Udr_SendByte( char ch);

#endif