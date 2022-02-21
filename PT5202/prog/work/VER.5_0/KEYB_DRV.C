/***************************************************************************/
/*  Copyright DK-Audio A/S 2002                                            */
/*  Project:    PT5202 Digital Video Generator                             */
/*  Module:    KEYB_DRV.C                                                  */
/*  Author:    JK                                                          */
/*  Org. date:  29.05.2002                                                 */
/*  Rev. date:                                                             */
/*  Status:    Version 1.0                                                 */
/*                                                                         */
/*  This module contains the following functions:                          */
/*                                                                         */
/*    bit ButtonReady();                                                   */
/*                                                                         */
/*  Purpose: IIC keyboard driver for 5230 style keyboard                   */
/***************************************************************************/

#include "define.h"
#include "iic.h"
#include "keyb_drv.h"
#include "rs232.h"


extern UC AutoESCTimer;    // Timer for auto return to status display

UC Button;              // The last pressed button
UC OldButton;           // The 2nd last pressed button

UC ButtonCommand;       // The last decoded button-command
UC OldButtonCommand;    // The 2nd last decoded button-command

UC DelayFastRepeat;     //
UC RepeatCounter;       //
UC FastRepeatCount = FastRepeatFreq;  //

//bit AutoESCSignal;       // 1: Return to status display
bit ButtonEnable;          // 1: Enables handling of pressing the keyboard
bit HorizontalRepeat;      // 1: Enable horizontal repetetion.
bit VerticalRepeat;        // 1: Enable vertical repetetion.
bit RepeatOn;              // 1: Repeat function for the pressed button
                           //    is ENABLED
bit CombiOn;               // 1: A combination of the pressed button(s)
                           //    is/are possible

UC RdFrontIIC(UC);
void keyb_init(void);

/***************************************************************************/
/*  ButtonReady                                                 KEYB_DRV.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 950509                                    */
/* Revised:    980524, KEn                                                 */
/*                                                                         */
/*  Function:  Read and decode the keyboard.                               */
/*  Remarks:    --                                                         */
/*  Returns:    TRUE: If new button ready                                  */
/*  Updates:    TBD                                                        */
/***************************************************************************/
bit ButtonReady(UC * butt_buffer) 
  {
  //FeedTheWatchdog;
  Button = RdFrontIIC(KEYB_ADDR);      // Read the keyboard (0x4e)

  if ( ButtonEnable) 
    {
    if ( RepeatOn) 
      {
      if ( Button == OldButton) 
        {
        if ( !RepeatCounter) 
          {
          ButtonCommand = Button;

          if ( DelayFastRepeat) 
            {
            DelayFastRepeat--;
            RepeatCounter = NormalRepeatFreq;
            }
          else
            RepeatCounter = FastRepeatCount;
          *butt_buffer = Button;
          return( TRUE);
          }
        else
          RepeatCounter--;
        }
      else
        RepeatOn = FALSE;
      }                         // End of if (Repeat_On)
    else 
      {
      if ( Button != NOButton) 
        {
        if ( Button == OldButton) 
          {
          //AutoESCSignal = OFF;      // reset AUTO ESCape timer, ie. auto-
          AutoESCTimer = 0;        //  matic return to status display

          switch ( Button) 
            {
            case RIGHTButton:      // RIGHT/LEFT CONDITIONALLY repeat 
            case LEFTButton:
              if ( HorizontalRepeat)
                RepeatOn = ButtonEnable = TRUE;
              else
                ButtonEnable = FALSE;

              DelayFastRepeat = 5;
              RepeatCounter = NormalRepeatFreq;

              ButtonCommand = Button;
              *butt_buffer = Button;
              return( TRUE);

            case UPButton:        // UP/DOWN CONDITIONALLY repeat
            case DOWNButton:
              if ( VerticalRepeat)
                RepeatOn = ButtonEnable = TRUE;
              else
                ButtonEnable = FALSE;

              DelayFastRepeat = 5;
              RepeatCounter = NormalRepeatFreq;
              ButtonCommand = Button;
              *butt_buffer = Button;
              return( TRUE);

            case EXECButton:        // EXECUTE NEVER repeat
            case GENLOCKButton:   //0xBF    // S11
            case BBButton:        //0xFE    // S2
            case SDIButton:       //0xFD    // S4
            case AUDIOButton:     //0xFB    // S6
            case PRESETButton:    //0xF7    // S8
              ButtonEnable = FALSE;
              ButtonCommand = Button;
              *butt_buffer = Button;
              return( TRUE);
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
    if ( Button == NOButton) 
      {        // If no buttons pressed
      ButtonEnable = TRUE;
      ButtonCommand = NOButton;
      OldButton = NOButton;
      RepeatOn = CombiOn = FALSE;
      }
  return( FALSE);
  }
  

void keyb_init(void)
  {  
  OldButtonCommand = ButtonCommand = NOCommand;
  }
  