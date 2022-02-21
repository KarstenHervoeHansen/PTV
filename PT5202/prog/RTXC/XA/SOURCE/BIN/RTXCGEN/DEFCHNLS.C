/* defchnls.c - RTXCio - channel definition program for RTXCgen */
/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include "sysopts.h"

#ifdef RTXCIO

#include <stdio.h>
#include <string.h>

#include "typedef.h"
#include "rtxstruc.h"
#include "rtxcgen.h"

#include "rtio_inc.h"                  /* RTXCio - Header file include file */

#define NCHNLS 64  /* max # of channels supported, arbitrary */
#define DESMAX 20 /* max # chars in description field */

extern char timestamp[];
extern int konst; /* use const flag */

extern void finclude(FILE *,char *);
extern void blank(char *);
extern int nodata(char *);
extern int getnum(char *);
extern void s_gettime(void);
extern void n_f(FILE *);
extern FILE *ixfopen(const char *,const char *);
extern FILE *oxfopen(const char *,const char *);
extern void clrscr(int);                /* screen clear given so many lines */
extern void RTXCstrupr(char *);

static void menu(void);
static void zero(void);

int nchnls;

static char job_id[DESMAX+1];

static int dirty;

void defchnls(void)
{
   int tmp, i, len;
   char string[80];
   FILE *inf, *outf, *outh;

   dirty = 0;
   zero();
   menu();
loop:
   printf("\nRTXCio Channel Object - Command > ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'E': /* edit channel parameters */
channels:
	 tmp = getnum("Number of I/O Channels > ");
	 if (tmp > NCHNLS )
         {
	       printf("\tMaximum number of channels exceeded!\n");
	       goto channels;
	 }
         dirty++;
	 nchnls = tmp;
         break;

      case 'L': /* load existing file */
	 if ((inf = ixfopen("cchnls.def","r")) == NULL)
         {
	    printf("\tSorry, can't open cchnls.def\n");
            break;
         }
         dirty = 0;
         fscanf(inf,"%20c\n",&job_id[0]);
         fscanf(inf,"%14c\n",&timestamp[0]);
	 fscanf(inf,"%d",&nchnls);
         fclose(inf); /* fall through into display */

      case 'V': /* view current def */
         printf("%20s %14s\n",job_id,timestamp);
	 printf("Number of I/O Channels > %d\n",nchnls);
         printf("Total number of RAM bytes required = %d\n",
	    nchnls * SIZEOF_CHANNELS);
         break;

      case 'K': /* keep description file */
	 if (nchnls == 0)
	    if ( !nodata("Are you sure no Channels are needed?") )
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

	 if ((outf = ixfopen("cchnls.def","w")) == NULL)
         {
	    printf("\tSorry, can't create cchnls.def\n");
            break;
         }

         s_gettime();
         fprintf(outf,"%20s\n",job_id);
         fprintf(outf,"%14s\n",timestamp);
	 fprintf(outf,"%d\n",nchnls);
         fclose(outf);
         break;

      case 'F':
         dirty++;

      case 'G': /* generate new C file  */
	 if (nchnls == 0)
	    if ( !nodata("Are you sure no Channels are needed?") )
               break;

	 if ((outf = oxfopen("cchnls.c","w")) == NULL)
         {
	    printf("\tSorry, can't create cchnls.c\n");
            break;
         }

         s_gettime();
       fprintf(outf,"/* cchnls.c - RTXCio Channel definitions - %20s %14s */\n\n",
            job_id,timestamp);
	 finclude(outf,"..\\rtxcio\\copyrite.h");

         n_f(outf); /* near/far defines */

	 fprintf(outf,"#include \"typedef.h\"                            /* RTXC - type declarations */\n\n");
	 fprintf(outf,"#include \"rtio_inc.h\"                  /* RTXCio - Header file include file */\n\n");
	 fprintf(outf,"#include \"cchnls.h\"                           /* USER - channel definitions */\n\n");

         fprintf(outf,"const ");
         fprintf(outf,"CHNLID nchnls = NCHNLS;\n\n");

	 fprintf(outf,"CCB_ENTRY * chnls_free;                             /* List of free channel   */\n");
	 fprintf(outf,"                                                    /* control blocks.        */\n");
	 fprintf(outf,"CCB_ENTRY * chnls_active;                           /* List of active channel */\n");
	 fprintf(outf,"                                                    /* control blocks.        */\n\n");
         fprintf(outf,"CCB_ENTRY channel_table[NCHNLS+1];\n");
	 fclose(outf);

         if (!dirty)
         {
	    printf("\tNew cchnls.h not generated\n");
            break;
         }

	 if ((outh = oxfopen("cchnls.h","w")) == NULL)
         {
	    printf("\tSorry, can't create cchnls.h\n");
            break;
         }
	 fprintf(outh,"/* cchnls.h - RTXCio Channel include file */\n\n");
	 finclude(outh,"..\\rtxcio\\copyrite.h");
	 fprintf(outf,"#define NCHNLS %d    /* number of channels */\n",nchnls);
         dirty = 0;    /* clean again */
         fclose(outh);
         break;

      case 'P': /* print current def */
	 fprintf(stdprn,"RTXCio Channels definitions - %20s %14s\n",
            job_id,timestamp);
	 printf("Number of I/O Channels > %d\n",nchnls);
	 printf("Total number of RAM bytes required = %d\n",
	    nchnls * SIZEOF_CHANNELS);
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
   clrscr(10);
   printf("\t*** RTXCio - Channel Object menu ***\n\n");
   printf("\tL. Load existing definition file\n");
   printf("\tE. Edit channel parameters\n");
   printf("\tV. View current channel definitions\n");
   printf("\tK. Keep new definition file\n");
   printf("\tG. Generate RTXC source files\n");
   printf("\tF. Force .H file generation\n");
   printf("\tP. Print current channel definitions\n");
   printf("\tH. Help\n");
   printf("\tX. Exit module\n");
}

static void zero(void)
{
   int j;

   nchnls = 0;
   for (j = 0; j < 14; j++)
      timestamp[j] = ' ';
   for (j = 0; j < DESMAX; j++)
      job_id[j] = ' ';
}
#endif
