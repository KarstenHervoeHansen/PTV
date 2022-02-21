// 990818 SAA Adjusted speed and delay in repeat
//

#include "define.h"     			/* Standard PTV definitions */
#include "KeyCodes.h"					/* defines codes used for key queue */
#include "Key_hw.h"     			/* Haedware interface to keyboard   */
#include "disp_02.h"
#include "front.h"


/* Define parameters for keyboard scanning */


#define DebounceTimeout 1   	/* Number of readings identical
                               to the first before keyaccept */

#define RepDelay        900  	/*50 orig Time (in scanrate units)  before repeating */

#define AutoEscDelay    3000 	/* 60 sec = 1 minut */

#define InitRepRate     600  	/* 15 orig Time between each rep (initial value) */
#define Acc2RepRate     600   	/* Time between each rep acc level 2 */
#define Acc3RepRate     600   	/* Time between each rep acc level 3 */
#define Acc4RepRate     600   	/* Time between each rep acc level 4 */

#define Acc2            20  	/* Number of keys sent before changing
                             		to acceleration level 2*/

#define Acc3            100 	/* Number of keys sent before changing
                             		to acceleration level 3*/

#define Acc4            600 	/* Number of keys sent before changing
                             		to acceleration level 4*/

/* Define states for state-machine */
enum KeyStates
  {
	KeyDebounce,
	KeyStable,
	KeyRepeat,
	KeyFree
  };

KeyPress Key;

static UI KeyState, OldKeyCode, RepRate;
static UI KeyCount, KeyTimer;

void keyscan_init( void)
  {
	/* Initialisation part */
  KeyState   = KeyDebounce ;  /* Initial state */
  OldKeyCode = KeyNone;       /* Program last key as 'none' */
  Key.Repeat = RepeatOff;
  RepRate    = InitRepRate;
  KeyTimer   = 1;

  /* Get initial key press */
  /* OR : does above initialisation give the init key as the first ? */
  /* Set up timer for scan rate */
  }

/*  Task for scanning keyboard */
void keyscan(void)
  {
  KeyboardGet(&Key.Code);

  /* Execute state machine */
  switch ( KeyState)
    {
    case KeyDebounce:
	    if (Key.Code != OldKeyCode)
        {
  	    KeyTimer = DebounceTimeout;
        }
      else
        {
    	  if (0 == --KeyTimer)
          {
      	  if ( Key.Code == KeyNone )
            {
        	  KeyState     = KeyFree;
            KeyTimer     = AutoEscDelay;
            }
          else
            {
          	KeyTimer = RepDelay;
            KeyState = KeyStable;
            }

          Key.Repeat = RepeatOff;
          KeyCount   = 1;
          RepRate    = InitRepRate;
          front();
          }
        }
      break;
		case KeyFree:
    	if (Key.Code != OldKeyCode)
        {
      	KeyTimer = DebounceTimeout;
        KeyState = KeyDebounce;
        }
      else
        {
      	if (KeyTimer != 0)
          {
        	KeyTimer--;
          if (KeyTimer == 0)
            {
          	Key.Code = KeyAutoEsc;
            front();
            Key.Code = KeyNone;
            }
          }
        }
      break;
    case KeyStable:
    	if (Key.Code != OldKeyCode)
        {
      	KeyTimer = DebounceTimeout;
        KeyState = KeyDebounce;
        }
      else
        {
      	if (0 == KeyTimer--)
          {
        	KeyState   = KeyRepeat;
          KeyTimer   = RepRate;
          Key.Repeat = Repeat1;
          front();
          KeyCount++;
          }
        }
      break;
    case KeyRepeat:
    	if (Key.Code != OldKeyCode)
        {
      	KeyTimer   = DebounceTimeout;
        KeyState   = KeyDebounce;
        }
      else
        {
      	if (0 == KeyTimer--)
          {
					if (KeyCount == Acc2)
            {
          	Key.Repeat = Repeat2;
            RepRate    = Acc2RepRate;
            }
          if (KeyCount == Acc3)
            {
          	Key.Repeat = Repeat3;
       	    RepRate    = Acc3RepRate;
            }
					if (KeyCount == Acc4)
            {
          	Key.Repeat = Repeat4;
            RepRate    = Acc4RepRate;
            }
          if (KeyCount < 1000)
          	KeyCount++;
          KeyTimer = RepRate;
          front();
		      }
        }
      break;
      default:
        break;
		  }
		OldKeyCode = Key.Code;
  } /* End Keyscan task  */



