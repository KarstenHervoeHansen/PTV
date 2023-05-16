/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		RS232CMD.H																	*/
/*	Author:		Kim Engedahl, VDE															*/
/*	Org. date:	960613																		*/
/*	Rev. date:	981001, KEn VDE															*/
/*	Status:		Version 2.2																	*/
/*																									*/
/*	Changes:																						*/
/* 981001 Increased number of patterns in Analog-/SDI-PatternPar.				*/
/* 980616 Added support for -15dB level in PT8635									*/
/* 970820 Added command: FactoryMainPSCalib()										*/
/* 970717 FactoryMainPSN5() and  FactoryMainPS12() implemented					*/
/* 970506 Altered SDIPatternPar[18] to SDIPatternPar[17]							*/
/***************************************************************************/

typedef struct CMD_STRUCT {
	code char* LongName;
	code char* ShortName;

	code struct CMD_STRUCT* SameLevel;
	code struct CMD_STRUCT* NextLevel;

	void ( *Suffix)();
	void ( *Function)();
};

typedef struct PAR_STRUCT {
	code char* LongName;
	code char* ShortName;

	code struct PAR_STRUCT* SameLevel;
};

extern code char IDNResponseCmdTxt[];
extern code char DownloadResponseCmdTxt[];

extern code struct CMD_STRUCT Response[1];

extern code struct CMD_STRUCT Mandated[10];
extern code struct CMD_STRUCT Subsystem[7];
extern code struct CMD_STRUCT System[5];
extern code struct CMD_STRUCT SystemLevel1[5];
extern code struct CMD_STRUCT Status[4];
extern code struct CMD_STRUCT StatusLevel1[4];
extern code struct CMD_STRUCT StatusLevel1_1[3];
extern code struct CMD_STRUCT StatusLevel1_2[3];
extern code struct CMD_STRUCT Diagnostic[2];
extern code struct CMD_STRUCT DiagnosticLevel1[1];
extern code struct CMD_STRUCT Display[1];
extern code struct CMD_STRUCT Input[2];
extern code struct CMD_STRUCT GenlockLevel1[3];
extern code struct CMD_STRUCT GenlockLevel2[1];
extern code struct CMD_STRUCT Output[7];
extern code struct CMD_STRUCT BBLevel1[5];
extern code struct CMD_STRUCT BBLevel2[3];
extern code struct CMD_STRUCT SBLevel1[7];
extern code struct CMD_STRUCT SBLevel2[3];
extern code struct CMD_STRUCT AnalogLevel1[6];
extern code struct CMD_STRUCT AnalogLevel2[3];
extern code struct CMD_STRUCT SDILevel1[7];
extern code struct CMD_STRUCT SDILevel2[3];
extern code struct CMD_STRUCT SDILevel3[2];
extern code struct CMD_STRUCT AudioLevel1[4];
extern code struct CMD_STRUCT TimeCodeLevel1[1];
extern code struct CMD_STRUCT BBMultiLevel1[1];
extern code struct CMD_STRUCT Factory[10];
extern code struct CMD_STRUCT FactoryLevel1[2];
extern code struct CMD_STRUCT FactoryLevel2[3];
extern code struct CMD_STRUCT FactoryLevel3[1];
extern code struct CMD_STRUCT FactoryLevel4[3];
extern code struct CMD_STRUCT FactoryLevel5[4];
extern code struct CMD_STRUCT FactoryLevel6[1];
extern code struct CMD_STRUCT FactoryLevel7[1];
extern code struct CMD_STRUCT FactoryLevel8[1];
extern code struct CMD_STRUCT FactoryLevel9[1];

extern code struct PAR_STRUCT GenlockInputPar[6];
extern code struct PAR_STRUCT GenlockSystemPar[10];
extern code struct PAR_STRUCT AnalogPatternPar[33];
extern code struct PAR_STRUCT AnalogTVSystemPar[3];
extern code struct PAR_STRUCT DigitalTVSystemPar[2];
extern code struct PAR_STRUCT SDIBlkPatternPar[4];
extern code struct PAR_STRUCT SDIPatternPar[28];
extern code struct PAR_STRUCT OffOnPar[2];
extern code struct PAR_STRUCT OffSilencePar[2];
extern code struct PAR_STRUCT AudioSignalPar[8];
extern code struct PAR_STRUCT AudioLevelPar[8];
extern code struct PAR_STRUCT AES_EBUSignalPar[8];
extern code struct PAR_STRUCT AES_EBULevelPar[9];
extern code struct PAR_STRUCT AES_EBUTimingPar[6];
extern code struct PAR_STRUCT FactoryAddressPar[7];

extern void IDNResponseMessage();

extern void CLSCommand();
extern void ESECommand();
extern void ESRCommand();
extern void OPCCommand();
extern void SRECommand();
extern void STBCommand();
extern void TSTCommand();
extern void WAICommand();
extern void RSTCommand();
extern void IDNCommand();

extern void SystemError();
extern void SystemVersion();

extern void PresetSuffix();
extern void SystemPreset();
extern void SystemPresetStore();
extern void SystemPresetName();
extern void SystemPresetDownload();
extern void SystemPresetUpload();
extern void SystemDownload();
extern void SystemUpload();

extern void StatusOperEvent();
extern void StatusOperCondition();
extern void StatusOperEnable();
extern void StatusQuestEvent();
extern void StatusQuestCondition();
extern void StatusQuestEnable();
extern void StatusPreset();
extern void StatusPTSuffix();
extern void StatusPT();

extern void DiagnosticDisplay();
extern void DiagnosticErrorQueue();
extern void DiagnosticErrorQueueReset();
extern void DisplayContrast();

extern void Genlock();
extern void GenlockInput();
extern void GenlockSystem();
extern void GenlockDelay();

extern void BBSuffix();
extern void BB();
extern void BBSystem();
extern void BBScHPhase();
extern void BBDelay();
extern void BBCopy();
extern void BBVersion();

extern void SBSuffix();
extern void SB();
extern void SBPattern();
extern void SBSystem();
extern void SBEDHInsert();
extern void SBEmbAudio();
extern void SBDelay();
extern void SBCopy();
extern void SBVersion();

extern void Analog();
extern void AnalogPattern();
extern void AnalogTextInsert();
extern void AnalogSystem();
extern void AnalogScHPhase();
extern void AnalogDelay();
extern void AnalogVersion();

extern void SDI();
extern void SDIPattern();
extern void SDITextInsert();
extern void SDISystem();
extern void SDIEDHInsert();
extern void SDIDelay();
extern void SDIVersion();

extern void SDIAudioSignal();
extern void SDIAudioLevel();

extern void Audio();
extern void AudioSignal();
extern void AudioLevel();
extern void AudioTiming();
extern void AudioVersion();

extern void TimeCodeVersion();
extern void BBMultiVersion();
extern void SDIGenlockVersion();

extern void FactoryPassword();
extern void FactoryAddress();
extern void FactoryCommand();

extern void FactorySPGUserText();
extern void FactorySPGStore();
extern void FactorySPGRead();
extern void FactoryAudioUserText();
extern void FactoryAudioRead();
extern void FactoryAudioWrite();
extern void FactoryMainUserText();
extern void FactoryMainPSNSuffix();
extern void FactoryMainPSN();
extern void FactoryMainPSSuffix();
extern void FactoryMainPS();
extern void FactoryMainPSCalib();
extern void FactorySDIGenlockUserText();
extern void FactoryTimeCodeUserText();
extern void FactoryMultiBBUserText();
extern void FactoryADCRead();
extern void FactoryLDetectorRead();

extern void FindDelayParameters();
extern UC FindParameter( char* par, code struct PAR_STRUCT* TmpPtr);

extern UC WriteV24Version( UC address, char unit);
extern UC WriteIICVersion( UC bus, UC address);
