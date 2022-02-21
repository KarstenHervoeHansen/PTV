/* ctask.c - RTXC Task definitions - pt5765 Rev.1         04/12/99 12:42 */

#include "typedef.h"
#include "rtxstruc.h"

#include "ctask.h"

#define STKSZ1 512
#define STKSZ2 512
#define STKSZ3 512
#define STKSZ4 512
#define STKSZ5 512
#define STKSZ6 512
#define STKSZ7 512
#define STKSZ8 512
#define STKSZ9 512
#define STKSZ10 512
#define STKSZ11 512
#define STKSZ12 512
#define STKSZ13 512
#define STKSZ14 512

extern const TASK near ntasks = NTASKS;

extern const TASK near dntasks = DNTASKS;

static int ks_stk stack1[STKSZ1/sizeof(int)];
static int ks_stk stack2[STKSZ2/sizeof(int)];
static int ks_stk stack3[STKSZ3/sizeof(int)];
static int ks_stk stack4[STKSZ4/sizeof(int)];
static int ks_stk stack5[STKSZ5/sizeof(int)];
static int ks_stk stack6[STKSZ6/sizeof(int)];
static int ks_stk stack7[STKSZ7/sizeof(int)];
static int ks_stk stack8[STKSZ8/sizeof(int)];
static int ks_stk stack9[STKSZ9/sizeof(int)];
static int ks_stk stack10[STKSZ10/sizeof(int)];
static int ks_stk stack11[STKSZ11/sizeof(int)];
static int ks_stk stack12[STKSZ12/sizeof(int)];
static int ks_stk stack13[STKSZ13/sizeof(int)];
static int ks_stk stack14[STKSZ14/sizeof(int)];

extern const TASK near startls[] =
{
   KEYSCAN , /* Keyboard scan task   */
   DISPDRIV, /* Diplay driver task   */
   RS232   , /* RS232 control        */
   FRONT   , /* Front panel control  */
   COMMAND , /* Command interperter  */
   INSTRU  , /* Instrument control   */
   ALARM   , /* Alarm eventlog cntrl */
   SCAN    , /* Scanning task        */
   TPS_SYNC, /* Measures locks       */
   AGC     , /* Measures level       */
   BER     , /* Measures BER         */
   IN_LEVEL, /* Calculates level     */
   NOISE   , /* Measures noise level */
   WATCHDOG, /*                      */
   0  /* null terminated list */
};

extern void keyscan (void);
extern void dispdriv(void);
extern void rs232   (void);
extern void front   (void);
extern void command (void);
extern void instru  (void);
extern void alarm   (void);
extern void scan    (void);
extern void tps_sync(void);
extern void agc     (void);
extern void ber     (void);
extern void in_level(void);
extern void noise   (void);
extern void watchdog(void);

#define ANY_FPU_TASKS 0
#if !defined(FPU) && ANY_FPU_TASKS
#error "Task requiring FPU support is defined, enable FPU support"
#endif


extern const KTCB near rtxktcb[1+NTASKS] =
{
  { (void(*)(void))0    , (char *)0, 0, 0 }, /* null task */
  { keyscan , (char *)stack1, STKSZ1, 1 }, /* KEYSCAN -Keyboard scan task   */
  { dispdriv, (char *)stack2, STKSZ2, 2 }, /* DISPDRIV-Diplay driver task   */
  { rs232   , (char *)stack3, STKSZ3, 5 }, /* RS232   -RS232 control        */
  { front   , (char *)stack4, STKSZ4, 10 }, /* FRONT   -Front panel control  */
  { command , (char *)stack5, STKSZ5, 10 }, /* COMMAND -Command interperter  */
  { instru  , (char *)stack6, STKSZ6, 15 }, /* INSTRU  -Instrument control   */
  { alarm   , (char *)stack7, STKSZ7, 20 }, /* ALARM   -Alarm eventlog cntrl */
  { scan    , (char *)stack8, STKSZ8, 30 }, /* SCAN    -Scanning task        */
  { tps_sync, (char *)stack9, STKSZ9, 35 }, /* TPS_SYNC-Measures locks       */
  { agc     , (char *)stack10, STKSZ10, 40 }, /* AGC     -Measures level       */
  { ber     , (char *)stack11, STKSZ11, 45 }, /* BER     -Measures BER         */
  { in_level, (char *)stack12, STKSZ12, 50 }, /* IN_LEVEL-Calculates level     */
  { noise   , (char *)stack13, STKSZ13, 55 }, /* NOISE   -Measures noise level */
  { watchdog, (char *)stack14, STKSZ14, 60 } /* WATCHDOG-                     */
};

TCB near rtxtcb[1+NTASKS+DNTASKS];

#define NAMMAX 8

extern const char taskname[1+NTASKS][NAMMAX+1] =
{
   "",
   "KEYSCAN",
   "DISPDRIV",
   "RS232",
   "FRONT",
   "COMMAND",
   "INSTRU",
   "ALARM",
   "SCAN",
   "TPS_SYNC",
   "AGC",
   "BER",
   "IN_LEVEL",
   "NOISE",
   "WATCHDOG"
};

/* End of file - ctask.c */
