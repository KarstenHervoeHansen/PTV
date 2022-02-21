/* FILE NAME        makelogo.c
 * PURPOSE          Generic program to insert logo into channel files.
 *
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       21.08.2002
 * NOTES:           This program inserts logo and then makes a .BMP file
 *                  with entire picture, named dump.bmp, so we can see 
 *                  the result.
 *                  This program inserts 3 logos in 3 Philips 4x3 images
 *                  for PAL, there are 2 images for 'with antipal' and
 *                  one image for 'without antipal'.  How they are placed,
 *                  see "Billede til PT8633" document.
 *                  Logo file must be filtered by separate program.
 *                
 *                  This program is used to insert TDC logo consisting of 
 *                  "Digital Video" and "Network" into 3 Philips pictures.
 *                  Original program, which inserted 'Cat's Paw' picture
 *                  is saved as makelogo.bak.c (25.04.2003)
 *                  "Digital Video" is in dvn1.bmp
 *                  "Network"       is in net1.bmp
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


#define BMP_FILE "dvn1.bmp"     /* File containing logo */
#define OUT_FILE "dump.bmp"    /* Output file containing picture with logo */

#define IMLEN     800           /* length of output image */
#define IMHEIGHT 1850           /* height of output image */
#define OUT_IMSTART 230          /* start of image for bmp output file */

#define IMSTART   230          /* Start address of image in .dat files */
                                /* PAL Philips 4x3                      */

#define BLOCK   IMSTART + 63   /* Start adderss in PROM where our logo is to be */
#define NR_LINES          31   /* Number of logo lines in our picture */
#define LOGO_START  (int)283   /* X coordinare of upper left logo sample in our picture */
#define LOGO_LINES_TO_SKIP  0  /* 7 skip first LOGO_LINES_TO_SKIP when loading logo */

#define CHAN_LINE_LEN  1024L   /* Line len in channelx.dat-files (word) in samples */
#define FILTER 0               /* Logo file must be filtered by separate program   */
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
int bmp_fptr;     /* pointer to logo .bmp file */
int out_fptr;     /* pointer to output .bmp file */

CHANNEL0   chan_params;

UI   chan1_buff[CHAN_LINE_LEN];
UI   chan2_buff[CHAN_LINE_LEN];
UI   chan3_buff[CHAN_LINE_LEN];
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
void open_output_file(void);
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
  int k;
  int start;
  int end;
  int block;
  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info;
  UL bmp_bytes_per_line;
  int logo_len;

  _fmode = O_BINARY; 

/*  kernel_size = 6; */
  make_filter(); 

  for (i = 0; i < CHAN_LINE_LEN; i++)
    {
    Ybuff[i] =0;
    Ubuff[i] =0;
    Vbuff[i] =0;
    }
  open_logo_file(&bmp_header, &bmp_info);
  fileopen();

/* read params from channel0.dat */
  read(chan0_fptr, &chan_params, sizeof(CHANNEL0));
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

/*************/

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
    printf("Logo file %s is nor 24 bits/pixel.\n", BMP_FILE);
    exit(1);
    }

/*********************************************************/
/*                   S T A R T                           */
/*********************************************************/


#if 1
/*
    First we fill the black rectangle where logo moves with white.  There
    are 3 such rectangles, one for without antipal and two for with antipal.
    They reside at 6795 to 6920.  The upper left coordinate is 218, 6795.
    Lower left is 218, 6920, len is 312 samples.
*/
  /* First rectangle. Med antimpal 1 */
  block = IMSTART + 434;  /* First line of black rectangle in image 1 */

  for (i = 0; i <  CHAN_LINE_LEN * 3; i += 3)
    {
    bmp_buff[i]     = 0xFF; /* White */
    bmp_buff[i + 1] = 0xFF;
    bmp_buff[i + 2] = 0xFF;
    }
  rgb_yuv(CHAN_LINE_LEN);

  for (i = 0; i < 42; i++)
    {
    fileload((UL)(block + i));
    for (j = 233; j < 509; j++)
      {
      chan1_buff[j] = Ybuff[j];
      chan2_buff[j] = Ubuff[j];
      chan3_buff[j] = Vbuff[j];
      }
    filesave((UL)(block + i));
    }


  /* Second rectangle. Med antimpal 2 */
  block = IMSTART + 1010;  /* First line of black rectangle in image 2 */

  for (i = 0; i <  CHAN_LINE_LEN * 3; i += 3)
    {
    bmp_buff[i]     = 0xFF; /* White */
    bmp_buff[i + 1] = 0xFF;
    bmp_buff[i + 2] = 0xFF;
    }
  rgb_yuv(CHAN_LINE_LEN);

  for (i = 0; i < 42; i++)
    {
    fileload((UL)(block + i));
    for (j = 233; j < 509; j++)
      {
      chan1_buff[j] = Ybuff[j];
      chan2_buff[j] = Ubuff[j];
      chan3_buff[j] = Vbuff[j];
      }
    filesave((UL)(block + i));
    }


  /* Third rectangle. Without antimpal  */
  block = IMSTART + 1586;  /* First line of black rectangle in image 3 */

  for (i = 0; i <  CHAN_LINE_LEN * 3; i += 3)
    {
    bmp_buff[i]     = 0xFF; /* White */
    bmp_buff[i + 1] = 0xFF;
    bmp_buff[i + 2] = 0xFF;
    }
  rgb_yuv(CHAN_LINE_LEN);

  for (i = 0; i < 42; i++)
    {
    fileload((UL)(block + i));
    for (j = 233; j < 509; j++)
      {
      chan1_buff[j] = Ybuff[j];
      chan2_buff[j] = Ubuff[j];
      chan3_buff[j] = Vbuff[j];
      }
    filesave((UL)(block + i));
    }

#endif

/*********************************************************/
/*             N O W  L O G O S                          */
/*********************************************************/

/*
   First we fill the black rectangle in upper circle part with white, so 
   it does not interfere with logo.  The black rectangle upper left 
   coordinate is 292, 56 and it has 41 lines.  Its len is 164 samples.
*/
  block = IMSTART + 56;  /* First line of black rectangle */

/* */
  for (i = 0; i <  CHAN_LINE_LEN * 3; i += 3)
    {
    bmp_buff[i]     = 0xFF; /* White */
    bmp_buff[i + 1] = 0xFF;
    bmp_buff[i + 2] = 0xFF;
    }
  rgb_yuv(CHAN_LINE_LEN);

  for (i = 0; i < 42; i++)
    {
    fileload((UL)(block + i));
    for (j = 292; j < 456; j++)
      {
      chan1_buff[j] = Ybuff[j];
      chan2_buff[j] = Ubuff[j];
      chan3_buff[j] = Vbuff[j];
      }
    filesave((UL)(block + i));
    }

#if 1
  /* First logo (med antipal 1) */
  block = BLOCK; 

  for (i = 0; i < NR_LINES; i++)
    {
    fileload((UL)(block + i));
    loadlogo((UL)(i + LOGO_LINES_TO_SKIP), bmp_bytes_per_line, bmp_info.biHeight, bmp_header.bfOffBits);
    rgb_yuv(logo_len);


/* copy Ybuff....with filtered logo back to chanx_buff */

    k     = 0;
    start = LOGO_START;
    end   = LOGO_START + logo_len - 1 ;
    for (j = start; j < end; j++)
      {
      chan1_buff[j] = Ybuff[k];
      chan2_buff[j] = Ubuff[k];
      chan3_buff[j] = Vbuff[k];
      k++;
      }
    filesave((UL)(block + i));
    }
#endif


/*                      Second logo                           */
  block = IMSTART + 56 + 576;  /* First line of black rectangle */

/* */
  for (i = 0; i <  CHAN_LINE_LEN * 3; i += 3)
    {
    bmp_buff[i]     = 0xFF; /* White */
    bmp_buff[i + 1] = 0xFF;
    bmp_buff[i + 2] = 0xFF;
    }
  rgb_yuv(CHAN_LINE_LEN);

  for (i = 0; i < 42; i++)
    {
    fileload((UL)(block + i));
    for (j = 292; j < 456; j++)
      {
      chan1_buff[j] = Ybuff[j];
      chan2_buff[j] = Ubuff[j];
      chan3_buff[j] = Vbuff[j];
      }
    filesave((UL)(block + i));
    }

#if 1
  /* Second logo (med antipal 1) */
  block = BLOCK + 576; 

  for (i = 0; i < NR_LINES; i++)
    {
    fileload((UL)(block + i));
    loadlogo((UL)(i + LOGO_LINES_TO_SKIP), bmp_bytes_per_line, bmp_info.biHeight, bmp_header.bfOffBits);
    rgb_yuv(logo_len);

    k     = 0;
    start = LOGO_START;
    end   = LOGO_START + logo_len - 1 ;
    for (j = start; j < end; j++)
      {
	    chan1_buff[j] = Ybuff[k];
	    chan2_buff[j] = Ubuff[k];
	    chan3_buff[j] = Vbuff[k];
	    k++;
      }
   filesave((UL)(block + i));
    }
#endif


/************* 3rd logo ********************/
/*                      Third logo                           */
  block = IMSTART + 56 + 576 + 576;  /* First line of black rectangle */

/* */
  for (i = 0; i <  CHAN_LINE_LEN * 3; i += 3)
    {
    bmp_buff[i]     = 0xFF; /* White */
    bmp_buff[i + 1] = 0xFF;
    bmp_buff[i + 2] = 0xFF;
    }
  rgb_yuv(CHAN_LINE_LEN);

  for (i = 0; i < 42; i++)
    {
    fileload((UL)(block + i));
    for (j = 292; j < 456; j++)
      {
      chan1_buff[j] = Ybuff[j];
      chan2_buff[j] = Ubuff[j];
      chan3_buff[j] = Vbuff[j];
      }
    filesave((UL)(block + i));
    }

#if 1
  /* Third logo (wo antipal) */
  block = BLOCK + 576 + 576; 

  for (i = 0; i < NR_LINES; i++)
    {
    fileload((UL)(block + i));
    loadlogo((UL)(i + LOGO_LINES_TO_SKIP), bmp_bytes_per_line, bmp_info.biHeight, bmp_header.bfOffBits);
    rgb_yuv(logo_len);

    k     = 0;
    start = LOGO_START;
    end   = LOGO_START + logo_len - 1 ;
    for (j = start; j < end; j++)
      {
	    chan1_buff[j] = Ybuff[k];
	    chan2_buff[j] = Ubuff[k];
	    chan3_buff[j] = Vbuff[k];
	    k++;
      }
   filesave((UL)(block + i));
    }
#endif



  fileclose();
  close_logo_file();



/* Now we will produce a .BMP file containing entire Philips picture
   with logo.  We read each picture line, convert it to RGB and write
   to .BMP file.
*/

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

  bmp_header.bfSize = bmp_bytes_per_line * IMHEIGHT + sizeof(BITMAPFILEHEADER)
                      + sizeof(BITMAPINFOHEADER);
  bmp_info.biHeight    = IMHEIGHT;
  bmp_info.biSizeImage = IMLEN * IMHEIGHT;

  printf("FileSize is:   %lu\n", bmp_header.bfSize);

  write(out_fptr, &bmp_header, sizeof(BITMAPFILEHEADER));
  write(out_fptr, &bmp_info, sizeof(BITMAPINFOHEADER));

  block = OUT_IMSTART; /* Start of picture in channelx.dat files */

  for (i = block + IMHEIGHT; i > block; i--)
    {
    fileload((UL)(i));
    for (j = 0; j < IMLEN; j++)
      yuv_rgb(j);
    write(out_fptr, bmp_buff, bmp_bytes_per_line);
    }
  close_output_file();
  fileclose();
  printf("Created picture file %s\n", OUT_FILE);
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

/* This function opens all files used */
void fileopen(void)
  {
  if ((chan0_fptr = open("CHANNEL0.DAT", O_RDWR | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
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

void open_output_file(void)
  {
  unlink(OUT_FILE);
  out_fptr = open(OUT_FILE, O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE);
  
  if (out_fptr == -1)
    {
    puts("Cant open output file.\n");
    exit(1);
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
  close(chan0_fptr);
  close(chan1_fptr);
  close(chan2_fptr);
  close(chan3_fptr);
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
/*           (kernel_size / 2 + kernel_size) are unusable.
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
/* Function: process_logo(int len)                                         */
/* Purpose:  To filter logo YUV data
/*
/* Args:     len: length of logo data 
/*
/* Remarks:  On entry, Ybuff,,,, contains of logo YUV data
/*           
/*           On exit, that data is filtered
/*
/* Returns:  Nothing.
/*           
/* NOTES:    This function substracts DAC offsets from YUV data before
/*           calling filter function and then adds them again
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
    tem_flo_buff[i] = tem_flo_buff[i] - chan_params.DA1;
    }
  /* convolute() manipulates tem_flo_buff[] */
#if FILTER
  convolute(len);
#endif
  for (i = 0; i < len; i++)
    Ybuff[i] = (UI)(tem_flo_buff[i] + chan_params.DA1);

  /* Logo in U channel */
  for (i = 0; i < len; i++)
    {
    tem_flo_buff[i] = Ubuff[i];
    tem_flo_buff[i] = tem_flo_buff[i] - chan_params.DA2;
    }
#if FILTER
  convolute(len);
#endif
  for (i = 0; i < len; i++)
    Ubuff[i] = (UI)(tem_flo_buff[i] + chan_params.DA2);

  /* Logo in V channel */
  for (i = 0; i < len; i++)
    {
    tem_flo_buff[i] = Vbuff[i];
    tem_flo_buff[i] = tem_flo_buff[i] - chan_params.DA3;
    }
#if FILTER
  convolute(len);
#endif
  for (i = 0; i < len; i++)
    Vbuff[i] = (UI)(tem_flo_buff[i] + chan_params.DA3);
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
#endif
  }
