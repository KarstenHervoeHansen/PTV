/*
 *  * Copyright (c) 2004 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/*
 * Xilinx EDK 8.1 EDK_I.18.7
 *
 * This file is a sample test application
 *
 * This application is intended to test and/or illustrate some 
 * functionality of your system.  The contents of this file may
 * vary depending on the IP in your system and may use existing
 * IP driver functions.  These drivers will be generated in your
 * XPS project when you run the "Generate Libraries" menu item
 * in XPS.
 *
 * Your XPS project directory is at:
 *    C:\MyProject\flash_test_edk14\
 */


// Located in: microblaze_0/include/xparameters.h
#include "xparameters.h"

#include "stdio.h"

#include "xutil.h"

//====================================================

int main (void) {


   print("-- Entering main() --\r\n");

   /* 
    * MemoryTest routine will not be run for the memory at 
    * 0x00000000 (dlmb_cntlr)
    * because it is being used to hold a part of this application program
    */


   /* Testing EMC Memory (Generic_External_Memory)*/
   {
      XStatus status;

      print("Starting MemoryTest for Generic_External_Memory:\r\n");
      print("  Running 32-bit test...");
      status = XUtil_MemoryTest32((Xuint32*)XPAR_GENERIC_EXTERNAL_MEMORY_MEM0_BASEADDR, 1024, 0xAAAA5555, XUT_ALLMEMTESTS);
      if (status == XST_SUCCESS) {
         print("PASSED!\r\n");
      }
      else {
         print("FAILED!\r\n");
      }
      print("  Running 16-bit test...");
      status = XUtil_MemoryTest16((Xuint16*)XPAR_GENERIC_EXTERNAL_MEMORY_MEM0_BASEADDR, 2048, 0xAA55, XUT_ALLMEMTESTS);
      if (status == XST_SUCCESS) {
         print("PASSED!\r\n");
      }
      else {
         print("FAILED!\r\n");
      }
      print("  Running 8-bit test...");
      status = XUtil_MemoryTest8((Xuint8*)XPAR_GENERIC_EXTERNAL_MEMORY_MEM0_BASEADDR, 4096, 0xA5, XUT_ALLMEMTESTS);
      if (status == XST_SUCCESS) {
         print("PASSED!\r\n");
      }
      else {
         print("FAILED!\r\n");
      }
   }

   /* Testing BRAM Memory (opb_bram_if_cntlr_1)*/
   {
      XStatus status;

      print("Starting MemoryTest for opb_bram_if_cntlr_1:\r\n");
      print("  Running 32-bit test...");
      status = XUtil_MemoryTest32((Xuint32*)XPAR_OPB_BRAM_IF_CNTLR_1_BASEADDR, 512, 0xAAAA5555, XUT_ALLMEMTESTS);
      if (status == XST_SUCCESS) {
         print("PASSED!\r\n");
      }
      else {
         print("FAILED!\r\n");
      }
      print("  Running 16-bit test...");
      status = XUtil_MemoryTest16((Xuint16*)XPAR_OPB_BRAM_IF_CNTLR_1_BASEADDR, 1024, 0xAA55, XUT_ALLMEMTESTS);
      if (status == XST_SUCCESS) {
         print("PASSED!\r\n");
      }
      else {
         print("FAILED!\r\n");
      }
      print("  Running 8-bit test...");
      status = XUtil_MemoryTest8((Xuint8*)XPAR_OPB_BRAM_IF_CNTLR_1_BASEADDR, 2048, 0xA5, XUT_ALLMEMTESTS);
      if (status == XST_SUCCESS) {
         print("PASSED!\r\n");
      }
      else {
         print("FAILED!\r\n");
      }
   }

   print("-- Exiting main() --\r\n");
   return 0;
}

