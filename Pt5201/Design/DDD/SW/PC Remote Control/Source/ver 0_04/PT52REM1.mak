# ---------------------------------------------------------------------------
VERSION = BCB.01
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = PT52REM1.exe
OBJFILES = PT52REM1.obj mainform.obj about.obj audio.obj Black1.obj black2.obj \
   black3.obj Colorbar.obj connect.obj status.obj genlock.obj util.obj proper.obj \
   pt52class.obj preset.obj
RESFILES = PT52REM1.res handpnt.res
RESDEPEN = $(RESFILES) mainform.dfm about.dfm audio.dfm Black1.dfm black2.dfm \
   black3.dfm Colorbar.dfm connect.dfm status.dfm genlock.dfm proper.dfm \
   preset.dfm
LIBFILES = 
DEFFILE = 
# ---------------------------------------------------------------------------
CFLAG1 = -Od -Hc -w -k -r- -y -v -vi- -c -a4 -b- -w-par -w-inl -Vx -Ve -x
CFLAG2 = -I"u:\project\pt5201 pc remote control";$(BCB)\builder1;u:\project\pt5201~1;.;$(BCB)\include;$(BCB)\include\vcl \
   -H=$(BCB)\lib\vcld.csm 
PFLAGS = -U"u:\project\pt5201 pc remote control";$(BCB)\builder1;u:\project\pt5201~1;.;$(BCB)\lib\obj;$(BCB)\lib \
   -I"u:\project\pt5201 pc remote control";$(BCB)\builder1;u:\project\pt5201~1;.;$(BCB)\include;$(BCB)\include\vcl \
   -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE -v \
   -$Y -$W -$O- -JPHNV -M     
RFLAGS = -i"u:\project\pt5201 pc remote control";$(BCB)\builder1;u:\project\pt5201~1;.;$(BCB)\include;$(BCB)\include\vcl 
LFLAGS = -L"u:\project\pt5201 pc remote control";$(BCB)\builder1;u:\project\pt5201~1;.;$(BCB)\lib\obj;$(BCB)\lib \
   -aa -Tpe -x -v -V4.0 
IFLAGS = 
LINKER = ilink32
# ---------------------------------------------------------------------------
ALLOBJ = c0w32.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) vcl.lib import32.lib cp32mt.lib 
# ---------------------------------------------------------------------------
.autodepend

$(PROJECT): $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES) 
!

.pas.hpp:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.pas.obj:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.cpp.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $* 

.c.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $**

.rc.res:
    $(BCB)\BIN\brcc32 $(RFLAGS) $<
#-----------------------------------------------------------------------------
