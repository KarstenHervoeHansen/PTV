//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2002 Xilinx, Inc.  All rights reserved. 
// Xilinx, Inc. 
//
// XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
// COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS 
// ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
// STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
// IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
// FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION. 
// XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
// THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
// ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
// FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
// AND FITNESS FOR A PARTICULAR PURPOSE. 
//
// $Header: /devl/xcs/repo/env/Jobs/MDT/sw/bsp/standalone/v1_00_a/src/profile/profile.h,v 1.3 2005/07/12 21:13:00 rajn Exp $
// $Id: profile.h,v 1.3 2005/07/12 21:13:00 rajn Exp $
//
//////////////////////////////////////////////////////////////////////

#ifndef	_PROFILE_H
#define	_PROFILE_H	1

#include <stdio.h>
#include "mblaze_nt_types.h"
#include "profile_config.h"

#ifdef __cplusplus
extern "C" {
#endif

void _system_init( void ) ;
void _system_clean( void ) ;
void mcount(unsigned long frompc, unsigned long selfpc);
void profile_intr_handler( void ) ;



/****************************************************************************
 * Profiling on hardware - Hash table maintained on hardware and data sent
 * to xmd for gmon.out generation.
 ****************************************************************************/
/*
 * histogram counters are unsigned shorts (according to the kernel).
 */
#define	HISTCOUNTER	unsigned short

struct tostruct {
	unsigned long  selfpc;
	long	       count;
	short 	       link;
	unsigned short pad;
};

struct fromstruct {
	unsigned long frompc ;
	short link ;
	unsigned short pad ;
} ;

/*
 * general rounding functions.
 */
#define ROUNDDOWN(x,y)	(((x)/(y))*(y))
#define ROUNDUP(x,y)	((((x)+(y)-1)/(y))*(y))

/*
 * The profiling data structures are housed in this structure.
 */
struct gmonparam {
	long int		state;

	// Histogram Information
	unsigned short		*kcount;	/* No. of bins in histogram */
	unsigned long		kcountsize;	/* Histogram samples */

	// Call-graph Information
	struct fromstruct	*froms;
	unsigned long		fromssize;
	struct tostruct		*tos;
	unsigned long		tossize;

	// Initialization I/Ps
	unsigned long    	lowpc;
	unsigned long		highpc;
	unsigned long		textsize;
	//unsigned long 		cg_froms;
	//unsigned long 		cg_tos;
};
extern struct gmonparam *_gmonparam;
extern int n_gmon_sections;

/*
 * Possible states of profiling.
 */
#define	GMON_PROF_ON	0
#define	GMON_PROF_BUSY	1
#define	GMON_PROF_ERROR	2
#define	GMON_PROF_OFF	3

/*
 * Sysctl definitions for extracting profiling information from the kernel.
 */
#define	GPROF_STATE	0	/* int: profiling enabling variable */
#define	GPROF_COUNT	1	/* struct: profile tick count buffer */
#define	GPROF_FROMS	2	/* struct: from location hash bucket */
#define	GPROF_TOS	3	/* struct: destination/count structure */
#define	GPROF_GMONPARAM	4	/* struct: profiling parameters (see above) */

#ifdef __cplusplus
}
#endif

#endif 		/* _PROFILE_H */












