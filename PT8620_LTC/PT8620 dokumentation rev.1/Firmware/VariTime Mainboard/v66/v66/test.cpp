/* test.c - program to test addition of genlock samples to
            tlg samples, i.e calc_tlg_delay() function      */

#include <stdio.h>
#include <conio.h>
#include <math.h>

#define UC unsigned char





typedef struct uu
  {
  long   samples_per_line;
  long   lines_per_frame;
  UC   sys_clk;
  UC   scan;
  UC   sync;
  long def_del;
  long fra_del;
  UC   format_id;
  long nadjust;
  }  HDPARAMS;


/*    spl    lpf    clk  sca   syn   def_del   fra_del  format_id */                               
HDPARAMS tlg_fmt_table[25] =                                                     
  {                                                                                                
    {   0L, 1125L,   0,   0,    0,     2474980L,        4949980L,  0, -20L},  /* "OFF"              0 */      
    {2200L, 1125L,   0,   0,    0,  2200L*1125L,  2L*2200L*1125L,  1, -20L},  /* "HD 1080P/60"      1 */  
    {2200L, 1125L,   1,   0,    3,  2200L*1125L,  2L*2200L*1125L,  2, -20L},  /* "HD 1080P/59.94"   2 */  
    {2640L, 1125L,   0,   0,    2,  2640L*1125L,  2L*2640L*1125L,  3, -20L},  /* "HD 1080P/50"      3 */  
    {4400L, 1125L,   0,   1,    0,  4400L*1125L,  2L*4400L*1125L,  4, -20L},  /* "HD 1080I/30"      4 */  
    {4400L, 1125L,   1,   1,    3,  4400L*1125L,  2L*4400L*1125L,  5, -20L},  /* "HD 1080I/29.97"   5 */  
    {5280L, 1125L,   0,   1,    2,  5280L*1125L,  2L*5280L*1125L,  6, -20L},  /* "HD 1080I/25"      6 */  
    {4400L, 1125L,   0,   0,    0,  4400L*1125L,  2L*4400L*1125L,  7, -20L},  /* "HD 1080P/30"      7 */  
    {4400L, 1125L,   1,   0,    3,  4400L*1125L,  2L*4400L*1125L,  8, -20L},  /* "HD 1080P/29.97"   8 */  
    {5280L, 1125L,   0,   0,    2,  5280L*1125L,  2L*5280L*1125L,  9, -20L},  /* "HD 1080P/25"      9 */  
    {5500L, 1125L,   0,   0,    0,  5500L*1125L,  2L*5500L*1125L, 10, -20L},  /* "HD 1080P/24"     10 */  
    {5500L, 1125L,   1,   0,    0,  5500L*1125L,  2L*5500L*1125L, 11, -20L},  /* "HD 1080P/23.98"  11 */  

    {4400L, 1125L,   0,   1,    0,  4400L*1125L,  2L*4400L*1125L, 22, -20L},  /*  1080sF/30        12 */  
    {4400L, 1125L,   1,   1,    3,  4400L*1125L,  2L*4400L*1125L, 23, -20L},  /*  1080sF/29.97     13 */  
    {5280L, 1125L,   0,   1,    2,  5280L*1125L,  2L*5280L*1125L, 24, -20L},  /*  1080sF/25        14 */  
    {5500L, 1125L,   0,   1,    0,  5500L*1125L,  2L*5500L*1125L, 25, -20L},  /*  1080sF/24        15 */  
    {5500L, 1125L,   1,   1,    0,  5500L*1125L,  2L*5500L*1125L, 26, -20L},  /*  1080sF/23.98     16 */  
                                                                                                   
    {3300L,  750L,   0,   0,    0,   3300L*750L,   2L*3300L*750L, 14, -20L},  /* "HD 720P/60"      17 */  
    {3300L,  750L,   1,   0,    3,   3300L*750L,   2L*3300L*750L, 15, -20L},  /* "HD 720P/59.94"   18 */  
    {3960L,  750L,   0,   0,    2,   3960L*750L,   2L*3960L*750L, 16, -20L},  /* "HD 720P/50"      19 */  
    {6600L,  750L,   0,   0,    0,   6600L*750L,   2L*6600L*750L, 17, -20L},  /* "HD 720P/30"      20 */  
    {6600L,  750L,   1,   0,    3,   6600L*750L,   2L*6600L*750L, 18, -20L},  /* "HD 720P/29.97"   21 */  
    {7920L,  750L,   0,   0,    2,   7920L*750L,   2L*7920L*750L, 19, -20L},  /* "HD 720P/25"      22 */  
    {8250L,  750L,   0,   0,    0,   8250L*750L,   2L*8250L*750L, 20, -20L},  /* "HD 720P/24"      23 */  
    {8250L,  750L,   1,   0,    0,   8250L*750L,   2L*8250L*750L, 21, -20L}   /* "HD 720P/23.98"   24 */  
  };                                                                                               
                /* 20 elements MAX_TLG_SYSTEMS  25 in mas.h  */                                    

long calc_tlg_delay(UC, UC, long, long);


int main(void)
  {
  long tmp_samps;
  long tlg_samps;
  long gen_samps;
  long i;
  UC   gener;
  UC   format;



  gener     = 0;
  format    = 14;
  gen_samps = 0L;
  tlg_samps = 0L;

  /* having given tlg delay, check if function calc_tlg_delay()
     returns something bigger than tlg_fmt_table[format].fra_del
  */
#if 1
  tlg_samps  = 2000000L;
#else
  tlg_samps  = 0L;
#endif

  tlg_samps += tlg_fmt_table[format].def_del;

  if (tlg_samps > tlg_fmt_table[format].fra_del)
    {
    printf("Invalid input tlg_samples.\n");
    exit(1);
    }
#if 0
  printf("tlg_samps = %lu, fra_del = %lu\n", tlg_samps, tlg_fmt_table[format].fra_del);
  printf("Press any key...\n");
  while (!kbhit())
    ;
#endif

  for (gen_samps = 0L; gen_samps < (276480000L); gen_samps += 46L) 
/*  for (gen_samps = 183389125L; gen_samps < (183389200L); gen_samps += 5) */
    {                              

    tmp_samps = calc_tlg_delay(gener, format, tlg_samps, gen_samps);

    if ((gen_samps % 100000L) == 0L)
      printf("result: %lu  genl_samps = %lu\n", tmp_samps, gen_samps);
/*    printf("result: %lu  genl_samps = %lu\n", tmp_samps, gen_samps); */

    if (tmp_samps > tlg_fmt_table[format].fra_del)
      {
      printf("Overflow: result = %lu, genl_samps = %lu\n", tmp_samps, gen_samps);
      break;
      }
    }
  printf("Done.\n");
  return 1;
  }
/******************************************************************
 * NAME       long calc_tlg_delay(UC gener, UC form, long tlg_samples,
                                  long genlock_samples)
 * PURPOSE    To recalculate TLG delay for current genlock delay/
 * 
 * ARGUMENTS  UC gener                - TLG generator nr
 *            UC format               - TLG generator format
 *            long tlg_samples        - TLG generator delay
 *            long genlock_samples    - genlock delay
 *
 * RETURNS    new TLG samples
 *
 * EXTERNS    None
 *
 * NOTES:     This function returns tlg samples calculated for given
 *            genlock samples.  Genlock samples are converted to 
 *            tlg samples and added to current tlg samples.
 *            This function is called each time TrxTLGDelay()is 
 *            invoked, which happens when:
 * 
 *            1 - new genlock delay is broadcast
 *            2 - new TLG delay is transmitted to PT8640.
 ******************************************************************
 * NOTE:      When this function is entered, tlg_delay already    *
 *            is increased by def_dely.                           *
 ******************************************************************
 *    
 *    276480000L  - one frame in genlock samples.
 *    276,480,000 = 625*1728*256
 * This in genlock samples is 5,934,080
 *
 *  4,294,967,295 = MAXLONG

To test behaviour of this function a test program (test.c) was 
written, which scans genlock_samples parameter and checks that
this function does not return more than 
tlg_fmt_table[format].fra_del.

Below is printout from this test program, showing returned values
when combined tlg_samples approach fra_del. As can be seen,
(this was done with the line:
  for (gen_samps = 183389125L; gen_samps < (183389200L); gen_samps += 5)
)
when result is equal to fra_del, increasing genlock_samples by
equivalent one tlg sample, causes output to roll over to
def_del + 1.

      C:\cxa\work\HDTV_board\mas_full>test
      result: 11879980  genl_samps = 183389125
      result: 11879980  genl_samps = 183389130
      result: 5939981   genl_samps = 183389135
      result: 5939981   genl_samps = 183389140
      result: 5939981   genl_samps = 183389145
      result: 5939981   genl_samps = 183389150
      result: 5939981   genl_samps = 183389155
      result: 5939981   genl_samps = 183389160
      result: 5939981   genl_samps = 183389165
      result: 5939981   genl_samps = 183389170
      result: 5939981   genl_samps = 183389175
      result: 5939982   genl_samps = 183389180
      result: 5939982   genl_samps = 183389185

 *
 ******************************************************************/
long calc_tlg_delay(UC gener, UC form, long tlg_delay, long genlock_delay)
  {
  float tmps;
  long  tmp_samps;
  long  spf;

  spf = (long)(tlg_fmt_table[form].samples_per_line *
         tlg_fmt_table[form].lines_per_frame);

  /* if genlock delay is bigger than 1 genlock frame,       */
  /* we make it equal to nr of genlock samples over 1 frame */
   
  if (genlock_delay > (276480000L))
    genlock_delay = genlock_delay % (276480000L);

  /* Convert  genlock samples to tlg samples */

  if (tlg_fmt_table[form].sys_clk)
    tmps = (148352.0 * (float) genlock_delay) / 6912000.0;
  else
    tmps = (148500.0 * (float) genlock_delay) / 6912000.0;

  genlock_delay = (long)tmps;

  /* if converted genlock delay is bigger that fra_del,
     we make it equal to converted genlock delay over
     one TLG frame
  */

  genlock_delay = genlock_delay % spf;

  /* add equvalent genlock samples                 */
  tmp_samps = (long) genlock_delay + tlg_delay;


  /* if more than fra_del, substract spf    */

  if (tmp_samps > (tlg_fmt_table[form].fra_del))
    {
    tmp_samps = tmp_samps - spf; /* 'minus'.... is correct, not '%" */
    }

  return (tmp_samps);
  }






