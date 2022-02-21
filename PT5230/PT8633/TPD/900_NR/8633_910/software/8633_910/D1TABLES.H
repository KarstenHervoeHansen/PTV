/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		D1TABLES.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980210																		*/
/*	Rev. date:	981002, KEn, DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 981002: Released in SW ver. 3.0														*/
/* 980603: Released in SW ver. 2.0														*/
/* 021113: Modified for TDC moving logo                                    */
/***************************************************************************/

#define	MaxNoOfSignals		7      // JK 8 orig
#define	MaxNoOfLevels		  7      // JK 8 oirg

extern code UC* LogoDataPageTable;

extern code UC* AudioPageTable[MaxNoOfLevels];
extern code UL AudioOffsetTable[MaxNoOfSignals];

extern code UC Audio525[48];
extern code UC Audio625[65];

extern code UC TextTable[408];
