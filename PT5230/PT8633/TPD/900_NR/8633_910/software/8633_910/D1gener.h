/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT8632/33 SDI Test Pattern Generator 1/2              */
/*  Module:    D1GENER.H                                  */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  980210                                    */
/*  Rev. date:  990319, KEn, DEV                              */
/*  Status:    Version 2.3                                  */
/*                                                  */
/*  Changes:                                            */
/* 99xxxx: Released in SW ver. 3.1                            */
/* 990319: New YPosxxLogo for all complex PAL patterns in PT8633.      */
/* 981109: Increased PatternType[] array from 25 to 29. New YPosxxLogo    */ 
/*        for PT8633 version.                              */
/* 981002: Released in SW ver. 3.0                            */
/* 980924: Added definitions for CCIR18, 330 & 331.                */
/* 980603: Released in SW ver. 2.0                            */
/* 980525: Added customized pattern                            */
/* 021113: Modified for TDC moving logo                                    */
/***************************************************************************/

#define UC        unsigned char
#define UI        unsigned int
#define UL        unsigned long

#define NoError      0x00
#define Buffer_OV    0x01    // Input buffer overrun
#define Illegal_Del  0x02    // Illegal delimiter
#define Illegal_Cmd  0x04    // Illegal command
#define Illegal_Par  0x08    // llegal parameter
#define IIC_NoAck    0x10    // No acknowledge from EEPROM
#define IIC_CheckSum 0x20    // Checksum error in EEPROM
#define IIC_DataErr  0x40    // Error writing/reading data to/from EEPROM
#define Error_7      0x80    //

#define TVSystemMask 0x20
#define EDHMask      0x10
#define Field2Mask   0x08

#if PT8633Version      //  !!! JK change
#define YPosPhil4x3GLogo   239  // 240 Start of reflection pulse. Actual start is +1
#define YPosPhil16x9GLogo  240  // Start of reflection pulse. Actual start is +1
#define YPosFuBKGLogo      167  // Start of reflection pulse. Actual start is +1
#else
#define YPosPhil4x3GLogo   239  // Start of reflection pulse. Actual start is +1
#define YPosPhil16x9GLogo  239  // Start of reflection pulse. Actual start is +1
#define YPosFuBKGLogo      166  // Start of reflection pulse. Actual start is +1
#endif

#define YPosPhilMLogo      201  // Start of reflection pulse. Actual start is +1

enum BooleanModes    { FALSE = 0, TRUE };
enum OffOnModes      { OFF = 0, ON };
enum TVSystemModes   { SDI625 = 0, SDI525 };

enum SDIPattern
  { 
  SDISMPTECBar = 0, SDIEBUCBar, SDIEBUCBarITU,
  SDICBar100, SDICBar75Grey, SDICBar75Red, SDICheckField,
  SDIShallowRamp, SDITimingTest, SDIBlack, SDIWindow15,
  SDIWindow20, SDIWindow100, SDICrossHatch, SDIPLUGE,
  SDIMultiburst, SDIRed75, SDIPhilips4x3,
  SDIPhilips4x3woAPAL, SDICCIR17, SDIDigAnlBlanking,
  SDIMultipulse, SDISinx, SDIDigitalGrey,
  SDIFieldTimingChk, SDIValidRamp, SDIStair5, SDIStair10,
  SDILuminanceSweep, SDIYCrCbSweep, SDIFlat100, SDIBowTie,
  SDIAltBlWh, SDIEndOfLine, SDIWhiteEndOfLine,
  SDIBlueEndOfLine, SDIRedEndOfLine, SDIYellowEndOfLine,
  SDICyanEndOfLine, SDILuminanceRamp, SDILimitRamp,
  SDILogoBar, SDIYellowGreyRamp, SDIGreyBlueRamp,
  SDICyanGreyRamp, SDIGreyRedRamp, SDICbYCrYRamp,
  SDIPhilips16x9, SDIFuBK4x3, SDIFuBK16x9, SDISafeArea,
  SDIVMT01, SDIFCCCBar, SDIFieldSquareWave,
  SDIModulatedStair5, SDICustomized1, SDICustomized2,
  SDICustom3, SDICustom4, SDICustom5, SDICustom6,
  SDICustom7, SDICustom8, SDICCIR18, SDICCIR330,
  SDICCIR331
  };

enum AudioSignals
  { 
  AudioSignalOff = 0,
  AudioStereo800Hz,
  AudioStereo1kHz,
  AudioStereoEBU1kHz,
  /*AudioStereoBBC1kHz,*/    // no BBC JK
  AudioMonoEBU1kHz,
  AudioMonoBBC1kHz,  
  AudioDual
  };

enum AudioLevels
  {
  AudioSilence = 0,
  AudioLevel0dB,
  AudioLevel9dB,
  AudioLevel12dB,
//  AudioLevel15dB,  // no -15 dB JK
  AudioLevel16dB,
  AudioLevel18dB,
  AudioLevel20dB
  };

enum AudioGroups    { AudioGroup1 = 0, AudioGroup2, AudioGroup3,
                AudioGroup4 };

enum TextStyles     { FreeTextStyle = 0, StandardTextStyle,
                ComplexTextStyle };

enum ClockStyles     { TextClockNone = 0, TextClockTime, TextClockDate,
                TextClockTimeDate };

extern volatile UC StatusPort;

extern volatile UC LinePortL;
extern volatile UC LinePortH;

extern volatile UC TextStatusPort;
extern volatile UC TextPort[512];

extern  volatile UC LowAudioRAM[11520];
extern  volatile UC LowClickAudioRAM[11520];

extern  volatile UC HighAudioRAM[11520];
extern  volatile UC HighClickAudioRAM[11520];

extern far volatile UC LowLogoRAM[22528];
extern far volatile UC HighLogoRAM[22528];

extern bit PageSelect;

extern near UC MyAddress;

extern near UL CalOffset;      // Calibration  offset
extern near UL UserOffset;      // User offset
extern near UL GenlockOffset;    // Genlock offset

extern near UI PhaseOffset;    // Calculated timing offset, ie. < 64us
extern near UI LineOffset;      // Calcualted line offset

extern near UC Status;        // Status byte

extern near UC PatternNo;      // Pattern number
extern near UC PatternAttr;    // Pattern attibute, ( ONLY Philips & FuBK)

extern bit UpdatingPatternType;  // 1: Pattern type is being updated
extern UI PatternType[29];      // Reserve 29 integers for Dynamic Patterns

extern bit EDHInsertion;      // 1: EDH insertion ON
extern bit TVSystem;          // 0: SDI525,  1: SDI625

extern near UC AudioSignal;    // Embedded audio signal selection
extern near UC AudioLevel;      // Embedded audio level selection
extern near UC AudioGroup;      // Embedded audio group selection

extern near UC TextStyle;      // Text string style information
extern near UC ClockStyle;      // Text clock style information

extern near UC ErrorStatus;    // Global error status

extern char OSDText[3][21];    // Reserve 63 Bytes for each OSD text line
extern char DateText[10];      // Reserve 10 Bytes for date information
extern char TimeText[10];      // Reserve 10 Bytes for clock information

extern bit GeneratorRunning;

extern near int LineCnt;      // Line counter

extern near UC TextStatus;      // Variable to hold group-/click-/line-offset 

extern bit AudioEnable;        // 1: Audio ON

extern bit TextLN1Enable;      // 1: Text line 1 ON
extern bit TextLN2Enable;      // 1: Text line 2 ON
extern bit TextLN3Enable;      // 1: Text line 3 ON

extern bit ClockEnable;        // 1: Clok information ON
extern bit LogoEnable;        // 1: Logo bit ON

extern near UC XPosLine1;      // Line 1 X-position
extern near int YPosLine1;      // Line 1 Y-position. Actual start is +1
extern near UC XPosLine2;      // Line 2 X-position
extern near int YPosLine2;      // Line 2 Y-position. Actual start is +1
extern near UC XPosLine3;      // Line 3 X-position.
extern near int YPosLine3;      // Line 3 Y-position. Actual start is +1

extern near int YPosLogo;      // Start of reflection pulse. Actual start is +1

extern near UC XPosTime;      // Start of time position, horizontal
extern near UC XPosDate;      // Start of date position, horizontal

extern near int Line1Cnt;      // Counter used for text line 1 insertion
extern near int Line2Cnt;      // Counter used for text line 2 insertion
extern near int Line3Cnt;      // Counter used for text line 3 insertion

extern bit Line1Enable;        // 1: Display line 1
extern bit Line2Enable;        // 1: Display line 2
extern bit Line3Enable;        // 1: Display line 3

extern bit Line1On;          // Set high in text line 1
extern bit Line2On;          // Set high in text line 2
extern bit Line3On;          // Set high in text line 3
extern bit LogoOn;          // Set high in when refplection pulse
