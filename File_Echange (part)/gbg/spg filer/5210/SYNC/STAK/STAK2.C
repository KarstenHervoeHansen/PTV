// program stak2;  { Stak manager    P.Christiansen  9002/9711 }
// skriver kun symbolerne for kaldene ud
// For hvert entry (kald) i 'staklist' tages input fra filen 'stakdat'
//    og kaldene udskrives i en tr‘struktur.
// Hvis en fil, "symbols" eksisterer med linier med: <kald> <symbol for kald>
// udskrives symbolerne i stedet.

#include <stdio.h>

FILE *ind, *ud, *symbols, *opgave;
char a[21][800];
char b[21][32];
char * s[21];
char * kald[21];
char *filnavn;
char fundet, ny, symbols_on, symb;
int n,i;
// *****************************************************************
char *r="                                                        ";

char * getnxt(char * st)	// find foerste ord paa linien
{  // char r[50];
   int i, j, tal1, fejl;

  i= 0; j= 0;
  while (st[i]==' ')  i++;
  while(st[i]>' ' && st[i] <= 'z')

  {  r[j++]= *(st+i); i++; }
  r[j]= '\0';			// afslut fundet ord
/*
  j= 0;
  while(*(s[n]+i) != '\0')
     *(s[n]+j++)= *(s[n]+i++);	// fjern fundet ord fra strengen
  *(s[n]+j)= *(s[n]+i);
*/
  return(r);                    // returner fundet ord
}
// *****************************************************************

char *navn(char *symbol)
{  char *t="                                                            ";
   char *u="                                                     ";
   char i, j;
   if (!symbols_on)
	return(symbol);
   else
   {  rewind(symbols);
      symb= 0;
      while (symb==0 && readln(symbols,t))
      {  u= getnxt(t);
	 if (strcmp(u,symbol)==0)
	    symb= 1;
      }
      if (symb==1)
      {  i= strlen(symbol);    // fjern fundet ord fra strengen
	 j= 0;
	 while(*(t+i) != '\0')
	    *(t+j++)= *(t+i++);
	 *(t+j)= *(t+i);
      }
      t= getnxt(t);
      if (*t=='\0')
	return(symbol);
      else
	 return(t);
   }
}
// *****************************************************************

void findlin(char *ka)
{  char c, m, i, j;
   char *u="                                                     ";
   char *kptr="                                                     ";

   rewind(ind);			// reset datafil
   fundet= 0;
   while (fundet==0 && readln(ind,s[n]))
   {   u= getnxt(s[n]);
      if (strcmp(u, ka)==0)
	 fundet= 1;
   }
   if ((i= strlen(u)) < strlen(s[n]))
      i++;
   j= 0;
   while(*(s[n]+i) != '\0')
      *(s[n]+j++)= *(s[n]+i++);	// fjern fundet ord fra strengen
   *(s[n]+j)= *(s[n]+i);

   if (fundet == 0)
   {  printf("fejl - %s ikke fundet",ka); exit(1);
   }

   do
   {  strcpy(kald[n+1], getnxt(s[n]));

      if((i=strlen(kald[n+1])) < strlen(s[n]))
	 i++;
      j= 0;
       while(*(s[n]+i) != '\0')
	 *(s[n]+j++)= *(s[n]+i++);	// fjern fundet ord fra strengen
      *(s[n]+j)= *(s[n]+i);

      if (*kald[n+1] != '\0')
      {  n++; if (ny==0) ny= n;
	 findlin(kald[n]);
      }
   }  while (*kald[n+1] != '\0');


    if (ny)
    {  // fputc('\n',ud);	// ny linie
	// fputc(n-1+'0',ud); fputc(' ',ud);	// skriv kaldeniveau
      for (i=1;i<=n;i++)
      if (i>=ny)
      {  kptr= navn(kald[i]);
	 // fputc('\n',ud);	// ny linie
//	 for (m=1;m<(i-1)*3-2;m++)  fputc(' ',ud); // lav indent
	 for (m=1;m<i-1;m++) { fputc('³',ud); fputc(' ',ud); fputc(' ',ud);}
	 fputc('À ',ud); fputc('Ä ',ud); fputc('Ä ',ud); // lav indent
	 while ((c= *kptr++)!='\0')
	    fputc(c,ud);			// skriv neste kald
	 fputc('\n',ud);			// ny linie
      }
      // fputc('\n',ud);
      ny= 0;
    }
    n--;
}

// *****************************************************************

int readln(FILE * fil,char *h)
{  char c;
   int i;
   h[0]= '\0'; i= 0;
   while ((fread(&c,1,1,fil))==1 && (c!='\n'|| (c=='\n' && i==0))) // les 1 linie (1' soegeord)
   {  h[i++] = c;
      if (h[0]=='\n')  i= 0;
   }
   h[i]= '\0';
   if (h[0]=='\0') return(0);
   else return(1);
}
// *****************************************************************

void main(int argc, char *argv[])
{   char *h, c;

   for (i=0;i<21;i++)
   {  s[i]= &a[i];
      kald[i]= &b[i];
   }

   if ((ind= fopen("stakdat","r"))==NULL)
   {  printf("FEJL: STAKDAT ikke fundet.");
      printf("Der skal findes en fil STAKDAT hvor hver subrutines kald er beskrevet,\n");
      printf("f.eks. linien:  MAIN SUB1 SUB2 SUB3   beskriver at rutinen main\n");
      printf("kalder sub1, sub2 og sub3. Disse skal s† igen beskrives p† hver\n");
      printf("sin linie (skal st† alene hvis de ikke kalder andre)\n");
      printf("I en s‘rskilt fil: SYMBOLS, kan p† hver linie angives\n");
      printf("et symbol og et alias\n");
      exit(1);
   }

   if (argc<2 && (opgave= fopen("staklist","r"))==NULL)
   {  printf("FEJL: Ingen parameter eller STAKLIST ikke fundet.\n");
      printf("Der skal enten angives p† kommandolinien eller i filen: STAKLIST\n");
      printf("hvilke rutiner der skal unders›ges.\n");
      printf("f. eks.: STAK2 SUB2 RUTINX   opl›ser sub2 og rutinx.\n");
      printf("I filen STAKLIST skal SUB2 og RUTINX skrives p† hver sin linie.\n");
      exit(1);
   }
//   if ((ud= fopen("stakud","w"))==NULL)
//	{ printf("Fejl ved aabning af %s\n","stakud"); exit(1); }
  ud= stdout;

   if ((symbols= fopen("symbols","r"))!=NULL)
      symbols_on= 1;
   else
      symbols_on= 0;

   if (argc<2)
   {  while (readln(opgave,kald[1]))
      {  n= 1;
	 printf("%s\n",navn(kald[1]));	// skriv 1' soegeord
	 findlin(kald[1]);
      }
      close(ud);
      close(opgave);
      close(ind);
      close(symbols);
   }
   else
   {  for (i=2;i<=argc;i++)
      {
	 n= 1;
	 kald[1]= argv[i-1];
	 printf("%s\n",navn(kald[1]));
	 findlin(kald[1]);
	 fputc('\n',ud);
      }

    close(ud);
    close(ind);
    close(symbols);
   }
}