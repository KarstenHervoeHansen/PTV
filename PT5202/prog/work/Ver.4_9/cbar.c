/****************************************************************************/
/* MODULE:                                                                  */
/*  cbar.c - ColorBar generator                                             */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*  int WriteCBGain( UC gain)                                               */
/*  int WriteCBChromaGain( UC gain)                                          */
/*  int SRAMVideoLoad( UC signal, UC bank)                                  */
/*  int SRAMSoundLoad( UC signal)                                           */
/*  void CBLineInterrupt( void)                                             */
/*  void CBDefaults( void)                                                  */
/*  int CBInit( void)                                                        */
/*  int CBUpdate( void)                                                      */
/*  int SetCBPattern( UI pattern)                                            */
/*  int SetCBSystem( UC system)                                              */
/*  int SetCBDelay( UL delay)                                                */
/*  int SetCBScHPhase( int schphase)                                        */
/*  int SetCBEmbAudioSignal( UC signal)                                     */
/*  int ValidateCBPattern( UC system, UI *pattern)                          */  
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

#include "reg8051.h"
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

#define SRAMBank0      0x00
#define SRAMBank1      0x10
#define SRAMBank2      0x14
#define SRAMBank3      0x18
#define SRAMBank4      0x1C
#define SRAMBank5      0x20
#define SRAMBank6      0x24
#define SRAMBank7      0x28
#define SRAMBank8      0x2C
#define SRAMBank9      0x30
#define SRAMBank10    0x34
#define SRAMBank11    0x38
#define SRAMBank12    0x3C

#define dEBUCBar_8bit_625           0
#define dEBUCBar_625              1
#define dSMPTECBar                2
#define dBlueBars                  3
#define dIWhiteQ                  4
#define dCBar100_625              5
#define dGrey75_625                6
#define dRed75_625                7
#define dRed75_525                8
#define dChroma_Multiburst        9
#define d_CCIR18_625              9
#define dFCC_Multiburst_525        10
#define dChroma_Multiburst_525    11
#define dWhiteLine_625            11
#define dCrossHatch19_625          12
#define dWhiteLine_525            13
#define dCrossHatch17_525          14
#define dPlugeWhite_625            15
#define dPlugeGreyC_625            16
#define dPlugeGreyD_625            17
#define dPlugeGreyE_625            18
#define dPlugeWhite_525            19
#define dPlugeGreyC_525            20
#define dPlugeGreyD_525            21
#define dPlugeGreyE_525            22
#define dGreyScale5_625            23
#define dGreyScale5_525            24
#define dGreyScale10_625          25
#define dGreyScale10_525          26
#define dWindow50Marker            27
#define dMarkerLine              28
#define dWindow15                29
#define dMarker15                30
#define dWindow20                31
#define dMarker20                32
#define dWindow100              33
#define dMarker100              34
#define dDigitalGrey_625          35
#define dDigitalGrey_525          36
#define dSDI_test_line1            37
#define dSDI_test_line2            38
#define dSDI_test_line3            39
#define dCBar75_STD_625             40
#define dFCC_CBar_525              41
#define dFCC_CBar100_525          42
#define dWindow10                  43
#define dMarker10                  44
#define dkHz15BlWh_625            45
#define dkHz15BlWh_525            46
#define dCCIR18_625                47

/****************************************************************************/
/*
The statusport is defined in the PLD as:

  bit 0:  S0        Background system
  bit 1:  S1

  bit 2:  S2        Linetype
  bit 3:  S3
  bit 4:  S4
  bit 5:  S5

  bit 6:  SYSSEL    System select ( 0:G, 1:M)
  bit 7:  SOUND      Sound ( 0:off, 1:on)

  bit 8:  3OR4      Samples in sound ( 0:3 samples, 1:4 samples)
  bit 9:  FIELD1    Field one bit ( 1 in Field 1/line 1 (out of 8/4) otherwise 0)
  bit 10: CB  
  bit 11: EDHON      EDH bit for future use ( 0:off, 1:on)

  bit 12: PS        PS ( 0:disable writing in SRAM, 1: enable writing in SRAM)
  bit 13: Line7      Line 7 pulse in PAL w/PAL_ID
  bit 14: PhaseReg  Phase offset register select
  bit 15: -          Not used
*/
/****************************************************************************/
                    
#define S1S0        0x0003
#define S5S4S3S2    0x003C
#define SYSSEL      0x0040
#define SOUNDON      0x0080
#define S3OR4        0x0100
#define FIELD1      0x0200
#define CB          0x0400
#define EDHON        0x0800
#define PS          0x1000
#define LINE7PULSE  0x2000
#define PHASEREGSEL  0x4000

#define SOUND        (int)(SOUNDON + S3OR4)

#define SyncLineA    1
#define SyncLineB   0
#define SyncLineC   2
#define SyncLineD   3

/****************************************************************************/
// Prototypes internal functions

int ValidateCBPattern( UC system, UI *pattern);

/* Tables start at TSTART  */

#define TSTART 0x800

_ROM_ UI TEST_SMPTECBarTable[] _at(TSTART) =
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

#define TA1 (TSTART + sizeof(TEST_SMPTECBarTable))

_ROM_	UI PALBlackTable[] _at(TA1) =
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

#define TA2 (TA1 + sizeof(PALBlackTable))

_ROM_ UI PALFullFieldTable[] _at(TA2)=
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
#define TA3 (TA2 + sizeof(PALFullFieldTable))

_ROM_ UI PALSplitFieldTable[] _at(TA3) =
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
#define TA4 (TA3 + sizeof(PALSplitFieldTable))

_ROM_ UI PALWindowTable[] _at(TA4) =
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

#define TA5 (TA4 + sizeof(PALWindowTable))

_ROM_ UI PALCheckfieldTable[] _at(TA5) =
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

#define TA6 (TA5 + sizeof(PALCheckfieldTable))

_ROM_ UI PALCrosshatchTable[] _at(TA6) =
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

#define TA7 (TA6 + sizeof(PALCrosshatchTable))

_ROM_ UI PALPLUGETable[] _at(TA7) =
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

#define TA8 (TA7 + sizeof(PALPLUGETable))

_ROM_ UI NTSCBlackTable[] _at(TA8) =
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

#define TA9 (TA8 + sizeof(NTSCBlackTable))

_ROM_ UI NTSCFullFieldTable[] _at(TA9) =
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

#define TA10 (TA9 + sizeof(NTSCFullFieldTable))

_ROM_ UI SMPTECBarTable[] _at(TA10) =
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

#define TA11 (TA10 + sizeof(SMPTECBarTable))

_ROM_ UI NTSCWindowTable[] _at(TA11) =
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

#define TA12 (TA11 + sizeof(NTSCWindowTable))

_ROM_ UI NTSCCheckfieldTable[] _at(TA12) =
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

#define TA13 (TA12 + sizeof(NTSCCheckfieldTable))

_ROM_ UI NTSCCrosshatchTable[] _at(TA13) =
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

#define TA14 (TA13 + sizeof(NTSCCrosshatchTable))

_ROM_ UI NTSCPLUGETable[] _at(TA14) =
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

#define TA15 (TA14 + sizeof(NTSCPLUGETable))

	_ROM_ UI Sound525[] _at(TA15) = 
    {
  	0x083, 0x181, 0x085, 0x003, 0x086, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181,
  	0x08B, 0x181, 0x08B, 0x181, 0x08B, 0x181, 0x083, 0x081
    };

#define TA16 (TA15 + sizeof(Sound525))

	_ROM_ UI Sound625[65] _at(TA16) = 
	  {
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

//Emulates the 8 lines avalible for text.
//Would be implemented ase a seperat table.

#define TA17 (TA16 + sizeof(Sound625))

_ROM_ UI TextLines_SyncLineC[] _at(TA17) =		//Interlaced 
  	{
	1*256	+	SRAMBank5 + SyncLineC,		//Line 0
	1*256	+	SRAMBank6 + SyncLineC,
	1*256	+	SRAMBank7 + SyncLineC,
	1*256	+	SRAMBank8 + SyncLineC,
	1*256	+	SRAMBank9 + SyncLineC,
	1*256	+	SRAMBank10 + SyncLineC,
	1*256	+	SRAMBank11 + SyncLineC,
	1*256	+	SRAMBank12 + SyncLineC,
	};

#define TA18 (TA17 + sizeof(Sound625))

_ROM_ UI TextLines_SyncLineD[] _at(TA18) =		//De-Interlaced 
	{
	1*256	+	SRAMBank5 + SyncLineD,		//Line 0
	1*256	+	SRAMBank6 + SyncLineD,
	1*256	+	SRAMBank7 + SyncLineD,
	1*256	+	SRAMBank8 + SyncLineD,
	1*256	+	SRAMBank9 + SyncLineD,
	1*256	+	SRAMBank10 + SyncLineD,
	1*256	+	SRAMBank11 + SyncLineD,
	1*256	+	SRAMBank12 + SyncLineD,
	};

                                              // Signal array defined in ROM
                                              //  (size/start is specified in BYTES
                                              //  in config.h)
_ROM_ int VideoSignalArray[SizeOfCBVideoCode/2] _at( StartOfCBVideoCode);
_ROM_ int SoundSignalArray[SizeOfEmbAudioCode/2] _at( StartOfEmbAudioCode);

//KS. Extended Video SRAM. Test Only.
volatile int ESRAMVideoPort _at( 0x54000);       // Port defined in PLD
volatile int SRAMVideoPort _at( 0x44000);       // Port defined in PLD
volatile int SRAMSoundPort _at( 0x40000);       // Port defined in PLD
volatile int StatusPort _at( 0x6a000);         // Port defined in PLD
volatile int HOffsetPort _at( 0x6a004);       // Port defined in PLD
volatile int ChromaGainPort _at( 0x6a008);    // Port defined in PLD

/***/
volatile UI LEDPort _at( 0x6a00C);        // Port defined in PLD
volatile UI TestLEDPort _at( 0x6a014);  // Port defined in PLD


volatile _near UI TimeTickCounter;
volatile _near int LineOffset;

volatile int PhaseOffset;

CBObject CBSetup;

typedef struct cbtag
    {
    _ROM_  UI * TablePtr;    // 4 bytes
    _ROM_  UI * newTablePtr; // 4 bytes
    UI LineCnt;              // 2 bytes
    UC FrameCnt;             // 1 byte
    UC SyncLineCnt;
    UC SyncLinePtr;
    UC SoundFrameCnt;
    UC SoundCnt;
    UC SoundPtr;
    UC SoundPtrMax;
    char Blanking;
    char NewPattern; 
    char SoundEnable;
    UI Status;               // 2 bytes
    } JK;



typedef union xxtag
  {
  JK  CBX;
  UC  array[sizeof(JK)];
  } CBVARS;
  
CBVARS cb_vars;
CBVARS test_vars;

extern       _bit JKflag;
_sfrbit    JKpin   _atbit(P1,5);    // P1^5
_sfrbit    SDApin  _atbit(P1,6);    // P1^6

extern UI LEDMirror;
extern UI TestLEDMirror;

#define CLASSIC
//#undef  CLASSIC

/**************************************************************************/
/* CBLineInterrupt                                                 CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000322                                    */
/* Revised:   000603, KEn, DEV                                            */
/*                                                                        */
/* Function:                                                              */
/* Remarks:   Delayed field interrupt is synchronized to line ??/??       */
/* Returns:                                                               */
/* Updates:                                                               */
/**************************************************************************/
void CBLineInterrupt( void)
  {
  static UI tmp;
  static UI tmpv;

  static UC TextLineCnt;  //Indicated from which line the Text lines have to be inserted !   	
  static UC TextLineDir;  //Indicates direction of the movement of the text line !		

  static UI LineNum;  //Used to prevent Interlaving flicker ! 

  //UI i;
  
  //JKpin = 0;

  if ( CBSetup.System < NTSC_US)
    {
    ;
    }
  else    // NTSC
    {
    
    if ( TF0)                        // If delayed field interrupt...
      {
      TF0 = 0;                      // Clear delayed field interrupt
#if 1
      if ( cb_vars.CBX.FrameCnt > 9)
        {
        cb_vars.CBX.FrameCnt = 1;
        cb_vars.CBX.SoundFrameCnt = 1;
        }

      cb_vars.CBX.SyncLineCnt = 12;              // Next interrupt in 12 lines
      test_vars.CBX.SyncLineCnt = 12;              // Next interrupt in 12 lines
      cb_vars.CBX.SyncLinePtr = 1;

      cb_vars.CBX.LineCnt = 6;

      cb_vars.CBX.SoundCnt = 2;
      cb_vars.CBX.SoundPtr = 3;
      cb_vars.CBX.SoundPtrMax = 47;

      cb_vars.CBX.Status |= FIELD1;                // This is field 1 of 4
      StatusPort = cb_vars.CBX.Status;
//#else      
//again
      if ( test_vars.CBX.FrameCnt > 9)
        {
        test_vars.CBX.FrameCnt = 1;
        test_vars.CBX.SoundFrameCnt = 1;
        }

      test_vars.CBX.SyncLineCnt = 12;              // Next interrupt in 12 lines
      test_vars.CBX.SyncLinePtr = 1;

      test_vars.CBX.LineCnt = 6;

      test_vars.CBX.SoundCnt = 2;
      test_vars.CBX.SoundPtr = 3;
      test_vars.CBX.SoundPtrMax = 47;

      test_vars.CBX.Status = cb_vars.CBX.Status; //FIELD1;                // This is field 1 of 4
      //StatusPort = test_vars.CBX.Status;
#endif
//KS. Controls position of the text lines in the test pattern.
	
      if (TextLineDir==0)
	{ 	
      	if (TextLineCnt++ == 100) 
		TextLineDir = 1;	
	}
      else
	{ 	
      	if (TextLineCnt-- == 50)	
		TextLineDir = 0;	
	}

      goto JKexit;
      }
    else
      {
#if 1    
      if ( cb_vars.CBX.Status & FIELD1)
        {
        cb_vars.CBX.Status &= ~FIELD1;
        StatusPort = cb_vars.CBX.Status;
        }

      if ( ++cb_vars.CBX.LineCnt >= 525)
        {
        cb_vars.CBX.FrameCnt++;
        cb_vars.CBX.LineCnt = 0;
        if ( ++cb_vars.CBX.SoundFrameCnt > 5)
          cb_vars.CBX.SoundFrameCnt = 1;
        }
#if 0
      if ( !--cb_vars.CBX.SoundCnt)
        {
        if ( cb_vars.CBX.SoundPtr > cb_vars.CBX.SoundPtrMax)
          {
          cb_vars.CBX.SoundPtr = 0;

          if ( cb_vars.CBX.SoundPtrMax == 46)
            cb_vars.CBX.SoundPtrMax = 47;
          else
            cb_vars.CBX.SoundPtrMax = 46;
          }
        tmp = Sound525[cb_vars.CBX.SoundPtr];      // Get sound information

        if ( cb_vars.CBX.SoundPtr++ == 47)        // Prepare for line 263
          if ( cb_vars.CBX.SoundFrameCnt & 0x01)  // Sound frames 1,3,5 are special
            tmp |= S3OR4;

        cb_vars.CBX.Status &= ~SOUND;              // Clear previous sound sample

        if ( cb_vars.CBX.SoundEnable)
          {
          cb_vars.CBX.Status |= ( tmp & SOUND);    // Sound samples
          }
        cb_vars.CBX.SoundCnt = tmp & 0x0F;         // Lines to next sound update
        StatusPort = cb_vars.CBX.Status;          // Update status port
        }
#endif          
//#else
//again
      if ( test_vars.CBX.Status & FIELD1)
        {
        test_vars.CBX.Status &= ~FIELD1;
        //StatusPort = test_vars.CBX.Status;
        }

      if ( ++test_vars.CBX.LineCnt >= 525)
        {
        test_vars.CBX.FrameCnt++;
        test_vars.CBX.LineCnt = 0;
        if ( ++test_vars.CBX.SoundFrameCnt > 5)
          test_vars.CBX.SoundFrameCnt = 1;
        }
#if 0
      if ( !--test_vars.CBX.SoundCnt)
        {
        if ( test_vars.CBX.SoundPtr > test_vars.CBX.SoundPtrMax)
          {
          test_vars.CBX.SoundPtr = 0;

          if ( test_vars.CBX.SoundPtrMax == 46)
            test_vars.CBX.SoundPtrMax = 47;
          else
            test_vars.CBX.SoundPtrMax = 46;
          }
        tmpv = Sound525[test_vars.CBX.SoundPtr];      // Get sound information

        if ( test_vars.CBX.SoundPtr++ == 47)        // Prepare for line 263
          if ( test_vars.CBX.SoundFrameCnt & 0x01)  // Sound frames 1,3,5 are special
            tmpv |= S3OR4;

        test_vars.CBX.Status &= ~SOUND;              // Clear previous sound sample

        if ( test_vars.CBX.SoundEnable)
          test_vars.CBX.Status |= ( tmpv & SOUND);    // Sound samples
        test_vars.CBX.SoundCnt = tmpv & 0x0F;         // Lines to next sound update
        //StatusPort = test_vars.CBX.Status;          // Update status port
        }
#endif
#endif        
     }
    } // if NTSC
#ifdef CLASSIC   
  if ( --cb_vars.CBX.SyncLineCnt == 0)
    {
    JKpin = 1;
    tmp = cb_vars.CBX.TablePtr[++cb_vars.CBX.SyncLinePtr];
    JKpin = 0;
    if ( tmp == 0)
      {
      if ( cb_vars.CBX.NewPattern)
        {     
        cb_vars.CBX.NewPattern = cb_vars.CBX.Blanking = FALSE;

        cb_vars.CBX.TablePtr = cb_vars.CBX.newTablePtr;
        }
      cb_vars.CBX.SyncLinePtr = 0;
      tmp = cb_vars.CBX.TablePtr[0];
      }

    cb_vars.CBX.SyncLineCnt = tmp / 256; // extract line cnt to SyncLineCnt
    tmp %= 256;              // extract SramBankX + SyncLineX tmp
    tmp &= 0x3F;
    cb_vars.CBX.Status &= ~( S5S4S3S2 | S1S0);
    cb_vars.CBX.Status |= tmp;

    if ( cb_vars.CBX.Blanking)                    // If blanking do NOT enable any
      cb_vars.CBX.Status &= ~S5S4S3S2;            //  bank
    StatusPort = cb_vars.CBX.Status;
    }
//KS.Test of text line moving in the test pattern. 

//Field #1.

     if (cb_vars.CBX.LineCnt >= TextLineCnt)		//Start of text lines !
        {
     	if (cb_vars.CBX.LineCnt <= (TextLineCnt+7))
        	{
		   tmp = TextLines_SyncLineC[cb_vars.CBX.LineCnt - TextLineCnt];
    		tmp %= 256;              // extract SramBankX + SyncLineX tmp
    		tmp &= 0x3F;
    		cb_vars.CBX.Status &= ~( S5S4S3S2 | S1S0);
    		cb_vars.CBX.Status |= tmp;
    		StatusPort = cb_vars.CBX.Status;
	        } 
       }
     if (cb_vars.CBX.LineCnt == (TextLineCnt + 8)) 	//Restore original pattern 
        {
	     tmp = cb_vars.CBX.TablePtr[cb_vars.CBX.SyncLinePtr];
   	  tmp %= 256;              // extract SramBankX + SyncLineX tmp
    	  tmp &= 0x3F;
    	  cb_vars.CBX.Status &= ~( S5S4S3S2 | S1S0);
    	  cb_vars.CBX.Status |= tmp;
    	  StatusPort = cb_vars.CBX.Status;
        }


//Field #2.
//To prevent Interlacing "flicker" Down and Up movements is handled differently.

     if (TextLineDir == 0)	//Down movement.
	{
	LineNum =	263; 	
	}
     else			//Up movement.
	{
	LineNum =	262; 	
	}  	

     if (cb_vars.CBX.LineCnt >= (TextLineCnt + LineNum))   //263 = number of lines in a feeld #1. !
        {
     	if (cb_vars.CBX.LineCnt <= (TextLineCnt + 7 + LineNum))
        	{
		tmp = TextLines_SyncLineD[cb_vars.CBX.LineCnt - (TextLineCnt + LineNum)];
    		tmp %= 256;              // extract SramBankX + SyncLineX tmp
    		tmp &= 0x3F;
    		cb_vars.CBX.Status &= ~( S5S4S3S2 | S1S0);
    		cb_vars.CBX.Status |= tmp;
    		StatusPort = cb_vars.CBX.Status;
		} 
       }
     if (cb_vars.CBX.LineCnt == (TextLineCnt + 8 + LineNum))  //Restore original pattern
        {
	tmp = cb_vars.CBX.TablePtr[cb_vars.CBX.SyncLinePtr];
   	tmp %= 256;              // extract SramBankX + SyncLineX tmp
    	tmp &= 0x3F;
    	cb_vars.CBX.Status &= ~( S5S4S3S2 | S1S0);
    	cb_vars.CBX.Status |= tmp;
    	StatusPort = cb_vars.CBX.Status;
       	}

//#else
  // again for test_vars *****************************************
  if ( --test_vars.CBX.SyncLineCnt == 0)
    {
     tmpv = test_vars.CBX.TablePtr[++test_vars.CBX.SyncLinePtr];
    if ( tmpv == 0)
      {
      if ( test_vars.CBX.NewPattern)
        {
        test_vars.CBX.NewPattern = test_vars.CBX.Blanking = FALSE;

        test_vars.CBX.TablePtr = test_vars.CBX.newTablePtr;
        }
      test_vars.CBX.SyncLinePtr = 0;
      tmpv = test_vars.CBX.TablePtr[0];
      }

    test_vars.CBX.SyncLineCnt = tmpv / 256; // extract line cnt to SyncLineCnt
    tmpv %= 256;              // extract SramBankX + SyncLineX tmp
    tmpv &= 0x3F;
    test_vars.CBX.Status &= ~( S5S4S3S2 | S1S0);
    test_vars.CBX.Status |= tmpv;

    if ( test_vars.CBX.Blanking)                    // If blanking do NOT enable any
      test_vars.CBX.Status &= ~S5S4S3S2;            //  bank
    //StatusPort = test_vars.CBX.Status;
    
    }
#endif 
JKexit: 
  LEDPort     = LEDMirror;
  TestLEDPort = TestLEDMirror;

#if 0
  for (i = 12; i < 13; i++)
    //if (test_vars.array[i] != cb_vars.array[i])
     if (test_vars.CBX.Status != cb_vars.CBX.Status)
      JKpin = 1;
#endif
  //JKpin = 1;
  return;      
  }



/**************************************************************************/
/* WriteCBGain                                                       CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000504                                    */
/* Revised:   000606, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int WriteCBGain( UC gain)
{
  return( Write_AD8402( CBPotMeter, 0, gain));
}

/**************************************************************************/
/* WriteCBChromaGain                                                 CBAR.C */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000504                                    */
/* Revised:   000603, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:   The resistor at V118 has been swapped ie. bit 0 is bit 3 etc.*/
/*           The conversion is done in SW instead of correcting the parts  */
/*           list.                                                        */
/* Returns:                                                                */
/* Updates:                                                                */
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
/* SRAMVideoLoad                                    CBAR.C		 */
/*                                                  			*/
/* Author:   Kim Steffensen,                                   		*/
/* Revised:                                                 		*/
/*                                                  			*/
/* Function: Copy video data to extended video RAM                        */
/* Remarks:  S5,S4,S3 and S2 should be kept at ZERO while loading AND the */
/*           interrupt system should NOT clear the PS bit.          	*/
/* Returns:                                            			*/
/* Updates:                                            			*/
/**************************************************************************/
int SRAMVideoLoad( UC signal, UC bank)
{
  volatile int *VideoDstPtr;
  _ROM_  int *VideoSrcPtr;
  int i;

//  bit 12: PS(Bit 12 in status register)  PS ( 0:disable writing in SRAM, 1: enable writing in SRAM)

  test_vars.CBX.Status |= PS;                    // Set PS bit while loading
  cb_vars.CBX.Status |= PS;                    // Set PS bit while loading
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif
                                                 
  VideoDstPtr = &SRAMVideoPort + (( bank/4)-4) * 0x0800;	//Destination Pointer
  VideoSrcPtr = &VideoSignalArray[ 0x800 * (long)signal];	//Source Pointer

  for ( i = 0; i < 2048; i++)
    *VideoDstPtr++ = *VideoSrcPtr++;

  test_vars.CBX.Status &= ~PS;                  // Clear PS bit again
  cb_vars.CBX.Status &= ~PS;                  // Clear PS bit again
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif

  return( OK);
}

/**************************************************************************/
/* ESRAMVideoLoad                                    CBAR.C		 */
/*                                                  			*/
/* Author:   Kim Steffensen,                                   		*/
/* Revised:                                                 		*/
/*                                                  			*/
/* Function: Copy video data to extended video RAM                        */
/* Remarks:  S5,S4,S3 and S2 should be kept at ZERO while loading AND the */
/*           interrupt system should NOT clear the PS bit.          	*/
/* Returns:                                            			*/
/* Updates:                                            			*/
/**************************************************************************/
int ESRAMVideoLoad( UC signal, UC bank)
{
  volatile int *VideoDstPtr;
  _ROM_  int *VideoSrcPtr;
  int i;

//  bit 12: PS(Bit 12 in status register)  PS ( 0:disable writing in SRAM, 1: enable writing in SRAM)

  test_vars.CBX.Status |= PS;                    // Set PS bit while loading
  cb_vars.CBX.Status |= PS;                    // Set PS bit while loading
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif
                                                 
  VideoDstPtr = &ESRAMVideoPort + (( bank/4)-4) * 0x0800;	//Destination Pointer
  VideoSrcPtr = &VideoSignalArray[ 0x800 * (long)signal];	//Source Pointer

  for ( i = 0; i < 2048; i++)
    *VideoDstPtr++ = *VideoSrcPtr++;

  test_vars.CBX.Status &= ~PS;                  // Clear PS bit again
  cb_vars.CBX.Status &= ~PS;                  // Clear PS bit again
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif

  return( OK);
}

/**************************************************************************/
/* SRAMSoundLoad                                    CBAR.C */
/*                                                  */
/* Author:   Kim Engedahl, DEV, 000410                          */
/* Revised:   000603, KEn, DEV                                 */
/*                                                  */
/* Function: Copy sound data to sound RAM                        */
/* Remarks:                                             */
/* Returns:                                            */
/* Updates:                                            */
/**************************************************************************/
int SRAMSoundLoad( UC signal)
{
  volatile int *SoundDstPtr;
  _ROM_  int *SoundSrcPtr;
  int i;

  signal = ( signal == EmbAudioStereo1kHz);

  test_vars.CBX.Status |= PS;                  // Set PS bit while loading
  cb_vars.CBX.Status |= PS;                  // Set PS bit while loading
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif
                                             
  SoundDstPtr = &SRAMSoundPort;
  SoundSrcPtr = &SoundSignalArray[2304*signal];

  for ( i = 0; i < 2304; i++)
    *SoundDstPtr++ = *SoundSrcPtr++;

  test_vars.CBX.Status &= ~PS;                  // Clear PS bit again
  cb_vars.CBX.Status &= ~PS;                  // Clear PS bit again
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif

  return( OK);
}


/**************************************************************************/
/* CBDefaults                                                       CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000526                                    */
/* Revised:   000617, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
void CBDefaults( void)
{
  Settings.CBSetup.System = CBSetup.System = Calibration.ResetSystem;

  if ( CBSetup.System < NTSC_US)
    Settings.CBSetup.Pattern = CBSetup.Pattern = EBUCBar;
  else
    Settings.CBSetup.Pattern = CBSetup.Pattern = SMPTECBar;

  Settings.CBSetup.Delay = CBSetup.Delay = 0;
  Settings.CBSetup.ScHPhase = CBSetup.ScHPhase = 0;
  Settings.CBSetup.EmbAudioSignal = CBSetup.EmbAudioSignal = EmbAudioOff;

  NV_Store( &Settings, SettingsPtr, sizeof( Settings));
}

/**************************************************************************/
/* CBInit                                                           CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000322                                    */
/* Revised:   0009145, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int CBInit( void)
{
  cb_vars.CBX.SyncLinePtr = 0;
  cb_vars.CBX.TablePtr = &NTSCFullFieldTable[0];

  cb_vars.CBX.NewPattern = FALSE;
  cb_vars.CBX.Blanking = TRUE;
  cb_vars.CBX.SoundEnable = FALSE;

  test_vars.CBX.SyncLinePtr = 0;
  test_vars.CBX.TablePtr = &NTSCFullFieldTable[0];

  test_vars.CBX.NewPattern = FALSE;
  test_vars.CBX.Blanking = TRUE;
  test_vars.CBX.SoundEnable = FALSE;

  cb_vars.CBX.Status |= SYSSEL;
  test_vars.CBX.Status |= SYSSEL;
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif

  cb_vars.CBX.SyncLineCnt = 1;                  // Interrupt in 1 line
  test_vars.CBX.SyncLineCnt = 1;                  // Interrupt in 1 line

  CBSetup.System = Settings.CBSetup.System;
  CBSetup.Pattern = Settings.CBSetup.Pattern;
  CBSetup.Delay = Settings.CBSetup.Delay;
  CBSetup.ScHPhase = Settings.CBSetup.ScHPhase;
  CBSetup.EmbAudioSignal = Settings.CBSetup.EmbAudioSignal;

  return( 0);
}

/**************************************************************************/
/* CBUpdate                                                          CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000413                                    */
/* Revised:   000603, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int CBUpdate( void)
{
  SetCBSystem( CBSetup.System);      // Also updates pattern

  SetCBDelay( CBSetup.Delay);
  SetCBScHPhase( CBSetup.ScHPhase);
  SetCBEmbAudioSignal( CBSetup.EmbAudioSignal);

  return( OK);
}

/**************************************************************************/
/* SetCBPattern                                                     CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000618, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int SetCBPattern( UI pattern)
{
  int system = CBSetup.System;

  if ( ValidateCBPattern( system , &pattern) == FAIL)
    return( FAIL);

  Settings.ActivePreset = 0;

  Settings.CBSetup.Pattern = CBSetup.Pattern = pattern;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));

  cb_vars.CBX.Blanking = TRUE;
  test_vars.CBX.Blanking = TRUE;

  cb_vars.CBX.Status &= ~S5S4S3S2;                         // Blank line immediately
  test_vars.CBX.Status &= ~S5S4S3S2;                         // Blank line immediately
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif

  switch ( pattern)
  {
    case SMPTECBar:
      SRAMVideoLoad( dSMPTECBar, SRAMBank1);
      SRAMVideoLoad( dBlueBars, SRAMBank2);
      SRAMVideoLoad( dIWhiteQ, SRAMBank3);

//KS. Emulates a 8 line/pixel text field ! 
//Must be setup no matter whitch pattern is selected. 
//Bank 5 -12 Can be initialised with the text lines at power 
//up and when a new text line is entered by the user(via the front or the PC)  

      SRAMVideoLoad( dWindow100, SRAMBank5);
      SRAMVideoLoad( dWindow20, SRAMBank6);
      SRAMVideoLoad( dWindow15, SRAMBank7);
      SRAMVideoLoad( dWindow10, SRAMBank8);
      SRAMVideoLoad( dWindow10, SRAMBank9);
      SRAMVideoLoad( dWindow15, SRAMBank10);
      SRAMVideoLoad( dWindow20, SRAMBank11);
      SRAMVideoLoad( dWindow100, SRAMBank12);

      cb_vars.CBX.newTablePtr = &SMPTECBarTable[0];
      test_vars.CBX.newTablePtr = &TEST_SMPTECBarTable[0];
      break;

    case EBUCBar:
      SRAMVideoLoad( dEBUCBar_625, SRAMBank1);

      cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      break;

    case FCCCBar:
      SRAMVideoLoad( dFCC_CBar_525, SRAMBank1);

      cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      break;

    case EBUCBarITU:
      SRAMVideoLoad( dEBUCBar_8bit_625, SRAMBank1);

      if ( system < NTSC_US)
        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      else
        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      break;

    case CBar100:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dCBar100_625, SRAMBank1);

        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dFCC_CBar100_525, SRAMBank1);

        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case CBarGrey75:
      SRAMVideoLoad( dCBar75_STD_625, SRAMBank1);
      SRAMVideoLoad( dGrey75_625, SRAMBank2);
  
      cb_vars.CBX.newTablePtr = &PALSplitFieldTable[0];
      break;

    case CBarRed75:
      SRAMVideoLoad( dEBUCBar_625, SRAMBank1);
      SRAMVideoLoad( dRed75_625, SRAMBank2);

      cb_vars.CBX.newTablePtr = &PALSplitFieldTable[0];
      break;
      
    case Red75:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dRed75_625, SRAMBank1);

        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dRed75_525, SRAMBank1);

        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Multiburst:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dChroma_Multiburst, SRAMBank1);

        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dFCC_Multiburst_525, SRAMBank1);

        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case CCIR18:
      SRAMVideoLoad( dCCIR18_625, SRAMBank1);

      cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      break;

    case Window10:
      SRAMVideoLoad( dWindow50Marker, SRAMBank1);
      SRAMVideoLoad( dMarkerLine, SRAMBank2);
      SRAMVideoLoad( dMarker10, SRAMBank3);
      SRAMVideoLoad( dWindow10, SRAMBank4);

      if ( system < NTSC_US)
        cb_vars.CBX.newTablePtr = &PALWindowTable[0];
      else
        cb_vars.CBX.newTablePtr = &NTSCWindowTable[0];
      break;

    case Window15:
      SRAMVideoLoad( dWindow50Marker, SRAMBank1);
      SRAMVideoLoad( dMarkerLine, SRAMBank2);
      SRAMVideoLoad( dMarker15, SRAMBank3);
      SRAMVideoLoad( dWindow15, SRAMBank4);

      if ( system < NTSC_US)                        
        cb_vars.CBX.newTablePtr = &PALWindowTable[0];
      else
        cb_vars.CBX.newTablePtr = &NTSCWindowTable[0];
      break;

    case Window20:
      SRAMVideoLoad( dWindow50Marker, SRAMBank1);
      SRAMVideoLoad( dMarkerLine, SRAMBank2);
      SRAMVideoLoad( dMarker20, SRAMBank3);
      SRAMVideoLoad( dWindow20, SRAMBank4);

      if ( system < NTSC_US)
        cb_vars.CBX.newTablePtr = &PALWindowTable[0];
      else
        cb_vars.CBX.newTablePtr = &NTSCWindowTable[0];
      break;

    case Window100:
      SRAMVideoLoad( dWindow50Marker, SRAMBank1);
      SRAMVideoLoad( dMarkerLine, SRAMBank2);
      SRAMVideoLoad( dMarker100, SRAMBank3);
      SRAMVideoLoad( dWindow100, SRAMBank4);

      if ( system < NTSC_US)
        cb_vars.CBX.newTablePtr = &PALWindowTable[0];
      else
        cb_vars.CBX.newTablePtr = &NTSCWindowTable[0];
      break;

    case BlWh15kHz:
      if ( system < NTSC_US)                        
      {
        SRAMVideoLoad( dkHz15BlWh_625, SRAMBank1);

        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dkHz15BlWh_525, SRAMBank1);

        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case White100:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dWhiteLine_625, SRAMBank1);

        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dWhiteLine_525, SRAMBank1);

        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Black:
      if ( system < NTSC_US)
        cb_vars.CBX.newTablePtr = &PALBlackTable[0];
      else
        cb_vars.CBX.newTablePtr = &NTSCBlackTable[0];
      break;

    case CheckField:
      SRAMVideoLoad( dSDI_test_line1, SRAMBank1);
      SRAMVideoLoad( dSDI_test_line2, SRAMBank2);
      SRAMVideoLoad( dSDI_test_line3, SRAMBank3);

      if ( system < NTSC_US)
        cb_vars.CBX.newTablePtr = &PALCheckfieldTable[0];
      else
        cb_vars.CBX.newTablePtr = &NTSCCheckfieldTable[0];
      break;

    case DigitalGrey:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dDigitalGrey_625, SRAMBank1);

        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dDigitalGrey_525, SRAMBank1);

        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Stair5:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dGreyScale5_625, SRAMBank1);

        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dGreyScale5_525, SRAMBank1);

        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Stair10:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dGreyScale10_625, SRAMBank1);

        cb_vars.CBX.newTablePtr = &PALFullFieldTable[0];
      }
      else
      {
        SRAMVideoLoad( dGreyScale10_525, SRAMBank1);

        cb_vars.CBX.newTablePtr = &NTSCFullFieldTable[0];
      }
      break;

    case Crosshatch:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dCrossHatch19_625, SRAMBank1);
        SRAMVideoLoad( dWhiteLine_625, SRAMBank2);

        cb_vars.CBX.newTablePtr = &PALCrosshatchTable[0];
      }
      else
      {
        SRAMVideoLoad( dCrossHatch17_525, SRAMBank1);
        SRAMVideoLoad( dWhiteLine_525, SRAMBank2);

        cb_vars.CBX.newTablePtr = &NTSCCrosshatchTable[0];
      }
      break;

    case PLUGE:
      if ( system < NTSC_US)
      {
        SRAMVideoLoad( dPlugeWhite_625, SRAMBank1);
        SRAMVideoLoad( dPlugeGreyC_625, SRAMBank2);
        SRAMVideoLoad( dPlugeGreyD_625, SRAMBank3);
        SRAMVideoLoad( dPlugeGreyE_625, SRAMBank4);

        cb_vars.CBX.newTablePtr = &PALPLUGETable[0];
      }
      else
      {
        SRAMVideoLoad( dPlugeWhite_525, SRAMBank1);
        SRAMVideoLoad( dPlugeGreyC_525, SRAMBank2);
        SRAMVideoLoad( dPlugeGreyD_525, SRAMBank3);
        SRAMVideoLoad( dPlugeGreyE_525, SRAMBank4);

        cb_vars.CBX.newTablePtr = &NTSCPLUGETable[0];
      }
      break;
  }

  test_vars.CBX.NewPattern = TRUE;
  cb_vars.CBX.NewPattern = TRUE;

  return( OK);
}

/**************************************************************************/
/* SetCBSystem                                     CBAR.C  */
/*                                                  */
/* Author:   Kim Engedahl, DEV, 000331                          */
/* Revised:   000705, KEn, DEV                                 */
/*                                                  */
/* Function:                                           */
/* Remarks:                                            */
/* Returns:                                            */
/* Updates:                                              */
/**************************************************************************/
int SetCBSystem( UC system)
{
  Settings.ActivePreset = 0;

  Settings.CBSetup.System = CBSetup.System = system;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));

  if ( system < NTSC_US)
  {
    test_vars.CBX.Status &= ~SYSSEL;
    cb_vars.CBX.Status &= ~SYSSEL;
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif

    WriteCBGain( Calibration.CBCalibration.GainG);
    WriteCBChromaGain( Calibration.CBCalibration.ChromaGainG);
  }
  else
  {
    test_vars.CBX.Status |= SYSSEL;
    cb_vars.CBX.Status |= SYSSEL;
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif

    WriteCBGain( Calibration.CBCalibration.GainM);
    WriteCBChromaGain( Calibration.CBCalibration.ChromaGainM);
  }


  tvp6k_system( system);  // Initialise the composite output

                          // Validate pattern and change if invalid in new system                                    
  ValidateCBPattern( system , &CBSetup.Pattern);

                          // This one initialises TablePtr;
  SetCBPattern( CBSetup.Pattern);

  SetCBScHPhase( CBSetup.ScHPhase);

  LEDSet( CBPALLED, system < NTSC_US);
  LEDSet( CBNTSCLED, system >= NTSC_US);

  return( OK);
}

/**************************************************************************/
/* SetCBDelay                                                       CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   001009, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:   CB Delay resolution is 37ns                                  */
/* Returns:                                                                */
/* Updates:                                                                */
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

  if ( CBSetup.System < NTSC_US)
    {
    tmpOffset += Calibration.CBCalibration.PhaseG;

    tmpOffset %= 4320000L;            // 4*625*1728

    LineOffset = 0xFFFF - ( tmpOffset / 1728L);        
    PhaseOffset = 1727 - ( tmpOffset % 1728L);
    }
  else 
    {
    tmpOffset += Calibration.CBCalibration.PhaseM;

    tmpOffset %= 1801800L;            // 2*525*1716

    LineOffset = 0xFFFF - ( tmpOffset / 1716L);        
    PhaseOffset = 1715 - ( tmpOffset % 1716L);

    if ( PhaseOffset > 1687)
      PhaseOffset += 12;
    }

  if ( PhaseOffset >= 1443)
    {
    test_vars.CBX.Status |= PHASEREGSEL;        // Set Phase Register Select
    cb_vars.CBX.Status |= PHASEREGSEL;        // Set Phase Register Select
    }
  else   
    { 
    test_vars.CBX.Status &= ~PHASEREGSEL;        // Clear Phase Register Select
    cb_vars.CBX.Status &= ~PHASEREGSEL;        // Clear Phase Register Select
    }
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif
  HOffsetPort = PhaseOffset;

  return( OK);
}

/**************************************************************************/
/* SetCBScHPhase                                                   CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000618, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:   Note that the resolution is different from BlackBurst, (1.4).*/
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int SetCBScHPhase( int schphase)
{
  long tmp;
  UC buffer[2];

  Settings.ActivePreset = 0;

  Settings.CBSetup.ScHPhase = CBSetup.ScHPhase = schphase;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));
  
  if ( CBSetup.System < NTSC_US)
    tmp = (long) Calibration.CBCalibration.ScHPhaseG;
  else 
    tmp = (long) Calibration.CBCalibration.ScHPhaseM;

  tmp += (long) schphase;

  if ( tmp < 0)                      // -1 deg equ. 359 deg a.s.f.
    tmp += 360;

  tmp *= 256;
  tmp /= 360;                        // tmp = ( ScHPhase*256)/360;

  buffer[0] = C_PHASE_address;
  buffer[1] = tmp & 0xFF;
  
  return( TransmitIIC( TVP6K_IIC_address, 2, buffer, IIC_PTV));
}

/**************************************************************************/
/* SetCBEmbAudioSignal                                             CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000331                                    */
/* Revised:   000618, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int SetCBEmbAudioSignal( UC signal)
{
  Settings.ActivePreset = 0;

  Settings.CBSetup.EmbAudioSignal = CBSetup.EmbAudioSignal = signal;
  NV_Store( &Settings, SettingsPtr, sizeof( Settings));

  if ( signal == EmbAudioOff)
    {
    test_vars.CBX.SoundEnable = FALSE;
    cb_vars.CBX.SoundEnable = FALSE;
    }
  else
    {
    test_vars.CBX.SoundEnable = TRUE;
    cb_vars.CBX.SoundEnable = TRUE;

    test_vars.CBX.Blanking = TRUE;                  // Blank line while loading SRAM
    cb_vars.CBX.Blanking = TRUE;                  // Blank line while loading SRAM

    test_vars.CBX.Status &= ~S5S4S3S2;             // Blank the line immediately
    cb_vars.CBX.Status &= ~S5S4S3S2;             // Blank the line immediately
#ifdef CLASSIC    
  StatusPort = cb_vars.CBX.Status;
#else  
  StatusPort = test_vars.CBX.Status;
#endif

    SRAMSoundLoad( signal);           // Download sound information

    test_vars.CBX.Blanking = FALSE;
    cb_vars.CBX.Blanking = FALSE;
    }

  return( OK);
}

/**************************************************************************/
/* ValidateCBPattern                                                CBAR.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000405                                    */
/* Revised:   000603, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                 */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
int ValidateCBPattern( UC system, UI *pattern)
{
  if ( system < NTSC_US)
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

