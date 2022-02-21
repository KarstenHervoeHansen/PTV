/* FILE NAME    config.c
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
 * NOTES:    
 *          
*/



#include <ansi_c.h>
#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <userint.h>

#include "cfg.h"
#include "def.h"
#include "bbcal.h"
#include "spgcal.h"
#include "sditest.h"
#include "aescal.h"
#include "cviutil.h"
#include "pt5201.h"


const char CalDataFilePath[2][50] = {"c:\\measdata\\","m:\\measdata\\5201\\"};  // where to save caldata on disk

const char FileExt5201[5] = {"._01"};

char BBStr[4][120],
    SPGStr[120],
    TSGStr[120],
    AudioStr[120],
    DacRefStr[2][50], // data for intern ref dac V74
     MainIDNStr[40],
     MainNCStr[20],
     MainFMSDateStr[40],
     MainSYSTStr[40],
     MainUpdateStr[40],
     MainKUStr[10],
     MasterSWVer[5],
    MasterSWVerStr[50],
    FileCreateStr[40];

//overskrifter
const char header_SPG[100]    = {"\nSPG            GFase  MFase          Ghphz Mhphz                  Date    SW\n"};
const char header_BB[100]     = {"\nBB#  Gain Offset  Level    GFase      MFase      GScH MSch        Date    SW\n"};
const char header_Audio[100]  = {"\nAudio     Left    Right                                           Date\n"};
const char header_TSG[100]    = {"\nTSG  Gain      Chroma      GFase   MFase         GSch MScH        Date    SW\n"};

int ret_flag;

// prototypes
int SaveCalData(void);
int dim_menu(void);
int undim_menu(void);


//----------------------------------------------------------------------------------------------------------
int ReadBB(int BBNo){
int dd[3];
char dacstr[20],
    phasestr[30],
    schstr[30],
    caldatestr[10],
    swstr[5];
 
  // read BB sw ver
  strread = ReadBBSWVer();
  if (strlen(strread) > 0)
    Fmt(swstr,"%s<%s",strread);
   else
    Fmt(swstr,"%s<???");

  // read BB cal date
  strread = ReadBBCalDate(BBNo);
  Fmt(caldatestr,"%s<%s",strread);

  // read 3xDAC (gain,offset,level)
  FlushInQ(DUTCOM);
  Fmt(DUTStr,"%s<:FACT:BB%i:DAC?;",BBNo);
  WriteCOMPort(DUTCOM,DUTStr);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  Scan(strread,"%s>%3i[x]",dd);
  if (strlen(strread) > 0)
    Fmt(dacstr,"%s<%i[w3p0],   %i[w3p0],   %i[w3p0]",dd[0],dd[1],dd[2]);
   else
    Fmt(dacstr,"%s<   ,      ,      ");
 
  // read PAL,NTSC phase
  FlushInQ(DUTCOM);
  Fmt(DUTStr,"%s<:FACT:BB%i:PHAS?;",BBNo);
  WriteCOMPort(DUTCOM,DUTStr);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 0)
    Fmt(phasestr,"%s<%s",strread);
   else
    Fmt(phasestr,"%s<%s              ",strread);

  // read PAL,NTSC sch-phase
  FlushInQ(DUTCOM);
  Fmt(DUTStr,"%s<:FACT:BB%i:SCHP?;",BBNo);
  WriteCOMPort(DUTCOM,DUTStr);
  strread = ReadlnCOMPort(DUTCOM,0.5);
  if (strlen(strread) > 0)
    Fmt(schstr,"%s<%s",strread);
   else
    Fmt(schstr,"%s<%s              ",strread);


  Fmt(BBStr[BBNo],"%s<BB%i,  %s,  %s,  %s,      %s,  %s\n",BBNo,dacstr,phasestr,schstr,caldatestr,swstr);


return TRUE;
} // ReadBB(int BBNo)




//----------------------------------------------------------------------------------------------------------
int ReadSPG(void){
char phasestr[30],
    hphzstr[30] ,
    caldatestr[10],
    swstr[10];

 // read SPG sw ver
 strread = ReadSPGSWVer();
 if (strlen(strread) == 3)
   Fmt(swstr,"%s<%s",strread);
  else
   Fmt(swstr,"%s<???");

 // read SPG cal date
 strread = ReadSPGCalDate();
 Fmt(caldatestr,"%s<%s",strread);

 // read SPG phases for PAL and NTSC
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:PHAS?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 0)
   Fmt(phasestr,"%s<%s",strread);
  else
   Fmt(phasestr,"%s<%s              ",strread);

 // read SPG hph zero for PAL and NTSC
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:HPHZ?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 0)
   Fmt(hphzstr,"%s<%s",strread);
  else
   Fmt(hphzstr,"%s<%s              ",strread);


 Fmt(SPGStr,"%s<               %s,    %s,            %s,  %s\n",phasestr,hphzstr,caldatestr,swstr);

 
return TRUE; 
} // ReadSPG(void)





//----------------------------------------------------------------------------------------------------------
int ReadAudio(void){
char levelstr[30],
    caldatestr[10];
int dd[2];    

 strread = ReadAudioCalDate();
 Fmt(caldatestr,"%s<%s",strread);

 // read audio level for left/right
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:AUD:ALEV?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%2i[x]",dd);
 if (strlen(strread) > 0)
   Fmt(levelstr,"%s<%i[w3p0],   %i[w3p0]",dd[0],dd[1]);
  else
   Fmt(levelstr,"%s<   ,      ,      ");

 Fmt(AudioStr,"%s<           %s,",levelstr);
 Fmt(AudioStr,"%s[aw43]< ");
 Fmt(AudioStr,"%s[a]<%s\n",caldatestr);

return TRUE;
} // ReadAES(void)









//----------------------------------------------------------------------------------------------------------
int ReadColorBarGen(void){
char gainstr[30],
   chromastr[30],
   phasestr[30],
   schstr[30],
   caldatestr[10];
int dd[2];   

 // read TSG cal date
 strread = ReadTSGCalDate();
 Fmt(caldatestr,"%s<%s",strread);

 // digital potm
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TSG:AGAIN?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%2i[x]",dd);
 if (strlen(strread) > 0)
   Fmt(gainstr,"%s<%i[w3p0],%i[w3p0]",dd[0],dd[1]);
  else
   Fmt(gainstr,"%s<   ,   ");

 // digital chroma gain
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TSG:CGAIN?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%2i[x]",dd);
 if (strlen(strread) > 0)
   Fmt(chromastr,"%s<%i[w3p0],%i[w3p0]",dd[0],dd[1]);
  else
   Fmt(chromastr,"%s<   ,   ");

 // read phase
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TSG:PHAS?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%2i[x]",dd);
 if (strlen(strread) > 0)
   Fmt(phasestr,"%s<%i[w7p0],%i[w7p0]",dd[0],dd[1]);
  else
   Fmt(phasestr,"%s<       ,       ");

 // read sch-phase
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:TSG:SCHP?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Scan(strread,"%s>%2i[x]",dd);
 if (strlen(strread) > 0)
   Fmt(schstr,"%s<%i[w3p0], %i[w3p0]",dd[0],dd[1]);
  else
   Fmt(schstr,"%s<   ,   ");

 Fmt(TSGStr,"%s<     %s,  %s,    %s,      %s,       %s,  %s\n",gainstr,chromastr,phasestr,schstr,caldatestr,MasterSWVer);



return TRUE;
} // ReadColorBarGen(void)




//----------------------------------------------------------------------------------
int ReadDacRef(void){
// read data for ref dac V74
char factstr[2][30],
    userstr[2][30];
int dd[3];

 // read factory dac value
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:FREF:DAC?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Fmt(factstr[0],"%s<%s",strread);

 // read factory cal date
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:FREF:DATE?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(factstr[1],"%s<%i[w2p0]%i[w2p0]%i[w2p0]",dd[0],dd[1],dd[2]); //yymmdd
 } 
  else
   Fmt(factstr[1],"%s<??????");
   
 // read user dac value
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:UREF:DAC?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 Fmt(userstr[0],"%s<%s",strread);

 // read user cal date
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:GENL:UREF:DATE?;");
 strread = ReadlnCOMPort(DUTCOM,0.5);
 if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(userstr[1],"%s<%i[w2p0]%i[w2p0]%i[w2p0]",dd[0],dd[1],dd[2]); //yymmdd
 } 
  else
   Fmt(userstr[1],"%s<??????");
   
 Fmt(DacRefStr[0],"%s<DAC V74 factory ref:  %s      Date: %s\n",factstr[0],factstr[1]);
 Fmt(DacRefStr[1],"%s<DAC V74 user ref:     %s      Date: %s\n",userstr[0],userstr[1]);


return TRUE;
}




//----------------------------------------------------------------------------------
int ReadCalData(void){
int dd[3];
int n,p,
   handle_returned, controlID_returned;


 //SaveToFile = FALSE;
 DisplayPanel(configPNL);
 SetCtrlAttribute (configPNL, CONFIG_RETURN_BTN,ATTR_CTRL_TAB_POSITION, 0);
 SetCtrlAttribute (configPNL, CONFIG_SAVE_BTN, ATTR_VISIBLE, ENABLED);  // kun gemning i fil en gang pr. læsning
 
 ResetTextBox (configPNL, CONFIG_TXT, "");
 
 // tekstfarve = rød
 SetCtrlAttribute (configPNL, CONFIG_TXT, ATTR_TEXT_COLOR, VAL_RED);
 ProcessDrawEvents();

 // set password = off & check RS-232 forbindelse
 if (WriteProtect(OFF,FALSE) == FALSE)
   {
   SetCtrlAttribute (configPNL, CONFIG_SAVE_BTN, ATTR_VISIBLE, DISABLED);  // F10 = save button bruges ikke her
   HidePanel (configPNL);
   return FALSE;
   }

  dim_menu();
 //SetMenuBarAttribute (mainmenuhandle, MAINMENU_DIVERSE_CALDATA, ATTR_DIMMED,ENABLED);


 //read company
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:COMP?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 Fmt(MainIDNStr,"%s<%s,",strread);    

 //read type
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:TYPE?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 Fmt(MainIDNStr,"%s[a]<%s,",strread);   

 //read KUnumber  ,PT5201-rev
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:KUN?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 Fmt(MainIDNStr,"%s[a]<%s,",strread);   
 // Main KU number (bruges til filnavn ved gemning)
 if (strlen(strread) > 6)
   CopyString(MainKUStr,0,strread,2,6);
  else
   Fmt(MainKUStr,"%s<KU??????");    

 //read PT5201-rev
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:SWR?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 Fmt(MainIDNStr,"%s[a]<%s\n",strread);    
 SetCtrlVal (configPNL, CONFIG_TXT, MainIDNStr);

 //read master-CPU sw-rev
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:VERS?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 Fmt(MasterSWVerStr,"%s<Master CPU version: %s\n",strread);   
 SetCtrlVal (configPNL, CONFIG_TXT, MasterSWVerStr);
 Fmt(MasterSWVer,"%s<%s",strread);    
 
 //read System  (PAL or NTSC)
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:SYST?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
 Fmt(MainSYSTStr,"%s<System: %s\n",strread);    
 SetCtrlVal (configPNL, CONFIG_TXT, MainSYSTStr);

 //read FMS production date
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:FMSD:PROD?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
  if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(MainFMSDateStr,"%s<Production: %i[w2p0]%i[w2p0]%i[w2p0]\n",dd[0],dd[1],dd[2]); //yymmdd
  } 
  else
   Fmt(MainFMSDateStr,"%s<??????"); //yymmdd
 SetCtrlVal (configPNL, CONFIG_TXT, MainFMSDateStr);

 //read FMS update date (last calibration)
 FlushInQ(DUTCOM);
 WriteCOMPort(DUTCOM,":FACT:MAIN:FMSD:UPD?;");
 strread = ReadlnCOMPort(DUTCOM,0.5); 
  if (strlen(strread) > 1){
   Scan(strread,"%s>%3i[x]",dd);
   Fmt(MainUpdateStr,"%s<Last calibration: %i[w2p0]%i[w2p0]%i[w2p0]\n",dd[0],dd[1],dd[2]);  //yymmdd
  } 
  else
   Fmt(MainUpdateStr,"%s<??????");  //yymmdd

 SetCtrlVal (configPNL, CONFIG_TXT, MainUpdateStr);


// ---- DAC V74 intern ref -----
 if (ReadDacRef() == TRUE){
  SetCtrlVal (configPNL, CONFIG_TXT, DacRefStr[0]);
  SetCtrlVal (configPNL, CONFIG_TXT, DacRefStr[1]);
 }

 
 // ---- SPG ----------
 SetCtrlVal (configPNL, CONFIG_TXT, header_SPG);
 ReadSPG();
 SetCtrlVal (configPNL, CONFIG_TXT, SPGStr);
 ProcessDrawEvents();
 

 // --- Black Burst ------
 SetCtrlVal (configPNL, CONFIG_TXT, header_BB);

 for (n = 1;n <= 3; n++){  // BB# 1 - 3
  if (ReadBB(n) == TRUE)
   SetCtrlVal (configPNL, CONFIG_TXT, BBStr[n]);
  ProcessDrawEvents();
 }
 

 // --- Audio ------
 SetCtrlVal (configPNL, CONFIG_TXT, header_Audio);
 if (ReadAudio() == TRUE)
  SetCtrlVal (configPNL, CONFIG_TXT, AudioStr);
 ProcessDrawEvents();


 // --- Analog TSG  ------
 SetCtrlVal (configPNL, CONFIG_TXT, header_TSG);
 
 if (ReadColorBarGen() == TRUE)
  SetCtrlVal (configPNL, CONFIG_TXT, TSGStr);
 ProcessDrawEvents();
 
 SetCtrlAttribute (configPNL, CONFIG_SAVE_BTN, ATTR_VISIBLE, DISABLED);  // F10 = save button bruges ikke her

// --- press F10 or F12 -----------
 do {
  GetUserEvent (WAIT, &handle_returned, &controlID_returned);
  switch (controlID_returned){
   case CONFIG_SAVE_BTN:  SaveCalData(); break; // F10
   case CONFIG_RETURN_BTN:  break;            // F12
  }
   
 } while (controlID_returned != CONFIG_RETURN_BTN);

 undim_menu();
 HidePanel(configPNL);

return 0;

} // ReadCalData







int SaveCalData(void){
/*
Gemmer kalibreringsdata
Data gemmes i en fil med navnet xxxxxxY._zz hvor
 xxxxxx = KU nummer for apparatet (main board)
 Y    = A for første gang der gemmes, derefter B,C,D.. osv.
 zz     = 01 for PT5201
*/
char filename[50], 
     wrtbuf[100], 
     MsgStr[200];
int  bytes_written, 
     fhandle, 
     n, 
     FilePresent, 
     IOErr,
     FmtIOErr,
     fno;
long fsize; 

  fno = 64;  // = ASCII 65 = A
  FilePresent = TRUE;
  while (FilePresent == TRUE) {
   fno++;              // start with 'A'
   Fmt(filename,"%s<%s%s%c%s",CalDataFilePath[LogDest],MainKUStr,(char)fno,FileExt5201);
   if (fno > 90 ){           // ASCII 90 = 'Z'
    Fmt(filename,"%s<%s%s%c%s",CalDataFilePath[LogDest],MainKUStr,(char)(fno-1),FileExt5201);
    Fmt(MsgStr,"%s< Der er ingen ledig fil til at gemme data i\n Sidst brugte fil var '%s'\n Slet nogle af filerne i '%s'",filename,CalDataFilePath[LogDest]);
    MessagePopup(" Fejl ved gemning af kalibreringsdata",MsgStr);
    HidePanel(configPNL);
    return FALSE;
   } 
   FilePresent = GetFileInfo (filename, &fsize);
  } //while


  fhandle = OpenFile (filename, VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_ASCII);

  bytes_written = WriteFile (fhandle, MainIDNStr, strlen(MainIDNStr));
  bytes_written = WriteFile (fhandle, MasterSWVerStr, strlen(MasterSWVerStr));
  bytes_written = WriteFile (fhandle, MainSYSTStr, strlen(MainSYSTStr));
  bytes_written = WriteFile (fhandle, MainFMSDateStr, strlen(MainFMSDateStr));
  bytes_written = WriteFile (fhandle, MainUpdateStr, strlen(MainUpdateStr));
  bytes_written = WriteFile (fhandle, DacRefStr[0], strlen(DacRefStr[0]));
  bytes_written = WriteFile (fhandle, DacRefStr[1], strlen(DacRefStr[1]));
  
  // SPG
  bytes_written = WriteFile (fhandle, header_SPG, strlen(header_SPG));
  bytes_written = WriteFile (fhandle, SPGStr, strlen(SPGStr));

  // BB
  bytes_written = WriteFile (fhandle, header_BB, strlen(header_BB));
  for (n = 1; n <= 3; n++){
   if (strlen(BBStr[n]) > 10)
    bytes_written = WriteFile (fhandle, BBStr[n], strlen(BBStr[n]));
  } 

 // Color Bar
  bytes_written = WriteFile (fhandle, header_TSG, strlen(header_TSG));
  bytes_written = WriteFile (fhandle, TSGStr, strlen(TSGStr));

 // AES
   bytes_written = WriteFile (fhandle, header_Audio, strlen(header_Audio));
   bytes_written = WriteFile (fhandle, AudioStr, strlen(AudioStr));
 
 // Write file creation date 
 GetDate(2);
 Fmt(FileCreateStr,"%s<\nFile created: %s%s%s\n",YearStr,MonthStr,DayStr);
 bytes_written = WriteFile (fhandle, FileCreateStr, strlen(FileCreateStr));


  IOErr = CloseFile (fhandle);

  if (IOErr != -1){
    Fmt(MsgStr,"%s< Kalibreringsdata gemt i fil '%s'",filename);
    MessagePopup(" Kalibreringsdata",MsgStr);
   }
  else{
  FmtIOErr = GetFmtIOError ();
    Fmt(MsgStr,"%s< Fejl ved gemning i fil '%s'\n Fejlkode=%i",filename,FmtIOErr);
    MessagePopup(" Kalibreringsdata",MsgStr);
  } 

 SetCtrlAttribute (configPNL, CONFIG_SAVE_BTN, ATTR_VISIBLE, ENABLED);  // kun gemning i fil en gang pr. læsning

return TRUE;
} // SaveCalData(void) 








//----------------------------------------------------------------------------------

int ShowCalFiles(void)
  {
  int  select_status, 
       dfile, 
       bytes_read,
       IOErr,
       lineno;
  char selected_path[MAX_PATHNAME_LEN], 
       line[150];
  int handle_returned, controlID_returned;
  
 SetPanelAttribute (configPNL, ATTR_TITLE, "Kalibrering Filer");
 DisplayPanel(configPNL);   //configPNL findes i cfg.uir file
 ResetTextBox (configPNL, CONFIG_TXT, "");
 //SetCtrlAttribute (configPNL, CONFIG_RETURN_BTN,ATTR_CTRL_TAB_POSITION, 0);
 ProcessDrawEvents();   
 SetCtrlAttribute (configPNL, CONFIG_SAVE_BTN, ATTR_VISIBLE, ENABLED);  // F10 = save button bruges ikke her
 select_status = VAL_EXISTING_FILE_SELECTED;
 ret_flag = 0;
 dim_menu();
 while (select_status == VAL_EXISTING_FILE_SELECTED) 
   {
   select_status = FileSelectPopup (CalDataFilePath[LogDest], "*.*", "*.*;*.*",
                                    " Kalibreringsfiler",
                                    VAL_SELECT_BUTTON, 1, 0, 1, 0,
                                    selected_path);

   if (select_status != VAL_EXISTING_FILE_SELECTED) 
     {
     SetCtrlAttribute (configPNL, CONFIG_SAVE_BTN, ATTR_VISIBLE, ENABLED);  // F10 = save button bruges ikke her
     undim_menu();
     HidePanel(configPNL);
     return TRUE;
     }
   SetPanelAttribute (configPNL, ATTR_TITLE, selected_path);
   SetCtrlAttribute (configPNL, CONFIG_TXT, ATTR_TEXT_COLOR, VAL_RED);
  
   dfile = OpenFile (selected_path, VAL_READ_ONLY, VAL_OPEN_AS_IS,VAL_ASCII);

   ResetTextBox (configPNL, CONFIG_TXT, "");
   bytes_read = ReadLine (dfile, line, 100);
   IOErr = GetFmtIOError ();
   if ((IOErr == 0) && (bytes_read >= 0)) 
     {
     Fmt(line,"%s[a]<\n");
     SetCtrlVal(configPNL, CONFIG_TXT,line);
     }
   while ((IOErr == 0) && (bytes_read >= 0))
     {
     bytes_read = ReadLine (dfile, line, 100);
     IOErr = GetFmtIOError ();
     Fmt(line,"%s[a]<\n");
     SetCtrlVal(configPNL, CONFIG_TXT,line);
     } // while

   IOErr = CloseFile (dfile);
   ProcessDrawEvents();
   do 
     {
     GetUserEvent (WAIT, &handle_returned, &controlID_returned);
     switch (controlID_returned)
       {
       case CONFIG_SAVE_BTN:
         SaveCalData(); 
         break; // F10
       case CONFIG_RETURN_BTN:  
         break;            // F12
       }
     } while (controlID_returned != CONFIG_RETURN_BTN);
   break;
   //WaitForContinue();     // <<<<Error here   
   } //while
  SetCtrlAttribute (configPNL, CONFIG_SAVE_BTN, ATTR_VISIBLE, ENABLED);  // F10 = save button bruges ikke her
  undim_menu();
  HidePanel(configPNL);  
  return TRUE;
  } // ShowCalFiles(void)
 

int CVICALLBACK CFGReturnCallBack (int panel, int control, int event, // F12
    void *callbackData, int eventData1, int eventData2)
  {
  switch (event) 
    {
    case EVENT_COMMIT:
      ret_flag = 1;
      break;
    case EVENT_RIGHT_CLICK:    
      break;
    }
  return 0;
  }


int CVICALLBACK CFGSaveCallBack (int panel, int control, int event,  // F10
    void *callbackData, int eventData1, int eventData2)
{
  switch (event) {
    case EVENT_COMMIT:
      //ret_flag = 1; 
      break;
  }
  return 0;
}


int dim_menu(void)
  {
  int lerr;
  
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_BB, ATTR_DIMMED, DISABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_COLORBAR, ATTR_DIMMED, DISABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_SDI, ATTR_DIMMED, DISABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_SPG, ATTR_DIMMED, DISABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_AUDIO, ATTR_DIMMED, DISABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_DIVERSE, ATTR_DIMMED, DISABLED);  
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_COMMAND, ATTR_DIMMED, DISABLED);  
  return lerr;
  }
  
int undim_menu(void)
  {
  int lerr;
  
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_BB, ATTR_DIMMED, ENABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_COLORBAR, ATTR_DIMMED, ENABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_SDI, ATTR_DIMMED, ENABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_SPG, ATTR_DIMMED, ENABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_AUDIO, ATTR_DIMMED, ENABLED);
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_DIVERSE, ATTR_DIMMED, ENABLED);  
  lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_COMMAND, ATTR_DIMMED, ENABLED);  
  return lerr;
  }
