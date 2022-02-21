/* $Id: xio.c,v 1.12 2005/03/09 20:13:59 meinelte Exp $ */
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
* @file xio.c
*
* Contains I/O functions for memory-mapped or non-memory-mapped I/O
* architectures.  These functions encapsulate PowerPC architecture-specific
* I/O requirements.
*
* @note
*
* This file contains architecture-dependent code.
*
* The order of the SYNCHRONIZE_IO and the read or write operation is
* important. For the Read operation, all I/O needs to complete prior
* to the desired read to insure valid data from the address. The PPC
* is a weakly ordered I/O model and reads can and will occur prior
* to writes and the SYNCHRONIZE_IO ensures that any writes occur prior
* to the read. For the Write operation the SYNCHRONIZE_IO occurs
* after the desired write to ensure that the address is updated with
* the new value prior to any subsequent read.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- --------------------------------------------------------
* 1.00a ecm  03/18/04 Changed the XIo_OutXX inline assembly functions from
*                     using 'r' as the constraint for the OutAddress to 'b'
*                     to eliminate R0 from the register choices for the
*                     compiler.
*
* 1.00a ecm  03/09/05 Changed the XIo_InXX and _Swap inline assembly functions
*                     from using 'r' as the constraint for the InAddress to 'b'
*                     to eliminate R0 from the register choices for the
*                     compiler, new GCC seems to allow it (GCC 3.3.4 now, was
*                     2.95 based previously).
* </pre>
******************************************************************************/


/***************************** Include Files *********************************/
#include "xio.h"
#include "xbasic_types.h"

/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/
#if defined __DCC__
asm volatile Xuint16 InSwap16(XIo_Address InAddress)
{
% reg InAddress
! "r3"

    eieio
    lhbrx r3,0,InAddress
}

asm volatile Xuint32 InSwap32(XIo_Address InAddress)
{
% reg InAddress
! "r3"

    eieio
    lwbrx r3,0,InAddress
}

asm volatile void OutSwap16(XIo_Address OutAddress, Xuint16 Value)
{
% reg OutAddress; reg Value

    sthbrx Value,0,OutAddress
    eieio
}

asm volatile void OutSwap32(XIo_Address OutAddress, Xuint32 Value)
{
% reg OutAddress; reg Value

    stwbrx Value,0,OutAddress
    eieio
}

#endif

/************************** Function Prototypes ******************************/


/*****************************************************************************/
/**
*
* Performs an input operation for an 8-bit memory location by reading from the
* specified address and returning the value read from that address.
*
* @param    InAddress contains the address to perform the input operation at.
*
* @return
*
* The value read from the specified input address.
*
* @note
*
* None.
*
******************************************************************************/
Xuint8 XIo_In8(XIo_Address InAddress)
{
    /* read the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

#if defined __GNUC__

    Xuint8 IoContents;
    __asm__ volatile ("eieio; lbz %0,0(%1)" : "=r" (IoContents): "b" (InAddress));
    return IoContents;

#else

    SYNCHRONIZE_IO;
    return *(Xuint8*)InAddress;

#endif

}

/*****************************************************************************/
/**
*
* Performs an input operation for a 16-bit memory location by reading from the
* specified address and returning the value read from that address.
*
* @param    InAddress contains the address to perform the input operation at.
*
* @return
*
* The value read from the specified input address.
*
* @note
*
* None.
*
******************************************************************************/
Xuint16 XIo_In16(XIo_Address InAddress)
{
    /* read the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

#if defined __GNUC__

    Xuint16 IoContents;
    __asm__ volatile ("eieio; lhz %0,0(%1)" : "=r" (IoContents): "b" (InAddress));
    return IoContents;

#else

    SYNCHRONIZE_IO;
    return *(Xuint16*)InAddress;

#endif
}

/*****************************************************************************/
/**
*
* Performs an input operation for a 32-bit memory location by reading from the
* specified address and returning the value read from that address.
*
* @param    InAddress contains the address to perform the input operation at.
*
* @return
*
* The value read from the specified input address.
*
* @note
*
* None.
*
******************************************************************************/
Xuint32 XIo_In32(XIo_Address InAddress)
{
    /* read the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

#ifdef __GNUC__

    Xuint32 IoContents;
    __asm__ volatile ("eieio; lwz %0,0(%1)" : "=r" (IoContents): "b" (InAddress));
    return IoContents;

#else

    SYNCHRONIZE_IO;
    return *(Xuint32*)InAddress;

#endif

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
    /* read the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */
#ifdef __GNUC__
    Xuint16 IoContents;

    __asm__ volatile ("eieio; lhbrx %0,0,%1": "=r" (IoContents) : "b" (InAddress));
    return IoContents;
#else
    return InSwap16(InAddress);
#endif
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
    /* read the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */
#ifdef __GNUC__
    Xuint32 IoContents;

    __asm__ volatile ("eieio; lwbrx %0,0,%1": "=r" (IoContents) : "b" (InAddress));
    return IoContents;
#else
    return InSwap32(InAddress);
#endif

}


/*****************************************************************************/
/**
*
* Performs an output operation for an 8-bit memory location by writing the
* specified value to the the specified address.
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
void XIo_Out8(XIo_Address OutAddress, Xuint8 Value)
{
    /* write the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

#ifdef __GNUC__

   __asm__ volatile ("stb %0,0(%1); eieio" : : "r" (Value), "b" (OutAddress));

#else

   *(volatile Xuint8*)OutAddress = Value;
   SYNCHRONIZE_IO;

#endif

}

/*****************************************************************************/
/**
*
* Performs an output operation for a 16-bit memory location by writing the
* specified value to the the specified address.
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
void XIo_Out16(XIo_Address OutAddress, Xuint16 Value)
{
    /* write the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

#ifdef __GNUC__

   __asm__ volatile ("sth %0,0(%1); eieio" : : "r" (Value), "b" (OutAddress));

#else

   *(volatile Xuint16*)OutAddress = Value;
   SYNCHRONIZE_IO;

#endif
}

/*****************************************************************************/
/**
*
* Performs an output operation for a 32-bit memory location by writing the
* specified value to the the specified address.
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
void XIo_Out32(XIo_Address OutAddress, Xuint32 Value)
{
    /* write the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */

#ifdef __GNUC__

   __asm__ volatile ("stw %0,0(%1); eieio" : : "r" (Value), "b" (OutAddress));

#else

   *(volatile Xuint32*)OutAddress = Value;
   SYNCHRONIZE_IO;

#endif
}

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
void XIo_EndianSwap16OLD(Xuint16 Source, Xuint16* DestPtr)
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
void XIo_EndianSwap32OLD(Xuint32 Source, Xuint32* DestPtr)
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
    /* write the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */
#ifdef __GNUC__
    __asm__ volatile ("sthbrx %0,0,%1; eieio": : "r" (Value), "b" (OutAddress));
#else
    OutSwap16(OutAddress, Value);
#endif
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
    /* write the contents of the I/O location and then synchronize the I/O
     * such that the I/O operation completes before proceeding on
     */
#ifdef __GNUC__
    __asm__ volatile ("stwbrx %0,0,%1; eieio": : "r" (Value), "b" (OutAddress));
#else
    OutSwap32(OutAddress, Value);
#endif
}

