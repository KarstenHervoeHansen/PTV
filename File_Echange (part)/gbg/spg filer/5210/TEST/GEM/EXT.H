#define UC unsigned char
#define UI unsigned int
#define  ex extern

ex bit alian;
ex bit bulocken;
ex bit epapu;	 //white bar is on active linie
ex bit level;
ex bit lockdet;
ex bit minus;
ex bit window;

ex UC atel, btel, ftel;
ex UC buamav;
ex UI buph;	//burst phase
ex UC black;
ex UC blackav;
ex UI cntenab;
ex UC cvartel;
ex UC errpre;
ex UI eph;
ex UI genphase; 	//wanted genlock fase, updated from frontplate
ex UC godgren;	//indicates which branch in SCH was last used
ex long hinput;
ex long hphzero;
ex UI hpos;
ex long href;
ex UI htel;
ex UC levelcnt;
ex UC linadd;
ex UC lintyp;
ex UI oldph;
ex UI outf;
ex UC papua;	 //pal pulse ampl.
ex UI phdet;
ex long phdiff;
ex UC phgod;	 //filter counter for line7
ex UC schgod;	//filter counter for SCH routine
ex UC schph;	 //sch fase paa input signal
ex UC sphgod;	 //filter counter for line7
ex UI subcpot;
ex UI subcref;
ex UC verr;
ex UC trig;	//trigger niavue for 50 % sync		

// PORTS
ex volatile UC *dualport;	// dual port ram
ex volatile UC freql;	//port for output frequency, low byte 	(sel 3)
ex volatile UC freqh;	//	-	-	-  high byte 	(sel 4)
ex volatile UC outport;	//output  port
ex volatile UC freqload;	//pseudo-port for update of DAC
ex volatile UC typeport;
ex volatile UC ehportl;	//port for read-in of exstern h position, low byte  (sel 7)
ex volatile UC ehporth;	//	-	-	-	-    	 high byte (sel 8)

// TABLES
ex code UC *tableph;	// pointer to phase table
ex code UC *tablepyt;	// pointer to 
ex code UC *tableam;     // pointer to ampl. table
ex code UI *fasekorgg;	// pointer to ampl. table
ex code UI *fasekormm;
ex code UI *faseposgg;
ex code UI *faseposmm;

// STRUCTS
ex volatile UC outs;
//UC buts;
ex volatile UC doflags;

// BITS IN BYTE: SAVESB
extern struct
{ UI	genlocken :1;
  UI	pplocken  :1;
  UI		  :6;
} savesb;

// BITS IN BYTE: OUTSB
extern struct
{ UI	syncav	:1;
  UI	lockbit	:1;
  UI	spgint	:1;
  UI	buav	:1;
  UI	pal	:1;
  UI	slowlock :1;
  UI	secam	:1;
  UI	tau	:1;
} outsb;

// BITS IN BYTE: DOFLB
extern struct
{ UI	dolock	:1;
  UI	dosch	:1;
  UI	domode	:1;
  UI	doknap	:1;
  UI	doline7	:1;
  UI	nsf	:1;
  UI		:2;
} doflb;

// FUNCTIONS
extern void burstfase(void);
extern void hlock(void);
extern void intern(void);
extern void hfasec(void);
extern void hfasef(void);






/* GAMMELT

// BUFFER **
UC combuf[8];	//buffer for commands from master uP


// BYTE VARIABLES IN BIT AREA *
UC saves;
volatile UC outs @ 0x21;
//UC buts;
volatile UC doflags @ 0x20;

// other bits **
bit minus;	 //flag for negative medium result
// sbit butena;	 //enables button read. Is set every frame
bit window;	 //flag for field pulse in window
bit lockdet;	 //
bit bulocken;	 //
//bit buok;	 //burst amplitude is ok		<12/10d
// bits in byte: saves **
bit genlocken; 	//genlock allowed
bit pplocken;  	//lock to 8 fields pulse allowed
bit alian;	 //
UC buam;
UC blackav;	//black value average
UC buamav;	 //
UC atel;	 //taeller rundt til 5 paa line  		| disse 3 signaler
UC black;	 //gennemsnits sort vaerdi
UC btel;	//taeller rundt til 125 paa carry fra a	| udgoer tilsammen en
UC cvartel;	//taeller varighed til outdata loades fra tabel
UC papulock;	//taeller til at finde ud af om der er 8 fields puls
UC sync;	//gennemsnits sync vaerdi
UC verr;	//taeller der taeller ned paa gode fields
UC speeddel; 	//counter for button speeder delay
UC speedval; 	//button speeder rate
UC lintyp;	//last read line type
UC dpoint;	//data pointer in table
UC savacc;	//store for accu
bit cntenab;	//line counter enable
// bits in byte: outs
struct
{ UI	genlocken :1;
  UI	pplocken  :1;
  UI		  :6;
} savesb @ 22;

// bits in byte: outs
struct
{ UI	syncav	:1;
  UI	lockbit	:1;
  UI	spgint	:1;
  UI	buav	:1;
  UI	pal	:1;
  UI	slowlock :1;
  UI	secam	:1;
  UI	tau	:1;
} outsb @ 21;

// bits in byte: doflags
struct
{ UI	dolock	:1;
  UI	dosch	:1;
  UI	domode	:1;
  UI	doknap	:1;
  UI	doline7	:1;
  UI	nsf	:1;
  UI		:2;
} doflb @ 20;
// PORT ADDRESSES **
volatile UC dualport @	0x4000;	// dual port ram
volatile UC freql @	0x6000;	//port for output frequency, low byte 	(sel 3)
volatile UC freqh @	0x6080;	//	-	-	-  high byte 	(sel 4)
volatile UC outport @	0x4000;	//output  port
volatile UC freqload @  0x8000;	//pseudo-port for update of DAC
volatile UC typeport @	0x2000;	//port for output of linietype 		(sel 5)

volatile UC ehportl  @	0x2000;	//port for read-in of exstern h position, low byte  (sel 7)
volatile UC ehporth  @	0x0000;	//	-	-	-	-    	 high byte (sel 8)
// volatile UC butport  @	0xf400;	//	-	- of buttons		(sel 9)

// BYTE Variables in Byte area *
//UC tid1;	//maalt tidsforsk. course input/output sync enhed clockpulser
//UC tid2;	//maalt tidsforsk. fine input/output sync enhed clockpulser/256 

UC phgod;	 //filter counter for line7
UC sphgod;	 //filter counter for line7
UC ftel;	 	//taeller rundt til 4 paa carry fra b	| 8 field linie taeller
//UC hamp;	 //sync amplityde
UI htel;	 //H-counter
// UC htello;	 //H-counter (lo)
// UC htelhi;	 //H-counter (lo)
//UC herr:	 //taeller der taeller ned ved gode lines
//buerr;	//counter for errors in burst
UC linadd;	//correction value for LINTYP output
UC errpre;	//error preset variable
// VARIABLES 16BIT **
UI eph;	 	//external burst phase
UI buph;
UI hpos;	//position of extern line
UI i;	 	//local variable
UI j;	 	//local variable
UI oldph;	//old ext. burst phase
UI outf;	//output freq.
UI phdet;	//phasedet. var. for use in freq.lock

// OBS: 'combuf' occupies from 5ah - 5fh
UI x;		//immediate result from burstfase
UI y;		//immediate result from burstfase
UI z;		//immediate result from burstfase

UI subcpot;	//wanted subc genlock phase, derived from genphase
UI subcref;	//subc phase in aktual line

// MORE 1-BYTE VARIABLES

// VARIABLES 24BIT 
long scratc;	 //scratch pad, 3 bytes
	
long hinput;	 //maalt horisontal fase af input signal
			// maalt som ram position
			// bestaar af 3 byte 
			// 2 der giver ram position i clockpulser
			// 1 der giver underindelingen
long href;	 //oensket horisontal fase af input signal
			// maalt som ram position
			// bestaar af 3 byte 
			// 2 der giver ram position i clockpulser
			// 1 der giver underindelingen
long hphzero;	 //calibrerings data til indstilling af genlock fase
			// maalt som ram position
			// bestaar af 3 byte 
			// 2 der giver ram position i clockpulser
			// 1 der giver underindelingen
long phdiff;		//fase difference mellem input og oensket fase
			// maalt som ram position
			// bestaar af 3 byte 
			// 2 der giver ram position i clockpulser
			// 1 der giver underindelingen


// MORE 1-BYTE VARIABLES

UC oldread;	 //last legal kommun. port read value
UC palav;	 //

#if gvers==1
UC secamav;	 //for secam filter

#else
UC oldpalm;	 //
UC diff;	 //for PALM filter
UC palm;	 // Do.

#endif


// BITS AND FLAGS **
/
bit comerr;	//flag for communication error
bit cmdin;	//flag for command received

bit level;  	//level shift bit
// bit xnneg;	//flag for xn negative (in Burstfase)
// bit ynneg;	//flag for yn negative (in Burstfase)
// bit yneg;	//flag for y negative (in Burstfase)



/
bit syncav;	//indicates sync available
bit lockbit;	//indicates spg genlocked
bit spgint;	//indicates (0 = oven reference)
bit buav;	//indicates burst available
bit pal;	//indicates paln
bit slowlock;	//indicates 623,624,626,627 lines found
bit secam;	//indicates secam
bit tau;	//indicates slow phase lock time constant


// bits in byte: buts
sw1	 bit  06bh	//int/ext switch button
sw2	 bit  06ch	//
sw3	 bit  06dh	//button for genlock phase down
sw4	 bit  06eh	//	-	-	  up
sw5	 bit  06fh	//pplock button


// PORT BITS **
sbit P3_0 @ 0x398;
sbit P3_5 @ 0x39d;

// tables
ex code UC tblph	// pointer to phase table
ex code UC tblpyt	// pointer to 
ex code UC tableam	// pointer to ampl. table

//pref1	 equ 		//entry in tabelt for field 1 at field1 interrupt
//pref2	 equ 		//	--	-	2	-	-
//pref3	 equ 		//	--	-	3	-	-
//pref4	 equ 		//	--	-	4	-	-
//startf1 equ 		//start point in tabelt for field 1
//startf2 equ 		//	-	-	-	2
//startf3 equ 		//	-	-	-	3
//startf4 equ 		//	-	-	-	4

//tabelf	 equ 		//from sinus/cosinus to burst phase
//tabela	 equ 		//from sinus/cosinus to burst amplitude

// EXTERN RAM ADDRESSES (in dual-port RAM)
xsaves	  equ	0xffe0	//flags
xgenphase equ	0xffe2	//wanted genlock fase, updated from frontplate
xhphzero  equ	0ffe4h	//calibrerings data til indstilling af genlock fase
*/
