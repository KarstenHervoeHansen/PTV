/* FILE NAME        makebot.c
 * PURPOSE          To insert logo into channel files.
 *
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       21.08.2002
 * NOTES:           The logo is inserted in lowedt part of Philips NTSC picture
 *                  Logo file is bot.bmp
 *                  Logo starts at 5204   (Start address in PROM)
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <io.h>

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long

#define BMP_FILE "bot.bmp"

#define BLOCK           5203

#define BMP_LINE_LEN    (long)(246 * 3 + 2)   /* 231 samples of 3 bytes */
#define NR_BMP_LINES     57    /* Number of lines in our .bmp file */
#define NR_LINES         23    /* Number of logo lines in our picture */
#define CHAN_LINE_LEN  1024L   /*Line len in channelx.dat-files (word) */

#define LOGO_LEN        (int)((BMP_LINE_LEN - 2) / 3) - 10
#define LOGO_START      259

#define LOGO_LINES_TO_SKIP  15  /* 7 skip first LOGO_LINES_TO_SKIP when loading logo */

#define FILTER 0

int chan1_fptr;
int chan2_fptr;
int chan3_fptr;
int bmp_fptr;

UI   chan1_buff[CHAN_LINE_LEN];
UI   chan2_buff[CHAN_LINE_LEN];
UI   chan3_buff[CHAN_LINE_LEN];
UC   bmp_buff[BMP_LINE_LEN];


UI   Ybuff[CHAN_LINE_LEN];
UI   Ubuff[CHAN_LINE_LEN];
UI   Vbuff[CHAN_LINE_LEN];
UI   tranparent_help[CHAN_LINE_LEN];

float Tempo[CHAN_LINE_LEN];


float Filter_logo[] =
  {
  9.927E-0005,
  1.032E-0001,
  3.967E-0001,
  3.967E-0001,
  1.032E-0001,
  9.027E-0005
  };

int   Nfolde2;



/* Protos */
void fileclose(void);
void fileopen(void);
void fileload(UL);
void logo_fold(void);
void make_logo_filter(void);
void process_logo(int);
void insert_logo(int, int, int);
void rgb_yuv(void);
void filesave(UL);
void loadlogo(UL);
void freeall(void);

void main(void)
  {
  int i;
  int j;
  int block;

  _fmode = O_BINARY; 

  for (i = 0; i < CHAN_LINE_LEN; i++)
    {
    Ybuff[i] =0;
    Ubuff[i] =0;
    Vbuff[i] =0;
    }
  fileopen();
/*  make_logo_filter(); */

  block = BLOCK; 

  for (i = 0; i < NR_LINES; i++)
    {
    fileload((UL)(block + i));
    loadlogo((UL)(i + LOGO_LINES_TO_SKIP)); /* + 170 + 1 */
    rgb_yuv();
    process_logo(64);

/* We are inserting logo here.  If given logo pixel is RED, do not
   insert insert this sample (logo must be without red background)
*/
    for (j = LOGO_START; j < LOGO_START + LOGO_LEN; j++)
      {
#if 1
      if (tranparent_help[j - LOGO_START] == 0xFF)
        continue;
#endif
      chan1_buff[j] = Ybuff[j - LOGO_START];
      chan2_buff[j] = Ubuff[j - LOGO_START];
      chan3_buff[j] = Vbuff[j - LOGO_START];
      }
    filesave((UL)(block + i));
    }
  fileclose();
  printf("All done\n");
  }
/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

/* This function opens all files used */
void fileopen(void)
  {
  if ((chan1_fptr = open("CHANNEL1.DAT", O_RDWR | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((chan2_fptr = open("CHANNEL2.DAT", O_RDWR | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((chan3_fptr = open("CHANNEL3.DAT", O_RDWR | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((bmp_fptr = open(BMP_FILE, O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  }


/**************************************************************************/
/* Function: fileload()                                                   */
/* Purpose:  To load one line data from channelx.dat files into
/*           respective chanx_buff buffers.
/* Args:     UL line_num - number of line which data to load.
/* Remarks:  Data is 16 bit samples (UI), there are CHAN_LINE_LEN of UI
/*           (2 bytes) in a line.
/*           On exit, chanx_buffs are filled with CHAN_LINE_LEN of UIs
/*
/* Returns:  Nothing.
/*           
/**************************************************************************/
void fileload(UL line_num)
  {

  lseek(chan1_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  read(chan1_fptr, chan1_buff, CHAN_LINE_LEN * 2);
  lseek(chan2_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  read(chan2_fptr, chan2_buff, CHAN_LINE_LEN * 2);
  lseek(chan3_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  read(chan3_fptr, chan3_buff, CHAN_LINE_LEN * 2);
  }

/**************************************************************************/
/* Function: loadlogo()                                                   */
/* Purpose:  To load one line data from logo.bmp file into
/*           bmp_buff buffer.
/*
/* Args:     UL line_num - number of first logo line which data to load.
/*           We skip first line_num of logo, to allow vertical centering.
/*
/* Remarks:  Data is 3 bytes (R,G,B), already taken into account in
/*           BMP_LINE_LEN.
/*
/*           On exit, bmp_buff is filled with BMP_LINE_LEN bytes
/*
/* Returns:  Nothing.
/*
/**************************************************************************/

void loadlogo(UL line_num)
  {
  long z;

  z = 54 + ((NR_BMP_LINES - 1 - line_num) * BMP_LINE_LEN);

  lseek(bmp_fptr, (z), SEEK_SET);

  read(bmp_fptr, bmp_buff, BMP_LINE_LEN);
  }
/**************************************************************************/
/* Function: rgb_yuv()                                                    */
/* Purpose:  To convert logo data from RGB to YUV
/*
/* Args:     None
/*
/* Remarks:  On entry, bmp_buff contains BMP_LINE_LEN bytes of logo RGB data
/*           
/*           On exit, Ybuff... are filled with BMP_LINE_LEN / 3 bytes
/*
/* Returns:  Nothing.
/*           
/**************************************************************************/
void rgb_yuv(void)
  {
  float R;
  float G;
  float B;
  float Y;
  float U;
  float V;
  int   i;
  int   z;
  int   bndx;
  int    r;
  int    g;
  int    b;
  float  color;

  bndx = LOGO_LEN + 4;

  for (i = 0; i < (bndx); i++)
    tranparent_help[i] = 0;

  for (i = 0; i < (bndx); i++)
    {
    z = 3 * i;
    B = (float) bmp_buff[z];
    b = (int)bmp_buff[z++];
    G = (float) bmp_buff[z];
    g = (int)bmp_buff[z++];
    R = (float) bmp_buff[z];
    r = (int)bmp_buff[z++];
/* see if it is pure red, if is, then put FF in tranparent_help[i] */
    if ((r == 0xFF) && (g == 0) && (b == 0))
      {
      tranparent_help[i] = 0xFF;
      }

    color = R + G + B;
    if ((color) > 747.0)
      {
      R = 242.25; /* white */
      G = 242.25;
      B = 242.25;
      }
    if ((color) < 10.0)
      {
      R = 67.0; /* black */
      G = 67.0;
      B = 67.0;
      }
    Y = (0.299 * R + 0.587 * G + 0.114 * B);
    U = (B - Y) * 0.493 * 1.1447;            /* Cb 0,5643371 */
    V = (R - Y) * 0.877 * 0.8133;            /* Cr 0,7132641 */

    Ybuff[i] = (UI) (Y * 876 / 255 + 64);
    Ubuff[i] = (UI) (U * 448 / 128 + 512);
    Vbuff[i] = (UI) (V * 448 / 128 + 512);
    }
  }


void filesave(UL line_num)
  {

  lseek(chan1_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  write(chan1_fptr, chan1_buff, CHAN_LINE_LEN * 2);
  lseek(chan2_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  write(chan2_fptr, chan2_buff, CHAN_LINE_LEN * 2);
  lseek(chan3_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  write(chan3_fptr, chan3_buff, CHAN_LINE_LEN * 2);
  }


void fileclose(void)
  {
  close(chan1_fptr);
  close(chan2_fptr);
  close(chan3_fptr);
  close(bmp_fptr);
  }

void logo_fold(void)
  {
  int i;
  int j;
  float scale;
  float sum;
  float temp[720];
  int   indx;

  for (i = 0; i < 720; i++)
    temp[i] = 0.0;

  Nfolde2 = 6;

  scale = 0.0;
  for (i = 0; i < Nfolde2; i++)
    scale = scale + Filter_logo[i];

  for (i = Nfolde2; i < 720; i++)
    {
    sum = 0.0;
    for (j = 0; j < Nfolde2; j++)
      {
      indx = i - j;
      sum = sum + Tempo[indx - 2] * Filter_logo[j] / scale;
      }
    temp[i] = sum;
    }
  for (i = 0; i < (720 - Nfolde2); i++)
    Tempo[i] = temp[i + 6];
  }


/**************************************************************************/
/* Function: process_logo(int n3)                                         */
/* Purpose:  To convert logo data from RGB to YUV
/*
/* Args:     Y_DAC_offset (?)
/*
/* Remarks:  On entry, Ybuff,,,, contains BMP_LINE_LEN / 3 bytes of logo YUV data
/*           
/*           On exit, that data is filtered
/*
/* Returns:  Nothing.
/*           
/**************************************************************************/

void process_logo(int Y_DAC_offset)
  {
  int i;
  float Y;
  float U;
  float V;

  for (i = 0; i < CHAN_LINE_LEN; i++)
    {
    Tempo[i]    = 0.0;
    }

  /* logo in Y channel */
  for (i = 0; i < 720; i++)
    {
    Tempo[i] = (float)Ybuff[i];
    Tempo[i] = Tempo[i] - (float)Y_DAC_offset;
    }
  /* logo_fold() manipulates Tempo[] */
#if FILTER
  logo_fold();
#endif
  for (i = 0; i < 720; i++)
    Ybuff[i] = (UI)(Tempo[i] + Y_DAC_offset);

  /* Logo in U channel */
  for (i = 0; i < 720; i++)
    {
    Tempo[i] = Ubuff[i];
    Tempo[i] = Tempo[i] - 512.0;
    }
#if FILTER
  logo_fold();
#endif
  for (i = 0; i < 720; i++)
    Ubuff[i] = (UI)(Tempo[i] + 512);

  /* Logo in V channel */
  for (i = 0; i < 720; i++)
    {
    Tempo[i] = Vbuff[i];
    Tempo[i] = Tempo[i] - 512.0;
    }
#if FILTER
  logo_fold();
#endif
  for (i = 0; i < 720; i++)
    Vbuff[i] = (UI)(Tempo[i] + 512);
/* removed */
  }
