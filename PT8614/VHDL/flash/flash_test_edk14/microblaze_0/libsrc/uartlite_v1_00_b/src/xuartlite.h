/* $Id: xuartlite.h,v 1.11.8.1 2005/09/02 22:09:40 trujillo Exp $ */
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
*       (c) Copyright 2002 Xilinx Inc.
*       All rights reserved.
*
*****************************************************************************/
/****************************************************************************/
/**
*
* @file xuartlite.h
*
* This component contains the implementation of the XUartLite component which is
* the driver for the Xilinx UART Lite device. This UART is a minimal hardware
* implementation with minimal features.  Most of the features, including baud
* rate, parity, and number of data bits are only configurable when the hardware
* device is built, rather than at run time by software.
*
* The device has 16 byte transmit and receive FIFOs and supports interrupts.
* The device does not have any way to disable the receiver such that the
* receive FIFO may contain unwanted data.  The FIFOs are not flushed when the
* driver is initialized, but a function is provided to allow the user to
* reset the FIFOs if desired.
*
* The driver defaults to no interrupts at initialization such that interrupts
* must be enabled if desired. An interrupt is generated when the transmit FIFO
* transitions from having data to being empty or when any data is contained in
* the receive FIFO.
*
* In order to use interrupts, it's necessary for the user to connect the driver
* interrupt handler, XUartLite_InterruptHandler, to the interrupt system of the
* application.  This function does not save and restore the processor context
* such that the user must provide it.  Send and receive handlers may be set for
* the driver such that the handlers are called when transmit and receive
* interrupts occur.  The handlers are called from interrupt context and are
* designed to allow application specific processing to be performed.
*
* The functions, XUartLite_Send and XUartLite_Recv, are provided in the driver
* to allow data to be sent and received. They are designed to be used in
* polled or interrupt modes.
*
* The driver provides a status for each received byte indicating any parity
* frame or overrun error. The driver provides statistics which allow visibility
* into these errors.
*
* <b>RTOS Independence</b>
*
* This driver is intended to be RTOS and processor independent.  It works
* with physical addresses only.  Any needs for dynamic memory management,
* threads or thread mutual exclusion, virtual memory, or cache control must
* be satisfied by the layer above this driver.
*
* @note
*
* The driver is partitioned such that a minimal implementation may be used.
* More features require additional files to be linked in.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  08/31/01 First release
* 1.00b jhl  02/21/02 Repartitioned the driver for smaller files
* </pre>
*
*****************************************************************************/

#ifndef XUARTLITE_H /* prevent circular inclusions */
#define XUARTLITE_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files ********************************/

#include "xbasic_types.h"
#include "xstatus.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/**
 * Callback function.  The first argument is a callback reference passed in by
 * the upper layer when setting the callback functions, and passed back to the
 * upper layer when the callback is invoked.
 * The second argument is the ByteCount which is the number of bytes that
 * actually moved from/to the buffer provided in the _Send/_Receive call.
 */
typedef void (*XUartLite_Handler)(void *CallBackRef, unsigned int ByteCount);

/**
 * Statistics for the XUartLite driver
 */
typedef struct
{
    Xuint32 TransmitInterrupts;     /**< Number of transmit interrupts */
    Xuint32 ReceiveInterrupts;      /**< Number of receive interrupts */
    Xuint32 CharactersTransmitted;  /**< Number of characters transmitted */
    Xuint32 CharactersReceived;     /**< Number of characters received */
    Xuint32 ReceiveOverrunErrors;   /**< Number of receive overruns */
    Xuint32 ReceiveParityErrors;    /**< Number of receive parity errors */
    Xuint32 ReceiveFramingErrors;   /**< Number of receive framing errors */
} XUartLite_Stats;

/**
 * The following data type is used to manage the buffers that are handled
 * when sending and receiving data in the interrupt mode. It is intended
 * for internal use only.
 */
typedef struct
{
    Xuint8 *NextBytePtr;
    unsigned int RequestedBytes;
    unsigned int RemainingBytes;
} XUartLite_Buffer;

/**
 * This typedef contains configuration information for the device.
 */
typedef struct
{
    Xuint16 DeviceId;       /**< Unique ID  of device */
    Xuint32 RegBaseAddr;    /**< Register base address */
    Xuint32 BaudRate;       /**< Fixed baud rate */
    Xuint8  UseParity;      /**< Parity generator enabled when XTRUE */
    Xuint8  ParityOdd;      /**< Parity generated is odd when XTRUE, even when
                                 XFALSE */
    Xuint8  DataBits;       /**< Fixed data bits */
} XUartLite_Config;

/**
 * The XUartLite driver instance data. The user is required to allocate a
 * variable of this type for every UART Lite device in the system. A pointer
 * to a variable of this type is then passed to the driver API functions.
 */
typedef struct
{
    XUartLite_Stats Stats;      /* Component Statistics */
    Xuint32 RegBaseAddress;     /* Base address of registers */
    Xuint32 IsReady;            /* Device is initialized and ready */


    XUartLite_Buffer SendBuffer;
    XUartLite_Buffer ReceiveBuffer;

    XUartLite_Handler RecvHandler;
    void *RecvCallBackRef;          /* Callback reference for recv handler */
    XUartLite_Handler SendHandler;
    void *SendCallBackRef;          /* Callback reference for send handler */
} XUartLite;


/***************** Macros (Inline Functions) Definitions ********************/


/************************** Function Prototypes *****************************/

/*
 * Required functions, in file xuart.c
 */
XStatus XUartLite_Initialize(XUartLite *InstancePtr, Xuint16 DeviceId);

void XUartLite_ResetFifos(XUartLite *InstancePtr);

unsigned int XUartLite_Send(XUartLite *InstancePtr, Xuint8 *DataBufferPtr,
                            unsigned int NumBytes);
unsigned int XUartLite_Recv(XUartLite *InstancePtr, Xuint8 *DataBufferPtr,
                            unsigned int NumBytes);

Xboolean XUartLite_IsSending(XUartLite *InstancePtr);
XUartLite_Config *XUartLite_LookupConfig(Xuint16 DeviceId);

/*
 * Functions for statistics, in file xuartlite_stats.c
 */
void XUartLite_GetStats(XUartLite *InstancePtr, XUartLite_Stats *StatsPtr);
void XUartLite_ClearStats(XUartLite *InstancePtr);

/*
 * Functions for self-test, in file xuartlite_selftest.c
 */
XStatus XUartLite_SelfTest(XUartLite *InstancePtr);

/*
 * Functions for interrupts, in file xuartlite_intr.c
 */
void XUartLite_EnableInterrupt(XUartLite *InstancePtr);
void XUartLite_DisableInterrupt(XUartLite *InstancePtr);

void XUartLite_SetRecvHandler(XUartLite *InstancePtr, XUartLite_Handler FuncPtr,
                              void *CallBackRef);
void XUartLite_SetSendHandler(XUartLite *InstancePtr, XUartLite_Handler FuncPtr,
                              void *CallBackRef);

void XUartLite_InterruptHandler(XUartLite *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */

