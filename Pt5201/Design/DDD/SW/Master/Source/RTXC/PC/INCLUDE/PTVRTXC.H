/* ptvrtxc.h - RTXC API for PTV library kernel services    "include libptv.lib" */

/*
 *   TK-PTV    Version 1.0
 *   Copyright (c) 1999.
 *   ProTeleVision Technologies A/S.
 *   ALL RIGHTS RESERVED
*/

#ifndef _PTVRTXC_H
#define _PTVRTXC_H

#include "rtxcopts.h"    /* RTXC options */

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#ifdef HAS_ALLOC /* { */
#ifdef HAS_INQMAP /* { */
 SEMA KSAPI KS_alloc_semafore(void);
#endif /* } HAS_INQMAP */
#endif /* } HAS_ALLOC */

#ifdef HAS_FREE /* { */
#ifdef HAS_INQMAP /* { */
 void KSAPI KS_free_semafore(SEMA sem);
#endif /* } HAS_INQMAP */
#endif /* } HAS_FREE */

#ifdef HAS_RESOURCES /* { */
#ifdef HAS_ALLOC /* { */
#ifdef HAS_INQMAP /* { */
 RESOURCE KSAPI KS_alloc_resource(void);
#endif /* } HAS_INQMAP */
#endif /* } HAS_ALLOC */
#endif /* } HAS_RESOURCES */

#ifdef HAS_RESOURCES /* { */
#ifdef HAS_FREE /* { */
#ifdef HAS_INQMAP /* { */
 void KSAPI KS_free_resource(RESOURCE res);
#endif /* } HAS_INQMAP */
#endif /* } HAS_FREE */
#endif /* } HAS_RESOURCES */


#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _RTXCOPTS_H */

/* End of file - ptvrtxc.h */
