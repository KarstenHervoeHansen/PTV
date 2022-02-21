/****************************************************************************/
/* MODULE:                                                                  */
/*   r232cmd.c - RS232 remote commands.                            					*/
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
/* Include files:							  																						*/

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "define.h"
#include "rtxcobj.h"
#include "rs232par.h"
#include "rs232err.h"
#include "rs232cmd.h"
#include "util.h"
#include "rs232.h"
#include "i2c_drv.h"

#include "tables.h"
#include "genlock.h"
#include "bb.h"
#include "cbar.h"
#include "audio.h"

#include "unitprg.h"

char code SystemVersionTxt[] 			= "1995.0";
char PasswordText[]		 						= "PASS5201";

char code CLSCmdTxt[] 	      		= "CLS";
char code ESECmdTxt[] 	      		= "ESE";
char code ESRCmdTxt[] 	      		= "ESR";
char code OPCCmdTxt[] 	      		= "OPC";
char code SRECmdTxt[] 	      		= "SRE";
char code STBCmdTxt[] 	      		= "STB";
char code TSTCmdTxt[] 	      		= "TST";
char code WAICmdTxt[] 	      		= "WAI";
char code RSTCmdTxt[] 	      		= "RST";
char code IDNCmdTxt[] 	      		= "IDN";
char code RCLCmdTxt[] 	      		= "RCL";
char code SAVCmdTxt[] 	      		= "SAV";

char code SYSTEMCmdTxt[] 	   	  	= "SYSTEM";
char code SYSTCmdTxt[] 		    	 	= "SYST";
char code STATUSCmdTxt[] 					= "STATUS";
char code STATCmdTxt[] 	 					= "STAT";
char code DIAGNOSTICCmdTxt[]			= "DIAGNOSTIC";
char code DIAGCmdTxt[] 	    			= "DIAG";
char code MONITORCmdTxt[]					= "MONITOR";
char code MONCmdTxt[] 	    			= "MON";
char code INPUTCmdTxt[] 	 			 	= "INPUT";
char code INPCmdTxt[] 	 				 	= "INP";
char code OUTPUTCmdTxt[] 					= "OUTPUT";
char code OUTPCmdTxt[] 						= "OUTP";
char code USERCmdTxt[]						= "USER";
char code MEASURECmdTxt[]					= "MEASURE";
char code MEASCmdTxt[] 	    			= "MEAS";
char code FACTORYCmdTxt[]					= "FACTORY";
char code FACTCmdTxt[] 	    			= "FACT";

char code ERRORCmdTxt[] 	     		= "ERROR";
char code ERRCmdTxt[] 		     		= "ERR";
char code VERSIONCmdTxt[] 	    	= "VERSION";
char code VERSCmdTxt[] 	      		= "VERS";
char code PRESETCmdTxt[] 					= "PRESET";
char code PRESCmdTxt[] 						= "PRES";
char code EVENTLOGCmdTxt[]				= "EVENTLOG";
char code DATECmdTxt[]						= "DATE";
char code TIMECmdTxt[]						= "TIME";

char code RECALLCmdTxt[] 					= "RECALL";
char code RECCmdTxt[] 						= "REC";
char code STORECmdTxt[] 					= "STORE";
char code STORCmdTxt[] 						= "STOR";
char code NAMECmdTxt[] 						= "NAME";

char code OPERATIONCmdTxt[] 			= "OPERATION";
char code OPERCmdTxt[] 	    			= "OPER";
char code QUESTIONABLECmdTxt[]	 	= "QUESTIONABLE";
char code QUESCmdTxt[]		 				= "QUES";
char code PTCmdTxt[] 	 						= "PT";

char code EVENTCmdTxt[] 					= "EVENT";
char code EVENCmdTxt[]						= "EVEN";
char code CONDITIONCmdTxt[]	 			= "CONDITION";
char code CONDCmdTxt[] 						= "COND";
char code ENABLECmdTxt[] 					= "ENABLE";
char code ENABCmdTxt[] 						= "ENAB";

char code DISPLAYCmdTxt[] 				= "DISPLAY";
char code DISPCmdTxt[] 						= "DISP";
char code ERRORQUEUECmdTxt[]  		= "ERRORQUEUE";
char code RESETCmdTxt[] 					= "RESET";
char code RESCmdTxt[] 						= "RES";

char code DEFAULTCmdTxt[] 			 	= "DEFAULT";
char code DEFCmdTxt[] 						= "DEF";

char code GENLOCKCmdTxt[] 				= "GENLOCK";
char code GENLCmdTxt[]			 			= "GENL";

char code DELAYCmdTxt[] 					= "DELAY";
char code DELCmdTxt[] 						= "DEL";

char code BBCmdTxt[] 							= "BB";
char code CBGENERATORCmdTxt[] 		= "CBGENERATOR";
char code CBGCmdTxt[] 						= "CBG";
char code AUDIOCmdTxt[] 					= "AUDIO";
char code AUDCmdTxt[] 						= "AUD";

char code SCHPHASECmdTxt[] 				= "SCHPHASE";
char code SCHPCmdTxt[] 						= "SCHP";

char code PATTERNCmdTxt[] 				= "PATTERN";
char code PATTCmdTxt[] 						= "PATT";
char code EMBAUDIOCmdTxt[] 				= "EMBAUDIO";
char code EMBCmdTxt[] 						= "EMB";

char code SIGNALCmdTxt[] 					= "SIGNAL";
char code SIGNCmdTxt[] 						= "SIGN";
char code LEVELCmdTxt[] 					= "LEVEL";
char code LEVCmdTxt[] 						= "LEV";
char code EDHINSERTCmdTxt[] 			= "EDHINSERT";
char code EDHCmdTxt[] 						= "EDH";

char code TIMINGCmdTxt[] 					= "TIMING";
char code TIMCmdTxt[] 						= "TIM";

char code WORDCLOCKCmdTxt[] 			= "WORDCLOCK";
char code WORDCmdTxt[] 						= "WORD";

char code CLICKCmdTxt[] 					= "CLICK";
char code CLICCmdTxt[] 						= "CLIC";

char code PASSWORDCmdTxt[] 				= "PASSWORD";
char code PASSCmdTxt[] 						= "PASS";
char code V24CIRCUITCmdTxt[] 			= "V24CIRCUIT";
char code V24CCmdTxt[] 						= "V24C";
//char code ADDRESSCmdTxt[] 				= "ADDRESS";
//char code ADDRCmdTxt[] 						= "ADDR";
char code COMMANDCmdTxt[] 				= "COMMAND";
char code COMMCmdTxt[] 						= "COMM";
char code TRANSPARENTCmdTxt[]  		= "TRANSPARENT";
char code TRANCmdTxt[] 				 		= "TRAN";

char code MAINBOARDCmdTxt[] 			= "MAINBOARD";
char code MAINCmdTxt[] 						= "MAIN";
/*char code COMPANYCmdTxt[] 				= "COMPANY";
char code COMPCmdTxt[] 						= "COMP";
char code TYPECmdTxt[] 						= "TYPE";
char code KUNUMBERCmdTxt[] 				= "KUNUMBER";
char code KUNCmdTxt[] 						= "KUN";
*/
char code NCCmdTxt[] 							= "NC";
/*char code SWREVISIONCmdTxt[] 			= "SWREVISION";
char code SWRCmdTxt[] 						= "SWR";
*/char code FMSDATECmdTxt[] 				= "FMSDATE";
char code FMSDCmdTxt[] 						= "FMSD";
char code PRODUCTIONCmdTxt[] 			= "PRODUCTION";
char code PRODCmdTxt[] 						= "PROD";
char code UPDATECmdTxt[] 					= "UPDATE";
char code UPDCmdTxt[] 						= "UPD";
char code FMSLOCATIONCmdTxt[] 		= "FMSLOCATION";
char code FMSLCmdTxt[] 						= "FMSL";

char code SPGGCmdTxt[] 						= "SPGG";
char code FREFERENCECmdTxt[] 		 	= "FREFERENCE";
char code FREFCmdTxt[] 						= "FREF";
char code DACCmdTxt[] 						= "DAC";
char code UREFERENCECmdTxt[] 			= "UREFERENCE";
char code UREFCmdTxt[] 						= "UREF";
char code HPHZEROCmdTxt[] 				= "HPHZERO";
char code HPHZCmdTxt[] 						= "HPHZ";
char code PHASECmdTxt[] 					= "PHASE";
char code PHASCmdTxt[] 						= "PHAS";
char code PALCmdTxt[]							= "PAL";
char code NTSCCmdTxt[]						= "NTSC";

char code CGAINCmdTxt[] 				 	= "CGAIN";
char code CGACmdTxt[] 						= "CGA";
char code GAINCmdTxt[] 						= "GAIN";
char code OFFSETCmdTxt[] 			 		= "OFFSET";
char code OFFSCmdTxt[] 						= "OFFS";
char code ALEVELCmdTxt[] 					= "ALEVEL";
char code ALEVCmdTxt[] 						= "ALEV";
char code LEFTCmdTxt[] 						= "LEFT";
char code RIGHTCmdTxt[] 					= "RIGHT";
char code RIGHCmdTxt[] 						= "RIGH";

char code SAVEParTxt[] 						= "SAVE";

char code INTERNALParTxt[] 				= "INTERNAL";
char code INTParTxt[] 						= "INT";
char code PALBURSTParTxt[] 				= "PALBURST";
char code PALBParTxt[] 						= "PALB";
char code NTSCBURSTParTxt[] 			= "NTSCBURST";
char code NTSCParTxt[] 						= "NTSC";

char code F10MHzParTxt[] 					= "F10MHZ";

char code PALParTxt[] 						= "PAL";
char code PAL_IDParTxt[] 					= "PAL_ID";

char code CBSMPTEParTxt[]					= "CBSMPTE";			// SMPTE Colorbar
char code CBSMParTxt[] 						= "CBSM";					
char code CBEBUParTxt[] 					= "CBEBU";      	// EBU Colorbar
char code CBEBParTxt[] 						= "CBEB";
char code CBFCCParTxt[] 					= "CBFCC";				// FCC Colorbar
char code CBFCParTxt[] 						= "CBFC";
char code CBEBU8ParTxt[] 					= "CBEBU8";				// Colorbar ITU801
char code CBEB8ParTxt[] 					= "CBEB8";
char code CB100ParTxt[]		 				= "CB100";				// 100% Colorbar
char code CBGREY75ParTxt[] 				= "CBGREY75";			// Colorbar + 75% Grey
char code CBGR75ParTxt[]					= "CBGR75";
char code CBRED75ParTxt[]  				= "CBRED75";			// Colorbar + 75% Red
char code CBR75ParTxt[] 					= "CBR75";
char code RED75ParTxt[] 					= "RED75";				// 75% Red

char code MULTIBURSTParTxt[] 			= "MULTIBURST";		// Multi burst
char code MULTParTxt[] 						= "MULT";
char code WIN10ParTxt[] 					= "WIN10";				// Window 10%
char code WIN15ParTxt[] 					= "WIN15";				// Window 15%
char code WIN20ParTxt[] 					= "WIN20";				// Window 20%
char code WIN100ParTxt[] 					= "WIN100";				// Window 100%
char code BLWH15ParTxt[] 					= "BLWH15KHZ";		// 15kHz Bl/Wh
char code WHITE100ParTxt[] 				= "WHITE100";			// White 100%
char code WHIT100ParTxt[] 				= "WHIT100";
char code BLACKParTxt[] 					= "BLACK";				// Black
char code BLACParTxt[]		 				= "BLAC";

char code SDICHECKParTxt[] 				= "SDICHECK";			// Check field
char code SDICParTxt[] 						= "SDIC";
char code DGREYParTxt[] 					= "DGREY";				// Digital grey
char code DGRParTxt[] 						= "DGR";
char code STAIRCASE5ParTxt[]			= "STAIRCASE5";	 	// 5-step staircase
char code STA5ParTxt[] 						= "STA5";
char code STAIRCASE10ParTxt[] 		= "STAIRCASE10";	// 10-step staircase
char code STA10ParTxt[] 					= "STA10";
char code CROSSHATCHParTxt[] 			= "CROSSHATCH";		// Cross hatch
char code CROSParTxt[] 						= "CROS";
char code PLUGEParTxt[] 					= "PLUGE";				// PLUGE
char code PLUGParTxt[] 						= "PLUG";

char code ANALOGParTxt[]					= "ANALOG";
char code ANALParTxt[]						= "ANAL";
char code AESEBUParTxt[]					= "AESEBU";
char code AESParTxt[]							= "AES";

char code F441KHZParTxt[]		 			= "F441KHZ";
char code F48KHZParTxt[] 					= "F48KHZ";

char code S500HZParTxt[] 					= "S500HZ";				// Stereo 500 Hz
char code S1KHZParTxt[] 					= "S1KHZ";   			// Stereo 1 kHz
char code SEBU1KHZParTxt[]		 		= "SEBU1KHZ";			// Stereo EBU 1 kHz
char code S8KHZParTxt[] 					= "S500HZ";				// Stereo 8 kHz

char code SILENCEParTxt[] 				= "SILENCE";
char code SILParTxt[] 						= "SIL";

char code DB0FSParTxt[] 					= "DB0FS";
char code DB9FSParTxt[] 					= "DB9FS";
char code DB12FSParTxt[]			 		= "DB12FS";
char code DB14FSParTxt[] 					= "DB14FS";
char code DB15FSParTxt[] 					= "DB15FS";
char code DB16FSParTxt[] 					= "DB16FS";
char code DB18FSParTxt[] 					= "DB18FS";
char code DB20FSParTxt[] 					= "DB20FS";

char code DMYParTxt[]							= "DMY";
char code MDYParTxt[]							= "MDY";
char code YMDParTxt[]							= "YMD";

char code BBParTxt[] 							= "BB";
char code SPGParTxt[]  						= "SPG";

char code OFFParTxt[] 						= "OFF";
char code ONParTxt[] 							= "ON";

char code MINParTxt[] 						= "MIN";
char code MAXParTxt[] 						= "MAX";


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

//void PresetSuffix(void);

void SystemError(void);
void SystemVersion(void);
void SystemTime(void);
void SystemDate(void);
void SystemPreset(void);
void SystemPresetStore(void);
void SystemPresetName(void);

void StatusOperEvent(void);
void StatusOperCondition(void);
void StatusOperEnable(void);
void StatusQuestEvent(void);
void StatusQuestCondition(void);
void StatusQuestEnable(void);
void StatusPreset(void);
void StatusPTSuffix(void);
void StatusPT(void);

void DiagnosticErrorQueue(void);
void DiagnosticErrorQueueReset(void);

void User_Default_Save(void);

void GenlockGen( void);
void GenlockGenSystem(void);
void GenlockGenDelay(void);

void BBGenSuffix( void);
void BBGen( void);
void CBGen( void);
void AudioGen( void);

void BBGenSystem( void);
void BBGenDelay( void);
void BBGenScHPhase( void);

void CBGenPattern( void);
void CBGenSystem( void);
void CBGenDelay( void);
void CBGenScHPhase( void);
void CBGenEmbAudioSignal( void);

void AudioGenOutput( void);
void AudioGenSystem( void);
void AudioGenSignal( void);
void AudioGenLevel( void);
void AudioGenTiming( void);
void AudioGenWordClock( void);
void AudioGenClick( void);

void FactPassword( void);
//void FactoryV24Address( void);
void FactV24Command( void);
void FactV24Transparent( void);

void FactMainIDN( void);
void FactMainNC( void);
void FactMainSystem( void);
void FactMainFMSDateProd( void);
void FactMainFMSDateUpd( void);
void FactMainFMSLocProd( void);
void FactMainFMSLocUpd( void);

int FindParameter( char* par, struct PAR_STRUCT code* TmpPtr);

long FLTToSamples( UC system, int F, int L, float T);
void PrintSamplesToFLT( UC system, long samples);

struct CMD_STRUCT code Mandated[12] = {
	{ CLSCmdTxt, NULL, &Mandated[1], NULL, NONE, CLSCommand},
	{ ESECmdTxt, NULL, &Mandated[2], NULL, NONE, ESECommand},
	{ ESRCmdTxt, NULL, &Mandated[3], NULL, NONE, ESRCommand},
	{ IDNCmdTxt, NULL, &Mandated[4], NULL, NONE, IDNCommand},
	{ OPCCmdTxt, NULL, &Mandated[5], NULL, NONE, OPCCommand},
	{ RSTCmdTxt, NULL, &Mandated[6], NULL, NONE, RSTCommand},
	{ SRECmdTxt, NULL, &Mandated[7], NULL, NONE, SRECommand},
	{ STBCmdTxt, NULL, &Mandated[8], NULL, NONE, STBCommand},
	{ TSTCmdTxt, NULL, &Mandated[9], NULL, NONE, TSTCommand},
	{ RCLCmdTxt, NULL, &Mandated[10], NULL, NONE, RCLCommand},
	{ SAVCmdTxt, NULL, &Mandated[11], NULL, NONE, SAVCommand},
	{ WAICmdTxt, NULL, NULL         , NULL, NONE, WAICommand}
};

struct CMD_STRUCT code Subsystem[9] = {
	{ SYSTEMCmdTxt,		 	SYSTCmdTxt, &Subsystem[1], &System[0],  	 NONE, NONE},
	{ STATUSCmdTxt, 		STATCmdTxt, &Subsystem[2], &Status[0],  	 NONE, NONE},
	{ DIAGNOSTICCmdTxt, DIAGCmdTxt, &Subsystem[3], &Diagnostic[0], NONE, NONE},
	{ MONITORCmdTxt, 		MONCmdTxt, 	&Subsystem[4], NULL, 			  	 NONE, NONE},
	{ INPUTCmdTxt, 			INPCmdTxt, 	&Subsystem[5], &Input[0],  	 	 NONE, NONE},
	{ OUTPUTCmdTxt, 		OUTPCmdTxt, &Subsystem[6], &Output[0],  	 NONE, NONE},
	{ USERCmdTxt, 			USERCmdTxt, &Subsystem[7], NULL, 			  	 NONE, NONE},
	{ MEASURECmdTxt, 		MEASCmdTxt, &Subsystem[8], NULL, 			  	 NONE, NONE},
	{ FACTORYCmdTxt, 		FACTCmdTxt, NULL, 				 &Factory[0],		 NONE, NONE},
};

// **************************************************************************
/*	CMD:	SUB1:	SUB2:	SUB3:	PARM:
		SYSTem
			:ERRor?
    	:VERSion?
	    :PRESet<?>
  	  	:RECall<?>	1 .. 3
    	  :STORe			1 .. 3
      	:NAMe<?>		1 .. 3 , <name>
	      :DATE<?>		<year>,<month>,<date>
  	    :TIME<?>		<hour>,<min>,<sec>
*/
struct CMD_STRUCT code System[6] = {
	{ ERRORCmdTxt, 		ERRCmdTxt, 	&System[1], NULL, 						NONE, SystemError},
	{ VERSIONCmdTxt,	VERSCmdTxt, &System[2], NULL, 						NONE, SystemVersion},
	{ PRESETCmdTxt, 	PRESCmdTxt, &System[3], &SystemLevel1[0], NONE, SystemPreset},
	{ EVENTLOGCmdTxt, EVENCmdTxt, &System[4], NULL, 						NONE, NONE},
	{ DATECmdTxt, 		DATECmdTxt, &System[5], NULL, 						NONE, NONE},
	{ TIMECmdTxt, 		TIMECmdTxt, NULL, 			NULL, 						NONE, NONE}
};

struct CMD_STRUCT code SystemLevel1[3] = {
	{ RECALLCmdTxt, RECCmdTxt, 	&SystemLevel1[1], NULL, NONE, SystemPreset},
	{ STORECmdTxt, 	STORCmdTxt, &SystemLevel1[2], NULL, NONE, SystemPresetStore},
	{ NAMECmdTxt, 	NAMECmdTxt,	NULL, 						NULL, NONE, SystemPresetName},
};

// **************************************************************************
/*	CMD:	SUB1:	SUB2:	SUB3:	PARM:
		STATus
	 		:OPERation
  	 		:EVENt?
	      :CONDition?
  	    :ENABle<?>	0 .. 65535
   		:QUEStionable
     		:EVENt?
	      :CONDition?
  	    :ENABle<?>	0 .. 65535
			:PRESet				TBD
	    :PT5201?
*/
struct CMD_STRUCT code Status[4] = {
	{ OPERATIONCmdTxt, 		OPERCmdTxt, &Status[1], &StatusLevel1_1[0], NONE, 					StatusOperEvent},
	{ QUESTIONABLECmdTxt, QUESCmdTxt, &Status[2], &StatusLevel1_2[0], NONE, 					StatusQuestEvent},
	{ PRESETCmdTxt, 			PRESCmdTxt, &Status[3], NULL, 							NONE, 				  StatusPreset},
	{ PTCmdTxt, 					PTCmdTxt, 	NULL, 			NULL, 							StatusPTSuffix, StatusPT},
};

struct CMD_STRUCT code StatusLevel1_1[3] = {
	{ EVENTCmdTxt, 		 EVENCmdTxt, &StatusLevel1_1[1], NULL, NONE, StatusOperEvent},
	{ CONDITIONCmdTxt, CONDCmdTxt, &StatusLevel1_1[2], NULL, NONE, StatusOperCondition},
	{ ENABLECmdTxt, 	 ENABCmdTxt, NULL, 							 NULL, NONE, StatusOperEnable},
};

struct CMD_STRUCT code StatusLevel1_2[3] = {
	{ EVENTCmdTxt,		 EVENCmdTxt, &StatusLevel1_2[1], NULL, NONE, StatusQuestEvent},
	{ CONDITIONCmdTxt, CONDCmdTxt, &StatusLevel1_2[2], NULL, NONE, StatusQuestCondition},
	{ ENABLECmdTxt, 	 ENABCmdTxt, NULL, 							 NULL, NONE, StatusQuestEnable},
};

// **************************************************************************
/*	CMD:	SUB1:	SUB2:	SUB3:	PARM:
		DIAGnostic
		 	:DISPlay		TBD
 	  	:ERRorqueue?
	  	:RESet
*/
struct CMD_STRUCT code Diagnostic[2] = {
	{ DISPLAYCmdTxt,		DISPCmdTxt, &Diagnostic[1], NULL, 								NONE, NONE},
	{ ERRORQUEUECmdTxt, ERRCmdTxt, 	NULL, 					&DiagnosticLevel1[0], NONE, DiagnosticErrorQueue},
};

struct CMD_STRUCT code DiagnosticLevel1[1] = {
	{ RESETCmdTxt, RESCmdTxt, NULL, NULL, NONE, DiagnosticErrorQueueReset},
};

// **************************************************************************
/*	CMD:	SUB1:	SUB2:	SUB3:	PARM:
		USER
			:DEFault		SAV
                	
*/
struct CMD_STRUCT code User[1] = {
	{ DEFAULTCmdTxt, DEFCmdTxt, NULL, NULL, NONE, User_Default_Save}
};


// **************************************************************************
/*	CMD:	SUB1:	SUB2:		PARM:
		INPut
			:GENLOCK<?>
				:SYSTem			INTernal | PALBurst | NTSCburst |F10MHZ
				:DELay			<Field>,<Line>,<Time>
*/
//**************************************************************************
struct CMD_STRUCT code Input[1] = {
	{ GENLOCKCmdTxt, GENLCmdTxt, NULL, &GenlockLevel1[0], NONE, GenlockGen},
};

struct CMD_STRUCT code GenlockLevel1[2] = {
	{ SYSTEMCmdTxt, SYSTCmdTxt, &GenlockLevel1[1], NULL, NONE, GenlockGenSystem},
	{ DELAYCmdTxt, 	DELCmdTxt, 	NULL, 						 NULL, NONE, GenlockGenDelay},
};

// **************************************************************************
/*	CMD:	SUB1:	SUB2:		PARM:
		OUTPut
			:BB1n<?>
				:SYSTem			INTernal | PALBurst | NTSCburst |F10MHZ
				:DELay			<Field>,<Line>,<Time>
				:SCHPhase		<-179to 180>
			:BB2n<?>
				:SYSTem			INTernal | PALBurst | NTSCburst |F10MHZ
				:DELay			<Field>,<Line>,<Time>
				:SCHPhase		<-179to 180>
			:BB3n<?>
				:SYSTem			INTernal | PALBurst | NTSCburst |F10MHZ
				:DELay			<Field>,<Line>,<Time>
				:SCHPhase		<-179to 180>
			:CBGenerator<?>
				:PATTern 	  <pattern name>
				:SYSTem			INTernal | PALBurst | NTSCburst |F10MHZ
				:DELay			<Field>,<Line>,<Time>
				:SCHPhase		<-179to 180>
				:EMBaudio<?>
					:SIGNal		SILence | F1KHZ
			:AUDio
				:OUTPut
				:SYSTem
				:LEVel
				:TIMing
				:WORDclock
				:CLICkperiod
*/
struct CMD_STRUCT code Output[3] = {
	{ BBCmdTxt, 				 BBCmdTxt,  &Output[1], &BBGenLevel1[0],		BBGenSuffix, BBGen},
	{ CBGENERATORCmdTxt, CBGCmdTxt, &Output[2], &CBGenLevel1[0], 		NONE,		  	 CBGen},
	{ AUDIOCmdTxt, 			 AUDCmdTxt, NULL,				&AudioGenLevel1[0], NONE,		  	 AudioGen},
};

struct CMD_STRUCT code BBGenLevel1[3] = {
	{ SYSTEMCmdTxt, 	SYSTCmdTxt, &BBGenLevel1[1], NULL, NONE, BBGenSystem},
	{ DELAYCmdTxt,	  DELCmdTxt,  &BBGenLevel1[2], NULL, NONE, BBGenDelay},
	{ SCHPHASECmdTxt, SCHPCmdTxt, NULL, 					 NULL, NONE, BBGenScHPhase},
};

struct CMD_STRUCT code CBGenLevel1[5] = {
	{ PATTERNCmdTxt, 	PATTCmdTxt, &CBGenLevel1[1], NULL, 							NONE, CBGenPattern},
	{ SYSTEMCmdTxt, 	SYSTCmdTxt, &CBGenLevel1[2], NULL, 							NONE, CBGenSystem},
	{ DELAYCmdTxt,	  DELCmdTxt,  &CBGenLevel1[3], NULL, 							NONE, CBGenDelay},
	{ SCHPHASECmdTxt, SCHPCmdTxt, &CBGenLevel1[4], NULL, 							NONE, CBGenScHPhase},
	{ EMBAUDIOCmdTxt, EMBCmdTxt, 	NULL, 					 &CBGenLevel1_1[0], NONE, NONE},
};

struct CMD_STRUCT code CBGenLevel1_1[3] = {
	{ SIGNALCmdTxt, 	 SIGNCmdTxt, &CBGenLevel1_1[1], NULL, NONE, CBGenEmbAudioSignal},
	{ LEVELCmdTxt, 	   LEVCmdTxt,  &CBGenLevel1_1[2], NULL, NONE, NONE},
	{ EDHINSERTCmdTxt, EDHCmdTxt,  NULL,							NULL, NONE, NONE},
};

struct CMD_STRUCT code AudioGenLevel1[7] = {
	{ OUTPUTCmdTxt, 	 OUTPCmdTxt, &AudioGenLevel1[1], NULL, NONE, AudioGenOutput},
	{ SYSTEMCmdTxt, 	 SYSTCmdTxt, &AudioGenLevel1[2], NULL, NONE, AudioGenSystem},
	{ SIGNALCmdTxt, 	 SIGNCmdTxt, &AudioGenLevel1[3], NULL, NONE, AudioGenSignal},
	{ LEVELCmdTxt, 		 LEVCmdTxt,  &AudioGenLevel1[4], NULL, NONE, AudioGenLevel},
	{ TIMINGCmdTxt,	   TIMCmdTxt,  &AudioGenLevel1[5], NULL, NONE, AudioGenTiming},
	{ WORDCLOCKCmdTxt, WORDCmdTxt, &AudioGenLevel1[6], NULL, NONE, AudioGenWordClock},
	{ CLICKCmdTxt, 		 CLICCmdTxt, NULL, 							 NULL, NONE, AudioGenClick},
};

// **************************************************************************
/*	CMD:	SUB1:	SUB2:		PARM:
		FACTORY
			:PASSword

			:V24Circuit
				:ADRess
				:COMMand
				:TRANsparent

			:MAINboard
				:COMPany
				:TYPE
				:KUNumber
				:NC
				:SWRevision
				:SYSTem
				:FMSData
					:PRODuction
					:UPDate
				:FMSLocation
					:PRODuction
					:UPDate

			:GENLock
				[:DATE]
				:FREFerence
					[:DATE]
					:DAC
				:UREFerence
					[:DATE]
					:DAC
				:HPHZero
					:PAL
					:NTSC
				:PHASe
					:PAL
					:NTSC

			:BB 1-3
				[:DATE]
				:PHASe
					:PAL
				 	:NTSC
				:SCHPhase
					:PAL
					:NTSC
				:DAC
					:GAIN
					:OFFSet
					:LEVel

			:CBGenerator
				[:DATE]
				:CGAin
				:GAIN
				:PHASE
					:PAL
					:NTSC
				:SCHPhase
					:PAL
					:NTSC

			:AUDio
				[:DATE]
				:ALEVel
					:LEFT
					:RIGHt
*/

struct CMD_STRUCT code Factory[7] = {
	{ PASSWORDCmdTxt, 		PASSCmdTxt,  &Factory[1], NULL,							 NONE, 				FactPassword},
	{ V24CIRCUITCmdTxt, 	V24CCmdTxt,  &Factory[2], &FactoryLevel1[0], NONE, 				NONE},
	{ MAINBOARDCmdTxt, 		MAINCmdTxt,  &Factory[3], &FactoryLevel2[0], NONE,		  	NONE},
	{ GENLOCKCmdTxt, 			GENLCmdTxt,	 &Factory[4],	&FactoryLevel3[0], NONE,		  	NONE},
	{ BBCmdTxt, 			 		BBCmdTxt, 	 &Factory[5],	NULL, 						 NONE,		  	NONE},
	{ CBGENERATORCmdTxt, 	CBGCmdTxt, 	 &Factory[6],	NULL, 						 NONE,		  	NONE},
	{ AUDIOCmdTxt, 			 	AUDCmdTxt, 	 NULL,			  NULL, 						 NONE,		  	NONE},
};

struct CMD_STRUCT code FactoryLevel1[2] = {
	{ COMMANDCmdTxt, 		 COMMCmdTxt, &FactoryLevel1[1], NULL, NONE, FactV24Command},
	{ TRANSPARENTCmdTxt, TRANCmdTxt, NULL,						  NULL, NONE, FactV24Transparent},
};

struct CMD_STRUCT code FactoryLevel2[5] = {
	{ IDNCmdTxt,  	 		 IDNCmdTxt,  &FactoryLevel2[1], NULL, 					     NONE, FactMainIDN},
	{ NCCmdTxt,  				 NCCmdTxt,   &FactoryLevel2[2], NULL, 					  	 NONE, FactMainNC},
	{ SYSTEMCmdTxt, 		 SYSTCmdTxt, &FactoryLevel2[3], NULL, 					  	 NONE, FactMainSystem},
	{ FMSDATECmdTxt, 		 FMSDCmdTxt, &FactoryLevel2[4], &FactoryLevel2_1[0], NONE, NONE},
	{ FMSLOCATIONCmdTxt, FMSLCmdTxt, NULL,						  &FactoryLevel2_2[0], NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel2_1[2] = {
	{ PRODUCTIONCmdTxt, PRODCmdTxt, &FactoryLevel2_1[1], NULL, NONE, FactMainFMSDateProd},
	{ UPDATECmdTxt, 		UPDCmdTxt,  NULL, 						   NULL, NONE, FactMainFMSDateUpd},
};

struct CMD_STRUCT code FactoryLevel2_2[2] = {
	{ PRODUCTIONCmdTxt, PRODCmdTxt, &FactoryLevel2_2[1], NULL, NONE, FactMainFMSLocProd},
	{ UPDATECmdTxt,     UPDCmdTxt,  NULL, 						   NULL, NONE, FactMainFMSLocUpd},
};

struct CMD_STRUCT code FactoryLevel3[5] = {
	{ DATECmdTxt,  	 	  DATECmdTxt, &FactoryLevel3[1], NULL, 								NONE, NONE},
	{ FREFERENCECmdTxt, FREFCmdTxt, &FactoryLevel3[2], &FactoryLevel3_1[0], NONE, NONE},
	{ UREFERENCECmdTxt,	UREFCmdTxt, &FactoryLevel3[3], &FactoryLevel3_2[0], NONE, NONE},
	{ HPHZEROCmdTxt,  	HPHZCmdTxt, &FactoryLevel3[4], &FactoryLevel3_3[0], NONE, NONE},
	{ PHASECmdTxt,  		PHASCmdTxt, NULL, 						 &FactoryLevel3_4[0], NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel3_1[2] = {
	{ DATECmdTxt, DATECmdTxt, &FactoryLevel3_1[1], NULL, NONE, NONE},
	{ DACCmdTxt,  DACCmdTxt,  NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel3_2[2] = {
	{ DATECmdTxt, DATECmdTxt, &FactoryLevel3_2[1], NULL, NONE, NONE},
	{ DACCmdTxt,  DACCmdTxt,  NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel3_3[2] = {
	{ PALCmdTxt,  PALCmdTxt,  &FactoryLevel3_3[1], NULL, NONE, NONE},
	{ NTSCCmdTxt, NTSCCmdTxt, NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel3_4[2] = {
	{ PALCmdTxt,  PALCmdTxt,  &FactoryLevel3_4[1], NULL, NONE, NONE},
	{ NTSCCmdTxt, NTSCCmdTxt, NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel4[4] = {
	{ DATECmdTxt,  	 	DATECmdTxt, &FactoryLevel4[1], NULL, 						    NONE, NONE},
	{ PHASECmdTxt, 		PHASCmdTxt, &FactoryLevel4[2], &FactoryLevel4_1[0], NONE, NONE},
	{ SCHPHASECmdTxt,	SCHPCmdTxt, &FactoryLevel4[3], &FactoryLevel4_2[0], NONE, NONE},
	{ DACCmdTxt,  		DACCmdTxt,  NULL, 						 &FactoryLevel4_3[0], NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel4_1[2] = {
	{ PALCmdTxt,  PALCmdTxt,  &FactoryLevel4_1[1], NULL, NONE, NONE},
	{ NTSCCmdTxt, NTSCCmdTxt, NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel4_2[2] = {
	{ PALCmdTxt,  PALCmdTxt,  &FactoryLevel4_2[1], NULL, NONE, NONE},
	{ NTSCCmdTxt, NTSCCmdTxt, NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel4_3[3] = {
	{ GAINCmdTxt,  	GAINCmdTxt, &FactoryLevel4_3[1], NULL, NONE, NONE},
	{ OFFSETCmdTxt, OFFSCmdTxt, &FactoryLevel4_3[2], NULL, NONE, NONE},
	{ LEVELCmdTxt,  LEVCmdTxt,  NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel5[5] = {
	{ DATECmdTxt,  	 	DATECmdTxt, &FactoryLevel5[1], NULL, 						    NONE, NONE},
	{ CGAINCmdTxt, 		CGACmdTxt, 	&FactoryLevel5[2], NULL, 						    NONE, NONE},
	{ PHASECmdTxt, 		PHASCmdTxt, &FactoryLevel5[3], &FactoryLevel5_1[0], NONE, NONE},
	{ SCHPHASECmdTxt,	SCHPCmdTxt, &FactoryLevel5[4], &FactoryLevel5_2[0], NONE, NONE},
	{ DACCmdTxt,  		DACCmdTxt,  NULL, 						 NULL, 								NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel5_1[2] = {
	{ PALCmdTxt,  PALCmdTxt,  &FactoryLevel5_1[1], NULL, NONE, NONE},
	{ NTSCCmdTxt, NTSCCmdTxt, NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel5_2[2] = {
	{ PALCmdTxt, 	PALCmdTxt,  &FactoryLevel5_2[1], NULL, NONE, NONE},
	{ NTSCCmdTxt, NTSCCmdTxt, NULL, 						   NULL, NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel6[2] = {
	{ DATECmdTxt,  	DATECmdTxt, &FactoryLevel6[1], NULL, 						    NONE, NONE},
	{ ALEVELCmdTxt, ALEVCmdTxt, NULL, 						 &FactoryLevel6_1[0], NONE, NONE},
};

struct CMD_STRUCT code FactoryLevel6_1[2] = {
	{ LEFTCmdTxt,  LEFTCmdTxt, &FactoryLevel6_1[1], NULL, NONE, NONE},
	{ RIGHTCmdTxt, RIGHCmdTxt, NULL, 						    NULL, NONE, NONE},
};

//**************************************************************************
// The following parameter structures are based on the enumerated types
//  defined in TABLES.H, ie. DO NOT change the order of these names. If
//  the order is changed in TABLES.H it must also be changed here.

struct PAR_STRUCT code SAVEPar[1] = {
	{ SAVEParTxt, SAVEParTxt, NULL}
};

struct PAR_STRUCT code GenlockSystemPar[4] = {
	{ INTERNALParTxt, INTParTxt, &GenlockSystemPar[1]},
	{ PALBURSTParTxt, PALBParTxt, &GenlockSystemPar[2]},
	{ NTSCBURSTParTxt, NTSCParTxt, &GenlockSystemPar[3]},
	{ F10MHzParTxt, F10MHzParTxt, NULL},
};

struct PAR_STRUCT code SystemPar[3] = {
	{ PALParTxt, PALParTxt, &SystemPar[1]},
	{ PAL_IDParTxt, PAL_IDParTxt, &SystemPar[2]},
	{ NTSCParTxt, NTSCParTxt, NULL},
};

struct PAR_STRUCT code CBPatternPar[22] = {
	{ CBSMPTEParTxt, 			CBSMParTxt, 		&CBPatternPar[1]},
	{ CBEBUParTxt, 	 			CBEBParTxt, 		&CBPatternPar[2]},
	{ CBFCCParTxt, 	 			CBFCParTxt, 		&CBPatternPar[3]},
	{ CBEBU8ParTxt,  			CBEB8ParTxt, 		&CBPatternPar[4]},
	{ CB100ParTxt, 	 			CB100ParTxt, 		&CBPatternPar[5]},
	{ CBGREY75ParTxt, 		CBGR75ParTxt, 	&CBPatternPar[6]},
	{ CBRED75ParTxt, 			CBR75ParTxt, 		&CBPatternPar[7]},
	{ RED75ParTxt, 				RED75ParTxt, 		&CBPatternPar[8]},

	{ MULTIBURSTParTxt,	 	MULTParTxt, 		&CBPatternPar[9]},
																			
	{ WIN10ParTxt, 				WIN10ParTxt, 		&CBPatternPar[10]},
	{ WIN15ParTxt, 				WIN15ParTxt,	 	&CBPatternPar[11]},
	{ WIN20ParTxt, 				WIN20ParTxt, 		&CBPatternPar[12]},
	{ WIN100ParTxt, 			WIN100ParTxt, 	&CBPatternPar[13]},
	{ BLWH15ParTxt, 			BLWH15ParTxt, 	&CBPatternPar[14]},
	{ WHITE100ParTxt, 		WHIT100ParTxt, 	&CBPatternPar[15]},
	{ BLACKParTxt, 				BLACParTxt, 		&CBPatternPar[16]},

	{ SDICHECKParTxt, 		SDICParTxt, 		&CBPatternPar[17]},
	{ DGREYParTxt, 				DGRParTxt, 			&CBPatternPar[18]},

	{ STAIRCASE5ParTxt, 	STA5ParTxt, 		&CBPatternPar[19]},
	{ STAIRCASE10ParTxt, 	STA10ParTxt, 		&CBPatternPar[20]},

	{ CROSSHATCHParTxt, 	CROSParTxt, 		&CBPatternPar[21]},
	{ PLUGEParTxt, 				PLUGParTxt, 		NULL},
};

struct PAR_STRUCT code EmbAudioSignalPar[2] = {
	{ OFFParTxt, 			OFFParTxt, 		&EmbAudioSignalPar[1]},
	{ S1KHZParTxt, 		S1KHZParTxt, 	NULL},
};

struct PAR_STRUCT code AudioOutputPar[2] = {
	{ ANALOGParTxt, ANALParTxt, &AudioOutputPar[1]},
	{ AESEBUParTxt, AESParTxt, 	NULL},
};

struct PAR_STRUCT code AESEBUSystemPar[2] = {
	{ PALParTxt, 	PALParTxt, 	&AESEBUSystemPar[1]},
	{ NTSCParTxt, NTSCParTxt, NULL},
};

struct PAR_STRUCT code AudioSignalPar[5] = {
	{ OFFParTxt, 			OFFParTxt, 			&AudioSignalPar[1]},
	{ S500HZParTxt, 	S500HZParTxt, 	&AudioSignalPar[2]},
	{ S1KHZParTxt, 		S1KHZParTxt, 		&AudioSignalPar[3]},
	{ SEBU1KHZParTxt, SEBU1KHZParTxt, &AudioSignalPar[4]},
	{ S8KHZParTxt, 		S8KHZParTxt, 		NULL},
};

struct PAR_STRUCT code AESEBULevelPar[9] = {
	{ SILENCEParTxt, SILParTxt, 	&AESEBULevelPar[1]},
	{ DB0FSParTxt, 	DB0FSParTxt, 	&AESEBULevelPar[2]},
	{ DB9FSParTxt, 	DB9FSParTxt, 	&AESEBULevelPar[3]},
	{ DB12FSParTxt, DB12FSParTxt, &AESEBULevelPar[4]},
	{ DB14FSParTxt, DB14FSParTxt, &AESEBULevelPar[5]},
	{ DB15FSParTxt, DB15FSParTxt, &AESEBULevelPar[6]},
	{ DB16FSParTxt, DB16FSParTxt, &AESEBULevelPar[7]},
	{ DB18FSParTxt, DB18FSParTxt, &AESEBULevelPar[8]},
	{ DB20FSParTxt, DB20FSParTxt, NULL},
};

struct PAR_STRUCT code AudioFrequencyPar[2] = {
	{ F441KHZParTxt, F441KHZParTxt, &AudioFrequencyPar[1]},
	{ F48KHZParTxt,  F48KHZParTxt, 	NULL},
};

struct PAR_STRUCT code UnitPar[2] = {
	{ BBParTxt, BBParTxt, &UnitPar[1]},
	{ SPGParTxt, SPGParTxt, NULL},
};

struct PAR_STRUCT code OffOnPar[2] = {
	{ OFFParTxt, OFFParTxt, &OffOnPar[1]},
	{ ONParTxt, ONParTxt, NULL},
};

struct PAR_STRUCT code MinMaxPar[2] = {
	{ MINParTxt, MINParTxt, &MinMaxPar[1]},
	{ MAXParTxt, MAXParTxt, NULL},
};


/***************************************************************************/
// Temporary variables PRIVATE NOT overlayable

static unsigned int Suffix[4]=
{
	1,1,1,1
};

static UC ParState;

//static UC EventIndex;
//static char EventItem[100];

static int IntVal;

static UC UcharVal;

//static bool PasswordProtected = true;  //HUSK AT ÆNDRE TILBAGE
static bool PasswordProtected = false;

extern UC ParCnt;

char*		ProductNumber="PT5201";
char*		ProductName="PT5201 Compact VariTime Sync Generator";
char*		CompanyName="PTV";
char*		KU_Number="------";
char*		SwRev="0.1";


/*extern char*		ProductNumber;
extern char*		ProductName;
extern char*		CompanyName;
extern char*		KU_Number;
extern char*		SwRev;
*/
//extern NvPointer	KuNo;

/**************************************************************************/
/* CLSCommand																	 								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Clear status command																				*/
/* Remarks:		No parameters																								*/
/* Returns:		-																														*/
/* Updates:																																*/
/**************************************************************************/
void CLSCommand( void)
{
	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else
			ResetErrorEventQueue();
	}
}

/**************************************************************************/
/* ESECommand																	 								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970627	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Set/read the Standard Event Status Enable Register					*/
/* Remarks:		--																													*/
/* Returns:		-																														*/
/* Updates:																																*/
/***************************************************************************/
void ESECommand( void)
{
	register int tmp;

	switch ( ParameterType)
	{
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0)
			{
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

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParState)
				SCPIError = ParNotAllowed;
			else
				rs232printf( 12, "%d\n", ESEReg);
		}
		else
		{
			if ( ParState == 0)
				SCPIError = MissingPar;
			else
				ESEReg = UcharVal;
		}
	}
}

/**************************************************************************/
/* ESRCommand																									 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970627	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Read and clear the Standard Event Status Register						*/
/* Remarks:		No parameters																								*/
/*						The register will reset to zero.														*/
/* Returns:		-																														*/
/* Updates:																																*/
/***************************************************************************/
void ESRCommand( void)
{

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			rs232printf( 12, "%d\n", UpdateESRRegister());

			ESRReg = 0;
		}
		else
			SCPIError = SyntaxErr;
	}
}

/**************************************************************************/
/* OPCCommand																	 								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Operation Complete Command/Query														*/
/* Remarks:		No parameters																								*/
/* Returns:		-																														*/
/* Updates:																																*/
/**************************************************************************/
void OPCCommand( void)
{

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
			;
		else
			;
	}
}

/**************************************************************************/
/* SRECommand															 		 								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970627	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Set/read the Service Request Enable Register								*/
/* Remarks:		Bit 6 in *SRE? register is always ZERO											*/
/*						Bit 6 in *SRE register is ignored														*/
/* Returns:		-																														*/
/* Updates:																																*/
/**************************************************************************/
void SRECommand( void)
{
	register int tmp;

	switch ( ParameterType)
	{
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0)
			{
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

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParState)
				SCPIError = ParNotAllowed;
			else
				rs232printf( 12, "%d\n", SREReg);
		}
		else
		{
			if ( ParState == 0)
				SCPIError = MissingPar;
			else
				SREReg = UcharVal & 0xBF;
		}
	}
}

/**************************************************************************/
/* STBCommand																									 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970627	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Read the Status Byte Register																*/
/* Remarks:		No parameters																								*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void STBCommand( void)
{
	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			UpdateSTBRegister();

			rs232printf( 12, "%d\n", STBReg);
		}
		else
			SCPIError = SyntaxErr;
	}
}

/**************************************************************************/
/* TSTCommand																									 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970630	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Selftest query																							*/
/* Remarks:		No parameters																								*/
/* Returns:		-																														*/
/* Updates:																																*/
/**************************************************************************/
void TSTCommand( void)
{
	unsigned int	test[2];
	unsigned char tmp[2];

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
		{
			;
		}
		else
		
		{
			if (ParCnt!=2)
			{
				SCPIError = SyntaxErr;	
				return;
			}

			sscanf(Cmd,"%d,%d",&test[0],&test[1]);

			tmp[0] = test[0] & 0x0FF;
			tmp[1] = test[1] & 0x0FF;

			TransmitIIC( 0x40, 2 ,tmp ,IIC_PTV);

			TransmitIIC( 0x40, 1 ,tmp ,IIC_PTV);
			ReceiveIIC( 0x40,  1, &tmp[1] ,IIC_PTV);

			rs232printf( 12,"%03d,%03d\n", (int)tmp[0], (int)tmp[1]);

		}
	}
/*	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
			;
		else
			SCPIError = SyntaxErr;
	}*/
}

/**************************************************************************/
/* WAICommand																									 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970630	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Wait-to-continue command																		*/
/* Remarks:		No parameters																								*/
/* Returns:		-																														*/
/* Updates:																																*/
/**************************************************************************/
void WAICommand( void)
{

	unsigned int	test[2];
	unsigned char tmp[2];

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
		{
			;
		}
		else
		
		{
			if (ParCnt!=1)
			{
				SCPIError = SyntaxErr;	
				return;
			}

			sscanf(Cmd,"%d",&test[0]);

			tmp[0] = test[0] & 0x0FF;

			TransmitIIC( 0x40, 1 ,tmp ,IIC_PTV);
			ReceiveIIC( 0x40,  1, &tmp[1] ,IIC_PTV);

			rs232printf( 12,"%03d,%03d\n", (int)tmp[0], (int)tmp[1]);

		}
	}
/*
	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
			SCPIError = SyntaxErr;
		else
			;
	}
*/
}

/**************************************************************************/
/* RSTCommand																									 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 961124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Reset comamnd																								*/
/* Remarks:		No parameters																								*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void RSTCommand( void)
{
	unsigned int	test, i;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest)
		{
			;
		}
		else
		
		{
			if (ParCnt!=1)
			{
				SCPIError = SyntaxErr;	
				return;
			}

			sscanf(Cmd,"%d", &test);

			switch ( test)
			{
				case 1:
					BBSPGReset( true, true, true);			// Set program bit and reset units

					for ( i = 0; i < 60000; i++)				//  Wait
						;

					BBSPGReset( false, true, true);			// Release reset for unit to be programmed
					break;

				case 2:
					BBSPGReset( true, true, true);			// Set program bit and reset units

					for ( i = 0; i < 60000; i++)				//  Wait
						;

					BBSPGReset( true, false, true);			// Release reset for unit to be programmed
					break;

				default:
					BBSPGReset( true, true, true);			// Set program bit and reset units
		
					for ( i = 0; i < 60000; i++)				//  Wait
						;

					BBSPGReset( false, false, false);		// Release reset for unit to be programmed
					break;
			}

		}
	}
}

/**************************************************************************/
/* IDNCommand																								   RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 980211	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Identification query																				*/
/* Remarks:		No parameter, Query only																		*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void IDNCommand( void)
{

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			rs232puts( CompanyName);

			rs232putchar( ',');

			rs232puts(ProductName);

			rs232putchar( ',');

			rs232puts(KU_Number);

			rs232putchar( ',');

			rs232puts(SwRev);

			rs232putchar( '\n');
		}
		else
			SCPIError = SyntaxErr;
	}
}

/**************************************************************************/
/* RCLCommand																									 RS232CMD.C */
/*																																				*/
/* Author:		    																												*/
/* Revised:									     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:																																*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void RCLCommand(void) 
{
	int	Reg = 0;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			SCPIError = SyntaxErr;
		}
		else
		{
			if ((ParCnt!=1) || ( sscanf(Cmd,"%d",&Reg)!=1))
			{
				SCPIError = SyntaxErr;	
				return;
			}
			if ((Reg<1) || (Reg>3))
			{
				SCPIError = DataOutOfRange;
				return;
			}
/*			if (!Recall(Reg-1))
			{
				SCPIError = ExecErr;
				return;
			}*/
		}
	}
}

/**************************************************************************/
/* SAVCommand																									 RS232CMD.C */
/*																																				*/
/* Author:		    																												*/
/* Revised:									     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:																																*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void SAVCommand(void) 
{
	int	Reg=0;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			SCPIError = SyntaxErr;
		}
		else
		{
			if ((ParCnt!=1) || (sscanf(Cmd,"%d",&Reg)!=1))
			{
				SCPIError = SyntaxErr;	
				return;
			}

			if ((Reg<1) || (Reg>3))
			{
				SCPIError = DataOutOfRange;
				return;
			}

/*			if (!Store(Reg-1))
			{
				SCPIError = ExecErr;
				return;
			}*/

		}
	}
}

/**************************************************************************/
/* SystemError																								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		000401				     																						*/
/*																																				*/
/* Function:	Write the next entry in the error queue (FIFO) to RS232 		*/
/* Remarks:		No parameters, Query only																		*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void SystemError( void)
{
	int i;
	code char *ptr;
	char buf[12];

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt > 0)
				SCPIError = ParNotAllowed;
			else
			{
				if (( i = ReadErrorEventQueue()) >= 0)
				{
//				rs232printf( 20, "%d,\"", ErrorTxt_Array[i].ErrorNo);

					sprintf( buf, "%d,\"", ErrorTxt_Array[i].ErrorNo);
					rs232puts( buf);

					ptr = ErrorTxt_Array[i].ErrorTxt;

					while ( *ptr)
						rs232putchar( *ptr++);

					 rs232puts( "\"\n");
				}
			}
		}
		else
			SCPIError = SyntaxErr;
	}
}

/**************************************************************************/
/* SystemVersion																							 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Write the SCPI version to RS232															*/
/* Remarks:		No parameter, Query only																		*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void SystemVersion( void)
{
	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
				rs232puts( "1995.0\n");
		}
		else
			SCPIError = SyntaxErr;
	}
}

/**************************************************************************/
/* SystemPreset																								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970206	    															*/
/* Revised:		980611			     																						*/
/*																																				*/
/* Function:	Ativate the specified preset																*/
/* Remarks:		--																													*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void SystemPreset(void) {

/*	switch ( ParameterType) {
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
*/
}

/**************************************************************************/
/* SystemPresetStore																					 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970206	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Store actual setup in preset																*/
/* Remarks:		--																													*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void SystemPresetStore( void)
{

	SAVCommand();
}

/**************************************************************************/
/* SystemPresetName																						 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970206	    															*/
/* Revised:		990305					     																				*/
/*																																				*/								
/* Function:	Name a specific preset number.															*/
/* Remarks:		--																													*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void SystemPresetName( void)
{

	RCLCommand();
}

/**************************************************************************/
/* StatusOperEvent																						 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970630	    															*/
/* Revised:		000402				     																					*/
/*																																				*/
/* Function:	Set/read the status operation event register								*/
/* Remarks:		Reading the event register clears it												*/
/* Returns:		-																														*/
/* Updates:		OperEventReg																								*/
/**************************************************************************/
void StatusOperEvent( void)
{
	if ( CmdExecute && !SCPIError)
	{
		if ( ParCnt)
			SCPIError = ParNotAllowed;
		else
		{
			if ( CmdRequest) {
				rs232printf( 12, "%d\n", OperEventReg);

				OperEventReg = 0;
			}
			else
				SCPIError = SyntaxErr;
		}
	}
}

/**************************************************************************/
/* StatusOperCondition																				 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970630	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Read the status questionable condition register							*/
/* Remarks:		No parameters																								*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void StatusOperCondition( void)
{

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
			rs232printf( 12, "%d\n", OperConditionReg);
		else
			SCPIError = SyntaxErr;
	}
}

/**************************************************************************/
/* StatusOperEnable																						 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970630	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Set/read the status operaion enable register								*/
/* Remarks:		--																													*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void StatusOperEnable( void)
{

	switch ( ParameterType)
	{
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0)
			{
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

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParState)
				SCPIError = ParNotAllowed;
			else
				rs232printf( 12, "%d\n", OperEnableReg);
		}
		else
		{
			if ( ParState == 0)
				SCPIError = MissingPar;
			else
				OperEnableReg = IntVal & 0xEFFF;
		}
	}
}

/***************************************************************************/
/* StatusQuestEvent																			 			 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970630	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Set/read the status questionablen event register						*/
/* Remarks:		Reading the event register clears it												*/
/* Returns:		-																														*/
/* Updates:		QuestEventReg																								*/
/***************************************************************************/
void StatusQuestEvent( void)
{

	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			rs232printf( 12, "%d\n", QuestEventReg);

			QuestEventReg = 0;
		}
		else
			SCPIError = SyntaxErr;
	}
}

/**************************************************************************/
/* StatusQuestCondition																		 		 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970630	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Read the status operation condition register								*/
/* Remarks:		No parameters																								*/
/* Returns:		-																														*/
/* Updates:		-																														*/
/**************************************************************************/
void StatusQuestCondition( void)
{
	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
			rs232printf( 12, "%d\n", QuestConditionReg);
		else
			SCPIError = SyntaxErr;
	}
}

/**************************************************************************/
/* StatusQuestEnable																					 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970630	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Set/read the status questionable enable register						*/
/* Remarks:		--																													*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void StatusQuestEnable( void)
{
	switch ( ParameterType)
	{
		case P_NoParameter:
			ParState = 0;
			break;

		case P_DecNum:
			if ( ParState == 0)
			{
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

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParState)
				SCPIError = ParNotAllowed;
			else
				rs232printf( 12, "%d\n", QuestEnableReg);
		}
		else
		{
			if ( ParState == 0)
				SCPIError = MissingPar;
			else
				QuestEnableReg = IntVal & 0xEFFF;
		}
	}
}

/**************************************************************************/
/* StatusPreset																								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void StatusPreset( void)
{
}

/**************************************************************************/
/* StatusPTSuffix																					 		 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void StatusPTSuffix( void)
{

	if ( NumSuffix != 5201)
		SCPIError = SyntaxErr;
}

/**************************************************************************/
/* StatusPT																 										 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:																																*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void StatusPT( void)
{

/*	if ( ParameterType != P_NoParameter)
		SCPIError = ParNotAllowed;

	if ( CmdExecute && !SCPIError) {
		if ( CmdRequest && ( NumSuffix == 5765)) {
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
*/
}

/**************************************************************************/
/* User_Default_Save													 	   						 RS232CMD.C */
/* 																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:		--																													*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void User_Default_Save( void)
{
}

/**************************************************************************/
/* DiagnosticErrorQueue																	 	    RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:		--																													*/
/* Returns:																																*/
/* Updates:																																*/
/**************************************************************************/
void DiagnosticErrorQueue( void)
{
/*	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest) {
		}
		else
			SCPIError = SyntaxErr;
	}
*/
}

/**************************************************************************/
/* DiagnosticErrorQueueReset										 					    RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Reset the internal PT5230 error queue, NOT the SCPI queue		*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		Internal error queue																				*/
/**************************************************************************/
void DiagnosticErrorQueueReset( void)
{
/*	register int i;

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
*/
}

/**************************************************************************/
/* GenlockGen															 	   								 RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980915				     																					*/
/*																																				*/
/* Function:	Return the Genlock Configuration.														*/
/* Remarks:		Request only																								*/
/* Returns:		--																													*/
/* Updates:	  --																													*/
/**************************************************************************/
void GenlockGen( void)
{
}

/**************************************************************************/
/* GenlockGenSystem																		 	   		 RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		000405				     																					*/
/*																																				*/
/* Function:	Handle the genlock system command														*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void GenlockGenSystem( void)
{
	int system;
	char code *str;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
				str = GenlockSystemPar[GenlockSetup.System].LongName;

				while ( *str)
					rs232putchar( *str++);

				rs232putchar('\n');
			}
		}
		else
		{
			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				system = FindParameter( Cmd, GenlockSystemPar);
	
				if ( system > -1)
					SetGenlockSystem( system);
				else
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* GenlockGenDelay		 										 								    RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970206	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Handle the genlock timing command														*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void GenlockGenDelay( void)
{
	int field, line;
	float offset;
	long samples;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
				PrintSamplesToFLT( GenlockSetup.System, GenlockSetup.Delay);
		}
		else
		{
			if (( ParCnt != 3) || ( sscanf( Cmd, "%d,%d,%f", &field, &line, &offset) != 3))
				SCPIError = SyntaxErr;
			else
			{
				if (( samples = FLTToSamples( GenlockSetup.System, field, line, offset)) == -1)
					SCPIError = ExecErr;
				else
					GenlockSetup.Delay = (UL) samples;
			}
		}
	}
}

/**************************************************************************/
/* BBSuffix																 	  						 	   RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:	Get suffix of the Black Burst generator											*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		Suffix																									 		*/
/**************************************************************************/
void BBGenSuffix( void) {

	if ( NumSuffix > 3)
		SCPIError = HdrSuffixOutOfRange;
	else
		Suffix[0] = NumSuffix-1;

}

/**************************************************************************/
/* BBGen																								 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Handle the Black Burst command.															*/
/* Remarks:		Request only																								*/
/* Returns:		--																													*/
/* Updates:		--																												 	*/
/**************************************************************************/
void BBGen( void)
{
}

/**************************************************************************/
/* BBGenSystem		 												 	    							 RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:	Handle the Black Burst system-command.											*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void BBGenSystem( void)
{
	int system;
	char code *str;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
				str = SystemPar[BBSetup[Suffix[0]].System].LongName;

				while ( *str)
					rs232putchar( *str++);

				rs232putchar('\n');
			}
		}
		else
		{
			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				system = FindParameter( Cmd, SystemPar);

				if ( system > -1)
					SetBBSystem( Suffix[0], system);
				else				
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* BBGenScHPhase													 	   								 RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/			
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:	Handle the Black Burst ScH-Phase command.										*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void BBGenScHPhase( void)
{
	int schphase;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
				rs232printf( 6, "%d\n", BBSetup[Suffix[0]].ScHPhase);
		}
		else
		{
			if (( ParCnt != 1) || ( sscanf( Cmd, "%d", &schphase) != 1))
				SCPIError = SyntaxErr;
			else
			{
				if (( schphase < ScHPhaseLow) || ( schphase > ScHPhaseHigh))
					SCPIError = DataOutOfRange;
				else
					SetBBScHPhase( Suffix[0], schphase);
			}
		}
	}
}

/**************************************************************************/
/* BBGenDelay	 														 	    							 RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		000407				     																					*/				
/*																																				*/
/* Function:	Handle the Black Burst timing command.											*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void BBGenDelay( void)
{
	int field, line;
	float offset;
	long samples;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
				PrintSamplesToFLT( BBSetup[Suffix[0]].System, BBSetup[Suffix[0]].Delay);
		}
		else
		{
			if (( ParCnt != 3) || ( sscanf( Cmd, "%d,%d,%f", &field, &line, &offset) != 3))
				SCPIError = SyntaxErr;
			else
			{
				if (( samples = FLTToSamples( BBSetup[Suffix[0]].System, field, line, offset)) == -1)
					SCPIError = ExecErr;
				else
					SetBBDelay( Suffix[0], (UL) samples);
			}
		}
	}
}

/**************************************************************************/
/* CBGen																									 	   RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 980211	    															*/
/* Revised:		980611				     																					*/
/*																																				*/
/* Function:	Return the setting of a Analog-TPG.													*/
/* Remarks:		Request only.																								*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/***************************************************************************/
void CBGen( void)
{
}

/**************************************************************************/
/* CBGenPattern																					 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 980211	    															*/
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:	Handle the Colorbar pattern command.												*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void CBGenPattern( void)
{
	int pattern;
	char code *str;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
				str = CBPatternPar[CBSetup.Pattern].LongName;

				while ( *str)
					rs232putchar( *str++);

				rs232putchar('\n');
			}
		}
		else
		{
			pattern = FindParameter( Cmd, CBPatternPar);

			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				if ( pattern > -1)
				{
					if ( SetCBPattern( pattern) != OK)
						SCPIError = ExecErr;
				}
				else
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* CBGenSystem														 	    							 RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 98021	    															*/
/* Revised:		000405				     																					*/
/*																																				*/
/* Function:	Handle the Colobar system-command.													*/
/* Remarks:		This command will possibly change pattern and/or delay if	 	*/
/*            these are not valid in the selected system									*/
/* Returns:		--																													*/
/* Updates:	   --																													*/
/***************************************************************************/
void CBGenSystem( void)
{
	int system;
	char code *str;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
				str = SystemPar[CBSetup.System].LongName;

				while ( *str)
					rs232putchar( *str++);

				rs232putchar('\n');
			}
		}
		else
		{
			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				system = FindParameter( Cmd, SystemPar);
	
				if ( system > -1)
					SetCBSystem( system);
				else
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* CBGenDelay															 	    							 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 980211	    															*/
/* Revised:		000405				     																					*/
/*																																				*/
/* Function:	Handle the Colorbar delay command.													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void CBGenDelay( void)
{
	int field, line;
	float offset;
	long samples;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
				PrintSamplesToFLT( CBSetup.System, CBSetup.Delay);
		}
		else
		{
			if (( ParCnt != 3) || ( sscanf( Cmd, "%d,%d,%f", &field, &line, &offset) != 3))
				SCPIError = SyntaxErr;
			else
			{
				if (( samples = FLTToSamples( CBSetup.System, field, line, offset)) == -1)
					SCPIError = ExecErr;
				else
					SetCBDelay( samples);
			}
		}
	}
}

/**************************************************************************/
/* CBGenScHPhase														 	   							 RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 980211	    															*/
/* Revised:		000401				     																					*/
/*																																				*/
/* Function:	Handle the Colorbar ScH-Phase-command.											*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void CBGenScHPhase( void)
{
	int schphase;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
				rs232printf( 6, "%d\n", CBSetup.ScHPhase);
		}
		else
		{
			if (( ParCnt != 1) || ( sscanf( Cmd, "%d", &schphase) != 1))
				SCPIError = SyntaxErr;
			else {
				if (( schphase < ScHPhaseLow) || ( schphase > ScHPhaseHigh))
					SCPIError = DataOutOfRange;
				else
					SetCBScHPhase( schphase);
			}
		}
	}
}

/**************************************************************************/
/* CBGenEmbAudioSignal											 	   							 RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 980211	    															*/
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:	Handle the Colorbar embedded audio signal										*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void CBGenEmbAudioSignal( void)
{
	int signal;
	char code *str;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
				str = EmbAudioSignalPar[CBSetup.EmbAudioSignal].LongName;

				while ( *str)
					rs232putchar( *str++);

				rs232putchar('\n');
			}
		}
		else
		{
			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				signal = FindParameter( Cmd, EmbAudioSignalPar);
	
				if ( signal > -1)
					SetCBEmbAudioSignal( signal);
				else
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* AudioGen																							 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:		000402				     																					*/
/*																																				*/
/* Function:	Handle the AES-EBU Audio generator command.									*/
/* Remarks:		Request only																								*/
/* Returns:	  --																													*/
/* Updates:		--																													*/
/**************************************************************************/
void AudioGen( void)
{
}

/**************************************************************************/
/* AudioGenOutput																				 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void AudioGenOutput( void)
{
	int output;
	char code *str;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
//				str = AudioOutputPar[AudioSetup.Output].LongName;

				while ( *str)
					rs232putchar( *str++);

				rs232putchar('\n');
			}
		}
		else
		{
			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				output = FindParameter( Cmd, AudioOutputPar);
	
				if ( output > -1)
					SetAudioOutput( output);
				else
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* AudioGenSystem																				 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:																															*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void AudioGenSystem( void)
{
	int system;
	char code *str;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
//				str = AESEBUSystemPar[AudioSetup.System].LongName;

				while ( *str)
					rs232putchar( *str++);

				rs232putchar('\n');
			}
		}
		else
		{
			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				system = FindParameter( Cmd, AESEBUSystemPar);
	
				if ( system > -1)
					SetAudioSystem( system);
				else
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* AudioGenSignal																				 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:		000402				     																					*/
/*																																				*/
/* Function:	Handle the AES-EBU Audio Generator audio-signal command.		*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void AudioGenSignal( void)
{
}

/**************************************************************************/
/* AudioGenLevel																				 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:		000402				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void AudioGenLevel( void)
{
}

/**************************************************************************/
/* AudioGenTiming																				 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:		000402				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void AudioGenTiming( void)
{
}

/**************************************************************************/
/* AudioGenWordClock																		 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void AudioGenWordClock( void)
{
	int frequency;
	char code *str;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
//				str = AudioFrequencyPar[AudioSetup.Frequency].LongName;

				while ( *str)
					rs232putchar( *str++);

				rs232putchar('\n');
			}
		}
		else
		{
			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				frequency = FindParameter( Cmd, AudioFrequencyPar);
	
				if ( frequency > -1)
					SetAudioWordClock( frequency);
				else
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* AudioGenClick																				 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:		000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void AudioGenClick( void)
{
	int click;

	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
/*			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
				rs232printf( 6, "%d\n", AudioSetup.Click);
*/
		}
		else
		{
			if (( ParCnt != 1) || ( sscanf( Cmd, "%d", &click) != 1))
				SCPIError = SyntaxErr;
			else {
				if (( click != click1s) && ( click != click3s))
					SCPIError = DataOutOfRange;
				else
					SetAudioClick( click);
			}
		}
	}
}

/**************************************************************************/
/* FactPassword																					 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:	  000402				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactPassword( void)
{
	if ( CmdExecute && !SCPIError)
	{
		if ( CmdRequest)
		{
			if ( ParCnt)
				SCPIError = ParNotAllowed;
			else
			{
				if ( PasswordProtected)
					rs232puts( "ON\n");
				else
					rs232puts( "OFF\n");
			}
		}
		else
		{
			if ( ParCnt != 1)
				SCPIError = SyntaxErr;
			else
			{
				if ( !strcmp( Cmd, "ON"))
					PasswordProtected = false;
				else
					if ( !strcmp( Cmd, PasswordText))
						PasswordProtected = false;
				else
					SCPIError = IllegalParValue;
			}
		}
	}
}

/**************************************************************************/
/* FactV24Command																				 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000402	    															*/
/* Revised:	  000402				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactV24Command( void)
{
	UC address;
	char command[30];

	if ( !PasswordProtected)
	{
		if ( CmdExecute && !SCPIError)
		{
			if ( CmdRequest)
				SCPIError = SyntaxErr;
			else
			{
				if (( ParCnt != 2) || ( sscanf( Cmd, "%d,%s", &address, command) != 2))
					SCPIError = SyntaxErr;
				else
				{
		//		sio0SndCommand( 12, BB_IIC_address, "%s %d;", cmd, BBSetup[output].Delay);
				}
			}
		}
	}
}

/**************************************************************************/
/* FactV24Transparent																		 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000407	    															*/
/* Revised:	  000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactV24Transparent( void)
{
	int unit;

	if ( !PasswordProtected)
	{
		if ( CmdExecute && !SCPIError)
		{
			if ( CmdRequest)
				SCPIError = SyntaxErr;
			else
			{
				unit = FindParameter( Cmd, UnitPar);
	
				if ( unit > -1)
					BBSPGCommunication( unit);
				else
					SCPIError = DataOutOfRange;
			}
		}
	}
}

/**************************************************************************/
/* FactMainIDN		 																			 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000407	    															*/
/* Revised:	  000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactMainIDN( void)
{
}


/**************************************************************************/
/* FactMainNC																						 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000407	    															*/
/* Revised:	  000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactMainNC( void)
{
}

/**************************************************************************/
/* FactMainSystem	 																			 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000407	    															*/
/* Revised:	  000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactMainSystem( void)
{
}

/**************************************************************************/
/* FactMainFMSDateProd																	 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000407	    															*/
/* Revised:	  000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactMainFMSDateProd( void)
{
}

/**************************************************************************/
/* FactMainFMSDateUpd																		 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000407	    															*/
/* Revised:	  000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactMainFMSDateUpd( void)
{
}

/**************************************************************************/
/* FactMainFMSLocProd																	 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000407	    															*/
/* Revised:	  000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactMainFMSLocProd( void)
{
}

/**************************************************************************/
/* FactMainFMSLocUpd																		 	     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 000407	    															*/
/* Revised:	  000407				     																					*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void FactMainFMSLocUpd( void)
{
}

/**************************************************************************/
/* FindParameter														 							     RS232CMD.C	*/
/*																																				*/
/* Author:		Kim Engedahl, DEV, 941124	    															*/
/* Revised:		980518				     																					*/
/*																																				*/
/* Function:	Find the character defined parameter												*/
/* Remarks:		--																													*/
/* Returns:		The array number containing the parameter or max. array+1		*/
/* Updates:		--																													*/
/**************************************************************************/
int FindParameter( char* par, struct PAR_STRUCT code* ParPtr)
{
	int tmp = 0;

	while ( ParPtr)
	{
		if (( !Cstrcmp( par, ParPtr->LongName)) ||
			 ( !Cstrcmp( par, ParPtr->ShortName)))
			return(tmp);

		ParPtr = ParPtr->SameLevel;

		if ( tmp < 255)
			tmp++;
		else
			break;
	}

	return( -1);
}


/**************************************************************************/
/* FLTToSamples															  								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970211	    															*/
/* Revised:		000401				     																					*/
/*																																				*/	
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		FLT in samples, or -1 if illegal														*/
/* Updates:		--																													*/
/***************************************************************************/
long FLTToSamples( UC system, int F, int L, float T) {

	long samples;
	char minus;

	if (( F == 0) && ( L == 0) && ( T == 0))
		return( 0);

	if (( F <= 0) && ( L <= 0) && ( T <= 0))
	{
		minus = 1;
	}
	else
	{
		if (( F >= 0) && ( L >= 0) && ( T >= 0))
			minus = 0;
		else
			return( -1);
	}

																			// 1 sample = (1/27MHz)/256
	samples = ( long)(( T*6912L)/1000L + 0.5);	

	if ( system < NTSC)									// If selected system is PAL..
	{

		if ( labs( samples) > 442367L)	 	// Test for illegal time var
			return( -1);

		switch ( F)												// Test for illegal field & line
		{
			case 0:
				if (( minus && ( L < -311)) || ( !minus && ( L > 312)))
					return( -1);
				break;

			case -1:
			case -3:
			case 2:
				if ( abs( L) > 312)
					return( -1);
				break;

			case -2:
			case 1:
			case 3:
				if ( abs( L) > 311)
					return( -1);
				break;

			case 4:
				if (( L > 0) || ( T > 0.0))
					return( -1);
				break;

			default:
				return( -1);
		}

		if ( !minus)											// if posistive delay..
		{
			if ( F % 2)
			{
				L += 313;
				F--;
			}
		}
		else															// ..else delay is negative
		{
			F += 7;

			samples += 442367L;

			if ( F % 2) {
				L += 624;
				F--;
			}
			else
				L += 312;

			if ( T == 0)
				samples++;
		}

		samples += F*138240000L + L*442368L;
	}
	else																// ..else system is NTSC
	{

		if ( labs( samples) > 439295L)		// Test for illegal time var
			return( -1);

		switch ( F)												// Test for illegal field & line
		{
			case 0:
				if (( minus && ( L < -261)) || ( !minus && ( L > 262)))
					return( -1);
				break;

			case -1:
				if ( abs( L) > 262)
					return( -1);
				break;

			case 1:
				if ( abs( L) > 261)
					return( -1);
				break;

			case 2:
				if (( L > 0) || ( T > 0.0))
					return( -1);
				break;

			default:
				return( -1);
		}

		if ( !minus)										// if posistive delay..
		{						

			if ( F % 2)
			{
				L += 263;
				F--;
			}
		}
		else															// ..else delay is negative
		{																	
			F += 3;

			samples += 439295;

			if ( F % 2)
			{
				L += 524;
				F--;
			}
			else
				L += 262;

			if ( T == 0)
				samples++;
		}

		samples += F*115315200L + L*439296L;
	}

	return( samples);
}

/**************************************************************************/
/* PrintSamplesToFLT												  								 RS232CMD.C */
/*																																				*/
/* Author:		Kim Engedahl, DEV, 970211	    															*/
/* Revised:		000401												     													*/
/*																																				*/
/* Function:	--																													*/
/* Remarks:		--																													*/
/* Returns:		--																													*/
/* Updates:		--																													*/
/**************************************************************************/
void PrintSamplesToFLT( UC system, long samples) {

	char sign;
	int frame, field, line;

	sign = '+';

	if ( system < NTSC)												// If system selected is PAL..
	{
		// 552960000 = 2*625*1728*256
		// 276480000 = 625*1728*256
		// 442368    = 1728*256

		if ( samples >= 552960000L)
		{
			samples--;

			frame = samples / 276480000L;
			line = ( samples % 276480000L) /  442368L;
			samples = ( samples % 276480000L) %  442368L;
			field = 2*frame;

			if ( line > 312)
			{
				field++;
				line = 624-line;
			}
			else
				line = 312-line;

			samples = 442368L - 1 - samples;
			field = 7-field;

			if ( field != 4)
				sign = '-';
		}
		else
		{
			frame = samples / 276480000L;
			line = ( samples % 276480000L) / 442368L;
			samples = ( samples % 276480000L) % 442368L;
			field = 2*frame;

			if ( line > 312)
			{
				field++;
				line -= 313;
			}
		}
	}
	else {                  									// ..system selected is NTSC
		if ( samples >= 230630400L)
		{
			// 230630400 = 525*1716*256
			// 439296    = 1716*256

			samples--;

			frame = samples / 230630400L;
			line = ( samples % 230630400L) / 439296L;
			samples = ( samples % 230630400L) % 439296L;

			field = 2*frame;

			if ( line > 262)
			{
				field++;
				line = 524-line;
			}
			else
				line = 262-line;

			samples = 439296L - 1 - samples;
			field = 3 - field;

			if ( field != 2)
				sign = '-';
		}
		else
		{
			frame = samples / 230630400L;
			line = ( samples % 230630400L) / 439296L;
			samples = ( samples % 230630400L) % 439296L;
			field = 2*frame;

			if ( line > 262)
			{
				field++;
				line -= 263;
			}
		}
	}

	rs232printf( 12, "%c%d,", sign, abs( field));
	rs232printf( 12, "%c%d,", sign, abs( line));
	rs232printf( 12, "%c%.1f\n", sign, fabs((float) ( samples*1000L)/6912L));
}
