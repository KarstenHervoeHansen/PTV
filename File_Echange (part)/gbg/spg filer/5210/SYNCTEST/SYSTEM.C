#include <xa.h>

void syssetup(void)
{
// System set-up
   PSWH= 0x80;	// enable system mode
   PSWH= 0x8f;	// highest priority for system
   BTRH= 0xff;	// 0xff EXT. BUS timing, Data Cycle, 0x9f is min.
   BTRL= 0xca;	// 0x45, 0xef EXT. BUS timing, Wr+ALE-pulses+Code, 0x5f is min.
   PSWL= 0;	// clear flags
   SCR= 0;	// timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
   WDCON= 0;
   WFEED1= 0xa5; // stop watchdog (needs food first)
   WFEED2= 0x5a;
   PSWH= 0x80;  // lowest priority for system
}
