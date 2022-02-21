/*
Program til generering af bin‘r prom file til digital audio (AES3-1992)

12NC for PROM: 4008 119 53772

Indg†r i PT8605/901: -14dB er erstattet af -15dB

Hver lyd specificeres med funktionen
   calsound(dBFS-level,Freq Ch-A, Freq Ch-B, SoundMode, BlokSize);
    hvor
      dBFS-level           = 0 .. -40
      Freq Ch-A, Freq Ch-B = 400, 800 eller 1000
      SoundMode		   = StereoMode, DualMode eller MonoMode
      BlokSize		   = Blok4k eller Blok16k

Silence (audio samples = 0) laves ved at s‘tte Freq = 0
Audio samples genereres med 20 bit.

Til inds‘ttelse af 1kb blokke med data = FFh i filen bruges funktionen
 fillFF(Blok)
  hvor
   Blok = antal 1kb blokke med FFh

Til inds‘ttelse af en 4kb blok med data = 00h og FFh til generering af
48 kHz signal bruges funktionen
 make48khz()


Peter Frederiksen, maj 1998
*/


#include <stdio.h>
#include <math.h>
#include <string.h>
#include <conio.h>



void stdimpl(int AudioMode);
void fillFF(long int no);
void make48khz();
int parity(int ch,int no);
float exp10( float y);
int pow2(int y);
unsigned char invertbyte( unsigned char inb);
unsigned char bintobyte(char *binstr);
void calcrc(int ch);
void biphase(int bloksize);
char *strmove(char *instr, char *sout, int st, int no);
void calsound(float  AudioLevel,
	      int    AudioFreq1,
	      int    AudioFreq2,
	      int    AudioMode,
	      int    BlokSize);


const int StereoMode = 0;
      int DualMode = 1;
      int MonoMode = 2;
      int ChA = 0;
      int ChB = 1;
      int NoEmphasis = 1;
      int YesEmphasis = 0;
      int Blok16k = 0;
      int Blok4k = 1;

unsigned char AudioData[2][3840];    // 2 channels (A and B)
				     // 192*5 frames * 4 bytes/frame

int	Emphasis,              // 1=no emphasis   0=emphasis
	AudioWord;             // 20 or 24 bit


FILE *framef,
     *binfile,
     *chstatusfile;


// ===============================================================
void main()
{

 clrscr();
 framef = fopen("fdata.aes","wt");           // text file with audio data/
 chstatusfile = fopen("chstatus.aes","wt");  // text file channel status
 binfile = fopen("aesdata.bin","w");         // binary file with prom data

 calsound(0.0,  1000,1000,StereoMode,Blok4k);
 calsound(-9.0, 1000,1000,StereoMode,Blok4k);
 calsound(-12.0,1000,1000,StereoMode,Blok4k);
 calsound(-15.0,1000,1000,StereoMode,Blok4k);
 calsound(-16.0,1000,1000,StereoMode,Blok4k);
 calsound(-18.0,1000,1000,StereoMode,Blok4k);
 calsound(-20.0,1000,1000,StereoMode,Blok4k);
 calsound(0.0,     0,   0,StereoMode,Blok4k);

 calsound(0.0,     0,1000,StereoMode,Blok4k);     // A=silence
 calsound(-9.0,    0,1000,StereoMode,Blok4k);
 calsound(-12.0,   0,1000,StereoMode,Blok4k);
 calsound(-15.0,   0,1000,StereoMode,Blok4k);
 calsound(-16.0,   0,1000,StereoMode,Blok4k);
 calsound(-18.0,   0,1000,StereoMode,Blok4k);
 calsound(-20.0,   0,1000,StereoMode,Blok4k);
 calsound(0.0,     0,   0,StereoMode,Blok4k);

 calsound(0.0,  1000,   0,StereoMode,Blok4k);     // B=silence
 calsound(-9.0, 1000,   0,StereoMode,Blok4k);
 calsound(-12.0,1000,   0,StereoMode,Blok4k);
 calsound(-15.0,1000,   0,StereoMode,Blok4k);
 calsound(-16.0,1000,   0,StereoMode,Blok4k);
 calsound(-18.0,1000,   0,StereoMode,Blok4k);
 calsound(-20.0,1000,   0,StereoMode,Blok4k);
 calsound(0.0,     0,   0,StereoMode,Blok4k);

 calsound(0.0,  1000,1000,MonoMode,Blok4k);
 calsound(-9.0, 1000,1000,MonoMode,Blok4k);
 calsound(-12.0,1000,1000,MonoMode,Blok4k);
 calsound(-15.0,1000,1000,MonoMode,Blok4k);
 calsound(-16.0,1000,1000,MonoMode,Blok4k);
 calsound(-18.0,1000,1000,MonoMode,Blok4k);
 calsound(-20.0,1000,1000,MonoMode,Blok4k);
 calsound(0.0,     0,   0,MonoMode,Blok4k);

 fillFF(96);
 make48khz();    // 4k blok
 fillFF(28);

 calsound(0.0,  1000, 400,DualMode,Blok16k);
 calsound(-9.0, 1000, 400,DualMode,Blok16k);
 calsound(-12.0,1000, 400,DualMode,Blok16k);
 calsound(-15.0,1000, 400,DualMode,Blok16k);
 calsound(-16.0,1000, 400,DualMode,Blok16k);
 calsound(-18.0,1000, 400,DualMode,Blok16k);
 calsound(-20.0,1000, 400,DualMode,Blok16k);
 calsound(0.0,     0,   0,DualMode,Blok16k);

 calsound(0.0,   800, 800,StereoMode,Blok16k);
 calsound(-9.0,  800, 800,StereoMode,Blok16k);
 calsound(-12.0, 800, 800,StereoMode,Blok16k);
 calsound(-15.0, 800, 800,StereoMode,Blok16k);
 calsound(-16.0, 800, 800,StereoMode,Blok16k);
 calsound(-18.0, 800, 800,StereoMode,Blok16k);
 calsound(-20.0, 800, 800,StereoMode,Blok16k);
 calsound(0.0,     0,   0,StereoMode,Blok16k);

 fclose(framef);
 fclose(binfile);
 fclose(chstatusfile);

} // main ============================================================






// ---------------------------------------------------------------
void calsound(float  AudioLevel,
	      int    AudioFreq1,
	      int    AudioFreq2,
	      int    AudioMode,
	      int    BlokSize)
{
/* 3.1415926535897932385 */

const double pi = 3.14159265358979;
      float MaxAudioLevel = 0.0;        /* dBFS */
      float MinAudioLevel = -40.0;      /* dBFS */
      long int dBFS20bit = 524287;
      long int dBFS24bit = 8388607;

unsigned char b0,b1,b2,b3;


int     SamplesPrPeriod[2],    // 48000 / AudioFreq
	SampleNo;              // 0..959

int     n,
	byteno,
	bitno,
	block;

long int  ToneA,
	  ToneB,
	  dBFSmax;


 AudioWord = 20;               // set audio word length
 Emphasis = NoEmphasis;        // set no emphasis

 for (n = 0; n < 3840; ++n)    /* init audio data */
 {
  AudioData[0][n] = 0;
  AudioData[1][n] = 0;
 }


 switch (AudioFreq1)
  {
  case 1000:  SamplesPrPeriod[ChA] = 48; break;
  case 800:   SamplesPrPeriod[ChA] = 60; break;
  case 400:   SamplesPrPeriod[ChA] = 120; break;
  default:    break;
  }

 switch (AudioFreq2)
  {
  case 1000:  SamplesPrPeriod[ChB] = 48; break;
  case 800:   SamplesPrPeriod[ChB] = 60; break;
  case 400:   SamplesPrPeriod[ChB] = 120; break;
  default:    break;
  }

 if ((AudioWord != 20) && (AudioWord != 24))    /* check valid audio word */
  AudioWord = 20;

 if (AudioLevel > MaxAudioLevel)
  AudioLevel =  MaxAudioLevel;
 if (AudioLevel < MinAudioLevel)
  AudioLevel =  MinAudioLevel;


 if (AudioWord == 24)                   // select max word sample (0 dBFS)
   dBFSmax = dBFS24bit * exp10(AudioLevel / 20.0);
  else
   dBFSmax = dBFS20bit * exp10(AudioLevel / 20.0);

 // printf(" Sample#     Ch A         Ch B \n");

 // calculate audio word in 5x192 frames ( = 960 samples )

 for (SampleNo = 0; SampleNo <= 959; ++SampleNo)
 {
  if (AudioFreq1 != 0)
   ToneA = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChA]);
    else
   ToneA = 0;
  if (AudioWord == 20)
   {
     b0 = 0;
     b1 = invertbyte((ToneA & 0xFF));
     b2 = invertbyte((ToneA >> 8) & 0xFF);
     b3 = invertbyte((ToneA >> 16) & 0xFF);
     b3 = b3 & 0xF0;
     AudioData[ChA][SampleNo*4+0] = b0;
     AudioData[ChA][SampleNo*4+1] = b1;
     AudioData[ChA][SampleNo*4+2] = b2;
     AudioData[ChA][SampleNo*4+3] = b3;
   }

  if (AudioWord == 24)
   {
     b0 = (invertbyte((ToneA & 0x0F)) >> 4);
     b1 = invertbyte((ToneA >> 4) & 0xFF);
     b2 = invertbyte((ToneA >> 12) & 0xFF);
     b3 = invertbyte((ToneA >> 20) & 0xFF);
     b3 = b3 & 0xF0;
     AudioData[ChA][SampleNo*4+0] = b0;
     AudioData[ChA][SampleNo*4+1] = b1;
     AudioData[ChA][SampleNo*4+2] = b2;
     AudioData[ChA][SampleNo*4+3] = b3;
   }

  if (AudioFreq2 != 0)
   ToneB = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChB]);
    else
   ToneB = 0;
  if (AudioWord == 20)
   {
     b0 = 0;
     b1 = invertbyte((ToneB & 0xFF));
     b2 = invertbyte((ToneB >> 8) & 0xFF);
     b3 = invertbyte((ToneB >> 16) & 0xFF);
     b3 = b3 & 0xF0;
     AudioData[ChB][SampleNo*4+0] = b0;
     AudioData[ChB][SampleNo*4+1] = b1;
     AudioData[ChB][SampleNo*4+2] = b2;
     AudioData[ChB][SampleNo*4+3] = b3;
   }

  if (AudioWord == 24)
   {
     b0 = (invertbyte((ToneB & 0x0F)) >> 4);
     b1 = invertbyte((ToneB >> 4) & 0xFF);
     b2 = invertbyte((ToneB >> 12) & 0xFF);
     b3 = invertbyte((ToneB >> 20) & 0xFF);
     b3 = b3 & 0xF0;
     AudioData[ChB][SampleNo*4+0] = b0;
     AudioData[ChB][SampleNo*4+1] = b1;
     AudioData[ChB][SampleNo*4+2] = b2;
     AudioData[ChB][SampleNo*4+3] = b3;
   }

  fprintf(framef,"%8d  %8ld  %8ld\n",SampleNo, ToneA, ToneB);

  if ((SampleNo > 285) & (SampleNo < 196)){
   printf("%8d  %8ld  %8ld",SampleNo, ToneA, ToneB);
   printf("%3d  %3d  %3d   %3d\n",b0,b1,b2,b3);
   }
 }  // for

 stdimpl(AudioMode);

  calcrc(ChA);
  calcrc(ChB);



 for (SampleNo = 0; SampleNo <= 959; ++SampleNo)
  {
   if (parity(ChA, SampleNo) == 0)   // check even parity for ch A
     parity(ChA, SampleNo);          // set even parity for ch A
   if (parity(ChB, SampleNo) == 0)
     parity(ChB, SampleNo);
  }



  biphase(BlokSize);

  printf("Level: %6.1f      Ch-A freq: %5d      Ch-B freq: %5d      Mode: %d\n",
	  AudioLevel,AudioFreq1,AudioFreq2, AudioMode);


}  // calsound


void stdimpl(int AudioMode)
{
int     block,
	byteno,
	bitno;


//  ----------- Standard implementation, see AES3  5.2.2 --------------
//   set channel status channel byte 0 bit 0 = 1  (professional mode)
  byteno = 0;
  bitno = 0;
  for (block = 0; block <= 4; ++block)
   {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 0 bit 2 = 1   (no emphasis)
  byteno = 0;
  bitno = 2;
  if (Emphasis == NoEmphasis)
  for (block = 0; block <= 4; ++block)
   {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 0 bit 7 = 1  (48 kHz sampling freq)
  byteno = 0;
  bitno = 7;
  for (block = 0; block <= 4; ++block)
   {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 1 bit 7 = 1   (192 bit block structure)
  byteno = 1;
  bitno = 7;
  for (block = 0; block <= 4; ++block)
   {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 1 bit 1 = 1   ( stereo )
  byteno = 1;
  bitno = 1;
  if (AudioMode == StereoMode)
  for (block = 0; block <= 4; ++block)
   {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 1 bit 3 = 1   ( dual / two channel )
  byteno = 1;
  bitno = 3;
  if (AudioMode == DualMode)
  for (block = 0; block <= 4; ++block)
   {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 1 bit 2 = 1   ( mono )
  byteno = 1;
  bitno = 2;
  if (AudioMode == MonoMode)
  for (block = 0; block <= 4; ++block)
   {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

/*   set channel status channel byte 2 bit 2 = 1   ( 24 bit audio word ) */
  byteno = 2;
  bitno = 2;
  if (AudioWord == 24)
  for (block = 0; block <= 4; ++block)
  {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
  } /* for */


/*   set channel status channel byte 2 bit 3 = 1 and bit 5 = 1
					    ( word length indicated ) */
  byteno = 2;
  bitno = 3;
  for (block = 0; block <= 4; ++block)
  {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
  } /* for */

  byteno = 2;
  bitno = 5;
  for (block = 0; block <= 4; ++block)
  {
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
  } /* for */

} //stdimpl




unsigned char bintobyte(char *binstr)
{
 //converts a 8 bit binary string to a byte }
 int n;
 unsigned char bb;

 bb = 0;
 for(n = 0; n < 8; ++n)
  {
   if ((binstr[n]-48) == 1)
    bb = bb + pow2(7-n);
  }

 return(bb);
}





void biphase(int bloksize)
// calculates each subframe as a binary string and then codes it into biphase.
// the biphase codes is written to a binary prom file.
{
 int        ch,k,d,no,          // loop counter
	    noofframes,Fr,
	    SubFrame,
	    NoOf8Bytes;
 long int   sf,
	    xx;
 char       Digit[3],       // 2 bit binary digits
	    Frame[33],      // 32 bit binary subframe string
	    BiPhStr[65],    // 64 bit biphase string for 1 subframe
	    prev;
 char	    ef[9];
 char	    *ff = ef;

 char       *Z1 = "11101000";   // preambles
 char	    *X1 = "11100010";
 char	    *Y1 = "11100100";
 char	    *Z0 = "00010111";
 char	    *X0 = "00011101";
 char	    *Y0 = "00011011";
 char	    *Bp00 = "00";       // biphase codes
 char	    *Bp01 = "01";
 char	    *Bp10 = "10";
 char	    *Bp11 = "11";


 prev = '0';
 Fr = 0;
 SubFrame = 1;
 NoOf8Bytes = 0;
 strcpy(Digit,"01");

 if (bloksize == Blok16k)
  noofframes = 960;

 if (bloksize == Blok4k)
  noofframes = 192;


 for(no = 0; no < noofframes; ++no)
  for(ch = 0; ch < 2; ++ch)        // channel A and B
  {
    strcpy(BiPhStr,"");

    // calculate the 32 bit sf from the 4 bytes
    xx = AudioData[ch][no*4+0];
    sf = 0x1000000 * xx;
    xx = AudioData[ch][no*4+1];
    sf = 0x10000 * xx + sf;
    xx = AudioData[ch][no*4+2];
    sf = 0x100 * xx + sf;
    xx = AudioData[ch][no*4+3];
    sf = xx + sf;

    // make 32 bit binary string of sf subframe
      for(d = 31; d >= 0; --d)
       {
	Frame[d] = Digit[sf & 1];
	sf = sf >> 1;
       } // for
      Frame[32] = '\x0';



    // put biphase coding of the 8 bits X,Y or Z preamble into BiPhStr
    if (prev == '0')
       {
	if ((Fr == 0) && (SubFrame == 1))     //  Z
	 strcat(BiPhStr,Z1);
	if ((Fr > 0) && (SubFrame == 1))      //  X
	 strcat(BiPhStr,X1);
	if ((Fr > 0) && (SubFrame == 2))      //  Y
	 strcat(BiPhStr,Y1);
       }
      else
       {
	if ((Fr == 0) && (SubFrame == 1))     //  Z
	 strcat(BiPhStr,Z0);
	if ((Fr > 0) && (SubFrame == 1))      //  X
	 strcat(BiPhStr,X0);
	if ((Fr > 0) && (SubFrame == 2))      //  Y
	 strcat(BiPhStr,Y0);
       } // if


    // biphase coding: 28 bits into BiPhStr
     for (d = 4; d <= 31; ++d)
      {
       switch (prev)
       {
	case '1' : if (Frame[d] == '1')
		     strcat(BiPhStr,Bp01);
		    else
		     strcat(BiPhStr,Bp00); break;

	case '0' : if (Frame[d] == '1')
		     strcat(BiPhStr,Bp10);
		    else
		     strcat(BiPhStr,Bp11); break;
       } // switch
       prev = BiPhStr[strlen(BiPhStr)-1];
      } // for d



    for (k = 0; k < 8; ++k)          // write binary data to PROM file
      {
       strmove(BiPhStr,ff,k*8,8);
       fprintf(binfile,"%c",bintobyte(ff));
      }
    ++NoOf8Bytes;                    // 8 bytes written to binary file


      if (Fr < 383)                      // 192 subframes * 2
	++Fr;
       else
	{
	 Fr = 0;                                  // start preamble Z
	 printf("%s","..  ");
	} // if


    if (SubFrame < 2)                   // toggles between channel A and B
      ++SubFrame;
     else
      SubFrame = 1;


  } // for n

 printf("\n");

 // fill up with \xFF
 for(k = 0; k < 1024; ++k)		// 16kb PROM blok: 2^14 = 16384 bytes
  fprintf(binfile,"%c",'\xff');		// 16384 - 15360 = 1024
					// 4kb PROM blok: 2^12 = 4096 bytes
					// 4096 - 3072 = 1024


} // biphase -------------------------------------------------



void fillFF(long int no)
{
 // fill up prom file with \xFF
 // no = number of 1024 bytes
 long int k;
 no = no * 1024;

 for(k = 0; k < no; ++k)
  fprintf(binfile,"%c",'\xff');

 printf("FFh blok: %dk\n",(no / 1024));
}



void make48khz()
{
 // fill prom file with 48 kHz data (per time = 20.833 us)
 // data uses 4096 bytes in prom
 // 1 periode: 1/6.144e6 * 128 = 20.833us
 int n,k;

 for(n = 0; n < 256; ++n)
 {
   for(k = 0; k < 8; ++k)
    fprintf(binfile,"%c",'\xff');   // write 8 x FFh
   for(k = 0; k < 8; ++k)
    fprintf(binfile,"%c",'\x00');   // write 8 x 00h
  }
 printf("48 kHz\n");
}




void calcrc(int ch)
{
int k,n,
    crc[184],
    Q[8],
    X8;

for (k = 0; k < 5; ++k)                  // calculate crc in 5 blocks
 {
    for (n = 0; n < 8; ++n)              // init the 8 crc registers to 1
     Q[n] = 1;

    for (n = 0; n < 184; ++n)            // get the 184 crc bits in bit C
     crc[n] = ((AudioData[ch][(k*768)+(n*4)+3] & 0x02) >> 1);

    for (n = 0; n < 184; ++n)           // G(x) = x^8 + x^4 + x^3 + x^2 +1
     {                                  // see annex B in AES3-1992
	X8 = (crc[n] ^ Q[7]);
	Q[7] = Q[6];
	Q[6] = Q[5];
	Q[5] = Q[4];
	Q[4] = (Q[3] ^ X8);
	Q[3] = (Q[2] ^ X8);
	Q[2] = (Q[1] ^ X8);
	Q[1] = Q[0];
	Q[0] = X8;
     }

      if (Q[0] == 1)
       AudioData[ch][(k*768)+191*4+3] = AudioData[ch][(k*768)+191*4+3] | 0x02;
      if (Q[1] == 1)
       AudioData[ch][(k*768)+190*4+3] = AudioData[ch][(k*768)+190*4+3] | 0x02;
      if (Q[2] == 1)
       AudioData[ch][(k*768)+189*4+3] = AudioData[ch][(k*768)+189*4+3] | 0x02;
      if (Q[3] == 1)
       AudioData[ch][(k*768)+188*4+3] = AudioData[ch][(k*768)+188*4+3] | 0x02;
      if (Q[4] == 1)
       AudioData[ch][(k*768)+187*4+3] = AudioData[ch][(k*768)+187*4+3] | 0x02;
      if (Q[5] == 1)
       AudioData[ch][(k*768)+186*4+3] = AudioData[ch][(k*768)+186*4+3] | 0x02;
      if (Q[6] == 1)
       AudioData[ch][(k*768)+185*4+3] = AudioData[ch][(k*768)+185*4+3] | 0x02;
      if (Q[7] == 1)
       AudioData[ch][(k*768)+184*4+3] = AudioData[ch][(k*768)+184*4+3] | 0x02;


     if ((k == 0) && (ch == 0))
     {
  //   fprintf(chstatusfile,"Level: %6.1f      Ch-A freq: %5d      Ch-B freq: %5d      Mode: %d\n",
  //          AudioLevel,AudioFreq1,AudioFreq2, AudioMode);
      for (n = 0; n < 192; ++n)
       {
	fprintf(chstatusfile,"%d",(AudioData[ch][(k*768)+n*4+3] & 0x02) >> 1);

	if ((((n+1) % 8) == 0) && (n > 0))
	 fprintf(chstatusfile,"\n");
       }
      fprintf(chstatusfile,"\n");

     } // if

 } // for (k..

} // calcrc








char *strmove(char *instr, char *sout, int st, int no)
{
 int k;

 k = 0;
 while((instr[k] != '\0') && (k <= no))
   {
     sout[k] = instr[k+st];
     k++;
   }
 sout[k-1] = '\0';
 return(sout);
}







int parity(int ch, int no)
{
 int        d,          // loop counter
	    p,          // counts number of '1'
	    EvenPar;    // 1=even parity   0=odd parity
 long int   SubFrame,
	    xx;
 char       Digit[3],   // 2 binary digits
	    Frame[33];  // 32 bit binary subframe string
// char	    ef[33];
// char	    *ff = ef;


 // calculate the 32 bit subframe from the 4 bytes
 xx = AudioData[ch][no*4+0];
 SubFrame = 0x1000000 * xx;
 xx = AudioData[ch][no*4+1];
 SubFrame = 0x10000 * xx + SubFrame;
 xx = AudioData[ch][no*4+2];
 SubFrame = 0x100 * xx + SubFrame;
 xx = AudioData[ch][no*4+3];
 SubFrame = xx + SubFrame;


 p = 0;
 strcpy(Digit,"01");
 for(d = 31; d >= 0; --d)
  {
   Frame[d] = Digit[SubFrame & 1];
   if (Frame[d] == '1')
    ++p;                       // counts number of '1'
   SubFrame = SubFrame >> 1;
  } // for
 Frame[32] = '\x0';


 if ((p % 2) == 0)
  {
   EvenPar = 1;
   /*
   printf("%s ",strmove(Frame,ff,0,4));
   printf("%s ",strmove(Frame,ff,4,24));
   printf("%s ",strmove(Frame,ff,28,4));
   printf("  %d  %d\n",ch,no);
   */
  }
  else
  {
   EvenPar = 0;
   ++AudioData[ch][no*4+3];  // set the parity bit P (frame bit 0)
  }

 return(EvenPar);
} // parity


int pow2(int y)
// beregner 2^y
{
 int x,n;
 x = 1;

 for(n = 0; n < y ; ++n)
  x = x * 2;

 return(x);
}



float exp10(float y)
{
float r;

r = y * log(10.0);
return(exp(r));
}




unsigned char invertbyte( unsigned char inb)
// invert the 8 bits in a byte
{
unsigned char bb;
bb = 0;
 if ((inb & 0x01) == 0x01)
  bb = bb + 0x80;
 if ((inb & 0x02) == 0x02)
  bb = bb + 0x40;
 if ((inb & 0x04) == 0x04)
  bb = bb + 0x20;
 if ((inb & 0x08) == 0x08)
  bb = bb + 0x10;
 if ((inb & 0x10) == 0x10)
  bb = bb + 0x08;
 if ((inb & 0x20) == 0x20)
  bb = bb + 0x04;
 if ((inb & 0x40) == 0x40)
  bb = bb + 0x02;
 if ((inb & 0x80) == 0x80)
  bb = bb + 0x01;

return(bb);
}

