/* $Id: xopbarb_l.h,v 1.1.18.1 2005/09/22 16:06:07 trujillo Exp $ */
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
* @file xopbarb_l.h
*
* This header file contains identifiers and low-level driver functions (or
* macros) that can be used to access the device.  The user should refer to the
* hardware device specification for more details of the device operation.
* High-level driver functions are defined in xopbarb.h.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00b jhl  04/24/02 First release
* </pre>
*
******************************************************************************/

#ifndef XOPBARB_L_H /* prevent circular inclusions */
#define XOPBARB_L_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xio.h"

/************************** Constant Definitions *****************************/

/*
 * Register offsets for the OPB Arbiter
 */
#define XOA_CR_OFFSET     0x100   /* (CR) Control Register */
#define XOA_LVLX_OFFSET   0x104   /* Start of priority level registers */
#define XOA_LVLX_SIZE     4       /* Size of priority level registers */

/*
 * OPB Arbiter Control Register (CR) masks
 */
#define XOA_CR_DYNAMIC_ENABLE_MASK  0x80000000   /* Dynamic priority enable */
#define XOA_CR_DYNAMIC_RW_MASK      0x40000000   /* Dynamic priority enable
                                                  * read/write */
#define XOA_CR_PARK_ENABLE_MASK     0x20000000   /* Park enable */
#define XOA_CR_PARK_RW_MASK         0x10000000   /* Park enable read/write */
#define XOA_CR_PARK_ON_ID_MASK      0x08000000   /* Park on park ID */
#define XOA_CR_PRIORITY_VALID_MASK  0x04000000   /* Priority registers valid */
#define XOA_CR_PARK_ID_MASK         0x0000000F   /* Park ID */


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/*****************************************************************************
*
* Low-level driver macros.  The list below provides signatures to help the
* user use the macros.
*
* void XOpbArb_mSetControlReg(Xuint32 BaseAddress, Xuint32 RegisterValue)
* Xuint32 XOpbArb_mGetControlReg(Xuint32 BaseAddress)
*
* void XOpbArb_mEnableDynamic(Xuint32 BaseAddress)
* void XOpbArb_mDisableDynamic(Xuint32 BaseAddress)
*
* void XOpbArb_mEnableParking(Xuint32 BaseAddress)
* void XOpbArb_mDisableParking(Xuint32 BaseAddress)
*
* void XOpbArb_mSetParkMasterNot(Xuint32 BaseAddress)
* void XOpbArb_mClearParkOnMaster(Xuint32 BaseAddress)
*
* void XOpbArb_mSetPriorityRegsValid(Xuint32 BaseAddress)
* void XOpbArb_mClearPriorityRegsValid(Xuint32 BaseAddress)
*
* void XOpbArb_mSetParkedMasterId(Xuint32 BaseAddress, Xuint8 ParkedMasterId)
* void XOpbArb_mSetPriorityReg(Xuint32 BaseAddress, Xuint8 Level,
*                              Xuint32 MasterId)
* Xuint32 XOpbArb_mGetPriorityReg(Xuint32 BaseAddress, Xuint8 Level)
*
*****************************************************************************/

/****************************************************************************/
/**
* Set the Control Register of the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
* @param    RegisterValue contains the value to be written to the register.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mSetControlReg(BaseAddress, RegisterValue) \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET, (RegisterValue))

/*****************************************************************************/
/**
* Get the Control Register of the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   The value read from the register.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mGetControlReg(BaseAddress) \
    XIo_In32((BaseAddress) + XOA_CR_OFFSET)

/*****************************************************************************/
/**
* Enable dynamic priority arbitration in the Control Register in the OPB
* Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mEnableDynamic(BaseAddress)                     \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET,                    \
              (XIo_In32((BaseAddress) + XOA_CR_OFFSET) |        \
               XOA_CR_DYNAMIC_ENABLE_MASK))

/*(***************************************************************************/
/**
* Disable dynamic priority arbitration in the Control Register in the OPB
* Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mDisableDynamic(BaseAddress)                    \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET,                    \
              (XIo_In32((BaseAddress) + XOA_CR_OFFSET) &        \
               ~XOA_CR_DYNAMIC_ENABLE_MASK))

/*****************************************************************************/
/**
* Enable parking in the Control Register in the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mEnableParking(BaseAddress)                     \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET,                    \
              (XIo_In32((BaseAddress) + XOA_CR_OFFSET) |        \
               XOA_CR_PARK_ENABLE_MASK))

/*****************************************************************************/
/**
* Disable parking in the Control Register in the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mDisableParking(BaseAddress)                    \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET,                    \
              (XIo_In32((BaseAddress) + XOA_CR_OFFSET) &        \
               ~XOA_CR_PARK_ENABLE_MASK))

/****************************************************************************/
/**
* Set park on master not last (park on a specific master ID) in the Control
* Register in the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mSetParkMasterNot(BaseAddress)                  \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET,                    \
              (XIo_In32((BaseAddress) + XOA_CR_OFFSET) |        \
               XOA_CR_PARK_ON_ID_MASK))

/*****************************************************************************/
/**
* Clear park on master not last (park on a specific master ID) in the OPB
* Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mClearParkMasterNot(BaseAddress)                \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET,                    \
              (XIo_In32((BaseAddress) + XOA_CR_OFFSET) &        \
               ~XOA_CR_PARK_ON_ID_MASK))

/*****************************************************************************/
/**
* Set the priority registers valid in the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mSetPriorityRegsValid(BaseAddress)              \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET,                    \
              (XIo_In32((BaseAddress) + XOA_CR_OFFSET) |        \
               XOA_CR_PRIORITY_VALID_MASK))

/*****************************************************************************/
/**
* Clear the priority registers valid in the Control Register of the OPB
* Arbiter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mClearPriorityRegsValid(BaseAddress)            \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET,                    \
              (XIo_In32((BaseAddress) + XOA_CR_OFFSET) &        \
               ~XOA_CR_PRIORITY_VALID_MASK))

/*****************************************************************************/
/**
* Set the parked master ID in the Control Register in the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
* @param    ParkedMasterId contains the ID of the master to park on (0 - 15).
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mSetParkedMasterId(BaseAddress, ParkedMasterId) \
{                                                               \
    Xuint32 ControlReg;                                         \
                                                                \
    ControlReg = XIo_In32((BaseAddress) + XOA_CR_OFFSET) &      \
                 ~XOA_CR_PARK_ID_MASK;                          \
    XIo_Out32((BaseAddress) + XOA_CR_OFFSET, ControlReg |       \
              (Xuint8)ParkedMasterId);                          \
}

/*****************************************************************************/
/**
* Set the priority register in the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
* @param    Level contain the priority level of the register to set (0 - 15).
* @param    MasterId contains the value to be written to the register (0 - 15).
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mSetPriorityReg(BaseAddress, Level, MasterId)               \
    XIo_Out32((BaseAddress) + XOA_LVLX_OFFSET + (Level * XOA_LVLX_SIZE),    \
              MasterId)

/*****************************************************************************/
/**
* Get the priority register in the OPB Arbiter.
*
* @param    BaseAddress contains the base address of the device.
* @param    Level contain the priority level of the register to get (0 - 15).
*
* @return   The contents of the specified priority register, a master ID
*           (0 - 15).
*
* @note     None.
*
******************************************************************************/
#define XOpbArb_mGetPriorityReg(BaseAddress, Level)                         \
    XIo_In32((BaseAddress) + XOA_LVLX_OFFSET + (Level * XOA_LVLX_SIZE))

/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/

#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */

