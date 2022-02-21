/* defqueue.c - queue definition program for RTXCgen */

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
int romqh = ROMQH;

#define NQUEUES 64 /* max # of queues supported, arbitrary */
#define DESMAX 20  /* max # of chars in description */
#define NAMMAX 8   /* max # of chars in queue name */
#define MINSIZE 1  /* minimum number of queues */

extern void blank(char *);
extern void finclude(FILE *,char *);
extern int getnum(char *);
extern void s_gettime(void);
extern int nodata(char *);
extern void edit_menu(void);
extern void n_f(FILE *);
extern FILE *ixfopen(const char *,const char *);
extern FILE *oxfopen(const char *,const char *);

extern char timestamp[];
extern int konst; /* use const flag */
extern int symflag;

static char job_id[DESMAX+1];
int nqueues;

struct querec
{
   char name[NAMMAX+1];
   int width;
   int depth;
   char desc[DESMAX+1];
} queue[1+NQUEUES];

static void menu(void);
static void define(int);
static void zero(void);
static int getqueue(char *);

static int dirty;

void defqueue(void)
{
   int i, q, j, len, max;
   long n;
   long kn;
   char string[80];
   FILE *inf, *outf, *outh;

   kn = 0;
   dirty = 0;
   nqueues = 0;
   zero();
   menu();
loop:
   printf("\nQueue Command> ");
   gets(string);
   strupr(string);
   switch (string[0])
   {
      case 'E': /* edit queue */
editor:
         printf("\nQueue Editor Command> ");
         gets(string);
         strupr(string);
         switch(string[0])
         {
            case 'A': /* append mode */
               if (nqueues >= NQUEUES)
               {
                  printf("\tMaximum number of queues exceeded\n");
                  goto editor;
               }
               dirty++;
               nqueues++;
               blank(queue[nqueues].name);
               blank(queue[nqueues].desc);
               define(nqueues);
               break;

            case 'I': /* insert mode */
insert:
               i = getqueue("Before queue #> ");
               if (i == 0)
               {
                  printf("\tInsert aborted\n");
                  break;
               }

               if (i < 0 || i > nqueues)
               {
                  printf("\tInvalid queue number\n");
                  goto insert;
               }

               if (nqueues+1 > NQUEUES)
               {
                  printf("\tMaximum number of queues exceeded\n");
                  break;
               }
               dirty++;
               max = nqueues+1;
               for (j = max; j >= i; j--)
                  memcpy(&queue[j],&queue[j-1],sizeof(struct querec));
               blank(queue[i].name);
               blank(queue[i].desc);
               define(i);
               nqueues++;
               break;

            case 'D': /* delete mode */
delete:
               i = getqueue("Delete queue #> ");
               if (i == 0)
               {
                  printf("\tDelete aborted\n");
                  break;
               }

               if (i < 0 || i > nqueues)
               {
                  printf("\tInvalid queue number\n");
                  goto delete;
               }

               dirty++;
               for (j = i; j <= nqueues; j++)
                  memcpy(&queue[j],&queue[j+1],sizeof(struct querec));
               nqueues--;
               break;

            case 'C': /* change mode */
change:
               i = getqueue("Change queue #> " );
               if (i == 0)
               {
                  printf("\tChange aborted\n");
                  break;
               }

               if (i < 0 || i > nqueues)
               {
                  printf("\tInvalid queue number\n");
                  goto change;
               }

               printf("  #   Name   Width Depth Description\n");
                 /*    123 12345678 12345 12345 1234567890... */
               printf("%3d %8s %5d %5d %20s\n",
                  i,&queue[i].name[0],queue[i].width,
                  queue[i].depth,&queue[i].desc[0]);
               define(i);
               break;

            case 'M': /* move mode */
move:
               q = getnum("Move queue #> ");
               if (q == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

               i = getnum("-before queue #> ");
               if (i == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

               if (q < 0 || q > nqueues || i < 0 || i > nqueues)
               {
                  printf("\tInvalid queue number\n");
                  goto move;
               }

               dirty++;
               memcpy(&queue[0],&queue[q],sizeof(struct querec));
               for (j = q; j < nqueues; j++)
                  memcpy(&queue[j],&queue[j+1],sizeof(struct querec));
               max = nqueues;
               if (q > i)
               {
                  for (j = max; j > i; j--)
                     memcpy(&queue[j],&queue[j-1],sizeof(struct querec));
                  memcpy(&queue[i],&queue[0],sizeof(struct querec));
               }
               else
               {
                  for (j = max; j >= i; j--)
                     memcpy(&queue[j],&queue[j-1],sizeof(struct querec));
                  memcpy(&queue[i-1],&queue[0],sizeof(struct querec));
               }
               blank(&queue[0].name[0]);
               break;

            case 'S': /* swap mode */
swap:
               q = getqueue("Swap queue #> ");
               if (q == 0)
               {
                  printf("\tSwap aborted\n");
                  break;
               }

               i = getnum("-with queue #> ");
               if (i == 0)
               {
                  printf("\tSwap aborted\n");
                  break;
               }

               if (q < 0 || q > nqueues || i < 0 || i > nqueues)
               {
                  printf("\tInvalid queue number\n");
                  goto swap;
               }

               dirty++;
               memcpy(&queue[0],&queue[q],sizeof(struct querec));
               memcpy(&queue[q],&queue[i],sizeof(struct querec));
               memcpy(&queue[i],&queue[0],sizeof(struct querec));
               blank(&queue[0].name[0]);
               break;

            case 'V': /* view mode */
               printf("%20s %14s\n",job_id,timestamp);
               printf("  #    Name   Width Depth Description\n");
                    /* 123 12345678 12345 12345 1234567890... */

               if (romqh)
                  kn = SIZEOF_KQH;
               for (n = SIZEOF_QH, i = 1, j = 1; i <= nqueues;
                    n += SIZEOF_QH, i++, j++)
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
                  printf("%3d %8s %5d %5d %20s\n",
                      i,&queue[i].name[0],queue[i].width,
                      queue[i].depth,&queue[i].desc[0]);
                  n += (long)queue[i].width * (long)queue[i].depth;
                  if (romqh)
                     kn += SIZEOF_KQH;
               }
               printf("Total number of RAM bytes required = %ld\n",n);
               if (romqh)
                  printf("Total number of ROM bytes required = %ld\n",kn);
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
         if ((inf = ixfopen("cqueue.def","r")) == NULL)
         {
            printf("\tSorry, can't open cqueue.def\n");
            break;
         }
         dirty = 0;
         fscanf(inf,"%20c\n",&job_id[0]);
         fscanf(inf,"%14c\n",&timestamp[0]);
         fscanf(inf,"%d",&nqueues);
         for (i = 1; i <= nqueues; i++)
            fscanf(inf,"%3d%8c%6d%6d%20c\n",
                &q,&queue[i].name[0],&queue[i].width,
                &queue[i].depth,&queue[i].desc[0]);
         fclose(inf); /* fall through into display */

      case 'V': /* view current defs */
         printf("%20s %14s\n",job_id,timestamp);
         printf("  #   Name   Width Depth Description\n");
             /*  123 12345678 12345 12345 1234567890... */

         if (romqh)
            kn = SIZEOF_KQH;
         for (n = SIZEOF_QH, i = 1, j = 1; i <= nqueues;
              n += SIZEOF_KQH, i++, j++)
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
            printf("%3d %8s %5d %5d %20s\n",
               i,&queue[i].name[0],queue[i].width,
               queue[i].depth,&queue[i].desc[0]);
            n += (long)queue[i].width * (long)queue[i].depth;
            if (romqh)
               kn += SIZEOF_KQH;
         }
         printf("Total number of RAM bytes required = %ld\n",n);
         if (romqh)
            printf("Total number of ROM bytes required = %ld\n",kn);
         break;

      case 'K': /* keep definition file */
         if (nqueues == 0)
            if ( !nodata("No queues are defined") )
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

         if ((outf = ixfopen("cqueue.def","w")) == NULL)
         {
            printf("\tSorry, can't create cqueue.def\n");
            break;
         }

         s_gettime();
         fprintf(outf,"%20s\n",job_id);
         fprintf(outf,"%14s\n",timestamp);
         fprintf(outf,"%d\n",nqueues);
         for (i = 1; i <= nqueues; i++)
            fprintf(outf,"%3d%8s%6d%6d%20s\n",
                i,&queue[i].name[0],queue[i].width,
                queue[i].depth,&queue[i].desc[0]);
         fclose(outf);
         break;

      case 'F': /* force generate (dirty) */
         dirty++;
         /* fall into 'G' case */

      case 'G': /* generate new C file  */
         if (nqueues == 0)
            if ( !nodata("No queues are defined") )
               break;

         if ((outf = oxfopen("cqueue.c","w")) == NULL)
         {
            printf("\tSorry, can't create cqueue.c\n");
            break;
         }
         s_gettime();
      fprintf(outf,"/* cqueue.c - RTXC Queue definitions - %20s %14s */\n\n",
                job_id,timestamp);
         finclude(outf,"copyrite.h");

         n_f(outf); /* near/far defines */

         fprintf(outf,"#include \"typedef.h\"\n");
         fprintf(outf,"#include \"rtxstruc.h\"\n\n");
         fprintf(outf,"#include \"cqueue.h\"\n\n");

         for (i = 1; i <= nqueues; i++)
         {
            if (queue[i].name[0] != ' ')
            {
               /* support for non-numeric width */
               if (queue[i].desc[0] != '-')
                  fprintf(outf,"#define Q%dWD %d /* %8s - %20s */\n",
                      i,queue[i].width,&queue[i].name[0],&queue[i].desc[0]);
               else
                  fprintf(outf,"#define Q%dWD (%s) /* %8s */\n",
                      i,&queue[i].desc[1],&queue[i].name[0]);

               fprintf(outf,"#define Q%dDP %d\n\n",i,queue[i].depth);
            }
         }
         if (konst)
            fprintf(outf,"extern const ");
         fprintf(outf,"QUEUE near nqueues = NQUEUES;\n\n");
         for (i = 1; i <= nqueues; i++)
         {
            if ( (queue[i].name[0] == ' ') || (queue[i].width == 0) )
               continue;
            fprintf(outf,"static char far q%d[Q%dDP][Q%dWD];\n",i,i,i);
         }
         fprintf(outf,"\n");

         if (romqh)
         {
            fprintf(outf,"QHEADER near qheader[1+NQUEUES];\n\n");

            if (konst)
               fprintf(outf,"extern const ");
            fprintf(outf,"QKHEADER near qkheader[1+NQUEUES] =\n{\n");

            for (i = 0; i <= nqueues-1; i++)
            {
               if (queue[i].name[0] == ' ')
                  fprintf(outf,
                     "   { (char *)0, (size_t)0, 0 }, /* not used */\n");
               else if (queue[i].width == 0)
                  fprintf(outf,
                     "   { (char *)0, (size_t)Q%dWD, Q%dDP },\n",i,i);
               else
                  fprintf(outf,
                     "   { &q%d[0][0], (size_t)Q%dWD, Q%dDP },\n",i,i,i);
            }
            if (queue[i].name[0] == ' ')
               fprintf(outf,
                  "   { (char *)0, (size_t)0, 0 }  /* not used */\n");
            else if (queue[i].width == 0)
               fprintf(outf,
                  "   { (char *)0, (size_t)Q%dWD, Q%dDP }\n",i,i);
            else
               fprintf(outf,
                  "   { &q%d[0][0], (size_t)Q%dWD, Q%dDP }\n",i,i,i);
            fprintf(outf,"};\n\n");

            goto skiprom;
         }

         fprintf(outf,"QHEADER qheader[1+NQUEUES] =\n{\n");
         for (i = 0; i <= nqueues-1; i++)
         {
            if (queue[i].name[0] != ' ')
               fprintf(outf,
"   { (char *)0, (size_t)0, 0, 0, 0, SEMA_PENDING, SEMA_PENDING }, /* not used */\n");
            else if (queue[i].width == 0)
               fprintf(outf,
"   { (char *)0, (size_t)Q%dWD, Q%dDP, Q%dDP-1, 0, SEMA_PENDING, SEMA_PENDING },\n",
                       i,i,i);
            else
               fprintf(outf,
"   { &q%d[0][0], (size_t)Q%dWD, Q%dDP, Q%dDP-1, 0, SEMA_PENDING, SEMA_PENDING },\n",
                       i,i,i,i);
         }
         if (queue[i].name[0] == ' ')
            fprintf(outf,
"   { (char *)0, (size_t)0, 0, 0, 0, SEMA_PENDING, SEMA_PENDING }  /* not used */\n");
         else if (queue[i].width == 0)
            fprintf(outf,
"   { (char *)0, (size_t)Q%dWD, Q%dDP, Q%dDP-1, 0, SEMA_PENDING, SEMA_PENDING }\n",
                    i,i,i);
         else
            fprintf(outf,
"   { &q%d[0][0], (size_t)Q%dWD, Q%dDP, Q%dDP-1, 0, SEMA_PENDING, SEMA_PENDING }\n",
                    i,i,i,i);

         fprintf(outf,"};\n\n");
skiprom:

         if (symflag) /* RTXCbug symbols enabled */
         {
            fprintf(outf,"#define NAMMAX %d\n\n",NAMMAX);
            if (konst)
               fprintf(outf,"extern const ");
            fprintf(outf,"char queuename[1+NQUEUES][NAMMAX+1] =\n");
            fprintf(outf,"{\n");
#if 0 /* names padded to NAMMAX chars vs mixed lengths */
            for (i = 0; i < nqueues; i++)
               fprintf(outf,"   \"%8s\",\n",&queue[i].name[0]);
            fprintf(outf,"   \"%8s\"\n",&queue[i].name[0]);
#else
            for (i = 0; i <= nqueues; i++)
            {
               fprintf(outf,"   \"");
               for (j = 0; j < NAMMAX; j++)
               {
                  if (queue[i].name[j] == ' ')
                     break;
                  fprintf(outf,"%c",queue[i].name[j]);
               }

               if (i < nqueues) /* add comma on all but last */
                  fprintf(outf,"\",\n");
               else
                  fprintf(outf,"\"\n");
            }
#endif
            fprintf(outf,"};\n");
         }
         fprintf(outf,"\n/* End of file - cqueue.c */\n");
         fclose(outf);

         if (!dirty)
         {
            printf("\tNew cqueue.h not generated\n");
            break;
         }

         if ((outh = oxfopen("cqueue.h","w")) == NULL)
         {
            printf("\tSorry, can't create cqueue.h\n");
            break;
         }
         fprintf(outh,"/* cqueue.h - RTXC Queue include file */\n\n");
         finclude(outh,"copyrite.h");
         fprintf(outh,"#define NQUEUES %d\n\n",nqueues);
         for (i = 1; i <= nqueues; i++)
         {
            if (queue[i].name[0] != ' ')
               fprintf(outh,"#define %8s %d /* %20s */\n",
                   &queue[i].name[0],i,&queue[i].desc[0]);
         }
         fprintf(outf,"\n/* End of file - cqueue.h */\n");

         dirty = 0;
         fclose(outh);
         break;

#if 0
      case 'P': /* print current defs */
         fprintf(stdprn," RTXC Queue definitions - %20s %14s\n\n",
            job_id,timestamp);
         fprintf(stdprn,"  #   Name   Width Depth Description\n");
                    /*   123 12345678 12345 12345 1234567890... */

         if (romqh)
            kn = SIZEOF_KQH;
         for (n = SIZEOF_QH, i = 1; i <= nqueues; n += SIZEOF_QH, i++)
         {
            fprintf(stdprn,"%3d %8s %5d %5d %20s\n",
               i,&queue[i].name[0],queue[i].width,
               queue[i].depth,&queue[i].desc[0]);
            n += (long)queue[i].width * (long)queue[i].depth;
            if (romqh)
               kn += SIZEOF_KQH;
         }
         fprintf(stdprn,"Total number of RAM bytes required = %ld\n",n);
         if (romqh)
            fprintf(stdprn,"Total number of ROM bytes required = %ld\n",kn);
         fprintf(stdprn,"\f");
         break;
#endif

      case 'H':
         menu();
         break;

      case 'X': /* exit */
         return;

      default:
         printf("\t\aInvalid command\n");
         break;
   }
   goto loop;
}

static void menu(void)
{
   printf("\t *** Queue definition module ***\n\n");
   printf("\tL. Load existing definition file\n");
   printf("\tE. Edit individual queue parameters\n");
   printf("\tV. View current queue definitions\n");
   printf("\tK. Keep new definition file\n");
   printf("\tG. Generate new C source file\n");
   printf("\tF. Force .H file generation\n");
#if 0
   printf("\tP. Print current queue definition\n");
#endif
   printf("\tH. Help\n");
   printf("\tX. Exit module\n");
}

static void zero(void)
{
   int i, j;

   for (i = 0; i <= NQUEUES; i++)
   {
      queue[i].width = 0;
      queue[i].depth = 0;
      for (j = 0; j < NAMMAX; j++)
         queue[i].name[j] = ' ';
      for (j = 0; j < DESMAX; j++)
         queue[i].desc[j] = ' ';
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
      blank(queue[q].name);
      if (len > NAMMAX)
         len = NAMMAX;
      for (i = 1; i <= len; i++)
         if (string[i] == ' ')
         {
            printf("\tSpaces not permitted in name\n");
            goto name;
         }
      for (i = 0; i < len; i++)
         queue[q].name[i] = string[i];
      if (queue[q].name[0] == ' ')
         for (i = 0; i < NAMMAX; i++)
            queue[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (queue[q].name[0] == ' ')
      {
           printf("\tName required\n");
           goto name;
      }
   }

   tmp = getnum("Width (in bytes, -1==0 )> ");
   if (tmp != 0)
      queue[q].width = tmp;
   if (queue[q].width <= 0) /* allow for -1 as reset to 0 */
      queue[q].width = 0;

   tmp = getnum("Depth> ");
   if (tmp != 0)
      queue[q].depth = tmp;
   if (queue[q].depth <= 0) /* force negative depth to 1 */
      queue[q].depth = 1;

   printf("Description (up to %d char)> ",DESMAX);
   gets(string);
   len = strlen(string);
   if (len != 0)
   {
      blank(queue[q].desc);
      if (len > DESMAX)
         len = DESMAX;
      for (i = 0; i < len; i++)
         queue[q].desc[i] = string[i];
      dirty++;
   }
}

/* numeric or symbolic queue references */
static int getqueue(char *p)
{
   int iqueue;
   char string[12]; /* 12345678\0 */

   printf("%s",p);
   gets(string);
   strupr(string);

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      iqueue = atoi(string);
      if ( (iqueue < 0) || (iqueue > nqueues) )
         iqueue = 0;
      return(iqueue);
   }
   else
   {
      for (iqueue = 1; iqueue <= nqueues; iqueue++)
      {
         if (!strcmp(string,&queue[iqueue].name[0]))
            return(iqueue);
      }
      return(0); /* not found */
   }
}

/* End of file - defqueue.c */