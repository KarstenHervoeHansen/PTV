/* General declerations */
/* SAA 990325 */
/* added 'FAIL' to use as general purpose error indicator in return() */

#ifndef _DEFINE_H
#define _DEFINE_H

#define UC unsigned char
#define UI unsigned int
#define UL unsigned long


typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef void*		          HANDLE;


#define TRUE -1
#define FALSE 0

#define OK   -1
#define FAIL 0

#ifndef NULL
 #define NULL (void *)0
#endif

#ifndef NONE
 #define NONE (void *)0
#endif


typedef enum {
	true=-1,
	false=0
} bool;


#ifndef __BORLANDC__
 #define code _rom
 #define bit char
 #define far 
#else
 #define code 
 #define _rom
 #define bit char
 #define _bit char 
#endif


#endif