/* $Id: xopbarb.h,v 1.1.18.1 2005/09/22 16:06:07 trujillo Exp $ */
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
* @file xopbarb.h
*
* This component contains the implementation of the XOpbArb component. It is the
* driver for the On-chip Peripheral Bus (OPB) Arbiter. The arbiter performs bus
* arbitration for devices on the OPB
*
* <b>Priority Arbitration</b>
*
* By default, the arbiter prioritizes masters on the bus based on their master
* IDs. A master's ID corresponds to the signals with which it connects to the
* arbiter. Master 0 is the highest priority, master 1 the next highest, and so
* on. The device driver allows an application to modify this default behavior.
*
* There are two modes of priority arbitration, dynamic and fixed. The device
* can be parameterized in either of these modes. Fixed priority arbitration
* makes use of priority levels that can be configured by software. There is
* one level for each master on the bus. Priority level 0 is assigned the master
* ID of the highest priority master, priority level 1 is assigned the master ID
* of the next highest priority master, and so on.
*
* Dynamic priority arbitration utilizes a Least Recently Used(LRU) algorithm to
* determine the priorities of masters on the bus. Once a master is granted the
* bus, it falls to the lowest priority master. A master that is not granted the
* bus moves up in priority until it becomes the highest priority master based on
* the fact that it has been the least recently used master. The arbiter uses the
* currently assigned priority levels as its starting configuration for the LRU
* algorithm. Software can modify this starting configuration by assigning
* master IDs to the priority levels.
*
* When configuring priority levels (i.e., assigning master IDs to priority
* levels), the application must suspend use of the priority levels by the
* device. Every master must be represented by one and only one priority level.
* The device driver enforces this by making the application suspend use of the
* priority levels by the device during the time it takes to correctly configure
* the levels. Once the levels are configured, the application must explicitly
* resume use of the priority levels by the device. During the time priority
* levels are suspended, the device reverts to its default behavior of assigning
* priorities based on master IDs.
*
* <b>Bus Parking</b>
*
* By default, bus parking is disabled. The device driver allows an application
* to enable bus parking, which forces the arbiter to always assert the grant
* signal for a specific master when no other masters are requesting the bus.
* The master chosen for parking is either the master that was last granted the
* bus, or the master configured by the SetParkId function. When bus parking
* is enabled but no park ID has been set, bus parking defaults to the master
* that was last granted the bus.
*
* <b>Device Configuration</b>
*
* The device can be configured in various ways during the FPGA implementation
* process.  Configuration parameters are stored in xopbarb_g.c. A table is
* defined where each entry contains configuration information for a device.
* This information includes such things as the base address of the
* memory-mapped device, the number of masters on the bus, and the priority
* arbitration scheme.
*
* When the device is parameterized with only 1 master, or if the device is
* parameterized without a slave interface, there are no registers accessible to
* software and no configuration entry is available. In these configurations it
* is likely that the driver will not be loaded or used by the application.
* But in the off-chance that it is, it is assumed that no configuration
* information for the arbiter is entered in the xopbarb_g.c table. If config
* information were entered for the device, there will be nothing to prevent
* the driver's use, and any use of the driver under these circumstances will
* result in undefined behavior.
*
* @note
*
* This driver is not thread-safe. Thread safety must be guaranteed by the layer
* above this driver if there is a need to access the device from multiple
* threads.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.02a rpm  08/13/01 First release
* </pre>
*
******************************************************************************/

#ifndef XOPBARB_H /* prevent circular inclusions */
#define XOPBARB_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xopbarb_l.h"

/************************** Constant Definitions *****************************/

/** @name Configuration options
 * @{
 */
/**
 * The options enable or disable additional features of the OPB Arbiter. Each
 * of the options are bit fields such that more than one may be specified.
 *
 * - XOA_DYNAMIC_PRIORITY_OPTION
 * <br>
 * The Dynamic Priority option configures the device for dynamic priority
 * arbitration, which uses a Least Recently Used (LRU) algorithm to determine
 * the priorities of OPB masters.  This option is not applicable if the device
 * is parameterized for fixed priority arbitration.  When the device is
 * parameterized for dynamic priority arbitration, it can still use a fixed
 * priority arbitration by turning this option off. Fixed priority arbitration
 * uses the priority levels as written by software to determine the priorities
 * of OPB masters. The default is fixed priority arbitration.
 *
 * - XOA_PARK_ENABLE_OPTION
 * <br>
 * The Park Enable option enables bus parking, which forces the arbiter to
 * always assert the grant signal for a specific master when no other masters
 * are requesting the bus.  The master chosen for parking is either the master
 * that was last granted the bus, or the master configured by the SetParkId
 * function.
 *
 * - XOA_PARK_BY_ID_OPTION
 * <br>
 * The Park By ID option enables bus parking based on the specific master ID.
 * The master ID defaults to master 0, and can be changed using
 * XOpbArb_SetParkId(). When this option is disabled, bus parking defaults to
 * the master that was last granted the bus. The park enable option must be set
 * for this option to take effect.
 */
#define XOA_DYNAMIC_PRIORITY_OPTION   0x1
#define XOA_PARK_ENABLE_OPTION        0x2
#define XOA_PARK_BY_ID_OPTION         0x4
/*@}*/

/**************************** Type Definitions *******************************/

/**
 * This typedef contains configuration information for the device.
 */
typedef struct
{
    Xuint16 DeviceId;       /**< Unique ID  of device */
    Xuint32 BaseAddress;    /**< Register base address */
    Xuint8 NumMasters;      /**< Number of masters on the bus */
} XOpbArb_Config;

/**
 * The XOpbArb driver instance data. The user is required to allocate a
 * variable of this type for every OPB arbiter device in the system. A pointer
 * to a variable of this type is then passed to the driver API functions.
 */
typedef struct
{
    Xuint32 BaseAddress;        /* Base address of registers */
    Xuint32 IsReady;            /* Device is initialized and ready */
    Xuint8 NumMasters;          /* Number of masters on this bus */
} XOpbArb;

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/*
 * Initialization
 */
XStatus XOpbArb_Initialize(XOpbArb *InstancePtr, Xuint16 DeviceId);
XOpbArb_Config *XOpbArb_LookupConfig(Xuint16 DeviceId);

/*
 * Diagnostics
 */
XStatus XOpbArb_SelfTest(XOpbArb *InstancePtr);

/*
 * Configuration
 */
XStatus XOpbArb_SetOptions(XOpbArb *InstancePtr, Xuint32 Options);
Xuint32 XOpbArb_GetOptions(XOpbArb *InstancePtr);
XStatus XOpbArb_SetPriorityLevel(XOpbArb *InstancePtr, Xuint8 Level,
                                 Xuint8 MasterId);
Xuint8 XOpbArb_GetPriorityLevel(XOpbArb *InstancePtr, Xuint8 Level);

void XOpbArb_SuspendPriorityLevels(XOpbArb *InstancePtr);
XStatus XOpbArb_ResumePriorityLevels(XOpbArb *InstancePtr);

XStatus XOpbArb_SetParkId(XOpbArb *InstancePtr, Xuint8 MasterId);
XStatus XOpbArb_GetParkId(XOpbArb *InstancePtr, Xuint8 *MasterIdPtr);

#ifdef __cplusplus
}
#endif

#endif  /* end of protection macro */
