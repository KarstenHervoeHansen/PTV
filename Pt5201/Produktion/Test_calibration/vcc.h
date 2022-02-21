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

#define  VCC_PNL                         1
#define  VCC_PNL_VCCOK_BTN               2       /* callback function: VCCOkCallBack */
#define  VCC_PNL_VCCEXIT_BTN             3       /* callback function: VCCExitCallBack */
#define  VCC_PNL_VCCSLIDE                4
#define  VCC_PNL_TXTBOX                  5
#define  VCC_PNL_LED                     6


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK VCCExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VCCOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
