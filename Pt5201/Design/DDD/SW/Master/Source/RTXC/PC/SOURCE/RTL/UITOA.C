/* uitoa.c */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1998.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/*
 * itoa, ltoa, uitoa, and ultoa are used by format()
 * but uitoa is not a standard C library function
*/

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

void ultoa(unsigned long, char *, int);

void uitoa(unsigned int, char *, int);

/* unsigned integer to ascii */
void uitoa(
unsigned int n,    /* unsigned integer to be converted */
char s[],          /* storage area to save ascii */
int radix)         /* optional radix, decimal assumed */
{
   /* ultoa is supported by Borland so just call it */
   ultoa((unsigned long)n, s, radix);
}

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

/* End of file - uitoa.c */
