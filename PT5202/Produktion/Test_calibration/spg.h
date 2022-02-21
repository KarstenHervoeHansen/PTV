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

#define  SPG_PNL                         1
#define  SPG_PNL_SPGEXIT_BTN             2       /* callback function: ExitCallBack */
#define  SPG_PNL_SPGPILOP_BTN            3       /* callback function: SPGPilopCallBack */
#define  SPG_PNL_SPGPILNED_BTN           4       /* callback function: SPGPilnedCallBack */
#define  SPG_PNL_SPG_SLIDE               5       /* callback function: SPGSlideCallBack */
#define  SPG_PNL_SPGOK_BTN               6       /* callback function: SPGOkCallBack */
#define  SPG_PNL_PHASESCHTXTBOX          7
#define  SPG_PNL_PHASE_DEC               8


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGPilnedCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGPilopCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGSlideCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
