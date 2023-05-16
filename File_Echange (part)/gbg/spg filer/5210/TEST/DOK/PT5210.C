#include "hp34401a.h"
#include "hp5313xa.h"
#include <ansi_c.h>
#include <gpib.h>
#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <cvirte.h>		/* Needed if linking in external compiler; harmless otherwise */
#include <userint.h>

// header files for uir files
#include "pt5210.h"
#include "bb_sync.h"
#include "phase.h"
#include "sdi.h"
#include "vlm.h"
#include "cfg.h"
#include "spghph.h"
#include "spggenl.h"
#include "spgphase.h"
#include "aes.h"
#include "vcc.h"
#include "fan.h"
#include "ltc.h"


// general utilities
#include "cviutil.h"

// calibration utilities
#include "vlmutil.h"
#include "bbcal.h"
#include "spgcal.h"
#include "sditest.h"
#include "aescal.h"
#include "ltctest.h"


#define IEEEBUS 1		//IEEE interface 
#define AT232 1			//RS232 interface (com5,6,7)

#define TESTSWVER  "Ver. 970307        7108 100 74471"



int sditotMENU, basictotMENU;




int main (int argc, char *argv[])
{
int ConfirmReply;
ViInt16 InputTermHP34401;
int sm,sg;


PasswordUsed = TRUE;

if (argc > 1)
 if (strcmp(argv[1],"nopass") == 0)
   PasswordUsed = FALSE;
  else
   PasswordUsed = TRUE;


#if IEEEBUS
    IEEEboard = ibfind ("gpib0");
	devicePM5640G = ibdev (0, 16, NO_SAD, T3s, 1, 0);
	sg = iberr;
	devicePM5640M = ibdev (0, 17, NO_SAD, T3s, 1, 0);
	sm = iberr;
#endif
	

	
	if (InitCVIRTE (0, argv, 0) == 0)	/* Needed if linking in external compiler; harmless otherwise */
		return -1;	/* out of memory */
	if ((mainPNL = LoadPanel (0, "pt5210.uir", MAIN_PNL)) < 0)
		return -1;
	if ((sditotPNL = LoadPanel (mainPNL, "pt5210.uir", SDITOT_PNL)) < 0)
		return -1;
	if ((syncPNL = LoadPanel (mainPNL, "bb_sync.uir", SYNC_PNL)) < 0)
		return -1;
	if ((vlmPNL = LoadPanel (mainPNL, "vlm.uir", VLM_PNL)) < 0)
		return -1;
	if ((spghphPNL = LoadPanel (mainPNL, "spghph.uir", SPGHPH_PNL)) < 0)
		return -1;
	if ((spggenlPNL = LoadPanel (mainPNL, "spggenl.uir", SPGGEN_PNL)) < 0)
		return -1;
	if ((phasePNL = LoadPanel (mainPNL, "phase.uir", SCH_PNL)) < 0)
		return -1;
	if ((spgphasePNL = LoadPanel (mainPNL, "spgphase.uir", SPGPHA_PNL)) < 0)
		return -1;
	if ((aesPNL = LoadPanel (mainPNL, "aes.uir", AES_PNL)) < 0)
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
	if ((sdipattPNL = LoadPanel (mainPNL, "sdi.uir", SDIPAT_PNL)) < 0)
		return -1;
	if ((cfgPNL = LoadPanel (mainPNL, "cfg.uir", CFG_PNL)) < 0)
		return -1;
	if ((dataPNL = LoadPanel (mainPNL, "cfg.uir", DATA_PNL)) < 0)
		return -1;
	if ((vccPNL = LoadPanel (mainPNL, "vcc.uir", VCC_PNL)) < 0)
		return -1;
	if ((fanPNL = LoadPanel (mainPNL, "fan.uir", FAN_PNL)) < 0)
		return -1;
	if ((ocxoPNL = LoadPanel (mainPNL, "ocxo.uir", FAN_PNL)) < 0)
		return -1;
	if ((ltcPNL = LoadPanel (mainPNL, "ltc.uir", LTC_PNL)) < 0)
		return -1;
		

 if ((sditotPNL = LoadPanel (mainPNL, "pt5210.uir", SDITOT_PNL)) < 0)
	return -1;
 sditotMENU = LoadMenuBar (sditotPNL, "pt5210.uir", SDIMENU);
 SetPanelMenuBar (sditotPNL, sditotMENU);

 if ((basictotPNL = LoadPanel (mainPNL, "pt5210.uir", BASTOT_PNL)) < 0)
	return -1;
// basictotMENU = LoadMenuBar (basictotPNL, "pt5210.uir", BASIC);

 SetPanelAttribute (sditotPNL, ATTR_FRAME_COLOR, VAL_OFFWHITE);
 SetCtrlAttribute (mainPNL, MAIN_PNL_TOTBASIC_BTN, ATTR_VISIBLE, FALSE);
		


 SetCtrlVal (mainPNL, MAIN_PNL_SWVER, TESTSWVER);
 
 outp(IOAddr+3,0x80);	   //init PC DIO24 interface
 outp(IOAddr+0,0xFF);	   //port A = FF
 outp(IOAddr+1,0xFF);	   //port B = FF
 outp(IOAddr+2,0xFF);	   //port C = FF
 
 vlm_StartLine = 100;
 vlm_NoOfLines = 100;
 vlm_StartPos = 31.0;
 vlm_Delay = 2.0;
 
 PM5662Status = 0xFF;
 

 Err232 = OpenComConfig (DUTCOM, COMname[DUTCOM-1], 9600, 0, 8, 2, 500, 500);
#if AT232
 Err232 = OpenComConfig (VCACOM, COMname[VCACOM-1], 19200, 0, 8, 2, 5000, 500);
 Err232 = OpenComConfig (PT5210COM, COMname[PT5210COM-1], 9600, 0, 8, 2, 500, 500);
 Err232 = OpenComConfig (PM3094COM, COMname[PM3094COM-1], 9600, 0, 8, 2, 500, 500);
#endif
 DisplayPanel (mainPNL);

 DisableBreakOnLibraryErrors();
 DevClear (0, 22);
 ErrIEEE = hp34401a_init ("GPIB::22", VI_OFF, VI_ON, &hp34401);
#if IEEEBUS
	if (ErrIEEE) {
	 MessagePopup (" IEEE fejl  (adresse 22)", "Ingen kontakt med multimeter HP34401\n\nCheck: IEEE-kabel\n            at instrumentet er tændt");
	 return FALSE;
	}
 ErrIEEE = hp34401a_conf (hp34401, 1, VI_ON, 10, 1);
#endif


 ErrIEEE = hp5313xa_init ("GPIB::3", VI_OFF, VI_ON, 2, &hp53132);
 
#if IEEEBUS
	if (ErrIEEE) {
	 MessagePopup (" IEEE fejl  (adresse 3)", "Ingen kontakt med counter HP53132\n\nCheck: IEEE-kabel\n            at instrumentet er tændt");
	 return FALSE;
	}

#endif


 EnableBreakOnLibraryErrors();
	

 if (Set5640Standard("G") != 0)
  return;
 if (Set5640Standard("M") != 0)
  return;
	
	
	RunUserInterface ();
	return 0;
}


void CVICALLBACK BBCallBack (int menuBar, int menuItem, void *callbackData,
		int panel) {
int BBNo;		
	switch (menuItem) {
	 case MAINMENU_BB_BB1_TOTAL: BBNo = 1; break;
	 case MAINMENU_BB_BB2_TOTAL: BBNo = 2; break;
	 case MAINMENU_BB_BB3_TOTAL: BBNo = 3; break;
	 case MAINMENU_BB_BB4_TOTAL: BBNo = 4; break;
	 case MAINMENU_BB_BB5_TOTAL: BBNo = 5; break;
	 case MAINMENU_BB_BB6_TOTAL: BBNo = 6; break;
	 case MAINMENU_BB_BB7_TOTAL: BBNo = 7; break;
	 case MAINMENU_BB_BB8_TOTAL: BBNo = 8; break;
	}
		
	if (MakeSyncOffsetCal(BBNo,SYNC_CAL) == TRUE)
	 if (MakeSyncOffsetCal(BBNo,OFFSET_CAL) == TRUE)
	  if (MakePhaseCal(BBNo,PAL) == TRUE)
	   MakePhaseCal(BBNo,NTSC);
		
}




void CVICALLBACK SDITestCallBack (int menuBar, int menuItem, void *callbackData,
		int panel) {
int menuSel;													  
	switch (menuItem) {

 		case MAINMENU_SDI_BLK34_TOTAL : 
 		case MAINMENU_SDI_BLK56_TOTAL :
 		case MAINMENU_SDI_BLK78_TOTAL :
 		case MAINMENU_SDI_TSG_TOTAL :
 										DisplayPanel(sditotPNL);
 										SetPanelAttribute (sditotPNL, ATTR_TITLE, " SDI Kalibrering ");
										menuSel = RunPopupMenu (sditotMENU, SDIMENU, sditotPNL, 50, 110, 0, 0, 0, 0);
 										HidePanel(sditotPNL);
 										SDITotalTest(menuItem,menuSel);
 										break;


		case MAINMENU_SDI_BLK34_PHASECAL : if (SDICal(L625,34) == TRUE)
										 SDICal(L525,34); break;
		case MAINMENU_SDI_BLK56_PHASECAL : if (SDICal(L625,56) == TRUE)
										 SDICal(L525,56); break;
		case MAINMENU_SDI_BLK78_PHASECAL : if (SDICal(L625,78) == TRUE)
										 SDICal(L525,78); break;
		case MAINMENU_SDI_TSG_PHASECAL : if (SDICal(L625,1) == TRUE)
										 SDICal(L525,1); break;
										 
		case MAINMENU_SDI_BLK34_PATT:    CheckSDIPattern(34); break;
		case MAINMENU_SDI_BLK56_PATT:    CheckSDIPattern(56); break;
		case MAINMENU_SDI_BLK78_PATT:    CheckSDIPattern(78); break;
		case MAINMENU_SDI_TSG_PATT:      CheckSDIPattern(1); break;
		
		
		case MAINMENU_SDI_BLK34_JITAMSP: if (SDIJitterTest(34) == TRUE)
										  SDIAmplSpecTest(34);break;
		case MAINMENU_SDI_BLK56_JITAMSP: if (SDIJitterTest(56) == TRUE)
										  SDIAmplSpecTest(56);break;
		case MAINMENU_SDI_BLK78_JITAMSP: if (SDIJitterTest(78) == TRUE)
										  SDIAmplSpecTest(78);break;
		case MAINMENU_SDI_TSG_JITAMSP: if (SDIJitterTest(1) == TRUE)
										  SDIAmplSpecTest(1);break;
										  
		case MAINMENU_SDI_BLK34_RLOSS: SDIReturnLossTest(34); break;
		case MAINMENU_SDI_BLK56_RLOSS: SDIReturnLossTest(56); break;
		case MAINMENU_SDI_BLK78_RLOSS: SDIReturnLossTest(78); break;
		case MAINMENU_SDI_TSG_RLOSS:   SDIReturnLossTest(1); break;

		case MAINMENU_SDI_BLK34_EMBAUDIO : CheckSDIEmbAudio_EDH(34); break;
		case MAINMENU_SDI_BLK56_EMBAUDIO : CheckSDIEmbAudio_EDH(56); break;
		case MAINMENU_SDI_BLK78_EMBAUDIO : CheckSDIEmbAudio_EDH(78); break;
		case MAINMENU_SDI_TSG_EMBAUDIO   : CheckSDIEmbAudio_EDH(1); break;
		
		case MAINMENU_SDI_GENL:		   SDIGenlockTest(); break;
	}
}




int CVICALLBACK ExitBtn (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{   // Ctrl-X
	switch (event) {
		case EVENT_COMMIT:
			CloseCom(DUTCOM);
#if AT232
			CloseCom(VCACOM);
			CloseCom(PM3094COM);
			CloseCom(PT5210COM);
#endif	
#if IEEEBUS
			hp34401a_close (hp34401);
#endif	
			QuitUserInterface (0);
			break;
	}
	return 0;
}


void CVICALLBACK AfslutMainCallBack (int menuBar, int menuItem, void *callbackData,
		int panel) {
	CloseCom(DUTCOM);
#if AT232
	CloseCom(VCACOM);
	CloseCom(PM3094COM);
	CloseCom(PT5210COM);
#endif	
#if IEEEBUS
	hp34401a_close (hp34401);
	//hp5313xa_close (hp53132);
#endif	
	QuitUserInterface (0);
}



void CVICALLBACK SPGTestCallBack (int menuBar, int menuItem, void *callbackData,
		int panel) {
	switch (menuItem) {
		case MAINMENU_SPG_HPHGENL:		if (MakeSPGHPHCal() == TRUE)
										if (MakeSPGGenlockCal(PAL) == TRUE)
										 MakeSPGGenlockCal(NTSC); break;
		case MAINMENU_SPG_TESTGENLOCK: 	if (MakeSPGGenlockInput() == TRUE)
										 MakeSPGGenlockSystem(); break;
		case MAINMENU_SPG_CHECKOVEN:	CheckOven(); break;
		case MAINMENU_SPG_INT27REF: 	MakeOCXOCal(27); break;
		case MAINMENU_SPG_INT10REF: 	MakeOCXOCal(10); break;
		case MAINMENU_SPG_RESETSPG: 	MakeSPGReset(); break;
	}	
}


void CVICALLBACK AudioTestCallBack (int menuBar, int menuItem, void *callbackData,
		int panel) {
	switch (menuItem) {
		case MAINMENU_AUDIOGEN: TestAudioGen();  break;
		
	}
}



void CVICALLBACK DiverseCallBack (int menuBar, int menuItem, void *callbackData,
		int panel) {
		
		switch (menuItem){
		case MAINMENU_DIVERSE_VLM : RunVLMMeasurements();break;
		case MAINMENU_DIVERSE_CALDATA : ReadCalData(); break;
		case MAINMENU_DIVERSE_CALFILES : ShowCalFiles(); break;
		case MAINMENU_DIVERSE_POWER_V5 :   CheckPowerSupply(5); break;
		case MAINMENU_DIVERSE_POWER_V12 :  CheckPowerSupply(12); break;
		case MAINMENU_DIVERSE_POWER_VM5 :  CheckPowerSupply(-5); break;
		case MAINMENU_DIVERSE_POWER_TOTAL: if (CheckPowerSupply(5) == TRUE) 
										   if (CheckPowerSupply(12) == TRUE) 
										    CheckPowerSupply(-5);
											break;
		case MAINMENU_DIVERSE_FAN : CheckFan();break;
		case MAINMENU_DIVERSE_WPON : WriteProtect(ON,TRUE); break;
		case MAINMENU_DIVERSE_WPOFF : WriteProtect(OFF,TRUE); break;
		case MAINMENU_DIVERSE_PM5640SCH : ReadPM5640ScHCalData(); break;
		}
}


void CVICALLBACK LTCCallBack (int menuBar, int menuItem, void *callbackData,
		int panel) {

		switch (menuItem){
		 case MAINMENU_LTC: LTC_TotalTest(); break;

		}
		
}

int CVICALLBACK TotalBasicCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
int menuSel;
	switch (event) {
		case EVENT_COMMIT:
							DisplayPanel(basictotPNL);
							SetPanelAttribute (basictotPNL, ATTR_TITLE, " Totaltest grundapparat ");
//							menuSel = RunPopupMenu (basictotMENU, BASIC, basictotPNL, 50, 110, 0, 0, 0, 0);
							HidePanel(basictotPNL);
							BasicTotalTest(menuSel);


			break;
	}
	return 0;
}
