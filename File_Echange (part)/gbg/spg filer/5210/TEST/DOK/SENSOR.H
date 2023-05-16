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

#define  SENS_PANEL                      1
#define  SENS_PANEL_QUITBUTTON           2       /* callback function: QuitCallBack */
#define  SENS_PANEL_FROMSENSOR           3
#define  SENS_PANEL_IDBUTTON             4       /* callback function: IDCallBack */
#define  SENS_PANEL_MCBUTTON             5       /* callback function: MCCallBack */
#define  SENS_PANEL_STOPBUTTON           6       /* callback function: STOPCallBack */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK IDCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MCCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK STOPCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
