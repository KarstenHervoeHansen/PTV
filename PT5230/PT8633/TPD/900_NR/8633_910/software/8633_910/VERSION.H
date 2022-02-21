/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT8632/33 SDI Test Pattern Generator 2								*/
/*	Module:		VERSION.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980210																		*/
/*	Rev. date:	990623, KEn DEV															*/
/*	Status:		Version 2.3																	*/
/*																									*/
/*	Changes:																						*/
/* 990623: PT8632/3210/33: Released in SW ver. 3.3									*/
/* 990335: PT8633: Released in SW ver. 3.2											*/
/* 981002: Released in SW ver. 3.0														*/
/* 980603: Released in SW ver. 2.0														*/
/***************************************************************************/

#define PT8633Version	1			// Compiler definition

#if PT8633Version
	//************************************************************************
	// DEFINITIONS FOR THE PT8633 GENERATOR
	//************************************************************************
	#define DefAddr			0xF3	// Default address in PT8633
	#define DefAddrEnable	0xFF	// Default address enable in PT8633

	#define SWVersion			33		// Software version

	#define HWType		  		33		// Hardware type: 33=PT8633

	#define HWVersion			0		// /900 version: 0=Normal, 1,2,.. = 901,902,..

	#define HWUserPattern	0		//

	#define HWInformation	0

#else
	//************************************************************************
	// DEFINITIONS FOR THE PT8632 GENERATOR
	//************************************************************************
	#define DefAddr			0x18	// Default address in PT8632
	#define DefAddrEnable	0x74	// Default address enable in PT8632

	#define SWVersion			33		// Software version

	#define HWType	  			32		// Hardware type: 32=PT8632

	#define HWVersion			0		// 900 version: 0=Normal, 1,2,.. = 901,902,..

	#define HWUserPattern	0		//

	#define HWInformation	0		// 0: Philips pattern (PT8632)
											// 1: FuBK Pattern (PT8632/10)

#endif
