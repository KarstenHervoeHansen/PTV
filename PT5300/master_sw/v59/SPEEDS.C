#pragma code symbols debug objectextend
 /*
 ; OLSEN Engineering ApS
 ; Navervej 30
 ; DK-4000 Roskilde
 ; tel (+45) 46 75 72 27
 ;
 ; FILE NAME        speeds.c51
 ; PURPOSE          Module for determinig speed in PR94 radar.
 ;
 ; SPEC             Janusz Kuzminski
 ; DESIGN           Janusz Kuzminski
 ; CODING           Janusz Kuzminski
 ; CODE INSP.
 ; TEST
 ; LAST UPDT:       24.09.95: Serial and Timer0 interrupts disabled
 ;                            while counting Doppler.
 ;                  16.10.95: IDATA used for pool.
 ;                  20.11.97: MPH CTLOAD updated.
 ;                  28.11.97: Division in quadrature detector is removed from
 ;                            interrupt routine, because it took too long time
 ;                            (62 uS).  It is placed now in speed routines, so
 ;                            detector is updated while waiting for CT to time
 ;                            out.  ver2.1 uses MDU to perform division.
 ;                  04.04.98: Algorithm for is_speed_x changed (ver3.3)
 ; NOTES:
 ;
*/

#include <reg517.h>             /* define 80537 registers */
#include <math.h>
#include <intrins.h>
#include "defins.h"

#define MAX_SAMPLES    100
#define MAX_SPEED_DEV  3
#define MINIMUM_SPEED  19
#define MAXMISHAPS     6

#ifdef MPH
#define PULS_FOR_2M    303     /* this is not correct */
#else
#define PULS_FOR_2M    303
#define PULS_FOR_1M    151
#endif

   /* see pr9402us.do */

#ifdef MPH
#define CTLOAD    (uint)  0xa8c5
#else
#define CTLOAD    (uint)  0x73A0
#endif


uchar is_speed_t(void);
uchar is_speed_b(void);
uchar is_speed_a(void);
uchar is_speed_test(void);

uchar determine_speed_a(void);
uchar determine_speed_t(void);
uchar determine_speed_b(void);

/* Variables */

extern uchar snumber;
extern uchar difspeed;

extern uchar mishaps;
extern uchar stimeout;
extern uchar temspeed;
extern uchar xtemp;
extern uint  ssum;
extern ui    top;
extern ui    bottom;

/* input signals */

sbit detect     = P1^1;
sbit valid_a    = P1^3;
sbit valid_t    = P1^4;
sbit quality    = P1^0;
sbit deg90      = P4^5;

extern bit test;
extern bit _22ms;
extern bit can_calculate;
extern bit bot_completed;
extern bit not_valid;

#define MDU_USED
#include "qdet.h"


/* NAME         uchar is_speed_a(void)
 * PURPOSE      To count Doppler pulses during 24 mS
 * ARGUMENTS    None
 *
 * RETURNS      1 if Doppler was unstable during 24 mS
 *              1 if speed was over 255
 *              speed if Doppler was stable during 24 mS
 *
 * EXTERNS      None
 * NOTES        This routine waits always 24 mS.
 */

uchar is_speed_a(void)
  {
  if (1)
    {
    not_valid = 0;
    CTREL  = CTLOAD;
    CTCON &= 0xf7;  /* clear CTF flag */
    IEN2   = 8;     /* enable CT interrupt */
    TH2    = 0;
    TL2    = 0;     /* zero contents of Timer2 */
    T2CON  = 0x02;      /* counter function @P1.7, page 116 */
    while (!_22ms)
      {
      QDETECTOR
      if (valid_a)
        {
        not_valid = 1;
        continue;
        }
      }
    IEN2  = 0;      /* disable CT interrupt */
    _22ms  = 0;
    if (not_valid)
      return (1);
    if (TH2)
      return (1);
    return (TL2);
    }
  return (1);
  }

/* NAME         uchar is_speed_t(void)
 * PURPOSE      To count Doppler pulses during 24 mS
 * ARGUMENTS    None
 *
 * RETURNS      1 if Doppler was unstable during 24 mS
 *              1 if speed was over 255
 *              speed if Doppler was stable during 24 mS
 *
 * EXTERNS      None
 * NOTES        This routine waits always 24 mS.
 */

uchar is_speed_t(void)
  {
  if (1)
    {
    not_valid = 0;
    CTREL  = CTLOAD;
    CTCON &= 0xf7;  /* clear CTF flag */
    IEN2   = 8;     /* enable CT interrupt */
    TH2    = 0;
    TL2    = 0;     /* zero contents of Timer2 */
    T2CON  = 0x02;      /* counter function @P1.7, page 116 */
    while (!_22ms)
      {
      QDETECTOR
      if (valid_t)
        {
        not_valid = 1;
        continue;
        }
      }
    IEN2  = 0;      /* disable CT interrupt */
    _22ms  = 0;
    if (not_valid)
      return (1);
    if (TH2)
      return (1);
    return (TL2);
    }
  return (1);
  }

/* NAME         uchar is_speed_b(void)
 * PURPOSE      To count Doppler pulses during 24 mS
 * ARGUMENTS    None
 *
 * RETURNS      1 if Doppler was unstable during 24 mS
 *              1 if speed was over 255
 *              speed if Doppler was stable during 24 mS
 *
 * EXTERNS      None
 * NOTES        This routine waits always 24 mS.
 */

uchar is_speed_b(void)
  {
  if (1)
    {
    not_valid = 0;
    CTREL  = CTLOAD;
    CTCON &= 0xf7;  /* clear CTF flag */
    IEN2   = 8;     /* enable CT interrupt */
    TH2    = 0;
    TL2    = 0;     /* zero contents of Timer2 */
    T2CON  = 0x02;      /* counter function @P1.7, page 116 */
    while (!_22ms)
      {
      QDETECTOR
      if (!quality)
        {
        not_valid = 1;
        continue;
        }
      }
    IEN2  = 0;      /* disable CT interrupt */
    _22ms  = 0;
    if (not_valid)
      return (1);
    if (TH2)
      return (1);
    return (TL2);
    }
  return (1);
  }

/* NAME         uchar is_speed_test(void)
 * PURPOSE      To count pulses from test generator during 24 mS
 * ARGUMENTS    None
 *
 * RETURNS      0 if Doppler was unstable during 24 mS
 *              0 if detect was low
 *              0 if quality was low (timeout occured)
 *              speed if Doppler was stable during 24 mS
 *
 * EXTERNS      None
 * NOTES
 */

uchar is_speed_test(void)
  {

  stimeout = MAX_RTC_TICK;
  while (detect)
    {
    QDETECTORTEST
    CTREL  = CTLOAD;
    CTCON &= 0xf7;  /* clear CTF flag */
    IEN2   = 8;     /* enable CT interrupt */
    TH2    = 0;
    TL2    = 0;     /* zero contents of Timer2 */
    T2CON  = 0x02;      /* counter function @P1.7, page 116 */
    while (!_22ms)
      {
      QDETECTORTEST
      if (!quality)
        {
        IEN2 = 0;   /* disable CT interrupt */
        _22ms  = 0;
        if (stimeout)
          goto label1;
        else
          return 0;
        }
      }
    IEN2  = 0;      /* disable CT interrupt */
    _22ms  = 0;
    return TL2;

label1:
    continue;
    } /* while detect */
  return (0);
  }

/*******************************/


/* NAME         uchar determine_speed_t(void)
 * PURPOSE      To calculate mean value of speed measurements during
 *              active periode of detect.
 * ARGUMENTS    None
 *
 * RETURNS      0 if speed could not be measured
 *              1 if MAX_SAMPLES measurements were taken (detect constantly
 *              HI)
 *              2 if speed was determined but car did not drive 2 meters
 *              3 if discrepancy between speed samples was more than 2 kmh.
 * EXTERNS      uchar xtemp
 *              uiint ssum
 *              uchar snumber
 *              uchar difspeed
 * NOTES
 */


uchar determine_speed_t(void)
  {
  uchar difference;

  mishaps  = 0;
  ssum     = 0;
  snumber  = 0;
  difspeed = 0;

  stimeout = MAX_RTC_TICK;
  while(1)
    {
    QDETECTOR
    xtemp = is_speed_t();
    if (xtemp == 1)
      {
      mishaps++;
      if (mishaps == MAXMISHAPS)
        goto label2;
      else
        continue;
      }
    else
      {
      if (snumber)
        {
        difference = abs((int)xtemp - (int)temspeed);
        if (difference > difspeed)
          difspeed = difference;
        }
      snumber++;
      temspeed = xtemp;
      if (snumber == MAX_SAMPLES) /* 100 gives approx. 1 s in this */
        return 1;                 /* Too many samples */
      ssum += (uint)xtemp;
      mishaps = 0;
      }
    } /* while 1 */
label2:
  if (snumber == 0) /* no speed could be measured */
    return 0;

  if (ssum < PULS_FOR_2M)
    {
#if 0
    if (ssum < PULS_FOR_1M)
      return 0;
#endif
    return 2;       /* car did not drive 2 meters */
    }
  if (difspeed > MAX_SPEED_DEV)
    return 3;      /* Signal too big discrepancy by returning 3 */

  temspeed = ssum / snumber;
  return (temspeed);
  }

/* NAME         uchar determine_speed_a(void)
 * PURPOSE      To calculate mean value of speed measurements during
 *              active periode of detect.
 * ARGUMENTS    None
 *
 * RETURNS      0 if speed could not be measured
 *              1 if MAX_SAMPLES measurements were taken (detect constantly
 *              HI)
 *              2 if speed was determined but car did not drive 2 meters
 *              3 if discrepancy between speed samples was more than 2 kmh.
 * EXTERNS      uchar xtemp
 *              uiint ssum
 *              uchar snumber
 *              uchar difspeed
 * NOTES
 */


uchar determine_speed_a(void)
  {
  uchar difference;

  mishaps  = 0;
  ssum     = 0;
  snumber  = 0;
  difspeed = 0;

  stimeout = MAX_RTC_TICK;
  while(1)
    {
    QDETECTOR
    xtemp = is_speed_a();
    if (xtemp == 1)
      {
      mishaps++;
      if (mishaps == MAXMISHAPS)
        goto label2;
      else
        continue;
      }
    else
      {
      if (snumber)
        {
        difference = abs((int)xtemp - (int)temspeed);
        if (difference > difspeed)
          difspeed = difference;
        }
      snumber++;
      temspeed = xtemp;
      if (snumber == MAX_SAMPLES) /* 100 gives approx. 1 s in this */
        return 1;                 /* Too many samples */
      ssum += (uint)xtemp;
      mishaps = 0;
      }
    } /* while 1 */
label2:
  if (snumber == 0) /* no speed could be measured */
    return 0;

  if (ssum < PULS_FOR_2M)
    {
#if 0
    if (ssum < PULS_FOR_1M)
      return 0;
#endif
    return 2;       /* car did not drive 2 meters */
    }
  if (difspeed > MAX_SPEED_DEV)
    return 3;      /* Signal too big discrepancy by returning 3 */

  temspeed = ssum / snumber;
  return (temspeed);
  }

/* NAME         uchar determine_speed_b(void)
 * PURPOSE      To calculate mean value of speed measurements during
 *              active periode of detect.
 * ARGUMENTS    None
 *
 * RETURNS      0 if speed could not be measured
 *              1 if MAX_SAMPLES measurements were taken (detect constantly
 *              HI)
 *              2 if speed was determined but car did not drive 2 meters
 *              3 if discrepancy between speed samples was more than 2 kmh.
 * EXTERNS      uchar xtemp
 *              uiint ssum
 *              uchar snumber
 *              uchar difspeed
 * NOTES
 */

uchar determine_speed_b(void)
  {
  uchar difference;

  mishaps  = 0;
  ssum     = 0;
  snumber  = 0;
  difspeed = 0;

  stimeout = MAX_RTC_TICK;
  while(1)
    {
    QDETECTOR
    xtemp = is_speed_b();
    if (xtemp == 1)
      {
      mishaps++;
      if (mishaps == MAXMISHAPS)
        goto label2;
      else
        continue;
      }
    else
      {
      if (snumber)
        {
        difference = abs((int)xtemp - (int)temspeed);
        if (difference > difspeed)
          difspeed = difference;
        }
      snumber++;
      temspeed = xtemp;
      if (snumber == MAX_SAMPLES) /* 100 gives approx. 1 s in this */
        return 1;                 /* Too many samples */
      ssum += (uint)xtemp;
      mishaps = 0;
      }
    } /* while 1 */
label2:
  if (snumber == 0) /* no speed could be measured */
    return 0;

  if (ssum < PULS_FOR_2M)
    {
#if 0
    if (ssum < PULS_FOR_1M)
      return 0;
#endif
    return 2;       /* car did not drive 2 meters */
    }
  if (difspeed > MAX_SPEED_DEV)
    return 3;      /* Signal too big discrepancy by returning 3 */

  temspeed = ssum / snumber;
  return (temspeed);
  }

/* end of speeds.c51 */
