#ifndef	__SETJMP_H
#define	__SETJMP_H

#if	defined(z80)
typedef	int	jmp_buf[5];

#elif	defined(i8086)
typedef	int	jmp_buf[8];

#elif	defined(i8096)
typedef	int	jmp_buf[10];

#elif	defined(m68k)
typedef	int	jmp_buf[10];

#elif	defined(_XA_)
typedef	int	jmp_buf[6];

#elif	defined(m6809)
typedef	int	jmp_buf[4];

#elif	defined(m6800)
typedef	int	jmp_buf[2];

#elif	defined(i8051)
typedef int	jmp_buf[4];

#elif	defined(h8300)||defined(h8300h)
typedef int	jmp_buf[2];

#elif	defined(m6805)
#error	setjmp() and longjmp() cannot be implemented on the 6805

#else
#error	setjmp.h: Processor type not supported, or not defined.
#endif

extern	int	setjmp(jmp_buf);
extern void	longjmp(jmp_buf, int);

#endif	/* __SETJMP_H */
