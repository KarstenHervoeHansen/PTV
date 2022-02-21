/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 1999. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  SELGEN_PNL                      1
#define  SELGEN_PNL_RETURN_BTN           2       /* callback function: ReturnSelGenlCallBack */
#define  SELGEN_PNL_GENLRING             3       /* callback function: GenlSignalCallBack */
#define  SELGEN_PNL_GENLATTN             4       /* callback function: GenlSignalCallBack */

#define  SPGGEN_PNL                      2
#define  SPGGEN_PNL_OK_BTN               2       /* callback function: OkCallBack */
#define  SPGGEN_PNL_SPGEXIT_BTN          3       /* callback function: ExitSPGCallBack */
#define  SPGGEN_PNL_GENLTXTBOX           4


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ExitSPGCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GenlSignalCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReturnSelGenlCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
