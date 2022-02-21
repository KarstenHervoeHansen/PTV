/* $Id: xplbarb.h,v 1.3.10.1 2005/09/22 15:08:15 trujillo Exp $ */
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
* @file xplbarb.h
*
* This component contains the implementation of the XPlbArb component. It is the
* driver for the PLB (Processor Local Bus) Arbiter. The arbiter performs bus
* arbitration on the PLB transactions.
*
* This driver allows the user to access the PLB Arbiter registers to support
* the handling of bus errors and other access errors and determine an
* appropriate solution if possible.
*
* The Arbiter Hardware generates an interrupt in error conditions which is not
* handled by the driver directly. It is the application's responsibility to
* attach to the appropriate interrupt with a handler which then calls functions
* provided by this driver to determine the cause of the error and take the
* necessary actions to correct the situation.
*
* <b>Hardware Features</b>
*
* The Xilinx PLB Arbiter is a soft IP core designed for Xilinx FPGAs and contains
* the following features:
*   - PLB address and data steering support for up to eight masters. Number of
*       PLB masters is configurable via a design parameter
*   - 64-bit and/or 32-bit support for masters and slaves
*   - PLB address pipelining
*   - PLB arbitration support for up to eight masters. Number of PLB masters is
*       configurable via a design parameter
*   - Three cycle arbitration
*   - Four levels of dynamic master request priority
*   - PLB watchdog timer
*   - PLB architecture compliant
*
* <b>Device Configuration</b>
*
* The device can be configured in various ways during the FPGA implementation
* process.  The configuration data for each device is contained in xplbarb_g.c.
* A table is defined where each entry contains configuration information for a
* device. This information includes such things as the base address of the DCR
* mapped device, and the number of masters on the bus.
*
* @note
*
* This driver is not thread-safe. Thread safety must be guaranteed by the layer
* above this driver if there is a need to access the device from multiple
* threads.
* <br><br>
* The Arbiter registers reside on the DCR address bus.
* <br><br>
* Any and all outstanding errors are cleared in the initialization function.
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

#ifndef XPLBARB_H /* prevent circular inclusions */
#define XPLBARB_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/
#include "xbasic_types.h"
#include "xstatus.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/**
 * This typedef contains configuration information for the device.  This
 * information would typically be extracted from Configuration ROM (CROM).
 */
typedef struct
{
    Xuint16 DeviceId;       /**< Unique ID  of device */
    Xuint32 BaseAddress;    /**< Register base address */
    Xuint8 NumMasters;      /**< Number of masters on the bus */
} XPlbArb_Config;

/**
 * The XPlbArb driver instance data. The user is required to allocate a
 * variable of this type for every PLB arbiter device in the system. A pointer
 * to a variable of this type is then passed to the driver API functions.
 */
typedef struct
{
    Xuint32 BaseAddress;        /* Base address of registers */
    Xuint32 IsReady;            /* Device is initialized and ready */
    Xuint8 NumMasters;          /* number of masters for this arbiter */
} XPlbArb;



/***************** Macros (Inline Functions) Definitions ********************/


/************************** Function Prototypes *****************************/


/*
 * Required functions in xplbarb.c
 */

/*
 * Initialization Functions
 */
XStatus XPlbArb_Initialize(XPlbArb *InstancePtr, Xuint16 DeviceId);
void XPlbArb_Reset(XPlbArb *InstancePtr);
XPlbArb_Config *XPlbArb_LookupConfig(Xuint16 DeviceId);

/*
 * Access Functions
 */
Xboolean XPlbArb_IsError(XPlbArb *InstancePtr);
void XPlbArb_ClearErrors(XPlbArb *InstancePtr, Xuint8 Master);

Xuint32 XPlbArb_GetErrorStatus(XPlbArb *InstancePtr, Xuint8 Master);
Xuint32 XPlbArb_GetErrorAddress(XPlbArb *InstancePtr);

/*
 * Configuration
 */
Xuint8 XPlbArb_GetNumMasters(XPlbArb *InstancePtr);
void XPlbArb_EnableInterrupt(XPlbArb *InstancePtr);
void XPlbArb_DisableInterrupt(XPlbArb *InstancePtr);

/*
 * Self-test functions in xplbarb_selftest.c
 */
XStatus XPlbArb_SelfTest(XPlbArb *InstancePtr, Xuint32 TestAddress);

#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */
