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

#define  OCXO_PNL                        1
#define  OCXO_PNL_OCXOOK_BTN             2       /* callback function: OCXOOkCallBack */
#define  OCXO_PNL_OCXOEXIT_BTN           3       /* callback function: OCXOExitCallBack */
#define  OCXO_PNL_TXTBOX                 4
#define  OCXO_PNL_SLIDE                  5
#define  OCXO_PNL_FREQ                   6


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK OCXOExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OCXOOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
