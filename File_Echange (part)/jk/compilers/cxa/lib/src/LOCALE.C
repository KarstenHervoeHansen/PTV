/*
 *	@(#)locale.c	1.3
 */

/**************************************************************************
**                                                                        *
**  FILE        :  locale.c                                               *
**                                                                        *
**  DESCRIPTION :  Skeleton functions for localization.			  *
**                                                                        *
**  COPYRIGHT   :  1996 Copyright TASKING, Inc.                           *
**                                                                        *
**************************************************************************/

#include <limits.h>
#include <locale.h>
#include <string.h>

char *setlocale( int category, const char *locale )
{
	category;
	locale;
	return ("C");
}

struct lconv *localeconv( void )
{
	static struct lconv lc;

	lc.decimal_point = ".";
	lc.thousands_sep = "";
	lc.grouping = "";
	lc.int_curr_symbol = "";
	lc.currency_symbol = "";
	lc.mon_decimal_point = "";
	lc.mon_thousands_sep = "";
	lc.mon_grouping = "";
	lc.positive_sign = "";
	lc.negative_sign = "";
	lc.int_frac_digits = CHAR_MAX;
	lc.frac_digits = CHAR_MAX;
	lc.p_cs_precedes = CHAR_MAX;
	lc.p_sep_by_space = CHAR_MAX;
	lc.n_cs_precedes = CHAR_MAX;
	lc.n_sep_by_space = CHAR_MAX;
	lc.p_sign_posn = CHAR_MAX;
	lc.n_sign_posn = CHAR_MAX;
	return(&lc);
}

int strcoll( const char *s1, const char *s2 )
{
	return (strcmp(s1, s2));
}

size_t strxfrm( char *s1, const char *s2, size_t n )
{
	if (strlen(s2) < n)
		strcpy(s1, s2);
	return (strlen(s2));
}

