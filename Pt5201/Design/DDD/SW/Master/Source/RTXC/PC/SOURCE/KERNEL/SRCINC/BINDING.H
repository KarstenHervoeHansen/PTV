/* binding.h - RTXC binding compile time options */

/*
 *   RTXC    Version 3.2
 *   Copyright (c) 1986-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/

#ifndef _BINDING_H /* { */
#define _BINDING_H

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

/******************************************************************************

INTERFACE LIBRARY LANGUAGE INTERFACE SPECIFICATION

The  RTXC interface library language calling sequence specification is a user
defined  option.  In  some processor /  compiler combinations, either  a C or
pascal  language interface can be  defined.  The code  generated for a pascal
calling sequence typically has the  function  itself  "clean  up"  the  stack
before  returning to the caller versus C (cdecl) where the caller must "clean
up" the stack (typically "add #n,sp") after each and every function call.

The choice of a  pascal  call  interface  usually  generates  smaller  object
modules, especially when lots of kernel  service  calls  are  invoked.    The
pascal mode is  useful  for  tuning  tiny  ROM-able  applications.    On  the
downside,  pascal is slightly non-standard and  foreign to most C programmers
(except for Windows programmers).

The INTEL C toolkits use a slightly different language model.  INTEL uses the
keywords,  reentrant (equivalent to  cdecl) and alien  (roughly equivalent to
pascal, really intended for PLM).

The effect of  this  switch  is  in  rtxcapi.c  and  rtxcapi.h  in  the  RTXC
application interface library definitions where a "codedef" keyword "lang" is
used  to allow for user defined language  specification.  The lang keyword is
#defined to cdecl, pascal or NIL to effect the compile time specification.

Note, under  Turbo C++ the  extern symbols are  all upper case  in pascal and
mixed  case in cdecl so there is some level of protection provided to prevent
extreme confusion when mixing languages and calling sequences.

******************************************************************************/

#define LANG_C /* select LANG_C, LANG_REGS or LANG_PASCAL */

/*****************************************************************************/
/******************************************************************************

RTXCBUG SUPPORT

Includes RTXCbug support in kernel and system tables

******************************************************************************/

#define CBUG /* select #define or #undef */

/*****************************************************************************/
/******************************************************************************

EXTENDED CONTEXT SUPPORT (floating point)

Extended context support,  e.g.,  floating  point,  stdin/out/stderr,  memory
management,   graphics  context  or  bank  switching,  includes  support  for
switching extended contexts within kernel and system tables.

The following notes pertain to  the  use  of  the  Floating  Point  Emulation
Library   (emu)  with  RTXC  3.2  applications  under  the  Borland  C++  3.1
development environment instead of using real (hardware) 80x87 support.

Borland C++ 3.1 uses interrupts 0x34 through 0x3d for FPU emulation plus 0x3e
for other  special "stuff".   As shipped, RTXC  uses vector 64  (0x40) so you
should  have no problems  there.  However,  we recommend that  you change the
RTXC entry mode to CALL_BASED  instead  of  VECTOR_BASED  in  binding.h  and
remake the library.  The  second  option  simply  avoids  the  shared  vector
problem all together.

One must define KS_STK_FAR in this file.

The FPU  switch in  this file must be defined (hardware only).
The FP_EMU  switch in  this file must be defined (emulation only).

RTXC  stack sizes for FP_EMU tasks need to be increased by at least 272 bytes
over  the non-FPU  requirements to  maintain the  emu soft  register context.
Note: When  a task is  started by KS_execute(),  the task stack  size will be
reduced  (by as much as 15 bytes) if necessary in order to align on paragraph
boundary.

When  defining tasks via RTXCgen, answer NO to the FPU question.  That switch
applies only to hardware floating point.

Each task that  requires floating  point operations  must call  init_FPU() as
part  of the task's initial logic (before any FPU code executes).

******************************************************************************/

#undef FPU /* select #define or #undef */
#undef FP_EMU /* select #define or #undef */

/*****************************************************************************/
/******************************************************************************

KERNEL STACK

Define HAS_KSTACK for those processor/bindings which support a kernel stack.
Default is defined. Also define size of kernel stack.

******************************************************************************/

#define HAS_KSTACK
#define RTXCSTKSZ  512    /* must match size in RTXCOPTS.INC */
                         /* and should be a multiple of 2 */

/*****************************************************************************/
/******************************************************************************

KERNEL STACK MODEL (ks_stk) SPECIFICATION (80X86 - PC versions only)

The  kernel stack model  specification is used  only in the  INTEL 80X86 (PC)
environment.  For all  other  processors,  the  stack  model  option  is  not
supported.

The kernel stack  model specification  is implemented  using a  RTXC specific
KS_STK keyword.   The "ks_stk" keyword is  substituted with either near, far,
or NIL  (for non-PC versions) by  the C preprocessor.   The KS_STK keyword is
used  in rtxc.c in qualifying the pointer  to the RTXC argument packet passed
to rtxc(), in rtxcapi.c in  passing  the  above  mentioned  pointer,  and  in
rtxcbug.c when checking stack sizes for overflow.

The choice  of the NEAR stack model (KS_STK_NEAR)  is faster than a FAR stack
(KS_STK_FAR).  The  only downside  to the  use of  a NEAR  stack is  that all
stacks  plus all other NEAR data including literal char strings must fit into
64  kbytes.  This may be a problem with large applications with lots of tasks
(each task  stack typically requires 256 - 1024  bytes) and there are lots of
NEAR data items.

If  KS_STK is specified as NEAR (Borland C 3.x), then the application must be
linked  with c0fl.lib.  If FAR stack is specified, then c0l.lib must be used.
Note, Borland 2.0 does not support c0fl.lib, nor KS_STK_NEAR model.

      Microsoft supports only KS_STK_NEAR
      Borland 3.x supports KS_STK_NEAR or KS_STK_FAR
      Borland 2.0 supports only KS_STK_FAR

******************************************************************************/

#define KS_STK_FAR /* select KS_STK_NEAR or KS_STK_FAR */

/*****************************************************************************/
/******************************************************************************

INLINE INTERRUPT CONTROL

The INLINE_INTS  option allows for inline code  generation for the ENABLE and
DISABLE macros which  control  the  enabling  and  disabling  of  interrupts.
Inline code is typically  faster  than  the  alternative  (explicit  function
calls).  If the compiler supports inline options, it should usually be used.

******************************************************************************/

#define INLINE_INTS /* select #define or #undef */

/*****************************************************************************/
/******************************************************************************

INLINE_MACRO_EXPANSION OPTION

The INLINE_MACRO_EXPANSION option allows for selecting expansion of various C
macros used in the kernel as inline C code versus function calls.  The inline
code is typically faster than the alternative (explicit function calls).  The
penalty for using  macro expansion  is that  the RTXC  kernel object  code is
slightly  larger.  The size should only be a concern in small microcontroller
or extremely ROM-limited environments.

******************************************************************************/

#define INLINE_MACRO_EXPANSION /* select #define or #undef */

/*****************************************************************************/
/******************************************************************************

EXPLICIT INITIALIZATION (zero) of critical uninitialized data items

The BSS_NOT_ZERO option  allows  for  explicit  initialization  (zeroing)  of
uninitialized  data areas.  In particular, critical tables and variables used
by RTXC will be cleared as part of the rtxcinit() procedures.  The net effect
is slightly slower  and larger initialiation  code.  This  option is #defined
when using  ROM based systems where the RAM  contents is unknown or when RTXC
applications are required to be hot-restartable.

******************************************************************************/

#define BSS_NOT_ZERO /* #define of #undef this variable */

/*****************************************************************************/
/******************************************************************************

RTXC CALL MODEL SPECIFICATION

The use of a VECTOR_BASED calling sequence is the most general case.  However
under QEMM, EMM386, SOFT-ICE  and other  386 DOS  extenders, the  overhead in
going through a  software  based  interrupt  may  be  considered  prohibitive
(approximately  100 - 200% overhead for a typical kernel call).  An alternate
solution  is to select the CALL_BASED  calling sequence which simply emulates
the software interrupt and  avoids  the  interrupt  overhead.    VECTOR_BASED
support is provided for embedded, TSR and other hybrid environments where the
kernel must be linked  independently from  the application.   Note  that both
options may not be supported on all processors.

******************************************************************************/

#define CALL_BASED /* choose VECTOR_BASED or CALL_BASED in 80x86 model */

#define RTXC_VECTOR 64 /* vector used for SW int iff VECTOR_BASED selected */

/*****************************************************************************/
/******************************************************************************

HUGE PARTITIONS (> 64 kbytes) Support for PC (Borland only) environments

The  HUGE_PARTITION compile time variable (PC  version - Borland only) allows
for one  or more partitions to be greater  than 64 kbytes in aggregate, i.e.,
100  blocks at 1000 bytes/block.  The  difference is limited to the partition
initialization code where huge pointers are used, else the runtime allocation
and deallocation code is  the same  (no slower  for huge  model).

The C source  code generated  by rtxcgen  is slightly  different in  that the
partition arrays are declared as  "static  char  huge  part%d[%ldL];"  versus
"static char  FAR part%d[M%dSZ  * M%dCT];"  since Borland  C++ (2.0  and 3.x)
allows arrays  greater than  64 kbytes  only in  huge model.   Note  that you
cannot have more  than 32k blocks (limited  by int on PC)  and a single block
may not  be larger  than 64  kbytes (on  PC -  defined by  size_t).   But any
combination is legal and limited only by the amount of memory available.

Note, just IGNORE the  warning  from  tasm,  the  Borland  assembler,  during
cpart.asm  saying  "Location counter overflow" on partitions > 64 kbytes when
defining huge partitions.

******************************************************************************/

#undef HUGE_PARTITIONS /* choose #define or #undef */

/*****************************************************************************/
/******************************************************************************

QUEUE COPY OPTION - QUEUE_MEMCPY

The  QUEUE_MEMCPY  compile  time  variable  determines  (defined)  whether  a
memcpy()  operation will be performed for  all queue data movement operations
or (undefined) whether all  1,  2,  and  4  byte  wide  queue  data  movement
operations will be performed using char, short and long data type assignments
and memcpy() will be performed for all other queue width sizes.  Defining the
QUEUE_MEMCPY  switch will produce  slightly slower code  for 1, 2  and 4 byte
wide queues, but the code will be significantly smaller.  This switch is most
valuable in  microcontrollers  where  code  size  is  critical  and  in  RISC
processors where maximum inline code is preferred.  Note, some compilers even
generate inline code for the memcpy call.

******************************************************************************/

#undef QUEUE_MEMCPY /* choose #define or #undef */

/*****************************************************************************/
/******************************************************************************

SYNCHRONIZED TASK START UP SPECIFICATION

In the  standard distribution, the main() function,  which serves as the NULL
task, calls KS_execute() for each task that is in the start list.  Each task,
when it is executed, will run until it blocks for some reason and then main()
will  start the next task.  The use of the SYNC_START option allows all tasks
that are defined in the  start  list  to  be  initialized  but  not  actually
started.   If the calling  task is the  NULL task and  SYNC_START is defined,
KS_execute()  will set the task status to  BLOCKED_WAIT instead of READY.  In
this mode,  after all  tasks are  executed, main()  will use  KS_unblock() to
unblock all tasks.  Multitasking will then begin.

******************************************************************************/

#undef SYNC_START /* select #define or #undef */

/*****************************************************************************/
/******************************************************************************

FAST KS_lock()/KS_lockw()/KS_lockt()/KS_unlock()
 &
FAST KS_alloc()/KD_allocw()/KS_alloct()

The  next  two  FAST  switches  improve  performance  at  the  expense  of  a
considerable increase in the size of the rtxcapi module.  The FASTLOCk switch
allows an open resource to be locked and unlocked from within the API without
have to enter the kernel.  If the resource is being used by another object, a
kernel  call is  initiated  for a  KS_lockw() or  KS_lockt().  The  FASTALLOC
switch allows an available memory  block to be  allocated from within the API
without having to enter the kernel.  If there are no memory blocks available,
a kernel call is initiated for a KS_allocw() or KS_alloct().

******************************************************************************/
/* user defined option for speed optimized KS_lockw()/KS_unlock() */
#undef FASTLOCK

/* user defined option for speed optimized KS_alloc()/KS_allocw() */
#undef FASTALLOC

/*****************************************************************************/
/******************************************************************************

KERNEL AWARENESS SUPPORT (Kernel Awareness Module required)

The switch, KA_SUPPORT, is used to enable kernel awareness support for the
RTXC kernel.  Kernel Awareness supports results in a slight increase in RAM
usage by the kernel.  To use kernel awareness, a Kernel Awareness Module
(usually in the form of a Dynamic Link Library) must be purchased and
incorporated into your debugging environment.  A kernel awareness module is
available for most debugging environments.

******************************************************************************/

#define KA_SUPPORT /* Kernel Awareness Support - KAM required */

/*****************************************************************************/

/******************************************************************************
                  end of user defined switches
******************************************************************************/

/************ kernel stack model *********************************************/

/* ks_stk must correspond to definition in rtxcopts.inc */
#ifdef KS_STK_FAR
#define ks_stk far /* default - most general - tlink with c0l.obj (ss != ds) */
#else
#define ks_stk near /* alternate - fastest - tlink with c0fl.obj (ss == ds) */
#endif

/************ api language model *********************************************/

#ifdef LANG_C
#define KSAPI cdecl /* default - standard C */
#endif

#ifdef LANG_PASCAL
#define KSAPI pascal /* alternate - smaller code */
#endif

#ifdef LANG_REGS
#define KSAPI __fastcall /* alternate - smaller code and faster */
#endif

/*****************************************************************************/

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif /* } _BINDING_H */

/* End of file - binding.h */
