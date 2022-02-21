/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		TEXT.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960613																		*/
/*	Rev. date:	000508, KEn, DEV															*/
/*	Status:		Version 1.4																	*/
/*																									*/
/*	This module contains NO functions.													*/
/*																									*/
/*	Changes:																						*/
/* 000508: Added  PositionTxt[]															*//* 991025: "master reset.. " text has been removed									*/
/* 990305: Added LockHdrTxt[]																*/
/* 980918: Added TOOSLOWTxt[] & TOOFASTTxt[].										*/
/* 980915: Added SYNCLOCKEDTxt[]															*/
/* 980727: Added CoModifyTxt																*/
/* 980723: Added text for TCI status													*/
/* 980709: Added LOCKEDTxt[], NOSIGNALTxt[]											*/
/* 980616: Released as 4008 002 06812													*/
/* 980611: Added AUDIO level text														*/
/* 980514: Released as 4008 002 06811													*/
/***************************************************************************/

extern code char PT5230HeadlineTxt[];

extern code char PowerUpDefaultTxt[];
extern code char PowerUpFactResetTxt[];

extern code char StatusPresetTxt[];
extern code char StatusErrorStatusTxt[];

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
extern code char ModifyHdrTxt[];
extern code char SystemHdrTxt[];
extern code char PatternHdrTxt[];
extern code char ScHPhaseHdrTxt[];
extern code char EDHHdrTxt[];
extern code char GroupHdrTxt[];
extern code char SignalHdrTxt[];
extern code char LevelHdrTxt[];
extern code char TimingHdrTxt[];
extern code char TimeVHdrTxt[];
extern code char TimeHHdrTxt[];
extern code char TimeTHdrTxt[];
extern code char UserTextHdrTxt[];
extern code char GenlockHdrTxt[];
extern code char TextHdrTxt[];
extern code char AudioHdrTxt[];
extern code char RecallHdrTxt[];
extern code char StoreHdrTxt[];
extern code char NameHdrTxt[];
extern code char LockHdrTxt[];
extern code char AutoReturnStatusHdrTxt[];
extern code char ReferenceHdrTxt[];
extern code char DownloadHdrTxt[];
extern code char BaudrateHdrTxt[];
extern code char DatabitHdrTxt[];
extern code char ParityHdrTxt[];
extern code char StatusHdrTxt[];
extern code char DateHdrTxt[];
extern code char TimeHdrTxt[];
extern code char RefHdrTxt[];
extern code char OffsetHdrTxt[];
extern code char HandshakeHdrTxt[];
extern code char ErrorNoHdrTxt[];
extern code char VersionHdrTxt[];
extern code char TestingMemoryHdrTxt[];
extern code char PressButtonHdrTxt[];

extern code char SlashDotHdrTxt[];
extern code char ThreeDotsTxt[];
extern code char degTxt[];
extern code char MenuTxt[];
extern code char SubmnuTxt[];

extern code char SYSTxt[];
extern code char EDITTxt[];
extern code char MODIFYTxt[];
extern code char SAVETxt[];
extern code char OKTxt[];
extern code char FAILTxt[];
extern code char ESCTxt[];
extern code char ROMTxt[];
extern code char RAMTxt[];

extern code char CoSelectTxt[];
extern code char CoSelectFunctionTxt[];
extern code char CoSelectInputTxt[];
extern code char CoSelectSignalTxt[];
extern code char CoEditTxt[];
extern code char CoEditDelayTxt[];
extern code char CoEditTextTxt[];
extern code char CoEditNameTxt[];
extern code char CoModifyTxt[];
extern code char CoConfigureTxt[];
extern code char CoStandardPatternTxt[];
extern code char CoComplexPatternTxt[];

extern code char HyphenTxt[];

extern code char FromKUTxt[];

extern code char AnalogTPG2Txt[];
extern code char AnalogTPG5Txt[];
extern code char AnlTPG2Txt[];
extern code char AnlTPG5Txt[];
extern code char SDITSG2Txt[];
extern code char SDITSG3Txt[];
extern code char SDITSG4Txt[];
extern code char SDITPG1Txt[];
extern code char SDITPG2Txt[];
extern code char SDITPG5Txt[];
extern code char SystemTxt[];
extern code char PatternTxt[];
extern code char ScHPhaseTxt[];
extern code char TimingTxt[];
extern code char EDHTxt[];
extern code char AudioTxt[];
extern code char TextTxt[];
extern code char StyleTxt[];
extern code char PositionTxt[];
extern code char ClockTxt[];
extern code char GroupTxt[];
extern code char SignalTxt[];
extern code char LevelTxt[];
extern code char ConfigTxt[];
extern code char OSCTxt[];
extern code char RS232Txt[];
extern code char RecallTxt[];
extern code char StoreTxt[];
extern code char NameTxt[];

extern code char AESEBUBNC1Txt[];
extern code char AESEBUBNC2Txt[];
extern code char AESEBUXLR1Txt[];
extern code char AESEBUXLR2Txt[];
extern code char AESEBUWord1Txt[];
extern code char AESEBUWord2Txt[];

extern code char BlkBurstTxt[];
extern code char AESEBUTxt[];
extern code char GenlockTxt[];
extern code char PresetTxt[];
extern code char TestTxt[];

extern code char EMBAudioTxt[];

extern code char ConfigDateTimeTxt[];
extern code char ConfigLockTxt[];
extern code char ConfigAutoESCTxt[];
extern code char ConfigLCDContTxt[];
extern code char ConfigDownloadTxt[];
extern code char ConfigDiagnoseTxt[];

extern code char TimeTxt[];
extern code char DateTxt[];
extern code char TimeDateTxt[];
extern code char ReferenceTxt[];
extern code char OffsetTxt[];

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
extern code char LCDADConvTxt[];
extern code char TEMPADConvTxt[];
extern code char VoltageTxt[];
extern code char LevelDetTxt[];
extern code char UnknownOptionTxt[];

extern code char GENLOCKEDTxt[];
extern code char SYNCLOCKEDTxt[];
extern code char UNLOCKEDTxt[];
extern code char NOGENLOCKTxt[];
extern code char NOSIGNALTxt[];
extern code char PRESENTTxt[];
extern code char TOOSLOWTxt[];
extern code char TOOFASTTxt[];
extern code char NOVITCTxt[];
extern code char VITCPALTxt[];
extern code char VITCNTSCTxt[];
extern code char NOLTCTxt[];
extern code char LTCPALTxt[];
extern code char LTCNTSCTxt[];

extern code char OnTxt[];
extern code char OffTxt[];

extern code char UPButtonTxt[];
extern code char DOWNButtonTxt[];
extern code char LEFTButtonTxt[];
extern code char RIGHTButtonTxt[];
extern code char EXECUTEButtonTxt[];
extern code char CBARButtonTxt[];
extern code char MBURSTButtonTxt[];
extern code char WINDOWButtonTxt[];
extern code char PRESETButtonTxt[];
extern code char SPECIALButtonTxt[];
extern code char LINEARITYButtonTxt[];
extern code char PATTERNButtonTxt[];
extern code char OUTPUTButtonTxt[];

extern code char BB1Txt[];
extern code char BB2Txt[];
extern code char BB3Txt[];
extern code char BB4Txt[];
extern code char BB5Txt[];
extern code char BB6Txt[];
extern code char BB7Txt[];
extern code char BB8Txt[];
extern code char BBMulTxt[];

extern code char AESEBU1Txt[];
extern code char AESEBU2Txt[];

extern code char DownloadTxt[];
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

extern code char MainConnTxt[];
extern code char OSCConnTxt[];
extern code char BBConnTxt[];
extern code char ATPGConnTxt[];
extern code char STSGConnTxt[];
extern code char STPGConnTxt[];
extern code char SDIGenlockConnTxt[];
extern code char Int2GenlockConnTxt[];
extern code char AESEBUConnTxt[];
extern code char TimeConnTxt[];

extern code char* code AnlTPGHdrArrTxt[];
extern code char* code SDITSGHdrArrTxt[];
extern code char* code SDITPGHdrArrTxt[];
extern code char* code AESEBUHdrArrTxt[];

extern code char* code ButtonArrTxt[];

extern code char* code BBArrTxt[];
extern code char* code AnlTPGArrTxt[];
extern code char* code SDITSGArrTxt[];
extern code char* code SDITPGArrTxt[];

extern code char* code AnlSystemArrTxt[];
extern code char* code SDISystemArrTxt[];

extern code char* code AnlPattArrTxt[];
extern code char* code SDIPattArrTxt[];
extern code char* code AttribArrTxt[];

extern code char* code TSGAudioSignalArrTxt[];
extern code char* code TSGAudioLevelArrTxt[];

extern code char* code TPGAudioSignalArrTxt[];
extern code char* code TPGAudioLevelArrTxt[];
extern code char* code TPGAudioGroupArrTxt[];

extern code char* code TPGTextEditArrTxt[];
extern code char* code TPGTextStyleArrTxt[];
extern code char* code TPGTextClockArrTxt[];

extern code char* code AESEBUSignalArrTxt[];
extern code char* code AESEBULevelArrTxt[];
extern code char* code AESEBUTimingArrTxt[];

extern code char* code GenlockInputArrTxt[];
extern code char* code GenlockSignalArrTxt[];

extern code char* code LockArrTxt[];

extern code char* code OffOnArrTxt[];
extern code char* code MinusPlusArrTxt[];

extern code char* code TimeFormatArrTxt[];
extern code char* code DateFormatArrTxt[];
extern code char* code DateTimeRefArrTxt[];

extern code char* code DownloadArrTxt[];

extern code char* code RS232BaudrateArrTxt[];
extern code char* code RS232ParityArrTxt[];
extern code char* code RS232HandshakeArrTxt[];
