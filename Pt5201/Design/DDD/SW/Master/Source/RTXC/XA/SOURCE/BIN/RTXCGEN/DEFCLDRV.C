/* defcldrv.c - RTXCio - Class driver definition program for RTXCgen */
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

#define NDRIVRS 32           /* max # of class drivers supported, arbitrary */
#define NENTRYS 32       /* max # of special functions or errors, arbitrary */

#define DESMAX 20                          /* max # of chars in description */
#define NAMMAX 8                             /* max # of chars in file name */
#define PERIOD 1      /* allow one space for a period in complete file name */
#define    EXT 3                   /* max # of chars in file name extension */

#define    SPF 1 /* Specifies in defentry() the entry is a Special Function */
#define    ERR 2      /* Specifies in defentry() the entry is a Error Entry */
#define    DDF 3                     /* Specifies a drivers definition list */
#define    HDR 4                         /* Specifies a drivers header file */

#define    YES 1                                                     /* yes */
#define     NO 0                                                      /* no */

#define UNDEFFED  0                                     /* driver undefined */
#define DEFFED    1                                       /* driver defined */

extern void cledit_menu(char *, int, char *);   /* editor menu */
extern void blank(char *);
extern int getnum(char *);
extern int nodata(char *);
extern void s_gettime(void);
extern FILE *ixfopen(const char *,const char *);
extern void clrscr(int);                /* screen clear given so many lines */
extern void defentry(char *, int, char *, char *);
extern void convert_2_def(char*, char *, int);
extern void convert_2_print(char *, char *);
extern void zero_jobtime(void);
extern void zero_drivers(void);
extern void RTXCstrupr(char *);

extern char timestamp[];

typedef struct entryrec
{
   char name[NAMMAX+1];
    int value;
   char desc[DESMAX+1];
}ENTRYREC;

extern ENTRYREC far entries[];

int ndrives;                                   /* number of drivers created */
int nddeffed;                                   /* number of drivers defined */

typedef struct drvrec
{
      int deflag;
      int deleted;
     char name[NAMMAX+1];
     char spfname[NAMMAX+1];
     char errname[NAMMAX+1];
     char oldspf[NAMMAX+1];
     char olderr[NAMMAX+1];
}DRVREC;

DRVREC far drivers[NDRIVRS+1];
DRVREC * selected_dr[NDRIVRS+1];

void save_header(char *, char *, int );

static  int dirty;
static char job_id[DESMAX+1];

static void define(int, int, char *,  char *, char *);
static void drvdef_menu(int);   /* driver definition editor menu */
static void load_drivers(char *);
static void drvload_menu(void);
static void store_old( int );
static void keep_driver( char * );
static  int setup_selection(void);

void defdrv(char *class, char *drvlist)
{
   int i, j, k, l, len, max, drvindex;
   char string[80], defname[NAMMAX+PERIOD+EXT+1];
   char ibuff[80];
   char ownerbuff[80];
   char changebuff1[80];
   char changebuff2[80];
   FILE *inf, *outf;
   char absname[NAMMAX+1];

   dirty = 0;
   ndrives = 0;
   nddeffed = 0;

   zero_jobtime();
   zero_drivers();

   drvdef_menu(14);
loop:
   printf("\nRTXCio Class Drivers - Command > ");
   gets(string);
   strupr(string);
   switch(string[0])
   {
       case 'D': /* modify driver definition */
	  drvload_menu();
defeditor:
	  printf("\nRTXCio Class Driver Definition - Command > ");
	  gets(string);
	  strupr(string);
	  switch(string[0])
	  {
	      case 'E': /* edit resource */
		 sprintf(ownerbuff,"\n\tController Class: %s  /  Driver: driver definitions\n",class );
		 cledit_menu("*** RTXCio - Class Driver Definition edit menu ***",11,ownerbuff);
editor:
		 printf("\nRTXCio Class Driver Definition Editor - Command > ");
		 gets(string);
		 strupr(string);
		 switch(string[0])
		 {

		     case 'A': /* append mode */
			if (ndrives >= NDRIVRS)
			{
			     printf("\tMaximum number of Class drivers exceeded\n");
			     goto defeditor;
			}
			ndrives++;
			blank(&drivers[ndrives].name[0]);
			blank(&drivers[ndrives].spfname[0]);
			blank(&drivers[ndrives].errname[0]);
			define(ndrives, 18,ownerbuff, "\nAppend mode:\n","");
			store_old(ndrives);
			if( drivers[ndrives].deflag == DEFFED )
				nddeffed++;
			drivers[ndrives].deleted = NO;
			break;

		     case 'D': /* delete mode */
delete:
			printf("\n\tSelected_%s_Drivers\n",class);
			      /*         12345678     */
			for(i=1, k=1,l=1;i<=ndrives;i++, k++)
			{
			     if( drivers[i].deleted == NO )
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
				  printf("\t%3d - %8s\n", i, &drivers[i].name[0]);
			     }
			}
			if(l==1)
				clrscr(19-k+2);
			else
				clrscr(21-k+2);

			sprintf(ibuff,"\nSelect the %s Driver to be DELETED > ",class );
			i = getnum(ibuff);
			if (i == 0)
			{
			     printf("\tDelete aborted\n");
			     break;
			}
			if (i < 0 || i > ndrives)
			{
			     printf("\tINVALID %s Driver selected!\n",class);
			     goto delete;
			}
delete_resp:
			printf("\n\n\t+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			printf("\t+++                        WARNING!                       +++\n");
			printf("\t+++ When deleting a Class Driver, you lose all the header +++\n");
			printf("\t+++ definition files defined for that Class Driver. An    +++\n");
			printf("\t+++ alternative is to 'undef' the Class Driver which will +++\n");
			printf("\t+++ retain the Class Driver information, but remove the   +++\n");
			printf("\t+++ driver from the application.                          +++\n");
			printf("\t+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");
			printf("Do you wish to continue with this deletion? ('Y' or 'N') > ",DESMAX);
			gets(string);
			strupr(string);
			if( string[0] == 'Y' )
			{
				if( drivers[i].deflag == DEFFED )
					nddeffed--;
				drivers[i].deleted = YES;
				dirty++;
				break;
			}
			else
			{
				if( string[0] == 'N' )
					break;
				else
					goto delete_resp;
			}


		     case 'C': /* change mode */
change:
			printf("\n\tSelected_%s_Drivers\n", class);
			      /*         12345678     */
			for(i=1, k=1, l=1;i<=ndrives;i++, k++)
			{
			     if( drivers[i].deleted == NO )
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
				  printf("\t%3d - %8s\n", i, &drivers[i].name[0]);
			     }
			}
			if(l==1)
				clrscr(19-k+2);
			else
				clrscr(21-k+2);

			sprintf(ibuff,"\nSelect the %s Driver to be CHANGED > ",class );
			i = getnum(ibuff);
			if (i == 0)
			{
			     printf("\tChange aborted\n");
			     break;
			}
			if (i < 0 || i > ndrives)
			{
			     printf("\tINVALID %s Driver selected!\n",class);
			     goto change;
			}
			k = drivers[i].deflag;      /* save old deflag value */
			sprintf(changebuff1,"\n    Driver  Selection_Status  Special_Function_Header  Error_Function_Header\n");
			    /*        12345678      12345678            12345678               12345678          */

			if( drivers[i].deflag == DEFFED )
			     sprintf(changebuff2,"    %8s      %8s            %8s               %8s\n",
								     &drivers[i].name[0]
								     , "YES     ",
								     &drivers[i].spfname[0],
								     &drivers[i].errname[0]);
			else
			     sprintf(changebuff2,"    %8s      %8s            %8s               %8s\n",
								     &drivers[i].name[0]
								     , "NO      ",
								     &drivers[i].spfname[0],
								     &drivers[i].errname[0]);
			define(i,15,ownerbuff,changebuff1,changebuff2);
			if( ( k == DEFFED) && (drivers[i].deflag == UNDEFFED) )
				nddeffed--;
			if( ( k == UNDEFFED) && (drivers[i].deflag == DEFFED) )
				nddeffed++;
			break;

		     case 'V': /* view mode */
			if(!ndrives)
			      break;
			printf("\n\t%20s %14s\n",job_id,timestamp);
			printf("\n\tController Class: %s\n",class);
			for (i = 1, j = 3,l=1; i <= ndrives;i++)
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
			    if( i == 1 )
			    {
				printf("\n    Driver  Selection_Status  Special_Function_Header  Error_Function_Header\n");
					/*    12345678      12345678            12345678               12345678          */
				j++;
				j++;
			    }
			    if( drivers[i].deleted == NO )
			    {
				printf("    %8s", &drivers[i].name[0]);
				if( drivers[i].deflag == DEFFED )
					printf("      %8s", "YES     ");
				else
					printf("      %8s", "NO      ");
				printf("            %8s               %8s\n",
						       &drivers[i].spfname[0],
						       &drivers[i].errname[0] );
				j++;
			    }
			}
			if(l==1)
				clrscr(19-j+1);
			else
				clrscr(21-j+1);
			printf("\n");
			break;

		     case 'X': /* exit editor */
			drvload_menu();
			goto defeditor;

		     case 'H': /* help */
			cledit_menu("*** RTXCio - Class Driver Definition edit menu ***",11,ownerbuff);
			break;

		     default:
			printf("\t\aInvalid command\n");
			break;
		  }
		  goto editor;
	      case 'L': /* load existing file */
		 load_drivers(drvlist);

	      case 'V': /* view current defs */
		 if(!ndrives)
		      break;

		 printf("\n\t%20s %14s\n",job_id,timestamp);
		 printf("\n\tController Class: %s\n",class);

		 for ( i = 1, j = 5, l=1; i <= ndrives; i++)
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
		    if( i == 1 )
		    {
			printf("\n    Driver  Selection_Status  Special_Function_Header  Error_Function_Header\n");
			    /*       12345678      12345678            12345678               12345678          */
		    }
		    if( drivers[i].deleted == NO )
		    {
			printf("    %8s", &drivers[i].name[0]);
			if( drivers[i].deflag == DEFFED )
				printf("      %8s", "YES     ");
			else
				printf("      %8s", "NO      ");
			printf("            %8s              %8s\n",
						       &drivers[i].spfname[0],
						       &drivers[i].errname[0] );
			j++;
		    }
		 }
		 if(l==1)
			clrscr(19-j+1);
		 else
			clrscr(21-j+1);

		 printf("\n");
		 break;

	      case 'K': /* keep description file */
		 keep_driver( drvlist );
		 break;

	      case 'P': /* print current device definitions */
		 if(!ndrives)
		     break;

		 fprintf(stdprn," RTXCio - Class driver definitions - %20s %14s\n\n",
							    job_id,timestamp);
		 fprintf(stdprn,"%20s\n\n", job_id);
		 fprintf(stdprn,"\nController Class: %s\n",class);
		 fprintf(stdprn,"\nDriver     Selection_Status      Special_Function_Header   Error_Function_Header\n");
			     /*     12345678        12345678           12345678                12345678          */

		 for ( i = 1; i <= ndrives; i++)
		 {
		    if( drivers[i].deleted == NO )
		    {
			fprintf(stdprn," %8s", &drivers[i].name[0]);
			if( drivers[i].deflag == DEFFED )
				fprintf(stdprn,"        %8s", "YES     ");
			else
				fprintf(stdprn,"        %8s", "NO      ");
			fprintf(stdprn,"           %8s                %8s\n",
						       &drivers[i].spfname[0],
						       &drivers[i].errname[0] );
		    }
		 }
		 fprintf(stdprn,"\n");
		 fprintf(stdprn,"\f");
		 break;

	      case 'H':
		 drvload_menu();
		 break;

	      case 'X':
		 if(dirty)
		 {
resp:
			clrscr(19);
			printf("\n\tController Class: %s\n",class);
			printf("\nWould you like to SAVE your Class Driver changes? ('Y' or 'N') > ",DESMAX);
			gets(string);
			strupr(string);
			if( string[0] == 'Y' )
			{
				keep_driver( drvlist );
				drvdef_menu(13);
				goto loop;
			}
			else
			{
				if( string[0] == 'N' )
				{
					drvdef_menu(14);
					goto loop;
				}
				else
					goto resp;
			}
		 }
		 drvdef_menu(14);
		 goto loop;

	      default:
		 printf("\t\aInvalid command\n");
		 break;
	  }
	  goto defeditor;

       case 'S': /* Special function definition */
	  load_drivers(drvlist);
	  if( !ndrives )
	  {
	       printf("\t\nYou MUST FIRST define a Class Driver!\n");
	       break;
	  }
	  printf(  "\n\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	  printf(  "\t    Below is a list of the %s Drivers you have selected    \n",class);
	  printf(  "\t    for your application. From this list, choose the       \n");
	  printf(  "\t    DRIVER associated with the SPECIAL FUNCTION header     \n");
	  printf(  "\t    file to be created or modified. To EXIT enter a Zero.  \n");
	  printf(  "\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	  i = setup_selection();
	  for(j=1, k=5, l=1; j<=i; j++, k++)
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
		printf("\t%3d - %s\n", j, selected_dr[j]->name);
	  }
	  if(l==1)
		clrscr(19-k+1);
	  else
		clrscr(21-k+1);

spfentry:
	  sprintf(ibuff,"\nSelect the %s Driver > ",class );
	  j = getnum(ibuff);
	  if (j == 0)
	  {
		drvdef_menu(14);
		break;
	  }

	  if (j < 0 || j > i )
	  {
		printf("\tINVALID %s Driver selected!\n",class);
		goto spfentry;
	  }
	  convert_2_print( selected_dr[j]->name, &absname[0] );
	  defentry(selected_dr[j]->spfname, SPF, class, absname);
	  drvdef_menu(14);
	  break;


       case 'E': /* Error entry definition */
	  load_drivers(drvlist);
	  if( !ndrives )
	  {
	       printf("\t\nYou MUST FIRST define a Class Driver!\n");
	       break;
	  }
	  printf(  "\n\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	  printf(  "\t    Below is a list of the %s Drivers you have selected    \n",class);
	  printf(  "\t    for your application. From this list, choose the       \n");
	  printf(  "\t    DRIVER associated with the ERROR header file to be     \n");
	  printf(  "\t    created or modified. To EXIT enter a Zero.             \n");
	  printf(  "\t++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

	  i = setup_selection();
	  for(j=1, k=5, l=1; j<=i; j++, k++)
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
		printf("\t%3d - %s\n", j, selected_dr[j]->name);
	  }
	  if(l==1)
		clrscr(19-k+1);
	  else
		clrscr(21-k+1);

errentry:
	  sprintf(ibuff,"\nSelect the %s Driver > ",class );
	  j = getnum(ibuff);
	  if (j == 0)
	  {
		drvdef_menu(14);
		break;
	  }

	  if (j < 0 || j > i )
	  {
		printf("\tINVALID %s Driver selected!\n",class);
		goto errentry;
	  }

	  convert_2_print( selected_dr[j]->name, &absname[0] );
	  defentry(selected_dr[j]->errname, ERR, class, absname);
	  drvdef_menu(14);
	  break;

       case 'X': /* exit editor */
	  return;

       case 'H': /* help */
	  drvdef_menu(14);
	  break;

       default:
	  printf("\t\aInvalid command\n");
	  break;
    }
    goto loop;

}

static void define(int q, int lines, char * owner,  char *change1, char *change2)
{
   int i, len, tmp;
   char string[80];

   clrscr(lines);

   printf(owner);
   if(*change2 != '\0')
   {
	printf(change1);
	printf(change2);
	printf("\nChange mode:\n");
   }
   else
	printf(change1);

name:
   printf("\nClass Driver Name (up to %d char)> ",NAMMAX);
   gets(&string[0]);
   strupr(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto name;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(&drivers[q].name[0]);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto name;
	 }
      for (i = 0; i < len; i++)
	 drivers[q].name[i] = string[i];
      for (i = len; i < NAMMAX; i++)
	     drivers[q].name[i] = ' ';
      drivers[q].name[NAMMAX] = '\0';

      if (drivers[q].name[0] == ' ')
	 for (i = 0; i < NAMMAX; i++)
	     drivers[q].name[i] = ' ';
      dirty++;
   }
   else
   {
      if (drivers[q].name[0] == ' ')
      {
	   printf("\tName required\n");
	   goto name;
      }
   }

spfname:
   printf("Class Driver's Special Function header file Name (up to %d char)> ",NAMMAX);
   gets(&string[0]);
   strlwr(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto spfname;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(&drivers[q].spfname[0]);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto spfname;
	 }
      for (i = 0; i < len; i++)
	 drivers[q].spfname[i] = string[i];
      for (i = len; i < NAMMAX; i++)
	     drivers[q].spfname[i] = ' ';
      drivers[q].spfname[NAMMAX] = '\0';

      if (drivers[q].spfname[0] == ' ')
	 for (i = 0; i < NAMMAX; i++)
	     drivers[q].spfname[i] = ' ';
      dirty++;
   }
   else
   {
      if (drivers[q].spfname[0] == ' ')
      {
	   printf("\tName required\n");
	   goto spfname;
      }
   }

errname:
   printf("Class Driver's Error header file Name (up to %d char)> ",NAMMAX);
   gets(&string[0]);
   strlwr(string);
   if (isdigit(string[0]) != 0)
   {
      printf("\tName must begin with alpha character\n");
      goto errname;
   }
   len = strlen(string);
   if (len != 0)
   {
      blank(&drivers[q].errname[0]);
      if (len > NAMMAX)
	 len = NAMMAX;
      for (i = 1; i <= len; i++)
	 if (string[i] == ' ')
	 {
	    printf("\tSpaces not permitted in name\n");
	    goto errname;
	 }
      for (i = 0; i < len; i++)
	 drivers[q].errname[i] = string[i];
      for (i = len; i < NAMMAX; i++)
	     drivers[q].errname[i] = ' ';
      drivers[q].errname[NAMMAX] = '\0';

      if (drivers[q].errname[0] == ' ')
      {
	 for (i = 0; i < NAMMAX; i++)
	     drivers[q].errname[i] = ' ';
      }
      dirty++;
   }
   else
   {
      if (drivers[q].errname[0] == ' ')
      {
	   printf("\tName required\n");
	   goto errname;
      }
   }

defvalue:
   printf("Select this Driver? (default=yes, Y or N) > ");
   gets(string);
   strupr(string);
   len = strlen(string);
   if( len != 0 )
   {
	if(string[0] == 'Y')
		drivers[q].deflag = DEFFED;
	else
	{
		if( string[0] == 'N' )
			drivers[q].deflag = UNDEFFED;
		else
			goto defvalue;
	}
	dirty++;
   }
}

static void drvdef_menu(int lines)   /* driver definition editor menu */
{
   clrscr(lines);
   printf("\n\t*** RTXCio - Class Drivers ***\n\n");
   printf("\tD. Definitions\n");
   printf("\tS. Special functions entries\n");
   printf("\tE. Error Entries\n");
   printf("\tH. Help\n");
   printf("\tX. eXit\n");
}

static void load_drivers(char *drvlist)
{
	 int i;
	 FILE *inf;

	 if ((inf = ixfopen(drvlist,"r")) == NULL)
	 {
	    printf("\tSorry, can't open %s\n", drvlist);
	    return;
	 }
	 dirty = 0;
	 fscanf(inf,"%20c\n",&job_id[0]);
	 fscanf(inf,"%14c\n",&timestamp[0]);
	 fscanf(inf,"%d\n",&ndrives);
	 fscanf(inf,"%d\n",&nddeffed);
	 for (i = 1; i <= ndrives; i++)
	 {
		fscanf(inf,"%d%8c%8c%s\n", &drivers[i].deflag,
					   &drivers[i].name[0],
					   &drivers[i].spfname[0],
					   &drivers[i].errname[0] );
		store_old(i);
		drivers[i].deleted = NO;
	 }
	 fclose(inf); /* fall through into display */
}

static void drvload_menu(void)
{
   clrscr(12);
   printf("\n\t*** RTXCio - Class Driver Definition menu ***\n\n");
   printf("\tL. Load the driver definition file associated with the selected class\n");
   printf("\tV. View the driver definition file (LOAD FIRST)\n");
   printf("\tE. Edit a driver definition file\n");
   printf("\tK. Keep the new driver definition file\n");
   printf("\tP. Print the driver definition file\n");
   printf("\tH. Help\n");
   printf("\tX. eXit module\n");
}

static void store_old( int index )
{
	strcpy( &drivers[index].oldspf[0], &drivers[index].spfname[0] );
	strcpy( &drivers[index].olderr[0], &drivers[index].errname[0] );
}

void save_header(char *from, char *to, int entrytype )
{
     char oldname[NAMMAX+PERIOD+EXT+1], newname[NAMMAX+PERIOD+EXT+1];

     convert_2_def(from, &oldname[0], entrytype );
     convert_2_def(to, &newname[0], entrytype );

     strlwr(oldname);
     strlwr(newname);

     if( rename(&oldname[0],&newname[0]) )
	  ; /* printf("\tCan't rename the file:  %s \n",&oldname[0]); */
     else
	strcpy(from,to);
}

static void keep_driver( char *drvlist )
{
	int i, j, k, len;
	char string[80], defname[NAMMAX+PERIOD+EXT+1];
	FILE  *outf, *deloutf;

	if (ndrives == 0)
		if ( !nodata("No Class Drivers are defined") )
			return;

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
	if ((outf = ixfopen(drvlist,"w")) == NULL)
	{
		printf("\tSorry, can't create %s\n",drvlist[0]);
		return;
	}
	s_gettime();
	fprintf(outf,"%20s\n",job_id);
	fprintf(outf,"%14s\n",timestamp);

	k = ndrives;
	for (i = 1; i <= ndrives; i++)
	if( drivers[i].deleted == YES )
		k--;

	fprintf(outf,"%d\n",k); /* true number of drivers after deletions */
	fprintf(outf,"%d\n",nddeffed);
	for (i = 1; i <= ndrives; i++)
	{
		if( drivers[i].deleted == NO )
		{
			if( strncmp( &drivers[i].oldspf[0], &drivers[i].spfname[0], NAMMAX ) != 0 )
				save_header( &drivers[i].oldspf[0], &drivers[i].spfname[0], SPF );
			if( strncmp( &drivers[i].olderr[0], &drivers[i].errname[0], NAMMAX ) != 0 )
				save_header( &drivers[i].olderr[0], &drivers[i].errname[0], ERR );
			fprintf(outf,"%3d%8s%8s%8s\n", drivers[i].deflag,
						       &drivers[i].name[0],
						       &drivers[i].spfname[0],
						       &drivers[i].errname[0]);
		}
		else
		{
			convert_2_def( &drivers[i].spfname[0], &defname[0], SPF );
			remove( defname );
			convert_2_def( &drivers[i].errname[0], &defname[0], ERR );
			remove( defname );
			convert_2_def( &drivers[i].spfname[0], &defname[0], HDR );
			remove( defname );
			convert_2_def( &drivers[i].errname[0], &defname[0], HDR );
			remove( defname );
			for (j = i; j <= ndrives; j++)
				memcpy(&drivers[j],&drivers[j+1],sizeof(struct drvrec));
			i--;
			ndrives--;
		}
	}
	fclose(outf);
	dirty = 0;
}

static int setup_selection(void)
{
    int i, j;

    j=1;
    for(i=1;i<=ndrives;i++)
    {
	  if( drivers[i].deflag )
	  {
	      selected_dr[j] = &drivers[i];
	      j++;
	  }
    }
    return(j-1);
}

#endif
