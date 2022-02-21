/* rtxcgen.c - RTXCgen main() definition program */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "sysopts.h"
#include "rtxcopts.h"

extern void defclock(void);
extern void defpart(void);
extern void defqueue(void);
extern void defres(void);
extern void defsema(void);
extern void deftask(void);
extern void defmbox(void);

int getnum(char *);             /* output prompt and return int  */
long getlnum(char *);           /* output prompt and return long */
void edit_menu(void);           /* editor menu */
void blank(char *);             /* blanks string */
void s_gettime(void);           /* time and date */
FILE *ixfopen(const char *,const char *); /* open input file with directory */

/* CONFIG, command line switches */
int konst = 1;          /* 1=use const, 0=no const */
int nfflag = 0;         /* 1=use near/far, 0=don't user near/far */
int symflag = 1;        /* 1=generate RTXCbug symbols */
char idirname[64] = "."; /* directory name for "input" files (.def) */
char odirname[64] = "."; /* directory name for "output" - .c and .h */

char timestamp[14+1];   /* xx/xx/xx nn:nn */

static void menu(void);
static void sysmenu(void);

void clrscr(int);                       /* screen clear given so many lines */

void main(int,char *[]);

void main(int argc,char *argv[])
{
   char string[80];
   int i;

   printf("Embedded System Products, Inc. - RTXCgen 04/30/97 Version 3.2\n");
   printf("Copyright (c) 1986-1997.  All rights reserved.\n");

   for (i = 1; i < argc; i++) /* argv[0] is pgm name */
   {
      switch(argv[i][0])
      {
         case '-': /* unix switches */
         case '/': /* DOS switches */
            switch(argv[i][1])
            {
               case 'k': /* konst switch OFF */
               case 'K':
                  konst = 0;
                  break;

               case 'd': /* suppress RTXCbug symbols */
               case 'D':
                  symflag = 0;
                  break;

               case 'o': /* output directory */
               case 'O':
                  if (argv[i][2])
                     strcpy(odirname,&argv[i][2]);
                  else
                  {
                     i++;
                     strcpy(odirname,&argv[i][0]);
                  }
                  break;

               case 'i': /* input directory */
               case 'I':
                  if (argv[i][2])
                     strcpy(idirname,&argv[i][2]);
                  else
                  {
                     i++;
                     strcpy(idirname,&argv[i][0]);
                  }
                  break;

               default:
                  fprintf(stderr,"usage: rtxcgen [-d] [-k] [-m]\n");
                  exit(1);
            }
            break;

         default:
            fprintf(stderr,"usage: rtxcgen [-d] [-k] [-m]\n");
            exit(1);
      }
   }

sysloop:
   sysmenu();
   printf("\nSystem Generation - Command > ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'K':
loop:
         menu();
         printf("\nCommand > ");
         gets(string);
         strupr(string);
         switch(string[0])
         {
            case 'S':
               defsema();
               break;

#ifdef HAS_RESOURCES
            case 'R':
               defres();
               break;
#endif

#ifdef HAS_PARTITIONS
            case 'P': /* formerly 'M' - defmem() */
               defpart();
               break;
#endif

            case 'C':
               defclock();
               break;

            case 'T':
               deftask();
               break;

#ifdef HAS_QUEUES
            case 'Q':
               defqueue();
               break;
#endif

#ifdef HAS_MAILBOXES
            case 'M':
               defmbox();
               break;
#endif

            case 'H':
               break;

            case 'X':
               goto outloop;

            default:
               printf("\a\tInvalid command\n");
               break;
         }
         goto loop;
outloop:
             break;

      case 'H':
         break;

      case 'X':
         exit(0);

      default:
         printf("\a\tInvalid command\n");
         break;
   }
   goto sysloop;
}

static void sysmenu(void)
{
   clrscr(17);
   printf("\t **** SYSTEM GENERATION ****\n\n");
   printf("\tK. RTXC\n");
   printf("\tH. Help\n");
   printf("\tX. eXit\n");
}


static void menu(void)
{
   clrscr(10);
   printf("\t **** RTXC SYSGEN ****\n\n");
   printf("\tT. Task definitions\n");
   printf("\tS. Semaphore definitions\n");
#ifdef HAS_RESOURCES
   printf("\tR. Resource definitions\n");
#endif
#ifdef HAS_QUEUES
   printf("\tQ. Queue definitions\n");
#endif
#ifdef HAS_PARTITIONS
   printf("\tP. Partition definitions\n");
#endif
#ifdef HAS_MAILBOXES
   printf("\tM. Mailbox definitions\n");
#endif
   printf("\tC. Clock definitions\n");
   printf("\tH. Help\n");
   printf("\tX. Exit\n");
}

int getnum(char *p)
{
   char buff[80];

   printf("%s",p);
   gets(buff);
   return(atoi(buff));
}

long getlnum(char *p)
{
   char buff[80];

   printf("%s",p);
   gets(buff);
   return(atol(buff));
}

void blank(char *p)  /* blanks string */
{
   while (*p != '\0')
      *p++ = ' ';
   return;
}

void clrscr(int nlines) /* screen clear given so many lines */
{
   int i;

   for (i = 1; i <= nlines; i++)
      printf("\n");
}

void edit_menu(void)   /* editor menu */
{
   printf("\t*** Definition editor ***\n");
   printf("\tA. Append\n");
   printf("\tI. Insert\n");
   printf("\tD. Delete\n");
   printf("\tC. Change\n");
   printf("\tM. Move\n");
   printf("\tS. Swap\n");
   printf("\tV. View\n");
   printf("\tX. Exit\n");
   printf("\tH. Help\n");
}

void s_gettime(void)  /* discover time and date, MSC format */
{
   struct tm *p;
   time_t ltime;

   time(&ltime);                 /* get time in secs */
   p = localtime(&ltime);        /* convert to struct tm */
   p->tm_mon++; /* month returned as 0-11 */
   sprintf(&timestamp[0],"%02d/%02d/%02d %02d:%02d",
      p->tm_mon,p->tm_mday,p->tm_year,p->tm_hour,p->tm_min);
}

/*
 * utility routine to #include file within another
*/
void finclude(FILE *outf,char *fname)
{
   FILE *inf;
   int c;

   if ((inf = ixfopen(fname,"r")) != NULL)
   {
      while( (c = getc(inf)) != EOF)
      {
         putc(c,outf);
      }
      fclose(inf);
   }
}

/*
 * utility routine to include near/far null #defines
*/
void n_f(FILE *outf)
{

}

int nodata(char *prompt)
{
   char string[10];

   printf(prompt);
   printf("\a, do you want to continue? ");
   gets(string);
   strupr(string);
   if (string[0] == 'Y')
      return(1);
   else
      return(0);
}

/* open "input" file using extension (directory) */
FILE *ixfopen(const char *ifilename,const char *mode)
{
   char xifilename[64];

   strcpy(xifilename,idirname);
   strcat(xifilename,"\\");
   strcat(xifilename,ifilename);
   return(fopen(xifilename,mode));
}

/* open "output" file using extension (directory) */
FILE *oxfopen(const char *ofilename,const char *mode)
{
   char xofilename[64];

   strcpy(xofilename,odirname);
   strcat(xofilename,"\\");
   strcat(xofilename,ofilename);
   return(fopen(xofilename,mode));
}

/* End of file - rtxcgen.c */