/*
 *  1996 Copyright TASKING, Inc.
 *
 *  COMPILATION PROCEDURE:  include HLL and ASM level symbols.
 *  PROCEDURES:	            main(), factorial(), myputs(), mygetchar()
 *  EXTERNAL PROCEDURES:    addone()
 */
#include <string.h>
#include <simio.h>
#define BELL_CHAR       '\007'

typedef enum color_e
{ 
    red, yellow, blue 
} 
color_type;

typedef struct rec_s
{ 
    short          a;
    char           b[16];
    unsigned long  c;
    color_type     color;
}
rec_type;

unsigned long    table[8] = { 0 };  /* table of factorial values            */
         long    initval  = 0;      /* initialization value for calculation */
         char    outbuf[80]; 	    /* output buffer for a line             */
         char    inbuf[2];          /* input buffer for one character       */
         char    bell[2] =   { BELL_CHAR, '\0' };

        rec_type recordvar = {-1, "Tasking", 12345UL, blue};
static  rec_type statrec   = {57, "hello,world", 54321UL, red };

        int     	 addone(int);
        unsigned long    factorial(int);
        void    	 myputs(char *);
        void		 mygetchar(char *);

void	main(void)
{
    unsigned long    sum;	/* will be 17+sum of factorials from 0 to 7 */
             int     loopvar;   /* the loop counter			    */
             char    cvar;	/* sample char variable			    */

    initval = 17;

    if (initval > recordvar.a) 
    {
	sum = 0;
    }

    /* This loop has an upper limit which is too high. */
    /* As a result, initval will get clobbered.	       */

    for (loopvar = 0; loopvar <= 8; ++loopvar) 
    {
	table[ loopvar ] = factorial(loopvar);
        sum += table[ loopvar ];
    }

    sum += initval;

    if (sum == 5931) 
    {
	myputs("pass\n");
    } 
    else 
    {
	myputs("fail\n");
    }
    
    recordvar.color = red;
    recordvar.a = strlen(recordvar.b);

    /* An infinite loop to be discovered using XVW. */
    while (loopvar) 
    {
	sum = sum + 1;
    }

    recordvar.color = blue;

    /* A call to a function written in assembly language: */
    sum = addone(5);

    myputs("Do you want to ring the bell (y or n)? ");
    mygetchar(&cvar);

    if (cvar == 'y') 
    {
	myputs(bell);
    }
    myputs("End of demo\n");
}


/* Compute the factorial of a number */

unsigned long    factorial(int num)
{
    char locvar = 'x';     /* an unused local variable */

    if (num < 2) 
	return 1;
    else 
	return num * factorial(num - 1);
}


/*  Put a string to an output port */

void    myputs(char *string)
{
    strcpy(outbuf, string);
    _simo(1, outbuf, 80);
}


/*  Read a line from an input port */

void    mygetchar(char *c)
{
    _simi(0, inbuf, 2);
    *c = inbuf[0];
}
