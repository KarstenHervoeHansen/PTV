/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		DISP_DRV.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	980616, KE, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/* 980616: Released as 4008 002 06812													*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

extern void InitDisplay();

extern void WriteAddressLN1( UC pos);
extern void WriteAddressLN2( UC pos);

extern void ClearToPosLN1( UC pos);
extern void ClearToPosLN2( UC pos);

extern void ClearRestLN1();
extern void ClearRestLN2();

extern void WriteCodeLN1( char pos, code char* LineTxt);
extern void WriteCodeLN2( char pos, code char* LineTxt);

extern void WriteCode1( code char* LineTxt);
extern void WriteCode2( code char* LineTxt);

extern void WriteTxtLN1( char pos, char* LineTxt);
extern void WriteTxtLN2( char pos, char* LineTxt);

extern void WriteTxt1( char* LineTxt);
extern void WriteTxt2( char* LineTxt);

extern void WriteCharLN1( char pos, char val);
extern void WriteCharLN2( char pos, char val);

extern void WriteChar1( char val);
extern void WriteChar2( char val);

extern void WriteCGChar( UC Address, UC Data);
extern void CharBlinkLN2( UC Pos, UC status);
