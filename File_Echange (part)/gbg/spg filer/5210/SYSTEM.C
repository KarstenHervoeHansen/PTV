/*
********************************************************************
* Project	: PT5210  Black Burst
* Filename	: SYSTEM.C
* Version	: 0.0	961111
* Purpose	: Table of subc. phases
* Org.date	: 960903
* Author	: PRC
********************************************************************
HISTORY:
961111 Bus speed trimmed to match ROM/RAM acces times.
*/
#include <xa.h>

void syssetup(void)
{
// System set-up
  PSWH= 0x8f;	// set system priority high
  BTRH= 0xae;	// to be trimmed (was 0xef) EXT. BUS timing, Data Cycle
  BTRL= 0xce;	// to be trimmed (was 0xcf) EXT. BUS timing, Wr+ALE-pulses+Code
  PSWL= 0;	// clear flags
  SCR= 0;	// timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
//  WDCON= 0;	// internal watchdog disable
//  WFEED1= 0xa5; // stop watchdog (needs food first)
//  WFEED2= 0x5a;
//  WDRUN= 0;
  PSWH &= 0xf0;  // set system priority low
}
