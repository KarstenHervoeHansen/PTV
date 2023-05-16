
# Makefile for gains   project

DEBUG=1

!if$(DEBUG)
CFLAGS = -c -K -v -Ic:\bc3\include -Lc:\bc3\lib
LFLAGS = /x /v
AFLAGS = /zi
!else
CFLAGS = -c -K -Ic:\bc3\include -Lc:\bc3\lib
LFLAGS = /x
AFLAGS =
!endif

CC = c:\bc3\bin\bcc
LN = c:\bc3\bin\tlink

LIBS  = c:\bc3\LIB\mathS.lib c:\bc3\LIB\CS.lib c:\bc3\LIB\emu.lib
START = c:\bc3\LIB\C0S.obj

.c.obj:
      $(CC) $(CFLAGS) $&.c

OBJECTS = gains.obj iic_drv.obj xcom.obj
#OBJECTS = gains.obj xcom.obj

gains.exe: $(OBJECTS)
#  $(LN) $(LFLAGS) $(START) $(OBJECTS),$<,,$(LIBS)
  $(LN) @linkresp

gains.obj: gains.c
iic_drv.obj: iic_drv.c
xcom.obj: xcom.c port.h

