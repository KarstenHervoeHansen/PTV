/* MODULE:                                                                  */
/*   r232cmd.c - RS232 remote commands.                                   */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/* TASKS:                                                                   */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/
/*
 *   PTV software for PT5201
 *   Copyright (c) 
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

/****************************************************************************/
/* Include files:                                           */

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "config.h"
#include "define.h"
#include "rtxcobj.h"
#include "rs232par.h"
#include "rs232err.h"
#include "rs232cmd.h"
#include "util.h"
#include "rs232.h"
#include "i2c_drv.h"
#include "sio0drv.h"
#include "instru.h"
#include "tables.h"
#include "genlock.h"
#include "bb.h"
#include "cbar.h"
#include "audio.h"
#include "plddrv.h"
#include "unitprg.h"


UC code MasterSWVersion           = 23;

char code SystemVersionTxt[]       = "1995.0";
char code PasswordText[]         = "PASS5201";

char code CLSCmdTxt[]             = "CLS";
char code ESECmdTxt[]             = "ESE";
char code ESRCmdTxt[]             = "ESR";
char code OPCCmdTxt[]             = "OPC";
char code SRECmdTxt[]             = "SRE";
char code STBCmdTxt[]             = "STB";
char code TSTCmdTxt[]             = "TST";
char code WAICmdTxt[]             = "WAI";
char code RSTCmdTxt[]             = "RST";
char code IDNCmdTxt[]             = "IDN";
char code RCLCmdTxt[]             = "RCL";
char code SAVCmdTxt[]             = "SAV";

char code SYSTEMCmdTxt[]            = "SYSTEM";
char code SYSTCmdTxt[]              = "SYST";
char code STATUSCmdTxt[]           = "STATUS";
char code STATCmdTxt[]              = "STAT";
char code DIAGNOSTICCmdTxt[]      = "DIAGNOSTIC";
char code DIAGCmdTxt[]             = "DIAG";
char code MONITORCmdTxt[]          = "MONITOR";
char code MONCmdTxt[]             = "MON";
char code INPUTCmdTxt[]            = "INPUT";
char code INPCmdTxt[]              = "INP";
char code OUTPUTCmdTxt[]           = "OUTPUT";
char code OUTPCmdTxt[]             = "OUTP";
char code USERCmdTxt[]            = "USER";
char code MEASURECmdTxt[]          = "MEASURE";
char code MEASCmdTxt[]             = "MEAS";
char code FACTORYCmdTxt[]          = "FACTORY";
char code FACTCmdTxt[]             = "FACT";

char code ERRORCmdTxt[]            = "ERROR";
char code ERRCmdTxt[]              = "ERR";
char code VERSIONCmdTxt[]           = "VERSION";
char code VERSCmdTxt[]             = "VERS";
char code PRESETCmdTxt[]         = "PRESET";
char code PRESCmdTxt[]           = "PRES";
char code EVENTLOGCmdTxt[]        = "EVENTLOG";
char code DATECmdTxt[]          = "DATE";
char code TIMECmdTxt[]          = "TIME";

char code RECALLCmdTxt[]         = "RECALL";
char code RECCmdTxt[]           = "REC";
char code STORECmdTxt[]         = "STORE";
char code STORCmdTxt[]           = "STOR";
char code NAMECmdTxt[]           = "NAME";
char code AUTHORCmdTxt[]         = "AUTHOR";
char code AUTHCmdTxt[]           = "AUTH";
char code DOWNLOADCmdTxt[]       = "DOWNLOAD";
char code DOWNCmdTxt[]           = "DOWN";
char code UPLOADCmdTxt[]         = "UPLOAd";
char code UPLCmdTxt[]           = "UPL";

char code OPERATIONCmdTxt[]       = "OPERATION";
char code OPERCmdTxt[]             = "OPER";
char code QUESTIONABLECmdTxt[]     = "QUESTIONABLE";
char code QUESCmdTxt[]           = "QUES";

char code EVENTCmdTxt[]         = "EVENT";
char code EVENCmdTxt[]          = "EVEN";
char code CONDITIONCmdTxt[]       = "CONDITION";
char code CONDCmdTxt[]           = "COND";
char code ENABLECmdTxt[]         = "ENABLE";
char code ENABCmdTxt[]           = "ENAB";

char code DISPLAYCmdTxt[]         = "DISPLAY";
char code DISPCmdTxt[]           = "DISP";
char code ERRORQUEUECmdTxt[]      = "ERRORQUEUE";
char code RESETCmdTxt[]         = "RESET";
char code RESCmdTxt[]           = "RES";

char code DEFAULTCmdTxt[]          = "DEFAULT";
char code DEFCmdTxt[]           = "DEF";

char code GENLOCKCmdTxt[]         = "GENLOCK";
char code GENLCmdTxt[]           = "GENL";

char code DELAYCmdTxt[]         = "DELAY";
char code DELCmdTxt[]           = "DEL";

char code CALIBRATECmdTxt[]        = "CALIBRATE";
char code CALCmdTxt[]           = "CAL";

char code ADJUSTCmdTxt[]         = "ADJUST";
char code ADJCmdTxt[]           = "ADJ";

char code BBCmdTxt[]           = "BB";
char code TSGENERATORCmdTxt[]     = "TSGENERATOR";
char code TSGCmdTxt[]           = "TSG";
char code AUDIOCmdTxt[]         = "AUDIO";
char code AUDCmdTxt[]           = "AUD";

char code ANALOGCmdTxt[]        = "ANALOG";
char code ANALCmdTxt[]          = "ANAL";

char code PATTERNCmdTxt[]         = "PATTERN";
char code PATTCmdTxt[]           = "PATT";
char code EMBAUDIOCmdTxt[]       = "EMBAUDIO";
char code EMBCmdTxt[]           = "EMB";

char code AESEBUCmdTxt[]        = "AESEBU";
char code AESCmdTxt[]          = "AES";

char code SCHPHASECmdTxt[]       = "SCHPHASE";
char code SCHPCmdTxt[]           = "SCHP";

char code SIGNALCmdTxt[]         = "SIGNAL";
char code SIGNCmdTxt[]           = "SIGN";
char code LEVELCmdTxt[]         = "LEVEL";
char code LEVCmdTxt[]           = "LEV";
char code EDHINSERTCmdTxt[]       = "EDHINSERT";
char code EDHCmdTxt[]           = "EDH";

char code TIMINGCmdTxt[]         = "TIMING";
char code TIMCmdTxt[]           = "TIM";

char code WORDCLOCKCmdTxt[]       = "WORDCLOCK";
char code WORDCmdTxt[]           = "WORD";

char code CLICKCmdTxt[]         = "CLICK";
char code CLICCmdTxt[]           = "CLIC";

char code PASSWORDCmdTxt[]       = "PASSWORD";
char code PASSCmdTxt[]           = "PASS";
char code V24CIRCUITCmdTxt[]       = "V24CIRCUIT";
char code V24CCmdTxt[]           = "V24C";
char code CHECKSUMCmdTxt[]        = "CHECKSUM";
char code CHECCmdTxt[]            = "CHEC";

char code CODECmdTxt[]            = "CODE";
char code RANGECmdTxt[]          = "RANGE";
char code RANGCmdTxt[]            = "RANG";
char code COMMANDCmdTxt[]         = "COMMAND";
char code COMMCmdTxt[]           = "COMM";
char code TRANSPARENTCmdTxt[]      = "TRANSPARENT";
char code TRANCmdTxt[]            = "TRAN";

char code MAINBOARDCmdTxt[]       = "MAINBOARD";
char code MAINCmdTxt[]           = "MAIN";
char code COMPANYCmdTxt[]         = "COMPANY";
char code COMPCmdTxt[]           = "COMP";
char code TYPECmdTxt[]           = "TYPE";
char code KUNUMBERCmdTxt[]       = "KUNUMBER";
char code KUNCmdTxt[]           = "KUN";
char code SWREVISIONCmdTxt[]       = "SWREVISION";
char code SWRCmdTxt[]           = "SWR";

char code NCCmdTxt[]           = "NC";
char code FMSDATECmdTxt[]         = "FMSDATE";
char code FMSDCmdTxt[]           = "FMSD";
char code PRODUCTIONCmdTxt[]       = "PRODUCTION";
char code PRODCmdTxt[]           = "PROD";
char code UPDATECmdTxt[]         = "UPDATE";
char code UPDCmdTxt[]           = "UPD";

char code FREFERENCECmdTxt[]        = "FREFERENCE";
char code FREFCmdTxt[]           = "FREF";
char code DACCmdTxt[]           = "DAC";
char code UREFERENCECmdTxt[]       = "UREFERENCE";
char code UREFCmdTxt[]           = "UREF";
char code HPHZEROCmdTxt[]         = "HPHZERO";
char code HPHZCmdTxt[]           = "HPHZ";
char code PHASECmdTxt[]         = "PHASE";
char code PHASCmdTxt[]           = "PHAS";

char code AGAINCmdTxt[]         = "AGAIN";
char code AGACmdTxt[]           = "AGA";
char code CGAINCmdTxt[]         = "CGAIN";
char code CGACmdTxt[]           = "CGA";
char code ALEVELCmdTxt[]         = "ALEVEL";
char code ALEVCmdTxt[]           = "ALEV";

char code PLDCmdTxt[]           = "PLD";
char code TCXOCmdTxt[]           = "TCXO";
char code MMCHIPCmdTxt[]         = "MMCHIP";
char code MMCHCmdTxt[]           = "MMCH";

char code SAVEParTxt[]           = "SAVE";

char code INTERNALParTxt[]       = "INTERNAL";
char code INTParTxt[]           = "INT";
char code PALBURSTParTxt[]       = "PALBURST";
char code PALBParTxt[]           = "PALB";
char code NTSCBURSTParTxt[]       = "NTSCBURST";
char code NTSCParTxt[]           = "NTSC";

char code F10MHzParTxt[]         = "F10MHZ";

char code PALParTxt[]           = "PAL";
char code PAL_IDParTxt[]         = "PAL_ID";

char code CBSMPTEParTxt[]        = "CBSMPTE";      // SMPTE Colorbar
char code CBSMParTxt[]           = "CBSM";          
char code CBEBUParTxt[]         = "CBEBU";          // EBU Colorbar
char code CBEBParTxt[]           = "CBEB";
char code CBFCCParTxt[]           = "CBFCC";        // FCC Colorbar
char code CBFCParTxt[]             = "CBFC";
char code CBEBU8ParTxt[]           = "CBEBU8";        // Colorbar ITU801
char code CBEB8ParTxt[]           = "CBEB8";
char code CB100ParTxt[]             = "CB100";        // 100% Colorbar
char code CBGREY75ParTxt[]         = "CBGREY75";      // Colorbar + 75% Grey
char code CBGR75ParTxt[]          = "CBGR75";
char code CBRED75ParTxt[]          = "CBRED75";      // Colorbar + 75% Red
char code CBR75ParTxt[]           = "CBR75";
char code RED75ParTxt[]           = "RED75";        // 75% Red

char code MULTIBURSTParTxt[]       = "MULTIBURST";    // Multi burst
char code MULTParTxt[]            = "MULT";
char code CCIR18ParTxt[]           = "CCIR18";
char code WIN10ParTxt[]          = "WIN10";        // Window 10%
char code WIN15ParTxt[]          = "WIN15";        // Window 15%
char code WIN20ParTxt[]          = "WIN20";        // Window 20%
char code WIN100ParTxt[]          = "WIN100";        // Window 100%
char code BLWH15ParTxt[]          = "BLWH15KHZ";      // 15kHz Bl/Wh
char code WHITE100ParTxt[]        = "WHITE100";      // White 100%
char code WHIT100ParTxt[]         = "WHIT100";
char code BLACKParTxt[]           = "BLACK";        // Black
char code BLACParTxt[]             = "BLAC";

char code SDICHECKParTxt[]         = "SDICHECK";      // Check field
char code SDICParTxt[]             = "SDIC";
char code DGREYParTxt[]           = "DGREY";        // Digital grey
char code DGRParTxt[]             = "DGR";
char code STAIRCASE5ParTxt[]      = "STAIRCASE5";     // 5-step staircase
char code STA5ParTxt[]             = "STA5";
char code STAIRCASE10ParTxt[]     = "STAIRCASE10";    // 10-step staircase
char code STA10ParTxt[]           = "STA10";
char code CROSSHATCHParTxt[]       = "CROSSHATCH";    // Cross hatch
char code CROSParTxt[]             = "CROS";
char code PLUGEParTxt[]           = "PLUGE";        // PLUGE
char code PLUGParTxt[]             = "PLUG";

char code ANALOGParTxt[]          = "ANALOG";
char code ANALParTxt[]            = "ANAL";
char code AESEBUParTxt[]          = "AESEBU";
char code AESParTxt[]            = "AES";

char code F441KHZParTxt[]         = "F441KHZ";
char code F48KHZParTxt[]         = "F48KHZ";
char code F45MHZParTxt[]         = "F45MHZ";

char code S500HZParTxt[]         = "S500HZ";        // Stereo 500 Hz
char code S1KHZParTxt[]         = "S1KHZ";         // Stereo 1 kHz
char code SEBU1KHZParTxt[]         = "SEBU1KHZ";      // Stereo EBU 1 kHz
char code S8KHZParTxt[]         = "S8KHZ";        // Stereo 8 kHz

char code SILENCEParTxt[]         = "SILENCE";
char code SILParTxt[]           = "SIL";

char code DMYParTxt[]          = "DMY";
char code MDYParTxt[]          = "MDY";
char code YMDParTxt[]          = "YMD";

char code BBParTxt[]           = "BB";
char code SPGParTxt[]            = "SPG";

char code OFFParTxt[]           = "OFF";
char code ONParTxt[]           = "ON";

char code UPParTxt[]           = "UP";
char code DOWNParTxt[]            = "DOWN";

char code NORMALParTxt[]         = "NORMAL";
char code NORMParTxt[]           = "NORM";

char code MINParTxt[]           = "MIN";
char code MAXParTxt[]           = "MAX";

char code UNLOCKEDParTxt[]        = "UNLOCKED";
char code GENLOCKEDParTxt[]        = "GENLOCKED";


void CLSCommand(void);
void ESECommand(void);
void ESRCommand(void);
void OPCCommand(void);
void SRECommand(void);
void STBCommand(void);
void TSTCommand(void);
void WAICommand(void);
void RSTCommand(void);
void IDNCommand(void);
void RCLCommand(void);
void SAVCommand(void);

void SystemError(void);
void SystemVersion(void);
void SystemTime(void);
void SystemDate(void);

void SystemPresetName(void);
void SystemPresetAuthor(void);
void SystemPresetDate(void);
void SystemPresetDownload(void);
void SystemPresetUpload(void);

void StatusOperEvent(void);
void StatusOperCondition(void);
void StatusOperEnable(void);
void StatusQuestEvent(void);
void StatusQuestCondition(void);
void StatusQuestEnable(void);
void StatusPreset(void);

void GenlockGen(void);
void GenlockGenSystem(void);
void GenlockGenDelay(void);
void GenlockGenCalibrateEnable(void);
void GenlockGenCalibrateAdjust(void);
void GenlockGenCalibrateStore(void);

void BBGenSuffix(void);
void BBGen(void);
void BBGenSystem(void);
void BBGenDelay(void);
void BBGenScHPhase(void);

void TSGen(void);
void TSGenPattern(void);
void TSGenSystem(void);
void TSGenDelay(void);
void TSGenScHPhase(void);
void TSGenEmbAudioSignal(void);

void AudioGenOutput(void);

void AudioGenAnalog(void);
void AudioGenAnalogSignal(void);
void AudioGenAnalogLevel(void);
void AudioGenAnalogClick(void);

void AudioGenAESEBU(void);
void AudioGenAESEBUSystem(void);
void AudioGenAESEBUSignal(void);
void AudioGenAESEBULevel(void);
void AudioGenAESEBUTiming(void);
void AudioGenAESEBUWordClock(void);
void AudioGenAESEBUClick(void);

void FactPassword(void);
void FactReset(void);
void FactV24Command(void);
void FactV24Transparent(void);

void FactChecksumCode(void);
void FactChecksumPLD(void);
void FactChecksumEmbAudio(void);
void FactChecksumPattern(void);
void FactChecksumRange(void);

void FactMainVersion(void);
void FactMainIDNCompany(void);
void FactMainIDNType(void);
void FactMainIDNKUNumber(void);
void FactMainIDNSWRevision(void);

void FactMainSystem(void);
void FactMainFMSDateProd(void);
void FactMainFMSDateUpd(void);

void FactGenlockGenDate(void);
void FactGenlockGenVersion(void);
void FactGenlockGenFRefDate(void);
void FactGenlockGenFRefDAC(void);
void FactGenlockGenURefDate(void);
void FactGenlockGenURefDAC(void);
void FactGenlockGenHPHZero(void);
void FactGenlockGenPhase(void);

void FactBBGenDate(void);
void FactBBGenVersion(void);
void FactBBGenSignal(void);
void FactBBGenPhase(void);
void FactBBGenScHPhase(void);
void FactBBGenDAC(void);

void FactTSGenDate(void);
void FactTSGenAGain(void);
void FactTSGenCGain(void);
void FactTSGenPhase(void);
void FactTSGenScHPhase(void);

void FactAudioGenStatus(void);
void FactAudioGenDate(void);
void FactAudioGenLevel(void);

void FactPLDVersion(void);
void FactTCXOStatus(void);
void FactMultiMediaChip(void);

int FindParameter(char* par, struct PAR_STRUCT code* TmpPtr);

long FLTToSamples(UC system, int F, int L, float T);
void PrintSamplesToFLT(UC system, long samples);

// **************************************************************************
struct CMD_STRUCT code Mandated[12] =   {
    { CLSCmdTxt, NULL, &Mandated[1],  NULL, NONE, CLSCommand  },
    { ESECmdTxt, NULL, &Mandated[2],  NULL, NONE, ESECommand  },
    { ESRCmdTxt, NULL, &Mandated[3],  NULL, NONE, ESRCommand  },
    { IDNCmdTxt, NULL, &Mandated[4],  NULL, NONE, IDNCommand  },
    { OPCCmdTxt, NULL, &Mandated[5],  NULL, NONE, OPCCommand  },
    { RSTCmdTxt, NULL, &Mandated[6],  NULL, NONE, RSTCommand  },
    { SRECmdTxt, NULL, &Mandated[7],  NULL, NONE, SRECommand  },
    { STBCmdTxt, NULL, &Mandated[8],  NULL, NONE, STBCommand  },
    { TSTCmdTxt, NULL, &Mandated[9],  NULL, NONE, TSTCommand  },
    { RCLCmdTxt, NULL, &Mandated[10], NULL, NONE, RCLCommand  },
    { SAVCmdTxt, NULL, &Mandated[11], NULL, NONE, SAVCommand  },
    { WAICmdTxt, NULL, NULL         , NULL, NONE, WAICommand  }
  };

struct CMD_STRUCT code Subsystem[9] =   {
    { SYSTEMCmdTxt,    SYSTCmdTxt, &Subsystem[1], &System[0],  NONE, NONE  },
    { STATUSCmdTxt,     STATCmdTxt, &Subsystem[2], &Status[0],  NONE, NONE  },
    { DIAGNOSTICCmdTxt, DIAGCmdTxt, &Subsystem[3], NULL,       NONE, NONE  },
    { MONITORCmdTxt,     MONCmdTxt,  &Subsystem[4], NULL,        NONE, NONE  },
    { INPUTCmdTxt,     INPCmdTxt,  &Subsystem[5], &Input[0],   NONE, NONE  },
    { OUTPUTCmdTxt,     OUTPCmdTxt, &Subsystem[6], &Output[0],  NONE, NONE  },
    { USERCmdTxt,       USERCmdTxt, &Subsystem[7], NULL,        NONE, NONE  },
    { MEASURECmdTxt,     MEASCmdTxt, &Subsystem[8], NULL,        NONE, NONE  },
    { FACTORYCmdTxt,     FACTCmdTxt, NULL,         &Factory[0], NONE, NONE  },
  };

// **************************************************************************
/*  CMD:  SUB1:  SUB2:  SUB3:  PARM:
    SYSTem
      :ERRor?
      :VERSion?
      :PRESet<?>
        [:ACTIVE?]
        :RECall<?>  1 .. 4
        :STORe      1 .. 4
        :NAMe<?>    1 .. 4 , <name>
        :AUTHor<?>   1 .. 4 , <author name>
        :DATE<?>    1 .. 4[, <year>,<month>,<date>]
        :DOWNload    1 .. 4
        :UPLoad      1 .. 4
      :EVENtlog
      :DATE
      :TIME
*/
struct CMD_STRUCT code System[6] =   {
    { ERRORCmdTxt,   ERRCmdTxt,   &System[1], NULL,         NONE, SystemError  },
    { VERSIONCmdTxt,  VERSCmdTxt, &System[2], NULL,         NONE, SystemVersion  },
    { PRESETCmdTxt,   PRESCmdTxt, &System[3], &SystemLevel1[0], NONE, RCLCommand  },
    { EVENTLOGCmdTxt, EVENCmdTxt, &System[4], NULL,         NONE, NONE  },
    { DATECmdTxt,     DATECmdTxt, &System[5], NULL,         NONE, NONE  },
    { TIMECmdTxt,     TIMECmdTxt, NULL,     NULL,         NONE, NONE  }
  };

struct CMD_STRUCT code SystemLevel1[7] =   {
    { RECALLCmdTxt,   RECCmdTxt,   &SystemLevel1[1], NULL, NONE, RCLCommand  },
    { STORECmdTxt,    STORCmdTxt, &SystemLevel1[2], NULL, NONE, SAVCommand  },
    { NAMECmdTxt,      NAMECmdTxt, &SystemLevel1[3],  NULL, NONE, SystemPresetName  },
    { AUTHORCmdTxt,   AUTHCmdTxt, &SystemLevel1[4],  NULL, NONE, SystemPresetAuthor  },
    { DATECmdTxt,      DATECmdTxt, &SystemLevel1[5],  NULL, NONE, SystemPresetDate  },
    { DOWNLOADCmdTxt, DOWNCmdTxt,  &SystemLevel1[6],  NULL, NONE, SystemPresetDownload  },
    { UPLOADCmdTxt,   UPLCmdTxt,  NULL,         NULL, NONE, SystemPresetUpload  },
  };

// **************************************************************************
/*  CMD:  SUB1:  SUB2:  SUB3:  PARM:
    STATus
       :OPERation
         :EVENt?
        :CONDition?
        :ENABle<?>  0 .. 65535
       :QUEStionable
         :EVENt?
        :CONDition?
        :ENABle<?>  0 .. 65535
      :PRESet        TBD
      :PT5201?
*/
struct CMD_STRUCT code Status[3] =   {
    { OPERATIONCmdTxt,    OPERCmdTxt, &Status[1], &StatusLevel1_1[0], NONE, StatusOperEvent  },
    { QUESTIONABLECmdTxt, QUESCmdTxt, &Status[2], &StatusLevel1_2[0], NONE, StatusQuestEvent  },
    { PRESETCmdTxt,      PRESCmdTxt, NULL,      NULL,           NONE, StatusPreset  },
  };

struct CMD_STRUCT code StatusLevel1_1[3] =   {
    { EVENTCmdTxt,    EVENCmdTxt, &StatusLevel1_1[1], NULL, NONE, StatusOperEvent  },
    { CONDITIONCmdTxt, CONDCmdTxt, &StatusLevel1_1[2], NULL, NONE, StatusOperCondition  },
    { ENABLECmdTxt,    ENABCmdTxt, NULL,            NULL, NONE, StatusOperEnable  },
  };

struct CMD_STRUCT code StatusLevel1_2[3] =   {
    { EVENTCmdTxt,     EVENCmdTxt, &StatusLevel1_2[1], NULL, NONE, StatusQuestEvent  },
    { CONDITIONCmdTxt, CONDCmdTxt, &StatusLevel1_2[2], NULL, NONE, StatusQuestCondition  },
    { ENABLECmdTxt,    ENABCmdTxt, NULL,           NULL, NONE, StatusQuestEnable  },
  };


// **************************************************************************
/*  CMD:  SUB1:  SUB2:    PARM:
    INPut
      :GENLOCK<?>
        :SYSTem      INTernal | PALBurst | NTSCburst |F10MHZ
        :DELay      <Field>,<Line>,<Time>
        :CALibrate
          :ENABle    OFF|ON
          :ADJust    UP|DOWN|MIN|MAX
          :STORe
                    
*/
//**************************************************************************
struct CMD_STRUCT code Input[1] =   {
    { GENLOCKCmdTxt, GENLCmdTxt, NULL, &GenlockLevel1[0], NONE, GenlockGen  },
  };

struct CMD_STRUCT code GenlockLevel1[3] =   {
    { SYSTEMCmdTxt,    SYSTCmdTxt, &GenlockLevel1[1], NULL,             NONE, GenlockGenSystem  },
    { DELAYCmdTxt,    DELCmdTxt,  &GenlockLevel1[2], NULL,             NONE, GenlockGenDelay  },
    { CALIBRATECmdTxt, CALCmdTxt,  NULL,           &GenlockLevel1_1[0], NONE, NONE  },
  };

struct CMD_STRUCT code GenlockLevel1_1[3] =   {
    { ENABLECmdTxt, ENABCmdTxt, &GenlockLevel1_1[1], NULL, NONE, GenlockGenCalibrateEnable  },
    { ADJUSTCmdTxt, ADJCmdTxt,  &GenlockLevel1_1[2], NULL, NONE, GenlockGenCalibrateAdjust  },
    { STORECmdTxt,  STORCmdTxt, NULL,            NULL, NONE, GenlockGenCalibrateStore  },
  };
// **************************************************************************
/*  CMD:  SUB1:  SUB2:    PARM:
    OUTPut
      :BB1-3<?>
        :SYSTem      INTernal | PALBurst | NTSCburst |F10MHZ
        :DELay      <Field>,<Line>,<Time>
        :SCHPhase    <-179to 180>

      :TSGenerator<?>
        :PATTern     <pattern name>
        :SYSTem      PAL|NTSC
        :DELay      <Field>,<Line>,<Time>
        :SCHPhase    <-179to 180>
        :EMBaudio<?>
          [:SIGNal]  OFF | SILence | F1KHZ
          :LEVel

      :AUDio
        :OUTPut
        :ANALog<?>
          :SIGNal
          :LEVel
          :CLICk
        :AESebu<?>
          :SYSTem
          :SIGNal
          :LEVel
          :TIMing
          :WORDclock
          :CLICkperiod
*/
struct CMD_STRUCT code Output[3] =   {
    { BBCmdTxt,       BBCmdTxt,  &Output[1], &BBGenLevel1[0],   BBGenSuffix, BBGen  },
    { TSGENERATORCmdTxt, TSGCmdTxt, &Output[2], &TSGenLevel1[0],    NONE,      TSGen  },
    { AUDIOCmdTxt,     AUDCmdTxt, NULL,      &AudioGenLevel1[0], NONE,      AudioGenOutput  },
  };

struct CMD_STRUCT code BBGenLevel1[3] =   {
    { SYSTEMCmdTxt,   SYSTCmdTxt, &BBGenLevel1[1], NULL, NONE, BBGenSystem  },
    { DELAYCmdTxt,     DELCmdTxt,  &BBGenLevel1[2], NULL, NONE, BBGenDelay  },
    { SCHPHASECmdTxt, SCHPCmdTxt, NULL,         NULL, NONE, BBGenScHPhase  },
  };

struct CMD_STRUCT code TSGenLevel1[5] =   {
    { PATTERNCmdTxt,   PATTCmdTxt, &TSGenLevel1[1], NULL,           NONE, TSGenPattern  },
    { SYSTEMCmdTxt,   SYSTCmdTxt, &TSGenLevel1[2], NULL,           NONE, TSGenSystem  },
    { DELAYCmdTxt,     DELCmdTxt,  &TSGenLevel1[3], NULL,           NONE, TSGenDelay  },
    { SCHPHASECmdTxt, SCHPCmdTxt, &TSGenLevel1[4], NULL,           NONE, TSGenScHPhase  },
    { EMBAUDIOCmdTxt, EMBCmdTxt,   NULL,         &TSGenLevel1_1[0], NONE, TSGenEmbAudioSignal  },
  };

struct CMD_STRUCT code TSGenLevel1_1[2] =   {
    { SIGNALCmdTxt, SIGNCmdTxt, &TSGenLevel1_1[1], NULL, NONE, TSGenEmbAudioSignal  },
    { LEVELCmdTxt,  LEVCmdTxt,  NULL,          NULL, NONE, NONE  },
  };

struct CMD_STRUCT code AudioGenLevel1[3] =   {
    { OUTPUTCmdTxt, OUTPCmdTxt, &AudioGenLevel1[1], NULL,            NONE, AudioGenOutput  },
    { ANALOGCmdTxt, ANALCmdTxt, &AudioGenLevel1[2], &AudioGenLevel1_1[0], NONE, AudioGenAnalog  },
    { AESEBUCmdTxt, AESCmdTxt,  NULL,            &AudioGenLevel1_2[0], NONE, AudioGenAESEBU  },
  };

struct CMD_STRUCT code AudioGenLevel1_1[3] =   {
    { SIGNALCmdTxt, SIGNCmdTxt, &AudioGenLevel1_1[1], NULL, NONE, AudioGenAnalogSignal  },
    { LEVELCmdTxt,  LEVCmdTxt,  &AudioGenLevel1_1[2], NULL, NONE, AudioGenAnalogLevel  },
    { CLICKCmdTxt,  CLICCmdTxt, NULL,             NULL, NONE, AudioGenAnalogClick  },
  };

struct CMD_STRUCT code AudioGenLevel1_2[6] =   {
    { SYSTEMCmdTxt,    SYSTCmdTxt, &AudioGenLevel1_2[1], NULL, NONE, AudioGenAESEBUSystem  },
    { SIGNALCmdTxt,    SIGNCmdTxt, &AudioGenLevel1_2[2], NULL, NONE, AudioGenAESEBUSignal  },
    { LEVELCmdTxt,    LEVCmdTxt,  &AudioGenLevel1_2[3], NULL, NONE, AudioGenAESEBULevel  },
    { TIMINGCmdTxt,   TIMCmdTxt,  &AudioGenLevel1_2[4], NULL, NONE, AudioGenAESEBUTiming  },
    { WORDCLOCKCmdTxt, WORDCmdTxt, &AudioGenLevel1_2[5], NULL, NONE, AudioGenAESEBUWordClock  },
    { CLICKCmdTxt,    CLICCmdTxt, NULL,             NULL, NONE, AudioGenAESEBUClick  },
  };

// **************************************************************************
/*  CMD:  SUB1:  SUB2:    PARM:
    FACTORY
      :PASSword

      :RESet

      :V24Circuit
        :COMMand
        :TRANsparent

      :CHECksum
        :CODE
        :PLD
        :EMBaudio
        :PATTern

      :MAINboard
        :VERSion
        :IDN
          :COMPany
          :TYPE
          :KUNumber
          :SWRevision
        :SYSTem        PAL|NTSC
        :FMSDate
          :PRODuction  <year>,<month>,<day>
          :UPDate      <year>,<month>,<day>

      :GENLock
        [:DATE]        <year>,<month>,<day>
        :VERSion
        :FREFerence
          [:DATE]      <year>,<month>,<day>
          :DAC
        :UREFerence
          [:DATE]       <year>,<month>,<day>
          :DAC
        :HPHZero      <pal>,<ntsc>
        :PHASe        <pal>,<ntsc>

      :BB 1
        [:DATE]        <year>,<month>,<day>
        :VERSion
        :SIGNAL        NORMAL|F45MHZ
        :PHASe        <pal>,<ntsc>
        :SCHPhase      <pal>,<ntsc>
        :DAC          <gain>,<offset>,<level>

      :BB 2-3
        [:DATE]        <year>,<month>,<day>
        :PHASe        <pal>,<ntsc>
        :SCHPhase      <pal>,<ntsc>
        :DAC          <gain>,<offset>,<level>

      :TSGenerator
        [:DATE]        <year>,<month>,<day>
        :AGAin        <pal>,<ntsc>
        :CGAin        <pal>,<ntsc>
        :PHASE        <pal>,<ntsc>
        :SCHPhase      <pal>,<ntsc>

      :AUDio
        [:STATus]
        :DATE          <year>,<month>,<day>
        :ALEVel        <left>,<right>

      :PLD
        :VERSion      <video>,<audio>

      :TCXO
        [:STATus]

      :MMCH            <subaddress>[,<data>]

*/                  

struct CMD_STRUCT code Factory[12] =   {
    { PASSWORDCmdTxt,   PASSCmdTxt, &Factory[1],  NULL,          NONE,      FactPassword  },
    { RESETCmdTxt,     RESCmdTxt,  &Factory[2],  NULL,          NONE,      FactReset  },
    { V24CIRCUITCmdTxt,   V24CCmdTxt, &Factory[3],  &FactoryLevel1[0], NONE,      NONE  },
    { CHECKSUMCmdTxt,   CHECCmdTxt, &Factory[4],  &FactoryLevel9[0], NONE,         NONE  },
    { MAINBOARDCmdTxt,   MAINCmdTxt, &Factory[5],  &FactoryLevel2[0], NONE,         NONE  },
    { GENLOCKCmdTxt,     GENLCmdTxt,  &Factory[6],  &FactoryLevel3[0], NONE,         FactGenlockGenDate  },
    { BBCmdTxt,       BBCmdTxt,   &Factory[7],  &FactoryLevel4[0], BBGenSuffix, FactBBGenDate  },
    { TSGENERATORCmdTxt, TSGCmdTxt,   &Factory[8],  &FactoryLevel5[0], NONE,         FactTSGenDate  },
    { AUDIOCmdTxt,     AUDCmdTxt,   &Factory[9],  &FactoryLevel6[0], NONE,         FactAudioGenStatus  },
    { PLDCmdTxt,       PLDCmdTxt,   &Factory[10], &FactoryLevel7[0], NONE,       FactPLDVersion  },
    { TCXOCmdTxt,       TCXOCmdTxt,  &Factory[11], &FactoryLevel8[0], NONE,       FactTCXOStatus  },
    { MMCHIPCmdTxt,     MMCHCmdTxt, NULL,        NULL,          NONE,       FactMultiMediaChip  },
  };

struct CMD_STRUCT code FactoryLevel1[2] =   {
    { COMMANDCmdTxt,      COMMCmdTxt, &FactoryLevel1[1], NULL, NONE, FactV24Command  },
    { TRANSPARENTCmdTxt, TRANCmdTxt, NULL,              NULL, NONE, FactV24Transparent  },
  };

struct CMD_STRUCT code FactoryLevel9[5] =   {
    { CODECmdTxt,     CODECmdTxt, &FactoryLevel9[1], NULL, NONE, FactChecksumCode  },
    { PLDCmdTxt,       PLDCmdTxt,  &FactoryLevel9[2], NULL, NONE, FactChecksumPLD  },
    { EMBAUDIOCmdTxt, EMBCmdTxt,  &FactoryLevel9[3], NULL, NONE, FactChecksumEmbAudio  },
    { PATTERNCmdTxt,   PATTCmdTxt, &FactoryLevel9[4], NULL, NONE, FactChecksumPattern  },
    { RANGECmdTxt,   RANGCmdTxt, NULL,          NULL, NONE, FactChecksumRange  },
  };

struct CMD_STRUCT code FactoryLevel2[4] =   {
    { VERSIONCmdTxt,   VERSCmdTxt, &FactoryLevel2[1], NULL,            NONE, FactMainVersion  },
    { IDNCmdTxt,       IDNCmdTxt,  &FactoryLevel2[2], &FactoryLevel2_2[0], NONE, NONE  },
    { SYSTEMCmdTxt,   SYSTCmdTxt, &FactoryLevel2[3], NULL,            NONE, FactMainSystem  },
    { FMSDATECmdTxt,   FMSDCmdTxt, NULL,          &FactoryLevel2_1[0], NONE, NONE  },
  };

struct CMD_STRUCT code FactoryLevel2_1[2] =   {
    { PRODUCTIONCmdTxt, PRODCmdTxt, &FactoryLevel2_1[1], NULL, NONE, FactMainFMSDateProd  },
    { UPDATECmdTxt,     UPDCmdTxt,  NULL,             NULL, NONE, FactMainFMSDateUpd  },
  };

struct CMD_STRUCT code FactoryLevel2_2[4] =   {
    { COMPANYCmdTxt,    COMPCmdTxt, &FactoryLevel2_2[1], NULL, NONE, FactMainIDNCompany  },
    { TYPECmdTxt,      TYPECmdTxt, &FactoryLevel2_2[2], NULL, NONE, FactMainIDNType  },
    { KUNUMBERCmdTxt,   KUNCmdTxt,  &FactoryLevel2_2[3], NULL, NONE, FactMainIDNKUNumber  },
    { SWREVISIONCmdTxt, SWRCmdTxt,  NULL,             NULL, NONE, FactMainIDNSWRevision  },
  };

struct CMD_STRUCT code FactoryLevel3[6] =   {
    { DATECmdTxt,      DATECmdTxt, &FactoryLevel3[1], NULL,           NONE, FactGenlockGenDate  },
    { VERSIONCmdTxt,    VERSCmdTxt, &FactoryLevel3[2], NULL,           NONE, FactGenlockGenVersion  },
    { FREFERENCECmdTxt, FREFCmdTxt, &FactoryLevel3[3], &FactoryLevel3_1[0], NONE, FactGenlockGenFRefDate  },
    { UREFERENCECmdTxt, UREFCmdTxt, &FactoryLevel3[4], &FactoryLevel3_2[0], NONE, FactGenlockGenURefDate  },
    { HPHZEROCmdTxt,    HPHZCmdTxt, &FactoryLevel3[5], NULL,            NONE, FactGenlockGenHPHZero  },
    { PHASECmdTxt,      PHASCmdTxt, NULL,           NULL,            NONE, FactGenlockGenPhase  },
  };

struct CMD_STRUCT code FactoryLevel3_1[2] =   {
    { DATECmdTxt, DATECmdTxt, &FactoryLevel3_1[1], NULL, NONE, FactGenlockGenFRefDate  },
    { DACCmdTxt,  DACCmdTxt,  NULL,             NULL, NONE, FactGenlockGenFRefDAC  },
  };

struct CMD_STRUCT code FactoryLevel3_2[2] =   {
    { DATECmdTxt, DATECmdTxt, &FactoryLevel3_2[1], NULL, NONE, FactGenlockGenURefDate  },
    { DACCmdTxt,  DACCmdTxt,  NULL,             NULL, NONE, FactGenlockGenURefDAC  },
  };

struct CMD_STRUCT code FactoryLevel4[6] =   {
    { DATECmdTxt,    DATECmdTxt, &FactoryLevel4[1], NULL, NONE, FactBBGenDate  },
    { VERSIONCmdTxt,   VERSCmdTxt, &FactoryLevel4[2], NULL, NONE, FactBBGenVersion  },
    { SIGNALCmdTxt,   SIGNCmdTxt, &FactoryLevel4[3], NULL, NONE, FactBBGenSignal  },
    { PHASECmdTxt,   PHASCmdTxt, &FactoryLevel4[4], NULL, NONE, FactBBGenPhase  },
    { SCHPHASECmdTxt,  SCHPCmdTxt, &FactoryLevel4[5], NULL, NONE, FactBBGenScHPhase  },
    { DACCmdTxt,      DACCmdTxt,  NULL,          NULL, NONE, FactBBGenDAC  },
  };

struct CMD_STRUCT code FactoryLevel5[5] =   {
    { DATECmdTxt,    DATECmdTxt, &FactoryLevel5[1], NULL, NONE, FactTSGenDate  },
    { AGAINCmdTxt,   AGACmdTxt,   &FactoryLevel5[2], NULL, NONE, FactTSGenAGain  },
    { CGAINCmdTxt,   CGACmdTxt,   &FactoryLevel5[3], NULL, NONE, FactTSGenCGain  },
    { PHASECmdTxt,   PHASCmdTxt, &FactoryLevel5[4], NULL, NONE, FactTSGenPhase  },
    { SCHPHASECmdTxt,  SCHPCmdTxt, NULL,          NULL, NONE, FactTSGenScHPhase  },
  };

struct CMD_STRUCT code FactoryLevel6[3] =   {
    { STATUSCmdTxt, STATCmdTxt, &FactoryLevel6[1], NULL, NONE, FactAudioGenStatus  },
    { DATECmdTxt,   DATECmdTxt, &FactoryLevel6[2], NULL, NONE, FactAudioGenDate  },
    { ALEVELCmdTxt, ALEVCmdTxt, NULL,           NULL, NONE, FactAudioGenLevel  },
  };

struct CMD_STRUCT code FactoryLevel7[1] =   {
    { VERSIONCmdTxt,   VERSCmdTxt, NULL, NULL, NONE, FactPLDVersion  },
  };

struct CMD_STRUCT code FactoryLevel8[1] =   {
    { STATUSCmdTxt, STATCmdTxt, NULL, NULL, NONE, FactTCXOStatus  },
  };

//**************************************************************************
// The following parameter structures are based on the enumerated types
//  defined in TABLES.H, ie. DO NOT change the order of these names. If
//  the order is changed in TABLES.H it must also be changed here.

struct PAR_STRUCT code SAVEPar[1] =   {
    { SAVEParTxt, SAVEParTxt, NULL  }
  };

struct PAR_STRUCT code GenlockSystemPar[4] =   {
    { INTERNALParTxt,  INTParTxt,   &GenlockSystemPar[1]  },
    { PALBURSTParTxt,  PALBParTxt,   &GenlockSystemPar[2]  },
    { NTSCBURSTParTxt, NTSCParTxt,   &GenlockSystemPar[3]  },
    { F10MHzParTxt,    F10MHzParTxt, NULL  },
  };

struct PAR_STRUCT code SystemPar[3] =   {
    { PALParTxt,    PALParTxt, &SystemPar[1]  },
    { PAL_IDParTxt, PAL_IDParTxt, &SystemPar[2]  },
    { NTSCParTxt,    NTSCParTxt, NULL  },
  };

struct PAR_STRUCT code CBPatternPar[23] =   {
    { CBSMPTEParTxt,     CBSMParTxt,   &CBPatternPar[1]  },
    { CBEBUParTxt,      CBEBParTxt,   &CBPatternPar[2]  },
    { CBFCCParTxt,      CBFCParTxt,   &CBPatternPar[3]  },
    { CBEBU8ParTxt,      CBEB8ParTxt,   &CBPatternPar[4]  },
    { CB100ParTxt,      CB100ParTxt,   &CBPatternPar[5]  },
    { CBGREY75ParTxt,   CBGR75ParTxt,   &CBPatternPar[6]  },
    { CBRED75ParTxt,     CBR75ParTxt,   &CBPatternPar[7]  },
    { RED75ParTxt,     RED75ParTxt,   &CBPatternPar[8]  },

    { MULTIBURSTParTxt,  MULTParTxt,   &CBPatternPar[9]  },
    { CCIR18ParTxt,     CCIR18ParTxt,  &CBPatternPar[10]  },
                                      
    { WIN10ParTxt,     WIN10ParTxt,   &CBPatternPar[11]  },
    { WIN15ParTxt,     WIN15ParTxt,  &CBPatternPar[12]  },
    { WIN20ParTxt,     WIN20ParTxt,   &CBPatternPar[13]  },
    { WIN100ParTxt,     WIN100ParTxt,   &CBPatternPar[14]  },
    { BLWH15ParTxt,     BLWH15ParTxt,   &CBPatternPar[15]  },
    { WHITE100ParTxt,   WHIT100ParTxt, &CBPatternPar[16]  },
    { BLACKParTxt,     BLACParTxt,   &CBPatternPar[17]  },

    { SDICHECKParTxt,   SDICParTxt,   &CBPatternPar[18]  },
    { DGREYParTxt,     DGRParTxt,     &CBPatternPar[19]  },

    { STAIRCASE5ParTxt,   STA5ParTxt,   &CBPatternPar[20]  },
    { STAIRCASE10ParTxt, STA10ParTxt,   &CBPatternPar[21]  },

    { CROSSHATCHParTxt,   CROSParTxt,   &CBPatternPar[22]  },
    { PLUGEParTxt,     PLUGParTxt,   NULL  },
  };

struct PAR_STRUCT code EmbAudioSignalPar[3] =   {
    { OFFParTxt,       OFFParTxt,     &EmbAudioSignalPar[1]  },
    { SILENCEParTxt,   SILParTxt,     &EmbAudioSignalPar[2]  },
    { S1KHZParTxt,     S1KHZParTxt,   NULL  },
  };

struct PAR_STRUCT code AudioOutputPar[2] =   {
    { ANALOGParTxt, ANALParTxt, &AudioOutputPar[1]  },
    { AESEBUParTxt, AESParTxt,  NULL  },
  };

struct PAR_STRUCT code AESEBUSystemPar[2] =   {
    { PALParTxt,  PALParTxt,  &AESEBUSystemPar[1]  },
    { NTSCParTxt, NTSCParTxt, NULL  },
  };

struct PAR_STRUCT code AudioSignalPar[4] =   {
    { S500HZParTxt,   S500HZParTxt,    &AudioSignalPar[1]  },
    { S1KHZParTxt,   S1KHZParTxt,    &AudioSignalPar[2]  },
    { SEBU1KHZParTxt, SEBU1KHZParTxt, &AudioSignalPar[3]  },
    { S8KHZParTxt,   S8KHZParTxt,    NULL  },
  };

struct PAR_STRUCT code AnalogLevelPar[1] =   {
    { SILENCEParTxt, SILParTxt, NULL  },
  };

struct PAR_STRUCT code AESEBULevelPar[1] =   {
    { SILENCEParTxt, SILParTxt, NULL  },
  };

struct PAR_STRUCT code AudioFrequencyPar[2] =   {
    { F441KHZParTxt, F441KHZParTxt, &AudioFrequencyPar[1]  },
    { F48KHZParTxt,  F48KHZParTxt,  NULL  },
  };

struct PAR_STRUCT code UnitPar[2] =   {
    { BBParTxt,  BBParTxt,  &UnitPar[1]  },
    { SPGParTxt, SPGParTxt, NULL  },
  };

struct PAR_STRUCT code OffOnPar[2] =   {
    { OFFParTxt, OFFParTxt, &OffOnPar[1]  },
    { ONParTxt,  ONParTxt,  NULL  },
  };

struct PAR_STRUCT code FactBBGenSignalPar[2] =   {
    { NORMALParTxt, NORMParTxt,   &FactBBGenSignalPar[1]  },
    { F45MHZParTxt, F45MHZParTxt, NULL  },
  };

struct PAR_STRUCT code MinMaxPar[2] =   {
    { MINParTxt, MINParTxt, &MinMaxPar[1]  },
    { MAXParTxt, MAXParTxt, NULL  },
  };

struct PAR_STRUCT code UpDownPar[2] =   {
    { UPParTxt,   UPParTxt,   &UpDownPar[1]  },
    { DOWNParTxt, DOWNParTxt, NULL  },
  };

/***************************************************************************/
// Temporary variables PRIVATE NOT overlayable

static unsigned int Suffix[4]=
  {
  1,1,1,1
  };

static bool PasswordProtected = true;
static UC GenlockCalEnable = 0;
static int GenlockCalValue = 0;

/**************************************************************************/
/* CLSCommand                                                    RS232CMD.C */
/*                                                                        */
/* Author:     Kim Engedahl, DEV, 941124                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Clear status command                                        */
/* Remarks:    No parameters                                                */
/* Returns:    -                                                            */
/* Updates:                                                                */
/**************************************************************************/
void CLSCommand(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        ResetErrorEventQueue();
      }
    }
  }

/**************************************************************************/
/* ESECommand                                                    RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970627                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Set/read the Standard Event Status Enable Register          */
/* Remarks:    --                                                          */
/* Returns:    -                                                            */
/* Updates:                                                                */
/**************************************************************************/
void ESECommand(void)
  {
  int Reg;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(12, "%d\n", ESEReg);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &Reg) != 1))
        {
        SCPIError = SyntaxErr;  
        return;
        }

      if (Reg > 255)
        {
        SCPIError = DataOutOfRange;
        return;
        }

      ESEReg = Reg & 0xFF;
      }
    }
  }

/**************************************************************************/
/* ESRCommand                                                   RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970627                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Read and clear the Standard Event Status Register            */
/* Remarks:    No parameters                                                */
/*            The register will reset to zero.                            */
/* Returns:    -                                                            */
/* Updates:                                                                */
/**************************************************************************/
void ESRCommand(void)
  {
  int Reg;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        rs232printf(12, "%d\n", UpdateESRRegister());

        ESRReg = 0;
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* OPCCommand                                                    RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Operation Complete Command/Query                            */
/* Remarks:    No parameters                                                */
/* Returns:    -                                                            */
/* Updates:                                                                */
/**************************************************************************/
void OPCCommand(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      }
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      }
    }
  }

/**************************************************************************/
/* SRECommand                                                     RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970627                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Set/read the Service Request Enable Register                */
/* Remarks:    Bit 6 in *SRE? register is always ZERO                      */
/*            Bit 6 in *SRE register is ignored                            */
/* Returns:    -                                                            */
/* Updates:                                                                */
/**************************************************************************/
void SRECommand(void)
  {
  int Reg;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(12, "%d\n", SREReg);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &Reg) != 1))
        {
        SCPIError = SyntaxErr;  
        return;
        }

      if (Reg > 255)
        {
        SCPIError = DataOutOfRange;
        return;
        }

      SREReg = Reg & 0xBF;
      }
    }
  }

/**************************************************************************/
/* STBCommand                                                   RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970627                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Read the Status Byte Register                                */
/* Remarks:    No parameters                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
void STBCommand(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        UpdateSTBRegister();

        rs232printf(12, "%d\n", STBReg);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* TSTCommand                                                   RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970630                                    */
/* Revised:    000505                                                       */
/*                                                                        */
/* Function:  Selftest query                                              */
/* Remarks:    No parameters                                                */
/* Returns:    -                                                            */
/* Updates:                                                                */
/**************************************************************************/
void TSTCommand(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        ;
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* WAICommand                                                   RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970630                                    */
/* Revised:    000505                                                       */
/*                                                                        */
/* Function:  Wait-to-continue command                                    */
/* Remarks:    No parameters                                                */
/* Returns:    -                                                            */
/* Updates:                                                                */
/**************************************************************************/
void WAICommand(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        ;
        }
      }
    }
  }

/**************************************************************************/
/* RSTCommand                                                   RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000617                                    */
/* Revised:    000617                                                       */
/*                                                                        */
/* Function:  Reset comamnd                                                */
/* Remarks:    No parameters                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
void RSTCommand(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        ResetInstrument();
      }
    }
  }

/**************************************************************************/
/* IDNCommand                                                   RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  Identification query                                        */
/* Remarks:    No parameter, Query only                                    */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void IDNCommand(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        rs232puts(Calibration.Company);

        rs232putchar(',');

        rs232puts(Calibration.Type);

        rs232putchar(',');

        rs232puts(Calibration.KUNumber);

        rs232putchar(',');

        rs232puts(Calibration.SWRevision);

        rs232putchar('\n');
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* RCLCommand                                                   RS232CMD.C */
/*                                                                        */
/* Author:    Sten A. Andersen/Thomas Kappelgaard, DEV, xxxxxx             */
/* Revised:    Kim Engedahl, DEV, 000617                                    */
/*                                                                        */
/* Function:                                                              */
/* Remarks:                                                                */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void RCLCommand(void) 
  {
  int  Reg = 0;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      SCPIError = SyntaxErr;
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &Reg) != 1))
        {
        SCPIError = SyntaxErr;  
        return;
        }

      if ((Reg == 0) || (Reg > NoOfPresets))
        SCPIError = DataOutOfRange;
      else
        {
        if (RecallPreset(Reg) != OK)
          SCPIError = ExecErr;
        }
      }
    }
  }

/**************************************************************************/
/* SAVCommand                                                   RS232CMD.C */
/*                                                                        */
/* Author:    Sten A. Andersen/Thomas Kappelgaard, DEV, xxxxxx             */
/* Revised:    Kim Engedahl, DEV, 000502                                    */
/*                                                                        */
/* Function:                                                              */
/* Remarks:                                                                */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void SAVCommand(void) 
  {
  int  Reg = 0;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &Reg) != 1))
        {
        SCPIError = SyntaxErr;  
        return;
        }

      if ((Reg == 0) || (Reg > NoOfPresets))
        SCPIError = DataOutOfRange;
      else
        {
        if (StorePreset(Reg) != OK)
          SCPIError = ExecErr;
        }
      }
    }
  }

/**************************************************************************/
/* SystemError                                                 RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    000401                                                       */
/*                                                                        */
/* Function:  Write the next entry in the error queue (FIFO) to RS232     */
/* Remarks:    No parameters, Query only                                    */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void SystemError(void)
  {
  int i;
  code char *ptr;
  char buf[12];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt > 0)
        SCPIError = ParNotAllowed;
      else
        {
        if ((i = ReadErrorEventQueue()) >= 0)
          {
          sprintf(buf, "%d,\"", ErrorTxt_Array[i].ErrorNo);
          rs232puts(buf);

          ptr = ErrorTxt_Array[i].ErrorTxt;

          while (*ptr)
            rs232putchar(*ptr++);

           rs232puts("\"\n");
          }
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* SystemVersion                                               RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    000401                                                       */
/*                                                                        */
/* Function:  Write the SCPI version to RS232                              */
/* Remarks:    No parameter, Query only                                    */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void SystemVersion(void)
  {
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = SystemVersionTxt;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* SystemPresetName                                             RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970206                                    */
/* Revised:    000503                                                       */
/*                                                                        */                
/* Function:  Name a specific preset number.                              */
/* Remarks:    the scanf function can not used MaxPresetNameLength, i.e.    */
/*            this define should be inserted as a value. The value is      */
/*            defined  in INSTRU.H                                           */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void SystemPresetName(void)
  {
  int Reg;
  char Name[MaxPresetNameLength];
  PresetObject Preset;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &Reg) != 1))
        SCPIError = SyntaxErr;
      else
        {
        if ((Reg == 0) || (Reg > NoOfPresets))
          SCPIError = DataOutOfRange;
        else
          {
          NV_Recall(&Preset, PresetPtr[Reg-1], sizeof(Preset));

          rs232puts(Preset.Name);
          rs232putchar('\n');
          }
        }
      }
    else
      {
                                // The second parameter, the string, in scanf
                                // should be limited to the definition of
                                // MaxPresetNameLength
      if ((ParCnt != 2) || ((sscanf(Cmd, "%d,%17s", &Reg, Name)) != 2))
        SCPIError = SyntaxErr;
      else
        {
        if (strlen(Name) == MaxPresetNameLength)
          SCPIError = SyntaxErr;
        else
          {
          if ((Reg == 0) || (Reg > NoOfPresets))
            SCPIError = DataOutOfRange;
          else
            {
            NV_Recall(&Preset, PresetPtr[Reg-1], sizeof(Preset));
            strcpy(Preset.Name, Name);
            NV_Store(&Preset, PresetPtr[Reg-1], sizeof(Preset));
            }
          }
        }
      }
    }
  }

/**************************************************************************/
/* SystemPresetAuthor                                           RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000502                                    */
/* Revised:    000503                                                       */
/*                                                                        */                
/* Function:  Name the author of a specific preset number.                 */
/* Remarks:    the scanf function can not used MaxPresetAuthorLength, i.e.  */
/*            this define should be inserted as a value. The value is      */
/*            defined  in INSTRU.H                                           */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void SystemPresetAuthor(void)
  {
  int Reg;
  char Author[MaxPresetAuthorLength];
  PresetObject Preset;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &Reg) != 1))
        SCPIError = SyntaxErr;
      else
        {
        if ((Reg == 0) || (Reg > NoOfPresets))
          SCPIError = DataOutOfRange;
        else
          {
          NV_Recall(&Preset, PresetPtr[Reg-1], sizeof(Preset));

          rs232puts(Preset.Author);
          rs232putchar('\n');
          }
        }
      }
    else
      {
                                // The second parameter, the string, in scanf
                                // should be limited to the definition of
                                // MaxPresetAuthorLength
      if ((ParCnt != 2) || ((sscanf(Cmd, "%d,%7s", &Reg, Author)) != 2))
        SCPIError = SyntaxErr;
      else
        {
        if (strlen(Author) == MaxPresetAuthorLength)
          SCPIError = SyntaxErr;
        else
          {
          if ((Reg == 0) || (Reg > NoOfPresets))
            SCPIError = DataOutOfRange;
          else
            {
            NV_Recall(&Preset, PresetPtr[Reg-1], sizeof(Preset));
            strcpy(Preset.Author, Author);
            NV_Store(&Preset, PresetPtr[Reg-1], sizeof(Preset));
            }
          }
        }
      }
    }
  }

/**************************************************************************/
/* SystemPresetDate                                             RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000617                                    */
/* Revised:    001016                                                       */
/*                                                                        */                
/* Function:  -                                                            */
/* Remarks:    -                                                            */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void SystemPresetDate(void)
  {
  int Reg;
  PresetObject Preset;
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &Reg) != 1))
        SCPIError = SyntaxErr;
      else
        {
        if ((Reg == 0) || (Reg > NoOfPresets))
          SCPIError = DataOutOfRange;
        else
          {
          NV_Recall(&Preset, PresetPtr[Reg-1], sizeof(Preset));

          day = Preset.Date % 100;
          month = (Preset.Date / 100) % 100;
          year = (Preset.Date / 10000);

          sprintf(buffer, "%d,%d,%d\n", year, month, day);

          rs232puts(buffer);
          }
        }
      }
    else
      {
      if ((ParCnt != 4) || ((sscanf(Cmd, "%d,%d,%d,%d", &Reg, &year, &month, &day)) != 4))
        SCPIError = SyntaxErr;
      else
        {
        if (ValidateDate(year, month, day) == FAIL)
          SCPIError = ExecErr;
        else
          {
          if ((Reg == 0) || (Reg > NoOfPresets))
            SCPIError = DataOutOfRange;
          else
            {
            NV_Recall(&Preset, PresetPtr[Reg-1], sizeof(Preset));
            Preset.Date = day + 100*month + 10000*year;
            NV_Store(&Preset, PresetPtr[Reg-1], sizeof(Preset));
            }
          }        
        }
      }
    }
  }

/**************************************************************************/
/* SystemPresetDownload                                         RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000413                                    */
/* Revised:    000413                                                       */
/*                                                                        */                
/* Function:  Download a setup to a specific preset withouth disturbing    */
/*            the setting/function of the PT5201.                           */
/* Remarks:    --                                                          */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void SystemPresetDownload(void)
  {
  ;
  }

/**************************************************************************/
/* SystemPresetUpload                                           RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000413                                    */
/* Revised:    000413                                                       */
/*                                                                        */                
/* Function:  Upload a setup from a specific preset withouth disturbing    */
/*            the setting/function of the PT5201.                           */
/* Remarks:    --                                                          */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void SystemPresetUpload(void)
  {
  ;
  }

/**************************************************************************/
/* StatusOperEvent                                             RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970630                                    */
/* Revised:    000402                                                       */
/*                                                                        */
/* Function:  Set/read the status operation event register                */
/* Remarks:    Reading the event register clears it                        */
/* Returns:    -                                                            */
/* Updates:    OperEventReg                                                */
/**************************************************************************/
void StatusOperEvent(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (ParCnt)
      SCPIError = ParNotAllowed;
    else
      {
      if (CmdRequest)   {
        rs232printf(12, "%d\n", OperEventReg);

        OperEventReg = 0;
        }
      else
        SCPIError = SyntaxErr;
      }
    }
  }

/**************************************************************************/
/* StatusOperCondition                                         RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970630                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Read the status questionable condition register              */
/* Remarks:    No parameters                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
void StatusOperCondition(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (ParCnt)
      SCPIError = ParNotAllowed;
    else
      {
      if (CmdRequest)
        rs232printf(12, "%d\n", OperConditionReg);
      else
        SCPIError = SyntaxErr;
      }
    }
  }

/**************************************************************************/
/* StatusOperEnable                                             RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970630                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Set/read the status operaion enable register                */
/* Remarks:    --                                                          */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void StatusOperEnable(void)
  {
  long  Reg;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(12, "%d\n", OperEnableReg);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%l", &Reg) != 1))
        {
        SCPIError = SyntaxErr;  
        return;
        }

      if (Reg > 65535)
        {
        SCPIError = DataOutOfRange;
        return;
        }

      OperEnableReg = Reg & 0xEFFF;
      }
    }
  }

/***************************************************************************/
/* StatusQuestEvent                                              RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970630                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Set/read the status questionablen event register            */
/* Remarks:    Reading the event register clears it                        */
/* Returns:    -                                                            */
/* Updates:    QuestEventReg                                                */
/***************************************************************************/
void StatusQuestEvent(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (ParCnt)
      SCPIError = ParNotAllowed;
    else
      {
      if (CmdRequest)
        {
        rs232printf(12, "%d\n", QuestEventReg);

        QuestEventReg = 0;
        }
      else
        SCPIError = SyntaxErr;
      }
    }
  }

/**************************************************************************/
/* StatusQuestCondition                                          RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970630                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Read the status operation condition register                */
/* Remarks:    No parameters                                                */
/* Returns:    -                                                            */
/* Updates:    -                                                            */
/**************************************************************************/
void StatusQuestCondition(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (ParCnt)
      SCPIError = ParNotAllowed;
    else
      {
      if (CmdRequest)
      rs232printf(12, "%d\n", QuestConditionReg);
      else
        SCPIError = SyntaxErr;
      }
    }
  }

/**************************************************************************/
/* StatusQuestEnable                                           RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970630                                    */
/* Revised:    000413                                                       */
/*                                                                        */
/* Function:  Set/read the status questionable enable register            */
/* Remarks:    --                                                          */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
void StatusQuestEnable(void)
  {
  long  Reg;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(12, "%d\n", QuestEnableReg);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%l", &Reg) != 1))
        {
        SCPIError = SyntaxErr;  
        return;
        }

      if (Reg > 65535)
        {
        SCPIError = DataOutOfRange;
        return;
        }

      QuestEnableReg = Reg & 0xEFFF;
      }
    }
  }

/**************************************************************************/
/* StatusPreset                                                 RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000617                                    */
/* Revised:    000617                                                       */
/*                                                                        */
/* Function:                                                              */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/
void StatusPreset(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        if (Settings.ActivePreset)
          rs232putchar('0' + Settings.ActivePreset);
        else
          rs232puts("OFF");

        rs232putchar('\n');
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* GenlockGen                                                     RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000525                                    */
/* Revised:    001219                                                       */
/*                                                                        */
/* Function:  Return the Genlock Configuration.                            */
/* Remarks:    Request only                                                */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void GenlockGen(void)
  {
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (!CmdRequest)
        SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
            if (GenlockStatus)
           str = GENLOCKEDParTxt;
            else
           str = UNLOCKEDParTxt;

           while (*str)
            rs232putchar(*str++);

         rs232putchar(',');

        str = GenlockSystemPar[GenlockSetup.System].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar(',');

        PrintSamplesToFLT(GenlockSetup.System, GenlockSetup.Delay);

        rs232putchar ('\n');
        }
      }
    }
  }

/**************************************************************************/
/* GenlockGenSystem                                               RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    000525                                                       */
/*                                                                        */
/* Function:  Handle the genlock system command                            */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void GenlockGenSystem(void)
  {
  int system;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = GenlockSystemPar[GenlockSetup.System].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        system = FindParameter(Cmd, GenlockSystemPar);
  
        if (system > -1)
          SetGenlockSystem(system);
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* GenlockGenDelay                                               RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000525                                    */
/* Revised:    000525                                                       */
/*                                                                        */
/* Function:  Handle the genlock timing command                            */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void GenlockGenDelay(void)
  {
  int field, line;
  float offset;
  long samples;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        PrintSamplesToFLT(GenlockSetup.System, GenlockSetup.Delay);

        rs232putchar ('\n');
        }
      }
    else
      {
      if ((ParCnt != 3) || (sscanf(Cmd, "%d,%d,%f", &field, &line, &offset) != 3))
        SCPIError = SyntaxErr;
      else
        {
        if ((samples = FLTToSamples(GenlockSetup.System, field, line, offset)) == -1)
          SCPIError = ExecErr;
        else
          SetGenlockDelay(samples);
        }
      }
    }
  }

/**************************************************************************/
/* GenlockGenCalibrateEnable                                     RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000525                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  Handle the genlock calibration                              */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void GenlockGenCalibrateEnable(void)
  {
  char code *str;
  int tmp;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        str = OffOnPar[GenlockCalEnable].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        tmp = FindParameter(Cmd, OffOnPar);
  
        if (tmp > -1)
          {
          GenlockCalEnable = tmp;

          if (tmp == 0)
            {
            SetGenlockSystem(GenlockSetup.System);

            SetBBSystem(BBOutput1, BBSetup[BBOutput1].System);

            BBControlPort = 0;
            }
          else
            {
                                              // Force genlock internal mode
            sio0SndCommand(12, SPG_V24_address, "GM0");

                                              // Force BB1 PAL mode
            sio0SndCommand(20, BB_V24_address, "HG%ld;", BBSetup[BBOutput1].Delay);

            BBControlPort = 1;                // Set BB1 to F45MHz

            GenlockCalValue = Calibration.GenlockCalibration.DACUserValue;

            sio0SndCommand(20, SPG_V24_address, "GX%d;", GenlockCalValue);
            }
          }
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* GenlockGenCalibrateAdjust                                      RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000525                                    */
/* Revised:    001016                                                       */
/*                                                                        */
/* Function:  Handle the genlock calibration                              */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void GenlockGenCalibrateAdjust(void)
  {
  int tmp;
  UI MinVal, MaxVal;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else
      {
      if (ParCnt == 0)
        SCPIError = SyntaxErr;
      else
        {
        MinVal = Calibration.GenlockCalibration.DACFactValue - 150;
        MaxVal = Calibration.GenlockCalibration.DACFactValue + 150;

        tmp = FindParameter(Cmd, MinMaxPar);
  
        if (tmp > -1)
          {
          if (tmp == 0)
            GenlockCalValue = MaxVal;
          else
            GenlockCalValue = MinVal;

          sio0SndCommand(20, SPG_V24_address, "GX%u;", GenlockCalValue);  
          }
        else
          {
          tmp = FindParameter(Cmd, UpDownPar);

          if (tmp > -1)
            {
            if (tmp == 0)
              {
              if (GenlockCalValue > MinVal)
                GenlockCalValue--;
              }
            else
              {
              if (GenlockCalValue < MaxVal)
                GenlockCalValue++;
              }

            sio0SndCommand(20, SPG_V24_address, "GX%u;", GenlockCalValue);
            }
          else
            SCPIError = DataOutOfRange;
          }  
        }
      }
    }
  }

/**************************************************************************/
/* GenlockGenCalibrateStore                                      RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000525                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  Handle the genlock calibration                              */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void GenlockGenCalibrateStore(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
        SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        if (GenlockCalEnable)
          {
          Calibration.GenlockCalibration.DACUserValue = GenlockCalValue;
              
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        else
          SCPIError = ExecErr;          
        }
      }
    }
  }

/**************************************************************************/
/* BBSuffix                                                       RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    000604                                                       */
/*                                                                        */
/* Function:  Get suffix of the Black Burst generator                      */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    Suffix                                                       */
/**************************************************************************/
void BBGenSuffix(void)
  {
  if (NumSuffix > 3)
    SCPIError = HdrSuffixOutOfRange;
  else
    Suffix[0] = NumSuffix-1;

  }

/**************************************************************************/
/* BBGen                                                        RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000524                                    */
/* Revised:    000604                                                       */
/*                                                                        */
/* Function:  Handle the Black Burst command.                              */
/* Remarks:    Request only                                                */
/* Returns:    --                                                          */
/* Updates:    --                                                           */
/**************************************************************************/
void BBGen(void)
  {
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (!CmdRequest)
        SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = SystemPar[BBSetup[Suffix[0]].System].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar(',');

        PrintSamplesToFLT(BBSetup[Suffix[0]].System, BBSetup[Suffix[0]].Delay);

        rs232printf(6, ",%d\n", BBSetup[Suffix[0]].ScHPhase);
        }
      }
    }
  }

/**************************************************************************/
/* BBGenSystem                                                   RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000524                                    */
/* Revised:    000604                                                       */
/*                                                                        */
/* Function:  Handle the Black Burst system-command.                      */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void BBGenSystem(void)
  {
  int system;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = SystemPar[BBSetup[Suffix[0]].System].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        system = FindParameter(Cmd, SystemPar);

        if (system > -1)
          SetBBSystem(Suffix[0], system);
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* BBGenDelay                                                     RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000524                                    */
/* Revised:    000604                                                       */        
/*                                                                        */
/* Function:  Handle the Black Burst timing command.                      */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void BBGenDelay(void)
  {
  int field, line;
  float offset;
  long samples;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        PrintSamplesToFLT(BBSetup[Suffix[0]].System, BBSetup[Suffix[0]].Delay);

        rs232putchar ('\n');
        }
      }
    else
      {
      if ((ParCnt != 3) || (sscanf(Cmd, "%d,%d,%f", &field, &line, &offset) != 3))
        SCPIError = SyntaxErr;
      else
        {
        if ((samples = FLTToSamples(BBSetup[Suffix[0]].System, field, line, offset)) == -1)
          SCPIError = ExecErr;
        else
          SetBBDelay(Suffix[0], (UL) samples);
        }
      }
    }
  }

/**************************************************************************/
/* BBGenScHPhase                                                 RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000524                                    */      
/* Revised:    000604                                                       */
/*                                                                        */
/* Function:  Handle the Black Burst ScH-Phase command.                    */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void BBGenScHPhase(void)
  {
  int schphase;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(6, "%d\n", BBSetup[Suffix[0]].ScHPhase);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &schphase) != 1))
        SCPIError = SyntaxErr;
      else
        {
        if ((schphase < ScHPhaseLow) || (schphase > ScHPhaseHigh))
          SCPIError = DataOutOfRange;
        else
          SetBBScHPhase(Suffix[0], schphase);
        }
      }
    }
  }

/**************************************************************************/
/* TSGen                                                        RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000524                                    */
/* Revised:    000727                                                       */
/*                                                                        */
/* Function:  Return the setting of a Analog-TPG.                          */
/* Remarks:    Request only.                                                */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/***************************************************************************/
void TSGen(void)
  {
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (!CmdRequest)
        SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = CBPatternPar[CBSetup.Pattern].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar(',');


        str = SystemPar[CBSetup.System].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar(',');


        PrintSamplesToFLT(CBSetup.System, CBSetup.Delay*256);


        rs232printf(6, ",%d,", CBSetup.ScHPhase);


        str = EmbAudioSignalPar[CBSetup.EmbAudioSignal].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    }
  }

/**************************************************************************/
/* TSGenPattern                                                  RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    000727                                                       */
/*                                                                        */
/* Function:  Handle the Colorbar pattern command.                        */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void TSGenPattern(void)
  {
  int pattern;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = CBPatternPar[CBSetup.Pattern].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      pattern = FindParameter(Cmd, CBPatternPar);

      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        if (pattern > -1)
          {
          if (SetCBPattern(pattern) != OK)
            SCPIError = ExecErr;
          }
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* TSGenSystem                                                  RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 98021                                    */
/* Revised:    000727                                                       */
/*                                                                        */
/* Function:  Handle the Colobar system-command.                          */
/* Remarks:    This command will possibly change pattern and/or delay if     */
/*            these are not valid in the selected system                  */
/* Returns:    --                                                          */
/* Updates:     --                                                          */
/**************************************************************************/
void TSGenSystem(void)
  {
  int system;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = SystemPar[CBSetup.System].LongName;

        while (*str)                     
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        system = FindParameter(Cmd, SystemPar);

        if (system == PAL_PALID)            // If line7, (PAL w/ID) ever should
          SCPIError = DataOutOfRange;        //  be supported remove this exception
        else            
          {
          if (system > -1)
            SetCBSystem(system);
          else
            SCPIError = DataOutOfRange;
          }
        }
      }
    }
  }

/**************************************************************************/
/* TSGenDelay                                                    RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    000604                                                       */
/*                                                                        */
/* Function:  Handle the Colorbar delay command.                          */
/* Remarks:    Delay should be multiplied/divided with/by 256 to conform    */
/*            with the black burst timing as the functions                */
/*            PrintSamplesToFLT/FLTToSamples are common to both the CBar  */
/*            and BB timing. The CBar timing equals 1728/1716 samples      */
/*            (app. 37ns/sample), while the BB timing equals 1728*256/    */
/*            1716*256 samples (app. 0.14 ns/sample).                      */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void TSGenDelay(void)
  {
  int field, line;
  float offset;
  long samples;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        PrintSamplesToFLT(CBSetup.System, CBSetup.Delay*256);

        rs232putchar ('\n');
        }
      }
    else
      {
      if ((ParCnt != 3) || (sscanf(Cmd, "%d,%d,%f", &field, &line, &offset) != 3))
        SCPIError = SyntaxErr;
      else
        {
        if ((samples = FLTToSamples(CBSetup.System, field, line, offset)) == -1)
          SCPIError = ExecErr;
        else    
          SetCBDelay(samples/256);
        }
      }
    }
  }

/**************************************************************************/
/* TSGenScHPhase                                                 RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    000727                                                       */
/*                                                                        */
/* Function:  Handle the Colorbar ScH-Phase-command.                      */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void TSGenScHPhase(void)
  {
  int schphase;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(6, "%d\n", CBSetup.ScHPhase);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &schphase) != 1))
        SCPIError = SyntaxErr;
      else
        {
        if ((schphase < ScHPhaseLow) || (schphase > ScHPhaseHigh))
          SCPIError = DataOutOfRange;
        else
          SetCBScHPhase(schphase);
        }
      }
    }
  }

/**************************************************************************/
/* TSGenEmbAudioSignal                                           RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 980211                                    */
/* Revised:    000727                                                       */
/*                                                                        */
/* Function:  Handle the Colorbar embedded audio signal                    */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void TSGenEmbAudioSignal(void)
  {
  int signal;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = EmbAudioSignalPar[CBSetup.EmbAudioSignal].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        signal = FindParameter(Cmd, EmbAudioSignalPar);
  
        if (signal > -1)
          SetCBEmbAudioSignal(signal);
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenOutput                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000603                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenOutput(void)
  {
  int output;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = AudioOutputPar[AudioOutput].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        output = FindParameter(Cmd, AudioOutputPar);
  
        if (output > -1)
          {
          if (SetAudioOutput(output) == FAIL)
            SCPIError = ExecErr;
          }
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAnalog                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000526                                    */
/* Revised:    000705                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    Request only.                                                */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAnalog(void)
  {
  char code *str;
  int level;

  if (CmdExecute && !SCPIError)
    {
    if (!CmdRequest)
        SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = AudioSignalPar[AudioSetup[AnalogAudio].Signal].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar(',');

        str = NULL;

        switch (AudioSetup[AnalogAudio].Level)
          {
           case P10dBm:
            level = 10;
            break;

           case P8dBm:
            level = 8;
            break;

           case P7dBm:
            level = 7;
            break;

           case P6dBm:
            level = 6;
            break;

           case P5dBm:
            level = 5;
            break;

           case P4dBm:
            level = 4;
            break;

           case P3dBm:
            level = 3;
            break;

           case P2dBm:
            level = 2;
            break;

           case P1dBm:
            level = 1;
            break;

           case M0dBm:
            level = 0;
            break;

           case M1dBm:
            level = -1;
            break;

           case M2dBm:
            level = -2;
            break;

           case M3dBm:
            level = -3;
            break;

           case M4dBm:
            level = -4;
            break;

           case M5dBm:
            level = -5;
            break;

           case M6dBm:
            level = -6;
            break;

           case M7dBm:
            level = -7;
            break;

           case M8dBm:
            level = -8;
            break;

           case M9dBm:
            level = -9;
            break;

           case M10dBm:
            level = -10;
            break;

           case M11dBm:
            level = -11;
            break;

           case M12dBm:
            level = -12;
            break;

           case M15dBm:
            level = -15;
            break;

           case M18dBm:
            level = -18;
            break;

          case M21dBm:
            level = -21;
            break;

           case M24dBm:
            level = -24;
            break;

           case M27dBm:
            level = -27;
            break;

           case M30dBm:
            level = -30;
            break;

           case M33dBm:
            level = -33;
            break;

          case M36dBm:
            level = -36;
            break;

          default:
            str = SILENCEParTxt;

            while (*str)
              rs232putchar(*str++);

            rs232putchar(',');
            break;
            }

        if (str == NULL)
          rs232printf(6, "%d,", level);

        rs232printf(6, "%d\n", AudioSetup[AnalogAudio].Click);      
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAnalogSignal                                           RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000603                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAnalogSignal(void)
  {
  int signal;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = AudioSignalPar[AudioSetup[AnalogAudio].Signal].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        signal = FindParameter(Cmd, AudioSignalPar);
  
        if (signal > -1)
          {
          if (SetAudioSignal(AnalogAudio, signal) == FAIL)
              SCPIError = ExecErr;
          }
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAnalogLevel                                          RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000705                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAnalogLevel(void)
  {
  code char *str;
  int level;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        switch (AudioSetup[AnalogAudio].Level)
          {
           case P10dBm:
            level = 10;
            break;

           case P8dBm:
            level = 8;
            break;

           case P7dBm:
            level = 7;
            break;

           case P6dBm:
            level = 6;
            break;

           case P5dBm:
            level = 5;
            break;

           case P4dBm:
            level = 4;
            break;

           case P3dBm:
            level = 3;
            break;

           case P2dBm:
            level = 2;
            break;

           case P1dBm:
            level = 1;
            break;

           case M0dBm:
            level = 0;
            break;

           case M1dBm:
            level = -1;
            break;

           case M2dBm:
            level = -2;
            break;

           case M3dBm:
            level = -3;
            break;

           case M4dBm:
            level = -4;
            break;

           case M5dBm:
            level = -5;
            break;

           case M6dBm:
            level = -6;
            break;

           case M7dBm:
            level = -7;
            break;

           case M8dBm:
            level = -8;
            break;

           case M9dBm:
            level = -9;
            break;

           case M10dBm:
            level = -10;
            break;

           case M11dBm:
            level = -11;
            break;

           case M12dBm:
            level = -12;
            break;

           case M15dBm:
            level = -15;
            break;

           case M18dBm:
            level = -18;
            break;

          case M21dBm:
            level = -21;
            break;

           case M24dBm:
            level = -24;
            break;

           case M27dBm:
            level = -27;
            break;

           case M30dBm:
            level = -30;
            break;

           case M33dBm:
            level = -33;
            break;

          case M36dBm:
            level = -36;
            break;

          default:
            str = SILENCEParTxt;

            while (*str)
              rs232putchar(*str++);

            rs232putchar('\n');
            return;
            }

        rs232printf(6, "%d\n", level);
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        level = FindParameter(Cmd, AnalogLevelPar);
  
        if (level > -1)
          level = AnalogSilence;
        else
          {
          if ((ParCnt != 1) || (sscanf(Cmd, "%d", &level) != 1))
            SCPIError = SyntaxErr;
          else
            {
            switch (level)
              {
               case 10:
                level = P10dBm;
                break;

               case 8:
                level = P8dBm;
                break;

               case 7:
                level = P7dBm;
                break;

               case 6:
                level = P6dBm;
                break;

               case 5:
                level = P5dBm;
                break;

               case 4:
                level = P4dBm;
                break;

               case 3:
                level = P3dBm;
                break;

               case 2:
                level = P2dBm;
                break;

               case 1:
                level = P1dBm;
                break;

               case 0:
                level = M0dBm;
                break;

               case -1:
                level = M1dBm;
                break;

               case -2:
                level = M2dBm;
                break;

               case -3:
                level = M3dBm;
                break;

               case -4:
                level = M4dBm;
                break;

               case -5:
                level = M5dBm;
                break;

               case -6:
                level = M6dBm;
                break;

               case -7:
                level = M7dBm;
                break;

               case -8:
                level = M8dBm;
                break;

               case -9:
                level = M9dBm;
                break;

               case -10:
                level = M10dBm;
                break;

               case -11:
                level = M11dBm;
                break;

               case -12:
                level = M12dBm;
                break;

               case -15:
                level = M15dBm;
                break;

               case -18:
                level = M18dBm;
                break;

              case -21:
                level = M21dBm;
                break;

               case -24:
                level = M24dBm;
                break;

               case -27:
                level = M27dBm;
                break;

               case -30:
                level = M30dBm;
                break;

               case -33:
                level = M33dBm;
                break;

              case -36:
                level = M36dBm;
                break;

              default:
                SCPIError = DataOutOfRange;
                return;
              }
            }
          }

        if (SetAudioLevel(AnalogAudio, level) == FAIL)
          SCPIError = ExecErr;
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAnalogClick                                           RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000603                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAnalogClick(void)
  {
  int click;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(6, "%d\n", AudioSetup[AnalogAudio].Click);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &click) != 1))
        SCPIError = SyntaxErr;
      else   {
        if ((click != click1s) && (click != click3s))
          SCPIError = DataOutOfRange;
        else
          {
          if (SetAudioClick(AnalogAudio, click) == FAIL)
            SCPIError = ExecErr;
          }
        }
      }
    }
  }


/**************************************************************************/
/* AudioGenAESEBU                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000526                                    */
/* Revised:    000617                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    Request only.                                                */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAESEBU(void)
  {
  char code *str;
  int level;

  if (CmdExecute && !SCPIError)
    {
    if (!CmdRequest)
        SCPIError = SyntaxErr;
    else
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = AESEBUSystemPar[AudioSetup[AESEBUAudio].System].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar(',');

        str = AudioSignalPar[AudioSetup[AESEBUAudio].Signal].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar(',');

        str = NULL;

        switch (AudioSetup[AESEBUAudio].Level)
          {
          case M0dBFS:
            level = 0;
            break;

          case M9dBFS:
            level = -9;
            break;

          case M12dBFS:
            level = -12;
            break;

          case M15dBFS:
            level = -15;
            break;

          case M16dBFS:
            level = -16;
            break;

          case M18dBFS:
            level = -18;
            break;

          case M21dBFS:
            level = -21;
            break;

          default:
            str = SILENCEParTxt;

            while (*str)
              rs232putchar(*str++);

            rs232putchar(',');
            break;
            }

        if (str == NULL)
          rs232printf(6, "%d,", level);

        rs232printf(6, "%.1f,", AudioSetup[AESEBUAudio].Timing);

        str = AudioFrequencyPar[AudioSetup[AESEBUAudio].Frequency].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar(',');

        rs232printf(6, "%d\n", AudioSetup[AESEBUAudio].Click);
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAESEBUSystem                                          RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000603                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAESEBUSystem(void)
  {
  int system;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = AESEBUSystemPar[AudioSetup[AESEBUAudio].System].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        system = FindParameter(Cmd, AESEBUSystemPar);
  
        if (system > -1)
          {
          if (SetAudioSystem(AESEBUAudio, system) == FAIL)
            SCPIError = ExecErr;
          }
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAESEBUSignal                                           RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000603                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAESEBUSignal(void)
  {
  int signal;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = AudioSignalPar[AudioSetup[AESEBUAudio].Signal].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        signal = FindParameter(Cmd, AudioSignalPar);
  
        if (signal > -1)
          {
          if (SetAudioSignal(AESEBUAudio, signal) == FAIL)
            SCPIError = ExecErr;
          }
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAESEBULevel                                           RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    0000628                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAESEBULevel(void)
  {
  code char *str;
  int level;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        switch (AudioSetup[AESEBUAudio].Level)
          {
          case M0dBFS:
            level = 0;
            break;

          case M9dBFS:
            level = -9;
            break;

          case M12dBFS:
            level = -12;
            break;

          case M15dBFS:
            level = -15;
            break;

          case M16dBFS:
            level = -16;
            break;

          case M18dBFS:
            level = -18;
            break;

          case M21dBFS:
            level = -21;
            break;

          default:
            str = SILENCEParTxt;

            while (*str)
              rs232putchar(*str++);

            rs232putchar('\n');
            return;
            }

        rs232printf(6, "%d\n", level);
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        level = FindParameter(Cmd, AESEBULevelPar);
  
        if (level > -1)
          level = AESEBUSilence;
        else
          {
          if ((ParCnt != 1) || (sscanf(Cmd, "%d", &level) != 1))
            SCPIError = SyntaxErr;
          else   {
            switch (level)
              {
              case 0:
                level = M0dBFS;
                break;

               case -9:
                level = M9dBFS;
                break;

               case -12:
                level = M12dBFS;
                break;

               case -15:
                level = M15dBFS;
                break;

               case -16:
                level = M16dBFS;
                break;

               case -18:
                level = M18dBFS;
                break;

               case -21:
                level = M21dBFS;
                break;

              default:
                SCPIError = DataOutOfRange;
                return;
              }
            }
          }

        if (SetAudioLevel(AESEBUAudio, level) == FAIL)
          SCPIError = ExecErr;
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAESEBUTiming                                           RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000616                                    */
/* Revised:    000616                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAESEBUTiming(void)
  {
  float timing;
  int tmp;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(6, "%.1f\n", AudioSetup[AESEBUAudio].Timing);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%f", &timing) != 1))
        SCPIError = SyntaxErr;
      else
        {
        tmp = (int)((timing / 0.8) + 0.5 - (1*(timing < 0)));

        if ((tmp < -12) || (tmp > 13))
          SCPIError = DataOutOfRange;
        else
          {
          if (SetAudioTiming(AESEBUAudio, timing) == FAIL)
            SCPIError = ExecErr;
          }
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAESEBUWordClock                                       RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000603                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAESEBUWordClock(void)
  {
  int frequency;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = AudioFrequencyPar[AudioSetup[AESEBUAudio].Frequency].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        frequency = FindParameter(Cmd, AudioFrequencyPar);
  
        if (frequency > -1)
          {
          if (SetAudioWordClock(AESEBUAudio, frequency) == FAIL)
            SCPIError = ExecErr;
          }
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* AudioGenAESEBUClick                                           RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000603                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void AudioGenAESEBUClick(void)
  {
  int click;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        rs232printf(6, "%d\n", AudioSetup[AESEBUAudio].Click);
      }
    else
      {
      if ((ParCnt != 1) || (sscanf(Cmd, "%d", &click) != 1))
        SCPIError = SyntaxErr;
      else
        {
        if ((click != click1s) && (click != click3s))
          SCPIError = DataOutOfRange;
        else
          {
          if (SetAudioClick(AESEBUAudio, click) == FAIL)
            SCPIError = ExecErr;
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactPassword                                                  RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactPassword(void)
  {
  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        if (PasswordProtected)
          rs232puts("ON\n");
        else
          rs232puts("OFF\n");
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        if (!strcmp(Cmd, "ON"))
          PasswordProtected = true;
        else
          if (!Cstrcmp(Cmd, PasswordText))
            PasswordProtected = false;
        else
          SCPIError = IllegalParValue;
        }
      }
    }
  }

/**************************************************************************/
/* FactReset                                                    RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000617                                    */
/* Revised:    000914                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactReset(void)
  {
  int i;

  if (!PasswordProtected)
    {
    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
          SCPIError = SyntaxErr;
      else
        {
        if (ParCnt)
          SCPIError = SyntaxErr;
        else
          {
          GenlockDefaults();        // 
          BBDefaults();               // 
          CBDefaults();               //
          AudioDefaults();           //

          GenlockInit();              // init the genlock setup
          BBInit();                   // init the black burst setups
          CBInit();                   // init the colorbar generator (PAL,EBU ColorBar)
          AudioInit();               // init the audio setup, (this includes an AudioTimer)        

          GenlockUpdate();            // update the genlock setup.
          CBUpdate();                 // update the colorbar generator (PAL,EBU ColorBar)
          BBUpdate();                 // update the black burst setups
          AudioUpdate();             // update the audio setup

          for (i = 1; i <= NoOfPresets; i++)
            StorePreset(i);

          Settings.ActivePreset = 0;
          NV_Store(&Settings, SettingsPtr, sizeof(Settings));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactV24Command                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000402                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactV24Command(void)
  {
  UC address;
  char command[30], result[30];;

  if (!PasswordProtected)
    {
    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        SCPIError = SyntaxErr;
      else
        {
        if ((ParCnt != 2) || (sscanf(Cmd, "%d,%s", &address, command) != 2))
          SCPIError = SyntaxErr;
        else
          {
          if (command[2] == '?')
            {
            if (sio0SndRequest(12, address, result, "%s", command) == OK)
              rs232puts(result);
            else
              SCPIError = ExecErr;
            }
          else
            {
            if (sio0SndCommand(20, address, "%s;", command) == FAIL)
              SCPIError = ExecErr;
            }
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactV24Transparent                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000407                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactV24Transparent(void)
  {
  int unit;

  if (!PasswordProtected)
    {
    if (CmdExecute && !SCPIError)
      {
      if (CmdRequest)
        SCPIError = SyntaxErr;
      else
        {
        unit = FindParameter(Cmd, UnitPar);
  
        if (unit > -1)
          BBSPGCommunication(unit);
        else
          SCPIError = DataOutOfRange;
        }
      }
    }
  }

/**************************************************************************/
/* FactChecksumCode                                              RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000705                                    */
/* Revised:    000705                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactChecksumCode(void)
  {
  char buffer[30];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {    
        sprintf(buffer, "%u\n", ROMChecksum(StartOfPROMCode, StartOfPROMCode+SizeOfPROMCode-1));
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactChecksumPLD                                              RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000705                                    */
/* Revised:    000705                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactChecksumPLD(void)
  {
  char buffer[30];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {    
        sprintf(buffer, "%u\n", ROMChecksum(StartOfPLDCode, StartOfPLDCode+SizeOfPLDCode-1));
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactChecksumEmbAudio                                          RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000705                                    */
/* Revised:    000705                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactChecksumEmbAudio(void)
  {
  char buffer[30];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {    
        sprintf(buffer, "%u\n", ROMChecksum(StartOfEmbAudioCode, StartOfEmbAudioCode+SizeOfEmbAudioCode-1));
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactChecksumPattern                                          RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000705                                    */
/* Revised:    000705                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactChecksumPattern(void)
  {
  char buffer[30];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {    
        sprintf(buffer, "%u\n", ROMChecksum(StartOfCBVideoCode, StartOfCBVideoCode+SizeOfCBVideoCode-1));
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactChecksumRange                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000705                                    */
/* Revised:    000705                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactChecksumRange(void)
  {
  UL StartAddr, EndAddr;

  char buffer[30];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if ((ParCnt != 2) || ((sscanf(Cmd, "%lu,%lu", &StartAddr, &EndAddr)) != 2))
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u\n", ROMChecksum(StartAddr, EndAddr));
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactMainVersion                                              RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000620                                    */
/* Revised:    000620                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMainVersion(void)
  {
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u\n", MasterSWVersion);
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactMainIDNCompany                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000407                                    */
/* Revised:    000616                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMainIDNCompany(void)
  {
  char Company[33];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        rs232puts(Calibration.Company);
        rs232putchar('\n');
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 1) || (sscanf(Cmd, "%32s", Company) != 1))
          SCPIError = SyntaxErr;
        else
          {
          strncpy(Calibration.Company, Company, 32);

           NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactMainIDNType                                              RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000407                                    */
/* Revised:    000616                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMainIDNType(void)
  {
  char Type[33];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        rs232puts(Calibration.Type);
        rs232putchar('\n');
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 1) || (sscanf(Cmd, "%32s", Type) != 1))
          SCPIError = SyntaxErr;
        else
          {
          strncpy(Calibration.Type, Type, 32);

           NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactMainIDNNumber                                          RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000407                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMainIDNKUNumber(void)
  {
  char KUNumber[9];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        rs232puts(Calibration.KUNumber);
        rs232putchar('\n');
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 1) || (sscanf(Cmd, "%8s", KUNumber) != 1))
          SCPIError = SyntaxErr;
        else
          {
          strncpy(Calibration.KUNumber, KUNumber, 9);

           NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactMainIDNSWRevision                                        RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000407                                    */
/* Revised:    000616                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMainIDNSWRevision(void)
  {
  char SWRevision[9];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        rs232puts(Calibration.SWRevision);
        rs232putchar('\n');
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 1) || (sscanf(Cmd, "%8s", SWRevision) != 1))
          SCPIError = SyntaxErr;
        else
          {
          strncpy(Calibration.SWRevision, SWRevision, 8);

           NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactMainSystem                                                 RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000617                                    */
/* Revised:    000617                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMainSystem(void)
  {
  int system;
  char code *str;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = ParNotAllowed;
      else
        {
        str = SystemPar[Calibration.ResetSystem].LongName;

        while (*str)
          rs232putchar(*str++);

        rs232putchar('\n');
        }
      }
    else
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        system = FindParameter(Cmd, SystemPar);

        if (system == PAL_PALID)            // If line7, (PAL w/ID), ever should
          SCPIError = DataOutOfRange;        //  be supported remove this exception
        else            
          {  
          if (system > -1)
            {
            Calibration.ResetSystem = system;

             NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
            }
          else
            SCPIError = DataOutOfRange;
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactMainFMSDateProd                                          RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000407                                    */
/* Revised:    0006014                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMainFMSDateProd(void)
  {
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        day = Calibration.ProdDate % 100;
        month = (Calibration.ProdDate / 100) % 100;
        year = (Calibration.ProdDate / 10000);

        sprintf(buffer, "%d,%d,%d\n", year, month, day);

        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 3) || ((sscanf(Cmd, "%d,%d,%d", &year, &month, &day)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          if (ValidateDate(year, month, day) == FAIL)
            SCPIError = ExecErr;
          else
            {
            Calibration.ProdDate = day + 100*month + 10000*year;
          
             NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
            }
          }        
        }
      }
    }
  }

/**************************************************************************/
/* FactMainFMSDateUpd                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000407                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMainFMSDateUpd(void)
  {
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        day = Calibration.UpdateDate % 100;
        month = (Calibration.UpdateDate / 100) % 100;
        year = (Calibration.UpdateDate / 10000);

        sprintf(buffer, "%d,%d,%d\n", year, month, day);

        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 3) || ((sscanf(Cmd, "%d,%d,%d", &year, &month, &day)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          if (ValidateDate(year, month, day) == FAIL)
            SCPIError = ExecErr;
          else
            {
            Calibration.UpdateDate = day + 100*month + 10000*year;
          
             NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
            }
          }        
        }
      }
    }
  }

/**************************************************************************/
/* FactGenlockGenDate                                             RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactGenlockGenDate(void)
  {
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        day = Calibration.GenlockCalibration.Date % 100;
        month = (Calibration.GenlockCalibration.Date / 100) % 100;
        year = (Calibration.GenlockCalibration.Date / 10000);

        sprintf(buffer, "%d,%d,%d\n", year, month, day);

        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 3) || ((sscanf(Cmd, "%d,%d,%d", &year, &month, &day)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          if (ValidateDate(year, month, day) == FAIL)
            SCPIError = ExecErr;
          else
            {
            Calibration.GenlockCalibration.Date = day + 100*month + 10000*year;
          
             NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
            }
          }        
        }
      }
    }
  }

/**************************************************************************/
/* FactGenlockGenVersion                                         RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000504                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactGenlockGenVersion(void)
  {
  char result[10];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        if (GenlockVersion(result) == OK)
          {
          rs232puts(result);
          rs232putchar('\n');
          }
        else
          SCPIError = ExecErr;      
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactGenlockGenFRefDate                                        RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000504                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactGenlockGenFRefDate(void)
  {
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        day = Calibration.GenlockCalibration.DACFactDate % 100;
        month = (Calibration.GenlockCalibration.DACFactDate / 100) % 100;
        year = (Calibration.GenlockCalibration.DACFactDate / 10000);

        sprintf(buffer, "%u,%u,%u\n", year, month, day);

        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 3) || ((sscanf(Cmd, "%u,%u,%u", &year, &month, &day)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          if (ValidateDate(year, month, day) == FAIL)
            SCPIError = ExecErr;
          else
            {
            Calibration.GenlockCalibration.DACFactDate = day + 100*month + 10000*year;
          
             NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
            }
          }        
        }
      }
    }
  }

/**************************************************************************/
/* FactGenlockGenFRefDAC                                         RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000504                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactGenlockGenFRefDAC(void)
  {
  char buffer[20];
  UI DACValue;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u\n", Calibration.GenlockCalibration.DACFactValue);

        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 1) || ((sscanf(Cmd, "%u", &DACValue)) != 1))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.GenlockCalibration.DACFactValue = DACValue;
              
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactGenlockGenURefDate                                        RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000504                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactGenlockGenURefDate(void)
  {
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        day = Calibration.GenlockCalibration.DACUserDate % 100;
        month = (Calibration.GenlockCalibration.DACUserDate / 100) % 100;
        year = (Calibration.GenlockCalibration.DACUserDate / 10000);

        sprintf(buffer, "%u,%u,%u\n", year, month, day);

        rs232puts(buffer);
        }
      }
    else
      {
      if ((ParCnt != 3) || ((sscanf(Cmd, "%u,%u,%u", &year, &month, &day)) != 3))
        SCPIError = SyntaxErr;
      else
        {
        if (ValidateDate(year, month, day) == FAIL)
          SCPIError = ExecErr;
        else
          {
          Calibration.GenlockCalibration.DACUserDate = day + 100*month + 10000*year;
          
           NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }        
        }
      }
    }
  }

/**************************************************************************/
/* FactGenlockGenURefDAC                                        RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000504                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactGenlockGenURefDAC(void)
  {
  char buffer[20];
  UI DACValue;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u\n", Calibration.GenlockCalibration.DACUserValue);

        rs232puts(buffer);
        }
      }
    else
      {
      if ((ParCnt != 1) || ((sscanf(Cmd, "%u", &DACValue)) != 1))
        SCPIError = SyntaxErr;
      else
        {
        Calibration.GenlockCalibration.DACUserValue = DACValue;
              
        NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
        }
      }
    }
  }

/**************************************************************************/
/* FactGenlockGenHPHZero                                         RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactGenlockGenHPHZero(void)
  {
  char buffer[30];
  UL HPHZeroG, HPHZeroM;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%lu,%lu\n", Calibration.GenlockCalibration.HPHZeroG, 
                                      Calibration.GenlockCalibration.HPHZeroM);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%lu,%lu", &HPHZeroG, &HPHZeroM)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.GenlockCalibration.HPHZeroG = HPHZeroG;
          Calibration.GenlockCalibration.HPHZeroM = HPHZeroM;
        
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactGenlockGenPhase                                           RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactGenlockGenPhase(void)
  {
  char buffer[30];
  UL PhaseG, PhaseM;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%lu,%lu\n", Calibration.GenlockCalibration.PhaseG, 
                                      Calibration.GenlockCalibration.PhaseM);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%lu,%lu", &PhaseG, &PhaseM)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.GenlockCalibration.PhaseG = PhaseG;
          Calibration.GenlockCalibration.PhaseM = PhaseM;
        
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactBBGenDate                                                 RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactBBGenDate(void)
  {
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        day = Calibration.BBCalibration[Suffix[0]].Date % 100;
        month = (Calibration.BBCalibration[Suffix[0]].Date / 100) % 100;
        year = (Calibration.BBCalibration[Suffix[0]].Date / 10000);

        sprintf(buffer, "%u,%u,%u\n", year, month, day);

        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 3) || ((sscanf(Cmd, "%u,%u,%u", &year, &month, &day)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          if (ValidateDate(year, month, day) == FAIL)
            SCPIError = ExecErr;
          else
            {
            Calibration.BBCalibration[Suffix[0]].Date = day + 100*month + 10000*year;
          
             NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
            }
          }        
        }
      }
    }
  }

/**************************************************************************/
/* FactBBGenVersion                                              RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000504                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactBBGenVersion(void)
  {
  char result[10];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt || Suffix[0])
        SCPIError = SyntaxErr;
      else
        {
        if (BBVersion(result) == OK)
          {
          rs232puts(result);
          rs232putchar('\n');
          }
        else
          SCPIError = ExecErr;      
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactBBGenSignal                                              RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000503                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactBBGenSignal(void)
  {
  int signal;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      SCPIError = SyntaxErr;
    else
      {
      if (!PasswordProtected)
        {
        if (ParCnt != 1)
          SCPIError = SyntaxErr;
        else
          {
          signal = FindParameter(Cmd, FactBBGenSignalPar);
  
          switch (signal)
            {
            case 0:
              BBControlPort = 0;
              break;

            case 1:
              BBControlPort = 1;
              break;

            default:
              SCPIError = DataOutOfRange;
              break;
            }
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactBBGenPhase                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactBBGenPhase(void)
  {
  char buffer[30];
  UL PhaseG, PhaseM;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%lu,%lu\n", Calibration.BBCalibration[Suffix[0]].PhaseG, 
                                      Calibration.BBCalibration[Suffix[0]].PhaseM);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%lu,%lu", &PhaseG, &PhaseM)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.BBCalibration[Suffix[0]].PhaseG = PhaseG;
          Calibration.BBCalibration[Suffix[0]].PhaseM = PhaseM;
        
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactBBGenScHPhase                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactBBGenScHPhase(void)
  {
  char buffer[30];
  UI ScHPhaseG, ScHPhaseM;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u,%u\n", Calibration.BBCalibration[Suffix[0]].ScHPhaseG, 
                                    Calibration.BBCalibration[Suffix[0]].ScHPhaseM);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%u,%u", &ScHPhaseG, &ScHPhaseM)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.BBCalibration[Suffix[0]].ScHPhaseG = ScHPhaseG;
          Calibration.BBCalibration[Suffix[0]].ScHPhaseM = ScHPhaseM;
        
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactBBGenDAC                                                  RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactBBGenDAC(void)
  {
  char buffer[30];
  UI DACGain, DACOffset, DACLevel;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u,%u,%u\n", Calibration.BBCalibration[Suffix[0]].DACGain, 
                                       Calibration.BBCalibration[Suffix[0]].DACOffset,
                                       Calibration.BBCalibration[Suffix[0]].DACLevel);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 3) || ((sscanf(Cmd, "%u,%u,%u", &DACGain, &DACOffset, &DACLevel)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.BBCalibration[Suffix[0]].DACGain = DACGain;
          Calibration.BBCalibration[Suffix[0]].DACOffset = DACOffset;
          Calibration.BBCalibration[Suffix[0]].DACLevel = DACLevel;  
              
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactTSGenDate                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000727                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactTSGenDate(void)
  {
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        day = Calibration.CBCalibration.Date % 100;
        month = (Calibration.CBCalibration.Date / 100) % 100;
        year = (Calibration.CBCalibration.Date / 10000);

        sprintf(buffer, "%u,%u,%u\n", year, month, day);

        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 3) || ((sscanf(Cmd, "%u,%u,%u", &year, &month, &day)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          if (ValidateDate(year, month, day) == FAIL)
            SCPIError = ExecErr;
          else
            {
            Calibration.CBCalibration.Date = day + 100*month + 10000*year;
          
             NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
            }
          }        
        }
      }
    }
  }

/**************************************************************************/
/* FactTSGenAGain                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000727,                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactTSGenAGain(void)
  {
  char buffer[20];
  UC GainG, GainM;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u,%u\n", Calibration.CBCalibration.GainG, 
                                    Calibration.CBCalibration.GainM);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%u,%u", &GainG, &GainM)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.CBCalibration.GainG = GainG;
          Calibration.CBCalibration.GainM = GainM;
                                                                  
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));

          if (CBSetup.System < NTSC)
            WriteCBGain(GainG);
          else
            WriteCBGain(GainM);
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactTSGenCGain                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000727,                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactTSGenCGain(void)
  {
  char buffer[20];
  UC ChromaGainG, ChromaGainM;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u,%u\n", Calibration.CBCalibration.ChromaGainG, 
                                    Calibration.CBCalibration.ChromaGainM);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%u,%u", &ChromaGainG, &ChromaGainM)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.CBCalibration.ChromaGainG = ChromaGainG;
          Calibration.CBCalibration.ChromaGainM = ChromaGainM;
                                                                  
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));

          if (CBSetup.System < NTSC)
            WriteCBChromaGain(ChromaGainG);
          else
            WriteCBChromaGain(ChromaGainM);
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactTSGenPhase                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000727                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    PAL range: 0-4*625*1728, NTSC range: 0-2*525*1716            */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactTSGenPhase(void)
  {
  char buffer[30];
  UL PhaseG, PhaseM;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%lu,%lu\n", Calibration.CBCalibration.PhaseG, 
                                      Calibration.CBCalibration.PhaseM);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%lu,%lu", &PhaseG, &PhaseM)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.CBCalibration.PhaseG = PhaseG;
          Calibration.CBCalibration.PhaseM = PhaseM;
        
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));

          SetCBDelay(CBSetup.Delay);
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactTSGenScHPhase                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000727                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactTSGenScHPhase(void)
  {
  char buffer[30];
  UC ScHPhaseG, ScHPhaseM;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u,%u\n", Calibration.CBCalibration.ScHPhaseG, 
                                    Calibration.CBCalibration.ScHPhaseM);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%u,%u", &ScHPhaseG, &ScHPhaseM)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.CBCalibration.ScHPhaseG = ScHPhaseG;
          Calibration.CBCalibration.ScHPhaseM = ScHPhaseM;
        
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));

          SetCBScHPhase(CBSetup.ScHPhase);
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactAudioGenStatus                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000505                                    */
/* Revised:    000625                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:                                                                */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactAudioGenStatus(void)
  {
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u\n", 0, AudioStatus());
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactAudioGenDate                                              RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactAudioGenDate(void)
  {
  UI year, month, day;
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        day = Calibration.AudioCalibration.Date % 100;
        month = (Calibration.AudioCalibration.Date / 100) % 100;
        year = (Calibration.AudioCalibration.Date / 10000);

        sprintf(buffer, "%u,%u,%u\n", year, month, day);

        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 3) || ((sscanf(Cmd, "%u,%u,%u", &year, &month, &day)) != 3))
          SCPIError = SyntaxErr;
        else
          {
          if (ValidateDate(year, month, day) == FAIL)
            SCPIError = ExecErr;
          else
            {
            Calibration.AudioCalibration.Date = day + 100*month + 10000*year;
          
             NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));
            }
          }        
        }
      }
    }
  }

/**************************************************************************/
/* FactAudioGenLevel                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000411                                    */
/* Revised:    000622                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactAudioGenLevel(void)
  {
  char buffer[20];
  UC AnalogLeft, AnalogRight;

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u,%u\n", Calibration.AudioCalibration.AnalogLeftChannel, 
                                    Calibration.AudioCalibration.AnalogRightChannel);
        rs232puts(buffer);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if ((ParCnt != 2) || ((sscanf(Cmd, "%u,%u", &AnalogLeft, &AnalogRight)) != 2))
          SCPIError = SyntaxErr;
        else
          {
          Calibration.AudioCalibration.AnalogLeftChannel = AnalogLeft;
          Calibration.AudioCalibration.AnalogRightChannel = AnalogRight;
              
          NV_Store(&Calibration, CalibrationPtr, sizeof(Calibration));

          WriteAnlAudioDACLevel(AnalogLeft, AnalogRight);
          }
        }
      }
    }
  }

/**************************************************************************/
/* FactPLDVersion                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000505                                    */
/* Revised:    000625                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    NOTE! The video PLD version has not been implemented at this*/
/*            this time, hence the return value of 0 does NOT reflect the  */
/*            video PLD version.                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactPLDVersion(void)
  {
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u,%u\n", 0, pld_version());
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactTCXOStatus                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000628                                    */
/* Revised:    000628                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:                                                                */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactTCXOStatus(void)
  {
  char buffer[20];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt)
        SCPIError = SyntaxErr;
      else
        {
        sprintf(buffer, "%u\n", tcxo_status());
        rs232puts(buffer);
        }
      }
    else
      SCPIError = SyntaxErr;
    }
  }

/**************************************************************************/
/* FactMultiMediaChip                                            RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 000505                                    */
/* Revised:    000614                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    --                                                          */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void FactMultiMediaChip(void)
  {
  UI test[2];
  UC tmp[2];

  if (CmdExecute && !SCPIError)
    {
    if (CmdRequest)
      {
      if (ParCnt != 1)
        SCPIError = SyntaxErr;
      else
        {
        sscanf(Cmd, "%d", &test[0]);

        tmp[0] = test[0] & 0x0FF;

        TransmitIIC(0x40, 1, tmp, IIC_PTV);
        ReceiveIIC(0x40, 1, &tmp[1], IIC_PTV);

        rs232printf(12,"%03d,%03d\n", (int) tmp[0], (int) tmp[1]);
        }
      }
    else
      {
      if (!PasswordProtected)
        {
        if (ParCnt != 2)
          SCPIError = SyntaxErr;  
        else
          {
          sscanf(Cmd, "%d,%d", &test[0], &test[1]);

          tmp[0] = test[0] & 0x0FF;
          tmp[1] = test[1] & 0x0FF;

          TransmitIIC(0x40, 2, tmp, IIC_PTV);

          TransmitIIC(0x40, 1, tmp, IIC_PTV);
          ReceiveIIC(0x40, 1, &tmp[1], IIC_PTV);

          rs232printf(12,"%03d,%03d\n", (int) tmp[0], (int) tmp[1]);
          }
        }
      }
    }
  }

/**************************************************************************/
/* FindParameter                                                RS232CMD.C  */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 941124                                    */
/* Revised:    000603                                                       */
/*                                                                        */
/* Function:  Find the character defined parameter                        */
/* Remarks:    --                                                          */
/* Returns:    The array number containing the parameter or max. array+1    */
/* Updates:    --                                                          */
/**************************************************************************/
int FindParameter(char* par, struct PAR_STRUCT code* ParPtr)
  {
  int tmp = 0;

  while (ParPtr)
    {
    if ((!Cstrcmp(par, ParPtr->LongName)) || (!Cstrcmp(par, ParPtr->ShortName)))
      return(tmp);

    ParPtr = ParPtr->SameLevel;

    if (tmp < 255)
      tmp++;
    else
      break;
    }

  return(-1);
  }


/**************************************************************************/
/* FLTToSamples                                                 RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:    000530                                                       */
/*                                                                        */  
/* Function:  --                                                          */
/* Remarks:    PAL Range: 1728*256, NTSC Range: 1716*256                    */
/* Returns:    FLT in samples, or -1 if illegal                            */
/* Updates:    --                                                          */
/***************************************************************************/
long FLTToSamples(UC system, int F, int L, float T)   {

  long samples;
  char minus;

  if ((F == 0) && (L == 0) && (T == 0))
    return(0);

  if ((F <= 0) && (L <= 0) && (T <= 0))
    {
    minus = 1;
    }
  else
    {
    if ((F >= 0) && (L >= 0) && (T >= 0))
      minus = 0;
    else
      return(-1);
    }

                                      // 1 sample = (1/27MHz)/256
  samples = (long)((T*6912L)/1000L + 0.5);  

  if (system < NTSC)                  // If selected system is PAL..
    {

    if (labs(samples) > 442367L)     // Test for illegal time var
      return(-1);

    switch (F)                        // Test for illegal field & line
      {
      case 0:
        if ((minus && (L < -311)) || (!minus && (L > 312)))
          return(-1);
        break;

      case -1:
      case -3:
      case 2:
        if (abs(L) > 312)
          return(-1);
        break;

      case -2:
      case 1:
      case 3:
        if (abs(L) > 311)
          return(-1);
        break;

      case 4:
        if ((L > 0) || (T > 0.0))
          return(-1);
        break;

      default:
        return(-1);
      }

    if (!minus)                      // if posistive delay..
      {
      if (F % 2)
        {
        L += 313;
        F--;
        }
      }
    else                              // ..else delay is negative
      {
      F += 7;

      samples += 442367L;

      if (F % 2)   {
        L += 624;
        F--;
        }
      else
        L += 312;

      if (T == 0)
        samples++;
      }

    samples += F*138240000L + L*442368L;
    }
  else                                // ..else system is NTSC
    {

    if (labs(samples) > 439295L)    // Test for illegal time var
      return(-1);

    switch (F)                        // Test for illegal field & line
      {
      case 0:
        if ((minus && (L < -261)) || (!minus && (L > 262)))
          return(-1);
        break;

      case -1:
        if (abs(L) > 262)
          return(-1);
        break;

      case 1:
        if (abs(L) > 261)
          return(-1);
        break;

      case 2:
        if ((L > 0) || (T > 0.0))
          return(-1);
        break;

      default:
        return(-1);
      }

    if (!minus)                    // if posistive delay..
      {            

      if (F % 2)
        {
        L += 263;
        F--;
        }
      }
    else                              // ..else delay is negative
      {                                  
      F += 3;

      samples += 439295;

      if (F % 2)
        {
        L += 524;
        F--;
        }
      else
        L += 262;

      if (T == 0)
        samples++;
      }

    samples += F*115315200L + L*439296L;
    }

  return(samples);
  }

/**************************************************************************/
/* PrintSamplesToFLT                                           RS232CMD.C */
/*                                                                        */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:    000530                                                       */
/*                                                                        */
/* Function:  --                                                          */
/* Remarks:    PAL Range: 1728*256, NTSC Range: 1716*256                    */
/* Returns:    --                                                          */
/* Updates:    --                                                          */
/**************************************************************************/
void PrintSamplesToFLT(UC system, long samples)   {

  char sign;
  int frame, field, line;

  sign = '+';

  if (system < NTSC)                        // If system selected is PAL..
    {
    // 552960000 = 2*625*1728*256
    // 276480000 = 625*1728*256
    // 442368    = 1728*256

    if (samples >= 552960000L)
      {
      samples--;

      frame = samples / 276480000L;
      line = (samples % 276480000L) /  442368L;
      samples = (samples % 276480000L) %  442368L;
      field = 2*frame;

      if (line > 312)
        {
        field++;
        line = 624-line;
        }
      else
        line = 312-line;

      samples = 442368L - 1 - samples;
      field = 7-field;

      if (field != 4)
        sign = '-';
      }
    else
      {
      frame = samples / 276480000L;
      line = (samples % 276480000L) / 442368L;
      samples = (samples % 276480000L) % 442368L;
      field = 2*frame;

      if (line > 312)
        {
        field++;
        line -= 313;
        }
      }
    }
  else   {                                    // ..system selected is NTSC
    if (samples >= 230630400L)
      {
      // 230630400 = 525*1716*256
      // 439296    = 1716*256

      samples--;

      frame = samples / 230630400L;
      line = (samples % 230630400L) / 439296L;
      samples = (samples % 230630400L) % 439296L;

      field = 2*frame;

      if (line > 262)
        {
        field++;
        line = 524-line;
        }
      else
        line = 262-line;

      samples = 439296L - 1 - samples;
      field = 3 - field;

      if (field != 2)
        sign = '-';
      }
    else
      {
      frame = samples / 230630400L;
      line = (samples % 230630400L) / 439296L;
      samples = (samples % 230630400L) % 439296L;
      field = 2*frame;

      if (line > 262)
        {
        field++;
        line -= 263;
        }
      }
    }

  rs232printf(12, "%c%d,", sign, abs(field));
  rs232printf(12, "%c%d,", sign, abs(line));
  rs232printf(12, "%c%.1f", sign, fabs((float) (samples*1000L)/6912L));
  }

