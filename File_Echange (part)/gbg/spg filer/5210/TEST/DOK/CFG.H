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

#define  CFG_PNL                         1
#define  CFG_PNL_OKBTN                   2       /* callback function: CfgOkCallBack */
#define  CFG_PNL_SAVETOFILEBTN           3       /* callback function: CfgSaveToFileCallBack */
#define  CFG_PNL_BB                      4
#define  CFG_PNL_ANALOGTSG               5
#define  CFG_PNL_SDITSG                  6
#define  CFG_PNL_SDIBLACK                7
#define  CFG_PNL_SDIGENL                 8
#define  CFG_PNL_LTC                     9
#define  CFG_PNL_MULTIBB                 10
#define  CFG_PNL_AUDIO                   11
#define  CFG_PNL_SPG                     12

#define  DATA_PNL                        2
#define  DATA_PNL_OKBTN                  2       /* callback function: DataOkCallBack */
#define  DATA_PNL_TXTBOX                 3


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK CfgOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CfgSaveToFileCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK DataOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
