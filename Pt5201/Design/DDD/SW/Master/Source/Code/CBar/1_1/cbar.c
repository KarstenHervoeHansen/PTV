/****************************************************************************/
/* MODULE:                                                                  */
/*  cbar.c - ColorBar generator                                             */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*	int WriteCBGain( UC gain)                                   						*/
/*	int WriteCBChromaGain( UC gain)                                    			*/
/*	int SRAMVideoLoad( UC signal, UC bank)                          		    */
/*	int SRAMSoundLoad( UC signal)                           						    */
/*	void CBLineInterrupt( void)                           							    */
/*	void CBDefaults( void)                                							    */
/*	int CBInit( void)                                 									   	*/
/*	int CBUpdate( void)                       								             	*/
/*	int SetCBPattern( UI pattern)																		      	*/
/*	int SetCBSystem( UC system)                                    					*/
/*	int SetCBDelay( UL delay)                                    						*/
/*	int SetCBScHPhase( int schphase)                                    		*/
/*	int SetCBEmbAudioSignal( UC signal)                                     */
/*	int ValidateCBPattern( UC system, UI *pattern)													*/	
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
#include "rtxcobj.h"
#include "config.h" 
#include "audio.h"
#include "cbar.h"
#include "tables.h"
#include "led_hw.h"
#include "tvp6kdrv.h"
#include "i2c_drv.h"
#include "instru.h"

#define SRAMBank0			0x00
#define SRAMBank1			0x10
#define SRAMBank2			0x14
#define SRAMBank3			0x18
#define SRAMBank4			0x1C
#define SRAMBank5			0x20
#define SRAMBank6			0x24
#define SRAMBank7			0x28
#define SRAMBank8			0x2C
#define SRAMBank9			0x30
#define SRAMBank10		0x34
#define SRAMBank11		0x38
#define SRAMBank12		0x3C

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
#define dMarkerLine							28
#define dWindow15								29
#define dMarker15								30
#define dWindow20								31
#define dMarker20								32
#define dWindow100							33
#define dMarker100							34
#define dDigitalGrey_625					35
#define dDigitalGrey_525					36
#define dSDI_test_line1						37
#define dSDI_test_line2						38
#define dSDI_test_line3						39
#define dCBar75_STD_625	 					40
#define dFCC_CBar_525						41
#define dFCC_CBar100_525					42
#define dWindow10								43
#define dMarker10								44
#define dkHz15BlWh_625						45
#define dkHz15BlWh_525						46
#define dCCIR18_625							47

/****************************************************************************/
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
	bit 9:	FIELD1		Field one bit ( 1 in Field 1/line 1 (out of 8/4) otherwise 0)
	bit 10: CB	
	bit 11: EDHON			EDH bit for future use ( 0:off, 1:on)

	bit 12: PS				PS ( 0:disable writing in SRAM, 1: enable writing in SRAM)
	bit 13: Line7			Line 7 pulse in PAL w/PAL_ID
	bit 14: PhaseReg	Phase offset register select
	bit 15: -					Not used
*/
/****************************************************************************/
										
#define S1S0		  	0x0003
#define S5S4S3S2		0x003C
#define SYSSEL			0x0040
#define SOUNDON	  	0x0080
#define S3OR4		  	0x0100
#define FIELD1			0x0200
#define CB			  	0x0400
#define EDHON		  	0x0800
#define PS			  	0x1000
#define LINE7PULSE	0x2000
#define PHASEREGSEL	0x4000

#define SOUND		  	(int)(SOUNDON + S3OR4)

#define SyncLineA		1
#define SyncLineB 	0
#define SyncLineC 	2
#define SyncLineD 	3

/****************************************************************************/
// Prototypes internal functions

int ValidateCBPattern( UC system, UI *pattern);

code UI Sound525[48] = {
	0x083, 0x181, 0x085, 0x003, 0x086, 0x181, 0x08B, 0x181,
	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
	0x08B, 0x181,	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x083, 0x081
};

code UI Sound625[65] = {
 	0x181,  0x084,  0x003,  0x085,  0x181,  0x089,  0x181,  0x089,
	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x089,
	0x181,  0x089,  0x181,  0x089,  0x181,  0x089,  0x181,  0x088,
	0x181
};

code UI PALBlackTable[] = 
{
	22*256 	+							SyncLineB,
	144*256	+							SyncLineC,
	144*256	+							SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	144*256	+							SyncLineD,
	144*256	+							SyncLineD,
	2*256		+							SyncLineA,
	0
};

code UI PALFullFieldTable[] = 
{
	22*256 	+							SyncLineB,
	144*256	+	SRAMBank1 + SyncLineC,
	144*256	+	SRAMBank1 + SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	144*256	+	SRAMBank1 + SyncLineD,
	144*256	+	SRAMBank1 + SyncLineD,
	2*256		+							SyncLineA,
	0
};

code UI PALSplitFieldTable[] = 
{
	22*256 	+							SyncLineB,
	192*256	+	SRAMBank1 + SyncLineC,
	96*256	+	SRAMBank2 + SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	192*256	+	SRAMBank1 + SyncLineD,
	96*256	+	SRAMBank2 + SyncLineD,
	2*256		+							SyncLineA,
	0
};

code UI PALWindowTable[] = 
{
	22*256 	+							SyncLineB,
	24*256	+         		SyncLineC,
	1*256		+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	1*256		+	  		      SyncLineC,
	1*256		+	SRAMBank3 + SyncLineC,
	44*256	+	      		  SyncLineC,
	144*256	+	SRAMBank4 + SyncLineC,
	72*256	+	    		    SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	24*256	+         		SyncLineD,
	1*256		+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	1*256		+	      		  SyncLineD,
	1*256		+	SRAMBank3 + SyncLineD,
	44*256	+	 			      SyncLineD,
	144*256	+	SRAMBank4 + SyncLineD,
	72*256	+	      		  SyncLineD,
	2*256		+							SyncLineA,
	0
};

code UI PALCheckfieldTable[] = 
{
	22*256 	+							SyncLineB,
	1*256		+	SRAMBank1 + SyncLineC,
	143*256	+	SRAMBank2 + SyncLineC,
	144*256	+	SRAMBank3 + SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	144*256	+	SRAMBank2 + SyncLineD,
	144*256	+	SRAMBank3 + SyncLineD,
	2*256		+							SyncLineA,
	0
};

code UI PALCrosshatchTable[] = 
{
	22*256 	+							SyncLineB,
	7*256		+ SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	20*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	7*256	  +	SRAMBank1 + SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	7*256		+ SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	20*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	7*256	  +	SRAMBank1 + SyncLineD,
	2*256		+							SyncLineA,
	0
};

code UI PALPLUGETable[] = 
{
	22*256 	+							SyncLineB,
	38*256	+       		  SyncLineC,
	53*256	+	SRAMBank1 + SyncLineC,
	53*256	+	SRAMBank2 + SyncLineC,
	53*256	+	SRAMBank3 + SyncLineC,
	53*256	+	SRAMBank4 + SyncLineC,
	38*256	+	  		      SyncLineC,
	2*256		+							SyncLineB,

	23*256 	+							SyncLineA,
	38*256	+     		    SyncLineD,
	53*256 	+	SRAMBank1 + SyncLineD,
	53*256 	+	SRAMBank2 + SyncLineD,
	53*256 	+	SRAMBank3 + SyncLineD,
	53*256 	+	SRAMBank4 + SyncLineD,
	38*256	+	 		    	  SyncLineD,
	2*256		+							SyncLineA,
	0
};

code UI NTSCBlackTable[] = 
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	122*256	+							SyncLineC,
	122*256	+							SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	122*256	+							SyncLineD,
	121*256	+							SyncLineD,
	0
};

code UI NTSCFullFieldTable[] = 
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	122*256	+	SRAMBank1	+	SyncLineC,
	122*256	+	SRAMBank1 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	122*256	+	SRAMBank1 + SyncLineD,
	121*256	+	SRAMBank1 + SyncLineD,
	0
};

code UI SMPTECBarTable[] = 
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	164*256	+	SRAMBank1 + SyncLineC,
	19*256	+	SRAMBank2 + SyncLineC,
	61*256	+	SRAMBank3 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	163*256	+	SRAMBank1 + SyncLineD,
	19*256	+	SRAMBank2 + SyncLineD,
	61*256	+	SRAMBank3 + SyncLineD,
	0
};

code UI NTSCWindowTable[] = 
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	30*256	+	        		SyncLineC,
	1*256		+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	1*256		+	    		    SyncLineC,
	1*256		+	SRAMBank3 + SyncLineC,
	29*256	+	 			    	SyncLineC,
	119*256	+	SRAMBank4 + SyncLineC,
	62*256	+			        SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	29*256	+	       			SyncLineD,
	1*256		+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	1*256		+	  		      SyncLineD,
	1*256		+	SRAMBank3 + SyncLineD,
	29*256	+	      		  SyncLineD,
	119*256	+	SRAMBank4 + SyncLineD,
	62*256	+	    		    SyncLineD,
	0
};

code UI NTSCCheckfieldTable[] = 
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	1*256		+	SRAMBank1 + SyncLineC,
	121*256	+	SRAMBank2 + SyncLineC,
	122*256	+	SRAMBank3 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	122*256	+	SRAMBank2 + SyncLineD,
	121*256	+	SRAMBank3 + SyncLineD,
	0
};

code UI NTSCCrosshatchTable[] = 
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	9*256		+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	17*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	16*256	+	SRAMBank1 + SyncLineC,
	1*256		+	SRAMBank2 + SyncLineC,
	7*256		+	SRAMBank1 + SyncLineC,

	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	8*256		+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	16*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	17*256	+	SRAMBank1 + SyncLineD,
	1*256		+	SRAMBank2 + SyncLineD,
	6*256		+	SRAMBank1 + SyncLineD,
	0
};

code UI NTSCPLUGETable[] = 
{
	3*256 	+							SyncLineA,
	16*256	+							SyncLineB,
	53*256	+	 		       	SyncLineC,
	56*256	+	SRAMBank1 + SyncLineC,
	19*256	+	SRAMBank2 + SyncLineC,
	19*256	+	SRAMBank3 + SyncLineC,
	19*256	+	SRAMBank4 + SyncLineC,
	78*256	+	    		    SyncLineC,
										
	2*256		+							SyncLineB,
	17*256 	+							SyncLineA,
	52*256	+	    		    SyncLineD,
	56*256	+	SRAMBank1 + SyncLineD,
	19*256	+	SRAMBank2 + SyncLineD,
	19*256	+	SRAMBank3 + SyncLineD,
	19*256	+	SRAMBank4 + SyncLineD,
	78*256	+	       		 	SyncLineD,
	0
};
																							// Signal array defined in ROM
																							//  (size/start is specified in BYTES
																							//  in config.h)
code int VideoSignalArray[SizeOfCBVideoCode/2] _at( StartOfCBVideoCode);
code int SoundSignalArray[SizeOfEmbAudioCode/2] _at( StartOfEmbAudioCode);

volatile int SRAMVideoPort _at( 0x44000); 			// Port defined in PLD
volatile int SRAMSoundPort _at( 0x40000); 			// Port defined in PLD
volatile int StatusPort _at( 0x6a000); 				// Port defined in PLD
volatile int HOffsetPort _at( 0x6a004); 			// Port defined in PLD
volatile int ChromaGainPort _at( 0x6a008);  	// Port defined in PLD

volatile _near UI TimeTickCounter;
volatile _near int LineOffset;

volatile int PhaseOffset;

CBObject CBSetup;

static _near UI Status _at( 0x120);

static code UI * _near TablePtr _at( 0x122);
static code UI *newTablePtr;

static _near UI LineCnt _at( 0x126);
static _near UC FrameCnt _at( 0x128);

static _near UC SyncLineCnt _at( 0x129);
static _near UC SyncLinePtr _at( 0x12A);
																 
static _near UC SoundFrameCnt _at( 0x12B);
static _near UC SoundCnt _at( 0x12C);
static _near UC SoundPtr _at( 0x12D);
static _near UC SoundPtrMax _at( 0x12E);

static char Blanking, NewPattern, SoundEnable;

/**************************************************************************/
/* WriteCBGain		 																						 	   CBAR.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000504																		*/
/* Revised:	 000606, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int WriteCBGain( UC gain)
{
	return( Write_AD8402( CBPotMeter, 0, gain));
}

/**************************************************************************/
/* WriteCBChromaGain		 																			 	   CBAR.C */
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000504																		*/
/* Revised:	 000603, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 The resistor at V118 has been swapped ie. bit 0 is bit 3 etc.*/
/*					 The conversion is done in SW instead of correcting the parts	*/
/*					 list.																												*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int WriteCBChromaGain( UC gain)
{
	UC tmp = 0;

	// gain bit 1 = bit4
	// gain bit 2 = bit3
	// gain bit 3 = bit2
	// gain bit 4 = bit1

	if ( gain & 0x01)
		tmp += 0x08;

	if ( gain & 0x02)
		tmp += 0x04;

	if ( gain & 0x04)
		tmp += 0x02;

	if ( gain & 0x08)
		tmp += 0x01;

	ChromaGainPort = tmp;

	return( OK);
}

/**************************************************************************/
/* SRAMVideoLoad																 	 CBAR.C */
/*																								  */
/* Author:	 Kim Engedahl, DEV, 000324												  */
/* Revised:	 000603, KEn, DEV     													  */
/*																								  */
/* Function: Copy video data to video RAM											  */
/* Remarks:	 S5,S4,S3 and S2 should be kept at ZERO while loading AND the */
/*					 interrupt system should NOT clear the PS bit.				  */
/* Returns:																					  */
/* Updates:																					  */
/**************************************************************************/
int SRAMVideoLoad( UC signal, UC bank)
{
	volatile int *VideoDstPtr;
	code int *VideoSrcPtr;
	int i;

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
/* SRAMSoundLoad																    CBAR.C */
/*																								  */
/* Author:	 Kim Engedahl, DEV, 000410												  */
/* Revised:	 000603, KEn, DEV     													  */
/*																								  */
/* Function: Copy sound data to sound RAM											  */
/* Remarks:	 																				  */
/* Returns:																					  */
/* Updates:																					  */
/**************************************************************************/
int SRAMSoundLoad( UC signal)
{
	volatile int *SoundDstPtr;
	code int *SoundSrcPtr;
	int i;

	signal = ( signal == EmbAudioStereo1kHz);

	Status |= PS;									// Set PS bit while loading
	StatusPort = Status;
																						 
	SoundDstPtr = &SRAMSoundPort;
	SoundSrcPtr = &SoundSignalArray[2304*signal];

	for ( i = 0; i < 2304; i++)
		*SoundDstPtr++ = *SoundSrcPtr++;

	Status &= ~PS;									// Clear PS bit again
	StatusPort = Status;

	return( OK);
}

/**************************************************************************/
/* CBLineInterrupt															    CBAR.C */
/*																								  */
/* Author:	 Kim Engedahl, DEV, 000322												  */
/* Revised:	 000603, KEn, DEV     													  */
/*																								  */
/* Function: 																				  */
/* Remarks:	 Delayed field interrupt is synchronized to line ??/??		  */
/* Returns:																					  */
/* Updates:																					  */
/**************************************************************************/
void CBLineInterrupt( void)
{
  static UI tmp;

	if ( CBSetup.System < NTSC)
	{
		if ( TF0)									// If delayed field interrupt...
		{
			TF0 = 0;									// Clear delayed field interrupt

			SyncLineCnt = 19;						// Next interrupt in 19 lines
			SyncLinePtr = 0;
	
			LineCnt = 3;

			SoundCnt = 2;
			SoundPtr = 2;

			SoundPtrMax = 64;

			Status |= FIELD1;						// This is field 1 of 8
			Status &= ~SYSSEL;					
			StatusPort = Status;

			return;
		}
		else
		{
			if ( Status & FIELD1)
			{
				Status &= ~FIELD1;
				StatusPort = Status;
			}

			LineCnt++;

			if ( CBSetup.System == PAL_PALID)
			{
				if ( LineCnt == 6)				// Prepare for line 7
				{
					Status |= LINE7PULSE;
					StatusPort = Status;			// Update status port
				}

				if ( LineCnt == 7)				// Prepare for line 8
				{
					Status &= ~LINE7PULSE;
					StatusPort = Status;			// Update status port
				}

			}		

			if ( !--SoundCnt)
			{
				if ( SoundPtr > SoundPtrMax)
				{
					SoundPtr = 0;

					if ( SoundPtrMax == 63)
						SoundPtrMax = 64;
					else
						SoundPtrMax = 63;
				}
				tmp = Sound625[SoundPtr++];	// Get sound information
		
				Status &= ~SOUND;					// Clear previous sound sample

				if ( SoundEnable) 				// If embedded audio on..
		 			Status |= ( tmp & SOUND);	//  set sound samples

				SoundCnt = tmp & 0x0F;			// Lines to next sound update
				StatusPort = Status;				// Update status port
			}
		}
	}
	else
	{
		if ( TF0)									// If delayed field interrupt...
		{
			TF0 = 0;									// Clear delayed field interrupt

			if ( FrameCnt > 9)
			{
				FrameCnt = 1;
				SoundFrameCnt = 1;
			}

			SyncLineCnt = 12;								// Next interrupt in 12 lines
			SyncLinePtr = 1;

			LineCnt = 6;

			SoundCnt = 2;
			SoundPtr = 3;
			SoundPtrMax = 47;

			Status |= FIELD1;								// This is field 1 of 4
			Status |= SYSSEL;
			StatusPort = Status;

			return;
		}
		else
		{
			if ( Status & FIELD1)
			{
				Status &= ~FIELD1;	
				StatusPort = Status;
			}

			if ( ++LineCnt >= 525)
			{
				FrameCnt++;
				LineCnt = 0;

				if ( ++SoundFrameCnt > 5)
					SoundFrameCnt = 1;
			}

			if ( !--SoundCnt)
			{
				if ( SoundPtr > SoundPtrMax)
				{
					SoundPtr = 0;

					if ( SoundPtrMax == 46)
						SoundPtrMax = 47;
					else
						SoundPtrMax = 46;
				}
				tmp = Sound525[SoundPtr];			// Get sound information

				if ( SoundPtr++ == 47)				// Prepare for line 263
					if ( SoundFrameCnt & 0x01)	// Sound frames 1,3,5 are special
						tmp |= S3OR4;

				Status &= ~SOUND;							// Clear previous sound sample

				if ( SoundEnable)
					Status |= ( tmp & SOUND);		// Sound samples

				SoundCnt = tmp & 0x0F;	 			// Lines to next sound update
				StatusPort = Status;					// Update status port
			}
		}
	}

	if ( --SyncLineCnt == 0)
	{
	 	tmp = TablePtr[++SyncLinePtr];

		if ( tmp == 0)
		{
			if ( NewPattern)
			{
				NewPattern = Blanking = FALSE;

				TablePtr = newTablePtr;
			}

			SyncLinePtr = 0;
			tmp = TablePtr[0];
		}

		SyncLineCnt = tmp / 256;

		tmp %= 256;
		tmp &= 0x3F;

		Status &= ~( S5S4S3S2 | S1S0);
		Status |= tmp;

		if ( Blanking)										// If blanking do NOT enable any
			Status &= ~S5S4S3S2;						//  bank

		StatusPort = Status;
	}	
}

/**************************************************************************/
/* CBDefaults																								     	CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000526																		*/
/* Revised:	 000617, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void CBDefaults( void)
{
	Settings.CBSetup.System = CBSetup.System = Calibration.ResetSystem;

	if ( CBSetup.System < NTSC)
		Settings.CBSetup.Pattern = CBSetup.Pattern = EBUCBar;
	else
		Settings.CBSetup.Pattern = CBSetup.Pattern = SMPTECBar;

	Settings.CBSetup.Delay = CBSetup.Delay = 0;
	Settings.CBSetup.ScHPhase = CBSetup.ScHPhase = 0;
	Settings.CBSetup.EmbAudioSignal = CBSetup.EmbAudioSignal = EmbAudioOff;

	NV_Store( &Settings, SettingsPtr, sizeof( Settings));
}

/**************************************************************************/
/* CBInit																										     	CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000322																		*/
/* Revised:	 0009145, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int CBInit( void)
{
	SyncLinePtr = 0;
	TablePtr = &NTSCFullFieldTable[0];

	NewPattern = FALSE;
	Blanking = TRUE;
	SoundEnable = FALSE;

	Status |= SYSSEL;
	StatusPort = Status;

	SyncLineCnt = 1;									// Interrupt in 1 line

	CBSetup.System = Settings.CBSetup.System;
	CBSetup.Pattern = Settings.CBSetup.Pattern;
	CBSetup.Delay = Settings.CBSetup.Delay;
	CBSetup.ScHPhase = Settings.CBSetup.ScHPhase;
	CBSetup.EmbAudioSignal = Settings.CBSetup.EmbAudioSignal;

	return( 0);
}

/**************************************************************************/
/* CBUpdate																									     	 CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000413																		*/
/* Revised:	 000603, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int CBUpdate( void)
{
	SetCBSystem( CBSetup.System);			// Also updates pattern

	SetCBDelay( CBSetup.Delay);
	SetCBScHPhase( CBSetup.ScHPhase);
	SetCBEmbAudioSignal( CBSetup.EmbAudioSignal);

	return( OK);
}

/**************************************************************************/
/* SetCBPattern																								     CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBPattern( UI pattern)
{
	int system = CBSetup.System;

	if ( ValidateCBPattern( system , &pattern) == FAIL)
		return( FAIL);

	Settings.ActivePreset = 0;

	Settings.CBSetup.Pattern = CBSetup.Pattern = pattern;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	Blanking = TRUE;

	Status &= ~S5S4S3S2;								 				// Blank line immediately
	StatusPort = Status;

	switch ( pattern)
	{
		case SMPTECBar:
			SRAMVideoLoad( dSMPTECBar, SRAMBank1);
			SRAMVideoLoad( dBlueBars, SRAMBank2);
			SRAMVideoLoad( dIWhiteQ, SRAMBank3);

			newTablePtr = &SMPTECBarTable[0];
			break;

		case EBUCBar:
			SRAMVideoLoad( dEBUCBar_625, SRAMBank1);

			newTablePtr = &PALFullFieldTable[0];
			break;

		case FCCCBar:
			SRAMVideoLoad( dFCC_CBar_525, SRAMBank1);

			newTablePtr = &NTSCFullFieldTable[0];
			break;

		case EBUCBarITU:
			SRAMVideoLoad( dEBUCBar_8bit_625, SRAMBank1);

			if ( system < NTSC)
				newTablePtr = &PALFullFieldTable[0];
			else
				newTablePtr = &NTSCFullFieldTable[0];
			break;

		case CBar100:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dCBar100_625, SRAMBank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dFCC_CBar100_525, SRAMBank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case CBarGrey75:
			SRAMVideoLoad( dCBar75_STD_625, SRAMBank1);
			SRAMVideoLoad( dGrey75_625, SRAMBank2);
	
			newTablePtr = &PALSplitFieldTable[0];
			break;

		case CBarRed75:
			SRAMVideoLoad( dEBUCBar_625, SRAMBank1);
			SRAMVideoLoad( dRed75_625, SRAMBank2);

			newTablePtr = &PALSplitFieldTable[0];
			break;
			
		case Red75:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dRed75_625, SRAMBank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dRed75_525, SRAMBank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Multiburst:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dChroma_Multiburst, SRAMBank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dFCC_Multiburst_525, SRAMBank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case CCIR18:
			SRAMVideoLoad( dCCIR18_625, SRAMBank1);

			newTablePtr = &PALFullFieldTable[0];
			break;

		case Window10:
			SRAMVideoLoad( dWindow50Marker, SRAMBank1);
			SRAMVideoLoad( dMarkerLine, SRAMBank2);
			SRAMVideoLoad( dMarker10, SRAMBank3);
			SRAMVideoLoad( dWindow10, SRAMBank4);

			if ( system < NTSC)												
				newTablePtr = &PALWindowTable[0];
			else
				newTablePtr = &NTSCWindowTable[0];
			break;

		case Window15:
			SRAMVideoLoad( dWindow50Marker, SRAMBank1);
			SRAMVideoLoad( dMarkerLine, SRAMBank2);
			SRAMVideoLoad( dMarker15, SRAMBank3);
			SRAMVideoLoad( dWindow15, SRAMBank4);

			if ( system < NTSC)												
				newTablePtr = &PALWindowTable[0];
			else
				newTablePtr = &NTSCWindowTable[0];
			break;

		case Window20:
			SRAMVideoLoad( dWindow50Marker, SRAMBank1);
			SRAMVideoLoad( dMarkerLine, SRAMBank2);
			SRAMVideoLoad( dMarker20, SRAMBank3);
			SRAMVideoLoad( dWindow20, SRAMBank4);

			if ( system < NTSC)												
				newTablePtr = &PALWindowTable[0];
			else
				newTablePtr = &NTSCWindowTable[0];
			break;

		case Window100:
			SRAMVideoLoad( dWindow50Marker, SRAMBank1);
			SRAMVideoLoad( dMarkerLine, SRAMBank2);
			SRAMVideoLoad( dMarker100, SRAMBank3);
			SRAMVideoLoad( dWindow100, SRAMBank4);

			if ( system < NTSC)												
				newTablePtr = &PALWindowTable[0];
			else
				newTablePtr = &NTSCWindowTable[0];
			break;

		case BlWh15kHz:
			if ( system < NTSC)												
			{
				SRAMVideoLoad( dkHz15BlWh_625, SRAMBank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dkHz15BlWh_525, SRAMBank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case White100:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dWhiteLine_625, SRAMBank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dWhiteLine_525, SRAMBank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Black:
			if ( system < NTSC)
				newTablePtr = &PALBlackTable[0];
			else
				newTablePtr = &NTSCBlackTable[0];
			break;

		case CheckField:
			SRAMVideoLoad( dSDI_test_line1, SRAMBank1);
			SRAMVideoLoad( dSDI_test_line2, SRAMBank2);
			SRAMVideoLoad( dSDI_test_line3, SRAMBank3);

			if ( system < NTSC)
				newTablePtr = &PALCheckfieldTable[0];
			else
				newTablePtr = &NTSCCheckfieldTable[0];
			break;

		case DigitalGrey:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dDigitalGrey_625, SRAMBank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dDigitalGrey_525, SRAMBank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Stair5:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dGreyScale5_625, SRAMBank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dGreyScale5_525, SRAMBank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Stair10:
			if ( system < NTSC)	
			{
				SRAMVideoLoad( dGreyScale10_625, SRAMBank1);

				newTablePtr = &PALFullFieldTable[0];
			}
			else
			{
				SRAMVideoLoad( dGreyScale10_525, SRAMBank1);

				newTablePtr = &NTSCFullFieldTable[0];
			}
			break;

		case Crosshatch:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dCrossHatch19_625, SRAMBank1);
				SRAMVideoLoad( dWhiteLine_625, SRAMBank2);

				newTablePtr = &PALCrosshatchTable[0];
			}
			else
			{
				SRAMVideoLoad( dCrossHatch17_525, SRAMBank1);
				SRAMVideoLoad( dWhiteLine_525, SRAMBank2);

				newTablePtr = &NTSCCrosshatchTable[0];
			}
			break;

		case PLUGE:
			if ( system < NTSC)
			{
				SRAMVideoLoad( dPlugeWhite_625, SRAMBank1);
				SRAMVideoLoad( dPlugeGreyC_625, SRAMBank2);
				SRAMVideoLoad( dPlugeGreyD_625, SRAMBank3);
				SRAMVideoLoad( dPlugeGreyE_625, SRAMBank4);

				newTablePtr = &PALPLUGETable[0];
			}
			else
			{
				SRAMVideoLoad( dPlugeWhite_525, SRAMBank1);
				SRAMVideoLoad( dPlugeGreyC_525, SRAMBank2);
				SRAMVideoLoad( dPlugeGreyD_525, SRAMBank3);
				SRAMVideoLoad( dPlugeGreyE_525, SRAMBank4);

				newTablePtr = &NTSCPLUGETable[0];
			}
			break;
	}

	NewPattern = TRUE;

	return( OK);
}

/**************************************************************************/
/* SetCBSystem																	   CBAR.C  */
/*																								  */
/* Author:	 Kim Engedahl, DEV, 000331												  */
/* Revised:	 000705, KEn, DEV     													  */
/*																								  */
/* Function:   																			  */
/* Remarks:																					  */
/* Returns:																					  */
/* Updates:			  																		  */
/**************************************************************************/
int SetCBSystem( UC system)
{
	Settings.ActivePreset = 0;

	Settings.CBSetup.System = CBSetup.System = system;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( system < NTSC)
	{
		Status &= ~SYSSEL;
		StatusPort = Status;		// Select new (SDI) system

		WriteCBGain( Calibration.CBCalibration.GainG);
		WriteCBChromaGain( Calibration.CBCalibration.ChromaGainG);
	}
	else
	{
		Status |= SYSSEL;
		StatusPort = Status;		// Select new (SDI) system

		WriteCBGain( Calibration.CBCalibration.GainM);
		WriteCBChromaGain( Calibration.CBCalibration.ChromaGainM);
	}


	tvp6k_system( system);	// Initialise the composite output

													// Validate pattern and change if invalid in new system																		
	ValidateCBPattern( system , &CBSetup.Pattern);

													// This one initialises TablePtr;
	SetCBPattern( CBSetup.Pattern);

	SetCBScHPhase( CBSetup.ScHPhase);

	LEDSet( CBPALLED, system < NTSC);
	LEDSet( CBNTSCLED, system >= NTSC);

	return( OK);
}

/**************************************************************************/
/* SetCBDelay																										   CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 001009, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 CB Delay resolution is 37ns																	*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBDelay( UL delay)
{
	UL tmpOffset;

	Settings.ActivePreset = 0;

	Settings.CBSetup.Delay = CBSetup.Delay = delay;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	// Divide GenlockSetup.Delay with 256 as the resulotion is 27ns/256 whereas the
	//  resolution of the TSG is 37ns.
	tmpOffset = delay + GenlockSetup.Delay/256;

	if ( CBSetup.System < NTSC)
	{
		tmpOffset += Calibration.CBCalibration.PhaseG;

		tmpOffset %= 4320000L;						// 4*625*1728

		LineOffset = 0xFFFF - ( tmpOffset / 1728L);				
		PhaseOffset = 1727 - ( tmpOffset % 1728L);
	}
	else 
	{
		tmpOffset += Calibration.CBCalibration.PhaseM;

		tmpOffset %= 1801800L;						// 2*525*1716

		LineOffset = 0xFFFF - ( tmpOffset / 1716L);				
		PhaseOffset = 1715 - ( tmpOffset % 1716L);

		if ( PhaseOffset > 1687)
			PhaseOffset += 12;
	}

	if ( PhaseOffset >= 1443)
		Status |= PHASEREGSEL;				// Set Phase Register Select
	else		
		Status &= ~PHASEREGSEL;				// Clear Phase Register Select
		
	StatusPort = Status;

	HOffsetPort = PhaseOffset;

	return( OK);
}

/**************************************************************************/
/* SetCBScHPhase																								   CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 Note that the resolution is different from BlackBurst, (1.4).*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBScHPhase( int schphase)
{
	long tmp;
	UC buffer[2];

	Settings.ActivePreset = 0;

	Settings.CBSetup.ScHPhase = CBSetup.ScHPhase = schphase;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));
  
	if ( CBSetup.System < NTSC)
		tmp = (long) Calibration.CBCalibration.ScHPhaseG;
	else 
		tmp = (long) Calibration.CBCalibration.ScHPhaseM;

	tmp += (long) schphase;

	if ( tmp < 0)											// -1 deg equ. 359 deg a.s.f.
		tmp += 360;

	tmp *= 256;
	tmp /= 360;												// tmp = ( ScHPhase*256)/360;

	buffer[0] = C_PHASE_address;
	buffer[1] = tmp & 0xFF;
	
	return( TransmitIIC( TVP6K_IIC_address, 2, buffer, IIC_PTV));
}

/**************************************************************************/
/* SetCBEmbAudioSignal																					   CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000331																		*/
/* Revised:	 000618, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int SetCBEmbAudioSignal( UC signal)
{
	Settings.ActivePreset = 0;

	Settings.CBSetup.EmbAudioSignal = CBSetup.EmbAudioSignal = signal;
	NV_Store( &Settings, SettingsPtr, sizeof( Settings));

	if ( signal == EmbAudioOff)
		SoundEnable = FALSE;
	else
	{
		SoundEnable = TRUE;

		Blanking = TRUE;	 				 			// Blank line while loading SRAM

		Status &= ~S5S4S3S2;			 			// Blank the line immediately
		StatusPort = Status;

		SRAMSoundLoad( signal);		 			// Download sound information

		Blanking = FALSE;
	}

	return( OK);
}

/**************************************************************************/
/* ValidateCBPattern	 																				     CBAR.C	*/
/*																																				*/
/* Author:	 Kim Engedahl, DEV, 000405																		*/
/* Revised:	 000603, KEn, DEV     																				*/
/*																																				*/
/* Function:   																														*/
/* Remarks:	 																															*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
int ValidateCBPattern( UC system, UI *pattern)
{
	if ( system < NTSC)
	{
		switch ( *pattern)
		{
			case SMPTECBar:
			case FCCCBar:
				*pattern = EBUCBar;
				break;

			default:
				return( OK);
		}
	}
	else
	{
		switch ( *pattern)
		{
			case EBUCBar:
			case CBarGrey75:
			case CBarRed75:
				*pattern = SMPTECBar;
				break;

			case CCIR18:
				*pattern = Multiburst;
				break;

			default:
				return( OK);
		}
	}

	return( FAIL);
}