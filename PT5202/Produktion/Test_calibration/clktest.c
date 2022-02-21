#include <rs232.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <userint.h>
#include "def.h"
#include "cviutil.h"
#include "bbcal.h"


// panel header files
#include "ltc.h"


char ClockLogFile[2][50] = {"c:\\5230log\\clock.log","m:\\measdata\\5230\\clock.log"};

char ClockKUStr[20];


// prototypes --------------------------------------------
int Clock_VITCTest(int TVSystem);
int Clock_LTCTest(int TVSystem);
int Clock_1HzTest(int mode);
int CheckRAMBatteryStatus (int testmode);




//----------------------------------------------------------------------------------
char *ReadClockKUNumber (void) {
  // read Time Clock  KU nummer  (user text 2)
char *strread;
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TIM:TEXT? 2;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 CopyString(strread,0,strread,3,6);
return strread;
}						  



//----------------------------------------------------------------------------------
char *ReadClockProdDate (void) {
  // read Time Clock produktions dato  (user text 3)
char *strread;
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TIM:TEXT? 3;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 CopyString(strread,0,strread,1,6);

return strread;
}


//----------------------------------------------------------------------------------
char *ReadClockCalDate (void) {
  // read Time Clock kalibrerings dato  (user text 4)
char *strread;
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TIM:TEXT? 4;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 CopyString(strread,0,strread,1,6);

return strread;
}


char *ReadClockPTNo (void) {
  // read Time Clock PT86xx number
char *strread;
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TIM:TEXT? 6;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 CopyString(strread,0,strread,1,6);

return strread;
}


char *ReadClockSWVer (void) {
  // read Time Clock CPU software version
char *strread;
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":OUTP:TIM:VERS?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);

return strread;
}




int CheckRAMBatteryStatus (int testmode) {
// testmode:  1: checker at apparat er tændt og sætter password = off
//               viser resultat og returnerer status	     
//            0: returnerer kun status
// status:  TRUE:  Battery OK       
//          FALSE: Battery FAIL 
char *strread;
int status;

 if (testmode == 1)
  if (WriteProtect(OFF,FALSE) == FALSE)
   return FALSE;

 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TIM:STAT?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%i",&status);
 status = (status & 0x0001);	   // bit 0: RAM battery status:  0=ok    1=fail
 status = ((status == 0) && (RS232Timeout == FALSE));
 
 if (testmode == 1){
  if (status == TRUE)
    MessagePopup(" PT8637 "," Batteri i V7 (M48T86)  = OK");
   else
    MessagePopup(" PT8637 "," Batteri i V7 (M48T86)  = Defekt !!");
 }   

return status;
}




//------------------------------------------------------------------
int InitClock(void){

int n;
int ValidKU;
char *usertxt;
char *dd;					// MM-DD-YYYY
char dato[10];  			// YYMMDD
char year[5],
     month[5],
     day[5];
unsigned char response[10]; // svar fra PromptPopup
char KUStr[10];  			// KUxxxxxx
char *KUread;		 		// Læst KU nummer
char PromptStr[50];
 

 // 0  producent
 WriteCOMPort(DUTCOM, ":FACT:TIM:TEXT 0,'PTV';");
 Delay(0.2);

 // 1  12nc
 Fmt(DUTStr,"%s<:FACT:TIM:TEXT 1,'%s';",NC12[15]);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.2);
 
 
 
  // 2  KU nummer
 KUread = ReadClockKUNumber();
 if (RS232Timeout == TRUE){
  MessagePopup(" PT8637 "," Ingen kontakt til option");
  return(FALSE);
  }

 if (isdigit(KUread[2]) == FALSE)	
   CopyString(KUread,0,"",0,-1);
    
 EnterKUNo(" Time Clock Interface", KUread, response);
  
  
 Fmt(DUTStr,"%s<:FACT:TIM:TEXT 2,'KU%s';",response);
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.1);
 
 // get system date 
 dd = DateStr();
 CopyString(dato,0,dd,8,2);
 CopyString(dato,StringLength(dato),dd,0,2);
 CopyString(dato,StringLength(dato),dd,3,2);
 
 // 3  produktionsdato
 usertxt = ReadClockProdDate();
 if (isdigit(usertxt[2]) == FALSE){			  
  Fmt(DUTStr,"%s<:FACT:TIM:TEXT 3,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.1);
 }

 // 4  kalibreringsdato 
 usertxt = ReadClockCalDate();
 if (isdigit(usertxt[2]) == FALSE){			  
  Fmt(DUTStr,"%s<:FACT:TIM:TEXT 4,'%s';",dato);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.1);
 } 

 // 6  PT-nummer
 WriteCOMPort(DUTCOM, ":FACT:TIM:TEXT 6,'PT8637';");
 Delay(0.1);

 // 7  reserveret - ikke defineret.


 // set date in M48T86 clock module
 Fmt(DUTStr,"%s<:OUTP:TIM:DFOR DMY;");
 WriteCOMPort(DUTCOM, DUTStr);
 Delay(0.1);
 CopyString (year, 0, dato, 0, 2);
 CopyString (month, 0, dato, 2, 2);
 CopyString (day, 0, dato, 4, 2);
 Fmt(DUTStr,"%s<:OUTP:TIM:DAT %s,%s,%s;",year,month,day);
 WriteCOMPort(DUTCOM, DUTStr);
 

return TRUE;
} // InitClock






//------------------------------------------------------------------------------
int TimeClockTotalTest(int TestNo){

 ClockOk = FALSE;
	
 if (TestNo == 1)
  if (Clock_VITCTest(PAL) == TRUE)
    Clock_VITCTest(NTSC);
    
 if ((TestNo == 2) || (ClockOk == TRUE))
  if (Clock_LTCTest(PAL) == TRUE)
    Clock_LTCTest(NTSC);
    
 if ((TestNo == 3) || (ClockOk == TRUE))
   Clock_1HzTest(TRUE);

return ClockOk;
}





//----------------------------------------------------------------------------------
int Clock_VITCTest(int TVSystem){
int handle_returned, 
    bytes_written,
    fhandle;
char *strread,
     MasterSPGSW[15],
     TxtStr[80],
	 wrtbuf[100],
     *d,			// MM-DD-YYYY
	 *tt;			// HH:MM:SS

 DisplayPanel(clockPNL);
 SetCtrlVal (clockPNL, CLK_PNL_BATTERYSTATUS, "");
 ResetTextBox (clockPNL, CLK_PNL_TXTBOX, "");
 ProcessDrawEvents();

 ClockOk = WriteProtect(OFF,FALSE);
 if (ClockOk == FALSE){
  HidePanel (clockPNL);
  return ClockOk;
 }

 if (TVSystem == PAL){
  ClockOk = InitClock();
  if (ClockOk == FALSE){
   HidePanel(clockPNL);
   return ClockOk;
  }
 }
 
 if (TVSystem == PAL){
  ClockOk = CheckRAMBatteryStatus(0);
  if (ClockOk == FALSE){
   MessagePopup(" Batteri i M48T86  (V7) "," Defekt !!");
   HidePanel(clockPNL);
   return ClockOk;
  }
 }
 
 if (ClockOk == TRUE)
  SetCtrlVal (clockPNL, CLK_PNL_BATTERYSTATUS, "RAM batteri: OK");
 
 
 
 Set5640Vits(VITSEXTDATA);
 
 if (TVSystem == PAL){
   Set5640Standard("G");
   SetPanelAttribute (clockPNL, ATTR_TITLE, " VITC test: (PAL) ");
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST PALB;");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Forbind 5640G VAR VIDEO til DUT GENLOCK A\n");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Sæt VITC omskifter = G \n\n");
  }
  else{
   Set5640Standard("M");
   SetPanelAttribute (clockPNL, ATTR_TITLE, " VITC test: (NTSC) ");
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST NTSC;");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Forbind 5640M VAR VIDEO til DUT GENLOCK A\n");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Sæt VITC omskifter = M \n\n");
  }
 
 Delay(0.05);
 WriteCOMPort(DUTCOM,":OUTP:TIM:REF VITC;");

 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Forbind DUT GENLOCK B til afslutning = 75 ohm\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Kontroller at tiden på DUT status display tæller op 12:3x:xx\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               REF = VITC on genlock\n");
 if (TVSystem == PAL)
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               STATUS = VITC(PAL)\n\n");
  else 
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               STATUS = VITC(NTSC)\n\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Kontroller med 25 ohm og derefter uden afslutning at tiden tæller korrekt op\n\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Ved OK:   Tryk F4");

 ProcessDrawEvents();
 WaitForContinue();

 if (TVSystem == PAL)
  if (ClockOk == FALSE)
   MessagePopup(" PT8637:  VITC Time Code  (PAL)"," FEJL ");
 			 
 
 if (TVSystem == NTSC)
  if (ClockOk == TRUE)
    MessagePopup(" PT8637:  VITC Time Code  "," OK ");
   else
    MessagePopup(" PT8637:  VITC Time Code  (NTSC)"," FEJL ");
    
    
 if (ClockOk == TRUE){
  // get system date 
  d = DateStr();
  CopyString(dato,0,d,8,2);
  CopyString(dato,strlen(dato),d,0,2);
  CopyString(dato,strlen(dato),d,3,2);
 
  // get system time
  tt = TimeStr();
  
  strread = ReadClockKUNumber();
  CopyString(ClockKUStr,0,strread,0,-1);
  
  strread = ReadMasterSWVer();
  CopyString(MasterSPGSW,0,strread,0,-1);
  
  // check om logfil findes, ellers opret en ny logfil
  fhandle = OpenFile (ClockLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
  if (TVSystem == PAL)
    Fmt(wrtbuf,"%s<%s,%s,KU%s,PT8637: VITC PAL  = ok,MasterSW=%s,%i\n",dato,tt,ClockKUStr,MasterSPGSW,AppType);	
   else 
    Fmt(wrtbuf,"%s<%s,%s,KU%s,PT8637: VITC NTSC = ok,MasterSW=%s,%i\n",dato,tt,ClockKUStr,MasterSPGSW,AppType);	
  bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
  CloseFile(fhandle);
  }
 
 Set5640Vits(VITSPASS);

 HidePanel(clockPNL);

 return ClockOk;
} // Clock_VITCTest





//----------------------------------------------------------------------------------
int Clock_LTCTest(int TVSystem){
int handle_returned, 
    bytes_written,
    fhandle;
char *strread,
     MasterSPGSW[15],
     TxtStr[80],
	 wrtbuf[100],
     *d,			// MM-DD-YYYY
	 *tt;			// HH:MM:SS

 DisplayPanel(clockPNL);
 ResetTextBox (clockPNL, CLK_PNL_TXTBOX, "");
 SetCtrlVal (clockPNL, CLK_PNL_BATTERYSTATUS, "");

 ClockOk = WriteProtect(OFF,FALSE);
 if (ClockOk == FALSE){
  HidePanel (clockPNL);
  return ClockOk;
 }

 Set5640Vits(VITSEXTDATA);

 if (TVSystem == PAL){
   Set5640Standard("G");
   SetPanelAttribute (clockPNL, ATTR_TITLE, " LTC test: (PAL) ");
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST PALB;");
   WriteCOMPort(PT5210COM,":INP:GENL:INP A; SYST PALB;");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Forbind 5640G VAR VIDEO til PT5210 GENL A IN\n");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Forbind PT5210 LTC OUT i rack til DUT XLR input\n");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Sæt VITC omskifter = G \n\n");
  }
  else{
   Set5640Standard("M");
   SetPanelAttribute (clockPNL, ATTR_TITLE, " LTC test: (NTSC) ");
   WriteCOMPort(DUTCOM,":INP:GENL:INP A_B; SYST NTSC;");
   WriteCOMPort(PT5210COM,":INP:GENL:INP A; SYST NTSC;");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Forbind 5640M VAR VIDEO til PT5210 GENL A IN\n");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Forbind PT5210 LTC OUT i rack til DUT XLR input\n");
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Sæt VITC omskifter = M \n\n");
  }
 
 Delay(0.05);
 WriteCOMPort(DUTCOM,":OUTP:TIM:REF LTC;");

 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Kontroller at tiden på DUT status display tæller op 12:3x:xx\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               REF = LTC input\n");
 if (TVSystem == PAL)
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               STATUS = LTC(PAL)\n\n");
  else 
   SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               STATUS = LTC(NTSC)\n\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Ved OK:   Tryk F4");


 ProcessDrawEvents();
 WaitForContinue();

 if (TVSystem == PAL)
  if (ClockOk == FALSE)
   MessagePopup(" PT8637:  LTC Time Code  (PAL)"," FEJL ");
 			 
 
 if (TVSystem == NTSC)
  if (ClockOk == TRUE)
    MessagePopup(" PT8637:  LTC Time Code  "," OK ");
   else
    MessagePopup(" PT8637:  LTC Time Code  (NTSC)"," FEJL ");
    
 
 if (ClockOk == TRUE){
  // get system date 
  d = DateStr();
  CopyString(dato,0,d,8,2);
  CopyString(dato,strlen(dato),d,0,2);
  CopyString(dato,strlen(dato),d,3,2);
 
  // get system time
  tt = TimeStr();
  
  strread = ReadClockKUNumber();
  CopyString(ClockKUStr,0,strread,0,-1);
  
  strread = ReadMasterSWVer();
  CopyString(MasterSPGSW,0,strread,0,-1);
  
  // check om logfil findes, ellers opret en ny logfil
  fhandle = OpenFile (ClockLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
  if (TVSystem == PAL)
    Fmt(wrtbuf,"%s<%s,%s,KU%s,PT8637: LTC PAL  = ok,MasterSW=%s,%i\n",dato,tt,ClockKUStr,MasterSPGSW,AppType);	
   else 
    Fmt(wrtbuf,"%s<%s,%s,KU%s,PT8637: LTC NTSC = ok,MasterSW=%s,%i\n",dato,tt,ClockKUStr,MasterSPGSW,AppType);	
  bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
  CloseFile(fhandle);
  }

 HidePanel(clockPNL);

 Set5640Vits(VITSPASS);

 return ClockOk;
} // Clock_LTCTest





//----------------------------------------------------------------------------------
int Clock_1HzTest(int mode){
// Test af 1 Hz og kontrol af dato i clock IC = M48T86
// mode = TRUE:   check dato i M48T86
// mode = FALSE:  check ikke dato i M48T86
int handle_returned, 
    bytes_written,
    fhandle;
char *strread,
     MasterSPGSW[15],
     TxtStr[80],
	 wrtbuf[100],
     *d,			// MM-DD-YYYY
	 *tt;			// HH:MM:SS

 DisplayPanel(clockPNL);
 ResetTextBox (clockPNL, CLK_PNL_TXTBOX, "");
 SetCtrlVal (clockPNL, CLK_PNL_BATTERYSTATUS, "");

 ClockOk = WriteProtect(OFF,FALSE);
 if (ClockOk == FALSE){
  HidePanel (clockPNL);
  return ClockOk;
 }

 Set5640Vits(VITSEXTDATA);
 
 SetPanelAttribute (clockPNL, ATTR_TITLE, " 1 Hz test  (input = 50 ohm) ");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Forbind 1 Hz OUT i rack til DUT XLR input\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Jumper PP1 monteres\n\n");
 
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Kontroller at tiden på PT5230 status display tæller op med 1 sek interval\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               REF = 1 Hz input\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               STATUS = PRESENT\n\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Hvis signal fjernes, skal display efter nogle sekunder vise\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               STATUS = NO SIGNAL\n\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Test med 1 Hz omskifter i både stilling 500ms og 100us\n\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
 SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Ved OK:   Tryk F4");

 Delay(0.05);
 WriteCOMPort(DUTCOM,":OUTP:TIM:REF REF1HZ;");

 WaitForContinue();

 if (ClockOk == TRUE){
  ResetTextBox (clockPNL, CLK_PNL_TXTBOX, "");
  SetPanelAttribute (clockPNL, ATTR_TITLE, " 1 Hz test  (input = 1 kohm) ");
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Jumper PP1 fjernes\n\n");
 
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Kontroller at tiden på PT5230 status display tæller op med 1 sek interval\n");
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               REF = 1 Hz input\n");
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, "               STATUS = PRESENT\n\n");
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Ved fejl: Tryk F9\n");
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Ved OK:   Tryk F4");

  WaitForContinue();
 }



// dato test
 if ((ClockOk == TRUE) && (mode == TRUE)){
  DisplayPanel(clockPNL);
  SetPanelAttribute (clockPNL, ATTR_TITLE, " Kontrol af dato i M48T86 (V7) ");
  ResetTextBox (clockPNL, CLK_PNL_TXTBOX, "\n\n");
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Sluk/tænd for apparatet og kontroller at dato er dags dato \n\n\n");
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " OK:    Tryk F4\n");
  SetCtrlVal(clockPNL,CLK_PNL_TXTBOX, " Fejl:  Tryk F9");
  WaitForContinue();
  HidePanel(clockPNL);
 } 
      

 
 
 if (ClockOk == TRUE)
   MessagePopup(" PT8637 test "," OK ");
  else
   MessagePopup(" PT8637 test "," FEJL ");
    
 if (ClockOk == TRUE){
  // get system date 
  d = DateStr();
  CopyString(dato,0,d,8,2);
  CopyString(dato,strlen(dato),d,0,2);
  CopyString(dato,strlen(dato),d,3,2);
 
  // get system time
  tt = TimeStr();
  
  strread = ReadClockKUNumber();
  CopyString(ClockKUStr,0,strread,0,-1);
  
  strread = ReadMasterSWVer();
  CopyString(MasterSPGSW,0,strread,0,-1);
  
  // check om logfil findes, ellers opret en ny logfil
  fhandle = OpenFile (ClockLogFile[LogDest], VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
  Fmt(wrtbuf,"%s<%s,%s,KU%s,PT8637: 1 Hz = ok,MasterSW=%s,%i\n",dato,tt,ClockKUStr,MasterSPGSW,AppType);	
  bytes_written = WriteFile (fhandle, wrtbuf, strlen(wrtbuf));
  CloseFile(fhandle);
  }

 HidePanel(clockPNL);


 return ClockOk;
} // Clock_1HzTest






//------------------------------------------------------------------------
int CVICALLBACK ClockOkCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			Cont = TRUE;

			break;
	}
	return 0;
}


int CVICALLBACK ClockExitCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			ClockOk = FALSE;
			Cont = TRUE;

			break;
	}
	return 0;
}
