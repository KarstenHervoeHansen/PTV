/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 2000            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:   TABLES.H                                    */
/*  Author:   Kim Engedahl, DEV      HDSilence                       */
/*  Org. date:  980119                                    */
/*  Rev. date:  000509, KEn, DEV           HDSilence                   */
/*  Status:   Version 1.4                                 */
/*                                                  */
/*  Changes:                                            */
/* 000508: Added support for TPG  text positioning.               */
/* 990420: Added Window 10% & 15 kHz Bl/Wh in both SDI & Analog.        */
/* 990305: Added SDIValidateAttrib() and AnlValidateAttrib functions.   */
/* 980917: Added SDI: CCCIR17,18,330,331                        */
/* 980916: Added analog signals: FuBK-4:3/16:9, Philips16:9, Crosshatch16:9*/
/*         WhiteCircle-16:9.                                */
/* 980722: Added enum type TCIRegisterSelect                      */
/* 980702: Update enum type DateTimeReference                   */
/* 980616: Released as 4008 002 06812                         */
/* 980514: Released as 4008 002 06811                         */
/***************************************************************************/

#define NoPattern     255
#define NoAudioLevel    255

#define SDIXPosMin    1
#define SDIXPosMax    44

#define SDIYPosMin    1
#define SDIYPosMax    11


#define PT8603XPosMin    1
#define PT8603XPosMax    44

#define PT8603YPosMin    1
#define PT8603YPosMax    9

#define HDXPosMin    1
#define HDXPosMax    13

#define HDYPosMin    1
#define HDYPosMax    7


#define AnlXPosMin    1
#define AnlXPosMax    44

#define AnlYPosMin    1
#define AnlYPosMax    11

enum AnlSystems {
  PAL = 0, PAL_PALID, NTSC, NTSCwoSetup
};

enum SDISystems {
  SDI625 = 0, SDI525
};

enum PatternGroups {
  CBarGroup = 0, MBurstGroup, WindowGroup, SpecialGroup, LinearityGroup,
  PatternGroup, NoGroup
};

enum PatternTypes {
  StandardPattern = 0, ComplexPattern
};

enum AnlPatterns {
  // Colorbar group:  7 patterns
  AnlSMPTECBar = 0, AnlEBUCBar, AnlFCCCBar, AnlCBar100, AnlCBarGrey75,
  AnlCBarRed75, AnlRed75,

  //Multiburst group: 7 patterns
  AnlMultiburst, AnlLumSweep, AnlMultipulse, AnlSinxx, AnlCCIR18,
  AnlNTC7Combination, AnlFCCMultiburst,

  // Window group: 8 patterns
  AnlWindow10, AnlWindow15, AnlWindow20, AnlWindow100, AnlBlWh15kHz,
  AnlGrey50, AnlWhite100, AnlBlackBurst,

  // Special group: 2 + 8 patterns
  AnlFieldSquareWave, AnlBlWh01Hz, AnlCustom1, AnlCustom2, AnlCustom3,
  AnlCustom4, AnlCustom5, AnlCustom6, AnlCustom7, AnlCustom8,

  // Linearity group: 12 patterns
  AnlShallowRamp, AnlRamp, AnlModulatedRamp, AnlStair5, AnlModulatedStair5,
  AnlStair10, AnlPulseBar, AnlCCIR17, AnlCCIR330, AnlCCIR331,
  AnlFCCComposite, AnlNTC7Composite,

  // Pattern group: 12 patterns
  AnlPhilips4x3, AnlPhilips16x9, AnlFuBK4x3, AnlFuBK16x9, AnlCrosshatch,
  AnlCrosshatch16x9, AnlWhiteCircle4x3, AnlWhiteCircle16x9, AnlPLUGE,
  AnlSafeArea, AnlkHz250, AnlVMT01
};

enum SDIPatterns
  {
  // Colorbar group: 8 patterns
  SDISMPTECBar = 0, SDIEBUCBar, SDIFCCCBar, SDIEBUCBarITU, SDICBar100,\
  SDICBarGrey75,  SDICBarRed75, SDIRed75,

  // Multiburst group: 6 patterns
  SDIMultiburst, SDILumSweep, SDIYCrCbSweep, SDIMultipulse, SDISinxx,
  SDICCIR18,

  // Window group: 7 patterns
  SDIWindow10, SDIWindow15, SDIWindow20, SDIWindow100, SDIBlWh15kHz, 
  SDIWhite100, SDIBlack,

  // Special group: 14 + 8 patterns
  SDICheckField, SDITimingTest, SDIFieldDelayTest, SDIBowTie,
  SDIAnalogBlanking, SDIDigitalGrey, SDIFieldSquareWave, SDIBlWh01Hz, SDIEOL,
  SDIWhiteEOL, SDIBlueEOL, SDIRedEOL, SDIYellowEOL, SDICyanEOL, SDICustom1,
  SDICustom2, SDICustom3, SDICustom4, SDICustom5, SDICustom6, SDICustom7,
  SDICustom8,

  // Linearity group: 17 patterns
  SDIShallowRamp, SDIRamp, SDILimitRamp, SDIValidRamp, SDIStair5,
  SDIModulatedStair5, SDIStair10, SDIPulseBar, SDICCIR17, SDICCIR330,
  SDICCIR331, SDIYellowGreyRamp, SDIGreyBlueRamp, SDICyanGreyRamp,
  SDIGreyRedRamp, SDICbYCrYRamp,

  // Pattern group: 8 patterns
  SDIPhilips4x3, SDIPhilips16x9, SDIFuBK4x3, SDIFuBK16x9, SDICrosshatch,
  SDIPLUGE, SDISafeArea, SDIVMT01
  };

enum PatternModifications
  {
  AttribAntiPAL = 0, AttribPLUGE, AttribStair10, AttribMotion,
  AttribCircles,     AttribLogo,  AttribFree1,   AttribFree2
  };

/*****************************************************************************/
/*                              HD                                           */
/*****************************************************************************/

enum TextScales
  {
  ScaleOne = 0, ScaleTwo, ScaleThree, ScaleFour
  }



enum TextColors
  {
  ColorBlack = 0, ColorBlue, ColorGreen, ColorCyan, ColorRed, ColorMagenta, 
  ColorYellow, ColorWhite
  }

enum TextMovement
  {
  TextMovementNone, TextMovementHoriz, TextMovementVertical, TextMovementBoth
  }


enum HDAudioLevels
  {
  HDLevel0 = 0, HDLevel6, HDLevel12, HDLevel18, HDLevel24, HDLevelM60 = 60      //, 
  };

enum HDAudioSignalItems
  {
  HDSilence, HDSine, HDClick, HDAudioOff
  }

enum HDPatterns
  {
  HDColorbar = 0, HDCombination, HDWindow, HDWhite,        /* First 4 have attributes */
  HDCrosshatch,   HDCheckField,  HDPluge,  HDLumRamp,      /* Others do not.          */
  HDClapBoard,    HDBlack
  };

enum HDTPGCBAttributes
  {
  HundHund = 0,
  HundSeven,
  SevenSeven,
  };


enum HDTPGWindowAttributes
  {
  WhiteM5 = 0,
  White0,
  White5,
  White10,
  White15,
  White20,
  White25,
  White30,
  White35,
  White40,
  White45,
  White50,
  White55,
  White60,
  White65,
  White70,
  White75,
  White80,
  White85,
  White90,
  White95,
  White100,
  White105,
  };

/*****************************************************************************/
/*                              HD                                           */
/*****************************************************************************/

enum TSGAudioSignals  {
  TSGAudioOff = 0, TSGAudioStereo1kHz
};

enum TSGAudioLevels {
  TSGAudioSilence = 0, TSGAudioLevel0dB, TSGAudioLevel9dB,
  TSGAudioLevel15dB, TSGAudioLevel18dB
};

enum TPGAudioSignals {
  TPGAudioOff = 0, TPGAudioStereo800Hz, TPGAudioStereo1kHz,
  TPGAudioStereoEBU1kHz, TPGAudioStereoBBC1kHz, TPGAudioMonoEBU1kHz,
  TPGAudioMono1kHz, TPGAudioDual
};

enum TPGAudioLevels {
  TPGAudioSilence = 0, TPGAudioLevel0dB, TPGAudioLevel9dB, TPGAudioLevel12dB,
  TPGAudioLevel14dB, TPGAudioLevel15dB, TPGAudioLevel16dB,  TPGAudioLevel18dB,
  TPGAudioLevel20dB
};

enum TPGAudioGroups {
  TPGAudioGroup1 = 0, TPGAudioGroup2, TPGAudioGroup3, TPGAudioGroup4
};

enum TPGTextStyles {
  TPGFreeTextStyle = 0, TPGStandardTextStyle, TPGComplexTextStyle
};

enum TPGTextClock {
  TPGTextClockNone = 0, TPGTextClockTime, TPGTextClockDate,
  TPGTextClockTimeDate
};

enum AESEBUSignals
  {
  AESEBUOff = 0,       AESEBUStereo800Hz,   AESEBUStereo1kHz,
  AESEBUStereoEBU1kHz, AESEBUStereoBBC1kHz, AESEBUMonoEBU1kHz,
  AESEBUMono1kHz,      AESEBUDual,          AESEBUWordClock
  };

enum AESEBULevels {
  AESEBUSilence = 0, AESEBULevel0dB, AESEBULevel9dB, AESEBULevel12dB,
  AESEBULevel14dB, AESEBULevel15dB, AESEBULevel16dB, AESEBULevel18dB,
  AESEBULevel20dB
};

enum AESEBUTiming {
  AESEBUPAL = 0, AESEBUNTSCPhase1, AESEBUNTSCPhase2, AESEBUNTSCPhase3,
  AESEBUNTSCPhase4, AESEBUNTSCPhase5
};

enum GenlockInput
  {                       //gps genlock
  GenlockA = 0, GenlockB, LoopThrough, SDIGenlock, GPSGenlock, Internal, Internal2
  };

enum GenlockSystem
  {
  BurstLockPAL = 0, BurstLockNTSC, SyncLock625, SyncLock525, SDI625Lock,
  SDI525Lock, MHzLock358, MHzLock443, MHzLock5, MHzLock10
  };

enum TCITimeFormat {
  Hour12Format = 0, Hour24Format
};

enum TCIDateFormat {
  DMYFormat = 0, MDYFormat, YMDFormat
};

enum TCITimeReference {
  VITCTimeInput = 0, LTCTimeInput, Reference1HzInput, VideoFieldFreqInput
};

enum TCIRegisterSelect {
  TCITimeRefRegister = 0, TCITimeRegister, TCIDateRegister,
  TCIErrorRegister, TCIOffsetRegister, TCIInfoRegister
};

//extern code UC PT8641AudioLevelTable[];

extern code UC GenlockInputTable[14];
extern code UC GenlockSystemTable[20];

extern code UI AESEBUSignalTable[9];
extern code UI AESEBULevelTable[9];
extern code UI AESEBUTimingTable[6];

extern code UC StdTextCharSet[100];
extern code UC ExtTextCharSet[236];
extern code UC PresetCharSet[128];

extern code UC* FindPatternTable( UC HWtype);
extern code UC* FindAudioLevelTable( UC HWtype);

extern void AnlValidateSetting( UC HWtype, UC ndx);
extern UC AnlValidatePattern( UC HWtype, UC ndx, UC system, UC pattern);
extern UC AnlValidateGroupPattern( UC HWtype, UC ndx, UC system, UC group, UC pattern);
extern UC AnlNextGroupPattern( UC HWtype, UC ndx, UC system, UC group, UC pattern);

extern UC AnlFindGroup( UC pattern);
extern UC AnlFindPatternType( UC pattern);
extern UC AnlTPGAttrib( UC ndx, UC pattern);
extern UC AnlValidateAttrib( UC HWtype, UC ndx, UC system, UC pattern, UC attrib);

extern void SDIValidateSetting( UC HWtype, UC ndx);
extern UC SDIValidatePattern( UC HWtype, UC ndx, UC system, UC pattern);
extern UC SDIValidateGroupPattern( UC HWtype, UC ndx, UC system, UC group, UC pattern);
extern UC SDINextGroupPattern( UC HWtype, UC ndx, UC system, UC group, UC pattern);

extern UC SDIFindGroup( UC pattern);
extern UC SDIFindPatternType( UC pattern);
extern UC SDITPGAttrib( UC ndx, UC pattern);
extern UC SDIValidateAttrib( UC HWtype, UC ndx, UC system, UC pattern, UC attrib);

extern UC HDTPGAttrib(UC ndx, UC pattern);



/* from text.c
{ VITCInputTxt,
							   GPSInputTxt,
							   AutoInputTxt,
							   VideoFieldFreqInputTxt };
*/
enum LTCTimeReference {
  LTC_VITC_Input = 0, iPAL,iNTSC,LTC_GPS_Input
};

