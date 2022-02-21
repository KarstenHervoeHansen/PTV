/* defres.c - resource definition program for RTXCgen */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "typedef.h"
#include "rtxstruc.h"
#include "rtxcgen.h"

/* CONFIG */

#define NRES 64   /* max # of resources supported, arbitrary */
#define DESMAX 20 /* max # of chars in description */
#define NAMMAX 8  /* max # of chars in resource name */

extern void blank(char *);
extern void finclude(FILE *,char *);
extern int nodata(char *);
extern void s_gettime(void);
extern void edit_menu(void);
extern void n_f(FILE *);
extern FILE *ixfopen(const char *,const char *);
extern FILE *oxfopen(const char *,const char *);

extern char timestamp[];
extern int konst; /* const use flag */
extern int symflag;

int nres;
static char job_id[DESMAX+1];

struct resrec
{
   char name[NAMMAX+1];
   char desc[DESMAX+1];
} res[1+NRES];

static void menu(void), define(int), zero(void);
static int getres(char *);

static int dirty;

void defres(void)
{
   int i, j, q, len, max;
   long n;
   char string[80];
   FILE *inf, *outf, *outh;

   dirty = 0;
   nres = 0;
   zero();
   menu();
loop:
   printf("\nResource Command> ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'E': /* edit resource */
editor:
         printf("\nResource Editor Command> ");
         gets(string);
         strupr(string);
         switch(string[0])
         {
            case 'A': /* append mode */
               if (nres >= NRES)
               {
                  printf("\tMaximum number of resources exceeded\n");
                  goto editor;
               }
               dirty++;
               nres++;
               blank(&res[nres].name[0]);
               blank(&res[nres].desc[0]);
               define(nres);
               break;

            case 'I': /* insert mode */
insert:
               i = getres("Before resource #> ");
               if (i == 0)
               {
                  printf("\tInsert aborted\n");
                  break;
               }

               if (i < 0 || i > nres)
               {
                  printf("\tInvalid resource number\n");
                  goto insert;
               }

               if (nres+1 > NRES)
               {
                  printf("\tMaximum number of resources exceeded\n");
                  break;
               }

               dirty++;
               max = nres + 1;
               for (j = max; j >= i; j--)
                  memcpy(&res[j],&res[j-1],sizeof(struct resrec));
               blank(&res[i].name[0]);
               blank(&res[i].desc[0]);
               define(i);
               ++nres;
               break;

            case 'D': /* delete mode */
delete:
               i = getres("Delete resource #> ");
               if (i == 0)
               {
                  printf("\tDelete aborted\n");
                  break;
               }

               if (i < 0 || i > nres)
               {
                  printf("\tInvalid resource number\n");
                  goto delete;
               }

               dirty++;
               for (j = i; j <= nres; j++)
                  memcpy(&res[j],&res[j+1],sizeof(struct resrec));
               nres--;
               break;

            case 'C': /* change mode */
change:
               i = getres("Change resource #> " );
               if (i == 0)
               {
                  printf("\tChange aborted\n");
                  break;
               }

               if (i < 0 || i > nres)
               {
                  printf("\tInvalid resource number\n");
                  goto change;
               }

               printf("  #   Name   Description\n");
                   /*  123 12345678 1234567890... */
               printf("%3d %8s %20s\n",i,&res[i].name[0],&res[i].desc[0]);
               define(i);
               break;

            case 'M': /* move mode */
move:
               q = getres("Move resource #> ");
               if (q == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

               i = getres("-before resource #> ");
               if (i == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

               if (q < 0 || q > nres || i < 0 || i > nres)
               {
                  printf("\tInvalid resource number\n");
                  goto move;
               }

               dirty++;
               memcpy(&res[0],&res[q],sizeof(struct resrec));
               for (j = q; j < nres; j++)
                  memcpy(&res[j],&res[j+1],sizeof(struct resrec));
               max = nres;
               if (q > i)
               {
                  for (j = max; j > i; j--)
                     memcpy(&res[j],&res[j-1],sizeof(struct resrec));
                  memcpy(&res[i],&res[0],sizeof(struct resrec));
               }
               else
               {
                  for (j = max; j >= i; j--)
                     memcpy(&res[j],&res[j-1],sizeof(struct resrec));
                  memcpy(&res[i-1],&res[0],sizeof(struct resrec));
               }
               blank(&res[0].name[0]);
               break;

            case 'S': /* swap mode */
swap:
               q = getres("Swap resource #> ");
               if (q == 0)
               {
                  printf("Swap aborted\n");
                  break;
               }

               i = getres("-with resource #> ");
               if (i == 0)
               {
                  printf("Swap aborted\n");
                  break;
               }

               if (q < 0 || q > nres || i < 0 || i > nres)
               {
                  printf("\tInvalid resource number\n");
                  goto swap;
               }

               dirty++;
               memcpy(&res[0],&res[q],sizeof(struct resrec));
               memcpy(&res[q],&res[i],sizeof(struct resrec));
               memcpy(&res[i],&res[0],sizeof(struct resrec));
               blank(&res[0].name[0]);
               break;

            case 'V': /* view mode */
               printf("%20s %14s\n",job_id,timestamp);
               printf("  #   Name   Description\n");
                    /* 123 12345678 1234567890... */

               for (n = SIZEOF_RH, i = 1, j = 1; i <= nres;
                    n += SIZEOF_RH, i++, j++)
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
                  printf("%3d %8s %20s\n",i,&res[i].name[0],&res[i].desc[0]);
               }
               printf("Total number of RAM bytes required = %ld\n",n);
               break;

            case 'X': /* exit editor */
               menu();
               goto loop;

            case 'H': /* help */
               edit_menu();
               break;

            default:
               printf("\t\aInvalid command\n");
               break;
         }
         goto editor;

      case 'L': /* load existing file */
         if ((inf = ixfopen("cres.def","r")) == NULL)
         {
            printf("\tSorry, can't open cres.def\n");
            break;
         }
         dirty = 0;
         fscanf(inf,"%20c\n",&job_id[0]);
         fscanf(inf,"%14c\n",&timestamp[0]);
         fscanf(inf,"%d",&nres);
         for (i = 1; i <= nres; i++)
            fscanf(inf,"%d%8c%20c\n",&i,&res[i].name[0],&res[i].desc[0]);
         fclose(inf); /* fall through into display */

      case 'V': /* view current defs */
         printf("%20s %14s\n",job_id,timestamp);
         printf("  #   Name   Description\n");
             /*  123 12345678 1234567890... */

         for (n = SIZEOF_RH, i = 1, j = 1; i <= nres;
              n += SIZEOF_RH, i++, j++)
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
            printf("%3d %8s %20s\n",i,&res[i].name[0],&res[i].desc[0]);
         }
         printf("Total number of RAM bytes required = %ld\n",n);
         break;

   case 'K': /* keep description file */
      if (nres == 0)
         if ( !nodata("No resources are defined") )
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

      if ((outf = ixfopen("cres.def","w")) == NULL)
      {
         printf("\tSorry, can't create cres.def\n");
         break;
      }
      s_gettime();
      fprintf(outf,"%20s\n",job_id);
      fprintf(outf,"%14s\n",timestamp);
      fprintf(outf,"%d\n",nres);
      for (i = 1; i <= nres; i++)
         fprintf(outf,"%3d%8s%20s\n",i,&res[i].name[0],&res[i].desc[0]);
      fclose(outf);
      break;

   case 'F':
      dirty++;

   case 'G': /* generate new C file  */
      if (nres == 0)
         if ( !nodata("No resources are defined") )
            break;

      if ((outf = oxfopen("cres.c","w")) == NULL)
      {
         printf("\tSorry, can't create cres.c\n");
         break;
      }
      s_gettime();
      fprintf(outf,"/* cres.c - RTXC Resource definitions - %20s %14s */\n\n",
         job_id,timestamp);
      finclude(outf,"copyrite.h");

      n_f(outf); /* near/far defines */

      fprintf(outf,"#include \"typedef.h\"\n");
      fprintf(outf,"#include \"rtxstruc.h\"\n\n");
      fprintf(outf,"#include \"cres.h\"\n\n");

      if (konst)
         fprintf(outf,"extern const ");
      fprintf(outf,"RESOURCE near nres = NRES;\n\n");

      fprintf(outf,"RHEADER near rheader[1+NRES];\n\n");

      if (symflag) /* RTXCbug symbols enabled */
      {
         fprintf(outf,"#define NAMMAX %d\n\n",NAMMAX);
         if (konst)
            fprintf(outf,"extern const ");
         fprintf(outf,"char resname[1+NRES][NAMMAX+1] =\n");
         fprintf(outf,"{\n");
#if 0 /* names padded to NAMMAX chars vs mixed lengths */
         for (i = 0; i < nres; i++)
            fprintf(outf,"   \"%8s\",\n",&res[i].name[0]);
         fprintf(outf,"   \"%8s\"\n",&res[i].name[0]);
#else
         for (i = 0; i <= nres; i++)
         {
            fprintf(outf,"   \"");
            for (j = 0; j < NAMMAX; j++)
            {
               if (res[i].name[j] == ' ')
                  break;
               fprintf(outf,"%c",res[i].name[j]);
            }

            if (i < nres) /* add comma on all but last */
               fprintf(outf,"\",\n");
            else
               fprintf(outf,"\"\n");
         }
#endif
         fprintf(outf,"};\n");
      }
      fprintf(outf,"\n/* End of file - cres.c */\n");
      fclose(outf);

      if (!dirty)
      {
          printf("\tNew cres.h not generated\n");
          break;
      }

      if ((outh = oxfopen("cres.h","w")) == NULL)
      {
         printf("\tSorry, can't create cres.h\n");
         break;
      }
      fprintf(outh,"/* cres.h - RTXC Resource include file */\n\n");
      finclude(outh,"copyrite.h");
      fprintf(outh,"#define NRES %d\n\n",nres);
      for (i = 1; i <= nres; i++)
      {
         if (res[i].name[0] != ' ')
            fprintf(outh,"#define %8s %d /* %20s */\n",
               res[i].name,i,res[i].desc);
      }
      fprintf(outf,"\n/* End of file - cres.h */\n");

      dirty = 0;
      fclose(outh);
      break;

#if 0
   case 'P': /* print current resource definitions */
      fprintf(stdprn," RTXC Resource definitions - %20s %14s\n\n",
         job_id,timestamp);
      fprintf(stdprn,"%20s\n\n", job_id);
      fprintf(stdprn,"  #   Name   Description\n");
                   /* 123 12345678 1234567890... */
      for (n = SIZEOF_RH, i = 1; i <= nres; n += SIZEOF_RH, i++)
         fprintf(stdprn,"%3d %8s %20s\n",i,res[i].name,res[i].desc);
      fprintf(stdprn,"Total number of RAM bytes required = %ld\n",n);
      fprintf(stdprn,"\f");
      break;
#endif

   case 'H':
      menu();
      break;

   case 'X':
      return;

   default:
      printf("\t\aInvalid command\n");
      break;
   }
   goto loop;
}

static void menu(void)
{
   printf("\t *** Resource definition module ***\n\n");
   printf("\tL. Load existing definition file\n");
   printf("\tE. Edit individual resource parameters\n");
   printf("\tV. View current resource definitions\n");
   printf("\tK. Keep new definition file\n");
   printf("\tG. Generate new C source file\n");
   printf("\tF. Force .H file generation\n");
#if 0
   printf("\tP. Print current resource definitions\n");
#endif
   printf("\tH. Help\n");
   printf("\tX. Exit module\n");
}

static void zero(void)
{
   int i, j;

   for (i = 0; i <= NRES; i++)
   {
      for (j = 0; j < NAMMAX; j++)
         res[i].name[j] = ' ';
      for (j = 0; j < DESMAX; j++)
         res[i].desc[j] = ' ';
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
      blank(res[q].name);
      if (len > NAMMAX)
         len = NAMMAX;
      for (i = 1; i <= len; i++)
         if (string[i] == ' ')
         {
            printf("\tSpaces not permitted in name\n");
            goto name;
         }
      for (i = 0; i < len; i++)
         res[q].name[i] = string[i];
      if (res[q].name[0] == ' ')
         for (i = 0; i < NAMMAX; i++)
             res[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (res[q].name[0] == ' ')
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
      blank(res[q].desc);
      if (len > DESMAX)
         len = DESMAX;
      for (i = 0; i < len; i++)
         res[q].desc[i] = string[i];
      dirty++;
   }
}

/* numeric or symbolic resource references */
static int getres(char *p)
{
   int ires;
   char string[12]; /* 12345678\0 */

   printf("%s",p);
   gets(string);
   strupr(string);

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      ires = atoi(string);
      if ( (ires < 0) || (ires > nres) )
         ires = 0;
      return(ires);
   }
   else
   {
      for (ires = 1; ires <= nres; ires++)
      {
         if (!strcmp(string,&res[ires].name[0]))
            return(ires);
      }
      return(0); /* not found */
   }
}

/* End of file - defres.c */