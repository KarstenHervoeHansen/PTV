/*-----------------------------------------------------------------------------
//     $Date: 2005/01/28 23:31:24 $
//     $RCSfile: ppc405_cache_init.h,v $
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

#ifndef PPC405_CACHE_INIT
#define PPC405_CACHE_INIT

#include <xpseudo_asm.h>

#define ppc405_init_cache() mtspr(XREG_SPR_CCR0, 0x50700000)

#endif            /* end of protection macro */
