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

#define  AES_PNL                         1
#define  AES_PNL_AESOK_BTN               2       /* callback function: AESOkCallBack */
#define  AES_PNL_AESEXIT_BTN             3       /* callback function: AESExitCallBack */
#define  AES_PNL_TXTBOX                  4

#define  ANALOG_PNL                      2
#define  ANALOG_PNL_AESOK_BTN            2       /* callback function: AESOkCallBack */
#define  ANALOG_PNL_AESEXIT_BTN          3       /* callback function: AESExitCallBack */
#define  ANALOG_PNL_TXTBOX               4
#define  ANALOG_PNL_ANALOGAUDIOLEVEL     5
#define  ANALOG_PNL_POTMVALUE            6
#define  ANALOG_PNL_LED                  7
#define  ANALOG_PNL_LEVELTOL             8
#define  ANALOG_PNL_COMMSTR              9

#define  ANMEAS_PNL                      3
#define  ANMEAS_PNL_MEASOK_BTN           2       /* callback function: MeasOkCallBack */
#define  ANMEAS_PNL_LEFTSLIDE            3       /* callback function: AnalogLeftChCallBack */
#define  ANMEAS_PNL_RIGHTSLIDE           4       /* callback function: AnalogRightChCallBack */
#define  ANMEAS_PNL_TXTBOX               5
#define  ANMEAS_PNL_METER                6
#define  ANMEAS_PNL_LEVELMV              7
#define  ANMEAS_PNL_LEVELDBM             8
#define  ANMEAS_PNL_FREQSLIDE            9       /* callback function: FreqSelCallBack */
#define  ANMEAS_PNL_LEVELSEL             10      /* callback function: LevelSelCallBack */
#define  ANMEAS_PNL_DECORATION           11
#define  ANMEAS_PNL_DECORATION_2         12
#define  ANMEAS_PNL_DECORATION_4         13
#define  ANMEAS_PNL_DECORATION_3         14


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK AESExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AESOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AnalogLeftChCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK AnalogRightChCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FreqSelCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LevelSelCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MeasOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
