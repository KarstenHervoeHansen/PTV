#pragma large
/* sub.c	PM5639	*/	/* BEWARE: SPECIAL VERSION */
/* last chg:	911029	*/
/* prev. chg:	911010	*/
/* org.date:	901224	*/
/* name:	P.Chr.	*/

/* 911010 "Use Measurement" on recalled meas. failed low light check	*/
/* 910920 Pregain factors for RGB now read from M.Head			*/
/* 910910 Average calc. removed.					*/
/*	  Dynamic Phosphor no. handling for more phosphors.		*/
/* 910716 Now amend possible in Name menus.				*/
/*	  Name-dependent lock-system for phosphors.			*/
/* 910702 White ref. now also shown in RGB mode (in lowest line)	*/
/*	  Foot-Lamberts now with 2 decimals				*/
/*	  "MEM #" now in upper line in xyY				*/
/* 910607 dev. readout in xyY						*/
/* 910606 in RGB now name under Ref.					*/
/* 910604 "r,g,b"-marks in xyY-display and changed ruler for hi-res	*/
/* 910529 now 7 phosphors						*/
/* 910528 tics on xyY-ruler and on RGB-bars				*/
/* 910521 msm[no]->unitname  chg. to  ->munit_no			*/

#include <reg51.h>		/* define 8051 registers */
#include <stdio.h>		/* define I/O functions	*/
#include <stdlib.h>		/* standard library	*/
#include <absacc.h>		/* absolute addresses	*/
#include <math.h>		/* math (abs,...)	*/
#include <defs.h>		/* own definitions	*/
#include <version.h>		/* version control file	*/

extern void recall_phosphor(char no);	/* com.c */
char floatstring[10];			/* for building string */

/* Basic I/O for display controller */

void putcmd(char c)
{			/* write a command byte to controller */
  while ((controlstatus & 0x40) != 0) ;	/* ensure bit 6 is low = ready */
  controlcmd = c;
}


void putdat(unsigned char c)
{			/* write a data byte to controller */
  while ((controlstatus & 0x40) != 0) ;	/* ensure bit 6 is low = ready */
  controldat = c;
}

/* Basic I/O for display */

unsigned int readcuradr(void)
{			/* read display cursor address */
  data unsigned int adr;

  putcmd(0x47);
  adr = controlread;			/* lo(adr) */
  adr += (int)controlread2 << 8;	/* hi(adr) = 256 * controlread2; */
  return (adr);
}


void setcuradr(unsigned int adr)
{			/* write a new cursor address to display */
  while ((controlstatus & 0x40) != 0) ;	/* wait for controller ready */
  controlcmd = 0x46;
  while ((controlstatus & 0x40) != 0) ;
  controldat =  adr & 0x00ff;		/* set low nipple af address */
  while ((controlstatus & 0x40) != 0) ;
  controldat =  adr >> 8;		/* set high nipple af address */
}


unsigned char getdis(unsigned int adr)
{			/* read a byte from display mem. */
  setcuradr(adr);			/* set cursor adr. */  
  putcmd(0x43);				/* header for read */
  return(controlread);			/* read byte */
}


void putdis(unsigned int adr, unsigned char dat)
{			/* write a byte to display mem. */
  setcuradr(adr);			/* set cursor adr. */
  putcmd(0x42);				/* header for write */
  putdat(dat);				/* write byte */
}


bit putpixel(char x, char y, char plan, char mode)
{			/* set, erase or toggle a dot in display */
  unsigned char n;
  unsigned int adr;

  if ((x>=0) && (x<64) && (y>=0)) 	/* test if inside screen */
  { n = 1 << (y % 8);			/* form bit pattern for dot */
    if (plan==1)  adr = screen2max - y/8 - 16 * (int)x; /* plane 1 */
    else adr = screen1max - y/8 - 16 * (int)x;		/* plane 0 */
    switch (mode)
    { case 0: putdis(adr,(getdis(adr) & (~n))); break;	/* clear dot */
      case 1: putdis(adr,(getdis(adr) | n)); break;	/* set dot */
      case 2: putdis(adr,(getdis(adr) ^ n)); break;	/* XOR dot */
    }
    return(1);		/* OK */
  }
  else return(0);	/* not OK */
}


void clrcross(void)	/* clear measurement "cross" in xyY display */
{
  data char x,y;

  for (x=crossx-1; x<crossx+2; x++)
    for (y=crossy-1; y<crossy+2; y++)
      putpixel(x,y,1,0);		/* erase cross */
}


void putcross(int dx, int dy)
{			/* set measurement "cross" in xyY display */
  data char i,j;

  if ((abs(dx) < 30) && (abs(dy) < 30))
  {
    crossx = targetx + (char)dx; crossy = targety + (char)dy;
    for (i=crossx-1; i<crossx+2; i++)
      for (j=crossy-1; j<crossy+2; j++)
        putpixel(i,j,1,1);		/* set cross */
  }
}


void outchar(char c)	/* display character at current cursor */
{			/* to be used only after outtext() or outtextxy() */
  data char i,n, *p;

  if ((c < ' ') || (c > 'z'))
    c = '.';
  p = asc[c-' '];			/* point to char in alfabet */
  n = *p; p++;
  for(i=1; i<= n; i++) 			/* output char */
    putdat(*p++);
}


void outtext(char *s)	/* put character string in display */
{
  data char c;

  putcmd(0x42);
  while((c= *s++) != '\0')
    outchar(c);
}


void outtextxy(char x, char line, char *s)
{			/* put character string in display at x,y */
  char c;

  setcuradr(screen1max - 16 + line - 16 * (int)x);
  putcmd(0x42);
  while((c= *s++) != '\0')
    outchar(c);
}


void textbar(char line, char mode)
{			/* set, erase or toggle  a bar under a textline */
  data char c,n;
  data unsigned int adr;

  adr = screen2max - 16 + line;
    switch (mode)
    { case 0: setcuradr(adr); putcmd(0x42);		/* bar off */
		for (n=0; n<64; n++) putdat(0); break;
      case 1: setcuradr(adr); putcmd(0x42);		/* bar on */
		for (n=0; n<64; n++) putdat(0xff); break;
      case 2: c=getdis(adr); setcuradr(adr); putcmd(0x42); /* toggle bar */
		for (n=0; n<64; n++) putdat(~c); break;
    }
}


void clrtext(char line)
{			/* clear a text line */
  data char n;
  data unsigned int adr;

  adr = screen1max - 16 + line;
  setcuradr(adr); putcmd(0x42);
  for (n=0; n<64; n++) putdat(0);
}


void strput(char *txt, char *target)
{			/* copy a string to a target */
  char *s = target;
  while ((*(s++) = *(txt++)) != '\0') ;
}


setrowadr(char x, char line)
{			/* set text cursor at x, line */
  unsigned int adr;

  adr = screen1max - 16 + (int)line - 16 * (int)x ;
  setcuradr(adr);
}


vline(char x, char ymin, char ymax, char mode)
{			/* draw, erase or toggle a vertical line */
  data char y;

  for (y=ymin; y<=ymax; y++) putpixel(x,y,0,mode);
}


hline(char xmin, char xmax, char y, char mode)
{			/* draw, erase or toggle  a horizontal line */
  data char x;

  for (x=xmin; x<=xmax; x++) putpixel(x,y,0,mode);
}


drawbar(char barposx, float valmax, float valmin, float val)
{			/* draw a value bar in RGB display */
  int ymax = rgbbar_max;		/* max and min bar pos. */
  int ymin = rgbbar_min;		/* REM: if these are altered, it is */
  int y,oldadr;				/* nessesary to change conversion of c */
  data char rowpos, r, x;		/* in rows 13 and 1 below */
  data unsigned char c;

  y = (int)(((float)(ymax - ymin))/(valmax - valmin) * (val - valmin) +\
      (float)ymin + 0.5);
  if (y > ymax) y = ymax;		/* keep inside bar box */
  if (y < ymin) y = ymin;
  oldadr = readcuradr();
  rowpos = 16 - ((char)y) /8;
  for (r=1; r<14; r++)
  { c = 0;
    setrowadr(barposx,r);
    putcmd(0x42);
    if (r == rowpos)
    { c = ~(0xff << (char)y % 8);
      if (r == 13) c |= 1;		/* draw in lower box line */
    }					/* to be altered with rgbbar_min! */
    else if (r > rowpos) c = 0xff;	/* make black vert. stribe of 8 bit */
					/* (in middle rows) */
    if (r == 1)
	   { c |= 0x10; c &= 0x1f; }	/* draw in upper box line */
					/* to be altered with rgbbar_max! */
    for (x=barposx; x<barposx+5; x++) putdat(c);
  }
  setcuradr(oldadr);			/* restore text cursor */
}


char *ftoa(float f, char d)
{ 		/* convert floating point number to string with d decimals */
  char *fptr;

  fptr = floatstring;				/* point to workstring */
  if (d == 3) sprintf(fptr,"%.3f",f);
  else if (d == 0)  sprintf(fptr,"%4.0f",f);	/* no decimals */
  else if (d == 1) sprintf(fptr,"%5.1f",f);	/* 1 decimal */
  else if (d == 2) sprintf(fptr,"%5.2f",f);	/* 2 decimals */
  else if (d == 4) sprintf(fptr,"%7.4f",f);	/* 4 decimals */

  return(fptr);				/* return pointer to the string */
}


void cls(void)		/* clear screen */
{
  data unsigned int adr;

  putcmd(0x4c);				/* 4c is code for normal moving cursor */
  setcuradr(0);				/* screen 1 start address */
  putcmd(0x42);
  for(adr=0; adr<1024; adr++) putdat(0x00);
  setcuradr(0x0400);			/* screen 2 start address */
  putcmd(0x42);
  for(adr=0; adr<1024; adr++) putdat(0x00);
  putcmd(0x4e);				/* 4e is code for cursor moves north */
}


void findorg(void)	/* find top line phosphor no. for phosphor list display */
{
  phos_org = sup[0]->phos_no - menu[3].maxline / 2 + 2;
  if (phos_org < 1) phos_org = 1;
  if (phos_org > (phosmax - menu[3].maxline + 4))
    phos_org = phosmax- menu[3].maxline + 4;

 /*  if (phos_org > 1) outtextxy(0,3,"Scroll");	 special version 
  if (phos_org < phosmax - menu[3].maxline + 4)
    outtextxy(0,menu[3].maxline+2,"Scroll");
 */
}


void setline(void)	/* draw the line in the menu heads */
{
  char i;

  setcuradr(screen1max - 14);
  putcmd(0x42);
  for (i=0;i<64;i++)
    putdat(8);
}


void setmenu(char n)	/* draw a menu */
{
  char i,y;

  *menuno = n;
  cls();
  menuptr = &menu[*menuno];
  *freeze = 0;				/* stop recall meas. mode */
  *menuon = 1;				/* text mode */
  outtextxy(0, 1, menuptr->txtline[0]);	/* write headline */
  setline();				/* draw line in head */
  for (i=1;i<11;i++)
    outtextxy(0, i+2, menuptr->txtline[i]);	/* write menu lines */
  putcmd(0x5b);				/* set Overlay mode for display, xor */
  putdat(0x0d);
  switch (*menuno)			/* find place for select bar */
  { case 2: y = 1; break;
    case 3: { findorg();
	      y = sup[0]->phos_no - phos_org +2;
	      break;
	    }
    case 7: y = menu[7].maxline; break;
    case 9: y = menu[9].maxline; break;
    case 10: y = menu[10].maxline; break;
    case 15: y = menu[15].maxline; break;
    case 16: { findorg();
	       y = menu[16].maxline;
	       break;
	     }
    default: y = *barptr[*menuno]; break;
  }
  textbar(y +2,1);			/* set the select bar */
  temp->select = y;
  menu_cnt = menu_delay;		/* start menu time-out */
}


void settestmenu()	/* draw testmenu */
{
  char i;

  cls();
  testmenuptr = &menu[0];
  testmenuon = 1;
  outtextxy(0, 1, testmenuptr->txtline[0]);	/* write headline */
  setline();					/* draw line in head */
  for (i=1;i<11;i++)
    outtextxy(0, i+2, testmenuptr->txtline[i]);	/* write text lines */
  putcmd(0x5b);					/* set Overlay mode for display, xor */
  putdat(0x0d);
  textbar(temp->select+2,1);			/* set the select bar */
}


void batt_warn(char x, char y)
{ 			/* draw battery warning sign */
  hline(x,x+12,y,1);
  hline(x,x+12,y-5,1);
  vline(x,y-4,y-1,1);
  vline(x+12,y-4,y-1,1);
  vline(x-1,y-3,y-2,1);
}


void recall_whiteref(char no)
{			/* recall saved white ref. */
  float divisor;

  whref[0]->x = whref[no]->x;
  whref[0]->y = whref[no]->y; 
  sup[0]->whref_no = no;		/* set current wh.ref no. */
					/* calc. U, V (for CIELUV) */
  divisor = 12. * whref[0]->y + 3. - 2 * whref[0]->x;
  whref[0]->u0 = 4 * whref[0]->x / divisor;
  whref[0]->v0 = 9 * whref[0]->y / divisor;
}


void store_whiteref(char no)
{			/* save a new white reference as no. */
  float divisor;

#if devvers == 1			/* future version */
  whref[no]->u0 = whref[6]->u0;		/* save new reference */
  whref[no]->v0 = whref[6]->v0; 
  divisor = 12.0 + 6 * whref[no]->u0 - 16 * whref[no]->v0;
  whref[no]->x = 9 * whref[no]->u0 / divisor;
  whref[no]->y = 4 * whref[no]->v0 / divisor;
#else
  whref[no]->x = whref[6]->x;		/* save new reference */
  whref[no]->y = whref[6]->y; 
#endif
  recall_whiteref(no);			/* make new reference active */
  *barptr[4] = no;			/* save new select bar position */
  strput(whref[no]->name, temp->txt);	/* copy name to temp. store */
  temp->charno = 0;
  whref[no]->key = 0x55;		/* unlock this wh.ref for selection */
}


void set_measunit(char no)
{			/* select measuring value units */
  unitfact = measunit[no].unitfact;
  sup[0]->munit_no = no;
}

 /*
void set_avgtime(char no)		for averaging of measurements
{
  data char i;

  sup[0]->avgtime_no = no;
  *sampleno = 1;			init pointer to sample store
  *samples = avgtime[no].samples;
  sample[0]->x = 0;			make new sum of samples
  sample[0]->y = 0;
  sample[0]->z = 0;
  for (i=1;i<= *samples;i++)
  { sample[0]->x += sample[i]->x;
    sample[0]->y += sample[i]->y;
    sample[0]->z += sample[i]->z;
  }
}
 */


void store_setup(char no)
{			/* store a set-up in no. */
  sup[no]->phos_no = sup[0]->phos_no;	/* store current set-up as no. */
  sup[no]->whref_no = sup[0]->whref_no;
  sup[no]->munit_no = sup[0]->munit_no;
 /*  sup[no]->avgtime_no = sup[0]->avgtime_no;		for averaging */
  sup[no]->mode_no = sup[0]->mode_no;
  sup[no]->rgbmode_no = *barptr[1];
  sup[no]->mem_no = sup[0]->mem_no;
  sup[no]->zoom = sup[0]->zoom;
}


void recall_setup(char no)
{			/* recall a stored set-up */
  *barptr[3] = sup[no]->phos_no;	
  recall_phosphor(*barptr[3]);			/* insert the new phosphor */
  *barptr[4] = sup[no]->whref_no;
  recall_whiteref(*barptr[4]);			/* insert the new wh.ref */
  *barptr[5] = sup[no]->munit_no;		/* set measuring unit */
  sup[0]->munit_no = *barptr[5];
 /*  *barptr[6] = sup[no]->avgtime_no;		set averaging */
 /*  sup[0]->avgtime_no = *barptr[6]; */
  sup[0]->mode_no = sup[no]->mode_no;		/* set measuring mode */
  *barptr[1] = sup[no]->rgbmode_no;
  sup[0]->zoom = sup[no]->zoom;			/* set zoom */
  sup[0]->mem_no = sup[no]->mem_no;		/* set last used mem no. */
}


void store_msm(char no)
{			/* store measurement in memory no. */
  extern void show_ovl(char line);

  if ((msm[0]->xm / xfact) < -0.5)		/* test if overload */
    show_ovl(menu[9].maxline + 3);
  else
  { msm[no]->mode_no = sup[0]->mode_no;		/* save display mode no. */
    msm[no]->xm = msm[0]->xm;			/* save measuring values */
    msm[no]->ym = msm[0]->ym;
    msm[no]->zm = msm[0]->zm;
    msm[no]->xval = msm[0]->xval;
    msm[no]->yval = msm[0]->yval;
    msm[no]->dev = msm[0]->dev;
    msm[no]->dx = msm[0]->dx;			/* save deviations in xyY mode */
    msm[no]->dy = msm[0]->dy;
    msm[no]->rbar = msm[0]->rbar;		/* save rgb bars in rgb mode */
    msm[no]->gbar = msm[0]->gbar;
    msm[no]->bbar = msm[0]->bbar;
    msm[no]->zoom = sup[0]->zoom;		/* save zoom value */
    msm[no]->yy = msm[0]->yy;			/* save luminance */
    msm[no]->munit_no = sup[0]->munit_no;	/* save measur. unit */
    msm[no]->mem_no = sup[0]->mem_no;		/* save last used mem no. */
    msm[no]->whrefname = whref[sup[0]->whref_no]->name; /* save name of wh.ref */
    msm[no]->key = 0x55;
  }
}


void invert_k(void)
{			/* calculate inverse of correction matrix */
  data char i,r,s;
  float t;

  for (r=0;r<3;r++)			/* r is row and s in column */
    for (s=0;s<3;s++)
      ki[r+s*3] = k[r+s*3];
  for (i=0;i<3;i++)
  { ki[i+9] = 1.;
    t = ki[i];
    for (s=1;s<4;s++)
      ki[i+(s-1)*3] = ki[i+s*3] / t;
    for (r=0;r<3;r++)
    { if (i==r) continue;
      ki[r+9] = 0.;
      t = ki[r];
      for (s=1;s<4;s++)
        ki[r+(s-1)*3] = ki[r+s*3] - t * ki[i+(s-1)*3];
    }
  }
}


void rgbref(char n)
{			/* calculate references for RGB bars */
  float xxr,yyr,zzr,xmref,ymref,zmref,xxyyzz;
			/* calculate backwards from an arbitrary luminance value */
  switch (n)		/* (here 90) and the selected x,y value and find the */
  { case 1: ;		/* equivalent Color Sensor values by multiplying with */
    case 2: ;		/* the inverse correction matrix, skale with pregain factors */
    case 3: ;		/* and multiply with phosphor matrix */
    case 4:
	    yyr = 90.0 / unitfact;
  	    xxyyzz = yyr / whref[0]->y;
	    xxr = whref[0]->x * xxyyzz;
	    zzr = xxyyzz - xxr - yyr;
			/* xxr is true X, yyr is Y and zzr is Z */
			/* multiply with inverse K-matrix */
	    xmref = (ki[0] * xxr + ki[3] * yyr + ki[6] * zzr) * xfact;
	    ymref = (ki[1] * xxr + ki[4] * yyr + ki[7] * zzr);
	    zmref = (ki[2] * xxr + ki[5] * yyr + ki[8] * zzr) * zfact;
	    break;
    case 5: ;
    case 6: ;
    case 7: ;
    case 8: ;
    case 9: xmref = msm[n-4]->xm;
	    ymref = msm[n-4]->ym;
	    zmref = msm[n-4]->zm;
	    break;
  }
			/* multiply with Phosphor-matrix */
  rref = int_scale * xmref + nn_gr * ymref + nn_br * zmref;
  gref = nn_rg * xmref + int_scale * ymref + nn_bg * zmref;
  bref = nn_rb * xmref + nn_gb * ymref + int_scale * zmref;
}


void draw_refline(unsigned char zoom)
{			/* draw reference line (for xyY mode */
  unsigned char res;
  if ((sup[0]->mode_no == 1) || (*freeze))	/* if xyY mode */
  { res = 25/zoom;
    hline(5,55,yaxismin-2,0);			/* erase resolution line */
    hline(5,55,yaxismin-3,0);
    putpixel(5,yaxismin-4,0,0);
    putpixel(55,yaxismin-4,0,0);
    hline(30-res ,30+res, yaxismin-3, 1);	/* set resolution line */
    putpixel(30-res,yaxismin-2,0,1);		/* and -dots */
    putpixel(30+res,yaxismin-2,0,1);
    if (res == 25)				/* draw tics in highest resol. */
    {
      putpixel(15,yaxismin-2,0,1);
      putpixel(25,yaxismin-2,0,1);
      putpixel(35,yaxismin-2,0,1);
      putpixel(45,yaxismin-2,0,1);
      putpixel(5,yaxismin-4,0,1);
      putpixel(55,yaxismin-4,0,1);
    }
  }
}


void initxyy(void)
{			/* set up screen for xyY display */
  cls();
  putcmd(0x5b); putdat(0x0c);			/* set layer addition */
  vline(xaxismin,yaxismin,yaxismax,1);		/* draw axes */
  hline(xaxismin+1,xaxismax,yaxismin,1);
  vline(targetx-2,targety-2,targety+2,1);	/* draw target */
  vline(targetx+2,targety-2,targety+2,1);
  hline(targetx-1,targetx+1,targety-2,1);
  hline(targetx-1,targetx+1,targety+2,1);
#if devvers == 1				/* future version */
  outtextxy(xaxismin-1,1,"v");
  outtextxy(xaxismax+3,8,"u");
  outtextxy(4,11,"u=");
  outtextxy(4,12,"v=");
#else
  outtextxy(xaxismin-1,1,"y");			/* write text */
  outtextxy(xaxismax+3,8,"x");
  outtextxy(4,11,"x=");
  outtextxy(4,12,"y=");
#endif
  outtextxy(29,2,"G");
  outtextxy(54,5,"R");
  outtextxy(12,7,"B");
  outtextxy(4,13,"Y=");
  outtextxy(4,14,"dE=");
  outtextxy(38,14,"CIELUV");
  outtextxy(4,15,"Ref: ");

  if (!*freeze)
  { outtext(whref[sup[0]->whref_no]->name);
    sup[0]->mode_no = 1;
    draw_refline(sup[0]->zoom);			/* draw reference line */
  }
  outtextxy(24,9,"0.05");
  crossx = targetx; crossy = targety;		/* initiate "cross" pos. */
  *freeze = 0;					/* turn off recall mode */
  *menuon = 0;					/* menu off */
  menu_cnt =0;					/* immediate menu fall-out */
}


void initrgb(void)
{			/* set up screen for RGB display */
  char barposx,i,j,row;
  char ymax = rgbbar_max;
  char ymin = rgbbar_min;
  char *s = "   ";
  unsigned char c1,c2;

  cls();
  putcmd(0x5b); putdat(0x0c);			/* set layer addition */
  for (row=1;row<15;row++)			/* draw boxes */
  { setcuradr(screen1max - 16 + row);
    putcmd(0x42);
    barposx = 0;
    if (((ymax >> 3) > 16 - row) && ((ymin >> 3) < 16 - row))
    { c1 = 0xff;
      c2 = 0;
      barposx = 8;
    }
    else if ((ymax >> 3) == 16 - row)
    { c1 = ~(0xff << ((ymax & 0x07)+1));	/* top row */
      c2 = 0x01 << (ymax & 0x07);
      barposx = 8;
    }
    else if ((ymin >> 3) == 16 - row)
    { c1 = 0xff << (ymin & 0x07);		/* bottom row */
      c2 = 0x01 << (ymin & 0x07);
      barposx = 8;
    }
    if (barposx)
    { for (i=1;i<barposx;i++) putdat(0);
      for (j=0;j<3;j++)
      { putdat(c1);
        for (i=0;i<5;i++) putdat(c2);
        putdat(c1);
        for (i=0;i<3;i++) putdat(0);
      }
    }
  }
  for (barposx=8; barposx<30; barposx += 10)
  { for (i=1;i<10;i++)				/* set tic marks */
      putpixel(barposx+6, ymin + 10*i, 0, 1);
  }
  outtextxy(38,7,"Ref:");			/* write text */
  switch (*barptr[1])
  { case 1: s = "  G"; break;
    case 2: s = "  R"; break;
    case 3: s = "  B"; break;
    case 4: s = "Lum"; break;
    case 5: s = " #1"; break;
    case 6: s = " #2"; break;
    case 7: s = " #3"; break;
    case 8: s = " #4"; break;
    case 9: s = " #5"; break;
  }
  outtextxy(51,8,s);
  outtextxy(8,14,"R");
  outtextxy(18,14,"G");
  outtextxy(28,14,"B");
  outtextxy(4,15,"Y=");

  *freeze = 0;				/* turn off recall mode */
  sup[0]->mode_no = 2;			/* set mode 2 */
  *menuon = 0;				/* menu off */
  menu_cnt =0;				/* immediate menu fall-out */
  rgbref(*barptr[1]);			/* calculate references */
}


void recall_msm(char no)
{			/* recall stored measurement */
  char res, decimals;
  float barmax, barmin;

  if (msm[no]->key == 0x55)			/* only if not locked */
  { if (msm[no]->mode_no > 1)			/* if in RGB mode */
    { initrgb();
      *freeze = 1;	/* stop new measurements (initrgb() enables!) */
      outtextxy(4,16,"Ref: ");			/* write text */
      outtext(msm[no]->whrefname);
      if (msm[no]->munit_no == 3) decimals = 2; else decimals = 1;
      outtextxy(16,15,ftoa(msm[no]->yy * measunit[msm[no]->munit_no].unitfact,decimals));
      outtext("    "); outtextxy(40,15,measunit[msm[no]->munit_no].name);
#if fotovers == 0				/* future version */
      outtextxy(38,4,"MEM#");
      outchar('0'+no);
#endif
      switch (msm[no]->zoom)			/* determine zoom factor */
      { case 1: barmax = 1.1; barmin = 0.9;
	    outtextxy(38,1,"110%"); outtextxy(38,13,"90%  ");
	    break; 
        case 5: barmax = 1.25; barmin = 0.75; 
	    outtextxy(38,1,"125%"); outtextxy(38,13,"75%  ");
	    break; 
        case 25: barmax = 2.0; barmin = 0.;
	    outtextxy(38,1,"200%"); outtextxy(38,13,"0%    ");
	    break; 
      }
      drawbar(8,barmax,barmin,msm[no]->rbar);	/* draw the bars */
      drawbar(18,barmax,barmin,msm[no]->gbar);
      drawbar(28,barmax,barmin,msm[no]->bbar);
    }

    else					/* if in xyY-mode */
    { *freeze = 1;				/* block output of wh-refer. */
      initxyy();				/* in init of mode */
      *freeze = 1;				/* again, stop new measurements */
      bad_meas = 1;				/* do not use next measurement */
      res = 25/(msm[no]->zoom);
      draw_refline(msm[no]->zoom);		/* draw reference line */

      if (msm[no]->yy >= 0.1 / measunit[1].unitfact) /* if low light */
      { putcross(msm[no]->dx, msm[no]->dy);	/* then output */
        outtextxy(16,11,ftoa(msm[no]->xval,3));
        outtextxy(16,12,ftoa(msm[no]->yval,3));
      }
      else
      { outtextxy(16,11,"         ");		/* else erase */
        outtextxy(16,12,"         ");
      }						/* output luminance */
      if (msm[no]->munit_no == 3) decimals = 2; else decimals = 1;
      outtextxy(16,13,ftoa(msm[no]->yy * measunit[msm[no]->munit_no].unitfact,\
		decimals));
      outtext("    ");				/* erase old figures */
      outtextxy(38,13,measunit[msm[no]->munit_no].name);

      if (msm[no]->yy >= 0.1 / measunit[1].unitfact) /* if not low light */
      { outtextxy(18,14,ftoa(msm[no]->dev,0));	/* write deviation in CIELUV */
        outtext("    ");
      }
      else outtextxy(20,14,"         ");	/* else erase figures */

      outtextxy(38,14,"CIELUV");		/* write text */
      outtextxy(22,15,msm[no]->whrefname);

#if fotovers == 0				/* if not special version */
      outtextxy(38,1,"MEM#"); 			/* then write mem. no. */
      outchar('0'+no);
#endif
    }
    msm[0]->xval = msm[no]->xval;		/* accept as current values */
    msm[0]->yval = msm[no]->yval;
    msm[0]->yy = msm[no]->yy;
  }
}


void initmode(void)	/* initiate measuring mode */
{
  if (sup[0]->mode_no > 1) initrgb(); else initxyy();
}


void scopy(char *s, char *t, char no)
{			/* copy string s to t for no. characters */
  char i;

  for (i=0;i<no;i++) *t++ = *s++;
  *t='\0';
}


void show_status(void)
{			/* write status values in status menu */
  outtextxy(28,3,phos[sup[0]->phos_no]->name);
  outtextxy(28,4,whref[sup[0]->whref_no]->name);
  outtextxy(28,5,ftoa(whref[sup[0]->whref_no]->x,3));
  outtextxy(28,6,ftoa(whref[sup[0]->whref_no]->y,3));
  outtextxy(43,7,measunit[sup[0]->munit_no].name);
 /*  outtextxy(38,8,avgtime[sup[0]->avgtime_no].name); for average! */
 /*  outtextxy(5,10,(sup[0]->mode_no > 1)?menu[1].txtline[*barptr[1]]:"      xy");*/
  if (sup[0]->mode_no > 1) outtextxy(5,9,menu[1].txtline[*barptr[1]]);
  else outtextxy(48,8,"xy");
}


char *ctos(unsigned char no)
/* This routine takes an unsigned char and converts it to a string where */
/* each leading zero is replaced by 2 spaces which in printout fills for */
/* one digit								 */
{
  char s[6];
  char i;

  i = 0;
  if (no >= 100)			/* find houndreds */
  { s[i++] = '1';
    no -= 100;
  }
  else
  { s[i++] = ' ';			/* no houndreds, set 2 spaces */
    s[i++] = ' ';
  }
  if (no >= 10)				/* find tens */
  { s[i++] = '0' + no / 10;
    no %= 10;
  }
  else
  { s[i++] = ' ';			/* no tens, set 2 spaces */
    s[i++] = ' ';
  }
  s[i++] = '0' + no;	/* last digit */
  s[i] = '\0';		/* end string */
  return(s);
}


void out_phos_name(void)
{			/* write phosphor names */
  char i, pno;
  for (i=0;i < menu[3].maxline - 3;i++)
  { pno = phos_org + i;
    outtextxy(0,i+4,ctos(pno));
    outtext(": ");
    outtext(phos[pno]->name);
  }
}


void out_ref_name(void)
{			/* write customer wh.ref names */
  outtextxy(8,3,whref[1]->name);
  outtextxy(8,4,whref[2]->name);
  outtextxy(8,5,whref[3]->name);
  outtextxy(8,6,whref[4]->name);
  outtextxy(8,7,whref[5]->name);
}


void bardown(void)
{			/* move the selection bar 1 line down */
  textbar(temp->select + 2, 0);		/* erase old bar */
  temp->select += 1;
  textbar(temp->select+2, 1);		/* set new bar */
}


void shut_dwn()
{			/* shut down the instrument */
  char i,j;
  contrast_port = 0x80;			/* remove backplane voltage */
  for (i=1;i!=0;i++)			/* pause specified by LCD manufact. */
    for (j=1;j!=0;j++) ;
  contrast_port = 0;			/* shut down */
  while(1) ;				/* stay here until power down */
}
