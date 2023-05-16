/*
********************************************************************
* Project	: PT5210  SYNC
* Filename	: SYNC.H
* Version	: 1.2	961030
* Purpose	: Variables definitions
* Org.date	: 960506
* Author	: PRC
Special:   This file should be included after the file 'equ.h'.
********************************************************************
HISTORY:

970120 Changed name from vars.c.
970114 RELEASED 1.1
*/

// BUFFER
near UC combuf[8];	//buffer for commands from master uP


// BYTE VARIABLES IN BIT AREA
// UC saves;
volatile UC doflags @ 0x20;
volatile UC outs @ 0x21;
volatile UC modebits @ 0x23;

// OTHER BITS
bit alian;	//
bit bulocken;	//
bit cntenab;	// line counter enable
bit epapu;	// white bar is on active linie
bit level;  	// level shift bit
bit lockdet;	//
bit minus;	// flag for negative medium result
bit window;	// flag for field pulse in window

// BYTE VARIABLES
NUC atel;	// taeller rundt til 5 paa line  		| disse 3 signaler
NUC black;	// gennemsnits sort vaerdi
NUC blackav;	// black value average
NUC btel;	// taeller rundt til 125 paa carry fra a	| udgoer tilsammen en
NUC buam;
NUC buamav;	//
NUI buph;
NUC cvartel;	// taeller varighed til outdata loades fra tabel
NUC dpoint;	// data pointer in table
NUI eph;	// external burst phase
NUC errpre;	// error preset variable
NUC ftel;	// taeller rundt til 4 paa carry fra b	| 8 field linie taeller
NUC genmode;	// genlock mode
NUI genphase_g;	// wanted genlock fase, updated from frontplate
NUI genphase_m;	// wanted genlock fase M
NUI genphase_d6; // wanted genlock fase D1/625
NUI genphase_d5; // wanted genlock fase D1/525
NUC godgren;	// indicates which branch in SCH was last used
NUI hpos;	// position of extern line
NUI htel;	// H-counter
NUI i;	 	// local variable
NUI j;	 	// local variable
NUC levelcnt;	// counter for good/bad fields
NUC linadd;	// correction value for LINTYP output
NUC lintyp;	// last read line type
NUI oldph;	// old ext. burst phase
NUC oldread;	// last legal kommun. port read value
NUI outf;	// output freq.
NUC palav;	//
NUC papua;	// pal pulse ampl.
NUC papulock;	// taeller til at finde ud af om der er 8 fields puls
NUI phdet;	// phasedet. var. for use in freq.lock
NUC phgod;	// filter counter for line7
NUC savacc;	// store for accu
NUC schgod;	// filter counter for SCH routine
NUC schph;	// sch fase paa input signal
NUC sersave;	// storage for V24 received byte
NUC speeddel; 	// counter for button speeder delay
NUC speedval; 	// button speeder rate
NUC sphgod;	// filter counter for line7
NUI subcpot;	// wanted subc genlock phase, derived from genphase
NUI subcref;	// subc phase in aktual line
NUC sync;	// gennemsnits sync vaerdi
NUC trig;	// trigger niveau for 50 % sync		
NUC verr;	// taeller der taeller ned paa gode fields
NUI x;		// immediate result from burstfase
NUI y;		// immediate result from burstfase
NUI z;		// immediate result from burstfase


NUC secamav;	 // for secam filter i G
NUC oldpalm;	 // for PAL-M
NUC diff;	 // for PAL-M filter
NUC palm;	 // Do.


// BITS IN BYTE: DOFLAGS(@20)
volatile bit	doline7	@ 0x101;
volatile bit	dosch	@ 0x102;
volatile bit	dolock	@ 0x103;
volatile bit	domode	@ 0x104;
volatile bit	docomm	@ 0x105;
volatile bit	nsf	@ 0x106;

// BITS IN BYTE outsb (@21): 
volatile bit	syncav	@ 0x108;
volatile bit	lockbit	@ 0x109;
volatile bit	spgint	@ 0x10a;
volatile bit	buav	@ 0x10b;
volatile bit	pal	@ 0x10c;
volatile bit	slowlock @ 0x10d;
volatile bit	secam	@ 0x10e;
volatile bit	tau	@ 0x10f;

// BITS IN BYTE savesb(@22): 
volatile bit genlocken @ 0x110;
volatile bit pplocken  @ 0x111;

// BITS IN BYTE MODEBITS (@23): 
volatile bit	g_sel		@ 0x118;
volatile bit	int10_sel	@ 0x119;
volatile bit	freq_sel	@ 0x11a;
volatile bit	a10m_sel	@ 0x11b;
volatile bit	d1_sel		@ 0x11c;
volatile bit	intern_sel	@ 0x11d;
volatile bit	bulock_sel	@ 0x11e;

// PORT ADDRESSES
char dualport @ 0x4000;	// dual port ram;
char * near dual = &dualport;
volatile UC freql @	0x6000;	//port for output frequency, low byte 	(sel 3)
volatile UC freqh @	0x6080;	//	-	-	-  high byte 	(sel 4)
volatile UC outport @	0x4fff;	//output  port
volatile UC freqload @  0x8000;	//pseudo-port for update of DAC
volatile UC typeport @	0x2000;	//port for output of linietype 		(sel 5)

volatile UC ehportl  @	0x2001;	//port for read-in of exstern h position, low byte  (sel 7)
volatile UC ehporth  @	0x0201;	//	-	-	-	-    	 high byte (sel 8)



// VARIABLES 24BIT
near UL scratc;	 // scratch pad, 3 bytes
	
near long hinput;	 // maalt horisontal fase af input signal
			// maalt som ram position
			// bestaar af 3 byte 
			// 2 der giver ram position i clockpulser
			// 1 der giver underindelingen
near long href;	 // oensket horisontal fase af input signal
			// maalt som ram position
			// bestaar af 3 byte 
			// 2 der giver ram position i clockpulser
			// 1 der giver underindelingen
near long hphzero_g;	//calibrerings data til indstilling af genlock fase
near long hphzero_m;	// maalt som ram position
			// bestaar af 3 byte 
			// 2 der giver ram position i clockpulser
			// 1 der giver underindelingen
near long phdiff;		//fase difference mellem input og oensket fase
			// maalt som ram position
			// bestaar af 3 byte 
			// 2 der giver ram position i clockpulser
			// 1 der giver underindelingen


// PORT BITS
// sbit P3_0 @ 0x398;
// sbit P1_5 @ 0x38d;

// TABLES
code UC * near tableph;
extern code UC fase[];

code UC * near tablepyt;
extern code UC pytg[];
extern code UC pytm[];

code UC *near tableam;
extern code UC divi[];

code UI * near fasekor;
extern code UI fasekorg[];
extern code UI fasekorm[];

code UI * near fasepos;
extern code UI faseposg[];
extern code UI faseposm[];
