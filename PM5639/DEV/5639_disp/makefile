
# Makefile  for res PROJECT (5633 display unit)


AA51= c:\c51p.GUN\bin\a51
CC51= c:\c51p.GUN\bin\c51
LN51= c:\c51p.GUN\bin\l51

.c.obj:
      $(CC51) $&.c $(CFLAGS)

.a51.obj:
      $(AA51) $&.a51 $(AFLAGS)


OBJECTS = res.obj,com.obj,run.obj,but.obj,sub.obj,tables.obj,date.obj

a.abs: res.obj com.obj run.obj but.obj sub.obj tables.obj date.obj

   del a.m51
   $(LN51) @LINK.RSP
   OHS51 a.abs
   hb80 a.hex res.bin

res.obj: 	res.c defs.h run.h com.h sub.h but.h date.h tables.h
run.obj: 	run.c defs.h sub.h tables.h
but.obj: 	but.c defs.h sub.h com.h tables.h
sub.obj: 	sub.c defs.h sub.h com.h tables.h
com.obj: 	com.c defs.h run.h res.h sub.h tables.h
tables.obj: tables.c defs.h tables.h
date.obj:	date.c defs.h sub.h tables.h
