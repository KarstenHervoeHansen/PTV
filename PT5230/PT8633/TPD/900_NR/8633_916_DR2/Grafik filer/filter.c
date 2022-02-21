/* FILE NAME        filter.c
 * PURPOSE          This program reads a .BMP file, filters it and saves 
 *                  result to new .BMP file named tdc.bmp
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       21.08.2002
 * NOTES:           The output file has different dimensions than input one,
 *                  due to filter algorithm.  In each line, left kernel_size / 2 + 1
 *                  pixels are unusable and right kernel_size pixels are
 *                  also unusable.
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


#define BMP_FILE "image1.bmp"  /* File containing logo */
#define OUT_FILE "dr2169f.bmp"    /* Output file containing picture with logo */

#define LEFT_WASTE  (kernel_size / 2 + 1)
#define RIGHT_WASTE (kernel_size)


#define CHAN_LINE_LEN  1024L   /*Line len in channelx.dat-files (word) in samples */
#define FILTER 1
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

int bmp_fptr;     /* pointer to logo   .bmp file */
int out_fptr;     /* pointer to output .bmp file */


UC   bmp_buff[CHAN_LINE_LEN * 3];


UI   Ybuff[CHAN_LINE_LEN];
UI   Ubuff[CHAN_LINE_LEN];
UI   Vbuff[CHAN_LINE_LEN];

double tem_flo_buff[CHAN_LINE_LEN];

double kernel[20];

int   kernel_size;

/* Protos */
void fileclose(void);
void close_logo_file(void);
void open_output_file(char *);
void close_output_file(void);
void fileopen(void);
void open_logo_file(BITMAPFILEHEADER*, BITMAPINFOHEADER*);
void fileload(UL);
void convolute(int);
void process_logo(int);
void rgb_yuv(int);
void filesave(UL);
void loadlogo(UL, UL, UL, UL);
void yuv_rgb(int);
void make_filter(void);

void main(void)
  {
  int i;
  int j;
  int block;
  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info;
  BITMAPFILEHEADER out_header;
  BITMAPINFOHEADER out_info;
  UL bmp_bytes_per_line;
  UL out_bytes_per_line;
  int logo_len;
  long z;

  _fmode = O_BINARY; 

  printf("Filter: reads %s, filters it and saves as %s\n", BMP_FILE, OUT_FILE);

  kernel_size = 6; 
  make_filter(); 

  for (i = 0; i < CHAN_LINE_LEN; i++)
    {
    Ybuff[i] = 0;
    Ubuff[i] = 0;
    Vbuff[i] = 0;
    }
  open_logo_file(&bmp_header, &bmp_info);

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
  printf("Done.\n");
/*  fileclose();
  exit(0); */
#endif

  if (bmp_info.biBitCount != 24)
    {
    printf("File %s is nor 24 bits/pixel.\n", BMP_FILE);
    exit(1);
    }

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
#if 0
  printf("bmp_info.biWidth * 3 = %d\n", bmp_info.biWidth * 3);
  printf("logo_len = %d\n", logo_len);
#endif
/********************/

/* We will save filtered logo into a separate file OUT_FILE.  Length of
   filtered logo is logo_len + kernel_size, but left LEFT_WASTE pixels
   and RIGHT_WASTE pixels are unusable.
*/
  open_output_file(OUT_FILE);
/* After filtering, the first LEFT_WASTE pixels and last
   RIGHT_WASTE pixels are useless, due to our filter algorithm
*/

  out_info.biWidth   = bmp_info.biWidth - LEFT_WASTE - RIGHT_WASTE; 
  out_bytes_per_line = out_info.biWidth * 3;
  if (out_bytes_per_line & 0x0003)
    {
    out_bytes_per_line |= 0x0003;
    ++out_bytes_per_line;
    }

  out_header.bfSize = out_bytes_per_line * bmp_info.biHeight + sizeof(BITMAPFILEHEADER)
                      + sizeof(BITMAPINFOHEADER);
  out_info.biHeight    = bmp_info.biHeight;
  out_info.biSizeImage = out_info.biWidth * out_info.biHeight;

/* copy rest of structures */
  out_header.bfType      = bmp_header.bfType;
  out_header.bfReserved1 = bmp_header.bfReserved1;
  out_header.bfReserved2 = bmp_header.bfReserved2;
  out_header.bfOffBits   = bmp_header.bfOffBits;

  out_info.biHeight    = bmp_info.biHeight;
  out_info.biPlanes    = bmp_info.biPlanes;
  out_info.biBitCount  = bmp_info.biBitCount;

  out_info.biSize          = bmp_info.biSize;
  out_info.biCompression   = bmp_info.biCompression;
  out_info.biXPelsPerMeter = bmp_info.biXPelsPerMeter;
  out_info.biYPelsPerMeter = bmp_info.biYPelsPerMeter;
  out_info.biClrUsed       = bmp_info.biClrUsed;
  out_info.biClrImportant  = bmp_info.biClrImportant;
/***************************/
  write(out_fptr, &out_header, sizeof(BITMAPFILEHEADER));
  write(out_fptr, &out_info,   sizeof(BITMAPINFOHEADER));
/***/

/* After filtering, the first LEFT_WASTE pixels and last
   RIGHT_WASTE pixels are useless, due to our filter algorithm
*/

  for (i = 0; i < bmp_info.biHeight; i++)
    {
    loadlogo((UL)i, bmp_bytes_per_line, bmp_info.biHeight, bmp_header.bfOffBits);
    rgb_yuv(logo_len);      /* Ybuff... are filled with logo YUV */
    process_logo(logo_len); /* Ybuff,,,, contains filtered logo YUV data */
    for (j = 0; j < out_info.biWidth; j++)
      yuv_rgb(j);
    z = out_header.bfOffBits + ((bmp_info.biHeight - 1 - i) * out_bytes_per_line);

    lseek(out_fptr, (z), SEEK_SET);

    /* write(out_fptr, bmp_buff, out_bytes_per_line);  */
    /* we write bytes starting at LEFT_WASTE * 3 to eliminate delay introduced */
    /* by filter algorithm                                                     */
    write(out_fptr, bmp_buff + (LEFT_WASTE * 3), out_bytes_per_line);  
    }

  close_logo_file();
  close_output_file();
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

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

void open_output_file(char * fname)
  {
  unlink(fname);
  out_fptr = open(fname, O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE);
  
  if (out_fptr == -1)
    {
    puts("Cant open output file.\n");
    exit(1);
    }
  }

void close_logo_file(void)
  {
  close(bmp_fptr);
  }

void close_output_file(void)
  {
  close(out_fptr);
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

/**************************************************************************/
/* Function: convolute(int signal_len)                                    */
/* Purpose:  To perform FIR filering of logo YUV data
/*
/* Args:     int signal_len: nr of samples in logo in logo data
/*
/* Remarks:  On entry, signal is in tem_flo_buff, kernel is in kernel
/*           
/*           On exit, tem_flo_buff contains filtered data.
/*
/* Returns:  Nothing.
/*           
/* NOTES:    This function performs convolution of our signal (logo data)
/*           and impulse response of a low pass filter (kernel).
/*           Length of resulting data is signal_len + kernel_size, of which
/*           (LEFT_WASTE + RIGHT_WASTE) are unusable.
/**************************************************************************/

#if 1
/* Input Side Algorithm */
void convolute(int signal_len)
  {
  int i;
  int j;
  double scale;
  double out_buff[720];
  int   indx;

  for (i = 0; i < 720; i++)
    out_buff[i] = 0.0;


  scale = 0.0;
  for (i = 0; i < kernel_size; i++)
    scale = scale + kernel[i];

  for (i = 0; i < signal_len; i++)
    {
    for (j = 0; j < kernel_size; j++)
      {
      out_buff[i + j] += tem_flo_buff[i] * kernel[j] / scale;
      }
    }

  for (i = 0; i < signal_len + kernel_size; i++)
    tem_flo_buff[i] = out_buff[i];
  }
#else
/* Output Side Algorithm */
void convolute(int signal_len)
  {
  int i;
  int j;
  double scale;
  double out_buff[720];
  int   indx;
  int   left;
  int   right;

  left  = 0;
  right = 0;

  for (i = 0; i < 720; i++)
    out_buff[i] = 0.0;


  scale = 0.0;
  for (i = 0; i < kernel_size; i++)
    scale = scale + kernel[i];

  for (i = 0; i < signal_len + kernel_size; i++)
    {
    out_buff[i] = 0;
    for (j = 0; j < kernel_size; j++)
      {
      if (i - j < 0)
        {
        left ++;
        continue;
        }
      if (i - j >= signal_len)
        {
        right ++;
        continue;
        }
      out_buff[i] = out_buff[i] + tem_flo_buff[i - j] * kernel[j] / scale;
      }
    }
  for (i = 0; i < signal_len + kernel_size; i++)
    tem_flo_buff[i] = out_buff[i];
  }
#endif

/**************************************************************************/
/* Function: process_logo(int len)                                        */
/* Purpose:  To convert logo data from RGB to YUV
/*
/* Args:     None
/*
/* Remarks:  On entry, Ybuff,,,, contains of logo YUV data
/*           
/*           On exit, that data is filtered
/*
/* Returns:  Nothing.
/*           
/**************************************************************************/

void process_logo(int len)
  {
  int i;
  double Y;
  double U;
  double V;

  for (i = 0; i < CHAN_LINE_LEN; i++)
    {
    tem_flo_buff[i]    = 0.0;
    }

  /* logo in Y channel */
  for (i = 0; i < len; i++)
    {
    tem_flo_buff[i] = (double)Ybuff[i];
    tem_flo_buff[i] = tem_flo_buff[i] - 64.0;
    }
  /* convolute() manipulates tem_flo_buff[] */
#if FILTER
  convolute(len);
#endif
  for (i = 0; i < len; i++)
    Ybuff[i] = (UI)(tem_flo_buff[i] + 64.0);

  /* Logo in U channel */
  for (i = 0; i < len; i++)
    {
    tem_flo_buff[i] = Ubuff[i];
    tem_flo_buff[i] = tem_flo_buff[i] - 512.0;
    }
#if FILTER
  convolute(len);
#endif
  for (i = 0; i < len; i++)
    Ubuff[i] = (UI)(tem_flo_buff[i] + 512.0);

  /* Logo in V channel */
  for (i = 0; i < len; i++)
    {
    tem_flo_buff[i] = Vbuff[i];
    tem_flo_buff[i] = tem_flo_buff[i] - 512.0;
    }
#if FILTER
  convolute(len);
#endif
  for (i = 0; i < len; i++)
    Vbuff[i] = (UI)(tem_flo_buff[i] + 512.0);
  }

void yuv_rgb(int sample)
  {
  double R;
  double G;
  double B;
  double Y;
  double U;
  double V;

  Y = 0.799087 * (Ybuff[sample] - 64.0);
  U = 0.781250 * (Ubuff[sample] - 512.0);
  V = 0.781250 * ((Vbuff[sample] & 0x3FF) - 512.0);

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

/**************************************************************************/
/* Function: make_filter(void)                                            */
/* Purpose:  To calculate kernel (impulse response of low pass filter)
/*
/* Args:     None
/*
/* Remarks:  None
/*           
/* Returns:  Nothing.
/*           
/* NOTES:    This function is a direct translation of Pascal algorithm
/*           used in original processing programs made by PTV
/*
/**************************************************************************/
void make_filter(void)
  {
  int i;
  int c;
  double Y;
  double Yr;
  double Ys;
  double Ampl;
  double Glampl;

  Yr     = 150E-9;
  Ys     = 13.5E6;
  Glampl = 0;
  c = (int)(1.03734 * Yr * Ys);

  for (i = 0; i < 20; i++)
    kernel[i] = 0.0;

  for (i = -c; i <= c; i++)
    {
    Y    = ((double) i) / Yr / Ys / 2.07468 + 0.5;
    Ampl = (Y - (sin(2.0 * M_PI * Y) / (2.0 * M_PI)));

    kernel[i + c] = Ampl - Glampl;
    Glampl        = Ampl;
    }
  kernel[c + i] = 1.0 - Glampl;
  kernel_size = 2 * i;

#if 0
  for (i = 0; i < 7; i++)
    printf("Kernel[%d] = %10.14e\n", i, kernel[i]);
  printf("kernel_size is %d\n", kernel_size);
  printf("c is %d\n", c);
#endif
  }
