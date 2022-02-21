#include <ansi_c.h>
/*
Program til generering af binær prom file til digital audio (AES3-1992) 
samt analog audio data til audio-dac CS4340
Compiler: LabWindows V4.0.1

12NC for Audio data 1 PROM: 4008 002 07290
12NC for Audio data 2 PROM: 4008 002 07300

Indgår i audio generator i PT5201.

Hver AES-lyd specificeres med funktionen
   calAESsound(dBFS-level,Freq Ch-A, Freq Ch-B, SoundMode, BlokSize);
    hvor
      dBFS-level     = 0 .. -40
      Freq Ch-A, Freq Ch-B = 0, 500, 1000 eller 8000
      SoundMode		= StereoMode, DualMode eller MonoMode
      SampleFreq     = SF4800 eller SF44100 
      BlokSize		   = Blok4k, Blok16k eller Blok32k

Silence (audio samples = 0) laves ved at sætte Freq = 0
Audio samples genereres med 20 bit.


Hver analog-lyd specificeres med funktionen
 void calANALOGsound(float  AudioLevel,
                       int  AudioFreq1,
                       int  AudioFreq2,
                       int  SampleFreq);
     hvor
      AudioLevel = 0 .. -48
      AudioFreqx = 0, 500, 1000 eller 8000
      AampleFreq = SF48000 eller SF44100


Til indsættelse af 1kb blokke med data = FFh i filen bruges funktionen
 fillFF(Blok)
  hvor
   Blok = antal 1kb blokke med FFh

sample=   0          0        0    0    0    0    0
sample=   0          0        0    0    0    0    0
sample=   1    85F2000     85F2    8   5F   32    0
sample=   1    85F2000     85F2    8   5F    2    0
sample=   2   10B51000    10B51   10   B5   16    0
sample=   2   10B51000    10B51   10   B5    1    0
sample=   3   18F8B000    18F8B   18   F8  176    0
sample=   3   18F8B000    18F8B   18   F8   11    0
sample=   4   2120F000    2120F   21   20  240    0
sample=   4   2120F000    2120F   21   20   15    0
sample=   5   2924E000    2924E   29   24  224    0

Peter Frederiksen, dec 1999



Rettelser:
000509   Beregning af sampleværdier for 8kHz er forskubbet pi/2(=90 grader) for at 8kHz signalet
			samples i peak'en. Ellers vises max peak på TEK764 forkert (0dBFS vises som -1.2dBFS).
			500 Hz og 1000 Hz er ikke forskudt pi/2. Sinuskurven for 1000 Hz skal starte og stoppe 
			i nulgennemgangen af hensyn til start og stop af klik.
000512   Levels for analog audio fastlagt efter aftale med HCM (og efter snak med Michael Brandes, DR)
			
*/


#include <utility.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


/* pi = 3.1415926535897932385 */
const double pi = 3.14159265358979;
const long int dBFS20bit = 524287;
const long int dBFS24bit = 8388607;
const int AudioWord = 20;               // set analog audio word length

const float MaxAudioLevel =    0.0;     /* dBFS */
const float MinAudioLevel = -130.0;     /* dBFS */

// Analog level definitions
const double AL[32] = {0.0, -2.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10.0, -11.0,
							-12.0, -13.0, -14.0, -15.0, -16.0, -17.0, -18.0, -19.0, -20.0, -21.0,
							-22.0, -23.0, -24.0, -27.0, -30.0, -33.0, -36.0, -39.0, -42.0, -45.0, -48.0, 0.0};

void stdimpl(int AudioMode, int sampfreq);
void fillFF(long int no);

int parity(int ch,int no);
float exp10( float y);
int pow2(int y);
unsigned char invertbyte( unsigned char inb);
unsigned char bintobyte(char *binstr);
void calcrc(int ch);
void biphase(int bloksize);
char *strmove(char *instr, char *sout, int st, int no);
void calAESsound(float  AudioLevel,
	      int    AudioFreq1,
	      int    AudioFreq2,
	      int    AudioMode,
	      int	 SampleFreq,
	      int    BlokSize);
void calANALOGsound(float  AudioLevel,
                      int  AudioFreq1,
                      int  AudioFreq2,
                      int  SampleFreq);


const int StereoMode = 0;
      int DualMode = 1;
      int MonoMode = 2;
      int ChA = 0;
      int ChB = 1;
      int NoEmphasis = 1;
      int YesEmphasis = 0;
      int Blok4k = 0;
      int Blok16k = 2;
      int Blok32k = 1;
      int SF48000 = 48000;
      int SF44100 = 44100;
      
unsigned char AudioData[2][4608];  // 2 channels (A and B)
                                   // 192*6 frames * 4 bytes/frame

int	Emphasis;         // 1=no emphasis   0=emphasis



FILE *framef,
     *binfile,
     *chstatusfile;


// ===============================================================
void main(){
int n;

 Cls();
 

 framef = fopen("fdata1.aes","wt");           // text file with audio data/
 chstatusfile = fopen("c_bit1.aes","wt");     // text file channel status
 binfile = fopen ("aesdata1.bin", "wb");         // binary file with prom data

 calAESsound(  0.0,  500,500,StereoMode,SF48000,Blok4k);
 calAESsound( -9.0,  500,500,StereoMode,SF48000,Blok4k);
 calAESsound(-12.0,  500,500,StereoMode,SF48000,Blok4k);
 calAESsound(-15.0,  500,500,StereoMode,SF48000,Blok4k);
 calAESsound(-16.0,  500,500,StereoMode,SF48000,Blok4k);
 calAESsound(-18.0,  500,500,StereoMode,SF48000,Blok4k);
 calAESsound(-21.0,  500,500,StereoMode,SF48000,Blok4k);
 calAESsound(  0.0,    0,  0,StereoMode,SF48000,Blok4k);

 // address = 32k = 8000h
 calAESsound(  0.0, 1000,1000,StereoMode,SF48000,Blok4k);
 calAESsound( -9.0, 1000,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-12.0, 1000,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-15.0, 1000,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-16.0, 1000,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-18.0, 1000,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-21.0, 1000,1000,StereoMode,SF48000,Blok4k);
 calAESsound(  0.0,    0,   0,StereoMode,SF48000,Blok4k);

 // address = 64k = 10000h
 calAESsound(  0.0,    0,1000,StereoMode,SF48000,Blok4k);  // silence in ch-A
 calAESsound( -9.0,    0,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-12.0,    0,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-15.0,    0,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-16.0,    0,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-18.0,    0,1000,StereoMode,SF48000,Blok4k);
 calAESsound(-21.0,    0,1000,StereoMode,SF48000,Blok4k);
 calAESsound(  0.0,    0,   0,StereoMode,SF48000,Blok4k);

 // address = 96k = 18000h
 calAESsound(  0.0, 8000,8000,StereoMode,SF48000,Blok4k);
 calAESsound( -9.0, 8000,8000,StereoMode,SF48000,Blok4k);
 calAESsound(-12.0, 8000,8000,StereoMode,SF48000,Blok4k);
 calAESsound(-15.0, 8000,8000,StereoMode,SF48000,Blok4k);
 calAESsound(-16.0, 8000,8000,StereoMode,SF48000,Blok4k);
 calAESsound(-18.0, 8000,8000,StereoMode,SF48000,Blok4k);
 calAESsound(-21.0, 8000,8000,StereoMode,SF48000,Blok4k);
 calAESsound(  0.0,    0,   0,StereoMode,SF48000,Blok4k);

 // address = 128k = 20000h
 calAESsound(  0.0,  8000,8000,StereoMode,SF44100,Blok4k);
 calAESsound( -9.0,  8000,8000,StereoMode,SF44100,Blok4k);
 calAESsound(-12.0,  8000,8000,StereoMode,SF44100,Blok4k);
 calAESsound(-15.0,  8000,8000,StereoMode,SF44100,Blok4k);
 calAESsound(-16.0,  8000,8000,StereoMode,SF44100,Blok4k);
 calAESsound(-18.0,  8000,8000,StereoMode,SF44100,Blok4k);
 calAESsound(-21.0,  8000,8000,StereoMode,SF44100,Blok4k);
 calAESsound(  0.0,  8000,8000,StereoMode,SF44100,Blok4k);

 // address = 160k = 28000h
 fillFF(96);
 // analog audio starts at address = 256k = 40000h

 n=0;
 calANALOGsound(AL[n++], 500, 500,SF48000);		// +12dB
 calANALOGsound(AL[n++], 500, 500,SF48000);		// +10dB
 calANALOGsound(AL[n++], 500, 500,SF48000);		// +8dB
 calANALOGsound(AL[n++], 500, 500,SF48000);		// +7dB
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);		// 0dB = 775mV
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);		// -11dB
 calANALOGsound(AL[n++], 500, 500,SF48000);		// -12dB
 calANALOGsound(AL[n++], 500, 500,SF48000);		// -15dB
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);
 calANALOGsound(AL[n++], 500, 500,SF48000);		// -36dB
 calANALOGsound(AL[n++],   0,   0,SF48000);


 // address = 288k = 48000h
 n=0;
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// +12dB
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// +10dB
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// +8dB
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// +7dB
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// 0dB = 775mV
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// -11dB
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// -12dB
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// -15dB
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);
 calANALOGsound(AL[n++], 1000, 1000,SF48000);		// -36dB
 calANALOGsound(AL[n++],    0,    0,SF48000);

 
 // address = 320k = 50000h				     silence in ch-A
 n=0;
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// +12dB
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// +10dB
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// +8dB
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// +7dB
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// 0dB = 775mV
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// -11dB
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// -12dB
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// -15dB
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);
 calANALOGsound(AL[n++],    0, 1000,SF48000);		// -36dB
 calANALOGsound(AL[n++],    0,    0,SF48000);

 
 // address = 352k = 58000h
 n=0;
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],8000,8000,SF48000);
 calANALOGsound(AL[n++],   0,   0,SF48000);

 // address = 384k = 60000h
 
 n=0;
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++], 500, 500,SF44100);
 calANALOGsound(AL[n++],   0,   0,SF44100);


 // address = 416k = 68000h
 n=0;
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],1000,1000,SF44100);
 calANALOGsound(AL[n++],   0,   0,SF44100);

 // address = 448k = 70000h

 n=0;
 calANALOGsound(AL[n++],   0,1000,SF44100);  // silence in ch-A
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,1000,SF44100);
 calANALOGsound(AL[n++],   0,   0,SF44100);

 // address = 480k = 78000h

 n=0;
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],8000,8000,SF44100);
 calANALOGsound(AL[n++],   0,   0,SF44100);
 // last address = 512k = 80000h

 fclose(framef);
 fclose(binfile);
 fclose(chstatusfile);
 
 //exit(0);


 framef = fopen("fdata2.aes","wt");           // text file with audio data/
 chstatusfile = fopen("c_bit2.aes","wt");     // text file channel status
 binfile = fopen("aesdata2.bin","w");         // binary file with prom data

 calAESsound(  0.0, 500, 500,StereoMode,SF44100,Blok32k);
 calAESsound( -9.0, 500, 500,StereoMode,SF44100,Blok32k);
 calAESsound(-12.0, 500, 500,StereoMode,SF44100,Blok32k);
 calAESsound(-15.0, 500, 500,StereoMode,SF44100,Blok32k);
 calAESsound(-16.0, 500, 500,StereoMode,SF44100,Blok32k);
 calAESsound(-18.0, 500, 500,StereoMode,SF44100,Blok32k);
 calAESsound(-21.0, 500, 500,StereoMode,SF44100,Blok32k);
 calAESsound(  0.0,   0,   0,StereoMode,SF44100,Blok32k);

 // address = 256k = 40000h
 calAESsound(  0.0, 1000,1000,StereoMode,SF44100,Blok16k);
 calAESsound( -9.0, 1000,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-12.0, 1000,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-15.0, 1000,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-16.0, 1000,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-18.0, 1000,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-21.0, 1000,1000,StereoMode,SF44100,Blok16k);
 calAESsound(  0.0,    0,   0,StereoMode,SF44100,Blok16k);
 
 // address = 384k = 60000h
 calAESsound(  0.0,    0,1000,StereoMode,SF44100,Blok16k);  // silence in ch-A
 calAESsound( -9.0,    0,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-12.0,    0,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-15.0,    0,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-16.0,    0,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-18.0,    0,1000,StereoMode,SF44100,Blok16k);
 calAESsound(-21.0,    0,1000,StereoMode,SF44100,Blok16k);
 calAESsound(  0.0,    0,   0,StereoMode,SF44100,Blok16k);
 
 // address = 512k = 80000h
 fclose(framef);
 fclose(binfile);
 fclose(chstatusfile);


} // end main 




// ---------------------------------------------------------------
void calAESsound(float  AudioLevel,
                   int  AudioFreq1,
                   int  AudioFreq2,
                   int  AudioMode,
                   int  SampleFreq,
                   int  BlokSize)
{
unsigned char b0,b1,b2,b3;


double SamplesPrPeriod[2];  // SampleFreq / AudioFreq
int	SampleNo;               // 0..1151 = 6x192

int     n,
	byteno,
	bitno,
	block;

long int  ToneA,
	  ToneB,
	  dBFSmax;


 Emphasis = NoEmphasis;        // set no emphasis

 for (n = 0; n < 4608; ++n){    /* init audio data in 6 blocks*/
  AudioData[0][n] = 0;
  AudioData[1][n] = 0;
 }


 if (SampleFreq == SF48000){
  switch (AudioFreq1){
   case 500:  SamplesPrPeriod[ChA] = 96; break;
   case 1000: SamplesPrPeriod[ChA] = 48; break;
   case 8000: SamplesPrPeriod[ChA] = 6; break;
   default:    break;
   }

  switch (AudioFreq2){
   case 500:  SamplesPrPeriod[ChB] = 96; break;
   case 1000: SamplesPrPeriod[ChB] = 48; break;
   case 8000: SamplesPrPeriod[ChB] = 6; break;
   default:    break;
   }
 } // if

 
 if (SampleFreq == SF44100){
  switch (AudioFreq1){
   case 500:  SamplesPrPeriod[ChA] = 88.6153846154; break;
   case 1000: SamplesPrPeriod[ChA] = 44.3076923077; break;
   case 8000: SamplesPrPeriod[ChA] =  5.4857142857; break;
   default:    break;
   }

  switch (AudioFreq2){
   case 500:  SamplesPrPeriod[ChB] = 88.6153846154; break;
   case 1000: SamplesPrPeriod[ChB] = 44.3076923077; break;
   case 8000: SamplesPrPeriod[ChB] =  5.4857142857; break;
   default:    break;
   }
 } // if


 if (AudioLevel > MaxAudioLevel)
  AudioLevel =  MaxAudioLevel;
 if (AudioLevel < MinAudioLevel)
  AudioLevel =  MinAudioLevel;


 if (AudioWord == 24)                   // select max word sample (0 dBFS)
   dBFSmax = dBFS24bit * exp10(AudioLevel / 20.0);
  else
   dBFSmax = dBFS20bit * exp10(AudioLevel / 20.0);

 // printf(" Sample#     Ch A         Ch B \n");

 // calculate audio word in 6x192 frames ( = 1152 samples )

 for (SampleNo = 0; SampleNo <= 1151; ++SampleNo)
 {
  if (AudioFreq1 != 0){
     if (AudioFreq1 == 8000)
      ToneA = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChA] + (pi / 2));
     else
      ToneA = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChA]);
    } 
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

  if (AudioFreq2 != 0){
     if (AudioFreq2 == 8000)
      ToneB = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChB] + (pi / 2));
     else
      ToneB = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChB]);
    } 
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

//  fprintf(framef,"%8d  %8ld  %8ld\n",SampleNo, ToneA, ToneB);

  if ((SampleNo > 285) && (SampleNo < 196)){
   printf("%8d  %8ld  %8ld",SampleNo, ToneA, ToneB);
   printf("%3d  %3d  %3d   %3d\n",b0,b1,b2,b3);
   }
 }  // for

 stdimpl(AudioMode,SampleFreq);

  calcrc(ChA);
  calcrc(ChB);



 for (SampleNo = 0; SampleNo <= 1151; ++SampleNo)
  {
   if (parity(ChA, SampleNo) == 0)   // check even parity for ch A
     parity(ChA, SampleNo);          // set even parity for ch A
   if (parity(ChB, SampleNo) == 0)
     parity(ChB, SampleNo);
  }



  biphase(BlokSize);

  printf("Level: %6.1f   Freq: %5d,%5d  Mode: %d   SF:%5d\n",
	  AudioLevel,AudioFreq1,AudioFreq2, AudioMode, SampleFreq);


}  // calAESsound




void calANALOGsound(float  AudioLevel,
                      int  AudioFreq1,
                      int  AudioFreq2,
                      int  SampleFreq)
{

unsigned char b0,b1,b2,b3;


double SamplesPrPeriod[2]; // SampleFreq(=48000) / AudioFreq
int	SampleNo;              // 0..1151

int n,NoOfSamples,
	block;

long int ToneA,A,
         ToneB,B,
         dBFSmax;


 for (n = 0; n < 96; ++n){    // init analog audio data
  AudioData[0][n] = 0;
  AudioData[1][n] = 0;
 }

 if (SampleFreq == SF48000){
  NoOfSamples = 96;
  switch (AudioFreq1){									 // 1/48000 = 20.83333us
   case 500:  SamplesPrPeriod[ChA] = 96; break;  //  96 x 20.83333s = 2ms = 500Hz
   case 1000: SamplesPrPeriod[ChA] = 48; break;  //  48 x 20.83333s = 1ms = 1000Hz
   case 8000: SamplesPrPeriod[ChA] = 6; break;	 //  6 x 20.83333s = 125us = 8000Hz
   default:    break;
   }

  switch (AudioFreq2){
   case 500:  SamplesPrPeriod[ChB] = 96; break;
   case 1000: SamplesPrPeriod[ChB] = 48; break;
   case 8000: SamplesPrPeriod[ChB] = 6; break;
   default:    break;
   }
 }  

 if (SampleFreq == SF44100){  // 1 sample = 22.67574us @ 44.1kHz,
  NoOfSamples = 88;
  switch (AudioFreq1){									  // 1/44100 = 22.6757us
   case 500:  SamplesPrPeriod[ChA] = 88; break;   //  88/1	    88 x 22.67574us = 1.995465ms = 501.136Hz
   case 1000: SamplesPrPeriod[ChA] = 44; break;   //  88/2	    44 x 22.67574us = 0.9977324ms = 1002.273Hz
   case 8000: SamplesPrPeriod[ChA] = 5.5; break;  //  88/16	    5.5x 22.67574us = 124.7166us = 8018.18Hz
   default:    break;
   }

  switch (AudioFreq2){
   case 500:  SamplesPrPeriod[ChB] = 88; break;
   case 1000: SamplesPrPeriod[ChB] = 44; break;
   case 8000: SamplesPrPeriod[ChB] = 5.5; break;
   default:    break;
   }
 }  

 if (AudioLevel > MaxAudioLevel)
  AudioLevel =  MaxAudioLevel;
 if (AudioLevel < MinAudioLevel)
  AudioLevel =  MinAudioLevel;


 if (AudioWord == 24)                   // select max word sample (0 dBFS)
   dBFSmax = dBFS24bit * exp10(AudioLevel / 20.0);
  else
   dBFSmax = dBFS20bit * exp10(AudioLevel / 20.0);

 for (SampleNo = 0; SampleNo < NoOfSamples; ++SampleNo){
  if (AudioFreq1 != 0){
     if (AudioFreq1 == 8000)
       ToneA = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChA] + (pi / 2));
      else 
       ToneA = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChA]);
    } 
    else
   ToneA = 0;
   
   if (AudioWord == 20){
    A = ToneA << 12; 			// MSB left channel
	b0 = A >> 24;
    b1 = (A >> 16) & 0x00FF;
    b2 = (A >> 8) & 0x0000FF;
    b3 = 0;
   }
   AudioData[ChA][SampleNo*4+0] = b0;
   AudioData[ChA][SampleNo*4+1] = b1;				   
   AudioData[ChA][SampleNo*4+2] = b2;
   AudioData[ChA][SampleNo*4+3] = b3;
// if (SampleNo >  45)
//   printf("sample=%4d %10X %8d %8X %4X %4X %4X %4X\n", SampleNo,A,ToneA,ToneA,b0,b1,b2,b3);
 
  if (AudioFreq2 != 0){
     if (AudioFreq1 == 8000)
       ToneB = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChB] + (pi / 2));
      else 
       ToneB = dBFSmax * sin((2.0 * pi * SampleNo) / SamplesPrPeriod[ChB]);
    }   
    else
   ToneB = 0;

   if (AudioWord == 20){
	B = ToneB << 12; 			// MSB right channel
	b0 = B >> 24;
    b1 = (B >> 16) & 0x00FF;
    b2 = (B >> 8) & 0x0000FF;
    b3 = 0;
   }
   AudioData[ChB][SampleNo*4+0] = b0;
   AudioData[ChB][SampleNo*4+1] = b1;
   AudioData[ChB][SampleNo*4+2] = b2;
   AudioData[ChB][SampleNo*4+3] = b3;


//  fprintf(framef,"%8d  %8ld  %8ld\n",SampleNo, ToneA, ToneB);
 
 } // for


 // only valid for block size = 1024 bytes
 // NoOfSamples x 8 = 768 bytes @ 48000
 // NoOfSamples x 8 = 704 bytes @ 44100
 for (SampleNo = 0; SampleNo < NoOfSamples; ++SampleNo){
   fprintf(binfile,"%c",AudioData[ChA][SampleNo*4+0]);
   fprintf(binfile,"%c",AudioData[ChA][SampleNo*4+1]);
   fprintf(binfile,"%c",AudioData[ChA][SampleNo*4+2]);
   fprintf(binfile,"%c",AudioData[ChA][SampleNo*4+3]);
   
   fprintf(binfile,"%c",AudioData[ChB][SampleNo*4+0]);
   fprintf(binfile,"%c",AudioData[ChB][SampleNo*4+1]);
   fprintf(binfile,"%c",AudioData[ChB][SampleNo*4+2]);
   fprintf(binfile,"%c",AudioData[ChB][SampleNo*4+3]);
 }
 

 // only valid for block size = 1024 bytes
 // 48000: 1024 - 768 = 256  (NoOfSamples * 8 = 768)
 // 44100: 1024 - 704 = 320  (NoOfSamples * 8 = 704)
 for(n = 0; n < (1024-(NoOfSamples*8)); ++n)
  fprintf(binfile,"%c",'\xff');


 printf("Level: %6.1f   Freq: %5d,%5d  Mode: analog @%d\n",AudioLevel,AudioFreq1,AudioFreq2,SampleFreq);

} // calANALOGsound





void stdimpl(int AudioMode, int sampfreq){
int     block,
	byteno,
	bitno;
//  ----------- Standard implementation, see AES3  5.2.2 --------------
//   set channel status channel byte 0 bit 0 = 1  (professional mode)
  byteno = 0;
  bitno = 0;
  for (block = 0; block <= 5; ++block){  // 6 blocks = 6x192 = 1152 samples
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 0 bit 2 = 1   (no emphasis)
  byteno = 0;
  bitno = 2;
  if (Emphasis == NoEmphasis)
  for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 0 bit 7 = 1  (48 kHz sampling freq)
  if (sampfreq == SF48000){
   byteno = 0;
   bitno = 7;
   for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for
  } // if

//   set channel status channel byte 0 bit 6 = 1  (44.1 kHz sampling freq)
  if (sampfreq == SF44100){
   byteno = 0;
   bitno = 6;
   for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for
  } // if

//   set channel status channel byte 1 bit 7 = 1   (192 bit block structure)
  byteno = 1;
  bitno = 7;
  for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 1 bit 1 = 1   ( stereo )
  byteno = 1;
  bitno = 1;
  if (AudioMode == StereoMode)
  for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 1 bit 3 = 1   ( dual / two channel )
  byteno = 1;
  bitno = 3;
  if (AudioMode == DualMode)
  for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

//   set channel status channel byte 1 bit 2 = 1   ( mono )
  byteno = 1;
  bitno = 2;
  if (AudioMode == MonoMode)
  for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
   } // for

/*   set channel status channel byte 2 bit 2 = 1   ( 24 bit audio word ) */
  byteno = 2;
  bitno = 2;
  if (AudioWord == 24)
  for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
  } /* for */


/*   set channel status channel byte 2 bit 3 = 1 and bit 5 = 1
					    ( word length indicated ) */
  byteno = 2;
  bitno = 3;
  for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
  } /* for */

  byteno = 2;
  bitno = 5;
  for (block = 0; block <= 5; ++block){
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChA][block*768 + byteno*32 + bitno*4 + 3];

    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
    ++AudioData[ChB][block*768 + byteno*32 + bitno*4 + 3];
  } /* for */

} //stdimpl




unsigned char bintobyte(char *binstr){
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





void biphase(int bloksize){
// calculates each subframe as a binary string and then codes it into biphase.
// the biphase codes is written to a binary prom file.

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

 if (bloksize == Blok4k)
  noofframes = 192;
 if (bloksize == Blok16k)
  noofframes = 576;
 if (bloksize == Blok32k)
  noofframes = 1152;



 for(no = 0; no < noofframes; ++no)
  for(ch = 0; ch < 2; ++ch){        // channel A and B
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
    for(d = 31; d >= 0; --d){
	 Frame[d] = Digit[sf & 1];
	 sf = sf >> 1;
    } // for
    Frame[32] = '\x0';



    // put biphase coding of the 8 bits X,Y or Z preamble into BiPhStr
    if (prev == '0'){
	 if ((Fr == 0) && (SubFrame == 1))     //  Z
	  strcat(BiPhStr,Z1);
	 if ((Fr > 0) && (SubFrame == 1))      //  X
	  strcat(BiPhStr,X1);
	 if ((Fr > 0) && (SubFrame == 2))      //  Y
	  strcat(BiPhStr,Y1);
    }
    else{
	 if ((Fr == 0) && (SubFrame == 1))     //  Z
	  strcat(BiPhStr,Z0);
	 if ((Fr > 0) && (SubFrame == 1))      //  X
	  strcat(BiPhStr,X0);
	 if ((Fr > 0) && (SubFrame == 2))      //  Y
	  strcat(BiPhStr,Y0);
    } // if prev


    // biphase coding: 28 bits into BiPhStr
     for (d = 4; d <= 31; ++d){
       switch (prev){
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



    for (k = 0; k < 8; ++k){          // write binary data to PROM file
      strmove(BiPhStr,ff,k*8,8);
      fprintf(binfile,"%c",bintobyte(ff));
    }
    ++NoOf8Bytes;                    // 8 bytes written to binary file


    if (Fr < 383)                      // 192 subframes * 2
	 ++Fr;
    else{
	 Fr = 0;                           // start preamble Z
	 printf("%s",">>  ");
	} // if


    if (SubFrame < 2)                   // toggles between channel A and B
      ++SubFrame;
     else
      SubFrame = 1;


  } // for ch

 printf("\n");

 
 
 // fill up with 0xFF
 
 // 4kb PROM blok: 2^12 = 4096 bytes
 // 4096 - 3072 = 1024

 // 16kb PROM blok: 2^14 = 16384 bytes
 // 16384 - 9216 = 7168    (3072 x 3 = 9216)

 // 32kb PROM blok: 2^15 = 32768 bytes
 // 32768 - 18432 = 14336    (3072 x 6 = 18432)
 
 if (bloksize == Blok4k){
  for(k = 0; k < 1024; ++k)
   fprintf(binfile,"%c",'\xff');
 }
 else
 if (bloksize == Blok16k){
  for(k = 0; k < 7168; ++k)
   fprintf(binfile,"%c",'\xff');
 }
 else
 if (bloksize == Blok32k){
  for(k = 0; k < 14336; ++k)
   fprintf(binfile,"%c",'\xff');
 }

} // biphase ----------------------------------------------------------------



void fillFF(long int no){
 // fill up prom file with 0xFF
 // no = number of 1024 bytes
 long int k;
 no = no * 1024;

 for(k = 0; k < no; ++k)
  fprintf(binfile,"%c",'\xff');

 printf("FFh blok: %dk\n",(no / 1024));
}




//----------------------------------------------------------------------------
void calcrc(int ch){
int k,n,
    crc[184],
    Q[8],
    X8;

for (k = 0; k < 6; ++k){                  // calculate crc in 6 blocks
 
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


     if ((k == 0) && (ch == 0)){
  //   fprintf(chstatusfile,"Level: %6.1f      Ch-A freq: %5d      Ch-B freq: %5d      Mode: %d\n",
  //          AudioLevel,AudioFreq1,AudioFreq2, AudioMode);
      for (n = 0; n < 192; ++n){
       fprintf(chstatusfile,"%d",(AudioData[ch][(k*768)+n*4+3] & 0x02) >> 1);

		 if ((((n+1) % 8) == 0) && (n > 0))
	 		fprintf(chstatusfile,"\n");
       }
      fprintf(chstatusfile,"\n");

     } // if

 } // for (k..

} // calcrc








char *strmove(char *instr, char *sout, int st, int no){
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







int parity(int ch, int no){
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



int pow2(int y){
// beregner 2^y
 int x,n;
 x = 1;

 for(n = 0; n < y ; ++n)
  x = x * 2;

 return(x);
}



float exp10(float y){
float r;

r = y * log(10.0);
return(exp(r));
}




unsigned char invertbyte( unsigned char inb){
// invert the 8 bits in a byte

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

