/* $Id: xflash_intel_l.h,v 1.1.18.1 2005/09/26 22:20:20 trujillo Exp $ */
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
* @file xflash_intel_l.h
*
* Contains identifiers and low-level macros/functions for the Intel 28FxxxJ3A
* StrataFlash driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rpm  05/06/02 First release
* </pre>
*
******************************************************************************/

#ifndef XFLASH_INTEL_L_H /* prevent circular inclusions */
#define XFLASH_INTEL_L_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xio.h"

/************************** Constant Definitions *****************************/

/*
 * Commands written to the devices are defined by the CMD_* constants below.
 * Each Command contains 8-bits of significant data. For x16 or greater devices,
 * the command data should appear on the LSB. Other bytes may be written as
 * don't cares. To eliminate the need to know the bus layout, the width of
 * individual parts, or potential byte-swapping requirements, the CMD_*
 * constants are defined with the command data in every nibble.
 */

/*
 * BCS/SCS command codes
 */
#define XFL_INTEL_CMD_READ_ARRAY          0xFFFF
#define XFL_INTEL_CMD_READ_ID_CODES       0x9090
#define XFL_INTEL_CMD_READ_STATUS_REG     0x7070
#define XFL_INTEL_CMD_CLEAR_STATUS_REG    0x5050
#define XFL_INTEL_CMD_WRITE_BUFFER        0xE8E8
#define XFL_INTEL_CMD_PROGRAM             0x4040
#define XFL_INTEL_CMD_BLOCK_ERASE         0x2020
#define XFL_INTEL_CMD_CONFIRM             0xD0D0
#define XFL_INTEL_CMD_SUSPEND             0xB0B0
#define XFL_INTEL_CMD_RESUME              0xD0D0

#define XFL_INTEL_STATUS_READY            0x0080

/*
 * SCS command codes
 */
#define XFL_INTEL_CMD_READ_QUERY                0x9898
#define XFL_INTEL_CMD_CONFIG                    0xB8B8
#define XFL_INTEL_CMD_LOCK_BLOCK_SET            0x6060
#define XFL_INTEL_CMD_LOCK_BLOCK_SET_CONFIRM    0x0101
#define XFL_INTEL_CMD_LOCK_BLOCK_CLEAR          0x6060

/*
 * Other command codes
 */
#define XFL_INTEL_CMD_PROTECTION         0xC0C0

/*
 * Configuration command codes
 */
#define XFL_INTEL_CONFIG_RYBY_LEVEL        0x0000
#define XFL_INTEL_CONFIG_RYBY_PULSE_ERASE  0x0101
#define XFL_INTEL_CONFIG_RYBY_PULSE_WRITE  0x0202
#define XFL_INTEL_CONFIG_RYBY_PULSE_ALL    0x0303


/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/*****************************************************************************
*
* Low-level driver macros and functions. The list below provides signatures
* to help the user use the macros.
*
* void XFlashIntel_mSendCmd(Xuint32 BaseAddress, Xuint32 Offset,
*                           unsigned int Command)
* int XFlashIntel_WaitReady(Xuint32 BaseAddress, Xuint32 Offset);
*
* int XFlashIntel_WriteAddr(Xuint32 BaseAddress, Xuint32 Offset,
*                           Xuint8 *BufferPtr, unsigned int Length);
* int XFlashIntel_ReadAddr(Xuint32 BaseAddress, Xuint32 Offset,
*                          Xuint8 *BufferPtr, unsigned int Length);
* int XFlashIntel_EraseAddr(Xuint32 BaseAddress, Xuint32 Offset);
* int XFlashIntel_LockAddr(Xuint32 BaseAddress, Xuint32 Offset);
* int XFlashIntel_UnlockAddr(Xuint32 BaseAddress, Xuint32 Offset);
*
*****************************************************************************/

/****************************************************************************/
/**
*
* Send the specified command to the flash device.
*
* @param    BaseAddress is the base address of the device
* @param    Offset is the offset address from the base address.
* @param    Command is the command to send.
*
* @return   None.
*
* @note     None.
*
*****************************************************************************/
#define XFlashIntel_mSendCmd(BaseAddress, Offset, Command) \
                    XIo_Out16((BaseAddress) + (Offset), (Command))

/************************** Variable Definitions *****************************/


/************************** Function Prototypes ******************************/

int XFlashIntel_WaitReady(Xuint32 BaseAddress, Xuint32 Offset);
int XFlashIntel_WriteAddr(Xuint32 BaseAddress, Xuint32 Offset,
                          Xuint8 *BufferPtr, unsigned int Length);
int XFlashIntel_ReadAddr(Xuint32 BaseAddress, Xuint32 Offset,
                         Xuint8 *BufferPtr, unsigned int Length);
int XFlashIntel_EraseAddr(Xuint32 BaseAddress, Xuint32 Offset);
int XFlashIntel_LockAddr(Xuint32 BaseAddress, Xuint32 Offset);
int XFlashIntel_UnlockAddr(Xuint32 BaseAddress, Xuint32 Offset);

#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */
