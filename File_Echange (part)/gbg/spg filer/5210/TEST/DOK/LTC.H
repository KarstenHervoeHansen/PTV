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

#define  LTC_PNL                         1
#define  LTC_PNL_OK_BTN                  2       /* callback function: LTCOkCallBack */
#define  LTC_PNL_EXIT_BTN                3       /* callback function: LTCExitCallBack */
#define  LTC_PNL_TXTBOX                  4
#define  LTC_PNL_SCOPSETUP               5
#define  LTC_PNL_RISEFALL                6
#define  LTC_PNL_VOLTPP                  7
#define  LTC_PNL_OVERSHOOT               8
#define  LTC_PNL_DCOFFSET                9


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK LTCExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LTCOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
