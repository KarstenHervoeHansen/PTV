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

#define  HPHSPG_PNL                      1
#define  HPHSPG_PNL_SPGEXIT_BTN          2       /* callback function: ExitCallBack */
#define  HPHSPG_PNL_CONT_BTN             3       /* callback function: ContCallBack */
#define  HPHSPG_PNL_SPGOK_BTN            4       /* callback function: SPGOkCallBack */
#define  HPHSPG_PNL_PHASESCHTXTBOX       5


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ContCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
