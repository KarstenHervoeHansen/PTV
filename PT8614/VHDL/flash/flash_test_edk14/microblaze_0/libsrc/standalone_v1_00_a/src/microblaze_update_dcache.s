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
# File   : microblaze_update_dcache.s
# Date   : 2003, September 24
# Company: Xilinx
# Group  : Emerging Software Technologies
#
# Summary:
# Update dcache on the microblaze.
# Takes in three parameters
#	r5 : Cache Tag Line
#	r6 : Cache Data
#	r7 : Lock/Valid information
#		Bit 30 is Lock  [ 1 indicates locked ]
#		Bit 31 is Valid [ 1 indicates valid ]
#
#	--------------------------------------------------------------
#	|  Lock	 |     Valid  | Effect
#	--------------------------------------------------------------
#	|   0    |      0     | Invalidate Cache
#       |   0    |      1     | Valid, but unlocked cacheline
#       |   1    |      0     | Invalidate Cache, No effect of lock
#       |   1    |      1     | Valid cache. Locked to a 
#       |        |            | particular addrees
#	--------------------------------------------------------------
#
# $Id: microblaze_update_dcache.s,v 1.3 2004/08/17 05:01:59 sid Exp $
#
####################################################################
	
	.text
	.globl	microblaze_update_dcache
	.ent	microblaze_update_dcache
	.align	2
microblaze_update_dcache:

#Read the MSR register into a temp register
	mfs	r18, rmsr

#Clear the dcache enable bit to disable the cache
# Register r10,r18 are  volatile registers and hence do not need to be saved before use
	
	andi	r10, r18, ~128
	mts	rmsr, r10

# Update the lock and valid info
	andi	r5,r5,0xfffffffc
	or	r5,r5,r7
	
# Update dcache
	wdc	r5,r6

#Return
	rtsd	r15, 8

# Restore the MSR back
	mts	rmsr,r18
	
	.end	microblaze_update_dcache

	
  
