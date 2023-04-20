/*	@(#)intrpt.c	1.5
 *
 *	C source file demonstrating cxa interrupt function
 *
 *	Compile using '-s' to view mixed C/assembly code.
 */
                        /* Argument psw of the _using function qualifier
                         * defines the value of the PSW placed in the
                         * interrupt vector table. PSWH bits RS0 and RS1
                         * select the active register bank used by the
                         * interrupt handler
			 */

#define  PSWH_0  0x80   /* system mode, register bank 0, low priority  */
#define  PSWH_1  0x9F   /* system mode, register bank 1, high priority */
#define  PSWL_X  0x0

_bit	 b;		/* interrupt handler sets a global bitvariable */

void main(void)
{
  while( 1 ) ;
}

_interrupt( 16 ) _using( PSWH_0 << 8 | PSWL_X ) void on( void )
{
                        /* All registers that might possibly be corrupted
                         * during the execution of the interrupt are
                         * saved on function entry and restored on
                         * function exit
			 */
  b = !b;
                        /* Vector 16 is the first entry in the TRAP vector
                         * table at adress 40H (64)
			 */
}


                        /* The appropriate interrupt vector, consisting
                         * of a JMP instruction jumping to the interrupt
                         * function, is generated. The vector may be
                         * suppressed with the -v options or the #pragma
                         * novector
			 */
#pragma novector


_interrupt( 17 ) _using( PSWH_1 << 8 | PSWL_X ) void off( void )
{
#pragma asm
  NOP		        ; you can make your own entry code here
#pragma endasm

  b = 0;

#pragma asm
  NOP		        ; you can make your own exit code here
#pragma endasm
}
