/*
********************************************************************
* Project : PT5210  SYNC
* Filename  : EXT.H
* Version : 1.2 961030
* Purpose : Ext. Variables definitions
* Org.date  : 960506
* Author  : PRC
Special:   This file should be included after the file 'equ.h'.
********************************************************************
HISTORY:

970114 RELEASED 1.1
*/

ex bit alian;
ex bit bulocken;
ex bit epapu;  //white bar is on active linie
ex bit level;
ex bit lockdet;
ex bit minus;
ex bit window;
ex bit g_sel;
ex bit d1_sel;

ex NUC atel, btel, ftel;
ex NUC buamav;
ex NUI buph;  //burst phase
ex NUC black;
ex NUC blackav;
ex bit cntenab;
ex NUC cvartel;
ex NUC errpre;
ex NUI eph;
ex NUC genmode;
ex NUI genphase_g; //wanted genlock fase, updated from frontplate
ex NUI genphase_m; //wanted genlock fase, updated from frontplate
ex NUC godgren;  //indicates which branch in SCH was last used
ex near long hinput;
ex near long hphzero_g;
ex near long hphzero_m;
ex near long href;
ex long phdiff;
ex NUI hpos;
ex NUI htel;
ex NUC levelcnt;
ex NUC linadd;
ex NUC lintyp;
ex NUI oldph;
ex NUI outf;
ex NUC papua;  //pal pulse ampl.
ex NUI phdet;
ex NUC phgod;  //filter counter for line7
ex NUC schgod;  //filter counter for SCH routine
ex NUC schph;  //sch fase paa input signal
ex NUC sphgod;   //filter counter for line7
ex NUI subcpot;
ex NUI subcref;
ex NUC verr;
ex NUC trig;  //trigger niavue for 50 % sync    
/************************** JK ***********************************/
/* variables to hold Timer0 reload values for G and M            */
#if 1
ex near ui vlenmin_g;
ex near ui vlenmin_m;
#endif

// PORTS
ex char * near dual;  // dual port ram
ex volatile UC freql; //port for output frequency, low byte   (sel 3)
ex volatile UC freqh; //  - - -  high byte  (sel 4)
ex volatile UC outport; //output  port
ex volatile UC freqload;  //pseudo-port for update of DAC
ex volatile UC typeport;
ex volatile UC ehportl; //port for read-in of exstern h position, low byte  (sel 7)
ex volatile UC ehporth; //  - - - -      high byte (sel 8)

// TABLES
ex code UC *tableph;  // pointer to phase table
ex code UC *tablepyt; // pointer to 
ex code UC *tableam;     // pointer to ampl. table
ex code UI *fasekor;  // pointer to ampl. table
ex code UI *fasepos;
ex code UC tablestart_g[];
ex code UC tablestart_m[];


// PACKAGED BITS
ex UC outs;
ex UC doflags;
ex UC modebits;

// BITS IN BYTE: DOFLAGS:
extern bit  dolock;
extern bit  dosch;
extern bit  domode;
extern bit  doknap;
extern bit  doline7;
extern bit  nsf;

// BITS IN BYTE outsb:
extern bit  syncav;
extern bit  lockbit;
extern bit  spgint;
extern bit  buav;
extern bit  pal;
extern bit  slowlock;
extern bit  secam;
extern bit  tau;

// BITS IN BYTE savesb:
extern bit genlocken;
extern bit pplocken;

// BITS IN BYTE MODEBITS: 
extern bit  g_sel;
extern bit  int10_sel;
extern bit  freq_sel;
extern bit  a10m_sel;
extern bit  d1_sel;
extern bit  intern_sel;
extern bit  bulock_sel;
extern bit  gps_sel;

// FUNCTIONS
extern void burstfase(void);
extern void freqfase(void);
extern void hlock(void);
extern void intern(void);
extern void hfasec(void);
extern void hfasef(void);
