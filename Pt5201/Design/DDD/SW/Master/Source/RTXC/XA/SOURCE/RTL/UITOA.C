/* uitoa.c */

/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/*
 * itoa and uitoa are used by format()
*/

extern int strlen(const char *);

#define NEED_ITOA
#define NEED_UITOA
#define NEED_REVERSE

/* CONFIG */
#define RADIX  /* indicates non-decimal radix support */
               /* some versions don't support non-decimal radix */

#ifdef NEED_REVERSE
void reverse(char *);
#else
extern void reverse(char *);
#endif

#ifdef RADIX
static const char lookup[] =
{
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   'A', 'B', 'C', 'D', 'E', 'F'
};
#endif


#ifdef NEED_UITOA
/* unsigned integer to ascii */
void uitoa(
unsigned int n,    /* unsigned integer to be converted */
char s[],          /* storage area to save ascii */
int radix)         /* optional radix, decimal assumed */
{
    int i = 0;

    do
        { /* generate digits in reverse order */
#ifdef RADIX
            s[i++] = lookup[n % radix]; /* get next digit */
#else
            s[i++] = n % 10 + '0'; /* get next digit */
#endif
        }
#ifdef RADIX
    while ((n /= radix) > 0); /* delete it */
#else
    while ((n /= 10) > 0); /* delete it */
#endif
    s[i] = '\0';

    reverse(s);
}
#endif


#ifdef NEED_ITOA
/* integer to ascii */
void itoa(
int n,       /* integer to be converted */
char s[],    /* storage area to save ascii */
int radix)   /* optional radix, decimal assumed */
{
    int i = 0;
    int sign;

    if ((sign = n) < 0) /* record sign */
        n = -n;         /* make n positive */

    do
        { /* generate digits in reverse order */
#ifdef RADIX
            s[i++] = lookup[n % radix]; /* get next digit */
#else
            s[i++] = n % 10 + '0'; /* get next digit */
#endif
        }
#ifdef RADIX
    while ((n /= radix) > 0); /* delete it */
#else
    while ((n /= 10) > 0); /* delete it */
#endif
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';

    reverse(s);
}
#endif


#ifdef NEED_REVERSE
void reverse(s)  /* reverse string s in place */
char s[];
{
    char c;
    int i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
#endif

/* end of uitoa.c */
