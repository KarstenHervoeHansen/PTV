/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		D1PATTM.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980210																		*/
/*	Rev. date:	990623, KEn, DEV															*/
/*	Status:		Version 2.2																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*	Changes:																						*/
/* 990623: Added Window 10% & 15kHz Bl/Wh												*/
/* 981002: Released in SW ver. 3.0														*/
/* 980924; Increased pattern table according to new signals in SDI625.		*/
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
	code struct PATTERN_ITEM* code M_PatternTable[68] = {
		&M_SMPTECBar[2],
		&M_Dummy,
		&M_EBUCBarITU[2],
		&M_CBar100[2],
		&M_Dummy,
		&M_Dummy,
		&M_CheckField[2],
		&M_ShallowRamp[2],
		&M_TimingTest[2],
		&M_Black[2],
		&M_Window15[2],
		&M_Window20[2],
		&M_Window100[2],
		&M_CrossHatch[2],
		&M_PLUGE[2],
		&M_Multiburst[2],
		&M_Red75[2],
		&M_Philips4x3[2],
		&M_Dummy,
		&M_Dummy,
		&M_AnalogBlanking[2],
		&M_Multipulse[2],
		&M_Sinxx[2],
		&M_DigitalGrey[2],
		&M_FieldTimingChk[2],
		&M_ValidRamp[2],
		&M_Stair5[2],
		&M_Stair10[2],
		&M_LuminanceSweep[2],
		&M_YCrCbSweep[2],
		&M_Flat100[2],
		&M_BowTie[2],
		&M_AltBlWh[2],
		&M_EndOfLine[2],
		&M_WhiteEndOfLine[2],
		&M_BlueEndOfLine[2],
		&M_RedEndOfLine[2],
		&M_YellowEndOfLine[2],
		&M_CyanEndOfLine[2],
		&M_LuminanceRamp[2],
		&M_LimitRamp[2],
		&M_PulseBar[2],
		&M_YellowGreyRamp[2],
		&M_GreyBlueRamp[2],
		&M_CyanGreyRamp[2],
		&M_GreyRedRamp[2],
		&M_CbYCrYRamp[2],
		&M_Philips16x9[2],
		&M_Dummy,
		&M_Dummy,
		&M_SafeArea[2],
		&M_Dummy,
		&M_FCCCBar[2],
		&M_FieldSquareWave[2],
		&M_ModulatedStair5[2],
		&M_Customized1[2],
		&M_Customized2[2],
		&M_Customized3[2],
		&M_Customized4[2],
		&M_Customized5[2],
		&M_Customized6[2],
		&M_Customized7[2],
		&M_Customized8[2],
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Window10[2],
		&M_kHz15BlWh[2],
	};
#else
	//************************************************************************
	// PATTERN DEFINITIONS FOR THE PT8632 GENERATOR
	//************************************************************************
	code struct PATTERN_ITEM* code M_PatternTable[68] = {
		&M_SMPTECBar[2],
		&M_Dummy,
		&M_EBUCBarITU[2],
		&M_CBar100[2],
		&M_Dummy,
		&M_Dummy,
		&M_CheckField[2],
		&M_ShallowRamp[2],
		&M_TimingTest[2],
		&M_Black[2],
		&M_Window15[2],
		&M_Window20[2],
		&M_Window100[2],
		&M_CrossHatch[2],
		&M_PLUGE[2],
		&M_Multiburst[2],
		&M_Red75[2],
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_AnalogBlanking[2],
		&M_Multipulse[2],
		&M_Dummy,
		&M_DigitalGrey[2],
		&M_FieldTimingChk[2],
		&M_ValidRamp[2],
		&M_Stair5[2],
		&M_Dummy,
		&M_LuminanceSweep[2],
		&M_Dummy,
		&M_Dummy,
		&M_BowTie[2],
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_LuminanceRamp[2],
		&M_LimitRamp[2],
		&M_PulseBar[2],
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_FCCCBar[2],
		&M_FieldSquareWave[2],
		&M_ModulatedStair5[2],
		&M_Customized1[2],
		&M_Customized2[2],
		&M_Customized3[2],
		&M_Customized4[2],
		&M_Customized5[2],
		&M_Customized6[2],
		&M_Customized7[2],
		&M_Customized8[2],
		&M_Dummy,
		&M_Dummy,
		&M_Dummy,
		&M_Window10[2],
		&M_kHz15BlWh[2],
	};
#endif

/***************************************************************************/
// DEFINITION OF PATTERN ARRAYS IN SDI525
/***************************************************************************/

code struct PATTERN_ITEM M_Dummy;

code struct PATTERN_ITEM M_SMPTECBar[10] = {
	SyncLineA,  			-3,	0, &M_SMPTECBar[1],
	SyncLineB,	 			-16,	0, &M_SMPTECBar[2],
	SMPTEColorBar,  		-164,	0, &M_SMPTECBar[3],
	BlueBars,  				-19,	0, &M_SMPTECBar[4],
	IWhiteQ,  				-61,	0, &M_SMPTECBar[5],

	SyncLineB,  			-2,	0, &M_SMPTECBar[6],
	SyncLineA,				-17,	0, &M_SMPTECBar[7],
	SMPTEColorBar+1,		-163,	0, &M_SMPTECBar[8],
	BlueBars+1,  			-19,	0, &M_SMPTECBar[9],
	IWhiteQ+1,  			-61,	0, &M_SMPTECBar[0],
};

code struct PATTERN_ITEM M_EBUCBarITU[6] = {
	SyncLineA,		  		-3,	0, &M_EBUCBarITU[1],
	SyncLineB,	 			-16,	0, &M_EBUCBarITU[2],
	EBUCB_ITU,  			-244,	0, &M_EBUCBarITU[3],

	SyncLineB,  			-2,	0, &M_EBUCBarITU[4],
	SyncLineA,				-17,	0, &M_EBUCBarITU[5],
	EBUCB_ITU+1, 			-243,	0, &M_EBUCBarITU[0],
};

code struct PATTERN_ITEM M_CBar100[6] = {
	SyncLineA,		  		-3,	0, &M_CBar100[1],
	SyncLineB,	 			-16,	0, &M_CBar100[2],
	CB100_525,  			-244,	0, &M_CBar100[3],

	SyncLineB,  			-2,	0, &M_CBar100[4],
	SyncLineA,				-17,	0, &M_CBar100[5],
	CB100_525+1, 			-243,	0, &M_CBar100[0],
};

code struct PATTERN_ITEM M_CheckField[9] = {
	SyncLineA,		  		-3,	0, &M_CheckField[1],
	SyncLineB,	 			-16,	0, &M_CheckField[2],
	SDITestSignalLine1, 	-1,	0, &M_CheckField[3],
	SDITestSignalLine2,	-121,	0, &M_CheckField[4],
	SDISignalLine3, 		-122,	0, &M_CheckField[5],

	SyncLineB,  			-2,	0, &M_CheckField[6],
	SyncLineA,				-17,	0, &M_CheckField[7],
	SDITestSignalLine2+1,-121,	0, &M_CheckField[8],
	SDISignalLine3+1, 	-122,	0, &M_CheckField[0],
};

code struct PATTERN_ITEM M_ShallowRamp[6] = {
	SyncLineA,		  		-3,	0, &M_ShallowRamp[1],
	SyncLineB,	 			-16,	0, &M_ShallowRamp[2],
	ShallowRamp_525,		-244,	0, &M_ShallowRamp[3],

	SyncLineB,  			-2,	0, &M_ShallowRamp[4],
	SyncLineA,				-17,	0, &M_ShallowRamp[5],
	ShallowRamp_525+1, 	-243,	0, &M_ShallowRamp[0],
};

code struct PATTERN_ITEM M_TimingTest[10] = {
	SyncLineA,		  		-3,	0, &M_TimingTest[1],
	SyncLineB,	 			-16,	0, &M_TimingTest[2],
	WhiteLine_525,			-1,	0, &M_TimingTest[3],
	DigitalTiming_525,	-242,	0, &M_TimingTest[4],
	WhiteLine_525, 		-1,	0, &M_TimingTest[5],

	SyncLineB,  			-2,	0, &M_TimingTest[6],
	SyncLineA,				-17,	0, &M_TimingTest[7],
	WhiteLine_525+1,		-1,	0, &M_TimingTest[8],
	DigitalTiming_525+1,	-241,	0, &M_TimingTest[9],
	WhiteLine_525+1, 		-1,	0, &M_TimingTest[0],
};

code struct PATTERN_ITEM M_Black[6] = {
	SyncLineA,		  		-3,	0, &M_Black[1],
	SyncLineB,	 			-16,	0, &M_Black[2],
	SyncLineC,  			-244,	0, &M_Black[3],

	SyncLineB,  			-2,	0, &M_Black[4],
	SyncLineA,				-17,	0, &M_Black[5],
	SyncLineD, 				-243,	0, &M_Black[0],
};

code struct PATTERN_ITEM M_Window15[20] = {
	SyncLineA,				-3,	0, &M_Window15[1],
	SyncLineB,				-16,	0, &M_Window15[2],
	SyncLineC,				-30,	0, &M_Window15[3],
	Window50Marker, 		-1,	0, &M_Window15[4],
	MarkerLine,				-1,	0, &M_Window15[5],
	SyncLineC,				-1,	0, &M_Window15[6],
	Marker15,				-1,	0, &M_Window15[7],
	SyncLineC,				-29,	0, &M_Window15[8],
	Window15,				-119,	0, &M_Window15[9],
	SyncLineC,				-62,	0, &M_Window15[10],

	SyncLineB,				-2,	0, &M_Window15[11],
	SyncLineA,				-17,	0, &M_Window15[12],
	SyncLineD,				-29,	0, &M_Window15[13],
	Window50Marker+1,		-1,	0, &M_Window15[14],
	MarkerLine+1,			-1,	0, &M_Window15[15],
	SyncLineD,				-1,	0, &M_Window15[16],
	Marker15+1,				-1,	0, &M_Window15[17],
	SyncLineD,				-29,	0, &M_Window15[18],
	Window15+1,				-119,	0, &M_Window15[19],
	SyncLineD,				-62,	0, &M_Window15[0],
};

code struct PATTERN_ITEM M_Window20[20] = {
	SyncLineA,				-3,	0, &M_Window20[1],
	SyncLineB,				-16,	0, &M_Window20[2],
	SyncLineC,				-30,	0, &M_Window20[3],
	Window50Marker, 		-1,	0, &M_Window20[4],
	MarkerLine,				-1,	0, &M_Window20[5],
	SyncLineC,				-1,	0, &M_Window20[6],
	Marker20,				-1,	0, &M_Window20[7],
	SyncLineC,				-29,	0, &M_Window20[8],
	Window20,				-119,	0, &M_Window20[9],
	SyncLineC,				-62,	0, &M_Window20[10],

	SyncLineB,				-2,	0, &M_Window20[11],
	SyncLineA,				-17,	0, &M_Window20[12],
	SyncLineD,				-29,	0, &M_Window20[13],
	Window50Marker+1,		-1,	0, &M_Window20[14],
	MarkerLine+1,			-1,	0, &M_Window20[15],
	SyncLineD,				-1,	0, &M_Window20[16],
	Marker20+1,				-1,	0, &M_Window20[17],
	SyncLineD,				-29,	0, &M_Window20[18],
	Window20+1,				-119,	0, &M_Window20[19],
	SyncLineD,				-62,	0, &M_Window20[0],
};
	
code struct PATTERN_ITEM M_Window100[20] = {
	SyncLineA,				-3,	0, &M_Window100[1],
	SyncLineB,				-16,	0, &M_Window100[2],
	SyncLineC,				-30,	0, &M_Window100[3],
	Window50Marker, 		-1,	0, &M_Window100[4],
	MarkerLine,				-1,	0, &M_Window100[5],
	SyncLineC,				-1,	0, &M_Window100[6],
	Marker100,				-1,	0, &M_Window100[7],
	SyncLineC,				-29,	0, &M_Window100[8],
	Window100,				-119,	0, &M_Window100[9],
	SyncLineC,				-62,	0, &M_Window100[10],

	SyncLineB,				-2,	0, &M_Window100[11],
	SyncLineA,				-17,	0, &M_Window100[12],
	SyncLineD,				-29,	0, &M_Window100[13],
	Window50Marker+1,		-1,	0, &M_Window100[14],
	MarkerLine+1,			-1,	0, &M_Window100[15],
	SyncLineD,				-1,	0, &M_Window100[16],
	Marker100+1,			-1,	0, &M_Window100[17],
	SyncLineD,				-29,	0, &M_Window100[18],
	Window100+1,			-119,	0, &M_Window100[19],
	SyncLineD,				-62,	0, &M_Window100[0],
};

code struct PATTERN_ITEM M_CrossHatch[62] = {
	SyncLineA,				-3,	0, &M_CrossHatch[1],
	SyncLineB,				-16,	0, &M_CrossHatch[2],
	CrossHatch17,			-9,	0, &M_CrossHatch[3],
	WhiteLine_525,			-1,	0, &M_CrossHatch[4],
	CrossHatch17,			-16,	0, &M_CrossHatch[5],
	WhiteLine_525,			-1,	0, &M_CrossHatch[6],
	CrossHatch17,			-17,	0, &M_CrossHatch[7],
	WhiteLine_525,			-1,	0, &M_CrossHatch[8],
	CrossHatch17,			-16,	0, &M_CrossHatch[9],
	WhiteLine_525,			-1,	0, &M_CrossHatch[10],
	CrossHatch17,			-17,	0, &M_CrossHatch[11],
	WhiteLine_525,			-1,	0, &M_CrossHatch[12],
	CrossHatch17,			-16,	0, &M_CrossHatch[13],
	WhiteLine_525,			-1,	0, &M_CrossHatch[14],
	CrossHatch17,			-17,	0, &M_CrossHatch[15],
	WhiteLine_525,			-1,	0, &M_CrossHatch[16],
	CrossHatch17,			-16,	0, &M_CrossHatch[17],
	WhiteLine_525,			-1,	0, &M_CrossHatch[18],
	CrossHatch17,			-17,	0, &M_CrossHatch[19],
	WhiteLine_525,			-1,	0, &M_CrossHatch[20],
	CrossHatch17,			-16,	0, &M_CrossHatch[21],
	WhiteLine_525,			-1,	0, &M_CrossHatch[22],
	CrossHatch17,			-17,	0, &M_CrossHatch[23],
	WhiteLine_525,			-1,	0, &M_CrossHatch[24],
	CrossHatch17,			-16,	0, &M_CrossHatch[25],
	WhiteLine_525,			-1,	0, &M_CrossHatch[26],
	CrossHatch17,			-17,	0, &M_CrossHatch[27],
	WhiteLine_525,			-1,	0, &M_CrossHatch[28],
	CrossHatch17,			-16,	0, &M_CrossHatch[29],
	WhiteLine_525,			-1,	0, &M_CrossHatch[30],
	CrossHatch17,			-7,	0, &M_CrossHatch[31],

	SyncLineB,				-2,	0, &M_CrossHatch[32],
	SyncLineA,				-17,	0, &M_CrossHatch[33],
	CrossHatch17+1,		-8,	0, &M_CrossHatch[34],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[35],
	CrossHatch17+1,		-17,	0, &M_CrossHatch[36],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[37],
	CrossHatch17+1,		-16,	0, &M_CrossHatch[38],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[39],
	CrossHatch17+1,		-17,	0, &M_CrossHatch[40],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[41],
	CrossHatch17+1,		-16,	0, &M_CrossHatch[42],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[43],
	CrossHatch17+1,		-17,	0, &M_CrossHatch[44],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[45],
	CrossHatch17+1,		-16,	0, &M_CrossHatch[46],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[47],
	CrossHatch17+1,		-17,	0, &M_CrossHatch[48],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[49],
	CrossHatch17+1,		-16,	0, &M_CrossHatch[50],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[51],
	CrossHatch17+1,		-17,	0, &M_CrossHatch[52],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[53],
	CrossHatch17+1,		-16,	0, &M_CrossHatch[54],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[55],
	CrossHatch17+1,		-17,	0, &M_CrossHatch[56],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[57],
	CrossHatch17+1,		-16,	0, &M_CrossHatch[58],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[59],
	CrossHatch17+1,		-17,	0, &M_CrossHatch[60],
	WhiteLine_525+1,		-1,	0, &M_CrossHatch[61],
	CrossHatch17+1,		-6,	0, &M_CrossHatch[0],
};

code struct PATTERN_ITEM M_PLUGE[16] = {
	SyncLineA,				-3,	0, &M_PLUGE[1],
	SyncLineB,				-16,	0, &M_PLUGE[2],
	SyncLineC,				-53,	0, &M_PLUGE[3],
	PlugeWhite_525, 		-56,	0, &M_PLUGE[4],
	PlugeGreyC_525,		-19,	0, &M_PLUGE[5],
	PlugeGreyD_525,		-19,	0, &M_PLUGE[6],
	PlugeGreyE_525,		-19,	0, &M_PLUGE[7],
	SyncLineC,				-78,	0, &M_PLUGE[8],

	SyncLineB,				-2,	0, &M_PLUGE[9],
	SyncLineA,				-17,	0, &M_PLUGE[10],
	SyncLineD,				-52,	0, &M_PLUGE[11],
	PlugeWhite_525+1,		-56,	0, &M_PLUGE[12],
	PlugeGreyC_525+1,		-19,	0, &M_PLUGE[13],
	PlugeGreyD_525+1,		-19,	0, &M_PLUGE[14],
	PlugeGreyE_525+1,		-19,	0, &M_PLUGE[15],
	SyncLineD,				-78,	0, &M_PLUGE[0],
};

code struct PATTERN_ITEM M_Multiburst[6] = {
	SyncLineA,		  		-3,	0, &M_Multiburst[1],
	SyncLineB,	 			-16,	0, &M_Multiburst[2],
	FCCMultiBurst_525,	-244,	0, &M_Multiburst[3],

	SyncLineB,  			-2,	0, &M_Multiburst[4],
	SyncLineA,				-17,	0, &M_Multiburst[5],
	FCCMultiBurst_525+1,-243,	0, &M_Multiburst[0],
};

code struct PATTERN_ITEM M_Red75[6] = {
	SyncLineA,		  		-3,	0, &M_Red75[1],
	SyncLineB,	 			-16,	0, &M_Red75[2],
	Red75_525,  			-244,	0, &M_Red75[3],

	SyncLineB,  			-2,	0, &M_Red75[4],
	SyncLineA,				-17,	0, &M_Red75[5],
	Red75_525+1, 			-243,	0, &M_Red75[0],
};

#if PT8633Version
	code struct PATTERN_ITEM M_Philips4x3[16] = {
		SyncLineA,		  		-3,	0, &M_Philips4x3[1],
		SyncLineB,	 			-16,	0, &M_Philips4x3[2],
		CounterType,			-115,	4804, &M_Philips4x3[3],		// Normal
		CounterType,			-16,	1, &M_Philips4x3[4],			// 1: Date/Time
		CounterType,			-17,	2, &M_Philips4x3[5],			// 2: PLUGE
		CounterType,			-17,	5100, &M_Philips4x3[6],		// Normal
		CounterType,			-17,	3, &M_Philips4x3[7],			// 3: Greyscale
		CounterType,			-62,	5168, &M_Philips4x3[8],		// Normal

		SyncLineB,  			-2,	0, &M_Philips4x3[9],
		SyncLineA,				-17,	0, &M_Philips4x3[10],
		CounterType,			-114,	4805, &M_Philips4x3[11],	// Normal
		CounterType,			-16,	4, &M_Philips4x3[12],		// 4: Date/Time
		CounterType,			-17,	5, &M_Philips4x3[13],		// 5: PLUGE
		CounterType,			-17,	5099, &M_Philips4x3[14],	// Normal
		CounterType,			-17,	6, &M_Philips4x3[15],		// 6: Greyscale
		CounterType,			-62,	5167, &M_Philips4x3[0],		// Normal
	};
#endif

code struct PATTERN_ITEM M_AnalogBlanking[6] = {
	SyncLineA,		  		-3,	0, &M_AnalogBlanking[1],
	SyncLineB,	 			-16,	0, &M_AnalogBlanking[2],
	TestAnalogBlanking,	-244,	0, &M_AnalogBlanking[3],

	SyncLineB,  			-2,	0, &M_AnalogBlanking[4],
	SyncLineA,				-17,	0, &M_AnalogBlanking[5],
	TestAnalogBlanking+1,-243,	0, &M_AnalogBlanking[0],
};

code struct PATTERN_ITEM M_Multipulse[6] = {
	SyncLineA,		  		-3,	0, &M_Multipulse[1],
	SyncLineB,	 			-16,	0, &M_Multipulse[2],
	Multipulse,				-244,	0, &M_Multipulse[3],

	SyncLineB,  			-2,	0, &M_Multipulse[4],
	SyncLineA,				-17,	0, &M_Multipulse[5],
	Multipulse+1,			-243,	0, &M_Multipulse[0],
};

#if PT8633Version
	code struct PATTERN_ITEM M_Sinxx[6] = {
		SyncLineA,		  		-3,	0, &M_Sinxx[1],
		SyncLineB,	 			-16,	0, &M_Sinxx[2],
		sinc_525,				-244,	0, &M_Sinxx[3],
	
		SyncLineB,  			-2,	0, &M_Sinxx[4],
		SyncLineA,				-17,	0, &M_Sinxx[5],
		sinc_525+1,				-243,	0, &M_Sinxx[0],
	};
#endif

code struct PATTERN_ITEM M_DigitalGrey[6] = {
	SyncLineA,		  		-3,	0, &M_DigitalGrey[1],
	SyncLineB,	 			-16,	0, &M_DigitalGrey[2],
	DigitalGrey_525,		-244,	0, &M_DigitalGrey[3],

	SyncLineB,  			-2,	0, &M_DigitalGrey[4],
	SyncLineA,				-17,	0, &M_DigitalGrey[5],
	DigitalGrey_525+1,	-243,	0, &M_DigitalGrey[0],
};

code struct PATTERN_ITEM M_FieldTimingChk[12] = {
	SyncLineA,				-3,	0, &M_FieldTimingChk[1],
	SyncLineB,				-16,	0, &M_FieldTimingChk[2],
	SyncLineC,				-244,	0, &M_FieldTimingChk[3],

	SyncLineB,				-2,	0, &M_FieldTimingChk[4],
	SyncLineA,				-17,	0, &M_FieldTimingChk[5],
	Y200mV_625+1,			-243,	0, &M_FieldTimingChk[6],

	SyncLineA,				-3,	0, &M_FieldTimingChk[7],
	SyncLineB,				-16,	0, &M_FieldTimingChk[8],
	Y400mV_625,				-244,	0, &M_FieldTimingChk[9],

	SyncLineB,				-2,	0, &M_FieldTimingChk[10],
	SyncLineA,				-17,	0, &M_FieldTimingChk[11],
	Y600mV_625+1,			-243,	0, &M_FieldTimingChk[0],
};

code struct PATTERN_ITEM M_ValidRamp[6] = {
	SyncLineA,		  		-3,	0, &M_ValidRamp[1],
	SyncLineB,	 			-16,	0, &M_ValidRamp[2],
	ValidRampY,				-244,	0, &M_ValidRamp[3],

	SyncLineB,  			-2,	0, &M_ValidRamp[4],
	SyncLineA,				-17,	0, &M_ValidRamp[5],
	ValidRampY+1,			-243,	0, &M_ValidRamp[0],
};

code struct PATTERN_ITEM M_Stair5[6] = {
	SyncLineA,		  		-3,	0, &M_Stair5[1],
	SyncLineB,	 			-16,	0, &M_Stair5[2],
	GreyScale5_525,		-244,	0, &M_Stair5[3],

	SyncLineB,  			-2,	0, &M_Stair5[4],
	SyncLineA,				-17,	0, &M_Stair5[5],
	GreyScale5_525+1, 	-243,	0, &M_Stair5[0],
};

#if PT8633Version
	code struct PATTERN_ITEM M_Stair10[6] = {
		SyncLineA,		  		-3,	0, &M_Stair10[1],
		SyncLineB,	 			-16,	0, &M_Stair10[2],
		GreyScale10_525,		-244,	0, &M_Stair10[3],

		SyncLineB,  			-2,	0, &M_Stair10[4],
		SyncLineA,				-17,	0, &M_Stair10[5],
		GreyScale10_525+1, 	-243,	0, &M_Stair10[0],
	};
#endif

code struct PATTERN_ITEM M_LuminanceSweep[6] = {
	SyncLineA,		  		-3,	0, &M_LuminanceSweep[1],
	SyncLineB,	 			-16,	0, &M_LuminanceSweep[2],
	LineSweepY_525,		-244,	0, &M_LuminanceSweep[3],

	SyncLineB,  			-2,	0, &M_LuminanceSweep[4],
	SyncLineA,				-17,	0, &M_LuminanceSweep[5],
	LineSweepY_525+1, 	-243,	0, &M_LuminanceSweep[0],
};

#if PT8633Version
	code struct PATTERN_ITEM M_YCrCbSweep[6] = {
		SyncLineA,		  		-3,	0, &M_YCrCbSweep[1],
		SyncLineB,	 			-16,	0, &M_YCrCbSweep[2],
		_ChromaLineSweep_525,-244,	0, &M_YCrCbSweep[3],

		SyncLineB,  			-2,	0, &M_YCrCbSweep[4],
		SyncLineA,				-17,	0, &M_YCrCbSweep[5],
		_ChromaLineSweep_525+1,-243,	0, &M_YCrCbSweep[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_Flat100[6] = {
		SyncLineA,		  		-3,	0, &M_Flat100[1],
		SyncLineB,	 			-16,	0, &M_Flat100[2],
		WhiteLine_525,			-244,	0, &M_Flat100[3],

		SyncLineB,  			-2,	0, &M_Flat100[4],
		SyncLineA,				-17,	0, &M_Flat100[5],
		WhiteLine_525+1,	 	-243,	0, &M_Flat100[0],
	};
#endif

code struct PATTERN_ITEM M_BowTie[10] = {
	SyncLineA,		  		-3,	0, &M_BowTie[1],
	SyncLineB,	 			-16,	0, &M_BowTie[2],
	MarkerForBowTie,		-25,	0, &M_BowTie[3],
	BowTie,					-194,	0, &M_BowTie[4],
	MarkerForBowTie,		-25,	0, &M_BowTie[5],

	SyncLineB,  			-2,	0, &M_BowTie[6],
	SyncLineA,				-17,	0, &M_BowTie[7],
	MarkerForBowTie+1,	-24,	0, &M_BowTie[8],
	BowTie+1,				-194,	0, &M_BowTie[9],
	MarkerForBowTie+1,	-25,	0, &M_BowTie[0],
};

#if PT8633Version
	code struct PATTERN_ITEM M_AltBlWh[6] = {
		SyncLineA,		  		-3,	0, &M_AltBlWh[1],
		SyncLineB,	 			-16,	0, &M_AltBlWh[2],
		GreyScale10_525,		-244,	0, &M_AltBlWh[3],

		SyncLineB,  			-2,	0, &M_AltBlWh[4],
		SyncLineA,				-17,	0, &M_AltBlWh[5],
		GreyScale10_525+1, 	-243,	0, &M_AltBlWh[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_EndOfLine[6] = {
		SyncLineA,		  		-3,	0, &M_EndOfLine[1],
		SyncLineB,	 			-16,	0, &M_EndOfLine[2],
		EndOfLinePorches,		-244,	0, &M_EndOfLine[3],

		SyncLineB,  			-2,	0, &M_EndOfLine[4],
		SyncLineA,				-17,	0, &M_EndOfLine[5],
		EndOfLinePorches+1, 	-243,	0, &M_EndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_WhiteEndOfLine[6] = {
		SyncLineA,		  		-3,	0, &M_WhiteEndOfLine[1],
		SyncLineB,	 			-16,	0, &M_WhiteEndOfLine[2],
		WhiteEoflPorch,		-244,	0, &M_WhiteEndOfLine[3],

		SyncLineB,  			-2,	0, &M_WhiteEndOfLine[4],
		SyncLineA,				-17,	0, &M_WhiteEndOfLine[5],
		WhiteEoflPorch+1, 	-243,	0, &M_WhiteEndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_BlueEndOfLine[6] = {
		SyncLineA,		  		-3,	0, &M_BlueEndOfLine[1],
		SyncLineB,	 			-16,	0, &M_BlueEndOfLine[2],
		BlueEoflPorch,			-244,	0, &M_BlueEndOfLine[3],

		SyncLineB,  			-2,	0, &M_BlueEndOfLine[4],
		SyncLineA,				-17,	0, &M_BlueEndOfLine[5],
		BlueEoflPorch+1, 		-243,	0, &M_BlueEndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_RedEndOfLine[6] = {
		SyncLineA,		  		-3,	0, &M_RedEndOfLine[1],
		SyncLineB,	 			-16,	0, &M_RedEndOfLine[2],
		RedEoflPorch,			-244,	0, &M_RedEndOfLine[3],

		SyncLineB,  			-2,	0, &M_RedEndOfLine[4],
		SyncLineA,				-17,	0, &M_RedEndOfLine[5],
		RedEoflPorch+1, 		-243,	0, &M_RedEndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_YellowEndOfLine[6] = {
		SyncLineA,		  		-3,	0, &M_YellowEndOfLine[1],
		SyncLineB,	 			-16,	0, &M_YellowEndOfLine[2],
		YellowEoflPorch,		-244,	0, &M_YellowEndOfLine[3],

		SyncLineB,  			-2,	0, &M_YellowEndOfLine[4],
		SyncLineA,				-17,	0, &M_YellowEndOfLine[5],
		YellowEoflPorch+1, 	-243,	0, &M_YellowEndOfLine[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_CyanEndOfLine[6] = {
		SyncLineA,		  		-3,	0, &M_CyanEndOfLine[1],
		SyncLineB,	 			-16,	0, &M_CyanEndOfLine[2],
		CyanEoflPorch,			-244,	0, &M_CyanEndOfLine[3],

		SyncLineB,  			-2,	0, &M_CyanEndOfLine[4],
		SyncLineA,				-17,	0, &M_CyanEndOfLine[5],
		CyanEoflPorch+1,	 	-243,	0, &M_CyanEndOfLine[0],
	};
#endif

code struct PATTERN_ITEM M_LuminanceRamp[6] = {
	SyncLineA,		  		-3,	0, &M_LuminanceRamp[1],
	SyncLineB,	 			-16,	0, &M_LuminanceRamp[2],
	HelLinierampe_525,	-244,	0, &M_LuminanceRamp[3],

	SyncLineB,  			-2,	0, &M_LuminanceRamp[4],
	SyncLineA,				-17,	0, &M_LuminanceRamp[5],
	HelLinierampe_525+1,	-243,	0, &M_LuminanceRamp[0],
};

code struct PATTERN_ITEM M_LimitRamp[6] = {
	SyncLineA,		  		-3,	0, &M_LimitRamp[1],
	SyncLineB,	 			-16,	0, &M_LimitRamp[2],
	BlackWhiteRamp,		-244,	0, &M_LimitRamp[3],

	SyncLineB,  			-2,	0, &M_LimitRamp[4],
	SyncLineA,				-17,	0, &M_LimitRamp[5],
	BlackWhiteRamp+1, 	-243,	0, &M_LimitRamp[0],
};

code struct PATTERN_ITEM M_PulseBar[6] = {
	SyncLineA,		  		-3,	0, &M_PulseBar[1],
	SyncLineB,	 			-16,	0, &M_PulseBar[2],
	T2T20BarInv2T_525,	-244,	0, &M_PulseBar[3],

	SyncLineB,  			-2,	0, &M_PulseBar[4],
	SyncLineA,				-17,	0, &M_PulseBar[5],
	T2T20BarInv2T_525+1, -243,	0, &M_PulseBar[0],
};

#if PT8633Version
	code struct PATTERN_ITEM M_YellowGreyRamp[6] = {
		SyncLineA,		  		-3,	0, &M_YellowGreyRamp[1],
		SyncLineB,	 			-16,	0, &M_YellowGreyRamp[2],
		YellowGreyRamp,		-244,	0, &M_YellowGreyRamp[3],

		SyncLineB,  			-2,	0, &M_YellowGreyRamp[4],
		SyncLineA,				-17,	0, &M_YellowGreyRamp[5],
		YellowGreyRamp+1, 	-243,	0, &M_YellowGreyRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_GreyBlueRamp[6] = {
		SyncLineA,		  		-3,	0, &M_GreyBlueRamp[1],
		SyncLineB,	 			-16,	0, &M_GreyBlueRamp[2],
		GreyBlueRamp,			-244,	0, &M_GreyBlueRamp[3],

		SyncLineB,  			-2,	0, &M_GreyBlueRamp[4],
		SyncLineA,				-17,	0, &M_GreyBlueRamp[5],
		GreyBlueRamp+1,	 	-243,	0, &M_GreyBlueRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_CyanGreyRamp[6] = {
		SyncLineA,		  		-3,	0, &M_CyanGreyRamp[1],
		SyncLineB,	 			-16,	0, &M_CyanGreyRamp[2],
		CyanGreyRamp,			-244,	0, &M_CyanGreyRamp[3],

		SyncLineB,  			-2,	0, &M_CyanGreyRamp[4],
		SyncLineA,				-17,	0, &M_CyanGreyRamp[5],
		CyanGreyRamp+1,		-243,	0, &M_CyanGreyRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_GreyRedRamp[6] = {
		SyncLineA,		  		-3,	0, &M_GreyRedRamp[1],
		SyncLineB,	 			-16,	0, &M_GreyRedRamp[2],
		GreyRedRamp,			-244,	0, &M_GreyRedRamp[3],

		SyncLineB,  			-2,	0, &M_GreyRedRamp[4],
		SyncLineA,				-17,	0, &M_GreyRedRamp[5],
		GreyRedRamp+1,		 	-243,	0, &M_GreyRedRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_CbYCrYRamp[6] = {
		SyncLineA,		  		-3,	0, &M_CbYCrYRamp[1],
		SyncLineB,	 			-16,	0, &M_CbYCrYRamp[2],
		SavtandYCbCrY,			-244,	0, &M_CbYCrYRamp[3],

		SyncLineB,  			-2,	0, &M_CbYCrYRamp[4],
		SyncLineA,				-17,	0, &M_CbYCrYRamp[5],
		SavtandYCbCrY+1,	 	-243,	0, &M_CbYCrYRamp[0],
	};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_Philips16x9[22] = {
		SyncLineA,		  		-3,	0, &M_Philips16x9[1],
		SyncLineB,	 			-16,	0, &M_Philips16x9[2],
		CounterType,			-18,	5292, &M_Philips16x9[3],	// Normal
		CounterType,			-57,	1, &M_Philips16x9[4],		//	1: Cirkel
		CounterType,			-40,	5442, &M_Philips16x9[5],	// Normal
		CounterType,			-16,	2, &M_Philips16x9[6],		// 2: Date/Time
		CounterType,			-17,	3, &M_Philips16x9[7],		// 3: PLUGE
		CounterType,			-17,	5588, &M_Philips16x9[8],	// Normal
		CounterType,			-17,	4, &M_Philips16x9[9],		//	4: Greyscale
		CounterType,			-44,	5, &M_Philips16x9[10],		//	5: Cirkel
		CounterType,			-18,	5744, &M_Philips16x9[11],	// Normal

		SyncLineB,  			-2,	0, &M_Philips16x9[12],
		SyncLineA,				-17,	0, &M_Philips16x9[13],
		CounterType,			-17,	5293, &M_Philips16x9[14],	// Normal
		CounterType,			-57,	6, &M_Philips16x9[15],		//	6: Cirkel
		CounterType,			-40,	5441, &M_Philips16x9[16],	// Normal
		CounterType,			-16,	7, &M_Philips16x9[17],		// 7: Date/Time
		CounterType,			-17,	8, &M_Philips16x9[18],		// 8: PLUGE
		CounterType,			-17,	5587, &M_Philips16x9[19],	// Normal
		CounterType,			-17,	9, &M_Philips16x9[20],		//	9: Greyscale
		CounterType,			-44,	10, &M_Philips16x9[21],		//	10: Cirkel
		CounterType,			-18,	5743, &M_Philips16x9[0],	// Normal
};
#endif

#if PT8633Version
	code struct PATTERN_ITEM M_SafeArea[34] = {
		SyncLineA,		  		-3,	0, &M_SafeArea[1],
		SyncLineB,	 			-16,	0, &M_SafeArea[2],
		SyncLineC,				-13,	0, &M_SafeArea[3],
		SafeAreaA_525,			-1,	0, &M_SafeArea[4],
		SyncLineC,				-12,	0, &M_SafeArea[5],
		SafeAreaB_525,			-1,	0, &M_SafeArea[6],
		SyncLineC,				-32,	0, &M_SafeArea[7],
		SafeAreaC_525,			-55,	0, &M_SafeArea[8],
		SafeAreaD_525,			-8,	0, &M_SafeArea[9],
		SafeAreaE_525,			-1,	0, &M_SafeArea[10],
		SafeAreaD_525,			-8,	0, &M_SafeArea[11],
		SafeAreaC_525,			-55,	0, &M_SafeArea[12],
		SyncLineC,				-32,	0, &M_SafeArea[13],
		SafeAreaB_525,			-1,	0, &M_SafeArea[14],
		SyncLineC,				-12,	0, &M_SafeArea[15],
		SafeAreaA_525,			-1,	0, &M_SafeArea[16],
		SyncLineC,				-12,	0, &M_SafeArea[17],

		SyncLineB,  			-2,	0, &M_SafeArea[18],
		SyncLineA,				-17,	0, &M_SafeArea[19],
		SyncLineD,				-12,	0, &M_SafeArea[20],
		SafeAreaA_525+1,		-1,	0, &M_SafeArea[21],
		SyncLineD,				-12,	0, &M_SafeArea[22],
		SafeAreaB_525+1,		-1,	0, &M_SafeArea[23],
		SyncLineD,				-32,	0, &M_SafeArea[24],
		SafeAreaC_525+1,		-55,	0, &M_SafeArea[25],
		SafeAreaD_525+1,		-8,	0, &M_SafeArea[26],
		SafeAreaE_525+1,		-1,	0, &M_SafeArea[27],
		SafeAreaD_525+1,		-8,	0, &M_SafeArea[28],
		SafeAreaC_525+1,		-55,	0, &M_SafeArea[29],
		SyncLineD,				-32,	0, &M_SafeArea[30],
		SafeAreaB_525+1,		-1,	0, &M_SafeArea[31],
		SyncLineD,				-12,	0, &M_SafeArea[32],
		SafeAreaA_525+1,		-1,	0, &M_SafeArea[33],
		SyncLineD,				-12,	0, &M_SafeArea[0],
	};
#endif

code struct PATTERN_ITEM M_FCCCBar[6] = {
	SyncLineA,		  		-3,	0, &M_FCCCBar[1],
	SyncLineB,	 			-16,	0, &M_FCCCBar[2],
	FCC_CB_525,				-244,	0, &M_FCCCBar[3],

	SyncLineB,  			-2,	0, &M_FCCCBar[4],
	SyncLineA,				-17,	0, &M_FCCCBar[5],
	FCC_CB_525+1,	 		-243,	0, &M_FCCCBar[0],
};

code struct PATTERN_ITEM M_FieldSquareWave[10] = {
	SyncLineA,		  		-3,	0, &M_FieldSquareWave[1],
	SyncLineB,	 			-16,	0, &M_FieldSquareWave[2],
	SyncLineC,				-46,	0, &M_FieldSquareWave[3],
	WhiteLine_525,			-150,	0, &M_FieldSquareWave[4],
	SyncLineC,				-48,	0, &M_FieldSquareWave[5],

	SyncLineB,  			-2,	0, &M_FieldSquareWave[6],
	SyncLineA,				-17,	0, &M_FieldSquareWave[7],
	SyncLineD,				-45,	0, &M_FieldSquareWave[8],
	WhiteLine_525+1,		-150,	0, &M_FieldSquareWave[9],
	SyncLineD,				-48,	0, &M_FieldSquareWave[0],
};

code struct PATTERN_ITEM M_ModulatedStair5[6] = {
	SyncLineA,		  		-3,	0, &M_ModulatedStair5[1],
	SyncLineB,	 			-16,	0, &M_ModulatedStair5[2],
	ModGreyScale5_525,	-244,	0, &M_ModulatedStair5[3],

	SyncLineB,  			-2,	0, &M_ModulatedStair5[4],
	SyncLineA,				-17,	0, &M_ModulatedStair5[5],
	ModGreyScale5_525+1,	-243,	0, &M_ModulatedStair5[0],
};

code struct PATTERN_ITEM M_Customized1[6] = {
	SyncLineA,		  		-3,	0, &M_Customized1[1],
	SyncLineB,	 			-16,	0, &M_Customized1[2],
	SyncLineC,  			-244,	0, &M_Customized1[3],

	SyncLineB,  			-2,	0, &M_Customized1[4],
	SyncLineA,				-17,	0, &M_Customized1[5],
	SyncLineD, 				-243,	0, &M_Customized1[0],
};

code struct PATTERN_ITEM M_Customized2[6] = {
	SyncLineA,		  		-3,	0, &M_Customized2[1],
	SyncLineB,	 			-16,	0, &M_Customized2[2],
	SyncLineC,  			-244,	0, &M_Customized2[3],

	SyncLineB,  			-2,	0, &M_Customized2[4],
	SyncLineA,				-17,	0, &M_Customized2[5],
	SyncLineD, 				-243,	0, &M_Customized2[0],
};

code struct PATTERN_ITEM M_Customized3[6] = {
	SyncLineA,		  		-3,	0, &M_Customized3[1],
	SyncLineB,	 			-16,	0, &M_Customized3[2],
	SyncLineC,  			-244,	0, &M_Customized3[3],

	SyncLineB,  			-2,	0, &M_Customized3[4],
	SyncLineA,				-17,	0, &M_Customized3[5],
	SyncLineD, 				-243,	0, &M_Customized3[0],
};

code struct PATTERN_ITEM M_Customized4[6] = {
	SyncLineA,		  		-3,	0, &M_Customized4[1],
	SyncLineB,	 			-16,	0, &M_Customized4[2],
	SyncLineC,  			-244,	0, &M_Customized4[3],

	SyncLineB,  			-2,	0, &M_Customized4[4],
	SyncLineA,				-17,	0, &M_Customized4[5],
	SyncLineD, 				-243,	0, &M_Customized4[0],
};

code struct PATTERN_ITEM M_Customized5[6] = {
	SyncLineA,		  		-3,	0, &M_Customized5[1],
	SyncLineB,	 			-16,	0, &M_Customized5[2],
	SyncLineC,  			-244,	0, &M_Customized5[3],

	SyncLineB,  			-2,	0, &M_Customized5[4],
	SyncLineA,				-17,	0, &M_Customized5[5],
	SyncLineD, 				-243,	0, &M_Customized5[0],
};

code struct PATTERN_ITEM M_Customized6[6] = {
	SyncLineA,		  		-3,	0, &M_Customized6[1],
	SyncLineB,	 			-16,	0, &M_Customized6[2],
	SyncLineC,  			-244,	0, &M_Customized6[3],

	SyncLineB,  			-2,	0, &M_Customized6[4],
	SyncLineA,				-17,	0, &M_Customized6[5],
	SyncLineD, 				-243,	0, &M_Customized6[0],
};

code struct PATTERN_ITEM M_Customized7[6] = {
	SyncLineA,		  		-3,	0, &M_Customized7[1],
	SyncLineB,	 			-16,	0, &M_Customized7[2],
	SyncLineC,  			-244,	0, &M_Customized7[3],

	SyncLineB,  			-2,	0, &M_Customized7[4],
	SyncLineA,				-17,	0, &M_Customized7[5],
	SyncLineD, 				-243,	0, &M_Customized7[0],
};

code struct PATTERN_ITEM M_Customized8[6] = {
	SyncLineA,		  		-3,	0, &M_Customized8[1],
	SyncLineB,	 			-16,	0, &M_Customized8[2],
	SyncLineC,  			-244,	0, &M_Customized8[3],

	SyncLineB,  			-2,	0, &M_Customized8[4],
	SyncLineA,				-17,	0, &M_Customized8[5],
	SyncLineD, 				-243,	0, &M_Customized8[0],
};

code struct PATTERN_ITEM M_Window10[20] = {
	SyncLineA,				-3,	0, &M_Window10[1],
	SyncLineB,				-16,	0, &M_Window10[2],
	SyncLineC,				-30,	0, &M_Window10[3],
	Window50Marker, 		-1,	0, &M_Window10[4],
	MarkerLine,				-1,	0, &M_Window10[5],
	SyncLineC,				-1,	0, &M_Window10[6],
	Marker10,				-1,	0, &M_Window10[7],
	SyncLineC,				-29,	0, &M_Window10[8],
	Window10,				-119,	0, &M_Window10[9],
	SyncLineC,				-62,	0, &M_Window10[10],

	SyncLineB,				-2,	0, &M_Window10[11],
	SyncLineA,				-17,	0, &M_Window10[12],
	SyncLineD,				-29,	0, &M_Window10[13],
	Window50Marker+1,		-1,	0, &M_Window10[14],
	MarkerLine+1,			-1,	0, &M_Window10[15],
	SyncLineD,				-1,	0, &M_Window10[16],
	Marker10+1,				-1,	0, &M_Window10[17],
	SyncLineD,				-29,	0, &M_Window10[18],
	Window10+1,				-119,	0, &M_Window10[19],
	SyncLineD,				-62,	0, &M_Window10[0],
};

code struct PATTERN_ITEM M_kHz15BlWh[6] = {
	SyncLineA,		  		-3,	0, &M_kHz15BlWh[1],
	SyncLineB,	 			-16,	0, &M_kHz15BlWh[2],
	kHz15BlWh_525,			-244,	0, &M_kHz15BlWh[3],

	SyncLineB,  			-2,	0, &M_kHz15BlWh[4],
	SyncLineA,				-17,	0, &M_kHz15BlWh[5],
	kHz15BlWh_525+1,	 	-243,	0, &M_kHz15BlWh[0],
};
