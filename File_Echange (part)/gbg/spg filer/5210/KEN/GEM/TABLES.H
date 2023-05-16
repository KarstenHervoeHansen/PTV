/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		TABLES.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960908																		*/
/*	Rev. date:	960112, KEn DEV															*/
/*	Status:		Version 0.01																*/
/*																									*/
/*	Changes:																						*/
/* 970112: Added new audio levels														*/
/* 970106: Added V24CRAMAddressTable													*/
/* 961222: Added pattern-signals.														*/
/***************************************************************************/

enum AnaTVSystems		{ PAL = 0, PAL_PALID, NTSC, NTSCwo };
enum SDITVSystems   	{ SDI625 = 0, SDI525 };

enum DownloadTypes	{ DwnPresetNo = 0, DwnAllPreset = 8, DwnCompleteSPG };

enum RS232Baudrates	{ Baud300 = 0, Baud600, Baud1200, Baud2400, Baud4800,
							  Baud9600 };

enum RS2332Parity  	{ None = 0, Odd, Even };

enum RS2332Databit  	{ Data7 = 7, Data8 };

enum RS232Handshake	{ XON_XOFF = 0, RTS_CTS };

enum SBPattern			{ SBBlack = 0, SBEBUCBar,  SBCBar100, SBSMPTECBar};

enum AnalogPattern   { SMPTECBar = 0, EBUCBar, CBar100, CBar75Grey,\
							  CBar75Red, Window15, Window20, Window100, CrossHatch,\
							  PLUGE, SafeArea, ShallowRamp, Multiburst, Red75,\
							  BlackBurst };

enum SDIPattern      { SDISMPTECBar = 0, SDIEBUCBar, SDIEBUCbar8bit,\
							  SDICBar100, SDICBar75Grey, SDICBar75Red, SDICheck,\
							  SDIShallowramp, SDIDigTimTest, SDIBlack,\
							  SDIWindow15, SDIWindow20, SDIWindow100, SDICrossHatch,\
							  SDIPLUGE, SDIMultiburst, SDIRed75 };

enum EMBAudio			{ EMBAudioOff = 0, EMBAudioSilence };

enum GenlockInput		{ GenlockA = 0, GenlockB, LoopThrough, SDIGenlock,\
							  Internal, Internal2 };

enum GenlockSystem	{ BurstLockPAL = 0, BurstLockNTSC, SyncLock625,\
							  SyncLock525, SDI625Lock, SDI525Lock, MHzLock358,\
							  MHzLock443, MHzLock5, MHzLock10 };

enum AudioSignals  	{ AudioSignalOff = 0, AudioStereo800Hz,\
							  AudioStereo1kHz, AudioStereoEBU1kHz,\
							  AudioStereoBBC1kHz, AudioMonoEBU1kHz,\
							  AudioMonoBBC1kHz, AudioDual, Audio48kHz };

enum AudioLevels     { AudioSilence = 0, AudioLevel0dB, AudioLevel9dB, \
							  AudioLevel12dB, AudioLevel14dB, AudioLevel16dB, \
							  AudioLevel18dB, AudioLevel20dB };

enum AudioTiming   	{ AudioPAL = 0, AudioNTSCPhase1, AudioNTSCPhase2,\
							  AudioNTSCPhase3, AudioNTSCPhase4, AudioNTSCPhase5 };

extern code UC V24CRAMAddressTable[7];

extern code UI AES_EBUSignalTable[9];
extern code UI AES_EBULevelTable[8];
extern code UI AES_EBUTimingTable[6];

extern code int RS232BaudrateTable[6];
extern code int RS232DelayTable[6];
