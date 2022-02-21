/* defclock.c - clock definition program for RTXCgen */

/*
 *   RTXC    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <stdio.h>
#include <string.h>

#include "typedef.h"
#include "rtxstruc.h"
#include "rtxcgen.h"

#define NTMRS 64  /* max # of timers supported, arbitrary */
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
extern void RTXCstrupr(char *);

static void menu(void);
static void zero(void);

int ntmrs;
int tick;
int rate;
static char job_id[DESMAX+1];

static int dirty;

void defclock(void)
{
   int tmp, i, len;
   char string[80];
   FILE *inf, *outf, *outh;

   dirty = 0;
   zero();
   menu();
loop:
   printf("\nClock Command> ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'E': /* edit clock parameters */
rate:
         tmp = getnum("Clock rate (in Hz)> ");
         if (tmp == 0)
         {
            if (rate == 0)
            {
               printf("\tUndefined clock rate\n");
               goto rate;
            }
            goto tick;
         }
         if (tmp < 0 || tmp > 1000)
         {
            printf("\tInvalid clock rate\n");
            goto rate;
         }
         dirty++;
         rate = tmp;

tick:
         tick = 1000 / rate;
         printf("Tick interval is %d ms\n",tick);

timers:
         tmp = getnum("Number of timers> ");
         if (tmp == 0)
         {
            if (ntmrs == 0)
            {
               printf("\tMust have at least 1 timer\n");
               goto timers;
            }
            break;
         }

         if (tmp < 0)
         {
            printf("\tInvalid number of timers\n");
            goto timers;
         }

         if (tmp > NTMRS)
         {
            printf("\tMaximum number of timers is %d\n",NTMRS);
            goto timers;
         }
         ntmrs = tmp;
         break;

      case 'L': /* load existing file */
         if ((inf = ixfopen("cclock.def","r")) == NULL)
         {
            printf("\tSorry, can't open cclock.def\n");
            break;
         }
         dirty = 0;
         fscanf(inf,"%20c\n",&job_id[0]);
         fscanf(inf,"%14c\n",&timestamp[0]);
         fscanf(inf,"%d",&rate);
         fscanf(inf,"%d",&tick);
         fscanf(inf,"%d",&ntmrs);
         fclose(inf); /* fall through into display */

      case 'V': /* view current def */
         printf("%20s %14s\n",job_id,timestamp);
         printf("Clock rate is %d Hz\n",rate);
         printf("Tick interval is %d ms\n",tick);
         printf("Maximum number of concurrent timers is %d\n",ntmrs);
         printf("Total number of RAM bytes required = %d\n",
            ntmrs * SIZEOF_CLKBLK);
         break;

      case 'K': /* keep description file */
         if (rate == 0 || tick == 0)
         {
            printf("\aClock parameters undefined, old files left intact\n");
            break;
         }

         if (ntmrs == 0)
            if ( !nodata("No timers are defined") )
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

         if ((outf = ixfopen("cclock.def","w")) == NULL)
         {
            printf("\tSorry, can't create cclock.def\n");
            break;
         }

         s_gettime();
         fprintf(outf,"%20s\n",job_id);
         fprintf(outf,"%14s\n",timestamp);
         fprintf(outf,"%d\n",rate);
         fprintf(outf,"%d\n",tick);
         fprintf(outf,"%d\n",ntmrs);
         fclose(outf);
         break;

      case 'F':
         dirty++;

      case 'G': /* generate new C file  */
         if (rate == 0 || tick == 0)
         {
            printf("\aClock parameters undefined, old files left intact\n");
            break;
         }

         if (ntmrs == 0)
            if ( !nodata("No timers are defined") )
               break;

         if ((outf = oxfopen("cclock.c","w")) == NULL)
         {
            printf("\tSorry, can't create cclock.c\n");
            break;
         }

         s_gettime();
       fprintf(outf,"/* cclock.c - RTXC Clock definitions - %20s %14s */\n\n",
            job_id,timestamp);
         finclude(outf,"copyrite.h");

         n_f(outf); /* near/far defines */

         fprintf(outf,"#include \"typedef.h\"\n");
         fprintf(outf,"#include \"rtxstruc.h\"\n\n");
         fprintf(outf,"#include \"cclock.h\"\n\n");


         if (konst)
            fprintf(outf,"const ");
	 fprintf(outf,"int ntmrs = NTMRS;\n");
         if (konst)
            fprintf(outf,"const ");
	 fprintf(outf,"int clktick = CLKTICK;\n");
         if (konst)
            fprintf(outf,"const ");
	 fprintf(outf,"int clkrate = CLKRATE;\n\n");
	 fprintf(outf,"CLKBLK ks_clkx clkq[NTMRS];\n");
         fclose(outf);

         if (!dirty)
         {
            printf("\tNew cclock.h not generated\n");
            break;
         }

         if ((outh = oxfopen("cclock.h","w")) == NULL)
         {
            printf("\tSorry, can't create cclock.h\n");
            break;
         }
         fprintf(outh,"/* cclock.h - RTXC Clock include file */\n\n");
         finclude(outh,"copyrite.h");
    	 fprintf(outh,"#define NTMRS %d /* number of timers */\n\n",ntmrs);
    	 fprintf(outh,"#define CLKRATE %d\n",rate);
         fprintf(outh,"#define CLKTICK %d\n",tick);
         dirty = 0;    /* clean again */
         fclose(outh);
         break;

      case 'P': /* print current def */
         fprintf(stdprn,"RTXC Clock definitions - %20s %14s\n",
            job_id,timestamp);
         fprintf(stdprn,"Clock rate is %d Hz\n",rate);
         fprintf(stdprn,"Tick interval is %d ms\n",tick);
         fprintf(stdprn,"Maximum number of concurrent timers is %d\n",ntmrs);
         fprintf(stdprn,"Total number of RAM bytes required = %d\n",
            ntmrs * SIZEOF_CLKBLK);
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
   printf("\t *** Clock definition module ***\n\n");
   printf("\tL. Load existing definition file\n");
   printf("\tE. Edit clock parameters\n");
   printf("\tV. View current clock definitions\n");
   printf("\tK. Keep new definition file\n");
   printf("\tG. Generate RTXC source files\n");
   printf("\tF. Force .H file generation\n");
   printf("\tP. Print current clock definitions\n");
   printf("\tH. Help\n");
   printf("\tX. Exit module\n");
}

static void zero(void)
{
   int j;

   ntmrs = 0;
   tick = 0;
   rate = 0;
   for (j = 0; j < 14; j++)
      timestamp[j] = ' ';
   for (j = 0; j < DESMAX; j++)
      job_id[j] = ' ';
}
