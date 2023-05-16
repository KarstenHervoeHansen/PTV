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

#define  D1JIT_PNL                       1
#define  D1JIT_PNL_D1START_BTN           2       /* callback function: D1StartCallBack */
#define  D1JIT_PNL_D1STOP_BTN            3       /* callback function: D1StopCallBack */
#define  D1JIT_PNL_JITTERTXTBOX          4
#define  D1JIT_PNL_PP_TOL                5
#define  D1JIT_PNL_PP                    6


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK D1StartCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK D1StopCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
