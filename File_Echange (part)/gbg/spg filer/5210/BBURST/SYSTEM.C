/*
********************************************************************
* Project	: PT5210  Black Burst
* Filename	: SYSTEM.C
* Version	: 1.1
* Purpose	: System setup
* Org.date	: 960703
* Author	: PRC
********************************************************************
HISTORY:
970114 RELEASED 1.1
*/
#include <xa.h>

void syssetup(void)
{
// System set-up
  PSWH= 0x8f;	// highest priority for system
  BTRH= 0xae;	// ae; foer 970107:0xaa; EXT. BUS timing, Data Cycle
  BTRL= 0xc9;	// c9; foer 0xca; EXT. BUS timing, Wr+ALE-pulses+Code
		// ved 0xcd bliver option for langsom til at detekteres!
  PSWL= 0;	// clear flags
  SCR= 0;	// timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
  WDCON= 0;
  WFEED1= 0xa5; // stop watchdog (needs food first)
  WFEED2= 0x5a;
  PSWH= 0x80;  // lowest priority for system
}
