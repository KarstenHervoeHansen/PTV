/* FILE NAME        ch2bmp.c
 * PURPOSE          Generic program to make a .bmp file from channelx.dat.
 *                  files
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       2.10.2002
 * NOTES:           Output file is named dump.bmp.
 *                  Invocation: ch2bmp imlen imheigth imstart
                    imlen    - length of image (samples)
                    imheigth - heigth of image (lines)
                    imstart  - start address of image in channelx.dat
  
    NO_THRESHOLD is used to control how the yuv_rgb() funcion works.
    if NO_THRESHOLD = 1, then the following algorithm will be used:

        #if NO_THRESHOLD
            if (G < 0.0)
              G = 0.0;
            if (G > 700.0)
              G = 700.0;
                 .
                 .

     and R, G and B values will be limited.  This means that if the picture
     contains some other elements, like subcarrier burst or sync, they will
     not be visible in .bmp file.

   if NO_THRESHOLD = 0, this algorithm will not be used and the .bmp file 
   will show these other elements, which may; be useful to see what is in
   the picture besides "picture".

 *                  
 *                  
 *                  
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long


#define OUT_FILE "dump.bmp"    /* Output file containing picture */

#define NO_THRESHOLD 1

/* Structure to hold info from channel0.dat file */
typedef struct tagCHANNEL0
  {
  double IFS;  /* inner first sample      */
  double ILS;  /* inner last sample       */
  double LLS;  /* line last sample        */
  double BS;   /* block size              */
  double LO;   /* line offset             */
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

/* handles to input files */
typedef struct tagHANDLES
  {
  int chan0;
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
void open_output_file(int *);
void close_output_file(int);
void fileopen(HANDLES *);
void fileload(UL, HANDLES, CHANBUFFERS, int);
void yuv_rgb(CHANBUFFERS, UC *, CHANNEL0, int);
void alloc_all(CHANBUFFERS *, YUVBUFFERS *, UC **, int);
void free_all(CHANBUFFERS *, YUVBUFFERS *, UC *);
void give_info(void);

void main(int argc, char *argv[])
  {
  CHANNEL0    chan_params;
  HANDLES     chan_handles;
  int         out_fptr;      /* handle to output .bmp file */
  CHANBUFFERS ch_buffers;
  YUVBUFFERS  yuv_buffers;
  UC *        bmp_buff;
  int         chan_line_len;

  int         imlen;
  int         imheigth;
  int         start_addr;

  int i;
  int j;
  int block;
  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info;
  UL bmp_bytes_per_line;
 
  if (argc != 4)
    {
    give_info();
    exit(2);
    }

  imlen      = atoi(argv[1]);
  imheigth   = atoi(argv[2]);
  start_addr = atoi(argv[3]);

  _fmode = O_BINARY; 

  printf("ch2bmp: creates dump.bmp file with image from channelx.dat files.\n");

  fileopen(&chan_handles);
  open_output_file(&out_fptr);

 /* read params from channel0.dat */
  read(chan_handles.chan0, &chan_params, sizeof(CHANNEL0));
  chan_line_len = (int) chan_params.BS;

  alloc_all(&ch_buffers, &yuv_buffers, &bmp_buff, chan_line_len);

  for (i = 0; i < chan_line_len; i++)
    {
    yuv_buffers.Ybuff[i] =0;
    yuv_buffers.Ubuff[i] =0;
    yuv_buffers.Vbuff[i] =0;
    }

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
/* Our picture is imlen x imheigth, we fill bmp_info and bmp_header structures 
   and write them into output file 
*/
/* The number of bytes in each line of a .BMP file is always
   a multiple of 4 as Windows does DoubleWord Alignment on line
   boundaries. You need a variable bmp_bytes_per_line that specifies
   how many bytes there are in a line.
*/

  bmp_info.biWidth   = imlen;
  bmp_bytes_per_line = bmp_info.biWidth * 3;
  if (bmp_bytes_per_line & 0x0003)
    {
    bmp_bytes_per_line |= 0x0003;
    ++bmp_bytes_per_line;
    }

  bmp_header.bfSize    = bmp_bytes_per_line * imheigth + sizeof(BITMAPFILEHEADER)
                         + sizeof(BITMAPINFOHEADER); 

  bmp_info.biHeight    = imheigth;
  bmp_info.biSizeImage = imlen * imheigth;

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

  

/************
  printf("FileSize is:   %lu\n", bmp_header.bfSize);*/

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

  /* Write headers into output bmp file */
  write(out_fptr, &bmp_header, sizeof(BITMAPFILEHEADER));
  write(out_fptr, &bmp_info,   sizeof(BITMAPINFOHEADER));

  block = start_addr; /* Start of picture in channelx.dat files */

  for (i = block + imheigth - 1; i >= block; i--)
    {
    fileload((UL)(i), chan_handles, ch_buffers, chan_line_len);

    yuv_rgb(ch_buffers, bmp_buff, chan_params, imlen);

    write(out_fptr, bmp_buff, bmp_bytes_per_line);
    }
  close_output_file(out_fptr);
  fileclose(chan_handles);

  free_all(&ch_buffers, &yuv_buffers, bmp_buff); 

  exit(0);
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

void give_info(void)
  {
  printf("ch2bmp: creates dump.bmp file with image from channelx.dat files.\n");
  printf("Arguments:\n");
  printf("          imlen    - length of image (samples)\n");
  printf("          imheigth - heigth of image (lines)\n");
  printf("          imstart  - start address of image in channelx.dat\n");
  }


void free_all(CHANBUFFERS * channel, YUVBUFFERS * yuv, UC * bmp)
  {
#if 1
  free((UI*)channel->chan1_buff);
  free((UI*)channel->chan2_buff);
  free((UI*)channel->chan3_buff);
  free((UI*)yuv->Ybuff);
  free((UI*)yuv->Ubuff);
  free((UI*)yuv->Vbuff);
#endif
  free((UC*)bmp);
  }


void alloc_all(CHANBUFFERS * channel, YUVBUFFERS * yuv, UC ** bmp, int len)
  {

  if ((channel->chan1_buff = (UI*)malloc(len * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  if ((channel->chan2_buff = (UI*)malloc(len * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  if ((channel->chan3_buff = (UI*)malloc(len * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }

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

  if ((*bmp = (UC*)malloc(3 * len * sizeof(UC))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }

  }



/* This function opens channelx.dat files used */
void fileopen(HANDLES * file_handles)
  {
  if ((file_handles->chan0 = open("CHANNEL0.DAT", O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((file_handles->chan1 = open("CHANNEL1.DAT", O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((file_handles->chan2 = open("CHANNEL2.DAT", O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((file_handles->chan3 = open("CHANNEL3.DAT", O_RDONLY | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  }

void open_output_file(int * hand)
  {
  unlink(OUT_FILE);

  *hand = open(OUT_FILE, O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE);
  
  if (*hand == -1)
    {
    puts("Cant open output file.\n");
    exit(1);
    }
  }


void fileclose(HANDLES hand)
  {
  close(hand.chan0);
  close(hand.chan1);
  close(hand.chan2);
  close(hand.chan3);
  }

void close_output_file(int hand)
  {
  close(hand);
  }
  

/**************************************************************************/
/* Function: fileload()                                                   */
/* Purpose:  To load one line data from channelx.dat files into
/*           respective chanx_buff buffers.
/* Args:     UL line_num - number of line which data to load.
/*           hands: handles to chanelx.dat files
/*           buff: poiters to data buffers
/*           len:  nr of samples to load (UI)
/*
/* Remarks:  Data is 16 bit samples (UI), there are CHAN_LINE_LEN of UI
/*           (2 bytes) in a line.
/*           On exit, chanx_buffs are filled with len of UIs
/*           If, due to errorneous params, EOF is passed, this function
/*           prints a warnning.
/* Returns:  Nothing.
/*
/**************************************************************************/
void fileload(UL line_num, HANDLES hands, CHANBUFFERS buff, int len)
  {
  int result;

  lseek(hands.chan1, (line_num * len * 2), SEEK_SET);
  result = read(hands.chan1, buff.chan1_buff, len * 2);
  if (result == 0)
    printf("Error reading file, linenum = %lu\n", line_num);

  lseek(hands.chan2, (line_num * len * 2), SEEK_SET);
  result = read(hands.chan2, buff.chan2_buff, len * 2);

  lseek(hands.chan3, (line_num * len * 2), SEEK_SET);
  result = read(hands.chan3, buff.chan3_buff, len * 2);
  }

/**************************************************************************/
/* Function: yuv_rgb(int sample)                                          */
/* Purpose:  To convert onse line of  YUV data to RGB
/*
/* Args:     in_buff: pointer struct to input data
/*           bmp: poiter to output data (RGB)
/*           ch0: struct containing channel0 data
/*           imlen: nr of samples to convert
/*
/* Remarks:  sample is read from chanx_buffs
/*           it is converted to RGB and,
/*           RGB data is placed at sample * 3 in bmp
/*
/* Returns:  Nothing.
/*           
/* NOTES:    None
/*           
/**************************************************************************/

void yuv_rgb(CHANBUFFERS in_buff, UC * bmp, CHANNEL0 ch0, int imlen)
  {
  double R;
  double G;
  double B;
  double Y;
  double U;
  double V;
  int    j;

  for (j = 0; j < imlen; j++)
    {
    Y = ch0.DR1 * (in_buff.chan1_buff[j] - ch0.DA1);
    U = ch0.DR2 * (in_buff.chan2_buff[j] - ch0.DA2);
    V = ch0.DR3 * ((in_buff.chan3_buff[j] & 0x3FF) - ch0.DA3);
  
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
    bmp[j * 3]     = (UC) (B / 700.0 * 255.0);
    bmp[j * 3 + 1] = (UC) (G / 700.0 * 255.0);
    bmp[j * 3 + 2] = (UC) (R / 700.0 * 255.0);
    }
  }

