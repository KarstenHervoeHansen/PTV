
# Makefile for server and client project
# See link.rsp for linker args for 32 bits protected mode console
# application !


BCB = C:\BC5\BIN

DEBUG=0

CC = $(BCB)\bcc32
LN = $(BCB)\ilink32

!if$(DEBUG)
CFLAGS = -c -K -WC -v -P -VF
!else
CFLAGS = -c -K -WC -P -VF
!endif

.cpp.obj:
      $(CC) $(CFLAGS) $&.cpp

OBJECTS1 = tcp_svr.obj wsedesc.obj
OBJECTS2 = tcp_cli.obj wsedesc.obj

all: tcp_cli.exe tcp_svr.exe

tcp_cli.exe: $(OBJECTS2)
  $(LN) @linkc.rsp


tcp_svr.exe: $(OBJECTS1)
  $(LN) @links.rsp


tcp_svr.obj:  tcp_svr.cpp
tcp_cli.obj:  tcp_cli.cpp
wsedesc.obj:  wsedesc.cpp

