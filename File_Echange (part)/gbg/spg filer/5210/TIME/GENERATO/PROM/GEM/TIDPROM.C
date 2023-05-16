/* PROGRAM tidprom.C
   laver prom til VITC-kode generator */

#include <stdio.h>

#define UC	unsigned char
#define UI	unsigned int
#define UL	unsigned long

FILE *fil, *udfil;
UC buf[4500], cbuf, i;
UC crc, tel;


UI filend;
UI frame;		   // õnsket max. antal samples pr. bid
long dti, dtu, rate;   	   // antal samples pr. input, output -bid
UC udelad;                 // flag for bortkastning af samples
/* *************************************************************** */
void syntaxvis(void)
{ printf("SYNTAX: SPEED <filnavn> [<speed> [<inddeling>]]\n");
  printf("  filnavn alene giver oplysninger om format\n");
  printf("  speed: 50..300% (normal=100)\n");
  printf("  inddeling: 200..4000 samples (normal=1600)\n");
  printf("  OBS! kun til MONO, 8 bit WAV-filer\n");
}
/* *************************************************************** */
UI lesnul(UI antal)
{ UI leste;
  if ((leste=fread(buf,1,antal,fil)) != antal)  filend= 1;
  while (!filend && abs(buf[leste-1]-0x80) > 5)
  { if (fread(buf+leste,1,1,fil) != 1)  filend= 1;
    else leste++;
  }
  return(leste);
}
/* *************************************************************** */
void tidset(UC samp)	// sët antal samples pr. bid
{ long fakt;		// samp er HI(samplerate) = 86 v. 22kHz
  fakt= rate/256;
  fakt= frame*(long)samp/fakt;
  if (dtu>=100)
  { dti= frame;    	// hurtigere
    dtu= fakt*100/dtu; udelad=1;
  }
  else
  { dtu= fakt -fakt* dtu / 100;	// langsommere
    dti= fakt - 2*dtu;
  }
}



/* *************************************************************** */

void crccalc(UC c)
{  if ((c==1 && (crc & 128)==128) || (c==0 && (crc & 128)==0))
   crc <<= 1;
   else { crc <<= 1; crc |= 1; }
   // printf("%d ",crc);
   tel+=1;
}
/* *************************************************************** */

void skrivnul(void)
{  UC c;
   c= 0xfe;
   fwrite(&c,1,1,udfil);
}
/* *************************************************************** */

void skrivud(UC c)
{
   buf[0]= 0xff; buf[1]= 0xfe;		// skriv sync = '10'
   fwrite(buf,2,1,udfil);
   crccalc(1); crccalc(0);
   for (i=0;i<8;i++)
   {  if (c & 1) {  buf[0]= 0xff; crccalc(1); }
      else  { buf[0]= 0xfe; crccalc(0); }
      fwrite(buf,1,1,udfil);
      c >>= 1;
   }
}
/* *************************************************************** */

void skrivcrc(void)
{  UC i, c;
   buf[0]= 0xff; buf[1]= 0xfe;		// skriv sync = '10'
   fwrite(buf,2,1,udfil);
   crccalc(1); crccalc(0);
   c= crc;
   for (i=0;i<8;i++)
   {  if (c & 128) {  buf[0]= 0xff; crccalc(1); }
      else  { buf[0]= 0xfe; crccalc(0); }
      fwrite(buf,1,1,udfil);
      c <<= 1;
   }
   if (crc!=0) 	{ printf("CRC-fejl\n"); exit(1); }
}
/* *************************************************************** */

void skrivfyld(void)
{  UC i;
   for (i=0;i<32;i++)  buf[i]= 0xff;
   fwrite(buf,32,1,udfil);
}
/* *************************************************************** */

int main(int argc, char *argv[])
{  char *filnavn= "ind.wav";
   char *udfilnavn= "tidgener.bin";

   UC frl, frh, secl, sech;	// reset ur
   UC minl, minh, hrl, hrh;


   char *t;    			// til input argument
   char oplys;                	// flag for syntax oplysning
   int gl1, gl2, ny1, ny2;      // samples
   long sum;			// antal skrevne samples
   long ti, tu;			// aktuel position i input, output fil
   long *bufptr, nyleng;        // til udregning af fillëngde
   UC ste, wide;		// mono/stereo og 8/16 bit
   UI lest;                     // antal byte lëst fra input
   frame= 1600;
   dti= 1600; dtu= 100;		// default opbrydning

   oplys= 0; udelad= 0; tel= 0;
/*
   switch (argc)
   { case 1: syntaxvis(); exit(1); break;
     case 2: filnavn= argv[1]; oplys= 1; break;
     case 3: case 4: filnavn= argv[1];
	     // lës speed % og udregn antal samples
	     // der skal gentages eller udelades (dtu);
	     dtu= 0;
	     t= argv[2];
	     while (*t != '\0')  dtu = 10* dtu + *t++ -'0';
	     if (dtu<50)
		{ printf("fejl: speed skal vëre >= 50\n"); exit(1);}
	     if (argc==4)
	     { frame= 0; t= argv[3];
	       while (*t != '\0')  frame = 10* frame + *t++ -'0';
	       if (frame<200 || frame>4000)
		{ printf("fejl: inddeling skal vëre mellem 200 og 4000"); exit(1);}
	     }
	     break;
     default: syntaxvis(); exit(1); break;
   }
   if ((fil= fopen(filnavn,"rb"))==NULL)
	{ printf("Error Opening File\n"); exit(1); }

    bufptr= (long *)buf;

    // TEST FOR GYLDIG (WAVE) FIL
    if (fread(buf,44,1,fil) == 0)
	{ printf("End of file fundet"); filend= 1;}
    if (buf[8]!='W' || buf[9]!='A' || buf[10]!='V' || buf[11]!='E')
	{ printf("Fejl - ikke en Wave-fil\n"); fclose(fil); exit(1); }
    if (buf[22]!=1 || buf[34]!=8)
	{ printf("Fejl - ikke en MONO, 8 bit-fil\n"); fclose(fil); exit(1); }
    rewind(fil);	// spol tilbage
*/


   // èBEN UDFIL
   if (!oplys)
   if ((udfil= fopen(udfilnavn,"wb"))==NULL)
	{  printf("Error Opening File\n"); exit(1); }

   frame = 0;

   frl= 4; frh= 2; secl= 0; sech= 0;	// set ur
   minl= 4; minh= 3; hrl= 2; hrh= 1;

   while (frame<2048)
   {
	frl += 1;
	if (frh>=2 && frl>=5)
	{  frh= 0; frl= 0; secl += 1;
	   printf("\n");
	}
	else if (frl==10)
	{  frl= 0; frh += 1;
	}
	if (secl==10)
	{  sech += 1; secl= 0;
	}
	if (sech==6)
	{  sech=0; minl += 1;
	}
	if (minl==10)
	{  minh += 1; minl= 0;
	}
	if (minh==6)
	{  minh=0; hrl += 1;
	}
	if (hrh==2 && hrl==4)
	{  hrh= 0; hrl= 0;
	}
	else if (hrl==10)
	{  hrh += 1; hrl= 0;
	}

// SKRIV UD
	crc= 0;
	skrivnul();				// byte 0
	skrivud(frl);	// skriv sync + frame, low	1-11
	skrivud(frh);	// skriv sync + frame, high	11-20
	skrivud(secl);	// skriv sync + sekund, low	21-30
	skrivud(sech);	// skriv sync + sekund, high	31-40
	skrivud(minl);	// skriv sync + minut, low	41-50
	skrivud(minh);	// skriv sync + minut, high	51-60
	skrivud(hrl);	// skriv sync + time, low	61-70
	skrivud(hrh);	// skriv sync + time, high	71-80
	printf("%d%d%d%d%d%d%d%d  ",hrh,hrl,minh,minl,sech,secl,frh,frl);
	skrivcrc();	// skriv sync + CRC		81-90
	skrivnul();				// byte 91
	skrivnul();				//	92
	skrivnul();				//	93
	skrivnul();				//	94
	skrivnul();				//	95
	skrivfyld();	// fyld med FF til 128 bytes	96-127
	frame += 1;
   }

   fclose(udfil);
//   fclose(fil);

   return 0;
}
/* *************************************************************** */
