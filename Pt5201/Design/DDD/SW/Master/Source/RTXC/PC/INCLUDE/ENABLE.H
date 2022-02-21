/* enable.h - interrupt enable/disable macros */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _ENABLE_H /* { */
#define _ENABLE_H

#include "rtxcopts.h"

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#define FLAG_STORAGE  unsigned short flags;               /* flag variable */
#define SAVE_FLAGS    __asm pushf;      __asm pop flags   /* save flags */
#define RESTORE_FLAGS __asm push flags; __asm popf        /* restore flags */

#ifdef INLINE_INTS /* { */
#define ENABLE        __asm sti
#define DISABLE       __asm cli; __asm nop
#define NOP           __asm nop
#else /* } INLINE_INTS { */
/* used primarily for lint and in other benchmark/qualification phases */
#define ENABLE  ut_enable()
#define DISABLE ut_disable()
#define NOP     ut_nop()
extern void ut_enable(void);
extern void ut_disable(void);
extern void ut_nop(void);
#endif /* } INLINE_INTS */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _ENABLE_H */

/* End of file - enable.h */
