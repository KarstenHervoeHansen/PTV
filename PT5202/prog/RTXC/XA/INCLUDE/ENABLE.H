/* enable.h - interrupt enable/disable macros */

/*
 *   RTXC    
 *   Copyright (c)
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _ENABLE_H /* { */
#define _ENABLE_H

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

extern void ks_enable(void);

#ifdef INLINE_INTS /* { INLINE_INTS */
#define ENABLE   ks_enable()
#define DISABLE  PSWH = (unsigned char)(SYSMODE+RTXCINTLVL)
#define POPPSWH  PSWH = pswh
#define PUSHPSWH pswh = PSWH
#define POPNULL

#else /* } INLINE_INTS { */

extern void ks_disable(void);
extern void poppswh(unsigned char);
extern void pushpswh(unsigned char *);

#define ENABLE  ks_enable()
#define DISABLE ks_disable()
#define POPPSWH  poppswh(pswh)
#define PUSHPSWH pushpswh(&pswh)
#define POPNULL

#endif /* } INLINE_INTS */

#ifdef __cplusplus /* { */
 }
#endif /* } __cplusplus */

#endif /* } _ENABLE_H */
