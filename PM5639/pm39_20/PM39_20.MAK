.AUTODEPEND

#		*Translator Definitions*
CC = bcc +PM39_20.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = C:\BORLANDC\LIB
INCLUDEPATH = C:\BORLANDC\INCLUDE


#		*Implicit Rules*
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

#		*List Macros*


EXE_dependencies =  \
 main.obj \
 files.obj \
 display.obj \
 pm39io.obj \
 gui.obj \
 gui1.obj \
 gui2.obj \
 philogo.obj \
 graph.obj \
 help.obj \
 vars.obj

#		*Explicit Rules*
pm39_20.exe: pm39_20.cfg $(EXE_dependencies)
  $(TLINK) /v/x/c/P-/L$(LIBPATH) @&&|
c0l.obj+
main.obj+
files.obj+
display.obj+
pm39io.obj+
gui.obj+
gui1.obj+
gui2.obj+
philogo.obj+
graph.obj+
help.obj+
vars.obj
pm39_20
		# no map file
graphics.lib+
emu.lib+
mathl.lib+
cl.lib
|


#		*Individual File Dependencies*
main.obj: pm39_20.cfg main.c 

files.obj: pm39_20.cfg files.c 

display.obj: pm39_20.cfg display.c 

pm39io.obj: pm39_20.cfg pm39io.c 

gui.obj: pm39_20.cfg gui.c 

gui1.obj: pm39_20.cfg gui1.c 

gui2.obj: pm39_20.cfg gui2.c 

philogo.obj: pm39_20.cfg philogo.c 

help.obj: pm39_20.cfg help.c 

vars.obj: pm39_20.cfg vars.c 

#		*Compiler Configuration File*
pm39_20.cfg: pm39_20.mak
  copy &&|
-ml
-2
-a
-v
-G
-O
-Og
-Oe
-Om
-Ov
-Ol
-Ob
-Op
-Oi
-Z
-k-
-vi
-H=PM39_20.SYM
-w-ret
-w-nci
-w-inl
-wpin
-wamb
-wamp
-w-par
-wasm
-wcln
-w-cpt
-wdef
-w-dup
-w-pia
-wsig
-wnod
-w-ill
-w-sus
-wstv
-wucp
-wuse
-w-ext
-w-ias
-w-ibc
-w-pre
-w-nst
-I$(INCLUDEPATH)
-L$(LIBPATH)
-P-.C
| pm39_20.cfg


