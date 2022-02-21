/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:   PT5230 Digital Video Generator                              */
/*  Module:    TEXT.C                                                      */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date: 980118                                                      */
/*  Rev. date: 031215, JK       ...   TLS1                                        */
/*  Status:    Version 1.5                                                 */
/*                                                                         */
/*  This module contains NO functions.                                     */
/*                                                                         */
/*  Changes:                                                               */
/* 031215: Added TLG menu texts                                            */
/* 000508: Added  PositionTxt[]                                            */
/* 991025: "master reset.. " text has been removed                         */
/* 990420: Added Window 10% & 15 kHz Bl/Wh in both SDI & Analog.           */
/* 990305: Added LockHdrTxt[]                                              */
/* 980918: Added TOOSLOWTxt[] & TOOFASTTxt[].                              */
/* 980917: Added new sdi pattern texts. Renamed text in MonoBBC1kHzTxt     */
/* 980916: Added new analog pattern texts.                                 */
/* 980915: Added SYNCLOCKEDTxt[]                                           */
/* 980727: Added CoModifyTxt                                               */
/* 980723: Added text for TCI status                                       */
/* 980709: Added LOCKEDTxt[], NOSIGNALTxt[]                                */
/* 980702: Updated DateTimeRefArrTxt[]                                     */ 
/* 980616: Released as 4008 002 06812                                      */
/* 980611: Added AUDIO level text                                          */
/* 980616: Released as 4008 002 06811                                      */
/***************************************************************************/

#include <xa.h>
#include "mas.h"
#include "text.h"

code char PT5230HeadlineTxt[]    = "HD-SD Sync Generator Frame.";

code char PowerUpDefaultTxt[]    = "HD-SD Sync Gen. Power-up diagnose  ";
code char PowerUpFactResetTxt[]  = "HD-SD Sync Gen. Factory-reset ...  ";

code char StatusPresetTxt[]      = "HD-SD Preset Status:";
code char StatusErrorStatusTxt[] = "HD-SD Error/Warning Status:";

/********************  HDTPG  ********************************/

code char HDDownloadTxt[]        = "HD Firmware Download...";


code char HDLevel0dBTxt[]          = "  0 dBFS";
code char HDLevel6dBTxt[]          = " -6 dBFS";
code char HDLevel12dBTxt[]         = " -12 dBFS";
code char HDLevel18dBTxt[]         = " -18 dBFS";
code char HDLevel24dBTxt[]         = " -24 dBFS";

code char* HDAudioLevelArrTxt[] =
  {
  HDLevel0dBTxt,
  HDLevel6dBTxt,
  HDLevel12dBTxt,
  HDLevel18dBTxt,
  HDLevel24dBTxt
  };

code char HDSilenceTxt[]   = "Silence";
code char HDSineTxt[]      = "Sine";
code char HDClickTxt[]     = "Click";
code char HDAudioOffTxt[]  = "Off";

code char OnOffTxt[]       = "ON-OFF";

code char* HDAudioSignalArrTxt[] =
  {
  HDSilenceTxt,
  HDSineTxt,
  HDClickTxt,
  HDAudioOffTxt
  };


code char PatBlTxt[]  = "BLACK";
code char PatCfTxt[]  = "SDI CHECKFIELD";
code char PatPlTxt[]  = "PLUGE";
code char PatLrTxt[]  = "LUMINANCE RAMP";
code char PatClTxt[]  = "CLAPPERBOARD";
code char PatCbTxt[]  = "COLORBAR";
code char PatCoTxt[]  = "COMBINATION";
code char PatWiTxt[]  = "WINDOW";
code char PatCrTxt[]  = "CROSSHATCH";
code char PatWhTxt[]  = "WHITE";


code char* HDTPGPattArrTxt[] = 
  {
  PatCbTxt, // colorbar
  PatCoTxt, // combination
  PatWiTxt, // window
  PatWhTxt, // white
  PatCrTxt, // crosshatch
  PatCfTxt, // sdi checkfield
  PatPlTxt, // pluge
  PatLrTxt, // luminance ramp
  PatClTxt, // clapperboard
  PatBlTxt  // black
  };


code char AttibTxt0[]  = "100/0/100/0";
code char AttibTxt1[]  = "100/0/75/0";
code char AttibTxt2[]  = "75/0/75/0";
code char AttibTxt3[]  = " -5%";   
code char AttibTxt4[]  = "  0%";   
code char AttibTxt5[]  = "  5%";   
code char AttibTxt6[]  = " 10%";   
code char AttibTxt7[]  = " 15%";   
code char AttibTxt8[]  = " 20%";   
code char AttibTxt9[]  = " 25%";   
code char AttibTxt10[] = " 30%";   
code char AttibTxt11[] = " 35%";   
code char AttibTxt12[] = " 40%";   
code char AttibTxt13[] = " 45%";   
code char AttibTxt14[] = " 50%";   
code char AttibTxt15[] = " 55%";   
code char AttibTxt16[] = " 60%";   
code char AttibTxt17[] = " 65%";   
code char AttibTxt18[] = " 70%";   
code char AttibTxt19[] = " 75%";   
code char AttibTxt20[] = " 80%";   
code char AttibTxt21[] = " 85%";   
code char AttibTxt22[] = " 90%";   
code char AttibTxt23[] = " 95%";   
code char AttibTxt24[] = "100%";   
code char AttibTxt25[] = "105%";   
code char AttibTxt26[] = "16:9";   
code char AttibTxt27[] = " 4:3";   

code char* HDTPGACBttribArrTxt[] = 
  { 
  AttibTxt0,       // "100/0/100/0"; HundHund = 0,
  AttibTxt1,       // "100/0/75/0";  HundSeven,   
  AttibTxt2        // "75/0/75/0";   SevenSeven, 
  };
 
code char* HDTPGAWindowttribArrTxt[] = 
  {
  AttibTxt3,       // " -5%White";   WhiteM5,     
  AttibTxt4,       // "  0%White";   White0,      
  AttibTxt5,       // "  5%White";   White5,      
  AttibTxt6,       // " 10%White";   White15,     
  AttibTxt7,       // " 15%White";   White10,     
  AttibTxt8,       // " 20%White";   White20,     
  AttibTxt9,       // " 25%White";   White25,     
  AttibTxt10,      // " 30%White";   White30,     
  AttibTxt11,      // " 35%White";   White35,     
  AttibTxt12,      // " 40%White";   White40,     
  AttibTxt13,      // " 45%White";   White45,     
  AttibTxt14,      // " 50%White";   White50,     
  AttibTxt15,      // " 55%White";   White55,     
  AttibTxt16,      // " 60%White";   White60,     
  AttibTxt17,      // " 65%White";   White65,     
  AttibTxt18,      // " 70%White";   White70,     
  AttibTxt19,      // " 75%White";   White75,     
  AttibTxt20,      // " 80%White";   White80,     
  AttibTxt21,      // " 85%White";   White85,     
  AttibTxt22,      // " 90%White";   White90,     
  AttibTxt23,      // " 95%White";   White95,     
  AttibTxt24,      // "100%White";   White100,    
  AttibTxt25       // "105%White";   White105, 
  };

code char* HDTPGAChttribArrTxt[] = 
  {   
  AttibTxt26,      // "16:9";        SixNine,     
  AttibTxt27       // " 4:3";        FourThree    
  };

/*********************************/
code char OneTxt[]               = "1";
code char TwoTxt[]               = "2";
code char ThreTxt[]              = "3";
code char FourTxt[]              = "4";

code char WhiteTxt[]             = "WHITE";
code char YellowTxt[]            = "YELLOW";
code char CyanTxt[]              = "CYAN";
code char GreenTxt[]             = "GREEN";
code char MagentaTxt[]           = "MAGENTA";
code char RedTxt[]               = "RED";
code char BlueTxt[]              = "BLUE";
code char BblackTxt[]            = "BLACK";
                                
code char MovementHorTxt[]       = "HORIZONTAL";
code char MovementVerTxt[]       = "VERTICAL";
code char MovementOffTxt[]       = "OFF";
code char MovementBothTxt[]      = "BOTH";

code char ScaleTxt[]      = "SCALE";
code char ColorTxt[]      = "TEXT COLOR";
code char BackgroundTxt[] = "BACKGROUND COLOR";
code char MovementTxt[]   = "MOVEMENT";

code char HD1Txt[]      = "HDBOARD1";
code char HD2Txt[]      = "HDBOARD2";
code char HD3Txt[]      = "HDBOARD3";

code char TRI1Txt[]     = "TLSBOARD1";
code char TRI2Txt[]     = "TLSBOARD2";
code char TRI3Txt[]     = "TLSBOARD3";

code char HDTPGTxt[]    = "HD-SDI";
code char HDTPG1Txt[]   = "HD1";
code char HDTPG2Txt[]   = "HD2";
code char HDTPG3Txt[]   = "HD3";
code char HDTPG4Txt[]   = "HD4";
code char HDTPG5Txt[]   = "HD5";
code char HDTPG6Txt[]   = "HD6";
code char HDTPG7Txt[]   = "HD7";
code char HDTPG8Txt[]   = "HD8";
code char HDTPG9Txt[]   = "HD9";
code char HDTPG10Txt[]  = "HD10";
code char HDTPG11Txt[]  = "HD11";
code char HDTPG12Txt[]  = "HD12";

code char HDTPGConnTxt1[]        = "[ HDG1]: ";
code char HDTPGConnTxt2[]        = "[ HDG2]: ";
code char HDTPGConnTxt3[]        = "[ HDG3]: ";

code char F0Txt[]                = "OFF";
code char F1Txt[]                = "HD 1080P/60";
code char F2Txt[]                = "HD 1080P/59.94";
code char F3Txt[]                = "HD 1080P/50";
                                 
code char F4Txt[]                = "HD 1080I/30";   /* These are used in HDTPG  */
code char F5Txt[]                = "HD 1080I/29.97";
code char F6Txt[]                = "HD 1080I/25";
code char F7Txt[]                = "HD 1080P/30";
code char F8Txt[]                = "HD 1080P/29.97";
code char F9Txt[]                = "HD 1080P/25";
code char F10Txt[]               = "HD 1080P/24";
code char F11Txt[]               = "HD 1080P/23.98";
code char F14Txt[]               = "HD 720P/60";
code char F15Txt[]               = "HD 720P/59.94";
code char F16Txt[]               = "HD 720P/50";
code char F17Txt[]               = "HD 720P/30";
code char F18Txt[]               = "HD 720P/29.97";
code char F19Txt[]               = "HD 720P/25";
code char F20Txt[]               = "HD 720P/24";
code char F21Txt[]               = "HD 720P/23.98";

code char F22Txt[]               = "SD 487I/29.97 (525)";  /* These are used in HDTPG  */
code char F23Txt[]               = "SD 576I/25 (625)";

/* Specific for DL  and TLG */
code char F30Txt[]    = "HD 1080sF/30";
code char F31Txt[]    = "HD 1080sF/29.97";
code char F32Txt[]    = "HD 1080sF/25";
code char F33Txt[]    = "HD 1080sF/24";
code char F34Txt[]    = "HD 1080sF/23.98";

code char* TLGFormatArrTxt[MAX_TLG_SYSTEMS] = 
  {
  F0Txt,     /* = 0  "OFF"            */
  F1Txt,     /* = 1  "HD 1080P/60"    */
  F2Txt,     /* = 2  "HD 1080P/59.94" */
  F3Txt,     /* = 3  "HD 1080P/50"    */
  F4Txt,     /* = 4  "HD 1080I/30"    */
  F5Txt,     /* = 5  "HD 1080I/29.97" */
  F6Txt,     /* = 6  "HD 1080I/25"    */
  F7Txt,     /* = 7  "HD 1080P/30"    */
  F8Txt,     /* = 8  "HD 1080P/29.97" */
  F9Txt,     /* = 9  "HD 1080P/25"    */
  F10Txt,    /* = 10 "HD 1080P/24"    */
  F11Txt,    /* = 11 "HD 1080P/23.98" */

  F30Txt,    /* = 12 "1080sF/30";     */
  F31Txt,    /* = 13 "1080sF/29.97";  */
  F32Txt,    /* = 14 "1080sF/25";     */
  F33Txt,    /* = 15 "1080sF/24";     */
  F34Txt,    /* = 16 "1080sF/23.98";  */

  F14Txt,    /* = 17 "HD 720P/60"     */
  F15Txt,    /* = 18 "HD 720P/59.94"  */
  F16Txt,    /* = 19 "HD 720P/50"     */
  F17Txt,    /* = 20 "HD 720P/30"     */
  F18Txt,    /* = 21 "HD 720P/29.97"  */
  F19Txt,    /* = 22 "HD 720P/25"     */
  F20Txt,    /* = 23 "HD 720P/24"     */
  F21Txt     /* = 24 "HD 720P/23.98"  */ /* 25 elements */
  };

/* These are used in RS232CMD.c: Systems for TLG */
code char L1Txt[]   = "OFF";                                
code char L2Txt[]   = "HD1080P60";                          
code char L3Txt[]   = "HD1080P5994";                       
code char L4Txt[]   = "HD1080P50";                          
code char L5Txt[]   = "HD1080I30";                          
code char L6Txt[]   = "HD1080I2997";                       
code char L7Txt[]   = "HD1080I25";                          
code char L8Txt[]   = "HD1080P30";                          
code char L9Txt[]   = "HD1080P2997";                       
code char L10Txt[]  = "HD1080P25";                          
code char L11Txt[]  = "HD1080P24";                          
code char L12Txt[]  = "HD1080P2398";

code char L13Txt[]  = "HD1080SF30";                          
code char L14Txt[]  = "HD1080SF2997";                       
code char L15Txt[]  = "HD1080SF25";                          
code char L16Txt[]  = "HD1080SF24";                          
code char L17Txt[]  = "HD1080SF2398";

code char L18Txt[]  = "HD720P60";                           
code char L19Txt[]  = "HD720P5994";                        
code char L20Txt[]  = "HD720P50";                       
code char L21Txt[]  = "HD720P30";                       
code char L22Txt[]  = "HD720P2997";                    
code char L23Txt[]  = "HD720P25";                       
code char L24Txt[]  = "HD720P24";                       
code char L25Txt[]  = "HD720P2398";   /* 25 elements */
/* End Systems for TLG */

/* These are used in RS232CMD.c: Systems for DL */
code char  N0Txt[]               = "OFF";            
code char  N4Txt[]               = "HD1080I30";    
code char  N5Txt[]               = "HD1080I2997";                       
code char  N6Txt[]               = "HD1080I25";                          
code char  N7Txt[]               = "HD1080P30";                          
code char  N8Txt[]               = "HD1080P2997";                       
code char  N9Txt[]               = "HD1080P25";                          
code char  N10Txt[]              = "HD1080P24";                               
code char  N11Txt[]              = "HD1080P2398";

code char  N30Txt[]              = "HD1080SF30";           
code char  N31Txt[]              = "HD1080SF2997";        
code char  N32Txt[]              = "HD1080SF25";           
code char  N33Txt[]              = "HD1080SF24";           
code char  N34Txt[]              = "HD1080SF2398";

code char  N14Txt[]              = "HD720P60";                                
code char  N15Txt[]              = "HD720P5994";                             
code char  N16Txt[]              = "HD720P50";                                
code char  N17Txt[]              = "HD720P30";                                
code char  N18Txt[]              = "HD720P2997";                             
code char  N19Txt[]              = "HD720P25";                                
code char  N20Txt[]              = "HD720P24";                                
code char  N21Txt[]              = "HD720P2398";                             
code char  N22Txt[]              = "SD525"; 
code char  N23Txt[]              = "SD625";

code char* HDTPGSystemArrTxt[MAX_HD_SYSTEMS] = /* 19 changed to 24 */
  {
  F0Txt,     /* = 0   = "OFF";                   */
  F4Txt,     /* = 1   = "HD 1080I/30";           */
  F5Txt,     /* = 2   = "HD 1080I/29.97";        */                             
  F6Txt,     /* = 3   = "HD 1080I/25";           */                             
  F7Txt,     /* = 4   = "HD 1080P/30";           */                             
  F8Txt,     /* = 5   = "HD 1080P/29.97";        */                             
  F9Txt,     /* = 6   = "HD 1080P/25";           */                             
  F10Txt,    /* = 7   = "HD 1080P/24";           */                             
  F11Txt,    /* = 8   = "HD 1080P/23.98";        */ 

  N30Txt,    /* = 9  "1080sF/30";     */
  N31Txt,    /* = 10 "1080sF/29.97";  */
  N32Txt,    /* = 11 "1080sF/25";     */
  N33Txt,    /* = 12 "1080sF/24";     */
  N34Txt,    /* = 13 "1080sF/23.98";  */

  F14Txt,    /* = 14  = "HD 720P/60";            */                             
  F15Txt,    /* = 15  = "HD 720P/59.94";         */                             
  F16Txt,    /* = 16  = "HD 720P/50";            */                             
  F17Txt,    /* = 17  = "HD 720P/30";            */                             
  F18Txt,    /* = 18  = "HD 720P/29.97";         */                             
  F19Txt,    /* = 19  = "HD 720P/25";            */                             
  F20Txt,    /* = 20  = "HD 720P/24";            */                             
  F21Txt,    /* = 21  = "HD 720P/23.98";         */                             
  F22Txt,    /* = 22  = "SD 487I/29.97 (525)";   */
  F23Txt     /* = 23  = "SD 576I/25 (625)";      */                             
  };

/* These are used in RS232CMD.c: Systems for HD */
code char M0Txt[] =      "OFF";                 
code char M4Txt[] =      "HD1080I30";         
code char M5Txt[] =      "HD1080I2997";                                   
code char M6Txt[] =      "HD1080I25";                                      
code char M7Txt[] =      "HD1080P30";                                      
code char M8Txt[] =      "HD1080P2997";                                   
code char M9Txt[] =      "HD1080P25";                                      
code char M10Txt[] =     "HD1080P24";                                      
code char M11Txt[] =     "HD1080P2398";       

//code char  N30Txt[]              = "HD1080SF30";       /* taken from rs232cmd.c */
//code char  N31Txt[]              = "HD1080SF2997";        
//code char  N32Txt[]              = "HD1080SF25";           
//code char  N33Txt[]              = "HD1080SF24";           
//code char  N34Txt[]              = "HD1080SF2398";        


code char M14Txt[] =     "HD720P60";                                       
code char M15Txt[] =     "HD720P5994";                                    
code char M16Txt[] =     "HD720P50";                                       
code char M17Txt[] =     "HD720P30";                                       
code char M18Txt[] =     "HD720P2997";                                    
code char M19Txt[] =     "HD720P25";                                       
code char M20Txt[] =     "HD720P24";                                       
code char M21Txt[] =     "HD720P2398";                                    
code char M22Txt[] =     "SD525"; 
code char M23Txt[] =     "SD625";                                 
/* End Systems for HD */


code char* DLTPGSystemArrTxt[MAX_DL_SYSTEMS] = /* 24 */
  {
  F0Txt,     /* = 0   = "OFF";                   */
  F4Txt,     /* = 1   = "HD 1080I/30";           */
  F5Txt,     /* = 2   = "HD 1080I/29.97";        */                             
  F6Txt,     /* = 3   = "HD 1080I/25";           */                             
  F7Txt,     /* = 4   = "HD 1080P/30";           */                             
  F8Txt,     /* = 5   = "HD 1080P/29.97";        */                             
  F9Txt,     /* = 6   = "HD 1080P/25";           */                             
  F10Txt,    /* = 7   = "HD 1080P/24";           */                             
  F11Txt,    /* = 8   = "HD 1080P/23.98";        */                             

  F30Txt,    /* = 9   = "1080sF/30";             */
  F31Txt,    /* = 10  = "1080sF/29.97";          */
  F32Txt,    /* = 11  = "1080sF/25";             */
  F33Txt,    /* = 12  = "1080sF/24";             */
  F34Txt,    /* = 13  = "1080sF/23.98";          */

  F14Txt,    /* = 14  = "HD 720P/60";            */                             
  F15Txt,    /* = 15  = "HD 720P/59.94";         */                             
  F16Txt,    /* = 16  = "HD 720P/50";            */                             
  F17Txt,    /* = 17  = "HD 720P/30";            */                             
  F18Txt,    /* = 18  = "HD 720P/29.97";         */                             
  F19Txt,    /* = 19  = "HD 720P/25";            */                             
  F20Txt,    /* = 20  = "HD 720P/24";            */                             
  F21Txt,    /* = 21  = "HD 720P/23.98";         */                             
  F22Txt,    /* = 22  = "SD 487I/29.97 (525)";   */
  F23Txt     /* = 23  = "SD 576I/25 (625)";      */                             
  };


code char* HDTextColorArrTxt[]  =
  {
  BblackTxt, 
  BlueTxt,
  GreenTxt,
  CyanTxt,
  RedTxt,
  MagentaTxt,
  YellowTxt,
  WhiteTxt
  };


code char* HDTextMovArrTxt[]  =
  {
  MovementOffTxt,
  MovementVerTxt,
  MovementHorTxt,
  MovementBothTxt
  };

code char* HDTextScaleArrTxt[] =
  {
  OneTxt,
  TwoTxt,
  ThreTxt,
  FourTxt
  };

code char* HDArrTxt[] =  /* JK addition */
  {
  HD1Txt,
  HD2Txt,
  HD3Txt
  };

/********************  HDTPG  ********************************/


/********************  DLTPG  ********************************/

code char DL1Txt[]      = "DLBOARD1";
code char DL2Txt[]      = "DLBOARD2";
code char DL3Txt[]      = "DLBOARD3";

code char DLTPGTxt[]    = "DL-SDI";
code char DLTPG1Txt[]   = "DL1";
code char DLTPG2Txt[]   = "DL2";
code char DLTPG3Txt[]   = "DL3";
code char DLTPG4Txt[]   = "DL4";
code char DLTPG5Txt[]   = "DL5";
code char DLTPG6Txt[]   = "DL6";

code char DLTPGConnTxt1[]        = "[ DLG1]: ";
code char DLTPGConnTxt2[]        = "[ DLG2]: ";
code char DLTPGConnTxt3[]        = "[ DLG3]: ";

code char* DLArrTxt[] =  /* JK addition */
  {
  DL1Txt,
  DL2Txt,
  DL3Txt
  };

code char SystInter1[]      = "SINGLE 4:2:2 YCbCr 10-bit";
code char SystInter2[]      = "DUAL 4:2:2:4 YCbCrA 12-bit";
code char SystInter3[]      = "DUAL 4:4:4 YCbCr 10-bit";
code char SystInter4[]      = "DUAL 4:4:4:4 YCbCrA 12-bit";
code char SystInter5[]      = "DUAL 4:4:4 GBR 10-bit";
code char SystInter6[]      = "DUAL 4:4:4:4 GBRA 12-bit";

code char *DLTPGSystInterTxt[] =
  {
  SystInter1,
  SystInter2,
  SystInter3,
  SystInter4,
  SystInter5,
  SystInter6,
  };

code char InterfaceTxt[] = "Interface";


/********************  DLTPG  ********************************/


code char SelfTestTxt[]               = "Selftest in progress ...";
code char InternalTestTxt[]           = "Internal test passed ...";

code char PressExecuteToStartTxt[]      = "Press EXECUTE to start download!";
code char WarningAboutToAlterTxt[]      = "WARNING! About to alter data";
code char WarningDownloadInTxt[]        = "WARNING! Download in progress";

code char ParallelRemoteOperatingTxt[]  = "Parallel remote operating.";
code char SerialRemoteOperatingTxt[]    = "Serial remote operating.";
code char LocalLockoutTxt[]             = "Local lockout.";

code char CheckCableTxt[]               = "Check cable & RS232 settings";
code char ChecksumErrorReceivingTxt[]   = "Checksum error receiving data";
code char DataNotCompatibleTxt[]        = "Data are not compatible";

code char DiagnoseDisplayLN1Txt[]       = "ABCEDFGHIJKLMNOPQRSTUVWXYZ_!\x22#$%&/()=?;:";
code char DiagnoseDisplayLN2Txt[]       = "abcdefghijklmnopqrstuvwxyz-1234567890+,.";

code char NoPresetActiveTxt[]           = "No preset active";

code char PresetHdrTxt[]                = "PRESET : ";
code char MenuHdrTxt[]                  = "MENU  : ";
code char SubmnuHdrTxt[]                = "SUBMNU: ";
code char SecondmnuHdrTxt[]             = "2NDMNU: ";
code char ThirdmnuHdrTxt[]              = "3RDMNU: ";
code char ForthmnuHdrTxt[]				= "4THMNU: ";
code char SelectHdrTxt[]                = "SELECT: ";
code char ModifyHdrTxt[]                = "MODIFY: ";
code char AnlTPG2HdrTxt[]               = "ANALOG-TPG2: ";
code char AnlTPG5HdrTxt[]               = "ANALOG-TPG5: ";
code char SDITSG2HdrTxt[]               = "SDI-TSG2: ";
code char SDITSG3HdrTxt[]               = "SDI-TSG3: ";
code char SDITSG4HdrTxt[]               = "SDI-TSG4: ";
code char SDITPG1HdrTxt[]               = "SDI-TPG1: ";
code char SDITPG2HdrTxt[]               = "SDI-TPG2: ";
code char SDITPG5HdrTxt[]               = "SDI-TPG5: ";

code char AESEBU1HdrTxt[]               = "AES-EBU1: ";
code char AESEBU2HdrTxt[]               = "AES-EBU2: ";

code char SystemHdrTxt[]                = "SYSTEM: ";
code char PatternHdrTxt[]               = "PATTERN: ";
code char ScHPhaseHdrTxt[]              = "ScH-PHASE: ";
code char EDHHdrTxt[]                   = "EDH-INSERTION: ";
code char GroupHdrTxt[]                 = "GROUP: ";
code char SignalHdrTxt[]                = "SIGNAL: ";
code char LevelHdrTxt[]                 = "LEVEL: ";
code char Spaces6Txt[]                  = "     ";
code char TimingHdrTxt[]                = "TIMING: ";
code char TimeVHdrTxt[]                 = "V: ";
code char TimeFHdrTxt[]                 = "F: ";
code char TimeHHdrTxt[]                 = "  H: ";
code char TimeTHdrTxt[]                 = "  T: ";
code char UserTextHdrTxt[]              = "USER TEXT: ";
code char AudioHdrTxt[]                 = "AUDIO: ";
code char GenlockHdrTxt[]               = "GENLOCK: ";
code char LTCHdrTxt[]         = "LTC: ";
code char TextHdrTxt[]                  = "TEXT: ";
code char RecallHdrTxt[]                = "RECALL : ";
code char StoreHdrTxt[]                 = "STORE : ";
code char NameHdrTxt[]                  = "NAME : ";
code char LockHdrTxt[]                  = "LOCK: ";
code char AutoReturnStatusHdrTxt[]      = "AUTO RETURN TO STATUS: ";
code char ReferenceHdrTxt[]             = "REFERENCE: ";
code char DownloadHdrTxt[]              = "DOWNLOAD: ";
code char BaudrateHdrTxt[]              = "BAUD-RATE: ";
code char DatabitHdrTxt[]               = "DATA-BIT: ";
code char ParityHdrTxt[]                = "PARITY: ";
code char HandshakeHdrTxt[]             = "HANDSHAKE: ";
code char StatusHdrTxt[]                = "STATUS: ";
code char DateHdrTxt[]                  = "DATE: ";
code char TimeHdrTxt[]                  = "TIME: ";
code char RefHdrTxt[]                   = "REF: ";
code char OffsetHdrTxt[]                = "OFFSET: ";
code char ErrorNoHdrTxt[]               = "E(   ): ";
code char VersionHdrTxt[]               = "Version: ";
code char TestingMemoryHdrTxt[]         = "Testing memory: ";
code char PressButtonHdrTxt[]           = "Press button: ";

code char SlashDotHdrTxt[]       = "../";
code char ThreeDotsTxt[]         = "...";
code char degTxt[]               = " deg";
code char mSTxt[]                = " ms";
code char MenuTxt[]              = "menu";
code char SubmnuTxt[]            = "submenu";

code char SYSTxt[]               = "SYS";
code char EDITTxt[]              = "EDIT";
code char MODIFYTxt[]            = "MODIFY";
code char INTERFACETxt[]         = "INTERFACE";
code char SAVETxt[]              = "SAVE";
code char OKTxt[]                = "OK";
code char FAILTxt[]              = "FAIL";
code char ESCTxt[]               = "ESC";
code char ROMTxt[]               = "ROM";
code char RAMTxt[]               = "RAM";

code char CoSelectTxt[]          = ", select";
code char CoSelectFunctionTxt[]  = ", select function";
code char CoSelectInputTxt[]     = ", select input";
code char CoSelectSignalTxt[]    = ", select signal";
code char CoEditTxt[]            = ", edit";
code char CoEditDelayTxt[]       = ", edit delay";
code char CoEditTextTxt[]        = ", edit text";
code char CoEditNameTxt[]        = ", edit name";
code char CoModifyTxt[]          = ", modify";
code char CoConfigureTxt[]       = ", configure";
code char CoStandardPatternTxt[] = ", standard pattern";
code char CoComplexPatternTxt[]  = ", complex pattern";

code char HyphenTxt[]            = "------";
                            
code char FromKUTxt[]            = " from KU";

code char TLGTxt[]         = "TRI-LEVEL";
code char TLG1Txt[]        = "TLS1";
code char TLG2Txt[]        = "TLS2";
code char TLG3Txt[]        = "TLS3";
code char TLG4Txt[]        = "TLS4";
code char TLG5Txt[]        = "TLS5";
code char TLG6Txt[]        = "TLS6";
code char TLG7Txt[]        = "TLS7";
code char TLG8Txt[]        = "TLS8";
code char TLG9Txt[]        = "TLS9";
code char TLG10Txt[]       = "TLS10";
code char TLG11Txt[]       = "TLS11";
code char TLG12Txt[]       = "TLS12";
code char FormatTxt[]      = "SYSTEM";
/************************************************/


code char AnalogTPG2Txt[]  = "ANALOG-TPG2";
code char AnalogTPG5Txt[]  = "ANALOG-TPG5";
code char AnlTPG2Txt[]     = "ANL-TPG2";
code char AnlTPG5Txt[]     = "ANL-TPG5";
code char SDITSG2Txt[]     = "SDI-TSG2";
code char SDITSG3Txt[]     = "SDI-TSG3";
code char SDITSG4Txt[]     = "SDI-TSG4";
code char SDITPG1Txt[]     = "SDI-TPG1";
code char SDITPG2Txt[]     = "SDI-TPG2";
code char SDITPG5Txt[]     = "SDI-TPG5";
code char SystemTxt[]      = "SYSTEM";
code char PatternTxt[]     = "PATTERN";
code char ScHPhaseTxt[]    = "ScH-PHASE";
code char TimingTxt[]      = "TIMING";
code char ClickTxt[]       = "CLICK OFFSET";
code char EDHTxt[]         = "EDH";
code char TextTxt[]        = "TEXT";
code char AudioTxt[]       = "AUDIO";
code char StyleTxt[]       = "STYLE";
code char PositionTxt[]    = "POS";
code char ClockTxt[]       = "CLOCK";
code char GroupTxt[]       = "GROUP";
code char SignalTxt[]      = "SIGNAL";
code char LevelTxt[]       = "LEVEL";
code char LevelsTxt[]       = "LEVELS";
code char ConfigTxt[]      = "CONFIG";
code char OSCTxt[]         = "OSC";
code char RS232Txt[]       = "RS232";
code char RecallTxt[]      = "RECALL";
code char StoreTxt[]       = "STORE";
code char NameTxt[]        = "NAME";
code char LTCModeTxt[]     = "MODE";
code char LTCTimezoneTxt[] = "TIMEZONE";
code char LTCDaylightTxt[] = "DAYLIGHT";
code char LTCDateTxt[]     = "DATE";
code char LTCFormatTxt[]  = "FORMAT";
code char LTCSyncTxt[]      = "SYNC";
code char LTCStartTxt[]     = "START";
code char LTCEndTxt[]     = "END";
code char LTCHourTxt[]      = "HOUR";
code char ConfirmTxt[]    = "CONFIRM";
code char LTCNSTxt[]    = "ns";
code char LTCDSTTxt[]   = "DST:";

code char AESEBUBNC1Txt[]   = "AES BNC #1";
code char AESEBUBNC2Txt[]   = "AES BNC #2";
code char AESEBUXLR1Txt[]   = "AES XLR #1";
code char AESEBUXLR2Txt[]   = "AES XLR #2";
code char AESEBUWord1Txt[]  = "AES WORD #1";
code char AESEBUWord2Txt[]  = "AES WORD #2";

code char BlkBurstTxt[]     = "BLACK-BURST";
code char AESEBUTxt[]       = "AES-EBU";
code char GenlockTxt[]      = "GENLOCK";
code char LTCTxt[]        = "LTC";
code char PresetTxt[]       = "PRESET";
code char TestTxt[]         = "TEST";
code char EMBAudioTxt[]     = "EMB.AUDIO";

code char ConfigDateTimeTxt[] = "DATE-TIME";
code char ConfigLockTxt[]     = "LOCK";
code char ConfigAutoESCTxt[]  = "AUTO-ESC";
code char ConfigLCDContTxt[]  = "LCD-CONTRAST";
code char ConfigDownloadTxt[] = "DOWNLOAD";
code char ConfigDiagnoseTxt[] = "DIAGNOSE";
code char ConfigUpdateTxt[]   = "UPDATE";

code char TimeTxt[]           = "TIME";
code char DateTxt[]           = "DATE";
code char TimeDateTxt[]       = "TIME+DATE";
code char ReferenceTxt[]      = "REFERENCE";
code char OffsetTxt[]         = "OFFSET";

code char RS232BaudrateTxt[]  = "BAUD-RATE";
code char RS232DatabitTxt[]   = "DATA-BIT";
code char RS232ParityTxt[]    = "PARITY";
code char RS232HandshakeTxt[] = "HANDSHAKE";

code char MainTxt[]           = "Main";
code char OptionsTxt[]        = "Options";
code char MemoryTxt[]         = "Memory";
code char DisplayTxt[]        = "Display";
code char KeyboardSLTxt[]     = "Keyboard";
code char VersionTxt[]        = "Version";
code char ConfigurationTxt[]  = "Configuration";
code char ErrorQueueTxt[]     = "ErrorQueue";

code char GeneralFailureTxt[]        = "General failure: ";
code char NoContactToTxt[]           = "No contact to ";
code char ErrorWritingToTxt[]        = "Error writing to ";
code char NoResponseFromTxt[]        = "No response from ";
code char ErrorReadingFromTxt[]      = "Error reading from ";
code char ErrorDownloadingFromTxt[]  = "Error downloading from ";
code char ChecksumErrorTxt[]         = "Checksum error: ";
code char NoGPSLockTxt[]   		     = "No GPS lock";
code char LTCDaylightSwitchedTxt[]   = "Daylight switched ";
code char LTCResyncedTxt[]  	     = "Resynced ";

code char MultipleFailureTxt[]       = "MULTIPLE FAILURE";
code char PowerFailureTxt[]          = "POWER FAILURE: ";
code char TemperatureFailureTxt[]    = "TEMPERATURE FAILURE";
                                    
code char NoWarningsTxt[]            = "No active warning";
code char NoErrorDetectedTxt[]       = "No error detected";
code char TemperatureWarningTxt[]    = "Temperature warning";
code char LevelDetectorTxt[]         = "Level error at ";

code char InternalErrorTxt[]         = "Unstable signal at ";
code char ConfigurationErrorTxt[]    = "Configuration error";
code char MultipleErrorTxt[]         = "Multiple errors detected";

code char TestingTxt[]                  = "Testing ";
code char TestingMainboardTxt[]         = "Testing main board";
code char TestingBlackburstTxt[]        = "Testing black burst unit";
code char TestingSPGBoardTxt[]          = "Testing oscillator board";
code char TestingRS232CommTxt[]         = "Testing RS232 communication";
code char TestingMainCompletedTxt[]     = "Testing main completed";
code char TestingOptionsCompletedTxt[]  = "Testing options completed";
code char TestingMemoryCompletedTxt[]   = "Testing memory completed";
code char TestingButtonsCompletedTxt[]  = "Testing buttons completed";

code char NoOptionsDetectedTxt[]    = "No options have been detected!!";
code char LoopbackConnectorTxt[]    = "Please insert a loopback connector!!";

code char ResetErrorQueueTxt[]      = "Reset ErrorQueue ?";
code char RS232TestTxt[]            = "RS232TEST";

code char VoltageN5Txt[]            = "-5 VOLT";
code char Voltage12Txt[]            = "12 VOLT";

code char SDIGenlockTxt[]           = "SDI GENLOCK";
code char GPSGenlockTxt[]           = "GPS GENLOCK";
code char TimeCodeTxt[]             = "TIME CODE";
code char ROMMemoryTxt[]            = "ROM MEMORY";
code char RAMMemoryTxt[]            = "RAM MEMORY";
code char KeyboardTxt[]             = "KEYBOARD";
code char LCDADConvTxt[]            = "LCD ADC";
code char TEMPADConvTxt[]           = "TEMP ADC";
code char VoltageTxt[]              = "POWER ADC";
code char LevelDetTxt[]             = "LEVEL DETECTOR";
code char UnknownOptionTxt[]        = "UNKNOWN OPTION";

code char GENLOCKEDTxt[]            = "GENLOCKED";
code char SYNCLOCKEDTxt[]           = "SYNCLOCKED";
code char UNLOCKEDTxt[]             = "UNLOCKED";
code char NOGENLOCKTxt[]            = "NO GENLOCK";
code char NOSIGNALTxt[]             = "NO SIGNAL";
code char PRESENTTxt[]              = "PRESENT";
code char TOOSLOWTxt[]              = "TOO SLOW";
code char TOOFASTTxt[]              = "TOO FAST";
code char NOVITCTxt[]               = "NO VITC";
code char VITCPALTxt[]              = "VITC(PAL)";
code char VITCNTSCTxt[]             = "VITC(NTSC)";
code char NOLTCTxt[]                = "NO LTC";
code char LTCPALTxt[]               = "LTC(PAL)";
code char LTCNTSCTxt[]              = "LTC(NTSC)";

code char OnTxt[]                   = "On";
code char OffTxt[]                  = "Off";
code char MinusTxt[]                = "-";
code char PlusTxt[]                 = "+";

code char UPButtonTxt[]             = "UP ";
code char DOWNButtonTxt[]           = "DOWN ";
code char LEFTButtonTxt[]           = "LEFT ";
code char RIGHTButtonTxt[]          = "RIGHT ";
code char EXECUTEButtonTxt[]        = "EXECUTE ";
code char CBARButtonTxt[]           = "C.BAR ";
code char MBURSTButtonTxt[]         = "M.BURST ";
code char WINDOWButtonTxt[]         = "WINDOW ";
code char PRESETButtonTxt[]         = "PRESET ";
code char SPECIALButtonTxt[]        = "SPECIAL ";
code char LINEARITYButtonTxt[]      = "LINEARITY ";
code char PATTERNButtonTxt[]        = "PATTERN ";
code char OUTPUTButtonTxt[]         = "OUTPUT ";
code char GENLOCKButtonTxt[]        = "GENLOCK";

code char BB1Txt[]              = "BB1";
code char BB2Txt[]              = "BB2";
code char BB3Txt[]              = "BB3";
code char BB4Txt[]              = "BB4";
code char BB5Txt[]              = "BB5";
code char BB6Txt[]              = "BB6";
code char BB7Txt[]              = "BB7";
code char BB8Txt[]              = "BB8";
code char BBMulTxt[]            = "BB2+MUL";

code char AESEBU1Txt[]          = "AES-EBU1";
code char AESEBU2Txt[]          = "AES-EBU2";

code char NTSCTxt[]             = "NTSC";
code char NTSCwoTxt[]           = "NTSC wo. setup";
code char PALTxt[]              = "PAL";
code char PAL_PALIDTxt[]        = "PAL w/PAL ID";

code char SDI525Txt[]           = "525/59.94";
code char SDI625Txt[]           = "625/50";

code char SMPTECBarTxt[]        = "SMPTE C.Bar";
code char EBUCBarTxt[]          = "EBU C.Bar";
code char FCCBarTxt[]           = "FCC C.Bar";
code char EBUCBarITUTxt[]       = "C.Bar ITU801";
code char CBar100Txt[]          = "100% C.Bar";
code char CBarGrey75Txt[]       = "75% C.Bar+Grey";
code char CBarRed75Txt[]        = "75% C.Bar+Red";
code char Red75Txt[]            = "75% Red";

code char MultiburstTxt[]       = "Multiburst";
code char LuminanceSweepTxt[]   = "Lum. Sweep";
code char YCrCbSweepTxt[]       = "Y, Cr, Cb Sweep";
code char MultipulseTxt[]       = "Multipulse";
code char SinxxTxt[]            = "Sinx/x";
code char CCIR18Txt[]           = "CCIR Line18";
code char NTC7CombTxt[]         = "NTC7 Combination";
code char FCCMultiburstTxt[]    = "FCC Multiburst";

code char Window10Txt[]         = "Window 10%";
code char Window15Txt[]         = "Window 15%";
code char Window20Txt[]         = "Window 20%";
code char Window100Txt[]        = "Window 100%";
code char BlWh15kHzTxt[]        = "15kHz Bl/Wh";
code char Grey50Txt[]           = "Grey 50%";
code char White100Txt[]         = "White 100%";
code char BlackTxt[]            = "Black";
code char BlackBurstTxt[]       = "Black Burst";

code char SDICheckFieldTxt[]     = "SDI Check Field";
code char DigitalTimingTxt[]     = "Digital Timing";
code char FieldDelayTestTxt[]    = "Field Delay Test";
code char BowTieTxt[]            = "Bow Tie";
code char AnalogBlankingTxt[]    = "Analog Blanking";
code char DigitalGreyTxt[]       = "Digital Grey";
code char FieldSquareWaveTxt[]   = "Field sq.wave";
code char BlWh01HzTxt[]          = "0.1Hz Bl/Wh";
code char EOLTxt[]               = "End of line";
code char WhiteEOLTxt[]          = "White end";
code char BlueEOLTxt[]           = "Blue end of line";
code char RedEOLTxt[]            = "Red end of line";
code char YellowEOLTxt[]         = "Yell end of line";
code char CyanEOLTxt[]           = "Cyan end of line";
code char Custom1Txt[]           = "Customized 1";
code char Custom2Txt[]           = "Customized 2";
code char Custom3Txt[]           = "Customized 3";
code char Custom4Txt[]           = "Customized 4";
code char Custom5Txt[]           = "Customized 5";
code char Custom6Txt[]           = "Customized 6";
code char Custom7Txt[]           = "Customized 7";
code char Custom8Txt[]           = "Customized 8";

code char ShallowRampTxt[]       = "Shallow Ramp";
code char RampTxt[]              = "Ramp";
code char LimitRampTxt[]         = "Limit Ramp";
code char ValidRampTxt[]         = "Valid Ramp";
code char ModulatedRampTxt[]     = "Ramp Modulated";
code char Stair5Txt[]            = "Staircase 5step";
code char ModulatedStair5Txt[]   = "Staircase Modul.";
code char Stair10Txt[]           = "Staircase 10step";
code char PulseBarTxt[]          = "Pulse & Bar";
code char CCIR17Txt[]            = "CCIR Line17";
code char CCIR330Txt[]           = "CCIR Line330";
code char CCIR331Txt[]           = "CCIR Line331";
code char FCCCompositeTxt[]      = "FCC Composite";
code char NTC7CompositeTxt[]     = "NTC7 Composite";
code char YellowGreyRampTxt[]    = "Ramp Yellow/Grey";
code char GreyBlueRampTxt[]      = "Ramp Grey/Blue";
code char CyanGreyRampTxt[]      = "Ramp Cyan/Grey";
code char GreyRedRampTxt[]       = "Ramp Grey/Red";
code char CbYCrYRampTxt[]        = "Ramp Cb,Y,Cr,Y";

code char Philips4x3Txt[]        = "PHILIPS 4:3";
code char Philips16x9Txt[]       = "PHILIPS 16:9";
code char FuBK4x3Txt[]           = "FuBK 4:3";
code char FuBK16x9Txt[]          = "FuBK 16:9";
code char CrosshatchTxt[]        = "Crosshatch";
code char Crosshatch16x9Txt[]    = "Crosshatch 16:9";
code char WhiteCircle4x3Txt[]    = "Circle 4:3";
code char WhiteCircle16x9Txt[]   = "Circle 16:9";
code char PLUGETxt[]             = "PLUGE";
code char SafeAreaTxt[]          = "Safe Area";
code char kHz250Txt[]            = "Sq.wave 250kHz";
code char VMT01Txt[]             = "VMT 01";

code char EMBAudioGroup1Txt[]    = "1, Chan 1-4";
code char EMBAudioGroup2Txt[]    = "2, Chan 5-8";
code char EMBAudioGroup3Txt[]    = "3, Chan 9-12";
code char EMBAudioGroup4Txt[]    = "4, Chan 13-16";

code char Line1Txt[]             = "LINE1";
code char Line2Txt[]             = "LINE2";
code char Line3Txt[]             = "LINE3";

code char Li1Txt[]                = "L1";
code char Li2Txt[]                = "L2";
code char Li3Txt[]                = "L3";

code char LineUpperTxt[]         = "UPPER";
code char LineLowerTxt[]         = "LOWER";
code char LineLeftTxt[]          = "LEFT";
code char LineRightTxt[]         = "RIGHT";

code char FreeTxt[]              = "Free";
code char StandardTxt[]          = "Standard";
code char ComplexTxt[]           = "Complex";

code char Stereo800HzTxt[]       = "Stereo 800 Hz";
code char Stereo1kHzTxt[]        = "Stereo 1 kHz";
code char StereoEBU1kHzTxt[]     = "Stereo EBU 1 kHz";
code char StereoBBC1kHzTxt[]     = "Stereo BBC 1 kHz";
code char MonoEBU1kHzTxt[]       = "Mono EBU 1 kHz";
code char Mono1kHzTxt[]          = "Mono 1 kHz";
code char DualTxt[]              = "Dual 1kHz 400Hz";
code char WordClockTxt[]         = "Word-clock";

code char SilenceTxt[]           = "Silence";
code char Level0dBTxt[]          = "  0 dBFS";
code char Level9dBTxt[]          = " -9 dBFS";
code char Level12dBTxt[]         = "-12 dBFS";
code char Level14dBTxt[]         = "-14 dBFS";
code char Level15dBTxt[]         = "-15 dBFS";
code char Level16dBTxt[]         = "-16 dBFS";
code char Level18dBTxt[]         = "-18 dBFS";
code char Level20dBTxt[]         = "-20 dBFS";

code char NTSCPhase1Txt[]        = "NTSC Phase 1";
code char NTSCPhase2Txt[]        = "NTSC Phase 2";
code char NTSCPhase3Txt[]        = "NTSC Phase 3";
code char NTSCPhase4Txt[]        = "NTSC Phase 4";
code char NTSCPhase5Txt[]        = "NTSC Phase 5";

code char GenlockATxt[]             = "A";
code char GenlockBTxt[]             = "B";
code char LoopThroughTxt[]          = "A-B";
code char SDITxt[]                  = "SDI";
code char GPSTxt[]                  = "GPS";
code char InternalGenlockTxt[]      = "Internal";
code char InternalOCXOGenlockTxt[]  = "Internal 2";

code char PALBurstTxt[]        = "PAL Burst";
code char NTSCBurstTxt[]       = "NTSC Burst";
code char Sync625Txt[]         = "625 Sync";
code char Sync525Txt[]         = "525 Sync";
code char MHz358Txt[]          = "3.58 MHz";
code char MHz443Txt[]          = "4.43 MHz";
code char MHz5Txt[]            = "5 MHz";
code char MHz10Txt[]           = "10 MHz";

code char LTCA[]         = "LTC A";
code char LTCB[]         = "LTC B";

code char LTC24Txt[]      = "24.00  FPS";
code char LTC25Txt[]      = "25.00  FPS";
code char LTC29Txt[]      = "29.97 DROP";
code char LTC29DFTxt[]      = "29.97 NOND";
code char LTC30Txt[]      = "30.00  FPS";

code char LTCNoneTxt[]      = "NONE";
code char LTCConfirmTxt[]   = "CONF";
code char LTCAutoTxt[]      = "AUTO";


code char NormalTxt[]          = "Normal";
code char PanelTxt[]           = "Panel";
code char DateTimeTxt[]        = "Date-Time";
code char DownloadTxt[]        = "Download";
code char DiagnoseTxt[]        = "Diagnose";

code char Hour12Txt[]          = "12h";
code char Hour24Txt[]          = "24h";

code char DMYTxt[]             = "DD-MM-YY";
code char MDYTxt[]             = "MM-DD-YY";
code char YMDTxt[]             = "YY-MM-DD";

code char LTCInputTxt[]              = "LTC-input";
code char VITCInputTxt[]             = "VITC on genlock";
code char VideoFieldFreqInputTxt[]   = "Video Field Freq";
code char Reference1HzTxt[]          = "1 Hz Reference";
code char InternalTxt[]              = "Internal";

code char PresetNoTxt[]              = "Preset #";
code char AllPresetTxt[]             = "All Presets";
code char CompleteSPGTxt[]           = "Complete SPG";

code char AttribAntiPALTxt[]         = "Anti-PAL";
code char AttribPLUGETxt[]           = "PLUGE";
code char AttribStair10Txt[]         = "Staircase 10";
code char AttribMotionTxt[]          = "Motion";
code char AttribCirclesTxt[]         = "Circles";
code char AttribLogoTxt[]            = "Logo";


code char N300Txt[]              = "300";
code char N600Txt[]              = "600";
code char N1200Txt[]             = "1200";
code char N2400Txt[]             = "2400";
code char N4800Txt[]             = "4800";
code char N9600Txt[]             = "9600";
code char N19200Txt[]            = "19200";

code char NoneTxt[]              = "None";
code char OddTxt[]               = "Odd";
code char EvenTxt[]              = "Even";

code char NoHandshakeTxt[]       = "None";
code char XONXOFFTxt[]           = "XON/XOFF";
code char RTSCTSTxt[]            = "RTS/CTS";

code char MainConnTxt[]          = "[ MAIN]: ";
code char OSCConnTxt[]           = "[  OSC]: ";
code char BBConnTxt[]            = "[ BB  ]: ";
code char ATPGConnTxt[]          = "[ATPG ]: ";
code char STSGConnTxt[]          = "[STSG ]: ";
code char STPGConnTxt[]          = "[STPG ]: ";
code char SDIGenlockConnTxt[]    = "[ SDIG]: ";
code char GPSGenlockConnTxt[]    = "[ GPSG]: ";
code char Int2GenlockConnTxt[]   = "[10MHz]: ";
code char AESEBUConnTxt[]        = "[  AES]: ";
code char TimeConnTxt[]          = "[ TIME]: ";
code char TLGConnTxt1[]          = "[ TLS1]: ";
code char TLGConnTxt2[]          = "[ TLS2]: ";
code char TLGConnTxt3[]          = "[ TLS3]: ";


/****************************************************/


code char* AnlTPGHdrArrTxt[]    = { AnlTPG2HdrTxt,
                             AnlTPG5HdrTxt };

code char* SDITSGHdrArrTxt[]    = { SDITSG2HdrTxt,
                             SDITSG3HdrTxt,
                             SDITSG4HdrTxt };

code char* SDITPGHdrArrTxt[]    = { SDITPG1HdrTxt,
                             SDITPG2HdrTxt,
                             SDITPG5HdrTxt };

code char* AESEBUHdrArrTxt[]    = { AESEBU1HdrTxt,
                             AESEBU2HdrTxt };

code char* ButtonArrTxt[] = 
  {
  LEFTButtonTxt,
  DOWNButtonTxt,
  RIGHTButtonTxt,
  EXECUTEButtonTxt,
  PRESETButtonTxt,
  OUTPUTButtonTxt,
  GENLOCKButtonTxt
#if 0
  CBARButtonTxt,
  MBURSTButtonTxt,
  WINDOWButtonTxt,
  SPECIALButtonTxt,
  LINEARITYButtonTxt,
  PATTERNButtonTxt,
#endif 
  };

code char* BBArrTxt[]          = { BB1Txt,
                             BB2Txt,
                             BB3Txt,
                             BB4Txt,
                             BB5Txt,
                             BB6Txt,
                             BB7Txt,
                             BB8Txt };

code char* AnlTPGArrTxt[]      = { AnlTPG2Txt,
                             AnlTPG5Txt };

code char* SDITSGArrTxt[]      = { SDITSG2Txt,
                             SDITSG3Txt,
                             SDITSG4Txt };

code char* SDITPGArrTxt[] =
  {
  SDITPG1Txt,
  SDITPG2Txt,
  SDITPG5Txt
  };

code char* AnlSystemArrTxt[]    = { PALTxt,
                             PAL_PALIDTxt,
                             NTSCTxt,
                             NTSCwoTxt };

code char* SDISystemArrTxt[]    =
  {
 SDI625Txt,
 SDI525Txt
 };

code char* AnlPattArrTxt[]      = { SMPTECBarTxt,
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

code char* SDIPattArrTxt[] = 
  {
  SMPTECBarTxt,
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

code char* AttribArrTxt[] = 
  { 
  AttribAntiPALTxt,
  AttribPLUGETxt,
  AttribStair10Txt,
  AttribMotionTxt,
  AttribCirclesTxt,
  AttribLogoTxt
  };

code char* TSGAudioSignalArrTxt[]  = { OffTxt,
                             Stereo1kHzTxt };

code char* TSGAudioLevelArrTxt[]  = { SilenceTxt,
                             Level0dBTxt,
                             Level9dBTxt,
                             Level15dBTxt,
                             Level18dBTxt };

code char* TPGAudioSignalArrTxt[]  = { OffTxt,
                             Stereo800HzTxt,
                             Stereo1kHzTxt,
                             StereoEBU1kHzTxt,
                             StereoBBC1kHzTxt,
                             MonoEBU1kHzTxt,
                             Mono1kHzTxt,
                             DualTxt };

code char* TPGAudioLevelArrTxt[]  = { SilenceTxt,
                             Level0dBTxt,
                             Level9dBTxt,
                             Level12dBTxt,
                             Level14dBTxt,
                             Level15dBTxt,
                             Level16dBTxt,
                             Level18dBTxt,
                             Level20dBTxt };

code char* TPGAudioGroupArrTxt[]  = { EMBAudioGroup1Txt,
                             EMBAudioGroup2Txt,
                             EMBAudioGroup3Txt,
                             EMBAudioGroup4Txt };

code char* TPGTextEditArrTxt[]    = { Line1Txt,
                             Line2Txt,
                             Line3Txt };



code char* code PT8603TextEditArrTxt[] = 
  {
  Li1Txt,
  Li2Txt,
  Li3Txt
  };


code char* TPGTextStyleArrTxt[]  = { FreeTxt,
                             StandardTxt,
                             ComplexTxt };

code char* TPGTextClockArrTxt[]  = { NoneTxt,
                             TimeTxt,
                             DateTxt,
                             TimeDateTxt };

code char* AESEBUSignalArrTxt[]  = { OffTxt,
                             Stereo800HzTxt,
                             Stereo1kHzTxt,
                             StereoEBU1kHzTxt,
                             StereoBBC1kHzTxt,
                             MonoEBU1kHzTxt,
                             Mono1kHzTxt,
                             DualTxt,
                             WordClockTxt };

code char* AESEBULevelArrTxt[]    = { SilenceTxt,
                             Level0dBTxt,
                             Level9dBTxt,
                             Level12dBTxt,
                             Level14dBTxt,
                             Level15dBTxt,
                             Level16dBTxt,
                             Level18dBTxt,
                             Level20dBTxt };

code char* AESEBUTimingArrTxt[]  = { PALTxt,
                             NTSCPhase1Txt,
                             NTSCPhase2Txt,
                             NTSCPhase3Txt,
                             NTSCPhase4Txt,
                             NTSCPhase5Txt };

code char* GenlockInputArrTxt[]  = { GenlockATxt,
                             GenlockBTxt,
                             LoopThroughTxt,
                             SDITxt,
                             GPSTxt,
                             InternalGenlockTxt,
                             InternalOCXOGenlockTxt };

code char* GenlockSignalArrTxt[]  =
  {
  PALBurstTxt,
  NTSCBurstTxt,
  Sync625Txt,
  Sync525Txt,
  SDI625Txt,
  SDI525Txt,
  MHz358Txt,
  MHz443Txt,
  MHz5Txt,
  MHz10Txt
  };
                    
code char* LTCArrTxt[] =
  {
  LTCA,
  LTCB
  };
  
code char* LTCFormatArrTxt[] = { LTC24Txt, LTC25Txt, LTC29Txt, LTC29DFTxt, LTC30Txt };

code char* LTCModeArrTxt[] = {LTCNoneTxt, LTCConfirmTxt, LTCAutoTxt };

code char* LockArrTxt[]        = { NormalTxt,
                             PanelTxt,
                             DateTimeTxt,
                             DownloadTxt,
                             DiagnoseTxt };

code char* OffOnArrTxt[] = 
  {
  OffTxt,
  OnTxt
  };

code char* MinusPlusArrTxt[]    = { MinusTxt,
                             PlusTxt };

code char* TimeFormatArrTxt[]    = { Hour12Txt,
                             Hour24Txt };

code char* DateFormatArrTxt[]    = { DMYTxt,
                             MDYTxt,
                             YMDTxt };

code char* DateTimeRefArrTxt[]    = { VITCInputTxt,
                             LTCInputTxt,
                             Reference1HzTxt,
                             VideoFieldFreqInputTxt };

code char* DownloadArrTxt[]      = { PresetNoTxt,
                             AllPresetTxt,
                             CompleteSPGTxt };

code char* RS232BaudrateArrTxt[]  = { N300Txt,
                             N600Txt,
                             N1200Txt,
                             N2400Txt,
                             N4800Txt,
                             N9600Txt,
                             N19200Txt };

code char* RS232ParityArrTxt[]    = { NoneTxt,
                             OddTxt,
                             EvenTxt };

code char* RS232HandshakeArrTxt[]  = { NoHandshakeTxt,
                             XONXOFFTxt,
                             RTSCTSTxt };


code char* TRIArrTxt[] =  /* JK addition */
  {
  TRI1Txt,
  TRI2Txt,
  TRI3Txt
  };




