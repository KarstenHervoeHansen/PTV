/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 2000						*/
/*	Project:		PT5230 Digital Video Generator										*/
/*	Module:		RS232CMD.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	980211																		*/
/*	Rev. date:	000608, KEn, DEV															*/
/*	Status:		Version 1.5 																*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*																									*/
/*	Changes:																						*/
/* 000508: Added support for TPG text positioning									*/
/* 991101: Customer complaint: SYST:PRES:DOWN didn't work correctly			*/
/*			  Download revision has changed in order to prevent download from	*/
/*				previous versions.															*/
/* 991026: Implemented "DIAG:ERR"														*/
/* 990420: Added Window 10% & 15 kHz Bl/Wh in both SDI & Analog.				*/
/* 990317: Replaced WriteIICVersion() & WriteV24Version() with WriteUnit-	*/
/*				Information().																	*/
/* 990317: Revised WriteV24Version() to include /900 numbers etc.				*/
/*	990305: Added calls SDIValidateAttrib() &	AnlValidateAttrib()				*/
/*			  Error in SystemPresetName() not all characters was legal			*/
/* 981001: CBEBu8 var skrevet med lille u.											*/
/* 980930: Removed MBBC1KHZParTxt altogether.										*/
/* 980929: Added AnlTPGAttribCircles(). Increased AnlTPGLevel3 with 1.		*/
/* 980925: Coorections in xxTPGAttribCommon()										*/
/* 980917: Added new SDI pattern texts. In SDITPGAudioSignal() & 				*/
/*				AESBUSignal(): separate test for MBBC1KHZParTxt						*/
/* 980917: Added Circle 4:3/16:9 and Crosshatch 16:9.								*/
/* 980915: SYNCLOCKED bliver nu returneret for genlock status i Genlock()	*/
/* 980730: Added FACT:TIM:STATus for Time Clock Interface						*/
/* 980728: Final implementation of Time Clock Interface							*/
/* 980702: Update TimeClockRefPar														*/
/* 980616: Released as 4008 002 06812													*/
/* 980618: Minor corrections																*/
/* 980611: Update errorcodes in functions												*/
/* 980604: Added FactoryAudioMode()														*/
/* 980523: Limited no of characters in ...TPGTextString() to 8 for the		*/
/*				PT860x series and to 48 for the PT863x series.						*/
/* 980518: In FindParameter(): Added feed_dog;										*/
/* 980514: Released as 4008 002 06811													*/
/* 021107: Modifications for moving logo, Audio BBc signals removed        */
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
#include "iic.h"
#include "message.h"
#include "cont_drv.h"
#include "keyb_drv.h"
#include "factory.h"

#include "disp_drv.h"

#define SizeOfTmpBuffer		17					// The max. number of characters the
														//  general purpose buffer can hold
#define DownloadOffset		(UC) 20
#define DownloadRevision	(UC)	2			// Download stream revision

#define PT860xMaxNoChar		(UC) 8
#define PT863xMaxNoChar		(UC) 16			//

code char SystemVersionTxt[] 			= "1995.0";
code char FactoryPasswordTxt[] 		= "CWh_bod";

code char IDNResponseCmdTxt[] 		= "*IDN?";
code char DownloadResponseCmdTxt[] 	= ":SYST:PRES:DOWN ";

code char NoErrorsTxt[]					= "No errors";
code char ActiveErrorsTxt[]			= "Active error";
code char NoActiveErrorsTxt[]			= "No active error";

code char CLSCmdTxt[] 					= "CLS";
code char ESECmdTxt[] 					= "ESE";
code char ESRCmdTxt[] 					= "ESR";
code char OPCCmdTxt[] 					= "OPC";
code char SRECmdTxt[] 					= "SRE";
code char STBCmdTxt[] 					= "STB";
code char TSTCmdTxt[] 					= "TST";
code char WAICmdTxt[] 					= "WAI";
code char RSTCmdTxt[] 					= "RST";
code char IDNCmdTxt[] 					= "IDN";

code char SYSTEMCmdTxt[] 				= "SYSTEM";
code char SYSTCmdTxt[] 					= "SYST";
code char STATUSCmdTxt[] 				= "STATUS";
code char STATCmdTxt[] 					= "STAT";
code char PTCmdTxt[] 					= "PT";
code char DIAGNOSTICCmdTxt[] 			= "DIAGNOSTIC";
code char DIAGCmdTxt[] 					= "DIAG";
code char ERRORQUEUECmdTxt[] 			= "ERRORQUEUE";
code char LASTCmdTxt[] 					= "LAST";
code char ENTRYCmdTxt[] 				= "ENTRY";
code char ENTRCmdTxt[] 					= "ENTR";
code char RESETCmdTxt[] 				= "RESET";
code char RESCmdTxt[] 					= "RES";
code char DISPLAYCmdTxt[] 				= "DISPLAY";
code char DISPCmdTxt[] 					= "DISP";
code char INPUTCmdTxt[] 				= "INPUT";
code char INPCmdTxt[] 					= "INP";
code char OUTPUTCmdTxt[] 				= "OUTPUT";
code char OUTPCmdTxt[] 					= "OUTP";
code char FACTORYCmdTxt[] 				= "FACTORY";
code char FACTCmdTxt[] 					= "FACT";

code char ERRORCmdTxt[] 				= "ERROR";
code char ERRCmdTxt[] 					= "ERR";
code char VERSIONCmdTxt[] 				= "VERSION";
code char VERSCmdTxt[] 					= "VERS";
code char DOWNLOADCmdTxt[] 			= "DOWNLOAD";
code char DOWNCmdTxt[] 					= "DOWN";
code char UPLOADCmdTxt[] 				= "UPLOAD";
code char UPLCmdTxt[] 					= "UPL";
code char NAMECmdTxt[] 					= "NAME";
code char NAMCmdTxt[] 					= "NAM";
code char RECALLCmdTxt[] 				= "RECALL";
code char RECCmdTxt[] 					= "REC";

code char OPERATIONCmdTxt[] 			= "OPERATION";
code char OPERCmdTxt[] 					= "OPER";
code char QUESTIONABLECmdTxt[] 		= "QUESTIONABLE";
code char QUESCmdTxt[]			 		= "QUES";
code char PRESETCmdTxt[] 				= "PRESET";
code char PRESCmdTxt[] 					= "PRES";

code char EVENTCmdTxt[] 				= "EVENT";
code char EVENCmdTxt[] 					= "EVEN";
code char CONDITIONCmdTxt[] 			= "CONDITION";
code char CONDCmdTxt[] 					= "COND";
code char ENABLECmdTxt[] 				= "ENABLE";
code char ENABCmdTxt[] 					= "ENAB";

code char CONTRASTCmdTxt[] 			= "CONTRAST";
code char CONTCmdTxt[] 					= "CONT";

code char GENLOCKCmdTxt[] 				= "GENLOCK";
code char GENLCmdTxt[]			 		= "GENL";

code char BBCmdTxt[] 					= "BB";
code char BBMULTICmdTxt[] 				= "BBMULTI";
code char BBMCmdTxt[] 					= "BBM";
code char DELAYCmdTxt[] 				= "DELAY";
code char DELCmdTxt[] 					= "DEL";
code char SCHPHASECmdTxt[] 			= "SCHPHASE";
code char SCHPCmdTxt[] 					= "SCHP";

code char ATPGENERATORCmdTxt[] 		= "ATPGENERATOR";
code char ATPGCmdTxt[] 					= "ATPG";
code char PATTERNCmdTxt[] 				= "PATTERN";
code char PATTCmdTxt[] 					= "PATT";
code char TEXTCmdTxt[] 					= "TEXT";
code char STRINGCmdTxt[] 				= "STRING";
code char STRCmdTxt[] 					= "STR";
code char STYLECmdTxt[] 				= "STYLE";
code char STYLCmdTxt[] 					= "STYL";
code char POSITIONCmdTxt[] 			= "POSITION";
code char POSCmdTxt[] 					= "POS";
code char CLOCKCmdTxt[] 				= "CLOCK";
code char CLOCCmdTxt[] 					= "CLOC";

code char STSGENERATORCmdTxt[] 		= "STSGENERATOR";
code char STSGCmdTxt[] 					= "STSG";
code char EDHINSERTCmdTxt[] 			= "EDHINSERT";
code char EDHCmdTxt[] 					= "EDH";
code char EMBAUDIOCmdTxt[] 			= "EMBAUDIO";
code char EMBCmdTxt[] 					= "EMB";

code char STPGENERATORCmdTxt[] 		= "STPGENERATOR";
code char STPGCmdTxt[] 					= "STPG";
code char MODIFYCmdTxt[] 				= "MODIFY";
code char MODCmdTxt[] 					= "MOD";
code char APALCmdTxt[] 					= "APAL";			// Anti-PAL
code char STAIRCASECmdTxt[] 			= "STAIRCASE";		// Staircase 10step
code char STACmdTxt[] 					= "STA";
code char MOTIONCmdTxt[] 				= "MOTION";			// Motion
code char MOTCmdTxt[] 					= "MOT";
code char CIRCLESCmdTxt[] 				= "CIRCLES";		// Circles
code char CIRCCmdTxt[] 					= "CIRC";
code char LOGOCmdTxt[] 					= "LOGO";			// Logo
code char LOGCmdTxt[] 					= "LOG";

code char AUDIOCmdTxt[] 				= "AUDIO";
code char AUDCmdTxt[] 					= "AUD";
code char SIGNALCmdTxt[] 				= "SIGNAL";
code char SIGNCmdTxt[] 					= "SIGN";
code char LEVELCmdTxt[] 				= "LEVEL";
code char LEVCmdTxt[] 					= "LEV";
code char GROUPCmdTxt[] 				= "GROUP";
code char GROCmdTxt[] 					= "GRO";
code char TIMINGCmdTxt[] 				= "TIMING";
code char TIMCmdTxt[] 					= "TIM";

code char TIMECLOCKCmdTxt[] 			= "TIMECLOCK";
code char DFORMATCmdTxt[] 				= "DFORMAT";
code char DFORCmdTxt[] 					= "DFOR";
code char DATECmdTxt[] 					= "DATE";
code char DATCmdTxt[] 					= "DAT";
code char TFORMATCmdTxt[] 				= "TFORMAT";
code char TFORCmdTxt[] 					= "TFOR";
code char TIMECmdTxt[] 					= "TIME";
code char REFERENCECmdTxt[] 			= "REFERENCE";
code char REFCmdTxt[] 					= "REF";
code char OFFSETCmdTxt[] 				= "OFFSET";
code char OFFSCmdTxt[] 					= "OFFS";

code char PASSWORDCmdTxt[] 			= "PASSWORD";
code char PASSCmdTxt[] 					= "PASS";
code char V24CIRCUITCmdTxt[] 			= "V24CIRCUIT";
code char V24CCmdTxt[] 					= "V24C";
code char ADDRESSCmdTxt[] 				= "ADDRESS";
code char ADDRCmdTxt[] 					= "ADDR";
code char COMMANDCmdTxt[] 				= "COMMAND";
code char COMMCmdTxt[] 					= "COMM";

code char SPGGENERATORCmdTxt[] 		= "SPGGENERATOR";
code char SPGGCmdTxt[] 					= "SPGG";
code char TEXTSTRINGCmdTxt[] 			= "TEXTSTRING";
code char STORECmdTxt[] 				= "STORE";
code char STORCmdTxt[] 					= "STOR";
code char READCmdTxt[] 					= "READ";
code char WRITECmdTxt[] 				= "WRITE";
code char WRITCmdTxt[] 					= "WRIT";
code char SDIGENLOCKCmdTxt[] 			= "SDIGENLOCK";
code char SDIGCmdTxt[] 					= "SDIG";
code char MAINBOARDCmdTxt[] 			= "MAINBOARD";
code char MAINCmdTxt[] 					= "MAIN";
code char PSN5CmdTxt[] 					= "PSN";
code char PS12CmdTxt[] 					= "PS";
code char PSCALIBCmdTxt[] 				= "PSCALIB";
code char PSCCmdTxt[] 					= "PSC";
code char MULTIBBCmdTxt[] 				= "MULTIBB";
code char MULTCmdTxt[] 					= "MULT";
code char ADCCmdTxt[] 					= "ADC";
code char LDETECTORCmdTxt[] 			= "LDETECTOR";
code char LDETCmdTxt[] 					= "LDET";
code char MODECmdTxt[]					= "MODE";

code char AParTxt[] 						= "A";
code char BParTxt[] 						= "B";
code char ABParTxt[] 					= "A_B";
code char SDIParTxt[] 					= "SDI";
code char INTERNALParTxt[] 			= "INTERNAL";
code char INTParTxt[] 					= "INT";
code char INTERNAL2ParTxt[] 			= "INTERNAL2";
code char INT2ParTxt[] 					= "INT2";

code char PALBURSTParTxt[] 			= "PALBURST";
code char PALBParTxt[] 					= "PALB";
code char NTSCBURSTParTxt[] 			= "NTSCBURST";
code char NTSCParTxt[] 					= "NTSC";
code char SYNC625ParTxt[] 				= "SYNC625";
code char SYNC525ParTxt[] 				= "SYNC525";
code char SDI625ParTxt[] 				= "SDI625";
code char SDI525ParTxt[] 				= "SDI525";
code char F358MHzParTxt[] 				= "F358MHZ";
code char F443MHzParTxt[] 				= "F443MHZ";
code char F5MHzParTxt[] 				= "F5MHZ";
code char F10MHzParTxt[] 				= "F10MHZ";

code char PALParTxt[] 					= "PAL";
code char PAL_IDParTxt[] 				= "PAL_ID";

code char CBSMPTEParTxt[] 				= "CBSMPTE";		// SMPTE Colorbar
code char CBSMParTxt[] 					= "CBSM";
code char CBEBUParTxt[] 				= "CBEBU";        // EBU Colorbar
code char CBEBParTxt[] 					= "CBEB";
code char CBFCCParTxt[] 				= "CBFCC";			// FCC Colorbar
code char CBFCParTxt[] 					= "CBFC";
code char CBEBU8ParTxt[] 				= "CBEBU8";			// Colorbar ITU801
code char CBEB8ParTxt[] 				= "CBEB8";
code char CB100ParTxt[]		 			= "CB100";			// 100% Colorbar
code char CBGREY75ParTxt[]				= "CBGREY75";		// Colorbar + 75% Grey
code char CBGR75ParTxt[]				= "CBGR75";
code char CBRED75ParTxt[] 				= "CBRED75";		// Colorbar + 75% Red
code char CBR75ParTxt[] 				= "CBR75";
code char RED75ParTxt[] 				= "RED75";			// 75% Red

code char MULTIBURSTParTxt[] 			= "MULTIBURST";	// Multi burst
code char MULTParTxt[] 					= "MULT";
code char LSWEEPParTxt[] 				= "LSWEEP";			// Luminance Sweep
code char LSWParTxt[] 					= "LSW";
code char YCRCBSWEEPParTxt[] 			= "YCRCBSWEEP";   // Y,Cr,Cb Sweep
code char YCRCParTxt[] 					= "YCRC";
code char MPULSEParTxt[] 				= "MPULSE";			// Multi pulse
code char MPULParTxt[] 					= "MPUL";
code char SINXXParTxt[] 				= "SINXX";        // Sinx/x
code char SINXParTxt[] 					= "SINX";
code char CCIR18ParTxt[] 				= "CCIR18";			// CCIR 18
code char NCMBParTxt[] 					= "NCMB";			// NTC7 Combination
code char FCCMBURSTParTxt[] 			= "FCCMBURST";		// FCC Multiburst
code char FCCMParTxt[] 					= "FCCM";

code char WIN10ParTxt[] 				= "WIN10";			// Window 10%
code char WIN15ParTxt[] 				= "WIN15";			// Window 15%
code char WIN20ParTxt[] 				= "WIN20";			// Window 20%
code char WIN100ParTxt[] 				= "WIN100";			// Window 100%
code char BLWH15ParTxt[] 				= "BLWH15KHZ";		// 15kHz Bl/Wh
code char GREY50ParTxt[] 				= "GREY50";			// Grey 50%
code char WHITE100ParTxt[] 			= "WHITE100";		// White 100%
code char WHIT100ParTxt[] 				= "WHIT100";
code char BLACKBURSTParTxt[] 			= "BLACKBURST";	// Black burst
code char BLACKParTxt[] 				= "BLACK";			// Black
code char BLACParTxt[]		 			= "BLAC";

code char SDICHECKParTxt[] 			= "SDICHECK";		// Check field
code char SDICParTxt[] 					= "SDIC";
code char DTIMINGParTxt[] 				= "DTIMING";      // Digital Timing
code char DTIMParTxt[] 					= "DTIM";
code char FDTESTParTxt[] 				= "FDTEST";			// Field Delay Test
code char FDTParTxt[] 					= "FDT";
code char BOWTIEParTxt[] 				= "BOWTIE";			// Bow Tie
code char BOWTParTxt[] 					= "BOWT";
code char ABLANKINGParTxt[] 			= "ABLANKING";		// Analog Blanking
code char ABLParTxt[]				 	= "ABL";
code char DGREYParTxt[] 				= "DGREY";			// Digital grey
code char DGRParTxt[] 					= "DGR";
code char FSWAVEParTxt[] 				= "FSWAVE";			// Field Sqare Wave
code char FSWParTxt[] 					= "FSW";
code char BLWH01ParTxt[] 				= "BLWH01";			// 0.1Hz Bl/Wh
code char CUSTOM1ParTxt[] 				= "CUSTOM1";		// Customized 1
code char CUST1ParTxt[] 				= "CUST1";			// Customized 1
code char CUSTOM2ParTxt[] 				= "CUSTOM2";		// Customized 2
code char CUST2ParTxt[] 				= "CUST2";
code char CUSTOM3ParTxt[] 				= "CUSTOM3";		// Customized 3
code char CUST3ParTxt[] 				= "CUST3";
code char CUSTOM4ParTxt[] 				= "CUSTOM4";		// Customized 4
code char CUST4ParTxt[] 				= "CUST4";
code char CUSTOM5ParTxt[] 				= "CUSTOM5";		// Customized 5
code char CUST5ParTxt[] 				= "CUST5";
code char CUSTOM6ParTxt[] 				= "CUSTOM6";		// Customized 6
code char CUST6ParTxt[] 				= "CUST6";
code char CUSTOM7ParTxt[] 				= "CUSTOM7";		// Customized 7
code char CUST7ParTxt[] 				= "CUST7";
code char CUSTOM8ParTxt[] 				= "CUSTOM8";		// Customized 8
code char CUST8ParTxt[] 				= "CUST8";
code char EOLINEParTxt[] 				= "EOLINE";			// End-of-line pulses
code char EOLParTxt[] 					= "EOL";
code char WEOLINEParTxt[] 				= "WEOLINE";		// White, end-of-line porches
code char WEOLParTxt[] 					= "WEOL";
code char BEOLINEParTxt[] 				= "BEOLINE";		// Blue, do.
code char BEOLParTxt[] 					= "BEOL";
code char REOLINEParTxt[] 				= "REOLINE";		// Red, do.
code char REOLParTxt[] 					= "REOL";
code char YEOLINEParTxt[] 				= "YEOLINE";		// Yellow, do.
code char YEOLParTxt[] 					= "YEOL";
code char CEOLINEParTxt[]				= "CEOLINE";		// Cyan, do.
code char CEOLParTxt[]					= "CEOL";

code char SHALLOWRAMParTxt[] 			= "SHALLOWRAMP";	// Shallow Ramp
code char SHALParTxt[] 					= "SHAL";
code char SRAMPParTxt[] 				= "SRAMP";			// Shallow Ramp
code char SRAMParTxt[] 					= "SRAM";
code char RAMPParTxt[] 					= "RAMP";			// Ramp, (luminance)
code char LRAMPParTxt[] 				= "LRAMP";			// Limit Ramp
code char LRAMParTxt[] 					= "LRAM";
code char VRAMPParTxt[] 				= "VRAMP";			// Valid Ramp
code char VRAMParTxt[] 					= "VRAM";
code char MRAMPParTxt[] 				= "MRAMP";			// Modulated Ramp
code char MRAMParTxt[] 					= "MRAM";
code char STAIRCASE5ParTxt[]			= "STAIRCASE5";	// 5-step staircase
code char STA5ParTxt[] 					= "STA5";
code char MSTAIRCASE5ParTxt[]			= "MSTAIRCASE5";	// Modulated 5-step staircase
code char MSTA5ParTxt[] 				= "MSTA5";
code char STAIRCASE10ParTxt[] 		= "STAIRCASE10";	// 10-step staircase
code char STA10ParTxt[] 				= "STA10";
code char PBARParTxt[] 					= "PBAR";			// Pulse & Bar
code char CCIR17ParTxt[] 				= "CCIR17";			// CCIR line 17
code char CCIR330ParTxt[] 				= "CCIR330";		// CCIR line 330
code char CCIR331ParTxt[] 				= "CCIR331";		// CCIR line 331
code char FCCCompositeParTxt[] 		= "FCCCOMPOSITE";	// FCC Composite
code char FCCCParTxt[] 					= "FCCC";
code char NCMPParTxt[] 					= "NCMP";			// NTC-7 Composite
code char YGRAMPParTxt[] 				= "YGRAMP";			// Yellow/Grey Ramp
code char YGRParTxt[] 					= "YGR";
code char GBRAMPParTxt[] 				= "GBRAMP";			// Grey/Blue Ramp
code char GBRParTxt[] 					= "GBR";
code char CGRAMPParTxt[] 				= "CGRAMP";			// Cyan/Grey Ramp
code char CGRParTxt[] 					= "CGR";
code char GRRAMPParTxt[] 				= "GRRAMP";			// Grey/Red Ramp
code char GRRParTxt[] 					= "GRR";
code char CBYCRYRAMPParTxt[] 			= "CBYCRYRAMP";	// Cb,Y,Cr,Y Ramp
code char CBYCParTxt[] 					= "CBYC";

code char PHILIPS43ParTxt[] 			= "PHILIPS43";		// Philips 4:3
code char PHIL43ParTxt[] 				= "PHIL43";
code char PHILIPS169ParTxt[] 			= "PHILIPS169";	// Philips 16:9
code char PHIL169ParTxt[] 				= "PHIL169";
code char FUBK43ParTxt[] 				= "FUBK43";			// FuBK 4:3
code char FUBK169ParTxt[] 				= "FUBK169";		// FuBK 16:9
code char CROSSHATCHParTxt[] 			= "CROSSHATCH";	// Cross hatch
code char CROSParTxt[] 					= "CROS";
code char CROSSHATCH169ParTxt[] 		= "CROSSHATCH169";// Cross hatch 16:9
code char CROS169ParTxt[] 				= "CROS169";
code char CIRCLE43ParTxt[] 			= "CIRCLE43";		// Circle 4:3
code char CIRC43ParTxt[] 				= "CIRC43";			//
code char CIRCLE169ParTxt[] 			= "CIRCLE169";		// Circle 16:9
code char CIRC169ParTxt[] 				= "CIRC169";		// 
code char PLUGEParTxt[] 				= "PLUGE";			// PLUGE
code char PLUGParTxt[] 					= "PLUG";
code char SAFEAREAParTxt[] 			= "SAFEAREA";		// Safe area
code char SAFParTxt[] 					= "SAF";
code char SWAVE250KHZParTxt[] 		= "SWAVE250KHZ";	// 250kHz
code char SWAV250KHZParTxt[] 			= "SWAV250KHZ";
code char VMT01ParTxt[] 				= "VMT01";			// VMT 01

code char S800HZParTxt[] 				= "S800HZ";			// Stereo 800 Hz
code char S1KHZParTxt[] 				= "S1KHZ";   		// Stereo 1 kHz
code char SEBU1KHZParTxt[] 			= "SEBU1KHZ";		// Stereo EBU 1 kHz
code char SBBC1KHZParTxt[]			 	= "SBBC1KHZ";		// Stereo BBC 1 kHz
code char M1KHZParTxt[] 				= "M1KHZ";			// Mono 1 kHz
code char MEBU1KHZParTxt[] 			= "MEBU1KHZ";		// Mono EBU 1 kHz
code char DUALParTxt[] 					= "DUAL";			// Dual sound
code char WORDCLOCKParTxt[] 			= "WORDCLOCK";		// WordClock (48kHz)
code char WORDParTxt[] 					= "WORD";

code char SILENCEParTxt[] 				= "SILENCE";
code char SILParTxt[] 					= "SIL";

code char DB0FSParTxt[] 				= "DB0FS";
code char DB9FSParTxt[] 				= "DB9FS";
code char DB12FSParTxt[]			 	= "DB12FS";
code char DB14FSParTxt[] 				= "DB14FS";
code char DB15FSParTxt[] 				= "DB15FS";
code char DB16FSParTxt[] 				= "DB16FS";
code char DB18FSParTxt[] 				= "DB18FS";
code char DB20FSParTxt[] 				= "DB20FS";

code char NTSC1ParTxt[] 				= "NTSC1";
code char NTSC2ParTxt[] 				= "NTSC2";
code char NTSC3ParTxt[] 				= "NTSC3";
code char NTSC4ParTxt[]			 		= "NTSC4";
code char NTSC5ParTxt[]		 			= "NTSC5";

code char GROUP1ParTxt[] 				= "GROUP1";
code char GRO1ParTxt[] 					= "GRO1";
code char GROUP2ParTxt[] 				= "GROUP2";
code char GRO2ParTxt[] 					= "GRO2";
code char GROUP3ParTxt[] 				= "GROUP3";
code char GRO3ParTxt[] 					= "GRO3";
code char GROUP4ParTxt[]			 	= "GROUP4";
code char GRO4ParTxt[]				 	= "GRO4";

code char DMYParTxt[]					= "DMY";
code char MDYParTxt[]					= "MDY";
code char YMDParTxt[]					= "YMD";

code char H12ParTxt[]					= "HOUR12";
code char H24ParTxt[]					= "HOUR24";
code char VITCParTxt[]					= "VITC";
code char LTCParTxt[]					= "LTC";
code char VFFREQUENCYParTxt[]			= "VFFREQUENCY";
code char VFFRParTxt[]					= "VFFR";
code char REF1HZParTxt[]				= "REF1HZ";

code char BBU_0ParTxt[] 				= "BBU_0";
code char BBU_1ParTxt[] 				= "BBU_1";
code char BBU_2ParTxt[] 				= "BBU_2";
code char BBU_3ParTxt[] 				= "BBU_3";
code char SPGParTxt[] 					= "SPG";
code char ANLSIGNALParTxt[] 			= "ANLSIGNAL";
code char ANLSParTxt[] 					= "ANLS";
code char SDISIGNALParTxt[] 			= "SDISIGNAL";
code char SDISParTxt[] 					= "SDIS";
code char SDITPGParTxt[]  				= "SDITPG";
code char SDITParTxt[] 					= "SDIT";

code char OFFParTxt[] 					= "OFF";
code char ONParTxt[] 					= "ON";

code char MINParTxt[] 					= "MIN";
code char MAXParTxt[] 					= "MAX";

code char FREEParTxt[] 					= "FREE";
code char STANDARDParTxt[] 			= "STANDARD";
code char STANParTxt[] 					= "STAN";
code char COMPLEXParTxt[] 				= "COMPLEX";
code char COMPParTxt[] 					= "COMP";

code char PT5210ParTxt[] 				= "PT5210";
code char PT5230ParTxt[] 				= "PT5230";

code char TIMEParTxt[] 					= "TIME";
code char TIMParTxt[] 					= "TIM";
code char DATEParTxt[] 					= "DATE";
code char DATParTxt[] 					= "DAT";
code char DTIMEParTxt[] 				= "DTIME";

code struct CMD_STRUCT Response[1] = {
	{ IDNResponseCmdTxt, NULL, NULL, NULL, NONE, IDNResponseMessage},
};

code struct CMD_STRUCT Mandated[10] = {
	{ CLSCmdTxt, NULL, &Mandated[1], NULL, NONE, CLSCommand},
	{ ESECmdTxt, NULL, &Mandated[2], NULL, NONE, ESECommand},
	{ ESRCmdTxt, NULL, &Mandated[3], NULL, NONE, ESRCommand},
	{ IDNCmdTxt, NULL, &Mandated[4], NULL, NONE, IDNCommand},
	{ OPCCmdTxt, NULL, &Mandated[5], NULL, NONE, OPCCommand},
	{ RSTCmdTxt, NULL, &Mandated[6], NULL, NONE, RSTCommand},
	{ SRECmdTxt, NULL, &Mandated[7], NULL, NONE, SRECommand},
	{ STBCmdTxt, NULL, &Mandated[8], NULL, NONE, STBCommand},
	{ TSTCmdTxt, NULL, &Mandated[9], NULL, NONE, TSTCommand},
	{ WAICmdTxt, NULL, NULL, NULL, NONE, WAICommand},
};

code struct CMD_STRUCT Subsystem[7] = {
	{ SYSTEMCmdTxt, SYSTCmdTxt, &Subsystem[1], &System[0], NONE, NONE},
	{ STATUSCmdTxt, STATCmdTxt, &Subsystem[2], &Status[0], NONE, NONE},
	{ DIAGNOSTICCmdTxt, DIAGCmdTxt, &Subsystem[3], &Diagnostic[0], NONE, NONE},
	{ DISPLAYCmdTxt, DISPCmdTxt, &Subsystem[4], &Display[0], NONE, NONE},
	{ INPUTCmdTxt, INPCmdTxt, &Subsystem[5], &Input[0], NONE, NONE},
	{ OUTPUTCmdTxt, OUTPCmdTxt, &Subsystem[6], &Output[0], NONE, NONE},
	{ FACTORYCmdTxt, FACTCmdTxt, NULL, &Factory[0], NONE, NONE},
};

//**************************************************************************
code struct CMD_STRUCT System[5] = {
	{ ERRORCmdTxt, ERRCmdTxt, &System[1], NULL, NONE, SystemError},
	{ VERSIONCmdTxt, VERSCmdTxt, &System[2], NULL, NONE, SystemVersion},
	{ PRESETCmdTxt, PRESCmdTxt, &System[3], &SystemLevel1[0], NONE, SystemPreset},
	{ DOWNLOADCmdTxt, DOWNCmdTxt, &System[4], NULL, NONE, SystemDownload},
	{ UPLOADCmdTxt, UPLCmdTxt, NULL, NULL, NONE, SystemUpload}
};

//**************************************************************************
code struct CMD_STRUCT SystemLevel1[5] = {
	{ RECALLCmdTxt, RECCmdTxt, &SystemLevel1[1], NULL, NONE, SystemPreset},
	{ STORECmdTxt, STORCmdTxt, &SystemLevel1[2], NULL, NONE, SystemPresetStore},
	{ NAMECmdTxt, NAMCmdTxt, &SystemLevel1[3], NULL, NONE, SystemPresetName},
	{ DOWNLOADCmdTxt, DOWNCmdTxt, &SystemLevel1[4], NULL, NONE, SystemPresetDownload},
	{ UPLOADCmdTxt, UPLCmdTxt, NULL, NULL, NONE, SystemPresetUpload}
};

//**************************************************************************
code struct CMD_STRUCT Status[4] = {
	{ OPERATIONCmdTxt, OPERCmdTxt, &Status[1], &StatusLevel1_1[0], NONE, StatusOperEvent},
	{ QUESTIONABLECmdTxt, QUESCmdTxt, &Status[2], &StatusLevel1_2[0], NONE, StatusQuestEvent},
	{ PRESETCmdTxt, PRESCmdTxt, &Status[3], NULL, NONE, StatusPreset},
	{ PTCmdTxt, PTCmdTxt, NULL, NULL, StatusPTSuffix, StatusPT},
};

code struct CMD_STRUCT StatusLevel1_1[3] = {
	{ EVENTCmdTxt, EVENCmdTxt, &StatusLevel1_1[1], NULL, NONE, StatusOperEvent},
	{ CONDITIONCmdTxt, CONDCmdTxt, &StatusLevel1_1[2], NULL, NONE, StatusOperCondition},
	{ ENABLECmdTxt, ENABCmdTxt, NULL, NULL, NONE, StatusOperEnable},
};

code struct CMD_STRUCT StatusLevel1_2[3] = {
	{ EVENTCmdTxt, EVENCmdTxt, &StatusLevel1_2[1], NULL, NONE, StatusQuestEvent},
	{ CONDITIONCmdTxt, CONDCmdTxt, &StatusLevel1_2[2], NULL, NONE, StatusQuestCondition},
	{ ENABLECmdTxt, ENABCmdTxt, NULL, NULL, NONE, StatusQuestEnable},
};

//**************************************************************************
code struct CMD_STRUCT Diagnostic[2] = {
	{ DISPLAYCmdTxt, DISPCmdTxt, &Diagnostic[1], NULL, NONE, DiagnosticDisplay},
	{ ERRORQUEUECmdTxt, ERRCmdTxt, NULL, &DiagnosticLevel1[0], NONE, DiagnosticErrorQueueLast},
};

//**************************************************************************
code struct CMD_STRUCT DiagnosticLevel1[3] = {
	{ LASTCmdTxt, LASTCmdTxt, &DiagnosticLevel1[1], NULL, NONE, DiagnosticErrorQueueLast},
	{ ENTRYCmdTxt, ENTRCmdTxt, &DiagnosticLevel1[2], NULL, NONE, DiagnosticErrorQueueEntry},
	{ RESETCmdTxt, RESCmdTxt, NULL, NULL, NONE, DiagnosticErrorQueueReset},
};

//**************************************************************************
code struct CMD_STRUCT Display[1] = {
	{ CONTRASTCmdTxt, CONTCmdTxt, NULL, NULL, NONE, DisplayContrast},
};

//**************************************************************************
code struct CMD_STRUCT Input[2] = {
	{ GENLOCKCmdTxt, GENLCmdTxt, &Input[1], &GenlockLevel1[0], NONE, Genlock},
	{ SDIGENLOCKCmdTxt, SDIGCmdTxt, NULL, &GenlockLevel2[0], NONE, NONE}
};

//**************************************************************************
code struct CMD_STRUCT GenlockLevel1[3] = {
	{ INPUTCmdTxt, INPCmdTxt, &GenlockLevel1[1], NULL, NONE, GenlockInput},
	{ SYSTEMCmdTxt, SYSTCmdTxt, &GenlockLevel1[2], NULL, NONE, GenlockSystem},
	{ DELAYCmdTxt, DELCmdTxt, NULL, NULL, NONE, GenlockDelay},
};

code struct CMD_STRUCT GenlockLevel2[1] = {
	{ VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, SDIGenlockVersion}
};

//**************************************************************************
code struct CMD_STRUCT Output[7] = {
	{ BBCmdTxt, BBCmdTxt, &Output[1], &BBLevel1[0], BBSuffix, BB},
	{ ATPGENERATORCmdTxt, ATPGCmdTxt, &Output[2], &AnlTPGLevel1[0], AnlTPGSuffix, AnlTPG},
	{ STSGENERATORCmdTxt, STSGCmdTxt, &Output[3], &SDITSGLevel1[0], SDITSGSuffix, SDITSG},
	{ STPGENERATORCmdTxt, STPGCmdTxt, &Output[4], &SDITPGLevel1[0], SDITPGSuffix, SDITPG},
	{ AUDIOCmdTxt, AUDCmdTxt, &Output[5], &AESEBULevel1[0], AESEBUSuffix, AESEBU},
	{ TIMECLOCKCmdTxt, TIMCmdTxt, &Output[6], &TimeClockLevel1[0], NONE, TimeClock},
	{ BBMULTICmdTxt, BBMCmdTxt, NULL, &BBMultiLevel1[0], NONE, NONE},
};

//**************************************************************************
code struct CMD_STRUCT BBLevel1[4] = {
	{ SYSTEMCmdTxt, SYSTCmdTxt, &BBLevel1[1], NULL, NONE, BBSystem},
	{ DELAYCmdTxt, DELCmdTxt, &BBLevel1[2], NULL, NONE, BBDelay},
	{ SCHPHASECmdTxt, SCHPCmdTxt, &BBLevel1[3], NULL, NONE, BBScHPhase},
	{ VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, BBVersion}
};

//**************************************************************************
code struct CMD_STRUCT AnlTPGLevel1[6] = {
	{ PATTERNCmdTxt, PATTCmdTxt, &AnlTPGLevel1[1], &AnlTPGLevel2[0], NONE, AnlTPGPattern},
	{ TEXTCmdTxt, TEXTCmdTxt, &AnlTPGLevel1[2], &AnlTPGLevel4[0], NONE, NONE},
	{ SYSTEMCmdTxt, SYSTCmdTxt, &AnlTPGLevel1[3], NULL, NONE, AnlTPGSystem},
	{ DELAYCmdTxt, DELCmdTxt, &AnlTPGLevel1[4], NULL, NONE, AnlTPGDelay},
	{ SCHPHASECmdTxt, SCHPCmdTxt, &AnlTPGLevel1[5], NULL, NONE, AnlTPGScHPhase},
	{ VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, AnlTPGVersion}
};

code struct CMD_STRUCT AnlTPGLevel2[1] = {
	{ MODIFYCmdTxt, MODCmdTxt, NULL, &AnlTPGLevel3[0], NONE, NONE},
};

code struct CMD_STRUCT AnlTPGLevel3[4] = {
	{ APALCmdTxt, APALCmdTxt, &AnlTPGLevel3[1], NULL, NONE, AnlTPGAttribAPAL},
	{ PLUGEParTxt, PLUGParTxt, &AnlTPGLevel3[2], NULL, NONE, AnlTPGAttribPLUGE},
	{ STAIRCASECmdTxt, STACmdTxt, &AnlTPGLevel3[3], NULL, AnlTPGAttribSuffix, AnlTPGAttribStair10},
	{ CIRCLESCmdTxt, CIRCCmdTxt, NULL, NULL, NONE, AnlTPGAttribCircles},
};

code struct CMD_STRUCT AnlTPGLevel4[4] = {
	{ STRINGCmdTxt, STRCmdTxt, &AnlTPGLevel4[1], NULL, AnlTPGTextStringSuffix, AnlTPGTextString},
	{ STYLECmdTxt, STYLCmdTxt, &AnlTPGLevel4[2], NULL, NONE, AnlTPGTextStyle},
	{ POSITIONCmdTxt, POSCmdTxt, &AnlTPGLevel4[3], NULL, NONE, AnlTPGTextPosition},
	{ CLOCKCmdTxt, CLOCCmdTxt, NULL, NULL, NONE, AnlTPGTextClock},
};

//**************************************************************************
code struct CMD_STRUCT SDITSGLevel1[6] = {
	{ PATTERNCmdTxt, PATTCmdTxt, &SDITSGLevel1[1], NULL, NONE, SDITSGPattern},
	{ SYSTEMCmdTxt, SYSTCmdTxt, &SDITSGLevel1[2], NULL, NONE, SDITSGSystem},
	{ DELAYCmdTxt, DELCmdTxt, &SDITSGLevel1[3], NULL, NONE, SDITSGDelay},
	{ EDHINSERTCmdTxt, EDHCmdTxt, &SDITSGLevel1[4], NULL, NONE, SDITSGEDHInsert},
	{ EMBAUDIOCmdTxt, EMBCmdTxt, &SDITSGLevel1[5], &SDITSGLevel2[0], NONE, NONE},
	{ VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, SDITSGVersion}
};

code struct CMD_STRUCT SDITSGLevel2[2] = {
	{ SIGNALCmdTxt, SIGNCmdTxt, &SDITSGLevel2[1], NULL, NONE, SDITSGAudioSignal},
	{ LEVELCmdTxt, LEVCmdTxt, NULL, NULL, NONE, SDITSGAudioLevel},
};

//**************************************************************************
code struct CMD_STRUCT SDITPGLevel1[7] = {
	{ PATTERNCmdTxt, PATTCmdTxt, &SDITPGLevel1[1], &SDITPGLevel2[0], NONE, SDITPGPattern},
	{ TEXTCmdTxt, TEXTCmdTxt, &SDITPGLevel1[2], &SDITPGLevel4[0], NONE, NONE},
	{ SYSTEMCmdTxt, SYSTCmdTxt, &SDITPGLevel1[3], NULL, NONE, SDITPGSystem},
	{ EDHINSERTCmdTxt, EDHCmdTxt, &SDITPGLevel1[4], NULL, NONE, SDITPGEDHInsert},
	{ EMBAUDIOCmdTxt, EMBCmdTxt, &SDITPGLevel1[5], &SDITPGLevel5[0], NONE, NONE},
	{ DELAYCmdTxt, DELCmdTxt, &SDITPGLevel1[6], NULL, NONE, SDITPGDelay},
	{ VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, SDITPGVersion}
};

code struct CMD_STRUCT SDITPGLevel2[1] = {
	{ MODIFYCmdTxt, MODCmdTxt, NULL, &SDITPGLevel3[0], NONE, NONE},
};

code struct CMD_STRUCT SDITPGLevel3[6] = {
	{ APALCmdTxt, APALCmdTxt, &SDITPGLevel3[1], NULL, NONE, SDITPGAttribAPAL},
	{ PLUGEParTxt, PLUGParTxt, &SDITPGLevel3[2], NULL, NONE, SDITPGAttribPLUGE},
	{ STAIRCASECmdTxt, STACmdTxt, &SDITPGLevel3[3], NULL, SDITPGAttribSuffix, SDITPGAttribStair10},
	{ MOTIONCmdTxt, MOTCmdTxt, &SDITPGLevel3[4], NULL, NONE, SDITPGAttribMotion},
	{ CIRCLESCmdTxt, CIRCCmdTxt, &SDITPGLevel3[5], NULL, NONE, SDITPGAttribCircles},
	{ LOGOCmdTxt, LOGCmdTxt, NULL, NULL, NONE, SDITPGAttribLogo},
};

code struct CMD_STRUCT SDITPGLevel4[4] = {
	{ STRINGCmdTxt, STRCmdTxt, &SDITPGLevel4[1], NULL, SDITPGTextStringSuffix, SDITPGTextString},
	{ STYLECmdTxt, STYLCmdTxt, &SDITPGLevel4[2], NULL, NONE, SDITPGTextStyle},
	{ POSITIONCmdTxt, POSCmdTxt, &SDITPGLevel4[3], NULL, NONE, SDITPGTextPosition},
	{ CLOCKCmdTxt, CLOCCmdTxt, NULL, NULL, NONE, SDITPGTextClock},
};

code struct CMD_STRUCT SDITPGLevel5[3] = {
	{ SIGNALCmdTxt, SIGNCmdTxt, &SDITPGLevel5[1], NULL, NONE, SDITPGAudioSignal},
	{ LEVELCmdTxt, LEVCmdTxt, &SDITPGLevel5[2], NULL, NONE, SDITPGAudioLevel},
	{ GROUPCmdTxt, GROCmdTxt, NULL, NULL, NONE, SDITPGAudioGroup},
};

//**************************************************************************
code struct CMD_STRUCT AESEBULevel1[4] = {
	{ SIGNALCmdTxt, SIGNCmdTxt, &AESEBULevel1[1], NULL, NONE, AESEBUSignal},
	{ LEVELCmdTxt, LEVCmdTxt, &AESEBULevel1[2], NULL, NONE, AESEBULevel},
	{ TIMINGCmdTxt, TIMCmdTxt, &AESEBULevel1[3], NULL, NONE, AESEBUTiming},
	{ VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, AESEBUVersion}
};

//**************************************************************************
code struct CMD_STRUCT TimeClockLevel1[7] = {
	{ DFORMATCmdTxt, DFORCmdTxt, &TimeClockLevel1[1], NULL, NONE, TimeClockDateFormat},
	{ DATECmdTxt, DATCmdTxt, &TimeClockLevel1[2], NULL, NONE, TimeClockDate},
	{ TFORMATCmdTxt, TFORCmdTxt, &TimeClockLevel1[3], NULL, NONE, TimeClockTimeFormat},
	{ TIMECmdTxt, TIMCmdTxt, &TimeClockLevel1[4], NULL, NONE, TimeClockTime},
	{ REFERENCECmdTxt, REFCmdTxt, &TimeClockLevel1[5], NULL, NONE, TimeClockReference},
	{ OFFSETCmdTxt, OFFSCmdTxt, &TimeClockLevel1[6], NULL, NONE, TimeClockOffset},
	{ VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, TimeClockVersion}
};

//**************************************************************************
code struct CMD_STRUCT BBMultiLevel1[1] = {
	{ VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, BBMultiVersion}
};

//**************************************************************************
code struct CMD_STRUCT Factory[10] = {
	{ PASSWORDCmdTxt, PASSCmdTxt, &Factory[1], NULL, NONE, FactoryPassword},
	{ V24CIRCUITCmdTxt, V24CCmdTxt, &Factory[2], &FactoryLevel1[0], NONE, NONE},
	{ SPGGENERATORCmdTxt, SPGGCmdTxt, &Factory[3], &FactoryLevel2[0], NONE, NONE},
	{ AUDIOCmdTxt, AUDCmdTxt, &Factory[4], &FactoryLevel3[0], FactoryAudioSuffix, NONE},
	{ MAINBOARDCmdTxt, MAINCmdTxt, &Factory[5], &FactoryLevel4[0], NONE, NONE},
	{ SDIGENLOCKCmdTxt, SDIGCmdTxt, &Factory[6], &FactoryLevel5[0], NONE, NONE},
	{ TIMECLOCKCmdTxt, TIMCmdTxt, &Factory[7], &FactoryLevel6[0], NONE, NONE},
	{ MULTIBBCmdTxt, MULTCmdTxt, &Factory[8], &FactoryLevel7[0], NONE, NONE},
	{ ADCCmdTxt, ADCCmdTxt, &Factory[9], &FactoryLevel8[0], NONE, NONE},
	{ LDETECTORCmdTxt, LDETCmdTxt, NULL, &FactoryLevel9[0], NONE, NONE},
};

code struct CMD_STRUCT FactoryLevel1[2] = {
	{ ADDRESSCmdTxt, ADDRCmdTxt, &FactoryLevel1[1], NULL, NONE, FactoryAddress},
	{ COMMANDCmdTxt, COMMCmdTxt, NULL, NULL, NONE, FactoryCommand},
};

code struct CMD_STRUCT FactoryLevel2[3] = {
	{ TEXTSTRINGCmdTxt, TEXTCmdTxt, &FactoryLevel2[1], NULL, NONE, FactorySPGUserText},
	{ STORECmdTxt, STORCmdTxt, &FactoryLevel2[2], NULL, NONE, FactorySPGStore},
	{ READCmdTxt, READCmdTxt, NULL, NULL, NONE, FactorySPGRead},
};

code struct CMD_STRUCT FactoryLevel3[3] = {
	{ TEXTSTRINGCmdTxt, TEXTCmdTxt, &FactoryLevel3[1], NULL, NONE, FactoryAudioUserText},
	{ READCmdTxt, READCmdTxt, &FactoryLevel3[2], NULL, NONE, FactoryAudioRead},
	{ MODECmdTxt, MODCmdTxt, NULL, NULL, NONE, FactoryAudioMode}
};

code struct CMD_STRUCT FactoryLevel4[4] = {
	{ TEXTSTRINGCmdTxt, TEXTCmdTxt, &FactoryLevel4[1], NULL, NONE, FactoryMainUserText},
	{ PSN5CmdTxt, PSN5CmdTxt, &FactoryLevel4[2], NULL, FactoryMainPSNSuffix, FactoryMainPSN},
	{ PS12CmdTxt, PS12CmdTxt,  &FactoryLevel4[3], NULL, FactoryMainPSSuffix, FactoryMainPS},
	{ PSCALIBCmdTxt, PSCCmdTxt, NULL, NULL, NONE, FactoryMainPSCalib}
};

code struct CMD_STRUCT FactoryLevel5[1] = {
	{ TEXTSTRINGCmdTxt, TEXTCmdTxt, NULL, NULL, NONE, FactorySDIGenlockUserText}
};

code struct CMD_STRUCT FactoryLevel6[2] = {
	{ TEXTSTRINGCmdTxt, TEXTCmdTxt, &FactoryLevel6[1], NULL, NONE, FactoryTimeClockUserText},
	{ STATUSCmdTxt, STATCmdTxt, NULL, NULL, NONE, FactoryTimeClockStatus}
};

code struct CMD_STRUCT FactoryLevel7[1] = {
	{ TEXTSTRINGCmdTxt, TEXTCmdTxt, NULL, NULL, NONE, FactoryMultiBBUserText}
};

code struct CMD_STRUCT FactoryLevel8[1] = {
	{ READCmdTxt, READCmdTxt, NULL, NULL, NONE, FactoryADCRead}
};

code struct CMD_STRUCT FactoryLevel9[1] = {
	{ READCmdTxt, READCmdTxt, NULL, NULL, NONE, FactoryLDetectorRead}
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
	{ INTERNALParTxt, INTParTxt, &GenlockInputPar[5]},
	{ INTERNAL2ParTxt, INT2ParTxt, NULL}
};

code struct PAR_STRUCT GenlockSystemPar[10] = {
	{ PALBURSTParTxt, PALBParTxt, &GenlockSystemPar[1]},
	{ NTSCBURSTParTxt, NTSCParTxt, &GenlockSystemPar[2]},
	{ SYNC625ParTxt, SYNC625ParTxt, &GenlockSystemPar[3]},
	{ SYNC525ParTxt, SYNC525ParTxt, &GenlockSystemPar[4]},
	{ SDI625ParTxt, SDI625ParTxt, &GenlockSystemPar[5]},
	{ SDI525ParTxt, SDI525ParTxt, &GenlockSystemPar[6]},
	{ F358MHzParTxt, F358MHzParTxt, &GenlockSystemPar[7]},
	{ F443MHzParTxt, F443MHzParTxt, &GenlockSystemPar[8]},
	{ F5MHzParTxt, F5MHzParTxt, &GenlockSystemPar[9]},
	{ F10MHzParTxt, F10MHzParTxt, NULL},
};

code struct PAR_STRUCT AnlSystemPar[3] = {
	{ PALParTxt, PALParTxt, &AnlSystemPar[1]},
	{ PAL_IDParTxt, PAL_IDParTxt, &AnlSystemPar[2]},
	{ NTSCParTxt, NTSCParTxt, NULL},
};

code struct PAR_STRUCT SDISystemPar[2] = {
	{ SDI625ParTxt, SDI625ParTxt, &SDISystemPar[1]},
	{ SDI525ParTxt, SDI525ParTxt, NULL},
};

code struct PAR_STRUCT AnlPatternPar[56] = {
	{ CBSMPTEParTxt, CBSMParTxt, &AnlPatternPar[1]},
	{ CBEBUParTxt, CBEBParTxt, &AnlPatternPar[2]},
	{ CBFCCParTxt, CBFCParTxt, &AnlPatternPar[3]},
	{ CB100ParTxt, CB100ParTxt, &AnlPatternPar[4]},
	{ CBGREY75ParTxt, CBGR75ParTxt, &AnlPatternPar[5]},
	{ CBRED75ParTxt, CBR75ParTxt, &AnlPatternPar[6]},
	{ RED75ParTxt, RED75ParTxt, &AnlPatternPar[7]},

	{ MULTIBURSTParTxt, MULTParTxt, &AnlPatternPar[8]},
	{ LSWEEPParTxt, LSWParTxt, &AnlPatternPar[9]},
	{ MPULSEParTxt, MPULParTxt, &AnlPatternPar[10]},
	{ SINXXParTxt, SINXParTxt, &AnlPatternPar[11]},
	{ CCIR18ParTxt, CCIR18ParTxt, &AnlPatternPar[12]},
	{ NCMBParTxt, NCMBParTxt, &AnlPatternPar[13]},
	{ FCCMBURSTParTxt, FCCMParTxt, &AnlPatternPar[14]},

	{ WIN10ParTxt, WIN10ParTxt, &AnlPatternPar[15]},
	{ WIN15ParTxt, WIN15ParTxt, &AnlPatternPar[16]},
	{ WIN20ParTxt, WIN20ParTxt, &AnlPatternPar[17]},
	{ WIN100ParTxt, WIN100ParTxt, &AnlPatternPar[18]},
	{ BLWH15ParTxt, BLWH15ParTxt, &AnlPatternPar[19]},
	{ GREY50ParTxt, GREY50ParTxt, &AnlPatternPar[20]},
	{ WHITE100ParTxt, WHIT100ParTxt, &AnlPatternPar[21]},
	{ BLACKBURSTParTxt, BLACParTxt, &AnlPatternPar[22]},

	{ FSWAVEParTxt, FSWParTxt, &AnlPatternPar[23]},
	{ BLWH01ParTxt, BLWH01ParTxt, &AnlPatternPar[24]},
	{ CUSTOM1ParTxt, CUST1ParTxt, &AnlPatternPar[25]},
	{ CUSTOM2ParTxt, CUST2ParTxt, &AnlPatternPar[26]},
	{ CUSTOM3ParTxt, CUST3ParTxt, &AnlPatternPar[27]},
	{ CUSTOM4ParTxt, CUST4ParTxt, &AnlPatternPar[28]},
	{ CUSTOM5ParTxt, CUST5ParTxt, &AnlPatternPar[29]},
	{ CUSTOM6ParTxt, CUST6ParTxt, &AnlPatternPar[30]},
	{ CUSTOM7ParTxt, CUST7ParTxt, &AnlPatternPar[31]},
	{ CUSTOM8ParTxt, CUST8ParTxt, &AnlPatternPar[32]},

	{ SRAMPParTxt, SRAMParTxt, &AnlPatternPar[33]},
	{ RAMPParTxt, RAMPParTxt, &AnlPatternPar[34]},
	{ MRAMPParTxt, MRAMParTxt, &AnlPatternPar[35]},
	{ STAIRCASE5ParTxt, STA5ParTxt, &AnlPatternPar[36]},
	{ MSTAIRCASE5ParTxt, MSTA5ParTxt, &AnlPatternPar[37]},
	{ STAIRCASE10ParTxt, STA10ParTxt, &AnlPatternPar[38]},
	{ PBARParTxt, PBARParTxt, &AnlPatternPar[39]},
	{ CCIR17ParTxt, CCIR17ParTxt, &AnlPatternPar[40]},
	{ CCIR330ParTxt, CCIR330ParTxt, &AnlPatternPar[41]},
	{ CCIR331ParTxt, CCIR331ParTxt, &AnlPatternPar[42]},
	{ FCCCompositeParTxt, FCCCompositeParTxt, &AnlPatternPar[43]},
	{ NCMPParTxt, NCMPParTxt, &AnlPatternPar[44]},

	{ PHILIPS43ParTxt, PHIL43ParTxt, &AnlPatternPar[45]},
	{ PHILIPS169ParTxt, PHIL169ParTxt, &AnlPatternPar[46]},
	{ FUBK43ParTxt, FUBK43ParTxt, &AnlPatternPar[47]},
	{ FUBK169ParTxt, FUBK169ParTxt, &AnlPatternPar[48]},
	{ CROSSHATCHParTxt, CROSParTxt, &AnlPatternPar[49]},
	{ CROSSHATCH169ParTxt, CROS169ParTxt, &AnlPatternPar[50]},
	{ CIRCLE43ParTxt, CIRC43ParTxt, &AnlPatternPar[51]},
	{ CIRCLE169ParTxt, CIRC169ParTxt, &AnlPatternPar[52]},
	{ PLUGEParTxt, PLUGParTxt, &AnlPatternPar[53]},
	{ SAFEAREAParTxt, SAFParTxt, &AnlPatternPar[54]},
	{ SWAVE250KHZParTxt, SWAV250KHZParTxt, &AnlPatternPar[55]},
	{ VMT01ParTxt, VMT01ParTxt, NULL},
};

code struct PAR_STRUCT SDIPatternPar[67] = {
	{ CBSMPTEParTxt, CBSMParTxt, &SDIPatternPar[1]},
	{ CBEBUParTxt, CBEBParTxt, &SDIPatternPar[2]},
	{ CBFCCParTxt, CBFCParTxt, &SDIPatternPar[3]},
	{ CBEBU8ParTxt, CBEB8ParTxt, &SDIPatternPar[4]},
	{ CB100ParTxt, CB100ParTxt, &SDIPatternPar[5]},
	{ CBGREY75ParTxt, CBGR75ParTxt, &SDIPatternPar[6]},
	{ CBRED75ParTxt, CBR75ParTxt, &SDIPatternPar[7]},
	{ RED75ParTxt, RED75ParTxt, &SDIPatternPar[8]},

	{ MULTIBURSTParTxt, MULTParTxt, &SDIPatternPar[9]},
	{ LSWEEPParTxt, LSWParTxt, &SDIPatternPar[10]},
	{ YCRCBSWEEPParTxt, YCRCParTxt, &SDIPatternPar[11]},
	{ MPULSEParTxt, MPULParTxt, &SDIPatternPar[12]},
	{ SINXXParTxt, SINXParTxt, &SDIPatternPar[13]},
	{ CCIR18ParTxt, CCIR18ParTxt, &SDIPatternPar[14]},

	{ WIN10ParTxt, WIN10ParTxt, &SDIPatternPar[15]},
	{ WIN15ParTxt, WIN15ParTxt, &SDIPatternPar[16]},
	{ WIN20ParTxt, WIN20ParTxt, &SDIPatternPar[17]},
	{ WIN100ParTxt, WIN100ParTxt, &SDIPatternPar[18]},
	{ BLWH15ParTxt, BLWH15ParTxt, &SDIPatternPar[19]},
	{ WHITE100ParTxt, WHIT100ParTxt, &SDIPatternPar[20]},
	{ BLACKParTxt, BLACParTxt, &SDIPatternPar[21]},

	{ SDICHECKParTxt, SDICParTxt, &SDIPatternPar[22]},
	{ DTIMINGParTxt, DTIMParTxt, &SDIPatternPar[23]},
	{ FDTESTParTxt, FDTParTxt, &SDIPatternPar[24]},
	{ BOWTIEParTxt, BOWTParTxt, &SDIPatternPar[25]},
	{ ABLANKINGParTxt, ABLParTxt, &SDIPatternPar[26]},
	{ DGREYParTxt, DGRParTxt, &SDIPatternPar[27]},
	{ FSWAVEParTxt, FSWParTxt, &SDIPatternPar[28]},
	{ BLWH01ParTxt, BLWH01ParTxt, &SDIPatternPar[29]},
	{ EOLINEParTxt, EOLParTxt, &SDIPatternPar[30]},
	{ WEOLINEParTxt, WEOLParTxt, &SDIPatternPar[31]},
	{ BEOLINEParTxt, BEOLParTxt, &SDIPatternPar[32]},
	{ REOLINEParTxt, REOLParTxt, &SDIPatternPar[33]},
	{ YEOLINEParTxt, YEOLParTxt, &SDIPatternPar[34]},
	{ CEOLINEParTxt, CEOLParTxt, &SDIPatternPar[35]},
	{ CUSTOM1ParTxt, CUST1ParTxt, &SDIPatternPar[36]},
	{ CUSTOM2ParTxt, CUST2ParTxt, &SDIPatternPar[37]},
	{ CUSTOM3ParTxt, CUST3ParTxt, &SDIPatternPar[38]},
	{ CUSTOM4ParTxt, CUST4ParTxt, &SDIPatternPar[39]},
	{ CUSTOM5ParTxt, CUST5ParTxt, &SDIPatternPar[40]},
	{ CUSTOM6ParTxt, CUST6ParTxt, &SDIPatternPar[41]},
	{ CUSTOM7ParTxt, CUST7ParTxt, &SDIPatternPar[42]},
	{ CUSTOM8ParTxt, CUST8ParTxt, &SDIPatternPar[43]},

	{ SRAMPParTxt, SRAMParTxt, &SDIPatternPar[44]},
	{ RAMPParTxt, RAMPParTxt, &SDIPatternPar[45]},
	{ LRAMPParTxt, LRAMParTxt, &SDIPatternPar[46]},
	{ VRAMPParTxt, VRAMParTxt, &SDIPatternPar[47]},
	{ STAIRCASE5ParTxt, STA5ParTxt, &SDIPatternPar[48]},
	{ MSTAIRCASE5ParTxt, MSTA5ParTxt, &SDIPatternPar[49]},
	{ STAIRCASE10ParTxt, STA10ParTxt, &SDIPatternPar[50]},
	{ PBARParTxt, PBARParTxt, &SDIPatternPar[51]},
	{ CCIR17ParTxt, CCIR17ParTxt, &SDIPatternPar[52]},
	{ CCIR330ParTxt, CCIR330ParTxt, &SDIPatternPar[53]},
	{ CCIR331ParTxt, CCIR331ParTxt, &SDIPatternPar[54]},
	{ YGRAMPParTxt, YGRParTxt, &SDIPatternPar[55]},
	{ GBRAMPParTxt, GBRParTxt, &SDIPatternPar[56]},
	{ CGRAMPParTxt, CGRParTxt, &SDIPatternPar[57]},
	{ GRRAMPParTxt, GRRParTxt, &SDIPatternPar[58]},
	{ CBYCRYRAMPParTxt, CBYCParTxt, &SDIPatternPar[59]},

	{ PHILIPS43ParTxt, PHIL43ParTxt, &SDIPatternPar[60]},
	{ PHILIPS169ParTxt, PHIL169ParTxt, &SDIPatternPar[61]},
	{ FUBK43ParTxt, FUBK43ParTxt, &SDIPatternPar[62]},
	{ FUBK169ParTxt, FUBK169ParTxt, &SDIPatternPar[63]},
	{ CROSSHATCHParTxt, CROSParTxt, &SDIPatternPar[64]},
	{ PLUGEParTxt, PLUGParTxt, &SDIPatternPar[65]},
	{ SAFEAREAParTxt, SAFParTxt, &SDIPatternPar[66]},
	{ VMT01ParTxt, VMT01ParTxt, NULL}
};

code struct PAR_STRUCT TSGAudioSignalPar[2] = {
	{ OFFParTxt, OFFParTxt, &TSGAudioSignalPar[1]},
	{ S1KHZParTxt, S1KHZParTxt, NULL},
};

code struct PAR_STRUCT TSGAudioLevelPar[5] = {
	{ SILENCEParTxt, SILParTxt, &TSGAudioLevelPar[1]},
	{ DB0FSParTxt, DB0FSParTxt, &TSGAudioLevelPar[2]},
	{ DB9FSParTxt, DB9FSParTxt, &TSGAudioLevelPar[3]},
	{ DB15FSParTxt, DB15FSParTxt, &TSGAudioLevelPar[4]},
	{ DB18FSParTxt, DB18FSParTxt, NULL}
};

code struct PAR_STRUCT TPGAudioSignalPar[7] =      // mod for moving logo: JK
  {
  	{ OFFParTxt, OFFParTxt, &TPGAudioSignalPar[1]},
  	{ S800HZParTxt, S800HZParTxt, &TPGAudioSignalPar[2]},
  	{ S1KHZParTxt, S1KHZParTxt, &TPGAudioSignalPar[3]},
  	{ SEBU1KHZParTxt, SEBU1KHZParTxt, &TPGAudioSignalPar[4]},
  //	{ SBBC1KHZParTxt, SBBC1KHZParTxt, &TPGAudioSignalPar[5]},
  	{ MEBU1KHZParTxt, MEBU1KHZParTxt, &TPGAudioSignalPar[5]},
  	{ M1KHZParTxt, M1KHZParTxt, &TPGAudioSignalPar[6]},
  	{ DUALParTxt, DUALParTxt, NULL},
  };

code struct PAR_STRUCT TPGAudioLevelPar[7] =     // mod for moving logo: JK
  {
  	{ SILENCEParTxt, SILParTxt, &TPGAudioLevelPar[1]},
  	{ DB0FSParTxt, DB0FSParTxt, &TPGAudioLevelPar[2]},
  	{ DB9FSParTxt, DB9FSParTxt, &TPGAudioLevelPar[3]},
  	{ DB12FSParTxt, DB12FSParTxt, &TPGAudioLevelPar[4]},
//  	{ DB14FSParTxt, DB14FSParTxt, &TPGAudioLevelPar[5]},
//  	{ DB15FSParTxt, DB15FSParTxt, &TPGAudioLevelPar[6]},   // JK
  	{ DB16FSParTxt, DB16FSParTxt, &TPGAudioLevelPar[5]},
  	{ DB18FSParTxt, DB18FSParTxt, &TPGAudioLevelPar[6]},
  	{ DB20FSParTxt, DB20FSParTxt, NULL},
  };

code struct PAR_STRUCT TPGAudioGroupPar[4] = {
	{ GROUP1ParTxt, GRO1ParTxt, &TPGAudioGroupPar[1]},
	{ GROUP2ParTxt, GRO2ParTxt, &TPGAudioGroupPar[2]},
	{ GROUP3ParTxt, GRO3ParTxt, &TPGAudioGroupPar[3]},
	{ GROUP4ParTxt, GRO4ParTxt, NULL}
};

code struct PAR_STRUCT AESEBUSignalPar[9] = {
	{ OFFParTxt, OFFParTxt, &AESEBUSignalPar[1]},
	{ S800HZParTxt, S800HZParTxt, &AESEBUSignalPar[2]},
	{ S1KHZParTxt, S1KHZParTxt, &AESEBUSignalPar[3]},
	{ SEBU1KHZParTxt, SEBU1KHZParTxt, &AESEBUSignalPar[4]},
	{ SBBC1KHZParTxt, SBBC1KHZParTxt, &AESEBUSignalPar[5]},
	{ MEBU1KHZParTxt, MEBU1KHZParTxt, &AESEBUSignalPar[6]},
	{ M1KHZParTxt, M1KHZParTxt, &AESEBUSignalPar[7]},
	{ DUALParTxt, DUALParTxt, &AESEBUSignalPar[8]},
	{ WORDCLOCKParTxt, WORDParTxt, NULL},
};

code struct PAR_STRUCT AESEBULevelPar[9] = {
	{ SILENCEParTxt, SILParTxt, &AESEBULevelPar[1]},
	{ DB0FSParTxt, DB0FSParTxt, &AESEBULevelPar[2]},
	{ DB9FSParTxt, DB9FSParTxt, &AESEBULevelPar[3]},
	{ DB12FSParTxt, DB12FSParTxt, &AESEBULevelPar[4]},
	{ DB14FSParTxt, DB14FSParTxt, &AESEBULevelPar[5]},
	{ DB15FSParTxt, DB15FSParTxt, &AESEBULevelPar[6]},
	{ DB16FSParTxt, DB16FSParTxt, &AESEBULevelPar[7]},
	{ DB18FSParTxt, DB18FSParTxt, &AESEBULevelPar[8]},
	{ DB20FSParTxt, DB20FSParTxt, NULL},
};

code struct PAR_STRUCT AESEBUTimingPar[6] = {
	{ PALParTxt, PALParTxt, &AESEBUTimingPar[1]},
	{ NTSC1ParTxt, NTSC1ParTxt, &AESEBUTimingPar[2]},
	{ NTSC2ParTxt, NTSC2ParTxt, &AESEBUTimingPar[3]},
	{ NTSC3ParTxt, NTSC3ParTxt, &AESEBUTimingPar[4]},
	{ NTSC4ParTxt, NTSC4ParTxt, &AESEBUTimingPar[5]},
	{ NTSC5ParTxt, NTSC5ParTxt, NULL},
};

code struct PAR_STRUCT TPGTextClockPar[4] = {
	{ OFFParTxt, OFFParTxt, &TPGTextClockPar[1]},
	{ TIMEParTxt, TIMParTxt, &TPGTextClockPar[2]},
	{ DATEParTxt, DATParTxt, &TPGTextClockPar[3]},
	{ DTIMEParTxt, DTIMParTxt, NULL}
};

code struct PAR_STRUCT TimeClockDFormatPar[3] = {
	{ DMYParTxt, DMYParTxt, &TimeClockDFormatPar[1]},
	{ MDYParTxt, MDYParTxt, &TimeClockDFormatPar[2]},
	{ YMDParTxt, YMDParTxt, NULL}
};

code struct PAR_STRUCT TimeClockTFormatPar[2] = {
	{ H12ParTxt, H12ParTxt, &TimeClockTFormatPar[1]},
	{ H24ParTxt, H24ParTxt, NULL}
};

code struct PAR_STRUCT TimeClockRefPar[4] = {
	{ VITCParTxt, VITCParTxt, &TimeClockRefPar[1]},
	{ LTCParTxt, LTCParTxt, &TimeClockRefPar[2]},
	{ REF1HZParTxt, REF1HZParTxt, &TimeClockRefPar[3]},
	{ VFFREQUENCYParTxt, VFFRParTxt, NULL},
};

code struct PAR_STRUCT FactoryAddressPar[8] = {
	{ BBU_0ParTxt, BBU_0ParTxt, &FactoryAddressPar[1]},
	{ BBU_1ParTxt, BBU_1ParTxt, &FactoryAddressPar[2]},
	{ BBU_2ParTxt, BBU_2ParTxt, &FactoryAddressPar[3]},
	{ BBU_3ParTxt, BBU_3ParTxt, &FactoryAddressPar[4]},
	{ SPGParTxt, SPGParTxt, &FactoryAddressPar[5]},
	{ ANLSIGNALParTxt, ANLSParTxt, &FactoryAddressPar[6]},
	{ SDISIGNALParTxt, SDISParTxt, &FactoryAddressPar[7]},
	{ SDITPGParTxt, SDITParTxt, NULL},
};

code struct PAR_STRUCT OffOnPar[2] = {
	{ OFFParTxt, OFFParTxt, &OffOnPar[1]},
	{ ONParTxt, ONParTxt, NULL},
};

code struct PAR_STRUCT MinMaxPar[2] = {
	{ MINParTxt, MINParTxt, &MinMaxPar[1]},
	{ MAXParTxt, MAXParTxt, NULL},
};

code struct PAR_STRUCT StylePar[3] = {
	{ FREEParTxt, FREEParTxt, &StylePar[1]},
	{ STANDARDParTxt, STANParTxt, &StylePar[2]},
	{ COMPLEXParTxt, COMPParTxt, NULL},
};

code struct PAR_STRUCT PT52ModePar[2] = {
	{ PT5210ParTxt, PT5210ParTxt, &PT52ModePar[1]},
	{ PT5230ParTxt, PT5230ParTxt, NULL},
};

code UC V24CRAMAddressTable[8] = {
	bbu0_addr,
	bbu1_addr,
	bbu2_addr,
	bbu3_addr,
	sync_addr,
	AnlSig_addr,
	SDISig_addr,
	SDITPG_addr
};

/***************************************************************************/
// Temporary variables PRIVATE NOT overlayable

static UC UnitAddress;
static char CmdBuffer[4];

static UC Suffix;
static UC StringSuffix;
static UC AttribSuffix;
static UC AudioSuffix;

static char TmpBuffer[SizeOfTmpBuffer+1];

static UC ParState;

static char Field;
static int Line;
static float HTime;
static char sign;

static UL UlongVal;

static int IntVal;
static int IntVal1;

static UC UcharVal;
static UC UcharVal1;

static UC* downPtr;

/***************************************************************************/
/*	IDNResponseMessage														 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970218	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char	 														*/
/*					ParState 2: Char, Char	 												*/
/*					ParState 3: Char, Char, Char	 										*/
/*					ParState 4: Char, Char, Char, Char									*/
/*					IDN is : PTV,PT5230,KU123456,1911									*/
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
		ResponseReceived = 0xFF;
}

/***************************************************************************/
/*	CLSCommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
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
/*	ESECommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970627	    										*/
/* Revised:		980611				     													*/
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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				sprintf( TmpBuffer, "%d\n", ESEReg);
				Udr_PutNChar( TmpBuffer);
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else
				ESEReg = UcharVal;
		}
	}
}

/***************************************************************************/
/*	ESRCommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970627	    										*/
/* Revised:		980611				     													*/
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
/*	OPCCommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
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
/*	SRECommand															 		 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970627	    										*/
/* Revised:		980611				     													*/
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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				sprintf( TmpBuffer, "%d\n", SREReg);
				Udr_PutNChar( TmpBuffer);
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else
				SREReg = UcharVal & 0xBF;
		}
	}
}

/***************************************************************************/
/*	STBCommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970627	    										*/
/* Revised:		980611				     													*/
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
/*	TSTCommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		980611				     													*/
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
/*	WAICommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		980611				     													*/
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
/*	RSTCommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 961124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Reset comamnd																*/
/*	Remarks:		No parameters																*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void RSTCommand() {

	register int i;

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;
	else {
		if ( CmdExecute && !SCPIError) {
			if ( !CmdRequest) {

				for ( i = 0; i < MaxAnlBlkUnits; i++) {
					Cmemcpy( &AnlBlkConfig[i], &AnlBlkConfigFactory[i], sizeof( AnlBlkConfig[0]));

					if ( AnlBlkUnit[i].Present)
						ConfigureAnlBlkUnit( i);
				}

				FeedTheWatchdog;

				for ( i = 0; i < MaxAnlTPGUnits; i++) {
					Cmemcpy( &AnlTPGConfig[i], &AnlTPGConfigFactory[i], sizeof( AnlTPGConfig[0]));

					if ( AnlTPGUnit[i].Present)
						ConfigureAnlTPGUnit( i);
				}

				FeedTheWatchdog;

				for ( i = 0; i < MaxSDITSGUnits; i++) {
					Cmemcpy( &SDITSGConfig[i], &SDITSGConfigFactory[i], sizeof( SDITSGConfig[0]));

					if ( SDITSGUnit[i].Present)
						ConfigureSDITSGUnit( i);
				}

				FeedTheWatchdog;

				for ( i = 0; i < MaxSDITPGUnits; i++){
					Cmemcpy( &SDITPGConfig[i], &SDITPGConfigFactory[i], sizeof( SDITPGConfig[0]));

					if ( SDITPGUnit[i].Present)
						ConfigureSDITPGUnit( i);
				}

				FeedTheWatchdog;

				for ( i = 0; i < MaxAESEBUUnits; i++)
					Cmemcpy( &AESEBUConfig[i], &AESEBUConfigFactory[i], sizeof( AESEBUConfig[0]));

				if ( AESEBUUnit.Present) {
					AESEBUUnitDrv( AESEBU1Item, AESEBUConfig[AESEBU1Item].Signal,\
														 AESEBUConfig[AESEBU1Item].Level,\
														 AESEBUConfig[AESEBU1Item].Timing);

					if ( AESEBUUnit.HWType == PT8635)
						AESEBUUnitDrv( AESEBU2Item, AESEBUConfig[AESEBU2Item].Signal,\
															 AESEBUConfig[AESEBU2Item].Level,\
															 AESEBUConfig[AESEBU2Item].Timing);
				}

				FeedTheWatchdog;

				for ( i = 0; i < MaxGenlockUnits; i++)
					Cmemcpy( &GenlockConfig[i], &GenlockConfigFactory[i], sizeof( GenlockConfig[0]));

				ActiveGenlockNo = Internal;
				ExternalGenlockNo = GenlockA;

				GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);

				Cmemcpy( &RS232Config, &RS232ConfigFactory, sizeof( RS232Config));

				LCDContrast = DefContrast;

				ActiveOutputNo = AnlBlkItem;		// AnlBlkItem is NOT part of output
				ActivePresetNo = 0;

				if ( Ptr == &status_menu[StatusPresetItem])
					Ptr->DispFunct();

				Flags.NormalLockOn = OFF;
				Flags.PanelLockOn = OFF;
				Flags.DateTimeLockOn = OFF;
				Flags.DownloadLockOn = OFF;
				Flags.DiagnoseLockOn = OFF;

				Flags.AutoESCEnable = ON;
				Flags.AutoESCToStatus = ON;

				PasswordState = ON;

														// Reset apparatus error queue
				for ( i=0; i < NoInErrorQueue; i++)
					ErrorQueue[i] = 0;

				ErrorQueuePtr = NoInErrorQueue-1;

				ResetErrorEventQueue();			// Reset RS232 error queue

														// Initialize the remote RS232
				Udr_InitializeUart( RS232Config.Baudrate, RS232Config.Databit,\
											 RS232Config.Parity, RS232Config.Handshake);

			}
		}
	}
}

/***************************************************************************/
/*	IDNCommand																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
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

			IIC2RdStr( mainram1_addr, CompanyName_addr, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			IIC2RdStr( mainram1_addr, ProductName1_addr, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			if ( strlen( TmpBuffer) >= 10) {
				IIC2RdStr( mainram1_addr, ProductName2_addr, TmpBuffer);
				Udr_PutNChar( TmpBuffer);
			}

			Udr_PutChar( ',');

			IIC2RdStr( mainram1_addr, ProductKUNo_addr, TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			GetPT5230SWVersion( TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutNewline();
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	SystemError																	 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		98061				     													*/
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
/*	SystemVersion																 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
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
/*	SystemPreset																 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		980611			     														*/
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

				if (( UcharVal == 0) || ( UcharVal > 6))
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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				Udr_PutChar( '0' + ActivePresetNo);
				Udr_PutNewline();
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else {
				ActivePresetNo = UcharVal;

				if ( Ptr == &status_menu[StatusPresetItem])
					Ptr->DispFunct();

				if ( ConfigurePreset( --UcharVal))
					SCPIError = ExecErr;
			}
		}
	}
}

/***************************************************************************/
/*	SystemPresetStore															 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Store actual setup in preset											*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemPresetStore() {

	register int i;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal = ( UC) atoi( Cmd);

				if (( UcharVal == 0) || ( UcharVal > 6))
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
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else {
				ActivePresetNo = UcharVal;

				if ( Ptr == &status_menu[StatusPresetItem])
					Ptr->DispFunct();

				UcharVal--;

				for ( i = 0; i < MaxAnlBlkUnits; i++)
					memcpy( &Preset[UcharVal].AnlBlkStore[i], &AnlBlkConfig[i], sizeof( AnlBlkConfig[0]));

				for ( i = 0; i < MaxAnlTPGUnits; i++)
					memcpy( &Preset[UcharVal].AnlTPGStore[i], &AnlTPGConfig[i], sizeof( AnlTPGConfig[0]));

				for ( i = 0; i < MaxSDITSGUnits; i++)
					memcpy( &Preset[UcharVal].SDITSGStore[i], &SDITSGConfig[i], sizeof( SDITSGConfig[0]));

				for ( i = 0; i < MaxSDITPGUnits; i++)
					memcpy( &Preset[UcharVal].SDITPGStore[i], &SDITPGConfig[i], sizeof( SDITPGConfig[0]));

				for ( i = 0; i < MaxAESEBUUnits; i++)
					memcpy( &Preset[UcharVal].AESEBUStore[i], &AESEBUConfig[i], sizeof( AESEBUConfig[0]));

				for ( i = 0; i < MaxGenlockUnits; i++)
					memcpy( &Preset[UcharVal].GenlockStore[i], &GenlockConfig[i], sizeof( GenlockConfig[0]));

				Preset[UcharVal].ActiveGenlockNo = ActiveGenlockNo;
				Preset[UcharVal].ExternalGenlockNo = ExternalGenlockNo;
			}
		}
	}
}

/***************************************************************************/
/*	SystemPresetName															 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		990305					     												*/
/*																									*/
/*	Function:	Name a specific preset number.										*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum, String												*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemPresetName() {

	register int i;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal = ( UC) atoi( Cmd);

				if (( UcharVal == 0) || ( UcharVal > 6))
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
			switch ( ParState) {
				case 0:
					SCPIError = MissingPar;
					break;

				case 1:
					Udr_PutChar( '"');
					Udr_PutNChar( Preset[UcharVal].Name);
					Udr_PutChar( '"');
					Udr_PutNewline();
					break;

				default:
					SCPIError = SyntaxErr;
					break;
			}
		}
		else {
			if ( ParState < 2)
				SCPIError = MissingPar;
			  else {
				for ( i=0; i < strlen( Cmd); i++)
					if ( !PresetCharSet[Cmd[i]])
						break;

				if ( i == strlen( Cmd))
					strcpy( Preset[UcharVal].Name, Cmd);
				else
					SCPIError = ParErr;
			}
		}
	}
}

/***************************************************************************/
/*	SystemPresetDownload														 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		991103					     												*/
/*																									*/
/*	Function:	Send a specific preset to another SPG								*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					The data is sent in four parts due to the small amount of	*/
/*					SCPI/RS232 input buffer memory, Cmd[512]/128.					*/
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

				if (( UcharVal == 0) || ( UcharVal > 6))
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
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else {
				NoOfBytes = sizeof( Preset[0]);
				downPtr  = ( UC*) &Preset[UcharVal];

				do {
					sprintf( TmpBuffer, "#3%03d\0", 2*min( 200, NoOfBytes) + 2 + 8);
//					TmpBuffer[5] = 0;

					Udr_PutNChar( TmpBuffer);

					Udr_PutChar(( DownloadRevision >> 4) + DownloadOffset); // Revision
					Udr_PutChar(( DownloadRevision & 0x0F) + DownloadOffset);

					checksum = DownloadRevision;

					for ( i = 0; i < min( 200, NoOfBytes); i++) {
						tmp = *downPtr >> 4;
						checksum += tmp;
						Udr_PutChar(( char) ( tmp + DownloadOffset));

						tmp = *downPtr & 0x0F;
						checksum += tmp;
						Udr_PutChar(( char) ( tmp + DownloadOffset));

						downPtr++;
					}

					Udr_PutChar((( checksum & 0xF0000000) >> 28) + DownloadOffset);
					Udr_PutChar((( checksum & 0x0F000000) >> 24) + DownloadOffset);
					Udr_PutChar((( checksum & 0x00F00000) >> 20) + DownloadOffset);
					Udr_PutChar((( checksum & 0x000F0000) >> 16) + DownloadOffset);
					Udr_PutChar((( checksum & 0x0000F000) >> 12) + DownloadOffset);
					Udr_PutChar((( checksum & 0x00000F00) >> 8) + DownloadOffset);
					Udr_PutChar((( checksum & 0x000000F0) >> 4) + DownloadOffset);
					Udr_PutChar((( checksum & 0x0000000F) + DownloadOffset));

					NoOfBytes -= min( 200, NoOfBytes);

					if ( NoOfBytes)
						Udr_PutChar( ',');

				} while( NoOfBytes);

				Udr_PutNewline();
			}
		}
	}
}

/***************************************************************************/
/*	SystemPresetUpload														 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		991103				     													*/
/*																									*/
/*	Function:	Receive a specific preset from another SPG						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum, Blockn												*/
/*					This routine MUST match SystemPresetDownload EXACTLY			*/
/*					Command wil split the transmission into 4 bloack arrays		*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemPresetUpload() {

	UI NoOfBytes, i;
	UL CalcChecksum, RecChecksum;

	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal = ( UC) atoi( Cmd);

				if (( UcharVal == 0) || ( UcharVal > 6))
					SCPIError = DataOutOfRange;
				else {
					UcharVal--;

					downPtr  = ( UC*) &Preset[UcharVal];
					UcharVal1 = ResponseReceived;
				}
			}
			else
				SCPIError = DataTypeErr;
			break;

		case P_Blockn:
			if ( !UcharVal1) {

				CalcChecksum = RecChecksum = 0;

				NoOfBytes = strlen( Cmd);	// At this point Cmd[] has been stripped
													//  of the number of bytes, (#3nnn).

				for ( i = 0; i < NoOfBytes; i++) {
					Cmd[i] -= DownloadOffset;

					if ( i < ( NoOfBytes - 8))
						CalcChecksum += Cmd[i];
					else
						RecChecksum = ( RecChecksum << 4) + Cmd[i];
				}

				if ((( Cmd[0] << 4) + Cmd[1]) != DownloadRevision)
					UcharVal1 = InvalidVersion;
				else 
					if ( RecChecksum != CalcChecksum)
						UcharVal1 = InvalidBlockData;
				else
					for ( i = 2; i < NoOfBytes-8; i += 2)
						*downPtr++ = ( Cmd[i] << 4) + Cmd[i+1];
			}

			if ( ParState < 5)
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
			if ( ParState < 5) {
				SCPIError = MissingPar;			// FACTORY PRESET FOR PRESET UcharVar
			}
			else {
				if ( !UcharVal1)
					ResponseReceived = 0xFF;
				else
					ResponseReceived = UcharVal1;
			}
		}
	}
}

/***************************************************************************/
/*	SystemDownload																 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Download actual setup from another SPG								*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemDownload() {

	SCPIError = ExecErr;
}

/***************************************************************************/
/*	SystemUpload																 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		971112				     													*/
/*																									*/
/*	Function:	Download actual setup from another SPG								*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void SystemUpload() {

	SCPIError = ExecErr;
}

/***************************************************************************/
/*	StatusOperEvent															 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		980611				     													*/
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
/*	StatusOperCondition														 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		980611				     													*/
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
/*	StatusOperEnable															 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		980611				     													*/
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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				sprintf( TmpBuffer, "%d\n", OperEnableReg);
				Udr_PutNChar( TmpBuffer);
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else
				OperEnableReg = IntVal & 0xEFFF;
		}
	}
}

/***************************************************************************/
/*	StatusQuestEvent															 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		980611				     													*/
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
/*	StatusQuestCondition														 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		980611				     													*/
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
/*	StatusQuestEnable															 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 970630	    										*/
/* Revised:		980611				     													*/
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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				sprintf( TmpBuffer, "%d\n", QuestEnableReg);
				Udr_PutNChar( TmpBuffer);
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else
				QuestEnableReg = IntVal & 0xEFFF;
		}
	}
}

/***************************************************************************/
/*	StatusPreset																 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void StatusPreset() {
}

/***************************************************************************/
/*	StatusPTSuffix														 		 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void StatusPTSuffix() {

	if ( NumSuffix != 5230)
		SCPIError = SyntaxErr;
}

/***************************************************************************/
/*	StatusPT																 		 RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void StatusPT() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest && ( NumSuffix == 5230)) {
			Udr_PutChar( '"');

			if ( ErrorStatus)
				Udr_CPutNChar( ActiveErrorsTxt);
			else {
				if ( ErrorQueue[ErrorQueuePtr])
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
/*	DiagnosticDisplay													 	    RS232CMD.C */
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void DiagnosticDisplay() {
}

/***************************************************************************/
/*	DiagnosticErrorQueueLast										 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		991026				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:																						*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void DiagnosticErrorQueueLast() {

	char tmpbuf[40];

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {

			UcharVal = ErrorQueue[ErrorQueuePtr] & 0x000000FF;

			if ( ErrorQueue[ErrorQueuePtr])
				GetErrorString( ErrorQueue[ErrorQueuePtr], tmpbuf);
			else
				Cstrcpy( tmpbuf, NoErrorDetectedTxt);

			sprintf( TmpBuffer, "%d", UcharVal);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');
	
			Udr_PutChar( '"');
			Udr_PutNChar( tmpbuf);
			Udr_PutChar( '"');
			Udr_PutNewline();
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	DiagnosticErrorQueueEntry										 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 991026	    										*/
/* Revised:		991026				     													*/
/*																									*/
/*	Function:	Return an entry in the error queue									*/
/*	Remarks:		Query only																	*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void DiagnosticErrorQueueEntry() {

	char tmpbuf[40];
	
	switch ( ParameterType) {
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0) {
				ParState++;

				UcharVal1 = ( UC) atoi( Cmd);

				if (( UcharVal1 == 0) || ( UcharVal1 > 5))
					SCPIError = DataOutOfRange;

				UcharVal1--;
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

			UcharVal = ErrorQueue[UcharVal1] & 0x000000FF;

			if ( ErrorQueue[UcharVal1])
				GetErrorString( ErrorQueue[UcharVal1], tmpbuf);
			else
				Cstrcpy( tmpbuf, NoErrorDetectedTxt);

			sprintf( TmpBuffer, "%d", UcharVal);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');
	
			Udr_PutChar( '"');
			Udr_PutNChar( tmpbuf);
			Udr_PutChar( '"');
			Udr_PutNewline();
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	DiagnosticErrorQueueReset										 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Reset the internal PT5230 error queue, NOT the SCPI queue	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Internal error queue														*/
/***************************************************************************/
void DiagnosticErrorQueueReset() {

	register int i;

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( !CmdRequest) {
			for( i = 0; i < NoInErrorQueue; i++)
				ErrorQueue[i] = 0;

			ErrorStatus = 0;
			ErrorQueuePtr = NoInErrorQueue-1;
		}
		else
			SCPIError = SyntaxErr;
	}
}

/***************************************************************************/
/*	DisplayContrast													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
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

				if (( UcharVal = ( UC) atoi( Cmd)) > MaxContrast)
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
			if ( ParState == 1)
				SCPIError = DataTypeErr;
			else {
				if ( ParState == 0)
					tmp = LCDContrast;
				else {
					if ( UcharVal == MIN)
						tmp = MinContrast;
					else
						tmp = MaxContrast;
				}

				sprintf( TmpBuffer, "%d\r\n", tmp);
				Udr_PutNChar( TmpBuffer);
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else {
				if ( ParState == 1)
					LCDContrast = UcharVal;
				else {
					if ( UcharVal == MIN)
						LCDContrast = MinContrast;
					else
						LCDContrast = MaxContrast;
				}
				SendContrast( LCDContrast);

				if ( IICErrorCode)
					SCPIError = ExecErr;
			}
		}
	}
}

/***************************************************************************/
/*	Genlock																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980915				     													*/
/*																									*/
/*	Function:	Return the Genlock Configuration.									*/
/*	Remarks:		Request only																*/
/*	Returns:		--																				*/
/*	Updates:	   --																				*/
/***************************************************************************/
void Genlock() {

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {

		if ( CmdRequest) {
			if ( UnlockedLED)
				Udr_CPutNChar( UNLOCKEDTxt);
			else {
				if (( GenlockConfig[ActiveGenlockNo].System < SyncLock625) &&
					 ( ActiveGenlockNo < SDIGenlock)) {
					if ( burstLockOn)
						Udr_CPutNChar( GENLOCKEDTxt);
					else
						Udr_CPutNChar( SYNCLOCKEDTxt);
				}
				else
					Udr_CPutNChar( GENLOCKEDTxt);
			}

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
				AnlSamplesToFLT( GenlockConfig[ActiveGenlockNo].System,\
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
/*	GenlockInput														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the genlock input command										*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				Udr_CPutNChar( GenlockInputPar[ActiveGenlockNo].LongName);
				Udr_PutNewline();
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else {
				if ( UcharVal != ActiveGenlockNo) {
					ActivePresetNo = 0;					// Clear possible preset

					if ( Ptr == &status_menu[StatusPresetItem])
						Ptr->DispFunct();

					ActiveGenlockNo = UcharVal;

					if ( UcharVal < Internal)			// If new input isn't internal..
						ExternalGenlockNo = UcharVal;	//  update also external no

					GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
				}
			}
		}
	}
}

/***************************************************************************/
/*	GenlockSystem														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the genlock system command									*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				Udr_CPutNChar( GenlockSystemPar[GenlockConfig[ActiveGenlockNo].System].LongName);
				Udr_PutNewline();
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else {
				if ( ActiveGenlockNo < Internal) {

					if ( ActiveGenlockNo == SDIGenlock) {
						if (( UcharVal < SDI625Lock) || ( UcharVal > SDI525Lock))
							SCPIError = ExecErr;
					}
					else {
						if (( UcharVal == SDI625Lock) || ( UcharVal == SDI525Lock))
							SCPIError = ExecErr;
					}

					if ( SCPIError == NoErr) {
						if ( UcharVal != GenlockConfig[ActiveGenlockNo].System) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							GenlockConfig[ActiveGenlockNo].System = UcharVal;

							GenlockUnitDrv( ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
						}
					}
				}
				else
					SCPIError = ExecErr;
			}
		}
	}
}

/***************************************************************************/
/*	GenlockDelay														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the genlock timing command									*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*					ParState 3:  DecNum, DecNum, DecNum									*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void GenlockDelay() {

	FindDelayParameters();

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest) {
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				if (( ActiveGenlockNo < Internal) &&
					 (	GenlockConfig[ActiveGenlockNo].System < MHzLock358)) {

					if ( ActiveGenlockNo == SDIGenlock)
						SDISamplesToFLT( GenlockConfig[ActiveGenlockNo].System,\
											  GenlockConfig[ActiveGenlockNo].Delay,\
											  &sign, &Field, &Line, &HTime);
					else
						AnlSamplesToFLT( GenlockConfig[ActiveGenlockNo].System,\
												  GenlockConfig[ActiveGenlockNo].Delay,\
												  &sign, &Field, &Line, &HTime);

					PrintTimingFLT( sign, Field, Line, HTime);
					Udr_PutNewline();
				}
			}
		}
		else {
			if ( ParState < 3)
				SCPIError = MissingPar;
			else {
				if ( ActiveGenlockNo < Internal) {
					UlongVal = AnlFLTToSamples(\
													GenlockConfig[ActiveGenlockNo].System,\
													Field, Line, HTime);

					if ( UlongVal != -1) {
						if ( UlongVal != GenlockConfig[ActiveGenlockNo].Delay) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							GenlockConfig[ActiveGenlockNo].Delay = UlongVal;

							TrxGenlockDelay( UlongVal);
						}
					}
					else
						SCPIError = DataOutOfRange;
				}
			}
		}
	}
}

/***************************************************************************/
/*	BBSuffix																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980402				     													*/
/*																									*/
/*	Function:	Get suffix of the Black Burst generator							*/
/* Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Suffix																		*/
/***************************************************************************/
void BBSuffix() {

	if ( NumSuffix > 8)
		SCPIError = HdrSuffixOutOfRange;
	else
		Suffix = NumSuffix-1;
}

/***************************************************************************/
/*	BB																		 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the Black Burst command.										*/
/* Remarks:		Request only																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void BB() {

	if ( AnlBlkUnit[Suffix].Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				Udr_CPutNChar( AnlSystemPar[AnlBlkConfig[Suffix].System].LongName);
				Udr_PutChar( ',');

				AnlSamplesToFLT( AnlBlkConfig[Suffix].System,\
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
/*	BBSystem																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the Black Burst system-command.								*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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

					UcharVal = FindParameter( Cmd, AnlSystemPar);

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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( AnlSystemPar[AnlBlkConfig[Suffix].System].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != AnlBlkConfig[Suffix].System) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						AnlBlkConfig[Suffix].System = UcharVal;

															// Test if delay is valid
															//  in the new system, (AFTER
															//  the system is saved)
						AnlValidateSetting( AnlBlkUnit[Suffix].HWType, Suffix);

						TrxAnlBlkSystem( Suffix, UcharVal, AnlBlkConfig[Suffix].Delay,\
															  AnlBlkConfig[Suffix].ScHPhase);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBScHPhase															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the Black Burst ScH-Phase command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					sprintf( TmpBuffer, "%d\r\n", AnlBlkConfig[Suffix].ScHPhase);
					Udr_PutNChar( TmpBuffer);
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( IntVal != AnlBlkConfig[Suffix].ScHPhase) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						AnlBlkConfig[Suffix].ScHPhase = IntVal;

						TrxAnlBlkScHPhase( Suffix, IntVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBDelay																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the Black Burst timing command.								*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*					ParState 3:  DecNum, DecNum, DecNum									*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void BBDelay() {

	if ( AnlBlkUnit[Suffix].Present) {

		FindDelayParameters();

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					AnlSamplesToFLT( AnlBlkConfig[Suffix].System,\
													  AnlBlkConfig[Suffix].Delay,\
															  &sign, &Field, &Line, &HTime);

					PrintTimingFLT( sign, Field, Line, HTime);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState < 3)
					SCPIError = MissingPar;
				else {
					UlongVal = AnlFLTToSamples(\
									AnlBlkConfig[Suffix].System, Field, Line, HTime);

					if ( UlongVal != -1) {
						if ( UlongVal != AnlBlkConfig[Suffix].Delay) {

							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							AnlBlkConfig[Suffix].Delay = UlongVal;

							TrxAnlBlkDelay( Suffix, AnlBlkConfig[Suffix].System, UlongVal);
						}
					}
					else
						SCPIError = DataOutOfRange;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBVersion															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Handle the Black Burst version-command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					Request only																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void BBVersion() {

	if ( AnlBlkUnit[Suffix].Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteUnitInformation( AnlBlkUnit[Suffix].HWType, Suffix);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGSuffix														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Get suffix of the Analog-TPG.											*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Suffix																		*/
/***************************************************************************/
void AnlTPGSuffix() {

	switch ( NumSuffix) {
		case 2:
			Suffix = 0;
			break;

		case 5:
			Suffix = 1;
			break;

		default:
			SCPIError = HdrSuffixOutOfRange;
			break;
	}
}
/***************************************************************************/
/*	AnlTPG																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Return the setting of a Analog-TPG.									*/
/*	Remarks:		Request only.																*/
/* Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPG() {

	UC tmp, textEnable;

	if ( AnlTPGUnit[Suffix].Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				tmp = AnlTPGConfig[Suffix].Pattern;	// tmp holds the pattern

				Udr_CPutNChar( AnlPatternPar[tmp].LongName);
				Udr_PutChar( ',');

				tmp = AnlFindPatternType( tmp);		// tmp holds the pattern type
				textEnable = AnlTPGConfig[Suffix].TextEnable;

				if ( tmp == ComplexPattern)
					textEnable &= 0x18;
				else
					textEnable &= 0x07;

				if ( textEnable || ( AnlTPGConfig[Suffix].TextStyle[tmp] & 0xF0))
					tmp = TRUE;
				else
					tmp = FALSE;

				Udr_CPutNChar( OffOnPar[tmp].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( AnlSystemPar[AnlTPGConfig[Suffix].System].LongName);
				Udr_PutChar( ',');

				AnlSamplesToFLT( AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Delay,\
										  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutChar( ',');

				sprintf( TmpBuffer, "%d\r\n", AnlTPGConfig[Suffix].ScHPhase);
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
/*	AnlTPGPattern														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG pattern command.								*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGPattern() {

	if ( AnlTPGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AnlPatternPar);

					if ( UcharVal > AnlVMT01)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( AnlPatternPar[AnlTPGConfig[Suffix].Pattern].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal == AnlValidatePattern( AnlTPGUnit[Suffix].HWType, Suffix,\
													AnlTPGConfig[Suffix].System, UcharVal)) {

						if ( UcharVal != AnlTPGConfig[Suffix].Pattern) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							AnlTPGConfig[Suffix].Pattern = UcharVal;

							TrxAnlTPGPattern( Suffix, UcharVal, AnlTPGAttrib( Suffix, UcharVal));

							ConfigureAnlTPGText( Suffix, UcharVal);
						}
					}
					else
						SCPIError = ExecErr;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGAttribSuffix												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980403	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Get suffix of the Analog-TPG pattern-attribute staircase.	*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		AttribSuffix																*/
/***************************************************************************/
void AnlTPGAttribSuffix() {

	if ( NumSuffix == 10)
		AttribSuffix = 10;
	else
		AttribSuffix = 0;
}

/***************************************************************************/
/*	AnlTPGAttribAPAL													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the Analog TPG pattern-attribute Anti-PAL.				*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					This command only applies for the Philips/FuBK patterns.		*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGAttribAPAL() {

	if ( AnlTPGUnit[Suffix].Present) {
		if ( AttribAntiPAL == AnlValidateAttrib( AnlTPGUnit[Suffix].HWType, Suffix,
			AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Pattern, AttribAntiPAL)) 
			AnlTPGAttribCommon( AttribAntiPAL);
		else
			SCPIError = ExecErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGAttribPLUGE													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG pattern-attribute PLUGE.					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					This command only applies for the Philips patterns.			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGAttribPLUGE() {

	if ( AnlTPGUnit[Suffix].Present) {
		if ( AttribPLUGE == AnlValidateAttrib( AnlTPGUnit[Suffix].HWType, Suffix,
			AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Pattern, AttribPLUGE))
			AnlTPGAttribCommon( AttribPLUGE);
		else
			SCPIError = ExecErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGAttribStair10												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG pattern-attribute Staircase 10step.	*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					This only applies to the Philips patterns.						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGAttribStair10() {

	if ( AnlTPGUnit[Suffix].Present) {
		if ( AttribSuffix == 10) {
			if ( AttribStair10 == AnlValidateAttrib( AnlTPGUnit[Suffix].HWType, Suffix,
				AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Pattern, AttribStair10))
				AnlTPGAttribCommon( AttribStair10);
			else
				SCPIError = ExecErr;
		}
		else
			SCPIError = SyntaxErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGAttribCircles												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980929	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the ANL-TPG pattern-attribute Circles.					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					Circles is only available in the Philips 16:9 pattern			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGAttribCircles() {

	if ( AnlTPGUnit[Suffix].Present) {
		if ( AttribCircles == AnlValidateAttrib( AnlTPGUnit[Suffix].HWType, Suffix,
			AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Pattern, AttribCircles))
			AnlTPGAttribCommon( AttribCircles);
		else
			SCPIError = ExecErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGAttribCommon												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980929				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG pattern-attributes.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					This command only applies for the Philips 4:3-pattern			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGAttribCommon( UC Attribute) {

	UC tmp, pattern;

	pattern = AnlTPGConfig[Suffix].Pattern;

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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				tmp = (( AnlTPGAttrib( Suffix, pattern) & ( 1 << Attribute)) > 0);

				Udr_CPutNChar( OffOnPar[tmp].LongName);
				Udr_PutNewline();
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else {
				tmp = pattern - AnlPhilips4x3;

				if ((( AnlTPGConfig[Suffix].Attrib[tmp] & ( 1 << Attribute)) ^ ( UcharVal << Attribute))) {
					ActivePresetNo = 0;			// Clear possible preset

					if ( Ptr == &status_menu[StatusPresetItem])
						Ptr->DispFunct();

					AnlTPGConfig[Suffix].Attrib[tmp] &= ~( 1 << Attribute);
					AnlTPGConfig[Suffix].Attrib[tmp] |= ( UcharVal << Attribute);

					TrxAnlTPGPattern( Suffix, AnlTPGConfig[Suffix].Pattern,\
														AnlTPGConfig[Suffix].Attrib[tmp]);
				}
			}
		}
	}
}

/***************************************************************************/
/*	AnlTPGTextStringSuffix											 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Get suffix of the Analog-TPG text string suffix.				*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		StringSuffix																*/
/***************************************************************************/
void AnlTPGTextStringSuffix() {

	UC tmp;

	if ( AnlFindPatternType( AnlTPGConfig[Suffix].Pattern) == ComplexPattern)
		tmp = 2;
	else
		tmp = 3;

	if ( NumSuffix > tmp)
		SCPIError = SuffixErr;
	else
		StringSuffix = NumSuffix - 1;
}

/***************************************************************************/
/*	AnlTPGTextString													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG text string command.						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum, Char												*/
/*					ParState 3: DecNum, String												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGTextString() {

	register int i, j;
	UC tmp, offset, noOfChar;
	code UC* CharPtr;

	if ( AnlTPGUnit[Suffix].Present) {

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

					switch ( AnlTPGUnit[Suffix].HWType) {
						case PT8601:
							CharPtr = StdTextCharSet;
							noOfChar = PT860xMaxNoChar;
							break;

						default:
							CharPtr = ExtTextCharSet;
							noOfChar = PT863xMaxNoChar;
							break;
					}

					offset = 0;

					for ( j = i = 0; i < strlen( Cmd); i++, j++) {
						switch (  Cmd[i]) {
							case 14:
								offset = 128;
								i++;
								break;
							case 15:
								offset = 0;
								i++;
								break;
						}

						tmp = Cmd[i] + offset;

						if ( tmp && !CharPtr[(UC) tmp]) {
							SCPIError = DataOutOfRange;
							break;
						}
						else
							Cmd[j] = tmp;
					}
					Cmd[j] = 0;

					if ( strlen( Cmd) > noOfChar)
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

			tmp = 3*AnlFindPatternType( AnlTPGConfig[Suffix].Pattern) + StringSuffix;

			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					i = ( AnlTPGConfig[Suffix].TextEnable & ( 1 << tmp)) > 0;

					Udr_CPutNChar( OffOnPar[i].LongName);
					Udr_PutNChar( ",\"");
					Udr_PutNChar( AnlTPGConfig[Suffix].Text[tmp]);
					Udr_PutChar( '"');
					Udr_PutNewline();
				}
			}
			else {
				switch ( ParState ) {
					case 0:
						SCPIError = MissingPar;
						break;

					case 1:
						if (( AnlTPGConfig[Suffix].TextEnable & ( 1 << tmp)) ^ ( UcharVal << tmp)) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							AnlTPGConfig[Suffix].TextEnable &= ~( 1 << tmp);
							AnlTPGConfig[Suffix].TextEnable |= ( UcharVal << tmp);

							TrxAnlTPGTextLineEnable( Suffix, StringSuffix+1, UcharVal);
						}
						break;

					case 2:
						if ( strcmp( AnlTPGConfig[Suffix].Text[tmp], Cmd)) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							strcpy( AnlTPGConfig[Suffix].Text[tmp], Cmd);
							TrxAnlTPGText( Suffix, StringSuffix+1, Cmd);
						}
						break;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGTextStyle													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980318	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG text style command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					This command only applies for the Philips 4:3-pattern.		*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGTextStyle() {

	UC type, tmp;

	if ( AnlTPGUnit[Suffix].Present) {

		type = AnlFindPatternType( AnlTPGConfig[Suffix].Pattern);

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, StylePar);

					if (( UcharVal > TPGComplexTextStyle) ||
						 (( type == StandardPattern) && ( UcharVal == TPGComplexTextStyle)))
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					tmp = AnlTPGConfig[Suffix].TextStyle[type] & 0x0F;

					Udr_CPutNChar( StylePar[tmp].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != ( AnlTPGConfig[Suffix].TextStyle[type] & 0x0F)) {

						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						UcharVal |= ( AnlTPGConfig[Suffix].TextStyle[type] & 0xF0);
						AnlTPGConfig[Suffix].TextStyle[type] = UcharVal;

						TrxAnlTPGTextStyle( Suffix, AnlTPGConfig[Suffix].Pattern, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGTextPosition												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000512	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG text positioning command.					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum,DecNum												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGTextPosition() {

	UC type;

	if ( AnlTPGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				switch ( ParState)
				{
					case 0:
						ParState++;

						UcharVal = atoi( Cmd);

						if (( UcharVal < AnlXPosMin) || ( UcharVal1 > AnlXPosMax))
							SCPIError = DataOutOfRange;
						break;

					case 1:
						ParState++;

						UcharVal1 = atoi( Cmd);

						if (( UcharVal1 < AnlYPosMin) || ( UcharVal1 > AnlYPosMax))
							SCPIError = DataOutOfRange;
						break;

					default:
						SCPIError = ParNotAllowed;
				}
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {

			type = AnlFindPatternType( AnlTPGConfig[Suffix].Pattern);

			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					sprintf( TmpBuffer, "%d,%d\r\n", 
								AnlTPGConfig[Suffix].TextPos[type][0],
								AnlTPGConfig[Suffix].TextPos[type][1]);
					Udr_PutNChar( TmpBuffer);
				}
			}
			else {
				if ( ParState < 2)
					SCPIError = MissingPar;
				else
					if (( AnlTPGConfig[Suffix].TextStyle[type] & 0x0F) != TPGFreeTextStyle)
						SCPIError = SyntaxErr;	
				else {
					if (( UcharVal != AnlTPGConfig[Suffix].TextPos[type][0]) ||
						 ( UcharVal1 != AnlTPGConfig[Suffix].TextPos[type][1]))
					{
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						AnlTPGConfig[Suffix].TextPos[type][0] = UcharVal;
						AnlTPGConfig[Suffix].TextPos[type][1] = UcharVal1;

						TrxAnlTPGTextPos( Suffix, 
												AnlTPGConfig[Suffix].TextStyle[type],
												UcharVal, UcharVal1);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGTextClock													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980318	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG time/date information.						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGTextClock() {

	UC tmp, type;

	if ( AnlTPGUnit[Suffix].Present) {
		if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

			switch ( ParameterType) {
				case P_NoParameter:
					ParState = 0;
					break;

				case P_Char:
					if ( ParState == 0) {
						ParState++;

						UcharVal = FindParameter( Cmd, TPGTextClockPar);

						if (( UcharVal > TPGTextClockTimeDate) ||
							 ( UcharVal == TPGTextClockDate))
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
				type = AnlFindPatternType( AnlTPGConfig[Suffix].Pattern);

				if ( CmdRequest) {
					if ( ParState)
						SCPIError = ParNotAllowed;
					else {
						tmp = AnlTPGConfig[Suffix].TextStyle[type] >> 4;

						Udr_CPutNChar( TPGTextClockPar[tmp].LongName);
						Udr_PutNewline();
					}
				}
				else {
					if ( ParState == 0)
						SCPIError = MissingPar;
					else {
						if ( UcharVal != ( AnlTPGConfig[Suffix].TextStyle[type] >> 4)) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							AnlTPGConfig[Suffix].TextStyle[type] &= 0x0F;
							AnlTPGConfig[Suffix].TextStyle[type] += ( UcharVal << 4);

							TrxAnlTPGTextStyle( Suffix, AnlTPGConfig[Suffix].Pattern, AnlTPGConfig[Suffix].TextStyle[type]);
						}
					}
				}
			}
		}
		else
			SCPIError = HardwareMissing;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGSystem														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 98021	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG system-command.								*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					This command will possibly change pattern and/or delay if	*/
/*             these are not valid in the selected system						*/
/*	Returns:		--																				*/
/*	Updates:	   --																				*/
/***************************************************************************/
void AnlTPGSystem() {

	UC pattern;

	if ( AnlTPGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AnlSystemPar);

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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( AnlSystemPar[AnlTPGConfig[Suffix].System].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != AnlTPGConfig[Suffix].System) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();
															// Save the updated system
						AnlTPGConfig[Suffix].System = UcharVal;

															// Test if delay/pattern is valid
															//  in the new system, (AFTER
															//  the system is saved)
						AnlValidateSetting( AnlTPGUnit[Suffix].HWType, Suffix);
															// Get validated pattern
						pattern = AnlTPGConfig[Suffix].Pattern;

						TrxAnlTPGSystem( Suffix, UcharVal, AnlTPGConfig[Suffix].Delay,\
												 pattern, AnlTPGAttrib( Suffix, pattern));
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGDelay															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG delay command.								*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*					ParState 3:  DecNum, DecNum, DecNum									*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGDelay() {

	if ( AnlTPGUnit[Suffix].Present) {

		FindDelayParameters();

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					AnlSamplesToFLT( AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Delay,\
										  &sign, &Field, &Line, &HTime);

					PrintTimingFLT( sign, Field, Line, HTime);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState < 3)
					SCPIError = MissingPar;
				else {
					UlongVal = AnlFLTToSamples( AnlTPGConfig[Suffix].System,\
															 Field, Line, HTime);

					if ( UlongVal != -1) {
						if ( UlongVal != AnlTPGConfig[Suffix].Delay) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							AnlTPGConfig[Suffix].Delay = UlongVal;

							TrxAnlTPGDelay( Suffix, AnlTPGConfig[Suffix].System, UlongVal);
						}
					}
					else
						SCPIError = DataOutOfRange;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGScHPhase														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG ScH-Phase-command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGScHPhase() {

	if ( AnlTPGUnit[Suffix].Present) {

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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					sprintf( TmpBuffer, "%d\r\n", AnlTPGConfig[Suffix].ScHPhase);
					Udr_PutNChar( TmpBuffer);
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( IntVal != AnlTPGConfig[Suffix].ScHPhase) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						AnlTPGConfig[Suffix].ScHPhase = IntVal;

						TrxAnlTPGScHPhase( Suffix, IntVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AnlTPGVersion														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Handle the Analog-TPG version command.								*/
/*	Remarks:		Request only																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AnlTPGVersion() {

	if ( AnlTPGUnit[Suffix].Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteUnitInformation( AnlTPGUnit[Suffix].HWType, Suffix);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITSGSuffix														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Get suffix of the SDI TSG.												*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Suffix																		*/
/***************************************************************************/
void SDITSGSuffix() {

	switch ( NumSuffix) {
		case 2:
			Suffix = 0;
			break;

		case 3:
			Suffix = 1;
			break;

		case 4:
			Suffix = 2;
			break;

		default:
			SCPIError = HdrSuffixOutOfRange;
			break;
	}
}

/***************************************************************************/
/*	SDITSG																		 RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Return the setting of a SDI-TSG.										*/
/* Remarks:		Request only.																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITSG() {

	if ( SDITSGUnit[Suffix].Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				Udr_CPutNChar( SDIPatternPar[SDITSGConfig[Suffix].Pattern].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( SDISystemPar[SDITSGConfig[Suffix].System].LongName);
				Udr_PutChar( ',');

				SDISamplesToFLT( SDITSGConfig[Suffix].System,\
													  SDITSGConfig[Suffix].Delay,\
															  &sign, &Field, &Line, &HTime);

				PrintTimingFLT( sign, Field, Line, HTime);
				Udr_PutChar( ',');

				Udr_CPutNChar( OffOnPar[SDITSGConfig[Suffix].EDH].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( TSGAudioSignalPar[SDITSGConfig[Suffix].AudioSignal].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( TSGAudioLevelPar[SDITSGConfig[Suffix].AudioLevel].LongName);
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
/*	SDITSGPattern														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TSG pattern command.									*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITSGPattern() {

	if ( SDITSGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, SDIPatternPar);

					if ( UcharVal > SDIVMT01)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( SDIPatternPar[SDITSGConfig[Suffix].Pattern].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal == SDIValidatePattern( SDITSGUnit[Suffix].HWType, Suffix,\
												 SDITSGConfig[Suffix].System, UcharVal)) {

						if ( UcharVal != SDITSGConfig[Suffix].Pattern) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							SDITSGConfig[Suffix].Pattern = UcharVal;

							TrxSDITSGPattern( Suffix, UcharVal);
						}
					}
					else
						SCPIError = ExecErr;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITSGSystem														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TSG system command.									*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					This command will possibly change pattern and/or delay if	*/
/*             these are not valid in the selected system						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITSGSystem() {

	if ( SDITSGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, SDISystemPar);

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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( SDISystemPar[SDITSGConfig[Suffix].System].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITSGConfig[Suffix].System) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITSGConfig[Suffix].System = UcharVal;

															// Test if delay/pattern is valid
															//  in the new system, (AFTER
															//  the system is saved)
						SDIValidateSetting( SDITSGUnit[Suffix].HWType, Suffix);

						TrxSDITSGSystem( Suffix, UcharVal, SDITSGConfig[Suffix].Delay,\
																SDITSGConfig[Suffix].Pattern);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITSGDelay															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TSG delay command.									*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*					ParState 3:  DecNum, DecNum, DecNum									*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITSGDelay() {

	if ( SDITSGUnit[Suffix].Present) {

		FindDelayParameters();

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					SDISamplesToFLT( SDITSGConfig[Suffix].System,\
														  SDITSGConfig[Suffix].Delay,\
														  &sign, &Field, &Line, &HTime);

					PrintTimingFLT( sign, Field, Line, HTime);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState < 3)
					SCPIError = MissingPar;
				else {
					UlongVal = SDIFLTToSamples(\
							SDITSGConfig[Suffix].System, Field, Line, HTime);

					if ( UlongVal != -1) {
						if ( UlongVal != SDITSGConfig[Suffix].Delay) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							SDITSGConfig[Suffix].Delay = UlongVal;

							TrxSDITSGDelay( Suffix, SDITSGConfig[Suffix].System, UlongVal);
						}
					}
					else
						SCPIError = DataOutOfRange;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITSGEDHInsert													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TSG EDH insertion command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITSGEDHInsert() {

	if ( SDITSGUnit[Suffix].Present) {

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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( OffOnPar[SDITSGConfig[Suffix].EDH].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITSGConfig[Suffix].EDH) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITSGConfig[Suffix].EDH = UcharVal;

						TrxSDITSGEDH( Suffix, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITSGVersion														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Handle the SDI-TSG version command.									*/
/*	Remarks:		Request only																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITSGVersion() {

	if ( SDITSGUnit[Suffix].Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError)
			if ( CmdRequest)
				WriteUnitInformation( SDITSGUnit[Suffix].HWType, Suffix);
			else
				SCPIError = SyntaxErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITSGAudioSignal													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TSG audio-signal command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITSGAudioSignal() {

	if ( SDITSGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, TSGAudioSignalPar);

					if ( UcharVal > TSGAudioStereo1kHz)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( TSGAudioSignalPar[SDITSGConfig[Suffix].AudioSignal].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITSGConfig[Suffix].AudioSignal) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITSGConfig[Suffix].AudioSignal = UcharVal;

						TrxSDITSGAudioSignal( Suffix, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITSGAudioLevel													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TSG audio-level command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITSGAudioLevel() {

	if ( SDITSGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, TSGAudioLevelPar);

					if ( UcharVal > TSGAudioLevel18dB)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( TSGAudioLevelPar[SDITSGConfig[Suffix].AudioLevel].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITSGConfig[Suffix].AudioLevel) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITSGConfig[Suffix].AudioLevel = UcharVal;

						TrxSDITSGAudioLevel( Suffix, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGSuffix														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Get suffix of the SDI-TPG.												*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Suffix																		*/
/***************************************************************************/
void SDITPGSuffix() {

	switch ( NumSuffix) {
		case 1:
			Suffix = 0;
			break;

		case 2:
			Suffix = 1;
			break;

		case 5:
			Suffix = 2;
			break;

		default:
			SCPIError = HdrSuffixOutOfRange;
			break;
	}
}

/***************************************************************************/
/*	SDITPG																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980512				     													*/
/*																									*/
/*	Function:	Return the setting of a SDI-TPG.										*/
/* Remarks:		Request only.																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPG() {

	UC tmp, textEnable;

	if ( SDITPGUnit[Suffix].Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {

				tmp = SDITPGConfig[Suffix].Pattern;	// tmp holds the pattern

				Udr_CPutNChar( SDIPatternPar[tmp].LongName);
				Udr_PutChar( ',');

				tmp = SDIFindPatternType( tmp);		// tmp holds the pattern type
				textEnable = SDITPGConfig[Suffix].TextEnable;

				if ( tmp == ComplexPattern)
					textEnable &= 0x18;
				else
					textEnable &= 0x07;

				if ( textEnable || ( SDITPGConfig[Suffix].TextStyle[tmp] & 0xF0))
					tmp = TRUE;
				else
					tmp = FALSE;

				Udr_CPutNChar( OffOnPar[tmp].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( SDISystemPar[SDITPGConfig[Suffix].System].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( OffOnPar[SDITPGConfig[Suffix].EDH].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( TPGAudioSignalPar[SDITPGConfig[Suffix].AudioSignal].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( TPGAudioLevelPar[SDITPGConfig[Suffix].AudioLevel].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( TPGAudioGroupPar[SDITPGConfig[Suffix].AudioGroup].LongName);
				Udr_PutChar( ',');

				SDISamplesToFLT( SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Delay,\
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
/*	SDITPGPattern														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG pattern command.									*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:	   --																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGPattern() {

	if ( SDITPGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, SDIPatternPar);

					if ( UcharVal > SDIVMT01)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( SDIPatternPar[SDITPGConfig[Suffix].Pattern].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {										// Do pattern exist in system ?
					if ( UcharVal == SDIValidatePattern( SDITPGUnit[Suffix].HWType, Suffix,\
													SDITPGConfig[Suffix].System, UcharVal)) {

						if ( UcharVal != SDITPGConfig[Suffix].Pattern) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							SDITPGConfig[Suffix].Pattern = UcharVal;

							TrxSDITPGPattern( Suffix, UcharVal, SDITPGAttrib( Suffix, UcharVal));

							ConfigureSDITPGText( Suffix, UcharVal);
						}
					}
					else
						SCPIError = ExecErr;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAttribSuffix												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980403	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Get suffix of the SDI-TPG pattern-attribute staircase.		*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		AttribSuffix																*/
/***************************************************************************/
void SDITPGAttribSuffix() {

	if ( NumSuffix == 10)
		AttribSuffix = 10;
	else
		AttribSuffix = 0;
}

/***************************************************************************/
/*	SDITPGAttribAPAL													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG pattern-attribute Anti-PAL.					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAttribAPAL() {

	if ( SDITPGUnit[Suffix].Present) {
		if ( AttribAntiPAL == SDIValidateAttrib( SDITPGUnit[Suffix].HWType, Suffix,
			SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribAntiPAL)) 
			SDITPGAttribCommon( AttribAntiPAL);
		else
			SCPIError = ExecErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAttribPLUGE													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG pattern-attribute PLUGE.						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					PLUGE is only available in the Philips pattern					*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAttribPLUGE() {

	if ( SDITPGUnit[Suffix].Present) {
		if ( AttribPLUGE == SDIValidateAttrib( SDITPGUnit[Suffix].HWType, Suffix,
			SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribPLUGE))
			SDITPGAttribCommon( AttribPLUGE);
		else
			SCPIError = ExecErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAttribStair10												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG pattern-attribute Staircase 10step.		*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					Staircase 10 is only available in the Philips pattern			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAttribStair10() {

	if ( SDITPGUnit[Suffix].Present) {
		if ( AttribSuffix == 10) {
			if ( AttribStair10 == SDIValidateAttrib( SDITPGUnit[Suffix].HWType, Suffix,
				SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribStair10))
				SDITPGAttribCommon( AttribStair10);
			else
				SCPIError = ExecErr;
		}
		else
			SCPIError = SyntaxErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAttribMotion												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG pattern-attribute Motion.						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAttribMotion() {

	if ( SDITPGUnit[Suffix].Present) {
		if ( AttribMotion == SDIValidateAttrib( SDITPGUnit[Suffix].HWType, Suffix,
			SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribMotion))
			SDITPGAttribCommon( AttribMotion);
		else
			SCPIError = ExecErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAttribCircles												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG pattern-attribute Circles.					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					Circles is only available in the Philips 16:9 pattern			*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAttribCircles() {

	if ( SDITPGUnit[Suffix].Present) {
		if ( AttribCircles == SDIValidateAttrib( SDITPGUnit[Suffix].HWType, Suffix,
			SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribCircles))
			SDITPGAttribCommon( AttribCircles);
		else
			SCPIError = ExecErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAttribLogo													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990305				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG pattern-attribute Logo.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAttribLogo() {

	if ( SDITPGUnit[Suffix].Present) {
		if ( AttribLogo == SDIValidateAttrib( SDITPGUnit[Suffix].HWType, Suffix,
			SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribLogo))
			SDITPGAttribCommon( AttribLogo);
		else
			SCPIError = ExecErr;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAttribCommon												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980625				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG pattern attributes.								*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAttribCommon( UC Attribute) {

	UC tmp, pattern;

	pattern = SDITPGConfig[Suffix].Pattern;

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
			if ( ParState)
				SCPIError = ParNotAllowed;
			else {
				tmp = (( SDITPGAttrib( Suffix, pattern) & ( 1 << Attribute)) > 0);

				Udr_CPutNChar( OffOnPar[tmp].LongName);
				Udr_PutNewline();
			}
		}
		else {
			if ( ParState == 0)
				SCPIError = MissingPar;
			else {
				tmp = pattern - SDIPhilips4x3;

				if ((( SDITPGConfig[Suffix].Attrib[tmp] & ( 1 << Attribute)) ^ ( UcharVal << Attribute))) {
					ActivePresetNo = 0;			// Clear possible preset

					if ( Ptr == &status_menu[StatusPresetItem])
						Ptr->DispFunct();

					SDITPGConfig[Suffix].Attrib[tmp] &= ~( 1 << Attribute);
					SDITPGConfig[Suffix].Attrib[tmp] |= ( UcharVal << Attribute);

					TrxSDITPGPattern( Suffix, SDITPGConfig[Suffix].Pattern,\
														SDITPGConfig[Suffix].Attrib[tmp]);
				}
			}
		}
	}
}

/***************************************************************************/
/*	SDITPGTextStringSuffix											 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Get suffix of the SDI-TPG text string.								*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		StringSuffix																*/
/***************************************************************************/
void SDITPGTextStringSuffix() {

	UC tmp;

	if ( SDIFindPatternType( SDITPGConfig[Suffix].Pattern) == ComplexPattern)
		tmp = 2;
	else
		tmp = 3;

	if ( NumSuffix > tmp)
		SCPIError = SuffixErr;
	else
		StringSuffix = NumSuffix - 1;
}

/***************************************************************************/
/*	SDITPGTextString													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG test string command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum, Char												*/
/*					ParState 3: DecNum, String												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGTextString() {

	register int i, j;
	UC tmp, offset, noOfChar;
	code UC* CharPtr;

	if ( SDITPGUnit[Suffix].Present) {

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

					switch ( SDITPGUnit[Suffix].HWType) {
						case PT8602:
						case PT8603:
							CharPtr = StdTextCharSet;
							noOfChar = PT860xMaxNoChar;
							break;

						default:
							CharPtr = ExtTextCharSet;
							noOfChar = PT863xMaxNoChar;
							break;
					}

					offset = 0;

					for ( j = i = 0; i < strlen( Cmd); i++, j++) {
						switch (  Cmd[i]) {
							case 14:
								offset = 128;
								i++;
								break;
							case 15:
								offset = 0;
								i++;
								break;
						}

						tmp = Cmd[i] + offset;

						if ( tmp && !CharPtr[(UC) tmp]) {
							SCPIError = DataOutOfRange;
							break;
						}
						else
							Cmd[j] = tmp;
					}
					Cmd[j] = 0;

					if ( strlen( Cmd) > noOfChar)
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

			tmp = 3*SDIFindPatternType( SDITPGConfig[Suffix].Pattern) + StringSuffix;

			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					i = ( SDITPGConfig[Suffix].TextEnable & ( 1 << tmp)) > 0;

					Udr_CPutNChar( OffOnPar[i].LongName);
					Udr_PutNChar( ",\"");
					Udr_PutNChar( SDITPGConfig[Suffix].Text[tmp]);
					Udr_PutChar( '"');
					Udr_PutNewline();
				}
			}
			else {
				switch ( ParState) {
					case 0:
						SCPIError = MissingPar;
						break;

					case 1:
						if (( SDITPGConfig[Suffix].TextEnable & ( 1 << tmp)) ^ ( UcharVal << tmp)) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							SDITPGConfig[Suffix].TextEnable &= ~( 1 << tmp);
							SDITPGConfig[Suffix].TextEnable |= ( UcharVal << tmp);

							TrxSDITPGTextLineEnable( Suffix, StringSuffix+1, UcharVal);
						}
						break;

					case 2:
						if ( strcmp( SDITPGConfig[Suffix].Text[tmp], Cmd)) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							strcpy( SDITPGConfig[Suffix].Text[tmp], Cmd);
							TrxSDITPGText( Suffix, StringSuffix+1, Cmd);
						}
						break;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGTextStyle													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980318	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG text-style command.								*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGTextStyle() {

	UC type, tmp;

	if ( SDITPGUnit[Suffix].Present) {
		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, StylePar);

					if (( UcharVal > TPGComplexTextStyle) ||
						 (( type == StandardPattern) && ( UcharVal == TPGComplexTextStyle)))
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

			type = SDIFindPatternType( SDITPGConfig[Suffix].Pattern);

			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else
				{
					tmp = SDITPGConfig[Suffix].TextStyle[type] & 0x0F;

					Udr_CPutNChar( StylePar[tmp].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != ( SDITPGConfig[Suffix].TextStyle[type] & 0x0F)) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						UcharVal |= ( SDITPGConfig[Suffix].TextStyle[type] & 0xF0);
						SDITPGConfig[Suffix].TextStyle[type] = UcharVal;

						TrxSDITPGTextStyle( Suffix, SDITPGConfig[Suffix].Pattern, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGTextPositioning											 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 000512	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG text-positioning command.						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: DecNum,DecNum												*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGTextPosition() {

	UC type, tmp;

	if ( SDITPGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				switch ( ParState)
				{
					case 0:
						ParState++;

						UcharVal = atoi( Cmd);

						if (( UcharVal < SDIXPosMin) || ( UcharVal1 > SDIXPosMax))
							SCPIError = DataOutOfRange;
						break;

					case 1:
						ParState++;

						UcharVal1 = atoi( Cmd);

						if (( UcharVal1 < SDIYPosMin) || ( UcharVal1 > SDIYPosMax))
							SCPIError = DataOutOfRange;
						break;

					default:
						SCPIError = ParNotAllowed;
				}
				break;

			default:
				SCPIError = DataTypeErr;
				break;
		}

		if ( CmdExecute && !SCPIError) {

			type = SDIFindPatternType( SDITPGConfig[Suffix].Pattern);

			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					sprintf( TmpBuffer, "%d,%d\r\n", 
								SDITPGConfig[Suffix].TextPos[type][0],
								SDITPGConfig[Suffix].TextPos[type][1]);
					Udr_PutNChar( TmpBuffer);
				}
			}
			else {
				if ( ParState < 2)
					SCPIError = MissingPar;
				else
					if (( SDITPGConfig[Suffix].TextStyle[type] & 0x0F) != TPGFreeTextStyle)
						SCPIError = SyntaxErr;	
				else {
					if (( UcharVal != SDITPGConfig[Suffix].TextPos[type][0]) ||
						 ( UcharVal1 != SDITPGConfig[Suffix].TextPos[type][1]))
					{
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITPGConfig[Suffix].TextPos[type][0] = UcharVal;
						SDITPGConfig[Suffix].TextPos[type][1] = UcharVal1;

						TrxSDITPGTextPos( Suffix,
												SDITPGConfig[Suffix].TextStyle[type],
											   UcharVal, UcharVal1);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGTextClock													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980318	    										*/
/* Revised:		000608				     													*/
/*																									*/
/*	Function:	Handle the SDI- TPG clock command.									*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGTextClock() {

	UC tmp, type;

	if ( SDITPGUnit[Suffix].Present) {

		if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

			switch ( ParameterType) {
				case P_NoParameter:
					ParState = 0;
					break;

				case P_Char:
					if ( ParState == 0) {
						ParState++;

						UcharVal = FindParameter( Cmd, TPGTextClockPar);

						if (( UcharVal > TPGTextClockTimeDate) || ( UcharVal == TPGTextClockDate))
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
				type = SDIFindPatternType( SDITPGConfig[Suffix].Pattern);

				if ( CmdRequest) {
					if ( ParState)
						SCPIError = ParNotAllowed;
					else {
						tmp = SDITPGConfig[Suffix].TextStyle[type] >> 4;

						Udr_CPutNChar( TPGTextClockPar[tmp].LongName);
						Udr_PutNewline();
					}
				}
				else {
					if ( ParState == 0)
						SCPIError = MissingPar;
					else {
						if ( UcharVal != ( SDITPGConfig[Suffix].TextStyle[type] >> 4)) {
							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							SDITPGConfig[Suffix].TextStyle[type] &= 0x0F;
							SDITPGConfig[Suffix].TextStyle[type] += ( UcharVal << 4);

							TrxSDITPGTextStyle( Suffix, SDITPGConfig[Suffix].Pattern, SDITPGConfig[Suffix].TextStyle[type]);
						}
					}
				}
			}
		}
		else
			SCPIError = HardwareMissing;
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGSystem														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG system command.									*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					This command will possibly change pattern and/or delay if	*/
/*             these are not valid in the selected system						*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGSystem() {

	UC pattern;

	if ( SDITPGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, SDISystemPar);

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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( SDISystemPar[SDITPGConfig[Suffix].System].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITPGConfig[Suffix].System) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITPGConfig[Suffix].System = UcharVal;

															// Test if delay/pattern is valid
															//  in the new system, (AFTER
															//  the system is saved)
						SDIValidateSetting( SDITPGUnit[Suffix].HWType, Suffix);
															// Get validated pattern
						pattern = SDITPGConfig[Suffix].Pattern;

						TrxSDITPGSystem( Suffix, UcharVal, SDITPGConfig[Suffix].Delay,\
											  pattern, SDITPGAttrib( Suffix, pattern));
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGEDHInsert													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG EDH insertion command.							*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGEDHInsert() {

	if ( SDITPGUnit[Suffix].Present) {

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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( OffOnPar[SDITPGConfig[Suffix].EDH].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITPGConfig[Suffix].EDH) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();
						SDITPGConfig[Suffix].EDH = UcharVal;

						TrxSDITPGEDH( Suffix, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGDelay															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the SDI-TPG delay command.									*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, DecNum											*/
/*					ParState 3:  DecNum, DecNum, DecNum									*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGDelay() {

	if ( SDITPGUnit[Suffix].Present) {

		FindDelayParameters();

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					SDISamplesToFLT( SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Delay,\
												  &sign, &Field, &Line, &HTime);

					PrintTimingFLT( sign, Field, Line, HTime);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState < 3)
					SCPIError = MissingPar;
				else {
					UlongVal = SDIFLTToSamples( SDITPGConfig[Suffix].System,\
														 Field, Line, HTime);

					if ( UlongVal != -1) {
						if ( UlongVal != SDITPGConfig[Suffix].Delay) {

							ActivePresetNo = 0;			// Clear possible preset

							if ( Ptr == &status_menu[StatusPresetItem])
								Ptr->DispFunct();

							SDITPGConfig[Suffix].Delay = UlongVal;

							TrxSDITPGDelay( Suffix, SDITPGConfig[Suffix].System, UlongVal);
						}
					}
					else
						SCPIError = DataOutOfRange;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGVersion														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Return the version of a SDI-TPG.										*/
/*	Remarks:		Request only																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGVersion() {

	if ( SDITPGUnit[Suffix].Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteUnitInformation( SDITPGUnit[Suffix].HWType, Suffix);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAudioSignal													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980930				     													*/
/*																									*/
/*	Function:	Set the embedded audio signal in a SDI-TPG.						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAudioSignal() {

	if ( SDITPGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, TPGAudioSignalPar);

					if ( UcharVal > TPGAudioDual)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( TPGAudioSignalPar[SDITPGConfig[Suffix].AudioSignal].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITPGConfig[Suffix].AudioSignal) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITPGConfig[Suffix].AudioSignal = UcharVal;

						TrxSDITPGAudioSignal( Suffix, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAudioLevel													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Set the embedded audio level in a SDI-TPG.						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAudioLevel() {

	code UC* tablePtr;

	if ( SDITPGUnit[Suffix].Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, TPGAudioLevelPar);

					tablePtr = FindAudioLevelTable( SDITPGUnit[Suffix].HWType);

					if (( UcharVal > TPGAudioLevel20dB) || ( tablePtr[UcharVal] == NoAudioLevel))
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( TPGAudioLevelPar[SDITPGConfig[Suffix].AudioLevel].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITPGConfig[Suffix].AudioLevel) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITPGConfig[Suffix].AudioLevel = UcharVal;

						TrxSDITPGAudioLevel( Suffix, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDITPGAudioGroup													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Set the embedded audio group in a SDI-TPG.						*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDITPGAudioGroup() {

	if ( SDITPGUnit[Suffix].Present && ( SDITPGUnit[Suffix].HWType == PT8633)) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, TPGAudioGroupPar);

					if ( UcharVal > TPGAudioGroup4)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( TPGAudioGroupPar[SDITPGConfig[Suffix].AudioGroup].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != SDITPGConfig[Suffix].AudioGroup) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						SDITPGConfig[Suffix].AudioGroup = UcharVal;

						TrxSDITPGAudioGroup( Suffix, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AESEBUSuffix														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980215	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Get suffix of the AES-EBU Audio Generator.						*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Suffix																		*/
/***************************************************************************/
void AESEBUSuffix() {

	switch ( NumSuffix) {
		case 1:
			Suffix = 0;
			break;

		case 2:
			if ( AESEBUUnit.HWType == PT8635) {
				Suffix = 1;
				break;
			}

		default:
			SCPIError = HdrSuffixOutOfRange;
			break;
	}
}

/***************************************************************************/
/*	AESEBU																 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980211	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the AES-EBU Audio generator command.						*/
/* Remarks:		Request only																*/
/*	Returns:	   --																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AESEBU() {

	if ( AESEBUUnit.Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( AESEBUSignalPar[AESEBUConfig[Suffix].Signal].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( AESEBULevelPar[AESEBUConfig[Suffix].Level].LongName);
				Udr_PutChar( ',');

				Udr_CPutNChar( AESEBUTimingPar[AESEBUConfig[Suffix].Timing].LongName);
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
/*	AESEBUSignal														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980215	    										*/
/* Revised:		980930				     													*/
/*																									*/
/*	Function:	Handle the AES-EBU Audio Generator audio-signal command.		*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AESEBUSignal() {

	if ( AESEBUUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AESEBUSignalPar);

					if (( UcharVal < AESEBUStereo800Hz) || ( UcharVal > AESEBUWordClock))
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( AESEBUSignalPar[AESEBUConfig[Suffix].Signal].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != AESEBUConfig[Suffix].Signal) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						AESEBUConfig[Suffix].Signal = UcharVal;
						AESEBUUnitDrv( Suffix, UcharVal, AESEBUConfig[Suffix].Level,\
																 AESEBUConfig[Suffix].Timing);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AESEBULevel															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980215	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Set the audio level in a PT8605/35 AES-EBU Audio generator.	*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					-14 dB is not allowed in PT8635										*/
/*					-15 dB is not allowed in PT8605										*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AESEBULevel() {

	if ( AESEBUUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AESEBULevelPar);

					if ( UcharVal > AESEBULevel20dB)
						SCPIError = IllegalParValue;

					if ( AESEBUUnit.HWType == PT8635) {
						if ( UcharVal == AESEBULevel14dB)
							SCPIError = IllegalParValue;
					}
					else {
						if ( UcharVal == AESEBULevel15dB)
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
			if ( CmdRequest) {
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( AESEBULevelPar[AESEBUConfig[Suffix].Level].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != AESEBUConfig[Suffix].Level) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						AESEBUConfig[Suffix].Level = UcharVal;

						AESEBUUnitDrv( Suffix, AESEBUConfig[Suffix].Signal, UcharVal,\
																AESEBUConfig[Suffix].Timing);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AESEBUTiming														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980215	    										*/
/* Revised:		980611				     													*/
/*																									*/
/*	Function:	Handle the AES-EBU Audio generator audio-level command.		*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void AESEBUTiming() {

	if ( AESEBUUnit.Present) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, AESEBUTimingPar);

					if ( UcharVal > AESEBUNTSCPhase5)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( AESEBUTimingPar[AESEBUConfig[Suffix].Timing].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( UcharVal != AESEBUConfig[Suffix].Timing) {
						ActivePresetNo = 0;			// Clear possible preset

						if ( Ptr == &status_menu[StatusPresetItem])
							Ptr->DispFunct();

						AESEBUConfig[Suffix].Timing = UcharVal;
						AESEBUUnitDrv( Suffix, AESEBUConfig[Suffix].Signal,\
													AESEBUConfig[Suffix].Level, UcharVal);
					}
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	AESEBUVersion														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980215	    										*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Handle the AES-EBU Audio generator version-command.			*/
/*	Remarks:		Request only																*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void AESEBUVersion() {

	if ( AESEBUUnit.Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteUnitInformation( AESEBUUnit.HWType, 0);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	TimeClock															 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980413    											*/
/* Revised:		980728				     													*/
/*																									*/
/*	Function:	Handle the Time Clock request command								*/
/* Remarks:    Request only                                                */
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeClock() {

	UC tmp;

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				Udr_CPutNChar( TimeClockDFormatPar[TimeClockConfig.Format >> 1].LongName);
				Udr_PutChar( ',');

				tmp = ( TimeClockConfig.Date >> 16) & 0xFF;

				sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
				Udr_PutNChar( TmpBuffer);
				Udr_PutChar( ',');

				tmp = ( TimeClockConfig.Date >> 8) & 0xFF;
				sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
				Udr_PutNChar( TmpBuffer);
				Udr_PutChar( ',');

				tmp = TimeClockConfig.Date & 0xFF;
				sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
				Udr_PutNChar( TmpBuffer);
				Udr_PutChar( ',');

				Udr_CPutNChar( TimeClockTFormatPar[TimeClockConfig.Format & 0x01].LongName);
				Udr_PutChar( ',');

				tmp = ( TimeClockConfig.Time >> 16) & 0xFF;

				sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
				Udr_PutNChar( TmpBuffer);
				Udr_PutChar( ',');

				tmp = ( TimeClockConfig.Time >> 8) & 0xFF;

				sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
				Udr_PutNChar( TmpBuffer);
				Udr_PutChar( ',');

				tmp = TimeClockConfig.Time & 0xFF;

				sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
				Udr_PutNChar( TmpBuffer);
				Udr_PutChar( ',');

				Udr_CPutNChar( TimeClockRefPar[TimeClockConfig.Reference].LongName);
				Udr_PutChar( ',');

				tmp = 1;

				if (( TimeClockConfig.Offset & 0x10000) == 0x10000)
					TmpBuffer[0] = '-';
				else
					tmp = 0;

				TmpBuffer[tmp] = (( TimeClockConfig.Offset >> 12) & 0x0F) + '0';
				if ( TmpBuffer[tmp] != '0')
					tmp++;
				TmpBuffer[tmp++] = (( TimeClockConfig.Offset >> 8) & 0x0F) + '0';
				TmpBuffer[tmp++] = '.';
				TmpBuffer[tmp++] = (( TimeClockConfig.Offset >> 4) & 0x0F) + '0';
				TmpBuffer[tmp] = 0;

				Udr_PutNChar( TmpBuffer);
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
/*	TimeClockDateFormat												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980413    											*/
/* Revised:		980728				     													*/
/*																									*/
/*	Function:	Handle the Time Clock generator date format command			*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeClockDateFormat() {

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, TimeClockDFormatPar);

					if ( UcharVal > YMDFormat)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( TimeClockDFormatPar[TimeClockConfig.Format >> 1].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					ActivePresetNo = 0;			// Clear possible preset

					if ( Ptr == &status_menu[StatusPresetItem])
						Ptr->DispFunct();

					TimeClockConfig.Format &= 0x01;
					TimeClockConfig.Format |= ( UcharVal << 1);
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	TimeClockDate														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980413    											*/
/* Revised:		980728				     													*/
/*																									*/
/*	Function:	Handle the Time Clock generator date command						*/
/*	Remarks:		ParState 0: Idle															*/
/*					ParState 1: DecNum													   */
/*					ParState 2: DecNum, DecNum											   */
/*					ParState 3: DecNum, DecNum, DecNum 								   */
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeClockDate() {

	UC tmp;

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				UlongVal = 0;
				break;

			case P_DecNum:
				if ( ParState < 3) {
					ParState++;             // Year/Month/Day

					if ( atoi( Cmd) < 100) {
						UcharVal = ( UC) atoi( Cmd);
						UlongVal += ( UcharVal / 10) * 16 + UcharVal % 10;

						if ( ParState < 3)
							UlongVal <<= 8;
					}
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					tmp = ( TimeClockConfig.Date >> 16) & 0xFF;

					sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
					Udr_PutNChar( TmpBuffer);
					Udr_PutChar( ',');

					tmp = ( TimeClockConfig.Date >> 8) & 0xFF;
					sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
					Udr_PutNChar( TmpBuffer);
					Udr_PutChar( ',');

					tmp = TimeClockConfig.Date & 0xFF;
					sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
					Udr_PutNChar( TmpBuffer);

					Udr_PutNewline();
				}
			}
			else {
				if ( ParState < 3)
					SCPIError = MissingPar;
				else {
					if ( ValidDate( UlongVal)) {
						TimeClockConfig.Date = UlongVal;

															// Select TCI date register
						UcharVal = TimeClockConfig.TCIControl & 0x1F;
						UcharVal |= ( TCIDateRegister << 5);

															// Update TCI
						TrxTimeClockData( UcharVal, 3, UlongVal);
					}
					else
						SCPIError = ExecErr;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	TimeClockTimeFormat												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980413    											*/
/* Revised:		980728				     													*/
/*																									*/
/*	Function:	Handle the Time Clock generator time format command			*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeClockTimeFormat() {

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, TimeClockTFormatPar);

					if ( UcharVal > Hour24Format)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( TimeClockTFormatPar[TimeClockConfig.Format & 0x01].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					TimeClockConfig.Format &= 0xFE;
					TimeClockConfig.Format |= UcharVal;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	TimeClockTime														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980413    											*/
/* Revised:		980728				     													*/
/*																									*/
/*	Function:	Handle the Time Clock generator time command						*/
/*	Remarks:		ParState 0: Idle															*/
/*					ParState 1: DecNum													   */
/*					ParState 2: DecNum, DecNum											   */
/*					ParState 3: DecNum, DecNum, DecNum 								   */
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeClockTime() {

	UC tmp;

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				UlongVal = 0;
				break;

			case P_DecNum:
				if ( ParState < 3) {
					ParState++;             // Hour/Minute/Second

					if ( atoi( Cmd) < 60) {
						UcharVal = ( UC) atoi( Cmd);
						UlongVal += ( UcharVal / 10) * 16 + UcharVal % 10;

						if ( ParState < 3)
							UlongVal <<= 8;
					}
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					tmp = ( TimeClockConfig.Time >> 16) & 0xFF;

					sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
					Udr_PutNChar( TmpBuffer);
					Udr_PutChar( ',');

					tmp = ( TimeClockConfig.Time >> 8) & 0xFF;

					sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
					Udr_PutNChar( TmpBuffer);
					Udr_PutChar( ',');

					tmp = TimeClockConfig.Time & 0xFF;

					sprintf( TmpBuffer, "%0d", 10*( tmp >> 4) + ( tmp & 0x0F));
					Udr_PutNChar( TmpBuffer);

					Udr_PutNewline();
				}
			}
			else {
				if ( ParState < 3)
					SCPIError = MissingPar;
				else {
					if ( ValidTime( UlongVal)) {
						TimeClockConfig.Time = UlongVal;

															// Select TCI time register
						UcharVal = TimeClockConfig.TCIControl & 0x1F;
						UcharVal |= ( TCITimeRegister << 5);

															// Update TCI
						TrxTimeClockData( UcharVal, 3, UlongVal);
					}
					else
						SCPIError = ExecErr;
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	TimeClockReference												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980413    											*/
/* Revised:		980728				     													*/
/*																									*/
/*	Function:	Handle the Time Clock generator reference command				*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeClockReference() {

	UC tmp;

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_Char:
				if ( ParState == 0) {
					ParState++;

					UcharVal = FindParameter( Cmd, TimeClockRefPar);

					if ( UcharVal > VideoFieldFreqInput)
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					Udr_CPutNChar( TimeClockRefPar[TimeClockConfig.Reference].LongName);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					TimeClockConfig.Reference = UcharVal;

															// Select TCI time reference register
					tmp = TimeClockConfig.TCIControl & 0x1F;
					tmp |= ( TCITimeRefRegister << 5);

															// Update TCI
					TrxTimeClockData( tmp, 1, UcharVal);
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	TimeClockOffset													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980413    											*/
/* Revised:		980728				     													*/
/*																									*/
/*	Function:	Handle the Time Clock generator offset command					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeClockOffset() {

	UC tmp;
	float tmpFloat;

	if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

		switch ( ParameterType) {
			case P_NoParameter:
				ParState = 0;
				break;

			case P_DecNum:
				if ( ParState == 0) {
					ParState++;

					if ( strlen( Cmd) < 6)
						HTime = (float) atof( Cmd);
					else
						SCPIError = DataOutOfRange;

					if (( HTime > 10.0) || ( HTime < -10.0))
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
				if ( ParState)
					SCPIError = ParNotAllowed;
				else {
					tmp = 1;

					if (( TimeClockConfig.Offset & 0x10000) == 0x10000)
						TmpBuffer[0] = '-';
					else
						tmp = 0;

					TmpBuffer[tmp] = (( TimeClockConfig.Offset >> 12) & 0x0F) + '0';
					if ( TmpBuffer[tmp] != '0')
						tmp++;
					TmpBuffer[tmp++] = (( TimeClockConfig.Offset >> 8) & 0x0F) + '0';
					TmpBuffer[tmp++] = '.';
					TmpBuffer[tmp++] = (( TimeClockConfig.Offset >> 4) & 0x0F) + '0';
					TmpBuffer[tmp] = 0;

					Udr_PutNChar( TmpBuffer);
					Udr_PutNewline();
				}
			}
			else {
				if ( ParState == 0)
					SCPIError = MissingPar;
				else {
					if ( HTime < 0) {
						TimeClockConfig.Offset = 0x10000;
						HTime = -HTime;
					}
					else
						TimeClockConfig.Offset = 0;

					tmpFloat = 10*HTime;
					tmp = ((UC) tmpFloat) % 10;
					TimeClockConfig.Offset += ( tmp << 4);

					tmp = ((UC) HTime) % 10;
					TimeClockConfig.Offset += ( tmp << 8);

					tmp = (UC)( HTime) / 10;
					TimeClockConfig.Offset += ( tmp << 12);

															// Select TCI time offset register
					tmp = TimeClockConfig.TCIControl & 0x1F;
					tmp |= ( TCIOffsetRegister << 5);

															// Update TCI
					TrxTimeClockData( tmp, 3, TimeClockConfig.Offset);
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	TimeClockVersion													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970216    											*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Return the version of the PT8637 Time Clock Interface.		*/
/*	Remarks:		Request only																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void TimeClockVersion() {

	if ( TimeClockUnit.Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				WriteUnitInformation( TimeClockUnit.HWType, 0);
			}
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	BBMultiVersion														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Return thne version of the PT8604 Multiple Parallel BB.		*/
/*	Remarks:		Request only																*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void BBMultiVersion() {

	if ( AnlBlkUnit[BB2Item].HWType == PT8604) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteUnitInformation( AnlBlkUnit[BB2Item].HWType, 0);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	SDIGenlockVersion													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Return thne version of the PT8606 SDI Genlock option.			*/
/*	Remarks:		Request only																*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void SDIGenlockVersion() {

	if ( SDIGenlockUnit.Present) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest)
				WriteUnitInformation( SDIGenlockUnit.HWType, 0);
			else
				SCPIError = SyntaxErr;
		}
	}
	else
		SCPIError = HardwareMissing;
}

/***************************************************************************/
/*	FactoryPassword													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970206	    										*/
/* Revised:		971023				     													*/
/*																									*/
/*	Function:	Read a user password, encode it and test if correct			*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: Char															*/
/*					ParState 2: String														*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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
/*	FactoryAddress														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		9803131				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle	 														*/
/*					ParState 1: DecNum														*/
/*					ParState 2: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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

					if ( UcharVal > 7)
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
/*	FactoryCommand														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		971030				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  String                    		;,?,$				*/
/*					ParState 2:  String, String						;    				*/
/*					ParState 3:  String, DecNum						;,?,($)			*/
/*					ParState 4:  String, DecNum, String				;    				*/
/*					ParState 5:  String, DecNum, DecNum				;				   */
/*					ParState 6:  String, DecNum, DecNum, DecNum	;				   */
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FactoryCommand() {

	char* ascptr;

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

						if ( strlen( Cmd) <= SizeOfTmpBuffer)
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

					case 5:
						ParState = 6;

						if ( strlen( Cmd) < 7)
							IntVal1 = atoi( Cmd);
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
								ascptr = RecAsc( UnitAddress, CmdBuffer, ( UC) UlongVal, &UcharVal);
							else
								ascptr = RecAsc( UnitAddress, CmdBuffer, 0xFF, &UcharVal);

							if ( UcharVal == 0) {
								Udr_PutNChar( ascptr);
								Udr_PutNewline();
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
								UcharVal = SndAsc( UnitAddress, CmdBuffer, 0xFF, TmpBuffer);
								break;

							case 3:
								UcharVal = SndLong( UnitAddress, CmdBuffer, UlongVal);
								break;

							case 4:
								UcharVal = SndAsc( UnitAddress, CmdBuffer, ( UC) UlongVal, TmpBuffer);
								break;

							case 5:
								UcharVal = SndInt2( UnitAddress, CmdBuffer, ( UI) UlongVal, IntVal);
								break;

							case 6:
								UcharVal = SndInt3( UnitAddress, CmdBuffer, ( UI) UlongVal, IntVal, IntVal1);
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
/*	FactorySPGUserText												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970105				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*					Number of characters in string is limited to 10.				*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
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
					IIC2RdStr( spgram_addr, (UC)( SPGUserText_addr+10*UcharVal), TmpBuffer);

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
					IIC2WrStr( spgram_addr, (UC)( SPGUserText_addr+10*UcharVal), TmpBuffer);
				else
					SCPIError = DataTypeErr;
			}
		}
	}
}

/***************************************************************************/
/*	FactorySPGStore													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		971023				     													*/
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

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {

			HPHZero_G = RecLong( sync_addr, "GA", 0xFF, &UcharVal);
			if ( UcharVal == 0)
				GenlPhase_G = RecLong( sync_addr, "GF", 0xFF, &UcharVal);

			if ( UcharVal == 0)
				HPHZero_M = RecLong( sync_addr, "GU", 0xFF, &UcharVal);

			if ( UcharVal == 0)
				GenlPhase_M = RecLong( sync_addr, "GG", 0xFF, &UcharVal);

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
					IIC2WrLongInt( spgram_addr, HPHZero_G_addr, HPHZero_G, GenlPhase_G);
					IIC2WrLongInt( spgram_addr, HPHZero_M_addr, HPHZero_M, GenlPhase_M);
				}
			}
			else
				SCPIError = DeviceSpecificErr;
		}
	}
}

/***************************************************************************/
/*	FactorySPGRead														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		970917				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactorySPGRead() {

	if ( PasswordState == OFF) {

		if ( ParameterType != P_NoParameter)
			SCPIError = ParNotAllowed;

		if ( CmdExecute && !SCPIError) {
			if ( CmdRequest) {
				_ultoa( IIC2RdLong( spgram_addr, HPHZero_G_addr), TmpBuffer);
				Udr_PutNChar( TmpBuffer);

				Udr_PutChar( ',');
				_ultoa( IIC2RdInt( spgram_addr, GenlPhase_G_addr), TmpBuffer);
				Udr_PutNChar( TmpBuffer);

				Udr_PutChar( ',');
				_ultoa( IIC2RdLong( spgram_addr, HPHZero_M_addr), TmpBuffer);
				Udr_PutNChar( TmpBuffer);

				Udr_PutChar( ',');
				_ultoa( IIC2RdInt( spgram_addr, GenlPhase_M_addr), TmpBuffer);
				Udr_PutNChar( TmpBuffer);
				Udr_PutNewline();
			}
		}
	}
}

/***************************************************************************/
/*	FactoryAudioSuffix												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980215	    										*/
/* Revised:		980608				     													*/
/*																									*/
/*	Function:	Get suffix of the AES-EBU Audio Generator							*/
/*	Remarks:		--																				*/
/*	Returns:		--																				*/
/*	Updates:		Suffix																		*/
/***************************************************************************/
void FactoryAudioSuffix() {

	if ( NumSuffix > 2)
		SCPIError = HdrSuffixOutOfRange;
	else
		AudioSuffix = NumSuffix;
}

/***************************************************************************/
/*	FactoryAudioUserText												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980607				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FactoryAudioUserText() {

	if ( PasswordState == OFF) {

		if ( AESEBUUnit.Present) {
			if ( AudioSuffix == 1) {

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
							IIC2RdStr( soundram_addr, (UC)( 10*UcharVal), TmpBuffer);

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
							IIC2WrStr( soundram_addr, (UC)( 10*UcharVal), TmpBuffer);
						else
							SCPIError = DataTypeErr;
					}
				}
			}
			else
				SCPIError = SyntaxErr;
		}
		else
			SCPIError = HardwareMissing;
	}
}

/***************************************************************************/
/*	FactoryAudioRead													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980607				     													*/
/*																									*/
/*	Function:	--																				*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FactoryAudioRead() {

	UC tmp, addr1, addr2;

	if ( PasswordState == OFF) {

		if ( AESEBUUnit.Present) {

			if ( CmdExecute && !SCPIError) {
				if ( CmdRequest) {

					if ( AudioSuffix == 1) {
						addr1 = sound1_addr;
						addr2 = sound2_addr;
					}
					else {
						addr1 = sound3_addr;
						addr2 = sound4_addr;
					}

					tmp = IIC2Read( addr1);

					FeedTheWatchdog;

					_ultoa( tmp, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');

					tmp = IIC2Read( addr2);

					FeedTheWatchdog;

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
/*	FactoryAudioMode													 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980604	    										*/
/* Revised:		980608				     													*/
/*																									*/
/*	Function:   --																				*/
/*	Remarks:		ParState 0: Idle															*/
/*					ParState 1: Char															*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FactoryAudioMode() {

	UC tmp;

	if ( PasswordState == OFF) {

		if ( AESEBUUnit.Present && ( AESEBUUnit.HWType == PT8635)) {

			if ( AudioSuffix == 2) {
				switch ( ParameterType) {
					case P_NoParameter:
						ParState = 0;
						break;

					case P_Char:
						if ( ParState == 0) {
							ParState++;

							UcharVal = FindParameter( Cmd, PT52ModePar);

							if ( UcharVal > 1)
								SCPIError = DataOutOfRange;
						}
						else
							SCPIError = DataTypeErr;
						break;

					default:
						SCPIError = ParNotAllowed;
						break;
				}

				if ( CmdExecute && !SCPIError) {

					if ( CmdRequest) {
						if ( ParState == 0) {

							tmp = !( IIC2Read( sound4_addr) >> 7);

							Udr_CPutNChar( PT52ModePar[tmp].LongName);
							Udr_PutNewline();
						}
						else
							SCPIError = DataTypeErr;
					}
					else {
						tmp = IIC2Read( sound4_addr);

						if ( UcharVal == 0)
							tmp |= 0x80;
						else
							tmp &= 0x7F;

						IIC2Write( sound4_addr, tmp);
					}
				}
			}
			else
				SCPIError = SyntaxErr;
		}
		else
			SCPIError = HardwareMissing;
	}
}

/***************************************************************************/
/*	FactoryMainUserText												 	    RS232CMD.C	*/
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

					IIC2RdStr( mainram1_addr, (UC)( 10*UcharVal), TmpBuffer);

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
					IIC2WrStr( mainram1_addr, (UC)( 10*UcharVal), TmpBuffer);
				else
					SCPIError = DataTypeErr;
			}
		}
	}
}

/***************************************************************************/
/*	FactoryMainPSNSuffix												 	    RS232CMD.C	*/
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
/*	FactoryMainPSN														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970717	    										*/
/* Revised:		980427				     													*/
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

					IIC2Write( mainram1_addr, VoltN5Min_addr);
					UcharVal = IIC2Read( mainram1_addr);

					_ultoa(( UL) UcharVal, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');

					UcharVal = IIC2Read( mainram1_addr);

					_ultoa(( UL) UcharVal, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutNewline();
				}
				else
					SCPIError = DataTypeErr;
			}
			else {
				if ( ParState == 2) {
					IIC2Write2( mainram1_addr, VoltN5Min_addr, UcharVal);
					IIC2Write2( mainram1_addr, VoltN5Max_addr, UcharVal1);

					VoltN5Min = UcharVal;
					VoltN5Max = UcharVal1;
				}
				else
					SCPIError = DataTypeErr;
			}
		}
	}
}

/***************************************************************************/
/*	FactoryMainPSSuffix												 	    RS232CMD.C	*/
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
/*	FactoryMainPS														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970717	    										*/
/* Revised:		980427				     													*/
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

					IIC2Write( mainram1_addr, Volt12Min_addr);
					UcharVal = IIC2Read( mainram1_addr);

					_ultoa(( UL) UcharVal, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutChar( ',');

					UcharVal = IIC2Read( mainram1_addr);

					_ultoa(( UL) UcharVal, TmpBuffer);
					Udr_PutNChar( TmpBuffer);

					Udr_PutNewline();
				}
				else
					SCPIError = DataTypeErr;
			}
			else {
				if ( ParState == 2) {
					IIC2Write2( mainram1_addr, Volt12Min_addr, UcharVal);
					IIC2Write2( mainram1_addr, Volt12Max_addr, UcharVal1);

					Volt12Min = UcharVal;
					Volt12Max = UcharVal1;
				}
				else
					SCPIError = DataTypeErr;
			}
		}
	}
}

/***************************************************************************/
/*	FactoryMainPSCalib												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970820	    										*/
/* Revised:		970827				     													*/
/*																									*/
/*	Function:																					*/
/*	Remarks:		ParState 0: Idle															*/
/*					ParState 1: Char															*/
/*	Returns:																						*/
/*	Updates:																						*/
/***************************************************************************/
void FactoryMainPSCalib() {

	if ( PasswordState == OFF) {

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

			default:
				SCPIError = ParNotAllowed;
				break;
		}

		if ( CmdExecute && !SCPIError) {

			if ( CmdRequest) {
				if ( ParState == 0) {

					IIC2Write( mainram1_addr, VoltCalib_addr);

					if (( UcharVal = IIC2Read( mainram1_addr)) != ON)
						UcharVal = OFF;

					Udr_CPutNChar( OffOnPar[UcharVal].LongName);
					Udr_PutNewline();
				}
				else
					SCPIError = DataTypeErr;
			}
			else {
				IIC2Write2( mainram1_addr, VoltCalib_addr, UcharVal);

				VoltCalib = UcharVal;
			}
		}
	}
}

/***************************************************************************/
/*	FactorySDIGenlockUserText										 	    RS232CMD.C	*/
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

						IIC1RdStr( d1ram_addr, (UC)( 10*UcharVal), TmpBuffer);

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
						IIC1WrStr( d1ram_addr, (UC)( 10*UcharVal), TmpBuffer);
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
/*	FactoryTimeClockUserText										 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970109	    										*/
/* Revised:		980730				     													*/
/*																									*/
/*	Function:	Handle user text for the Time Clock Interface					*/
/*	Remarks:		ParState 0:  Idle															*/
/*					ParState 1:  DecNum														*/
/*					ParState 2:  DecNum, String											*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FactoryTimeClockUserText() {

	if ( PasswordState == OFF) {

		if ( TimeClockUnit.Present) {

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

						IIC1RdStr( timeram_addr, (UC)( 10*UcharVal), TmpBuffer);

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
						IIC1WrStr( timeram_addr, (UC)( 10*UcharVal), TmpBuffer);
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
/*	FactoryTimeClockStatus											 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 980730	    										*/
/* Revised:		980730				     													*/
/*																									*/
/*	Function:	Handle status for the Time Clock Interface						*/
/*	Remarks:		Query only																	*/
/*	Returns:		--																				*/
/*	Updates:		--																				*/
/***************************************************************************/
void FactoryTimeClockStatus() {

	UC tmp;

	if ( PasswordState == OFF) {

		if ( TimeClockUnit.Present && ( TimeClockUnit.HWType == PT8637)) {

			if ( ParameterType != P_NoParameter)
				SCPIError = ParNotAllowed;

			if ( CmdExecute && !SCPIError) {
				if ( !CmdRequest)
					SCPIError = SyntaxErr;
				else {
														// Get TCI error information.
					TimeClockConfig.TCIControl &= 0x1F;
					TimeClockConfig.TCIControl |= ( TCIErrorRegister << 5);
					IIC1Write( time1_addr, TimeClockConfig.TCIControl);

					FeedTheWatchdog;

					IIC1ReadN( time1_addr, 2, ( UC*) TmpBuffer);

					tmp = TmpBuffer[1];

					_ultoa( (UL) tmp, TmpBuffer);
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
/*	FactoryMultiBBUserText											 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970109	    										*/
/* Revised:		980427				     													*/
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

		if ( AnlBlkUnit[BB2Item].HWType == PT8604) {

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

						IIC1RdStr( multibb_addr, (UC)( 10*UcharVal), TmpBuffer);

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
						IIC1WrStr( multibb_addr, (UC)( 10*UcharVal), TmpBuffer);
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
/*	FactoryADCRead														 	    RS232CMD.C	*/
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
/*	FactoryLDetectorRead												 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 970122	    										*/
/* Revised:		970820				     													*/
/*																									*/
/*	Function:	Write content of level detectors to RS232							*/
/*	Remarks:		ParState 0:  Idle															*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
void FactoryLDetectorRead() {

	if ( PasswordState == OFF)

		if ( CmdExecute && CmdRequest && !SCPIError) {

			_ultoa(( LevelStatus & 0x00FF), TmpBuffer);
			Udr_PutNChar( TmpBuffer);

			Udr_PutChar( ',');

			_ultoa(( LevelStatus >> 8), TmpBuffer);
			Udr_PutNChar( TmpBuffer);
			Udr_PutNewline();
		}
}

/***************************************************************************/
/*	FindDelayParameters												 	    RS232CMD.C	*/
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
/*	FindParameter														 	    RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 941124	    										*/
/* Revised:		980518				     													*/
/*																									*/
/*	Function:	Find the character defined parameter								*/
/*	Remarks:		--																				*/
/*	Returns:		The array number containing the parameter or max. array+1	*/
/*	Updates:		--																				*/
/***************************************************************************/
UC FindParameter( char* par, code struct PAR_STRUCT* ParPtr) {

	UC tmp = 0;

	while ( ParPtr) {
		FeedTheWatchdog;

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
/*	WriteUnitInformation											 	    	RS232CMD.C	*/
/*																									*/
/* Author:		Kim Engedahl, DEV, 990317	    										*/
/* Revised:		990317				     													*/
/*																									*/
/*	Function:	Write the companyname, type number, KU number and software	*/
/*					version from a	V24 operated unit to RS232							*/
/*	Remarks:		-																				*/
/*	Returns:		-																				*/
/*	Updates:		-																				*/
/***************************************************************************/
UC WriteUnitInformation( UC HWtype, UC suffix) {

	char* ascptr;

	UC HWversion, SWversion, HWinformation;
	UL KUnumber;

	switch ( HWtype) {
		case PT8601:
		case PT8631:
			ascptr = RecStr( AnlTPGUnit[Suffix].Address, "AR", CompanyName_addr/10, &UcharVal);

			KUnumber = AnlTPGUnit[Suffix].KUNumber;
			HWversion = AnlTPGUnit[Suffix].HWVersion;
			SWversion = AnlTPGUnit[Suffix].SWVersion;
			HWinformation = AnlTPGUnit[Suffix].HWInformation;
			break;

		case PT8602:
		case PT8603:
		case PT8632:
		case PT8633:
			ascptr = RecStr( SDITPGUnit[Suffix].Address, "SR", CompanyName_addr/10, &UcharVal);

			KUnumber = SDITPGUnit[Suffix].KUNumber;
			HWversion = SDITPGUnit[Suffix].HWVersion;
			SWversion = SDITPGUnit[Suffix].SWVersion;
			HWinformation = SDITPGUnit[Suffix].HWInformation;
			break;

		case PT8605:
		case PT8635:
			IIC2RdStr( soundram_addr, CompanyName_addr, TmpBuffer);
			ascptr = TmpBuffer;

			KUnumber = AESEBUUnit.KUNumber;
			HWversion = AESEBUUnit.HWVersion;
			SWversion = AESEBUUnit.SWVersion;
			HWinformation = AESEBUUnit.HWInformation;
			break;

		case PT8606:
			IIC1RdStr( d1ram_addr, CompanyName_addr, TmpBuffer);
			ascptr = TmpBuffer;

			KUnumber = SDIGenlockUnit.KUNumber;
			HWversion = SDIGenlockUnit.HWVersion;
			SWversion = SDIGenlockUnit.SWVersion;
			HWinformation = SDIGenlockUnit.HWInformation;
			break;

		case PT8607:
		case PT8637:
			IIC1RdStr( timeram_addr, CompanyName_addr, TmpBuffer);
			ascptr = TmpBuffer;

			KUnumber = TimeClockUnit.KUNumber;
			HWversion = TimeClockUnit.HWVersion;
			SWversion = TimeClockUnit.SWVersion;
			HWinformation = TimeClockUnit.HWInformation;
			break;

		case PT8604:
		case PT8608:
			if ( HWtype == PT8604) {
				IIC1RdStr( multibb_addr, CompanyName_addr, TmpBuffer);
				ascptr = TmpBuffer;
			}
			else
				ascptr = RecStr( AnlBlkUnit[Suffix].Address, "HR", CompanyName_addr/10, &UcharVal);

			KUnumber = AnlBlkUnit[Suffix].KUNumber;
			HWversion = AnlBlkUnit[Suffix].HWVersion;
			SWversion = AnlBlkUnit[Suffix].SWVersion;
			HWinformation = AnlBlkUnit[Suffix].HWInformation;
			break;

		case PT8609:
		case PT8639:
			ascptr = RecStr( SDITSGUnit[Suffix].Address, "HR", CompanyName_addr/10, &UcharVal);

			KUnumber = SDITSGUnit[Suffix].KUNumber;
			HWversion = SDITSGUnit[Suffix].HWVersion;
			SWversion = SDITSGUnit[Suffix].SWVersion;
			HWinformation = SDITSGUnit[Suffix].HWInformation;
			break;
	}

	if ( UcharVal == 0)
		Udr_PutNChar( ascptr);

	sprintf( TmpBuffer, ",PT86%02d", HWtype);
	Udr_PutNChar( TmpBuffer);

	if ( HWversion) {
		sprintf( TmpBuffer, "_9%02d", HWversion);
		Udr_PutNChar( TmpBuffer);
	}
	else {
		if (( HWtype == PT8632) && ( HWinformation == 1))
			Udr_PutNChar( "_10");
	}

	sprintf( TmpBuffer, ",KU%06.0f", (float) KUnumber);
	Udr_PutNChar( TmpBuffer);

	if ( SWversion) {
		sprintf( TmpBuffer, ",%04.1f", (float) SWversion/10.0);
		Udr_PutNChar( TmpBuffer);
	}
	else
		Udr_PutNChar( ",0");

	Udr_PutNewline();

	return( 0);
}

