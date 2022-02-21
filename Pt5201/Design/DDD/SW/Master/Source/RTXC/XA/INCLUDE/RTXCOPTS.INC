; rtxcopts.inc - RTXC assembly options 8051XA

;
;   RTXC    
;   Copyright (c) 
;   Embedded System Products, Inc.
;   ALL RIGHTS RESERVED
;
;============================================================================

;-------------------------------------
; KERNEL priority level
;    NOTE: RTXCINTLVL must match level in RTXCOPTS.H
;-------------------------------------
RTXCINTLVL    equ   0Dh

;-------------------------------------
; KERNEL Trap number
;    NOTE:
;-------------------------------------
RTXC_TRAPNUM  equ    0h

;-------------------------------------
; RTXC stack size
;    NOTE: RTXCSTKSZ must match definition in RTXCOPTS.H
;-------------------------------------
RTXCSTKSZ    equ   256; can be changed as needed, min = approx 256 bytes

;-------------------------------------
; Nulltask's stack size
;-------------------------------------
NULLSTKSZ    equ   128; Size of null stack, min = 128

;===========================================================================
; This switch is to define 12 bytes of kernel data into the first 1k bytes
; of RAM.  Having this switch defined to _near allows the kernel to execute
; considerably faster at a minimal cost. If this data is not desired to be
; in the 1k bytes, then define CORE_IRAMDATA to zero (0).
;                1 = YES,  0 = NO
;---------------------------------------------------------------------------
%DEFINE(CORE_IRAMDATA)(1)
;===========================================================================

;===========================================================================
; This allows the kernel in the LARGE memory model to access the stacks as
; 16 bit data.
;
;NOTE1: This switch must match the setting of the switch of the same name
;       in the RTXCOPTS.H module.
;
;NOTE2: This switch is supported only in the Tasking LARGE memory model.
;                1 = YES,  0 = NO
;---------------------------------------------------------------------------
%DEFINE(_KS_STK_NEAR)(0)
;===========================================================================

;****************************************************************************
; Do not modify anything below this line
;---------------------------------------------------------------------------
%IF(%EQS(%_MODEL,m) OR %EQS(%_MODEL,l))THEN(
   %DEFINE(GCALL)(FCALL)
   %DEFINE(GJMP)(FJMP)
   %DEFINE(CODETYPE)(HCODE)
   %IF(%EQS(%CORE_IRAMDATA,1))THEN(
       %DEFINE(DATATYPE)(DATA)
       %IF(%EQS(%_MODEL,m))THEN(
	   %DEFINE(KSSTKTYPE)(IDATA)
       )ELSE(
	   %DEFINE(KSSTKTYPE)(HDATA)
       )FI
   )ELSE(
       %IF(%EQS(%_MODEL,m))THEN(
	  %DEFINE(DATATYPE)(IDATA)          ; medium model without core_iramdata
	  %DEFINE(KSSTKTYPE)(IDATA)
       )ELSE(
	  %DEFINE(DATATYPE)(HDATA)          ; large model without core_iramdata
	  %DEFINE(KSSTKTYPE)(HDATA)
       )FI
   )FI
   %DEFINE(KRNL_CODESEG_DEF)(ISVCRTXC_CODE SEGMENT HCODE)
   %DEFINE(ISRS_CODESEG_DEF)(    ISRS_CODE SEGMENT HCODE)
)ELSE(
   %DEFINE(GCALL)(CALL)
   %DEFINE(GJMP)(JMP)
   %DEFINE(CODETYPE)(CODE)
   %DEFINE(KSSTKTYPE)(IDATA)
   %IF(%EQS(%CORE_IRAMDATA,1))THEN(
       %DEFINE(DATATYPE)(DATA)
   )ELSE(
       %DEFINE(DATATYPE)(IDATA)          ; small model without core_iramdata
   )FI
   %DEFINE(KRNL_CODESEG_DEF)(ISVCRTXC_CODE SEGMENT CODE)
   %DEFINE(ISRS_CODESEG_DEF)(    ISRS_CODE SEGMENT CODE)
)FI

;===========================================================================
; The SYSMODE switch should always be defined and equal to 0x080.
; This defines the system mode.  User mode is not supported by RTXC.
;---------------------------------------------------------------------------
SYSMODE  equ 080h
;===========================================================================

; end of rtxcopts.inc
