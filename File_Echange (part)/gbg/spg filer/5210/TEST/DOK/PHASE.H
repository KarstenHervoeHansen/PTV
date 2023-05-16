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

#define  SCH_PNL                         1
#define  SCH_PNL_PHASESCHOK_BTN          2       /* callback function: PhaseScHOkCallBack */
#define  SCH_PNL_PHASEEXIT_BTN           3       /* callback function: ExitCallBack */
#define  SCH_PNL_PHASEPILOP_BTN          4       /* callback function: PhasePilopCallBack */
#define  SCH_PNL_PHASEPILNED_BTN         5       /* callback function: PhasePilnedCallBack */
#define  SCH_PNL_PHASE_SLIDE             6       /* callback function: PhaseSlideCallBack */
#define  SCH_PNL_SCHPILOP_BTN            7       /* callback function: ScHPilopCallBack */
#define  SCH_PNL_SCHPILNED_BTN           8       /* callback function: ScHPilnedCallBack */
#define  SCH_PNL_SCH_SLIDE               9       /* callback function: SchSlideCallBack */
#define  SCH_PNL_RIGHT_BTN               10      /* callback function: Phase_RightCallBack */
#define  SCH_PNL_LEFT_BTN                11      /* callback function: Phase_LeftCallBack */
#define  SCH_PNL_PHASESCHTXTBOX          12
#define  SCH_PNL_SCH_DEC                 13
#define  SCH_PNL_PHASE_DEC               14
#define  SCH_PNL_BURST                   15


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
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
