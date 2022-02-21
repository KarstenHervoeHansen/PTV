######################################################################
# Copyright (c) 2004 Xilinx, Inc.  All rights reserved. 
# 
# Xilinx, Inc. 
# XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A 
# COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS 
# ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR 
# STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION 
# IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE 
# FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION. 
# XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO 
# THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO 
# ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE 
# FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY 
# AND FITNESS FOR A PARTICULAR PURPOSE. 
# 
# File   : microblaze_disable_dcache.s
# Date   : 2002, March 20.
# Company: Xilinx
# Group  : Emerging Software Technologies
#
# Summary:
# Disable dcache on the microblaze.
#
# $Id: microblaze_disable_dcache.s,v 1.2 2004/07/31 23:46:47 sathya Exp $
#
####################################################################
	
	.text
	.globl	microblaze_disable_dcache
	.ent	microblaze_disable_dcache
	.align	2
microblaze_disable_dcache:	
	#Make space on stack for a temporary
	addi	r1, r1, -4
	#Save register r12
	swi	r12, r1, 0
	#Read the MSR register
	mfs	r12, rmsr
	#Clear the dcache enable bit
	andi	r12, r12, ~128
	#Save the MSR register
	mts	rmsr, r12
	#Load register r12
	lwi	r12, r1, 0
	#Return
	rtsd	r15, 8
	#Update stack in the delay slot
	addi	r1, r1, 4
	.end	microblaze_disable_dcache

	
  
