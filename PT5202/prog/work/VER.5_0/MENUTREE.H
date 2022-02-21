/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:    MENUTREE.H                                  */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  980115, file is based on PT5210/MENUTREE.H            */
/*  Rev. date:  000508,  KEn, DEV                              */
/*  Status:    Version 1.1                                  */
/***************************************************************************/
#include "tables.h"

typedef struct MENU_ITEM 
  {
  UC ItemNumber;

  code char* ItemTxt;
  UC ItemPos;
  UC ItemConfig;

  void ( *DispFunct)(void);

  void ( *LeftBut)(void);
  void ( *UpBut)(void);
  void ( *DownBut)(void);
  void ( *RightBut)(void);

  void ( *ExecBut)(void);
  void ( *EscapeFct)(void);

  code struct MENU_ITEM* NextItem;
  code struct MENU_ITEM* PrevItem;
  };

#define NONE_           0x00
#define L_              0x01
#define D_              0x02
#define R_              0x04
#define U_              0x08
#define E_              0x10
#define P_              0x20
#define MORE_           0x40
#define F_              0x80                            // Free to use

#define ScHPhaseLow     -179
#define ScHPhaseHigh     180
#define NA                 0
 
#define startOfArrowsField 36


enum GenlockSystems
  {
  INTERNAL = 0, GPAL, GNTSC, GMHZ
  };

enum SdiSystems
  {
  SPAL = 0, SDUMMY, SNTSC7, SNTSC0
  };

enum FactorySystems
  {
  FPAL = 0, FNTSC7, FNTSC0
  };

enum BaseItems 
  {
  GenlockItem = 0, AnlBlkItem, SdiItem, AUDIOItem, PRESETItem, CALIBRATIONItem, SYSTEMItem
  };

enum GenlockItems
  {
  GenlockSystemItem = 0, GenlockTimingItem
  };
  
enum AnlBlkItems 
  {
  BB1Item = 0, BB2Item, BB3Item
  };

enum BBItems 
  {
  BBSystemItem = 0, BBTimingItem, BBScHPhaseItem
  };

enum SdiItems
  {
  SdiSystemItem = 0, SdiTimingItem, SdiPatternItem, SdiEmbeddedItem, SdiAnalogItem, SdiTextItem, SdiMovementItem
  };
  
#if 0
enum SdiEmbeddedItems
  {
  Off = 0, Silence, Stereo1khz
  };  
#endif
enum SDITextItems
  {
  SDITextEditItem = 0, SDITextMovementItem
  };


enum AudioItems
  {
  AnalogAudioItem = 0, DigitalAudioItem
  };
  
enum AnalogAudioItems
  {
  AnalogAudioSignalItem = 0, AnalogAudioLevelItem, AnalogAudioClickItem
  };  
  
enum DigitalAudioItems
  {
  DigitalAudioSignalItem = 0, DigitalAudioLevelItem, DigitalAudioTimingItem, DigitalAudioSystemItem, DigitalAudioFrequencyItem, DigitalAudioClickItem
  };

enum SaveRestoreItems
  {
  SaveItem = 0, RestoreItem
  };
  
enum PresetsItems
  {
  PresetRecallItem = 0, PresetStoreItem, PresetNameItem
  };
  
enum CalibraionItems
  {
  CalibrationMaxItem = 0, CalibrationMinItem, CalibrationAdjItem
  };
   
enum dir_commands
  {
  maxcmd = 0, mincmd, upcmd, downcmd
  };
  
enum status_items
  {
  StatusGenlockItem = 0, StatusSDIItem, StatusAudioItem, StatusPresetItem, StatusBBItem
  };

void AnlTimeAdjust(UC, long);
void SDITimeAdjust(UC, long);


void BaseMenuShow(void);
void BaseDown(void);
void AnlBlkMenuShow(void);
void UpToBase(void);
void ItemRight(void); 
void ItemLeft(void);

void AudioItemRight(void); 
void AudioItemLeft(void); 

void UpToAnlBlk(void);
void BBMenuShow(void);
void AnlBlkDown(void);
void BBDown(void);
void BBSystemSelect(void);
void BBTimingSelect(void);
void BBScHPhaseSelect(void);
void GenlockMenuShow(void);
void GenlockDown(void);
void GenlockSystemSelect(void);
void GenlockTimingSelect(void);

void SdiMenuShow(void);
void SdiDown(void);
void SdiSystemSelect(void);
void SdiTimingSelect(void);
void SdiPatternSelect(void);
void SdiEmbeddedSelect(void);
void SdiAnalogEdit(void);

void SDITextShow(void);
void UpToSDI(void);
void SDITextDown(void);
void SdiTextEdit(void);
void SdiMovementSelect(void);
void UpToSdi(void);
void SdiMovementSelect(void);
void SdiTextMenuShow(void);
void SdiTextDown(void);

void AudioMenuShow(void);
void AudioDown(void);
void AnalogAudioMenuShow(void);
void AnalogAudioDown(void);
void AnalogAudioClickSelect(void);
void AnalogAudioSignalSelect(void);
void AnalogAudioLevelSelect(void);
void DigitalAudioMenuShow(void);
void DigitalAudioDown(void);
void DigitalAudioClickSelect(void);
void DigitalAudioSignalSelect(void);
void DigitalAudioTimingSelect(void);
void DigitalAudioFrequencySelect(void);
void DigitalAudioSystemSelect(void);
void DigitalAudioLevelSelect(void);
void UpToAudio(void);
void PresetMenuShow(void);
void PresetDown(void);
void PresetRecallSelect(void);
void PresetStoreSelect(void);
void PresetNameSelect(void);
void SaveRestorePresetsMenuShow(void);
void SaveRestorePresetsDown(void);
void FactoryResetSystemSelect(void);
void CalibrationMenuShow(void);

void EscFunction(void);
void WriteType1_SAVE_ESC(char);
void UpButType1(void);
void DownButType1(void);
void LeftButType1(void);
void RightButType1(void);
void ExecButType1(void);
void WriteType3_OK_ESC( char FuncState);
void WriteType3_OK_ESC_JK(char FuncState);

void WriteTypeJK_ESC(char FuncState);
void UpButType3(void);
void DownButType3(void);
void LeftButType3(void);
void RightButType3(void);
void RightButTypeJK(void);
void LeftButTypeJK(void);
void ExecButType3(void);
void ExecButTypeJK(void); 

void WriteType6_SAVE_ESC(char);
void UpButType6(void);
void DownButType6(void);
void LeftButType6(void);
void RightButType6(void);
void ExecButType6(void);
void WriteType8_SAVE_ESC( char FuncState);
void UpButType8(void);
void DownButType8(void);
void LeftButType8(void);
void RightButType8(void);
void ExecButType8(void);
void UpButType12(void); 
void DownButType12(void); 
void LeftButType12(void); 
void RightButType12(void); 
void ExecButType12(void); 
void WriteType12_SAVE_ESC( char);
void WriteItemArrows( UC, UC);
void WriteArrowsField( UC);
void WriteItemBrackets( UC, UC); 
char* Mstrcpy( register char *, register code char *);
char* Mstrcat( register char*, register code char*);
size_t Mstrlen(register code char* s);

void AnlSamplesToFLT(UC, long, char*, int*, int*, float*); 
void SDISamplesToFLT(UC, long, char*, int*, int*, float*);

void WriteTimingFLT(char, int, int, float);
void menu_init(void);
void check_keys(void);
void empty(void);
char GetNextPresetASCII(UC, UC);
void spaces2zeros(char *);

//void copy_config(char *, char *, UC);


void ExecButTypeCal(void);
void RightButTypeCal(void);
void LeftButTypeCal(void);
void DownButTypeCal(void);
void UpButTypeCal(void);
void WriteTypeCal_SAVE_ESC(char);

void StatusGenlockShow(void);
void StatusSDIShow(void);
void StatusAudioShow(void);
void StatusPresetShow(void);
void StatusBBShow(void);
void StatusDown(void);
void UpToStatus(void);
void ret2status(void);
void  copy_presets(void);
void update_status(void);
void MMemcpy(void *, void *, UC);
