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

#define  PANEL                           1

#define  SYNC_PNL                        2
#define  SYNC_PNL_SYNCEXIT_BTN           2       /* callback function: ExitCallBack */
#define  SYNC_PNL_VIDEOMETER             3
#define  SYNC_PNL_STATUSLED              4
#define  SYNC_PNL_LEVELBOX               5
#define  SYNC_PNL_TEXTMSG                6
#define  SYNC_PNL_COMMTXT                7


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
