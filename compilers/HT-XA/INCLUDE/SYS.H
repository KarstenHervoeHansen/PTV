/*
 *	System-dependent functions.
 */

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
typedef	unsigned short	wchar_t;	/* wide char type */
#define	_STDDEF
#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))
#endif	_STDDEF

#ifndef	NULL
#define	NULL	((void *)0)
#endif	NULL

extern int	errno;			/* system error number */

#if	defined(DOS)
#ifndef	_FREEMEM
#define	_FREEMEM
struct freemem {
	unsigned long 	fr_dosmem;
	unsigned long	fr_extmem;
};
#endif
extern void	getfreemem(struct freemem *);
#endif

extern int	execl(char *, char *, ...);
extern int	execv(char *, char **);
extern int	execve(char *, char **, char **);
extern int	execle(char *, char *, ...);
extern int	execvp(char *, char **);
extern int	execlp(char *, char *, ...);
extern int	getpid(void);
extern char *	mktemp(char *);
#if	defined(unix) || defined(ATDOS)
extern void	sync(void);
extern int	fork(void);
extern int	wait(int *);
extern int	getuid(void);
extern int	setuid(int);
extern int	kill(int, int);
extern int	alarm(int);
extern int	pause(void);
extern void	sleep(unsigned int);
extern int	setpgrp(void);
extern int	getpgrp(void);
#else	unix
extern int	spawnl(char *, char *, ...);
extern int	spawnv(char *, char **);
extern int	spawnle(char *, char *, ...);
extern int	spawnve(char *, char **, char **);
extern int	spawnlp(char *, char *, ...);
extern int	spawnvp(char *, char **);
#endif	unix
extern int	chdir(char *);
extern int	chdrv(char *);
extern int	mkdir(char *);
extern int	rmdir(char *);
extern char *	getcwd(char *);
extern void	persist_validate(void);
extern int	persist_check(int);
extern char *	getdrv(void);
extern char **	_getargs(char *, char *);
extern int	_argc_;

#if	z80 || i8086
#define	inp(p)		(*(port unsigned char *)(p))
#define	outp(p,v)	(*(port unsigned char *)(p) = (v))
#if	i8086
#define	inpw(p)		(*(port unsigned short *)(p))
#define	outpw(p,v)	(*(port unsigned short *)(p) = (v))
#endif
#endif

extern void *	sbrk(size_t);
extern void	brk(void *);
#if	_HOSTED
struct _iob;
extern int	_pnum(unsigned long, signed char, signed char, unsigned char, unsigned char, struct _iobuf *, unsigned char);
extern int	_fnum(double, short, short, short, struct _iobuf *, short);
#else
extern int	_pnum(unsigned long, signed char, signed char, unsigned char, unsigned char, struct __prbuf *, unsigned char);
extern int	_fnum(double, short, short, short, struct __prbuf *, short);
#endif

#if	defined(ATDOS)
extern int	load_driver(char *);
extern int	send_message(unsigned short, unsigned short, unsigned long,
			unsigned long, void *, unsigned short);
extern int	get_message(unsigned short *, unsigned long *, void *,
			unsigned short *);
#endif
