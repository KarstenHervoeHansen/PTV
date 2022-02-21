/* FILE NAME        bmp2chan.c
 * PURPOSE          Generic program to make a set of channel.x files from 
 *                  a bmp file.  For 8633 moving logo projrct.
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       4.11.2002
 * NOTES:           The input bmp file is tdc.bmp, 256x44, 24 bit/pixel.
 *                  The image is already filtered.        
 *                  This program produces set of channel.x.dat files
 *                  out of the bmp file.
 *                  Invocation: bmp2chan x, where x is a dummy parameter.
 *
 *  !!!!! Could not get this to function properly.  Makelogo program was !!
 *  !!!!! modified to perform the same function (makechan.c) !!!!!!!!!!!!!!
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


#define IN_FILE "tdc.bmp"    /* Input file containing picture */

#define IMLEN     (int)256        /* Length of input image in pixels */
#define IMHEIGHT  44         /* number of lines in image */

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

/* handles to output files */
typedef struct tagHANDLES
  {
  int chan1;
  int chan2;
  int chan3;
  } HANDLES;

/* channelx buffers */
typedef struct tagCHANBUFFERS
  {
  UI * chan1_buff;
  UI * chan2_buff;
  UI * chan3_buff;
  } CHANBUFFERS;

/* auxilary buffers */
typedef struct tagYUVBUFFERS
  {
  UI * Ybuff;
  UI * Ubuff;
  UI * Vbuff;
  } YUVBUFFERS;


/* Protos */
void fileclose(HANDLES);
void open_input_file(BITMAPFILEHEADER*, BITMAPINFOHEADER*, int*);
void close_input_file(int);
void fileopen(HANDLES *);
void loadlogo(UL, int, char*, UL, UL, int);
void rgb_yuv(YUVBUFFERS, char *, int);
void alloc_all(YUVBUFFERS *, char **, int, int);
void free_all(YUVBUFFERS *, char *);
void give_info(void);

void main(int argc, char** argv)
  {
  HANDLES     chan_handles;
  int         in_fptr;      /* handle to input .bmp file */
  YUVBUFFERS  yuv_buffers;
  char *      bmp_buff;
  int         chan_line_len;
  int         bmp_line_len;

  int              i;
  int              j;
  int              block;
  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info;
  UL               bmp_bytes_per_line;
 

  if (argc != 2)
    {
    give_info();
    exit(1);
    }

  printf("Creating channelx.dat files for %s\n", IN_FILE);

  _fmode = O_BINARY; 

  fileopen(&chan_handles);    /* open channelx.dat files */  
  open_input_file(&bmp_header, &bmp_info, &in_fptr);  /* open logo bmp.file */

  bmp_bytes_per_line = bmp_info.biWidth;
  if (bmp_bytes_per_line & 0x0003)
    {
    bmp_bytes_per_line |= 0x0003;
    ++bmp_bytes_per_line;
    }
  
  chan_line_len = IMLEN * 2;

  alloc_all(&yuv_buffers, &bmp_buff, chan_line_len, bmp_bytes_per_line);

  /* fill buffers with white */
  for (i = 0; i < IMLEN; i++)
    {
    yuv_buffers.Ybuff[i] = 940;
    yuv_buffers.Ubuff[i] = 512;
    yuv_buffers.Vbuff[i] = 512;
    }


               /* Main Loop */
  block = 0; /* Start of picture in channelx.dat files */

  for (i = 0; i < IMHEIGHT; i++)
    {
#if 1
    loadlogo((UL)(block), in_fptr, bmp_buff, bmp_bytes_per_line, bmp_info.biHeight, bmp_header.bfOffBits);
    /* convert color logo data to YUV  */
    rgb_yuv(yuv_buffers, bmp_buff, (int) bmp_bytes_per_line / 3);
#endif
    /* save result in temporary files channelx.DAT */
    write(chan_handles.chan1, yuv_buffers.Ybuff, chan_line_len);
    write(chan_handles.chan2, yuv_buffers.Ubuff, chan_line_len);
    write(chan_handles.chan3, yuv_buffers.Vbuff, chan_line_len);
    block++;
    }
  close_input_file(in_fptr);
  fileclose(chan_handles);

  free_all(&yuv_buffers, bmp_buff); 

  exit(0);
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/
/**************************************************************************/
/* Function: loadlogo()                                                   */
/* Purpose:  To load one line of logo data from a .bmp file into
/*           bmp line buffer.
/* Args:     UL line_num - number of line which data to load.
/*           int fptr    - handle to bmp file
/*           char * bmp_buff   - poiter to data buffer
/*           UL bytes_per_line - length of bmp line
/*           UL lines_in_bmp   - nr of lines in bmp image
/*           UL bmp_offset     - offset to picture data from beg of file
/*
/* NOTES:    The picture is stored up side down in a .BMP file, i.e.
/*           first upper picture line is stored as last line in file.
/*           Therefore we need lines_in_bmp to calculate offset of line
/*           in file.
/* Returns:  Nothing.
/*
/**************************************************************************/
void loadlogo(UL line_num, int fptr, char * bmp_buff, UL bytes_per_line, UL lines_in_bmp, int bmp_offset)
  {
  long z;
  int  result;

  z = (long)bmp_offset + ((lines_in_bmp - 1 - line_num) * bytes_per_line);

  lseek(fptr, (z), SEEK_SET);

  result = read(fptr, bmp_buff, bytes_per_line);
  if (result == 0)
    printf("Error reading file, line = %lu", line_num);
  }

void give_info(void)
  {
  printf("bmp2chan: creates set of channelx.files from a bmp file.\n");
  printf("Invocation:\n");
  printf("          bmp2chan x, where x is a dummy parameter.\n");
  }


void free_all(YUVBUFFERS * yuv, char * bmp)
  {
#if 1
  free((UI*)yuv->Ybuff);
  free((UI*)yuv->Ubuff);
  free((UI*)yuv->Vbuff);
#endif
  free((char*)bmp);
  }


void alloc_all(YUVBUFFERS * yuv, char ** bmp, int len, int bmp_len)
  {

  if ((yuv->Ybuff = (UI*)malloc(len * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  if ((yuv->Ubuff = (UI*)malloc(len * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  if ((yuv->Vbuff = (UI*)malloc(len * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }

  if ((*bmp = (char*)malloc(bmp_len * sizeof(char))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }

  }



/* This function opens all channel files used for reading and writing */
void fileopen(HANDLES * file_handles)
  {
  if ((file_handles->chan1 = open("CHANNEL1.DAT", O_WRONLY | O_BINARY | O_CREAT, S_IWRITE)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((file_handles->chan2 = open("CHANNEL2.DAT", O_WRONLY | O_BINARY | O_CREAT, S_IWRITE)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((file_handles->chan3 = open("CHANNEL3.DAT", O_WRONLY | O_BINARY | O_CREAT, S_IWRITE)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  }

void open_input_file(BITMAPFILEHEADER* header, BITMAPINFOHEADER* info, int * hand)
  {
  *hand = open(IN_FILE, O_RDONLY | O_BINARY);
  
  if (*hand == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
/* load bmp info and header */
  read(* hand, header, sizeof(BITMAPFILEHEADER));
  read(* hand, info, sizeof(BITMAPINFOHEADER));
  }


void fileclose(HANDLES hand)
  {
  close(hand.chan1);
  close(hand.chan2);
  close(hand.chan3);
  }

void close_input_file(int hand)
  {
  close(hand);
  }
  

/**************************************************************************/
/* Function: rgb_yuv()                                                    */
/* Purpose:  To convert logo data from B&W to YUV and scale down logo
/*           at the same time.
/* Args:     int logo_len: nr of samples in logo in our picture
/*
/* Remarks:  On entry, bmp contains logo  data.
/*           len is bmp image len in pixels.
/*           On exit, Ybuff... are filled with logo YUV
/*           This function is direct translation of a procedure used in
/*           TELECOPY.PAS.
/* Returns:  Nothing.
/*           
/**************************************************************************/

#if 1
/**/
void rgb_yuv(YUVBUFFERS yuv_buffs, char * bmp, int len)
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

  for (i = 0; i < (len); i++)
    {
    z = 3 * i;
    B = (double) bmp[z];
    z++;
    G = (double) bmp[z];
    z++;
    R = (double) bmp[z];

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

    yuv_buffs.Ybuff[i] = (UI) (Y * 876.0 / 255.0 + 64.0);
    yuv_buffs.Ubuff[i] = (UI) (U * 448.0 / 128.0 + 512.0);
    yuv_buffs.Vbuff[i] = (UI) (V * 448.0 / 128.0 + 512.0);
    }
  }
#endif