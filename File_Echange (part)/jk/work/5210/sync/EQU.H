/*
********************************************************************
* Project	: PT5210  SYNC
* Filename	: EQU.H
* Version	: 1.2
* Purpose	: Variables definitions
* Org.date	: 960506
* Author	: PRC
********************************************************************
HISTORY:
970120 changed name from equ.c.
970114 RELEASED 1.1
961014 phmar and sch61 divided into G and M versions
*/


#define NUC near unsigned char
#define NUI near unsigned int
#define UC unsigned char
#define UI unsigned int
#define UL unsigned long
#define sbit static bit
#define ex extern

sbit P3_0 @ 0x398;
sbit P3_5 @ 0x39d;

#define master_addr 0x10
	// V24 address of master.

#define ns P3_0
	//flag, incomming sync is bad

#define iicdiff P3_5
	//flag, master has read the port

//  CONSTANTS
#define acal	   2	// a-value for datatreatment
#define add1	   7	// V-lock correction value for LINADD (use in LINE7)
#define add2	   5	// V-lock correction value for LINADD (use in LINE7)
#define add4	   1	// V-lock correction value for LINADD
#define amax	   3	// limit for a-couter (lines)
#define apreset	   2	// a vaerdi til brug ved field1 interrupt
#define asamp	   1	// value for a for which to sample in next line
#define bl1	   1	// values for line sorting in "lineint" <931021a7
#define buerrpre   30	// limit for acceptance of burst		<9/10a
#define freq1	   0	// limit for output freq
#define freq2	   0x0fff // do., upper
#define freqref	   0x800  // reference frequence for intern use
#define herrpre    30	// limit for acceptance of h		<2/10c
#define id61	   0x15
#define id62	   0x2a
#define level1	   51	// for check of blacklevel
#define level2	   65	// for check of blacklevel
#define papug	   36	// limit for accept of pal-pulse
#define papupos    372	// relativ palpuls position i forhold til sync
#define pham40	   0x1c	// check figure for burst phase   snj 931112
#define pham45	   0x20	// -	-	-	-       snj 931112
#define pha40	   0xe0	// -	-	-	-       snj 931112
#define pha45	   0xdc	// -	-	-	-       snj 931112
#define samp	   4	// value for setting sampling (use in LINEINT)
#define sch62	   0xb0	// (2's kompl. af sch61)
#define secag1     90	// secam detection
#define secag2     115	// secam detection
#define secag3     20	// secam detection
#define sepos	   154	//
#define supr4	   2	// V-lock suppress flag value for LINADD
#define syg1	   24	// graense for sync amplitude lower
#define verrpre    3	// graense for godkendelse af field (antal gode frames)
#define windlen	   5	// field pulse acceptance window length (lines)
#define vdetect	   3	// v-teller vaerdi inden for window for 625 linier

//  G-VERSION
#define afield_g   1	//
#define bmax_g	   209	// limit for b counter (5-line groups)
#define bpreset_g  208	//  vaerdi til brug ved field1 interrupt
#define buamg1_g   50 	// burst detection
#define buamg2_g   70 	// burst detection
#define bupos_g	   91  	// relativ burst position i forhold til sync
#define fmax_g	   4	// limit for f counter (frames)
#define hcal1_g	   0x320 // low limit for hphzero calibrating (the 2 highest bytes)
#define hcal2_g	   0x324 // high	-	-	-	-	-	-
#define hcen_g	   54	// for line pull - NB// kun 1 byte stor (se rutine 'hfacec')
#define hcyk_g	   108	// for line pull
#define hlen_g	   864	// line length in clockpulses
#define hstart_g   0	// offset from HPOS to seek for LINE
#define palg1_g    185	// pal-n detection
#define palg2_g    205	// pal-n detection
#define phmar_g	   0x000001e7 // graense for at godkende h fase
#define refmult_g  0x4000 // subc.ref frame offset
#define sch61_g	   0xf3 // ? sv. til 0 degr. hysterese - 0??h til 11 degr.)
#define subcinc_g  0xbec5 // fasetilvaekst paa subc. paa x lin.
#define syg2_g	   36	// graense for sync amplitude upper
#define typesize_g 112	// size of tabel for types
#define vlenmin_g  623	// no. of lines to frame check window

//  M-VERSION
#define afield_m   3
#define bmax_m	   175	// limit for b counter (5-line groups)
#define bpreset_m  175	// vaerdi til brug ved field1 interrupt
#define buamg1_m   45	// burst detection
#define buamg2_m   65	// burst detection
#define bupos_m	   88	// relativ burst position i forhold til sync
#define fmax_m	   2	// limit for f counter (frames)
#define hcal1_m	   0x310  // low limit for hphzero calibrating (the 2 highest bytes)<950327
#define hcal2_m	   0x314  // high	-	-	-	-	-	-	<950327
#define hcen_m	   54   // for line pull - NB// kun 1 byte stor (se rutine 'hfacec')
#define hcyk_m	   108  // for line pull
#define hlen_m	   858  // line length in clockpulses
#define hstart_m   0	// offset from HPOS to seek for LINE
#define palg1_m    64	// pal-m detection
#define palg2_m    192	// pal-m detection
#define phmar_m	   0x00000256 // graense for at godkende h fase
#define refmult_m  0x8000 // subc.ref frame offset
#define sch61_m	   0x12E // ? sv. til 0 degr. hysterese - 0??h til 11 degr.)
#define subcinc_m  0x8000 // Ver.M. fasetilvaekst paa subc. paa xlin.
#define syg2_m	   34	// graense for sync amplitude upper
#define typesize_m 104	// size of tabel for types
#define vlenmin_m  523	// no. of lines to frame check window

//  D1/625-VERSION
#define afield_d6  1	//
#define bmax_d6	   209	// limit for b counter (5-line groups)
#define bpreset_d6 208	//  vaerdi til brug ved field1 interrupt
#define buamg1_d6  50 	// burst detection
#define buamg2_d6  70 	// burst detection
#define bupos_d6   91  	// relativ burst position i forhold til sync
#define fmax_d6	   4	// limit for f counter (frames)
#define hcal1_d6   0x320 // low limit for hphzero calibrating (the 2 highest bytes)
#define hcal2_d6   0x324 // high	-	-	-	-	-	-
#define hcen_d6	   54	// for line pull - NB// kun 1 byte stor (se rutine 'hfacec')
#define hcyk_d6	   108	// for line pull
#define hlen_d6	   864	// line length in clockpulses
#define hstart_d6  0	// offset from HPOS to seek for LINE
#define palg1_d6   185	// pal-n detection
#define palg2_d6   205	// pal-n detection
#define refmult_d6 0x4000 // subc.ref frame offset
#define subcinc_d6 0xbec5 // fasetilvaekst paa subc. paa x lin.
#define syg2_d6	   36	// graense for sync amplitude upper
#define typesize_d6 112	// size of tabel for types
#define vlenmin_d6 623	// no. of lines to frame check window

//  D1/525-VERSION
#define afield_d5  3	//
#define bmax_d5	   175	// limit for b counter (5-line groups)
#define bpreset_d5 175	//  vaerdi til brug ved field1 interrupt
#define buamg1_d5  50 	// burst detection
#define buamg2_d5  70 	// burst detection
#define bupos_d5   91  	// relativ burst position i forhold til sync
#define fmax_d5	   2	// limit for f counter (frames)
#define hcal1_d5   0x320 // low limit for hphzero calibrating (the 2 highest bytes)
#define hcal2_d5   0x324 // high	-	-	-	-	-	-
#define hcen_d5	   54	// for line pull - NB// kun 1 byte stor (se rutine 'hfacec')
#define hcyk_d5	   108	// for line pull
#define hlen_d5	   858	// line length in clockpulses
#define hstart_d5  0	// offset from HPOS to seek for LINE
#define palg1_d5   185	// pal-n detection
#define palg2_d5   205	// pal-n detection
#define refmult_d5 0x8000 // subc.ref frame offset
#define subcinc_d5 0x8000 // fasetilvaekst paa subc. paa x lin.
#define syg2_d5	   34	// graense for sync amplitude upper
#define typesize_d5 104	// size of tabel for types
#define vlenmin_d5 523	// no. of lines to frame check window


#define line7req 2
#define schreq	4
#define lockreq	8

// CONSTANTS FOR STATUS OUTPUT
//			tau,secam,slowlock,paln
//			  ÚÙÚÄÄÄ-buav,spgint,lockbit,syncav
#define palglock	0x8b
#define palgunl		0x09
#define pallock		0x93
#define palunl		0x11
#define secamlock	0xc3
#define secamunl	0x41
#define hlocked		0x83
#define hunl		0x01
#define slowerr		0xa5
#define internl		0x04
#define d1lock		0x8b
#define flock		0x8b


/*			tau,secam,slowlock,paln
			ÚÙ  ÚÄÄÄ-buav,spgint,lockbit,syncav
palglock	equ	10001011b
palgunl		equ	00001001b
pallock		equ	10010011b
palunl		equ	00010001b
secamlock	equ	11000011b
secamunl	equ	01000001b
hlocked		equ	10000011b
hunl		equ	00000001b
slowerr		equ	10100101b  ;940121 snj
internl		equ	00000100b
d1lock		equ	10001011b
flock		equ	10001011b
*/
