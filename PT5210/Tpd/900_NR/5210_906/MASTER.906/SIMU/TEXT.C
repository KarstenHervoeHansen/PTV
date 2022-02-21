/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		TEXT.C																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960613																		*/
/*	Rev. date:	980107, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	This module contains NO functions.													*/
/*																									*/
/*	Changes:																						*/
/* 970108: /906 version. Final texts													*/
/* 971215: /906 versiion. Added SDIPhilipsPattern									*/ 
/* 971212: DataNotCompatibleTxt[]														*/
/* 971027: Added TestingBlackburstTxt													*/
/* 971016: Added N19200Txt & NoHandshakeTxt & ResetErrorQueueTxt				*/
/* 971014: Added TestingOptionsCompletedTxt & LoopbackConnectorTxt			*/
/*			  Added RS232TestTxt																*/
/* 971002: Removed ConfigurationArrTxt. Added ConnectorArrTxt					*/
/* 970930; Added several texts for diagnose-menus									*/
/* 970909: Several new texts added														*/
/* 970821: Several new texts added														*/
/* 970205: Several new texts added														*/
/* 970201: Added Diagnose texts															*/
/* 970127: Added VoltageTxt																*/
/* 970112: Updated AES-EBU audio levels												*/
/* 970110: Added definitions for error code text									*/
/* 961222: Added definitions for new signal text									*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include "52sync.h"
#include "text.h"

code char PT5210HeadlineTxt[] 			= "PT 5210 VariTime digital SPG";

code char PowerUp_DefaultTxt[] 			= "PT 5210 VariTime SPG Power-up diagnose  ";
code char PowerUp_MasterResetTxt[] 		= "PT 5210 VariTime SPG Master-reset ...";
code char PowerUp_FactResetTxt[] 		= "PT 5210 VariTime SPG Factory-reset ...";

code char Status_PresetTxt[]				= "PT 5210 Preset Status:";
code char Status_ErrorStatusTxt[]		= "PT 5210 Error/Warning Status:";

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

code char PresetHdrTxt[]					= "PRESET [ ]: ";
code char MenuHdrTxt[]						= "MENU  : ";
code char SubmnuHdrTxt[]					= "SUBMNU: ";
code char SecondmnuHdrTxt[]				= "2NDMNU: ";
code char SelectHdrTxt[]					= "SELECT: ";
code char AnalogSignalHdrTxt[]			= "ANALOG-SIGNAL: ";
code char SDISignalHdrTxt[]				= "SDI-SIGNAL: ";
code char SystemHdrTxt[]					= "SYSTEM: ";
code char PatternHdrTxt[]					= "PATTERN: ";
code char ScHPhaseHdrTxt[]					= "ScH-PHASE: ";
code char SBEDHHdrTxt[]						= "EDH-INSERTION: ";
code char SignalHdrTxt[]					= "SIGNAL: ";
code char LevelHdrTxt[]						= "LEVEL: ";
code char TimingHdrTxt[]					= "TIMING: ";
code char SBAudioHdrTxt[]					= "EMBEDDED-AUDIO: ";
code char BlackCopyHdrTxt[]				= "COPY CONFIG. FROM: ";
code char TimeVHdrTxt[]						= "V: ";
code char TimeHHdrTxt[]						= "  H: ";
code char TimeTHdrTxt[]						= "  T: ";
code char UserTextHdrTxt[]					= "USER TEXT: ";
code char GenlockHdrTxt[]					= "GENLOCK: ";
code char AES_EBUHdrTxt[]					= "AES-EBU: ";
code char InpHdrTxt[]						= "INP: ";
code char TextHdrTxt[]						= "TEXT: ";
code char RecallHdrTxt[]					= "RECALL [ ]: ";
code char StoreHdrTxt[]						= "STORE [ ]: ";
code char NameHdrTxt[]						= "NAME [ ]: ";
code char AutoReturnStatusHdrTxt[]		= "AUTO RETURN TO STATUS: ";
code char DownloadHdrTxt[]					= "DOWNLOAD: ";
code char BaudrateHdrTxt[]					= "BAUD-RATE: ";
code char DatabitHdrTxt[]					= "DATA-BIT: ";
code char ParityHdrTxt[]					= "PARITY: ";
code char HandshakeHdrTxt[]				= "HANDSHAKE: ";
code char StatusHdrTxt[]					= "STATUS: ";
code char ErrorNoHdrTxt[]					= "E(   ): ";
code char VersionHdrTxt[]					= "Version: ";
code char TestingMemoryHdrTxt[]		   = "Testing memory: ";
code char PressButtonHdrTxt[]				= "Press button: ";

code char SlashDotHdrTxt[]					= "../";
code char ThreeDotsTxt[]					= "...";
code char degTxt[]							= " deg";
code char MenuTxt[]							= "menu";
code char SubmnuTxt[]						= "submenu";
code char SelectTxt[]						= "select";

code char SYSTxt[] 							= "SYS";
code char EDITTxt[] 							= "EDIT";
code char SAVETxt[] 							= "SAVE";
code char OKTxt[]								= "OK";
code char FAILTxt[]							= "FAIL";
code char ESCTxt[] 							= "ESC";
code char ROMTxt[]							= "ROM";
code char RAMTxt[]							= "RAM";

code char Select_Txt[]						= ", select";
code char Select_FunctionTxt[]			= ", select function";
code char Select_InputTxt[]				= ", select input";
code char Select_SignalTxt[]				= ", select signal";
code char Select_LevelTxt[]				= ", select level";
code char Edit_Txt[]							= ", edit";
code char Edit_DelayTxt[]					= ", edit delay";
code char Edit_TextTxt[]					= ", edit text";
code char Edit_NameTxt[]					= ", edit name";
code char Configure_Txt[]					= ", configure";
code char Configure_OutputTxt[]			= ", configure output";

code char HyphenTxt[]						= "------";

code char FromKUTxt[]						= " from KU";

code char AnalogSignalTxt[]				= "ANALOG-SIGNAL";
code char SDISignalTxt[]					= "SDI-SIGNAL";
code char SystemTxt[]						= "SYSTEM";
code char PatternTxt[]						= "PATTERN";
code char ScHPhaseTxt[]						= "ScH-PHASE";
code char TimeTxt[]							= "TIME";
code char CopyTxt[]							= "COPY";
code char EDHTxt[]							= "EDH";
code char TextTxt[]							= "TEXT";
code char ConfigTxt[]						= "CONFIG";
code char OSCTxt[]							= "OSC";
code char RS232Txt[]							= "RS232";
code char SignalTxt[]						= "SIGNAL";
code char LevelTxt[]							= "LEVEL";
code char TimingTxt[]						= "TIMING";
code char RecallTxt[]						= "RECALL";
code char StoreTxt[]							= "STORE";
code char NameTxt[]							= "NAME";

code char ANLBlackTxt[]						= "ANALOG-BLACK";
code char SDIBlackTxt[]						= "SDI-BLACK&CB";
code char AudioTxt[]							= "AES-EBU";
code char GenlockTxt[]						= "GENLOCK";
code char PresetTxt[]						= "PRESET";
code char TestTxt[]							= "TEST";

code char SBAudioTxt[]						= "EMB.AUDIO";
code char SDIAudioSignalTxt[]				= "EMB.AUDIO-SIGNAL";
code char SDIAudioLevelTxt[]				= "EMB.AUDIO-LEVEL";

code char ConfigLockTxt[]					= "LOCK";
code char ConfigAutoESCTxt[]				= "AUTO-ESC";
code char ConfigLCDContTxt[]				= "LCD-CONTRAST";
code char ConfigDownloadTxt[]				= "DOWNLOAD";
code char ConfigDiagnoseTxt[]				= "DIAGNOSE";

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
code char LCD_ADConvTxt[]					= "LCD ADC";
code char TEMP_ADConvTxt[]					= "TEMP ADC";
code char VoltageTxt[]						= "POWER ADC";
code char LevelDetTxt[]						= "LEVEL DETECTOR";
code char UnknownOptionTxt[]				= "UNKNOWN OPTION";

code char GENLOCKEDTxt[]					= "GENLOCKED";
code char UNLOCKEDTxt[]						= "UNLOCKED";

code char OnTxt[]								= "On";
code char OffTxt[]							= "Off";

code char UPButtonTxt[]						= "UP ";
code char DOWNButtonTxt[]					= "DOWN ";
code char LEFTButtonTxt[]					= "LEFT ";
code char RIGHTButtonTxt[]					= "RIGHT ";
code char EXECUTEButtonTxt[]				= "EXECUTE ";
code char PRESETButtonTxt[]				= "PRESET ";
code char OUTPUTButtonTxt[]				= "OUTPUT ";
code char GENLOCKButtonTxt[]				= "GENLOCK ";

code char BB1Txt[]							= "BB1";
code char BB2Txt[]							= "BB2";
code char BB3Txt[]							= "BB3";
code char BB4Txt[]							= "BB4";
code char BB5Txt[]							= "BB5";
code char BB6Txt[]							= "BB6";
code char BB7Txt[]							= "BB7";
code char BB8Txt[]							= "BB8";
code char BBMulTxt[]							= "BB2+MUL";
code char SB34Txt[]							= "SB3-4";
code char SB56Txt[]							= "SB5-6";
code char SB78Txt[]							= "SB7-8";

code char NTSCTxt[]							= "NTSC";
code char NTSCwoTxt[]						= "NTSC wo. setup";
code char PALTxt[]							= "PAL";
code char PAL_PALIDTxt[]					= "PAL w/PAL ID";

code char SDI525Txt[]						= "525/59.94";
code char SDI625Txt[]						= "625/50";

code char SMPTECBarTxt[]					= "SMPTE C.Bar";
code char EBUCBarTxt[] 						= "EBU C.Bar";
code char EBUCBar8bitTxt[]					= "EBU C.Bar - 8 bit";
code char CBar100Txt[]						= "100% C.Bar";
code char CBar75GreyTxt[]					= "75% C.Bar+Grey";
code char CBar75RedTxt[]					= "75% C.Bar+Red";
code char Window15Txt[]						= "Window 15%";
code char Window20Txt[]		 				= "Window 20%";
code char Window100Txt[]		 			= "Window 100%";
code char CrossHatchTxt[]	 				= "Cross Hatch";
code char PLUGETxt[]				 			= "PLUGE";
code char SafeAreaTxt[]		 				= "Safe Area";
code char ShallowRampTxt[]					= "Shallow Ramp";
code char MultiBurstTxt[]					= "Multiburst";
code char Red75Txt[]							= "75% Red";
code char BlackBurstTxt[]					= "Black Burst";

code char SDICheckFieldTxt[]				= "SDI Check Field";
code char DigitalTimingTxt[]				= "Digital Timing";
code char BlackTxt[]							= "Black";

code char SDIPhilipsTxt[]					= "Philips1";
code char SDIPhilipswoAPALTxt[]			= "Philips2";
code char SDIPhilipsMotionTxt[]			= "Philips";
code char SDIPhilipswoAPALMotionTxt[]	= "Philips wo/APAL";

code char Stereo800HzTxt[]					= "Stereo 800 Hz";
code char Stereo1kHzTxt[]					= "Stereo 1 kHz";
code char StereoEBU1kHzTxt[]				= "Stereo EBU 1 kHz";
code char StereoBBC1kHzTxt[]				= "Stereo BBC 1 kHz";
code char MonoEBU1kHzTxt[]					= "Mono EBU 1 kHz";
code char MonoBBC1kHzTxt[]					= "Mono BBC 1 kHz";
code char DualTxt[]							= "Dual 1kHz 400Hz";
code char kHz48Txt[]							= "48 kHz reference";

code char SilenceTxt[]						= "Silence";
code char Level0dBTxt[]						= "  0 dBFS";
code char Level9dBTxt[]						= " -9 dBFS";
code char Level12dBTxt[]					= "-12 dBFS";
code char Level14dBTxt[]					= "-14 dBFS";
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
code char DownloadTxt[]						= "Download";
code char DiagnoseTxt[]						= "Diagnose";

code char PresetNoTxt[]						= "Preset #";
code char AllPresetTxt[]					= "All Presets";
code char CompleteSPGTxt[]					= "Complete SPG";

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

code char ConnMainTxt[]						= "[MAIN]";
code char ConnOSCTxt[]						= "[ OSC]";
code char ConnBB12Txt[]						= "[BB12]";
code char ConnBB34Txt[]						= "[BB34]";
code char ConnBB56Txt[]						= "[BB56]";
code char ConnBB78Txt[]						= "[BB78]";
code char ConnSDITestTxt[]					= "[SDIS]";
code char ConnSDIGenlockTxt[]				= "[SDIG]";
code char ConnAES_EBUTxt[]					= "[ AES]";
code char ConnTimeTxt[]						= "[TIME]";

code char* code ButtonArrTxt[]			= { LEFTButtonTxt,
														 DOWNButtonTxt,
														 RIGHTButtonTxt,
														 EXECUTEButtonTxt,
														 PRESETButtonTxt,
														 OUTPUTButtonTxt,
														 GENLOCKButtonTxt };

code char* code BBArrTxt[]					= { BB1Txt,
														 BB2Txt,
														 BB3Txt,
														 BB4Txt,
														 BB5Txt,
														 BB6Txt,
														 BB7Txt,
														 BB8Txt };

code char* code SBArrTxt[]					= { SB34Txt,
														 SB56Txt,
														 SB78Txt };

code char* code SBPatternArrTxt[]		= { BlackTxt,
														 EBUCBarTxt,
														 CBar100Txt,
														 SMPTECBarTxt };

code char* code AnaTVSystemArrTxt[]		= { PALTxt,
														 PAL_PALIDTxt,
														 NTSCTxt,
														 NTSCwoTxt };

code char* code SDITVSystemArrTxt[]		= { SDI625Txt,
														 SDI525Txt };

code char* code AnalogPatternArrTxt[]	= { SMPTECBarTxt,
														 EBUCBarTxt,
														 CBar100Txt,
														 CBar75GreyTxt,
														 CBar75RedTxt,
														 Window15Txt,
														 Window20Txt,
														 Window100Txt,
														 CrossHatchTxt,
														 PLUGETxt,
														 SafeAreaTxt,
														 ShallowRampTxt,
														 MultiBurstTxt,
														 Red75Txt,
														 BlackBurstTxt };

code char* code SDIPatternArrTxt[]		= { SMPTECBarTxt,
														 EBUCBarTxt,
														 EBUCBar8bitTxt,
														 CBar100Txt,
														 CBar75GreyTxt,
														 CBar75RedTxt,
														 SDICheckFieldTxt,
														 ShallowRampTxt,
														 DigitalTimingTxt,
														 BlackTxt,
														 Window15Txt,
														 Window20Txt,
														 Window100Txt,
														 CrossHatchTxt,
														 PLUGETxt,
														 MultiBurstTxt,
														 Red75Txt,
														 SDIPhilipsTxt,
														 SDIPhilipswoAPALTxt,
														 SDIPhilipsMotionTxt,
														 SDIPhilipswoAPALMotionTxt};

code char* code EMBAudioArrTxt[]			= { OffTxt,
														 SilenceTxt };

code char* code AES_EBUSignalArrTxt[]	 = { OffTxt,
														 Stereo800HzTxt,
														 Stereo1kHzTxt,
														 StereoEBU1kHzTxt,
														 StereoBBC1kHzTxt,
														 MonoEBU1kHzTxt,
														 MonoBBC1kHzTxt,
														 DualTxt,
														 kHz48Txt };

code char* code AES_EBULevelArrTxt[]	= { SilenceTxt,
														 Level0dBTxt,
														 Level9dBTxt,
														 Level12dBTxt,
														 Level14dBTxt,
														 Level16dBTxt,
														 Level18dBTxt,
														 Level20dBTxt };

code char* code AES_EBUTimingArrTxt[]	= { PALTxt,
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
														 DownloadTxt,
														 DiagnoseTxt };

code char* code OffOnArrTxt[]				= { OffTxt,
														 OnTxt };

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

code char* code ConnectorArrTxt[]		= { ConnMainTxt,
														 ConnOSCTxt,
														 ConnBB12Txt,
														 ConnBB34Txt,
														 ConnBB56Txt,
														 ConnBB78Txt,
														 ConnSDITestTxt,
														 ConnSDIGenlockTxt,
														 ConnAES_EBUTxt,
														 ConnTimeTxt };

