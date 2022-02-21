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
#include "pt5230.h"
#include "bbcal.h"
#include "def.h"

/* sditest.c
 PT5210/5230: 		 SDI BLACK (PT8609), SDI TSG(PT8639), SDI TPG(PT8632/33)
 PF 970130
 rettelser:
 980323    PT8639/8632/8633 tilføjet 
 990301    SDI-generatorer: check af CRC ved EDH-test indført
 990615    Pattern test af 900 numre af PT8602/8603 indført
 991012    Reset af SDI generatorer fra menupunkt indført
 991125    Check af om BB-ref modul og SDI-genlock modul er monteret udføres
           efter udførsel af Init-function. Dette muliggør rettelse af KU-nummer
           uden at disse moduler behøver at være monteret.
 991229    PT8602/8603: TEXT ON erstattet af TEXT POS1 fra SWver 3.7
 001011    Tolerance for 4 frames/no LPF jittermåling rettet fra 0.12 til 0.14UI
           Tolerance for 1 line/100kHz LPF jittermåling rettet fra 0.03 til 0.04UI


 
 
 
 SDINo:     SDI TPG = 1,2 eller 5       
            SDI Black = 34, 56 eller 78
            SDI TSG = 22,23 eller 24
 
 TVLines:   L625 eller L525

*/

// constants

const float xd = 0.2;	// omgå fejl i remote system pga overflow i input buffer
						// ellers kræves handshake


const float JT_peakpeakMaxFrame4 = 0.14;    // UI målt på SDI625 75% color bar
											// 4 frames   lp=off
const float JT_peakpeakMaxLine  = 0.04;	    // UI målt på SDI625 75% color bar
											// 1 line     lp=1kHz

const float CheckFreqAmsp[2] = {10,350};	// Frekevns hvor ampl. spec. checkes
const float	AMSP_Max[2] = {+0.75,-12};		// Max level ved amsp check frekvens
const float	AMSP_Min[2] = {-1.50,-40};		// Min level ved amsp check frekvens
// -tol skal være -0.75 med VCA måler ikke korrekt lige efter autokalibrering
//001011 PF

const float CheckRLoss[3] = {20 , 150, 270};// Frekevns hvor return loss. checkes
const float	RLoss_Max[3]  = {-30, -23, -20};// Max level ved checkfrekvensen

const double VCOAdjTol = 0.02;				// ± volt tol ved SDI genlock justering (TP1/R6)

char SDILogFile[2][50] = {"c:\\5230log\\sdical.log","m:\\measdata\\5230\\sdical.log"};
char SDIMeasLogFile[2][50] = {"c:\\5230log\\sdimeas.log","m:\\measdata\\5230\\sdimeas.log"};

int	 	bytes_written,
	 	fhandle;

char	SDIKUStr[20],
	    wrtbuf[80],
        *d,				// MM-DD-YYYY
	    *tt;			// HH:MM:SS

float	jitterMeanFrame4,
		jitterMeanLine;
float	LevelmV;


// variables
int 
	jitPNL,
	sdiPNL,
	pattPNL,
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
	VCAOk;

int	XX1_Stik;

unsigned int SDICalphase[2];


// prototyping ----------------------------------------------------------------
int InitSDI(int SDINo, int xx_stik);
int WriteSDICalDate(int BBNo, int xx_stik);
int SaveInEEPROM(int SDINo, int TxtNo, char Text[], int xx_stik);
int SDIGenlockTest(void);
char *ReadSDIKUNumber (int SDINo, int xx_stik);
char *ReadSDIGenlKUNumber (void);
char *ReadSDIGenlProdDate (void);
char *ReadSDIGenlCalDate (void);
int SDIReturnLossMeasurement(void);
char *ReadSDITPGHwVer (int SDINo, int xx_stik);
char *ReadSDITPGSwVer (int SDINo, int xx_stik);
int ResetSDI(int sdino);
int SetupPT5211(void);




//------------------------------------------------------------------------------
int GetTPGXX1Connection(void){
// Beder brugeren om at returnere stikforbindelsen fra PT8633/PT8631 til 
// oscillator board. TPG'en er forbundet enten til XA1 eller XB1.
// Variablen XX1_Stik opdateres
// return = selected button
 int selected_button;

 selected_button = GenericMessagePopup ("SDI TPG",
										"Til hvilket stik er generatoren forbundet?",
										"XA1", "XB1", "Afbryd",
										0, 0, 0,
										VAL_GENERIC_POPUP_BTN1,
										VAL_GENERIC_POPUP_BTN1,
										VAL_GENERIC_POPUP_BTN3);
 if (selected_button == 1)
   XX1_Stik = XA1_Stik;
  else
 if (selected_button == 2)
   XX1_Stik = XB1_Stik;
  else
 if (selected_button == 3)
   XX1_Stik = 0;

  return selected_button;
} // GetTPGXX1Connection 






/*--------------------------------------------------------------------------------
Fejl i VCA: Når delay øges med 1 sample (=37ns) ad gangen, vises kun hver anden 
sample på VCA'en, hvorimod når delay mindskes med 1 sample vises hver sample. 
For at undgå dennne fejlvisning gennemføres timing-kalibreringen i 2 step:
 1) kalibrering til delay = +2 samples (74ns)
 2) kalibrering til delay = 0. (efterfulgt af gemning i EEPROM)

*/

int SDICal(int TVLines, int SDINo, int GetXX1){
// GetXX1 = NO         XX1_Stik opdateres ikke af brugeren
// GetXX1 = YES        XX1_Stik opdateres af brugeren
char 	TxtStr[100],
		resp[20],
		*SDIGenlKU,
		*strread,
		*HBread,
		HWVer[20],
		SWVer[20];
		
int 	handle_returned, 
		controlID_returned,
	 	bytes_read,
		k,
		NegDelay;
unsigned int SDIphase,
			 phase_read,
			 MaxPhase;
float sdidelay;
double dd;



// SDINo:     SDI TPG = 1,2 eller 5       
//            SDI Black = 34, 56 eller 78
//            SDI TSG = 22,23 eller 24
// TVLines:   L625 eller L525

 if ((SDINo == SDIBLACK34) || (SDINo == SDIBLACK56) || (SDINo == SDIBLACK78))
   Fmt(TxtStr,"%s< SDI Black %i   (SDI %i) ",SDINo,TVLines);
  else
 if ((SDINo == SDITSG2) || (SDINo == SDITSG3) || (SDINo == SDITSG4))
   Fmt(TxtStr,"%s< SDI Test Signal Generator #%i (SDI%i) ",SDINo-20,TVLines);
  else 
 if ((SDINo == SDITPG1) || (SDINo == SDITPG2) || (SDINo == SDITPG5))
   Fmt(TxtStr,"%s< SDI Test Pattern Generator #%i (SDI%i) ",SDINo,TVLines);
 SetPanelAttribute (sdiPNL, ATTR_TITLE, TxtStr);
 
 SetCtrlAttribute (sdiPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);
 DisplayPanel (sdiPNL);
 ResetTextBox(sdiPNL, SDI_PNL_TXTBOX,"");
 SetCtrlVal (sdiPNL, SDI_PNL_DELAY, "");
 ProcessDrawEvents();


 SDITimingOk = WriteProtect(OFF,FALSE);
 if (SDITimingOk == FALSE){
  HidePanel (sdiPNL);
  return SDITimingOk;
 }
 


 if (GetXX1 == TRUE)
  if (GetTPGXX1Connection() == 3){
   HidePanel (sdiPNL);
   SDITimingOk = FALSE;
   return FALSE;
  }
 
 if (TVLines == L625){
  if (InitSDI(SDINo,XX1_Stik) == FALSE){
   HidePanel (sdiPNL);
   SDITimingOk = FALSE;
   return FALSE;
  } 
 
  Delay(0.5);
 

  // Check at SDI genlock er monteret
  SDIGenlKU = ReadSDIGenlKUNumber();
  if (strlen(SDIGenlKU) != 6){
   MessagePopup(" SDI Kalibrering"," PT8606 SDI Genlock option er ikke monteret !\n Monter option og start igen");
   HidePanel(sdiPNL);
   return FALSE;
  }
 } // if TVLines..


 
 if (TVLines == L625){
  VCAOk = CheckReadyVCA();

  if (VCAOk == FALSE){
   HidePanel(sdiPNL);
   SDITimingOk = FALSE;
   return FALSE;
  }
 }
 
 vca_signaldelay();
 
 if (TVLines == L625){ 
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind PT5210 D1 5-6 til DUT SDI GENLOCK IN\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind DUT SDI GENLOCK OUT til R&S VCA A-in\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n");
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n F4: OK\n F9: STOP\n");
  WaitForContinue();
 }

 
 if (SDITimingOk == FALSE){
  MessagePopup (" SDI Timing Kalibrering", " AFBRUDT");
  HidePanel(sdiPNL);
  return FALSE;
 }
 

 
 if (TVLines == L525)
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n F9: STOP\n");

 SetCtrlAttribute (sdiPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, FALSE);
 
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n Kalibrerer *");

// indstil PT5210 i test rack og genlock i DUT
 if (TVLines == L625) {
  WriteCOMPort(PT5210COM,":OUTP:SB56:SYST SDI625; PATT BLACK; "); 
  Delay(0.1);
  WriteCOMPort(PT5210COM,":OUTP:SB34:SYST SDI625; PATT BLACK; "); 
  WriteCOMPort(DUTCOM,":INP:GENL:INP SDI;"); Delay(xd);
  WriteCOMPort(DUTCOM,"SYST SDI625;"); Delay(xd);
  WriteCOMPort(DUTCOM,"DEL 0,0,0;");  Delay(xd);
  vca_selectsystem(L625);
 }
 else {
  WriteCOMPort(PT5210COM,":OUTP:SB56:SYST SDI525; PATT BLACK; "); 
  Delay(0.1);
  WriteCOMPort(PT5210COM,":OUTP:SB34:SYST SDI525; PATT BLACK; "); 
  WriteCOMPort(DUTCOM,":INP:GENL:INP SDI;"); Delay(xd);
  WriteCOMPort(DUTCOM,"SYST SDI525;"); Delay(xd);
  WriteCOMPort(DUTCOM,"DEL 0,0,0;");  Delay(xd);
  vca_selectsystem(L525);
 }

 Delay(0.2);
 

  
 
 switch (SDINo) {
  case SDIBLACK34: if (TVLines == L625)			   // 8609
  			 WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  			 WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		    Delay(0.05);
           WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;");
		   break;

  case SDIBLACK56: if (TVLines == L625)			   // 8609
  			 WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		    Delay(0.05);
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;");
  		   break;

  case SDIBLACK78: if (TVLines == L625)			   // 8609
  		  	 WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		    Delay(0.05);
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;");
  		   break;

  case SDITSG2: if (TVLines == L625)			   // 8639
  			 WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  			 WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		    Delay(0.05);
           WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;");     
		   break;
  case SDITSG3: if (TVLines == L625)			   // 8639
  			 WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  			 WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		    Delay(0.05);
           WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;");     
		   break;
  case SDITSG4: if (TVLines == L625)			   // 8639
  			     WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			    else
  			     WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		        Delay(0.05);
                WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;");     
		        break;


  case SDITPG1: 
            if (AppType == PT5210){
              if (TVLines == L625)
  		  	   WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			  else
  		       WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		      Delay(0.05);
  		      WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;");		// SDI TPG1 = 8602/03
  		    } 
  		    
            if (AppType == PT5230){
              if (TVLines == L625)
  		  	   WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			  else
  		       WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		      Delay(0.05);
  		      WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;");		// SDI TPG1 = 8632
  		    } 
  		    break;
  		    
  case SDITPG2:
            if (TVLines == L625)
  		  	 WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		    Delay(0.05);
  		    if (XX1_Stik == XA1_Stik) 
  		      WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;");	// SDI TPG2 = 8633
  		     else
  		      WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;");
  		   break;   
  
  case SDITPG5:  if (TVLines == L625)
  		  	 WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
		    Delay(0.05);
  		    if (XX1_Stik == XA1_Stik) 
  		      WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;");	// SDI TPG2 = 8633
  		     else
  		      WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;");
  		   break;   
  }
 Delay(0.1);

 // Reset SDI black & TSG  8609/8639
 if ((TVLines == L625) && (SDINo >= SDITSG2)){		 // SDI BLACK & TSG 625
  WriteCOMPort(DUTCOM, "COMM 'HG',0;");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'HX$';");
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'HO',0;");
  Delay(0.05);
  MaxPhase = 276480000;		// 625*1728*256
 } 
 else
 if ((TVLines == L525) && (SDINo >= SDITSG2)){		 // SDI BLACK & TSG 525
  WriteCOMPort(DUTCOM, "COMM 'HM',0;");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'HY$';");
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'HO',0;");
  Delay(0.05);
  MaxPhase = 230630400;		// 525*1716*256
 } 
 else
 // Reset SDI TPG  8602/03/32
 if ((TVLines == L625) && (SDINo <= SDITPG5)){ //    SDI TPG  625
  WriteCOMPort(DUTCOM, "COMM 'SG',0;");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'SX$';");
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'SO',0;");
  Delay(0.05);
  MaxPhase = 276480000;		// 625*1728*256
 } 
 else
 if ((TVLines == L525) && (SDINo <= SDITPG5)){ //    SDI TPG  525
  WriteCOMPort(DUTCOM, "COMM 'SM',0;");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'SY$';");
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'SO',0;");
  Delay(0.05);
  MaxPhase = 230630400;		// 525*1716*256
 } 


 done = FALSE;
 SDIphase = 0;

 if ((TVLines == L625) && (SDINo >= SDITSG2))  // 8609/39
   Fmt(DUTStr,"%s<COMM 'HG',%i;",SDIphase);
 else  
 if ((TVLines == L525) && (SDINo >= SDITSG2))
   Fmt(DUTStr,"%s<COMM 'HM',%i;",SDIphase);
 else  
 if ((TVLines == L625) && (SDINo <= SDITPG5))  // 8602/03/32/33
   Fmt(DUTStr,"%s<COMM 'SG',%i;",SDIphase);
 else  
 if ((TVLines == L525) && (SDINo <= SDITPG5))
   Fmt(DUTStr,"%s<COMM 'SM',%i;",SDIphase);
 
 WriteCOMPort(DUTCOM, DUTStr);
 SetCtrlVal (sdiPNL, SDI_PNL_DELAY, DUTStr);

 Delay(3);
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," *");
 bytes_read = vca_signaldelay_readdelay(&sdidelay);
 Delay(0.5);

 SDITimingOk = FALSE;
 

 
 
 do {
   bytes_read = vca_signaldelay_readdelay(&sdidelay);
  
   if ((sdidelay > 72e-9) && (sdidelay < 76e-9) && (bytes_read > 0))
     SDITimingOk = TRUE;
    else
     SDITimingOk = FALSE;

  

   dd = (sdidelay * 27.0e6) - 2;   // -2 for at ramme et delay = 74 ns (2 samples)
  
   if (dd < 0){		// korrekt afrunding
     dd = dd - 0.5;
     NegDelay = TRUE;
     }
    else{
     dd = dd + 0.5;
     NegDelay = FALSE;
     }
  
   k = (int) dd;
   k = k  * 256;

  
   if (abs(k) > MaxPhase)
    k = 0;
   if (k > 0)
    SDIphase = MaxPhase - k;
   if (k < 0)
    SDIphase = abs(k);
   
  

   if ((TVLines == L625) && (SDINo >= SDITSG2)) // 8609/39
     Fmt(DUTStr,"%s<COMM 'HG',%i;",SDIphase);
    else  
   if ((TVLines == L525) && (SDINo >= SDITSG2))
     Fmt(DUTStr,"%s<COMM 'HM',%i;",SDIphase);
    else  
   if ((TVLines == L625) && (SDINo <= SDITPG5)) // 8602/03/32/33
     Fmt(DUTStr,"%s<COMM 'SG',%i;",SDIphase);
    else  
   if ((TVLines == L525) && (SDINo <= SDITPG5))
     Fmt(DUTStr,"%s<COMM 'SM',%i;",SDIphase);
 
   WriteCOMPort(DUTCOM, DUTStr);
   Fmt(TxtStr,"%s< %f[b4p3] µS   (%s)",sdidelay*1e6,DUTStr);
   SetCtrlVal (sdiPNL, SDI_PNL_DELAY, TxtStr);

   Delay(3);
 

   if (SDITimingOk == FALSE)
    SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," *");
  
  
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 
 } while ((SDITimingOk == FALSE) && (done == FALSE));

 Delay(3);




// ---------- step delay -2 samples ------------------------------------------ 
 do {
   bytes_read = vca_signaldelay_readdelay(&sdidelay);
  
   if ((sdidelay == 0) && (bytes_read > 0))
   	 SDITimingOk = TRUE;
   	else
   	 SDITimingOk = FALSE;
    

   Fmt(TxtStr,"%s< %f[b4p3] µS    (%s)",sdidelay*1e6,DUTStr);
   SetCtrlVal (sdiPNL, SDI_PNL_DELAY, TxtStr);


   if ((sdidelay > 36e-9) && (sdidelay < 38e-9))
    SDIphase = SDIphase - 256;
   if ((sdidelay > 72e-9) && (sdidelay < 76e-9))
    SDIphase = SDIphase - 512;
   if (sdidelay < 0)
    SDIphase = SDIphase + 512;
   
   
   if ((TVLines == L625) && (SDINo >= SDITSG2)) // 8609/39
     Fmt(DUTStr,"%s<COMM 'HG',%i;",SDIphase);
    else  
   if ((TVLines == L525) && (SDINo >= SDITSG2))
     Fmt(DUTStr,"%s<COMM 'HM',%i;",SDIphase);
    else  
   if ((TVLines == L625) && (SDINo <= SDITPG5)) // 8602/03/32/33
     Fmt(DUTStr,"%s<COMM 'SG',%i;",SDIphase);
    else  
   if ((TVLines == L525) && (SDINo <= SDITPG5))
    Fmt(DUTStr,"%s<COMM 'SM',%i;",SDIphase);

   WriteCOMPort(DUTCOM, DUTStr);
   SetCtrlVal (sdiPNL, SDI_PNL_DELAY, DUTStr);

   Delay(3);
 

   if (SDITimingOk == FALSE)
    SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," *");
  
  
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 
 } while ((SDITimingOk == FALSE) && (done == FALSE));





 
 
 
 
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n");
 
 // gem data i EEPROM
 Delay(0.1);
 if (SDITimingOk == TRUE) {
  if ((TVLines == L625) && (SDINo >= SDITSG2))
    WriteCOMPort(DUTCOM,"COMM 'HX$';");
   else
  if ((TVLines == L525) && (SDINo >= SDITSG2))
    WriteCOMPort(DUTCOM,"COMM 'HY$';");
   else
  if ((TVLines == L625) && (SDINo <= SDITPG5))
    WriteCOMPort(DUTCOM,"COMM 'SX$';");
   else
  if ((TVLines == L525) && (SDINo <= SDITPG5))
    WriteCOMPort(DUTCOM,"COMM 'SY$';");
 
  Delay(0.2);

  // check at data er gemt korrekt i EEPROM
  FlushInQ(DUTCOM);
  if ((TVLines == L625) && (SDINo >= SDITSG2))
    WriteCOMPort(DUTCOM,"COMM 'HX?';");
  else
  if ((TVLines == L525) && (SDINo >= SDITSG2))
    WriteCOMPort(DUTCOM,"COMM 'HY?';");
  else
  if ((TVLines == L625) && (SDINo <= SDITPG5))
    WriteCOMPort(DUTCOM,"COMM 'SX?';");
  else
  if ((TVLines == L525) && (SDINo <= SDITPG5))
    WriteCOMPort(DUTCOM,"COMM 'SY?';");
    
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&phase_read);
  if (SDIphase != phase_read)
   SDITimingOk = FALSE;
 } //if
 
 
  if (SDITimingOk == TRUE) {
   
    if (TVLines == L625)
      SDICalphase[0] = phase_read;   // cal data from L625
     else
      SDICalphase[1] = phase_read;   // cal data from L525
 
    if (TVLines == L525){			 // kalibrering afsluttes efter SDI525 kalibrering
    
      // get system date 
      d = DateStr();
      CopyString(dato,0,d,8,2);
      CopyString(dato,strlen(dato),d,0,2);
      CopyString(dato,strlen(dato),d,3,2);
 
      // get system time
      tt = TimeStr();
  
	  strread = ReadSDIKUNumber(SDINo,XX1_Stik);
	  CopyString(SDIKUStr,0,strread,0,-1);
	  
      // check om logfil findes, ellers opret en ny logfil
      fhandle = OpenFile (SDILogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   
      // SDI cal data gemmes i logfil

	  if (SDINo <= SDITPG5){
       strread = ReadSDITPGHwVer (SDINo,XX1_Stik);
 	   CopyString(HWVer,0,strread,0,-1);
       strread = ReadSDITPGSwVer (SDINo,XX1_Stik);
	   CopyString(SWVer,0,strread,0,-1);
       Fmt(wrtbuf,"%s<%s,%s,KU%s,%i,%i,%i,SW=%s,HW=%s,%i\n",dato,tt,SDIKUStr,SDINo,SDICalphase[0],SDICalphase[1],SWVer,HWVer,AppType);	
      }
	  if (SDINo >= SDITSG2){
       strread = ReadSDISWVer(SDINo);
	   CopyString(SWVer,0,strread,0,-1);
       Fmt(wrtbuf,"%s<%s,%s,KU%s,%i,%i,%i,SW=%s,%i\n",dato,tt,SDIKUStr,SDINo,SDICalphase[0],SDICalphase[1],SWVer,AppType);
      }
      bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
      CloseFile(fhandle);
    

     MessagePopup("SDI Delay Kalibrering"," OK\n Kalibreringsdata gemt i EEPROM og logfil");
    } //if (TVLines == L525)
    
    WriteSDICalDate(SDINo,XX1_Stik);
  }
  else {
     switch (SDINo){
      case SDIBLACK34:
      case SDITSG3:     Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;COMM 'HW',4,'000000';"); break;
      case SDIBLACK56:
      case SDITSG4:     Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;COMM 'HW',4,'000000';"); break;
      case SDIBLACK78:
      case SDITSG2:     Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;COMM 'HW',4,'000000';"); break;
     
      case SDITPG1:     if (AppType == PT5210)
                          Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;COMM 'SW',4,'000000';");
                         else
                          Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIT;COMM 'SW',4,'000000';");break;
      case SDITPG2:
      case SDITPG5:     if (AppType == PT5230)
      				     if (XX1_Stik == XA1_Stik)
                           Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;COMM 'SW',4,'000000';");
                          else
                           Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;COMM 'SW',4,'000000';");break;
     } // switch
    WriteCOMPort(DUTCOM, DUTStr);
    MessagePopup("SDI Delay Kalibrering"," FEJLET\n Kalibreringsdata ikke gemt i EEPROM");
  }//if (SDITimingOk == TRUE)

 // Indstil genlock til intern, således at 'config error' undgås når PT8606 fjernes efter test
 WriteCOMPort(DUTCOM,":INP:GENL:INP INT;"); Delay(xd);

HidePanel(sdiPNL);
return SDITimingOk;
} //SDICal






//----------------------------------------------------------------------------------
// kalibrering af SDI TSG 900 versioner
int SDI900Cal(int TVLines, int SDINo, int GetXX1){
char 	TxtStr[100],
		resp[20],
		wrtbuf[20],
		*SDIGenlKU,
		*strread,
		*HBread,
		HWVer[20],
		SWVer[20];
		
int 	handle_returned, 
		controlID_returned,
	 	bytes_read,
		k,
		NegDelay,
		No900;
unsigned int SDIphase,
			 phase_read,
			 MaxPhase;
float sdidelay;
double dd;




// SDINo:     SDI TSG = 1       SDI Black = 34, 56 eller 78
// TVLines:   L625 eller L525

 Fmt(TxtStr,"%s< SDI Black %i   (SDI %i) ",SDINo,TVLines);
 SetPanelAttribute (sdiPNL, ATTR_TITLE, " SDI TSG  900 serie");
 SetCtrlAttribute (sdiPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);
 DisplayPanel (sdiPNL);
 ResetTextBox(sdiPNL, SDI_PNL_TXTBOX,"");
 SetCtrlVal (sdiPNL, SDI_PNL_DELAY, "");
 ProcessDrawEvents();


 SDITimingOk = WriteProtect(OFF,FALSE);
 if (SDITimingOk == FALSE){
  HidePanel (sdiPNL);
  return SDITimingOk;
 }



  // Check at SDI genlock er monteret
 SDIGenlKU = ReadSDIGenlKUNumber();
 if (strlen(SDIGenlKU) != 6){
  MessagePopup(" SDI Kalibrering"," PT8606 SDI Genlock option er ikke monteret !\n Monter option og start igen");
  HidePanel(sdiPNL);
  return FALSE;
 }
 
 if (InitSDI(SDINo,0) == FALSE){
  HidePanel (sdiPNL);
  SDITimingOk = FALSE;
  return FALSE;
 } 

 
 strread = ReadSDITPGHwVer(SDINo,0);
 Scan(strread,"%s>%i",&PT86No);
 No900 = 900 + (PT86No / 256);

 PT86No = PT86No & 0x000000ff;   // test kun på bit 0-7, clear 8-31

 if (PT86No == 2)
   Fmt(TxtStr,"%s< SDI TSG  PT8602/%i    (SDI %i)",No900,TVLines);
  else
   Fmt(TxtStr,"%s< SDI TSG  PT8603/%i    (SDI %i)",No900,TVLines);
 SetPanelAttribute (sdiPNL, ATTR_TITLE, TxtStr);
 

 // 900 version skrives til usertext #6 i EEPROM
 if (PT86No == 2)
   Fmt(wrtbuf,"%s<COMM 'SW',6,'PT8602/%i';",No900);
 else 
 if (PT86No == 3)
   Fmt(wrtbuf,"%s<COMM 'SW',6,'PT8603/%i';",No900);
 else 
   Fmt(wrtbuf,"%s<COMM 'SW',6,'PT8603';");
 
 WriteCOMPort(DUTCOM, wrtbuf); 
 
 
 
 
 
 Delay(0.5);
 
 VCAOk = CheckReadyVCA();

 if (VCAOk == FALSE){
  HidePanel(sdiPNL);
  SDITimingOk = FALSE;
  return FALSE;
 }
 
 vca_signaldelay();
 
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind PT5210 D1 5-6 til DUT SDI GENLOCK IN\n");
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind DUT SDI GENLOCK OUT til R&S VCA A-in\n");
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n");
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n F4: OK\n F9: STOP\n");
 WaitForContinue();

 
 if (SDITimingOk == FALSE){
  MessagePopup (" SDI Timing Kalibrering", " AFBRUDT");
  HidePanel(sdiPNL);
  return FALSE;
 }
 


 SetCtrlAttribute (sdiPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, FALSE);
 
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX,"\n Kalibrerer *");


 if (TVLines == L625) {
  WriteCOMPort(PT5210COM,":OUTP:SB56:SYST SDI625; PATT BLACK; "); 
  Delay(0.1);
  WriteCOMPort(PT5210COM,":OUTP:SB34:SYST SDI625; PATT BLACK; "); 
  WriteCOMPort(DUTCOM,":INP:GENL:INP SDI; SYST SDI625; DEL 0,0,0;"); //<<<<<<<<<<<<<<
  vca_selectsystem(L625);
 }
 else {
  WriteCOMPort(PT5210COM,":OUTP:SB56:SYST SDI525; PATT BLACK; "); 
  Delay(0.1);
  WriteCOMPort(PT5210COM,":OUTP:SB34:SYST SDI525; PATT BLACK; "); 
  WriteCOMPort(DUTCOM,":INP:GENL:INP SDI; SYST SDI525; DEL 0,0,0;"); //<<<<<<<<<<<<<
  vca_selectsystem(L525);
 }

 Delay(0.2);
 

WriteCOMPort(DUTCOM,":FACT:V24C:ADDR ANLS;");
WriteCOMPort(DUTCOM, "COMM 'AP',17;");
 
 
 switch (SDINo) {
  case 34: if (TVLines == L625)
  			 WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  			 WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;");     // SDI black
		   break;

  case 56: if (TVLines == L625)
  			 WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;"); 
  		   break;

  case 78: if (TVLines == L625)
  		  	 WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625; PATT CBEBU; DEL 0,0,0;");
  			else
  		     WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI525; PATT CBSMPTE; DEL 0,0,0;");
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;");
  		   break;

  case 1:  if (TVLines == L625){ // set system + pattern=black
			 WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;"); Delay(0.05);
			 WriteCOMPort(DUTCOM,"COMM 'SG',0;"); Delay(0.05);
			 WriteCOMPort(DUTCOM,"COMM 'SP',9;"); Delay(0.05);
			 }
  			else{
			 WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;"); Delay(0.05);
			 WriteCOMPort(DUTCOM,"COMM 'SM',0;"); Delay(0.05);
			 WriteCOMPort(DUTCOM,"COMM 'SP',9;"); Delay(0.05);
			} 
  		   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;");		// SDT TSG
  		   break;   
  }
 Delay(0.2);

 // Reset SDI black
 if ((TVLines == L625) && (SDINo > 1)){		 // SDI BLACK  625
  WriteCOMPort(DUTCOM, "COMM 'HG',0;");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'HX$';");
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'HO',0;");
  Delay(0.05);
  MaxPhase = 276480000;		// 625*1728*256
 } 
 else
 if ((TVLines == L525) && (SDINo > 1)){		 // SDI BLACK  525
  WriteCOMPort(DUTCOM, "COMM 'HM',0;");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'HY$';");
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'HO',0;");
  Delay(0.05);
  MaxPhase = 230630400;		// 525*1716*256
 } 
 else
 if ((TVLines == L625) && (SDINo == 1)){		 // SDI TSG  625
  WriteCOMPort(DUTCOM, "COMM 'SG',0;");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'SX$';");
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'SO',0;");
  Delay(0.05);
  MaxPhase = 276480000;		// 625*1728*256
 } 
 else
 if ((TVLines == L525) && (SDINo == 1)){		 // SDI TSG  525
  WriteCOMPort(DUTCOM, "COMM 'SM',0;");
  Delay(0.05);
  WriteCOMPort(DUTCOM, "COMM 'SY$';");
  Delay(0.1);
  WriteCOMPort(DUTCOM, "COMM 'SO',0;");
  Delay(0.05);
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
 SetCtrlVal (sdiPNL, SDI_PNL_DELAY, DUTStr);

 Delay(3);
 SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," *");
 bytes_read = vca_signaldelay_readdelay(&sdidelay);
 Delay(0.5);

 SDITimingOk = FALSE;
 
 do {
  bytes_read = vca_signaldelay_readdelay(&sdidelay);
  
   if ((sdidelay > 72e-9) && (sdidelay < 76e-9) && (bytes_read > 0))
     SDITimingOk = TRUE;
    else
     SDITimingOk = FALSE;

  

  dd = (sdidelay * 27.0e6) - 2;   // -2 for at ramme et delay = 74 ns (2 samples)
  
  if (dd < 0){		// korrekt afrunding
    dd = dd - 0.5;
    NegDelay = TRUE;
    }
   else{
    dd = dd + 0.5;
    NegDelay = FALSE;
    }
  
  k = (int) dd;
  k = k  * 256;

  
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
  Fmt(TxtStr,"%s< %f[b4p3] µS   (%s)",sdidelay*1e6,DUTStr);
  SetCtrlVal (sdiPNL, SDI_PNL_DELAY, TxtStr);

  Delay(3);
 

 if (SDITimingOk == FALSE)
  SetCtrlVal(sdiPNL, SDI_PNL_TXTBOX," *");
  
  
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 
 } while ((SDITimingOk == FALSE) && (done == FALSE));

 Delay(3);




// ---------- step delay -2 samples ------------------------------------------ 
 do {
  bytes_read = vca_signaldelay_readdelay(&sdidelay);
  
   if ((sdidelay == 0) && (bytes_read > 0))
   	 SDITimingOk = TRUE;
   	else
   	 SDITimingOk = FALSE;
    

  Fmt(TxtStr,"%s< %f[b4p3] µS    (%s)",sdidelay*1e6,DUTStr);
  SetCtrlVal (sdiPNL, SDI_PNL_DELAY, TxtStr);


  if ((sdidelay > 36e-9) && (sdidelay < 38e-9))
   SDIphase = SDIphase - 256;
  if ((sdidelay > 72e-9) && (sdidelay < 76e-9))
   SDIphase = SDIphase - 512;
  if (sdidelay < 0)
   SDIphase = SDIphase + 512;
   
   
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
  SetCtrlVal (sdiPNL, SDI_PNL_DELAY, DUTStr);

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
 
  Delay(0.2);

  // check at data er gemt korrekt i EEPROM
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
   
    if (TVLines == L625)
      SDICalphase[0] = phase_read;
     else
      SDICalphase[1] = phase_read;
 
      // get system date 
      d = DateStr();
      CopyString(dato,0,d,8,2);
      CopyString(dato,strlen(dato),d,0,2);
      CopyString(dato,strlen(dato),d,3,2);
 
      // get system time
      tt = TimeStr();
  
	  strread = ReadSDIKUNumber(SDINo,0);
	  CopyString(SDIKUStr,0,strread,0,-1);
	  
      // check om logfil findes, ellers opret en ny logfil
      fhandle = OpenFile (SDILogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   
      // SDI cal data gemmes i logfil

	  if (SDINo == 1){
       strread = ReadSDITPGHwVer (SDINo,0);
 	   CopyString(HWVer,0,strread,0,-1);
       strread = ReadSDITPGSwVer (SDINo,0);
	   CopyString(SWVer,0,strread,0,-1);
	   if (TVLines == L625)
         Fmt(wrtbuf,"%s<%s,%s,KU%s,%i,%i,SW=%s,HW=%i\n",dato,tt,SDIKUStr,SDINo,SDICalphase[0],SWVer,No900);
        else
         Fmt(wrtbuf,"%s<%s,%s,KU%s,%i,%i,SW=%s,HW=%i\n",dato,tt,SDIKUStr,SDINo,SDICalphase[1],SWVer,No900);
      }
	  if (SDINo != 1){
       strread = ReadSDISWVer(SDINo);
	   CopyString(SWVer,0,strread,0,-1);
	   if (TVLines == L625)
         Fmt(wrtbuf,"%s<%s,%s,KU%s,%i,%i,SW=%s,%i\n",dato,tt,SDIKUStr,SDINo,SDICalphase[0],SWVer,AppType);	
        else 
         Fmt(wrtbuf,"%s<%s,%s,KU%s,%i,%i,SW=%s,%i\n",dato,tt,SDIKUStr,SDINo,SDICalphase[1],SWVer,AppType);	
      }
      bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
      CloseFile(fhandle);
    

     MessagePopup("SDI Delay Kalibrering"," OK\n Kalibreringsdata gemt i EEPROM og logfil");
    
    WriteSDICalDate(SDINo,0);
  }
  else {
    switch (SDINo){
     case 34: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;COMM 'HW',4,'000000';"); break;
     case 56: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;COMM 'HW',4,'000000';"); break;
     case 78: Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;COMM 'HW',4,'000000';"); break;
     case 1:  Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;COMM 'SW',4,'000000';"); break;
    } // switch
  WriteCOMPort(DUTCOM, DUTStr);
  MessagePopup("SDI Delay Kalibrering"," FEJLET\n Kalibreringsdata ikke gemt i EEPROM");
 }//if (SDITimingOk == TRUE)

HidePanel(sdiPNL);
return SDITimingOk;
} //SDI900Cal













//-------------------------------------------------------------------------------
int CheckSDIPattern(int SDINo, int GetXX1){
double PatternShiftDelay;
int handle_returned,
    controlID_returned,
    PT86No,
    No900;
char *usertxt, TxtStr[100];

// SDINo:     SDI TPG = 1,2,5       
//            SDI Black = 34,56,78
//            SDI TSG = 2,3,4
// PT86No	  2,3,32,33  (kun for SDI TPG'er)
// TVLines:   L625 eller L525

// GetXX1 bruges ikke her

 No900 = 0;
 SDIPattOk = TRUE;
 PatternShiftDelay = 3.0;

 ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");

 SDIPattOk = WriteProtect(OFF,FALSE);
 if (SDIPattOk == FALSE){
  HidePanel (pattPNL);
  return SDIPattOk;
 }
 Delay(0.1);
 
 // read PT86xx number
 if (SDINo <= SDITPG5){
  usertxt = ReadSDITPGHwVer(SDINo,XX1_Stik);
  Scan(usertxt,"%s>%i",&PT86No);
 }
 
 if (PT86No > 255){			   // 900 nummer
  No900 = (PT86No >> 8) + 900;
  PT86No = PT86No & 0x00FF;
 }

// init panel
 if (SDINo >= SDIBLACK34)
   Fmt(TxtStr,"%s< Pattern test:  SDI Black %i ",SDINo);
 else
 if (SDINo <= SDITPG5){
   switch (PT86No){
    case 2:  Fmt(TxtStr,"%s< Pattern test:  SDI TPG %i  (PT8602) ",SDINo); break;
    case 3:  Fmt(TxtStr,"%s< Pattern test:  SDI TPG %i  (PT8603) ",SDINo); break;
    case 32: Fmt(TxtStr,"%s< Pattern test:  SDI TPG %i  (PT8632) ",SDINo); break;
    case 33: Fmt(TxtStr,"%s< Pattern test:  SDI TPG %i  (PT8633) ",SDINo); break;
   } // switch 
 } // if (SDINo <= SDITPG5)
 else
   Fmt(TxtStr,"%s< Pattern test:  SDI TSG %i  (PT8639) ",SDINo-20);

 if (No900 > 255){			   // 900 nummer
  Fmt(TxtStr,"%s[a]<  (%i) ",No900);
 }
 
 
 SetPanelAttribute (pattPNL, ATTR_TITLE, TxtStr);


 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind SDI generator til D1 DAC IN\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Barco monitor indstilles til component YUV: Tast 'COMP'\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," På monitor vises derefter følgende pattern i rækkefølge:\n");


 if (SDINo >= SDIBLACK34){	//8609
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 625:   EBU Colorbar    100% Colorbar    Black\n");   
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 525:   SMPTE Colorbar  Black\n");
 }
 
 if ((SDINo >= SDITSG2) && (SDINo <= SDITSG4)){		// 8639
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 625:   EBU Colorbar    Colorbar+Red     Pluge\n");   
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 525:   SMPTE Colorbar  Black            Window 100%\n\n");
 }

 if ((SDINo == SDITPG1) && (PT86No == 2)){													 // 8602
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 625:   EBU Colorbar     Colorbar+Red    Shallow Ramp\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 525:   SMPTE Colorbar   SDI Timing      SDI Check Field\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Teksten 'TOMMY' og 'DENNIS' vises i nogle pattern:\n\n");
 }
 
 if ((SDINo == SDITPG1) && (PT86No == 3)){													 // 8603
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 625:   EBU Colorbar     Colorbar+Red    Multi Burst\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 525:   SMPTE Colorbar   Cross Hatch     Window 100%\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Teksten 'TOMMY' og 'DENNIS' vises i nogle pattern:\n\n");
 }

 if ((SDINo == SDITPG1) && (PT86No == 32)){													 // 8632
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 625:   EBU Colorbar     Philips 4:3     Multi Burst\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 525:   SMPTE Colorbar   Cross Hatch     Window 100%\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Teksten 'TOMMY' og 'DENNIS' vises i nogle pattern:\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Teksten i Philips 4:3 skifter position\n\n");
 }

 if (((SDINo == SDITPG2) || (SDINo == SDITPG5)) && (PT86No == 33)){							 // 8633
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 625:   EBU Colorbar     Philips 4:3     FuBK 4:3       Multi Burst\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," 525:   SMPTE Colorbar   Philips 16:9    Cross Hatch    Window 100%\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Teksten 'TOMMY' og 'DENNIS' vises i nogle pattern:\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Teksten i Philips 4:3 skifter position\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," I FuBK er der en bevægelig pind\n\n");
 }

 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," HUSK: Test begge udgange fra generatoren !!");

 SetCtrlVal(pattPNL, PATT_PNL_SCOPMSG," ");
 
 DisplayPanel (pattPNL);

 WaitForContinue();
 
 if (SDIPattOk == FALSE){
  MessagePopup (" SDI Pattern Test", "FEJLET");
  HidePanel(pattPNL);
  return FALSE;
 }

 
 switch (SDINo) {
  case SDIBLACK34: WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625;"); break;
  case SDIBLACK56: WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625;"); break;
  case SDIBLACK78: WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625;"); break;

  case SDITSG2:    WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625;"); break;
  case SDITSG3:    WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625;"); break;
  case SDITSG4:    WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625;"); break;
  
  case SDITPG1:    if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;");
                   else
                    WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625;");break;
  case SDITPG2:    if (AppType == PT5230)
                    WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625;"); break;
  case SDITPG5:    if (AppType == PT5230)
                    WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625;"); break;
 
 } // switch
 
 done = FALSE;
 
 // ----- SDI Black  --------------------------------------- 8609
 if (SDINo >= SDIBLACK34)
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
 } // end while

 // ----- SDI Test Signal Generator ------------------------ 8639
 if ((SDINo >= SDITSG2) && (SDINo <= SDITSG4))
 while (done == FALSE){
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBEBU;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBRED75;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT PLUG;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT CBSMPTE;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT BLACK;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT WIN100;");
  Delay(PatternShiftDelay);
 }  // end while
 
 // ----- SDI TPG1 (PT8602) pattern test ----------------------------------
 if (AppType == PT5210)
 if ((SDINo == SDITPG1) && (PT86No == 2))
 while (done == FALSE){
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBEBU;");
  WriteCOMPort(DUTCOM,"TEXT ON; TEXT ' TOMMY ';");
  WriteCOMPort(DUTCOM,"TEXT POS1; TEXT ' TOMMY ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBRED75;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT SHAL;");
  WriteCOMPort(DUTCOM,"TEXT OFF;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT CBSMPTE;");
  WriteCOMPort(DUTCOM,"TEXT ON; TEXT ' DENNIS ';");
  WriteCOMPort(DUTCOM,"TEXT POS1; TEXT ' DENNIS ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT DTIM;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT SDIC;");
  WriteCOMPort(DUTCOM,"TEXT OFF;");
  Delay(PatternShiftDelay);
 } // end while

 // ----- SDI TPG1  (PT8603) pattern test ---------------------------------
 if (AppType == PT5210)
 if ((SDINo == SDITPG1) && (PT86No == 3))
 while (done == FALSE){
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBEBU;");
  WriteCOMPort(DUTCOM,"TEXT ON; TEXT ' TOMMY ';");
  WriteCOMPort(DUTCOM,"TEXT POS1; TEXT ' TOMMY ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBRED75;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI625; PATT MULT;");
  WriteCOMPort(DUTCOM,"TEXT OFF;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT CBSMPTE;");
  WriteCOMPort(DUTCOM,"TEXT ON; TEXT ' DENNIS ';");
  WriteCOMPort(DUTCOM,"TEXT POS1; TEXT ' DENNIS ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT CROS;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"SYST SDI525; PATT WIN100;");
  WriteCOMPort(DUTCOM,"TEXT OFF;");
  Delay(PatternShiftDelay);
 } // end while


 // ----- SDI TPG1  (PT8632) pattern test -------------------------------- 

 if (AppType == PT5230)
 if ((SDINo == SDITPG1) && (PT86No == 32)) 
 while (done == FALSE){
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625; PATT CBEBU;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 ON; STR1 ' TOMMY ';");
  WriteCOMPort(DUTCOM,"STR2 ON; STR2 ' & ';");
  WriteCOMPort(DUTCOM,"STR3 ON; STR3 ' PTV ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625; PATT PHIL43;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 ON; STR1 ' TV ';");
  WriteCOMPort(DUTCOM,"STR2 ON; STR2 ' DENNIS ';");
  WriteCOMPort(DUTCOM,"STYL COMP;");
  Delay(1.0);
  WriteCOMPort(DUTCOM,"STYL STAN;");
  Delay(2.0);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"STYL COMP;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625; PATT MULT;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 OFF;");
  WriteCOMPort(DUTCOM,"STR2 OFF;");
  WriteCOMPort(DUTCOM,"STR3 OFF;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI525; PATT CBSMPTE;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 ON; STR1 ' PTV ';");
  WriteCOMPort(DUTCOM,"STR2 ON; STR2 ' & ';");
  WriteCOMPort(DUTCOM,"STR3 ON; STR3 ' DENNIS ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI525; PATT CROS;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI525; PATT WIN100;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 OFF;");
  WriteCOMPort(DUTCOM,"STR2 OFF;");
  WriteCOMPort(DUTCOM,"STR3 OFF;");
  Delay(PatternShiftDelay);
 } // end while


 // ----- SDI TPG2,TPG5  (PT8633)  pattern test ------------------------------
 if (AppType == PT5230)
 if (((SDINo == SDITPG2) || (SDINo == SDITPG5)) && (PT86No == 33))
 while (done == FALSE){
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (SDINo == SDITPG2) 
   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625; PATT CBEBU;");
   else
   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625; PATT CBEBU;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 ON; STR1 ' TOMMY ';");
  WriteCOMPort(DUTCOM,"STR2 ON; STR2 ' & ';");
  WriteCOMPort(DUTCOM,"STR3 ON; STR3 ' PTV ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  if (SDINo == SDITPG2) 
   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625; PATT PHIL43;");
   else
   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625; PATT PHIL43;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 ON; STR1 ' TOMMY ';");
  WriteCOMPort(DUTCOM,"STR2 ON; STR2 ' DENNIS ';");
  WriteCOMPort(DUTCOM,"STYL COMP;");
  Delay(1.0);
  WriteCOMPort(DUTCOM,"STYL STAN;");
  Delay(2.0);
  if (done == TRUE) break;
  WriteCOMPort(DUTCOM,"STYL COMP;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  if (SDINo == SDITPG2){
   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625; PATT FUBK43;");
   WriteCOMPort(DUTCOM,"TEXT:STR1 OFF;STR2 OFF;");
   WriteCOMPort(DUTCOM,":OUTP:STPG2:PATT:MOD:MOT ON;");
   }
   else{
   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625; PATT FUBK43;");
   WriteCOMPort(DUTCOM,"TEXT:STR1 OFF;STR2 OFF;");
   WriteCOMPort(DUTCOM,":OUTP:STPG5:PATT:MOD:MOT ON;");
   }
  Delay(2.0); // af hensyn til MOTION ON
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  if (SDINo == SDITPG2) 
   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625; PATT MULT;");
   else
   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625; PATT MULT;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 OFF;");
  WriteCOMPort(DUTCOM,"STR2 OFF;");
  WriteCOMPort(DUTCOM,"STR3 OFF;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  if (SDINo == SDITPG2) 
   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI525; PATT CBSMPTE;");
   else
   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI525; PATT CBSMPTE;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 ON; STR1 ' DENNIS ';");
  WriteCOMPort(DUTCOM,"STR2 ON; STR2 ' & ';");
  WriteCOMPort(DUTCOM,"STR3 ON; STR3 ' TOMMY ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  if (SDINo == SDITPG2) 
   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI525; PATT PHIL169;");
   else
   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI525; PATT PHIL169;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 ON; STR1 ' TOMMY ';");
  WriteCOMPort(DUTCOM,"STR2 ON; STR2 ' DENNIS ';");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  if (SDINo == SDITPG2) 
   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI525; PATT CROS;");
   else
   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI525; PATT CROS;");
  Delay(PatternShiftDelay);
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
  if (done == TRUE) break;
  if (SDINo == SDITPG2) 
   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI525; PATT WIN100;");
   else
   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI525; PATT WIN100;");
  WriteCOMPort(DUTCOM,"TEXT:STR1 OFF;");
  WriteCOMPort(DUTCOM,"STR2 OFF;");
  WriteCOMPort(DUTCOM,"STR3 OFF;");
  Delay(PatternShiftDelay);
 } // end while






 if (SDIPattOk == FALSE){
  MessagePopup (" SDI Pattern Test", "FEJLET");
  HidePanel(pattPNL);
  return FALSE;
 }


// -- SDI / analog BB timing check ----------------------
 SetPanelAttribute (pattPNL, ATTR_TITLE, " SDI - BB Timing ");
 WriteCOMPort(DUTCOM,"SYST SDI625; PATT CBEBU; DEL 0,0,0;");

 WriteCOMPort(DUTCOM,":OUTP:BB1:SYST PAL_ID; DEL 0,0,0;");
 WriteCOMPort(DUTCOM,":OUTP:BB2:SYST PAL_ID; DEL 0,0,0;");
 
 SelectPM3094Setup(7);
 SetCtrlVal(pattPNL, PATT_PNL_SCOPMSG,"PM3094 FRONTS = F7");
 ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind SDI generator til D1 DAC IN\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind BARCO MONITOR Y til scop CH-1\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind Black Burst #1 til scop CH-2\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Check på scopet, at videosignalerne i CH-1 og CH-2 er i fase\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP");

 WaitForContinue();
 

 if (SDIPattOk == TRUE)
   MessagePopup (" SDI Pattern Test + BB Timing", "OK");
  else
   MessagePopup (" SDI - BB Timing", "FEJLET");


HidePanel(pattPNL);

return TRUE;
} //CheckSDIPattern
 





//---------------------------------------------------------------------------------
int CheckSDIEmbAudio_EDH(int SDINo, int GetXX1){
// 970819  PT5210 remote komando syntax for embedded audio level ændret. Gammel syntax
//         gælder til og med PT5210-master SW rev 2.1. 
// 980325  PT5230 anvender kun ny syntax.

// GetXX1 bruges ikke her

double AudioLevelShiftDelay;
char TxtStr[60];
int handle_returned, controlID_returned;
 
 SDIEmbAudioOk = TRUE;

 DisplayPanel (pattPNL);
 SetCtrlVal(pattPNL, PATT_PNL_SCOPMSG,"");

// init panel
 if (SDINo >= SDIBLACK34)	
   Fmt(TxtStr,"%s< Embedded Audio:  SDI Black %i ",SDINo);
 else
 if (SDINo <= SDITPG5)	
   Fmt(TxtStr,"%s< Embedded Audio:  SDI Test Pattern Generator %i ",SDINo);
 else
   Fmt(TxtStr,"%s< Embedded Audio:  SDI Test Signal Generator %i ",SDINo-20);

 SetPanelAttribute (pattPNL, ATTR_TITLE, TxtStr);
 
 ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
 
 
 SDIEmbAudioOk = WriteProtect(OFF,FALSE);
 if (SDIEmbAudioOk == FALSE){
  HidePanel (pattPNL);
  return SDIEmbAudioOk;
 }
 

 
 // Embedded Audio = Silence
 // Audio Group = 1
 switch (SDINo) {
  case SDIBLACK34: WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625;EMB SIL;"); break; //8609
  case SDIBLACK56: WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625;EMB SIL;"); break;
  case SDIBLACK78: WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625;EMB SIL;"); break;
  
  case SDITSG2:    WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625;PATT BLACK;");
                   WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:SIGN S1KHZ;LEV SIL;");break;  //8639
  case SDITSG3:    WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625;PATT BLACK;");
                   WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:SIGN S1KHZ;LEV SIL;");break;
  case SDITSG4:    WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625;PATT BLACK;"); 
                   WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:SIGN S1KHZ;LEV SIL;");break;

  case SDITPG1:    if (AppType == PT5210){
                   WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;PATT BLACK;");	  //8602/03
  		           WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:SIGN S1KHZ;LEV SIL;");
  		           } 
  		           else {
                   WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625;PATT BLACK;");	  //8602/03/32
  		           WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:SIGN S1KHZ;LEV SIL;");
  		           } break;
  case SDITPG2:    if (AppType == PT5230){
                   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625;PATT BLACK;");	  //8633
  		           WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:SIGN S1KHZ;LEV SIL;GROUP GROUP1;");
  		           } break;
  case SDITPG5:    if (AppType == PT5230){
                   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625;PATT BLACK;");	  //8633
  		           WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:SIGN S1KHZ;LEV SIL;GROUP GROUP1;");
  		           } break;
 } 
 
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind SDI generator til DEX3072 D1 IN\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind DEX3072 AES75 OUT A til TEK764 AES75 IN\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil DEX3072: CH SELECT A = 0 og derefter = 1\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  A = grøn LED\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil TEK764:  AUDIO\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  CH-1 og CH-2 = MUTE\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil TEK764:  VIEW = CH STATUS      FORMAT = Text\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  Block CRC = is valid\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  Channel use = professional\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP");


 WaitForContinue();
 
 if (SDIEmbAudioOk == FALSE){
  MessagePopup (" SDI Embedded Audio", "FEJLET");
  HidePanel(pattPNL);
  return SDIEmbAudioOk;
 }

 
 
 //  Embedded Audio = OFF 
 switch (SDINo) {
  case SDIBLACK34: WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625;EMB OFF;");		  // 8609
  		           WriteCOMPort(DUTCOM,":OUTP:SB34:PATT BLACK;"); break;
  case SDIBLACK56: WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625;EMB OFF;"); 
  		           WriteCOMPort(DUTCOM,":OUTP:SB56:PATT BLACK;"); break;
  case SDIBLACK78: WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625;EMB OFF;"); 
  		           WriteCOMPort(DUTCOM,":OUTP:SB78:PATT BLACK;"); break;
  		           
  case SDITSG2:    WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625;EMB:SIGN OFF;");  // 8639
  		           WriteCOMPort(DUTCOM,":OUTP:STSG2:PATT BLACK;"); break;
  case SDITSG3:    WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625;EMB:SIGN OFF;"); 
  		           WriteCOMPort(DUTCOM,":OUTP:STSG3:PATT BLACK;"); break;
  case SDITSG4:    WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625;EMB:SIGN OFF;"); 
  		           WriteCOMPort(DUTCOM,":OUTP:STSG4:PATT BLACK;"); break;
  		           
  case SDITPG1:    if (AppType == PT5210){
                   WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;"); 				  // 8602/03
  		           WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:SIGN OFF;");
  		           }
  		           else {
                   WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625;"); 
  		           WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:SIGN OFF;");			  // 8632
  		           } break;
 case SDITPG2:     if (AppType == PT5230){
                   WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625;"); 
  		           WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:SIGN OFF;");			  // 8633
  		           } break;
 case SDITPG5:     if (AppType == PT5230){
                   WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625;"); 			  // 8633
  		           WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:SIGN OFF;");
  		           } break;
 } 

 ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"\n Indstil TEK764:  AUDIO\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"        Aflæs:  CH-1 og CH-2 = VBIT SET\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n");
 
 WaitForContinue();
 
 if (SDIEmbAudioOk == FALSE){
  MessagePopup (" SDI Embedded Audio", "FEJLET");
  HidePanel(pattPNL);
  return SDIEmbAudioOk;
 }

 

  

// ------- SDI TPG & TSG Embedded Audio Level Test -------------------------------------------
 if ((SDINo != SDIBLACK34) && (SDINo != SDIBLACK56) && (SDINo != SDIBLACK78)){  // no audio level in PT8609
 
 switch (SDINo){
  case SDITPG1: if (AppType == PT5210)
                  WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;EMB SIL;");
                 else 
                  WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625;EMB:SIGN S1KHZ;LEV SIL;"); break;
  case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625;EMB:SIGN S1KHZ;EMB SIL;GROUP GROUP1;"); break;
  case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625;EMB:SIGN S1KHZ;EMB SIL;GROUP GROUP1;"); break;
  
  case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625;EMB SIGN S1KHZ;");
                WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV SIL;"); break;
  case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625;EMB SIGN S1KHZ;");
                WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV SIL;"); break;
  case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625;EMB SIGN S1KHZ;");
                WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV SIL;"); break;
 }

  SetPanelAttribute (pattPNL, ATTR_TITLE, " Embedded Audio Test ");
  ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil TEK764:  AUDIO\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Aflæs level på TEK 764 (CH-1 og CH-2)\n");
  
  if ((AppType == PT5210) && (SDINo <= SDITPG5))
   SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Level skal skifte mellem 0, -9, -12, -14, -16, -18, -20 og MUTE\n\n");
  if ((AppType == PT5230) && (SDINo <= SDITPG5))
   SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Level skal skifte mellem 0, -9, -12, -15, -16, -18, -20 og MUTE\n\n");
  if ((SDINo >= SDITSG2) && (SDINo <= SDITSG4))
   SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Level skal skifte mellem 0, -9, -15 og MUTE\n\n");

  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," OBS: Mellem skiftene vises der muligvis CODE ERROR og VBIT SET\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"      Dette er ingen fejl!\n\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil TEK764:  VIEW = CH STATUS        FORMAT = Text\n");
  if (SDINo <= SDITPG5)
   SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Channel mode: skifter mellem stereo, 2-channel og monaural\n");
  if ((SDINo >= SDITSG2) && (SDINo <= SDITSG4))
   SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Channel mode: kun stereo\n");
  
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Block CRC: is valid\n\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," OK:    Tast F4\n");
  SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Fejl:  Tast F9\n\n");

  AudioLevelShiftDelay = 4.0;
  Cont = FALSE;
  while (Cont == FALSE) {

   // level = 0 dBFS
   if (AppType == PT5210){
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB0FS;");
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:SIGN S1KHZ;");
   }
   if (AppType == PT5230){
    switch (SDINo){
     case SDITPG1: WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:LEV DB0FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:SIGN S1KHZ;"); break;
     case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:LEV DB0FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:SIGN S1KHZ;GROUP GROUP1;"); break;
     case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:LEV DB0FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:SIGN S1KHZ;GROUP GROUP1;"); break;
                   
     case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV DB0FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:SIGN S1KHZ;"); break;
     case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV DB0FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:SIGN S1KHZ;"); break;
     case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV DB0FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:SIGN S1KHZ;"); break;
    }
   }//if
   
   Delay(AudioLevelShiftDelay);
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (Cont == TRUE) break;
   
   // level = -9 dBFS
   if (AppType == PT5210){
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM9FS;");
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB9FS;");
   }
   if (AppType == PT5230){
    switch (SDINo){
     case SDITPG1: WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:LEV DB9FS;");break;
     case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:LEV DB9FS;");break;
     case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:LEV DB9FS;");break;

     case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV DB9FS;");break;
     case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV DB9FS;");break;
     case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV DB9FS;");break;
    }
   }//if

   Delay(AudioLevelShiftDelay);
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (Cont == TRUE) break;
   
   // level = -12 dBFS	   (-15 dBFS for 8639)
   if (AppType == PT5210){
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM12FS;");
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB12FS;");
   }
   if (AppType == PT5230){
    switch (SDINo){
     case SDITPG1: WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:LEV DB12FS;");break;
     case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:LEV DB12FS;");break;
     case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:LEV DB12FS;");break;

     case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV DB15FS;");break;
     case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV DB15FS;");break;
     case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV DB15FS;");break;
    }
   }//if

   Delay(AudioLevelShiftDelay);
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (Cont == TRUE) break;
   
   // level = -14 / -15 dBFS	   (silence for 8639)
   if (AppType == PT5210){
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM14FS;");  // bemærk -14 dBFS i PT5210
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB14FS;");
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:SIGN DUAL;");
   }
   if (AppType == PT5230){
    switch (SDINo){
     case SDITPG1: WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:LEV DB15FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:SIGN DUAL;"); break;
     case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:LEV DB15FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:SIGN DUAL;");break;
     case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:LEV DB15FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:SIGN DUAL;");break;

     case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV SIL;");break;
     case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV SIL;");break;
     case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV SIL;");break;
    }
   }//if

   Delay(AudioLevelShiftDelay);
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (Cont == TRUE) break;
   
   // level = -16 dBFS		( 0 dBFS for 8639)
   if (AppType == PT5210){
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM16FS;");
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB16FS;");
   }
   if (AppType == PT5230){
    switch (SDINo){
     case SDITPG1: WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:LEV DB16FS;");break;
     case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:LEV DB16FS;");break;
     case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:LEV DB16FS;");break;

     case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV DB0FS;");break;
     case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV DB0FS;");break;
     case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV DB0FS;");break;
    }
   }//if
   
   Delay(AudioLevelShiftDelay);
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (Cont == TRUE) break;
   
   // level = -18 dBFS	  ( -9 dBFS for 8639)
   if (AppType == PT5210){
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM18FS;");
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB18FS;");
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:SIGN MBBC1KHZ;");
   }
   if (AppType == PT5230){
    switch (SDINo){
     case SDITPG1: WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:LEV DB18FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:SIGN MBBC1KHZ;"); break;
     case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:LEV DB18FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:SIGN MBBC1KHZ;");break;
     case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:LEV DB18FS;");
                   WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:SIGN MBBC1KHZ;");break;
                   
     case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV DB9FS;");break;
     case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV DB9FS;");break;
     case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV DB9FS;");break;
    }
   }//if

   Delay(AudioLevelShiftDelay);
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (Cont == TRUE) break;
   
   // level = -20 dBFS		( -15 dBFS for 8639)
   if (AppType == PT5210){
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM20FS;");
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB20FS;");
   }
   if (AppType == PT5230){
    switch (SDINo){
     case SDITPG1: WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:LEV DB20FS;");break;
     case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:LEV DB20FS;");break;
     case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:LEV DB20FS;");break;

     case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV DB15FS;");break;
     case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV DB15FS;");break;
     case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV DB15FS;");break;
    }
   }//if

   Delay(AudioLevelShiftDelay);
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (Cont == TRUE) break;
   
   // level = silence
   if (AppType == PT5210){
    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV SIL;");
   }
   if (AppType == PT5230){
    switch (SDINo){
     case SDITPG1: WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:LEV SIL;");break;
     case SDITPG2: WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:LEV SIL;");break;
     case SDITPG5: WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:LEV SIL;");break;

     case SDITSG2: WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:LEV SIL;");break;
     case SDITSG3: WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:LEV SIL;");break;
     case SDITSG4: WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:LEV SIL;");break;
    }
   }//if

   Delay(AudioLevelShiftDelay);
  } // while


  
  if (AppType == PT5210){
   WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DBM18FS;");
   WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:LEV DB18FS;");
   WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:SIGN S1KHZ;");
  }


  if (SDIEmbAudioOk == FALSE){
   MessagePopup (" SDI TSG Embedded Audio", "FEJLET");
   HidePanel(pattPNL);
   return SDIEmbAudioOk;
  } 
  
 } // if (SDINo == 1) 
  

  
  
  
  
  
//  EDH = ON Test ---------------------------------------
 SDIEmbAudioOk = TRUE;
 vca_selectsystem(L625);
 vca_selectinput(SERIAL_B);
 
 switch (SDINo) {
  case SDIBLACK34: WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625;PATT BLACK;EMB OFF;EDH ON;"); break;
  case SDIBLACK56: WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625;PATT BLACK;EMB OFF;EDH ON;"); break;
  case SDIBLACK78: WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625;PATT BLACK;EMB OFF;EDH ON;"); break;

  case SDITSG2:    WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625;PATT BLACK;EMB OFF;EDH ON;");
                   WriteCOMPort(DUTCOM,":OUTP:STSG2:EMB:SIGN OFF;"); break;
  case SDITSG3:    WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625;PATT BLACK;EMB OFF;EDH ON;"); 
                   WriteCOMPort(DUTCOM,":OUTP:STSG3:EMB:SIGN OFF;");break;
  case SDITSG4:    WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625;PATT BLACK;EMB OFF;EDH ON;");
                   WriteCOMPort(DUTCOM,":OUTP:STSG4:EMB:SIGN OFF;");break;
  
  case SDITPG1:    if (AppType == PT5210){
                    WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;PATT BLACK;EDH ON;"); 
                    WriteCOMPort(DUTCOM,":OUTP:SDIS:EMB:SIGN OFF;"); 
                    WriteCOMPort(DUTCOM,":OUTP:SDIS:TEXT OFF;"); 
                    }
                   if (AppType == PT5230){
                    WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625;PATT BLACK;EMBAUDIO OFF;EDH ON;"); 
                    WriteCOMPort(DUTCOM,":OUTP:STPG1:EMB:SIGN OFF;"); 
                    WriteCOMPort(DUTCOM,":OUTP:STPG1:TEXT OFF;"); 
                    }
                   break;
  case SDITPG2:    if (AppType == PT5230){
                    WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625;PATT BLACK;EMBAUDIO OFF;EDH ON;"); 
                    WriteCOMPort(DUTCOM,":OUTP:STPG2:EMB:SIGN OFF;");
                    WriteCOMPort(DUTCOM,":OUTP:STPG2:TEXT OFF;"); 
                    }
                   break;
  case SDITPG5:    if (AppType == PT5230){
                    WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625;PATT BLACK;EMBAUDIO OFF;EDH ON;"); 
                    WriteCOMPort(DUTCOM,":OUTP:STPG5:EMB:SIGN OFF;"); 
                    WriteCOMPort(DUTCOM,":OUTP:STPG5:TEXT OFF;"); 
                    }
                   break;
 } // switch

 SetPanelAttribute (pattPNL, ATTR_TITLE, " SDI EDH Test ");
 ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Forbind SDI generator til VCA B-IN\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Indstil R&S VCA:  SCOPE  -  ANC DATA STATUS  -  INFO\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Aflæs VCA:\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"                          FIELD 1   FIELD 2\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"    ACTIVE PICTURE CRC:   7AC2      7AC2\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"    FULL-FIELD CRC:       3765      6455\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," SDI-generator er indstillet til:\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"   SYSTEM=625/50   PATTERN=BLACK   AUDIO=OFF   EDH=ON   TEXT=OFF");
 
 WaitForContinue();
 
 if (SDIEmbAudioOk == FALSE){
  MessagePopup (" SDI  EDH", "FEJLET");
  HidePanel(pattPNL);
  return SDIEmbAudioOk;
 }



// EDH = OFF Test ----------------------------------------
 switch (SDINo) {
  case SDIBLACK34: WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625;EDH OFF;"); break;
  case SDIBLACK56: WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625;EDH OFF;"); break;
  case SDIBLACK78: WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625;EDH OFF;"); break;

  case SDITSG2:    WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625;EDH OFF;"); break;
  case SDITSG3:    WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625;EDH OFF;"); break;
  case SDITSG4:    WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625;EDH OFF;"); break;
  
  case SDITPG1:    if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625;EDH OFF;"); 
                   if (AppType == PT5230)
                    WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625;EDH OFF;"); 
                   break;
  case SDITPG2:    if (AppType == PT5230)
                    WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625;EDH OFF;"); 
                   break;
  case SDITPG5:    if (AppType == PT5230)
                    WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625;EDH OFF;"); 
                   break;
 } // switch

 ResetTextBox(pattPNL, PATT_PNL_TXTBOX,"");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Aflæs VCA:\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"                          FIELD 1   FIELD 2\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"    ACTIVE PICTURE CRC:   0000      0000\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"    FULL-FIELD CRC:       0000      0000\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," Tast F4: OK\n      F9: STOP\n\n\n\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX," SDI-generator er indstillet til:\n");
 SetCtrlVal(pattPNL, PATT_PNL_TXTBOX,"   SYSTEM=625/50   PATTERN=BLACK   AUDIO=OFF   EDH=OFF   TEXT=OFF");
 
 WaitForContinue();
 
 
 if (SDIEmbAudioOk == TRUE)
   MessagePopup (" SDI Embedded Audio & EDH", "OK");
  else
   MessagePopup (" SDI Embedded Audio & EDH", "FEJLET");
   

 HidePanel(pattPNL);

 return SDIEmbAudioOk;

} //CheckSDIEmbAudio









//---------------------------------------------------------------------------
int SDIJitterTest(int SDINo, int GetXX1){
// GetXX1 bruges ikke her
float jitterPPmeas[20];
float  jitterMean;
char TxtStr[60],
	 *KUread;
int handle_returned, 
    controlID_returned, 
    n, 
    k,
    xx_stik;

// init panel
if (SDINo >= SDIBLACK34)	
   Fmt(TxtStr,"%s< Jittermålinger:  SDI Black %i ",SDINo);
 else
 if (SDINo <= SDITPG5)	
   Fmt(TxtStr,"%s< Jittermålinger:  SDI Test Pattern Generator %i ",SDINo);
 else
   Fmt(TxtStr,"%s< Jittermålinger:  SDI Test Signal Generator %i ",SDINo-20);

 SetPanelAttribute (jitPNL, ATTR_TITLE, TxtStr);
 SetCtrlAttribute (jitPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);

 ResetTextBox(jitPNL, JIT_PNL_PPMEAS,"");
 SetCtrlVal(jitPNL, JIT_PNL_PPTOL1 ,"");
 SetCtrlVal(jitPNL, JIT_PNL_PPMEAN1 ,"");
 SetCtrlVal(jitPNL, JIT_PNL_PPTOL2 ,"");
 SetCtrlVal(jitPNL, JIT_PNL_PPMEAN2 ,"");
 
 ResetTextBox(jitPNL, JIT_PNL_TXTBOX,"");
 
 Fmt(TxtStr,"%s< %f[b4] UI", JT_peakpeakMaxFrame4);
 SetCtrlVal(jitPNL, JIT_PNL_PPTOL1,TxtStr);
 
 Fmt(TxtStr,"%s< %f[b4] UI", JT_peakpeakMaxLine);
 SetCtrlVal(jitPNL, JIT_PNL_PPTOL2,TxtStr);
 ProcessDrawEvents();
 

 DisplayPanel (jitPNL);
 
 SDIJitterOk = WriteProtect(OFF,FALSE);
 if (SDIJitterOk == FALSE){
  HidePanel (jitPNL);
  return SDIJitterOk;
 }
 
 
 
 
 SetCtrlVal(jitPNL, JIT_PNL_TXTBOX," Forbind SDI generator til R&S VCA B-in\n\n");
 SetCtrlVal(jitPNL, JIT_PNL_TXTBOX," F4: OK \n F9: STOP\n");


 SDIJitterOk = TRUE; 

 WaitForContinue();
 
 if (SDIJitterOk == FALSE) {
  HidePanel(jitPNL);
  return FALSE;
 }

 
 
 
 
 k = 1;			// k=1:  1.jittermåling   4 frames lpf=off
 				// k=2:  2.jittermåling   1 line   lpf=100kHz


 while ((k < 3) && (SDIJitterOk == TRUE)){

 ResetTextBox(jitPNL, JIT_PNL_PPMEAS,"");

  if (k == 1){
   SetCtrlVal (jitPNL, JIT_PNL_VCASETUP,"Jitter målinger: 4 FRAMES,  LPF = OFF");
  } 
  if (k == 2){
   SetCtrlVal (jitPNL, JIT_PNL_VCASETUP, "Jitter målinger: 1 LINE,  LPF = 100 kHz");
  } 
 
  ProcessDrawEvents();
  
  vca_selectsystem(L625);
  vca_selectinput(SERIAL_B);
  vca_jittertime();
  if (k == 1){
   vca_jittertime_lpfilter(OFF);
   vca_jittertime_meastime(FRAME4);
   }
  if (k == 2){
   vca_jittertime_lpfilter(F100K);
   vca_jittertime_meastime(LINE);
   }
  vca_jittertime_Xgain(X1);
  vca_jittertime_Ygain(Y1);
  vca_jittertime_average(A10);

  switch (SDINo) {
   case SDIBLACK34: WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625; PATT CBEBU;"); break;
   case SDIBLACK56: WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625; PATT CBEBU;"); break;
   case SDIBLACK78: WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625; PATT CBEBU;"); break;
   
   case SDITSG2:    WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625; PATT CBEBU;"); break;
   case SDITSG3:    WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625; PATT CBEBU;"); break;
   case SDITSG4:    WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625; PATT CBEBU;"); break;
   
   case SDITPG1:    if (AppType == PT5210)
                     WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625; PATT CBEBU;");
                    if (AppType == PT5230)
                     WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625; PATT CBEBU;");
                    break;
   case SDITPG2:    if (AppType == PT5230)
                     WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625; PATT CBEBU;");
                    break;
   case SDITPG5:    if (AppType == PT5230)
                     WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625; PATT CBEBU;");
                    break;
   }

  Delay(3.0);
  vca_jittertime_readpeakpeak(&jitterPPmeas[0]);
 
  // get 10 measurements
  for (n = 0; n < 10; n++) {
   Delay(1.0);
   vca_jittertime_readpeakpeak(&jitterPPmeas[n]);

   Fmt(TxtStr,"%s< %f[b4p3]\n", JT_peakpeak_ui);
   SetCtrlVal(jitPNL, JIT_PNL_PPMEAS ,TxtStr);
  
  } //for 

  // calculate mean from meas #5..#10
  jitterMean = 0;
  for (n = 5; n < 10; n++) 
   jitterMean = jitterMean + jitterPPmeas[n];

  if (k == 1)
   jitterMeanFrame4 = jitterMean / 5;
  if (k == 2)
   jitterMeanLine = jitterMean / 5;
 
  
  if (k == 1)
   if (jitterMeanFrame4 >= JT_peakpeakMaxFrame4)
    SDIJitterOk = FALSE; 
  if (k == 2)
   if (jitterMeanLine >= JT_peakpeakMaxLine)
    SDIJitterOk = FALSE; 


  if (k == 1){
   Fmt(TxtStr,"%s< %f[b4p3] UI", jitterMeanFrame4);
   SetCtrlVal(jitPNL, JIT_PNL_PPMEAN1 ,TxtStr);
  } 
  if (k == 2){
   Fmt(TxtStr,"%s< %f[b4p3] UI", jitterMeanLine);
   SetCtrlVal(jitPNL, JIT_PNL_PPMEAN2 ,TxtStr);
  } 
 
  k++;
 } // while (k...
 

  // Find stikforbindelse til generator
  KUread = ReadSDIKUNumber(SDINo, XB1_Stik);
  if (RS232Timeout == FALSE)
   xx_stik = XB1_Stik;
  
  if (RS232Timeout == TRUE){
   KUread = ReadSDIKUNumber(SDINo, XA1_Stik);
   if (RS232Timeout == FALSE)
    xx_stik = XA1_Stik;
    else{
     xx_stik = 0;
     SDIJitterOk = FALSE;
     MessagePopup (" SDI Jitter Time", " FEJLET:\n Ingen kontakt til SDI generator");
     HidePanel(jitPNL);
     return FALSE;
    }
  } // if
 
 
  if (SDIJitterOk == TRUE){
    Fmt(TxtStr,"%s<%f[b4w3p0] %f[b4w3p0]",jitterMeanFrame4*1000,jitterMeanLine*1000);   // jit = x.xxxUI til xxxUI  
    SaveInEEPROM(SDINo, 9, TxtStr, xx_stik);
    MessagePopup (" SDI Jitter Time", "Måling   OK");
    }
   else{
    SaveInEEPROM(SDINo, 9, "         ", xx_stik);
    MessagePopup (" SDI Jitter Time", "Måling   FEJLET");
   } 

 
 HidePanel(jitPNL);
return SDIJitterOk;
} // SDIJitterTest







//---------------------------------------------------------------------------------
int SDIAmplSpecTest(int SDINo, int GetXX1){
// GetXX1 bruges ikke her
char *strread, 
     *KUread, 
     TxtStr[100];
int handle_returned, 
    controlID_returned, 
    n, 
    amsp_handle,
    LevelmV,
    xx_stik;
float LevelMeas[2];

// init panel
 if (SDINo >= SDIBLACK34)	
   Fmt(TxtStr,"%s< Amplitudespectrum:  SDI Black %i ",SDINo);
 else
 if (SDINo <= SDITPG5)	
   Fmt(TxtStr,"%s< Amplitudespectrum:  SDI Test Pattern Generator %i ",SDINo);
 else
   Fmt(TxtStr,"%s< Amplitudespectrum:  SDI Test Signal Generator %i ",SDINo-20);

 SetPanelAttribute (amspPNL, ATTR_TITLE, TxtStr);
 SetCtrlAttribute (amspPNL, SDI_PNL_SDIOK_BTN, ATTR_VISIBLE, TRUE);
 
 Fmt(TxtStr,"%s< Level ved %f[b4p0] MHz: %f[b4p2] til %f[b4p2] dB",CheckFreqAmsp[0],AMSP_Min[0],AMSP_Max[0]);
 SetCtrlAttribute (amspPNL, AMSP_PNL_LEV_0, ATTR_LABEL_TEXT, TxtStr);
 Fmt(TxtStr,"%s< Level ved %f[b4p0] MHz:  max: %f[b4p2] dB",CheckFreqAmsp[1],AMSP_Max[1]);
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
 
 vca_selectsystem(L625);
 vca_selectinput(SERIAL_B);
 vca_amplspec();
 vca_amplspec_rescale();
 vca_amplspec_sweep(NORM);
 vca_amplspec_norm(OFF);
 vca_amplspec_Xgain(X50Div);		//   MHz/div
 vca_amplspec_Ygain(Y10Div);		//   dB/div

 WaitForContinue();
 
 if (SDIAmplSpecOk == FALSE) {
  HidePanel(amspPNL);
  return FALSE;
 }
 
 SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," Vent *");

 

  switch (SDINo) {
   case SDIBLACK34: WriteCOMPort(DUTCOM,":OUTP:SB34:SYST SDI625; PATT CBEBU;"); break; // 8609
   case SDIBLACK56: WriteCOMPort(DUTCOM,":OUTP:SB56:SYST SDI625; PATT CBEBU;"); break;
   case SDIBLACK78: WriteCOMPort(DUTCOM,":OUTP:SB78:SYST SDI625; PATT CBEBU;"); break;
   
   case SDITSG2:    WriteCOMPort(DUTCOM,":OUTP:STSG2:SYST SDI625; PATT CBEBU;"); break;  // 8639
   case SDITSG3:    WriteCOMPort(DUTCOM,":OUTP:STSG3:SYST SDI625; PATT CBEBU;"); break;
   case SDITSG4:    WriteCOMPort(DUTCOM,":OUTP:STSG4:SYST SDI625; PATT CBEBU;"); break;
   
   case SDITPG1:    if (AppType == PT5210)
                     WriteCOMPort(DUTCOM,":OUTP:SDIS:SYST SDI625; PATT CBEBU;");		 // 8602/03
                    if (AppType == PT5230)
                     WriteCOMPort(DUTCOM,":OUTP:STPG1:SYST SDI625; PATT CBEBU;");		 // 8602/03/32
                    break;
   case SDITPG2:    if (AppType == PT5230)
                     WriteCOMPort(DUTCOM,":OUTP:STPG2:SYST SDI625; PATT CBEBU;");
                    break;																 // 8633
   case SDITPG5:    if (AppType == PT5230)
                     WriteCOMPort(DUTCOM,":OUTP:STPG5:SYST SDI625; PATT CBEBU;");		 // 8633
                    break;
   }

 for (n = 0; n < 6; n++){
  SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," *");
  Delay(2.0);
 }

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
   LevelmV = (int)800.0 * pow (10.0, (LevelMeas[0] / 20.0)); // fra dB til mV (800mV=0dB)

 SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," *");

   
   
if (SDIAmplSpecOk == FALSE) { // hvis 1. måling fejler hentes en ny måling fra VCA
 Delay(3.0);
 SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX," *");
 SDIAmplSpecOk = TRUE;
 vca_amplspec_readdata();
 
 LevelMeas[0] = AS_level[(int)(CheckFreqAmsp[0]/AS_freq_delta) - 1] + 0.2;
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
   LevelmV = (int)800.0 * pow (10.0, (LevelMeas[0] / 20.0)); // fra dB til mV (800mV=0dB)
   
}// if (SDIAmplSpecOk == FALSE)
   
   
   
   
 SetCtrlVal(amspPNL, AMSP_PNL_TXTBOX,"\n");
   
if (SDIAmplSpecOk == TRUE){
   // get system date 
   d = DateStr();
   CopyString(dato,0,d,8,2);
   CopyString(dato,strlen(dato),d,0,2);
   CopyString(dato,strlen(dato),d,3,2);
 
   // get system time
   tt = TimeStr();
  
   strread = ReadSDIKUNumber(SDINo, XX1_Stik);
   CopyString(SDIKUStr,0,strread,0,-1);
	
   // check om logfil findes, ellers opret en ny logfil
   fhandle = OpenFile (SDIMeasLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   
   // SDI jitter & amplitude spectrum målinger gemmes i logfil
   Fmt(wrtbuf,"%s<Jitter,%s,%s,KU%s,%i[w2],%f[b4p3],%f[b4p3],   Amsp,%i,%i\n",dato,tt,SDIKUStr,SDINo,jitterMeanFrame4,jitterMeanLine,LevelmV,AppType);	
   bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
   CloseFile(fhandle);


  // Find stikforbindelse til generator
  KUread = ReadSDIKUNumber(SDINo, XB1_Stik);
  if (RS232Timeout == FALSE)
   xx_stik = XB1_Stik;
  
  if (RS232Timeout == TRUE){
   KUread = ReadSDIKUNumber(SDINo, XA1_Stik);
   if (RS232Timeout == FALSE)
    xx_stik = XA1_Stik;
    else{
     xx_stik = 0;
     SDIJitterOk = FALSE;
     MessagePopup (" SDI Jitter Time", " FEJLET:\n Ingen kontakt til SDI generator");
     HidePanel(amspPNL);
     return FALSE;
    }
  } // if
   

  MessagePopup (" SDI Amplitude Spectrum", "Måling   OK");

   
   
   Fmt(TxtStr,"%s<%i[w3]",LevelmV);   //  Amsp = xxx
   SaveInEEPROM(SDINo, 7, TxtStr, xx_stik);
   }
  else{
   MessagePopup (" SDI Amplitude Spectrum", "Måling   FEJLET");
   SaveInEEPROM(SDINo, 7, "       ", xx_stik);
  } 

 
 HidePanel(amspPNL);

return SDIAmplSpecOk;
} //SDIAmplSpecTest






//---------------------------------------------------------------------------------
int SDIReturnLossTest(int SDINo, int GetXX1){
// GetXX1 bruges ikke her
char *strread,
	 *KUread,
	 TxtStr[100];
int handle_returned, 
    controlID_returned, 
    n,
    xx_stik,
    rloss_handle;
float LevelMeas[3], RLossLevel;

// init panel
 if (SDINo >= SDIBLACK34)
  Fmt(TxtStr,"%s< Return Loss:  SDI Black %i ",SDINo);						  // 8609
 else 
 if (SDINo <= SDITPG5)
   Fmt(TxtStr,"%s< Return Loss:  SDI Test Pattern Generator %i ",SDINo);	  // 8602/03/32/33
 else 
  Fmt(TxtStr,"%s< Return Loss:  SDI Test Signal Generator %i ",SDINo-20);	  // 8639

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
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Placer jumper PP1 på SDI generator\n\n");
 
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
 Delay(2.0);
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 Delay(2.0);
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 
 vca_returnloss_norm(ON);
 
 Delay(2.0);
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 Delay(2.0);

// Den ene SDI udgang måles
 ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
 ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes til SDI output\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebroen SKAL forbindes direkte til BNC-stik\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," HUSK: Jumper i PP1 inden tryk på F4\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: STOP\n\n");
 DeleteGraphPlot (rlossPNL, RLOSS_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);

 WaitForContinue();

 if (SDIRLossOk == FALSE) {
  HidePanel(rlossPNL);
  return SDIRLossOk;
 }
 
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Vent *");
 Delay(2.0);
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 Delay(2.0);
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
 
 RLossLevel = LevelMeas[2];
 
 

// Den anden SDI udgang måles
 ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes til det andet SDI output\n\n\n");
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
 Delay(2.0);
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," *");
 Delay(2.0);
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
 


 if (LevelMeas[2] > RLossLevel)   // gem den største værdi ( = dårligste return loss)
  RLossLevel = LevelMeas[2];
 
 strread = ReadSDIKUNumber(SDINo, XX1_Stik);
 CopyString(SDIKUStr,0,strread,0,-1);
 if (strlen(SDIKUStr) < 6){
  MessagePopup (" SDI Return Loss", " Fjern jumper PP1 ");
  Delay(1.0);  // vent på opstart af SDI-generator
  }
 
 if (SDIRLossOk == TRUE) {
   // get system date 
   d = DateStr();
   CopyString(dato,0,d,8,2);
   CopyString(dato,strlen(dato),d,0,2);
   CopyString(dato,strlen(dato),d,3,2);
 
   // get system time
   tt = TimeStr();
  
   strread = ReadSDIKUNumber(SDINo, XX1_Stik);
   CopyString(SDIKUStr,0,strread,0,-1);
	
   // check om logfil findes, ellers opret en ny logfil
   fhandle = OpenFile (SDIMeasLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   
   // SDI return loss måling gemmes i logfil
   Fmt(wrtbuf,"%s<R Loss,%s,%s,KU%s,%i[w2],%f[b4p1],%i\n",dato,tt,SDIKUStr,SDINo,RLossLevel,AppType);	
   bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
   CloseFile(fhandle);

  // Find stikforbindelse til generator
  KUread = ReadSDIKUNumber(SDINo, XB1_Stik);
  if (RS232Timeout == FALSE)
   xx_stik = XB1_Stik;
  
  if (RS232Timeout == TRUE){
   KUread = ReadSDIKUNumber(SDINo, XA1_Stik);
   if (RS232Timeout == FALSE)
    xx_stik = XA1_Stik;
    else{
     xx_stik = 0;
     SDIJitterOk = FALSE;
     MessagePopup (" SDI Jitter Time", " FEJLET:\n Ingen kontakt til SDI generator");
     HidePanel(rlossPNL);
     return FALSE;
    }
  } // if



   Fmt(TxtStr,"%s<%f[b4p1]",RLossLevel); // return loss = -xx.x
   SaveInEEPROM(SDINo, 8, TxtStr, xx_stik);
   MessagePopup (" SDI Return Loss", "Måling   OK");
   }
  else{
   SaveInEEPROM(SDINo, 8, "       ", xx_stik);
   MessagePopup (" SDI Return Loss", "Måling   FEJLET");
  } 


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
unsigned char response[10]; // svar fra PromptPopup
char KUStr[10];  			// KUxxxxxx
char *KUread;		 		// Læst KU nummer
char PromptStr[50];
 

 // 0  producent
 WriteCOMPort(DUTCOM, ":FACT:SDIG:TEXT 0,'PTV';");
 Delay(0.2);

 // 1  12nc
 Fmt(DUTStr,"%s<:FACT:SDIG:TEXT 1,'%s';",NC12[2]);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.5);
 
 
 
  // 2  KU nummer
  KUread = ReadSDIGenlKUNumber();
  if (isdigit(KUread[2]) == FALSE)	
    CopyString(KUread,0,"",0,-1);
  
  if (RS232Timeout == TRUE) {
   MessagePopup(" SDI Genlock Kalibrering"," FEJL\n Intet svar fra SDI Genlock");
   return FALSE;
  } 

  EnterKUNo("SDI Genlock", KUread, response);

  
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
 

 if (InitSDIGenl() == FALSE)
  return FALSE;
 
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

 WriteCOMPort(PT5210COM,":OUTP:SB56:SYST SDI625; PATT CBEBU;");
 WriteCOMPort(DUTCOM,":INP:GENL:INP SDI;"); Delay(xd);
 WriteCOMPort(DUTCOM,"SYST SDI625;"); Delay(xd);
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
}//SDIGenlockTest(void)











//-----------------------------------------------------------------------
int SaveInEEPROM(int SDINo, int TxtNo, char Text[], int xx_stik){

 switch (SDINo){
  case SDIBLACK34:
  case SDITSG3:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;"); break;		 // 8609/39
  case SDIBLACK56:
  case SDITSG4:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;"); break;
  case SDIBLACK78:
  case SDITSG2:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
  case SDITPG1:    if (AppType == PT5210)
                     Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;");
                    else
                     Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIT;");break;		 // 8602/03/32
  case SDITPG2:
  case SDITPG5:   if (xx_stik == XA1_Stik)
                    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;");			 // 8633
                    else
                    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
 }
 WriteCOMPort(DUTCOM, DUTStr);

 Delay(0.1);
 if (SDINo > SDITPG5)
   Fmt(DUTStr,"%s<COMM 'HW',%i,'%s';",TxtNo,Text);						 // 8609/8639
  else
   Fmt(DUTStr,"%s<COMM 'SW',%i,'%s';",TxtNo,Text);						 // 8602/03/32/33
 WriteCOMPort(DUTCOM, DUTStr);									 
 Delay(0.1);

return TRUE;
}









int WriteSDICalDate(int SDINo, int xx_stik){
char *dd;					// MM-DD-YYYY

 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // Send addr for SDI#
 switch (SDINo){
  case SDIBLACK34:
  case SDITSG3:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;"); break;		 // 8609/39
  case SDIBLACK56:
  case SDITSG4:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;"); break;
  case SDIBLACK78:
  case SDITSG2:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
  case SDITPG1:   if (AppType == PT5210)
                    Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;");
                   else
                    Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIT;"); break;		 // 8602/03/32
  case SDITPG2:
  case SDITPG5:   if (xx_stik == XA1_Stik)
                    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;");			 // 8633
                    else
                    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
 }
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.1);
 if (SDINo <= SDITPG5)
   Fmt(DUTStr,"%s<COMM 'SW',4,'%s';",dato);						 // 8602/03/32/33
  else
   Fmt(DUTStr,"%s<COMM 'HW',4,'%s';",dato);						 // 8609/39
 WriteCOMPort(DUTCOM, DUTStr);									 
 
 Delay(0.3);

return 1;
} // WriteSDICalDate




char *ReadRLossInEEPROM (int SDINo, int xx_stik) {
  // read return loss in EEPROM  (user text 8)
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
  case SDIBLACK34:
  case SDITSG3:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',8;"); break;
  case SDIBLACK56:
  case SDITSG4:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',8;"); break;
  case SDIBLACK78:
  case SDITSG2:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',8;"); break;
  case SDITPG1:   if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',8;");
                   else
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;COMM 'SR?',8;");break;
  case SDITPG2:
  case SDITPG5:   if (xx_stik == XA1_Stik)   
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'SR?',8;");
   				   else
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'SR?',8;"); break;
  }
  Delay(0.1);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,strlen(strread)-2);	 // "-xx.x"   return -xx.x

 return strread;
} // ReadRLossInEEPROM






char *ReadJitterInEEPROM (int SDINo, int xx_stik) {
  // read return loss in EEPROM  (user text 9)
char *strread;
 FlushInQ(DUTCOM);
 switch (SDINo){
  case SDIBLACK34:
  case SDITSG3:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',9;"); break;
  case SDIBLACK56:
  case SDITSG4:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',9;"); break;
  case SDIBLACK78:
  case SDITSG2:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',9;"); break;
  case SDITPG1:   if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',9;");
                   else
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;COMM 'SR?',9;");break;
  case SDITPG2:
  case SDITPG5:   if (xx_stik == XA1_Stik)   
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'SR?',9;");
   				   else
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'SR?',9;"); break;
 }
 Delay(0.1);
 strread = ReadlnCOMPort(DUTCOM,0.5);
 CopyString(strread,0,strread,1,strlen(strread)-2);	 // "xx.x"   return xx.x

 return strread;
}						  





char *ReadAmspInEEPROM (int SDINo, int xx_stik) {
  // read amplitude spectrum in EEPROM  (user text 7)
char *strread;
 FlushInQ(DUTCOM);
 switch (SDINo){
  case SDIBLACK34:
  case SDITSG3:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',7;"); break;
  case SDIBLACK56:
  case SDITSG4:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',7;"); break;
  case SDIBLACK78:
  case SDITSG2:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',7;"); break;
  case SDITPG1:   if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',7;");
                   else
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;COMM 'SR?',7;");break;
  case SDITPG2:
  case SDITPG5:   if (xx_stik == XA1_Stik)   
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'SR?',7;");
   				   else
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'SR?',7;"); break;
 }
 Delay(0.1);
 strread = ReadlnCOMPort(DUTCOM,0.5);
 CopyString(strread,0,strread,1,strlen(strread)-2);	 // "xx.x"   return xx.x

 return strread;
} // ReadAmspInEEPROM						  






char *ReadSDIKUNumber (int SDINo, int xx_stik) {
  // read SDI KU nummer  (user text 2)
char *strread;
 FlushInQ(DUTCOM);
 switch (SDINo){
  case SDIBLACK34:
  case SDITSG3:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',2;"); break;
  case SDIBLACK56:
  case SDITSG4:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',2;"); break;
  case SDIBLACK78:
  case SDITSG2:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',2;"); break;
  case SDITPG1:   if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',2;");
                   else
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;COMM 'SR?',2;");break;
  case SDITPG2:
  case SDITPG5:   if (xx_stik == XA1_Stik)   
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'SR?',2;");
   				   else
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'SR?',2;"); break;
 }
  Delay(0.1);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,3,6);	 // "KUxxxxxx"   return xxxxxx

 return strread;
} // ReadSDIKUNumber					  






char *ReadSDIProdDate (int SDINo, int xx_stik) {
  // read SDI produktions dato  (user text 3)
char *strread;
 FlushInQ(DUTCOM);
 switch (SDINo){
  case SDIBLACK34:
  case SDITSG3:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',3;"); break;
  case SDIBLACK56:
  case SDITSG4:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',3;"); break;
  case SDIBLACK78:
  case SDITSG2:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',3;"); break;
  case SDITPG1:   if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',3;");
                   else 
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;COMM 'SR?',3;");break;
  case SDITPG2:
  case SDITPG5:   if (xx_stik == XA1_Stik)   
  				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'SR?',3;");
   				   else
   				    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'SR?',3;"); break;
 }
  Delay(0.1);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);	 // "xxxxxx"     return xxxxxx

 return strread;
} // ReadSDIProdDate


char *ReadSDICalDate (int SDINo, int xx_stik) {
  // read SDI kalibrerings dato  (user text 4)
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
   case SDIBLACK34:
   case SDITSG3:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'HR?',4;"); break;
   case SDIBLACK56:
   case SDITSG4:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_3;COMM 'HR?',4;"); break;
   case SDIBLACK78:
   case SDITSG2:   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'HR?',4;"); break;
   case SDITPG1:   if (AppType == PT5210)
                     WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SR?',4;");			  //8602/03
                    else
                     WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;COMM 'SR?',4;");break;	  // 8632
   case SDITPG2:
   case SDITPG5:   if (xx_stik == XA1_Stik)   
   				     WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'SR?',4;");
   				    else
   				     WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'SR?',4;"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);	 // "xxxxxx"     return xxxxxx

 return strread;
} // ReadSDICalDate




char *ReadSDITPGHwVer (int SDINo, int xx_stik) {
  // read SDI TPG hardware version
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
   case SDITPG1:  if (AppType == PT5210)
                   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SV?';");			 // 8302/03
                  else
                   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;COMM 'SV?';");break; // 8332
   case SDITPG2: 
   case SDITPG5: if (xx_stik == XA1_Stik)
   				  WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'SV?';");		  // 8633
   				 else
                  WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'SV?';"); break; 
  } 
  strread = ReadlnCOMPort(DUTCOM,0.5);
 return strread;
}




char *ReadSDITPGSwVer (int SDINo, int xx_stik) {
  // read SDI TSG SW version from µP
char *strread;
  FlushInQ(DUTCOM);
  switch (SDINo){
   case SDITPG1: if (AppType == PT5210)
                   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;COMM 'SI?';");
                  else
                   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;COMM 'SI?';"); break;// 8602/03/32
   case SDITPG2: 
   case SDITPG5: if (xx_stik == XA1_Stik)
                   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;COMM 'SI?';");	 // 8633
                  else 
                   WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;COMM 'SI?';"); break;
  }
  strread = ReadlnCOMPort(DUTCOM,0.5);
 return strread;
}





  




//---------------------------------------------------------------------------------
int InitSDI(int SDINo, int xx_stik){

/* 
 SDINo:     SDI TPG = 1,2 eller 5       
            SDI Black = 34, 56 eller 78
            SDI TSG = 22,23 eller 24
 xx_stik    til hvilket stik SDITPG2,5 er forbundet (kun ved PT8633) 
 			XA1	= 1	  XB1 = 2

 TVLines:   L625 eller L525
*/
int n,
    ValidKU;
char *usertxt;
char *dd;					// MM-DD-YYYY
unsigned char response[10]; // svar fra PromptPopup
char KUStr[10];  			// KUxxxxxx
char *KUread;
char PromptStr[50];

 
 // Send addr for SDI#
 switch (SDINo){
  case SDIBLACK34:
  case SDITSG3:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;"); break; 
  case SDIBLACK56:
  case SDITSG4:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_3;"); break;
  case SDIBLACK78:
  case SDITSG2:    Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
  case SDITPG1:   if (AppType == PT5210)
                    Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIS;");
                   else
                    Fmt(DUTStr,"%s<:FACT:V24C:ADDR SDIT;");break;  //8602/03
  case SDITPG2:
  case SDITPG5:if (xx_stik == XA1_Stik)
                 Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_1;");          //8633
                else
                 Fmt(DUTStr,"%s<:FACT:V24C:ADDR BBU_2;"); break;
  }
 WriteCOMPort(DUTCOM, DUTStr);
 
 Delay(0.1);


 if ((SDINo == SDITPG1) || (SDINo == SDITPG2) || (SDINo == SDITPG5)){
  PT86No = 0;
  usertxt = ReadSDITPGHwVer(SDINo, XX1_Stik);
  Scan(usertxt,"%s>%i",&PT86No);
  if (PT86No == 0) {	 // 900 serie har numre fra 256+900nr (dvs 257...)
    MessagePopup("SDI TPG"," Ingen kontakt til generator (PT8602/03/32/33");
    return FALSE;
    }
 } 

 if ((SDINo == SDITPG1) || (SDINo == SDITPG2) || (SDINo == SDITPG5)){
  usertxt = ReadSDITPGSwVer(SDINo, XX1_Stik);
  if (RS232Timeout == TRUE) {
   switch (SDINo){
    case SDITPG1: if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;");
                   else 
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;");break; //8602/03/32
    case SDITPG2:
    case SDITPG5: if (xx_stik == XA1_Stik)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;"); // 8633
                   else
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;"); break;
   }
    Delay(0.05);
    WriteCOMPort(DUTCOM,"COMM 'SZ';");  // reset generator
    Delay(2.0);
  }
 }
 
 
 // 0  producent
  switch (SDINo){
   case SDITPG1:
   case SDITPG2:
   case SDITPG5: WriteCOMPort(DUTCOM, "COMM 'SW',0,'PTV';"); break;
   case SDIBLACK34:
   case SDIBLACK56:
   case SDIBLACK78:
   case SDITSG2:
   case SDITSG3:
   case SDITSG4:  WriteCOMPort(DUTCOM, "COMM 'HW',0,'PTV';"); break;
  }      
 Delay(0.2);

 // 1  12nc
  switch (SDINo){
   case SDITPG1: if (AppType == PT5210)
                   Fmt(DUTStr, "%s<COMM 'SW',1,'%s';",NC12[5]);	       //8602/03
                  else 
                   Fmt(DUTStr, "%s<COMM 'SW',1,'%s';",NC12[10]);break; //8632
   case SDITPG2: 
   case SDITPG5: Fmt(DUTStr, "%s<COMM 'SW',1,'%s';",NC12[11]); break;  //8633
   case SDIBLACK34:
   case SDIBLACK56:
   case SDIBLACK78: Fmt(DUTStr, "%s<COMM 'HW',1,'%s';",NC12[4]); break; //8609
   case SDITSG2:
   case SDITSG3:
   case SDITSG4: Fmt(DUTStr, "%s<COMM 'HW',1,'%s';",NC12[12]); break;   //8639
  }      
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.5);
 
 
 
  // 2  KU nummer
  KUread = ReadSDIKUNumber(SDINo, XX1_Stik);
  if (isdigit(KUread[2]) == FALSE)	
    CopyString(KUread,0,"",0,-1);
    

 if ((SDINo == SDITPG1) || (SDINo == SDITPG2) || (SDINo == SDITPG5))
   Fmt(PromptStr,"%s< SDI TPG #%i",SDINo);
  else
 if ((SDINo == SDITSG2) || (SDINo == SDITSG3) || (SDINo == SDITSG4))
   Fmt(PromptStr,"%s< SDI TSG #%i",SDINo-20);
  else 
   Fmt(PromptStr,"%s< SDI Black #%i",SDINo);

  if (RS232Timeout == TRUE) {
   MessagePopup(" SDI Kalibrering"," FEJL\n Intet svar fra SDI generator");
   return FALSE;
  } 

  EnterKUNo(PromptStr, KUread, response);


  switch (SDINo){
   case SDITPG1: 
   case SDITPG2: 
   case SDITPG5: Fmt(DUTStr, "%s<COMM 'SW',2,'KU%s';",response); break;
   case SDIBLACK34:
   case SDIBLACK56:
   case SDIBLACK78:
   case SDITSG2:
   case SDITSG3:
   case SDITSG4: Fmt(DUTStr, "%s<COMM 'HW',2,'KU%s';",response); break;
  }      
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);

 
 
 // get system date 
 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // 3  produktionsdato
 usertxt = ReadSDIProdDate(SDINo, XX1_Stik);
 if (isdigit(usertxt[0]) == FALSE){
  switch (SDINo){
   case SDITPG1: 
   case SDITPG2: 
   case SDITPG5: Fmt(DUTStr, "%s<COMM 'SW',3,'%s';",dato); break;
   case SDIBLACK34:
   case SDIBLACK56:
   case SDIBLACK78:
   case SDITSG2:
   case SDITSG3:
   case SDITSG4: Fmt(DUTStr, "%s<COMM 'HW',3,'%s';",dato); break;
  }      
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 }

 // 4  kalibreringsdato 
 usertxt = ReadSDICalDate(SDINo, XX1_Stik);
 if (isdigit(usertxt[0]) == FALSE){			  
  switch (SDINo){
   case SDITPG1: 
   case SDITPG2: 
   case SDITPG5: Fmt(DUTStr, "%s<COMM 'SW',4,'%s';",dato); break;
   case SDIBLACK34:
   case SDIBLACK56:
   case SDIBLACK78:
   case SDITSG2:
   case SDITSG3:
   case SDITSG4: Fmt(DUTStr, "%s<COMM 'HW',4,'%s';",dato); break;
  }      
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 }
 
 
 // 6  PT-nummer
 if ((SDINo == SDIBLACK34) || (SDINo == SDIBLACK56) || (SDINo == SDIBLACK78)) // 8609
   WriteCOMPort(DUTCOM, "COMM 'HW',6,'PT8609';");
  else
 if ((SDINo == SDITSG2) || (SDINo == SDITSG3) || (SDINo == SDITSG4))		  // 8639
   WriteCOMPort(DUTCOM, "COMM 'HW',6,'PT8639';");
 Delay(0.2);
 
 if (SDINo <= SDITPG5){ 	   // 8602/03/32/33
   usertxt = ReadSDITPGHwVer(SDINo, XX1_Stik);
   Scan(usertxt,"%s>%i",&PT86No);
   if (PT86No == 2)
     WriteCOMPort(DUTCOM, "COMM 'SW',6,'PT8602';"); 
    else
   if (PT86No == 3)
     WriteCOMPort(DUTCOM, "COMM 'SW',6,'PT8603';"); 
    else
   if (PT86No == 32)
     WriteCOMPort(DUTCOM, "COMM 'SW',6,'PT8632';");
    else
   if (PT86No == 33)
     WriteCOMPort(DUTCOM, "COMM 'SW',6,'PT8633';"); 
  } 

 // 5  reserveret - 
 // 6  reserveret - PT86 nummer
 // 7  reserveret - amplitude spectrum.
 // 8  reserveret - return loss.
 // 9  reserveret - jitter time: (4 frames, lp=off), (1 line, lp=100kHz)
 
 
return TRUE;
} // InitSDI




//-------------------------------------------------------------------------
int SDITotalTest(int SDINo,int TestNo){


	SDITimingOk = FALSE;
	SDIAmplSpecOk = FALSE;
	SDIJitterOk = FALSE;
	SDIRLossOk = FALSE;
	SDIEmbAudioOk = FALSE;
	
	
    if ((SDINo >= SDITPG2) && (SDINo <= SDITPG5))
     if (GetTPGXX1Connection() == 3){  // opdaterer variablen XX1_Stik
      return FALSE;
     }


	if (TestNo == 1){
	 if (SDICal(L625,SDINo,FALSE) == TRUE)
	  SDICal(L525,SDINo, FALSE);
    }
 		 
 	if ((TestNo == 2) || (SDITimingOk == TRUE)){
 	 if (SDIJitterTest(SDINo,FALSE) == TRUE)
 	  SDIAmplSpecTest(SDINo,FALSE);
 	} 

 	if ( (TestNo == 3) || ((SDIAmplSpecOk == TRUE) && (SDIJitterOk == TRUE)) )
 	 SDIReturnLossTest(SDINo,FALSE);
 							
 	if ((TestNo == 4) || (SDIRLossOk == TRUE))
 	 CheckSDIEmbAudio_EDH(SDINo,FALSE);

 	if ((TestNo == 5) || (SDIEmbAudioOk == TRUE))
 	 CheckSDIPattern(SDINo,FALSE);
			 		
return TRUE;
}





int SDIReturnLossMeasurement(void){
// generel måleroutine - ikke testet færdig
char TxtStr[100];
int handle_returned, 
	controlID_returned, 
	n, 
	rloss_handle,btn,
	PrintStatus;
float LevelMeas[3], 
	  RLossLevel;

// init panel
 SetPanelAttribute (rlossPNL, ATTR_TITLE," Return Loss:  SDI Signal Generator");
 SetCtrlAttribute (rlossPNL, RLOSS_PNL_RLOSSOK_BTN, ATTR_VISIBLE, TRUE);
// SetCtrlAttribute (rlossPNL, RLOSS_PNL_SAVERLOSS_BTN, ATTR_VISIBLE, FALSE);
 DeleteGraphPlot (rlossPNL, RLOSS_PNL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
 ProcessDrawEvents();
 
 ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");

 DisplayPanel (rlossPNL);
 SDIRLossOk = TRUE;
 
 SDIRLossOk = WriteProtect(OFF,FALSE);
 if (SDIRLossOk == FALSE){
  HidePanel (rlossPNL);
  return SDIRLossOk;
 }
 

 ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 input forbindes til R&S VCA NOISE\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 output forbindes til R&S VCA B-in\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes IKKE!!!\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Placer jumper PP1 på SDI generator\n\n");
 
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: FEJL\n\n");

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

// SDI udgang måles
 ResetTextBox(rlossPNL, RLOSS_PNL_TXTBOX,"");
 ResetTextBox(rlossPNL, RLOSS_PNL_MAXLEVEL,"");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebro VCA-Z1 TEST forbindes til SDI output\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," Målebroen SKAL forbindes direkte til BNC-stik\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," HUSK: Jumper i PP1 inden tryk på F4\n\n");
 SetCtrlVal(rlossPNL, RLOSS_PNL_TXTBOX," F4: OK \n F9: FEJL\n\n");

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
				
 //SetCtrlAttribute (rlossPNL, RLOSS_PNL_SAVERLOSS_BTN, ATTR_VISIBLE, FALSE);

 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 GetUserEvent (WAIT, &handle_returned, &controlID_returned);
 
 SetPrintAttribute (ATTR_PAPER_HEIGHT, VAL_USE_PRINTER_DEFAULT);
 SetPrintAttribute (ATTR_PAPER_WIDTH, VAL_USE_PRINTER_DEFAULT);
 SetPrintAttribute (ATTR_COLOR_MODE, VAL_BW);
 
/* if (controlID_returned == RLOSS_PNL_SAVERLOSS_BTN){
  PrintStatus = PrintCtrl (rlossPNL, RLOSS_PNL_GRAPH, "sdirloss.prn",VAL_INTEGRAL_SCALE, 1);
  MessagePopup(" SDI Return Loss"," Måledata skrevet til fil: \"sdirloss.prn\"");
 } 
 
 */

 if ((LevelMeas[0] > RLoss_Max[0]) || 
    (LevelMeas[1] > RLoss_Max[1])  ||
    (LevelMeas[2] > RLoss_Max[2]))
  SDIRLossOk = FALSE;
 //SetCtrlAttribute (rlossPNL, RLOSS_PNL_SAVERLOSS_BTN, ATTR_VISIBLE, FALSE);

 HidePanel(rlossPNL);
 return SDIRLossOk;
} // SDIReturnLossMeasurement




//-------------------------------------------------------------------------------
int ResetSDI(int SDINo){
int button_pressed;
char *usertxt;

  button_pressed = ConfirmPopup ("Reset SDI Generator", "ADVARSEL: Reset bruges normalt kun på nyproducerede generatorer\nKalibreringsdata nulstilles");
 
  if (button_pressed == NO)
   return FALSE;
 
  
  
  if (SDINo != SDITPG1)
   if (GetTPGXX1Connection() == 3)
    return FALSE;
  

  switch (SDINo){
    case SDITPG1: if (AppType == PT5210)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIS;");		//8602/03 i PT5210
                   else 
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SDIT;");break; //8632 i PT5230
    case SDITPG2:
    case SDITPG5: if (XX1_Stik == XA1_Stik)
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_1;"); // 8633 i PT5230
                   else
                    WriteCOMPort(DUTCOM,":FACT:V24C:ADDR BBU_2;"); break;
  }
  
  Delay(0.05);
  WriteCOMPort(DUTCOM,"COMM 'SZ';");  // reset generator
  Delay(1.0);
  
  
  usertxt = ReadSDITPGSwVer(SDINo, XX1_Stik);
  if (RS232Timeout == FALSE){
     MessagePopup ("Reset af SDI generator", "Generator er nu reset\nSluk / tænd for apparatet");
     return TRUE;
     }
    else{
     MessagePopup ("Reset af SDI generator", "Generator er IKKE reset");
     return FALSE;

  }

} // ResetSDI()







//-------------------------------------------------------------------------------
int SetupPT5211(void){

 WriteCOMPort(PT5210COM,":OUTP:SB34:SYST SDI625; PATT CBEBU;"); 
 Delay(0.1);
 WriteCOMPort(PT5210COM,":OUTP:SB56:SYST SDI525; PATT CBSMPTE;"); 
 Delay(0.1);
 MessagePopup(" Setup for PT5211 "," PT5210 i testrack indstillet:\n SDI34 = EBU Color Bar\n SDI56 = SMPTE Color Bar");


return TRUE;
}




//---------------------------------------------------------------------------------
int CVICALLBACK SDIExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	if (event == EVENT_COMMIT) {
		 SDIJitterOk = FALSE;;
		 SDITimingOk = FALSE;;
		 SDIAmplSpecOk = FALSE;  
		 SDIRLossOk = FALSE;  
		 SDIGenlOk = FALSE;
		 SDIPattOk = FALSE;
		 ANLTPGPattOk = FALSE;
		 SDIEmbAudioOk = FALSE;
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
