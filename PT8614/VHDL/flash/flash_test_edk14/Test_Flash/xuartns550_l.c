/* $Id: xuartns550_l.c,v 1.2 2002/05/17 18:17:29 moleres Exp $ */
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
* @file xuartns550_l.c
*
* This file contains low-level driver functions that can be used to access the
* device.  The user should refer to the hardware device specification for more
* details of the device operation.
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


/***************************** Include Files *********************************/

#include "xuartns550_l.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/


/****************************************************************************/
/**
*
* This function sends a data byte with the UART. This function operates in the
* polling mode and blocks until the data has been put into the UART transmit
* holding register.
*
* @param    BaseAddress contains the base address of the UART.
* @param    Data contains the data byte to be sent.
*
* @return   None.
*
* @note     None.
*
*****************************************************************************/
void XUartNs550_SendByte(Xuint32 BaseAddress, Xuint8 Data)
{
    /* Wait til we know that the byte can be sent, the 550 does not have any
     * way to tell how much room is in the FIFO such that we must wait for
     * it to be empty
     */
    while (!XUartNs550_mIsTransmitEmpty(BaseAddress));

    /* Write the data byte to the UART to be transmitted */

    XUartNs550_mWriteReg(BaseAddress, XUN_THR_OFFSET, Data);
}

/****************************************************************************/
/**
*
* This function receives a byte from the UART.  It operates in a polling mode
* and blocks until a byte of data is received.
*
* @param    BaseAddress contains the base address of the UART.
*
* @return   The data byte received by the UART.
*
* @note     None.
*
*****************************************************************************/
Xuint8 XUartNs550_RecvByte(Xuint32 BaseAddress)
{
   /* Wait for there to be data received */

   while (!XUartNs550_mIsReceiveData(BaseAddress));

   /* Return the next data byte the UART received */

   return XUartNs550_mReadReg(BaseAddress, XUN_RBR_OFFSET);
}


/*****************************************************************************
*
* Set the baud rate for the UART.
*
* @param    BaseAddress contains the base address of the UART.
* @param    InputClockHz is the frequency of the input clock to the device in
*           Hertz.
* @param    BaudRate is the baud rate to be set.
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
void XUartNs550_SetBaud(Xuint32 BaseAddress, Xuint32 InputClockHz,
                        Xuint32 BaudRate)
{

    Xuint8 BaudLSB;
    Xuint8 BaudMSB;
    Xuint8 LcrRegister;
    Xuint32 Divisor;

    /*
     * Determine what the divisor should be to get the specified baud
     * rater based upon the input clock frequency and a baud clock prescaler
     * of 16
     */
    Divisor = InputClockHz / (BaudRate * 16UL);

    /* Get the least significant and most significant bytes of the divisor
     * so they can be written to 2 byte registers
     */
    BaudLSB = Divisor & XUN_DIVISOR_BYTE_MASK;
    BaudMSB = (Divisor >> 8) & XUN_DIVISOR_BYTE_MASK;

    /* Get the line control register contents and set the divisor latch
     * access bit so the baud rate can be set
     */
    LcrRegister = XUartNs550_mReadReg(BaseAddress, XUN_LCR_OFFSET);

    XUartNs550_mWriteReg(BaseAddress, XUN_LCR_OFFSET,
                         LcrRegister | XUN_LCR_DLAB);

    /*
     * Set the baud Divisors to set rate, the initial write of 0xFF is to keep
     * the divisor from being 0 which is not recommended as per the NS16550D
     * spec sheet
     */
    XUartNs550_mWriteReg(BaseAddress, XUN_DRLS_OFFSET, 0xFF);
    XUartNs550_mWriteReg(BaseAddress, XUN_DRLM_OFFSET, BaudMSB);
    XUartNs550_mWriteReg(BaseAddress, XUN_DRLS_OFFSET, BaudLSB);

    /*
     * Clear the Divisor latch access bit, DLAB to allow nornal
     * operation and write to the line control register
     */
    XUartNs550_mWriteReg(BaseAddress, XUN_LCR_OFFSET, LcrRegister);
}

