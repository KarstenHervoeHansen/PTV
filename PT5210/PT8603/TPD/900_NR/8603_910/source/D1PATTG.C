/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8603 SDI Digital test signal generator							*/
/*	Module:		D1PATTG.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	990301, KEn	DEV															*/
/*	Status:		Version 1.2																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/* Changes:																						*/
/* 990301: Splitfield EBU CB + Gray changed to 75% CB + Gray					*/
/* 980929: Increased number of patterns to 66 to support Greyscale			*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <intrpt.h>

#include "d1gener.h"
#include "d1patt.h"

code struct PATTERN_ITEM* code G_PatternTable[66] = {
	&G_Dummy,
	&G_EBUCBar[1],
	&G_EBUCBar8bit[1],
	&G_CBar100[1],
	&G_CBar75Grey[1],
	&G_CBar75Red[1],
	&G_Check[1],
	&G_ShallowRamp[1],
	&G_DigTimTest[1],
	&G_Black[1],
	&G_Window15[1],
	&G_Window20[1],
	&G_Window100[1],
	&G_CrossHatch[1],
	&G_PLUGE[1],
	&G_MultiBurst[1],
	&G_Red75[1],
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Stair5[1],
	&G_Stair10[1],
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
	&G_Dummy,
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
	&G_Dummy
};

/***************************************************************************/
// DEFINITION OF PATTERN ARRAYS, PAL-G
/***************************************************************************/

code struct PATTERN_ITEM G_Dummy;

code struct PATTERN_ITEM G_EBUCBar[6] = {
	SyncLineB,  			 -22, &G_EBUCBar[1],			// Line 625
	EBUCB_625,	 			-288, &G_EBUCBar[2],
	SyncLineB,  			  -2, &G_EBUCBar[3],
	SyncLineA,  			 -23, &G_EBUCBar[4],
	EBUCB_625+1,		  	-288, &G_EBUCBar[5],
	SyncLineA,  			  -2, &G_EBUCBar[0],
};

code struct PATTERN_ITEM G_EBUCBar8bit[6] = {
	SyncLineB,				-22,	&G_EBUCBar8bit[1],	// Line 625
	EBUCB_8bit_625,		-288,	&G_EBUCBar8bit[2],
	SyncLineB,  			-2,	&G_EBUCBar8bit[3],
	SyncLineA,  			-23,	&G_EBUCBar8bit[4],
	EBUCB_8bit_625+1,		-288,	&G_EBUCBar8bit[5],
	SyncLineA,  			-2,	&G_EBUCBar8bit[0],
};

code struct PATTERN_ITEM G_CBar100[6] = {
	SyncLineB,  			-22,	&G_CBar100[1],			// Line 625
	CB100_625,	 			-288,	&G_CBar100[2],
	SyncLineB,  			-2,	&G_CBar100[3],
	SyncLineA,  			-23,	&G_CBar100[4],
	CB100_625+1,			-288,	&G_CBar100[5],
	SyncLineA,  			-2,	&G_CBar100[0],
};

code struct PATTERN_ITEM G_CBar75Grey[8] = {
	SyncLineB,  			-22,	&G_CBar75Grey[1],			// Line 625
	CB75STD_625,	 		-192,	&G_CBar75Grey[2],
	CBarGrey75_625,		-96,	&G_CBar75Grey[3],
	SyncLineB,  			-2,	&G_CBar75Grey[4],
	SyncLineA,  			-23,	&G_CBar75Grey[5],
	CB75STD_625+1,			-192,	&G_CBar75Grey[6],
	CBarGrey75_625+1,		-96,	&G_CBar75Grey[7],
	SyncLineA, 				-2,	&G_CBar75Grey[0],
};

code struct PATTERN_ITEM G_CBar75Red[8] = {
	SyncLineB,  			-22,	&G_CBar75Red[1],			// Line 625
	EBUCB_625,				-192,	&G_CBar75Red[2],
	CBarRed75_625,			-96,	&G_CBar75Red[3],
	SyncLineB, 				-2,	&G_CBar75Red[4],
	SyncLineA,				-23,	&G_CBar75Red[5],
	EBUCB_625+1,			-192,	&G_CBar75Red[6],
	CBarRed75_625+1,		-96,	&G_CBar75Red[7],
	SyncLineA,				-2,	&G_CBar75Red[0],
};

code struct PATTERN_ITEM G_Check[9] = {
	SyncLineB,  			-22,	&G_Check[1],				// Line 625
	SDI_test_line1,		-1,	&G_Check[2],
	SDI_test_line2,		-143,	&G_Check[3],
	SDI_test_line3, 		-144,	&G_Check[4],
	SyncLineB, 				-2,	&G_Check[5],
	SyncLineA,				-23,	&G_Check[6],
	SDI_test_line2+1,		-144,	&G_Check[7],
	SDI_test_line3+1,		-144,	&G_Check[8],
	SyncLineA, 				-2,	&G_Check[0],
};

code struct PATTERN_ITEM G_ShallowRamp[6] = {
	SyncLineB,				-22,	&G_ShallowRamp[1],		// Line 625
	ShallowRamp_625,		-288,	&G_ShallowRamp[2],
	SyncLineB,				-2,	&G_ShallowRamp[3],
	SyncLineA,				-23,	&G_ShallowRamp[4],
	ShallowRamp_625+1,	-288,	&G_ShallowRamp[5],
	SyncLineA,				-2,	&G_ShallowRamp[0],
};

code struct PATTERN_ITEM G_DigTimTest[10] = {
	SyncLineB,  			-22,	&G_DigTimTest[1],			// Line 625
	WhiteLine_625,			-1,	&G_DigTimTest[2],
	DigitalTiming_625,	-286,	&G_DigTimTest[3],
	WhiteLine_625,			-1,	&G_DigTimTest[4],
	SyncLineB,				-2,	&G_DigTimTest[5],
	SyncLineA,				-23,	&G_DigTimTest[6],
	WhiteLine_625+1,		-1,	&G_DigTimTest[7],
	DigitalTiming_625+1,	-286,	&G_DigTimTest[8],
	WhiteLine_625+1,		-1,	&G_DigTimTest[9],
	SyncLineA,  			-2,	&G_DigTimTest[0],
};

code struct PATTERN_ITEM G_Black[6] = {
	SyncLineB,  			 -22, &G_Black[1],				// Line 625
	SyncLineC,	 			-288, &G_Black[2],
	SyncLineB,  			  -2, &G_Black[3],
	SyncLineA,  			 -23, &G_Black[4],
	SyncLineD,		  		-288, &G_Black[5],
	SyncLineA,  			  -2, &G_Black[0],
};

code struct PATTERN_ITEM G_Window15[20] = {
	SyncLineB, 				-22,	&G_Window15[1],			// Line 625
	SyncLineC,				-24,	&G_Window15[2],
	Window50Marker,		-1,	&G_Window15[3],
	MarkerLine,				-1,	&G_Window15[4],
	SyncLineC,				-1,	&G_Window15[5],
	Marker15,				-1,	&G_Window15[6],
	SyncLineC, 				-44,	&G_Window15[7],
	Window15,				-144,	&G_Window15[8],
	SyncLineC,				-72,	&G_Window15[9],
	SyncLineB,				-2,	&G_Window15[10],
	SyncLineA,				-23,	&G_Window15[11],
	SyncLineD,				-24,	&G_Window15[12],
	Window50Marker+1,		-1,	&G_Window15[13],
	MarkerLine+1,			-1,	&G_Window15[14],
	SyncLineD,				-1,	&G_Window15[15],
	Marker15+1,				-1,	&G_Window15[16],
	SyncLineD,				-44,	&G_Window15[17],
	Window15+1,				-144,	&G_Window15[18],
	SyncLineD,				-72,	&G_Window15[19],
	SyncLineA,				-2,	&G_Window15[0],
};

code struct PATTERN_ITEM G_Window20[20] = {
	SyncLineB, 				-22,	&G_Window20[1],			// Line 625
	SyncLineC,				-24,	&G_Window20[2],
	Window50Marker,		-1,	&G_Window20[3],
	MarkerLine, 			-1,	&G_Window20[4],
	SyncLineC,				-1,	&G_Window20[5],
	Marker20,				-1,	&G_Window20[6],
	SyncLineC, 				-44,	&G_Window20[7],
	Window20,				-144,	&G_Window20[8],
	SyncLineC,				-72,	&G_Window20[9],
	SyncLineB,				-2,	&G_Window20[10],
	SyncLineA,				-23,	&G_Window20[11],
	SyncLineD,				-24,	&G_Window20[12],
	Window50Marker+1,		-1,	&G_Window20[13],
	MarkerLine+1,			-1,	&G_Window20[14],
	SyncLineD,				-1,	&G_Window20[15],
	Marker20+1,				-1,	&G_Window20[16],
	SyncLineD,				-44,	&G_Window20[17],
	Window20+1,				-144,	&G_Window20[18],
	SyncLineD,				-72,	&G_Window20[19],
	SyncLineA, 				-2,	&G_Window20[0],
};

code struct PATTERN_ITEM G_Window100[20] = {
	SyncLineB,				-22,	&G_Window100[1],			// Line 625
	SyncLineC,				-24,	&G_Window100[2],
	Window50Marker,		-1,	&G_Window100[3],
	MarkerLine, 			-1,	&G_Window100[4],
	SyncLineC,				-1,	&G_Window100[5],
	Marker100,				-1,	&G_Window100[6],
	SyncLineC,				-44,	&G_Window100[7],
	Window100,				-144,	&G_Window100[8],
	SyncLineC,				-72,	&G_Window100[9],
	SyncLineB,				-2,	&G_Window100[10],
	SyncLineA,				-23,	&G_Window100[11],
	SyncLineD,				-24,	&G_Window100[12],
	Window50Marker+1,		-1,	&G_Window100[13],
	MarkerLine+1,			-1,	&G_Window100[14],
	SyncLineD,				-1,	&G_Window100[15],
	Marker100+1,			-1,	&G_Window100[16],
	SyncLineD,				-44,	&G_Window100[17],
	Window100+1,			-144,	&G_Window100[18],
	SyncLineD,				-72,	&G_Window100[19],
	SyncLineA,				-2,	&G_Window100[0],
};

code struct PATTERN_ITEM G_CrossHatch[62] = {
	SyncLineB,				-22,	&G_CrossHatch[1],			// Line 525
	CrossHatch19_625,		-7,	&G_CrossHatch[2],
	WhiteLine_625,			-1,	&G_CrossHatch[3],
	CrossHatch19_625,		-20,	&G_CrossHatch[4],
	WhiteLine_625,			-1,	&G_CrossHatch[5],
	CrossHatch19_625,		-20,	&G_CrossHatch[6],
	WhiteLine_625,			-1,	&G_CrossHatch[7],
	CrossHatch19_625,		-20,	&G_CrossHatch[8],
	WhiteLine_625,			-1,	&G_CrossHatch[9],
	CrossHatch19_625,		-20,	&G_CrossHatch[10],
	WhiteLine_625,			-1,	&G_CrossHatch[11],
	CrossHatch19_625,		-20,	&G_CrossHatch[12],
	WhiteLine_625,			-1,	&G_CrossHatch[13],
	CrossHatch19_625,		-20,	&G_CrossHatch[14],
	WhiteLine_625,			-1,	&G_CrossHatch[15],
	CrossHatch19_625,		-20,	&G_CrossHatch[16],
	WhiteLine_625,			-1,	&G_CrossHatch[17],
	CrossHatch19_625,		-20,	&G_CrossHatch[18],
	WhiteLine_625,			-1,	&G_CrossHatch[19],
	CrossHatch19_625,		-20,	&G_CrossHatch[20],
	WhiteLine_625,			-1,	&G_CrossHatch[21],
	CrossHatch19_625,		-20,	&G_CrossHatch[22],
	WhiteLine_625,			-1,	&G_CrossHatch[23],
	CrossHatch19_625,		-20,	&G_CrossHatch[24],
	WhiteLine_625,			-1,	&G_CrossHatch[25],
	CrossHatch19_625,		-20,	&G_CrossHatch[26],
	WhiteLine_625,			-1,	&G_CrossHatch[27],
	CrossHatch19_625,		-20,	&G_CrossHatch[28],
	WhiteLine_625,			-1,	&G_CrossHatch[29],
	CrossHatch19_625,		-7,	&G_CrossHatch[30],
	SyncLineB,				-2,	&G_CrossHatch[31],
	SyncLineA,				-23,	&G_CrossHatch[32],
	CrossHatch19_625+1,	-7,	&G_CrossHatch[33],
	WhiteLine_625+1,		-1,	&G_CrossHatch[34],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[35],
	WhiteLine_625+1,		-1,	&G_CrossHatch[36],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[37],
	WhiteLine_625+1,		-1,	&G_CrossHatch[38],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[39],
	WhiteLine_625+1,		-1,	&G_CrossHatch[40],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[41],
	WhiteLine_625+1,		-1,	&G_CrossHatch[42],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[43],
	WhiteLine_625+1,		-1,	&G_CrossHatch[44],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[45],
	WhiteLine_625+1,		-1,	&G_CrossHatch[46],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[47],
	WhiteLine_625+1,		-1,	&G_CrossHatch[48],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[49],
	WhiteLine_625+1,		-1,	&G_CrossHatch[50],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[51],
	WhiteLine_625+1,		-1,	&G_CrossHatch[52],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[53],
	WhiteLine_625+1,		-1,	&G_CrossHatch[54],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[55],
	WhiteLine_625+1,		-1,	&G_CrossHatch[56],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[57],
	WhiteLine_625+1,		-1,	&G_CrossHatch[58],
	CrossHatch19_625+1,	-20,	&G_CrossHatch[59],
	WhiteLine_625+1,		-1,	&G_CrossHatch[60],
	CrossHatch19_625+1,	-7,	&G_CrossHatch[61],
	SyncLineA,				-2,	&G_CrossHatch[0],
};

code struct PATTERN_ITEM G_PLUGE[16] = {
	SyncLineB,				-22,	&G_PLUGE[1],				// Line 625
	SyncLineC,				-38,	&G_PLUGE[2],
	PlugeWhite_625, 		-53,	&G_PLUGE[3],
	PlugeGreyC_625,		-53,	&G_PLUGE[4],
	PlugeGreyD_625,		-53,	&G_PLUGE[5],
	PlugeGreyE_625,		-53,	&G_PLUGE[6],
	SyncLineC,				-38,	&G_PLUGE[7],
	SyncLineB,				-2,	&G_PLUGE[8],
	SyncLineA,				-23,	&G_PLUGE[9],
	SyncLineD,				-38,	&G_PLUGE[10],
	PlugeWhite_625+1,		-53,	&G_PLUGE[11],
	PlugeGreyC_625+1,		-53,	&G_PLUGE[12],
	PlugeGreyD_625+1,		-53,	&G_PLUGE[13],
	PlugeGreyE_625+1,		-53,	&G_PLUGE[14],
	SyncLineD,				-38,	&G_PLUGE[15],
	SyncLineA,				-2,	&G_PLUGE[0],
};

code struct PATTERN_ITEM G_MultiBurst[6] = {
	SyncLineB,				-22,	&G_MultiBurst[1],			// Line 625
	Chroma_MultiBurst,	-288,	&G_MultiBurst[2],
	SyncLineB,				-2,	&G_MultiBurst[3],
	SyncLineA,				-23,	&G_MultiBurst[4],
	Chroma_MultiBurst+1,	-288,	&G_MultiBurst[5],
	SyncLineA,				-2,	&G_MultiBurst[0],
};

code struct PATTERN_ITEM G_Red75[6] = {
	SyncLineB,  			-22,	&G_Red75[1],				// Line 625
	Red75_625,				-288,	&G_Red75[2],
	SyncLineB, 				-2,	&G_Red75[3],
	SyncLineA,				-23,	&G_Red75[4],
	Red75_625+1,			-288,	&G_Red75[5],
	SyncLineA,				-2,	&G_Red75[0],
};

code struct PATTERN_ITEM G_Stair5[6] = {
	SyncLineB,				-22,	&G_Stair5[1],
	GreyScale5_625,		-288,	&G_Stair5[2],
	SyncLineB,				-2,	&G_Stair5[3],
	SyncLineA,				-23,	&G_Stair5[4],
	GreyScale5_625+1,		-288,	&G_Stair5[5],
	SyncLineA,				-2,	&G_Stair5[0],
};

code struct PATTERN_ITEM G_Stair10[6] = {
	SyncLineB,				-22,	&G_Stair10[1],
	GreyScale10_625,		-288,	&G_Stair10[2],
	SyncLineB,				-2,	&G_Stair10[3],
	SyncLineA,				-23,	&G_Stair10[4],
	GreyScale10_625+1,	-288,	&G_Stair10[5],
	SyncLineA,				-2,	&G_Stair10[0],
};
code struct PATTERN_ITEM G_Customized1[6] = {
	SyncLineB,  			 -22, &G_Customized1[1],
	SyncLineC,	 			-288, &G_Customized1[2],
	SyncLineB,  			  -2, &G_Customized1[3],
	SyncLineA,  			 -23, &G_Customized1[4],
	SyncLineD,		  		-288, &G_Customized1[5],
	SyncLineA,  			  -2, &G_Customized1[0],
};

code struct PATTERN_ITEM G_Customized2[6] = {
	SyncLineB,  			 -22, &G_Customized2[1],
	SyncLineC,	 			-288, &G_Customized2[2],
	SyncLineB,  			  -2, &G_Customized2[3],
	SyncLineA,  			 -23, &G_Customized2[4],
	SyncLineD,		  		-288, &G_Customized2[5],
	SyncLineA,  			  -2, &G_Customized2[0],
};

code struct PATTERN_ITEM G_Customized3[6] = {
	SyncLineB,  			 -22, &G_Customized3[1],
	SyncLineC,	 			-288, &G_Customized3[2],
	SyncLineB,  			  -2, &G_Customized3[3],
	SyncLineA,  			 -23, &G_Customized3[4],
	SyncLineD,		  		-288, &G_Customized3[5],
	SyncLineA,  			  -2, &G_Customized3[0],
};

code struct PATTERN_ITEM G_Customized4[6] = {
	SyncLineB,  			 -22, &G_Customized4[1],
	SyncLineC,	 			-288, &G_Customized4[2],
	SyncLineB,  			  -2, &G_Customized4[3],
	SyncLineA,  			 -23, &G_Customized4[4],
	SyncLineD,		  		-288, &G_Customized4[5],
	SyncLineA,  			  -2, &G_Customized4[0],
};

code struct PATTERN_ITEM G_Customized5[6] = {
	SyncLineB,  			 -22, &G_Customized5[1],
	SyncLineC,	 			-288, &G_Customized5[2],
	SyncLineB,  			  -2, &G_Customized5[3],

	SyncLineA,  			 -23, &G_Customized5[4],
	SyncLineD,		  		-288, &G_Customized5[5],
	SyncLineA,  			  -2, &G_Customized5[0],
};

code struct PATTERN_ITEM G_Customized6[6] = {
	SyncLineB,  			 -22, &G_Customized6[1],
	SyncLineC,	 			-288, &G_Customized6[2],
	SyncLineB,  			  -2, &G_Customized6[3],

	SyncLineA,  			 -23, &G_Customized6[4],
	SyncLineD,		  		-288, &G_Customized6[5],
	SyncLineA,  			  -2, &G_Customized6[0],
};

code struct PATTERN_ITEM G_Customized7[6] = {
	SyncLineB,  			 -22, &G_Customized7[1],
	SyncLineC,	 			-288, &G_Customized7[2],
	SyncLineB,  			  -2, &G_Customized7[3],
	SyncLineA,  			 -23, &G_Customized7[4],
	SyncLineD,		  		-288, &G_Customized7[5],
	SyncLineA,  			  -2, &G_Customized7[0],
};

code struct PATTERN_ITEM G_Customized8[6] = {
	SyncLineB,  			 -22, &G_Customized8[1],
	SyncLineC,	 			-288, &G_Customized8[2],
	SyncLineB,  			  -2, &G_Customized8[3],
	SyncLineA,  			 -23, &G_Customized8[4],
	SyncLineD,		  		-288, &G_Customized8[5],
	SyncLineA,  			  -2, &G_Customized8[0],
};

