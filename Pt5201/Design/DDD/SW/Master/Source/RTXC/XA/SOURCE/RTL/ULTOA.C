/* ultoa.c */

/*
 *   RTXCio    
 *   Copyright (c) 
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

extern int strlen(const char *);

/* CONFIG */
#define NEED_LTOA
#define NEED_ULTOA
/* #define NEED_REVERSE */

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


#ifdef NEED_ULTOA
/* unsigned long to ascii */
void ultoa(
unsigned long n,    /* unsigned integer to be converted */
char s[],          /* storage area to save ascii */
int radix)         /* optional radix, decimal assumed */
{
    int i = 0;

    do
        { /* generate digits in reverse order */
#ifdef RADIX
            s[i++] = lookup[n % radix]; /* get next digit */
#else
            s[i++] = n % 10L + '0'; /* get next digit */
#endif
        }
#ifdef RADIX
    while ((n /= radix) > 0L); /* delete it */
#else
    while ((n /= 10L) > 0); /* delete it */
#endif
    s[i] = '\0';

    reverse(s);
}
#endif


#ifdef NEED_LTOA
/* long to ascii */
void ltoa(
long n,            /* unsigned long to be converted */
char s[],          /* storage area to save ascii */
int radix)         /* optional radix, decimal assumed */
{
    int i = 0;

    do
        { /* generate digits in reverse order */
#ifdef RADIX
            s[i++] = lookup[n % radix]; /* get next digit */
#else
            s[i++] = n % 10L + '0'; /* get next digit */
#endif
        }
#ifdef RADIX
    while ((n /= radix) > 0L); /* delete it */
#else
    while ((n /= 10L) > 0); /* delete it */
#endif
    s[i] = '\0';

    reverse(s);
}
#endif


#ifdef NEED_REVERSE
static void reverse(s)  /* reverse string s in place */
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

/* end of ultoa.c */
