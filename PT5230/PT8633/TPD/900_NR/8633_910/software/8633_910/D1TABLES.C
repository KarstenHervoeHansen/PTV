/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 1/2							*/
/*	Module:		D1TABLES.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980210																		*/
/*	Rev. date:	981002, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains NO functions													*/
/*																									*/
/*	Changes:																						*/
/* 981002: Released in SW ver. 3.0														*/
/* 980603: Released in SW ver. 2.0														*/
/* 021113: Modified for TDC moving logo                                    */
/***************************************************************************/

#include <xa.h>

#include "d1gener.h"
#include "d1tables.h"

code UC LogoDataPage[45056] @ 0x40000;  //Logo data overwrites -15 dB data;

code UC AudioPage0[13824] @ 0x0CA00;
code UC AudioPage1[65535] @ 0x10000;
code UC AudioPage2[65535] @ 0x20000;
code UC AudioPage3[65535] @ 0x30000;
//code UC AudioPage4[65535] @ 0x40000;
code UC AudioPage5[65535] @ 0x50000;
code UC AudioPage6[65535] @ 0x60000;
code UC AudioPage7[65535] @ 0x70000;

code UC* LogoDataPageTable = &LogoDataPage[0];

code UC* AudioPageTable[MaxNoOfLevels] =
  {
	&AudioPage0[0],			// Silence
	&AudioPage1[0],			//   0 dB
	&AudioPage2[0],			//  -9 dB
	&AudioPage3[0],			// -12 dB
//	&AudioPage4[0],			// -15 dB		// -14dB in PT8602/03 // no -15dB JK
	&AudioPage5[0],			// -16 dB
	&AudioPage6[0],			// -18 dB
	&AudioPage7[0]			// -20 dB
  };

//    SignalAddr = AudioPageTable[AudioLevel] + AudioOffsetTable[AudioSignal];
code UL AudioOffsetTable[MaxNoOfSignals] =
  {
	0,								// AudioSignalOff
	0,								// AudioStereo800Hz
	0x5A00,						// AudioStereo1kHz
	0x5A00,						// AudioStereoEBU1kHz
//	0x5A00,						// AudioStereoBBC1kHz // no BBC JK
	0x6C00,						// AudioMonoEBU1kHz
	0x6C00,						// AudioMonoBBC1kHz
	0x7E00						// AudioDual
  };

code UC Audio525[48] = {
	0x43, 0xC1, 0x45, 0x03, 0x46, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1,	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1, 0x43, 0x41
};

code UC Audio625[65] = {
 	0xC1,  0x44,  0x03,  0x45,  0xC1,  0x49,  0xC1,  0x49,
	0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,
	0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,
	0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,
	0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,
	0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,
	0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,
	0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x49,  0xC1,  0x48,
	0xC1
};

											// NOTE! # is used for clock colon
code UC TextTable[408] = {
// spc     !               #       $               &       '
	1, 94,  1, 93,  1, 0,   1, 237, 1, 238, 1, 0,   2, 65,  1, 158,

// (       )       *       +       ,       -       .       /
	1, 235, 1, 236, 1, 0,   2, 99,  1, 92,  2, 95,  1, 91,  2, 161,

// 0       1       2       3       4       5       6       7
	2, 69,  2, 71,  2, 73,  2, 75,  2, 77,  2, 79,  2, 81,  2, 83,

// 8       9       :       ;       <       =       >       ?
	2, 85,  2, 87,  2, 89,  1, 0,   1, 0,   2, 159, 1, 0,   1, 0,

//         A       B       C       D       E       F       G
 	1, 0,   2, 1,   2, 3,   2, 5,   2, 7,   2, 9,   2, 11,  2, 13,

// H       I       J       K       L       M       N       O
 	2, 15,  1, 17,  2, 18,  2, 20,  2, 22,  3, 24,  2, 27,  2, 29,

// P       Q       R       S       T       U       V       W
 	2, 31,  2, 33,  2, 35,  2, 37,  2, 39,  2, 41,  2, 43,  3, 45,

// X       Y       Z                                       _
 	2, 48,  2, 50,  2, 52,  1, 0,   1, 0,   1, 0,   1, 0,   2, 97,

// `       a       b       c       d       e       f       g  
 	1, 0,   2, 101, 2, 109, 2, 111, 2, 113, 2, 115, 1, 117, 2, 118,

// h       i       j       k       l       m       n       o  
 	2, 105, 1, 120, 1, 121, 2, 122, 1, 124, 3, 125, 2, 128, 2, 107,

// p       q       r       s       t       u       v       w  
 	2, 130, 2, 132, 2, 134, 2, 136, 2, 138, 2, 140, 2, 142, 3, 144,

// x       y       z       {       |       }       ~          
 	2, 103, 2, 147, 2, 149, 1, 0,   1, 0,   1, 0,   1, 0,   1, 0,

// Ä       Å       Ç       É       Ñ       Ö       Ü       á
 	2,171,  2, 221, 2, 203, 2, 197, 2, 193, 2, 199, 2, 156, 2, 201,

// à       â       ä       ã       å       ç       é       è  
 	2, 205, 2, 207, 2, 209, 1, 228, 1, 227, 1, 0,   2, 61,  2, 59,

// ê       ë       í       ì       î       ï       ñ       ó  
 	2, 173, 3, 151, 3,  54, 2, 217, 2, 213, 1, 0,   2, 231, 1, 0,

// ò       ô       ö       õ       ú       ù       û       ü  
 	1, 0,   2, 63,  2, 191, 2, 154, 1, 0,   2, 57,  1, 0,   1, 0,

// †       °       ¢       £       §       •       ¶       ß  
 	2, 195, 1, 226, 2, 215, 2, 189, 2, 211, 2, 181, 1, 0,   1, 0,

// ®       ©       ™       ´       ¨       ≠       Æ       Ø  
 	1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,

// ∞       ±       ≤       ≥       ¥       µ       ∂       ∑  
 	1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   2, 163, 2, 165, 2, 167,

// ∏       π       ∫       ª       º       Ω       æ       ø  
 	1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,

// ¿       ¡       ¬       √       ƒ       ≈       ∆       «  
 	1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   2, 169,

// »       …               À       Ã       Õ       Œ       œ  
 	1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,

// –       —       “       ”       ‘       ’       ÷       ◊  
 	1, 0,   1, 0,   2, 175, 2, 177, 2, 179, 1, 0,   1, 223, 1, 224,

// ÿ       Ÿ       ⁄       €       ‹       ›       o       ﬂ  
 	1, 225, 1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,   1, 0,

// ‡       ·       ‚       „       ‰       Â       Ê       Á  
 	2, 185, 2,  67, 2, 187, 1, 0,   2, 219, 2, 183, 1, 0,   1, 0,

// Ë       È       Í       Î      
 	1, 0,   2, 233, 2, 229, 1, 0
};
