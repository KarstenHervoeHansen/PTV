#include <gpib.h>
/* bbcal.c
 PT5210:   Analog Black Burst Calibration
 BB har interne RS-232 adresser:  BB1-2 = 0 	 BB3-4 = 17d
                                  BB5-6 = 19d    BB7-8 = 18d
 961122,PF

 Rettelser:
 970224 PF/SNJ	NTSC fasen er flyttet 2 fields, således at ScH med linie 11 puls er ok ved 
 				måling med PM5662
				Se konstanten PhaseOffsetNTSC

*/

#include <rs232.h>
#include <ansi_c.h>
#include "phase.h"
#include <utility.h>
#include <formatio.h>
#include <userint.h>
#include "cviutil.h"
#include "bb_sync.h"
#include <hp34401a.h>
#include "pt5210.h"
#include "cfg.h"
#include "spgcal.h"
#include "aescal.h"
#include "sditest.h"
#include "ltctest.h"
#include "vcc.h"
#include "fan.h"

#define SYNC_DAC 0
#define OFFSET_DAC 1

#define IEEEBUS 1



int syncPNL,
	phasePNL;

const int  		ScHOffsetPAL = -540,
		  		PhaseOffsetPAL = -5450;

const int  		ScHOffsetNTSC = -30,			
		  		PhaseOffsetNTSC = 230630400-8250;   // (525*2*1716*256)/2 

const double	Max5 = 5.1, 
				Min5 = 5.0,
				Max12 = 12.5,
				Min12 = 11.5,
				MaxM5 = -4.8,
				MinM5 = -5.2;

const double 	SPGTP7Max = 3.4,
			 	SPGTP7Min = 3.0,
			 	AudioTP1Min = -3.5,
			 	AudioTP1Max =  3.5,
			 	LTC_TP7MaxG = 2.5,
			 	LTC_TP7MinG = 1.7,
			 	LTC_TP7MaxM = 3.4,
			 	LTC_TP7MinM = 2.6;

const int		FanRunTemp = 45;

const char 		CalDataFilePath[50] = {""};		  // where to save caldata on disk
const char		CalDataFileExtention[5] = {".dat"};  // file extention for caldata on disk

unsigned char	DACValue;

int 			PhaseOffset,
    			ScHOffset;

int 			ScHValue,
				BBNumber,
				PhaseValue;
				
int  CheckingOven,
	 CheckingPS;
	 CheckingAudioVCO,
	 CheckingLTCVCO;
	 ;				

int SaveToFile;

int	VoltOk,
	FanOk,
	SPGTP7Ok,
	AudioTP1Ok,
	LTCTP7Ok,
	err,
	n,
    ConfirmReply;

int BBKU[8],
	SDIBLACKKU[3],
	SDITSGKU,
	ANALOGTSGKU,
	SPGKU,
	SDIGenlKU,
	AudioKU,
	LTCKU;
	
char BBCalStr[8][100],
	 AudioCalStr[100],
	 SPGCalStr[100],
	 SDIGenlCalStr[100],
	 SDICalStr[4][100],
	 SDITSGCalStr[100],
	 LTCCalStr[100];


//-------------------------------------------------------------------------------
int InitBB(int BBNo);
int SaveCalData(void);
int ShowCalFiles(void);
int ReadADC(int *Temp, int *Plus5, int *Minus5, int *Plus12);



//--------------------------------------------------------------------------------
void SetDAC(unsigned char dac, int BB, int DACType){
char DACStr[11];

 Fmt(DACStr,"%s<%i",dac);
 
 if (DACType == SYNC_DAC){
  if ((BB % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HD',",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'JD',",0,-1);
 }  

 if (DACType == OFFSET_DAC){
  if ((BB % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HC',",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'JC',",0,-1);
 }  
 
 CopyString(DUTStr,StringLength(DUTStr),DACStr,0,-1);
 CopyString(DUTStr,StringLength(DUTStr),";",0,-1);
 WriteCOMPort(DUTCOM,DUTStr);
 SetCtrlVal(syncPNL, SYNC_PNL_COMMTXT,DUTStr);
 Delay(1.0);

}


void StoreDAC(int BB, int DACType){
 if (DACType == SYNC_DAC){
  if ((BB % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HD$';",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'JD$';",0,-1);
 }  
 
 if (DACType == OFFSET_DAC){
  if ((BB % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HC$';",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'JC$';",0,-1);
 }  

 WriteCOMPort(DUTCOM,DUTStr);
 SetCtrlVal(syncPNL, SYNC_PNL_COMMTXT,DUTStr);
 Delay(2.2);

}


int ReadDAC(int BB, int DACType, unsigned int *dacdataread) {
char *strread;

 if (DACType == SYNC_DAC){
  if ((BB % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HD?';",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'JD?';",0,-1);
 }  
 if (DACType == OFFSET_DAC){
  if ((BB % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HC?';",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'JC?';",0,-1);
 }  
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,DUTStr);
 strread = ReadlnCOMPort(DUTCOM,1.0);
 Scan(strread,"%s>%i",dacdataread);

return StringLength(strread);
}










int MakeSyncOffsetCal(int BBNo, int CalType)

{
const double MinmVStepSync = 0.1;   	// mV	   (typisk = 0.25 mV pr step)
const double MaxmVStepSync = 0.5;
const double MinmVStepDCOffset = 0.8;   // mV	   (typisk = 1.14 mV pr step)
const double MaxmVStepDCOffset = 1.5;

double MinmVStep;   
double MaxmVStep;

int NoOfOkMeas;
int StartKal;
int LevelInsideTol,
	LevelOk;
double	mVprStep,
		NomLevel,
		LevelTol;
double meas,m1;
char TitleStr[30];
char MsgStr[100];
char LevelStr[20];
char ErrStr[200];
char *dataread, *strread;
unsigned int DACdataRead;

int handle_returned, controlID_returned;

char V24Addr[11];


 // set password = off & check RS-232 forbindelse
 if (CalType == SYNC_CAL)
  if (WriteProtect(OFF,FALSE) == FALSE)
   return FALSE;


/*
 // read 12nc
 if (BBNo > 2){
  FlushInQ(DUTCOM);
  switch (BBNo){
   case 3: 
   case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',1;"); break;
   case 5: 
   case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',1;"); break;
   case 7: 
   case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',1;"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strread[8] != '3'){ 			  // BB3-8:  12nc = "109 82930"
   DCOffsetOk = FALSE;
   SyncLevelOk = FALSE;
   Fmt(MsgStr,"%s< BB #%i ikke monteret i dette apparat !\n Test afbrydes",BBNo);
   MessagePopup(" BB Kalibrering",MsgStr);
   return FALSE;
  }
 } //if BBNo
*/ 

 
 if (CalType == SYNC_CAL)
  if (InitBB(BBNo) == FALSE)
   return FALSE;


// init panel
 Fmt(TitleStr,"%s< Black Burst #%i ",BBNo);

 if (CalType == SYNC_CAL){
  SetVLM(50,200,20,30);			// measurements in sync bottom
  VLMdata(0,4);					// AC coupled meter
  MinmVStep = MinmVStepSync;   
  MaxmVStep = MaxmVStepSync;
  NomLevel = NomSyncLevel;
  LevelTol = SyncLevelTol;
  SetCtrlAttribute (syncPNL, SYNC_PNL_LEVELBOX, ATTR_LABEL_TEXT,"Sync Level:");
 }
 
 if (CalType == OFFSET_CAL){
  SetVLM(50,200,400,500);		// measurements in active video line
  VLMdata(1,4);					// DC coupled meter
  MinmVStep = MinmVStepDCOffset;   
  MaxmVStep = MaxmVStepDCOffset;
  NomLevel = NomDCOffset;
  LevelTol = DCOffsetTol;
  SetCtrlAttribute (syncPNL, SYNC_PNL_LEVELBOX, ATTR_LABEL_TEXT,"DC Offset:");
}
 

 Fmt(LevelStr,"%s<%f[p0w5] ± %f[p0] mV",NomLevel,LevelTol);
 InsertTextBoxLine(syncPNL, SYNC_PNL_LEVELBOX,0,LevelStr);
 
 SetPanelAttribute (syncPNL, ATTR_TITLE, TitleStr);

 DisplayPanel (syncPNL);

 SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_GREEN);
 
 ErrIEEE = hp34401a_conf (hp34401, 1, VI_ON, 5, 1);

 if (CheckFrontRearTerminal(REARTERM34401) == FALSE) {
  HidePanel(syncPNL);
  return FALSE;
 }
 
 
 StartKal = YES;
 if (CalType == SYNC_CAL)
  StartKal = ConfirmPopup (""," Start kalibrering?");
 
 if (StartKal == NO) {
  HidePanel (syncPNL);
  return FALSE;
 }

 if (CalType == SYNC_CAL)
  switch (BBNo) {
   case 1 : WriteCOMPort(DUTCOM,":OUTP:BB1:SYST PAL;"); break;
   case 2 : WriteCOMPort(DUTCOM,":OUTP:BB2:SYST PAL;"); break;
   case 3 : WriteCOMPort(DUTCOM,":OUTP:BB3:SYST PAL;"); break;
   case 4 : WriteCOMPort(DUTCOM,":OUTP:BB4:SYST PAL;"); break;
   case 5 : WriteCOMPort(DUTCOM,":OUTP:BB5:SYST PAL;"); break;
   case 6 : WriteCOMPort(DUTCOM,":OUTP:BB6:SYST PAL;"); break;
   case 7 : WriteCOMPort(DUTCOM,":OUTP:BB7:SYST PAL;"); break;
   case 8 : WriteCOMPort(DUTCOM,":OUTP:BB8:SYST PAL;"); break;
  }


 switch (BBNo) {
  case 1: CopyString(V24Addr,0," 0;",0,-1); break;	   // BB# 1,2
  case 2: CopyString(V24Addr,0," 0;",0,-1); break;
  case 3: CopyString(V24Addr,0," 17;",0,-1); break;	   // BB# 3,4
  case 4: CopyString(V24Addr,0," 17;",0,-1); break;
  case 5: CopyString(V24Addr,0," 19;",0,-1); break;	   // BB# 5,6
  case 6: CopyString(V24Addr,0," 19;",0,-1); break;
  case 7: CopyString(V24Addr,0," 18;",0,-1); break;	   // BB# 7,8
  case 8: CopyString(V24Addr,0," 18;",0,-1); break;
 }
 

 done = FALSE;
 LevelInsideTol = FALSE;
 DACValue = 130;
 NoOfOkMeas = 0; 


 
 // Send addr for BB#
 CopyString(DUTStr,0,":FACT:V24C:ADDR",0,-1);
 CopyString(DUTStr,StringLength(DUTStr),V24Addr,0,-1);
 WriteCOMPort(DUTCOM, DUTStr);

 // Reset SyncDAC, OffsetDAC, Phase og Sch (både PAL og NTSC)
 if (CalType == SYNC_CAL){
  if ((BBNo % 2) == 1)
    WriteCOMPort(DUTCOM,"COMM 'HZ';");   // 1. BB
   else
    WriteCOMPort(DUTCOM,"COMM 'JZ';");   // 2. BB
 }


 // 1. måling
 SetDAC(DACValue,BBNo,CalType);
 DisableBreakOnLibraryErrors();
 ErrIEEE = hp34401a_singleMeas (hp34401, &m1);
 EnableBreakOnLibraryErrors();
 m1 = m1 * 1000 / VLMGain;
 SetCtrlVal (syncPNL, SYNC_PNL_VIDEOMETER, m1);
 
 
 // 2. måling
 DACValue = DACValue - 30;
 SetDAC(DACValue,BBNo,CalType);
 DisableBreakOnLibraryErrors();
 ErrIEEE = hp34401a_singleMeas (hp34401, &meas);
 EnableBreakOnLibraryErrors();
 meas = meas * 1000 / VLMGain;
 SetCtrlVal (syncPNL, SYNC_PNL_VIDEOMETER, meas);
 
 // beregn DAC værdi 
 mVprStep = (meas-m1) / 30;
 DACValue = DACValue - ((NomLevel - meas) / mVprStep);

 
 // check mV pr step
 if ( (fabs(mVprStep) < MinmVStep) | (fabs(mVprStep) > MaxmVStep)){
  SyncLevelOk = FALSE;
  Fmt(ErrStr,"%s< mV pr DAC-Sync step udenfor tol (%f[p2w5] - %f[p2w5]):  %f[p2w5]",fabs(MinmVStep),fabs(MaxmVStep),fabs(mVprStep));

  if (CalType == SYNC_CAL)
    Fmt(ErrStr,"%s[a]<\n Typisk værdi: 0.25mV/step\n Check modstande og lodninger");
   else
    Fmt(ErrStr,"%s[a]<\n Typisk værdi: 1.25mV/step\n Check modstande og lodninger");
  

  MessagePopup (" BB Kalibreringsfejl ",ErrStr);
  HidePanel (syncPNL);
  return FALSE;
 }

 
	do {

		 if (CalType == SYNC_CAL)
  		   SetDAC(DACValue,BBNo,SYNC_DAC);
  		  else
   		   SetDAC(DACValue,BBNo,OFFSET_DAC);
		 
 	     GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);

		 DisableBreakOnLibraryErrors();
		 ErrIEEE = hp34401a_singleMeas (hp34401, &meas);
		 EnableBreakOnLibraryErrors();

		 meas = meas * 1000 / VLMGain;
		 SetCtrlVal (syncPNL, SYNC_PNL_VIDEOMETER, meas);
		 
		 if (meas > NomLevel)
  		  DACValue--;
		  else
		 if (meas < NomLevel)
		  DACValue++;
		 
		 
		LevelInsideTol = ((meas > (NomLevel - LevelTol)) & (meas < (NomLevel + LevelTol)));
			
		if (LevelInsideTol){
		 NoOfOkMeas++;
		 SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_GREEN);
		 }
		else {
		 NoOfOkMeas = 0;
		 SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_RED);
		 }

		
		 if ((NoOfOkMeas > 3) & (done == FALSE))	  // mindst 3 OK-målinger i træk før godkendelse
		  LevelOk = TRUE;
		 else 
		  LevelOk = FALSE;
		
	} while ((LevelOk == FALSE) & (done == FALSE));


 if (CalType == SYNC_CAL)
   SyncLevelOk = LevelOk;
  else
   DCOffsetOk = LevelOk;


 if ((SyncLevelOk == YES)  & (CalType == SYNC_CAL))
  StoreDAC(BBNo,SYNC_DAC);

 if ((DCOffsetOk == YES)   & (CalType == OFFSET_CAL))
  StoreDAC(BBNo,OFFSET_DAC);
	
 if ((SyncLevelOk == NO) & (CalType == SYNC_CAL)){
  SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_RED);
  MessagePopup ("Sync Level Kalibrering", "Kalibrering   AFVIST ");
 }
	 
 if ((DCOffsetOk == NO)  & (CalType == OFFSET_CAL)){
  SetCtrlAttribute (syncPNL, SYNC_PNL_STATUSLED, ATTR_ON_COLOR,VAL_RED);
  MessagePopup ("DC Offset Kalibrering", "Kalibrering   AFVIST ");
 }
		 

 
 if( ((SyncLevelOk == YES) & (CalType == SYNC_CAL)) | ((DCOffsetOk == YES)  & (CalType == OFFSET_CAL)) ) {
  if (CalType == SYNC_CAL) 
    if (ReadDAC(BBNo, SYNC_DAC, &DACdataRead) < 3){
     MessagePopup ("Sync Level Kalibrering", " Intet svar ved læsning af kalibreringsdata i EEPROM ");
     SyncLevelOk = FALSE;
    } 

  if (CalType == OFFSET_CAL) 
    if (ReadDAC(BBNo, OFFSET_DAC, &DACdataRead) < 3){
     MessagePopup ("DC Offset Kalibrering", " Intet svar ved læsning af kalibreringsdata i EEPROM ");
     DCOffsetOk = FALSE;
    }
 
 
  if ((DACValue > DACdataRead + 1) | (DACValue < DACdataRead - 1)){
   if (CalType == SYNC_CAL){
    MessagePopup ("Sync Level Kalibrering", " Kalibreringsdata ikke gemt korrekt i EEPROM ");
    SyncLevelOk = FALSE;
   }  
    else {
     MessagePopup ("DC Offset Kalibrering", " Kalibreringsdata ikke gemt korrekt i EEPROM ");
     DCOffsetOk = FALSE;
    } 
  }
 }

HidePanel (syncPNL);

 if (CalType == SYNC_CAL)
   return SyncLevelOk;
  else	
   return DCOffsetOk;
}




//-------------------------------------------------------------------------------
char *ReadBBKUNumber (int BB) {
  // read BB KU nummer  (user text 2)
char *strread;
  FlushInQ(DUTCOM);
  switch (BB){
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HR?',2;"); break;
  case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HR?',2;"); break;
  case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HR?',2;"); break;
  case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HR?',2;"); break;
  case 5: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HR?',2;"); break;
  case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HR?',2;"); break;
  case 7: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HR?',2;"); break;
  case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HR?',2;"); break;
  }
  Delay(0.1);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,3,6);	 // "KUxxxxxx"   return xxxxxx

 return strread;
}						  



char *ReadBBProdDate (int BB) {
  // read BB produktions dato  (user text 3)
char *strread;
  FlushInQ(DUTCOM);
  switch (BB){
   case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HR?',3;"); break;
   case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HR?',3;"); break;
   case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HR?',3;"); break;
   case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HR?',3;"); break;
   case 5: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HR?',3;"); break;
   case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HR?',3;"); break;
   case 7: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HR?',3;"); break;
   case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HR?',3;"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}


char *ReadBBCalDate (int BB) {
  // read BB kalibrerings dato  (user text 4)
char *strread;
  FlushInQ(DUTCOM);
  switch (BB){
   case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HR?',4;"); break;
   case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HR?',5;"); break;
   case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HR?',4;"); break;
   case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HR?',5;"); break;
   case 5: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HR?',4;"); break;
   case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HR?',5;"); break;
   case 7: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HR?',4;"); break;
   case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HR?',5;"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}







//-------------------------------------------------------------------------------
int ReadCalData(void){
char *strread;
char *usertxt;
char DataStr[100];
int SDI,BB;
unsigned int pp[2];

 SaveToFile = FALSE;
 DisplayPanel(cfgPNL);
 SetCtrlAttribute (cfgPNL, CFG_PNL_OKBTN, ATTR_CTRL_TAB_POSITION, 0);
 SetCtrlAttribute(cfgPNL,CFG_PNL_OKBTN,ATTR_DIMMED,TRUE);
 SetCtrlAttribute(cfgPNL,CFG_PNL_SAVETOFILEBTN,ATTR_DIMMED,TRUE);

 ClearListCtrl(cfgPNL,CFG_PNL_BB);
 ClearListCtrl(cfgPNL,CFG_PNL_SPG);
 ClearListCtrl(cfgPNL,CFG_PNL_AUDIO);
 ClearListCtrl(cfgPNL,CFG_PNL_SDIBLACK);
 ClearListCtrl(cfgPNL,CFG_PNL_SDITSG);
 ClearListCtrl(cfgPNL,CFG_PNL_SDIGENL);
 ClearListCtrl(cfgPNL,CFG_PNL_ANALOGTSG);
 ClearListCtrl(cfgPNL,CFG_PNL_MULTIBB);
 ClearListCtrl(cfgPNL,CFG_PNL_LTC);

// tekstfarve = rød
 SetCtrlAttribute (cfgPNL, CFG_PNL_BB, ATTR_TEXT_COLOR, VAL_RED);
 SetCtrlAttribute (cfgPNL, CFG_PNL_SPG, ATTR_TEXT_COLOR, VAL_RED);
 SetCtrlAttribute (cfgPNL, CFG_PNL_AUDIO, ATTR_TEXT_COLOR, VAL_RED);
 SetCtrlAttribute (cfgPNL, CFG_PNL_SDIBLACK, ATTR_TEXT_COLOR, VAL_RED);
 SetCtrlAttribute (cfgPNL, CFG_PNL_SDITSG, ATTR_TEXT_COLOR, VAL_RED);
 SetCtrlAttribute (cfgPNL, CFG_PNL_SDIGENL, ATTR_TEXT_COLOR, VAL_RED);
 SetCtrlAttribute (cfgPNL, CFG_PNL_ANALOGTSG, ATTR_TEXT_COLOR, VAL_RED);
 SetCtrlAttribute (cfgPNL, CFG_PNL_MULTIBB, ATTR_TEXT_COLOR, VAL_RED);
 SetCtrlAttribute (cfgPNL, CFG_PNL_LTC, ATTR_TEXT_COLOR, VAL_RED);

//overskrifter
 InsertListItem(cfgPNL,CFG_PNL_BB,0,       "BB#  Sync  Offset  GFase      GScH   MFase      MScH   KU      PRODDATO  KALDATO",0);
 InsertListItem(cfgPNL,CFG_PNL_SPG,0,      " PAL HPH         ScH     NTSC HPH         ScH          KU      PRODDATO  KALDATO",0);
 InsertListItem(cfgPNL,CFG_PNL_AUDIO,0,    "                                                       KU      PRODDATO  KALDATO",0);
 InsertListItem(cfgPNL,CFG_PNL_SDIBLACK,0, "SDI#  GFase       MFase    RLoss  Jitter               KU      PRODDATO  KALDATO",0);
 InsertListItem(cfgPNL,CFG_PNL_SDITSG,0  , "      GFase       MFase    RLoss  Jitter               KU      PRODDATO  KALDATO",0);
 InsertListItem(cfgPNL,CFG_PNL_SDIGENL,0,  "                                                       KU      PRODDATO  KALDATO",0);
 InsertListItem(cfgPNL,CFG_PNL_ANALOGTSG,0,"                                                       KU      PRODDATO  KALDATO",0);
 InsertListItem(cfgPNL,CFG_PNL_MULTIBB,0,  "                                                       KU      PRODDATO  KALDATO",0);
 InsertListItem(cfgPNL,CFG_PNL_LTC,0,      "                                                       KU      PRODDATO  KALDATO",0);
 ProcessDrawEvents();
 
 
 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (cfgPNL);
  return FALSE;
  }
 
 
 // ---------  BB 1,2,3,4,5,6,7,8 ----------------------------------------
 for (BB = 1; BB < 9; BB++){

  Fmt(DataStr,"%s< %i   ",BB);
  
  // read 12nc
  FlushInQ(DUTCOM);
  switch (BB){
   case 3:
   case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',1;"); break;
   case 5:
   case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',1;"); break;
   case 7:
   case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',1;"); break;
  }
  if (BB > 2){
   strread = ReadlnCOMPort(DUTCOM,0.5);
   if (strread[8] != '3'){  // 12nc = "109 82930"
    InsertListItem(cfgPNL,CFG_PNL_BB,BB,DataStr,0);
    ProcessDrawEvents();
    continue;
   } 
  }
 
  

  BBKU[BB-1] = 0;
  
  // read sync level DAC
  FlushInQ(DUTCOM);
  switch (BB){
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HD?';"); break;
  case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'JD?';"); break;
  case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HD?';"); break;
  case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'JD?';"); break;
  case 5: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HD?';"); break;
  case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'JD?';"); break;
  case 7: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HD?';"); break;
  case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'JD?';"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[w3x]",&DACData[BB-1][0]);
  if (StringLength(strread) > 0)
    CopyString(DataStr,StringLength(DataStr),strread,0,-1);

  CopyString(DataStr,StringLength(DataStr),"   ",0,-1);


  // read offset DAC
  FlushInQ(DUTCOM);
  switch (BB){
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HC?';"); break;
  case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'JC?';"); break;
  case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HC?';"); break;
  case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'JC?';"); break;
  case 5: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HC?';"); break;
  case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'JC?';"); break;
  case 7: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HC?';"); break;
  case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'JC?';"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[w3x]",&DACData[BB-1][1]);
  if (StringLength(strread) > 0)
    CopyString(DataStr,StringLength(DataStr),strread,0,-1);
   else
    CopyString(DataStr,StringLength(DataStr),"---",0,-1);
  CopyString(DataStr,StringLength(DataStr),"     ",0,-1);


  // read PAL phase and ScH
  FlushInQ(DUTCOM);
  switch (BB){
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HX?';"); break;
  case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'JX?';"); break;
  case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HX?';"); break;
  case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'JX?';"); break;
  case 5: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HX?';"); break;
  case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'JX?';"); break;
  case 7: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HX?';"); break;
  case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'JX?';"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%2i[x]",pp);
  PhaseData[BB-1][0] = pp[0];
  PhaseData[BB-1][1] = pp[1];

  if (StringLength(strread) > 0)
    CopyString(DataStr,StringLength(DataStr),strread,0,-1);
   else
    CopyString(DataStr,StringLength(DataStr),"---",0,-1);

  CopyString(DataStr,StringLength(DataStr),"  ",0,-1);

  // read NTSC phase and ScH
  FlushInQ(DUTCOM);
  switch (BB){
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'HY?';"); break;
  case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 0;COMM 'JY?';"); break;
  case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'HY?';"); break;
  case 4: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 17;COMM 'JY?';"); break;
  case 5: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'HY?';"); break;
  case 6: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 19;COMM 'JY?';"); break;
  case 7: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'HY?';"); break;
  case 8: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 18;COMM 'JY?';"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%2i[x]",pp);
  PhaseData[BB-1][2] = pp[0];
  PhaseData[BB-1][3] = pp[1];

  if (StringLength(strread) > 0)
    CopyString(DataStr,StringLength(DataStr),strread,0,-1);
   else
    CopyString(DataStr,StringLength(DataStr),"---",0,-1);
  
  CopyString(DataStr,StringLength(DataStr),"  ",0,-1);

  strread = ReadBBKUNumber(BB);
  if (StringLength(strread) == 6){
    CopyString(DataStr,StringLength(DataStr),strread,0,6);
    Scan(strread,"%s>%i",&BBKU[BB-1]);
    }
   else 
    CopyString(DataStr,StringLength(DataStr),"??????",0,-1);	
  CopyString(DataStr,StringLength(DataStr),"  ",0,-1);

  
  strread = ReadBBProdDate(BB);
  if (StringLength(strread) == 6)
    CopyString(DataStr,StringLength(DataStr),strread,0,6);
   else 
    CopyString(DataStr,StringLength(DataStr),"??????",0,-1);	
  CopyString(DataStr,StringLength(DataStr),"    ",0,-1);
 
  
  strread = ReadBBCalDate(BB);
  if (StringLength(strread) == 6)
    CopyString(DataStr,StringLength(DataStr),strread,0,6);
   else 
    CopyString(DataStr,StringLength(DataStr),"??????",0,-1);	
  CopyString(DataStr,StringLength(DataStr)," ",0,-1);
  
  InsertListItem(cfgPNL,CFG_PNL_BB,BB,DataStr,0);
  CopyString(BBCalStr[BB-1],0,DataStr,0,-1);
  ProcessDrawEvents();
 } // for 


 
//---------------------- SPG Data ---------------------------------------------------
 SPGKU = 0;
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:SPGG:READ?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (StringLength(strread) > 0)
   Scan(strread,"%s>%4i[x]",SPGCalData);
  else {
   SPGCalData[0] = 0; SPGCalData[1] = 0; SPGCalData[2] = 0; SPGCalData[3] = 0;
 }
 
 Fmt(DataStr,"%s<     ");
 Fmt(DataStr,"%s[w11a]<%i[u] ",SPGCalData[0]);
 Fmt(DataStr,"%s[w9a]<%i[u]",SPGCalData[1]);
 Fmt(DataStr,"%s[w11a]<%i[u] ",SPGCalData[2]);
 Fmt(DataStr,"%s[w9a]<%i[u]",SPGCalData[3]);
 Fmt(DataStr,"%s[w10a]< ");
 
 strread = ReadSPGKUNumber();
 if ((StringLength(strread) > 5) && (isdigit(strread[0]))){
   Fmt(DataStr,"%s[w6a]<%s",strread);
   Scan(strread,"%s>%i",&SPGKU);
   }
  else
   Fmt(DataStr,"%s[a]<      ");

 Fmt(DataStr,"%s[a]<  ");
 strread = ReadSPGProdDate();
 if ((StringLength(strread) > 5) && (isdigit(strread[0])))
   Fmt(DataStr,"%s[w6a]<%s",strread);
  else
   Fmt(DataStr,"%s[a]<      ");

 Fmt(DataStr,"%s[a]<    ");
 strread = ReadSPGCalDate();
 if ((StringLength(strread) > 5) && (isdigit(strread[0])))
   Fmt(DataStr,"%s[w6a]<%s",strread);
  else
   Fmt(DataStr,"%s[a]<      ");
 
 
 if (StringLength(strread) == 0)
   InsertListItem(cfgPNL,CFG_PNL_SPG,1," ---",0);
  else {
   InsertListItem(cfgPNL,CFG_PNL_SPG,1,DataStr,0);
   CopyString(SPGCalStr,0,DataStr,0,-1);
  }
 
//---------------------- AES Audio Generator Data ---------------------------------------------------
 AudioKU = 0;
 Fmt(DataStr,"%s[w55]< ");
 strread = ReadAudioKUNumber();
 if ((StringLength(strread) > 5) && (isdigit(strread[0]))){
   Fmt(DataStr,"%s[w6a]<%s",strread);
   Scan(strread,"%s>%i",&AudioKU);
   }
  else
   Fmt(DataStr,"%s[a]<      ");

 Fmt(DataStr,"%s[a]<  ");
 strread = ReadAudioProdDate();
 if ((StringLength(strread) > 5) && (isdigit(strread[0])))
   Fmt(DataStr,"%s[w6a]<%s",strread);
  else
   Fmt(DataStr,"%s[a]<      ");

 Fmt(DataStr,"%s[a]<    ");
 strread = ReadAudioCalDate();
 if ((StringLength(strread) > 5) && (isdigit(strread[0])))
   Fmt(DataStr,"%s[w6a]<%s",strread);
  else
   Fmt(DataStr,"%s[a]<      ");
 
 
 if (StringLength(strread) == 0)
   InsertListItem(cfgPNL,CFG_PNL_AUDIO,1," ---",0);
  else {
   InsertListItem(cfgPNL,CFG_PNL_AUDIO,1,DataStr,0);
   CopyString(AudioCalStr,0,DataStr,0,-1);
  }
 
 
//---------------- SDI BLACK ---------------------------------------- 
 for (SDI = 1; SDI < 4; SDI++){
 
  switch (SDI){
   case 1: Fmt(DataStr,"%s< 34"); break;
   case 2: Fmt(DataStr,"%s< 56"); break;
   case 3: Fmt(DataStr,"%s< 78"); break;
  }

  // read 12nc
  FlushInQ(DUTCOM);
  switch (SDI){
   case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',1;"); break;
   case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',1;"); break;
   case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',1;"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strread[8] != '4'){ 			  // 12nc = "109 82940"
    InsertListItem(cfgPNL,CFG_PNL_SDIBLACK,SDI,DataStr,0);
    ProcessDrawEvents();
   continue;
  }

 
  
  SDIBLACKKU[SDI-1] = 0;
  
  // read SDI GPhase from EEPROM
  FlushInQ(DUTCOM);
  switch (SDI){
   case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HX?';"); break;
   case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HX?';"); break;
   case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HX?';"); break;
  }

  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&SDIData[SDI-1][0]);
  if (StringLength(strread) > 0)
    Fmt(DataStr,"%s[a]<%i[w12u]",SDIData[SDI-1][0]);
   else{
	InsertListItem (cfgPNL, CFG_PNL_SDIBLACK, SDI, DataStr, 0);
    ProcessDrawEvents();
    continue;
    }

  //Fmt(DataStr,"%s[w3a]< ");

 
 
  // read SDI MPhase from EEPROM
  FlushInQ(DUTCOM);
  switch (SDI){
   case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HY?';"); break;
   case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HY?';"); break;
   case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HY?';"); break;
  }

  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&SDIData[SDI-1][1]);
  if (StringLength(strread) > 0)
    Fmt(DataStr,"%s[a]<%i[w12u]",SDIData[SDI-1][1]);
   else
 	InsertListItem(cfgPNL,CFG_PNL_SDIBLACK,SDI," --------- ",0);
  
  Fmt(DataStr,"%s[w16a]< ");

  switch (SDI) {
   case 1: strread = ReadRLossInEEPROM(34); break;
   case 2: strread = ReadRLossInEEPROM(56); break;
   case 3: strread = ReadRLossInEEPROM(78); break;
  }
  
  if ((StringLength(strread) > 2) && (isdigit(strread[1])))
    Fmt(DataStr,"%s[w6a]<%s",strread);
   else
    Fmt(DataStr,"%s[a]<      ");

  
  switch (SDI) {
   case 1: strread = ReadJitterInEEPROM(34); break;
   case 2: strread = ReadJitterInEEPROM(56); break;
   case 3: strread = ReadJitterInEEPROM(78); break;
  }
  
  if ((StringLength(strread) > 2) && (isdigit(strread[1])))
    Fmt(DataStr,"%s[w6a]<%s",strread);
   else
    Fmt(DataStr,"%s[a]<      ");
  

  
  
  
  
  Fmt(DataStr,"%s[a]<  ");
  

  switch (SDI) {
   case 1: strread = ReadSDIKUNumber(34); break;
   case 2: strread = ReadSDIKUNumber(56); break;
   case 3: strread = ReadSDIKUNumber(78); break;
  }
  
  if ((StringLength(strread) > 5) && (isdigit(strread[0]))){
    Fmt(DataStr,"%s[w6a]<%s",strread);
    Scan(strread,"%s>%i",&SDIBLACKKU[SDI-1]);
    }
   else
    Fmt(DataStr,"%s[a]<      ");

  Fmt(DataStr,"%s[a]<  ");


  switch (SDI) {
   case 1 :strread = ReadSDIProdDate(34); break;
   case 2 :strread = ReadSDIProdDate(56); break;
   case 3 :strread = ReadSDIProdDate(78); break;
  } 
  if ((StringLength(strread) > 5) && (isdigit(strread[0])))
    Fmt(DataStr,"%s[w6a]<%s",strread);
   else
    Fmt(DataStr,"%s[a]<      ");

  Fmt(DataStr,"%s[a]<    ");

  switch (SDI) {
   case 1: strread = ReadSDICalDate(34); break;
   case 2: strread = ReadSDICalDate(56); break;
   case 3: strread = ReadSDICalDate(78); break;
  } 
  if ((StringLength(strread) > 5) && (isdigit(strread[0])))
    Fmt(DataStr,"%s[w6a]<%s",strread);
   else
    Fmt(DataStr,"%s[a]<      ");


 
  InsertListItem(cfgPNL,CFG_PNL_SDIBLACK,SDI,DataStr,0);
  CopyString(SDICalStr[SDI-1],0,DataStr,0,-1);
 
 
 
 } //for SDI = ...




 
//---------------------- SDI Genlock Data ---------------------------------------------------
 SDIGenlKU = 0;
 Fmt(DataStr,"%s[w55]< ");
 strread = ReadSDIGenlKUNumber();
 if ((StringLength(strread) > 5) && (isdigit(strread[0]))){
   Fmt(DataStr,"%s[w6a]<%s",strread);
   Scan(strread,"%s>%i",&SDIGenlKU);
   }
  else
   Fmt(DataStr,"%s[a]<      ");

 Fmt(DataStr,"%s[a]<  ");
 strread = ReadSDIGenlProdDate();
 if ((StringLength(strread) > 5) && (isdigit(strread[0])))
   Fmt(DataStr,"%s[w6a]<%s",strread);
  else
   Fmt(DataStr,"%s[a]<      ");

 Fmt(DataStr,"%s[a]<    ");
 strread = ReadSDIGenlCalDate();
 if ((StringLength(strread) > 5) && (isdigit(strread[0])))
   Fmt(DataStr,"%s[w6a]<%s",strread);
  else
   Fmt(DataStr,"%s[a]<      ");
 
 
 if (StringLength(strread) == 0)
   InsertListItem(cfgPNL,CFG_PNL_SDIGENL,1," ---",0);
  else {
   InsertListItem(cfgPNL,CFG_PNL_SDIGENL,1,DataStr,0);
   CopyString(SDIGenlCalStr,0,DataStr,0,-1);
  }
 
 

//---------------------- LTC Data ---------------------------------------------------
 LTCKU = 0;
 Fmt(DataStr,"%s[w55]< ");
 strread = ReadLTCKUNumber();
 if ((StringLength(strread) > 5) && (isdigit(strread[0]))){
   Fmt(DataStr,"%s[w6a]<%s",strread);
   Scan(strread,"%s>%i",&LTCKU);
   }
  else
   Fmt(DataStr,"%s[a]<      ");

 Fmt(DataStr,"%s[a]<  ");
 strread = ReadLTCProdDate();
 if ((StringLength(strread) > 5) && (isdigit(strread[0])))
   Fmt(DataStr,"%s[w6a]<%s",strread);
  else
   Fmt(DataStr,"%s[a]<      ");

 Fmt(DataStr,"%s[a]<    ");
 strread = ReadLTCCalDate();
 if ((StringLength(strread) > 5) && (isdigit(strread[0])))
   Fmt(DataStr,"%s[w6a]<%s",strread);
  else
   Fmt(DataStr,"%s[a]<      ");
 
 
 if (StringLength(strread) == 0)
   InsertListItem(cfgPNL,CFG_PNL_LTC,1," ---",0);
  else {
   InsertListItem(cfgPNL,CFG_PNL_LTC,1,DataStr,0);
   CopyString(LTCCalStr,0,DataStr,0,-1);
  }
 

 

//------------------------------------------------------------------------------- 
 SetCtrlAttribute(cfgPNL,CFG_PNL_OKBTN,ATTR_DIMMED,FALSE);
 SetCtrlAttribute(cfgPNL,CFG_PNL_SAVETOFILEBTN,ATTR_DIMMED,FALSE);
 WaitForContinue();

 if (SaveToFile == TRUE)
  SaveCalData();

 
 HidePanel(cfgPNL);

return 0;

} // ReadCalData











//--------------------------------------------------------------------
int  SaveCalData(void){
char filename[50], wrtbuf[100], MsgStr[50];
int bytes_written, fhandle, n, FilePresent, IOErr, fno, sl;
long fsize;	

// -------- BB1,  BB3,  BB5,  BB7    (incl SPG) -------------------------------------
 n = 0;
 while (n < 8){
  fno = 64;
  FilePresent = TRUE;
  while (FilePresent == TRUE) {
   fno++;							 // start with 'A'
   Fmt(filename,"%s<%s%i[w6p0]%c%s",CalDataFilePath,BBKU[n],(char)fno,CalDataFileExtention);
   FilePresent = GetFileInfo (filename, &fsize);
  } //while
 
  if (BBKU[n] != 0){
   fhandle = OpenFile (filename, VAL_READ_WRITE, VAL_TRUNCATE, VAL_ASCII);
   Fmt(wrtbuf,"%s<BB#  Sync  Offset  GFase      GScH   MFase      MScH   KU      PRODDATO  KALDATO\n");
   sl = StringLength(wrtbuf);
   bytes_written = WriteFile (fhandle, wrtbuf, sl);

   Fmt(BBCalStr[n],"%s[a]<\n");
   sl = StringLength(BBCalStr[n]);
   bytes_written = WriteFile (fhandle, BBCalStr[n], sl);   // BB1,3,5,7
   Fmt(BBCalStr[n+1],"%s[a]<\n");
   sl = StringLength(BBCalStr[n+1]);
   bytes_written = WriteFile (fhandle, BBCalStr[n+1], sl); // BB2,4,6,8
   
   if (n == 0) {
	Fmt(wrtbuf,"%s<\n PAL HPH         ScH     NTSC HPH         ScH          KU      PRODDATO  KALDATO\n");
    sl = StringLength(wrtbuf);
    bytes_written = WriteFile (fhandle, wrtbuf, sl);
	Fmt(SPGCalStr,"%s[a]<\n");
    sl = StringLength(SPGCalStr);
    bytes_written = WriteFile (fhandle, SPGCalStr, sl);
   }
   
   IOErr = GetFmtIOError ();
   Fmt(MsgStr,"%s<ERROR on saving cal-data: Error = %i",IOErr);
   if ((sl != bytes_written) || (IOErr > 0))
     MessagePopup(" Cal Data",MsgStr);
    else
    {
     Fmt(MsgStr,"%s< Gemt  i  fil: '%s'",filename);
     MessagePopup (" BB + SPG Kalibreringsdata",MsgStr);
    }
   
   CloseFile (fhandle);
   
  } //if
  
  n = n+2;
 } //while

// ----------------  Audio Generator -----------------------------
 if (AudioKU != 0){
  fno = 64;
  FilePresent = TRUE;
  while (FilePresent == TRUE) {
   fno++;							 // start with 'A'
   Fmt(filename,"%s<%s%i[w6p0]%c%s",CalDataFilePath,AudioKU,(char)fno,CalDataFileExtention);
   FilePresent = GetFileInfo (filename, &fsize);
  } //while

  fhandle = OpenFile (filename, VAL_READ_WRITE, VAL_TRUNCATE, VAL_ASCII);
  Fmt(wrtbuf,"%s<                                                       KU      PRODDATO  KALDATO\n");
  sl = StringLength(wrtbuf);
  bytes_written = WriteFile (fhandle, wrtbuf, sl);
  Fmt(AudioCalStr,"%s[a]<\n");
  sl = StringLength(AudioCalStr);
  bytes_written = WriteFile (fhandle, AudioCalStr, sl);
  IOErr = GetFmtIOError ();

  Fmt(MsgStr,"%s<ERROR on saving cal-data: Error = %i",IOErr);
  if ((sl != bytes_written) || (IOErr > 0))
    MessagePopup(" Cal Data",MsgStr);
   else
   {
    Fmt(MsgStr,"%s< Gemt  i  fil: '%s'",filename);
    MessagePopup (" Audio Generator Kalibreringsdata",MsgStr);
   }
  
  CloseFile (fhandle);
 } //if AudioKU..





// -------- SDI34,  SDI56,  SDI78,  -------------------------------------------------
 n = 0;
 while (n < 3){
  
  fno = 64;
  FilePresent = TRUE;
  while (FilePresent == TRUE) {
   fno++;							 // start with 'A'
   Fmt(filename,"%s<%s%i[w6p0]%c%s",CalDataFilePath,SDIBLACKKU[n],(char)fno,CalDataFileExtention);
   FilePresent = GetFileInfo (filename, &fsize);
  } //while

  if (SDIBLACKKU[n] != 0){
   fhandle = OpenFile (filename, VAL_READ_WRITE, VAL_TRUNCATE, VAL_ASCII);
   Fmt(wrtbuf,"%s<SDI#  GFase       MFase    RLoss  Jitter               KU      PRODDATO  KALDATO\n");
   sl = StringLength(wrtbuf);
   bytes_written = WriteFile (fhandle, wrtbuf, sl);

   Fmt(SDICalStr[n],"%s[a]<\n");
   sl = StringLength(SDICalStr[n]);
   bytes_written = WriteFile (fhandle, SDICalStr[n], sl);   // SDI 34,56,78
   
   IOErr = GetFmtIOError ();
   Fmt(MsgStr,"%s<ERROR on saving cal-data: Error = %i",IOErr);
   if ((sl != bytes_written) || (IOErr > 0))
     MessagePopup(" Cal Data",MsgStr);
    else
    {
     Fmt(MsgStr,"%s< Gemt  i  fil: '%s'",filename);
     MessagePopup (" SDI  Kalibreringsdata",MsgStr);
    }
   
   CloseFile (fhandle);
   
  } //if

 n++;
 
 } //while




// -------- SDI TSG  -------------------------------------------------------

//  mangler




// ----------------  SDI Genlock -------------------------------------------
 if (SDIGenlKU != 0){
  fno = 64;
  FilePresent = TRUE;
  while (FilePresent == TRUE) {
   fno++;							 // start with 'A'
   Fmt(filename,"%s<%s%i[w6p0]%c%s",CalDataFilePath,SDIGenlKU,(char)fno,CalDataFileExtention);
   FilePresent = GetFileInfo (filename, &fsize);
  } //while

  fhandle = OpenFile (filename, VAL_READ_WRITE, VAL_TRUNCATE, VAL_ASCII);
  Fmt(wrtbuf,"%s<                                                       KU      PRODDATO  KALDATO\n");
  sl = StringLength(wrtbuf);
  bytes_written = WriteFile (fhandle, wrtbuf, sl);
  Fmt(SDIGenlCalStr,"%s[a]<\n");
  sl = StringLength(SDIGenlCalStr);
  bytes_written = WriteFile (fhandle, SDIGenlCalStr, sl);
  IOErr = GetFmtIOError ();
 
  Fmt(MsgStr,"%s<ERROR on saving cal-data: Error = %i",IOErr);
  if ((sl != bytes_written) || (IOErr > 0))
    MessagePopup(" Cal Data",MsgStr);
   else
   {
    Fmt(MsgStr,"%s< Gemt  i  fil: '%s'",filename);
    MessagePopup (" SDI Genlock Kalibreringsdata",MsgStr);
   }
  
  CloseFile (fhandle);
 } // if SDIGenlKU...




// ----------------  LTC -------------------------------------------
 if (LTCKU != 0){
  fno = 64;
  FilePresent = TRUE;
  while (FilePresent == TRUE) {
   fno++;							 // start with 'A'
   Fmt(filename,"%s<%s%i[w6p0]%c%s",CalDataFilePath,LTCKU,(char)fno,CalDataFileExtention);
   FilePresent = GetFileInfo (filename, &fsize);
  } //while

  fhandle = OpenFile (filename, VAL_READ_WRITE, VAL_TRUNCATE, VAL_ASCII);
  Fmt(wrtbuf,"%s<                                                       KU      PRODDATO  KALDATO\n");
  sl = StringLength(wrtbuf);
  bytes_written = WriteFile (fhandle, wrtbuf, sl);
  Fmt(LTCCalStr,"%s[a]<\n");
  sl = StringLength(LTCCalStr);
  bytes_written = WriteFile (fhandle, LTCCalStr, sl);
  IOErr = GetFmtIOError ();
 
  Fmt(MsgStr,"%s<ERROR on saving cal-data: Error = %i",IOErr);
  if ((sl != bytes_written) || (IOErr > 0))
    MessagePopup(" Cal Data",MsgStr);
   else
   {
    Fmt(MsgStr,"%s< Gemt  i  fil: '%s'",filename);
    MessagePopup (" LTC Kalibreringsdata",MsgStr);
   }
  
  CloseFile (fhandle);
 } // if LTCKU...





return TRUE;
} //SaveCalData
 



//----------------------------------------------------------------------------------
int ShowCalFiles(void){
int select_status, dfile, bytes_read,IOErr,lineno;
char selected_path[MAX_PATHNAME_LEN], line[100];

 DisplayPanel(dataPNL);   //dataPNL findes i cfg.uir file
 SetPanelAttribute (dataPNL, ATTR_TITLE, "");

 select_status = VAL_EXISTING_FILE_SELECTED;
 
 while (select_status == VAL_EXISTING_FILE_SELECTED) {

  SetCtrlAttribute (dataPNL, DATA_PNL_OKBTN, ATTR_VISIBLE, FALSE);
 
  select_status = FileSelectPopup (CalDataFilePath, "*.dat",
								   "*.dat;*.*", " Kalibreringsfiler",
								   VAL_SELECT_BUTTON, 1, 0, 1, 0,
								   selected_path);

  if (select_status != VAL_EXISTING_FILE_SELECTED) {
   HidePanel(dataPNL);
   return TRUE;
  }


  SetPanelAttribute (dataPNL, ATTR_TITLE, selected_path);
  SetCtrlAttribute (dataPNL, DATA_PNL_OKBTN, ATTR_VISIBLE, TRUE);
  SetCtrlAttribute (dataPNL, DATA_PNL_TXTBOX, ATTR_TEXT_COLOR, VAL_RED);
  dfile = OpenFile (selected_path, VAL_READ_ONLY, VAL_OPEN_AS_IS,VAL_ASCII);

  ResetTextBox (dataPNL, DATA_PNL_TXTBOX, "");
  bytes_read = ReadLine (dfile, line, 80);
  IOErr = GetFmtIOError ();
  if ((IOErr == 0) && (bytes_read >= 0)) {
   Fmt(line,"%s[a]<\n");
   SetCtrlVal(dataPNL, DATA_PNL_TXTBOX,line);
  }
 
  while ((IOErr == 0) && (bytes_read >= 0)){
   bytes_read = ReadLine (dfile, line, 80);
   IOErr = GetFmtIOError ();
   Fmt(line,"%s[a]<\n");
   SetCtrlVal(dataPNL, DATA_PNL_TXTBOX,line);
  } 

  WaitForContinue();


 } //while
 
 HidePanel(dataPNL);


return TRUE;
}
 




//-------------------------------------------------------------------------------
int WriteBBCalDate(int BBNo){
char *dd;					// MM-DD-YYYY
char dato[10];  			// YYMMDD

 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // Send addr for BB#
 CopyString(DUTStr,0,":FACT:V24C:ADDR",0,-1);
 CopyString(DUTStr,StringLength(DUTStr),V24Addr[BBNo],0,-1);
 WriteCOMPort(DUTCOM, DUTStr);

 if ((BBNo % 2) == 1)
   CopyString(DUTStr,StringLength(DUTStr),"COMM 'HW',4,'",0,-1);
  else 
   CopyString(DUTStr,StringLength(DUTStr),"COMM 'HW',5,'",0,-1);
 CopyString(DUTStr,StringLength(DUTStr),dato,0,-1);
 CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
 WriteCOMPort(DUTCOM, DUTStr);
 
 Delay(0.3);

return 1;
}





//-------------------------------------------------------------------------------
int InitBB(int BBNo){

int n;
int ValidKU;
char *usertxt;
char *dd;					// MM-DD-YYYY
char dato[10];  			// YYMMDD
unsigned char response[10]; // svar fra PromptPopup
char KUStr[10];  			// KUxxxxxx
char *KUread;
char BBStr[10];  			// BB #
char PromptStr[50];
 
 // Send addr for BB#
 CopyString(DUTStr,0,":FACT:V24C:ADDR",0,-1);
 CopyString(DUTStr,StringLength(DUTStr),V24Addr[BBNo],0,-1);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.1);

 // 0  producent
 if ((BBNo % 2) == 1){		// kun i BB 1,3,5,7
  CopyString(DUTStr,0,"COMM 'HW',0,'PHILIPS';",0,-1);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.3);
 }
 
 // 1  12nc
 if ((BBNo % 2) == 1)		// kun i BB 1,3,5,7
  CopyString(DUTStr,0,"COMM 'HW',1,'",0,-1);
 
 if (BBNo == 1)
  CopyString(DUTStr,StringLength(DUTStr),NC12[MAIN],0,-1);
 if ((BBNo == 3) | (BBNo == 5) | (BBNo == 7))
  CopyString(DUTStr,StringLength(DUTStr),NC12[DUALBB],0,-1);
 CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.9);
 
 
 
 if ( ((BBNo % 2) == 1) || (BBNo == 4) ){		// kun i BB 1,3,4,5,7
  // 2  KU nummer
  KUread = ReadBBKUNumber(BBNo);
  if (isdigit(KUread[2]) == FALSE)	
    CopyString(KUread,0,"",0,-1);
    
  if (CompareStrings (KUread, 0, "999333", 0, 0) == 0) {
   MessagePopup(" BB kalibrering"," Denne BB er et referencemodul, og må IKKE kalibreres!!\n Kalibrering stoppet");
   return FALSE;
  }
  

  Fmt(BBStr,"%s<%i",BBNo);
  CopyString(PromptStr,0," BB # ",0,-1);
  CopyString(PromptStr,StringLength(PromptStr),BBStr,0,-1);
  CopyString(PromptStr,StringLength(PromptStr),":   Indtast KU nummer",0,-1);

  do {
   ValidKU = TRUE;
   PromptPopup (PromptStr, KUread, response, 6);
  
   if ((StringLength(response) == 0) & (StringLength(KUread) == 6))  // Only ENTER pressed
    CopyString(response,0,KUread,0,-1);

   for (n = 0; n < 6; n++){
    if (isdigit(response[n]) == FALSE)		  // digits only
     ValidKU = FALSE;
   }
  
   if (StringLength(response) != 6)			  // exactly 6 digits
    ValidKU = FALSE;
  } while (ValidKU == FALSE);

  CopyString(KUStr,0,"KU",0,-1);
  CopyString(KUStr,StringLength(KUStr),response,0,-1);
  if ((BBNo % 2) == 1)
   CopyString(DUTStr,0,"COMM 'HW',2,'",0,-1);
  CopyString(DUTStr,StringLength(DUTStr),KUStr,0,-1);
  CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.3);
 } // if
 
 // get system date 
 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // 3  produktionsdato
 usertxt = ReadBBProdDate(BBNo);
 if (isdigit(usertxt[0]) == FALSE){			  
   if ((BBNo % 2) == 1) {
    CopyString(DUTStr,0,"COMM 'HW',3,'",0,-1);
   CopyString(DUTStr,StringLength(DUTStr),dato,0,-1);
   CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
   WriteCOMPort(DUTCOM, DUTStr);
   Delay(0.3);
   }
 }

 // 4 & 5 kalibreringsdato 
 usertxt = ReadBBCalDate(BBNo);
 if (isdigit(usertxt[0]) == FALSE){			  
  if ((BBNo % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HW',4,'",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'HW',5,'",0,-1);
  CopyString(DUTStr,StringLength(DUTStr),dato,0,-1);
  CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 }


 // 6   PT-nummer 
 if (BBNo < 3)   
   WriteCOMPort(DUTCOM, "COMM 'HW',6,'PT5210';");
  else 
   WriteCOMPort(DUTCOM, "COMM 'HW',6,'PT8606';");
 Delay(0.2);

 // 7   reserveret - ikke defineret.

return TRUE;
} // InitBB




//-------------------------------------------------------------------------------
void SetPhase(int ph){
char PhaseStr[11];

 Fmt(PhaseStr,"%s<%i",ph);
 
 if (TV == PAL){
  if ((BBNumber % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HG',",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'JG',",0,-1);
 }  

 if (TV == NTSC){
  if ((BBNumber % 2) == 1)
    CopyString(DUTStr,0,"COMM 'HM',",0,-1);
   else
    CopyString(DUTStr,0,"COMM 'JM',",0,-1);
 }  

 CopyString(DUTStr,StringLength(DUTStr),PhaseStr,0,-1);
 CopyString(DUTStr,StringLength(DUTStr),";",0,-1);
 WriteCOMPort(DUTCOM,DUTStr);
 Delay(0.1);
}



//-------------------------------------------------------------------------------
void SetScH(int sch){
char ScHStr[11];

 Fmt(ScHStr,"%s<%i",sch);
 
 if ((BBNumber % 2) == 1)
   CopyString(DUTStr,0,"COMM 'HH',",0,-1);
  else
   CopyString(DUTStr,0,"COMM 'JH',",0,-1);

 CopyString(DUTStr,StringLength(DUTStr),ScHStr,0,-1);
 CopyString(DUTStr,StringLength(DUTStr),";",0,-1);
 WriteCOMPort(DUTCOM,DUTStr);
 Delay(0.1);

}





//-------------------------------------------------------------------------------
int MakePhaseCal(int BBNo, int TVSys) {
/* 
 Ved start af kalibrering, resettes registre i BB generator for både PAL og NTSC.
 Det betyder at både PAL og NTSC skal kalibreres; også selv om kun det ene af 
 systemerne skal kalibreres.
 Reset foregår ved start af sync level kalibrering. Se funktionen MakeSyncOffsetCal.
 */

char TitleStr[50];
int handle_returned, controlID_returned;



 Set5640Standard("G");
 Set5640Pattern("G", 290);
 Set5640Standard("M");
 Set5640Pattern("M", 290);
 
 TV = TVSys;
 BBNumber = BBNo;
 if (TVSys == PAL)
  Fmt(TitleStr,"%s< PAL Fase, ScH og burst amplitude for BB #%i ",BBNo);
   else
  Fmt(TitleStr,"%s< NTSC Fase, ScH og burst amplitude for BB #%i ",BBNo);

 done = FALSE;
 PhaseScHOk == FALSE;
 
 DisplayPanel (phasePNL);
 SetPanelAttribute (phasePNL, ATTR_TITLE, TitleStr);

 SetCtrlAttribute (phasePNL, SCH_PNL_PHASEPILOP_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, SCH_PNL_PHASEPILNED_BTN, ATTR_DIMMED,TRUE);
 SetCtrlAttribute (phasePNL, SCH_PNL_SCHPILOP_BTN, ATTR_DIMMED,FALSE);
 SetCtrlAttribute (phasePNL, SCH_PNL_SCHPILNED_BTN, ATTR_DIMMED,FALSE);
 SetCtrlAttribute (phasePNL, SCH_PNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
 SetCtrlAttribute (phasePNL, SCH_PNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);

 
 if (TVSys == PAL) {
  PhaseValue = 0;
  SetCtrlAttribute (phasePNL, SCH_PNL_PHASE_SLIDE, ATTR_MAX_VALUE, PhaseValue + 2000);
  SetCtrlAttribute (phasePNL, SCH_PNL_PHASE_SLIDE, ATTR_MIN_VALUE, PhaseValue - 2000);
  SetCtrlVal (phasePNL, SCH_PNL_PHASE_SLIDE, PhaseValue);
  ScHValue = 0;
  SetCtrlAttribute (phasePNL, SCH_PNL_SCH_SLIDE, ATTR_MAX_VALUE, ScHValue + 300);
  SetCtrlAttribute (phasePNL, SCH_PNL_SCH_SLIDE, ATTR_MIN_VALUE, ScHValue - 300);
  SetCtrlVal (phasePNL, SCH_PNL_SCH_SLIDE, ScHValue);
  ScHOffset = ScHOffsetPAL;
  PhaseOffset = PhaseOffsetPAL;
 } 
 else {
  PhaseValue = 0;
  SetCtrlAttribute (phasePNL, SCH_PNL_PHASE_SLIDE, ATTR_MAX_VALUE, PhaseValue + 400);
  SetCtrlAttribute (phasePNL, SCH_PNL_PHASE_SLIDE, ATTR_MIN_VALUE, PhaseValue - 400);
  SetCtrlVal (phasePNL, SCH_PNL_PHASE_SLIDE, PhaseValue);
  ScHValue = 0;
  SetCtrlAttribute (phasePNL, SCH_PNL_SCH_SLIDE, ATTR_MAX_VALUE, ScHValue + 300);
  SetCtrlAttribute (phasePNL, SCH_PNL_SCH_SLIDE, ATTR_MIN_VALUE, ScHValue - 300);
  SetCtrlVal (phasePNL, SCH_PNL_SCH_SLIDE, ScHValue);
  ScHOffset = ScHOffsetNTSC;
  PhaseOffset = PhaseOffsetNTSC;
 }
 
 

 ResetTextBox(phasePNL, SCH_PNL_PHASESCHTXTBOX,"");
 if (TV == PAL) {
  WriteCOMPort(PT5210COM,":OUTP:BB1:SYST PAL_ID;SCHP 0;");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Forbind DUT GENLOCK B   til 5662 CH-A\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Forbind DUT BB          til 5662 CH-B\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," NB: Kabler fra DUT til 5662 SKAL være lige lange!!\n\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Indstil 5662G: VECT B        INT REF \n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Indstil 5662M: WFM  A-B  2H  INT REF  X-MAG\n\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," ScH aflæses på 5662G      Fase aflæses på 5662M\n");
  if ((BBNo % 2) == 1)
   SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," HUSK: Burst amplitude justeres til samme amplitude som sync med C203\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX,"\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Ved fejl: Tryk F9           ");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Ved OK:   Tryk F4");
  SetPM5662(PAL);
 }

 if (TV == NTSC) {
  WriteCOMPort(PT5210COM,":OUTP:BB1:SYST NTSC;SCHP 0;");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Forbind DUT GENLOCK B   til 5662 CH-A\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Forbind DUT BB          til 5662 CH-B\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," NB: Kabler fra DUT til 5662 SKAL være lige lange!!\n\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Indstil 5662G: WFM  A-B  2H  INT REF  X-MAG\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Indstil 5662M: VECT B        INT REF \n\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," ScH aflæses på 5662M      Fase aflæses på 5662G\n");
  if ((BBNo % 2) == 0)
    SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," HUSK: Burst amplitude justeres til samme amplitude som sync med C303\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX,"\n");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Ved fejl: Tryk F9           ");
  SetCtrlVal(phasePNL, SCH_PNL_PHASESCHTXTBOX," Ved OK:   Tryk F4");
  SetPM5662(NTSC);
 }

 if (TV == PAL){
  if ((BBNo % 2) == 1)
    SetCtrlAttribute (phasePNL, SCH_PNL_BURST, ATTR_VISIBLE, TRUE);
   else
    SetCtrlAttribute (phasePNL, SCH_PNL_BURST, ATTR_VISIBLE, FALSE);
 }

 if (TV == NTSC){
  if ((BBNo % 2) == 0)
    SetCtrlAttribute (phasePNL, SCH_PNL_BURST, ATTR_VISIBLE, TRUE);
   else
    SetCtrlAttribute (phasePNL, SCH_PNL_BURST, ATTR_VISIBLE, FALSE);
 }
 
 
 
 
 
 if (TV == PAL){
   Delay(0.2);
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST PALB;");
   Delay(0.5);
    switch (BBNo) {
     case 1 : WriteCOMPort(DUTCOM,":OUTP:BB1:SYST PAL_ID;"); break;
     case 2 : WriteCOMPort(DUTCOM,":OUTP:BB2:SYST PAL_ID;"); break;
     case 3 : WriteCOMPort(DUTCOM,":OUTP:BB3:SYST PAL_ID;"); break;
     case 4 : WriteCOMPort(DUTCOM,":OUTP:BB4:SYST PAL_ID;"); break;
     case 5 : WriteCOMPort(DUTCOM,":OUTP:BB5:SYST PAL_ID;"); break;
     case 6 : WriteCOMPort(DUTCOM,":OUTP:BB6:SYST PAL_ID;"); break;
     case 7 : WriteCOMPort(DUTCOM,":OUTP:BB7:SYST PAL_ID;"); break;
     case 8 : WriteCOMPort(DUTCOM,":OUTP:BB8:SYST PAL_ID;"); break;
    }
  }  
  
  if (TV == NTSC){
   Delay(0.2);
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST NTSC;");
   Delay(0.5);
    switch (BBNo) {
     case 1 : WriteCOMPort(DUTCOM,":OUTP:BB1:SYST NTSC;"); break;
     case 2 : WriteCOMPort(DUTCOM,":OUTP:BB2:SYST NTSC;"); break;
     case 3 : WriteCOMPort(DUTCOM,":OUTP:BB3:SYST NTSC;"); break;
     case 4 : WriteCOMPort(DUTCOM,":OUTP:BB4:SYST NTSC;"); break;
     case 5 : WriteCOMPort(DUTCOM,":OUTP:BB5:SYST NTSC;"); break;
     case 6 : WriteCOMPort(DUTCOM,":OUTP:BB6:SYST NTSC;"); break;
     case 7 : WriteCOMPort(DUTCOM,":OUTP:BB7:SYST NTSC;"); break;
     case 8 : WriteCOMPort(DUTCOM,":OUTP:BB8:SYST NTSC;"); break;
    }
  }
   

 // Send addr for BB#
 CopyString(DUTStr,0,":FACT:V24C:ADDR",0,-1);
 CopyString(DUTStr,StringLength(DUTStr),V24Addr[BBNo],0,-1);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.1);

 
 // Reset af PAL & NTSC faser til default værdier gøres i MakeSyncOffsetCal 
   


 SetPhase(PhaseValue + PhaseOffset);
 SetScH(ScHValue + ScHOffset);
 

 // her ventes indtil brugeren har tastet OK=F4 eller FEJL=F9
 while (done == FALSE) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 }



 if (PhaseScHOk == FALSE)
   switch (TV){
    case PAL  :  MessagePopup (" Phase, ScH og burst amplitude kalibrering", "Kalibrering   FEJLET"); break;
    case NTSC : MessagePopup (" Phase & ScH kalibrering", "Kalibrering   FEJLET"); break;
   }
  
 
 
 
 if (PhaseScHOk == YES) {
  
  if (TV == PAL){
   if ((BBNo % 2) == 1)
     WriteCOMPort(DUTCOM,"COMM 'HX$';");		// store in EEPROM
    else
     WriteCOMPort(DUTCOM,"COMM 'JX$';");
  }


   if (TV == NTSC){
    if ((BBNo % 2) == 1)
      WriteCOMPort(DUTCOM,"COMM 'HY$';");		// store in EEPROM
     else
      WriteCOMPort(DUTCOM,"COMM 'JY$';");
   }
  
  
  WriteBBCalDate(BBNo);

  
 } //if

 if ((BBNo == 2) && (TV == PAL))
  if (PhaseScHOk == YES) {
   SetPM5662(PAL);
   MessagePopup (" Stik CX302", "Check BB signal på 5662G");
  } 


  HidePanel (phasePNL);
 

  if (PhaseScHOk == YES)
    return TRUE;
   else
    return FALSE;
} // MakePhaseCal






//----------------------------------------------------------------------------
int CheckPowerSupply(int NomVolt) {
double voltmeas, VMax,VMin;
int handle_returned, controlID_returned;
char MsgTxt[30], TitleStr[30];

 CheckingPS = TRUE;
 CheckingLTCVCO = FALSE;
 CheckingOven = FALSE;
 CheckingAudioVCO = FALSE;

 switch (NomVolt) {
  case 5:  Fmt(TitleStr,"%s< +5 VDC Justering "); break;
  case 12: Fmt(TitleStr,"%s< +12 VDC Kontrol "); break;
  case -5: Fmt(TitleStr,"%s< -5 VDC Kontrol "); break;
 } 

 DisplayPanel(vccPNL);
 SetPanelAttribute (vccPNL, ATTR_TITLE, TitleStr);

 ErrIEEE = hp34401a_conf (hp34401, 1, VI_ON, 5, 1);

 if (CheckFrontRearTerminal(FRONTTERM34401) == FALSE) {
  HidePanel(vccPNL);
  return FALSE;
 }
 
 ResetTextBox (vccPNL, VCC_PNL_TXTBOX, "");
 if (NomVolt == 5) {
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Juster 5 volt forsyning med potmeter på powersupply\n\n");
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med HP34401 i testpunkt +5V ved stikket XK1\n\n");
  VMax = Max5;
  VMin = Min5;
  SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MAX_VALUE, VMax + 0.3);
  SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MIN_VALUE, VMin - 0.3);
 }

 if (NomVolt == 12) {
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Kontroller 12 volt forsyning\n\n");
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med HP34401 i testpunkt +12V ved stikket XK1\n\n");
  VMax = Max12;
  VMin = Min12;
  SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MAX_VALUE, VMax + 1);
  SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MIN_VALUE, VMin - 1);
 }
 
 if (NomVolt == -5) {
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Kontroller -5 volt forsyning\n\n");
  SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med HP34401 i testpunkt -5V ved stikket XK1\n\n");
  VMax = MaxM5;
  VMin = MinM5;
  SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MAX_VALUE, VMax + 0.5);
  SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MIN_VALUE, VMin - 0.5);
 }

 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved OK:   Tryk F4\n");
 Fmt(MsgTxt,"%s<%f[p2] - %f[p2] VDC",VMin,VMax);
 
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_LABEL_TEXT,MsgTxt);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_TEXT_JUSTIFY,VAL_CENTER_JUSTIFIED);

 done = FALSE;

 while (done == FALSE) {
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  DisableBreakOnLibraryErrors();
  ErrIEEE = hp34401a_singleMeas (hp34401, &voltmeas);
  EnableBreakOnLibraryErrors();
  SetCtrlVal (vccPNL, VCC_PNL_VCCSLIDE, voltmeas);
  if ((voltmeas > VMax) || (voltmeas < VMin)) {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_RED);
	SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_RED);
    }
   else {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_GREEN);
	SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
   } 

 } // while

 if (NomVolt == -5) 
  if (VoltOk == TRUE)
    MessagePopup (" Power Supply"," OK");
   else 
    MessagePopup (" Power Supply"," FEJLET");
 
 CheckingPS = FALSE;
 HidePanel(vccPNL);
return VoltOk;
}



//-------------------------------------------------------------------------------
int ReadADC(int *Temp, int *Plus5, int *Minus5, int *Plus12){
char *strread;
int aa[4];

 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM, ":FACT:ADC:READ?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%4i[x]",aa);
 *Temp = aa[0];
 *Plus5 = aa[1];
 *Minus5 = aa[2];
 *Plus12 = aa[3];

return strlen(strread);
}








//-----------------------------------------------------------------------------------
int CheckFan(void) {
int handle_returned, controlID_returned;
int ADC1,ADC2,ADC3,ADC4,Temp;
char MsgTxt[80];

  
 DisplayPanel(fanPNL);
 SetPanelAttribute (fanPNL, ATTR_TITLE, " Blæser Kontrol ");
 ResetTextBox (fanPNL, FAN_PNL_TXTBOX, "");

 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (fanPNL);
  return FALSE;
  }

 SetCtrlVal(fanPNL,FAN_PNL_TXTBOX," Check at målt temperatur er korrekt ved stuetemperatur\n\n");
 SetCtrlVal(fanPNL,FAN_PNL_TXTBOX," NTC modstand varmes derefter op med varmeblæser\n\n");
 Fmt(MsgTxt,"%s< Kontroller at blæseren tænder, når temperaturen er ca %i°C\n\n",FanRunTemp);
 SetCtrlVal(fanPNL,FAN_PNL_TXTBOX, MsgTxt);
 SetCtrlVal(fanPNL,FAN_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(fanPNL,FAN_PNL_TXTBOX, " Ved OK:   Tryk F4\n");
 SetCtrlAttribute (fanPNL, FAN_PNL_ADCSTR, ATTR_TEXT_COLOR, VAL_BLUE);
 
 FanOk = TRUE; 
 done = FALSE;

 while (done == FALSE) {
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  // read ADC
  ReadADC(&ADC1,&ADC2,&ADC3,&ADC4);
  Delay(0.3);
  
  if (ADC1 > 104)
    Temp = (int)(215-ADC1)/2.467;
   else 
    Temp = (int)(192-ADC1)/1.95;
  
  Fmt(MsgTxt,"%s< %i°C",Temp);
  SetCtrlVal(fanPNL,FAN_PNL_ADCSTR,MsgTxt);
  SetCtrlVal(fanPNL,FAN_PNL_TERMOMETER,Temp);

 
  ProcessDrawEvents();

 } // while

 if (FanOk == TRUE)
   MessagePopup(" Blæser test"," OK");
  else 
   MessagePopup(" Blæser test"," FEJLET");

 HidePanel(fanPNL);
return FanOk;
}





//----------------------------------------------------------------------------
int CheckOven(void) {
// Spændingen måles i TP7
double SPGTP7meas;
int handle_returned, controlID_returned;
char MsgTxt[30];

 CheckingLTCVCO = FALSE;
 CheckingOven = TRUE;
 CheckingPS = FALSE;
 CheckingAudioVCO = FALSE;
 DisplayPanel(vccPNL);
 SetPanelAttribute (vccPNL, ATTR_TITLE, " Kontrol af Ovn ");
 ResetTextBox (vccPNL, VCC_PNL_TXTBOX, "");

 ErrIEEE = hp34401a_conf (hp34401, 1, VI_ON, 5, 1);

 if (CheckFrontRearTerminal(FRONTTERM34401) == FALSE) {
  HidePanel(vccPNL);
  return FALSE;
 }

 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (vccPNL);
  return FALSE;
  }

 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MAX_VALUE, SPGTP7Max + 0.5);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MIN_VALUE, SPGTP7Min - 0.5);
 
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Spændingen i TP7 kontrolleres, når apparatet har været tændt i > 15 min.\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med HP34401 i TP7\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved fejl:  Check spændingen i V107, pin 2 = 10.4V\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, "            Check spændingen i V111, pin 4 =  3.8V\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved OK:   Tryk F4\n");
 Fmt(MsgTxt,"%s<%f[p2] - %f[p2] VDC",SPGTP7Min,SPGTP7Max);
 
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_LABEL_TEXT,MsgTxt);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_TEXT_JUSTIFY,VAL_CENTER_JUSTIFIED);

 done = FALSE;

 while (done == FALSE) {
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  DisableBreakOnLibraryErrors();
  ErrIEEE = hp34401a_singleMeas (hp34401, &SPGTP7meas);
  EnableBreakOnLibraryErrors();
  SetCtrlVal (vccPNL, VCC_PNL_VCCSLIDE, SPGTP7meas);
  if ((SPGTP7meas > SPGTP7Max) || (SPGTP7meas < SPGTP7Min)) {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_RED);
	SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_RED);
   }
   else {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_GREEN);
    SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
   } 
    

 } // while

  if (SPGTP7Ok == TRUE)
    MessagePopup (" Ovn Temperatur "," Måling i TP7\n OK");
   else 
    MessagePopup (" Ovn Temperatur "," Måling i TP7\n FEJLET");
 
 HidePanel(vccPNL);
 CheckingOven = FALSE;
 
return SPGTP7Ok;
} //CheckOven




//----------------------------------------------------------------------------
int CheckAudioGenVCO(void) {
// Spændingen måles i TP1
double AudioTP1meas;
int handle_returned, controlID_returned;
char MsgTxt[30];

 CheckingLTCVCO = FALSE;
 CheckingAudioVCO = TRUE;
 CheckingOven = FALSE;
 CheckingPS = FALSE;
 DisplayPanel(vccPNL);
 ResetTextBox (vccPNL, VCC_PNL_TXTBOX, "");
 SetPanelAttribute (vccPNL, ATTR_TITLE, " Audio Generator: Kontrol af VCO spænding ");

 ErrIEEE = hp34401a_conf (hp34401, 1, VI_ON, 5, 1);

 if (CheckFrontRearTerminal(FRONTTERM34401) == FALSE) {
  HidePanel(vccPNL);
  return FALSE;
 }

 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MAX_VALUE, AudioTP1Max + 1.0);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MIN_VALUE, AudioTP1Min - 1.0);
 
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Spændingen i TP1 kontrolleres, når apparatet har været tændt i > 15 min.\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med HP34401 i TP1\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved OK:   Tryk F4\n");
 Fmt(MsgTxt,"%s<%f[p2] - %f[p2] VDC",AudioTP1Min,AudioTP1Max);
 
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_LABEL_TEXT,MsgTxt);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_TEXT_JUSTIFY,VAL_CENTER_JUSTIFIED);

 done = FALSE;

 while (done == FALSE) {
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  DisableBreakOnLibraryErrors();
  ErrIEEE = hp34401a_singleMeas (hp34401, &AudioTP1meas);
  EnableBreakOnLibraryErrors();
  SetCtrlVal (vccPNL, VCC_PNL_VCCSLIDE, AudioTP1meas);
  if ((AudioTP1meas > AudioTP1Max) || (AudioTP1meas < AudioTP1Min)) {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_RED);
	SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_RED);
    }
   else {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_GREEN);
	SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
    }

 } // while


 HidePanel(vccPNL);
 CheckingAudioVCO = FALSE;
 
return AudioTP1Ok;
} //CheckAudioGenVCO





//----------------------------------------------------------------------------
int CheckLTCVCO(int TVSystem) {
// Spændingen måles i TP7
double LTCTP7meas,TP7Max,TP7Min;
int handle_returned, controlID_returned;
char MsgTxt[30];
short AddrList[2] = {3,-1};

 EnableLocal (0, AddrList);

 CheckingLTCVCO = TRUE;
 CheckingAudioVCO = FALSE;
 CheckingOven = FALSE;
 CheckingPS = FALSE;
 DisplayPanel(vccPNL);
 ResetTextBox (vccPNL, VCC_PNL_TXTBOX, "");

 if (TVSystem == PAL){
   TP7Max = LTC_TP7MaxG;
   TP7Min = LTC_TP7MinG;
   SetPanelAttribute (vccPNL, ATTR_TITLE, " LTC: Kontrol af VCO spænding (PAL) ");
   }
  else {
   TP7Max = LTC_TP7MaxM;
   TP7Min = LTC_TP7MinM;
   SetPanelAttribute (vccPNL, ATTR_TITLE, " LTC: Kontrol af VCO spænding (NTSC) ");
   }

 ErrIEEE = hp34401a_conf (hp34401, 1, VI_ON, 5, 1);
 
 if (CheckFrontRearTerminal(FRONTTERM34401) == FALSE) {
  HidePanel(vccPNL);
  return FALSE;
 }

 
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel(vccPNL);
  return FALSE;
 }
 
 
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MAX_VALUE, TP7Max + 1.0);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_MIN_VALUE, TP7Min - 1.0);
 
 if (TVSystem == PAL){
   Set5640Standard("G");
   WriteCOMPort(DUTCOM,":INP:GENL:INP A; SYST PALB;");
  }
  else{
   Set5640Standard("M");
   WriteCOMPort(DUTCOM,":INP:GENL:INP A; SYST NTSC;");
  }
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Spændingen i TP7 kontrolleres, når apparatet har været tændt i > 15 min.\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Der måles med HP34401 i TP7\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved fejl: Check spænding i TP6 = 5 Volt\n");
 if (TVSystem == PAL)
   SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, "           Check frekvens i TP8 = 4.000 kHz\n\n");
  else
   SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, "           Check frekvens i TP8 = 4.795 kHz\n\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(vccPNL,VCC_PNL_TXTBOX, " Ved OK:   Tryk F4");
 Fmt(MsgTxt,"%s<%f[p2] - %f[p2] VDC",TP7Min,TP7Max);
 
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_LABEL_TEXT,MsgTxt);
 SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_TEXT_JUSTIFY,VAL_CENTER_JUSTIFIED);

 done = FALSE;

 while (done == FALSE) {
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  DisableBreakOnLibraryErrors();
  ErrIEEE = hp34401a_singleMeas (hp34401, &LTCTP7meas);
  EnableBreakOnLibraryErrors();
  SetCtrlVal (vccPNL, VCC_PNL_VCCSLIDE, LTCTP7meas);
  if ((LTCTP7meas > TP7Max) || (LTCTP7meas < TP7Min)) {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_RED);
	SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_RED);
    }
   else {
    SetCtrlAttribute (vccPNL, VCC_PNL_VCCSLIDE, ATTR_FILL_COLOR, VAL_GREEN);
	SetCtrlAttribute (vccPNL, VCC_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
    }

 } // while


 HidePanel(vccPNL);
 CheckingLTCVCO = FALSE;
 
return LTCTP7Ok;
} //CheckLTCVCO










//----------------------------------------------------------------------------
int CVICALLBACK ScHPilopCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(phasePNL,SCH_PNL_SCH_SLIDE,&ScHValue);
			ScHValue = ScHValue + 3;
			SetCtrlVal(phasePNL,SCH_PNL_SCH_SLIDE,ScHValue);
			if (ScHValue < 0)
			 ScHValue = 2048 + ScHValue;
			SetScH(ScHValue + ScHOffset);
			break;
	}
	return 0;
}

int CVICALLBACK ScHPilnedCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(phasePNL,SCH_PNL_SCH_SLIDE,&ScHValue);
			ScHValue = ScHValue - 3;
			SetCtrlVal(phasePNL,SCH_PNL_SCH_SLIDE,ScHValue);
			if (ScHValue < 0)
			 ScHValue = 2048 + ScHValue;
			SetScH(ScHValue + ScHOffset);
			break;
	}
	return 0;
}

int CVICALLBACK SchSlideCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_VAL_CHANGED:
			GetCtrlVal(phasePNL,SCH_PNL_SCH_SLIDE,&ScHValue);
			if (ScHValue < 0)
			 ScHValue = 2048 + ScHValue;
			SetScH(ScHValue + ScHOffset);
			break;
	}
	return 0;
}



int CVICALLBACK PhasePilopCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(phasePNL,SCH_PNL_PHASE_SLIDE,&PhaseValue);
			PhaseValue = PhaseValue + 5;
			SetCtrlVal(phasePNL,SCH_PNL_PHASE_SLIDE,PhaseValue);
			if (TV == PAL) {
			 if (PhaseValue < 0)
			  PhaseValue = 0x41eb0000 + PhaseValue;
			}  
			if (TV == NTSC) {
			 if (PhaseValue < 0)
			  PhaseValue = 0x1b7e4800 + PhaseValue;
			}
			SetPhase(PhaseValue + PhaseOffset);
			break;
	}
	return 0;
}


int CVICALLBACK PhasePilnedCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(phasePNL,SCH_PNL_PHASE_SLIDE,&PhaseValue);
			PhaseValue = PhaseValue - 5;
			SetCtrlVal(phasePNL,SCH_PNL_PHASE_SLIDE,PhaseValue);
			if (TV == PAL) {
			 if (PhaseValue < 0)
			  PhaseValue = 0x41eb0000 + PhaseValue;
			}  
			if (TV == NTSC) {
			 if (PhaseValue < 0)
			  PhaseValue = 0x1b7e4800 + PhaseValue;
			}
			SetPhase(PhaseValue + PhaseOffset);
			break;
	}
	return 0;
}


int CVICALLBACK PhaseSlideCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_VAL_CHANGED:
			GetCtrlVal(phasePNL,SCH_PNL_PHASE_SLIDE,&PhaseValue);
			if (TV == PAL) {
			 if (PhaseValue < 0)
			  PhaseValue = 0x41eb0000 + PhaseValue;
			}  
			if (TV == NTSC) {
			 if (PhaseValue < 0)
			  PhaseValue = 0x1b7e4800 + PhaseValue;
			}
			SetPhase(PhaseValue + PhaseOffset);
			break;
	}
	return 0;
}



int CVICALLBACK ExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			done = TRUE;
			if (panel == phasePNL)
			 PhaseScHOk = FALSE;

			break;
	}
	return 0;
}

int CVICALLBACK Phase_LeftCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetCtrlAttribute (phasePNL, SCH_PNL_SCHPILOP_BTN, ATTR_DIMMED,TRUE);
		 SetCtrlAttribute (phasePNL, SCH_PNL_SCHPILNED_BTN, ATTR_DIMMED,TRUE);
		 SetCtrlAttribute (phasePNL, SCH_PNL_PHASEPILOP_BTN, ATTR_DIMMED,FALSE);
		 SetCtrlAttribute (phasePNL, SCH_PNL_PHASEPILNED_BTN, ATTR_DIMMED,FALSE);

		 SetCtrlAttribute (phasePNL, SCH_PNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
		 SetCtrlAttribute (phasePNL, SCH_PNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
			break;
	}
	return 0;
}

int CVICALLBACK Phase_RightCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		 SetCtrlAttribute (phasePNL, SCH_PNL_PHASEPILOP_BTN, ATTR_DIMMED,TRUE);
		 SetCtrlAttribute (phasePNL, SCH_PNL_PHASEPILNED_BTN, ATTR_DIMMED,TRUE);
		 SetCtrlAttribute (phasePNL, SCH_PNL_SCHPILOP_BTN, ATTR_DIMMED,FALSE);
		 SetCtrlAttribute (phasePNL, SCH_PNL_SCHPILNED_BTN, ATTR_DIMMED,FALSE);
		
		 SetCtrlAttribute (phasePNL, SCH_PNL_SCH_DEC, ATTR_FRAME_COLOR,VAL_BLUE);
		 SetCtrlAttribute (phasePNL, SCH_PNL_PHASE_DEC, ATTR_FRAME_COLOR,VAL_LT_GRAY);
			break;
	}
	return 0;
}

int CVICALLBACK PhaseScHOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			done = TRUE;
			PhaseScHOk = TRUE;
			break;
	}
	return 0;
}



// Læser kalibreringdata fra EEPROM'mer i diverse units
int CVICALLBACK CfgOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: Cont = TRUE; break;
		
	}
	return 0;
}

int CVICALLBACK CfgSaveToFileCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: SaveToFile = TRUE; Cont = TRUE; break;
		
	}
	return 0;
}



int CVICALLBACK VCCOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: 	done = TRUE;
	 						if (CheckingPS == TRUE)
	 						 VoltOk = TRUE;
	 						if (CheckingOven == TRUE)
	 						 SPGTP7Ok = TRUE;
	 						if (CheckingAudioVCO == TRUE)
	 						 AudioTP1Ok = TRUE;
	 						if (CheckingLTCVCO == TRUE)
	 						 LTCTP7Ok = TRUE;
							break;
	}
	return 0;
}

int CVICALLBACK VCCExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:	done = TRUE;
	 						if (CheckingPS == TRUE)
	 						 VoltOk = FALSE;
	 						if (CheckingOven == TRUE)
	 						 SPGTP7Ok = FALSE;
	 						if (CheckingAudioVCO == TRUE)
	 						 AudioTP1Ok = FALSE;
	 						if (CheckingLTCVCO == TRUE)
	 						 LTCTP7Ok = FALSE;
							break;
	}
	return 0;
}

int CVICALLBACK FanOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: done = TRUE; FanOk = TRUE;
						   break;
	}
	return 0;
}

int CVICALLBACK FanExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: done = TRUE;  FanOk = FALSE;
						   break;
	}
	return 0;
}

int CVICALLBACK DataOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: Cont = TRUE;
						   break;
	}
	return 0;
}
