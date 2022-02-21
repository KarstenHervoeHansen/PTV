/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 1998. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  SPGHPH_PNL                      1
#define  SPGHPH_PNL_SPGOK_BTN            2       /* callback function: OkCallBack */
#define  SPGHPH_PNL_SPGEXIT_BTN          3       /* callback function: ExitSPGCallBack */
#define  SPGHPH_PNL_TXTBOX               4
#define  SPGHPH_PNL_MEANDATA             5
#define  SPGHPH_PNL_MINDATA              6
#define  SPGHPH_PNL_MAXDATA              7
#define  SPGHPH_PNL_MEASDATA             8
#define  SPGHPH_PNL_READHPH              9
#define  SPGHPH_PNL_SCOPSETUP            10


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ExitSPGCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
