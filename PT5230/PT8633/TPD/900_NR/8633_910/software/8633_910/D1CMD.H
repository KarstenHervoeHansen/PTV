/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		D1CMD.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980210																		*/
/*	Rev. date:	981002, KEn, DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 981002: Released in SW ver. 3.0														*/
/* 980603: Released in SW ver. 2.0														*/
/***************************************************************************/

extern void UnitAddressCommand();
extern void GOffsetCommand();
extern void MOffsetCommand();
extern void ReadSystemCommand();
extern void ReadOffsetCommand();
extern void GenlockOffsetCommand();
extern void PatternCommand();
extern void EDHCommand();
extern void AudioSignalCommand();
extern void AudioLevelCommand();
extern void AudioGroupCommand();
extern void TextCommand();
extern void TextONCommand();
extern void TextPositionCommand();
extern void TextStyleCommand();
extern void ReadStatusCommand();
extern void GPhaseCommand();
extern void MPhaseCommand();
extern void ResetCommand();
extern void WriteUserTextCommand();
extern void ReadUserTextCommand();
extern void ReadSWVersionCommand();
extern void ReadHWVersionCommand();
extern void ReadHWInformationCommand();
extern void CalculateOffset();
extern void TimeCommand();
extern void DateCommand();
