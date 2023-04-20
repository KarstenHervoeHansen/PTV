/*
 *	Version : @(#)locale.h	1.2
 */

/**************************************************************************
**                                                                        *
**  FILE        :  locale.h                                         	  *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes and macros for 		  *
**		   localization.					  *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef	_LOCALE_H
#define _LOCALE_H

#define	NULL	0

struct lconv {
		char *decimal_point;
		char *thousands_sep;
		char *grouping;
		char *int_curr_symbol;
		char *currency_symbol;
		char *mon_decimal_point;
		char *mon_thousands_sep;
		char *mon_grouping;
		char *positive_sign;
		char *negative_sign;
		char int_frac_digits;
		char frac_digits;
		char p_cs_precedes;
		char p_sep_by_space;
		char n_cs_precedes;
		char n_sep_by_space;
		char p_sign_posn;
		char n_sign_posn;
	};

#define LC_ALL		0
#define LC_COLLATE	1
#define LC_CTYPE	2
#define LC_NUMERIC	3
#define LC_TIME		4
#define LC_MONETARY	5

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
char *setlocale( int, const char *);

struct lconv *localeconv( void );
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* _LOCALE_H */
