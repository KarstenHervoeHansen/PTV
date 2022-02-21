/* defpart.c - partition definition program for RTXCgen */

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
int romph = ROMPH; /* ROMable partition header */

#define NPARTS 64 /* max # of partitions supported, arbitrary */
#define DESMAX 20 /* max # of chars in description */
#define NAMMAX 8  /* max # of chars in partition name */
#define MINSIZE sizeof(char *) /* minimum partition size */

extern void finclude(FILE *,char *);
extern int getnum(char *);
extern long getlnum(char *);
extern void edit_menu(void);
extern void blank(char *);
extern void s_gettime(void);
extern int nodata(char *);
extern void n_f(FILE *);
extern FILE *ixfopen(const char *,const char *);
extern FILE *oxfopen(const char *,const char *);

extern char timestamp[];
extern int konst;
extern int symflag;

int nparts;
#ifdef DYNAMIC_PARTS
int dnparts; /* # of dynamic partitions */
#endif

static char job_id[DESMAX+1];

struct partrec
{
   char name[NAMMAX+1];
   long size; /* block size (typically size_t) */
   int count; /* block count */
   char desc[DESMAX+1];
} mem[1+NPARTS];

static void menu(void);
static void define(int);
static void zero(void);
static int getpart(char *);

static int dirty;

void defpart(void)
{
   int i, q, j, len, max;
   long n;
   long kn;
   char string[80];
   FILE *inf, *outf, *outh;
#ifdef DYNAMIC_PARTS
    int tmp;
#endif

   kn = 0;
   dirty = 0;
   nparts = 0;
#ifdef DYNAMIC_PARTS
    dnparts = 0;
#endif

   zero();
   menu();

loop:
   printf("\nPartition Command> ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'E': /* edit partition */
editor:
         printf("\nPartition Editor Command> ");
         gets(string);
         strupr(string);
         switch(string[0])
         {
            case 'A': /* append mode */
#ifdef DYNAMIC_PARTS
               if ((nparts+dnparts) >= NPARTS)
#else
               if (nparts >= NPARTS)
#endif
               {
                  printf("\tMaximum number of partitions exceeded\n");
                  goto editor;
               }
               dirty++;
               nparts++;
               blank(mem[nparts].name);
               blank(mem[nparts].desc);
               mem[nparts].count = 1;
               mem[nparts].size = MINSIZE;
               define(nparts);
               break;

            case 'I': /* insert mode */
insert:
               i = getpart("Before partition #> ");
               if (i == 0)
               {
                  printf("\tInsert aborted\n");
                  break;
               }
               if (i < 0 || i > nparts)
               {
                  printf("\tInvalid partition number\n");
                  goto insert;
               }

#ifdef DYNAMIC_PARTS
               if ((nparts+dnparts+1) > NPARTS)
#else
               if (nparts+1 > NPARTS)
#endif
               {
                  printf("\tMaximum number of partitions exceeded\n");
                  break;
               }
               dirty++;
               max = nparts+1;
               for (j = max; j >= i; j--)
                  memcpy(&mem[j],&mem[j-1],sizeof(struct partrec));
               blank(mem[i].name);
               blank(mem[i].desc);
               mem[i].count = 1;
               mem[i].size = MINSIZE;
               define(i);
               ++nparts;
               break;

            case 'D': /* delete mode */
delete:
               i = getpart("Delete partition #> ");
               if (i == 0)
               {
                  printf("\tDelete aborted\n");
                  break;
               }

               if (i < 0 || i > nparts)
               {
                  printf("\tInvalid partition number\n");
                  goto delete;
               }

               dirty++;
               for (j = i; j <= nparts; j++)
                  memcpy(&mem[j],&mem[j+1],sizeof(struct partrec));
               nparts--;
               break;

            case 'C': /* change mode */
change:
               i = getpart("Change partition #> " );
               if (i == 0)
               {
                  printf("\tChange aborted\n");
                  break;
               }

               if (i < 0 || i > nparts)
               {
                  printf("\tInvalid partition number\n");
                  goto change;
               }

               printf("  #   Name     Size Count Description\n");
                   /*  123 12345678 123456 12345 1234567890... */
               printf("%3d %8s %6ld %5d %20s\n",
                  i,&mem[i].name[0],mem[i].size,
                  mem[i].count,&mem[i].desc[0]);
               define(i);
               break;

            case 'M': /* move mode */
move:
               q = getpart("Move partition #> ");
               if (q == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

               i = getpart("-before partition #> ");
               if (i == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

               if (q < 0 || q > nparts || i < 0 || i > nparts)
               {
                  printf("\tInvalid partition number\n");
                  goto move;
               }

               dirty++;
               memcpy(&mem[0],&mem[q],sizeof(struct partrec));
               for (j = q; j < nparts; j++)
                  memcpy(&mem[j],&mem[j+1],sizeof(struct partrec));
               max = nparts;
               if (q > i)
               {
                  for (j = max; j > i; j--)
                     memcpy(&mem[j],&mem[j-1],sizeof(struct partrec));
                  memcpy(&mem[i],&mem[0],sizeof(struct partrec));
               }
               else
               {
                  for (j = max; j >= i; j--)
                     memcpy(&mem[j],&mem[j-1],sizeof(struct partrec));
                  memcpy(&mem[i-1],&mem[0],sizeof(struct partrec));
               }
               blank(&mem[0].name[0]);
               break;

            case 'S': /* swap mode */
swap:
               q = getpart("Swap partition #> ");
               if (q == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

               i = getpart("-with partition #> ");
               if (i == 0)
               {
                  printf("\tMove aborted\n");
                  break;
               }

               if (q < 0 || q > nparts || i < 0 || i > nparts)
               {
                  printf("\tInvalid partition number\n");
                  goto swap;
               }
               dirty++;
               memcpy(&mem[0],&mem[q],sizeof(struct partrec));
               memcpy(&mem[q],&mem[i],sizeof(struct partrec));
               memcpy(&mem[i],&mem[0],sizeof(struct partrec));
               blank(&mem[0].name[0]);
               break;

            case 'V': /* view mode */
               printf("%20s %14s\n",job_id,timestamp);
#ifdef DYNAMIC_PARTS
                    printf("\tNumber of dynamic partitions is %d\n",dnparts);
#endif
               printf("  #   Name     Size Count Description\n");
               /*      123 12345678 123456 12345 1234567890... */

               if (romph) /* count ROM memory */
                  kn = SIZEOF_KPH;
               for (n = SIZEOF_PH, i = 1, j = 1; i <= nparts;
                    n += SIZEOF_PH, i++, j++)
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
                  printf("%3d %8s %6ld %5d %20s\n",
                     i,&mem[i].name[0],mem[i].size,
                     mem[i].count,&mem[i].desc[0]);
                  n += mem[i].size * (long)mem[i].count;
                  if (romph)
                     kn += SIZEOF_KPH;
               }
#ifdef DYNAMIC_PARTS
                     n += dnparts * SIZEOF_PH;
#endif
               printf("Total number of RAM bytes required = %ld\n",n);
               if (romph)
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
         if ((inf = ixfopen("cpart.def","r")) == NULL)
         {
            printf("\tSorry, can't open cpart.def\n");
            break;
         }
         dirty = 0;
         fscanf(inf,"%20c\n", &job_id[0]);
         fscanf(inf,"%14c\n", &timestamp[0]);
         fscanf(inf,"%d", &nparts);
         for (i = 1; i <= nparts; i++)
            fscanf(inf,"%3d%8c%6ld%5d%20c\n",
               &q,&mem[i].name[0],&mem[i].size,
               &mem[i].count,&mem[i].desc[0]);

#ifdef DYNAMIC_PARTS
            fscanf(inf,"%d",&dnparts);
#endif
         fclose(inf);  /* fall thru into view code */

      case 'V': /* view current defs */
         printf("%20s %14s\n",job_id,timestamp);
#ifdef DYNAMIC_PARTS
            printf("\tNumber of dynamic partitions is %d\n",dnparts);
#endif
         printf("  #   Name     Size Count Description\n");
            /*   123 12345678 123456 12345 1234567890... */

         if (romph)
            kn = SIZEOF_KPH;
         for (n = SIZEOF_PH, i = 1, j = 1; i <= nparts;
              n += SIZEOF_PH, i++, j++)
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
            printf("%3d %8s %6ld %5d %20s\n",
               i,&mem[i].name[0],mem[i].size,
               mem[i].count,&mem[i].desc[0]);
            n += mem[i].size * (long)mem[i].count;
            if (romph)
               kn += SIZEOF_KPH;
         }
#ifdef DYNAMIC_PARTS
            n += dnparts * SIZEOF_PH;
#endif
         printf("Total number of RAM bytes required = %ld\n",n);
         if (romph)
            printf("Total number of ROM bytes required = %ld\n",kn);
         break;

      case 'K': /* keep definition file */
#ifdef DYNAMIC_PARTS
         if ((nparts + dnparts) == 0)
#else
         if (nparts == 0)
#endif
            if ( !nodata("No partitions are defined") )
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

#ifdef DYNAMIC_PARTS
re_dnpart:
            tmp = getnum("# of dynamic partitions (-1 == 0) > ");
            if (tmp == -1) /* allow zero after non-zero entry */
            {
               dnparts = 0;
               dirty++;
               goto create;
            }
            if (tmp == 0)
               goto create;
            if ((nparts + tmp) > NPARTS) /* partition count too large */
            {
               printf("\tToo many partitions, Max # partitions is %d\n",
                                                                    NPARTS);
               goto re_dnpart;
            }
            dnparts = tmp;
            dirty++;
create:
#endif
         if ((outf = ixfopen("cpart.def","w")) == NULL)
         {
            printf("\tSorry, can't create cpart.def\n");
            break;
         }

         s_gettime();
         fprintf(outf,"%20s\n",job_id);
         fprintf(outf,"%14s\n",timestamp);
         fprintf(outf,"%d\n",nparts);
         for (i = 1; i <= nparts; i++)
            fprintf(outf,"%3d%8s%6ld%5d%20s\n",
               i,&mem[i].name[0],mem[i].size,
               mem[i].count,&mem[i].desc[0]);
#ifdef DYNAMIC_PARTS
            fprintf(outf,"%d\n",dnparts);
#endif
         fclose(outf);
         break;

      case 'F': /* dirty */
         dirty++;

      case 'G': /* generate new C file */
#ifdef DYNAMIC_PARTS
         if ((nparts+dnparts) == 0)
#else
         if (nparts == 0)
#endif
            if ( !nodata("No partitions are defined") )
               break;

         if ((outf = oxfopen("cpart.c","w")) == NULL)
         {
            printf("\tSorry, can't create cpart.c\n");
            break;
         }

         s_gettime();
         fprintf(outf,
            "/* cpart.c - RTXC Partition definitions - %20s %14s */\n\n",
            job_id,timestamp);
         finclude(outf,"copyrite.h");

         n_f(outf); /* near/far defines */

         fprintf(outf,"#include \"typedef.h\"\n");
         fprintf(outf,"#include \"rtxstruc.h\"\n\n");
         fprintf(outf,"#include \"cpart.h\"\n\n");

         for (i = 1; i <= nparts; i++)
         {
            if (mem[i].name[0] != ' ')
            {
               /* support for non-numeric sizes */
               if (mem[i].desc[0] != '-')
                  fprintf(outf,"#define M%dSZ %ld /* %8s - %20s */\n",
                     i,mem[i].size,&mem[i].name[0],&mem[i].desc[0]);
               else
                  fprintf(outf,"#define M%dSZ (%s) /* %8s */\n",
                     i,&mem[i].desc[1],&mem[i].name[0]);

               fprintf(outf,"#define M%dCT %d\n\n",i,mem[i].count);
            }
         }

         if (konst)
            fprintf(outf,"extern const ");
         fprintf(outf,"MAP near nparts = NPARTS;\n\n");
#ifdef DYNAMIC_PARTS
         if (konst)
            fprintf(outf,"extern const ");
         fprintf(outf,"MAP near dnparts = DNPARTS;\n\n");
#endif

         for (i = 1; i <= nparts; i++)
         {
            if (mem[i].name[0] != ' ')
#ifndef HUGE_PARTITIONS
               fprintf(outf,"static char far part%d[M%dCT*M%dSZ];\n",i,i,i);
#else
               if (mem[i].desc[0] != '-')
                  fprintf(outf,"static char huge part%d[%ldL];\n",
                     i, mem[i].size * (long)mem[i].count);
               else
                  fprintf(outf,"static char huge part%d[%ldL*M%dSZ];\n",
                     i, (long)mem[i].count, i);
#endif
         }
         fprintf(outf,"\n");

         if (romph)
         {
#ifdef DYNAMIC_PARTS
            fprintf(outf,"PHEADER near pheader[1+NPARTS+DNPARTS];\n\n");
#else
            fprintf(outf,"PHEADER near pheader[1+NPARTS];\n\n");
#endif
            if (konst)
               fprintf(outf,"extern const ");
            fprintf(outf,"PKHEADER near pkheader[1+NPARTS] = \n");
         }
         else
#ifdef DYNAMIC_PARTS
            fprintf(outf,"PHEADER near pheader[1+NPARTS+DNPARTS] = \n");
#else
            fprintf(outf,"PHEADER near pheader[1+NPARTS] = \n");
#endif

         fprintf(outf,"{\n");
         for (i = 0; i < nparts; i++)
         {
            if (mem[i].name[0] != ' ')
               fprintf(outf,
                  "   { (struct xmap *)&part%d[0], (size_t)M%dSZ, M%dCT }",
                  i,i,i);
            else
               fprintf(outf,"   { (struct xmap *)0, (size_t)0, 0 }");
            fprintf(outf,",\n");
         }
         if (mem[i].name[0] != ' ')
            fprintf(outf,
               "   { (struct xmap *)&part%d[0], (size_t)M%dSZ, M%dCT }\n",
               i,i,i);
         else
            fprintf(outf,"   { (struct xmap *)0, (size_t)0, 0 }\n");
         fprintf(outf,"};\n\n");

         if (symflag) /* RTXCbug symbols enabled */
         {
            fprintf(outf,"#define NAMMAX %d\n\n",NAMMAX);
            if (konst)
               fprintf(outf,"extern const ");
            fprintf(outf,"char partname[1+NPARTS][NAMMAX+1] =\n");
            fprintf(outf,"{\n");

#if 0 /* names padded to NAMMAX chars vs mixed lengths */
            for (i = 0; i < nparts; i++)
               fprintf(outf,"   \"%8s\",\n",&mem[i].name[0]);
            fprintf(outf,"   \"%8s\"\n",&mem[i].name[0]);
#else
            for (i = 0; i <= nparts; i++)
            {
               fprintf(outf,"   \"");
               for (j = 0; j < NAMMAX; j++)
               {
                  if (mem[i].name[j] == ' ')
                     break;
                  fprintf(outf,"%c",mem[i].name[j]);
               }

               if (i < nparts) /* add comma on all but last */
                  fprintf(outf,"\",\n");
               else
                  fprintf(outf,"\"\n");
            }
#endif
            fprintf(outf,"};\n");
         }
         fprintf(outf,"\n/* End of file - cpart.c */\n");
         fclose(outf);

         if (!dirty)
         {
             printf("\tNew cpart.h not generated\n");
             break;
         }

         if ((outh = oxfopen("cpart.h","w")) == NULL)
         {
            printf("\tSorry, can't find cpart.h\n");
            break;
         }
         fprintf(outh,"/* cpart.h - RTXC Partition include file */\n\n");
         finclude(outh,"copyrite.h");
         fprintf(outh,"#define NPARTS %d\n\n",nparts);
#ifdef DYNAMIC_PARTS
            fprintf(outh,"#define DNPARTS %d\n\n",dnparts);
#endif
         for (i = 1; i <= nparts; i++)
         {
            if (mem[i].name[0] != ' ')
               fprintf(outh,"#define %8s %d /* %20s */\n",
                    mem[i].name,i,mem[i].desc);
         }
         fprintf(outf,"\n/* End of file - cpart.h */\n");

         dirty = 0;
         fclose(outh);
         break;

#if 0
      case 'P': /* print current defs */
         fprintf(stdprn," RTXC Partition definitions - %20s %14s\n\n",
            job_id,timestamp);
#ifdef DYNAMIC_PARTS
            fprintf(stdprn,"\tNumber of dynamic partitions is %d\n",dnparts);
#endif
         fprintf(stdprn,"  #   Name     Size Count Description\n");
                      /* 123 12345678 123456 12345 1234567890... */

         if (romph)
            kn = SIZEOF_KPH;
         for (n = SIZEOF_PH, i = 1; i <= nparts; n += SIZEOF_PH, i++)
         {
            fprintf(stdprn,"%3d %8s %6ld %5d %20s\n",
               i,&mem[i].name[0],mem[i].size,
               mem[i].count,&mem[i].desc[0]);
            n += mem[i].size * (long)mem[i].count;
            if (romph)
               kn += SIZEOF_KPH;
         }
#ifdef DYNAMIC_PARTS
            n += dnparts * SIZEOF_PH;
#endif
         fprintf(stdprn,"Total number of RAM bytes required = %ld\n",n);
         if (romph)
            fprintf(stdprn,"Total number of ROM bytes required = %ld\n",kn);
         fprintf(stdprn,"\f");
         break;
#endif

      case 'H':
         menu();
         break;

      case 'X': /* quit */
         return;

      default:
         printf("\t\aInvalid command\n");
         break;
   }
   goto loop;
}

static void menu(void)
{
   printf("\t *** Partition definition module ***\n\n");
   printf("\tL. Load existing definition file\n");
   printf("\tE. Edit individual partition parameters\n");
   printf("\tV. View current partition definitions\n");
   printf("\tK. Keep new definition file\n");
   printf("\tG. Generate new C source file\n");
   printf("\tF. Force .H file generation\n");
#if 0
   printf("\tP. Print current partition definitions\n");
#endif
   printf("\tH. Help\n");
   printf("\tX. Exit module\n");
}

static void zero(void)
{
   int i, j;

   for (i = 0; i <= NPARTS; i++)
   {
      mem[i].size = 0L;
      mem[i].count = 0;
      for (j = 0; j < NAMMAX; j++)
         mem[i].name[j] = ' ';
      for (j = 0; j < DESMAX; j++)
         mem[i].desc[j] = ' ';
   }
   for (j = 0; j < 14; j++)
      timestamp[j] = ' ';
   for (j = 0; j < DESMAX; j++)
      job_id[j] = ' ';
}

static void define(int q)
{
   int i, len, tmp;
   long ltmp;
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
      blank(mem[q].name);
      if (len > NAMMAX)
         len = NAMMAX;
      for (i = 1; i <= len; i++)
         if (string[i] == ' ')
         {
            printf("\tSpaces not permitted in name\n");
            goto name;
         }
      for (i = 0; i < len; i++)
         mem[q].name[i]= string[i];
      if (mem[q].name[0] == ' ')
         for (i = 0; i < NAMMAX; i++)
            mem[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (mem[q].name[0] == ' ')
      {
           printf("\tName required\n");
           goto name;
      }
   }

   ltmp = getlnum("Size (in bytes)> ");
   if (ltmp != 0)
      mem[q].size = ltmp;
   if (mem[q].size < MINSIZE)
      mem[q].size = MINSIZE;

   tmp = getnum("Count> ");
   if (tmp != 0)
      mem[q].count = tmp;
   if (mem[q].count < 0) /* allow for 1 count as -1 */
      mem[q].count = 1;

   printf("Description (up to %d char)> ",DESMAX);
   gets(string);
   len = strlen(string);
   if (len != 0)
   {
      blank(mem[q].desc);
      if (len > DESMAX)
         len = DESMAX;
      for (i = 0; i < len; i++)
         mem[q].desc[i] = string[i];
      dirty++;
   }
}

/* numeric or symbolic memory references (symbolic 8 char match) */
static int getpart(char *p)
{
   int part;
   char string[12]; /* 12345678\0 */

   printf("%s",p);
   gets(string);
   strupr(string);

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      part = atoi(string);
      if ( (part < 0) || (part > nparts) )
         part = 0;
      return(part);
   }
   else
   {
      for (part = 1; part <= nparts; part++)
      {
         if (!strcmp(string,&mem[part].name[0]))
            return(part);
      }
      return(0); /* not found */
   }
}

/* End of file - defpart.c */