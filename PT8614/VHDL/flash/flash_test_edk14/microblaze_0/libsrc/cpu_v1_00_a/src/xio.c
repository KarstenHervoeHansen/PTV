/* $Id: xio.c,v 1.4 2003/11/07 21:40:42 moleres Exp $ */
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
*       (c) Copyright 2002-2003 Xilinx Inc.
*       All rights reserved.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xio.c
*
* Contains I/O functions for memory-mapped or non-memory-mapped I/O
* architectures.  These functions encapsulate generic CPU I/O requirements.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a rpm  11/07/03 Added InSwap/OutSwap routines for endian conversion
* </pre>
*
* @note
*
* This file may contain architecture-dependent code.
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "xio.h"
#include "xbasic_types.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/*****************************************************************************/
/**
*
* Performs a 16-bit endian converion.
*
* @param    Source contains the value to be converted.
* @param    DestPtr contains a pointer to the location to put the
*           converted value.
*
* @return
*
* None.
*
* @note
*
* None.
*
******************************************************************************/
void XIo_EndianSwap16(Xuint16 Source, Xuint16* DestPtr)
{
    *DestPtr = (Xuint16)(((Source & 0xFF00) >> 8) | ((Source & 0x00FF) << 8));
}

/*****************************************************************************/
/**
*
* Performs a 32-bit endian converion.
*
* @param    Source contains the value to be converted.
* @param    DestPtr contains a pointer to the location to put the
*           converted value.
*
* @return
*
* None.
*
* @note
*
* None.
*
******************************************************************************/
void XIo_EndianSwap32(Xuint32 Source, Xuint32* DestPtr)
{
    /* get each of the half words from the 32 bit word */

    Xuint16 LoWord = (Xuint16)(Source & 0x0000FFFF);
    Xuint16 HiWord = (Xuint16)((Source & 0xFFFF0000) >> 16);

    /* byte swap each of the 16 bit half words */

    LoWord = (((LoWord & 0xFF00) >> 8) | ((LoWord & 0x00FF) << 8));
    HiWord = (((HiWord & 0xFF00) >> 8) | ((HiWord & 0x00FF) << 8));

    /* swap the half words before returning the value */

    *DestPtr = (Xuint32)((LoWord << 16) | HiWord);
}

/*****************************************************************************/
/**
*
* Performs an input operation for a 16-bit memory location by reading from the
* specified address and returning the byte-swapped value read from that
* address.
*
* @param    InAddress contains the address to perform the input operation at.
*
* @return
*
* The byte-swapped value read from the specified input address.
*
* @note
*
* None.
*
******************************************************************************/
Xuint16 XIo_InSwap16(XIo_Address InAddress)
{
    Xuint16 InData;
    
    /* get the data then swap it */
    InData = XIo_In16(InAddress);
    
    return (Xuint16)(((InData & 0xFF00) >> 8) | ((InData & 0x00FF) << 8));
}

/*****************************************************************************/
/**
*
* Performs an input operation for a 32-bit memory location by reading from the
* specified address and returning the byte-swapped value read from that
* address.
*
* @param    InAddress contains the address to perform the input operation at.
*
* @return
*
* The byte-swapped value read from the specified input address.
*
* @note
*
* None.
*
******************************************************************************/
Xuint32 XIo_InSwap32(XIo_Address InAddress)
{
    Xuint32 InData;
    Xuint32 SwapData;
    
    /* get the data then swap it */
    InData = XIo_In32(InAddress);
    XIo_EndianSwap32(InData, &SwapData);
    
    return SwapData;
}

/*****************************************************************************/
/**
*
* Performs an output operation for a 16-bit memory location by writing the
* specified value to the the specified address. The value is byte-swapped
* before being written.
*
* @param    OutAddress contains the address to perform the output operation at.
* @param    Value contains the value to be output at the specified address.
*
* @return
*
* None.
*
* @note
*
* None.
*
******************************************************************************/
void XIo_OutSwap16(XIo_Address OutAddress, Xuint16 Value)
{
    Xuint16 OutData;
    
    /* swap the data then output it */
    OutData = (Xuint16)(((Value & 0xFF00) >> 8) | ((Value & 0x00FF) << 8));
    
    XIo_Out16(OutAddress, OutData);
}

/*****************************************************************************/
/**
*
* Performs an output operation for a 32-bit memory location by writing the
* specified value to the the specified address. The value is byte-swapped
* before being written.
*
* @param    OutAddress contains the address to perform the output operation at.
* @param    Value contains the value to be output at the specified address.
*
* @return
*
* None.
*
* @note
*
* None.
*
******************************************************************************/
void XIo_OutSwap32(XIo_Address OutAddress, Xuint32 Value)
{
    Xuint32 OutData;
    
    /* swap the data then output it */
    XIo_EndianSwap32(Value, &OutData);
    XIo_Out32(OutAddress, OutData);
}

