/*-----------------------------------------------------------------------------
//     $Date: 2005/03/29 22:47:49 $
//     $RCSfile: flash_test.c,v $
//-----------------------------------------------------------------------------
//
//     XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"
//     SOLELY FOR USE IN DEVELOPING PROGRAMS AND SOLUTIONS FOR
//     XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION
//     AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE, APPLICATION
//     OR STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS
//     IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,
//     AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE
//     FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY
//     WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE
//     IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR
//     REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF
//     INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//     FOR A PARTICULAR PURPOSE.
//     
//     (c) Copyright 2005 Xilinx, Inc.
//     All rights reserved.
//
//---------------------------------------------------------------------------*/

#include <stdio.h>
#include "xbasic_types.h"
#include "xstatus.h"
#include "xio.h"
#include "xuartns550_l.h"
#include "xparameters.h"
#include "xflash_intel_l.h"
#include "xgpio_l.h"
#if PPC
#include "ppc405_cache_init.h"
#endif

#define DEBUG 1
#define MAXERRORS 100
#define FLASH_BASEADDR 0x00000000
#define TESTRANGE 20
#if DEBUG
#define debug_print(x)  printf(x)
#define debug_putnum(x)  putnum(x)
#define debug_printch(x) XUartNs550_SendByte(UART_BASEADDR, (x))
#else
#define debug_print(x)
#define debug_putnum(x)
#define debug_printch(x)
#endif

#define UART_BASEADDR 0x40600000//XPAR_OPB_UART16550_0_BASEADDR
#define UART_CLOCK    148500000//XPAR_XUARTNS550_CLOCK_HZ

#if !SIM
#define UART_BAUDRATE 9600                      /* real hardware */
#else
#define UART_BAUDRATE (UART_CLOCK / 16 / 3)     /* simulation */
#endif

#define DEFAULT_MEMORYSIZE (32*1024*1024)   /* 8 x 32M bit devices */
#define BLOCKSIZE       128*1024              /* 128KB block /2 (x16mode) */
#define BLOCKSIZE32      32*1024
#define BUFSIZE         512*1024
#define ESCAPE          0x1b

#define MEMREG_ADDR      0x40020000
#define READYBIT_ADDR    0x40000000
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/

static Xuint32 Flash_Query (Xuint32 BaseAddress, Xuint32 offset)
{
  Xuint32 result;

  XFlashIntel_mSendCmd(FLASH_BASEADDR, 0x27, XFL_INTEL_CMD_READ_QUERY);

  /* The Flash is in x16 mode and there are two chips */
  offset <<= 1;  /* account for x16 mode */
  offset <<= 1;  /* to account for two chips */

  result = XIo_In32(BaseAddress+offset);

  XFlashIntel_mSendCmd(FLASH_BASEADDR, 0x27, XFL_INTEL_CMD_READ_ARRAY);

  return result;
}

/* Check flash size */
static Xuint32 GetFlashSize (Xuint32 BaseAddress)
{
  Xuint32 value;

  xil_printf("Querying Flash device size\n");

  value = Flash_Query(BaseAddress, 0x27);

  if ((value & 0x0FF) != ((value >> 16) & 0x0FF))
    {
      xil_printf("ERROR: Flash sizes for 2 chips don't match\n Flash query value = %X\n",value);
   //   return 0;
    }
  xil_printf("Flash query value = %X\n",value);
  switch (value & 0x0FF)
    {
    case 0x16:
      /* 32 Mbits x 2 chips */
      xil_printf("  Found 32 Mbit devices\n");
      return 32*1024*1024 / 8 * 2;
    case 0x17:
      /* 64 Mbits x 2 chips */
      xil_printf("  Found 64 Mbit devices\n");
      return 64*1024*1024 / 8;
    case 0x18:
      /* 128 Mbits x 2 chips */
      xil_printf("  Found 128 Mbit devices\n");
      return 128*1024*1024 / 8;
    }

  return DEFAULT_MEMORYSIZE;
}


/*---------------------------------------------------------------------------*/

static Xboolean Flash_Erase (Xuint32 BaseAddress, Xuint32 StartAddress,Xuint32 MidAddress,
                             Xuint32 EndAddress)
{
  XStatus status;
  Xboolean result = XFALSE;
  Xuint32 offset;

  xil_printf("Erasing from 0x%X to 0x%X (inclusive)\n",
             StartAddress, EndAddress + 3); /* last address extended to whole 32-bit word */

  if ((StartAddress & 3) || (EndAddress & 3))
    {
      xil_printf("Addresses must be word-aligned (multiples of 4 bytes)\n");
      xil_printf("Erase Test Failed\n");
      return XTRUE;
    }
    offset=0;
  /* Erase all blocks between StartAddress and EndAddress inclusive */
   for (offset=0; offset < MidAddress-StartAddress; offset += BLOCKSIZE)  //blocksize = 128kB
     {
		XFlashIntel_UnlockAddr(BaseAddress, offset);
			
      status = XFlashIntel_EraseAddr(BaseAddress, offset);
      if (status != XST_SUCCESS)
        {
          xil_printf("Failed to erase block containing address 0x%X, status = %X\n", BaseAddress + offset, status);
			 result = XTRUE;
        }
		else
		xil_printf("Erased!!  Block containing address 0x%X, status = %X\n", BaseAddress + offset, status);
     }
  /* Erase all blocks between StartAddress and EndAddress inclusive */
  ///xil_printf("midaddress
   for (; offset < EndAddress-StartAddress+4; offset += BLOCKSIZE32)  //blocksize = 128kB
     {
		XFlashIntel_UnlockAddr(BaseAddress, offset);
			
      status = XFlashIntel_EraseAddr(BaseAddress, offset);
      if (status != XST_SUCCESS)
        {
          xil_printf("Failed to erase block containing address 0x%X, status = %X\n", BaseAddress + offset, status);
			 result = XTRUE;
        }
		else
		xil_printf("Erased!!  Block containing address 0x%X, status = %X\n", BaseAddress + offset, status);
     }
  Xuint32 test = XIo_In16(BaseAddress+offset-BLOCKSIZE);
  xil_printf("last block address: 0x%X, data = 0x%X\n", BaseAddress + offset,test);
  return result;
}

// TestRamBusError currently doesn't do anything
static Xboolean TestRamBusError(void)
{
  Xuint32 val;

  val = 0;        /* replace this line and check the bus error registers */
  if (val != 0)
  {
    xil_printf("error; bus error\n");
    return XTRUE;
  }

  return XFALSE;
}

/*  if (TestRamWrite(BaseAddress, startAdr, (endAdr-startAdr)/4 + 1,
                   startAdr, 0x4, 4, &errors, maxErrors))*/

static Xboolean TestRamWrite
(
 Xuint32 BaseAddress,
 Xuint32 startAdr,
 Xuint32 size,
 Xuint16 startVal,
 Xint32 incr,  /* allow negative increment */
 Xuint8 tsize,
 Xuint32 *errors,
 Xuint32 maxerrors)
{
  int i;
  Xuint16 val;
  int status;
  if (maxerrors && (*errors >= maxerrors))
    return XFALSE;

  i = 0;
  while (i < size)
    {
		//xil_printf("Write data at address %X \n",  startAdr);
      val = 0;
      /*****
      for(j=0; j < tsize; j++)
        {
          val = (val << 8) | startVal;
          startVal += incr;
        }
      *****/
      val = startVal;
      startVal += incr;
      switch(tsize)
        {
        case 1:
          /**** *(volatile Xuint8 *) startAdr = (Xuint8) val; ***/
          XFlashIntel_WriteAddr(BaseAddress, startAdr-BaseAddress,
                                (Xuint8 *) &val, 1);
          startAdr++;
          break;
        case 2:
          /* *(volatile Xuint16 *) startAdr = (Xuint16) val; */
          status = XFlashIntel_WriteAddr(BaseAddress, startAdr-BaseAddress,
                                (Xuint8 *) &val, 2);
          startAdr += 2;
          break;
        case 4:
          /* *(volatile Xuint32 *) startAdr = val; */
          XFlashIntel_WriteAddr(BaseAddress, startAdr-BaseAddress,
                                (Xuint8 *) &val, 4);
          startAdr += 4;
          break;
        }
      i++;
    }
   xil_printf("Write done! Last address 0x%X, Last data: 0x%X\n",
             startAdr, val); 
  return TestRamBusError();
}


static Xboolean compError(Xuint32 adr, Xuint32 exp, Xuint32 got,
                          Xuint32 *errors, Xuint32 maxerrors)
{
  (*errors)++;
  xil_printf("error; adr: 0x%X, expected: 0x%X, got: 0x%X\n",
             adr, exp, got);

  return (maxerrors && (*errors >= maxerrors));
}

static Xboolean TestRamCompare
(
 Xuint32 BaseAddress,
 Xuint32 startAdr,
 Xuint32 size,
 Xuint16 startVal,
 Xint32 incr,  /* allow negative increment */
 Xuint8 tsize,
 Xuint32 *errors,
 Xuint32 maxerrors)
{
  Xuint32 i;
  Xuint16 val;
  Xuint32 getVal=0;
  Xuint32 curAdr;

  i = 0;
//  xil_printf("start address= %X, size = %X\n",startAdr,size);
  while (i < size)
    {
      val = 0;
      /*****
      for(j=0; j < tsize; j++)
        {
          val = (val << 8) | startVal;
          startVal += incr;
        }
      *****/
      val = startVal;
      startVal += incr;
      curAdr = startAdr;
      switch(tsize)
        {
        case 1:
          /* getVal = (Xuint32) *(volatile Xuint8 *) startAdr; */
          getVal = (Xuint32) XIo_In8(startAdr);
          startAdr++;
			 
          break;
        case 2:
          /* getVal = (Xuint32) *(volatile Xuint16 *) startAdr; */
          getVal = (Xuint32) XIo_In16(startAdr);
			// xil_printf("comp address= %X, data = %X\n",startAdr, getVal);
          startAdr += 2;		 
          break;
        case 4:
          /* getVal = *(volatile Xuint32 *) startAdr; */
          getVal = (Xuint32) XIo_In32(startAdr);
          startAdr += 4;
          break;
        }

      if (getVal != val)
        if (compError(curAdr, val, getVal, errors, maxerrors))
          return XTRUE;
     
	  i++;
    }
  xil_printf("Test flash done! Last test address= %X, data = %X\n",startAdr, getVal);
  return XFALSE;
}


static Xboolean TestRamWalkAddress (Xuint32 BaseAddress, Xuint32 startAdr,
                                    Xuint32 endAdr, Xboolean one,
                                    Xuint32 *errors,
                                    Xuint32 maxErrors)
{
  unsigned int i;
  Xuint16 bitVal;
  Xuint32 numAddrBits = 0;
  Xuint32 memSize = (endAdr - startAdr + 4) / 2;  /* divide since 2 chips */

  if (maxErrors && (*errors >= maxErrors))
    return XFALSE;

  /* Determine number of address lines to exercise */
  for (i=1; i<=32; i++)
    {
      if (memSize & 0x1)
        numAddrBits = i;
      memSize >>= 1;
    }

  if (one)
    bitVal = 0x1;
  else
    bitVal = ~0x1;

  /* Don't bother testing byte and half-word address since that
     isn't external to FPGA. */

  i = 2;
  bitVal <<= 2;
  xil_printf("  (tests address bits %d to 2)\n", numAddrBits-1);
  while (i < numAddrBits)
  {
    if(TestRamWrite(BaseAddress, startAdr+bitVal, 1, bitVal, 0x0, 2,
                    errors, maxErrors))
      return XTRUE;

    /* compare */
    if (TestRamCompare(BaseAddress, startAdr+bitVal, 1, bitVal, 0x0, 2,
                       errors, maxErrors))
      return XTRUE;

    if (maxErrors && (*errors >= maxErrors))
      return XFALSE;

    i++;
    bitVal = bitVal << 1;
  }

  return XFALSE;
}

static Xboolean TestRamWalkByte (Xuint32 BaseAddress, Xuint32 startAdr,
                                 Xuint32 size, Xboolean one,
                                 Xuint32 *errors,
                                 Xuint32 maxErrors)
{
  unsigned int i;
  Xuint16 bitVal;

  if (maxErrors && (*errors >= maxErrors))
    return XFALSE;

  if (one)
    bitVal = 0x1;
  else
    bitVal = ~0x1;

  i = 0;
  while (i < size * 8)
  {
    if(TestRamWrite(BaseAddress, startAdr, 1, bitVal, 0x0, 2,
                    errors, maxErrors))
      return XTRUE;

    /* compare */

    if (TestRamCompare(BaseAddress, startAdr, 1, bitVal, 0x0, 2,
                       errors, maxErrors))
      return XTRUE;

    if (maxErrors && (*errors >= maxErrors))
      return XFALSE;

    i++;
    bitVal = bitVal << 1;
    startAdr += 2;
  }

  return XFALSE;
}

static Xboolean TestErase (Xuint32 BaseAddress, Xuint32 StartAddress,
                           Xuint32 MidAddress,Xuint32 EndAddress, Xuint32 *errors,
                           Xuint32 maxErrors)
{
   if (Flash_Erase(BaseAddress, StartAddress, MidAddress,EndAddress)) {
    // Erase failure...we'll just set errors to max to dump out of test
    *errors = maxErrors;
    return XTRUE;
  }
  if (TestRamCompare(BaseAddress, StartAddress, (EndAddress-StartAddress)/2+1,
                     0xffff, 0x0, 2, errors, maxErrors))
    return XTRUE;

  return XFALSE;
}

/*-----------------------------------------------------------------------------------------
 A test procedure to check if a certain block in the flash is locked and unlocks 
 it and afterwards erases the flash.
-----------------------------------------------------------------------------------------*/
int mytest()
{
	Xuint32 status;
	Xuint32 flashSize = 32*1024*1024;
	
	 XFlashIntel_mSendCmd(FLASH_BASEADDR+128*1024, 0x04,  0x90);
	 Xuint32 lockingStatus = XIo_In16(FLASH_BASEADDR+ 128*1024+ 0x04);	 
	 xil_printf("Locking register is %X\n",lockingStatus);
	
	XFlashIntel_UnlockAddr(FLASH_BASEADDR+128*1024, 0);
   XFlashIntel_mSendCmd(FLASH_BASEADDR, 128*1024, XFL_INTEL_CMD_BLOCK_ERASE);
   XFlashIntel_mSendCmd(FLASH_BASEADDR, 128*1024, XFL_INTEL_CMD_CONFIRM);
	do
	{
		status = XIo_In16(FLASH_BASEADDR+128*1024);
	}
	while ((status & XFL_INTEL_STATUS_READY) != XFL_INTEL_STATUS_READY);
	
	xil_printf("status = %X\n", status);
	return 0;
}

/*----------------------------------------------------------------------
small test procedure to display the data in a certain address in flash
----------------------------------------------------------------------*/
int mytest2()
{
    Xuint16 val;
	Xuint32 flashaddr = 0;
	do
	{
		val = XIo_In16(flashaddr);
		flashaddr += 2;
		xil_printf("Flash read data: %X at address %X \n", val, flashaddr);
	}
	while(flashaddr < 20);
	
	return 0;

}

/*------------------------------------------------------------------------------------------------------------------ 
This procedure tests if the PPC and FPGA can communicate via two shared registers. One register contains
a ready/ack bit which makes a simple async commnunication protocol. This bit is set by the PPC telling the data is
ready and cleared by the FPGA telling the data has been received. The other register is for storing the data. 
The data comes from flash and will be read by FPGA.
------------------------------------------------------------------------------------------------------------------*/
static int mytest3()
{
   Xuint16 val;
	Xuint32 flashaddr = 0;
	Xuint32 temp_reg;
   Xuint8 ready;
	Xuint32 startAddress = 0;
	Xuint32 endAddress = 20;
	
	flashaddr = startAddress;
	
   while(flashaddr < endAddress)
	{
		do
		{
			ready = XGpio_mGetDataReg(READYBIT_ADDR, 1);    // ready/ack bit. set by PPC when data ready
		}                                                   // and cleared by FPGA when it reads in data
		while(ready != 0);
		
	   xil_printf("Flash read at address %X\n",flashaddr);
		
		val = XIo_In16(flashaddr);      // read in first 16 bits
		flashaddr += 2;                 // increase flash address (1 address contains 8 bits)
		temp_reg = val;
		temp_reg = temp_reg << 16;
		val = XIo_In16(flashaddr);      // read in second 16 bits
		flashaddr += 2;                 // increase flash address
		temp_reg = temp_reg + val;      // combine two words into 32 bits
	
		XGpio_mSetDataReg(MEMREG_ADDR, 1, temp_reg);    //send data to FPGA by writing it to a register
		XGpio_mSetDataReg(READYBIT_ADDR, 1, 1);         // set the ready bit 
	}
	return 0; 
}

int main()
{
  Xuint32 flashSize;
  Xint32 result;
/*
#ifdef PPC
#ifndef CACHESOFF
  // Enable Caches
  ppc405_init_cache();
  XCache_EnableICache(0x8C000001);
  XCache_EnableDCache(0x88000001);
#endif
#endif
*/
//  XUartNs550_SetBaud(UART_BASEADDR, UART_CLOCK, UART_BAUDRATE);
//  XUartNs550_mSetLineControlReg(UART_BASEADDR, XUN_LCR_8_DATA_BITS);

  xil_printf("Xilinx Flash Test\n");
  xil_printf("=================\n");

 flashSize = GetFlashSize(FLASH_BASEADDR);
  if (flashSize == 0)
    return 1;

  xil_printf("Testing Flash addresses %08X to %08X\n", FLASH_BASEADDR,
             FLASH_BASEADDR+flashSize-1);

		
  result = mytest3();
  xil_printf("FLASH TEST TOTAL ERRORS: %d\n", result);
  xil_printf("========================================\n");

  return 0;
}
