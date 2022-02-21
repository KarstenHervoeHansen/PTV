/*  dos.h

    Defines structs, unions, macros, and functions for dealing
    with MSDOS and the Intel iAPX86 microprocessor family.

*/
/*
 *      C/C++ Run Time Library - Version 6.02
 *
 *      Copyright (c) 1987, 1993 by Borland International
 *      All Rights Reserved.
 *
 */

#ifndef __DOS_H
#define __DOS_H

#if !defined(___DEFS_H)
#include <_defs.h>
#endif


#if !defined(RC_INVOKED)

#if defined(__STDC__)
#pragma warn -nak
#endif

#pragma option -a-

#endif  /* !RC_INVOKED */


#ifdef __cplusplus
extern "C" {
#endif


#define FA_NORMAL   0x00        /* Normal file, no attributes */
#define FA_RDONLY   0x01        /* Read only attribute */
#define FA_HIDDEN   0x02        /* Hidden file */
#define FA_SYSTEM   0x04        /* System file */
#define FA_LABEL    0x08        /* Volume label */
#define FA_DIREC    0x10        /* Directory */
#define FA_ARCH     0x20        /* Archive */

/* MSC names for file attributes */
#define _A_NORMAL   0x00        /* Normal file, no attributes */
#define _A_RDONLY   0x01        /* Read only attribute */
#define _A_HIDDEN   0x02        /* Hidden file */
#define _A_SYSTEM   0x04        /* System file */
#define _A_VOLID    0x08        /* Volume label */
#define _A_SUBDIR   0x10        /* Directory */
#define _A_ARCH     0x20        /* Archive */

#define SEEK_CUR    1
#define SEEK_END    2
#define SEEK_SET    0


#if !defined(__STDC__)
#define environ  _environ
#endif


struct  dfree
{
    unsigned df_avail;
    unsigned df_total;
    unsigned df_bsec;
    unsigned df_sclus;
};

struct diskfree_t
{
    unsigned total_clusters;
    unsigned avail_clusters;
    unsigned sectors_per_cluster;
    unsigned bytes_per_sector;
};

struct time
{
    unsigned char   ti_min;     /* Minutes */
    unsigned char   ti_hour;    /* Hours */
    unsigned char   ti_hund;    /* Hundredths of seconds */
    unsigned char   ti_sec;     /* Seconds */
};

struct dostime_t
{
    unsigned char hour;         /* Hours */
    unsigned char minute;       /* Minutes */
    unsigned char second;       /* Seconds */
    unsigned char hsecond;      /* Hundredths of seconds */
};

struct date
{
    int     da_year;        /* Year - 1980 */
    char    da_day;     /* Day of the month */
    char    da_mon;     /* Month (1 = Jan) */
};

struct dosdate_t
{
    unsigned char day;      /* 1-31 */
    unsigned char month;    /* 1-12 */
    unsigned int  year;     /* 1980 - 2099 */
    unsigned char dayofweek;/* 0 - 6 (0=Sunday) */
};

#if defined(__FLAT__)

#ifndef _FFBLK_DEF
#define _FFBLK_DEF
struct  ffblk   {
    long            ff_reserved;
    long            ff_fsize;
    unsigned long   ff_attrib;
    unsigned short  ff_ftime;
    unsigned short  ff_fdate;
    char            ff_name[256];
};
#endif  /* __FFBLK_DEF */

struct find_t
{
    long            reserved;
    long            size;              /* size of file */
    unsigned long   attrib;            /* attribute byte for matched file */
    unsigned short  wr_time;           /* time of last write to file */
    unsigned short  wr_date;           /* date of last write to file */
    char            name[256];         /* asciiz name of matched file */
};

#else  /* __FLAT__ */

#ifndef _FFBLK_DEF
#define _FFBLK_DEF
struct ffblk
{
    char        ff_reserved[21];
    char        ff_attrib;
    unsigned    ff_ftime;
    unsigned    ff_fdate;
    long        ff_fsize;
    char        ff_name[13];
};
#endif  /* _FFBLK_DEF */

struct find_t
{
    char     reserved[21];      /* Microsoft reserved - do not change */
    char     attrib;            /* attribute byte for matched file */
    unsigned wr_time;           /* time of last write to file */
    unsigned wr_date;           /* date of last write to file */
    long     size;              /* size of file */
    char     name[13];          /* asciiz name of matched file */
};
#endif  /* !__FLAT__ */


void                           __emit__(unsigned char __byte, ...);
void     _RTLENTRY             __int__(int __interruptnum);
unsigned _RTLENTRY  _EXPFUNC32 _dos_close(int __fd);
unsigned _RTLENTRY  _EXPFUNC32 _dos_creat(const char _FAR *__pathP, unsigned __attr,
                                          int _FAR *__fd);
unsigned _RTLENTRY  _EXPFUNC32 _dos_creatnew(const char _FAR *__pathP, unsigned __attr,
                                             int _FAR *__fd);
unsigned _RTLENTRY  _EXPFUNC32 _dos_findfirst(const char _FAR *__path,
                                              unsigned __attrib,
                                              struct find_t _FAR *__finfo);
unsigned _RTLENTRY  _EXPFUNC32 _dos_findnext(struct find_t _FAR *__finfo);
void     _RTLENTRY  _EXPFUNC32 _dos_getdate(struct dosdate_t _FAR *__datep);
unsigned _RTLENTRY  _EXPFUNC32 _dos_getdiskfree(unsigned __drive,
                                                struct diskfree_t _FAR *__dtable);
void     _RTLENTRY  _EXPFUNC32 _dos_getdrive(unsigned _FAR *__drive);
unsigned _RTLENTRY  _EXPFUNC32 _dos_getfileattr(const char _FAR *__filename,
                                                unsigned _FAR *__attrib);
unsigned _RTLENTRY  _EXPFUNC32 _dos_getftime(int __fd, unsigned _FAR *__date,
                                             unsigned _FAR *__time);
void     _RTLENTRY  _EXPFUNC32 _dos_gettime(struct dostime_t _FAR *__timep);
unsigned _RTLENTRY  _EXPFUNC32 _dos_open(const char _FAR *__pathP, unsigned __oflag,
                                         int _FAR *__fd);
unsigned _RTLENTRY  _EXPFUNC32 _dos_setdate(struct dosdate_t _FAR *__datep);
void     _RTLENTRY  _EXPFUNC32 _dos_setdrive(unsigned __drive, unsigned _FAR *__ndrives);
unsigned _RTLENTRY  _EXPFUNC32 _dos_setfileattr(const char _FAR *__filename,
                                                unsigned _FAR __attrib);
unsigned _RTLENTRY  _EXPFUNC32 _dos_setftime(int __fd, unsigned __date, unsigned __time);
unsigned _RTLENTRY  _EXPFUNC32 _dos_settime(struct dostime_t _FAR *__timep);
void     _RTLENTRY  _EXPFUNC   disable(void);
long     _RTLENTRY  _EXPFUNC   dostounix(struct date _FAR *__d, struct time _FAR *__t);
void     _RTLENTRY  _EXPFUNC   enable(void);
void     _RTLENTRYF _EXPFUNC32 getdate(struct date _FAR *__datep);
void     _RTLENTRY  _EXPFUNC32 getdfree(unsigned char __drive,
                                        struct dfree _FAR *__dtable);
int      _RTLENTRY  _EXPFUNC32 _getdrive(void);
void     _RTLENTRYF _EXPFUNC32 gettime(struct time _FAR *__timep);
void     _RTLENTRY  _EXPFUNC32 setdate(struct date _FAR *__datep);
void     _RTLENTRY  _EXPFUNC32 settime( struct time _FAR *__timep);
void     _RTLENTRY  _EXPFUNC   unixtodos(long __time, struct date _FAR *__d,
                                         struct time _FAR *__t);
int      _RTLENTRYF _EXPFUNC32 unlink(const char _FAR *__path);


#if defined(__FLAT__)
unsigned _RTLENTRY  _EXPFUNC   _dos_read(int __fd, void *__buf, unsigned __len,
                                         unsigned *__nread);
unsigned _RTLENTRY  _EXPFUNC   _dos_write(int __fd, const void *__buf, unsigned __len,
                                          unsigned *__nread );
#else
unsigned _RTLENTRY             _dos_read(int __fd, void __far *__buf, unsigned __len,
                                         unsigned _FAR *__nread);
unsigned _RTLENTRY             _dos_write(int __fd, const void __far *__buf, unsigned __len,
                                          unsigned _FAR *__nread );
#endif

#define _disable()      __emit__((unsigned char)(0xfa)) /* MSC name */
#define disable()       __emit__((unsigned char)(0xfa)) /* MSC name */
#define _enable()       __emit__((unsigned char)(0xfb)) /* MSC name */
#define enable()        __emit__((unsigned char)(0xfb)) /* MSC name */
#define geninterrupt(i) __int__(i)      /* Interrupt instruction */


#if !defined(__FLAT__) || defined(__DPMI32__)

struct  fcb
{
    char    fcb_drive;      /* 0 = default, 1 = A, 2 = B */
    char    fcb_name[8];    /* File name */
    char    fcb_ext[3];     /* File extension */
    short   fcb_curblk;     /* Current block number */
    short   fcb_recsize;    /* Logical record size in bytes */
    long    fcb_filsize;    /* File size in bytes */
    short   fcb_date;       /* Date file was last written */
    char    fcb_resv[10];   /* Reserved for DOS */
    char    fcb_currec;     /* Current record in block */
    long    fcb_random;     /* Random record number */
};

struct  xfcb
{
    char       xfcb_flag;    /* Contains 0xff to indicate xfcb */
    char       xfcb_resv[5]; /* Reserved for DOS */
    char       xfcb_attr;    /* Search attribute */
    struct fcb xfcb_fcb;     /* The standard fcb */
};

#if defined(__MSC) && !defined(__cplusplus)
struct  DOSERROR
{
        int     exterror;
        char    class;
        char    action;
        char    locus;
};
#else
struct  DOSERROR
{
        int     de_exterror;
        char    de_class;
        char    de_action;
        char    de_locus;
};
#endif  /* __MSC and not C++ */

struct fatinfo
{
    char     fi_sclus;
    char     fi_fatid;
    unsigned fi_nclus;
    int      fi_bysec;
};

#ifndef _REG_DEFS
#define _REG_DEFS

#if defined(__DPMI32__)
struct DWORDREGS
{
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esi;
    unsigned int edi;
    unsigned int cflag;
    unsigned int flags;
};
#endif

/* __PADDING allows overlaying of the DWORDREGS, WORDREGS,and BYTEREGS
 * structs.  For example: r.h.ah == LOBYTE(r.w.ax) == LOWORD(r.x.eax) */
#if defined(__DPMI32__)
#define __PADDING unsigned short : 16;
#else
#define __PADDING
#endif

struct WORDREGS
{
    unsigned short ax; __PADDING
    unsigned short bx; __PADDING
    unsigned short cx; __PADDING
    unsigned short dx; __PADDING
    unsigned short si; __PADDING
    unsigned short di; __PADDING
    unsigned short cflag;
    unsigned short flags;
};

struct BYTEREGS
{
    unsigned char al;
    unsigned char ah; __PADDING
    unsigned char bl;
    unsigned char bh; __PADDING
    unsigned char cl;
    unsigned char ch; __PADDING
    unsigned char dl;
    unsigned char dh; __PADDING
};

union REGS
{
#if defined(__DPMI32__)
    struct  DWORDREGS x;
#else
    struct  WORDREGS  x;
#endif
    struct  WORDREGS  w;
    struct  BYTEREGS  h;
};

struct SREGS
{
    unsigned short es;
    unsigned short cs;
    unsigned short ss;
    unsigned short ds;

#if defined(__DPMI32__)
    unsigned short fs;
    unsigned short gs;
#endif
};

#if !defined(__DPMI32__)
struct REGPACK
{
    unsigned    r_ax, r_bx, r_cx, r_dx;
    unsigned    r_bp, r_si, r_di, r_ds, r_es, r_flags;
};
#endif

#endif  /* _REG_DEFS */


int           _RTLENTRYF _EXPFUNC   bdos(int __dosfun, unsigned __dosdx, unsigned __dosal);
int           _RTLENTRYF            bdosptr(int __dosfun, void _FAR *__argument,
                                            unsigned __dosal);
void          _RTLENTRY             ctrlbrk(int _USERENTRY (*handler)(void));
unsigned      _RTLENTRY             _dos_commit(int __fd);
int           _RTLENTRY  _EXPFUNC16 dosexterr( struct DOSERROR _FAR *__eblkp );
int           _RTLENTRY             getcbrk(void);
void          _RTLENTRY             getfat(unsigned char __drive,
                                           struct fatinfo _FAR *__dtable);
void          _RTLENTRY             getfatd(struct fatinfo _FAR *__dtable);
unsigned      _RTLENTRY             getpsp(void);
char _FAR *   _RTLENTRY             parsfnm(const char _FAR *__cmdline,
                                            struct fcb _FAR *__fcb, int __opt);
int           _RTLENTRY             peek(unsigned __segment, unsigned __offset);
char          _RTLENTRY             peekb(unsigned __segment, unsigned __offset);
void          _RTLENTRY             poke(unsigned __segment, unsigned __offset, int __value);
void          _RTLENTRY             pokeb(unsigned __segment, unsigned __offset, char __value);
void          _RTLENTRY             segread(struct SREGS _FAR *__segp);
int           _RTLENTRY             setcbrk(int __cbrkvalue);

#if defined(__DPMI32__)
char *        _RTLENTRY             getdta(void);
int           _RTLENTRY             int386(int __intno, union REGS *__inregs,
                                           union REGS *__outregs);
int           _RTLENTRY             int386x(int __intno, union REGS *__inregs,
                                            union REGS *__outregs,
                                            struct SREGS *__segregs );
void          _RTLENTRY             setdta(char *__dta);
#else
char __far *  _RTLENTRY             getdta(void);
int           _RTLENTRY             int86(int __intno, union REGS _FAR *__inregs,
                                          union REGS _FAR *__outregs);
int           _RTLENTRY             int86x(int __intno, union REGS _FAR *__inregs,
                                           union REGS _FAR *__outregs,
                                           struct SREGS _FAR *__segregs );
int           _RTLENTRY             intdos(union REGS _FAR *__inregs,
                                           union REGS _FAR *__outregs );
int           _RTLENTRY             intdosx(union REGS _FAR *__inregs,
                                            union REGS _FAR *__outregs,
                                            struct SREGS _FAR *__segregs );
void          _RTLENTRY             intr(int __intno, struct REGPACK _FAR *__preg);
void          _RTLENTRY             setdta(char __far *__dta);
#endif

#ifndef _PORT_DEFS
#define _PORT_DEFS
unsigned char _RTLENTRY             __inportb__(unsigned __portid);
unsigned      _RTLENTRY             __inportw__(unsigned __portid);
unsigned char _RTLENTRY             __outportb__(unsigned __portid, unsigned char __value);
unsigned      _RTLENTRY             __outportw__(unsigned __portid, unsigned __value);
unsigned      _RTLENTRY             inport(unsigned __portid);
unsigned char _RTLENTRY             inportb(unsigned __portid);
unsigned      _RTLENTRY             inpw(unsigned __portid);
int           _RTLENTRY             inp(unsigned __portid);
void          _RTLENTRY             outport(unsigned __portid, unsigned __value);
void          _RTLENTRY             outportb(unsigned __portid, unsigned char __value);
unsigned      _RTLENTRY             outpw(unsigned __portid, unsigned __value);
int           _RTLENTRY             outp(unsigned __portid, int __value);

#define inport(__portid)            __inportw__(__portid)
#define inportb(__portid)           __inportb__(__portid)
#define outport(__portid, __value)  ((void) __outportw__(__portid, __value))
#define outportb(__portid, __value) ((void) __outportb__(__portid, __value))

/* MSC-compatible macros for port I/O */
#define inp(__portid)               __inportb__ (__portid)
#define inpw(__portid)              __inportw__ (__portid)
#define outp(__portid, __value)     __outportb__(__portid, (unsigned char)(__value))
#define outpw(__portid, __value)    __outportw__(__portid, __value)
#endif  /* _PORT_DEFS */


#endif  /* !__FLAT__ || DPMI32 */


#if !defined(__WIN32__) || defined(__DPMI32__)

struct  COUNTRY
{
    int     co_date;
    char    co_curr[5];
    char    co_thsep[2];
    char    co_desep[2];
    char    co_dtsep[2];
    char    co_tmsep[2];
    char    co_currstyle;
    char    co_digits;
    char    co_time;
    long    co_case;
    char    co_dasep[2];
    char    co_fill[10];
};

struct COUNTRY _FAR * _RTLENTRY _EXPFUNC32 country(int __xcode,
                                                   struct COUNTRY _FAR *__cp);
int                   _RTLENTRY _EXPFUNC32 getverify(void);
void                  _RTLENTRY _EXPFUNC32 setverify(int __value);

#endif  /* !__WIN32__ || __DPMI32__ */


#if !defined(_Windows) || defined(__DPMI16__) || defined(__DPMI32__)
void _RTLENTRYF delay(unsigned __milliseconds);
void _RTLENTRY  nosound(void);
void _RTLENTRY  sound(unsigned __frequency);
#endif  /* !_Windows || __DPMI16__ || __DPMI32__ */


#if !defined(__FLAT__)

struct  devhdr
{
    long        dh_next;        /* Next device pointer */
    short       dh_attr;        /* Attributes */
    unsigned short  dh_strat;   /* Driver strategy routine */
    unsigned short  dh_inter;   /* Driver interrupt routine */
    char        dh_name[8];     /* Device name */
};

typedef struct
{
    char    ds_drive;          /* do not change    */
    char    ds_pattern [13];   /*  these fields,       */
    char    ds_reserved [7];   /*   Microsoft reserved */
    char    ds_attrib;
    short   ds_time;
    short   ds_date;
    long    ds_size;
    char    ds_nameZ [13];     /* result of the search, asciiz */
}   dosSearchInfo;  /* used with DOS functions 4E, 4F   */


#if defined(_Windows)
  extern unsigned _RTLENTRY _0000H;
  extern unsigned _RTLENTRY _0040H;
  extern unsigned _RTLENTRY _A000H;
  extern unsigned _RTLENTRY _B000H;
  extern unsigned _RTLENTRY _B800H;
  extern unsigned _RTLENTRY _C000H;
  extern unsigned _RTLENTRY _D000H;
  extern unsigned _RTLENTRY _E000H;
  extern unsigned _RTLENTRY _F000H;
  extern unsigned _RTLENTRY _AHINCR;

  #if defined(STRICT)      
      struct HINSTANCE__;
      typedef const struct HINSTANCE__ __near *HINSTANCE;
      extern HINSTANCE _RTLENTRY _hInstance;
  #else
      extern unsigned _RTLENTRY _hInstance;
  #endif

  #define __Seg0000 ((unsigned)&_0000H)
  #define __Seg0040 ((unsigned)&_0040H)
  #define __SegA000 ((unsigned)&_A000H)
  #define __SegB000 ((unsigned)&_B000H)
  #define __SegB800 ((unsigned)&_B800H)
  #define __SegC000 ((unsigned)&_C000H)
  #define __SegD000 ((unsigned)&_D000H)
  #define __SegE000 ((unsigned)&_E000H)
  #define __SegF000 ((unsigned)&_F000H)
  #define __SegIncr ((unsigned)&_AHINCR)
#else
  #define __Seg0000 0x0000
  #define __Seg0040 0x0040
  #define __SegA000 0xA000
  #define __SegB000 0xB000
  #define __SegB800 0xB800
  #define __SegC000 0xC000
  #define __SegD000 0xD000
  #define __SegE000 0xE000
  #define __SegF000 0xF000
  #define __SegIncr 0x0100
#endif
  
#if !defined(_RTLDLL)
  extern  int _RTLENTRY  errno;
  extern  int _RTLENTRY  _doserrno;
#else
  int __far * __far _RTLENTRY __getErrno(void);
  int __far * __far _RTLENTRY __getDOSErrno(void);
  #define errno (*__getErrno())
  #define _doserrno (*__getDOSErrno())
#endif  /* !_RTLDLL */


/* Variables */

#if !defined(__DLL__)
extern  int           _RTLENTRY _argc;
extern  char        **_RTLENTRY _argv;
extern  unsigned      _RTLENTRY _psp;
#endif

extern  int const     _RTLENTRY _8087;
extern  char        **_RTLENTRY _environ;
extern  unsigned      _RTLENTRY _heaplen;
extern  unsigned char _RTLENTRY _osmajor;
extern  unsigned char _RTLENTRY _osminor;
extern  unsigned      _RTLENTRY _stklen;
extern  unsigned      _RTLENTRY _fpstklen;
extern  unsigned      _RTLENTRY _version;
extern  unsigned      _RTLENTRY _osversion;      /* MSC name for _version */

#define NFDS    20          /* Maximum number of fds */

/* axret values for _hardresume() */
#define _HARDERR_IGNORE 0   /* ignore error */
#define _HARDERR_RETRY  1   /* retry the operation */
#define _HARDERR_ABORT  2   /* abort program */
#define _HARDERR_FAIL   3   /* fail the operation */

#if !defined(_Windows) || defined(__DPMI16__)
void     _RTLENTRY  sleep(unsigned __seconds);
#endif

#if !defined(_Windows)
int      _RTLENTRY  absread(int __drive, int __nsects, long __lsect,
                           void _FAR *__buffer);
int      _RTLENTRY  abswrite(int __drive, int __nsects, long __lsect,
                             void _FAR *__buffer);
int      _RTLENTRY  allocmem(unsigned __size, unsigned _FAR *__segp);
unsigned _RTLENTRY  _dos_allocmem(unsigned __size, unsigned _FAR *__segp);
unsigned _RTLENTRY  _dos_freemem(unsigned __segx);
void     _RTLENTRY  _dos_keep(unsigned char __status, unsigned __size);
unsigned _RTLENTRY  _dos_setblock(unsigned __size, unsigned __segx,
                                 unsigned _FAR *__maxp);
int      _RTLENTRY  freemem(unsigned __segx);
void     _RTLENTRY  keep(unsigned char __status, unsigned __size);
int      _RTLENTRY  randbrd(struct fcb _FAR *__fcb, int __rcnt);
int      _RTLENTRY  randbwr(struct fcb _FAR *__fcb, int __rcnt);
int      _RTLENTRY  setblock(unsigned __segx, unsigned __newsize);

#if defined(__cplusplus)
void     _RTLENTRY  _harderr(void _USERENTRY (__far *__fptr)(unsigned __deverr,
                            unsigned __doserr, unsigned __far *__hdr));
#else
void     _RTLENTRY  _harderr(void _USERENTRY (__far *__fptr)());
#endif

void     _RTLENTRY  _hardresume(int __axret);
void     _RTLENTRY  _hardretn(int __retn);

#if defined(__cplusplus)
void     _RTLENTRYF harderr(int _USERENTRY (*__handler)(int __errval, int __ax,
                            int __bp, int __si));
#else
void     _RTLENTRYF harderr(int _USERENTRY (*__handler)());
#endif

void     _RTLENTRYF hardresume(int __axret);
void     _RTLENTRYF hardretn(int __retn);

#if !__STDC__
extern  unsigned    _RTLENTRY  _ovrbuffer;

int      __cdecl __far _OvrInitEms(unsigned __emsHandle, unsigned __emsFirst,
                                   unsigned __emsPages);
int      __cdecl __far _OvrInitExt(unsigned long __extStart,
                                   unsigned long __extLength);
#endif

#endif  /* !_Windows */

#if !__STDC__
#define MK_FP(seg,ofs) ((void __seg *)(seg) + (void __near *)(ofs))
#define FP_SEG(fp)     ((unsigned)(void __seg *)(void __far *)(fp))
#define FP_OFF(fp)     ((unsigned)(fp))

void _RTLENTRY __cli__(void);
void _RTLENTRY __sti__(void);

#ifdef __cplusplus
void        _RTLENTRY _chain_intr(void interrupt (__far *__target)(...));
void interrupt (__far * _RTLENTRY  _dos_getvect(unsigned __interruptno))(...);
void        _RTLENTRY _dos_setvect(unsigned __interruptno,
                                   void interrupt(__far *__isr)(...));
void interrupt (__far * _RTLENTRYF getvect(int __interruptno))(...);
void        _RTLENTRYF setvect(int __interruptno,
                               void interrupt (__far *__isr)(...));

int  inline _RTLENTRY  peek(unsigned __segment, unsigned __offset)
{
    return (*((int __far *)MK_FP(__segment, __offset)));
}

char inline _RTLENTRY  peekb(unsigned __segment, unsigned __offset)
{
    return (*((char __far *)MK_FP(__segment, __offset)));
}

void inline _RTLENTRY  poke(unsigned __segment, unsigned __offset, int __value)
{
    (*((int __far *)MK_FP(__segment, __offset)) = __value);
}

void inline _RTLENTRY  pokeb(unsigned __segment, unsigned __offset, char __value)
{
    (*((char __far *)MK_FP(__segment, __offset )) = __value);
}

#else  /* __cplusplus */

void        _RTLENTRY  _chain_intr(void interrupt(__far *__target)());
void interrupt (__far * _RTLENTRY  _dos_getvect(unsigned __interruptno))();
void        _RTLENTRY  _dos_setvect(unsigned __interruptno,
                                    void interrupt (__far *__isr)());
void interrupt (__far * _RTLENTRYF getvect(int __interruptno))();
void        _RTLENTRYF setvect(int __interruptno,
                               void interrupt(__far *__isr)());

#define peek(a,b)    (*((int  __far * )MK_FP((a),(b))))
#define peekb(a,b)   (*((char __far * )MK_FP((a),(b))))
#define poke(a,b,c)  (*((int  __far * )MK_FP((a),(b))) = (int)(c))
#define pokeb(a,b,c) (*((char __far * )MK_FP((a),(b))) = (char)(c))

#endif  /* !__cplusplus */

#endif  /* !__STDC__ */


#if defined(__DPMI16__)
typedef enum
{
    RTM_OK             = 0x00,
    RTM_NO_MEMORY      = 0x01,  /* out of memory */
    RTM_FILE_IO_ERROR  = 0x22   /* error during file IO */
} RTMstatus_t;     /* returns from RTM API calls */

RTMstatus_t __far __pascal MEMinitSwapFile(char __far *fileName, unsigned long fileSize);
RTMstatus_t __far __pascal MEMcloseSwapFile(int del);
unsigned short    __pascal RTMgetVersion(void);
#endif


#else  /* defined __FLAT__ */


#if defined(__MT__)
  extern  int * _RTLENTRY _EXPFUNC __errno(void);
  extern  int * _RTLENTRY _EXPFUNC __doserrno(void);
  #define errno (*__errno())
  #define _doserrno (*__doserrno())
#else  /* __MT__ */
  extern  int _RTLENTRY _EXPDATA errno;
  extern  int _RTLENTRY _EXPDATA _doserrno;
#endif  /* !__MT__ */


/* Variables */
#if defined(_RTLDLL)
    extern int           _RTLENTRY __dll_argc;
    extern char **       _RTLENTRY __dll_argv;
    #define _argc __dll_argc
    #define _argv __dll_argv
#else
    extern int           _RTLENTRY _EXPDATA _argc;
    extern char **       _RTLENTRY _EXPDATA _argv;
#endif

extern int const     _RTLENTRY _EXPDATA _8087;
extern char **       _RTLENTRY _EXPDATA _environ;

extern unsigned char _RTLENTRY _EXPDATA _osmajor;
extern unsigned char _RTLENTRY _EXPDATA _osminor;
extern unsigned      _RTLENTRY _EXPDATA _osversion;
extern unsigned      _RTLENTRY _EXPDATA _version;

void _RTLENTRY _EXPFUNC _sleep(unsigned __seconds);

#define sleep(unsigned) _sleep(unsigned)


#if defined(__DPMI32__)
unsigned short __stdcall GetLoaderVersion(void);
#endif


#if !defined(__DPMI32__) && !defined(RC_INVOKED)
#pragma obsolete _dos_close
#pragma obsolete _dos_creat
#pragma obsolete _dos_creatnew
#pragma obsolete _dos_findfirst
#pragma obsolete _dos_findnext
#pragma obsolete _dos_getdate
#pragma obsolete _dos_getdiskfree
#pragma obsolete _dos_getdrive
#pragma obsolete _dos_getfileattr
#pragma obsolete _dos_getftime
#pragma obsolete _dos_gettime
#pragma obsolete _dos_open
#pragma obsolete _dos_read
#pragma obsolete _dos_setdate
#pragma obsolete _dos_setdrive
#pragma obsolete _dos_setfileattr
#pragma obsolete _dos_setftime
#pragma obsolete _dos_settime
#pragma obsolete _dos_write
#endif


#endif  /* __FLAT__ */


#ifdef __cplusplus
}
#endif


#if !defined(RC_INVOKED)

#pragma option -a. /* restore default packing */

#if defined(__STDC__)
#pragma warn .nak
#endif

#endif  /* !RC_INVOKED */


#endif  /* __DOS_H */
