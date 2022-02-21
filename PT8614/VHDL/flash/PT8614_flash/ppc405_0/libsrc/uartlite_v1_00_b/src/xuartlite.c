/* $Id: xuartlite.c,v 1.21 2005/01/21 18:38:24 meinelte Exp $ */
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
* @file xuartlite.c
*
* Contains required functions for the XUartLite driver. See the xuartlite.h
* header file for more details on this driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ecm  08/31/01 First release
* 1.00b jhl  02/21/02 Repartitioned the driver for smaller files
* 1.00b rmm  05/13/03 Fixed diab compiler warnings relating to asserts
* </pre>
*
*****************************************************************************/

/***************************** Include Files ********************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xparameters.h"
#include "xuartlite.h"
#include "xuartlite_i.h"
#include "xio.h"

/************************** Constant Definitions ****************************/


/**************************** Type Definitions ******************************/


/***************** Macros (Inline Functions) Definitions ********************/


/************************** Variable Definitions ****************************/


/************************** Function Prototypes *****************************/

static void StubHandler(void *CallBackRef, unsigned int ByteCount);

/****************************************************************************/
/**
*
* Initialize a XUartLite instance.  The receive and transmit FIFOs of the
* UART are not flushed, so the user may want to flush them. The hardware
* device does not have any way to disable the receiver such that any valid
* data may be present in the receive FIFO.  This function disables the UART
* interrupt. The baudrate and format of the data are fixed in the hardware
* at hardware build time.
*
* @param    InstancePtr is a pointer to the XUartLite instance to be worked on.
* @param    DeviceId is the unique id of the device controlled by this
*           XUartLite instance.  Passing in a device id associates the
*           generic XUartLite instance to a specific device, as chosen by
*           the caller or application developer.
*
* @return
*
* - XST_SUCCESS if everything starts up as expected.
* - XST_DEVICE_NOT_FOUND if the device is not found in the configuration table.
*
* @note
*
* None.
*
*****************************************************************************/
XStatus XUartLite_Initialize(XUartLite *InstancePtr, Xuint16 DeviceId)
{
    XUartLite_Config *UartLiteConfigPtr;

    /*
     * Assert validates the input arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);

    /*
     * Lookup the device configuration in the configuration table. Use this
     * configuration info when initializing this component.
     */
    UartLiteConfigPtr = XUartLite_LookupConfig(DeviceId);

    if (UartLiteConfigPtr == (XUartLite_Config *)XNULL)
    {
        return XST_DEVICE_NOT_FOUND;
    }

    /*
     * Set some default values, including setting the callback
     * handlers to stubs.
     */

    InstancePtr->SendBuffer.NextBytePtr = XNULL;
    InstancePtr->SendBuffer.RemainingBytes = 0;
    InstancePtr->SendBuffer.RequestedBytes = 0;

    InstancePtr->ReceiveBuffer.NextBytePtr = XNULL;
    InstancePtr->ReceiveBuffer.RemainingBytes = 0;
    InstancePtr->ReceiveBuffer.RequestedBytes = 0;

    InstancePtr->IsReady = XCOMPONENT_IS_READY;
    InstancePtr->RegBaseAddress = UartLiteConfigPtr->RegBaseAddr;
    InstancePtr->RecvHandler = StubHandler;
    InstancePtr->SendHandler = StubHandler;

    /* Write to the control register to disable the interrupts, don't
     * reset the FIFOs are the user may want the data that's present
     */
    XIo_Out32(InstancePtr->RegBaseAddress + XUL_CONTROL_REG_OFFSET, 0);

    /*
     * Clear the statistics for this driver
     */
    XUartLite_mClearStats(InstancePtr);

    return XST_SUCCESS;
}

/****************************************************************************/
/**
*
* This functions sends the specified buffer of data using the UART in either
* polled or interrupt driven modes. This function is non-blocking such that it
* will return before the data has been sent by the UART. If the UART is busy
* sending data, it will return and indicate zero bytes were sent.
*
* In a polled mode, this function will only send as much data as the UART can
* buffer in the FIFO. The application may need to call it repeatedly to
* send a buffer.
*
* In interrupt mode, this function will start sending the specified buffer and
* then the interrupt handler of the driver will continue sending data until the
* buffer has been sent. A callback function, as specified by the application,
* will be called to indicate the completion of sending the buffer.
*
* @param    InstancePtr is a pointer to the XUartLite instance to be worked on.
* @param    BufferPtr is pointer to a buffer of data to be sent.
* @param    NumBytes contains the number of bytes to be sent. A value of zero
*           will stop a previous send operation that is in progress in interrupt
*           mode. Any data that was already put into the transmit FIFO will be
*           sent.
*
* @return
*
* The number of bytes actually sent.
*
* @note
*
* The number of bytes is not asserted so that this function may be called with
* a value of zero to stop an operation that is already in progress.
*
******************************************************************************/
unsigned int XUartLite_Send(XUartLite *InstancePtr, Xuint8 *DataBufferPtr,
                            unsigned int NumBytes)
{
    unsigned int BytesSent;
    Xuint32 StatusRegister;

    /*
     * Assert validates the input arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(DataBufferPtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);
    XASSERT_NONVOID(((signed)NumBytes) >= 0);

    /* Enter a critical region by disabling the UART interrupts to
     * allow this call to stop a previous operation that may be interrupt
     * driven
     */
    StatusRegister = XUartLite_mGetStatusReg(InstancePtr->RegBaseAddress);

    XIo_Out32(InstancePtr->RegBaseAddress + XUL_CONTROL_REG_OFFSET, 0);

    /* Setup the specified buffer to be sent by setting the instance
     * variables so it can be sent with polled or interrupt mode
     */
    InstancePtr->SendBuffer.RequestedBytes = NumBytes;
    InstancePtr->SendBuffer.RemainingBytes = NumBytes;
    InstancePtr->SendBuffer.NextBytePtr = DataBufferPtr;

    /* Restore the interrupt enable register to it's previous value such
     * that the critical region is exited.
     * This is done here to minimize the amount of time the interrupt is
     * disabled since there is only one interrupt and the receive could
     * be filling up while interrupts are blocked.
     */

    StatusRegister &= XUL_CR_ENABLE_INTR;
    XIo_Out32(InstancePtr->RegBaseAddress + XUL_CONTROL_REG_OFFSET, StatusRegister);

    /* Send the buffer using the UART and return the number of bytes sent */

    BytesSent = XUartLite_SendBuffer(InstancePtr);

    return BytesSent;
}


/****************************************************************************/
/**
*
* This function will attempt to receive a specified number of bytes of data
* from the UART and store it into the specified buffer. This function is
* designed for either polled or interrupt driven modes. It is non-blocking
* such that it will return if no data has already received by the UART.
*
* In a polled mode, this function will only receive as much data as the UART
* can buffer in the FIFO. The application may need to call it repeatedly to
* receive a buffer. Polled mode is the default mode of operation for the driver.
*
* In interrupt mode, this function will start receiving and then the interrupt
* handler of the driver will continue receiving data until the buffer has been
* received. A callback function, as specified by the application, will be called
* to indicate the completion of receiving the buffer or when any receive errors
* or timeouts occur. Interrupt mode must be enabled using the SetOptions function.
*
* @param    InstancePtr is a pointer to the XUartLite instance to be worked on.
* @param    BufferPtr is pointer to buffer for data to be received into
* @param    NumBytes is the number of bytes to be received. A value of zero will
*           stop a previous receive operation that is in progress in interrupt mode.
*
* @return
*
* The number of bytes received.
*
* @note
*
* The number of bytes is not asserted so that this function may be called with
* a value of zero to stop an operation that is already in progress.
*
*****************************************************************************/
unsigned int XUartLite_Recv(XUartLite *InstancePtr, Xuint8 *DataBufferPtr,
                            unsigned int NumBytes)
{
    unsigned int ReceivedCount;
    Xuint32 StatusRegister;

    /*
     * Assert validates the input arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);
    XASSERT_NONVOID(DataBufferPtr != XNULL);
    XASSERT_NONVOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);
    XASSERT_NONVOID(((signed)NumBytes) >= 0);

    /* Enter a critical region by disabling all the UART interrupts to allow
     * this call to stop a previous operation that may be interrupt driven
     */
    StatusRegister = XUartLite_mGetStatusReg(InstancePtr->RegBaseAddress);
    XIo_Out32(InstancePtr->RegBaseAddress + XUL_CONTROL_REG_OFFSET, 0);

    /* Setup the specified buffer to be received by setting the instance
     * variables so it can be received with polled or interrupt mode
     */
    InstancePtr->ReceiveBuffer.RequestedBytes = NumBytes;
    InstancePtr->ReceiveBuffer.RemainingBytes = NumBytes;
    InstancePtr->ReceiveBuffer.NextBytePtr = DataBufferPtr;

    /* Restore the interrupt enable register to it's previous value such
     * that the critical region is exited
     */
    StatusRegister &= XUL_CR_ENABLE_INTR;
    XIo_Out32(InstancePtr->RegBaseAddress + XUL_CONTROL_REG_OFFSET, StatusRegister);

    /* Receive the data from the UART and return the number of bytes
     * received
     * This is done here to minimize the amount of time the interrupt is
     * disabled since there is only one interrupt and the transmit could
     * be emptying out while interrupts are blocked.
     */

    ReceivedCount = XUartLite_ReceiveBuffer(InstancePtr);

    return ReceivedCount;

}

/****************************************************************************/
/**
*
* This function resets the FIFOs, both transmit and receive, of the UART such
* that they are emptied.  Since the UART does not have any way to disable it
* from receiving data, it may be necessary for the application to reset the
* FIFOs to get rid of any unwanted data.
*
* @param    InstancePtr is a pointer to the XUartLite instance to be worked on.
*
* @return
*
* None.
*
* @note
*
* None.
*
*****************************************************************************/
void XUartLite_ResetFifos(XUartLite *InstancePtr)
{
    Xuint32 Register;

    XASSERT_VOID(InstancePtr != XNULL);
    XASSERT_VOID(InstancePtr->IsReady == XCOMPONENT_IS_READY);

    /* Read the status register 1st such that the next write to the control
     * register won't destroy the state of the interrupt enable bit
     */
    Register =
        XIo_In32(InstancePtr->RegBaseAddress + XUL_STATUS_REG_OFFSET);

    /*
     * Mask off the interrupt enable bit to maintain it's state.
     */

    Register &= XUL_SR_INTR_ENABLED;

    /* Write to the control register to reset both FIFOs, these bits are
     * self-clearing such that there's no need to clear them
     */
    XIo_Out32(InstancePtr->RegBaseAddress + XUL_CONTROL_REG_OFFSET,
              Register | XUL_CR_FIFO_TX_RESET | XUL_CR_FIFO_RX_RESET);
}

/****************************************************************************/
/**
*
* This function determines if the specified UART is sending data. If the
* transmitter register is not empty, it is sending data.
*
* @param    InstancePtr is a pointer to the XUartLite instance to be worked on.
*
* @return
*
* A value of XTRUE if the UART is sending data, otherwise XFALSE.
*
* @note
*
* None.
*
*****************************************************************************/
Xboolean XUartLite_IsSending(XUartLite *InstancePtr)
{
    Xuint32 StatusRegister;

    /*
     * Assert validates the input arguments
     */
    XASSERT_NONVOID(InstancePtr != XNULL);

    /* Read the status register to determine if the transmitter is
     * empty
     */
    StatusRegister = XIo_In32(InstancePtr->RegBaseAddress + XUL_STATUS_REG_OFFSET);

    /* If the transmitter is not empty then indicate that the UART is still
     * sending some data
     */
    return ((StatusRegister & XUL_SR_TX_FIFO_EMPTY) == 0);
}


/****************************************************************************
*
* This function provides a stub handler such that if the application does not
* define a handler but enables interrupts, this function will be called.
*
* @param    CallBackRef has no purpose but is necessary to match the
*           interface for a handler.
* @param    ByteCount has no purpose but is necessary to match the
*           interface for a handler.
*
* @return
*
* None.
*
* @note
*
* None.
*
*****************************************************************************/
static void StubHandler(void *CallBackRef, unsigned int ByteCount)
{
    /*
     * Assert occurs always since this is a stub and should never be called
     */
    XASSERT_VOID_ALWAYS();
}

/****************************************************************************
*
* Looks up the device configuration based on the unique device ID.  The table
* UartliteConfigTable contains the configuration info for each device in the
* system.
*
* @param DeviceId is the unique device ID to match on.
*
* @return
*
* A pointer to the configuration data for the device, or XNULL if no match
* was found.
*
* @note
*
* None.
*
******************************************************************************/
XUartLite_Config *XUartLite_LookupConfig(Xuint16 DeviceId)
{
    XUartLite_Config *CfgPtr = XNULL;
    int i;

    for (i=0; i < XPAR_XUARTLITE_NUM_INSTANCES; i++)
    {
        if (XUartLite_ConfigTable[i].DeviceId == DeviceId)
        {
            CfgPtr = &XUartLite_ConfigTable[i];
            break;
        }
    }

    return CfgPtr;
}

/****************************************************************************/
/**
*
* This function sends a buffer that has been previously specified by setting
* up the instance variables of the instance. This function is designed to be
* an internal function for the XUartLite component such that it may be called
* from a shell function that sets up the buffer or from an interrupt handler.
*
* This function sends the specified buffer of data to the UART in either
* polled or interrupt driven modes. This function is non-blocking such that
* it will return before the data has been sent by the UART.
*
* In a polled mode, this function will only send as much data as the UART can
* buffer, either in the transmitter or in the FIFO if present and enabled.
* The application may need to call it repeatedly to send a buffer.
*
* In interrupt mode, this function will start sending the specified buffer and
* then the interrupt handler of the driver will continue until the buffer
* has been sent. A callback function, as specified by the application, will
* be called to indicate the completion of sending the buffer.
*
* @param    InstancePtr is a pointer to the XUartLite instance to be worked on.
*
* @return
*
* NumBytes is the number of bytes actually sent (put into the UART transmitter
* and/or FIFO).
*
* @note
*
* None.
*
*****************************************************************************/
unsigned int XUartLite_SendBuffer(XUartLite *InstancePtr)
{
    unsigned int SentCount = 0;
    Xuint32 StatusRegister;

    /* Read the line status register to determine if the transmitter is
     * full
     */

    StatusRegister = XUartLite_mGetStatusReg(InstancePtr->RegBaseAddress);

    /*
     * Fill the FIFO from the the buffer that was specified
     */

    SentCount = 0;

    while (((StatusRegister & XUL_SR_TX_FIFO_FULL) == 0) &&
           (SentCount < InstancePtr->SendBuffer.RemainingBytes))
    {
        XIo_Out32(InstancePtr->RegBaseAddress + XUL_TX_FIFO_OFFSET,
                  InstancePtr->SendBuffer.NextBytePtr[SentCount]);

        SentCount++;

        StatusRegister = XUartLite_mGetStatusReg(InstancePtr->RegBaseAddress);

    }

    /*
     * Update the buffer to reflect the bytes that were sent from it
     */
    InstancePtr->SendBuffer.NextBytePtr += SentCount;
    InstancePtr->SendBuffer.RemainingBytes -= SentCount;

    /*
     * Increment associated counters
     */
     InstancePtr->Stats.CharactersTransmitted += SentCount;

    /*
     * Return the number of bytes that were sent, althought they really were
     * only put into the FIFO, not completely sent yet
     */
    return SentCount;
}

/****************************************************************************/
/**
*
* This function receives a buffer that has been previously specified by setting
* up the instance variables of the instance. This function is designed to be
* an internal function for the XUartLite component such that it may be called
* from a shell function that sets up the buffer or from an interrupt handler.
*
* This function will attempt to receive a specified number of bytes of data
* from the UART and store it into the specified buffer. This function is
* designed for either polled or interrupt driven modes. It is non-blocking
* such that it will return if there is no data has already received by the
* UART.
*
* In a polled mode, this function will only receive as much data as the UART
* can buffer, either in the receiver or in the FIFO if present and enabled.
* The application may need to call it repeatedly to receive a buffer. Polled
* mode is the default mode of operation for the driver.
*
* In interrupt mode, this function will start receiving and then the interrupt
* handler of the driver will continue until the buffer has been received. A
* callback function, as specified by the application, will be called to indicate
* the completion of receiving the buffer or when any receive errors or timeouts
* occur. Interrupt mode must be enabled using the SetOptions function.
*
* @param    InstancePtr is a pointer to the XUartLite instance to be worked on.
*
* @return
*
* The number of bytes received.
*
* @note
*
* None.
*
*****************************************************************************/
unsigned int XUartLite_ReceiveBuffer(XUartLite *InstancePtr)
{
    Xuint8 StatusRegister;
    unsigned int ReceivedCount = 0;

    /* Loop until there is not more data buffered by the UART or the specified
     * number of bytes is received
     */

    while (ReceivedCount < InstancePtr->ReceiveBuffer.RemainingBytes)
    {
        /* Read the Status Register to determine if there is any data in
         * the receiver/FIFO
         */
        StatusRegister = XUartLite_mGetStatusReg(InstancePtr->RegBaseAddress);

        /* If there is data ready to be removed, then put the next byte
         * received into the specified buffer and update the stats to reflect
         * any receive errors for the byte
         */
        if (StatusRegister & XUL_SR_RX_FIFO_VALID_DATA)
        {
            InstancePtr->ReceiveBuffer.NextBytePtr[ReceivedCount++] =
                XIo_In32(InstancePtr->RegBaseAddress + XUL_RX_FIFO_OFFSET);

            XUartLite_mUpdateStats(InstancePtr, StatusRegister);
        }

        /* There's no more data buffered, so exit such that this function does
         * not block waiting for data
         */
        else
        {
            break;
        }
    }

    /* Enter a critical region by disabling all the UART interrupts to allow
     * this call to stop a previous operation that may be interrupt driven
     */
    StatusRegister = XUartLite_mGetStatusReg(InstancePtr->RegBaseAddress);
    XIo_Out32(InstancePtr->RegBaseAddress + XUL_CONTROL_REG_OFFSET, 0);

    /* Update the receive buffer to reflect the number of bytes that was
     * received
     */
    InstancePtr->ReceiveBuffer.NextBytePtr += ReceivedCount;
    InstancePtr->ReceiveBuffer.RemainingBytes -= ReceivedCount;

    /*
     * Increment associated counters in the statistics
     */
    InstancePtr->Stats.CharactersReceived += ReceivedCount;

    /* Restore the interrupt enable register to it's previous value such
     * that the critical region is exited
     */
    StatusRegister &= XUL_CR_ENABLE_INTR;
    XIo_Out32(InstancePtr->RegBaseAddress + XUL_CONTROL_REG_OFFSET, StatusRegister);

    return ReceivedCount;
}

