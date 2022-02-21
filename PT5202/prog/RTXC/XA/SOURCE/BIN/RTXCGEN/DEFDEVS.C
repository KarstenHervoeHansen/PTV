/* defdevs.c - RTXCio - Device definition program for RTXCgen */
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

#define NDEV 32   /* max # of devices supported, arbitrary */
#define DESMAX 20 /* max # of chars in description */
#define NAMMAX 8  /* max # of chars in resource name */

extern int getnum(char *);
extern void blank(char *);
extern void finclude(FILE *,char *);
extern int nodata(char *);
extern void s_gettime(void);
extern void n_f(FILE *);
extern FILE *ixfopen(const char *,const char *);
extern FILE *oxfopen(const char *,const char *);
extern void RTXCstrupr(char *);

extern char timestamp[];
extern int konst; /* const use flag */
extern int symflag;

int ndevs;
static char job_id[DESMAX+1];

void objedit_menu(char *);   /* RTXCio object editor menu */

struct devrec
{
   char name[NAMMAX+1];
   char desc[DESMAX+1];
   char cntlr[NAMMAX+1];
   char initf[NAMMAX+1];
    int unitno;
   char sema4[NAMMAX+1];
   char res[NAMMAX+1];
   char attb[NAMMAX+3];
} dev[1+NDEV];

static void menu(void), define(int), zero(void);
static int getdev(char *);

static int dirty;

void defdevs(void)
{
   int i, j, q, len, max;
   long n;
   char string[80];
   FILE *inf, *outf, *outh;
   int compv;

   dirty = 0;
   ndevs = 0;
   compv = 0;

   zero();
   menu();
loop:
   printf("\nRTXCio Device Object - Command > ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'E': /* edit resource */
      objedit_menu("*** Device Object editor ***");
editor:
	 printf("\nRTXCio Device Object Editor - Command > ");
         gets(string);
         strupr(string);
         switch(string[0])
         {
            case 'A': /* append mode */
	       if (ndevs >= NDEV)
               {
		  printf("\tMaximum number of devices exceeded\n");
                  goto editor;
               }
               dirty++;
	       ndevs++;
	       blank(&dev[ndevs].name[0]);
	       blank(&dev[ndevs].desc[0]);
	       blank(&dev[ndevs].cntlr[0]);
	       blank(&dev[ndevs].initf[0]);
	       blank(&dev[ndevs].sema4[0]);
	       blank(&dev[ndevs].res[0]);
	       blank(&dev[ndevs].attb[0]);
	       define(ndevs);
               break;

            case 'I': /* insert mode */
insert:
	       i = getdev("Before device #> ");
               if (i == 0)
               {
                  printf("\tInsert aborted\n");
                  break;
               }

	       if (i < 0 || i > ndevs)
               {
		  printf("\tInvalid device number\n");
                  goto insert;
               }

	       if (ndevs+1 > NDEV)
               {
		  printf("\tMaximum number of devices exceeded\n");
                  break;
               }

               dirty++;
	       max = ndevs + 1;
               for (j = max; j >= i; j--)
		  memcpy(&dev[j],&dev[j-1],sizeof(struct devrec));
	       blank(&dev[i].name[0]);
	       blank(&dev[i].desc[0]);
               define(i);
	       ++ndevs;
               break;

            case 'D': /* delete mode */
delete:
	       i = getdev("Delete device #> ");
               if (i == 0)
               {
                  printf("\tDelete aborted\n");
                  break;
               }

	       if (i < 0 || i > ndevs)
               {
		  printf("\tInvalid device number\n");
                  goto delete;
               }

               dirty++;
	       for (j = i; j <= ndevs; j++)
		  memcpy(&dev[j],&dev[j+1],sizeof(struct devrec));
	       ndevs--;
               break;

            case 'C': /* change mode */
change:
	       i = getdev("Change device #> " );
               if (i == 0)
               {
                  printf("\tChange aborted\n");
                  break;
               }

	       if (i < 0 || i > ndevs)
               {
		  printf("\tInvalid device number\n");
                  goto change;
               }

	       printf("\n  #    Device     Cntlr  Unitno  Init_func  Resource  Semaphore  Attribute\n");
		     /*  123  12345678  12345678     123   12345678  12345678   12345678  1234567891 */
	       printf("%3d  %8s  %8s     %3d   %8s  %8s   %8s  %10s\n",
		       i,&dev[i].name[0], &dev[i].cntlr[0], dev[i].unitno,
		       &dev[i].initf[0], &dev[i].res[0], &dev[i].sema4[0],
							  &dev[i].attb[0] );
	       printf("     Description: ");
		   /*          1234567890... */
	       printf("  %20s\n\n",&dev[i].desc[0]);

	       define(i);
               break;

            case 'M': /* move mode */
move:
	       q = getdev("Move device #> ");
               if (q == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

	       i = getdev("-before device #> ");
               if (i == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

	       if (q < 0 || q > ndevs || i < 0 || i > ndevs)
               {
		  printf("\tInvalid device number\n");
                  goto move;
               }

               dirty++;
	       memcpy(&dev[0],&dev[q],sizeof(struct devrec));
	       for (j = q; j < ndevs; j++)
		  memcpy(&dev[j],&dev[j+1],sizeof(struct devrec));
	       max = ndevs;
               if (q > i)
               {
                  for (j = max; j > i; j--)
		     memcpy(&dev[j],&dev[j-1],sizeof(struct devrec));
		  memcpy(&dev[i],&dev[0],sizeof(struct devrec));
               }
               else
               {
                  for (j = max; j >= i; j--)
		     memcpy(&dev[j],&dev[j-1],sizeof(struct devrec));
		  memcpy(&dev[i-1],&dev[0],sizeof(struct devrec));
               }
	       blank(&dev[0].name[0]);
               break;

            case 'S': /* swap mode */
swap:
	       q = getdev("Swap device #> ");
               if (q == 0)
               {
                  printf("Swap aborted\n");
                  break;
               }

	       i = getdev("-with device #> ");
               if (i == 0)
               {
                  printf("Swap aborted\n");
                  break;
               }

	       if (q < 0 || q > ndevs || i < 0 || i > ndevs)
               {
		  printf("\tInvalid device number\n");
                  goto swap;
               }

               dirty++;
	       memcpy(&dev[0],&dev[q],sizeof(struct devrec));
	       memcpy(&dev[q],&dev[i],sizeof(struct devrec));
	       memcpy(&dev[i],&dev[0],sizeof(struct devrec));
	       blank(&dev[0].name[0]);
               break;

            case 'V': /* view mode */
               printf("%20s %14s\n",job_id,timestamp);

	       for (n = SIZEOF_DEVICE, i = 1, j = 1; i <= ndevs;
		    n += SIZEOF_DEVICE, i++)
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
		  j++;
		  printf("\n  #    Device     Cntlr  Unitno  Init_func  Resource  Semaphore  Attribute\n");
			/*  123  12345678  12345678     123   12345678  12345678   12345678  1234567891 */
		  j++;
		  printf("%3d  %8s  %8s     %3d   %8s  %8s   %8s  %10s\n",
			  i,&dev[i].name[0], &dev[i].cntlr[0], dev[i].unitno,
			  &dev[i].initf[0], &dev[i].res[0], &dev[i].sema4[0],
							     &dev[i].attb[0] );
		  j++;
		  printf("     Description: ");
		      /*          1234567890... */
		  printf("  %20s\n",&dev[i].desc[0]);
		  j++;
	       }
	       printf("\n");
	       printf("Total number of RAM bytes required = %ld\n",n);
               break;

            case 'X': /* exit editor */
	       menu();
	       goto loop;

            case 'H': /* help */
	       objedit_menu("*** Device Object editor ***");
	       break;

            default:
               printf("\t\aInvalid command\n");
               break;
         }
         goto editor;

      case 'L': /* load existing file */
	 if ((inf = ixfopen("cdevice.def","r")) == NULL)
         {
	    printf("\tSorry, can't open cdevice.def\n");
            break;
         }
         dirty = 0;
         fscanf(inf,"%20c\n",&job_id[0]);
         fscanf(inf,"%14c\n",&timestamp[0]);
	 fscanf(inf,"%d",&ndevs);
	 for (i = 1; i <= ndevs; i++)
	    fscanf(inf,"%3d%8c%20c\n",&i,&dev[i].name[0],&dev[i].desc[0]);
	 for (i = 1; i <= ndevs; i++)
	    fscanf(inf,"%8c%8c%3d%8c%8c%8c%10c\n",&dev[i].name[0],
						 &dev[i].cntlr[0],
						 &dev[i].unitno,
						 &dev[i].initf[0],
						 &dev[i].res[0],
						 &dev[i].sema4[0],
						 &dev[i].attb[0] );

	 fclose(inf); /* fall through into display */

      case 'V': /* view current defs */
         printf("%20s %14s\n",job_id,timestamp);

	 for (n = SIZEOF_DEVICE, i = 1, j = 1; i <= ndevs;
	      n += SIZEOF_DEVICE, i++)
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
	    j++;
	    printf("\n  #    Device     Cntlr  Unitno  Init_func  Resource  Semaphore  Attribute\n");
		  /*  123  12345678  12345678     123   12345678  12345678   12345678  1234567891 */
	    j++;
	    printf("%3d  %8s  %8s     %3d   %8s  %8s   %8s  %10s\n",
			  i,&dev[i].name[0], &dev[i].cntlr[0], dev[i].unitno,
			  &dev[i].initf[0], &dev[i].res[0], &dev[i].sema4[0],
							     &dev[i].attb[0] );
	    j++;
	    printf("     Description: ");
		/*          1234567890... */
	    printf("  %20s\n",&dev[i].desc[0]);
	    j++;
	 }
	 printf("\n");
	 printf("Total number of RAM bytes required = %ld\n",n);
         break;

   case 'K': /* keep description file */
      if (ndevs == 0)
	 if ( !nodata("No devices are defined") )
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

      if ((outf = ixfopen("cdevice.def","w")) == NULL)
      {
	 printf("\tSorry, can't create cdevice.def\n");
         break;
      }
      s_gettime();
      fprintf(outf,"%20s\n",job_id);
      fprintf(outf,"%14s\n",timestamp);
      fprintf(outf,"%d\n",ndevs);
      for (i = 1; i <= ndevs; i++)
	 fprintf(outf,"%3d%8s%20s\n",i,&dev[i].name[0],&dev[i].desc[0]);
      for (i = 1; i <= ndevs; i++)
	 fprintf(outf,"%8s%8s%3d%8s%8s%8s%10s\n",&dev[i].name[0],
						&dev[i].cntlr[0],
						dev[i].unitno,
						&dev[i].initf[0],
						&dev[i].res[0],
						&dev[i].sema4[0],
						&dev[i].attb[0] );

      fclose(outf);
      break;

   case 'F':
      dirty++;

   case 'G': /* generate new C file  */
      if (ndevs == 0)
	 if ( !nodata("No devices are defined") )
            break;

      if ((outf = oxfopen("cdevice.c","w")) == NULL)
      {
	 printf("\tSorry, can't create cdevice.c\n");
         break;
      }
      s_gettime();
      fprintf(outf,"/* cdevice.c - RTXCio - device definitions - %20s %14s */\n\n",
         job_id,timestamp);
      finclude(outf,"..\\rtxcio\\copyrite.h");

      n_f(outf); /* near/far defines */

      fprintf(outf,"#include \"typedef.h\"                            /* RTXC - type declarations */\n");
      fprintf(outf,"#include \"csema.h\"                         /* RTXC - semaphore declarations */\n");
      fprintf(outf,"#include \"cres.h\"                           /* RTXC - resource declarations */\n\n");

      fprintf(outf,"#include \"rtio_inc.h\"                  /* RTXCio - Header file include file */\n\n");

      fprintf(outf,"#include \"cdevice.h\"                           /* USER - device definitions */\n");
      fprintf(outf,"#include \"ccntlr.h\"                        /* USER - controller definitions */\n\n");

      fprintf(outf,"#define NAMMAX %d\n\n",NAMMAX);

      fprintf(outf,"extern void %8s( DEV_INIT_PACK * );\n", &dev[1].initf[0]);
      for (i = 2; i <= ndevs; i++)
      {
	   for (j = 1; j < i; j++)
	   {
		compv = strncmp(&dev[j].initf[0], &dev[i].initf[0], NAMMAX);
		if( !compv )
			break;
	   }
	   if( j == i )
		fprintf(outf,"extern void %8s( DEV_INIT_PACK * );\n", &dev[i].initf[0]);
      }

      fprintf(outf,"\nconst ");
      fprintf(outf,"DEVID nldevs = NLDEVS;\n\n");

      fprintf(outf,"\nPDU_HEADER * logical_dev_tbl[NLDEVS+1];        /* logical to physical device tablee */\n\n");

      fprintf(outf,"const DEV_INIT_PACK dev_init_tbl[NLDEVS+1] = {\n");
      fprintf(outf,"{        0,        0, 0,        0,        0,        0,          0 },\n");
      for (i = 1; i <= ndevs; i++)
      {
	 if( i != ndevs )
	    fprintf(outf,"{ %8s, %8s, %d, %8s, %8s, %8s, %10s },\n",&dev[i].cntlr[0],
								    &dev[i].name[0],
								    dev[i].unitno,
								    &dev[i].initf[0],
								    &dev[i].res[0],
								    &dev[i].sema4[0],
								    &dev[i].attb[0] );
	 else
	    fprintf(outf,"{ %8s, %8s, %d, %8s, %8s, %8s, %10s }\n",&dev[i].cntlr[0],
								    &dev[i].name[0],
								    dev[i].unitno,
								    &dev[i].initf[0],
								    &dev[i].res[0],
								    &dev[i].sema4[0],
								    &dev[i].attb[0] );

      }
      fprintf(outf,"};\n\n");

      fprintf(outf,"const char devname[1+NLDEVS][NAMMAX+1] =\n");
      fprintf(outf,"{\n");
      for (i = 0; i <= ndevs; i++)
      {
	  fprintf(outf,"   \"");
	  for (j = 0; j < NAMMAX; j++)
	  {
	     if (dev[i].name[j] == ' ')
		break;
	     fprintf(outf,"%c",dev[i].name[j]);
	  }

	  if (i < ndevs) /* add comma on all but last */
	     fprintf(outf,"\",\n");
	  else
	     fprintf(outf,"\"\n");
      }
      fprintf(outf,"};\n");
      fclose(outf);

      if (!dirty)
      {
	  printf("\tNew cdevice.h not generated\n");
          break;
      }

      if ((outh = oxfopen("cdevice.h","w")) == NULL)
      {
	 printf("\tSorry, can't create cdevice.h\n");
         break;
      }
      fprintf(outh,"/* cdevice.h - RTXCio - Device include file */\n\n");
      finclude(outh,"..\\rtxcio\\copyrite.h");

      fprintf(outf,"#define NLDEVS %d\n\n",ndevs);

      for (i = 1; i <= ndevs; i++)
      {
	  fprintf(outf,"#define   ");
	  for (j = 0; j < NAMMAX; j++)
	  {
	     if (dev[i].name[j] == ' ')
		break;
	     fprintf(outf,"%c",dev[i].name[j]);
	  }
	  fprintf(outf,"%3d\n",i);
      }
      fprintf(outf,"\n");
      dirty = 0;
      fclose(outh);
      break;

   case 'P': /* print current device definitions */
      fprintf(stdprn," RTXCio - Device definitions - %20s %14s\n\n",
         job_id,timestamp);
      fprintf(stdprn,"%20s\n\n", job_id);

      for (n = SIZEOF_DEVICE, i = 1; i <= ndevs; n += SIZEOF_DEVICE, i++)
      {
	   fprintf(stdprn,"\n  #    Device     Cntlr  Unitno  Init_func  Resource  Semaphore  Attribute\n");
			 /*  123  12345678  12345678     123   12345678  12345678   12345678  1234567891 */

	   fprintf(stdprn,"%3d  %8s  %8s     %3d   %8s  %8s   %8s  %10s\n",
			  i,&dev[i].name[0], &dev[i].cntlr[0], dev[i].unitno,
			  &dev[i].initf[0], &dev[i].res[0], &dev[i].sema4[0],
							     &dev[i].attb[0] );
	   fprintf(stdprn,"     Description: ");
		      /*          1234567890... */
	   fprintf(stdprn,"  %20s\n",&dev[i].desc[0]);
      }
      fprintf(stdprn,"\n");
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
   printf("\n\t*** RTXCio - Device Object menu ***\n\n");
   printf("\tL. Load existing definition file\n");
   printf("\tE. Edit individual Device parameters\n");
   printf("\tV. View current Device definitions\n");
   printf("\tK. Keep new definition file\n");
   printf("\tG. Generate new C source file\n");
   printf("\tF. Force .H file generation\n");
   printf("\tP. Print current Device definitions\n");
   printf("\tH. Help\n");
   printf("\tX. Exit module\n");
}

static void zero(void)
{
   int i, j;

   for (i = 0; i <= NDEV; i++)
   {
      for (j = 0; j < NAMMAX; j++)
	 dev[i].name[j] = ' ';
      for (j = 0; j < DESMAX; j++)
	 dev[i].desc[j] = ' ';
   }
   for (j = 0; j < 14; j++)
      timestamp[j] = ' ';
   for (j = 0; j < DESMAX; j++)
      job_id[j] = ' ';
}

static void define(int q)
{
   int i, len, tmp;
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
      blank(dev[q].name);
      if (len > NAMMAX)
         len = NAMMAX;
      for (i = 1; i <= len; i++)
         if (string[i] == ' ')
         {
            printf("\tSpaces not permitted in name\n");
            goto name;
         }
      for (i = 0; i < len; i++)
	 dev[q].name[i] = string[i];
      if (dev[q].name[0] == ' ')
         for (i = 0; i < NAMMAX; i++)
	     dev[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (dev[q].name[0] == ' ')
      {
           printf("\tName required\n");
           goto name;
      }
   }
cntlr:
   printf("Controller name (up to %d char)> ",NAMMAX);
   gets(string);
   strupr(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto cntlr;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(dev[q].cntlr);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto cntlr;
	 }
      for (i = 0; i < len; i++)
	 dev[q].cntlr[i] = string[i];
      if (dev[q].cntlr[0] == ' ')
	 for (i = 0; i < NAMMAX; i++)
	     dev[q].cntlr[i] = ' ';
      dirty++;
   }
   else
   {
      if (dev[q].cntlr[0] == ' ')
      {
	   printf("\tName required\n");
	   goto cntlr;
      }
   }
unitno:
   printf("Device's unit number in the Controller> ");
   tmp = getnum("");
   if (tmp >= 0)
      dev[q].unitno = tmp;
   else
      goto unitno;

initf:
   printf("Initialization function name (up to %d char)> ",NAMMAX);
   gets(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto initf;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(dev[q].initf);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto initf;
	 }
      for (i = 0; i < len; i++)
	 dev[q].initf[i] = string[i];
      if (dev[q].initf[0] == ' ')
	 for (i = 0; i < NAMMAX; i++)
	     dev[q].initf[i] = ' ';
      dirty++;
   }
   else
   {
      if (dev[q].initf[0] == ' ')
      {
	   printf("\tName required\n");
	   goto initf;
      }
   }
sema4:
   printf("Device semaphore name (up to %d char)> ",NAMMAX);
   gets(string);
   strupr(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto sema4;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(dev[q].sema4);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto sema4;
	 }
      for (i = 0; i < len; i++)
	 dev[q].sema4[i] = string[i];
      if (dev[q].sema4[0] == ' ')
	 for (i = 0; i < NAMMAX; i++)
	     dev[q].sema4[i] = ' ';
      dirty++;
   }
   else
   {
      if (dev[q].sema4[0] == ' ')
      {
	   printf("\tName required\n");
	   goto sema4;
      }
   }
res:
   printf("Device resource name (up to %d char)> ",NAMMAX);
   gets(string);
   strupr(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto res;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(dev[q].res);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto res;
	 }
      for (i = 0; i < len; i++)
	 dev[q].res[i] = string[i];
      if (dev[q].res[0] == ' ')
	 for (i = 0; i < NAMMAX; i++)
	     dev[q].res[i] = ' ';
      dirty++;
   }
   else
   {
      if (dev[q].res[0] == ' ')
      {
	   printf("\tName required\n");
	   goto res;
      }
   }
attb:
   printf("Device attribute name (up to %d char)> ",(NAMMAX+2));
   gets(string);
   strupr(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto attb;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(dev[q].attb);
      if (len > (NAMMAX+2))
	 len = (NAMMAX+2);
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto attb;
	 }
      for (i = 0; i < len; i++)
	 dev[q].attb[i] = string[i];
      if (dev[q].attb[0] == ' ')
	 for (i = 0; i < (NAMMAX+2); i++)
	     dev[q].attb[i] = ' ';
      dirty++;
   }
   else
   {
      if (dev[q].attb[0] == ' ')
      {
	   printf("\tName required\n");
	   goto attb;
      }
   }

   printf("Description (up to %d char)> ",DESMAX);
   gets(string);
   len = strlen(string);
   if (len != 0)
   {
      blank(dev[q].desc);
      if (len > DESMAX)
         len = DESMAX;
      for (i = 0; i < len; i++)
	 dev[q].desc[i] = string[i];
      dirty++;
   }
}

/* numeric or symbolic resource references */
static int getdev(char *p)
{
   int idev;
   char string[12]; /* 12345678\0 */

   printf("%s",p);
   gets(string);
   strupr(string);

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      idev = atoi(string);
      if ( (idev < 0) || (idev > ndevs) )
	 idev = 0;
      return(idev);
   }
   else
   {
      for (idev = 1; idev <= ndevs; idev++)
      {
	 if (!strcmp(string,&dev[idev].name[0]))
	    return(idev);
      }
      return(0); /* not found */
   }
}

void objedit_menu(char *s)   /* RTXCio object editor menu */
{
   printf("\n\t%s\n\n",s);
   printf("\tA. Append\n");
   printf("\tI. Insert\n");
   printf("\tD. Delete\n");
   printf("\tC. Change\n");
   printf("\tM. Move\n");
   printf("\tS. Swap\n");
   printf("\tV. View\n");
   printf("\tX. eXit\n");
   printf("\tH. Help\n");
}
#endif
