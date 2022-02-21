/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:    MESSAGE.C                                  */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  980208                                    */
/*  Rev. date:  981026, KEn, DEV                              */
/*  Status:    Version 1.2                                  */
/*                                                  */
/* Changes:                                                                */
/* 991026: Rewritten void WriteErrorLN2(UC LN2Pos, UL error)          */
/*       Introduced new function to handle writing of an error        */
/* 981005: Cleanup in leveldetector errors in function WriteErrorLN2()    */ 
/* 980915: Error in MessgeHandling(): Local lockout for serial remote not  */
/*         displayed correctly.                              */
/* 980616: Released as 4008 002 06812                          */
/* 980611: Update LevelStatus handling with 6 audio levels for PT8635    */
/* 980609: In DisplayErrorLine() & GotoErrorStatus() the system error:    */
/*        Level Detector ALWAYS will pass                      */
/* 980514: Released as 4008 002 06811                          */
/***************************************************************************/

/***************************************************************************/
/* Include files:                                        */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <xa.h>

#include "mas.h"

#include <ctype.h>

#include "sw5230.h"
#include "message.h"
#include "menutree.h"
#include "menu.h"
#include "menuhlp.h"
#include "disp_drv.h"
#include "text.h"
#include "iic.h"
#include "serial1.h"
#include "tables.h"
#include "keyb_drv.h"
#include "util.h"
#include "xaexprt.h"

static char tbuff[80];

/***************************************************************************/
/* MessageHandling                                 MESSAGE.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960621                          */
/* Revised:    980915                                       */
/*                                                  */
/*  Function:  Display a message text in display, and start message timer  */
/*  Remarks:                                            */
/*  Returns:    --                                        */
/*  Updates:                                            */
/***************************************************************************/
void MessageHandling(UC messageno) {

  switch (messageno) {
    case ParallelLockoutMessage:
      WriteTxtLN1(0, "M(001): ");
      WriteCode1(ParallelRemoteOperatingTxt);
      ClearRestLN1();

      WriteCodeLN2(0, LocalLockoutTxt);
      ClearRestLN2();

      MessageWindowOn = ON;
      MessageCntDwn = 15;        // 15*200ms = 3s
      break;

    case SerialLockoutMessage:
      WriteTxtLN1(0, "M(003): ");
      WriteCode1(SerialRemoteOperatingTxt);
      ClearRestLN1();

      WriteCodeLN2(0, LocalLockoutTxt);
      ClearRestLN2();

      MessageWindowOn = ON;
      MessageCntDwn = 15;        // 15*200ms = 3s
      break;

    case NoResponseFromRS232:
      WriteTxtLN1(0, "E(160): ");
      WriteCode1(NoResponseFromTxt);
      WriteCode1(RS232Txt);
      ClearRestLN1();
      WriteArrowsField(U_+D_+R_+L_+E_);  // Write icons/dots on line 1&2

      WriteCodeLN2(0, CheckCableTxt);
      ClearRestLN2();
      break;

    case ErrorDownloadingFromRS232:
      WriteTxtLN1(0, "E(161): ");
      WriteCode1(ErrorDownloadingFromTxt);
      WriteCode1(RS232Txt);
      ClearRestLN1();
      WriteArrowsField(U_+D_+R_+L_+E_);  // Write icons/dots on line 1&2

      WriteCodeLN2(0, ChecksumErrorReceivingTxt);
      ClearRestLN2();
      break;

    case ErrorInDataFromRS232:
      WriteTxtLN1(0, "E(162): ");
      WriteCode1(ErrorDownloadingFromTxt);
      WriteCode1(RS232Txt);
      ClearRestLN1();
      WriteArrowsField(U_+D_+R_+L_+E_);  // Write icons/dots on line 1&2

      WriteCodeLN2(0, DataNotCompatibleTxt);
      ClearRestLN2();
      break;
      
    case SwitchedDaylightOn:
      WriteTxtLN1(0, "M(201): ");
      WriteCode1(LTCDaylightSwitchedTxt);
      WriteCode1(OnTxt);
      ClearRestLN1();
      WriteArrowsField(U_+D_+R_+L_+E_);  // Write icons

      WriteCodeLN2(0, LTCArrTxt[LTCNo]);
      ClearRestLN2();
      break;
      
    case SwitchedDaylightOff:
      WriteTxtLN1(0, "M(201): ");
      WriteCode1(LTCDaylightSwitchedTxt);
      WriteCode1(OffTxt);
      ClearRestLN1();
      WriteArrowsField(U_+D_+R_+L_+E_);  // Write icons

      WriteCodeLN2(0, LTCArrTxt[LTCNo]);
      ClearRestLN2();
      break;
    
    case ResyncedLTC:
      WriteTxtLN1(0, "M(202): ");
      WriteCode1(LTCResyncedTxt);
      WriteCode1(LTCArrTxt[LTCNo]);
      ClearRestLN1();
      WriteArrowsField(U_+D_+R_+L_+E_);  // Write icons

      WriteCharLN2(0, ' ');
      ClearRestLN2();
      break;      
    default:
      break;
  }
}

/***************************************************************************/
/* SetupRS232GotoLocalMessage                           MESSAGE.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970723                          */
/* Revised:    970723                                       */
/*                                                  */
/*  Function:  Prepare for the "Goto Local?"  display and state machine    */
/*  Remarks:                                            */
/*  Returns:    --                                        */
/*  Updates:                                            */
/***************************************************************************/
void SetupRS232GotoLocalMessage() {

  if (MenuModeOn) {          // If in menu mode..
    if (SelectModeOn) {        // If in selection mode ..
      Ptr->EscapeFct();        //  be sure to terminate NICELY!!
      Ptr->DispFunct();
    }

    UpToStatus();            // Update ptr to status-display
    Ptr->DispFunct();          // Display status-display
  }
  PrevPtr = Ptr;              // Save old status display which is
                        //  ALWAYS a status display

  FuncState = 2;              // Prepare for state machine
  FuncTriggers = 0;
  FuncSave = FuncEscape = FALSE;
}

/***************************************************************************/
/* RS232GotoLocalMessage                             MESSAGE.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960621                          */
/* Revised:    980415                                       */
/*                                                  */
/*  Function:                                          */
/*  Remarks:                                            */
/*  Returns:    --                                        */
/*  Updates:                                            */
/***************************************************************************/
void RS232GotoLocalMessage() {

  if (FuncSave || FuncEscape) {

    if (FuncSave)  {            // If OK selected..
      SerialRemoteActive = FALSE;  //  goto local mode
    }
    MenuModeOn = SelectModeOn = OFF;

    FuncSave = FuncEscape = FALSE;  // This is normally done in the function
                          //  pointed to by PrevPtr. In this case
                          //  it MUST be reset manually 'cause
                          //  PrevPtr points to a StatusDisplay

    Ptr = PrevPtr;              // Restore old status display pointer
    Ptr->DispFunct();            //  which ALWAYS is a status display
    return;
  }

  MenuModeOn = SelectModeOn = ON;

  WriteTxtLN1(0, "M(002): ");
  WriteCode1(SerialRemoteOperatingTxt);
  ClearRestLN1();

  WriteTxtLN2(0, "Goto local ?");
  ClearRestLN2();

  WriteType10_OK_ESC(FuncState);      // Write the SAVE ESC text

  FuncTriggers = 0;                // Reset the button-selection
}

/***************************************************************************/
/* WriteErrorLN2                                                 MESSAGE.C */
/*                                                                         */
/* Author:     Kim Engedahl, DEV, 960621                                   */
/* Revised:    981005                                                      */
/*                                                                         */
/* Function:   Write an error text based on <errorcode> on line 2, LN2Pos  */
/* Remarks:    Translation of the 32-bit parameter, error:                 */
/*             16MSB          8 BIT    8 LSB                               */
/*             level detector [index]  ErrorType ErrorCode                 */
/*             Option PT8601 SHOULD only be placed in upper space TXG2, ie.*/
/*             it is ONLY tested in this place.                            */
/*             Level detectors:                                            */
/*                         01  02/03  31v1  31v2  32/33 08/09/39           */
/*                BB1                                                      */
/*                BB2                                                      */
/*                BBU_1A   X            X     X     X       X              */
/*                BBU_1B                      X     X       X              */
/*                BBU_2A   X            X     X     X       X              */
/*                BBU_2B                      X     X       X              */
/*                BBU_3A                                    X              */
/*                BBU_3B                                    X              */
/*                SDI            X                  X                      */
/* Returns:    --                                                          */
/* Updates:    TBD                                                         */
/***************************************************************************/
void WriteErrorLN2(UC LN2Pos, UL error)
  {
  char tmpbuf[40];

  WriteCodeLN2(LN2Pos, ErrorNoHdrTxt);        // Write error text header

  sprintf(tmpbuf, "%03d", (UC) (error & 0x000000FF) );
  WriteTxtLN2(LN2Pos+2, tmpbuf);            // Write error number

  GetErrorString(error, tmpbuf);

  WriteTxtLN2(LN2Pos+8, tmpbuf);
  }

/***************************************************************************/
/* GetErrorString                                               MESSAGE.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 991026                                    */
/* Revised:    991026                                                      */
/*                                                                         */
/*  Function:  Compose and return an error string with the actual error    */
/*  Remarks:     Translation of the 32-bit parameter, error:               */
/*              16MSB        8 BIT        8 LSB                            */
/*          level detector  [index]  ErrorType ErrorCode                   */
/*          See drawing 'Main Board', sheet 5 for details on reading level */
/*          detectors.                                                     */
/*          On 23.11.2006, the level detectors on DL and HD boards are     */
/*          tackled by microprocessor, which controls the LEVOK pin, which */
/*          goes to BB4_OK, BB6_OK or BB8_OK line.                         */
/*          The BB3_OK, BB5_OK and BB7_OK lines are controlled by FPGA and */
/*          are now used to indicate malfunctions inside the FPGA.  They   */
/*          are decoded here as Internal Error at 'board'.                 */ 
/*           InternalErrorTxt[] = "Unstable signal at "                    */
/*  Returns:    --                                                         */
/*  Updates:                                                               */
/*
#define SystemErrorType             0
#define AnlBlkErrorType            10 0x0A
#define SDITSGErrorType            20 0x14
#define AESEBUErrorType            30 0x1E
#define AnlTPGErrorType            40 0x28
#define SDITPGErrorType            50 0x32
#define SDIGenlockErrorType        60 0x3C
#define TimeClockErrorType         70 0x46
#define OSCErrorType               80 0x50
#define ROMMemoryErrorType         90 0x5A
#define RAMMemoryErrorType        100 0x64
#define KeyboardErrorType         110 0x6E
#define LCD_ADCErrorType          120 0x78
#define TempADCErrorType          130 0x82
#define VoltageADCErrorType       140 0x8C
#define LevelDetectorErrorType    150

#define HDTPGErrorType            160
#define TRIErrorType              170
#define DLTPGErrorType            180

#define InternalErrorType         190

// SYSTEM ERRORS
#define MultipleFailure             1
#define PowerN5Failure              2
#define Power12Failure              3
#define TemperatureFailure          4

#define TemperatureWarning          5
#define LevelDetectorError          6
#define ConfigurationError          7
#define MultipleError               8

// NORMAL ERRORS
#define ChecksumError               5
*/
/***************************************************************************/
void GetErrorString(UL error, char* tmpbuf)
  {
  UI LevelError;
  UC errorindex, errortype, errorcode;
  code char* TxtPtr;

  FeedTheWatchdog;

  LevelError = (UI) (error >> 16);

  errorindex = (UC) ((error & 0x0000FF00) >> 8);
  errorcode  = (UC)  (error & 0x000000FF);

  errortype = (errorcode / 10) * 10;
  errorcode -= errortype;

  if (errortype == SystemErrorType) /* ( == 0) */
    {        // If any system error..
    switch (errorcode)
      {
      case LevelDetectorError: /* ( == 6) */
        Cstrcpy(tmpbuf, LevelDetectorTxt);  //LevelDetectorTxt[] = "Level error at ";
        TxtPtr = UnknownOptionTxt;          // Default: Unknown Option
        if (AESEBUUnit.Present && (AESEBUUnit.HWType == PT8635))
          {
          if ((LevelError & 0x4000) == 0)   // PT8635 BNC Word #2
            TxtPtr = AESEBUWord2Txt;
          if ((LevelError & 0x2000) == 0)   // PT8635 BNC Word #1
            TxtPtr = AESEBUWord1Txt;
          if ((LevelError & 0x1000) == 0)   // PT8635 XLR generator #2
            TxtPtr = AESEBUXLR2Txt;
          if ((LevelError & 0x0800) == 0)   // PT8635 XLR generator #1
            TxtPtr = AESEBUXLR1Txt;
          if ((LevelError & 0x0400) == 0)   // PT8635 BNC generator #2
            TxtPtr = AESEBUBNC2Txt;
          if ((LevelError & 0x0200) == 0)   // PT8635 BNC generator #1
            TxtPtr = AESEBUBNC1Txt;
          }

        if ((LevelError & 0x0100) == 0)     // SDI-TPG1 Generator (BB9_OK @ XJ1)
          if (SDITPGUnit[0].Present)
            TxtPtr = SDITPG1Txt;

#if 0
    sprintf(tbuff, "LevelError  = 0x%x\n", LevelError & 0x00FF);
    Udr_PutNChar(tbuff);
#endif

/*********************************   XC1 connector begin ****************************/
 
        if ((LevelError & 0x0080) == 0)  // XC1 connector (BB8_OK)
          {     // TSG4, BB6
          if (AnlBlkUnit[BB6Item].Present)
            TxtPtr = BBArrTxt[BB6Item];
          else
            if (TLGUnit[0].Present)
              TxtPtr = TRI1Txt;
            else
              if (HDTPGUnit[4].Present)
                TxtPtr = HD2Txt;            /* HD Board 2 */
              else
                if (DLTPGUnit[2].Present)
                  TxtPtr = DL2Txt;          /* DL Board 2 */
                else
                  if (SDITSGUnit[2].Present)
                    TxtPtr = SDITSG4Txt;
           }
                                
        if ((LevelError & 0x0040) == 0)  // XC1 connector (BB7_OK)
          {    // TSG4, BB5
          if (AnlBlkUnit[BB5Item].Present)
            TxtPtr = BBArrTxt[BB5Item];
          else 
            if (TLGUnit[0].Present)
              TxtPtr = TRI1Txt;
            else
              if (HDTPGUnit[4].Present)
                {
                Cstrcpy(tmpbuf, InternalErrorTxt);  //InternalErrorTxt[] = "Unstable signal at ";
                TxtPtr = HD2Txt;          /* HD Board 2 */
                }
              else
                if (DLTPGUnit[2].Present)
                  {
                  Cstrcpy(tmpbuf, InternalErrorTxt);  //InternalErrorTxt[] = "Unstable signal at ";
                  TxtPtr = DL2Txt;        /* DL Board 2 */
                  }
                else
                  if (SDITSGUnit[2].Present)
                    TxtPtr = SDITSG4Txt;
          }
/*********************************   XC1 connector end  ****************************/

/*********************************   XB1 connector begin ***************************/

        if ((LevelError & 0x0020) == 0)  // XB1 connector (BB6_OK)
          {    // BB8, ATPG2V2, ATPG5V2, // TSG2, STPG2, STPG5
          if (AnlBlkUnit[BB8Item].Present)
            TxtPtr = BBArrTxt[BB8Item];
          else /* ************** NEW: TLG unit @ bbu2_addr *****************/
            if (TLGUnit[8].Present)
              TxtPtr = TRI3Txt;
            else
              if (HDTPGUnit[8].Present)
                TxtPtr = HD3Txt;          /* Board 3 */
              else
                if (DLTPGUnit[4].Present)
                  TxtPtr = DL3Txt;        /* Board 3 */
                else
                  {
                  if (AnlTPGUnit[0].Present || AnlTPGUnit[1].Present)
                    {
                    if (AnlTPGUnit[0].Present && AnlTPGUnit[1].Present)
                      TxtPtr = AnalogTPG5Txt;
                    else
                      TxtPtr = AnalogTPG2Txt;
                    }
                  else
                    if (SDITSGUnit[0].Present)
                      TxtPtr = SDITSG2Txt;
                    else
                      {
                      if (SDITPGUnit[1].Present || SDITPGUnit[2].Present)
                        {
                        if (SDITPGUnit[1].Present && SDITPGUnit[2].Present)
                          TxtPtr = SDITPG5Txt;
                        else
                          TxtPtr = SDITPG2Txt;
                        }
                      }
                    }
            }
                                 
        if ((LevelError & 0x0010) == 0) // XB1 connector (BB5_OK)
          {    // BB7, ATPG2, ATPG5, TSG2, // STPG2, STPG5,
          if (AnlBlkUnit[BB7Item].Present)
            TxtPtr = BBArrTxt[BB7Item];
          else /* ************** NEW: TLG unit @ bbu2_addr *****************/
            if (TLGUnit[8].Present)
              TxtPtr = TRI3Txt;
            else
              if (HDTPGUnit[8].Present)
                {
                Cstrcpy(tmpbuf, InternalErrorTxt);  //InternalErrorTxt[] = "Unstable signal at ";
                TxtPtr = HD3Txt;          /* Board 3 */
                }
              else
                if (DLTPGUnit[4].Present)
                  {
                  Cstrcpy(tmpbuf, InternalErrorTxt);  //InternalErrorTxt[] = "Unstable signal at ";
                  TxtPtr = DL3Txt;        /* Board 3 */
                  }
                else
                  {
                  if (AnlTPGUnit[0].Present || AnlTPGUnit[1].Present)
                    {
                    if (AnlTPGUnit[0].Present && AnlTPGUnit[1].Present)
                      TxtPtr = AnalogTPG5Txt;
                    else
                      TxtPtr = AnalogTPG2Txt;
                    }
                  else
                    {
                    if (SDITSGUnit[0].Present)
                      TxtPtr = SDITSG2Txt;
                    else
                      if (SDITPGUnit[1].Present || SDITPGUnit[2].Present)
                        {
                        if (SDITPGUnit[1].Present && SDITPGUnit[2].Present)
                          TxtPtr = SDITPG5Txt;
                        else
                          TxtPtr = SDITPG2Txt;
                        }
                      }
                    }
            }
/*********************************   XB1 connector end  ****************************/

/*********************************   XA1 connector begin ****@ bbu1_addr *******/

        if ((LevelError & 0x0008) == 0)  // BBU_1B connector (BB4_OK)
          {    // BB4, ATPG2, TSG3, STPG2   // BB+Mul
          if (AnlBlkUnit[BB4Item].Present)
            TxtPtr = BBArrTxt[BB4Item];
          else
            if (AnlBlkUnit[BB2Item].HWType == PT8604)
              TxtPtr = BBMulTxt;
            else
              if (TLGUnit[4].Present)
                TxtPtr = TRI2Txt;
              else
                if (HDTPGUnit[0].Present)
                  TxtPtr = HD1Txt;          /* Board 1 */
                else
                  if (DLTPGUnit[0].Present)
                    TxtPtr = DL1Txt;        /* Board 1 */
                  else
                    if (AnlTPGUnit[0].Present)
                      TxtPtr = AnalogTPG2Txt;
                    else
                      if (SDITSGUnit[1].Present)
                        TxtPtr = SDITSG3Txt;
                      else
                        if (SDITPGUnit[1].Present)
                          TxtPtr = SDITPG2Txt;
           }
                          
        if ((LevelError & 0x0004) == 0)     // XA1 connector (BB3_OK)
          {    // BB3, ATPG2, TSG3, STPG2   // BB+Mul
          if (AnlBlkUnit[BB3Item].Present)
            TxtPtr = BBArrTxt[BB3Item];
          else
            if (AnlBlkUnit[BB2Item].HWType == PT8604)
              TxtPtr = BBMulTxt;
            else
              if (TLGUnit[4].Present)
                TxtPtr = TRI2Txt;
              else
                if (HDTPGUnit[0].Present)
                  {
                  Cstrcpy(tmpbuf, InternalErrorTxt);  //InternalErrorTxt[] = "Unstable signal at ";
                  TxtPtr = HD1Txt;          /* Board 1 */
                  }
                 else
                  if (DLTPGUnit[0].Present)
                    {
                    Cstrcpy(tmpbuf, InternalErrorTxt);  //InternalErrorTxt[] = "Unstable signal at ";
                    TxtPtr = DL1Txt;        /* Board 1 */
                    }
                  else
                    if (AnlTPGUnit[0].Present)
                      TxtPtr = AnalogTPG2Txt;
                    else
                      if (SDITSGUnit[1].Present)
                        TxtPtr = SDITSG3Txt;
                      else
                        if (SDITPGUnit[1].Present)
                          TxtPtr = SDITPG2Txt;
           }
/*********************************   XA1 connector end  ****************************/

        if ((LevelError & 0x0002) == 0)       // BB2 (BB2_OK)
          TxtPtr = BBArrTxt[BB2Item];

        if ((LevelError & 0x0001) == 0)       // BB1 (BB1_OK)
          TxtPtr = BBArrTxt[BB1Item];
        Cstrcat(tmpbuf, TxtPtr);
        break;  /* case LevelError */

      case ConfigurationError:
        Cstrcpy(tmpbuf, ConfigurationErrorTxt);
        break;
      case MultipleError: /* (== 8) */
        Cstrcpy(tmpbuf, MultipleErrorTxt);
        break;
      case MultipleFailure: /* == 1 */
        Cstrcpy(tmpbuf, MultipleFailureTxt);
        break;
      case PowerN5Failure:
        Cstrcpy(tmpbuf, PowerFailureTxt);
        Cstrcat(tmpbuf, VoltageN5Txt);
        break;
      case Power12Failure:
        Cstrcpy(tmpbuf, PowerFailureTxt);
        Cstrcat(tmpbuf, Voltage12Txt);
        break;
      case TemperatureFailure:
        Cstrcpy(tmpbuf, TemperatureFailureTxt);
        break;
      case TemperatureWarning:
        Cstrcpy(tmpbuf, TemperatureWarningTxt);
        break;
      } // switch (errorcode)
    }   // if (errortype == SystemErrorType)
  else
    {
    switch (errorcode)
      {
      case 0:
        TxtPtr = GeneralFailureTxt;
        break;
      case 1:
        TxtPtr = NoContactToTxt;
        break;
      case 2:
        TxtPtr = ErrorWritingToTxt;
        break;
      case 3:
        TxtPtr = NoResponseFromTxt;
        break;
      case 4:
        TxtPtr = ErrorReadingFromTxt;
        break;
      case ChecksumError:
        TxtPtr = ChecksumErrorTxt;
        break;
      default:
        TxtPtr = GeneralFailureTxt;
        break;
      }
    Cstrcpy(tmpbuf, TxtPtr);

    switch (errortype)
      {
#if 0
    sprintf(tbuff, "errortype  = 0x%x\n\r", (int) errortype);
    Udr_PutNChar(tbuff);
#endif
      case TRIErrorType:
        TxtPtr = TRIArrTxt[errorindex]; /* JK addition */
        break;
      case HDTPGErrorType:
        TxtPtr = HDArrTxt[errorindex];  /* JK addition */
        break;
      case DLTPGErrorType:
        TxtPtr = DLArrTxt[errorindex];  /* JK addition */
        break;
      case AnlBlkErrorType:
        TxtPtr = BBArrTxt[errorindex];
        break;
      case SDITSGErrorType:
        TxtPtr = base_menu[SDITSG2Item + errorindex].ItemTxt;
        break;
      case AESEBUErrorType:
        TxtPtr = AESEBU_menu[errorindex].ItemTxt;
        break;
      case AnlTPGErrorType:
        TxtPtr = base_menu[AnlTPG2Item + errorindex].ItemTxt;
        break;
      case SDITPGErrorType:
        TxtPtr = base_menu[SDITPG1Item + errorindex].ItemTxt;
        break;
      case SDIGenlockErrorType:
        TxtPtr = SDIGenlockTxt;
        break;
      case TimeClockErrorType:
        TxtPtr = TimeCodeTxt;
        break;
      case OSCErrorType:
        TxtPtr = OSCTxt;
        break;
      case ROMMemoryErrorType:
        TxtPtr = ROMMemoryTxt;
        break;
      case RAMMemoryErrorType:
        TxtPtr = RAMMemoryTxt;
        break;
      case KeyboardErrorType:
        TxtPtr = KeyboardTxt;
        break;
      case LCD_ADCErrorType:
        TxtPtr = LCDADConvTxt;
        break;
      case TempADCErrorType:
        TxtPtr = TEMPADConvTxt;
        break;
      case VoltageADCErrorType:
        TxtPtr = VoltageTxt;
        break;
      case LevelDetectorErrorType:
        TxtPtr = LevelDetTxt; //LevelDetTxt[] = "LEVEL DETECTOR";
        break;
      default:
        TxtPtr = UnknownOptionTxt;
        break;
      }  // switch (errortype)
    Cstrcat(tmpbuf, TxtPtr);
    }
  }

/***************************************************************************/
/* DisplayErrorLine                                             MESSAGE.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 960621                                    */
/* Revised:    980610                                                      */
/*                                                                         */
/*  Function:  Display an error on line 2 without disturbing pointer Ptr   */
/*  Remarks:    This function is ONLY used during power up. It will record */
/*          ALL errors. It WILL record any error the number of times       */
/*          it occurs.                                                     */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void DisplayErrorLine(UI error, UC timer)
  {
  UL ErrorVar;
  ErrorStatus = error;

  ErrorVar = (UL) 0x10000*LevelStatus + (UL) ErrorStatus;
#if 0
    sprintf(tbuff, "DisplayErrorLine, ErrorStatus = %d, ErrorVar = %ld\n", ErrorStatus, ErrorVar);
    Udr_PutNChar(tbuff);
#endif

  if (++ErrorQueuePtr > NoInErrorQueue-1)  // If next queue entry GT. max..
    ErrorQueuePtr = 0;              //  set pointer to 0

  ErrorQueue[ErrorQueuePtr] = ErrorVar;  // Save extended error status

  CharBlinkLN2(0, OFF);            // Switch off possible blinking

  WarningLED = ON;                 // Errors wil switch ON Warning LED
  UpdateLEDs();

  WriteErrorLN2(0, ErrorVar);      // Write error in display
  ClearRestLN2();

  if (timer)
    {                              // If time enabled..
    DisplayTimer = timer;          //  display error line for timer*20ms

    while (DisplayTimer)
      FeedTheWatchdog;
    }
  }


/***************************************************************************/
/* GotoErrorStatus                                              MESSAGE.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 960621                                    */
/* Revised:    980610                                                      */
/*                                                                         */
/*  Function:  --                                                          */
/*  Remarks:    index:8 error:8                                            */
/*              index: which SDITPG, or which SDITSG... etc.               */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */

/*   UI ErrorStatus; is Last active global error code, 32 BIT (?? UI is 16 bits) */
    // 8 MSB   8 LSB
    // [index]  ErrorType+ErrorNo
/***************************************************************************/
void GotoErrorStatus(UI error)
  {
  UL ErrorVar;
#if 0
    sprintf(tbuff, "GotoErrorStatus1, error = %d, ErrorStatus = %d\n\r", error, ErrorStatus);
    Udr_PutNChar(tbuff);
#endif

  if (ErrorStatus && (ErrorStatus < 10))
    {                              // If system error type active.
    if (error > ErrorStatus)       //  only higher priority errors
      return;                      //  OR level errors are allowed

    if (error == ErrorStatus)
      if (error != (SystemErrorType + LevelDetectorError))
        return;
    }
#if 0
    sprintf(tbuff, "GotoErrorStatus2 MenuModeOn=%d SelectModeOn=%d\n\r", (int)MenuModeOn, (int)SelectModeOn);
    Udr_PutNChar(tbuff);
#endif

  if (MenuModeOn)
    {                   // If in menu mode..
    if (SelectModeOn)   // If in selection mode..
      {                 //  be sure to terminate NICELY!!
      Ptr->EscapeFct(); //  FuncSave = FALSE; FuncEscape = TRUE;     
      Ptr->DispFunct();
      }
    UpToStatus();     //  MenuModeOn = FALSE;  Ptr = &status_menu[StatusNdx];
    Ptr->DispFunct();
    }

  ErrorStatus = error;
#if 0
    sprintf(tbuff, "GotoErrorStatus3: ErrorStatus = %d\n\r", ErrorStatus);
    Udr_PutNChar(tbuff);
#endif

  ErrorVar = (UL) 0x10000 * LevelStatus + (UL) ErrorStatus;

  if (++ErrorQueuePtr > NoInErrorQueue - 1)  // If next queue entry GT. max..
    ErrorQueuePtr = 0;                       //  set pointer to 0

  ErrorQueue[ErrorQueuePtr] = ErrorVar;    // Save extended error status

#if 0
    sprintf(tbuff, "Adding to queue, ErrorVar = 0x%lx\n\r", ~((ErrorVar & 0x00FF0000) >> 16));
    Udr_PutNChar(tbuff);
#endif

  /***********************************************************************/
  /* There is a mishap in the mode the error queue operates.  If at the  */
  /* start we have 3 TLG boards mounted in XA1, XB1 and XC1 and two first*/
  /* boards report level error, both errors will be encoded properly by  */
  /* V443 into one byte and will be pushed onto error queue as ONE error.*/
  /* When diagnosing the error queue, no decoding of individual bits is  */
  /* done and only error on the first TLG board will be reported.        */
  /* Note also that function GetErrorString() will not decode individual */
  /* bits and only first error will be reported.                         */
  /***********************************************************************/

  ErrorWindowOn = TRUE;

  CharBlinkLN2(0, OFF);

  WarningLED = ON;    // Errors wil switch ON Warning LED
  UpdateLEDs();

  WriteErrorLN2(0, ErrorVar);         // Write error in display
  ClearRestLN2();

  WriteArrowsField(L_+D_+U_+R_+E_);

  StatusNdx = StatusErrorItem;        // Force menupointer to STATUS-
  Ptr = &status_menu[StatusNdx];      //  ERROR display
  } /* end of GotoErrorStatus() */

/***************************************************************************/
/* GetPT5230SWVersion                                MESSAGE.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 980208                          */
/* Revised:    980215                                       */
/*                                                  */
/*  Function:  Return the SW version of the main- & oscillator-board      */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    string which contains the version as: xx.x-yy.y          */
/***************************************************************************/
void GetPT5230SWVersion(char* string)
  {

  UC tmp=0, error=-1;
 
  while(error)
  	tmp = (UC) RecInt(sync_addr, "GI", 0xFF, &error);
  	
  if (error)
    tmp=0;

  sprintf(string, "%04.1f-%04.1f\0", PT5230SWVersion/10.0, tmp/10.0);
  }
