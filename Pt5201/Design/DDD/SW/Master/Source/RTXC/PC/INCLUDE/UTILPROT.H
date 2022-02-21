/*
 *   RTXC Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _UTILPROT_H
#define _UTILPROT_H

#include "rtxcapi.h"
#include "runtm.h"

#ifdef RADIX
extern const char RTXCRadixCTable[];
#endif

/* Function prototypes */

extern unsigned hexnibble(char);
extern int hex2bin(char *, unsigned char *, int);
extern int formatv(char *, char *, int *, ...);
extern unsigned hexnibble(char);

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

extern int atoi(const char *s);
extern int ishexdigit(char );
extern void ultoa(unsigned long, char *, int);
extern void ltoa(long, char *, int);
extern unsigned long atohl(char *);
extern long atoh(char *);
extern void uitoa(unsigned int , char *, int);
extern void itoa(int, char *, int);
extern void reverse(char *);
//extern int strlen(char const *);
extern int stcmp(char *, char *);
extern int stlen(char *);

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif
