/*
Rettelser:
970306 	Kommando GI? indført: læs SW version i SPG: 
    	GI? returnerer SW ver fra version 020, tidligere udgaver
 		returnerer intet. Her bruges GI til field 1/5 skift i PAL.
 		Med SW ver 020 er GI erstattet af GJ og GJ erstattet af GK.
 */




#include <gpib.h>
#include "hp5313xa.h"
#include "ocxo.h"
#include "spgphase.h"
#include <ansi_c.h>
#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <userint.h>
#include "spghph.h"
#include "spggenl.h"
#include "cviutil.h"
#include "vlmutil.h"
#include "bbcal.h"
#include "sditest.h"

#define IEEEBUS 1

// SPG har intern RS-232 adresse = 15h = 21d
 
 
 
const int GenlWaitInterval = 1;  			// sec
const double AutoHPHWaitInterval = 0.9;		// sec

const double F27FreqNom = 27e6;				// Hz
const double F27FreqTol = 0.5;				// Hz

const double F10FreqNom = 10e6;				// Hz
const double F10FreqTol = 0.5;				// Hz


const unsigned int hphminmaxdiff = 150;		// max forskel ved HPH måling


int SPGSWRev;

int	SPGdone,
	TV;
 
int AutoHPH_PALOk,
	AutoHPH_NTSCOk,
	GenlBOk,
	GenlA_BOk,
	GenlPALOk,
	GenlNTSCOk,
	Genl443MHzOk,
	Genl358MHzOk,
	Genl10MHzOk,
	GenlSDIOk,
	GenlSystemOk,
	GenlInputOk,
	SPGCalOk,
	SPGPhaseCalOk,
 	SPGGenlPhasePALOk,
 	SPGGenlPhaseNTSCOk,
 	F10Ok,
 	F27Ok;

int	GenlockStatus;
int SPGGenlValue,
	SPGGenlOffsetValue;




int InitSPG(void);
int WriteSPGCalDate(void);
int MakeSPGGenlockCal(int TVSys);








int GetGenlockStatus(void){
char *strread;
int gs;
 
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21; COMM 'GS?';");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 0)
  Scan(strread,"%s>%i",&gs);
  else
  gs = 0;
 gs = gs & 0x06;	 // udmask bit 1,2    (bit 0,3-7=0)
 
return gs;
}
 



 
//--------------------------------------------------------------------------------
int MakeSPGGenlockInput(void){
// TEST1,2 er en test af inputsystemet: B input    A-B input
// Der testes med genlock til PAL.
//

char *SDIKU;
int button_pressed;

 DisplayPanel (spggenlPNL);
 ResetTextBox(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"");
 ProcessDrawEvents();

 Set5640Standard("G");
 Set5640Standard("M");
 
 GenlBOk = TRUE;
 GenlA_BOk = TRUE;


 // Check at SDI genlock er monteret
 SDIKU = ReadSDIGenlKUNumber();
// SDIKU = ReadSDIGenlKUNumber();
 if (strlen(SDIKU) < 6){
  MessagePopup(" SDI Genlock"," SDI Genlock option er ikke monteret !\n Monter option og start igen");
  HidePanel(spggenlPNL);
  return FALSE;
 }
 

 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (spggenlPNL);
  return FALSE;
  }


 
 // 1.TEST     Genlock for input B   (PAL) --------------------------------
 SetPanelAttribute (spggenlPNL, ATTR_TITLE, " Genlock input B ");

 WriteCOMPort(DUTCOM,":INP:GENL:INP B; SYST PALB;");
 SetGenlockSignal(GENL_PAL);

 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," GENL SIGNAL forbindes til DUT GENLOCK B\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Kontroller at lysdioden UNLOCKED er slukket\n\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Fortsæt:  Tast F4\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Afbryd:   Tast F9\n");

 WaitForContinue();
 if (GenlBOk == FALSE){
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (spggenlPNL);
  return FALSE;
  }


 Delay(2);
 
 // Check at DUT er genlocked til B
 Repeat = TRUE;
 GenlockStatus = GetGenlockStatus();
   while ((GenlockStatus != DUT_LOCKED) & (GenlBOk == TRUE)) {

	button_pressed = GenericMessagePopup ("DUT genlock",
										  "Genlock PAL burst til B input ikke låst",
										  "OK", "Afvist", "", 0, -1, 0,
										  VAL_GENERIC_POPUP_BTN1,
										  VAL_GENERIC_POPUP_BTN2,
										  VAL_GENERIC_POPUP_BTN3);
     switch (button_pressed) {
      case 1: GenlBOk = TRUE; Repeat = FALSE; break;
      case 2: GenlBOk = FALSE; Repeat = FALSE; break;
     }

    GenlockStatus = GetGenlockStatus();
   } //while
 



  if (GenlBOk == FALSE) {
   MessagePopup (" Genlock test", " Genlock input B\n FEJLET");
   HidePanel(spggenlPNL);
   SetGenlockSignal(GENL_OFF);
   return FALSE;
  }



// 2. TEST      Genlock for input A-B   (PAL) --------------------------------
 SetPanelAttribute (spggenlPNL, ATTR_TITLE, " Genlock input A-B ");

 WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST PALB;");
 Delay(1); 
 
 ResetTextBox(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," GENL SIGNAL forbindes til DUT GENLOCK A\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," DUT GENLOCK B forbindes til 5662 CH-A\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," DUT GENLOCK MONITOR forbindes til 5662 CH-B\n\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Check burstamplitude på GENLOCK MONITOR:\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Amplituden skal være ens indenfor ±1.5% på 5662 CH-A og CH-B\n\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Kontroller at lysdioden UNLOCKED er slukket\n\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Fortsæt:  Tast F4\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Afbryd:   Tast F9\n");

 WaitForContinue();
 if (GenlA_BOk == FALSE){
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 // Check at DUT er genlocked til A-B 
 Repeat = TRUE;
 GenlockStatus = GetGenlockStatus();
   while ((GenlockStatus != DUT_LOCKED) & (GenlA_BOk == TRUE)) {

    button_pressed = GenericMessagePopup ("DUT genlock",
									      "Genlock PAL burst til A-B input ikke låst", "OK", "Afvist", "",
									      0, -1, 0,
									      VAL_GENERIC_POPUP_BTN1,
									      VAL_GENERIC_POPUP_BTN2,
									      VAL_GENERIC_POPUP_BTN3);
     switch (button_pressed) {
      case 1: GenlA_BOk = TRUE; Repeat = FALSE; break;
      case 2: GenlA_BOk = FALSE; Repeat = FALSE; break;
     }

    GenlockStatus = GetGenlockStatus();
   } //while

  if (GenlA_BOk == FALSE) {
   MessagePopup (" Genlock test", " Genlock input A-B\n FEJLET");
   HidePanel(spggenlPNL);
   SetGenlockSignal(GENL_OFF);
   return FALSE;
  }


 SetGenlockSignal(GENL_OFF);

 GenlInputOk = GenlBOk & GenlA_BOk;

 HidePanel(spggenlPNL);
 

return TRUE;
} 






int MakeSPGGenlockSystem(void){
/* 
 TEST 3 .. 8 er en test af genlocksystemet med følgende signaler: PAL, NTSC, 4.43MHz, 
 3.58MHz, 10MHz og SDI. Som input vælges A, dog føres SDI til SDI Genlock option.
 For PAL, NTSC, 10MHz, 4.43MHz og 3.58MHz genlock testes om der genlockes ved ± 6dB.
 Frekvenssignalerne 10MHz, 4.43MHz og 3.58MHz dæmpes med et dæmpeled monteret i 
 genlock mux'en. 
 Genlock med PAL og NTSC programmeres med PM5640. PAL og NTSC testes desuden 
 med Hum og Noise.
*/


// 3. TEST   Genlock PAL --------------------------------

int button_pressed;
int handle_returned, controlID_returned;
int n;
char MsgStr[100];

 DisplayPanel (spggenlPNL);
 
 GenlPALOk = TRUE;
 GenlNTSCOk = TRUE;
 Genl443MHzOk = TRUE;
 Genl358MHzOk = TRUE;
 Genl10MHzOk = TRUE;
 GenlSDIOk = TRUE;
 
 SetPanelAttribute (spggenlPNL, ATTR_TITLE, " Genlock Test  (input A) ");
 ResetTextBox(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"");

 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Genlock signal input test:\n\n");

 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (spggenlPNL);
  return FALSE;
  }


 WriteCOMPort(DUTCOM,":INP:GENL:INP A; SYST PALB;");
 Set5640Standard("G");
 Set5640Pattern("G", 290);

// PAL -------- Hum, Noise, Burst, Sync - 6dB --------------------------------------------
 Set5640BurstLevel("G", 300 * 0.5);
 Set5640SyncLevel("G", 300 * 0.5);
 Set5640Hum("G", 1000);
 Set5640Noise("G", -40);
 SetGenlockSignal(GENL_PAL);
 Delay(1.0);
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," PAL:  Burst+Sync=-6dB   Hum=1000mV   Noise=-40dB  ");
 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Set5640Standard("G");
  GenlPALOk = FALSE;
  Fmt(MsgStr,"%s<Genlock PAL til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


// PAL -------- Hum, Noise, Burst, Sync + 6dB --------------------------------------------------
 Set5640BurstLevel("G", 300 * 2);
 Set5640SyncLevel("G", 300 * 2);
 Set5640Hum("G", 1000);
 Set5640Noise("G", -40);
 Delay(1.0);
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," PAL:  Burst+Sync=+6dB   Hum=1000mV   Noise=-40dB  ");
 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Set5640Standard("G");
  GenlPALOk = FALSE;
  MessagePopup ("Genlock fejl","Genlock PAL til A input ikke låst");
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");
  


 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 
 if (GenlPALOk == FALSE){
  SetGenlockSignal(GENL_OFF);
  Set5640Standard("G");
  HidePanel(spggenlPNL);
  return FALSE;
 }
 
// 4. TEST    PAL SUBC -------- +4 dB ---------------------------------------------------
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," PAL SUBC = +4dB ");
 SetGenlockSignal(GENL_443_HIGH);
 WriteCOMPort(DUTCOM,":INP:GENL:INP A; SYST F443MHZ;");

 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Genl443MHzOk = FALSE;
  Fmt(MsgStr,"%s<Genlock PAL SUBC til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


 
 
// PAL SUBC -------- - 6dB ---------------------------------------------------------------
 
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," PAL SUBC = -6dB ");
 SetGenlockSignal(GENL_443_LOW);
 
 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Genl443MHzOk = FALSE;
  Fmt(MsgStr,"%s<Genlock PAL SUBC til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 
 if (Genl443MHzOk == FALSE){
  SetGenlockSignal(GENL_OFF);
  HidePanel(spggenlPNL);
  return FALSE;
 }








 WriteCOMPort(DUTCOM,":INP:GENL:INP A; SYST NTSC;");
 Set5640Standard("M");
 Set5640Pattern("M", 290);

// 5. TEST    NTSC -------- Hum, Noise, Burst, Sync - 6dB --------------------------------
 Set5640BurstLevel("M", 40 * 0.5);
 Set5640SyncLevel("M", 40 * 0.5);
 Set5640Hum("M", 100);
 Set5640Noise("M", -40);
 SetGenlockSignal(GENL_NTSC);
 Delay(1.0);
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," NTSC: Burst+Sync=-6dB   Hum=100IRE   Noise=-40dB  ");
 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Set5640Standard("M");
  GenlNTSCOk = FALSE;
  Fmt(MsgStr,"%s<Genlock NTSC til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


// NTSC -------- Hum, Noise, Burst, Sync + 6dB ------------------------------------------
 Set5640BurstLevel("M", 40 * 2);
 Set5640SyncLevel("M", 40 * 2);
 Set5640Hum("M", 100);
 Set5640Noise("M", -40);
 Delay(1.0);

 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," NTSC: Burst+Sync=+6dB   Hum=100IRE   Noise=-40dB  ");
 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Set5640Standard("M");
  GenlNTSCOk = FALSE;
  Fmt(MsgStr,"%s<Genlock NTSC til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");

 
 Set5640Standard("G");
 Set5640Standard("M");


 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 
 if (Genl443MHzOk == FALSE){
  SetGenlockSignal(GENL_OFF);
  HidePanel(spggenlPNL);
  return FALSE;
 }



 
// 6. TEST   NTSC SUBC -------- +4dB --------------------------------------------
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," NTSC SUBC = +4dB ");
 SetGenlockSignal(GENL_358_HIGH);
 
 WriteCOMPort(DUTCOM,":INP:GENL:INP A; SYST F358MHZ;");

 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Genl358MHzOk = FALSE;
  Fmt(MsgStr,"%s<Genlock NTSC SUBC til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


 
 
// NTSC SUBC -------- - 6dB --------------------------------------------
 
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," NTSC SUBC = -6dB ");
 SetGenlockSignal(GENL_358_LOW);
 
 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Genl358MHzOk = FALSE;
  Fmt(MsgStr,"%s<Genlock NTSC SUBC til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 
 if (Genl358MHzOk == FALSE){
  SetGenlockSignal(GENL_OFF);
  HidePanel(spggenlPNL);
  return FALSE;
 }





// 7. TEST   Genlock 10 MHz --- + 6dB --------------------------------------------------------
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," 10 MHz = +6dB ");
 SetGenlockSignal(GENL_10M_HIGH);
 
 WriteCOMPort(DUTCOM,":INP:GENL:INP A; SYST F10MHZ;");

 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Genl10MHzOk = FALSE;
  Fmt(MsgStr,"%s<Genlock 10 MHz til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


 

 
// 10 MHz -------- - 6dB --------------------------------------------
 
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," 10 MHz = -6dB ");
 SetGenlockSignal(GENL_10M_LOW);
 
 WriteCOMPort(DUTCOM,":INP:GENL:INP A; SYST F10MHZ;");

 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  Genl10MHzOk = FALSE;
  Fmt(MsgStr,"%s<Genlock 10MHz til A input ikke låst");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


 SetGenlockSignal(GENL_OFF);



// 8. TEST  SDI Genlock -----------------------------------------------------------------
 button_pressed = ConfirmPopup ("SDI Genlock Test ", "PT5210 D1 BLACK  forbindes DUT SDI GENLOCK\n ");

 if (button_pressed == NO){
  HidePanel(spggenlPNL);
  return FALSE;
 }
 
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," SDI genlock  ");
 
 WriteCOMPort(DUTCOM,":INP:GENL:INP SDI; SYST SDI625;");
 WriteCOMPort(PT5210COM,":OUTP:SB2:SYST SDI625; PATT CBEBU;");

 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 
 
 if (GenlockStatus != DUT_LOCKED){
  GenlSDIOk = FALSE;
  MessagePopup ("Genlock fejl","SDI Genlock (625) ikke låst");
  HidePanel(spggenlPNL);
  return FALSE;
 }

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," OK\n");


 
 GenlSystemOk = GenlPALOk & GenlNTSCOk & Genl443MHzOk & 
                Genl358MHzOk & Genl10MHzOk & GenlSDIOk;
 
 if (GenlSystemOk == TRUE)
  MessagePopup (" Genlock Test"," OK");
 
 Set5640Standard("G");
 Set5640Standard("M");
 HidePanel(spggenlPNL);

   
return TRUE;
} //MakeSPGGenlockSystem






//--------------------------------------------------------------------------------
int MakeSPGHPHCal(void){
int handle_returned, controlID_returned;
int svar, button_pressed,n;
unsigned int hph[20],
			 hh,
			 hphmean,
			 hphmax,
			 hphmin,
			 hphdiff;
char *strread,*KUread;
char MsgStr[100];



 DisplayPanel (spghphPNL);
 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"");
 ProcessDrawEvents();
 
 
 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (spghphPNL);
  return FALSE;
  }


// check at BB-ref modul er monteret
 KUread = ReadBBKUNumber(3);
 if (strcmp(KUread,"999333") != 0){
  MessagePopup(" SPG kalibrering"," BB referencemodul (KU999333) skal være monteret som BB #3\n Monter referencemodul og start igen");
  HidePanel (spghphPNL);
  return FALSE;
 }
 

 // check SW version i SPG
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:V24C:ADDR SPG;");
 WriteCOMPort(DUTCOM,"COMM 'GI?';"); 
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%i",&SPGSWRev);
 

 
 AutoHPH_PALOk = TRUE;
 AutoHPH_NTSCOk = TRUE;

 Set5640Standard("G");
 Set5640Standard("M");

 InitSPG();
 
 
 // Auto HPH for PAL --------------------------------
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,"");

 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Forbind DUT GENLOCK B med scop CH-1\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Forbind DUT BB3 med scop CH-2\n\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Fortsæt:  Tast F4\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Afbryd:   Tast F9\n");


 
 WaitForContinue();
 if (AutoHPH_PALOk == FALSE){
  HidePanel(spghphPNL);
  return FALSE;
 }


 // Set genlock input = PAL_ID
 WriteCOMPort(PT5210COM,":OUTP:BB1:SYST PAL_ID; SCHP 0;");

 // Set DUT genlock to input-AB PAL burst
 WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST PALB;");
 WriteCOMPort(DUTCOM,":OUTP:BB3:SYST PAL_ID; SCHP 0;");
 
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Genlock=PAL burst         BB3=PAL_ID\n");
 Delay(1);
 
 // Check at DUT er genlocked
 n = 0;
 GenlockStatus = GetGenlockStatus();
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," *");
  n++;
  Delay(GenlWaitInterval);
  ProcessDrawEvents();
 }//while
 
 if (GenlockStatus != DUT_LOCKED){
  Fmt(MsgStr,"%s<Genlock PAL til A input ikke låst    Status=");
  Fmt(MsgStr,"%s[a]<%x",GenlockStatus);
  Fmt(MsgStr,"%s[a]< (hex)");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spghphPNL);
  return FALSE;
 }

 WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21;");

 Delay(1);
 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 FlushInQ(DUTCOM);
 
 // disse målinger gemmes ikke
 for (n = 0; n < 5; n++) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_PALOk == FALSE) {
    HidePanel(spghphPNL); 
    return FALSE;
   }

  Delay(0.1);
  WriteCOMPort(DUTCOM,"COMM 'GA';"); 
  Delay(AutoHPHWaitInterval);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 'GA?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  Fmt(MsgStr,"%s<%i[u]",hh);
  Fmt(MsgStr,"%s[a]<\n");
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
 }
 

 // disse målinger gemmes og middelværdi beregnes
 for (n = 0; n < 10; n++) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_PALOk == FALSE) {
    HidePanel(spghphPNL); 
    return FALSE;
   }
  
  Delay(0.1);
  WriteCOMPort(DUTCOM,"COMM 'GA';");
  Delay(AutoHPHWaitInterval);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 'GA?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  Fmt(MsgStr,"%s<%i[u]",hh);
  Fmt(MsgStr,"%s[a]<\n");
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
  hh = hh & 0x00ffffff;
  hph[n] = hh;
 }
 
 // find hph min
 hphmin = 0xffffffff;
 for (n = 0; n < 10; n++) {
  if (hph[n] < hphmin)
   hphmin = hph[n];
 }
 hphmin = hphmin | 0xff000000;   // set 8 MSB bits

 // find hph max
 hphmax = 0x0;
 for (n = 0; n < 10; n++) {
  if (hph[n] > hphmax)
   hphmax = hph[n];
 }
 hphmax = hphmax | 0xff000000;   // set 8 MSB bits
 
 hphdiff = hphmax - hphmin;
 
 
 // calculate mean
 hphmean = 0;
 for (n = 0; n < 10; n++) 
   hphmean = hphmean + hph[n];
 hphmean = hphmean / 10;
 hphmean = hphmean | 0xff000000;   // set 8 MSB bits

 Fmt(MsgStr,"%s<%i[u]",hphmin);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,MsgStr);
 Fmt(MsgStr,"%s<%i[u]",hphmax);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,MsgStr);
 Fmt(MsgStr,"%s<%i[u]",hphmean);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,MsgStr);

 if (hphdiff > hphminmaxdiff) {
  Fmt(MsgStr,"%s< Forskel mellem Max og Min er for stor (max forskel: ");
  Fmt(MsgStr,"%s[a]<%i",hphminmaxdiff);
  Fmt(MsgStr,"%s[a]<)\n Kalibrering stoppet");
  MessagePopup (" SPG Kalibreringsfejl: Auto HPH for PAL", MsgStr);
  AutoHPH_PALOk = FALSE;
  HidePanel(spghphPNL);
  return FALSE;
 }
 
 AutoHPH_PALOk = TRUE;

 if (AutoHPH_PALOk == TRUE) {
  Fmt(DUTStr,"%s<COMM 'GV',");
  Fmt(DUTStr,"%s[a]<%i[u]",hphmean);
  Fmt(DUTStr,"%s[a]<;");
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.3);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 'GA?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  if (hphmean != hh)
   AutoHPH_PALOk = FALSE;
 } 
 

  if (AutoHPH_PALOk == FALSE) {
   MessagePopup (" SPG: Auto HPH kalibrering for PAL fejlet", " Middelværdi ikke gemt korrekt i SPG\n Kalibrering stoppes");
   HidePanel(spghphPNL);
   return FALSE;
  }

 
 SelectPM3094Setup(1);
 

 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Linie 7 puls i CH-1 skal være i samme field som linie 7 puls i CH-2\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Field 1/5 kan skiftes med tasten 'Skift field 1/5'\n");
 
 Repeat = TRUE;
 while (Repeat == TRUE){

  button_pressed = GenericMessagePopup ("SPG kalibrering:  Field 1/5 for PAL",
									    "OK?", "OK", "Afvist", "Skift field 1/5",
									    0, -1, 0,
									    VAL_GENERIC_POPUP_BTN1,
									    VAL_GENERIC_POPUP_BTN2,
									    VAL_GENERIC_POPUP_BTN3);
   switch (button_pressed) {
    case 1: AutoHPH_PALOk = TRUE; Repeat = FALSE; break;
    case 2: AutoHPH_PALOk = FALSE; Repeat = FALSE; break;
    case 3: if (SPGSWRev < 1) 
    		  WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21; COMM 'GI';");  // old SPG SW
    		 else
    		  WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21; COMM 'GJ';");  // fra SWver 020
    		break;
   }

 } //while
 
 
 // if OK   store in EEPROM after HPH NTSC and genlock(PAL & NTSC) calibration
 

  if (AutoHPH_PALOk == FALSE) {
    HidePanel(spghphPNL);
   return FALSE;
  }

 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,"");


 Set5640Standard("G");
 Set5640Standard("M");
 

 // Auto HPH for NTSC --------------------------------

 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"\n");
   
 // Set genlock input = NTSC incl ID-puls i linie 11
 WriteCOMPort(PT5210COM,":OUTP:BB1:SYST NTSC; SCHP 0;");

 // Set DUT genlock to input-AB NTSC burst
 WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST NTSC;");
 WriteCOMPort(DUTCOM,":OUTP:BB3:SYST NTSC; SCHP 0;");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Genlock: NTSC\n");
 Delay(1);

 // Check at DUT er genlocked 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 while ((GenlockStatus != DUT_LOCKED) & (n < 10)){
  GenlockStatus = GetGenlockStatus();
  SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," *");
  n++;
  Delay(GenlWaitInterval);
  ProcessDrawEvents();
 }//while
 
 if (GenlockStatus != DUT_LOCKED){
  Fmt(MsgStr,"%s<Genlock NTSC til A input ikke låst    Status=");
  Fmt(MsgStr,"%s[a]<%x",GenlockStatus);
  Fmt(MsgStr,"%s[a]< (hex)");
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spghphPNL);
  return FALSE;
 }


 WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21;");

 Delay(1);
 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 FlushInQ(DUTCOM);
 
 // disse målinger gemmes ikke
 for (n = 0; n < 5; n++) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_NTSCOk == FALSE) {
    HidePanel(spghphPNL); 
    return FALSE;
   }
  Delay(0.1);
  WriteCOMPort(DUTCOM,"COMM 'GU';"); 
  Delay(AutoHPHWaitInterval);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 'GU?';"); 
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  Fmt(MsgStr,"%s<%i[u]",hh);
  Fmt(MsgStr,"%s[a]<\n");
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
 }
 

 // disse målinger gemmes og middelværdi beregnes
 for (n = 0; n < 10; n++) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_NTSCOk == FALSE) {
    HidePanel(spghphPNL); 
    return FALSE;
   }
  Delay(0.1);
  WriteCOMPort(DUTCOM,"COMM 'GU';");
  Delay(AutoHPHWaitInterval);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 'GU?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  Fmt(MsgStr,"%s<%i[u]",hh);
  Fmt(MsgStr,"%s[a]<\n");
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
  hh = hh & 0x00ffffff;
  hph[n] = hh;
 }
 
 // find hph min
 hphmin = 0xffffffff;
 for (n = 0; n < 10; n++) {
  if (hph[n] < hphmin)
   hphmin = hph[n];
 }
 hphmin = hphmin | 0xff000000;   // set 8 MSB bits

 // find hph max
 hphmax = 0x0;
 for (n = 0; n < 10; n++) {
  if (hph[n] > hphmax)
   hphmax = hph[n];
 }
 hphmax = hphmax | 0xff000000;   // set 8 MSB bits
 
 hphdiff = hphmax - hphmin;

 
 // calculate mean
 hphmean = 0;
 for (n = 0; n < 10; n++) 
   hphmean = hphmean + hph[n];
 hphmean = hphmean / 10;
 hphmean = hphmean | 0xff000000;   // set 8 MSB bits

 Fmt(MsgStr,"%s<%i[u]",hphmin);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,MsgStr);
 Fmt(MsgStr,"%s<%i[u]",hphmax);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,MsgStr);
 Fmt(MsgStr,"%s<%i[u]",hphmean);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,MsgStr);
 
 if (hphdiff > hphminmaxdiff) {
  Fmt(MsgStr,"%s< Forskel mellem Max og Min er for stor (max forskel: ");
  Fmt(MsgStr,"%s[a]<%i",hphminmaxdiff);
  Fmt(MsgStr,"%s[a]<)\n Kalibrering stoppet");
  MessagePopup (" SPG Kalibreringsfejl: Auto HPH for NTSC", MsgStr);
  AutoHPH_NTSCOk = FALSE;
  HidePanel(spghphPNL);
  return FALSE;
 }

 AutoHPH_NTSCOk = TRUE;

 if (AutoHPH_NTSCOk == TRUE) {
  Fmt(DUTStr,"%s<COMM 'GY',");
  Fmt(DUTStr,"%s[a]<%i[u]",hphmean);
  Fmt(DUTStr,"%s[a]<;");
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.3);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 'GU?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  if (hphmean != hh)
   AutoHPH_NTSCOk = FALSE;
 } 
 

  if (AutoHPH_NTSCOk == FALSE) {
   MessagePopup (" SPG: Auto HPH kalibrering for NTSC fejlet","Middelværdi ikke gemt korrekt i SPG\n Kalibrering stoppes");
   HidePanel(spghphPNL);
   return FALSE;
  }



 SelectPM3094Setup(2);

 
 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Linie 11 puls i CH-1 skal være i samme field som linie 11 puls i CH-2\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Field 1/3 kan skiftes med tasten 'Skift field 1/3'\n");
 
 Repeat = TRUE;
 while (Repeat == TRUE){

  button_pressed = GenericMessagePopup ("SPG kalibrering:  Field 1/3 for NTSC",
									    "OK?", "OK", "Afvist", "Skift field 1/3",
									    0, -1, 0,
									    VAL_GENERIC_POPUP_BTN1,
									    VAL_GENERIC_POPUP_BTN2,
									    VAL_GENERIC_POPUP_BTN3);
   switch (button_pressed) {
    case 1: AutoHPH_NTSCOk = TRUE; Repeat = FALSE; break;
    case 2: AutoHPH_NTSCOk = FALSE; Repeat = FALSE; break;
    case 3: if (SPGSWRev < 1) 
    		  WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21; COMM 'GJ';");  // old SPG SW
    		 else
    		  WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21; COMM 'GK';");  // fra SWver 020
    		break;
   }

 } //while
 

 // if OK  store in EEPROM after genlock(PAL & NTSC) calibration. See MakeSPGGenlockCal

 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,"");


 HidePanel (spghphPNL);
 
return AutoHPH_NTSCOk;
} //MakeSPGHPHCal







//-------------------------------------------------------------------------------
void SetSPGGenlockValue(int genlval){
char SPGGenlStr[11];

 if (TV == PAL)
   Fmt(DUTStr,"%s<COMM 'GF',");
  else
   Fmt(DUTStr,"%s<COMM 'GG',");
 Fmt(DUTStr,"%s[a]<%i",genlval);
 Fmt(DUTStr,"%s[a]<;");

 WriteCOMPort(DUTCOM,DUTStr);
 Delay(0.05);
} //SetSPGGenlockValue







//-------------------------------------------------------------------------------
int MakeSPGGenlockCal(int TVSys) {
/* 
 Der kalibreres altid med BB3, som her skal være et BB-reference modul
*/
char TitleStr[50],
	 MsgStr[50],
	 *strread,*strread1,*KUread;

int handle_returned, controlID_returned;



 Set5640Standard("G");
 Set5640Pattern("G", 290);
 Set5640Standard("M");
 Set5640Pattern("M", 290);
 
 TV = TVSys;
 

 SPGdone = FALSE;
 
 if (TVSys == PAL)
   SPGGenlPhasePALOk = FALSE;
  else
   SPGGenlPhaseNTSCOk = FALSE;
 
 DisplayPanel (spgphasePNL);


 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (spgphasePNL);
  return FALSE;
  }


 if (TVSys == PAL)
   SetPanelAttribute (spgphasePNL, ATTR_TITLE, " SPG genlock PAL        (BB3 = ref) ");
  else
   SetPanelAttribute (spgphasePNL, ATTR_TITLE, " SPG genlock NTSC       (BB3 = ref) ");


  // +-2000
 
 if (TVSys == PAL) {
  SPGGenlValue = 0;
  SPGGenlOffsetValue = 26240;
  SetCtrlAttribute (spgphasePNL, SPGPHA_PNL_SLIDE, ATTR_MAX_VALUE, SPGGenlValue + 20000);
  SetCtrlAttribute (spgphasePNL, SPGPHA_PNL_SLIDE, ATTR_MIN_VALUE, SPGGenlValue - 20000);
  SetCtrlVal (spgphasePNL, SPGPHA_PNL_SLIDE, SPGGenlValue);
  SetPM5662(PAL);
 } 
 
 if (TVSys == NTSC) {
  SPGGenlValue = 0;
  SPGGenlOffsetValue = 23260;
  SetCtrlAttribute (spgphasePNL, SPGPHA_PNL_SLIDE, ATTR_MAX_VALUE, SPGGenlValue + 20000);
  SetCtrlAttribute (spgphasePNL, SPGPHA_PNL_SLIDE, ATTR_MIN_VALUE, SPGGenlValue - 20000);
  SetCtrlVal (spgphasePNL, SPGPHA_PNL_SLIDE, SPGGenlValue);
  SetPM5662(NTSC);
 }
 
 

 ResetTextBox(spgphasePNL, SPGPHA_PNL_TXTBOX,"");
 if (TVSys == PAL) {
  WriteCOMPort(PT5210COM,":OUTP:BB1:SYST PAL_ID; SCHP 0;");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Forbind DUT GENLOCK B   til 5662 CH-A\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Forbind DUT BB3         til 5662 CH-B\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," NB: Kabler fra DUT til 5662 SKAL være lige lange!!\n\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Indstil 5662M: WFM  A-B  INT REF  2H  X-MAG\n\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Fase aflæses på 5662M\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Aflæs 5662M: Juster så faseforskel mellem 5662M CH-A og CH-B = 0\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Ved fejl: tryk F9\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Ved OK: tryk F4");
 }

 if (TVSys == NTSC) {
  WriteCOMPort(PT5210COM,":OUTP:BB1:SYST NTSC; SCHP 0;");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Forbind PT5210 BB til DUT GENLOCK A\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Forbind DUT GENLOCK B   til 5662 CH-A\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Forbind DUT BB3         til 5662 CH-B\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," NB: Kabler fra DUT til 5662 SKAL være lige lange!!\n\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Indstil 5662G: WFM  A-B  INT REF  2H  X-MAG\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Fase aflæses på 5662G\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Aflæs 5662G: Juster så faseforskel mellem 5662G CH-A og CH-B = 0\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Ved fejl: tryk F9\n");
  SetCtrlVal(spgphasePNL, SPGPHA_PNL_TXTBOX," Ved OK: tryk F4");
 }

 
 if (TVSys == PAL){
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST PALB;");
   WriteCOMPort(DUTCOM,":OUTP:BB3:SYST PAL_ID;");
  }  
  
  if (TVSys == NTSC){
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST NTSC;");
   WriteCOMPort(DUTCOM,":OUTP:BB3:SYST NTSC;");
  }
   

 

 // Send addr for SPG#
 WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21;");
 Delay(0.1);


 SetSPGGenlockValue(SPGGenlValue + SPGGenlOffsetValue);
 

 // her ventes indtil brugeren har tastst OK=F4 eller FEJL=F9
 while (SPGdone == FALSE) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 }



 // Da NTSC kalibreres efter PAL, gemmes værdierne i EEPROM efter NTSC kalibreringen
 // Dette forudsætter at AutoHPH kalibreringen er udført inden genlock kalibreringen
 
 SPGPhaseCalOk = FALSE;
 if ((TV == NTSC) && (AutoHPH_PALOk == TRUE) && (AutoHPH_NTSCOk == TRUE) && 
  	 (SPGGenlPhasePALOk == TRUE) && (SPGGenlPhaseNTSCOk == TRUE))
  SPGPhaseCalOk = TRUE;  
  	
 if ((TV == NTSC) && (SPGPhaseCalOk == TRUE)){
  WriteSPGCalDate();
  WriteCOMPort(DUTCOM,":FACT:SPGG:STOR;");
  Delay(0.5);
  
  // check af at de 4 værdier er gemt korrekt i EEPROM
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:SPGG:STOR?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Delay(0.5);
  
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:SPGG:READ?;");
  strread1 = ReadlnCOMPort(DUTCOM,0.5);
  
  if (CompareStrings (strread, 0, strread1, 0, 0) != 0) {
   MessagePopup (" SPG kalibrering", " FEJL: Kalibreringsdata ikke gemt korrekt i EEPROM");
   if (TV == PAL)
    SPGGenlPhasePALOk = FALSE;
   else
    SPGGenlPhaseNTSCOk = FALSE;
   SPGPhaseCalOk = FALSE;
  }
  else
  MessagePopup (" SPG kalibrering", " Kalibreringsdata gemt i EEPROM");
  
 }
 
 
 HidePanel (spgphasePNL);
 
 if (TV == NTSC){
  SPGCalOk = SPGPhaseCalOk & AutoHPH_PALOk & AutoHPH_NTSCOk;
 
 }
 
 

 if (TV == PAL)
   return SPGGenlPhasePALOk;
  else
   return SPGGenlPhaseNTSCOk;
 
} // MakeSPGGenlockCal









//--------------------------------------------------------------------------------
int MakeOCXOCal(int Freq){	  // Freq = 27 or 10
char MsgTxt[30];
int handle_returned, controlID_returned;
ViReal64 freqmeas,diff; // = double
double FreqNom, FreqTol;

 if (Freq == 27){
   SetPanelAttribute (ocxoPNL, ATTR_TITLE, " 27 MHz OCXO ref justering ");
   WriteCOMPort(DUTCOM,":INP:GENL:INP INT;");
   Fmt(MsgTxt,"%s< ± %f[p1] Hz",F27FreqTol);
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Forbind probe fra HP53132 CH-1 til TP8\n");
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Juster frekvens med R151\n\n");
   FreqNom = F27FreqNom;
   FreqTol = F27FreqTol;
   F27Ok = TRUE;
  } 
  else{
   SetPanelAttribute (ocxoPNL, ATTR_TITLE, " 10 MHz OCXO ref justering ");
   WriteCOMPort(DUTCOM,":INP:GENL:INP INTERNAL2;");
   Fmt(MsgTxt,"%s< ± %f[p1] Hz",F10FreqTol);
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Forbind probe fra HP53132 CH-1 til TP8\n");
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Juster frekvens med R49\n\n");
   FreqNom = F10FreqNom;
   FreqTol = F10FreqTol;
   F10Ok = TRUE;
  } 

 DisplayPanel(ocxoPNL);

 ResetTextBox(ocxoPNL, OCXO_PNL_TXTBOX,"");
 
 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (ocxoPNL);
  return FALSE;
  }
 
 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," HUSK: Apparatet skal have været tændt i MINDST 2 timer\n\n");
 SetCtrlVal(ocxoPNL,OCXO_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(ocxoPNL,OCXO_PNL_TXTBOX, " Ved OK:   Tryk F4\n");
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_LABEL_TEXT, MsgTxt);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_FREQ, ATTR_TEXT_COLOR, VAL_RED);

#if IEEEBUS
 DevClear (0, 3);
 ErrIEEE = hp5313xa_init ("GPIB::3", VI_OFF, VI_ON, 2, &hp53132);
 Delay(0.1);
 ErrIEEE = hp5313xa_freqPeriodRatio (hp53132, 1, 1, 9, VI_ON, 1);
 
#endif 
 

 done = FALSE;
 while (done == FALSE) {
  

  DisableBreakOnLibraryErrors();
  hp5313xa_writeInstrData (hp53132, ":READ?");
  Delay(1.5);
  ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeas);
  if (ErrIEEE == 0xBFFF0015)  //Timeout
   freqmeas = 0.0;
  Delay(0.1);
  EnableBreakOnLibraryErrors();
 
  // min max slide values
  
  diff = fabs(freqmeas - FreqNom);
  if (diff <= 10) {
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MAX_VALUE, 10.0);
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MIN_VALUE, -10.0);
   } 
   else 
  if (diff <= 30) {
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MAX_VALUE, 50.0);
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MIN_VALUE, -50.0);
   } 
   else
  if (diff <= 80) {
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MAX_VALUE, 100.0);
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MIN_VALUE, -100.0);
   } 
   else
  if (diff <= 400) {
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MAX_VALUE, 500.0);
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MIN_VALUE, -500.0);
   } 
   else {
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MAX_VALUE, 2000.0);
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MIN_VALUE, -2000.0);
   } 

  SetCtrlVal (ocxoPNL, OCXO_PNL_SLIDE, freqmeas - FreqNom);
  Fmt(MsgTxt,"%s<%f[p1]",freqmeas);
  SetCtrlVal (ocxoPNL, OCXO_PNL_FREQ, MsgTxt);
  ProcessDrawEvents();
  
  if (fabs(freqmeas - FreqNom) > FreqTol) {
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_FILL_COLOR, VAL_RED);
	SetCtrlAttribute (ocxoPNL, OCXO_PNL_FREQ, ATTR_TEXT_COLOR, VAL_RED);
   } 
   else {
    SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_FILL_COLOR, VAL_GREEN);
	SetCtrlAttribute (ocxoPNL, OCXO_PNL_FREQ, ATTR_TEXT_COLOR, VAL_GREEN);
   } 
   

  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
 } // while

 
 HidePanel(ocxoPNL);


 if (Freq == 27)
   return F27Ok;
  else
   return F10Ok;
}




					   

//--------------------------------------------------------------------------------
int MakeSPGReset(void){ 
int button_pressed;
int dd[4];
char *strread;
int SPGResetOk;

 button_pressed = ConfirmPopup ("Reset SPG", "ADVARSEL: Reset bruges normalt kun på nyproducerede SPG'er\nKalibreringsdata nulstilles");

 if (button_pressed == NO){
  return FALSE;
 }

 WriteCOMPort(DUTCOM,":FACT:V24C:ADDR 21; COMM 'GZ';");
 Delay(0.1);
 WriteCOMPort(DUTCOM,":FACT:SPGG:STOR;"); 
 Delay(0.3);
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:SPGG:READ?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (StringLength(strread) > 0)
   Scan(strread,"%s>%4i[x]",dd);
  else {
   dd[0] = 0; dd[1] = 0; dd[2] = 0; dd[3] = 0;
  }
  

 SPGResetOk = FALSE;
 if ((dd[0] == 196290) && (dd[1] == 28810) && (dd[2] == 196290) && (dd[3] == 28810))
  SPGResetOk = TRUE;
  
 if (SPGResetOk == TRUE)
   MessagePopup ("Reset SPG", "Kalibreringsdata er nu nulstillet\nSluk / tænd for apparatet");
  else
   MessagePopup ("Reset SPG", "Reset af kalibreringsdata FEJLET\n");


return SPGResetOk;
}


//----------------------------------------------------------------------------------
int WriteSPGCalDate(void){
char *dd;					// MM-DD-YYYY
char dato[10];  			// YYMMDD

 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);

 
 // 4  kalibreringsdato 
 CopyString(DUTStr,0,":FACT:SPGG:TEXT 4,'",0,-1);
 CopyString(DUTStr,StringLength(DUTStr),dato,0,-1);
 CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.3);

return TRUE;
}






//----------------------------------------------------------------------------------
char *ReadSPGKUNumber (void) {
  // read SPG KU nummer  (user text 2)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:SPGG:TEXT? 2;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,3,6);	// "KUxxxxxx"	 return xxxxxx
 return strread;
}						  



//----------------------------------------------------------------------------------
char *ReadSPGProdDate (void) {
  // read SPG produktions dato  (user text 3)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:SPGG:TEXT? 3;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}


//----------------------------------------------------------------------------------
char *ReadSPGCalDate (void) {
  // read SPG kalibrerings dato  (user text 4)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:SPGG:TEXT? 4;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}






//----------------------------------------------------------------------------------
int InitSPG(void){

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
 WriteCOMPort(DUTCOM, ":FACT:SPGG:TEXT 0,'PHILIPS';");
 Delay(0.2);

 // 1  12nc
 CopyString(DUTStr,0,":FACT:SPGG:TEXT 1,'",0,-1);
 CopyString(DUTStr,StringLength(DUTStr),NC12[7],0,-1);
 CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.7);
 
 
 
  // 2  KU nummer
  KUread = ReadSPGKUNumber();
  if (isdigit(KUread[0]) == FALSE)	
    CopyString(KUread,0,"",0,-1);
    
  do {
   ValidKU = TRUE;
   PromptPopup ("SPG:   Indtast KU nummer", KUread, response, 6);

   if ((StringLength(response) == 0) & (StringLength(KUread) == 6))  // Only ENTER pressed
    CopyString(response,0,KUread,0,-1);

   for (n = 0; n < 6; n++)
    if (isdigit(response[n]) == FALSE)		  // digits only
     ValidKU = FALSE;
   
  
   if (StringLength(response) != 6)			  // exactly 6 digits
    ValidKU = FALSE;
  } while (ValidKU == FALSE);

  CopyString(KUStr,0,"KU",0,-1);
  CopyString(KUStr,StringLength(KUStr),response,0,-1);
  CopyString(DUTStr,0,":FACT:SPGG:TEXT 2,'",0,-1);
  CopyString(DUTStr,StringLength(DUTStr),KUStr,0,-1);
  CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 
 // get system date 
 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // 3  produktionsdato
 usertxt = ReadSPGProdDate();
 if (isdigit(usertxt[0]) == FALSE){			  
  CopyString(DUTStr,0,":FACT:SPGG:TEXT 3,'",0,-1);
  CopyString(DUTStr,StringLength(DUTStr),dato,0,-1);
  CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 }

 // 4  kalibreringsdato 
 usertxt = ReadSPGCalDate();
 if (isdigit(usertxt[0]) == FALSE){			  
  CopyString(DUTStr,0,":FACT:SPGG:TEXT 4,'",0,-1);
  CopyString(DUTStr,StringLength(DUTStr),dato,0,-1);
  CopyString(DUTStr,StringLength(DUTStr),"';",0,-1);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 }


 // 6  PT-nummer
 WriteCOMPort(DUTCOM, ":FACT:SPGG:TEXT 6,'PT5210';");
 Delay(0.2);

 // 7  reserveret - ikke defineret.


return TRUE;
} // InitSPG


//----------------------------------------------------------------------------------

int BasicTotalTest(int Sel){



return TRUE;
}




//----------------------------------------------------------------------------------
int CVICALLBACK OkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:	Cont = TRUE; break;
	}	
		
return 0;
}



int CVICALLBACK SPGOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:	SPGdone = TRUE;
							if (TV == PAL)
							  SPGGenlPhasePALOk = TRUE;
							 else
 							  SPGGenlPhaseNTSCOk = TRUE;
 							break;
	}
	return 0;
}



int CVICALLBACK ExitSPGCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:	Cont = TRUE; 
							AutoHPH_PALOk = FALSE; 
							AutoHPH_NTSCOk = FALSE; 
 							GenlBOk = FALSE;
 							GenlA_BOk = FALSE;
 							GenlPALOk = FALSE;
 							GenlNTSCOk = FALSE;
 							Genl443MHzOk = FALSE;
 							Genl358MHzOk = FALSE;
 							Genl10MHzOk = FALSE;
 							GenlSDIOk = FALSE;
							SPGdone = TRUE;
							if (TV == PAL)
							  SPGGenlPhasePALOk = FALSE;
							 else
							  SPGGenlPhaseNTSCOk = FALSE;
							break;
		
		
	}
	return 0;
}



int CVICALLBACK SPGPilopCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(spgphasePNL,SPGPHA_PNL_SLIDE,&SPGGenlValue);
			SPGGenlValue = SPGGenlValue + 2;
			SetCtrlVal(spgphasePNL,SPGPHA_PNL_SLIDE,SPGGenlValue);
			SetSPGGenlockValue(SPGGenlValue + SPGGenlOffsetValue);
			break;
	}
	return 0;
}

int CVICALLBACK SPGPilnedCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(spgphasePNL,SPGPHA_PNL_SLIDE,&SPGGenlValue);
			SPGGenlValue = SPGGenlValue - 2;
			SetCtrlVal(spgphasePNL,SPGPHA_PNL_SLIDE,SPGGenlValue);
			SetSPGGenlockValue(SPGGenlValue + SPGGenlOffsetValue);
			break;
	}
	return 0;
}

int CVICALLBACK SPGSlideCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_VAL_CHANGED:
			GetCtrlVal(spgphasePNL,SPGPHA_PNL_SLIDE,&SPGGenlValue);
			SetSPGGenlockValue(SPGGenlValue + SPGGenlOffsetValue);
			break;
	}
	return 0;
}

int CVICALLBACK OCXOOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: done = TRUE;
			break;
	}
	return 0;
}

int CVICALLBACK OCXOExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: done = TRUE; F27Ok = FALSE; F10Ok = FALSE;
			break;
	}
	return 0;
}

