/* defiogen.c - RTXCio - generation program for RTXCgen */
/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "sysopts.h"

#ifdef RTXCIO

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifdef __TURBOC__
#include <mem.h>
#endif
#ifdef __I8086__
#include <memory.h>
#endif

#include "typedef.h"
#include "rtxstruc.h"

#include "rtxcgen.h"

#include "rtio_inc.h"                  /* RTXCio - Header file include file */

/* CONFIG */

#define NCTLCLS 32      /* max # of Controller classes supported, arbitrary */
#define NDRIVRS 32           /* max # of class drivers supported, arbitrary */
#define NENTRYS 32       /* max # of special functions or errors, arbitrary */

#define DESMAX   20                        /* max # of chars in description */
#define EDESMAX  35                /* Entry - max # of chars in description */
#define ENAMMAX  15                 /* Entry - max # of chars in entry name */
#define NAMMAX    8                         /* max # of chars in Class name */
#define PERIOD    1        /* allow one space for a period in the file name */
#define    EXT    4                    /* max # of chars in Class extension */


#define    SPF 1 /* Specifies in defentry() the entry is a Special Function */
#define    ERR 2      /* Specifies in defentry() the entry is a Error Entry */
#define    DDF 3                     /* Specifies a drivers definition list */
#define    HDR 4                                 /* Specifies a header file */

#define    YES 1                                                     /* yes */
#define     NO 0                                                      /* no */

#define UNDEFFED  0                           /* controller class undefined */
#define DEFFED    1                             /* controller class defined */

extern void blank(char *);
extern void finclude(FILE *,char *);
extern void s_gettime(void);
extern FILE *ixfopen(const char *,const char *);
extern void convert_2_def( char *, char *, int );
extern void RTXCstrupr(char *);

extern char timestamp[];
extern int nctlcls;                 /* number of controller classes defined */
extern int ndrives;                            /* number of drivers defined */
extern int nentrys;                            /* number of entries defined */

typedef struct entryrec
{
   char name[ENAMMAX+1];
    int value;
   char desc[EDESMAX+1];
}ENTRYREC;

extern ENTRYREC far entries[];

typedef struct drvrec
{
      int deflag;
      int deleted;
     char name[NAMMAX+1];
     char spfname[NAMMAX+1];
     char errname[NAMMAX+1];
     char oldspf[NAMMAX+1];
     char olderr[NAMMAX+1];
}DRVREC;

extern DRVREC far drivers[];

typedef struct ctlclrec
{
      int deflag;                                        /* define or undef */
      int deleted;                                        /* deleted or not */
     char name[NAMMAX+1];
     char oldname[NAMMAX+1];
     char spfname[NAMMAX+1];
     char oldspf[NAMMAX+1];
     char drvlist[NAMMAX+PERIOD+EXT+1];
} CTLCLREC;

extern CTLCLREC far ctlcl[NCTLCLS+1];

typedef struct filerec
{
     char inclist[NAMMAX+PERIOD+EXT+1];
} FILEREC;

FILEREC far spffiles[NCTLCLS+NDRIVRS+1];
FILEREC far errfiles[NCTLCLS+NDRIVRS+1];

static char job_id[DESMAX+1];

void zero_jobtime(void);
void zero_hdrlists(void);
void zero_class(void);
void zero_drivers(void);
void zero_entries(void);

static void load_class(void);
static void load_drivers(char *);
static int update_header(char *, int, int);
static void delete_header(char *);

void defiogen(void)
{
     int len, i, j, k, m;
     FILE *outf;
     char string[80];

     zero_jobtime();
     zero_class();

     load_class();
     j=0;m=0;
     for(i=1;i<=nctlcls;i++)
     {
	  zero_drivers();
	  load_drivers(ctlcl[i].drvlist);
	  if( ctlcl[i].deflag )
	  {
	       j = update_header( &ctlcl[i].spfname[0], j, SPF );
	       for(k=1;k<=ndrives;k++)
	       {
		   if( drivers[k].deflag )
		   {
		       j = update_header( &drivers[k].spfname[0], j, SPF );
		       m = update_header( &drivers[k].errname[0], m, ERR );
		   }  /* end if driver defined */
		   else
		   {
		       delete_header( &drivers[k].spfname[0] );
		       delete_header( &drivers[k].errname[0] );
		   }
	       }  /* end driver search */
	  }  /* end if controller class defined */
	  else
	  {
		delete_header( &ctlcl[i].spfname[0] );
		for(k=1;k<=ndrives;k++)
		{
		     delete_header( &drivers[k].spfname[0] );
		     delete_header( &drivers[k].errname[0] );
		}
	  }
     }  /* end controller class search */

     zero_jobtime();
     printf("Job Identification(up to %d char)> ",DESMAX);
     gets(string);
     len = strlen(string);
     if (len != 0)
     {
	 if (len > DESMAX)
	    len = DESMAX;
	 for (i = 0; i < len; i++)
	    job_id[i] = string[i];
     }

     if ((outf = ixfopen("sysiospf.h","w")) == NULL)
     {
	 printf("\tSorry, can't create sysiospf.h\n");
	 return;
     }
     s_gettime();
     fprintf(outf,"/* sysiospf.h - RTXCio Special Function Include File*/\n\n");
     fprintf(outf,"/* Job description : %20s */\n",job_id);
     fprintf(outf,"/* Time file created : %14s */\n\n",timestamp);
     finclude(outf,"copyrite.h");

     for (i = 0; i < j; i++)
	 fprintf(outf,"#include \"%s\"\n",&spffiles[i].inclist[0]);
     fclose(outf);

     if ((outf = ixfopen("sysioerr.h","w")) == NULL)
     {
	 printf("\tSorry, can't create sysioerr.h\n");
	 return;
     }
     s_gettime();
     fprintf(outf,"/* sysioerr.h - RTXCio Error Include File*/\n\n");
     fprintf(outf,"/* Job description : %20s */\n",job_id);
     fprintf(outf,"/* Time file created : %14s */\n\n",timestamp);
     finclude(outf,"copyrite.h");

     for (i = 0; i < m; i++)
	 fprintf(outf,"#include \"%s\"\n",&errfiles[i].inclist[0]);
     fclose(outf);
}

static void load_class(void)
{
	int i, ndeffed;
	FILE *inf;

	if ((inf = ixfopen("ccntlrcl.cdf","r")) == NULL)
		return;

	fscanf(inf,"%20c\n",&job_id[0]);
	fscanf(inf,"%14c\n",&timestamp[0]);
	fscanf(inf,"%d\n",&nctlcls);
	fscanf(inf,"%d\n",&ndeffed);
	for (i = 1; i <= nctlcls; i++)
	{
		fscanf(inf,"%d%8c%8c%s\n", &ctlcl[i].deflag,
					   &ctlcl[i].name[0],
					   &ctlcl[i].spfname[0],
					   &ctlcl[i].drvlist[0] );
		ctlcl[i].deleted = NO;
	}
	fclose(inf); /* fall through into display */
}


static void load_drivers(char *drvlist)
{
	 int i, nddeffed;
	 FILE *inf;

	 if ((inf = ixfopen(drvlist,"r")) == NULL)
	    return;

	 fscanf(inf,"%20c\n",&job_id[0]);
	 fscanf(inf,"%14c\n",&timestamp[0]);
	 fscanf(inf,"%d\n",&ndrives);
	 fscanf(inf,"%d\n",&nddeffed);
	 for (i = 1; i <= ndrives; i++)
	 {
		fscanf(inf,"%d%8c%8c%s\n", &drivers[i].deflag,
					   &drivers[i].name[0],
					   &drivers[i].spfname[0],
					   &drivers[i].errname[0] );
		drivers[i].deleted = NO;
	 }
	 fclose(inf); /* fall through into display */
}

static int update_header(char * entryname, int indx ,int entrytype )
{
     int i, j, tindx;
     char defname[NAMMAX+PERIOD+EXT+1];
     FILE *inf, *outf;

     tindx = indx;
     zero_entries();

     convert_2_def(entryname, defname, entrytype);
     if ((inf = ixfopen(defname,"r")) == NULL)
	 return(tindx);

     fscanf(inf,"%20c\n",&job_id[0]);
     fscanf(inf,"%14c\n",&timestamp[0]);
     fscanf(inf,"%d\n",&nentrys);
     for (i = 1; i <= nentrys; i++)
     {
	   fscanf(inf,"%15c%d%35c\n", &entries[i].name[0],
				      &entries[i].value,
				      &entries[i].desc[0] );
     }
     fclose(inf); /* fall through into display */

     if( nentrys == 0 )
	return(tindx);

     if( entrytype == SPF )
     {
	  convert_2_def(entryname, &spffiles[tindx].inclist[0], HDR );
	  if ((outf = ixfopen(&spffiles[tindx].inclist[0],"w")) == NULL)
		return(tindx);
	  fprintf(outf,"/* Module:  %13s */\n\n",&spffiles[tindx].inclist[0]);
     }
     else
     {
	  convert_2_def(entryname, &errfiles[tindx].inclist[0], HDR );
	  if ((outf = ixfopen(&errfiles[tindx].inclist[0],"w")) == NULL)
		return(tindx);
	  fprintf(outf,"/* Module:  %13s */\n\n",&errfiles[tindx].inclist[0]);
     }

     fprintf(outf,"/* Job description : %20s */\n",job_id);
     fprintf(outf,"/* Time file created : %14s */\n\n",timestamp);

     for(i=1;i<=nentrys;i++)
	  fprintf(outf,"#define  %15s  %3d          /* %35s */\n", &entries[i].name[0],
					    entries[i].value,
					   &entries[i].desc[0] );
     fclose(outf);
     return(tindx+1);
}

void zero_hdrlists(void)
{
   int i, j;

   for (i = 0; i <= NCTLCLS+NDRIVRS+1; i++)
      for (j = 0; j < NAMMAX+PERIOD+EXT+1; j++)
      {
	 spffiles[i].inclist[j] = ' ';
	 errfiles[i].inclist[j] = ' ';
      }
}

void zero_jobtime(void)
{
   int j;

   for (j = 0; j < 14; j++)
      timestamp[j] = ' ';
   for (j = 0; j < DESMAX; j++)
      job_id[j] = ' ';
}

void zero_entries(void)
{
    int i, j;

    for (i = 0; i <= NENTRYS; i++)
    {
	for (j = 0; j < ENAMMAX; j++)
	    entries[i].name[j] = ' ';
	for (j = 0; j < EDESMAX; j++)
	    entries[i].desc[j] = ' ';
	entries[i].value = 0;
    }
    nentrys = 0;
}

void zero_drivers(void)
{
   int i, j;

   for (i = 0; i <= NDRIVRS; i++)
   {
      for (j = 0; j < NAMMAX; j++)
      {
	 drivers[i].name[j] = ' ';
	 drivers[i].spfname[j] = ' ';
	 drivers[i].errname[j] = ' ';
      }
      drivers[i].deflag = YES;
   }
   ndrives = 0;
}

void zero_class(void)
{
   int i, j;

   for (i = 0; i <= NCTLCLS; i++)
   {
      for (j = 0; j < NAMMAX; j++)
      {
	  ctlcl[i].name[j] = ' ';
	  ctlcl[i].spfname[j] = ' ';
      }
      for (j = 0; j < NAMMAX+PERIOD+EXT; j++)
	 ctlcl[i].drvlist[j] = ' ';
      ctlcl[i].deflag = YES;
   }
   nctlcls = 0;
}

static void delete_header(char * entryname)
{
     char defname[NAMMAX+PERIOD+EXT+1];

     convert_2_def(entryname, defname, HDR );
     if ( ixfopen(defname,"r") == NULL)
	 return;
     else
	remove(defname);
     return;
}
#endif
