/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 2000            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:   RS232CMD.H                                  */
/*  Author:   Kim Engedahl, DEV                             */
/*  Org. date:  980211                                    */
/*  Rev. date:  000512, KEn, DEV                              */
/*  Status:   Version 1.5                                 */
/*                                                  */
/*  Changes:                                            */
/* 000512: Added support for TPG text positioning                 */
/* 991026: Impklemented "DIAG:ERR"                            */
/* 990420: Added Window 10% & 15 kHz Bl/Wh in both SDI & Analog.        */
/* 990317: Replaced WriteIICVersion() & WriteV24Version() with WriteUnit- */
/*        Information()                                 */
/* 990317: Revised parameters for WriteV24Version()               */
/* 980929: Added AnlTPGAttribCircles(). Increased AnlTPGLevel3 with 1.    */
/* 980917: Increased SDIPatternPar[] from 61 to 65                  */
/* 980917: Increased AnlPatternPar[] from 48 to 54                  */
/* 980730: Increased FactoryLevel6[] & added FactoryTimeClockStatus     */
/* 980702: Update TimeClockRefPar                           */
/* 980616: Released as 4008 002 06812                         */
/* 980604: Increased FactoryLevel3                            */
/* 980514: Released as 4008 002 06811                         */
/***************************************************************************/

typedef struct CMD_STRUCT
  {
  code char* LongName;
  code char* ShortName;

  code struct CMD_STRUCT* SameLevel;
  code struct CMD_STRUCT* NextLevel;

  void ( *Suffix)();
  void ( *Function)();
  };

typedef struct PAR_STRUCT
  {
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
extern code struct CMD_STRUCT DiagnosticLevel1[3];

extern code struct CMD_STRUCT Display[1];

extern code struct CMD_STRUCT Input[2];

extern code struct CMD_STRUCT GenlockLevel1[3];
extern code struct CMD_STRUCT GenlockLevel2[1];

extern code struct CMD_STRUCT Output[11];

extern code struct CMD_STRUCT LTCLevel1[5];
extern code struct CMD_STRUCT LTCTimeLevel[2];
extern code struct CMD_STRUCT LTCDaylightLevel[3];

extern code struct CMD_STRUCT TLGLevel1[3];
extern code struct CMD_STRUCT HDAttirbLevel1[1];
extern code struct CMD_STRUCT HDAttribLevel2[26];
extern code struct CMD_STRUCT DLAttirbLevel1[1];
extern code struct CMD_STRUCT DLINterfLevel1[1];

extern code struct CMD_STRUCT HDLevel1[6];
extern code struct CMD_STRUCT DLLevel1[6];
extern code struct CMD_STRUCT HDEMBALevel[3]; // ?
extern code struct CMD_STRUCT DLEMBALevel[3];
extern code struct CMD_STRUCT HDTextLevel[6];
extern code struct PAR_STRUCT HDTextColotPar[8];
extern code struct CMD_STRUCT DLTextLevel[6];
extern code struct CMD_STRUCT DLTextLevel[6];

extern code struct CMD_STRUCT BBLevel1[4];

extern code struct CMD_STRUCT AnlTPGLevel1[6];
extern code struct CMD_STRUCT AnlTPGLevel2[1];
extern code struct CMD_STRUCT AnlTPGLevel3[4];
extern code struct CMD_STRUCT AnlTPGLevel4[4];

extern code struct CMD_STRUCT SDITSGLevel1[6];
extern code struct CMD_STRUCT SDITSGLevel2[2];

extern code struct CMD_STRUCT SDITPGLevel1[7];
extern code struct CMD_STRUCT SDITPGLevel2[1];
extern code struct CMD_STRUCT SDITPGLevel3[6];
extern code struct CMD_STRUCT SDITPGLevel4[5];
extern code struct CMD_STRUCT SDITPGLevel5[3];

extern code struct CMD_STRUCT AESEBULevel1[4];

extern code struct CMD_STRUCT TimeClockLevel1[7];

extern code struct CMD_STRUCT BBMultiLevel1[1];

extern code struct CMD_STRUCT Factory[10];
extern code struct CMD_STRUCT FactoryLevel1[2];
extern code struct CMD_STRUCT FactoryLevel2[3];
extern code struct CMD_STRUCT FactoryLevel3[3];
extern code struct CMD_STRUCT FactoryLevel4[4];
extern code struct CMD_STRUCT FactoryLevel5[1];
extern code struct CMD_STRUCT FactoryLevel6[2];
extern code struct CMD_STRUCT FactoryLevel7[1];
extern code struct CMD_STRUCT FactoryLevel8[1];
extern code struct CMD_STRUCT FactoryLevel9[1];

extern code struct PAR_STRUCT GenlockInputPar[7];
extern code struct PAR_STRUCT GenlockSystemPar[10];

extern code struct PAR_STRUCT AnlSystemPar[3];
extern code struct PAR_STRUCT SDISystemPar[2];

extern code struct PAR_STRUCT AnlPatternPar[56];
extern code struct PAR_STRUCT SDIPatternPar[67];

extern code struct PAR_STRUCT TSGAudioSignalPar[2];
extern code struct PAR_STRUCT TSGAudioLevelPar[5];

extern code struct PAR_STRUCT TPGAudioSignalPar[8];
extern code struct PAR_STRUCT TPGAudioLevelPar[9];
extern code struct PAR_STRUCT TPGAudioGroupPar[4];

extern code struct PAR_STRUCT AESEBUSignalPar[9];
extern code struct PAR_STRUCT AESEBULevelPar[9];
extern code struct PAR_STRUCT AESEBUTimingPar[6];

extern code struct PAR_STRUCT TPGTextClockPar[4];

extern code struct PAR_STRUCT TimeClockDFormatPar[3];
extern code struct PAR_STRUCT TimeClockTFormatPar[2];
extern code struct PAR_STRUCT TimeClockRefPar[4];

extern code struct PAR_STRUCT FactoryAddressPar[8];

extern code struct PAR_STRUCT OffOnPar[2];
extern code struct PAR_STRUCT MinMaxPar[2];
extern code struct PAR_STRUCT StylePar[3];
extern code struct PAR_STRUCT PT52ModePar[2];

extern code struct PAR_STRUCT LTCFormatPar[5];
extern code struct PAR_STRUCT LTCModePar[3];

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
extern void DiagnosticErrorQueueLast();
extern void DiagnosticErrorQueueEntry();
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

extern void AnlTPGSuffix();
extern void AnlTPG();
extern void AnlTPGPattern();
extern void AnlTPGAttribSuffix();
extern void AnlTPGAttribAPAL();
extern void AnlTPGAttribPLUGE();
extern void AnlTPGAttribStair10();
extern void AnlTPGAttribCircles();
extern void AnlTPGAttribCommon( UC Attribute);
extern void AnlTPGTextStringSuffix();
extern void AnlTPGTextString();
extern void AnlTPGTextStyle();
extern void AnlTPGTextPosition();
extern void AnlTPGTextClock();
extern void AnlTPGSystem();
extern void AnlTPGScHPhase();
extern void AnlTPGDelay();
extern void AnlTPGVersion();

extern void SDITSGSuffix();
extern void SDITSG();
extern void SDITSGPattern();
extern void SDITSGSystem();
extern void SDITSGEDHInsert();
extern void SDITSGDelay();
extern void SDITSGCopy();
extern void SDITSGVersion();

extern void SDITSGAudioSignal();
extern void SDITSGAudioLevel();

extern void TLG();
extern void TLGSuffix();
extern void TLGFormat();
extern void TLGDelay();
extern void TLGVersion();

extern void HD();
extern void HDSuffix();
extern void HDFormat();
extern void HDDelay();
extern void HDVersion();

extern void DL();
extern void DLSuffix();
extern void DLFormat();
extern void DLDelay();
extern void DLVersion();

extern void SDITPGSuffix();
extern void SDITPG();
extern void SDITPGPattern();
extern void SDITPGAttribSuffix();
extern void SDITPGAttribAPAL();
extern void SDITPGAttribPLUGE();
extern void SDITPGAttribStair10();
extern void SDITPGAttribMotion();
extern void SDITPGAttribCircles();
extern void SDITPGAttribLogo();
extern void SDITPGAttribCommon( UC Attribute);
extern void SDITPGTextStringSuffix();
extern void SDITPGTextString();
extern void do_8603Text();
extern void do_8603TextPos();
extern void SDITPGTextStyle();
extern void SDITPGTextMovement();
extern void SDITPGTextPosition();
extern void SDITPGTextClock();
extern void SDITPGSystem();
extern void SDITPGEDHInsert();
extern void SDITPGDelay();
extern void SDITPGVersion();

extern void SDITPGAudioSignal();
extern void SDITPGAudioLevel();
extern void SDITPGAudioGroup();

extern void AESEBUSuffix();
extern void AESEBU();
extern void AESEBUSignal();
extern void AESEBULevel();
extern void AESEBUTiming();
extern void AESEBUVersion();

extern void TimeClock();
extern void TimeClockDateFormat();
extern void TimeClockDate();
extern void TimeClockTimeFormat();
extern void TimeClockTime();
extern void TimeClockReference();
extern void TimeClockOffset();
extern void TimeClockVersion();

extern void BBMultiVersion();

extern void SDIGenlockVersion();

extern void FactoryPassword();
extern void FactoryAddress();
extern void FactoryCommand();

extern void FactorySPGUserText();
extern void FactorySPGStore();
extern void FactorySPGRead();
extern void FactoryAudioSuffix();
extern void FactoryAudioUserText();
extern void FactoryAudioRead();
extern void FactoryAudioMode();
extern void FactoryMainUserText();
extern void FactoryMainPSNSuffix();
extern void FactoryMainPSN();
extern void FactoryMainPSSuffix();
extern void FactoryMainPS();
extern void FactoryMainPSCalib();
extern void FactorySDIGenlockUserText();
extern void FactoryTimeClockUserText();
extern void FactoryTimeClockStatus();
extern void FactoryMultiBBUserText();
extern void FactoryADCRead();
extern void FactoryLDetectorRead();

extern void FindDelayParameters();
extern UC FindParameter( char* par, code struct PAR_STRUCT* TmpPtr);

extern UC WriteUnitInformation( UC HWtype, UC suffix);

extern void HDPattern();
extern void HDAudioSignal();
extern void HDAudioLevel();
extern void HDAudioClick();

extern void HDTextStringSuffix(); 
extern void HDTextString();
extern void HDTextMovement();
extern void HDTextScale();
extern void HDTextColor();
extern void HDTextBack();
extern void HDTextPosition();

extern void HDAttrCommon();
extern void do_hd_common(code struct PAR_STRUCT *pptr, UC limit);
extern UC   HDValidateAttrib(UC patt, UC att);

extern void DLPattern();
extern void DLAudioSignal();
extern void DLAudioLevel();
extern void DLAudioClick();

extern void DLTextStringSuffix(); 
extern void DLTextString();
extern void DLTextMovement();
extern void DLTextScale();
extern void DLTextColor();
extern void DLTextBack();
extern void DLTextPosition();
extern void DLAttrCommon();
extern void do_dl_common(code struct PAR_STRUCT *pptr, UC limit);
extern void DLAInterCommon();
extern UC   DLValidateInter(UC);

extern void LTCSuffix();
extern void LTCVersion();
extern void LTC();
extern void LTCOffset();
extern void LTCFormat();
extern void LTCTime();
extern void LTCTimezone();
extern void LTCDaylight();
extern void LTCDLStart();
extern void LTCDLEnd();
extern void LTCDLMode();
extern void LTCDebug();