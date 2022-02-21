/* table.c : profram to calculate def_delay for HDTV project */

#include <stdio.h>

#define UL unsigned long
#define UC unsigned char

#define MAX_HD_SYSTEMS  19
#define MAX_TLG_SYSTEMS 24

#define DEF_DELAY(x) (long)(2 * (tab.members[x].samples_per_line * tab.members[x].lines_per_frame) - 20)
#define FRA_DELAY(x) (long)((tab.members[x].samples_per_line * tab.members[x].lines_per_frame) - 20)

typedef struct aa
  {
  long   samples_per_line;
  long   lines_per_frame;
  UC   sys_clk;
  UC   scan;
  UC   sync;
  long def_del;
  long fra_del;
  }  INDIVID;

typedef struct cc
  {
  INDIVID elems[MAX_HD_SYSTEMS];
  } HD_FORMATS;

typedef struct bb
  {
  INDIVID members[MAX_TLG_SYSTEMS];
  } TLG_FORMATS;

/*spl    lpf   clk  sca   syn  def_del   fra_del*/
HD_FORMATS hdtpg_fmt_table =
  {
     0,    1,   0,   0,    0,  2474980,  4949980, /*  0 */  /* "OFF"; */
  4400, 1125,   0,   1,    0,  4949980,  9899980, /*  1 */
  4400, 1125,   1,   1,    3,  4949980,  9899980, /*  2 */
  5280, 1125,   0,   1,    2,  5939980, 11879980, /*  3 */
  4400, 1125,   0,   0,    0,  4949980,  9899980, /*  4 */
  4400, 1125,   1,   0,    3,  4949980,  9899980, /*  5 */
  5280, 1125,   0,   0,    2,  5939980, 11879980, /*  6 */
  5500, 1125,   0,   0,    0,  6187480, 12374980, /*  7 */
  5500, 1125,   1,   0,    0,  6187480, 12374980, /*  8 */
  /* Two below removed on 5.04.2006                       */
  /*4400, 1125,   0,   1,    0,  4949980,  9899980, /*  9 */
  /*4400, 1125,   1,   1,    3,  4949980,  9899980, /* 10 */
  3300,  750,   0,   0,    0,  2474980,  4949980, /*  9 */
  3300,  750,   1,   0,    3,  2474980,  4949980, /* 10 */
  3960,  750,   0,   0,    2,  2969980,  5939980, /* 11 */
  6600,  750,   0,   0,    0,  4949980,  9899980, /* 12 */
  6600,  750,   1,   0,    3,  4949980,  9899980, /* 13 */
  7920,  750,   0,   0,    2,  5939980, 11879980, /* 14 */
  8250,  750,   0,   0,    0,  6187480, 12374980, /* 15 */
  8250,  750,   1,   0,    0,  6187480, 12374980, /* 16 */
  /* last two are for HDTPG only, changed def_del, fra_del     */
  /* lpf and spl: 27.10.2005                                   */
  9438,  525,   0,   1,    0,  4949980,  9899980, /* 17 SDI525 */
  9504,  625,   0,   1,    0,  5939980, 11879980  /* 18 SDI625 */
  };




TLG_FORMATS tab = 
  {
  /* first 4 are only for TLG */
     0, 1125,   0,   0,    0,  2474980,  4949980, /* -1       0 */  /* "OFF"; */
  2200, 1125,   0,   0,    0,  2474980,  4949980, /*  0       1 */  
  2200, 1125,   1,   0,    3,  2474980,  4949980, /*  1 */
  2640, 1125,   0,   0,    2,  2969980,  5939980, /*  2 */
  /* 18 entries below are common for TLG and HDTPG               */
  4400, 1125,   0,   1,    0,  4949980,  9899980, /*  3 */
  4400, 1125,   1,   1,    3,  4949980,  9899980, /*  4 */
  5280, 1125,   0,   1,    2,  5939980, 11879980, /*  5 */
  4400, 1125,   0,   0,    0,  4949980,  9899980, /*  6 */
  4400, 1125,   1,   0,    3,  4949980,  9899980, /*  7 */
  5280, 1125,   0,   0,    2,  5939980, 11879980, /*  8 */
  5500, 1125,   0,   0,    0,  6187480, 12374980, /*  9 */
  5500, 1125,   1,   0,    0,  6187480, 12374980, /* 10 */
  4400, 1125,   0,   1,    0,  4949980,  9899980, /* 11 */
  4400, 1125,   1,   1,    3,  4949980,  9899980, /* 12 */
  3300,  750,   0,   0,    0,  2474980,  4949980, /* 13 */
  3300,  750,   1,   0,    3,  2474980,  4949980, /* 14 */
  3960,  750,   0,   0,    2,  2969980,  5939980, /* 15 */
  6600,  750,   0,   0,    0,  4949980,  9899980, /* 16 */
  6600,  750,   1,   0,    3,  4949980,  9899980, /* 17 */
  7920,  750,   0,   0,    2,  5939980, 11879980, /* 18 */
  8250,  750,   0,   0,    0,  6187480, 12374980, /* 19 */
  8250,  750,   1,   0,    0,  6187480, 12374980, /* 20  21 */
  };

char *names [MAX_TLG_SYSTEMS] =
  {
  "OFF",                                                
  "HD 1080P/60",                                        
  "HD 1080P/59.94",                                     
  "HD 1080P/50",                                        
                                                        
  "HD 1080I/30",   /* These are used in HDTPG  */       
  "HD 1080I/29.97",                                     
  "HD 1080I/25",                                        
  "HD 1080P/30",                                        
  "HD 1080P/29.97",                                     
  "HD 1080P/25",                                        
  "HD 1080P/24",                                        
  "HD 1080P/23.98",                                     
  "HD 1035I/30",    /********** REMOVE! *******/        
  "HD 1035I/29.97", /********** REMOVE! *******/        
                                                        
  "HD 720P/60",                                         
  "HD 720P/59.94",                                      
  "HD 720P/50",                                         
  "HD 720P/30",                                         
  "HD 720P/29.97",                                      
  "HD 720P/25",
  "HD 720P/24",                                         
  "HD 720P/23.98",                                      
                                                        
  "SD 487I/29.97 (525)",  /* These are used in HDTPG  */
  "SD 576I/25 (625)"  
  };


int main(void)
  {
  UL  new_table[22];
  UL  fra_table[22];
  UL  spl;
  int i;

  for (i = 0; i < 22; i++)
    {
    new_table[i]= DEF_DELAY(i);
    }
  for (i = 0; i < 22; i++)
    {
    fra_table[i]= FRA_DELAY(i);
    }


  printf("code TLG_FORMATS tab =\n");
  printf("  {\n");
  for (i = 0; i < 10; i++)
    {
    printf("%ld, %ld, %d, %ld, %ld\n",
           tab.members[i].samples_per_line,
           tab.members[i].lines_per_frame,
           tab.members[i].sys_clk,
           new_table[i],
           fra_table[i]);

    spl = hdtpg_fmt_table.elems[i].samples_per_line;

    printf("spl = %ld\n", spl);
    }
  }