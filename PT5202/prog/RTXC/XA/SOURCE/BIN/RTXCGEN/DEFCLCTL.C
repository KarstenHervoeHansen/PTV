/* defctlcl.c - RTXCio - Controller class definition program for RTXCgen */
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

#define DESMAX 20                          /* max # of chars in description */
#define NAMMAX  8                           /* max # of chars in Class name */
#define PERIOD  1          /* allow one space for a period in the file name */
#define    EXT  4                      /* max # of chars in Class extension */

#define    SPF 1 /* Specifies in defentry() the entry is a Special Function */
#define    ERR 2      /* Specifies in defentry() the entry is a Error Entry */
#define    DDF 3                     /* Specifies a drivers definition list */
#define    HDR 4                                 /* Specifies a header file */

#define    YES 1                                                     /* yes */
#define     NO 0                                                      /* no */

#define UNDEFFED  0                           /* controller class undefined */
#define DEFFED    1                             /* controller class defined */

extern void blank(char *);
extern  int getnum(char *);
extern  int nodata(char *);
extern void s_gettime(void);
extern FILE *ixfopen(const char *,const char *);
extern void clrscr(int);                /* screen clear given so many lines */
extern void save_header(char *, char *, int );
extern void defentry(char *, int, char *, char *);
extern void defdrv(char *, char *);
extern void convert_2_def( char *, char *, int );
extern void zero_jobtime(void);
extern void zero_class(void);
extern void RTXCstrupr(char *);

extern char timestamp[];

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

CTLCLREC far ctlcl[NCTLCLS+1];

CTLCLREC * selected_cl[NCTLCLS+1];

int nctlcls;                  /* number of controller classes created */
int ndeffed;                  /* number of controller classes defined */

void convert_2_print(char *, char *);
void cledit_menu(char *, int, char *);   /* editor menu */
void rtxcio_menu(void);
void defclass(void);

static  int dirty;
static char job_id[DESMAX+1];

static void define(int, int, char *, char *);
static void load_menu(char *,int );
static void class_menu(int);
static void load_class(void);
static void keep_class(void);
static void store_old( int );
static void delete_drivers(char *);
static  int setup_selection(void);


void defrtxcio(void)
{
   int i, j, k, l, clindex, len;
   char string[80];
   char absname[NAMMAX+1];

   rtxcio_menu();
rtxcioloop:
   printf("\nRTXCio Definition - Command > ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
	case 'C':
	class_menu(16);
classloop:
	    printf("\nRTXCio Class Definition - Command > ");
	    gets(string);
	    strupr(string);
	    switch(string[0])
	    {
		case 'C':
		    defclass();
		    break;

		case 'S':
		    load_class();
		    if( !nctlcls )
		    {
			printf("\t\nYou MUST FIRST define a Controller Class!\n");
			break;
		    }

		    printf(  "\n\t+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		    printf(  "\t    Below is a list of the Controller classes you    \n");
		    printf(  "\t    have selected for your application. From this    \n");
		    printf(  "\t    list, choose the Controller class associated     \n");
		    printf(  "\t    with the SPECIAL FUNCTION header file to be      \n");
		    printf(  "\t    created or modified. To EXIT enter a zero.       \n");
		    printf(  "\t+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

		    i = setup_selection();
		    for(j=1, k=6,l=1; j<=i; j++, k++)
		    {
			if (k > SCREENSIZE - 5)
			{
			     k = 1;
			     l++;
			     printf("-- More --");
			     gets(string);
			     strupr(string);
			     if (string[0] == 'X')
				break;
			}
			printf("\t%3d - %s\n", j, selected_cl[j]->name);
		    }
		    if(l==1)
			clrscr(19-k+1);
		    else
			clrscr(21-k+1);
entrydef:
		    j = getnum("\nSelect the Controller Class > ");
		    if (j == 0)
		    {
			class_menu(16);
			break;
		    }

		    if (j < 0 || j > i )
		    {
			printf("\tINVALID Controller Class selected!\n");
			goto entrydef;
		    }
		    convert_2_print( selected_cl[j]->name, &absname[0] );
		    defentry(selected_cl[j]->spfname, SPF, absname, "class definition" );
		    class_menu(15);
		    break;

		case 'H':
		    class_menu(16);
		    break;

		case 'X':
		    rtxcio_menu();
		    goto rtxcioloop;

		default:
		    printf("\t\aInvalid command\n");
		    class_menu(16);
		    break;
	    }
	    goto classloop;

	case 'D':
	    load_class();
	    if( !nctlcls )
	    {
		printf("\t\nYou MUST FIRST define a Controller Class!\n");
		break;
	    }
	    printf(  "\n\t+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	    printf(  "\t    Below is a list of the Controller classes you    \n");
	    printf(  "\t    have selected for your application. From this    \n");
	    printf(  "\t    list, choose the Controller class associated     \n");
	    printf(  "\t    with the DRIVER to be created or modified. To    \n");
	    printf(  "\t    EXIT enter a zero.                               \n");
	    printf(  "\t+++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	    i = setup_selection();
	    for(j=1, k=6, l=1; j<=i; j++, k++)
	    {
		if (k > SCREENSIZE - 5)
		{
		     k = 1;
		     l++;
		     printf("-- More --");
		     gets(string);
		     strupr(string);
		     if (string[0] == 'X')
			break;
		}
		printf("\t%3d - %s\n", j, selected_cl[j]->name);
	    }
	    if(l==1)
		clrscr(19-k+1);
	    else
		clrscr(21-k+1);

driverdef:
	    j = getnum("\nSelect the Controller Class > ");
	    if (j == 0)
	    {
		  rtxcio_menu();
		  break;
	    }

	    if (j < 0 || j > i )
	    {
		  printf("\tINVALID Controller Class selected!\n");
		  goto driverdef;
	    }

	    convert_2_print( selected_cl[j]->name, &absname[0] );
	    defdrv( absname, selected_cl[j]->drvlist);
	    rtxcio_menu();
	    break;

	case 'H':
	    rtxcio_menu();
	    break;

	case 'X':
	    return;

	default:
	    printf("\t\aInvalid command\n");
	    break;
	}
	goto rtxcioloop;
}


void defclass(void)
{
   int i, j, k, q, len, max;
   long n;
   char string[80];
   char changebuff1[80];
   char changebuff2[80];
   char ibuff[80];
   char *hdrstring;
   FILE *inf, *outf, *outh;

   dirty = 0;
   nctlcls = 0;
   ndeffed = 0;

   zero_jobtime();
   zero_class();

   load_menu("*** RTXCio - Controller Class ***",12);
loop:
   printf("\nRTXCio Controller Class Definition - Command > ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'E': /* edit resource */
      cledit_menu("*** RTXCio - Controller Class edit menu ***", 13, "");
editor:
	 printf("\nRTXCio Controller Class Editor - Command > ");
         gets(string);
         strupr(string);
         switch(string[0])
         {
            case 'A': /* append mode */
	       if (nctlcls >= NCTLCLS)
               {
		  printf("\tMaximum number of Controller classes exceeded\n");
                  goto editor;
               }
               dirty++;
	       nctlcls++;
	       blank(&ctlcl[nctlcls].name[0]);
	       blank(&ctlcl[nctlcls].spfname[0]);
	       blank(&ctlcl[nctlcls].drvlist[0]);
	       define(nctlcls, 20, "\nAppend mode:\n","");
	       store_old(nctlcls);
	       if( ctlcl[nctlcls].deflag == DEFFED )
		     ndeffed++;
	       ctlcl[nctlcls].deleted = NO;
	       break;

            case 'D': /* delete mode */
	       if( !nctlcls )
	       {
		   printf("\nNo Controller Classes exist\n");
		   cledit_menu("*** RTXCio - Controller Class edit menu ***", 12, "");
		   break;
	       }
delete:
	       printf("\n\tController_Classes\n");
		     /*         12345678     */
	       for(i=1, k=0;i<=nctlcls;i++, k++)
	       {
		   if( ctlcl[i].deleted == NO )
		   {
		       if (k > SCREENSIZE - 5)
		       {
				k = 1;
				printf("-- More --");
				gets(string);
				strupr(string);
				if (string[0] == 'X')
					break;
		       }
		       printf("\t%3d - %8s\n", i, &ctlcl[i].name[0]);
		   }
	       }
	       clrscr(21-k+1);
	       i = getnum("\nSelect the Controller Class to be DELETED > ");
	       if (i == 0)
               {
		   printf("\tDelete aborted\n");
		   cledit_menu("*** RTXCio - Controller Class edit menu ***", 12, "");
		   break;
               }
	       if (i < 0 || i > nctlcls)
               {
		   printf("\tINVALID Controller class selected!\n");
		   goto delete;
               }

delete_resp:
	       printf("\n\n\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	       printf("\t+++                      WARNING!                    +++\n");
	       printf("\t+++ When deleting a Controller Class, you lose the   +++\n");
	       printf("\t+++ special function header definition file for that +++\n");
	       printf("\t+++ Class.  You also lose all the associated drivers +++\n");
	       printf("\t+++ and thier associated header definition files. An +++\n");
	       printf("\t+++ alternative is to 'undef' the Controller Class   +++\n");
	       printf("\t+++ which will retain the Class information, but     +++\n");
	       printf("\t+++ remove the Class from your application.          +++\n");
	       printf("\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
	       printf("Do you wish to continue with this deletion? ('Y' or 'N') > ",DESMAX);
	       gets(string);
	       strupr(string);
	       if( string[0] == 'Y' )
	       {
			dirty++;
			if( ctlcl[i].deflag == DEFFED )
				ndeffed--;
			ctlcl[i].deleted = YES;
			cledit_menu("*** RTXCio - Controller Class edit menu ***", 13, "");
			break;
	       }
	       else
	       {
			if( string[0] == 'N' )
			{
				cledit_menu("*** RTXCio - Controller Class edit menu ***", 13, "");
				break;
			}
			else
				goto delete_resp;
	       }


	    case 'C': /* change mode */
	       if( !nctlcls )
	       {
		   printf("\nNo Controller Classes exist\n");
		   cledit_menu("*** RTXCio - Controller Class edit menu ***", 12, "");
		   break;
	       }
change:
	       printf("\n\tController_Classes\n");
		     /*         12345678     */
	       for(i=1, k=0;i<=nctlcls;i++, k++)
	       {
		   if( ctlcl[i].deleted == NO )
		   {
		       if (k > SCREENSIZE - 5)
		       {
				k = 1;
				printf("-- More --");
				gets(string);
				strupr(string);
				if (string[0] == 'X')
					break;
		       }
		       printf("\t%3d - %8s\n", i, &ctlcl[i].name[0]);
		   }
	       }
	       clrscr(21-k+1);
	       i = getnum("\nSelect the Controller Class to be CHANGED > ");
	       k = ctlcl[i].deflag;      /* save old deflag value */
	       if (i == 0)
               {
                  printf("\tChange aborted\n");
		  cledit_menu("*** RTXCio - Controller Class edit menu ***", 12, "");
		  break;
               }

	       if (i < 0 || i > nctlcls)
               {
		  printf("\tINVALID Controller class selected!\n");
                  goto change;
               }

	       for (j = 0; j < 80; j++)
	       {
			changebuff1[j] = '\0';
			changebuff2[j] = '\0';
	       }

	       sprintf(changebuff1,"\n\tController_Class   Selected_Status   Class_Special_Function_Header \n");
		       /*      12345678            12345678               12345678     */

	       if( ctlcl[i].deflag == DEFFED )
		      sprintf(changebuff2,"\t    %8s            %8s               %8s\n\n",
							    &ctlcl[i].name[0],
							    "YES     ",
							    &ctlcl[i].spfname[0] );
	       else
		      sprintf(changebuff2,"\t    %8s            %8s               %8s\n\n",
							    &ctlcl[i].name[0],
							    "NO      ",
							    &ctlcl[i].spfname[0] );

	       define(i,17,changebuff1,changebuff2);
	       if( ( k == DEFFED) && (ctlcl[i].deflag == UNDEFFED) )
		     ndeffed--;
	       if( ( k == UNDEFFED) && (ctlcl[i].deflag == DEFFED) )
		     ndeffed++;
	       cledit_menu("*** RTXCio - Controller Class edit menu ***", 13, "");
	       break;

            case 'V': /* view mode */
	       if( !nctlcls)
	       {
		   printf("\nNo Controller Classes exist\n");
		   break;
	       }
	       for (i = 1, j = 2; i <= nctlcls;i++)
               {
		  if (j > SCREENSIZE - 5)
                  {
                     j = 1;
                     printf("-- More --");
                     gets(string);
                     strupr(string);
                     if (string[0] == 'X')
                        break;
		  }
		  if( i == 1 )
		  {
			printf("\n\t%20s %14s\n",job_id,timestamp);
			printf("\n\tController_Class   Selection_Status   Class_Special_Function_Header \n");
				/*      12345678             12345678               12345678     */
		  }
		  if( ctlcl[i].deleted == NO )
		  {
			printf("\t    %8s", &ctlcl[i].name[0]);
			if( ctlcl[i].deflag == DEFFED )
				printf("            %8s", "YES     ");
			else
				printf("            %8s", "NO      ");
			printf("               %8s\n", &ctlcl[i].spfname[0] );
			j++;
		  }
	       }
	       printf("\n");
	       j++;
	       clrscr(19-j+3);
	       break;

            case 'X': /* exit editor */
	       load_menu("*** RTXCio - Controller Class ***",12);
	       goto loop;

            case 'H': /* help */
	       cledit_menu("*** RTXCio - Controller Class edit menu ***",13, "");
	       break;

            default:
               printf("\t\aInvalid command\n");
               break;
         }
         goto editor;

      case 'L': /* load existing file */
	 load_class();
      case 'V': /* view current defs */
	 if( !nctlcls)
	 {
	      printf("\nNo Controller Classes exist\n");
	      break;
	 }
	 for ( i = 1, j = 2; i <= nctlcls; i++)
         {
	    if (j > SCREENSIZE - 5)
            {
               j = 1;
               printf("-- More --");
               gets(string);
               strupr(string);
               if (string[0] == 'X')
                  break;
            }
	    if( i == 1 )
	    {
		printf("\n\t%20s %14s\n",job_id,timestamp);
		printf("\n\tController_Class   Selection_Status   Class_Special_Function_Header \n");
		       /*      12345678              12345678               12345678     */
	    }
	    if( ctlcl[i].deleted == NO )
	    {
		printf("\t    %8s", &ctlcl[i].name[0]);
		if( ctlcl[i].deflag == DEFFED )
			printf("            %8s", "YES     ");
		else
			printf("            %8s", "NO      ");
		printf("               %8s\n", &ctlcl[i].spfname[0] );
		j++;
	    }
	 }
	 printf("\n");
	 j++;
	 clrscr(19-j+3);
	 break;

   case 'K': /* keep description file */
      keep_class();
      load_menu("*** RTXCio - Controller Class ***",11);
      break;

   case 'P': /* print current device definitions */
      fprintf(stdprn," RTXCio - Controller class definitions - %20s %14s\n\n",
         job_id,timestamp);
      fprintf(stdprn,"%20s\n\n", job_id);

      if( !nctlcls)
      {
	   fprintf(stdprn,"\nNo Controller Classes exist\n");
	   fprintf(stdprn,"\f");
	   break;
      }

      fprintf(stdprn,"\nController_Class   Selection_Status   Class_Special_Function_Header \n");
		    /*      12345678             12345678               12345678     */

      for ( i = 1; i <= nctlcls; i++)
      {

	    if( ctlcl[i].deleted == NO )
	    {
		fprintf(stdprn,"    %8s", &ctlcl[i].name[0]);
		if( ctlcl[i].deflag == DEFFED )
		      fprintf(stdprn,"            %8s", "YES     ");
		else
		      fprintf(stdprn,"            %8s", "NO      ");
		fprintf(stdprn,"               %8s\n", &ctlcl[i].spfname[0] );
	    }
      }
      fprintf(stdprn,"\n");
      fprintf(stdprn,"\f");
      break;

   case 'H':
      load_menu("*** RTXCio - Controller Class ***",12);
      break;

   case 'X':
      if(dirty)
      {
resp:
	clrscr(21);
	printf("\nWould you like to SAVE your Controller Class changes? ('Y' or 'N') > ",DESMAX);
	gets(string);
	strupr(string);
	if( string[0] == 'Y' )
	{
		keep_class();
		class_menu(15);
		return;
	}
	else
	{
		if( string[0] == 'N' )
		{
			class_menu(16);
			return;
		}
		else
			goto resp;
	}
      }
      class_menu(16);
      return;

   default:
      printf("\t\aInvalid command\n");
      break;
   }
   goto loop;
}

static void class_menu(int lines)
{
   clrscr(lines);
   printf("\t*** RTXCio - Class Definitions ***\n\n");
   printf("\tC. Controller classes\n");
   printf("\tS. Special functions\n");
   printf("\tH. Help\n");
   printf("\tX. eXit module\n");
}

void rtxcio_menu(void)
{
   clrscr(16);
   printf("\t*** RTXCio - Definitions ***\n\n");
   printf("\tC. Classes\n");
   printf("\tD. Drivers\n");
   printf("\tH. Help\n");
   printf("\tX. eXit module\n");
}

void load_menu(char *s, int lines)
{
   clrscr(lines);
   printf("\n\t%s\n\n",s);
   printf("\tL. Load existing Controller Class definition file\n");
   printf("\tV. View current Controller Class definitions\n");
   printf("\tE. Edit individual Controller Class parameters\n");
   printf("\tK. Keep new Controller Class definition file\n");
   printf("\tP. Print current Controller Class definitions\n");
   printf("\tH. Help\n");
   printf("\tX. eXit module\n");
}


static void define(int q, int lines, char *change1, char *change2)
{
   int i, len, tmp;
   char string[80];

   clrscr(lines);

   if(*change2 != '\0')
   {
	printf(change1);
	printf(change2);
	printf("Change mode:\n");
   }
   else
	printf(change1);
name:
   printf("\nController Class Name (up to %d char)> ",NAMMAX);
   gets(string);
   strupr(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto name;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(ctlcl[q].name);
      if (len > NAMMAX)
         len = NAMMAX;
      for (i = 1; i <= len; i++)
         if (string[i] == ' ')
         {
            printf("\tSpaces not permitted in name\n");
            goto name;
         }
      for (i = 0; i < len; i++)
	 ctlcl[q].name[i] = string[i];
      if (ctlcl[q].name[0] == ' ')
         for (i = 0; i < NAMMAX; i++)
	     ctlcl[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (ctlcl[q].name[0] == ' ')
      {
           printf("\tName required\n");
           goto name;
      }
   }
spfname:
   printf("Controller Class special functions header Name (up to %d char)> ",NAMMAX);
   gets(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto spfname;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(ctlcl[q].spfname);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto spfname;
	 }
      for (i = 0; i < len; i++)
	 ctlcl[q].spfname[i] = string[i];
      if (ctlcl[q].spfname[0] == ' ')
	 for (i = 0; i < NAMMAX; i++)
	     ctlcl[q].spfname[i] = ' ';
      dirty++;
   }
   else
   {
      if (ctlcl[q].spfname[0] == ' ')
      {
	   printf("\tName required\n");
	   goto spfname;
      }
   }

defvalue:
   printf("Select this Controller Class? (default=yes, Y or N) > ");
   gets(string);
   strupr(string);
   len = strlen(string);
   if( len != 0 )
   {
	if(string[0] == 'Y')
		ctlcl[q].deflag = DEFFED;
	else
	{
		if( string[0] == 'N' )
			ctlcl[q].deflag = UNDEFFED;
		else
			goto defvalue;
	}
	dirty++;
   }

   convert_2_def( &ctlcl[q].name[0], &ctlcl[q].drvlist[0], DDF );
}

void cledit_menu(char *s,int lines, char *owner)   /* driver editor menu */
{
   clrscr(lines);
   printf(owner);
   printf("\n\t%s\n\n",s);
   printf("\tA. Append\n");
   printf("\tD. Delete\n");
   printf("\tC. Change\n");
   printf("\tV. View\n");
   printf("\tH. Help\n");
   printf("\tX. eXit\n");
}

static void load_class(void)
{
	int i;
	FILE *inf;

	if ((inf = ixfopen("ccntlrcl.cdf","r")) == NULL)
	{
		printf("\tSorry, can't open ccntlrcl.cdf\n");
		return;
	}
	dirty = 0;
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
		store_old(i);
		ctlcl[i].deleted = NO;
	}
	fclose(inf); /* fall through into display */
}

static void keep_class(void)
{
      int len, i, j;
      FILE *outf;
      char string[80];
      char defname[NAMMAX+PERIOD+EXT+1];

      if (nctlcls == 0)
	 if ( !nodata("No Controller classes are defined") )
	    return;

      printf("Job Identification(up to %d char)> ",DESMAX);
      gets(string);
      len = strlen(string);
      if (len != 0)
      {
	 blank(&job_id[0]);
	 if (len > DESMAX)
	    len = DESMAX;
	 for (i = 0; i < len; i++)
	    job_id[i] = string[i];
      }

      if ((outf = ixfopen("ccntlrcl.cdf","w")) == NULL)
      {
	 printf("\tSorry, can't create ccntlrcl.cdf\n");
	 return;
      }
      s_gettime();
      fprintf(outf,"%20s\n",job_id);
      fprintf(outf,"%14s\n",timestamp);

      j = nctlcls;
      for (i = 1; i <= nctlcls; i++)
	   if( ctlcl[i].deleted == YES )
		j--;
      fprintf(outf,"%d\n",j); /* true number of controllers after deletions */
      fprintf(outf,"%d\n",ndeffed);
      for (i = 1; i <= nctlcls; i++)
      {
	   if( ctlcl[i].deleted == NO )
	   {
		if( strncmp( &ctlcl[i].oldname[0], &ctlcl[i].name[0], NAMMAX ) != 0 )
			save_header( &ctlcl[i].oldname[0], &ctlcl[i].name[0], DDF );
		if( strncmp( &ctlcl[i].oldspf[0], &ctlcl[i].spfname[0], NAMMAX ) != 0 )
			save_header( &ctlcl[i].oldspf[0], &ctlcl[i].spfname[0], SPF );
		fprintf(outf,"%3d%8s%8s%s\n", ctlcl[i].deflag,
					      &ctlcl[i].name[0],
					      &ctlcl[i].spfname[0],
					      &ctlcl[i].drvlist[0]);
	   }
	   else
	   {
		delete_drivers( &ctlcl[i].drvlist[0] );
		convert_2_def( &ctlcl[i].spfname[0], &defname[0], SPF );
		remove( defname );
		convert_2_def( &ctlcl[i].spfname[0], &defname[0], HDR );
		remove( defname );
		for (j = i; j <= nctlcls; j++)
		     memcpy(&ctlcl[j],&ctlcl[j+1],sizeof(struct ctlclrec));
		i--;
		nctlcls--;
	   }
      }
      fclose(outf);
      dirty = 0;
}

static void store_old( int index )
{
	strcpy( &ctlcl[index].oldname[0], &ctlcl[index].name[0] );
	strcpy( &ctlcl[index].oldspf[0], &ctlcl[index].spfname[0] );
}

static void delete_drivers(char *drvlist)
{
	 int i, ndrives, nddeffed;
	 FILE *inf;
	 char defname[NAMMAX+PERIOD+EXT+1];

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
	 }
	 fclose(inf); /* fall through into display */

	 for (i = 1; i <= ndrives; i++)
	 {
		convert_2_def( &drivers[i].spfname[0], &defname[0], SPF );
		remove( defname );
		convert_2_def( &drivers[i].spfname[0], &defname[0], HDR );
		remove( defname );
		convert_2_def( &drivers[i].errname[0], &defname[0], ERR );
		remove( defname );
		convert_2_def( &drivers[i].errname[0], &defname[0], HDR );
		remove( defname );
	 }
	 remove( drvlist );
}

void convert_2_print(char *filename, char *defname)
{
	while( *filename != '\0')
	{
	       if( *filename != ' ')
			*defname++ = *filename++;
	       else
			filename++;
	}
	*defname = '\0';
}

static int setup_selection(void)
{
    int i, j;

    j=1;
    for(i=1;i<=nctlcls;i++)
    {
	  if( ctlcl[i].deflag )
	  {
	      selected_cl[j] = &ctlcl[i];
	      j++;
	  }
    }
    return(j-1);
}
#endif
