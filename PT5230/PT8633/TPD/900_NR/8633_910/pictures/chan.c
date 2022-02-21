/* FILE NAME        chan.c
 * PURPOSE          To generate set of channelx.dat files with known data
 *                  
 * SPEC             Janusz Kuzminski
 * DESIGN           Janusz Kuzminski
 * CODING           Janusz Kuzminski
 *
 * TEST
 * LAST UPDT:       2.10.2002
 *
 * NOTES:           
 *                  
 */
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include <fcntl.h>
#include <sys\stat.h>

#define UI unsigned int
#define UL unsigned long


#define IN_FILE "teledenb.bmp"    /* Output file containing picture */

#define CHAN_LEN 512              /* LOGOX.DAT hold 512 words       */
#define IMHEIGHT  44           /* height of output image */
#define BMP_LEN  (int)400              /* LENGTH OF BMP BUFFER           */

#define START    140

#define NO_THRESHOLD 1


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


/* Protos */
void fileclose(HANDLES);
void close_input_file(int);
void fileopen(HANDLES *);
void loadlogo(UL, int, char*, UL, UL, int);
void rgb_yuv(CHANBUFFERS*);
void alloc_all(CHANBUFFERS *);
void free_all(CHANBUFFERS *);
void give_info(void);

void main(void)
  {
  CHANBUFFERS chan_buffers;
  HANDLES     chan_handles;
  int         chan_line_len;

  int              i;
  int              j;
  UI               val;
 
  _fmode = O_BINARY; 

  fileopen(&chan_handles);    /* open CHANNEL0.DAT, LOGOY.DAT, LOGOU.DAT, LOGOV.DAT */  

  chan_line_len  = 256;


  alloc_all(&chan_buffers);

  val = 1024;
  for (i = 0; i < 44; i++)
    {
    for (j = 0; j < 256; j++)
      {
      chan_buffers.chan1_buff[j] = val;
      chan_buffers.chan2_buff[j] = val;
      chan_buffers.chan3_buff[j] = val;
      }
    write(chan_handles.chan1, chan_buffers.chan1_buff, chan_line_len * 2);
    write(chan_handles.chan2, chan_buffers.chan2_buff, chan_line_len * 2);
    write(chan_handles.chan3, chan_buffers.chan3_buff, chan_line_len * 2);
    }
  fileclose(chan_handles);

  free_all(&chan_buffers); 

  exit(0);
  }

/*********************************************************************/
/*                        Processing Functions                       */
/*********************************************************************/

void free_all(CHANBUFFERS * yuv)
  {
#if 1
  free((UI*)yuv->chan1_buff);
  free((UI*)yuv->chan2_buff);
  free((UI*)yuv->chan3_buff);
#endif
  }


void alloc_all(CHANBUFFERS * yuv)
  {

  if ((yuv->chan1_buff = (UI*)malloc(256 * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  if ((yuv->chan2_buff = (UI*)malloc(256 * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  if ((yuv->chan3_buff = (UI*)malloc(256 * sizeof(UI))) == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(1);
    }
  }



/* This function opens all channel files used for reading and writing */
void fileopen(HANDLES * file_handles)
  {
  if ((file_handles->chan1 = open("channel1.DAT", O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE)) == -1)
    {
    puts("Cant open output file.\n");
    exit(1);
    }
  if ((file_handles->chan2 = open("channel2.DAT", O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE)) == -1)
    {
    puts("Cant open output file.\n");
    exit(1);
    }
  if ((file_handles->chan3 = open("channel3.DAT", O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE)) == -1)
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

