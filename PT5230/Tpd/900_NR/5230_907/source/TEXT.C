/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1998						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		TEXT.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980118																		*/
/*	Rev. date:	991025, KEn, DEV															*/
/*	Status:		Version 1.4																	*/
/*																									*/
/*	This module contains NO functions.													*/
/*																									*/
/*	Changes:																						*/
/* 000508: Added  PositionTxt[]															*/
/* 991025: "master reset.. " text has been removed									*/
/* 990420: Added Window 10% & 15 kHz Bl/Wh in both SDI & Analog.				*/
/* 990305: Added LockHdrTxt[]																*/
/* 980918: Added TOOSLOWTxt[] & TOOFASTTxt[].										*/
/* 980917: Added new sdi pattern texts. Renamed text in MonoBBC1kHzTxt		*/
/* 980916: Added new analog pattern texts.											*/
/* 980915: Added SYNCLOCKEDTxt[]															*/
/* 980727: Added CoModifyTxt																*/
/* 980723: Added text for TCI status													*/
/* 980709: Added LOCKEDTxt[], NOSIGNALTxt[]											*/
/* 980702: Updated DateTimeRefArrTxt[]													*/ 
/* 980616: Released as 4008 002 06812													*/
/* 980611: Added AUDIO level text														*/
/* 980616: Released as 4008 002 06811													*/
/* 022213: Modified for TDC moving logo                                    */
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include "text.h"

code char PT5230HeadlineTxt[] 			= "PT 5230 Dig.Vid Gen.";

code char PowerUpDefaultTxt[] 			= "PT 5230 Dig.Vid Gen. Power-up diagnose  ";
code char PowerUpFactResetTxt[] 			= "PT 5230 Dig.Vid Gen. Factory-reset ...  ";

code char StatusPresetTxt[]				= "PT 5230 Preset Status:";
code char StatusErrorStatusTxt[]			= "PT 5230 Error/Warning Status:";

code char SelfTestTxt[]						= "Selftest in progress ...";
code char InternalTestTxt[]				= "Internal test passed ...";

code char PressExecuteToStartTxt[]		= "Press EXECUTE to start download!";
code char WarningAboutToAlterTxt[]		= "WARNING! About to alter data";
code char WarningDownloadInTxt[]			= "WARNING! Download in progress";

code char ParallelRemoteOperatingTxt[]	= "Parallel remote operating.";
code char SerialRemoteOperatingTxt[]	= "Serial remote operating.";
code char LocalLockoutTxt[]				= "Local lockout.";

code char CheckCableTxt[]					= "Check cable & RS232 settings";
code char ChecksumErrorReceivingTxt[]	= "Checksum error receiving data";
code char DataNotCompatibleTxt[]			= "Data are not compatible";

code char DiagnoseDisplayLN1Txt[] 		= "ABCEDFGHIJKLMNOPQRSTUVWXYZ_!\x22#$%&/()=?;:";
code char DiagnoseDisplayLN2Txt[] 		= "abcdefghijklmnopqrstuvwxyz-1234567890+,.";

code char NoPresetActiveTxt[]				= "No preset active";

code char PresetHdrTxt[]					= "PRESET : ";
code char MenuHdrTxt[]						= "MENU  : ";
code char SubmnuHdrTxt[]					= "SUBMNU: ";
code char SecondmnuHdrTxt[]				= "2NDMNU: ";
code char SelectHdrTxt[]					= "SELECT: ";
code char ModifyHdrTxt[]					= "MODIFY: ";
code char AnlTPG2HdrTxt[]					= "ANALOG-TPG2: ";
code char AnlTPG5HdrTxt[]					= "ANALOG-TPG5: ";
code char SDITSG2HdrTxt[]					= "SDI-TSG2: ";
code char SDITSG3HdrTxt[]					= "SDI-TSG3: ";
code char SDITSG4HdrTxt[]					= "SDI-TSG4: ";
code char SDITPG1HdrTxt[]					= "SDI-TPG1: ";
code char SDITPG2HdrTxt[]					= "SDI-TPG2: ";
code char SDITPG5HdrTxt[]					= "SDI-TPG5: ";

code char AESEBU1HdrTxt[]					= "AES-EBU1: ";
code char AESEBU2HdrTxt[]					= "AES-EBU2: ";

code char SystemHdrTxt[]					= "SYSTEM: ";
code char PatternHdrTxt[]					= "PATTERN: ";
code char ScHPhaseHdrTxt[]					= "ScH-PHASE: ";
code char EDHHdrTxt[]						= "EDH-INSERTION: ";
code char GroupHdrTxt[]						= "GROUP: ";
code char SignalHdrTxt[]					= "SIGNAL: ";
code char LevelHdrTxt[]						= "LEVEL: ";
code char TimingHdrTxt[]					= "TIMING: ";
code char TimeVHdrTxt[]						= "V: ";
code char TimeHHdrTxt[]						= "  H: ";
code char TimeTHdrTxt[]						= "  T: ";
code char UserTextHdrTxt[]					= "USER TEXT: ";
code char AudioHdrTxt[]						= "AUDIO: ";
code char GenlockHdrTxt[]					= "GENLOCK: ";
code char TextHdrTxt[]						= "TEXT: ";
code char RecallHdrTxt[]					= "RECALL : ";
code char StoreHdrTxt[]						= "STORE : ";
code char NameHdrTxt[]						= "NAME : ";
code char LockHdrTxt[]						= "LOCK: ";
code char AutoReturnStatusHdrTxt[]		= "AUTO RETURN TO STATUS: ";
code char ReferenceHdrTxt[]				= "REFERENCE: ";
code char DownloadHdrTxt[]					= "DOWNLOAD: ";
code char BaudrateHdrTxt[]					= "BAUD-RATE: ";
code char DatabitHdrTxt[]					= "DATA-BIT: ";
code char ParityHdrTxt[]					= "PARITY: ";
code char HandshakeHdrTxt[]				= "HANDSHAKE: ";
code char StatusHdrTxt[]					= "STATUS: ";
code char DateHdrTxt[]						= "DATE: ";
code char TimeHdrTxt[]						= "TIME: ";
code char RefHdrTxt[]						= "REF: ";
code char OffsetHdrTxt[]					= "OFFSET: ";
code char ErrorNoHdrTxt[]					= "E(   ): ";
code char VersionHdrTxt[]					= "Version: ";
code char TestingMemoryHdrTxt[]		   = "Testing memory: ";
code char PressButtonHdrTxt[]				= "Press button: ";

code char SlashDotHdrTxt[]					= "../";
code char ThreeDotsTxt[]					= "...";
code char degTxt[]							= " deg";
code char MenuTxt[]							= "menu";
code char SubmnuTxt[]						= "submenu";

code char SYSTxt[] 							= "SYS";
code char EDITTxt[] 							= "EDIT";
code char MODIFYTxt[]						= "MODIFY";
code char SAVETxt[] 							= "SAVE";
code char OKTxt[]								= "OK";
code char FAILTxt[]							= "FAIL";
code char ESCTxt[] 							= "ESC";
code char ROMTxt[]							= "ROM";
code char RAMTxt[]							= "RAM";

code char CoSelectTxt[]						= ", select";
code char CoSelectFunctionTxt[]			= ", select function";
code char CoSelectInputTxt[]				= ", select input";
code char CoSelectSignalTxt[]				= ", select signal";
code char CoEditTxt[]						= ", edit";
code char CoEditDelayTxt[]					= ", edit delay";
code char CoEditTextTxt[]					= ", edit text";
code char CoEditNameTxt[]					= ", edit name";
code char CoModifyTxt[]						= ", modify";
code char CoConfigureTxt[]					= ", configure";
code char CoStandardPatternTxt[]			= ", standard pattern";
code char CoComplexPatternTxt[]			= ", complex pattern";

code char HyphenTxt[]						= "------";

code char FromKUTxt[]						= " from KU";

code char AnalogTPG2Txt[]					= "ANALOG-TPG2";
code char AnalogTPG5Txt[]					= "ANALOG-TPG5";
code char AnlTPG2Txt[]						= "ANL-TPG2";
code char AnlTPG5Txt[]						= "ANL-TPG5";
code char SDITSG2Txt[]						= "SDI-TSG2";
code char SDITSG3Txt[]						= "SDI-TSG3";
code char SDITSG4Txt[]						= "SDI-TSG4";
code char SDITPG1Txt[]						= "SDI-TPG1";
code char SDITPG2Txt[]						= "SDI-TPG2";
code char SDITPG5Txt[]						= "SDI-TPG5";
code char SystemTxt[]						= "SYSTEM";
code char PatternTxt[]						= "PATTERN";
code char ScHPhaseTxt[]						= "ScH-PHASE";
code char TimingTxt[]						= "TIMING";
code char EDHTxt[]							= "EDH";
code char TextTxt[]							= "TEXT";
code char AudioTxt[]							= "AUDIO";
code char StyleTxt[]							= "STYLE";
code char PositionTxt[]			 			= "POS";
code char ClockTxt[]							= "CLOCK";
code char GroupTxt[]							= "GROUP";
code char SignalTxt[]						= "SIGNAL";
code char LevelTxt[]							= "LEVEL";
code char ConfigTxt[]						= "CONFIG";
code char OSCTxt[]							= "OSC";
code char RS232Txt[]							= "RS232";
code char RecallTxt[]						= "RECALL";
code char StoreTxt[]							= "STORE";
code char NameTxt[]							= "NAME";

code char AESEBUBNC1Txt[]					= "AES BNC #1";
code char AESEBUBNC2Txt[]					= "AES BNC #2";
code char AESEBUXLR1Txt[]					= "AES XLR #1";
code char AESEBUXLR2Txt[]					= "AES XLR #2";
code char AESEBUWord1Txt[]					= "AES WORD #1";
code char AESEBUWord2Txt[]					= "AES WORD #2";

code char BlkBurstTxt[]						= "BLACK-BURST";
code char AESEBUTxt[]						= "AES-EBU";
code char GenlockTxt[]						= "GENLOCK";
code char PresetTxt[]						= "PRESET";
code char TestTxt[]							= "TEST";

code char EMBAudioTxt[]						= "EMB.AUDIO";

code char ConfigDateTimeTxt[]				= "DATE-TIME";
code char ConfigLockTxt[]					= "LOCK";
code char ConfigAutoESCTxt[]				= "AUTO-ESC";
code char ConfigLCDContTxt[]				= "LCD-CONTRAST";
code char ConfigDownloadTxt[]				= "DOWNLOAD";
code char ConfigDiagnoseTxt[]				= "DIAGNOSE";

code char TimeTxt[]							= "TIME";
code char DateTxt[]							= "DATE";
code char TimeDateTxt[]						= "TIME+DATE";
code char ReferenceTxt[]					= "REFERENCE";
code char OffsetTxt[]						= "OFFSET";

code char RS232BaudrateTxt[]				= "BAUD-RATE";
code char RS232DatabitTxt[]				= "DATA-BIT";
code char RS232ParityTxt[]					= "PARITY";
code char RS232HandshakeTxt[]				= "HANDSHAKE";

code char MainTxt[]							= "Main";
code char OptionsTxt[]						= "Options";
code char MemoryTxt[] 						= "Memory";
code char DisplayTxt[]						= "Display";
code char KeyboardSLTxt[]					= "Keyboard";
code char VersionTxt[]						= "Version";
code char ConfigurationTxt[]				= "Configuration";
code char ErrorQueueTxt[]					= "ErrorQueue";

code char GeneralFailureTxt[]				= "General failure: ";
code char NoContactToTxt[]					= "No contact to ";
code char ErrorWritingToTxt[]				= "Error writing to ";
code char NoResponseFromTxt[]				= "No response from ";
code char ErrorReadingFromTxt[]			= "Error reading from ";
code char ErrorDownloadingFromTxt[]		= "Error downloading from ";
code char ChecksumErrorTxt[]				= "Checksum error: ";

code char MultipleFailureTxt[]			= "MULTIPLE FAILURE";
code char PowerFailureTxt[]				= "POWER FAILURE: ";
code char TemperatureFailureTxt[]		= "TEMPERATURE FAILURE";

code char NoWarningsTxt[]					= "No active warning";
code char NoErrorDetectedTxt[]			= "No error detected";
code char TemperatureWarningTxt[]		= "Temperature warning";
code char LevelDetectorTxt[]				= "Level error at ";
code char ConfigurationErrorTxt[]		= "Configuration error";
code char MultipleErrorTxt[]				= "Multiple errors detected";

code char TestingTxt[]						= "Testing ";
code char TestingMainboardTxt[]			= "Testing main board";
code char TestingBlackburstTxt[]			= "Testing black burst unit";
code char TestingSPGBoardTxt[]			= "Testing oscillator board";
code char TestingRS232CommTxt[]			= "Testing RS232 communication";
code char TestingMainCompletedTxt[]		= "Testing main completed";
code char TestingOptionsCompletedTxt[]	= "Testing options completed";
code char TestingMemoryCompletedTxt[]	= "Testing memory completed";
code char TestingButtonsCompletedTxt[]	= "Testing buttons completed";

code char NoOptionsDetectedTxt[]			= "No options have been detected!!";
code char LoopbackConnectorTxt[]			= "Please insert a loopback connector!!";

code char ResetErrorQueueTxt[]			= "Reset ErrorQueue ?";
code char RS232TestTxt[]					= "RS232TEST";

code char VoltageN5Txt[]					= "-5 VOLT";
code char Voltage12Txt[]					= "12 VOLT";

code char SDIGenlockTxt[]					= "SDI GENLOCK";
code char TimeCodeTxt[]						= "TIME CODE";
code char ROMMemoryTxt[]					= "ROM MEMORY";
code char RAMMemoryTxt[]					= "RAM MEMORY";
code char KeyboardTxt[]						= "KEYBOARD";
code char LCDADConvTxt[]					= "LCD ADC";
code char TEMPADConvTxt[]					= "TEMP ADC";
code char VoltageTxt[]						= "POWER ADC";
code char LevelDetTxt[]						= "LEVEL DETECTOR";
code char UnknownOptionTxt[]				= "UNKNOWN OPTION";

code char GENLOCKEDTxt[]					= "GENLOCKED";
code char SYNCLOCKEDTxt[]					= "SYNCLOCKED";
code char UNLOCKEDTxt[]						= "UNLOCKED";
code char NOGENLOCKTxt[]					= "NO GENLOCK";
code char NOSIGNALTxt[]						= "NO SIGNAL";
code char PRESENTTxt[]						= "PRESENT";
code char TOOSLOWTxt[]						= "TOO SLOW";
code char TOOFASTTxt[]						= "TOO FAST";
code char NOVITCTxt[]						= "NO VITC";
code char VITCPALTxt[]						= "VITC(PAL)";
code char VITCNTSCTxt[]						= "VITC(NTSC)";
code char NOLTCTxt[]							= "NO LTC";
code char LTCPALTxt[]						= "LTC(PAL)";
code char LTCNTSCTxt[]						= "LTC(NTSC)";

code char OnTxt[]								= "On";
code char OffTxt[]							= "Off";
code char MinusTxt[]							= "-";
code char PlusTxt[]							= "+";

code char UPButtonTxt[]						= "UP ";
code char DOWNButtonTxt[]					= "DOWN ";
code char LEFTButtonTxt[]					= "LEFT ";
code char RIGHTButtonTxt[]					= "RIGHT ";
code char EXECUTEButtonTxt[]				= "EXECUTE ";
code char CBARButtonTxt[]					= "C.BAR ";
code char MBURSTButtonTxt[]				= "M.BURST ";
code char WINDOWButtonTxt[]				= "WINDOW ";
code char PRESETButtonTxt[]				= "PRESET ";
code char SPECIALButtonTxt[]				= "SPECIAL ";
code char LINEARITYButtonTxt[]			= "LINEARITY ";
code char PATTERNButtonTxt[]				= "PATTERN ";
code char OUTPUTButtonTxt[]				= "OUTPUT ";

code char BB1Txt[]							= "BB1";
code char BB2Txt[]							= "BB2";
code char BB3Txt[]							= "BB3";
code char BB4Txt[]							= "BB4";
code char BB5Txt[]							= "BB5";
code char BB6Txt[]							= "BB6";
code char BB7Txt[]							= "BB7";
code char BB8Txt[]							= "BB8";
code char BBMulTxt[]							= "BB2+MUL";

code char AESEBU1Txt[]						= "AES-EBU1";
code char AESEBU2Txt[]						= "AES-EBU2";

code char NTSCTxt[]							= "NTSC";
code char NTSCwoTxt[]						= "NTSC wo. setup";
code char PALTxt[]							= "PAL";
code char PAL_PALIDTxt[]					= "PAL w/PAL ID";

code char SDI525Txt[]						= "525/59.94";
code char SDI625Txt[]						= "625/50";

code char SMPTECBarTxt[]					= "SMPTE C.Bar";
code char EBUCBarTxt[] 						= "EBU C.Bar";
code char FCCBarTxt[]						= "FCC C.Bar";
code char EBUCBarITUTxt[]					= "C.Bar ITU801";
code char CBar100Txt[]						= "100% C.Bar";
code char CBarGrey75Txt[]					= "75% C.Bar+Grey";
code char CBarRed75Txt[]					= "75% C.Bar+Red";
code char Red75Txt[]							= "75% Red";

code char MultiburstTxt[]					= "Multiburst";
code char LuminanceSweepTxt[]				= "Lum. Sweep";
code char YCrCbSweepTxt[]					= "Y, Cr, Cb Sweep";
code char MultipulseTxt[]					= "Multipulse";
code char SinxxTxt[]							= "Sinx/x";
code char CCIR18Txt[]						= "CCIR Line18";
code char NTC7CombTxt[]						= "NTC7 Combination";
code char FCCMultiburstTxt[]				= "FCC Multiburst";

code char Window10Txt[]						= "Window 10%";
code char Window15Txt[]						= "Window 15%";
code char Window20Txt[]		 				= "Window 20%";
code char Window100Txt[]		 			= "Window 100%";
code char BlWh15kHzTxt[]  					= "15kHz Bl/Wh";
code char Grey50Txt[]  						= "Grey 50%";
code char White100Txt[]      				= "White 100%";
code char BlackTxt[]							= "Black";
code char BlackBurstTxt[]					= "Black Burst";

code char SDICheckFieldTxt[]				= "SDI Check Field";
code char DigitalTimingTxt[]				= "Digital Timing";
code char FieldDelayTestTxt[]				= "Field Delay Test";
code char BowTieTxt[]						= "Bow Tie";
code char AnalogBlankingTxt[]				= "Analog Blanking";
code char DigitalGreyTxt[]					= "Digital Grey";
code char FieldSquareWaveTxt[]  			= "Field sq.wave";
code char BlWh01HzTxt[]      				= "0.1Hz Bl/Wh";
code char EOLTxt[]   						= "End of line";
code char WhiteEOLTxt[]   					= "White end";
code char BlueEOLTxt[]   					= "Blue end of line";
code char RedEOLTxt[]   					= "Red end of line";
code char YellowEOLTxt[]					= "Yell end of line";
code char CyanEOLTxt[]  					= "Cyan end of line";
code char Custom1Txt[]  					= "Customized 1";
code char Custom2Txt[]  					= "Customized 2";
code char Custom3Txt[]  					= "Customized 3";
code char Custom4Txt[]  					= "Customized 4";
code char Custom5Txt[]  					= "Customized 5";
code char Custom6Txt[]  					= "Customized 6";
code char Custom7Txt[]  					= "Customized 7";
code char Custom8Txt[]  					= "Customized 8";

code char ShallowRampTxt[]         		= "Shallow Ramp";
code char RampTxt[]							= "Ramp";
code char LimitRampTxt[]         		= "Limit Ramp";
code char ValidRampTxt[]         		= "Valid Ramp";
code char ModulatedRampTxt[]   			= "Ramp Modulated";
code char Stair5Txt[]        				= "Staircase 5step";
code char ModulatedStair5Txt[]			= "Staircase Modul.";
code char Stair10Txt[]        			= "Staircase 10step";
code char PulseBarTxt[]      				= "Pulse & Bar";
code char CCIR17Txt[]        				= "CCIR Line17";
code char CCIR330Txt[]       				= "CCIR Line330";
code char CCIR331Txt[]       				= "CCIR Line331";
code char FCCCompositeTxt[]  				= "FCC Composite";
code char NTC7CompositeTxt[]  			= "NTC7 Composite";
code char YellowGreyRampTxt[]  	      = "Ramp Yellow/Grey";
code char GreyBlueRampTxt[]  	      	= "Ramp Grey/Blue";
code char CyanGreyRampTxt[]  	   	   = "Ramp Cyan/Grey";
code char GreyRedRampTxt[]					= "Ramp Grey/Red";
code char CbYCrYRampTxt[]  			   = "Ramp Cb,Y,Cr,Y";

code char Philips4x3Txt[]    				= "PHILIPS 4:3";
code char Philips16x9Txt[]    			= "PHILIPS 16:9";
code char FuBK4x3Txt[]						= "FuBK 4:3";
code char FuBK16x9Txt[]						= "FuBK 16:9";
code char CrosshatchTxt[]	 				= "Crosshatch";
code char Crosshatch16x9Txt[]				= "Crosshatch 16:9";
code char WhiteCircle4x3Txt[]				= "Circle 4:3";
code char WhiteCircle16x9Txt[]			= "Circle 16:9";
code char PLUGETxt[]				 			= "PLUGE";
code char SafeAreaTxt[]		 				= "Safe Area";
code char kHz250Txt[]		 				= "Sq.wave 250kHz";
code char VMT01Txt[]		 					= "VMT 01";

code char EMBAudioGroup1Txt[]				= "1, Chan 1-4";
code char EMBAudioGroup2Txt[]				= "2, Chan 5-8";
code char EMBAudioGroup3Txt[]				= "3, Chan 9-12";
code char EMBAudioGroup4Txt[]				= "4, Chan 13-16";

code char Line1Txt[]							= "LINE1";
code char Line2Txt[]							= "LINE2";
code char Line3Txt[]							= "LINE3";
code char LineUpperTxt[]					= "UPPER";
code char LineLowerTxt[]					= "LOWER";
code char LineLeftTxt[]						= "LEFT";
code char LineRightTxt[]					= "RIGHT";

code char FreeTxt[]							= "Free";
code char StandardTxt[]						= "Standard";
code char ComplexTxt[]						= "Complex";

code char Stereo800HzTxt[]					= "Stereo 800 Hz";
code char Stereo1kHzTxt[]					= "Stereo 1 kHz";
code char StereoEBU1kHzTxt[]				= "Stereo EBU 1 kHz";
//code char StereoBBC1kHzTxt[]				= "Stereo BBC 1 kHz";
code char MonoEBU1kHzTxt[]					= "Mono EBU 1 kHz";
code char Mono1kHzTxt[]						= "Mono 1 kHz";
code char DualTxt[]							= "Dual 1kHz 400Hz";
code char WordClockTxt[]					= "Word-clock";

code char SilenceTxt[]						= "Silence";
code char Level0dBTxt[]						= "  0 dBFS";
code char Level9dBTxt[]						= " -9 dBFS";
code char Level12dBTxt[]					= "-12 dBFS";
code char Level14dBTxt[]					= "-14 dBFS";
code char Level15dBTxt[]					= "-15 dBFS";
code char Level16dBTxt[]					= "-16 dBFS";
code char Level18dBTxt[]					= "-18 dBFS";
code char Level20dBTxt[]					= "-20 dBFS";

code char NTSCPhase1Txt[]					= "NTSC Phase 1";
code char NTSCPhase2Txt[]					= "NTSC Phase 2";
code char NTSCPhase3Txt[]					= "NTSC Phase 3";
code char NTSCPhase4Txt[]					= "NTSC Phase 4";
code char NTSCPhase5Txt[]					= "NTSC Phase 5";

code char GenlockATxt[]						= "A";
code char GenlockBTxt[]						= "B";
code char LoopThroughTxt[]					= "A-B";
code char SDITxt[]							= "SDI";
code char InternalGenlockTxt[]			= "Internal";
code char InternalOCXOGenlockTxt[]		= "Internal 2";

code char PALBurstTxt[]						= "PAL Burst";
code char NTSCBurstTxt[]					= "NTSC Burst";
code char Sync625Txt[]						= "625 Sync";
code char Sync525Txt[]						= "525 Sync";
code char MHz358Txt[]						= "3.58 MHz";
code char MHz443Txt[]						= "4.43 MHz";
code char MHz5Txt[]							= "5 MHz";
code char MHz10Txt[]							= "10 MHz";

code char NormalTxt[]						= "Normal";
code char PanelTxt[]							= "Panel";
code char DateTimeTxt[]						= "Date-Time";
code char DownloadTxt[]						= "Download";
code char DiagnoseTxt[]						= "Diagnose";

code char Hour12Txt[]						= "12h";
code char Hour24Txt[]						= "24h";

code char DMYTxt[]							= "DD-MM-YY";
code char MDYTxt[]							= "MM-DD-YY";
code char YMDTxt[]							= "YY-MM-DD";

code char LTCInputTxt[]						= "LTC-input";
code char VITCInputTxt[]					= "VITC on genlock";
code char VideoFieldFreqInputTxt[]		= "Video Field Freq";
code char Reference1HzTxt[]				= "1 Hz Reference";
code char InternalTxt[]						= "Internal";

code char PresetNoTxt[]						= "Preset #";
code char AllPresetTxt[]					= "All Presets";
code char CompleteSPGTxt[]					= "Complete SPG";

code char AttribAntiPALTxt[]				= "Anti-PAL";
code char AttribPLUGETxt[]					= "PLUGE";
code char AttribStair10Txt[]				= "Staircase 10";
code char AttribMotionTxt[]				= "Motion";
code char AttribCirclesTxt[]				= "Circles";
code char AttribLogoTxt[]					= "Logo";

code char N300Txt[]							= "300";
code char N600Txt[]							= "600";
code char N1200Txt[]							= "1200";
code char N2400Txt[]							= "2400";
code char N4800Txt[]							= "4800";
code char N9600Txt[]							= "9600";
code char N19200Txt[]						= "19200";

code char NoneTxt[]							= "None";
code char OddTxt[]							= "Odd";
code char EvenTxt[]							= "Even";

code char NoHandshakeTxt[]					= "None";
code char XONXOFFTxt[]						= "XON/XOFF";
code char RTSCTSTxt[]						= "RTS/CTS";

code char MainConnTxt[]						= "[ MAIN]: ";
code char OSCConnTxt[]						= "[  OSC]: ";
code char BBConnTxt[]						= "[ BB  ]: ";
code char ATPGConnTxt[]						= "[ATPG ]: ";
code char STSGConnTxt[]						= "[STSG ]: ";
code char STPGConnTxt[]						= "[STPG ]: ";
code char SDIGenlockConnTxt[]				= "[ SDIG]: ";
code char Int2GenlockConnTxt[]			= "[10MHz]: ";
code char AESEBUConnTxt[]					= "[  AES]: ";
code char TimeConnTxt[]						= "[ TIME]: ";

code char* code AnlTPGHdrArrTxt[]		= { AnlTPG2HdrTxt,
														 AnlTPG5HdrTxt };

code char* code SDITSGHdrArrTxt[]		= { SDITSG2HdrTxt,
														 SDITSG3HdrTxt,
														 SDITSG4HdrTxt };

code char* code SDITPGHdrArrTxt[]		= { SDITPG1HdrTxt,
														 SDITPG2HdrTxt,
														 SDITPG5HdrTxt };

code char* code AESEBUHdrArrTxt[]		= { AESEBU1HdrTxt,
														 AESEBU2HdrTxt };

code char* code ButtonArrTxt[]			= { LEFTButtonTxt,
														 DOWNButtonTxt,
														 RIGHTButtonTxt,
														 EXECUTEButtonTxt,
														 CBARButtonTxt,
														 MBURSTButtonTxt,
														 WINDOWButtonTxt,
														 PRESETButtonTxt,
														 SPECIALButtonTxt,
														 LINEARITYButtonTxt,
														 PATTERNButtonTxt,
														 OUTPUTButtonTxt };

code char* code BBArrTxt[]					= { BB1Txt,
														 BB2Txt,
														 BB3Txt,
														 BB4Txt,
														 BB5Txt,
														 BB6Txt,
														 BB7Txt,
														 BB8Txt };

code char* code AnlTPGArrTxt[]			= { AnlTPG2Txt,
														 AnlTPG5Txt };

code char* code SDITSGArrTxt[]			= { SDITSG2Txt,
														 SDITSG3Txt,
														 SDITSG4Txt };

code char* code SDITPGArrTxt[]			= { SDITPG1Txt,
														 SDITPG2Txt,
														 SDITPG5Txt };

code char* code AnlSystemArrTxt[]		= { PALTxt,
														 PAL_PALIDTxt,
														 NTSCTxt,
														 NTSCwoTxt };

code char* code SDISystemArrTxt[]		= { SDI625Txt,
														 SDI525Txt };

code char* code AnlPattArrTxt[]			= { SMPTECBarTxt,
														 EBUCBarTxt,
														 FCCBarTxt,
														 CBar100Txt,
														 CBarGrey75Txt,
														 CBarRed75Txt,
														 Red75Txt,

														 MultiburstTxt,
														 LuminanceSweepTxt,
														 MultipulseTxt,
														 SinxxTxt,
														 CCIR18Txt,
														 NTC7CombTxt,
														 FCCMultiburstTxt,

														 Window10Txt,
														 Window15Txt,
														 Window20Txt,
														 Window100Txt,
														 BlWh15kHzTxt,
														 Grey50Txt,
														 White100Txt,
														 BlackBurstTxt,

														 FieldSquareWaveTxt,
														 BlWh01HzTxt,
														 Custom1Txt,
														 Custom2Txt,
														 Custom3Txt,
														 Custom4Txt,
														 Custom5Txt,
														 Custom6Txt,
														 Custom7Txt,
														 Custom8Txt,

														 ShallowRampTxt,
														 RampTxt,
														 ModulatedRampTxt,
														 Stair5Txt,
														 ModulatedStair5Txt,
														 Stair10Txt,
														 PulseBarTxt,
														 CCIR17Txt,
														 CCIR330Txt,
														 CCIR331Txt,
														 FCCCompositeTxt,
														 NTC7CompositeTxt,

														 Philips4x3Txt,
														 Philips16x9Txt,
														 FuBK4x3Txt,
														 FuBK16x9Txt,
														 CrosshatchTxt,
														 Crosshatch16x9Txt,
														 WhiteCircle4x3Txt,
														 WhiteCircle16x9Txt,
														 PLUGETxt,
														 SafeAreaTxt,
														 kHz250Txt,
														 VMT01Txt };

code char* code SDIPattArrTxt[]			= { SMPTECBarTxt,
														 EBUCBarTxt,
														 FCCBarTxt,
														 EBUCBarITUTxt,
														 CBar100Txt,
														 CBarGrey75Txt,
														 CBarRed75Txt,
														 Red75Txt,

														 MultiburstTxt,
														 LuminanceSweepTxt,
														 YCrCbSweepTxt,
														 MultipulseTxt,
														 SinxxTxt,
														 CCIR18Txt,

														 Window10Txt,
														 Window15Txt,
														 Window20Txt,
														 Window100Txt,
														 BlWh15kHzTxt,
														 White100Txt,
														 BlackTxt,

														 SDICheckFieldTxt,
														 DigitalTimingTxt,
														 FieldDelayTestTxt,
														 BowTieTxt,
														 AnalogBlankingTxt,
														 DigitalGreyTxt,
														 FieldSquareWaveTxt,
														 BlWh01HzTxt,
														 EOLTxt,
														 WhiteEOLTxt,
														 BlueEOLTxt,
														 RedEOLTxt,
														 YellowEOLTxt,
														 CyanEOLTxt,
														 Custom1Txt,
														 Custom2Txt,
														 Custom3Txt,
														 Custom4Txt,
														 Custom5Txt,
														 Custom6Txt,
														 Custom7Txt,
														 Custom8Txt,

														 ShallowRampTxt,
														 RampTxt,
														 LimitRampTxt,
														 ValidRampTxt,
														 Stair5Txt,
														 ModulatedStair5Txt,
														 Stair10Txt,
														 PulseBarTxt,
														 CCIR17Txt,
														 CCIR330Txt,
														 CCIR331Txt,
														 YellowGreyRampTxt,
														 GreyBlueRampTxt,
														 CyanGreyRampTxt,
														 GreyRedRampTxt,
														 CbYCrYRampTxt,

														 Philips4x3Txt,
														 Philips16x9Txt,
														 FuBK4x3Txt,
														 FuBK16x9Txt,
														 CrosshatchTxt,
														 PLUGETxt,
														 SafeAreaTxt,
														 VMT01Txt };

code char* code AttribArrTxt[]			= { AttribAntiPALTxt,
														 AttribPLUGETxt,
														 AttribStair10Txt,
														 AttribMotionTxt,
														 AttribCirclesTxt,
														 AttribLogoTxt };

code char* code TSGAudioSignalArrTxt[]	= { OffTxt,
														 Stereo1kHzTxt };

code char* code TSGAudioLevelArrTxt[]	= { SilenceTxt,
														 Level0dBTxt,
														 Level9dBTxt,
														 Level15dBTxt,
														 Level18dBTxt };

code char* code TPGAudioSignalArrTxt[]	= { OffTxt,
														 Stereo800HzTxt,
														 Stereo1kHzTxt,
														 StereoEBU1kHzTxt,
												//		 StereoBBC1kHzTxt, // JK
														 MonoEBU1kHzTxt,
														 Mono1kHzTxt,
														 DualTxt };

code char* code TPGAudioLevelArrTxt[]	=
  {
  SilenceTxt,
  Level0dBTxt,
  Level9dBTxt,
  Level12dBTxt,
// Level14dBTxt,
// Level15dBTxt,     // JK
  Level16dBTxt,
  Level18dBTxt,
  Level20dBTxt
  };

code char* code TPGAudioGroupArrTxt[]	= { EMBAudioGroup1Txt,
														 EMBAudioGroup2Txt,
														 EMBAudioGroup3Txt,
														 EMBAudioGroup4Txt };

code char* code TPGTextEditArrTxt[]		= { Line1Txt,
														 Line2Txt,
														 Line3Txt };

code char* code TPGTextStyleArrTxt[]	= { FreeTxt,
														 StandardTxt,
														 ComplexTxt };

code char* code TPGTextClockArrTxt[]	= { NoneTxt,
														 TimeTxt,
														 DateTxt,
														 TimeDateTxt };

code char* code AESEBUSignalArrTxt[]	= { OffTxt,
														 Stereo800HzTxt,
														 Stereo1kHzTxt,
														 StereoEBU1kHzTxt,
													//	 StereoBBC1kHzTxt,     // JK
														 MonoEBU1kHzTxt,
														 Mono1kHzTxt,
														 DualTxt,
														 WordClockTxt };

code char* code AESEBULevelArrTxt[]		= { SilenceTxt,
														 Level0dBTxt,
														 Level9dBTxt,
														 Level12dBTxt,
														 Level14dBTxt,
												//		 Level15dBTxt,         // JK
														 Level16dBTxt,
														 Level18dBTxt,
														 Level20dBTxt };

code char* code AESEBUTimingArrTxt[]	= { PALTxt,
														 NTSCPhase1Txt,
														 NTSCPhase2Txt,
														 NTSCPhase3Txt,
														 NTSCPhase4Txt,
														 NTSCPhase5Txt };

code char* code GenlockInputArrTxt[]	= { GenlockATxt,
														 GenlockBTxt,
														 LoopThroughTxt,
														 SDITxt,
														 InternalGenlockTxt,
														 InternalOCXOGenlockTxt };

code char* code GenlockSignalArrTxt[]	= { PALBurstTxt,
														 NTSCBurstTxt,
														 Sync625Txt,
														 Sync525Txt,
														 SDI625Txt,
														 SDI525Txt,
														 MHz358Txt,
														 MHz443Txt,
														 MHz5Txt,
														 MHz10Txt };

code char* code LockArrTxt[]				= { NormalTxt,
														 PanelTxt,
														 DateTimeTxt,
														 DownloadTxt,
														 DiagnoseTxt };

code char* code OffOnArrTxt[]				= { OffTxt,
														 OnTxt };

code char* code MinusPlusArrTxt[]		= { MinusTxt,
														 PlusTxt };

code char* code TimeFormatArrTxt[]		= { Hour12Txt,
														 Hour24Txt };

code char* code DateFormatArrTxt[]		= { DMYTxt,
														 MDYTxt,
														 YMDTxt };

code char* code DateTimeRefArrTxt[]		= { VITCInputTxt,
														 LTCInputTxt,
														 Reference1HzTxt,
														 VideoFieldFreqInputTxt };

code char* code DownloadArrTxt[]			= { PresetNoTxt,
														 AllPresetTxt,
														 CompleteSPGTxt };

code char* code RS232BaudrateArrTxt[]	= { N300Txt,
														 N600Txt,
														 N1200Txt,
														 N2400Txt,
														 N4800Txt,
														 N9600Txt,
														 N19200Txt };

code char* code RS232ParityArrTxt[]		= { NoneTxt,
														 OddTxt,
														 EvenTxt };

code char* code RS232HandshakeArrTxt[]	= { NoHandshakeTxt,
														 XONXOFFTxt,
														 RTSCTSTxt };

