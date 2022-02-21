/* $Id: xgpio.h,v 1.3.8.1 2005/09/06 15:24:28 trujillo Exp $ */
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
*       (c) Copyright 2002 - 2004 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
* @file xgpio.h
*
* This file contains the software API definition of the Xilinx General Purpose
* I/O (XGpio) device driver component.
*
* The Xilinx GPIO controller is a soft IP core designed for Xilinx FPGAs on
* the OPB or PLB bus and contains the following general features:
*   - Support for up to 32 I/O discretes for each channel (64 bits total).
*   - Each of the discretes can be configured for input or output.
*   - Configurable support for dual channels and interrupt generation.
*
* The driver provides interrupt management functions. Implementation of
* interrupt handlers is left to the user. Refer to the provided interrupt
* example in the examples directory for details.
*
* This driver is intended to be RTOS and processor independent. Any needs for
* dynamic memory management, threads or thread mutual exclusion, virtual
* memory, or cache control must be satisfied by the layer above this driver.
*
* @note
*
* This API utilizes 32 bit I/O to the GPIO registers. With less than 32 bits,
* the unused bits from registers are read as zero and written as don't cares.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rmm  03/13/02 First release
* 2.00a jhl  11/26/03 Added support for dual channels and interrupts
* </pre>
*****************************************************************************/

#ifndef XGPIO_H  /* prevent circular inclusions */
#define XGPIO_H  /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xgpio_l.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/**
 * This typedef contains configuration information for the device.
 */
typedef struct
{
    Xuint16  DeviceId;          /* Unique ID  of device */
    Xuint32  BaseAddress;       /* Device base address */
    Xboolean InterruptPresent;  /* Are interrupts supported in h/w */
    Xboolean IsDual;            /* Are 2 channels supported in h/w */
} XGpio_Config;

/**
 * The XGpio driver instance data. The user is required to allocate a
 * variable of this type for every GPIO device in the system. A pointer
 * to a variable of this type is then passed to the driver API functions.
 */
typedef struct
{
    Xuint32  BaseAddress;   /* Device base address */
    Xuint32  IsReady;       /* Device is initialized and ready */
    XGpio_Config *ConfigPtr;/* Pointer to the configuration */
} XGpio;

/***************** Macros (Inline Functions) Definitions ********************/


/************************** Function Prototypes *****************************/

/*
 * API Basic functions implemented in xgpio.c
 */
XStatus XGpio_Initialize(XGpio *InstancePtr, Xuint16 DeviceId);
void    XGpio_SetDataDirection(XGpio *InstancePtr, unsigned Channel,
                               Xuint32 DirectionMask);
Xuint32 XGpio_DiscreteRead(XGpio *InstancePtr, unsigned Channel);
void    XGpio_DiscreteWrite(XGpio *InstancePtr, unsigned Channel, Xuint32 Mask);

XGpio_Config *XGpio_LookupConfig(Xuint16 DeviceId);

/*
 * API Functions implemented in xgpio_extra.c
 */
void XGpio_DiscreteSet(XGpio *InstancePtr, unsigned Channel, Xuint32 Mask);
void XGpio_DiscreteClear(XGpio *InstancePtr, unsigned Channel, Xuint32 Mask);

/*
 * API Functions implemented in xgpio_selftest.c
 */
XStatus XGpio_SelfTest(XGpio *InstancePtr);

/*
 * API Functions implemented in xgpio_intr.c
 */
void XGpio_InterruptGlobalEnable(XGpio *InstancePtr);
void XGpio_InterruptGlobalDisable(XGpio *InstancePtr);
void XGpio_InterruptEnable(XGpio *InstancePtr, Xuint32 Mask);
void XGpio_InterruptDisable(XGpio *InstancePtr, Xuint32 Mask);
void XGpio_InterruptClear(XGpio *InstancePtr, Xuint32 Mask);
Xuint32 XGpio_InterruptGetEnabled(XGpio *InstancePtr);
Xuint32 XGpio_InterruptGetStatus(XGpio *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */

