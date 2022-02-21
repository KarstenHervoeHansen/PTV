/****************************************************************************/
/* MODULE:                                                                  */
/*  front.c -                                                                */
/****************************************************************************/
/* FUNCTIONS:                                                               */
/*                                                                          */
/*                                                                          */
/* TASKS:                                                                   */
    void front(void);
/*                                                                          */
/* NOTES:                                                                   */
/****************************************************************************/
/*
 *   PTV software for PT5201    
 *   Copyright (c) 
  *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/

#include "define.h"
#include "instru.h"
#include "KeyScan.h"
#include "KeyCodes.h"          // defines codes used for key queue
#include "Key_hw.h"           // Hardware interface to keyboard 
#include "led_hw.h"           // Hardware interface to LEDs
//#include "menutree.h"
#include "disp_02.h"

enum KeyStates        { StateNone = 0, StateDisplay, StateSelect, StateReset   };

extern KeyPress Key;

static RepeatCnt;
static KeyState;

/**************************************************************************/
/* front                                                         FRONT.C  */
/*                                                                        */
/* Author:   Kim Engedahl, DEV, 000609                                    */
/* Revised:   000620, KEn, DEV                                             */
/*                                                                        */
/* Function:                                                               */
/* Remarks:                                                                */
/* Returns:                                                                */
/* Updates:                                                                */
/**************************************************************************/

void front_init(void)
  {
  Key.Code = KeyNone;
  }

void front(void)
  {
    switch (Key.Code)
      {
      case KeyNone:
        switch (KeyState)
          {
          case StateReset:
            ResetInstrument();
            break;
          case StateSelect:
            if (Settings.ActivePreset)
              RecallPreset(Settings.ActivePreset);
            break;
          }
        KeyState = StateNone;
        waitms(200);               // Display LEDs for 100ms before returning to normal
        SystemsLEDSet(NormLEDs, 0);
        break;

      case KeyExecute:
        switch (Key.Repeat)
          {
          case RepeatOff:              // Display active preset, (if any)
            SystemsLEDSet(PresetLEDs, Settings.ActivePreset);
            RepeatCnt = 0;
            KeyState = StateDisplay;
            break;
          case Repeat2:                // Display/select next preset when releasing button
            if (RepeatCnt-- == 0)
              {
              RepeatCnt = 2;
              if (++Settings.ActivePreset > NoOfPresets)
                Settings.ActivePreset = 0;
              SystemsLEDSet(PresetLEDs, Settings.ActivePreset);
              }
            KeyState = StateSelect;
            break;
          case Repeat3:                // Reset the unit when releasing button
            KeyState = StateReset;
            SystemsLEDSet(ResetLEDs, 0);
#if 0
            ResetInstrument();
            KeyState = StateNone;
            waitms(100);               // Display LEDs for 100ms before returning to normal
            SystemsLEDSet(NormLEDs, 0);
#endif
            break;
            }
          break;
      }
  }
