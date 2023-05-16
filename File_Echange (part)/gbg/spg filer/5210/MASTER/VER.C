/*************** VERSION SELECTION *****************/


/* SELECT G eller M VERSION (0= G, 1= M) */
#define ntsc	0


/* SELECT VERSION NO. */
#if (ntsc==0)		/* G-version */
#define version	"003.1"

#else			/* M-version */
#define version	"001.1"
#endif


/* SELECT GHOST VERSION (1= ghost) */
#define ghostver	1


/* SELECT BLOWER VERSION (1= new blower circuit) */
#define nybleser	1

/***************************************************/
