/* deftask.c - task definition program for RTXCgen */

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
int romtcb = ROMTCB; /* indicates romable tcb */

int taskstksz = TASKSTKSZ; /* default stack size for tasks */

#define NTASKS 126 /* max # of tasks supported, limited by char */
/* if TASK is an int/short then max # tasks is only limited by reality */

#define NAMMAX 8  /* max # of chars in task name */
#define DESMAX 20  /* max # of chars in description */

#undef VARNAMLEN  /* support for name space expansion via recompilation */
   /* BUT scanf does not support %*c as an input format -  change %20c/%8c*/
static int namlen = NAMMAX;
static int deslen = DESMAX;

extern char timestamp[];
extern int konst; /* use const flag */
extern int symflag;

int ntasks;
#ifdef DYNAMIC_TASKS
int dntasks; /* # of dynamic tasks */
#endif

static char job_id[DESMAX+1];

extern void blank(char *);
extern void finclude(FILE *,char *);
extern void edit_menu(void);
extern int nodata(char *);
extern void s_gettime(void);
extern int getnum(char *);
extern void n_f(FILE *);
extern FILE *ixfopen(const char *,const char *);
extern FILE *oxfopen(const char *,const char *);
extern void RTXCstrupr(char *);

struct taskrec
{
    char name[NAMMAX+1];
    char entry[NAMMAX+1];
    char desc[DESMAX+1];
    char fpumode[2];
    int stksiz;
    int order;
    int priority;
} task[1+NTASKS];

static void menu(void);
static void zero(void);
static void define(int);
static int gettask(char *);

static int dirty;

void deftask(void)
{
    int j, i, q, len, max;
    char string[80];
    FILE *inf, *outf, *outh;
    long n, kn;
#ifdef DYNAMIC_TASKS
    int tmp;
#endif

    dirty = 0;
    ntasks = 0;
#ifdef DYNAMIC_TASKS
    dntasks = 0;
#endif

    zero();
    menu();
loop:
    printf("\nTask Command> ");
    gets(string);
    strupr(string);
    switch (string[0])
    {
        case 'E': /* edit task */
editor:
           printf("\nTask Editor Command> ");
           gets(string);
           strupr(string);
           switch(string[0])
           {
               case 'A': /* append mode */
#ifdef DYNAMIC_TASKS
                   if ((ntasks+dntasks) >= NTASKS)
#else
                   if (ntasks >= NTASKS)
#endif
                   {
                       printf("\tMaximum number of tasks exceeded\n");
                       goto editor;
                   }
                   dirty++;
                   ntasks++;
                   blank(task[ntasks].name);
                   blank(task[ntasks].entry);
                   blank(task[ntasks].desc);
		   task[ntasks].priority = ntasks;
                   task[ntasks].order = -99;
                   task[ntasks].fpumode[0] = 'N';
                   define(ntasks);
                   break;

               case 'I': /* insert mode */
insert:
                   i = gettask("Before task #> ");
                   if (i == 0)
                   {
                       printf("\tInsert aborted\n");
                       break;
                   }

                   if (i < 0 || i > ntasks)
                   {
                       printf("\tInvalid task number\n");
                       goto insert;
                   }

#ifdef DYNAMIC_TASKS
                   if ((ntasks+dntasks+1) > NTASKS)
#else
                   if (ntasks+1 > NTASKS)
#endif
                   {
                       printf("\tMaximum number of tasks exceeded\n");
                       break;
                   }

                   dirty++;
                   max = ntasks+1;
                   for (j = max; j >= i; j--)
                        memcpy(&task[j],&task[j-1],sizeof(struct taskrec));
                   blank(task[i].name);
                   blank(task[i].entry);
                   blank(task[i].desc);
                   task[i].priority = ntasks;
                   task[i].order = -99;
                   task[i].fpumode[0] = 'N';
                   define(i);
                   ++ntasks;
                   break;

               case 'D': /* delete mode */
delete:
                   i = gettask("Delete task #> ");
                   if (i == 0)
                   {
                       printf("\tDelete aborted\n");
                       break;
                   }

                   if (i < 0 || i > ntasks)
                   {
                       printf("\tInvalid task number\n");
                       goto delete;
                   }

                   dirty++;
                   for (j = i; j <= ntasks; j++)
                       memcpy(&task[j],&task[j+1],sizeof(struct taskrec));
                   ntasks--;
                   break;

                case 'C': /* change mode */
change:
                    i = gettask("Change task #> " );
                    if (i == 0)
                    {
                        printf("\tChange aborted\n");
                        break;
                    }

                    if (i < 0 || i > ntasks)
                    {
                        printf("\tInvalid task number\n");
                        goto change;
                    }

                    printf(
           "  #   Name    Pri  Entry  Stack Start Description         FPU\n");
         /* 123 12345678 1234 12345678 1234  123  12345678901234567890 1 */
#ifdef VARNAMLEN
                    printf("%3d %*s %4d %*s %4d  %3d  %20s %1s\n",
                           i,namlen,&task[i].name[0],task[i].priority,
                           namlen,&task[i].entry[0],task[i].stksiz,
                           task[i].order,&task[i].desc[0],&task[i].fpumode[0]);
#else
                    printf("%3d %8s %4d %8s %4d  %3d  %20s %1s\n",
                           i,&task[i].name[0],task[i].priority,
                           &task[i].entry[0],task[i].stksiz,
                           task[i].order,&task[i].desc[0],&task[i].fpumode[0]);
#endif
                    define(i);
                    break;

                case 'M': /* move mode */
move:
                    q = gettask("Move task #> ");
                    if (q == 0)
                    {
                        printf("\tMove aborted\n");
                        break;
                    }

                    i = gettask("-before task #> ");
                    if (i == 0)
                    {
                        printf("\tMove aborted\n");
                        break;
                    }

                    if (q < 0 || q > ntasks || i < 0 || i > ntasks)
                    {
                        printf("\tInvalid task number\n");
                        goto move;
                    }

                    dirty++;
                    memcpy(&task[0],&task[q],sizeof(struct taskrec));
                    for (j = q; j < ntasks; j++)
                        memcpy(&task[j],&task[j+1],sizeof(struct taskrec));
                    max = ntasks;
                    if (q > i)
                    {
                        for (j = max; j > i; j--)
                            memcpy(&task[j],&task[j-1],sizeof(struct taskrec));
                        memcpy(&task[i],&task[0],sizeof(struct taskrec));
                    }
                    else
                    {
                        for (j = max; j >= i; j--)
                            memcpy(&task[j],&task[j-1],sizeof(struct taskrec));
                        memcpy(&task[i-1],&task[0],sizeof(struct taskrec));
                    }
                    blank(&task[0].name[0]);
                    break;

                case 'S': /* swap mode */
swap:
                    q = gettask("Swap task #> ");
                    if (q == 0)
                    {
                        printf("\tSwap aborted\n");
                        break;
                    }

                    i = gettask("-with task #> ");
                    if (i == 0)
                    {
                        printf("\tSwap aborted\n");
                        break;
                    }

                    if (q < 0 || q > ntasks || i < 0 || i > ntasks)
                    {
                        printf("\tInvalid task number\n");
                        goto swap;
                    }

                    dirty++;
                    memcpy(&task[0],&task[q],sizeof(struct taskrec));
                    memcpy(&task[q],&task[i],sizeof(struct taskrec));
                    memcpy(&task[i],&task[0],sizeof(struct taskrec));
                    blank(&task[0].name[0]);
                    break;

                case 'V': /* view mode */
                    printf("%20s %14s\n",job_id,timestamp);
                    printf("\tDefault task stack size is %d\n",taskstksz);
#ifdef DYNAMIC_TASKS
                    printf("\tNumber of dynamic tasks is %d\n",dntasks);
#endif
                    printf(
            "  #   Name    Pri  Entry  Stack Start Description         FPU\n");
          /* 123 12345678 1234 12345678 1234  123  12345678901234567890 1 */

                    if (romtcb)
                        kn = SIZEOF_KTCB;
                    for (n = SIZEOF_TCB, i = 1, j = 1; i <= ntasks;
                         n += SIZEOF_TCB, i++, j++)
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

#ifdef VARNAMLEN
                        printf("%3d %*s %4d %*s %4d  %3d  %20s %1s\n",
                               i,namlen,&task[i].name[0],task[i].priority,
                               namlen,&task[i].entry[0],
                               task[i].stksiz,task[i].order,&task[i].desc[0],
                               &task[i].fpumode[0]);
#else
                        printf("%3d %8s %4d %8s %4d  %3d  %20s %1s\n",
                               i,&task[i].name[0],task[i].priority,
                               &task[i].entry[0],
                               task[i].stksiz,task[i].order,&task[i].desc[0],
                               &task[i].fpumode[0]);
#endif
                        n += task[i].stksiz;
#ifdef FPU
                        if (task[i].fpumode[0] == 'Y')
                           n += SIZEOF_FPREGS;
#endif
                        if (romtcb)
                           kn += SIZEOF_KTCB;
                     }
#ifdef DYNAMIC_TASKS
                     n += dntasks * SIZEOF_TCB;
#endif
                     printf("Total number of RAM bytes required = %ld\n",n);
                     if (romtcb)
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
            if ((inf = ixfopen("ctask.def","r")) == NULL)
            {
                printf("\tSorry, can't open ctask.def\n");
                break;
            }
            dirty = 0;
            fscanf(inf,"%20c\n",&job_id[0]);
            fscanf(inf,"%14c\n",&timestamp[0]);
            fscanf(inf,"%d",&taskstksz);
            fscanf(inf,"%d",&ntasks);
            for (i = 1; i <= ntasks; i++)
#ifdef X_VARNAMLEN
                fscanf(inf,"%d%*c%*c%d%3d%20c%1c\n",
                       &task[i].priority,namlen,&task[i].name[0],
                       namlen,&task[i].entry[0],
                       &task[i].stksiz,
                       &task[i].order,&task[i].desc[0],&task[i].fpumode[0]);
#else
                fscanf(inf,"%d%8c%8c%d%3d%20c%1c\n",
                       &task[i].priority,&task[i].name[0],&task[i].entry[0],
                       &task[i].stksiz,
                       &task[i].order,&task[i].desc[0],&task[i].fpumode[0]);
#endif

#ifdef DYNAMIC_TASKS
            fscanf(inf,"%d",&dntasks);
#endif
            fclose(inf); /* fall through into display */

        case 'V': /* view current defs */
            printf("%20s %14s\n",job_id,timestamp);
            printf("\tDefault task stack size is %d\n",taskstksz);
#ifdef DYNAMIC_TASKS
            printf("\tNumber of dynamic tasks is %d\n",dntasks);
#endif
            printf(
           "  #   Name    Pri  Entry  Stack Start Description         FPU\n");
         /* 123 12345678 1234 12345678 1234  123  12345678901234567890 1 */

            if (romtcb)
               kn = SIZEOF_KTCB;
            for (n = SIZEOF_TCB, i = 1, j = 1; i <= ntasks;
                 n += SIZEOF_TCB, i++, j++)
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
#ifdef VARNAMLEN
                printf("%3d %*s %4d %*s %4d  %3d  %20s %1s\n",
                    i,namlen,&task[i].name[0],task[i].priority,
                    namlen,&task[i].entry[0],task[i].stksiz,
                    task[i].order,&task[i].desc[0],&task[i].fpumode[0]);
#else
                printf("%3d %8s %4d %8s %4d  %3d  %20s %1s\n",
                    i,&task[i].name[0],task[i].priority,
                    &task[i].entry[0],task[i].stksiz,
                    task[i].order,&task[i].desc[0],&task[i].fpumode[0]);
#endif
                n += task[i].stksiz;
#ifdef FPU
                if (task[i].fpumode[0] == 'Y')
                   n += SIZEOF_FPREGS;
#endif
                if (romtcb)
                   kn += SIZEOF_KTCB;
            }
#ifdef DYNAMIC_TASKS
            n += dntasks * SIZEOF_TCB;
#endif
            printf("Total number of RAM bytes required = %ld\n",n);
            if (romtcb)
                printf("Total number of ROM bytes required = %ld\n",kn);
            break;

        case 'K': /* keep definition file */
#ifdef DYNAMIC_TASKS
            if ((ntasks + dntasks) == 0)
#else
            if (ntasks == 0)
#endif
               if ( !nodata("No tasks are defined") )
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

#ifdef DYNAMIC_TASKS
re_dntask:
            tmp = getnum("# of dynamic tasks> ");
            if (tmp == -1) /* allow zero after non-zero entry */
            {
               dntasks = 0;
               dirty++;
               goto create;
            }
            if (tmp == 0)
               goto create;
            if ((ntasks + tmp) > NTASKS) /* task count too large */
            {
               printf("\tToo many tasks, Max # tasks is %d\n",NTASKS);
               goto re_dntask;
            }
            dntasks = tmp;
            dirty++;
create:
#endif
            if ((outf = ixfopen("ctask.def","w")) == NULL)
            {
                printf("\tSorry, can't create ctask.def\n");
                break;
            }
            s_gettime();
            fprintf(outf,"%20s\n",job_id);
            fprintf(outf,"%14s\n",timestamp);
            fprintf(outf,"%d\n",taskstksz);
            fprintf(outf,"%d\n",ntasks);
            for (i = 1; i <= ntasks; i++)
#ifdef VARNAMLEN
                fprintf(outf,"%4d%*s%*s%4d%3d%20s%1s\n",
                        task[i].priority,namlen,&task[i].name[0],
                        namlen,&task[i].entry[0],task[i].stksiz,task[i].order,
                        &task[i].desc[0],&task[i].fpumode[0]);
#else
                fprintf(outf,"%4d%8s%8s%4d%3d%20s%1s\n",
                        task[i].priority,&task[i].name[0],
                        &task[i].entry[0],task[i].stksiz,task[i].order,
                        &task[i].desc[0],&task[i].fpumode[0]);
#endif
#ifdef DYNAMIC_TASKS
            fprintf(outf,"%d\n",dntasks);
#endif
            fclose(outf);
            break;

        case 'F': /* dirty */
            dirty++;

        case 'G': /* generate new C file and H file iff dirty */
#ifdef DYNAMIC_TASKS
            if ((ntasks+dntasks) == 0)
#else
            if (ntasks == 0)
#endif
               if ( !nodata("No tasks are defined") )
                  break;

            if ((outf = oxfopen("ctask.c","w")) == NULL)
            {
                 printf("\tSorry, can't create ctask.c\n");
                 break;
            }
            s_gettime();
           fprintf(outf,"/* ctask.c - RTXC Task definitions - %20s %14s */\n\n",
                job_id,timestamp);
            finclude(outf,"copyrite.h");

            n_f(outf); /* near/far defines */

            fprintf(outf,"#include \"typedef.h\"\n");
            fprintf(outf,"#include \"rtxstruc.h\"\n\n");
            fprintf(outf,"#include \"ctask.h\"\n\n");
            fprintf(outf,"#define NULL 0\n\n");
            for (i = 1; i <= ntasks; i++)
                if (task[i].name[0] != ' ')
                    fprintf(outf,"#define STKSZ%d %d\n",i,task[i].stksiz);
            fprintf(outf,"\n");

            if (konst)
               fprintf(outf,"const ");
	    fprintf(outf,"TASK ntasks = NTASKS;\n\n");
#ifdef DYNAMIC_TASKS
            if (konst)
               fprintf(outf,"const ");
	    fprintf(outf,"TASK dntasks = DNTASKS;\n\n");
#endif
            for (i = 1; i <= ntasks; i++)
            {
                if (task[i].name[0] != ' ')
                {
		    fprintf(outf,"static char ks_stkx stack%d[STKSZ%d];\n",i,i);
                }
            }

            fprintf(outf,"\n");
            if (konst)
               fprintf(outf,"const ");
	    fprintf(outf,"TASK startls[] =\n");
            fprintf(outf,"{\n");
            for (i = 1; i <= 99; i++) /* 1-99 are valid orders */
            {
                 for (j = 1; j <= ntasks; j++)
                     if (task[j].order == i)
                     {
                         if (task[j].name[0] != ' ')
#ifdef VARNAMLEN
                             fprintf(outf,"   %*s, /* %20s */\n",
                                 namlen,&task[j].name[0],&task[j].desc[0]);
#else
                             fprintf(outf,"   %8s, /* %20s */\n",
                                 &task[j].name[0],&task[j].desc[0]);
#endif
                         else
                             fprintf(outf,"   %d, /* %20s */\n",
                                 i,&task[j].desc[0]);
                     }
            }
            fprintf(outf,"   0  /* null terminated list */\n");
            fprintf(outf,"};\n\n");

            for (i = 1; i <= ntasks; i++) /* define entry point externs */
            {
                if (task[i].name[0] != ' ')
#ifdef VARNAMLEN
                    fprintf(outf,"extern void %*s(void);\n", namlen,
                        task[i].entry);
#else
                    fprintf(outf,"extern void %8s(void);\n", task[i].entry);
#endif
            }
            fprintf(outf,"\n");

            for (i = 1; i <= ntasks; i++) /* allocate fpu save areas */
            {
                if (task[i].fpumode[0] == 'Y')
		    fprintf(outf,"static FPREGS fpu%d;\n",i);
            }
            fprintf(outf,"\n");

            if (konst)
               fprintf(outf,"const ");
	    fprintf(outf,"KTCB rtxktcb[1+NTASKS] =\n");
            fprintf(outf,"{\n");
            for (i = 0; i <= ntasks; i++)
            {
                if (task[i].name[0] == ' ')
                    fprintf(outf,
                        "  { NULL    , NULL, 0, 0 }, /* null task */\n");
                else
                {
#ifdef VARNAMLEN
                    fprintf(outf,"  { %*s, stack%d, STKSZ%d, %d",
                            namlen,task[i].entry,i,i,task[i].priority);
#else
                    fprintf(outf,"  { %8s, stack%d, STKSZ%d, %d",
                            task[i].entry,i,i,task[i].priority);
#endif
                    if (task[i].fpumode[0] == 'Y')
                        fprintf(outf,", &fpu%d", i);
                    if (i == ntasks)
                        fprintf(outf," } /* %s-%20s */\n",
                                &task[i].name[0],&task[i].desc[0]);
                    else
                        fprintf(outf," }, /* %s-%20s */\n",
                                &task[i].name[0],&task[i].desc[0]);
                }
            }
            fprintf(outf,"};\n\n");

#ifdef DYNAMIC_TASKS
	    fprintf(outf,"TCB ks_tblx rtxtcb[1+NTASKS+DNTASKS];\n\n");
#else
	    fprintf(outf,"TCB ks_tblx rtxtcb[1+NTASKS];\n\n");
#endif

            if (symflag) /* RTXCbug symbols enabled */
            {
               fprintf(outf,"#define NAMMAX %d\n\n",NAMMAX);
               if (konst)
                  fprintf(outf,"const ");
               fprintf(outf,"char taskname[1+NTASKS][NAMMAX+1] =\n");
               fprintf(outf,"{\n");
#if 0 /* names padded to NAMMAX chars vs mixed lengths */
               for (i = 0; i < ntasks; i++)
                  fprintf(outf,"   \"%s\",\n",&task[i].name[0]);
               fprintf(outf,"   \"%s\"\n",&task[i].name[0]);
#else
               for (i = 0; i <= ntasks; i++)
               {
                  fprintf(outf,"   \"");
                  for (j = 0; j < NAMMAX; j++)
                  {
                     if (task[i].name[j] == ' ')
                        break;
                     fprintf(outf,"%c",task[i].name[j]);
                  }

                  if (i < ntasks) /* add comma on all but last */
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
                printf("\tNew ctask.h not generated\n");
                break;
            }

            if ((outh = oxfopen("ctask.h","w")) == NULL)
            {
                printf("\tSorry, can't create ctask.h");
                goto loop;
            }
            fprintf(outh,"/* ctask.h - RTXC Task include file */\n\n");
            finclude(outh,"copyrite.h");
	    fprintf(outf,"#define NTASKS %d\n\n",ntasks);
#ifdef DYNAMIC_TASKS
            fprintf(outh,"#define DNTASKS %d\n\n",dntasks);
#endif
	    for (i = 1; i <= ntasks; i++)
            {
                if (task[i].name[0] != ' ')
#ifdef VARNAMLEN
                    fprintf(outh,"#define %*s %d /* %20s */\n",
                        namlen,&task[i].name[0],i,&task[i].desc[0]);
#else
                    fprintf(outh,"#define %8s %d /* %20s */\n",
                        &task[i].name[0],i,&task[i].desc[0]);
#endif
            }

            dirty = 0;    /* clean again */
            fclose(outh);
            break;

        case 'P': /* print task definitions */
            fprintf(stdprn," RTXC Task definitions - %20s %14s\n\n",
                 job_id,timestamp);
            fprintf(stdprn,"Default task stack size is %d\n",taskstksz);
#ifdef DYNAMIC_TASKS
            fprintf(stdprn,"\tNumber of dynamic tasks is %d\n",dntasks);
#endif
            fprintf(stdprn,
        "  #   Name    Pri  Entry  Stack Start Description         FPU\n");
      /* 123 12345678 1234 12345678 1234  123  12345678901234567890 1 */

            if (romtcb)
                kn = SIZEOF_KTCB;
            for (n = SIZEOF_TCB, i = 1; i <= ntasks; n += SIZEOF_TCB, i++)
            {
#ifdef VARNAMLEN
                fprintf(stdprn,"%3d %*s %4d %*s %4d  %3d  %20s %1s\n",
                    i,namlen,&task[i].name[0],task[i].priority,
                    namlen,&task[i].entry[0],task[i].stksiz,
                    task[i].order,&task[i].desc[0],&task[i].fpumode[0]);
#else
                fprintf(stdprn,"%3d %8s %4d %8s %4d  %3d  %20s %1s\n",
                    i,&task[i].name[0],task[i].priority,
                    &task[i].entry[0],task[i].stksiz,
                    task[i].order,&task[i].desc[0],&task[i].fpumode[0]);
#endif
                n += task[i].stksiz;
#ifdef FPU
                if (task[i].fpumode[0] == 'Y')
                   n += SIZEOF_FPREGS;
#endif
                if (romtcb)
                   kn += SIZEOF_KTCB;
            }
#ifdef DYNAMIC_TASKS
            n += dntasks * SIZEOF_TCB;
#endif
            fprintf(stdprn,"Total number of RAM bytes required = %ld\n",n);
            if (romtcb)
               fprintf(stdprn,"Total number of ROM bytes required = %ld\n",kn);
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
     printf("\t *** Task definition module ***\n\n");
     printf("\tL. Load an existing description file\n");
     printf("\tE. Edit individual task parameters\n");
     printf("\tV. View current task definitions\n");
     printf("\tK. Keep new definition file\n");
     printf("\tG. Generate new C source file\n");
     printf("\tF. Force .H file generation\n");
     printf("\tP. Print current task definition\n");
     printf("\tH. Help\n");
     printf("\tX. Exit module\n");
}

static void zero(void)
{
    int i, j;

    for (i = 0; i <= NTASKS; i++)
    {
        for (j = 0; j < NAMMAX; j++)
        {
            task[i].name[j] = ' ';
            task[i].entry[j] = ' ';
        }
        for (j = 0; j < DESMAX; j++)
            task[i].desc[j] = ' ';
        task[i].stksiz = 0;
        task[i].order = 0;
        task[i].priority = 0;
        task[i].fpumode[0] = 'N';
        task[i].fpumode[1] = '\0';
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
    if (len == 0)
    {
        if (task[q].name[0] == ' ')
        {
            printf("\tName required\n");
            goto name;
        }
        goto priority;
    }
    blank(task[q].name);
    if (len > NAMMAX)
        len = NAMMAX;
    for (i = 1; i <= len; i++)
        if (string[i] == ' ')
        {
            printf("\tSpaces not permitted in name\n");
            goto name;
        }
    for (i = 0; i < len; i++)
        task[q].name[i] = string[i];
    if (task[q].name[0] == ' ')
        for (i = 0; i < NAMMAX; i++)
            task[q].name[i] = ' ';
    dirty++;

priority:
    tmp = getnum("Priority> ");
    if (tmp == 0)
        goto entry;
    if (tmp > NULLTASK_PRIORITY-1) /* priority limit */
    {
        printf("\tInvalid priority, 1-%d\n",NULLTASK_PRIORITY-1);
        goto priority;
    }
    task[q].priority = tmp;

entry:
    printf("Entry (up to %d char)> ",NAMMAX);
    gets(string);
    strlwr(string);
    if (isdigit(string[0]) != 0)
    {
        printf("\tEntry must begin with alpha character\n");
        goto entry;
    }
    len = strlen(string);
    if (len == 0)
    {
        if (task[q].entry[0] == ' ')
        {
            printf("\tEntry name required\n");
            goto entry;
        }
        goto size;
    }
    blank(task[q].entry);
    if (len > NAMMAX)
        len = NAMMAX;
    for (i = 0; i < len; i++)
        if (string[i] == ' ')
        {
            printf("\tSpaces not permitted in entry\n");
            goto entry;
        }
    for (i = 0; i < len; i++)
        task[q].entry[i] = string[i];

size:
    tmp = getnum("Task stack size> ");
    if (tmp != 0)
        task[q].stksiz = tmp;
    if (task[q].stksiz == 0)
    {
        task[q].stksiz = taskstksz;
        printf("\tDefault task stack size is %d\n",taskstksz);
    }

order:
    tmp = getnum("Starting task order> ");
    if (tmp == 0)
        goto desc;
    if (tmp > 99) /* starting order limited to 1-99 */
    {
        printf("\tInvalid starting order, 1-99\n");
        goto order;
    }
    task[q].order = tmp;

desc:
    printf("Description (up to %d char)> ",DESMAX);
    gets(string);
    len = strlen(string);
    if (len == 0)
        goto mode;
    blank(task[q].desc);
    if (len > DESMAX)
        len = DESMAX;
    for (i = 0; i < len; i++)
        task[q].desc[i] = string[i];
    dirty++;

mode:

#ifdef FPU
    printf("FPU - Y(es) or N(o)> ");
    gets(string);
    strupr(string);
    len = strlen(string);
    if (len == 0)
        return;
    if (string[0] != 'N' && string[0] != 'Y')
    {
        printf("\tValid options are Y or N\n");
        goto mode;
    }
    blank(task[q].fpumode);
    if (len > 1)
        len = 1;
    for (i = 0; i < len; i++)
        task[q].fpumode[i] = string[i];
    if (task[q].fpumode[0] == ' ')
        task[q].fpumode[0] = 'N';
#endif

    return;
}

/* numeric or symbolic task references */
static int gettask(char *p)
{
   int itask;
   char string[12]; /* 12345678\0 */

   printf("%s",p);
   gets(string);
   strupr(string);

   if ( (string[0] >= '0') && (string[0] <= '9') )
   {
      itask = atoi(string);
      if ( (itask < 0) || (itask > ntasks) )
         itask = 0;
      return(itask);
   }
   else
   {
      for (itask = 1; itask <= ntasks; itask++)
      {
         if (!strcmp(string,&task[itask].name[0]))
            return(itask);
      }
      return(0); /* not found */
   }
}
