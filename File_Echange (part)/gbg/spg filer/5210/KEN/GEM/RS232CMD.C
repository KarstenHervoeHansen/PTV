/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		RS232CMD.C51																*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960713																		*/
/*	Rev. date:	970724, KEn DEV															*/
/*	Status:		Version 0.2 																*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*																									*/
/*	Changes:																						*/
/* 970724 FindParameter did not use par from parameter list						*/
/* 970717 FactoryMainPSN5() and  FactoryMainPS12() implemented					*/
/*			 FactoryMainPSSuffix() & FactoryMainPSNSuffix() implemented			*/
/* 970630 WAICommand(), TSTCommand() & OPCCommand() implemented				*/
/*			 DisplayContrast() implemented												*/
/*			 StatusOperEvent(), StatusQuestEvent() implemented						*/
/*			 StatusQuestEnable() & StatusOperEnable() implemented					*/
/*			 StatusQuestCondition() & StatusOperCondition() implemented			*/
/* 970627 SRECommand(), ESECommand() & ESRCommand() implemented				*/
/*			 STBCommand() implemented														*/
/* 970626 GenlockSystem() now performs a system check								*/
/*			 FactoryPassword has been coded												*/
/* 970625 String-data in SYST:PRES:NAME?, OUTP:ASIG:TEXT?;, OUTP:ASIG?,		*/
/*			  OUTP:SDIS:TEXT? and OUTP:SDIS? is now enclosed in quotes			*/
/*			 SystemPresetName(), AnalogTextInsert() & SDITextInsert() now has	*/
/*			  test if string is valid.														*/
/*			 WriteIICVersion: SW version NA altered to 0, ZERO						*/
/*			 DBMnFS has changed to DBnFS													*/
/*			 Pattern BLACK in ANLS has changed to BLACKBURST						*/
/*			 INP:GENL:DEL? now returns +0,+0,+0 for INTERNAL(2) genlock			*/
/*			 INP:GENL? now returns full response										*/
/*			 SBPattern() & SDIPattern() & AnalogPattern() now prohibits			*/
/*			  patterns NOT selectable in selected system								*/
/* 970506 Removed Safe Area from SDI Signal list									*/
/* 970224 Could not write to SDI genlock												*/
/* 970218 Added new state in FactoryCommand											*/
/* 970216 Generel update of all routines and addition of new routines		*/
/* 970206 Added password protection														*/
/* 970129 Added errorcode: HardwareNotpresentErr									*/
/* 970122 Added Audio:Read, ADC:Read & levreldetector:read						*/
/* 970109 Problems with suffix in BB & SB corrected								*/
/*			 Added usertext in main ram													*/
/* 970106 Remove FactoryReset																*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mas.h"

#include "52sync.h"
#include "unit_drv.h"
#include "serial1.h"
#include "util.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "rs232par.h"
#include "rs232err.h"
#include "rs232cmd.h"
#include "tables.h"
#include "timing.h"
#include "text.h"
#include "menu.h"
#include "menutree.h"
#include "piic.h"
#include "iniic.h"
#include "message.h"
#include "cont_drv.h"
#include "keyb_drv.h"

#define DownloadOffset	(UC) 20

code char SystemVersionTxt[] = "1995.0";
code char FactoryPasswordTxt[] = "CWh_bod";

code char IDNResponseCmdTxt[] = "*IDN?";
code char DownloadResponseCmdTxt[] = ":SYST:PRES:DOWN ";

code char NoErrorsTxt[]			= "No errors";
code char ActiveErrorsTxt[]	= "Active error";
code char NoActiveErrorsTxt[]	= "No active error";

code char CLSCmdTxt[] = "CLS";
code char ESECmdTxt[] = "ESE";
code char ESRCmdTxt[] = "ESR";
code char OPCCmdTxt[] = "OPC";
code char SRECmdTxt[] = "SRE";
code char STBCmdTxt[] = "STB";
code char TSTCmdTxt[] = "TST";
code char WAICmdTxt[] = "WAI";
code char RSTCmdTxt[] = "RST";;
code char IDNCmdTxt[] = "IDN";

code char SystemCmdTxt[] = "SYSTEM";
code char SystCmdTxt[] = "SYST";
code char StatusCmdTxt[] = "STATUS";
code char StatCmdTxt[] = "STAT";
code char PTCmdTxt[] = "PT";
code char DiagnosticCmdTxt[] = "DIAGNOSTIC";
code char ErrorQueueCmdTxt[] = "ERRORQUEUE";
code char ResetCmdTxt[] = "RESET";
code char ResCmdTxt[] = "RES";
code char DiagCmdTxt[] = "DIAG";
code char DisplayCmdTxt[] = "DISPLAY";
code char DispCmdTxt[] = "DISP";
code char InputCmdTxt[] = "INPUT";
code char InpCmdTxt[] = "INP";
code char OutputCmdTxt[] = "OUTPUT";
code char OutpCmdTxt[] = "OUTP";
code char FactoryCmdTxt[] = "FACTORY";
code char FactCmdTxt[] = "FACT";

code char ErrorCmdTxt[] = "ERROR";
code char ErrCmdTxt[] = "ERR";
code char VersionCmdTxt[] = "VERSION";
code char VersCmdTxt[] = "VERS";
code char DownloadCmdTxt[] = "DOWNLOAD";
code char DownCmdTxt[] = "DOWN";
code char UploadCmdTxt[] = "UPLOAD";
code char UplCmdTxt[] = "UPL";
code char NameCmdTxt[] = "NAME";
code char RecallCmdTxt[] = "RECALL";
code char RecCmdTxt[] = "REC";

code char OperationCmdTxt[] = "OPERATION";
code char OperCmdTxt[] = "OPER";
code char QuestionableCmdTxt[] = "QUESTIONABLE";
code char QuesCmdTxt[] = "QUES";
code char PresetCmdTxt[] = "PRESET";
code char PresCmdTxt[] = "PRES";

code char EventCmdTxt[] = "EVENT";
code char EvenCmdTxt[] = "EVEN";
code char ConditionCmdTxt[] = "CONDITION";
code char CondCmdTxt[] = "COND";
code char EnableCmdTxt[] = "ENABLE";
code char EnabCmdTxt[] = "ENAB";

code char ContrastCmdTxt[] = "CONTRAST";
code char ContCmdTxt[] = "CONT";

code char GenlockCmdTxt[] = "GENLOCK";
code char GenlCmdTxt[] = "GENL";

code char BBCmdTxt[] = "BB";
code char BBMultiCmdTxt[] = "BBMULTI";
code char BBMCmdTxt[] = "BBM";
code char DelayCmdTxt[] = "DELAY";
code char DelCmdTxt[] = "DEL";
code char ScHPhaseCmdTxt[] = "SCHPHASE";
code char ScHPCmdTxt[] = "SCHP";

code char CopyCmdTxt[] = "COPY";

code char SBCmdTxt[] = "SB";
code char PatternCmdTxt[] = "PATTERN";
code char PattCmdTxt[] = "PATT";
code char EDHInsertCmdTxt[] = "EDHINSERT";
code char EDHCmdTxt[] = "EDH";
code char EmbAudioCmdTxt[] = "EMBAUDIO";
code char EmbCmdTxt[] = "EMB";

code char ASignalCmdTxt[] = "ASIGNAL";
code char ASigCmdTxt[] = "ASIG";

code char SDISignalCmdTxt[] = "SDISIGNAL";
code char SDISCmdTxt[] = "SDIS";
code char TextCmdTxt[] = "TEXT";
code char TextInsertCmdTxt[] = "TEXTINSERT";

code char AudioCmdTxt[] = "AUDIO";
code char AudCmdTxt[] = "AUD";
code char SignalCmdTxt[] = "SIGNAL";
code char SignCmdTxt[] = "SIGN";
code char LevelCmdTxt[] = "LEVEL";
code char LevCmdTxt[] = "LEV";
code char TimingCmdTxt[] = "TIMING";
code char TimCmdTxt[] = "TIM";

code char TimeCodeCmdTxt[] = "TIMECODE";

code char PasswordCmdTxt[] = "PASSWORD";
code char PASSCmdTxt[] = "PASS";
code char V24CircuitCmdTxt[] = "V24CIRCUIT";
code char V24CCmdTxt[] = "V24C";
code char AddressCmdTxt[] = "ADDRESS";
code char AddrCmdTxt[] = "ADDR";
code char CommandCmdTxt[] = "COMMAND";
code char CommCmdTxt[] = "COMM";

code char SPGGeneratorCmdTxt[] = "SPGGENERATOR";
code char SPGGCmdTxt[] = "SPGG";
code char TextStringCmdTxt[] = "TEXTSTRING";
code char StoreCmdTxt[] = "STORE";
code char StorCmdTxt[] = "STOR";
code char ReadCmdTxt[] = "READ";
code char WriteCmdTxt[] = "WRITE";
code char WritCmdTxt[] = "WRIT";
code char SDIGenlockCmdTxt[] = "SDIGENLOCK";
code char SDIGCmdTxt[] = "SDIG";
code char MainboardCmdTxt[] = "MAINBOARD";
code char MainCmdTxt[] = "MAIN";
code char PSN5CmdTxt[] = "PSN";
code char PS12CmdTxt[] = "PS";
code char MultiBBCmdTxt[] = "MULTIBB";
code char MultCmdTxt[] = "MULT";
code char ADCCmdTxt[] = "ADC";
code char LDetectorCmdTxt[] = "LDETECTOR";
code char LDETCmdTxt[] = "LDET";

code char AParTxt[] = "A";
code char BParTxt[] = "B";
code char ABParTxt[] = "A_B";
code char SDIParTxt[] = "SDI";
code char InternalParTxt[] = "INTERNAL";
code char IntParTxt[] = "INT";
code char Internal2ParTxt[] = "INTERNAL2";
code char Int2ParTxt[] = "INT2";

code char PALBurstParTxt[] = "PALBURST";
code char PALBParTxt[] = "PALB";
code char NTSCBurstParTxt[] = "NTSCBURST";
code char NTSCParTxt[] = "NTSC";
code char Sync625ParTxt[] = "SYNC625";
code char Sync525ParTxt[] = "SYNC525";
code char SDI625ParTxt[] = "SDI625";
code char SDI525ParTxt[] = "SDI525";
code char F358MHzParTxt[] = "F358MHZ";
code char F443MHzParTxt[] = "F443MHZ";
code char F5MHzParTxt[] = "F5MHZ";
code char F10MHzParTxt[] = "F10MHZ";

code char PALParTxt[] = "PAL";
code char PAL_IDParTxt[] = "PAL_ID";

code char BlackParTxt[] = "BLACK";
code char BlacParTxt[] = "BLAC";
code char CBEBUParTxt[] = "CBEBU";
code char CBEBParTxt[] = "CBEB";
code char CBEBU8ParTxt[] = "CBEBU8";
code char CBEB8ParTxt[] = "CBEB8";
code char CB100ParTxt[] = "CB100";
code char CBGrey75ParTxt[] = "CBGREY75";
code char CBGR75ParTxt[] = "CBGR75";
code char CBRed75ParTxt[] = "CBRED75";
code char CBR75ParTxt[] = "CBR75";
code char SafeAreaParTxt[] = "SAFEAREA";
code char SafParTxt[] = "SAF";
code char SDICheckParTxt[] = "SDICHECK";
code char SDICParTxt[] = "SDIC";
code char ShallowRampParTxt[] = "SHALLOWRAMP";
code char SHALParTxt[] = "SHAL";
code char DTimingParTxt[] = "DTIMING";
code char DTIMParTxt[] = "DTIM";
code char WIN15ParTxt[] = "WIN15";
code char WIN20ParTxt[] = "WIN20";
code char WIN100ParTxt[] = "WIN100";
code char CrossHatchParTxt[] = "CROSSHATCH";
code char CROSParTxt[] = "CROS";
code char PlugeParTxt[] = "PLUGE";
code char PLUGParTxt[] = "PLUG";
code char MultiBurstParTxt[] = "MULTIBURST";
code char MULTParTxt[] = "MULT";
code char Red75ParTxt[] = "RED75";
code char BlackBurstParTxt[] = "BLACKBURST";

code char CBSMPTEParTxt[] = "CBSMPTE";
code char CBSMParTxt[] = "CBSM";

code char OFFParTxt[] = "OFF";
code char ONParTxt[] = "ON";

code char SilenceParTxt[] = "SILENCE";
code char SilParTxt[] = "SIL";

code char S800HZParTxt[] = "S800HZ";
code char S1KHZParTxt[] = "S1KHZ";
code char SEBU1KHZParTxt[] = "SEBU1KHZ";
code char SBBC1KHZParTxt[] = "SBBC1KHZ";
code char MEBU1KHZParTxt[] = "MEBU1KHZ";
code char MBBC1KHZParTxt[] = "MBBC1KHZ";
code char DUALParTxt[] = "DUAL";
code char F48KHZParTxt[] = "F48KHZ";

code char DB0FSParTxt[] = "DB0FS";
code char DB9FSParTxt[] = "DB9FS";
code char DB12FSParTxt[] = "DB12FS";
code char DB14FSParTxt[] = "DB14FS";
code char DB16FSParTxt[] = "DB16FS";
code char DB18FSParTxt[] = "DB18FS";
code char DB20FSParTxt[] = "DB20FS";

code char NTSC1ParTxt[] = "NTSC1";
code char NTSC2ParTxt[] = "NTSC2";
code char NTSC3ParTxt[] = "NTSC3";
code char NTSC4ParTxt[] = "NTSC4";
code char NTSC5ParTxt[] = "NTSC5";

code char bbu_0ParTxt[] = "BBU_0";
code char bbu_1ParTxt[] = "BBU_1";
code char bbu_2ParTxt[] = "BBU_2";
code char bbu_3ParTxt[] = "BBU_3";
code char spgParTxt[] = "SPG";
code char AnlSignalParTxt[] = "ANLSIGNAL";
code char AnlSParTxt[] = "ANLS";
code char SDISignalParTxt[] = "SDISIGNAL";
code char SDISParTxt[] = "SDIS";

code char MinParTxt[] = "MIN";
code char MaxParTxt[] = "MAX";

code struct CMD_STRUCT Response[1] = {
	{ IDNResponseCmdTxt, NULL, NULL, NULL, NONE, IDNResponseMessage},
};

code struct CMD_STRUCT Mandated[10] = {
	{ CLSCmdTxt, NULL, &Mandated[1], NULL, NONE, CLSCommand},
	{ ESECmdTxt, NULL, &Mandated[2], NULL, NONE, ESECommand},
	{ ESRCmdTxt, NULL, &Mandated[3], NULL, NONE, ESRCommand},
	{ OPCCmdTxt, NULL, &Mandated[4], NULL, NONE, OPCCommand},
	{ SRECmdTxt, NULL, &Mandated[5], NULL, NONE, SRECommand},
	{ STBCmdTxt, NULL, &Mandated[6], NULL, NONE, STBCommand},
	{ TSTCmdTxt, NULL, &Mandated[7], NULL, NONE, TSTCommand},
	{ WAICmdTxt, NULL, &Mandated[8], NULL, NONE, WAICommand},
	{ RSTCmdTxt, NULL, &Mandated[9], NULL, NONE, RSTCommand},
	{ IDNCmdTxt, NULL, NULL, NULL, NONE, IDNCommand},
};

code struct CMD_STRUCT Subsystem[7] = {
	{ SystemCmdTxt, SystCmdTxt, &Subsystem[1], &System[0], NONE, NONE},
	{ StatusCmdTxt, StatCmdTxt, &Subsystem[2], &Status[0], NONE, NONE},
	{ DiagnosticCmdTxt, DiagCmdTxt, &Subsystem[3], &Diagnostic[0], NONE, NONE},
	{ DisplayCmdTxt, DispCmdTxt, &Subsystem[4], &Display[0], NONE, NONE},
	{ InputCmdTxt, InpCmdTxt, &Subsystem[5], &Input[0], NONE, NONE},
	{ OutputCmdTxt, OutpCmdTxt, &Subsystem[6], &Output[0], NONE, NONE},
	{ FactoryCmdTxt, FactCmdTxt, NULL, &Factory[0], NONE, NONE},
};

//**************************************************************************
code struct CMD_STRUCT System[5] = {
	{ ErrorCmdTxt, ErrCmdTxt, &System[1], NULL, NONE, SystemError},
	{ VersionCmdTxt, VersCmdTxt, &System[2], NULL, NONE, SystemVersion},
	{ PresetCmdTxt, PresCmdTxt, &System[3], &SystemLevel1[0], NONE, SystemPreset},
	{ DownloadCmdTxt, DownCmdTxt, &System[4], NULL, NULL, SystemDownload},
	{ UploadCmdTxt, UplCmdTxt, NULL, NULL, NULL, SystemUpload}
};

//**************************************************************************
code struct CMD_STRUCT SystemLevel1[5] = {
	{ RecallCmdTxt, RecCmdTxt, &SystemLevel1[1], NULL, NONE, SystemPreset},
	{ StoreCmdTxt, StorCmdTxt, &SystemLevel1[2], NULL, NONE, SystemPresetStore},
	{ NameCmdTxt, NameCmdTxt, &SystemLevel1[3], NULL, NONE, SystemPresetName},
	{ DownloadCmdTxt, DownCmdTxt, &SystemLevel1[4], NULL, NONE, SystemPresetDownload},
	{ UploadCmdTxt, UplCmdTxt, NULL, NULL, NONE, SystemPresetUpload}
};

//**************************************************************************
code struct CMD_STRUCT Status[4] = {
	{ OperationCmdTxt, OperCmdTxt, &Status[1], &StatusLevel1_1[0], NONE, StatusOperEvent},
	{ QuestionableCmdTxt, QuesCmdTxt, &Status[2], &StatusLevel1_2[0], NONE, StatusQuestEvent},
	{ PresetCmdTxt, PresCmdTxt, &Status[3], NULL, NONE, StatusPreset},
	{ PTCmdTxt, PTCmdTxt, NULL, NULL, StatusPTSuffix, StatusPT},
};

code struct CMD_STRUCT StatusLevel1_1[3] = {
	{ EventCmdTxt, EvenCmdTxt, &StatusLevel1_1[1], NULL, NONE, StatusOperEvent},
	{ ConditionCmdTxt, CondCmdTxt, &StatusLevel1_1[2], NULL, NONE, StatusOperCondition},
	{ EnableCmdTxt, EnabCmdTxt, NULL, NULL, NONE, StatusOperEnable},
};

code struct CMD_STRUCT StatusLevel1_2[3] = {
	{ EventCmdTxt, EvenCmdTxt, &StatusLevel1_2[1], NULL, NONE, StatusQuestEvent},
	{ ConditionCmdTxt, CondCmdTxt, &StatusLevel1_2[2], NULL, NONE, StatusQuestCondition},
	{ EnableCmdTxt, EnabCmdTxt, NULL, NULL, NONE, StatusQuestEnable},
};

//**************************************************************************
code struct CMD_STRUCT Diagnostic[2] = {
	{ DisplayCmdTxt, DispCmdTxt, &Diagnostic[1], NULL, NONE, DiagnosticDisplay},
	{ ErrorQueueCmdTxt, ErrCmdTxt, NULL, &DiagnosticLevel1[0], NONE, DiagnosticErrorQueue},
};

//**************************************************************************
code struct CMD_STRUCT DiagnosticLevel1[1] = {
	{ ResetCmdTxt, ResCmdTxt, NULL, NULL, NONE, DiagnosticErrorQueueReset},
};

//**************************************************************************
code struct CMD_STRUCT Display[1] = {
	{ ContrastCmdTxt, ContCmdTxt, NULL, NULL, NONE, DisplayContrast},
};

//**************************************************************************
code struct CMD_STRUCT Input[2] = {
	{ GenlockCmdTxt, GenlCmdTxt, &Input[1], &GenlockLevel1[0], NONE, Genlock},
	{ SDIGenlockCmdTxt, SDIGCmdTxt, NULL, &GenlockLevel2[0], NONE, NONE}
};

//**************************************************************************
code struct CMD_STRUCT GenlockLevel1[3] = {
	{ InputCmdTxt, InpCmdTxt, &GenlockLevel1[1], NULL, NONE, GenlockInput},
	{ SystemCmdTxt, SystCmdTxt, &GenlockLevel1[2], NULL, NONE, GenlockSystem},
	{ DelayCmdTxt, DelCmdTxt, &GenlockLevel1[3], NULL, NONE, GenlockDelay},
};

//**************************************************************************
code struct CMD_STRUCT GenlockLevel2[1] = {
	{ VersionCmdTxt, VersCmdTxt, NULL, NULL, NONE, SDIGenlockVersion}
};

//**************************************************************************
code struct CMD_STRUCT Output[7] = {
	{ BBCmdTxt, BBCmdTxt, &Output[1], &BBLevel1[0], BBSuffix, BB},
	{ SBCmdTxt, SBCmdTxt, &Output[2], &SBLevel1[0], SBSuffix, SB},
	{ ASignalCmdTxt, ASigCmdTxt, &Output[3], &AnalogLevel1[0], NONE, Analog},
	{ SDISignalCmdTxt, SDISCmdTxt, &Output[4], &SDILevel1[0], NONE, SDI},
	{ AudioCmdTxt, AudCmdTxt, &Output[5], &AudioLevel1[0], NONE, Audio},
	{ TimeCodeCmdTxt, TimCmdTxt, &Output[6], &TimeCodeLevel1[0], NONE, NONE},
	{ BBMultiCmdTxt, BBMCmdTxt, NULL, &BBMultiLevel1[0], NONE, NONE},
};

//**************************************************************************
code struct CMD_STRUCT BBLevel1[5] = {
	{ SystemCmdTxt, SystCmdTxt, &BBLevel1[1], NULL, NONE, BBSystem},
	{ DelayCmdTxt, DelCmdTxt, &BBLevel1[2], NULL, NONE, BBDelay},
	{ ScHPhaseCmdTxt, ScHPCmdTxt, &BBLevel1[3], NULL, NONE, BBScHPhase},
	{ CopyCmdTxt, CopyCmdTxt, &BBLevel1[4], NULL, NONE, BBCopy},
	{ VersionCmdTxt, VersCmdTxt, NULL, NULL, NONE, BBVersion}
};

//**************************************************************************
code struct CMD_STRUCT SBLevel1[7] = {
	{ PatternCmdTxt, PattCmdTxt, &SBLevel1[1], NULL, NONE, SBPattern},
	{ SystemCmdTxt, SystCmdTxt, &SBLevel1[2], NULL, NONE, SBSystem},
	{ DelayCmdTxt, DelCmdTxt, &SBLevel1[3], NULL, NONE, SBDelay},
	{ EDHInsertCmdTxt, EDHCmdTxt, &SBLevel1[4], NULL, NONE, SBEDHInsert},
	{ EmbAudioCmdTxt, EmbCmdTxt, &SBLevel1[5], NULL, NONE, SBEmbAudio},
	{ CopyCmdTxt, CopyCmdTxt, &SBLevel1[6], NULL, NONE, SBCopy},
	{ VersionCmdTxt, VersCmdTxt, NULL, NULL, NONE, SBVersion}
};

//**************************************************************************
code struct CMD_STRUCT AnalogLevel1[6] = {
	{ PatternCmdTxt, PattCmdTxt, &AnalogLevel1[1], NULL, NONE, AnalogPattern},
	{ TextInsertCmdTxt, TextCmdTxt, &AnalogLevel1[2], NULL, NONE, AnalogTextInsert},
	{ SystemCmdTxt, SystCmdTxt, &AnalogLevel1[3], NULL, NONE, AnalogSystem},
	{ DelayCmdTxt, DelCmdTxt, &AnalogLevel1[4], NULL, NONE, AnalogDelay},
	{ ScHPhaseCmdTxt, ScHPCmdTxt, &AnalogLevel1[5], NULL, NONE, AnalogScHPhase},
	{ VersionCmdTxt, VersCmdTxt, NULL, NULL, NONE, AnalogVersion}
};

//**************************************************************************
code struct CMD_STRUCT SDILevel1[7] = {
	{ PatternCmdTxt, PattCmdTxt, &SDILevel1[1], NULL, NONE, SDIPattern},
	{ TextInsertCmdTxt, TextCmdTxt, &SDILevel1[2], NULL, NONE, SDITextInsert},
	{ SystemCmdTxt, SystCmdTxt, &SDILevel1[3], NULL, NONE, SDISystem},
	{ EDHInsertCmdTxt, EDHCmdTxt, &SDILevel1[4], NULL, NONE, SDIEDHInsert},
	{ EmbAudioCmdTxt, EmbCmdTxt, &SDILevel1[5], &SDILevel3[0], NONE, NONE},
	{ DelayCmdTxt, DelCmdTxt, &SDILevel1[6], NULL, NONE, SDIDelay},
	{ VersionCmdTxt, VersCmdTxt, NULL, NULL, NONE, SDIVersion}
};

code struct CMD_STRUCT SDILevel3[2] = {
	{ SignalCmdTxt, SignCmdTxt, &SDILevel3[1], NULL, NONE, SDIAudioSignal},
	{ LevelCmdTxt, LevCmdTxt, NULL, NULL, NONE, SDIAudioLevel},
};

//**************************************************************************
code struct CMD_STRUCT AudioLevel1[4] = {
	{ SignalCmdTxt, SignCmdTxt, &AudioLevel1[1], NULL, NONE, AudioSignal},
	{ LevelCmdTxt, LevCmdTxt, &AudioLevel1[2], NULL, NONE, AudioLevel},
	{ TimingCmdTxt, TimCmdTxt, &AudioLevel1[3], NULL, NONE, AudioTiming},
	{ VersionCmdTxt, VersCmdTxt, NULL, NULL, NONE, AudioVersion}
};

//**************************************************************************
code struct CMD_STRUCT TimeCodeLevel1[1] = {
	{ VersionCmdTxt, VersCmdTxt, NULL, NULL, NONE, TimeCodeVersion}
};

//**************************************************************************
code struct CMD_STRUCT BBMultiLevel1[1] = {
	{ VersionCmdTxt, VersCmdTxt, NULL, NULL, NONE, BBMultiVersion}
};

//**************************************************************************
code struct CMD_STRUCT Factory[10] = {
	{ PasswordCmdTxt, PASSCmdTxt, &Factory[1], NULL, NONE, FactoryPassword},
	{ V24CircuitCmdTxt, V24CCmdTxt, &Factory[2], &FactoryLevel1[0], NONE, NONE},
	{ SPGGeneratorCmdTxt, SPGGCmdTxt, &Factory[3], &FactoryLevel2[0], NONE, NONE},
	{ SDIGenlockCmdTxt, SDIGCmdTxt, &Factory[4], &FactoryLevel3[0], NONE, NONE},
	{ AudioCmdTxt, AudCmdTxt, &Factory[5], &FactoryLevel4[0], NONE, NONE},
	{ MainboardCmdTxt, MainCmdTxt, &Factory[6], &FactoryLevel5[0], NONE, NONE},
	{ TimeCodeCmdTxt, TimCmdTxt, &Factory[7], &FactoryLevel6[0], NONE, NONE},
	{ MultiBBCmdTxt, MultCmdTxt, &Factory[8], &FactoryLevel7[0], NONE, NONE},
	{ ADCCmdTxt, ADCCmdTxt, &Factory[9], &FactoryLevel8[0], NONE, NONE},
	{ LDetectorCmdTxt, LDETCmdTxt, NULL, &FactoryLevel9[0], NONE, NONE},
};

code struct CMD_STRUCT FactoryLevel1[2] = {
	{ AddressCmdTxt, AddrCmdTxt, &FactoryLevel1[1], NULL, NONE, FactoryAddress},
	{ CommandCmdTxt, CommCmdTxt, NULL, NULL, NONE, FactoryCommand},
};

code struct CMD_STRUCT FactoryLevel2[3] = {
	{ TextStringCmdTxt, TextCmdTxt, &FactoryLevel2[1], NULL, NONE, FactorySPGUserText},
	{ StoreCmdTxt, StorCmdTxt, &FactoryLevel2[2], NULL, NONE, FactorySPGStore},
	{ ReadCmdTxt, ReadCmdTxt, NULL, NULL, NONE, FactorySPGRead},
};

code struct CMD_STRUCT FactoryLevel3[1] = {
	{ TextStringCmdTxt, TextCmdTxt, NULL, NULL, NONE, FactorySDIGenlockUserText}
};

code struct CMD_STRUCT FactoryLevel4[3] = {
	{ TextStringCmdTxt, TextCmdTxt, &FactoryLevel4[1], NULL, NONE, FactoryAudioUserText},
	{ WritCmdTxt, WriteCmdTxt, &FactoryLevel4[2], NULL, NONE, FactoryAudioWrite},
	{ ReadCmdTxt, ReadCmdTxt, NULL, NULL, NONE, FactoryAudioRead}
};

code struct CMD_STRUCT FactoryLevel5[3] = {
	{ TextStringCmdTxt, TextCmdTxt, &FactoryLevel5[1], NULL, NONE, FactoryMainUserText},
	{ PSN5CmdTxt, PSN5CmdTxt, &FactoryLevel5[2], NULL, FactoryMainPSNSuffix, FactoryMainPSN},
	{ PS12CmdTxt, PS12CmdTxt, NULL, NULL, FactoryMainPSSuffix, FactoryMainPS}
};

code struct CMD_STRUCT FactoryLevel6[1] = {
	{ TextStringCmdTxt, TextCmdTxt, NULL, NULL, NONE, FactoryTimeCodeUserText}
};

code struct CMD_STRUCT FactoryLevel7[1] = {
	{ TextStringCmdTxt, TextCmdTxt, NULL, NULL, NONE, FactoryMultiBBUserText}
};

code struct CMD_STRUCT FactoryLevel8[1] = {
	{ ReadCmdTxt, ReadCmdTxt, NULL, NULL, NONE, FactoryADCRead}
};

code struct CMD_STRUCT FactoryLevel9[1] = {
	{ ReadCmdTxt, ReadCmdTxt, NULL, NULL, NONE, FactoryLDetectorRead}
};
//**************************************************************************
// The following parameter structures are based on the enumerated types
//  defined in TABLES.H, ie. DO NOT change the order of these names. If
//  the order is changed in TABLES.H it must also be changed here.

code struct PAR_STRUCT GenlockInputPar[6] = {
	{ AParTxt, AParTxt, &GenlockInputPar[1]},
	{ BParTxt, BParTxt, &GenlockInputPar[2]},
	{ ABParTxt, ABParTxt, &GenlockInputPar[3]},
	{ SDIParTxt, SDIParTxt, &GenlockInputPar[4]},
	{ InternalParTxt, IntParTxt, &GenlockInputPar[5]},
	{ Internal2ParTxt, Int2ParTxt, NULL}
};

code struct PAR_STRUCT GenlockSystemPar[10] = {
	{ PALBurstParTxt, PALBParTxt, &GenlockSystemPar[1]},
	{ NTSCBurstParTxt, NTSCParTxt, &GenlockSystemPar[2]},
	{ Sync625ParTxt, Sync625ParTxt, &GenlockSystemPar[3]},
	{ Sync525ParTxt, Sync525ParTxt, &GenlockSystemPar[4]},
	{ SDI625ParTxt, SDI625ParTxt, &GenlockSystemPar[5]},
	{ SDI525ParTxt, SDI525ParTxt, &GenlockSystemPar[6]},
	{ F358MHzParTxt, F358MHzParTxt, &GenlockSystemPar[7]},
	{ F443MHzParTxt, F443MHzParTxt, &GenlockSystemPar[8]},
	{ F5MHzParTxt, F5MHzParTxt, &GenlockSystemPar[9]},
	{ F10MHzParTxt, F10MHzParTxt, NULL},
};

code struct PAR_STRUCT AnalogPatternPar[15] = {
	{ CBSMPTEParTxt, CBSMParTxt, &AnalogPatternPar[1]},
	{ CBEBUParTxt, CBEBParTxt, &AnalogPatternPar[2]},
	{ CB100ParTxt, CB100ParTxt, &AnalogPatternPar[3]},
	{ CBGrey75ParTxt, CBGR75ParTxt, &AnalogPatternPar[4]},
	{ CBRed75ParTxt, CBR75ParTxt, &AnalogPatternPar[5]},
	{ WIN15ParTxt, WIN15ParTxt, &AnalogPatternPar[6]},
	{ WIN20ParTxt, WIN20ParTxt, &AnalogPatternPar[7]},
	{ WIN100ParTxt, WIN100ParTxt, &AnalogPatternPar[8]},
	{ CrossHatchParTxt, CROSParTxt, &AnalogPatternPar[9]},
	{ PlugeParTxt, PLUGParTxt, &AnalogPatternPar[10]},
	{ SafeAreaParTxt, SafParTxt, &AnalogPatternPar[11]},
	{ ShallowRampParTxt, SHALParTxt, &AnalogPatternPar[12]},
	{ MultiBurstParTxt, MULTParTxt, &AnalogPatternPar[13]},
	{ Red75ParTxt, Red75ParTxt, &AnalogPatternPar[14]},
	{ BlackBurstParTxt, BlacParTxt, NULL}
};

code struct PAR_STRUCT AnalogTVSystemPar[3] = {
	{ PALParTxt, PALParTxt, &AnalogTVSystemPar[1]},
	{ PAL_IDParTxt, PAL_IDParTxt, &AnalogTVSystemPar[2]},
	{ NTSCParTxt, NTSCParTxt, NULL},
};

code struct PAR_STRUCT DigitalTVSystemPar[2] = {
	{ SDI625ParTxt, SDI625ParTxt, &DigitalTVSystemPar[1]},
	{ SDI525ParTxt, SDI525ParTxt, NULL},
};

code struct PAR_STRUCT SDIBlkPatternPar[4] = {
	{ BlackParTxt, BlacParTxt, &SDIBlkPatternPar[1]},
	{ CBEBUParTxt, CBEBParTxt, &SDIBlkPatternPar[2]},
	{ CB100ParTxt, CB100ParTxt, &SDIBlkPatternPar[3]},
	{ CBSMPTEParTxt, CBSMParTxt, NULL},
};

code struct PAR_STRUCT SDIPatternPar[17] = {
	{ CBSMPTEParTxt, CBSMParTxt, &SDIPatternPar[1]},
	{ CBEBUParTxt, CBEBParTxt, &SDIPatternPar[2]},
	{ CBEBU8ParTxt, CBEB8ParTxt, &SDIPatternPar[3]},
	{ CB100ParTxt, CB100ParTxt, &SDIPatternPar[4]},
	{ CBGrey75ParTxt, CBGR75ParTxt, &SDIPatternPar[5]},
	{ CBRed75ParTxt, CBR75ParTxt, &SDIPatternPar[6]},
	{ SDICheckParTxt, SDICParTxt, &SDIPatternPar[7]},
	{ ShallowRampParTxt, SHALParTxt, &SDIPatternPar[8]},
	{ DTimingParTxt, DTIMParTxt, &SDIPatternPar[9]},
	{ BlackParTxt, BlacParTxt, &SDIPatternPar[10]},
	{ WIN15ParTxt, WIN15ParTxt, &SDIPatternPar[11]},
	{ WIN20ParTxt, WIN20ParTxt, &SDIPatternPar[12]},
	{ WIN100ParTxt, WIN100ParTxt, &SDIPatternPar[13]},
	{ CrossHatchParTxt, CROSParTxt, &SDIPatternPar[14]},
	{ PlugeParTxt, PLUGParTxt, &SDIPatternPar[15]},
	{ MultiBurstParTxt, MULTParTxt, &SDIPatternPar[16]},
	{ Red75ParTxt, Red75ParTxt, NULL}
};

code struct PAR_STRUCT OffOnPar[2] = {
	{ OFFParTxt, OFFParTxt, &OffOnPar[1]},
	{ ONParTxt, ONParTxt, NULL},
};

code struct PAR_STRUCT OffSilencePar[2] = {
	{ OFFParTxt, OFFParTxt, &OffSilencePar[1]},
	{ SilenceParTxt, SilParTxt, NULL},
};

code struct PAR_STRUCT AudioSignalPar[9] = {
	{ OFFParTxt, OFFParTxt, &AudioSignalPar[1]},
	{ S800HZParTxt, S800HZParTxt, &AudioSignalPar[2]},
	{ S1KHZParTxt, S1KHZParTxt, &AudioSignalPar[3]},
	{ SEBU1KHZParTxt, SEBU1KHZParTxt, &AudioSignalPar[4]},
	{ SBBC1KHZParTxt, SBBC1KHZParTxt, &AudioSignalPar[5]},
	{ MEBU1KHZParTxt, MEBU1KHZParTxt, &AudioSignalPar[6]},
	{ MBBC1KHZParTxt, MBBC1KHZParTxt, &AudioSignalPar[7]},
	{ DUALParTxt, DUALParTxt, &AudioSignalPar[8]},
	{ F48KHZParTxt, F48KHZParTxt, NULL},
};

code struct PAR_STRUCT AudioLevelPar[8] = {
	{ SilenceParTxt, SilParTxt, &AudioLevelPar[1]},
	{ DB0FSParTxt, DB0FSParTxt, &AudioLevelPar[2]},
	{ DB9FSParTxt, DB9FSParTxt, &AudioLevelPar[3]},
	{ DB12FSParTxt, DB12FSParTxt, &AudioLevelPar[4]},
	{ DB14FSParTxt, DB14FSParTxt, &AudioLevelPar[5]},
	{ DB16FSParTxt, DB16FSParTxt, &AudioLevelPar[6]},
	{ DB18FSParTxt, DB18FSParTxt, &AudioLevelPar[7]},
	{ DB20FSParTxt, DB20FSParTxt, NULL},
};

code struct PAR_STRUCT AudioTimingPar[6] = {
	{ PALParTxt, PALParTxt, &AudioTimingPar[1]},
	{ NTSC1ParTxt, NTSC1ParTxt, &AudioTimingPar[2]},
	{ NTSC2ParTxt, NTSC2ParTxt, &AudioTimingPar[3]},
	{ NTSC3ParTxt, NTSC3ParTxt, &AudioTimingPar[4]},
	{ NTSC4ParTxt, NTSC4ParTxt, &AudioTimingPar[5]},
	{ NTSC5ParTxt, NTSC5ParTxt, NULL},
};

code struct PAR_STRUCT FactoryAddressPar[7] = {
	{ bbu_0ParTxt, bbu_0ParTxt, &FactoryAddressPar[1]},
	{ bbu_1ParTxt, bbu_1ParTxt, &FactoryAddressPar[2]},
	{ bbu_2ParTxt, bbu_2ParTxt, &FactoryAddressPar[3]},
	{ bbu_3ParTxt, bbu_3ParTxt, &FactoryAddressPar[4]},
	{ spgParTxt, spgParTxt, &FactoryAddressPar[5]},
	{ AnlSignalParTxt, AnlSParTxt, &FactoryAddressPar[6]},
	{ SDISignalParTxt, SDISParTxt, NULL},
};

code struct PAR_STRUCT MinMaxPar[2] = {
	{ MinParTxt, MinParTxt, &MinMaxPar[1]},
	{ MaxParTxt, MaxParTxt, NULL},
};

/***************************************************************************/
// Temporary variables PRIVATE NOT overlayable

static UC UnitAddress;
static char CmdBuffer[4];

static UC Suffix;

static char TmpBuffer[16];

static UC ParState;

static char Field;
static int Line;
static float HTime;
static char sign;

static UL UlongVal;
static UC UcharVal;
static UC UcharVal1;
static int IntVal;

/***************************************************************************/
/*	IDNResponseMessage												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970218	    										*/
/* Revised:		970218				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char	 														*/
/*					ParState 2: Char, Char	 												*/
/*					ParState 3: Char, Char, Char	 										*/
/*					ParState 4: Char, Char, Char, Char									*/
/*					IDN is : PHILIPS,PT5210,KU123456,1911								*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void IDNResponseMessage() {

	register UC i, CommaSearch;
	char* txtptr;
	char tmp;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_ASCII:
			ParState++;

			txtptr = Cmd;
			i = CommaSearch = 0;

			while ( tmp = *txtptr) {

				if (( TmpBuffer[i] = tmp) == ',') {
					TmpBuffer[i] = 0;

					switch ( CommaSearch++) {
						case 0:					// Company name
							break;

						case 1:					// Model name
							break;

						case 2:					// KU number
							ResponseKUNo = atol( &TmpBuffer[2]);

													// SW version
							strncpy( &TmpBuffer[0], txtptr+1, 4);
							TmpBuffer[4] = 0;

							ResponseSWVersion = ( UC)( 10*atof( TmpBuffer));
							break;
					}
					i = 0;
				}
				else
					i++;

				txtptr++;
			}
			break;

		default:
			break;
	}

	if ( CmdExecute && ( ParState == 1))
		ResponseReceived = TRUE;
}

/***************************************************************************/
/*	CLSCommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Clear status command														*/
/*	Remarks:		No parameters																*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void CLSCommand() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else
			ResetErrorEventQueue();
	}
}

/***************************************************************************/
/*	ESECommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970627	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Set/read the Standard Event Status Enable Register				*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void ESECommand() {

	register int tmp;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				if (( tmp = atoi( Cmd)) > 255)
					SCPIError = DataOutOfRange;
				else
					UcharVal = ( UC) tmp;
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( ParState == 0) {
				sprintf( TmpBuffer, "%d\n", ESEReg);
				Udr_PutNChar( TmpBuffer);
			}
			else
				SCPIError = ParNotAllowed;
		}
		else {
			if ( ParState == 1)
				ESEReg = UcharVal;
			else
				SCPIError = MissingPar;
		}
	}
}

/***************************************************************************/
/*	ESRCommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970627	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Read and clear the Standard Event Status Register				*/
/*	Remarks:		No parameters																*/
/*					The register will reset to zero.										*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void ESRCommand() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			sprintf( TmpBuffer, "%d\n", UpdateESRRegister());
			Udr_PutNChar( TmpBuffer);

			ESRReg = 0;
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	OPCCommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Operation Complete Command/Query										*/
/*	Remarks:		No parameters																*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void OPCCommand() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			;
		else
			;
	}
}

/***************************************************************************/
/*	SRECommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970627	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Set/read the Service Request Enable Register						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					Bit 6 in *SRE? register is always ZERO								*/
/*					Bit 6 in *SRE register is ignored									*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void SRECommand() {

	register int tmp;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				if (( tmp = atoi( Cmd)) > 255)
					SCPIError = DataOutOfRange;
				else
					UcharVal = ( UC) tmp;
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( ParState == 0) {
				sprintf( TmpBuffer, "%d\n", SREReg);
				Udr_PutNChar( TmpBuffer);
			}
			else
				SCPIError = ParNotAllowed;
		}
		else {
			if ( ParState == 1)
				SREReg = UcharVal & 0xBF;
			else
				SCPIError = MissingPar;
		}
	}
}

/***************************************************************************/
/*	STBCommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970627	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Read the Status Byte Register											*/
/*	Remarks:		No parameters																*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void STBCommand() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			UpdateSTBRegister();

			sprintf( TmpBuffer, "%d\n", STBReg);
			Udr_PutNChar( TmpBuffer);
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	TSTCommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Selftest query																*/
/*	Remarks:		No parameters																*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void TSTCommand() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			;
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	WAICommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Wait-to-continue command												*/
/*	Remarks:		No parameters																*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void WAICommand() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else
			;
	}
}

/***************************************************************************/
/*	RSTCommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 961124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Reset comamnd																*/
/*	Remarks:		No parameters																*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void RSTCommand() {

	register int j;

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;
	else {
		if ( CmdExecute && !SCPIError) {
			if ( !CmdRequest) {

				for ( j=0; j < NoOfAnlBlkUnits; j++) {
					Cmemcpy( &AnlBlkConfig[j], &AnlBlkConfigFactory[j], sizeof( AnlBlkConfig[0]));

					if ( j < NoOfSDIBlkUnits)
						Cmemcpy( &SDIBlkConfig[j], &SDIBlkConfigFactory[j], sizeof( SDIBlkConfig[0]));

					if ( j < NoOfGenlockUnits)
						Cmemcpy( &GenlockConfig[j], &GenlockConfigFactory[j], sizeof( GenlockConfig[0]));
				}
				Cmemcpy( &AnalogConfig, &AnalogConfigFactory, sizeof( AnalogConfig));
				Cmemcpy( &SDIConfig,  &SDIConfigFactory, sizeof( SDIConfig));
				Cmemcpy( &AES_EBUConfig, &AES_EBUConfigFactory, sizeof( AES_EBUConfig));

				Cmemcpy( &RS232Config, &RS232ConfigFactory, sizeof( RS232Config));

				LCDContrast = Def_Contrast;

				ActiveOutputNo = AES_EBUItem;
				ActivePresetNo = 0;

				Flags.NormalLockOn = OFF;
				Flags.PanelLockOn = OFF;
				Flags.DownloadLockOn = OFF;
				Flags.DiagnoseLockOn = OFF;

				Flags.AutoESCEnable = ON;
				Flags.AutoESCToStatus = ON;

				PasswordState = ON;
														// Reset apparatus error queue
				for ( j=0; j < NoInErrorQueue; j++)
					ErrorQueue[j] = 0;

				ErrorQueuePtr = NoInErrorQueue-1;

				ResetErrorEventQueue();			// Reset RS232 error queue

				Udr_InitializeUart();			// Initialize the remote RS232

														// Initialize mainboard BB's
				for ( j = BB1Item; j < BB8Item+1; j++)
					if ( AnlBlkUnit[j].Present)
						ErrorCode = ConfigureAnlBlkUnit( j);

				feed_dog;							// Watch-dog feed
														// Initialize mainboard SB's
				for ( j = SB34Item; j < SB78Item+1; j++)
					if ( SDIBlkUnit[j].Present)
						ErrorCode = ConfigureSDIBlkUnit( j);

				feed_dog;							// Watch-dog feed
														// Initialize Analog test signal
				if ( AnalogSignalUnit.Present)
					ErrorCode = ConfigureAnalogSignalUnit();

				feed_dog;							// Watch-dog feed
														// Initialie SDI test signal
				if ( SDISignalUnit.Present)
					ErrorCode = ConfigureSDISignalUnit();

				feed_dog;							// Watch-dog feed

				if ( AES_EBUUnit.Present) 		// AES/EBU audio generator
					ErrorCode = AES_EBUUnitDrv( AES_EBUConfig.Signal, AES_EBUConfig.Level, AES_EBUConfig.Timing);

				feed_dog;							// Watch-dog feed

				ErrorCode = GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
			}
			else
				SCPIError = SyntaxErr;
		}
	}
}

/***************************************************************************/
/*	IDNCommand															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Identification query														*/
/*	Remarks:		No parameter, Query only												*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void IDNCommand() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {

			IIC2_RdStr( mainram1_addr, CompanyName_addr, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			IIC2_RdStr( mainram1_addr, ProductName1_addr, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			if ( strlen( TmpBuffer) >= 10) {
				IIC2_RdStr( mainram1_addr, ProductName2_addr, TmpBuffer);
				Udr_PutNChar( TmpBuffer);
			}

			Udr_PutChar( ',');

			IIC2_RdStr( mainram1_addr, ProductKUNo_addr, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			ReadSWVersion( TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutNewline();
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	SystemError															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Write the next entry in the error queue (FIFO) to RS232 		*/
/*	Remarks:		No parameters, Query only												*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemError() {

	register int i;

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if (( i = ReadErrorEventQueue()) >= 0) {
				sprintf( TmpBuffer, "%d", ErrorTxt_Array[i].ErrorNo);
				Udr_PutNChar( TmpBuffer);

				Udr_PutNChar( ",\"");

				Udr_CPutNChar( ErrorTxt_Array[i].ErrorTxt);
				Udr_PutChar( '"');

				Udr_PutNewline();
			}
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	SystemVersion														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Write the SCPI version to RS232										*/
/*	Remarks:		No parameter, Query only												*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemVersion() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			Udr_CPutNChar( SystemVersionTxt);
			Udr_PutNewline();
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	SystemPreset														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Ativate the specified preset											*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemPreset() {

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal = ( UC) atoi( Cmd);

				if (( UcharVal == 0) || ( UcharVal > 8))
					SCPIError = DataOutOfRange;
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( ParState == 0) {
				Udr_PutChar( '0' + ActivePresetNo);
				Udr_PutNewline();
			}
			else
				SCPIError = SyntaxErr;
		}
		else {
			ActivePresetNo = UcharVal;
			ErrorCode = ConfigurePreset( --UcharVal);
		}
	}
}

/***************************************************************************/
/*	SystemPresetStore													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Store actual setup in preset											*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemPresetStore() {

	register UC i;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal = ( UC) atoi( Cmd);

				if (( UcharVal == 0) || ( UcharVal > 8))
					SCPIError = DataOutOfRange;
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( !CmdRequest) {
			ActivePresetNo = UcharVal;

			UcharVal--;

			for ( i = 0; i < NoOfAnlBlkUnits; i++)
				memcpy( &Preset[UcharVal].AnlBlkStore[i], &AnlBlkConfig[i], sizeof( AnlBlkConfig[0]));

			for ( i = 0; i < NoOfSDIBlkUnits; i++)
				memcpy( &Preset[UcharVal].SDIBlkStore[i], &SDIBlkConfig[i], sizeof( SDIBlkConfig[0]));

			memcpy( &Preset[UcharVal].AnalogStore, &AnalogConfig, sizeof( AnalogConfig));
			memcpy( &Preset[UcharVal].SDIStore, &SDIConfig, sizeof( SDIConfig));
			memcpy( &Preset[UcharVal].AES_EBUStore, &AES_EBUConfig, sizeof( AES_EBUConfig));

			for ( i = 0; i < NoOfGenlockUnits; i++)
				memcpy( &Preset[UcharVal].GenlockStore[i], &GenlockConfig[i], sizeof( GenlockConfig[0]));

			Preset[UcharVal].ActiveGenlockNo = ActiveGenlockNo;
			Preset[UcharVal].ExternalGenlockNo = ExternalGenlockNo;
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	SystemPresetName													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:	Name a specific preset number.										*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum, String												*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemPresetName() {

	register UC i;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal = ( UC) atoi( Cmd);

				if (( UcharVal == 0) || ( UcharVal > 8))
					SCPIError = DataOutOfRange;
				else
					UcharVal--;
			}
			else
				SCPIError = DataTypeErr;
			break;

		case P_String:
			if ( ParState == 1) {
				ParState++;

				if ( strlen( Cmd) > 16)
					SCPIError = DataOutOfRange;
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( ParState == 1) {
				Udr_PutChar( '"');
				Udr_PutNChar( Preset[UcharVal].Name);
				Udr_PutChar( '"');
				Udr_PutNewline();
			}
			else
				SCPIError = SyntaxErr;
		}
		else {
			if ( ParState == 2) {

				for ( i=0; i < strlen( Cmd); i++)
					if ( !CheckTextASCII( FALSE, Cmd[i]))
						break;

				if ( i == strlen( Cmd)) {
					strcpy( Preset[UcharVal].Name, "                ");
					strncpy( Preset[UcharVal].Name, Cmd, strlen( Cmd));
				}
				else
					SCPIError = ParErr;
			}
			else
				SCPIError = SyntaxErr;
		}
	}
}

/***************************************************************************/
/*	SystemPresetDownload													  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Upload a specific preset to another SPG							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemPresetDownload() {

	UI NoOfBytes, i;
	UL checksum;
	UC tmp;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal = ( UC) atoi( Cmd);

				if (( UcharVal == 0) || ( UcharVal > 8))
					SCPIError = DataOutOfRange;
				else
					UcharVal--;
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( !CmdRequest) {
			NoOfBytes = sizeof( Preset[0]);

			TmpBuffer[0] = '#';
			TmpBuffer[1] = '3';
			sprintf( &TmpBuffer[2], "%03d", 2*NoOfBytes + 2 + 8);
			TmpBuffer[5] = 0;

			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( 0 + DownloadOffset);				// Revision: 01
			Udr_PutChar( 1 + DownloadOffset);

			checksum = 1;

			memcpy( Cmd, &Preset[UcharVal], NoOfBytes);

			for ( i = 0; i < NoOfBytes; i++) {
				tmp = (UC) Cmd[i] >> 4;
				checksum += tmp;
				Udr_PutChar(( char) ( tmp + DownloadOffset));

				tmp = (UC) Cmd[i] & 0x0F;
				checksum += tmp;
				Udr_PutChar(( char) ( tmp + DownloadOffset));
			}

			Udr_PutChar((( checksum & 0xF0000000) >> 28) + DownloadOffset);
			Udr_PutChar((( checksum & 0x0F000000) >> 24) + DownloadOffset);
			Udr_PutChar((( checksum & 0x00F00000) >> 20) + DownloadOffset);
			Udr_PutChar((( checksum & 0x000F0000) >> 16) + DownloadOffset);
			Udr_PutChar((( checksum & 0x0000F000) >> 12) + DownloadOffset);
			Udr_PutChar((( checksum & 0x00000F00) >> 8) + DownloadOffset);
			Udr_PutChar((( checksum & 0x000000F0) >> 4) + DownloadOffset);
			Udr_PutChar((( checksum & 0x0000000F) + DownloadOffset));

			Udr_PutNewline();
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	SystemPresetUpload													  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Upload a specific preset to another SPG							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum, Blockn												*/
/*					This routine MUST match SystemPresetDownload EXACTLY			*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemPresetUpload() {

	UC revision;
	UI NoOfBytes, i, l;
	UL CalcChecksum = 0, RecChecksum = 0;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal = ( UC) atoi( Cmd);

				if (( UcharVal == 0) || ( UcharVal > 8))
					SCPIError = DataOutOfRange;
				else
					UcharVal--;
			}
			else
				SCPIError = DataTypeErr;
			break;

		case P_Blockn:
			if ( ParState == 1)
				ParState++;
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else {
			if ( ParState == 2) {

				NoOfBytes = 2*sizeof( Preset[0]) + 2;

				for ( i = 0; i < NoOfBytes; i++) {
					Cmd[i] -= DownloadOffset;
					CalcChecksum += Cmd[i];
				}

				revision = ( Cmd[0] << 4) + Cmd[1];

				for ( i = NoOfBytes; i < NoOfBytes+8; i++) {
					Cmd[i] -= DownloadOffset;
					RecChecksum = ( RecChecksum << 4) + Cmd[i];
				}

				if ( RecChecksum != CalcChecksum)
					ResponseReceived = FALSE;
				else {
					for ( l = 0, i = 2; i < NoOfBytes; i += 2, l++)
						Cmd[l] = ( Cmd[i] << 4) + Cmd[i+1];

					memcpy( &Preset[UcharVal], Cmd, sizeof( Preset[0]));

					ResponseReceived = TRUE;
				}
			}
		}
	}
}

/***************************************************************************/
/*	SystemDownload														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970227				     													*/
/*																									*/
/*	Function:	Download actual setup from another SPG								*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemDownload() {
}

/***************************************************************************/
/*	SystemUpload														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970227				     													*/
/*																									*/
/*	Function:	Download actual setup from another SPG								*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemUpload() {
}

/***************************************************************************/
/*	StatusOperEvent													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Set/read the status operation event register						*/
/*	Remarks:		Reading the event register clears it								*/
/*	Returns:		-																				*/
/*	Updates:		OperEventReg																*/
/***************************************************************************/
void StatusOperEvent() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			sprintf( TmpBuffer, "%d\n", OperEventReg);
			Udr_PutNChar( TmpBuffer);

			OperEventReg = 0;
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	StatusOperCondition												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Read the status questionable condition register					*/
/*	Remarks:		No parameters																*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void StatusOperCondition() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			sprintf( TmpBuffer, "%d\n", OperConditionReg);
			Udr_PutNChar( TmpBuffer);
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	StatusOperEnable													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Set/read the status operaion enable register						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void StatusOperEnable() {

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				if ( atol( Cmd) > 65535)
					SCPIError = DataOutOfRange;
				else
					IntVal = atoi( Cmd);
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( ParState == 0) {
				sprintf( TmpBuffer, "%d\n", OperEnableReg);
				Udr_PutNChar( TmpBuffer);
			}
			else
				SCPIError = ParNotAllowed;
		}
		else {
			if ( ParState == 1)
				OperEnableReg = IntVal & 0xEFFF;
			else
				SCPIError = MissingPar;
		}
	}
}

/***************************************************************************/
/*	StatusQuestEvent													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Set/read the status questionablen event register				*/
/*	Remarks:		Reading the event register clears it								*/
/*	Returns:		-																				*/
/*	Updates:		QuestEventReg																*/
/***************************************************************************/
void StatusQuestEvent() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			sprintf( TmpBuffer, "%d\n", QuestEventReg);
			Udr_PutNChar( TmpBuffer);

			QuestEventReg = 0;
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	StatusQuestCondition												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Read the status operation condition register						*/
/*	Remarks:		No parameters																*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void StatusQuestCondition() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			sprintf( TmpBuffer, "%d\n", QuestConditionReg);
			Udr_PutNChar( TmpBuffer);
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	StatusQuestEnable													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Set/read the status questionable enable register				*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void StatusQuestEnable() {

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				if ( atol( Cmd) > 65535)
					SCPIError = DataOutOfRange;
				else
					IntVal = atoi( Cmd);
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( ParState == 0) {
				sprintf( TmpBuffer, "%d\n", QuestEnableReg);
				Udr_PutNChar( TmpBuffer);
			}
			else
				SCPIError = ParNotAllowed;
		}
		else {
			if ( ParState == 1)
				QuestEnableReg = IntVal & 0xEFFF;
			else
				SCPIError = MissingPar;
		}
	}
}

/***************************************************************************/
/*	StatusPreset														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void StatusPreset() {
}

/***************************************************************************/
/*	StatusPTSuffix														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void StatusPTSuffix() {

	if ( NumSuffix != 5210)
		SCPIError = SyntaxErr;
}

/***************************************************************************/
/*	StatusPT																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void StatusPT() {

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest && ( NumSuffix == 5210)) {
			Udr_PutChar( '"');

			if ( ErrorStatus)
				Udr_CPutNChar( ActiveErrorsTxt);
			else {
				if ( ErrorQueue[ErrorQueuePtr] != NoError)
					Udr_CPutNChar( NoActiveErrorsTxt);
				else
					Udr_CPutNChar( NoErrorsTxt);
			}

			Udr_PutChar( '"');
			Udr_PutNewline();
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	DiagnosticDisplay													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void DiagnosticDisplay() {
}

/***************************************************************************/
/*	DiagnosticErrorQueue												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		9703165				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void DiagnosticErrorQueue() {

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	DiagnosticErrorQueueReset										 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void DiagnosticErrorQueueReset() {

	register UC i;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else {
			for( i = 0 ; i < NoInErrorQueue; i++)
				ErrorQueue[i] = 0;

			ErrorStatus = 0;
			ErrorQueuePtr = NoInErrorQueue-1;

			Warning_LED = OFF;
			Update_LEDs();
		}
	}
}

/***************************************************************************/
/*	DisplayContrast													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970630				     													*/
/*																									*/
/*	Function:	Adjust the contrast on the 40x2 LCD display						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: Char															*/
/*	Returns:		-																				*/
/*	Updates:																						*/
/***************************************************************************/
void DisplayContrast() {

	register int tmp;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState = 1;

				if (( UcharVal = ( UC) atoi( Cmd)) > Max_Contrast)
					SCPIError = DataOutOfRange;
			}
			else
				SCPIError = DataTypeErr;
			break;

		case P_Char:
			if ( ParState == 0) {
				ParState = 2;

				UcharVal = FindParameter( Cmd, MinMaxPar);

				if ( UcharVal > MAX)
					SCPIError = IllegalParValue;
			}
			else
				SCPIError = ParNotAllowed;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if (( ParState == 0) || ( ParState == 2)) {

				if ( ParState == 0)
					tmp = LCDContrast;
				else {
					if ( UcharVal == MIN)
						tmp = Min_Contrast;
					else
						tmp = Max_Contrast;
				}

				sprintf( TmpBuffer, "%d\r\n", tmp);
				Udr_PutNChar( TmpBuffer);
			}
			else
				SCPIError = DataTypeErr;
		}
		else {
			if (( ParState == 1) || ( ParState == 2)) {

				if ( ParState == 1)
					LCDContrast = UcharVal;
				else {
					if ( UcharVal == MIN)
						LCDContrast = Min_Contrast;
					else
						LCDContrast = Max_Contrast;
				}
				Send_Contrast( LCDContrast);

				if ( IICErrorCode)
					SCPIError = ExecErr;
			}
			else
				SCPIError = DataTypeErr;
		}
	}
}

/***************************************************************************/
/*	Genlock																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void Genlock() {

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {

			if ( Unlocked_LED )
				Udr_CPutNChar( UNLOCKEDTxt);
			else
				Udr_CPutNChar( GENLOCKEDTxt);

			Udr_PutChar( ',');
			Udr_CPutNChar( GenlockInputPar[ActiveGenlockNo].LongName);

			Udr_PutChar( ',');

			if ( ActiveGenlockNo < Internal)
				Udr_CPutNChar( GenlockSystemPar[GenlockConfig[ActiveGenlockNo].System].LongName);
			else
				Udr_PutNChar( "NA");

			Udr_PutChar( ',');

			if ( ActiveGenlockNo == SDIGenlock)
				SDISamplesToFLT( GenlockConfig[ActiveGenlockNo].System,\
									  GenlockConfig[ActiveGenlockNo].Delay,\
									  &sign, &Field, &Line, &HTime);
			else
				AnalogSamplesToFLT( GenlockConfig[ActiveGenlockNo].System,\
										  GenlockConfig[ActiveGenlockNo].Delay,\
										  &sign, &Field, &Line, &HTime);

			PrintTimingFLT( sign, Field, Line, HTime);

			Udr_PutNewline();
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	GenlockInput														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void GenlockInput() {

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_Char:
			if ( ParState == 0) {
				ParState++;

				UcharVal = FindParameter( Cmd, GenlockInputPar);

				if ( UcharVal > Internal2)
					SCPIError = IllegalParValue;
			}
			else
				SCPIError = ParNotAllowed;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			Udr_CPutNChar( GenlockInputPar[ActiveGenlockNo].LongName);
			Udr_PutNewline();
		}
		else {
			ActiveGenlockNo = UcharVal;

			if ( UcharVal < Internal)				// If new input isn't internal..
				ExternalGenlockNo = UcharVal;		//  update also external no

			GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
		}
	}
}

/***************************************************************************/
/*	GenlockSystem														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970626				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void GenlockSystem() {

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_Char:
			if ( ParState == 0) {
				ParState++;

				UcharVal = FindParameter( Cmd, GenlockSystemPar);

				if ( UcharVal > MHzLock10)
					SCPIError = IllegalParValue;
			}
			else
				SCPIError = ParNotAllowed;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			Udr_CPutNChar( GenlockSystemPar[GenlockConfig[ActiveGenlockNo].System].LongName);
			Udr_PutNewline();
		}
		else {
			if ( ActiveGenlockNo < Internal) {

				if ( ActiveGenlockNo == SDIGenlock) {
					if (( UcharVal < SDI625Lock) || ( UcharVal > SDI625Lock))
						SCPIError = ExecErr;
				}
				else {
					if (( UcharVal == SDI625Lock) || ( UcharVal == SDI625Lock))
						SCPIError = ExecErr;
				}

				if ( SCPIError == NoErr) {
					GenlockConfig[ActiveGenlockNo].System = UcharVal;
					GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
				}
			}
			else
				SCPIError = ExecErr;
		}
	}
}

/***************************************************************************/
/*	GenlockDelay														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*					ParState 3:  DecNum, DecNum, DecNum									*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void GenlockDelay() {

	FindDelayParameters();

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if (( ActiveGenlockNo < Internal) &&
				 (	GenlockConfig[ActiveGenlockNo].System < MHzLock358)) {

				if ( ActiveGenlockNo == SDIGenlock)
					SDISamplesToFLT( GenlockConfig[ActiveGenlockNo].System,\
										  GenlockConfig[ActiveGenlockNo].Delay,\
										  &sign, &Field, &Line, &HTime);
				else
					AnalogSamplesToFLT( GenlockConfig[ActiveGenlockNo].System,\
											  GenlockConfig[ActiveGenlockNo].Delay,\
											  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutNewline();
			}
			else
				SCPIError = ExecErr;
		}
		else {
			if ( ParState == 3) {
				if ( ActiveGenlockNo < Internal) {
					UlongVal = AnalogFLTToSamples(\
										  GenlockConfig[ActiveGenlockNo].System,\
											Field, Line, HTime);

					if ( UlongVal != -1) {
						GenlockConfig[ActiveGenlockNo].Delay = UlongVal;
						TrxGenlockDelay( UlongVal);
					}
					else
						SCPIError = DataOutOfRange;
				}
			}
			else
				SCPIError = DataTypeErr;
		}
	}
}

/***************************************************************************/
/*	BBSuffix																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/* Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BBSuffix() {

	if ( NumSuffix > 8)
		SCPIError = HdrSuffixOutOfRange;
	else
		Suffix = NumSuffix-1;
}

/***************************************************************************/
/*	BB																		 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/* Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BB() {

	if ( AnlBlkUnit[Suffix].Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				Udr_CPutNChar( AnalogTVSystemPar[AnlBlkConfig[Suffix].System].LongName);
				Udr_PutChar( ',');

				AnalogSamplesToFLT( AnlBlkConfig[Suffix].System,\
										  AnlBlkConfig[Suffix].Delay,\
										  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);

				Udr_PutChar( ',');

				sprintf( TmpBuffer, "%d\r\n", AnlBlkConfig[Suffix].ScHPhase);
				Udr_PutNChar( TmpBuffer);
			}
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBSystem																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BBSystem() {

	if ( AnlBlkUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AnalogTVSystemPar);

					if ( UcharVal > NTSC)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AnalogTVSystemPar[AnlBlkConfig[Suffix].System].LongName);
				Udr_PutNewline();
			}
			else {
				AnlBlkConfig[Suffix].System = UcharVal;
				TrxAnlBlkSystem( Suffix, UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBScHPhase															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BBScHPhase() {

	if ( AnlBlkUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				if ( ParState == 0) {
					ParState++;

					IntVal = atoi( Cmd);

					if (( IntVal < ScHPhaseLow) || ( IntVal > ScHPhaseHigh))
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				sprintf( TmpBuffer, "%d\r\n", AnlBlkConfig[Suffix].ScHPhase);
				Udr_PutNChar( TmpBuffer);
			}
			else {
				AnlBlkConfig[Suffix].ScHPhase = IntVal;
				TrxAnlBlkScHPhase( Suffix, IntVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBDelay																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BBDelay() {

	if ( AnlBlkUnit[Suffix].Present) {

		FindDelayParameters();

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				AnalogSamplesToFLT( AnlBlkConfig[Suffix].System,\
										  AnlBlkConfig[Suffix].Delay,\
										  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutNewline();
			}
			else {
				if ( ParState == 3) {
					UlongVal = AnalogFLTToSamples(\
								AnlBlkConfig[Suffix].System, Field, Line, HTime);

					if ( UlongVal != -1) {
						AnlBlkConfig[Suffix].Delay = UlongVal;
						TrxAnlBlkDelay( Suffix, AnlBlkConfig[Suffix].Delay);
					}
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = DataTypeErr;
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBCopy																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970317				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BBCopy() {

	if ( AnlBlkUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = Cmd[2] - '1';

					if ( strlen( Cmd) <= 3)
						if ( !strncmp( Cmd, "BB", 2) && ( UcharVal <= BB8Item)) {
							if ( !AnlBlkUnit[UcharVal].Present)
								SCPIError = IllegalParValue;
						}
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( !CmdRequest) {
				memcpy( &AnlBlkConfig[Suffix], &AnlBlkConfig[UcharVal], sizeof( AnlBlkConfig[0]));
				ErrorCode = TrxAnlBlkSystem( Suffix, AnlBlkConfig[Suffix].System);
			}
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBVersion															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BBVersion() {

	if ( AnlBlkUnit[Suffix].Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteV24Version( AnlBlkUnit[Suffix].Address, 'H');
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SBSuffix																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SBSuffix() {

	switch ( NumSuffix) {
		case 34:
			Suffix = 0;
			break;

		case 56:
			Suffix = 1;
			break;

		case 78:
			Suffix = 2;
			break;

		default:
			SCPIError = HdrSuffixOutOfRange;
			break;
	}
}

/***************************************************************************/
/*	SB																		 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970317				     													*/
/*																									*/
/*	Function:																					*/
/* Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SB() {

	if ( SDIBlkUnit[Suffix].Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				Udr_CPutNChar( SDIBlkPatternPar[SDIBlkConfig[Suffix].Pattern].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( DigitalTVSystemPar[SDIBlkConfig[Suffix].System].LongName);
				Udr_PutChar( ',');

				SDISamplesToFLT( SDIBlkConfig[Suffix].System,\
									  SDIBlkConfig[Suffix].Delay,\
									  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutChar( ',');

				Udr_CPutNChar( OffOnPar[SDIBlkConfig[Suffix].EDH].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( OffSilencePar[SDIBlkConfig[Suffix].EMBAudio].LongName);
				Udr_PutNewline();
			}
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SBPattern															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SBPattern() {

	if ( SDIBlkUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, SDIBlkPatternPar);

					if ( UcharVal > SBSMPTECBar)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( SDIBlkPatternPar[SDIBlkConfig[Suffix].Pattern].LongName);
				Udr_PutNewline();
			}
			else {
				if ( SDIBlkConfig[Suffix].System == SDI625) {
					if ( UcharVal == SBSMPTECBar)
						SCPIError = ExecErr;
				}
				else {
					if (( UcharVal == SBEBUCBar) || ( UcharVal == SBCBar100))
						SCPIError = ExecErr;
				}

				if ( SCPIError == NoErr) {
					SDIBlkConfig[Suffix].Pattern = UcharVal;
					TrxSDIBlkPattern( Suffix, UcharVal);
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SBSystem																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SBSystem() {

	if ( SDIBlkUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, DigitalTVSystemPar);

					if ( UcharVal > SDI525)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( DigitalTVSystemPar[SDIBlkConfig[Suffix].System].LongName);
				Udr_PutNewline();
			}
			else {
				SDIBlkConfig[Suffix].System = UcharVal;
				TrxSDIBlkSystem( Suffix, UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SBDelay																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SBDelay() {

	if ( SDIBlkUnit[Suffix].Present) {

		FindDelayParameters();

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				SDISamplesToFLT( SDIBlkConfig[Suffix].System,\
									  SDIBlkConfig[Suffix].Delay,\
									  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutNewline();
			}
			else {
				if ( ParState == 3) {
					UlongVal = SDIFLTToSamples(\
								SDIBlkConfig[Suffix].System, Field, Line, HTime);

					if ( UlongVal != -1) {
						SDIBlkConfig[Suffix].Delay = UlongVal;
						TrxSDIBlkDelay( Suffix, SDIBlkConfig[Suffix].Delay);
					}
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = DataTypeErr;
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SBEDHInsert															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SBEDHInsert() {

	if ( SDIBlkUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, OffOnPar);

					if ( UcharVal > ON)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( OffOnPar[SDIBlkConfig[Suffix].EDH].LongName);
				Udr_PutNewline();
			}
			else {
				SDIBlkConfig[Suffix].EDH = UcharVal;
				TrxSDIBlkEDH( Suffix, UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SBEmbAudio															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SBEmbAudio() {

	if ( SDIBlkUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, OffSilencePar);

					if ( UcharVal > EMBAudioSilence)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( OffSilencePar[SDIBlkConfig[Suffix].EMBAudio].LongName);
				Udr_PutNewline();
			}
			else {
				SDIBlkConfig[Suffix].EMBAudio = UcharVal;
				TrxSDIBlkAudio( Suffix, UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SBCopy																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970317				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SBCopy() {

	if ( SDIBlkUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					switch ( UcharVal = 10*( Cmd[2] - '0') + Cmd[3] - '0') {
						case 34:
							UcharVal = SB34Item;
							break;

						case 56:
							UcharVal = SB56Item;
							break;

						case 78:
							UcharVal = SB78Item;
							break;

						default:
							SCPIError = IllegalParValue;
							break;
					};

					if ( SCPIError == NoError) {
						if ( strlen( Cmd) <= 4) {
							if ( !strncmp( Cmd, "SB", 2) && ( UcharVal <= SB78Item)) {
								if ( !SDIBlkUnit[UcharVal].Present)
									SCPIError = IllegalParValue;
							}
						}
					}
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( !CmdRequest) {
				memcpy( &SDIBlkConfig[Suffix], &SDIBlkConfig[UcharVal], sizeof( SDIBlkConfig[0]));

				ErrorCode = TrxSDIBlkSystem( Suffix, SDIBlkConfig[UcharVal].System);
				ErrorCode = TrxSDIBlkEDH( Suffix, SDIBlkConfig[UcharVal].EDH);
				ErrorCode = TrxSDIBlkAudio( Suffix, SDIBlkConfig[UcharVal].EMBAudio);
			}
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SBVersion															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SBVersion() {

	if ( SDIBlkUnit[Suffix].Present) {

		if ( CmdExecute && !SCPIError)
			if ( CmdRequest)
				WriteV24Version( SDIBlkUnit[Suffix].Address, 'H');
			else
				SCPIError = SyntaxErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	Analog																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/* Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void Analog() {

	if ( AnalogSignalUnit.Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				Udr_CPutNChar( AnalogPatternPar[AnalogConfig.Pattern].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( OffOnPar[AnalogConfig.TextOn].LongName);
				Udr_PutChar( ',');

				Udr_PutChar( '"');
				Udr_PutNChar( AnalogConfig.TextInfo);
				Udr_PutChar( '"');
				Udr_PutChar( ',');

				Udr_CPutNChar( AnalogTVSystemPar[AnalogConfig.System].LongName);
				Udr_PutChar( ',');

				AnalogSamplesToFLT( AnalogConfig.System, AnalogConfig.Delay,\
										  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutChar( ',');

				sprintf( TmpBuffer, "%d\r\n", AnalogConfig.ScHPhase);
				Udr_PutNChar( TmpBuffer);
			}
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnalogPattern														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AnalogPattern() {

	if ( AnalogSignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AnalogPatternPar);

					if ( UcharVal > BlackBurst)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AnalogPatternPar[AnalogConfig.Pattern].LongName);
				Udr_PutNewline();
			}
			else {
				if ( AnalogConfig.System < NTSC) {
					if ( UcharVal == SMPTECBar)
						SCPIError = ExecErr;
				}
				else {
					if (( UcharVal > SMPTECBar) && ( UcharVal < Window15))
						SCPIError = ExecErr;
				}

				if ( SCPIError == NoErr) {
					AnalogConfig.Pattern = UcharVal;
					TrxAnalogPattern( UcharVal);
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnalogTextInsert													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					ParState 2: String														*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AnalogTextInsert() {

	register UC i;

	if ( AnalogSignalUnit.Present) {
		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, OffOnPar);

					if ( UcharVal > ON)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = DataTypeErr;
				break;

			case P_String:
				if ( ParState == 0) {
					ParState += 2;

					if ( strlen( Cmd) > 8)
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = DataTypeErr;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( OffOnPar[AnalogConfig.TextOn].LongName);
				Udr_PutChar( ',');
				Udr_PutChar( '"');
				Udr_PutNChar( AnalogConfig.TextInfo);
				Udr_PutChar( '"');
				Udr_PutNewline();
			}
			else {
				if ( ParState == 1)
					AnalogConfig.TextOn = UcharVal;
				else {
					for ( i=0; i < strlen( Cmd); i++)
						if ( !CheckTextASCII( TRUE, Cmd[i]))
							break;

					if ( i == strlen( Cmd)) {
						strcpy( AnalogConfig.TextInfo, "        ");
						strncpy( AnalogConfig.TextInfo, Cmd, strlen( Cmd));

						TrxAnalogText( AnalogConfig.TextOn, AnalogConfig.TextInfo);
					}
					else
						SCPIError = ParErr;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnalogSystem														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AnalogSystem() {

	if ( AnalogSignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AnalogTVSystemPar);

					if ( UcharVal > NTSC)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AnalogTVSystemPar[AnalogConfig.System].LongName);
				Udr_PutNewline();
			}
			else {
				AnalogConfig.System = UcharVal;
				TrxAnalogSystem( UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnalogDelay															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AnalogDelay() {

	if ( AnalogSignalUnit.Present) {

		FindDelayParameters();

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				AnalogSamplesToFLT( AnalogConfig.System, AnalogConfig.Delay,\
										  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutNewline();
			}
			else {
				if ( ParState == 3) {
					UlongVal = AnalogFLTToSamples( AnalogConfig.System,\
															 Field, Line, HTime);

					if ( UlongVal != -1) {
						AnalogConfig.Delay = UlongVal;
						TrxAnalogDelay( AnalogConfig.Delay);
					}
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = DataTypeErr;
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnalogScHPhase														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AnalogScHPhase() {

	if ( AnalogSignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				if ( ParState == 0) {
					ParState++;

					IntVal = atoi( Cmd);

					if (( IntVal < ScHPhaseLow) || ( IntVal > ScHPhaseHigh))
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				sprintf( TmpBuffer, "%d\r\n", AnalogConfig.ScHPhase);
				Udr_PutNChar( TmpBuffer);
			}
			else {
				AnalogConfig.ScHPhase = IntVal;
				TrxAnalogScHPhase( IntVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnalogVersion														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AnalogVersion() {

	if ( AnalogSignalUnit.Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteV24Version( AnalogSignalUnit.Address, 'A');
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDI																	 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/* Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDI() {

	if ( SDISignalUnit.Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				Udr_CPutNChar( SDIPatternPar[SDIConfig.Pattern].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( OffOnPar[SDIConfig.TextOn].LongName);
				Udr_PutChar( ',');

				Udr_PutChar( '"');
				Udr_PutNChar( SDIConfig.TextInfo);
				Udr_PutChar( '"');
				Udr_PutChar( ',');

				Udr_CPutNChar( DigitalTVSystemPar[SDIConfig.System].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( OffOnPar[SDIConfig.EDH].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( AudioSignalPar[SDIConfig.AudioSignal].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( AudioLevelPar[SDIConfig.AudioLevel].LongName);
				Udr_PutChar( ',');

				SDISamplesToFLT( SDIConfig.System, SDIConfig.Delay,\
									  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutNewline();
			}
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDIPattern															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDIPattern() {

	if ( SDISignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, SDIPatternPar);

					if ( UcharVal > SDIRed75)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( SDIPatternPar[SDIConfig.Pattern].LongName);
				Udr_PutNewline();
			}
			else {
				if ( Connectors[SDISConn].Type == PT8602) {
					if ( SDIConfig.Pattern > SDIBlack)
						SCPIError = HardwareMissing;
				}
				else {
					if ( SDIConfig.System == SDI625) {
						if ( UcharVal == SDISMPTECBar)
							SCPIError = ExecErr;
					}
					else {
						if (( UcharVal > SDISMPTECBar) && ( UcharVal < SDICheck))
							SCPIError = ExecErr;
					}
				}

				if ( SCPIError == NoErr) {
					SDIConfig.Pattern = UcharVal;
					TrxSDIPattern( UcharVal);
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITextInsert														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					ParState 2: String														*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDITextInsert() {

	register UC i;

	if ( SDISignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, OffOnPar);

					if ( UcharVal > ON)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = DataTypeErr;
				break;

			case P_String:
				if ( ParState == 0) {
					ParState += 2;

					if ( strlen( Cmd) > 8)
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = DataTypeErr;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( OffOnPar[SDIConfig.TextOn].LongName);
				Udr_PutChar( ',');
				Udr_PutChar( '"');
				Udr_PutNChar( SDIConfig.TextInfo);
				Udr_PutChar( '"');
				Udr_PutNewline();
			}
			else {
				if ( ParState == 1) {
					SDIConfig.TextOn = UcharVal;
					TrxSDIText( SDIConfig.TextOn, SDIConfig.TextInfo);
				}
				else {
					for ( i=0; i < strlen( Cmd); i++)
						if ( !CheckTextASCII( TRUE, Cmd[i]))
							break;

					if ( i == strlen( Cmd)) {
						strcpy( SDIConfig.TextInfo, "        ");

						strncpy( SDIConfig.TextInfo, Cmd, strlen( Cmd));
						TrxSDIText( SDIConfig.TextOn, SDIConfig.TextInfo);
					}
					else
						SCPIError = ParErr;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDISystem															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDISystem() {

	if ( SDISignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, DigitalTVSystemPar);

					if ( UcharVal > SDI525)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( DigitalTVSystemPar[SDIConfig.System].LongName);
				Udr_PutNewline();
			}
			else {
				SDIConfig.System = UcharVal;
				TrxSDISystem( UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDIEDHInsert														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDIEDHInsert() {

	if ( SDISignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, OffOnPar);

					if ( UcharVal > ON)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( OffOnPar[SDIConfig.EDH].LongName);
				Udr_PutNewline();
			}
			else {
				SDIConfig.EDH = UcharVal;
				TrxSDIEDH( UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDIDelay																 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDIDelay() {

	if ( SDISignalUnit.Present) {

		FindDelayParameters();

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				SDISamplesToFLT( SDIConfig.System, SDIConfig.Delay,\
									  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutNewline();
			}
			else {
				if ( ParState == 3) {
					UlongVal = SDIFLTToSamples( SDIConfig.System,\
														 Field, Line, HTime);

					if ( UlongVal != -1) {
						SDIConfig.Delay = UlongVal;
						TrxSDIDelay( SDIConfig.Delay);
					}
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = DataTypeErr;
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDIVersion															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDIVersion() {

	if ( SDISignalUnit.Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteV24Version( SDISignalUnit.Address, 'S');
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDIAudioSignal														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDIAudioSignal() {

	if ( SDISignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AudioSignalPar);

					if ( UcharVal > AudioDual)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
			break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AudioSignalPar[SDIConfig.AudioSignal].LongName);
				Udr_PutNewline();
			}
			else {
				SDIConfig.AudioSignal = UcharVal;
				TrxSDIAudioSignal( UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDIAudioLevel														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDIAudioLevel() {

	if ( SDISignalUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AudioLevelPar);

					if ( UcharVal > AudioLevel20dB)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AudioLevelPar[SDIConfig.AudioLevel].LongName);
				Udr_PutNewline();
			}
			else {
				SDIConfig.AudioLevel = UcharVal;
				TrxSDIAudioLevel( UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	Audio																	 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970317				     													*/
/*																									*/
/*	Function:																					*/
/* Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void Audio() {

	if ( AES_EBUUnit.Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AudioSignalPar[AES_EBUConfig.Signal].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( AudioLevelPar[AES_EBUConfig.Level].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( AudioTimingPar[AES_EBUConfig.Timing].LongName);
				Udr_PutNewline();
			}
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AudioSignal															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Handle the signal in the AES/EBU audio option					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AudioSignal() {

	if ( AES_EBUUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AudioSignalPar);

					if (( UcharVal < AudioStereo800Hz) || ( UcharVal > Audio48kHz))
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AudioSignalPar[AES_EBUConfig.Signal].LongName);
				Udr_PutNewline();
			}
			else {
				AES_EBUConfig.Signal = UcharVal;
				AES_EBUUnitDrv( UcharVal, AES_EBUConfig.Level, AES_EBUConfig.Timing);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AudioLevel															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Handle the level in the AES/EBU audio option						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AudioLevel() {

	if ( AES_EBUUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AudioLevelPar);

					if ( UcharVal > AudioLevel20dB)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AudioLevelPar[AES_EBUConfig.Level].LongName);
				Udr_PutNewline();
			}
			else {
				AES_EBUConfig.Level = UcharVal;
				AES_EBUUnitDrv( AES_EBUConfig.Signal, UcharVal, AES_EBUConfig.Timing);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AudioTiming															 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Handle the timing in the AES/EBU audio option					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void AudioTiming() {

	if ( AES_EBUUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AudioTimingPar);

					if ( UcharVal > AudioNTSCPhase5)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AudioTimingPar[AES_EBUConfig.Timing].LongName);
				Udr_PutNewline();
			}
			else {
				AES_EBUConfig.Timing = UcharVal;
				AES_EBUUnitDrv( AES_EBUConfig.Signal, AES_EBUConfig.Level, UcharVal);
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AudioVersion														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Return information about the AES/EBU audio option				*/
/*	Remarks:																						*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void AudioVersion() {

	if ( AES_EBUUnit.Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteIICVersion( IICBus2, soundram_addr);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	TimeCodeVersion													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970216    											*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:	Return information about the time code option					*/
/*	Remarks:																						*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void TimeCodeVersion() {

	if ( TimeCodeUnit.Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteIICVersion( IICBus1, timeram_addr);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBMultiVersion														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BBMultiVersion() {

	if ( Connectors[BB34Conn].Type == PT8604) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteIICVersion( IICBus1, multibb_addr);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDIGenlockVersion													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970316				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void SDIGenlockVersion() {

	if ( SDIGenlockUnit.Present) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteIICVersion( IICBus1, d1ram_addr);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	FactoryPassword													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		970206				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					ParState 2: String														*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryPassword() {

	register UC i;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_Char:
			if ( ParState == 0) {
				ParState++;

				UcharVal = FindParameter( Cmd, OffOnPar);

				if ( UcharVal > ON)
					SCPIError = DataOutOfRange;
			}
			else
				SCPIError = DataTypeErr;
			break;

		case P_String:
			if ( ParState == 0) {
				ParState += 2;

				if ( strlen( Cmd) > 20)
					SCPIError = DataOutOfRange;
			}
			else
				SCPIError = DataTypeErr;
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			Udr_CPutNChar( OffOnPar[PasswordState].LongName);
			Udr_PutNewline();
		}
		else {
			if (( ParState == 1) && ( UcharVal == ON))
				PasswordState = ON;
			else {
				i = 0;
				while ( Cmd[i])
					Cmd[i++] -= 10;

				if ( !Cstrcmp( Cmd, FactoryPasswordTxt))
					PasswordState = OFF;
			}
		}
	}
}


/***************************************************************************/
/*	FactoryAddress														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryAddress() {

	if ( PasswordState == OFF) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				if ( ParState == 0) {
					ParState++;

					if ( atoi( Cmd) < 256)
						UcharVal = ( UC) atoi( Cmd);
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState = 2;

					UcharVal = FindParameter( Cmd, FactoryAddressPar);

					if ( UcharVal > 6)
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				sprintf( TmpBuffer, "%d\r\n", UnitAddress);
				Udr_PutNChar( TmpBuffer);
			}
			else {
				if ( ParState == 1)
					UnitAddress = UcharVal;
				else
					UnitAddress = V24CRAMAddressTable[UcharVal];
			}
		}
	}
}

/***************************************************************************/
/*	FactoryCommand														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970218				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  String                    ;,?,$						*/
/*					ParState 2:  String, String				;    						*/
/*					ParState 3:  String, DecNum				;,?,($)					*/
/*					ParState 4:  String, DecNum, String		;    						*/
/*					ParState 5:  String, DecNum, DecNum		;						   */
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryCommand() {

	if ( PasswordState == OFF) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_String:
				switch ( ParState) {
					case 0:
						ParState++;

						if ( strlen( Cmd) < 4)
							strcpy( CmdBuffer, Cmd);
						else
							SCPIError = DataOutOfRange;
						break;

					case 1:
					case 3:
						ParState++;

						if ( strlen( Cmd) < 11)
							strcpy( TmpBuffer, Cmd);
						else
							SCPIError = DataOutOfRange;
						break;

					default:
						SCPIError = ParNotAllowed;
						break;
				}
				break;

			case P_DecNum:
				switch ( ParState) {
					case 1:
						ParState = 3;

						if ( strlen( Cmd) < 11)
							UlongVal = atol( Cmd);
						else
							SCPIError = DataOutOfRange;
						break;

					case 3:
						ParState = 5;

						if ( strlen( Cmd) < 7)
							IntVal = atoi( Cmd);
						else
							SCPIError = DataOutOfRange;
						break;

					default:
						SCPIError = ParNotAllowed;
						break;
				}
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_PutNChar( CmdBuffer);
				Udr_PutNewline();
			}
			else {
				switch ( CmdBuffer[2]) {
					case '?':
						if (( ParState == 1) || ( ParState == 3)) {

							if ( ParState == 3)
								UcharVal = SndRequest_n( UnitAddress, CmdBuffer, ( UC) UlongVal);
							else
								UcharVal = SndRequest( UnitAddress, CmdBuffer);

							if ( UcharVal == 0) {
								if (( UcharVal = AnswerCheck()) == 0) {
									Udr_PutNChar( AnswerBuf);
									Udr_PutNewline();
								}
								else
									UcharVal += 2;
							}
						}
						else
							SCPIError = ExecErr;
						break;

					case '$':
						if ( ParState == 1)
							UcharVal = SndSpecial( UnitAddress, CmdBuffer, '$');
						else
							SCPIError = ExecErr;
						break;

					default:
						switch ( ParState) {
							case 1:
								UcharVal = SndVoid( UnitAddress, CmdBuffer);
								break;

							case 2:
								UcharVal = SndAsc( UnitAddress, CmdBuffer, TmpBuffer);
								break;

							case 3:
								UcharVal = SndLong( UnitAddress, CmdBuffer, UlongVal);
								break;

							case 4:
								UcharVal = SndAsc_n( UnitAddress, CmdBuffer, ( UC) UlongVal, TmpBuffer);
								break;

							case 5:
								UcharVal = SndInt2( UnitAddress, CmdBuffer, ( UI) UlongVal, IntVal);
								break;
						}
						break;
				}
			}

			if ( UcharVal)
				SCPIError = -380 - UcharVal;
		}
	}
}

/***************************************************************************/
/*	FactorySPGUserText												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactorySPGUserText() {

	if ( PasswordState == OFF) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				if ( ParState == 0) {
					ParState++;

					if ( strlen( Cmd) < 2)
						UcharVal = ( UC) atol( Cmd);
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			case P_String:
				if ( ParState == 1) {
					ParState++;

					if ( strlen( Cmd) < 11)
						strcpy( TmpBuffer, Cmd);
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				if ( ParState == 1) {
					IIC2_RdStr( spgram_addr, (UC)(UserText_addr+10*UcharVal), TmpBuffer);

					Udr_PutChar( '"');
					Udr_PutNChar( TmpBuffer);
					Udr_PutChar( '"');
					Udr_PutNewline();
				}
				else
					SCPIError = DataTypeErr;
			}
			else {
				if ( ParState == 2)
					IIC2_WrStr( spgram_addr, (UC)(UserText_addr+10*UcharVal), TmpBuffer);
				else
					SCPIError = DataTypeErr;
			}
		}
	}
}

/***************************************************************************/
/*	FactorySPGStore													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactorySPGStore() {

	UL HPHZero_G, HPHZero_M;
	UI GenlPhase_G, GenlPhase_M;

	if ( PasswordState == OFF) {

		if ( CmdExecute && !SCPIError) {

			if (( UcharVal = SndRequest( sync_addr, "GA?")) == 0) {
				if (( UcharVal = AnswerCheck()) == 0) {
					HPHZero_G = atol( AnswerBuf);

					if (( UcharVal = SndRequest( sync_addr, "GF?")) == 0) {
						if (( UcharVal = AnswerCheck()) == 0) {
							GenlPhase_G = atol( AnswerBuf);

							if (( UcharVal = SndRequest( sync_addr, "GU?")) == 0) {
								if (( UcharVal = AnswerCheck()) == 0) {
									HPHZero_M = atol( AnswerBuf);

									if (( UcharVal = SndRequest( sync_addr, "GG?")) == 0) {
										if (( UcharVal = AnswerCheck()) == 0)
											GenlPhase_M = atol( AnswerBuf);
									}
								}
							}
						}
					}
				}
			}

			if ( UcharVal == 0) {
				if ( CmdRequest) {
					_ultoa( HPHZero_G, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');
					_ultoa( GenlPhase_G, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');
					_ultoa( HPHZero_M, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');
					_ultoa( GenlPhase_M, TmpBuffer);
					Udr_PutNChar( TmpBuffer);
					Udr_PutNewline();
				}
				else {
					IIC2_WrLongInt( spgram_addr, HPHZero_G_addr, HPHZero_G, GenlPhase_G);
					IIC2_WrLongInt( spgram_addr, HPHZero_M_addr, HPHZero_M, GenlPhase_M);
				}
			}
			else
				SCPIError = DeviceSpecificErr;
		}
	}
}

/***************************************************************************/
/*	FactorySPGRead														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactorySPGRead() {

	if ( PasswordState == OFF) {

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				_ultoa( IIC2_RdLong( spgram_addr, HPHZero_G_addr), TmpBuffer);
				Udr_PutNChar( TmpBuffer);

				Udr_PutChar( ',');
				_ultoa( IIC2_RdInt( spgram_addr, GenlPhase_G_addr), TmpBuffer);
				Udr_PutNChar( TmpBuffer);

				Udr_PutChar( ',');
				_ultoa( IIC2_RdLong( spgram_addr, HPHZero_M_addr), TmpBuffer);
				Udr_PutNChar( TmpBuffer);

				Udr_PutChar( ',');
				_ultoa( IIC2_RdInt( spgram_addr, GenlPhase_M_addr), TmpBuffer);
				Udr_PutNChar( TmpBuffer);
				Udr_PutNewline();
			}
		}
	}
}

/***************************************************************************/
/*	FactoryAudioUserText												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970107				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryAudioUserText() {

	if ( PasswordState == OFF) {

		if ( AES_EBUUnit.Present) {

			switch ( ParameterType) {
				case P_NoParameter:
					ParState = 0;
					break;

				case P_DecNum:
					if ( ParState == 0) {
						ParState++;

						if ( strlen( Cmd) < 3) {
							if (( UcharVal = ( UC) atol( Cmd)) > 19)
								SCPIError = DataOutOfRange;
						}
						else
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = ParNotAllowed;
					break;

				case P_String:
					if ( ParState == 1) {
						ParState++;

						if ( strlen( Cmd) < 11)
							strcpy( TmpBuffer, Cmd);
						else
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = ParNotAllowed;
					break;

				default:
					SCPIError = DataTypeErr;
					break;
			}

			if ( CmdExecute && !SCPIError) {
				if ( CmdRequest) {
					if ( ParState == 1) {
						IIC2_RdStr( soundram_addr, (UC)( 10*UcharVal), TmpBuffer);

						Udr_PutChar( '"');
						Udr_PutNChar( TmpBuffer);
						Udr_PutChar( '"');
						Udr_PutNewline();
					}
					else
						SCPIError = DataTypeErr;
				}
				else {
					if ( ParState == 2)
						IIC2_WrStr( soundram_addr, (UC)( 10*UcharVal), TmpBuffer);
					else
						SCPIError = DataTypeErr;
				}
			}
		}
		else
			SCPIError = HardwareMissing;
	}
}

/***************************************************************************/
/*	FactoryAudioWrite													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970122				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryAudioWrite() {

	if ( PasswordState == OFF) {

		if ( AES_EBUUnit.Present) {
		}
		else
			SCPIError = HardwareMissing;
	}
}

/***************************************************************************/
/*	FactoryAudioRead													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970122				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryAudioRead() {

	UC tmp;

	if ( PasswordState == OFF) {

		if ( AES_EBUUnit.Present) {

			if ( CmdExecute && !SCPIError) {
				if ( CmdRequest) {

					tmp = IIC2_Read( sound1_addr);

					feed_dog;

					_ultoa( tmp, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');

					tmp = IIC2_Read( sound2_addr);

					feed_dog;
					_ultoa( tmp, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutNewline();
				}
			}
		}
		else
			SCPIError = HardwareMissing;
	}
}

/***************************************************************************/
/*	FactoryMainUserText												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970109	    										*/
/* Revised:		970109				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryMainUserText() {

	if ( PasswordState == OFF) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				if ( ParState == 0) {
					ParState++;

					if ( strlen( Cmd) < 3) {
						if (( UcharVal = ( UC) atol( Cmd)) > 19)
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			case P_String:
				if ( ParState == 1) {
					ParState++;

					if ( strlen( Cmd) < 11)
						strcpy( TmpBuffer, Cmd);
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {

			if ( CmdRequest) {
				if ( ParState == 1) {

					IIC2_RdStr( mainram1_addr, (UC)( 10*UcharVal), TmpBuffer);

					Udr_PutChar( '"');
					Udr_PutNChar( TmpBuffer);
					Udr_PutChar( '"');
					Udr_PutNewline();
				}
				else
					SCPIError = DataTypeErr;
			}
			else {
				if ( ParState == 2)
					IIC2_WrStr( mainram1_addr, (UC)( 10*UcharVal), TmpBuffer);
				else
					SCPIError = DataTypeErr;
			}
		}
	}
}

/***************************************************************************/
/*	FactoryMainPSNSuffix												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970717	    										*/
/* Revised:		970717				     													*/
/*																									*/
/*	Function:																					*/
/* Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryMainPSNSuffix() {

	if ( NumSuffix != 5)
		SCPIError = HdrSuffixOutOfRange;
}

/***************************************************************************/
/*	FactoryMainPSN														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970717	    										*/
/* Revised:		970717				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryMainPSN() {

	if ( PasswordState == OFF) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				switch ( ParState) {
					case 0:
						ParState++;

						if (( strlen( Cmd) < 4) && ( atoi( Cmd) < 256))
							UcharVal = ( UC) atoi( Cmd);
						else
							SCPIError = DataOutOfRange;
						break;

					case 1:
						ParState++;

						if (( strlen( Cmd) < 4) && ( atoi( Cmd) < 256))
							UcharVal1 = ( UC) atoi( Cmd);
						else
							SCPIError = DataOutOfRange;
						break;

					default:
						SCPIError = ParNotAllowed;
						break;
				}
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {

			if ( CmdRequest) {
				if ( ParState == 0) {

					IIC2_Write( mainram1_addr, VoltN5Min_addr);
					UcharVal = IIC2_Read( mainram1_addr);

					_ultoa(( UL) UcharVal, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');

					UcharVal = IIC2_Read( mainram1_addr);

					_ultoa(( UL) UcharVal, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutNewline();
				}
				else
					SCPIError = DataTypeErr;
			}
			else {
				if ( ParState == 2) {
					IIC2_Write2( mainram1_addr, VoltN5Min_addr, UcharVal);
					IIC2_Write2( mainram1_addr, VoltN5Max_addr, UcharVal1);
				}
				else
					SCPIError = DataTypeErr;
			}
		}
	}
}

/***************************************************************************/
/*	FactoryMainPSSuffix												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970717	    										*/
/* Revised:		970717				     													*/
/*																									*/
/*	Function:																					*/
/* Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryMainPSSuffix() {

	if ( NumSuffix != 12)
		SCPIError = HdrSuffixOutOfRange;
}

/***************************************************************************/
/*	FactoryMainPS														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970717	    										*/
/* Revised:		970717				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryMainPS() {

	if ( PasswordState == OFF) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				if ( ParState < 2) {
					ParState++;

					IntVal = atoi( Cmd);

					if (( strlen( Cmd) < 4) && ( IntVal < 256)) {
						if ( ParState == 1)
							UcharVal = ( UC) IntVal;
						else
							UcharVal1 = ( UC) IntVal;
					}
					else
						SCPIError = DataOutOfRange;
				}
				else
					SCPIError = ParNotAllowed;
				break;
		}

		if ( CmdExecute && !SCPIError) {

			if ( CmdRequest) {
				if ( ParState == 0) {

					IIC2_Write( mainram1_addr, Volt12Min_addr);
					UcharVal = IIC2_Read( mainram1_addr);

					_ultoa(( UL) UcharVal, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');

					UcharVal = IIC2_Read( mainram1_addr);

					_ultoa(( UL) UcharVal, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutNewline();
				}
				else
					SCPIError = DataTypeErr;
			}
			else {
				if ( ParState == 2) {
					IIC2_Write2( mainram1_addr, Volt12Min_addr, UcharVal);
					IIC2_Write2( mainram1_addr, Volt12Max_addr, UcharVal1);
				}
				else
					SCPIError = DataTypeErr;
			}
		}
	}
}

/***************************************************************************/
/*	FactorySDIGenlockUserText										 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970109	    										*/
/* Revised:		970109				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactorySDIGenlockUserText() {

	if ( PasswordState == OFF) {

		if ( SDIGenlockUnit.Present) {

			switch ( ParameterType) {
				case P_NoParameter:
					ParState = 0;
					break;

				case P_DecNum:
					if ( ParState == 0) {
						ParState++;

						if ( strlen( Cmd) < 3) {
							if (( UcharVal = ( UC) atol( Cmd)) > 19)
								SCPIError = DataOutOfRange;
						}
						else
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = ParNotAllowed;
					break;

				case P_String:
					if ( ParState == 1) {
						ParState++;

						if ( strlen( Cmd) < 11)
							strcpy( TmpBuffer, Cmd);
						else
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = ParNotAllowed;
					break;

				default:
					SCPIError = DataTypeErr;
					break;
			}

			if ( CmdExecute && !SCPIError) {

				if ( CmdRequest) {
					if ( ParState == 1) {

						IIC1_RdStr( d1ram_addr, (UC)( 10*UcharVal), TmpBuffer);

						Udr_PutChar( '"');
						Udr_PutNChar( TmpBuffer);
						Udr_PutChar( '"');
						Udr_PutNewline();
					}
					else
						SCPIError = DataTypeErr;
				}
				else {
					if ( ParState == 2)
						IIC1_WrStr( d1ram_addr, (UC)( 10*UcharVal), TmpBuffer);
					else
						SCPIError = DataTypeErr;
				}
			}
		}
		else
			SCPIError = HardwareMissing;
	}
}

/***************************************************************************/
/*	FactoryTimeCodeUserText											 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970109	    										*/
/* Revised:		970109				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryTimeCodeUserText() {

	if ( PasswordState == OFF) {

		if ( TimeCodeUnit.Present) {

			switch ( ParameterType) {
				case P_NoParameter:
					ParState = 0;
					break;

				case P_DecNum:
					if ( ParState == 0) {
						ParState++;

						if ( strlen( Cmd) < 3) {
							if (( UcharVal = ( UC) atol( Cmd)) > 19)
								SCPIError = DataOutOfRange;
						}
						else
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = ParNotAllowed;
					break;

				case P_String:
					if ( ParState == 1) {
						ParState++;

						if ( strlen( Cmd) < 11)
							strcpy( TmpBuffer, Cmd);
						else
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = ParNotAllowed;
					break;

				default:
					SCPIError = DataTypeErr;
					break;
			}

			if ( CmdExecute && !SCPIError) {

				if ( CmdRequest) {
					if ( ParState == 1) {

						IIC1_RdStr( timeram_addr, (UC)( 10*UcharVal), TmpBuffer);

						Udr_PutChar( '"');
						Udr_PutNChar( TmpBuffer);
						Udr_PutChar( '"');
						Udr_PutNewline();
					}
					else
						SCPIError = DataTypeErr;
				}
				else {
					if ( ParState == 2)
						IIC1_WrStr( timeram_addr, (UC)( 10*UcharVal), TmpBuffer);
					else
						SCPIError = DataTypeErr;
				}
			}
		}
		else
			SCPIError = HardwareMissing;
	}
}

/***************************************************************************/
/*	FactoryMultiBBUserText											 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970109	    										*/
/* Revised:		970109				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryMultiBBUserText() {

	if ( PasswordState == OFF) {

		if ( Connectors[BB34Conn].Type == PT8604) {

			switch ( ParameterType) {
				case P_NoParameter:
					ParState = 0;
					break;

				case P_DecNum:
					if ( ParState == 0) {
						ParState++;

						if ( strlen( Cmd) < 3) {
							if (( UcharVal = ( UC) atol( Cmd)) > 19)
								SCPIError = DataOutOfRange;
						}
						else
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = ParNotAllowed;
					break;

				case P_String:
					if ( ParState == 1) {
						ParState++;

						if ( strlen( Cmd) < 11)
							strcpy( TmpBuffer, Cmd);
						else
							SCPIError = DataOutOfRange;
					}
					else
						SCPIError = ParNotAllowed;
					break;

				default:
					SCPIError = DataTypeErr;
					break;
			}

			if ( CmdExecute && !SCPIError) {

				if ( CmdRequest) {
					if ( ParState == 1) {

						IIC1_RdStr( multibb_addr, (UC)( 10*UcharVal), TmpBuffer);

						Udr_PutChar( '"');
						Udr_PutNChar( TmpBuffer);
						Udr_PutChar( '"');
						Udr_PutNewline();
					}
					else
						SCPIError = DataTypeErr;
				}
				else {
					if ( ParState == 2)
						IIC1_WrStr( multibb_addr, (UC)( 10*UcharVal), TmpBuffer);
					else
						SCPIError = DataTypeErr;
				}
			}
		}
		else
			SCPIError = HardwareMissing;
	}
}

/***************************************************************************/
/*	FactoryADCRead														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970122	    										*/
/* Revised:		970127				     													*/
/*																									*/
/*	Function:	Write content of all A/D converters to RS232						*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void FactoryADCRead() {

	if ( PasswordState == OFF)

		if ( CmdExecute && CmdRequest && !SCPIError) {

			_ultoa( TempMeas, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			_ultoa( VoltN5Meas, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			_ultoa( Volt5Meas, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			_ultoa( Volt12Meas, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutNewline();
		}
}

/***************************************************************************/
/*	FactoryLDetectorRead												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970122	    										*/
/* Revised:		970122				     													*/
/*																									*/
/*	Function:	Write content of level detectors to RS232							*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void FactoryLDetectorRead() {

	if ( PasswordState == OFF)

		if ( CmdExecute && CmdRequest && !SCPIError) {

			_ultoa( lev1det, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			_ultoa( lev2det, TmpBuffer);
			Udr_PutNChar( TmpBuffer);
			Udr_PutNewline();
		}
}

/***************************************************************************/
/*	FindDelayParameters												 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:	Get Field, Line and HTime from RS232 command						*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*					ParState 3:  DecNum, DecNum, DecNum									*/
/*	Returns:		-																				*/
/*	Updates:		Field, Line, HTime														*/
/***************************************************************************/
void FindDelayParameters() {

	switch ( ParameterType) {

		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			switch ( ParState) {
				case 0:
					ParState++;

					if ( strlen( Cmd) < 3)
						Field = ( char) atoi( Cmd);
					else
						SCPIError = DataOutOfRange;
					break;

				case 1:
					ParState++;

					if ( strlen( Cmd) < 5)
						Line = ( int) atoi( Cmd);
					else
						SCPIError = DataOutOfRange;
					break;

				case 2:
					ParState++;

					if ( strlen( Cmd) < 10)
						HTime = (float) atof( Cmd);
					else
						SCPIError = DataOutOfRange;
					break;

				default:
					SCPIError = ParNotAllowed;
					break;
			}
			break;

		default:
			SCPIError = DataTypeErr;
			break;
	}
}

/***************************************************************************/
/*	FindParameter														 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970724				     													*/
/*																									*/
/*	Function:	Find the character defined parameter								*/
/*	Remarks:		-																				*/
/*	Returns:		The array number containing the parameter or max. array+1	*/
/*	Updates:		-																				*/
/***************************************************************************/
UC FindParameter( char* par, code struct PAR_STRUCT* ParPtr) {

	UC tmp = 0;

	while ( ParPtr) {
		if (( !Cstrcmp( par, ParPtr->LongName)) ||
			 ( !Cstrcmp( par, ParPtr->ShortName)))
			break;

		ParPtr = ParPtr->SameLevel;

		if ( tmp < 255)
			tmp++;
		else
			break;
	}

	return( tmp);
}

/***************************************************************************/
/*	WriteV24Version													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970216	    										*/
/* Revised:		970225				     													*/
/*																									*/
/*	Function:	Write the companyname, productname, KU number and software	*/
/*					version from a	V24 operated unit to RS232							*/
/*	Remarks:		-																				*/
/*	Returns:		errorcode																	*/
/*	Updates:		-																				*/
/***************************************************************************/
UC WriteV24Version( UC address, char unit) {

	char v24cmd[4] = { 'x','x','?', 0 };

	v24cmd[0] = unit;
	v24cmd[1] = 'R';

	UcharVal = SndRequest_n( address, v24cmd, CompanyName_addr/10);

	if ( UcharVal == 0)
		if (( UcharVal = AnswerCheck()) == 0) {

			AnswerBuf[ strlen(AnswerBuf)-1] = 0;

			Udr_PutNChar( &AnswerBuf[1]);
			Udr_PutChar( ',');
		}

	UcharVal = SndRequest_n( address, v24cmd, ProductName1_addr/10);

	if ( UcharVal == 0)
		if (( UcharVal = AnswerCheck()) == 0) {

			AnswerBuf[ strlen(AnswerBuf)-1] = 0;

			Udr_PutNChar( &AnswerBuf[1]);
			Udr_PutChar( ',');
		}

	if ( strlen( AnswerBuf) >= 10) {

		UcharVal = SndRequest_n( address, v24cmd, ProductName2_addr/10);

		if ( UcharVal == 0)
			if (( UcharVal = AnswerCheck()) == 0) {

				AnswerBuf[ strlen(AnswerBuf)-1] = 0;

				Udr_PutNChar( &AnswerBuf[1]);
				Udr_PutChar( ',');
			}
	}

	UcharVal = SndRequest_n( address, v24cmd, ProductKUNo_addr/10);

	if ( UcharVal == 0)
		if (( UcharVal = AnswerCheck()) == 0) {

			AnswerBuf[ strlen(AnswerBuf)-1] = 0;

			Udr_PutNChar( &AnswerBuf[1]);
			Udr_PutChar( ',');
		}

	v24cmd[1] = 'I';

	UcharVal = SndRequest( address, v24cmd);

	if ( UcharVal == 0)
		if (( UcharVal = AnswerCheck()) == 0) {

			TmpBuffer[0] = '0' + ( atoi( AnswerBuf) / 10);
			TmpBuffer[1] = '.';
			TmpBuffer[2] = '0' + ( atoi( AnswerBuf) % 10);
			TmpBuffer[3] = 0x00;

			Udr_PutNChar( TmpBuffer);
		}

	Udr_PutNewline();

	return( 0);
}

/***************************************************************************/
/*	WriteIICVersion													 	  RS232CMD.C51	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970216	    										*/
/* Revised:		970625				     													*/
/*																									*/
/*	Function:	Write the companyname, productname and KU number from a		*/
/*					IIC operated unit to RS232												*/
/*	Remarks:		Software version does NOT exist for these units					*/
/*	Returns:		errorcode																	*/
/*	Updates:		-																				*/
/***************************************************************************/
UC WriteIICVersion( UC bus, UC address) {

	if ( bus == IICBus2)
		IIC2_RdStr( address, CompanyName_addr, TmpBuffer);
	else
		IIC1_RdStr( address, CompanyName_addr, TmpBuffer);

	Udr_PutNChar( TmpBuffer);

	Udr_PutChar( ',');

	if ( bus == IICBus2)
		IIC2_RdStr( address, ProductName1_addr, TmpBuffer);
	else
		IIC1_RdStr( address, ProductName1_addr, TmpBuffer);

	Udr_PutNChar( TmpBuffer);

	if ( strlen( TmpBuffer) >= 10) {

		if ( bus == IICBus2)
			IIC2_RdStr( address, ProductName2_addr, TmpBuffer);
		else
			IIC1_RdStr( address, ProductName2_addr, TmpBuffer);

		Udr_PutNChar( TmpBuffer);
	}
	Udr_PutChar( ',');

	if ( bus == IICBus2)
		IIC2_RdStr( address, ProductKUNo_addr, TmpBuffer);
	else
		IIC1_RdStr( address, ProductKUNo_addr, TmpBuffer);

	Udr_PutNChar( TmpBuffer);

	Udr_PutNChar( ",0");				// Software version not applicable

	Udr_PutNewline();

	return( 0);
}
