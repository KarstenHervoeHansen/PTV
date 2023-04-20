/*
 *	Low-level console I/O functions
 */

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
typedef	unsigned short	wchar_t;	/* wide char type */
#define	_STDDEF
#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))
#endif	/* _STDDEF */

#ifndef	NULL
#define	NULL	((void *)0)
#endif	/* NULL */

extern int	errno;			/* system error number */

#if	!_HOSTED
extern void	init_uart(void);
#endif
extern char	getch(void);
extern char	getche(void);
extern void	putch(char);
extern void	ungetch(char);
#ifdef	_MPC_
extern bit	kbhit(void);
#else
extern int	kbhit(void);
#endif
extern char *	cgets(char *);
extern void	cputs(const char *);
