/* DEFINITION FILE */
/* version:	3.X	*/
/* author:	P.Chr.	*/
/* HISTORY:

*/

#include "ver.c"


#define UI unsigned int
#define UC unsigned char
#define UL unsigned long
#define XB	XBYTE

// CONSTANTS:

#if (ntsc==1)
#define IE3_ID_String	"PHILIPS,PM5655/525,,"
#else
#define IE3_ID_String	"PHILIPS,PM5655/625,,"
#endif

// PHASES
#if (ntsc==1)
#define	defsubcpha	0x60
#define defgenphase	0x5BAA
#define phatol		2015
#define defhphzero	0x00034304
#else
#define	defsubcpha	0x60
#define defgenphase	0x64a0
#define phatol		2496
#define defhphzero	0x000422f4
#endif

// OPTIONS ADDRESSES:
#define	opt1adr		0x44
#define	opt2adr		0x48
#define	opt3adr		0xe6
#define	opt4adr		0x42
#define	opt5adr		0x4a

// SYSTEM DEFINITIONS
#define sumadr		0xfffe
// earlier: 0xdffe

#define checkadr	0x1ff0
#define ramofs		0x0000
#define ramerr		8

// IEEE DEFINITIONS
// #define MaxParam 4
#define MaxString 30

// ABSOLUTE ADDRESSES  AND DEFAULT VALUES
// Variables on absolute addresses will not be changed if re-compiling.

// !!DO NOT USE ADR. RAMOFS+0 - IS UNSTABLE AT POWER-TOGGLE
#define romsum		XBYTE[1]
#define systatus	XBYTE[2]

// 1 free byte: 20003

#define pattern		XBYTE[4]
#define group		XBYTE[5]
#define itsgrup		XBYTE[0x6]
#define substpat	XBYTE[0x7]
#define substgrup	XBYTE[0x8]
#define mainmode	XBYTE[0x9]

#define busadr		XBYTE[0x0a]
#define pageno		XBYTE[0x0b]

// 1 free byte: 2000c

/* TTL PORT. Make 2 buffers of 3 bytes and 2 tables of 64 bytes */
#define	rbuf		((char *)(0x2000dL))
#define	wbuf		((char *)(0x20010L))
#define ttlsize		25

// 11 free bytes: 20013-2001c

#define subcphase	XBYTE[0x1d]
#define genphaold	((UI *)0x2001EL)
#define genphase	((UI *)0x20020L)
#define hphzero		((unsigned long *)(0x20022L))
#define hphptr		((UC *)(0x20022L))



/*  SOURCE CODE TEXT STRINGS 7 strings of 8 bytes: */
/* souwords: uses 20080 to 200c7 */

/* 24 bytes for 8 bit source code */
#define cod8		((UC *)0x200c8L)

#define codwsc		((UI *)0x200e0L)
#define codtek		((UI *)0x200e2L)

// free bytes: 200e4 to 200ef

/* TEXT BYTES */
#define cpos		((char **)(0x200edL))
/* 16 H-positions */
#define hpos		((char *)(0x200f0L))



// ********** SYSTEM VARIABLES *******
//extern UI Number;		// number in message from frontplate
//extern UC options;
extern bit Acknowledge;
extern bit llockout;
//extern UI fejl;		// error flags
extern UI errlog;		// error flags to IEEE
extern UC infodat;	// status data 
extern UC infodat2;	// status data 

//extern void Serial0_init();

extern void checkopt(void);	// Find options
extern void globlock(void);
extern void pause(UI ms);	// make a pause
extern void vilock(void);

// ********** SYNC GEN VARIABLES *******
extern UC syncnew;
extern UC genbw;
extern UC syncsource;
extern void defphases(void);	// 
extern void syncmanage(void);	// main read/write routine for sync gen.
extern void autozero(void);
extern void blgenlo(void);	// BG - set bl/wh genlock ON/OFF
extern void gencorse(void);
extern void genlopt(void);
extern void genfine(void);
extern void subfa(void);
extern void syins(void);

// ********** GENERATOR VARIABLES *******
extern UC bufptr;

extern void modesel(void);	// VM -main mode select, send later
extern void pattern_select(void);
extern UC readgen(UC i, UC n, UI m);	//reads one byte from generator with header i,n
extern void sendit(UI n);

// ********** TTL VARIABLES *******
extern UC ttlgrup;	// selected TTL preset-group
extern UC *functab;
extern void ttlexe(void);	// decode commands from TTL and execute
extern void ttlinit(UC mode);	// Sets default assignment of TTL-pins
extern void ttloutin(void);	// output to TTL and read inputs
extern void readttl(void);	// TTL port read routine


// ********** TEMPERATURE VARIABLES *******
#define templimit  	104
				// appr.45gr. sensor - 41 gr. air
#define blowvolt	79
  // Adjust 'blowvolt' to set the lowest blower volt. to appr. 8V (1V pr.12)
#define panictemp	52
				// appr.75gr. sensor - 71 gr. air
#define hottemp		59
				// appr.70gr. sensor - 66 gr. air
extern bit shotdown;
// extern UC tempwarn;

// ROM TABLES 
extern code UC ramdef[];	// Text RAM-chars download table
extern code UC testtxt[];	// Test text table
extern code UC pintab[];	// TTL pin-no. to port-bit-no. conversion table
extern code UC ttlstd[ttlsize];	// TTL-function to port-bit-no. Standard table 


extern char get_char0(void);
extern char get_char1(void);

void PutStr0(const char *st);

extern void copys(UC *s, UC *r, UC n);
