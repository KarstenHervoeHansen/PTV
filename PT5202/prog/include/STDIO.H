/*
 *      Version : @(#)stdio.h	1.2
 */

/**************************************************************************
**                                                                        *
**  FILE        :  stdio.h                                                *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes and macros for            *
**                 input and output functions                             *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef _STDIO_H
#define _STDIO_H
#include <stdarg.h>

#ifndef _SIZE_T_DEF
typedef unsigned int    size_t;
#define _SIZE_T_DEF
#endif

typedef unsigned long   fpos_t;

#define NULL	0

#define BUFSIZ  512
#define _NFILE  20

struct _iobuf {
        char *  _ptr;
        int     _cnt;
        char *  _base;
        char    _flag;
        char    _file;
        int     _bufsiz;
        };

typedef struct _iobuf   FILE;

extern FILE _iob[_NFILE];

#define stdin   (&_iob[0])
#define stdout  (&_iob[1])
#define stderr  (&_iob[2])
#define stduser (&_iob[3])

#define EOF     (-1)

#define _IOFBF          0
#define _IONBF          04
#define _IOLBF          0100
#define _IOREAD         01
#define _IOWRT          02
#define _IOMYBUF        010
#define _IOEOF          020
#define _IOERR          040
#define _IOSTRG         0100    
#define _IORW           0200

#define FOPEN_MAX       _NFILE  /* stdin, stdout, stderr, stduser */
#define FILENAME_MAX    6

#define SEEK_CUR        1
#define SEEK_END        2
#define SEEK_SET        0

#define L_tmpnam        24
#define P_tmpdir        "/tmp"
#define TMP_MAX         32767

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
int    fprintf  ( FILE *, const char *, ... );
int    fscanf   ( FILE *, const char *, ... );
int    printf   ( const char *, ... );
int    scanf    ( const char *, ... );
int    sprintf  ( char *, const char *, ... );
int    sscanf   ( const char *, const char *, ... );
int    vfprintf ( FILE *, const char *, va_list );
int    vprintf  ( const char *, va_list );
int    vsprintf ( char *, const char *, va_list );
int    getc     ( FILE * );
int    getchar  ( void );
int    fgetc    ( FILE * );
char * fgets    ( char *, int, FILE * );
int    putc     ( int, FILE * );
int    putchar  ( int );
int    fputc    ( int, FILE * );
int    fputs    ( const char *, FILE * );
char * gets     ( char * );
int    puts     ( const char * );
void   clearerr ( FILE * );
int    feof     ( FILE * );
int    ferror   ( FILE * );
void   perror   ( const char * );

int    ungetc   ( int, FILE * );

/* Direct input/output */
size_t fread    ( void *, size_t, size_t, FILE * );
size_t fwrite   ( const void *, size_t, size_t, FILE * );

FILE * fopen    ( const char *, const char * );
FILE * freopen  ( const char *, const char *, FILE * );
int    fclose   ( FILE * );
int    fflush   ( FILE * );
int    setvbuf  ( FILE *, char *, int, size_t );
void   setbuf   ( FILE *, char * );

int    fgetpos  ( FILE * , fpos_t *  );
int    fseek    ( FILE * , long , int  );
int    fsetpos  ( FILE * , const fpos_t *  );
long   ftell    ( FILE *  );
int    remove   ( const char * );
int    rename   ( const char *, const char * );
void   rewind   ( FILE * );
FILE * tmpfile  ( void );
char * tmpnam   ( char * );


/* Low level input/output routines */
int    _ioread  ( FILE * );
int    _iowrite ( int, FILE * );
size_t _read    ( FILE *, char *, size_t);
size_t _write   ( FILE *, char *, size_t);
int    _fclose  ( FILE * );
int    _fopen   ( char *, FILE * );
FILE * _open    ( char *, char *, FILE * );
long   _lseek   ( FILE *, long, int );

/* Just leave these here */
int    _flsbuf  ( int, FILE * );
int    _filbuf  ( FILE * );
#ifdef __cplusplus
}
#endif /* __cplusplus */

#define getc(p)         (--(p)->_cnt>=0? (int)(*(p)->_ptr++&0377):_filbuf(p))
#define getchar()       getc(stdin)
#define putc(x,p)       (--(p)->_cnt>=0?                                \
                                ((int)(*(p)->_ptr++=((char)(x)))&0377): \
                                _flsbuf((unsigned char)(x),p))
#define putchar(x)      putc(x,stdout)
#define feof(p)         (((p)->_flag&_IOEOF)!=0)
#define ferror(p)       (((p)->_flag&_IOERR)!=0)
#define fileno(p)       p->_file
#define clearerr(p)     ((void)(p->_flag &= ~(_IOEOF|_IOERR)))

#endif  /* _STDIO_H */
