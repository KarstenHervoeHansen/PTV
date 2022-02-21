; binding.inc - RTXC binding assembly time options

;
; RTXC    Version 3.2
; Copyright (c) 1986-1997.
; Embedded System Products, Inc.
; ALL RIGHTS RESERVED
;

;
; choose _far or _near equ 1
;
; __HUGE__ requires ks_stk_far equ 1
;
ks_stk_near equ 0
ks_stk_far  equ 1

FPU = 0   ; 0=no extended context, 1=extended context, e.g., FPU

RTXCSTKSZ = 512   ; can be changed as needed
                  ; NOTE: RTXCSTKSZ must match definition in binding.h

; End of file - binding.inc
