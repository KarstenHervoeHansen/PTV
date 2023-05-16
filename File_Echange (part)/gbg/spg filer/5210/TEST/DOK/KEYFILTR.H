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

#define  PANEL                           1
#define  PANEL_STRING                    2       /* callback function: StringCallback */
#define  PANEL_DISABLE_CURSOR_KEYS       3
#define  PANEL_IGNORE_NUMBERS            4
#define  PANEL_PUNCT_IS_SPACES           5
#define  PANEL_FORCE_TO_UPPER            6
#define  PANEL_HELP                      7       /* callback function: HelpCallback */
#define  PANEL_QUIT                      8       /* callback function: QuitCallback */
#define  PANEL_TEXTMSG                   9
#define  PANEL_DECORATION                10


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK HelpCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StringCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
