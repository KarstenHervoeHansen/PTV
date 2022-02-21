/*********************************************************************************/
/* FILE NAME     pt5201.c
 * PURPOSE       Main file for PT5201/PT5202 testprogram  
 *             
 * SPEC       
 * DESIGN        000224 Peter Frederiksen, PTV
 * CODING    
 * CODE INSP.
 * TEST     
 *            
 * MODIFIED:     20.05.2003: Made to calibate 5201 for LEADER
 *               20.03.2003: DELTA version for 5201/5202 JK
 *
 * NOTES:        21.12.2000: Two new tests added, CHECK PRESETS and FINAL CONFIG.
 *
 *               File: "def.h" conains conditionals for disabling all periferials
 *               so the program can be run on any PC with COM1 only.
/*********************************************************************************/

#define TESTSWVER  "VER: 5201 ver 1.0"
#define CHECKED    1
#define UNCHECKED  0

#include "hp34970a.h"   
#include "hp5313xa.h"
#include <ansi_c.h>
#include <gpib.h>
#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <cvirte.h>   /* Needed if linking in external compiler; harmless otherwise */



// header files for uir files
#include <userint.h>                  
#include "pt5201.h"
#include "bb_sync.h"
#include "phase.h"
#include "sdi.h"
#include "vlm.h"
#include "cfg.h"
#include "spghph.h"
#include "spggenl.h"
#include "spgphase.h"
#include "aes.h"
#include "refosc.h"
#include "vcc.h"
#include "presets.h" 
#include "fconfig.h" 

// general utilities
#include "cviutil.h"
#include "def.h"    // define statements

// calibration utilities
#include "vlmutil.h"
#include "bbcal.h"
#include "spgcal.h"
#include "sditest.h"
#include "aescal.h"
#include "config.h"
#include "clktest.h"


// Globals
int test_no;    //current test number :JK
char *test_names[] =
  {
  " 1. PS + 3.5 V",
  " 2. PS + 5.0 V",
  " 3. PS - 5.0 V",
  " 4. RESET SPG",
  " 5. JUSTER 13 MHz TCXO",
  " 6. KALIBRER GENLOCK AUTO HPH",
  " 7. CHECK GENLOCK",
  " 8. BB1",
  " 9. BB2",
  "10. BB3",
  "11. TSG LUMINANCE KALIBRERING",
  "12. TSG TIMING KALIBRERING",
  "13. TSG PATTERN CHECK",
  "14. JITTER & AMPLITUDE SPECTRUM",
  "15. EMBEDDED AUDIO",
  "16. KALIBRER ANALOG AUDIO LEVEL",
  "17. CHECK ANALOG AUDO LEVEL",
  "18. CHECK ANALOG AUDIO FREQUENCY",
  "19. CHECK AES-GENETATOR", 
  "20. CHECK WORD CLOCK",
  "21. CHECK PRESETS",
  "22. FINAL KONFIGURATION"
  };

extern int Cont;
int presets_ok;
int fconfig_ok;
int pPNL;
int fPNL;


// Prototypes
int do_all_tests(void); 
int make_bb_test(void);
int make_presets(void);
int make_fconfig(void);
void set_standard(int);
void EnterSerialNo(char text[50], char *kuread, char *kustr);
int CheckSerialNo(char *kustr);



int main (int argc, char *argv[])
{
int ConfirmReply;
int sm,sg;
char ff[MAX_FILENAME_LEN];
int button_pressed;
int aterr;
int i;




if (argc > 1)
  {
  if ((strcmp(argv[1],"5201") == 0) || (strcmp(argv[1],"PT5201") == 0))
    AppType = PT5201;
  else
    AppType = PT5201;
  } //if 


#if IEEEBUS
   IEEEboard = ibfind ("gpib0");
  devicePM5640G = ibdev (0, PM5640GIEEEADDR, NO_SAD, T3s, 1, 0);
  sg = iberr;
  devicePM5640M = ibdev (0, PM5640MIEEEADDR, NO_SAD, T3s, 1, 0);
  sm = iberr;
#endif
  

  if (InitCVIRTE (0, argv, 0) == 0) /* Needed if linking in external compiler; harmless otherwise */
    return -1;  /* out of memory */
  if ((mainPNL = LoadPanel (0, "pt5201.uir", MAIN_PNL)) < 0)
     return -1;
  if ((syncPNL = LoadPanel (mainPNL, "bb_sync.uir", SYNC_PNL)) < 0)
    return -1;
  if ((vlmPNL = LoadPanel (mainPNL, "vlm.uir", VLM_PNL)) < 0)
    return -1;
  if ((spghphPNL = LoadPanel (mainPNL, "spghph.uir", SPGHPH_PNL)) < 0)
    return -1;
  if ((spggenlPNL = LoadPanel (mainPNL, "spggenl.uir", SPGGEN_PNL)) < 0)
    return -1;
  if ((selgenlPNL = LoadPanel (0, "spggenl.uir", SELGEN_PNL)) < 0)
    return -1;
  if ((phasePNL = LoadPanel (mainPNL, "phase.uir", PHASEPNL)) < 0)
    return -1;
  if ((spgphasePNL = LoadPanel (mainPNL, "spgphase.uir", SPGPHA_PNL)) < 0)
    return -1;
  if ((aesPNL = LoadPanel (mainPNL, "aes.uir", AES_PNL)) < 0)
    return -1;
  if ((anPNL = LoadPanel (mainPNL, "aes.uir", ANALOG_PNL)) < 0)
    return -1;
  if ((anmeasPNL = LoadPanel (mainPNL, "aes.uir", ANMEAS_PNL)) < 0)
    return -1;
  if ((amspPNL = LoadPanel (mainPNL, "sdi.uir", AMSP_PNL)) < 0)
    return -1;
  if ((jitPNL = LoadPanel (mainPNL, "sdi.uir", JIT_PNL)) < 0)
    return -1;
  if ((rlossPNL = LoadPanel (mainPNL, "sdi.uir", RLOSS_PNL)) < 0)
    return -1;
  if ((sdigenlPNL = LoadPanel (mainPNL, "sdi.uir", SDIGEN_PNL)) < 0)
    return -1;
  if ((sdiPNL = LoadPanel (mainPNL, "sdi.uir", SDI_PNL)) < 0)
    return -1;
  if ((pattPNL = LoadPanel (mainPNL, "sdi.uir", PATT_PNL)) < 0)
    return -1;
  if ((configPNL = LoadPanel (mainPNL, "cfg.uir", CONFIG)) < 0)  // config panel
    return -1;
  if ((instrPNL = LoadPanel (mainPNL, "cfg.uir", INSTR_PNL)) < 0)
    return -1;
  if ((vccPNL = LoadPanel (mainPNL, "vcc.uir", VCC_PNL)) < 0)
    return -1;
  if ((ocxoPNL = LoadPanel (mainPNL, "refosc.uir", OCXO_PNL)) < 0)
    return -1;
  if ((cntPNL = LoadPanel (mainPNL, "refosc.uir", COUNT_PNL)) < 0)
    return -1;
  
    
  if ((pPNL = LoadPanel (mainPNL, "presets.uir", P_PNL)) < 0)
    return -1;
  if ((fPNL = LoadPanel (mainPNL, "fconfig.uir", F_PNL)) < 0)
    return -1;
    
  mainmenuhandle = LoadMenuBar (mainPNL, "pt5201.uir", MAINMENU);
  
  
  SetCtrlAttribute (phasePNL, PHASEPNL_COMM, ATTR_VISIBLE, TRUE);    // =TRUE ved debug phase kalibrering
                                 // BB & Analog TPG
  /* jk: START_TESTS button dimmed until config completed */
  SetCtrlAttribute(mainPNL, MAIN_PNL_START_TESTS, ATTR_DIMMED, DISABLED);
  
  SetCtrlVal (mainPNL, MAIN_PNL_SWVER, TESTSWVER);          // Test SW rev
  SetCtrlVal (mainPNL, MAIN_PNL_NAME1, "Vent...");
  SetCtrlVal (mainPNL, MAIN_PNL_NAME2, "");
  DisplayPanel (mainPNL);
 
  // GPS receiver
  SetCtrlAttribute (mainPNL, MAIN_PNL_GPS_TIMER, ATTR_INTERVAL, 50.0); // Timer interval
  GPSCheckNo = 0;
  CheckXLDCRef();


  // Init PC DIO24 interface ----------------------
  outp(IOAddr+3,0x80);    //set mode 
  outp(IOAddr+0,0xFF);    //port A 
  outp(IOAddr+1,0xFF);    //port B 
  outp(IOAddr+2,0xFF);    //port C 
 
  // Init Video Level Meter ----------------------------
  vlm_StartLine = 100;
  vlm_NoOfLines = 100;
  vlm_StartPos = 31.0;
  vlm_Delay = 2.0;
 
  // Remote Control PM5662G/M   (PC DIO24 PortB) ------
  PCDIOPortBStatus = 0xFF;
 

  Err232 = OpenComConfig (DUTCOM, COMname[DUTCOM-1], 9600, 0, 8, 2, 500, 500);

#if AT232
  aterr = OpenComConfig (VCACOM, COMname[VCACOM-1], 19200, 0, 8, 2, 5000, 500);
  if (aterr != 0)
    {
    MessagePopup (" Kan ikke åbne COM5 port", " Afslut program - sluk/tænd PC - start igen");
    return FALSE;
    }

  if (OpenComConfig (PT5210COM, COMname[PT5210COM-1], 9600, 0, 8, 2, 500, 500) != 0)
    {
    MessagePopup (" Kan ikke åbne COM6 port", " Afslut program - sluk/tænd PC - start igen");
    return FALSE;
    }
 
  if (OpenComConfig (PM3094COM, COMname[PM3094COM-1], 9600, 0, 8, 2, 500, 500) != 0)
    {
    MessagePopup (" Kan ikke åbne COM7 port", " Afslut program - sluk/tænd PC - start igen");
    return FALSE;
    }
#endif

  SetCtrlVal (mainPNL, MAIN_PNL_NAME1, ".");

  if (ReadVLMGain() == FALSE)    // file = VLMGAIN.DAT
    return FALSE;

  if (ReadTestSetup() == FALSE)    // file = TSTSETUP.DAT
    return FALSE;

 

  if (GPSStatusOk == FALSE)
    MessagePopup(" TrueTime GPS modtager fejler"," HP53132 må IKKE anvendes til reference kalibrering !!\n Kontakt evt instrument service");
 
 
  //Check if connected to LAN
  // Modified 08.03.01 for DK-Audio net JK
  LogDest = LANLOG;
  i = GetFirstFile ("L:\\PRODUK\\MEASDATA", 1, 0, 0, 0, 0, 1, ff);
  if (i != 0)
    {
    button_pressed = GenericMessagePopup (" Ingen forbindelse til LAN  (l-drev)",
                      " Genstart Windows og husk at logge på LAN med navn = 'PROD_5210'",
                      "Afbryd og genstart Windows","Log til lokal harddisk","", 0, -1, 0,
                      VAL_GENERIC_POPUP_BTN1,
                      VAL_GENERIC_POPUP_BTN1,
                      VAL_GENERIC_POPUP_BTN2);

     if (button_pressed == 1)
       return FALSE;
     if (button_pressed == 2)
       {
       LogDest = LOCALLOG;
       MessagePopup(" Kalibreringsdata"," Kalibreringsdata logges til C-drev\n Husk at overføre data til logfil i 'M:\\MEASDATA\\.....' når LAN igen er ok");
       } 
     } 



  // Check IEEE connection to HP34970 Switch Unit/DMM and HP53132 Counter
  DisableBreakOnLibraryErrors();
  DevClear (0, 22);
  Delay(0.2);

#if IEEEBUS
  DevClear (0, 3);
  Delay(0.2);
  ErrIEEE = hp5313xa_init ("GPIB::3", VI_OFF, VI_ON, 2, &hp53132);
 
   if (ErrIEEE) 
     {
     MessagePopup (" IEEE fejl  (adresse 3)", "Ingen kontakt med counter HP53132\n\nCheck: IEEE-kabel\n            at instrumentet er tændt");
     return FALSE;
     }

  CheckHP53132ExtRef();

 
 
  DisableBreakOnLibraryErrors();
  ErrIEEE = hp34970a_init ("GPIB::9", VI_OFF, VI_ON, &hp34970);
  if (ErrIEEE) 
    {
    MessagePopup (" IEEE fejl  (adresse 9)", "Ingen kontakt med Data Acquisition HP34970\n\nCheck: IEEE-kabel\n            at instrumentet er tændt");
    return FALSE;
    }
  EnableBreakOnLibraryErrors();
#endif 


  SetGenlockSignal(GENL_OFF);

  
  SetCtrlVal (mainPNL, MAIN_PNL_NAME1, "PT5201");
  SetCtrlVal (mainPNL, MAIN_PNL_NAME2, "Compact VariTime Sync Generator");
  /* jk */
  SetCtrlAttribute(mainPNL, MAIN_PNL_START_TESTS, ATTR_DIMMED, ENABLED);
  test_no = 0;
  // Fill List Box with test names :JK
  for (i = 0; i < NR_OF_TESTS; i++)
    {
    aterr = InsertListItem(mainPNL, MAIN_PNL_PROGRESS, i, test_names[i], 0);
    }
  RunUserInterface ();
  return 0;
  }  /* End of main()  */



// Select Black Burst 1-3 calibration & test
void CVICALLBACK BBCallBack (int menuBar, int menuItem, void *callbackData, int panel) 
  {
  switch (menuItem) 
    {
    case MAINMENU_BB_BB1_TOTAL: BBNo = 1; break; // BBNo is global
    case MAINMENU_BB_BB2_TOTAL: BBNo = 2; break;
    case MAINMENU_BB_BB3_TOTAL: BBNo = 3; break;
    }
   
  if (MakeSyncOffsetCal(SYNC_CAL) == TRUE)    // 1,2,3 BB
    if (MakeSyncOffsetCal(OFFSET_CAL) == TRUE)
      if (MakeBBPhaseCal(PAL) == TRUE)
          MakeBBPhaseCal(NTSC);   
  }




void CVICALLBACK SDITestCallBack (int menuBar, int menuItem, void *callbackData,
    int panel) {
int menuSel;                            
  switch (menuItem) {
    case MAINMENU_SDI_JITAMSP: if (SDIJitterTest() == TRUE)
                        SDIAmplSpecTest();break;
                      
    case MAINMENU_SDI_RLOSS:  SDIReturnLossTest(); break;

    case MAINMENU_SDI_EMBAUDIO  : CheckSDIEmbAudio_EDH(); break;
    
  }
}



void CVICALLBACK AnalogTPGCallBack (int menuBar, int menuItem, void *callbackData,
    int panel) {
  switch (menuItem){
    case MAINMENU_COLORBAR_LEVEL:  // Test Signal Generator
         BBNo = 4; 
         TV = PAL;
         if (MakeSyncOffsetCal(SYNC_CAL) == TRUE){
           TV = NTSC;
           MakeSyncOffsetCal(SYNC_CAL);
         }  
         break;

    case MAINMENU_COLORBAR_PHASE:
       if (MakeTSGPhaseCal(PAL) == TRUE)
           MakeTSGPhaseCal(NTSC);
         break;

    case MAINMENU_COLORBAR_PATTERN:
         CheckTPGPattern();
         break;

   }    
}



// menu on mainPNL
void CVICALLBACK SPGTestCallBack (int menuBar, int menuItem, void *callbackData,
    int panel) {
  switch (menuItem) {
    case MAINMENU_SPG_HPHGENL:    MakeSPGHPHCal(); break;
    case MAINMENU_SPG_TESTGENLOCK:TestSPGGenlockSystem(); break;
//    case MAINMENU_SPG_INT27REF:   MakeRefCal(27); break;     // 27.0 MHz X-TAL ikke klar
    case MAINMENU_SPG_INT13REF:   MakeRefCal(13); break;    // 13.0 MHz TCXO
    case MAINMENU_SPG_RESETSPG:   MakeSPGReset(); break;
  } 
}


// menu on mainPNL
void CVICALLBACK AudioTestCallBack (int menuBar, int menuItem, void *callbackData,
    int panel) {
  switch (menuItem) {
    case MAINMENU_AUDIO_AES: TestAESAudioGen();  break;
    case MAINMENU_AUDIO_WCLK:  TestWordClockOutput();  break;
    case MAINMENU_AUDIO_ANCAL: if ((CalAnalogAudioLevel(LEFTCHANNEL)) == TRUE)
                         CalAnalogAudioLevel(RIGHTCHANNEL); break;
    case MAINMENU_AUDIO_ANLEVMEAS : AnalogAudioLevelCheck(); break;
    case MAINMENU_AUDIO_ANFREQMEAS : AnalogAudioFreqCheck(); break;

    
  }
}


// menu on mainPNL
void CVICALLBACK DiverseCallBack (int menuBar, int menuItem, void *callbackData,
  int panel) {
    
  switch (menuItem){
    case MAINMENU_DIVERSE_VLM : RunVLMMeasurement();break;
    case MAINMENU_DIVERSE_AUDIOLEVEL : MeasAnalogAudioLevel();break;
    case MAINMENU_DIVERSE_CALDATA : ReadCalData(); break;           // see config.c
    case MAINMENU_DIVERSE_CALFILES : ShowCalFiles(); break;
    case MAINMENU_DIVERSE_POWERCHECK33 : CheckPowerSupply(33); break;   // se bbcal.c
    case MAINMENU_DIVERSE_POWERCHECK50 : CheckPowerSupply(50); break;   // se bbcal.c
    case MAINMENU_DIVERSE_POWERCHECK50M : CheckPowerSupply(-50); break; // se bbcal.c
    case MAINMENU_DIVERSE_WPON: WriteProtect(ON,TRUE); break;
    case MAINMENU_DIVERSE_WPOFF: WriteProtect(OFF,TRUE); break;
    case MAINMENU_DIVERSE_HP53132: MeasWithHP53132(); break;          // see spgcal.c
    case MAINMENU_DIVERSE_INSTR: ShowInstrumentSetup(); break;        // see cviutil.c
    case MAINMENU_DIVERSE_SELGENLSIGNAL: DisplayPanel(selgenlPNL); break;// see spgcal.c + spggenl.uir
    case MAINMENU_DIVERSE_SYS5201: SetStartupSystem5201();  break;    // see bbcal.c
    case MAINMENU_DIVERSE_EXITPROGRAM:  
                      CloseCom(DUTCOM);
                      #if AT232
                      CloseCom(VCACOM);
                      CloseCom(PM3094COM);
                      CloseCom(PT5210COM);
                      #endif  
                      #if IEEEBUS
                      hp5313xa_close (hp53132);
                      #endif  
                      QuitUserInterface (0);
                      break;
  }
}


#if 0
void CVICALLBACK TotalCallBack (int menuBar, int menuItem, void *callbackData,int panel) {
  Cont = TRUE;
  switch (menuItem){
    case MAINMENU_TOTAL_POWERCHECK33:
    case MAINMENU_TOTAL_POWERCHECK50:
    case MAINMENU_TOTAL_POWERCHECK50M:
    case MAINMENU_TOTAL_RESETSPG:
    case MAINMENU_TOTAL_INT13REF:
    case MAINMENU_TOTAL_HPHGENL:
    case MAINMENU_TOTAL_TESTGENLOCK:
    case MAINMENU_TOTAL_BB1_TOTAL:
    case MAINMENU_TOTAL_BB2_TOTAL:
    case MAINMENU_TOTAL_BB3_TOTAL:
    case MAINMENU_TOTAL_LEVEL:
    case MAINMENU_TOTAL_PHASE:
    case MAINMENU_TOTAL_PATTERN:
    case MAINMENU_TOTAL_JITAMSP:
    case MAINMENU_TOTAL_EMBAUDIO:
    case MAINMENU_TOTAL_ANCAL:
    case MAINMENU_TOTAL_ANLEVMEAS:
    case MAINMENU_TOTAL_ANFREQMEAS:
    case MAINMENU_TOTAL_AES:
    case MAINMENU_TOTAL_WCLK:
                BasicTotalTest(menuItem); break;   // se spgcal.c
  } 
}
#endif

void CVICALLBACK CommandCallback (int menuBar, int menuItem, void *callbackData,
  int panel) {
    
  switch (menuItem){
    case MAINMENU_COMMAND_BB1PAL :  WriteCOMPort(DUTCOM,":OUTP:BB1:SYST PAL;");break;
    case MAINMENU_COMMAND_BB1NTSC : WriteCOMPort(DUTCOM,":OUTP:BB1:SYST NTSC;");break;
    case MAINMENU_COMMAND_BB2PAL :  WriteCOMPort(DUTCOM,":OUTP:BB2:SYST PAL;");break;
    case MAINMENU_COMMAND_BB2NTSC : WriteCOMPort(DUTCOM,":OUTP:BB2:SYST NTSC;");break;
    case MAINMENU_COMMAND_BB3PAL :  WriteCOMPort(DUTCOM,":OUTP:BB3:SYST PAL;");break;
    case MAINMENU_COMMAND_BB3NTSC : WriteCOMPort(DUTCOM,":OUTP:BB3:SYST NTSC;");break;
    case MAINMENU_COMMAND_TSGPAL :  WriteCOMPort(DUTCOM,":OUTP:TSG:SYST PAL;");break;
    case MAINMENU_COMMAND_TSGNTSC : WriteCOMPort(DUTCOM,":OUTP:TSG:SYST NTSC;");break;
    case MAINMENU_COMMAND_GENLPAL : WriteCOMPort(DUTCOM,":INP:GENL:SYST PALB;");break;
    case MAINMENU_COMMAND_GENLNTSC : WriteCOMPort(DUTCOM,":INP:GENL:SYST NTSC;");break;
    case MAINMENU_COMMAND_GENLINT :  WriteCOMPort(DUTCOM,":INP:GENL:SYST INT;");break;
    case MAINMENU_COMMAND_GEN10MHZ : WriteCOMPort(DUTCOM,":INP:GENL:SYST F10MHZ;");break;
    case MAINMENU_COMMAND_AUDAES48 : WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP AES;");
                          WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SIGN S1KHZ;WORD F48KHZ;");break;
    case MAINMENU_COMMAND_AUDAES441 : WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP AES;");
                           WriteCOMPort(DUTCOM,":OUTP:AUD:AES:SIGN S500HZ;WORD F441KHZ;");break;
    case MAINMENU_COMMAND_AUDANA_1K_6DB : WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP ANAL;");
                              WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:SIGN S1KHZ;LEV 6;");break;
    case MAINMENU_COMMAND_AUDANA_500_0DB : WriteCOMPort(DUTCOM,":OUTP:AUD:OUTP ANAL;");
                              WriteCOMPort(DUTCOM,":OUTP:AUD:ANAL:SIGN S500HZ;LEV 0;");break;
  }
} 



// ---------- Ctrl-X on mainPNL ----------------------------------------------------
int CVICALLBACK ExitBtn (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2){ 

  switch (event) {
    case EVENT_COMMIT:
      CloseCom(DUTCOM);
#if AT232
      CloseCom(VCACOM);
      CloseCom(PM3094COM);
      CloseCom(PT5210COM);
#endif  
#if IEEEBUS
      hp5313xa_close (hp53132);
#endif  
#if HP34970
      hp34970a_close (hp34970);
#endif

      QuitUserInterface (0);
      break;
  }
  return 0;
}





// Total Test Button :JK
int CVICALLBACK do_start_tests (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
  {
  int lerr;

  switch (event)
    {
    case EVENT_COMMIT:
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_BB, ATTR_DIMMED, DISABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_COLORBAR, ATTR_DIMMED, DISABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_SDI, ATTR_DIMMED, DISABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_SPG, ATTR_DIMMED, DISABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_AUDIO, ATTR_DIMMED, DISABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_DIVERSE, ATTR_DIMMED, DISABLED);  
      //make_fconfig();
      do_all_tests();
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_BB, ATTR_DIMMED, ENABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_COLORBAR, ATTR_DIMMED, ENABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_SDI, ATTR_DIMMED, ENABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_SPG, ATTR_DIMMED, ENABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_AUDIO, ATTR_DIMMED, ENABLED);
      lerr = SetMenuBarAttribute(mainmenuhandle, MAINMENU_DIVERSE, ATTR_DIMMED, ENABLED);  
      
      break;
    case EVENT_RIGHT_CLICK:
      break;
    }
  return 0;
  }


int do_all_tests(void)
  {
  int lserr;
  int i;
  int checked;

 
  // Find how many are checked and set test_no to first not checked
  // If all are checked (total test has been finished) uncheck all
  test_no = 0;
  for (i = 0; i < NR_OF_TESTS; i++)
    {
    IsListItemChecked(mainPNL, MAIN_PNL_PROGRESS, i, &checked);
    if (checked)
      test_no++;
    }  

  if (test_no == (NR_OF_TESTS))    // if all tests were done
    {
    for (i = 0; i < NR_OF_TESTS; i++)  // uncheck all items in Progress window
      lserr = CheckListItem(mainPNL, MAIN_PNL_PROGRESS, i, UNCHECKED);      
    test_no = 0;
    }
    
  while (test_no < NR_OF_TESTS)
    {
    switch (test_no)
      {
#if 1      
      case 0:
        if (!CheckPowerSupply(33))    
          return FALSE;
        break;
      case 1:
        if (!CheckPowerSupply(50))
          return FALSE;
        break;
      case 2:
        if (!CheckPowerSupply(-50))
          return FALSE;
        break;
      case 3:
        if (!MakeSPGReset())
          return FALSE;
        break;
      case 4:
        if (!MakeRefCal(13))
          return FALSE;
        break;
#endif        
      case 5:
        if (!MakeSPGHPHCal())
          return FALSE;
        break;
      case 6:
        if (!TestSPGGenlockSystem())
          return FALSE;
        break;
      case 7:     // BB1 test
        BBNo = 1; 
        if (!make_bb_test())
          return FALSE;
        break;
      case 8:     // BB2 test
        BBNo = 2; 
        if (!make_bb_test())
          return FALSE;
        break;
      case 9:     // BB3 test
        BBNo = 3; 
        if (!make_bb_test())
          return FALSE;
        break;
      case 10:     // TSG: Luminance Kalibrering
         BBNo = 4; 
         TV = PAL;
         if (MakeSyncOffsetCal(SYNC_CAL))
           {
           TV = NTSC;
           MakeSyncOffsetCal(SYNC_CAL);
           }
         else
           return FALSE;
        break;
      case 11:      // TSG: Timing Kalibrering 
        if (!MakeTSGPhaseCal(PAL))   
          return FALSE;
        if (!MakeTSGPhaseCal(NTSC))    
          return FALSE;
        break;
      case 12:
        if (!CheckTPGPattern())
          return FALSE;
        break;
      case 13:
        if (!SDIJitterTest())  
          return FALSE;
        if (!SDIAmplSpecTest())
          return FALSE;
        break;
      case 14:
        if (!CheckSDIEmbAudio_EDH())
          return FALSE;
        break;
      case 15:
        if (!CalAnalogAudioLevel(LEFTCHANNEL))   // Check this one and next
          return FALSE;                          // rotten here
        if (!CalAnalogAudioLevel(RIGHTCHANNEL))   
          return FALSE;
        break;
      case 16:
        if (!AnalogAudioLevelCheck())
          return FALSE;
        break;
      case 17:
        if (!AnalogAudioFreqCheck())
          return FALSE;
        break;
      case 18:
        if (!TestAESAudioGen())
          return FALSE;
        break;
      case 19:
        if (!TestWordClockOutput())
          return FALSE;
        break;
      case 20:
        if (!make_presets())
          return FALSE;
        break;
      case 21:
        if (!make_fconfig())
          return FALSE;
        break;
      } // switch(test_no)
    lserr = CheckListItem(mainPNL, MAIN_PNL_PROGRESS, test_no, CHECKED);      
    test_no++;
    }   // while (test_no...)
  return TRUE;
  }

int make_bb_test(void)
  {
  if (!MakeSyncOffsetCal(SYNC_CAL))    // 1,2,3 BB   
    return FALSE;
    if (!MakeSyncOffsetCal(OFFSET_CAL))            
      return FALSE;
      if (!MakeBBPhaseCal(PAL))                    
        return FALSE;
        return (MakeBBPhaseCal(NTSC));
  }        


int make_presets(void)
  {
  
  DisplayPanel (pPNL);
  ResetTextBox(pPNL, P_PNL_TEXTBOX,"");
  //SetPanelAttribute (pPNL, ATTR_TITLE, "*Presets*");
  
  SetCtrlVal(pPNL, P_PNL_TEXTBOX," Check af PRESETS knap\n\n");
  SetCtrlVal(pPNL, P_PNL_TEXTBOX," Hold knappen nede i > 15 sekunder\n");
  SetCtrlVal(pPNL, P_PNL_TEXTBOX," - see LEDs tænde parvis\n");
  SetCtrlVal(pPNL, P_PNL_TEXTBOX," - alle LEDs tændt efter 15 sekunder\n");

  WaitForContinue();
 
  if (presets_ok == FALSE)
    {
    HidePanel(pPNL);
    return FALSE;
    }
  
  HidePanel(pPNL);
  return TRUE;
  }

  
int CVICALLBACK Presets_Fail (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
  {
  if (event == EVENT_COMMIT) 
    {
    presets_ok = FALSE; Cont = TRUE;;
    }    
  return 0;
  }

int CVICALLBACK Presets_Ok (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
  {
  if (event == EVENT_COMMIT) 
    {
    presets_ok = TRUE; Cont = TRUE;;
    }    
  return 0;
  }

int CVICALLBACK Fconfig_Fail (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
  {
  if (event == EVENT_COMMIT) 
    {
    fconfig_ok = FALSE; Cont = TRUE;;
    }    
  return 0;
  }

int CVICALLBACK Fconfig_Ok (int panel, int control, int event,
    void *callbackData, int eventData1, int eventData2)
  {
  if (event == EVENT_COMMIT) 
    {
    fconfig_ok = TRUE; Cont = TRUE;;
    }    
  return 0;
  }


void set_standard(int which)
  {
  char KUread[10], response[10];
  
  switch (which)
    {
    case PAL:
      Delay(0.3);
      WriteCOMPort(DUTCOM,":FACT:MAIN:SYST PAL;"); // This makes reset_instrument, very long 
      Delay(1.0);
      WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:COMP 'DK-AUDIO';"); 
      Delay(1.0);
      SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");
      WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:TYPE 'PT5201';"); 
      Delay(0.3);
      SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");
      break;
    case NTSC:
      Delay(0.3);
      WriteCOMPort(DUTCOM,":FACT:MAIN:SYST NTSC;"); // This makes reset_instrument, very long   
      Delay(1.0);
      WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:COMP 'DK-AUDIO';"); 
      Delay(1.0);
      SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");
      WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:TYPE 'PT5201';"); 
      Delay(0.3);
      SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");
      break;
    case NTSCJP:
      WriteCOMPort(DUTCOM,":FACT:MAIN:SYST JNTSC\n");
      WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:COMP 'LEADER';"); 
      WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:TYPE 'LT428';"); 
      WriteCOMPort(DUTCOM,":FACT:MAIN:IDN:SWR '010';"); 
      
      EnterSerialNo("LT428: ", "", response);
      Fmt(KUStr,"%s<%s",response);
      Fmt (DUTStr, ":FACT:MAIN:IDN:KUN '%s';",KUStr);
      WriteCOMPort(DUTCOM, DUTStr);
      Delay(0.03);
      break;
    }

  SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");

    
  // Now presets: set all to something meaningful
  Delay(0.3);
  WriteCOMPort(DUTCOM,":SYST:PRES:STOR 4;NAME 4,'ABC';AUTH 4,\"ABC\";DATE 4,00,00,00;"); 
  SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");
  Delay(0.3);
  WriteCOMPort(DUTCOM,":SYST:PRES:STOR 3;NAME 3,'ABC';AUTH 3,\"ABC\";DATE 3,00,00,00;"); 
  SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");
  Delay(0.3);
  WriteCOMPort(DUTCOM,":SYST:PRES:STOR 2;NAME 2,'ABC';AUTH 2,\"ABC\";DATE 2,00,00,00;"); 
  Delay(0.3);
  SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");
  WriteCOMPort(DUTCOM,":SYST:PRES:STOR 1;NAME 1,'ABC';AUTH 1,\"ABC\";DATE 1,00,00,00;"); 

  Delay(3.0);
  SetCtrlVal(fPNL, F_PNL_TEXTBOX," *");
  WriteCOMPort(DUTCOM,"*RST\n");
  }

int make_fconfig(void)
  {
  int stat;
  int value;
  char version_buffer[10];
  
  DisplayPanel (fPNL);
  ResetTextBox(fPNL, F_PNL_TEXTBOX,"Vælg standard ved opstarten:\n\n");

/* Set sofware version from PT5201RevStr into F_PNL_TEXTBOX_SWR */
  strcpy(version_buffer, PT5201RevStr);
  ResetTextBox(fPNL, F_PNL_TEXTBOX_SWR, version_buffer);

  if (!WriteProtect(OFF, FALSE))
    {
    MessagePopup(" RS-232 remote"," FEJL:  Check RS-232 kabel, og at apparatet er tændt");
    HidePanel(fPNL);
    return FALSE;
    }

  WaitForContinue();
 
  if (fconfig_ok == FALSE)
    {
    HidePanel(fPNL);
    return FALSE;
    }
    
/* Get System settings from switch */    
  stat = GetCtrlVal (fPNL, F_PNL_RS, &value);

/* Get value of software version from TEXTBOX_SWR, possibly edited by user. */
  GetTextBoxLine(fPNL, F_PNL_TEXTBOX_SWR, 0, version_buffer);

  Fmt (DUTStr, ":FACT:MAIN:IDN:SWR '%s';",version_buffer);
  WriteCOMPort(DUTCOM, DUTStr);
  Delay(0.5);

  SetCtrlVal(fPNL, F_PNL_TEXTBOX,"...Vent..  ");
  //SetCtrlVal(fPNL, F_PNL_TEXTBOX,"\n");

  set_standard(value);

/* Perform manual check of all buttons on front plate and check that display
   reacts properly

  ResetTextBox(fPNL, F_PNL_TEXTBOX,"Tryk alle short-cut tasterne og kontroller\n");
  SetCtrlVal(fPNL, F_PNL_TEXTBOX,"at displayet viser rigtige menuer.\n");
  WaitForContinue();


  ResetTextBox(fPNL, F_PNL_TEXTBOX,"Kontroller at pil tasterne virker.\n");
  WaitForContinue();

  ResetTextBox(fPNL, F_PNL_TEXTBOX,"Vælg PRESETS menu og udfør RECALL1.\n");
  SetCtrlVal(fPNL, F_PNL_TEXTBOX,  "I STATUS menu, kontroller at PRESET1 er aktiv.\n");
  WaitForContinue();
*/

  HidePanel(fPNL);
  return TRUE;
  }

void EnterSerialNo(char text[50], char *kuread, char *kustr)
  {
  char prompttext[100], response[10];
  int validku;

  Fmt(prompttext,"%s<%s:   Indtast seriel nummer ",text);
#if 1
    PromptPopup (prompttext, kuread, response, 8);
    if ((strlen(response) == 0) && (strlen(kuread) == 8))  // Only ENTER pressed
      CopyString(response,0,kuread,0,-1);
    validku = CheckSerialNo(response);
  CopyString(kustr,0,response,0,-1);
#endif  
  
  } 


int CheckSerialNo(char *kustr)
  {
  int n, validku;
 
  validku = TRUE;
 
  if (strlen(kustr) != 8)        // 8 digits
    return FALSE;
#if 1 
  for (n = 0; n < 8; n++)
    if (isdigit(kustr[n]) == FALSE) // digits only
      return FALSE;
#endif      
  return validku;
  } 
