/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8602/03 SDI Digital test signal generator						*/
/*	Module:		D1TABLES.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960703																		*/
/*	Rev. date:	970512, KEn	DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains NO functions													*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/
#include <xa.h>

#include "d1gener.h"
#include "d1tables.h"

code UC SoundPage0[13824] @ 0x0CA00;
code UC SoundPage1[65535] @ 0x10000;
code UC SoundPage2[65535] @ 0x20000;
code UC SoundPage3[65535] @ 0x30000;
code UC SoundPage4[65535] @ 0x40000;
code UC SoundPage5[65535] @ 0x50000;
code UC SoundPage6[65535] @ 0x60000;
code UC SoundPage7[65535] @ 0x70000;

code UC* SoundPageTable[MaxNoOfLevels] = {
	&SoundPage0[0],			// Silence
	&SoundPage1[0],			//   0 dB
	&SoundPage2[0],			//  -9 dB
	&SoundPage3[0],			// -12 dB
	&SoundPage4[0],			// -14 dB
	&SoundPage5[0],			// -16 dB
	&SoundPage6[0],			// -18 dB
	&SoundPage7[0]				// -20 dB
};

code UL SoundOffsetTable[MaxNoOfSignals] = {
	0,								// AudioSignalOff
	0,								// AudioStereo800Hz
	0x5A00,						// AudioStereo1kHz
	0x5A00,						// AudioStereoEBU1kHz
	0x5A00,						// AudioStereoBBC1kHz
	0x6C00,						// AudioMonoEBU1kHz
	0x6C00,						// AudioMonoBBC1kHz
	0x7E00						// AudioDual
};

code UC Sound525[48] = {
	0x43, 0xC1, 0x45, 0x03, 0x46, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1,	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1,
	0x4B, 0xC1, 0x4B, 0xC1, 0x4B, 0xC1, 0x43, 0x41
};

code UC Sound625[65] = {
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

code UC TextTable[128] = {
// spc     !                                       &
	1, 94,  1, 93,  1,  0,  1,  0,  1,  0,  1,  0,  2, 65,  1,  0,

//                                 ,       -       .
	1,  0,  1,  0,  1,  0,  1,  0,  1, 92,  2, 95,  1, 91,  1,  0,

// 0       1       2       3       4       5       6       7
	2, 69,  2, 71,  2, 73,  2, 75,  2, 77,  2, 79,  2, 81,  2, 83,

// 8       9       :
	2, 85,  2, 87,  2, 89,  1,  0,  1,  0,  1,  0,  1,  0,  1,  0,

//         A       B       C       D       E       F       G
 	1,  0,  2,  1,  2,  3,  2,  5,  2,  7,  2,  9,  2, 11,  2, 13,

// H       I       J       K       L       M       N       O
 	2, 15,  1, 17,  2, 18,  2, 20,  2, 22,  3, 24,  2, 27,  2, 29,

// P       Q       R       S       T       U       V       W
 	2, 31,  2, 33,  2, 35,  2, 37,  2, 39,  2, 41,  2, 43,  3, 45,

// X       Y       Z                                       _
 	2, 48,  2, 50,  2, 52,  1,  0,  1,  0,  1,  0,  1,  0,  2, 97
};


