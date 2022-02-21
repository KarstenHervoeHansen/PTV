/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PT8603 SDI Digital test signal generator              */
/*  Module:    D1PATT.H                                    */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  960614                                    */
/*  Rev. date:  990301, KEn DEV                              */
/*  Status:    Version 1.2                                  */
/*                                                  */
/*  Changes:                                            */
/* 990301: Added CB75STD_625                                */
/* 980929: Added Support for Staircase 5- & 10-step, both G & M, ie.      */
/*        GreyScale5_.. & GreyScale10_..                        */
/***************************************************************************/

/***************************************************************************/
// DEFINITION OF THE DIFFERENT LINE TYPES
/***************************************************************************/
#define SyncLineA          0x0000
#define SyncLineB          0x0001
#define SyncLineC          0x0002
#define SyncLineD          0x0003
#define sinc               0x0004
#define EBUCB_625          0x0006
#define CCIR17             0x0008
#define CCIR18_625         0x000A
#define MultiPulse         0x000C
#define Grey75_625         0x000E
#define ShallowRamp_625    0x0010
#define SDI_test_line1     0x0012
#define SDI_test_line3     0x0014
#define DigitalTiming_625  0x0016
#define SDI_test_line2     0x0018
#define WhiteLine_625      0x001A
#define Window50Marker     0x001C
#define Window15           0x001E
#define Window20           0x0020
#define Window100          0x0022
#define MarkerLine         0x0024
#define Marker15           0x0026
#define Marker20           0x0028
#define Marker100          0x002A
#define Red75_625          0x002C
#define Red75_525          0x002E
#define PlugeWhite_625     0x0030
#define PlugeGreyC_625     0x0032
#define PlugeGreyD_625     0x0034
#define PlugeGreyE_625     0x0036
#define PlugeWhite_525     0x0038
#define PlugeGreyC_525     0x003A
#define PlugeGreyD_525     0x003C
#define PlugeGreyE_525     0x003E
#define CrossHatch19_625   0x0040
#define SMPTEColorBar      0x0042
#define BlueBars           0x0044
#define IWhiteQ            0x0046
#define Chroma_MultiBurst  0x0048
#define _CCIR18_625        0x0048
#define WhiteLine_525      0x004A
#define CrossHatch17       0x004C
#define FCC_CB_525         0x004E
#define CB100_625          0x0050
#define ShallowRamp_525    0x0052
#define FCC_MultiBurst_525 0x0054
#define Chroma_MultiBurst_525  0x0056
#define SafeAreaA_525      0x0056
#define SafeAreaB_525      0x0058
#define SafeAreaC_525      0x005A
#define SafeAreaD_525      0x005C
#define SafeAreaE_525      0x005E
#define SafeAreaA_625      0x0060
#define SafeAreaB_625      0x0062
#define SafeAreaC_625      0x0064
#define SafeAreaD_625      0x0066
#define SafeAreaE_625      0x0068
#define EBUCB_8bit_625     0x006A
#define DigitalTiming_525  0x006C
#define AnalogBlanking     0x006E
#define CBarGrey75_625     0x0070
#define CBarRed75_625      0x0072
#define GreyScale5_625     0x0074
#define GreyScale5_525     0x0076
#define GreyScale10_625    0x0078
#define GreyScale10_525    0x007A

#define CB75STD_625        0x007C

/***************************************************************************/
// DEFINITION OF ALL PATTERNS AVAILABLE
/***************************************************************************/

typedef struct PATTERN_ITEM
 {
 code UI LineType;
 code UI NextIntrIn;
 code struct PATTERN_ITEM* NextPattern;
 };

extern code struct PATTERN_ITEM* code G_PatternTable[66];
extern code struct PATTERN_ITEM* code M_PatternTable[66];

/***************************************************************************/
// DEFINITION OF PATTERN ARRAYS, PAL-G
/***************************************************************************/
extern code struct PATTERN_ITEM G_Dummy;
extern code struct PATTERN_ITEM G_EBUCBar[6];
extern code struct PATTERN_ITEM G_EBUCBar8bit[6];
extern code struct PATTERN_ITEM G_CBar100[6];
extern code struct PATTERN_ITEM G_CBar75Grey[8];
extern code struct PATTERN_ITEM G_CBar75Red[8];
extern code struct PATTERN_ITEM G_Check[9];
extern code struct PATTERN_ITEM G_ShallowRamp[6];
extern code struct PATTERN_ITEM G_DigTimTest[10];
extern code struct PATTERN_ITEM G_Black[6];
extern code struct PATTERN_ITEM G_Window15[20];
extern code struct PATTERN_ITEM G_Window20[20];
extern code struct PATTERN_ITEM G_Window100[20];
extern code struct PATTERN_ITEM G_CrossHatch[62];
extern code struct PATTERN_ITEM G_PLUGE[16];
extern code struct PATTERN_ITEM G_MultiBurst[6];
extern code struct PATTERN_ITEM G_Red75[6];
extern code struct PATTERN_ITEM G_Stair5[6];
extern code struct PATTERN_ITEM G_Stair10[6];
extern code struct PATTERN_ITEM G_Customized1[6];
extern code struct PATTERN_ITEM G_Customized2[6];
extern code struct PATTERN_ITEM G_Customized3[6];
extern code struct PATTERN_ITEM G_Customized4[6];
extern code struct PATTERN_ITEM G_Customized5[6];
extern code struct PATTERN_ITEM G_Customized6[6];
extern code struct PATTERN_ITEM G_Customized7[6];
extern code struct PATTERN_ITEM G_Customized8[6];

/***************************************************************************/
// DEFINITION OF PATTERN ARRAYS, NTSC
/***************************************************************************/
extern code struct PATTERN_ITEM M_Dummy;
extern code struct PATTERN_ITEM M_SMPTECBar[10];
extern code struct PATTERN_ITEM M_Check[9];
extern code struct PATTERN_ITEM M_ShallowRamp[6];
extern code struct PATTERN_ITEM M_DigTimTest[10];
extern code struct PATTERN_ITEM M_Black[6];
extern code struct PATTERN_ITEM M_Window15[20];
extern code struct PATTERN_ITEM M_Window20[20];
extern code struct PATTERN_ITEM M_Window100[20];
extern code struct PATTERN_ITEM M_CrossHatch[62];
extern code struct PATTERN_ITEM M_PLUGE[16];
extern code struct PATTERN_ITEM M_MultiBurst[6];
extern code struct PATTERN_ITEM M_Red75[6];
extern code struct PATTERN_ITEM M_Stair5[6];
extern code struct PATTERN_ITEM M_Stair10[6];
extern code struct PATTERN_ITEM M_Customized1[6];
extern code struct PATTERN_ITEM M_Customized2[6];
extern code struct PATTERN_ITEM M_Customized3[6];
extern code struct PATTERN_ITEM M_Customized4[6];
extern code struct PATTERN_ITEM M_Customized5[6];
extern code struct PATTERN_ITEM M_Customized6[6];
extern code struct PATTERN_ITEM M_Customized7[6];
extern code struct PATTERN_ITEM M_Customized8[6];
