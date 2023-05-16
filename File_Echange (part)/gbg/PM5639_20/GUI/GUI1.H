/* Size and location of learn whr ref window: */

#define	WhrLeft		100
#define	WhrTop		( 60 + WINDOWBARDEEP)
#define	WhrRight		540
#define	WhrBottom	290
#define	WhrWidth		( WhrRight - WhrLeft)
#define	WhrRect	212

#define	WhrleftCol	( WhrLeft + ( WhrWidth - 2*WhrRect)/3. + 24)
#define	WhrrightCol	( WhrLeft + WhrRect + 2*( WhrWidth - 2*WhrRect)/3.)


/* Size and location of learn phosphor window: */

#define	PhoLeft		150
#define	PhoTop		( 60 + WINDOWBARDEEP)
#define	PhoRight		490
#define	PhoBottom	290
#define	PhoWidth		( PhoRight - PhoLeft)

#define	PholeftCol	( PhoLeft + ( PhoWidth/4 - 8*16/2))
#define	PhorightCol	( PhoLeft + ( 3 * PhoWidth / 4) - 10*8/2)
		/* 8*16 is length of EDITFIELD ef[0] */
		/* 10*8 is length of EDITFIELD ef[1] */

#define WHRLINESIZE		16		/* Size of one White Reference file line */
										/* and Phosphor file line */

/* External variables used in GUI1.C: */

extern const char* HeadText[];
extern const char* WhrText[];

extern WININFO* WinPtr[MAXWINDOW];
extern int NoWin;

extern int COMopened[2];
extern ERRORINFO errTxt[];					/* MESSAGE.C */

extern const int HlpLearnWhrRef;			/* HELP.H */
extern const int HlpLearnPhosphor;		/* HELP.H */

extern const int HlpSaveWhrRef;			/* HELP.H */
extern const int HlpSavePhosphor;		/* HELP.H */

extern const int HlpRestoreWhrRef;		/* HELP.H */
extern const int HlpRestorePhosphor;	/* HELP.H */

