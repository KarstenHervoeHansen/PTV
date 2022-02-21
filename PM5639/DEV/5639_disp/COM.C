/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PM5639 Display Unit                            */
/*  Module:    COM.C                                      */
/*  Author:    Preben Christiansen, DEV                        */
/*  Org. date:  910923                                    */
/*  Rev. 
:  970822, KEn  DEV                              */
/*  Status:    Version 2.3                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*       void pause( unsigned char n);                        */
/*       void sendbyte( char val);                          */
/*       void sendi( char* s);                            */
/*       void send( char* s);                              */
/*       void ncat( char* s, unsigned int n);                  */
/*       void sendint( unsigned int n);                      */
/*         void sendname( char no);                          */
/*       bit readstr( void);                              */
/*       int stoi( char* s);                              */
/*       bit read_name( char no);                          */
/*       bit read_headid( void);                            */
/*       bit read_size( void);                            */
/*       bit read_fact( void);                            */
/*       bit read_k( void);                               */
/*       char read_id( void);                             */
/*       bit samestr( char* s, char* t, char n);                 */
/*       bit read_number( void);                           */
/*       bit store_phosphor( char no);                       */
/*       bit recall_phosphor( char no);                      */
/*       bit install_head( void);                           */
/*                                                  */
/* Changes:                                            */
/*  970822 Added recognition of sensors: 48-63. Released as version 2.3    */
/*  951207 Released as version 2.2                            */
/*  951126 Added new function: sendbyte( char val)                  */
/*  951109 Revised call of function initall()                      */
/* 951030 timecount variable replaced by GenericTimecount            */
/*  951027 Sending headers defined as code                        */
/* 920520 Now supports projector head                           */
/* 920520 For earlier versions of history see u'v'-version             */
/***************************************************************************/
#include "pra.h"

#include <reg51.h>          /* Define 8051 register  */
#include <stdio.h>          /* Define I/O functions  */
#include <stdlib.h>          /* Standard library  */
#include <absacc.h>          /* Absolute addresses  */
#include <math.h>            /* Math (abs,...)  */
#include <run.h>            /* Own definitions */
#include <res.h>            /* Own definitions */
#include <sub.h>            /* Own definitions */
#include <defs.h>            /* Own definitions */
#include <tables.h>          /* Own definitions */


code char* pageadr   = "MA52;";    /* Define constant sending headers */
code char* kmadr     = "MA103;";
code char* factadr   = "MA97;";
code char* idadr     = "MA58;";
code char* numberadr = "MA77;";
code char* kuadr     = "MA85;";
code char* headidadr = "MA62;";


char send_head[8] = "SM    ;";  /* Define sending headers */
char madr_head[8] = "MA    ;";

char* checkadr;

unsigned int readintval;

/***************************************************************************/
/*  pause                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Generate general purpose delay                    */
/*  Syntax:    void pause( unsigned char n);                      */
/*  Remarks:    Pause is 2msec * n                            */
/*          Routine should be carefully examinde after compilation to  */
/*          ensure that the loop is actually 2msec long            */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void pause( unsigned char n) {
  unsigned char i, j;

  for ( i=n; i!=0; i--)              /* NOTICE!!!!!!! */
    for ( j=250; j!=0; j--) {          /* MUST BE CAREFULLY EXAMINED */
      feed_dog = 1;                /* AFTER COMPILATION TO ENSURE */
      feed_dog = 0;                /* DELAY ACTUALLY IS 2MS */
    }
}

/***************************************************************************/
/*  sendbyte                                            */
/*                                                  */
/* Written by:  Kim Engedahl, DEV                               */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      951126                                    */
/*  Revised:    951126                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Send byte immediately to sensor, ie. do not wait for an     */
/*          empty bus.                                  */
/*  Syntax:    void sendbyte( char val);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void sendbyte( char val) {
  unsigned char j;

  for ( j=42; j!=0; j--) 
    ;                      /* wait 0.25 ms before next char*/
  sending = 1;

  busy = 1;                /* wait until transmitted */

  for ( j=42; j!=0; j--) 
    ;                    /* wait 0.25 ms before next char*/

  SBUF = val;
  while ( busy) 
    ;                    /* busy is cleared by interrupt */

  sending = 0;
  bad_meas = 1;              /* do not use answer as measurement */
}


/***************************************************************************/
/*  sendi                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Send string immediately to sensor, ie. do not wait for an   */
/*          empty bus.                                  */
/*  Syntax:    void sendi( char* s);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void sendi( char* s) {
  unsigned char c, j;

  for ( j=42; j!=0; j--) 
    ;                      /* wait 0.25 ms before next char*/
  sending = 1;

  while ( c = *s++) {             /* send 1 char */
    busy = 1;                /* wait until transmitted */

    for ( j=42; j!=0; j--) 
      ;                    /* wait 0.25 ms before next char*/

    SBUF = c;
    while ( busy) 
      ;                    /* busy is cleared by interrupt */
  }

  sending = 0;
  bad_meas = 1;                /* do not use answer as measurement */
}

/***************************************************************************/
/*  send                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951030                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Send string to sensor when the bus is empty.            */
/*  Syntax:    void send( char* s);                            */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void send( char* s) {            /* send, but wait for empty bus */

  GenericTimecount = 105;

  while ( GenericTimecount && !sendok) {
    feed_dog = 1;
    feed_dog = 0;
  }

  if ( !sendok) 
    errorno = 1;              /* if time-out then set error */

  sendi( s);                  /* send string */
}

/***************************************************************************/
/*  ncat                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Add a 4-digit decimal <n> to a string <s>              */
/*  Syntax:    void ncat( char* s, unsigned int n);                */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void ncat( char *s, unsigned int n) {
  unsigned char m, dig;

  dig = 0;
  m = n/1000;                  /* find thousands */
  s++; s++;
  *s = m + '0';
  if (m) {
    s++; 
    dig = 1; 
  }
  n -= 1000*m;
  m = n/100;                  /* find houndreds */
  *s = m + '0';
  if (m || dig) {
    s++; 
    dig = 1; 
  }
  n -= 100*m;
  m = n/10;                  /* find tens */
  *s = m + '0';
  if (m || dig) 
    s++;
  n -= 10*m;                  /* the rest is units */
  *s++ = n + '0';
  *s++ = ';';
  *s = '\0';
}

/***************************************************************************/
/*  sendint                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Send an integer to the sensor                      */
/*  Syntax:    void sendint( unsigned int n);                    */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void sendint( unsigned int n) {
  ncat( send_head, n >> 8);        /* prepare and send hi byte */
  sendi( send_head);
  pause( 40);                  /* wait 80 ms for head to store byte */

  ncat( send_head, n & 0x00ff);      /* prepare and send lo byte */
  sendi( send_head);
  pause( 40);                  /* wait 80 ms */
}

/***************************************************************************/
/*  sendname                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Send a phosphor name to the sensor                  */
/*  Syntax:    void sendname( char no);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
void sendname( char no) {
  char *s;
  unsigned char c;

        /* if empty name then name = space+null (for lock-key detection) */
        /* this will make this unnamed phosphor selectable */

  if ( !*phos[no]->name ) 
    strput( "\40\0", phos[no]->name);
  s = phos[no]->name;
  ncat( madr_head, ramsize +12 -(int)no * 20);        /* form the address */
  send( madr_head);                      /* send address to sensor */

  while ( c = *s++) {            /* prepare and send 1 char. at a time */
    ncat( send_head, (int)c);
    pause( 1);
    send( send_head);
    pause( 40);                /* wait 80ms for each */
  }
  ncat( send_head, '\0');          /* send terminating char (null) */
  send( send_head);
  pause( 40);
}

/***************************************************************************/
/*  readstr                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Read a string from the sensor                      */
/*  Syntax:    bit readstr( void);                            */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit readstr( void) {
  char i;

  for ( i=0; i<=3; i++)
    rgbptr[i] = rgbstring[i];    /* initiate pointer to receiving string */

  rsno = sendok = 0;

  GenericTimecount = 16;              /* Time-out for reading 16*25ms*/
  while ( GenericTimecount && !sendok) {
    feed_dog = 1;
    feed_dog = 0;
  }

  return ( sendok) ? 1:0;              /* Signal OK/fault */
}

/***************************************************************************/
/*  stoi                                              */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Convert a string to an integer                    */
/*  Syntax:    int stoi( char* s);                            */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
int stoi( char *s) {
  unsigned int n;
  unsigned char c;

  n = (int)*s++ << 8;            /* convert answer to integer */
  c = *s;
  n |= c;
  return( n);                  /* returns signed integer */
}

/***************************************************************************/
/*  read_name                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Read the name of a phosphor from the sensor            */
/*  Syntax:    bit read_name( char no);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit read_name( char no) {
  char *s, *t;
  char i;
    
  ncat( madr_head, ramsize +12 - (int)no * 20);  /* form the address */
  sendi( madr_head);                /* send read-address to m.head */
  readbin = 8+sumon;
  sendi("RN8;");                    /* read 8 chars */

  if ( !readstr()) 
    return(0);

  s = rgbstring[0];
  t = s;                        /* control checksum */
  csum = 0;
  for ( i=0; i<8; i++) 
    csum += *t++; 

  if ( sumon && ( csum != *t++)) {
      errorno = 1;
      return(0);
  }

  scopy( s, phos[no]->name, 7);          /* save 7 chars of the string */
  *(phos[no]->name + 6) = '\0';          /* end the string */

                /* if empty name then lock this phosphor else unlock */
  phos[no]->key = 0x55;              /* unlock */

  if ( !*phos[no]->name ) 
    phos[no]->key = 0;              /* lock */

  return(1);
}

/***************************************************************************/
/*  read_headid                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Read the identification from the sensor              */
/*  Syntax:    bit read_headid( void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit read_headid( void) {
  char *s;

  sendi("MS;");                /* stop measurements */
  sendi("MS;");                /* stop measurements */
  sendi("I?;");

  if (!readstr()) 
    return(0);

  outtextxy( 1, 7, rgbstring[0]);
  outtextxy( 1, 8, rgbstring[1]);
  outtextxy( 1, 9, rgbstring[2]);
  outtextxy( 1, 10, rgbstring[3]);

  sendi( idadr);
  readbin = 4;
  sendi( "RN4;");

  if ( !readstr())
    return( 0);

  s = rgbstring[0];

  outtextxy( 1, 11, ftoa( *s, 0));
  outtext( ",");
  outtext( ftoa( *(s+1), 0));
  outtext( ",");
  outtext( ftoa( *(s+2), 0));
  outtext( ",");
  outtext( ftoa( *(s+3), 0));

  sendi("MC;");
  return(1);
}

/***************************************************************************/
/*  read_size                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Read no. of phosphors and size of RAM from the sensor      */
/*  Syntax:    bit read_size( void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit read_size( void) {
  int m;
  char i, *t;

  sendi( pageadr);

  csum = 0;
  readbin = 2+sumon;
  sendi("RN2;");          /* read 2 chars */

  if (!readstr()) 
    return(0);

  t = rgbstring[0];
  ramsize = *t << 8;
  csum += *t++;
  phosmax = *t++;
  csum += phosmax;

  if ( sumon && (csum != *t))
    return(0);

  if ( phosmax<0 || phosmax>100) 
    phosmax = 7;                          /* correct errors */

  if ( ramsize >4096 || ramsize <1024) 
    ramsize = 1024;

  return(1);
}

/***************************************************************************/
/*  read_fact                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Read pre-gain factors for RGB from the sensor          */
/*  Syntax:    bit read_fact( void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit read_fact( void) {
  char *t;
  int n;

  sendi( factadr);
  csum = 0;
  readbin = 6+sumon;
  sendi("RN6;");          /* read 6 chars */

  if (!readstr()) 
    return(0);

  t = rgbstring[0];
  
  n = stoi(t);
  xfact = (float) n; 
  xfact /= 1024.0;
  csum += *t++;
  csum += *t++;
  n = stoi(t);
  yfact = (float)n; 
  yfact /= 1024.0;
  csum += *t++;
  csum += *t++;
  n = stoi(t);
  zfact = (float) n; 
  zfact /= 1024.0;
  csum += *t++;
  csum += *t++;

  if (sumon && ( csum != *t))
    return(0);

  return(1);
}

/***************************************************************************/
/*  read_k                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Read the correction matrix from the sensor            */
/*  Syntax:    bit read_k( void);                            */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
/*
   [0] [1] [2] [3] [4] [5] [6] [7] [8]   k-matrix in sensor
   [0] [3] [6] [1] [4] [7] [2] [5] [8]   k-matrix in hand held display
*/
bit read_k( void) {
  unsigned char r, s;
  char *t;
  int n;

  sendi(kmadr);
  csum = 0;

  readbin = 18+sumon;
  sendi("RN18;");

  if (!readstr()) 
    return(0);

  t = rgbstring[0];
  
  for (r = 0; r < 3; r++)
    for (s = 0; s < 3; s++)
      {
      n = stoi(t);
      k[r + s * 3] = (float)n;
      csum += *t++;
      csum += *t++;
      }

  if ( sumon && ( csum != *t))
      return(0);

  return(1);
}

/***************************************************************/
/*  read_id                                                    */
/*                                                             */
/* Written by:  Preben Christiansen, DEV                       */
/*  Revised by:  Kim Engedahl, DEV                             */
/*  Date:      910212                                          */
/*  Revised:    970822                                         */
/*                                                             */
/*  Function:  Validate the sensor type                        */
/*  Remarks:    ----                                           */
/*  Returns:  CRThead  PRJhead or 0 if error                   */
/*  Updates:    ----                                           */
/*
 Below is an array taken from calibration program for 5639/94:

 DefInternID : array[1..9,1..4] of byte =    {addr 58 - 61}
  (
  (1, 0, 0, 1),  {5639/90 serie III }
  (1, 0, 0, 2),  {5639/90 serie IV  }
  (0, 0, 0, 0),  {                  }
  (0, 0, 0, 0),  {                  }
  (1, 0, 0,32),  {5639/92 serie I   }
  (1, 0, 0,33),  {5639/92 serie II  }
  (0, 0, 0, 0),  {                  }
  (1, 0, 0,56),  {5639/93           }
  (1, 0, 0,57),  {5639/94           }
  (1, 0, 0,40)   {5639/916          }
  );

 For 5639/94 sensor:

  58   59  60  61   <---- Memory address
   1    0   0  57   <---- Contents

*/

/***************************************************************/
char read_id(void)
  {
  char *s, *t, c;
  char i;
  
  sendi(idadr); /* MA58 */
  readbin = 4;
  sendi("RN4;");

  if (!readstr())
    {               /* read type */
    errorno = 2;    /* error: No M.head */
    return(0);
    }

  s    = rgbstring[0];
  t    = s;                    /* control checksum */
  csum = 0;
  for (i = 0; i < 4; i++) 
    csum += *t++; 

  sumon = 1;
  if (csum != *t) 
    sumon--;

            /* Read customer code (1'st byte) */
            /* Philips code is xxxxxxx1 */
            /* Read type code (4'th byte) */
            /* Philips type CRT is 0 - 15 */
            /* Philips type PRJ is 16 - 31 */
            /* Philips type INDUSTRY is 32 - 63 */

  c = *(s + 3);

  if (c == 57)
    sensor_is_94 = 1;
  else
    sensor_is_94 = 0;

  if (*s & PTVhead) /* PTVhead = 1 */
    {
    if ((c < 16) && (c >= 0))
      return(CRThead);
    else 
      if ((c < 32) && (c >= 16))
        return(PRJhead);
      else
        if ((c < 64) && (c >= 32))
          return(CRThead);
        else
          return(0);
    }
  else
    {
    errorno = 3;   /* error: Wrong head */
    return(0);
    }
  }

/***************************************************************************/
/*  samestr                                            */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Compares two strings for n characters                */
/*  Syntax:    bit samestr( char* s, char* t, char n);              */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit samestr( char *s, char *t, char n) {
  char i;

  for ( i=0; i<n; i++)
    if (*s++ != *t++) 
      return(0);
  return(1);
}

/***************************************************************************/
/*  read_number                                          */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Test to see if sensor has changed                  */
/*  Syntax:    bit read_number( void);                          */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit read_number( void) {
  char *s, *t;
  unsigned char i;

  newhead = 0;
  sendi( numberadr);          /* read last 5 dig. in 12-dig. no. */
  readbin = 5+sumon;
  sendi( "RN5;");

  if ( readstr()) {
    s = rgbstring[0];
    t = s;                /* control checksum */
    csum = 0;
    for ( i=0; i<5; i++) 
      csum += *t++; 

    if (sumon && (csum != *t)) {
      errorno = 1;
      return(0);
    }

    if ( !samestr( s, number, 5))    /* if new no. then set flag */
      newhead = 1;
    scopy( s, new_number, 5);      /* save the new head no */
    sendi( kuadr);              /* read KU-no. from Sensor */
    readbin = 6+sumon;
    sendi("RN6;");
    if (readstr()) {
      s = rgbstring[0];

      t = s;                  /* control checksum */
      csum = 0;
      for ( i=0; i<6; i++) 
        csum += *t++; 

      if ( sumon && (csum != *t)) {
        errorno = 1;
        return(0);
      }

      if ( !samestr( s, kuno, 6))    /* if new no. then set flag */
        newhead = 1;
      scopy(s,new_kuno,6);          /* save the new ku-number */

      if (*menuon && (*menuno == 3)) {
        clrtext(14);            /* Sensor passed, clear warning */
        clrtext(15);
      }
      return(1);
    }
    else 
      return(0);
  }
  else 
    return(0);
}

/***************************************************************************/
/*  store_phosphor                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Stores a phosphor-matrix in the sensor                */
/*  Syntax:    bit store_phosphor( char no);                      */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit store_phosphor( char no) {
  char c, *t, storeok, try, i;

  if (no < 3) 
    return(0);                  /* phosphor 1 and 2 is prohibited */

  outtextxy(46,16,"Wait..");

  sendi("MS;");                          /* stop measurements */
  sendi("MS;");                          /* stop measurements */
  ncat(madr_head, ramsize - (int)no * 20);        /* form the address */

  try = storeok = 0;
  while (!storeok && try<2) {    /* repeat until read is ok */
    sendi(madr_head);          /* send read-address to m.head */
    sendint(nn_grt);
    sendint(nn_brt);
    sendint(nn_rgt);
    sendint(nn_bgt);
    sendint(nn_rbt);
    sendint(nn_gbt);
                        /* (phosphor name is sent after editing) */
    csum = nn_grt>>8 + (nn_grt & 0x00ff);  /* calc. check sum */
    csum += nn_brt>>8 + (nn_brt & 0x00ff);
    csum += nn_rgt>>8 + (nn_rgt & 0x00ff);
    csum += nn_bgt>>8 + (nn_bgt & 0x00ff);
    csum += nn_rbt>>8 + (nn_rbt & 0x00ff);
    csum += nn_gbt>>8 + (nn_gbt & 0x00ff);


    sendi(madr_head);                /* send read-address to m.head */
    csum = 0;
    readbin = 12+sumon;
    sendi("RN12;");                /* read 12 chars */

    if (!readstr()) 
      return(0);
    t = rgbstring[0];

    for ( i=0; i<12; i++) 
      csum += *t++;

    if (sumon) {
      if (csum == *t) 
        storeok = 1;
      else 
        try++;
    }
    else 
      storeok = 1;
  }
  if ( !storeok) {
    outtextxy( 0, 16, "error! not stored");
    pause(255);
    pause(255);
    phos[no]->key = 0;          /* lock phosphor */
    phos[no]->name[0] = '\0';      /* erase name */
    sendname(no);
    return(0);
  }
  nn_br =  nn_brt;              /* make new matrix active */
  nn_gr =  nn_grt;
  nn_bg =  nn_bgt;
  nn_rg =  nn_rgt;
  nn_gb =  nn_gbt;
  nn_rb =  nn_rbt;
  sup[0]->phos_no = no;
  *barptr[3] = no;
  strput(phos[no]->name, temp->txt);  /* copy old name to temp. store */
  temp->charno = 0;
  phos[no]->key = 0x55;           /* unlock this phosphor */
  prompts_on = 0;              /* spell prompts of for now */
  sendi("MC;");

  set_setup_no = 0;

  return(1);
}

/***************************************************************************/
/*  recall_phosphor                                      */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951005                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  Recalls a phosphor-matrix from the sensor              */
/*  Syntax:    bit recall_phosphor( char no);                    */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit recall_phosphor( char no) {
  char readok, *t, i, try;

  sendi("MS;");                    /* stop measurements */
  sendi("MS;");                    /* stop measurements */
  ncat(madr_head, ramsize - (int)no * 20);  /* form the address */

  try = readok = 0;
  while (!readok && try<2) {          /* repeat until read is ok */
    sendi(madr_head);              /* send read-address to Sensor */
    readbin = 12+sumon;
    sendi("RN12;");              /* read 12 chars */

    if (!readstr()) 
      return(0);

    t = rgbstring[0];
  
    nn_grt = stoi(t);
    csum = *t++;
    csum += *t++;
    nn_brt = stoi(t);
    csum += *t++;
    csum += *t++;
    nn_rgt = stoi(t);
    csum += *t++;
    csum += *t++;
    nn_bgt = stoi(t);
    csum += *t++;
    csum += *t++;
    nn_rbt = stoi(t);
    csum += *t++;
    csum += *t++;
    nn_gbt = stoi(t);
    csum += *t++;
    csum += *t++;

    if (sumon) {
      if (csum == *t) 
        readok = 1;
      else 
        try++;
    }
    else 
      readok = 1;
  }

  if (no > 2)                /* read name if phosphor no. > 2 */
    if (!read_name(no))        /* set error if reading failed */
      readok = 0;
  if (!readok) {
    errorno = 1;
    return(0);
  }

  nn_br =  nn_brt;            /* read was ok, accept tempor. values */
  nn_gr =  nn_grt;
  nn_bg =  nn_bgt;
  nn_rg =  nn_rgt;
  nn_gb =  nn_gbt;
  nn_rb =  nn_rbt;

  sup[0]->phos_no = no;        /* save as current phosphor no */
  sendi("MC;");

  set_setup_no = 0;

  return(1);
}

/***************************************************************************/
/*  install_head                                        */
/*                                                  */
/* Written by:  Preben Christiansen, DEV                           */
/*  Revised by:  Kim Engedahl, DEV                              */
/*  Date:      910212                                    */
/*  Revised:    951109                                    */
/*                                                  */
/* Module:    COM.C                                      */
/*  Function:  (Re)install the sensor                          */
/*  Syntax:    bit install_head( void);                        */
/*  Remarks:    ----                                      */
/*  Returns:    ----                                      */
/*  Updates:    ----                                      */
/***************************************************************************/
bit install_head( void) {

  unsigned char i, tmp;

  sendi("MS;");            /* stop measurements */
  sendi("MS;");            /* stop measurements */

  pause(1);

  if ( !( head_version = read_id())) {
    head_version = last_head_version;
    return(0);
  }

  if ( head_version != last_head_version) {
    last_head_version = allocbuf[0] = head_version;

    initall( NO_Button);

    new_head_version = 1;
  }

  if ( !read_number())       /* Check if head was changed, exit if error*/
    return( 0);

  outtextxy( 46, 16, "Wait..");

                      /* read ramsize and no. of phosphors */
                      /* read pregain factors */
                      /* read correction matrix for xyY */
  if ( !read_size() || !read_fact() || !read_k()) 
    return( 0);

  invert_k();               /* calc. the inverse of correction matrix */

  switch ( head_version) {
    case CRThead:
      tmp =  sup[0]->phos_no;

      for ( i=3; i<phosmax+1; i++)     /* read phosphor names from head */
        if ( !read_name(i)) 
          errorno = 1;

      rgbref( *barptr[1]);     /* calc. references for RGB bars */
                               /* if actual phosphor# is now illegal, select #1 */

      if (( tmp > phosmax) || ( phos[tmp]->key != 0x55))
        recall_phosphor( 1);
      else 
        recall_phosphor( tmp);
      break;

    case PRJhead:
      rgbref( *barptr[30]);         /* calc. references for RGB bars */
      recall_phosphor( 1);
      break;
  }

  sendi( "MX;");
  sendi( "MC;");

  initdisplay();

  return (1);                                  /* return OK */
}
