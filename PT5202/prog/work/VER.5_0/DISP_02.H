/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:    DISP_DRV.H                                  */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  960703                                    */
/*  Rev. date:  980616, KE, DEV                              */
/*  Status:    Version 1.0                                  */
/*                                                  */
/*  Changes:                                            */
/* 980616: Released as 4008 002 06812                          */
/* 980514: Released as 4008 002 06811                          */
/***************************************************************************/

void InitDisplay(void);
bit  DisplayBusyTest(void);
void WriteAddressLN1(UC);
void WriteAddressLN2(UC);

void ClearToPosLN1( UC);
void ClearToPosLN2( UC);

void ClearRestLN1(void);
void ClearRestLN2(void);

void WriteCodeLN1( char pos, code char* LineTxt);
void WriteCodeLN2( char pos, code char* LineTxt);

void WriteCode1( code char* LineTxt);
void WriteCode2( code char* LineTxt);

void WriteTxtLN1( char pos, char* LineTxt);
void WriteTxtLN2( char pos, char* LineTxt);

void WriteTxt1( char* LineTxt);
void WriteTxt2( char* LineTxt);

void WriteCharLN1( char pos, char val);
void WriteCharLN2( char pos, char val);

void WriteChar1( char val);
void WriteChar2( char val);

void WriteCGChar( UC Address, UC Data);
void CharBlinkLN2( UC Pos, UC status);

void waitms(UI);

