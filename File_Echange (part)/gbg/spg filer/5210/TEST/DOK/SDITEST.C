#include "hp34401a.h"
#include <analysis.h>
#include <rs232.h>
#include <ansi_c.h>
#include "sdi.h"
#include <utility.h>
#include <formatio.h>
#include <userint.h>
#include "cviutil.h"
#include "vcautil.h"
#include "pt5210.h"

// sditest.c
// PT5210: 		 SDI BLACK og SDI TSG
// PF 970130





// constants
const float JT_peakpeakMax = 0.12;	  		// UI

const float CheckFreqAmsp[2] = {20,350};		// Frekevns hvor ampl. spec. checkes
const float	AMSP_Max[2] = {+0.5,-10};		
const float	AMSP_Min[2] = {-0.5,-40};

const float CheckRLoss[3] = {20 , 150, 270};	// Frekevns hvor return loss. checkes
const float	RLoss_Max[3]  = {-30, -25, -20};	// Max level ved checkfrekvensen

const double VCOAdjTol = 0.02;					// ± just tol ved SDI genlock justering (TP1/R6)

// variables
int 
	jitPNL,
	sdiPNL,
	sdipattPNL,
	amspPNL,
	rlossPNL,
	sdigenlPNL;
	
int SDIJitterOk,
	SDITimingOk,
	SDIAmplSpecOk,
	SDIRLossOk,
	SDIGenlOk,
	SDIPattOk,
	SDIEmbAudioOk,
    SDIEDHOk,
	VCAOk;



int InitSDI(int SDINo);
int WriteSDICalDate(int BBNo);
int SaveInEEPROM(int SDINo, int TxtNo, char Text[]);
int SDIGenlockTest(void);
char *ReadSDIGenlKUNumber (void);
char *ReadSDIGenlProdDate (void);
char *ReadSDIGenlCalDate (void);












//--------------------------------------------------------------------------------
int SDICal(int TVLines, int SDINo){
char 	TxtStr[100],resp[20],
		*strread,*HBread;
int 	handle_returned, 
		controlID_returned;		
int 	bytes_read,
		k;
unsigned int SDIphase, 
			 phase_read,
			 MaxPhase;
float sdidelay;




// SDINo:     SDI TSG = 1       SDI Black = 34, 56 eller 78
// TVLines:   L625 eller L525

 if (SDINo > 30)	
   Fmt(TxtStr,"%s< SDI Black %i   (SDI %i) ",SDINo,TVLines);
  else
   Fmt(TxtStr,"%s< SDI Test Signal Generator   (SDI %i) ",TVLines);
 SetPanelAttribute (sdiPNL, ATTR_TITLE, TxtStr);
 SetCtrlAttribute (sdiPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);
 DisplayPanel (sdiPNL);
 ResetTextBox(sdiPNL, SDI_PNL_TXTBOX,"");


 SDITimingOk = WriteProtect(OFF,FALSE);
 if (SDITimingOk == FALSE){
  HidePanel (sdiPNL);
  return SDITimingOk;
 }

 // read 12nc
 FlushInQ(DUTCOM);
 switch (SDINo){
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',1;"); break;
  case 2: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',1;"); break;
  case 3: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',1;"); break;
 }
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strread[8] != '4'){ 			  // 12nc = "109 82940"
  SDITimingOk = FALSE;
  MessagePopup(" SDI Kalibrering"," SDI Black ikke monteret i dette apparat !\n Test afbrydes");
  HidePanel (sdiPNL);
  return SDITimingOk;
 }
 
 
 

 if (TVLines == L625)
  InitSDI(SDINo);
 Delay(0.5);
 
 if (TVLines == L625)
  VCAOk = CheckReadyVCA();
  
 if (VCAOk == FALSE){
  HidePanel(sdiPNL);
  return FALSE;
 }
 
 vca_signaldelay();
 
 if (TVLines == L625){ 
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind PT5210 D1 Black til DUT SDI GENLOCK IN\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind DUT SDI GENLOCK OUT til R&S VCA A-in\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n F4: OK\n F9: STOP\n");
  WaitForContinue();
 }
 
 if (TVLines == L525)
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n F9: STOP\n");

 SetCtrlAttribute (sdiPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, FALSE);
 
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n Kalibrerer *");

 if (TVLines == L625) {
  WriteCOMPort(PT5210COM,":OUTP:SB2:SYST SDI625; PATT BLACK; "); 
  WriteCOMPort(DUTCOM,":INP:GENL:INP SDI; SYST SDI625; DEL 0,0,0;"); 
  vca_selectsystem(L625);
 }
 else {
  WriteCOMPort(PT5210COM,":OUTP:SB2:SYST SDI525; PATT BLACK; "); 
  WriteCOMPort(DUTCOM,":INP:GENL:INP SDI; SYST SDI525; DEL 0,0,0;"); 
  vca_selectsystem(L525);
 }

 Delay(0.2);
 

 
 
 switch (SDINo) {
  case 34: if (TVLines == L625)
  			 WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  			 WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;");     // SDI black
		   break;

  case 56: if (TVLines == L625)
  			 WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;"); 
  		   break;

  case 78: if (TVLines == L625)
  		  	 WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;");
  		   break;

  case 1:  if (TVLines == L625)
  		  	 WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;");		// SDT TSG
  		   break;   
  }
 Delay(0.2);

 // Reset SDI black
 if ((TVLines == L625) && (SDINo > 1)){		 // SDI BLACK  625
  WriteCOMPort(DUTCOM, "COMM 'HG',0;");
  Delay(0.02);
  WriteCOMPort(DUTCOM, "COMM 'HX$';");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'HO',0;");
  Delay(0.02);
  MaxPhase = 276480000;		// 625*1728*256
 } 
 else
 if ((TVLines == L525) && (SDINo > 1)){		 // SDI BLACK  525
  WriteCOMPort(DUTCOM, "COMM 'HM',0;");
  Delay(0.02);
  WriteCOMPort(DUTCOM, "COMM 'HY$';");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'HO',0;");
  Delay(0.02);
  MaxPhase = 230630400;		// 525*1716*256
 } 
 else
 if ((TVLines == L625) && (SDINo == 1)){		 // SDI TSG  625
  WriteCOMPort(DUTCOM, "COMM 'SG',0;");
  Delay(0.02);
  WriteCOMPort(DUTCOM, "COMM 'SX$';");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'SO',0;");
  Delay(0.02);
  MaxPhase = 276480000;		// 625*1728*256
 } 
 else
 if ((TVLines == L525) && (SDINo == 1)){		 // SDI TSG  525
  WriteCOMPort(DUTCOM, "COMM 'SM',0;");
  Delay(0.02);
  WriteCOMPort(DUTCOM, "COMM 'SY$';");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'SO',0;");
  Delay(0.02);
  MaxPhase = 230630400;		// 525*1716*256
 } 


 done = FALSE;
 SDIphase = 0;

 if ((TVLines == L625) && (SDINo > 1))
   Fmt(DUTStr,"%s<COMM 'HG',%i;",SDIphase);
 else  
 if ((TVLines == L525) && (SDINo > 1))
   Fmt(DUTStr,"%s<COMM 'HM',%i;",SDIphase);
 else  
 if ((TVLines == L625) && (SDINo == 1))
   Fmt(DUTStr,"%s<COMM 'SG',%i;",SDIphase);
 else  
 if ((TVLines == L525) && (SDINo == 1))
   Fmt(DUTStr,"%s<COMM 'SM',%i;",SDIphase);
 
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(3);
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," *");
 bytes_read = vca_signaldelay_readdelay(&sdidelay);
 Delay(0.5);

 SDITimingOk = FALSE;
 
 do {
  bytes_read = vca_signaldelay_readdelay(&sdidelay);
  
   if ((sdidelay == 0.0) && (bytes_read > 0))
     SDITimingOk = TRUE;
    else
     SDITimingOk = FALSE;

  FlushInQ(DUTCOM);
  
  Fmt(TxtStr,"%s< %f[b4p3] µS    (%s)",sdidelay*1e6,DUTStr);
  SetCtrlVal (sdiPNL, SDI_PNL_DELAY, TxtStr);

//  PromptPopup ("HG?", "", resp, 12);
//  Scan(resp,"%s>%i[u]",&SDIphase);
  
  k = (int)((sdidelay / 37.037037e-9) * 256);
  if (abs(k) > MaxPhase)
   k = 0;
  if (k > 0)
    SDIphase = MaxPhase - k;
  if (k < 0)
    SDIphase = abs(k);
  
  

  if ((TVLines == L625) && (SDINo > 1))
    Fmt(DUTStr,"%s<COMM 'HG',%i;",SDIphase);
  else  
  if ((TVLines == L525) && (SDINo > 1))
    Fmt(DUTStr,"%s<COMM 'HM',%i;",SDIphase);
  else  
  if ((TVLines == L625) && (SDINo == 1))
    Fmt(DUTStr,"%s<COMM 'SG',%i;",SDIphase);
  else  
  if ((TVLines == L525) && (SDINo == 1))
    Fmt(DUTStr,"%s<COMM 'SM',%i;",SDIphase);

  WriteCOMPort(DUTCOM, DUTStr);

  Delay(3);
 

 if (SDITimingOk == FALSE)
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," *");
  
  
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 
 } while ((SDITimingOk == FALSE) && (done == FALSE));
 

 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n");
 
 
 
 // gem data i EEPROM
 Delay(0.1);
 if (SDITimingOk == TRUE) {
  if ((TVLines == L625) && (SDINo > 1))
    WriteCOMPort(DUTCOM,"COMM 'HX$';");
   else
  if ((TVLines == L525) && (SDINo > 1))
    WriteCOMPort(DUTCOM,"COMM 'HY$';");
   else
  if ((TVLines == L625) && (SDINo == 1))
    WriteCOMPort(DUTCOM,"COMM 'SX$';");
   else
  if ((TVLines == L525) && (SDINo == 1))
    WriteCOMPort(DUTCOM,"COMM 'SY$';");
 
  // check at data er gemt korrekt i EEPROM
  Delay(0.1);
  FlushInQ(DUTCOM);
  if ((TVLines == L625) && (SDINo > 1))
    WriteCOMPort(DUTCOM,"COMM 'HX?';");
  else
  if ((TVLines == L525) && (SDINo > 1))
    WriteCOMPort(DUTCOM,"COMM 'HY?';");
  else
  if ((TVLines == L625) && (SDINo == 1))
    WriteCOMPort(DUTCOM,"COMM 'SX?';");
  else
  if ((TVLines == L525) && (SDINo == 1))
    WriteCOMPort(DUTCOM,"COMM 'SY?';");
    
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&phase_read);
  if (SDIphase != phase_read)
   SDITimingOk = FALSE;
 } //if
 
 
 if (SDITimingOk == TRUE) {
   if (TVLines == L525)
    MessagePopup("Delay kalibrering"," OK\n Kalibreringsdata gemt i EEPROM");
   WriteSDICalDate(SDINo);
   }
  else {
   switch (SDINo){
     case 34: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;COMM 'HW',4,'000000';"); break;
     case 56: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;COMM 'HW',4,'000000';"); break;
     case 78: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;COMM 'HW',4,'000000';"); break;
     case 1:  Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;COMM 'SW',4,'000000';"); break;
   }
   WriteCOMPort(DUTCOM, DUTStr);
   MessagePopup("SDI delay kalibrering"," FEJLET\n Kalibreringsdata ikke gemt i EEPROM");
 }//if 

HidePanel(sdiPNL);
return SDITimingOk;
}






//-------------------------------------------------------------------------------
int CheckSDIPattern(int SDINo){
double PatternShiftDelay;
int handle_returned, controlID_returned;

// SDINo:     SDI TSG = 1       SDI Black = 34, 56 eller 78
// TVLines:   L625 eller L525

 SDIPattOk = TRUE;
 PatternShiftDelay = 1.5;

 DisplayPanel (sdipattPNL);
 SetPanelAttribute (sdipattPNL, ATTR_TITLE, " SDI Pattern ");
 ResetTextBox(sdipattPNL, SDIPAT_PNL_TXTBOX,"");

 SDIPattOk = WriteProtect(OFF,FALSE);
 if (SDIPattOk == FALSE){
  HidePanel (sdipattPNL);
  return SDIPattOk;
 }
 
 
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Forbind SDI generator til D1 DAC IN\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Barco monitor indstilles til component YUV: Tast 'COMP'\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," På monitor vises derefter følgende pattern i rækkefølge:\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," 625:   EBU Colorbar    100% Colorbar    Black\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," 525:   SMPTE Colorbar  Black\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," HUSK: Test begge udgange fra generatoren !!");

 WaitForContinue();
 
 if (SDIPattOk == FALSE){
  MessagePopup (" SDI Pattern Test", "FEJLET");
  HidePanel(sdipattPNL);
  return FALSE;
 }

 switch (SDINo) {
  case 34: WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI625;"); break;
  case 56: WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI625;"); break;
  case 78: WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI625;"); break;
  case 1:  WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;"); break;
 
 }
 
 // <<<<<<<<<<<<<<<<< TSG pattern test mangler <<<<<<<<<<<<<<<<<<<<
 done = FALSE;
 while (done == FALSE){
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBEBU;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT CB100;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT BLACK;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT CBSMPTE;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT BLACK;");
  Delay(PatternShiftDelay);
 }

 if (SDIPattOk == FALSE){
  MessagePopup (" SDI Pattern Test", "FEJLET");
  HidePanel(sdipattPNL);
  return FALSE;
 }

 SetPanelAttribute (sdipattPNL, ATTR_TITLE, " SDI - BB Timing ");
 WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBEBU; DEL 0,0,0;");

 WriteCOMPort(DUTCOM,":OUTP:BB1:SYST PAL_ID; DEL 0,0,0;");
 WriteCOMPort(DUTCOM,":OUTP:BB2:SYST PAL_ID; DEL 0,0,0;");
 
 SelectPM3094Setup(5);
 ResetTextBox(sdipattPNL, SDIPAT_PNL_TXTBOX,"");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Forbind SDI generator til D1 DAC IN\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Forbind BARCO MONITOR Y til scop CH-1\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Forbind Black Burst #1 til scop CH-2\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Check på scopet, at videosignalerne i CH-1 og CH-2 er i fase\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP");

 WaitForContinue();
 

 if (SDIPattOk == TRUE)
   MessagePopup (" SDI Pattern Test + BB Timing", "OK");
  else
   MessagePopup (" SDI - BB Timing", "FEJLET");


HidePanel(sdipattPNL);

return TRUE;
} //CheckSDIPattern
 





//---------------------------------------------------------------------------------
int CheckSDIEmbAudio_EDH(int SDINo){
int handle_returned, controlID_returned;
 
 SDIEmbAudioOk = TRUE;

 DisplayPanel (sdipattPNL);
 SetPanelAttribute (sdipattPNL, ATTR_TITLE, " SDI Embedded Audio ");
 ResetTextBox(sdipattPNL, SDIPAT_PNL_TXTBOX,"");
 
 
 SDIEmbAudioOk = WriteProtect(OFF,FALSE);
 if (SDIEmbAudioOk == FALSE){
  HidePanel (sdipattPNL);
  return SDIEmbAudioOk;
 }
 

 
 // Embedded Audio = Silence
 switch (SDINo) {
  case 34: WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI625;EMB SIL;"); break;
  case 56: WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI625;EMB SIL;"); break;
  case 78: WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI625;EMB SIL;"); break;
  case 1:  WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;EMB SIL;"); break;
 } 
 
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Forbind SDI generator til DEX3072 D1 IN\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Forbind DEX3072 AES75 OUT A til TEK764 AES75 IN\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Indstil DEX3072: CH SELECT A = 0 og derefter = 1\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX,"        Aflæs:  A = grøn LED\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Indstil TEK764:  AUDIO\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX,"        Aflæs:  CH-1 og CH-2 = MUTE\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Indstil TEK764:  CH STATUS, Text\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX,"        Aflæs:  Block CRC = valid\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX,"        Aflæs:  Channel use = professional\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n");


 WaitForContinue();
 
 if (SDIEmbAudioOk == FALSE){
  MessagePopup (" SDI Embedded Audio", "FEJLET");
  HidePanel(sdipattPNL);
  return SDIEmbAudioOk;
 }

 
 
 //  Embedded Audio = OFF 
 switch (SDINo) {
  case 34: WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI625;EMB OFF;"); break;
  case 56: WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI625;EMB OFF;"); break;
  case 78: WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI625;EMB OFF;"); break;
  case 1:  WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;EMB OFF;"); break;
 } 

 ResetTextBox(sdipattPNL, SDIPAT_PNL_TXTBOX,"");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX,"\n Indstil TEK764:  AUDIO\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX,"        Aflæs:  CH-1 og CH-2 = VBIT SET\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n");
 
 WaitForContinue();
 
 if (SDIEmbAudioOk == FALSE){
  MessagePopup (" SDI Embedded Audio", "FEJLET");
  HidePanel(sdipattPNL);
  return SDIEmbAudioOk;
 }

 

//  EDH = ON Test 
 SDIEDHOk = TRUE;
 vca_selectsystem(L625);
 vca_selectinput(SERIAL_B);
 
 switch (SDINo) {
  case 34: WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI625;EDH ON;"); break;
  case 56: WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI625;EDH ON;"); break;
  case 78: WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI625;EDH ON;"); break;
  case 1:  WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;EDH ON;"); break;
 } 

 SetPanelAttribute (sdipattPNL, ATTR_TITLE, " SDI EDH Test ");
 ResetTextBox(sdipattPNL, SDIPAT_PNL_TXTBOX,"");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Forbind SDI generator til VCA B-IN\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Indstil R&S VCA:  SCOPE  -  ANC DATA STATUS\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Aflæs VCA:    EDH  YES\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n");
 
 WaitForContinue();
 
 if (SDIEDHOk == FALSE){
  MessagePopup (" SDI  EDH", "FEJLET");
  HidePanel(sdipattPNL);
  return SDIEmbAudioOk;
 }

 // EDH = OFF Test
 switch (SDINo) {
  case 34: WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI625;EDH OFF;"); break;
  case 56: WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI625;EDH OFF;"); break;
  case 78: WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI625;EDH OFF;"); break;
  case 1:  WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;EDH OFF;"); break;
 } 

 ResetTextBox(sdipattPNL, SDIPAT_PNL_TXTBOX,"");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX,"\n Aflæs VCA:    EDH  NO\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n");
 
 WaitForContinue();
 
 if (SDIEDHOk == FALSE){
  MessagePopup (" SDI  EDH", "FEJLET");
  HidePanel(sdipattPNL);
  return SDIEmbAudioOk;
 }

 
 
 
 if (SDINo > 1) {
  MessagePopup (" SDI Embedded Audio & EDH", "OK");
  HidePanel(sdipattPNL);
  return SDIEmbAudioOk;
 }  

 

  
// SDI TSG Embedded Audio Test  <<<<<<<<<<<<<<<<<<<<<<< ikke færdig - mangler audio signal test
  
 WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;EMB SIL;");

  
// ------- Audio level test ------------------------------------------------------
 SetPanelAttribute (sdipattPNL, ATTR_TITLE, " SDI TSG Embedded Audio ");
 ResetTextBox(sdipattPNL, SDIPAT_PNL_TXTBOX,"");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Aflæs level på TEK 764\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Level skal skifte mellem 0, -9, -12, -14, -16, -18, -20 og MUTE\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," OBS: Mellem levelskiftene vises der muligvis CODE ERROR. Dette er ingen fejl!\n\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," OK:    Tast F4\n");
 SetCtrlVal(sdipattPNL, SDIPAT_PNL_TXTBOX," Fejl:  Tast F9\n\n");

 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:SIGN S1KHZ;");
 
 Cont = FALSE;
 while (Cont == FALSE) {
 
 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB0FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM9FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM12FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM14FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM16FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM18FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM20FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV SIL;");
 Delay(2);
 
 } // while

 WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM18FS;");
  

 if (SDIEmbAudioOk == TRUE)
   MessagePopup (" SDI TSG Embedded Audio", "OK");
  else 
   MessagePopup (" SDI TSG Embedded Audio", "FEJLET");

  
 HidePanel(sdipattPNL);

 return SDIEmbAudioOk;

} //CheckSDIEmbAudio









//---------------------------------------------------------------------------
int SDIJitterTest(int SDINo){
float jitterPPmeas[20], jitterMean;
char TxtStr[60];
int handle_returned, controlID_returned, n;

// init panel
 if (SDINo > 30)	
   Fmt(TxtStr,"%s< Jittermålinger:  SDI Black %i ",SDINo);
  else
   Fmt(TxtStr,"%s< Jittermålinger:  SDI Test Signal Generator ");
 SetPanelAttribute (jitPNL, ATTR_TITLE, TxtStr);
 SetCtrlAttribute (jitPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);

 DisplayPanel (jitPNL);
 						   
 ResetTextBox(jitPNL, JIT_PNL_PPMEAS,"");
 SetCtrlVal(jitPNL, JIT_PNL_PPTOL ,"");
 SetCtrlVal(jitPNL, JIT_PNL_PPMEAN ,"");
 
 ResetTextBox(jitPNL, JIT_PNL_TXTBOX,"");
 
 
 SDIJitterOk = WriteProtect(OFF,FALSE);
 if (SDIJitterOk == FALSE){
  HidePanel (jitPNL);
  return SDIJitterOk;
 }
 
 
 
 
 SetCtrlVal(jitPNL, JIT_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n\n");
 SetCtrlVal(jitPNL, JIT_PNL_TXTBOX," F4: OK \n F9: STOP\n");

 Fmt(TxtStr,"%s< %f[b4] UI", JT_peakpeakMax);
 SetCtrlVal(jitPNL, JIT_PNL_PPTOL ,TxtStr);

 SDIJitterOk = TRUE; 

 WaitForContinue();
 
 if (SDIJitterOk == FALSE) {
  HidePanel(jitPNL);
  return FALSE;
 }
 

 vca_selectsystem(L625);
 vca_selectinput(SERIAL_B);
 vca_jittertime();
 vca_jittertime_lpfilter(OFF);
 vca_jittertime_Xgain(X1);
 vca_jittertime_Ygain(Y1);
 vca_jittertime_meastime(FRAME4);
 vca_jittertime_average(A10);

 switch (SDINo) {
  case 34: WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI625; PATT CBEBU;"); break;
  case 56: WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI625; PATT CBEBU;"); break;
  case 78: WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI625; PATT CBEBU;"); break;
  case 1:  WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625; PATT CBEBU;"); break;
  }

 Delay(3.0);
 vca_jittertime_readpeakpeak(&jitterPPmeas[0]);
 
 // get 15 measurements
 for (n = 0; n < 15; n++) {
  Delay(1.0);
  vca_jittertime_readpeakpeak(&jitterPPmeas[n]);

  Fmt(TxtStr,"%s< %f[b4p3]\n", JT_peakpeak_ui);
  SetCtrlVal(jitPNL, JIT_PNL_PPMEAS ,TxtStr);
  
 } //for 

 // calculate mean
 jitterMean = 0;
 for (n = 10; n < 15; n++) 
  jitterMean = jitterMean + jitterPPmeas[n];

 jitterMean = jitterMean / 5;
 
 if (jitterMean > JT_peakpeakMax)
  SDIJitterOk = FALSE; 

 Fmt(TxtStr,"%s< %f[b4p3] UI", jitterMean);
 SetCtrlVal(jitPNL, JIT_PNL_PPMEAN ,TxtStr);
 
 if (SDIJitterOk == TRUE) {
   Fmt(TxtStr,"%s<%f[b4p1]",jitterMean);
   SaveInEEPROM(SDINo, 9, TxtStr);
   MessagePopup (" SDI Jitter Time", "Måling   OK");
   }
  else
   MessagePopup (" SDI Jitter Time", "Måling   FEJLET");

 
 HidePanel(jitPNL);
return SDIJitterOk;
}







//---------------------------------------------------------------------------------
int SDIAmplSpecTest(int SDINo){
char TxtStr[100];
int handle_returned, controlID_returned, n, amsp_handle;
float LevelMeas[2];

// init panel
 if (SDINo > 30)	
   Fmt(TxtStr,"%s< Amplitudespectrum:  SDI Black %i ",SDINo);
  else
   Fmt(TxtStr,"%s< Amplitudespectrum:  SDI Test Signal Generator ");
 SetPanelAttribute (amspPNL, ATTR_TITLE, TxtStr);
 SetCtrlAttribute (amspPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);
 
 Fmt(TxtStr,"%s< Level ved %f[b4p0] MHz: %f[b4p1] til %f[b4p1] dB",CheckFreqAmsp[0],AMSP_Min[0],AMSP_Max[0]);
 SetCtrlAttribute (amspPNL, AMSP_PNL_LEV_0, ATTR_LABEL_TEXT, TxtStr);
 Fmt(TxtStr,"%s< Level ved %f[b4p0] MHz:  max: %f[b4p1] dB",CheckFreqAmsp[1],AMSP_Max[1]);
 SetCtrlAttribute (amspPNL, AMSP_PNL_LEV_1, ATTR_LABEL_TEXT, TxtStr);

 DisplayPanel (amspPNL);
 DeleteGraphPlot (amspPNL, AMSP_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
 						   

 SDIAmplSpecOk = WriteProtect(OFF,FALSE);
 if (SDIAmplSpecOk == FALSE){
  HidePanel (amspPNL);
  return SDIAmplSpecOk;
 }
 

 
 SetCtrlVal(amspPNL, AMSP_PNL_LEV_0 ,"");
 SetCtrlVal(amspPNL, AMSP_PNL_LEV_1 ,"");

 ResetTextBox(amspPNL, AMSP_PNL_TXTBOX,"");
 SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n\n");
 SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");
 ProcessDrawEvents();

 SDIAmplSpecOk = TRUE; 
 
 WaitForContinue();
 
 if (SDIAmplSpecOk == FALSE) {
  HidePanel(amspPNL);
  return FALSE;
 }
 
 SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," Vent *");

 vca_selectsystem(L625);
 vca_selectinput(SERIAL_B);
 vca_amplspec();
 vca_amplspec_rescale();
 vca_amplspec_sweep(NORM);
 vca_amplspec_norm(OFF);
 vca_amplspec_Xgain(X50Div);		//   MHz/div
 vca_amplspec_Ygain(Y10Div);		//   dB/div
 

 switch (SDINo) {
  case 34: WriteCOMPort(DUTCOM,":OUTP:SB1:SYST SDI625; PATT CBEBU;"); break;
  case 56: WriteCOMPort(DUTCOM,":OUTP:SB2:SYST SDI625; PATT CBEBU;"); break;
  case 78: WriteCOMPort(DUTCOM,":OUTP:SB3:SYST SDI625; PATT CBEBU;"); break;
  case 1:  WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625; PATT CBEBU;"); break;
  }

 for (n = 0; n < 4; n++){
  SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," *");
  Delay(2.0);
 }

 SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX,"\n");
 vca_amplspec_readdata();
 
 LevelMeas[0] = AS_level[(int)(CheckFreqAmsp[0]/AS_freq_delta) - 1];
 LevelMeas[1] = AS_level[(int)(CheckFreqAmsp[1]/AS_freq_delta) - 1];
 
 Fmt(TxtStr,"%s< %f[b4p2] dB:",LevelMeas[0]);
 SetCtrlVal (amspPNL, AMSP_PNL_LEV_0, TxtStr);
 Fmt(TxtStr,"%s< %f[b4p2] dB:",LevelMeas[1]);
 SetCtrlVal (amspPNL, AMSP_PNL_LEV_1, TxtStr);
 
 amsp_handle = PlotXY (amspPNL, AMSP_PNL_GRAPH, AS_freq,
				AS_level,AS_data_length, VAL_FLOAT, VAL_FLOAT,VAL_THIN_LINE,
				VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_RED);

 if ((LevelMeas[0] > AMSP_Max[0]) || 
    (LevelMeas[0] < AMSP_Min[0])  || 
    (LevelMeas[1] > AMSP_Max[1]))
  SDIAmplSpecOk = FALSE;
 
 
 if (SDIAmplSpecOk == TRUE) 
   MessagePopup (" SDI Amplitude Spectrum", "Måling   OK");
  else
   MessagePopup (" SDI Amplitude Spectrum", "Måling   FEJLET");

 
 HidePanel(amspPNL);

return SDIAmplSpecOk;
} //SDIAmplSpecTest






//---------------------------------------------------------------------------------
int SDIReturnLossTest(int SDINo){

char TxtStr[100];
int handle_returned, controlID_returned, n, rloss_handle;
float LevelMeas[3];

// init panel
 if (SDINo > 30)	
   Fmt(TxtStr,"%s< Return Loss:  SDI Black %i ",SDINo);
  else
   Fmt(TxtStr,"%s< Return Loss:  SDI Test Signal Generator ");
 SetPanelAttribute (rlossPNL, ATTR_TITLE, TxtStr);
 SetCtrlAttribute (rlossPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);
 
 ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");

 DisplayPanel (rlossPNL);
 						   
 SDIRLossOk = WriteProtect(OFF,FALSE);
 if (SDIRLossOk == FALSE){
  HidePanel (rlossPNL);
  return SDIRLossOk;
 }
 

 ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 input forbindes til R&S VCA NOISE\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 output forbindes til R&S VCA B-in\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes IKKE!!!\n\n");
 if (SDINo > 30)	
   SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Placer jumper PP1 på SDI BLACK\n\n");
  else
   SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Placer jumper PP1 på SDI TSG\n\n");
 
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");

 WaitForContinue();

 if (SDIRLossOk == FALSE) {
  HidePanel(rlossPNL);
  return SDIRLossOk;
 }
 

 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
 vca_selectsystem(L625);
 vca_selectinput(SERIAL_B);
 vca_returnloss();
 vca_returnloss_rescale();
 vca_returnloss_Ygain(Y10Div);
 vca_returnloss_Xgain(X50Div);
 vca_returnloss_sweep(NORM);
 vca_returnloss_cursorfreq(270);
 vca_returnloss_norm(OFF);
 Delay(4);
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 vca_returnloss_norm(ON);
 Delay(4);

// Den ene SDI udgang måles
 ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
 ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes til SDI output\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebroen SKAL forbindes direkte til BNC-stik\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");
 DeleteGraphPlot (rlossPNL, RLOSS_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);

 WaitForContinue();

 if (SDIRLossOk == FALSE) {
  HidePanel(rlossPNL);
  return SDIRLossOk;
 }
 
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
 Delay(4);
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 
 vca_returnloss_readdata();
 
 LevelMeas[0] = RL_level[(int)(CheckRLoss[0]/RL_freq_delta) - 1];
 LevelMeas[1] = RL_level[(int)(CheckRLoss[1]/RL_freq_delta) - 1];
 LevelMeas[2] = RL_level[(int)(CheckRLoss[2]/RL_freq_delta) - 1];
 
 Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[0],LevelMeas[0],RLoss_Max[0]);
 SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
 Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[1],LevelMeas[1],RLoss_Max[1]);
 SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
 Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[2],LevelMeas[2],RLoss_Max[2]);
 SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);

 
 // plot data
 rloss_handle = PlotXY (rlossPNL, RLOSS_PNL_GRAPH, RL_freq,
				RL_level,RL_data_length, VAL_FLOAT, VAL_FLOAT,VAL_THIN_LINE,
				VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_RED);


 if ((LevelMeas[0] > RLoss_Max[0]) || 
    (LevelMeas[1] > RLoss_Max[1])  ||
    (LevelMeas[2] > RLoss_Max[2]))
  SDIRLossOk = FALSE;

 if (SDIRLossOk == FALSE) {
  MessagePopup (" SDI Return Loss", "Måling   FEJLET");
  HidePanel(rlossPNL);
  return SDIRLossOk;
 }  

 
 
 

// Den anden SDI udgang måles
 ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes til den anden SDI output\n\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," HUSK:  Fjern jumper PP1 efter måling!!\n\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");

 WaitForContinue();
 
 if (SDIRLossOk == FALSE) {
  HidePanel(rlossPNL);
  return SDIRLossOk;
 }

 ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");
 DeleteGraphPlot (rlossPNL, RLOSS_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
 ProcessDrawEvents();
 
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
 Delay(4);
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 
 vca_returnloss_readdata();
 
 LevelMeas[0] = RL_level[(int)(CheckRLoss[0]/RL_freq_delta) - 1];
 LevelMeas[1] = RL_level[(int)(CheckRLoss[1]/RL_freq_delta) - 1];
 LevelMeas[2] = RL_level[(int)(CheckRLoss[2]/RL_freq_delta) - 1];
 
 Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[0],LevelMeas[0],RLoss_Max[0]);
 SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
 Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[1],LevelMeas[1],RLoss_Max[1]);
 SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);
 Fmt(TxtStr,"%s< %f[w3b4p0]MHz: %f[w5b4p1]dB  (max: %f[w5b4p1])\n",CheckRLoss[2],LevelMeas[2],RLoss_Max[2]);
 SetCtrlVal(rlossPNL, RLOSS_PNL_MAXLEVEL,TxtStr);

 
 // plot data
 rloss_handle = PlotXY (rlossPNL, RLOSS_PNL_GRAPH, RL_freq,
				RL_level,RL_data_length, VAL_FLOAT, VAL_FLOAT,VAL_THIN_LINE,
				VAL_EMPTY_SQUARE, VAL_SOLID, 1,VAL_RED);


 if ((LevelMeas[0] > RLoss_Max[0]) || 
    (LevelMeas[1] > RLoss_Max[1]) ||
    (LevelMeas[2] > RLoss_Max[2]))
  SDIRLossOk = FALSE;
 


 
 
 
 if (SDIRLossOk == TRUE) {
   Fmt(TxtStr,"%s<%f[b4p1]",LevelMeas[2]);
   SaveInEEPROM(SDINo, 7, TxtStr);
   MessagePopup (" SDI Return Loss", "Måling   OK");
   }
  else
   MessagePopup (" SDI Return Loss", "Måling   FEJLET");


 HidePanel(rlossPNL);
return SDIRLossOk;
} //SDIReturnLossTest






//----------------------------------------------------------------------------------
char *ReadSDIGenlKUNumber (void) {
  // read SDI genlock KU nummer  (user text 2)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:SDIG:TEXT? 2;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,3,6);
 return strread;
}						  



//----------------------------------------------------------------------------------
char *ReadSDIGenlProdDate (void) {
  // read SDI genlock produktions dato  (user text 3)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:SDIG:TEXT? 3;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}


//----------------------------------------------------------------------------------
char *ReadSDIGenlCalDate (void) {
  // read SDI genlock kalibrerings dato  (user text 4)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:SDIG:TEXT? 4;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}


	







//----------------------------------------------------------------------------------
int InitSDIGenl(void){

int n;
int ValidKU;
char *usertxt;
char *dd;					// MM-DD-YYYY
char dato[10];  			// YYMMDD
unsigned char response[10]; // svar fra PromptPopup
char KUStr[10];  			// KUxxxxxx
char *KUread;		 		// Læst KU nummer
char PromptStr[50];
 

 // 0  producent
 WriteCOMPort(DUTCOM, ":FACT:SDIG:TEXT 0,'PHILIPS';");
 Delay(0.2);

 // 1  12nc
 Fmt(DUTStr,"%s<:FACT:SDIG:TEXT 1,'%s';",NC12[2]);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.5);
 
 
 
  // 2  KU nummer
  KUread = ReadSDIGenlKUNumber();
  if (isdigit(KUread[2]) == FALSE)	
    CopyString(KUread,0,"",0,-1);
    
  do {
   ValidKU = TRUE;
   PromptPopup ("SDI Genlock:   Indtast KU nummer", KUread, response, 6);

   if ((StringLength(response) == 0) & (StringLength(KUread) == 6))  // Only ENTER pressed
    CopyString(response,0,KUread,0,-1);

   for (n = 0; n < 6; n++)
    if (isdigit(response[n]) == FALSE)		  // digits only
     ValidKU = FALSE;
   
  
   if (StringLength(response) != 6)			  // exactly 6 digits
    ValidKU = FALSE;
  } while (ValidKU == FALSE);
  Fmt(DUTStr,"%s<:FACT:SDIG:TEXT 2,'KU%s';",response);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 
 // get system date 
 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // 3  produktionsdato
 usertxt = ReadSDIGenlProdDate();
 if (isdigit(usertxt[2]) == FALSE){			  
  Fmt(DUTStr,"%s<:FACT:SDIG:TEXT 3,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 }

 // 4  kalibreringsdato 
 usertxt = ReadSDIGenlCalDate();
 if (isdigit(usertxt[2]) == FALSE){			  
  Fmt(DUTStr,"%s<:FACT:SDIG:TEXT 4,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 } 

 // 6  PT-nummer
 WriteCOMPort(DUTCOM, ":FACT:SDIG:TEXT 6,'PT8606';");
 Delay(0.2);

 // 7  reserveret - ikke defineret.

return TRUE;
} // InitSDIGenl












//-------------------------------------------------------------------------------
int SDIGenlockTest(void){
int handle_returned, controlID_returned;
double voltmeas, TP1AdjVolt;
char TxtStr[80];
 
 
 if (CheckFrontRearTerminal(FRONTTERM34401) == FALSE) {
  HidePanel(sdigenlPNL);
  return FALSE;
 }
 

 SDIGenlOk = WriteProtect(OFF,FALSE);
 if (SDIGenlOk == FALSE){
  HidePanel (sdigenlPNL);
  return SDIGenlOk;
 }
 


 InitSDIGenl(); 
 
 ErrIEEE = hp34401a_conf (hp34401, 1, VI_ON, 5, 1);

 DisplayPanel (sdigenlPNL);
 ResetTextBox(sdigenlPNL, SDIGEN_PNL_TXTBOX,"");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Forbind PT5210 D1 BLACK til DUT SDI GENLOCK IN\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Forbind DUT SDI GENLOCK OUT til D1 DAC IN\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Barco monitor indstilles til component YUV: Tast 'COMP'\n\n");
 
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Potmeter R6 (VCO ADJ) drejes med uret indtil colorbar\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," forsvinder fra Barco monitor\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Potmeter drejes derefter langsomt mod uret indtil colorbar NETOP\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," ses tydeligt på Barco monitor\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Mål med multimeter HP34401 i TP1\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Tast derefter:  F4: OK     (hold probe på TP1 samtidig med tast på F4)\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX,"                 F9: FEJL");
 
 SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_LED, ATTR_VISIBLE, FALSE);
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX,"");
 SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_VCO, ATTR_MAX_VALUE, 3.0);
 SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_VCO, ATTR_MIN_VALUE, 1.0);
 SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_VCO, ATTR_FILL_COLOR, VAL_GREEN);
 SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_VCO, ATTR_LABEL_TEXT,"1 - 3 V");

 WriteCOMPort(PT5210COM,":OUTP:SB2:SYST SDI625; PATT CBEBU;");
 WriteCOMPort(DUTCOM,":INP:GENL:INP SDI; SYST SDI625;"); 
 SDIGenlOk = TRUE;

 // find VCO-spænding hvor der netop IKKE låses
 done = FALSE;
 while (done == FALSE){
 
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  DisableBreakOnLibraryErrors();
  ErrIEEE = hp34401a_singleMeas (hp34401, &voltmeas);
  EnableBreakOnLibraryErrors();
  SetCtrlVal (sdigenlPNL, SDIGEN_PNL_VCO, voltmeas);
  Delay(0.5);
  ProcessDrawEvents();
 } // while

 if (SDIGenlOk == FALSE) {
  HidePanel(sdigenlPNL);
  return SDIGenlOk;
 }

 if (voltmeas < 1.0){
  SDIGenlOk = FALSE;
  Fmt(TxtStr,"%s< FEJLET: Spænding i TP1 udenfor tolerance (min 1V)\n Målt: %f[p2] V",voltmeas);
  MessagePopup(" SDI Genlock Justering",TxtStr);
  HidePanel(sdigenlPNL);
  return SDIGenlOk;
 }
 
 TP1AdjVolt = voltmeas + 0.25;   // se justeringsvejledning i Gennum databog for GS9005

 // Juster VCO-spænding +250mV over hvor der netop IKKE låses
 Fmt(TxtStr,"%s< Potmeter drejes nu langsomt mod uret til spændingen er %f[p2] ± %f[p2] VDC\n\n",TP1AdjVolt,VCOAdjTol);
 ResetTextBox(sdigenlPNL, SDIGEN_PNL_TXTBOX,"");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Mål med multimeter HP34401 i TP1\n\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX,TxtStr);
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," STATUS skal lyse grønt \n\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX," Tast derefter:  F4: OK     (hold probe på TP1 samtidig med tast på F4)\n");
 SetCtrlVal(sdigenlPNL, SDIGEN_PNL_TXTBOX,"                 F9: FEJL\n");
 Fmt(TxtStr,"%s< %f[p2] ± %f[p2] V",TP1AdjVolt,VCOAdjTol);
 SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_VCO, ATTR_LABEL_TEXT,TxtStr);
 SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_LED, ATTR_VISIBLE, TRUE);

 done = FALSE;
 while (done == FALSE){
 
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  
  DisableBreakOnLibraryErrors();
  ErrIEEE = hp34401a_singleMeas (hp34401, &voltmeas);
  EnableBreakOnLibraryErrors();
  SetCtrlVal (sdigenlPNL, SDIGEN_PNL_VCO, voltmeas);

  if (fabs(voltmeas - TP1AdjVolt) <= VCOAdjTol ) {
    SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_VCO, ATTR_FILL_COLOR, VAL_GREEN);
	SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
   }
   else { 
    SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_VCO, ATTR_FILL_COLOR, VAL_RED);
	SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_LED, ATTR_ON_COLOR, VAL_RED);
  }

  ProcessDrawEvents();
  
  Delay(0.5);
 
 } // while


 if (fabs(voltmeas - TP1AdjVolt) > VCOAdjTol )
  SDIGenlOk = FALSE;
  
 Delay(0.5);

 
 // check SDI genlock signal til SPG
 if (SDIGenlOk == TRUE)
  if (GetGenlockStatus() != DUT_LOCKED){
   SDIGenlOk = FALSE;
   MessagePopup(" SDI Genlock"," FEJLET\n SDI genlock fejlet");
   HidePanel(sdigenlPNL);
   return SDIGenlOk;
  } //if
  
 if (SDIGenlOk == TRUE) {
   SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
   MessagePopup(" SDI Genlock Justering"," OK");
  }
  else {
   SetCtrlAttribute (sdigenlPNL, SDIGEN_PNL_LED, ATTR_ON_COLOR, VAL_RED);
   MessagePopup(" SDI Genlock Justering"," FEJLET");
 }
 
 
 HidePanel(sdigenlPNL);

 return SDIGenlOk;
}

















//-----------------------------------------------------------------------
int SaveInEEPROM(int SDINo, int TxtNo, char Text[]){

 switch (SDINo){
  case 34: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;"); break;		 // SDI BLACK
  case 56: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;"); break;
  case 78: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
  case 1:  Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;"); break;		 // SDI TSG
 }
 WriteCOMPort(DUTCOM, DUTStr);

 Delay(0.1);
 if (SDINo > 1)
   Fmt(DUTStr,"%s<COMM 'HW',%i,'%s';",TxtNo,Text);						 // SDI BLACK
  else
   Fmt(DUTStr,"%s<COMM 'SW',%i,'%s';",TxtNo,Text);						 // SDI TSG
 WriteCOMPort(DUTCOM, DUTStr);									 
 Delay(0.1);

return TRUE;
}









int WriteSDICalDate(int SDINo){
char *dd;					// MM-DD-YYYY
char dato[10];  			// YYMMDD

 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // Send addr for SDI#
 switch (SDINo){
  case 34: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;"); break;		 // SDI BLACK
  case 56: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;"); break;
  case 78: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
  case 1:  Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;"); break;		 // SDI TSG
 }
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.1);
 if (SDINo > 1)
   Fmt(DUTStr,"%s<COMM 'HW',4,'%s';",dato);						 // SDI BLACK
  else
   Fmt(DUTStr,"%s<COMM 'SW',4,'%s';",dato);						 // SDI TSG
 WriteCOMPort(DUTCOM, DUTStr);									 
 
 Delay(0.3);

return 1;
}


char *ReadRLossInEEPROM (int SDINo) {
  // read return loss in EEPROM  (user text 8)
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
  case 34: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',8;"); break;
  case 56: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',8;"); break;
  case 78: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',8;"); break;
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',8;"); break;
  }
  Delay(0.1);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,strlen(strread)-2);	 // "-xx.x"   return -xx.x

 return strread;
}						  


char *ReadJitterInEEPROM (int SDINo) {
  // read return loss in EEPROM  (user text 9)
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
  case 34: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',9;"); break;
  case 56: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',9;"); break;
  case 78: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',9;"); break;
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',9;"); break;
  }
  Delay(0.1);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,strlen(strread)-2);	 // "xx.x"   return xx.x

 return strread;
}						  






char *ReadSDIKUNumber (int SDINo) {
  // read SDI KU nummer  (user text 2)
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
  case 34: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',2;"); break;
  case 56: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',2;"); break;
  case 78: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',2;"); break;
  case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',2;"); break;
  }
  Delay(0.1);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,3,6);	 // "KUxxxxxx"   return xxxxxx

 return strread;
}						  



char *ReadSDIProdDate (int SDINo) {
  // read SDI produktions dato  (user text 3)
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
   case 34: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',3;"); break;
   case 56: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',3;"); break;
   case 78: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',3;"); break;
   case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',3;"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);	 // "xxxxxx"     return xxxxxx

 return strread;
}


char *ReadSDICalDate (int SDINo) {
  // read SDI kalibrerings dato  (user text 4)
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
   case 34: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',4;"); break;
   case 56: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',4;"); break;
   case 78: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',4;"); break;
   case 1: WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',4;"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);	 // "xxxxxx"     return xxxxxx

 return strread;
}







  




//---------------------------------------------------------------------------------
int InitSDI(int SDINo){

int n;
int ValidKU;
char *usertxt;
char *dd;					// MM-DD-YYYY
char dato[10];  			// YYMMDD
unsigned char response[10]; // svar fra PromptPopup
char KUStr[10];  			// KUxxxxxx
char *KUread;
char PromptStr[50];
 
 // Send addr for SDI#
 switch (SDINo){
  case 34: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;"); break;
  case 56: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;"); break;
  case 78: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
  case 1:  Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;"); break;
 }
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.1);

 // 0  producent
 if (SDINo > 1)
   WriteCOMPort(DUTCOM, "COMM 'HW',0,'PHILIPS';");
  else 
   WriteCOMPort(DUTCOM, "COMM 'SW',0,'PHILIPS';");
 Delay(0.3);

 // 1  12nc
 if (SDINo > 1)
   Fmt(DUTStr,"%s<COMM 'HW',1,'%s';",NC12[4]);
  else
   Fmt(DUTStr,"%s<COMM 'SW',1,'%s';",NC12[5]);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.9);
 
 
 
  // 2  KU nummer
  KUread = ReadSDIKUNumber(SDINo);
  if (isdigit(KUread[2]) == FALSE)	
    CopyString(KUread,0,"",0,-1);
    

 if (SDINo == 1)
   Fmt(PromptStr,"%s< SDI TSG:  Indtast KU nummer");
  else
   Fmt(PromptStr,"%s< SDI # %i:  Indtast KU nummer",SDINo);

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

  if (SDINo > 1)
    Fmt(DUTStr,"%s<COMM 'HW',2,'KU%s';",response);
   else 
    Fmt(DUTStr,"%s<COMM 'SW',2,'KU%s';",response);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.3);

 
 
 // get system date 
 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // 3  produktionsdato
 usertxt = ReadSDIProdDate(SDINo);
 if (isdigit(usertxt[0]) == FALSE){
  if (SDINo > 1)
    Fmt(DUTStr,"%s<COMM 'HW',3,'%s';",dato);
   else 
    Fmt(DUTStr,"%s<COMM 'SW',3,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.3);
 }

 // 4  kalibreringsdato 
 usertxt = ReadSDICalDate(SDINo);
 if (isdigit(usertxt[0]) == FALSE){			  
  if (SDINo > 1)
    Fmt(DUTStr,"%s<COMM 'HW',4,'%s';",dato);		 // SDI BLACK
   else 
    Fmt(DUTStr,"%s<COMM 'SW',4,'%s';",dato);		 // SDI TSG
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.3);
 }
 
 
 // 6  PT-nummer
 if (SDINo > 1)
   WriteCOMPort(DUTCOM, "COMM 'HW',6,'PT8609';");
  else 
   WriteCOMPort(DUTCOM, "COMM 'SW',6,'PT8602';");  //<<<<<<<<< PT 8603 < ???? <<<<<<<<<<<
 Delay(0.2);

 // 7  reserveret - ikke defineret.
 // 8  reserveret - return loss.
 // 9  reserveret - jitter time.
 
 
return TRUE;
} // InitSDI




//-------------------------------------------------------------------------
int SDITotalTest(int Item,int Sel){
int SDINo;

 switch (Item){
  case MAINMENU_SDI_BLK34_TOTAL:  SDINo = 34; break;
  case MAINMENU_SDI_BLK56_TOTAL:  SDINo = 56; break;
  case MAINMENU_SDI_BLK78_TOTAL:  SDINo = 78; break;
  case MAINMENU_SDI_TSG_TOTAL:    SDINo = 1; break;
 }

	if (Sel == SDIMENU_TOT_TIMING){
	 if (SDICal(L625,SDINo) == TRUE)
	  SDICal(L525,SDINo);
    }
 		 
 	if ((Sel == SDIMENU_TOT_JITAMSP) || (SDITimingOk == TRUE)){
 	 if (SDIJitterTest(SDINo) == TRUE)
 	  SDIAmplSpecTest(SDINo);
 	} 

 	if ( (Sel == SDIMENU_TOT_RLOSS) || ((SDIAmplSpecOk == TRUE) && (SDIJitterOk == TRUE)) )
 	 SDIReturnLossTest(SDINo);
 							
 	if ((Sel == SDIMENU_TOT_EMBAUDEDH) || (SDIRLossOk == TRUE))
 	 CheckSDIEmbAudio_EDH(SDINo);

 	if ((Sel == SDIMENU_TOT_PATT) || (SDIEmbAudioOk == TRUE))
 	 CheckSDIPattern(SDINo);
			 		
return TRUE;
}















//---------------------------------------------------------------------------------
int CVICALLBACK SDIExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	if (event == EVENT_COMMIT) {
		 SDIJitterOk = FALSE;;
		 SDIAmplSpecOk = FALSE;  
		 SDIRLossOk = FALSE;  
		 SDIGenlOk = FALSE;
		 SDIPattOk = FALSE;
		 SDIEmbAudioOk = FALSE;
		 SDIEDHOk = FALSE;
		 done = TRUE; Cont = TRUE;
	 }
	return 0;
}



int CVICALLBACK SDIOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	if (event == EVENT_COMMIT) {
		 done = TRUE; Cont = TRUE;;
	 }		

	return 0;
}
