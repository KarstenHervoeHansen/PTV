#include	<xa.h>
#include	<intrpt.h>

static long	i, j, k;

void banked interrupt
timer_tick(void)
{
	i++;
}

void banked interrupt
timer_tick1(void)
{
	j++;
}

main()
{
	ROM_VECTOR(IV_T0, timer_tick, IV_SYSTEM+IV_PRI14+IV_BANK1);	// set vector
	ROM_VECTOR(IV_T1, timer_tick1, IV_SYSTEM+IV_PRI15+IV_BANK2);	// set vector
	PSWH &= 0xF0;				// reduce processor priority
	IPA0 |= 0xE0;				// set timer0 priority
	IPA1 |= 0xF0;				// set timer1 priority
	TMOD = 0x22;				// timer mode 
	RTL0 = 0x20;				// reload value
	TR0 = 1;				// enable timer
	ET0 = 1;				// enable timer interrupts
	TR1 = 1;				// enable timer
	ET1 = 1;				// enable timer interrupts
	ei();					// enable global interrupts
	for(;;)					// loop forever
		k++;
}

