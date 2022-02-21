/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		TEXT.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960613																		*/
/*	Rev. date:	991110, KEn	DEV															*/
/*	Status:		Version 2.3																	*/
/*																									*/
/*	This module contains NO functions.													*/
/*																									*/
/*	Changes:																						*/
/* 991110: Select_Level_Txt removed														*/
/* 991007: Added support for two AES-EBU generators, (PT8635)					*/
/* 990614: Added LockHdrTxt[]																*/
/* 990613: Added TextOffOnParTxt															*/
/* 980915: Added SYNCLOCKEDTxt[]															*/
/* 980616: Added AudioLevelArrTxt														*/
/* 980616: Added AUDIO level																*/
/* 971212: DataNotCompatibleTxt[]														*/
/* 971027: Added TestingBlackburstTxt													*/
/* 971016: ResetErrorQueueTxt																*/
/* 971014: Added TestingOptionsCompletedTxt & LoopbackConnectorTxt			*/
/*			  Added RS232TestTxt																*/
/* 971002: Removed ConfigurationArrTxt. Added ConnectorArrTxt					*/
/* 970930: Added several texts for diagnose-menus									*/
/* 970821: Several new texts added														*/
/* 970205: Several new texts added														*/
/* 970201: Added Diagnose texts															*/
/* 970127: Added VoltageTxt																*/
/* 970110: Added definitions for error code text									*/
/* 961222: Added declarations for new signal text									*/
/***************************************************************************/

extern code char PT5210HeadlineTxt[];

extern code char PowerUp_DefaultTxt[];
extern code char PowerUp_FactResetTxt[];
extern code char Status_PresetTxt[];
extern code char Status_ErrorStatusTxt[];

extern code char SelfTestTxt[];
extern code char InternalTestTxt[];

extern code char PressExecuteToStartTxt[];
extern code char WarningAboutToAlterTxt[];
extern code char WarningDownloadInTxt[];

extern code char ParallelRemoteOperatingTxt[];
extern code char SerialRemoteOperatingTxt[];
extern code char LocalLockoutTxt[];

extern code char CheckCableTxt[];
extern code char ChecksumErrorReceivingTxt[];
extern code char DataNotCompatibleTxt[];

extern code char DiagnoseDisplayLN1Txt[];
extern code char DiagnoseDisplayLN2Txt[];

extern code char NoPresetActiveTxt[];

extern code char PresetHdrTxt[];
extern code char MenuHdrTxt[];
extern code char SubmnuHdrTxt[];
extern code char SecondmnuHdrTxt[];
extern code char SelectHdrTxt[];
extern code char AnalogSignalHdrTxt[];
extern code char SDISignalHdrTxt[];
extern code char SystemHdrTxt[];
extern code char PatternHdrTxt[];
extern code char ScHPhaseHdrTxt[];
extern code char SBEDHHdrTxt[];
extern code char SignalHdrTxt[];
extern code char LevelHdrTxt[];
extern code char TimingHdrTxt[];
extern code char SBAudioHdrTxt[];
extern code char BlackCopyHdrTxt[];
extern code char TimeVHdrTxt[];
extern code char TimeHHdrTxt[];
extern code char TimeTHdrTxt[];
extern code char UserTextHdrTxt[];
extern code char GenlockHdrTxt[];
extern code char AES_EBU1HdrTxt[];
extern code char AES_EBU2HdrTxt[];
extern code char AES_EBUHdrTxt[];
extern code char InpHdrTxt[];
extern code char TextHdrTxt[];
extern code char RecallHdrTxt[];
extern code char StoreHdrTxt[];
extern code char NameHdrTxt[];
extern code char LockHdrTxt[];
extern code char AutoReturnStatusHdrTxt[];
extern code char DownloadHdrTxt[];
extern code char BaudrateHdrTxt[];
extern code char DatabitHdrTxt[];
extern code char ParityHdrTxt[];
extern code char StatusHdrTxt[];
extern code char HandshakeHdrTxt[];
extern code char ErrorNoHdrTxt[];
extern code char VersionHdrTxt[];
extern code char TestingMemoryHdrTxt[];
extern code char PressButtonHdrTxt[];

extern code char ThreeDotsTxt[];
extern code char SlashDotHdrTxt[];
extern code char degTxt[];

extern code char MenuTxt[];
extern code char SubmnuTxt[];
extern code char SelectTxt[];

extern code char SYSTxt[];
extern code char EDITTxt[];
extern code char SAVETxt[];
extern code char FAILTxt[];
extern code char OKTxt[];
extern code char ESCTxt[];
extern code char ROMTxt[];
extern code char RAMTxt[];

extern code char Select_Txt[];
extern code char Select_FunctionTxt[];
extern code char Select_InputTxt[];
extern code char Select_SignalTxt[];
extern code char Edit_Txt[];
extern code char Edit_DelayTxt[];
extern code char Edit_TextTxt[];
extern code char Edit_NameTxt[];
extern code char Configure_Txt[];
extern code char Configure_OutputTxt[];

extern code char HyphenTxt[];

extern code char FromKUTxt[];

extern code char SystemTxt[];
extern code char TimeTxt[];
extern code char ScHPhaseTxt[];
extern code char CopyTxt[];
extern code char EDHTxt[];
extern code char PatternTxt[];
extern code char TextTxt[];
extern code char ConfigTxt[];
extern code char OSCTxt[];
extern code char PresetTxt[];
extern code char RS232Txt[];
extern code char TimingTxt[];
extern code char LevelTxt[];
extern code char SignalTxt[];
extern code char ModeTxt[];
extern code char RecallTxt[];
extern code char StoreTxt[];
extern code char NameTxt[];

extern code char AESEBUBNC1Txt[];
extern code char AESEBUBNC2Txt[];
extern code char AESEBUXLR1Txt[];
extern code char AESEBUXLR2Txt[];
extern code char AESEBUWord1Txt[];
extern code char AESEBUWord2Txt[];

extern code char ANLBlackTxt[];
extern code char SDIBlackTxt[];
extern code char AnalogSignalTxt[];
extern code char SDISignalTxt[];
extern code char AES_EBUTxt[];
extern code char GenlockTxt[];
extern code char TestTxt[];

extern code char UPButtonTxt[];
extern code char DOWNButtonTxt[];
extern code char LEFTButtonTxt[];
extern code char RIGHTButtonTxt[];
extern code char EXECUTEButtonTxt[];
extern code char PRESETButtonTxt[];
extern code char OUTPUTButtonTxt[];
extern code char GENLOCKButtonTxt[];

extern code char BB1Txt[];
extern code char BB2Txt[];
extern code char BB3Txt[];
extern code char BB4Txt[];
extern code char BB5Txt[];
extern code char BB6Txt[];
extern code char BB7Txt[];
extern code char BB8Txt[];
extern code char BBMulTxt[];
extern code char SB34Txt[];
extern code char SB56Txt[];
extern code char SB78Txt[];

extern code char AES_EBU1Txt[];
extern code char AES_EBU2Txt[];

extern code char SBAudioTxt[];

extern code char SDIAudioSignalTxt[];
extern code char SDIAudioLevelTxt[];

extern code char ConfigLockTxt[];
extern code char ConfigAutoESCTxt[];
extern code char ConfigLCDContTxt[];
extern code char ConfigDownloadTxt[];
extern code char ConfigDiagnoseTxt[];

extern code char RS232BaudrateTxt[];
extern code char RS232DatabitTxt[];
extern code char RS232ParityTxt[];
extern code char RS232HandshakeTxt[];

extern code char MainTxt[];
extern code char OptionsTxt[];
extern code char MemoryTxt[];
extern code char DisplayTxt[];
extern code char KeyboardSLTxt[];
extern code char VersionTxt[];
extern code char ConfigurationTxt[];
extern code char ErrorQueueTxt[];


extern code char GeneralFailureTxt[];
extern code char NoContactToTxt[];
extern code char ErrorWritingToTxt[];
extern code char NoResponseFromTxt[];
extern code char ErrorReadingFromTxt[];
extern code char ErrorDownloadingFromTxt[];
extern code char ChecksumErrorTxt[];

extern code char MultipleFailureTxt[];
extern code char PowerFailureTxt[];
extern code char TemperatureFailureTxt[];

extern code char NoWarningsTxt[];
extern code char NoErrorDetectedTxt[];
extern code char TemperatureWarningTxt[];
extern code char LevelDetectorTxt[];
extern code char ConfigurationErrorTxt[];
extern code char MultipleErrorTxt[];

extern code char TestingTxt[];
extern code char TestingMainboardTxt[];
extern code char TestingBlackburstTxt[];
extern code char TestingSPGBoardTxt[];
extern code char TestingRS232CommTxt[];
extern code char TestingMainCompletedTxt[];
extern code char TestingOptionsCompletedTxt[];
extern code char TestingMemoryCompletedTxt[];
extern code char TestingButtonsCompletedTxt[];

extern code char NoOptionsDetectedTxt[];
extern code char LoopbackConnectorTxt[];

extern code char ResetErrorQueueTxt[];

extern code char RS232TestTxt[];

extern code char VoltageN5Txt[];
extern code char Voltage12Txt[];

extern code char SDIGenlockTxt[];
extern code char TimeCodeTxt[];
extern code char ROMMemoryTxt[];
extern code char RAMMemoryTxt[];
extern code char KeyboardTxt[];
extern code char LCD_ADConvTxt[];
extern code char TEMP_ADConvTxt[];
extern code char VoltageTxt[];
extern code char LevelDetTxt[];
extern code char UnknownOptionTxt[];

extern code char GENLOCKEDTxt[];
extern code char SYNCLOCKEDTxt[];
extern code char UNLOCKEDTxt[];

extern code char OnTxt[];
extern code char OffTxt[];

extern code char NTSCTxt[];
extern code char PALTxt[];
extern code char D525Txt[];
extern code char D625Txt[];

extern code char SMPTECBarTxt[];
extern code char EBUCBarTxt[];
extern code char SplitFieldCBarTxt[];
extern code char Window15Txt[];
extern code char Window20Txt[];
extern code char Window100Txt[];
extern code char CrossHatchTxt[];
extern code char PlugeTxt[];
extern code char SafeAreaTxt[];
extern code char ShallowRampTxt[];
extern code char MultiBurstTxt[];
extern code char RedTxt[];
extern code char BlackBurstTxt[];

extern code char SDICheckFieldTxt[];
extern code char DigitalTimingTxt[];

extern code char Stereo800HzTxt[];
extern code char Stereo1kHzTxt[];
extern code char StereoEBU1kHzTxt[];
extern code char StereoBBC1kHzTxt[];
extern code char MonoEBU1kHzTxt[];
extern code char MonoBBC1kHzTxt[];
extern code char DualTxt[];

extern code char SilenceTxt[];
extern code char Level0dBTxt[];
extern code char Level10dBTxt[];
extern code char Level20dBTxt[];

extern code char NTSCPhase1Txt[];
extern code char NTSCPhase2Txt[];
extern code char NTSCPhase3Txt[];
extern code char NTSCPhase4Txt[];
extern code char NTSCPhase5Txt[];

extern code char GenlockATxt[];
extern code char GenlockBTxt[];
extern code char LoopThroughTxt[];
extern code char SDIGenlockTxt[];
extern code char InternalGenlockTxt[];

extern code char PALBurstTxt[];
extern code char NTSCBurstTxt[];
extern code char Sync625Txt[];
extern code char Sync525Txt[];
extern code char MHz443Txt[];
extern code char MHz358Txt[];
extern code char MHz5Txt[];
extern code char MHz10Txt[];

extern code char NormalTxt[];
extern code char PanelTxt[];
extern code char DownloadTxt[];
extern code char DiagnoseTxt[];

extern code char N300Txt[];
extern code char N600Txt[];
extern code char N1200Txt[];
extern code char N2400Txt[];
extern code char N4800Txt[];
extern code char N9600Txt[];

extern code char NoneTxt[];
extern code char OddTxt[];
extern code char EvenTxt[];

extern code char XONXOFFTxt[];
extern code char RTSCTSTxt[];

extern code char* code AES_EBUHdrArrTxt[];
extern code char* code ButtonArrTxt[];
extern code char* code BBArrTxt[];
extern code char* code SBArrTxt[];
extern code char* code SBPatternArrTxt[];
extern code char* code AnaTVSystemArrTxt[];
extern code char* code SDITVSystemArrTxt[];
extern code char* code AnalogPatternArrTxt[];
extern code char* code SDIPatternArrTxt[];
extern code char* code EMBAudioArrTxt[];
extern code char* code AudioSignalArrTxt[];
extern code char* code AudioLevelArrTxt[];
extern code char* code AES_EBUSignalArrTxt[];
extern code char* code AES_EBULevelArrTxt[];
extern code char* code AES_EBUTimingArrTxt[];
extern code char* code GenlockInputArrTxt[];
extern code char* code GenlockSignalArrTxt[];
extern code char* code LockArrTxt[];
extern code char* code OffOnArrTxt[];
extern code char* code TextOffOnArrTxt[];
extern code char* code DownloadArrTxt[];
extern code char* code RS232BaudrateArrTxt[];
extern code char* code RS232ParityArrTxt[];
extern code char* code RS232HandshakeArrTxt[];
extern code char* code ConnectorArrTxt[];
