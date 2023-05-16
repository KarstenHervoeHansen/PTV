/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		TABLES.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960908																		*/
/*	Rev. date:	981001, KEn DEV															*/
/*	Status:		Version 2.1																	*/
/*																									*/
/*	Changes:																						*/
/* 981001: Added LOTS OF SIGNALS in AnalogPattern & SDIPattern.				*/
/* 980917: Renamed MonoBBC1KHz & AES_EBUBBC1kHz to Mono1KHz & AES_EBU1kHz  */
/* 980616: Separated AES_EBU enmu types and SDI Audio enum types. Added		*/
/*				-15dB in AES_EBULevelTable[].												*/
/* 971028: Added GenlockInputTable & GenlockSystemTable							*/
/* 970112: Added new audio levels														*/
/* 970106: Added V24CRAMAddressTable													*/
/* 961222: Added pattern-signals.														*/
/***************************************************************************/

enum AnaTVSystems		{ PAL = 0, PAL_PALID, NTSC, NTSCwo };
enum SDITVSystems   	{ SDI625 = 0, SDI525 };

enum DownloadTypes	{ DwnPresetNo = 0, DwnAllPreset = 8, DwnCompleteSPG };

enum SBPattern			{ SBBlack = 0, SBEBUCBar,  SBCBar100, SBSMPTECBar};

enum AnalogPattern   { SMPTECBar = 0, EBUCBar, CBar100, CBar75Grey,\
							  CBar75Red, Window15, Window20, Window100, CrossHatch,\
							  PLUGE, SafeArea, ShallowRamp, Multiburst, Red75,\
							  BlackBurst, Philips4x3, Philips4x3woAPAL, CCCIR17,\
							  Multipulse, Sinxx, LumSweep, FCCCBar, NTC7Combination,\
							  FCCMultiburst, Grey50, White100, FieldSquareWave, \
							  BlWh01Hz, Ramp, ModulatedRamp, Stair5, ModulatedStair5,\
							  Stair10, PulseBar, CCIR330, CCIR331, FCCComposite,\
							  NTC7Composite, kHz250, VMT01, Custom1, Custom2,\
							  Custom3, Custom4, Custom5, Custom6, Custom7, Custom8 };

enum SDIPattern      { SDISMPTECBar = 0, SDIEBUCBar, SDIEBUCbar8bit,\
							  SDICBar100, SDICBar75Grey, SDICBar75Red, SDICheck,\
							  SDIShallowramp, SDIDigTimTest, SDIBlack,\
							  SDIWindow15, SDIWindow20, SDIWindow100, SDICrossHatch,\
							  SDIPLUGE, SDIMultiburst, SDIRed75, SDIPhilips4x3,\
							  SDIPhilips4x3woAPAL, SDICCCIR17, SDIAnalogBlanking,\
							  SDIMultipulse, SDISinxx, SDIDigitalGrey,\
							  SDIFieldDelayTest, SDIValidRamp, SDIStair5, SDIStair10,\
							  SDILumSweep, SDIYCrCbSweep, SDIWhite100, SDIBowTie,\
							  SDIBlWh01Hz, SDIEOL, SDIWhiteEOL, SDIBlueEOL,\
							  SDIRedEOL, SDIYellowEOL, SDICyanEOL, SDIRamp,\
							  SDILimitRamp, SDIPulseBar, SDIYellowGreyRamp,\
							  SDIGreyBlueRamp, SDICyanGreyRamp, SDIGreyRedRamp,\
							  SDIPhilip16x9, SDIFuBK4x3, SDIFuBK16x9, SDISafeArea,\
							  SDIVMT01, SDIFCCCBar, SDIFieldSquareWave,\
							  SDIModulatedStair5, SDICustom1, SDICustom2,\
							  SDICustom3, SDICustom4, SDICustom5, SDICustom6,\
							  SDICustom7, SDICustom8 };

enum EMBAudio			{ EMBAudioOff = 0, EMBAudioSilence };

enum GenlockInput		{ GenlockA = 0, GenlockB, LoopThrough, SDIGenlock,\
							  Internal, Internal2 };

enum GenlockSystem	{ BurstLockPAL = 0, BurstLockNTSC, SyncLock625,\
							  SyncLock525, SDI625Lock, SDI525Lock, MHzLock358,\
							  MHzLock443, MHzLock5, MHzLock10 };

enum AudioSignals  	{ AudioSignalOff = 0, AudioStereo800Hz,\
							  AudioStereo1kHz, AudioStereoEBU1kHz,\
							  AudioStereoBBC1kHz, AudioMonoEBU1kHz,\
							  AudioMono1kHz, AudioDual};

enum AudioLevels     { AudioSilence = 0, AudioLevel0dB, AudioLevel9dB, \
							  AudioLevel12dB, AudioLevel14dB, AudioLevel16dB, \
							  AudioLevel18dB, AudioLevel20dB };

enum AES_EBUSignals 	{ AES_EBUStereo800Hz, AES_EBUStereo1kHz,\
							  AES_EBUStereoEBU1kHz, AES_EBUStereoBBC1kHz,\
							  AES_EBUMonoEBU1kHz, AES_EBUMono1kHz, AES_EBUDual,\
							  AES_EBU48kHz };

enum AES_EBULevels   { AES_EBUSilence = 0, AES_EBULevel0dB, AES_EBULevel9dB, \
							  AES_EBULevel12dB, AES_EBULevel14dB, AES_EBULevel15dB, \
							  AES_EBULevel16dB, AES_EBULevel18dB, AES_EBULevel20dB };

enum AES_EBUTiming  	{ AES_EBUPAL = 0, AES_EBUNTSCPhase1, AES_EBUNTSCPhase2,\
							  AES_EBUNTSCPhase3, AES_EBUNTSCPhase4,\
							  AES_EBUNTSCPhase5 };

extern code UC GenlockInputTable[12];
extern code UC GenlockSystemTable[20];

extern code UC V24CRAMAddressTable[7];

extern code UI AES_EBUSignalTable[8];
extern code UI AES_EBULevelTable[9];
extern code UI AES_EBUTimingTable[6];
