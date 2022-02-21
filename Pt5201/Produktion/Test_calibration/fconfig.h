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

#define  F_PNL                           1
#define  F_PNL_FCONFIG_FAIL              2       /* callback function: Fconfig_Fail */
#define  F_PNL_OK_BUTTON                 3       /* callback function: Fconfig_Ok */
#define  F_PNL_TEXTBOX                   4
#define  F_PNL_RS                        5
#define  F_PNL_TEXTBOX_SWR               6


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK Fconfig_Fail(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Fconfig_Ok(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
