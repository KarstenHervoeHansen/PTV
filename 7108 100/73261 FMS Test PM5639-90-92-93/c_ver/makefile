
# Makefile for gains   project

COMP = c:\bc3


DEBUG=0

!if$(DEBUG)
CFLAGS = -c -K -v -I$(COMP)\include -L$(COMP)\lib
LFLAGS = /v
AFLAGS = /zi
!else
CFLAGS = -c -K -I$(COMP)\include -L$(COMP)\lib
LFLAGS =
AFLAGS =
!endif

CC = $(COMP)\bin\bcc
LN = $(COMP)\bin\tlink

LIBS  = $(COMP)\mathS.lib $(COMP)\LIB\CS.lib $(COMP)\LIB\emu.lib
START = $(COMP)\LIB\C0S.obj

.c.obj:
      $(CC) $(CFLAGS) $&.c

OBJECTS = gains.obj iic_drv.obj xcom.obj

gains.exe: $(OBJECTS)
  $(LN) @linkresp.bc3
  detab log.dat
#  $(LN) @linkresp.tc

gains.obj:   gains.c
iic_drv.obj: iic_drv.c
xcom.obj:    xcom.c port.h

