/* $Id: xplbarb.c,v 1.5 2004/07/19 17:29:05 moleres Exp $ */
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
* @file xplbarb.c
*
* Contains required functions for the XPlbArb component. See xplbarb.h for
* more information.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- ----------------------------------------------------
* 1.00a ecm  12/7/01  First release
* 1.01a rpm  05/13/02 Updated to match hw version, publicized LookupConfig
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xstatus.h"
#include "xparameters.h"

#include "xplbarb.h"
#include "xplbarb_i.h"

#include "xio.h"
#include "xio_dcr.h"    /* DCR only, no choice */

/************************** Constant Definitions ****************************/

#define PEAR_SHIFT_VALUE    25
#define PEAR_SHIFT_MASK     0x000000FF

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/
/*
 * Array for master bit positions
 */

static Xuint32 MasterBitPos[32] =
{
    0x80000000,
    0x40000000,
    0x20000000,
    0x10000000,
    0x08000000,
    0x04000000,
    0x02000000,
    0x01000000,
    0x00800000,
    0x00400000,
    0x00200000,
    0x00100000,
    0x00080000,
    0x00040000,
    0x00020000,
    0x00010000,
    0x00008000,
    0x00004000,
    0x00002000,
    0x00001000,
    0x00000800,
    0x00000400,
    0x00000200,
    0x00000100,
    0x00000080,
    0x00000040,
    0x00000020,
    0x00000010,
    0x00000008,
    0x00000004,
    0x00000002,
    0x00000001
};


/************************** Function Prototypes *****************************/


/****************************************************************************/
/**
*
* Initializes a specific XPlbArb instance. Looks up the configuration for
* the given device instance and initialize the instance structure.
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
* @param    DeviceId is the unique id of the device controlled by this XPlbArb
*           component.
*
* @return
*
* - XST_SUCCESS if everything starts up as expected.
* - XST_DEVICE_NOT_FOUND if the requested device is not found
*
* @note
*
* None.
*
*****************************************************************************/
XStatus XPlbArb_Initialize(XPlbArb *InstancePtr, Xuint16 DeviceId)
{
    XPlbArb_Config *BusConfigPtr;

    /*
     * Assert validates the input arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);

    /*
     * Lookup the device configuration in the temporary CROM table. Use this
     * configuration info down below when initializing this component.
     */
    BusConfigPtr = XPlbArb_LookupConfig(DeviceId);

    if (BusConfigPtr == (XPlbArb_Config *)XNULL)
    {
        return XST_DEVICE_NOT_FOUND;
    }

    /*
     * Set some default values.
     */
    InstancePtr->BaseAddress = BusConfigPtr->BaseAddress;
    InstancePtr->NumMasters = BusConfigPtr->NumMasters;

    /*
     *  clear all errors, write of 1 to each status bit clears it
     */
    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PESR_MERR_OFFSET, 0xFFFFFFFF);

    /*
     * Indicate the instance is now ready to use, initialized without error
     */
    InstancePtr->IsReady = XCOMPONENT_IS_READY;

    return XST_SUCCESS;
}


/*****************************************************************************/
/**
*
* Returns XTRUE is there is an error outstanding
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
*
* @return
*
* Boolean XTRUE if there is an error, XFALSE if there is no current error.
*
* @note
*
* None.
*
******************************************************************************/
Xboolean XPlbArb_IsError(XPlbArb *InstancePtr)
{
    Xuint32 RegisterContents;

    /*
     * Assert validates the input arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    RegisterContents =
            XPlbArb_In32(InstancePtr->BaseAddress + XPA_PESR_MERR_OFFSET);

    return (RegisterContents != 0UL);
}


/*****************************************************************************/
/**
*
* Clears the Errors for the specified master
*.
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
* @param    Master of which the indicated error is to be cleared, valid range
*           is 0 - the number of masters - 1 on the bus
*
* @return
*
* None.
*
* @note
*
* None.
*
******************************************************************************/
void XPlbArb_ClearErrors(XPlbArb *InstancePtr, Xuint8 Master)
{

    Xuint32 RegisterContents;

    /*
     * Assert validates the input arguments
     */
    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);
    XASSERT_VOID(Master < InstancePtr->NumMasters);

    /*
     * read error register
     */
    RegisterContents =
        XPlbArb_In32(InstancePtr->BaseAddress + XPA_PESR_MERR_OFFSET);


    /*
     * mask off the bits specific to the requested master
     */
    RegisterContents &= MasterBitPos[Master];

    /*
     * Clear the specific master's errors
     */
    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PESR_MERR_OFFSET,
              RegisterContents);
}


/*****************************************************************************/
/**
*
* Returns the Error status for the specified master. These are bit masks.
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
* @param    Master of which the indicated error is to be cleared, valid range
*           is 0 - the number of masters on the bus
*
* @return
*
* The current error status for the requested master on the PLB. The status is
* a bit-mask that can contain the following bit values:
* <pre>
*   XPA_DRIVING_BEAR_MASK           Indicates that an error has occurred and
*                                   this master is driving the error address
*   XPA_ERROR_READ_MASK             Indicates the error was a read error (it
*                                   is a write error otherwise).
*   XPA_ERROR_STATUS_LOCK_MASK      Indicates the error status and address
*                                   are locked and cannot be overwritten.
*   XPA_PEAR_SIZE_MASK              Size of access that caused error
*   XPA_PEAR_TYPE_MASK              Type of access that caused error
* </pre>
*
* @note
*
* None.
*
******************************************************************************/
Xuint32 XPlbArb_GetErrorStatus(XPlbArb *InstancePtr, Xuint8 Master)
{
    Xuint32 RegisterContents;
    Xuint32 ReturnStatus = 0UL;
    Xuint32 MasterMask;

    /*
     * Assert validates the input arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);
    XASSERT_NONVOID(Master < InstancePtr->NumMasters);

    /*
     * select the proper bit position
     */
    MasterMask = MasterBitPos[Master];


    /*
     * read each register and build up the return status
     */
    RegisterContents =
        XPlbArb_In32(InstancePtr->BaseAddress + XPA_PESR_MDRIVE_OFFSET);

    if ((RegisterContents & MasterMask) != 0)
    {
        ReturnStatus |= XPA_DRIVING_BEAR_MASK;

    }

    RegisterContents =
        XPlbArb_In32(InstancePtr->BaseAddress + XPA_PESR_READ_OFFSET);

    if ((RegisterContents & MasterMask) != 0)
    {
        ReturnStatus |= XPA_ERROR_READ_MASK;

    }

    RegisterContents =
        XPlbArb_In32(InstancePtr->BaseAddress + XPA_PESR_LCK_ERR_OFFSET);

    if ((RegisterContents & MasterMask) != 0)
    {
        ReturnStatus |= XPA_ERROR_STATUS_LOCK_MASK;

    }

    RegisterContents =
        XPlbArb_In32(InstancePtr->BaseAddress + XPA_PEAR_SIZE_TYPE_OFFSET);

    RegisterContents =
      ((RegisterContents >> PEAR_SHIFT_VALUE) & PEAR_SHIFT_MASK);

    if (RegisterContents != 0)
    {
        ReturnStatus |= RegisterContents;

    }

    return ReturnStatus;
}


/*****************************************************************************/
/**
*
* Returns the PLB Address where the most recent error occured. If there isn't
* an outstanding error, the last address in error is returned. 0x00000000
* is the initial value coming out of reset.
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
*
* @return
*
* Address where error causing access occurred
*
* @note
*
* Calling XPlbArb_IsError() is recommended to confirm that an error has
* occurred prior to calling this function to ensure that the data in the
* error address register is relevant.
*
******************************************************************************/
Xuint32 XPlbArb_GetErrorAddress(XPlbArb *InstancePtr)
{
    Xuint32 RegisterContents;

    /*
     * Assert validates the input arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    RegisterContents =
        XPlbArb_In32(InstancePtr->BaseAddress + XPA_PEAR_ADDR_OFFSET);

    return RegisterContents;
}

/*****************************************************************************/
/**
*
* Returns the number of masters associated with the arbiter.
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
*
* @return
*
* The number of masters. This is a number from 1 to the maximum of 32.
*
* @note
*
* The value returned from this call needs to be adjusted if it is to be used
* as the argument for other calls since the masters are numbered from 0 and
* this function returns values starting at 1.
*
******************************************************************************/
Xuint8 XPlbArb_GetNumMasters(XPlbArb *InstancePtr)
{
    XASSERT_NONVOID(InstancePtr != XNULL);

    return InstancePtr->NumMasters;
}


/*****************************************************************************/
/**
*
* Enables the interrupt output from the arbiter
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
*
* @return
*
* None.
*
* @note
*
* The Arbiter hardware generates interrupts in error conditions. These
* interrupts are not handled by the driver directly. It is the application's
* responsibility to attach to the appropriate interrupt source with a handler
* which then calls functions provided by this driver to determine the cause of
* the error and take the necessary actions to correct the situation.
*
******************************************************************************/
void XPlbArb_EnableInterrupt(XPlbArb *InstancePtr)
{
    /*
     * ASSERT the arguments
     */
    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * enable the interrupts from the arbiter
     */
    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PACR_OFFSET,
              XPA_PACR_ENABLE_INTR_MASK);
}


/*****************************************************************************/
/**
*
* Disables the interrupt output from the arbiter
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
*
* @return
*
* None
*
* @note
*
* The Arbiter hardware generates interrupts in error conditions. These
* interrupts are not handled by the driver directly. It is the application's
* responsibility to attach to the appropriate interrupt source with a handler
* which then calls functions provided by this driver to determine the cause of
* the error and take the necessary actions to correct the situation.
*
******************************************************************************/
void XPlbArb_DisableInterrupt(XPlbArb *InstancePtr)
{
    /*
     * ASSERT the arguments
     */
    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * disable the interrupts from the arbiter
     */
    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PACR_OFFSET, 0);
}


/*****************************************************************************/
/**
*
* Forces a software reset to occur in the arbiter. Disables interrupts in the
* process.
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
*
* @return
*
* None.
*
* @note
*
* Disables interrupts in the process.
*
******************************************************************************/
void XPlbArb_Reset(XPlbArb *InstancePtr)
{
    /*
     * ASSERT the arguments
     */
    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * forces software interrupt to arbiter
     */
    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PACR_OFFSET,
              XPA_PACR_SOFTWARE_RESET_MASK);

    /*
     * clears software interrupt to arbiter
     */
    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PACR_OFFSET, 0);
}

/*****************************************************************************/
/**
*
* Looks up the device configuration based on the unique device ID. The table
* XPlbArb_ConfigTable contains the configuration info for each device in the
* system.
*
* @param DeviceId is the unique device ID to look for.
*
* @return
*
* A pointer to the configuration data for the device, or XNULL if no match is
* found.
*
* @note
*
* None.
*
******************************************************************************/
XPlbArb_Config *XPlbArb_LookupConfig(Xuint16 DeviceId)
{
    XPlbArb_Config *CfgPtr = XNULL;

    int i;

    for (i=0; i < XPAR_XPLBARB_NUM_INSTANCES; i++)
    {
        if (XPlbArb_ConfigTable[i].DeviceId == DeviceId)
        {
            CfgPtr = &XPlbArb_ConfigTable[i];
            break;
        }
    }

    return CfgPtr;
}

