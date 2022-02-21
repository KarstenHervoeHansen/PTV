/***************************************************************************/
/*	Copyright ProTeleVision Technologies A/S, BRONDBY 1999						*/
/*	Project:		PTV XA Controller Board													*/
/*	Module:		SYSTEM.H	 																	*/
/*	Author:		Kim Engedahl, DEV															*/
/*	Org. date:	990528																		*/
/*	Rev. date:	990902, KEn, DEV															*/
/*	Status:		Version 1.0																	*/
/*																									*/
/*	This module contains no functions													*/
/*																									*/
/*	Changes:																						*/
/*																									*/
/***************************************************************************/
#ifndef _SYSTEM_H
#define _SYSTEM_H

#define sbit static _bit

#define IV_T0					0x84

#define IV_EX1					0x88
#define IV_EX0					0x80

#define IV_RI0					0xA0
#define IV_TI0					0xA4
#define IV_RI1					0xA8
#define IV_TI1					0xAC

#define IV_SWI4				0x10C

#define IV_PSW					0x8F00	// System mode, high priority, bank 0

#define IV_SYSTEM				0x8000

#define IV_PRI00				0x0000	// priorities 0 - 15
#define IV_PRI01				0x0100
#define IV_PRI02				0x0200
#define IV_PRI03				0x0300
#define IV_PRI04				0x0400
#define IV_PRI05				0x0500
#define IV_PRI06				0x0600
#define IV_PRI07				0x0700
#define IV_PRI08				0x0800
#define IV_PRI09				0x0900
#define IV_PRI10				0x0A00
#define IV_PRI11				0x0B00
#define IV_PRI12				0x0C00
#define IV_PRI13				0x0D00
#define IV_PRI14				0x0E00
#define IV_PRI15				0x0F00

#define IV_BANK0				0x0000
#define IV_BANK1				0x1000
#define IV_BANK2				0x2000
#define IV_BANK3				0x3000


#define toupper(c)			((((char)c >= 'a') && ((char)c <= 'z')) ? (c &= ~('a' - 'A') & 0xFF) : c)
#define isalpha(c)			((((c) >= 'a') && ((c) <= 'z')) || (((c) >= 'A') && ((c) <= 'Z')))
#define isdigit(c)			(((c) >= '0') && ((c) <= '9'))
#define isxdigit(c)			((((c) >= 'a') && ((c) <= 'f')) || (((c) >= 'A') && ((c) <= 'F')) ||(((c) >= '0') && ((c) <= '9')))

#define iswhitespace(c)		(((c) != 0x0A) && ((c) <= 0x20))
#define isoctaldigit(c)		(((c) >= '0') && ((c) <= '7'))
#define isbinarydigit(c)	(((c) == '0') || ((c) == '1'))
#define iscolon(c)			((c) == ':')
#define isasterix(c)			((c) == '*')
#define issemicolon(c)		((c) == ';')
#define isquestionmark(c)	((c) == '?')
#define isunderscore(c)		((c) == '_')
#define isplussign(c)		((c) == '+')
#define isminussign(c)		((c) == '-')								  
#define isslash(c)			((c) == '/')
#define isdoubledagger(c)	((c) == '#')
#define isdot(c)				((c) == '.')
#define iscomma(c)			((c) == ',')
#define isquote(c)			((c) == '\'')
#define isdoublequote(c)	((c) == '\"')
#define isnewline(c)			((c) == 0x0A)		// Newline is Linefeed

#define HIGH(c)				( c / 0x100)
#define LOW(c)					( c % 0x100)

/***************************************************************************/
/* GLOBAL ENUMERATIONS																		*/
/***************************************************************************/
enum boolean	 		 	   { FALSE = 0, TRUE };
enum Logics						{ LOW = 0, HIGH };
enum OffOnModes			   { OFF = 0, ON };
enum MinMax						{ MIN = 0, MAX };
enum directions				{ STAY = 0, UP, DOWN };

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;

#endif
