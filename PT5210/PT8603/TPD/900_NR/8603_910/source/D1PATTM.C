/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8603 SDI Digital test signal generator							*/
/*	Module:		D1PATTM.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	989029, KEn	DEV															*/
/*	Status:		Version 1.1																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/* Changes:																						*/
/* 980929: Increased number of patterns to 66 to support Greyscale			*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <intrpt.h>

#include "d1gener.h"
#include "d1patt.h"

code struct PATTERN_ITEM* code M_PatternTable[66] = {
	&M_SMPTECBar[2],
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Check[2],
	&M_ShallowRamp[2],
	&M_DigTimTest[2],
	&M_Black[2],
	&M_Window15[2],
	&M_Window20[2],
	&M_Window100[2],
	&M_CrossHatch[2],
	&M_PLUGE[2],
	&M_MultiBurst[2],
	&M_Red75[2],
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Stair5[2],
	&M_Stair10[2],
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
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Dummy,
	&M_Customized1[1],
	&M_Customized2[1],
	&M_Customized3[1],
	&M_Customized4[1],
	&M_Customized5[1],
	&M_Customized6[1],
	&M_Customized7[1],
	&M_Customized8[1],
	&M_Dummy,
	&M_Dummy,
	&M_Dummy
};

/***************************************************************************/
// DEFINITION OF PATTERN ARRAYS, NTSC
/***************************************************************************/

code struct PATTERN_ITEM M_Dummy;

code struct PATTERN_ITEM M_SMPTECBar[10] = {
	SyncLineA,  			-3,	&M_SMPTECBar[1],			// Line 
	SyncLineB,	 			-16,	&M_SMPTECBar[2],
	SMPTEColorBar,  		-164,	&M_SMPTECBar[3],
	BlueBars,  				-19,	&M_SMPTECBar[4],
	IWhiteQ,  				-61,	&M_SMPTECBar[5],
	SyncLineB,  			-2,	&M_SMPTECBar[6],
	SyncLineA,				-17,	&M_SMPTECBar[7],
	SMPTEColorBar+1,		-163,	&M_SMPTECBar[8],
	BlueBars+1,  			-19,	&M_SMPTECBar[9],
	IWhiteQ+1,  			-61,	&M_SMPTECBar[0],
};

code struct PATTERN_ITEM M_Check[9] = {
	SyncLineA,		  		-3,	&M_Check[1],				// Line
	SyncLineB,	 			-16,	&M_Check[2],
	SDI_test_line1, 		-1,	&M_Check[3],
	SDI_test_line2, 		-121,	&M_Check[4],
	SDI_test_line3, 		-122,	&M_Check[5],
	SyncLineB,  			-2,	&M_Check[6],
	SyncLineA,				-17,	&M_Check[7],
	SDI_test_line2+1,		-121,	&M_Check[8],
	SDI_test_line3+1, 	-122,	&M_Check[0],
};

code struct PATTERN_ITEM M_ShallowRamp[6] = {
	SyncLineA,		  		-3,	&M_ShallowRamp[1],		// Line
	SyncLineB,	 			-16,	&M_ShallowRamp[2],
	ShallowRamp_525,		-244,	&M_ShallowRamp[3],
	SyncLineB,  			-2,	&M_ShallowRamp[4],
	SyncLineA,				-17,	&M_ShallowRamp[5],
	ShallowRamp_525+1, 	-243,	&M_ShallowRamp[0],
};

code struct PATTERN_ITEM M_DigTimTest[10] = {
	SyncLineA,		  		-3,	&M_DigTimTest[1],			// Line
	SyncLineB,	 			-16,	&M_DigTimTest[2],
	WhiteLine_525,			-1,	&M_DigTimTest[3],
	DigitalTiming_525,	-242,	&M_DigTimTest[4],
	WhiteLine_525, 		-1,	&M_DigTimTest[5],
	SyncLineB,  			-2,	&M_DigTimTest[6],
	SyncLineA,				-17,	&M_DigTimTest[7],
	WhiteLine_525+1,		-1,	&M_DigTimTest[8],
	DigitalTiming_525+1,	-241,	&M_DigTimTest[9],
	WhiteLine_525+1, 		-1,	&M_DigTimTest[0],
};

code struct PATTERN_ITEM M_Black[6] = {
	SyncLineA,		  		-3,	&M_Black[1],				// Line
	SyncLineB,	 			-16,	&M_Black[2],
	SyncLineC,  			-244,	&M_Black[3],
	SyncLineB,  			-2,	&M_Black[4],
	SyncLineA,				-17,	&M_Black[5],
	SyncLineD, 				-243,	&M_Black[0],
};

code struct PATTERN_ITEM M_Window15[20] = {
	SyncLineA,				-3,	&M_Window15[1],			// Line 525
	SyncLineB,				-16,	&M_Window15[2],
	SyncLineC,				-30,	&M_Window15[3],
	Window50Marker, 		-1,	&M_Window15[4],
	MarkerLine,				-1,	&M_Window15[5],
	SyncLineC,				-1,	&M_Window15[6],
	Marker15,				-1,	&M_Window15[7],
	SyncLineC,				-29,	&M_Window15[8],
	Window15,				-119,	&M_Window15[9],
	SyncLineC,				-62,	&M_Window15[10],
	SyncLineB,				-2,	&M_Window15[11],
	SyncLineA,				-17,	&M_Window15[12],
	SyncLineD,				-29,	&M_Window15[13],
	Window50Marker+1,		-1,	&M_Window15[14],
	MarkerLine+1,			-1,	&M_Window15[15],
	SyncLineD,				-1,	&M_Window15[16],
	Marker15+1,				-1,	&M_Window15[17],
	SyncLineD,				-29,	&M_Window15[18],
	Window15+1,				-119,	&M_Window15[19],
	SyncLineD,				-62,	&M_Window15[0],
};

code struct PATTERN_ITEM M_Window20[20] = {
	SyncLineA,				-3,	&M_Window20[1],			// Line 525
	SyncLineB,				-16,	&M_Window20[2],
	SyncLineC,				-30,	&M_Window20[3],
	Window50Marker, 		-1,	&M_Window20[4],
	MarkerLine,				-1,	&M_Window20[5],
	SyncLineC,				-1,	&M_Window20[6],
	Marker20,				-1,	&M_Window20[7],
	SyncLineC,				-29,	&M_Window20[8],
	Window20,				-119,	&M_Window20[9],
	SyncLineC,				-62,	&M_Window20[10],
	SyncLineB,				-2,	&M_Window20[11],
	SyncLineA,				-17,	&M_Window20[12],
	SyncLineD,				-29,	&M_Window20[13],
	Window50Marker+1,		-1,	&M_Window20[14],
	MarkerLine+1,			-1,	&M_Window20[15],
	SyncLineD,				-1,	&M_Window20[16],
	Marker20+1,				-1,	&M_Window20[17],
	SyncLineD,				-29,	&M_Window20[18],
	Window20+1,				-119,	&M_Window20[19],
	SyncLineD,				-62,	&M_Window20[0],
};
	
code struct PATTERN_ITEM M_Window100[20] = {
	SyncLineA,				-3,	&M_Window100[1],			// Line 525
	SyncLineB,				-16,	&M_Window100[2],
	SyncLineC,				-30,	&M_Window100[3],
	Window50Marker, 		-1,	&M_Window100[4],
	MarkerLine,				-1,	&M_Window100[5],
	SyncLineC,				-1,	&M_Window100[6],
	Marker100,				-1,	&M_Window100[7],
	SyncLineC,				-29,	&M_Window100[8],
	Window100,				-119,	&M_Window100[9],
	SyncLineC,				-62,	&M_Window100[10],
	SyncLineB,				-2,	&M_Window100[11],
	SyncLineA,				-17,	&M_Window100[12],
	SyncLineD,				-29,	&M_Window100[13],
	Window50Marker+1,		-1,	&M_Window100[14],
	MarkerLine+1,			-1,	&M_Window100[15],
	SyncLineD,				-1,	&M_Window100[16],
	Marker100+1,			-1,	&M_Window100[17],
	SyncLineD,				-29,	&M_Window100[18],
	Window100+1,			-119,	&M_Window100[19],
	SyncLineD,				-62,	&M_Window100[0],
};

code struct PATTERN_ITEM M_CrossHatch[62] = {
	SyncLineA,				-3,	&M_CrossHatch[1],			// Line 525
	SyncLineB,				-16,	&M_CrossHatch[2],
	CrossHatch17,			-9,	&M_CrossHatch[3],
	WhiteLine_525,			-1,	&M_CrossHatch[4],
	CrossHatch17,			-16,	&M_CrossHatch[5],
	WhiteLine_525,			-1,	&M_CrossHatch[6],
	CrossHatch17,			-17,	&M_CrossHatch[7],
	WhiteLine_525,			-1,	&M_CrossHatch[8],
	CrossHatch17,			-16,	&M_CrossHatch[9],
	WhiteLine_525,			-1,	&M_CrossHatch[10],
	CrossHatch17,			-17,	&M_CrossHatch[11],
	WhiteLine_525,			-1,	&M_CrossHatch[12],
	CrossHatch17,			-16,	&M_CrossHatch[13],
	WhiteLine_525,			-1,	&M_CrossHatch[14],
	CrossHatch17,			-17,	&M_CrossHatch[15],
	WhiteLine_525,			-1,	&M_CrossHatch[16],
	CrossHatch17,			-16,	&M_CrossHatch[17],
	WhiteLine_525,			-1,	&M_CrossHatch[18],
	CrossHatch17,			-17,	&M_CrossHatch[19],
	WhiteLine_525,			-1,	&M_CrossHatch[20],
	CrossHatch17,			-16,	&M_CrossHatch[21],
	WhiteLine_525,			-1,	&M_CrossHatch[22],
	CrossHatch17,			-17,	&M_CrossHatch[23],
	WhiteLine_525,			-1,	&M_CrossHatch[24],
	CrossHatch17,			-16,	&M_CrossHatch[25],
	WhiteLine_525,			-1,	&M_CrossHatch[26],
	CrossHatch17,			-17,	&M_CrossHatch[27],
	WhiteLine_525,			-1,	&M_CrossHatch[28],
	CrossHatch17,			-16,	&M_CrossHatch[29],
	WhiteLine_525,			-1,	&M_CrossHatch[30],
	CrossHatch17,			-7,	&M_CrossHatch[31],

	SyncLineB,				-2,	&M_CrossHatch[32],
	SyncLineA,				-17,	&M_CrossHatch[33],

	CrossHatch17+1,		-8,	&M_CrossHatch[34],
	WhiteLine_525+1,		-1,	&M_CrossHatch[35],
	CrossHatch17+1,		-17,	&M_CrossHatch[36],
	WhiteLine_525+1,		-1,	&M_CrossHatch[37],
	CrossHatch17+1,		-16,	&M_CrossHatch[38],
	WhiteLine_525+1,		-1,	&M_CrossHatch[39],
	CrossHatch17+1,		-17,	&M_CrossHatch[40],
	WhiteLine_525+1,		-1,	&M_CrossHatch[41],
	CrossHatch17+1,		-16,	&M_CrossHatch[42],
	WhiteLine_525+1,		-1,	&M_CrossHatch[43],
	CrossHatch17+1,		-17,	&M_CrossHatch[44],
	WhiteLine_525+1,		-1,	&M_CrossHatch[45],
	CrossHatch17+1,		-16,	&M_CrossHatch[46],
	WhiteLine_525+1,		-1,	&M_CrossHatch[47],
	CrossHatch17+1,		-17,	&M_CrossHatch[48],
	WhiteLine_525+1,		-1,	&M_CrossHatch[49],
	CrossHatch17+1,		-16,	&M_CrossHatch[50],
	WhiteLine_525+1,		-1,	&M_CrossHatch[51],
	CrossHatch17+1,		-17,	&M_CrossHatch[52],
	WhiteLine_525+1,		-1,	&M_CrossHatch[53],
	CrossHatch17+1,		-16,	&M_CrossHatch[54],
	WhiteLine_525+1,		-1,	&M_CrossHatch[55],
	CrossHatch17+1,		-17,	&M_CrossHatch[56],
	WhiteLine_525+1,		-1,	&M_CrossHatch[57],
	CrossHatch17+1,		-16,	&M_CrossHatch[58],
	WhiteLine_525+1,		-1,	&M_CrossHatch[59],
	CrossHatch17+1,		-17,	&M_CrossHatch[60],
	WhiteLine_525+1,		-1,	&M_CrossHatch[61],
	CrossHatch17+1,		-6,	&M_CrossHatch[0],
};

code struct PATTERN_ITEM M_PLUGE[16] = {
	SyncLineA,				-3,	&M_PLUGE[1],				// Line 625
	SyncLineB,				-16,	&M_PLUGE[2],
	SyncLineC,				-53,	&M_PLUGE[3],
	PlugeWhite_525, 		-56,	&M_PLUGE[4],
	PlugeGreyC_525,		-19,	&M_PLUGE[5],
	PlugeGreyD_525,		-19,	&M_PLUGE[6],
	PlugeGreyE_525,		-19,	&M_PLUGE[7],
	SyncLineC,				-78,	&M_PLUGE[8],
	SyncLineB,				-2,	&M_PLUGE[9],
	SyncLineA,				-17,	&M_PLUGE[10],
	SyncLineD,				-52,	&M_PLUGE[11],
	PlugeWhite_525+1,		-56,	&M_PLUGE[12],
	PlugeGreyC_525+1,		-19,	&M_PLUGE[13],
	PlugeGreyD_525+1,		-19,	&M_PLUGE[14],
	PlugeGreyE_525+1,		-19,	&M_PLUGE[15],
	SyncLineD,				-78,	&M_PLUGE[0],
};

code struct PATTERN_ITEM M_MultiBurst[6] = {
	SyncLineA,		  		-3,	&M_MultiBurst[1],			// Line
	SyncLineB,	 			-16,	&M_MultiBurst[2],
	FCC_MultiBurst_525,	-244,	&M_MultiBurst[3],
	SyncLineB,  			-2,	&M_MultiBurst[4],
	SyncLineA,				-17,	&M_MultiBurst[5],
	FCC_MultiBurst_525+1,-243,	&M_MultiBurst[0],
};

code struct PATTERN_ITEM M_Red75[6] = {
	SyncLineA,		  		-3,	&M_Red75[1],				// Line
	SyncLineB,	 			-16,	&M_Red75[2],
	Red75_525,  			-244,	&M_Red75[3],
	SyncLineB,  			-2,	&M_Red75[4],
	SyncLineA,				-17,	&M_Red75[5],
	Red75_525+1, 			-243,	&M_Red75[0],
};

code struct PATTERN_ITEM M_Stair5[6] = {
	SyncLineA,		  		-3,	&M_Stair5[1],
	SyncLineB,	 			-16,	&M_Stair5[2],
	GreyScale5_525,		-244,	&M_Stair5[3],
	SyncLineB,  			-2,	&M_Stair5[4],
	SyncLineA,				-17,	&M_Stair5[5],
	GreyScale5_525+1, 	-243,	&M_Stair5[0],
};

code struct PATTERN_ITEM M_Stair10[6] = {
	SyncLineA,		  		-3,	&M_Stair10[1],
	SyncLineB,	 			-16,	&M_Stair10[2],
	GreyScale10_525,		-244,	&M_Stair10[3],
	SyncLineB,  			-2,	&M_Stair10[4],
	SyncLineA,				-17,	&M_Stair10[5],
	GreyScale10_525+1, 	-243,	&M_Stair10[0],
};

code struct PATTERN_ITEM M_Customized1[6] = {
	SyncLineA,		  		-3,	&M_Customized1[1],
	SyncLineB,	 			-16,  &M_Customized1[2],
	SyncLineC,  			-244, &M_Customized1[3],
	SyncLineB,  			-2,	&M_Customized1[4],
	SyncLineA,				-17,	&M_Customized1[5],
	SyncLineD, 				-243,	&M_Customized1[0],
};

code struct PATTERN_ITEM M_Customized2[6] = {
	SyncLineA,		  		-3,	&M_Customized2[1],
	SyncLineB,	 			-16,	&M_Customized2[2],
	SyncLineC,  			-244,	&M_Customized2[3],
	SyncLineB,  			-2,	&M_Customized2[4],
	SyncLineA,				-17,	&M_Customized2[5],
	SyncLineD, 				-243,	&M_Customized2[0],
};

code struct PATTERN_ITEM M_Customized3[6] = {
	SyncLineA,		  		-3,	&M_Customized3[1],
	SyncLineB,	 			-16,	&M_Customized3[2],
	SyncLineC,  			-244,	&M_Customized3[3],
	SyncLineB,  			-2,	&M_Customized3[4],
	SyncLineA,				-17,	&M_Customized3[5],
	SyncLineD, 				-243,	&M_Customized3[0],
};

code struct PATTERN_ITEM M_Customized4[6] = {
	SyncLineA,		  		-3,	&M_Customized4[1],
	SyncLineB,	 			-16,	&M_Customized4[2],
	SyncLineC,  			-244,	&M_Customized4[3],
	SyncLineB,  			-2,	&M_Customized4[4],
	SyncLineA,				-17,	&M_Customized4[5],
	SyncLineD, 				-243,	&M_Customized4[0],
};

code struct PATTERN_ITEM M_Customized5[6] = {
	SyncLineA,		  		-3,	&M_Customized5[1],
	SyncLineB,	 			-16,	&M_Customized5[2],
	SyncLineC,  			-244,	&M_Customized5[3],
	SyncLineB,  			-2,	&M_Customized5[4],
	SyncLineA,				-17,	&M_Customized5[5],
	SyncLineD, 				-243,	&M_Customized5[0],
};

code struct PATTERN_ITEM M_Customized6[6] = {
	SyncLineA,		  		-3,	&M_Customized6[1],
	SyncLineB,	 			-16,	&M_Customized6[2],
	SyncLineC,  			-244,	&M_Customized6[3],
	SyncLineB,  			-2,	&M_Customized6[4],
	SyncLineA,				-17,	&M_Customized6[5],
	SyncLineD, 				-243,	&M_Customized6[0],
};

code struct PATTERN_ITEM M_Customized7[6] = {
	SyncLineA,		  		-3,	&M_Customized7[1],
	SyncLineB,	 			-16,	&M_Customized7[2],
	SyncLineC,  			-244,	&M_Customized7[3],
	SyncLineB,  			-2,	&M_Customized7[4],
	SyncLineA,				-17,	&M_Customized7[5],
	SyncLineD, 				-243,	&M_Customized7[0],
};

code struct PATTERN_ITEM M_Customized8[6] = {
	SyncLineA,		  		-3,	&M_Customized8[1],
	SyncLineB,	 			-16,	&M_Customized8[2],
	SyncLineC,  			-244,	&M_Customized8[3],
	SyncLineB,  			-2,	&M_Customized8[4],
	SyncLineA,				-17,	&M_Customized8[5],
	SyncLineD, 				-243,	&M_Customized8[0],
};

