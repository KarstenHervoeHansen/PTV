# ---------------------------------------------------------------------------
VERSION = BCB.01
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = PT52REM.exe
OBJFILES = PT52REM.obj System.obj about.obj audio.obj Black1.obj black2.obj \
   black3.obj tsg.obj mainform.obj preset.obj proper.obj pt52class.obj util.obj \
   genlock.obj comm.obj
RESFILES = PT52REM.res handpnt.res
RESDEPEN = $(RESFILES) System.dfm about.dfm audio.dfm Black1.dfm black2.dfm \
   black3.dfm tsg.dfm mainform.dfm preset.dfm proper.dfm genlock.dfm
LIBFILES = 
DEFFILE = 
# ---------------------------------------------------------------------------
CFLAG1 = -Od -w -k -r- -y -v -vi- -c -a4 -b- -w-par -w-inl -Vx -Ve -x
CFLAG2 = -Ic:\borland\CBuilder\include;c:\borland\CBuilder\include\vcl 
PFLAGS = -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE \
   -Uc:\borland\CBuilder\lib\obj;c:\borland\CBuilder\lib \
   -Ic:\borland\CBuilder\include;c:\borland\CBuilder\include\vcl -v -$Y -$W -$O- \
   -$I- -JPHNV -M     
RFLAGS = -ic:\borland\CBuilder\include;c:\borland\CBuilder\include\vcl 
LFLAGS = -Lc:\borland\CBuilder\lib\obj;c:\borland\CBuilder\lib -aa -Tpe -x -v \
   -V4.0 
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
