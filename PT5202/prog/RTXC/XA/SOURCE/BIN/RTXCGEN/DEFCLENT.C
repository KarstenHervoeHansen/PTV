/* defclent.c - RTXCio - Entry definition program for RTXCgen */
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

#include "rtxcgen.h"

/* CONFIG */
#define NENTRYS 32       /* max # of special functions or errors, arbitrary */

#define DESMAX  20                     /* max # of chars in JOB description */
#define EDESMAX 35                   /* max # of chars in ENTRY description */
#define NAMMAX  15                          /* max # of chars in ENTRY name */

#define SPF  1   /* Specifies in defentry() the entry is a Special Function */
#define ERR  2        /* Specifies in defentry() the entry is a Error Entry */
#define DDF  3                       /* Specifies a drivers definition list */
#define HDR  4                                   /* Specifies a header file */


extern  int getnum(char *);
extern void blank(char *);
extern  int nodata(char *);
extern void s_gettime(void);
extern void clrscr(int);                /* screen clear given so many lines */
extern FILE *ixfopen(const char *,const char *);
extern void cledit_menu(char *, int, char *);   /* editor menu */
extern void zero_jobtime(void);
extern void zero_entries(void);
extern void RTXCstrupr(char *);

extern char timestamp[];

int nentrys;

typedef struct entryrec
{
   char name[NAMMAX+1];
    int value;
   char desc[EDESMAX+1];
}ENTRYREC;

ENTRYREC far entries[NENTRYS+1];

void convert_2_def(char*, char *, int);

static char job_id[DESMAX+1];
static int dirty;

static void define(int, int, char *, int, char *, char *);
static void keep_entry(char *, int);
static void load_menu(char *, char *);

void defentry(char *entryname, int entrytype, char *classname, char * drivername )
{
   int i, j, k, l, len, max, clindex;
   char string[80];
   char defname[13];
   char ownerbuff[80];
   char changebuff1[80];
   char changebuff2[80];
   FILE *inf, *outf;

   dirty = 0;
   nentrys = 0;
   zero_jobtime();
   zero_entries();

   for (j = 0; j < DESMAX; j++)
      job_id[j] = ' ';

   sprintf(ownerbuff,"\n\tController Class: %s  /  Driver: %s\n",classname, drivername);
   if( entrytype == SPF )
	load_menu("*** RTXCio - Special Function Entry menu ***",ownerbuff);
   else
	load_menu("*** RTXCio - Error Entry menu ***",ownerbuff);
loop:
   if( entrytype == SPF )
	printf("\nRTXCio Special Function Entry - Command > ");
   else
	printf("\nRTXCio Error Entry menu - Command > ");

   gets(string);
   strupr(string);
   switch(string[0])
   {
      case 'E': /* edit resource */
	 if( entrytype == SPF )
		cledit_menu("*** RTXCio - Special Function Entry editor ***", 11, ownerbuff);
	 else
		cledit_menu("*** RTXCio - Error Entry editor ***",11, ownerbuff);
editor:
	 if( entrytype == SPF )
		printf("\nRTXCio Special Function Entry Editor - Command > ");
	 else
		printf("\nRTXCio Error Entry Editor - Command > ");
	 gets(string);
	 strupr(string);
	 switch(string[0])
	 {
	    case 'A': /* append mode */
	       if (nentrys >= NENTRYS)
	       {
		  printf("\tMaximum number of Entries exceeded\n");
		  goto editor;
	       }
	       dirty++;
	       nentrys++;
	       blank(&entries[nentrys].name[0]);
	       entries[nentrys].value = 0;
	       blank(&entries[nentrys].desc[0]);
	       define(nentrys, entrytype,ownerbuff,18,"\nAppend mode:\n","");
	       break;

	    case 'D': /* delete mode */
delete:
	       if( !nentrys )
		    break;

	       printf(ownerbuff);
	       if( entrytype == SPF )
			printf("\n\tSpecial_Function_Entries\n");
			      /*    1234  123456789012345     */
	       else
			printf("\n\tError_Entries\n");
			      /*    1234  123456789012345     */
	       for(i=1, k=2, l=1;i<=nentrys;i++, k++)
	       {
		    if (k > SCREENSIZE - 5)
		    {
			 k = 1;
			 l++;
			 printf("-- More --");
			 gets(string);
			 strupr(string);
			 if (string[0] == 'X')
				break;
		    }
		    printf("\t%3d - %15s\n", i, &entries[i].name[0]);
	       }
	       if(l==1)
		   clrscr(19-k+1);
	       else
		   clrscr(21-k+1);
	       i = getnum("\nDelete Entry Number > ");
	       if (i == 0)
	       {
		  printf("\tDelete aborted\n");
		  break;
	       }

	       if (i < 0 || i > nentrys)
	       {
		  if( entrytype == SPF )
			printf("\tINVALID Special Function Entry selected!\n");
		  else
			printf("\tINVALID Error Entry selected!\n");
		  goto delete;
	       }

	       dirty++;
	       for (j = i; j <= nentrys; j++)
		  memcpy(&entries[j],&entries[j+1],sizeof(struct entryrec));
	       nentrys--;
	       break;

	    case 'C': /* change mode */
change:
	       if( !nentrys )
		    break;
	       printf(ownerbuff);
	       if( entrytype == SPF )
			printf("\n\tSpecial_Function_Entries\n");
			      /*    1234  123456789012345     */
	       else
			printf("\n\tError_Entries\n");
			      /*    1234  123456789012345     */
	       for(i=1, k=2, l=1;i<=nentrys;i++, k++)
	       {
		    if (k > SCREENSIZE - 5)
		    {
			 k = 1;
			 l++;
			 printf("-- More --");
			 gets(string);
			 strupr(string);
			 if (string[0] == 'X')
				break;
		    }
		    printf("\t%3d - %15s\n", i, &entries[i].name[0]);
	       }
	       if(l==1)
		   clrscr(19-k+1);
	       else
		   clrscr(21-k+1);
	       i = getnum("\nChange Entry Number > ");

	       if (i == 0)
	       {
		  printf("\tChange aborted\n");
		  break;
	       }

	       if (i < 0 || i > nentrys)
	       {
		  if( entrytype == SPF )
			printf("\tINVALID Special Function Entry selected!\n");
		  else
			printf("\tINVALID Error Entry selected!\n");
		  goto change;
	       }

	       for (j = 0; j < 80; j++)
	       {
			changebuff1[j] = '\0';
			changebuff2[j] = '\0';
	       }

	       if( entrytype == SPF )
	       {
		   sprintf(changebuff1,"\n\tSpecial_Function    Value   Description\n");
			   /*   123456789012345    123     123456...35 */
		   sprintf(changebuff2,"\t %15s    %3d     %35s\n\n",
					      &entries[i].name[0],
					       entries[i].value,
					      &entries[i].desc[0] );
	       }
	       else
	       {
		   sprintf(changebuff1,"\n\tError               Value      Description\n");
			   /*   123456789012345    123      123456...35 */
		   sprintf(changebuff2,"\t %15s    %3d      %35s\n\n",
					      &entries[i].name[0],
					       entries[i].value,
					      &entries[i].desc[0] );
	       }

	       define(i, entrytype,&ownerbuff[0], 15, &changebuff1[0], &changebuff2[0]);
	       break;

	    case 'V': /* view mode */
	       if(!nentrys)
		    break;

	       printf("\n\t%20s %14s\n",job_id,timestamp);
	       printf(ownerbuff);
	       if( entrytype == SPF )
		   printf("\n\tSpecial_Function    Value    Description\n");
			   /*   123456789012345    123      123456...35 */
	       else
		   printf("\n\tError               Value    Description\n");
			   /*   123456789012345    123      123456...35 */

	       for (i = 1, j = 4; i <= nentrys;i++)
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
			if( entrytype == SPF )
				printf("\t %15s    %3d      %35s\n",
						&entries[i].name[0],
						 entries[i].value,
						&entries[i].desc[0] );
			else
				printf("\t% 15s    %3d      %35s\n",
						&entries[i].name[0],
						 entries[i].value,
						&entries[i].desc[0] );
			j++;
	       }
	       printf("\n");
	       if(l==1)
		   clrscr(19-j);
	       else
		   clrscr(21-j);
	       break;

	    case 'X': /* exit editor */
	       if( entrytype == SPF )
			load_menu("*** RTXCio - Special Function Entry menu ***",ownerbuff);
	       else
			load_menu("*** RTXCio - Error Entry menu ***",ownerbuff);
	       goto loop;

	    case 'H': /* help */
	       if( entrytype == SPF )
			cledit_menu("*** RTXCio - Special Function Entry editor ***",11, ownerbuff);
	       else
			cledit_menu("*** RTXCio - Error Entry editor ***",11, ownerbuff);
	       break;

	    default:
	       printf("\t\aInvalid command\n");
	       break;
	 }
	 goto editor;

      case 'L': /* load existing file */
	 convert_2_def(entryname, defname, entrytype);
	 if ((inf = ixfopen(defname,"r")) == NULL)
	 {
	    printf("\tSorry, can't open %s\n",defname);
	    break;
	 }
	 dirty = 0;
	 fscanf(inf,"%20c\n",&job_id[0]);
	 fscanf(inf,"%14c\n",&timestamp[0]);
	 fscanf(inf,"%d\n",&nentrys);
	 for (i = 1; i <= nentrys; i++)
	 {
	      fscanf(inf,"%15c%d%35c\n",
					  &entries[i].name[0],
					  &entries[i].value,
					  &entries[i].desc[0] );
	 }
	 fclose(inf); /* fall through into display */

      case 'V': /* view current defs */
	 if(!nentrys)
	       break;

	 printf("\n\t%20s %14s\n",job_id,timestamp);
	 printf(ownerbuff);
	 if( entrytype == SPF )
	       printf("\n\tSpecial_Function    Value    Description\n");
		     /*     123456789012345    123      123456...35 */
	 else
	       printf("\n\tError               Value    Description\n");
		     /*     123456789012345    123      123456...35 */

	 for (i = 1, j = 4 ,l=1; i <= nentrys;i++)
	 {
		if (j > SCREENSIZE - 5)
		{
			j = 1;
			l++;
			printf("-- More --");
			gets(string);
			strupr(string);
			if (string[0] == 'X')
			break;
		}
		if( entrytype == SPF )
			printf("\t %15s    %3d      %35s\n",
						&entries[i].name[0],
						 entries[i].value,
						&entries[i].desc[0] );
		else
			printf("\t %15s    %3d      %35s\n",
						&entries[i].name[0],
						 entries[i].value,
						&entries[i].desc[0] );
		j++;
	 }
	 printf("\n");
	 if(l==1)
		clrscr(19-j);
	 else
		clrscr(21-j);
	 break;

   case 'K': /* keep description file */
      keep_entry(entryname, entrytype);
      break;


   case 'P': /* print current device definitions */
      if( entrytype == SPF )
	  fprintf(stdprn," RTXCio - Special Function Entry definitions - %20s %14s\n\n",
	 job_id,timestamp);
      else
	  fprintf(stdprn," RTXCio - Error Entry definitions - %20s %14s\n\n",
	 job_id,timestamp);

      fprintf(stdprn,"%20s\n\n", job_id);

      fprintf(stdprn,ownerbuff);
      fprintf(stdprn,"\n\tSpecial_Function    Value    Description\n");
		     /*     123456789012345    123      123456...35 */

      for ( i = 1; i <= nentrys; i++)
      {
	    if( entrytype == SPF )
		  fprintf(stdprn,"\t %15s    %3d      %35s\n\n",
						   &entries[i].name[0],
						    entries[i].value,
						   &entries[i].desc[0] );
	    else
		  fprintf(stdprn,"\t %15s    %3d      %35s\n\n",
						   &entries[i].name[0],
						    entries[i].value,
						   &entries[i].desc[0] );
      }
      fprintf(stdprn,"\n");
      fprintf(stdprn,"\f");
      break;

   case 'H':
      if( entrytype == SPF )
	   load_menu("*** RTXCio - Special Function Entry menu ***",ownerbuff);
      else
	   load_menu("*** RTXCio - Error Entry menu ***",ownerbuff);
      break;

   case 'X':
      if(dirty)
      {
resp:
	   clrscr(19);
	   printf(ownerbuff);
	   if( entrytype == SPF )
		printf("\nWould you like to SAVE your Special Function changes? ('Y' or 'N') > ");
	   else
		printf("\nWould you like to SAVE your Error Entry changes? ('Y' or 'N') > ");
	   gets(string);
	   strupr(string);
	   if( string[0] == 'Y' )
		keep_entry(entryname, entrytype);
	   else
	   {
		if( string[0] == 'N' )
			return;
		else
			goto resp;
	   }
      }
      return;

   default:
      printf("\t\aInvalid command\n");
      break;
   }
   goto loop;

}

static void define(int p, int entrytype, char * owner, int lines, char *change1, char *change2)
{
   int i, len, tmp;
   char string[80];

   clrscr(lines);
   printf(owner);
   printf(change1);
   printf(change2);
   if(*change2 != '\0')
	printf("Change mode:\n");
name:
   if( entrytype == SPF )
	printf("\nSpecial Function Name (up to %d char)> ",NAMMAX);
   else
	printf("\nError Entry Name (up to %d char)> ",NAMMAX);
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
      blank(&entries[p].name[0]);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto name;
	 }
      for (i = 0; i < len; i++)
	 entries[p].name[i] = string[i];
      for (i = len; i < NAMMAX; i++)
	     entries[p].name[i] = ' ';
      entries[p].name[NAMMAX] = '\0';

      if (entries[p].name[0] == ' ')
	 for (i = 0; i < NAMMAX; i++)
	     entries[p].name[i] = ' ';
      dirty++;
   }
   else
   {
      if(entries[p].name[0] == ' ')
      {
	   printf("\tName required\n");
	   goto name;
      }
   }

value:
   if( entrytype == SPF )
	tmp = getnum("Special Function Value > ");
   else
	tmp = getnum("Error Entry Value > ");
   if( tmp == 0 )  /* a character or a return */
		goto desc;
   entries[p].value = tmp;
   dirty++;

desc:
   printf("Description (up to %d char)> ",EDESMAX);
   gets(string);
   len = strlen(string);
   if (len != 0)
   {
      blank(&entries[p].desc[0]);
      if (len > EDESMAX)
	 len = EDESMAX;
      for (i = 0; i < len; i++)
	 entries[p].desc[i] = string[i];
      for (i = len; i < EDESMAX; i++)
	     entries[p].desc[i] = ' ';
      entries[p].desc[EDESMAX] = '\0';
      dirty++;
   }
}

void convert_2_def(char* entryname, char *defname, int entrytype )
{
	while( *entryname != '\0')
	{
		if(*entryname != ' ')
			*defname++ = *entryname++;
		else
			entryname++;
	}
	*defname++ = '.';
	if(entrytype == HDR )
	{
		*defname++ = 'h';
		*defname = '\0';
		strlwr(defname);
		return;
	}
	if(entrytype == SPF )
		*defname++ = 's';
	else
	   if(entrytype == ERR )
		*defname++ = 'e';
	   else
		*defname++ = 'd';
	*defname++ = 'd';
	*defname++ = 'f';
	*defname = '\0';
	strlwr(defname);
}

static void keep_entry(char *entryname, int entrytype)
{
      int i, j, len;
      char string[80], defname[13];
      FILE *outf;

      if (nentrys == 0)
	 if( entrytype == SPF )
	 {
		if ( !nodata("No Special Function Entries are defined") )
			return;
	 }
	 else
	 {
		if ( !nodata("No Error Entries are defined") )
			return;
	 }
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
      convert_2_def(entryname,defname, entrytype );
      if ((outf = ixfopen( defname,"w")) == NULL)
      {
	 printf("\tSorry, can't create %s\n",defname);
	 return;
      }
      s_gettime();
      fprintf(outf,"%20s\n",job_id);
      fprintf(outf,"%14s\n",timestamp);
      fprintf(outf,"%d\n",nentrys);
      for(j=1;j<=nentrys;j++)
	      fprintf(outf,"%15s%3d%35s\n", &entries[j].name[0],
					     entries[j].value,
					    &entries[j].desc[0] );
      fclose(outf);
      dirty = 0;
}

static void load_menu(char *s,char * owner)
{
   clrscr(10);
   printf(owner);
   printf("\n\t%s\n\n",s);
   printf("\tL. Load existing definition file\n");
   printf("\tV. View current definitions\n");
   printf("\tE. Edit individual parameters\n");
   printf("\tK. Keep new definition file\n");
   printf("\tP. Print current definitions\n");
   printf("\tH. Help\n");
   printf("\tX. eXit module\n");
}

#endif
