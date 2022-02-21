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

#define  COUNT_PNL                       1
#define  COUNT_PNL_F45EXIT_BTN           2       /* callback function: F45MHzCallback */
#define  COUNT_PNL_TXTBOX                3
#define  COUNT_PNL_FREQ                  4
#define  COUNT_PNL_DECORATION            5

#define  OCXO_PNL                        2
#define  OCXO_PNL_REFOSCOK_BTN           2       /* callback function: RefOscCallBack */
#define  OCXO_PNL_REFOSCERR_BTN          3       /* callback function: RefOscCallBack */
#define  OCXO_PNL_TXTBOX                 4
#define  OCXO_PNL_SLIDE                  5
#define  OCXO_PNL_VCOVOLT                6
#define  OCXO_PNL_COMSTR                 7
#define  OCXO_PNL_FREQ                   8
#define  OCXO_PNL_DEC1                   9


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK F45MHzCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RefOscCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
