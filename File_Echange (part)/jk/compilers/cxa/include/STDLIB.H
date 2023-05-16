/*
 *      @(#)stdlib.h	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  stdlib.h                                               *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes of functions for          *
**                 numeric conversions, storage allocation and exit       *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef _STDLIB_H
#define _STDLIB_H

#include	<limits.h>

#ifndef _SIZE_T_DEF
typedef unsigned int    size_t;
#define _SIZE_T_DEF
#endif

#define NULL	0

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1
#define RAND_MAX        INT_MAX		/* in limits.h */

typedef struct  {
                        int quot;
                        int rem;
                } div_t;

typedef struct  {
                        long quot;
                        long rem;
                } ldiv_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
int           atoi   ( const char * );
long          atol   ( const char * );
double        atof   ( const char * );
long          strtol ( const char *, char * *, int );
unsigned long strtoul( const char *, char * *, int );
double        strtod ( const char *, char * * );
int           abs    ( int );
long          labs   ( long );
div_t         div    ( int, int );
ldiv_t        ldiv   ( long, long );
int           rand   ( void );
void          srand  ( unsigned int );

void *        calloc ( size_t, size_t );
void *        malloc ( size_t );
void *        realloc( void *, size_t );
void          free   ( void * );

void          exit   ( int );
void          abort  ( void );
int           atexit ( void (*)(void) );

char *        getenv ( const char * );
int	      system ( const char * );

void * bsearch( const void *, const void *, size_t, size_t,
		int (*)( const void *, const void * ) );
void   qsort  ( void *, size_t, size_t,
		int (*)( const void *, const void * ) );

/* wide characters not supported! Added for skeletons. */
#ifndef _WCHAR_T
typedef unsigned short wchar_t;
#define _WCHAR_T
#endif

#define MB_CUR_MAX      0

int     mblen( const char *, size_t );
int     mbtowc( wchar_t *, const char *, size_t );
int     wctomb( char *, wchar_t );
size_t  mbstowcs( wchar_t *, const char *, size_t );
size_t  wcstombs( char *, const wchar_t *, size_t );


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* _STDLIB_H */
