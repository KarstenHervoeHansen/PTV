#include "hp34401a.h"
#include <userint.h>
#include <gpib.h>
#include <ansi_c.h>
#include <formatio.h>
#include <rs232.h>
#include <utility.h>
#include "vcautil.h"

#define IEEEBUS 1
#define AT232 1

#define NO 0
#define YES 1
#define TRUE 1
#define FALSE 0
#define LF 10
#define CR 13
#define WAIT 1
#define NOWAIT 0
#define ON 1
#define OFF 0
#define PAL 0
#define NTSC 1

#define REARTERM34401 0
#define FRONTTERM34401 1

#define MAIN 		0
#define AUDIOGEN 	1
#define SDIGENLOCK 	2
#define DUALBB 		3
#define SDIBLACK 	4
#define SDITSG 		5
#define BBBUFFER 	6
#define SPG 		7
#define ANALOGTSG 	8
#define TIMECODE 	9

#define GENL_PAL		0
#define GENL_NTSC		1
#define GENL_443_HIGH	2
#define GENL_443_LOW	3
#define GENL_358_HIGH	4
#define GENL_358_LOW	5
#define GENL_10M_HIGH	6
#define GENL_10M_LOW	7
#define GENL_OFF		10


const char *NC12[10] = {"109 82900",  // main
						"109 82910",  // audio
						"109 82920",  // d1 genlock
						"109 82930",  // dual analog BB
						"109 82940",  // d1 black
						"109 82950",  // d1 test signal
						"109 82960",  // analog BB buffer
						"109 82970",  // spg
						"109 82980",  // analog test signal
						"109 82990"}; // time code


const char *V24Addr[9] = {""," 0;"," 0;"," 17;"," 17;"," 19;"," 19;"," 18;"," 18;"};

const double VLMGain = 4.008;
const double NomSyncLevel = -300;	// mV
const double SyncLevelTol = 2;		// mV
const double NomDCOffset = 0;		// mV
const double DCOffsetTol = 3;		// mV

const int	IOAddr = 0x210;
const int 	DUTCOM = 1;				// Device Under Test            COM Port#
const int 	VCACOM = 5;				// R & S   VCA                  COM Port#
const int 	PT5210COM = 6;			// PT5210 sync gen i test rack  COM Port#
const int 	PM3094COM = 7;			// PM3094 scop i test rack  	COM Port#
const char 	*COMname[7] = {"com1","com2","","","com5","com6","com7"};


const int PM5640GNomScH = 152;		// 152 = 0° ScH fase for 5640G KU021306
const int PM5640MNomScH = 303;		// 303 = 0° ScH fase for 5640M KU021307
									// ScH data læses med ReadPM5640ScHCalData()
									// Se menupunkt i Diverse


char DUTStr[50];

int DACData[8][2];				  // [BB][syncDAC,offsetDAC]
unsigned int SDIData[4][2];		  // [SDI][Gdata,Mdata]		(SDI=black1,2,3 + TSG)
unsigned int PhaseData[8][4];	  // [BB][GPhase,GScH,MPhase,MScH]
unsigned int SPGCalData[4];

unsigned char PM5662Status; // Status af PM5662 indstilling


int 	IEEEboard;

int		SyncLevelOk;			// boolean variables
int		DCOffsetOk;
int		PhaseScHOk;

int		mainPNL,
	 	cfgPNL,					// kalibreringsdata for enheder
		oplysPNL,			  	// oplysninger om KU, 12nc, dato  m.v.
	 	spghphPNL,
	 	spggenlPNL,
	 	aesPNL,
	 	vccPNL,
	 	dataPNL,
	 	fanPNL,
	 	ocxoPNL,
	 	sditotPNL,
	 	basictotPNL,
		spgphasePNL;

int 	PasswordUsed;
int 		devicePM5640G,
	 		devicePM5640M;
int			ErrIEEE,
	 		Err232;
int 		hp34401;
int 		hp53132;

int			done;

char 		ASCIIdata[100];		// ASCII data read from a COM port
unsigned char BINdata[2000];	// Binary data read from a COM port
 

int Cont;
int	Repeat;






void VLMdata(unsigned short xdata, unsigned short sel){
// IOPort A:   bit 0: SCL    bit 1: SDA    bit 2..4: SEL 0..2   
unsigned char n,p;

	xdata = xdata << 6;
	p = 0xe0 + (sel * 4);	// control data bit 2,3,4  (bit 5,6,7=1)
	for (n = 0; n < 10; ++n)
	 {
	  if ((xdata & 0x8000) == 0)
	    p = p & 0xfd;		// set SDA
	  if ((xdata & 0x8000) == 0x8000)
	    p = p | 0x02;			
	  outp(IOAddr,p); ++p;
	  outp(IOAddr,p); --p;
	  outp(IOAddr,p);
	  
	  xdata = xdata << 1;
	  
	} // for
} // vlmdata


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





void SetPM5662(int TVSystem){

/* TVSystem = G | M          Mode = VECT | WFM
   IOPort B:   bit 0: 5662G    pin  7(!WFM)	   pin 5(!A_B)	  !=active low
               bit 1: 5662G    pin 14(B-in)
               bit 2: 5662M    pin 7(VECT)	   pin 5(A_B)
               bit 3: 5662M    pin 14(!B-in)
               hardwired	   pin  4=low  (!2H)
               				   pin 17=low  (!XMAG)
               				   pin 13=low  (remote=on)
               				   pin  1=GND
               				   
  bit#		3 2 1 0
 -------------------- 
  G WFM		x x 1 0
  G VECT	x x 0 1
  M WFM		1 0 x x
  M VECT	0 1 x x
*/
 
 if (TVSystem == PAL){
   PM5662Status = PM5662Status | 0xF9;	   // bit 0,3 = 1   (G VETC)
   PM5662Status = PM5662Status & 0xF9;	   // bit 1,2 = 0	(M WFM)
  }

 if (TVSystem == NTSC){
   PM5662Status = PM5662Status | 0xF6;	   // bit 1,2 = 1   (M VETC)
   PM5662Status = PM5662Status & 0xF6;	   // bit 0,3 = 0   (G WFM)
  }

  outp(IOAddr+1,PM5662Status);
  Delay(0.01);

}






void SetGenlockSignal(int Signal){

 outp(IOAddr+2,0xFF);
 Delay(0.02);
 switch (Signal){
  case GENL_PAL:	  outp(IOAddr+2,0xFE); break;
  case GENL_NTSC: 	  outp(IOAddr+2,0xFC); break;
  case GENL_443_HIGH: outp(IOAddr+2,0xFD); break;
  case GENL_443_LOW:  outp(IOAddr+2,0xF5); break;
  case GENL_358_HIGH: outp(IOAddr+2,0xFB); break;
  case GENL_358_LOW:  outp(IOAddr+2,0xF3); break;
  case GENL_10M_HIGH: outp(IOAddr+2,0xFA); break;
  case GENL_10M_LOW:  outp(IOAddr+2,0xF2); break;
  case GENL_OFF:	  outp(IOAddr+2,0xFF); break;
 }

}











char *ReadlnCOMPort(int PortNo, double Timeout){
int bytes_read;
 SetComTime (PortNo, Timeout);   // set timeout
 ASCIIdata[0] = 0x0;
 bytes_read = ComRdTerm (PortNo, ASCIIdata, 100, LF);   //terminates on LF
 if (bytes_read > 0) {
  if(ASCIIdata[bytes_read - 1] == CR)
    ASCIIdata[bytes_read - 1] = 0x00;
 }  
  else{
//   ASCIIdata[0] = 0x30;
   ASCIIdata[1] = 0x00;	   // '0' + NULL
   }
 
 return (&ASCIIdata[0]);
}



unsigned char *ReadBinCOMPort(int PortNo, double Timeout, int NoOfBytes){
int bytes_read;
int InQL;
// SetComTime (PortNo, Timeout);   // set timeout
 BINdata[0] = 0x0;
 bytes_read = ComRd (PortNo, BINdata, NoOfBytes);   //terminates on NoOfBytes
 BINdata[bytes_read] = 0x0;
 return (&BINdata[0]);
}



void WriteCOMPort(int PortNo, char TransmitStr[]){
int bytes_send;
int NoOfBytes;

 NoOfBytes = StringLength (TransmitStr);
 FlushOutQ(PortNo);
 bytes_send = ComWrt (PortNo, TransmitStr, NoOfBytes);
 
 while 
  (GetOutQLen(PortNo)> 0);
}




void WaitForContinue(void){
int handle_returned, controlID_returned;
 Cont = FALSE;
 while (Cont == FALSE)
  GetUserEvent(NOWAIT,&handle_returned, &controlID_returned);
}






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
  
  
  
int Set5640Standard(char GM[2]){
int wrtstatus, rdstatus, sch_read, sch_val;
char read_buf[30], wrt_buf[30];
#if IEEEBUS 
 
 GM[0] = toupper((char)GM[0]);
 
 if (GM[0] == 'G'){
  wrtstatus = ibwrt (devicePM5640G, "STANDARD", 8);
  Delay(0.01);  
  wrtstatus = ibwrt (devicePM5640G, "SCHPHASE ?", 10);
  rdstatus  = ibrd (devicePM5640G, read_buf, 10);
  Scan(read_buf,"%s>%i",&sch_read);
  
  if ((sch_read > 1023) || (sch_read < 0)){
   MessagePopup (" PM5640G Sch fase","ADVARSEL: Fejl ved indstilling af PM5640G\n Program stoppes");
   return -1;
  }

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
 } 

 if (GM[0] == 'M'){
  wrtstatus = ibwrt (devicePM5640M, "STANDARD", 8);
  Delay(0.01);  
  wrtstatus = ibwrt (devicePM5640M, "SCHPHASE ?", 10);
  rdstatus  = ibrd (devicePM5640M, read_buf, 10);
  Scan(read_buf,"%s>%i",&sch_read);
  
  if ((sch_read > 1023) || (sch_read < 0)){
   MessagePopup (" PM5640M Sch fase","ADVARSEL: Fejl ved indstilling af PM5640M\n Program stoppes");
   return -1;
  }
  
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
 
 Delay(0.01);  
#endif
return 0; 
}
  


int ReadPM5640ScHCalData(void){
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

  Delay(0.01);  
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




int Set5640ExternDataVits(void){
char SendStr[30];
int wrtstatus;
#if IEEEBUS 

 Fmt(SendStr,"%s<VITSLINE 10,DATA");
 wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 Delay(0.02);  
 Fmt(SendStr,"%s<VITSLINE 12,DATA");
 wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 Delay(0.02);  
 Fmt(SendStr,"%s<VITSLINE 323,DATA");
 wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 Delay(0.02);  
 Fmt(SendStr,"%s<VITSLINE 325,DATA");
 wrtstatus = ibwrt (devicePM5640G, SendStr, strlen(SendStr));
 Delay(0.02);  

 
 Fmt(SendStr,"%s<VITSLINE 1,10,DATA");
 wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.02);  
 Fmt(SendStr,"%s<VITSLINE 1,12,DATA");
 wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.02);  
 Fmt(SendStr,"%s<VITSLINE 2,10,DATA");
 wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.02);  
 Fmt(SendStr,"%s<VITSLINE 2,12,DATA");
 wrtstatus = ibwrt (devicePM5640M, SendStr, strlen(SendStr));
 Delay(0.02);  
#endif
return wrtstatus; 
}







void SelectPM3094Setup(int SetupNo){
char DataStr[50];
char *strread;
#if AT232
 Fmt(DataStr,"%s<RS %i\r",SetupNo);  
 WriteCOMPort(PM3094COM,DataStr);
 WriteCOMPort(PM3094COM,"GL\r");
#endif
}



int WriteProtect(int OnOff, int flag){
char *strread;
int OnOffOk;

OnOffOk = TRUE;

#if AT232
 if (OnOff == ON){
  WriteCOMPort(DUTCOM,":FACT:PASS ON;");
  Delay(0.05);
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:PASS?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strcmp (strread, "ON") != 0) {
   MessagePopup(" Skrivebeskyttelse"," FEJL:  Ikke sat ON\n Check RS-232 kabel, og at apparatet er tændt");
   OnOffOk = FALSE;
  } 
  else
  if (flag == TRUE)
   MessagePopup(" Skrivebeskyttelse"," ON");
 }
 
 else{
  WriteCOMPort(DUTCOM,":FACT:PASS 'Marilyn';");
  Delay(0.05);
 
  if (PasswordUsed == FALSE)			 // master SW uden password funktion
   return TRUE;
  
  FlushInQ(DUTCOM);
  WriteCOMPort(DUTCOM,":FACT:PASS?;");
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strcmp (strread, "OFF") != 0){	 // master SW med password funktion
   MessagePopup(" Skrivebeskyttelse"," FEJL:  Ikke sat OFF\n Check RS-232 kabel, og at apparatet er tændt");
   OnOffOk = FALSE;
  }
  else
  if (flag == TRUE)
   MessagePopup(" Skrivebeskyttelse"," OFF");
 } 

#endif
 return OnOffOk;
}


int CheckFrontRearTerminal(int Term){
int ConfirmReply;
ViInt16 InputTermHP34401; // = signed short
 
#if IEEEBUS

 hp34401a_checkTerminals (hp34401, &InputTermHP34401);

 if (InputTermHP34401 != Term) {
  do {
   if (Term == FRONTTERM34401)
     ConfirmReply = ConfirmPopup (" HP34401 input ", "Tasten \"Terminals\" skal være trykket ud");
    else
     ConfirmReply = ConfirmPopup (" HP34401 input ", "Tasten \"Terminals\" skal være trykket ind");

   hp34401a_checkTerminals (hp34401, &InputTermHP34401);

  } while ((ConfirmReply == YES) & (InputTermHP34401 != Term));

 if (ConfirmReply == NO)
   return FALSE;
  else
   return TRUE;
 } //if

 if (Term == InputTermHP34401)
   return TRUE;
  else 
   return FALSE;

#endif
}


int CheckReadyVCA(void){
char IDStr[40];
int ConfirmReply,
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
