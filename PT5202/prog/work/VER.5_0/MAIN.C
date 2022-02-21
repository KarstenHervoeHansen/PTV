/* rtxcmain.c - C main() module */

#include "define.h"
#include "plddrv.h"
#include "nv_drv.h"
#include "rs232.h"
#include "sio0drv.h"
#include "priori.h"
#include "disp_02.h"

/****************/
#define xtal          29491200    // Crystal frequency for the mainboard
#define Test1LED	    0x01
#define Test2LED      0x02
#define EX0INTLVL     0x0F

code char StartupTxt[] = "PT5202: Startup...";
code char FailTxt[]    = "PLD Load Fail...";


_near UC  SecTimer;   // Seconds timer: updated every second
extern UC  DispTimer;  // Display driver timer: updated every 20ms
_near UI  UserTimer;  // Generel user timer: updated every 20ms
UC DisplayTimer;      // Error display timer: updated every 20ms
UC  UpdateTimer;         // Timer for display animation
bit TimerFlag;           // Control bit for 21 ms timing
/***************/

extern int TestLEDSet( UI LED, bool value);
extern void ptvmain(void);
int TestLEDSet( UI LED, bool value);


void main(void)
  { 

  BCR   = 0x06;    // Set 20 bit BUS, 8 bit DATA
  PSWH  = 0x8F;    // Set system priority high
  BTRH  = 0xBA;    // To be trimmed (was 0xef) EXT. BUS timing, Data Cycle
  BTRL  = 0xEA;    // To be trimmed (was 0xcf) EXT. BUS timing, Wr+ALE-pulses+Code
  PSWL  = 0;       // Clear flags
  SCR   = 0;       // Timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
  PSWH &= 0xF0;    // Set system priority low

  WDCON  = 0;
  WFEED1 = 0xA5;
  WFEED2 = 0x5A;

  uart1_init();
  uart0_init();


  InitDisplay();

  WriteCodeLN1(0, StartupTxt);
  ClearRestLN1();



// Load data from ROM to SPG PLD and reset the PLD
  if (pld_init() == FAIL)
    {
    WriteCodeLN1(0, FailTxt);
    for (;;)
      ;            // do nothing
    }

  IPA0 = EX0INTLVL; //0x0f

  IT0 = 1;   // Set to specify falling edge triggered ext. interrupt
  EX0 = 1;   // Enable external 0 interrupt

  EA = 1;

  ptvmain();
 }

/* end of main.c */
