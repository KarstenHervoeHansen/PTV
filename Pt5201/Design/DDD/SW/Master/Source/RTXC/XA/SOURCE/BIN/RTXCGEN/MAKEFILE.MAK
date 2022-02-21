#
# filename = makefile.mak
#
# makefile for Microsoft C 6.0 nmake
#

#
# invoke as: > nmake -f makefile.mak
#

MSC=c:\msc60 # root install path for MSC 6.0

CC=$(MSC)\bin\cl
CCFLAGS=/c /AL $(include) $(cclist) $(warn) $(debug)

LD=$(MSC)\bin\link

include=/I$(MSC)\include /I..\kernel /I..\rtxcio 

#cclist=/Fc # full C + asm listings

# note: /W4 may generate benign warnings
#warn=/W4

#debug=/Od /Zi

#
# default .c.obj rules
#
.c.obj:
   $(CC) $(CCFLAGS) $*.c

#
# primary target
#
all: rtxcgen.exe sizes.exe

#
# rtxcgen dependencies
#
rtxcgen.exe: rtxcgen.obj defclock.obj defmbox.obj defpart.obj \
	     defqueue.obj defres.obj defsema.obj deftask.obj \
	     defchnls.obj defcntlr.obj defdevs.obj defiogen.obj \
	     defclctl.obj defclent.obj defcldrv.obj
   $(LD) /ST:4096 @rtxcgen.lnk

rtxcgen.obj: rtxcgen.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h

defclock.obj: defclock.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h rtxcgen.h

defmbox.obj: defmbox.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h rtxcgen.h

defpart.obj: defpart.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h rtxcgen.h

defqueue.obj: defqueue.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h rtxcgen.h

defres.obj: defres.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h rtxcgen.h

defsema.obj: defsema.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h rtxcgen.h

deftask.obj: deftask.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h rtxcgen.h

defchnls.obj: defchnls.c

defcntlr.obj: defcntlr.c

defdevs.obj: defdevs.c

defclctl.obj: defclctl.c

defclent.obj: defclent.c

defcldrv.obj: defcldrv.c

#
# throwdown to show structure sizes to aid in user configuration
#
sizes.exe: sizes.obj
   $(LD) sizes,sizes.exe,sizes.map,$(MSC)\lib\llibce.lib /E;

sizes.obj: sizes.c ..\kernel\rtxstruc.h ..\kernel\rtxcopts.h
