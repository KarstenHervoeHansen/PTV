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

#define  VLM_PANEL                       1
#define  VLM_PANEL_QUIT                  2
#define  VLM_PANEL_DC_ON_OFF             3       /* callback function: DC_Control */
#define  VLM_PANEL_VIDEOMEAS_6           4       /* callback function: VideoMeas_6 */
#define  VLM_PANEL_VIDEOMEAS_5           5       /* callback function: VideoMeas_5 */
#define  VLM_PANEL_VIDEOMEAS_4           6       /* callback function: VideoMeas_4 */
#define  VLM_PANEL_VIDEOMEAS_3           7       /* callback function: VideoMeas_3 */
#define  VLM_PANEL_VIDEOMEAS_2           8       /* callback function: VideoMeas_2 */
#define  VLM_PANEL_VIDEOMEAS_1           9       /* callback function: VideoMeas_1 */
#define  VLM_PANEL_SYNCMEAS              10      /* callback function: SyncMeas */
#define  VLM_PANEL_SHOWLEVEL             11
#define  VLM_PANEL_METER                 12
#define  VLM_PANEL_STARTLINE             13      /* callback function: StartLineCallBack */
#define  VLM_PANEL_NOOFLINES             14      /* callback function: NoOfLinesCallBack */
#define  VLM_PANEL_DELAY                 15      /* callback function: DelayCallBack */
#define  VLM_PANEL_STARTPOS              16      /* callback function: StartPosCallBack */
#define  VLM_PANEL_HPOS_DEC              17
#define  VLM_PANEL_TEXTMSG               18


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK DC_Control(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DelayCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NoOfLinesCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartLineCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartPosCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SyncMeas(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VideoMeas_1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VideoMeas_2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VideoMeas_3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VideoMeas_4(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VideoMeas_5(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VideoMeas_6(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
