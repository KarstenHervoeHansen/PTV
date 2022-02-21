/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:    TABLES.C                                    */
/*  Author:    Kim Engedahl, DEV        TPGAudioDual                      */
/*  Org. date:  980119                                    */
/*  Rev. date:  990623, KEn, DEV                              */
/*  Status:    Version 1.5                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*    code UC* FindPatternTable( UC HWtype);                      */
/*    code UC* FindAudioLevelTable( UC HWtype);                    */
/*                                                  */
/*    void AnlValidateSetting( UC HWtype, UC ndx)                  */
/*    UC AnlValidatePattern( UC HWtype, UC ndx, UC system, UC pattern)    */
/*    UC AnlValidateGroupPattern( UC HWtype, UC ndx, UC system, \        */
/*                                   UC group, UC pattern)  */
/*    UC AnlNextGroupPattern( UC HWtype, UC ndx, UC system, \          */
/*                                   UC group, UC pattern)  */
/*    UC AnlFindGroup( UC pattern)                            */
/*    UC AnlFindPatternType( UC pattern)                        */
/*    UC AnlTPGAttrib( UC ndx, UC pattern)                      */
/*    UC AnlValidateAttrib( UC HWtype, UC ndx, UC system, UC pattern,\    */
/*                                           UC attrib)  */
/*                                                  */
/*    void SDIValidateSetting( UC HWtype, UC ndx)                  */
/*    UC SDIValidatePattern( UC HWtype, UC ndx, UC system, UC pattern)    */
/*    UC SDIValidateGroupPattern( UC HWtype, UC ndx, UC system, \        */
/*                                   UC group, UC pattern)  */
/*    UC SDINextGroupPattern( UC HWtype, UC ndx, UC system, \          */
/*                                   UC group, UC pattern)  */
/*    UC SDIFindGroup( UC pattern);                            */
/*    UC SDIFindPatternType( UC pattern)                        */
/*    UC SDITPGAttrib( UC ndx, UC pattern)                      */
/*    UC SDIValidateAttrib( UC HWtype, UC ndx, UC system, UC pattern,\    */
/*                                           UC attrib)  */
/*                                                  */
/*  Comment:                                            */
/*  All the tables in this file contains the values send to all the units  */
/*  and options. PLEASE handle with care!                        */
/* ALL tables should correspond with actual hardware AND enumerated types  */
/* as defined in the file TABLES.H                            */
/*                                                  */
/*  Changes:                                            */
/* 990623: Enabled Window 10% & 15 kHz Bl/Wh for PT8632/33/39          */
/* 990423: Added Window 10% & 15 kHz Bl/Wh in both SDI & Analog.        */
/* 990305: Added SDIValidateAttrib() and AnlValidateAttrib functions.    */
/* 980925: PT8601 SW ver. 2.5 & PT8603 SW ver. 3.5 now supports 5- & 10-  */
/*         step staircase.                                */
/* 980917: Added SDI: CCIR 17,18,330,331. AnlTPGAttrib() & AnlFindPattern-  */
/*        Type() updated to handle new complex signals.            */
/* 980916: Added analog signals: FuBK-4:3/16:9, Philips16:9, Crosshatch16:9*/
/*         WhiteCircle-16:9.                                */
/* 980616: Released as 4008 002 06812                          */
/* 980524: Major reorganisation. Added support for custopmized patterns    */
/* 980514: Released as 4008 002 06811                          */
/***************************************************************************/

/***************************************************************************/
/* Include files:                                        */

#include <xa.h>

#include <stdlib.h>

#include "mas.h"
#include "string.h"
#include "tables.h"

/***************************************************************************/
/* PT8601 Analog Test Pattern Generator. Conversion table      TABLES.C  */
/***************************************************************************/
code UC PT8601PatternTable[] = {
  0,         // AnlSMPTECBar
  1,         // AnlEBUCBar
  NoPattern,    // AnlFCCBar
  2,         // AnlCBar100
  3,         // AnlCBarGrey75
  4,         // AnlCBarRed75
  13,         // AnlRed75,

  12,         // AnlMultiburst
  NoPattern,    // AnlLumSweep
  NoPattern,     // AnlMultipulse
  NoPattern,     // AnlSinxx
  NoPattern,     // AnlCCIR18
  NoPattern,     // AnlNTC7Comb
  NoPattern,     // AnlFCCMultiburst,

  NoPattern,    // AnlWindow10
  5,         // AnlWindow15
  6,         // AnlWindow20
  7,         // AnlWindow100
  NoPattern,     // AnlBlWh15kHz
  NoPattern,     // AnlGrey50
  NoPattern,     // AnlWhite100
  14,         // AnlBlackBurst

  NoPattern,     // AnlFieldSquareWave
  NoPattern,     // AnlBlWh01Hz
  40,        // Customized 1
  41,        // Customized 2
  42,        // Customized 3
  43,        // Customized 4
  44,        // Customized 5
  45,        // Customized 6
  46,        // Customized 7
  47,        // Customized 8

  11,         // AnlShallowRamp
  NoPattern,     // AnlRamp
  NoPattern,     // AnlModulatedRamp
  30,         // AnlStair5
  NoPattern,     // AnlModulatedStair5
  32,         // AnlStair10
  NoPattern,     // AnlPulseBar
  NoPattern,     // AnlCCIR17
  NoPattern,     // AnlCCIR330
  NoPattern,     // AnlCCIR331
  NoPattern,     // AnlFCCComposite
  NoPattern,     // AnlNTC7Composite

  NoPattern,    // AnlPhilips4x3
  NoPattern,    // AnlPhilips16x9
  NoPattern,    // AnlFuBK4x3
  NoPattern,    // AnlFuBK16x9
  8,         // AnlCrosshatch
  NoPattern,    // AnlCrosshatch16x9
  NoPattern,    // AnlWhiteCircle4x3
  NoPattern,    // AnlWhiteCircle16x9
  9,         // AnlPLUGE
  10,         // AnlSafeArea
  NoPattern,    // AnlkHz250
  NoPattern,     // AnlVMT01
};

/***************************************************************************/
/* PT8602 SDI Test Pattern Generator. Conversion table        TABLES.C  */
/***************************************************************************/
code UC PT8602PatternTable[] = {
  0,          // SDISMPTECBar
  1,          // SDIEBUCBar
  NoPattern,    // SDIFCCCBar
  2,          // SDIEBUCBarITU
  3,          // SDICBar100
  4,          // SDICBarGrey75
  5,          // SDICBarRed75
  NoPattern,    // SDIRed75

  NoPattern,    // SDIMultiburst
  NoPattern,    // SDILumSweep
  NoPattern,    // SDIYCrCbSweep
  NoPattern,    // SDIMultipulse
  NoPattern,    // SDISinxx
  NoPattern,    // SDICCIR18

  NoPattern,    // SDIWindow10
  NoPattern,    // SDIWindow15
  NoPattern,    // SDIWindow20
  NoPattern,    // SDIWindow100
  NoPattern,    // SDIBlWh15kHz
  NoPattern,    // SDIWhite100
  9,          // SDIBlack

  6,          // SDICheckField
  8,          // SDITimingTest
  NoPattern,    // SDIFieldDelayTest
  NoPattern,    // SDIBowTie
  NoPattern,    // SDIAnalogBlanking
  NoPattern,    // SDIDigitalGrey
  NoPattern,    // SDIFieldSquareWave
  NoPattern,    // SDIBlWh01Hz
  NoPattern,    // SDIEOL
  NoPattern,    // SDIWhiteEOL
  NoPattern,    // SDIBlueEOL
  NoPattern,    // SDIRedEOL
  NoPattern,    // SDIYellowEOL
  NoPattern,    // SDICyanEOL
  55,        // Customized 1
  56,        // Customized 2
  57,        // Customized 3
  58,        // Customized 4
  59,        // Customized 5
  60,        // Customized 6
  61,        // Customized 7
  62,        // Customized 8

  7,          // SDIShallowRamp
  NoPattern,    // SDIRamp
  NoPattern,    // SDILimitRamp
  NoPattern,    // SDIValidRamp
  NoPattern,    // SDIStair5
  NoPattern,    // SDIModulatedStair5
  NoPattern,    // SDIStair10
  NoPattern,    // SDIPulseBar
  NoPattern,    // SDICCIR17
  NoPattern,    // SDICCIR330
  NoPattern,    // SDICCIR331
  NoPattern,    // SDIYellowGreyRamp
  NoPattern,    // SDIGreyBlueRamp
  NoPattern,    // SDICyanGreyRamp
  NoPattern,    // SDIGreyRedRamp
  NoPattern,    // SDICbYCrRamp

  NoPattern,    // SDIPhilips4x3
  NoPattern,    // SDIPhilips16x9
  NoPattern,    // SDIFuBK4x3
  NoPattern,    // SDIFuBK16x9
  NoPattern,    // SDICrosshatch
  NoPattern,    // SDIPLUGE
  NoPattern,    // SDISafeArea
  NoPattern,    // SDIVMT01
};

code UC PT8602AudioLevelTable[] = {
  0,          // TPGAudioSilence
  1,          // TPGAudioLevel0dB
  2,          // TPGAudioLevel9dB
  3,          // TPGAudioLevel12dB
  4,          // TPGAudioLevel14dB
  NoAudioLevel,  // TPGAudioLevel15dB
  5,          // TPGAudioLevel16dB
  6,          // TPGAudioLevel18dB
  7          // TPGAudioLevel20dB
};

/***************************************************************************/
/* PT8603 SDI Test Pattern Generator. Conversion table        TABLES.C  */
/***************************************************************************/
code UC PT8603PatternTable[] = {
  0,          // SDISMPTECBar
  1,          // SDIEBUCBar
  NoPattern,    // SDIFCCCBar
  2,          // SDIEBUCBarITU
  3,          // SDICBar100
  4,          // SDICBarGrey75
  5,          // SDICBarRed75
  16,        // SDIRed75

  15,        // SDIMultiburst
  NoPattern,    // SDILumSweep
  NoPattern,    // SDIYCrCbSweep
  NoPattern,    // SDIMultipulse
  NoPattern,    // SDISinxx
  NoPattern,    // SDICCIR18

  NoPattern,    // SDIWindow10
  10,        // SDIWindow15
  11,        // SDIWindow20
  12,        // SDIWindow100
  NoPattern,    // SDIBlWh15kHz
  NoPattern,    // SDIWhite100
  9,          // SDIBlack

  6,          // SDICheckField
  8,          // SDITimingTest
  NoPattern,    // SDIFieldDelayTest
  NoPattern,    // SDIBowTie
  NoPattern,    // SDIAnalogBlanking
  NoPattern,    // SDIDigitalGrey
  NoPattern,    // SDIFieldSquareWave
  NoPattern,    // SDIBlWh01Hz
  NoPattern,    // SDIEOL
  NoPattern,    // SDIWhiteEOL
  NoPattern,    // SDIBlueEOL
  NoPattern,    // SDIRedEOL
  NoPattern,    // SDIYellowEOL
  NoPattern,    // SDICyanEOL
  55,        // Customized 1
  56,        // Customized 2
  57,        // Customized 3
  58,        // Customized 4
  59,        // Customized 5
  60,        // Customized 6
  61,        // Customized 7
  62,        // Customized 8

  7,          // SDIShallowRamp
  NoPattern,    // SDIRamp
  NoPattern,    // SDILimitRamp
  NoPattern,    // SDIValidRamp
  26,        // SDIStair5
  NoPattern,    // SDIModulatedStair5
  27,        // SDIStair10
  NoPattern,    // SDIPulseBar
  NoPattern,    // SDICCIR17
  NoPattern,    // SDICCIR330
  NoPattern,    // SDICCIR331
  NoPattern,    // SDIYellowGreyRamp
  NoPattern,    // SDIGreyBlueRamp
  NoPattern,    // SDICyanGreyRamp
  NoPattern,    // SDIGreyRedRamp
  NoPattern,    // SDICbYCrRamp

  NoPattern,    // SDIPhilips4x3
  NoPattern,    // SDIPhilips16x9
  NoPattern,    // SDIFuBK4x3
  NoPattern,    // SDIFuBK16x9
  13,        // SDICrosshatch
  14,        // SDIPLUGE
  NoPattern,    // SDISafeArea
  NoPattern,    // SDIVMT01
};

code UC PT8603AudioLevelTable[] = {
  0,          // TPGAudioSilence
  1,          // TPGAudioLevel0dB
  2,          // TPGAudioLevel9dB
  3,          // TPGAudioLevel12dB
  4,          // TPGAudioLevel14dB
  NoAudioLevel,  // TPGAudioLevel15dB
  5,          // TPGAudioLevel16dB
  6,          // TPGAudioLevel18dB
  7          // TPGAudioLevel20dB
};

/***************************************************************************/
/* PT8609 SDI Black/Colorbar Generator. Conversion table        TABLES.C  */
/***************************************************************************/
code UC PT8609PatternTable[] = {
  3,          // SDISMPTECBar
  1,          // SDIEBUCBar
  NoPattern,    // SDIFCCCBar
  NoPattern,    // SDIEBUCBarITU
  2,          // SDICBar100
  NoPattern,    // SDICBarGrey75
  NoPattern,    // SDICBarRed75
  NoPattern,    // SDIRed75

  NoPattern,    // SDIMultiburst
  NoPattern,    // SDILumSweep
  NoPattern,    // SDIYCrCbSweep
  NoPattern,    // SDIMultipulse
  NoPattern,    // SDISinxx
  NoPattern,    // SDICCIR18

  NoPattern,    // SDIWindow10
  NoPattern,    // SDIWindow15
  NoPattern,    // SDIWindow20
  NoPattern,    // SDIWindow100
  NoPattern,    // SDIBlWh15kHz
  NoPattern,    // SDIWhite100
  0,          // SDIBlack

  NoPattern,    // SDICheckField
  NoPattern,    // SDITimingTest
  NoPattern,    // SDIFieldDelayTest
  NoPattern,    // SDIBowTie
  NoPattern,    // SDIAnalogBlanking
  NoPattern,    // SDIDigitalGrey
  NoPattern,    // SDIFieldSquareWave
  NoPattern,    // SDIBlWh01Hz
  NoPattern,    // SDIEOL
  NoPattern,    // SDIWhiteEOL
  NoPattern,    // SDIBlueEOL
  NoPattern,    // SDIRedEOL
  NoPattern,    // SDIYellowEOL
  NoPattern,    // SDICyanEOL
  NoPattern,    // Customized 1
  NoPattern,    // Customized 2
  NoPattern,    // Customized 3
  NoPattern,    // Customized 4
  NoPattern,    // Customized 5
  NoPattern,    // Customized 6
  NoPattern,    // Customized 7
  NoPattern,    // Customized 8

  NoPattern,    // SDIShallowRamp
  NoPattern,    // SDIRamp
  NoPattern,    // SDILimitRamp
  NoPattern,    // SDIValidRamp
  NoPattern,    // SDIStair5
  NoPattern,    // SDIModulatedStair5
  NoPattern,    // SDIStair10
  NoPattern,    // SDIPulseBar
  NoPattern,    // SDICCIR17
  NoPattern,    // SDICCIR330
  NoPattern,    // SDICCIR331
  NoPattern,    // SDIYellowGreyRamp
  NoPattern,    // SDIGreyBlueRamp
  NoPattern,    // SDICyanGreyRamp
  NoPattern,    // SDIGreyRedRamp
  NoPattern,    // SDICbYCrRamp

  NoPattern,    // SDIPhilips4x3
  NoPattern,    // SDIPhilips16x9
  NoPattern,    // SDIFuBK4x3
  NoPattern,    // SDIFuBK16x9
  NoPattern,    // SDICrosshatch
  NoPattern,    // SDIPLUGE
  NoPattern,    // SDISafeArea
  NoPattern,    // SDIVMT01
};

/***************************************************************************/
/* PT8631 Analog Test Pattern Generator. Conversion table      TABLES.C  */
/***************************************************************************/
code UC PT8631PatternTable[] = {
  0,         // AnlSMPTECBar
  1,         // AnlEBUCBar
  21,         // AnlFCCBar
  2,         // AnlCBar100
  3,         // AnlCBarGrey75
  4,         // AnlCBarRed75
  13,         // AnlRed75,

  NoPattern,     // AnlMultiburst
  20,         // AnlLumSweep
  18,         // AnlMultipulse
  19,         // AnlSinxx
  12,         // AnlCCIR18
  22,         // AnlNTC7Comb
  23,         // AnlFCCMultiburst,

  54,        // AnlWindow10
  5,         // AnlWindow15
  6,         // AnlWindow20
  7,         // AnlWindow100
  55,         // AnlBlWh15kHz
  24,         // AnlGrey50
  25,         // AnlWhite100
  14,         // AnlBlackBurst

  26,         // AnlFieldSquareWave
  27,         // AnlBlWh01Hz
  40,        // Customized 1
  41,        // Customized 2
  42,        // Customized 3
  43,        // Customized 4
  44,        // Customized 5
  45,        // Customized 6
  46,        // Customized 7
  47,        // Customized 8

  NoPattern,    // AnlShallowRamp
  28,         // AnlRamp
  29,         // AnlModulatedRamp
  30,         // AnlStair5
  31,         // AnlModulatedStair5
  32,         // AnlStair10
  33,         // AnlPulseBar
  17,         // AnlCCIR17
  34,         // AnlCCIR330
  35,         // AnlCCIR331
  36,         // AnlFCCComposite
  37,         // AnlNTC7Composite

  15,        // AnlPhilips4x3
  50,        // AnlPhilips16x9
  48,        // AnlFuBK4x3
  49,        // AnlFuBK16x9
  8,         // AnlCrosshatch
  51,        // AnlCrosshatch16x9
  52,        // AnlWhiteCircle4x3
  53,        // AnlWhiteCircle16x9
  9,         // AnlPLUGE
  10,         // AnlSafeArea
  38,         // AnlkHz250
  39,         // AnlVMT01
};

/***************************************************************************/
/* PT8632 SDI Test Pattern Generator. Conversion table        TABLES.C  */
/***************************************************************************/
code UC PT8632PatternTable[] = {
  0,          // SDISMPTECBar
  1,          // SDIEBUCBar
  52,        // SDIFCCCBar
  2,          // SDIEBUCBarITU
  3,          // SDICBar100
  4,          // SDICBarGrey75
  5,          // SDICBarRed75
  16,        // SDIRed75

  15,        // SDIMultiburst
  28,        // SDILumSweep
  NoPattern,    // SDIYCrCbSweep
  21,        // SDIMultipulse
  NoPattern,    // SDISinxx
  NoPattern,    // SDICCIR18

  66,        // SDIWindow10
  10,        // SDIWindow15
  11,        // SDIWindow20
  12,        // SDIWindow100
  67,        // SDIBlWh15kHz
  NoPattern,    // SDIWhite100
  9,          // SDIBlack

  6,          // SDICheckField
  8,          // SDITimingTest
  24,        // SDIFieldDelayTest
  31,        // SDIBowTie
  20,        // SDIAnalogBlanking
  23,        // SDIDigitalGrey
  53,        // SDIFieldSquareWave
  NoPattern,    // SDIBlWh01Hz
  NoPattern,    // SDIEOL
  NoPattern,    // SDIWhiteEOL
  NoPattern,    // SDIBlueEOL
  NoPattern,    // SDIRedEOL
  NoPattern,    // SDIYellowEOL
  NoPattern,    // SDICyanEOL
  55,        // Customized 1
  56,        // Customized 2
  57,        // Customized 3
  58,        // Customized 4
  59,        // Customized 5
  60,        // Customized 6
  61,        // Customized 7
  62,        // Customized 8

  7,          // SDIShallowRamp
  39,        // SDIRamp
  40,        // SDILimitRamp
  25,        // SDIValidRamp
  26,        // SDIStair5
  54,        // SDIModulatedStair5
  NoPattern,    // SDIStair10
  41,        // SDIPulseBar
  NoPattern,    // SDICCIR17
  NoPattern,    // SDICCIR330
  NoPattern,    // SDICCIR331
  NoPattern,    // SDIYellowGreyRamp
  NoPattern,    // SDIGreyBlueRamp
  NoPattern,    // SDICyanGreyRamp
  NoPattern,    // SDIGreyRedRamp
  NoPattern,    // SDICbYCrRamp

  17,        // SDIPhilips4x3
  NoPattern,    // SDIPhilips16x9
  48,        // SDIFuBK4x3
  NoPattern,    // SDIFuBK16x9
  13,        // SDICrosshatch
  14,        // SDIPLUGE
  NoPattern,    // SDISafeArea
  NoPattern,    // SDIVMT01
};

code UC PT8632AudioLevelTable[] = {
  0,          // TPGAudioSilence
  1,          // TPGAudioLevel0dB
  2,          // TPGAudioLevel9dB
  3,          // TPGAudioLevel12dB
  NoAudioLevel,  // TPGAudioLevel14dB
  4,          // TPGAudioLevel15dB
  5,          // TPGAudioLevel16dB
  6,          // TPGAudioLevel18dB
  7          // TPGAudioLevel20dB
};

/***************************************************************************/
/* PT8633 SDI Test Pattern Generator. Conversion table        TABLES.C  */
/***************************************************************************/
code UC PT8633PatternTable[] = {
  0,          // SDISMPTECBar
  1,          // SDIEBUCBar
  52,        // SDIFCCCBar
  2,          // SDIEBUCBarITU
  3,          // SDICBar100
  4,          // SDICBarGrey75
  5,          // SDICBarRed75
  16,        // SDIRed75

  15,        // SDIMultiburst
  28,        // SDILumSweep
  29,        // SDIYCrCbSweep
  21,        // SDIMultipulse
  22,         // SDISinxx
  63,        // SDICCIR18

  66,         // SDIWindow10
  10,         // SDIWindow15
  11,         // SDIWindow20
  12,         // SDIWindow100
  67,        // SDIBlWh15kHz
  30,         // SDIWhite100
  9,           // SDIBlack

  6,          // SDICheckField
  8,          // SDITimingTest
  24,        // SDIFieldDelayTest
  31,         // SDIBowTie
  20,          // SDIAnalogBlanking
  23,          // SDIDigitalGrey
  53,        // SDIFieldSquareWave
  32,         // SDIBlWh01Hz
  33,         // SDIEOL
  34,        // SDIWhiteEOL
  35,        // SDIBlueEOL
  36,        // SDIRedEOL
  37,        // SDIYellowEOL
  38,        // SDICyanEOL
  55,        // Customized 1
  56,        // Customized 2
  57,        // Customized 3
  58,        // Customized 4
  59,        // Customized 5
  60,        // Customized 6
  61,        // Customized 7
  62,        // Customized 8

  7,          // SDIShallowRamp
  39,        // SDIRamp
  40,        // SDILimitRamp
  25,        // SDIValidRamp
  26,        // SDIStair5
  54,        // SDIModulatedStair5
  27,        // SDIStair10
  41,        // SDIPulseBar
  19,        // SDICCIR17
  64,        // SDICCIR330
  65,        // SDICCIR331
  42,        // SDIYellowGreyRamp
  43,        // SDIGreyBlueRamp
  44,        // SDICyanGreyRamp
  45,        // SDIGreyRedRamp
  46,        // SDICbYCrRamp

  17,        // SDIPhilips4x3
  47,        // SDIPhilips16x9
  48,        // SDIFuBK4x3
  49,        // SDIFuBK16x9
  13,        // SDICrosshatch
  14,        // SDIPLUGE
  50,        // SDISafeArea
  51,        // SDIVMT01
};

code UC PT8633AudioLevelTable[] = {
  0,          // TPGAudioSilence
  1,          // TPGAudioLevel0dB
  2,          // TPGAudioLevel9dB
  3,          // TPGAudioLevel12dB
  NoAudioLevel,  // TPGAudioLevel14dB
  4,          // TPGAudioLevel15dB
  5,          // TPGAudioLevel16dB
  6,          // TPGAudioLevel18dB
  7          // TPGAudioLevel20dB
};

/***************************************************************************/
/* PT8639 SDI Test Signal Generator. Conversion table          TABLES.C  */
/***************************************************************************/
code UC PT8639PatternTable[] = {
  3,          // SDISMPTECBar
  1,          // SDIEBUCBar
  18,        // SDIFCCCBar
  14,        // SDIEBUCBarITU
  2,          // SDICBar100
  NoPattern,    // SDICBarGrey75
  5,          // SDICBarRed75
  13,        // SDIRed75

  12,        // SDIMultiburst
  NoPattern,    // SDILumSweep
  NoPattern,    // SDIYCrCbSweep
  NoPattern,    // SDIMultipulse
  NoPattern,    // SDISinxx
  NoPattern,    // SDICCIR18

  27,        // SDIWindow10
  7,          // SDIWindow15
  8,          // SDIWindow20
  9,          // SDIWindow100
  28,        // SDIBlWh15kHz
  NoPattern,    // SDIWhite100
  0,          // SDIBlack

  6,          // SDICheckField
  NoPattern,    // SDITimingTest
  NoPattern,    // SDIFieldDelayTest
  NoPattern,    // SDIBowTie
  NoPattern,    // SDIAnalogBlanking
  15,        // SDIDigitalGrey
  NoPattern,    // SDIFieldSquareWave
  NoPattern,    // SDIBlWh01Hz
  NoPattern,    // SDIEOL
  NoPattern,    // SDIWhiteEOL
  NoPattern,    // SDIBlueEOL
  NoPattern,    // SDIRedEOL
  NoPattern,    // SDIYellowEOL
  NoPattern,    // SDICyanEOL
  19,        // Customized 1
  20,        // Customized 2
  21,        // Customized 3
  22,        // Customized 4
  23,        // Customized 5
  24,        // Customized 6
  25,        // Customized 7
  26,        // Customized 8

  NoPattern,    // SDIShallowRamp
  NoPattern,    // SDIRamp
  NoPattern,    // SDILimitRamp
  NoPattern,    // SDIValidRamp
  16,        // SDIStair5
  NoPattern,    // SDIModulatedStair5
  NoPattern,    // SDIStair10
  NoPattern,    // SDIPulseBar
  NoPattern,    // SDICCIR17
  NoPattern,    // SDICCIR330
  NoPattern,    // SDICCIR331
  NoPattern,    // SDIYellowGreyRamp
  NoPattern,    // SDIGreyBlueRamp
  NoPattern,    // SDICyanGreyRamp
  NoPattern,    // SDIGreyRedRamp
  NoPattern,    // SDICbYCrRamp

  NoPattern,    // SDIPhilips4x3
  NoPattern,    // SDIPhilips16x9
  NoPattern,    // SDIFuBK4x3
  NoPattern,    // SDIFuBK16x9
  10,        // SDICrosshatch
  11,        // SDIPLUGE
  NoPattern,    // SDISafeArea
  NoPattern,    // SDIVMT01
};

/***************************************************************************/
/* SPG board                                    TABLES.C  */
/***************************************************************************/
code UC GenlockInputTable[14] =
  {
  0x40, 0x00,      // A
  0xC0, 0x00,      // B
  0xA0, 0x00,      // LOOP
  0x10, 0x00,      // SDI Genlock
  0x10, 0x80,      // GPS Genlock    NEW
  0x01, 0x00,      // Internal
  0x0F, 0x01       // Internal2
  };

/***************************************************************************/
/* SPG board                                    TABLES.C  */
/***************************************************************************/
code UC GenlockSystemTable[20] = {
  0x01, 8,      // BurstLockPAL
  0x00, 10,    // BurstLockNTSC
  0x01, 7,      // SyncLock625
  0x00, 11,    // SyncLock525
  0x11, 6,      // SDI625Lock
  0x10, 9,      // SDI525Lock
  0x04, 5,      // MHzLock358
  0x05, 4,      // MHzLock443
  0x05, 3,      // MHzLock5
  0x0D, 2      // MHzLock10
};

/***************************************************************************/
/*  AES/EBU Audio generator                            TABLES.C  */
/***************************************************************************/
code UI AESEBULevelTable[9] = {
  0x0007,      // AESEBUSilence
  0x0000,      // AESEBULevel0dB
  0x0001,      // AESEBULevel9dB
  0x0002,      // AESEBULevel12dB
  0x0003,      // AESEBULevel14dB
  0x0003,      // AESEBULevel15dB
  0x0004,      // AESEBULevel16dB
  0x0005,      // AESEBULevel18dB
  0x0006      // AESEBULevel20dB
};

/***************************************************************************/
/*  AES/EBU Audio generator                            TABLES.C  */
/***************************************************************************/
code UI AESEBUSignalTable[9] = {
  0,          // NOT USED
  0x0048,      // AESEBUStereo800Hz
  0x0000,      // AESEBUStereo1kHz
  0x0008,      // AESEBUStereoEBU1kHz
  0x0010,      // AESEBUStereoBBC1kHz
  0x0018,      // AESEBUMonoEBU1kHz
  0x0020,      // AESEBUMonoBBC1kHz
  0x0040,      // AESEBUDual
  0x0078      // AESEBUWordClock
};

/***************************************************************************/
/*  AES/EBU Audio generator                            TABLES.C  */
/***************************************************************************/
code UI AESEBUTimingTable[6] = {
  0x0000,      // AESEBUPAL
  0x0100,      // AESEBUNTSCPhase1
  0x0200,      // AESEBUNTSCPhase2
  0x0300,      // AESEBUNTSCPhase3
  0x0400,      // AESEBUNTSCPhase4
  0x0500      // AES_EBUNTSCPhase5
};

/***************************************************************************/
/*  PATTERN TEXT CHARACTER TABLE FOR PT860X                      TABLES.C  */
/***************************************************************************/
code UC StdTextCharSet[100] = {
// 0-24
//   ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

// 25-49
//                      , !, ", #, $, %, &, ', (, ), *, +, ,, -, ., /, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1,

// 50-74
//  2, 3, 4, 5, 6, 7, 8, 9, :, ;, <, =, >, ?, @, A, B, C, D, E, F, G, H, I, J,
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

// 75-99
// K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, [, \, ], ^, _, `, a, b, c,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
};


/* ,-,0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,_ */
/***************************************************************************/
/* PT8641 HDTPG. Conversion table          TABLES.C  */
/***************************************************************************/
#if 0
code UC PT8641AudioLevelTable[] = 
  {
  0,          // TPGAudioSilence
  1,          // TPGAudioLevel0dB
  2,          // TPGAudioLevel9dB
  3,          // TPGAudioLevel12dB
  NoAudioLevel,  // TPGAudioLevel14dB 255
  4,          // TPGAudioLevel15dB
  5,          // TPGAudioLevel16dB
  6,          // TPGAudioLevel18dB
  7           // TPGAudioLevel20dB
  };
#endif
/***************************************************************************/
/*  PATTERN TEXT CHARACTER TABLE FOR PT863X                TABLES.C  */
/***************************************************************************/
code UC ExtTextCharSet[236] = {
// 0-24
//   ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

// 25-49
//                        , !, ", #, $, %, &, ', (, ), *, +, ,, -, ., /, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,

// 50-74
//  2, 3, 4, 5, 6, 7, 8, 9, :, ;, <, =, >, ?, @, A, B, C, D, E, F, G, H, I, J,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

// 75-99
//  K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, [, \, ], ^, _, `, a, b, c,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1,

// 100-124
//  d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, {, |,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,

// 125-149
//  }, ~,    Ä, Å, Ç, É, Ñ, ..., Ü, á, à, â, ä, ã, å, ç, é, è, ê, ', ', ", ", ï,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0,

// 150-174
//  -, -, ò, (tm), ö, õ, ú, ù, û, ü,  , °, ¢, £, §, •, ¶, ß, ®, (c), ™, ´, ¨, ≠, (r),
  1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,

// 175-199
// Ø, ∞, ±, ≤, ≥, ¥, µ, ∂, *, ∏, π, ∫, ª, 1/4, 1/2, 3/4, ø, ¿, ¡, ¬, √, ƒ, ≈, ∆, «,
  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,

// 200-224
//  », …,  , À,  , Õ, Œ, œ, –, —, “, ”, ‘, ’, ÷, ◊, ÿ, Ÿ, ⁄, €, ‹, ›, ﬁ, ﬂ,  ,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,

// 225-235
//  ·, ‚, „, ‰, Â, Ê, Á, Ë, È, Í, Î
  1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0
};

/***************************************************************************/
/*  PRESET TEXT CHARACTER TABLE                        TABLES.C  */
/***************************************************************************/
code UC PresetCharSet[128] = {
// 0-24
//   ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,  ,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

// 25-49
//                        , !, ", #, $, %, &, Ô, (, ), *, +, ,, -, ., /, 0, 1,
  0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1,

// 50-74
//  2, 3, 4, 5, 6, 7, 8, 9, :, ;, <, =, >, ?, @, A, B, C, D, E, F, G, H, I, J,
  1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

// 75-99
//  K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, [, \, ], ^, _, `, a, b, c,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1,

// 100-124
//  d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z, {, |,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,

// 125-127
//  }, ~,
  0, 0, 0
};

/***************************************************************************/
/*  FindPatternTable                                TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980213                          */
/* Revised:    980504                                       */
/*                                                  */
/*  Function:  Return a pointer to the pattern table related to a specific  */
/*          generator.                                  */
/*  Remarks:    --                                        */
/*  Returns:     Pointer to pattern table.                        */
/*  Updates:    --                                        */
/***************************************************************************/
code UC* FindPatternTable( UC HWtype) {

  switch ( HWtype) {
    case PT8601:
      return( PT8601PatternTable);

    case PT8602:
      return( PT8602PatternTable);

    case PT8603:
      return( PT8603PatternTable);

    case PT8609:
      return( PT8609PatternTable);

    case PT8631:
      return( PT8631PatternTable);

    case PT8632:
      return( PT8632PatternTable);

    case PT8633:
      return( PT8633PatternTable);

    case PT8639:
      return( PT8639PatternTable);
  }

  return( PT8609PatternTable);
}

/***************************************************************************/
/*  FindAudioLevelTable                              TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980512                          */
/* Revised:    980512                                       */
/*                                                  */
/*  Function:  Return a pointer to the audio level table related to a     */
/*          specific generator.                            */
/*  Remarks:    --                                        */
/*  Returns:     Pointer to audio level table.                      */
/*  Updates:    --                                        */
/***************************************************************************/
code UC* FindAudioLevelTable( UC HWtype)
  {
  switch ( HWtype)
    {
    case PT8602:
      return( PT8602AudioLevelTable);
    case PT8603:
      return( PT8603AudioLevelTable);
    case PT8632:
      return( PT8632AudioLevelTable);
    case PT8633:
      return( PT8633AudioLevelTable);
    //case PT8641:
      //return( PT8641AudioLevelTable);
    }
  return( PT8602AudioLevelTable);
  }

/***************************************************************************/
/*  AnlValidateSetting                              TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980212                          */
/* Revised:    980526                                       */
/*                                                  */
/*  Function:  Test if a specific configuration is valid.            */
/*  Remarks:    This function will validate the configuration if it is     */
/*          invalid in the specific system.                    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlValidateSetting( UC HWtype, UC ndx) {

  register UC i;
  UC system;
  char* textPtr;
  UC* patternPtr;
  UL* delayPtr;

  switch ( HWtype) {
    case PT8608:
      system = AnlBlkConfig[ndx].System;
      delayPtr = &AnlBlkConfig[ndx].Delay;
      break;

    case PT8601:
      textPtr = AnlTPGConfig[ndx].Text[0];

      i = 0;
      while ( textPtr[i] && ( i < 8))
        if ( !StdTextCharSet[textPtr[i++]]) {
          strcpy( textPtr, "ANALOG1");
          break;
        }
                            // Break IS missing!!
    case PT8631:
      system = AnlTPGConfig[ndx].System;
      patternPtr = &AnlTPGConfig[ndx].Pattern;
      delayPtr = &AnlTPGConfig[ndx].Delay;

      *patternPtr = AnlValidateGroupPattern( HWtype, ndx, system, AnlFindGroup( *patternPtr), *patternPtr);

      if ( *patternPtr == NoPattern)
        *patternPtr = AnlValidateGroupPattern( HWtype, ndx, system, CBarGroup, AnlEBUCBar);
      break;
  }

  if ( system > PAL_PALID)
    if ( *delayPtr >= ( long) 4*525*1716*256)
      *delayPtr = 0;
}

/***************************************************************************/
/*  AnlValidatePattern                              TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980325                          */
/* Revised:    990423                                       */
/*                                                  */
/*  Function:  Test if the pattern is valid in specified system.         */
/*  Remarks:    --                                        */
/*  Returns:     The argument <pattern> if it is valid, otherwise the      */
/*          substitute pattern will be returned.                */
/*  Updates:    --                                        */
/***************************************************************************/
UC AnlValidatePattern( UC HWtype, UC ndx, UC system, UC pattern) {

  UC tmp, HWcustom, SWversion;

  if ( FindPatternTable( HWtype)[pattern] == NoPattern)
    return( NoPattern);          // The pattern does not exist at all

  HWcustom = AnlTPGUnit[ndx].HWCustomPattern;
  SWversion = AnlTPGUnit[ndx].SWVersion;

  if ( system < NTSC) {           // Patterns that do not exist in PAL
    switch ( pattern) {
      case AnlSMPTECBar:
      case AnlFCCCBar:
        return( AnlEBUCBar);

      case AnlWindow10:
      case AnlBlWh15kHz:
        if (( HWtype != PT8631) || ( SWversion < 25))
          return( NoPattern);
        break;

      case AnlNTC7Combination:
      case AnlFCCMultiburst:
        return( AnlCCIR18);

      case AnlStair5:
      case AnlStair10:
        if (( HWtype == PT8601) && ( SWversion < 25))
          return( NoPattern);
        break;

      case AnlFCCComposite:
      case AnlNTC7Composite:
        return( AnlCCIR17);

      case AnlPhilips16x9:
      case AnlFuBK4x3:
      case AnlFuBK16x9:
      case AnlCrosshatch16x9:
      case AnlWhiteCircle4x3:
      case AnlWhiteCircle16x9:
        if (( HWtype != PT8631) || ( SWversion < 20))
          return( NoPattern);
        break;
    }
  }
  else {                    // Patterns that do not exist in NTSC
    switch ( pattern) {
      case AnlWindow10:
      case AnlBlWh15kHz:
        if (( HWtype != PT8631) || ( SWversion < 25))
          return( NoPattern);
        break;

      case AnlLumSweep:
      case AnlMultipulse:
        if (( HWtype == PT8631) && ( SWversion < 20))
          return( AnlSinxx);
        break;

      case AnlkHz250:
        if (( HWtype == PT8631) && ( SWversion < 20))
          return( AnlSafeArea);
        break;

      case AnlRamp:
      case AnlModulatedRamp:
        if (( HWtype == PT8631) && ( SWversion < 20))
          return( AnlStair5);
        break;

      case AnlStair10:
        if (( HWtype == PT8631) && ( SWversion < 20))
          return( AnlStair5);
                        // break IS missing!!
      case AnlStair5:
        if (( HWtype == PT8601) && ( SWversion < 25))
          return( NoPattern);
        break;

      case AnlEBUCBar:
      case AnlCBar100:
      case AnlCBarGrey75:
      case AnlCBarRed75:
        return( AnlSMPTECBar);

      case AnlCCIR18:
        return( AnlFCCMultiburst);

      case AnlCCIR17:
      case AnlCCIR330:
      case AnlCCIR331:
        return( AnlFCCComposite);

      case AnlPhilips4x3:
      case AnlPhilips16x9:
        if (( HWtype == PT8631) && ( SWversion >= 25))
          break;
        return( AnlCrosshatch);

      case AnlFuBK4x3:
      case AnlWhiteCircle4x3:
        if (( HWtype == PT8631) && ( SWversion < 20))
          return( NoPattern);
        return( AnlCrosshatch);

      case AnlFuBK16x9:
      case AnlWhiteCircle16x9:
      case AnlCrosshatch16x9:
        if (( HWtype == PT8631) && ( SWversion < 20))
          return( NoPattern);
        return( AnlCrosshatch16x9);

      case AnlVMT01:
        return( AnlCrosshatch);
    }
  }

  if (( pattern >= AnlCustom1) && ( pattern <= AnlCustom8)) {

    if ( system < NTSC) {
      if ( !( HWcustom & 0x08))
        return( NoPattern);

      tmp = HWcustom & 0x07;
    }
    else {
      if ( !( HWcustom & 0x80))
        return( NoPattern);

      tmp = ( HWcustom >> 4) & 0x07;
    }

    if (( pattern - AnlCustom1) <= tmp)
      return( pattern);

    return( AnlCustom1);
  }

  return( pattern);
}

/***************************************************************************/
/*  AnlValidateGroupPattern                            TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980330                          */
/* Revised:    990420                                       */
/*                                                  */
/*  Function:  Test if a pattern is valid in the specified group.        */
/*  Remarks:    --                                        */
/*  Returns:    The argument <pattern> if it exists in the group, otherwise  */
/*          the first pattern in the group will be returned. If the     */
/*          group doesn't contain any patterns, the parameter NoPattern */
/*           will be returned. It is an assumption that the pattern in  */
/*             fact DO belongto the specified group.                */
/*  Updates:    --                                        */
/***************************************************************************/
UC AnlValidateGroupPattern( UC HWtype, UC ndx, UC system, UC group, UC pattern) {

  register UC i;
  UC first, last;

  switch ( group) {
    case CBarGroup:
      first = AnlSMPTECBar;
      last = AnlRed75;
      break;

    case MBurstGroup:
      first = AnlMultiburst;
      last = AnlFCCMultiburst;
      break;

    case WindowGroup:
      first = AnlWindow10;
      last = AnlBlackBurst;
      break;

    case SpecialGroup:
      first = AnlFieldSquareWave;
      last = AnlCustom8;
      break;

    case LinearityGroup:
      first = AnlShallowRamp;
      last = AnlNTC7Composite;
      break;

    case PatternGroup:
      first = AnlPhilips4x3;
      last = AnlVMT01;
      break;
  }
                            // Test if pattern is valid
  pattern = AnlValidatePattern( HWtype, ndx, system, pattern);

  if ( pattern != NoPattern)             // If the pattern IS valid..
    return( pattern);              //  return it immediately

                            // Find 1st valid pattern
  for ( i = first; i <= last; i++) {
    pattern = AnlValidatePattern( HWtype, ndx, system, i);

    if ( pattern != NoPattern)        // If a valid pattern is found..
      return( pattern);            //  return it
  }

  return( NoPattern);              // Otherwise no pattern exists
}

/***************************************************************************/
/*  AnlFindNextGroupPattern                            TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980203                          */
/* Revised:    990420                                       */
/*                                                  */
/*  Function:  Find the next pattern in a specific group.            */
/*  Remarks:    This function MUST match the enumerated defintions for the  */
/*          ANALOG TPG as defined in TABLES.H.                  */
/*  Returns:    The next pattern in the specified group.              */
/*  Updates:    --                                        */
/***************************************************************************/
UC AnlNextGroupPattern( UC HWtype, UC ndx, UC system, UC group, UC pattern) {

  UC first, last;

  switch ( group) {
    case CBarGroup:
      first = AnlSMPTECBar;
      last = AnlRed75;
      break;

    case MBurstGroup:
      first = AnlMultiburst;
      last = AnlFCCMultiburst;
      break;

    case WindowGroup:
      first = AnlWindow10;
      last = AnlBlackBurst;
      break;

    case SpecialGroup:
      first = AnlFieldSquareWave;
      last = AnlCustom8;
      break;

    case LinearityGroup:
      first = AnlShallowRamp;
      last = AnlNTC7Composite;
      break;

    case PatternGroup:
      first = AnlPhilips4x3;
      last = AnlVMT01;
      break;

    default:
      first = AnlSMPTECBar;
      last = AnlVMT01;
      break;
  }

  do {
    if ( pattern++ == last)
      pattern = first;
  } while( pattern != AnlValidatePattern( HWtype, ndx, system, pattern));

  return( pattern);
}

/***************************************************************************/
/*  AnlFindGroup                                  TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980203                          */
/* Revised:    990420                                       */
/*                                                  */
/*  Function:  Finds the group that a analog pattern belongs to.        */
/*  Remarks:    This function MUST match the enumerated defintions for the  */
/*          Analog-TPG as defined in TABLES.H.                  */
/*  Returns:    The group that the pattern belongs to.                */
/*  Updates:    --                                        */
/***************************************************************************/
UC AnlFindGroup( UC pattern) {

  if ( pattern <= AnlRed75)
    return( CBarGroup);

  if ( pattern <= AnlFCCMultiburst)
    return( MBurstGroup);

  if ( pattern <= AnlBlackBurst)
    return( WindowGroup);

  if ( pattern <= AnlCustom8)
    return( SpecialGroup);

  if ( pattern <= AnlNTC7Composite)
    return( LinearityGroup);

  if ( pattern <= AnlVMT01)
    return( PatternGroup);

  return( NoGroup);
}

/***************************************************************************/
/*  AnlFindPatternType                              TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980304                          */
/* Revised:    980917                                       */
/*                                                  */
/*  Function:  Return the type of a pattern.                      */
/*  Remarks:    --                                        */
/*  Returns:    Type of the pattern, i.e. StandardPattern or ComplexPattern  */
/*  Updates:    --                                        */
/***************************************************************************/
UC AnlFindPatternType( UC pattern) {

  if (( pattern >= AnlPhilips4x3) && ( pattern <= AnlFuBK16x9))
    return( ComplexPattern);

  return( StandardPattern);
}

/***************************************************************************/
/*  AnlTPGAttrib                                  TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980222                          */
/* Revised:    980917                                       */
/*                                                  */
/*  Function:  Return the current attribute of a selected pattern        */
/*  Remarks:    --                                        */
/*  Returns:    Attribute of the selected pattern or ZERO              */
/*  Updates:    --                                        */
/***************************************************************************/
UC AnlTPGAttrib( UC ndx, UC pattern) {

  if (( pattern >= AnlPhilips4x3) && ( pattern <= AnlFuBK16x9))
    return( AnlTPGConfig[ndx].Attrib[pattern - AnlPhilips4x3]);

  return( 0);
}

/***************************************************************************/
/*  AnlValidateAttrib                                TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 990305                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Test if the attribute is valid for the specified pattern.  */
/*  Returns:    The argument <attrib> if it exists in the group, otherwise  */
/*          255 will be returned.                          */
/*  Updates:    --                                        */
/***************************************************************************/
UC AnlValidateAttrib( UC HWtype, UC ndx, UC system, UC pattern, UC attrib)
  {

  switch ( attrib)
    {
    case AttribAntiPAL:
      if ( system >= NTSC)
        return( 255);
      break;

    case AttribPLUGE:
    case AttribStair10:
      if (( pattern < AnlPhilips4x3) || ( pattern > AnlPhilips16x9))
        return( 255);
      break;

    case AttribMotion:
      return( 255);

    case AttribCircles:
      if ( pattern != AnlPhilips16x9)
        return( 255);
      break;

    case AttribLogo:
      return( 255);
    }
  return( attrib);
  }

/***************************************************************************/
/*  SDIValidateSetting                              TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980212                          */
/* Revised:    980526                                       */
/*                                                  */
/*  Function:  Test if a specific configuration is valid.            */
/*  Remarks:    This function will validate the configuration if it is     */
/*          invalid in the specific system.                    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDIValidateSetting( UC HWtype, UC ndx) {

  register UC i;
  UC system;
  char* textPtr;
  UC* patternPtr;
  UL* delayPtr;

  switch ( HWtype) {
    case PT8609:
      if ( SDITSGConfig[ndx].AudioLevel > TSGAudioSilence)
        SDITSGConfig[ndx].AudioLevel = TSGAudioSilence;

    case PT8639:
      system = SDITSGConfig[ndx].System;
      patternPtr = &SDITSGConfig[ndx].Pattern;
      delayPtr = &SDITSGConfig[ndx].Delay;
      break;

    case PT8602:
    case PT8603:
      textPtr = SDITPGConfig[ndx].Text[0];

      i = 0;
      while ( textPtr[i] && i < 8)
        if ( !StdTextCharSet[textPtr[i++]]) {
          strcpy( textPtr, "DIGITAL1");
          break;
        }
                              // Break IS missing!!
    case PT8632:
    case PT8633:
      system = SDITPGConfig[ndx].System;
      patternPtr = &SDITPGConfig[ndx].Pattern;
      delayPtr = &SDITPGConfig[ndx].Delay;
      break;
  }

  *patternPtr = SDIValidateGroupPattern( HWtype, ndx, system, SDIFindGroup( *patternPtr), *patternPtr);

  if ( *patternPtr == NoPattern)
    *patternPtr = SDIValidateGroupPattern( HWtype, ndx, system, CBarGroup, SDIEBUCBar);

  if ( system == SDI525)
    if ( *delayPtr >= ( long) 4*525*1716*256)
      *delayPtr = 0;
}

/***************************************************************************/
/*  SDIValidatePattern                              TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980325                          */
/* Revised:    990623                                       */
/*                                                  */
/*  Function:  Test if the pattern is valid in specified system.        */
/*  Remarks:    --                                        */
/*  Returns:     The argument <pattern> if it is valid, otherwise the      */
/*          substitute pattern will be returned.                */
/*  Updates:    --                                        */
/***************************************************************************/
UC SDIValidatePattern( UC HWtype, UC ndx, UC system, UC pattern) {

  UC tmp, HWcustom, HWinfo, SWversion;

  if ( FindPatternTable( HWtype)[pattern] == NoPattern)
    return( NoPattern);          // The pattern does not exist at all

  switch ( HWtype) {
    case PT8609:
    case PT8639:
      HWcustom = SDITSGUnit[ndx].HWCustomPattern;
      HWinfo = SDITSGUnit[ndx].HWInformation;
      SWversion = SDITSGUnit[ndx].SWVersion;
      break;

    default:
      HWcustom = SDITPGUnit[ndx].HWCustomPattern;
      HWinfo = SDITPGUnit[ndx].HWInformation;
      SWversion = SDITPGUnit[ndx].SWVersion;
      break;
  }

  if ( system == SDI625) {        // Patterns that do not exist in 625
    switch ( pattern) {
      case SDISMPTECBar:
      case SDIFCCCBar:
        return( SDIEBUCBar);

      case SDIWindow10:
      case SDIBlWh15kHz:
        switch ( HWtype) {
          case PT8632:
          case PT8633:
            if ( SWversion < 33)
              return( NoPattern);
            break;

          case PT8639:
            if ( SWversion < 13)
              return( NoPattern);
            break;

          default:          
            return( NoPattern);        
        }
        break;

      case SDIStair5:
      case SDIStair10:
        if (( HWtype == PT8603) && ( SWversion < 35))
          return( NoPattern);
        break;

      case SDICCIR18:
      case SDICCIR17:
      case SDICCIR330:
      case SDICCIR331:
        if (( HWtype != PT8633) || ( SWversion < 30))
          return( NoPattern);
        break;

      case SDIPhilips4x3:
        if (( HWtype == PT8632) && ( HWinfo != 0))
          return( NoPattern);
        break;

      case SDIFuBK4x3:
        if (( HWtype == PT8632) && ( HWinfo != 1))
          return( NoPattern);
        break;
    }
  }
  else {                    // Patterns that do not exist in 525
    switch ( pattern) {
      case SDICBar100:
        if ( HWtype < PT8632)
          return( SDISMPTECBar);
        break;

      case SDIEBUCBarITU:
        if (( HWtype == PT8602) || ( HWtype == PT8603))
          return( SDISMPTECBar);
        break;

      case SDIEBUCBar:
      case SDICBarGrey75:
      case SDICBarRed75:
        return( SDISMPTECBar);

      case SDIWindow10:
      case SDIBlWh15kHz:
        switch ( HWtype) {
          case PT8632:
          case PT8633:
            if ( SWversion < 33)
              return( NoPattern);
            break;

          case PT8639:
            if ( SWversion < 13)
              return( NoPattern);
            break;

          default:          
            return( NoPattern);        
        }
        break;

      case SDICCIR18:
        if (( HWtype != PT8633) || ( SWversion < 30))
          return( NoPattern);
        return( SDILumSweep);

      case SDIStair5:
      case SDIStair10:
        if (( HWtype == PT8603) && ( SWversion < 35))
          return( NoPattern);
        break;

      case SDICCIR17:
      case SDICCIR330:
      case SDICCIR331:
        if (( HWtype != PT8633) || ( SWversion < 30))
          return( NoPattern);
        return( SDIStair5);

      case SDIPhilips4x3:
        if ( HWtype == PT8632)
          return( SDICrosshatch);
        break;

      case SDIFuBK4x3:
        if ( HWtype == PT8632)
          return( SDICrosshatch);
        return( SDIPhilips4x3);

      case SDIFuBK16x9:
        return( SDIPhilips16x9);

      case SDIVMT01:
        return( SDICrosshatch);

    }
  }

  if (( pattern >= SDICustom1) && ( pattern <= SDICustom8)) {

    if ( system == SDI625) {
      if ( !( HWcustom & 0x08))
        return( NoPattern);

      tmp = HWcustom & 0x07;
    }
    else {
      if ( !( HWcustom & 0x80))
        return( NoPattern);

      tmp = ( HWcustom >> 4) & 0x07;
    }

    if (( pattern - SDICustom1) <= tmp)
      return( pattern);

    return( SDICustom1);
  }

  return( pattern);
}

/***************************************************************************/
/*  SDIValidateGroupPattern                            TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980330                          */
/* Revised:    990420                                       */
/*                                                  */
/*  Function:  Test if the pattern is valid in the specified group.      */
/*  Returns:    The argument <pattern> if it exists in the group, otherwise  */
/*          the first pattern in the group will be returned. If the     */
/*          group doesn't contain any patterns, the parameter NoPattern */
/*           will be returned. It is an assumption that the pattern in  */
/*             fact DO belongto the specified group.                */
/*  Updates:    --                                        */
/***************************************************************************/
UC SDIValidateGroupPattern( UC HWtype, UC ndx, UC system, UC group, UC pattern) {

  register UC i;
  UC first, last;

  switch ( group) {
    case CBarGroup:
      first = SDISMPTECBar;
      last = SDIRed75;
      break;

    case MBurstGroup:
      first = SDIMultiburst;
      last = SDICCIR18;
      break;

    case WindowGroup:
      first = SDIWindow10;
      last = SDIBlack;
      break;

    case SpecialGroup:
      first = SDICheckField;
      last = SDICustom8;
      break;

    case LinearityGroup:
      first = SDIShallowRamp;
      last = SDICbYCrYRamp;
      break;

    case PatternGroup:
      first = SDIPhilips4x3;
      last = SDIVMT01;
      break;
  }
                            // Test if pattern is valid
  pattern = SDIValidatePattern( HWtype, ndx, system, pattern);

  if ( pattern != NoPattern)             // If the pattern IS valid..
    return( pattern);              //  return it immediately

                            // Find 1st valid pattern
  for ( i = first; i <= last; i++) {
    pattern = SDIValidatePattern( HWtype, ndx, system, i);

    if ( pattern != NoPattern)        // If a valid pattern is found..
      return( pattern);            //  return it
  }

  return( NoPattern);              // Otherwise no pattern exists
}

/***************************************************************************/
/*  SDIFindNextGroupPattern                            TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980203                          */
/* Revised:    990420                                       */
/*                                                  */
/*  Function:  Find the next pattern in a specific group.            */
/*  Remarks:    This function MUST match the enumerated defintions for the  */
/*          SDI TPG as defined in TABLES.H.                    */
/*  Returns:    The next pattern in the specified group.              */
/*  Updates:    --                                        */
/***************************************************************************/
UC SDINextGroupPattern( UC HWtype, UC ndx, UC system, UC group, UC pattern) {

  UC first, last;

  switch ( group) {
    case CBarGroup:
      first = SDISMPTECBar;
      last = SDIRed75;
      break;

    case MBurstGroup:
      first = SDIMultiburst;
      last = SDICCIR18;
      break;

    case WindowGroup:
      first = SDIWindow10;
      last = SDIBlack;
      break;

    case SpecialGroup:
      first = SDICheckField;
      last = SDICustom8;
      break;

    case LinearityGroup:
      first = SDIShallowRamp;
      last = SDICbYCrYRamp;
      break;

    case PatternGroup:
      first = SDIPhilips4x3;
      last = SDIVMT01;
      break;

    default:
      first = SDISMPTECBar;
      last = SDIVMT01;
      break;
  }

  do {
    if ( pattern++ == last)
      pattern = first;
  } while ( pattern != SDIValidatePattern( HWtype, ndx, system, pattern));

  return( pattern);
}

/***************************************************************************/
/*  SDIFindGroup                                  TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980203                          */
/* Revised:    990420                                       */
/*                                                  */
/*  Function:  Find the group that a SDI pattern belongs to.          */
/*  Remarks:    This function MUST match the enumerated defintions for the  */
/*          SDI-TPG as defined in TABLES.H.                    */
/*  Returns:    The group that the pattern belongs to.                */
/*  Updates:    --                                        */
/***************************************************************************/
UC SDIFindGroup( UC pattern) {

  if ( pattern <= SDIRed75)
    return( CBarGroup);

  if ( pattern <= SDICCIR18)
    return( MBurstGroup);

  if ( pattern <= SDIBlack)
    return( WindowGroup);

  if ( pattern <= SDICustom8)
    return( SpecialGroup);

  if ( pattern <= SDICbYCrYRamp)
    return( LinearityGroup);

  if ( pattern <= SDIVMT01)
    return( PatternGroup);

  return( NoGroup);
}

/***************************************************************************/
/*  SDIFindPatternType                              TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980304                          */
/* Revised:    980522                                       */
/*                                                  */
/*  Function:  Return the type of a pattern.                      */
/*  Remarks:    --                                        */
/*  Returns:    Type of the pattern, i.e. StandardPattern or Complexpattern.*/
/*  Updates:    --                                        */
/***************************************************************************/
UC SDIFindPatternType( UC pattern) {

  if (( pattern >= SDIPhilips4x3) && ( pattern <= SDIFuBK16x9))
    return( ComplexPattern);

  return( StandardPattern);
}

/***************************************************************************/
/*  SDITPGAttrib                                  TABLES.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980222                          */
/* Revised:    980522                                       */
/*                                                  */
/*  Function:  Return the current attribute of a selected pattern.      */
/*  Remarks:    --                                        */
/*  Returns:    Attribute of the selected pattern, or ZERO.            */
/*  Updates:    --                                        */
/***************************************************************************/
UC SDITPGAttrib( UC ndx, UC pattern) {

  if (( pattern >= SDIPhilips4x3) && ( pattern <= SDIFuBK16x9))
    return( SDITPGConfig[ndx].Attrib[pattern - SDIPhilips4x3]);

  return( 0);
}

/***************************************************************************/
/*  SDIValidateAttrib                                            TABLES.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 990305                                    */
/* Revised:    990305                                                      */
/*                                                                         */
/*  Function:  Test if the attribute is valid for the specified pattern.   */
/*  Returns:   The argument <attrib> if it exists in the group, otherwise  */
/*             255 will be returned.                                       */
/*  Updates:    --                                                         */
/***************************************************************************/
UC SDIValidateAttrib( UC HWtype, UC ndx, UC system, UC pattern, UC attrib) {

  switch ( attrib) {
    case AttribAntiPAL:
      if ( system == SDI525)
        return( 255);
      break;

    case AttribPLUGE:
    case AttribStair10:
      if (( pattern < SDIPhilips4x3) || ( pattern > SDIPhilips16x9))
        return( 255);
      break;

    case AttribMotion:
      if (( pattern < SDIPhilips4x3) || ( pattern > SDIFuBK16x9))
        return( 255);
      break;

    case AttribCircles:
      if ( pattern != SDIPhilips16x9)
        return( 255);
      break;

    case AttribLogo:
      return( 255);
  }

  return( attrib);
}

UC HDTPGAttrib(UC ndx, UC pattern)
  {
  return (0);
  }

