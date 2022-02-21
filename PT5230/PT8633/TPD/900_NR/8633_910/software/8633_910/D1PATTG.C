/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		D1PATTG.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980210																		*/
/*	Rev. date:	990623, KEn, DEV															*/
/*	Status:		Version 2.4																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*	Changes:																						*/
/* 990623: Added Window 10% & 15kHz Bl/Wh												*/
/*	990323: PT8633: Increased array of G_FuBK16x9[36] to support new audio	*/
/*	990322: PT8633: Increased array of G_FuBK4x3[36] to support new audio	*/
/*	990322: PT8633: Increased array of G_Philips16x9[56] to support new		*/
/*				audio																				*/
/* 990304: FuBK4x3 is now available in PT8632										*/
/* 990219: G_CBar75Red[8] changed to G_EBUCBRed[8]	i.e. 75% CBar in have	*/
/*				been changed to EBU CBar for this splitfiled pattern.				*/
/* 981109: PT8633: Updated G_Philips4x3[36] to support new motion picture. */
/* 981002: Released in SW ver. 3.0														*/
/* 980924: Added for PT8633: CCIR17, 18, 330 & 331.								*/
/* 980603: Released in SW ver. 2.0														*/
/* 980525: Added customized pattern														*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <intrpt.h>

#include "version.h"

#include "d1gener.h"
#include "d1patt.h"


#if PT8633Version
	//************************************************************************
	// PATTERN DEFINITIONS FOR THE PT8633 GENERATOR
	//************************************************************************
	code struct PATTERN_ITEM* code G_PatternTable[68] = {
		&G_Dummy,
		&G_EBUCBar[1],
		&G_EBUCBarITU[1],
		&G_CBar100[1],
		&G_CBar75Grey[1],
		&G_EBUCBarRed[1],
		&G_CheckField[1],
		&G_ShallowRamp[1],
		&G_TimingTest[1],
		&G_Black[1],
		&G_Window15[1],
		&G_Window20[1],
		&G_Window100[1],
		&G_CrossHatch[1],
		&G_PLUGE[1],
		&G_Multiburst[1],
		&G_Red75[1],
		&G_Philips4x3[1],
		&G_Philips4x3woAPAL[1],
		&G_CCIR17[1],
		&G_AnalogBlanking[1],
		&G_Multipulse[1],
		&G_Sinxx[1],
		&G_DigitalGrey[1],
		&G_FieldTimingChk[1],
		&G_ValidRamp[1],
		&G_Stair5[1],
		&G_Stair10[1],
		&G_LuminanceSweep[1],
		&G_YCrCbSweep[1],
		&G_Flat100[1],
		&G_BowTie[1],
		&G_AltBlWh[1],
		&G_EndOfLine[1],
		&G_WhiteEndOfLine[1],
		&G_BlueEndOfLine[1],
		&G_RedEndOfLine[1],
		&G_YellowEndOfLine[1],
		&G_CyanEndOfLine[1],
		&G_LuminanceRamp[1],
		&G_LimitRamp[1],
		&G_PulseBar[1],
		&G_YellowGreyRamp[1],
		&G_GreyBlueRamp[1],
		&G_CyanGreyRamp[1],
		&G_GreyRedRamp[1],
		&G_CbYCrYRamp[1],
		&G_Philips16x9[1],
		&G_FuBK4x3[1],
		&G_FuBK16x9[1],
		&G_SafeArea[1],
		&G_VMT01[1],
		&G_Dummy,
		&G_FieldSquareWave[1],
		&G_ModulatedStair5[1],
		&G_Customized1[1],
		&G_Customized2[1],
		&G_Customized3[1],
		&G_Customized4[1],
		&G_Customized5[1],
		&G_Customized6[1],
		&G_Customized7[1],
		&G_Customized8[1],
		&G_CCIR18[1],
		&G_CCIR330[1],
		&G_CCIR331[1],
		&G_Window10[1],
		&G_kHz15BlWh[1]
	};
#else
	//************************************************************************
	// PATTERN DEFINITIONS FOR THE PT8632 GENERATOR
	//************************************************************************
	code struct PATTERN_ITEM* code G_PatternTable[68] = {
		&G_Dummy,
		&G_EBUCBar[1],
		&G_EBUCBarITU[1],
		&G_CBar100[1],
		&G_CBar75Grey[1],
		&G_EBUCBarRed[1],
		&G_CheckField[1],
		&G_ShallowRamp[1],
		&G_TimingTest[1],
		&G_Black[1],
		&G_Window15[1],
		&G_Window20[1],
		&G_Window100[1],
		&G_CrossHatch[1],
		&G_PLUGE[1],
		&G_Multiburst[1],
		&G_Red75[1],
		&G_Philips4x3[1],
		&G_Philips4x3woAPAL[1],
		&G_Dummy,
		&G_AnalogBlanking[1],
		&G_Multipulse[1],
		&G_Dummy,
		&G_DigitalGrey[1],
		&G_FieldTimingChk[1],
		&G_ValidRamp[1],
		&G_Stair5[1],
		&G_Dummy,
		&G_LuminanceSweep[1],
		&G_Dummy,
		&G_Dummy,
		&G_BowTie[1],
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_LuminanceRamp[1],
		&G_LimitRamp[1],
		&G_PulseBar[1],
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_FuBK4x3[1],
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_FieldSquareWave[1],
		&G_ModulatedStair5[1],
		&G_Customized1[1],
		&G_Customized2[1],
		&G_Customized3[1],
		&G_Customized4[1],
		&G_Customized5[1],
		&G_Customized6[1],
		&G_Customized7[1],
		&G_Customized8[1],
		&G_Dummy,
		&G_Dummy,
		&G_Dummy,
		&G_Window10[1],
		&G_kHz15BlWh[1]
	};
#endif

//************************************************************************
// DEFINITION OF PATTERN ARRAYS IN SDI625
//************************************************************************
code struct PATTERN_ITEM G_Dummy;

code struct PATTERN_ITEM G_EBUCBar[6] = {
	SyncLineB,  			 -22, 0, &G_EBUCBar[1],
	EBUCB_625,	 			-288, 0, &G_EBUCBar[2],
	SyncLineB,  			  -2, 0, &G_EBUCBar[3],

	SyncLineA,  			 -23, 0, &G_EBUCBar[4],
	EBUCB_625+1,		  	-288, 0, &G_EBUCBar[5],
	SyncLineA,  			  -2, 0, &G_EBUCBar[0],
};

code struct PATTERN_ITEM G_EBUCBarITU[6] = {
	SyncLineB,				-22,	0, &G_EBUCBarITU[1],
	EBUCB_ITU,				-288,	0, &G_EBUCBarITU[2],
	SyncLineB,  			-2,	0, &G_EBUCBarITU[3],

	SyncLineA,  			-23,	0, &G_EBUCBarITU[4],
	EBUCB_ITU+1,			-288,	0, &G_EBUCBarITU[5],
	SyncLineA,  			-2,	0, &G_EBUCBarITU[0],
};

code struct PATTERN_ITEM G_CBar100[6] = {
	SyncLineB,  			-22,	0, &G_CBar100[1],
	CB100_625,	 			-288,	0, &G_CBar100[2],
	SyncLineB,  			-2,	0, &G_CBar100[3],

	SyncLineA,  			-23,	0, &G_CBar100[4],
	CB100_625+1,			-288,	0, &G_CBar100[5],
	SyncLineA,  			-2,	0, &G_CBar100[0],
};

code struct PATTERN_ITEM G_CBar75Grey[8] = {
	SyncLineB,  			-22,	0, &G_CBar75Grey[1],
	CB75STD_625, 			-192,	0, &G_CBar75Grey[2],
	Grey75_625,				-96,	0, &G_CBar75Grey[3],
	SyncLineB,  			-2,	0, &G_CBar75Grey[4],

	SyncLineA,  			-23,	0, &G_CBar75Grey[5],
	CB75STD_625+1,			-192,	0, &G_CBar75Grey[6],
	Grey75_625+1,			-96,	0, &G_CBar75Grey[7],
	SyncLineA, 				-2,	0, &G_CBar75Grey[0],
};

code struct PATTERN_ITEM G_EBUCBarRed[8] = {
	SyncLineB,  			-22,	0, &G_EBUCBarRed[1],
	EBUCB_625,				-192,	0, &G_EBUCBarRed[2],
	Red75_625,				-96,	0, &G_EBUCBarRed[3],
	SyncLineB, 				-2,	0, &G_EBUCBarRed[4],

	SyncLineA,				-23,	0, &G_EBUCBarRed[5],
	EBUCB_625+1,			-192,	0, &G_EBUCBarRed[6],
	Red75_625+1,			-96,	0, &G_EBUCBarRed[7],
	SyncLineA,				-2,	0, &G_EBUCBarRed[0],
};

code struct PATTERN_ITEM G_CheckField[9] = {
	SyncLineB,  			-22,	0, &G_CheckField[1],
	SDITestSignalLine1,	-1,	0, &G_CheckField[2],
	SDITestSignalLine2,	-143,	0, &G_CheckField[3],
	SDISignalLine3, 		-144,	0, &G_CheckField[4],
	SyncLineB, 				-2,	0, &G_CheckField[5],

	SyncLineA,				-23,	0, &G_CheckField[6],
	SDITestSignalLine2+1,-144,	0, &G_CheckField[7],
	SDISignalLine3+1,		-144,	0, &G_CheckField[8],
	SyncLineA, 				-2,	0, &G_CheckField[0],
};

code struct PATTERN_ITEM G_ShallowRamp[6] = {
	SyncLineB,				-22,	0, &G_ShallowRamp[1],
	ShallowRamp_625,		-288,	0, &G_ShallowRamp[2],
	SyncLineB,				-2,	0, &G_ShallowRamp[3],

	SyncLineA,				-23,	0, &G_ShallowRamp[4],
	ShallowRamp_625+1,	-288,	0, &G_ShallowRamp[5],
	SyncLineA,				-2,	0, &G_ShallowRamp[0],
};

code struct PATTERN_ITEM G_TimingTest[10] = {
	SyncLineB,  			-22,	0, &G_TimingTest[1],
	WhiteLine_625,			-1,	0, &G_TimingTest[2],
	DigitalTiming_625,	-286,	0, &G_TimingTest[3],
	WhiteLine_625,			-1,	0, &G_TimingTest[4],
	SyncLineB,				-2,	0, &G_TimingTest[5],

	SyncLineA,				-23,	0, &G_TimingTest[6],
	WhiteLine_625+1,		-1,	0, &G_TimingTest[7],
	DigitalTiming_625+1,	-286,	0, &G_TimingTest[8],
	WhiteLine_625+1,		-1,	0, &G_TimingTest[9],
	SyncLineA,  			-2,	0, &G_TimingTest[0],
};

code struct PATTERN_ITEM G_Black[6] = {
	SyncLineB,  			 -22, 0, &G_Black[1],
	SyncLineC,	 			-288, 0, &G_Black[2],
	SyncLineB,  			  -2, 0, &G_Black[3],

	SyncLineA,  			 -23, 0, &G_Black[4],
	SyncLineD,		  		-288, 0, &G_Black[5],
	SyncLineA,  			  -2, 0, &G_Black[0],
};

code struct PATTERN_ITEM G_Window15[20] = {
	SyncLineB, 				-22,	0, &G_Window15[1],
	SyncLineC,				-24,	0, &G_Window15[2],
	Window50Marker,		-1,	0, &G_Window15[3],
	MarkerLine,				-1,	0, &G_Window15[4],
	SyncLineC,				-1,	0, &G_Window15[5],
	Marker15,				-1,	0, &G_Window15[6],
	SyncLineC, 				-44,	0, &G_Window15[7],
	Window15,				-144,	0, &G_Window15[8],
	SyncLineC,				-72,	0, &G_Window15[9],
	SyncLineB,				-2,	0, &G_Window15[10],

	SyncLineA,				-23,	0, &G_Window15[11],
	SyncLineD,				-24,	0, &G_Window15[12],
	Window50Marker+1,		-1,	0, &G_Window15[13],
	MarkerLine+1,			-1,	0, &G_Window15[14],
	SyncLineD,				-1,	0, &G_Window15[15],
	Marker15+1,				-1,	0, &G_Window15[16],
	SyncLineD,				-44,	0, &G_Window15[17],
	Window15+1,				-144,	0, &G_Window15[18],
	SyncLineD,				-72,	0, &G_Window15[19],
	SyncLineA,				-2,	0, &G_Window15[0],
};

code struct PATTERN_ITEM G_Window20[20] = {
	SyncLineB, 				-22,	0, &G_Window20[1],
	SyncLineC,				-24,	0, &G_Window20[2],
	Window50Marker,		-1,	0, &G_Window20[3],
	MarkerLine, 			-1,	0, &G_Window20[4],
	SyncLineC,				-1,	0, &G_Window20[5],
	Marker20,				-1,	0, &G_Window20[6],
	SyncLineC, 				-44,	0, &G_Window20[7],
	Window20,				-144,	0, &G_Window20[8],
	SyncLineC,				-72,	0, &G_Window20[9],
	SyncLineB,				-2,	0, &G_Window20[10],

	SyncLineA,				-23,	0, &G_Window20[11],
	SyncLineD,				-24,	0, &G_Window20[12],
	Window50Marker+1,		-1,	0, &G_Window20[13],
	MarkerLine+1,			-1,	0, &G_Window20[14],
	SyncLineD,				-1,	0, &G_Window20[15],
	Marker20+1,				-1,	0, &G_Window20[16],
	SyncLineD,				-44,	0, &G_Window20[17],
	Window20+1,				-144,	0, &G_Window20[18],
	SyncLineD,				-72,	0, &G_Window20[19],
	SyncLineA, 				-2,	0, &G_Window20[0],
};

code struct PATTERN_ITEM G_Window100[20] = {
	SyncLineB,				-22,	0, &G_Window100[1],
	SyncLineC,				-24,	0, &G_Window100[2],
	Window50Marker,		-1,	0, &G_Window100[3],
	MarkerLine, 			-1,	0, &G_Window100[4],
	SyncLineC,				-1,	0, &G_Window100[5],
	Marker100,				-1,	0, &G_Window100[6],
	SyncLineC,				-44,	0, &G_Window100[7],
	Window100,				-144,	0, &G_Window100[8],
	SyncLineC,				-72,	0, &G_Window100[9],
	SyncLineB,				-2,	0, &G_Window100[10],

	SyncLineA,				-23,	0, &G_Window100[11],
	SyncLineD,				-24,	0, &G_Window100[12],
	Window50Marker+1,		-1,	0, &G_Window100[13],
	MarkerLine+1,			-1,	0, &G_Window100[14],
	SyncLineD,				-1,	0, &G_Window100[15],
	Marker100+1,			-1,	0, &G_Window100[16],
	SyncLineD,				-44,	0, &G_Window100[17],
	Window100+1,			-144,	0, &G_Window100[18],
	SyncLineD,				-72,	0, &G_Window100[19],
	SyncLineA,				-2,	0, &G_Window100[0],
};

code struct PATTERN_ITEM G_CrossHatch[62] = {
	SyncLineB,				-22,	0, &G_CrossHatch[1],
	CrossHatch19_625,		-7,	0, &G_CrossHatch[2],
	WhiteLine_625,			-1,	0, &G_CrossHatch[3],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[4],
	WhiteLine_625,			-1,	0, &G_CrossHatch[5],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[6],
	WhiteLine_625,			-1,	0, &G_CrossHatch[7],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[8],
	WhiteLine_625,			-1,	0, &G_CrossHatch[9],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[10],
	WhiteLine_625,			-1,	0, &G_CrossHatch[11],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[12],
	WhiteLine_625,			-1,	0, &G_CrossHatch[13],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[14],
	WhiteLine_625,			-1,	0, &G_CrossHatch[15],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[16],
	WhiteLine_625,			-1,	0, &G_CrossHatch[17],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[18],
	WhiteLine_625,			-1,	0, &G_CrossHatch[19],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[20],
	WhiteLine_625,			-1,	0, &G_CrossHatch[21],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[22],
	WhiteLine_625,			-1,	0, &G_CrossHatch[23],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[24],
	WhiteLine_625,			-1,	0, &G_CrossHatch[25],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[26],
	WhiteLine_625,			-1,	0, &G_CrossHatch[27],
	CrossHatch19_625,		-20,	0, &G_CrossHatch[28],
	WhiteLine_625,			-1,	0, &G_CrossHatch[29],
	CrossHatch19_625,		-7,	0, &G_CrossHatch[30],
	SyncLineB,				-2,	0, &G_CrossHatch[31],

	SyncLineA,				-23,	0, &G_CrossHatch[32],
	CrossHatch19_625+1,	-7,	0, &G_CrossHatch[33],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[34],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[35],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[36],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[37],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[38],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[39],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[40],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[41],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[42],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[43],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[44],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[45],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[46],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[47],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[48],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[49],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[50],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[51],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[52],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[53],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[54],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[55],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[56],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[57],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[58],
	CrossHatch19_625+1,	-20,	0, &G_CrossHatch[59],
	WhiteLine_625+1,		-1,	0, &G_CrossHatch[60],
	CrossHatch19_625+1,	-7,	0, &G_CrossHatch[61],
	SyncLineA,				-2,	0, &G_CrossHatch[0],
};

code struct PATTERN_ITEM G_PLUGE[16] = {
	SyncLineB,				-22,	0, &G_PLUGE[1],
	SyncLineC,				-38,	0, &G_PLUGE[2],
	PlugeWhite_625, 		-53,	0, &G_PLUGE[3],
	PlugeGreyC_625,		-53,	0, &G_PLUGE[4],
	PlugeGreyD_625,		-53,	0, &G_PLUGE[5],
	PlugeGreyE_625,		-53,	0, &G_PLUGE[6],
	SyncLineC,				-38,	0, &G_PLUGE[7],
	SyncLineB,				-2,	0, &G_PLUGE[8],

	SyncLineA,				-23,	0, &G_PLUGE[9],
	SyncLineD,				-38,	0, &G_PLUGE[10],
	PlugeWhite_625+1,		-53,	0, &G_PLUGE[11],
	PlugeGreyC_625+1,		-53,	0, &G_PLUGE[12],
	PlugeGreyD_625+1,		-53,	0, &G_PLUGE[13],
	PlugeGreyE_625+1,		-53,	0, &G_PLUGE[14],
	SyncLineD,				-38,	0, &G_PLUGE[15],
	SyncLineA,				-2,	0, &G_PLUGE[0],
};

code struct PATTERN_ITEM G_Multiburst[6] = {
	SyncLineB,				-22,	0, &G_Multiburst[1],
	Chroma_MultiBurst,	-288,	0, &G_Multiburst[2],
	SyncLineB,				-2,	0, &G_Multiburst[3],

	SyncLineA,				-23,	0, &G_Multiburst[4],
	Chroma_MultiBurst+1,	-288,	0, &G_Multiburst[5],
	SyncLineA,				-2,	0, &G_Multiburst[0],
};

code struct PATTERN_ITEM G_Red75[6] = {
	SyncLineB,  			-22,	0, &G_Red75[1],
	Red75_625,				-288,	0, &G_Red75[2],
	SyncLineB, 				-2,	0, &G_Red75[3],

	SyncLineA,				-23,	0, &G_Red75[4],
	Red75_625+1,			-288,	0, &G_Red75[5],
	SyncLineA,				-2,	0, &G_Red75[0],
};

#if PT8633Version
	code struct PATTERN_ITEM G_Philips4x3[36] = {
		SyncLineB,  			-22,	0, &G_Philips4x3[1],
		CounterType,			-134,	1, &G_Philips4x3[2],		// 1: Normal
		CounterType,			-20,	2, &G_Philips4x3[3],		// 2: Date/Time+268
		CounterType,			-21,	3, &G_Philips4x3[4],		// 3: PLUGE+308
		CounterType,			-21,	4, &G_Philips4x3[5],		// 4: Normal+350
		CounterType,			-21,	5, &G_Philips4x3[6],		// 5: Greyscale+392
		CounterType,			-21, 25, &G_Philips4x3[7],		// 25: Motion+434
		CounterType,			-50,	6, &G_Philips4x3[8],		// 6: Normal+476
		SyncLineB, 				-2,	0, &G_Philips4x3[9],

		SyncLineA,				-23,	0, &G_Philips4x3[10],
		CounterType,			-134,	7, &G_Philips4x3[11],	// 7: Normal+1
		CounterType,			-20,	8, &G_Philips4x3[12],	// 8: Date/Time+1
		CounterType,			-21,	9, &G_Philips4x3[13],	// 9: PLUGE+1
		CounterType,			-21,	10, &G_Philips4x3[14],	// 10: Normal+1
		CounterType,			-21,	11, &G_Philips4x3[15],	// 11: Greyscale+1
		CounterType,			-21,	26, &G_Philips4x3[16],	// 26: Motion+1
		CounterType,			-50,	12, &G_Philips4x3[17],	// 12: Normal+1
		SyncLineA,				-2,	0, &G_Philips4x3[18],

		SyncLineB,  			-22,	0, &G_Philips4x3[19],
		CounterType,			-134,	13, &G_Philips4x3[20],	// 13: Normal+2
		CounterType,			-20,	14, &G_Philips4x3[21],	// 14: Date/Time+2
		CounterType,			-21,	15, &G_Philips4x3[22],	// 15: PLUGE+2
		CounterType,			-21,	16, &G_Philips4x3[23],	// 16: Normal+2
		CounterType,			-21,	17, &G_Philips4x3[24],	// 17: Greyscale+2
		CounterType,			-21,	27, &G_Philips4x3[25],	// 27: Motion+2
		CounterType,			-50,	18, &G_Philips4x3[26],	// 18: Normal+2
		SyncLineB, 				-2,	0, &G_Philips4x3[27],

		SyncLineA,				-23,	0, &G_Philips4x3[28],
		CounterType,			-134,	19, &G_Philips4x3[29],	// 19: Normal+3
		CounterType,			-20,	20, &G_Philips4x3[30],	// 20: Date/Time+3
		CounterType,			-21,	21, &G_Philips4x3[31],	// 21: PLUGE+3
		CounterType,			-21,	22, &G_Philips4x3[32],	// 22: Normal+3
		CounterType,			-21,	23, &G_Philips4x3[33],	// 23: Greyscale+3
		CounterType,			-21,	28, &G_Philips4x3[34],	// 28: Motion+3
		CounterType,			-50,	24, &G_Philips4x3[35],	// 24: Normal+3
		SyncLineA,				-2,	0, &G_Philips4x3[0],
	};
#else
	code struct PATTERN_ITEM G_Philips4x3[32] = {
		SyncLineB,  			-22,	0, &G_Philips4x3[1],
		CounterType,			-134,	1, &G_Philips4x3[2],		// 1: Normal
		CounterType,			-20,	2, &G_Philips4x3[3],		// 2: Date/Time+268
		CounterType,			-21,	3, &G_Philips4x3[4],		// 3: PLUGE+308
		CounterType,			-21,	4, &G_Philips4x3[5],		// 4: Normal+350
		CounterType,			-21,	5, &G_Philips4x3[6],		// 5: Greyscale+392
		CounterType,			-71,	6, &G_Philips4x3[7],		// 6: Normal+434
		SyncLineB, 				-2,	0, &G_Philips4x3[8],

		SyncLineA,				-23,	0, &G_Philips4x3[9],
		CounterType,			-134,	7, &G_Philips4x3[10],	// 7: Normal+1
		CounterType,			-20,	8, &G_Philips4x3[11],	// 8: Date/Time+1
		CounterType,			-21,	9, &G_Philips4x3[12],	// 9: PLUGE+1
		CounterType,			-21,	10, &G_Philips4x3[13],	// 10: Normal+1
		CounterType,			-21,	11, &G_Philips4x3[14],	// 11: Greyscale+1
		CounterType,			-71,	12, &G_Philips4x3[15],	// 12: Normal+1
		SyncLineA,				-2,	0, &G_Philips4x3[16],

		SyncLineB,  			-22,	0, &G_Philips4x3[17],
		CounterType,			-134,	13, &G_Philips4x3[18],	// 13: Normal+2
		CounterType,			-20,	14, &G_Philips4x3[19],	// 14: Date/Time+2
		CounterType,			-21,	15, &G_Philips4x3[20],	// 15: PLUGE+2
		CounterType,			-21,	16, &G_Philips4x3[21],	// 16: Normal+2
		CounterType,			-21,	17, &G_Philips4x3[22],	// 17: Greyscale+2
		CounterType,			-71,	18, &G_Philips4x3[23],	// 18: Normal+2
		SyncLineB, 				-2,	0, &G_Philips4x3[24],

		SyncLineA,				-23,	0, &G_Philips4x3[25],
		CounterType,			-134,	19, &G_Philips4x3[26],	// 19: Normal+3
		CounterType,			-20,	20, &G_Philips4x3[27],	// 20: Date/Time+3
		CounterType,			-21,	21, &G_Philips4x3[28],	// 21: PLUGE+3
		CounterType,			-21,	22, &G_Philips4x3[29],	// 22: Normal+3
		CounterType,			-21,	23, &G_Philips4x3[30],	// 23: Greyscale+3
		CounterType,			-71,	24, &G_Philips4x3[31],	// 24: Normal+3
		SyncLineA,				-2,	0, &G_Philips4x3[0],
	};
#endif

code struct PATTERN_ITEM G_Philips4x3woAPAL[6] = {
	SyncLineB,  			-22,	0, &G_Philips4x3woAPAL[1],
	CounterType,			-288,	1382, &G_Philips4x3woAPAL[2],
	SyncLineB, 				-2,	0, &G_Philips4x3woAPAL[3],

	SyncLineA,				-23,	0, &G_Philips4x3woAPAL[4],
	CounterType,			-288,	1383, &G_Philips4x3woAPAL[5],
	SyncLineA,				-2,	0, &G_Philips4x3woAPAL[0],
};

#if PT8633Version
	code struct PATTERN_ITEM G_CCIR17[6] = {
		SyncLineB,				-22,	0, &G_CCIR17[1],
		CCIR17,					-288,	0, &G_CCIR17[2],
		SyncLineB,  			-2,	0, &G_CCIR17[3],

		SyncLineA,  			-23,	0, &G_CCIR17[4],
		CCIR17+1,				-288,	0, &G_CCIR17[5],
		SyncLineA,  			-2,	0, &G_CCIR17[0],
	};
#endif

code struct PATTERN_ITEM G_AnalogBlanking[6] = {
	SyncLineB,				-22,	0, &G_AnalogBlanking[1],
	TestAnalogBlanking,	-288,	0, &G_AnalogBlanking[2],
	SyncLineB,				-2,	0, &G_AnalogBlanking[3],

	SyncLineA,				-23,	0, &G_AnalogBlanking[4],
	TestAnalogBlanking+1,-288,	0, &G_AnalogBlanking[5],
	SyncLineA,				-2,	0, &G_AnalogBlanking[0],
};

code struct PATTERN_ITEM G_Multipulse[6] = {
	SyncLineB,				-22,	0, &G_Multipulse[1],
	Multipulse,				-288,	0, &G_Multipulse[2],
	SyncLineB,				-2,	0, &G_Multipulse[3],

	SyncLineA,				-23,	0, &G_Multipulse[4],
	Multipulse+1,			-288,	0, &G_Multipulse[5],
	SyncLineA,				-2,	0, &G_Multipulse[0],
};

#if PT8633Version
	code struct PATTERN_ITEM G_Sinxx[6] = {
		SyncLineB,				-22,	0, &G_Sinxx[1],
		sinc,						-288,	0, &G_Sinxx[2],
		SyncLineB,				-2,	0, &G_Sinxx[3],

		SyncLineA,				-23,	0, &G_Sinxx[4],
		sinc+1,					-288,	0, &G_Sinxx[5],
		SyncLineA,				-2,	0, &G_Sinxx[0],
	};
#endif

code struct PATTERN_ITEM G_DigitalGrey[6] = {
	SyncLineB,				-22,	0, &G_DigitalGrey[1],
	DigitalGrey_625,		-288,	0, &G_DigitalGrey[2],
	SyncLineB,				-2,	0, &G_DigitalGrey[3],

	SyncLineA,				-23,	0, &G_DigitalGrey[4],
	DigitalGrey_625+1,	-288,	0, &G_DigitalGrey[5],
	SyncLineA,				-2,	0, &G_DigitalGrey[0],
};

code struct PATTERN_ITEM G_FieldTimingChk[24] = {
	SyncLineB,				-22,	0, &G_FieldTimingChk[1],
	SyncLineC,				-288,	0, &G_FieldTimingChk[2],
	SyncLineB,				-2,	0, &G_FieldTimingChk[3],

	SyncLineA,				-23,	0, &G_FieldTimingChk[4],
	Y100mV_625+1,			-288,	0, &G_FieldTimingChk[5],
	SyncLineA,				-2,	0, &G_FieldTimingChk[6],

	SyncLineB,				-22,	0, &G_FieldTimingChk[7],
	Y200mV_625,				-288,	0, &G_FieldTimingChk[8],
	SyncLineB,				-2,	0, &G_FieldTimingChk[9],

	SyncLineA,				-23,	0, &G_FieldTimingChk[10],
	Y300mV_625+1,			-288,	0, &G_FieldTimingChk[11],
	SyncLineA,				-2,	0, &G_FieldTimingChk[12],

	SyncLineB,				-22,	0, &G_FieldTimingChk[13],
	Y400mV_625,				-288,	0, &G_FieldTimingChk[14],
	SyncLineB,				-2,	0, &G_FieldTimingChk[15],

	SyncLineA,				-23,	0, &G_FieldTimingChk[16],
	Y500mV_625+1,			-288,	0, &G_FieldTimingChk[17],
	SyncLineA,				-2,	0, &G_FieldTimingChk[18],

	SyncLineB,				-22,	0, &G_FieldTimingChk[19],
	Y600mV_625,				-288,	0, &G_FieldTimingChk[20],
	SyncLineB,				-2,	0, &G_FieldTimingChk[21],

	SyncLineA,				-23,	0, &G_FieldTimingChk[22],
	WhiteLine_625+1,		-288,	0, &G_FieldTimingChk[23],
	SyncLineA,				-2,	0, &G_FieldTimingChk[0]
};

code struct PATTERN_ITEM G_ValidRamp[6] = {
	SyncLineB,				-22,	0, &G_ValidRamp[1],
	ValidRampY,				-288,	0, &G_ValidRamp[2],
	SyncLineB,				-2,	0, &G_ValidRamp[3],

	SyncLineA,				-23,	0, &G_ValidRamp[4],
	ValidRampY+1,			-288,	0, &G_ValidRamp[5],
	SyncLineA,				-2,	0, &G_ValidRamp[0],
};

code struct PATTERN_ITEM G_Stair5[6] = {
	SyncLineB,				-22,	0, &G_Stair5[1],
	GreyScale5_625,		-288,	0, &G_Stair5[2],
	SyncLineB,				-2,	0, &G_Stair5[3],

	SyncLineA,				-23,	0, &G_Stair5[4],
	GreyScale5_625+1,		-288,	0, &G_Stair5[5],
	SyncLineA,				-2,	0, &G_Stair5[0],
};

#if PT8633Version
	code struct PATTERN_ITEM G_Stair10[6] = {
		SyncLineB,				-22,	0, &G_Stair10[1],
		GreyScale10_625,		-288,	0, &G_Stair10[2],
		SyncLineB,				-2,	0, &G_Stair10[3],

		SyncLineA,				-23,	0, &G_Stair10[4],
		GreyScale10_625+1,	-288,	0, &G_Stair10[5],
		SyncLineA,				-2,	0, &G_Stair10[0],
	};
#endif

code struct PATTERN_ITEM G_LuminanceSweep[6] = {
	SyncLineB,				-22,	0, &G_LuminanceSweep[1],
	LineSweepY_625,		-288,	0, &G_LuminanceSweep[2],
	SyncLineB,				-2,	0, &G_LuminanceSweep[3],

	SyncLineA,				-23,	0, &G_LuminanceSweep[4],
	LineSweepY_625+1,		-288,	0, &G_LuminanceSweep[5],
	SyncLineA,				-2,	0, &G_LuminanceSweep[0],
};

#if PT8633Version
	code struct PATTERN_ITEM G_YCrCbSweep[6] = {
		SyncLineB,				-22,	0, &G_YCrCbSweep[1],
		_ChromaLineSweep_625,-288,	0, &G_YCrCbSweep[2],
		SyncLineB,				-2,	0, &G_YCrCbSweep[3],

		SyncLineA,				-23,	0, &G_YCrCbSweep[4],
		_ChromaLineSweep_625+1,	-288,	0, &G_YCrCbSweep[5],
		SyncLineA,				-2,	0, &G_YCrCbSweep[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_Flat100[6] = {
		SyncLineB,				-22,	0, &G_Flat100[1],
		WhiteLine_625,			-288,	0, &G_Flat100[2],
		SyncLineB,				-2,	0, &G_Flat100[3],

		SyncLineA,				-23,	0, &G_Flat100[4],
		WhiteLine_625+1,		-288,	0, &G_Flat100[5],
		SyncLineA,				-2,	0, &G_Flat100[0],
	};
#endif

code struct PATTERN_ITEM G_BowTie[10] = {
	SyncLineB,				-22,	0, &G_BowTie[1],
	MarkerForBowTie,		-25,	0, &G_BowTie[2],
	BowTie,					-238,	0, &G_BowTie[3],
	MarkerForBowTie,		-25,	0, &G_BowTie[4],
	SyncLineB,				-2,	0, &G_BowTie[5],

	SyncLineA,				-23,	0, &G_BowTie[6],
	MarkerForBowTie+1,	-25,	0, &G_BowTie[7],
	BowTie+1,				-238,	0, &G_BowTie[8],
	MarkerForBowTie+1,	-25,	0, &G_BowTie[9],
	SyncLineA,				-2,	0, &G_BowTie[0],
};

#if PT8633Version
	code struct PATTERN_ITEM G_AltBlWh[6] = {
		SyncLineB,				-22,	0, &G_AltBlWh[1],
		DigitalGrey_625,		-288,	0, &G_AltBlWh[2],
		SyncLineB,				-2,	0, &G_AltBlWh[3],

		SyncLineA,				-23,	0, &G_AltBlWh[4],
		DigitalGrey_625+1,	-288,	0, &G_AltBlWh[5],
		SyncLineA,				-2,	0, &G_AltBlWh[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_EndOfLine[6] = {
		SyncLineB,				-22,	0, &G_EndOfLine[1],
		EndOfLinePorches,		-288,	0, &G_EndOfLine[2],
		SyncLineB,				-2,	0, &G_EndOfLine[3],

		SyncLineA,				-23,	0, &G_EndOfLine[4],
		EndOfLinePorches+1,	-288,	0, &G_EndOfLine[5],
		SyncLineA,				-2,	0, &G_EndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_WhiteEndOfLine[6] = {
		SyncLineB,				-22,	0, &G_WhiteEndOfLine[1],
		WhiteEoflPorch,		-288,	0, &G_WhiteEndOfLine[2],
		SyncLineB,				-2,	0, &G_WhiteEndOfLine[3],

		SyncLineA,				-23,	0, &G_WhiteEndOfLine[4],
		WhiteEoflPorch+1,		-288,	0, &G_WhiteEndOfLine[5],
		SyncLineA,				-2,	0, &G_WhiteEndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_BlueEndOfLine[6] = {
		SyncLineB,				-22,	0, &G_BlueEndOfLine[1],
		BlueEoflPorch,			-288,	0, &G_BlueEndOfLine[2],
		SyncLineB,				-2,	0, &G_BlueEndOfLine[3],

		SyncLineA,				-23,	0, &G_BlueEndOfLine[4],
		BlueEoflPorch+1,		-288,	0, &G_BlueEndOfLine[5],
		SyncLineA,				-2,	0, &G_BlueEndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_RedEndOfLine[6] = {
		SyncLineB,				-22,	0, &G_RedEndOfLine[1],
		RedEoflPorch,			-288,	0, &G_RedEndOfLine[2],
		SyncLineB,				-2,	0, &G_RedEndOfLine[3],

		SyncLineA,				-23,	0, &G_RedEndOfLine[4],
		RedEoflPorch+1,		-288,	0, &G_RedEndOfLine[5],
		SyncLineA,				-2,	0, &G_RedEndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_YellowEndOfLine[6] = {
		SyncLineB,				-22,	0, &G_YellowEndOfLine[1],
		YellowEoflPorch,		-288,	0, &G_YellowEndOfLine[2],
		SyncLineB,				-2,	0, &G_YellowEndOfLine[3],

		SyncLineA,				-23,	0, &G_YellowEndOfLine[4],
		YellowEoflPorch+1,	-288,	0, &G_YellowEndOfLine[5],
		SyncLineA,				-2,	0, &G_YellowEndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_CyanEndOfLine[6] = {
		SyncLineB,				-22,	0, &G_CyanEndOfLine[1],
		CyanEoflPorch,			-288,	0, &G_CyanEndOfLine[2],
		SyncLineB,				-2,	0, &G_CyanEndOfLine[3],

		SyncLineA,				-23,	0, &G_CyanEndOfLine[4],
		CyanEoflPorch+1,		-288,	0, &G_CyanEndOfLine[5],
		SyncLineA,				-2,	0, &G_CyanEndOfLine[0],
	};
#endif

code struct PATTERN_ITEM G_LuminanceRamp[6] = {
	SyncLineB,				-22,	0, &G_LuminanceRamp[1],
	Rampe52usec_625,		-288,	0, &G_LuminanceRamp[2],
	SyncLineB,				-2,	0, &G_LuminanceRamp[3],

	SyncLineA,				-23,	0, &G_LuminanceRamp[4],
	Rampe52usec_625+1,	-288,	0, &G_LuminanceRamp[5],
	SyncLineA,				-2,	0, &G_LuminanceRamp[0],
};

code struct PATTERN_ITEM G_LimitRamp[6] = {
	SyncLineB,				-22,	0, &G_LimitRamp[1],
	BlackWhiteRamp,		-288,	0, &G_LimitRamp[2],
	SyncLineB,				-2,	0, &G_LimitRamp[3],

	SyncLineA,				-23,	0, &G_LimitRamp[4],
	BlackWhiteRamp+1,		-288,	0, &G_LimitRamp[5],
	SyncLineA,				-2,	0, &G_LimitRamp[0],
};

code struct PATTERN_ITEM G_PulseBar[6] = {
	SyncLineB,				-22,	0, &G_PulseBar[1],
	T2T20BarInv2T_625,	-288,	0, &G_PulseBar[2],
	SyncLineB,				-2,	0, &G_PulseBar[3],

	SyncLineA,				-23,	0, &G_PulseBar[4],
	T2T20BarInv2T_625+1,	-288,	0, &G_PulseBar[5],
	SyncLineA,				-2,	0, &G_PulseBar[0],
};

#if PT8633Version
	code struct PATTERN_ITEM G_YellowGreyRamp[6] = {
		SyncLineB,				-22,	0, &G_YellowGreyRamp[1],
		YellowGreyRamp,		-288,	0, &G_YellowGreyRamp[2],
		SyncLineB,				-2,	0, &G_YellowGreyRamp[3],

		SyncLineA,				-23,	0, &G_YellowGreyRamp[4],
		YellowGreyRamp+1,		-288,	0, &G_YellowGreyRamp[5],
		SyncLineA,				-2,	0, &G_YellowGreyRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_GreyBlueRamp[6] = {
		SyncLineB,				-22,	0, &G_GreyBlueRamp[1],
		GreyBlueRamp,			-288,	0, &G_GreyBlueRamp[2],
		SyncLineB,				-2,	0, &G_GreyBlueRamp[3],

		SyncLineA,				-23,	0, &G_GreyBlueRamp[4],
		GreyBlueRamp+1,		-288,	0, &G_GreyBlueRamp[5],
		SyncLineA,				-2,	0, &G_GreyBlueRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_CyanGreyRamp[6] = {
		SyncLineB,				-22,	0, &G_CyanGreyRamp[1],
		CyanGreyRamp,			-288,	0, &G_CyanGreyRamp[2],
		SyncLineB,				-2,	0, &G_CyanGreyRamp[3],

		SyncLineA,				-23,	0, &G_CyanGreyRamp[4],
		CyanGreyRamp+1,		-288,	0, &G_CyanGreyRamp[5],
		SyncLineA,				-2,	0, &G_CyanGreyRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_GreyRedRamp[6] = {
		SyncLineB,				-22,	0, &G_GreyRedRamp[1],
		GreyRedRamp,			-288,	0, &G_GreyRedRamp[2],
		SyncLineB,				-2,	0, &G_GreyRedRamp[3],

		SyncLineA,				-23,	0, &G_GreyRedRamp[4],
		GreyRedRamp+1,			-288,	0, &G_GreyRedRamp[5],
		SyncLineA,				-2,	0, &G_GreyRedRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_CbYCrYRamp[6] = {
		SyncLineB,				-22,	0, &G_CbYCrYRamp[1],
		SavtandYCbCrY,			-288,	0, &G_CbYCrYRamp[2],
		SyncLineB,				-2,	0, &G_CbYCrYRamp[3],

		SyncLineA,				-23,	0, &G_CbYCrYRamp[4],
		SavtandYCbCrY+1,		-288,	0, &G_CbYCrYRamp[5],
		SyncLineA,				-2,	0, &G_CbYCrYRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_Philips16x9[56] = {
		SyncLineB,				-22,	0, &G_Philips16x9[1],
		CounterType,			-14,	3430, &G_Philips16x9[2],	// Normal
		CounterType,			-72,	1, &G_Philips16x9[3],		//	1: Cirkel
		CounterType,			-15,	3602, &G_Philips16x9[4],	// Normal
		CounterType,			-32,	2, &G_Philips16x9[5],		// 2: AntiPAL
		CounterType,			-1,	3696, &G_Philips16x9[6],	// Normal
		CounterType,			-20,	3, &G_Philips16x9[7],		// 3: Date/Time
		CounterType,			-21,	4, &G_Philips16x9[8],		// 4: PLUGE
		CounterType,			-21,	3780, &G_Philips16x9[9],	// Normal
		CounterType,			-21,	5, &G_Philips16x9[10],		//	5: Greyscale
		CounterType,			-21,	15, &G_Philips16x9[11],		//	15: Motion
		CounterType,			-35,	6, &G_Philips16x9[12],		//	6: Cirkel
		CounterType,			-15,	3976, &G_Philips16x9[13],	// Normal
		SyncLineB,				-2,	0, &G_Philips16x9[14],

		SyncLineA,				-23,	0, &G_Philips16x9[15],
		CounterType,			-14,	3431, &G_Philips16x9[16],	// Normal
		CounterType,			-72,	7, &G_Philips16x9[17],		//	7: Cirkel+1
		CounterType,			-15,	3603, &G_Philips16x9[18],	// Normal
		CounterType,			-32,	8, &G_Philips16x9[19],		// 8: AntiPAL+1
		CounterType,			-1,	3697, &G_Philips16x9[20],	// Normal
		CounterType,			-20,	9, &G_Philips16x9[21],		// 9: Date/Time+1
		CounterType,			-21,	10, &G_Philips16x9[22],		// 10: PLUGE+1
		CounterType,			-21,	3781, &G_Philips16x9[23],	// Normal
		CounterType,			-21,	11, &G_Philips16x9[24],		//	11: Greyscale+1
		CounterType,			-21,	16, &G_Philips16x9[25],		//	16: Motion+1
		CounterType,			-35,	12, &G_Philips16x9[26],		//	12: Cirkel+1
		CounterType,			-15,	3977, &G_Philips16x9[27],	// Normal
		SyncLineA,				-2,	0, &G_Philips16x9[28],

		SyncLineB,				-22,	0, &G_Philips16x9[29],
		CounterType,			-14,	3430, &G_Philips16x9[30],	// Normal
		CounterType,			-72,	1, &G_Philips16x9[31],		//	1: Cirkel
		CounterType,			-15,	3602, &G_Philips16x9[32],	// Normal
		CounterType,			-32,	13, &G_Philips16x9[33],		// 13: AntiPAL+2
		CounterType,			-1,	3696, &G_Philips16x9[34],	// Normal
		CounterType,			-20,	3, &G_Philips16x9[35],		// 3: Date/Time
		CounterType,			-21,	4, &G_Philips16x9[36],		// 4: PLUGE
		CounterType,			-21,	3780, &G_Philips16x9[37],	// Normal
		CounterType,			-21,	5, &G_Philips16x9[38],		//	5: Greyscale
		CounterType,			-21,	15, &G_Philips16x9[39],		//	15: Motion
		CounterType,			-35,	6, &G_Philips16x9[40],		//	6: Cirkel
		CounterType,			-15,	3976, &G_Philips16x9[41],	// Normal
		SyncLineB,				-2,	0, &G_Philips16x9[42],

		SyncLineA,				-23,	0, &G_Philips16x9[43],
		CounterType,			-14,	3431, &G_Philips16x9[44],	// Normal
		CounterType,			-72,	7, &G_Philips16x9[45],		//	7: Cirkel+1
		CounterType,			-15,	3603, &G_Philips16x9[46],	// Normal
		CounterType,			-32,	14, &G_Philips16x9[47],		// 14: AntiPAL+3
		CounterType,			-1,	3697, &G_Philips16x9[48],	// Normal
		CounterType,			-20,	9, &G_Philips16x9[49],		// 9: Date/Time+1
		CounterType,			-21,	10, &G_Philips16x9[50],		// 10: PLUGE+1
		CounterType,			-21,	3781, &G_Philips16x9[51],	// Normal
		CounterType,			-21,	11, &G_Philips16x9[52],		//	11: Greyscale+1
		CounterType,			-21,	16, &G_Philips16x9[53],		//	16: Motion+1
		CounterType,			-35,	12, &G_Philips16x9[54],		//	12: Cirkel+1
		CounterType,			-15,	3977, &G_Philips16x9[55],	// Normal
		SyncLineA,				-2,	0, &G_Philips16x9[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_FuBK4x3[36] = {
		SyncLineB,				-22,	0, &G_FuBK4x3[1],
		CounterType,			-144,	1958, &G_FuBK4x3[2],
		CounterType,			-20,	7, &G_FuBK4x3[3],				// 7: Motion
		CounterType,			-39,	2286, &G_FuBK4x3[4],			// Normal
		CounterType,			-40,	1, &G_FuBK4x3[5],				// 1: AntiPAL+1
		CounterType,			-1,	2444, &G_FuBK4x3[6],			// Normal
		CounterType,			-19,	5, &G_FuBK4x3[7],				// 5: Date/Time
		CounterType,			-25,	2484, &G_FuBK4x3[8],			// Normal
		SyncLineB,				-2,	0, &G_FuBK4x3[9],

		SyncLineA,				-23,	0, &G_FuBK4x3[10],
		CounterType,			-144,	1959, &G_FuBK4x3[11],
		CounterType,			-20,	8, &G_FuBK4x3[12],			// 7: Motion+1
		CounterType,			-39,	2287, &G_FuBK4x3[13],		// Normal
		CounterType,			-40,	2, &G_FuBK4x3[14],			// 2: AntiPAL+2
		CounterType,			-1,	2445, &G_FuBK4x3[15],		// Normal
		CounterType,			-19,	6, &G_FuBK4x3[16],			// 6: Date/Time+1
		CounterType,			-25,	2485, &G_FuBK4x3[17],		// Normal
		SyncLineA,				-2,	0, &G_FuBK4x3[18],

		SyncLineB,				-22,	0, &G_FuBK4x3[19],
		CounterType,			-144,	1958, &G_FuBK4x3[20],
		CounterType,			-20,	7, &G_FuBK4x3[21],			// 7: Motion
		CounterType,			-39,	2286, &G_FuBK4x3[22],		// Normal
		CounterType,			-40,	3, &G_FuBK4x3[23],			// 3: AntiPAL+3
		CounterType,			-1,	2444, &G_FuBK4x3[24],		// Normal
		CounterType,			-19,	5, &G_FuBK4x3[25],			// 5: Date/Time
		CounterType,			-25,	2484, &G_FuBK4x3[26],		// Normal
		SyncLineB,				-2,	0, &G_FuBK4x3[27],

		SyncLineA,				-23,	0, &G_FuBK4x3[28],
		CounterType,			-144,	1959, &G_FuBK4x3[29],
		CounterType,			-20,	8, &G_FuBK4x3[30],			// 7: Motion+1
		CounterType,			-39,	2287, &G_FuBK4x3[31],		// Normal
		CounterType,			-40,	4, &G_FuBK4x3[32],			// 4: AntiPAL+4
		CounterType,			-1,	2445, &G_FuBK4x3[33],		// Normal
		CounterType,			-19,	6, &G_FuBK4x3[34],			// 6: Date/Time+1
		CounterType,			-25,	2485, &G_FuBK4x3[35],		// Normal
		SyncLineA,				-2,	0, &G_FuBK4x3[0],
	};
#else
	code struct PATTERN_ITEM G_FuBK4x3[28] = {
		SyncLineB,				-22,	0, &G_FuBK4x3[1],
		CounterType,			-203,	230, &G_FuBK4x3[2],
		CounterType,			-40,	1, &G_FuBK4x3[3],				// 1: AntiPAL+1
		CounterType,			-1,	716, &G_FuBK4x3[4],			// Normal
		CounterType,			-19,	5, &G_FuBK4x3[5],				// 5: Date/Time
		CounterType,			-25,	756, &G_FuBK4x3[6],			// Normal
		SyncLineB,				-2,	0, &G_FuBK4x3[7],

		SyncLineA,				-23,	0, &G_FuBK4x3[8],
		CounterType,			-203,	231, &G_FuBK4x3[9],
		CounterType,			-40,	2, &G_FuBK4x3[10],			// 2: AntiPAL+2
		CounterType,			-1,	717, &G_FuBK4x3[11],			// Normal
		CounterType,			-19,	6, &G_FuBK4x3[12],			// 6: Date/Time+1
		CounterType,			-25,	757, &G_FuBK4x3[13],			// Normal
		SyncLineA,				-2,	0, &G_FuBK4x3[14],

		SyncLineB,				-22,	0, &G_FuBK4x3[15],
		CounterType,			-203,	230, &G_FuBK4x3[16],
		CounterType,			-40,	3, &G_FuBK4x3[17],			// 3: AntiPAL+3
		CounterType,			-1,	716, &G_FuBK4x3[18],			// Normal
		CounterType,			-19,	5, &G_FuBK4x3[19],			// 5: Date/Time
		CounterType,			-25,	756, &G_FuBK4x3[20],			// Normal
		SyncLineB,				-2,	0, &G_FuBK4x3[21],

		SyncLineA,				-23,	0, &G_FuBK4x3[22],
		CounterType,			-203,	231, &G_FuBK4x3[23],
		CounterType,			-40,	4, &G_FuBK4x3[24],			// 4: AntiPAL+4
		CounterType,			-1,	717, &G_FuBK4x3[25],			// Normal
		CounterType,			-19,	6, &G_FuBK4x3[26],			// 6: Date/Time+1
		CounterType,			-25,	757, &G_FuBK4x3[27],			// Normal
		SyncLineA,				-2,	0, &G_FuBK4x3[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_FuBK16x9[36] = {
		SyncLineB,				-22,	0, &G_FuBK16x9[1],
		CounterType,			-144,	2694, &G_FuBK16x9[2],
		CounterType,			-20,	7, &G_FuBK16x9[3],			// 7: Motion
		CounterType,			-39,	3022, &G_FuBK16x9[4],		// Normal
		CounterType,			-40,	1, &G_FuBK16x9[5],			// 1: AntiPAL+1
		CounterType,			-1,	3180, &G_FuBK16x9[6],		// Normal
		CounterType,			-19,	5, &G_FuBK16x9[7],			// 5: Date/Time
		CounterType,			-25,	3220, &G_FuBK16x9[8],		// Normal
		SyncLineB,				-2,	0, &G_FuBK16x9[9],

		SyncLineA,				-23,	0, &G_FuBK16x9[10],
		CounterType,			-144,	2695, &G_FuBK16x9[11],
		CounterType,			-20,	8, &G_FuBK16x9[12],			// 8: Motion+1
		CounterType,			-39,	3023, &G_FuBK16x9[13],		// Normal
		CounterType,			-40,	2, &G_FuBK16x9[14],			// 2: AntiPAL+2
		CounterType,			-1,	3181, &G_FuBK16x9[15],		// Normal
		CounterType,			-19,	6, &G_FuBK16x9[16],			// 6: Date/Time+1
		CounterType,			-25,	3221, &G_FuBK16x9[17],		// Normal
		SyncLineA,				-2,	0, &G_FuBK16x9[18],

		SyncLineB,				-22,	0, &G_FuBK16x9[19],
		CounterType,			-144,	2694, &G_FuBK16x9[20],
		CounterType,			-20,	7, &G_FuBK16x9[21],			// 7: Motion
		CounterType,			-39,	3022, &G_FuBK16x9[22],		// Normal
		CounterType,			-40,	3, &G_FuBK16x9[23],			// 3: AntiPAL+3
		CounterType,			-1,	3180, &G_FuBK16x9[24],		// Normal
		CounterType,			-19,	5, &G_FuBK16x9[25],			// 5: Date/Time
		CounterType,			-25,	3220, &G_FuBK16x9[26],		// Normal
		SyncLineB,				-2,	0, &G_FuBK16x9[27],

		SyncLineA,				-23,	0, &G_FuBK16x9[28],
		CounterType,			-144,	2695, &G_FuBK16x9[29],
		CounterType,			-20,	8, &G_FuBK16x9[30],			// 8: Motion+1
		CounterType,			-39,	3023, &G_FuBK16x9[31],		// Normal
		CounterType,			-40,	4, &G_FuBK16x9[32],			// 4: AntiPAL+4
		CounterType,			-1,	3181, &G_FuBK16x9[33],		// Normal
		CounterType,			-19,	6, &G_FuBK16x9[34],			// 6: Date/Time+1
		CounterType,			-25,	3221, &G_FuBK16x9[35],		// Normal
		SyncLineA,				-2,	0, &G_FuBK16x9[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_SafeArea[34] = {
		SyncLineB,				-22,	0, &G_SafeArea[1],
		SyncLineC,				-14,	0, &G_SafeArea[2],
		SafeAreaA_625,			-1,	0, &G_SafeArea[3],
		SyncLineC,				-14,	0, &G_SafeArea[4],
		SafeAreaB_625,			-1,	0, &G_SafeArea[5],
		SyncLineC,				-35,	0, &G_SafeArea[6],
		SafeAreaC_625,			-69,	0, &G_SafeArea[7],
		SafeAreaD_625,			-10,	0, &G_SafeArea[8],
		SafeAreaE_625,			-1,	0, &G_SafeArea[9],
		SafeAreaD_625,			-10,	0, &G_SafeArea[10],
		SafeAreaC_625,			-69,	0, &G_SafeArea[11],
		SyncLineC,				-35,	0, &G_SafeArea[12],
		SafeAreaB_625,			-1,	0, &G_SafeArea[13],
		SyncLineC,				-14,	0, &G_SafeArea[14],
		SafeAreaA_625,			-1,	0, &G_SafeArea[15],
		SyncLineC,				-13,	0, &G_SafeArea[16],
		SyncLineB,				-2,	0, &G_SafeArea[17],

		SyncLineA,				-23,	0, &G_SafeArea[18],
		SyncLineD,				-14,	0, &G_SafeArea[19],
		SafeAreaA_625+1,		-1,	0, &G_SafeArea[20],
		SyncLineD,				-14,	0, &G_SafeArea[21],
		SafeAreaB_625+1,		-1,	0, &G_SafeArea[22],
		SyncLineD,				-35,	0, &G_SafeArea[23],
		SafeAreaC_625+1,		-69,	0, &G_SafeArea[24],
		SafeAreaD_625+1,		-10,	0, &G_SafeArea[25],
		SafeAreaE_625+1,		-1,	0, &G_SafeArea[26],
		SafeAreaD_625+1,		-10,	0, &G_SafeArea[27],
		SafeAreaC_625+1,		-69,	0, &G_SafeArea[28],
		SyncLineD,				-35,	0, &G_SafeArea[29],
		SafeAreaB_625+1,		-1,	0, &G_SafeArea[30],
		SyncLineD,				-14,	0, &G_SafeArea[31],
		SafeAreaA_625+1,		-1,	0, &G_SafeArea[32],
		SyncLineD,				-13,	0, &G_SafeArea[33],
		SyncLineA,				-2,	0, &G_SafeArea[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM G_VMT01[28] = {
		SyncLineB,				-22,	0, &G_VMT01[1],
		SyncLineC,				-5,	0, &G_VMT01[2],
		WhiteLine_625,			-122,	0, &G_VMT01[3],
		SyncLineC,				-119,	0, &G_VMT01[4],
		SavTandForVMT,			-5,	0, &G_VMT01[5],
		CCIR17,					-5,	0, &G_VMT01[6],
		CCIR18,					-5,	0, &G_VMT01[7],
		CCIR330,					-5,	0, &G_VMT01[8],
		CCIR331,					-5,	0, &G_VMT01[9],
		CB100_625,				-5,	0, &G_VMT01[10],
		kHz15,					-5,	0, &G_VMT01[11],
		EBUCB_625,				-5,	0, &G_VMT01[12],
		SyncLineC,				-2,	0, &G_VMT01[13],
		SyncLineB,				-2,	0, &G_VMT01[14],

		SyncLineA,				-23,	0, &G_VMT01[15],
		SyncLineD,				-5,	0, &G_VMT01[16],
		WhiteLine_625+1,		-122,	0, &G_VMT01[17],
		SyncLineD,				-119,	0, &G_VMT01[18],
		SavTandForVMT+1,		-5,	0, &G_VMT01[19],
		CCIR17+1,				-5,	0, &G_VMT01[20],
		CCIR18+1,				-5,	0, &G_VMT01[21],
		CCIR330+1,				-5,	0, &G_VMT01[22],
		CCIR331+1,				-5,	0, &G_VMT01[23],
		CB100_625+1,			-5,	0, &G_VMT01[24],
		kHz15+1,					-5,	0, &G_VMT01[25],
		EBUCB_625+1,			-5,	0, &G_VMT01[26],
		SyncLineD,				-2,	0, &G_VMT01[27],
		SyncLineA,				-2,	0, &G_VMT01[0],
	};
#endif

code struct PATTERN_ITEM G_FieldSquareWave[10] = {
	SyncLineB,				-22,	0, &G_FieldSquareWave[1],
	SyncLineC,				-66,	0, &G_FieldSquareWave[2],
	WhiteLine_625,			-155,	0, &G_FieldSquareWave[3],
	SyncLineC,				-67,	0, &G_FieldSquareWave[4],
	SyncLineB,				-2,	0, &G_FieldSquareWave[5],

	SyncLineA,				-23,	0, &G_FieldSquareWave[6],
	SyncLineD,				-66,	0, &G_FieldSquareWave[7],
	WhiteLine_625+1,		-155,	0, &G_FieldSquareWave[8],
	SyncLineD,				-67,	0, &G_FieldSquareWave[9],
	SyncLineA,				-2,	0, &G_FieldSquareWave[0],
};

code struct PATTERN_ITEM G_ModulatedStair5[6] = {
	SyncLineB,				-22,	0, &G_ModulatedStair5[1],
	ModGreyScale5_625,	-288,	0, &G_ModulatedStair5[2],
	SyncLineB,				-2,	0, &G_ModulatedStair5[3],

	SyncLineA,				-23,	0, &G_ModulatedStair5[4],
	ModGreyScale5_625+1,	-288,	0, &G_ModulatedStair5[5],
	SyncLineA,				-2,	0, &G_ModulatedStair5[0],
};

code struct PATTERN_ITEM G_Customized1[6] = {
	SyncLineB,  			 -22, 0, &G_Customized1[1],
	SyncLineC,	 			-288, 0, &G_Customized1[2],
	SyncLineB,  			  -2, 0, &G_Customized1[3],

	SyncLineA,  			 -23, 0, &G_Customized1[4],
	SyncLineD,		  		-288, 0, &G_Customized1[5],
	SyncLineA,  			  -2, 0, &G_Customized1[0],
};

code struct PATTERN_ITEM G_Customized2[6] = {
	SyncLineB,  			 -22, 0, &G_Customized2[1],
	SyncLineC,	 			-288, 0, &G_Customized2[2],
	SyncLineB,  			  -2, 0, &G_Customized2[3],

	SyncLineA,  			 -23, 0, &G_Customized2[4],
	SyncLineD,		  		-288, 0, &G_Customized2[5],
	SyncLineA,  			  -2, 0, &G_Customized2[0],
};

code struct PATTERN_ITEM G_Customized3[6] = {
	SyncLineB,  			 -22, 0, &G_Customized3[1],
	SyncLineC,	 			-288, 0, &G_Customized3[2],
	SyncLineB,  			  -2, 0, &G_Customized3[3],

	SyncLineA,  			 -23, 0, &G_Customized3[4],
	SyncLineD,		  		-288, 0, &G_Customized3[5],
	SyncLineA,  			  -2, 0, &G_Customized3[0],
};

code struct PATTERN_ITEM G_Customized4[6] = {
	SyncLineB,  			 -22, 0, &G_Customized4[1],
	SyncLineC,	 			-288, 0, &G_Customized4[2],
	SyncLineB,  			  -2, 0, &G_Customized4[3],

	SyncLineA,  			 -23, 0, &G_Customized4[4],
	SyncLineD,		  		-288, 0, &G_Customized4[5],
	SyncLineA,  			  -2, 0, &G_Customized4[0],
};

code struct PATTERN_ITEM G_Customized5[6] = {
	SyncLineB,  			 -22, 0, &G_Customized5[1],
	SyncLineC,	 			-288, 0, &G_Customized5[2],
	SyncLineB,  			  -2, 0, &G_Customized5[3],

	SyncLineA,  			 -23, 0, &G_Customized5[4],
	SyncLineD,		  		-288, 0, &G_Customized5[5],
	SyncLineA,  			  -2, 0, &G_Customized5[0],
};

code struct PATTERN_ITEM G_Customized6[6] = {
	SyncLineB,  			 -22, 0, &G_Customized6[1],
	SyncLineC,	 			-288, 0, &G_Customized6[2],
	SyncLineB,  			  -2, 0, &G_Customized6[3],

	SyncLineA,  			 -23, 0, &G_Customized6[4],
	SyncLineD,		  		-288, 0, &G_Customized6[5],
	SyncLineA,  			  -2, 0, &G_Customized6[0],
};

code struct PATTERN_ITEM G_Customized7[6] = {
	SyncLineB,  			 -22, 0, &G_Customized7[1],
	SyncLineC,	 			-288, 0, &G_Customized7[2],
	SyncLineB,  			  -2, 0, &G_Customized7[3],

	SyncLineA,  			 -23, 0, &G_Customized7[4],
	SyncLineD,		  		-288, 0, &G_Customized7[5],
	SyncLineA,  			  -2, 0, &G_Customized7[0],
};

code struct PATTERN_ITEM G_Customized8[6] = {
	SyncLineB,  			 -22, 0, &G_Customized8[1],
	SyncLineC,	 			-288, 0, &G_Customized8[2],
	SyncLineB,  			  -2, 0, &G_Customized8[3],

	SyncLineA,  			 -23, 0, &G_Customized8[4],
	SyncLineD,		  		-288, 0, &G_Customized8[5],
	SyncLineA,  			  -2, 0, &G_Customized8[0],
};

#if PT8633Version
	code struct PATTERN_ITEM G_CCIR18[6] = {
		SyncLineB,				-22,	0, &G_CCIR18[1],
		CCIR18,					-288,	0, &G_CCIR18[2],
		SyncLineB,  			-2,	0, &G_CCIR18[3],

		SyncLineA,  			-23,	0, &G_CCIR18[4],
		CCIR18+1,				-288,	0, &G_CCIR18[5],
		SyncLineA,  			-2,	0, &G_CCIR18[0],
	};

	code struct PATTERN_ITEM G_CCIR330[6] = {
		SyncLineB,				-22,	0, &G_CCIR330[1],
		CCIR330,					-288,	0, &G_CCIR330[2],
		SyncLineB,  			-2,	0, &G_CCIR330[3],

		SyncLineA,  			-23,	0, &G_CCIR330[4],
		CCIR330+1,				-288,	0, &G_CCIR330[5],
		SyncLineA,  			-2,	0, &G_CCIR330[0],
	};

	code struct PATTERN_ITEM G_CCIR331[6] = {
		SyncLineB,				-22,	0, &G_CCIR331[1],
		CCIR331,					-288,	0, &G_CCIR331[2],
		SyncLineB,  			-2,	0, &G_CCIR331[3],

		SyncLineA,  			-23,	0, &G_CCIR331[4],
		CCIR331+1,				-288,	0, &G_CCIR331[5],
		SyncLineA,  			-2,	0, &G_CCIR331[0],
	};
#endif

code struct PATTERN_ITEM G_Window10[20] = {
	SyncLineB, 				-22,	0, &G_Window10[1],
	SyncLineC,				-24,	0, &G_Window10[2],
	Window50Marker,		-1,	0, &G_Window10[3],
	MarkerLine,				-1,	0, &G_Window10[4],
	SyncLineC,				-1,	0, &G_Window10[5],
	Marker10,				-1,	0, &G_Window10[6],
	SyncLineC, 				-44,	0, &G_Window10[7],
	Window10,				-144,	0, &G_Window10[8],
	SyncLineC,				-72,	0, &G_Window10[9],
	SyncLineB,				-2,	0, &G_Window10[10],

	SyncLineA,				-23,	0, &G_Window10[11],
	SyncLineD,				-24,	0, &G_Window10[12],
	Window50Marker+1,		-1,	0, &G_Window10[13],
	MarkerLine+1,			-1,	0, &G_Window10[14],
	SyncLineD,				-1,	0, &G_Window10[15],
	Marker10+1,				-1,	0, &G_Window10[16],
	SyncLineD,				-44,	0, &G_Window10[17],
	Window10+1,				-144,	0, &G_Window10[18],
	SyncLineD,				-72,	0, &G_Window10[19],
	SyncLineA,				-2,	0, &G_Window10[0],
};

code struct PATTERN_ITEM G_kHz15BlWh[6] = {
	SyncLineB,				-22,	0, &G_kHz15BlWh[1],
	kHz15BlWh_625,			-288,	0, &G_kHz15BlWh[2],
	SyncLineB,				-2,	0, &G_kHz15BlWh[3],

	SyncLineA,				-23,	0, &G_kHz15BlWh[4],
	kHz15BlWh_625+1,		-288,	0, &G_kHz15BlWh[5],
	SyncLineA,				-2,	0, &G_kHz15BlWh[0],
};
