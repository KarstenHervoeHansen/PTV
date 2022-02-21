/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2003. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAIN_PNL                        1
#define  MAIN_PNL_EXIT_BTN               2       /* callback function: ExitBtn */
#define  MAIN_PNL_GPSRECEIVER            3
#define  MAIN_PNL_HP53132EXTREF          4
#define  MAIN_PNL_START_TESTS            5       /* callback function: do_start_tests */
#define  MAIN_PNL_PROGRESS               6
#define  MAIN_PNL_NAME1                  7
#define  MAIN_PNL_NAME2                  8
#define  MAIN_PNL_DEC1                   9
#define  MAIN_PNL_GPS_TIMER              10      /* callback function: GPSXLDCCallBack */
#define  MAIN_PNL_GPSCHECKNUMBER         11
#define  MAIN_PNL_SWVER                  12


     /* Menu Bars, Menus, and Menu Items: */

#define  MAINMENU                        1
#define  MAINMENU_BB                     2
#define  MAINMENU_BB_BB1_TOTAL           3       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB2_TOTAL           4       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB3_TOTAL           5       /* callback function: BBCallBack */
#define  MAINMENU_COLORBAR               6
#define  MAINMENU_COLORBAR_LEVEL         7       /* callback function: AnalogTPGCallBack */
#define  MAINMENU_COLORBAR_PHASE         8       /* callback function: AnalogTPGCallBack */
#define  MAINMENU_COLORBAR_PATTERN       9       /* callback function: AnalogTPGCallBack */
#define  MAINMENU_SDI                    10
#define  MAINMENU_SDI_JITAMSP            11      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_RLOSS              12      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_EMBAUDIO           13      /* callback function: SDITestCallBack */
#define  MAINMENU_SPG                    14
#define  MAINMENU_SPG_INT13REF           15      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_HPHGENL            16      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_TESTGENLOCK        17      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_INT27REF           18      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_SPGSEP             19
#define  MAINMENU_SPG_RESETSPG           20      /* callback function: SPGTestCallBack */
#define  MAINMENU_AUDIO                  21
#define  MAINMENU_AUDIO_AES              22      /* callback function: AudioTestCallBack */
#define  MAINMENU_AUDIO_WCLK             23      /* callback function: AudioTestCallBack */
#define  MAINMENU_AUDIO_AUDIOSEP         24
#define  MAINMENU_AUDIO_ANCAL            25      /* callback function: AudioTestCallBack */
#define  MAINMENU_AUDIO_ANLEVMEAS        26      /* callback function: AudioTestCallBack */
#define  MAINMENU_AUDIO_ANFREQMEAS       27      /* callback function: AudioTestCallBack */
#define  MAINMENU_DIVERSE                28
#define  MAINMENU_DIVERSE_POWERCHECK33   29      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_POWERCHECK50   30      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_POWERCHECK50M  31      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP1           32
#define  MAINMENU_DIVERSE_SYS5201        33      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP2           34
#define  MAINMENU_DIVERSE_CALDATA        35      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_CALFILES       36      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP3           37
#define  MAINMENU_DIVERSE_SELGENLSIGNAL  38      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP4           39
#define  MAINMENU_DIVERSE_WPOFF          40      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_WPON           41      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP5           42
#define  MAINMENU_DIVERSE_VLM            43      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_AUDIOLEVEL     44      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_HP53132        45      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP6           46
#define  MAINMENU_DIVERSE_INSTR          47      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP7           48
#define  MAINMENU_DIVERSE_EXITPROGRAM    49      /* callback function: DiverseCallBack */
#define  MAINMENU_COMMAND                50
#define  MAINMENU_COMMAND_BB1PAL         51      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_BB1NTSC        52      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_BB2PAL         53      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_BB2NTSC        54      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_BB3PAL         55      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_BB3NTSC        56      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_TSGPAL         57      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_TSGNTSC        58      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_GENLPAL        59      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_GENLNTSC       60      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_GEN10MHZ       61      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_GENLINT        62      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_AUDAES48       63      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_AUDAES441      64      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_AUDANA_1K_6DB  65      /* callback function: CommandCallback */
#define  MAINMENU_COMMAND_AUDANA_500_0DB 66      /* callback function: CommandCallback */


     /* Callback Prototypes: */ 

void CVICALLBACK AnalogTPGCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK AudioTestCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK BBCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK CommandCallback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK DiverseCallBack(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK do_start_tests(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitBtn(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GPSXLDCCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK SDITestCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK SPGTestCallBack(int menubar, int menuItem, void *callbackData, int panel);


#ifdef __cplusplus
    }
#endif
