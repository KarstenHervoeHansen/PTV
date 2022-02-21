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

#define  SPGPHA_PNL                      1
#define  SPGPHA_PNL_SPGOK_BTN            2       /* callback function: SPGOkCallBack */
#define  SPGPHA_PNL_SPGEXIT_BTN          3       /* callback function: ExitSPGCallBack */
#define  SPGPHA_PNL_PILOP_BTN            4       /* callback function: SPGPilopCallBack */
#define  SPGPHA_PNL_PILNED_BTN           5       /* callback function: SPGPilnedCallBack */
#define  SPGPHA_PNL_SLIDE                6       /* callback function: SPGSlideCallBack */
#define  SPGPHA_PNL_TXTBOX               7
#define  SPGPHA_PNL_PHASE_DEC            8


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ExitSPGCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGPilnedCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGPilopCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SPGSlideCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
