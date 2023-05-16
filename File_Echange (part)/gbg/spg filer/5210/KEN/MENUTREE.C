/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MENUTREE.C																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	960613																		*/
/*	Rev. date:	980915, KEn	DEV															*/
/*	Status:		Version 2.1																	*/
/*																									*/
/*	This module contains NO functions													*/
/*																									*/
/*	Changes:																						*/
/* 980915: More prikkerne i status er fjernet.										*/
/* 971016: Removed DiagnoseErrorQueueTxt												*/
/* 970724: Forgot NextItem & PrevItem in some structures							*/
/* 970723: Major change: Ptr->DispShow() & Ptr->DispUpdate has now been		*/
/*			  now been replaced by one function: Ptr->DispFunct();				*/
/* 970226: Changes to RS232 menu items													*/
/* 970203: Added a dummy Black Burst item												*/
/* 970201: Updated the diagnose menus													*/
/***************************************************************************/

/***************************************************************************/
/* Include files:																				*/

#include <xa.h>
#include <string.h>

#include "52sync.h"
#include "menu.h"
#include "menuhlp.h"
#include "menutree.h"
#include "text.h"
#include "message.h"
#include "util.h"

code struct MENU_ITEM status_menu[StatusMax] = {
	{	StatusPresetItem, NULL, 0, ( L_+D_+R_),
		StatusPresetUpdate,
		StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
		&status_menu[1], &status_menu[5],
	},
	{	StatusGenlockItem, NULL, 0, ( L_+D_+R_),
		StatusGenlockUpdate,
		StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
		&status_menu[2], &status_menu[0],
	},
	{	StatusAnalogSignalItem, NULL, 0, ( L_+D_+R_),
		StatusAnalogSignalUpdate,
		StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
		&status_menu[3], &status_menu[1],
	},
	{	StatusSDISignalItem, NULL, 0, ( L_+D_+R_),
		StatusSDISignalUpdate,
		StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
		&status_menu[4], &status_menu[2],
	},
	{	StatusAudioItem, NULL, 0, ( L_+D_+R_),
		StatusAudioUpdate,
		StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
		&status_menu[5], &status_menu[3],
	},
	{	StatusErrorItem, NULL, 0, ( L_+D_+R_),
		StatusErrorUpdate,
		StatusItemLeft, NONE, StatusDown, StatusItemRight, NONE, NONE,
		&status_menu[0], &status_menu[4],
	}
};

code struct MENU_ITEM base_menu[BaseMax] = {
	{	AnlBlackItem, ANLBlackTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE,
		&base_menu[1], &base_menu[8]
	},
	{	SDIBlackItem, SDIBlackTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE,NONE,
		&base_menu[2], &base_menu[0]
	},
	{	AnalogSignalItem, AnalogSignalTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE,
		&base_menu[3], &base_menu[1]
	},
	{	SDISignalItem, SDISignalTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE,
		&base_menu[4], &base_menu[2]
	},
	{	AES_EBUItem, AudioTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE,
		&base_menu[5], &base_menu[3]
	},
	{	GenlockItem, GenlockTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE,
		&base_menu[6], &base_menu[4]
	},
	{	PresetItem, PresetTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE,
		&base_menu[7], &base_menu[5]
	},
	{	ConfigItem, ConfigTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE,
		&base_menu[8], &base_menu[6]
	},
	{	TestItem, TestTxt, 0, ( L_+D_+U_+R_+MORE_),
		BaseMenuShow,
		BaseItemLeft, UpToStatus, BaseDown, BaseItemRight, NONE, NONE,
		&base_menu[0], &base_menu[7]
	}
};

code struct MENU_ITEM anlblk_menu[NoOfAnlBlkUnits] = {
	{	BB1Item, BB1Txt, NA, ( L_+D_+U_+R_),
		AnlBlkMenuShow,
		AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
		&anlblk_menu[1], &anlblk_menu[7]
	},
	{	BB2Item, BB2Txt, NA, ( L_+D_+U_+R_),
		AnlBlkMenuShow,
		AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
		&anlblk_menu[2], &anlblk_menu[0]
	},
	{	BB3Item, BB3Txt, NA, ( L_+D_+U_+R_),
		AnlBlkMenuShow,
		AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
		&anlblk_menu[3], &anlblk_menu[1]
	},
	{	BB4Item, BB4Txt, NA, ( L_+D_+U_+R_),
		AnlBlkMenuShow,
		AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
		&anlblk_menu[4], &anlblk_menu[2]
	},
	{	BB5Item, BB5Txt, NA, ( L_+D_+U_+R_),
		AnlBlkMenuShow,
		AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
		&anlblk_menu[5], &anlblk_menu[3]
	},
	{	BB6Item, BB6Txt, NA, ( L_+D_+U_+R_),
		AnlBlkMenuShow,
		AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
		&anlblk_menu[6], &anlblk_menu[4]
	},
	{	BB7Item, BB7Txt, NA, ( L_+D_+U_+R_),
		AnlBlkMenuShow,
		AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
		&anlblk_menu[7], &anlblk_menu[5]
	},
	{	BB8Item, BB8Txt, NA, ( L_+D_+U_+R_),
		AnlBlkMenuShow,
		AnlBlkItemLeft, UpToBase, AnlBlkDown, AnlBlkItemRight, NONE, NONE,
		&anlblk_menu[0], &anlblk_menu[6]
	}
};

code struct MENU_ITEM BB_menu[BBMax] = {
	{	BBSystemItem, SystemTxt, 8, ( L_+D_+U_+R_),
		BBMenuShow,
		ItemLeft, UpToAnlBlk, BBDown, ItemRight, NONE, NONE,
		&BB_menu[1], &BB_menu[3]
	},
	{	BBTimeItem, TimeTxt, 16, ( L_+D_+U_+R_),
		BBMenuShow,
		ItemLeft, UpToAnlBlk, BBDown, ItemRight, NONE, NONE,
		&BB_menu[2], &BB_menu[0]
	},
	{	BBScHPhaseItem, ScHPhaseTxt, 22, ( L_+D_+U_+R_),
		BBMenuShow,
		ItemLeft, UpToAnlBlk, BBDown, ItemRight, NONE, NONE,
		&BB_menu[3], &BB_menu[1]
	},
	{	BBCopyItem, CopyTxt, 33, ( L_+D_+U_+R_),
		BBMenuShow,
		ItemLeft, UpToAnlBlk, BBDown, ItemRight, NONE, NONE,
		&BB_menu[0], &BB_menu[2]
	}
};

code struct MENU_ITEM BB_submenu[BBMax] = {
	{	BBSystemItem, SystemTxt, 0, NA,
		BBSystemSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	BBTimeItem, TimeTxt, 0, NA,
		BBTimeSelect,
		LeftButType6, UpButType6,
		DownButType6, RightButType6, ExecButType6, EscFunction,
		NULL, NULL,
	},
	{	BBScHPhaseItem, ScHPhaseTxt, 0, NA,
		BBScHPhaseSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	BBCopyItem, CopyTxt, 0, NA,
		BBCopySelect,
		LeftButType3, UpButType3,
		DownButType3, RightButType3, ExecButType3, EscFunction,
		NULL, NULL,
	}
};

code struct MENU_ITEM SDIblk_menu[NoOfSDIBlkUnits] = {
	{	SB34Item, SB34Txt, NA, ( L_+D_+U_+R_),
		SDIBlkMenuShow,
		SDIBlkItemLeft, UpToBase, SDIBlkDown, SDIBlkItemRight, NONE, NONE,
		&SDIblk_menu[1], &SDIblk_menu[2]
	},
	{	SB56Item, SB56Txt, NA, ( L_+D_+U_+R_),
		SDIBlkMenuShow,
		SDIBlkItemLeft, UpToBase, SDIBlkDown, SDIBlkItemRight, NONE, NONE,
		&SDIblk_menu[2], &SDIblk_menu[0]
	},
	{	SB78Item, SB78Txt, NA, ( L_+D_+U_+R_),
		SDIBlkMenuShow,
		SDIBlkItemLeft, UpToBase, SDIBlkDown, SDIBlkItemRight, NONE, NONE,
		&SDIblk_menu[0], &SDIblk_menu[1]
	}
};

code struct MENU_ITEM SB_menu[SBMax] = {
	{	SBPatternItem, PatternTxt, 8, ( L_+D_+U_+R_+MORE_),
		SBMenuShow,
		ItemLeft, UpToSDIBlk, SBDown, ItemRight, NONE, NONE,
		&SB_menu[1], &SB_menu[5]
	},
	{	SBSystemItem, SystemTxt, 18, ( L_+D_+U_+R_+MORE_),
		SBMenuShow,
		ItemLeft, UpToSDIBlk, SBDown, ItemRight, NONE, NONE,
		&SB_menu[2], &SB_menu[0]
	},
	{	SBEDHItem, EDHTxt, 27, ( L_+D_+U_+R_+MORE_),
		SBMenuShow,
		ItemLeft, UpToSDIBlk, SBDown, ItemRight, NONE, NONE,
		&SB_menu[3], &SB_menu[1]
	},
	{	SBAudioItem, SBAudioTxt, 8, ( L_+D_+U_+R_+MORE_),
		SBMenuShow,
		ItemLeft, UpToSDIBlk, SBDown, ItemRight, NONE, NONE,
		&SB_menu[4], &SB_menu[2]
	},
	{	SBTimeItem, TimeTxt, 20, ( L_+D_+U_+R_+MORE_),
		SBMenuShow,
		ItemLeft, UpToSDIBlk, SBDown, ItemRight, NONE, NONE,
		&SB_menu[5], &SB_menu[3]
	},
	{	SBCopyItem, CopyTxt, 27, ( L_+D_+U_+R_+MORE_),
		SBMenuShow,
		ItemLeft, UpToSDIBlk, SBDown, ItemRight, NONE, NONE,
		&SB_menu[0], &SB_menu[4]
	}
};

code struct MENU_ITEM SB_submenu[SBMax] = {
	{	SBPatternItem, PatternTxt, NA, NA,
		SBPatternSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SBSystemItem, SystemTxt, NA, NA,
		SBSystemSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SBEDHItem, EDHTxt, 0, NA,
		SBEDHSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SBAudioItem, SBAudioTxt, NA, NA,
		SBAudioSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SBTimeItem, TimeTxt, NA, NA,
		SBTimeSelect,
		LeftButType6, UpButType6,
		DownButType6, RightButType6, ExecButType6, EscFunction,
		NULL, NULL,
	},
	{	SBCopyItem, CopyTxt, NA, NA,
		SBCopySelect,
		LeftButType3, UpButType3,
		DownButType3, RightButType3, ExecButType3, EscFunction,
		NULL, NULL,
	}
};

code struct MENU_ITEM anlsig_menu[AnlSigMax] = {
	{	AnalogPatternItem, PatternTxt, 8, ( L_+D_+U_+R_+MORE_),
		AnalogMenuShow,
		ItemLeft, UpToBase, AnalogDown, ItemRight, NONE, NONE,
		&anlsig_menu[1], &anlsig_menu[4]
	},
	{	AnalogTextItem, TextTxt, 18, ( L_+D_+U_+R_+MORE_),
		AnalogMenuShow,
		ItemLeft, UpToBase, AnalogDown, ItemRight, NONE, NONE,
		&anlsig_menu[2], &anlsig_menu[0]
	},
	{	AnalogSystemItem, SystemTxt, 25, ( L_+D_+U_+R_+MORE_),
		AnalogMenuShow,
		ItemLeft, UpToBase, AnalogDown, ItemRight, NONE, NONE,
		&anlsig_menu[3], &anlsig_menu[1]
	},
	{	AnalogTimeItem, TimeTxt, 8, ( L_+D_+U_+R_+MORE_),
		AnalogMenuShow,
		ItemLeft, UpToBase, AnalogDown, ItemRight, NONE, NONE,
		&anlsig_menu[4], &anlsig_menu[2]
	},
	{	AnalogScHPhaseItem, ScHPhaseTxt, 15, ( L_+D_+U_+R_+MORE_),
		AnalogMenuShow,
		ItemLeft, UpToBase, AnalogDown, ItemRight, NONE, NONE,
		&anlsig_menu[0], &anlsig_menu[3]
	}
};

code struct MENU_ITEM anlsig_submenu[AnlSigMax] = {
	{	AnalogPatternItem, PatternTxt, 8, ( L_+D_+U_+R_+MORE_),
		AnalogPatternSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	AnalogTextItem, CopyTxt, 8, ( L_+D_+U_+R_+MORE_),
		AnalogTextSelect,
		LeftButType7, UpButType7,
		DownButType7, RightButType7, ExecButType7, EscFunction,
		NULL, NULL,
	},
	{	AnalogSystemItem, SystemTxt, 8, ( L_+D_+U_+R_+MORE_),
		AnalogSystemSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	AnalogTimeItem, TimeTxt, 8, ( L_+D_+U_+R_+MORE_),
		AnalogTimeSelect,
		LeftButType6, UpButType6,
		DownButType6, RightButType6, ExecButType6, EscFunction,
		NULL, NULL,
	},
	{	AnalogScHPhaseItem, ScHPhaseTxt, 8, ( L_+D_+U_+R_+MORE_),
		AnalogScHPhaseSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	}
};

code struct MENU_ITEM SDIsig_menu[SDISigMax] = {
	{	SDIPatternItem, PatternTxt, 8, ( L_+D_+U_+R_+MORE_),
		SDIMenuShow,
		ItemLeft, UpToBase, SDIDown, ItemRight, NONE, NONE,
		&SDIsig_menu[1], &SDIsig_menu[6]
	},
	{	SDITextItem, TextTxt, 18, ( L_+D_+U_+R_+MORE_),
		SDIMenuShow,
		ItemLeft, UpToBase, SDIDown, ItemRight, NONE, NONE,
		&SDIsig_menu[2], &SDIsig_menu[0]
	},
	{	SDISystemItem, SystemTxt, 25, ( L_+D_+U_+R_+MORE_),
		SDIMenuShow,
		ItemLeft, UpToBase, SDIDown, ItemRight, NONE, NONE,
		&SDIsig_menu[3], &SDIsig_menu[1]
	},
	{	SDIEDHItem, EDHTxt, 8, ( L_+D_+U_+R_+MORE_),
		SDIMenuShow,
		ItemLeft, UpToBase, SDIDown, ItemRight, NONE, NONE,
		&SDIsig_menu[4], &SDIsig_menu[2]
	},
	{	SDIAudioSignalItem, SDIAudioSignalTxt, 15, ( L_+D_+U_+R_+MORE_),
		SDIMenuShow,
		ItemLeft, UpToBase, SDIDown, ItemRight, NONE, NONE,
		&SDIsig_menu[5], &SDIsig_menu[3]
	},
	{	SDIAudioLevelItem, SDIAudioLevelTxt, 8, ( L_+D_+U_+R_+MORE_),
		SDIMenuShow,
		ItemLeft, UpToBase, SDIDown, ItemRight, NONE, NONE,
		&SDIsig_menu[6], &SDIsig_menu[4]
	},
	{	SDITimeItem, TimeTxt, 27, ( L_+D_+U_+R_+MORE_),
		SDIMenuShow,
		ItemLeft, UpToBase, SDIDown, ItemRight, NONE, NONE,
		&SDIsig_menu[0], &SDIsig_menu[5]
	}
};

code struct MENU_ITEM SDIsig_submenu[SDISigMax] = {
	{	SDIPatternItem, PatternTxt, 8, NA,
		SDIPatternSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SDITextItem, TextTxt, 8, NA,
		SDITextSelect,
		LeftButType7, UpButType7,
		DownButType7, RightButType7, ExecButType7, EscFunction,
		NULL, NULL,
	},
	{	SDISystemItem, SystemTxt, 8, NA,
		SDISystemSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SDIEDHItem, EDHTxt, 8, NA,
		SDIEDHSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SDIAudioSignalItem, SDIAudioSignalTxt, 15, NA,
		SDIAudioSignalSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SDIAudioLevelItem, SDIAudioLevelTxt, 8, NA,
		SDIAudioLevelSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	SDITimeItem, TimeTxt, 8, NA,
		SDITimeSelect,
		LeftButType6, UpButType6,
		DownButType6, RightButType6, ExecButType6, EscFunction,
		NULL, NULL,
	}
};

code struct MENU_ITEM AES_EBU_menu[AES_EBUMax] = {
	{	AES_EBUSignalItem, SignalTxt, 8, ( L_+D_+U_+R_),
		AES_EBUMenuShow,
		ItemLeft, UpToBase, AES_EBUDown, ItemRight, NONE, NONE,
		&AES_EBU_menu[1], &AES_EBU_menu[2]
	},
	{	AES_EBULevelItem, LevelTxt, 17, ( L_+U_+R_),
		AES_EBUMenuShow,
		ItemLeft, UpToBase, AES_EBUDown, ItemRight, NONE, NONE,
		&AES_EBU_menu[2], &AES_EBU_menu[0]
	},
	{	AES_EBUTimingItem, TimingTxt, 25, ( L_+U_+R_),
		AES_EBUMenuShow,
		ItemLeft, UpToBase, AES_EBUDown, ItemRight, NONE, NONE,
		&AES_EBU_menu[0], &AES_EBU_menu[1]
	}
};

code struct MENU_ITEM AES_EBU_submenu[AES_EBUMax] = {
	{	AES_EBUSignalItem, SignalTxt, 8, NA,
		AES_EBUSignalSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	AES_EBULevelItem, LevelTxt, 8, NA,
		AES_EBULevelSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	AES_EBUTimingItem, TimingTxt, 8, NA,
		AES_EBUTimingSelect,
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

code struct MENU_ITEM genlocktime_menu = {
	NA, TimeTxt, 8, NONE_,
	GenlockTimeSelect,
	LeftButType6, UpButType6,
	DownButType6, RightButType6, ExecButType6, EscFunction,
	NULL, NULL
};

code struct MENU_ITEM preset_menu[PresetMax] = {
	{	PresetRecallItem, RecallTxt, 8, ( L_+D_+U_+R_),
		PresetMenuShow,
		ItemLeft, UpToBase, PresetDown, ItemRight, NONE, NONE,
		&preset_menu[1], &preset_menu[2]
	},
	{	PresetStoreItem, StoreTxt, 17, ( L_+D_+U_+R_),
		PresetMenuShow,
		ItemLeft, UpToBase, PresetDown, ItemRight, NONE, NONE,
		&preset_menu[2], &preset_menu[0]
	},
	{	PresetNameItem, NameTxt, 25, ( L_+D_+U_+R_),
		PresetMenuShow,
		ItemLeft, UpToBase, PresetDown, ItemRight, NONE, NONE,
		&preset_menu[0], &preset_menu[1]
	}
};

code struct MENU_ITEM preset_submenu[PresetMax] = {
	{	PresetRecallItem, RecallTxt, 8, NA,
		PresetRecallSelect,
		LeftButType3, UpButType3,
		DownButType3, RightButType3, ExecButType3, EscFunction,
		NULL, NULL,
	},
	{	PresetStoreItem, StoreTxt, 8, NA,
		PresetStoreSelect,
		LeftButType3, UpButType3,
		DownButType3, RightButType3, ExecButType3, EscFunction,
		NULL, NULL,
	},
	{	PresetNameItem, NameTxt, 8, NA,
		PresetNameSelect,
		LeftButType8, UpButType8,
		DownButType8, RightButType8, ExecButType8, EscFunction,
		NULL, NULL,
	}
};

code struct MENU_ITEM config_menu[ConfigMax] = {
	{	ConfigLockItem, ConfigLockTxt, 8, ( L_+D_+U_+R_+MORE_),
		ConfigMenuShow,
		ItemLeft, UpToBase, ConfigDown, ItemRight, NONE, NONE,
		&config_menu[1], &config_menu[5]
	},
	{	ConfigAutoESCItem, ConfigAutoESCTxt, 14, ( L_+D_+U_+R_+MORE_),
		ConfigMenuShow,
		ItemLeft, UpToBase, ConfigDown, ItemRight, NONE, NONE,
		&config_menu[2], &config_menu[0]
	},
	{	ConfigLCDContItem, ConfigLCDContTxt, 24, ( L_+U_+D_+R_+MORE_),
		ConfigMenuShow,
		ItemLeft, UpToBase, ConfigDown, ItemRight, NONE, NONE,
		&config_menu[3], &config_menu[1]
	},
	{	ConfigDownloadItem, ConfigDownloadTxt, 8, ( L_+U_+D_+R_+MORE_),
		ConfigMenuShow,
		ItemLeft, UpToBase, ConfigDown, ItemRight, NONE, NONE,
		&config_menu[4], &config_menu[2]
	},
	{	ConfigRS232Item, RS232Txt, 18, ( L_+D_+U_+R_+MORE_),
		ConfigMenuShow,
		ItemLeft, UpToBase, ConfigDown, ItemRight, NONE, NONE,
		&config_menu[5], &config_menu[3]
	},
	{	ConfigDiagnoseItem, ConfigDiagnoseTxt, 25, ( L_+D_+U_+R_+MORE_),
		ConfigMenuShow,
		ItemLeft, UpToBase, ConfigDown, ItemRight, NONE, NONE,
		&config_menu[0], &config_menu[4]
	}
};

code struct MENU_ITEM config_submenu[ConfigMax] = {
	{	ConfigLockItem, ConfigLockTxt, 8, ( L_+U_+R_+MORE_),
		ConfigLockSelect,
		LeftButType2, UpButType2,
		DownButType2, RightButType2, ExecButType2, EscFunction,
		NULL, NULL,
	},
	{	ConfigAutoESCItem, ConfigAutoESCTxt, 8, ( L_+U_+R_+MORE_),
		ConfigAutoESCSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	ConfigLCDContItem, ConfigLCDContTxt, 8, ( L_+U_+R_+MORE_),
		ConfigLCDContSelect,
		LeftButType4, UpButType4,
		DownButType4, RightButType4, NONE, EscFunction,
		NULL, NULL,
	},
	{ ConfigDownloadItem, TimeTxt, 8, NONE_,
		DownloadTypeSelect,
		LeftButType3, UpButType3,
		DownButType3, RightButType3, ExecButType3, EscFunction,
		NULL, NULL
	},
	{	ConfigRS232Item, NULL, 0, NA,
		NONE, NONE, NONE, NONE, NONE, NONE, NONE,
		NULL, NULL,
	},
	{	ConfigDiagnoseItem, ConfigDiagnoseTxt, 17, ( L_+U_+R_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, NONE, NONE, ItemRight, NONE, NONE,
		NULL, NULL,
	},
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
	{	RS232BaudrateItem, RS232BaudrateTxt, 8, ( L_+D_+U_+R_+MORE_),
		RS232MenuShow,
		ItemLeft, UpToConfig, RS232Down, ItemRight, NONE, NONE,
		&RS232_menu[1], &RS232_menu[3]
	},
	{	RS232DatabitItem, RS232DatabitTxt, 19, ( L_+D_+U_+R_+MORE_),
		RS232MenuShow,
		ItemLeft, UpToConfig, RS232Down, ItemRight, NONE, NONE,
		&RS232_menu[2], &RS232_menu[0]
	},
	{	RS232ParityItem, RS232ParityTxt, 29, ( L_+D_+U_+R_+MORE_),
		RS232MenuShow,
		ItemLeft, UpToConfig, RS232Down, ItemRight, NONE, NONE,
		&RS232_menu[3], &RS232_menu[1]
	},
	{	RS232HandshakeItem, RS232HandshakeTxt, 8, ( L_+D_+U_+R_+MORE_),
		RS232MenuShow,
		ItemLeft, UpToConfig, RS232Down, ItemRight, NONE, NONE,
		&RS232_menu[0], &RS232_menu[2]
	}
};

code struct MENU_ITEM RS232_submenu[RS232Max] = {
	{	RS232BaudrateItem, RS232BaudrateTxt, NA, NA,
		RS232BaudrateSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	RS232DatabitItem, RS232DatabitTxt, NA, NA,
		RS232DatabitSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	RS232ParityItem, RS232ParityTxt, NA, NA,
		RS232ParitySelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	},
	{	RS232HandshakeItem, RS232HandshakeTxt, NA, NA,
		RS232HandshakeSelect,
		LeftButType1, UpButType1,
		DownButType1, RightButType1, ExecButType1, EscFunction,
		NULL, NULL,
	}
};

code struct MENU_ITEM diagnose_menu[DiagnoseMax] = {
	{	DiagMainItem, MainTxt, 8, ( L_+U_+R_+E_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
		&diagnose_menu[1], &diagnose_menu[7]
	},
	{	DiagOptionsItem, OptionsTxt, 15, ( L_+U_+R_+E_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
		&diagnose_menu[2], &diagnose_menu[0]
	},
	{	DiagRS232Item, RS232Txt, 25, ( L_+U_+R_+E_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
		&diagnose_menu[3], &diagnose_menu[1]
	},
	{	DiagDisplayItem, DisplayTxt, 8, ( L_+U_+R_+E_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
		&diagnose_menu[4], &diagnose_menu[2]
	},
	{	DiagKeyboardItem, KeyboardSLTxt, 18, ( L_+U_+R_+E_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
		&diagnose_menu[5], &diagnose_menu[3]
	},
	{	DiagMemItem, MemoryTxt, 29, ( L_+U_+R_+E_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
		&diagnose_menu[6], &diagnose_menu[4]
	},
	{	DiagConfigItem, ConfigurationTxt, 8, ( L_+U_+R_+E_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
		&diagnose_menu[7], &diagnose_menu[5]
	},
	{	DiagErrorQueueItem, ErrorQueueTxt, 24, ( L_+U_+R_+E_+MORE_),
		DiagnoseMenuShow,
		ItemLeft, UpToConfig, NONE, ItemRight, DiagnoseDown, NONE,
		&diagnose_menu[0], &diagnose_menu[6]
	},
};

code struct MENU_ITEM diagnose_submenu[DiagnoseMax] = {
	{	DiagMainItem, MainTxt, 0, U_,
		DiagnoseMainTest,
		NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
		NULL, NULL,
	},
	{	DiagOptionsItem, OptionsTxt, 0, U_,
		DiagnoseOptionsTest,
		NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
		NULL, NULL,
	},
	{	DiagRS232Item, RS232Txt, 0, U_,
		DiagnoseRS232Test,
		NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
		NULL, NULL,
	},
	{	DiagDisplayItem, DisplayTxt, 0, U_,
		DiagnoseDisplayTest,
		NONE, UpButType11, NONE, NONE, NONE, NONE,
		NULL, NULL,
	},
	{	DiagKeyboardItem, KeyboardTxt, 0, U_,
		DiagnoseKeyboardTest,
		NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
		NULL, NULL,
	},
	{	DiagMemItem, MemoryTxt, 0, U_,
		DiagnoseMemoryTest,
		NONE, UpButType11, NONE, NONE, ExecButType11, NONE,
		NULL, NULL,
	},
	{	DiagConfigItem, ConfigurationTxt, 0, U_,
		DiagnoseConfigTest,
		LeftButType5, UpButType5,
		DownButType5, RightButType5, NONE, EscFunction,
		NULL, NULL,
	},
	{	DiagErrorQueueItem, ErrorQueueTxt, 0, U_,
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
	0, TestTxt, 8, ( L_+D_+U_+R_+MORE_),
	NONE,
	LeftButType5, UpButType5,
	DownButType5, RightButType5, ExecButType5, EscFunction,
	NULL, NULL
};

