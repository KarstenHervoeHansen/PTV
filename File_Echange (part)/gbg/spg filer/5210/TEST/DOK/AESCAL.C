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


#define  IEEEBUS 1


const double AESFreqTol = 0.1;   // Hz	   Audio tones
const double Freq48kHzTol = 1;   // Hz     48 kHz ref

const double AESBNCRiseTimeMax = 44;   // ns
const double AESBNCRiseTimeMin = 30;   // ns
const double AESBNCFallTimeMax = 44;   // ns
const double AESBNCFallTimeMin = 30;   // ns

const double AESBNCVoltppMax = 1.1;    // V
const double AESBNCVoltppMin = 0.9;    // V

const double AESBNCVoltOffsetMax = 0.05;   // V


int AESFreqOk;
int AudioGenOk;


int InitAudioGen(void);
int ReadCounter_AES(double CHAFreq,double CHBFreq);
int InitHP53132_AES(void);

ViReal64 RiseTime,FallTime,Voltpp,VoltOffset;







//-----------------------------------------------------------------------------------
int TestAudioGen(void){
int handle_returned, controlID_returned;
char *strread,TxtStr[50];
int IICport[2];
int AudioVideoLocked, n;
ViReal64  VoltMax,VoltMin;

 DisplayPanel (aesPNL);
 AudioGenOk = TRUE;
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 
 AudioGenOk = WriteProtect(OFF,FALSE);
 if (AudioGenOk == FALSE){
  HidePanel (aesPNL);
  return AudioGenOk;
 }
 
 SelectPM3094Setup(3);
 WriteCOMPort(DUTCOM,":OUTP:AUD:SIGN S800HZ; LEV DBM9FS; TIM PAL;");

 
 DevClear (0, 3);
 InitHP53132_AES();
 InitAudioGen();
 
 
 
 
// VCO spænding -------------------------------------------------------------
 AudioGenOk = CheckAudioGenVCO();   // function findes i bbcal.c
 if (AudioGenOk == FALSE){
  MessagePopup (" Audio generator test ", " FEJLET ");
  HidePanel (aesPNL);
  return AudioGenOk;
 }




 // 48kHz ref test -----------------------------------------------------------
 DisplayPanel(aesPNL);
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," AES BNC forbindes til HP53132 CH-1\n\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," OK:    Tast F4\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Fejl:  Tast F9\n\n");


 WaitForContinue();
 if (AudioGenOk == FALSE){
  MessagePopup (" Audio generator test ", " FEJLET ");
  HidePanel (aesPNL);
  return FALSE;
 }

 
// 48 kHz   ---------------------------------------------------------------
 WriteCOMPort(DUTCOM,":OUTP:AUD:SIGN F48KHZ;");
 Delay(2);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," 48 kHz");
 
 AESFreqOk = ReadCounter_AES(48000,0);

 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"   OK\n\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"   FAIL\n");
   AudioGenOk = FALSE;
  }
  

 
// Rise/Fall time   ---------------------------------------------------------------
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Risetime     ");
 ErrIEEE = hp5313xa_inputCond (hp53132, 1, VI_OFF, VI_ON, VI_OFF,VI_OFF);
 hp5313xa_writeInstrData (hp53132, ":MEAS:SCAL:VOLT:RISE:TIME?");
 Delay(3);
 ErrIEEE = hp5313xa_mathRes (hp53132, &RiseTime);
 RiseTime = RiseTime * 1e9;
 if ((RiseTime > AESBNCRiseTimeMax) || (RiseTime < AESBNCRiseTimeMin))
  AudioGenOk = FALSE;
 
 if (AudioGenOk == TRUE)
   Fmt(TxtStr,"%s<%f[p1] ns    OK\n",RiseTime);
  else 
   Fmt(TxtStr,"%s<%f[p1] ns    FAIL   (%f[p1] - %f[p1])\n",RiseTime,AESBNCRiseTimeMin,AESBNCRiseTimeMax);
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 

 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Falltime     ");
 hp5313xa_writeInstrData (hp53132, ":MEAS:SCAL:VOLT:FALL:TIME?");
 Delay(3);
 ErrIEEE = hp5313xa_mathRes (hp53132, &FallTime);
 FallTime = FallTime * 1e9;
 if ((FallTime > AESBNCFallTimeMax) || (FallTime < AESBNCFallTimeMin))
  AudioGenOk = FALSE;
 
 if (AudioGenOk == TRUE)
   Fmt(TxtStr,"%s<%f[p1] ns    OK\n",FallTime);
  else 
   Fmt(TxtStr,"%s<%f[p1] ns    FAIL   (%f[p1] - %f[p1])\n",FallTime,AESBNCFallTimeMin,AESBNCFallTimeMax);
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);



// BNC Vpp offset  ---------------------------------------------------------------
 ErrIEEE = hp5313xa_inputCond (hp53132, 1, VI_ON, VI_ON, VI_OFF,VI_OFF);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Volt pp    ");
 hp5313xa_writeInstrData (hp53132, ":MEAS:SCAL:VOLT:MAX?");
 Delay(2);
 ErrIEEE = hp5313xa_dataRes (hp53132, &VoltMax);
 VoltMax = VoltMax /2;   // der måles uden afslutning

 hp5313xa_writeInstrData (hp53132, ":MEAS:SCAL:VOLT:MIN?");
 Delay(2);
 ErrIEEE = hp5313xa_dataRes (hp53132, &VoltMin);
 VoltMin = VoltMin /2;  // der måles uden afslutning
 
 Voltpp = VoltMax - VoltMin - 0.02;
 VoltOffset = fabs(VoltMax) - fabs(VoltMin);
 
 if ((Voltpp > AESBNCVoltppMax) || (Voltpp < AESBNCVoltppMin))
  AudioGenOk = FALSE;
 if (AudioGenOk == TRUE)
   Fmt(TxtStr,"%s<%f[p2w5] Vpp    OK\n",Voltpp);
  else 
   Fmt(TxtStr,"%s<%f[p2w5] Vpp    FAIL   (%f[p1] - %f[p1])\n",Voltpp,AESBNCVoltppMax,AESBNCVoltppMin);
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," DC Offset  ");
 if (VoltOffset > AESBNCVoltOffsetMax)
  AudioGenOk = FALSE;
 if (AudioGenOk == TRUE)
   Fmt(TxtStr,"%s<%f[p2w5] VDC    OK\n",VoltOffset);
  else 
   Fmt(TxtStr,"%s<%f[p2w5] VDC    FAIL   (max %f[p1])\n",VoltOffset,AESBNCVoltOffsetMax);
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);
 
 ErrIEEE = hp5313xa_inputCond (hp53132, 1, VI_OFF, VI_ON, VI_OFF,VI_OFF);
 
 ProcessDrawEvents();
 Delay(1);
 



 // Audio freq test ------------------------------------
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," AES BNC forbindes til TEK 764 AES 75 input\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," AES XLR forbindes til TEK 764 AES 110 input\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 Menu: Input CH 1-2: BNC-unbalanced \n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"              Input CH 3-4: XLR-balanced \n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 Menu: Headphones Audio Source = MANUAL\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"                         Manual Left CH-1    Manual Right CH-2\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 HEADPHONE kabel forbindes til HP53132 counter CH-1 og CH-2\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 VIEW: AUDIO\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," TEK764 VOLUME drejes i max  (helt højre om)\n\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," OK:    Tast F4\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Fejl:  Tast F9\n\n");


 WaitForContinue();
 if (AudioGenOk == FALSE){
  MessagePopup (" Audio generator test ", " FEJLET ");
  HidePanel (aesPNL);
  return FALSE;
 }

 
// Dual 1000,400Hz 
 DevClear (0, 3);
 InitHP53132_AES();
 WriteCOMPort(DUTCOM,":OUTP:AUD:SIGN DUAL; LEV DB0FS; TIM PAL;");
 Delay(2);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Dual 1 kHz / 400 Hz");
 
 AESFreqOk = ReadCounter_AES(1000,400);

 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"   OK\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"   FAIL\n");
   AudioGenOk = FALSE;
  }
  


 WriteCOMPort(DUTCOM,":OUTP:AUD:SIGN S800HZ;");
 Delay(2);

 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Stereo 800 Hz");
 
 AESFreqOk = ReadCounter_AES(800,800);
 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"         OK\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"         FAIL\n");
   AudioGenOk = FALSE;
  }
  
 //  Stereo 1 kHz 
 WriteCOMPort(DUTCOM,":OUTP:AUD:SIGN S1KHZ;");
 Delay(2);
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Stereo 1 kHz");
 
 AESFreqOk = ReadCounter_AES(1000,1000);
 if (AESFreqOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"          OK");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"          FAIL");
   AudioGenOk = FALSE;
  }
  
 
 Delay(1);

 if (AudioGenOk == FALSE){
  MessagePopup (" Audio generator test ", " FEJLET ");
  HidePanel (aesPNL);
  return FALSE;
 }

 


// ------- Audio level test ------------------------------------------------------
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"\n\n Aflæs level på TEK 764\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Level skal skifte mellem 0, -9, -12, -14, -16, -18, -20 og MUTE\n\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Check at der er signal i både CH-1, CH-2, CH-3 og CH-4\n\n");
 
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," OBS: Mellem levelskiftene vises der muligvis CODE ERROR. Dette er ingen fejl!\n\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," OK:    Tast F4\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Fejl:  Tast F9\n\n");
 
 Cont = FALSE;
 while (Cont == FALSE) {
 
 WriteCOMPort(DUTCOM,":OUTP:AUD:LEV DB0FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:AUD:LEV DBM9FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:AUD:LEV DBM12FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:AUD:LEV DBM14FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:AUD:LEV DBM16FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:AUD:LEV DBM18FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:AUD:LEV DBM20FS;");
 Delay(2);
 GetUserEvent (NOWAIT, &handle_returned, &controlID_returned);
 if (Cont == TRUE) break;
 WriteCOMPort(DUTCOM,":OUTP:AUD:LEV SIL;");
 Delay(2);
 
 } // while

 WriteCOMPort(DUTCOM,":OUTP:AUD:SIGN DUAL:LEV DBM9FS;");
 
 if (AudioGenOk == TRUE)
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Audio level test  OK\n");
  else {
   SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Audio level test  FEJLET\n");
   HidePanel(aesPNL);
   return AudioGenOk;
   }
 Delay(1);



// ------- Audio/Video Lock test --------------------------------------- 
 ResetTextBox(aesPNL, AES_PNL_TXTBOX,"");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Test af audio-video lock:\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Lysdioden D1 skal lyse konstant\n");
 SetCtrlVal(aesPNL, AES_PNL_TXTBOX," Lysdioden D4 må gerne slukke et kort øjeblik mellem hver lock\n\n");
 
 AudioVideoLocked = TRUE;

 for (n = 0; n < 6; n++){
  switch (n){
    case 0 : SetCtrlVal(aesPNL, AES_PNL_TXTBOX," PAL lock          ");
    		 WriteCOMPort(DUTCOM,":OUTP:AUD:TIM PAL;"); 
    		 break;
    case 1 : SetCtrlVal(aesPNL, AES_PNL_TXTBOX," NTSC Phase 1  lock"); 
    		 WriteCOMPort(DUTCOM,":OUTP:AUD:TIM NTSC1;"); 
    		 break;
    case 2 : SetCtrlVal(aesPNL, AES_PNL_TXTBOX," NTSC Phase 2  lock"); 
    		 WriteCOMPort(DUTCOM,":OUTP:AUD:TIM NTSC2;"); 
    		 break;
    case 3 : SetCtrlVal(aesPNL, AES_PNL_TXTBOX," NTSC Phase 3  lock");
    		 WriteCOMPort(DUTCOM,":OUTP:AUD:TIM NTSC3;"); 
			 break;
    case 4 : SetCtrlVal(aesPNL, AES_PNL_TXTBOX," NTSC Phase 4  lock"); 
    		 WriteCOMPort(DUTCOM,":OUTP:AUD:TIM NTSC4;"); 
    		 break;
    case 5 : SetCtrlVal(aesPNL, AES_PNL_TXTBOX," NTSC Phase 5  lock"); 
    		 WriteCOMPort(DUTCOM,":OUTP:AUD:TIM NTSC5;"); 
    		 break;
   }

  Delay(1.5);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:AUD:READ?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%2i[x]",IICport);		// PORT0=V5   PORT1=V9   (se diagram 4008 109 82910)
  											// PORT0: bit0-2:L0-2     bit 3-6:S0-3    bit7:27MHz lock
  											// PORT1: bit0-2:VS0-2    bit 3:VIDLOCK   bit4-7:not used
  											
  if ( ((IICport[1] & 0x08) != 0x08) || ((IICport[0] & 0x80) != 0x80) )
   AudioVideoLocked = FALSE;
 
  if (AudioVideoLocked == TRUE)
    SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"      OK\n");
   else
    SetCtrlVal(aesPNL, AES_PNL_TXTBOX,"      FAIL\n");
   
 
 } //for
 
 WriteCOMPort(DUTCOM,":OUTP:AUD:TIM PAL;"); 
 
 if (AudioVideoLocked == FALSE)
  AudioGenOk = FALSE;

 

 
 
 if (AudioGenOk = TRUE)
   MessagePopup(" Audio generator test ","OK");
  else
   MessagePopup(" Audio generator test ","FEJLET");

 HidePanel(aesPNL);

return AudioGenOk;
}






//----------------------------------------------------------------------------------
char *ReadAudioKUNumber (void) {
  // read audio generator KU nummer  (user text 2)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:AUD:TEXT? 2;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,3,6);
 return strread;
}						  



//----------------------------------------------------------------------------------
char *ReadAudioProdDate (void) {
  // read audio generator produktions dato  (user text 3)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:AUD:TEXT? 3;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}


//----------------------------------------------------------------------------------
char *ReadAudioCalDate (void) {
  // read audio generator kalibrerings dato  (user text 4)
char *strread;
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:AUD:TEXT? 4;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  CopyString(strread,0,strread,1,6);

 return strread;
}


//-------------------------------------------------------------------------------
int ReadCounter_AES(double CHAFreq,double CHBFreq){
char TxtStr[50];
ViReal64 freqmeasA,freqmeasB;
 
 DisableBreakOnLibraryErrors();

 if (CHBFreq > 0)
   ErrIEEE = hp5313xa_freqPeriodRatio (hp53132, 1, 1, 6, VI_ON, 3);
  else
   ErrIEEE = hp5313xa_freqPeriodRatio (hp53132, 1, 1, 6, VI_ON, 3);
 Delay(0.2);
 hp5313xa_writeInstrData (hp53132, ":READ?");
 Delay(1.2);
 ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeasA);
 Delay(1.0);

 if (CHBFreq > 0){		
  ErrIEEE = hp5313xa_freqPeriodRatio (hp53132, 2, 1, 6, VI_ON, 3);
  Delay(0.2);
  hp5313xa_writeInstrData (hp53132, ":READ?");
  Delay(1.2);
  ErrIEEE = hp5313xa_mathRes (hp53132, &freqmeasB);
  Delay(0.2);
 } 
 EnableBreakOnLibraryErrors();
 
 if (CHBFreq > 0){				   // Audio tones meas
  if ((fabs(CHAFreq - freqmeasA) > AESFreqTol) || 
      (fabs(CHBFreq - freqmeasB) > AESFreqTol))
   return FALSE;
  else
   return TRUE;
 }
 else{							   // 48 kHz meas
  if (fabs(CHAFreq - freqmeasA) > Freq48kHzTol)
   return FALSE;
  else
   return TRUE;
 }
 
// Fmt(TxtStr,"%s<\n%f[p2]   %f[p2]",freqmeasA,freqmeasB);
// SetCtrlVal(aesPNL, AES_PNL_TXTBOX,TxtStr);

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



//----------------------------------------------------------------------------------
int InitAudioGen(void){

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
 WriteCOMPort(DUTCOM, ":FACT:AUD:TEXT 0,'PHILIPS';");
 Delay(0.2);

 // 1  12nc
 Fmt(DUTStr,"%s<:FACT:AUD:TEXT 1,'%s';",NC12[1]);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.5);
 
 
 
  // 2  KU nummer
  KUread = ReadAudioKUNumber();
  if (isdigit(KUread[2]) == FALSE)	
    CopyString(KUread,0,"",0,-1);
    
  do {
   ValidKU = TRUE;
   PromptPopup ("Audio generator:   Indtast KU nummer", KUread, response, 6);

   if ((StringLength(response) == 0) & (StringLength(KUread) == 6))  // Only ENTER pressed
    CopyString(response,0,KUread,0,-1);

   for (n = 0; n < 6; n++)
    if (isdigit(response[n]) == FALSE)		  // digits only
     ValidKU = FALSE;
   
  
   if (StringLength(response) != 6)			  // exactly 6 digits
    ValidKU = FALSE;
  } while (ValidKU == FALSE);
  Fmt(DUTStr,"%s<:FACT:AUD:TEXT 2,'KU%s';",response);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 
 // get system date 
 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // 3  produktionsdato
 usertxt = ReadAudioProdDate();
 if (isdigit(usertxt[2]) == FALSE){			  
  Fmt(DUTStr,"%s<:FACT:AUD:TEXT 3,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 }

 // 4  kalibreringsdato 
 usertxt = ReadAudioCalDate();
 if (isdigit(usertxt[2]) == FALSE){			  
  Fmt(DUTStr,"%s<:FACT:AUD:TEXT 4,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.2);
 } 

 // 6  PT-nummer
 WriteCOMPort(DUTCOM, ":FACT:AUD:TEXT 6,'PT8605';");
 Delay(0.2);

 // 7  reserveret - ikke defineret.

return TRUE;
} // InitAudioGen










int CVICALLBACK AESOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: Cont = TRUE; AudioGenOk = TRUE;
						break;
	}
	return 0;
}

int CVICALLBACK AESExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT: Cont = TRUE; AudioGenOk = FALSE; 
						break;
		
	}
	return 0;
}

