/* test.c - program to test TLGSamplesToFLT() function */

#include <stdio.h>

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long

typedef struct aa
  {
  long samples_per_line;
  long lines_per_frame;
  UC sys_clk;
  long fra_del;
  long def_del;
  }  INDIVID;

typedef struct bb
  {
  INDIVID members[21];
  } TLG_FORMATS;


TLG_FORMATS tlg_fmt_table =
  {
  2200, 1125, 0,  4949980,  2474980,
  2200, 1125, 1,  4949980,  2474980,
  2640, 1125, 0,  5939980,  2969980,
  4400, 1125, 0,  9899980,  4949980,
  4400, 1125, 1,  9899980,  4949980,
  5280, 1125, 0, 11879980,  5939980,
  4400, 1125, 0,  9899980,  4949980,
  4400, 1125, 1,  9899980,  4949980,
  5280, 1125, 0, 11879980,  5939980,
  5500, 1125, 0, 12374980,  6187480,
  5500, 1125, 1, 12374980,  6187480,
  4400, 1125, 0,  9899980,  4949980,
  4400, 1125, 1,  9899980,  4949980,
  3300, 750,  0,  4949980,  2474980,
  3300, 750,  1,  4949980,  2474980,
  3960, 750,  0,  5939980,  2969980,
  6600, 750,  0,  9899980,  4949980,
  6600, 750,  1,  9899980,  4949980,
  7920, 750,  0, 11879980,  5939980,
  8250, 750,  0, 12374980,  6187480,
  8250, 750,  1, 12374980,  6187480
  };

void TLGSamplesToFLT(void);


int main(void)
  {
  long  samples;
  int   field;
  int   line;
  float timing;
  char  sign;

  samples = 2159L;

  TLGSamplesToFLT(0, samples, &sign, &field, &line, &timing);

  printf("%c  %d   %d   %f\n", sign, field, line, timing);
  
  return 1;
  }



/***************************************************************************/
/* void TLGSamplesToFLT(void)                                      MENU.C  */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 14.10.2003                                                     */
/*                                                                         */
/* Function: To convert tlg_samples to 3 components:                       */
/*           V - number of frames                                          */
/*           H - number of lines                                           */
/*           T - number of nanoseconds                                     */
/* Remarks: When calculating timing in nanoseconds, sysclk is taken into   */
/*          consideration, as it is different for different formats        */
/*          Variable clock is used to determine scaling factor for         */
/*          nanoseconds.                                                   */
/*                                                                         */
/*                                                                         */
/* Returns:  Nothing                                                       */
/* Updates:     TBD                                                        */
/***************************************************************************/
void TLGSamplesToFLT(UC frmt, long samps, char* sign, char* F, int* L, float* T)
  {
  long clock;
  char frame;
  int  line;
  long spl; 
  long lpf;
  
  long samples_2_send;

  spl = tlg_fmt_table.members[frmt].samples_per_line;
  lpf = tlg_fmt_table.members[frmt].lines_per_frame;


  *sign = '+';

  if (samps < 0)
    *sign = '-';

  if (samps <= 1*(lpf*spl))
    {
    frame =  samps / (lpf*spl);
    line  = (samps % (lpf*spl)) / (spl);
    samps = (samps % (lpf*spl)) % (spl);
    }
  else
    {
    frame =  samps / (lpf*spl);
    line  = (samps % (lpf*spl)) / (spl);
    samps = (samps % (lpf*spl)) % (spl);
    }

  *F = abs(frame);
  *L = abs(line);

  /* Timing resolution depends on sysclk, which is dependent on format */
  /* If sysclk is 0, then sysclk is 148.500                            */
  /* If sysclk is 1, then sysclk is 148.352                            */

  if (tlg_fmt_table.members[frmt].sys_clk)
    clock = 148352;
  else
    clock = 148352;   /* clock = 148500; */
  *T = fabs((float) (samps * 1000000L) / clock);
  }

