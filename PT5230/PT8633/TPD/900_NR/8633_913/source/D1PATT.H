/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT8632/33 SDI Test Pattern Generator 1/2              */
/*  Module:   D1PATT.H                                    */
/*  Author:   Kim Engedahl, DEV                             */
/*  Org. date:  960614                                    */
/*  Rev. date:  990623, KEn, DEV                              */
/*  Status:   Version 2.4                                 */
/*                                                  */
/*  Changes:                                            */
/* 10.01.2005: Window10, Window15, Window20 changed to full screen          */
/*             i.e. no windows, for MEDIAPLAZA in Germany.                  */
/*             Added MEDIAPLAZA def in d1pattg.h                            */
/* 990623: Added lines/signals for 15kHz Bl/Wh & Window 10%           */
/*  990323: PT8633: Increased array of G_FuBK16x9[36] to support new audio  */
/*  990322: PT8633: Increased array of G_FuBK4x3[36] to support new audio */
/*  990322: PT8633: Increased array of G_Philips16x9[56] to support new   */
/*        audio                                       */
/* 990219: Changed CBar75Red to EBUCBarRed                      */
/* 981109: For PT8633: Increased array of G_Philips4x3[] from 32 to 36.   */
/* 981002: Released in SW ver. 3.0                            */
/* 980924: Added 3 new patterns in SDI625 for the PT8633.           */
/* 980603: Released in SW ver. 2.0                            */
/* 980525: Added customized pattern                           */
/***************************************************************************/

#define MEDIAPLAZA

#if PT8633Version
  //************************************************************************
  // DEFINITIONS FOR THE PT8633 GENERATOR
  //************************************************************************
  #define SyncLineA         0x0000
  #define SyncLineB         0x0001
  #define SyncLineC         0x0002
  #define SyncLineD         0x0003
  
  #define EBUCB_625         0x0006
  #define EBUCB_ITU         0x0052
  #define Grey75_625        0x000A
  #define SMPTEColorBar     0x003E
  #define BlueBars          0x0040
  #define IWhiteQ           0x0042
  #define FCC_CB_525        0x004A
  #define CB100_625         0x004C
  #define Red75_625         0x0028
  #define Red75_525         0x002A
  #define CB75STD_625       0x009C
  #define CB100_525         0x00B4
  
  #define Chroma_MultiBurst   0x0044
  #define _CCIR18_625         0x0044
  #define FCCMultiBurst_525   0x0050
  #define ChromaMultiBurst_525  0x0052
  #define LineSweepY_625      0x008C
  #define LineSweepY_525      0x008E

  #define _YLineSweep_625       0x0090
  #define _ChromaLineSweep_625  0x0090
  #define _ChromaLineSweep_525  0x0092
  #define _YLineSweep_525       0x0092

  #define sinc              0x0004
  #define sinc_525          0x00C6
  #define Multipulse        0x0008

  #define CrossHatch19_625    0x003C
  #define WhiteLine_625       0x0016
  #define WhiteLine_525       0x0046
  #define CrossHatch17        0x0048
  #define PlugeWhite_625      0x002C
  #define PlugeGreyC_625      0x002E
  #define PlugeGreyD_625      0x0030
  #define PlugeGreyE_625      0x0032
  #define PlugeWhite_525      0x0034
  #define PlugeGreyC_525      0x0036
  #define PlugeGreyD_525      0x0038
  #define PlugeGreyE_525      0x003A
  #define SafeAreaA_525     0x0078
  #define SafeAreaB_525     0x007A
  #define SafeAreaC_525     0x007C
  #define SafeAreaD_525     0x007E
  #define SafeAreaE_525     0x0080
  #define SafeAreaA_625     0x0082
  #define SafeAreaB_625     0x0084
  #define SafeAreaC_625     0x0086
  #define SafeAreaD_625     0x0088
  #define SafeAreaE_625     0x008A
  #define kHz15BlWh_625     0x00D0
  #define kHz15BlWh_525     0x00D2

  #define ShallowRamp_625     0x0062
  #define ShallowRamp_525     0x0064
  #define ValidRampY        0x006A
  #define ValidRampUV       0x006C
  #define BlackWhiteRamp      0x00AE
  #define YellowGreyRamp      0x00BE
  #define GreyBlueRamp        0x00C0
  #define CyanGreyRamp        0x00C2
  #define GreyRedRamp       0x00C4
  #define Rampe52usec_625     0x000C
  #define GreyScale5_625      0x0058
  #define GreyScale5_525      0x005A
  #define GreyScale10_625     0x005C
  #define GreyScale10_525     0x005E
  #define ModGreyScale5_625   0x00C8
  #define ModGreyScale5_525   0x00CA
  #define SavtandYCbCrY     0x00BC
  #define T2T20BarInv2T_625   0x00B0
  #define T2T20BarInv2T_525   0x00B2
  #define HelLinierampe_525   0x004E

  #define Window50Marker      0x0018

  #define Our               0x0082 /****************************/

  #define MarkerLine        0x0020
  #define Window10          0x00CC
  #define Window15          0x001A
  #define Window20          0x001C
  #define Window100         0x001E
  #define Marker10          0x00CE
  #define Marker15          0x0022
  #define Marker20          0x0024
  #define Marker100         0x0026
  #define Black           0x0060

  #define SDITestSignalLine1    0x000E
  #define SDITestSignalLine2    0x0014
  #define SDISignalLine3      0x0010
  #define DigitalGrey_625     0x0066
  #define DigitalGrey_525     0x0068
  #define MarkerForBowTie     0x00B8
  #define BowTie            0x00BA
  #define WhiteITU801       0x0094
  #define EndOfLinePorches    0x009E
  #define WhiteEoflPorch      0x00A0
  #define BlueEoflPorch     0x00A2
  #define RedEoflPorch        0x0096
  #define YellowEoflPorch     0x0098
  #define CyanEoflPorch     0x009A
  #define DigitalTiming_625   0x0012
  #define DigitalTiming_525   0x0054
  #define TestAnalogBlanking    0x0056
  #define Y100mV_625        0x006C
  #define Y200mV_625        0x006E
  #define Y300mV_625        0x0070
  #define Y400mV_625        0x0072
  #define Y500mV_625        0x0074
  #define Y600mV_625        0x0076

  #define SavTandForVMT     0x00A4
  #define CCIR17            0x00A6
  #define CCIR18            0x00B6
  #define kHz15           0x00A8
  #define CCIR330         0x00AA
  #define CCIR331         0x00AC

  #define CounterType       0xFFFF  // Special type to indicate counter
#else
  //************************************************************************
  // DEFINITIONS FOR THE PT8632 GENERATOR
  //************************************************************************
  #define SyncLineA         0x0000
  #define SyncLineB         0x0001
  #define SyncLineC         0x0002
  #define SyncLineD         0x0003

  #define EBUCB_625         0x0006
  #define EBUCB_ITU         0x0052
  #define Grey75_625        0x000A
  #define SMPTEColorBar     0x003E
  #define BlueBars          0x0040
  #define IWhiteQ         0x0042
  #define FCC_CB_525        0x004A
  #define CB100_625         0x004C
  #define Red75_625         0x0028
  #define Red75_525         0x002A
  #define CB75STD_625       0x0078
  #define CB100_525         0x007A

  #define Chroma_MultiBurst   0x0044
  #define _CCIR18_625       0x0044
  #define FCCMultiBurst_525   0x0050
  #define ChromaMultiBurst_525  0x0052
  #define LineSweepY_625      0x007E
  #define LineSweepY_525      0x0080
  #define Multipulse        0x0082

  #define CrossHatch19_625    0x003C
  #define WhiteLine_625     0x0016
  #define WhiteLine_525     0x0046
  #define CrossHatch17        0x0048
  #define PlugeWhite_625      0x002C
  #define PlugeGreyC_625      0x002E
  #define PlugeGreyD_625      0x0030
  #define PlugeGreyE_625      0x0032
  #define PlugeWhite_525      0x0034
  #define PlugeGreyC_525      0x0036
  #define PlugeGreyD_525      0x0038
  #define PlugeGreyE_525      0x003A
  #define kHz15BlWh_625     0x009A
  #define kHz15BlWh_525     0x009C

  #define ShallowRamp_625     0x0062
  #define ShallowRamp_525     0x0064
  #define Rampe52usec_625     0x008A
  #define HelLinierampe_525   0x008C
  #define BlackWhiteRamp      0x007C
  #define ValidRampY        0x006A
  #define ValidRampUV       0x006C
  #define GreyScale5_625      0x0058
  #define GreyScale5_525      0x005A
  #define ModGreyScale5_625   0x008E
  #define ModGreyScale5_525   0x0090
  #define T2T20BarInv2T_625   0x0092
  #define T2T20BarInv2T_525   0x0094
  #define GreyScale10_625     0x005C
  #define GreyScale10_525     0x005E

  #define Window50Marker      0x0018
  #define MarkerLine        0x0020
  #define Window10          0x0096
  #define Window15          0x001A
  #define Window20          0x001C
  #define Window100         0x001E
  #define Marker10          0x0098
  #define Marker15          0x0022
  #define Marker20          0x0024
  #define Marker100         0x0026
  #define Black           0x0060

  #define SDITestSignalLine1    0x000E
  #define SDITestSignalLine2    0x0014
  #define SDISignalLine3      0x0010
  #define MarkerForBowTie     0x0084
  #define BowTie            0x0086
  #define DigitalGrey_625     0x0066
  #define DigitalGrey_525     0x0068
  #define DigitalTiming_625   0x0012
  #define DigitalTiming_525   0x0054
  #define TestAnalogBlanking    0x0056
  #define Y100mV_625        0x006C
  #define Y200mV_625        0x006E
  #define Y300mV_625        0x0070
  #define Y400mV_625        0x0072
  #define Y500mV_625        0x0074
  #define Y600mV_625        0x0076

  #define CounterType       0xFFFF  // Special type to indicate counter
#endif

//************************************************************************
// DEFINITION OF ALL PATTERNS AVAILABLE
//************************************************************************
typedef struct PATTERN_ITEM {
  code UI LineType;
  code UI NextIntrIn;
  code UI LineAddrStart;

  code struct PATTERN_ITEM* NextPattern;
};

extern code struct PATTERN_ITEM* code G_PatternTable[68];
extern code struct PATTERN_ITEM* code M_PatternTable[68];

//************************************************************************
// DEFINITION OF PATTERN ARRAYS IN SDI625
//************************************************************************
extern code struct PATTERN_ITEM G_Dummy;

extern code struct PATTERN_ITEM G_EBUCBar[6];
extern code struct PATTERN_ITEM G_EBUCBarITU[6];
extern code struct PATTERN_ITEM G_CBar100[6];
extern code struct PATTERN_ITEM G_CBar75Grey[8];
extern code struct PATTERN_ITEM G_EBUCBarRed[8];
extern code struct PATTERN_ITEM G_CheckField[9];
extern code struct PATTERN_ITEM G_ShallowRamp[6];
extern code struct PATTERN_ITEM G_TimingTest[10];
extern code struct PATTERN_ITEM G_Black[6];

#ifdef MEDIAPLAZA
extern code struct PATTERN_ITEM G_Window20[6];
extern code struct PATTERN_ITEM G_Window15[6];
extern code struct PATTERN_ITEM G_Window10[6];
#else
extern code struct PATTERN_ITEM G_Window20[20];
extern code struct PATTERN_ITEM G_Window15[20];
extern code struct PATTERN_ITEM G_Window10[20];
#endif

extern code struct PATTERN_ITEM G_Window100[20];
extern code struct PATTERN_ITEM G_CrossHatch[62];
extern code struct PATTERN_ITEM G_PLUGE[16];
extern code struct PATTERN_ITEM G_Multiburst[6];
extern code struct PATTERN_ITEM G_Red75[6];

#if PT8633Version
extern code struct PATTERN_ITEM G_Philips4x3[36];
#else
extern code struct PATTERN_ITEM G_Philips4x3[32];
#endif

extern code struct PATTERN_ITEM G_Philips4x3woAPAL[6];
extern code struct PATTERN_ITEM G_CCIR17[6];
extern code struct PATTERN_ITEM G_AnalogBlanking[6];
extern code struct PATTERN_ITEM G_Multipulse[6];
extern code struct PATTERN_ITEM G_Sinxx[6];
extern code struct PATTERN_ITEM G_DigitalGrey[6];
extern code struct PATTERN_ITEM G_FieldTimingChk[24];
extern code struct PATTERN_ITEM G_ValidRamp[6];
extern code struct PATTERN_ITEM G_Stair5[6];
extern code struct PATTERN_ITEM G_Stair10[6];
extern code struct PATTERN_ITEM G_LuminanceSweep[6];
extern code struct PATTERN_ITEM G_YCrCbSweep[6];
extern code struct PATTERN_ITEM G_Flat100[6];
extern code struct PATTERN_ITEM G_BowTie[10];
extern code struct PATTERN_ITEM G_AltBlWh[6];
extern code struct PATTERN_ITEM G_EndOfLine[6];
extern code struct PATTERN_ITEM G_WhiteEndOfLine[6];
extern code struct PATTERN_ITEM G_BlueEndOfLine[6];
extern code struct PATTERN_ITEM G_RedEndOfLine[6];
extern code struct PATTERN_ITEM G_YellowEndOfLine[6];
extern code struct PATTERN_ITEM G_CyanEndOfLine[6];
extern code struct PATTERN_ITEM G_LuminanceRamp[6];
extern code struct PATTERN_ITEM G_LimitRamp[6];
extern code struct PATTERN_ITEM G_PulseBar[6];
extern code struct PATTERN_ITEM G_YellowGreyRamp[6];
extern code struct PATTERN_ITEM G_GreyBlueRamp[6];
extern code struct PATTERN_ITEM G_CyanGreyRamp[6];
extern code struct PATTERN_ITEM G_GreyRedRamp[6];
extern code struct PATTERN_ITEM G_CbYCrYRamp[6];
extern code struct PATTERN_ITEM G_Philips16x9[56];

#if PT8633Version
extern code struct PATTERN_ITEM G_FuBK4x3[36];
#else
extern code struct PATTERN_ITEM G_FuBK4x3[28];
#endif

extern code struct PATTERN_ITEM G_FuBK16x9[36];
extern code struct PATTERN_ITEM G_SafeArea[34];
extern code struct PATTERN_ITEM G_VMT01[28];
extern code struct PATTERN_ITEM G_FieldSquareWave[10];
extern code struct PATTERN_ITEM G_ModulatedStair5[6];
extern code struct PATTERN_ITEM G_Customized1[6];
extern code struct PATTERN_ITEM G_Customized2[6];
extern code struct PATTERN_ITEM G_Customized3[6];
extern code struct PATTERN_ITEM G_Customized4[6];
extern code struct PATTERN_ITEM G_Customized5[6];
extern code struct PATTERN_ITEM G_Customized6[6];
extern code struct PATTERN_ITEM G_Customized7[6];
extern code struct PATTERN_ITEM G_Customized8[6];
extern code struct PATTERN_ITEM G_CCIR18[6];
extern code struct PATTERN_ITEM G_CCIR330[6];
extern code struct PATTERN_ITEM G_CCIR331[6];
extern code struct PATTERN_ITEM G_kHz15BlWh[6];

//************************************************************************
// DEFINITION OF PATTERN ARRAYS IN SDI525
//************************************************************************
extern code struct PATTERN_ITEM M_Dummy;

extern code struct PATTERN_ITEM M_SMPTECBar[10];
extern code struct PATTERN_ITEM M_EBUCBarITU[6];
extern code struct PATTERN_ITEM M_CBar100[6];
extern code struct PATTERN_ITEM M_CheckField[9];
extern code struct PATTERN_ITEM M_ShallowRamp[6];
extern code struct PATTERN_ITEM M_TimingTest[10];
extern code struct PATTERN_ITEM M_Black[6];
extern code struct PATTERN_ITEM M_Window15[20];
extern code struct PATTERN_ITEM M_Window20[20];
extern code struct PATTERN_ITEM M_Window100[20];
extern code struct PATTERN_ITEM M_CrossHatch[62];
extern code struct PATTERN_ITEM M_PLUGE[16];
extern code struct PATTERN_ITEM M_Multiburst[6];
extern code struct PATTERN_ITEM M_Red75[6];
extern code struct PATTERN_ITEM M_Philips4x3[16];
extern code struct PATTERN_ITEM M_AnalogBlanking[6];
extern code struct PATTERN_ITEM M_Multipulse[6];
extern code struct PATTERN_ITEM M_Sinxx[6];
extern code struct PATTERN_ITEM M_DigitalGrey[6];
extern code struct PATTERN_ITEM M_FieldTimingChk[12];
extern code struct PATTERN_ITEM M_ValidRamp[6];
extern code struct PATTERN_ITEM M_Stair5[6];
extern code struct PATTERN_ITEM M_Stair10[6];
extern code struct PATTERN_ITEM M_LuminanceSweep[6];
extern code struct PATTERN_ITEM M_YCrCbSweep[6];
extern code struct PATTERN_ITEM M_Flat100[6];
extern code struct PATTERN_ITEM M_BowTie[10];
extern code struct PATTERN_ITEM M_AltBlWh[6];
extern code struct PATTERN_ITEM M_EndOfLine[6];
extern code struct PATTERN_ITEM M_WhiteEndOfLine[6];
extern code struct PATTERN_ITEM M_BlueEndOfLine[6];
extern code struct PATTERN_ITEM M_RedEndOfLine[6];
extern code struct PATTERN_ITEM M_YellowEndOfLine[6];
extern code struct PATTERN_ITEM M_CyanEndOfLine[6];
extern code struct PATTERN_ITEM M_LuminanceRamp[6];
extern code struct PATTERN_ITEM M_LimitRamp[6];
extern code struct PATTERN_ITEM M_PulseBar[6];
extern code struct PATTERN_ITEM M_YellowGreyRamp[6];
extern code struct PATTERN_ITEM M_GreyBlueRamp[6];
extern code struct PATTERN_ITEM M_CyanGreyRamp[6];
extern code struct PATTERN_ITEM M_GreyRedRamp[6];
extern code struct PATTERN_ITEM M_CbYCrYRamp[6];
extern code struct PATTERN_ITEM M_Philips16x9[22];
extern code struct PATTERN_ITEM M_SafeArea[34];
extern code struct PATTERN_ITEM M_VMT01[6];
extern code struct PATTERN_ITEM M_FCCCBar[6];
extern code struct PATTERN_ITEM M_FieldSquareWave[10];
extern code struct PATTERN_ITEM M_ModulatedStair5[6];
extern code struct PATTERN_ITEM M_Customized1[6];
extern code struct PATTERN_ITEM M_Customized2[6];
extern code struct PATTERN_ITEM M_Customized3[6];
extern code struct PATTERN_ITEM M_Customized4[6];
extern code struct PATTERN_ITEM M_Customized5[6];
extern code struct PATTERN_ITEM M_Customized6[6];
extern code struct PATTERN_ITEM M_Customized7[6];
extern code struct PATTERN_ITEM M_Customized8[6];
extern code struct PATTERN_ITEM M_Window10[20];
extern code struct PATTERN_ITEM M_kHz15BlWh[6];
