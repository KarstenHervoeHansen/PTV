/* $Id: xflash_intel_l.c,v 1.2 2004/11/01 19:50:45 robertm Exp $ */
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
* @file xflash_intel_l.c
*
* Contains low-level functions for the XFlashIntel driver.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a rpm  05/06/02 First release
* 1.00a rmm  11/01/04 CR 196154: Fixed BaseAddress alignment check in functions
*                     XFlashIntel_WriteAddr() and XFlashIntel_ReadAddr().
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xflash_intel_l.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Variable Definitions *****************************/

/************************** Function Prototypes ******************************/

static int SendCmdSeq(Xuint32 BaseAddress, Xuint32 Offset,
                      unsigned int Cmd1, unsigned int Cmd2);
static int WriteWord(Xuint32 BaseAddress, Xuint32 Offset, Xuint8 *BufPtr);
static void ReadWord(Xuint32 BaseAddress, Xuint32 Offset, Xuint8 *BufPtr);


/*****************************************************************************/
/**
*
* Write the specified address with some number of bytes.
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address to begin writing.
* @param BufferPtr is the buffer that will be written to flash.
* @param Length is the number of bytes in BufferPtr that will be written to
*        flash.
*
* @return
*
* The number of bytes actually written.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
int XFlashIntel_WriteAddr(Xuint32 BaseAddress, Xuint32 Offset,
                          Xuint8 *BufferPtr, unsigned int Length)
{
    unsigned int i;
    int status;
    /*
     * Implementation restrictions:
     * - address must be aligned to a word
     * - length must be a multiple of a word
     */

   /* if ((Offset & 3) || (BaseAddress & 3))
    {
        return 0;
    }*/

    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_CLEAR_STATUS_REG);
    for (i = 0; i < Length; i += 4)
    {
		  status = WriteWord(BaseAddress, Offset + i, &BufferPtr[i]);
        if ( status != 0)
        {
            xil_printf("write failure at %X, return status = %d\n", BaseAddress+Offset+i,status);
				return i;
        }
    }
    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_READ_ARRAY);

    return Length;
}

/*****************************************************************************/
/**
*
* Read some number of bytes from the specified address.
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address to begin reading.
* @param BufferPtr is the buffer used to store the bytes that are read.
* @param Length is the number of bytes to read from flash.
*
* @return
*
* The number of bytes actually read.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
int XFlashIntel_ReadAddr(Xuint32 BaseAddress, Xuint32 Offset,
                         Xuint8 *BufferPtr, unsigned int Length)
{
    unsigned int i;

    /*
     * Implementation restrictions:
     * - address must be aligned to a word
     * - length must be a multiple of a word
     */
    if ((Offset & 3) || (BaseAddress & 3))
    {
        return -1;
    }

    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_CLEAR_STATUS_REG);
    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_READ_ARRAY);
    for (i = 0; i < Length; i += 4)
    {
        ReadWord(BaseAddress, Offset + i, &BufferPtr[i]);
    }

    return Length;
}

/*****************************************************************************/
/**
*
* Erase the block beginning at the specified address. The user is assumed to
* know the block boundaries and pass in an address/offset that is block
* aligned.
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address to begin erasing.
*        This offset is assumed to be a block boundary.
*
* @return
*
* 0 if successful, or -1 if an error occurred.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
int XFlashIntel_EraseAddr(Xuint32 BaseAddress, Xuint32 Offset)
{
    return SendCmdSeq(BaseAddress, Offset, XFL_INTEL_CMD_BLOCK_ERASE,
                      XFL_INTEL_CMD_CONFIRM);
}

/*****************************************************************************/
/**
*
* Lock the block beginning at the specified address. The user is assumed to
* know the block boundaries and pass in an address/offset that is block
* aligned.
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address to lock.
*        This offset is assumed to be a block boundary.
*
* @return
*
* 0 if successful, or -1 if an error occurred.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
int XFlashIntel_LockAddr(Xuint32 BaseAddress, Xuint32 Offset)
{
    return SendCmdSeq(BaseAddress, Offset, XFL_INTEL_CMD_LOCK_BLOCK_SET,
                      XFL_INTEL_CMD_LOCK_BLOCK_SET_CONFIRM);
}

/*****************************************************************************/
/**
*
* Unlock the block beginning at the specified address. The user is assumed to
* know the block boundaries and pass in an address/offset that is block
* aligned.
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address to unlock.
*        This offset is assumed to be a block boundary.
*
* @return
*
* 0 if successful, or -1 if an error occurred.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
int XFlashIntel_UnlockAddr(Xuint32 BaseAddress, Xuint32 Offset)
{
    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_LOCK_BLOCK_CLEAR);
    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_CONFIRM);
//    XFlashIntel_mSendCmd(BaseAddress, 0x04,  XFL_INTEL_CMD_READ_ID_CODES);
//	 Xuint32 lockingStatus = XIo_In16(BaseAddress+ 0x04);
	 
//	 xil_printf("Locking register is %X\n",lockingStatus);
	 
    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_READ_ARRAY);


    return 0;
}

/*****************************************************************************/
/**
*
* Wait for the flash array to be in the ready state (ready for a command).
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address.
*
* @return
*
* 0 if successful, or -1 if an error occurred.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
int XFlashIntel_WaitReady(Xuint32 BaseAddress, Xuint32 Offset)
{
    unsigned int Status;

    /*
     * Query the device until its status indicates that it is ready
     */

	  do
    {
        Status = XIo_In16(BaseAddress + Offset);
    }
    while (((Status & XFL_INTEL_STATUS_READY) != XFL_INTEL_STATUS_READY));

    if (Status != XFL_INTEL_STATUS_READY)
    {
        /* programming error */
        XFlashIntel_mSendCmd(BaseAddress, Offset,
                             XFL_INTEL_CMD_CLEAR_STATUS_REG);
        XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_READ_ARRAY);
        return Status;//-1;  //xia debug
    }

    return 0;
}

/*****************************************************************************/
/**
*
* Send a command sequence of two commands, then wait for the device to be in
* a ready state.
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address.
* @param Cmd1 is the first command to send.
* @param Cmd2 is the second command to send
*
* @return
*
* 0 if successful, or -1 if an error occurred.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
static int SendCmdSeq(Xuint32 BaseAddress, Xuint32 Offset,
                      unsigned int Cmd1, unsigned int Cmd2)
{
    int Result;

    XFlashIntel_mSendCmd(BaseAddress, Offset, Cmd1);
    XFlashIntel_mSendCmd(BaseAddress, Offset, Cmd2);
    Result = XFlashIntel_WaitReady(BaseAddress, Offset);

    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_READ_ARRAY);

    return Result;
}

/*****************************************************************************/
/**
*
* Write one word (32-bits) of data to the flash array.
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address to begin writing.
* @param BufPtr is a pointer to the four bytes of data to be written.
*
* @return
*
* 0 if successful, or -1 if an error occurred.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
static int WriteWord(Xuint32 BaseAddress, Xuint32 Offset, Xuint8 *BufPtr)
{
    Xuint16 DataWord;
    int i;

    DataWord = 0;
    for (i = 0; i < 2; i++)
    {
        DataWord = (DataWord << 8) | (unsigned int)BufPtr[i];
    }

    XFlashIntel_mSendCmd(BaseAddress, Offset, XFL_INTEL_CMD_PROGRAM);
    XIo_Out16(BaseAddress + Offset, DataWord);
    //xil_printf("Write data %X \n",  DataWord);
    return XFlashIntel_WaitReady(BaseAddress, Offset);
}

/*****************************************************************************/
/**
*
* Read one word (32-bits) of data from the flash array.
*
* @param BaseAddress is the base address of the device.
* @param Offset is the offset address from the base address to begin writing.
* @param BufPtr is a pointer to storage for the the four bytes of data to be
*        read.
*
* @return
*
* None.
*
* @note
*
* This function assumes 32-bit access to the flash array.
*
******************************************************************************/
static void ReadWord(Xuint32 BaseAddress, Xuint32 Offset, Xuint8 *BufPtr)
{
    Xuint32 DataWord;
    int i, j;

    DataWord = XIo_In16(BaseAddress + Offset);

    for (i=3, j=0; i >= 0; i--, j++)
    {
        BufPtr[j] = (Xuint8)(DataWord >> (i * 8));
    }
}

