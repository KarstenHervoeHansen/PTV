/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998                 */
/*  Project:    PT5230 Digital Video Generator                             */
/*  Module:    MENUTREE.C                                                  */
/*  Author:    Kim Engedahl, DEV                                           */
/*  Org. date:  980119                                                     */
/*  Rev. date:  000509 KEn, DEV                                            */
/*  Status:    Version 1.2                                                 */
/*                                                                         */
/*  This module contains NO functions                                      */
/*                                                                         */
/*  Changes:                                                               */
/* 000508: Added support for TPG text positioining                         */
/* 980929: TPGTextItemRight/Left was split int0 AnlTPG../SDITPG...         */
/* 980916: Updated datetime_submenu to correct state machines.             */
/* 980825: Status??Update changed to Status??Show.                         */
/* 980724: Update to support Time Clock Interface.                         */
/* 980616: Released as 4008 002 06812                                      */
/* 980514: Released as 4008 002 06811                                      */
/***************************************************************************/
#include <xa.h>
#include <string.h>

#include "mas.h"
#include "menutree.h"
#include "menu.h"
#include "menuhlp.h"
#include "text.h"
#include "message.h"
#include "util.h"
#include "unit_drv.h"


code struct MENU_ITEM status_menu[StatusMax] = {
  {  StatusPresetItem, NULL, 0, (L_+D_+R_),
    StatusPresetShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[1], &status_menu[14],
  },
  {  StatusGenlockItem, NULL, 0, (L_+D_+R_),
    StatusGenlockShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[2], &status_menu[0],
  },
  {  StatusAnlTPG2Item, NULL, 0, (L_+D_+R_),
    StatusAnlTPGShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[3], &status_menu[1],
  },
  {  StatusAnlTPG5Item, NULL, 0, (L_+D_+R_),
    StatusAnlTPGShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[4], &status_menu[2],
  },
  {  StatusSDITSG2Item, NULL, 0, (L_+D_+R_),
    StatusSDITSGShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[5], &status_menu[3],
  },
  {  StatusSDITSG3Item, NULL, 0, (L_+D_+R_),
    StatusSDITSGShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[6], &status_menu[4],
  },
  {  StatusSDITSG4Item, NULL, 0, (L_+D_+R_),
    StatusSDITSGShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[7], &status_menu[5],
  },
  {  StatusSDITPG1Item, NULL, 0, (L_+D_+R_),
    StatusSDITPGShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[8], &status_menu[6],
  },
  {  StatusSDITPG2Item, NULL, 0, (L_+D_+R_),
    StatusSDITPGShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[9], &status_menu[7],
  },
  {  StatusSDITPG5Item, NULL, 0, (L_+D_+R_),
    StatusSDITPGShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[10], &status_menu[8],
  },
  {  StatusAESEBU1Item, NULL, 0, (L_+D_+R_),
    StatusAESEBUShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[11], &status_menu[9],
  },
  {  StatusAESEBU2Item, NULL, 0, (L_+D_+R_),
    StatusAESEBUShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[12], &status_menu[10],
  },
  {  StatusDateTimeItem, NULL, 0, (L_+D_+R_),
    StatusDateTimeShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[13], &status_menu[11],
  },
  {  StatusErrorItem, NULL, 0, (L_+D_+R_),
    StatusErrorShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[14], &status_menu[12],
  },
    {  StatusTimeItem, NULL, 0, (L_+D_+R_),
    StatusTimeShow,
    StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
    &status_menu[0], &status_menu[13],
  },
};

code struct MENU_ITEM base_menu[BaseMax] = //  17 items
  {
    {
    AnlBlkItem, BlkBurstTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 0
    &base_menu[1], &base_menu[15]
    },
    {
    TLGItem, TLGTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 1
    &base_menu[2], &base_menu[0]
    },
    {
    HDTPGItem, HDTPGTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 2
    &base_menu[3], &base_menu[1]
    },
    {
    DLTPGItem, DLTPGTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 3
    &base_menu[4], &base_menu[2]
    },
    {
    AnlTPG2Item, AnalogTPG2Txt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE,NONE, // 4
    &base_menu[5], &base_menu[3]
    },
    {
    AnlTPG5Item, AnalogTPG5Txt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 5
    &base_menu[6], &base_menu[4]
    },
    {
    SDITSG2Item, SDITSG2Txt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 6
    &base_menu[7], &base_menu[5]
    },
    {
    SDITSG3Item, SDITSG3Txt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 7
    &base_menu[8], &base_menu[6]
    },
    {
    SDITSG4Item, SDITSG4Txt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 8
    &base_menu[9], &base_menu[7]
    },
    {
    SDITPG1Item, SDITPG1Txt, 0, (L_+D_+U_+R_+MORE_),  
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 9
    &base_menu[10], &base_menu[8]
    },
    {
    SDITPG2Item, SDITPG2Txt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 10
    &base_menu[11], &base_menu[9]
    },
    {
    SDITPG5Item, SDITPG5Txt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 11
    &base_menu[12], &base_menu[10]
    },
    {
    AESEBUItem, AESEBUTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 12
    &base_menu[13], &base_menu[11]
    },
    {
    GenlockItem, GenlockTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 13
    &base_menu[14], &base_menu[12]
    },
    //LTC item
    {
    LTCItem, LTCTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 14
  &base_menu[15], &base_menu[13]
  },
    //NTP item
    {
	NTPItem, NetworkTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 14
  	&base_menu[16], &base_menu[14]
    },
    {
    PresetItem, PresetTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 15
    &base_menu[17], &base_menu[15]
    },
    {
    ConfigItem, ConfigTxt, 0, (L_+D_+U_+R_+MORE_),
    BaseMenuShow,
    BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE, // 16
    &base_menu[0], &base_menu[16]
    }
  };

/**********************************************/
#if 0
typedef struct MENU_ITEM
  {
  UC ItemNumber;
  code char* ItemTxt;
  UC ItemPos;
  UC ItemConfig;
  void ( *DispFunct)();
  void ( *LeftBut)();
  void ( *UpBut)();
  void ( *DownBut)();
  void ( *RightBut)();
  void ( *ExecBut)();
  void ( *EscapeFct)();
  code struct MENU_ITEM* NextItem;
  code struct MENU_ITEM* PrevItem;
  };
#endif

code struct MENU_ITEM tlg_menu[MaxTLGUnits] =
  {
    {
    TLG1Item, TLG1Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 0 */
    &tlg_menu[1], &tlg_menu[11]
    },
    {
    TLG2Item, TLG2Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 1 */
    &tlg_menu[2], &tlg_menu[0]
    },
    {
    TLG3Item, TLG3Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 2 */
    &tlg_menu[3], &tlg_menu[1]
    },
    {
    TLG4Item, TLG4Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 3 */
    &tlg_menu[4], &tlg_menu[2]
    },
    {
    TLG5Item, TLG5Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 4 */
    &tlg_menu[5], &tlg_menu[3]
    },
    {
    TLG6Item, TLG6Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 5 */
    &tlg_menu[6], &tlg_menu[4]
    },
    {
    TLG7Item, TLG7Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 6 */
    &tlg_menu[7], &tlg_menu[5]
    },
    {
    TLG8Item, TLG8Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 7 */
    &tlg_menu[8], &tlg_menu[6]
    },
    {
    TLG9Item, TLG9Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 8 */
    &tlg_menu[9], &tlg_menu[7]
    },
    {
    TLG10Item, TLG10Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 9 */
    &tlg_menu[10], &tlg_menu[8]
    },
    {
    TLG11Item, TLG11Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 10 */
    &tlg_menu[11], &tlg_menu[9]
    },
    {
    TLG12Item, TLG12Txt, NA, (L_+D_+U_+R_),
    TLGMenuShow,
    TLGItemLeft, UpToBase, TLGDown, TLGItemRight, NONE, NONE, /* 11 */
    &tlg_menu[0], &tlg_menu[10]
    }
  };


code struct MENU_ITEM tlg_submenu[TLGMax] =
  {
    {
    TLGFormatItem, FormatTxt, 8, (L_+D_+U_+R_),
    TLGSubMenuShow,
    ItemLeft, UpToTLG,
    TLGSubDown, ItemRight, NONE, NONE,
    &tlg_submenu[1], &tlg_submenu[1]
    },
    {
    TLGTimingItem, TimingTxt, 16, (L_+D_+U_+R_),
    TLGSubMenuShow,
    ItemLeft, UpToTLG,
    TLGSubDown, ItemRight, NONE, NONE,
    &tlg_submenu[0], &tlg_submenu[0]
    }
  };

code struct MENU_ITEM tlg_sub_submenu[TLGMax] =
  {
    {
    TLGFormatItem, FormatTxt, 0, NA,
    TLGFormatSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {
    TLGTimingItem, TimingTxt, 0, NA,
    TLGTimingSelect,
    LeftButTypeTLG, UpButTypeTLG,
    DownButTypeTLG, RightButTypeTLG, ExecButTypeTLG, EscFunction,
    NULL, NULL
    }
  };

/***************  DLTPG  BEG  *******************************/

code struct MENU_ITEM DLTPG_menu[MaxDLTPGUnits] =
  {
    {
    DLTPG1Item, DLTPG1Txt, NA, (L_+D_+U_+R_),
    DLTPGMenuShow,
    DLTPGItemLeft, UpToBase, DLTPGDown, DLTPGItemRight, NONE, NONE, /* 0 */
    &DLTPG_menu[1], &DLTPG_menu[5]
    },
    {
    DLTPG2Item, DLTPG2Txt, NA, (L_+D_+U_+R_),
    DLTPGMenuShow,
    DLTPGItemLeft, UpToBase, DLTPGDown, DLTPGItemRight, NONE, NONE, /* 1 */
    &DLTPG_menu[2], &DLTPG_menu[0]
    },
    {
    DLTPG3Item, DLTPG3Txt, NA, (L_+D_+U_+R_),
    DLTPGMenuShow,
    DLTPGItemLeft, UpToBase, DLTPGDown, DLTPGItemRight, NONE, NONE, /* 2 */
    &DLTPG_menu[3], &DLTPG_menu[1]
    },
    {
    DLTPG4Item, DLTPG4Txt, NA, (L_+D_+U_+R_),
    DLTPGMenuShow,
    DLTPGItemLeft, UpToBase, DLTPGDown, DLTPGItemRight, NONE, NONE, /* 3 */
    &DLTPG_menu[4], &DLTPG_menu[2]
    },
    {
    DLTPG5Item, DLTPG5Txt, NA, (L_+D_+U_+R_),
    DLTPGMenuShow,
    DLTPGItemLeft, UpToBase, DLTPGDown, DLTPGItemRight, NONE, NONE, /* 4 */
    &DLTPG_menu[5], &DLTPG_menu[3]
    },
    {
    DLTPG6Item, DLTPG6Txt, NA, (L_+D_+U_+R_),
    DLTPGMenuShow,
    DLTPGItemLeft, UpToBase, DLTPGDown, DLTPGItemRight, NONE, NONE, /* 5 */
    &DLTPG_menu[0], &DLTPG_menu[4]
    }
  };

code struct MENU_ITEM DLTPG_submenu[DLTPGMenuItemsMax] =
  {
    {
    DLTPGPatternItem, PatternTxt, 8, (L_+D_+U_+R_+MORE_), // 0
    DLTPGSubMenuShow,
    ItemLeft, UpToDLTPG,
    DLTPGSubDown, ItemRight, NONE, NONE,
    &DLTPG_submenu[1], &DLTPG_submenu[4]
    },
    {
    DLTPGSystemItem, SystemTxt, 18/*25*/, (L_+D_+U_+R_+MORE_), // 1
    DLTPGSubMenuShow,
    ItemLeft, UpToDLTPG,
    DLTPGSubDown, ItemRight, NONE, NONE,
    &DLTPG_submenu[2], &DLTPG_submenu[0]
    },
    {
    DLTPGTimingItem, TimingTxt, 27, (L_+D_+U_+R_+MORE_), // 2
    DLTPGSubMenuShow,
    ItemLeft, UpToDLTPG,
    DLTPGSubDown, ItemRight, NONE, NONE,
    &DLTPG_submenu[3], &DLTPG_submenu[1]
    },
    {
    DLTPGAudioItem, AudioTxt, 8, (L_+D_+U_+R_+MORE_), //3
    DLTPGSubMenuShow,
    ItemLeft, UpToDLTPG,
    DLTPGSubDown, ItemRight, NONE, NONE,
    &DLTPG_submenu[4], &DLTPG_submenu[2]
    },
    {
    DLTPGTextItem, TextTxt, 18, (L_+D_+U_+R_+MORE_),   // 4
    DLTPGSubMenuShow,
    ItemLeft, UpToDLTPG,
    DLTPGSubDown, ItemRight, NONE, NONE,
    &DLTPG_submenu[0], &DLTPG_submenu[3]
    }
  };

code struct MENU_ITEM DLTPG_sub_submenu[DLTPGMenuItemsMax] =
  {
    {
    DLTPGPatternItem, PatternTxt, 0, NA,
    DLTPGPatternSelect,
    LeftButType7, UpButType7,
    DownButType7, RightButType7, ExecButType7, EscFunction,
    NULL, NULL,
    },
    {  
    DLTPGSystemItem, SystemTxt, 8, NA,
    DLTPGSystemSelect,
    LeftButType7, UpButType7,
    DownButType7, RightButType7, ExecButType7, EscFunction,
    NULL, NULL,
    },
    {
    DLTPGTimingItem, TimingTxt, 0, NA,
    DLTPGTimingSelect,
    LeftButTypeTLG, UpButTypeTLG,
    DownButTypeTLG, RightButTypeTLG, ExecButTypeTLG, EscFunction,
    NULL, NULL,
    },
    {  
    DLTPGAudioItem, EMBAudioTxt, 8, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
    },
    {  
    DLTPGTextItem, TextTxt, 8, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
    }
  };
/********** Interface   BEG ************************/
code struct MENU_ITEM DLTPGsystem_modify_menu =
  {
  NA, PatternTxt, 8, NONE_,
  DLTPGInterfaceModify,
  LeftButType1, UpButType1,
  DownButType1, RightButType1, ExecButType1, EscFunction,
  NULL, NULL
  };
/********** Interface END ************************/

code struct MENU_ITEM DLTPGpattern_modify_menu =
  {
  NA, PatternTxt, 8, NONE_,
  DLTPGPatternModify,
  LeftButType1, UpButType1,
  DownButType1, RightButType1, ExecButType1, EscFunction,
  NULL, NULL
  };


code struct MENU_ITEM DLTPGtext_menu[DLTPGTextMax] =
  {
    {  
    EditItem, EDITTxt, 8, (L_+D_+U_+R_+MORE_),
    DLTPGTextMenuShow,
    ItemLeft, UpToDLTPGsub, DLTPGTextDown, ItemRight,
    NONE, NONE,
    &DLTPGtext_menu[1], &DLTPGtext_menu[5]
    },
    {  
    ScaleItem, ScaleTxt, 14, (L_+D_+U_+R_+MORE_),
    DLTPGTextMenuShow,
    ItemLeft, UpToDLTPGsub, DLTPGTextDown, ItemRight,
    NONE, NONE,
    &DLTPGtext_menu[2], &DLTPGtext_menu[0]
    },
    {  
    PositionItem, PositionTxt, 21, (L_+D_+U_+R_+MORE_),
    DLTPGTextMenuShow,
    ItemLeft, UpToDLTPG, DLTPGTextDown, ItemRight,
    NONE, NONE,
    &DLTPGtext_menu[3], &DLTPGtext_menu[1]
    },
    {  
    MovementItem, MovementTxt, 26, (L_+D_+U_+R_+MORE_),
    DLTPGTextMenuShow,
    ItemLeft, UpToDLTPG, DLTPGTextDown, ItemRight,
    NONE, NONE,
    &DLTPGtext_menu[4], &DLTPGtext_menu[2]
    },
    {  
    ColorItem, ColorTxt, 8, (L_+D_+U_+R_+MORE_),
    DLTPGTextMenuShow,
    ItemLeft, UpToDLTPG, DLTPGTextDown, ItemRight,
    NONE, NONE,
    &DLTPGtext_menu[5], &DLTPGtext_menu[3]
    },
    {  
    BackgroundItem, BackgroundTxt, 20, (L_+D_+U_+R_+MORE_),
    DLTPGTextMenuShow,
    ItemLeft, UpToDLTPG, DLTPGTextDown, ItemRight,
    NONE, NONE,
    &DLTPGtext_menu[0], &DLTPGtext_menu[4]
    }
  };

code struct MENU_ITEM DLTPGtext_submenu[DLTPGTextMax] =
  {
    {  
    EditItem, EDITTxt, 8, NA,
    DLTPGTextEditSelect,
    LeftButType12, UpButType12,
    DownButType12, RightButType12, ExecButType12, EscFunction,
    NULL, NULL
    },
    {  
    ScaleItem, ScaleTxt, 8, NA,
    DLTPGTextScaleSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    PositionItem, PositionTxt, 8, NA,
    DLTPGTextPosSelect,
    LeftButType14, UpButType14,
    DownButType14, RightButType14, ExecButType14, EscFunction,
    NULL, NULL
    },
    {
    MovementItem, MovementTxt, 8, NA,
    DLTPGTextMovementSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {  
    ColorItem, ColorTxt, 8, NA,
    DLTPGTextColorSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    BackgroundItem, BackgroundTxt, 8, NA,
    DLTPGTextBackgroundSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    }
  };

code struct MENU_ITEM DLTPGaudio_menu[DLTPGAudioMax] =
  {
    {  
    DLTPGAudioSignalItem, SignalTxt, 8, (L_+D_+U_+R_+MORE_),
    DLTPGAudioMenuShow,
    ItemLeft, UpToDLTPGsub, DLTPGAudioDown, ItemRight,
    NONE, NONE,
    &DLTPGaudio_menu[1], &DLTPGaudio_menu[2]
    },
    {  
    DLTPGAudioLevelItem, LevelTxt, 17, (L_+D_+U_+R_+MORE_),
    DLTPGAudioMenuShow,
    ItemLeft, UpToDLTPGsub, DLTPGAudioDown, ItemRight,
    NONE, NONE,
    &DLTPGaudio_menu[2], &DLTPGaudio_menu[0]
    },
    {  
    DLTPGAudioClickItem, ClickTxt,  8, (L_+D_+U_+R_+MORE_),
    DLTPGAudioMenuShow,
    ItemLeft, UpToDLTPGsub, DLTPGAudioDown, ItemRight,
    NONE, NONE,
    &DLTPGaudio_menu[0], &DLTPGaudio_menu[1]
    }
  };

code struct MENU_ITEM DLTPGaudio_submenu[DLTPGAudioMax] =
  {
    {  
    DLTPGAudioSignalItem, SignalTxt, 0, NA,
    DLTPGAudioSignalSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    DLTPGAudioLevelItem, LevelTxt, 0, NA,
    DLTPGAudioLevelSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    DLTPGAudioClickItem, ClickTxt, 0, NA,
    DLTPGAudioClickSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    }
  };

/***************  DLTPG  END  *******************************/


/***************  HDTPG  BEG  *******************************/

code struct MENU_ITEM hdtpg_menu[MaxHDTPGUnits] =
  {
    {
    HDTPG1Item, HDTPG1Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 0 */
    &hdtpg_menu[1], &hdtpg_menu[11]
    },
    {
    HDTPG2Item, HDTPG2Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 1 */
    &hdtpg_menu[2], &hdtpg_menu[0]
    },
    {
    HDTPG3Item, HDTPG3Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 2 */
    &hdtpg_menu[3], &hdtpg_menu[1]
    },
    {
    HDTPG4Item, HDTPG4Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 3 */
    &hdtpg_menu[4], &hdtpg_menu[2]
    },
    {
    HDTPG5Item, HDTPG5Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 4 */
    &hdtpg_menu[5], &hdtpg_menu[3]
    },
    {
    HDTPG6Item, HDTPG6Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 5 */
    &hdtpg_menu[6], &hdtpg_menu[4]
    },
    {
    HDTPG7Item, HDTPG7Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 6 */
    &hdtpg_menu[7], &hdtpg_menu[5]
    },
    {
    HDTPG8Item, HDTPG8Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 7 */
    &hdtpg_menu[8], &hdtpg_menu[6]
    },
    {
    HDTPG9Item, HDTPG9Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 8 */
    &hdtpg_menu[9], &hdtpg_menu[7]
    },
    {
    HDTPG10Item, HDTPG10Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 9 */
    &hdtpg_menu[10], &hdtpg_menu[8]
    },
    {
    HDTPG11Item, HDTPG11Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 10 */
    &hdtpg_menu[11], &hdtpg_menu[9]
    },
    {
    HDTPG12Item, HDTPG12Txt, NA, (L_+D_+U_+R_),
    HDTPGMenuShow,
    HDTPGItemLeft, UpToBase, HDTPGDown, HDTPGItemRight, NONE, NONE, /* 11 */
    &hdtpg_menu[0], &hdtpg_menu[10]
    }
  };

code struct MENU_ITEM hdtpg_submenu[HDTPGMenuItemsMax] =
  {
    {
    HDTPGPatternItem, PatternTxt, 8, (L_+D_+U_+R_+MORE_), // 0
    HDTPGSubMenuShow,
    ItemLeft, UpToHDTPG,
    HDTPGSubDown, ItemRight, NONE, NONE,
    &hdtpg_submenu[1], &hdtpg_submenu[4]
    },
    {
    HDTPGSystemItem, SystemTxt, 18/*25*/, (L_+D_+U_+R_+MORE_), // 1
    HDTPGSubMenuShow,
    ItemLeft, UpToHDTPG,
    HDTPGSubDown, ItemRight, NONE, NONE,
    &hdtpg_submenu[2], &hdtpg_submenu[0]
    },
    {
    HDTPGTimingItem, TimingTxt, 27, (L_+D_+U_+R_+MORE_), // 2
    HDTPGSubMenuShow,
    ItemLeft, UpToHDTPG,
    HDTPGSubDown, ItemRight, NONE, NONE,
    &hdtpg_submenu[3], &hdtpg_submenu[1]
    },
    {
    HDTPGAudioItem, AudioTxt, 8, (L_+D_+U_+R_+MORE_), //3
    HDTPGSubMenuShow,
    ItemLeft, UpToHDTPG,
    HDTPGSubDown, ItemRight, NONE, NONE,
    &hdtpg_submenu[4], &hdtpg_submenu[2]
    },
    {
    HDTPGTextItem, TextTxt, 18, (L_+D_+U_+R_+MORE_),   // 4
    HDTPGSubMenuShow,
    ItemLeft, UpToHDTPG,
    HDTPGSubDown, ItemRight, NONE, NONE,
    &hdtpg_submenu[0], &hdtpg_submenu[3]
    }
  };

code struct MENU_ITEM hdtpg_sub_submenu[HDTPGMenuItemsMax] =
  {
    {
    HDTPGPatternItem, PatternTxt, 0, NA,
    HDTPGPatternSelect,
    LeftButType7, UpButType7,
    DownButType7, RightButType7, ExecButType7, EscFunction,
    NULL, NULL,
    },
    {  
    HDTPGSystemItem, SystemTxt, 8, NA,
    HDTPGSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {
    HDTPGTimingItem, TimingTxt, 0, NA,
    HDTPGTimingSelect,
    LeftButTypeTLG, UpButTypeTLG,
    DownButTypeTLG, RightButTypeTLG, ExecButTypeTLG, EscFunction,
    NULL, NULL,
    },
    {  
    HDTPGAudioItem, EMBAudioTxt, 8, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
    },
    {  
    HDTPGTextItem, TextTxt, 8, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
    }
  };


code struct MENU_ITEM HDTPGpattern_modify_menu =
  {
  NA, PatternTxt, 8, NONE_,
  HDTPGPatternModify,
  LeftButType1, UpButType1,
  DownButType1, RightButType1, ExecButType1, EscFunction,
  NULL, NULL
  };

code struct MENU_ITEM HDTPGtext_menu[HDTPGTextMax] =
  {
    {  
    EditItem, EDITTxt, 8, (L_+D_+U_+R_+MORE_),
    HDTPGTextMenuShow,
    ItemLeft, UpToHDTPGsub, HDTPGTextDown, ItemRight,
    NONE, NONE,
    &HDTPGtext_menu[1], &HDTPGtext_menu[5]
    },
    {  
    ScaleItem, ScaleTxt, 14, (L_+D_+U_+R_+MORE_),  // we have SCALE here
    HDTPGTextMenuShow,
    ItemLeft, UpToHDTPGsub, HDTPGTextDown, ItemRight,
    NONE, NONE,
    &HDTPGtext_menu[2], &HDTPGtext_menu[0]
    },
    {  
    PositionItem, PositionTxt, 21, (L_+D_+U_+R_+MORE_),
    HDTPGTextMenuShow,
    ItemLeft, UpToHDTPG, HDTPGTextDown, ItemRight,
    NONE, NONE,
    &HDTPGtext_menu[3], &HDTPGtext_menu[1]
    },
    {  
    MovementItem, MovementTxt, 26, (L_+D_+U_+R_+MORE_), /* we have movement of text here */
    HDTPGTextMenuShow,
    ItemLeft, UpToHDTPG, HDTPGTextDown, ItemRight,
    NONE, NONE,
    &HDTPGtext_menu[4], &HDTPGtext_menu[2]
    },
    {  
    ColorItem, ColorTxt, 8, (L_+D_+U_+R_+MORE_), /*  we have color of text here */
    HDTPGTextMenuShow,
    ItemLeft, UpToHDTPG, HDTPGTextDown, ItemRight,
    NONE, NONE,
    &HDTPGtext_menu[5], &HDTPGtext_menu[3]
    },
    {  
    BackgroundItem, BackgroundTxt, 20, (L_+D_+U_+R_+MORE_), /*  we have bacgrounf color of text here */
    HDTPGTextMenuShow,
    ItemLeft, UpToHDTPG, HDTPGTextDown, ItemRight,
    NONE, NONE,
    &HDTPGtext_menu[0], &HDTPGtext_menu[4]
    }
  };

code struct MENU_ITEM HDTPGtext_submenu[HDTPGTextMax] =
  {
    {  
    EditItem, EDITTxt, 8, NA,
    HDTPGTextEditSelect,
    LeftButType12, UpButType12,
    DownButType12, RightButType12, ExecButType12, EscFunction,
    NULL, NULL
    },
    {  
    ScaleItem, ScaleTxt, 8, NA,         // we have SCALE here
    HDTPGTextScaleSelect,               // this handles SCALE now
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    PositionItem, PositionTxt, 8, NA,
    HDTPGTextPosSelect,
    LeftButType14, UpButType14,
    DownButType14, RightButType14, ExecButType14, EscFunction,
    NULL, NULL
    },
    {
    MovementItem, MovementTxt, 8, NA,
    HDTPGTextMovementSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {  
    ColorItem, ColorTxt, 8, NA,
    HDTPGTextColorSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    BackgroundItem, BackgroundTxt, 8, NA,
    HDTPGTextBackgroundSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    }
  };

code struct MENU_ITEM HDTPGaudio_menu[HDTPGAudioMax] =
  {
    {  
    HDTPGAudioSignalItem, SignalTxt, 8, (L_+D_+U_+R_+MORE_),
    HDTPGAudioMenuShow,
    ItemLeft, UpToHDTPGsub, HDTPGAudioDown, ItemRight,
    NONE, NONE,
    &HDTPGaudio_menu[1], &HDTPGaudio_menu[2]
    },
    {  
    HDTPGAudioLevelItem, LevelTxt, 17, (L_+D_+U_+R_+MORE_),
    HDTPGAudioMenuShow,
    ItemLeft, UpToHDTPGsub, HDTPGAudioDown, ItemRight,
    NONE, NONE,
    &HDTPGaudio_menu[2], &HDTPGaudio_menu[0]
    },
    {  
    HDTPGAudioClickItem, ClickTxt,  8, (L_+D_+U_+R_+MORE_),
    HDTPGAudioMenuShow,
    ItemLeft, UpToHDTPGsub, HDTPGAudioDown, ItemRight,
    NONE, NONE,
    &HDTPGaudio_menu[0], &HDTPGaudio_menu[1]
    }
  };

code struct MENU_ITEM HDTPGaudio_submenu[HDTPGAudioMax] =
  {
    {  
    HDTPGAudioSignalItem, SignalTxt, 0, NA,
    HDTPGAudioSignalSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    HDTPGAudioLevelItem, LevelTxt, 0, NA,
    HDTPGAudioLevelSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {  
    HDTPGAudioClickItem, ClickTxt, 0, NA,
    HDTPGAudioClickSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    }
  };

/***************  HDTPG  END  *******************************/



code struct MENU_ITEM anlblk_menu[MaxAnlBlkUnits] = {
  {  BB1Item, BB1Txt, NA, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
    &anlblk_menu[1], &anlblk_menu[7]
  },
  {  BB2Item, BB2Txt, NA, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
    &anlblk_menu[2], &anlblk_menu[0]
  },
  {  BB3Item, BB3Txt, NA, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
    &anlblk_menu[3], &anlblk_menu[1]
  },
  {  BB4Item, BB4Txt, NA, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
    &anlblk_menu[4], &anlblk_menu[2]
  },
  {  BB5Item, BB5Txt, NA, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
    &anlblk_menu[5], &anlblk_menu[3]
  },
  {  BB6Item, BB6Txt, NA, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
    &anlblk_menu[6], &anlblk_menu[4]
  },
  {  BB7Item, BB7Txt, NA, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
    &anlblk_menu[7], &anlblk_menu[5]
  },
  {  BB8Item, BB8Txt, NA, (L_+D_+U_+R_),
    AnlBlkMenuShow,
    AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
    &anlblk_menu[0], &anlblk_menu[6]
  }
};

code struct MENU_ITEM BB_menu[BBMax] =
  {
    {
    BBSystemItem, SystemTxt, 8, (L_+D_+U_+R_),
    BBMenuShow,
    ItemLeft, UpToAnlBlk, BBDown, ItemRight, NONE, NONE,
    &BB_menu[1], &BB_menu[2]
    },
    {
    BBTimingItem, TimingTxt, 17, (L_+D_+U_+R_),
    BBMenuShow,
    ItemLeft, UpToAnlBlk, BBDown, ItemRight, NONE, NONE,
    &BB_menu[2], &BB_menu[0]
    },
    {
    BBScHPhaseItem, ScHPhaseTxt, 26, (L_+D_+U_+R_),
    BBMenuShow,
    ItemLeft, UpToAnlBlk, BBDown, ItemRight, NONE, NONE,
    &BB_menu[0], &BB_menu[1]
    },
  };

code struct MENU_ITEM BB_submenu[BBMax] =
  {
    {
    BBSystemItem, SystemTxt, 0, NA,
    BBSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {
    BBTimingItem, TimingTxt, 0, NA,
    BBTimingSelect,
    LeftButType6, UpButType6,
    DownButType6, RightButType6, ExecButType6, EscFunction,
    NULL, NULL,
    },
    {
    BBScHPhaseItem, ScHPhaseTxt, 0, NA,
    BBScHPhaseSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    }
  };

code struct MENU_ITEM AnlTPG_menu[AnlTPGMax] = {
  {  AnlTPGPatternItem, PatternTxt, 8, (L_+D_+U_+R_+MORE_),
    AnlTPGMenuShow,
    ItemLeft, UpToBase, AnlTPGDown, ItemRight, NONE, NONE,
    &AnlTPG_menu[1], &AnlTPG_menu[4]
  },
  {  AnlTPGTextItem, TextTxt, 18, (L_+D_+U_+R_+MORE_),
    AnlTPGMenuShow,
    ItemLeft, UpToBase, AnlTPGDown, ItemRight, NONE, NONE,
    &AnlTPG_menu[2], &AnlTPG_menu[0]
  },
  {  AnlTPGSystemItem, SystemTxt, 25, (L_+D_+U_+R_+MORE_),
    AnlTPGMenuShow,
    ItemLeft, UpToBase, AnlTPGDown, ItemRight, NONE, NONE,
    &AnlTPG_menu[3], &AnlTPG_menu[1]
  },
  {  AnlTPGTimingItem, TimingTxt, 8, (L_+D_+U_+R_+MORE_),
    AnlTPGMenuShow,
    ItemLeft, UpToBase, AnlTPGDown, ItemRight, NONE, NONE,
    &AnlTPG_menu[4], &AnlTPG_menu[2]
  },
  {  AnlTPGScHPhaseItem, ScHPhaseTxt, 17, (L_+D_+U_+R_+MORE_),
    AnlTPGMenuShow,
    ItemLeft, UpToBase, AnlTPGDown, ItemRight, NONE, NONE,
    &AnlTPG_menu[0], &AnlTPG_menu[3]
  }
};

code struct MENU_ITEM AnlTPG_submenu[AnlTPGMax] = {
  {  AnlTPGPatternItem, PatternTxt, 8, (L_+D_+U_+R_+MORE_),
    AnlTPGPatternSelect,
    LeftButType7, UpButType7,
    DownButType7, RightButType7, ExecButType7, EscFunction,
    NULL, NULL,
  },
  {  AnlTPGTextItem, TextTxt, 8, (L_+D_+U_+R_+MORE_),
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
  },
  {  AnlTPGSystemItem, SystemTxt, 8, (L_+D_+U_+R_+MORE_),
    AnlTPGSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  AnlTPGTimingItem, TimingTxt, 8, (L_+D_+U_+R_+MORE_),
    AnlTPGTimingSelect,
    LeftButType6, UpButType6,
    DownButType6, RightButType6, ExecButType6, EscFunction,
    NULL, NULL,
  },
  {  AnlTPGScHPhaseItem, ScHPhaseTxt, 8, (L_+D_+U_+R_+MORE_),
    AnlTPGScHPhaseSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM AnlTPGpattern_menu = {
  NA, PatternTxt, 8, NONE_,
  AnlTPGPatternModify,
  LeftButType2, UpButType2,
  DownButType2, RightButType2, ExecButType2, EscFunction,
  NULL, NULL
};

code struct MENU_ITEM AnlTPGtext_menu[TPGTextMax] = {
  {  TPGTextEditItem, EDITTxt, 8, (L_+D_+U_+R_),
    AnlTPGTextMenuShow,
    AnlTPGTextItemLeft, UpToAnlTPG, AnlTPGTextDown, AnlTPGTextItemRight,
    NONE, NONE,
    &AnlTPGtext_menu[1], &AnlTPGtext_menu[3]
  },
  {  TPGTextStyleItem, StyleTxt, 15, (L_+D_+U_+R_),
    AnlTPGTextMenuShow,
    AnlTPGTextItemLeft, UpToAnlTPG, AnlTPGTextDown, AnlTPGTextItemRight,
    NONE, NONE,
    &AnlTPGtext_menu[2], &AnlTPGtext_menu[0]
  },
  {  TPGTextPosItem, PositionTxt, 23, (L_+D_+U_+R_),
    AnlTPGTextMenuShow,
    AnlTPGTextItemLeft, UpToAnlTPG, AnlTPGTextDown, AnlTPGTextItemRight,
    NONE, NONE,
    &AnlTPGtext_menu[3], &AnlTPGtext_menu[1]
  },
  {  TPGTextClockItem, ClockTxt, 29, (L_+D_+U_+R_),
    AnlTPGTextMenuShow,
    AnlTPGTextItemLeft, UpToAnlTPG, AnlTPGTextDown, AnlTPGTextItemRight,
    NONE, NONE,
    &AnlTPGtext_menu[0], &AnlTPGtext_menu[2]
  },
};

code struct MENU_ITEM AnlTPGtext_submenu[TPGTextMax] = {
  {  TPGTextEditItem, EDITTxt, 8, NA,
    AnlTPGTextEditSelect,
    LeftButType12, UpButType12,
    DownButType12, RightButType12, ExecButType12, EscFunction,
    NULL, NULL,
  },
  {  TPGTextStyleItem, StyleTxt, 8, NA,
    AnlTPGTextStyleSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  TPGTextPosItem, PositionTxt, 8, NA,
    AnlTPGTextPosSelect,
    LeftButType14, UpButType14,
    DownButType14, RightButType14, ExecButType14, EscFunction,
    NULL, NULL,
  },
  {  TPGTextClockItem, ClockTxt, 8, NA,
    AnlTPGTextClockSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM SDITSG_menu[SDITSGMax] =
  {
    {  
    SDITSGPatternItem, PatternTxt, 8, (L_+D_+U_+R_+MORE_), // 0
    SDITSGMenuShow,
    ItemLeft, UpToBase, SDITSGDown, ItemRight, NONE, NONE,
    &SDITSG_menu[1], &SDITSG_menu[4]
    },
    {  
    SDITSGSystemItem, SystemTxt, 18, (L_+D_+U_+R_+MORE_), // 1
    SDITSGMenuShow,
    ItemLeft, UpToBase, SDITSGDown, ItemRight, NONE, NONE,
    &SDITSG_menu[2], &SDITSG_menu[0]
    },
    {  
    SDITSGEDHItem, EDHTxt, 27, (L_+D_+U_+R_+MORE_),       // 2
    SDITSGMenuShow,
    ItemLeft, UpToBase, SDITSGDown, ItemRight, NONE, NONE,
    &SDITSG_menu[3], &SDITSG_menu[1]
    },
    { 
    SDITSGAudioItem, EMBAudioTxt, 8, (L_+D_+U_+R_+MORE_), // 3
    SDITSGMenuShow,
    ItemLeft, UpToBase, SDITSGDown, ItemRight, NONE, NONE,
    &SDITSG_menu[4], &SDITSG_menu[2]
    },
    {  
    SDITSGTimingItem, TimingTxt, 20, (L_+D_+U_+R_+MORE_), // 4
    SDITSGMenuShow,
    ItemLeft, UpToBase, SDITSGDown, ItemRight, NONE, NONE,
    &SDITSG_menu[0], &SDITSG_menu[3]
    }
};

code struct MENU_ITEM SDITSG_submenu[SDITSGMax] = {
  {  SDITSGPatternItem, PatternTxt, NA, NA,
    SDITSGPatternSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  SDITSGSystemItem, SystemTxt, NA, NA,
    SDITSGSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  SDITSGEDHItem, EDHTxt, 0, NA,
    SDITSGEDHSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  SDITSGAudioItem, EMBAudioTxt, NA, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
  },
  {  SDITSGTimingItem, TimingTxt, NA, NA,
    SDITSGTimingSelect,
    LeftButType6, UpButType6,
    DownButType6, RightButType6, ExecButType6, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM SDITSGaudio_menu[SDITSGAudioMax] = {
  {  SDITSGAudioSignalItem, SignalTxt, 8, (L_+D_+U_+R_),
    SDITSGAudioMenuShow,
    ItemLeft, UpToSDITSG, SDITSGAudioDown, ItemRight, NONE, NONE,
    &SDITSGaudio_menu[1], &SDITSGaudio_menu[1]
  },
  {  SDITSGAudioLevelItem, LevelTxt, 18, (L_+D_+U_+R_),
    SDITSGAudioMenuShow,
    ItemLeft, UpToSDITSG, SDITSGAudioDown, ItemRight, NONE, NONE,
    &SDITSGaudio_menu[0], &SDITSGaudio_menu[0]
  },
};

code struct MENU_ITEM SDITSGaudio_submenu[SDITSGAudioMax] = {
  {  SDITSGAudioSignalItem, SignalTxt, 8, (L_+D_+U_+R_+MORE_),
    SDITSGAudioSignalSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  SDITSGAudioLevelItem, LevelTxt, 8, (L_+D_+U_+R_+MORE_),
    SDITSGAudioLevelSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  }
};

/********************  SDITPG  BEG  ************************/

code struct MENU_ITEM SDITPG_menu[SDITPGMax] =
  {
    {  
    SDITPGPatternItem, PatternTxt, 8, (L_+D_+U_+R_+MORE_),
    SDITPGMenuShow,
    ItemLeft, UpToBase, SDITPGDown, ItemRight, NONE, NONE,
    &SDITPG_menu[1], &SDITPG_menu[5]
    },
    {  
    SDITPGTextItem, TextTxt, 18, (L_+D_+U_+R_+MORE_),
    SDITPGMenuShow,
    ItemLeft, UpToBase, SDITPGDown, ItemRight, NONE, NONE,
    &SDITPG_menu[2], &SDITPG_menu[0]
    },
    {  
    SDITPGSystemItem, SystemTxt, 25, (L_+D_+U_+R_+MORE_),
    SDITPGMenuShow,
    ItemLeft, UpToBase, SDITPGDown, ItemRight, NONE, NONE,
    &SDITPG_menu[3], &SDITPG_menu[1]
    },
    {  
    SDITPGEDHItem, EDHTxt, 8, (L_+D_+U_+R_+MORE_),
    SDITPGMenuShow,
    ItemLeft, UpToBase, SDITPGDown, ItemRight, NONE, NONE,
    &SDITPG_menu[4], &SDITPG_menu[2]
    },
    {  
    SDITPGAudioItem, EMBAudioTxt, 15, (L_+D_+U_+R_+MORE_),
    SDITPGMenuShow,
    ItemLeft, UpToBase, SDITPGDown, ItemRight, NONE, NONE,
    &SDITPG_menu[5], &SDITPG_menu[3]
    },
    {  
    SDITPGTimingItem, TimingTxt, 27, (L_+D_+U_+R_+MORE_),
    SDITPGMenuShow,
    ItemLeft, UpToBase, SDITPGDown, ItemRight, NONE, NONE,
    &SDITPG_menu[0], &SDITPG_menu[4]
    }
};

code struct MENU_ITEM SDITPG_submenu[SDITPGMax] =
  {
    {  
    SDITPGPatternItem, PatternTxt, 8, NA,
    SDITPGPatternSelect,
    LeftButType7, UpButType7,
    DownButType7, RightButType7, ExecButType7, EscFunction,
    NULL, NULL,
    },
  {  SDITPGTextItem, TextTxt, 8, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
  },
  {  SDITPGSystemItem, SystemTxt, 8, NA,
    SDITPGSystemSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  SDITPGEDHItem, EDHTxt, 8, NA,
    SDITPGEDHSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  SDITPGAudioItem, EMBAudioTxt, 8, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
  },
  {  SDITPGTimingItem, TimingTxt, 8, NA,
    SDITPGTimingSelect,
    LeftButType6, UpButType6,
    DownButType6, RightButType6, ExecButType6, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM SDITPGpattern_menu =
  {
  NA, PatternTxt, 8, NONE_,
  SDITPGPatternModify,
  LeftButType2, UpButType2,
  DownButType2, RightButType2, ExecButType2, EscFunction,
  NULL, NULL
  };

code struct MENU_ITEM SDITPGtext_menu[TPGTextMax] =
  {
    {  
    TPGTextEditItem, EDITTxt, 8, (L_+D_+U_+R_),
    SDITPGTextMenuShow,
    SDITPGTextItemLeft, UpToSDITPG, SDITPGTextDown, SDITPGTextItemRight,
    NONE, NONE,
    &SDITPGtext_menu[1], &SDITPGtext_menu[3]
    },
    {  
    TPGTextStyleItem, StyleTxt, 15, (L_+D_+U_+R_),
    SDITPGTextMenuShow,
    SDITPGTextItemLeft, UpToSDITPG, SDITPGTextDown, SDITPGTextItemRight,
    NONE, NONE,
    &SDITPGtext_menu[2], &SDITPGtext_menu[0]
    },
    {  
    TPGTextPosItem, PositionTxt, 23, (L_+D_+U_+R_),
    SDITPGTextMenuShow,
    SDITPGTextItemLeft, UpToSDITPG, SDITPGTextDown, SDITPGTextItemRight,
    NONE, NONE,
    &SDITPGtext_menu[3], &SDITPGtext_menu[1]
    },
    {  
    TPGTextClockItem, ClockTxt, 29, (L_+D_+U_+R_),
    SDITPGTextMenuShow,
    SDITPGTextItemLeft, UpToSDITPG, SDITPGTextDown, SDITPGTextItemRight,
    NONE, NONE,
    &SDITPGtext_menu[0], &SDITPGtext_menu[2]
    },
  };

code struct MENU_ITEM SDITPGtext_submenu[TPGTextMax] =
  {
    {  
    TPGTextEditItem, EDITTxt, 8, NA,
    SDITPGTextEditSelect,
    LeftButType12, UpButType12,
    DownButType12, RightButType12, ExecButType12, EscFunction,
    NULL, NULL,
    },
    {  
    TPGTextStyleItem, StyleTxt, 8, NA,
    SDITPGTextStyleSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {  
    TPGTextPosItem, PositionTxt, 8, NA,
    SDITPGTextPosSelect,
    LeftButType14, UpButType14,
    DownButType14, RightButType14, ExecButType14, EscFunction,
    NULL, NULL,
    },
    {  
    TPGTextClockItem, ClockTxt, 8, NA,
    SDITPGTextClockSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    }
  };

/* This menu is entered when user presses DOWN on TEXT in SDI-SIGNAL menu */
/* and it shows:                                                          */
/*   <EDIT>  <ON/OFF>  <MOVEMENT>  <ESC>                                  */
code struct MENU_ITEM PT8603TPGtext_menu[4] =
  {
    {
    EdItem, EDITTxt, 8, (L_+D_+U_+R_),
    SDITPGTextMenuShow, 
    ItemLeft, UpToSDITPG, SDITPGTextDown, ItemRight, NONE, NONE,  /* 0 */
    &PT8603TPGtext_menu[1], &PT8603TPGtext_menu[3]
    },
    {
    OnOffItem, OnOffTxt, 14,  (L_+D_+U_+R_),
    SDITPGTextMenuShow, 
    ItemLeft, UpToSDITPG, SDITPGTextDown, ItemRight, NONE, NONE,  /* 1 */
    &PT8603TPGtext_menu[2], &PT8603TPGtext_menu[0]
    },
    {
    MovItem, MovementTxt, 22,  (L_+D_+U_+R_),
    SDITPGTextMenuShow, 
    ItemLeft, UpToSDITPG, SDITPGTextDown, ItemRight, NONE, NONE,  /* 2 */
    &PT8603TPGtext_menu[3], &PT8603TPGtext_menu[1]
    },
    {
    TextPosItem, PositionTxt, 32,  (L_+D_+U_+R_),
    SDITPGTextMenuShow, 
    ItemLeft, UpToSDITPG, SDITPGTextDown, ItemRight, NONE, NONE,  /* 3 */
    &PT8603TPGtext_menu[0], &PT8603TPGtext_menu[2]
    }
  };

code struct MENU_ITEM PT8603TPG_submenu[4] =
  {
    {
    EditItem, EDITTxt, 8, NA,
    PT8603TextEditSelect,
    LeftButTypeDK, UpButTypeDK,
    DownButTypeDK, RightButTypeDK, ExecButTypeDK, EscFunction,
    NULL, NULL
    },
    {
    OnOffItem, OnOffTxt, 8, NA,
    PT8603TextOnOffSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction, /* ExecButType1 ?? */
    NULL, NULL
    },
    {
    MovementItem, MovementTxt, 8, NA,
    PT8603TextMovementSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL
    },
    {
    TextPosItem, PositionTxt, 8, NA,
    PT8603TextPosSelect,
    LeftButType14, UpButType14,
    DownButType14, RightButType14, ExecButType14, EscFunction,
    NULL, NULL
    }
 };
/********************************************************************/

code struct MENU_ITEM SDITPGaudio_menu[SDITPGAudioMax] =
  {
    {  
    SDITPGAudioSignalItem, SignalTxt, 8, (L_+D_+U_+R_),
    SDITPGAudioMenuShow,
    SDITPGAudioItemLeft, UpToSDITPG, SDITPGAudioDown, SDITPGAudioItemRight,
    NONE, NONE,
    &SDITPGaudio_menu[1], &SDITPGaudio_menu[2]
    },
    {  
    SDITPGAudioLevelItem, LevelTxt, 17, (L_+D_+U_+R_),
    SDITPGAudioMenuShow,
    SDITPGAudioItemLeft, UpToSDITPG, SDITPGAudioDown, SDITPGAudioItemRight,
    NONE, NONE,
    &SDITPGaudio_menu[2], &SDITPGaudio_menu[0]
    },
    {  
    SDITPGAudioGroupItem, GroupTxt, 25, (L_+D_+U_+R_),
    SDITPGAudioMenuShow,
    SDITPGAudioItemLeft, UpToSDITPG, SDITPGAudioDown, SDITPGAudioItemRight,
    NONE, NONE,
    &SDITPGaudio_menu[0], &SDITPGaudio_menu[1]
    },
  };

code struct MENU_ITEM SDITPGaudio_submenu[SDITPGAudioMax] =
  {
    {  
    SDITPGAudioSignalItem, SignalTxt, 8, (L_+D_+U_+R_+MORE_),
    SDITPGAudioSignalSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {  
    SDITPGAudioLevelItem, LevelTxt, 8, (L_+D_+U_+R_+MORE_),
    SDITPGAudioLevelSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    },
    {  
    SDITPGAudioGroupItem, GroupTxt, 8, (L_+D_+U_+R_+MORE_),
    SDITPGAudioGroupSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
    }
  };

/********************  SDITPG  END  ************************/

code struct MENU_ITEM AESEBUn_menu[MaxAESEBUUnits] = {
  {  AESEBU1Item, AESEBU1Txt, 8, (L_+D_+U_+R_),
    AESEBUnMenuShow,
    AESEBUnItemLeft, UpToBase, AESEBUnDown, AESEBUnItemRight, NONE, NONE,
    &AESEBUn_menu[1], &AESEBUn_menu[1]
  },
  {  AESEBU2Item, AESEBU2Txt, 19, (L_+D_+U_+R_),
    AESEBUnMenuShow,
    AESEBUnItemLeft, UpToBase, AESEBUnDown, AESEBUnItemRight, NONE, NONE,
    &AESEBUn_menu[0], &AESEBUn_menu[0]
  }
};

code struct MENU_ITEM AESEBU_menu[AESEBUMax] = {
  {  AESEBUSignalItem, SignalTxt, 8, (L_+D_+U_+R_),
    AESEBUMenuShow,
    ItemLeft, UpToAESEBUn, AESEBUDown, ItemRight, NONE, NONE,
    &AESEBU_menu[1], &AESEBU_menu[2]
  },
  {  AESEBULevelItem, LevelTxt, 17, (L_+U_+R_),
    AESEBUMenuShow,
    ItemLeft, UpToAESEBUn, AESEBUDown, ItemRight, NONE, NONE,
    &AESEBU_menu[2], &AESEBU_menu[0]
  },
  {  AESEBUTimingItem, TimingTxt, 25, (L_+U_+R_),
    AESEBUMenuShow,
    ItemLeft, UpToAESEBUn, AESEBUDown, ItemRight, NONE, NONE,
    &AESEBU_menu[0], &AESEBU_menu[1]
  }
};

code struct MENU_ITEM AESEBU_submenu[AESEBUMax] = {
  {  AESEBUSignalItem, SignalTxt, 8, NA,
    AESEBUSignalSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  AESEBULevelItem, LevelTxt, 8, NA,
    AESEBULevelSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  AESEBUTimingItem, TimingTxt, 8, NA,
    AESEBUTimingSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
};

code struct MENU_ITEM genlock_menu = {
  NA, SystemTxt, 8, NONE_,
  GenlockInputSelect,
  LeftButType9, UpButType9,
  DownButType9, RightButType9, ExecButType9, EscFunction,
  NULL, NULL
};

code struct MENU_ITEM genlocksys_menu = {
  NA, SystemTxt, 8, NONE_,
  GenlockSystemSelect,
  LeftButType1, UpButType1,
  DownButType1, RightButType1, ExecButType1, EscFunction,
  NULL, NULL
};

code struct MENU_ITEM genlocktiming_menu = {
  NA, TimingTxt, 8, NONE_,
  GenlockTimingSelect,
  LeftButType6, UpButType6,
  DownButType6, RightButType6, ExecButType6, EscFunction,
  NULL, NULL
};

code struct MENU_ITEM LTC_menu[LTCMax] = {
  { LTCSelectItem, SystemTxt, 8, NONE_,
    LTCSelect,
  LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCOffsetItem, OffsetTxt, 8, NONE_,
    LTCOffsetSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCFormatItem, LTCFormatTxt, 8, NONE_,
    LTCFormatSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCTimeItem, TimeTxt, 8, NONE_,
    LTCTimeSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCModeItem, LTCModeTxt, 8, NONE_,
    LTCModeSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCTimezoneItem, LTCTimezoneTxt, 8, NONE_,
    LTCTimezoneSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCDaylightItem, LTCDaylightTxt, 8, NONE_,
    LTCDaylightSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCStartItem, LTCStartTxt, 8, NONE_,
    LTCStartSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCEndItem, LTCEndTxt, 8, NONE_,
    LTCEndSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCConfirmDaylightItem, ConfirmTxt, 8, NONE_,
    LTCDaylightConfirm,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCSyncConfirmItem, ConfirmTxt, 8, NONE_,
    LTCSyncConfirm,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCMessageItem, LTCTxt, 8, NONE_,
    LTCMessageMenu,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  }
  
};


code struct MENU_ITEM NTP_MENU[NTPMax] = {
	{
	NTPSelectItem, SystemTxt, 8, NONE_,
    NTPSelectMenu,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
	},
	{ NTPETHERConfigItem, ETHConfigTxt, 8,
	(L_+D_+U_+R_+MORE_),
    NTPETHERConfigMenu,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
	},
	{ NTPConfigItem, ConfigTxt,  8,
	(L_+D_+U_+R_),
    NTPConfigMenu,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
	},
	{ NTPSetAddrItem, ConfigTxt, 8,
	(L_+D_+U_+R_),
	NTPAddrModifyMenu,
	LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
	}
};

code struct MENU_ITEM preset_menu[PresetMax] = {
  {  PresetRecallItem, RecallTxt, 8, (L_+D_+U_+R_),
    PresetMenuShow,
    ItemLeft, UpToBase, PresetDown, ItemRight, NONE, NONE,
    &preset_menu[1], &preset_menu[2]
  },
  {  PresetStoreItem, StoreTxt, 17, (L_+D_+U_+R_),
    PresetMenuShow,
    ItemLeft, UpToBase, PresetDown, ItemRight, NONE, NONE,
    &preset_menu[2], &preset_menu[0]
  },
  {  PresetNameItem, NameTxt, 25, (L_+D_+U_+R_),
    PresetMenuShow,
    ItemLeft, UpToBase, PresetDown, ItemRight, NONE, NONE,
    &preset_menu[0], &preset_menu[1]
  }
};

code struct MENU_ITEM preset_submenu[PresetMax] = {
  {  PresetRecallItem, RecallTxt, 8, NA,
    PresetRecallSelect,
    LeftButType3, UpButType3,
    DownButType3, RightButType3, ExecButType3, EscFunction,
    NULL, NULL,
  },
  {  PresetStoreItem, StoreTxt, 8, NA,
    PresetStoreSelect,
    LeftButType3, UpButType3,
    DownButType3, RightButType3, ExecButType3, EscFunction,
    NULL, NULL,
  },
  {  PresetNameItem, NameTxt, 8, NA,
    PresetNameSelect,
    LeftButType8, UpButType8,
    DownButType8, RightButType8, ExecButType8, EscFunction,
    NULL, NULL,
  }
};
code struct MENU_ITEM config_menu[ConfigMax] = {
  {  ConfigDateTimeItem, ConfigDateTimeTxt, 8, (L_+U_+D_+R_+MORE_),
    ConfigMenuShow,
    ConfigItemLeft, UpToBase, ConfigDown, ConfigItemRight, NONE, NONE,
    &config_menu[1], &config_menu[6]
  },
  {  ConfigLockItem, ConfigLockTxt, 20, (L_+D_+U_+R_+MORE_),
    ConfigMenuShow,
    ConfigItemLeft, UpToBase, ConfigDown, ConfigItemRight, NONE, NONE,
    &config_menu[2], &config_menu[0]
  },
  {  ConfigAutoESCItem, ConfigAutoESCTxt, 27, (L_+D_+U_+R_+MORE_),
    ConfigMenuShow,
    ConfigItemLeft, UpToBase, ConfigDown, ConfigItemRight, NONE, NONE,
    &config_menu[3], &config_menu[1]
  },
  {  ConfigLCDContItem, ConfigLCDContTxt, 8, (L_+U_+D_+R_+MORE_),
    ConfigMenuShow,
    ConfigItemLeft, UpToBase, ConfigDown, ConfigItemRight, NONE, NONE,
    &config_menu[4], &config_menu[2]
  },
  {  ConfigDownloadItem, ConfigDownloadTxt, 23, (L_+U_+D_+R_+MORE_),
    ConfigMenuShow,
    ConfigItemLeft, UpToBase, ConfigDown, ConfigItemRight, NONE, NONE,
    &config_menu[5], &config_menu[3]
  },
  {  ConfigRS232Item, RS232Txt, 8, (L_+D_+U_+R_+MORE_),
    ConfigMenuShow,
    ConfigItemLeft, UpToBase, ConfigDown, ConfigItemRight, NONE, NONE,
    &config_menu[6], &config_menu[4]
  },
  {  ConfigDiagnoseItem, ConfigDiagnoseTxt, 16, (L_+D_+U_+R_+MORE_),
    ConfigMenuShow,
    ConfigItemLeft, UpToBase, ConfigDown, ConfigItemRight, NONE, NONE,
    &config_menu[0], &config_menu[5]
  }
};

code struct MENU_ITEM config_submenu[ConfigMax] = {
  {  ConfigDateTimeItem, NULL, 0, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
  },
  {  ConfigLockItem, ConfigLockTxt, 8, (L_+U_+R_+MORE_),
    ConfigLockSelect,
    LeftButType2, UpButType2,
    DownButType2, RightButType2, ExecButType2, EscFunction,
    NULL, NULL,
  },
  {  ConfigAutoESCItem, ConfigAutoESCTxt, 8, (L_+U_+R_+MORE_),
    ConfigAutoESCSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  ConfigLCDContItem, ConfigLCDContTxt, 8, (L_+U_+R_+MORE_),
    ConfigLCDContSelect,
    LeftButType4, UpButType4,
    DownButType4, RightButType4, NONE, EscFunction,
    NULL, NULL,
  },
  {  ConfigDownloadItem, TimingTxt, 8, NONE_,
    DownloadTypeSelect,
    LeftButType3, UpButType3,
    DownButType3, RightButType3, ExecButType3, EscFunction,
    NULL, NULL
  },
  {  ConfigRS232Item, NULL, 0, NA,
    NONE, NONE, NONE, NONE, NONE, NONE, NONE,
    NULL, NULL,
  },
  {  ConfigDiagnoseItem, ConfigDiagnoseTxt, 17, (L_+U_+R_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, NONE, NONE, ItemRight, NONE, NONE,
    NULL, NULL,
  },
};


code struct MENU_ITEM datetime_menu[DateTimeMax] = {
  {  DateTimeDateItem, DateTxt, 8, (L_+D_+U_+R_+MORE_),
    DateTimeMenuShow,
    ItemLeft, UpToConfig, DateTimeDown, ItemRight, NONE, NONE,
    &datetime_menu[1], &datetime_menu[3]
  },
  {  DateTimeTimeItem, TimeTxt, 15, (L_+D_+U_+R_+MORE_),
    DateTimeMenuShow,
    ItemLeft, UpToConfig, DateTimeDown, ItemRight, NONE, NONE,
    &datetime_menu[2], &datetime_menu[0]
  },
  {  DateTimeRefItem, ReferenceTxt, 22, (L_+D_+U_+R_+MORE_),
    DateTimeMenuShow,
    ItemLeft, UpToConfig, DateTimeDown, ItemRight, NONE, NONE,
    &datetime_menu[3], &datetime_menu[1]
  },
  {  DateTimeOffsetItem, OffsetTxt, 8, (L_+D_+U_+R_+MORE_),
    DateTimeMenuShow,
    ItemLeft, UpToConfig, DateTimeDown, ItemRight, NONE, NONE,
    &datetime_menu[0], &datetime_menu[2]
  }
};

code struct MENU_ITEM datetime_submenu[DateTimeMax] = {
  {  DateTimeDateItem, DateTxt, 8, (L_+D_+U_+R_+MORE_),
    DateTimeDateSelect,
    LeftButType13, UpButType13,
    DownButType13, RightButType13, ExecButType13, EscFunction,
    NULL, NULL,
  },
  {  DateTimeTimeItem, TimeTxt, 8, (L_+D_+U_+R_+MORE_),
    DateTimeTimeSelect,
    LeftButType13, UpButType13,
    DownButType13, RightButType13, ExecButType13, EscFunction,
    NULL, NULL,
  },
  {  DateTimeRefItem, ReferenceTxt, 8, (L_+D_+U_+R_+MORE_),
    DateTimeRefSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  DateTimeOffsetItem, OffsetTxt, 8, (L_+D_+U_+R_+MORE_),
    DateTimeOffsetSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM download_submenu = {
  NA, TimeTxt, 8, NONE_,
  DownloadTestSPG,
  UpToDownload, UpToDownload,
  UpToDownload, UpToDownload, ExecButType3, EscFunction,
  NULL, NULL
};

code struct MENU_ITEM download_2ndmenu = {
  NA, TimeTxt, 8, NONE_,
  DownloadFromSPG,
  LeftButType10, UpButType10,
  NONE, RightButType10, ExecButType10, EscFunction,
  NULL, NULL
};

code struct MENU_ITEM RS232_menu[RS232Max] = {
  {  RS232BaudrateItem, RS232BaudrateTxt, 8, (L_+D_+U_+R_+MORE_),
    RS232MenuShow,
    ItemLeft, UpToConfig, RS232Down, ItemRight, NONE, NONE,
    &RS232_menu[1], &RS232_menu[3]
  },
  {  RS232DatabitItem, RS232DatabitTxt, 19, (L_+D_+U_+R_+MORE_),
    RS232MenuShow,
    ItemLeft, UpToConfig, RS232Down, ItemRight, NONE, NONE,
    &RS232_menu[2], &RS232_menu[0]
  },
  {  RS232ParityItem, RS232ParityTxt, 29, (L_+D_+U_+R_+MORE_),
    RS232MenuShow,
    ItemLeft, UpToConfig, RS232Down, ItemRight, NONE, NONE,
    &RS232_menu[3], &RS232_menu[1]
  },
  {  RS232HandshakeItem, RS232HandshakeTxt, 8, (L_+D_+U_+R_+MORE_),
    RS232MenuShow,
    ItemLeft, UpToConfig, RS232Down, ItemRight, NONE, NONE,
    &RS232_menu[0], &RS232_menu[2]
  }
};

code struct MENU_ITEM RS232_submenu[RS232Max] = {
  {  RS232BaudrateItem, RS232BaudrateTxt, NA, NA,
    RS232BaudrateSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  RS232DatabitItem, RS232DatabitTxt, NA, NA,
    RS232DatabitSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  RS232ParityItem, RS232ParityTxt, NA, NA,
    RS232ParitySelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  },
  {  RS232HandshakeItem, RS232HandshakeTxt, NA, NA,
    RS232HandshakeSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM diagnose_menu[DiagnoseMax] = {
  {  DiagMainItem, MainTxt, 8, (L_+U_+R_+E_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
    &diagnose_menu[1], &diagnose_menu[7]
  },
  {  DiagOptionsItem, OptionsTxt, 15, (L_+U_+R_+E_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
    &diagnose_menu[2], &diagnose_menu[0]
  },
  {  DiagRS232Item, RS232Txt, 25, (L_+U_+R_+E_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
    &diagnose_menu[3], &diagnose_menu[1]
  },
  {  DiagDisplayItem, DisplayTxt, 8, (L_+U_+R_+E_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
    &diagnose_menu[4], &diagnose_menu[2]
  },
  {  DiagKeyboardItem, KeyboardSLTxt, 18, (L_+U_+R_+E_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
    &diagnose_menu[5], &diagnose_menu[3]
  },
  {  DiagMemItem, MemoryTxt, 29, (L_+U_+R_+E_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
    &diagnose_menu[6], &diagnose_menu[4]
  },
  {  DiagConfigItem, ConfigurationTxt, 8, (L_+U_+R_+E_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
    &diagnose_menu[7], &diagnose_menu[5]
  },
  {  DiagErrorQueueItem, ErrorQueueTxt, 24, (L_+U_+R_+E_+MORE_),
    DiagnoseMenuShow,
    ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
    &diagnose_menu[0], &diagnose_menu[6]
  },
};

code struct MENU_ITEM diagnose_submenu[DiagnoseMax] = {
  {  DiagMainItem, MainTxt, 0, U_,
    DiagnoseMainTest,
    NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
    NULL, NULL,
  },
  {  DiagOptionsItem, OptionsTxt, 0, U_,
    DiagnoseOptionsTest,
    NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
    NULL, NULL,
  },
  {  DiagRS232Item, RS232Txt, 0, U_,
    DiagnoseRS232Test,
    NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
    NULL, NULL,
  },
  {  DiagDisplayItem, DisplayTxt, 0, U_,
    DiagnoseDisplayTest,
    NONE, UpButType11, NONE, NONE, NONE, NONE,
    NULL, NULL,
  },
  {  DiagKeyboardItem, KeyboardTxt, 0, U_,
    DiagnoseKeyboardTest,
    NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
    NULL, NULL,
  },
  {  DiagMemItem, MemoryTxt, 0, U_,
    DiagnoseMemoryTest,
    NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
    NULL, NULL,
  },
  {  DiagConfigItem, ConfigurationTxt, 0, U_,
    DiagnoseConfigTest,
    LeftButType5, UpButType5,
    DownButType5, RightButType5, NONE, EscFunction,
    NULL, NULL,
  },
  {  DiagErrorQueueItem, ErrorQueueTxt, 0, U_,
    DiagnoseErrorQueueTest,
    LeftButType5, UpButType5,
    DownButType5, RightButType5, ExecButType5, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM reset_menu = {
  NA, NULL, 0, U_,
  DiagnoseErrorQueueReset,
  LeftButType10, UpButType10,
  NONE, RightButType10, ExecButType10, EscFunction,
  NULL, NULL,
};

code struct MENU_ITEM RS232local_menu = {
  NA, NULL, 0, U_,
  RS232GotoLocalMessage,
  LeftButType10, UpButType10,
  NONE, RightButType10, ExecButType10, EscFunction,
  NULL, NULL,
};

code struct MENU_ITEM test_menu = {
  0, TestTxt, 8, (L_+D_+U_+R_+MORE_),
  NONE,
  LeftButType5, UpButType5,
  DownButType5, RightButType5, ExecButType5, EscFunction,
  NULL, NULL
};


code struct MENU_ITEM time_menu[TimeMax] = {
  {  TimeTimeItem, TimeTxt, 8, (L_+D_+U_+R_+MORE_),
    TimeMenuShow,
    LeftButGeneric, UpButGeneric, DownButGeneric, RightButGeneric, NONE, NONE,
    &time_menu[1], &time_menu[1]
  },
  {  TimeRefItem, ReferenceTxt, 15, (L_+D_+U_+R_+MORE_),
    TimeMenuShow,
    LeftButGeneric, UpButGeneric, DownButGeneric, RightButGeneric, NONE, NONE,
    &time_menu[0], &time_menu[0]
  }
  
};


code struct MENU_ITEM time_submenu[TimeMax] = {
  {  TimeTimeItem, TimeTxt, 8, (L_+D_+U_+R_+MORE_),
    RTCTimeSelect,
    LeftButType13, UpButType13,
    DownButType13, RightButType13, ExecButType13, EscFunction,
    NULL, NULL,
  },
  {  TimeRefItem, ReferenceTxt, 8, (L_+D_+U_+R_+MORE_),
    LTC8620RefSelect,
    LeftButType1, UpButType1,
    DownButType1, RightButType1, ExecButType1, EscFunction,
    NULL, NULL,
  }
};

code struct MENU_ITEM LTC8620_menu[LTCMax] = {
  { LTCSelectItem, SystemTxt, 8, NONE_,
    LTC8620Select,
  LeftButGeneric, UpToBase,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCOffsetItem, DateTxt, 8, NONE_,
    LTCOffsetSet,
    LeftButGeneric, UpToBase,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCFormatItem, LTCFormatTxt, 8, NONE_,
    LTCFormatSet,
    LeftButGeneric, UpToBase,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCTimeItem, TimeTxt, 8, NONE_,
    LTC8620TimeSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCModeItem, LTCModeTxt, 8, NONE_,
    LTCModeSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCTimezoneItem, LTCTimezoneTxt, 8, NONE_,
    LTCTimezoneSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCDaylightItem, LTCDaylightTxt, 8, NONE_,
    LTCDaylightSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCStartItem, LTCStartTxt, 8, NONE_,
    LTCStartSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCEndItem, LTCEndTxt, 8, NONE_,
    LTCEndSet,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCConfirmDaylightItem, ConfirmTxt, 8, NONE_,
    LTCDaylightConfirm,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  
  { LTCSyncConfirmItem, ConfirmTxt, 8, NONE_,
    LTC8620SyncConfirm,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCMessageItem, LTCTxt, 8, NONE_,
    LTCMessageMenu,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  },
  { LTCSyncItem, ConfirmTxt, 8, NONE_,
    LTC8620Sync,
    LeftButGeneric, UpButGeneric,
    DownButGeneric, RightButGeneric, ExecButGeneric, EscFunction,
    NULL, NULL
  }
  
};


