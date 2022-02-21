/* $Id: xplbarb_selftest.c,v 1.4 2004/07/19 17:29:05 moleres Exp $ */
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
* @file xplbarb_selftest.c
*
* Contains diagnostic self-test functions for the XPlbArb component. See
* xplbarb.h for more information about the component.
*
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  11/16/01 First release
* 1.01a rpm  05/13/02 Updated to match hw version, removed _g.h
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xstatus.h"

#include "xplbarb.h"
#include "xplbarb_i.h"
#include "xio_dcr.h"

/************************** Constant Definitions ****************************/


/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/

/************************** Function Prototypes *****************************/


/*****************************************************************************/
/**
*
* Runs a self-test on the driver/device.
*
* This tests reads the PACR to verify that the proper value is there.
*
* XST_SUCCESS is returned if expected value is there,
* XST_PLBARB_FAIL_SELFTEST is returned otherwise.
*
* @param    InstancePtr is a pointer to the XPlbArb instance to be worked on.
* @param    TestAddress is a location that could cause an error on read,
*           not used - user definable for hw specific implementations.
*
* @return
*
* XST_SUCCESS if successful, or XST_PLBARB_FAIL_SELFTEST if the driver
* fails the self test.
*
* @note
*
* None.
*
******************************************************************************/
XStatus XPlbArb_SelfTest(XPlbArb *InstancePtr, Xuint32 TestAddress)
{
    volatile Xuint32 TestReadResult;
    Xuint32 TestRegisterWrite;
    Xuint32 TestRegisterRead;
    Xuint32 CurrentPACR;
    Xuint32 CurrentRegister;

    /*
     * ASSERT the arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /*
     * perform the read operation
     * must be a raw memory read, Xio functions could be overridden
     */
    /* TestReadResult = (volatile) *(Xuint32 *)TestAddress; */

    /*
     * read error register
     */
    TestReadResult = XPlbArb_In32(InstancePtr->BaseAddress + XPA_PACR_OFFSET);

    /*
     * check for error
     */
    if (TestReadResult != XPA_PACR_ENABLE_INTR_MASK)
    {
        /*
         * test failed, indicate this to the calling routine
         */
        return XST_PLBARB_FAIL_SELFTEST;
    }


    /*
     * if the Arbiter appears to be in reset condition, continue with testing
     * setting the test enable bit, writing a value to a register, reading it
     * back and then returning the register to its original state
     */
    CurrentPACR = XPlbArb_In32(InstancePtr->BaseAddress + XPA_PACR_OFFSET);


    CurrentRegister =
            XPlbArb_In32(InstancePtr->BaseAddress + XPA_PEAR_ADDR_OFFSET);

    TestRegisterWrite = XPA_PACR_TEST_ENABLE_MASK;

    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PACR_OFFSET, TestRegisterWrite);


    TestRegisterWrite = 0xDEADBEEF;

    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PEAR_ADDR_OFFSET,
               TestRegisterWrite);

    TestRegisterRead =
            XPlbArb_In32(InstancePtr->BaseAddress + XPA_PEAR_ADDR_OFFSET);


    /*
     * return the values
     */
    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PEAR_ADDR_OFFSET,
              CurrentRegister);

    XPlbArb_Out32(InstancePtr->BaseAddress + XPA_PACR_OFFSET, CurrentPACR);

    /*
     * test the results
     */
    if (TestRegisterWrite != TestRegisterRead)
    {
        /*
         * test failed, indicate this to the calling routine
         */
        return XST_PLBARB_FAIL_SELFTEST;

    }

    /*
     * test passed, indicate this to the calling routine
     */
    return XST_SUCCESS;
}


