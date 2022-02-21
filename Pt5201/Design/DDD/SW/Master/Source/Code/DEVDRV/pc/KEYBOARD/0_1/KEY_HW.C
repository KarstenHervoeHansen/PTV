#include "define.h"     /* Standard PTV definitions */
#include "KeyCodes.h"  	/* Include definitions of keyboard related items */
#include <conio.h>      /* getch();  read the PC keyboard */
#include <stdio.h>
#include "key_hw.h"

#include "displ.h"


extern volatile char brkflag;
static char s[32];
int KeyboardGet(unsigned char *scancode)
{
   static char cc = '5';
   static char oc ;
   static char extend,oextend;

   if( kbhit() )
   {
      cc = getch();

      if (cc == 0)
      {
          extend = TRUE;
          cc = getch();
      }
      else
      {
          extend = FALSE;
      }  

      oc = cc;
      oextend = extend;
   }
   else
   {
      extend = oextend;
      cc = oc;
      oc = '5';
      oextend = FALSE;
   }

   if (extend == FALSE)
   {
      switch (cc) {
      case '8': *scancode =KeyUp;
      break;
      case '2': *scancode =KeyDown;
      break;
      case '4': *scancode =KeyLeft;
      break;
      case '6': *scancode =KeyRight;
      break;
      case '1': *scancode =KeyExecute;
      break;
      case '5': *scancode =KeyNone;
      break;
      case 'q': brkflag = 1;
      break;
      case 27: brkflag = 1;
      break;
      case 13: *scancode =KeyExecute;
      break;
      default:

/*               WindowSetPos(0,0,3);
               WindowWrite(" Testing keyboard codes",3);
               WindowClearTo(39,3);
               WindowSetPos(0,1,3);
               WindowWrite("Code received >>> ",3);
               sprintf(s,"%u",cc);
               WindowWrite(s,3);
               WindowClearTo(39,3);
               WindowShow(3);
  */
      break;
      }
   }
   else
   {
      switch (cc) {
      case 72: *scancode =KeyUp;
      break;
      case 80: *scancode =KeyDown;
      break;
      case 75: *scancode =KeyLeft;
      break;
      case 77: *scancode =KeyRight;
      break;
      default:

/*               WindowSetPos(0,0,3);
               WindowWrite(" Testing keyboard codes",3);
               WindowClearTo(39,3);
               WindowSetPos(0,1,3);
               WindowWrite("Code received >>> 00 + ",3);
               sprintf(s,"%u",cc);
               WindowWrite(s,3);
               WindowClearTo(39,3);
               WindowShow(3);
  */
      break;
      }
      
   }   
   return(OK);
}

static UC LEDMirror;

#define  LED0    0x01
#define  LED1    0x02
#define  LED2    0x04
#define  LED3    0x08
#define  LED4    0x10
#define  LED5    0x20
#define  LED6    0x40
#define  LED7    0x80

int KeyLEDset(UC led, int value)
{
   
      /* Manipulate mirror */
      if(value == FALSE)
      {
         
         switch (led)
         {
         case 0:
            LEDMirror |= LED0;
            break;
         case 1:
            LEDMirror |= LED1;
            break;
         case 2:
            LEDMirror |= LED2;
            break;
         case 3:
            LEDMirror |= LED3;
            break;
         case 4:
            LEDMirror |= LED4;
            break;
         case 5:
            LEDMirror |= LED5;
            break;
         case 6:
            LEDMirror |= LED6;
            break;
         case 7:
            LEDMirror |= LED6;
            break;
         default:
            break;   
         } 
      }
      else
      {
         switch (led)
         {
         case 0:
            LEDMirror &= ~LED0;
            break;
         case 1:
            LEDMirror &= ~LED1;
            break;
         case 2:
            LEDMirror &= ~LED2;
            break;
         case 3:
            LEDMirror &= ~LED3;
            break;
         case 4:
            LEDMirror &= ~LED4;
            break;
         case 5:
            LEDMirror &= ~LED5;
            break;
         case 6:
            LEDMirror &= ~LED6;
            break;
         case 7:
            LEDMirror &= ~LED7;
            break;
         default:
            break;   
         } 
      }
//      TransmitIIC(LEDIICadr,1,&LEDMirror,IIC_PTV);
   
   return(OK);
}