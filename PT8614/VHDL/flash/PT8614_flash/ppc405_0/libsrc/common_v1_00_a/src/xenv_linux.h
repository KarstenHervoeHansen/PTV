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
* @file xenv_linux.h
*
* Defines common services specified by xenv.h. Some of these services are
* defined as not performing any action. The implementation of these services
* are left to the user.
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
* 1.00a ch   10/24/02 First release
* 1.00a xd   11/03/04 Improved support for doxygen.
* </pre>
*
*
******************************************************************************/

#ifndef XENV_LINUX_H /* prevent circular inclusions */
#define XENV_LINUX_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif


/***************************** Include Files *********************************/
#include "sleep.h"

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
    char *Dest = (char*)(DestPtr);                                \
    char *Src  = (char*)(SrcPtr);                                 \
    unsigned BytesLeft = (Bytes);                                 \
                                                                  \
    while (BytesLeft--) *Dest++ = *Src++;                         \
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
    char *Dest = (char*)(DestPtr);                                \
    char c = (Data);                                              \
    unsigned BytesLeft = (Bytes);                                 \
                                                                  \
    while (BytesLeft--) *Dest++ = c;                              \
}

/**
 * A structure that contains a time stamp used by other time stamp macros
 * defined below. This structure is processor dependent.
 */
typedef int XENV_TIME_STAMP;

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
 * <br><br>
 * This macro must be implemented by the user.
 *
 *****************************************************************************/
#define XENV_TIME_STAMP_GET(StampPtr)

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
 * This macro must be implemented by the user.
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
 * This macro must be implemented by the user
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
 * @note    None.
 *
 *****************************************************************************/
#define XENV_USLEEP(delay)                                        \
{                                                                 \
    usleep(delay);                                                \
}


#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */

