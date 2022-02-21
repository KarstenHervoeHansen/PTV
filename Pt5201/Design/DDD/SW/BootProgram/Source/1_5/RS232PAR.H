/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		RS232PAR.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990521																		*/
/*	Rev. date:	990902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/
#ifndef _RS232PAR_H
#define _RS232PAR_H


enum ParameterTypes	{ P_NoParameter = 0, P_Char, P_DecNum, P_HexDecNum,\
							  P_OctDecNum, P_BinDecNum, P_String, P_Block0,\
							  P_Blockn, P_Expression, P_ASCII };

#define CmdBufferLen	31

extern _near char Cmd[CmdBufferLen+1];

extern _bit NewCmdExecuted;
extern _bit CmdRequest, CmdExecute;

extern _near byte ParameterType;

extern void CmdHandler( char c);

#endif