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

#define  FILT_PNL                        1
#define  FILT_PNL_FILTEROK_BTN           2       /* callback function: FilterOkCallBack */
#define  FILT_PNL_FILTEREXIT_BTN         3       /* callback function: FilterExitCallBack */
#define  FILT_PNL_CCIR17_BTN             4       /* callback function: SpecialPatternCallBack */
#define  FILT_PNL_MULTIPULSE_BTN         5       /* callback function: SpecialPatternCallBack */
#define  FILT_PNL_SINXX_BTN              6       /* callback function: SpecialPatternCallBack */
#define  FILT_PNL_LINESWEEP_BTN          7       /* callback function: SpecialPatternCallBack */
#define  FILT_PNL_MULTIBURST_BTN         8       /* callback function: SpecialPatternCallBack */
#define  FILT_PNL_SHOWHIDEFILTJUST_BTN   9       /* callback function: ShowHideFilterJustCallBack */
#define  FILT_PNL_TXTBOX                 10
#define  FILT_PNL_SCOPSETUP              11
#define  FILT_PNL_DECORATION             12
#define  FILT_PNL_PATTERNMSG             13

#define  FLTJUS_PNL                      2
#define  FLTJUS_PNL_SINXX_PICTURE        2
#define  FLTJUS_PNL_PICTURE              3


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK FilterExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FilterOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ShowHideFilterJustCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SpecialPatternCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
