/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2000. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PHASEPNL                        1
#define  PHASEPNL_PHASESCHOK_BTN         2       /* callback function: PhaseScHOkCallBack */
#define  PHASEPNL_PHASEEXIT_BTN          3       /* callback function: ExitCallBack */
#define  PHASEPNL_PHASEPILOP_BTN         4       /* callback function: PhasePilopCallBack */
#define  PHASEPNL_PHASEPILNED_BTN        5       /* callback function: PhasePilnedCallBack */
#define  PHASEPNL_PHASESLIDE             6       /* callback function: PhaseSlideCallBack */
#define  PHASEPNL_GAINPILOP_BTN          7       /* callback function: GainPilopCallBack */
#define  PHASEPNL_GAINPILNED_BTN         8       /* callback function: GainPilnedCallBack */
#define  PHASEPNL_SCHPILOP_BTN           9       /* callback function: ScHPilopCallBack */
#define  PHASEPNL_SCHPILNED_BTN          10      /* callback function: ScHPilnedCallBack */
#define  PHASEPNL_GAINSLIDE              11      /* callback function: GainSlideCallBack */
#define  PHASEPNL_SCHSLIDE               12      /* callback function: SchSlideCallBack */
#define  PHASEPNL_RIGHT_BTN              13      /* callback function: Phase_RightCallBack */
#define  PHASEPNL_LEFT_BTN               14      /* callback function: Phase_LeftCallBack */
#define  PHASEPNL_PHASESCHTXTBOX         15
#define  PHASEPNL_COMM                   16
#define  PHASEPNL_SCH_DEC                17
#define  PHASEPNL_PHASE_DEC              18
#define  PHASEPNL_GAIN_DEC               19


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GainPilnedCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GainPilopCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GainSlideCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PhasePilnedCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PhasePilopCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PhaseScHOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PhaseSlideCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Phase_LeftCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Phase_RightCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ScHPilnedCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ScHPilopCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SchSlideCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
