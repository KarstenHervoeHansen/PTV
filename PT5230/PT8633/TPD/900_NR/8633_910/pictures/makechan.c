/* FILE NAME        makechan.c
 * PURPOSE          To conver logo from tdc.bmp into a set of channelx.dat.
 *                  files
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       21.08.2002
 * NOTES:
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


#define BMP_FILE "tdc.bmp"     /* File containing logo */


#define BLOCK             0   /* Start adderss in channelx.dat where our logo is to be */
#define LOGO_START   (int)0   /* X coordinare of upper left logo sample in our picture */

#define CHAN_LINE_LEN  256L   /*Line len in channelx.dat-files (word) in samples */
#define NR_LINES         44   /* Number of logo lines in our picture */
#define NO_THRESHOLD 1


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

int chan1_fptr;
int chan2_fptr;
int chan3_fptr;
int bmp_fptr;     /* pointer to logo .bmp file */


UI   chan1_buff[CHAN_LINE_LEN];
UI   chan2_buff[CHAN_LINE_LEN];
UI   chan3_buff[CHAN_LINE_LEN];
UC   bmp_buff[CHAN_LINE_LEN * 3];


UI   Ybuff[CHAN_LINE_LEN];
UI   Ubuff[CHAN_LINE_LEN];
UI   Vbuff[CHAN_LINE_LEN];

/* Protos */
void fileclose(void);
void close_logo_file(void);
void open_output_file(void);
void close_output_file(void);
void fileopen(void);
void open_logo_file(BITMAPFILEHEADER*, BITMAPINFOHEADER*);
void fileload(UL);
void rgb_yuv(int);
void filesave(UL);
void loadlogo(UL, UL, UL, UL);

void main(void)
  {
  int i;
  int j;
  int block;
  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info;
  UL bmp_bytes_per_line;
  int logo_len;

  _fmode = O_BINARY; 

  printf("Creating channelx.dat files for %s\n", BMP_FILE);

  for (i = 0; i < CHAN_LINE_LEN; i++)
    {
    Ybuff[i] =0;
    Ubuff[i] =0;
    Vbuff[i] =0;
    }
  open_logo_file(&bmp_header, &bmp_info);
  fileopen();

/* The number of bytes in each line of a .BMP file is always
   a multiple of 4 as Windows does DoubleWord Alignment on line
   boundaries. You need a variable bmp_bytes_per_line that specifies
   how many bytes there are in a line.
*/
  bmp_bytes_per_line = bmp_info.biWidth * 3;
  if (bmp_bytes_per_line & 0x0003)
    {
    bmp_bytes_per_line |= 0x0003;
    ++bmp_bytes_per_line;
    }
  logo_len = (int) (bmp_bytes_per_line / 3);

/********************/
  block = BLOCK; 

  for (i = 0; i < NR_LINES; i++)
    {
    fileload((UL)(block + i));
    loadlogo((UL)(i), bmp_bytes_per_line, bmp_info.biHeight, bmp_header.bfOffBits);
    rgb_yuv(logo_len);

    for (j = LOGO_START; j < LOGO_START + logo_len; j++)
      {
      chan1_buff[j] = Ybuff[j - LOGO_START];
      chan2_buff[j] = Ubuff[j - LOGO_START];
      chan3_buff[j] = Vbuff[j - LOGO_START];
      }
    filesave((UL)(block + i));
    }

  fileclose();
  close_logo_file();
  printf("Done %d lines.\n", i);
  exit (0); /* !!!!!!!!!!!!!!!!! we end here !!!!!!!!!!!!!!!!!!! */
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

/* This function opens all files used */
void fileopen(void)
  {
  unlink("CHANNEL1.DAT");
  unlink("CHANNEL2.DAT");
  unlink("CHANNEL3.DAT");

  if ((chan1_fptr = open("CHANNEL1.DAT", O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE)) == -1)
    {
    puts("Cant open output file 1.\n");
    exit(1);
    }
  if ((chan2_fptr = open("CHANNEL2.DAT", O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE)) == -1)
    {
    puts("Cant open output file 2.\n");
    exit(1);
    }
  if ((chan3_fptr = open("CHANNEL3.DAT", O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE)) == -1)
    {
    puts("Cant open output file 3.\n");
    exit(1);
    }
  }

/* This function opens logo file and fills header and info structures */
void open_logo_file(BITMAPFILEHEADER* header, BITMAPINFOHEADER* info)
  {
  if ((bmp_fptr = open(BMP_FILE, O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
/* load bmp info and header */
  read(bmp_fptr, header, sizeof(BITMAPFILEHEADER));
  read(bmp_fptr, info, sizeof(BITMAPINFOHEADER));
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
  }

void close_logo_file(void)
  {
  close(bmp_fptr);
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
/* Function: loadlogo(UL line_num, UL bytes_per_line, UL lines_in_bmp)    */
/* Purpose:  To load one line data from logo.bmp file into
/*           bmp_buff buffer.
/*
/* Args:     UL line_num - number of first logo line which data to load.
/*           UL bytes_per_line - nr. of bytes in each bmp line
/*           UL lines_in_bmp   - height of .bmp logo
/*           UL offset         - offset to picture data from beg of file
/*
/* Remarks:  Data is 3 bytes (R,G,B), 
/*
/*           On exit, bmp_buff is filled with bytes_per_line bytes
/*
/* Returns:  Nothing.
/*
/* NOTES:    The picture is stored up side down in a .BMP file, i.e.
/*           first upper picture line is stored as last line in file.
/*           Therefore we need lines_in_bmp to calculate offset of line
/*           in file.
/**************************************************************************/

void loadlogo(UL line_num, UL bytes_per_line, UL lines_in_bmp, UL offset)
  {
  long z;

  z = offset + ((lines_in_bmp - 1 - line_num) * bytes_per_line);

  lseek(bmp_fptr, (z), SEEK_SET);

  read(bmp_fptr, bmp_buff, bytes_per_line);
  }

/**************************************************************************/
/* Function: rgb_yuv()                                                    */
/* Purpose:  To convert logo data from RGB to YUV
/*
/* Args:     int logo_len: nr of samples in logo in our picture
/*
/* Remarks:  On entry, bmp_buff contains logo RGB data
/*           
/*           On exit, Ybuff... are filled with logo YUV
/*
/* Returns:  Nothing.
/*           
/**************************************************************************/
void rgb_yuv(int len_of_logo)
  {
  double R;
  double G;
  double B;
  double Y;
  double U;
  double V;
  int   i;
  int   z;
  double color;

  for (i = 0; i < (len_of_logo); i++)
    {
    z = 3 * i;
    B = (double) bmp_buff[z];
    z++;
    G = (double) bmp_buff[z];
    z++;
    R = (double) bmp_buff[z];

    color = R + G + B;
#if NO_THRESHOLD
    if ((color) > 747.0)
      {
      R = 255.0; /* 95% white */
      G = 255.0;
      B = 255.0;
      }
    if ((color) < 10.0)
      {
      R = 0.0; /* 95% black */
      G = 0.0;
      B = 0.0;
      }
#endif
    Y = (0.299 * R + 0.587 * G + 0.114 * B);
    U = (B - Y) * 0.493 * 1.1447;            /* Cb 0,5643371 */
    V = (R - Y) * 0.877 * 0.8133;            /* Cr 0,7132641 */

    Ybuff[i] = (UI) (Y * 876.0 / 255.0 + 64.0);
    Ubuff[i] = (UI) (U * 448.0 / 128.0 + 512.0);
    Vbuff[i] = (UI) (V * 448.0 / 128.0 + 512.0);
    }
  }


