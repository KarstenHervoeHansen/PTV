/* FILE NAME    cviutil.c
 * PURPOSE      Utility file for test af PT5201
 *             
 * SPEC         nov 1996, Peter Frederiksen
 * DESIGN
 * CODING    
 * CODE INSP.
 * TEST     
 *            
 * MODIFIED:    980910: GPS receiver check indført  
 *              991014  Check af om GPS receiver er ok kan styres med GPSCHECK
 *					    01.03.08: Connection to net @ DK-Audio establidhed.  Path changed
 *                        to: l:\produk\MEASDATA
 * NOTES:    
 *          
*/

#define GPSCHECK 1  // 1=GPS check on    
                    // 0=GPS check off, reports GPSreceiver as OK


#include "hp5313xa.h"
#include <userint.h>
#include <gpib.h>
#include <ansi_c.h>
#include <formatio.h>
#include <rs232.h>
#include <utility.h>
#include "vcautil.h"

#include "def.h"

// header files for uir files
#include "cfg.h"
#include "pt5201.h"


char GPSStatusStr[30];         // Return string from GPS reveiver.
int  GPSStatusOk,          // Status for GPS receiver (TRUE/FALSE).
     GPSCheckNo;           // Antal gange at GPS modtager er checket 
                                     // siden programstart.
    
const int MaxGPSMinuteDiff = 15;  // max tid i minutter for opdatering af 
                      //  status file for GPS-modtager
                      
#if 0 // New paths for DK-Audio net
char GPSStatusFile[50] = {"m:\\measdata\\5230\\gpsstat.dat"};

// logfiles: 0=logfile on local drive    1=logfile on LAN drive
char BBLogFile[2][50] = {"c:\\5201log\\bbcal.log","m:\\measdata\\5201\\bbcal.log"};
char AESLogFile[2][50] = {"c:\\520log\\aescal.log","m:\\measdata\\5201\\aescal.log"};
char SPGLogFile[2][50] = {"c:\\520log\\spgcal.log","m:\\measdata\\5201\\spgcal.log"};
char SDILogFile[2][50] = {"c:\\5201log\\sdical.log","m:\\measdata\\5201\\sdical.log"};
char SDIMeasLogFile[2][50] = {"c:\\5201log\\sdimeas.log","m:\\measdata\\5201\\sdimeas.log"};
#else
char GPSStatusFile[50] = {"l:\\produk\\MEASDATA\\5230\\gpsstat.dat"};

// logfiles: 0=logfile on local drive    1=logfile on LAN drive
char BBLogFile[2][50] = {"c:\\5201log\\bbcal.log","l:\\produk\\MEASDATA\\5201\\bbcal.log"};
char AESLogFile[2][50] = {"c:\\520log\\aescal.log","l:\\produk\\MEASDATA\\5201\\aescal.log"};
char SPGLogFile[2][50] = {"c:\\520log\\spgcal.log","l:\\produk\\MEASDATA\\5201\\spgcal.log"};
char SDILogFile[2][50] = {"c:\\5201log\\sdical.log","l:\\produk\\MEASDATA\\5201\\sdical.log"};
char SDIMeasLogFile[2][50] = {"c:\\5201log\\sdimeas.log","l:\\produk\\MEASDATA\\5201\\sdimeas.log"};
#endif

// Video Level Meter kalibrering  ------------------------------------------------
double VLMGain;
const double VLMGainMax = 4.05; // max-min limits for VLMGain read from file=VlmGainFileName
const double VLMGainMin = 3.95;

const char  *VlmGainFileName = {"VLMGAIN.DAT"};
int     vlmgainfile;      // file handle for VLMGain file


// Test setup
const char *TestSetupFileName = {"TSTSETUP.DAT"};
int      testsetupfile;     // file handle for Test Setup file


const int IOAddr = 0x220;     // PC I/O-address for PC-DIO24 interface

// COM-port assignments 
// COM2 = Mouse 
// COM8 = TrueTime GPS-receiver i det ene test rack 
const int   DUTCOM = 1;       // Device Under Test            COM Port#
const int   VCACOM = 5;       // R & S   VCA                  COM Port#
const int   PT5210COM = 6;      // PT5210 sync gen i test rack  COM Port#
const int   PM3094COM = 7;      // PM3094 scop i test rack      COM Port#
const char  *COMname[8] = {"com1","com2","","","com5","com6","com7","com8"};


const int PM5640GNomScH = 152;    // 152 = 0° ScH fase for 5640G KU021306
const int PM5640MNomScH = 303;    // 303 = 0° ScH fase for 5640M KU021307
                  // ScH data læses med ReadPM5640ScHCalData()
                  // Se menupunkt i Diverse

// PT5201 main data
int   BBNo;
char  KUStr[10];
char  PT5201RevStr[10];


int DACData[8][3];        // [BB][syncDAC,offsetDAC,gainDAC]
unsigned int PhaseData[8][4]; // [BB][GPhase,GScH,MPhase,MScH]
unsigned int SPGCalData[4];
int TPGDACData[3];            // [syncDAC,offsetDAC,gainDAC]
unsigned int TPGData[4];      // [GPhase,GScH,MPhase,MScH]


unsigned char PCDIOPortBStatus;   // Status af PM5662 indstilling

int   AppType;            // apparattype  (only PT5201)

int RS232Timeout;

int   IEEEboard;

int LogDest;        // flag for destination (0=local or 1=LAN) of logfiles

// boolean test variables
int SyncLevelOk,
    DCOffsetOk,
    BBGainOk,
    PhaseScHGainOk,
    VoltOk,
    Volt50Ok,
    Volt33Ok,
    VoltM50Ok,
    TcxoOk,
    AudioVCOOk,
    BBCalOk,
    TSGCalOk,
    SPGCalOk,
    AutoHPH_PALOk,
    AutoHPH_NTSCOk,
    GenlOk,
    GenlPALOk,
    GenlNTSCOk,
    Genl10MHzOk,
    SPGPhaseCalOk,
    SPGGenlPhasePALOk,
    SPGGenlPhaseNTSCOk,
    SPGResetOk,
    TPGPattOk,
    F13Ok,
    F27Ok;

    

// panel handles
int mainPNL,
    configPNL,    // kalibreringsdata for enheder
    instrPNL,   // instrumentoplysninger  
    spghphPNL,
    spggenlPNL,
    selgenlPNL,
    aesPNL,
    anPNL,
    anmeasPNL,
    vccPNL,
    ocxoPNL,
    cntPNL,
    totPNL,
    spgphasePNL,
    syncPNL,
    phasePNL;

    
// menu handles   
int   mainmenuhandle,
    totalmenuhandle;
    
int   devicePM5640G,
    devicePM5640M,
    ErrIEEE,
    Err232,
    hp34970,
    hp53132;


char  DUTStr[50],         // ASCII data written to DUT on COM1
    *dato,                  // YYMMDD  (see GetDate)
    *kustr,
    ASCIIdata[100],     // ASCII data read from a COM port
    YearStr[5],
    MonthStr[5],
    DayStr[5],
    *strread,
    wrtbuf[100],
    MsgStr[100],   // diverse udskrifter
    *dd,      // MM-DD-YYYY
    *tt;      // HH:MM:SS
    
unsigned char BINdata[2000];  // Binary data read from a COM port

int fhandle, 
   bytes_written;
 

// program flow control variables
int done,
    Cont,
    Repeat;





//-------------------------------------------------------------------------------
/*
Læser VLM gain kalibreringsdata fra filen 'VlmGainFileName'
Værdien kontrolleres for min/max grænser.
Ved fejl returneres FALSE, eller TRUE.
Værdien er individuel for hver testplads.
*/
int ReadVLMGain(void){
int  bytes_read,
     ScanErr,
     IOErr,
     p;
char MsgStr[200],
     line[100];
     
 vlmgainfile = OpenFile (VlmGainFileName, VAL_READ_ONLY, VAL_OPEN_AS_IS,VAL_ASCII);
  bytes_read = ReadLine (vlmgainfile, line, -1);// læs f.eks: '3.998'
  IOErr = GetFmtIOError ();
  if ((IOErr != 0) || (bytes_read < 4)) {   
   Fmt(MsgStr," Ved læsning af Video Level Meter kalibreringsdata fra fil '%s'\n Kalibreing og test ikke mulig  -  Ret fejlen",VlmGainFileName);
   MessagePopup(" Fejl",MsgStr);
   return FALSE;
  }

 IOErr = CloseFile (vlmgainfile);
  
 p = FindPattern (line, 0, -1, "//", 1, 0);
 if (p > 6)
   CopyString(line,0,line,0,p-1);  // slet fra // til linieslut
  else
   CopyString(line,0,line,0,strlen(line)); 
 
 ScanErr = Scan(line,"%s>%f",&VLMGain);
  if (ScanErr != 1) {   
   Fmt(MsgStr," Ved læsning af Video Level Meter kalibreringsdata fra fil '%s'\n Kalibreing og test ikke mulig\n Scan error: %i",VlmGainFileName, ScanErr);
   MessagePopup(" Fejl",MsgStr);
   return FALSE;
  }
 
 if ((VLMGain > VLMGainMax) || (VLMGain < VLMGainMin)){
  Fmt(MsgStr," Video Level Meter kalibreringsdata\n Gain: %f[p3]\n Gyldigt område: %f[p3] - %f[p3]",VLMGain,VLMGainMin,VLMGainMax);
  MessagePopup(" Fejl",MsgStr);
  return FALSE;
 }
 
return TRUE;
} // ReadVLMGain(void)




//-------------------------------------------------------------------------------
int ReadTestSetup(void){
// Læser diverse test setup fra filen 'TestSetupFileName'
// Ved fejl returneres FALSE, eller TRUE.
int  bytes_read,
     IOErr,DataErr,
     p;
char MsgStr[200],
     line[50];
     
 DisableBreakOnLibraryErrors();
 testsetupfile = OpenFile (TestSetupFileName, VAL_READ_ONLY, VAL_OPEN_AS_IS,VAL_ASCII);
 EnableBreakOnLibraryErrors();
 
 if (testsetupfile == -1){   
  Fmt(MsgStr," Fil '%s' mangler\n Kalibreing og test ikke mulig  -  Ret fejlen",TestSetupFileName);
  MessagePopup(" Fil ikke fundet ",MsgStr);
  return FALSE;
 }

 bytes_read = ReadLine (testsetupfile, line, -1); // read 1. line
 IOErr = GetFmtIOError ();

// 1. linie: PT5201 samlet apparat revision (se dokumentoversigt for PT5201)
 DataErr = FALSE;
 p = FindPattern (line, 0, -1, "PT5201REV=", 1, 0);
 if (p == 0)
   CopyString(PT5201RevStr,0,line,10,strlen(line) - 10); // PT5201REV=xxx
  else{
   Fmt(MsgStr," Fil: %s   1. linie skal være PT5201REV=xxx\n Kalibreing og test ikke mulig  -  Ret fejlen",TestSetupFileName);
   MessagePopup(" Fejl ved læsning af PT5201 revision",MsgStr);
   Fmt(PT5201RevStr,"???"); 
   DataErr = TRUE;
   }
 
 IOErr = CloseFile (testsetupfile);
 
 if ((IOErr == -1) || (DataErr == TRUE))
   return FALSE;
  else
   return TRUE;
} // ReadTestSetup(void)





//-----------------------------------------------------------------------------
char date[10];  // must not be local
char *GetDate(int dateformat){
//Read labwindows date format MM-DD-YYYY
//datetype = 1: return date in format YYMMDD
//datetype = 2: return date in format YY,MM,DD and
//           updates global variables YearStr, MonthStr, DayStr
char *labw_date,yy[3],mm[3],dd[3];

 labw_date = DateStr();   // MM-DD-YYYY
 CopyString(yy,0,labw_date,8,2); // YY
 CopyString(mm,0,labw_date,0,2); // MM
 CopyString(dd,0,labw_date,3,2); // DD

 if (dateformat == 1)
  Fmt(date,"%s<%s%s%s",yy,mm,dd);
 else if (dateformat == 2){
  Fmt(date,"%s<%s,%s,%s",yy,mm,dd);
  Fmt(YearStr,"%s<%s",yy);
  Fmt(MonthStr,"%s<%s",mm);
  Fmt(DayStr,"%s<%s",dd);
 }
 else
  Fmt(date,"%s<0,0,0");

return (&date[0]);
}



//------------------------------------------------------------------------------
int CheckKUNo(char *kustr){
int n, validku;
 
 validku = TRUE;
 
 if (strlen(kustr) != 6)        // 6 digits
  return FALSE;
 
 for (n = 0; n < 6; n++)
  if (isdigit(kustr[n]) == FALSE) // digits only
   return FALSE;

return validku;
} // CheckKUNo




//-------------------------------------------------------------------------------
void EnterKUNo(char text[50], char *kuread, char *kustr){
char prompttext[100], response[10];
int validku;

 Fmt(prompttext,"%s<%s:   Indtast KU nummer ",text);

 do {
  PromptPopup (prompttext, kuread, response, 6);

  if ((strlen(response) == 0) && (strlen(kuread) == 6))  // Only ENTER pressed
   CopyString(response,0,kuread,0,-1);
   
  validku = CheckKUNo(response);
 
 } while (validku == FALSE);


 CopyString(kustr,0,response,0,-1);
} // EnterKUNo




//-------------------------------------------------------------------------------
void VLMdata(unsigned short xdata, unsigned short sel){
// IOPort A:   bit 0: SCL    bit 1: SDA    bit 2..4: SEL 0..2   
unsigned char n,p;

  xdata = xdata << 6;
  p = 0xe0 + (sel * 4); // control data bit 2,3,4  (bit 5,6,7=1)
  for (n = 0; n < 10; ++n)
   {
    if ((xdata & 0x8000) == 0)
      p = p & 0xfd;   // set SDA
    if ((xdata & 0x8000) == 0x8000)
      p = p | 0x02;     
    outp(IOAddr,p); ++p;
    outp(IOAddr,p); --p;
    outp(IOAddr,p);
    
    xdata = xdata << 1;
    
  } // for
} // vlmdata



//------------------------------------------------------------------------------
void SetVLM (unsigned short StartLine, 
       unsigned short StopLine, 
       unsigned short StartPos, 
       unsigned short StopPos)
{
  VLMdata(StartLine,0);
  VLMdata(StopLine,1);
  VLMdata(StartPos,2);
  VLMdata(StopPos,3);
} // SetVLM





//------------------------------------------------------------------------------
void SetPM5662(int TVSystem){
// PM5662 remote styres via portB i PC-DIO24 interface
/* TVSystem = G or M          Mode = VECT | WFM
   IOPort B:   bit 0: 5662G   pin  7(!WFM)     pin 5(!A_B)    !=active low
               bit 1: 5662G   pin 14(B-in)
               bit 2: 5662M   pin 7(VECT)    pin 5(A_B)
               bit 3: 5662M   pin 14(!B-in)
               hardwired:    pin  4=low  (!2H)
                         pin 17=low  (!XMAG)
                         pin 13=low  (remote=on)
                         pin  1=GND
                         
  bit#    3 2 1 0
 -------------------- 
  G WFM   x x 1 0
  G VECT    x x 0 1
  M WFM   1 0 x x
  M VECT    0 1 x x
*/
 
 if (TVSystem == PAL){
   PCDIOPortBStatus |= 0xF9;     // bit 0,3 = 1 (G = VETC)
   PCDIOPortBStatus &= 0xF9;     // bit 1,2 = 0 (M = WFM)
 }
 else if (TVSystem == NTSC){
   PCDIOPortBStatus |= 0xF6;     // bit 1,2 = 1 (M = VETC)
   PCDIOPortBStatus &= 0xF6;     // bit 0,3 = 0 (G = WFM)
 }

  outp(IOAddr+1,PCDIOPortBStatus);
  Delay(0.01);

}






// vælger genlock signal på BNC-stik: GENL SIGNAL 
// relæ'er på genlock mux (7108 100 74491) styres via portC i PC-DIO24 interface
void SetGenlockSignal(int Signal){
 outp(IOAddr+2,0xFF);
 Delay(0.02);
 switch (Signal){
  case GENL_PAL:       outp(IOAddr+2,0xFE); break;
  case GENL_NTSC:    outp(IOAddr+2,0xFC); break;
  case GENL_443_HIGH: outp(IOAddr+2,0xFD); break;
  case GENL_443_LOW:  outp(IOAddr+2,0xF5); break;
  case GENL_358_HIGH: outp(IOAddr+2,0xFB); break;
  case GENL_358_LOW:  outp(IOAddr+2,0xF3); break;
  case GENL_10M_HIGH: outp(IOAddr+2,0xFA); break;
  case GENL_10M_LOW:  outp(IOAddr+2,0xF2); break;
  case GENL_OFF:     outp(IOAddr+2,0xFF); break;
 }

}










//-------------------------------------------------------------------------------
char *ReadlnCOMPort(int PortNo, double Timeout){
int bytes_read;
 SetComTime (PortNo, Timeout);   // set timeout
 ASCIIdata[0] = 0x0;
 bytes_read = ComRdTerm (PortNo, ASCIIdata, 100, LF);   //terminates on LF
 if (bytes_read > 0) {
  if(ASCIIdata[bytes_read - 1] == CR)
    ASCIIdata[bytes_read - 1] = 0x00;
  ASCIIdata[bytes_read] = 0x00;
  RS232Timeout = FALSE;
 }  
  else{
   ASCIIdata[0] = 0x30;
   ASCIIdata[1] = 0x00;    // '0' + NULL
   RS232Timeout = TRUE;
  }
 
 return (&ASCIIdata[0]);
}



//-------------------------------------------------------------------------------
unsigned char *ReadBinCOMPort(int PortNo, double Timeout, int NoOfBytes){
int bytes_read;
int InQL;
// SetComTime (PortNo, Timeout);   // set timeout
 BINdata[0] = 0x0;
 bytes_read = ComRd (PortNo, BINdata, NoOfBytes);   //terminates on NoOfBytes
 BINdata[bytes_read] = 0x0;
 return (&BINdata[0]);
}



//-------------------------------------------------------------------------------
void WriteCOMPort(int PortNo, char TransmitStr[]){
int bytes_send;
int NoOfBytes;

 NoOfBytes = StringLength (TransmitStr);
 FlushOutQ(PortNo);
 bytes_send = ComWrt (PortNo, TransmitStr, NoOfBytes);
 
 while 
  (GetOutQLen(PortNo)> 0);
}




//-------------------------------------------------------------------------------
void WaitForContinue(void){
int handle_returned, controlID_returned;
 Cont = FALSE;
 while (Cont == FALSE)
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
}






//-------------------------------------------------------------------------------
int Set5640Pattern(char GM[2], int Patt){
char SendStr[30];
int wrtstatus;
#if IEEEBUS 

 GM[0] = toupper((char)GM[0]);

 if ((Patt < 0) | (Patt > 600))
  return -1;
 
 Fmt(SendStr,"%s<PATTERN %i",Patt);
 if (GM[0] == 'G')
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 if (GM[0] == 'M')
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.01);  
#endif
return wrtstatus; 
}



//-------------------------------------------------------------------------------
int Set5640SyncLevel(char GM[2], int Level){
char SendStr[30];
int wrtstatus;
#if IEEEBUS 

 GM[0] = toupper((char)GM[0]);
 
 if (GM[0] == 'G')
  if ((Level < 0) | (Level > 600))
   return -1;

 if (GM[0] == 'M')
  if ((Level < 0) | (Level > 80))
   return -1;
 
 Fmt(SendStr,"%s<SYNCLEVEL %i",Level);
 if (GM[0] == 'G')
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 if (GM[0] == 'M')
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.01);  
#endif
return wrtstatus; 
}




//-------------------------------------------------------------------------------
int Set5640BurstLevel(char GM[2], int Level){
char SendStr[30];
int wrtstatus;
#if IEEEBUS 
 
 GM[0] = toupper((char)GM[0]);

 if (GM[0] == 'G')
  if ((Level < 0) | (Level > 600))
   return -1;

 if (GM[0] == 'M')
  if ((Level < 0) | (Level > 80))
   return -1;
 
 Fmt(SendStr,"%s<BURSTLEVEL %i",Level);
 if (GM[0] == 'G')
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 if (GM[0] == 'M')
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.01);  
#endif
return wrtstatus; 
}
  
  
  
//-------------------------------------------------------------------------------
int Set5640Hum(char GM[2], int Hum){
char SendStr[30];
int wrtstatus;
#if IEEEBUS 
 
 GM[0] = toupper((char)GM[0]);

 if (GM[0] == 'G')
  if ((Hum < 0) | (Hum > 1500))
   return -1;

 if (GM[0] == 'M')
  if ((Hum < 0) | (Hum > 200))
   return -1;
 
 Fmt(SendStr,"%s<HUMLEVEL %i",Hum);
 if (GM[0] == 'G')
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 if (GM[0] == 'M')
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.01);  
#endif
return wrtstatus; 
}
  



//-------------------------------------------------------------------------------
int Set5640Noise(char GM[2], int Noise){
char SendStr[30];
int wrtstatus;
#if IEEEBUS 
 
 GM[0] = toupper((char)GM[0]);
 
 if ((Noise < -60) | (Noise > -10))
  return -1;

 Fmt(SendStr,"%s<NOISELEVEL %i",Noise);
 if (GM[0] == 'G')
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 if (GM[0] == 'M')
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.01);  
#endif
return wrtstatus; 
}
  
  
  

//-------------------------------------------------------------------------------
int Set5640Standard(char GM[2]){
int wrtstatus, rdstatus, sch_read, sch_val;
char read_buf[30], wrt_buf[30];
#if IEEEBUS 
 
 GM[0] = toupper((char)GM[0]);
 
 if (GM[0] == 'G'){
  wrtstatus = ibwrt (devicePM5640G, "STANDARD", 8);
  Delay(0.05);  
  wrtstatus = ibwrt (devicePM5640G, "SCHPHASE ?", 10);
  Delay(0.05);  
  rdstatus  = ibrd (devicePM5640G, read_buf, 10);
  Delay(0.05);  
  Scan(read_buf,"%s>%i",&sch_read);

  /*
  Fejlmeddelelse fjernet, da fejlen optræder periodisk og apparatet
  tilsyneladende virker alligevel ??
  if ((sch_read > 1023) || (sch_read < 0)){
   MessagePopup (" PM5640G Sch fase","ADVARSEL: Fejl ved indstilling af PM5640G\n Program stoppes");
   return -1;
  }
  */

  if (sch_read > PM5640GNomScH)
    sch_val = sch_read - PM5640GNomScH; 
   else
    sch_val = PM5640GNomScH - sch_read;   
  Fmt(wrt_buf,"%s<SCHPHASE ");
  Fmt(wrt_buf,"%s[a]<%i",sch_val);
  if (sch_read > PM5640GNomScH)
    Fmt(wrt_buf,"%s[a]< DOWN");
   else
    Fmt(wrt_buf,"%s[a]< UP");
  wrtstatus = ibwrt (devicePM5640G,wrt_buf, StringLength(wrt_buf));
  Delay(0.05);  
 } 

 if (GM[0] == 'M'){
  wrtstatus = ibwrt (devicePM5640M, "STANDARD", 8);
  Delay(0.05);  
  wrtstatus = ibwrt (devicePM5640M, "SCHPHASE ?", 10);
  Delay(0.05);  
  rdstatus  = ibrd (devicePM5640M, read_buf, 10);
  Delay(0.05);  
  Scan(read_buf,"%s>%i",&sch_read);
  
  /*
  Fejlmeddelelse fjernet, da fejlen optræder periodisk og apparatet
  tilsyneladende virker alligevel ??
  if ((sch_read > 1023) || (sch_read < 0)){
   MessagePopup (" PM5640M Sch fase","ADVARSEL: Fejl ved indstilling af PM5640M\n Program stoppes");
   return -1;
  }
  */
  
  if (sch_read > PM5640MNomScH)
    sch_val = sch_read - PM5640MNomScH; 
   else
    sch_val = PM5640MNomScH - sch_read;   
  Fmt(wrt_buf,"%s<SCHPHASE ");
  Fmt(wrt_buf,"%s[a]<%i",sch_val);
  if (sch_read > PM5640MNomScH)
    Fmt(wrt_buf,"%s[a]< DOWN");
   else
    Fmt(wrt_buf,"%s[a]< UP");
  wrtstatus = ibwrt (devicePM5640M,wrt_buf, StringLength(wrt_buf));
 } 
 
 Delay(0.05);  
#endif
return 0; 
}
  


//-------------------------------------------------------------------------------
int ReadPM5640ScHCalData(void){
/*
Udlæser kalibreringsdata for ScH-fasen i PM5640. For at kunne programmere en absolut ScH fase
i PM5640 skal de aktuelle kalibreringsdata kendes. Når disse data er kendt, kan man beregne
hvor meget fasen skal ændres for at indstille en bestemt fase.
Hvis PM5640 er blevet kalibreret, skal de nye kalibreringsdata være kendt for at kunne
indstille en absolut ScH fase.
Konstanterne PM5640GNomScH og PM5640MNomScH rettes efter kalibrering af PM5640.
*/
int wrtstatus, 
  rdstatus, 
  schG_read,
  schM_read, 
  datavalid;
char read_buf[30], MsgStr[100];
#if IEEEBUS 
  if (ConfirmPopup (" Læs PM5640 Sch Data ", " Indstil ScH fase til 0° på både PM5640G og PM5640M") == NO)
   return 0;
   
  wrtstatus = ibwrt (devicePM5640G, "SCHPHASE ?", 10);
  rdstatus  = ibrd (devicePM5640G, read_buf, 10);
  Scan(read_buf,"%s>%i",&schG_read);

  Delay(0.02);  
  wrtstatus = ibwrt (devicePM5640M, "SCHPHASE ?", 10);
  rdstatus  = ibrd (devicePM5640M, read_buf, 10);
  Scan(read_buf,"%s>%i",&schM_read);
  
  datavalid = FALSE;
  if (((schG_read <= 1023) && (schG_read >= 0)) && ((schM_read <= 1023) && (schM_read >= 0)))
   datavalid = TRUE;

  if (datavalid == FALSE)
    Fmt(MsgStr,"%s< ADVARSEL: Fejl ved læsning\n PM5640G: ");
   else
    Fmt(MsgStr,"%s< PM5640G: ");
  
  Fmt(MsgStr,"%s[a]<%i",schG_read);
  Fmt(MsgStr,"%s[a]<\n PM5640M: ");
  Fmt(MsgStr,"%s[a]<%i",schM_read);
  
  if (datavalid == FALSE)
   Fmt(MsgStr,"%s[a]<\n Gyldigt område: 0-1023");
  
  MessagePopup (" PM5640 Sch Data ", MsgStr);

#endif
return 0;
}




//-------------------------------------------------------------------------------
int Set5640Vits(int VitsMode){
char SendStr[30];
int wrtstatus;
#if IEEEBUS 
 if (VitsMode == VITSEXTDATA){
  Fmt(SendStr,"%s<VITSLINE 10,DATA");
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 12,DATA");
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 323,DATA");
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 325,DATA");
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
  Delay(0.03);  
  
  Fmt(SendStr,"%s<VITSLINE 1,10,DATA");
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 1,12,DATA");
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 2,10,DATA");
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 2,12,DATA");
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
  Delay(0.03);  
 } 
 else{
  Fmt(SendStr,"%s<VITSLINE 10,PASS");
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 12,PASS");
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 323,PASS");
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 325,PASS");
  wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
  Delay(0.03);  
  
  Fmt(SendStr,"%s<VITSLINE 1,10,PASS");
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 1,12,PASS");
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 2,10,PASS");
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
  Delay(0.03);  
  Fmt(SendStr,"%s<VITSLINE 2,12,PASS");
  wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
  Delay(0.03);  
 } 
#endif
return wrtstatus; 
}






//-------------------------------------------------------------------------------
void SelectPM3094Setup(int SetupNo){
// Vælger et forud indstillet setup i oscilliscopet PM3094
// Der kan vælges 10 forskellige setup (FRONTS = F1-F10)
// return:  intet
char DataStr[50];
#if AT232
 Fmt(DataStr,"%s<RS %i\r",SetupNo);  
 WriteCOMPort(PM3094COM,DataStr);
 Delay(0.03);
 WriteCOMPort(PM3094COM,"GL\r");
#endif
}



//-------------------------------------------------------------------------------
int WriteProtect(int OnOff, int flag){
/* Indstiller password i apparatet til enten ON eller OFF
OnOff:  ON  = password sættes til ON
      OFF = password sættes til OFF
flag:   TRUE  = Meddelelse gives til bruger om tilstand af password
flag:   FALSE = Ingen meddelelse gives til bruger om tilstand af password
   
Hvis der ikke er svar på RS-232 kommunikationen gives der altid meddelelse 
til brugeren

return:  TRUE hvis password er som ønsket og der er kontakt via RS-232, ellers FALSE
*/

int OnOffOk;

OnOffOk = TRUE;

 if (OnOff == ON){
  WriteCOMPort(DUTCOM,":FACT:PASS ON;");
  Delay(0.05);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:PASS?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strcmp (strread, "ON") != 0) {
   MessagePopup(" RS-232 remote"," FEJL:  Check RS-232 kabel, og at apparatet er tændt");
   OnOffOk = FALSE;
  } 
  else
  if (flag == TRUE)
   MessagePopup(" Skrivebeskyttelse"," ON");
 } // if (OnOff == ON)
 
 else{
  WriteCOMPort(DUTCOM,":FACT:PASS 'PASS5201';");
  Delay(0.05);
 
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:PASS?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strcmp (strread, "OFF") != 0){   // master SW med password funktion
   MessagePopup(" DUT RS-232 remote"," FEJL:  Check RS-232 kabel, og at apparatet er tændt");
   OnOffOk = FALSE;
  }
  else
  if (flag == TRUE)
   MessagePopup(" Skrivebeskyttelse"," OFF");
 } 

 return OnOffOk;
}










//-------------------------------------------------------------------------------
int CheckReadyVCA(void){
// Kontrollerer at VCA'en er tændt og kan kommunikere med PC'en.
// return:  TRUE hvis der er kontakt til VCA'en, ellers FALSE.
char IDStr[40];
int  ConfirmReply,
   IDOk;

 vca_reset();
 vca_readid();
 CopyString (IDStr, 0, VCA_ID, 0, 17);
 IDOk = TRUE;
 if (CompareStrings (IDStr, 0, "ROHDE&SCHWARZ,VCA", 0, 0) != 0)
  IDOk = FALSE;

 if (IDOk == FALSE) {
  do {
  ConfirmReply = ConfirmPopup (" R&S VCA ", " RS-232  (COM5)\n Ingen forbindelse mellem PC og VCA");
    if (ConfirmReply == NO)
     IDOk = FALSE;

  vca_reset();
  vca_readid();
  CopyString (IDStr, 0, VCA_ID, 0, 17);

  } while ((ConfirmReply == YES) & (CompareStrings (IDStr, 0, "ROHDE&SCHWARZ,VCA", 0, 0) != 0));

 } //if

 return IDOk;
}





//-------------------------------------------------------------------------------
int ShowInstrumentSetup(void){
char MsgStr[80];
 DisplayPanel(instrPNL);
 ResetTextBox(instrPNL, INSTR_PNL_TXTBOX,"\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," PM5640G *)   IEEE adresse = 16\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," PM5640M *)   IEEE adresse = 17\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," HP34970A     IEEE adresse = 09\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," HP53132A     IEEE adresse = 03\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," DUT          COM1   9600,8,N,2\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," Mus          COM2\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," R&S VCA      COM5   19200,8,N,2\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," PT5210       COM6   9600,8,N,2\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," PM3094       COM7   9600,8,N,2\n");
 Fmt(MsgStr," Video Level Meter Gain: %f[p3]\n",VLMGain);
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX,MsgStr);
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX,"\n\n");

 
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX," *) Ved kalibrering af PM5640 skal der lægges nye kalibreringsværdier for ScH-fasen \n"); 
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX,"    ind i testprogrammet. Ellers vil ScH-fasen ikke indstilles til 0° ved opstart.\n");
 SetCtrlVal(instrPNL, INSTR_PNL_TXTBOX,"    Kalibreringsværdierne for ScH-fasen kan læses i menupunktet 'Diverse'\n");
 // se const PM5640GNomScH   PM5640MNomScH
 WaitForContinue();
 HidePanel(instrPNL);
return TRUE;
}



//------------------------------------------------------------------------------------------
int CheckXLDCRef(void){
/* 
Check status i TrueTime-XLDC GPS modtager
På den ene test-PC i FMS kører et program (XLDC.EXE) i baggrund, som overvåger GPS-modtageren.
Programmet skriver status af GPS-modtageren til en fil = GPSStatusFile.
Funktionen CheckXLDCRef læser statusfilen med et interval, som bestemmes af timeren MAIN_PNL_GPS_TIMER
i filen PT5230.uir. Timeren initialiseres i filen PT5230.c.
Se desuden funktionen GPSXLDCCallBack i denne fil.

Status af GPS-modtageren vises på mainPNL (se MAIN_PNL_GPSRECEIVER).
Ved start af kalibrering af 27MHz ref med HP53132 counter, læses status af GPS-modtageren.
Hvis status ikke er ok, kan der ikke kalibreres. HP53132 kan stadig bruges til mindre
krævende målinger, som f.eks. måling af audio frekvens.

*/
int fhandle,
    fileerr,
    bytes_read,
    minutesread,
    hourread,
    totalminutesread,
    minutesnow,
    hournow,
    totalminutesnow,
    timeok,
    dateok,
    xldcok;
char dateread[20],
     timeread[20],
     gpsstatusread[50],
     *dd,
     *tt,
     datestr[15],
     timestr[2][15],
     MsgTxt[50];

#if GPSCHECK
 // read date + time from system
 tt = TimeStr();            // get system time
 CopyString(timestr[0],0,tt,0,2); // get hour string
 CopyString(timestr[1],0,tt,3,2); // get minutes string
 Scan(timestr[0],"%s>%i",&hournow);
 Scan(timestr[1],"%s>%i",&minutesnow);
 totalminutesnow = hournow * 60 + minutesnow;

 dd = DateStr();        // get system date
 CopyString(datestr,0,dd,0,-1); // get date string

 DisableBreakOnLibraryErrors();
 // read date + time + gps status from file 'GPSStatusFile'
 fhandle = OpenFile (GPSStatusFile, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
 EnableBreakOnLibraryErrors();
 
 if (fhandle == -1){   //hvis fejl, prøv en gang til lidt senere
  Delay(2.0);
  fhandle = OpenFile (GPSStatusFile, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_ASCII);
 }
 
 bytes_read = ReadLine (fhandle, dateread, -1);
 bytes_read = ReadLine (fhandle, timeread, -1);
 bytes_read = ReadLine (fhandle, gpsstatusread, -1);
 fileerr = CloseFile (fhandle);

 CopyString(timestr[0],0,timeread,0,2); // get hour string
 CopyString(timestr[1],0,timeread,3,2); // get minutes string
 Scan(timestr[0],"%s>%i",&hourread);
 Scan(timestr[1],"%s>%i",&minutesread);
 totalminutesread = hourread * 60 + minutesread;
 
 // compare time, date and gps status
 timeok = (abs (totalminutesnow - totalminutesread) < MaxGPSMinuteDiff);
 dateok = (strcmp (datestr, dateread) == 0);
 xldcok = (strcmp (gpsstatusread, "XLDC: ANT 1 PLL 1 GPS 1") == 0);
 
 strcpy(GPSStatusStr,gpsstatusread);       // copy read gps status to global variable


 GPSStatusOk = timeok && dateok && xldcok;     // GPSStatusOk is a global variable
#else 
  SetCtrlAttribute (mainPNL, MAIN_PNL_GPSCHECKNUMBER, ATTR_VISIBLE,FALSE);
  GPSStatusOk = TRUE;
#endif   
 
 GPSCheckNo++;
 
 if (GPSStatusOk == TRUE){
   Fmt(MsgTxt,"%s<%i",GPSCheckNo);
#if GPSCHECK   
   SetCtrlAttribute (mainPNL, MAIN_PNL_GPSRECEIVER, ATTR_WIDTH, 50);
   SetCtrlAttribute (mainPNL, MAIN_PNL_DEC1, ATTR_WIDTH, 150);
   SetCtrlVal (mainPNL, MAIN_PNL_GPSRECEIVER," OK");
   SetCtrlVal (mainPNL, MAIN_PNL_GPSCHECKNUMBER, MsgTxt);
   SetCtrlAttribute (mainPNL, MAIN_PNL_GPSRECEIVER, ATTR_TEXT_COLOR, VAL_BLACK);
#else   
   SetCtrlAttribute (mainPNL, MAIN_PNL_GPSRECEIVER, ATTR_WIDTH, 150);
   SetCtrlAttribute (mainPNL, MAIN_PNL_DEC1, ATTR_WIDTH, 200);
   SetCtrlVal (mainPNL, MAIN_PNL_GPSRECEIVER," Check grøn LED blinker");
   SetCtrlAttribute (mainPNL, MAIN_PNL_GPSRECEIVER, ATTR_TEXT_COLOR, VAL_GREEN);
#endif
  }
  else
   {
   if (xldcok == FALSE)
      Fmt(MsgTxt,"%s< FEJL:   %s",GPSStatusStr);    
     else if (dateok == FALSE)
      Fmt(MsgTxt,"%s< FEJL: Date out");
     else if (timeok == FALSE)
      Fmt(MsgTxt,"%s< FEJL: Time out");

   SetCtrlAttribute (mainPNL, MAIN_PNL_GPSRECEIVER, ATTR_WIDTH, 220);
   SetCtrlAttribute (mainPNL, MAIN_PNL_DEC1, ATTR_WIDTH, 250);
   SetCtrlAttribute (mainPNL, MAIN_PNL_GPSRECEIVER, ATTR_TEXT_COLOR, VAL_RED);
   SetCtrlVal (mainPNL, MAIN_PNL_GPSRECEIVER, MsgTxt);
 }
  
return GPSStatusOk;
}





//------------------------------------------------------------------------------------------------------
int CheckHP53132ExtRef(void){
// select and checks for external 10 MHz ref at HP53132 counter
double ExtRefFreq;
char   rbuf[50];
int    rd,
       HP53132ExtRefOk;

  HP53132ExtRefOk = TRUE;
#if IEEEBUS
 DevClear (0, 3);
 Delay(0.5);
 ErrIEEE = hp5313xa_writeInstrData (hp53132, "*RST");
 Delay(0.5);
 ErrIEEE = hp5313xa_writeInstrData (hp53132, ":SENS:ROSC:EXT:CHECK ON");
 ErrIEEE = hp5313xa_writeInstrData (hp53132, ":SENS:ROSC:SOUR EXT");
 ErrIEEE = hp5313xa_writeInstrData (hp53132, ":SENS:ROSC:SOUR:AUTO OFF");

 ErrIEEE = hp5313xa_writeInstrData (hp53132, ":SENS:ROSC:EXT:FREQ?");
 ErrIEEE = hp5313xa_readInstrData (hp53132, 40, rbuf, &rd);
 rbuf[rd-1] = 0x0;
 Scan(rbuf,"%s>%f",&ExtRefFreq);
 if (ExtRefFreq > 1.00001e7){
  SetCtrlVal (mainPNL, MAIN_PNL_HP53132EXTREF, " MANGLER");
  SetCtrlAttribute (mainPNL, MAIN_PNL_HP53132EXTREF, ATTR_TEXT_COLOR,VAL_RED);
  MessagePopup(" Extern reference til HP53132 "," 10 MHz ext. reference fra GPS-modtager mangler\n HP53132 må IKKE anvendes til 27 MHz kalibrering !!\n Kontakt evt. instrumentservice");
  HP53132ExtRefOk = FALSE;
  }
 else {
  SetCtrlVal (mainPNL, MAIN_PNL_HP53132EXTREF, " 10 MHz");
  SetCtrlAttribute (mainPNL, MAIN_PNL_HP53132EXTREF, ATTR_TEXT_COLOR,VAL_BLACK);
  HP53132ExtRefOk = TRUE;
 }
#endif 
return HP53132ExtRefOk; 
} //  CheckHP53132ExtRef(void)





//-------------------------------------------------------------------------------
int CVICALLBACK GPSXLDCCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
{
char *tt,
     timestr[5];
int  minutes;

  switch (event) {
    case EVENT_TIMER_TICK:
       tt = TimeStr();        // get system time
             CopyString(timestr,0,tt,3,2);  // get minutes string
             Scan(timestr,"%s>%i",&minutes);
             if ((minutes % 2) == 1)    // odd minutes
              CheckXLDCRef();

      break;
  }
  return 0;
}






//-------------------------------------------------------------------------------
int CVICALLBACK InstrCallBack (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2) {
  switch (event) {
    case EVENT_COMMIT: Cont = TRUE; break;
    
  }
  return 0;
}

