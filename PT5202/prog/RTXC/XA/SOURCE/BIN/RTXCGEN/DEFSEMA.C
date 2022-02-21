/* defsema.c - semaphore definition program for RTXCgen */

/*
 *   RTXC    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

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

/* CONFIG */

#define NSEMAS 64 /* max # of semaphores supported, arbitrary */
#define DESMAX 20 /* max # of chars in description */
#define NAMMAX 8  /* max # of chars in sema name */

int nsemas;
int stksiz; /* semaphore interrupt queue size */

extern char timestamp[];
extern int konst; /* use const flag */
extern int symflag;

static char job_id[DESMAX+1];

extern void blank(char *);
extern void finclude(FILE *,char *);
extern void edit_menu(void);
extern int nodata(char *);
extern void s_gettime(void);
extern void n_f(FILE *);
extern FILE *ixfopen(const char *,const char *);
extern FILE *oxfopen(const char *,const char *);
extern void RTXCstrupr(char *);

struct semarec
{
    char name[NAMMAX+1];
    char desc[DESMAX+1];
} sema[1+NSEMAS];

static void menu(void), zero(void), define(int);
static int getsema(char *);

static int dirty;

void defsema(void)
{
    int j, i, q, max, len;
    long n;
    char string[80];
    FILE *inf, *outf, *outh;

    dirty = 0;
    nsemas = 0;
    zero();
    menu();

loop:
    printf("\nSemaphore Command> ");
    gets(string);
    strupr(string);
    switch(string[0])
    {
        case 'E': /* edit semaphore */
editor:
           printf("\nSemaphore Editor Command> ");
           gets(string);
           strupr(string);
           switch(string[0])
           {
               case 'A': /* append mode */
                   if (nsemas >= NSEMAS)
                   {
                       printf("\tMaximum number of semaphores exceeded\n");
                       goto editor;
                   }
                   dirty++;
                   nsemas++;
                   blank(&sema[nsemas].name[0]);
                   blank(&sema[nsemas].desc[0]);
                   define(nsemas);
                   stksiz = nsemas;
                   break;

               case 'I': /* insert mode */
insert:
                   i = getsema("Before semaphore #> ");
                   if (i == 0)
                   {
                       printf("\tInsert aborted\n");
                       break;
                   }

                   if (i < 0 || i > nsemas)
                   {
                       printf("\tInvalid semaphore number\n");
                       goto insert;
                   }

                   if (nsemas+1 > NSEMAS)
                   {
                       printf("\tMaximum number of semaphores exceeded\n");
                       break;
                   }

                   dirty++;
                   max = nsemas + 1;
                   for (j = max; j >= i; j--)
                       memcpy(&sema[j],&sema[j-1],sizeof(struct semarec));
                   blank(&sema[i].name[0]);
                   blank(&sema[i].desc[0]);
                   define(i);
                   ++nsemas;
                   stksiz = nsemas;
                   break;

               case 'D': /* delete mode */
delete:
                   i = getsema("Delete semaphore #> ");
                   if (i == 0)
                   {
                       printf("\tDelete aborted\n");
                       break;
                   }

                   if (i < 0 || i > nsemas)
                   {
                       printf("\tInvalid semaphore number\n");
                       goto delete;
                   }

                   dirty++;
                   for (j = i; j <= nsemas; j++)
                       memcpy(&sema[j],&sema[j+1],sizeof(struct semarec));
                   nsemas--;
                   stksiz = nsemas;
                   break;

                case 'C': /* change mode */
change:
                    i = getsema("Change semaphore #> " );
                    if (i == 0)
                    {
                        printf("\tChange aborted\n");
                        break;
                    }

                    if (i < 0 || i > nsemas)
                    {
                        printf("\tInvalid semaphore number\n");
                        goto change;
                    }

                    printf("  #   Name   Description\n");
                       /*   123 12345678 1234567890... */
                    printf("%3d %8s %20s\n",
                            i,&sema[i].name[0],&sema[i].desc[0]);
                    define(i);
                    break;

                case 'M': /* move mode */
move:
                    q = getsema("Move semaphore #> ");
                    if (q == 0)
                    {
                        printf("\tMove aborted\n");
                        break;
                    }

                    i = getsema("-before semaphore #> ");
                    if (q == 0)
                    {
                        printf("\tMove aborted\n");
                        break;
                    }

                    if (q < 0 || q > nsemas || i < 0 || i > nsemas)
                    {
                        printf("\tInvalid semaphore number\n");
                        goto move;
                    }

                    dirty++;
                    memcpy(&sema[0],&sema[q],sizeof(struct semarec));
                    for (j = q; j < nsemas; j++)
                        memcpy(&sema[j],&sema[j+1],sizeof(struct semarec));
                    max = nsemas;
                    if (q > i)
                    {
                         for (j = max; j > i; j--)
                            memcpy(&sema[j],&sema[j-1],sizeof(struct semarec));
                         memcpy(&sema[i],&sema[0],sizeof(struct semarec));
                    }
                    else
                    {
                         for (j = max; j >= i; j--)
                            memcpy(&sema[j],&sema[j-1],sizeof(struct semarec));
                         memcpy(&sema[i-1],&sema[0],sizeof(struct semarec));
                    }
                    blank(&sema[0].name[0]);
                    break;

                case 'S': /* swap mode */
swap:
                    q = getsema("Swap semaphore #> ");
                    if (q == 0)
                    {
                        printf("\tSwap aborted\n");
                        break;
                    }

                    i = getsema("-with semaphore #> ");
                    if (i == 0)
                    {
                        printf("\tSwap aborted\n");
                        break;
                    }

                    if (q < 0 || q > nsemas || i < 0 || i > nsemas)
                    {
                        printf("\tInvalid semaphore number\n");
                        goto swap;
                    }

                    dirty++;
                    memcpy(&sema[0],&sema[q],sizeof(struct semarec));
                    memcpy(&sema[q],&sema[i],sizeof(struct semarec));
                    memcpy(&sema[i],&sema[0],sizeof(struct semarec));
                    blank(&sema[0].name[0]);
                    break;

                case 'V': /* view mode */
                    printf("%20s %14s\n",job_id,timestamp);
                    printf("  #   Name   Description\n");
                    for (n = SIZEOF_SEMA, i = 1, j = 1; i <= nsemas;
                         n += SIZEOF_SEMA, i++, j++)
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
                         printf("%3d %8s %20s\n",
                             i,&sema[i].name[0],&sema[i].desc[0]);
                    }
#ifdef SEMA_USE_TABLE /* { */
                    n += (1 + nsemas);      /* add bytes for sema_usage[] */
#endif /* } SEMA_USE_TABLE */
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
            if ((inf = ixfopen("csema.def","r")) == NULL)
            {
                printf("\tSorry, can't open csema.def\n");
                break;
            }
            dirty = 0;
            fscanf(inf,"%20c\n",&job_id[0]);
            fscanf(inf,"%14c\n",&timestamp[0]);
            fscanf(inf,"%d",&nsemas);
            fscanf(inf,"%d",&stksiz);
            for (i = 1; i <= nsemas; i++)
                fscanf(inf,"%d%8c%20c\n",
                    &i,&sema[i].name[0],&sema[i].desc[0]);
            fclose(inf); /* fall through into display */

        case 'V': /* view current defs */
            printf("%20s %14s\n",job_id,timestamp);
            printf("  #   Name   Description\n");
            for (n = SIZEOF_SEMA, i = 1, j = 1; i <= nsemas;
                         n += SIZEOF_SEMA, i++, j++)
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
                printf("%3d %8s %20s\n",
                    i,&sema[i].name[0],&sema[i].desc[0]);
            }
#ifdef SEMA_USE_TABLE /* { */
            n += (1 + nsemas);      /* add bytes for sema_usage[] */
#endif /* } SEMA_USE_TABLE */
            printf("Total number of RAM bytes required = %ld\n",n);
            break;

        case 'K': /* keep description file */
            if (nsemas == 0)
               if ( !nodata("No semaphores are defined") )
                  break;

            printf("Job Identification (up to %d char)> ",DESMAX);
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

            if ((outf = ixfopen("csema.def","w")) == NULL)
            {
                printf("\tSorry, can't create csema.def\n");
                break;
            }
            s_gettime();
            fprintf(outf,"%20s\n",job_id);
            fprintf(outf,"%14s\n",timestamp);
            fprintf(outf,"%d\n",nsemas);
            fprintf(outf,"%d\n",stksiz);
            for (i = 1; i <= nsemas; i++)
                fprintf(outf,"%3d%8s%20s\n",
                    i,&sema[i].name[0],&sema[i].desc[0]);
            fclose(outf);
            break;

        case 'F':
            dirty++;

        case 'G': /* generate new C file */
           if (nsemas == 0)
              if ( !nodata("No semaphores are defined") )
                 break;

            if ((outf = oxfopen("csema.c","w")) == NULL)
            {
               printf("\tSorry, can't create csema.c\n");
               break;
            }

            s_gettime();
            fprintf(outf,
                "/* csema.c - RTXC Semaphore definitions - %20s %14s */\n\n",
                job_id,timestamp);
            finclude(outf,"copyrite.h");

            n_f(outf); /* near/far defines */

            fprintf(outf,"#include \"typedef.h\"\n");
            fprintf(outf,"#include \"rtxstruc.h\"\n\n");
            fprintf(outf,"#include \"csema.h\"\n\n");

            fprintf(outf,"#define SIGLISTSIZE %d /* sema list size */\n\n",
                SIGLISTSIZE);
            if (konst)
               fprintf(outf,"const ");
	    fprintf(outf,"SEMA nsemas = NSEMAS;\n\n");
            if (konst)
               fprintf(outf,"const ");
	    fprintf(outf,"int siglistsize = SIGLISTSIZE;\n\n");
	    fprintf(outf,"SEMA ks_datax siglist[SIGLISTSIZE];/* sema list */\n\n");
	    fprintf(outf,"SSTATE ks_tblx semat[1+NSEMAS];\n\n");
#ifdef SEMA_USE_TABLE /* { */
	    fprintf(outf,"char sema_usage[1+NSEMAS];\n\n");
#endif /* } SEMA_USE_TABLE */

            if (symflag) /* RTXCbug symbols enabled */
            {
               fprintf(outf,"#define NAMMAX %d\n\n",NAMMAX);
               if (konst)
                  fprintf(outf,"const ");
               fprintf(outf,"char semaname[1+NSEMAS][NAMMAX+1] =\n");
               fprintf(outf,"{\n");
#if 0 /* names padded to NAMMAX chars vs mixed lengths */
               for (i = 0; i < nsemas; i++)
                  fprintf(outf,"   \"%8s\",\n",&sema[i].name[0]);
               fprintf(outf,"   \"%8s\"\n",&sema[i].name[0]);
#else
               for (i = 0; i <= nsemas; i++)
               {
                  fprintf(outf,"   \"");
                  for (j = 0; j < NAMMAX; j++)
                  {
                     if (sema[i].name[j] == ' ')
                        break;
                     fprintf(outf,"%c",sema[i].name[j]);
                  }

                  if (i < nsemas) /* add comma on all but last */
                     fprintf(outf,"\",\n");
                  else
                     fprintf(outf,"\"\n");
               }
#endif
               fprintf(outf,"};\n");
            }

            fclose(outf);

            if (!dirty)
            {
                printf("\tNew csema.h not generated\n");
                break;
            }

            if ((outh = oxfopen("csema.h","w")) == NULL)
            {
                printf("\tSorry, can't create csema.h");
                break;
            }
            fprintf(outh,"/* csema.h - RTXC Semaphore include file */\n\n");
            finclude(outh,"copyrite.h");
    	    fprintf(outh,"#define NSEMAS %d\n\n",nsemas);
	        for (i = 1; i <= nsemas; i++)
            {
                if (sema[i].name[0] != ' ')
                    fprintf(outh,"#define %8s %d /* %20s */\n",
                        &sema[i].name[0],i,&sema[i].desc[0]);
            }

            dirty = 0;    /* clean again */
            fclose(outh);
            break;

        case 'P': /* print current semaphore definitions */
            fprintf(stdprn,"RTXC Semaphore Definitions - %20s %14s\n\n",
                job_id,timestamp);
            fprintf(stdprn,"  #   Name   Description\n");
                         /* 123 12345678 1234567890... */
            for (i = 1; i <= nsemas; i++)
                fprintf(stdprn,"%3d %8s %20s\n",
                    i,&sema[i].name[0],&sema[i].desc[0]);
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
    printf("\t *** Semaphore definition module ***\n\n");
    printf("\tL. Load existing definition file\n");
    printf("\tE. Edit individual semaphore parameters\n");
    printf("\tV. View current semaphore definitions\n");
    printf("\tK. Keep new definition file\n");
    printf("\tG. Generate new C source file\n");
    printf("\tF. Force .H file generation\n");
    printf("\tP. Print current semaphore definitions\n");
    printf("\tH. Help\n");
    printf("\tX. Exit module\n");
}

static void zero(void)
{
    int i, j;

    for (i = 0; i <= NSEMAS; i++)
    {
        for (j = 0; j < NAMMAX; j++)
            sema[i].name[j] = ' ';
        for (j = 0; j < DESMAX; j++)
            sema[i].desc[j] = ' ';
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
      blank(&sema[q].name[0]);
      if (len > NAMMAX)
         len = NAMMAX;
      for (i = 1; i <= len; i++)
         if (string[i] == ' ')
         {
            printf("\tSpaces not permitted in Name\n");
            goto name;
         }
      for (i = 0; i < len; i++)
         sema[q].name[i] = string[i];
      if (sema[q].name[0] == ' ')
         for (i = 0; i < NAMMAX; i++)
            sema[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (sema[q].name[0] == ' ')
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
      blank(&sema[q].desc[0]);
      if (len > DESMAX)
         len = DESMAX;
      for (i = 0; i < len; i++)
         sema[q].desc[i] = string[i];
      dirty++;
   }
}

/* numeric or symbolic sema references */
static int getsema(char *p)
{
   int isema;
   char string[12]; /* 12345678\0 */

   printf("%s",p);
   gets(string);
   strupr(string);

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      isema = atoi(string);
      if ( (isema < 0) || (isema > nsemas) )
         isema = 0;
      return(isema);
   }
   else
   {
      for (isema = 1; isema <= nsemas; isema++)
      {
         if (!strcmp(string,&sema[isema].name[0]))
            return(isema);
      }
      return(0); /* not found */
   }
}
