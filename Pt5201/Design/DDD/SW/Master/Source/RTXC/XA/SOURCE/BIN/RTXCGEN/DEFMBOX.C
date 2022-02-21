/* defmbox.c - mailbox definition program for RTXCgen */

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

#define NMBOXES 64 /* max # of mailboxes supported, arbitrary */
#define DESMAX 20 /* max # of chars in description */
#define NAMMAX 8  /* max # of chars in mbox name */

int nmboxes;

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

struct mboxrec
{
    char name[NAMMAX+1];
    char desc[DESMAX+1];
} mbox[1+NMBOXES];

static void menu(void), zero(void), define(int);
static int getmbox(char *);

static int dirty;

void defmbox(void)
{
    int j, i, q, max, len;
    int n;
    char string[80];
    FILE *inf, *outf, *outh;

    dirty = 0;
    nmboxes = 0;
    zero();
    menu();

loop:
    printf("\nMailbox Command> ");
    gets(string);
    strupr(string);
    switch(string[0])
    {
        case 'E': /* edit mailbox */
editor:
           printf("\nMailbox Editor Command> ");
           gets(string);
           strupr(string);
           switch(string[0])
           {
               case 'A': /* append mode */
                   if (nmboxes >= NMBOXES)
                   {
                       printf("\tMaximum number of mailboxes exceeded\n");
                       goto editor;
                   }
                   dirty++;
                   nmboxes++;
                   blank(mbox[nmboxes].name);
                   blank(mbox[nmboxes].desc);
                   define(nmboxes);
                   break;

               case 'I': /* insert mode */
insert:
                   i = getmbox("Before mailbox #> ");
                   if (i == 0)
                   {
                       printf("\tInsert aborted\n");
                       break;
                   }

                   if (i < 0 || i > nmboxes)
                   {
                       printf("\tInvalid mailbox number\n");
                       goto insert;
                   }

                   if (nmboxes+1 > NMBOXES)
                   {
                       printf("\tMaximum number of mailboxes exceeded\n");
                       break;
                   }

                   dirty++;
                   max = nmboxes + 1;
                   for (j = max; j >= i; j--)
                       memcpy(&mbox[j],&mbox[j-1],sizeof(struct mboxrec));
                   blank(mbox[i].name);
                   blank(mbox[i].desc);
                   define(i);
                   ++nmboxes;
                   break;

               case 'D': /* delete mode */
delete:
                   i = getmbox("Delete mailbox #> ");
                   if (i == 0)
                   {
                       printf("\tDelete aborted\n");
                       break;
                   }

                   if (i < 0 || i > nmboxes)
                   {
                       printf("\tInvalid mailbox number\n");
                       goto delete;
                   }

                   dirty++;
                   for (j = i; j <= nmboxes; j++)
                       memcpy(&mbox[j],&mbox[j+1],sizeof(struct mboxrec));
                   nmboxes--;
                   break;

                case 'C': /* change mode */
change:
                    i = getmbox("Change mailbox #> " );
                    if (i == 0)
                    {
                        printf("\tChange aborted\n");
                        break;
                    }

                    if (i < 0 || i > nmboxes)
                    {
                        printf("\tInvalid mailbox number\n");
                        goto change;
                    }

                    printf("  #   Name   Description\n");
                       /*   123 12345678 1234567890... */
                    printf("%3d %8s %20s\n",
                            i,&mbox[i].name[0],&mbox[i].desc[0]);
                    define(i);
                    break;

                case 'M': /* move mode */
move:
                    q = getmbox("Move mailbox #> ");
                    if (q == 0)
                    {
                        printf("\tMove aborted\n");
                        break;
                    }

                    i = getmbox("-before mailbox #> ");
                    if (q == 0)
                    {
                        printf("\tMove aborted\n");
                        break;
                    }

                    if (q < 0 || q > nmboxes || i < 0 || i > nmboxes)
                    {
                        printf("\tInvalid mailbox number\n");
                        goto move;
                    }

                    dirty++;
                    memcpy(&mbox[0],&mbox[q],sizeof(struct mboxrec));
                    for (j = q; j < nmboxes; j++)
                        memcpy(&mbox[j],&mbox[j+1],sizeof(struct mboxrec));
                    max = nmboxes;
                    if (q > i)
                    {
                         for (j = max; j > i; j--)
                            memcpy(&mbox[j],&mbox[j-1],sizeof(struct mboxrec));
                         memcpy(&mbox[i],&mbox[0],sizeof(struct mboxrec));
                    }
                    else
                    {
                         for (j = max; j >= i; j--)
                            memcpy(&mbox[j],&mbox[j-1],sizeof(struct mboxrec));
                         memcpy(&mbox[i-1],&mbox[0],sizeof(struct mboxrec));
                    }
                    blank(&mbox[0].name[0]);
                    break;

                case 'S': /* swap mode */
swap:
                    q = getmbox("Swap mailbox #> ");
                    if (q == 0)
                    {
                        printf("\tSwap aborted\n");
                        break;
                    }

                    i = getmbox("-with mailbox #> ");
                    if (i == 0)
                    {
                        printf("\tSwap aborted\n");
                        break;
                    }

                    if (q < 0 || q > nmboxes || i < 0 || i > nmboxes)
                    {
                        printf("\tInvalid mailbox number\n");
                        goto swap;
                    }

                    dirty++;
                    memcpy(&mbox[0],&mbox[q],sizeof(struct mboxrec));
                    memcpy(&mbox[q],&mbox[i],sizeof(struct mboxrec));
                    memcpy(&mbox[i],&mbox[0],sizeof(struct mboxrec));
                    blank(&mbox[0].name[0]);
                    break;

                case 'V': /* view mode */
                    printf("%20s %14s\n",job_id,timestamp);
                    printf("  #   Name   Description\n");
                    for (n = SIZEOF_MH, i = 1, j = 1; i <= nmboxes;
                         n += SIZEOF_MH, i++, j++)
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
                             i,&mbox[i].name[0],&mbox[i].desc[0]);
                    }
                    printf("Total number of RAM bytes required = %d\n",n);
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
            if ((inf = ixfopen("cmbox.def","r")) == NULL)
            {
                printf("\tSorry, can't open cmbox.def\n");
                break;
            }
            dirty = 0;
            fscanf(inf,"%20c\n",&job_id[0]);
            fscanf(inf,"%14c\n",&timestamp[0]);
            fscanf(inf,"%d",&nmboxes);
            for (i = 1; i <= nmboxes; i++)
                fscanf(inf,"%d%8c%20c\n",
                    &i,&mbox[i].name[0],&mbox[i].desc[0]);
            fclose(inf); /* fall through into display */

        case 'V': /* view current defs */
            printf("%20s %14s\n",job_id,timestamp);
            printf("  #   Name   Description\n");

            for (n = SIZEOF_MH, i = 1, j = 1; i <= nmboxes;
                 n += SIZEOF_MH, i++, j++)
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
                    i,&mbox[i].name[0],&mbox[i].desc[0]);
            }
            printf("Total number of RAM bytes required = %d\n",n);
            break;

        case 'K': /* keep description file */
            if (nmboxes == 0)
               if ( !nodata("No mailboxes are defined") )
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

            if ((outf = ixfopen("cmbox.def","w")) == NULL)
            {
                printf("\tSorry, can't create cmbox.def\n");
                break;
            }
            s_gettime();
            fprintf(outf,"%20s\n",job_id);
            fprintf(outf,"%14s\n",timestamp);
            fprintf(outf,"%d\n",nmboxes);
            for (i = 1; i <= nmboxes; i++)
                fprintf(outf,"%3d%8s%20s\n",
                    i,&mbox[i].name[0],&mbox[i].desc[0]);
            fclose(outf);
            break;

        case 'F':
            dirty++;
             /* fall into generate case */

        case 'G': /* generate new C file */
           if (nmboxes == 0)
              if ( !nodata("No mailboxes are defined") )
                 break;

            if ((outf = oxfopen("cmbox.c","w")) == NULL)
            {
               printf("\tSorry, can't create cmbox.c\n");
               break;
            }

            s_gettime();
            fprintf(outf,
                "/* cmbox.c - RTXC Mailbox definitions - %20s %14s */\n\n",
                job_id,timestamp);
            finclude(outf,"copyrite.h");

            n_f(outf); /* near/far defines */

            fprintf(outf,"#include \"typedef.h\"\n");
            fprintf(outf,"#include \"rtxstruc.h\"\n\n");
            fprintf(outf,"#include \"cmbox.h\"\n\n");

            if (konst)
               fprintf(outf,"const ");
	    fprintf(outf,"MBOX nmboxes = NMBOXES;\n\n");
	    fprintf(outf,"MHEADER ks_tblx mheader[1+NMBOXES];\n\n");

            if (symflag) /* RTXCbug symbols enabled */
            {
               fprintf(outf,"#define NAMMAX %d\n\n",NAMMAX);
               if (konst)
                  fprintf(outf,"const ");
               fprintf(outf,"char mboxname[1+NMBOXES][NAMMAX+1] =\n");
               fprintf(outf,"{\n");
#if 0 /* names padded to NAMMAX chars vs mixed lengths */
               for (i = 0; i < nmboxes; i++)
                  fprintf(outf,"   \"%8s\",\n",&mbox[i].name[0]);
               fprintf(outf,"   \"%8s\"\n",&mbox[i].name[0]);
#else
               for (i = 0; i <= nmboxes; i++)
               {
                  fprintf(outf,"   \"");
                  for (j = 0; j < NAMMAX; j++)
                  {
                     if (mbox[i].name[j] == ' ')
                        break;
                     fprintf(outf,"%c",mbox[i].name[j]);
                  }

                  if (i < nmboxes) /* add comma on all but last */
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
                printf("\tNew cmbox.h not generated\n");
                break;
            }

            if ((outh = oxfopen("cmbox.h","w")) == NULL)
            {
                printf("\tSorry, can't create cmbox.h");
                break;
            }
            fprintf(outh,"/* cmbox.h - RTXC Mailbox include file */\n\n");
            finclude(outh,"copyrite.h");
    	    fprintf(outh,"#define NMBOXES %d\n\n",nmboxes);
	        for (i = 1; i <= nmboxes; i++)
            {
                if (mbox[i].name[0] != ' ')
                    fprintf(outh,"#define %8s %d /* %20s */\n",
                        &mbox[i].name[0],i,&mbox[i].desc[0]);
            }

            dirty = 0;    /* clean again */
            fclose(outh);
            break;

        case 'P': /* print current mailbox definitions */
            fprintf(stdprn,"RTXC Mailbox Definitions - %20s %14s\n\n",
                job_id,timestamp);
            fprintf(stdprn,"  #   Name   Description\n");
                         /* 123 12345678 1234567890... */
            for (n = SIZEOF_MH, i = 1; i <= nmboxes; n += SIZEOF_MH, i++)
                fprintf(stdprn,"%3d %8s %20s\n",
                    i,&mbox[i].name[0],&mbox[i].desc[0]);
            fprintf(stdprn,"Total number of RAM bytes required = %d\n",n);
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
    printf("\t *** Mailbox definition module ***\n\n");
    printf("\tL. Load existing definition file\n");
    printf("\tE. Edit individual mailbox parameters\n");
    printf("\tV. View current mailbox definitions\n");
    printf("\tK. Keep new definition file\n");
    printf("\tG. Generate new C source file\n");
    printf("\tF. Force .H file generation\n");
    printf("\tP. Print current mailbox definitions\n");
    printf("\tH. Help\n");
    printf("\tX. Exit module\n");
}

static void zero(void)
{
    int i, j;

    for (i = 0; i < NMBOXES; i++)
    {
        for (j = 0; j < NAMMAX; j++)
            mbox[i].name[j] = ' ';
        for (j = 0; j < DESMAX; j++)
            mbox[i].desc[j] = ' ';
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
      blank(&mbox[q].name[0]);
      if (len > NAMMAX)
         len = NAMMAX;
      for (i = 1; i <= len; i++)
         if (string[i] == ' ')
         {
            printf("\tSpaces not permitted in Name\n");
            goto name;
         }
      for (i = 0; i < len; i++)
         mbox[q].name[i] = string[i];
      if (mbox[q].name[0] == ' ')
         for (i = 0; i < NAMMAX; i++)
            mbox[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (mbox[q].name[0] == ' ')
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
      blank(&mbox[q].desc[0]);
      if (len > DESMAX)
         len = DESMAX;
      for (i = 0; i < len; i++)
         mbox[q].desc[i] = string[i];
      dirty++;
   }
}

/* numeric or symbolic mailbox references */
static int getmbox(char *p)
{
   int imbox;
   char string[12]; /* 12345678\0 */

   printf("%s",p);
   gets(string);
   strupr(string);

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      imbox = atoi(string);
      if ( (imbox < 0) || (imbox > nmboxes) )
         imbox = 0;
      return(imbox);
   }
   else
   {
      for (imbox = 1; imbox <= nmboxes; imbox++)
      {
         if (!strcmp(string,&mbox[imbox].name[0]))
            return(imbox);
      }
      return(0); /* not found */
   }
}
