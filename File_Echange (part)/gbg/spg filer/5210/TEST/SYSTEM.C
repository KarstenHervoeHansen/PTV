#include <xa.h>

void syssetup(void)
{
// System set-up
  PSWH= 0x80;	// enable system mode
  PSWH= 0x8f;	// highest priority for system
  BTRL= 0xef;	// EXT. BUS timing, 0x5f is min.
  BTRH= 0xff;	// do., DATA, 0x9f is min.
  PSWL= 0;	// clear flags
  SCR= 0;	// timer speed prescaler (0 gives 4, 4 gives 16, 8 gives 64)
  WFEED1= 0xa5; // stop watchdog (needs food first)
  WFEED2= 0x5a;
  WDRUN= 0;
  PSWH= 0x80;  // lowest priority for system
}
