// 990818 SAA Adjusted speed and delay in repeat
//

#include "rtxcobj.h"

#include "define.h"     			/* Standard PTV definitions */
#include "KeyCodes.h"					/* defines codes used for key queue */
#include "Key_hw.h"     			/* Haedware interface to keyboard   */


/* Define parameters for keyboard scanning */

#define ScanRate       	20 		/* Scanrate in Milliseconds */

#define DebounceTimeout 1   	/* Number of readings identical
                               to the first before keyaccept */

#define RepDelay        50  	/* Time (in scanrate units)  before repeating */

#define AutoEscDelay    3000 	/* 60 sec = 1 minut */

#define InitRepRate     15  	/* Time between each rep (initial value) */
#define Acc2RepRate     15   	/* Time between each rep acc level 2 */
#define Acc3RepRate     15   	/* Time between each rep acc level 3 */
#define Acc4RepRate     15   	/* Time between each rep acc level 4 */

#define Acc2            10  	/* Number of keys sent before changing
                             		to acceleration level 2*/

#define Acc3            50 	/* Number of keys sent before changing
                             		to acceleration level 3*/

#define Acc4            300 	/* Number of keys sent before changing
                             		to acceleration level 4*/


/*#define Acc2            10  	 Number of keys sent before changing
                             		to acceleration level 2*/

/*#define Acc3            19  	 Number of keys sent before changing
                             		to acceleration level 3*/

/*#define Acc4            28  	 Number of keys sent before changing
                             		to acceleration level 4*/

/* Define states for state-machine */
enum KeyStates{
	KeyDebounce,
	KeyStable,
	KeyRepeat,
	KeyFree
};

/* Task definition */
TStream *KeyScanStream;

static TTimer *KeyScanTimer;

/*  Task for scanning keyboard */
void keyscan( void)
{
	/* Internal vars */
  static UC KeyState, OldKeyCode, RepRate;
  static KeyPress Key;
  static UI KeyCount, KeyTimer;

	KeyScanStream = newTStream( 2, 10);
	KS_purgequeue( KeyScanStream->ID);

	KeyScanTimer = newTTimer();
	KeyScanTimer->SetPeriod( KeyScanTimer, 20);  //20); <- original value !!!!!!!!!!!!

	/* Initialisation part */
  KeyState   = KeyDebounce ;  /* Initial state */
  OldKeyCode = KeyNone;       /* Program last key as 'none' */
  Key.Repeat = RepeatOff;
  RepRate    = InitRepRate;
  KeyTimer   = 1;

  /* Get initial key press */
  /* OR : does above initialisation give the init key as the first ? */
  /* Set up timer for scan rate */

	/*  Task loop start */
	for (;;)
  {
  	/* Wait for timeout */
		KeyScanTimer->TimeOut->Wait( KeyScanTimer->TimeOut, 0);

		/* Get actual key scan reading */
    switch ( KeyboardGet( &Key.Code) )
    {
    	case OK:
      	break;

			default:
      	break;
    }/*end switch*/

    /* Execute state machine */
    switch ( KeyState){
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
            	KeyTimer     = RepDelay;
              KeyState     = KeyStable;
            }

            Key.Repeat   = RepeatOff;
            KeyCount     = 1;
            RepRate    = InitRepRate;
						KeyScanStream->Put( KeyScanStream, &Key);
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
							KeyScanStream->Put( KeyScanStream, &Key);
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
          	KeyState = KeyRepeat;
            KeyTimer = RepRate;
            Key.Repeat = Repeat1;
						KeyScanStream->Put( KeyScanStream, &Key);
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

						KeyScanStream->Put( KeyScanStream, &Key);
					}
        }
        break;

        default:
	        break;
			}/*end switch*/

			OldKeyCode = Key.Code;
		}
		/*End task loop*/
} /* End Keyscan task  */



