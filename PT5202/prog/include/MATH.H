/*
 *	@(#)math.h	1.8
 */

/**************************************************************************
**                                                                        *
**  FILE        :  math.h                                                 *
**                                                                        *
**  DESCRIPTION :  Include file with prototypes for mathematical          *
**                 functions                                              *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef	_MATH_H
#define	_MATH_H

#ifdef	_SINGLE_FP
#    define	HUGE_VAL	(3.40282347E+38F)	  /* FLT_MAX in 'float.h' */
#else
#    define	HUGE_VAL	(1.7976931348623157E+308) /* DBL_MAX in 'float.h' */
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
double	sin	( double );
double	cos	( double );
double	tan	( double );
double	asin	( double );
double	acos	( double );
double	atan	( double );
double	atan2	( double, double );
double	sinh	( double );
double	cosh	( double );
double	tanh	( double );
double	exp	( double );
double	log	( double );
double	log10	( double );
double	pow	( double, double );
double	sqrt	( double );
double	ceil	( double );
double	floor	( double );
double	fabs	( double );
double	ldexp	( double, int );
double	frexp	( double, int * );
double	modf	( double, double * );
double	fmod	( double, double );
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* _MATH_H */
