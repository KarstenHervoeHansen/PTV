/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		D1UTIL.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980119																		*/
/*	Rev. date:	981002, KEn, DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 981002: Released as SW ver. 3.0														*/
/* 980603: Released as SW ver. 2.0														*/
/***************************************************************************/

extern UC stringlen( char* textptr);

extern void ClearOSDLine( UC lineno);

extern void InsertOSDText( UC lineno, UC xpos, char* textptr);
extern void InsertOSDTime();
extern void InsertOSDDate();

extern void WriteOSDText( UC lineno);

extern void UpdateTextEnable();
extern void UpdateTextStyle();
extern void UpdatePattern( UC pattern);

extern void CopyLogoRAM();
extern void CopyAudioRAM();

extern void Wait10ms();
void control_synchro(void);


