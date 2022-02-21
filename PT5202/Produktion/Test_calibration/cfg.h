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

#define  CONFIG                          1
#define  CONFIG_TXT                      2
#define  CONFIG_SAVE_BTN                 3       /* callback function: CFGSaveCallBack */
#define  CONFIG_RETURN_BTN               4       /* callback function: CFGReturnCallBack */

#define  INSTR_PNL                       2
#define  INSTR_PNL_TXTBOX                2
#define  INSTR_PNL_INSTR_BTN             3       /* callback function: InstrCallBack */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK CFGReturnCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CFGSaveCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK InstrCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
