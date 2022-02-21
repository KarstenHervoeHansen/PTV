/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:    MENUTREE.H                                                  */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  980115, file is based on PT5210/MENUTREE.H                 */
/*  Rev. date:  000508,  KEn, DEV                                          */
/*  Status:    Version 1.1                                                 */
/*                                                                         */
/*  Changes:                                                               */
/* 000508: Added an item to TPGTextMax  and enum type TPGTextPosItem       */
/* 980616: Released as 4008 002 06812                                      */
/* 980514: Released as 4008 002 06811                                      */
/***************************************************************************/


#define StatusMax      15    // Max number of STATUS-DISPLAYs
#define BaseMax        18    // Max. # of items in the BASE MENU  New: NTP

#define TLGMax          2    // Max. items in the TLG SUBMENU

#define HDTPGMenuItemsMax 5  // Max. items in the HDTPG SUBMENU 

#define HDTPGAudioMax   3    // Max. items in the SDI-TPGx/EMB.AUDIO SUBMENU
#define HDTPGTextMax    6    // Max. items in the HD/TEXT SUBMENU

/********************* DL ***************************************/
#define DLTPGMenuItemsMax 5  // Max. items in the DLTPG SUBMENU 

#define DLTPGAudioMax   3    // Max. items in the  DL/EMB.AUDIO SUBMENU
#define DLTPGTextMax    6    // Max. items in the DL/TEXT SUBMENU
/********************* DL ***************************************/

#define BBMax           3    // Max. items in the BB SUBMENU
#define AnlTPGMax       5    // Max. items in the ANALOG-TPGx MENU
#define SDITSGMax       5    // Max. items in the SDI-TSGx SUBMENU
#define SDITSGAudioMax  2    // Max. items in the SDI-TSGx/EMB.AUDIO SUBMENU
#define SDITPGMax       6    // Max. items in the SDI-TPGx MENU
#define SDITPGAudioMax  3    // Max. items in the SDI-TPGx/EMB.AUDIO SUBMENU
#define TPGTextMax      4    // Max. items in the ANALOG-/SDI-TPGx/TEXT SUBMENU
#define AESEBUMax       3    // Max. items in the AES-EBU MENU
#define PresetMax       3    // Max. items in the PRESET MENU
#define ConfigMax       8    // Max. items in the CONFIG MENU
#define LTCMax      	13   // Max. items in the LTC MENU
#define NTPMax			4	 // Max. items in the NTP MENU

#define DateTimeMax     4    // Max. items in the DATE-TIME SUBMENU
#define RS232Max        4    // Max. items in the RS232 SUBMENU
#define DiagnoseMax     9    // Max. items in the DIAGNOSE SELECT

#define TimeMax     3    // Max. items in the DATE-TIME SUBMENU


// Definition of item names in the different menus

enum PT8603TEXTNdxItems
  {
  EdItem = 0, OnOffItem, MovItem, TextPosItem
  };

enum StatusDispItems {
  StatusPresetItem = 0, StatusGenlockItem,\
  StatusAnlTPG2Item, StatusAnlTPG5Item,\
  StatusSDITSG2Item, StatusSDITSG3Item, StatusSDITSG4Item,\
  StatusSDITPG1Item, StatusSDITPG2Item, StatusSDITPG5Item,\
  StatusAESEBU1Item, StatusAESEBU2Item,\
  StatusDateTimeItem, StatusErrorItem,StatusTimeItem
};

enum BaseItems
  {
  AnlBlkItem = 0,
  TLGItem,
  HDTPGItem,
  DLTPGItem,
  AnlTPG2Item, AnlTPG5Item,
  SDITSG2Item, SDITSG3Item, SDITSG4Item,
  SDITPG1Item, SDITPG2Item, SDITPG5Item,
  AESEBUItem,  GenlockItem, LTCItem, NTPItem, PresetItem, ConfigItem
  };

#if 0
// Below are base items used in standard version without mix sync:
enum BaseItems
  {
  AnlBlkItem = 0,\
  AnlTPG2Item, AnlTPG5Item,\
  SDITSG2Item, SDITSG3Item, SDITSG4Item,\
  SDITPG1Item, SDITPG2Item, SDITPG5Item,\
  AESEBUItem,  GenlockItem, LTCItem, PresetItem, ConfigItem, TestItem
  };

// Below are base items used in version with mix sync:
enum BaseItems
  {
  AnlBlkItem = 0,\
  MixSyncItem,\
  AnlTPG2Item, AnlTPG5Item,\
  SDITSG2Item, SDITSG3Item, SDITSG4Item,\
  SDITPG1Item, SDITPG2Item, SDITPG5Item,\
  AESEBUItem,  GenlockItem, LTCItem, PresetItem, ConfigItem, LastItem
  };
#endif


enum TlGGenItems               // new TLG
  {
  TLG1Item = 0, TLG2Item, TLG3Item, TLG4Item, TLG5Item, TLG6Item, \
  TLG7Item, TLG8Item, TLG9Item, TLG10Item, TLG11Item, TLG12Item
  }

enum TlGItems                  // new TLG
  {
  TLGFormatItem = 0, TLGTimingItem
  }

/***************  HDTPG  *******************************/

enum HDTPGGenItems
  {
  HDTPG1Item = 0, HDTPG2Item, HDTPG3Item, HDTPG4Item, HDTPG5Item,  \
  HDTPG6Item,     HDTPG7Item, HDTPG8Item, HDTPG9Item, HDTPG10Item, \
  HDTPG11Item,    HDTPG12Item
  }


enum HDTPGItems 
  {
  HDTPGPatternItem = 0, HDTPGSystemItem, HDTPGTimingItem, \
  HDTPGAudioItem, HDTPGTextItem
  };

enum HDTPGAudioItems
  {
  HDTPGAudioSignalItem = 0, HDTPGAudioLevelItem, HDTPGAudioClickItem, \
  HDTPGAudioGroupItem,      HDTPGAudioTimingItem
  };

/***************  HDTPG  *******************************/

/***************  DLTPG  *******************************/
enum DLTPGGenItems
  {
  DLTPG1Item = 0, DLTPG2Item, DLTPG3Item, DLTPG4Item, DLTPG5Item,  \
  DLTPG6Item
  }


enum DLTPGItems 
  {
  DLTPGPatternItem = 0, DLTPGSystemItem, DLTPGTimingItem, \
  DLTPGAudioItem, DLTPGTextItem
  };

enum DLTPGAudioItems
  {
  DLTPGAudioSignalItem = 0, DLTPGAudioLevelItem, DLTPGAudioClickItem, \
  DLTPGAudioGroupItem,      DLTPGAudioTimingItem
  };

/***************  DLTPG  *******************************/

enum AnlBlkItems
  {
  BB1Item = 0, BB2Item, BB3Item, BB4Item, BB5Item, BB6Item, BB7Item, BB8Item
  };

enum BBItems {
  BBSystemItem = 0, BBTimingItem, BBScHPhaseItem
};

enum AnlTPGItems {
  AnlTPGPatternItem = 0, AnlTPGTextItem, AnlTPGSystemItem,\
  AnlTPGTimingItem, AnlTPGScHPhaseItem
};

enum SDITSGItems {
  SDITSGPatternItem = 0, SDITSGSystemItem, SDITSGEDHItem,\
  SDITSGAudioItem, SDITSGTimingItem
};

enum SDITSGAudioItems {
  SDITSGAudioSignalItem = 0, SDITSGAudioLevelItem
};

enum SDITPGItems {
  SDITPGPatternItem = 0, SDITPGTextItem, SDITPGSystemItem, SDITPGEDHItem,\
  SDITPGAudioItem, SDITPGTimingItem
};

enum SDITPGAudioItems {
  SDITPGAudioSignalItem = 0, SDITPGAudioLevelItem, SDITPGAudioGroupItem
};

enum TPGTextItems 
  {
  TPGTextEditItem = 0, TPGTextStyleItem, TPGTextPosItem, TPGTextClockItem
  };

/*  mymenus                JK */
enum SDITEXTNdxItems
  {
  PT8603EditItem = 0, PT8603OnOffItem, PT8603MovementItem, PT8603TextPosItem
  };

//*********************
enum HDTPGTextItems 
  {
  HDTPGTextEditItem = 0, HDTPGTextScaleItem, HDTPGTextPosItem, \
  HDTPGTextMovementItem, HDTPGTextColorItem, HDTPGTextBAckgroundItem
  };

enum HDTextItems 
  {
  EditItem = 0, ScaleItem, PositionItem, MovementItem, ColorItem, BackgroundItem
  };


enum AESEBUnItems {
  AESEBU1Item = 0, AESEBU2Item
};

enum AESEBUItems {
  AESEBUSignalItem = 0, AESEBULevelItem, AESEBUTimingItem
};

enum GenlockItems {
  GenlockInputItem = 0, GenlockSystemItem, GenlockTimingItem
};

enum LTCItems {
  LTCSelectItem = 0, LTCOffsetItem, LTCFormatItem, LTCTimeItem,\
  LTCModeItem, LTCTimezoneItem, LTCDaylightItem, LTCStartItem, LTCEndItem,\
  LTCConfirmDaylightItem, LTCSyncConfirmItem, LTCMessageItem,LTCSyncItem
};

enum NTPItems {
	NTPSelectItem=0, NTPETHERConfigItem, NTPConfigItem, NTPSetAddrItem
};

enum PresetItems {
  PresetRecallItem = 0, PresetStoreItem, PresetNameItem
};

enum ConfigItems {
  ConfigDateTimeItem = 0, ConfigLockItem, ConfigAutoESCItem,\
  ConfigLCDContItem, ConfigDownloadItem, ConfigRS232Item,\
  ConfigDiagnoseItem,ConfigTimeItem
};

enum DateTimeItems {
  DateTimeDateItem = 0, DateTimeTimeItem, DateTimeRefItem,\
  DateTimeOffsetItem
};


enum TimeItems {
  TimeTimeItem = 0, TimeRefItem,TimeFormatItem
};


enum RS232Items {
  RS232BaudrateItem = 0, RS232DatabitItem, RS232ParityItem,\
  RS232HandshakeItem
};

enum DiagnoseItems
  {
  DiagMainItem = 0, DiagOptionsItem, DiagRS232Item, DiagDisplayItem,\
  DiagKeyboardItem, DiagMemItem, DiagConfigItem, DiagErrorQueueItem
  };

/********************** PT8603 ******************************/
extern code struct MENU_ITEM PT8603TPGtext_menu[4];
extern code struct MENU_ITEM PT8603TPG_submenu[4] ;
/********************** PT8603 ******************************/


extern code struct MENU_ITEM status_menu[StatusMax];

extern code struct MENU_ITEM base_menu[BaseMax];

extern code struct MENU_ITEM tlg_menu[MaxTLGUnits];
extern code struct MENU_ITEM tlg_submenu[TLGMax];
extern code struct MENU_ITEM tlg_sub_submenu[TLGMax];


/***************  HDTPG  BEG  *******************************/
extern code struct MENU_ITEM hdtpg_menu[MaxHDTPGUnits];
extern code struct MENU_ITEM hdtpg_submenu[HDTPGMenuItemsMax];
extern code struct MENU_ITEM hdtpg_sub_submenu[HDTPGMenuItemsMax];
extern code struct MENU_ITEM HDTPGtext_menu[HDTPGTextMax];
extern code struct MENU_ITEM HDTPGtext_submenu[HDTPGTextMax];
extern code struct MENU_ITEM HDTPGaudio_menu[HDTPGAudioMax];
extern code struct MENU_ITEM HDTPGaudio_submenu[HDTPGAudioMax];


extern code struct MENU_ITEM HDTPGpattern_modify_menu;

/***************  HDTPG  END  *******************************/
/***************  DLTPG  BEG  *******************************/
extern code struct MENU_ITEM DLTPG_menu[MaxDLTPGUnits];
extern code struct MENU_ITEM DLTPG_submenu[DLTPGMenuItemsMax];
extern code struct MENU_ITEM DLTPG_sub_submenu[DLTPGMenuItemsMax];
extern code struct MENU_ITEM DLTPGtext_menu[DLTPGTextMax];
extern code struct MENU_ITEM DLTPGtext_submenu[DLTPGTextMax];
extern code struct MENU_ITEM DLTPGaudio_menu[DLTPGAudioMax];
extern code struct MENU_ITEM DLTPGaudio_submenu[DLTPGAudioMax];

extern code struct MENU_ITEM DLTPGpattern_modify_menu;
extern code struct MENU_ITEM DLTPGsystem_modify_menu;

/***************  DLTPG  END  *******************************/

extern code struct MENU_ITEM anlblk_menu[MaxAnlBlkUnits];
extern code struct MENU_ITEM BB_menu[BBMax];
extern code struct MENU_ITEM BB_submenu[BBMax];

extern code struct MENU_ITEM AnlTPG_menu[AnlTPGMax];
extern code struct MENU_ITEM AnlTPG_submenu[AnlTPGMax];

extern code struct MENU_ITEM SDITSG_menu[SDITSGMax];
extern code struct MENU_ITEM SDITSG_submenu[SDITSGMax];

extern code struct MENU_ITEM AnlTPGpattern_menu;

extern code struct MENU_ITEM AnlTPGtext_menu[TPGTextMax];
extern code struct MENU_ITEM AnlTPGtext_submenu[TPGTextMax];

extern code struct MENU_ITEM SDITSGaudio_menu[SDITSGAudioMax];
extern code struct MENU_ITEM SDITSGaudio_submenu[SDITSGAudioMax];

extern code struct MENU_ITEM SDITPG_menu[SDITPGMax];
extern code struct MENU_ITEM SDITPG_submenu[SDITPGMax];

extern code struct MENU_ITEM SDITPGpattern_menu;


extern code struct MENU_ITEM SDITPGtext_menu[TPGTextMax];
extern code struct MENU_ITEM SDITPGtext_submenu[TPGTextMax];

extern code struct MENU_ITEM SDITPGaudio_menu[SDITPGAudioMax];
extern code struct MENU_ITEM SDITPGaudio_submenu[SDITPGAudioMax];

extern code struct MENU_ITEM AESEBUn_menu[MaxAESEBUUnits];
extern code struct MENU_ITEM AESEBU_menu[AESEBUMax];
extern code struct MENU_ITEM AESEBU_submenu[AESEBUMax];

extern code struct MENU_ITEM genlock_menu;
extern code struct MENU_ITEM genlocksys_menu;
extern code struct MENU_ITEM genlocktiming_menu;

extern code struct MENU_ITEM LTC_menu[LTCMax];
extern code struct MENU_ITEM LTC8620_menu[LTCMax];

extern code struct MENU_ITEM NTP_MENU[NTPMax];

extern code struct MENU_ITEM preset_menu[PresetMax];
extern code struct MENU_ITEM preset_submenu[PresetMax];

extern code struct MENU_ITEM config_menu[ConfigMax];
extern code struct MENU_ITEM config_submenu[ConfigMax];

extern code struct MENU_ITEM datetime_menu[DateTimeMax];
extern code struct MENU_ITEM datetime_submenu[DateTimeMax];

extern code struct MENU_ITEM RS232_menu[RS232Max];
extern code struct MENU_ITEM RS232_submenu[RS232Max];

extern code struct MENU_ITEM diagnose_menu[DiagnoseMax];
extern code struct MENU_ITEM diagnose_submenu[DiagnoseMax];

extern code struct MENU_ITEM download_submenu;
extern code struct MENU_ITEM download_2ndmenu;

extern code struct MENU_ITEM reset_menu;

extern code struct MENU_ITEM RS232local_menu;

extern code struct MENU_ITEM test_menu;

extern code struct MENU_ITEM time_menu[TimeMax];
extern code struct MENU_ITEM time_submenu[TimeMax];

