/***************************************************************************/
/*  Copyright ProTeleVision Technologies A/S, BRONDBY 1998            */
/*  Project:    PT5230 Digital Video Generator                    */
/*  Module:    TIMING.C                                    */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  970211                                    */
/*  Rev. date:  980616, KEn, DEV                              */
/*  Status:    Version 1.0                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*    long AnlFLTToSamples(UC system, char F, int L, float T)          */
/*    void AnlSamplesToFLT(UC system, long samples,\                */
/*                         char* sign, char* F, int* L, float* T)*/
/*    long SDIFLTToSamples(UC system, char F, int L, float T)             */
/*    void SDISamplesToFLT(UC system, long samples,\                      */
/*                         char* sign, char* F, int* L, float* T)*/
/*    void WriteTimingFLT(char sign, char field, int line, float time)    */
/*    void PrintTimingFLT(char sign, char field, int line, float time)    */
/*                                                  */
/*  Changes:                                            */
/* 980616: Released as 4008 002 06812                          */
/***************************************************************************/

/***************************************************************************/
#include <xa.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mas.h"

#include "menutree.h"
#include "menu.h"
#include "menuhlp.h"
#include "xaexprt.h"
#include "xadrivr.h"
#include "tables.h"
#include "text.h"
#include "disp_drv.h"
#include "timing.h"
#include "serial1.h"

#define labs(c)   ((c) >= 0 ? (c) : (-c))

static char TextBuffer[10];
static char tbuff[80];     /* temporary buffer used for printing messages */

/***************************************************************************/
/* AnlFLTToSamples                                  TIMING.C */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970211                          */
/* Revised:    980215                                       */
/*                                                  */
/*  Function:  --                                        */
/*  Remarks:    --                                        */
/*  Returns:    FLT in samples, or -1 if illegal                    */
/*  Updates:    --                                        */
/***************************************************************************/
long AnlFLTToSamples(UC system, char F, int L, float T) {

  long samples;
  char minus;

  if ((F == 0) && (L == 0) && (T == 0))
    return(0);

  if ((F <= 0) && (L <= 0) && (T <= 0)) {
    minus = 1;
  }
  else {
    if ((F >= 0) && (L >= 0) && (T >= 0))
      minus = 0;
    else
      return(-1);
  }

                            // 1 sample = (1/27MHz)/256
  samples = (long)((T*6912)/1000 + 0.5);  

  if (system < NTSC) {            // If selected system is PAL..

    if (labs(samples) > 442367)      // Test for illegal time var
      return(-1);

    switch (F) {                // Test for illegal field & line
      case 0:
        if ((minus && (L < -311)) || (!minus && (L > 312)))
          return(-1);
        break;

      case -1:
      case -3:
      case 2:
        if (abs(L) > 312)
          return(-1);
        break;

      case -2:
      case 1:
      case 3:
        if (abs(L) > 311)
          return(-1);
        break;

      case 4:
        if ((L > 0) || (T > 0.0))
          return(-1);
        break;

      default:
        return(-1);
    }

    if (!minus) {                // if posistive delay..
      if (F % 2) {
        L += 313;
        F--;
      }
    }
    else {                    // ..else delay is negative
      F += 7;

      samples += 442367;

      if (F % 2) {
        L += 624;
        F--;
      }
      else
        L += 312;

      if (T == 0)
        samples++;
    }

    samples += F*138240000 + L*442368;
  }
  else {                      // ..else system is NTSC

    if (labs(samples) > 439295)      // Test for illegal time var
      return(-1);

    switch (F) {                // Test for illegal field & line
      case 0:
        if ((minus && (L < -261)) || (!minus && (L > 262)))
          return(-1);
        break;

      case -1:
        if (abs(L) > 262)
          return(-1);
        break;

      case 1:
        if (abs(L) > 261)
          return(-1);
        break;

      case 2:
        if ((L > 0) || (T > 0.0))
          return(-1);
        break;

      default:
        return(-1);
    }

    if (!minus) {                // if posistive delay..

      if (F % 2) {
        L += 263;
        F--;
      }
    }
    else {                    // ..else delay is negative
      F += 3;

      samples += 439295;

      if (F % 2) {
        L += 524;
        F--;
      }
      else
        L += 262;

      if (T == 0)
        samples++;
    }

    samples += F*115315200 + L*439296;
  }

  return(samples);
}

/***************************************************************************/
/* AnlSamplesToFLT                                  TIMING.C  */
/*                                                  */
/* Author:    Kim Engedahl, DEV, 970211                          */
/* Revised:    980215                                       */
/*                                                  */
/*  Function:  --                                        */
/*  Remarks:    --                                        */
/*  Returns:    --                                        */
/*  Updates:    --                                        */
/***************************************************************************/
void AnlSamplesToFLT(UC system, long samples, char* sign, char* F, int* L, float* T)
  {
  char frame, field;
  int line;

  *sign = '+';

  if (system < NTSC)
    {                // If system selected is PAL..
    if (samples >=  ((long) 2*625*1728*256))
      {
      samples--;
      frame   = samples  / ((long) 625*1728*256);
      line    = (samples % ((long) 625*1728*256)) / ((long) 1728*256);
      samples = (samples % ((long) 625*1728*256)) % ((long) 1728*256);
      field   = 2 * frame;
      if (line > 312)
        {
        field++;
        line = 624 - line;
        }
      else
        line  = 312 - line;
      samples = ((long) 1728 * 256) - 1 - samples;
      field   = 7 - field;
      if (field != 4)
        *sign = '-';
      }
    else
      {
      frame   = samples / ((long) 625*1728*256);
      line    = (samples % ((long) 625*1728*256)) / ((long) 1728*256);
      samples = (samples % ((long) 625*1728*256)) % ((long) 1728*256);
      field   = 2 * frame;
      if (line > 312)
        {
        field++;
        line -= 313;
        }
      }
    }
  else
    {                                // ..system selected is NTSC
    if (samples >=  ((long) 525*1716*256))
      {
      samples--;
      frame   = samples / ((long) 525*1716*256);
      line    = (samples % ((long) 525*1716*256)) / ((long) 1716*256);
      samples = (samples % ((long) 525*1716*256)) % ((long) 1716*256);
      field   = 2 * frame;
      if (line > 262)
        {
        field++;
        line = 524-line;
        }
      else
        line  = 262-line;
      samples = ((long) 1716*256) - 1 - samples;
      field   = 3 - field;
      if (field != 2)
        *sign = '-';
      }
    else
      {
      frame   = samples / ((long) 525*1716*256);
      line    = (samples % ((long) 525*1716*256)) / ((long) 1716*256);
      samples = (samples % ((long) 525*1716*256)) % ((long) 1716*256);
      field   = 2*frame;
      if (line > 262)
        {
        field++;
        line -= 263;
        }
      }
    }
  *L = abs(line);
  *F = abs(field);
  *T = fabs((float) (samples*1000)/6912);
  }

/***************************************************************************/
/* SDIFLTToSamples                                               TIMING.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:   980215                                                       */
/*                                                                         */
/*  Function: To convert timing in form of F, L and T into samples         */
/*  Args:     UC    system - TV system                                     */
/*            char  F      - field nr                                      */
/*            int   L      - line  nr                                      */
/*            float T      - offset in ns                                  */
/*  Returns:  -1 if conversion cannot be done                              */
/*            samples otherwise                                            */
/*  Notes:    --                                                           */
/***************************************************************************/
long SDIFLTToSamples(UC system, char F, int L, float T)
  {
  long samples;
  char minus;

  if ((F == 0) && (L == 0) && (T == 0))
    return(0);

  if ((F <= 0) && (L <= 0) && (T <= 0))
    minus = 1;
  else
    {
    if ((F >= 0) && (L >= 0) && (T >= 0))
      minus = 0;
    else
      return(-1);
    }

  samples = (long)((T*6912)/1000 + 0.5);
  
  if (system == SDI625)
    {                                // Check it time equivqlent to samples..
    if (labs(samples) > 442367)      // is bigger than 64 us...
      return(-1);                    // if samples bigger than samples per line...

    switch (F)
      {                              // Test for illegal field & line
      case 0:
        if ((minus && (L < -313)) || (!minus && (L > 312)))
          return(-1);
        break;
      case 1:
        if ((L > 0) || (T > 0.0))
          return(-1);
        break;
      default:
        return(-1);
      }
    if (!minus)
      {                      // if posistive delay..
      if (F % 2)
        {
        L += 312;
        F--;
        }
      }
    else
      {                      // ..else delay is negative
      F += 1;
      samples += 442367; // 442367 = 1728*256-1 = spl-1
      if (F % 2)
        {
        L += 624;
        F--;
        }
      else
        L += 313;
      if (T == 0)
        samples++;
      }
    samples += F*138240000 + L*442368; // 625*1728*256/2=138240000 == spf/2
    }
  else
    {                                // ..else system is NTSC
    if (labs(samples) > 439295)      // Test for illegal time var
      return(-1);
    switch (F)
      {                              // Test for illegal field & line
      case 0:
        if ((minus && (L < -263)) || (!minus && (L > 262)))
          return(-1);
        break;
      case 1:
        if ((L > 0) || (T > 0.0))
          return(-1);
        break;
      default:
        return(-1);
      }
    if (!minus)
      {                    // if positive delay..
      if (F % 2)
        {
        L += 262;
        F--;
        }
      }
    else
      {                    // ..else delay is negative
      F += 1;
      samples += 439295;
      if (F % 2)
        {
        L += 524;
        F--;
        }
      else
        L += 263;
      if (T == 0)
        samples++;
      }
    samples += F*115315200 + L*439296;
    }
  return(samples);
  }

/***************************************************************************/
/* SDISamplesToFLT                                               TIMING.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:    980215                                                      */
/*                                                                         */
/*  Function:  --                                                          */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void SDISamplesToFLT(UC system, long samples, char* sign, char* F, int* L, float* T)
  {
  char field;
  int  line;

  *sign = '+';

  if (system == SDI625)
    {            // If system selected is SDI625..
    if (samples >=  ((long) 312*1728*256)) // 276480000
      {
      samples--;
      line    = (samples % ((long) 625*1728*256)) / ((long) 1728*256);
      samples = (samples % ((long) 625*1728*256)) % ((long) 1728*256);
      field   = 0;
      if (line > 311)
        {
        field++;
        line = 624-line;
        }
      else
        line  = 311-line;
      samples = ((long) 1728*256) - 1 - samples;
      field   = 1 - field;
      if (field != 1)
        *sign = '-';
      }
    else
      {
      line    = (samples % ((long) 625*1728*256)) / ((long) 1728*256);
      samples = (samples % ((long) 625*1728*256)) % ((long) 1728*256);
      field   = 0;
      if (line > 311)
        {
        field++;
        line -= 312;
        }
      }
    }
  else
    {                                // ..system selected is SI525
    if (samples >=  ((long) 262*1716*256))
      {
      samples--;
      line    = (samples % ((long) 525*1716*256)) / ((long) 1716*256);
      samples = (samples % ((long) 525*1716*256)) % ((long) 1716*256);
      field = 0;
      if (line > 261)
        {
        field++;
        line = 524-line;
        }
      else
        line  = 261-line;
      samples = ((long) 1716*256) - 1 - samples;
      field   = 1 - field;
      if (field != 1)
        *sign = '-';
      }
    else
      {
      line    = (samples % ((long) 525*1716*256)) / ((long) 1716*256);
      samples = (samples % ((long) 525*1716*256)) % ((long) 1716*256);
      field = 0;
      if (line > 261)
        {
        field++;
        line -= 262;
        }
      }
    }
  *L = abs(line);
  *F = abs(field);
  *T = fabs((float) (samples*1000)/6912);
  }

/***************************************************************************/
/* WriteTimingFLT                                                TIMING.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:    980215                                                      */
/*                                                                         */
/*  Function:  --                                                          */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void WriteTimingFLT(char sign, char field, int line, float time)
  {
  WriteCodeLN2(0, TimeVHdrTxt);         // Write V: (ie. field) header
  sprintf(TextBuffer, "%2d", field);    // Copy field number to buffer
  TextBuffer[0] = sign;                 // Add sign to field number
  WriteTxt2(TextBuffer);                // Write field number

  WriteCode2(TimeHHdrTxt);              // Write H: (ie. line) header
  sprintf(TextBuffer, "%04d", line);    // Copy line number to buffer
  TextBuffer[0] = sign;                 // Add sign to line no
  WriteTxt2(TextBuffer);                // Write line number

  WriteCode2(TimeTHdrTxt);              // Write T: (ie. ns) header
                                        // Copy samples (in ns) to buffer
  sprintf(TextBuffer, "%08.1f", time);
  TextBuffer[0] = sign;                 // Add sign to time value
  WriteTxt2(TextBuffer);                // Write time value

  ClearRestLN2();
  }


/***************************************************************************/
/* WriteTimingFLTTLG                                             TIMING.C  */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:    980215                                                      */
/*                                                                         */
/*  Function: To write TLG timing on 2.nd line of LCD display              */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void WriteTimingFLTTLG(char sign, char field, int line, float time)
  {
#if 0
  WriteCodeLN2(0, Spaces6Txt);
  WriteCode2(TimeHHdrTxt);              // Write H: (ie. line) header
  sprintf(TextBuffer, "%05d", line);    // Copy line number to buffer
  TextBuffer[0] = sign;                 // Add sign to line no
  WriteTxt2(TextBuffer);                // Write line number

  WriteCode2(TimeTHdrTxt);              // Write T: (ie. ns) header
                                        // Copy samples (in ns) to buffer
  sprintf(TextBuffer, "%08.1f", time);
  TextBuffer[0] = sign;                 // Add sign to time value
  WriteTxt2(TextBuffer);                // Write time value

  ClearRestLN2();
#else
  if (line < 0)
    sign = '-';
  else
    sign = '+';

  WriteCodeLN2(0, Spaces6Txt);
  WriteCode2(TimeHHdrTxt);              // Write H: (ie. line) header
  sprintf(TextBuffer, "%05d", line);    // Copy line number to buffer
  //TextBuffer[0] = sign;                 // Add sign to line no
  WriteTxt2(TextBuffer);                // Write line number

  WriteCode2(TimeTHdrTxt);              // Write T: (ie. ns) header
                                        // Copy samples (in ns) to buffer

  if (time < 0.0)
    sign = '-';
  else
    sign = '+';

  sprintf(TextBuffer, "%08.1f", time);
  //TextBuffer[0] = sign;                 // Add sign to time value
  WriteTxt2(TextBuffer);                // Write time value
  ClearRestLN2();

#if 0
        sprintf(tbuff, "line: %d   T: %05.1f\n", line, time);
        Udr_PutNChar(tbuff);
#endif

#endif
  }


/***************************************************************************/
/* PrintTimingFLT                                                 TIMING.C */
/*                                                                         */
/* Author:    Kim Engedahl, DEV, 970211                                    */
/* Revised:    980215                                                      */
/*                                                                         */
/*  Function: To output Timing via SCPI.                                   */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void PrintTimingFLT(char sign, char field, int line, float time)
  {
  sprintf(TextBuffer, "%2d", field);
  TextBuffer[0] = sign;               // Add sign to field number
  Udr_PutNChar(TextBuffer);
  Udr_PutChar(',');

  sprintf(TextBuffer, "%04d", line);
  TextBuffer[0] = sign;               // Add sign to line no
  Udr_PutNChar(TextBuffer);
  Udr_PutChar(',');

  sprintf(TextBuffer, "%08.1f", time);
  TextBuffer[0] = sign;               // Add sign to time value
  Udr_PutNChar(TextBuffer);
  }

/***************************************************************************/
/* PrintTimingFLTTLG                                              TIMING.C */
/*                                                                         */
/* Author:    Janusz Kuzminski, DEV, 12.12.2006                            */
/* Revised:    980215                                                      */
/*                                                                         */
/*  Function: To output HD and TLG Timing via SCPI.                        */
/*  Remarks:    --                                                         */
/*  Returns:    --                                                         */
/*  Updates:    --                                                         */
/***************************************************************************/
void PrintTimingFLTTLG(char sign, char field, int line, float time)
  {
  sprintf(TextBuffer, "%2d", field);
  //TextBuffer[0] = sign;               // Add sign to field number
  Udr_PutNChar(TextBuffer);
  Udr_PutChar(',');

  sprintf(TextBuffer, "%04d", line);
  //TextBuffer[0] = sign;               // Add sign to line no
  Udr_PutNChar(TextBuffer);
  Udr_PutChar(',');

  sprintf(TextBuffer, "%08.1f", time);
  //TextBuffer[0] = sign;               // Add sign to time value
  Udr_PutNChar(TextBuffer);
  }

/********************************************************************************
 * NAME:      long add_genlock_delay(UC form, long tlg_delay,
 *                                   long genlock_delay, 
 *                                   code HDPARAMS *table)
 * 
 * PURPOSE    To recalculate TLG, HD or Dl generator delay for 
 *            current genlock delay.
 * 
 * ARGUMENTS  UC gener                - TLG generator nr, not used
 *            UC format               - TLG generator format
 *            long tlg_samples        - TLG generator delay
 *            long genlock_samples    - genlock delay
 *            HDPARAMS * table        - pointer to format table
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
 *            is increased by def_dely, because this function is  *
 *            only called from TrxTLGDelay()                      *
 ******************************************************************
 *    
 *    276480000L  - one frame in genlock samples.
 *    276,480,000 = 625*1728*256
 * This in genlock samples is 5,934,080
 *
 *  4,294,967,295 = ULONG_MAX
 *  2,147,483,647 =  LONG_MAX
 *    276,480,000
To test behaviour of this function a test program (test.c) was 
written, which scans genlock_samples parameter and checks that
this function does not return more than fra_del.

Below is printout from this test program, showing returned values
when combined tlg_samples approach fra_del. As can be seen,
(this was done with the line:
  for (gen_samps = 183389125L; gen_samps < (183389200L); gen_samps += 5)
 and for format 5
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
long add_genlock_delay(UC form, long tlg_delay, long genlock_delay, 
                       code HDPARAMS *table)
  {
  float tmps;
  long  tmp_samps;
  long  spf;

  spf = (long)(table[form].samples_per_line * table[form].lines_per_frame);

  /* if genlock delay is bigger than 1 genlock frame,           */
  /* we make it equal to nr of genlock samples over N * 1_frame */
  /* probably it is not necessary...                            */

  if (genlock_delay > (276480000L))
    genlock_delay = genlock_delay % (276480000L);

  /* Convert genlock samples to tlg samples ***************************************************/
  /* Timing resolution depends on sysclk, which is dependent on format */
  /* If sysclk is 0, then sysclk is 148.500                            */
  /* If sysclk is 1, then sysclk is 148.352                            */
  /* Convert genlock samples to tlg samples ***************************************************/

  if (table[form].sys_clk)
    tmps = (148352.0 * (float) genlock_delay) / 6912000.0; /* clk = 1, OK */
  else
    tmps = (148500.0 * (float) genlock_delay) / 6912000.0; /* clk = 0, OK */

  genlock_delay = (long)tmps;

  /* if converted genlock delay is bigger than fra_del,
     we make it equal to converted genlock delay over
     one TLG frame
  */

  genlock_delay = genlock_delay % spf;

  /* add equvalent genlock samples                 */
  tmp_samps = genlock_delay + tlg_delay;

  return (tmp_samps);
  }

/***************************************************************************/
/* TLGFLTToSamples                                               TIMING.C  */
/*                                                                         */
/* Author:    JK                                                           */
/* Revised:   15.11.2006                                                   */
/*                                                                         */
/*  Function: To convert timing in form of F, L and T into tlg samples     */
/*  Args:     UC    form   - TLG format                                    */
/*            char  F      - field nr                                      */
/*            int   L      - line  nr                                      */
/*            float T      - offset in ns                                  */
/*  Returns:  -1 if conversion cannot be done                              */
/*            samples otherwise                                            */
/*  Notes:    --                                                           */
/*                                                                         */
/*  This function is used only in connection with SCPI remote commands.    */
/*  This function converts F, L, T input by user into tlg_samples, which   */
/*  then are used to set the delay of a TLG generator commanded by user.   */
/*  Check is made as samples obtained this way are within the following    */
/*  range:                                                                 */
/*          Nd = -(Nframe / 2)....(Nframe / 2 - 1)                         */
/*                                                                         */
/*          See specification: "PT8611 Timing Behaviour"                   */
/*                             of 23.05.2007, filename unknown.            */
/***************************************************************************/
long TLGFLTToSamples(UC form, char F, int L, float T, code HDPARAMS* table)
  {
  long  samples;
  long  lpf;
  long  spl;
  long  Nframe;

  if ((L == 0) && (T == 0.0))
    return(0);

  spl    = table[form].samples_per_line;
  lpf    = table[form].lines_per_frame;
  Nframe = spl * lpf;

  samples = 0;
  
  if (table[form].sys_clk)   /* This one apparently works better */
    samples = (long)((T / 6.740));
  else
    samples = (long)((T / 6.734));
  /* check if samples are bigger than spl */

  if (labs(samples) > spl)   /* if is bigger than full line in samples... */
    return(-1);

  if (abs(L) > lpf / 2)
    return(-1);

  samples += (long)(L * spl);

  if (samples < (-(Nframe / 2)))
    return(-1);
  else
    if (samples > ((Nframe / 2) - 1))
      return(-1);

  return(samples);
  }


/********************************************************************************
 * NAME       TLGSamplesToFLT(UC frmt, long Nd, char* sign, char* F,
 *                            int* L, float* T, code HDPARAMS *table)
 * Author:    Janusz Kuzminski
 * Revised:   14.10.2003 
 *            18.12.2006
 *     
 * PURPOSE    To convert delay sent in second argument (Nd) to field,
 *            line and timing.  The timing is in nanoseconds.  The 
 *            conversion is needed only to display values on LCD.
 *            The WriteTimingFLTTLG() function is used to do that.
 *
 * ARGUMENTS  UC frmt     - system
 *            long Nd     - current delay in samples
 *            char  *sign - holder for sign
 *            char  *F    - holder for field
 *            int   *L    - holder for line
 *            float *T    - holder for timing.
 *            code HDPARAMS *table - system table to use.
 * RETURNS    Nothing
 *
 * EXTERNS    None.
 *
 * NOTES:     This function converts currenet delay in samples to FLT, which then
 *            will be displayed in second line of LCD display.  The 
 *            behaviour is identical for interlaced and non-interlaced
 *            systems.  The field is always zero.  Both positive and 
 *            negative delays are produced.
 *            This function is common for TLG, HD and DL generators.
 *            
 *             See specification: "PT8611 Timing Behaviour"
 *                             of 23.05.2007, filename unknown.
 ********************************************************************************/
void TLGSamplesToFLT(UC frmt, long Nd, char* sign, char* F, int* L, float* T,
                      code HDPARAMS *table)
  {
  float clock;
  int   Nl;     /* displacement in lines             */
  long  spl;    /* samples per line                  */
  long  Ns;     /* displacement on a line in samples */
  float x;

  spl   = table[frmt].samples_per_line;

#if 0
  x = (float)Nd / (float)spl;

  if (x >= 0.0)
    Nl = (int)(x + 0.5);
  else
    Nl = (int)(x - 0.5);   /* calculate lines using ROUND() */
#else
  Nl = (int)(Nd / spl);    /* calculate lines using TRUNCATE() */
#endif

  Ns = Nd - (Nl * spl);    /* displacement on line */


  /* Timing resolution depends on sysclk, which is dependent on format */
  /* If sysclk is 0, then sysclk is 148.500                            */
  /* If sysclk is 1, then sysclk is 148.352                            */
  if (table[frmt].sys_clk)
    {
    clock = 148500.0 / 1.001;   /* clock = 148352  if sysclk == 1 */
    }
  else
    {
    clock = 148500.0;           /* clock = 148500; */
    }
  *T = (((float)Ns * 1000000.0) / clock);

  *F = 0;
  *L = (Nl);
  }

/***************************************************************************/
/* long adjust_hd_samples(long samps, UC sys, code HDPARAMS *table)        */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 29.06.2007                                                     */
/*                                                                         */
/* Function: This function adjusts samps according to algor below.         */
/*                                                                         */
/* Aguments: long samps  - current samples value                           */
/*           UC   sys    - current system                                  */
/*           code HDPARAMS *table - parameter table to use                 */
/*                                                                         */
/* Returns: New value of samples.                                          */

/* Returns: New value of samples.                                          */
/* Notes:   This function takes care that returned samples are always in   */
/*          the following interval:                                        */
/*                                                                         */
/*          Nd = -(Nframe / 2)....(Nframe / 2 - 1)                         */
/*                                                                         */
/*          See specification: "PT8611 Timing Behaviour"                   */
/*                             of 23.05.2007, filename unknown.            */

/***************************************************************************/
long adjust_hd_samples(long samps, UC sys, code HDPARAMS *table)
  {
  long  Nframe;  /* frame length in samples */
  long  Nd;      /* frame displacement      */

  Nd = samps;
#if 0
        sprintf(tbuff, "Isamps: %ld\n", Nd);
        Udr_PutNChar(tbuff);
#endif

  Nframe = table[sys].samples_per_line * table[sys].lines_per_frame;

  if (samps < (-(Nframe / 2)))
    Nd = samps + Nframe;
  else
    if (samps > ((Nframe / 2) - 1))
      Nd = samps - Nframe;

#if 0
        sprintf(tbuff, "sampsO: %ld\n", Nd);
        Udr_PutNChar(tbuff);
#endif

  return (Nd);
  }

/***************************************************************************/
/* long prepare_samples4fpga(long samps, UC sys, code HDPARAMS *table)     */
/*                                                                         */
/* Author:  Janusz Kuzminski                                               */
/* Revised: 15.08.2007                                                     */
/*                                                                         */
/* Function: This function adjusts samps according to algor below, it is   */
/*           used just before sending samps to the FPGA                    */
/*                                                                         */
/* Aguments: long samps  - current samples value, including genlock samps. */
/*           UC   sys    - current system                                  */
/*           code HDPARAMS *table - parameter table to use                 */
/*                                                                         */
/* Notes:   This function implements algorithm for calculating samples     */
/*          sent to the FPGA, as:                                          */
/*                                                                         */
/*         Nt = ((Nd+Nframe/2)modNframe)+Nframe-(Nframe/2)+Nadjust         */
/*                                                                         */
/*          See specification: "PT8611 Timing Behaviour"                   */
/*                             of 23.05.2007, filename unknown.            */
/***************************************************************************/
long prepare_samples4fpga(long samps, UC sys, code HDPARAMS *table)
  {
  long  Nframe;  /* frame length in samples */
  long  Nt;      /* frame displacement      */


  Nframe = table[sys].samples_per_line * table[sys].lines_per_frame;

  Nt = ((samps + (Nframe / 2)) % Nframe) + Nframe - (Nframe / 2) + table[sys].nadjust;
         
  return (Nt);
  }