/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:    TEXT.H                                                      */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  960613                                                     */
/*  Rev. date:  000508, KEn, DEV                                           */
/*  Status:    Version 1.4                                                 */
/*                                                                         */
/*  This module contains NO functions.                                     */
/*                                                                         */
/*  Changes:                                                               */
/* 000508: Added  PositionTxt[]                                            */
/* 991025: "master reset.. " text has been removed                         */
/* 990305: Added LockHdrTxt[]                                              */
/* 980918: Added TOOSLOWTxt[] & TOOFASTTxt[].                              */
/* 980915: Added SYNCLOCKEDTxt[]                                           */
/* 980727: Added CoModifyTxt                                               */
/* 980723: Added text for TCI status                                       */
/* 980709: Added LOCKEDTxt[], NOSIGNALTxt[]                                */
/* 980616: Released as 4008 002 06812                                      */
/* 980611: Added AUDIO level text                                          */
/* 980514: Released as 4008 002 06811                                      */
/***************************************************************************/

extern code char LevelsTxt[];
extern code char DLTPGTxt[];
extern code char DLTPG1Txt[];
extern code char DLTPG2Txt[];
extern code char DLTPG3Txt[];
extern code char DLTPG4Txt[];
extern code char DLTPG5Txt[];
extern code char DLTPG6Txt[];

extern code char InterfaceTxt[];
extern code char* DLArrTxt[];

/************************************************/

extern code char HDDownloadTxt[];

extern code char ColorTxt[];
extern code char BackgroundTxt[];
extern code char MovementTxt[];

extern code char TRI1Txt[];
extern code char TRI2Txt[];
extern code char TRI3Txt[];

extern code char HD1Txt[];
extern code char HD2Txt[];
extern code char HD3Txt[];

extern code char DL1Txt[];
extern code char DL2Txt[];
extern code char DL3Txt[];

extern code char HDTPGTxt[];
extern code char HDTPG1Txt[];
extern code char HDTPG2Txt[];
extern code char HDTPG3Txt[];
extern code char HDTPG4Txt[];
extern code char HDTPG5Txt[];
extern code char HDTPG6Txt[];
extern code char HDTPG7Txt[];
extern code char HDTPG8Txt[];
extern code char HDTPG9Txt[];
extern code char HDTPG10Txt[];
extern code char HDTPG11Txt[];
extern code char HDTPG12Txt[];



extern code char TLGTxt[];
extern code char TLG1Txt[];
extern code char TLG2Txt[];
extern code char TLG3Txt[];
extern code char TLG4Txt[];
extern code char TLG5Txt[];
extern code char TLG6Txt[];
extern code char TLG7Txt[];
extern code char TLG8Txt[];
extern code char TLG9Txt[];
extern code char TLG10Txt[];
extern code char TLG11Txt[];
extern code char TLG12Txt[];
extern code char FormatTxt[];


extern code char F1Txt[];
extern code char F2Txt[];
extern code char F3Txt[];
extern code char F4Txt[];
extern code char F5Txt[];
extern code char F6Txt[];
extern code char F7Txt[];
extern code char F8Txt[];
extern code char F9Txt[];
extern code char F10Txt[];
extern code char F11Txt[];
extern code char F14Txt[];
extern code char F15Txt[];
extern code char F16Txt[];
extern code char F17Txt[];
extern code char F18Txt[];
extern code char F19Txt[];
extern code char F20Txt[];
extern code char F21Txt[];


extern code char L1Txt[];
extern code char L2Txt[];
extern code char L3Txt[];
extern code char L4Txt[];
extern code char L5Txt[];
extern code char L6Txt[];
extern code char L7Txt[];
extern code char L8Txt[];
extern code char L9Txt[];
extern code char L10Txt[];
extern code char L11Txt[];
extern code char L12Txt[];
extern code char L13Txt[];
extern code char L14Txt[];
extern code char L15Txt[];
extern code char L16Txt[];
extern code char L17Txt[];
extern code char L18Txt[];
extern code char L19Txt[];
extern code char L20Txt[];
extern code char L21Txt[];
extern code char L22Txt[];
extern code char L23Txt[];
extern code char L24Txt[];
extern code char L25Txt[];

extern code char  M0Txt[];     
extern code char  M4Txt[];   
extern code char  M5Txt[];                                
extern code char  M6Txt[];                                
extern code char  M7Txt[];                                
extern code char  M8Txt[];                                
extern code char  M9Txt[];                                
extern code char  M10Txt[];                            
extern code char  M11Txt[];
extern code char  M14Txt[];                            
extern code char  M15Txt[];                            
extern code char  M16Txt[];                            
extern code char  M17Txt[];                            
extern code char  M18Txt[];                            
extern code char  M19Txt[];                            
extern code char  M20Txt[];                            
extern code char  M21Txt[];                            
extern code char  M22Txt[];
extern code char  M23Txt[];                            

extern code char  N0Txt[];     
extern code char  N4Txt[];   
extern code char  N5Txt[];                        
extern code char  N6Txt[];                         
extern code char  N7Txt[];                         
extern code char  N8Txt[];                        
extern code char  N9Txt[];                         
extern code char  N10Txt[];                             
extern code char  N11Txt[];  

extern code char  N30Txt[];          
extern code char  N31Txt[];         
extern code char  N32Txt[];          
extern code char  N33Txt[];          
extern code char  N34Txt[];  

extern code char  N14Txt[];                             
extern code char  N15Txt[];                            
extern code char  N16Txt[];                             
extern code char  N17Txt[];                             
extern code char  N18Txt[];                            
extern code char  N19Txt[];                             
extern code char  N20Txt[];                             
extern code char  N21Txt[];                            
extern code char  N22Txt[];  
extern code char  N23Txt[];  

/************************************************/

/*******************  HDTPG  *****************************/
extern code char code ScaleTxt[];

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
extern code char ThirdmnuHdrTxt[];
extern code char ForthmnuHdrTxt[];
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
extern code char TimeFHdrTxt[];
extern code char TimeHHdrTxt[];
extern code char TimeTHdrTxt[];
extern code char UserTextHdrTxt[];
extern code char GenlockHdrTxt[];
extern code char LTCHdrTxt[];
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
extern code char mSTxt[];
extern code char MenuTxt[];
extern code char SubmnuTxt[];

extern code char SYSTxt[];
extern code char EDITTxt[];
extern code char MODIFYTxt[];
extern code char INTERFACETxt[];
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
extern code char ClickTxt[];
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
extern code char ETHConfigTxt[];
extern code char OSCTxt[];
extern code char RS232Txt[];
extern code char RecallTxt[];
extern code char StoreTxt[];
extern code char NameTxt[];
extern code char LTCModeTxt[];
extern code char LTCTimezoneTxt[];
extern code char LTCDaylightTxt[];
extern code char LTCDateTxt[];
extern code char LTCSyncTxt[];
extern code char LTCFormatTxt[];
extern code char LTCStartTxt[];
extern code char LTCEndTxt[];
extern code char LTCHourTxt[];
extern code char ConfirmTxt[];
extern code char LTCNSTxt[];
extern code char LTCDSTTxt[];

extern code char AESEBUBNC1Txt[];
extern code char AESEBUBNC2Txt[];
extern code char AESEBUXLR1Txt[];
extern code char AESEBUXLR2Txt[];
extern code char AESEBUWord1Txt[];
extern code char AESEBUWord2Txt[];

extern code char BlkBurstTxt[];
extern code char AESEBUTxt[];
extern code char GenlockTxt[];
extern code char LTCTxt[];
extern code char NetworkTxt[];
extern code char PresetTxt[];
extern code char TestTxt[];

extern code char EMBAudioTxt[];

extern code char ConfigDateTimeTxt[];
extern code char ConfigLockTxt[];
extern code char ConfigAutoESCTxt[];
extern code char ConfigLCDContTxt[];
extern code char ConfigDownloadTxt[];
extern code char ConfigDiagnoseTxt[];
extern code char ConfigUpdateTxt[];

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
extern code char NoGPSLockTxt[];
extern code char LTCDaylightSwitchedTxt[];
extern code char LTCResyncedTxt[];

extern code char MultipleFailureTxt[];
extern code char PowerFailureTxt[];
extern code char TemperatureFailureTxt[];

extern code char NoWarningsTxt[];
extern code char NoErrorDetectedTxt[];
extern code char TemperatureWarningTxt[];
extern code char LevelDetectorTxt[];
extern code char InternalErrorTxt[];
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
extern code char GPSGenlockTxt[];
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
extern code char GPSGenlockConnTxt[];
extern code char ETHConnTxt[];
extern code char LTCConnTxt[];
extern code char Int2GenlockConnTxt[];
extern code char AESEBUConnTxt[];
extern code char TimeConnTxt[];
extern code char TLGConnTxt1[];
extern code char TLGConnTxt2[];
extern code char TLGConnTxt3[];

extern code char HDTPGConnTxt1[];
extern code char HDTPGConnTxt2[];
extern code char HDTPGConnTxt3[];

extern code char DLTPGConnTxt1[];
extern code char DLTPGConnTxt2[];
extern code char DLTPGConnTxt3[];

extern code char DHCPTxt[];
extern code char IPAddrTxt[];
extern code char SUBNETTxt[];
extern code char GATEWAYTxt[];
extern code char DNSTxt[];
extern code char MACTxt[];
extern code char NetfinderTxt[];
extern code char TelnetTxt[];
extern code char ResetPasswordTxt[];
extern code char PortTxt[];


/********************** ArrTxt ***********************/

extern code char* TLGFormatArrTxt[]; 
extern code char* HDTPGSystemArrTxt[]; /* HDTPG */
extern code char* DLTPGSystemArrTxt[];

extern code char* HDTPGAChttribArrTxt[];
extern code char* HDTPGAWindowttribArrTxt[];
extern code char* HDTPGACBttribArrTxt[];

extern code char* AnlTPGHdrArrTxt[];
extern code char* SDITSGHdrArrTxt[];
extern code char* SDITPGHdrArrTxt[];
extern code char* AESEBUHdrArrTxt[];

extern code char* ButtonArrTxt[];

extern code char* BBArrTxt[];
extern code char* AnlTPGArrTxt[];
extern code char* SDITSGArrTxt[];
extern code char* SDITPGArrTxt[];

extern code char* AnlSystemArrTxt[];
extern code char* SDISystemArrTxt[];

extern code char* AnlPattArrTxt[];
extern code char* SDIPattArrTxt[];
extern code char* AttribArrTxt[];

extern code char* TSGAudioSignalArrTxt[];
extern code char* TSGAudioLevelArrTxt[];

extern code char* TPGAudioSignalArrTxt[];
extern code char* TPGAudioLevelArrTxt[];
extern code char* TPGAudioGroupArrTxt[];

extern code char* TPGTextEditArrTxt[];
extern code char* TPGTextStyleArrTxt[];
extern code char* TPGTextClockArrTxt[];

extern code char* AESEBUSignalArrTxt[];
extern code char* AESEBULevelArrTxt[];
extern code char* AESEBUTimingArrTxt[];

extern code char* GenlockInputArrTxt[];
extern code char* GenlockSignalArrTxt[];

extern code char* LTCArrTxt[];
extern code char* LTCModeArrTxt[];
extern code char* LTCFormatArrTxt[];

extern code char* LockArrTxt[];

extern code char* OffOnArrTxt[];
extern code char* MinusPlusArrTxt[];

extern code char* TimeFormatArrTxt[];
extern code char* DateFormatArrTxt[];
extern code char* DateTimeRefArrTxt[];

extern code char* DownloadArrTxt[];

extern code char* RS232BaudrateArrTxt[];
extern code char* RS232ParityArrTxt[];
extern code char* RS232HandshakeArrTxt[];

extern code char* HDArrTxt[];
extern code char* TRIArrTxt[];  /* JK addition */
extern code char* HDTextColorArrTxt[];
extern code char* HDTextMovArrTxt[];
extern code char* HDTextScaleArrTxt[];
extern code char* HDTPGPattArrTxt[];

extern code char* HDAudioLevelArrTxt[];
extern code char* HDAudioSignalArrTxt[];

extern code char *DLTPGSystInterTxt[];


extern code char* code PT8603TextEditArrTxt[];

extern code char OnOffTxt[];

extern code char Spaces6Txt[];

extern code char GPSTxt[];


extern code char *TimeRefArrTxt[];
extern code char AutoTxt[];

extern code char NTSCTxt[];
extern code char PALTxt[];
extern code char PT8620Txt[];

