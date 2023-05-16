#pragma large
/* run.c	PM5639	*/  /* BEWARE: SPECIAL VERSION */
/* changed:	911029	*/
/* prev. chg:	911007	*/
/* org.date:	910212	*/
/* name:	P.Chr.	*/
/* functions:	alloc, memalloc, initall, common_calc, outxyy, outrgb	*/

/* HISTORY */
/* 910909 Average calc. removed.					*/
/*	  Overload now detected.					*/
/* 910807 Luminance output as zero when low				*/
/* 910724 Test mode added						*/
/* 910702 White ref. now also shown in RGB mode (in lowest line)	*/
/*	  Foot-Lamberts now with 2 decimals				*/
/* 910603 now not used phosph and wh.ref's locked			*/
/* 910529 7 phosphors now						*/
/* 910528 RGB bar prolonged and upper bar-text moved up			*/
/* 910521 default lum. Cd/m2, RGB/memory# is now normalized with luminance */

#include <reg51.h>		/* define 8051 registers */
#include <stdio.h>		/* define I/O functions	*/
#include <stdlib.h>		/* standard library	*/
#include <absacc.h>		/* absolute addresses	*/
#include <math.h>		/* math (abs,...)	*/
#include <calls.h>		/* own definitions	*/
#include <defs.h>		/* own definitions	*/
#include <version.h>		/* version control file	*/

/* ALLOCATION OF MEMORY */
static char allocbuf[2000];	/* max reserved space in RAM */
static char *allocp = allocbuf; /* pointer to next free space */

char *alloc(int n)		/* reserve space for n bytes in RAM */
{ allocp += n;
  return(allocp - n);
}


void memalloc(void)		/* reserve space for resident variables */
{ int n;
  data char i;
  n = sizeof(*whref[0]);
  for (i=0;i<refmax+2;i++)
  { whref[i] = (struct whr *)alloc(n); 
  }

  n = sizeof(*sup[0]);
  for (i=0;i<supmax+2;i++)
    sup[i] = (struct status *)alloc(n); 

  n = sizeof(*msm[0]);
  for (i=0;i<measmax+1;i++)
    msm[i] = (struct meas *)alloc(n); 

  temp = (struct tmp *)alloc(sizeof(*temp)); 

  n = sizeof(*sample[0]);
/*  for (i=0;i<11;i++)  (for averaging) */
  for (i=0;i<1;i++)
    sample[i] = (struct avg *)alloc(n); 

  menuptr = (struct menus *)alloc(sizeof(*menuptr)); 
  testmenuptr = (struct menus *)alloc(sizeof(*testmenuptr)); 

  for (i=0;i<28;i++)
    barptr[i] = alloc(1); 

  checkptr = alloc(1);
  contrast = alloc(1);
  freeze = alloc(1);
  menuno = alloc(1);
  menuon = alloc(1);
  mother = alloc(1);
  sampleno = alloc(1);
  samples = alloc(1);
  number = alloc(6);
  kuno = alloc(7);
  new_number = alloc(6);
  new_kuno = alloc(7);

  n = sizeof(*phos[0]);
  for (i=0;i<phosabsmax+2;i++)
  { phos[i] = (struct phosp *)alloc(n); 
  }
}


void initall(void)		/* initiate memory and variables */
{ data char i;
  float divisor;

  memalloc();			/* allocate memory */

  i = P1;		/* check for special start-up commands */
  if (i == 0x80)	/* RAM reset: ENTER + xyY + FUNCT.+ ON/OFF buttons */
    *checkptr = 0;	/* if reset then destroy check figure */
  else if (i == 0x9e) test_on = 1;	/* test mode: FUNCT.+ ON/OFF buttons */
				/* test RAM check figure */
  if (*checkptr != 0x55)	/* if RAM not OK then initiate user memory */
  { for (i=3;i<phosmax+1;i++)
    { phos[i]->name[0] = '\0';	/* erase stored phosphor name */
      phos[i]->key = 0;		/* lock phosphor selections */
    }
    phos[1]->key = 0x55;	/* unlock phosphor 1 and 2 */
    phos[2]->key = 0x55;

    for (i=4;i<refmax+1;i++)
    { whref[i]->name[0] = '\0';	/* erase stored wh.ref names */
      whref[i]->key = 0;	/* lock wh.ref selections */
    }
    whref[1]->x = whx6500;	/* set default wh.refs	*/
    whref[1]->y = why6500;
    strput("D6500",whref[1]->name);
    whref[2]->x = whx3200;
    whref[2]->y = why3200;
    strput("3200K cor.",whref[2]->name);
    whref[3]->x = whx9300;
    whref[3]->y = why9300;
    strput("9300K cor.",whref[3]->name);

    for (i=1;i<4;i++)		/* unlock factory programmed wh.refs */
      whref[i]->key = 0x55;
				/* initiate menu select bars */
    for (i=0;i<28;i++) *barptr[i] = menu[i].maxline;
    *barptr[2] = 1;
    *barptr[7] = menu[7].maxline;
    *barptr[9] = menu[9].maxline;
    *barptr[10] = menu[10].maxline;
    *barptr[15] = menu[15].maxline;
    *barptr[16] = menu[16].maxline;

    sup[0]->phos_no = 1; *barptr[3] = 1; /* initiate set-up */
    sup[0]->whref_no = 1; *barptr[4] = 1;
    sup[0]->munit_no = 1; *barptr[5] = 1;
    sup[0]->rgbmode_no = 1; *barptr[1] = 1;
    sup[0]->mem_no = 1; *barptr[9] = 1;
    sup[0]->zoom = 25;
    sup[0]->mode_no = 1;	/* set standard mode: xyY */
    *menuon = 0;
    *freeze = 0;
    *menuno = 2;

    *contrast = 50;		/* standard contrast */
    temp->charno = 0;		/* char no. pointer for spelling */

/*    sup[0]->avgtime_no = 2; *barptr[6] = 2;   initiate averaging
    if (*sampleno >10) *sampleno = 1;
    if (*samples >10) *samples = 1;
    *samples = avgtime[2].samples;
*/
  }

/* the next is done in every start-up */
  strput(phosname1,phos[1]->name);	/* default phosphor names */
  strput(phosname2,phos[2]->name);
  unitfact = measunit[sup[0]->munit_no].unitfact; /* luminance factor */
  butenable = 0;			/* inhibit ON button */
  contrast_port = *contrast | 0x80;	/* bootstrap power-on */
  idle_cnt = idle_delay;		/* arm auto shut-off */
  if (*menuon) menu_cnt = menu_delay;	/* arm menu fall-out */
  rep_cnt = 0;				/* initiate button repetition */
  rep_delay = 0;

/*  *sampleno = 0;	init pointer to sample store (averaging) */
/*  for (i=0;i<11;i++)*/	
  for (i=0;i<1;i++)
  { sample[i]->x = 0.0;
    sample[i]->y = 0.0;
    sample[i]->z = 0.0;
  }
	/* initiate measur. values for averaging */
/*  for (i=1; i < *samples+1; i++)
    { sample[0]->x += sample[i]->x;
      sample[0]->y += sample[i]->y;
      sample[0]->z += sample[i]->z;
    }
*/

  if (*checkptr != 0x55)	/* if RAM not OK then initiate user memory */
  { *checkptr = 0x55;		/* set RAM OK flag */
    recall_whiteref(1);		/* select wh.ref no. 1 */
    for (i=1;i<supmax+2;i++)	/* set default recalls */
      store_setup(i);
    for (i=1;i<measmax+1;i++)	/* erase stored measurements */
      msm[i]->key = 0;
  }
}


static char decimals;
static float lum, lumlow, yy, xm, ym, zm, xval, yval;

void common_calc(void)	/* calculations common to xyY- and RGB-mode */
{ float x,y,z,sum,xx,zz,u,v,u0,v0,divisor;

  x = sample[0]->x;		/* save C.Sensor values */
  y = sample[0]->y;
  z = sample[0]->z;
  if (x < -0.5) overload = 1;

  /* multiply with correction matrix to make standard observer values */
  xx = x * k[0] + y * k[3] + z * k[6];	/* = true X */
  yy = x * k[1] + y * k[4] + z * k[7];	/* = true Y */
  zz = x * k[2] + y * k[5] + z * k[8];	/* = true Z */

  if (xx < 0.) xx = 0.;		/* correct negative values */

  sum = xx + yy + zz;		/* calculate x,y */
  xval = xx/sum;
  yval = yy/sum;

	/* calculate pre-gain corrected values for RGB display */
  xm = x * xfact; /* / (*samples);*/	/* averaging disabled */
  ym = y;  /*  / (*samples); */		/* yfact is always 1.0 */
  zm = z * zfact; /* / (*samples);*/

	/* test for low light or overload */
  if ((yy < 0.1 / measunit[1].unitfact) || overload)
    lumlow = 0;
  else lumlow = 0;
  lum = (lumlow) ? 0.0 : yy * unitfact;	/* calc. luminance */

	/* determine number format for luminance */
  if (sup[0]->munit_no == 3) decimals = 2; else decimals = 1;

  msm[0]->xm = xm;	/* store values of current measurement */
  msm[0]->ym = ym;
  msm[0]->zm = zm;
  msm[0]->xval = xval;
  msm[0]->yval = yval;
  msm[0]->yy = yy;
  divisor = 12. * yval + 3. - 2 * xval; /* calc. deviation in CIELUV */
  u = 4 * xval / divisor;
  v = 9 * yval / divisor;
  u0 = whref[0]->u0;
  v0 = whref[0]->v0;
  msm[0]->dev = 1300. * sqrt((u - u0)*(u - u0) + (v - v0)*(v - v0));

#if devvers == 1		/* future version */
  xerr = (int)((u - u0) * 1000);
  yerr = (int)((v - v0) * 1000);
  msm[0]->uval = u;
  msm[0]->vval = v;
#else
  xerr = (int)((xval - whref[0]->x) * 1000);
  yerr = (int)((yval - whref[0]->y) * 1000);
#endif
}


void outxyy(void)		/* output measurement in xyY format */
{ data char i,dx,dy,res;

  common_calc();		/* calculations common to xyY- and RGB-mode */
  clrcross();			/* erase old measure dot */
  dx = xerr/(sup[0]->zoom);	/* calc. deviations from x,y target */
  dy = yerr/(sup[0]->zoom);
  msm[0]->dx = dx;		/* save them */
  msm[0]->dy = dy;

  if (!lumlow)			/* write to screen */
  { putcross(dx,dy);
    outtextxy(16,11,ftoa(xval,3));
    outtext("  ");
    outtextxy(16,12,ftoa(yval,3));
    if (msm[0]->dev < 999.)
      outtextxy(18,14,ftoa(msm[0]->dev,0));
    outtext("   ");		/* erase old figures */
/*    outtextxy(38,14,"CIELUV");*/
  }
  else
  { outtextxy(16,11,"         ");
    outtextxy(16,12,"         ");
    outtextxy(18,14,"         ");
  }
  outtextxy(16,13,ftoa(lum,4));
  outtext("    ");		/* erase old figures */
/*  outtextxy(38,13,measunit[sup[0]->munit_no].name);*/
}


void outrgb(void)		/* output measurement in RGB format */
{ data char i, *s, *t;
  float basis,rbar,gbar,bbar,rval,gval,bval,barmax,barmin,barfact;

  common_calc();		/* calculations common to xyY- and RGB-mode */

  outtextxy(16,15,ftoa(lum,4));		/* output luminance */
  outtext("    ");
 /* outtextxy(40,15,measunit[sup[0]->munit_no].name);*/ /* and measur. unit */
  if (*barptr[1] < 5)				/* and wh.ref name */
  { outtextxy(4,16,"Ref: ");
    outtext(whref[sup[0]->whref_no]->name);
  }
  /* multiply with phosphor matrix to make r, g, b sensors independant */
  rval = int_scale * xm + nn_gr * ym + nn_br * zm;
  gval = nn_rg * xm + int_scale * ym + nn_bg * zm;
  bval = nn_rb * xm + nn_gb * ym + int_scale * zm;

  if (!lumlow)		/* calculate bar size relative to references */
  { switch (*barptr[1])	/* there are 9 RGB modes */
    { case 1: basis = gref/gval;
	    gbar = 1.0;
	    rbar = rval/rref * basis;
	    bbar = bval/bref * basis;
	    break;
      case 2: basis = rref/rval;
	    rbar = 1.0;
	    gbar = gval/gref * basis;
	    bbar = bval/bref * basis;
	    break;
      case 3: basis = bref/bval;
	    bbar = 1.0;
	    rbar = rval/rref * basis;
	    gbar = gval/gref * basis;
	    break;
      case 4: 
    	    barfact = 90. / unitfact / yy;
	    rbar = rval/rref * barfact;
	    gbar = gval/gref * barfact;
	    bbar = bval/bref * barfact;
	    break;
      case 5: ;
      case 6: ;
      case 7: ;
      case 8: ;
      case 9: barfact = msm[*barptr[1]-4]->yy / yy;
	    rbar = rval/rref * barfact;
	    gbar = gval/gref * barfact;
	    bbar = bval/bref * barfact;
	    break;
    }
  }
  else rbar = gbar = bbar = 0;

  msm[0]->rbar = rbar;		/* save bar values */
  msm[0]->gbar = gbar;
  msm[0]->bbar = bbar;

  switch (sup[0]->zoom)		/* determine max. and min. bar value */
  { case 1: barmax = 1.1; barmin = 0.9; s = "110%"; t = "90%  ";
	    break; 
    case 5: barmax = 1.25; barmin = 0.75; s = "125%"; t = "75%  "; 
	    break; 
    case 25: barmax = 2.0; barmin = 0.; s = "200%"; t = "0%    ";
	    break; 
  }
  outtextxy(38,1,s); outtextxy(38,13,t);	/* write them to display */

  drawbar(8,barmax,barmin,rbar);	/* display RGB bars */
  drawbar(18,barmax,barmin,gbar);
  drawbar(28,barmax,barmin,bbar);

}
