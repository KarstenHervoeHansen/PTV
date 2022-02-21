/****************************************************************************/
/* File:     FPGAPC.C                                                       */
/* Purpose:  To catch data from FPGA and dump to file                       */
/*                                                                          */
/* Author:   JK, 16.08.2006                                                 */
/* Revised:                                                                 */
/*                                                                          */
/* Invocation: FPGAPC numwords filename                                     */
/*                                                                          */
/* Remarks:  This program is compiled in DOS and will not run under         */
/*           WindowsXP, 2000, etc. as it uses direct access to parallel     */
/*           port. The parallel port must be set to ECP.                    */
/*           The program uses hardware hanshaking between PC and FPGA.      */
/*           Data is 10 bits long and is read from parallel port base       */
/*           register (8 bits) and from 2 bits in status register           */
/*           (bits S3 and S4).  The handshake signals are following:        */
/*           /STB: strobe (/data ready) - input from FPGA, when LO the FPGA */
/*                 has placed valid data on data lines.                     */
/*            IBF: input buffer full (/acknowledge) - output from PC to     */
/*                 FPGA, when HI the PC has read data and is processing it, */
/*                 when LO, the PC has finished processing that data.       */
/*           STB is bit 5 of starus register.                               */
/*           IBF is bit 0 of control register.                              */
/*                                                                          */
/* Test:     The program was tested by having FPGA send data being output   */
/*           of incrementing counter.  Each data word received by PC must   */
/*           be greater by one compared to word received before.            */
/*           When compiled with WITH_CHECK defined, the program will check  */
/*           the above requirement.                                         */
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include <math.h>
#include "xms.h"

#define  VERSION (float) 1.0

/* We use Parallel Port in ECP mode */
#define PORT      (int) 0x378
#define STAT      (int) 0x379
#define CTRL      (int) 0x37A
#define ECR      (int) (PORT + 0x402)    /* Extended Control Register */
#define CRA      (int) (PORT + 0x400)    /* Configuration Register A  */

/* IBF (input buffer full) is an output signal, which when HI, */
/* means that the PC has read and processed data               */
/* It goes to Printer port pin 1, which is bit 0 of CTRL reg.  */
/* bit C0 is inverted.                                         */
#define IBF_LO    ibf |= 0x01;  outportb(CTRL, ibf) 
#define IBF_HI    ibf &= 0xFE;  outportb(CTRL, ibf) 


/* /STB is an input signal, which when taken LO by FPGA, means */
/* that valid 10 bit data is available on data port (8 bits)   */
/* on status port bits S3 (D8) and S4 (D9)                     */

typedef union _ui
  {
   unsigned char chars [2];
   unsigned int  word;
   } ui;

#define MEMDATA ((unsigned long) 0x5AA320)

#define MOVING_PIN_SPEED 30000

/* rotating pin: one step per MOVING_PIN_SPEED words sent */
char *rotating_pin[] =
  {
  "|",
  "/",
  "-",
  "\\",
  NULL
  };

void give_info(void);

int main(int argc, char **argv)
  {
  FILE * hand;
  ui  idata;
  ui  previous_data;
  unsigned long cnt;
  unsigned long numbytes;
  unsigned long i;
  unsigned int  tmp;
  int res;
  unsigned char ibf;
  unsigned char stb;
  clock_t  startClock;
  clock_t  endClock;
  double   seconds;
  int      pin;         /* used to control rotating pin */
  int      line_cnt;    /* used to control rotating pin */


  if (argc != 3)
    {
    give_info();
    return (1);
    }

  numbytes = atol(argv[1]);  /* first command line arg is numbytes */

  printf("FPGAPC for DOS Version %.2f.\n", VERSION);
  printf("Loads %ld of data from FPGA to file %s\n", numbytes, argv[2]);

  cnt            = 0;
  previous_data.word     = 0;
  stb            = 1;
  ibf            = 0x20;
  pin            = 0;
  line_cnt       = 0;

  outportb(ECR, 0x20);     /* set Extended Control Reg: Set Byte Mode */

  hand = fopen(argv[2], "wb");
  if (hand == NULL)
    {
    printf("Cannot open file %s\n", argv[2]);
    return (1);
    }

#if 0
  /* Set unbuffered File i/o */
  if (setvbuf(hand, NULL, _IONBF, 512) != 0)
    {
    printf("Failed to set buffer to NULL.\n");
    goto gracefull_exit;
    }
#endif

  IBF_LO;                   /* IBF LO IBF_LO */

  startClock = clock();

#ifdef WITH_CHECK
  /* make one run to get firs data word */
  /* wait for strobe to go low */
  do
    {
    if (kbhit())
      {
      getch();
      goto gracefull_exit;
      }
    stb  = inportb(STAT);
    stb &= 0x20;
    } while (stb);

  idata.word = inport(PORT);
  IBF_HI;
  idata.chars[1]  = idata.chars[1] >> 3;
  idata.chars[1] &= 0x03;               /* clear 6 MSB bits */

  previous_data.word = idata.word + 1;    /* increment data and put into previous_data */
  if (previous_data.word == 0x400)        /* max data is 0x3FF                         */
    previous_data.word = 0;
  IBF_LO;                         /* next data must be equal to previous_data  */
  /* first word of data acquired... */
#endif

  while(cnt < numbytes)
    {
    /* wait for strobe to go low */
    do
      {
      stb  = inportb(STAT);
      stb &= 0x20;
      } while (stb);

    idata.word = inport(PORT);            /* read data        */
    IBF_HI;
    idata.chars[1]  = idata.chars[1] >> 3;
    idata.chars[1] &= 0x03;               /* clear 6 MSB bits */

    fwrite(&idata.word, sizeof(int), 1, hand);

#ifdef WITH_CHECK
    /* check that data read is previous data plus one         */
    if (idata.word != previous_data.word)
      printf("Difference at %ld, is 0x%x should be 0x%x\n", cnt, idata.word, previous_data.word);
    previous_data.word = idata.word + 1;
    if (previous_data.word == 0x400)
      previous_data.word = 0;
#endif
    cnt++;
    /* animate rotating pin */
    line_cnt++;
    if (line_cnt == MOVING_PIN_SPEED)
      {
      printf("%s\r", rotating_pin[pin++]);
      if (rotating_pin[pin] == NULL)
        pin = 0;
      line_cnt = 0;
      }

    IBF_LO;
    }
gracefull_exit:

  fclose(hand);
  printf("Acquired %ld words\n", cnt); 
  endClock = clock();
  seconds = (double)((endClock - startClock) / CLOCKS_PER_SEC);
  printf("Execution Time = %.1f seconds\n", seconds);
  return (0);
  }


void give_info(void)
  {
  printf("FPGRAPC: Loads data from FPGA to a file via parallel port\n");
  printf("in ECP mode.\n");
  printf("Note: the program will 'hang' when run with no connection to FPGA.\n");
  printf("Invokation: FPGAPC numwords file, where:\n");
  printf("numwords is the number of data words to acquire,\n");
  printf("file     is the data file name.\n");
  return;
  }
