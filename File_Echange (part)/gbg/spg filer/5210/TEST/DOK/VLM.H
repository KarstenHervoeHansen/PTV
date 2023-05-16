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

#define  VLM_PNL                         1
#define  VLM_PNL_QUITVLM                 2       /* callback function: QuitVlmCallBack */
#define  VLM_PNL_DC_ON_OFF               3       /* callback function: DC_Control */
#define  VLM_PNL_SYNCMEAS                4       /* callback function: SyncMeas */
#define  VLM_PNL_VIDEOMEAS_1             5       /* callback function: VideoMeas_1 */
#define  VLM_PNL_VIDEOMEAS_2             6       /* callback function: VideoMeas_2 */
#define  VLM_PNL_VIDEOMEAS_3             7       /* callback function: VideoMeas_3 */
#define  VLM_PNL_VIDEOMEAS_4             8       /* callback function: VideoMeas_4 */
#define  VLM_PNL_VIDEOMEAS_5             9       /* callback function: VideoMeas_5 */
#define  VLM_PNL_VIDEOMEAS_6             10      /* callback function: VideoMeas_6 */
#define  VLM_PNL_STARTLINE               11      /* callback function: StartLineCallBack */
#define  VLM_PNL_NOOFLINES               12      /* callback function: NoOfLinesCallBack */
#define  VLM_PNL_STARTPOS                13      /* callback function: StartPosCallBack */
#define  VLM_PNL_DELAY                   14      /* callback function: DelayCallBack */
#define  VLM_PNL_METER                   15
#define  VLM_PNL_TEXTMSG                 16
#define  VLM_PNL_DECORATION              17
#define  VLM_PNL_DECORATION_2            18
#define  VLM_PNL_DECORATION_3            19
#define  VLM_PNL_DECORATION_4            20
#define  VLM_PNL_LEVELTXT                21


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK DC_Control(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DelayCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK NoOfLinesCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitVlmCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
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
