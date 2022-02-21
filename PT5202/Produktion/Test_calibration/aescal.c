/* FILE NAME   aescal.c
 * PURPOSE     Test af audio generator i PT5201 
 *             
 * SPEC       
 * DESIGN      juni 2000, Peter Frederiksen
 * CODING    
 * CODE INSP.
 * TEST     
 *            
 * MODIFIED:
 *
 * NOTES:      4.01.01: HP34970 adresses for audio channels L&R changed to 105 and 106
 *                      HP34970 measure calls changed       
*/


#include "hp34970a.h"
#include <gpib.h>
#include "hp5313xa.h"
#include <ansi_c.h>
#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <userint.h>
#include "aes.h"
#include "cviutil.h"
#include "bbcal.h"
#include "def.h"


char  MasterSW[15];
char  ErrStr[200];
char  TxtStr[150];

const double AESFreqTol = 0.2;  // Hz    Audio tones tolerances
const double WClkTol = 1;     // Hz    48/44.1 kHz ref tolerances


// wordclock parameters
const double WClkRiseTimeMin = 30;       // ns
const double WClkRiseTimeMax = 50;
const double WClkFallTimeMin = 30;
const double WClkFallTimeMax = 50;

const double WClkVoltppMax = 2.7;        // V
const double WClkVoltppMin = 2.2;        // V
const double WClkVoltOffsetMax = 1.50;    // V
const double WClkVoltOffsetMin = 1.00;    // V


// Analog Audio parameters
const double dBm0 = 0.775;
const double NomAnalogAudioLevel = 1550.0,  // mV RMS  (6dBm @ 600 ohms)
         MaxmVStep = 6.0,
         MinmVStep = 1.0,
         AnalogAudioLevelTol = 5;// Analog audio level tolerance in mV
const int MaxNoOfMeas = 15;     // max antal målinger for audio level kalibrering
const int RequiredNoOfOkMeas = 4; // krævet antal målinger indenfor tol
const int NoOfLevelCheck = 6;
struct {
   char *levelcmd;
   double nomlevel;
   char *freqcmd;
   double nomfreq;
    } checkno[6] = // <- see 'NoOfLevelCheck'
          {"+8" , +8.0,"S500HZ",  500.0,
           "+4" ,  4.0,"S1KHZ"  , 1000.0,
           "0"  ,  0.0,"S8KHZ"  , 8000.0,
           "-6" , -6.0,""   , 0.0,
           "-12",-12.0,""   , 0.0,
           "-18",-18.0,""   , 0.0};
               
const double checktol = 0.3;    //± dBm tol ved analog audio level check
const double freqchecktol = 1.0;  //± Hz tol ved analog audio freq check




int     NoOfOkMeas,         // antal målinger indenfor tol
        NoOfMeas;         // antal målinger

unsigned char AudioPOTMValue,
          AudioPOTMValueRight, 
          AudioPOTMValueLeft;




int AESFreqOk,
   LeftFreqOk,
   RightFreqOk,
   AnalogFreqOk,
    AudioGenOk,
    RiseTimeOk,
    FallTimeOk,
    VoltppOk,
    VoltOffsetOk,
    WClkRiseFallOk,
    WClkOk,
    WClkVoltppOk,
    WClkOffsetOk,
    AnalogLevelCalOk;

ViReal64 RiseTime,FallTime,Voltpp,VoltOffset;
    



// prototyping
int ReadCounter_AES(double CHAFreq,double CHBFreq);
int InitHP53132_AES(void);
int MeasWordClockRiseFallTime(void);
int AnalogAudioFreqCheck(void);
int AnalogAudioLevelCheck(void);
int MeasWordClockOffset(void);
int CalAnalogAudioLevel(int ChNo);
int AudioGenTotalTest(int TestNo);

void SetAudioPOTM(unsigned char dacleft, unsigned char dacright);
void SaveAudioPOTM(unsigned char dacleft, unsigned char dacright);



//****************************************************************************
int ReadACVolt(double *result){
ViChar datastr[20];
int st;
// The function 'hp34970a_voltageMeasure' must be executed once before using this function
st = hp34970a_cmdString_Q (hp34970, "READ?", 20, datastr);
Scan(datastr,"%s>%f",result);
return (st);
}


int ReadAudioFreq(double *freq){
ViChar datastr[20];
int st;
st = hp34970a_cmdString_Q (hp34970, "READ?", 20, datastr);
Scan(datastr,"%s>%f",freq);
return(st);
}


//-----------------------------------------------------------------------------------
int TestAnalogAudioGen(void){

 SetPanelAttribute (anPNL, ATTR_TITLE, " PT5201 Analog Audio Generator ");

 if (WriteProtect(OFF,FALSE) == FALSE)
  return (AudioGenOk = FALSE);
 
 WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP ANAL;"); // select analog audio
 

// Calibrate audio level
  if ((AnalogLevelCalOk = CalAnalogAudioLevel(LEFTCHANNEL)) == FALSE){
  AudioGenOk = FALSE;
  return AudioGenOk;
  }
  
  if ((AnalogLevelCalOk = CalAnalogAudioLevel(RIGHTCHANNEL)) == FALSE){
  AudioGenOk = FALSE;
  return AudioGenOk;
  }


// Check Level Left/Right Channel
  if (AnalogAudioLevelCheck() == FALSE){
  AudioGenOk = FALSE;
  return AudioGenOk;
  }
 
// Check Freq Left/Right Channel
  if (AnalogAudioFreqCheck() == FALSE){
  AudioGenOk = FALSE;
  return AudioGenOk;
  }

 

 return AudioGenOk;
} //TestAnalogAudioGen






//-----------------------------------------------------------------------------------
int TestAESAudioGen(void){
/*
Der testes følgende:
 måling af VCO spænding i TP18
 måling af WCLK-signal på BNC-stik: Vpp, offset, rise/fall time
 måling af 500Hz, 1kHz og 8kHz frekvenser.
 kontrol af level på TEK764
 kontrol af video/audio lock for PAL og NTSC.
 
*/
int handle_returned, 
    controlID_returned,
    bytes_written,
    fhandle,
    relaytype;    // 1:Relay RE1 = bistable   0:Relay RE1 = monostable
char *strread,
     TxtStr[80],
    wrtbuf[100],
    *tt;      // HH:MM:SS
int AudioStatusPort,
    AudioVideoLockedOk,
    n;
float VoltMax,
    VoltMin;

int err;

 SetPanelAttribute (aesPNL, ATTR_TITLE, " PT5201 AES Audio Generator ");
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 DisplayPanel (aesPNL);
 

 AudioGenOk = WriteProtect(OFF,FALSE);
 if (AudioGenOk == FALSE){
  HidePanel (aesPNL);
  return AudioGenOk;
 }
 

 
// VCO spænding @ 5.6448 MHz --------------------------------------------------
  AudioGenOk = CheckAudioGenVCO(VCO6144);   // function findes i bbcal.c
  if (AudioGenOk == FALSE){
   //MessagePopup (" Audio generator test @ 48kHz  (X-TAL: 6.144MHz)", " VCO spændingstest i TP18 FEJLET ");
   HidePanel (aesPNL);
   return AudioGenOk;
  }

// VCO spænding @ 6.144 MHz ---------------------------------------------------
  AudioGenOk = CheckAudioGenVCO(VCO56448);   // function findes i bbcal.c
  if (AudioGenOk == FALSE){
   //MessagePopup (" Audio generator test @ 44.1kHz  (X-TAL: 5.6448MHz)", " VCO spændingstest i TP18 FEJLET ");
   HidePanel (aesPNL);
   return AudioGenOk;
  }


#if IEEE 
 DevClear (0, 3); // clear counter HP53132
#endif 
 Delay(0.5);

 InitHP53132_AES();



 // AES Audio freq test -----------------------------------------------------
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");

 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," AES/EBU XLR forbindes til TEK 764 AES-110 input\n");

 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 Menu: Input CH 3-4: XLR-balanced \n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 Menu: Headphones Audio Source = MANUAL evt AUTO\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"                         Manual Left CH-3    Manual Right CH-4\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 Menu: Level Meters: Display Mode: 4 channels\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 VIEW: AUDIO\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 VOLUME drejes i max  (helt højre om)\n\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," OK:    Tast F4\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Fejl:  Tast F9\n\n");


 WaitForContinue();

 if (AudioGenOk == FALSE){
  //MessagePopup (" Audio generator test ", " FEJLET ");
  HidePanel (aesPNL);
  return FALSE;
 }

 WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP AES;");        // mode = AES
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:WORD F48KHZ;"); // samp freq = 48k
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV 0;");       // level = 0dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SIGN S500HZ;"); // Stereo 500 Hz 

 InitHP53132_AES();
 err = hp34970a_switch (hp34970, "212", HP34970A_SW_POS_CLOSE,HP34970A_SW_MODE_NORMAL); //card2 mux1 ch2
 err = hp34970a_switch (hp34970, "222", HP34970A_SW_POS_CLOSE,HP34970A_SW_MODE_NORMAL); //card2 mux2 ch2
 
 Delay(2.0);

// *************** Freq test @48kHz ******************************
//  Stereo 500 Hz 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Stereo 500 Hz");
 
 AESFreqOk = ReadCounter_AES(500,500);
 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"         OK\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"         FAIL\n");
   AudioGenOk = FALSE;
  }
  

//  Stereo 1 kHz 
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SIGN S1KHZ;");  // Stereo 1kHz 
 Delay(2);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Stereo 1 kHz");
 
 AESFreqOk = ReadCounter_AES(1000,1000);
 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"          OK\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"          FAIL\n");
   AudioGenOk = FALSE;
  }


// Stereo 8000 Hz 
#if IEEE
 DevClear (0, 3);
#endif 
 Delay(0.5);

 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SIGN S8KHZ;");  // Stereo 8kHz 
 Delay(2.0);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Stereo 8000 Hz");
 
 AESFreqOk = ReadCounter_AES(8000,8000);

 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"        OK\n");
  else{
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"        FAIL\n");
   AudioGenOk = FALSE;
  }
  

 if (AudioGenOk == FALSE){
  //MessagePopup (" AES-Audio generator test @ 48kHz ", " FEJLET ");
  HidePanel (aesPNL);
  return FALSE;
 }


// *************** Freq test @44.1 kHz ******************************
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:WORD F441KHZ;");  // samp freq = 44.1k
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV 0;");     // level = 0dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SIGN S500HZ;"); // Stereo 500 Hz 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"\n Test @ 44.1kHz:\n");
 Delay(5.0);  // wait for VCO to settle and lock
 //  Stereo 500 Hz 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Stereo 500 Hz");
 
 AESFreqOk = ReadCounter_AES(497.65,497.65);
 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"         OK\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"         FAIL\n");
   AudioGenOk = FALSE;
  }
  

//  Stereo 1 kHz 
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SIGN S1KHZ;");  // Stereo 1kHz 
 Delay(2);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Stereo 1 kHz");
 
 AESFreqOk = ReadCounter_AES(995.31,995.31);
 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"          OK\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"          FAIL\n");
   AudioGenOk = FALSE;
  }
  
 
// Stereo 8000 Hz 
#if IEEE
 DevClear (0, 3);
#endif 
 Delay(0.5);

 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SIGN S8KHZ;");  // Stereo 8kHz 
 Delay(2.0);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Stereo 8000 Hz");
 
 AESFreqOk = ReadCounter_AES(8039.06,8039.06);

 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"        OK\n");
  else{
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"        FAIL\n");
   AudioGenOk = FALSE;
  }
  

 if (AudioGenOk == FALSE){
  //MessagePopup (" AES-Audio generator test @ 44.1kHz", " FEJLET ");
  HidePanel (aesPNL);
  return FALSE;
 }








// ------- AES Audio level test -----------------------------------------------
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"\n\n Aflæs level på TEK 764\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Level skal skifte mellem 0, -9, -12, -15, -16, -18, -20 og MUTE\n\n");

 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," OBS: Mellem levelskiftene vises der muligvis CODE ERROR. Dette er ingen fejl!\n\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," OK:    Tast F4\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Fejl:  Tast F9\n\n");
 
 Cont = FALSE;
 while (Cont == FALSE) {
 
 
 // 0 dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV 0;");     // level = 0dBFS
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 
 
 // 9 dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV -9;");

 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 
 
 // 12 dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV -12;");

 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 
 
 // 15 dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV -15;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 
 
 // 16 dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV -16;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 
 
 // 18 dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV -18;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 

 // 20 dBFS
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV -20;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 

 // silence/MUTE
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV SIL;");
 Delay(3);
 
 } // while


 
// leave level test with level = 9 dBFS 
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:LEV -9;");

 if (AudioGenOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Audio level test  OK\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Audio level test  FEJLET\n");
   //MessagePopup(" AES-Audio level test "," FEJLET ");
   Delay(2.0);
   HidePanel(aesPNL);
   return AudioGenOk;
   }
 Delay(1);
 HidePanel(aesPNL);
 return AudioGenOk;  //<<<<<<<<<<<<<<<<<<<<<
  



// ------- Audio/Video Lock test --------------------------------------- 
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Test af audio-video lock\n\n");
 
 AudioVideoLockedOk = TRUE;
 

 for (n = 0; n < 4; n++)
   {
   switch (n)
     {
     case 0 :
       SetCtrlVal(aesPNL, AES_PNL_TXTBOX," PAL lock          ");
       WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SYST PAL;TIM +4.8;"); 
       break;
     case 1 :
       SetCtrlVal(aesPNL, AES_PNL_TXTBOX," PAL lock          ");
       WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SYST PAL;TIM -4.8;"); 
       break;
     case 2 :
       SetCtrlVal(aesPNL, AES_PNL_TXTBOX," NTSC lock         ");
       WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SYST NTSC;TIM +8.0;"); 
       break;
     case 3 :
       SetCtrlVal(aesPNL, AES_PNL_TXTBOX," NTSC lock         ");
       WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SYST NTSC;TIM +0.0;"); 
       break;
     } // switch

 Delay(1.5);         // delay fra lock kommando til test om der er video/audio lock
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:AUD:STATUS:READ?;");

// AudioStatusPort: bit0-6: control bits (digital potm...)
//              bit 7: relaytype
//              bit 8: PLL-locked
//              bit 9: audio-video locked
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%i",&AudioStatusPort);  

#if DEBUG
  Fmt(TxtStr,"%s<  %s\n",strread);
  SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
#endif  

 
  if ((AudioStatusPort & 0x300) != 0x300)  // bit 8,9 = 1  (audiovideolock + 27MHzlock)
   AudioVideoLockedOk = FALSE;

  if (AudioVideoLockedOk == TRUE)
    SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"    OK\n");
   else
    SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"    FAIL\n");

 } //for (n = 0; n < 4; n++)

 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"\n");
 relaytype = (AudioStatusPort & 0x80) >> 8;  // bit 7: relaytype  

 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SYST PAL;");


 if (AudioVideoLockedOk == FALSE)
  AudioGenOk = FALSE;

 
 // AES test resultat gemmes i logfil
 if (AudioGenOk == TRUE){
   // get system date 
   dato = GetDate(1);
 
   // get system time
   tt = TimeStr();
  
   strread = ReadMasterSWVer();
   CopyString(MasterSW,0,strread,0,-1);

   // check om logfil findes, ellers opret en ny logfil
   fhandle = OpenFile (AESLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   Fmt(wrtbuf,"%s<%s,%s,AES/EBU signal = ok,MasterSW=%s\n",dato,tt,MasterSW); 
   bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
   CloseFile(fhandle);
 } // if
 


#if 0
 if (AudioGenOk == TRUE)
   MessagePopup(" AES-Generator test ","OK");
  else
   MessagePopup(" AES-Generator test ","FEJLET");
#endif
 HidePanel(aesPNL);

return AudioGenOk;
} // TestAESAudioGen






//-----------------------------------------------------------------------------------
int TestWordClockOutput(void){
/*
Der testes følgende:
 måling af wordclock signal på BNC-stik: Vpp, offset, rise/fall time
*/
int handle_returned, controlID_returned;
int bytes_written, fhandle;
char *strread,
     TxtStr[80],
    wrtbuf[100],
     *d,      // MM-DD-YYYY
    *tt;    // HH:MM:SS


 SetPanelAttribute (aesPNL, ATTR_TITLE, " Wordclock ");

 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 ProcessDrawEvents();
 DisplayPanel(aesPNL);
 

 AudioGenOk = WriteProtect(OFF,FALSE);
 if (AudioGenOk == FALSE){
  HidePanel (aesPNL);
  return AudioGenOk;
 }
 
 WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP AES;");        // mode = AES
 WriteCOMPort(DUTCOM,":OUTP:AUD:AES:WORD F48KHZ;");

 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Wordclock BNC forbindes til counter HP53132 CH-1\n\n\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," OK:    Tast F4\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Fejl:  Tast F9\n\n");
 ProcessDrawEvents();

 WaitForContinue();

 if (AudioGenOk == FALSE){
  //MessagePopup (" Audio generator test ", " AFBRUDT ");
  HidePanel (aesPNL);
  return FALSE;
 }

 hp34970a_switch (hp34970, "211", HP34970A_SW_POS_CLOSE,HP34970A_SW_MODE_EXCLUSIVE);  //card2 mux1 ch1

 
 // set sample frequency = 48 kHz  (wordclock) ---------------------------------------
 Delay(2.0);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," 48 kHz");
 
 AESFreqOk = ReadCounter_AES(48000,0);

 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"   48 kHz OK\n\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"   48 kHz FAIL\n");
   AudioGenOk = FALSE;
  }
  

 if (MeasWordClockOffset() == FALSE)
   AudioGenOk = FALSE;

 ErrIEEE = hp5313xa_inputCond (hp53132, 1, VI_OFF, VI_ON, VI_OFF,VI_OFF);  // counter = 1Mohm, CH = 1
 
 Delay(1);

 
 // Wordclock status gemmes i logfil
 if (AudioGenOk == TRUE){
   // get system date 
   dato = GetDate(1);
 
   // get system time
   tt = TimeStr();
  
    
   strread = ReadMasterSWVer(); // see bbcal.c
   CopyString(MasterSW,0,strread,0,-1);

   // check om logfil findes, ellers opret en ny logfil
   fhandle = OpenFile (AESLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
   
   Fmt(wrtbuf,"%s<%s,%s,Vpp=%f[p2], Voffset=%f[p2],MasterSW=%s\n",dato,tt,Voltpp,VoltOffset,MasterSW);  
   bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
   CloseFile(fhandle);
 } // if
 
 
 
#if 0 
 if (AudioGenOk == TRUE)
   MessagePopup (" Wordclock output ", " OK ");
  else
   MessagePopup (" Wordclock output "," FEJLET ");
#endif
 HidePanel (aesPNL);

return AudioGenOk;
} // TestWordClockOutput






 // Rise/Fall time  WCLK BNC-stik  -------------------------------------------------------
int MeasWordClockRiseFallTime(void){
int WCRiseFallOk;
char TxtStr[80];
ViReal64 RiseTime,FallTime;

 WClkRiseFallOk = TRUE;
 Fmt(TxtStr,"%s< Risetime    ");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 ErrIEEE = hp5313xa_inputCond (hp53132, 1, VI_OFF, VI_ON, VI_OFF,VI_OFF);  // counter = CH-1
 hp5313xa_writeInstrData (hp53132, ":MEAS:SCAL:VOLT:RISE:TIME?");
 Delay(3);
 ErrIEEE = hp5313xa_mathRes (hp53132, &RiseTime);
 RiseTime = RiseTime * 1e9;
 if ((RiseTime > WClkRiseTimeMax) || (RiseTime < WClkRiseTimeMin))
  WCRiseFallOk = FALSE;
 
 if (WCRiseFallOk == TRUE)
   Fmt(TxtStr,"%s<%f[p1] ns    OK\n",RiseTime);
  else 
   Fmt(TxtStr,"%s<%f[p1] ns    FAIL   (%f[p1] - %f[p1])\n",RiseTime,WClkRiseTimeMin,WClkRiseTimeMax);
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 

 Fmt(TxtStr,"%s< Falltime     ");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 hp5313xa_writeInstrData (hp53132, ":MEAS:SCAL:VOLT:FALL:TIME?");
 Delay(3);
 ErrIEEE = hp5313xa_mathRes (hp53132, &FallTime);
 FallTime = FallTime * 1e9;
 if ((FallTime > WClkFallTimeMax) || (FallTime < WClkFallTimeMin))
  WCRiseFallOk = FALSE;
 
 if (WCRiseFallOk == TRUE)
   Fmt(TxtStr,"%s<%f[p1] ns    OK\n",FallTime);
  else 
   Fmt(TxtStr,"%s<%f[p1] ns    FAIL   (%f[p1] - %f[p1])\n",FallTime,WClkFallTimeMin,WClkFallTimeMax);
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 ProcessDrawEvents();

return WClkRiseFallOk;
} // MeasWordClockRiseFallTime




// Wordclock BNC Vpp + offset  ----------------------------------------------------------------
int MeasWordClockOffset(void){
char TxtStr[80];
ViReal64  VoltMax,VoltMin;

 WClkOffsetOk = TRUE;
 WClkVoltppOk = TRUE;
 WClkOk = TRUE;

 ErrIEEE = hp5313xa_inputCond (hp53132, 1, VI_ON, VI_OFF, VI_OFF, VI_OFF); // Counter Zin = 50 ohm
 
 Fmt(TxtStr,"%s< Volt pp    (@75 ohm)  ");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 hp5313xa_writeInstrData (hp53132, ":MEAS:SCAL:VOLT:MAX?");
 Delay(2);
 ErrIEEE = hp5313xa_dataRes (hp53132, &VoltMax);
 VoltMax = (VoltMax * 1.25) - 0.1;   // der måles med 50 ohm afslutning  i stedet for 75 ohm

 Delay(0.5);
 hp5313xa_writeInstrData (hp53132, ":MEAS:SCAL:VOLT:MIN?");
 Delay(2);
 ErrIEEE = hp5313xa_dataRes (hp53132, &VoltMin);
 VoltMin = (VoltMin * 1.25) + 0.1 ;  // der måles med 50 ohm afslutning i stedet for 75 ohm
 
 Voltpp = VoltMax - VoltMin;
 VoltOffset = (fabs(VoltMax) - fabs(VoltMin)) / 2;
 
 // Voltpp
 if ((Voltpp > WClkVoltppMax) || (Voltpp < WClkVoltppMin)){
  WClkOk = FALSE;
  WClkVoltppOk = FALSE;
 }
 if (WClkVoltppOk == TRUE)
   Fmt(TxtStr,"%s<%f[p2w5] Vpp    OK\n",Voltpp);
  else 
   Fmt(TxtStr,"%s<%f[p2w5] Vpp    FAIL   (%f[p1] - %f[p1])\n",Voltpp,WClkVoltppMin,WClkVoltppMax);
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 

 //VoltOffset
 Fmt(TxtStr,"%s< DC offset  (@75 ohm)  ");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 if ((VoltOffset > WClkVoltOffsetMax) || (VoltOffset < WClkVoltOffsetMin)){
  WClkOk = FALSE;
  WClkOffsetOk = FALSE;
 } 
  
 if (WClkOffsetOk == TRUE)
   Fmt(TxtStr,"%s<%f[p2w5] VDC    OK\n",VoltOffset);
  else 
   Fmt(TxtStr,"%s<%f[p2w5] VDC    FAIL   (%f[p1]-%f[p1])\n",VoltOffset,WClkVoltOffsetMin,WClkVoltOffsetMax);
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 
 ProcessDrawEvents();

return WClkOk;
} // MeasWordClockOffset

//-------------------------------------------------------------------------------
int ReadCounter_AES(double CHAFreq,double CHBFreq)
  {
  char TxtStr[50];
  ViReal64 freqmeasA,freqmeasB;
  int herr;

#if IEEEBUS
 
 DisableBreakOnLibraryErrors();

 if (CHAFreq > 0)
   {
   ErrIEEE = hp5313xa_freqPeriodRatio (hp53132, 1, 1, 6, VI_ON, 3);
   Delay(0.2);
   herr = hp5313xa_writeInstrData (hp53132, ":READ?");
   Delay(1.2); // Timeout occurs on next line. This is original line
   ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeasA);
   Delay(1.0);
   }

 if (CHBFreq > 0)
   {    
   ErrIEEE = hp5313xa_freqPeriodRatio (hp53132, 2, 1, 6, VI_ON, 3);
   Delay(0.2);
   hp5313xa_writeInstrData (hp53132, ":READ?");
   Delay(1.2);
   ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeasB);
   Delay(0.2);
   } 
 EnableBreakOnLibraryErrors();
 
 if (CHBFreq > 0)
   {           // Audio tones meas
   if ((fabs(CHAFreq - freqmeasA) > AESFreqTol) || (fabs(CHBFreq - freqmeasB) > AESFreqTol))
     return FALSE;
   else
     return TRUE;
   }
 else
   {                 // 48 kHz meas
   if (fabs(CHAFreq - freqmeasA) > WClkTol)
     return FALSE;
   else
     return TRUE;
   }
#else
   return TRUE;
#endif 

} // ReadCounter_AES





// ------- Analog Audio Freq test -----------------------------------------------
int AnalogAudioFreqCheck(void){
ViChar mm[30];
int n;
double measL, measR;

 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"\n");
 SetPanelAttribute (aesPNL, ATTR_TITLE, " Analog Audio Generator: Frekvens Check ");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Analog audio left channel (XLR) forbindes til HP34970 LEFT\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Analog audio right channel (XLR) forbindes til HP34970 RIGHT\n\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Fortsæt:  Tast F4\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Afbryd:   Tast F9\n\n");
 DisplayPanel (aesPNL);

 WaitForContinue();
 if (AudioGenOk == FALSE){
  //MessagePopup (" Analog audio generator test ", " FEJLET ");
  HidePanel (aesPNL);
  return FALSE;
 }

 Fmt(TxtStr,"%s<  Nom Freq    Left    Right          Tolerance = %f[p1w4]\n",freqchecktol);  
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);


 AudioGenOk = WriteProtect(OFF,FALSE);
 if (AudioGenOk == FALSE){
  HidePanel (aesPNL);
  return AudioGenOk;
 }

 WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP ANAL;");     // analog audio mode

#if IEEEBUS
 hp34970a_freqPeriodMeasure (hp34970, "105", HP34970A_FPM_TYPE_FREQ,
                   HP34970A_FPM_APER_100MS, mm);

#endif

 AnalogFreqOk = TRUE;

 for (n = 0; n < 3; n++)
   {
   Fmt(DUTStr,"%s<:OUTP:AUD:ANAL:SIGN %s;:OUTP:AUD:ANAL:LEV 0;",checkno[n].freqcmd);
   WriteCOMPort(DUTCOM,DUTStr);
   Delay(0.5);

  // Left channel freq meas
#if IEEEBUS

   hp34970a_freqPeriodMeasure (hp34970, "105", HP34970A_FPM_TYPE_FREQ,
                   HP34970A_FPM_APER_100MS, mm);
   hp34970a_freqPeriodSetup (hp34970, "105", HP34970A_FPS_TYPE_FREQ,
                  HP34970A_FPS_VR_AUTO,
                  HP34970A_FPS_APER_100MS);
#endif

   ReadAudioFreq(&measL);
   LeftFreqOk =  (measL < (checkno[n].nomfreq + freqchecktol)) && 
                 (measL > (checkno[n].nomfreq - freqchecktol));
  
   if (LeftFreqOk == FALSE)
     AnalogFreqOk = FALSE;
 
   // Right channel freq meas
#if IEEEBUS

/******/
   hp34970a_freqPeriodMeasure (hp34970, "106", HP34970A_FPM_TYPE_FREQ,
                   HP34970A_FPM_APER_100MS, mm);
/******/
   hp34970a_freqPeriodSetup (hp34970, "106", HP34970A_FPS_TYPE_FREQ,
                  HP34970A_FPS_VR_AUTO,
                  HP34970A_FPS_APER_100MS);
#endif

   ReadAudioFreq(&measR);
   RightFreqOk = (measR < (checkno[n].nomfreq + freqchecktol)) && 
                 (measR > (checkno[n].nomfreq - freqchecktol));
 
   if (RightFreqOk == FALSE)
     AnalogFreqOk = FALSE;
 
   Fmt(TxtStr,"%s< %f[p1w6]:     %f[p1w6]  %f[p1w6]\n",checkno[n].nomfreq,measL,measR);
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
   } // for
  
 if (AnalogFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"\n Analog audio frekvens test OK\n");
 else 
   {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"\n Analog audio frekvens test FAIL\n");
   AudioGenOk = FALSE;
   //MessagePopup (" Analog audio frekvens test ", " FEJLET ");
   }
  
 WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:SIGN S1KHZ;"); // analog audio signal

 Delay(2.0);

 HidePanel (aesPNL);
 return AnalogFreqOk;
} //AnalogAudioFreqCheck

// -------------------------------------------------------------------------------
int AnalogAudioLevelCheck(void)
  { 
  int AnLeftOk, AnRightOk, n;
  double measL, measR;
  ViChar mm[30];


  ResetTextBox(aesPNL, AES_PNL_TXTBOX,"\n");
  SetPanelAttribute (aesPNL, ATTR_TITLE, " Analog Audio Generator: Level Check ");
  SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Analog audio left channel (XLR) forbindes til HP34970 LEFT\n");
  SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Analog audio right channel (XLR) forbindes til HP34970 RIGHT\n\n");
  SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Fortsæt:  Tast F4\n");
  SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Afbryd:   Tast F9\n\n");
  DisplayPanel (aesPNL);
 
  // set password = off & check RS-232 forbindelse
  if (WriteProtect(OFF,FALSE) == FALSE)
    {
    HidePanel (anPNL);
    return FALSE;
    }

  WaitForContinue();
  if (AudioGenOk == FALSE)
    {
    //MessagePopup (" Analog audio level test ", " Stoppet af bruger ");
    HidePanel (aesPNL);
    return FALSE;
    }

  Fmt(TxtStr,"%s<  Nom Level    Left    Right          Tolerance = %f[p1w4]\n",checktol);  
  SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);

  WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP ANAL;");     // analog audio mode
  WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:SIGN S1KHZ;"); // analog audio signal

#if IEEEBUS
  hp34970a_voltageMeasure (hp34970, "105", HP34970A_VM_VTYPE_AC,
                  HP34970A_VM_VRANGE_10V,
                  HP34970A_VM_RES_5_5DIG, mm);

  hp34970a_voltageSetup (hp34970, "105", HP34970A_VS_VTYPE_AC,
                HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_5_5DIG);
#endif

  for (n = 0; n < NoOfLevelCheck; n++) 
    {
    Fmt(DUTStr,"%s<:OUTP:AUD:ANAL:LEV %s;",checkno[n].levelcmd);
    WriteCOMPort(DUTCOM,DUTStr);
  
    // select HP34970A for left audio
#if IEEEBUS

   hp34970a_voltageMeasure (hp34970, "105", HP34970A_VM_VTYPE_AC,
                  HP34970A_VM_VRANGE_10V,
                  HP34970A_VM_RES_5_5DIG, mm);
   hp34970a_voltageSetup (hp34970, "105", HP34970A_VS_VTYPE_AC,
                 HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_6_5DIG);
#endif
   Delay(0.7);
  
   DisableBreakOnLibraryErrors();
   ReadACVolt(&measL);
   measL = 20 * log10(measL/dBm0);  // << Handle evt.errors of log10 :JK
   EnableBreakOnLibraryErrors();
   AnLeftOk = (measL < (checkno[n].nomlevel + checktol)) && 
              (measL > (checkno[n].nomlevel - checktol));


   // select HP34970A for right audio
#if IEEEBUS

   hp34970a_voltageMeasure (hp34970, "106", HP34970A_VM_VTYPE_AC,
                  HP34970A_VM_VRANGE_10V,
                  HP34970A_VM_RES_5_5DIG, mm);
   hp34970a_voltageSetup (hp34970, "106", HP34970A_VS_VTYPE_AC,
                 HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_6_5DIG);
#endif
   Delay(0.7);

   DisableBreakOnLibraryErrors();
   ReadACVolt(&measR);
   measR = 20 * log10(measR/dBm0); // <<<<<< Must have some error handler: JK
   EnableBreakOnLibraryErrors();
   AnRightOk = (measR < (checkno[n].nomlevel + checktol)) && 
               (measR > (checkno[n].nomlevel - checktol));
  
   Fmt(TxtStr,"%s< %f[p1w6]:     %f[p1w6]  %f[p1w6]    ",checkno[n].nomlevel,measL,measR);
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 
   if ((AnLeftOk == TRUE) && (AnRightOk == TRUE))
     SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"OK\n");
   else
     {
     SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"FEJL\n");
     AudioGenOk = FALSE;
     }
   } // for


 
// leave level test with level = 0 dBm 
 WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:LEV 0;");

 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"\n\n");
 if (AudioGenOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Audio level test  OK\n");
 else
   {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Audio level test  FEJLET\n");
   //MessagePopup (" Analog audio level test ", " FEJLET ");
   } 

 Delay(2.0);
 HidePanel(aesPNL);

 return AudioGenOk;
} //AnalogAudioLevelCheck


//-------------------------------------------------------------------------------
int CalAnalogAudioLevel(int ChNo){
int LevelInsideTol,
    CalOk;
int handle_returned, controlID_returned;    
double meas1, meas2, mVprStep; 
unsigned char AudioPOTMValueSend;
ViChar mm[30];

 ResetTextBox(anPNL, ANALOG_PNL_TXTBOX,"");
 if (ChNo == LEFTCH){
  SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX," Analog audio left channel (XLR) forbindes til HP34970 LEFT\n");
  SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX," Analog audio right channel (XLR) forbindes til HP34970 RIGHT\n\n");
  SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX," OK:    Tast F4\n");
  SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX," Fejl:  Tast F9\n\n");
 }
  else
  SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX,"\n\n Fejl:  Tast F9\n\n");

 SetCtrlVal(anPNL, ANALOG_PNL_LED, OFF);
 SetCtrlAttribute(anPNL, ANALOG_PNL_LED, ATTR_ON_COLOR,VAL_RED);
 Fmt(TxtStr,"%s< %f[p0] ± %f[p0] mV",NomAnalogAudioLevel,AnalogAudioLevelTol);
 SetCtrlVal(anPNL, ANALOG_PNL_LEVELTOL, TxtStr);

#if DEBUG
 SetCtrlAttribute (anPNL, ANALOG_PNL_COMMSTR, ATTR_VISIBLE, TRUE);
#else
 SetCtrlAttribute (anPNL, ANALOG_PNL_COMMSTR, ATTR_VISIBLE, FALSE);
#endif

 DisplayPanel (anPNL);

 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE){
  HidePanel (anPNL);
  return AnalogLevelCalOk = FALSE;
  }

 if (ChNo == LEFTCH)
  WaitForContinue();
 
 if (AudioGenOk == FALSE){
  //MessagePopup (" Analog Audio Generator ", " Level Kalibrering AFBRUDT ");
  HidePanel (anPNL);
  return FALSE;
 }

 SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX," Målinger *");

 WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP ANAL;");     // analog audio mode
 WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:SIGN S1KHZ;"); // analog audio signal
 WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:LEV +6;");     // analog audio level = 1550mV
 
 
#if IEEEBUS
 if (ChNo == LEFTCH)
   {
   hp34970a_voltageMeasure (hp34970, "105", HP34970A_VM_VTYPE_AC,
                  HP34970A_VM_VRANGE_10V,
                  HP34970A_VM_RES_5_5DIG, mm);

   hp34970a_voltageSetup (hp34970, "105", HP34970A_VS_VTYPE_AC,
                HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_5_5DIG);
   } 
 else
   {
   hp34970a_voltageMeasure (hp34970, "106", HP34970A_VM_VTYPE_AC,
                  HP34970A_VM_VRANGE_10V,
                  HP34970A_VM_RES_5_5DIG, mm);

   hp34970a_voltageSetup (hp34970, "106", HP34970A_VS_VTYPE_AC,
                HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_5_5DIG);
   }
#endif

 
 AnalogLevelCalOk = TRUE;
 NoOfOkMeas = 0;  // antal målinger indenfor tol
 NoOfMeas = 0;    // antal målinger
 CalOk = FALSE;
 
 // 1. måling
 AudioPOTMValue = 210;  // startværdi for audio potmeter
 if (ChNo == LEFTCH)
   AudioPOTMValueLeft = AudioPOTMValue;
  else 
   AudioPOTMValueRight = AudioPOTMValue;

 SetAudioPOTM(AudioPOTMValueLeft, AudioPOTMValueRight);
 DisableBreakOnLibraryErrors();
 ReadACVolt(&meas1);
 EnableBreakOnLibraryErrors();
 meas1 *= 1000;
 SetCtrlVal (anPNL, ANALOG_PNL_ANALOGAUDIOLEVEL, meas1);
 SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX," *");

 // 2. måling
 AudioPOTMValue -= 30;
 if (ChNo == LEFTCH)
   AudioPOTMValueLeft = AudioPOTMValue;
  else 
   AudioPOTMValueRight = AudioPOTMValue;

 SetAudioPOTM(AudioPOTMValueLeft, AudioPOTMValueRight);
 DisableBreakOnLibraryErrors();
 ReadACVolt(&meas2);
 EnableBreakOnLibraryErrors();
 meas2 *= 1000;
 SetCtrlVal (anPNL, ANALOG_PNL_ANALOGAUDIOLEVEL, meas2);
 SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX," *");
 
 // beregn potm værdi 
 mVprStep = (meas2-meas1) / 30;
 AudioPOTMValue = AudioPOTMValue - ((NomAnalogAudioLevel - meas2) / mVprStep);

 // check mV pr step
 if ( (fabs(mVprStep) < MinmVStep) || (fabs(mVprStep) > MaxmVStep)){
  AnalogLevelCalOk = FALSE;
  Fmt(ErrStr,"%s< mV pr potmeter step udenfor tol (%f[p2w5] - %f[p2w5]):  %f[p2w5]",fabs(MinmVStep),fabs(MaxmVStep),fabs(mVprStep));
  Fmt(ErrStr,"%s[a]<\n Typisk værdi: 2mV/step\n Check modstande og lodninger");
  
  MessagePopup (" Analog Audio Level Kalibrering ",ErrStr);
  HidePanel (anPNL);
  return (CalOk = FALSE);
 }

 SetCtrlVal(anPNL, ANALOG_PNL_LED, ON);

 done = FALSE;
  do {
    if (ChNo == LEFTCH)
      AudioPOTMValueLeft = AudioPOTMValue;
     else 
      AudioPOTMValueRight = AudioPOTMValue;

    SetAudioPOTM(AudioPOTMValueLeft, AudioPOTMValueRight);

    AudioPOTMValueSend = AudioPOTMValue;
     GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);

    DisableBreakOnLibraryErrors();
    ReadACVolt(&meas1);
    EnableBreakOnLibraryErrors();
    meas1 *= 1000;  // to mV
    SetCtrlVal (anPNL, ANALOG_PNL_ANALOGAUDIOLEVEL, meas1);
    SetCtrlVal(anPNL, ANALOG_PNL_TXTBOX," *");
    ProcessDrawEvents();
     
    if (meas1 > NomAnalogAudioLevel)
        AudioPOTMValue++;
     else
      AudioPOTMValue--;

    LevelInsideTol =  ((meas1 > (NomAnalogAudioLevel - AnalogAudioLevelTol)) && 
                 (meas1 < (NomAnalogAudioLevel + AnalogAudioLevelTol)));
      
    if (LevelInsideTol){
     NoOfOkMeas++;
     SetCtrlAttribute (anPNL, ANALOG_PNL_LED, ATTR_ON_COLOR, VAL_GREEN);
     }
    else {
     NoOfOkMeas = 0;
     SetCtrlAttribute (anPNL, ANALOG_PNL_LED, ATTR_ON_COLOR,VAL_RED);
     }

    
     if ((NoOfOkMeas > RequiredNoOfOkMeas) && (done == FALSE))    // mindst 3 OK-målinger i træk før godkendelse
      CalOk = TRUE;
     else 
      CalOk = FALSE;
      
     NoOfMeas++; 
  } while ((CalOk == FALSE) && (done == FALSE) && (NoOfMeas <= MaxNoOfMeas));

 if (done == TRUE){
  //MessagePopup("Analog Audio Level Kalibrering","Stoppet af bruger");
  HidePanel (anPNL);
  return (CalOk = FALSE);
 }

 if (NoOfMeas > MaxNoOfMeas){
   Fmt(ErrStr,"%s< Fejlet efter mere end %i målinger\n POTM = %i",MaxNoOfMeas,AudioPOTMValueSend);
   MessagePopup (" Audio Level Kalibrering",ErrStr);
  HidePanel (anPNL);
   return (CalOk = FALSE);
 }

 if ((CalOk == TRUE) && (ChNo == RIGHTCH))
   SaveAudioPOTM(AudioPOTMValueLeft,AudioPOTMValueRight);

 HidePanel (anPNL);
return (CalOk);
} // CalAnalogAudioLevel


//-------------------------------------------------------------------------------
void MeasAnalogAudioLevel(void){  // kaldt fra 'Diverse' menu
int handle_returned, 
   controlID_returned;    
double  meas,mVmeas, 
      dBmLevel; 
ViChar mm[30];

 ResetTextBox(anmeasPNL, ANMEAS_PNL_TXTBOX,"\n");
 SetCtrlVal(anmeasPNL, ANMEAS_PNL_TXTBOX," Kabel fra audio generator (XLR) forbindes til HP34970 LEFT channel \n\n");
 SetCtrlVal(anmeasPNL, ANMEAS_PNL_TXTBOX," Stop målinger:   Tast F4\n");

 DisplayPanel (anmeasPNL);

 WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP ANAL;");     // analog audio mode
 WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:SIGN S1KHZ;"); // analog audio signal
 WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:LEV +6;");     // analog audio level = 1550mV
 
 // set HP34970 for ac-measurements
#if IEEEBUS
 hp34970a_voltageMeasure (hp34970, "105", HP34970A_VM_VTYPE_AC,
                  HP34970A_VM_VRANGE_10V,
                  HP34970A_VM_RES_5_5DIG, mm);

 hp34970a_voltageSetup (hp34970, "105", HP34970A_VS_VTYPE_AC,
                HP34970A_VS_VRANGE_10V, HP34970A_VS_RES_5_5DIG);
 
#endif

 meas = 0.772; 
 done = FALSE;
  do {

     GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);

    DisableBreakOnLibraryErrors();
    ReadACVolt(&meas);
    EnableBreakOnLibraryErrors();
    dBmLevel = 20 * log10(meas/0.775);
    mVmeas = meas * 1000;
    if (mVmeas <= 400)
     SetCtrlAttribute (anmeasPNL, ANMEAS_PNL_METER, ATTR_MAX_VALUE, 500.0);
    else 
    if (mVmeas <= 1400)
     SetCtrlAttribute (anmeasPNL, ANMEAS_PNL_METER, ATTR_MAX_VALUE, 1500.0);
    else 
     SetCtrlAttribute (anmeasPNL, ANMEAS_PNL_METER, ATTR_MAX_VALUE, 4000.0);
     
    SetCtrlVal (anmeasPNL, ANMEAS_PNL_METER, mVmeas);
    Fmt(TxtStr,"%s<%f[p0w6]",mVmeas);
    SetCtrlVal (anmeasPNL, ANMEAS_PNL_LEVELMV, TxtStr);
    Fmt(TxtStr,"%s<%f[p1w6]",dBmLevel);
    SetCtrlVal (anmeasPNL, ANMEAS_PNL_LEVELDBM, TxtStr);
    ProcessDrawEvents();
     
  } while (done == FALSE);

 HidePanel (anmeasPNL);
 } //MeasAnalogAudioLevel

//-------------------------------------------------------------------------
int AudioGenTotalTest(int TestNo){

  AudioGenOk = FALSE;
  
  if (TestNo == 1)
   TestAESAudioGen();
    
  if ((TestNo == 2) || (AudioGenOk == TRUE))
   TestAnalogAudioGen();

  if ((TestNo == 3) || (AudioGenOk == TRUE))
   TestWordClockOutput();

return TRUE;
}


//---------------------------------------------------------------------
void SaveAudioPOTM(unsigned char dacleft, unsigned char dacright){


 Fmt(DUTStr,"%s<:FACT:AUD:ALEV %i,%i;",dacleft,dacright);   // Save audio cal data for left and right channel
 WriteCOMPort(DUTCOM,DUTStr);
 
 // Save audio cal date
 GetDate(2);
 Fmt(DUTStr,"%s<:FACT:AUD:DATE %s,%s,%s;",YearStr,MonthStr,DayStr);
 WriteCOMPort(DUTCOM,DUTStr);

 // Save last update = last calibration date
 Delay(0.03);
 Fmt(DUTStr,"%s<:FACT:MAIN:FMSD:UPD %s,%s,%s;",YearStr,MonthStr,DayStr);
 WriteCOMPort(DUTCOM,DUTStr);
}




//-----------------------------------------------------------------------------
void SetAudioPOTM(unsigned char dacleft, unsigned char dacright){

 Fmt(DUTStr,"%s<:FACT:AUD:ALEV %i,%i;",dacleft,dacright);   // Save audio cal data for left and right channel
 WriteCOMPort(DUTCOM,DUTStr);

 Fmt(TxtStr,"%s< %i[p3]  %i[p3]",dacleft, dacright);
 SetCtrlVal (anPNL, ANALOG_PNL_POTMVALUE, TxtStr);
#if DEBUG
 SetCtrlVal (anPNL, ANALOG_PNL_COMMSTR, DUTStr);
#endif
 Delay(0.5);

}



char dastr[10];
//----------------------------------------------------------------------------------
char *ReadAudioCalDate (void) {
// read Audio generator kalibrerings dato
int dd[3];

  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:AUD:DATE?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(dastr,"%s<%i[w2p0]%i[w2p0]%i[w2p0]",dd[0],dd[1],dd[2]);  //yymmdd
  } 
  else
   Fmt(dastr,"%s<??????");  //yymmdd

 return dastr;
}






//-------------------------------------------------------------------------------
int InitHP53132_AES(void) {
#if IEEEBUS
 // set input trigger level/slope/attenuation/sensitivity
 ErrIEEE = hp5313xa_trigSens (hp53132, 1, VI_ON, 0.0, VI_OFF, VI_ON, 1);
 ErrIEEE = hp5313xa_trigSens (hp53132, 2, VI_ON, 0.0, VI_OFF, VI_ON, 1);
#endif 
return 0;
}








int CVICALLBACK AESOkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:  Cont = TRUE; 
                  AudioGenOk = TRUE;
                  break;
  }
  return 0;
}

int CVICALLBACK AESExitCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT:  Cont = TRUE; 
                  done = TRUE;
                  AudioGenOk = FALSE; 
                  break;
    
  }
  return 0;
}


int CVICALLBACK MeasOkCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:  done = TRUE;

      break;
  }
  return 0;
}


int CVICALLBACK AnalogRightChCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:
        GetCtrlVal (anmeasPNL, ANMEAS_PNL_RIGHTSLIDE, &AudioPOTMValueRight);
        SetAudioPOTM(AudioPOTMValueRight, 1);
      break;
  }
  return 0;
}



int CVICALLBACK AnalogLeftChCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:
        GetCtrlVal (anmeasPNL, ANMEAS_PNL_LEFTSLIDE, &AudioPOTMValueLeft);
        SetAudioPOTM(AudioPOTMValueLeft, 2);

      break;
  }
  return 0;
}

#if 1
int CVICALLBACK FreqSelCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
int freqsel;
  switch (event) {
    case EVENT_VAL_CHANGED:
      GetCtrlVal (anmeasPNL, ANMEAS_PNL_FREQSLIDE, &freqsel);
       switch (freqsel){
        case 0: WriteCOMPort(DUTCOM,":OUTP:AUD:FREQ S500HZ;"); break;
        case 1: WriteCOMPort(DUTCOM,":OUTP:AUD:FREQ S1000HZ;"); break;
        case 2: WriteCOMPort(DUTCOM,":OUTP:AUD:FREQ S1000HZCLICK;"); break;
        case 3: WriteCOMPort(DUTCOM,":OUTP:AUD:FREQ S8000HZ;"); break;
       } 

    break;
  }
  return 0;
}


int CVICALLBACK LevelSelCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
int levelsel;
  switch (event) {
    case EVENT_VAL_CHANGED:
      GetCtrlVal (anmeasPNL, ANMEAS_PNL_LEVELSEL, &levelsel);
      WriteCOMPort(DUTCOM,":OUTP:AUD:LEVEL levelsel<<<<;");
    break;
  }
  return 0;
}
#endif