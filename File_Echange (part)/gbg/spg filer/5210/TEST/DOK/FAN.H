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

#define  FAN_PNL                         1
#define  FAN_PNL_FANOK_BTN               2       /* callback function: FanOkCallBack */
#define  FAN_PNL_FANEXIT_BTN             3       /* callback function: FanExitCallBack */
#define  FAN_PNL_TXTBOX                  4
#define  FAN_PNL_ADCSTR                  5
#define  FAN_PNL_TERMOMETER              6


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK FanExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FanOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
