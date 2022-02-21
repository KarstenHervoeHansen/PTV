/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 1996. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                           1
#define  PANEL_EXIT_BTN                  2       /* callback function: ExitCallBack */
#define  PANEL_F1_BTN                    3       /* callback function: F1CallBack */
#define  PANEL_F2_BTN                    4       /* callback function: F2Callback */
#define  PANEL_F3_BTN                    5       /* callback function: F3CallBack */
#define  PANEL_F4_BTN                    6       /* callback function: F4CallBack */
#define  PANEL_VCA_TEXTBOX               7
#define  PANEL_RL_GRAPH                  8       /* callback function: RL_GraphCallback */
#define  PANEL_RL_LEVEL                  9


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK F1CallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK F2Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK F3CallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK F4CallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RL_GraphCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
