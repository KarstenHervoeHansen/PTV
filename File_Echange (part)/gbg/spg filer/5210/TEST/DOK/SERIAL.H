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

#define  CONFIG                          1
#define  CONFIG_COMPORT                  2
#define  CONFIG_BAUDRATE                 3
#define  CONFIG_PARITY                   4
#define  CONFIG_DATABITS                 5
#define  CONFIG_STOPBITS                 6
#define  CONFIG_INPUTQ                   7       /* callback function: InputQCallBack */
#define  CONFIG_OUTPUTQ                  8
#define  CONFIG_CTSMODE                  9
#define  CONFIG_XMODE                    10
#define  CONFIG_TIMEOUT                  11
#define  CONFIG_CLOSECONFIG              12      /* callback function: CloseConfigCallback */
#define  CONFIG_TIMEOUT_MSG1             13

#define  SERIAL                          2
#define  SERIAL_CONFIG                   2       /* callback function: ConfigCallBack */
#define  SERIAL_ERROR                    3       /* callback function: ErrorCallBack */
#define  SERIAL_COMSTATUS                4       /* callback function: ComStatusCallBack */
#define  SERIAL_SENDTERM                 5
#define  SERIAL_TBOX_SEND                6
#define  SERIAL_SENDBYTE                 7
#define  SERIAL_SENDMODE                 8       /* callback function: SendModeCallBack */
#define  SERIAL_BYTES                    9
#define  SERIAL_SEND                     10      /* callback function: SendCallBack */
#define  SERIAL_READ_COUNT               11
#define  SERIAL_READTERM                 12
#define  SERIAL_TBOX_READ                13
#define  SERIAL_READ                     14      /* callback function: ReadCallBack */
#define  SERIAL_CLEARBOX                 15      /* callback function: ClearBoxCallBack */
#define  SERIAL_GETOUTQ                  16      /* callback function: GetOutQCallBack */
#define  SERIAL_GETINQ                   17      /* callback function: GetInQCallBack */
#define  SERIAL_FLUSHOUTQ                18      /* callback function: FlushOutQCallBack */
#define  SERIAL_FLUSHINQ                 19      /* callback function: FlushInCallBack */
#define  SERIAL_QUIT                     20      /* callback function: QuitCallBack */
#define  SERIAL_DECORATION               21
#define  SERIAL_DECORATION_2             22
#define  SERIAL_TRANSMIT_MSG             23
#define  SERIAL_RECEIVE_MSG              24
#define  SERIAL_CONFIG_MSG               25
#define  SERIAL_RCV_HELP_MSG             26
#define  SERIAL_TRANS_HELP_MSG           27


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */ 

int  CVICALLBACK ClearBoxCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CloseConfigCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ComStatusCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ConfigCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ErrorCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FlushInCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK FlushOutQCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetInQCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK GetOutQCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK InputQCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReadCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SendCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SendModeCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
