/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8602/3 SDI Digital test signal generator						*/
/*	Module:		D1TABLES.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960614																		*/
/*	Rev. date:	970512, KEn DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	Changes:																						*/
/***************************************************************************/

#define	MaxNoOfSignals		8
#define	MaxNoOfLevels		8

extern code UC* SoundPageTable[MaxNoOfSignals];
extern code UL SoundOffsetTable[MaxNoOfSignals];

extern code UC Sound525[48];
extern code UC Sound625[65];

extern code UC TextTable[128];
