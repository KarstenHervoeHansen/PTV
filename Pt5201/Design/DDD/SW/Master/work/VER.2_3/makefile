
.SUFFIXES: .c .asm .src .obj .hex

PROJ=PT5201
#INCDIR=..\..\code\include\1_1
INCDIR=.\

default: all

#--------------- Target1 -------------------------------
Target1=PT5201
TargetType=XA

XACC=c:\cxa\bin\CXA
XAMP=c:\cxa\bin\MPPXA
XAAS=c:\cxa\bin\ASXA
XALINK=c:\cxa\bin\CCXA
XALOC=c:\cxa\bin\LCXA


XACFLAGS=-A1 -Cxag3 -e -g -Ic:\cxa\include -Ic:\cxa\rtxc\xa\include -I$(INCDIR) -Ml -O2 -s
XAMFLAGS=-D_MODEL=l -Ic:\cxa\rtxc\xa\include
XAAFLAGS=-Cxag3 -e -gs -OHjs
XALFLAGS= -Wlk -M -cl -Ml -LC:\cxa\lib\xa -Lc:\cxa\rtxc\xa\lib
XALCFLAGS=-f3 -M -dxa.dsc

OUTPUTDIR1=.\temp\xa
TARGETDIR1=.\temp


.asm{$(OUTPUTDIR1)}.obj:
#       @ECHO _
        @ECHO Preprocessing asm for XA : $** - $(@R).obj
#       @ECHO +$(XAMP) $(XAMFLAGS) -o $(@R).src $**
        @$(XAMP) -f &&|
        $(XAMFLAGS) -o $(@R).src $**
|
#       @ECHO assembling for XA : $(@R).src - $@
#       @ECHO +$(XAAS) $(@R).src -c $(XAAFLAGS) -o $@
        @$(XAAS) -f &&|
        $(@R).src -c $(XAAFLAGS) -o $@
|
#       @MOVE $(@B).lst $(OUTPUTDIR1)


.c{$(OUTPUTDIR1)}.obj:
#       @ECHO _
        @ECHO Compiling for XA : $** - $(@R).obj
#       @ECHO +$(XACC) $** $(XACFLAGS) -o $(@R).src
        @$(XACC) -f &&|
        $** $(XACFLAGS) -o $(@R).src
|
#       @ECHO assembling for XA : $(@R).src - $@
#       @ECHO +$(XAAS) $(@R).src -c $(XAAFLAGS) -o $@
        @$(XAAS) -f &&|
        $(@R).src $(XAAFLAGS) -o $@
|
        @MOVE $(@B).lst $(OUTPUTDIR1)


# Target1Dependencies:

# $(OUTPUTDIR1)\rtxcbug.obj \
# $(OUTPUTDIR1)\cchnls.obj \
# $(OUTPUTDIR1)\ccntlr.obj \
# $(OUTPUTDIR1)\cdevice.obj


$(TARGETDIR1)\$(Target1).hex: \
 $(OUTPUTDIR1)\ptvmain.obj \
 $(OUTPUTDIR1)\rtxcobj.obj \
 $(OUTPUTDIR1)\start.obj \
 $(OUTPUTDIR1)\isrs.obj \
 $(OUTPUTDIR1)\main.obj \
#channel drivers
 $(OUTPUTDIR1)\rs232.obj \
 $(OUTPUTDIR1)\sio0drv.obj \
 $(OUTPUTDIR1)\i2c_drv.obj \
#device drivers
 $(OUTPUTDIR1)\nv_drv.obj \
 $(OUTPUTDIR1)\ext0drv.obj \
 $(OUTPUTDIR1)\tim0drv.obj \
 $(OUTPUTDIR1)\led_hw.obj \
 $(OUTPUTDIR1)\key_hw.obj \
 $(OUTPUTDIR1)\plddrv.obj \
 $(OUTPUTDIR1)\tvp6kdrv.obj \
 $(OUTPUTDIR1)\unitprg.obj \
#other files
 $(OUTPUTDIR1)\cbar.obj \
 $(OUTPUTDIR1)\audio.obj \
 $(OUTPUTDIR1)\genlock.obj \
 $(OUTPUTDIR1)\bb.obj \
 $(OUTPUTDIR1)\instru.obj \
 $(OUTPUTDIR1)\keyscan.obj \
 $(OUTPUTDIR1)\front.obj \
 $(OUTPUTDIR1)\interp.obj \
 $(OUTPUTDIR1)\rs232cmd.obj \
 $(OUTPUTDIR1)\rs232par.obj \
 $(OUTPUTDIR1)\rs232err.obj \
 $(OUTPUTDIR1)\util.obj \
 $(OUTPUTDIR1)\ctask.obj \
 $(OUTPUTDIR1)\cclock.obj \
 $(OUTPUTDIR1)\cmbox.obj \
 $(OUTPUTDIR1)\cpart.obj \
 $(OUTPUTDIR1)\cqueue.obj \
 $(OUTPUTDIR1)\cres.obj \
 $(OUTPUTDIR1)\csema.obj
# $(Target1).i
        @ECHO ***** Linking for XA :  - $@
#       @echo +$(XALINK) $(XALFLAGS) -o$(@B).out  librtxc.a librtio.a libdrv.a librtl.a $**
#       @$(XALINK) -f &&|
#       $(XALFLAGS) ..\..\rtxc\xa\lib\librtxc.a ..\..\rtxc\xa\lib\librtio.a ..\..\rtxc\xa\lib\libdrv.a ..\..\rtxc\xa\lib\librtl.a -o$@ $**
#|
        @$(XALINK) -f &&|
         $(XALFLAGS) -o$(@B).out c:\cxa\rtxc\xa\lib\librtxc.a c:\cxa\rtxc\xa\lib\librtio.a c:\cxa\rtxc\xa\lib\libdrv.a c:\cxa\rtxc\xa\lib\librtl.a $**
#        $(XALFLAGS) -Wlk -emEDE=\"$(Target1).i\" -o$(@B).out ..\..\rtxc\xa\lib\librtxc.a ..\..\rtxc\xa\lib\librtio.a ..\..\rtxc\xa\lib\libdrv.a ..\..\rtxc\xa\lib\librtl.a $**
|
#       @echo +$(XALOC) -o$@ $(@B).out -emEDE=\"$(Target1).i\" $(XALCFLAGS)
        @$(XALOC) -o$@ $(@B).out -emEDE=\"$(Target1).i\" -f &&|
        $(XALCFLAGS)
|
#       @MOVE $(@R).map $(OUTPUTDIR1)


$(OUTPUTDIR1)\main.obj: \
        main.c \
        $(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\ptvmain.obj: \
        ptvmain.c \
        $(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\start.obj: \
        start.asm

$(OUTPUTDIR1)\isrs.obj: \
        isrs.asm

$(OUTPUTDIR1)\rtxcobj.obj: \
        rtxcobj.c \

#$(OUTPUTDIR1)\rtxcbug.obj: \
#       ..\..\app_spec\xa\rtxcbug.c \
#       ..\..\app_spec\xa\rtxcbug.h

$(OUTPUTDIR1)\rs232.obj: \
        rs232.c \
        $(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\sio0drv.obj: \
        sio0drv.c \
        $(INCDIR)\sio0drv.h

$(OUTPUTDIR1)\i2c_drv.obj: \
        i2c_drv.c \
        $(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\nv_drv.obj: \
        nv_drv.c \
        $(INCDIR)\nv_drv.h

$(OUTPUTDIR1)\ext0drv.obj: \
        ext0drv.c \
        $(INCDIR)\ext0drv.h

$(OUTPUTDIR1)\tim0drv.obj: \
        tim0drv.c \
        $(INCDIR)\tim0drv.h

$(OUTPUTDIR1)\led_hw.obj: \
        led_hw.c \
        $(INCDIR)\led_hw.h \

$(OUTPUTDIR1)\key_hw.obj: \
        key_hw.c \
        $(INCDIR)\key_hw.h \

$(OUTPUTDIR1)\plddrv.obj: \
        plddrv.c \
        $(INCDIR)\plddrv.h \

$(OUTPUTDIR1)\tvp6kdrv.obj: \
        tvp6kdrv.c \
        $(INCDIR)\tvp6kdrv.h \

$(OUTPUTDIR1)\unitprg.obj: \
        unitprg.c \
        $(INCDIR)\unitprg.h \

$(OUTPUTDIR1)\cbar.obj: \
        cbar.c \
        $(INCDIR)\cbar.h \

$(OUTPUTDIR1)\audio.obj: \
        audio.c \
        $(INCDIR)\audio.h \

$(OUTPUTDIR1)\genlock.obj: \
        genlock.c \
        $(INCDIR)\genlock.h \

$(OUTPUTDIR1)\bb.obj: \
        bb.c \
        $(INCDIR)\bb.h \

$(OUTPUTDIR1)\instru.obj: \
        instru.c \
        $(INCDIR)\rtxcobj.h \

$(OUTPUTDIR1)\keyscan.obj: \
        keyscan.c \
        $(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\front.obj: \
        front.c \
        $(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\interp.obj: \
        interp.c \
        $(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\rs232cmd.obj: \
        rs232cmd.c \
        $(INCDIR)\rs232cmd.h

$(OUTPUTDIR1)\rs232par.obj: \
        rs232par.c \
        $(INCDIR)\rs232par.h

$(OUTPUTDIR1)\rs232err.obj: \
        rs232err.c \
        $(INCDIR)\rs232err.h

$(OUTPUTDIR1)\util.obj: \
        util.c \
        $(INCDIR)\util.h

$(OUTPUTDIR1)\ctask.obj: \
        ctask.c \
        ctask.h

$(OUTPUTDIR1)\cclock.obj: \
        cclock.c \
        cclock.h

$(OUTPUTDIR1)\cmbox.obj: \
        cmbox.c \
        cmbox.h

$(OUTPUTDIR1)\cpart.obj: \
        cpart.c \
        cpart.h

$(OUTPUTDIR1)\cqueue.obj: \
        cqueue.c \
        cqueue.h

$(OUTPUTDIR1)\cres.obj: \
        cres.c \
        cres.h

$(OUTPUTDIR1)\csema.obj: \
        csema.c \
        csema.h

$(OUTPUTDIR1)\cchnls.obj: \
        cchnls.c \
        cchnls.h

$(OUTPUTDIR1)\ccntlr.obj: \
        ccntlr.c \
        ccntlr.h

$(OUTPUTDIR1)\cdevice.obj: \
        cdevice.c \
        cdevice.h

#--------------- Target3 -------------------------------
Target3=librtxc
TargetType=XA

XAMAKE=MKXA

OUTPUTDIR3=..\..\rtxc\xa\source\kernel
TARGETDIR3=..\..\rtxc\xa\lib

..\..\rtxc\xa\lib\librtxc.a: \
 ..\..\rtxc\xa\source\kernel\rtxcinit.c \
 ..\..\rtxc\xa\source\kernel\rtxcapi.c \
 ..\..\rtxc\xa\source\kernel\rtxc.c \
 ..\..\rtxc\xa\source\kernel\isvcrtxc.asm \
 ..\..\rtxc\xa\include\rtxcopts.h \
 ..\..\rtxc\xa\include\enable.h \
 ..\..\rtxc\xa\include\rtxcopts.inc
        @ECHO _
        @ECHO Building: $* library file
        $(XAMAKE) -G $(OUTPUTDIR3)


#--------------- Common -------------------------------
# CommonRules:


main.obj: \
 $(OUTPUTDIR1)\main.obj \
 $(OUTPUTDIR2)\main.obj

ptvmain.obj: \
 $(OUTPUTDIR1)\ptvmain.obj \
 $(OUTPUTDIR2)\ptvmain.obj

rtxcobj.obj: \
 $(OUTPUTDIR1)\rtxcobj.obj \
 $(OUTPUTDIR2)\rtxcobj.obj

start.obj: \
 $(OUTPUTDIR1)\start.obj

isrs.obj: \
 $(OUTPUTDIR1)\isrs.obj

rtxcbug.obj: \
 $(OUTPUTDIR1)\rtxcbug.obj
 $(OUTPUTDIR1)\rtxcbug.obj

instru.obj: \
 $(OUTPUTDIR1)\instru.obj \
 $(OUTPUTDIR2)\instru.obj 

command.obj: \
 $(OUTPUTDIR1)\command.obj \
 $(OUTPUTDIR2)\command.obj

rs232.obj: \
 $(OUTPUTDIR1)\rs232.obj \
 $(OUTPUTDIR2)\rs232.obj

sio0drv.obj: \
 $(OUTPUTDIR1)\sio0drv.obj \
 $(OUTPUTDIR2)\sio0drv.obj

ext0drv.obj: \
 $(OUTPUTDIR1)\ext0drv.obj \
 $(OUTPUTDIR2)\ext0drv.obj

interp.obj: \
 $(OUTPUTDIR1)\interp.obj \
 $(OUTPUTDIR2)\interp.obj

rs232cmd.obj: \
 $(OUTPUTDIR1)\rs232cmd.obj \
 $(OUTPUTDIR2)\rs232cmd.obj

rs232par.obj: \
 $(OUTPUTDIR1)\rs232par.obj \
 $(OUTPUTDIR2)\rs232par.obj

rs232err.obj: \
 $(OUTPUTDIR1)\rs232err.obj \
 $(OUTPUTDIR2)\rs232err.obj

util.obj: \
 $(OUTPUTDIR1)\util.obj \
 $(OUTPUTDIR2)\util.obj

ctask.obj: \
 $(OUTPUTDIR1)\ctask.obj \
 $(OUTPUTDIR2)\ctask.obj

cclock.obj: \
 $(OUTPUTDIR1)\cclock.obj \
 $(OUTPUTDIR2)\cclock.obj

cmbox.obj: \
 $(OUTPUTDIR1)\cmbox.obj \
 $(OUTPUTDIR2)\cmbox.obj

cpart.obj: \
 $(OUTPUTDIR1)\cpart.obj \
 $(OUTPUTDIR2)\cpart.obj

cqueue.obj: \
 $(OUTPUTDIR1)\cqueue.obj \
 $(OUTPUTDIR2)\cqueue.obj

cres.obj: \
 $(OUTPUTDIR1)\cres.obj \
 $(OUTPUTDIR2)\cres.obj

csema.obj: \
 $(OUTPUTDIR1)\csema.obj \
 $(OUTPUTDIR2)\csema.obj

cchnls.obj: \
 $(OUTPUTDIR1)\cchnls.obj

ccntlr.obj: \
 $(OUTPUTDIR1)\ccntlr.obj

cdevice.obj: \
 $(OUTPUTDIR1)\cdevice.obj


all: \
 $(TARGETDIR1)\$(Target1).hex


libs: \
 $(TARGETDIR3)\$(Target3).a


