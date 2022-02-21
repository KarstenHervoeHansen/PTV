 /***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 2000                 */
/*  Project:    PT5300 Digital Video Generator                             */
/*  Module:    RS232CMD.C                                                  */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  980211                                                     */
/*  Rev. date:  000608, KEn, DEV                                           */
/*  Status:    Version 1.5                                                 */
/*                                                                         */
/*  This module contains the following functions:                          */
/*                                                                         */
/*  Changes:                                                               */
/* 17.01.2007: Added support for TLG, HD, DL and PT8633 (text only)        */
/* 000508: Added support for TPG text positioning                          */
/* 991101: Customer complaint: SYST:PRES:DOWN didn't work correctly        */
/*         Download revision has changed in order to prevent download from */
/*         previous versions.                                              */
/* 991026: Implemented "DIAG:ERR"                                          */
/* 990420: Added Window 10% & 15 kHz Bl/Wh in both SDI & Analog.           */
/* 990317: Replaced WriteIICVersion() & WriteV24Version() with WriteUnit-  */
/*         Information().                                                  */
/* 990317: Revised WriteV24Version() to include /900 numbers etc.          */
/* 990305: Added calls SDIValidateAttrib() &  AnlValidateAttrib()          */
/*         Error in SystemPresetName() not all characters was legal        */
/* 981001: CBEBu8 var skrevet med lille u.                                 */
/* 980930: Removed MBBC1KHZParTxt altogether.                              */
/* 980929: Added AnlTPGAttribCircles(). Increased AnlTPGLevel3 with 1.     */
/* 980925: Coorections in xxTPGAttribCommon()                              */
/* 980917: Added new SDI pattern texts. In SDITPGAudioSignal() &           */
/*         AESBUSignal(): separate test for MBBC1KHZParTxt                 */
/* 980917: Added Circle 4:3/16:9 and Crosshatch 16:9.                      */
/* 980915: SYNCLOCKED bliver nu returneret for genlock status i Genlock()  */
/* 980730: Added FACT:TIM:STATus for Time Clock Interface                  */
/* 980728: Final implementation of Time Clock Interface                    */
/* 980702: Update TimeClockRefPar                                          */
/* 980616: Released as 4008 002 06812                                      */
/* 980618: Minor corrections                                               */
/* 980611: Update errorcodes in functions                                  */
/* 980604: Added FactoryAudioMode()                                        */
/* 980523: Limited no of characters in ...TPGTextString() to 8 for the     */
/*         PT860x series and to 48 for the PT863x series.                  */
/* 980518: In FindParameter(): Added feed_dog;                             */
/* 980514: Released as 4008 002 06811                                      */
/***************************************************************************/

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
#include "text.h"
#include "menutree.h"
#include "menu.h"
#include "timing.h"
#include "iic.h"
#include "message.h"
#include "cont_drv.h"
#include "keyb_drv.h"
#include "factory.h"

#include "disp_drv.h"

#define SizeOfTmpBuffer    30          // The max. number of characters the
                                       //  general purpose buffer can hold
#define DownloadOffset    (UC)  20
#define DownloadRevision  (UC)   2      // Download stream revision

#define PT860xMaxNoChar    (UC)  8
#define PT863xMaxNoChar    (UC) 16
#define PT8603MaxNoChar    (UC) 32

code char SystemVersionTxt[]       = "1995.0";
code char FactoryPasswordTxt[]     = "CWh_bod"; /* The password is Marilyn */
                                                /* :FACT:PASS 'Marilyn';   */

code char IDNResponseCmdTxt[]        = "*IDN?";
code char DownloadResponseCmdTxt[]   = ":SYST:PRES:DOWN ";

code char NoErrorsTxt[]          = "No errors";
code char ActiveErrorsTxt[]      = "Active error";
code char NoActiveErrorsTxt[]    = "No active error";

code char CLSCmdTxt[]           = "CLS";
code char ESECmdTxt[]           = "ESE";
code char ESRCmdTxt[]           = "ESR";
code char OPCCmdTxt[]           = "OPC";
code char SRECmdTxt[]           = "SRE";
code char STBCmdTxt[]           = "STB";
code char TSTCmdTxt[]           = "TST";
code char WAICmdTxt[]           = "WAI";
code char RSTCmdTxt[]           = "RST";
code char IDNCmdTxt[]           = "IDN";

code char LTCLevelCmdTxt[]		= "LTCGENERATOR";
code char LTCCmdTxt[]			= "LTCG";

code char TRILEVELCmdTxt[]      = "TRILEVEL";
code char TLGCmdTxt[]           = "TLG";

code char HDLEVELCmdTxt[]       = "HDSDI";
code char HDCmdTxt[]            = "HD";

code char DLLEVELCmdTxt[]       = "DLSDI";
code char DLCmdTxt[]            = "DL";

code char SYSTEMCmdTxt[]         = "SYSTEM";
code char SYSTCmdTxt[]           = "SYST";
code char STATUSCmdTxt[]         = "STATUS";
code char STATCmdTxt[]           = "STAT";
code char PTCmdTxt[]             = "PT";
code char DIAGNOSTICCmdTxt[]     = "DIAGNOSTIC";
code char DIAGCmdTxt[]           = "DIAG";
code char ERRORQUEUECmdTxt[]     = "ERRORQUEUE";
code char LASTCmdTxt[]           = "LAST";
code char ENTRYCmdTxt[]          = "ENTRY";
code char ENTRCmdTxt[]           = "ENTR";
code char RESETCmdTxt[]          = "RESET";
code char RESCmdTxt[]            = "RES";
code char DISPLAYCmdTxt[]        = "DISPLAY";
code char DISPCmdTxt[]           = "DISP";
code char INPUTCmdTxt[]          = "INPUT";
code char INPCmdTxt[]            = "INP";
code char OUTPUTCmdTxt[]         = "OUTPUT";
code char OUTPCmdTxt[]           = "OUTP";
code char FACTORYCmdTxt[]        = "FACTORY";
code char FACTCmdTxt[]           = "FACT";

code char ERRORCmdTxt[]          = "ERROR";
code char ERRCmdTxt[]            = "ERR";
code char VERSIONCmdTxt[]        = "VERSION";
code char VERSCmdTxt[]           = "VERS";
code char DOWNLOADCmdTxt[]       = "DOWNLOAD";
code char DOWNCmdTxt[]           = "DOWN";
code char UPLOADCmdTxt[]         = "UPLOAD";
code char UPLCmdTxt[]            = "UPL";
code char NAMECmdTxt[]           = "NAME";
code char NAMCmdTxt[]            = "NAM";
code char RECALLCmdTxt[]         = "RECALL";
code char RECCmdTxt[]            = "REC";
code char DEBUGCmdTxt[]			 = "DEBUG";

code char OPERATIONCmdTxt[]      = "OPERATION";
code char OPERCmdTxt[]           = "OPER";
code char QUESTIONABLECmdTxt[]   = "QUESTIONABLE";
code char QUESCmdTxt[]           = "QUES";
code char PRESETCmdTxt[]         = "PRESET";
code char PRESCmdTxt[]           = "PRES";

code char EVENTCmdTxt[]          = "EVENT";
code char EVENCmdTxt[]           = "EVEN";
code char CONDITIONCmdTxt[]      = "CONDITION";
code char CONDCmdTxt[]           = "COND";
code char ENABLECmdTxt[]         = "ENABLE";
code char ENABCmdTxt[]           = "ENAB";

code char CONTRASTCmdTxt[]       = "CONTRAST";
code char CONTCmdTxt[]           = "CONT";

code char GENLOCKCmdTxt[]        = "GENLOCK";
code char GENLCmdTxt[]           = "GENL";

code char BBCmdTxt[]             = "BB";
code char BBMULTICmdTxt[]        = "BBMULTI";
code char BBMCmdTxt[]            = "BBM";
code char DELAYCmdTxt[]          = "DELAY";
code char DELCmdTxt[]            = "DEL";
code char SCHPHASECmdTxt[]       = "SCHPHASE";
code char SCHPCmdTxt[]           = "SCHP";

code char ATPGENERATORCmdTxt[]   = "ATPGENERATOR";
code char ATPGCmdTxt[]           = "ATPG";
code char PATTERNCmdTxt[]        = "PATTERN";
code char PATTCmdTxt[]           = "PATT";
code char TEXTCmdTxt[]           = "TEXT";
code char STRINGCmdTxt[]         = "STRING";
code char STRCmdTxt[]            = "STR";
code char STYLECmdTxt[]          = "STYLE";
code char STYLCmdTxt[]           = "STYL";
code char POSITIONCmdTxt[]       = "POSITION";
code char POSCmdTxt[]            = "POS";
code char CLOCKCmdTxt[]          = "CLOCK";
code char CLOCCmdTxt[]           = "CLOC";

code char STSGENERATORCmdTxt[]   = "STSGENERATOR";
code char STSGCmdTxt[]           = "STSG";

code char EDHINSERTCmdTxt[]      = "EDHINSERT";
code char EDHCmdTxt[]            = "EDH";
code char EMBAUDIOCmdTxt[]       = "EMBAUDIO";
code char EMBCmdTxt[]            = "EMB";

code char STPGENERATORCmdTxt[]   = "STPGENERATOR";
code char STPGCmdTxt[]           = "STPG";
code char MODIFYCmdTxt[]         = "MODIFY";

code char INTERFACECmdTxt[]      = "INTERFACE";
code char INTERCmdTxt[]          = "INTERF";

code char MODCmdTxt[]            = "MOD";
code char APALCmdTxt[]           = "APAL";      // Anti-PAL
code char STAIRCASECmdTxt[]      = "STAIRCASE";    // Staircase 10step
code char STACmdTxt[]            = "STA";
code char MOTIONCmdTxt[]         = "MOTION";      // Motion
code char MOTCmdTxt[]            = "MOT";
code char CIRCLESCmdTxt[]        = "CIRCLES";    // Circles
code char CIRCCmdTxt[]           = "CIRC";
code char LOGOCmdTxt[]           = "LOGO";      // Logo
code char LOGCmdTxt[]            = "LOG";

code char AUDIOCmdTxt[]          = "AUDIO";
code char AUDCmdTxt[]            = "AUD";
code char SIGNALCmdTxt[]         = "SIGNAL";
code char SIGNCmdTxt[]           = "SIGN";
code char LEVELCmdTxt[]          = "LEVEL";
code char LEVCmdTxt[]            = "LEV";

code char CLICKCmdTxt[]          = "CLICK";
code char CLICmdTxt[]            = "CLI";

code char GROUPCmdTxt[]          = "GROUP";
code char GROCmdTxt[]            = "GRO";
code char TIMINGCmdTxt[]         = "TIMING";
code char TIMCmdTxt[]            = "TIM";

code char TIMECLOCKCmdTxt[]      = "TIMECLOCK";
code char DFORMATCmdTxt[]        = "DFORMAT";
code char DFORCmdTxt[]           = "DFOR";
code char DATECmdTxt[]           = "DATE";
code char DATCmdTxt[]            = "DAT";
code char TFORMATCmdTxt[]        = "TFORMAT";
code char TFORCmdTxt[]           = "TFOR";
code char TIMECmdTxt[]           = "TIME";
code char REFERENCECmdTxt[]      = "REFERENCE";
code char REFCmdTxt[]            = "REF";
code char OFFSETCmdTxt[]         = "OFFSET";
code char OFFSCmdTxt[]           = "OFFS";
code char FORMATCmdTxt[]		 = "FORMAT";
code char FORMCmdTxt[]			 = "FORM";
code char TIMEZONECmdTxt[]		 = "TIMEZONE";
code char TIMEZCmdTxt[]			 = "TIMEZ";
code char DAYLIGHTCmdTxt[]		 = "DAYLIGHT";
code char DAYLCmdTxt[]			 = "DAYL";
code char STARTCmdTxt[]			 = "START";
code char ENDCmdTxt[]			 = "END";


code char PASSWORDCmdTxt[]       = "PASSWORD";
code char PASSCmdTxt[]           = "PASS";
code char V24CIRCUITCmdTxt[]     = "V24CIRCUIT";
code char V24CCmdTxt[]           = "V24C";
code char ADDRESSCmdTxt[]        = "ADDRESS";
code char ADDRCmdTxt[]           = "ADDR";
code char COMMANDCmdTxt[]        = "COMMAND";
code char COMMCmdTxt[]           = "COMM";

code char SPGGENERATORCmdTxt[]   = "SPGGENERATOR";
code char SPGGCmdTxt[]           = "SPGG";
code char TEXTSTRINGCmdTxt[]     = "TEXTSTRING";
code char STORECmdTxt[]          = "STORE";
code char STORCmdTxt[]           = "STOR";
code char READCmdTxt[]           = "READ";
code char WRITECmdTxt[]          = "WRITE";
code char WRITCmdTxt[]           = "WRIT";
code char SDIGENLOCKCmdTxt[]     = "SDIGENLOCK";
code char SDIGCmdTxt[]           = "SDIG";
code char MAINBOARDCmdTxt[]      = "MAINBOARD";
code char MAINCmdTxt[]           = "MAIN";
code char PSN5CmdTxt[]           = "PSN";
code char PS12CmdTxt[]           = "PS";
code char PSCALIBCmdTxt[]        = "PSCALIB";
code char PSCCmdTxt[]            = "PSC";
code char MULTIBBCmdTxt[]        = "MULTIBB";
code char MULTCmdTxt[]           = "MULT";
code char ADCCmdTxt[]            = "ADC";
code char LDETECTORCmdTxt[]      = "LDETECTOR";
code char LDETCmdTxt[]           = "LDET";
code char MODECmdTxt[]           = "MODE";
code char SYNCCmdTxt[]			 = "SYNC";

code char AParTxt[]              = "A";
code char BParTxt[]              = "B";
code char ABParTxt[]             = "A_B";
code char SDIParTxt[]            = "SDI";
code char INTERNALParTxt[]       = "INTERNAL";
code char INTParTxt[]            = "INT";
code char INTERNAL2ParTxt[]      = "INTERNAL2";
code char INT2ParTxt[]           = "INT2";

code char PALBURSTParTxt[]       = "PALBURST";
code char PALBParTxt[]           = "PALB";
code char NTSCBURSTParTxt[]      = "NTSCBURST";
code char NTSCParTxt[]           = "NTSC";
code char SYNC625ParTxt[]        = "SYNC625";
code char SYNC525ParTxt[]        = "SYNC525";
code char SDI625ParTxt[]         = "SDI625";
code char SDI525ParTxt[]         = "SDI525";
code char F358MHzParTxt[]        = "F358MHZ";
code char F443MHzParTxt[]        = "F443MHZ";
code char F5MHzParTxt[]          = "F5MHZ";
code char F10MHzParTxt[]         = "F10MHZ";

code char PALParTxt[]            = "PAL";
code char PAL_IDParTxt[]         = "PAL_ID";

code char CBSMPTEParTxt[]        = "CBSMPTE";    // SMPTE Colorbar
code char CBSMParTxt[]           = "CBSM";
code char CBEBUParTxt[]          = "CBEBU";        // EBU Colorbar
code char CBEBParTxt[]           = "CBEB";
code char CBFCCParTxt[]          = "CBFCC";      // FCC Colorbar
code char CBFCParTxt[]           = "CBFC";
code char CBEBU8ParTxt[]         = "CBEBU8";      // Colorbar ITU801
code char CBEB8ParTxt[]          = "CBEB8";
code char CB100ParTxt[]          = "CB100";      // 100% Colorbar
code char CBGREY75ParTxt[]       = "CBGREY75";    // Colorbar + 75% Grey
code char CBGR75ParTxt[]         = "CBGR75";
code char CBRED75ParTxt[]        = "CBRED75";    // Colorbar + 75% Red
code char CBR75ParTxt[]          = "CBR75";
code char RED75ParTxt[]          = "RED75";      // 75% Red

code char MULTIBURSTParTxt[]     = "MULTIBURST";  // Multi burst
code char MULTParTxt[]           = "MULT";
code char LSWEEPParTxt[]         = "LSWEEP";      // Luminance Sweep
code char LSWParTxt[]            = "LSW";
code char YCRCBSWEEPParTxt[]     = "YCRCBSWEEP";   // Y,Cr,Cb Sweep
code char YCRCParTxt[]           = "YCRC";
code char MPULSEParTxt[]         = "MPULSE";      // Multi pulse
code char MPULParTxt[]           = "MPUL";
code char SINXXParTxt[]          = "SINXX";        // Sinx/x
code char SINXParTxt[]           = "SINX";
code char CCIR18ParTxt[]         = "CCIR18";      // CCIR 18
code char NCMBParTxt[]           = "NCMB";      // NTC7 Combination
code char FCCMBURSTParTxt[]      = "FCCMBURST";    // FCC Multiburst
code char FCCMParTxt[]           = "FCCM";

code char WIN10ParTxt[]          = "WIN10";      // Window 10%
code char WIN15ParTxt[]          = "WIN15";      // Window 15%
code char WIN20ParTxt[]          = "WIN20";      // Window 20%
code char WIN100ParTxt[]         = "WIN100";      // Window 100%
code char BLWH15ParTxt[]         = "BLWH15KHZ";    // 15kHz Bl/Wh
code char GREY50ParTxt[]         = "GREY50";      // Grey 50%
code char WHITE100ParTxt[]       = "WHITE100";    // White 100%
code char WHIT100ParTxt[]        = "WHIT100";
code char BLACKBURSTParTxt[]     = "BLACKBURST";  // Black burst
code char BLACKParTxt[]          = "BLACK";      // Black
code char BLACParTxt[]           = "BLAC";

code char SDICHECKParTxt[]       = "SDICHECK";    // Check field
code char SDICParTxt[]           = "SDIC";
code char DTIMINGParTxt[]        = "DTIMING";      // Digital Timing
code char DTIMParTxt[]           = "DTIM";
code char FDTESTParTxt[]         = "FDTEST";      // Field Delay Test
code char FDTParTxt[]            = "FDT";
code char BOWTIEParTxt[]         = "BOWTIE";      // Bow Tie
code char BOWTParTxt[]           = "BOWT";
code char ABLANKINGParTxt[]      = "ABLANKING";    // Analog Blanking
code char ABLParTxt[]            = "ABL";
code char DGREYParTxt[]          = "DGREY";      // Digital grey
code char DGRParTxt[]            = "DGR";
code char FSWAVEParTxt[]         = "FSWAVE";      // Field Sqare Wave
code char FSWParTxt[]            = "FSW";
code char BLWH01ParTxt[]         = "BLWH01";      // 0.1Hz Bl/Wh
code char CUSTOM1ParTxt[]        = "CUSTOM1";    // Customized 1
code char CUST1ParTxt[]          = "CUST1";      // Customized 1
code char CUSTOM2ParTxt[]        = "CUSTOM2";    // Customized 2
code char CUST2ParTxt[]          = "CUST2";
code char CUSTOM3ParTxt[]        = "CUSTOM3";    // Customized 3
code char CUST3ParTxt[]          = "CUST3";
code char CUSTOM4ParTxt[]        = "CUSTOM4";    // Customized 4
code char CUST4ParTxt[]          = "CUST4";
code char CUSTOM5ParTxt[]        = "CUSTOM5";    // Customized 5
code char CUST5ParTxt[]          = "CUST5";
code char CUSTOM6ParTxt[]        = "CUSTOM6";    // Customized 6
code char CUST6ParTxt[]          = "CUST6";
code char CUSTOM7ParTxt[]        = "CUSTOM7";    // Customized 7
code char CUST7ParTxt[]          = "CUST7";
code char CUSTOM8ParTxt[]        = "CUSTOM8";    // Customized 8
code char CUST8ParTxt[]          = "CUST8";
code char EOLINEParTxt[]         = "EOLINE";      // End-of-line pulses
code char EOLParTxt[]            = "EOL";
code char WEOLINEParTxt[]        = "WEOLINE";    // White, end-of-line porches
code char WEOLParTxt[]           = "WEOL";
code char BEOLINEParTxt[]        = "BEOLINE";    // Blue, do.
code char BEOLParTxt[]           = "BEOL";
code char REOLINEParTxt[]        = "REOLINE";    // Red, do.
code char REOLParTxt[]           = "REOL";
code char YEOLINEParTxt[]        = "YEOLINE";    // Yellow, do.
code char YEOLParTxt[]           = "YEOL";
code char CEOLINEParTxt[]        = "CEOLINE";    // Cyan, do.
code char CEOLParTxt[]           = "CEOL";

code char SHALLOWRAMParTxt[]     = "SHALLOWRAMP";  // Shallow Ramp
code char SHALParTxt[]           = "SHAL";
code char SRAMPParTxt[]          = "SRAMP";      // Shallow Ramp
code char SRAMParTxt[]           = "SRAM";
code char RAMPParTxt[]           = "RAMP";      // Ramp, (luminance)
code char LRAMPParTxt[]          = "LRAMP";      // Limit Ramp
code char LRAMParTxt[]           = "LRAM";
code char VRAMPParTxt[]          = "VRAMP";      // Valid Ramp
code char VRAMParTxt[]           = "VRAM";
code char MRAMPParTxt[]          = "MRAMP";      // Modulated Ramp
code char MRAMParTxt[]           = "MRAM";
code char STAIRCASE5ParTxt[]     = "STAIRCASE5";  // 5-step staircase
code char STA5ParTxt[]           = "STA5";
code char MSTAIRCASE5ParTxt[]    = "MSTAIRCASE5";  // Modulated 5-step staircase
code char MSTA5ParTxt[]          = "MSTA5";
code char STAIRCASE10ParTxt[]    = "STAIRCASE10";  // 10-step staircase
code char STA10ParTxt[]          = "STA10";
code char PBARParTxt[]           = "PBAR";      // Pulse & Bar
code char CCIR17ParTxt[]         = "CCIR17";      // CCIR line 17
code char CCIR330ParTxt[]        = "CCIR330";    // CCIR line 330
code char CCIR331ParTxt[]        = "CCIR331";    // CCIR line 331
code char FCCCompositeParTxt[]   = "FCCCOMPOSITE";  // FCC Composite
code char FCCCParTxt[]           = "FCCC";
code char NCMPParTxt[]           = "NCMP";      // NTC-7 Composite
code char YGRAMPParTxt[]         = "YGRAMP";      // Yellow/Grey Ramp
code char YGRParTxt[]            = "YGR";
code char GBRAMPParTxt[]         = "GBRAMP";      // Grey/Blue Ramp
code char GBRParTxt[]            = "GBR";
code char CGRAMPParTxt[]         = "CGRAMP";      // Cyan/Grey Ramp
code char CGRParTxt[]            = "CGR";
code char GRRAMPParTxt[]         = "GRRAMP";      // Grey/Red Ramp
code char GRRParTxt[]            = "GRR";
code char CBYCRYRAMPParTxt[]     = "CBYCRYRAMP";  // Cb,Y,Cr,Y Ramp
code char CBYCParTxt[]           = "CBYC";

code char PHILIPS43ParTxt[]      = "PHILIPS43";    // Philips 4:3
code char PHIL43ParTxt[]         = "PHIL43";
code char PHILIPS169ParTxt[]     = "PHILIPS169";  // Philips 16:9
code char PHIL169ParTxt[]        = "PHIL169";
code char FUBK43ParTxt[]         = "FUBK43";      // FuBK 4:3
code char FUBK169ParTxt[]        = "FUBK169";    // FuBK 16:9
code char CROSSHATCHParTxt[]     = "CROSSHATCH";  // Cross hatch
code char CROSParTxt[]           = "CROS";
code char CROSSHATCH169ParTxt[]  = "CROSSHATCH169";// Cross hatch 16:9
code char CROS169ParTxt[]        = "CROS169";
code char CIRCLE43ParTxt[]       = "CIRCLE43";    // Circle 4:3
code char CIRC43ParTxt[]         = "CIRC43";      //
code char CIRCLE169ParTxt[]      = "CIRCLE169";    // Circle 16:9
code char CIRC169ParTxt[]        = "CIRC169";    // 
code char PLUGEParTxt[]          = "PLUGE";      // PLUGE
code char PLUGParTxt[]           = "PLUG";
code char SAFEAREAParTxt[]       = "SAFEAREA";    // Safe area
code char SAFParTxt[]            = "SAF";
code char SWAVE250KHZParTxt[]    = "SWAVE250KHZ";  // 250kHz
code char SWAV250KHZParTxt[]     = "SWAV250KHZ";
code char VMT01ParTxt[]          = "VMT01";      // VMT 01

code char S800HZParTxt[]         = "S800HZ";      // Stereo 800 Hz
code char S1KHZParTxt[]          = "S1KHZ";       // Stereo 1 kHz
code char SEBU1KHZParTxt[]       = "SEBU1KHZ";    // Stereo EBU 1 kHz
code char SBBC1KHZParTxt[]       = "SBBC1KHZ";    // Stereo BBC 1 kHz
code char M1KHZParTxt[]          = "M1KHZ";      // Mono 1 kHz
code char MEBU1KHZParTxt[]       = "MEBU1KHZ";    // Mono EBU 1 kHz
code char DUALParTxt[]           = "DUAL";      // Dual sound
code char WORDCLOCKParTxt[]      = "WORDCLOCK";    // WordClock (48kHz)
code char WORDParTxt[]           = "WORD";

code char SILENCEParTxt[]        = "SILENCE";
code char SILParTxt[]            = "SIL";
code char SineParTxt[]           = "SINE";
code char ClickParTxt[]          = "CLICK";

code char DB0FSParTxt[]          = "DB0FS";
code char DB6FSParTxt[]          = "DB6FS";
code char DB9FSParTxt[]          = "DB9FS";
code char DB12FSParTxt[]         = "DB12FS";
code char DB14FSParTxt[]         = "DB14FS";
code char DB15FSParTxt[]         = "DB15FS";
code char DB16FSParTxt[]         = "DB16FS";
code char DB18FSParTxt[]         = "DB18FS";
code char DB20FSParTxt[]         = "DB20FS";
code char DB24FSParTxt[]         = "DB24FS";

code char NTSC1ParTxt[]          = "NTSC1";
code char NTSC2ParTxt[]          = "NTSC2";
code char NTSC3ParTxt[]          = "NTSC3";
code char NTSC4ParTxt[]          = "NTSC4";
code char NTSC5ParTxt[]          = "NTSC5";

code char GROUP1ParTxt[]         = "GROUP1";
code char GRO1ParTxt[]           = "GRO1";
code char GROUP2ParTxt[]         = "GROUP2";
code char GRO2ParTxt[]           = "GRO2";
code char GROUP3ParTxt[]         = "GROUP3";
code char GRO3ParTxt[]           = "GRO3";
code char GROUP4ParTxt[]         = "GROUP4";
code char GRO4ParTxt[]           = "GRO4";

code char DMYParTxt[]            = "DMY";
code char MDYParTxt[]            = "MDY";
code char YMDParTxt[]            = "YMD";

code char H12ParTxt[]            = "HOUR12";
code char H24ParTxt[]            = "HOUR24";
code char VITCParTxt[]           = "VITC";
code char LTCParTxt[]            = "LTC";
code char VFFREQUENCYParTxt[]    = "VFFREQUENCY";
code char VFFRParTxt[]           = "VFFR";
code char REF1HZParTxt[]         = "REF1HZ";

code char BBU_0ParTxt[]          = "BBU_0";
code char BBU_1ParTxt[]          = "BBU_1";
code char BBU_2ParTxt[]          = "BBU_2";
code char BBU_3ParTxt[]          = "BBU_3";
code char SPGParTxt[]            = "SPG";
code char ANLSIGNALParTxt[]      = "ANLSIGNAL";
code char ANLSParTxt[]           = "ANLS";
code char SDISIGNALParTxt[]      = "SDISIGNAL";
code char SDISParTxt[]           = "SDIS";
code char SDITPGParTxt[]         = "SDITPG";
code char SDITParTxt[]           = "SDIT";

code char OFFParTxt[]            = "OFF";
code char ONParTxt[]             = "ON";

code char MINParTxt[]            = "MIN";
code char MAXParTxt[]            = "MAX";

code char FREEParTxt[]           = "FREE";
code char STANDARDParTxt[]       = "STANDARD";
code char STANParTxt[]           = "STAN";
code char COMPLEXParTxt[]        = "COMPLEX";
code char COMPParTxt[]           = "COMP";

code char PT5210ParTxt[]         = "PT5210";
code char PT5300ParTxt[]         = "PT5300";

code char TIMEParTxt[]           = "TIME";
code char TIMParTxt[]            = "TIM";
code char DATEParTxt[]           = "DATE";
code char DATParTxt[]            = "DAT";
code char DTIMEParTxt[]          = "DTIME";


/* PATTERNS FOR HD */
code char BllTxt[]               = "BLACK";
code char CflTxt[]               = "SDICHECK";
code char PllTxt[]               = "PLUGE";
code char LrlTxt[]               = "LRAMP";
code char CllTxt[]               = "CLAPPERBRD";
code char CblTxt[]               = "COLORBAR";
code char ColTxt[]               = "COMBINATION";
code char WilTxt[]               = "WINDOW";
code char CrlTxt[]               = "CROSSHATCH";
code char WhlTxt[]               = "WHITE";

code char BlsTxt[]               = "BLAC";
code char CfsTxt[]               = "SDIC";
code char PlsTxt[]               = "PLUG";
code char LrsTxt[]               = "LRAM";
code char ClsTxt[]               = "CLAP";
code char CbsTxt[]               = "CB";
code char CosTxt[]               = "COMBI";
code char WisTxt[]               = "WIN";
code char CrsTxt[]               = "CROS";
code char WhsTxt[]               = "WHIT";

/* Text for HD */
code char MOVEMENTCmdTxt[]       = "MOVEMENT";
code char SCALECmdTxt[]          = "SCALE";
code char COLORCmdTxt[]          = "COLOR";
code char BACKGRGRCmdTxt[]       = "BACKGROUND";
code char MOVCmdTxt[]            = "MOV";  
code char SCACmdTxt[]            = "SCA";
code char COLCmdTxt[]            = "COL";
code char BACGCmdTxt[]           = "BACK";


/* Text colors for HD */
code char WHITETxt[]             = "WHITE";
code char YELLOWTxt[]            = "YELLOW";
code char CYANTxt[]              = "CYAN";
code char GREENTxt[]             = "GREEN";
code char MAGENTATxt[]           = "MAGENTA";
code char REDTxt[]               = "RED";
code char BLUETxt[]              = "BLUE";
code char BLACKTxt[]             = "BLACK";
                                 
code char WHIxt[]                = "WHI";
code char YELTxt[]               = "YEL";
code char CYATxt[]               = "CYA";
code char GRETxt[]               = "GRE";
code char MAGTxt[]               = "MAG";
code char BLUxt[]                = "BLU";
code char BLATxt[]               = "BLA";

/* Text movement for HD */
code char HorizontalTxt[]        = "HORIZONTAL";
code char VeritcalTxt[]          = "VERTICAL";
code char BothTxt[]              = "BOTH";
code char HorizTxt[]             = "HOR";
code char VertTxt[]              = "VER";

/* Attribs for CB in HD */
code char CBATTTxt0[]            = "100100";
code char CBATTTxt1[]            = "10075";
code char CBATTTxt2[]            = "7575";


/* Attribs for HD patterns */
code char AttCmdTxt0[]           = "HH";
code char AttCmdTxt1[]           = "HS";
code char AttCmdTxt2[]           = "SS";
code char AttCmdTxt3[]           = "AM5";   
code char AttCmdTxt4[]           = "A0";   
code char AttCmdTxt5[]           = "A5";   
code char AttCmdTxt6[]           = "A10";   
code char AttCmdTxt7[]           = "A15";   
code char AttCmdTxt8[]           = "A20";   
code char AttCmdTxt9[]           = "A25";   
code char AttCmdTxt10[]          = "A30";   
code char AttCmdTxt11[]          = "A35";   
code char AttCmdTxt12[]          = "A40";   
code char AttCmdTxt13[]          = "A45";   
code char AttCmdTxt14[]          = "A50";   
code char AttCmdTxt15[]          = "A55";   
code char AttCmdTxt16[]          = "A60";   
code char AttCmdTxt17[]          = "A65";   
code char AttCmdTxt18[]          = "A70";   
code char AttCmdTxt19[]          = "A75";   
code char AttCmdTxt20[]          = "A80";   
code char AttCmdTxt21[]          = "A85";   
code char AttCmdTxt22[]          = "A90";   
code char AttCmdTxt23[]          = "A95";   
code char AttCmdTxt24[]          = "A100";   
code char AttCmdTxt25[]          = "A105";   

/* Interface for DL System */
code char Inter1[]               = "I1"; //"S422YCBCR10BIT";
code char Inter2[]               = "I2"; //"D4224YCBCRA12BIT";
code char Inter3[]               = "I3"; //"D444YCBCR10BIT";
code char Inter4[]               = "I4"; //"D4444YCBCRA12BIT";
code char Inter5[]               = "I5"; //"D444GBR10BIT";
code char Inter6[]               = "I6"; //"D4444GBRA12BIT";

/* Interface for LTC generators */
code char LTC24FPS[]			= "24FPS";
code char LTC25FPS[]			= "25FPS";
code char LTC2997DROP[]			= "2997DROP";
code char LTC2997NOND[]			= "2997NOND";
code char LTC30FPS[]			= "30FPS";

code char LTCNONE[]				= "NONE";
code char LTCCONF[]				= "CONF";
code char LTCAUTO[]				= "AUTO";

LTC_STRUCT tmp_ltc;

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

code struct CMD_STRUCT Subsystem[7] =
  {
    { SYSTEMCmdTxt,     SYSTCmdTxt, &Subsystem[1], &System[0],     NONE, NONE},
    { STATUSCmdTxt,     STATCmdTxt, &Subsystem[2], &Status[0],     NONE, NONE},
    { DIAGNOSTICCmdTxt, DIAGCmdTxt, &Subsystem[3], &Diagnostic[0], NONE, NONE},
    { DISPLAYCmdTxt,    DISPCmdTxt, &Subsystem[4], &Display[0],    NONE, NONE},
    { INPUTCmdTxt,      INPCmdTxt,  &Subsystem[5], &Input[0],      NONE, NONE},
    { OUTPUTCmdTxt,     OUTPCmdTxt, &Subsystem[6], &Output[0],     NONE, NONE},
    { FACTORYCmdTxt,    FACTCmdTxt, NULL,          &Factory[0],    NONE, NONE},
  };

 
code struct CMD_STRUCT System[5] =
  {
    { ERRORCmdTxt,    ERRCmdTxt,  &System[1], NULL, NONE, SystemError},
    { VERSIONCmdTxt,  VERSCmdTxt, &System[2], NULL, NONE, SystemVersion},
    { PRESETCmdTxt,   PRESCmdTxt, &System[3], &SystemLevel1[0], NONE, SystemPreset},
    { DOWNLOADCmdTxt, DOWNCmdTxt, &System[4], NULL, NONE, SystemDownload},
    { UPLOADCmdTxt,   UPLCmdTxt,   NULL,      NULL, NONE, SystemUpload}
  };

 
code struct CMD_STRUCT SystemLevel1[5] = {
  { RECALLCmdTxt,   RECCmdTxt,  &SystemLevel1[1], NULL, NONE, SystemPreset},
  { STORECmdTxt,    STORCmdTxt, &SystemLevel1[2], NULL, NONE, SystemPresetStore},
  { NAMECmdTxt,     NAMCmdTxt,  &SystemLevel1[3], NULL, NONE, SystemPresetName},
  { DOWNLOADCmdTxt, DOWNCmdTxt, &SystemLevel1[4], NULL, NONE, SystemPresetDownload},
  { UPLOADCmdTxt,   UPLCmdTxt,   NULL,            NULL, NONE, SystemPresetUpload}
};

 
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


code struct CMD_STRUCT Diagnostic[2] = {
  { DISPLAYCmdTxt, DISPCmdTxt, &Diagnostic[1], NULL, NONE, DiagnosticDisplay},
  { ERRORQUEUECmdTxt, ERRCmdTxt, NULL, &DiagnosticLevel1[0], NONE, DiagnosticErrorQueueLast},
};


code struct CMD_STRUCT DiagnosticLevel1[3] = {
  { LASTCmdTxt, LASTCmdTxt, &DiagnosticLevel1[1], NULL, NONE, DiagnosticErrorQueueLast},
  { ENTRYCmdTxt, ENTRCmdTxt, &DiagnosticLevel1[2], NULL, NONE, DiagnosticErrorQueueEntry},
  { RESETCmdTxt, RESCmdTxt, NULL, NULL, NONE, DiagnosticErrorQueueReset},
};


code struct CMD_STRUCT Display[1] = {
  { CONTRASTCmdTxt, CONTCmdTxt, NULL, NULL, NONE, DisplayContrast},
};

 
code struct CMD_STRUCT Input[2] = {
  { GENLOCKCmdTxt, GENLCmdTxt, &Input[1], &GenlockLevel1[0], NONE, Genlock},
  { SDIGENLOCKCmdTxt, SDIGCmdTxt, NULL, &GenlockLevel2[0], NONE, NONE}
};

 
code struct CMD_STRUCT GenlockLevel1[3] = {
  { INPUTCmdTxt, INPCmdTxt, &GenlockLevel1[1], NULL, NONE, GenlockInput},
  { SYSTEMCmdTxt, SYSTCmdTxt, &GenlockLevel1[2], NULL, NONE, GenlockSystem},
  { DELAYCmdTxt, DELCmdTxt, NULL, NULL, NONE, GenlockDelay},
};

code struct CMD_STRUCT GenlockLevel2[1] = {
  { VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, SDIGenlockVersion}
};

//********                                               ******************
/*
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
*/

// :OUTP:LTCG#:OFFSET (offset)
// :OUTP:LTCG#:FORMAT (format), (syncmode)
// :OUTP:LTCG#:TIME:TIMEZONE (hours), (mins)
// :OUTP:LTCG#:TIME:DAYLIGHT:START (date), (hour)
// :OUTP:LTCG#:TIME:DAYLIGHT:END (date), (hour)
// :OUTP:LTCG#:TIME:DAYLIGHT:MODE (mode), (state)

//:OUTP
code struct CMD_STRUCT Output[11] =
  {
    { BBCmdTxt,           BBCmdTxt,   &Output[1],   &BBLevel1[0],        BBSuffix,     BB},
    { ATPGENERATORCmdTxt, ATPGCmdTxt, &Output[2],   &AnlTPGLevel1[0],    AnlTPGSuffix, AnlTPG},
    { STSGENERATORCmdTxt, STSGCmdTxt, &Output[3],   &SDITSGLevel1[0],    SDITSGSuffix, SDITSG},
    { STPGENERATORCmdTxt, STPGCmdTxt, &Output[4],   &SDITPGLevel1[0],    SDITPGSuffix, SDITPG},
    { AUDIOCmdTxt,        AUDCmdTxt,  &Output[5],   &AESEBULevel1[0],    AESEBUSuffix, AESEBU},
    { TIMECLOCKCmdTxt,    TIMCmdTxt,  &Output[6],   &TimeClockLevel1[0], NONE,         TimeClock},
    { TRILEVELCmdTxt,     TLGCmdTxt,  &Output[7],   &TLGLevel1[0],       TLGSuffix,    TLG},
    { HDLEVELCmdTxt,      HDCmdTxt,   &Output[8],   &HDLevel1[0],        HDSuffix,     HD},
    { DLLEVELCmdTxt,      DLCmdTxt,   &Output[9],   &DLLevel1[0],        DLSuffix,     DL},
    { BBMULTICmdTxt,      BBMCmdTxt,  &Output[10],  &BBMultiLevel1[0],   NONE,         NONE},
    { LTCLevelCmdTxt,	  LTCCmdTxt,  &Output[11],	&LTCLevel1[0],		 LTCSuffix,	   LTC}	,
    { LTCLevelCmdTxt,	  LTCCmdTxt,  NULL,			&LTCPT8620_Level1[0],		 NONE,	   LTC}	
  };


code struct CMD_STRUCT LTCPT8620_Level1[5] = 
  {
	  {SYNCCmdTxt,	SYNCCmdTxt, &LTCLevel1[1], 	NULL, NONE, LTCFormat},
	  {TIMEParTxt,		TIMEParTxt, &LTCLevel1[2], 	&LTCTimeLevel[0], NONE, LTCTime},
	  {VERSIONCmdTxt,	VERSCmdTxt,  &LTCLevel1[4],	NULL, NONE, LTCVersion},
	  {DEBUGCmdTxt,		DEBUGCmdTxt, NULL,			NULL, NONE, LTCDebug}
  };
  


//:OUTP:LTCG#
code struct CMD_STRUCT LTCLevel1[5] = 
  {
	  {OFFSETCmdTxt,	OFFSCmdTxt, &LTCLevel1[1], 	NULL, NONE, LTCOffset},
	  {FORMATCmdTxt, 	FORMCmdTxt, &LTCLevel1[2], 	NULL, NONE, LTCFormat},
	  {TIMEParTxt,		TIMEParTxt, &LTCLevel1[3], 	&LTCTimeLevel[0], NONE, LTCTime},
	  {VERSIONCmdTxt,	VERSCmdTxt,  &LTCLevel1[4],	NULL, NONE, LTCVersion},
	  {DEBUGCmdTxt,		DEBUGCmdTxt, NULL,			NULL, NONE, LTCDebug}
  };
  
//:OUTP:LTCG#:TIME
code struct CMD_STRUCT LTCTimeLevel[2] =
{
	{TIMEZONECmdTxt, TIMEZCmdTxt, 	&LTCTimeLevel[1], 	NULL, NONE, LTCTimezone},
	{DAYLIGHTCmdTxt, DAYLCmdTxt,	NULL, 			&LTCDaylightLevel[0], NONE, LTCDaylight}
};

//:OUTP:LTCG#:TIME:DAYLIGHT
code struct CMD_STRUCT LTCDaylightLevel[3] =
{
	{STARTCmdTxt,	STARTCmdTxt,	&LTCDaylightLevel[1],	NULL, NONE, LTCDLStart},
	{ENDCmdTxt,		ENDCmdTxt,		&LTCDaylightLevel[2],	NULL, NONE, LTCDLEnd},
	{MODECmdTxt, 	MODECmdTxt,		NULL,					NULL, NONE, LTCDLMode}
};

code struct CMD_STRUCT TLGLevel1[3] =
  {
    {SYSTEMCmdTxt,  SYSTCmdTxt, &TLGLevel1[1], NULL, NONE, TLGFormat},
    {DELAYCmdTxt,   DELCmdTxt,  &TLGLevel1[2], NULL, NONE, TLGDelay},
    {VERSIONCmdTxt, VERSCmdTxt, NULL,          NULL, NONE, TLGVersion}
  };

code struct CMD_STRUCT HDLevel1[6] =
  {
    {PATTERNCmdTxt,   PATTCmdTxt, &HDLevel1[1], &HDAttirbLevel1[0], NONE, HDPattern},
    {TEXTCmdTxt,      TEXTCmdTxt, &HDLevel1[2], &HDTextLevel[0],    NONE, NONE},
    {EMBAUDIOCmdTxt,  EMBCmdTxt,  &HDLevel1[3], &HDEMBALevel[0],    NONE, NONE},
    {SYSTEMCmdTxt,    SYSTCmdTxt, &HDLevel1[4], NULL,               NONE, HDFormat},
    {DELAYCmdTxt,     DELCmdTxt,  &HDLevel1[5], NULL,               NONE, HDDelay},
    {VERSIONCmdTxt,   VERSCmdTxt, NULL,         NULL,               NONE, HDVersion}
  };

code struct CMD_STRUCT HDAttirbLevel1[1] =
  {
    {MODIFYCmdTxt, MODCmdTxt, NULL, NULL, NONE, HDAttrCommon},
  };


code struct CMD_STRUCT HDTextLevel[6] =
  {
    { STRINGCmdTxt,   STRCmdTxt,  &HDTextLevel[1], NULL, HDTextStringSuffix, HDTextString},
    { MOVEMENTCmdTxt, MOVCmdTxt,  &HDTextLevel[2], NULL, NONE, HDTextMovement},
    { SCALECmdTxt,    SCACmdTxt,  &HDTextLevel[3], NULL, NONE, HDTextScale},
    { COLORCmdTxt,    COLCmdTxt,  &HDTextLevel[4], NULL, NONE, HDTextColor},
    { BACKGRGRCmdTxt, BACGCmdTxt, &HDTextLevel[5], NULL, NONE, HDTextBack},
    { POSITIONCmdTxt, POSCmdTxt,  NULL,            NULL, NONE, HDTextPosition},
  };


code struct CMD_STRUCT HDEMBALevel[3] =
  {
    { SIGNALCmdTxt, SIGNCmdTxt, &HDEMBALevel[1], NULL, NONE, HDAudioSignal},
    { LEVELCmdTxt,  LEVCmdTxt,  &HDEMBALevel[2], NULL, NONE, HDAudioLevel},
    { CLICKCmdTxt,  CLICmdTxt,  NULL,            NULL, NONE, HDAudioClick},
  };

code struct CMD_STRUCT DLLevel1[6] =
  {
    {PATTERNCmdTxt,   PATTCmdTxt, &DLLevel1[1], &DLAttirbLevel1[0], NONE, DLPattern},
    {TEXTCmdTxt,      TEXTCmdTxt, &DLLevel1[2], &DLTextLevel[0],    NONE, NONE},
    {EMBAUDIOCmdTxt,  EMBCmdTxt,  &DLLevel1[3], &DLEMBALevel[0],    NONE, NONE},
    {SYSTEMCmdTxt,    SYSTCmdTxt, &DLLevel1[4], &DLINterfLevel1[0], NONE, DLFormat},
    {DELAYCmdTxt,     DELCmdTxt,  &DLLevel1[5], NULL,               NONE, DLDelay},
    {VERSIONCmdTxt,   VERSCmdTxt, NULL,         NULL,               NONE, DLVersion}
  };

code struct CMD_STRUCT DLINterfLevel1[1] =
  {
    {INTERFACECmdTxt, INTERCmdTxt, NULL, NULL, NONE, DLAInterCommon}
  };


code struct CMD_STRUCT DLAttirbLevel1[1] =
  {
    {MODIFYCmdTxt, MODCmdTxt, NULL, NULL, NONE, DLAttrCommon}
  };

code struct CMD_STRUCT DLTextLevel[6] =
  {
    { STRINGCmdTxt,   STRCmdTxt,  &DLTextLevel[1], NULL, HDTextStringSuffix, DLTextString},
    { MOVEMENTCmdTxt, MOVCmdTxt,  &DLTextLevel[2], NULL, NONE, DLTextMovement},
    { SCALECmdTxt,    SCACmdTxt,  &DLTextLevel[3], NULL, NONE, DLTextScale},
    { COLORCmdTxt,    COLCmdTxt,  &DLTextLevel[4], NULL, NONE, DLTextColor},
    { BACKGRGRCmdTxt, BACGCmdTxt, &DLTextLevel[5], NULL, NONE, DLTextBack},
    { POSITIONCmdTxt, POSCmdTxt,  NULL,            NULL, NONE, DLTextPosition},
  };

 
code struct CMD_STRUCT DLEMBALevel[3] =
  {
    { SIGNALCmdTxt, SIGNCmdTxt, &DLEMBALevel[1], NULL, NONE, DLAudioSignal},
    { LEVELCmdTxt,  LEVCmdTxt,  &DLEMBALevel[2], NULL, NONE, DLAudioLevel},
    { CLICKCmdTxt,  CLICmdTxt,  NULL,            NULL, NONE, DLAudioClick},
  };

code struct CMD_STRUCT BBLevel1[4] = {
  { SYSTEMCmdTxt, SYSTCmdTxt, &BBLevel1[1], NULL, NONE, BBSystem},
  { DELAYCmdTxt, DELCmdTxt, &BBLevel1[2], NULL, NONE, BBDelay},
  { SCHPHASECmdTxt, SCHPCmdTxt, &BBLevel1[3], NULL, NONE, BBScHPhase},
  { VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, BBVersion}
};

 
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

 
code struct CMD_STRUCT SDITSGLevel1[6] =
  {
    { PATTERNCmdTxt,   PATTCmdTxt, &SDITSGLevel1[1], NULL,             NONE, SDITSGPattern},
    { SYSTEMCmdTxt,    SYSTCmdTxt, &SDITSGLevel1[2], NULL,             NONE, SDITSGSystem},
    { DELAYCmdTxt,     DELCmdTxt,  &SDITSGLevel1[3], NULL,             NONE, SDITSGDelay},
    { EDHINSERTCmdTxt, EDHCmdTxt,  &SDITSGLevel1[4], NULL,             NONE, SDITSGEDHInsert},
    { EMBAUDIOCmdTxt,  EMBCmdTxt,  &SDITSGLevel1[5], &SDITSGLevel2[0], NONE, NONE},
    { VERSIONCmdTxt,   VERSCmdTxt, NULL,             NULL,             NONE, SDITSGVersion}
  };

code struct CMD_STRUCT SDITSGLevel2[2] =
  {
    { SIGNALCmdTxt, SIGNCmdTxt, &SDITSGLevel2[1], NULL, NONE, SDITSGAudioSignal},
    { LEVELCmdTxt, LEVCmdTxt, NULL, NULL, NONE, SDITSGAudioLevel},
  };

 
code struct CMD_STRUCT SDITPGLevel1[7] =
  {
    { PATTERNCmdTxt,   PATTCmdTxt, &SDITPGLevel1[1], &SDITPGLevel2[0], NONE, SDITPGPattern},
    { TEXTCmdTxt,      TEXTCmdTxt, &SDITPGLevel1[2], &SDITPGLevel4[0], NONE, NONE},
    { SYSTEMCmdTxt,    SYSTCmdTxt, &SDITPGLevel1[3], NULL,             NONE, SDITPGSystem},
    { EDHINSERTCmdTxt, EDHCmdTxt,  &SDITPGLevel1[4], NULL,             NONE, SDITPGEDHInsert},
    { EMBAUDIOCmdTxt,  EMBCmdTxt,  &SDITPGLevel1[5], &SDITPGLevel5[0], NONE, NONE},
    { DELAYCmdTxt,     DELCmdTxt,  &SDITPGLevel1[6], NULL,             NONE, SDITPGDelay},
    { VERSIONCmdTxt,   VERSCmdTxt, NULL,             NULL,             NONE, SDITPGVersion}
  };

code struct CMD_STRUCT SDITPGLevel2[1] = {
  { MODIFYCmdTxt, MODCmdTxt, NULL, &SDITPGLevel3[0], NONE, NONE},
};

code struct CMD_STRUCT SDITPGLevel3[6] = {
  { APALCmdTxt,      APALCmdTxt, &SDITPGLevel3[1], NULL, NONE,               SDITPGAttribAPAL},
  { PLUGEParTxt,     PLUGParTxt, &SDITPGLevel3[2], NULL, NONE,               SDITPGAttribPLUGE},
  { STAIRCASECmdTxt, STACmdTxt,  &SDITPGLevel3[3], NULL, SDITPGAttribSuffix, SDITPGAttribStair10},
  { MOTIONCmdTxt,    MOTCmdTxt,  &SDITPGLevel3[4], NULL, NONE,               SDITPGAttribMotion},
  { CIRCLESCmdTxt,   CIRCCmdTxt, &SDITPGLevel3[5], NULL, NONE,               SDITPGAttribCircles},
  { LOGOCmdTxt,      LOGCmdTxt,  NULL,             NULL, NONE,               SDITPGAttribLogo},
};

code struct CMD_STRUCT SDITPGLevel4[5] = /* movement added 15.01.2007 */
  {
    { STRINGCmdTxt,   STRCmdTxt,  &SDITPGLevel4[1], NULL, SDITPGTextStringSuffix, SDITPGTextString},
    { STYLECmdTxt,    STYLCmdTxt, &SDITPGLevel4[2], NULL, NONE, SDITPGTextStyle},
    { MOVEMENTCmdTxt, MOVCmdTxt,  &SDITPGLevel4[3], NULL, NONE, SDITPGTextMovement},
    { POSITIONCmdTxt, POSCmdTxt,  &SDITPGLevel4[4], NULL, NONE, SDITPGTextPosition},
    { CLOCKCmdTxt,    CLOCCmdTxt, NULL,             NULL, NONE, SDITPGTextClock},
  };

code struct CMD_STRUCT SDITPGLevel5[3] =
  {
    { SIGNALCmdTxt, SIGNCmdTxt, &SDITPGLevel5[1], NULL, NONE, SDITPGAudioSignal},
    { LEVELCmdTxt,  LEVCmdTxt,  &SDITPGLevel5[2], NULL, NONE, SDITPGAudioLevel},
    { GROUPCmdTxt,  GROCmdTxt,  NULL,             NULL, NONE, SDITPGAudioGroup},
  };

 
code struct CMD_STRUCT AESEBULevel1[4] = {
  { SIGNALCmdTxt, SIGNCmdTxt, &AESEBULevel1[1], NULL, NONE, AESEBUSignal},
  { LEVELCmdTxt, LEVCmdTxt, &AESEBULevel1[2], NULL, NONE, AESEBULevel},
  { TIMINGCmdTxt, TIMCmdTxt, &AESEBULevel1[3], NULL, NONE, AESEBUTiming},
  { VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, AESEBUVersion}
};

 
code struct CMD_STRUCT TimeClockLevel1[7] = {
  { DFORMATCmdTxt, DFORCmdTxt, &TimeClockLevel1[1], NULL, NONE, TimeClockDateFormat},
  { DATECmdTxt, DATCmdTxt, &TimeClockLevel1[2], NULL, NONE, TimeClockDate},
  { TFORMATCmdTxt, TFORCmdTxt, &TimeClockLevel1[3], NULL, NONE, TimeClockTimeFormat},
  { TIMECmdTxt, TIMCmdTxt, &TimeClockLevel1[4], NULL, NONE, TimeClockTime},
  { REFERENCECmdTxt, REFCmdTxt, &TimeClockLevel1[5], NULL, NONE, TimeClockReference},
  { OFFSETCmdTxt, OFFSCmdTxt, &TimeClockLevel1[6], NULL, NONE, TimeClockOffset},
  { VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, TimeClockVersion}
};

 
code struct CMD_STRUCT BBMultiLevel1[1] = {
  { VERSIONCmdTxt, VERSCmdTxt, NULL, NULL, NONE, BBMultiVersion}
};

 
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
 
// The following parameter structures are based on the enumerated types
//  defined in TABLES.H, ie. DO NOT change the order of these names. If
//  the order is changed in TABLES.H it must also be changed here.

code struct PAR_STRUCT GenlockInputPar[7] = {
  { AParTxt, AParTxt, &GenlockInputPar[1]},
  { BParTxt, BParTxt, &GenlockInputPar[2]},
  { ABParTxt, ABParTxt, &GenlockInputPar[3]},
  { SDIParTxt, SDIParTxt, &GenlockInputPar[4]},
  { GPSTxt, GPSTxt, &GenlockInputPar[5]},
  { INTERNALParTxt, INTParTxt, &GenlockInputPar[6]},
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

/*
typedef struct PAR_STRUCT
  {
  code char* LongName;
  code char* ShortName;

  code struct PAR_STRUCT* SameLevel;
  };
#define MAX_HD_SYSTEMS   19
#define MAX_DL_SYSTEMS   24
#define MAX_TLG_SYSTEMS  25

*/

code struct PAR_STRUCT TLGFormatPar[MAX_TLG_SYSTEMS] = 
  {                                       
  {L1Txt,  L1Txt,  &TLGFormatPar[1]},     
  {L2Txt,  L2Txt,  &TLGFormatPar[2]},     
  {L3Txt,  L3Txt,  &TLGFormatPar[3]},     
  {L4Txt,  L4Txt,  &TLGFormatPar[4]},     
  {L5Txt,  L5Txt,  &TLGFormatPar[5]},     
  {L6Txt,  L6Txt,  &TLGFormatPar[6]},     
  {L7Txt,  L7Txt,  &TLGFormatPar[7]},     
  {L8Txt,  L8Txt,  &TLGFormatPar[8]},     
  {L9Txt,  L9Txt,  &TLGFormatPar[9]},     
  {L10Txt, L10Txt, &TLGFormatPar[10]},    
  {L11Txt, L11Txt, &TLGFormatPar[11]},    
  {L12Txt, L12Txt, &TLGFormatPar[12]},    
  {L13Txt, L13Txt, &TLGFormatPar[13]},    //code char L13Txt[]  = "HD1080sF30";
  {L14Txt, L14Txt, &TLGFormatPar[14]},    
  {L15Txt, L15Txt, &TLGFormatPar[15]},    
  {L16Txt, L16Txt, &TLGFormatPar[16]},    
  {L17Txt, L17Txt, &TLGFormatPar[17]},    
  {L18Txt, L18Txt, &TLGFormatPar[18]},    
  {L19Txt, L19Txt, &TLGFormatPar[19]},    
  {L20Txt, L20Txt, &TLGFormatPar[20]},
  {L21Txt, L21Txt, &TLGFormatPar[21]},
  {L22Txt, L22Txt, &TLGFormatPar[22]},
  {L23Txt, L23Txt, &TLGFormatPar[23]},
  {L24Txt, L24Txt, &TLGFormatPar[24]},
  {L25Txt, L25Txt, NULL}
  };

    
code struct PAR_STRUCT HDFormatPar[MAX_HD_SYSTEMS] =
  {
  {M0Txt,  M0Txt,  &HDFormatPar[1]},
  {M4Txt,  M4Txt,  &HDFormatPar[2]},
  {M5Txt,  M5Txt,  &HDFormatPar[3]},                    
  {M6Txt,  M6Txt,  &HDFormatPar[4]},                    
  {M7Txt,  M7Txt,  &HDFormatPar[5]},                    
  {M8Txt,  M8Txt,  &HDFormatPar[6]},                    
  {M9Txt,  M9Txt,  &HDFormatPar[7]},                    
  {M10Txt, M10Txt, &HDFormatPar[8]},                           
  {M11Txt, M11Txt, &HDFormatPar[9]},

  {N30Txt,  N30Txt,  &HDFormatPar[10]},       
  {N31Txt,  N31Txt,  &HDFormatPar[11]},      
  {N32Txt,  N32Txt,  &HDFormatPar[12]},       
  {N33Txt,  N33Txt,  &HDFormatPar[13]},       
  {N34Txt,  N34Txt,  &HDFormatPar[14]},      

  {M14Txt, M14Txt, &HDFormatPar[15]},                           
  {M15Txt, M15Txt, &HDFormatPar[16]},                           
  {M16Txt, M16Txt, &HDFormatPar[17]},                           
  {M17Txt, M17Txt, &HDFormatPar[18]},                           
  {M18Txt, M18Txt, &HDFormatPar[19]},                           
  {M19Txt, M19Txt, &HDFormatPar[20]},                           
  {M20Txt, M20Txt, &HDFormatPar[21]},                           
  {M21Txt, M21Txt, &HDFormatPar[22]},                           
  {M22Txt, M22Txt, &HDFormatPar[23]},
  {M23Txt, M23Txt, NULL}
  };

code struct PAR_STRUCT DLFormatPar[MAX_DL_SYSTEMS] = // 24
  {
  {N0Txt,   N0Txt,   &DLFormatPar[1]},
  {N4Txt,   N4Txt,   &DLFormatPar[2]},
  {N5Txt,   N5Txt,   &DLFormatPar[3]},                     
  {N6Txt,   N6Txt,   &DLFormatPar[4]},                      
  {N7Txt,   N7Txt,   &DLFormatPar[5]},                      
  {N8Txt,   N8Txt,   &DLFormatPar[6]},                     
  {N9Txt,   N9Txt,   &DLFormatPar[7]},                      
  {N10Txt,  N10Txt,  &DLFormatPar[8]},                          
  {N11Txt,  N11Txt,  &DLFormatPar[9]},                         
  {N30Txt,  N30Txt,  &DLFormatPar[10]},       
  {N31Txt,  N31Txt,  &DLFormatPar[11]},      
  {N32Txt,  N32Txt,  &DLFormatPar[12]},       
  {N33Txt,  N33Txt,  &DLFormatPar[13]},       
  {N34Txt,  N34Txt,  &DLFormatPar[14]},      
  {N14Txt,  N14Txt,  &DLFormatPar[15]},                          
  {N15Txt,  N15Txt,  &DLFormatPar[16]},                         
  {N16Txt,  N16Txt,  &DLFormatPar[17]},                          
  {N17Txt,  N17Txt,  &DLFormatPar[18]},                          
  {N18Txt,  N18Txt,  &DLFormatPar[19]},                         
  {N19Txt,  N19Txt,  &DLFormatPar[20]},                          
  {N20Txt,  N20Txt,  &DLFormatPar[21]},                          
  {N21Txt,  N21Txt,  &DLFormatPar[22]},                         
  {N22Txt,  N22Txt,  &DLFormatPar[23]},
  {N23Txt,  N23Txt,  NULL},
  };

code struct PAR_STRUCT HDPatternPar[10] =
  {
    { CblTxt,  CbsTxt,   &HDPatternPar[1]},
    { ColTxt,  CosTxt,   &HDPatternPar[2]},
    { WilTxt,  WisTxt,   &HDPatternPar[3]},
    { WhlTxt,  WhsTxt,   &HDPatternPar[4]},
    { CrlTxt,  CrsTxt,   &HDPatternPar[5]},
    { CflTxt,  CfsTxt,   &HDPatternPar[6]},
    { PllTxt,  PlsTxt,   &HDPatternPar[7]},
    { LrlTxt,  LrsTxt,   &HDPatternPar[8]},
    { CllTxt,  ClsTxt,   &HDPatternPar[9]},
    { BllTxt,  BlsTxt,   NULL}
  };
/* PATTERNS FOR HD
code char BlTxt[]  = "BLACK";
code char CfTxt[]  = "SDICF";
code char PlTxt[]  = "PLUGE";
code char LrTxt[]  = "LUMRAMP";
code char ClTxt[]  = "CLAPPER";
code char CbTxt[]  = "COLORBAR";
code char CoTxt[]  = "COMBINATION";
code char WiTxt[]  = "WINDOW";
code char CrTxt[]  = "CROSSHATCH";
code char WhTxt[]  = "WHITE";
enum HDPatterns
  {
  HDColorbar = 0, HDCombination, HDWindow, HDWhite,  HDCrosshatch,
  HDCheckField,   HDPluge,       HDLumRamp, HDClapBoard,
  HDBlack    
  };

enum TextColors
  {
  ColorBlack = 0, ColorBlue, ColorGreen, ColorCyan, ColorRed, ColorMagenta, 
  ColorYellow, ColorWhite
  }
enum TextMovement
  {
  TextMovementNone, TextMovementHoriz, TextMovementVertical, TextMovementBoth
  }

          params
 */

code struct PAR_STRUCT DLInterPar[6] =
  {
    {Inter1,  Inter1,   &DLInterPar[1]},
    {Inter2,  Inter2,   &DLInterPar[2]},
    {Inter3,  Inter3,   &DLInterPar[3]},
    {Inter4,  Inter4,   &DLInterPar[4]},
    {Inter5,  Inter5,   &DLInterPar[5]},
    {Inter6,  Inter6,   NULL}
  };

code struct PAR_STRUCT HDItensPar[3] = // for Colorbar and Combination
  {
    {AttCmdTxt0,   AttCmdTxt0,  &HDItensPar[1]},
    {AttCmdTxt1,   AttCmdTxt1,  &HDItensPar[2]},
    {AttCmdTxt2,   AttCmdTxt2,  NULL}
  };

code struct PAR_STRUCT HDLevelPar[23] = // for Window and White
  {
    {AttCmdTxt3,   AttCmdTxt3,  &HDLevelPar[1]},
    {AttCmdTxt4,   AttCmdTxt4,  &HDLevelPar[2]},
    {AttCmdTxt5,   AttCmdTxt5,  &HDLevelPar[3]},
    {AttCmdTxt6,   AttCmdTxt6,  &HDLevelPar[4]},
    {AttCmdTxt7,   AttCmdTxt7,  &HDLevelPar[5]},
    {AttCmdTxt8,   AttCmdTxt8,  &HDLevelPar[6]},
    {AttCmdTxt9,   AttCmdTxt9,  &HDLevelPar[7]},
    {AttCmdTxt10,  AttCmdTxt10, &HDLevelPar[8]},
    {AttCmdTxt11,  AttCmdTxt11, &HDLevelPar[9]},
    {AttCmdTxt12,  AttCmdTxt12, &HDLevelPar[10]},
    {AttCmdTxt13,  AttCmdTxt13, &HDLevelPar[11]},
    {AttCmdTxt14,  AttCmdTxt14, &HDLevelPar[12]},
    {AttCmdTxt15,  AttCmdTxt15, &HDLevelPar[13]},
    {AttCmdTxt16,  AttCmdTxt16, &HDLevelPar[14]},
    {AttCmdTxt17,  AttCmdTxt17, &HDLevelPar[15]},
    {AttCmdTxt18,  AttCmdTxt18, &HDLevelPar[16]},
    {AttCmdTxt19,  AttCmdTxt19, &HDLevelPar[17]},
    {AttCmdTxt20,  AttCmdTxt20, &HDLevelPar[18]},
    {AttCmdTxt21,  AttCmdTxt21, &HDLevelPar[19]},
    {AttCmdTxt22,  AttCmdTxt22, &HDLevelPar[20]},
    {AttCmdTxt23,  AttCmdTxt23, &HDLevelPar[21]},
    {AttCmdTxt24,  AttCmdTxt24, &HDLevelPar[22]},
    {AttCmdTxt25,  AttCmdTxt25, NULL}
  };
code struct PAR_STRUCT HDTextColotPar[8] =
  {
    {BLACKTxt,   BLATxt,  &HDTextColotPar[1]},
    {BLUETxt,    BLUxt,   &HDTextColotPar[2]},
    {GREENTxt,   GRETxt,  &HDTextColotPar[3]},
    {CYANTxt,    CYATxt,  &HDTextColotPar[4]},
    {REDTxt,     REDTxt,  &HDTextColotPar[5]},
    {MAGENTATxt, MAGTxt,  &HDTextColotPar[6]},
    {YELLOWTxt,  YELTxt,  &HDTextColotPar[7]},
    {WHITETxt,   WHIxt,   NULL}
  };

code struct PAR_STRUCT TextMovePar[4] =
  {
    { OFFParTxt,     OFFParTxt, &TextMovePar[1]},
    { VeritcalTxt,   VertTxt,   &TextMovePar[2]},
    { HorizontalTxt, HorizTxt,  &TextMovePar[3]},
    { BothTxt,       BothTxt,   NULL},
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

code struct PAR_STRUCT SDIPatternPar[67] =
  {
    { CBSMPTEParTxt,  CBSMParTxt,   &SDIPatternPar[1]},
    { CBEBUParTxt,    CBEBParTxt,   &SDIPatternPar[2]},
    { CBFCCParTxt,    CBFCParTxt,   &SDIPatternPar[3]},
    { CBEBU8ParTxt,   CBEB8ParTxt,  &SDIPatternPar[4]},
    { CB100ParTxt,    CB100ParTxt,  &SDIPatternPar[5]},
    { CBGREY75ParTxt, CBGR75ParTxt, &SDIPatternPar[6]},
    { CBRED75ParTxt,  CBR75ParTxt,  &SDIPatternPar[7]},
    { RED75ParTxt,    RED75ParTxt,  &SDIPatternPar[8]},

    { MULTIBURSTParTxt, MULTParTxt,   &SDIPatternPar[9]},
    { LSWEEPParTxt,     LSWParTxt,    &SDIPatternPar[10]},
    { YCRCBSWEEPParTxt, YCRCParTxt,   &SDIPatternPar[11]},
    { MPULSEParTxt,     MPULParTxt,   &SDIPatternPar[12]},
    { SINXXParTxt,      SINXParTxt,   &SDIPatternPar[13]},
    { CCIR18ParTxt,     CCIR18ParTxt, &SDIPatternPar[14]},
  
    { WIN10ParTxt,    WIN10ParTxt,   &SDIPatternPar[15]},
    { WIN15ParTxt,    WIN15ParTxt,   &SDIPatternPar[16]},
    { WIN20ParTxt,    WIN20ParTxt,   &SDIPatternPar[17]},
    { WIN100ParTxt,   WIN100ParTxt,  &SDIPatternPar[18]},
    { BLWH15ParTxt,   BLWH15ParTxt,  &SDIPatternPar[19]},
    { WHITE100ParTxt, WHIT100ParTxt, &SDIPatternPar[20]},
    { BLACKParTxt,    BLACParTxt,    &SDIPatternPar[21]},

    { SDICHECKParTxt,  SDICParTxt,   &SDIPatternPar[22]},
    { DTIMINGParTxt,   DTIMParTxt,   &SDIPatternPar[23]},
    { FDTESTParTxt,    FDTParTxt,    &SDIPatternPar[24]},
    { BOWTIEParTxt,    BOWTParTxt,   &SDIPatternPar[25]},
    { ABLANKINGParTxt, ABLParTxt,    &SDIPatternPar[26]},
    { DGREYParTxt,     DGRParTxt,    &SDIPatternPar[27]},
    { FSWAVEParTxt,    FSWParTxt,    &SDIPatternPar[28]},
    { BLWH01ParTxt,    BLWH01ParTxt, &SDIPatternPar[29]},
    { EOLINEParTxt,    EOLParTxt,    &SDIPatternPar[30]},
    { WEOLINEParTxt,   WEOLParTxt,   &SDIPatternPar[31]},
    { BEOLINEParTxt,   BEOLParTxt,   &SDIPatternPar[32]},
    { REOLINEParTxt,   REOLParTxt,   &SDIPatternPar[33]},
    { YEOLINEParTxt,   YEOLParTxt,   &SDIPatternPar[34]},
    { CEOLINEParTxt,   CEOLParTxt,   &SDIPatternPar[35]},
    { CUSTOM1ParTxt,   CUST1ParTxt,  &SDIPatternPar[36]},
    { CUSTOM2ParTxt,   CUST2ParTxt,  &SDIPatternPar[37]},
    { CUSTOM3ParTxt,   CUST3ParTxt,  &SDIPatternPar[38]},
    { CUSTOM4ParTxt,   CUST4ParTxt,  &SDIPatternPar[39]},
    { CUSTOM5ParTxt,   CUST5ParTxt,  &SDIPatternPar[40]},
    { CUSTOM6ParTxt,   CUST6ParTxt,  &SDIPatternPar[41]},
    { CUSTOM7ParTxt,   CUST7ParTxt,  &SDIPatternPar[42]},
    { CUSTOM8ParTxt,   CUST8ParTxt,  &SDIPatternPar[43]},
  
    { SRAMPParTxt,       SRAMParTxt,    &SDIPatternPar[44]},
    { RAMPParTxt,        RAMPParTxt,    &SDIPatternPar[45]},
    { LRAMPParTxt,       LRAMParTxt,    &SDIPatternPar[46]},
    { VRAMPParTxt,       VRAMParTxt,    &SDIPatternPar[47]},
    { STAIRCASE5ParTxt,  STA5ParTxt,    &SDIPatternPar[48]},
    { MSTAIRCASE5ParTxt, MSTA5ParTxt,   &SDIPatternPar[49]},
    { STAIRCASE10ParTxt, STA10ParTxt,   &SDIPatternPar[50]},
    { PBARParTxt,        PBARParTxt,    &SDIPatternPar[51]},
    { CCIR17ParTxt,      CCIR17ParTxt,  &SDIPatternPar[52]},
    { CCIR330ParTxt,     CCIR330ParTxt, &SDIPatternPar[53]},
    { CCIR331ParTxt,     CCIR331ParTxt, &SDIPatternPar[54]},
    { YGRAMPParTxt,      YGRParTxt,     &SDIPatternPar[55]},
    { GBRAMPParTxt,      GBRParTxt,     &SDIPatternPar[56]},
    { CGRAMPParTxt,      CGRParTxt,     &SDIPatternPar[57]},
    { GRRAMPParTxt,      GRRParTxt,     &SDIPatternPar[58]},
    { CBYCRYRAMPParTxt,  CBYCParTxt,    &SDIPatternPar[59]},
  
    { PHILIPS43ParTxt,   PHIL43ParTxt,  &SDIPatternPar[60]},
    { PHILIPS169ParTxt,  PHIL169ParTxt, &SDIPatternPar[61]},
    { FUBK43ParTxt,      FUBK43ParTxt,  &SDIPatternPar[62]},
    { FUBK169ParTxt,     FUBK169ParTxt, &SDIPatternPar[63]},
    { CROSSHATCHParTxt,  CROSParTxt,    &SDIPatternPar[64]},
    { PLUGEParTxt,       PLUGParTxt,    &SDIPatternPar[65]},
    { SAFEAREAParTxt,    SAFParTxt,     &SDIPatternPar[66]},
    { VMT01ParTxt,       VMT01ParTxt,   NULL}
  };

code struct PAR_STRUCT TSGAudioSignalPar[2] =
  {
    { OFFParTxt,   OFFParTxt,   &TSGAudioSignalPar[1]},
    { S1KHZParTxt, S1KHZParTxt, NULL},
  };

code struct PAR_STRUCT TSGAudioLevelPar[5] =
  {
    { SILENCEParTxt, SILParTxt,    &TSGAudioLevelPar[1]},
    { DB0FSParTxt,   DB0FSParTxt,  &TSGAudioLevelPar[2]},
    { DB9FSParTxt,   DB9FSParTxt,  &TSGAudioLevelPar[3]},
    { DB15FSParTxt,  DB15FSParTxt, &TSGAudioLevelPar[4]},
    { DB18FSParTxt,  DB18FSParTxt, NULL}
  };

code struct PAR_STRUCT TPGAudioSignalPar[8] =
  {
    { OFFParTxt,      OFFParTxt,      &TPGAudioSignalPar[1]},
    { S800HZParTxt,   S800HZParTxt,   &TPGAudioSignalPar[2]},
    { S1KHZParTxt,    S1KHZParTxt,    &TPGAudioSignalPar[3]},
    { SEBU1KHZParTxt, SEBU1KHZParTxt, &TPGAudioSignalPar[4]},
    { SBBC1KHZParTxt, SBBC1KHZParTxt, &TPGAudioSignalPar[5]},
    { MEBU1KHZParTxt, MEBU1KHZParTxt, &TPGAudioSignalPar[6]},
    { M1KHZParTxt,    M1KHZParTxt,    &TPGAudioSignalPar[7]},
    { DUALParTxt,     DUALParTxt,     NULL},
  };

code struct PAR_STRUCT HDAudioSignalPar[4] =
  {
    { SILENCEParTxt, SILParTxt,   &HDAudioSignalPar[1]},
    { SineParTxt,    SineParTxt,  &HDAudioSignalPar[2]},
    { ClickParTxt,   ClickParTxt, &HDAudioSignalPar[3]},
    { OFFParTxt,     OFFParTxt,   NULL}
  };


code struct PAR_STRUCT TPGAudioLevelPar[9] =
  {
    { SILENCEParTxt, SILParTxt,    &TPGAudioLevelPar[1]},
    { DB0FSParTxt,   DB0FSParTxt,  &TPGAudioLevelPar[2]},
    { DB9FSParTxt,   DB9FSParTxt,  &TPGAudioLevelPar[3]},
    { DB12FSParTxt,  DB12FSParTxt, &TPGAudioLevelPar[4]},
    { DB14FSParTxt,  DB14FSParTxt, &TPGAudioLevelPar[5]},
    { DB15FSParTxt,  DB15FSParTxt, &TPGAudioLevelPar[6]},
    { DB16FSParTxt,  DB16FSParTxt, &TPGAudioLevelPar[7]},
    { DB18FSParTxt,  DB18FSParTxt, &TPGAudioLevelPar[8]},
    { DB20FSParTxt,  DB20FSParTxt, NULL},
  };

code struct PAR_STRUCT HDAudioLevelPar[5] =
  {
    { DB0FSParTxt,   DB0FSParTxt,  &HDAudioLevelPar[1]},
    { DB6FSParTxt,   DB6FSParTxt,  &HDAudioLevelPar[2]},
    { DB12FSParTxt,  DB12FSParTxt, &HDAudioLevelPar[3]},
    { DB18FSParTxt,  DB18FSParTxt, &HDAudioLevelPar[4]},
    { DB24FSParTxt,  DB20FSParTxt, NULL},
  };


code struct PAR_STRUCT TPGAudioGroupPar[4] = {
  { GROUP1ParTxt, GRO1ParTxt, &TPGAudioGroupPar[1]},
  { GROUP2ParTxt, GRO2ParTxt, &TPGAudioGroupPar[2]},
  { GROUP3ParTxt, GRO3ParTxt, &TPGAudioGroupPar[3]},
  { GROUP4ParTxt, GRO4ParTxt, NULL}
};

code struct PAR_STRUCT AESEBUSignalPar[9] =
  {
  { OFFParTxt,       OFFParTxt,      &AESEBUSignalPar[1]},
  { S800HZParTxt,    S800HZParTxt,   &AESEBUSignalPar[2]},
  { S1KHZParTxt,     S1KHZParTxt,    &AESEBUSignalPar[3]},
  { SEBU1KHZParTxt,  SEBU1KHZParTxt, &AESEBUSignalPar[4]},
  { SBBC1KHZParTxt,  SBBC1KHZParTxt, &AESEBUSignalPar[5]},
  { MEBU1KHZParTxt,  MEBU1KHZParTxt, &AESEBUSignalPar[6]},
  { M1KHZParTxt,     M1KHZParTxt,    &AESEBUSignalPar[7]},
  { DUALParTxt,      DUALParTxt,     &AESEBUSignalPar[8]},
  { WORDCLOCKParTxt, WORDParTxt,     NULL},
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
  { PT5300ParTxt, PT5300ParTxt, NULL},
};

code struct PAR_STRUCT LTCFormatPar[5] =
  {
    {LTC24FPS,   	LTC24FPS,  &LTCFormatPar[1]},
    {LTC25FPS,   	LTC25FPS,  &LTCFormatPar[2]},
    {LTC2997DROP,   LTC2997DROP,  &LTCFormatPar[3]},
    {LTC2997NOND,   LTC2997NOND,  &LTCFormatPar[4]},
    {LTC30FPS,   	LTC30FPS,  NULL}
};

code struct PAR_STRUCT LTCModePar[3] =
  {
    {LTCNONE,   	LTCNONE,  &LTCModePar[1]},
    {LTCCONF,   	LTCCONF,  &LTCModePar[2]},
    {LTCAUTO,   	LTCAUTO,  NULL}
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
static char GPSBuffer[12];

static UC ParState;

static char  Field;
static int   Line;
static float HTime;
static char  sign;

static UL UlongVal;

static int IntVal;
static int IntVal1;

static UC ucharval;
static UC UcharVal1;

static UC*  downPtr;
static char tbuff[60];





/***************************************************************************/
/*  IDNResponseMessage                             RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970218                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                               */
/*          ParState 2: Char, Char                           */
/*          ParState 3: Char, Char, Char                       */
/*          ParState 4: Char, Char, Char, Char                  */
/*          IDN is : PTV,PT5300,KU123456,1911                  */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void IDNResponseMessage() {

  register UC i, CommaSearch;
  char* txtptr;
  char tmp;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_ASCII:
      ParState++;

      txtptr = Cmd;
      i = CommaSearch = 0;

      while (tmp = *txtptr) {

        if ((TmpBuffer[i] = tmp) == ',') {
          TmpBuffer[i] = 0;

          switch (CommaSearch++) {
            case 0:          // Company name
              break;

            case 1:          // Model name
              break;

            case 2:          // KU number
              ResponseKUNo = atol(&TmpBuffer[2]);

                          // SW version
              strncpy(&TmpBuffer[0], txtptr+1, 4);
              TmpBuffer[4] = 0;

              ResponseSWVersion = (UC)(10*atof(TmpBuffer));
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

  if (CmdExecute && (ParState == 1))
    ResponseReceived = 0xFF;
}

/***************************************************************************/
/*  CLSCommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Clear status command                            */
/*  Remarks:    No parameters                                */
/*  Returns:    -                                        */
/*  Updates:                                            */
/***************************************************************************/
void CLSCommand() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else
      ResetErrorEventQueue();
  }
}

/***************************************************************************/
/*  ESECommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970627                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set/read the Standard Event Status Enable Register        */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*  Returns:    -                                        */
/*  Updates:                                            */
/***************************************************************************/
void ESECommand() {

  register int tmp;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        if ((tmp = atoi(Cmd)) > 255)
          SCPIError = DataOutOfRange;
        else
          ucharval = (UC) tmp;
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState)
        SCPIError = ParNotAllowed;
      else {
        sprintf(TmpBuffer, "%d\n", ESEReg);
        Udr_PutNChar(TmpBuffer);
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else
        ESEReg = ucharval;
    }
  }
}

/***************************************************************************/
/*  ESRCommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970627                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Read and clear the Standard Event Status Register        */
/*  Remarks:    No parameters                                */
/*          The register will reset to zero.                    */
/*  Returns:    -                                        */
/*  Updates:                                            */
/***************************************************************************/
void ESRCommand() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      sprintf(TmpBuffer, "%d\n", UpdateESRRegister());
      Udr_PutNChar(TmpBuffer);

      ESRReg = 0;
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  OPCCommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Operation Complete Command/Query                    */
/*  Remarks:    No parameters                                */
/*  Returns:    -                                        */
/*  Updates:                                            */
/***************************************************************************/
void OPCCommand() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest)
      ;
    else
      ;
  }
}

/***************************************************************************/
/*  SRECommand                                    RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970627                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set/read the Service Request Enable Register            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          Bit 6 in *SRE? register is always ZERO                */
/*          Bit 6 in *SRE register is ignored                  */
/*  Returns:    -                                        */
/*  Updates:                                            */
/***************************************************************************/
void SRECommand() {

  register int tmp;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        if ((tmp = atoi(Cmd)) > 255)
          SCPIError = DataOutOfRange;
        else
          ucharval = (UC) tmp;
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState)
        SCPIError = ParNotAllowed;
      else {
        sprintf(TmpBuffer, "%d\n", SREReg);
        Udr_PutNChar(TmpBuffer);
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else
        SREReg = ucharval & 0xBF;
    }
  }
}

/***************************************************************************/
/*  STBCommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970627                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Read the Status Byte Register                      */
/*  Remarks:    No parameters                                */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void STBCommand() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      UpdateSTBRegister();

      sprintf(TmpBuffer, "%d\n", STBReg);
      Udr_PutNChar(TmpBuffer);
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  TSTCommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970630                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Selftest query                                */
/*  Remarks:    No parameters                                */
/*  Returns:    -                                        */
/*  Updates:                                            */
/***************************************************************************/
void TSTCommand() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest)
      ;
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  WAICommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970630                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Wait-to-continue command                        */
/*  Remarks:    No parameters                                */
/*  Returns:    -                                        */
/*  Updates:                                            */
/***************************************************************************/
void WAICommand() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else
      ;
  }
}

/***************************************************************************/
/*  RSTCommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 961124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Reset comamnd                                */
/*  Remarks:    No parameters                                */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void RSTCommand() {

  register int i;

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;
  else {
    if (CmdExecute && !SCPIError) {
      if (!CmdRequest) {

        for (i = 0; i < MaxAnlBlkUnits; i++) {
          Cmemcpy(&AnlBlkConfig[i], &AnlBlkConfigFactory[i], sizeof(AnlBlkConfig[0]));

          if (AnlBlkUnit[i].Present)
            ConfigureAnlBlkUnit(i);
        }

        FeedTheWatchdog;

        for (i = 0; i < MaxAnlTPGUnits; i++) {
          Cmemcpy(&AnlTPGConfig[i], &AnlTPGConfigFactory[i], sizeof(AnlTPGConfig[0]));

          if (AnlTPGUnit[i].Present)
            ConfigureAnlTPGUnit(i);
        }

        FeedTheWatchdog;

        for (i = 0; i < MaxSDITSGUnits; i++) {
          Cmemcpy(&SDITSGConfig[i], &SDITSGConfigFactory[i], sizeof(SDITSGConfig[0]));

          if (SDITSGUnit[i].Present)
            ConfigureSDITSGUnit(i);
        }

        FeedTheWatchdog;

        for (i = 0; i < MaxSDITPGUnits; i++){
          Cmemcpy(&SDITPGConfig[i], &SDITPGConfigFactory[i], sizeof(SDITPGConfig[0]));

          if (SDITPGUnit[i].Present)
            ConfigureSDITPGUnit(i);
        }

        FeedTheWatchdog;

        for (i = 0; i < MaxAESEBUUnits; i++)
          Cmemcpy(&AESEBUConfig[i], &AESEBUConfigFactory[i], sizeof(AESEBUConfig[0]));

        if (AESEBUUnit.Present) {
          AESEBUUnitDrv(AESEBU1Item, AESEBUConfig[AESEBU1Item].Signal,\
                             AESEBUConfig[AESEBU1Item].Level,\
                             AESEBUConfig[AESEBU1Item].Timing);

          if (AESEBUUnit.HWType == PT8635)
            AESEBUUnitDrv(AESEBU2Item, AESEBUConfig[AESEBU2Item].Signal,\
                               AESEBUConfig[AESEBU2Item].Level,\
                               AESEBUConfig[AESEBU2Item].Timing);
        }

        FeedTheWatchdog;

        for (i = 0; i < MaxGenlockUnits; i++)
          Cmemcpy(&GenlockConfig[i], &GenlockConfigFactory[i], sizeof(GenlockConfig[0]));

        ActiveGenlockNo = Internal;
        ExternalGenlockNo = GenlockA;

        GenlockUnitDrv(ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
        
        //program LTCs
      for (i = 0; i < MaxLTCUnits; i++)
        {
      Cmemcpy(&Preset[0].LTCStore[i], &LTCConfigFactory, sizeof(LTCConfig[0]));
      Cmemcpy(&LTCConfig[i], &LTCConfigFactory, sizeof(LTCConfig[0]));
        }
        
      FeedTheWatchdog;

        Cmemcpy(&RS232Config, &RS232ConfigFactory, sizeof(RS232Config));

        LCDContrast = DefContrast;

        ActiveOutputNo = AnlBlkItem;    // AnlBlkItem is NOT part of output
        ActivePresetNo = 0;

        if (Ptr == &status_menu[StatusPresetItem])
          Ptr->DispFunct();

        Flags.NormalLockOn    = OFF;
        Flags.PanelLockOn     = OFF;
        Flags.DateTimeLockOn  = OFF;
        Flags.DownloadLockOn  = OFF;
        Flags.DiagnoseLockOn  = OFF;

        Flags.AutoESCEnable   = ON;
        Flags.AutoESCToStatus = ON;

        PasswordState         = ON;

                            // Reset apparatus error queue
        for (i=0; i < NoInErrorQueue; i++)
          ErrorQueue[i] = 0;

        ErrorQueuePtr = NoInErrorQueue-1;

        ResetErrorEventQueue();      // Reset RS232 error queue

                            // Initialize the remote RS232
        Udr_InitializeUart(RS232Config.Baudrate, RS232Config.Databit,\
                       RS232Config.Parity, RS232Config.Handshake);

      }
    }
  }
}

/***************************************************************************/
/*  IDNCommand                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Identification query                            */
/*  Remarks:    No parameter, Query only                        */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void IDNCommand() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {

      IIC2RdStr(mainram1_addr, CompanyName_addr, TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      Udr_PutChar(',');

      IIC2RdStr(mainram1_addr, ProductName1_addr, TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      if (strlen(TmpBuffer) >= 10) {
        IIC2RdStr(mainram1_addr, ProductName2_addr, TmpBuffer);
        Udr_PutNChar(TmpBuffer);
      }

      Udr_PutChar(',');

      IIC2RdStr(mainram1_addr, ProductKUNo_addr, TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      Udr_PutChar(',');

      GetPT5230SWVersion(TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      Udr_PutNewline();
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  SystemError                                   RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    98061                                       */
/*                                                  */
/*  Function:  Write the next entry in the error queue (FIFO) to RS232     */
/*  Remarks:    No parameters, Query only                        */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void SystemError() {

  register int i;

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if ((i = ReadErrorEventQueue()) >= 0) {
        sprintf(TmpBuffer, "%d", ErrorTxt_Array[i].ErrorNo);
        Udr_PutNChar(TmpBuffer);

        Udr_PutNChar(",\"");

        Udr_CPutNChar(ErrorTxt_Array[i].ErrorTxt);
        Udr_PutChar('"');

        Udr_PutNewline();
      }
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  SystemVersion                                 RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Write the SCPI version to RS232                    */
/*  Remarks:    No parameter, Query only                        */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void SystemVersion() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      Udr_CPutNChar(SystemVersionTxt);
      Udr_PutNewline();
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  SystemPreset                                 RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970206                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Ativate the specified preset                      */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void SystemPreset() {

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        ucharval = (UC) atoi(Cmd);

        if ((ucharval == 0) || (ucharval > NoOfPresets))
          SCPIError = DataOutOfRange;
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState)
        SCPIError = ParNotAllowed;
      else {
        Udr_PutChar('0' + ActivePresetNo);
        Udr_PutNewline();
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
        ActivePresetNo = ucharval;

        if (Ptr == &status_menu[StatusPresetItem])
          Ptr->DispFunct();

        if (ConfigurePreset(--ucharval))
          SCPIError = ExecErr;
      }
    }
  }
}

/***************************************************************************/
/*  SystemPresetStore                               RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970206                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Store actual setup in preset                      */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void SystemPresetStore() {

  register int i;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        ucharval = (UC) atoi(Cmd);

        if ((ucharval == 0) || (ucharval > NoOfPresets))
          SCPIError = DataOutOfRange;
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
        ActivePresetNo = ucharval;

        if (Ptr == &status_menu[StatusPresetItem])
          Ptr->DispFunct();

        ucharval--;

        for (i = 0; i < MaxAnlBlkUnits; i++)
          memcpy(&Preset[ucharval].AnlBlkStore[i], &AnlBlkConfig[i], sizeof(AnlBlkConfig[0]));

        for (i = 0; i < MaxAnlTPGUnits; i++)
          memcpy(&Preset[ucharval].AnlTPGStore[i], &AnlTPGConfig[i], sizeof(AnlTPGConfig[0]));

        for (i = 0; i < MaxSDITSGUnits; i++)
          memcpy(&Preset[ucharval].SDITSGStore[i], &SDITSGConfig[i], sizeof(SDITSGConfig[0]));

        for (i = 0; i < MaxSDITPGUnits; i++)
          memcpy(&Preset[ucharval].SDITPGStore[i], &SDITPGConfig[i], sizeof(SDITPGConfig[0]));

        for (i = 0; i < MaxAESEBUUnits; i++)
          memcpy(&Preset[ucharval].AESEBUStore[i], &AESEBUConfig[i], sizeof(AESEBUConfig[0]));

        for (i = 0; i < MaxGenlockUnits; i++)
          memcpy(&Preset[ucharval].GenlockStore[i], &GenlockConfig[i], sizeof(GenlockConfig[0]));
          
        for (i = 0; i < MaxLTCUnits; i++)
          memcpy(&Preset[ucharval].LTCStore[i], &LTCConfig[i], sizeof(LTCConfig[0]));
         

        Preset[ucharval].ActiveGenlockNo = ActiveGenlockNo;
        Preset[ucharval].ExternalGenlockNo = ExternalGenlockNo;
      }
    }
  }
}

/***************************************************************************/
/*  SystemPresetName                               RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970206                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Name a specific preset number.                    */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          ParState 2: DecNum, String                        */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void SystemPresetName() {

  register int i;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        ucharval = (UC) atoi(Cmd);

        if ((ucharval == 0) || (ucharval > NoOfPresets))
          SCPIError = DataOutOfRange;
        else
          ucharval--;
      }
      else
        SCPIError = DataTypeErr;
      break;

    case P_String:
      if (ParState == 1) {
        ParState++;

        if (strlen(Cmd) > 16)
          SCPIError = DataOutOfRange;
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      switch (ParState) {
        case 0:
          SCPIError = MissingPar;
          break;

        case 1:
          Udr_PutChar('"');
          Udr_PutNChar(Preset[ucharval].Name);
          Udr_PutChar('"');
          Udr_PutNewline();
          break;

        default:
          SCPIError = SyntaxErr;
          break;
      }
    }
    else {
      if (ParState < 2)
        SCPIError = MissingPar;
        else {
        for (i=0; i < strlen(Cmd); i++)
          if (!PresetCharSet[Cmd[i]])
            break;

        if (i == strlen(Cmd))
          strcpy(Preset[ucharval].Name, Cmd);
        else
          SCPIError = ParErr;
      }
    }
  }
}

/***************************************************************************/
/*  SystemPresetDownload                             RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970206                          */
/* Revised:    991103                                       */
/*                                                  */
/*  Function:  Send a specific preset to another SPG                */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          The data is sent in four parts due to the small amount of  */
/*          SCPI/RS232 input buffer memory, Cmd[512]/128.          */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void SystemPresetDownload() {

  UI NoOfBytes, i;
  UL checksum;
  UC tmp;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        ucharval = (UC) atoi(Cmd);

        if ((ucharval == 0) || (ucharval > NoOfPresets))
          SCPIError = DataOutOfRange;
        else
          ucharval--;
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
        NoOfBytes = sizeof(Preset[0]);
        downPtr  = (UC*) &Preset[ucharval];

        do {
          sprintf(TmpBuffer, "#3%03d\0", 2*min(200, NoOfBytes) + 2 + 8);
//          TmpBuffer[5] = 0;

          Udr_PutNChar(TmpBuffer);

          Udr_PutChar((DownloadRevision >> 4) + DownloadOffset); // Revision
          Udr_PutChar((DownloadRevision & 0x0F) + DownloadOffset);

          checksum = DownloadRevision;

          for (i = 0; i < min(200, NoOfBytes); i++) {
            tmp = *downPtr >> 4;
            checksum += tmp;
            Udr_PutChar((char) (tmp + DownloadOffset));

            tmp = *downPtr & 0x0F;
            checksum += tmp;
            Udr_PutChar((char) (tmp + DownloadOffset));

            downPtr++;
          }

          Udr_PutChar(((checksum & 0xF0000000) >> 28) + DownloadOffset);
          Udr_PutChar(((checksum & 0x0F000000) >> 24) + DownloadOffset);
          Udr_PutChar(((checksum & 0x00F00000) >> 20) + DownloadOffset);
          Udr_PutChar(((checksum & 0x000F0000) >> 16) + DownloadOffset);
          Udr_PutChar(((checksum & 0x0000F000) >> 12) + DownloadOffset);
          Udr_PutChar(((checksum & 0x00000F00) >> 8) + DownloadOffset);
          Udr_PutChar(((checksum & 0x000000F0) >> 4) + DownloadOffset);
          Udr_PutChar(((checksum & 0x0000000F) + DownloadOffset));

          NoOfBytes -= min(200, NoOfBytes);

          if (NoOfBytes)
            Udr_PutChar(',');

        } while(NoOfBytes);

        Udr_PutNewline();
      }
    }
  }
}

/***************************************************************************/
/*  SystemPresetUpload                             RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970206                          */
/* Revised:    991103                                       */
/*                                                  */
/*  Function:  Receive a specific preset from another SPG            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          ParState 2: DecNum, Blockn                        */
/*          This routine MUST match SystemPresetDownload EXACTLY      */
/*          Command wil split the transmission into 4 bloack arrays    */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void SystemPresetUpload() {

  UI NoOfBytes, i;
  UL CalcChecksum, RecChecksum;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        ucharval = (UC) atoi(Cmd);

        if ((ucharval == 0) || (ucharval > NoOfPresets))
          SCPIError = DataOutOfRange;
        else {
          ucharval--;

          downPtr  = (UC*) &Preset[ucharval];
          UcharVal1 = ResponseReceived;
        }
      }
      else
        SCPIError = DataTypeErr;
      break;

    case P_Blockn:
      if (!UcharVal1) {

        CalcChecksum = RecChecksum = 0;

        NoOfBytes = strlen(Cmd);  // At this point Cmd[] has been stripped
                          //  of the number of bytes, (#3nnn).

        for (i = 0; i < NoOfBytes; i++) {
          Cmd[i] -= DownloadOffset;

          if (i < (NoOfBytes - 8))
            CalcChecksum += Cmd[i];
          else
            RecChecksum = (RecChecksum << 4) + Cmd[i];
        }

        if (((Cmd[0] << 4) + Cmd[1]) != DownloadRevision)
          UcharVal1 = InvalidVersion;
        else 
          if (RecChecksum != CalcChecksum)
            UcharVal1 = InvalidBlockData;
        else
          for (i = 2; i < NoOfBytes-8; i += 2)
            *downPtr++ = (Cmd[i] << 4) + Cmd[i+1];
      }

      if (ParState < 5)
        ParState++;
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else {
      if (ParState < 5) {
        SCPIError = MissingPar;      // FACTORY PRESET FOR PRESET UcharVar
      }
      else {
        if (!UcharVal1)
          ResponseReceived = 0xFF;
        else
          ResponseReceived = UcharVal1;
      }
    }
  }
}

/***************************************************************************/
/*  SystemDownload                                 RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970206                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Download actual setup from another SPG                */
/*  Remarks:    -                                        */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void SystemDownload() {

  SCPIError = ExecErr;
}

/***************************************************************************/
/*  SystemUpload                                                RS232CMD.C */
/*                                                                         */
/* Author:   Janusz Kuzminski, 25.06.2006                                  */
/* Revised:  13.10.2006                                                    */
/*                                                                         */
/* Purpose:  To set master into transparent mode for HD/DL board Fpga and  */
/*           uP  firmware download.                                        */
/* Returns:  Nothing                                                       */
/* Notes:    This function is invoked by the following SCPI command:       */
/*           :SYST:UPL n;                                                  */
/*           Parameter n, which is in IntVal, is the kind of board,        */
/*           combined with board number.  Left nibble of n carries board   */
/*           type:                                                         */
/*            1 - HD board PT8612                                          */
/*            2 - DL board PT8613                                          */
/*           right carries board number (1, 2 or 3).                       */
/*           n is passed to the transparent_mode() function, implementing  */
/*           'transparent mode' transfer of data between user and and      */
/*           board's uP software.                                          */
/*                                                                         */
/* Rev:      18.01.2007: This function is now also used for Master SW      */
/*                       update.  In that case, the argument in IntVal     */
/*                       must be 0x8000                                    */
/***************************************************************************/
void SystemUpload()
  {

    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;
      case P_DecNum:
        if (ParState == 0)
          {
          ParState++;
          IntVal = atoi(Cmd);
#if 0
          if ((IntVal < 1) || (IntVal > 3))
            SCPIError = DataOutOfRange;
#endif
          }
        else
          SCPIError = ParNotAllowed;
        break;
      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (IntVal == 0x8000)
            update_sw();
          else
            transparent_mode((UC)IntVal); /* IntVal carries board type/number */
          }
        }
      }

  /* SCPIError = ExecErr; */
  }

/***************************************************************************/
/*  StatusOperEvent                               RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970630                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set/read the status operation event register            */
/*  Remarks:    Reading the event register clears it                */
/*  Returns:    -                                        */
/*  Updates:    OperEventReg                                */
/***************************************************************************/
void StatusOperEvent() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      sprintf(TmpBuffer, "%d\n", OperEventReg);
      Udr_PutNChar(TmpBuffer);

      OperEventReg = 0;
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  StatusOperCondition                             RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970630                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Read the status questionable condition register          */
/*  Remarks:    No parameters                                */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void StatusOperCondition() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      sprintf(TmpBuffer, "%d\n", OperConditionReg);
      Udr_PutNChar(TmpBuffer);
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  StatusOperEnable                               RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970630                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set/read the status operaion enable register            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void StatusOperEnable() {

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        if (atol(Cmd) > 65535)
          SCPIError = DataOutOfRange;
        else
          IntVal = atoi(Cmd);
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState)
        SCPIError = ParNotAllowed;
      else {
        sprintf(TmpBuffer, "%d\n", OperEnableReg);
        Udr_PutNChar(TmpBuffer);
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else
        OperEnableReg = IntVal & 0xEFFF;
    }
  }
}

/***************************************************************************/
/*  StatusQuestEvent                               RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970630                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set/read the status questionablen event register        */
/*  Remarks:    Reading the event register clears it                */
/*  Returns:    -                                        */
/*  Updates:    QuestEventReg                                */
/***************************************************************************/
void StatusQuestEvent() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      sprintf(TmpBuffer, "%d\n", QuestEventReg);
      Udr_PutNChar(TmpBuffer);

      QuestEventReg = 0;
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  StatusQuestCondition                             RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970630                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Read the status operation condition register            */
/*  Remarks:    No parameters                                */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void StatusQuestCondition() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      sprintf(TmpBuffer, "%d\n", QuestConditionReg);
      Udr_PutNChar(TmpBuffer);
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  StatusQuestEnable                               RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970630                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set/read the status questionable enable register        */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void StatusQuestEnable() {

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        if (atol(Cmd) > 65535)
          SCPIError = DataOutOfRange;
        else
          IntVal = atoi(Cmd);
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState)
        SCPIError = ParNotAllowed;
      else {
        sprintf(TmpBuffer, "%d\n", QuestEnableReg);
        Udr_PutNChar(TmpBuffer);
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else
        QuestEnableReg = IntVal & 0xEFFF;
    }
  }
}

/***************************************************************************/
/*  StatusPreset                                 RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void StatusPreset() {
}

/***************************************************************************/
/*  StatusPTSuffix                                  RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void StatusPTSuffix() {

  if (NumSuffix != 5300)
    SCPIError = SyntaxErr;
}

/***************************************************************************/
/*  StatusPT                                      RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void StatusPT() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {
    if (CmdRequest && (NumSuffix == 5300)) {
      Udr_PutChar('"');

      if (ErrorStatus)
        Udr_CPutNChar(ActiveErrorsTxt);
      else {
        if (ErrorQueue[ErrorQueuePtr])
          Udr_CPutNChar(NoActiveErrorsTxt);
        else
          Udr_CPutNChar(NoErrorsTxt);
      }

      Udr_PutChar('"');
      Udr_PutNewline();
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  DiagnosticDisplay                                 RS232CMD.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void DiagnosticDisplay() {
}

/***************************************************************************/
/*  DiagnosticErrorQueueLast                           RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    991026                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void DiagnosticErrorQueueLast() {

  char tmpbuf[40];

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {

      ucharval = ErrorQueue[ErrorQueuePtr] & 0x000000FF;

      if (ErrorQueue[ErrorQueuePtr])
        GetErrorString(ErrorQueue[ErrorQueuePtr], tmpbuf);
      else
        Cstrcpy(tmpbuf, NoErrorDetectedTxt);

      sprintf(TmpBuffer, "%d", ucharval);
      Udr_PutNChar(TmpBuffer);

      Udr_PutChar(',');
  
      Udr_PutChar('"');
      Udr_PutNChar(tmpbuf);
      Udr_PutChar('"');
      Udr_PutNewline();
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  DiagnosticErrorQueueEntry                           RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 991026                          */
/* Revised:    991026                                       */
/*                                                  */
/*  Function:  Return an entry in the error queue                  */
/*  Remarks:    Query only                                  */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void DiagnosticErrorQueueEntry() {

  char tmpbuf[40];
  
  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState++;

        UcharVal1 = (UC) atoi(Cmd);

        if ((UcharVal1 == 0) || (UcharVal1 > 5))
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

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {

      ucharval = ErrorQueue[UcharVal1] & 0x000000FF;

      if (ErrorQueue[UcharVal1])
        GetErrorString(ErrorQueue[UcharVal1], tmpbuf);
      else
        Cstrcpy(tmpbuf, NoErrorDetectedTxt);

      sprintf(TmpBuffer, "%d", ucharval);
      Udr_PutNChar(TmpBuffer);

      Udr_PutChar(',');
  
      Udr_PutChar('"');
      Udr_PutNChar(tmpbuf);
      Udr_PutChar('"');
      Udr_PutNewline();
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  DiagnosticErrorQueueReset                           RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Reset the internal PT5300 error queue, NOT the SCPI queue  */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Internal error queue                            */
/***************************************************************************/
void DiagnosticErrorQueueReset()
  {
  register int i;

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError)
    {
    if (!CmdRequest)
      {
      for(i = 0; i < NoInErrorQueue; i++)
        ErrorQueue[i] = 0;

      ErrorStatus   = 0;
      ErrorQueuePtr = NoInErrorQueue-1;
      WarningLED    = OFF;
      UpdateLEDs();
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/***************************************************************************/
/*  DisplayContrast                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Adjust the contrast on the 40x2 LCD display            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          ParState 2: Char                              */
/*  Returns:    -                                        */
/*  Updates:                                            */
/***************************************************************************/
void DisplayContrast() {

  register int tmp;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      if (ParState == 0) {
        ParState = 1;

        if ((ucharval = (UC) atoi(Cmd)) > MaxContrast)
          SCPIError = DataOutOfRange;
      }
      else
        SCPIError = DataTypeErr;
      break;

    case P_Char:
      if (ParState == 0) {
        ParState = 2;

        ucharval = FindParameter(Cmd, MinMaxPar);

        if (ucharval > MAX)
          SCPIError = IllegalParValue;
      }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState == 1)
        SCPIError = DataTypeErr;
      else {
        if (ParState == 0)
          tmp = LCDContrast;
        else {
          if (ucharval == MIN)
            tmp = MinContrast;
          else
            tmp = MaxContrast;
        }

        sprintf(TmpBuffer, "%d\r\n", tmp);
        Udr_PutNChar(TmpBuffer);
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
        if (ParState == 1)
          LCDContrast = ucharval;
        else {
          if (ucharval == MIN)
            LCDContrast = MinContrast;
          else
            LCDContrast = MaxContrast;
        }
        SendContrast(LCDContrast);

        if (IICErrorCode)
          SCPIError = ExecErr;
      }
    }
  }
}

/***************************************************************************/
/*  Genlock                                       RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980915                                       */
/*                                                  */
/*  Function:  Return the Genlock Configuration.                  */
/*  Remarks:    Request only                                */
/*  Returns:    --                                        */
/*  Updates:     --                                        */
/***************************************************************************/
void Genlock() {

  if (ParameterType != P_NoParameter)
    SCPIError = ParNotAllowed;

  if (CmdExecute && !SCPIError) {

    if (CmdRequest) {
      if (UnlockedLED)
        Udr_CPutNChar(UNLOCKEDTxt);
      else {
        if ((GenlockConfig[ActiveGenlockNo].System < SyncLock625) &&
           (ActiveGenlockNo < SDIGenlock)) {
          if (burstLockOn)
            Udr_CPutNChar(GENLOCKEDTxt);
          else
            Udr_CPutNChar(SYNCLOCKEDTxt);
        }
        else
          Udr_CPutNChar(GENLOCKEDTxt);
      }

      Udr_PutChar(',');
      Udr_CPutNChar(GenlockInputPar[ActiveGenlockNo].LongName);

      Udr_PutChar(',');

      if (ActiveGenlockNo < Internal)
        Udr_CPutNChar(GenlockSystemPar[GenlockConfig[ActiveGenlockNo].System].LongName);
      else
        Udr_PutNChar("NA");

      Udr_PutChar(',');

      if (ActiveGenlockNo == SDIGenlock)
        SDISamplesToFLT(GenlockConfig[ActiveGenlockNo].System,\
                    GenlockConfig[ActiveGenlockNo].Delay,\
                    &sign, &Field, &Line, &HTime);
      else
        AnlSamplesToFLT(GenlockConfig[ActiveGenlockNo].System,\
                      GenlockConfig[ActiveGenlockNo].Delay,\
                      &sign, &Field, &Line, &HTime);

      PrintTimingFLT(sign, Field, Line, HTime);

      Udr_PutNewline();
    }
    else
      SCPIError = SyntaxErr;
  }
}

/***************************************************************************/
/*  GenlockInput                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the genlock input command                    */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void GenlockInput() {

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0) {
        ParState++;

        ucharval = FindParameter(Cmd, GenlockInputPar);

        if (ucharval > Internal2)
          SCPIError = IllegalParValue;
      }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState)
        SCPIError = ParNotAllowed;
      else {
        Udr_CPutNChar(GenlockInputPar[ActiveGenlockNo].LongName);
        Udr_PutNewline();
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
	    // Check if installed
	    if( (ucharval==SDIGenlock) )
	    {
		    if (!SDIGenlockUnit.Present)
		    	ucharval=ActiveGenlockNo;
		}
	    else if( (ucharval==GPSGenlock) )
	    {
		    if (!GPSGenlockUnit.Present)
		    	ucharval=ActiveGenlockNo;
		}
		
        if (ucharval != ActiveGenlockNo) {
          ActivePresetNo = 0;          // Clear possible preset

          if (Ptr == &status_menu[StatusPresetItem])
            Ptr->DispFunct();

          ActiveGenlockNo = ucharval;

          if (ucharval < Internal)      // If new input isn't internal..
            ExternalGenlockNo = ucharval;  //  update also external no

          GenlockUnitDrv(ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
        }
      }
    }
  }
}

/***************************************************************************/
/*  GenlockSystem                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the genlock system command                  */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void GenlockSystem() {

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0) {
        ParState++;

        ucharval = FindParameter(Cmd, GenlockSystemPar);

        if (ucharval > MHzLock10)
          SCPIError = IllegalParValue;
      }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState)
        SCPIError = ParNotAllowed;
      else {
        Udr_CPutNChar(GenlockSystemPar[GenlockConfig[ActiveGenlockNo].System].LongName);
        Udr_PutNewline();
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
        if (ActiveGenlockNo < Internal) {

          if (ActiveGenlockNo == SDIGenlock) {
            if ((ucharval < SDI625Lock) || (ucharval > SDI525Lock))
              SCPIError = ExecErr;
          }
          else {
            if ((ucharval == SDI625Lock) || (ucharval == SDI525Lock))
              SCPIError = ExecErr;
          }

          if (SCPIError == NoErr) {
            if (ucharval != GenlockConfig[ActiveGenlockNo].System) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              GenlockConfig[ActiveGenlockNo].System = ucharval;

              GenlockUnitDrv(ActiveGenlockNo, GenlockConfig[ActiveGenlockNo].System);
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
/*  GenlockDelay                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970206                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the genlock timing command                  */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, DecNum                      */
/*          ParState 3:  DecNum, DecNum, DecNum                  */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void GenlockDelay()
  {
  FindDelayParameters();

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParState)
        SCPIError = ParNotAllowed;
      else
        {
        if ((ActiveGenlockNo < Internal) && (GenlockConfig[ActiveGenlockNo].System < MHzLock358))
          {
          if (ActiveGenlockNo == SDIGenlock)
            SDISamplesToFLT(GenlockConfig[ActiveGenlockNo].System,\
                        GenlockConfig[ActiveGenlockNo].Delay,\
                        &sign, &Field, &Line, &HTime);
          else
            AnlSamplesToFLT(GenlockConfig[ActiveGenlockNo].System,\
                          GenlockConfig[ActiveGenlockNo].Delay,\
                          &sign, &Field, &Line, &HTime);

          PrintTimingFLT(sign, Field, Line, HTime);
          Udr_PutNewline();
          }
        }
      }
    else
      {
      if (ParState < 3)
        SCPIError = MissingPar;
      else
        {
        if (ActiveGenlockNo < Internal)
          {
          UlongVal = AnlFLTToSamples(\
                          GenlockConfig[ActiveGenlockNo].System,\
                          Field, Line, HTime);

          if (UlongVal != -1)
            {
            if (UlongVal != GenlockConfig[ActiveGenlockNo].Delay)
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              GenlockConfig[ActiveGenlockNo].Delay = UlongVal;

              TrxGenlockDelay(UlongVal);
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
/*  BBSuffix                                       RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980402                                       */
/*                                                  */
/*  Function:  Get suffix of the Black Burst generator              */
/* Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Suffix                                    */
/***************************************************************************/
void BBSuffix() {

  if (NumSuffix > 8)
    SCPIError = HdrSuffixOutOfRange;
  else
    Suffix = NumSuffix-1;
}

/***************************************************************************/
/*  BB                                           RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the Black Burst command.                    */
/* Remarks:    Request only                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void BB() {

  if (AnlBlkUnit[Suffix].Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {

        Udr_CPutNChar(AnlSystemPar[AnlBlkConfig[Suffix].System].LongName);
        Udr_PutChar(',');

        AnlSamplesToFLT(AnlBlkConfig[Suffix].System,\
                            AnlBlkConfig[Suffix].Delay,\
                                &sign, &Field, &Line, &HTime);

        PrintTimingFLT(sign, Field, Line, HTime);

        Udr_PutChar(',');

        sprintf(TmpBuffer, "%d\r\n", AnlBlkConfig[Suffix].ScHPhase);
        Udr_PutNChar(TmpBuffer);
      }
      else
        SCPIError = SyntaxErr;
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  BBSystem                                       RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the Black Burst system-command.                */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void BBSystem()
  {

  if (AnlBlkUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, AnlSystemPar);

          if (ucharval > NTSC)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(AnlSystemPar[AnlBlkConfig[Suffix].System].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != AnlBlkConfig[Suffix].System) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            AnlBlkConfig[Suffix].System = ucharval;

                              // Test if delay is valid
                              //  in the new system, (AFTER
                              //  the system is saved)
            AnlValidateSetting(AnlBlkUnit[Suffix].HWType, Suffix);

            TrxAnlBlkSystem(Suffix, ucharval, AnlBlkConfig[Suffix].Delay,\
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
/*  BBScHPhase                                     RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the Black Burst ScH-Phase command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void BBScHPhase() {

  if (AnlBlkUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState == 0) {
          ParState++;

          IntVal = atoi(Cmd);

          if ((IntVal < ScHPhaseLow) || (IntVal > ScHPhaseHigh))
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          sprintf(TmpBuffer, "%d\r\n", AnlBlkConfig[Suffix].ScHPhase);
          Udr_PutNChar(TmpBuffer);
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (IntVal != AnlBlkConfig[Suffix].ScHPhase) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            AnlBlkConfig[Suffix].ScHPhase = IntVal;

            TrxAnlBlkScHPhase(Suffix, IntVal);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  BBDelay                                       RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the Black Burst timing command.                */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, DecNum                      */
/*          ParState 3:  DecNum, DecNum, DecNum                  */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void BBDelay() {

  if (AnlBlkUnit[Suffix].Present) {

    FindDelayParameters();

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          AnlSamplesToFLT(AnlBlkConfig[Suffix].System,\
                            AnlBlkConfig[Suffix].Delay,\
                                &sign, &Field, &Line, &HTime);

          PrintTimingFLT(sign, Field, Line, HTime);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState < 3)
          SCPIError = MissingPar;
        else {
          UlongVal = AnlFLTToSamples(\
                  AnlBlkConfig[Suffix].System, Field, Line, HTime);

          if (UlongVal != -1) {
            if (UlongVal != AnlBlkConfig[Suffix].Delay) {

              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              AnlBlkConfig[Suffix].Delay = UlongVal;

              TrxAnlBlkDelay(Suffix, AnlBlkConfig[Suffix].System, UlongVal);
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
/*  BBVersion                                     RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    990317                                       */
/*                                                  */
/*  Function:  Handle the Black Burst version-command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          Request only                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void BBVersion() {

  if (AnlBlkUnit[Suffix].Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest)
        WriteUnitInformation(AnlBlkUnit[Suffix].HWType, Suffix);
      else
        SCPIError = SyntaxErr;
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AnlTPGSuffix                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Get suffix of the Analog-TPG.                      */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Suffix                                    */
/***************************************************************************/
void AnlTPGSuffix() {

  switch (NumSuffix) {
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
/*  AnlTPG                                       RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Return the setting of a Analog-TPG.                  */
/*  Remarks:    Request only.                                */
/* Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPG() {

  UC tmp, textEnable;

  if (AnlTPGUnit[Suffix].Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {

        tmp = AnlTPGConfig[Suffix].Pattern;  // tmp holds the pattern

        Udr_CPutNChar(AnlPatternPar[tmp].LongName);
        Udr_PutChar(',');

        tmp = AnlFindPatternType(tmp);    // tmp holds the pattern type
        textEnable = AnlTPGConfig[Suffix].TextEnable;

        if (tmp == ComplexPattern)
          textEnable &= 0x18;
        else
          textEnable &= 0x07;

        if (textEnable || (AnlTPGConfig[Suffix].TextStyle[tmp] & 0xF0))
          tmp = TRUE;
        else
          tmp = FALSE;

        Udr_CPutNChar(OffOnPar[tmp].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(AnlSystemPar[AnlTPGConfig[Suffix].System].LongName);
        Udr_PutChar(',');

        AnlSamplesToFLT(AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Delay,\
                      &sign, &Field, &Line, &HTime);

        PrintTimingFLT(sign, Field, Line, HTime);
        Udr_PutChar(',');

        sprintf(TmpBuffer, "%d\r\n", AnlTPGConfig[Suffix].ScHPhase);
        Udr_PutNChar(TmpBuffer);
      }
      else
        SCPIError = SyntaxErr;
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AnlTPGPattern                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG pattern command.                */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGPattern() {

  if (AnlTPGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, AnlPatternPar);

          if (ucharval > AnlVMT01)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(AnlPatternPar[AnlTPGConfig[Suffix].Pattern].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval == AnlValidatePattern(AnlTPGUnit[Suffix].HWType, Suffix,\
                          AnlTPGConfig[Suffix].System, ucharval)) {

            if (ucharval != AnlTPGConfig[Suffix].Pattern) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              AnlTPGConfig[Suffix].Pattern = ucharval;

              TrxAnlTPGPattern(Suffix, ucharval, AnlTPGAttrib(Suffix, ucharval));

              ConfigureAnlTPGText(Suffix, ucharval);
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
/*  AnlTPGAttribSuffix                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980403                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Get suffix of the Analog-TPG pattern-attribute staircase.  */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    AttribSuffix                                */
/***************************************************************************/
void AnlTPGAttribSuffix() {

  if (NumSuffix == 10)
    AttribSuffix = 10;
  else
    AttribSuffix = 0;
}

/***************************************************************************/
/*  AnlTPGAttribAPAL                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the Analog TPG pattern-attribute Anti-PAL.        */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          This command only applies for the Philips/FuBK patterns.    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGAttribAPAL() {

  if (AnlTPGUnit[Suffix].Present) {
    if (AttribAntiPAL == AnlValidateAttrib(AnlTPGUnit[Suffix].HWType, Suffix,
      AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Pattern, AttribAntiPAL)) 
      AnlTPGAttribCommon(AttribAntiPAL);
    else
      SCPIError = ExecErr;
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AnlTPGAttribPLUGE                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG pattern-attribute PLUGE.          */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          This command only applies for the Philips patterns.      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGAttribPLUGE() {

  if (AnlTPGUnit[Suffix].Present) {
    if (AttribPLUGE == AnlValidateAttrib(AnlTPGUnit[Suffix].HWType, Suffix,
      AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Pattern, AttribPLUGE))
      AnlTPGAttribCommon(AttribPLUGE);
    else
      SCPIError = ExecErr;
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AnlTPGAttribStair10                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG pattern-attribute Staircase 10step.  */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          This only applies to the Philips patterns.            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGAttribStair10() {

  if (AnlTPGUnit[Suffix].Present) {
    if (AttribSuffix == 10) {
      if (AttribStair10 == AnlValidateAttrib(AnlTPGUnit[Suffix].HWType, Suffix,
          AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Pattern, AttribStair10))
        AnlTPGAttribCommon(AttribStair10);
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
/*  AnlTPGAttribCircles                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980929                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the ANL-TPG pattern-attribute Circles.          */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          Circles is only available in the Philips 16:9 pattern      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGAttribCircles() {

  if (AnlTPGUnit[Suffix].Present) {
    if (AttribCircles == AnlValidateAttrib(AnlTPGUnit[Suffix].HWType, Suffix,
      AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Pattern, AttribCircles))
      AnlTPGAttribCommon(AttribCircles);
    else
      SCPIError = ExecErr;
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AnlTPGAttribCommon                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980929                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG pattern-attributes.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          This command only applies for the Philips 4:3-pattern      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGAttribCommon(UC Attribute) {

  UC tmp, pattern;

  pattern = AnlTPGConfig[Suffix].Pattern;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0) {
        ParState++;

        ucharval = FindParameter(Cmd, OffOnPar);

        if (ucharval > ON)
          SCPIError = IllegalParValue;
      }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      if (ParState)
        SCPIError = ParNotAllowed;
      else {
        tmp = ((AnlTPGAttrib(Suffix, pattern) & (1 << Attribute)) > 0);

        Udr_CPutNChar(OffOnPar[tmp].LongName);
        Udr_PutNewline();
      }
    }
    else {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
        tmp = pattern - AnlPhilips4x3;

        if (((AnlTPGConfig[Suffix].Attrib[tmp] & (1 << Attribute)) ^ (ucharval << Attribute))) {
          ActivePresetNo = 0;      // Clear possible preset

          if (Ptr == &status_menu[StatusPresetItem])
            Ptr->DispFunct();

          AnlTPGConfig[Suffix].Attrib[tmp] &= ~(1 << Attribute);
          AnlTPGConfig[Suffix].Attrib[tmp] |= (ucharval << Attribute);

          TrxAnlTPGPattern(Suffix, AnlTPGConfig[Suffix].Pattern,\
                            AnlTPGConfig[Suffix].Attrib[tmp]);
        }
      }
    }
  }
}

/***************************************************************************/
/*  AnlTPGTextStringSuffix                             RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Get suffix of the Analog-TPG text string suffix.        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    StringSuffix                                */
/***************************************************************************/
void AnlTPGTextStringSuffix() {

  UC tmp;

  if (AnlFindPatternType(AnlTPGConfig[Suffix].Pattern) == ComplexPattern)
    tmp = 2;
  else
    tmp = 3;

  if (NumSuffix > tmp)
    SCPIError = SuffixErr;
  else
    StringSuffix = NumSuffix - 1;
}

/***************************************************************************/
/*  AnlTPGTextString                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG text string command.            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          ParState 2: DecNum, Char                        */
/*          ParState 3: DecNum, String                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGTextString() {

  register int i, j;
  UC tmp, offset, noOfChar;
  code UC* CharPtr;

  if (AnlTPGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = DataTypeErr;
        break;

      case P_String:
        if (ParState == 0) {
          ParState += 2;

          switch (AnlTPGUnit[Suffix].HWType) {
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

          for (j = i = 0; i < strlen(Cmd); i++, j++) {
            switch ( Cmd[i]) {
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

            if (tmp && !CharPtr[(UC) tmp]) {
              SCPIError = DataOutOfRange;
              break;
            }
            else
              Cmd[j] = tmp;
          }
          Cmd[j] = 0;

          if (strlen(Cmd) > noOfChar)
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = DataTypeErr;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {

      tmp = 3*AnlFindPatternType(AnlTPGConfig[Suffix].Pattern) + StringSuffix;

      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          i = (AnlTPGConfig[Suffix].TextEnable & (1 << tmp)) > 0;

          Udr_CPutNChar(OffOnPar[i].LongName);
          Udr_PutNChar(",\"");
          Udr_PutNChar(AnlTPGConfig[Suffix].Text[tmp]);
          Udr_PutChar('"');
          Udr_PutNewline();
        }
      }
      else {
        switch (ParState ) {
          case 0:
            SCPIError = MissingPar;
            break;

          case 1:
            if ((AnlTPGConfig[Suffix].TextEnable & (1 << tmp)) ^ (ucharval << tmp)) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              AnlTPGConfig[Suffix].TextEnable &= ~(1 << tmp);
              AnlTPGConfig[Suffix].TextEnable |= (ucharval << tmp);

              TrxAnlTPGTextLineEnable(Suffix, StringSuffix+1, ucharval);
            }
            break;

          case 2:
            if (strcmp(AnlTPGConfig[Suffix].Text[tmp], Cmd)) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              strcpy(AnlTPGConfig[Suffix].Text[tmp], Cmd);
              TrxAnlTPGText(Suffix, StringSuffix+1, Cmd);
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
/*  AnlTPGTextStyle                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980318                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG text style command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          This command only applies for the Philips 4:3-pattern.    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGTextStyle() {

  UC type, tmp;

  if (AnlTPGUnit[Suffix].Present) {

    type = AnlFindPatternType(AnlTPGConfig[Suffix].Pattern);

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, StylePar);

          if ((ucharval > TPGComplexTextStyle) ||
             ((type == StandardPattern) && (ucharval == TPGComplexTextStyle)))
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          tmp = AnlTPGConfig[Suffix].TextStyle[type] & 0x0F;

          Udr_CPutNChar(StylePar[tmp].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != (AnlTPGConfig[Suffix].TextStyle[type] & 0x0F)) {

            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            ucharval |= (AnlTPGConfig[Suffix].TextStyle[type] & 0xF0);
            AnlTPGConfig[Suffix].TextStyle[type] = ucharval;

            TrxAnlTPGTextStyle(Suffix, AnlTPGConfig[Suffix].Pattern, ucharval);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AnlTPGTextPosition                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 000512                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG text positioning command.          */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          ParState 2: DecNum,DecNum                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGTextPosition() {

  UC type;

  if (AnlTPGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        switch (ParState)
        {
          case 0:
            ParState++;

            ucharval = atoi(Cmd);

            if ((ucharval < AnlXPosMin) || (UcharVal1 > AnlXPosMax))
              SCPIError = DataOutOfRange;
            break;

          case 1:
            ParState++;

            UcharVal1 = atoi(Cmd);

            if ((UcharVal1 < AnlYPosMin) || (UcharVal1 > AnlYPosMax))
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

    if (CmdExecute && !SCPIError) {

      type = AnlFindPatternType(AnlTPGConfig[Suffix].Pattern);

      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          sprintf(TmpBuffer, "%d,%d\r\n", 
                AnlTPGConfig[Suffix].TextPos[type][0],
                AnlTPGConfig[Suffix].TextPos[type][1]);
          Udr_PutNChar(TmpBuffer);
        }
      }
      else {
        if (ParState < 2)
          SCPIError = MissingPar;
        else
          if ((AnlTPGConfig[Suffix].TextStyle[type] & 0x0F) != TPGFreeTextStyle)
            SCPIError = SyntaxErr;  
        else {
          if ((ucharval != AnlTPGConfig[Suffix].TextPos[type][0]) ||
             (UcharVal1 != AnlTPGConfig[Suffix].TextPos[type][1]))
          {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            AnlTPGConfig[Suffix].TextPos[type][0] = ucharval;
            AnlTPGConfig[Suffix].TextPos[type][1] = UcharVal1;

            TrxAnlTPGTextPos(Suffix, 
                        AnlTPGConfig[Suffix].TextStyle[type],
                        ucharval, UcharVal1);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AnlTPGTextClock                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980318                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG time/date information.            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGTextClock() {

  UC tmp, type;

  if (AnlTPGUnit[Suffix].Present) {
    if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

      switch (ParameterType) {
        case P_NoParameter:
          ParState = 0;
          break;

        case P_Char:
          if (ParState == 0) {
            ParState++;

            ucharval = FindParameter(Cmd, TPGTextClockPar);

            if ((ucharval > TPGTextClockTimeDate) ||
               (ucharval == TPGTextClockDate))
              SCPIError = IllegalParValue;
          }
          else
            SCPIError = ParNotAllowed;
          break;

        default:
          SCPIError = DataTypeErr;
          break;
      }

      if (CmdExecute && !SCPIError) {
        type = AnlFindPatternType(AnlTPGConfig[Suffix].Pattern);

        if (CmdRequest) {
          if (ParState)
            SCPIError = ParNotAllowed;
          else {
            tmp = AnlTPGConfig[Suffix].TextStyle[type] >> 4;

            Udr_CPutNChar(TPGTextClockPar[tmp].LongName);
            Udr_PutNewline();
          }
        }
        else {
          if (ParState == 0)
            SCPIError = MissingPar;
          else {
            if (ucharval != (AnlTPGConfig[Suffix].TextStyle[type] >> 4)) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              AnlTPGConfig[Suffix].TextStyle[type] &= 0x0F;
              AnlTPGConfig[Suffix].TextStyle[type] += (ucharval << 4);

              TrxAnlTPGTextStyle(Suffix, AnlTPGConfig[Suffix].Pattern, AnlTPGConfig[Suffix].TextStyle[type]);
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
/*  AnlTPGSystem                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 98021                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG system-command.                */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          This command will possibly change pattern and/or delay if  */
/*             these are not valid in the selected system            */
/*  Returns:    --                                        */
/*  Updates:     --                                        */
/***************************************************************************/
void AnlTPGSystem() {

  UC pattern;

  if (AnlTPGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, AnlSystemPar);

          if (ucharval > NTSC)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(AnlSystemPar[AnlTPGConfig[Suffix].System].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != AnlTPGConfig[Suffix].System) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();
                              // Save the updated system
            AnlTPGConfig[Suffix].System = ucharval;

                              // Test if delay/pattern is valid
                              //  in the new system, (AFTER
                              //  the system is saved)
            AnlValidateSetting(AnlTPGUnit[Suffix].HWType, Suffix);
                              // Get validated pattern
            pattern = AnlTPGConfig[Suffix].Pattern;

            TrxAnlTPGSystem(Suffix, ucharval, AnlTPGConfig[Suffix].Delay,\
                         pattern, AnlTPGAttrib(Suffix, pattern));
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AnlTPGDelay                                     RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG delay command.                */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, DecNum                      */
/*          ParState 3:  DecNum, DecNum, DecNum                  */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGDelay() {

  if (AnlTPGUnit[Suffix].Present) {

    FindDelayParameters();

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          AnlSamplesToFLT(AnlTPGConfig[Suffix].System, AnlTPGConfig[Suffix].Delay,\
                      &sign, &Field, &Line, &HTime);

          PrintTimingFLT(sign, Field, Line, HTime);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState < 3)
          SCPIError = MissingPar;
        else {
          UlongVal = AnlFLTToSamples(AnlTPGConfig[Suffix].System,\
                               Field, Line, HTime);

          if (UlongVal != -1) {
            if (UlongVal != AnlTPGConfig[Suffix].Delay) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              AnlTPGConfig[Suffix].Delay = UlongVal;

              TrxAnlTPGDelay(Suffix, AnlTPGConfig[Suffix].System, UlongVal);
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
/*  AnlTPGScHPhase                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG ScH-Phase-command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGScHPhase()
  {
  if (AnlTPGUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState == 0)
          {
          ParState++;

          IntVal = atoi(Cmd);

          if ((IntVal < ScHPhaseLow) || (IntVal > ScHPhaseHigh))
            SCPIError = DataOutOfRange;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          sprintf(TmpBuffer, "%d\r\n", AnlTPGConfig[Suffix].ScHPhase);
          Udr_PutNChar(TmpBuffer);
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (IntVal != AnlTPGConfig[Suffix].ScHPhase)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            AnlTPGConfig[Suffix].ScHPhase = IntVal;

            TrxAnlTPGScHPhase(Suffix, IntVal);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  AnlTPGVersion                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990317                                       */
/*                                                  */
/*  Function:  Handle the Analog-TPG version command.                */
/*  Remarks:    Request only                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlTPGVersion() {

  if (AnlTPGUnit[Suffix].Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest)
        WriteUnitInformation(AnlTPGUnit[Suffix].HWType, Suffix);
      else
        SCPIError = SyntaxErr;
    }
  }
  else
    SCPIError = HardwareMissing;
}
/*
    TrxTLGFormat(TLGNoNdx, TLGConfig[TLGNoNdx].Format,
                TLGConfig[TLGNoNdx].Delay + DEF_DELAY(TLGConfig[TLGNoNdx].Format));
*/

/******************************************************************
 * NAME       void TLG()
 * PURPOSE    To handle TLG command
 * 
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None
 *
 * NOTES:     This function tackles TLG querry of the form:
 *            :OUTP:TLGx?;, i.e. it prints system and delay for 
 *            generator x.
 * 
 ******************************************************************/
void TLG()
  {
  UC    tmp;

  if (TLGUnit[0].Present || TLGUnit[4].Present || TLGUnit[8].Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        tmp = TLGConfig[Suffix].Format;  // tmp holds the pattern

        Udr_CPutNChar(TLGFormatPar[tmp].LongName); // 
        Udr_PutChar(',');

        TLGSamplesToFLT(TLGConfig[Suffix].Format, TLGConfig[Suffix].Delay, 
                        &sign, &Field, &Line, &HTime, tlg_fmt_table);

        PrintTimingFLTTLG(sign, Field, Line, HTime);

        Udr_PutNewline();
        }
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
  }

void TLGSuffix()
  {
  switch (NumSuffix)
    {
    case  1:
    case  2:
    case  3:
    case  4:
    case  5:
    case  6:
    case  7:
    case  8:
    case  9:
    case 10:
    case 11:
    case 12:
      Suffix = NumSuffix - 1;
      break;
    default:
      SCPIError = HdrSuffixOutOfRange;
      break;
    }
  }


void TLGFormat()
  {

  if (TLGUnit[0].Present || TLGUnit[4].Present || TLGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;
      case P_Char:
        if (ParState == 0)
          {
          ParState++;
          ucharval = FindParameter(Cmd, TLGFormatPar);

          if (ucharval > MAX_TLG_SYSTEMS)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;
      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(TLGFormatPar[TLGConfig[Suffix].Format].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != TLGConfig[Suffix].Format)
            {
            ActivePresetNo = 0;      // Clear possible preset
            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();
            TLGConfig[Suffix].Format = ucharval;
            //TLGConfig[Suffix].Delay  = 0;
            TLGConfig[Suffix].Delay = adjust_hd_samples(TLGConfig[Suffix].Delay, ucharval, tlg_fmt_table);

            TrxTLGDelay(Suffix, TLGConfig[Suffix].Format,
                        TLGConfig[Suffix].Delay,
                        GenlockConfig[ActiveGenlockNo].Delay);

            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }


void TLGDelay()
  {
  if (TLGUnit[0].Present || TLGUnit[4].Present || TLGUnit[8].Present)
    {
    FindDelayParameters(); //Htime Line and Field are updated

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          TLGSamplesToFLT(TLGConfig[Suffix].Format, TLGConfig[Suffix].Delay, 
                          &sign, &Field, &Line, &HTime, tlg_fmt_table);
  
          PrintTimingFLTTLG(sign, Field, Line, HTime);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState < 3)
          SCPIError = MissingPar;
        else
          {
          UlongVal = TLGFLTToSamples(TLGConfig[Suffix].Format, Field, Line, HTime,
                                     tlg_fmt_table);
          if (UlongVal != -1)
            {
            if (UlongVal != TLGConfig[Suffix].Delay)
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              TLGConfig[Suffix].Delay = UlongVal;  /* Save new Delay... */

              TrxTLGDelay(Suffix, TLGConfig[Suffix].Format,
                          TLGConfig[Suffix].Delay,
                          GenlockConfig[ActiveGenlockNo].Delay);
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
/*  TLGVersion                                                 RS232CMD.C  */
/*                                                                         */
/* Author:    Janusz Kuzminski                                             */
/* Revised:   23.12.2006                                                   */
/*                                                                         */
/*  Function:  Return the version of a TLG.                                */
/*  Remarks:    Request only                                               */
/*              :OUTP:TLG5:VERS?;                                          */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void TLGVersion()
  {
  if (TLGUnit[Suffix].Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        WriteUnitInformation(TLGUnit[Suffix].HWType, Suffix);
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
  }



/***************************************************************************/
/*  SDITSGSuffix                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Get suffix of the SDI TSG.                        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Suffix                                    */
/***************************************************************************/
void SDITSGSuffix() {

  switch (NumSuffix) {
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
/*  SDITSG                                     RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Return the setting of a SDI-TSG.                    */
/* Remarks:    Request only.                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITSG() {

  if (SDITSGUnit[Suffix].Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {

        Udr_CPutNChar(SDIPatternPar[SDITSGConfig[Suffix].Pattern].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(SDISystemPar[SDITSGConfig[Suffix].System].LongName);
        Udr_PutChar(',');

        SDISamplesToFLT(SDITSGConfig[Suffix].System,\
                            SDITSGConfig[Suffix].Delay,\
                                &sign, &Field, &Line, &HTime);

        PrintTimingFLT(sign, Field, Line, HTime);
        Udr_PutChar(',');

        Udr_CPutNChar(OffOnPar[SDITSGConfig[Suffix].EDH].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(TSGAudioSignalPar[SDITSGConfig[Suffix].AudioSignal].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(TSGAudioLevelPar[SDITSGConfig[Suffix].AudioLevel].LongName);
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
/*  SDITSGPattern                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TSG pattern command.                  */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITSGPattern() {

  if (SDITSGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, SDIPatternPar);

          if (ucharval > SDIVMT01)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(SDIPatternPar[SDITSGConfig[Suffix].Pattern].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval == SDIValidatePattern(SDITSGUnit[Suffix].HWType, Suffix,\
                         SDITSGConfig[Suffix].System, ucharval)) {

            if (ucharval != SDITSGConfig[Suffix].Pattern) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              SDITSGConfig[Suffix].Pattern = ucharval;

              TrxSDITSGPattern(Suffix, ucharval);
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
/*  SDITSGSystem                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TSG system command.                  */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          This command will possibly change pattern and/or delay if  */
/*             these are not valid in the selected system            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITSGSystem() {

  if (SDITSGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, SDISystemPar);

          if (ucharval > SDI525)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(SDISystemPar[SDITSGConfig[Suffix].System].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != SDITSGConfig[Suffix].System) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITSGConfig[Suffix].System = ucharval;

                              // Test if delay/pattern is valid
                              //  in the new system, (AFTER
                              //  the system is saved)
            SDIValidateSetting(SDITSGUnit[Suffix].HWType, Suffix);

            TrxSDITSGSystem(Suffix, ucharval, SDITSGConfig[Suffix].Delay,\
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
/*  SDITSGDelay                                     RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TSG delay command.                  */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, DecNum                      */
/*          ParState 3:  DecNum, DecNum, DecNum                  */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITSGDelay()
  {
  if (SDITSGUnit[Suffix].Present)
    {
    FindDelayParameters();

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          SDISamplesToFLT(SDITSGConfig[Suffix].System,\
                              SDITSGConfig[Suffix].Delay,\
                              &sign, &Field, &Line, &HTime);

          PrintTimingFLT(sign, Field, Line, HTime);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState < 3)
          SCPIError = MissingPar;
        else {
          UlongVal = SDIFLTToSamples(\
              SDITSGConfig[Suffix].System, Field, Line, HTime);

          if (UlongVal != -1)
            {
            if (UlongVal != SDITSGConfig[Suffix].Delay)
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              SDITSGConfig[Suffix].Delay = UlongVal;

              TrxSDITSGDelay(Suffix, SDITSGConfig[Suffix].System, UlongVal);
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
/*  SDITSGEDHInsert                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TSG EDH insertion command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITSGEDHInsert()
  {
  if (SDITSGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(OffOnPar[SDITSGConfig[Suffix].EDH].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != SDITSGConfig[Suffix].EDH) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITSGConfig[Suffix].EDH = ucharval;

            TrxSDITSGEDH(Suffix, ucharval);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITSGVersion                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990317                                       */
/*                                                  */
/*  Function:  Handle the SDI-TSG version command.                  */
/*  Remarks:    Request only                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITSGVersion() {

  if (SDITSGUnit[Suffix].Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      if (CmdRequest)
        WriteUnitInformation(SDITSGUnit[Suffix].HWType, Suffix);
      else
        SCPIError = SyntaxErr;
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITSGAudioSignal                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TSG audio-signal command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITSGAudioSignal() {

  if (SDITSGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, TSGAudioSignalPar);

          if (ucharval > TSGAudioStereo1kHz)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
      break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(TSGAudioSignalPar[SDITSGConfig[Suffix].AudioSignal].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != SDITSGConfig[Suffix].AudioSignal) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITSGConfig[Suffix].AudioSignal = ucharval;

            TrxSDITSGAudioSignal(Suffix, ucharval);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITSGAudioLevel                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TSG audio-level command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITSGAudioLevel() {

  if (SDITSGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, TSGAudioLevelPar);

          if (ucharval > TSGAudioLevel18dB)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(TSGAudioLevelPar[SDITSGConfig[Suffix].AudioLevel].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != SDITSGConfig[Suffix].AudioLevel) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITSGConfig[Suffix].AudioLevel = ucharval;

            TrxSDITSGAudioLevel(Suffix, ucharval);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITPGSuffix                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Get suffix of the SDI-TPG.                        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Suffix                                    */
/***************************************************************************/
void SDITPGSuffix() {

  switch (NumSuffix) {
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
/*  SDITPG                                       RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980512                                       */
/*                                                  */
/*  Function:  Return the setting of a SDI-TPG.                    */
/* Remarks:    Request only.                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPG() {

  UC tmp, textEnable;

  if (SDITPGUnit[Suffix].Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {

        tmp = SDITPGConfig[Suffix].Pattern;  // tmp holds the pattern

        Udr_CPutNChar(SDIPatternPar[tmp].LongName);
        Udr_PutChar(',');
        /*  text on/off */
        if (SDITPGUnit[Suffix].HWType != PT8603)
          {
          tmp = SDIFindPatternType(tmp);    // tmp holds the pattern type
          textEnable = SDITPGConfig[Suffix].TextEnable;
  
          if (tmp == ComplexPattern)
            textEnable &= 0x18;
          else
            textEnable &= 0x07;
  
          if (textEnable || (SDITPGConfig[Suffix].TextStyle[tmp] & 0xF0))
            tmp = TRUE;
          else
            tmp = FALSE;
  
          Udr_CPutNChar(OffOnPar[tmp].LongName);
          Udr_PutChar(',');
          }
        else
          {
          if (SDITPGConfig[Suffix].TextEnable)
            tmp = TRUE;
          else
            tmp = FALSE;
          Udr_CPutNChar(OffOnPar[tmp].LongName);
          Udr_PutChar(',');
          }
        /*  text on/off */
        Udr_CPutNChar(SDISystemPar[SDITPGConfig[Suffix].System].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(OffOnPar[SDITPGConfig[Suffix].EDH].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(TPGAudioSignalPar[SDITPGConfig[Suffix].AudioSignal].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(TPGAudioLevelPar[SDITPGConfig[Suffix].AudioLevel].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(TPGAudioGroupPar[SDITPGConfig[Suffix].AudioGroup].LongName);
        Udr_PutChar(',');

        SDISamplesToFLT(SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Delay,\
                    &sign, &Field, &Line, &HTime);

        PrintTimingFLT(sign, Field, Line, HTime);
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
/*  SDITPGPattern                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG pattern command.                  */
/*  Remarks:    ParState 0: Idle                               */
/*              ParState 1: Char                              */
/*  Returns:     --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGPattern()
 {

  if (SDITPGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, SDIPatternPar);

          if (ucharval > SDIVMT01)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(SDIPatternPar[SDITPGConfig[Suffix].Pattern].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {                    // Do pattern exist in system ?
          if (ucharval == SDIValidatePattern(SDITPGUnit[Suffix].HWType, Suffix,\
                          SDITPGConfig[Suffix].System, ucharval)) {

            if (ucharval != SDITPGConfig[Suffix].Pattern) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              SDITPGConfig[Suffix].Pattern = ucharval;

              TrxSDITPGPattern(Suffix, ucharval, SDITPGAttrib(Suffix, ucharval));

              ConfigureSDITPGText(Suffix, ucharval);
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
/*  SDITPGAttribSuffix                                         RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980403                                    */
/* Revised:    980611                                                      */
/*                                                                         */
/*  Function:  Get suffix of the SDI-TPG pattern-attribute staircase.      */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    AttribSuffix                                               */
/***************************************************************************/
void SDITPGAttribSuffix()
  {
  if (NumSuffix == 10)
    AttribSuffix = 10;
  else
    AttribSuffix = 0;
  }

/***************************************************************************/
/*  SDITPGAttribAPAL                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG pattern-attribute Anti-PAL.          */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGAttribAPAL() {

  if (SDITPGUnit[Suffix].Present) {
    if (AttribAntiPAL == SDIValidateAttrib(SDITPGUnit[Suffix].HWType, Suffix,
        SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribAntiPAL)) 
      SDITPGAttribCommon(AttribAntiPAL);
    else
      SCPIError = ExecErr;
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITPGAttribPLUGE                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG pattern-attribute PLUGE.            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          PLUGE is only available in the Philips pattern          */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGAttribPLUGE() {

  if (SDITPGUnit[Suffix].Present) {
    if (AttribPLUGE == SDIValidateAttrib(SDITPGUnit[Suffix].HWType, Suffix,
      SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribPLUGE))
      SDITPGAttribCommon(AttribPLUGE);
    else
      SCPIError = ExecErr;
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITPGAttribStair10                                        RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    990305                                                      */
/*                                                                         */
/*  Function:  Handle the SDI-TPG pattern-attribute Staircase 10step.      */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*              Staircase 10 is only available in the Philips pattern      */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void SDITPGAttribStair10()
  {
  if (SDITPGUnit[Suffix].Present)
    {
    if (AttribSuffix == 10)
      {
      if (AttribStair10 == SDIValidateAttrib(SDITPGUnit[Suffix].HWType, Suffix,
        SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribStair10))
        SDITPGAttribCommon(AttribStair10);
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
/*  SDITPGAttribMotion                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG pattern-attribute Motion.            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGAttribMotion()
  {
  if (SDITPGUnit[Suffix].Present)
    {
    if (AttribMotion == SDIValidateAttrib(SDITPGUnit[Suffix].HWType, Suffix,
        SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribMotion))
      SDITPGAttribCommon(AttribMotion);
    else
      SCPIError = ExecErr;
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  SDITPGAttribCircles                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG pattern-attribute Circles.          */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          Circles is only available in the Philips 16:9 pattern      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGAttribCircles() {

  if (SDITPGUnit[Suffix].Present) {
    if (AttribCircles == SDIValidateAttrib(SDITPGUnit[Suffix].HWType, Suffix,
      SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribCircles))
      SDITPGAttribCommon(AttribCircles);
    else
      SCPIError = ExecErr;
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITPGAttribLogo                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    990305                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG pattern-attribute Logo.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGAttribLogo() {

  if (SDITPGUnit[Suffix].Present) {
    if (AttribLogo == SDIValidateAttrib(SDITPGUnit[Suffix].HWType, Suffix,
      SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Pattern, AttribLogo))
      SDITPGAttribCommon(AttribLogo);
    else
      SCPIError = ExecErr;
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITPGAttribCommon                                         RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    980625                                                      */
/*                                                                         */
/*  Function:  Handle the SDI-TPG pattern attributes.                      */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void SDITPGAttribCommon(UC Attribute)
  {
  UC tmp, pattern;

  pattern = SDITPGConfig[Suffix].Pattern;

  switch (ParameterType)
    {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0)
        {
        ParState++;

        ucharval = FindParameter(Cmd, OffOnPar);

        if (ucharval > ON)
          SCPIError = IllegalParValue;
        }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
    }

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParState)
        SCPIError = ParNotAllowed;
      else
        {
        tmp = ((SDITPGAttrib(Suffix, pattern) & (1 << Attribute)) > 0);

        Udr_CPutNChar(OffOnPar[tmp].LongName);
        Udr_PutNewline();
        }
      }
    else
      {
      if (ParState == 0)
        SCPIError = MissingPar;
      else {
        tmp = pattern - SDIPhilips4x3;

        if (((SDITPGConfig[Suffix].Attrib[tmp] & (1 << Attribute)) ^ (ucharval << Attribute))) {
          ActivePresetNo = 0;      // Clear possible preset

          if (Ptr == &status_menu[StatusPresetItem])
            Ptr->DispFunct();

          SDITPGConfig[Suffix].Attrib[tmp] &= ~(1 << Attribute);
          SDITPGConfig[Suffix].Attrib[tmp] |= (ucharval << Attribute);

          TrxSDITPGPattern(Suffix, SDITPGConfig[Suffix].Pattern,\
                            SDITPGConfig[Suffix].Attrib[tmp]);
          }
        }
      }
    }
  }

/***************************************************************************/
/*  SDITPGTextStringSuffix                             RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Get suffix of the SDI-TPG text string.                */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    StringSuffix                                */
/***************************************************************************/

void SDITPGTextStringSuffix()
  {
  UC tmp;

  if (SDIFindPatternType(SDITPGConfig[Suffix].Pattern) == ComplexPattern)
    tmp = 2;
  else
    tmp = 3;

  if (NumSuffix > tmp)
    SCPIError = SuffixErr;
  else
    StringSuffix = NumSuffix - 1;
  }

/***************************************************************************/
/*  SDITPGTextString                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG test string command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          ParState 2: DecNum, Char                        */
/*          ParState 3: DecNum, String                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGTextString()
  {
  register int i, j;
  UC tmp, offset, noOfChar;
  code UC* CharPtr;

  if (SDITPGUnit[Suffix].HWType == PT8603)
    {
    do_8603Text();
    return;
    }
  if (SDITPGUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = DataTypeErr;
        break;

      case P_String:
        if (ParState == 0)
          {
          ParState += 2;

          switch (SDITPGUnit[Suffix].HWType)
            {
            case PT8602:
              CharPtr = StdTextCharSet;
              noOfChar = PT860xMaxNoChar;
              break;

            default:
              CharPtr = ExtTextCharSet;
              noOfChar = PT863xMaxNoChar;
              break;
            }

          offset = 0;

          for (j = i = 0; i < strlen(Cmd); i++, j++)
            {
            switch ( Cmd[i])
              {
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

            if (tmp && !CharPtr[(UC) tmp])
              {
              SCPIError = DataOutOfRange;
              break;
              }
            else
              Cmd[j] = tmp;
            }
          Cmd[j] = 0;

          if (strlen(Cmd) > noOfChar)
            SCPIError = DataOutOfRange;
          }
        else
          SCPIError = DataTypeErr;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      tmp = 3*SDIFindPatternType(SDITPGConfig[Suffix].Pattern) + StringSuffix;

      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          i = (SDITPGConfig[Suffix].TextEnable & (1 << tmp)) > 0;

          Udr_CPutNChar(OffOnPar[i].LongName);
          Udr_PutNChar(",\"");
          Udr_PutNChar(SDITPGConfig[Suffix].Text[tmp]);
          Udr_PutChar('"');
          Udr_PutNewline();
          }
        }
      else
        {
        switch (ParState)
          {
          case 0:
            SCPIError = MissingPar;
            break;

          case 1: /* user entered ON/OFF */
            if ((SDITPGConfig[Suffix].TextEnable & (1 << tmp)) ^ (ucharval << tmp))
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              SDITPGConfig[Suffix].TextEnable &= ~(1 << tmp);
              SDITPGConfig[Suffix].TextEnable |= (ucharval << tmp);

              TrxSDITPGTextLineEnable(Suffix, StringSuffix+1, ucharval);
              }
            break;

          case 2: /* user entered some string */
            if (strcmp(SDITPGConfig[Suffix].Text[tmp], Cmd))
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              strcpy(SDITPGConfig[Suffix].Text[tmp], Cmd);
              TrxSDITPGText(Suffix, StringSuffix+1, Cmd);
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
/*  do_8603Text                                                RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   16.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the SDI-TPG test string command for PT8603           */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*              ParState 2: DecNum, Char                                   */
/*              ParState 3: DecNum, String                                 */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void do_8603Text()
  {
  register int i, j;
  UC tmp, noOfChar;
  code UC* CharPtr;

  if (SDITPGUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:         // user entered ON/OFF
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = DataTypeErr;
        break;

      case P_String:         // user entered some string
        if (ParState == 0)
          {
          ParState += 2;

          CharPtr  = StdTextCharSet;
          noOfChar = PT8603MaxNoChar;

          for (j = i = 0; i < strlen(Cmd); i++, j++)
            {
            tmp = Cmd[i];

            if (tmp && !CharPtr[(UC) tmp])
              {
              SCPIError = DataOutOfRange;
              break;
              }
            else
              Cmd[j] = tmp;
            }
          Cmd[j] = 0;

          if (strlen(Cmd) > noOfChar)
            SCPIError = DataOutOfRange;
          }
        else
          SCPIError = DataTypeErr;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      } //switch (ParameterType)

    if (CmdExecute && !SCPIError)
      {
      tmp = StringSuffix;

      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          i = SDITPGConfig[Suffix].TextEnable;

          Udr_CPutNChar(OffOnPar[i].LongName);
          Udr_PutNChar(",\"");
          Udr_PutNChar(SDITPGConfig[Suffix].Text[tmp]);
          Udr_PutChar('"');
          Udr_PutNewline();
          }
        }
      else
        {
        switch (ParState)
          {
          case 0:
            SCPIError = MissingPar;
            break;

          case 1:               // user entered ON/0FF ?
            if (SDITPGConfig[Suffix].TextEnable != ucharval)
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              SDITPGConfig[Suffix].TextEnable = ucharval;

              TrxSDITPGTextLineEnable(Suffix, 1, ucharval);
              }
            break;

          case 2:               // user entered some text ?
            if (strcmp(SDITPGConfig[Suffix].Text[tmp], Cmd))
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              strcpy(SDITPGConfig[Suffix].Text[tmp], Cmd);
              TrxSDITPGText(Suffix, StringSuffix+1, Cmd);
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
/*  SDITPGTextStyle                                            RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980318                                    */
/* Revised:    000608                                                      */
/*                                                                         */
/*  Function:  Handle the SDI-TPG text-style command.                      */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/*  Modified:  Pt8603 has no style, return with SCPIError = ParNotAllowed  */ 
/***************************************************************************/
void SDITPGTextStyle()
  {
  UC type, tmp;

  if (SDITPGUnit[Suffix].HWType == PT8603)
    {
    SCPIError = SyntaxErr;
    return;;
    }

  if (SDITPGUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, StylePar);

          if ((ucharval > TPGComplexTextStyle) ||
             ((type == StandardPattern) && (ucharval == TPGComplexTextStyle)))
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      type = SDIFindPatternType(SDITPGConfig[Suffix].Pattern);

      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          tmp = SDITPGConfig[Suffix].TextStyle[type] & 0x0F;

          Udr_CPutNChar(StylePar[tmp].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != (SDITPGConfig[Suffix].TextStyle[type] & 0x0F))
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            ucharval |= (SDITPGConfig[Suffix].TextStyle[type] & 0xF0);
            SDITPGConfig[Suffix].TextStyle[type] = ucharval;

            TrxSDITPGTextStyle(Suffix, SDITPGConfig[Suffix].Pattern, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  SDITPGTextMovement                                         RS232CMD.C  */
/*                                                                         */
/* Author:    Janusz Kuzminski                                             */
/* Revised:   16.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the SDI-TPG text-movement command, in case of 8603   */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/*  Modified:  Pt8603 has no style, return with SCPIError = ParNotAllowed  */ 
/***************************************************************************/
void SDITPGTextMovement()
  {
  if (SDITPGUnit[Suffix].HWType != PT8603)
    {
    SCPIError = SyntaxErr;
    return;;
    }

  if (SDITPGUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(OffOnPar[SDITPGConfig[Suffix].TextMovement].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != SDITPGConfig[Suffix].TextMovement)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITPGConfig[Suffix].TextMovement = ucharval;

            SndInt(SDITPGUnit[Suffix].Address, "SH", ucharval);  // new for 8603: text move
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  SDITPGTextPositioning                                      RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 000512                                    */
/* Revised:    000608                                                      */
/*                                                                         */
/*  Function:  Handle the SDI-TPG text-positioning command.                */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*              ParState 2: DecNum,DecNum                                  */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void SDITPGTextPosition()
  {
  UC type;

  if (SDITPGUnit[Suffix].HWType == PT8603)
    {
    do_8603TextPos();
    return;
    }

  if (SDITPGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        switch (ParState)
        {
          case 0:
            ParState++;

            ucharval = atoi(Cmd);

            if ((ucharval < SDIXPosMin) || (UcharVal1 > SDIXPosMax))
              SCPIError = DataOutOfRange;
            break;

          case 1:
            ParState++;

            UcharVal1 = atoi(Cmd);

            if ((UcharVal1 < SDIYPosMin) || (UcharVal1 > SDIYPosMax))
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

    if (CmdExecute && !SCPIError) {

      type = SDIFindPatternType(SDITPGConfig[Suffix].Pattern);

      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          sprintf(TmpBuffer, "%d,%d\r\n", 
                SDITPGConfig[Suffix].TextPos[type][0],
                SDITPGConfig[Suffix].TextPos[type][1]);
          Udr_PutNChar(TmpBuffer);
        }
      }
      else {
        if (ParState < 2)
          SCPIError = MissingPar;
        else
          if ((SDITPGConfig[Suffix].TextStyle[type] & 0x0F) != TPGFreeTextStyle)
            SCPIError = SyntaxErr;  
        else {
          if ((ucharval != SDITPGConfig[Suffix].TextPos[type][0]) ||
             (UcharVal1 != SDITPGConfig[Suffix].TextPos[type][1]))
          {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITPGConfig[Suffix].TextPos[type][0] = ucharval;
            SDITPGConfig[Suffix].TextPos[type][1] = UcharVal1;

            TrxSDITPGTextPos(Suffix,
                        SDITPGConfig[Suffix].TextStyle[type],
                         ucharval, UcharVal1);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  do_8603TextPos                                             RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   17.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the SDI-TPG text-positioning command for PT8603      */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*              ParState 2: DecNum,DecNum                                  */
/*               ucharval  = Xpos                                          */
/*               UcharVal1 = Ypos                                          */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void do_8603TextPos()
  {
  UC Xpos;
  UC Ypos;
  UC tmp;

  if (SDITPGUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        switch (ParState)
          {
          case 0:
            ParState++;

            ucharval = atoi(Cmd);

            if ((ucharval < PT8603XPosMin) || (ucharval > PT8603XPosMax))
              SCPIError = DataOutOfRange;
            break;

          case 1:
            ParState++;

            UcharVal1 = atoi(Cmd);

            if ((UcharVal1 < PT8603YPosMin) || (UcharVal1 > PT8603YPosMax))
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

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          sprintf(TmpBuffer, "%d,%d\r\n", 
                SDITPGConfig[Suffix].TextPos[0][0],
                SDITPGConfig[Suffix].TextPos[0][1]);
          Udr_PutNChar(TmpBuffer);
          }
        }
      else
        {
        if (ParState < 2)
          SCPIError = MissingPar;
        else
          {
          if ((ucharval != SDITPGConfig[Suffix].TextPos[0][0]) ||
             (UcharVal1 != SDITPGConfig[Suffix].TextPos[0][1]))
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITPGConfig[Suffix].TextPos[0][0] = ucharval;
            SDITPGConfig[Suffix].TextPos[0][1] = UcharVal1;

            Xpos = ucharval;
            Ypos = UcharVal1;
    
            if (SDITPGConfig[Suffix].System == SDI625)
              tmp = 22*Ypos+8;
            else
              tmp = 18*Ypos+6;
    
            SndInt3(SDITPGUnit[Suffix].Address, "SJ", (UI)1, (UI)(4 + Xpos * 2), (UI)tmp);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  SDITPGTextClock                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980318                          */
/* Revised:    000608                                       */
/*                                                  */
/*  Function:  Handle the SDI- TPG clock command.                  */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGTextClock() {

  UC tmp, type;

  if (SDITPGUnit[Suffix].Present) {

    if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

      switch (ParameterType) {
        case P_NoParameter:
          ParState = 0;
          break;

        case P_Char:
          if (ParState == 0) {
            ParState++;

            ucharval = FindParameter(Cmd, TPGTextClockPar);

            if ((ucharval > TPGTextClockTimeDate) || (ucharval == TPGTextClockDate))
              SCPIError = IllegalParValue;
          }
          else
            SCPIError = ParNotAllowed;
          break;

        default:
          SCPIError = DataTypeErr;
          break;
      }

      if (CmdExecute && !SCPIError) {
        type = SDIFindPatternType(SDITPGConfig[Suffix].Pattern);

        if (CmdRequest) {
          if (ParState)
            SCPIError = ParNotAllowed;
          else {
            tmp = SDITPGConfig[Suffix].TextStyle[type] >> 4;

            Udr_CPutNChar(TPGTextClockPar[tmp].LongName);
            Udr_PutNewline();
          }
        }
        else {
          if (ParState == 0)
            SCPIError = MissingPar;
          else {
            if (ucharval != (SDITPGConfig[Suffix].TextStyle[type] >> 4)) {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              SDITPGConfig[Suffix].TextStyle[type] &= 0x0F;
              SDITPGConfig[Suffix].TextStyle[type] += (ucharval << 4);

              TrxSDITPGTextStyle(Suffix, SDITPGConfig[Suffix].Pattern, SDITPGConfig[Suffix].TextStyle[type]);
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
/*  SDITPGSystem                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG system command.                  */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          This command will possibly change pattern and/or delay if  */
/*             these are not valid in the selected system            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGSystem() {

  UC pattern;

  if (SDITPGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, SDISystemPar);

          if (ucharval > SDI525)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(SDISystemPar[SDITPGConfig[Suffix].System].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != SDITPGConfig[Suffix].System) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITPGConfig[Suffix].System = ucharval;

                              // Test if delay/pattern is valid
                              //  in the new system, (AFTER
                              //  the system is saved)
            SDIValidateSetting(SDITPGUnit[Suffix].HWType, Suffix);
                              // Get validated pattern
            pattern = SDITPGConfig[Suffix].Pattern;

            TrxSDITPGSystem(Suffix, ucharval, SDITPGConfig[Suffix].Delay,\
                        pattern, SDITPGAttrib(Suffix, pattern));
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITPGEDHInsert                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG EDH insertion command.              */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGEDHInsert() {

  if (SDITPGUnit[Suffix].Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(OffOnPar[SDITPGConfig[Suffix].EDH].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != SDITPGConfig[Suffix].EDH) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();
            SDITPGConfig[Suffix].EDH = ucharval;

            TrxSDITPGEDH(Suffix, ucharval);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDITPGDelay                                     RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the SDI-TPG delay command.                  */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, DecNum                      */
/*          ParState 3:  DecNum, DecNum, DecNum                  */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGDelay()
  {
  if (SDITPGUnit[Suffix].Present)
    {
    FindDelayParameters();

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          SDISamplesToFLT(SDITPGConfig[Suffix].System, SDITPGConfig[Suffix].Delay,\
                          &sign, &Field, &Line, &HTime);

          PrintTimingFLT(sign, Field, Line, HTime);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState < 3)
          SCPIError = MissingPar;
        else
          {
          UlongVal = SDIFLTToSamples(SDITPGConfig[Suffix].System,\
                             Field, Line, HTime);

          if (UlongVal != -1)
            {
            if (UlongVal != SDITPGConfig[Suffix].Delay)
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              SDITPGConfig[Suffix].Delay = UlongVal;

              TrxSDITPGDelay(Suffix, SDITPGConfig[Suffix].System, UlongVal);
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
/*  SDITPGVersion                                              RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    990317                                                      */
/*                                                                         */
/*  Function:  Return the version of a SDI-TPG.                            */
/*  Remarks:    Request only                                               */
/*              :outp:stpg1:vers?;                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void SDITPGVersion()
  {
  if (SDITPGUnit[Suffix].Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        WriteUnitInformation(SDITPGUnit[Suffix].HWType, Suffix);
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  SDITPGAudioSignal                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980930                                       */
/*                                                  */
/*  Function:  Set the embedded audio signal in a SDI-TPG.            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGAudioSignal()
  {
  if (SDITPGUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, TPGAudioSignalPar);

          if (ucharval > TPGAudioDual)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
      break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(TPGAudioSignalPar[SDITPGConfig[Suffix].AudioSignal].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != SDITPGConfig[Suffix].AudioSignal)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITPGConfig[Suffix].AudioSignal = ucharval;

            TrxSDITPGAudioSignal(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  SDITPGAudioLevel                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set the embedded audio level in a SDI-TPG.            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGAudioLevel()
  {
  code UC* tablePtr;

  if (SDITPGUnit[Suffix].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, TPGAudioLevelPar);

          tablePtr = FindAudioLevelTable(SDITPGUnit[Suffix].HWType);

          if ((ucharval > TPGAudioLevel20dB) || (tablePtr[ucharval] == NoAudioLevel))
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(TPGAudioLevelPar[SDITPGConfig[Suffix].AudioLevel].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != SDITPGConfig[Suffix].AudioLevel)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITPGConfig[Suffix].AudioLevel = ucharval;

            TrxSDITPGAudioLevel(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  SDITPGAudioGroup                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set the embedded audio group in a SDI-TPG.            */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDITPGAudioGroup() {

  if (SDITPGUnit[Suffix].Present && (SDITPGUnit[Suffix].HWType == PT8633)) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, TPGAudioGroupPar);

          if (ucharval > TPGAudioGroup4)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
      break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(TPGAudioGroupPar[SDITPGConfig[Suffix].AudioGroup].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != SDITPGConfig[Suffix].AudioGroup) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            SDITPGConfig[Suffix].AudioGroup = ucharval;

            TrxSDITPGAudioGroup(Suffix, ucharval);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AESEBUSuffix                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980215                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Get suffix of the AES-EBU Audio Generator.            */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Suffix                                    */
/***************************************************************************/
void AESEBUSuffix() {

  switch (NumSuffix) {
    case 1:
      Suffix = 0;
      break;

    case 2:
      if (AESEBUUnit.HWType == PT8635) {
        Suffix = 1;
        break;
      }

    default:
      SCPIError = HdrSuffixOutOfRange;
      break;
  }
}

/***************************************************************************/
/*  AESEBU                                       RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980211                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the AES-EBU Audio generator command.            */
/* Remarks:    Request only                                */
/*  Returns:     --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AESEBU() {

  if (AESEBUUnit.Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        Udr_CPutNChar(AESEBUSignalPar[AESEBUConfig[Suffix].Signal].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(AESEBULevelPar[AESEBUConfig[Suffix].Level].LongName);
        Udr_PutChar(',');

        Udr_CPutNChar(AESEBUTimingPar[AESEBUConfig[Suffix].Timing].LongName);
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
/*  AESEBUSignal                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980215                          */
/* Revised:    980930                                       */
/*                                                  */
/*  Function:  Handle the AES-EBU Audio Generator audio-signal command.    */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AESEBUSignal() {

  if (AESEBUUnit.Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, AESEBUSignalPar);

          if ((ucharval < AESEBUStereo800Hz) || (ucharval > AESEBUWordClock))
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(AESEBUSignalPar[AESEBUConfig[Suffix].Signal].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != AESEBUConfig[Suffix].Signal) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            AESEBUConfig[Suffix].Signal = ucharval;
            AESEBUUnitDrv(Suffix, ucharval, AESEBUConfig[Suffix].Level,\
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
/*  AESEBULevel                                     RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980215                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Set the audio level in a PT8605/35 AES-EBU Audio generator.  */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          -14 dB is not allowed in PT8635                    */
/*          -15 dB is not allowed in PT8605                    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AESEBULevel() {

  if (AESEBUUnit.Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, AESEBULevelPar);

          if (ucharval > AESEBULevel20dB)
            SCPIError = IllegalParValue;

          if (AESEBUUnit.HWType == PT8635) {
            if (ucharval == AESEBULevel14dB)
              SCPIError = IllegalParValue;
          }
          else {
            if (ucharval == AESEBULevel15dB)
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

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(AESEBULevelPar[AESEBUConfig[Suffix].Level].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != AESEBUConfig[Suffix].Level) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            AESEBUConfig[Suffix].Level = ucharval;

            AESEBUUnitDrv(Suffix, AESEBUConfig[Suffix].Signal, ucharval,\
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
/*  AESEBUTiming                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980215                          */
/* Revised:    980611                                       */
/*                                                  */
/*  Function:  Handle the AES-EBU Audio generator audio-level command.    */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AESEBUTiming() {

  if (AESEBUUnit.Present) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, AESEBUTimingPar);

          if (ucharval > AESEBUNTSCPhase5)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(AESEBUTimingPar[AESEBUConfig[Suffix].Timing].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (ucharval != AESEBUConfig[Suffix].Timing) {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            AESEBUConfig[Suffix].Timing = ucharval;
            AESEBUUnitDrv(Suffix, AESEBUConfig[Suffix].Signal,\
                          AESEBUConfig[Suffix].Level, ucharval);
          }
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  AESEBUVersion                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980215                          */
/* Revised:    990317                                       */
/*                                                  */
/*  Function:  Handle the AES-EBU Audio generator version-command.      */
/*  Remarks:    Request only                                */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void AESEBUVersion() {

  if (AESEBUUnit.Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest)
        WriteUnitInformation(AESEBUUnit.HWType, 0);
      else
        SCPIError = SyntaxErr;
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  TimeClock                                     RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980728                                       */
/*                                                  */
/*  Function:  Handle the Time Clock request command                */
/* Remarks:    Request only                                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeClock() {

  UC tmp;

  if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        Udr_CPutNChar(TimeClockDFormatPar[TimeClockConfig.Format >> 1].LongName);
        Udr_PutChar(',');

        tmp = (TimeClockConfig.Date >> 16) & 0xFF;

        sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
        Udr_PutNChar(TmpBuffer);
        Udr_PutChar(',');

        tmp = (TimeClockConfig.Date >> 8) & 0xFF;
        sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
        Udr_PutNChar(TmpBuffer);
        Udr_PutChar(',');

        tmp = TimeClockConfig.Date & 0xFF;
        sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
        Udr_PutNChar(TmpBuffer);
        Udr_PutChar(',');

        Udr_CPutNChar(TimeClockTFormatPar[TimeClockConfig.Format & 0x01].LongName);
        Udr_PutChar(',');

        tmp = (TimeClockConfig.Time >> 16) & 0xFF;

        sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
        Udr_PutNChar(TmpBuffer);
        Udr_PutChar(',');

        tmp = (TimeClockConfig.Time >> 8) & 0xFF;

        sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
        Udr_PutNChar(TmpBuffer);
        Udr_PutChar(',');

        tmp = TimeClockConfig.Time & 0xFF;

        sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
        Udr_PutNChar(TmpBuffer);
        Udr_PutChar(',');

        Udr_CPutNChar(TimeClockRefPar[TimeClockConfig.Reference].LongName);
        Udr_PutChar(',');

        tmp = 1;

        if ((TimeClockConfig.Offset & 0x10000) == 0x10000)
          TmpBuffer[0] = '-';
        else
          tmp = 0;

        TmpBuffer[tmp] = ((TimeClockConfig.Offset >> 12) & 0x0F) + '0';
        if (TmpBuffer[tmp] != '0')
          tmp++;
        TmpBuffer[tmp++] = ((TimeClockConfig.Offset >> 8) & 0x0F) + '0';
        TmpBuffer[tmp++] = '.';
        TmpBuffer[tmp++] = ((TimeClockConfig.Offset >> 4) & 0x0F) + '0';
        TmpBuffer[tmp] = 0;

        Udr_PutNChar(TmpBuffer);
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
/*  TimeClockDateFormat                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980728                                       */
/*                                                  */
/*  Function:  Handle the Time Clock generator date format command      */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeClockDateFormat() {

  if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, TimeClockDFormatPar);

          if (ucharval > YMDFormat)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(TimeClockDFormatPar[TimeClockConfig.Format >> 1].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          ActivePresetNo = 0;      // Clear possible preset

          if (Ptr == &status_menu[StatusPresetItem])
            Ptr->DispFunct();

          TimeClockConfig.Format &= 0x01;
          TimeClockConfig.Format |= (ucharval << 1);
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  TimeClockDate                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980728                                       */
/*                                                  */
/*  Function:  Handle the Time Clock generator date command            */
/*  Remarks:    ParState 0: Idle                              */
/*          ParState 1: DecNum                             */
/*          ParState 2: DecNum, DecNum                         */
/*          ParState 3: DecNum, DecNum, DecNum                    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeClockDate() {

  UC tmp;

  if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        UlongVal = 0;
        break;

      case P_DecNum:
        if (ParState < 3) {
          ParState++;             // Year/Month/Day

          if (atoi(Cmd) < 100) {
            ucharval = (UC) atoi(Cmd);
            UlongVal += (ucharval / 10) * 16 + ucharval % 10;

            if (ParState < 3)
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

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          tmp = (TimeClockConfig.Date >> 16) & 0xFF;

          sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
          Udr_PutNChar(TmpBuffer);
          Udr_PutChar(',');

          tmp = (TimeClockConfig.Date >> 8) & 0xFF;
          sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
          Udr_PutNChar(TmpBuffer);
          Udr_PutChar(',');

          tmp = TimeClockConfig.Date & 0xFF;
          sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
          Udr_PutNChar(TmpBuffer);

          Udr_PutNewline();
        }
      }
      else {
        if (ParState < 3)
          SCPIError = MissingPar;
        else {
          if (ValidDate(UlongVal)) {
            TimeClockConfig.Date = UlongVal;

                              // Select TCI date register
            ucharval = TimeClockConfig.TCIControl & 0x1F;
            ucharval |= (TCIDateRegister << 5);

                              // Update TCI
            TrxTimeClockData(ucharval, 3, UlongVal);
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
/*  TimeClockTimeFormat                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980728                                       */
/*                                                  */
/*  Function:  Handle the Time Clock generator time format command      */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeClockTimeFormat() {

  if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, TimeClockTFormatPar);

          if (ucharval > Hour24Format)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(TimeClockTFormatPar[TimeClockConfig.Format & 0x01].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          TimeClockConfig.Format &= 0xFE;
          TimeClockConfig.Format |= ucharval;
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  TimeClockTime                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980728                                       */
/*                                                  */
/*  Function:  Handle the Time Clock generator time command            */
/*  Remarks:    ParState 0: Idle                              */
/*          ParState 1: DecNum                             */
/*          ParState 2: DecNum, DecNum                         */
/*          ParState 3: DecNum, DecNum, DecNum                    */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeClockTime() {

  UC tmp;

  if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        UlongVal = 0;
        break;

      case P_DecNum:
        if (ParState < 3) {
          ParState++;             // Hour/Minute/Second

          if (atoi(Cmd) < 60) {
            ucharval = (UC) atoi(Cmd);
            UlongVal += (ucharval / 10) * 16 + ucharval % 10;

            if (ParState < 3)
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

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          tmp = (TimeClockConfig.Time >> 16) & 0xFF;

          sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
          Udr_PutNChar(TmpBuffer);
          Udr_PutChar(',');

          tmp = (TimeClockConfig.Time >> 8) & 0xFF;

          sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
          Udr_PutNChar(TmpBuffer);
          Udr_PutChar(',');

          tmp = TimeClockConfig.Time & 0xFF;

          sprintf(TmpBuffer, "%0d", 10*(tmp >> 4) + (tmp & 0x0F));
          Udr_PutNChar(TmpBuffer);

          Udr_PutNewline();
        }
      }
      else {
        if (ParState < 3)
          SCPIError = MissingPar;
        else {
          if (ValidTime(UlongVal)) {
            TimeClockConfig.Time = UlongVal;

                              // Select TCI time register
            ucharval = TimeClockConfig.TCIControl & 0x1F;
            ucharval |= (TCITimeRegister << 5);

                              // Update TCI
            TrxTimeClockData(ucharval, 3, UlongVal);
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
/*  TimeClockReference                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980728                                       */
/*                                                  */
/*  Function:  Handle the Time Clock generator reference command        */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeClockReference() {

  UC tmp;

  if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, TimeClockRefPar);

          if (ucharval > VideoFieldFreqInput)
            SCPIError = IllegalParValue;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          Udr_CPutNChar(TimeClockRefPar[TimeClockConfig.Reference].LongName);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          TimeClockConfig.Reference = ucharval;

                              // Select TCI time reference register
          tmp = TimeClockConfig.TCIControl & 0x1F;
          tmp |= (TCITimeRefRegister << 5);

                              // Update TCI
          TrxTimeClockData(tmp, 1, ucharval);
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  TimeClockOffset                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980413                          */
/* Revised:    980728                                       */
/*                                                  */
/*  Function:  Handle the Time Clock generator offset command          */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeClockOffset() {

  UC tmp;
  float tmpFloat;

  if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState == 0) {
          ParState++;

          if (strlen(Cmd) < 6)
            HTime = (float) atof(Cmd);
          else
            SCPIError = DataOutOfRange;

          if ((HTime > 10.0) || (HTime < -10.0))
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          tmp = 1;

          if ((TimeClockConfig.Offset & 0x10000) == 0x10000)
            TmpBuffer[0] = '-';
          else
            tmp = 0;

          TmpBuffer[tmp] = ((TimeClockConfig.Offset >> 12) & 0x0F) + '0';
          if (TmpBuffer[tmp] != '0')
            tmp++;
          TmpBuffer[tmp++] = ((TimeClockConfig.Offset >> 8) & 0x0F) + '0';
          TmpBuffer[tmp++] = '.';
          TmpBuffer[tmp++] = ((TimeClockConfig.Offset >> 4) & 0x0F) + '0';
          TmpBuffer[tmp] = 0;

          Udr_PutNChar(TmpBuffer);
          Udr_PutNewline();
        }
      }
      else {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (HTime < 0) {
            TimeClockConfig.Offset = 0x10000;
            HTime = -HTime;
          }
          else
            TimeClockConfig.Offset = 0;

          tmpFloat = 10*HTime;
          tmp = ((UC) tmpFloat) % 10;
          TimeClockConfig.Offset += (tmp << 4);

          tmp = ((UC) HTime) % 10;
          TimeClockConfig.Offset += (tmp << 8);

          tmp = (UC)(HTime) / 10;
          TimeClockConfig.Offset += (tmp << 12);

                              // Select TCI time offset register
          tmp = TimeClockConfig.TCIControl & 0x1F;
          tmp |= (TCIOffsetRegister << 5);

                              // Update TCI
          TrxTimeClockData(tmp, 3, TimeClockConfig.Offset);
        }
      }
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  TimeClockVersion                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970216                          */
/* Revised:    990317                                       */
/*                                                  */
/*  Function:  Return the version of the PT8637 Time Clock Interface.    */
/*  Remarks:    Request only                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void TimeClockVersion() {

  if (TimeClockUnit.Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        WriteUnitInformation(TimeClockUnit.HWType, 0);
      }
      else
        SCPIError = SyntaxErr;
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  BBMultiVersion                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    990317                                       */
/*                                                  */
/*  Function:  Return thne version of the PT8604 Multiple Parallel BB.    */
/*  Remarks:    Request only                                */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void BBMultiVersion() {

  if (AnlBlkUnit[BB2Item].HWType == PT8604) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest)
        WriteUnitInformation(AnlBlkUnit[BB2Item].HWType, 0);
      else
        SCPIError = SyntaxErr;
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  SDIGenlockVersion                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    990317                                       */
/*                                                  */
/*  Function:  Return thne version of the PT8606 SDI Genlock option.      */
/*  Remarks:    Request only                                */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void SDIGenlockVersion() {

  if (SDIGenlockUnit.Present) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest)
        WriteUnitInformation(SDIGenlockUnit.HWType, 0);
      else
        SCPIError = SyntaxErr;
    }
  }
  else
    SCPIError = HardwareMissing;
}

/***************************************************************************/
/*  FactoryPassword                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970206                          */
/* Revised:    971023                                       */
/*                                                  */
/*  Function:  Read a user password, encode it and test if correct      */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: Char                              */
/*          ParState 2: String                            */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactoryPassword() {

  register UC i;

  switch (ParameterType) {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0) {
        ParState++;

        ucharval = FindParameter(Cmd, OffOnPar);

        if (ucharval > ON)
          SCPIError = DataOutOfRange;
      }
      else
        SCPIError = DataTypeErr;
      break;

    case P_String:
      if (ParState == 0) {
        ParState += 2;

        if (strlen(Cmd) > 20)
          SCPIError = DataOutOfRange;
      }
      else
        SCPIError = DataTypeErr;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
  }

  if (CmdExecute && !SCPIError) {
    if (CmdRequest) {
      Udr_CPutNChar(OffOnPar[PasswordState].LongName);
      Udr_PutNewline();
    }
    else {
      if ((ParState == 1) && (ucharval == ON))
        PasswordState = ON;
      else {
        i = 0;
        while (Cmd[i])
          Cmd[i++] -= 10;

        if (!Cstrcmp(Cmd, FactoryPasswordTxt))
          PasswordState = OFF;
      }
    }
  }
}


/***************************************************************************/
/*  FactoryAddress                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    9803131                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0: Idle                               */
/*          ParState 1: DecNum                            */
/*          ParState 2: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactoryAddress() {

  if (PasswordState == OFF) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState == 0) {
          ParState++;

          if (atoi(Cmd) < 256)
            ucharval = (UC) atoi(Cmd);
          else
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState = 2;

          ucharval = FindParameter(Cmd, FactoryAddressPar);

          if (ucharval > 7)
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
    }

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        sprintf(TmpBuffer, "%d\r\n", UnitAddress);
        Udr_PutNChar(TmpBuffer);
      }
      else {
        if (ParState == 1)
          UnitAddress = ucharval;
        else
          UnitAddress = V24CRAMAddressTable[ucharval];
      }
    }
  }
}

/***************************************************************************/
/*  FactoryCommand                                             RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    971030                                                      */
/*                                                                         */
/*  Function:                                                              */
/*  Remarks:ParState 0:  Idle                                              */
/*          ParState 1:  String                               ;,?,$        */
/*          ParState 2:  String, String                       ;            */
/*          ParState 3:  String, DecNum                       ;,?,($)      */
/*          ParState 4:  String, DecNum, String               ;            */
/*          ParState 5:  String, DecNum, DecNum                ;           */
/*          ParState 6:  String, DecNum, DecNum, DecNum        ;           */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void FactoryCommand() {

  char* ascptr;

  if (PasswordState == OFF)
    {
    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_String:
        switch (ParState) {
          case 0:
            ParState++;

            if (strlen(Cmd) < 4)
              strcpy(CmdBuffer, Cmd);
            else
              SCPIError = DataOutOfRange;
            break;

          case 1:
          case 3:
            ParState++;

            if (strlen(Cmd) <= SizeOfTmpBuffer)
              strcpy(TmpBuffer, Cmd);
            else
              SCPIError = DataOutOfRange;
            break;

          default:
            SCPIError = ParNotAllowed;
            break;
        }
        break;

      case P_DecNum:
        switch (ParState) {
          case 1:
            ParState = 3;

            if (strlen(Cmd) < 11)
              UlongVal = atol(Cmd);
            else
              SCPIError = DataOutOfRange;
            break;

          case 3:
            ParState = 5;

            if (strlen(Cmd) < 7)
              IntVal = atoi(Cmd);
            else
              SCPIError = DataOutOfRange;
            break;

          case 5:
            ParState = 6;

            if (strlen(Cmd) < 7)
              IntVal1 = atoi(Cmd);
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

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        Udr_PutNChar(CmdBuffer);
        Udr_PutNewline();
        }
      else
        {
        switch (CmdBuffer[2])
          {
          case '?':
            if ((ParState == 1) || (ParState == 3))
              {
              if (ParState == 3)
                ascptr = RecAsc(UnitAddress, CmdBuffer, (UC) UlongVal, &ucharval);
              else
                {
                ascptr = RecAsc(UnitAddress, CmdBuffer, 0xFF, &ucharval);
                }

              if (ucharval == 0)
                {
                Udr_PutNChar(ascptr);
                Udr_PutNewline();
                }
              }
            else
              SCPIError = ExecErr;
            break;

          case '$':
            if (ParState == 1)
              ucharval = SndSpecial(UnitAddress, CmdBuffer, '$');
            else
              SCPIError = ExecErr;
            break;

          default:

            switch (ParState)
              {
              case 1:
                ucharval = SndVoid(UnitAddress, CmdBuffer);
                break;

              case 2:
                ucharval = SndAsc(UnitAddress, CmdBuffer, 0xFF, TmpBuffer);
                break;

              case 3:
                ucharval = SndLong(UnitAddress, CmdBuffer, UlongVal);
                break;

              case 4:
                ucharval = SndAsc(UnitAddress, CmdBuffer, (UC) UlongVal, TmpBuffer);
                break;

              case 5:
                ucharval = SndInt2(UnitAddress, CmdBuffer, (UI) UlongVal, IntVal);
                break;

              case 6:
                ucharval = SndInt3(UnitAddress, CmdBuffer, (UI) UlongVal, IntVal, IntVal1);
                break;
              }
            break;
          }
        }

      if (ucharval)
        SCPIError = -380 - ucharval;
      }
    }
  }

/***************************************************************************/
/*  FactorySPGUserText                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    970105                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, String                      */
/*          Number of characters in string is limited to 10.        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactorySPGUserText() {

  if (PasswordState == OFF) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState == 0) {
          ParState++;

          if (strlen(Cmd) < 2)
            ucharval = (UC) atol(Cmd);
          else
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      case P_String:
        if (ParState == 1) {
          ParState++;

          if (strlen(Cmd) < 11)
            strcpy(TmpBuffer, Cmd);
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

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        if (ParState == 1) {
          IIC2RdStr(spgram_addr, (UC)(SPGUserText_addr+10*ucharval), TmpBuffer);

          Udr_PutChar('"');
          Udr_PutNChar(TmpBuffer);
          Udr_PutChar('"');
          Udr_PutNewline();
        }
        else
          SCPIError = DataTypeErr;
      }
      else {
        if (ParState == 2)
          IIC2WrStr(spgram_addr, (UC)(SPGUserText_addr+10*ucharval), TmpBuffer);
        else
          SCPIError = DataTypeErr;
      }
    }
  }
}

/***************************************************************************/
/*  FactorySPGStore                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    971023                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactorySPGStore() {

  UL HPHZero_G, HPHZero_M;
  UI GenlPhase_G, GenlPhase_M;

  if (PasswordState == OFF) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {

      HPHZero_G = RecLong(sync_addr, "GA", 0xFF, &ucharval);
      if (ucharval == 0)
        GenlPhase_G = RecLong(sync_addr, "GF", 0xFF, &ucharval);

      if (ucharval == 0)
        HPHZero_M = RecLong(sync_addr, "GU", 0xFF, &ucharval);

      if (ucharval == 0)
        GenlPhase_M = RecLong(sync_addr, "GG", 0xFF, &ucharval);

      if (ucharval == 0) {
        if (CmdRequest) {
          _ultoa(HPHZero_G, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutChar(',');
          _ultoa(GenlPhase_G, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutChar(',');
          _ultoa(HPHZero_M, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutChar(',');
          _ultoa(GenlPhase_M, TmpBuffer);
          Udr_PutNChar(TmpBuffer);
          Udr_PutNewline();
        }
        else {
          IIC2WrLongInt(spgram_addr, HPHZero_G_addr, HPHZero_G, GenlPhase_G);
          IIC2WrLongInt(spgram_addr, HPHZero_M_addr, HPHZero_M, GenlPhase_M);
        }
      }
      else
        SCPIError = DeviceSpecificErr;
    }
  }
}

/***************************************************************************/
/*  FactorySPGRead                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    970917                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactorySPGRead() {

  if (PasswordState == OFF) {

    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError) {
      if (CmdRequest) {
        _ultoa(IIC2RdLong(spgram_addr, HPHZero_G_addr), TmpBuffer);
        Udr_PutNChar(TmpBuffer);

        Udr_PutChar(',');
        _ultoa(IIC2RdInt(spgram_addr, GenlPhase_G_addr), TmpBuffer);
        Udr_PutNChar(TmpBuffer);

        Udr_PutChar(',');
        _ultoa(IIC2RdLong(spgram_addr, HPHZero_M_addr), TmpBuffer);
        Udr_PutNChar(TmpBuffer);

        Udr_PutChar(',');
        _ultoa(IIC2RdInt(spgram_addr, GenlPhase_M_addr), TmpBuffer);
        Udr_PutNChar(TmpBuffer);
        Udr_PutNewline();
      }
    }
  }
}

/***************************************************************************/
/*  FactoryAudioSuffix                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980215                          */
/* Revised:    980608                                       */
/*                                                  */
/*  Function:  Get suffix of the AES-EBU Audio Generator              */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    Suffix                                    */
/***************************************************************************/
void FactoryAudioSuffix() {

  if (NumSuffix > 2)
    SCPIError = HdrSuffixOutOfRange;
  else
    AudioSuffix = NumSuffix;
}

/***************************************************************************/
/*  FactoryAudioUserText                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980607                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, String                      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactoryAudioUserText() {

  if (PasswordState == OFF) {

    if (AESEBUUnit.Present) {
      if (AudioSuffix == 1) {

        switch (ParameterType) {
          case P_NoParameter:
            ParState = 0;
            break;

          case P_DecNum:
            if (ParState == 0) {
              ParState++;

              if (strlen(Cmd) < 3) {
                if ((ucharval = (UC) atol(Cmd)) > 19)
                  SCPIError = DataOutOfRange;
              }
              else
                SCPIError = DataOutOfRange;
            }
            else
              SCPIError = ParNotAllowed;
            break;

          case P_String:
            if (ParState == 1) {
              ParState++;

              if (strlen(Cmd) < 11)
                strcpy(TmpBuffer, Cmd);
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

        if (CmdExecute && !SCPIError) {
          if (CmdRequest) {
            if (ParState == 1) {
              IIC2RdStr(soundram_addr, (UC)(10*ucharval), TmpBuffer);

              Udr_PutChar('"');
              Udr_PutNChar(TmpBuffer);
              Udr_PutChar('"');
              Udr_PutNewline();
            }
            else
              SCPIError = DataTypeErr;
          }
          else {
            if (ParState == 2)
              IIC2WrStr(soundram_addr, (UC)(10*ucharval), TmpBuffer);
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
/*  FactoryAudioRead                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 941124                          */
/* Revised:    980607                                       */
/*                                                  */
/*  Function:  --                                        */
/*  Remarks:    ParState 0:  Idle                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactoryAudioRead() {

  UC tmp, addr1, addr2;

  if (PasswordState == OFF) {

    if (AESEBUUnit.Present) {

      if (CmdExecute && !SCPIError) {
        if (CmdRequest) {

          if (AudioSuffix == 1) {
            addr1 = sound1_addr;
            addr2 = sound2_addr;
          }
          else {
            addr1 = sound3_addr;
            addr2 = sound4_addr;
          }

          tmp = IIC2Read(addr1);

          FeedTheWatchdog;

          _ultoa(tmp, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutChar(',');

          tmp = IIC2Read(addr2);

          FeedTheWatchdog;

          _ultoa(tmp, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutNewline();
        }
      }
    }
    else
      SCPIError = HardwareMissing;
  }
}

/***************************************************************************/
/*  FactoryAudioMode                                 RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980604                          */
/* Revised:    980608                                       */
/*                                                  */
/*  Function:   --                                        */
/*  Remarks:    ParState 0: Idle                              */
/*          ParState 1: Char                              */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactoryAudioMode() {

  UC tmp;

  if (PasswordState == OFF) {

    if (AESEBUUnit.Present && (AESEBUUnit.HWType == PT8635)) {

      if (AudioSuffix == 2) {
        switch (ParameterType) {
          case P_NoParameter:
            ParState = 0;
            break;

          case P_Char:
            if (ParState == 0) {
              ParState++;

              ucharval = FindParameter(Cmd, PT52ModePar);

              if (ucharval > 1)
                SCPIError = DataOutOfRange;
            }
            else
              SCPIError = DataTypeErr;
            break;

          default:
            SCPIError = ParNotAllowed;
            break;
        }

        if (CmdExecute && !SCPIError) {

          if (CmdRequest) {
            if (ParState == 0) {

              tmp = !(IIC2Read(sound4_addr) >> 7);

              Udr_CPutNChar(PT52ModePar[tmp].LongName);
              Udr_PutNewline();
            }
            else
              SCPIError = DataTypeErr;
          }
          else {
            tmp = IIC2Read(sound4_addr);

            if (ucharval == 0)
              tmp |= 0x80;
            else
              tmp &= 0x7F;

            IIC2Write(sound4_addr, tmp);
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
/*  FactoryMainUserText                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970109                          */
/* Revised:    970109                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, String                      */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactoryMainUserText() {

  if (PasswordState == OFF) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState == 0) {
          ParState++;

          if (strlen(Cmd) < 3) {
            if ((ucharval = (UC) atol(Cmd)) > 19)
              SCPIError = DataOutOfRange;
          }
          else
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = ParNotAllowed;
        break;

      case P_String:
        if (ParState == 1) {
          ParState++;

          if (strlen(Cmd) < 11)
            strcpy(TmpBuffer, Cmd);
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

    if (CmdExecute && !SCPIError) {

      if (CmdRequest) {
        if (ParState == 1) {

          IIC2RdStr(mainram1_addr, (UC)(10*ucharval), TmpBuffer);

          Udr_PutChar('"');
          Udr_PutNChar(TmpBuffer);
          Udr_PutChar('"');
          Udr_PutNewline();
        }
        else
          SCPIError = DataTypeErr;
      }
      else {
        if (ParState == 2)
          IIC2WrStr(mainram1_addr, (UC)(10*ucharval), TmpBuffer);
        else
          SCPIError = DataTypeErr;
      }
    }
  }
}

/***************************************************************************/
/*  FactoryMainPSNSuffix                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970717                          */
/* Revised:    970717                                       */
/*                                                  */
/*  Function:                                          */
/* Remarks:                                            */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactoryMainPSNSuffix() {

  if (NumSuffix != 5)
    SCPIError = HdrSuffixOutOfRange;
}

/***************************************************************************/
/*  FactoryMainPSN                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970717                          */
/* Revised:    980427                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, DecNum                      */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactoryMainPSN() {

  if (PasswordState == OFF) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        switch (ParState) {
          case 0:
            ParState++;

            if ((strlen(Cmd) < 4) && (atoi(Cmd) < 256))
              ucharval = (UC) atoi(Cmd);
            else
              SCPIError = DataOutOfRange;
            break;

          case 1:
            ParState++;

            if ((strlen(Cmd) < 4) && (atoi(Cmd) < 256))
              UcharVal1 = (UC) atoi(Cmd);
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

    if (CmdExecute && !SCPIError) {

      if (CmdRequest) {
        if (ParState == 0) {

          IIC2Write(mainram1_addr, VoltN5Min_addr);
          ucharval = IIC2Read(mainram1_addr);

          _ultoa((UL) ucharval, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutChar(',');

          ucharval = IIC2Read(mainram1_addr);

          _ultoa((UL) ucharval, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutNewline();
        }
        else
          SCPIError = DataTypeErr;
      }
      else {
        if (ParState == 2) {
          IIC2Write2(mainram1_addr, VoltN5Min_addr, ucharval);
          IIC2Write2(mainram1_addr, VoltN5Max_addr, UcharVal1);

          VoltN5Min = ucharval;
          VoltN5Max = UcharVal1;
        }
        else
          SCPIError = DataTypeErr;
      }
    }
  }
}

/***************************************************************************/
/*  FactoryMainPSSuffix                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970717                          */
/* Revised:    970717                                       */
/*                                                  */
/*  Function:                                          */
/* Remarks:                                            */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactoryMainPSSuffix() {

  if (NumSuffix != 12)
    SCPIError = HdrSuffixOutOfRange;
}

/***************************************************************************/
/*  FactoryMainPS                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970717                          */
/* Revised:    980427                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, DecNum                      */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactoryMainPS() {

  if (PasswordState == OFF) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState < 2) {
          ParState++;

          IntVal = atoi(Cmd);

          if ((strlen(Cmd) < 4) && (IntVal < 256)) {
            if (ParState == 1)
              ucharval = (UC) IntVal;
            else
              UcharVal1 = (UC) IntVal;
          }
          else
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = ParNotAllowed;
        break;
    }

    if (CmdExecute && !SCPIError) {

      if (CmdRequest) {
        if (ParState == 0) {

          IIC2Write(mainram1_addr, Volt12Min_addr);
          ucharval = IIC2Read(mainram1_addr);

          _ultoa((UL) ucharval, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutChar(',');

          ucharval = IIC2Read(mainram1_addr);

          _ultoa((UL) ucharval, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutNewline();
        }
        else
          SCPIError = DataTypeErr;
      }
      else {
        if (ParState == 2) {
          IIC2Write2(mainram1_addr, Volt12Min_addr, ucharval);
          IIC2Write2(mainram1_addr, Volt12Max_addr, UcharVal1);

          Volt12Min = ucharval;
          Volt12Max = UcharVal1;
        }
        else
          SCPIError = DataTypeErr;
      }
    }
  }
}

/***************************************************************************/
/*  FactoryMainPSCalib                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970820                          */
/* Revised:    970827                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0: Idle                              */
/*          ParState 1: Char                              */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactoryMainPSCalib() {

  if (PasswordState == OFF) {

    switch (ParameterType) {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0) {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = DataOutOfRange;
        }
        else
          SCPIError = DataTypeErr;
        break;

      default:
        SCPIError = ParNotAllowed;
        break;
    }

    if (CmdExecute && !SCPIError) {

      if (CmdRequest) {
        if (ParState == 0) {

          IIC2Write(mainram1_addr, VoltCalib_addr);

          if ((ucharval = IIC2Read(mainram1_addr)) != ON)
            ucharval = OFF;

          Udr_CPutNChar(OffOnPar[ucharval].LongName);
          Udr_PutNewline();
        }
        else
          SCPIError = DataTypeErr;
      }
      else {
        IIC2Write2(mainram1_addr, VoltCalib_addr, ucharval);

        VoltCalib = ucharval;
      }
    }
  }
}

/***************************************************************************/
/*  FactorySDIGenlockUserText                           RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970109                          */
/* Revised:    970109                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, String                      */
/*  Returns:                                            */
/*  Updates:  
/*  UPDATE 07/01/08 : copyed all instructions for use with GPS genlock module */
/*                    doing the same writing/readings, but from d2ram_addr      */
/*            thus emulating SDI module and making setup of GPS module possible */
/*            through available setup software
/***************************************************************************/
void FactorySDIGenlockUserText() {
  
  if (PasswordState == OFF) {
    
    if (SDIGenlockUnit.Present) {
    
      switch (ParameterType) {
        case P_NoParameter:
          ParState = 0;
          break;

        case P_DecNum:
          if (ParState == 0) {
            ParState++;

            if (strlen(Cmd) < 3) {
              if ((ucharval = (UC) atol(Cmd)) > 19)
                SCPIError = DataOutOfRange;
            }
            else
              SCPIError = DataOutOfRange;
          }
          else
            SCPIError = ParNotAllowed;
          break;

        case P_String:
          if (ParState == 1) {
            ParState++;

            if (strlen(Cmd) < 11)
              strcpy(TmpBuffer, Cmd);
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

      if (CmdExecute && !SCPIError) {    
        if (CmdRequest) {
          if (ParState == 1) {

            IIC1RdStr(d1ram_addr, (UC)(10*ucharval), TmpBuffer);

            Udr_PutChar('"');
            Udr_PutNChar(TmpBuffer);
            Udr_PutChar('"');
            Udr_PutNewline();
          }
          else
            SCPIError = DataTypeErr;
        }
        else {
          if (ParState == 2)
          {
            IIC1WrStr(d1ram_addr, (UC)(10*ucharval), TmpBuffer); 
          }
          else
            SCPIError = DataTypeErr;
        }
      }
    }
    
    // GPS genlock setup part
    else if (GPSGenlockUnit.Present) {
    
      switch (ParameterType) {
        case P_NoParameter:
          ParState = 0;
          break;

        case P_DecNum:
          if (ParState == 0) {
            ParState++;

            if (strlen(Cmd) < 3) {
              if ((ucharval = (UC) atol(Cmd)) > 19)
                SCPIError = DataOutOfRange;
            }
            else
              SCPIError = DataOutOfRange;
          }
          else
            SCPIError = ParNotAllowed;
          break;

        case P_String:
          if (ParState == 1) {
            ParState++;

            if (strlen(Cmd) < 11)
              strcpy(TmpBuffer, Cmd);
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

      if (CmdExecute && !SCPIError) {
        if (CmdRequest) {
          if (ParState == 1) {
      //read string no. ucharval  //200-203 = strings for KU, softwareversion and calibrationdate
            IIC1RdStr(d2ram_addr, (UC)(200+ucharval), TmpBuffer);

            Udr_PutChar('"');
            Udr_PutNChar(TmpBuffer);
            Udr_PutChar('"');
            Udr_PutNewline();
          }
          else
            SCPIError = DataTypeErr;
        }
        else {
          if (ParState == 2)
          {
          //write string no. ucharval
          //write subaddress+tmpbuffer  60-63 = write sub adress for above strings ^^^
            sprintf(GPSBuffer, "%c%s", (UC)(60+ucharval), TmpBuffer);
            //write string (11 bytes)
            IIC1WriteN(d2ram_addr, 11, GPSBuffer);
          }
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
/*  FactoryTimeClockUserText                           RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970109                          */
/* Revised:    980730                                       */
/*                                                  */
/*  Function:  Handle user text for the Time Clock Interface          */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, String                      */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactoryTimeClockUserText() {

  if (PasswordState == OFF) {

    if (TimeClockUnit.Present) {

      switch (ParameterType) {
        case P_NoParameter:
          ParState = 0;
          break;

        case P_DecNum:
          if (ParState == 0) {
            ParState++;

            if (strlen(Cmd) < 3) {
              if ((ucharval = (UC) atol(Cmd)) > 19)
                SCPIError = DataOutOfRange;
            }
            else
              SCPIError = DataOutOfRange;
          }
          else
            SCPIError = ParNotAllowed;
          break;

        case P_String:
          if (ParState == 1) {
            ParState++;

            if (strlen(Cmd) < 11)
              strcpy(TmpBuffer, Cmd);
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

      if (CmdExecute && !SCPIError) {

        if (CmdRequest) {
          if (ParState == 1) {

            IIC1RdStr(timeram_addr, (UC)(10*ucharval), TmpBuffer);

            Udr_PutChar('"');
            Udr_PutNChar(TmpBuffer);
            Udr_PutChar('"');
            Udr_PutNewline();
          }
          else
            SCPIError = DataTypeErr;
        }
        else {
          if (ParState == 2)
            IIC1WrStr(timeram_addr, (UC)(10*ucharval), TmpBuffer);
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
/*  FactoryTimeClockStatus                             RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980730                          */
/* Revised:    980730                                       */
/*                                                  */
/*  Function:  Handle status for the Time Clock Interface            */
/*  Remarks:    Query only                                  */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void FactoryTimeClockStatus() {

  UC tmp;

  if (PasswordState == OFF) {

    if (TimeClockUnit.Present && (TimeClockUnit.HWType == PT8637)) {

      if (ParameterType != P_NoParameter)
        SCPIError = ParNotAllowed;

      if (CmdExecute && !SCPIError) {
        if (!CmdRequest)
          SCPIError = SyntaxErr;
        else {
                            // Get TCI error information.
          TimeClockConfig.TCIControl &= 0x1F;
          TimeClockConfig.TCIControl |= (TCIErrorRegister << 5);
          IIC1Write(time1_addr, TimeClockConfig.TCIControl);

          FeedTheWatchdog;

          IIC1ReadN(time1_addr, 2, (UC*) TmpBuffer);

          tmp = TmpBuffer[1];

          _ultoa((UL) tmp, TmpBuffer);
          Udr_PutNChar(TmpBuffer);

          Udr_PutNewline();
        }
      }
    }
    else
      SCPIError = HardwareMissing;
  }
}

/***************************************************************************/
/*  FactoryMultiBBUserText                             RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970109                          */
/* Revised:    980427                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:    ParState 0:  Idle                              */
/*          ParState 1:  DecNum                            */
/*          ParState 2:  DecNum, String                      */
/*  Returns:                                            */
/*  Updates:                                            */
/***************************************************************************/
void FactoryMultiBBUserText() {

  if (PasswordState == OFF) {

    if (AnlBlkUnit[BB2Item].HWType == PT8604) {

      switch (ParameterType) {
        case P_NoParameter:
          ParState = 0;
          break;

        case P_DecNum:
          if (ParState == 0) {
            ParState++;

            if (strlen(Cmd) < 3) {
              if ((ucharval = (UC) atol(Cmd)) > 19)
                SCPIError = DataOutOfRange;
            }
            else
              SCPIError = DataOutOfRange;
          }
          else
            SCPIError = ParNotAllowed;
          break;

        case P_String:
          if (ParState == 1) {
            ParState++;

            if (strlen(Cmd) < 11)
              strcpy(TmpBuffer, Cmd);
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

      if (CmdExecute && !SCPIError) {

        if (CmdRequest) {
          if (ParState == 1) {

            IIC1RdStr(multibb_addr, (UC)(10*ucharval), TmpBuffer);

            Udr_PutChar('"');
            Udr_PutNChar(TmpBuffer);
            Udr_PutChar('"');
            Udr_PutNewline();
          }
          else
            SCPIError = DataTypeErr;
        }
        else {
          if (ParState == 2)
            IIC1WrStr(multibb_addr, (UC)(10*ucharval), TmpBuffer);
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
/*  FactoryADCRead                                   RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970122                          */
/* Revised:    970127                                       */
/*                                                  */
/*  Function:  Write content of all A/D converters to RS232            */
/*  Remarks:    ParState 0:  Idle                              */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void FactoryADCRead() {

  if (PasswordState == OFF)

    if (CmdExecute && CmdRequest && !SCPIError) {

      _ultoa(TempMeas, TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      Udr_PutChar(',');

      _ultoa(VoltN5Meas, TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      Udr_PutChar(',');

      _ultoa(Volt5Meas, TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      Udr_PutChar(',');

      _ultoa(Volt12Meas, TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      Udr_PutNewline();
    }
}

/***************************************************************************/
/*  FactoryLDetectorRead                               RS232CMD.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970122                          */
/* Revised:    970820                                       */
/*                                                  */
/*  Function:  Write content of level detectors to RS232              */
/*  Remarks:    ParState 0:  Idle                              */
/*  Returns:    -                                        */
/*  Updates:    -                                        */
/***************************************************************************/
void FactoryLDetectorRead() {

  if (PasswordState == OFF)

    if (CmdExecute && CmdRequest && !SCPIError) {

      _ultoa((LevelStatus & 0x00FF), TmpBuffer);
      Udr_PutNChar(TmpBuffer);

      Udr_PutChar(',');

      _ultoa((LevelStatus >> 8), TmpBuffer);
      Udr_PutNChar(TmpBuffer);
      Udr_PutNewline();
    }
}

/***************************************************************************/
/*  FindDelayParameters                                        RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    970105                                                      */
/*                                                                         */
/*  Function:  Get Field, Line and HTime from RS232 command                */
/*  Remarks:   ParState 0:  Idle                                           */
/*          ParState 1:  DecNum                                            */
/*          ParState 2:  DecNum, DecNum                                    */
/*          ParState 3:  DecNum, DecNum, DecNum                            */
/*  Returns:    -                                                          */
/*  Updates:    Field, Line, HTime                                         */
/***************************************************************************/
void FindDelayParameters() {

  switch (ParameterType) {

    case P_NoParameter:
      ParState = 0;
      break;

    case P_DecNum:
      switch (ParState) {
        case 0:
          ParState++;

          if (strlen(Cmd) < 3)
            Field = (char) atoi(Cmd);
          else
            SCPIError = DataOutOfRange;
          break;

        case 1:
          ParState++;
          
          if (strlen(Cmd) < 5)         /* orig: 5 JK (probably including sign) */
            Line = (int) atoi(Cmd);    /* must be 6 to tackle HD Line          */
          else
            SCPIError = DataOutOfRange;
          break;

        case 2:
          ParState++;
          
          if (strlen(Cmd) < 10)
            HTime = (float) atof(Cmd);
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
/*  FindParameter                                              RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    980518                                                      */
/*                                                                         */
/*  Function:  Find the character defined parameter                        */
/*  Remarks:    --                                                         */
/*  Returns:    The array number containing the parameter or max. array+1  */
/*  Updates:    --                                                         */
/*
typedef struct PAR_STRUCT
  {
  code char* LongName;
  code char* ShortName;

  code struct PAR_STRUCT* SameLevel;
  };
*/
/***************************************************************************/
UC FindParameter(char* par, code struct PAR_STRUCT* ParPtr)
  {
  UC tmp = 0;

  while (ParPtr)
    {
    FeedTheWatchdog;

    if ((!Cstrcmp(par, ParPtr->LongName)) || (!Cstrcmp(par, ParPtr->ShortName)))
      break;

    ParPtr = ParPtr->SameLevel;

    if (tmp < 255)
      tmp++;
    else
      break;
    }
  return(tmp);
  }

/***************************************************************************/
/*  WriteUnitInformation                                       RS232CMD.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 990317                                    */
/* Revised:    990317                                                      */
/*                                                                         */
/*  Function:  Write the companyname, type number, KU number and software  */
/*             version from a  V24 operated unit to RS232                  */
/*  Remarks:    -                                                          */
/*  Returns:    -                                                          */
/*  Updates:    -                                                          */
/***************************************************************************/
UC WriteUnitInformation(UC HWtype, UC suffix)
  {
  char* ascptr;

  UC HWversion, SWversion, HWinformation;
  UL KUnumber;

  switch (HWtype)
    {
    case PT8601:
    case PT8631:
      ascptr = RecStr(AnlTPGUnit[Suffix].Address, "AR", CompanyName_addr/10, &ucharval);

      KUnumber      = AnlTPGUnit[Suffix].KUNumber;
      HWversion     = AnlTPGUnit[Suffix].HWVersion;
      SWversion     = AnlTPGUnit[Suffix].SWVersion;
      HWinformation = AnlTPGUnit[Suffix].HWInformation;
      break;

    case PT8602:
    case PT8603:
    case PT8632:
    case PT8633:
      ascptr = RecStr(SDITPGUnit[Suffix].Address, "SR", CompanyName_addr/10, &ucharval);

      KUnumber      = SDITPGUnit[Suffix].KUNumber;
      HWversion     = SDITPGUnit[Suffix].HWVersion;
      SWversion     = SDITPGUnit[Suffix].SWVersion;
      HWinformation = SDITPGUnit[Suffix].HWInformation;
      break;

    case PT8605:
    case PT8635:
      IIC2RdStr(soundram_addr, CompanyName_addr, TmpBuffer);
      ascptr        = TmpBuffer;

      KUnumber      = AESEBUUnit.KUNumber;
      HWversion     = AESEBUUnit.HWVersion;
      SWversion     = AESEBUUnit.SWVersion;
      HWinformation = AESEBUUnit.HWInformation;
      break;

    case PT8606:
      IIC1RdStr(d1ram_addr, CompanyName_addr, TmpBuffer);
      ascptr = TmpBuffer;

      KUnumber      = SDIGenlockUnit.KUNumber;
      HWversion     = SDIGenlockUnit.HWVersion;
      SWversion     = SDIGenlockUnit.SWVersion;
      HWinformation = SDIGenlockUnit.HWInformation;
      break;

    case PT8607:
    case PT8637:
      IIC1RdStr(timeram_addr, CompanyName_addr, TmpBuffer);
      ascptr        = TmpBuffer;

      KUnumber      = TimeClockUnit.KUNumber;
      HWversion     = TimeClockUnit.HWVersion;
      SWversion     = TimeClockUnit.SWVersion;
      HWinformation = TimeClockUnit.HWInformation;
      break;

    case PT8604:
    case PT8608:
      if (HWtype == PT8604)
        {
        IIC1RdStr(multibb_addr, CompanyName_addr, TmpBuffer);
        ascptr = TmpBuffer;
        }
      else
        ascptr = RecStr(AnlBlkUnit[Suffix].Address, "HR", CompanyName_addr/10, &ucharval);

      KUnumber      = AnlBlkUnit[Suffix].KUNumber;
      HWversion     = AnlBlkUnit[Suffix].HWVersion;
      SWversion     = AnlBlkUnit[Suffix].SWVersion;
      HWinformation = AnlBlkUnit[Suffix].HWInformation;
      break;

    case PT8609:
    case PT8639:
      ascptr = RecStr(SDITSGUnit[Suffix].Address, "HR", CompanyName_addr/10, &ucharval);

      KUnumber      = SDITSGUnit[Suffix].KUNumber;
      HWversion     = SDITSGUnit[Suffix].HWVersion;
      SWversion     = SDITSGUnit[Suffix].SWVersion;
      HWinformation = SDITSGUnit[Suffix].HWInformation;
      break;

    case PT8611: /* TLG, 8611                   TR = ReadUserTextCommand   error */
      ascptr = RecStr(TLGUnit[Suffix].Address, "TR", CompanyName_addr/10, &ucharval);
      /* the above should read Company Name */
      /* #define ProductKUNo_addr     (UC) 20  // KU123456 */

      KUnumber      = TLGUnit[Suffix].KUNumber;
      HWversion     = TLGUnit[Suffix].HWVersion;
      HWversion     = 0;
      SWversion     = TLGUnit[Suffix].SWVersion;
      HWinformation = TLGUnit[Suffix].HWInformation;
      break;

    case PT8612: /* HD, 8612                    UC = ReadUserTextCommand   error */
      ascptr = RecStr(HDTPGUnit[Suffix].Address, "UC", CompanyName_addr/10, &ucharval);
      /* the above should read Company Name */
      /* #define ProductKUNo_addr     (UC) 20  // KU123456 */

      KUnumber      = HDTPGUnit[Suffix].KUNumber;
      HWversion     = HDTPGUnit[Suffix].HWVersion;
      HWversion     = 0;
      SWversion     = HDTPGUnit[Suffix].SWVersion;
      HWinformation = HDTPGUnit[Suffix].HWInformation;
      break;

    case PT8613: /* TLG, 8613                   VC = ReadUserTextCommand   error */
      ascptr = RecStr(DLTPGUnit[Suffix].Address, "VC", CompanyName_addr/10, &ucharval);
      /* the above should read Company Name */
      /* #define ProductKUNo_addr     (UC) 20  // KU123456 */

      KUnumber      = DLTPGUnit[Suffix].KUNumber;
      HWversion     = DLTPGUnit[Suffix].HWVersion;
      HWversion     = 0;
      SWversion     = DLTPGUnit[Suffix].SWVersion;
      HWinformation = DLTPGUnit[Suffix].HWInformation;
      break;
      
    case PT8616: // GPS Genlock board
    	ascptr 			= "DKT";
    	KUnumber 		= GPSGenlockUnit.KUNumber;
    	HWversion		= GPSGenlockUnit.HWVersion;
    	SWversion		= GPSGenlockUnit.SWVersion;
    	HWinformation	= GPSGenlockUnit.HWInformation;
    	break;
    
    default:
      SCPIError = HardwareMissing;
      return(0);
      break;
    }

  if (ucharval == 0)      /* if no error.... */
    Udr_PutNChar(ascptr);

  sprintf(TmpBuffer, ",PT86%02d", HWtype);
  Udr_PutNChar(TmpBuffer);

  if (HWversion)
    {
    sprintf(TmpBuffer, "_9%02d", HWversion);
    Udr_PutNChar(TmpBuffer);
    }
  else
    {
    if ((HWtype == PT8632) && (HWinformation == 1))
      Udr_PutNChar("_10");
    }

  sprintf(TmpBuffer, ",KU%06.0f", (float) KUnumber);
  Udr_PutNChar(TmpBuffer);

  if (SWversion)
    {
    sprintf(TmpBuffer, ",%04.1f", (float) SWversion/10.0);
    Udr_PutNChar(TmpBuffer);
    }
  else
    Udr_PutNChar(",0");

  Udr_PutNewline();

  return(0);
  }

/******************************************************************
 * NAME       void DL()
 * PURPOSE    To handle DL command
 * 
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None
 *
 * NOTES:     This function tackles DL querry of the form:
 *            :OUTP:DLx?;, i.e. it prints system and delay for 
 *            generator x.
 * 
 ******************************************************************/
void DL()
  {
  UC    tmp;

  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        /* Pattern */
        tmp = DLTPGConfig[Suffix].Pattern;
        Udr_CPutNChar(HDPatternPar[tmp].LongName);
        Udr_PutChar(',');
        /* Text ON/OFF */
        if (DLTPGConfig[Suffix].TextEnable)
          tmp = 1;
        else
          tmp = 0;
        Udr_CPutNChar(OffOnPar[tmp].LongName);
        Udr_PutChar(',');
        /* System */
        tmp = DLTPGConfig[Suffix].System;
        Udr_CPutNChar(DLFormatPar[tmp].LongName);
        Udr_PutChar(',');
        /* Audio Signal */
        Udr_CPutNChar(HDAudioSignalPar[DLTPGConfig[Suffix].AudioSignal].LongName);
        Udr_PutChar(',');
        /* Audio Level */
        Udr_CPutNChar(HDAudioLevelPar[DLTPGConfig[Suffix].AudioLevel].LongName);
        Udr_PutChar(',');
        /* Timing */
        TLGSamplesToFLT(DLTPGConfig[Suffix].System, DLTPGConfig[Suffix].Delay, 
                          &sign, &Field, &Line, &HTime, dltpg_fmt_table);
        PrintTimingFLTTLG(sign, Field, Line, HTime);

        Udr_PutNewline();
        }
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
  }

void DLSuffix()
  {
  switch (NumSuffix)
    {
    case  1:
    case  2:
    case  3:
    case  4:
    case  5:
    case  6:
      Suffix = NumSuffix - 1;
      break;
    default:
      SCPIError = HdrSuffixOutOfRange;
      break;
    }
  }


void DLFormat()
  {
  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;
      case P_Char:
        if (ParState == 0)
          {
          ParState++;
          ucharval = FindParameter(Cmd, DLFormatPar);

          if (ucharval == MAX_DL_SYSTEMS)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;
      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(DLFormatPar[DLTPGConfig[Suffix].System].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != DLTPGConfig[Suffix].System)
            {
            ActivePresetNo = 0;      // Clear possible preset
            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();
            DLTPGConfig[Suffix].System = ucharval;
            //DLTPGConfig[Suffix].Delay  = 0;
            DLTPGConfig[Suffix].Delay = adjust_hd_samples(DLTPGConfig[Suffix].Delay, ucharval, dltpg_fmt_table);

            TrxDLTPGSystem(Suffix, DLTPGConfig[Suffix].System,
                        DLTPGConfig[Suffix].Delay,
                        GenlockConfig[ActiveGenlockNo].Delay);

            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

  
  
/***************************************************************************/
/*  DLAInterCommon                                             RS232CMD.C  */
/*                                                                         */
/* Author:   JK                                                            */
/* Revised:  05.01.2007                                                    */
/*                                                                         */
/*  Function:  Handle the DL systeminterface                               */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*              This function checks if 
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void DLAInterCommon()
  {
  UC tmp, system;

  system = DLTPGConfig[Suffix].System;

  if (!(DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present))
    {
    SCPIError = HardwareMissing;
    return;
    }

  switch (ParameterType)
    {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0)
        {
        ParState++;

        ucharval = FindParameter(Cmd, DLInterPar);

        if (ucharval == 6)
          SCPIError = IllegalParValue;

        if (DLValidateInter(system))
          {
          SCPIError = ExecErr;
          }
        else
          {
          }
        }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
    }

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParState)
        SCPIError = ParNotAllowed;
      else
        {
        if (DLValidateInter(system))
          {
          SCPIError = ExecErr;
          }
        else
          {
          /* Get the current interface of selected System */
          tmp = DLTPGConfig[Suffix].Interface[system];
  
          Udr_CPutNChar(DLInterPar[tmp].LongName);
          Udr_PutNewline();
          }
        }
      }
    else
      {
      if (ParState == 0)
        SCPIError = MissingPar;
      else
        {
        if (DLTPGConfig[Suffix].Interface[system] != ucharval)
          {
          ActivePresetNo = 0;

          if (Ptr == &status_menu[StatusPresetItem])
            Ptr->DispFunct();

          DLTPGConfig[Suffix].Interface[system] = ucharval;

          TrxDLTPGInterface(Suffix, ucharval);
          }
        }
      }
    }
  }

/******************************************************************
 * NAME       UC DLValidateInter()
 * PURPOSE    Given system and interface check that inteface is 
 *            valic.
 * 
 * ARGUMENTS  UC sys    - system
 *
 * RETURNS    0 if OK
 *            1 if not
 *
 * EXTERNS    None
 *
 * NOTES:     Only systems 1...13 have interface
 * 
 ******************************************************************/
UC DLValidateInter(UC sys)
  {
  if ((sys > 0) && (sys < 14))
    return (0);
  else
    return (1);
  }


void DLDelay()
  {
  if (DLTPGUnit[0].Present || DLTPGUnit[4].Present || DLTPGUnit[8].Present)
    {
    FindDelayParameters();

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          TLGSamplesToFLT(DLTPGConfig[Suffix].System, DLTPGConfig[Suffix].Delay, 
                          &sign, &Field, &Line, &HTime, dltpg_fmt_table);
  
          PrintTimingFLTTLG(sign, Field, Line, HTime);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState < 3)
          SCPIError = MissingPar;
        else
          {
          UlongVal = TLGFLTToSamples(DLTPGConfig[Suffix].System, Field, Line, HTime,
                                     dltpg_fmt_table);
          if (UlongVal != -1)
            {
            if (UlongVal != DLTPGConfig[Suffix].Delay)
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              DLTPGConfig[Suffix].Delay = UlongVal;  /* Save new Delay... */

              TrxDLTPGSystem(Suffix, DLTPGConfig[Suffix].System,
                          DLTPGConfig[Suffix].Delay,
                          GenlockConfig[ActiveGenlockNo].Delay);
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

void DLVersion()
  {
  if (DLTPGUnit[Suffix].Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        WriteUnitInformation(DLTPGUnit[Suffix].HWType, Suffix);
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
  }

void DLPattern()
  {
  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, HDPatternPar);

          if (ucharval > HDBlack)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDPatternPar[DLTPGConfig[Suffix].Pattern].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != DLTPGConfig[Suffix].Pattern)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            DLTPGConfig[Suffix].Pattern = ucharval;

            TrxDLTPGPattern(Suffix, ucharval);
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
/*  DLAttrCommon                                               RS232CMD.C  */
/*                                                                         */
/* Author:   JK                                                            */
/* Revised:  05.01.2007                                                    */
/*                                                                         */
/*  Function:  Handle the DL pattern attributes (modificatons)             */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*            This function checks if which pattern it is, then calls      */
/*            appropriate function for tackling Colorbar and Combination   */
/*            attributes, or function for tackling Window and White attrib.*/
/*             The syntax is:                                              */
/*              :outp:dl5:patt:mod?;                                       */
/*              :outp:dl5:patt:mod A105;                                   */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void DLAttrCommon()
  {
  UC tmp, pattern;

  pattern = DLTPGConfig[Suffix].Pattern;

  if (!(DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present))
    {
    SCPIError = HardwareMissing;
    return;
    }

  switch (pattern)
    {
    case HDColorbar:
    case HDCombination:
      do_dl_common(HDItensPar, 3);
      break;

    case HDWhite:
    case HDWindow:
      do_dl_common(HDLevelPar, 23);
      break;

    default:
      SCPIError = ExecErr;
      return;
      break;
    }
  }

void do_dl_common(code struct PAR_STRUCT *pptr, UC limit)
  {
  UC tmp, pattern;

  pattern = DLTPGConfig[Suffix].Pattern;

  switch (ParameterType)
    {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0)
        {
        ParState++;

        ucharval = FindParameter(Cmd, pptr);

        if (ucharval == limit)
          SCPIError = IllegalParValue;
        }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
    }

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParState)
        SCPIError = ParNotAllowed;
      else
        {
        /* Get the current attribute of a selected pattern */
        tmp = DLTPGConfig[Suffix].Attrib[pattern];

        Udr_CPutNChar(pptr[tmp].LongName);
        Udr_PutNewline();
        }
      }
    else
      {
      if (ParState == 0)
        SCPIError = MissingPar;
      else
        {
        if (DLTPGConfig[Suffix].Attrib[pattern] != ucharval)
          {
          ActivePresetNo = 0;

          if (Ptr == &status_menu[StatusPresetItem])
            Ptr->DispFunct();

          DLTPGConfig[Suffix].Attrib[pattern] = ucharval;

          TrxDLTPGAttributes(Suffix, ucharval);
          }
        }
      }
    }
  }

/***************************************************************************/
/*  DLAudioSignal                                              RS232CMD.C  */
/*                                                                         */
/* Author:   JK                                                            */
/* Revised:  05.01.2007                                                    */
/*                                                                         */
/*  Function:  Handle the DL embedded audio signal                         */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*             The syntax is:                                              */
/*              :outp:dl5:emb:sign?;                                       */
/*              :outp:dl5:emb:sign sil;                                    */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void DLAudioSignal()
  {
  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, HDAudioSignalPar);

          if (ucharval > HDAudioOff)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
      break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDAudioSignalPar[DLTPGConfig[Suffix].AudioSignal].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != DLTPGConfig[Suffix].AudioSignal)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            DLTPGConfig[Suffix].AudioSignal = ucharval;

            TrxDLTPGAudioSignal(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  DLAudioLevel                                               RS232CMD.C  */
/*                                                                         */
/* Author:   JK                                                            */
/* Revised:  05.01.2007                                                    */
/*                                                                         */
/*  Function:  Handle the DL embedded audio level                          */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*             The syntax is:                                              */
/*              :outp:dl5:emb:level?;                                      */
/*              :outp:dl5:emb:lev -30;                                     */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void DLAudioLevel()
  {
  code UC* tablePtr;
  int  level;
  UC   txtbuff[10];

  if (DLTPGUnit[0].Present || DLTPGUnit[4].Present || DLTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      //case P_Char:
      case P_DecNum:
        if (ParState == 0)
          {
          ParState++;

          //ucharval = FindParameter(Cmd, DLAudioLevelPar); 

          IntVal = atoi(Cmd);
          IntVal = abs(IntVal);

          if ((IntVal < 0))
            SCPIError = IllegalParValue;
          if ((IntVal > 60))
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;  // 
        else
          {
          IntVal = DLTPGConfig[Suffix].AudioLevel;

          if (IntVal)
            sprintf(txtbuff, "-%d", IntVal);
          else
            sprintf(txtbuff, " %d", IntVal);

          Udr_PutNChar(txtbuff);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (IntVal != DLTPGConfig[Suffix].AudioLevel)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            DLTPGConfig[Suffix].AudioLevel = IntVal;

            TrxDLTPGAudioLevel(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }


void DLAudioClick()
  {
  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState == 0)
          {
          ParState++;

          IntVal = atoi(Cmd);

          if ((IntVal < ClickLow) || (IntVal > ClickHigh))
            SCPIError = DataOutOfRange;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          sprintf(TmpBuffer, "%d\r\n", DLTPGConfig[Suffix].AudioClick);
          Udr_PutNChar(TmpBuffer);
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (IntVal != DLTPGConfig[Suffix].AudioClick)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            DLTPGConfig[Suffix].AudioClick = IntVal;

            TrxDLTPGAudioClick(Suffix, IntVal);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  DLTextStringSuffix                                         RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Get suffix of the DL text string (maybe 1,2 or 3, i.e       */
/*             text line nr.                                               */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    StringSuffix                                               */
/***************************************************************************/
void DLTextStringSuffix()
  {
  if (NumSuffix > 3)
    SCPIError = SuffixErr;
  else
    StringSuffix = NumSuffix - 1;
  }

/***************************************************************************/
/*  DLTextString                                               RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the DL test string command.                          */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*              ParState 2: DecNum, Char                                   */
/*              ParState 3: DecNum, String                                 */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void DLTextString()
  {
  register int i, j;
  UC tmp, noOfChar;

  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:         // user entered ON/OFF
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = DataTypeErr;
        break;

      case P_String:         // user entered some string
        if (ParState == 0)
          {
          ParState += 2;

          noOfChar = PT863xMaxNoChar;

          for (j = i = 0; i < strlen(Cmd); i++, j++)
            {
            tmp = Cmd[i];  

            if ((tmp < 0x20) || (tmp > 0x7E))
              {
              SCPIError = DataOutOfRange;
              break;
              }
            else
              Cmd[j] = tmp;
            }
          Cmd[j] = 0;

          if (strlen(Cmd) > noOfChar)
            SCPIError = DataOutOfRange;
          }
        else
          SCPIError = DataTypeErr;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      } //switch (ParameterType)

    if (CmdExecute && !SCPIError)
      {
      tmp = StringSuffix;

      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          i = (DLTPGConfig[Suffix].TextEnable & (1 << tmp)) > 0;

          Udr_CPutNChar(OffOnPar[i].LongName);
          Udr_PutNChar(",\"");
          Udr_PutNChar(DLTPGConfig[Suffix].Text[tmp]);
          Udr_PutChar('"');
          Udr_PutNewline();
          }
        }
      else
        {
        switch (ParState)
          {
          case 0:
            SCPIError = MissingPar;
            break;

          case 1:               // user entered ON/0FF ?
            if ((DLTPGConfig[Suffix].TextEnable & (1 << tmp)) ^ (ucharval << tmp))
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              DLTPGConfig[Suffix].TextEnable &= ~(1 << tmp);
              DLTPGConfig[Suffix].TextEnable |= (ucharval << tmp);

              TrxDLTPGText(Suffix, &DLTPGConfig[Suffix]);
              }
            break;

          case 2:               // user entered some text ?
            if (strcmp(SDITPGConfig[Suffix].Text[tmp], Cmd))
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              strcpy(DLTPGConfig[Suffix].Text[tmp], Cmd);
              TrxDLTPGText(Suffix, &DLTPGConfig[Suffix]);
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
/*  DLTextMovement                                             RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the DL Text Movement command.                        */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/

void DLTextMovement()
  {
  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, TextMovePar);

          if (ucharval > 3)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      } // switch (ParameterType)

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(TextMovePar[DLTPGConfig[Suffix].TextMovement].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != DLTPGConfig[Suffix].TextMovement)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            DLTPGConfig[Suffix].TextMovement = ucharval;

            TrxDLTPGTextMovement(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  DLTextScale                                                RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the DL text scase command.                           */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void DLTextScale()
  {
  UC type;

  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        switch (ParState)
          {
          case 0:
            ParState++;

            ucharval = atoi(Cmd);
            ucharval--;

            if ((ucharval < ScaleOne) || (ucharval > ScaleFour))
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

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          sprintf(TmpBuffer, "%d\r\n", DLTPGConfig[Suffix].TextScale + 1);
          Udr_PutNChar(TmpBuffer);
          }
        }
      else
        {
        if (ParState < 1)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != DLTPGConfig[Suffix].TextScale)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            DLTPGConfig[Suffix].TextScale = ucharval;

            TrxDLTPGTextScale(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  DLTextColor                                                RS232CMD.C  */
/*                                                                         */
/* Author:    Jk                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the DL text color command.                           */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*                                                                         */
/* Notes:    DLTPGConfig[DLTPGNoNdx].TextColor member carries text color   */
/*           and text background info in the following manner:             */
/*                                                                         */
/*           |X|R|G|B|X|R|G|B| .TextColor byte                             */
/*                                                                         */
/*           Bits 0-2 represent text RGB value, bits 4-6 represent         */
/*           background RGB value.                                         */
/***************************************************************************/
void DLTextColor()
  {
  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, HDTextColotPar);

          if (ucharval > ColorWhite)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDTextColotPar[DLTPGConfig[Suffix].TextColor & 0x0F].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != (DLTPGConfig[Suffix].TextColor & 0x0F))
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            DLTPGConfig[Suffix].TextColor &= 0xF0;      // Zero text color
            DLTPGConfig[Suffix].TextColor |= ucharval;

            TrxDLTPGTextColor(Suffix, DLTPGConfig[Suffix].TextColor);
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
/*  DLTextBack                                                 RS232CMD.C  */
/*                                                                         */
/* Author:    Jk                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the DL text background color command.                */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*                                                                         */
/* Notes:    DLTPGConfig[DLTPGNoNdx].TextColor member carries text color   */
/*           and text background info in the following manner:             */
/*                                                                         */
/*           |X|R|G|B|X|R|G|B| .TextColor byte                             */
/*                                                                         */
/*           Bits 0-2 represent text RGB value, bits 4-6 represent         */
/*           background RGB value.                                         */
/***************************************************************************/
void DLTextBack()
  {
  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, HDTextColotPar);

          if (ucharval > ColorWhite)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDTextColotPar[DLTPGConfig[Suffix].TextColor >> 4].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != (DLTPGConfig[Suffix].TextColor >> 4))
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            ucharval = ucharval << 4;

            DLTPGConfig[Suffix].TextColor &= 0x0F;      // Zero text backg. color
            DLTPGConfig[Suffix].TextColor |= ucharval;

            TrxDLTPGTextColor(Suffix, DLTPGConfig[Suffix].TextColor);
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
/*  DLTextPosition                                             RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the DL text-positioning command.                     */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*              ParState 2: DecNum,DecNum                                  */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void DLTextPosition()
  {
  UC type;

  if (DLTPGUnit[0].Present || DLTPGUnit[2].Present || DLTPGUnit[4].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        switch (ParState)
          {
          case 0:
            ParState++;

            ucharval = atoi(Cmd);

            if ((ucharval < HDXPosMin) || (ucharval > HDXPosMax))
              SCPIError = DataOutOfRange;
            break;

          case 1:
            ParState++;

            UcharVal1 = atoi(Cmd);

            if ((UcharVal1 < HDYPosMin) || (UcharVal1 > HDYPosMax))
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

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          sprintf(TmpBuffer, "%d,%d\r\n", 
                DLTPGConfig[Suffix].TextPos[0],
                DLTPGConfig[Suffix].TextPos[1]);
          Udr_PutNChar(TmpBuffer);
          }
        }
      else
        {
        if (ParState < 2)
          SCPIError = MissingPar;
        else
          {
          if ((ucharval != DLTPGConfig[Suffix].TextPos[0]) ||
             (UcharVal1 != DLTPGConfig[Suffix].TextPos[1]))
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            DLTPGConfig[Suffix].TextPos[0] = ucharval;
            DLTPGConfig[Suffix].TextPos[1] = UcharVal1;

            TrxDLTPGTextPos(Suffix, 
                            DLTPGConfig[Suffix].TextPos[0], 
                            DLTPGConfig[Suffix].TextPos[1]);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }



/******************************************************************
 * NAME       void HD()
 * PURPOSE    To handle HD command
 * 
 * ARGUMENTS  None
 *
 * RETURNS    Nothing
 *
 * EXTERNS    None
 *
 * NOTES:     This function tackles HD querry of the form:
 *            :OUTP:HDx?;, i.e. it prints system and delay for 
 *            generator x.
 * 
 ******************************************************************/
void HD()
  {
  UC    tmp;

  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        /* Pattern */
        tmp = HDTPGConfig[Suffix].Pattern;
        Udr_CPutNChar(HDPatternPar[tmp].LongName);
        Udr_PutChar(',');
        /* Text ON/OFF */
        if (HDTPGConfig[Suffix].TextEnable)
          tmp = 1;
        else
          tmp = 0;
        Udr_CPutNChar(OffOnPar[tmp].LongName);
        Udr_PutChar(',');
        /* System */
        tmp = HDTPGConfig[Suffix].System;
        Udr_CPutNChar(HDFormatPar[tmp].LongName);
        Udr_PutChar(',');
        /* Audio Signal */
        Udr_CPutNChar(HDAudioSignalPar[HDTPGConfig[Suffix].AudioSignal].LongName);
        Udr_PutChar(',');
        /* Audio Level */
        Udr_CPutNChar(HDAudioLevelPar[HDTPGConfig[Suffix].AudioLevel].LongName);
        Udr_PutChar(',');
        /* Timing */
        TLGSamplesToFLT(HDTPGConfig[Suffix].System, HDTPGConfig[Suffix].Delay, 
                          &sign, &Field, &Line, &HTime, hdtpg_fmt_table);
        PrintTimingFLTTLG(sign, Field, Line, HTime);
        Udr_PutNewline();
        }
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
  }

void HDSuffix()
  {
  switch (NumSuffix)
    {
    case  1:
    case  2:
    case  3:
    case  4:
    case  5:
    case  6:
    case  7:
    case  8:
    case  9:
    case 10:
    case 11:
    case 12:
      Suffix = NumSuffix - 1;
      break;
    default:
      SCPIError = HdrSuffixOutOfRange;
      break;
    }
  }


void HDFormat()
  {

  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;
      case P_Char:
        if (ParState == 0)
          {
          ParState++;
          ucharval = FindParameter(Cmd, HDFormatPar);

          if (ucharval == MAX_HD_SYSTEMS)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;
      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDFormatPar[HDTPGConfig[Suffix].System].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != HDTPGConfig[Suffix].System)
            {
            ActivePresetNo = 0;      // Clear possible preset
            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();
            HDTPGConfig[Suffix].System = ucharval;
            //HDTPGConfig[Suffix].Delay  = 0;
            HDTPGConfig[Suffix].Delay = adjust_hd_samples(HDTPGConfig[Suffix].Delay, ucharval, hdtpg_fmt_table);

            TrxHDTPGSystem(Suffix, HDTPGConfig[Suffix].System,
                        HDTPGConfig[Suffix].Delay,
                        GenlockConfig[ActiveGenlockNo].Delay);

            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }


void HDDelay()
  {
  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    FindDelayParameters();

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          TLGSamplesToFLT(HDTPGConfig[Suffix].System, HDTPGConfig[Suffix].Delay, 
                          &sign, &Field, &Line, &HTime, hdtpg_fmt_table);
  
          PrintTimingFLTTLG(sign, Field, Line, HTime);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState < 3)
          SCPIError = MissingPar;
        else
          {
          UlongVal = TLGFLTToSamples(HDTPGConfig[Suffix].System, Field, Line, HTime,
                                     hdtpg_fmt_table);
          if (UlongVal != -1)
            {
            if (UlongVal != HDTPGConfig[Suffix].Delay)
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              HDTPGConfig[Suffix].Delay = UlongVal;  /* Save new Delay... */

              TrxHDTPGSystem(Suffix, HDTPGConfig[Suffix].System,
                          HDTPGConfig[Suffix].Delay,
                          GenlockConfig[ActiveGenlockNo].Delay);
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
/*  HDPattern                                                  RS232CMD.C  */
/*                                                                         */
/* Author:    Jk                                                           */
/* Revised:    000608                                                      */
/*                                                                         */
/*  Function:  Handle the HD pattern command.                              */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*  Returns:     --                                                        */
/*  Updates:    --                                                         */
/***************************************************************************/
void HDPattern()
  {
  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, HDPatternPar);

          if (ucharval > HDBlack)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDPatternPar[HDTPGConfig[Suffix].Pattern].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != HDTPGConfig[Suffix].Pattern)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            HDTPGConfig[Suffix].Pattern = ucharval;

            TrxHDTPGPattern(Suffix, ucharval);
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
/*  HDValidateAttrib                                           RS232CMD.C  */
/*                                                                         */
/* Author:   JK                                                            */
/* Revised:  05.01.2007                                                    */
/*                                                                         */
/*  Function:  To check if given attribute is valid for given pattern.     */
/*  Returns:   1 if valid                                                  */
/*             0 if not valid                                              */
/*  Remarks:  The Attrib array member contains attribs for following       */
/*            patterns only:                                               */
/*            Attrib[0] = attrib for Colorbar: 0,1 or 2                    */
/*            Attrib[1] = attrib for Combinat: 0,1 or 2                    */
/*            Attrib[2] = attrib for Window:   3...25                      */
/*            Attrib[3] = attrib for White:    3...25                      */
/*            i.e. Pattern is the index for Attrib                         */
/*  Updates:    --                                                         */
/***************************************************************************/
UC HDValidateAttrib(UC patt, UC att)
  {
  switch (patt)
    {
    case 0:
    case 1:
      if (att > 2)
        return (0);
      else
        return (1);
      break;
    case 2:
    case 3:
      if ((att > 2) && (patt < 26))
        return (1);
      else
        return (0);
      break;
    default:
      return (0);
    }
       
  }

/***************************************************************************/
/*  HDAttrCommon                                               RS232CMD.C  */
/*                                                                         */
/* Author:   JK                                                            */
/* Revised:  05.01.2007                                                    */
/*                                                                         */
/*  Function:  Handle the HD pattern attributes (modificatons)             */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*            This function checks if which pattern it is, then calls      */
/*            appropriate function for tackling Colorbar and Combination   */
/*            attributes, or function for tackling Window and White attrib.*/
/*             The syntax is:                                              */
/*              :outp:hd5:patt:mod?;                                       */
/*              :outp:hd5:patt:mod A105;                                   */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void HDAttrCommon()
  {
  UC tmp, pattern;

  pattern = HDTPGConfig[Suffix].Pattern;

  if (!(HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present))
    {
    SCPIError = HardwareMissing;
    return;
    }

  switch (pattern)
    {
    case HDColorbar:
    case HDCombination:
      do_hd_common(HDItensPar, 3);
      break;

    case HDWhite:
    case HDWindow:
      do_hd_common(HDLevelPar, 23);
      break;

    default:
      SCPIError = ExecErr;
      return;
      break;
    }
  }

void do_hd_common(code struct PAR_STRUCT *pptr, UC limit)
  {
  UC tmp, pattern;

  pattern = HDTPGConfig[Suffix].Pattern;


  switch (ParameterType)
    {
    case P_NoParameter:
      ParState = 0;
      break;

    case P_Char:
      if (ParState == 0)
        {
        ParState++;

        ucharval = FindParameter(Cmd, pptr);

        if (ucharval == limit)
          SCPIError = IllegalParValue;
        }
      else
        SCPIError = ParNotAllowed;
      break;

    default:
      SCPIError = DataTypeErr;
      break;
    }

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParState)
        SCPIError = ParNotAllowed;
      else
        {
        /* Get the current attribute of a selected pattern */
        tmp = HDTPGConfig[Suffix].Attrib[pattern];

        Udr_CPutNChar(pptr[tmp].LongName);
        Udr_PutNewline();
        }
      }
    else
      {
      if (ParState == 0)
        SCPIError = MissingPar;
      else
        {
        if (HDTPGConfig[Suffix].Attrib[pattern] != ucharval)
          {
          ActivePresetNo = 0;

          if (Ptr == &status_menu[StatusPresetItem])
            Ptr->DispFunct();

          HDTPGConfig[Suffix].Attrib[pattern] = ucharval;

          TrxHDTPGAttributes(Suffix, ucharval);
          }
        }
      }
    }
  }

void HDAudioSignal()
  {
  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, HDAudioSignalPar);

          if (ucharval > HDAudioOff)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
      break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDAudioSignalPar[HDTPGConfig[Suffix].AudioSignal].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != HDTPGConfig[Suffix].AudioSignal)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            HDTPGConfig[Suffix].AudioSignal = ucharval;

            TrxHDTPGAudioSignal(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/* void HDAudioLevel()                                MENU.C  */
/*                                                                         */
/* Author:   Janusz Kuzminski                                              */
/* Revised:  31.07.2007                                                    */
/*                                                                         */
/* Function: To do HD audio Level                                          */
/*                                                                         */
/* Remarks:                                                                */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Notes:    Levels now are from 0 to -60 dB with step of 1, so this       */
/*           this routine behaves NOT like its equivalents for other       */
/*           generators.                                                   */
/***************************************************************************/
void HDAudioLevel()
  {
  code UC* tablePtr;
  int  level;
  UC   txtbuff[10];

  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      //case P_Char:
      case P_DecNum:
        if (ParState == 0)
          {
          ParState++;

          //ucharval = FindParameter(Cmd, HDAudioLevelPar); 

          IntVal = atoi(Cmd);
          IntVal = abs(IntVal);

          if ((IntVal < 0))
            SCPIError = IllegalParValue;
          if ((IntVal > 60))
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;  // 
        else
          {
          IntVal = HDTPGConfig[Suffix].AudioLevel;

          if (IntVal)
            sprintf(txtbuff, "-%d", IntVal);
          else
            sprintf(txtbuff, " %d", IntVal);

          Udr_PutNChar(txtbuff);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (IntVal != HDTPGConfig[Suffix].AudioLevel)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            HDTPGConfig[Suffix].AudioLevel = IntVal;

            TrxHDTPGAudioLevel(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }


void HDAudioClick()
  {
  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        if (ParState == 0)
          {
          ParState++;

          IntVal = atoi(Cmd);

          if ((IntVal < ClickLow) || (IntVal > ClickHigh))
            SCPIError = DataOutOfRange;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          sprintf(TmpBuffer, "%d\r\n", HDTPGConfig[Suffix].AudioClick);
          Udr_PutNChar(TmpBuffer);
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else {
          if (IntVal != HDTPGConfig[Suffix].AudioClick)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            HDTPGConfig[Suffix].AudioClick = IntVal;

            TrxHDTPGAudioClick(Suffix, IntVal);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }


/***************************************************************************/
/*  HDTextStringSuffix                                         RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Get suffix of the HD text string (maybe 1,2 or 3, i.e       */
/*             text line nr.                                               */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    StringSuffix                                               */
/***************************************************************************/
void HDTextStringSuffix()
  {
  if (NumSuffix > 3)
    SCPIError = SuffixErr;
  else
    StringSuffix = NumSuffix - 1;
  }

/***************************************************************************/
/*  HDTextString                                               RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the HD test string command.                          */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*              ParState 2: DecNum, Char                                   */
/*              ParState 3: DecNum, String                                 */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void HDTextString()
  {
  register int i, j;
  UC tmp, noOfChar;

  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:         // user entered ON/OFF
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, OffOnPar);

          if (ucharval > ON)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = DataTypeErr;
        break;

      case P_String:
        if (ParState == 0)
          {
          ParState += 2;

          noOfChar = PT863xMaxNoChar;

          for (j = i = 0; i < strlen(Cmd); i++, j++)
            {
            tmp = Cmd[i];  

            if ((tmp < 0x20) || (tmp > 0x7E))
              {
              SCPIError = DataOutOfRange;
              break;
              }
            else
              Cmd[j] = tmp;
            }
          Cmd[j] = 0;

          if (strlen(Cmd) > noOfChar)
            SCPIError = DataOutOfRange;
          }
        else
          SCPIError = DataTypeErr;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      } //switch (ParameterType)

    if (CmdExecute && !SCPIError)
      {
      tmp = StringSuffix;

      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          i = (HDTPGConfig[Suffix].TextEnable & (1 << tmp)) > 0;

          Udr_CPutNChar(OffOnPar[i].LongName);
          Udr_PutNChar(",\"");
          Udr_PutNChar(HDTPGConfig[Suffix].Text[tmp]);
          Udr_PutChar('"');
          Udr_PutNewline();
          }
        }
      else
        {
        switch (ParState)
          {
          case 0:
            SCPIError = MissingPar;
            break;

          case 1:               // user entered ON/0FF ?
            if ((HDTPGConfig[Suffix].TextEnable & (1 << tmp)) ^ (ucharval << tmp))
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              HDTPGConfig[Suffix].TextEnable &= ~(1 << tmp);
              HDTPGConfig[Suffix].TextEnable |= (ucharval << tmp);

              TrxHDTPGText(Suffix, &HDTPGConfig[Suffix]);
              }
            break;

          case 2:               // user entered some text ?
            if (strcmp(SDITPGConfig[Suffix].Text[tmp], Cmd))
              {
              ActivePresetNo = 0;      // Clear possible preset

              if (Ptr == &status_menu[StatusPresetItem])
                Ptr->DispFunct();

              strcpy(HDTPGConfig[Suffix].Text[tmp], Cmd);
              TrxHDTPGText(Suffix, &HDTPGConfig[Suffix]);
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
/*  HDTextMovement                                             RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the HD Text Movement command.                        */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void HDTextMovement()
  {
  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, TextMovePar);

          if (ucharval > 3)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      } // switch (ParameterType)

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(TextMovePar[HDTPGConfig[Suffix].TextMovement].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != HDTPGConfig[Suffix].TextMovement)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            HDTPGConfig[Suffix].TextMovement = ucharval;

            TrxHDTPGTextMovement(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  HDTextScale                                                RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the HD text scase command.                           */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void HDTextScale()
  {
  UC type;

  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        switch (ParState)
          {
          case 0:
            ParState++;

            ucharval = atoi(Cmd);
            ucharval--;

            if ((ucharval < ScaleOne) || (ucharval > ScaleFour))
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

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          sprintf(TmpBuffer, "%d\r\n", HDTPGConfig[Suffix].TextScale + 1);
          Udr_PutNChar(TmpBuffer);
          }
        }
      else
        {
        if (ParState < 1)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != HDTPGConfig[Suffix].TextScale)
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            HDTPGConfig[Suffix].TextScale = ucharval;

            TrxHDTPGTextScale(Suffix, ucharval);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }

/***************************************************************************/
/*  HDTextColor                                                RS232CMD.C  */
/*                                                                         */
/* Author:    Jk                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the HD text color command.                           */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*                                                                         */
/* Notes:    HDTPGConfig[HDTPGNoNdx].TextColor member carries text color   */
/*           and text background info in the following manner:             */
/*                                                                         */
/*           |X|R|G|B|X|R|G|B| .TextColor byte                             */
/*                                                                         */
/*           Bits 0-2 represent text RGB value, bits 4-6 represent         */
/*           background RGB value.                                         */
/***************************************************************************/
void HDTextColor()
  {
  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, HDTextColotPar);

          if (ucharval > ColorWhite)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDTextColotPar[HDTPGConfig[Suffix].TextColor & 0x0F].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != (HDTPGConfig[Suffix].TextColor & 0x0F))
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            HDTPGConfig[Suffix].TextColor &= 0xF0;      // Zero text color
            HDTPGConfig[Suffix].TextColor |= ucharval;

            TrxHDTPGTextColor(Suffix, HDTPGConfig[Suffix].TextColor);
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
/*  HDTextBack                                                 RS232CMD.C  */
/*                                                                         */
/* Author:    Jk                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the HD text background color command.                */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: Char                                           */
/*                                                                         */
/* Notes:    HDTPGConfig[HDTPGNoNdx].TextColor member carries text color   */
/*           and text background info in the following manner:             */
/*                                                                         */
/*           |X|R|G|B|X|R|G|B| .TextColor byte                             */
/*                                                                         */
/*           Bits 0-2 represent text RGB value, bits 4-6 represent         */
/*           background RGB value.                                         */
/***************************************************************************/
void HDTextBack()
  {
  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_Char:
        if (ParState == 0)
          {
          ParState++;

          ucharval = FindParameter(Cmd, HDTextColotPar);

          if (ucharval > ColorWhite)
            SCPIError = IllegalParValue;
          }
        else
          SCPIError = ParNotAllowed;
        break;

      default:
        SCPIError = DataTypeErr;
        break;
      }

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else
          {
          Udr_CPutNChar(HDTextColotPar[HDTPGConfig[Suffix].TextColor >> 4].LongName);
          Udr_PutNewline();
          }
        }
      else
        {
        if (ParState == 0)
          SCPIError = MissingPar;
        else
          {
          if (ucharval != (HDTPGConfig[Suffix].TextColor >> 4))
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            ucharval = ucharval << 4;

            HDTPGConfig[Suffix].TextColor &= 0x0F;      // Zero text backg. color
            HDTPGConfig[Suffix].TextColor |= ucharval;

            TrxHDTPGTextColor(Suffix, HDTPGConfig[Suffix].TextColor);
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
/*  HDTextPosition                                             RS232CMD.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   04.01.2007                                                   */
/*                                                                         */
/*  Function:  Handle the HD text-positioning command.                     */
/*  Remarks:    ParState 0: Idle                                           */
/*              ParState 1: DecNum                                         */
/*              ParState 2: DecNum,DecNum                                  */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void HDTextPosition()
  {
  UC type;

  if (HDTPGUnit[0].Present || HDTPGUnit[4].Present || HDTPGUnit[8].Present)
    {
    switch (ParameterType)
      {
      case P_NoParameter:
        ParState = 0;
        break;

      case P_DecNum:
        switch (ParState)
          {
          case 0:
            ParState++;

            ucharval = atoi(Cmd);

            if ((ucharval < HDXPosMin) || (ucharval > HDXPosMax))
              SCPIError = DataOutOfRange;
            break;

          case 1:
            ParState++;

            UcharVal1 = atoi(Cmd);

            if ((UcharVal1 < HDYPosMin) || (UcharVal1 > HDYPosMax))
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

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        {
        if (ParState)
          SCPIError = ParNotAllowed;
        else {
          sprintf(TmpBuffer, "%d,%d\r\n", 
                HDTPGConfig[Suffix].TextPos[0],
                HDTPGConfig[Suffix].TextPos[1]);
          Udr_PutNChar(TmpBuffer);
          }
        }
      else
        {
        if (ParState < 2)
          SCPIError = MissingPar;
        else
          {
          if ((ucharval != HDTPGConfig[Suffix].TextPos[0]) ||
             (UcharVal1 != HDTPGConfig[Suffix].TextPos[1]))
            {
            ActivePresetNo = 0;      // Clear possible preset

            if (Ptr == &status_menu[StatusPresetItem])
              Ptr->DispFunct();

            HDTPGConfig[Suffix].TextPos[0] = ucharval;
            HDTPGConfig[Suffix].TextPos[1] = UcharVal1;

            TrxHDTPGTextPos(Suffix, 
                            HDTPGConfig[Suffix].TextPos[0], 
                            HDTPGConfig[Suffix].TextPos[1]);
            }
          }
        }
      }
    }
  else
    SCPIError = HardwareMissing;
  }



void HDVersion()
  {
  if (HDTPGUnit[Suffix].Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        WriteUnitInformation(HDTPGUnit[Suffix].HWType, Suffix);
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
  }

  
/****************************************************************************/
/*  LTCVersion                                    				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Show the LTC generator version	      	     				*/
/*  Remarks:   :OUTP:LTCG#:VERS?;                         					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCVersion()
{
	if (GPSGenlockUnit.Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        WriteUnitInformation(GPSGenlockUnit.HWType, Suffix);
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
}
  
/****************************************************************************/
/*  LTC		                                       				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Show info about the LTC generator      	     				*/
/*  Remarks:   :OUTP:LTCG#?;                               					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTC()
{
	if (CmdExecute && !SCPIError)
		SCPIError = SyntaxErr;
}
  
/***************************************************************************/
/*  LTCSuffix                                       				RS232CMD.C  */
/*                                                  							*/
/* Author:    JKH                        									   */
/* Revised:    
/*                                                							  */
/*  Function:  Get suffix of the LTC generator           					   */
/* Remarks:    --                                       					 */
/*  Returns:    --                                       					 */
/*  Updates:    Suffix                                 					   */
/***************************************************************************/
void LTCSuffix()
  {
	switch(NumSuffix)
	{
		case 1:
		case 2:
			Suffix = NumSuffix - 1;
			break;
		default:
		  SCPIError = HdrSuffixOutOfRange;
	}
  }
  
/****************************************************************************/
/*  LTCOffset                                      				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Get/set delay of the LTC generator           				*/
/*  Remarks:    --                                       					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCOffset()
{
	long offset=0;
    double offsetDouble;
  
	if(GPSGenlockUnit.Present)
	{
		switch (ParameterType)
		{
			case P_NoParameter:
				ParState=0;
				break;
				
			case P_DecNum:
				switch (ParState)
				{
					case 0:
						ParState++;
						
						offsetDouble = atof(Cmd);
						if((offsetDouble>500000000) || (offsetDouble<-500000000))
							SCPIError = DataOutOfRange;
					break;
					
					default:
						SCPIError = SyntaxErr;
						break;
				}	
				break;
			default:
				SCPIError = DataTypeErr;
			break;
		}
					
		if(CmdExecute && !SCPIError)
		{
			if(CmdRequest)
			{
				if(ParState)
					SCPIError = ParNotAllowed;
				else
				{
					offset=LTCConfig[Suffix].Offset;
					offsetDouble=offset/0.1485;
    				sprintf(TmpBuffer, "%012.1f", offsetDouble);
    				Udr_PutNChar(TmpBuffer);
    				Udr_PutNewline();
				}
			}
			else
			{
				if(ParState < 1)
					SCPIError = MissingPar;
				else
				{
					offset = offsetDouble*0.1485;
					LTCConfig[Suffix].Offset = offset;
					
					 //send data to GPS module
				    if(Suffix==0)
				    {
				      IIC1Write2(d2ram_addr, 0x07, (unsigned char)(offset));
				      IIC1Write2(d2ram_addr, 0x08, (unsigned char)(offset>>8));
				      IIC1Write2(d2ram_addr, 0x09, (unsigned char)(offset>>16));
				      IIC1Write2(d2ram_addr, 0x0A, (unsigned char)(offset>>24));
				    }
				    else
				    {
				      IIC1Write2(d2ram_addr, 0x1B, (unsigned char)(offset));
				      IIC1Write2(d2ram_addr, 0x1C, (unsigned char)(offset>>8));
				      IIC1Write2(d2ram_addr, 0x1D, (unsigned char)(offset>>16));
				      IIC1Write2(d2ram_addr, 0x1E, (unsigned char)(offset>>24));
				    }
				    
				    ActivePresetNo = 0;      // Clear possible preset
				}
			}
		}
	}
	else
		SCPIError = HardwareMissing;
}

/****************************************************************************/
/*  LTCFormat                                      				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Get/set format of the LTC generator           				*/
/*  Remarks:   ex: :OUTP:LTCG#:FORMAT '24FPS','NONE',12,30;   				*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCFormat()
{
	if(GPSGenlockUnit.Present)
	{			
		switch (ParameterType)
		{
			case P_NoParameter:
				ParState = 0;
				break;

			case P_String:
				if (ParState == 0)
				{
					ParState++;

					tmp_ltc.Format = FindParameter(Cmd, LTCFormatPar);

					if (tmp_ltc.Format > 4)
						SCPIError = IllegalParValue;
				}
				else if (ParState == 1)
				{
					ParState++;	
					
					tmp_ltc.DFMode = FindParameter(Cmd, LTCModePar);

					if (tmp_ltc.DFMode > 2)
						SCPIError = IllegalParValue;
				}
				else
					SCPIError = SyntaxErr;
				break;
			
			case P_DecNum:
				if (ParState==2)
				{
					ParState++;
					
					tmp_ltc.DFHour = (char)atoi(Cmd);
					if((tmp_ltc.DFHour>=24) || (tmp_ltc.DFHour<0))
						SCPIError = DataOutOfRange;
				}
				else if(ParState==3)
				{
					ParState++;
					
					tmp_ltc.DFMin = (char)atoi(Cmd);
					if((tmp_ltc.DFMin%10!=0) || (tmp_ltc.DFMin>50) || (tmp_ltc.DFMin<0))
						SCPIError = DataOutOfRange;	
				}
				else
					SCPIError = SyntaxErr;
				break;
						
			default:
				SCPIError = DataTypeErr;
				break;
		}
				
		if(CmdExecute && !SCPIError)
		{
			if(CmdRequest)
			{
				if(ParState)
					SCPIError = ParNotAllowed;
				else
				{
					switch(LTCConfig[Suffix].Format)
					{
						case 0:	Udr_CPutNChar(LTC24FPS);		break;
						case 1:	Udr_CPutNChar(LTC25FPS);		break;
						case 2:	Udr_CPutNChar(LTC2997DROP);		break;
						case 3:	Udr_CPutNChar(LTC2997NOND); 	break;
						case 4:	Udr_CPutNChar(LTC30FPS); 		break;
						default:	Udr_CPutNChar(ERRORCmdTxt);
					}
					
					Udr_PutChar(',');
					
					switch(LTCConfig[Suffix].DFMode)
					{
						case 0:	Udr_CPutNChar(LTCNONE);	break;
						case 1:	Udr_CPutNChar(LTCCONF);	break;
						case 2:	Udr_CPutNChar(LTCAUTO);	break;
						default:	Udr_CPutNChar(ERRORCmdTxt);
					}
					
					Udr_PutChar(',');
					
					sprintf(TmpBuffer, "%u,%u\n", LTCConfig[Suffix].DFHour, LTCConfig[Suffix].DFMin);
					Udr_PutNChar(TmpBuffer);
					Udr_PutNewline();
				}
			}
			else
			{
				if(ParState<4)
					SCPIError = MissingPar;
				else
				{
					LTCConfig[Suffix].Format=tmp_ltc.Format;
					LTCConfig[Suffix].DFMode=tmp_ltc.DFMode;
					LTCConfig[Suffix].DFHour=tmp_ltc.DFHour;
					LTCConfig[Suffix].DFMin=tmp_ltc.DFMin;
					
					if(Suffix==0)
					{
	    				IIC1Write2(d2ram_addr, 0x0B, tmp_ltc.Format);
	    				IIC1Write2(d2ram_addr, 0x0C, tmp_ltc.DFMode);
	      				IIC1Write2(d2ram_addr, 0x0D, tmp_ltc.DFHour);
	      				IIC1Write2(d2ram_addr, 0x0E, tmp_ltc.DFMin);
					}
	    			else
	    			{
	      				IIC1Write2(d2ram_addr, 0x1F, tmp_ltc.Format);
	      				IIC1Write2(d2ram_addr, 0x20, tmp_ltc.DFMode);
	      				IIC1Write2(d2ram_addr, 0x21, tmp_ltc.DFHour);
	      				IIC1Write2(d2ram_addr, 0x22, tmp_ltc.DFMin);
	  				}
  				}
			}
		}
	}
	else
		SCPIError=HardwareMissing;
}

/****************************************************************************/
/*  LTCFormat                                      				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Get/set time of the LTC generator    	       				*/
/*  Remarks:    --                                       					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCTime()
{
	if(CmdExecute && !SCPIError)
		SCPIError = SyntaxErr;
}


/****************************************************************************/
/*  LTCTimezone                                    				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Get/set timezone of the LTC generator    	       			*/
/*  Remarks:    --                                       					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCTimezone()
{
	// Hardware present?
	if(GPSGenlockUnit.Present)
	{			
		switch (ParameterType)
		{
			// First time run
			case P_NoParameter:
				ParState = 0;
				break;
				
			case P_DecNum:
				switch(ParState)
				{
					case 0:
						tmp_ltc.TimezoneHour = (char)atoi(Cmd);
						
						//only between -12 and 12
						if((tmp_ltc.TimezoneHour<-12) || (tmp_ltc.TimezoneHour>12))
							SCPIError = DataOutOfRange;
						break;
					case 1:
						tmp_ltc.TimezoneMin = (char)atoi(Cmd);
						
						//-12:30 or 12:30 not allowed
						if((tmp_ltc.TimezoneHour==12) || (tmp_ltc.TimezoneHour==-12))
							if(tmp_ltc.TimezoneMin==30)
								SCPIError = DataOutOfRange;
						
						//only 00 or 30 allowed
						if((tmp_ltc.TimezoneMin%30!=0) || (tmp_ltc.TimezoneMin<0))
							SCPIError = DataOutOfRange;
						break;
						
					default:
						SCPIError = SyntaxErr;
				}
				
				ParState++;
				break;
			
			default:
				SCPIError = DataTypeErr;
				break;
		}
		
		// Whole command received
		if(CmdExecute && !SCPIError)
		{
			// Request
			if(CmdRequest)
			{
				// Parameters on request
				if(ParState)
					SCPIError = ParNotAllowed;
				else
				{
					// Print info
					sprintf(TmpBuffer, "%i,%i\n", LTCConfig[Suffix].TimezoneHour, LTCConfig[Suffix].TimezoneMin);
					Udr_PutNChar(TmpBuffer);
					Udr_PutNewline();
				}
			}
			// Send parameters
			else
			{
				if(ParState<2)
					SCPIError = MissingPar;
				else
				{
					LTCConfig[Suffix].TimezoneHour = tmp_ltc.TimezoneHour;
					LTCConfig[Suffix].TimezoneMin = tmp_ltc.TimezoneMin;
					
					if(tmp_ltc.TimezoneHour<0)
						tmp_ltc.TimezoneMin=-1*tmp_ltc.TimezoneMin;
						
					//send data to GPS module
				    if(Suffix==0)
				    {
				      IIC1Write2(d2ram_addr, 0x0F, tmp_ltc.TimezoneHour);
				      IIC1Write2(d2ram_addr, 0x10, tmp_ltc.TimezoneMin);
				    }
				    else
				    {
				      IIC1Write2(d2ram_addr, 0x23, tmp_ltc.TimezoneHour);
				      IIC1Write2(d2ram_addr, 0x24, tmp_ltc.TimezoneMin);
				    }
				}
			}
		}
	}
	// No hardware
	else
		SCPIError=HardwareMissing;	
}

/****************************************************************************/
/*  LTCDaylight                                    				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Get/set daylight of the LTC generator    	       			*/
/*  Remarks:    --                                       					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCDaylight()
{
		
		// Hardware present?
	if(GPSGenlockUnit.Present)
	{			
		switch (ParameterType)
		{
			// First time run
			case P_NoParameter:
				ParState = 0;
				break;
			
			default:
				SCPIError = DataTypeErr;
				break;

		}
		
		// Whole command received
		if(CmdExecute && !SCPIError)
		{
			// Request
			if(CmdRequest)
			{
				// Parameters on request
				if(ParState)
					SCPIError = ParNotAllowed;
					
				// Print info
				else
				{
					switch(LTCConfig[Suffix].DaylightMode)
					{
						case 0:	Udr_CPutNChar(LTCNONE);	break;
						case 1:	Udr_CPutNChar(LTCCONF);	break;
						case 2:	Udr_CPutNChar(LTCAUTO);	break;
						default:	Udr_CPutNChar(ERRORCmdTxt);
					}
					
					Udr_PutChar(',');
					
					if(LTCConfig[Suffix].DaylightState)
						Udr_CPutNChar(ONParTxt);
					else
						Udr_CPutNChar(OFFParTxt);
						
					sprintf(TmpBuffer,",%u,%u,%u,%u,%u,%u\n", LTCConfig[Suffix].DaylightStartMonth+1, LTCConfig[Suffix].DaylightStartDay, LTCConfig[Suffix].DaylightStartHour,
															LTCConfig[Suffix].DaylightEndMonth+1, LTCConfig[Suffix].DaylightEndDay, LTCConfig[Suffix].DaylightEndHour);
					Udr_PutNChar(TmpBuffer);
					Udr_PutNewline();
				}
			}
			// Send parameters
			else
			{
				SCPIError = SyntaxErr;
			}
		}
	}
	// No hardware
	else
		SCPIError=HardwareMissing;	
}

/****************************************************************************/
/*  LTCDLStart                                     				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Get/set start of daylight			   	 	       			*/
/*  Remarks:    --                                       					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCDLStart()
{
	// Hardware present?
	if(GPSGenlockUnit.Present)
	{			
		switch (ParameterType)
		{
			// First time run
			case P_NoParameter:
				ParState = 0;
				break;
				
			case P_DecNum:
				switch(ParState)
				{
					case 0:
						tmp_ltc.DaylightStartMonth = (char)(atoi(Cmd) - 1); //months = 0-11
						if((tmp_ltc.DaylightStartMonth>11) || (tmp_ltc.DaylightStartMonth<0))
							SCPIError = DataOutOfRange;
						break;
						
					case 1:
						tmp_ltc.DaylightStartDay = (char)(atoi(Cmd)); 
						if((tmp_ltc.DaylightStartDay < 1) || (tmp_ltc.DaylightStartDay > (month_days[tmp_ltc.DaylightStartMonth])))
							SCPIError = DataOutOfRange;
						break;
					case 2:
						tmp_ltc.DaylightStartHour = (char)(atoi(Cmd));
						if((tmp_ltc.DaylightStartHour<0) ||(tmp_ltc.DaylightStartHour > 23))
							SCPIError = DataOutOfRange;
						break;
					default:
						SCPIError = SyntaxErr;
				}
				ParState++;
				break;
			default:
				SCPIError = DataTypeErr;
				break;
		}
		
		// Whole command received
		if(CmdExecute && !SCPIError)
		{
			// Request
			if(CmdRequest)
			{
				// Parameters on request
				if(ParState)
					SCPIError = ParNotAllowed;
					
				// Print info
				else
				{
					sprintf(TmpBuffer,"%u,%u,%u\n", LTCConfig[Suffix].DaylightStartMonth+1, LTCConfig[Suffix].DaylightStartDay, LTCConfig[Suffix].DaylightStartHour);
					Udr_PutNChar(TmpBuffer);
					Udr_PutNewline();
				}
			}
			// Send parameters
			else
			{
				if(ParState < 3)
					SCPIError = MissingPar;
				else
				{
					LTCConfig[Suffix].DaylightStartMonth = tmp_ltc.DaylightStartMonth;
					LTCConfig[Suffix].DaylightStartDay = tmp_ltc.DaylightStartDay;
					LTCConfig[Suffix].DaylightStartHour = tmp_ltc.DaylightStartHour;
					
					if(Suffix==0)
				    {
				      IIC1Write2(d2ram_addr, 0x12, tmp_ltc.DaylightStartMonth);
				      IIC1Write2(d2ram_addr, 0x13, tmp_ltc.DaylightStartDay);
				      IIC1Write2(d2ram_addr, 0x14, tmp_ltc.DaylightStartHour);
				    }
				    else
				    {
				      IIC1Write2(d2ram_addr, 0x26, tmp_ltc.DaylightStartMonth);
				      IIC1Write2(d2ram_addr, 0x27, tmp_ltc.DaylightStartDay);
				      IIC1Write2(d2ram_addr, 0x28, tmp_ltc.DaylightStartHour);
				    }	
				}	
			}
		}
	}
	// No hardware
	else
		SCPIError=HardwareMissing;	
}

/****************************************************************************/
/*  LTCDLEnd                                      				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Get/set end of daylight				    	       			*/
/*  Remarks:    --                                       					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCDLEnd()
{
	// Hardware present?
	if(GPSGenlockUnit.Present)
	{			
		switch (ParameterType)
		{
			// First time run
			case P_NoParameter:
				ParState = 0;
				break;
				
			case P_DecNum:
				switch(ParState)
				{
					case 0:
						tmp_ltc.DaylightEndMonth = (char)(atoi(Cmd) - 1); //months = 0-11
						if((tmp_ltc.DaylightEndMonth>11) || (tmp_ltc.DaylightEndMonth<0))
							SCPIError = DataOutOfRange;
						break;
						
					case 1:
						tmp_ltc.DaylightEndDay = (char)(atoi(Cmd)); 
						if((tmp_ltc.DaylightEndDay < 1) || (tmp_ltc.DaylightEndDay > (month_days[tmp_ltc.DaylightEndMonth])))
							SCPIError = DataOutOfRange;
						break;
						
					case 2:
						tmp_ltc.DaylightEndHour = (char)(atoi(Cmd));
						if((tmp_ltc.DaylightEndHour<0) ||(tmp_ltc.DaylightEndHour > 23))
							SCPIError = DataOutOfRange;
						break;
						
					default:
						SCPIError = SyntaxErr;
				}
				ParState++;
				break;
				
			default:
				SCPIError = DataTypeErr;
				break;
		}
		
		// Whole command received
		if(CmdExecute && !SCPIError)
		{
			// Request
			if(CmdRequest)
			{
				// Parameters on request
				if(ParState)
					SCPIError = ParNotAllowed;
					
				// Print info
				else
				{
					sprintf(TmpBuffer,"%u,%u,%u\n", LTCConfig[Suffix].DaylightEndMonth+1, LTCConfig[Suffix].DaylightEndDay, LTCConfig[Suffix].DaylightEndHour);
					Udr_PutNChar(TmpBuffer);
					Udr_PutNewline();
				}
			}
			// Send parameters
			else
			{
				if(ParState < 3)
					SCPIError = MissingPar;
				else
				{
					LTCConfig[Suffix].DaylightEndMonth = tmp_ltc.DaylightEndMonth;
					LTCConfig[Suffix].DaylightEndDay = tmp_ltc.DaylightEndDay;
					LTCConfig[Suffix].DaylightEndHour = tmp_ltc.DaylightEndHour;
					
					if(Suffix==0)
				    {
				      IIC1Write2(d2ram_addr, 0x15, tmp_ltc.DaylightEndMonth);
				      IIC1Write2(d2ram_addr, 0x16, tmp_ltc.DaylightEndDay);
				      IIC1Write2(d2ram_addr, 0x17, tmp_ltc.DaylightEndHour);
				    }
				    else
				    {
				      IIC1Write2(d2ram_addr, 0x29, tmp_ltc.DaylightEndMonth);
				      IIC1Write2(d2ram_addr, 0x2A, tmp_ltc.DaylightEndDay);
				      IIC1Write2(d2ram_addr, 0x2B, tmp_ltc.DaylightEndHour);
				    }	
				}	
			}
		}
	}
	// No hardware
	else
		SCPIError=HardwareMissing;	
}

/****************************************************************************/
/*  LTCDLMode                                      				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  Get/set LTC Daylight switch mode		    	       			*/
/*  Remarks:    --                                       					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCDLMode()
{
	// Hardware present?
	if(GPSGenlockUnit.Present)
	{			
		switch (ParameterType)
		{
			// First time run
			case P_NoParameter:
				ParState = 0;
				break;

			case P_String:
				switch(ParState)
				{
					case 0:
						tmp_ltc.DaylightMode = FindParameter(Cmd, LTCModePar);
						if(tmp_ltc.DaylightMode>2)
							SCPIError = IllegalParValue;
						break;
						
					case 1:
						tmp_ltc.DaylightState = FindParameter(Cmd, OffOnPar);
						if(tmp_ltc.DaylightState>1)
							SCPIError = IllegalParValue;
						break;
						
					default:
						SCPIError = SyntaxErr;
				}
				ParState++;
				break;
			
			default:
				SCPIError = DataTypeErr;
				break;
		}
		
		// Whole command received
		if(CmdExecute && !SCPIError)
		{
			// Request
			if(CmdRequest)
			{
				// Parameters on request
				if(ParState)
					SCPIError = ParNotAllowed;
					
				// Print info
				else
				{
					switch(LTCConfig[Suffix].DaylightMode)
					{
						case 0:	Udr_CPutNChar(LTCNONE);	break;
						case 1:	Udr_CPutNChar(LTCCONF);	break;
						case 2:	Udr_CPutNChar(LTCAUTO);	break;
						default:	Udr_CPutNChar(ERRORCmdTxt);
					}
					
					Udr_PutChar(',');
					
					if(LTCConfig[Suffix].DaylightState)
						Udr_CPutNChar(ONParTxt);
					else
						Udr_CPutNChar(OFFParTxt);
						
					Udr_PutNewline();				
				}
			}
			// Send parameters
			else
			{
				if(ParState<2)
					SCPIError = MissingPar;
				else
				{
					LTCConfig[Suffix].DaylightMode=tmp_ltc.DaylightMode;
					LTCConfig[Suffix].DaylightState=tmp_ltc.DaylightState;
					
					if(Suffix==0)
			          	IIC1Write2(d2ram_addr, 0x11, tmp_ltc.DaylightMode);
			      	else
			        	IIC1Write2(d2ram_addr, 0x25, tmp_ltc.DaylightMode);
			            
			        LTCConfig[Suffix].DaylightState=tmp_ltc.DaylightState;
			        	
			      	//send daylight savings mode
			      	UcharVal1=0;
			      
			      	if(LTCConfig[0].DaylightState==0)
			        	UcharVal1|=2;
			      	else
			      		UcharVal1|=1;
			      
			    	if(LTCConfig[1].DaylightState==0)
			        	UcharVal1|=32;
			      	else
			      		UcharVal1|=16;
			      
			    	IIC1Write2(d2ram_addr, 0x38, UcharVal1);
				}
			}
		}
	}
	// No hardware
	else
		SCPIError=HardwareMissing;
}

/****************************************************************************/
/*  LTCDebug                                      				RS232CMD.C  */
/*                                                  						*/
/* Author:    JKH                        								    */
/* Revised:   																*/
/*                                                							*/
/*  Function:  reads and prints debug string from PT8616					*/
/*  Remarks:    --                                       					*/
/*  Returns:    --                                       					*/
/*  Updates:    --                                  					    */
/****************************************************************************/
void LTCDebug()
{
	if (GPSGenlockUnit.Present)
    {
    if (ParameterType != P_NoParameter)
      SCPIError = ParNotAllowed;

    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
      {
	      	IIC1RdStr(d2ram_addr, 100, TmpBuffer);
			Udr_PutNChar(TmpBuffer);
			Udr_PutNChar(",");
			IIC1RdStr(d2ram_addr, 109, TmpBuffer);
			Udr_PutNChar(TmpBuffer);
			Udr_PutNChar(",");
			IIC1RdStr(d2ram_addr, 110, TmpBuffer);
			Udr_PutNChar(TmpBuffer);
			Udr_PutNChar(",");
			IIC1RdStr(d2ram_addr, 111, TmpBuffer);
			Udr_PutNChar(TmpBuffer);
			Udr_PutNChar(",");
			IIC1RdStr(d2ram_addr, 112, TmpBuffer);
			Udr_PutNChar(TmpBuffer);			
			Udr_PutNewline();
	  }
      else
        SCPIError = SyntaxErr;
      }
    }
  else
    SCPIError = HardwareMissing;
}