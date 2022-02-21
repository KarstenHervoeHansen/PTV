/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:   RS232PAR.H                                  */
/*  Author:   Kim Engedahl, DEV                             */
/*  Org. date:  961124                                    */
/*  Rev. date:  980616, KEn, DEV                              */
/*  Status:   Version 1.0                                 */
/*                                                  */
/*  Changes:                                            */
/* 980616: Released as 4008 002 06812                         */
/* 980514: Released as 4008 002 06811                         */
/***************************************************************************/

enum ParameterTypes { P_NoParameter = 0, P_Char, P_DecNum, P_HexDecNum,\
                P_OctDecNum, P_BinDecNum, P_String, P_Block0,\
                P_Blockn, P_Expression, P_ASCII };

extern char Cmd[512];

extern bit NewCmdExecuted;
extern bit CmdRequest, CmdExecute;

extern volatile UC ResponseReceived;

extern UI NumSuffix;

extern UC ParameterType;

extern void CmdHandler( char c);
extern void ResponseHandler( char* str);
