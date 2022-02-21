/* $Id: xplbarb_l.h,v 1.4.10.1 2005/09/22 15:08:16 trujillo Exp $ */
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
* @file xplbarb_l.h
*
* This file contains internal identifiers and low-level macros that can be
* used to access the device directly.  See xplbarb.h for a description of
* the high-level driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.01a rpm  05/10/02 First release
* </pre>
*
******************************************************************************/

#ifndef XPLBARB_L_H /* prevent circular inclusions */
#define XPLBARB_L_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xio.h"
#include "xio_dcr.h"    /* DCR only, no choice */

/************************** Constant Definitions *****************************/

/* PLB Arbiter Register offsets - DCR bus */

#define XPA_PESR_MERR_OFFSET         0x00 /* Master error register */

#define XPA_PESR_MDRIVE_OFFSET       0x01 /* Master driving error status */
#define XPA_PESR_READ_OFFSET         0x02 /* set if read error,
                                           * clear if write error */
#define XPA_PESR_LCK_ERR_OFFSET      0x03 /* error status locked if set */
#define XPA_PEAR_ADDR_OFFSET         0x04 /* addesss of error */
#define XPA_PEAR_BYTE_EN_OFFSET      0x05 /* byte lane(s) where error occurred */
#define XPA_PEAR_SIZE_TYPE_OFFSET    0x06 /* size/type of error that occurred */
#define XPA_PACR_OFFSET              0x07 /* control register */

/* PACR Register masks */

#define XPA_PACR_ENABLE_INTR_MASK    0x80000000 /* set to enable interrupts */
#define XPA_PACR_SOFTWARE_RESET_MASK 0x40000000 /* set to force reset, */
#define XPA_PACR_TEST_ENABLE_MASK    0x20000000 /* set to allow testing,
                                                 * clear otherwise */

/* PEAR Size Register masks */

#define XPA_PEAR_SIZE_MASK           0x000000F0 /* size of access that
                                                 * caused error */
#define XPA_PEAR_TYPE_MASK           0x0000000E /* type of access that
                                                 * caused error */

/* PLB Arbiter Register Masks */

#define XPA_DRIVING_BEAR_MASK       0x80000000 /* timeout error occurred */
#define XPA_ERROR_READ_MASK         0x40000000 /* set if read error,
                                                * clear if write error */
#define XPA_ERROR_STATUS_LOCK_MASK  0x20000000 /* status is locked in register */

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/* Define the appropriate I/O access method for the arbiter currently only
 * DCR
 */
#define XPlbArb_In32   XIo_DcrIn
#define XPlbArb_Out32  XIo_DcrOut

/*****************************************************************************
*
* Low-level driver macros and functions. The list below provides signatures
* to help the user use the macros.
*
* Xuint32 XPlbArb_mGetPesrMerrReg(Xuint32 BaseAddress)
* void XPlbArb_mSetPesrMerrReg(Xuint32 BaseAddress, Mask)
*
* Xuint32 XPlbArb_mGetPesrMDriveReg(Xuint32 BaseAddress)
* Xuint32 XPlbArb_mGetPesrRnwReg(Xuint32 BaseAddress)
* Xuint32 XPlbArb_mGetPesrLockReg(Xuint32 BaseAddress)
*
* Xuint32 XPlbArb_mGetPearAddrReg(Xuint32 BaseAddress)
* Xuint32 XPlbArb_mGetPearByteEnReg(Xuint32 BaseAddress)
* Xuint32 XPlbArb_mGetControlReg(Xuint32 BaseAddress)
*
* void XPlbArb_mEnableInterrupt(Xuint32 BaseAddress)
* void XPlbArb_mDisableInterrupt(Xuint32 BaseAddress)
* void XPlbArb_mReset(Xuint32 BaseAddress)
*
*****************************************************************************/

/****************************************************************************/
/**
*
* Get the error status register.
*
* @param    BaseAddress is the base address of the device
*
* @return   The 32-bit value of the error status register.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mGetPesrMerrReg(BaseAddress) \
                    XPlbArb_In32((BaseAddress) + XPA_PESR_MERR_OFFSET)


/****************************************************************************/
/**
*
* Set the error status register.
*
* @param    BaseAddress is the base address of the device
* @param    Mask is the 32-bit value to write to the error status register.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mSetPesrMerrReg(BaseAddress, Mask) \
                    XPlbArb_Out32((BaseAddress) + XPA_PESR_MERR_OFFSET, (Mask))


/****************************************************************************/
/**
*
* Get the master driving the error, if any.
*
* @param    BaseAddress is the base address of the device
*
* @return   The 32-bit value of the PESR Master driving error register.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mGetPesrMDriveReg(BaseAddress) \
                    XPlbArb_In32((BaseAddress) + XPA_PESR_MDRIVE_OFFSET)


/****************************************************************************/
/**
*
* Get the value of the Read-Not-Write register, which indicates whether the
* error is a read error or write error.
*
* @param    BaseAddress is the base address of the device
*
* @return   The 32-bit value of the PESR RNW error register.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mGetPesrRnwReg(BaseAddress) \
                    XPlbArb_In32((BaseAddress) + XPA_PESR_READ_OFFSET)


/****************************************************************************/
/**
*
* Get the value of the lock bit register, which indicates whether the master
* has locked the error registers.
*
* @param    BaseAddress is the base address of the device
*
* @return   The 32-bit value of the PESR Lock error register.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mGetPesrLockReg(BaseAddress) \
                    XPlbArb_In32((BaseAddress) + XPA_PESR_LCK_ERR_OFFSET)


/****************************************************************************/
/**
*
* Get the erorr address (or PEAR), which is the address that just caused the
* error.
*
* @param    BaseAddress is the base address of the device
*
* @return   The 32-bit error address.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mGetPearAddrReg(BaseAddress) \
                    XPlbArb_In32((BaseAddress) + XPA_PEAR_ADDR_OFFSET)


/****************************************************************************/
/**
*
* Get the erorr address byte enable register.
*
* @param    BaseAddress is the base address of the device
*
* @return   The 32-bit error address byte enable register contents.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mGetPearByteEnReg(BaseAddress) \
                    XPlbArb_In32((BaseAddress) + XPA_PEAR_BYTE_EN_OFFSET)


/****************************************************************************/
/**
*
* Get the contents of the control register.
*
* @param    BaseAddress is the base address of the device
*
* @return   The 32-bit control register contents.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mGetControlReg(BaseAddress) \
                    XPlbArb_In32((BaseAddress) + XPA_PACR_OFFSET)

/****************************************************************************/
/**
*
* Enable interrupts in the bridge. Preserve the contents of the ctrl register.
*
* @param    BaseAddress is the base address of the device
*
* @return   None.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mEnableInterrupt(BaseAddress) \
             XPlbArb_Out32((BaseAddress) + XPA_PACR_OFFSET, \
                XPlbArb_In32((BaseAddress) + XPA_PACR_OFFSET) | \
                    XPA_PACR_ENABLE_INTR_MASK)


/****************************************************************************/
/**
*
* Disable interrupts in the bridge. Preserve the contents of the ctrl register.
*
* @param    BaseAddress is the base address of the device
*
* @return   None.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mDisableInterrupt(BaseAddress) \
             XPlbArb_Out32((BaseAddress) + XPA_PACR_OFFSET, \
                XPlbArb_In32((BaseAddress) + XPA_PACR_OFFSET) & \
                    ~XPA_PACR_ENABLE_INTR_MASK)


/****************************************************************************/
/**
*
* Reset the bridge. Preserve the contents of the control register.
*
* @param    BaseAddress is the base address of the device
*
* @return   None.
*
* @note     None.
*
*****************************************************************************/
#define XPlbArb_mReset(BaseAddress) \
             XPlbArb_Out32((BaseAddress) + XPA_PACR_OFFSET, \
                XPlbArb_In32((BaseAddress) + XPA_PACR_OFFSET) | \
                    XPA_PACR_SOFTWARE_RESET_MASK)


/************************** Function Prototypes ******************************/

#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */
