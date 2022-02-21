typedef struct keystruk {
    UC Code;
    UC Repeat;
} KeyPress;

/* Function used to wait for specified keys in dialog windows		*/
/* ONLY function started from the front TASK is allowed to use this */
/* function . The function is implemented in front.c  		        */
/* keys are a zero terminated string of 'Code's                     */
/* The function updates the 'arrowfield' indicating which keys      */

//KeyPress WaitKey(UC *keys);

/* following defines are valid for 'KeyPress.Code' */

#define KeyUp		0xDD
#define KeyDown		0xDB
#define KeyLeft		0xDE
#define KeyRight	0xD7
#define KeyExecute	0xCF
#define KeyS6		0xFE
#define KeyS7		0xFD
#define KeyS8		0xFB
#define KeyS9		0xF7
#define KeyS10		0xEF
#define KeyS11		0xBD
#define KeyS12		0xBB
#define KeyS13		0xB7
#define KeyS14		0xAF

/* Keys S15 - S18 are identical codes to keys S7 - S10 */

#define KeyS15		KeyS7
#define KeyS16		KeyS8
#define KeyS17		KeyS9
#define KeyS18		KeyS10
#define KeyNone		0xFF
#define KeyAutoEsc	0x1b      /* Code used to report autoescape timeout */
							/* Code can be disabled global */
#define KeyFastEsc  0x1a      /* Used as fast timeout (few seconds) on display message */
#define KeyErrorBus	0x02	  /* report hardware buserror as keycode    */
#define KeyErrorAccess	0x03  /* report hardware accesserror as keycode */

/* multiple keys pressed, only for arrows and execute
   codes can be created using binary AND on the codes */

#define KeyResetAll	(KeyLeft & KeyRight & KeyExecute)
#define KeyResetSoft	(KeyLeft & KeyRight)

/* following defines are valid for 'KeyPress.Repeat' */
#define RepeatOff    0
#define Repeat1      1
#define Repeat2      2
#define Repeat3      3
#define Repeat4      4
