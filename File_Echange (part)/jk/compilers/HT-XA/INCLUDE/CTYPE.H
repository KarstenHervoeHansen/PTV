#if	defined(_MPC_)

extern bit isalpha(char);
extern bit isupper(char);
extern bit islower(char);
extern bit isdigit(char);
extern bit isxdigit(char);
extern bit isspace(char);
extern bit ispunct(char);
extern bit isalnum(char);
extern bit isprint(char);
extern bit isgraph(char);
extern bit iscntrl(char);

extern char	toupper(char);
extern char	tolower(char);
#else

#define	_U	0x01
#define	_L	0x02
#define	_N	0x04
#define	_S	0x08
#define _P	0x10
#define _C	0x20
#define	_X	0x40
#define	_B	0x80

#ifdef	_XA_
extern code unsigned char _ctype_[257];
#else
extern const unsigned char _ctype_[257];
#endif

#define	isalpha(c)	((_ctype_+1)[(unsigned char)(c)]&(_U|_L))
#define	isupper(c)	((_ctype_+1)[(unsigned char)(c)]&_U)
#define	islower(c)	((_ctype_+1)[(unsigned char)(c)]&_L)
#define	isdigit(c)	((_ctype_+1)[(unsigned char)(c)]&_N)
#define	isxdigit(c)	((_ctype_+1)[(unsigned char)(c)]&(_N|_X))
#define	isspace(c)	((_ctype_+1)[(unsigned char)(c)]&_S)
#define ispunct(c)	((_ctype_+1)[(unsigned char)(c)]&_P)
#define isalnum(c)	((_ctype_+1)[(unsigned char)(c)]&(_U|_L|_N))
#define isprint(c)	((_ctype_+1)[(unsigned char)(c)]&(_P|_U|_L|_N|_B))
#define isgraph(c)	((_ctype_+1)[(unsigned char)(c)]&(_P|_U|_L|_N))
#define iscntrl(c)	((_ctype_+1)[(unsigned char)(c)]&_C)

extern char	toupper(int);
extern char	tolower(int);
#endif

#define isascii(c)	(!((c)&~0x7F))
#define toascii(c)	((c)&0x7F)

