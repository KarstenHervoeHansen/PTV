/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8602/3 SDI Digital test signal generator						*/
/*	Module:		D1CMD.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960614																		*/
/*	Rev. date:	980929, KEn DEV															*/
/*	Status:		Version 1.1																	*/
/*																									*/
/*	Changes:																						*/
/* 980929: Added Reference to Read_HW_Information_Command()						*/
/***************************************************************************/

extern void Unit_Address_Command();
extern void G_Offset_Command();
extern void M_Offset_Command();
extern void Read_System_Command();
extern void Read_Offset_Command();
extern void Genlock_Offset_Command();
extern void Pattern_Command();
extern void EDH_Command();
extern void Audio_Signal_Command();
extern void Audio_Level_Command();
extern void Text_ON_Command();
extern void Text_Command();
extern void Read_Status_Command();
extern void G_Phase_Command();
extern void M_Phase_Command();
extern void Reset_Command();
extern void Write_User_Text_Command();
extern void Read_User_Text_Command();
extern void Read_SW_Version_Command();
extern void Read_HW_Version_Command();
extern void Read_HW_Information_Command();
extern void Calculate_Offset();
