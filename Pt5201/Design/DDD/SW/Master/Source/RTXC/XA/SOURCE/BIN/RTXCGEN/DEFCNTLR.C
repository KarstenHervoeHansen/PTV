/* defcntlr.c - RTXCio - controller definition program for RTXCgen */
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

#define NCONTS 12 /* max # of controllers supported */
#define DESMAX 20 /* max # of chars in description */
#define NAMMAX 8  /* max # of chars in resource name */

extern void defchnls(void);
extern void defdevs(void);

extern void blank(char *);
extern void finclude(FILE *,char *);
extern int nodata(char *);
extern void s_gettime(void);
extern void objedit_menu(char *);
extern void n_f(FILE *);
extern FILE *ixfopen(const char *,const char *);
extern FILE *oxfopen(const char *,const char *);
extern void clrscr(int);                /* screen clear given so many lines */
extern void RTXCstrupr(char *);

extern char timestamp[];
extern int konst; /* const use flag */
extern int symflag;

int ncntlrs;
static char job_id[DESMAX+1];

struct cntlrrec
{
   char name[NAMMAX+1];
   char desc[DESMAX+1];
} con[1+NCONTS];

void defcntlrs(void);

static void menu(void), define(int), zero(void);
static int getcntlr(char *);
static void ioobj_menu(void);

static int dirty;

void detioobj(void)
{
   char string[80];

objloop:
   ioobj_menu();
   printf("\nRTXCio Objects - Command > ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
	case 'C':
	    defcntlrs();
	    break;

	case 'D':
	    defdevs();
	    break;

	case 'L':
	    defchnls();
	    break;

	case 'H':
	    break;

	case 'X':
	    return;

	default:
	    printf("\t\aInvalid command\n");
	    break;
	}
	goto objloop;
}


void defcntlrs(void)
{
   int i, j, q, len, max;
   long n;
   char string[80];
   FILE *inf, *outf, *outh;

   dirty = 0;
   ncntlrs = 0;
   zero();
   menu();
loop:
   printf("\nRTXCio Controller Object - Command > ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'E': /* edit resource */
      objedit_menu("*** Controller Object editor ***");
editor:
	 printf("\nRTXCio Controller Object Editor - Command > ");
         gets(string);
         strupr(string);
         switch(string[0])
         {
            case 'A': /* append mode */
	       if (ncntlrs >= NCONTS)
               {
		  printf("\tMaximum number of controllers exceeded\n");
                  goto editor;
               }
               dirty++;
	       ncntlrs++;
	       blank(&con[ncntlrs].name[0]);
	       blank(&con[ncntlrs].desc[0]);
	       define(ncntlrs);
               break;

            case 'I': /* insert mode */
insert:
	       i = getcntlr("Before controller #> ");
               if (i == 0)
               {
                  printf("\tInsert aborted\n");
                  break;
               }

	       if (i < 0 || i > ncntlrs)
               {
		  printf("\tInvalid controller number\n");
                  goto insert;
               }

	       if (ncntlrs+1 > NCONTS)
               {
		  printf("\tMaximum number of controllers exceeded\n");
                  break;
               }

               dirty++;
	       max = ncntlrs + 1;
               for (j = max; j >= i; j--)
		  memcpy(&con[j],&con[j-1],sizeof(struct cntlrrec));
	       blank(&con[i].name[0]);
	       blank(&con[i].desc[0]);
               define(i);
	       ++ncntlrs;
               break;

            case 'D': /* delete mode */
delete:
	       i = getcntlr("Delete controller #> ");
               if (i == 0)
               {
                  printf("\tDelete aborted\n");
                  break;
               }

	       if (i < 0 || i > ncntlrs)
               {
		  printf("\tInvalid controller number\n");
                  goto delete;
               }

               dirty++;
	       for (j = i; j <= ncntlrs; j++)
		  memcpy(&con[j],&con[j+1],sizeof(struct cntlrrec));
	       ncntlrs--;
               break;

            case 'C': /* change mode */
change:
	       i = getcntlr("Change controller #> " );
               if (i == 0)
               {
                  printf("\tChange aborted\n");
                  break;
               }

	       if (i < 0 || i > ncntlrs)
               {
		  printf("\tInvalid controller number\n");
                  goto change;
               }

               printf("  #   Name   Description\n");
                   /*  123 12345678 1234567890... */
	       printf("%3d %8s %20s\n",i,&con[i].name[0],&con[i].desc[0]);
               define(i);
               break;

            case 'M': /* move mode */
move:
	       q = getcntlr("Move controller #> ");
               if (q == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

	       i = getcntlr("-before controller #> ");
               if (i == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

	       if (q < 0 || q > ncntlrs || i < 0 || i > ncntlrs)
               {
		  printf("\tInvalid controller number\n");
                  goto move;
               }

               dirty++;
	       memcpy(&con[0],&con[q],sizeof(struct cntlrrec));
	       for (j = q; j < ncntlrs; j++)
		  memcpy(&con[j],&con[j+1],sizeof(struct cntlrrec));
	       max = ncntlrs;
               if (q > i)
               {
                  for (j = max; j > i; j--)
		     memcpy(&con[j],&con[j-1],sizeof(struct cntlrrec));
		  memcpy(&con[i],&con[0],sizeof(struct cntlrrec));
               }
               else
               {
                  for (j = max; j >= i; j--)
		     memcpy(&con[j],&con[j-1],sizeof(struct cntlrrec));
		  memcpy(&con[i-1],&con[0],sizeof(struct cntlrrec));
               }
	       blank(&con[0].name[0]);
               break;

            case 'S': /* swap mode */
swap:
	       q = getcntlr("Swap controller #> ");
               if (q == 0)
               {
                  printf("Swap aborted\n");
                  break;
               }

	       i = getcntlr("-with controller #> ");
               if (i == 0)
               {
                  printf("Swap aborted\n");
                  break;
               }

	       if (q < 0 || q > ncntlrs || i < 0 || i > ncntlrs)
               {
		  printf("\tInvalid controller number\n");
                  goto swap;
               }

               dirty++;
	       memcpy(&con[0],&con[q],sizeof(struct cntlrrec));
	       memcpy(&con[q],&con[i],sizeof(struct cntlrrec));
	       memcpy(&con[i],&con[0],sizeof(struct cntlrrec));
	       blank(&con[0].name[0]);
               break;

            case 'V': /* view mode */
               printf("%20s %14s\n",job_id,timestamp);
               printf("  #   Name   Description\n");
                    /* 123 12345678 1234567890... */

	       for (n = SIZEOF_CNTLR, i = 1, j = 1; i <= ncntlrs;
		    n += SIZEOF_CNTLR, i++, j++)
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
		  printf("%3d %8s %20s\n",i,&con[i].name[0],&con[i].desc[0]);
               }
               printf("Total number of RAM bytes required = %ld\n",n);
               break;

            case 'X': /* exit editor */
	       menu();
	       goto loop;

            case 'H': /* help */
	       objedit_menu("*** Controller Object editor ***");
	       break;

            default:
               printf("\t\aInvalid command\n");
               break;
         }
         goto editor;

      case 'L': /* load existing file */
	 if ((inf = ixfopen("ccntlr.def","r")) == NULL)
         {
	    printf("\tSorry, can't open ccntlr.def\n");
            break;
         }
         dirty = 0;
         fscanf(inf,"%20c\n",&job_id[0]);
         fscanf(inf,"%14c\n",&timestamp[0]);
	 fscanf(inf,"%d",&ncntlrs);
	 for (i = 1; i <= ncntlrs; i++)
	    fscanf(inf,"%d%8c%20c\n",&i,&con[i].name[0],&con[i].desc[0]);
         fclose(inf); /* fall through into display */

      case 'V': /* view current defs */
         printf("%20s %14s\n",job_id,timestamp);
         printf("  #   Name   Description\n");
             /*  123 12345678 1234567890... */

	 for (n = SIZEOF_CNTLR, i = 1, j = 1; i <= ncntlrs;
	      n += SIZEOF_CNTLR, i++, j++)
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
	    printf("%3d %8s %20s\n",i,&con[i].name[0],&con[i].desc[0]);
         }
         printf("Total number of RAM bytes required = %ld\n",n);
         break;

   case 'K': /* keep description file */
      if (ncntlrs == 0)
	 if ( !nodata("No controllers are defined") )
            break;

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

      if ((outf = ixfopen("ccntlr.def","w")) == NULL)
      {
	 printf("\tSorry, can't create ccntlr.def\n");
         break;
      }
      s_gettime();
      fprintf(outf,"%20s\n",job_id);
      fprintf(outf,"%14s\n",timestamp);
      fprintf(outf,"%d\n",ncntlrs);
      for (i = 1; i <= ncntlrs; i++)
	 fprintf(outf,"%3d%8s%20s\n",i,&con[i].name[0],&con[i].desc[0]);
      fclose(outf);
      break;

   case 'F':
      dirty++;

   case 'G': /* generate new C file  */
      if (ncntlrs == 0)
	 if ( !nodata("No controllers are defined") )
            break;

      if ((outf = oxfopen("ccntlr.c","w")) == NULL)
      {
	 printf("\tSorry, can't create ccntlr.c\n");
         break;
      }
      s_gettime();
      fprintf(outf,"/* ccntlr.c - RTXCio - Controller definitions - %20s %14s */\n\n",
         job_id,timestamp);
      finclude(outf,"..\\rtxcio\\copyrite.h");

      n_f(outf); /* near/far defines */

      fprintf(outf,"#include \"typedef.h\"                            /* RTXC - type declarations */\n");
      fprintf(outf,"#include \"rtxstruc.h\"                      /* RTXC - structure declarations */\n\n");
      fprintf(outf,"#include \"rtio_inc.h\"                  /* RTXCio - Header file include file */\n\n");
      fprintf(outf,"#include \"ccntlr.h\"                        /* USER - controller definitions */\n\n");

      fprintf(outf,"#define NAMMAX %d\n\n",NAMMAX);

      fprintf(outf,"const ");
      fprintf(outf,"CNTLRID ncntlrs = NCNTLRS;\n\n");

      fprintf(outf,"CNTLR_UNIT * cntlr_loc_table[NCNTLRS+1];        /* Controller locator table */\n\n");
      fprintf(outf,"CNTLR_UNIT cntlr_table[NCNTLRS+1];               /* Controller Unit table */\n\n");

      fprintf(outf,"const char cntlrname[1+NCNTLRS][NAMMAX+1] =\n");
      fprintf(outf,"{\n");
      for (i = 0; i <= ncntlrs; i++)
      {
	  fprintf(outf,"   \"");
	  for (j = 0; j < NAMMAX; j++)
	  {
	     if (con[i].name[j] == ' ')
		break;
	     fprintf(outf,"%c",con[i].name[j]);
	  }

	  if (i < ncntlrs) /* add comma on all but last */
	     fprintf(outf,"\",\n");
	  else
	     fprintf(outf,"\"\n");
      }
      fprintf(outf,"};\n");
      fclose(outf);

      if (!dirty)
      {
	  printf("\tNew ccntlr.h not generated\n");
          break;
      }

      if ((outh = oxfopen("ccntlr.h","w")) == NULL)
      {
	 printf("\tSorry, can't create ccntlr.h\n");
         break;
      }
      fprintf(outh,"/* ccntlr.h - RTXCio - Controller include file */\n\n");
      finclude(outh,"..\\rtxcio\\copyrite.h");

      fprintf(outf,"#define NCNTLRS %d\n\n",ncntlrs);

      for (i = 1; i <= ncntlrs; i++)
      {
	  fprintf(outf,"#define   ");
	  for (j = 0; j < NAMMAX; j++)
	  {
	     if (con[i].name[j] == ' ')
		break;
	     fprintf(outf,"%c",con[i].name[j]);
	  }
	  fprintf(outf,"%3d\n",i);
      }
      fprintf(outf,"\n");
      dirty = 0;
      fclose(outh);
      break;

   case 'P': /* print current controller definitions */
      fprintf(stdprn," RTXCio - Controller Object Definitions - %20s %14s\n\n",
         job_id,timestamp);
      fprintf(stdprn,"%20s\n\n", job_id);
      fprintf(stdprn,"  #   Name   Description\n");
                   /* 123 12345678 1234567890... */
      for (n = SIZEOF_CNTLR, i = 1; i <= ncntlrs; n += SIZEOF_CNTLR, i++)
	 fprintf(stdprn,"%3d %8s %20s\n",i,con[i].name,con[i].desc);
      fprintf(stdprn,"Total number of RAM bytes required = %ld\n",n);
      fprintf(stdprn,"\f");
      break;

   case 'H':
      menu();
      break;

   case 'X':
      if (dirty) {
        printf("You have not generated source files for your changes.\n");
        printf("Do you really want to exit?");
        gets(string);
        RTXCstrupr(string);
        if (string[0]=='Y')
          return;
      }
      else return;
      break;

   default:
      printf("\t\aInvalid command\n");
      break;
   }
   goto loop;
}

static void menu(void)
{
   printf("\n\t*** RTXCio - Controller Object menu ***\n\n");
   printf("\tL. Load existing definition file\n");
   printf("\tE. Edit individual controller parameters\n");
   printf("\tV. View current controller definitions\n");
   printf("\tK. Keep new definition file\n");
   printf("\tG. Generate new C source file\n");
   printf("\tF. Force .H file generation\n");
   printf("\tP. Print current controller definitions\n");
   printf("\tH. Help\n");
   printf("\tX. Exit module\n");
}

static void zero(void)
{
   int i, j;

   for (i = 0; i <= NCONTS; i++)
   {
      for (j = 0; j < NAMMAX; j++)
	 con[i].name[j] = ' ';
      for (j = 0; j < DESMAX; j++)
	 con[i].desc[j] = ' ';
   }
   for (j = 0; j < 14; j++)
      timestamp[j] = ' ';
   for (j = 0; j < DESMAX; j++)
      job_id[j] = ' ';
}

static void define(int q)
{
   int i, len;
   char string[80];

name:
   printf("Name (up to %d char)> ",NAMMAX);
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
      blank(con[q].name);
      if (len > NAMMAX)
         len = NAMMAX;
      for (i = 1; i <= len; i++)
         if (string[i] == ' ')
         {
            printf("\tSpaces not permitted in name\n");
            goto name;
         }
      for (i = 0; i < len; i++)
	 con[q].name[i] = string[i];
      if (con[q].name[0] == ' ')
         for (i = 0; i < NAMMAX; i++)
	     con[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (con[q].name[0] == ' ')
      {
           printf("\tName required\n");
           goto name;
      }
   }

   printf("Description (up to %d char)> ",DESMAX);
   gets(string);
   len = strlen(string);
   if (len != 0)
   {
      blank(con[q].desc);
      if (len > DESMAX)
         len = DESMAX;
      for (i = 0; i < len; i++)
	 con[q].desc[i] = string[i];
      dirty++;
   }
}

/* numeric or symbolic resource references */
static int getcntlr(char *p)
{
   int icon;
   char string[12]; /* 12345678\0 */

   printf("%s",p);
   gets(string);
   strupr(string);

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      icon = atoi(string);
      if ( (icon < 0) || (icon > ncntlrs) )
	 icon = 0;
      return(icon);
   }
   else
   {
      for (icon = 1; icon <= ncntlrs; icon++)
      {
	 if (!strcmp(string,&con[icon].name[0]))
	    return(icon);
      }
      return(0); /* not found */
   }
}

static void ioobj_menu(void)
{
   clrscr(14);
   printf("\t**** RTXCio Objects ****\n\n");
   printf("\tC. Controllers\n");
   printf("\tD. Devices\n");
   printf("\tL. channeLs\n");
   printf("\tH. Help\n");
   printf("\tX. eXit\n");
}
#endif
