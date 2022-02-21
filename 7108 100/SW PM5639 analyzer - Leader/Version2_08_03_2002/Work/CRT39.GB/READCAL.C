/*


*/


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include <dir.h>

const char *SWVer = {"V1.0"};

FILE *cal39file,*offset39file;
char fname[500][15];
long os[100];
float offset[50];


//-------- prototypes --------------------------------
int sort_function( const void *a, const void *b);



//-------- functions ---------------------------------
int sort_function( const void *a, const void *b){
 return( strcmp((char *)a,(char *)b) );
}



int findcalfile(){
int done,k;
struct ffblk filedata;

 printf("\nDirectory listing of *.I39\n");
 done = findfirst("*.i39",&filedata,0);
 k = 0;
 if (done == 0)
  strcpy(fname[k],filedata.ff_name);
 k++;
 while (!done)
 {
   printf("%20s", filedata.ff_name);
   done = findnext(&filedata);
   if (done == 0){
    strcpy(fname[k],filedata.ff_name);
    }
   k++;
 }
 printf("\n");
 return k-1;
} //findcalfile()



int strcopy(char *instr, char *outstr, int index, int no){
// copies 'no' of bytes starting at 'index' from 'instr' to 'outstr'
// return is number of copied chars
int n;
 n = 0;
 while ((instr[n+index] != '\0') && (n < no)){
  outstr[n] = instr[index+n];
  n++;
 }
 outstr[n] = '\0';
return (n);
}



void readcaldata(char *fname){
 char strread[4][100],s[5],fdate[10];
 int xx,k,n;
 const float ADRef = 2.4;
 const long ADCounts = 50000;

 cal39file  = fopen(fname,"rt");           // text file with audio data/

 fgets(strread[0], 90, cal39file);        // read file header information
 fgets(strread[0], 90, cal39file);        // read file date
 strcopy(strread[0],fdate,14,6);
 for (k=0; k<17; k++)
  fgets(strread[0], 90, cal39file);        // read file header information

 fgets(strread[0], 90, cal39file);         // data from addr 120-129
 fgets(strread[1], 90, cal39file);         // data from addr 130-139
 fgets(strread[2], 90, cal39file);         // data from addr 140-149
 fgets(strread[3], 90, cal39file);         // data from addr 150-156

 fclose(cal39file);

 for (k=0; k<9; k++){                // data from addr 121-129
  strcopy(strread[0],s,22+(k*5),3);
  os[k] = atoi(s);
 }

 for (k=0; k<10; k++){                // data from addr 130-139
  strcopy(strread[1],s,17+(k*5),3);
  os[k+9] = atoi(s);
 }
 for (k=0; k<10; k++){                // data from addr 140-149
  strcopy(strread[2],s,17+(k*5),3);
  os[k+19] = atoi(s);
 }
 for (k=0; k<7; k++){                 // data from addr 150-156
  strcopy(strread[3],s,17+(k*5),3);
  os[k+29] = atoi(s);
 }

 for (k=0; k<35; k=k+2){
  xx = os[k+1]*256 + os[k];
  if (xx < 0)
   xx = -1 * (32768 + xx);
  offset[k/2] = (xx * 1000 * ADRef) / ADCounts;
 }

 offset39file  = fopen("offset.txt","at");           // text file with offset data/
 fprintf(offset39file,"%8s,%6s,",fname,fdate);
 for (k=0; k<18; k++){
  fprintf(offset39file,"%6.3f,",offset[k]);
 }
 fprintf(offset39file,"\n");
 fclose(offset39file);
} //readcaldata









void main()
{
int k,no;
 clrscr();
 no = findcalfile();

 qsort((void *)fname, no, sizeof(fname[0]), sort_function);

 offset39file  = fopen("offset.txt","wt");           // text file with offset data/
 fprintf(offset39file," KU          Dato     R                                         G                                         B\n");
 fprintf(offset39file,"                      0      1      2      3      4      5      0      1      2      3      4      5      0      1      2      3      4      5\n");
 fclose(offset39file);

 for (k=0;k<no;k++){
  readcaldata(fname[k]);
 }

 printf("readcal   %s\n",SWVer);

}