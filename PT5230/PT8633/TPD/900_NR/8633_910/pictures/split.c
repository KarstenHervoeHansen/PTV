/* FILE NAME        split.c
 * PURPOSE          program to split channelx.dat files into mlogo.dat file
 *                  for moving logo in 8633.
 *                  Output file is Mlogo.dat
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 * TEST
 * LAST UPDT:       13.10.2002
 * NOTES:           This program implements the same algorithm as
 *                  tkdsplit.pas. It converts data from 10 bit word
 *                  to 2 x 8 bit bytes and places them in the manner
 *                  described in prom_split() function.
                    Y,U,V components are placed in following sequence,
                    as required by standard for SDI format:

                    z =  0 PROM[0]  = Y(0) = 940
                    z =  1 PROM[1]  = V(0) = 512
                    z =  2 PROM[2]  = Y(1) = 940
                    z =  3 PROM[3]  = U(2) = 512
                    z =  4 PROM[4]  = Y(2) = 940
                    z =  5 PROM[5]  = V(2) = 512
                    z =  6 PROM[6]  = Y(3) = 940
                    z =  7 PROM[7]  = U(4) = 512
                    z =  8 PROM[8]  = Y(4) = 940
                    z =  9 PROM[9]  = V(4) = 512
                    z = 10 PROM[10] = Y(5) = 940
                    z = 11 PROM[11] = U(6) = 512
                    z = 12 PROM[12] = Y(6) = 940
                    z = 13 PROM[13] = V(6) = 512

 *                  
 */
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long

#define LOGO_LEN  256         /* length of logo line in channelx.dat in samples */
#define NR_LINRES  44

#define OUT_FILE "Mlogo.dat"  /* Output file containing split logo */


/* handles to input files */
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


/* Protos */
void prom_split(CHANBUFFERS, UC *);
void fileclose(HANDLES);
void open_output_file(int *);
void close_output_file(int);
void fileopen(HANDLES *);
void fileload(UL, HANDLES, CHANBUFFERS, int);
void alloc_all(CHANBUFFERS *, UC **, int, int);
void free_all(CHANBUFFERS *, UC *);
void give_info(void);

void main(void)
  {
  HANDLES     chan_handles;
  int         out_fptr;      /* handle to output prom file */
  CHANBUFFERS ch_buffers;    /* buffers for channelx.dat data */
  UC *        prom_buff;
  int i;
  int j;
  int block;
  int result;
  int         chan_line_len;
  int         prom_line_len;
  long        pos;
 
  UC * sound; /* table to hold logo data for test purpose */


  printf(" split: splits logo data from channelx.dat files into Mlogo.dat\n");

  _fmode = O_BINARY; 

  chan_line_len = LOGO_LEN;
  prom_line_len = LOGO_LEN;

  fileopen(&chan_handles);
  open_output_file(&out_fptr);

  alloc_all(&ch_buffers, &prom_buff, chan_line_len, prom_line_len * 4);

  /* Zero prom memory */
  for (i = 0; i < prom_line_len * 4; i++)
    prom_buff[i] = 0x00;
  

  /*                        MAIN LOOP                       */

  for (i = 0; i < NR_LINRES; i++)
    {
    fileload((UL)(i), chan_handles, ch_buffers, LOGO_LEN);

    prom_split(ch_buffers, prom_buff);

    pos = (long)(4 * LOGO_LEN) * (long)i;
    lseek(out_fptr, pos, SEEK_SET); 
    result = write(out_fptr, prom_buff, (4 * LOGO_LEN));
    if (result == -1)
      printf("Error writing file, line = %d\n", i);
    }

  close_output_file(out_fptr);
  fileclose(chan_handles);

  free_all(&ch_buffers, prom_buff); 
  printf("Done %d lines\n", i);
  exit(0);
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

void prom_split(CHANBUFFERS in_buffs, UC * prom)
  {
  int i;
  int z;
  UI  temp;

  z = 0;
  for (i = 0; i < LOGO_LEN; i++)
    {
    /* printf("z = %d PROM[%d] = Y(%d) = %d\n", z, z, i, in_buffs.chan1_buff[i]); */
    prom[z]                  = (UC) (in_buffs.chan1_buff[i] & (UI)0x00FF);
    prom[z + (2 * LOGO_LEN)] = (UC) ((in_buffs.chan1_buff[i] >> 8) & (UI)0x0003);
    z++;
    if ((i % 2) == 0)
      {
      /* odd (V) */
      /* printf("z = %d PROM[%d] = V(%d) = %d\n", z, z, i, in_buffs.chan3_buff[i]); */
      prom[z]                  = (UC) (in_buffs.chan3_buff[i] & (UI)0x00FF);
      prom[z + (2 * LOGO_LEN)] = (UC) ((in_buffs.chan3_buff[i] >> 8) & (UI)0x0003);
      z++;
      }
    else
      {
      /* even (U) */
      /* printf("z = %d PROM[%d] = U(%d) = %d\n", z, z, i + 1, in_buffs.chan2_buff[i + 1]); */
      if (i == LOGO_LEN -1)
        {
        prom[z]                  = (UC) (in_buffs.chan2_buff[0] & (UI)0x00FF);
        prom[z + (2 * LOGO_LEN)] = (UC) ((in_buffs.chan2_buff[0] >> 8) & (UI)0x0003);
        }
      else
        {
        prom[z]                  = (UC) (in_buffs.chan2_buff[i + 1] & (UI)0x00FF);
        prom[z + (2 * LOGO_LEN)] = (UC) ((in_buffs.chan2_buff[i + 1] >> 8) & (UI)0x0003);
        }
#if 0
      if (i == (LOGO_LEN - 1))
        printf("%d\n", prom[z + (2 * LOGO_LEN)]);
#endif
      z++;
      /* Note that on the last run, the above will read data past the buffer
         length. (in_buffs.chan2_buff[i + 1]).  Therefore we have an algorithm
         to roll over to 0 in this case. */
      }
    }
  }

void give_info(void)
  {
  printf("split: splits data from channelx.dat files.\n");
  }


void free_all(CHANBUFFERS * channel, UC * bmp)
  {
  free((UI*)channel->chan1_buff);
  free((UI*)channel->chan2_buff);
  free((UI*)channel->chan3_buff);
  free((UC*)bmp);
  }

/* Allocate memory for channel data buffers and output buffer */
void alloc_all(CHANBUFFERS * channel, UC ** bmp, int len, int promlen)
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

  if ((*bmp = (UC*)malloc(promlen * sizeof(UC))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }

  }



/* This function opens channelx.dat files used */
void fileopen(HANDLES * file_handles)
  {
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
  *hand = open(OUT_FILE, O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE);
  
  if (*hand == -1)
    {
    puts("Cant open output file.\n");
    exit(1);
    }
  }


void fileclose(HANDLES hand)
  {
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
/* Remarks:  Data is 16 bit samples (UI), there are CHAN_LINE_LEN of UI
/*           (2 bytes) in a line.
/*           On exit, chanx_buffs are filled with len of UIs
/*           If, due to errorneous params, EOF is passed, this function
/*           prints a warnning.
/* Returns:  Nothing.
/* NOTES:    
/*
/**************************************************************************/
void fileload(UL line_num, HANDLES hands, CHANBUFFERS buff, int len)
  {
  int result;

  lseek(hands.chan1, (line_num * len * 2), SEEK_SET);
  result = read(hands.chan1, buff.chan1_buff, len * 2);
  if (result != len * 2)
    printf("Error reading file, linenum = %lu, see the source.\n", line_num);

  lseek(hands.chan2, (line_num * len * 2), SEEK_SET);
  result = read(hands.chan2, buff.chan2_buff, len * 2);

  lseek(hands.chan3, (line_num * len * 2), SEEK_SET);
  result = read(hands.chan3, buff.chan3_buff, len * 2);
  }


