/* $Id: xuartns550_l.h,v 1.3.12.1 2005/09/02 21:58:22 trujillo Exp $ */
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
* @file xuartns550_l.h
*
* This header file contains identifiers and low-level driver functions (or
* macros) that can be used to access the device.  The user should refer to the
* hardware device specification for more details of the device operation.
* High-level driver functions are defined in xuartns550.h.
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

#ifndef XUARTNS550_L_H /* prevent circular inclusions */
#define XUARTNS550_L_H /* by using protection macros */

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/

#include "xbasic_types.h"
#include "xio.h"

/************************** Constant Definitions *****************************/

/*
 * Offset from the device base address (IPIF) to the IP registers.
 */
#define XUN_REG_OFFSET 0x1000

/* 16450/16550 compatible UART, register offsets as byte registers */

#define XUN_RBR_OFFSET  (XUN_REG_OFFSET + 0x03) /* receive buffer, read only */
#define XUN_THR_OFFSET  (XUN_REG_OFFSET + 0x03) /* transmit holding register */
#define XUN_IER_OFFSET  (XUN_REG_OFFSET + 0x07) /* interrupt enable */
#define XUN_IIR_OFFSET  (XUN_REG_OFFSET + 0x0B) /* interrupt id, read only */
#define XUN_FCR_OFFSET  (XUN_REG_OFFSET + 0x0B) /* fifo control, write only */
#define XUN_LCR_OFFSET  (XUN_REG_OFFSET + 0x0F) /* line control register */
#define XUN_MCR_OFFSET  (XUN_REG_OFFSET + 0x13) /* modem control register */
#define XUN_LSR_OFFSET  (XUN_REG_OFFSET + 0x17) /* line status register */
#define XUN_MSR_OFFSET  (XUN_REG_OFFSET + 0x1B) /* modem status register */
#define XUN_DRLS_OFFSET (XUN_REG_OFFSET + 0x03) /* divisor register LSB */
#define XUN_DRLM_OFFSET (XUN_REG_OFFSET + 0x07) /* divisor register MSB */

/* the following constant specifies the size of the FIFOs, the size of the
 * FIFOs includes the transmitter and receiver such that it is the total number
 * of bytes that the UART can buffer
 */
#define XUN_FIFO_SIZE               16

/* Interrupt Enable Register bits */

#define XUN_IER_MODEM_STATUS        0x08    /* modem status interrupt */
#define XUN_IER_RX_LINE             0x04    /* receive status interrupt */
#define XUN_IER_TX_EMPTY            0x02    /* transmitter empty interrupt */
#define XUN_IER_RX_DATA             0x01    /* receiver data available */

/* Interrupt ID Register bits */

#define XUN_INT_ID_MASK             0x0F    /* only the interrupt ID */
#define XUN_INT_ID_FIFOS_ENABLED    0xC0    /* only the FIFOs enable */

/* FIFO Control Register bits */

#define XUN_FIFO_RX_TRIG_MSB        0x80    /* trigger level MSB */
#define XUN_FIFO_RX_TRIG_LSB        0x40    /* trigger level LSB */
#define XUN_FIFO_TX_RESET           0x04    /* reset the transmit FIFO */
#define XUN_FIFO_RX_RESET           0x02    /* reset the receive FIFO */
#define XUN_FIFO_ENABLE             0x01    /* enable the FIFOs */
#define XUN_FIFO_RX_TRIGGER         0xC0    /* both trigger level bits */

/* Line Control Register bits */

#define XUN_LCR_DLAB                0x80    /* divisor latch access */
#define XUN_LCR_SET_BREAK           0x40    /* cause a break condition */
#define XUN_LCR_STICK_PARITY        0x20
#define XUN_LCR_EVEN_PARITY         0x10    /* 1 = even, 0 = odd parity */
#define XUN_LCR_ENABLE_PARITY       0x08
#define XUN_LCR_2_STOP_BITS         0x04    /* 1 = 2 stop bits,0 = 1 stop bit */
#define XUN_LCR_8_DATA_BITS         0x03
#define XUN_LCR_7_DATA_BITS         0x02
#define XUN_LCR_6_DATA_BITS         0x01
#define XUN_LCR_LENGTH_MASK         0x03    /* both length bits mask */
#define XUN_LCR_PARITY_MASK         0x18    /* both parity bits mask */

/* Modem Control Register bits */

#define XUN_MCR_LOOP                0x10    /* local loopback */
#define XUN_MCR_OUT_2               0x08    /* general output 2 signal */
#define XUN_MCR_OUT_1               0x04    /* general output 1 signal */
#define XUN_MCR_RTS                 0x02    /* RTS signal */
#define XUN_MCR_DTR                 0x01    /* DTR signal */

/* Line Status Register bits */

#define XUN_LSR_RX_FIFO_ERROR       0x80    /* an errored byte is in the FIFO */
#define XUN_LSR_TX_EMPTY            0x40    /* transmitter is empty */
#define XUN_LSR_TX_BUFFER_EMPTY     0x20    /* transmit holding reg empty */
#define XUN_LSR_BREAK_INT           0x10    /* break detected interrupt */
#define XUN_LSR_FRAMING_ERROR       0x08    /* framing error on current byte */
#define XUN_LSR_PARITY_ERROR        0x04    /* parity error on current byte */
#define XUN_LSR_OVERRUN_ERROR       0x02    /* overrun error on receive FIFO */
#define XUN_LSR_DATA_READY          0x01    /* receive data ready */
#define XUN_LSR_ERROR_BREAK         0x1E    /* errors except FIFO error and
                                               break detected */

#define XUN_DIVISOR_BYTE_MASK       0xFF

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/*****************************************************************************
*
* Low-level driver macros.  The list below provides signatures to help the
* user use the macros.
*
* Xuint8 XUartNs550_mReadReg(Xuint32 BaseAddress. int RegOffset)
* void XUartNs550_mWriteReg(Xuint32 BaseAddress, int RegOffset,
*                           Xuint8 RegisterValue)
*
* Xuint8 XUartNs550_mGetLineStatusReg(Xuint32 BaseAddress)
* Xuint8 XUartNs550_mGetLineControlReg(Xuint32 BaseAddress)
* void XUartNs550_mSetLineControlReg(Xuint32 BaseAddress, Xuint8 RegisterValue)
*
* void XUartNs550_mEnableIntr(Xuint32 BaseAddress)
* void XUartNs550_mDisableIntr(Xuint32 BaseAddress)
*
* Xboolean XUartNs550_mIsReceiveData(Xuint32 BaseAddress)
* Xboolean XUartNs550_mIsTransmitEmpty(Xuint32 BaseAddress)
*
*****************************************************************************/

/****************************************************************************/
/**
* Read a UART register.
*
* @param    BaseAddress contains the base address of the device.
* @param    RegOffset contains the offset from the 1st register of the device
*           to select the specific register.
*
* @return   The value read from the register.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mReadReg(BaseAddress, RegOffset) \
    XIo_In8((BaseAddress) + (RegOffset))

/****************************************************************************/
/**
* Write to a UART register.
*
* @param    BaseAddress contains the base address of the device.
* @param    RegOffset contains the offset from the 1st register of the device
*           to select the specific register.
*
* @return   The value read from the register.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mWriteReg(BaseAddress, RegOffset, RegisterValue) \
    XIo_Out8((BaseAddress) + (RegOffset), (RegisterValue))

/****************************************************************************/
/**
* Get the UART Line Status Register.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   The value read from the register.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mGetLineStatusReg(BaseAddress)   \
    XIo_In8((BaseAddress) + XUN_LSR_OFFSET)

/****************************************************************************/
/**
* Get the UART Line Status Register.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   The value read from the register.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mGetLineControlReg(BaseAddress)  \
    XIo_In8((BaseAddress) + XUN_LCR_OFFSET)

/****************************************************************************/
/**
* Set the UART Line Status Register.
*
* @param    BaseAddress contains the base address of the device.
* @param    RegisterValue is the value to be written to the register.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mSetLineControlReg(BaseAddress, RegisterValue) \
    XIo_Out8((BaseAddress) + XUN_LCR_OFFSET, (RegisterValue))

/****************************************************************************/
/**
* Enable the transmit and receive interrupts of the UART.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mEnableIntr(BaseAddress)                             \
    XIo_Out8((BaseAddress) + XUN_IER_OFFSET,                            \
             XIo_In8((BaseAddress) + XUN_IER_OFFSET) |                  \
             (XUN_IER_RX_LINE | XUN_IER_TX_EMPTY | XUN_IER_RX_DATA))

/****************************************************************************/
/**
* Disable the transmit and receive interrupts of the UART.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   None.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mDisableIntr(BaseAddress)                            \
    XIo_Out8((BaseAddress) + XUN_IER_OFFSET,                            \
             XIo_In8((BaseAddress) + XUN_IER_OFFSET) &                  \
             ~(XUN_IER_RX_LINE | XUN_IER_TX_EMPTY | XUN_IER_RX_DATA))

/****************************************************************************/
/**
* Determine if there is receive data in the receiver and/or FIFO.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   XTRUE if there is receive data, XFALSE otherwise.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mIsReceiveData(BaseAddress)                          \
    (XIo_In8((BaseAddress) + XUN_LSR_OFFSET) & XUN_LSR_DATA_READY)

/****************************************************************************/
/**
* Determine if a byte of data can be sent with the transmitter.
*
* @param    BaseAddress contains the base address of the device.
*
* @return   XTRUE if a byte can be sent, XFALSE otherwise.
*
* @note     None.
*
******************************************************************************/
#define XUartNs550_mIsTransmitEmpty(BaseAddress)                        \
    (XIo_In8((BaseAddress) + XUN_LSR_OFFSET) & XUN_LSR_TX_BUFFER_EMPTY)

/************************** Function Prototypes ******************************/

void XUartNs550_SendByte(Xuint32 BaseAddress, Xuint8 Data);

Xuint8 XUartNs550_RecvByte(Xuint32 BaseAddress);

void XUartNs550_SetBaud(Xuint32 BaseAddress, Xuint32 InputClockHz,
                        Xuint32 BaudRate);

/************************** Variable Definitions *****************************/

#ifdef __cplusplus
}
#endif

#endif            /* end of protection macro */

