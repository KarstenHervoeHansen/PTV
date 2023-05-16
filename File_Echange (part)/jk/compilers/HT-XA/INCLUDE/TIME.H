#ifndef	_TIME

#ifndef	_STDDEF
typedef	int		ptrdiff_t;	/* result type of pointer difference */
typedef	unsigned	size_t;		/* type yielded by sizeof */
typedef unsigned short	wchar_t;	/* wide char type */
#define	_STDDEF
#define	offsetof(ty, mem)	((int)&(((ty *)0)->mem))
#endif	_STDDEF

typedef	long	time_t;		/* for representing times in seconds */
struct tm {
	int	tm_sec;
	int	tm_min;
	int	tm_hour;
	int	tm_mday;
	int	tm_mon;
	int	tm_year;
	int	tm_wday;
	int	tm_yday;
	int	tm_isdst;
};

#define	CLOCKS_PER_SEC	1
#define	clock()	(-1L)
#define	difftime(t1, t0)	((double)((time_t)(t1)-(time_t)(t0)))
#define	_TIME
#endif	_TIME

extern int	time_zone;	/* minutes WESTWARD of Greenwich */
				/* this value defaults to 0 since with
				   operating systems like MS-DOS there is
				   no time zone information available */

extern time_t	time(time_t *);	/* seconds since 00:00:00 Jan 1 1970 */
extern int	stime(time_t *);	/* set time */
extern char *	asctime(struct tm *);	/* converts struct tm to ascii time */
extern char *	ctime(time_t *);	/* current local time in ascii form */
extern struct tm *	gmtime(time_t *);	/* Universal time */
extern struct tm *	localtime(time_t *);	/* local time */
extern time_t		mktime(struct tm *);
extern size_t		strftime(char *, size_t, const char *, const struct tm *);
