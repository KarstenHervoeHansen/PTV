#include <ansi_c.h>
#include <rs232.h>
#include <formatio.h>
#include <utility.h>
/*
Test af LTC generator

970304, PF

Rettelser:



*/



#include <userint.h>
#include "ltc.h"
#include "cviutil.h"
#include "bbcal.h"



int ltcPNL;
int LTCsignalOk;



int LTC_VCOGOk,
	LTC_VCOMOk,
	LTC_SignalGOk,
	LTC_SignalMOk,
	
	LTC_TimingGOk,		  // ikke implementeret
	LTC_TimingMOk;
	


int InitLTC(void);
char *ReadLTCKUNumber (void);
char *ReadLTCProdDate (void);
char *ReadLTCCalDate (void);











//-----------------------------------------------------------------------
int CheckLTC_Signal(int TVSystem){
char MsgStr[50];



 DisplayPanel(ltcPNL);
 ResetTextBox (ltcPNL, LTC_PNL_TXTBOX, "");

 LTCsignalOk = TRUE;

 Set5640ExternDataVits();
 
 SelectPM3094Setup(3);
 
 SetCtrlVal (ltcPNL, LTC_PNL_SCOPSETUP," PM3094: FRONTS = F3");

 if (TVSystem == PAL){
   Set5640Standard("G");
   SetPanelAttribute (ltcPNL, ATTR_TITLE, " LTC: Kontrol af LTC signal  (PAL) ");
   SetCtrlVal (ltcPNL, LTC_PNL_RISEFALL, " Rise/Fall Time: 50 +12 -8 µS");
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST PALB;");
   SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Forbind 5640G VAR VIDEO til DUT GENLOCK A\n");
   SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Sæt VITC omskifter = G \n");
  }
  else{
   Set5640Standard("M");
   SetPanelAttribute (ltcPNL, ATTR_TITLE, " LTC: Kontrol af LTC signal  (NTSC) ");
   SetCtrlVal (ltcPNL, LTC_PNL_RISEFALL, " Rise/Fall Time: 25 ± 4 µS");
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST NTSC;");
   SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Forbind 5640M VAR VIDEO til DUT GENLOCK A\n");
   SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Sæt VITC omskifter = M \n");
  }
 SetCtrlVal (ltcPNL, LTC_PNL_VOLTPP, " Peak-Peak spænding: 4.2 ± 0.2 V");
 SetCtrlVal (ltcPNL, LTC_PNL_OVERSHOOT," Overshoot: < 0.2 V");
 SetCtrlVal (ltcPNL, LTC_PNL_DCOFFSET, " DC offset: 0 ± 50 mV");

 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Forbind DUT LTC til TEK 764 LTC IN\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Forbind DUT GENLOCK B til afslutning = 75 ohm\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Forbind TEK 764 LTC OUT til scop CH-4\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Indstil TEK764: Menu - Time Code - LTC \n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, "                 VIEW = AUDIO \n\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Kontroller rise/fall time på scop\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Kontroller peak-peak spænding scop\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Kontroller overshoot\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Kontroller DC offset\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Kontroller at LTC-tiden på TEK 764 tæller op 12:3x:xx\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Kontroller med 25 ohm og ingen afslutning at LTC-tiden tæller korrekt op\n\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(ltcPNL,LTC_PNL_TXTBOX, " Ved OK:   Tryk F4");


 WaitForContinue();

 if (TVSystem == PAL)
  if (LTCsignalOk == FALSE)
   MessagePopup(" LTC          "," FEJL ");
 			 
 
 if (TVSystem == NTSC)
  if (LTCsignalOk == TRUE)
    MessagePopup(" LTC          "," OK ");
   else
    MessagePopup(" LTC          "," FEJL ");
    
 HidePanel(ltcPNL);

 return LTCsignalOk;
} //CheckLTC_Signal







 //-----------------------------------------------------------------------
int LTC_TotalTest(void){

 InitLTC();

 LTC_VCOGOk = CheckLTCVCO(PAL);		 // function og tolerancer findes i 'bbcal.c'
 
 if (LTC_VCOGOk == TRUE)
  LTC_VCOMOk = CheckLTCVCO(NTSC);
  else
  return FALSE;
  

 if (LTC_VCOMOk == TRUE)
  LTC_SignalGOk = CheckLTC_Signal(PAL);
  else
  return FALSE;
 
 if (LTC_SignalGOk == TRUE)
  LTC_SignalMOk = CheckLTC_Signal(NTSC);
  else
  return FALSE;
 
 
return TRUE;
}






//----------------------------------------------------------------------------------
char *ReadLTCKUNumber (void) {
  // read LTC  KU nummer  (user text 2)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:TIM:TEXT? 2;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,3,6);
 return strread;
}						  



//----------------------------------------------------------------------------------
char *ReadLTCProdDate (void) {
  // read LTC produktions dato  (user text 3)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:TIM:TEXT? 3;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}


//----------------------------------------------------------------------------------
char *ReadLTCCalDate (void) {
  // read LTC kalibrerings dato  (user text 4)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:TIM:TEXT? 4;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}










//----------------------------------------------------------------------------------
int InitLTC(void){

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
 WriteCOMPort(DUTCOM, ":FACT:TIM:TEXT 0,'PHILIPS';");
 Delay(0.2);

 // 1  12nc
 Fmt(DUTStr,"%s<:FACT:TIM:TEXT 1,'%s';",NC12[9]);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.5);
 
 
 
  // 2  KU nummer
  KUread = ReadLTCKUNumber();
  if (isdigit(KUread[2]) == FALSE)	
    CopyString(KUread,0,"",0,-1);
    
  do {
   ValidKU = TRUE;
   PromptPopup (" LTC:   Indtast KU nummer", KUread, response, 6);

   if ((StringLength(response) == 0) & (StringLength(KUread) == 6))  // Only ENTER pressed
    CopyString(response,0,KUread,0,-1);

   for (n = 0; n < 6; n++)
    if (isdigit(response[n]) == FALSE)		  // digits only
     ValidKU = FALSE;
   
  
   if (StringLength(response) != 6)			  // exactly 6 digits
    ValidKU = FALSE;
  } while (ValidKU == FALSE);
  Fmt(DUTStr,"%s<:FACT:TIM:TEXT 2,'KU%s';",response);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 
 // get system date 
 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // 3  produktionsdato
 usertxt = ReadLTCProdDate();
 if (isdigit(usertxt[2]) == FALSE){			  
  Fmt(DUTStr,"%s<:FACT:TIM:TEXT 3,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 }

 // 4  kalibreringsdato 
 usertxt = ReadLTCCalDate();
 if (isdigit(usertxt[2]) == FALSE){			  
  Fmt(DUTStr,"%s<:FACT:TIM:TEXT 4,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 } 

 // 6  PT-nummer
 WriteCOMPort(DUTCOM, ":FACT:TIM:TEXT 6,'PT8607';");
 Delay(0.2);

 // 7  reserveret - ikke defineret.

return TRUE;
} // InitLTC


















//------------------------------------------------------------------------
int CVICALLBACK LTCOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			Cont = TRUE;

			break;
	}
	return 0;
}


int CVICALLBACK LTCExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			LTCsignalOk = FALSE;
			Cont = TRUE;

			break;
	}
	return 0;
}
