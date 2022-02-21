/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		UTIL.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990523																		*/
/*	Rev. date:	990902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/
#ifndef _UTIL_H
#define _UTIL_H

extern void NONE(void);
extern byte HexToByte( char s);
extern byte htol( char* s, dword* result);
extern byte otol( char* s, dword* result);
extern byte btol( char* s, dword* result);
extern void dwordtoa( dword val, char* result);
extern void bytetohex( byte val, char *result);
extern void wordtohex( word val, char *result);
extern int Cstrcmp( char * s1, _rom char * s2);

#endif
