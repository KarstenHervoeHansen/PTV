/* EQU.C
date:		960425
version:	0.0
author:	P.Chr.
*/

// ** VERSION CONTROL **
		// 1= G-version. 0= M-version.
#define gvers	1

#define NUC near unsigned char
#define NUI near unsigned int
#define UC unsigned char
#define UI unsigned int
#define UL unsigned long
#define sbit static bit

// * CONSTANTS **
#define acal	   2	//(6) a-value for datatreatment
#define add1	   7	//V-lock correction value for LINADD (use in LINE7)
#define add2	   5	//V-lock correction value for LINADD (use in LINE7)
#define add4	   1	//V-lock correction value for LINADD
#define amax	   3	//(9)limit for a-couter (lines)
#define apreset	   2	//(2)a vaerdi til brug ved field1 interrupt
#define asamp	   1	//(8)value for a for which to sample in next line
#define bl1	   1	//values for line sorting in "lineint" <931021a7
#define buerrpre   30	//limit for acceptance of burst		<9/10a
#define freq1	   0	//limit for output freq
#define freq2	   0x0fff // do., upper
#define freqref	   0x800  //reference frequence for intern use
#define herrpre    30	//limit for acceptance of h		<2/10c
#define id61	   0x15
#define id62	   0x2a
#define level1	   51	//for check of blacklevel
#define level2	   65	//for check of blacklevel
#define papug	   36	//limit for accept of pal-pulse
#define papupos    372	//relativ palpuls position i forhold til sync
#define pham40	   0x1c	//check figure for burst phase   snj 931112
#define pham45	   0x20	// -	-	-	-       snj 931112
#define pha40	   0xe0	// -	-	-	-       snj 931112
#define pha45	   0xdc	// -	-	-	-       snj 931112
#define phmar	   0x280//graense for at godkende h fase
//bitaddl	   0	//pos. of bit 'addl' in type byte
//bitsamp	   2	//pos. of bit 'samp' in type byte
//bitsupr	   1	//pos. of bit 'supr' in type byte
#define samp	   4	//value for setting sampling (use in LINEINT)
#define sch61	   0x50	//(040h sv. til 0 degr. hysterese - 048h til 11 degr.)
#define sch62	   0xb0	//(2's kompl. af sch61)          snj 931112
#define secag1     90	//secam detection
#define secag2     115	//secam detection
#define secag3     20	//secam detection
#define sepos	   154	//(164)
#define supr4	   2	//V-lock suppress flag value for LINADD
#define syg1	   24	//snj 931126 graense for sync amplitude lower
//vlenmax    0fd8ah 	//no. of lines to frame time-out (-630)
#define verrpre    3	//graense for godkendelse af field (antal gode frames)
#define windlen	   5	//(foer 10) field pulse acceptance window length (lines)
#define vdetect	   3	//v-teller vaerdi inden for window for 625 linier

#if gvers==1
//	** G-VERSION **
#define afield	   1	//(4)
#define bl2	   105
#define bl3	   207
#define bl4	   102
#define bl5	   208
#define bl6	   104
#define bl7	   103
#define bmax	   209	//(70)limit for b counter (5-line groups)
#define bpreset	   208	//(70)b vaerdi til brug ved field1 interrupt
#define buamg1     100	//burst detection
#define buamg2     140	//burst detection
#define bupos	   120	//relativ burst position i forhold til sync
#define fmax	   4	//limit for f counter (frames)
#define hcal1	   0x420 //low limit for hphzero calibrating (the 2 highest bytes)
#define hcal2	   0x424 //high	-	-	-	-	-	-
#define hcen	   71	//for line pull - NB// kun 1 byte stor (se rutine 'hfacec')
#define hcyk	   142	//for line pull
#define hlen	   1135	//line length in clockpulses
#define hstart	   5	//(3)offset from HPOS to seek for LINE
#define palg1      185	//pal-n detection
#define palg2      205	//pal-n detection
#define refmult	   0x4000 //subc.ref frame offset
#define subcinc    0xbec5 //Ver.G.(3c50)//fasetilvaekst paa subc. paa 9 (5) lin.
#define syg2	   36	//graense for sync amplitude upper
#define typesize   112	//size of tabel for types
#define vlenmin	   623	// no. of lines to frame check window

#else
//	** M-VERSION **
#define afield	   3
#define bl2	   175
#define bl3	   173
#define bl4	   86
#define bl5	   174
#define bl6	   88
#define bl7	   87
#define bmax	   175	//(70)limit for b counter (5-line groups)
#define bpreset	   175	//(70)b vaerdi til brug ved field1 interrupt
#define buamg1     95	//burst detection	<950327
#define buamg2     133	//burst detection	<950327
#define bupos	   96	//relativ burst position i forhold til sync
#define fmax	   2	//limit for f counter (frames)
#define hcal1	   0x340 //low limit for hphzero calibrating (the 2 highest bytes)<950327
#define hcal2	   0x344 //high	-	-	-	-	-	-	<950327
#define hcen	   57	//for line pull - NB// kun 1 byte stor (se rutine 'hfacec')
#define hcyk	   114	//for line pull
#define hlen	   910	//line length in clockpulses
#define hstart	   4	//(3)offset from HPOS to seek for LINE
#define palg1      64	//pal-m detection
#define palg2      192	//pal-m detection
#define refmult	   0x8000 //subc.ref frame offset
#define subcinc    0x8000 //Ver.M.(bec5)//fasetilvaekst paa subc. paa 9 (5) lin.
#define syg2	   34	//graense for sync amplitude upper <950227
#define typesize   104	//size of tabel for types
#define vlenmin	   523	// no. of lines to frame check window
#endif


// CONSTANTS FOR STATUI OUTPUT
//			tau,secam,slowlock,paln
//			ÚÙ  ÚÄÄÄ-buav,spgint,lockbit,syncav
#define palglock		0x8f
#define palgunl			0x0b
#define pallock			0x97
#define palunl			0x15
#define secamlock		0xc7
#define secamunl		0x45
#define hlocked			0x87
#define hunl			0x05
#define slowerr			0xa1
#define internl			0x00

#define ns P3^0
	//flag, incomming sync is bad
#define iicdiff P3^5
	//flag, master has read the port

