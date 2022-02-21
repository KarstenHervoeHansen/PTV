/* FILE NAME        ch2bmp.c
 * PURPOSE          Generic program to make a .bmp file from channelx.dat.
 *                  files
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       1.10.2002
 * NOTES:           converts CHANNELX.DAT to CHAN.BMP

 *                  
 *                  
 *                  
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


#define OUT_FILE "CHAN.bmp"    /* Output file containing picture */

#define IMLEN    256           /* length of output image */ 
#define IMHEIGHT  44           /* height of output image */
#define IMSTART    0           /* Start address of output image in .dat files */

#define CHAN_LINE_LEN  IMLEN   /*Line len in channelx.dat-files (word) in samples */
#define FILTER 1
#define NO_THRESHOLD 1

/* Structure to hold info from channel0.dat file */
typedef struct tagCHANNEL0
  {
  double IFS;  /* inner first sample */
  double ILS;  /* inner last sample  */
  double LLS;  /* line last sample   */
  double BS;   /* block size         */
  double LO;   /* line offset        */
  double SO1;  /* sample offset Y channel */
  double SO2;  /* sample offset U channel */
  double SO3;  /* sample offset V channel */
  double DA1;  /* Y DAC offset            */
  double DA2;  /* U DAC offset            */
  double DA3;  /* V DAC offset            */
  double DR1;  /* Y DAC resolution        */
  double DR2;  /* U DAC resolution        */
  double DR3;  /* V DAC resolution        */
  double F;    /* frequency               */
  double NAL;  /* nr of active lines      */
  double NAS;  /* nr of active samples    */
  double AR;   /* aspect ratio            */
  double RN1;  /* roundoff number chan Y  */
  double RN2;  /* roundoff number chan U  */
  double RN3;  /* roundoff number chan V  */
  double C4SR; /* channel 4 subsampling rate */
  } CHANNEL0;

/* Structures used in .BMP files */
typedef struct tagBITMAPFILEHEADER 
  {
  UI   bfType;
  UL   bfSize;
  UI   bfReserved1;
  UI   bfReserved2;
  UL   bfOffBits;
  } BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
  {
  UL      biSize;
  UL      biWidth;
  UL      biHeight;
  UI      biPlanes;
  UI      biBitCount;

  UL      biCompression;
  UL      biSizeImage;
  UL      biXPelsPerMeter;
  UL      biYPelsPerMeter;
  UL      biClrUsed;
  UL      biClrImportant;
  } BITMAPINFOHEADER;

int chan0_fptr;
int chan1_fptr;
int chan2_fptr;
int chan3_fptr;
int out_fptr;     /* pointer to output .bmp file */

CHANNEL0   chan_params;

UI   chan1_buff[CHAN_LINE_LEN];
UI   chan2_buff[CHAN_LINE_LEN];
UI   chan3_buff[CHAN_LINE_LEN];
UC   bmp_buff[CHAN_LINE_LEN * 3];


UI   Ybuff[CHAN_LINE_LEN];
UI   Ubuff[CHAN_LINE_LEN];
UI   Vbuff[CHAN_LINE_LEN];


/* Protos */
void fileclose(void);
void open_output_file(void);
void close_output_file(void);
void fileopen(void);
void fileload(UL);
void filesave(UL);
void yuv_rgb(int);

void main(void)
  {
  int i;
  int j;
  int k;
  int start;
  int end;
  int block;
  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info;
  UL bmp_bytes_per_line;
  int lines;

  _fmode = O_BINARY; 

  printf("CH2BMP; converts CHANNELX.DAT to CHAN.BMP\n");
  for (i = 0; i < CHAN_LINE_LEN; i++)
    {
    Ybuff[i] =0;
    Ubuff[i] =0;
    Vbuff[i] =0;
    }
  fileopen();

/* read params from channel0.dat */
  read(chan0_fptr, &chan_params, sizeof(CHANNEL0)); 
#if 0
  chan_params.DA1 = 64;
  chan_params.DA2 = 512;
  chan_params.DA3 = 512;

  chan_params.DR1 = 
  chan_params.DR2 = 
  chan_params.DR3 = 
#endif

#if 0
  printf("IFS = %f\n", chan_params.IFS);
  printf("ILS = %f\n", chan_params.ILS);
  printf("LLS = %f\n", chan_params.LLS);
  printf("BS  = %f\n", chan_params.BS);
  printf("LO  = %f\n", chan_params.LO);
  printf("SO1 = %f\n", chan_params.SO1);
  printf("SO2 = %f\n", chan_params.SO2);
  printf("SO3 = %f\n", chan_params.SO3);
  printf("DA1 = %f\n", chan_params.DA1);
  printf("DA2 = %f\n", chan_params.DA2);
  printf("DA3 = %f\n", chan_params.DA3);
  printf("DR1 = %f\n", chan_params.DR1);
  printf("DR2 = %f\n", chan_params.DR2);
  printf("DR3 = %f\n", chan_params.DR3);
  printf("F   = %f\n", chan_params.F);
  printf("NAL = %f\n", chan_params.NAL);
  printf("NAS = %f\n", chan_params.NAS);
  printf("AR  = %f\n", chan_params.AR);
  printf("RN1 = %f\n", chan_params.RN1);
  printf("RN2 = %f\n", chan_params.RN2);
  printf("RN3 = %f\n", chan_params.RN3);
  printf("C4SR = %f\n", chan_params.C4SR);
#endif

/* Now we will produce a .BMP file  */


  fileopen();           /* open channelx.dat files */
  open_output_file();

/* Our picture is IMLEN x IMHEIGHT, we fill bmp_info and bmp_header structures 
   and write them into output file 
*/

  bmp_info.biWidth   = IMLEN;
  bmp_bytes_per_line = bmp_info.biWidth * 3;
  if (bmp_bytes_per_line & 0x0003)
    {
    bmp_bytes_per_line |= 0x0003;
    ++bmp_bytes_per_line;
    }

  bmp_header.bfSize    = bmp_bytes_per_line * (IMHEIGHT) + sizeof(BITMAPFILEHEADER)
                         + sizeof(BITMAPINFOHEADER); 

  bmp_info.biHeight    = IMHEIGHT;
  bmp_info.biSizeImage = IMLEN * (IMHEIGHT);

  bmp_header.bfType         = 0x4D42;
  bmp_header.bfReserved1    = 0;
  bmp_header.bfReserved2    = 0;
  bmp_header.bfOffBits      = 54;

  bmp_info.biSize           = 40;
  bmp_info.biPlanes         = 1;
  bmp_info.biBitCount       = 24;
  bmp_info.biCompression    = 0;
  bmp_info.biXPelsPerMeter  = 3780;
  bmp_info.biYPelsPerMeter  = 3780;
  bmp_info.biClrUsed        = 0;
  bmp_info.biClrImportant   = 0;

  

/*************/
  printf("FileSize is:   %lu\n", bmp_header.bfSize);

#if 0
  printf("Header:\n");
  printf("Type is:       %x\n",  bmp_header.bfType);
  printf("FileSize is:   %lu\n", bmp_header.bfSize);
  printf("Reserved1 is:  %u\n", bmp_header.bfReserved1);
  printf("Reserved2 is:  %u\n", bmp_header.bfReserved2);
  printf("Offbits is:    %lu\n", bmp_header.bfOffBits);

  printf("Info:\n");
  printf("InfoSize is:      %lu\n", bmp_info.biSize);
  printf("Width is:         %lu\n", bmp_info.biWidth);
  printf("Height is:        %lu\n", bmp_info.biHeight);
  printf("Planes is:        %d\n",  bmp_info.biPlanes);
  printf("Bit count is:     %d\n",  bmp_info.biBitCount);
  printf("Compression is:   %lu\n", bmp_info.biCompression);
  printf("Size image is:    %lu\n", bmp_info.biSizeImage);
  printf("X pix/m is:       %lu\n", bmp_info.biXPelsPerMeter);
  printf("Y pix/m is:       %lu\n", bmp_info.biYPelsPerMeter);
  printf("CtrlUsed is:      %lu\n", bmp_info.biClrUsed);
  printf("Important is:     %lu\n", bmp_info.biClrImportant);


  printf("bytes per line is: %lu\n", bmp_bytes_per_line);

/*  fileclose();
  exit(0); */
#endif
/********************/

  write(out_fptr, &bmp_header, sizeof(BITMAPFILEHEADER));
  write(out_fptr, &bmp_info, sizeof(BITMAPINFOHEADER));

  block = IMSTART; /* Start of picture in channelx.dat files */

  lines = 0;
  for (i = block + IMHEIGHT - 1; i >= block; i--)
    {
    fileload((UL)(i));
    for (j = 0; j < IMLEN; j++)
      yuv_rgb(j);
    write(out_fptr, bmp_buff, bmp_bytes_per_line);
    lines++;
    }
  close_output_file();
  fileclose();
  printf("All done, lines = %d\n", lines);
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

/* This function opens all files used */
void fileopen(void)
  {
#if 1
  if ((chan0_fptr = open("CHANNEL0.DAT", O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
#endif
  if ((chan1_fptr = open("CHANNEL1.DAT", O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((chan2_fptr = open("CHANNEL2.DAT", O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((chan3_fptr = open("CHANNEL3.DAT", O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  }

void open_output_file(void)
  {
  out_fptr = open(OUT_FILE, O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE);
  
  if (out_fptr == -1)
    {
    puts("Cant open output file.\n");
    exit(1);
    }
  }


void fileclose(void)
  {
  close(chan0_fptr);
  close(chan1_fptr);
  close(chan2_fptr);
  close(chan3_fptr);
  }

void close_output_file(void)
  {
  close(out_fptr);
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
  int result;

  lseek(chan1_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  result = read(chan1_fptr, chan1_buff, CHAN_LINE_LEN * 2);
  if (result == 0)
    printf("Error reading file, linenum = %lu\n", line_num);

  lseek(chan2_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  result = read(chan2_fptr, chan2_buff, CHAN_LINE_LEN * 2);

  lseek(chan3_fptr, (line_num * CHAN_LINE_LEN * 2), SEEK_SET);
  result = read(chan3_fptr, chan3_buff, CHAN_LINE_LEN * 2);
  }

/**************************************************************************/
/* Function: yuv_rgb(int sample)                                          */
/* Purpose:  To convert onse sample YUV data to RGB
/*
/* Args:     sample number in chanx_buff
/*
/* Remarks:  On entry, sample is read from chanx_buffs
/*           it is converted to RGB and,
/*           on exit, RGB data is placed at sample * 3 in bmp_buff
/*
/* Returns:  Nothing.
/*           
/* NOTES:    None
/*           
/**************************************************************************/

void yuv_rgb(int sample)
  {
  double R;
  double G;
  double B;
  double Y;
  double U;
  double V;

  Y = chan_params.DR1 * (chan1_buff[sample] - chan_params.DA1);
  U = chan_params.DR2 * (chan2_buff[sample] - chan_params.DA2);
  V = chan_params.DR3 * ((chan3_buff[sample] & 0x3FF) - chan_params.DA3);

  B = U / 0.562 + Y;
  R = V / 0.710 + Y;
  G = (Y - 0.299 * R - 0.114 * B) / 0.587;
#if NO_THRESHOLD
  if (G < 0.0)
    G = 0.0;
  if (G > 700.0)
    G = 700.0;

  if (B < 0.0)
    B = 0.0;
  if (B > 700.0)
    B = 700.0;

  if (R < 0.0)
    R = 0.0;
  if (R > 700.0)
    R = 700.0;
#endif
  bmp_buff[sample * 3]     = (UC) (B / 700.0 * 255.0);
  bmp_buff[sample * 3 + 1] = (UC) (G / 700.0 * 255.0);
  bmp_buff[sample * 3 + 2] = (UC) (R / 700.0 * 255.0);
  }

