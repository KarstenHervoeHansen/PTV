/* FILE NAME        video.c
 * PURPOSE          To extract SDI video data of Philips 4x3 test picture
 *                  from channelx.dat files and dump the data into file
 *                  named philips.dat
 *
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * CODE INSP.
 * TEST
 * LAST UPDT:       17.11.2004
 * NOTES:           Output file is named philips.dat.
 *                  Invocation: video x
 *                   where x is a dummy parameter.
                    Data is placed in file named philips.dat

    Video data from 8633 generator is used.  "Billede til 8633" memory
    map document specifies start address and number of lines for 
    the Philips4x3 test patterns:

    PAL    start = 1382  nr. of lines = 576
    NTSC   start = 4804  nr. of lines = 488

    The program works as follows:

    Files are open/created.
    File pointer for channelx.dat file is moved to start.
    Line counter is zeored.
    One line of data is read from channelx.dat files into buffers.
    Data from buffers is read and written into output file in the
     following sequence: UYVYUYV.... as integers, alltogether 
     720 Y, 360 U and 360 V samples, function shuffle() does that.
    The process is repeated until line counter is 576 (PAL).
    Files are closed.

    The program must be compiled separately for PAL and NTSC, the 
    following defines parameters for PAL and NTSC

#if 0
  imheigth   = 576;
  start_addr = 1382;
#else
  imheigth   = 488;
  start_addr = 4804;
#endif

    To test the program, a separate one will be written, which will
    read data from philips.dat and make a bmp picture of it.
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


#define DAT_FILE "philips.dat"    /* file containing picture data */

#define NO_THRESHOLD 1
#define IMSTART     0

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
  UI * chanY_buff;
  UI * chanU_buff;
  UI * chanV_buff;
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
void open_output_file(FILE *);
void close_output_file(FILE *);
void fileopen(HANDLES *);
void fileload(UL, HANDLES, CHANBUFFERS, int);
void shuffle(CHANBUFFERS, UI *, CHANNEL0, int);
void alloc_all(CHANBUFFERS *, YUVBUFFERS *, UI **, int);
void free_all(CHANBUFFERS *, YUVBUFFERS *, UI *);
void give_info(void);
void filesave(UL line_num, HANDLES hands, CHANBUFFERS buff, int len);

void main(int argc, char *argv[])
  {
  CHANNEL0    chan_params;
  HANDLES     chan_handles;
  FILE *      dat_fptr;      /* handle to output .dat file */
  CHANBUFFERS ch_buffers;
  YUVBUFFERS  yuv_buffers;
  UI *        dat_buff;
  int         chan_line_len;

  int         imlen;      /* length of line in samples */
  int         imheigth;   /* heigth of image in lines  */
  int         start_addr; /* start address of image in channelx.dat files */
  int         line_no;
  int i;
  int j;

  BITMAPFILEHEADER bmp_header;
  BITMAPINFOHEADER bmp_info;
  UL bmp_bytes_per_line;
 
  if (argc != 2)
    {
    give_info();
    exit(2);
    }

  imlen      = 720;

/* Change here between Philips4x3 in PAL or NTSC */

#if 0
  imheigth   = 576;     /* (for PAL)*/
  start_addr = 1382;    /* (for PAL)*/
#else
  imheigth   = 488;     /* (for NTSC)*/
  start_addr = 4804;    /* (for NTSC)*/
#endif

  _fmode = O_BINARY; 

  printf("video: creates philips.dat file with Philips4x3 image data\n");
  printf("from channelx.dat files.\n");

  fileopen(&chan_handles);

  dat_fptr = fopen(DAT_FILE, "w");
  if (dat_fptr == NULL)
    {
    printf("Cannot open output file!\n");
    exit (1);
    }

 /* read params from channel0.dat */
  read(chan_handles.chan0, &chan_params, sizeof(CHANNEL0));
  chan_line_len = (int) chan_params.BS;

  alloc_all(&ch_buffers, &yuv_buffers, &dat_buff, chan_line_len);

  for (i = 0; i < chan_line_len; i++)
    {
    yuv_buffers.Ybuff[i] =0;
    yuv_buffers.Ubuff[i] =0;
    yuv_buffers.Vbuff[i] =0;
    }

#if 0
/*********************************************************/
/*  Burst Gate Check elimination                         */
/*  Copy 70 samples of lines 413 - 452 into 70 samples   */
/*  of lines 329 - 368                                   */
/*********************************************************/
  printf("Burst Gate Check elimination\n");
  for (i = 0; i < 41; i++)
    {
    fileload((UL)(i), chan_handles, ch_buffers, chan_line_len);

    fileload((UL)((IMSTART + 413) + i), chan_handles, ch_buffers, chan_line_len); /* load lines w/o BGC */
    for (j = 0; j < 70; j++)             /* copy 70 samples to buffers */
      {
      yuv_buffers.Ybuff[j] = ch_buffers.chanY_buff[j];
      yuv_buffers.Ubuff[j] = ch_buffers.chanU_buff[j];
      yuv_buffers.Vbuff[j] = ch_buffers.chanV_buff[j];
      }

    fileload((UL)((IMSTART + 329) + i), chan_handles, ch_buffers, chan_line_len); /* load lines with BGC */
    for (j = 0; j < 70; j++)             /* copy 70 samples from buffers */
      {
      ch_buffers.chanY_buff[j] = yuv_buffers.Ybuff[j];
      ch_buffers.chanU_buff[j] = yuv_buffers.Ubuff[j];
      ch_buffers.chanV_buff[j] = yuv_buffers.Vbuff[j];
      }

    filesave((UL)((IMSTART + 329) + i), chan_handles, ch_buffers, chan_line_len);
    }


  for (i = 0; i < 41; i++)
    {
    fileload((UL)((IMSTART + 989) + i), chan_handles, ch_buffers, chan_line_len); /* load lines w/o BGC */
    for (j = 0; j < 70; j++)             /* copy 70 samples to buffers */
      {
      yuv_buffers.Ybuff[j] = ch_buffers.chanY_buff[j];
      yuv_buffers.Ubuff[j] = ch_buffers.chanU_buff[j];
      yuv_buffers.Vbuff[j] = ch_buffers.chanV_buff[j];
      }

    fileload((UL)((IMSTART + 905) + i), chan_handles, ch_buffers, chan_line_len); /* load lines with BGC */
    for (j = 0; j < 70; j++)             /* copy 70 samples from buffers */
      {
      ch_buffers.chanY_buff[j] = yuv_buffers.Ybuff[j];
      ch_buffers.chanU_buff[j] = yuv_buffers.Ubuff[j];
      ch_buffers.chanV_buff[j] = yuv_buffers.Vbuff[j];
      }
    filesave((UL)((IMSTART + 905) + i), chan_handles, ch_buffers, chan_line_len);
    }

  /* Now eliminate Burst Gate in Philips NTSC */

  for (i = 0; i < 34; i++)
    {
    fileload((UL)(i), chan_handles, ch_buffers, chan_line_len);

    fileload((UL)((IMSTART + 4964) + i), chan_handles, ch_buffers, chan_line_len); /* load lines w/o BGC */
    for (j = 0; j < 70; j++)             /* copy 70 samples to buffers */
      {
      yuv_buffers.Ybuff[j] = ch_buffers.chanY_buff[j];
      yuv_buffers.Ubuff[j] = ch_buffers.chanU_buff[j];
      yuv_buffers.Vbuff[j] = ch_buffers.chanV_buff[j];
      }

    fileload((UL)((IMSTART + 4896) + i), chan_handles, ch_buffers, chan_line_len); /* load lines with BGC */
    for (j = 0; j < 70; j++)             /* copy 70 samples from buffers */
      {
      ch_buffers.chanY_buff[j] = yuv_buffers.Ybuff[j];
      ch_buffers.chanU_buff[j] = yuv_buffers.Ubuff[j];
      ch_buffers.chanV_buff[j] = yuv_buffers.Vbuff[j];
      }

    filesave((UL)((IMSTART + 4896) + i), chan_handles, ch_buffers, chan_line_len);
    }
  fclose(dat_fptr);
  fileclose(chan_handles);
  printf("Burst Gate Eliminated.\n");
  free_all(&ch_buffers, &yuv_buffers, dat_buff); 
  exit(0);
#endif


  /*  main loop */
  line_no = 0;
  for (i = start_addr; i < imheigth + start_addr; i++)
    {
    fileload((UL)(i), chan_handles, ch_buffers, chan_line_len);

    shuffle(ch_buffers, dat_buff, chan_params, 720);
    line_no++;
#if 0
    fprintf(dat_fptr, "Line = %d\n", line_no); 
    fprintf(dat_fptr, "SAV[0] = %03X\n", dat_buff[0]);
    fprintf(dat_fptr, "SAV[1] = %03X\n", dat_buff[1]);
    fprintf(dat_fptr, "SAV[2] = %03X\n", dat_buff[2]);
    fprintf(dat_fptr, " XY    = %03X\n", dat_buff[3]);
#endif

    for (j = 0; j < 1444; j++)
      fprintf(dat_fptr, "%03X\n", dat_buff[j]);


    }
  fclose(dat_fptr);
  fileclose(chan_handles);

  free_all(&ch_buffers, &yuv_buffers, dat_buff); 

  exit(0);
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

void give_info(void)
  {
  printf("video: creates philips.dat file with Philips4x3 image data\n");
  printf("from channelx.dat files.\n");
  printf("Invokation:\n");
  printf("video x\n");
  printf("where x is a dummy parameter\n");
  }


void free_all(CHANBUFFERS * channel, YUVBUFFERS * yuv, UI * bmp)
  {
#if 1
  free((UI*)channel->chanY_buff);
  free((UI*)channel->chanU_buff);
  free((UI*)channel->chanV_buff);
  free((UI*)yuv->Ybuff);
  free((UI*)yuv->Ubuff);
  free((UI*)yuv->Vbuff);
#endif
  free((UI*)bmp);
  }


void alloc_all(CHANBUFFERS * channel, YUVBUFFERS * yuv, UI ** bmp, int len)
  {

  if ((channel->chanY_buff = (UI*)malloc(len * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  if ((channel->chanU_buff = (UI*)malloc(len * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  if ((channel->chanV_buff = (UI*)malloc(len * sizeof(UI))) == NULL)
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

  if ((*bmp = (UI*)malloc(3 * 720 * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }

  }



/* This function opens channelx.dat files used */
void fileopen(HANDLES * file_handles)
  {
  if ((file_handles->chan0 = open("CHANNEL0.DAT", O_RDWR | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((file_handles->chan1 = open("CHANNEL1.DAT", O_RDWR | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((file_handles->chan2 = open("CHANNEL2.DAT", O_RDWR | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
    exit(1);
    }
  if ((file_handles->chan3 = open("CHANNEL3.DAT", O_RDWR | O_BINARY)) == -1)
    {
    puts("Cant open input file.\n");
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
  result = read(hands.chan1, buff.chanY_buff, len * 2);
  if (result == 0)
    printf("Error reading file, linenum = %lu\n", line_num);

  lseek(hands.chan2, (line_num * len * 2), SEEK_SET);
  result = read(hands.chan2, buff.chanU_buff, len * 2);

  lseek(hands.chan3, (line_num * len * 2), SEEK_SET);
  result = read(hands.chan3, buff.chanV_buff, len * 2);
  }

void filesave(UL line_num, HANDLES hands, CHANBUFFERS buff, int len)
  {

  lseek(hands.chan1, (line_num * len * 2), SEEK_SET);
  write(hands.chan1, buff.chanY_buff, len * 2);
  lseek(hands.chan2, (line_num * len * 2), SEEK_SET);
  write(hands.chan2, buff.chanU_buff, len * 2);
  lseek(hands.chan3, (line_num * len * 2), SEEK_SET);
  write(hands.chan3, buff.chanV_buff, len * 2);
  }

/********************************************************************************/
/* Function: void shuffle(CHANBUFFERS in_buff, UI * bmp, CHANNEL0 ch0, int imlen)
/*
/* Purpose:  To read YUV data from channel buffers and place them in output
/*           buffer in the sequence specified by SDI standard
/*
/* Args:     in_buff: structure of pointers to input data buffers
/*           bmp: poiter to output data
/*           ch0: struct containing pointers to channel0 data
/*           imlen: nr of samples to tackle (720)
/*
/* Returns:  Nothing.
/*           
/* NOTES:    Picture below shows the sequence in which particular samples 
/*           are loaded into out_buff (UYVYUYVY...).  Numbers in |X| indicate
/*           the sequence of loading.  Components which have | | are not 
/*           loaded.

                 j=0     j=1     j=2    j=3    j=4   
                  |       |       |      |      |
            Y    |2|     |4|     |6|    |8|    |10|
                  |       |       |      |      |
            U    |1|     | |     |5|    | |    |9 |
                  |       |       |      |      |
            V    | |     |3|     | |    |7|    |  |
                  |       |       |      |      |

/*           
/********************************************************************************/
void shuffle(CHANBUFFERS in_buff, UI * out_buff, CHANNEL0 ch0, int imlen)
  {
  int j;
  int i;

  i =  0;
  
  /* We use offset of 14 samples here, as original picture contains some
     dummy data at the beigning of each line.
  */
  for (j = 14; j < imlen + 4 + 14; j++)
    {
    if (j % 2)
      {         /* if odd */                     /* j = 1, 3, 5, 7, 9....*/
      out_buff[i++] = in_buff.chanV_buff[j];     /* V  3 */
      out_buff[i++] = in_buff.chanY_buff[j];     /* Y  4 */
      }
    else
      {          /* if even */                   /* j = 0, 2, 4, 6, 8....*/
      out_buff[i++] = in_buff.chanU_buff[j];     /* U  1 */
      out_buff[i++] = in_buff.chanY_buff[j];     /* Y  2 */
      }
    }
#if 0
  printf("imlen = %d, i = %d\n", imlen, i);
#endif
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

