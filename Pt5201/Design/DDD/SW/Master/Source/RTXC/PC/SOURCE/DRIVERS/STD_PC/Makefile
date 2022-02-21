#
#   RTXC    Version 3.2
#   Copyright (c) 1986-1998.
#   Embedded System Products, Inc.
#   ALL RIGHTS RESERVED
#

#
# filename = makefile
#
# makefile for Borland C++ v5.0x using Borland make v4.0
#

!include "..\..\..\Include\default.mak"

INC = ..\..\..\Include
LIB = ..\..\..\Lib

MODEL = __LARGE__   # normal model, use with large model application
#MODEL = __HUGE__   # huge model, only if application requires HUGE model

#
# .c.obj default rules
#
.c.obj:
   $(CC) $(CCFLAGS) $*.c
   $(AR) $(LIB)\libdrv -+$*.obj
   $(RM) $(LIB)\libdrv.bak

#
# .asm.obj default rules
#
.asm.obj:
   $(AS) $(ASFLAGS) $*.asm
   $(AR) $(LIB)\libdrv -+$*.obj
   $(RM) $(LIB)\libdrv.bak

#-----------------------------------------------------------------
# CPU type
#-----------------------------------------------------------------
#cccpu = -1-  # 8088/8086
cccpu = -1   # 80186 (+ 80286/386/486)
#cccpu = -3 -DCPU386  # 80386/486 - BCC 3.1 **ONLY** (32 bit math)

#ascpu = 0    # 8088, also need cccpu = -1- above
ascpu = 2    # 80186 (+ 80286/...), also need cccpu = -1 above
#ascpu = 3    # 80386/486 also need cccpu = -3 above

#-----------------------------------------------------------------
# FPU type
#-----------------------------------------------------------------
ccfp = -f-   # NO fp code at all in kernel even with FPU support

asfp =       # NO fp code at all in kernel even with FPU support

warn = -w    # enabled
#warn = -w-   # minimal warnings

# used by compiler and linker
#ccdebug =    # disabled
ccdebug = -v # enabled

#
# register variables
# -r-  None
# -rd  Only with register keyword
# -r   Automatic (cannot be used with OMF file output)

#
# Include Paths
#
include = -I$(INC)

#
# Note: The RTXC Run-Time Library functions are compiled using
#       large (-ml) model.  This is true for all RTXC functions called
#       directly by user application code.  Despite large model option,
#       the code object is forced in the RTXC_TEXT segment.  This is
#       done to isolate RTXC from the application.
#
#       -ml  compact model
#       -mh  huge model
#       -k-  suppress stack frame (useful only for call tracebacks)
#       -G   optimize for speed instead of size
#       -O-  disable optimizer
#       -Oi  inline functions (fast but code is larger)
#       -O2  C++ 3.0 - fastest code
#       -a   align structure sizes (helpful if odd sized structs)
#       -Z   alias optimizer (use with caution - rtxc qualified OK)
#       -S   output mixed C and assembly language file - not object
#
#
# Note: The -O2-p (Maximum speed optimization BUT with No copy propagation)
#       switch is IMPORTANT and required if using INLINE KS() calls.
#
!if $(MODEL) == __LARGE__ ## STD/LARGE MODEL WITH INLINE ASM FOLLOWS
CCFLAGS = -c -ml -a2 -k- -G -O2-p -Oi -Z -r- $(ccfp) $(cccpu) $(warn) $(ccdebug) \
           $(include) -Ic:\programs\bc4\include
!else ## HUGE MODEL FOLLOWS
CCFLAGS = -c -mh -a2 -k- -G -O2-p -Z -r- $(ccfp) $(cccpu) $(warn) $(ccdebug) \
           $(include) -Dnear= -Dfar= -Dhuge= 
!endif

# tasm options
#
# -l   listing enabled
# -c   cross reference in listing
# -n   suppress symbol table in listing
# -x   show false conditionals
#
# note: tasm may be called after bcc (with -S) in order to view mixed C +
#       assembly listings when aslist is defined.  Alternatively, -B -Etasm.exe
#       -Tl will produce .lst file but NOT mixed C/asm listings - at least
#       not for me.
#
#aslist=
aslist=-l -n -x -t

asdebug =
#asdebug = -zi

#
# -ml = case sensitive symbols
#
ASFLAGS = -ml -dCPU=$(ascpu) -d$(MODEL) $(asfp) $(asdebug) $(aslist) $(include) 

#
# RTXC library
#
libdrv.lib: clkdrv.obj isrs.obj 
#	rtxstdio.obj  bspinit.obj bspcb.obj comdrv.obj \
#            condrv.obj prndrv.obj dvz.obj

#
# RTXC RTL (large/huge) model
#
clkdrv.obj: clkdrv.c $(INC)\typedef.h

comdrv.obj: comdrv.c $(INC)\comdrv.h $(INC)\enable.h $(INC)\rtxcapi.h $(INC)\rtxstdio.h

bspinit.obj: bspinit.c $(INC)\rtxstdio.h $(INC)\typedef.h

bspcb.obj: bspcb.c $(INC)\rtxstdio.h cclock.h

rtxstdio.obj: rtxstdio.c $(INC)\rtxstdio.h

isrs.obj: isrs.asm $(INC)\isrs.inc

condrv.obj: condrv.c $(INC)\rtxcapi.h $(INC)\rtxstdio.h

prndrv.obj: prndrv.c $(INC)\rtxcapi.h

dvz.obj: dvz.c $(INC)\rtxcapi.h

#
# Clean Directory - remove intermediate files
#
clean:
        $(RM) *.bak
        $(RM) *.lst
        $(RM) *.obj
        $(RM) *.map
        $(RM) *.csm
        $(RM) *.mrt
        $(RM) *.mbt
        $(RM) *.ncb
        $(RM) *.obr
        $(RM) *.~de
        $(RM) *.r$p
