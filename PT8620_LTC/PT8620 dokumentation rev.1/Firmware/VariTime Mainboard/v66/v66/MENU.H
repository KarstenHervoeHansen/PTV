/***************************************************************************/
/*  Copyright   ProTeleVision Technologies A/S, BRONDBY 1998               */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:     MENU.H                                                     */
/*  Author:     Kim Engedahl, DEV                                          */
/*  Org. date:  980115                                                     */
/*  Rev. date:  000508, KEn, DEV                                           */
/*  Status:     Version 1.2                                                */
/*                                                                         */
/*  Changes:                                                               */
/* 000508: Support for positioning in TPG text                             */
/* 980825: Status??Update changed to Status??Show                          */
/* 980616: Released as 4008 002 06812                                      */
/* 980514: Released as 4008 002 06811                                      */
/***************************************************************************/




#define T_STEP_ZOOM   (FuncTriggers & 0xC0)
#define T_STEP_ZOOM_L (FuncTriggers & 0x40)
#define T_STEP_ZOOM_R (FuncTriggers & 0x80)
#define T_OR_H_OR_V   (FuncTriggers & 0x3C) /* changed from 0x3F, following elimination of V */
#define T_SEL         (FuncTriggers & 0x30)
#define H_SEL         (FuncTriggers & 0x0C)
#define V_SEL         (FuncTriggers & 0x03)
#define T_UP          (FuncTriggers & 0x10)
#define H_UP          (FuncTriggers & 0x04)
#define V_UP          (FuncTriggers & 0x01)

#define stepNdxTLGMax    4
#define stepNdxTLGMin    0

#define MaxHDAttribs     3

typedef struct uu
  {
  long   samples_per_line;
  long   lines_per_frame;
  UC   sys_clk;
  UC   scan;
  UC   sync;
  long def_del;
  long fra_del;
  UC   format_id; /* for indirect indexing of formats in board uP */
                  /* made for compatibilty when adding or         */
                  /* removing formats                             */
  long nadjust;
  }  HDPARAMS;



extern code HDPARAMS  dltpg_fmt_table[];
extern code HDPARAMS  hdtpg_fmt_table[];
extern code HDPARAMS  tlg_fmt_table[];

extern near long stepVal;
extern UC        stepNdx;
extern code long stepsizeTLGTable[2][5];
extern code UC   HDAttribValues[MaxHDAttribs];

extern near long tlg_samples;


// Ndx indicates the last active item in menu:

extern near UC   StatusNdx;              // Display item WriteTimingFLTTLG
extern near UC   BaseNdx;                // Base-menu item
extern      char RAMBaseNdx;             // index to RAM based base-menu item
extern      char FirstItem;

extern near UC AnlBlkNdx;                // Black-burst submenu item
extern near UC BBNdx[MaxAnlBlkUnits];    // Black-burst 2ndmenu item

extern near UC DLTPGNoNdx;                     //     -"-     DLTPG sub menu item
extern near UC DLTPGsubmenuNdx[MaxDLTPGUnits]; //     -"-     DLTPG submenu item

extern near UC DLTPGTextMenuNdx[MaxDLTPGUnits];  //     -"-     DLTPG text 2ndmenu item
extern near UC DLTPGAudioMenuNdx[MaxDLTPGUnits]; //     -"-     DLTPG audio 2ndmenu item

extern near UC HDTPGNoNdx;                     //     -"-     HDTPG sub menu item
extern near UC HDTPGsubmenuNdx[MaxHDTPGUnits]; //     -"-     HDTPG submenu item

extern near UC HDTPGTextMenuNdx[MaxHDTPGUnits];  //     -"-     HDTPG text 2ndmenu item
extern near UC HDTPGAudioMenuNdx[MaxHDTPGUnits]; //     -"-     HDTPG audio 2ndmenu item

extern near UC TLGNdx[MaxTLGUnits];              //     -"-     tlg menu item
extern near UC TLGNoNdx;                         //     -"-     Tlg sub menu item

extern near UC AnlTPGNdx[MaxAnlTPGUnits];       // Analog-TPG submenu item

extern near UC SDITSGNdx[MaxSDITSGUnits];       // SDI-TSG submenu item
extern near UC SDITSGAudioNdx[MaxSDITSGUnits];  // SDI-TSG audio 2ndmenu item

extern near UC SDITPGNdx[MaxSDITPGUnits];      // SDI-TPG submenu item
extern near UC SDITPGTextNdx[MaxSDITPGUnits];  // SDI-TPG text 2ndmenu item
extern near UC SDITPGAudioNdx[MaxSDITPGUnits]; // SDI-TPG audio 2ndmenu item

extern near UC AESEBUnNdx;                     // AES-EBU submenu item
extern near UC AESEBUNdx[MaxAESEBUUnits];      // AES-EBU 2ndmenu item

extern near UC PresetNdx;                  // Preset submenu item
extern near UC ConfigNdx;                  // Config submenu item
extern near UC DateTimeNdx;              // Date/Time 2ndmenu item
extern near UC RS232Ndx;                 // RS232 2ndmenu item
extern near UC DownloadNdx;                // Download select item
extern near UC DiagnoseNdx;               // Diagnose select item
extern near UC LTCNdx;          // LTC select item

extern UL ResponseKUNo;                  // TBD
extern UC ResponseSWVersion;              // TBD

/***************** PT8603 *******************/
extern void PT8603SDITPGTextDown();
extern void PT8603TextEditSelect();
extern void PT8603TextOnOffSelect();
extern void PT8603TextMovementSelect();
extern void PT8603TextPosSelect();
/***************** PT8603 *******************/


extern void BaseItemLeft();
extern void BaseItemRight();
void ToBase();

extern UC FindSDIUnit(UC no);

extern void GroupButtonSelect( UC group);
extern void PresetButtonSelect();
extern void OutputButtonSelect();
void GenlockButtonSelect();

extern void StatusPresetShow();
extern void StatusGenlockShow();
extern void StatusAnlTPGShow();
extern void StatusSDITSGShow();
extern void StatusSDITPGShow();
extern void StatusAESEBUShow();
extern void StatusDateTimeShow();
extern void StatusTimeShow();

extern void StatusErrorShow();
extern void StatusDown();
extern void UpToStatus();

extern void BaseMenuShow();
extern void BaseDown();
extern void UpToBase();

extern void AnlBlkMenuShow();
extern void AnlBlkDown();
extern void UpToAnlBlk();

extern void BBMenuShow();
extern void BBDown();
extern void BBSystemSelect();
extern void BBTimingSelect();
extern void BBScHPhaseSelect();

extern void AnlTPGMenuShow();
extern void AnlTPGDown();
extern void AnlTPGPatternSelect();
extern void AnlTPGPatternModify();
extern void AnlTPGSystemSelect();
extern void AnlTPGTimingSelect();
extern void AnlTPGScHPhaseSelect();

extern void UpToAnlTPG();

extern void AnlTPGTextMenuShow();
extern void AnlTPGTextDown();
extern void AnlTPGTextEditSelect();
extern void AnlTPGTextStyleSelect();
extern void AnlTPGTextPosSelect();
extern void AnlTPGTextClockSelect();

extern void SDITSGMenuShow();
extern void SDITSGDown();
extern void SDITSGPatternSelect();
extern void SDITSGSystemSelect();
extern void SDITSGEDHSelect();
extern void SDITSGTimingSelect();

extern void SDITSGAudioMenuShow();
extern void SDITSGAudioDown();
extern void UpToSDITSG();
extern void SDITSGAudioSignalSelect();
extern void SDITSGAudioLevelSelect();

extern void SDITPGMenuShow();
extern void SDITPGDown();
extern void SDITPGPatternSelect();
extern void SDITPGPatternModify();
extern void SDITPGSystemSelect();
extern void SDITPGEDHSelect();
extern void SDITPGTimingSelect();

extern void UpToSDITPG();

extern void SDITPGTextMenuShow();
extern void SDITPGTextDown();
extern void SDITPGTextEditSelect();
extern void SDITPGTextStyleSelect();
extern void SDITPGTextPosSelect();
extern void SDITPGTextClockSelect();

extern void SDITPGAudioMenuShow();
extern void SDITPGAudioDown();
extern void SDITPGAudioGroupSelect();
extern void SDITPGAudioSignalSelect();
extern void SDITPGAudioLevelSelect();

extern void AESEBUnMenuShow();
extern void AESEBUnDown();
extern void UpToAESEBUn();

extern void AESEBUMenuShow();
extern void AESEBUDown();
extern void AESEBUSignalSelect();
extern void AESEBULevelSelect();
extern void AESEBUTimingSelect();

extern void GenlockInputShow();
extern void GenlockInputSelect();
extern void GenlockSystemSelect();
extern void GenlockTimingSelect();

extern void LTCMenuShow();
extern void LTCSelect();

extern void LTCOffsetSet();
extern void LTCFormatSet();
extern void LTCTimeSet();
extern void LTCModeSet();
extern void LTCTimezoneSet();
extern void LTCDaylightSet();
extern void LTCStartSet();
extern void LTCEndSet();
extern void LTCDaylightConfirm();
extern void LTCSyncConfirm();
extern void LTCMessageMenu();

extern void LTC8620Select();
extern void LTC8620TimeSet();
extern void UpToLTC8620();
extern void LTC8620Sync();
extern void LTC8620SyncConfirm();

extern void NTPMenuShow();
extern void NTPSelectMenu();
extern void NTPETHERConfigMenu();
extern void NTPConfigMenu();
extern void NTPAddrModifyMenuHelper(char addrnum);
extern void NTPAddrModifyMenu();
extern void NTPSaveAddr();

extern void PresetMenuShow();
extern void PresetDown();
extern void PresetRecallSelect();
extern void PresetStoreSelect();
extern void PresetNameSelect();

extern void ConfigMenuShow();
extern void ConfigDown();
extern void UpToConfig();
extern void ConfigLockSelect();
extern void ConfigAutoESCSelect();
extern void ConfigLCDContSelect();

extern void DateTimeMenuShow();
extern void DateTimeDown();
extern void DateTimeDateSelect();
extern void DateTimeTimeSelect();
extern void DateTimeRefSelect();
extern void DateTimeOffsetSelect();

extern void TimeDown();
extern void TimeMenuShow();

extern void TimeDown();
extern void TimeMenuShow();

extern  void RTCTimeSelect();
extern  void LTC8620RefSelect();
extern void DownloadTypeSelect();
extern void UpToDownload();
extern void DownloadTestSPG();
extern void DownloadFromSPG();

extern void RS232MenuShow();
extern void RS232Down();
extern void RS232BaudrateSelect();
extern void RS232DatabitSelect();
extern void RS232ParitySelect();
extern void RS232HandshakeSelect();

extern void DiagnoseMenuShow();
extern void DiagnoseDown();

extern void DiagnoseMainTest();
extern void DiagnoseOptionsTest();
extern void DiagnoseRS232Test();
extern void DiagnoseMemoryTest();
extern void DiagnoseDisplayTest();
extern void DiagnoseKeyboardTest();
extern void DiagnoseConfigTest();
extern void DiagnoseErrorQueueTest();
extern void DiagnoseErrorQueueReset();

extern void AnlTimeAdjust( UC unit, UC System, long Delay);
extern void SDITimeAdjust( UC unit, UC System, long Delay);

/**********  Protos of HDTPG functions  **************/
extern void HDTPGMenuShow();
extern void HDTPGDown();
extern void HDTPGSubMenuShow();
extern void HDTPGSubDown();
extern void UpToHDTPG();
extern void UpToHDTPGsub();

extern void HDTPGPatternSelect();
extern void HDTPGPatternModify();

extern void HDTPGSystemSelect();
extern void HDTPGEDHSelect();
extern void HDTPGTextMenuShow();
extern void UpToHDTPG();
extern void HDTPGTextDown();
extern void HDTPGTextEditSelect();
extern void HDTPGTextScaleSelect();
extern void HDTPGTextPosSelect();
extern void HDTPGTextClockSelect();
extern void HDTPGAudioMenuShow();
extern void HDTPGAudioDown();
extern void HDTPGAudioSignalSelect();
extern void HDTPGAudioLevelSelect();
extern void HDTPGAudioGroupSelect();
extern void HDTPGAudioTimingSelect();
extern void HDTPGAudioClickSelect();
extern void HDTPGAudioItemRight();
extern void HDTPGTimingSelect();
extern void HDTPGTextColorSelect();
extern void HDTPGTextBackgroundSelect();
extern void HDTPGTextMovementSelect();

/************************/

/**********  Protos of DLTPG functions  **************/
extern void DLTPGMenuShow();
extern void DLTPGDown();
extern void DLTPGSubMenuShow();
extern void DLTPGSubDown();
extern void UpToDLTPG();
extern void UpToDLTPGsub();

extern void DLTPGPatternSelect();
extern void DLTPGPatternModify();

extern void DLTPGSystemSelect();
extern void DLTPGEDHSelect();
extern void DLTPGTextMenuShow();
extern void UpToDLTPG();
extern void DLTPGTextDown();
extern void DLTPGTextEditSelect();
extern void DLTPGTextScaleSelect();
extern void DLTPGTextPosSelect();
extern void DLTPGTextClockSelect();
extern void DLTPGAudioMenuShow();
extern void DLTPGAudioDown();
extern void DLTPGAudioSignalSelect();
extern void DLTPGAudioLevelSelect();
extern void DLTPGAudioGroupSelect();
extern void DLTPGAudioTimingSelect();
extern void DLTPGAudioClickSelect();
extern void DLTPGAudioItemRight();
extern void DLTPGTimingSelect();
extern void DLTPGTextColorSelect();
extern void DLTPGTextBackgroundSelect();
extern void DLTPGTextMovementSelect();
extern void DLTPGInterfaceModify();
extern UC   FindHDInterface(UC Triggers, UC system, UC interface);

/**********  Protos of DLTPG functions  **************/

/**********  Protos of TLG functions  **************/
extern void TLGMenuShow();
extern void TLGSubMenuShow();
extern void TLGDown();
extern void UpToTLG();
extern void TLGSubDown();
extern void TLGFormatSelect();
extern void TLGTimingSelect();
extern void TLGTimeAdjust(UC item, UC system, long delay, UC which);

/************************/

extern UC FindHDAttribute(UC, UC, UC);
extern void SetLTCTime();
extern UL GetTimeLong();
extern char * GetTimeCode();
extern void SetRTCTimeLong(UL TB);
extern UL UC2bcd(UC ival);