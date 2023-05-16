/* External variables used in MAIN.C: */

extern const int HlpIntroduction;		/* HELP.H */

extern const char* COMText[];				/* GUI.C */
extern ERRORINFO errTxt[];					/* HELP.C */

extern int NoWin;								/* GUI.C */
extern int ActiveMenu;						/* GUI.C */
extern int MenuMode;							/* GUI.C */
extern int COMopened[2];					/* GUI.C */
extern int FASTmode;							/* GUI.C */

extern MENU* Menu[MENUCOUNT];				/* GUI.C */
extern WININFO* WinPtr[MAXWINDOW];		/* GUI.C */

extern char* SoftwareType;					/* PHILOGO.C */
extern char* abouttxt[];					/* MESSAGE.C */

extern int LogoTimeoutON;					/* PHILOGO.C */

extern char Baud9600;						/* PM39IO.C */