/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT8602/3 SDI Digital test signal generator						*/
/*	Module:		D1GENER.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960614																		*/
/*	Rev. date:	990614, KEn DEV															*/
/*	Status:		Version 1.2																	*/
/*																									*/
/*	Changes:																						*/
/* 990614: StartOfText now defined extern												*/
/* 980929: Removed SWVersion to file: version.h. Added new signal 			*/
/*				definitions																		*/
/* 970429: Introduced IIC_DataErr														*/
/***************************************************************************/

#define UC				unsigned char
#define UI				unsigned int
#define UL				unsigned long

#define NoError		0x00
#define Buffer_OV		0x01		// Input buffer overrun
#define Illegal_Del	0x02		// Illegal delimiter
#define Illegal_Cmd	0x04		// Illegal command
#define Illegal_Par	0x08		// llegal parameter
#define IIC_NoAck		0x10		// No acknowledge from EEPROM
#define IIC_CheckSum	0x20		// Checksum error in EEPROM
#define IIC_DataErr	0x40		// Error writing/reading data to/from EEPROM
#define Error_7		0x80		//

#define TVSystemMask	0x20
#define EDHMask		0x10
#define Field2Mask	0x08

enum BooleanModes		{ FALSE = 0, TRUE };
enum OffOnModes		{ OFF = 0, ON };
enum TVSystemModes	{ PALG = 0, NTSC };

enum SDIPattern		{ SDISMPTECBar = 0, SDIEBUCBar, SDIEBUCBar8bit,\
							  SDICBar100, SDICBar75Grey, SDICBar75Red, SDICheck,\
							  SDIShallowRamp, SDIDigTimTest, SDIBlack, SDIWindow15,\
							  SDIWindow20, SDIWindow100, SDICrossHatch, SDIPLUGE,\
							  SDIMultiburst, SDIRed75, SDIPhilips4x3,
							  SDIPhilips4x3woAPAL, SDICCIR17, SDIDigAnlBlanking,
							  SDIMultipulse, SDISinx, SDIDigitalGrey,
							  SDIFieldTimingChk, SDIValidRamp, SDIStair5, SDIStair10,
							  SDILuminanceSweep, SDIYCrCbSweep, SDIFlat100, SDIBowTie,
							  SDIAltBlWh, SDIEndOfLine, SDIWhiteEndOfLine,
							  SDIBlueEndOfLine, SDIRedEndOfLine, SDIYellowEndOfLine,
							  SDICyanEndOfLine, SDILuminanceRamp, SDILimitRamp,
							  SDIPulseBar, SDIYellowGreyRamp, SDIGreyBlueRamp,
							  SDICyanGreyRamp, SDIGreyRedRamp, SDICbYCrYRamp,
							  SDIPhilips16x9, SDIFuBK4x3, SDIFuBK16x9, SDISafeArea,
							  SDIVMT01, SDIFCCCBar, SDIFieldSquareWave,
							  SDIModulatedStair5, SDICustomized1, SDICustomized2,
							  SDICustom3, SDICustom4, SDICustom5, SDICustom6,
							  SDICustom7, SDICustom8, SDICCIR18, SDICCIR330,
							  SDICCIR331 };

enum AudioSignals	  	{ AudioSignalOff = 0, AudioStereo800Hz,\
							  AudioStereo1kHz, AudioStereoEBU1kHz,\
							  AudioStereoBBC1kHz, AudioMonoEBU1kHz,\
							  AudioMonoBBC1kHz, AudioDual };

enum AudioLevels		{ AudioSilence = 0, AudioLevel0dB, AudioLevel9dB, \
							  AudioLevel12dB, AudioLevel14dB, AudioLevel16dB, \
							  AudioLevel18dB, AudioLevel20dB };

extern volatile UC StatusPort;
extern volatile UC LinePortL;
extern volatile UC LinePortH;
extern volatile UC TextStatusPort;
extern volatile UC TextPort[256];

extern volatile UC LowSoundRAM[11520];
extern volatile UC LowClickSoundRAM[11520];
extern volatile UC HighSoundRAM[11520];
extern volatile UC HighClickSoundRAM[11520];

extern bit PageSelect;

extern near UL CalOffset;			// Calibration offset
extern near UL UserOffset;			// User offset
extern near UL GenlockOffset;		// Genlock offset

extern near UI PhaseOffset;		// Calculated timing offset, ie. < 64us
extern near UI LineOffset;			// Calcualted line offset

extern near UC Status;

extern near UC PatternNo;

extern bit EDHInsertion;
extern bit TVSystem;

extern near UC AudioSignal;		// Embedded audio signal selection
extern near UC AudioLevel;			// Embedded audio level selection

extern near UC ErrorStatus;

extern near int LineCnt;			// Line counter
extern bit SoundEnable;

extern near char OSDText[9];
extern near UC first_line_w_text;		// Start of text insertion. Actual start is +1
extern bit TextEnable;

extern near UC TextStatus;

extern bit GeneratorRunning;