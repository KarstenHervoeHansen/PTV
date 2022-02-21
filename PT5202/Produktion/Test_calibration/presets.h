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

#define  P_PNL                           1
#define  P_PNL_TEXTBOX                   2
#define  P_PNL_FAIL_BUTTON               3       /* callback function: Presets_Fail */
#define  P_PNL_OK_BUTTON                 4       /* callback function: Presets_Ok */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK Presets_Fail(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Presets_Ok(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
