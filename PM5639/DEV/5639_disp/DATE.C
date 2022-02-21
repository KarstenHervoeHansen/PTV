/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997              */
/*  Project:    PM5639 Display Unit                            */
/*  Module:    DATE.C                                    */
/*  Author:    Kim Engedahl, DEV                              */
/*  Org. date:  921015                                    */
/*  Rev. date:  970822, KEn  DEV                              */
/*  Status:    Version 2.3                                  */
/*                                                  */
/*  This module contains the following functions:                  */
/*                                                  */
/*       void date(void)                                */
/*                                                  */
/* Changes:                                            */
/*  970822 New date and released as version 2.3                    */
/*  951207 New date and released as version 2.2                    */
/* 920520 Now supports projector head                           */
/* 920520 For earlier versions of history see u'v'-version             */
/***************************************************************************/
#include "pra.h"

#include <reg51.h>        /* Define 8051 registers */
#include <defs.h>          /* Own definitions */
#include <sub.h>          /* Own definitions */
#include <tables.h>        /* Own definitions */

void date( void) {
  char *p;

  p = datebuf;

  scopy( "030710 Ver 2.6", p, 15); /* checksum = 0x235F */
}
