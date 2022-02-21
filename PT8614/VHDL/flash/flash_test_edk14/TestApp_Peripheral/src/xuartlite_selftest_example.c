#define TESTAPP_GEN

/* $Id: xuartlite_selftest_example.c,v 1.1 2005/01/27 21:03:49 meinelte Exp $ */
/*****************************************************************************
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
*       (c) Copyright 2005 Xilinx Inc.
*       All rights reserved.
*
*****************************************************************************/
/****************************************************************************/
/**
*
* @file xuartlite_selftest_example.c
*
* This file contains a design example using the Uart Lite driver
* (XUartLite) and hardware device using polled mode.
*
* @note
*
* None.
*
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  01/25/04 First Release.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xparameters.h"
#include "xuartlite.h"
#include "xstatus.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/****************************************************************************
*
* MACRO:
*
* STATUS_CHECK
*
* DESCRIPTION:
*
* This macro checks the status for functions called. It performs a return
* if the status was not success.  The purpose of this macro is to allow
* centralized status checking.
*
* ARGUMENTS:
*
* Status contains any status from a Xilinx driver.
*
* RETURN VALUE:
*
* None.
*
* NOTES:
*
* Signature: void STATUS_CHECK(XStatus Status)
*
****************************************************************************/
#define STATUS_CHECK(Status)            \
    {                                   \
        if (Status != XST_SUCCESS)      \
        {                               \
            return Status;              \
        }                               \
    }

/************************** Function Prototypes ******************************/

XStatus UartLiteSelfTestExample(Xuint16 DeviceId);

/************************** Variable Definitions *****************************/

XUartLite UartLite;

/******************************************************************************
*
* Main function to call the example.  This function is not included if the
* example is generated from the TestAppGen test tool.
*
******************************************************************************/
#ifndef TESTAPP_GEN
int main(void)
{
    (void)UartLiteSelfTestExample(XPAR_OPB_UARTLITE_0_DEVICE_ID);

    return 0;
}
#endif

/*****************************************************************************/
/**
*
* This function does a minimal test on the UART Lite device and driver as a
* design example.  The purpose of this function is to illustrate
* how to use the XUartLite component.
*
*
* @param DeviceId is the XPAR_<uartlite_instance>_DEVICE_ID value from
*        xparameters.h
*
* @return
*
* 0 if successful, otherwise the number of errors that occurred.
*
* @note
*
* None
*
****************************************************************************/
XStatus UartLiteSelfTestExample(Xuint16 DeviceId)
{
    XStatus Status;

    /* Initialize the UART Lite driver so that it's ready to use,
     * specify the device ID that is generated in xparameters.h
     */
    Status = XUartLite_Initialize(&UartLite, DeviceId);
    STATUS_CHECK(Status);

    /* Perform a self-test to ensure that the hardware was built
     * correctly
     */
    Status = XUartLite_SelfTest(&UartLite);
    STATUS_CHECK(Status);

    return XST_SUCCESS;
}
