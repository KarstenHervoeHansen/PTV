/* ctask.h - RTXC Task include file */

#define NTASKS 14

#define DNTASKS 31

#define KEYSCAN  1 /* Keyboard scan task   */
#define DISPDRIV 2 /* Diplay driver task   */
#define RS232    3 /* RS232 control        */
#define FRONT    4 /* Front panel control  */
#define COMMAND  5 /* Command interperter  */
#define INSTRU   6 /* Instrument control   */
#define ALARM    7 /* Alarm eventlog cntrl */
#define SCAN     8 /* Scanning task        */
#define TPS_SYNC 9 /* Measures locks       */
#define AGC      10 /* Measures level       */
#define BER      11 /* Measures BER         */
#define IN_LEVEL 12 /* Calculates level     */
#define NOISE    13 /* Measures noise level */
#define WATCHDOG 14 /*                      */

/* End of file - ctask.h */
