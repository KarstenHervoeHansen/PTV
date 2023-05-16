# @(#)mkxa_mk	1.2
# default rules for XA tools
#

SHELLCMD=	break call cd chcp chdir cls copy ctty date del dir echo erase\
		exit for goto if md mkdir path pause prompt rd rem ren rename\
		rmdir set shift time type ver verify vol
RM	=	del

.SUFFIXES:	.c .cc .asm .obj .abs


MAKE	=	mkxa
CC	=	ccxa
CCFLAGS	=
CPPFLAGS=
AS	=	ccxa
ASFLAGS	=
LD	=	ccxa
LDFLAGS	=


.c.obj:
	$(CC) -c -o $@ $(CCFLAGS) $(CCFLAGS_$*) $(CPPFLAGS) $(CPPFLAGS_$*) $<

.cc.obj:
	$(CC) -c -o $@ $(CCFLAGS) $(CCFLAGS_$*) $(CPPFLAGS) $(CPPFLAGS_$*) $<

.asm.obj:
	$(AS) -c -o $@ $(ASFLAGS) $(ASFLAGS_$*) $<

.obj.abs:
	$(LD) -o $@  $(match .obj $!) $(match .a $!) $(LDFLAGS) $(LDFLAGS_$*)

