/*
********************************************************************
* Project : PT5210  SYNC
* Filename  : SYSTEM.C
* Version : 1.1
* Purpose : System setup
* Org.date  : 960703
* Author  : PRC
********************************************************************
HISTORY:
970114 RELEASED 1.1
970109 BTRX= 0xaec9 for speed. Demands 70/84 ns PROM access.
*/
#include <xa.h>

void syssetup(void)
  {
  // System set-up
  PSWH   = 0x8f;   // highest priority for system
#if 1              // original values
  BTRH   = 0xae;   // foer 970107:0xff; EXT. BUS timing, Data Cycle
  BTRL   = 0xc9;   // foer 0xca; EXT. BUS timing, Wr+ALE-pulses+Code
#else              // like in Master
  BTRH  = 0xAE;    // To be trimmed (was 0xef) EXT. BUS timing, Data Cycle
  BTRL  = 0xCE;    // To be trimmed (was 0xcf) EXT. BUS timing, Wr+ALE-pulses+Code
#endif
  PSWL   = 0;      // clear flags
  SCR    = 0;      // timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
  WDCON  = 0; 
  WFEED1 = 0xa5;   // stop watchdog (needs food first)
  WFEED2 = 0x5a;
  PSWH   = 0x80;   // lowest priority for system
  }

#if 0              // values for Master
  BCR   = 0x02;    // Set 20 bit BUS, 8 bit DATA
  PSWH  = 0x8F;    // Set system priority high
  BTRH  = 0xAE;    // To be trimmed (was 0xef) EXT. BUS timing, Data Cycle
  BTRL  = 0xCE;    // To be trimmed (was 0xcf) EXT. BUS timing, Wr+ALE-pulses+Code
  PSWL  = 0;       // Clear flags
  SCR   = 0;       // Timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
  PSWH &= 0xF0;    // Set system priority low
#endif