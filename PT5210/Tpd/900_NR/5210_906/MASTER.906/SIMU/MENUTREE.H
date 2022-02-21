/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PT5210 VariTime digital sync generator, master					*/
/*	Module:		MENUTREE.H																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	950613																		*/
/*	Rev. date:	971112, KEn	DEV															*/
/*	Status:		Version 2.0																	*/
/*																									*/
/*	Changes:																						*/
/* 970203 Added a dummy Black Burst item												*/
/* 970131 Added diagnose items															*/
/***************************************************************************/

#define StatusMax				6		// Max number of STATUS-DISPLAYs
#define BaseMax				9		// Max. # of items in the BASE MENU

#define BBMax   				4		// Max. # of items in BLACK-BB SUBMENU
#define SBMax					6		// Max. # of items in BLACK-SB SUBMENU

#define AnlSigMax				5		// Max. # of items in ANALOG-SIGNAL MENU
#define SDISigMax				7		// Max. # of items in SDI-SIGNAL MENU
#define AES_EBUMax			3		// Max. # of items in AES-EBU MENU
#define PresetMax				3		// Max. # of items in PRESET MENU
#define ConfigMax				6		// Max. # of items in CONFIG MENU

#define RS232Max				4		// Max. # of items in the RS232 SUBMENU
#define DiagnoseMax			9		// Max. # of items in the DIAGNOSE SUBMENU


// Definition of item names in the different menus

enum StatusDispItems {
	StatusPresetItem = 0, StatusGenlockItem, StatusAnalogSignalItem,
	StatusSDISignalItem, StatusAudioItem, StatusErrorItem };

enum BaseNdxItems	{
	AnlBlackItem = 0, SDIBlackItem, AnalogSignalItem, SDISignalItem,\
	AES_EBUItem, GenlockItem, PresetItem, ConfigItem, TestItem
};

enum BlkNdxItems {
	BB1Item = 0, BB2Item, BB3Item, BB4Item, BB5Item, BB6Item, BB7Item, BB8Item
};

enum BBNdxItems {
	BBSystemItem = 0, BBTimeItem, BBScHPhaseItem, BBCopyItem
};

enum SDIBlkNdxItems {
	SB34Item = 0, SB56Item, SB78Item
};

enum SBNdxItems	{
	SBPatternItem = 0, SBSystemItem, SBEDHItem, SBAudioItem, SBTimeItem,\
	SBCopyItem
};

enum AnalogNdxItems	{
	AnalogPatternItem = 0, AnalogTextItem, AnalogSystemItem,	AnalogTimeItem,
	AnalogScHPhaseItem
};

enum SDINdxItems	{
	SDIPatternItem = 0, SDITextItem, SDISystemItem, SDIEDHItem,
	SDIAudioSignalItem, SDIAudioLevelItem, SDITimeItem
};

enum AES_EBUItems {
	AES_EBUSignalItem = 0, AES_EBULevelItem, AES_EBUTimingItem
};

enum GenlockItems {
	GenlockInputItem = 0, GenlockSystemItem, GenlockTimeItem
};

enum PresetItems {
	PresetRecallItem = 0, PresetStoreItem, PresetNameItem
};

enum ConfigItems {
	ConfigLockItem = 0, ConfigAutoESCItem, ConfigLCDContItem,\
	ConfigDownloadItem, ConfigRS232Item, ConfigDiagnoseItem
};

enum RS232Items {
	RS232BaudrateItem = 0, RS232DatabitItem, RS232ParityItem,\
	RS232HandshakeItem
};

enum DiagnoseItems {
	DiagMainItem = 0, DiagOptionsItem, DiagRS232Item, DiagDisplayItem,\
	DiagKeyboardItem, DiagMemItem, DiagConfigItem, DiagErrorQueueItem
};

extern code struct MENU_ITEM status_menu[StatusMax];

extern code struct MENU_ITEM base_menu[BaseMax];

extern code struct MENU_ITEM anlblk_menu[NoOfAnlBlkUnits];
extern code struct MENU_ITEM BB_menu[BBMax];
extern code struct MENU_ITEM BB_submenu[BBMax];

extern code struct MENU_ITEM SDIblk_menu[NoOfSDIBlkUnits];
extern code struct MENU_ITEM SB_menu[SBMax];
extern code struct MENU_ITEM SB_submenu[SBMax];

extern code struct MENU_ITEM anlsig_menu[AnlSigMax];
extern code struct MENU_ITEM anlsig_submenu[AnlSigMax];

extern code struct MENU_ITEM SDIsig_menu[SDISigMax];
extern code struct MENU_ITEM SDIsig_submenu[SDISigMax];

extern code struct MENU_ITEM AES_EBU_menu[AES_EBUMax];
extern code struct MENU_ITEM AES_EBU_submenu[AES_EBUMax];

extern code struct MENU_ITEM genlock_menu;
extern code struct MENU_ITEM genlocksys_menu;
extern code struct MENU_ITEM genlocktime_menu;

extern code struct MENU_ITEM config_menu[ConfigMax];
extern code struct MENU_ITEM config_submenu[ConfigMax];

extern code struct MENU_ITEM preset_menu[PresetMax];
extern code struct MENU_ITEM preset_submenu[PresetMax];

extern code struct MENU_ITEM download_submenu;
extern code struct MENU_ITEM download_2ndmenu;

extern code struct MENU_ITEM RS232_menu[RS232Max];
extern code struct MENU_ITEM RS232_submenu[RS232Max];

extern code struct MENU_ITEM diagnose_menu[DiagnoseMax];
extern code struct MENU_ITEM diagnose_submenu[DiagnoseMax];

extern code struct MENU_ITEM reset_menu;

extern code struct MENU_ITEM RS232local_menu;

extern code struct MENU_ITEM test_menu;
