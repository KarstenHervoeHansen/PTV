/* FILE NAME   spgcal.c
 * PURPOSE      
 *             
 * SPEC       
 * DESIGN
 * CODING    
 * CODE INSP.
 * TEST     
 *            
 * MODIFIED:
 *
 * NOTES:       PT5210 rettelser:
 *             970306  Kommando GI? indf?rt: l?s SW version i SPG: 
               GI? returnerer SW ver fra version 020, tidligere udgaver
               returnerer intet. Her bruges GI til field 1/5 skift i PAL.
               Med SW ver 020 er GI erstattet af GJ og GJ erstattet af GK.

               970603  Test af synclevel rettes fra 150 mV til 180 mV (PAL), og
               fra 20 IRE til 24 IRE (NTSC) efter aftale med BL? og SNJ.
               Dett g?res for at undg? fejl, som kan optr?de pga af hysteresis
               (120-165 mV) i detekteringen af sync.
    
               970922  AutoHPH data l?ses og vises p? sk?rm ved fieldskift 1/5 i PAL og
               1/3 i NTSC. Dette har ingen indflydelse p? kalibreringen.
 

               981124  MakeSPGReset rettet s?ledes, at l?ste data fra SPG'en vises, 
               hvis reset af SPG fejler. Dette har ingen indflydelse p? kalibreringen.

               PT5201 rettelser:

 *             HP34970 switch channel select xyz:
 *             x = board no    1=20-ch-mux (hp34901)   2=dual 4-ch-rfmux (hp34905)
 *             hp34901:
 *             yz = relay no 1-20    
 *             hp34905:
 *             y = channel no    1=ch1       2=ch2
 *             z = relay no 1-4   
*/



#include "hp34970a.h"
#include "refosc.h"
#include <gpib.h>
#include "hp5313xa.h"
#include "refosc.h"
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
#include "pt5201.h"
#include "def.h"


// SPG har intern RS232-adresse = 15h = 21d
 
const double  GenlWaitInterval = 1.0,   // sec
          AutoHPHWaitInterval = 0.6;  // sec
const int NoOfGenlock = 7;            // antal gange der fors?ges genlocked
                            // se 'TestSPGGenlockSystem'

const double  FreqNom = 4.5e6,    // Hz
          FreqTol = 0.2;      // Hz

#if 0
// 27MHz og TCXO kalibrering
const double  TP30max = 0.0,      // max sp?nding i TP30 ved intern
              TP30min = -1.5;   // min sp?nding i TP30 ved intern
#else
// 27MHz og TCXO kalibrering   JK change
const double  TP30max =  1.5,      // max sp?nding i TP30 ved intern
              TP30min = -1.5;   // min sp?nding i TP30 ved intern
#endif
// TCXO kalibrering (bruges ikke)
const double  TP31max = 1.7,      // max sp?nding i TP31 ved intern
          TP31min = 1.3;      // min sp?nding i TP31 ved intern

const unsigned int hphminmaxdiff = 200; // max forskel ved HPH m?ling

const int NumberOfOkMeas = 5,   // Number of OK measurements when calibrating ref-osc
        MaxNumberOfMeas = 15; // Max number of measurements when calibrating ref-osc

int SPGdone,
    TV;
 
int GenlockStatus,  // TRUE hvis apparatet er l?st til extern ref
    FreqCalOk;      // Freq cal of 13MHz-TCXO or 27MHZ-ref-osc
int SPGGenlValue,
   SPGGenlOffsetValue;
  
int WriteSPGCalDate(void);
int MakeSPGGenlockCal(int TVSys);
int BasicTotalTest(int TestNo);
char *ReadSPGKUNumber (void);

//****************************************************************************
void WriteDacV74(int dacvalue){

 Fmt(DUTStr,"%s<:FACT:V24C:COMM 21,'GX%i';",dacvalue);  // SPG-address = 15h
 WriteCOMPort(DUTCOM, DUTStr);
#if DEBUG
 SetCtrlVal (ocxoPNL, OCXO_PNL_COMSTR, DUTStr);  // kun ved debug
#endif 

}


//*******************************************************************************
char *ReadSPGSWVer (void) {
  // read SPG SW-version from SPG-?P

 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:VERS?;");
 Delay(0.05);
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 1)
  CopyString(strread,0,strread,strlen(strread)-3,3);
  else
   Fmt(strread,"%s<???");

return strread;
}


//****************************************************************************
int GetGenlockStatus(void){
// status = tilstand af UNLOCKED LED l?ses i SPG'en
char *strread;
int gs;
 
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'GS?';");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 0)
  Scan(strread,"%s>%i",&gs);
  else
  gs = 0;
 gs = gs & 0x09;   // udmask bit 0,3    (bit 1,2,4-7=0)
 
return gs;
} // GetGenlockStatus
 

//----------------------------------------------------------------------------------
int WriteSPGCalDate(void){

 GetDate(2);
 Fmt(DUTStr,"%s<:FACT:GENL:DATE %s,%s,%s;",YearStr,MonthStr,DayStr);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.05);

return TRUE;
}





//****************************************************************************
int TestSPGGenlockSystem(void){
/* 
 TEST 1,2,3 er en test af genlocksystemet med f?lgende signaler: 
 PAL, NTSC og 10MHz.
 
 For PAL og NTSC genlock testes om der genlockes ved -6dB med noise og brum.
 Frekvenssignalet 10MHz d?mpes med et d?mpeled monteret i genlock mux'en (i BNC-stik panalet).
 Genlocksignalerne for PAL og NTSC programmeres med PM5640.
 
*/


// 1. TEST   Genlock PAL --------------------------------

int button_pressed;
int handle_returned, controlID_returned;
int fhandle,
    bytes_written,
    n;

 GenlPALOk = TRUE;
 GenlNTSCOk = TRUE;
 Genl10MHzOk = TRUE;
 GenlOk = TRUE;

 SetPanelAttribute (spggenlPNL, ATTR_TITLE, " Genlock Test ");
 ResetTextBox(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," PT5210 GENL SIGNAL forbindes til DUT GENLOCK A\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," DUT GENLOCK B forbindes til PM5662 CH-A\n\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Forts?t:  Tast F4\n");
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," Afbryd:   Tast F9\n\n");
 DisplayPanel (spggenlPNL);

 WaitForContinue();
 if (GenlOk == FALSE){
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  return FALSE;
 }

 
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (spggenlPNL);
  GenlOk = FALSE;
  return FALSE;
  }

 WriteCOMPort(DUTCOM,":INP:GENL:SYST PALB;");
 Set5640Standard("G");
 Set5640Pattern("G", 290);




// PAL -------- Hum, Noise, Burst=150mV, Sync=180mV --------------------------------------------
 Set5640BurstLevel("G", 300 * 0.5);
 Set5640SyncLevel("G", 300 * 0.6);
 Set5640Hum("G", 500);
 Set5640Noise("G", -40);
 SetGenlockSignal(GENL_PAL);
 Delay(1.0);
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," PAL:   Burst=150mV  Sync=180mV  Hum=500mV   Noise=-40dB  ");
 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < NoOfGenlock)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 

 
 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"  OK\n");

 
 if (GenlockStatus != DUT_LOCKED){
  Set5640Standard("G");
  GenlPALOk = FALSE;
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"\n  PAL genlock fejlet \n");
  //MessagePopup (" Genlock fejl"," PAL genlock fejlet ");
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  Set5640Standard("G");
  Set5640Standard("M");
  return FALSE;
 }

// PAL -------- Hum, Noise, Burst=600mV, Sync=600mV --------------------------------------------
 Set5640BurstLevel("G", 300 * 2.0);
 Set5640SyncLevel("G", 300 * 2.0);
 Set5640Hum("G", 500);
 Set5640Noise("G", -40);
 SetGenlockSignal(GENL_PAL);
 Delay(1.0);
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," PAL:   Burst=600mV  Sync=600mV  Hum=500mV   Noise=-40dB  ");
 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < NoOfGenlock)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while
 

 
 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"  OK\n\n");

 
 if (GenlockStatus != DUT_LOCKED){
  Set5640Standard("G");
  GenlPALOk = FALSE;
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"\n  PAL genlock fejlet \n");
  //MessagePopup (" Genlock fejl"," PAL genlock fejlet ");
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  Set5640Standard("G");
  Set5640Standard("M");
  return FALSE;
 }






// 2. TEST    NTSC -------- Hum, Noise, Burst, Sync - 6dB --------------------------------
 WriteCOMPort(DUTCOM,":INP:GENL:SYST NTSC;");
 Set5640Standard("M");
 Set5640Pattern("M", 290);

 Set5640BurstLevel("M", 40 * 0.5);
 Set5640SyncLevel("M", 40 * 0.6);
 Set5640Hum("M", 50);
 Set5640Noise("M", -40);
 SetGenlockSignal(GENL_NTSC);
 Delay(1.0);
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," NTSC:  Burst=20IRE  Sync=24IRE  Hum=50IRE   Noise=-40dB  ");
 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < NoOfGenlock)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"  OK\n");


 if (GenlockStatus != DUT_LOCKED){
  Set5640Standard("M");
  GenlNTSCOk = FALSE;
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"\n  PAL genlock fejlet \n");
  //MessagePopup (" Genlock fejl"," Genlock NTSC fejlet ");
  Delay(2.0);
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  Set5640Standard("G");
  Set5640Standard("M");
  return FALSE;
 }


// NTSC -------- Hum, Noise, Burst, Sync + 6dB --------------------------------
 WriteCOMPort(DUTCOM,":INP:GENL:SYST NTSC;");
 Set5640Standard("M");
 Set5640Pattern("M", 290);

 Set5640BurstLevel("M", 40 * 2.0);
 Set5640SyncLevel("M", 40 * 2.0);
 Set5640Hum("M", 50);
 Set5640Noise("M", -40);
 SetGenlockSignal(GENL_NTSC);
 Delay(1.0);
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," NTSC:  Burst=80IRE  Sync=80IRE  Hum=50IRE   Noise=-40dB  ");
 
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < NoOfGenlock)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"  OK\n\n");


 if (GenlockStatus != DUT_LOCKED){
  Set5640Standard("M");
  GenlNTSCOk = FALSE;
  MessagePopup (" Genlock fejl"," Genlock NTSC fejlet ");
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  Set5640Standard("G");
  Set5640Standard("M");
  return FALSE;
 }





// 3. TEST   Genlock 10 MHz --- 1.5Vpp ----------------------------------------------
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," 10 MHz: Amplitude = 1.5Vpp i 75ohm  ");
 SetGenlockSignal(GENL_10M_HIGH);
 
 WriteCOMPort(DUTCOM,":INP:GENL:SYST F10MHZ;");

 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < NoOfGenlock)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while

 
 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"  OK\n");
 

 if (GenlockStatus != DUT_LOCKED){
  Genl10MHzOk = FALSE;
  MessagePopup (" Genlock fejl"," Genlock 10 MHz fejlet ");
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  Set5640Standard("G");
  Set5640Standard("M");
  return FALSE;
 }

 
// 10 MHz -------- 0.5Vpp ---------------------------------------------------------
 SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," 10 MHz: Amplitude = 0.5Vpp i 75ohm  ");
 SetGenlockSignal(GENL_10M_LOW);

 Delay(1.0);
 n = 0;
 GenlockStatus = GetGenlockStatus();
 
 while ((GenlockStatus != DUT_LOCKED) & (n < NoOfGenlock)){
  GenlockStatus = GetGenlockStatus();
  n++;
  Delay(GenlWaitInterval);
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX," *");
  ProcessDrawEvents();
 }//while

 if (GenlockStatus == DUT_LOCKED)
  SetCtrlVal(spggenlPNL, SPGGEN_PNL_GENLTXTBOX,"  OK\n");
 

 if (GenlockStatus != DUT_LOCKED){
  Genl10MHzOk = FALSE;
  MessagePopup (" Genlock fejl"," Genlock 10 MHz fejlet ");
  HidePanel(spggenlPNL);
  SetGenlockSignal(GENL_OFF);
  Set5640Standard("G");
  Set5640Standard("M");
  return FALSE;
 }


 SetGenlockSignal(GENL_OFF);


 // check samlet genlock test
 GenlOk = GenlPALOk && GenlNTSCOk && Genl10MHzOk;
 
 SetGenlockSignal(GENL_OFF);
 Set5640Standard("G");
 Set5640Standard("M");

 if (GenlOk == TRUE)
  MessagePopup (" Genlock Test"," OK");
 
 HidePanel(spggenlPNL);
   
   
   
return TRUE;
} //MakeSPGGenlockSystem






/***************************************************************************/
/* int MakeSPGHPHCal(void)                                        spgcal.C */
/*                                                                         */
/* Author:   JK                                                            */
/* Revised:  31.01.2003                                                    */
/*                                                                         */
/* Function: Calibration of SPG HPH                                        */
/* Remarks:  Optimized for PT5202, with V74 DAC compensation               */
/* Returns:  Nothing                                                       */
/* Updates:    -                                                           */
/***************************************************************************/
int MakeSPGHPHCal(void)
  {
  int handle_returned, controlID_returned,
      svar, button_pressed,n, flag;
  unsigned int hph[20],
               hh,
               hphmean,
               hphmax,
               hphmin,
               hphdiff,
               hphreadg,
               hphreadm;
  short int    compens;               
  char         MsgStr[150];

 DisplayPanel (spghphPNL);
 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"");

 Delay(0.1);
 SelectPM3094Setup(1);
 SetCtrlVal (spghphPNL, SPGHPH_PNL_SCOPSETUP, " PM3094: FRONTS = F1");

 ProcessDrawEvents();
 
 
 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (spghphPNL);
  AutoHPH_PALOk = FALSE;
  AutoHPH_PALOk = FALSE;
  return FALSE;
 }
  

 InitPT5201();


 AutoHPH_PALOk = TRUE;
 AutoHPH_NTSCOk = TRUE;

//goto ntsc;  // debug NTSC auto hph zero

 // Auto HPH for PAL --------------------------------
 

 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 ResetTextBox(spghphPNL, SPGHPH_PNL_MINDATA,"");
 ResetTextBox(spghphPNL, SPGHPH_PNL_MAXDATA,"");
 ResetTextBox(spghphPNL, SPGHPH_PNL_MEANDATA,"");
 ResetTextBox (spghphPNL, SPGHPH_PNL_READHPH, "");


 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Forbind PT5210 BB til DUT GENLOCK\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Forbind DUT GENLOCK med scop CH-1\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Forbind DUT BB1 med scop CH-2\n\n\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," OBS: Scop CH-1 og CH-2 skal v?re afsluttet i 75 ohm\n\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Forts?t:  Tast F4\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Afbryd:   Tast F9\n");
 SetCtrlAttribute (spghphPNL, SPGHPH_PNL_MEASDATA, ATTR_LABEL_TEXT, "DAC V74 Value       " );
                                                                                             
 
 WaitForContinue();
 if (AutoHPH_PALOk == FALSE){
  HidePanel(spghphPNL);
  return FALSE;
 }


 // Set black-burst from PT5210 = PAL_ID
 Delay(0.03);
 WriteCOMPort(PT5210COM,":OUTP:BB1:SYST PAL_ID;");Delay(0.01);
 WriteCOMPort(PT5210COM,":OUTP:BB1:SCHP 0;");Delay(0.01);     // ScH phase = 0

 // Set DUT genlock = PAL burst
 WriteCOMPort(DUTCOM,":INP:GENL:SYST PALB;");Delay(0.01);
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HX0';");Delay(0.01);// Set calib. phase G
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HK0';");Delay(0.01);// Set G-7 system and phase
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HT0';");Delay(0.01);// Set calib ScH phase G
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HH0';");Delay(0.01);// Set ScH phase
 
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HK1105862260';"); // BB1 with PAL_ID ?????????

 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Genlock=PAL burst         BB1=PAL_ID\n");
 Delay(1);
 
 
 // Check at DUT er genlocked
 n = 0;
 GenlockStatus = GetGenlockStatus();
 while ((GenlockStatus != DUT_LOCKED) & (n < 10))
   {
   GenlockStatus = GetGenlockStatus();
   SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," *");
   n++;
   Delay(GenlWaitInterval);
   ProcessDrawEvents();
   }//while
 
 if (GenlockStatus != DUT_LOCKED)
   {
   Fmt(MsgStr,"%s<PAL genlock ikke l?st\nHvis apparatet er nyt, skal SPG resettes");
   MessagePopup ("Genlock fejl",MsgStr);
   HidePanel(spghphPNL);
   return FALSE;
   }
#if 1
// ********* SPG DAC V74 offset compensation (PAL only) ***********
 
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," DAC V74 offset compensation...\n");

 WriteCOMPort(DUTCOM,"COMM 21,'GB0';"); // Write 0 to h_dacv74_offset first
 Delay(AutoHPHWaitInterval);  

 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'MV?';"); // Read DAC V74
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Delay(AutoHPHWaitInterval);    
/*
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'MV?';"); // Read DAC V74
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Delay(AutoHPHWaitInterval);    

 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'MV?';"); // Read DAC V74
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Delay(AutoHPHWaitInterval);    
*/
 for (n = 0; n < 7; n++) // get 7 readings
   {
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_PALOk == FALSE) 
     {
     HidePanel(spghphPNL); 
     return FALSE;
     }
   FlushInQ(DUTCOM);
   WriteCOMPort(DUTCOM,"COMM 21,'MV?';"); //  Read DAC V74
   Delay(AutoHPHWaitInterval);        
   strread = ReadlnCOMPort(DUTCOM,0.5);
   Scan(strread,"%s>%i[u]",&hh);
   Fmt(MsgStr,"%s<%i[u]\n",hh);
   SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
   hph[n] = hh;
   } // for

  // find min
  hphmin = 0xffffffff;
  for (n = 0; n < 7; n++) 
    {
    if (hph[n] < hphmin)
    hphmin = hph[n];
    }

  // find max
  hphmax = 0x0;
  for (n = 0; n < 7; n++) 
    {
    if (hph[n] > hphmax)
    hphmax = hph[n];
    }
 
  hphdiff = hphmax - hphmin;
 
  // calculate mean
  hphmean = 0;
  for (n = 0; n < 7; n++) 
    hphmean = hphmean + hph[n];
  hphmean = hphmean / 7;

#if 1
  Fmt(MsgStr,"%s<%i[u]",hphmin);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,MsgStr);
  Fmt(MsgStr,"%s<%i[u]",hphmax);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,MsgStr);
  Fmt(MsgStr,"%s<%i[u]",hphmean);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,MsgStr);
#endif

  // find required offset (compensation)
 compens = hphmean - 2048;
 if (compens < 0)
   compens = 0xffff + compens;

 Fmt(DUTStr,"%s<COMM 21,'GB%i[u]';",compens); // set h_dacv74_offset
 WriteCOMPort(DUTCOM,DUTStr);   // 
 Delay(AutoHPHWaitInterval);
 Delay(AutoHPHWaitInterval);  
 

 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 ResetTextBox(spghphPNL, SPGHPH_PNL_MINDATA,"");
 ResetTextBox(spghphPNL, SPGHPH_PNL_MAXDATA,"");
 ResetTextBox(spghphPNL, SPGHPH_PNL_MEANDATA,"");
 ResetTextBox (spghphPNL, SPGHPH_PNL_READHPH, "");


// Now read 7 times from DAC and calculate necessary things again

 for (n = 0; n < 7; n++) // get 7 readings
   {
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_PALOk == FALSE) 
     {
     HidePanel(spghphPNL); 
     return FALSE;
     }
   FlushInQ(DUTCOM);
   WriteCOMPort(DUTCOM,"COMM 21,'MV?';"); //  Read DAC V74
   Delay(AutoHPHWaitInterval);        
   strread = ReadlnCOMPort(DUTCOM,0.5);
   Scan(strread,"%s>%i[u]",&hh);
   Fmt(MsgStr,"%s<%i[u]\n",hh);
   SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
   hph[n] = hh;
   } // for

  // find min
  hphmin = 0xffffffff;
  for (n = 0; n < 7; n++) 
    {
    if (hph[n] < hphmin)
    hphmin = hph[n];
    }

  // find max
  hphmax = 0x0;
  for (n = 0; n < 7; n++) 
    {
    if (hph[n] > hphmax)
    hphmax = hph[n];
    }
 
  hphdiff = hphmax - hphmin;
 
  // calculate mean
  hphmean = 0;
  for (n = 0; n < 7; n++) 
    hphmean = hphmean + hph[n];
  hphmean = hphmean / 7;

#if 1
  Fmt(MsgStr,"%s<%i[u]",hphmin);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,MsgStr);
  Fmt(MsgStr,"%s<%i[u]",hphmax);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,MsgStr);
  Fmt(MsgStr,"%s<%i[u]",hphmean);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,MsgStr);
#endif



 // Now check that dac_offsetmax is not bigger than 2048 + 10 and
 // that dac_offset_min is not less            than 2048 - 10

#if 1
  if ((hphmax > 2058) || (hphmin < 2038)) 
   {
   Fmt(MsgStr,"%s< DAC vaerdien er udernfor gransen = +- 10: ");
   Fmt(MsgStr,"%s[a]<)\n Kalibrering stoppet");
   MessagePopup (" SPG Kalibreringsfejl: Auto HPH for PAL", MsgStr);
   AutoHPH_PALOk = FALSE;
   HidePanel(spghphPNL);
   return FALSE;
   }
#endif
 // Now write compensation value to NOVRAM using FACTORY command
 
  Fmt(DUTStr,"%s<:FACT:GENL:DOFFSET %i[ub2];",compens);  
  WriteCOMPort(DUTCOM, DUTStr);
   
   
#endif


//****************** END **************

 SetCtrlAttribute (spghphPNL, SPGHPH_PNL_MEASDATA, ATTR_LABEL_TEXT, "Auto HPH value:         " );
 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"\n");
 // setup for internal commands
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'GA';"); // Auto HPH G

 Delay(1);
 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 FlushInQ(DUTCOM);
 
 // disse 3 m?linger gemmes ikke
 for (n = 0; n < 3; n++) 
   {
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_PALOk == FALSE) 
     {
     HidePanel(spghphPNL); 
     return FALSE;
     }

   Delay(0.03);
   WriteCOMPort(DUTCOM,"COMM 21,'GA';");  // Auto HPH G
   Delay(AutoHPHWaitInterval);
   FlushInQ(DUTCOM);
   WriteCOMPort(DUTCOM,"COMM 21,'GA?';"); // Read HPH-zero G ?????
   strread = ReadlnCOMPort(DUTCOM,0.5);
   Scan(strread,"%s>%i[u]",&hh);
   Fmt(MsgStr,"%s<%i[u]",hh);
   Fmt(MsgStr,"%s[a]<\n");
   SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
   } // for...
 

 // disse 7 m?linger gemmes og middelv?rdi beregnes
 // What do these 7 measurements represent????
 flag = FALSE;
 for (n = 0; n < 7; n++) 
   {
   GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_PALOk == FALSE) 
     {
     HidePanel(spghphPNL); 
     return FALSE;
     }
  
   Delay(0.03);
   WriteCOMPort(DUTCOM,"COMM 21,'GA';");  // Auto HPH G
   Delay(AutoHPHWaitInterval);            // Wait ???
   FlushInQ(DUTCOM);
   WriteCOMPort(DUTCOM,"COMM 21,'GA?';"); // Read HPH-zero G
   strread = ReadlnCOMPort(DUTCOM,0.5);
   Scan(strread,"%s>%i[u]",&hh);
   Fmt(MsgStr,"%s<%i[u]\n",hh);
   SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
   if ((hh & 0xff000000) == 0xff000000)
     {
     hh = hh & 0x00ffffff;   // clear 8 MSB bits
     flag = TRUE;
     } 
   hph[n] = hh;
   } // for
 
  // find hph min
  hphmin = 0xffffffff;
  for (n = 0; n < 7; n++) 
    {
    if (hph[n] < hphmin)
    hphmin = hph[n];
    }
  if (flag)
    hphmin = hphmin | 0xff000000;   // set 8 MSB bits

  // find hph max
  hphmax = 0x0;
  for (n = 0; n < 7; n++) 
    {
    if (hph[n] > hphmax)
    hphmax = hph[n];
    }
  if (flag)
    hphmax = hphmax | 0xff000000;   // set 8 MSB bits
 
  hphdiff = hphmax - hphmin;
 
 
  // calculate mean
  hphmean = 0;
  for (n = 0; n < 7; n++) 
    hphmean = hphmean + hph[n];
  hphmean = hphmean / 7;
  if (flag)
    hphmean = hphmean | 0xff000000;   // set 8 MSB bits in PT5210/5230 only

  Fmt(MsgStr,"%s<%i[u]",hphmin);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,MsgStr);
  Fmt(MsgStr,"%s<%i[u]",hphmax);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,MsgStr);
  Fmt(MsgStr,"%s<%i[u]",hphmean);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,MsgStr);

  if (hphdiff > hphminmaxdiff) 
   {
   Fmt(MsgStr,"%s< Forskel mellem Max og Min er for stor (max forskel: ");
   Fmt(MsgStr,"%s[a]<%i",hphminmaxdiff);
   Fmt(MsgStr,"%s[a]<)\n Kalibrering stoppet");
   MessagePopup (" SPG Kalibreringsfejl: Auto HPH for PAL", MsgStr);
   AutoHPH_PALOk = FALSE;
   HidePanel(spghphPNL);
   return FALSE;
   }
 
 
 
  //AutoHPH_PALOk = TRUE;

  if (AutoHPH_PALOk == TRUE) 
    {
    Fmt(DUTStr,"%s<COMM 21,'GV%i[u]';",hphmean); // WRITE TO DUT: Set HPH-zero G
    WriteCOMPort(DUTCOM,DUTStr);   // 
    Delay(0.3);
    FlushInQ(DUTCOM);
    WriteCOMPort(DUTCOM,"COMM 21,'GA?';");  // Read HPH-zero G
    strread = ReadlnCOMPort(DUTCOM,0.5);
    Scan(strread,"%s>%i[u]",&hh);
    if (hphmean != hh)
      AutoHPH_PALOk = FALSE;                 // if different, AutoHPH_PALOk is false
    } 
 
 //ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");

  if (AutoHPH_PALOk == FALSE) 
    {
    MessagePopup (" SPG: Auto HPH kalibrering for PAL fejlet", " Middelv?rdi ikke gemt korrekt i SPG\n Kalibrering stoppes");
     ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
     SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,"");
     SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,"");
     SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,"");
     SetCtrlVal (spghphPNL, SPGHPH_PNL_READHPH, "");
    HidePanel(spghphPNL);
    return FALSE;
    }


  ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"\n");
  SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Linie 7 puls i CH-1 skal v?re i samme field som linie 7 puls i CH-2\n");
  SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Field 1/5 kan skiftes med tasten 'Skift field 1/5'\n");
 
  Repeat = TRUE;
  while (Repeat == TRUE)
    {
    button_pressed = GenericMessagePopup ("SPG kalibrering:  Field 1/5 for PAL",
                      "OK?", "OK", "Afvist", "Skift field 1/5",
                      0, -1, 0,
                      VAL_GENERIC_POPUP_BTN1,
                      VAL_GENERIC_POPUP_BTN2,
                      VAL_GENERIC_POPUP_BTN3);
    switch (button_pressed) 
      {
      case 1: 
        AutoHPH_PALOk = TRUE; 
        Repeat = FALSE; 
        break;
      case 2: 
        AutoHPH_PALOk = FALSE; 
        Repeat = FALSE; 
        break;
      case 3:
        WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'GJ';");  // frame invert G
        Delay(0.03);
        FlushInQ(DUTCOM);
        WriteCOMPort(DUTCOM,"COMM 21,'GA?';"); // Read HPH-zero G
        strread = ReadlnCOMPort(DUTCOM,0.5);
        Scan(strread,"%s>%i[u]",&hphreadg);
        Delay(0.03);
        FlushInQ(DUTCOM);
        WriteCOMPort(DUTCOM,"COMM 21,'GU?';"); // Read HPH-zero M
        strread = ReadlnCOMPort(DUTCOM,0.5);
        Scan(strread,"%s>%i[u]",&hphreadm);
        Fmt(MsgStr,"%s<GA?=%i[u]  GU?=%i[u]",hphreadg,hphreadm); // GU? thats for M????
        SetCtrlVal (spghphPNL, SPGHPH_PNL_READHPH, MsgStr);
        break;
      }
    } //while
 
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,"");
 SetCtrlVal (spghphPNL, SPGHPH_PNL_READHPH, "");
 
   // if OK   store in NVRAM after hph-NTSC-calibration
 

  if (AutoHPH_PALOk == FALSE) 
    {
    HidePanel(spghphPNL);
    return FALSE;
    }


ntsc: // label

 
 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,"");
 SetCtrlVal (spghphPNL, SPGHPH_PNL_READHPH, "");


 
 
 // Auto HPH for NTSC --------------------------------
 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"\n");

 Delay(0.1);
 SelectPM3094Setup(2);
 SetCtrlVal (spghphPNL, SPGHPH_PNL_SCOPSETUP, " PM3094: FRONTS = F2");
   
 // Set black-burst from PT5210 = NTSC
 WriteCOMPort(PT5210COM,":OUTP:BB1:SYST NTSC; SCHP 0;");
 Delay(0.03);

 // Set DUT BB1 = NTSC with ID in line 11
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HY0';");Delay(0.01);// h-phase
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HM0';");Delay(0.01);
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HU0';");Delay(0.01);// sch-phase
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HH0';");Delay(0.01);
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HM230572640';");  // BB1 with NTSC
 Delay(0.01);
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HN1';");  //ID-pulse in linie 11
 Delay(0.01);

 // Set DUT genlock to input-AB NTSC burst
 WriteCOMPort(DUTCOM,":INP:GENL:SYST NTSC;");

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
  Fmt(MsgStr,"%s<NTSC genlock ikke l?st    Status=%x (hex)",GenlockStatus);
  MessagePopup ("Genlock fejl",MsgStr);
  HidePanel(spghphPNL);
  return FALSE;
 }


// setup for internal commands
 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'GU';");

 Delay(1);
 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 FlushInQ(DUTCOM);
 
 // disse 3 m?linger gemmes ikke
 for (n = 0; n < 3; n++) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_NTSCOk == FALSE) {
    HidePanel(spghphPNL); 
    return FALSE;
   }
  Delay(0.03);
  WriteCOMPort(DUTCOM,"COMM 21,'GU';"); 
  Delay(AutoHPHWaitInterval);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 21,'GU?';"); 
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  Fmt(MsgStr,"%s<%i[u]\n",hh);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
 }
 

 // disse 7 m?linger gemmes og middelv?rdi beregnes
 flag = FALSE;
 for (n = 0; n < 7; n++) {
  GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
   if (AutoHPH_NTSCOk == FALSE) {
    HidePanel(spghphPNL); 
    return FALSE;
   }
  Delay(0.03);
  WriteCOMPort(DUTCOM,"COMM 21,'GU';");
  Delay(AutoHPHWaitInterval);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 21,'GU?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  Fmt(MsgStr,"%s<%i[u]\n",hh);
  SetCtrlVal(spghphPNL, SPGHPH_PNL_MEASDATA,MsgStr);
  if ((hh & 0xff000000) == 0xff000000){
   hh = hh & 0x00ffffff;   // clear 8 MSB bits
   flag = TRUE;
  }
   hph[n] = hh;
 }
 
 // find hph min
 hphmin = 0xffffffff;
 for (n = 0; n < 7; n++) {
  if (hph[n] < hphmin)
   hphmin = hph[n];
 }
 if (flag)
  hphmin = hphmin | 0xff000000;   // set 8 MSB bits

 // find hph max
 hphmax = 0x0;
 for (n = 0; n < 7; n++) {
  if (hph[n] > hphmax)
   hphmax = hph[n];
 }
 if (flag)
  hphmax = hphmax | 0xff000000;   // set 8 MSB bits
 
 hphdiff = hphmax - hphmin;

 
 // calculate mean
 hphmean = 0;
 for (n = 0; n < 7; n++) 
   hphmean = hphmean + hph[n];
 hphmean = hphmean / 7;
 if (flag)
  hphmean = hphmean | 0xff000000;   // set 8 MSB bits

 Fmt(MsgStr,"%s<%i[u]",hphmin);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,MsgStr);
 Fmt(MsgStr,"%s<%i[u]",hphmax);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,MsgStr);
 Fmt(MsgStr,"%s<%i[u]",hphmean);
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,MsgStr);
 
 if (hphdiff > hphminmaxdiff) {
  Fmt(MsgStr,"%s< Forskel mellem Max og Min er for stor (max forskel: %i)\n Kalibrering stoppet",hphminmaxdiff);
  MessagePopup (" SPG Kalibreringsfejl: Auto HPH for NTSC", MsgStr);
  AutoHPH_NTSCOk = FALSE;
  HidePanel(spghphPNL);
  WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HN0';"); //No ID-pulse in linie 11
  return FALSE;
 }

 AutoHPH_NTSCOk = TRUE;

 if (AutoHPH_NTSCOk == TRUE) {
  Fmt(DUTStr,"%s<COMM 21,'GY%i[u]';",hphmean);
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.3);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 21,'GU?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hh);
  if (hphmean != hh)
   AutoHPH_NTSCOk = FALSE;
 } 
 

  if (AutoHPH_NTSCOk == FALSE) {
   MessagePopup (" SPG: Auto HPH kalibrering for NTSC fejlet","Middelv?rdi ikke gemt korrekt i SPG\n Kalibrering stoppes");
   HidePanel(spghphPNL);
  WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HN0';"); //No ID-pulse in linie 11
   return FALSE;
  }


 ResetTextBox(spghphPNL, SPGHPH_PNL_TXTBOX,"\n");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_TXTBOX," Linie 11 puls i CH-1 skal v?re i samme field som linie 11 puls i CH-2\n");
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
    case 3: WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'GK';");  // invert M-frame 
            Delay(0.03);
            FlushInQ(DUTCOM);
            WriteCOMPort(DUTCOM,"COMM 21,'GA?';");
            strread = ReadlnCOMPort(DUTCOM,0.5);
            Scan(strread,"%s>%i[u]",&hphreadg);
            Delay(0.03);
            FlushInQ(DUTCOM);
            WriteCOMPort(DUTCOM,"COMM 21,'GU?';");
            strread = ReadlnCOMPort(DUTCOM,0.5);
            Scan(strread,"%s>%i[u]",&hphreadm);
            Fmt(MsgStr,"%s<GA?=%i[u]  GU?=%i[u]",hphreadg,hphreadm);
        SetCtrlVal (spghphPNL, SPGHPH_PNL_READHPH, MsgStr);
        break;
   }

 } //while
 
 ResetTextBox(spghphPNL, SPGHPH_PNL_MEASDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MINDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MAXDATA,"");
 SetCtrlVal(spghphPNL, SPGHPH_PNL_MEANDATA,"");


 // if OK store hph-zero values for PAL and NTSC in NVRAM
 if (AutoHPH_NTSCOk == TRUE){
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 21,'GA?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hphreadg);
  Delay(0.03);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,"COMM 21,'GU?';");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%i[u]",&hphreadm);
  Delay(0.03);

  
  Fmt(DUTStr,"%s<:FACT:GENL:HPHZ %i[u],%i[u];",hphreadg,hphreadm);  // save hph zero in NVRAM
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.03);
  WriteCOMPort(DUTCOM,":FACT:GENL:PHAS 28810,28810;");            // save G/M-phases=0 in NVRAM
  Delay(0.03);

  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:GENL:HPHZ?;"); // read from NVRAM
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%2i[ux]",hph);
  
  if ((hphreadg != hph[0]) || (hphreadm != hph[1])){  // compare read NVRAM-values with saved values
    Fmt(MsgStr,"%s<PAL:    Gemt=%i[u]  L?st=%i[u]\nNTSC: Gemt=%i[u]  L?st=%i[u]",hphreadg,hph[0],hphreadm,hph[1]);
     MessagePopup (" Auto HPH kalibreringsfejl",MsgStr);
    AutoHPH_PALOk = AutoHPH_NTSCOk = FALSE;
   WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HN0';");  //No ID-pulse in linie 11
   HidePanel (spghphPNL);
    return FALSE;
  }
  else{

    WriteSPGCalDate();
   WriteFMSUpdate();
   
   kustr =  ReadKUStr();
   
    dato = GetDate(1);  // get system date YYMMDD
   
    tt = TimeStr();   // get system time
  
  // check om logfil findes, ellers opret en ny logfil. LogDest angiver drev+sti til logfil.
  fhandle = OpenFile (SPGLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
  Fmt(wrtbuf,"%s<%s,%s,%s,%i[u],%i[u]\n",dato,tt,kustr,hphreadg,hphreadm);
  bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
  CloseFile(fhandle);
  
  
  }

 } 

 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 52,'HN0';");  //No ID-pulse in linie 11


 HidePanel (spghphPNL);
 
return AutoHPH_NTSCOk;
} //MakeSPGHPHCal







//*****************************************************************************
void SetSPGGenlockValue(int genlval){ // bruges kun PT5210/5230
char SPGGenlStr[11];

 if (TV == PAL)
   Fmt(DUTStr,"%s<COMM 21,'GF%i';",genlval);
  else
   Fmt(DUTStr,"%s<COMM 21,'GG%i';",genlval);

 WriteCOMPort(DUTCOM,DUTStr);
 Delay(0.05);
} //SetSPGGenlockValue







//*****************************************************************************
int MakeRefCal(int Freq){   // Freq = FREQ27 or FREQ13
/*  Kalibrering af 27MHz VCXO-ref eller 13MHz TCXO-ref
  Frekvensen m?les som 4.5MHz fra BB1-output
  Check sp?nding i TP30 ved kalibrering af 13MHz TCXO-ref og 27MHz-ref

  Ved m?ling af 4.5MHz opsamles 3 m?linger, som alle er indenfor tolerancen.
  Af disse 3 m?linger findes den m?ling som har den mindste fejl.
*/  

char  TPTxt[30];
ViChar mm[30];
int handle_returned, controlID_returned;
int V74dac, MeasInsideTol, NumberOfMeas, 
    TPVoltOk, Step, dacread, dac[3], n, minno;
ViReal64 freqmeas1,freqmeas2;
double diff, tpmeas, tpmax, tpmin, HzPerDacStep,freqerr[3], minerr;



 FreqCalOk = TRUE;

 if (CheckHP53132ExtRef() == FALSE){
   HidePanel(ocxoPNL);
   FreqCalOk = F13Ok = F27Ok = FALSE;
   return FALSE;
   } 
   
   
 if (GPSStatusOk == FALSE){      // see  CheckXLDCRef(void) in cviutil.c
   Fmt(MsgStr,"%s< TrueTime GPS modtager virker ikke korrekt:\n %s",GPSStatusStr);
   MessagePopup (" Fejl ved reference til counter HP53132 ",MsgStr);
   HidePanel(ocxoPNL);
   FreqCalOk = F13Ok = F27Ok = FALSE;
   return FALSE;
 }
   

 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (ocxoPNL);
  FreqCalOk = F13Ok = F27Ok = FALSE;
  return FALSE;
  }
 
 ResetTextBox(ocxoPNL, OCXO_PNL_TXTBOX,"");
 DisplayPanel(ocxoPNL);

 if (Freq == FREQ27){
  tpmax = TP30max;
  tpmin = TP30min;
  Fmt(TPTxt,"%s<TP30:  %f[p1] - %f[p1] Volt",tpmin,tpmax);
   SetPanelAttribute (ocxoPNL, ATTR_TITLE, " 27 MHz VCXO ref kalibrering ");
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Forbind kabel fra HP53132 CH-A til BB1\n");
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Forbind DMM-probe til TP30\n\n");
  Fmt(MsgStr," Sp?ndingen i TP30 skal v?re mellem %f[p2] og %f[p2] VDC\n\n",tpmin,tpmax);
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX,MsgStr);
   F27Ok = TRUE;
  } 
 else 
  if (Freq == FREQ13){
  tpmax = TP30max;
  tpmin = TP30min;
  Fmt(TPTxt,"%s<TP30:  %f[p1] til %f[p1] V",tpmin,tpmax);
   SetPanelAttribute (ocxoPNL, ATTR_TITLE, " 13 MHz TCXO ref kalibrering ");
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Forbind kabel fra HP53132 CH-A til BB1\n");
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Forbind DMM-probe til TP30 (eller X13 pin 9)\n\n");
  Fmt(MsgStr," Sp?ndingen i TP30 skal v?re mellem %f[p2] og %f[p2] VDC\n",tpmin,tpmax);
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX,MsgStr);
  Fmt(MsgStr," Frekvens fra BB1: %f[p1] ? %f[p1] Hz\n\n",FreqNom,FreqTol);
   SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX,MsgStr);
   F13Ok = TRUE;
  } 

 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," HUSK: Apparatet skal have v?ret t?ndt i MINDST 15 minutter\n\n");
 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Forts?t:  Tast F4\n");
 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," Afbryd:   Tast F9\n\n");

 SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MAX_VALUE, 500.0);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MIN_VALUE, -500.0);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_VCOVOLT, ATTR_MAX_VALUE, tpmax + 0.4);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_VCOVOLT, ATTR_MIN_VALUE, tpmin - 0.4);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_VCOVOLT, ATTR_VISIBLE, TRUE);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_DEC1, ATTR_VISIBLE, TRUE);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_VCOVOLT, ATTR_LABEL_TEXT, TPTxt);
 Fmt(MsgStr,"%s< ? %f[p1] Hz",FreqTol);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_LABEL_TEXT, MsgStr);
 SetCtrlAttribute (ocxoPNL, OCXO_PNL_FREQ, ATTR_TEXT_COLOR, VAL_RED);


 WriteCOMPort(DUTCOM,":INP:GENL:SYST INT;");
 Delay(0.01);
 WriteCOMPort(DUTCOM,":OUTP:BB1:SYST PAL;");
 Delay(0.01);
 WriteCOMPort(DUTCOM,":FACT:BB1:SIGN F45MHZ;"); // BB1 outputs 4.5MHz
 

 WaitForContinue();
 if (FreqCalOk == FALSE)
   {
   HidePanel(ocxoPNL);
   return FALSE;
   }

 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," M?linger ");

#if IEEEBUS
 hp34970a_voltageMeasure (hp34970, "102", HP34970A_VM_VTYPE_DC,
                  HP34970A_VM_VRANGE_10V,
                  HP34970A_VM_RES_5_5DIG, mm);

 hp34970a_voltageSetup (hp34970, "102", HP34970A_VS_VTYPE_DC,
                HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_5_5DIG);
 ReadDCVolt(&tpmeas);


// Channel 1 for counter HP53132 (channel select string: see top of file)
 hp34970a_switch (hp34970, "211", HP34970A_SW_POS_CLOSE,
                 HP34970A_SW_MODE_EXCLUSIVE);

 DevClear (0, 3);
 ErrIEEE = hp5313xa_init ("GPIB::3", VI_OFF, VI_ON, 2, &hp53132);
 Delay(0.1);
 ErrIEEE = hp5313xa_freqPeriodRatio (hp53132, 1, 1, 9, VI_ON, 1);
#endif 


// 1. m?ling af frekvens
 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," *");
 V74dac = 1200;
 WriteDacV74(V74dac);
 Delay(1.0);
#if IEEEBUS
 DisableBreakOnLibraryErrors();                                                       
 hp5313xa_writeInstrData (hp53132, ":READ?");
 Delay(1.5);
 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," *");
 ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeas1);
 if (ErrIEEE == 0xBFFF0015)  //Timeout
  freqmeas1 = 0.0;
 EnableBreakOnLibraryErrors();
 SetCtrlVal (ocxoPNL, OCXO_PNL_SLIDE, freqmeas1 - FreqNom);
 Fmt(MsgStr,"%s<%f[p1]",freqmeas1);
 SetCtrlVal (ocxoPNL, OCXO_PNL_FREQ, MsgStr);
 ProcessDrawEvents();
#else
 freqmeas1 = 4.5e6 + 63;
 Delay(0.1);
#endif
 
 
// 2. m?ling af frekvens
 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," *");
 V74dac = 1500;
 WriteDacV74(V74dac);
 Delay(1.0);
#if IEEEBUS
 DisableBreakOnLibraryErrors();                                                       
 hp5313xa_writeInstrData (hp53132, ":READ?");
 Delay(1.5);
 SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," *");
 ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeas2);
 if (ErrIEEE == 0xBFFF0015)  //Timeout
  freqmeas2 = 0.0;
 EnableBreakOnLibraryErrors();
 SetCtrlVal (ocxoPNL, OCXO_PNL_SLIDE, freqmeas2 - FreqNom);
 Fmt(MsgStr,"%s<%f[p1]",freqmeas2);
 SetCtrlVal (ocxoPNL, OCXO_PNL_FREQ, MsgStr);
 ProcessDrawEvents();
#else
 freqmeas2 = 4.5e6 - 60;
 Delay(0.1);
#endif
 
 HzPerDacStep = (freqmeas1 - freqmeas2) / 300;
 V74dac = 1200 + ((freqmeas1 - FreqNom) / HzPerDacStep);
 WriteDacV74(V74dac);
 
 if (HzPerDacStep < 0.05){
  MessagePopup("TCXO-kalibrering fejlet","Dac V74 reagerer ikke");
  HidePanel(ocxoPNL);
  return F13Ok = FreqCalOk = FALSE;
 }
 

 NumberOfMeas = 0;
 MeasInsideTol = 0;
 FreqCalOk = FALSE;
 done = FALSE;
 Step = 2;
 dac[0] = dac[1] = dac[2] = 0;
 freqerr[0] = freqerr[1] = freqerr[2] = 100; 
 minerr = 100;
 
 while ((done == FALSE) && 
      (FreqCalOk == FALSE) && 
      (NumberOfMeas <= MaxNumberOfMeas)){
    
    // opsamling af 3 seneste m?linger i FIFO-buffere  
    freqerr[2] = freqerr[1];  // frekvensafvigelse
    freqerr[1] = freqerr[0]; 
    freqerr[0] = fabs(freqmeas1 - FreqNom);
     dac[2] = dac[1];       // tilh?rende dac-v?rdi
     dac[1] = dac[0];
     dac[0] = V74dac; 
 
    if (freqerr[0] > 1)
       Step = 3;
      else
       Step = 1;

    if (MeasInsideTol <= 3){
      if (freqmeas1 > FreqNom)
       V74dac += Step; 
        else
       V74dac -= Step;
    } 
    else{
       for (n = 0; n < 3; n++) // efter 3 m?linger indenfor tol findes dac-v?rdi med mindste fejl
         if (freqerr[n] < minerr){
           minerr = freqerr[n];
           minno = n;
         }  
      V74dac = dac[minno];  
    }
     
     
     WriteDacV74(V74dac);
     Delay(0.3);
     
      // M?ling i TP31
    #if IEEEBUS
     DisableBreakOnLibraryErrors();
    ReadDCVolt(&tpmeas);
     EnableBreakOnLibraryErrors();
    #else
     tpmeas = -1.0;
     Delay(0.1);
    #endif
    
    NumberOfMeas += 1;
    
      // M?ling af frekvens
    #if IEEEBUS
     DisableBreakOnLibraryErrors();                                                       
     hp5313xa_writeInstrData (hp53132, ":READ?");
     Delay(1.5);
     ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeas1);
    SetCtrlVal(ocxoPNL, OCXO_PNL_TXTBOX," *");
     if (ErrIEEE == 0xBFFF0015)  //Timeout
      freqmeas1 = 0.0;
     EnableBreakOnLibraryErrors();
    #else
     freqmeas1 = 4.5e6 + 10;
     Delay(0.1);
    #endif


      // min max slide values
      diff = fabs(freqmeas1 - FreqNom);
      if (diff <= 8) {
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MAX_VALUE, 10.0);
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_MIN_VALUE, -10.0);
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

      SetCtrlVal (ocxoPNL, OCXO_PNL_SLIDE, freqmeas1 - FreqNom);
      Fmt(MsgStr,"%s<%f[p1]",freqmeas1);
      SetCtrlVal (ocxoPNL, OCXO_PNL_FREQ, MsgStr);

      SetCtrlVal (ocxoPNL, OCXO_PNL_VCOVOLT, tpmeas);

      ProcessDrawEvents();

      if (fabs(freqmeas1 - FreqNom) > FreqTol) 
        {
        MeasInsideTol = 0;
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_FILL_COLOR, VAL_RED);
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_FREQ, ATTR_TEXT_COLOR, VAL_RED);
        }
      else 
        {
        MeasInsideTol += 1;
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_SLIDE, ATTR_FILL_COLOR, VAL_GREEN);
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_FREQ, ATTR_TEXT_COLOR, VAL_GREEN);
        }

      if (tpmeas < tpmax)
        {
        TPVoltOk = TRUE;
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_VCOVOLT, ATTR_FILL_COLOR, VAL_GREEN);
        }
      else 
        {
        TPVoltOk = FALSE;
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_VCOVOLT, ATTR_FILL_COLOR,VAL_RED);
        }
       

      if (tpmeas > tpmin)
        {
        TPVoltOk = TRUE;
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_VCOVOLT, ATTR_FILL_COLOR, VAL_GREEN);
        }
      else 
        {
        TPVoltOk = FALSE;
        SetCtrlAttribute (ocxoPNL, OCXO_PNL_VCOVOLT, ATTR_FILL_COLOR,VAL_RED);
        }
       
       
      FreqCalOk = ((MeasInsideTol >= NumberOfOkMeas) && 
                (TPVoltOk == TRUE));
       
                                                                                          
      GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
 } // while
 
 if (done == TRUE){
  MessagePopup("TCXO-kalibrering","Stoppet af bruger");
  FreqCalOk = FALSE;
 }
 
 if (NumberOfMeas > MaxNumberOfMeas){
  Fmt(MsgStr,"%s<Antal m?linger %i  (Max = %i)",NumberOfMeas,MaxNumberOfMeas);
  MessagePopup("TCXO-kalibrering fejlet",MsgStr);
  FreqCalOk = FALSE;
 }
 
 if (FreqCalOk == TRUE){
  GetDate(2);
  // Save in user ref
  Fmt(DUTStr,"%s<:FACT:GENL:UREF:DATE %s,%s,%s;",YearStr,MonthStr,DayStr);
  WriteCOMPort(DUTCOM,DUTStr);
  Fmt(DUTStr,"%s<:FACT:GENL:UREF:DAC %i;",V74dac);
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.03);
  WriteCOMPort(DUTCOM,":FACT:GENL:UREF:DAC?;");

  // Check saved user ref
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 0)
    Scan(strread,"%s>%i",&dacread);
   else
    dacread = 0;
    
   if (dacread != V74dac){
   Fmt(MsgStr,"%s< User kalibreringsv?rdi ikke gemt korrekt i NVRAM\n Gemt=%i L?st=%i",V74dac,dacread);
   MessagePopup("TCXO-kalibrering fejlet",MsgStr);
  } 

  // Save in factory ref
  Fmt(DUTStr,"%s<:FACT:GENL:FREF:DATE %s,%s,%s;",YearStr,MonthStr,DayStr);
  WriteCOMPort(DUTCOM,DUTStr);
  Fmt(DUTStr,"%s<:FACT:GENL:FREF:DAC %i;",V74dac);
  WriteCOMPort(DUTCOM,DUTStr);
  Delay(0.03);

  // Check saved factory ref
  WriteCOMPort(DUTCOM,":FACT:GENL:FREF:DAC?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 0)
    Scan(strread,"%s>%i",&dacread);
   else
    dacread = 0;
    
   if (dacread != V74dac){
   Fmt(MsgStr,"%s< Factory kalibreringsv?rdi ikke gemt korrekt i NVRAM\n Gemt=%i L?st=%i",V74dac,dacread);
   MessagePopup("TCXO-kalibrering fejlet",MsgStr);
  } 
  


  // Save last update = last calibration date
  Delay(0.03);
  Fmt(DUTStr,"%s<:FACT:MAIN:FMSD:UPD %s,%s,%s;",YearStr,MonthStr,DayStr);
  WriteCOMPort(DUTCOM,DUTStr);
 } // if (FreqCalOk == TRUE)
 

 WriteCOMPort(DUTCOM,":FACT:BB1:SIGN NORM;");

 HidePanel(ocxoPNL);


 if (Freq == 27)
   return F27Ok = FreqCalOk;
  else
   return F13Ok = FreqCalOk;
}




             

//--------------------------------------------------------------------------------
int MakeSPGReset(void){ 
int button_pressed;
int hphz[2],phase[2];
char *strread, msgstr[100], hphzstr[30], phasestr[30];

 if (WriteProtect(OFF,FALSE) == FALSE){
  SPGResetOk = FALSE;
  return FALSE;
  }

 button_pressed = ConfirmPopup ("Reset SPG", 
 "ADVARSEL: Reset bruges normalt kun p? nyproducerede SPG'er\nKalibreringsdata nulstilles.\nReset?");

 if (button_pressed == NO)
   {
   SPGResetOk = SPGNOTRESET;
   return SPGResetOk;
   }

 WriteCOMPort(DUTCOM,":FACT:V24C:COMM 21,'GZ';");
 Delay(0.05);
 WriteCOMPort(DUTCOM,":FACT:GENL:HPHZ 196290,196290;"); // hph zero default values
 Delay(0.05);
 WriteCOMPort(DUTCOM,":FACT:GENL:PHAS 28810,28810;"); // h-genlock phase default values
 Delay(0.05);
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:HPHZ?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (StringLength(strread) > 0){
   Fmt(hphzstr,"%s<%s,", strread);
   Scan(hphzstr,"%s>%2i[x]",hphz);
   }
  else {
   hphz[0] = 0; 
   hphz[1] = 0; 
  }

 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:PHAS?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (StringLength(strread) > 0){
   Fmt(phasestr,"%s<%s,", strread);
   Scan(phasestr,"%s>%2i[x]",phase);
   }
  else {
   phase[0] = 0; 
   phase[1] = 0; 
  }
  
 if (RS232Timeout == TRUE)
   Fmt(msgstr,"%s< Timeout ved l?sning fra SPG");
  else
   Fmt(msgstr,"%s< Data l?st: %s%s\n Forventet: 196290,196290,28810,28810",hphzstr,phasestr);

 SPGResetOk = FALSE;
 if ((hphz[0] == 196290) && (hphz[1] == 196290) && (phase[0] == 28810) && (phase[1] == 28810))
  SPGResetOk = TRUE;
  
 if (SPGResetOk == TRUE)
   MessagePopup ("Reset SPG", "Kalibreringsdata er nu nulstillet\n Sluk -> t?nd for apparatet");
  else
   MessagePopup ("Reset af kalibreringsdata FEJLET",msgstr);


return SPGResetOk;
}



//********************************************************************************
int MeasWithHP53132(void){
ViReal64 freqmeas;
int handle_returned, controlID_returned;

 ResetTextBox(cntPNL, COUNT_PNL_TXTBOX,"");
 SetCtrlVal(cntPNL, COUNT_PNL_TXTBOX," M?ling af 13MHz TCXO frekvens \n\n");
 SetCtrlVal(cntPNL, COUNT_PNL_TXTBOX," Der m?les med BB1 forbundet til HP53132 CH-1\n");
 SetCtrlVal(cntPNL, COUNT_PNL_TXTBOX," Signalet fra BB1 er 4.5MHz\n\n\n");
 SetCtrlVal(cntPNL, COUNT_PNL_TXTBOX," Retur: F9");
 DisplayPanel(cntPNL);

 if (WriteProtect(OFF,FALSE) == FALSE){
  return FALSE;
  }


#if IEEEBUS
// Channel 1 for counter HP53132 (channel select string: see top of file)
 hp34970a_switch (hp34970, "211", HP34970A_SW_POS_CLOSE,
                 HP34970A_SW_MODE_EXCLUSIVE);

 DevClear (0, 3);
 ErrIEEE = hp5313xa_init ("GPIB::3", VI_OFF, VI_ON, 2, &hp53132);
 Delay(0.1);
 ErrIEEE = hp5313xa_freqPeriodRatio (hp53132, 1, 1, 9, VI_ON, 1);
#endif 

 WriteCOMPort(DUTCOM,":FACT:BB1:SYST PAL;");
 Delay(0.01);
 WriteCOMPort(DUTCOM,":FACT:BB1:SIGN F45MHZ;"); // BB1 outputs 4.5MHz

 done = FALSE;

 while (done == FALSE){
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
  DisableBreakOnLibraryErrors();                                                       
  hp5313xa_writeInstrData (hp53132, ":READ?");
  Delay(1.5);
  ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeas);
  if (ErrIEEE == 0xBFFF0015)  //Timeout
   freqmeas = 0.0;
  EnableBreakOnLibraryErrors();

  Fmt(MsgStr,"%s<%f[p1] Hz",freqmeas);
  SetCtrlVal (cntPNL, COUNT_PNL_FREQ, MsgStr);

 }

 WriteCOMPort(DUTCOM,":FACT:BB1:SIGN NORMAL;"); // BB1 outputs video
 
 HidePanel(cntPNL);


return TRUE;
}




char dstr[10];
//----------------------------------------------------------------------------------
char *ReadSPGCalDate (void) {
// read SPG kalibrerings dato
int dd[3];

  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:GENL:DATE?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(dstr,"%s<%i[w2p0]%i[w2p0]%i[w2p0]",dd[0],dd[1],dd[2]); //yymmdd
  } 
  else
   Fmt(dstr,"%s<??????"); //yymmdd

 return dstr;
}








//----------------------------------------------------------------------------------
int BasicTotalTest(int TestNo){

return TRUE;

#if 0
  Volt50Ok = FALSE;
  Volt33Ok = FALSE;
  VoltM50Ok = FALSE;
  SPGResetOk = FALSE;
  F13Ok = FALSE;
  GenlOk = FALSE;
  SPGCalOk = FALSE;
  BBCalOk = FALSE;
  
  

  if (TestNo == MAINMENU_TOTAL_POWERCHECK33)
   CheckPowerSupply(33);

  if ((TestNo == MAINMENU_TOTAL_POWERCHECK50) || (Volt33Ok == TRUE))
   CheckPowerSupply(50);

  if ((TestNo == MAINMENU_TOTAL_POWERCHECK50M) || (Volt50Ok == TRUE))
   CheckPowerSupply(-50);

  if ((TestNo == MAINMENU_TOTAL_RESETSPG) || (VoltM50Ok == TRUE))
   MakeSPGReset();

  if ((TestNo == MAINMENU_TOTAL_INT13REF) || (SPGResetOk == TRUE))
   MakeRefCal(13);;

  if ((TestNo == MAINMENU_TOTAL_HPHGENL) || ( F13Ok == TRUE))
   MakeSPGHPHCal();


  if ((TestNo == MAINMENU_TOTAL_TESTGENLOCK) || (AutoHPH_NTSCOk == TRUE))
   TestSPGGenlockSystem();


  if ((TestNo == MAINMENU_TOTAL_BB1_TOTAL) || (GenlOk == TRUE)){
   BBNo = 1;
   if (MakeSyncOffsetCal(SYNC_CAL) == TRUE)
    if (MakeSyncOffsetCal(OFFSET_CAL) == TRUE)
     if (MakeBBPhaseCal(PAL) == TRUE)
      MakeBBPhaseCal(NTSC);
  }    

  if ((TestNo == MAINMENU_TOTAL_BB2_TOTAL) || (BBCalOk == TRUE)){
   BBNo = 2;
   if (MakeSyncOffsetCal(SYNC_CAL) == TRUE)
    if (MakeSyncOffsetCal(OFFSET_CAL) == TRUE)
     if (MakeBBPhaseCal(PAL) == TRUE)
      MakeBBPhaseCal(NTSC);
  }    

  if ((TestNo == MAINMENU_TOTAL_BB3_TOTAL) || (BBCalOk == TRUE)){
   BBNo = 3;
   if (MakeSyncOffsetCal(SYNC_CAL) == TRUE)
    if (MakeSyncOffsetCal(OFFSET_CAL) == TRUE)
     if (MakeBBPhaseCal(PAL) == TRUE)
      MakeBBPhaseCal(NTSC);
  }    

#endif
  
return TRUE;
}






//----------------------------------------------------------------------------------
int CVICALLBACK OkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:  Cont = TRUE; break;
  } 
    
return 0;
}



int CVICALLBACK SPGOkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:  SPGdone = TRUE;
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
    case EVENT_COMMIT:  Cont = TRUE; 
              AutoHPH_PALOk = FALSE; 
              AutoHPH_NTSCOk = FALSE; 
              GenlOk = FALSE;
              GenlPALOk = FALSE;
              GenlNTSCOk = FALSE;
              Genl10MHzOk = FALSE;
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


//-----------  Call back ved afslutning af valg af genlock signaler -----------
int CVICALLBACK ReturnSelGenlCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:
       HidePanel(selgenlPNL);
      break;
  }
  return 0;
}

//-----------  Call back ved valg af genlock signaler ----(se spggenl.uir)-----------
int CVICALLBACK GenlSignalCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
int stsel,stattn;
  switch (event) {
    case EVENT_VAL_CHANGED:
      GetCtrlVal (selgenlPNL, SELGEN_PNL_GENLRING, &stsel);
      GetCtrlVal (selgenlPNL, SELGEN_PNL_GENLATTN, &stattn);
      if ((stsel >= 0) && (stsel <=2))
        SetCtrlAttribute (selgenlPNL, SELGEN_PNL_GENLATTN, ATTR_DIMMED, TRUE);
       else
        SetCtrlAttribute (selgenlPNL, SELGEN_PNL_GENLATTN, ATTR_DIMMED, FALSE);
       switch (stsel){
        case 0: SetGenlockSignal(GENL_OFF);
                SetCtrlAttribute (selgenlPNL, SELGEN_PNL_GENLATTN, ATTR_DIMMED, TRUE);break;
        case 1: SetGenlockSignal(GENL_PAL); 
                SetCtrlAttribute (selgenlPNL, SELGEN_PNL_GENLATTN, ATTR_DIMMED, TRUE);break;
        case 2: SetGenlockSignal(GENL_NTSC);
                SetCtrlAttribute (selgenlPNL, SELGEN_PNL_GENLATTN, ATTR_DIMMED, TRUE);break;
        case 3: if (stattn == TRUE)
                  SetGenlockSignal(GENL_443_LOW);
                 else
                  SetGenlockSignal(GENL_443_HIGH);
                SetCtrlAttribute (selgenlPNL, SELGEN_PNL_GENLATTN, ATTR_DIMMED, FALSE); break; 
        case 4: if (stattn == TRUE)
                  SetGenlockSignal(GENL_358_LOW);
                 else
                  SetGenlockSignal(GENL_358_HIGH);
                SetCtrlAttribute (selgenlPNL, SELGEN_PNL_GENLATTN, ATTR_DIMMED, FALSE); break; 
        case 5: if (stattn == TRUE)
                  SetGenlockSignal(GENL_10M_LOW);
                 else
                  SetGenlockSignal(GENL_10M_HIGH);
                SetCtrlAttribute (selgenlPNL, SELGEN_PNL_GENLATTN, ATTR_DIMMED, FALSE); break; 
       } // switch
      break;  // switch
  }
  return 0;
}

int CVICALLBACK RefOscCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:
      if (control == OCXO_PNL_REFOSCOK_BTN){
       Cont = TRUE; done = TRUE; F27Ok = TRUE; F13Ok = TRUE;
       }
       else if (control == OCXO_PNL_REFOSCERR_BTN){
       Cont = TRUE; done = TRUE; F27Ok = FALSE; F13Ok = FALSE; FreqCalOk = FALSE;
       }

      break;
  }
  return 0;
}


int CVICALLBACK F45MHzCallback (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:
      if (control ==  COUNT_PNL_F45EXIT_BTN)
       done = TRUE;
      break;
  }
  return 0;
}



