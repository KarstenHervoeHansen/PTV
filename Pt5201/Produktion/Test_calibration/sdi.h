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

#define  AMSP_PNL                        1
#define  AMSP_PNL_AMSPOK_BTN             2       /* callback function: SDIOkCallBack */
#define  AMSP_PNL_AMSPEXIT_BTN           3       /* callback function: SDIExitCallBack */
#define  AMSP_PNL_TXTBOX                 4
#define  AMSP_PNL_GRAPH                  5
#define  AMSP_PNL_LEV_1                  6
#define  AMSP_PNL_LEV_0                  7

#define  JIT_PNL                         2
#define  JIT_PNL_JITOK_BTN               2       /* callback function: SDIOkCallBack */
#define  JIT_PNL_JITEXIT_BTN             3       /* callback function: SDIExitCallBack */
#define  JIT_PNL_TXTBOX                  4
#define  JIT_PNL_PPMEAN2                 5
#define  JIT_PNL_PPTOL2                  6
#define  JIT_PNL_PPMEAN1                 7
#define  JIT_PNL_PPTOL1                  8
#define  JIT_PNL_PPMEAS                  9
#define  JIT_PNL_VCASETUP                10
#define  JIT_PNL_DECORATION              11
#define  JIT_PNL_DECORATION_2            12
#define  JIT_PNL_FRAMEMSG                13
#define  JIT_PNL_LINEMSG                 14

#define  PATT_PNL                        3
#define  PATT_PNL_SDIPATTOK_BTN          2       /* callback function: SDIOkCallBack */
#define  PATT_PNL_SDIPATTEXIT_BTN        3       /* callback function: SDIExitCallBack */
#define  PATT_PNL_TXTBOX                 4
#define  PATT_PNL_SCOPMSG                5

#define  RLOSS_PNL                       4
#define  RLOSS_PNL_RLOSSOK_BTN           2       /* callback function: SDIOkCallBack */
#define  RLOSS_PNL_RLOSSEXIT_BTN         3       /* callback function: SDIExitCallBack */
#define  RLOSS_PNL_TXTBOX                4
#define  RLOSS_PNL_GRAPH                 5
#define  RLOSS_PNL_MAXLEVEL              6

#define  SDIGEN_PNL                      5
#define  SDIGEN_PNL_SDIGENLOK_BTN        2       /* callback function: SDIOkCallBack */
#define  SDIGEN_PNL_SDIGENLEXIT_BTN      3       /* callback function: SDIExitCallBack */
#define  SDIGEN_PNL_VCO                  4
#define  SDIGEN_PNL_TXTBOX               5
#define  SDIGEN_PNL_LED                  6

#define  SDI_PNL                         6
#define  SDI_PNL_SDIOK_BTN               2       /* callback function: SDIOkCallBack */
#define  SDI_PNL_SDIEXIT_BTN             3       /* callback function: SDIExitCallBack */
#define  SDI_PNL_TXTBOX                  4
#define  SDI_PNL_DELAY                   5


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK SDIExitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SDIOkCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
