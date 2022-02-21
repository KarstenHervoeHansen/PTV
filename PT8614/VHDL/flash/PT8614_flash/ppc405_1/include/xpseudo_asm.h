/* $Id: xpseudo_asm.h,v 1.2 2004/07/31 23:46:47 sathya Exp $ */
/******************************************************************************
*
* Copyright (c) 2004 Xilinx, Inc.  All rights reserved. 
* 
* Xilinx, Inc. 
* XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
* COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS 
* ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
* STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
* IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
* FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION. 
* XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
* THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
* ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
* FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
* AND FITNESS FOR A PARTICULAR PURPOSE.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xpseudo_asm.h
*
* This header file contains macros for using inline assembler code. 
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date     Changes
* ----- ---- -------- -----------------------------------------------
* 1.00a ch   06/18/02 First release
* </pre>
*
******************************************************************************/
#include "xreg405.h"
#ifdef __DCC__
#include "xpseudo_asm_dcc.h"
#else
#include "xpseudo_asm_gcc.h"
#endif
