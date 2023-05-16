/*	@(#)builtin.c	1.4
 *
 *	C source file demonstrating cxa built-in functions
 *
 *	Compile using '-s' option to inspect generated code.
 */

static _bit		b;
static int		sj;


void
rotate( int pi )
{
  int	ri = 2;

  sj = _ror8( pi, ri );
  sj = _rorc8( pi, ri );
  sj = _ror16( pi, ri );
  sj = _rorc16( pi, ri );
  sj = _rol8( pi, 4 );
  sj = _rolc8( pi, 4 );
  sj = _rol16( pi, 4 );
  sj = _rolc16( pi, 4 );
}


void
semaphore( void )
{
  void g( void );
  void h( void );

  if ( _testclear( b ) )
    {	                /* success: semaphore 'b' was free (1) and now used
	                 * for our critical region (set to 0). Note that the
	                 * code of this action may be longer then 127 words
	                 */
      g();
      b = 1;	        /* end critical actions: free semaphore */
    }
  else
    {	                /* failed: semaphore 'b' was (and is) occupied (0) */
      h();
    }
}


void
sys_control( void )
{
  register int value;

  value = P0;           /* read from port P0	*/
  _nop();		/* delay for one cycle 	*/
  P1 = value;	        /* write to port P1	*/

  _intxa( 17 );         /* Execute a software interrupt
 	                 * via the software TRAP function.
 	                 * 17 is the second entry in the
 	                 * TRAP table.
			 */
}


#define OVERFLOW  1

extern int errno;
void
div( char divisor16, unsigned char divisoru16,
     int  divisor32, unsigned int  divisoru32 )
{
  static int          m16;     char          d16;
  static unsigned int mu16;    unsigned char du16;
  static long         m32;     int           d32;
  static unsigned     mu32;    unsigned int  du32;

                        /* The intermediate result after the division
	                 * must fit,  else behaviour is undefined.
	                 */

  if( d16 = _div16( m16, divisor16 ), V )
    errno = OVERFLOW;
  if( du16 = _divu16( mu16, divisoru16 ), V )
    errno = OVERFLOW;
  if( d32 = _div32( m32, divisor32 ), V )
    errno = OVERFLOW;
  if( du32 = _divu32( mu32, divisoru32 ), V )
    errno = OVERFLOW;
}


void
mod( char divisor16, unsigned char divisoru16,
     int  divisor32, unsigned int  divisoru32 )
{
  static int          m16;     char          d16;
  static unsigned int mu16;    unsigned char du16;
  static long         m32;     int           d32;
  static unsigned     mu32;    unsigned int  du32;

                        /* The intermediate result after the division
	                 * must fit,  else behaviour is undefined.
	                 */

  if( d16 = _mod16( m16, divisor16 ), V )
    errno = OVERFLOW;
  if( du16 = _modu16( mu16, divisoru16 ), V )
    errno = OVERFLOW;
  if( d32 = _mod32( m32, divisor32 ), V )
    errno = OVERFLOW;
  if( du32 = _modu32( mu32, divisoru32 ), V )
    errno = OVERFLOW;
}


unsigned char
f_da( unsigned char operand )
{
  unsigned char deci;

  deci = _da( operand + 1 );

  return( deci );
}
