/* format.c - output formatter */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1998.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

/* modification history
 *
 * 09/12/94  wld  added support for S[0] and L[0] (minimum width)
 * 12/03/94  wld  vararg support - major changes
 * 12/03/94  wld  fixed Fw.d parse bug
 *
 * outstanding issues
 *
 * varargs - dummy va_arg calls and arguments
 * get_int() varargs issue
 *
*/

#include <string.h>

#include <stdarg.h>

/* CONFIG */
#define LONG     /* indicates long supported, suppress on microcontrollers */
#ifdef FPU /* { floating point support */
#define FLOAT   /* indicates Fx.x/Dx.x supported, suppress on most */
#endif /* } FPU */
#define RADIX    /* indicates non-decimal radix support */

#ifndef NULL
#define NULL 0
#endif

#define ESC '\x1B'

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/* itoa() on some systems does not support radix */
extern void itoa(int, char *, int);

/* uitoa() on some systems is itostr() */
extern void uitoa(unsigned int, char *, int);

#ifdef LONG
/* ltoa() on some systems does not support radix */
extern void ltoa(long, char *, int);

/* ultoa() on some systems is ltostr() */
extern void ultoa(unsigned long, char *, int);

#else

#include <stdlib.h>
extern void uitoa(unsigned int, char *, int);

#endif

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

typedef unsigned long ULONG;

static int get_int(char **);

#ifdef FLOAT

#define FLOATLESS

#ifdef FLOATLESS
static long ftomsig(float *);
static long dtomsig(double *);

static long ftolsig(float  *, char, long);
static long dtolsig(double *, char, long);
#endif
#endif

#ifdef FLOAT
static const long mul[] =
{
   1,
   10,
   100,
   1000,
   10000,
   100000,
   1000000,
   10000000,
   100000000
};

static const double r_off[] =        /* round off */
{
   0.5,
   0.05,
   0.005,
   0.0005,
   0.00005,
   0.000005,
   0.0000005,
   0.00000005,
   0.000000005
};
#endif

int formatv(char *fmt, char *buffer, va_list arg_ptr);

/*
 * version to allow interface such as formatv("(I2,I1)", buffer, &i, &j);
*/
int formatv(char *fmt, char *buffer, va_list arg_ptr)
{
   int i;
   int space;
   char fill_char = ' ';
   struct stack
   {
      int rep;
      char *pos;
   } stk[3];

   struct stack *stkptr = &stk[0];
   char *tb_ptr, *str_ptr, delim;
   int temp;
   char temp_buff[34]; /* room for (Z-,R2,UL34) */
   char *sol_ptr = buffer;
   int rep_fctr = 1;
#ifdef RADIX
   int radix = 10;
#endif
   int length = 0;
   int n;
#ifdef FLOAT
   long l_sig, m_sig;
   int DOUBLE; /* Dw.d vs Fw.d mode */
   struct flot /* floating point structure */
   {
      int width;     /* field length */
      int dec_pt;    /* no. of positions after dec pt */
   } flt;
#ifndef FLOATLESS
   float float_val;
   double double_val;
#else
   float  *float_ptr;
   double *double_ptr;
#endif
#endif

   stkptr->pos = NULL;            /* initialize stack */
   stkptr->rep = 0;

   for(;;)
   {
      switch(*fmt)
      {
         case '(':
            (++stkptr)->pos = ++fmt;
            stkptr->rep = rep_fctr;
            rep_fctr = 1;            /* reset rep_fctr to default */
            break;

         case ')':
            if ( --stkptr->rep > 0 )
               fmt = stkptr->pos;
            else
            {
               if ( (--stkptr)->pos == NULL )
	       {
                  va_end(arg_ptr);
                  return(length);
               }
               else
                  fmt++;
            }
            break;

         case '*':
            fmt++;
            rep_fctr = *(va_arg(arg_ptr, int *));
            break;

         case '0':
         case '1':           /* repetition factors */
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
            rep_fctr = get_int(&fmt);
            break;

         case 'P':                      /* pass ptr to new buffer */
            fmt++;
            buffer = va_arg(arg_ptr, char *);
            break;

         case 'N':                      /* null character */
            fmt++;
            *buffer++ = '\0';
            length++;
            break;

         case 'O':                      /* output */
            str_ptr = va_arg(arg_ptr, char *);

            fmt++;
            if (*fmt == '*')
            {
               fmt++;
               n = *(va_arg(arg_ptr, int *));
            }
            else
               n = get_int(&fmt);

            length += n;
            for (i = 1; i <= n; i++)
               *buffer++ = *str_ptr++;
            break;

         case 'Q':                      /* query length */
            fmt++;
            *(va_arg(arg_ptr, int *)) = length;
            break;

         case 'Z':                      /* zero suppress mode */
            fill_char = (char)((*++fmt == '-') ? '0' : ' ');
            fmt++;
            break;

         case 'T':                     /* tabs */
            fmt++;
            if (*fmt == '*')
            {
               fmt++;
               n = *(va_arg(arg_ptr, int *));
            }
            else
               n = get_int(&fmt);

            buffer = sol_ptr + n - 1;
            break;

         case ',':
            fmt++;
            break;

         case 'U':                   /* unsigned */
            switch(*++fmt)
            {
#ifdef LONG
               case 'L':
                  ultoa(*(va_arg(arg_ptr, unsigned long *)), temp_buff, radix);
                  break;
#endif

               case 'i':
                  uitoa((unsigned int)*(va_arg(arg_ptr, unsigned short *)),
                        temp_buff, radix);
                  break;

               case 'I':
                  uitoa(*(va_arg(arg_ptr, unsigned int *)), temp_buff, radix);
                  break;

               case 'B':
                  uitoa((unsigned int)*(va_arg(arg_ptr, unsigned char *)),
                        temp_buff, radix);
                  break;

            }
            goto FILL_BUFF;

#ifdef LONG
         case 'L':                   /* long to ascii */
            ltoa( *(va_arg(arg_ptr, long *)), temp_buff, radix);
            goto FILL_BUFF;
#endif

         case 'B':                   /* byte to ascii */
            itoa((int)*(va_arg(arg_ptr, signed char *)), temp_buff, radix);
            goto FILL_BUFF;

         case 'i':                   /* short to ascii */
            itoa((int)*(va_arg(arg_ptr, short *)), temp_buff, radix);
            goto FILL_BUFF;

         case 'I':                   /* integer to ascii */
            itoa( *(va_arg(arg_ptr, int *)), temp_buff, radix);

FILL_BUFF:
            fmt++;
            if (*fmt == '*')
            {
               fmt++;
               n = *(va_arg(arg_ptr, int *));
            }
            else
               n = get_int(&fmt);

            space = strlen(temp_buff);

            if (n == 0) /* as needed (under test) */
               n = strlen(temp_buff);

            length += n;

            if (n >= space)          /* enough room */
            {
               temp = n - space;
               tb_ptr = temp_buff;
               if (*tb_ptr == '-')  /* if neg, put - sign first */
                  if (fill_char == '0')
                  {
                     *buffer++ = *tb_ptr;
                     *tb_ptr = '0';
                     temp--;
                  }
               for (i = 1; i <= temp; i++)
                  *buffer++ = fill_char;
               for (i = 1; i <= space; i++)
                  *buffer++ = *tb_ptr++;
            }
            else  /* not enough room to print integer */
            {
               for (i = 1; i <= n; i++)
                  *buffer++ = '*';
            }
            break;

#ifdef FLOAT
         case 'D':
         case 'F':
            DOUBLE = (*fmt == 'D') ? 1 : 0;
            fmt++;
            if (DOUBLE)
            {
#ifndef FLOATLESS
               double_val = *(va_arg(arg_ptr, double *));
               m_sig = (long)double_val;
#else
               double_ptr = va_arg(arg_ptr, double *);
               m_sig = dtomsig(double_ptr);
#endif
            }
            else /* since float */
            {
#ifndef FLOATLESS
               float_val = *(va_arg(arg_ptr, float *));
               m_sig = (long) float_val;
#else
               float_ptr = va_arg(arg_ptr, float *);
               m_sig = ftomsig(float_ptr);
#endif
            }

            flt.width = get_int(&fmt); /* needs fixing */
            length += flt.width;
            if (*fmt++ == '.')
               flt.dec_pt = get_int(&fmt); /* needs fixing */
            else
               flt.dec_pt = 0;
            ltoa(m_sig, temp_buff, 10);
            space = strlen(temp_buff);
            if (space >  flt.width - flt.dec_pt - 1) /* not enough room */
            {
               temp = flt.width - flt.dec_pt - 1;
               for (i = 1; i <= temp; i++)
                  *buffer++ = '*';
               *buffer++ = '.';
               for (i = 1; i <= flt.dec_pt; i++)
                  *buffer++ = '*';
            }
            else  /* since enough room */
            {
               temp = flt.width - flt.dec_pt - 1 - space;
               tb_ptr = temp_buff;
               if (*tb_ptr == '-')
                  if (fill_char == '0')
                  {
                     *buffer++ = *tb_ptr;
                     *tb_ptr = '0';
                     temp--;
                  }
               for (i = 1; i <= temp; i++)
                  *buffer++ = fill_char;
               for (i = 1; i <= space; i++)
                  *buffer++ = *tb_ptr++;
               *buffer++ = '.';
               temp = flt.dec_pt;
#ifndef FLOATLESS
               if (DOUBLE)
               {
                 if (double_val < 0)
                 {
                    double_val = -double_val;
                    m_sig = -m_sig;
                 }
                 l_sig =(long)(mul[temp] * (double_val + r_off[temp] - m_sig));
               }
               else
               {
                 if (float_val < 0)
                 {
                    float_val = -float_val;
                    m_sig = -m_sig;
                 }
                 l_sig =(long)(mul[temp] * (float_val + r_off[temp] - m_sig));
               }
#else
               if (DOUBLE)
               {
                  l_sig = dtolsig(double_ptr, temp, m_sig);
	       }
               else
	       {
                  l_sig = ftolsig(float_ptr, temp, m_sig);
               }
#endif

               ltoa( (long) l_sig, temp_buff, 10);
               space = strlen(temp_buff);                    /* 9-15-87 */
               if (space < flt.dec_pt)
               {
                  temp = flt.dec_pt - space;
                  for (i = 1; i <= temp; i++)
                     *buffer++ = '0';
               }
               else
                  space = flt.dec_pt;
               tb_ptr = temp_buff;
               for (i = 1; i <= space; i++)
                  *buffer++ = *tb_ptr++;                      /* to here  */
            }

            break;

#endif

         case 'X':                         /* spaces */
            fmt++;
            if (*fmt == '*')
            {
               fmt++;
               n = *(va_arg(arg_ptr, int *));
            }
            else
               n = get_int(&fmt);

            for (i = 1; i <= n; i++)
               *buffer++ = ' ';
            length += n;
            break;

         case '/':                       /* new line */
            *buffer++ = '\n', length++;
            sol_ptr = buffer;
            fmt++;
            break;

#ifdef RADIX
         case 'R':                      /* radix */
            fmt++;
            if (*fmt == '*')
            {
               fmt++;
               n = *(va_arg(arg_ptr, int *));
            }
            else
               n = get_int(&fmt);

            if ( n >= 2 && n <= 36 )
               radix = n;
            break;
#endif

         case 'S':
            str_ptr = va_arg(arg_ptr, char *);

            fmt++;
            if (*fmt == '*')
            {
               fmt++;
               n = *(va_arg(arg_ptr, int *));
            }
            else
               n = get_int(&fmt);

            i = 0;
            if (n == 0)
              n = strlen(str_ptr);
            while (*str_ptr && i < n)
               *buffer++ = *str_ptr++, i++, length++;
            while (i++ < n)
               *buffer++ = ' ', length++;
            break;

         case 'H':
            fmt++;
         case '\'':
            delim = *fmt++;
            while (*fmt != delim)
               *buffer++ = *fmt++, length++;
            fmt++;
            break;

         default:
            fmt++;
            break;

      } /* end of switch */

   } /* end of for(;;) */

}

static int get_int(char **fmt_ptr)
{
   char *fmt2 = *fmt_ptr;
   int n = 0;

   while ( (*fmt2 >= '0') && (*fmt2 <= '9') )
   {
      n = 10 * n + (*fmt2 - '0');
      fmt2++;
   }
   *fmt_ptr = fmt2;

   return(n);
}

#ifdef FLOAT
static long dtomsig(double *ptr)
{
   return (long)*ptr;
}

static long ftomsig(float *ptr)
{
   return (long)*ptr;
}


static long dtolsig(double *ptr, char temp, long msig)
{
   if (*ptr < 0.0)
   {
      *ptr = -(*ptr);
      msig = -msig;
   }

   return (long)(mul[temp] * (*ptr + r_off[temp] - msig));
}

static long ftolsig(float *ptr, char temp, long msig)
{
   if (*ptr < 0.0)
   {
      *ptr = -(*ptr);
      msig = -msig;
   }

   return (long)(mul[temp] * (*ptr + r_off[temp] - msig));
}
#endif

/* End of file - format.c */
