/*	String functions */

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
typedef unsigned short	wchar_t;	/* wide char type */
#define	_STDDEF
#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))
extern int	errno;			/* system error number */

#endif	_STDDEF

#if	defined(_PIC14) || defined(_PIC12)
#define	__CONST	const
#else
#define	__CONST
#endif

#ifndef	NULL
#define	NULL	((void *)0)
#endif	NULL

extern void *	memcpy(void *, const void *, size_t);
extern void *	memmove(void *, const void *, size_t);
extern char *	strcpy(char *, const char *);
extern char *	strncpy(char *, const char *, size_t);
extern char *	strcat(char *, const char *);
extern char *	strncat(char *, const char *, size_t);
extern int	memcmp(const void *, const void *, size_t);
extern int	strcmp(const char *, const char *);
extern int	stricmp(const char *, const char *);
extern int	strncmp(const char *, const char *, size_t);
extern int	strnicmp(const char *, const char *, size_t);
extern size_t	strcoll(char *, size_t, char *);
extern __CONST void *	memchr(const void *, int, size_t);
extern size_t	strcspn(const char *, const char *);
extern __CONST char *	strpbrk(const char *, const char *);
extern size_t	strspn(const char *, const char *);
extern __CONST char *	strstr(const char *, const char *);
extern __CONST char *	stristr(const char *, const char *);
extern char *	strtok(char *, const char *);
extern char *	strdup(const char *);
extern void *	memset(void *, int, size_t);
extern char *	strerror(int);
extern size_t	strlen(const char *);
extern __CONST char *	strchr(const char *, int);
extern __CONST char *	strichr(const char *, int);
extern __CONST char *	strrchr(const char *, int);
extern __CONST char *	strrichr(const char *, int);

#ifdef	z80
#pragma	inline(memcpy)
#pragma	inline(memset)
#pragma	inline(strcpy)
#pragma	inline(strlen)
#pragma	inline(strcmp)
#endif
