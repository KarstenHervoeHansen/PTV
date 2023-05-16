/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1996							*/
/*	Project:		PT5210 VariTime digital sync generator								*/
/*	Module:		XADRIVR.H																	*/
/*	Author:		P.H.Seerden, Application note: AN95040								*/
/*	Org. date:	950105, P.H.Seerden, Philips Semiconductors B.V.				*/
/*	Rev. date:	980616, KEn	DEV															*/
/*	Status:		Version 0.2																	*/
/*																									*/
/*	This module consists a number of 'local' declarations of the XA51 		*/
/*	driver package.																			*/
/*																									*/
/*	History:																						*/
/*	05-01-95		P.H. Seerden, Initial version											*/
/*	04-10-96		KEn, modified version for the XA processor						*/
/* 10-06-97		KEn, removed CTS_OFF, CTS_ON.											*/
/* 06-10-97		KEn, ddded Udr_RS232LoopbackTest()									*/
/* 10-10-97		KEn, removed some external variables								*/
/* 06-16-98		KEn, The return value of Udr_FreeSpaceInRxBuf( void) is now	*/
/*			 		instead of char, (problems with software handshake)			*/
/***************************************************************************/

#define RTS_OFF		INT1 = 1				// RTS output = 1 (P3.3)
#define RTS_ON			INT1 = 0				// RTS output = 0

extern near char rxBufHead;
extern near char rxBufTail;
extern near char txBufHead;
extern near char txBufTail;

extern char Udr_ReceiveByte( char *c);
extern UC Udr_FreeSpaceInRxBuf( void);
extern char Udr_SendByte( char ch);

extern bit GetParity( char ch);
extern void Set_7Bit_Parity( char* ch);
extern void Set_8Bit_Parity( char ch);

extern bit Udr_RS232LoopbackTest( void);
