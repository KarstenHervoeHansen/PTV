/******************************************************************************
*
*       XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
*       AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND
*       SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,
*       OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,
*       APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION
*       THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
*       AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
*       FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
*       WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
*       IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
*       REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
*       INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*       FOR A PARTICULAR PURPOSE.
*
*       (c) Copyright 2002 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xenv_vxworks.h
*
* Defines common services that are typically found in a VxWorks target
* environment.
*
* @note
*
* This file is not intended to be included directly by driver code. Instead,
* the generic xenv.h file is intended to be included by driver code.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a xd   11/03/04 Improved support for doxygen.
*       rmm  09/13/03 CR 177068: Fix compiler warning in XENV_MEM_FILL
*       rmm  10/24/02 Added XENV_USLEEP macro
* 1.00a rmm  07/16/01 First release
* </pre>
*
*
******************************************************************************/

#ifndef XENV_VXWORKS_H /* prevent circular inclusions */
#define XENV_VXWORKS_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#include "xbasic_types.h"
#include "vxWorks.h"
#include "vxLib.h"
#include <string.h>

/*****************************************************************************/
/**
 *
 * Copies a non-overlapping block of memory.
 *
 * @param   DestPtr is the destination address to copy data to.
 * @param   SrcPtr is the source address to copy data from.
 * @param   Bytes is the number of bytes to copy.
 *
 * @return  None.
 *
 * @note
 *
 * Signature: void XENV_MEM_COPY(void *DestPtr, void *SrcPtr, unsigned Bytes)
 *
 *****************************************************************************/
#define XENV_MEM_COPY(DestPtr, SrcPtr, Bytes)                     \
{                                                                 \
    void *Dest = (DestPtr);                                       \
    void *Src  = (SrcPtr);                                        \
    unsigned LengthBytes = (Bytes);                               \
                                                                  \
    memcpy(Dest, Src, (size_t)LengthBytes);                       \
}

/*****************************************************************************/
/**
 *
 * Fills an area of memory with constant data.
 *
 * @param   DestPtr is the destination address to set.
 * @param   Data contains the value to set.
 * @param   Bytes is the number of bytes to set.
 *
 * @return  None.
 *
 * @note
 *
 * Signature: void XENV_MEM_FILL(void *DestPtr, char Data, unsigned Bytes)
 *
 *****************************************************************************/
#define XENV_MEM_FILL(DestPtr, Data, Bytes)                       \
{                                                                 \
    void *Dest = (DestPtr);                                       \
    unsigned LengthBytes = (Bytes);                               \
                                                                  \
    memset(Dest, (int)Data, (size_t)LengthBytes);                 \
}

#if (CPU_FAMILY==PPC)
/**
 * A structure that contains a time stamp used by other time stamp macros
 * defined below. This structure is processor dependent.
 */
typedef struct
{
    Xuint32 TimeBaseUpper;
    Xuint32 TimeBaseLower;
} XENV_TIME_STAMP;
#endif

/*****************************************************************************/
/**
 *
 * Time is derived from the 64 bit PPC timebase register
 *
 * @param   StampPtr is the storage for the retrieved time stamp.
 *
 * @return  None.
 *
 * @note
 *
 * Signature: void XENV_TIME_STAMP_GET(XTIME_STAMP *StampPtr)
 *
 *****************************************************************************/
#define XENV_TIME_STAMP_GET(StampPtr)                   \
{                                                       \
    vxTimeBaseGet((UINT32*)&(StampPtr)->TimeBaseUpper,  \
                  (UINT32*)&(StampPtr)->TimeBaseLower); \
}

/*****************************************************************************/
/**
 *
 * This macro is not yet implemented and always returns 0.
 *
 * @param   Stamp1Ptr is the first sampled time stamp.
 * @param   Stamp2Ptr is the second sampled time stamp.
 *
 * @return  0
 *
 * @note    None.
 *
 *****************************************************************************/
#define XENV_TIME_STAMP_DELTA_US(Stamp1Ptr, Stamp2Ptr)     (0)

/*****************************************************************************/
/**
 *
 * This macro is not yet implemented and always returns 0.
 *
 * @param   Stamp1Ptr is the first sampled time stamp.
 * @param   Stamp2Ptr is the second sampled time stamp.
 *
 * @return  0
 *
 * @note
 *
 * None.
 *
 *****************************************************************************/
#define XENV_TIME_STAMP_DELTA_MS(Stamp1Ptr, Stamp2Ptr)     (0)

/*****************************************************************************/
/**
 *
 * Delay the specified number of microseconds.
 *
 * @param   delay is the number of microseconds to delay.
 *
 * @return  None.
 *
 *****************************************************************************/
#define XENV_USLEEP(delay)  sysUsDelay(delay)


#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */

