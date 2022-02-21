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
# File   : microblaze_init_icache_range.s
# Date   : 2003, September 24
# Company: Xilinx
# Group  : Emerging Software Technologies
#
# Summary:
# Update icache on the microblaze.
# Takes in two parameters
#	r5 : Cache Base Address 
#	(start address in the cache where invalidation begins)
#	r6 : Size of Cache to be invalidated
#
# $Id: microblaze_init_icache_range.s,v 1.2 2004/07/31 23:46:47 sathya Exp $
#
####################################################################
	
	.text
	.globl	microblaze_init_icache_range
	.ent	microblaze_init_icache_range
	.align	2
microblaze_init_icache_range:

# Read the MSR register into a temp register
	mfs	r9, rmsr

# Clear the icache enable bit to disable the cache
# Register r10,r18 are  volatile registers and hence do not need to be saved before use
	andi	r10, r9, ~32
	mts	rmsr, r10

## Start the loop
	add	r6,r5,r6	# One address beyond last address
L_start:	
	cmp	r18,r5,r6
	bleid	r18, L_done     # Jump out of the loop if done
	addi	r6,r6,-4	# Decrement the addrees by 4 [ delay slot] 
	brid	L_start		# Branch to the beginning of the loop
	wic	r6,r0		# Invalidate the Cache [ delay slot]
L_done:	
# Return
	rtsd	r15, 8

# Restore the MSR back
	mts	rmsr,r9
	.end	microblaze_init_icache_range

	
  
