/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 1997. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  BASTOT_PNL                      1
#define  BASTOT_PNL_TXTMSG1              2

#define  MAIN_PNL                        2
#define  MAIN_PNL_EXIT_BTN               2       /* callback function: ExitBtn */
#define  MAIN_PNL_TOTBASIC_BTN           3       /* callback function: TotalBasicCallBack */
#define  MAIN_PNL_NAMETXT_2              4
#define  MAIN_PNL_NAMETXT_1              5
#define  MAIN_PNL_SWVER                  6

#define  SDITOT_PNL                      3
#define  SDITOT_PNL_TXTMSG1              2


     /* Menu Bars, Menus, and Menu Items: */

#define  MAINMENU                        1
#define  MAINMENU_BB                     2
#define  MAINMENU_BB_BB1_TOTAL           3       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB2_TOTAL           4       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB3_TOTAL           5       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB4_TOTAL           6       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB5_TOTAL           7       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB6_TOTAL           8       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB7_TOTAL           9       /* callback function: BBCallBack */
#define  MAINMENU_BB_BB8_TOTAL           10      /* callback function: BBCallBack */
#define  MAINMENU_SDI                    11
#define  MAINMENU_SDI_BLK34              12
#define  MAINMENU_SDI_BLK34_SUBMENU      13
#define  MAINMENU_SDI_BLK34_TOTAL        14      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK34_SEP34        15
#define  MAINMENU_SDI_BLK34_PHASECAL     16      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK34_JITAMSP      17      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK34_RLOSS        18      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK34_EMBAUDIO     19      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK34_PATT         20      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK56              21
#define  MAINMENU_SDI_BLK56_SUBMENU      22
#define  MAINMENU_SDI_BLK56_TOTAL        23      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK56_SEP56        24
#define  MAINMENU_SDI_BLK56_PHASECAL     25      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK56_JITAMSP      26      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK56_RLOSS        27      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK56_EMBAUDIO     28      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK56_PATT         29      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK78              30
#define  MAINMENU_SDI_BLK78_SUBMENU      31
#define  MAINMENU_SDI_BLK78_TOTAL        32      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK78_SEP78        33
#define  MAINMENU_SDI_BLK78_PHASECAL     34      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK78_JITAMSP      35      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK78_RLOSS        36      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK78_EMBAUDIO     37      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_BLK78_PATT         38      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_TSG                39
#define  MAINMENU_SDI_TSG_SUBMENU        40
#define  MAINMENU_SDI_TSG_TOTAL          41      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_TSG_SEPSDI         42
#define  MAINMENU_SDI_TSG_PHASECAL       43      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_TSG_JITAMSP        44      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_TSG_RLOSS          45      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_TSG_EMBAUDIO       46      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_TSG_PATT           47      /* callback function: SDITestCallBack */
#define  MAINMENU_SDI_SDISEP             48
#define  MAINMENU_SDI_GENL               49      /* callback function: SDITestCallBack */
#define  MAINMENU_SPG                    50
#define  MAINMENU_SPG_HPHGENL            51      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_TESTGENLOCK        52      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_CHECKOVEN          53      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_INT27REF           54      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_INT10REF           55      /* callback function: SPGTestCallBack */
#define  MAINMENU_SPG_SPGSEP             56
#define  MAINMENU_SPG_RESETSPG           57      /* callback function: SPGTestCallBack */
#define  MAINMENU_AUDIOGEN               58      /* callback function: AudioTestCallBack */
#define  MAINMENU_DIVERSE                59
#define  MAINMENU_DIVERSE_POWER          60
#define  MAINMENU_DIVERSE_POWER_SUBMENU  61
#define  MAINMENU_DIVERSE_POWER_TOTAL    62      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_POWER_SEP_PS   63
#define  MAINMENU_DIVERSE_POWER_V5       64      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_POWER_V12      65      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_POWER_VM5      66      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_FAN            67      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_CALDATA        68      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_CALFILES       69      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP1           70
#define  MAINMENU_DIVERSE_WPOFF          71      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_WPON           72      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP2           73
#define  MAINMENU_DIVERSE_VLM            74      /* callback function: DiverseCallBack */
#define  MAINMENU_DIVERSE_SEP3           75
#define  MAINMENU_DIVERSE_PM5640SCH      76      /* callback function: DiverseCallBack */
#define  MAINMENU_LTC                    77      /* callback function: LTCCallBack */
#define  MAINMENU_AFSLUT                 78      /* callback function: AfslutMainCallBack */

#define  SDIMENU                         2
#define  SDIMENU_TOT                     2
#define  SDIMENU_TOT_TIMING              3
#define  SDIMENU_TOT_JITAMSP             4
#define  SDIMENU_TOT_RLOSS               5
#define  SDIMENU_TOT_EMBAUDEDH           6
#define  SDIMENU_TOT_PATT                7


     /* Callback Prototypes: */ 

void CVICALLBACK AfslutMainCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK AudioTestCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK BBCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK DiverseCallBack(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK ExitBtn(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK LTCCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK SDITestCallBack(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK SPGTestCallBack(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK TotalBasicCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
