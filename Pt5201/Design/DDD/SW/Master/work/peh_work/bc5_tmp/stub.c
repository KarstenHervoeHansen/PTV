
#include "rtxcapi.h"

void rtxcinit(void)         // initialize RTXC
{
}

void ticktimer_start(void)  // initialize tick timer
{
}


char setintlvl(char A)        // function to set task PPL
{
    return A;
}


int KS_user(int (*fun)(void *), void *arg)
{
	 return fun(arg);
}

KSRC KS_unlock(RESOURCE unused)
{
	 return unused;
}

SSTATE KS_inqsema(SEMA unused)
{
	 return unused;
}

void * KS_ISRalloc(MAP unused)
{
	 return (void *)unused;
}

TASK KS_inqres(RESOURCE unused)
{
    return unused;
}

void * KS_alloc(MAP A)
{
	 return (void *)A;
}

void KS_free(MAP A, void *B)
{
	 A=A; B=B;
}

TICKS KS_inqtimer(CLKBLK ks_clk *A)
{
	 return (TICKS)A;
}

PRIORITY KS_inqpriority(TASK A)
{
	 return (TICKS)A;
}

void KS_enqueuew(QUEUE A, void *B)
{
	 A = A; B = B;
}

void KS_dequeuew(QUEUE A, void *B)
{
	 A = A; B = B;
}

void KS_purgequeue(QUEUE A)
{
	 A = A; 
}


KSRC KS_defqueue(QUEUE A, size_t B, int C, void ks_queue *D, int E)
{
	 A =A; B = B; C = C; D = D;E = E;
	 return 0;
}

KSRC KS_wait(SEMA A)
{
	 return A;
}

SEMA KS_waitm(const SEMA *A)
{
	 return (SEMA)A;
}

KSRC KS_waitt(SEMA A, TICKS B)
{
	 A = A; B = B;
	 return 0;
}

void KS_delay(TASK A, TICKS B)
{
    A = A; B = B;
}


CLKBLK ks_clk * KS_start_timer(CLKBLK ks_clk *A, TICKS B, TICKS C, SEMA D)
{
	 A =A; B = B; C = C; D = D;
	 return 0;
}

KSRC KS_stop_timer(CLKBLK ks_clk *A)
{
	 return (KSRC)A;
}

KSRC KS_restart_timer(CLKBLK ks_clk * A, TICKS B, TICKS C)
{
	 A =A; B = B; C = C; 
	 return 0;
}


void KS_pend(SEMA A)
{
    A = A;
}

KSRC KS_lockw(RESOURCE A)
{
	 return A;
}

CLKBLK ks_clk * KS_alloc_timer(void)
{
    return 0;
}


void KS_free_timer(CLKBLK ks_clk *A)
{
    A = A;
}

KSRC KS_signal(SEMA A)
{
    return A;
}

KSRC KS_deftask(TASK A, PRIORITY B, ks_stk char *C, size_t D, void (*E)(void))
{
	A = A; B = B; C = C; D = D; E = E;
	 return 0;
}

void KS_defpriority(TASK A, PRIORITY B)
{
	A = A; B = B; 
}

void KS_resume(TASK A)
{
	 A = A;
}

void KS_suspend(TASK A)
{
	 A = A;
}


TASK KS_alloc_task(void)
{
	 return 0;
}

void KS_terminate(TASK A)
{
	 A = A;
}

void KS_execute(TASK A)
{
	 A = A;
}

FRAME ks_stk * KS_ISRexit(FRAME ks_stk *A, SEMA B)
{
	B = B;
	return A;
}

MAP KS_create_part(void *A, size_t B, size_t C)
{
	A = A;
	B = B;
	C = C;
	return 0;
}

KSRC KS_dequeue(QUEUE A, void *B)
{
	A = A;
	B = B;
	return 0;
}

KSRC KS_dequeuet(QUEUE A, void *B, TICKS C)
{
	A = A;
	B = B;
	C = C;
	return 0;
}



