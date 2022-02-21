/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PM5639 Display Unit														*/
/*	Module:		TABLES.H																		*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	951017																		*/
/*	Rev. date:	970822 KEn	DEV															*/
/*	Status:		Version 2.3																	*/
/*																									*/
/*	Changes:																						*/
/*	970822 Released as version 2.3														*/
/*	951207 Released as version 2.2														*/
/***************************************************************************/

/* VARIABLES - EXTERNS are DEFINED in file TABLES.C */

extern code	char systset[];
extern code	char scroll[];
extern code	char csrform[];

extern code char *asc[];				/* pointers to ascii letter defs. */

extern code struct munit measunit[];	/* meas. unit factors and names */

extern code struct menus menu[];			/* menu definitions */

extern code float TT[MaxTable][5];

extern struct menus* menuptr;
extern struct meas* msm[6];
extern struct phosp* phos[101];
extern struct avg* sample;
extern struct status* sup[13];

extern struct tmp* temp;
extern struct menus* testmenuptr;
extern struct whr* whref[12];
extern struct unit* cfactor[12];

extern char head_version;
extern char last_head_version;

extern char allocbuf[size_of_allocbuf];
extern char* allocp;

extern char datebuf[15];
extern char* software_date;

extern bit bad_meas;

extern char* barptr[42];

extern bit batt_low;
extern bit busy;

extern uchar but;
extern uchar butcmd;

extern bit butenable;
extern char* checkptr;
extern data char combi;
extern bit combi_on;
extern char* contrast;
extern char crossx, crossy;
extern uchar csum;
extern bit err_disp;
extern bit set_setup_no;
extern char errorno;
extern char* freeze;
extern uint idle_cnt;
extern float k[9];
extern float ki[12];
extern char* kuno;
extern char* new_kuno;
extern char* laphr;
extern char* lapmin;
extern char* lapsec;
extern char laptics;

extern uint light_cnt;
extern bit light_exit;
extern bit light_on;

extern uint menu_cnt;

extern bit menu_exit;
extern char* menuno;
extern char* menuon;
extern char* mother;

extern bit newbut;
extern char newhead;
extern int nn_br,nn_gr,nn_bg,nn_rg,nn_gb,nn_rb;
extern int nn_brt,nn_grt,nn_bgt,nn_rgt,nn_gbt,nn_rbt;

extern char non_shdwn;
extern bit noscan;
extern char* number;
extern char* new_number;
extern char oldbut;
extern uchar overload;
extern char phos_org;
extern char phosmax;
extern char prompts_on;
extern int ramsize;
extern char readbin;
extern char rep_cnt;
extern char rep_delay;
extern char rep1_delay;
extern bit repeat_on;
extern char resat;
extern char rsno;
extern char* rgbptr[4];
extern char rgbstring[4][20];
extern float rref, gref, bref;
extern char* sampleno;
extern char* samples;
extern bit sending;
extern bit sendok;
extern char sumon;
extern bit test_on;
extern char testmode;
extern float unitfact;
extern float convfact;
extern int xerr, yerr;
extern float xfact,yfact,zfact;

extern xdata char timecount;
extern xdata char GenericTimecount;

extern bit new_head_version;

extern uchar KLine;

extern uchar MasterResetLock;