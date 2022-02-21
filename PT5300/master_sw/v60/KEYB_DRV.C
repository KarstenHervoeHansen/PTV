/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:   KEYB_DRV.C                                  */
/*  Author:   Kim Engedahl, DEV                             */
/*  Org. date:  980203                                    */
/*  Rev. date:  980616, KEn, DEV                              */
/*  Status:   Version 1.0                                 */
/*                                                  */
/*  This module contains the following functions:                 */
/*                                                  */
/*    UC UpdateLEDs();                                    */
/*    void UpdateGrpLEDs(UC group);                          */
/*    bit ButtonReady();                                  */
/*                                                  */
/*  Changes:                                            */
/* 980616: Released as 4008 002 06812                         */
/* 980514: Released as 4008 002 06811                         */
/***************************************************************************/

/***************************************************************************/
/* Include files:                                       */

#include <xa.h>
#include "mas.h"
#include <string.h>
#include <stdio.h>
#include "xaexprt.h"


#include "keyb_drv.h"
#include "iic.h"
#include "tables.h"
#include "message.h"

UC Button;              // The last pressed button
UC OldButton;           // The 2nd last pressed button

UC ButtonCommand;         // The last decoded button-command
UC OldButtonCommand;        // The 2nd last decoded button-command

UC DelayFastRepeat;       //
UC RepeatCounter;         //
UC FastRepeatCount = FastRepeatFreq;  //

bit ButtonEnable;         // 1: Enables handling of pressing the keyboard
bit HorizontalRepeat;     // 1: Enable horizontal repetetion.
bit VerticalRepeat;       // 1: Enable vertical repetetion.
bit RepeatOn;           // 1: Repeat function for the pressed button
                    //  is ENABLED
bit CombiOn;            // 1: A combination of the pressed button(s)
                    //  is/are possible
static char tbuff[80];     /* temporary buffer used for printing messages */

/***************************************************************************/
/*  UpdateLEDs                                     KEYB_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 960709                       */
/* Revised:   980524, KEn                                   */
/*                                                  */
/*  Function: Update the eight front plate LEDs.                  */
/*  Remarks:    --                                        */
/*  Returns:    System error code, i.e. type incl. IIC specific error.    */
/*  Updates:    --                                        */
/***************************************************************************/
UC UpdateLEDs()
  {
  UC tmp = 0xFF;

  FeedTheWatchdog;

 if (WarningLED)   // If WARNINGS .. 
  tmp &= 0xFE;     //  turn on warning LED

  if (UnlockedLED)  // If UNLOCKED ..
    tmp &= 0xFD;    //  turn on unlocked LED

  if (GenlockLED)
    tmp &= 0xFB;

  if (!WrFrontIIC(led_addr, tmp))
    return(KeyboardErrorType + IICErrorCode);

  return(NoIICError);
  }

/***************************************************************************/
/*  ButtonReady                                    KEYB_DRV.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 950509                       */
/* Revised:   980524, KEn                                   */
/*                                                  */
/*  Function: Read and decode the keyboard.                     */
/*  Remarks:    --                                        */
/*  Returns:    TRUE: If new button ready                       */
/*  Updates:    TBD                                     *
/***************************************************************************/
bit ButtonReady()
  {
  FeedTheWatchdog;

  if (!DoKeyboardScan)
    return(FALSE);

  DoKeyboardScan = FALSE;

  Button = RdFrontIIC(keyb_addr);      // Read the keyboard

  if (IICErrorCode)              // In case of any errors...
    return(FALSE);             //  discard the read value
  
  if (ButtonEnable)
    {
    if (RepeatOn)
      {
      if (Button == OldButton)
        {
        if (!RepeatCounter)
          {
          ButtonCommand = Button;

          if (DelayFastRepeat)
            {
            DelayFastRepeat--;
            RepeatCounter = NormalRepeatFreq;
            }
          else
            RepeatCounter = FastRepeatCount;

          return(TRUE);
          }
        else
          RepeatCounter--;
        }
      else
        RepeatOn = FALSE;
      }                         // End of if (Repeat_On)
    else
      {
      if (Button != NOButton)
        {
        if (Button == OldButton)
          {
          AutoESCSignal = OFF;      // reset AUTO ESCape timer, ie. auto-
          AutoESCTimer  = 0;        //  matic return to status display
#if 0
   sprintf(tbuff, "Button = %x\n", (int)Button);
   Udr_PutNChar(tbuff);
#endif
          switch (Button)
            {
            case RIGHTButton:     // RIGHT/LEFT CONDITIONALLY repeat 
            case LEFTButton:
              if (HorizontalRepeat)
                RepeatOn = ButtonEnable = TRUE;
              else
                ButtonEnable = FALSE;

              DelayFastRepeat = 5;
              RepeatCounter   = NormalRepeatFreq;

              ButtonCommand   = Button;
              return(TRUE);

            case UPButton:        // UP/DOWN CONDITIONALLY repeat
            case DOWNButton:
              if (VerticalRepeat)
                RepeatOn = ButtonEnable = TRUE;
              else
                ButtonEnable  = FALSE;

              DelayFastRepeat = 5;
              RepeatCounter   = NormalRepeatFreq;

              ButtonCommand   = Button;
              return(TRUE);

            case EXECButton:        // EXECUTE NEVER repeat

            case OUTPUTButton:      // OUTPUT NEVER repeat
            case PRESETButton:      // PRESET NEVER repeat

            case GENLOCKButton:
              ButtonEnable  = FALSE;
              ButtonCommand = Button;
              return(TRUE);
            }
          }
        else
          OldButton = Button;
        }
      else
        OldButton = NOButton;
      }
    }
  else 
    if (Button == NOButton)
      {        // If no buttons pressed
      ButtonEnable       = TRUE;
      ButtonCommand      = NOButton;
      OldButton          = NOButton;
      RepeatOn = CombiOn = FALSE;
      }
  return(FALSE);
  }
