/****************************************************************************/
/* MODULE:                                                                  */
/*  cbar.c - ColorBar generator                                             */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#include "define.h"        // Standard PTV defines
#include "cbar.h"
#include "tables.h"
#include "led_hw.h"
#include "tvp6kdrv.h"

#define Bank1			0x10
#define Bank2			0x14
#define Bank3			0x18
#define Bank4			0x1C
#define Bank5			0x20
#define Bank6			0x24
#define Bank7			0x28
#define Bank8			0x2C
#define Bank9			0x30
#define Bank10		0x34
#define Bank11		0x38
#define Bank12		0x3C

#define dEBUCBar_8bit_625	 				0
#define dEBUCBar_625							1
#define dSMPTECBar								2
#define dBlueBars									3
#define dIWhiteQ									4
#define dCBar100_625							5
#define dGrey75_625								6
#define dRed75_625								7
#define dRed75_525								8
#define dChroma_Multiburst				9
#define d_CCIR18_625							9
#define dFCC_Multiburst_525				10
#define dChroma_Multiburst_525		11
#define dWhiteLine_625						11
#define dCrossHatch19_625					12
#define dWhiteLine_525						13
#define dCrossHatch17_525					14
#define dPlugeWhite_625						15
#define dPlugeGreyC_625						16
#define dPlugeGreyD_625						17
#define dPlugeGreyE_625						18
#define dPlugeWhite_525						19
#define dPlugeGreyC_525						20
#define dPlugeGreyD_525						21
#define dPlugeGreyE_525						22
#define dGreyScale5_625						23
#define dGreyScale5_525						24
#define dGreyScale10_625					25
#define dGreyScale10_525					26
#define dWindow50Marker						27
#define dMarkerLine								28
#define dWindow15									29
#define dMarker15									30
#define dWindow20									31
#define dMarker20									32
#define dWindow100								33
#define dMarker100								34
#define dDigitalGrey_625					35
#define dDigitalGrey_525					36
#define dSDI_test_line1						37
#define dSDI_test_line2						38
#define dSDI_test_line3						39
#define dCBar75_STD_625	 					40
#define dFCC_CBar_525							41
#define dFCC_CBar100_525					42
#define dWindow10									43
#define dMarker10									44
#define dkHz15BlWh_625						45
#define dkHz15BlWh_525						46

/*
;************************************************************************
		CSEG at 4000H
Sound525:	DB	043H, 0C1H, 045H, 003H, 046H, 0C1H, 04BH, 0C1H
		DB	04BH, 0C1H, 04BH, 0C1H,	04BH, 0C1H, 04BH, 0C1H
		DB	04BH, 0C1H, 04BH, 0C1H, 04BH, 0C1H, 04BH, 0C1H
		DB	04BH, 0C1H, 04BH, 0C1H, 04BH, 0C1H, 04BH, 0C1H
		DB	04BH, 0C1H, 04BH, 0C1H, 04BH, 0C1H, 04BH, 0C1H
		DB	04BH, 0C1H, 04BH, 0C1H, 04BH, 0C1H, 043H, 041H

		;Sound625 starts at Sound525+48
Sound625:	DB	0C1H, 044H, 003H, 045H, 0C1H, 049H, 0C1H, 049H
		DB	0C1H, 049H, 0C1H, 049H, 0C1H, 049H, 0C1H, 049H
		DB	0C1H, 049H, 0C1H, 049H, 0C1H, 049H, 0C1H, 049H
		DB	0C1H, 049H, 0C1H, 049H, 0C1H, 049H, 0C1H, 049H
		DB	0C1H, 049H, 0C1H, 049H,	0C1H, 049H, 0C1H, 049H
		DB	0C1H, 049H, 0C1H, 049H, 0C1H, 049H, 0C1H, 049H
		DB	0C1H, 049H, 0C1H, 049H, 0C1H, 049H, 0C1H, 049H
		DB	0C1H, 049H, 0C1H, 049H,	0C1H, 049H, 0C1H, 048H
		DB	0C1H

*/
/*
The statusport is defined in the PLD as:

	bit 0:	S0				Background system
	bit 1:	S1

	bit 2:	S2				Linetype
	bit 3:	S3
	bit 4:	S4
	bit 5:	S5

	bit 6:	SYSSEL		System select ( 0:G, 1:M)
	bit 7:	SOUND			Sound ( 0:off, 1:on)

	bit 8:	3OR4			Samples in sound ( 0:3 samples, 1:4 samples)
	bit 9:	FIELD1		Field one bit ( 1 in Field 1 otherwise 0)
	bit 10: CB	
	bit 11: EDHON			EDH bit for future use ( 0:off, 1:on)

	bit 12: PS				PS ( 0:disable writing in SRAM, 1: enable writing in SRAM)
	bit 13: -					Not used
	bit 14: -					Not used
	bit 15: -					Not used
*/
										
#define S1S0				0x0003
#define S5S4S3S2		0x003C								
#define SYSSEL			0x0040
#define SOUND				0x0080
#define S3OR4				0x0100
#define FIELD1			0x0200
#define CB					0x0400
#define EDHON				0x0800
#define PS					0x1000

#define LineTypeA		1
#define LineTypeB 	0
#define LineTypeC 	2
#define LineTypeD 	3

code UI PALBlackTable[] = 
{
	22*256 	+					LineTypeB,
	144*256	+					LineTypeC,
	144*256	+					LineTypeC,
	2*256		+					LineTypeB,

	23*256 	+					LineTypeA,
	144*256	+					LineTypeD,
	144*256	+					LineTypeD,
	2*256		+					LineTypeA,
	0
};

code UI PALFullFieldTable[] = 
{
	22*256 	+					LineTypeB,
	144*256	+	Bank1 + LineTypeC,
	144*256	+	Bank1 + LineTypeC,
	2*256		+					LineTypeB,

	23*256 	+					LineTypeA,
	144*256	+	Bank1 + LineTypeD,
	144*256	+	Bank1 + LineTypeD,
	2*256		+					LineTypeA,
	0
};

code UI PALSplitFieldTable[] = 
{
	22*256 	+					LineTypeB,
	192*256	+	Bank1 + LineTypeC,
	96*256	+	Bank2 + LineTypeC,
	2*256		+					LineTypeB,

	23*256 	+					LineTypeA,
	192*256	+	Bank1 + LineTypeD,
	96*256	+	Bank2 + LineTypeD,
	2*256		+					LineTypeA,
	0
};

code UI PALWindowTable[] = 
{
	22*256 	+					LineTypeB,
	24*256	+         LineTypeC,
	1*256		+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	1*256		+	        LineTypeC,
	1*256		+	Bank3 + LineTypeC,
	44*256	+	        LineTypeC,
	144*256	+	Bank4 + LineTypeC,
	72*256	+	        LineTypeC,
	2*256		+					LineTypeB,

	23*256 	+					LineTypeA,
	24*256	+         LineTypeD,
	1*256		+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	1*256		+	        LineTypeD,
	1*256		+	Bank3 + LineTypeD,
	44*256	+	        LineTypeD,
	144*256	+	Bank4 + LineTypeD,
	72*256	+	        LineTypeD,
	2*256		+					LineTypeA,
	0
};

code UI PALCheckfieldTable[] = 
{
	22*256 	+					LineTypeB,
	1*256		+	Bank1 + LineTypeC,
	143*256	+	Bank2 + LineTypeC,
	144*256	+	Bank3 + LineTypeC,
	2*256		+					LineTypeB,

	23*256 	+					LineTypeA,
	144*256	+	Bank2 + LineTypeD,
	144*256	+	Bank3 + LineTypeD,
	2*256		+					LineTypeA,
	0
};

code UI PALCrosshatchTable[] = 
{
	22*256 	+					LineTypeB,
	7*256		+ Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	20*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	7*256	  +	Bank1 + LineTypeC,
	2*256		+					LineTypeB,

	23*256 	+					LineTypeA,
	7*256		+ Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	20*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	7*256	  +	Bank1 + LineTypeD,
	2*256		+					LineTypeA,
	0
};

code UI PALPLUGETable[] = 
{
	22*256 	+					LineTypeB,
	38*256	+         LineTypeC,
	53*256	+	Bank1 + LineTypeC,
	53*256	+	Bank2 + LineTypeC,
	53*256	+	Bank3 + LineTypeC,
	53*256	+	Bank4 + LineTypeC,
	38*256	+	        LineTypeC,
	2*256		+					LineTypeB,

	23*256 	+					LineTypeA,
	38*256	+         LineTypeD,
	53*256 	+	Bank1 + LineTypeD,
	53*256 	+	Bank2 + LineTypeD,
	53*256 	+	Bank3 + LineTypeD,
	53*256 	+	Bank4 + LineTypeD,
	38*256	+	        LineTypeD,
	2*256		+					LineTypeA,
	0
};

code UI NTSCBlackTable[] = 
{
	3*256 	+					LineTypeA,
	16*256	+					LineTypeB,
	122*256	+					LineTypeC,
	122*256	+					LineTypeC,

	2*256		+					LineTypeB,
	17*256 	+					LineTypeA,
	122*256	+					LineTypeD,
	121*256	+					LineTypeD,
	0
};

code UI NTSCFullFieldTable[] = 
{
	3*256 	+					LineTypeA,
	16*256	+					LineTypeB,
	122*256	+	Bank1	+	LineTypeC,
	122*256	+	Bank1 + LineTypeC,

	2*256		+					LineTypeB,
	17*256 	+					LineTypeA,
	122*256	+	Bank1 + LineTypeD,
	121*256	+	Bank1 + LineTypeD,
	0
};

code UI SMPTECBarTable[] = 
{
	3*256 	+					LineTypeA,
	16*256	+					LineTypeB,
	164*256	+	Bank1 + LineTypeC,
	19*256	+	Bank2 + LineTypeC,
	61*256	+	Bank3 + LineTypeC,

	2*256		+					LineTypeB,
	17*256 	+					LineTypeA,
	163*256	+	Bank1 + LineTypeD,
	19*256	+	Bank2 + LineTypeD,
	61*256	+	Bank3 + LineTypeD,
	0
};

code UI NTSCWindowTable[] = 
{
	3*256 	+					LineTypeA,
	16*256	+					LineTypeB,
	30*256	+	        LineTypeC,
	1*256		+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	1*256		+	        LineTypeC,
	1*256		+	Bank3 + LineTypeC,
	29*256	+	      + LineTypeC,
	119*256	+	Bank4 + LineTypeC,
	62*256	+	        LineTypeC,

	2*256		+					LineTypeB,
	17*256 	+					LineTypeA,
	29*256	+	        LineTypeD,
	1*256		+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	1*256		+	        LineTypeD,
	1*256		+	Bank3 + LineTypeD,
	29*256	+	        LineTypeD,
	119*256	+	Bank4 + LineTypeD,
	62*256	+	        LineTypeD,
	0
};

code UI NTSCCheckfieldTable[] = 
{
	3*256 	+					LineTypeA,
	16*256	+					LineTypeB,
	1*256		+	Bank1 + LineTypeC,
	121*256	+	Bank2 + LineTypeC,
	122*256	+	Bank3 + LineTypeC,

	2*256		+					LineTypeB,
	17*256 	+					LineTypeA,
	122*256	+	Bank2 + LineTypeD,
	121*256	+	Bank3 + LineTypeD,
	0
};

code UI NTSCCrosshatchTable[] = 
{
	3*256 	+					LineTypeA,
	16*256	+					LineTypeB,
	9*256		+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	16*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	17*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	16*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	17*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	16*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	17*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	16*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	17*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	16*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	17*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	16*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	17*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	16*256	+	Bank1 + LineTypeC,
	1*256		+	Bank2 + LineTypeC,
	7*256		+	Bank1 + LineTypeC,

	2*256		+					LineTypeB,
	17*256 	+					LineTypeA,
	8*256		+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	17*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	16*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	17*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	16*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	17*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	16*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	17*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	16*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	17*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	16*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	17*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	16*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	17*256	+	Bank1 + LineTypeD,
	1*256		+	Bank2 + LineTypeD,
	6*256		+	Bank1 + LineTypeD,
	0
};

code UI NTSCPLUGETable[] = 
{
	3*256 	+					LineTypeA,
	16*256	+					LineTypeB,
	53*256	+	        LineTypeC,
	56*256	+	Bank1 + LineTypeC,
	19*256	+	Bank2 + LineTypeC,
	19*256	+	Bank3 + LineTypeC,
	19*256	+	Bank4 + LineTypeC,
	78*256	+	        LineTypeC,

	2*256		+					LineTypeB,
	17*256 	+					LineTypeA,
	52*256	+	        LineTypeD,
	56*256	+	Bank1 + LineTypeD,
	19*256	+	Bank2 + LineTypeD,
	19*256	+	Bank3 + LineTypeD,
	19*256	+	Bank4 + LineTypeD,
	78*256	+	        LineTypeD,
	0
};
																						// Signal array defined in ROM
code UI VideoSignalArray[98304] _at( 0x50000);

volatile UI SRAMVideoPort _at( 0x44000); 		// Port defined in PLD
volatile UI StatusPort _at( 0x6a000); 			// Port defined in PLD
volatile UI HOffsetPort _at( 0x6a004); 			// Port defined in PLD

volatile _near UI TimeTickCounter;
volatile _near int LineOffset;
volatile _near bool NewFieldPulse;

volatile int PhaseOffset;

CBObject CBSetup;

static _near UI Status;

static code UI * _near TablePtr;
static code UI *newTablePtr;

static _near UI LineTypeCnt; 
static _near UC LineTypePtr;
																 
static UL CBCalOffset;

static char Blanking, NewPattern;

/**************************************************************************/
/* SRAMVideoLoad																							     CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000324																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 S5,S4,S3 and S2 should be kept at ZERO while loading AND the	*/
/*						interrupt system should NOT clear the PS bit.								*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SRAMVideoLoad( int signal, UC bank)
{
	volatile UI *VideoDstPtr;
	code UI *VideoSrcPtr;
	int i;

	if ( bank > Bank12)
		return( FAIL);

	Status |= PS;										// Set PS bit while loading
	StatusPort = Status;
																						 
	VideoDstPtr = &SRAMVideoPort + (( bank/4)-4) * 0x0800;
	VideoSrcPtr = &VideoSignalArray[ 0x800 * (long)signal];

	for ( i = 0; i < 2048; i++)
		*VideoDstPtr++ = *VideoSrcPtr++;

	Status &= ~PS;									// Clear PS bit again
	StatusPort = Status;

	return( OK);
}

/**************************************************************************/
/* CBS1S0Lines  																							     CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000322																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function: 																															*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void CBS1S0Lines( void)
{
  UI tmp;

	if ( --LineTypeCnt == 0)
	{
	 	tmp = TablePtr[++LineTypePtr];

		if ( tmp == 0)
		{
			if ( NewPattern)
			{
				NewPattern = Blanking = FALSE;

				TablePtr = newTablePtr;
			}

			LineTypePtr = 0;
			tmp = TablePtr[0];
		}

		LineTypeCnt = tmp / 256;

		tmp %= 256;
		tmp &= 0x3F;

		Status &= ~( S5S4S3S2 | S1S0);
		Status |= tmp;

		if ( Blanking)
			Status &= ~S5S4S3S2;

		StatusPort = Status;
	}	
}

/**************************************************************************/
/* CBDelayedFieldCode																				  	   CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000322																		*/
/* Revised:	 000322, KEn, DEV     																				*/
/*																																				*/
/* Function: 																															*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void DelayedFieldCode( void)
{
}

/**************************************************************************/
/* CBInit																										     	CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000322																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void CBInit( void)
{
	LineTypePtr = 0;
	TablePtr = &NTSCFullFieldTable[0];

	NewPattern = 0;
	Blanking = 1;

	Status |= SYSSEL;
	StatusPort = Status;

	LineTypeCnt = 1;				// Interrupt in 1 line

	CBSetup.System = NTSC;
	CBSetup.ScHPhase = 0;
	CBSetup.Delay = 0;
	CBSetup.EmbAudioSignal = EmbAudioOff;

	LEDSet( CBPALLED, CBSetup.System < NTSC);
	LEDSet( CBNTSCLED, CBSetup.System >= NTSC);

	SetCBPattern( SMPTECBar);

	tvp6k_system( NTSC);

	CBCalOffset = 0;
}

/**************************************************************************/
/* SetCBPattern																								     CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000405, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBPattern( UI pattern)
{
	int system = CBSetup.System;

	if ( ValidCBPattern( system , &pattern) == FAIL)
		return( FAIL);

	Blanking = TRUE;

	Status &= ~S5S4S3S2;								 				// Blank line immediately
	StatusPort = Status;

	switch ( pattern)
	{
		case SMPTECBar:										 				// Split field
			SRAMVideoLoad( dSMPTECBar, Bank1);
			SRAMVideoLoad( dBlueBars, Bank2);
			SRAMVideoLoad( dIWhiteQ, Bank3);

			newTablePtr = &SMPTECBarTable[0];
			break;
																			 				// Full field
		case EBUCBar:
			SRAMVideoLoad( dEBUCBar_625, Bank1);

			newTablePtr = &PALFullFieldTable[0];
			break;

		case FCCCBar:											 				// Full field
			SRAMVideoLoad( dFCC_CBar_525, Bank1);

			newTablePtr = &NTSCFullFieldTable[0];
			break;

		case EBUCBarITU:									 				// Full field
			SRAMVideoLoad( dEBUCBar_8bit_625, Bank1);

			if ( system < NTSC)
				newTablePtr = &PALFullFieldTable[0];
			else
				newTablePtr = &NTSCFullFieldTable[0];
			break;

		case CBar100:											 				// Full field
			if ( system < NTSC)
			{
				SRAMVideoLoad( dCBar100_625, Bank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dFCC_CBar100_525, Bank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case CBarGrey75:									 				// Split field
			SRAMVideoLoad( dCBar75_STD_625, Bank1);
			SRAMVideoLoad( dGrey75_625, Bank2);
	
			newTablePtr = &PALSplitFieldTable[0];
			break;

		case CBarRed75:														// Split field
			SRAMVideoLoad( dEBUCBar_625, Bank1);
			SRAMVideoLoad( dRed75_625, Bank2);

			newTablePtr = &PALSplitFieldTable[0];
			break;
			
		case Red75:																// Full field 
			if ( system < NTSC)
			{
				SRAMVideoLoad( dRed75_625, Bank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dRed75_525, Bank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Multiburst:													// Full field	
			if ( system < NTSC)
			{
				SRAMVideoLoad( dChroma_Multiburst, Bank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dFCC_Multiburst_525, Bank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Window10:														// Window
			SRAMVideoLoad( dWindow50Marker, Bank1);
			SRAMVideoLoad( dMarkerLine, Bank2);
			SRAMVideoLoad( dMarker10, Bank3);
			SRAMVideoLoad( dWindow10, Bank4);

			if ( system < NTSC)												
				newTablePtr = &PALWindowTable[0];
			else
				newTablePtr = &NTSCWindowTable[0];
			break;

		case Window15:														// Window
			SRAMVideoLoad( dWindow50Marker, Bank1);
			SRAMVideoLoad( dMarkerLine, Bank2);
			SRAMVideoLoad( dMarker15, Bank3);
			SRAMVideoLoad( dWindow15, Bank4);

			if ( system < NTSC)												
				newTablePtr = &PALWindowTable[0];
			else
				newTablePtr = &NTSCWindowTable[0];
			break;

		case Window20:														// Window
			SRAMVideoLoad( dWindow50Marker, Bank1);
			SRAMVideoLoad( dMarkerLine, Bank2);
			SRAMVideoLoad( dMarker20, Bank3);
			SRAMVideoLoad( dWindow20, Bank4);

			if ( system < NTSC)												
				newTablePtr = &PALWindowTable[0];
			else
				newTablePtr = &NTSCWindowTable[0];
			break;

		case Window100:														// Window
			SRAMVideoLoad( dWindow50Marker, Bank1);
			SRAMVideoLoad( dMarkerLine, Bank2);
			SRAMVideoLoad( dMarker100, Bank3);
			SRAMVideoLoad( dWindow100, Bank4);

			if ( system < NTSC)												
				newTablePtr = &PALWindowTable[0];
			else
				newTablePtr = &NTSCWindowTable[0];
			break;

		case BlWh15kHz:														// Full field
			if ( system < NTSC)												
			{
				SRAMVideoLoad( dkHz15BlWh_625, Bank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dkHz15BlWh_525, Bank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case White100:														// Full field
			if ( system < NTSC)
			{
				SRAMVideoLoad( dWhiteLine_625, Bank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dWhiteLine_525, Bank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case BlackBurst:													// Full field
			if ( system < NTSC)
				newTablePtr = &PALBlackTable[0];
			else
				newTablePtr = &NTSCBlackTable[0];
			break;

		case CheckField:													// Split field
			SRAMVideoLoad( dSDI_test_line1, Bank1);
			SRAMVideoLoad( dSDI_test_line2, Bank2);
			SRAMVideoLoad( dSDI_test_line3, Bank3);

			if ( system < NTSC)
				newTablePtr = &PALCheckfieldTable[0];
			else
				newTablePtr = &NTSCCheckfieldTable[0];
			break;

		case DigitalGrey:													// Full field
			if ( system < NTSC)
			{
				SRAMVideoLoad( dDigitalGrey_625, Bank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dDigitalGrey_525, Bank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Stair5:															// Full field
			if ( system < NTSC)
			{
				SRAMVideoLoad( dGreyScale5_625, Bank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dGreyScale5_525, Bank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Stair10:															// Full field
			if ( system < NTSC)	
			{
				SRAMVideoLoad( dGreyScale10_625, Bank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dGreyScale10_525, Bank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Crosshatch:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dCrossHatch19_625, Bank1);
				SRAMVideoLoad( dWhiteLine_625, Bank2);

				newTablePtr = &PALCrosshatchTable[0];
			}
			else
			{
				SRAMVideoLoad( dCrossHatch17_525, Bank1);
				SRAMVideoLoad( dWhiteLine_525, Bank2);

				newTablePtr = &NTSCCrosshatchTable[0];
			}
			break;

		case PLUGE:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dPlugeWhite_625, Bank1);
				SRAMVideoLoad( dPlugeGreyC_625, Bank2);
				SRAMVideoLoad( dPlugeGreyD_625, Bank3);
				SRAMVideoLoad( dPlugeGreyE_625, Bank4);

				newTablePtr = &PALPLUGETable[0];
			}
			else
			{
				SRAMVideoLoad( dPlugeWhite_525, Bank1);
				SRAMVideoLoad( dPlugeGreyC_525, Bank2);
				SRAMVideoLoad( dPlugeGreyD_525, Bank3);
				SRAMVideoLoad( dPlugeGreyE_525, Bank4);

				newTablePtr = &NTSCPLUGETable[0];
			}
			break;
	}

	NewPattern = TRUE;

	CBSetup.Pattern = pattern;

	return( OK);
}

/**************************************************************************/
/* SetCBSystem																									   CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBSystem( UC system)
{
	CBSetup.System = system;

	if ( system < NTSC)
		Status &= ~SYSSEL;
	else
		Status |= SYSSEL;

	StatusPort = Status;

	tvp6k_system( system);

	ValidCBPattern( system , &CBSetup.Pattern);

	SetCBPattern( CBSetup.Pattern);						// This one initialises TablePtr;

	LEDSet( CBPALLED, system < NTSC);
	LEDSet( CBNTSCLED, system >= NTSC);

	return( OK);
}

/**************************************************************************/
/* SetCBDelay																										   CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000331, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBDelay( UL delay)
{
	UL tmpOffset;

	CBSetup.Delay = delay;

	tmpOffset = delay + CBCalOffset;

	if ( CBSetup.System < NTSC)
	{
		tmpOffset %= 276480000L;				// 625*1728*256

		LineOffset = 0xFFFF - ( tmpOffset / 442368);				
		PhaseOffset = 1727 - (( tmpOffset % 442368) / 256);
	}
	else 
	{
		tmpOffset %= 230630400L;				// 525*1716*256

		LineOffset = 0xFFFF - ( tmpOffset / 439296);				
		PhaseOffset = 1715 - (( tmpOffset % 439296) / 256);
	}

	HOffsetPort = PhaseOffset;

	return( OK);
}

/**************************************************************************/
/* SetCBScHPhase																								   CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000407, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 Resolution is different from BlackBurst											*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBScHPhase( int schphase)
{
	long tmp = schphase;

	if (( schphase < ScHPhaseLow) || ( schphase > ScHPhaseHigh))
		return( FAIL);

	CBSetup.ScHPhase = schphase;

	if ( schphase < 0)								// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 256;
	tmp /= 360;												// tmp = ( ScHPhase*256)/360;

	return( OK);
}

/**************************************************************************/
/* SetCBEmbAudioSignal																					   CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000405, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBEmbAudioSignal( UC signal)
{
	CBSetup.EmbAudioSignal = signal;

	return( OK);
}

/**************************************************************************/
/* ValidCBPattern																							     CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000405																		*/
/* Revised:	 000405, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int ValidCBPattern( UC system, UI *pattern)
{
	if ( system < NTSC)
	{
		switch ( *pattern)
		{
			case SMPTECBar:
				break;

			case FCCCBar:
				break;

			default:
				return( OK);
		}

		*pattern = EBUCBar;
	}
	else
	{
		switch ( *pattern)
		{
			case EBUCBar:
				break;

			case CBarGrey75:
				break;

			case CBarRed75:
				break;

			default:
				return( OK);
		}

		*pattern = SMPTECBar;
	}

	return( FAIL);
}