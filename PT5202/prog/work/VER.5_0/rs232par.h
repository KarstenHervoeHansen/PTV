/****************************************************************************/
/* MODULE:                                                                  */
/*   rs232par.h - 																												  */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*  NONE                                                                    */
/*                                                                          */
/* TASKS:                                                                   */
/*  NONE                                                                    */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c)
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

enum ParameterTypes	{
	P_NoParameter = 0,
	P_Char,
	P_DecNum,
	P_HexDecNum,
	P_OctDecNum,
	P_BinDecNum,
	P_String,
	P_Block0,
  P_Blockn,
  P_Expression,
  P_ASCII
};

extern char Cmd[512];

extern bit NewCmdExecuted;
extern bit CmdRequest, CmdExecute;

extern UC ResponseReceived;

extern UI NumSuffix;

extern UC ParType[10];

extern UC ParCnt;

extern void CmdHandler( char c);
