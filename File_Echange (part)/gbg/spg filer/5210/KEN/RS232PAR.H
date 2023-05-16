/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		RS232PAR.H																	*/
/*	Author:		Kim Engedahl, VDE															*/
/*	Org. date:	961124																		*/
/*	Rev. date:	971212, KEn VDE															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 971212 Altered ResponseRecived to an UC											*/
/* 970723 Added bit: NewCmdExecuted														*/
/***************************************************************************/

enum ParameterTypes	{ P_NoParameter = 0, P_Char, P_DecNum, P_HexDecNum,\
							  P_OctDecNum, P_BinDecNum, P_String, P_Block0,\
							  P_Blockn, P_Expression, P_ASCII };

extern char Cmd[512];

extern bit NewCmdExecuted;
extern bit CmdRequest, CmdExecute;

extern UC ResponseReceived;

extern UI NumSuffix;

extern UC ParameterType;

extern void CmdHandler( char c);
extern void ResponseHandler( char* str);
