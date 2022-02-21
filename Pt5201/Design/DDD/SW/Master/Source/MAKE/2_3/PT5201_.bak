
.SUFFIXES: .c .asm .src .obj .hex

PROJ=PT5201
INCDIR=..\..\code\include\1_0

default: all

#--------------- Target1 -------------------------------
Target1=PT5201
TargetType=XA

XACC=CXA
XAMP=MPPXA
XAAS=ASXA
XALINK=CCXA
XALOC=LCXA


XACFLAGS=-A1 -Cxag3 -e -g -Ic:\cxa\include -I..\..\rtxcdef\xa -I..\..\rtxc\xa\include -I$(INCDIR) -I..\..\app_spec\xa -Ml -O2 -s
XAMFLAGS=-D_MODEL=l -I..\..\rtxc\xa\include
XAAFLAGS=-Cxag3 -e -gs -OHjs
XALFLAGS= -Wlk -M -cl -Ml -LC:\cxa\lib\xa -L..\..\rtxc\xa\lib
XALCFLAGS=-f3 -M -dxa.dsc

OUTPUTDIR1=.\temp\xa
TARGETDIR1=.\temp


.asm{$(OUTPUTDIR1)}.obj:
#	@ECHO _
	@ECHO Preprocessing asm for XA : $** - $(@R).obj
#	@ECHO +$(XAMP) $(XAMFLAGS) -o $(@R).src $**
	@$(XAMP) -f &&|
	$(XAMFLAGS) -o $(@R).src $**
|
#	@ECHO assembling for XA : $(@R).src - $@
#	@ECHO +$(XAAS) $(@R).src -c $(XAAFLAGS) -o $@
	@$(XAAS) -f &&|
	$(@R).src -c $(XAAFLAGS) -o $@
|
#	@MOVE $(@B).lst $(OUTPUTDIR1)


.c{$(OUTPUTDIR1)}.obj:
#	@ECHO _
	@ECHO Compiling for XA : $** - $(@R).obj
#	@ECHO +$(XACC) $** $(XACFLAGS) -o $(@R).src
	@$(XACC) -f &&|
	$** $(XACFLAGS) -o $(@R).src
|
#	@ECHO assembling for XA : $(@R).src - $@
#	@ECHO +$(XAAS) $(@R).src -c $(XAAFLAGS) -o $@
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
 $(OUTPUTDIR1)\start.obj \
 $(OUTPUTDIR1)\isrs.obj \
 $(OUTPUTDIR1)\main.obj \
 $(OUTPUTDIR1)\ptvmain.obj \
 $(OUTPUTDIR1)\rtxcobj.obj \
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
#	@echo +$(XALINK) $(XALFLAGS) -o$(@B).out  librtxc.a librtio.a libdrv.a librtl.a $**
#	@$(XALINK) -f &&|
#	$(XALFLAGS) ..\..\rtxc\xa\lib\librtxc.a ..\..\rtxc\xa\lib\librtio.a ..\..\rtxc\xa\lib\libdrv.a ..\..\rtxc\xa\lib\librtl.a -o$@ $**
#|
	@$(XALINK) -f &&|
	 $(XALFLAGS) -o$(@B).out ..\..\rtxc\xa\lib\librtxc.a ..\..\rtxc\xa\lib\librtio.a ..\..\rtxc\xa\lib\libdrv.a ..\..\rtxc\xa\lib\librtl.a $**
#	 $(XALFLAGS) -Wlk -emEDE=\"$(Target1).i\" -o$(@B).out ..\..\rtxc\xa\lib\librtxc.a ..\..\rtxc\xa\lib\librtio.a ..\..\rtxc\xa\lib\libdrv.a ..\..\rtxc\xa\lib\librtl.a $**
|
#	@echo +$(XALOC) -o$@ $(@B).out -emEDE=\"$(Target1).i\" $(XALCFLAGS)
	@$(XALOC) -o$@ $(@B).out -emEDE=\"$(Target1).i\" -f &&|
	$(XALCFLAGS)
|
#	@MOVE $(@R).map $(OUTPUTDIR1)


$(OUTPUTDIR1)\main.obj: \
	..\..\Code\init\xa\1_0\main.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\ptvmain.obj: \
	..\..\Code\ptvmain\1_0\ptvmain.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\start.obj: \
	..\..\Code\init\xa\1_0\start.asm

$(OUTPUTDIR1)\isrs.obj: \
	..\..\Code\init\xa\1_0\isrs.asm

$(OUTPUTDIR1)\rtxcobj.obj: \
	..\..\Code\rtxcobj\0_3\rtxcobj.c \

#$(OUTPUTDIR1)\rtxcbug.obj: \
#	..\..\app_spec\xa\rtxcbug.c \
#	..\..\app_spec\xa\rtxcbug.h

$(OUTPUTDIR1)\rs232.obj: \
	..\..\Code\chndrv\xa\rs232\1_1\rs232.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\sio0drv.obj: \
	..\..\Code\chndrv\xa\sio0drv\1_0\sio0drv.c \
	$(INCDIR)\sio0drv.h

$(OUTPUTDIR1)\i2c_drv.obj: \
	..\..\Code\chndrv\xa\iic\1_1\i2c_drv.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\nv_drv.obj: \
	..\..\Code\devdrv\nv_drv\1_0\nv_drv.c \
	$(INCDIR)\nv_drv.h

$(OUTPUTDIR1)\ext0drv.obj: \
	..\..\Code\devdrv\xa\ext0drv\1_0\ext0drv.c \
	$(INCDIR)\ext0drv.h

$(OUTPUTDIR1)\tim0drv.obj: \
	..\..\Code\devdrv\xa\tim0drv\1_0\tim0drv.c \
	$(INCDIR)\tim0drv.h

$(OUTPUTDIR1)\led_hw.obj: \
	..\..\Code\devdrv\xa\leds\1_1\led_hw.c \
	$(INCDIR)\led_hw.h \

$(OUTPUTDIR1)\key_hw.obj: \
	..\..\Code\devdrv\xa\KeyBoard\1_0\key_hw.c \
	$(INCDIR)\key_hw.h \

$(OUTPUTDIR1)\plddrv.obj: \
	..\..\Code\devdrv\xa\plddrv\1_0\plddrv.c \
	$(INCDIR)\plddrv.h \

$(OUTPUTDIR1)\tvp6kdrv.obj: \
	..\..\Code\devdrv\xa\tvp6kdrv\1_1\tvp6kdrv.c \
	$(INCDIR)\tvp6kdrv.h \

$(OUTPUTDIR1)\unitprg.obj: \
	..\..\Code\devdrv\xa\unitprg\1_0\unitprg.c \
	$(INCDIR)\unitprg.h \

$(OUTPUTDIR1)\cbar.obj: \
	..\..\Code\cbar\1_1\cbar.c \
	$(INCDIR)\cbar.h \

$(OUTPUTDIR1)\audio.obj: \
	..\..\Code\audio\1_1\audio.c \
	$(INCDIR)\audio.h \

$(OUTPUTDIR1)\genlock.obj: \
	..\..\Code\genlock\1_1\genlock.c \
	$(INCDIR)\genlock.h \

$(OUTPUTDIR1)\bb.obj: \
	..\..\Code\bb\1_1\bb.c \
	$(INCDIR)\bb.h \

$(OUTPUTDIR1)\instru.obj: \
	..\..\Code\instru\1_1\instru.c \
	$(INCDIR)\rtxcobj.h \

$(OUTPUTDIR1)\keyscan.obj: \
	..\..\Code\front\1_0\keyscan.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\front.obj: \
	..\..\Code\front\1_0\front.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\interp.obj: \
	..\..\Code\interp\1_0\interp.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR1)\rs232cmd.obj: \
	..\..\Code\interp\1_7\rs232cmd.c \
	$(INCDIR)\rs232cmd.h

$(OUTPUTDIR1)\rs232par.obj: \
	..\..\Code\interp\1_0\rs232par.c \
	$(INCDIR)\rs232par.h

$(OUTPUTDIR1)\rs232err.obj: \
	..\..\Code\interp\1_0\rs232err.c \
	$(INCDIR)\rs232err.h

$(OUTPUTDIR1)\util.obj: \
	..\..\Code\interp\1_0\util.c \
	$(INCDIR)\util.h

$(OUTPUTDIR1)\ctask.obj: \
	..\..\rtxcdef\xa\ctask.c \
	..\..\rtxcdef\xa\ctask.h

$(OUTPUTDIR1)\cclock.obj: \
	..\..\rtxcdef\xa\cclock.c \
	..\..\rtxcdef\xa\cclock.h

$(OUTPUTDIR1)\cmbox.obj: \
	..\..\rtxcdef\xa\cmbox.c \
	..\..\rtxcdef\xa\cmbox.h

$(OUTPUTDIR1)\cpart.obj: \
	..\..\rtxcdef\xa\cpart.c \
	..\..\rtxcdef\xa\cpart.h

$(OUTPUTDIR1)\cqueue.obj: \
	..\..\rtxcdef\xa\cqueue.c \
	..\..\rtxcdef\xa\cqueue.h

$(OUTPUTDIR1)\cres.obj: \
	..\..\rtxcdef\xa\cres.c \
	..\..\rtxcdef\xa\cres.h

$(OUTPUTDIR1)\csema.obj: \
	..\..\rtxcdef\xa\csema.c \
	..\..\rtxcdef\xa\csema.h

$(OUTPUTDIR1)\cchnls.obj: \
	..\..\rtxcdef\xa\cchnls.c \
	..\..\rtxcdef\xa\cchnls.h

$(OUTPUTDIR1)\ccntlr.obj: \
	..\..\rtxcdef\xa\ccntlr.c \
	..\..\rtxcdef\xa\ccntlr.h

$(OUTPUTDIR1)\cdevice.obj: \
	..\..\rtxcdef\xa\cdevice.c \
	..\..\rtxcdef\xa\cdevice.h


#--------------- Target2 -------------------------------
Target2=PC5201
TargetType=PC

PCCC=BCC
PCAS=TASM
PCLINK=TLINK


PCCFLAGS=-v -f -I..\..\rtxcdef\pc -I..\..\rtxc\pc\include -I$(INCDIR) -I..\..\app_spec\pc -Ic:\bc4\include -ml
PCAFLAGS=
PCLFLAGS=-v -m -s -LC:\bc4\lib;..\..\rtxc\pc\lib

OUTPUTDIR2=.\temp\pc
TARGETDIR2=.\temp

.c{$(OUTPUTDIR2)}.obj:
#	@ECHO _
#	@ECHO Compiling for PC: $** - $@
#	@echo +$(PCCC) -c $(PCCFLAGS) -o$@ $**
	@$(PCCC) @&&|	      
	-c $(PCCFLAGS) -o$@ $**
|


# Target2Dependencies:

$(TARGETDIR2)\$(Target2).exe:\
 $(OUTPUTDIR2)\main.obj \
 $(OUTPUTDIR2)\ptvmain.obj \
 $(OUTPUTDIR2)\rtxcobj.obj \
#channel drivers
 $(OUTPUTDIR2)\rs232.obj \
 $(OUTPUTDIR2)\sio0drv.obj \
#device drivers
 $(OUTPUTDIR2)\instru.obj \
 $(OUTPUTDIR2)\interp.obj \
# $(OUTPUTDIR2)\rs232cmd.obj \
# $(OUTPUTDIR2)\rs232par.obj \
# $(OUTPUTDIR2)\rs232err.obj \
 $(OUTPUTDIR2)\util.obj \
 $(OUTPUTDIR2)\ctask.obj \
 $(OUTPUTDIR2)\cclock.obj \
 $(OUTPUTDIR2)\cmbox.obj \
 $(OUTPUTDIR2)\cpart.obj \
 $(OUTPUTDIR2)\cqueue.obj \
 $(OUTPUTDIR2)\cres.obj \
 $(OUTPUTDIR2)\csema.obj
#	@ECHO _
	@ECHO ***** Linking for PC :  - $@
#	@echo +$(PCLINK) $(PCLFLAGS) -e$@ $**
#	@$(PCLINK) @&&|
#	$(PCLFLAGS) -e$@ $**
#	|
#	@echo +$(PCLINK) $(PCLFLAGS) cl0 $** ,$*,$*,emu mathl cl librtxc libdrv
#	$(PCLINK) $(PCLFLAGS) c0l+@&&|
#	$**
#|,$*,$*,librtxc libdrv librtl emu mathl cl
	$(PCLINK) $(PCLFLAGS) c0l+@&&|
	$**,$*,$*,librtxc librtl libdrv emu mathl cl
|
#	@MOVE $(@R).map $(OUTPUTDIR2)



$(OUTPUTDIR2)\main.obj: \
	..\..\Code\init\pc\main.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR2)\ptvmain.obj: \
	..\..\Code\ptvmain\0_1\ptvmain.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR2)\rtxcobj.obj: \
	..\..\Code\rtxcobj\0_3\rtxcobj.c \

$(OUTPUTDIR2)\rs232.obj: \
	..\..\Code\chndrv\pc\rs232\0_1\rs232.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR2)\sio0drv.obj: \
	..\..\Code\chndrv\pc\sio0drv\0_1\sio0drv.c \
	$(INCDIR)\sio0drv.h

$(OUTPUTDIR2)\LED_hw.obj: \
	..\..\Code\devdrv\xa\LEDs\work\LED_hw.c \
	$(INCDIR)\LED_hw.h \

$(OUTPUTDIR2)\but_hw.obj: \
	..\..\Code\devdrv\xa\PushButton\work\but_hw.c \
	$(INCDIR)\but_hw.h \

$(OUTPUTDIR2)\instru.obj: \
	..\..\Code\instru\work\instru.c \
	$(INCDIR)\rtxcobj.h \

$(OUTPUTDIR2)\interp.obj: \
	..\..\Code\interp\0_1\interp.c \
	$(INCDIR)\rtxcobj.h

$(OUTPUTDIR2)\rs232cmd.obj: \
	..\..\Code\interp\work\rs232cmd.c \
	$(INCDIR)\rs232cmd.h

$(OUTPUTDIR2)\rs232par.obj: \
	..\..\Code\interp\0_1\rs232par.c \
	$(INCDIR)\rs232par.h

$(OUTPUTDIR2)\rs232err.obj: \
	..\..\Code\interp\0_1\rs232err.c \
	$(INCDIR)\rs232err.h

$(OUTPUTDIR2)\util.obj: \
	..\..\Code\interp\0_1\util.c \
	$(INCDIR)\util.h

$(OUTPUTDIR2)\ctask.obj: \
	..\..\rtxcdef\pc\ctask.c \
	..\..\rtxcdef\pc\ctask.h

$(OUTPUTDIR2)\cclock.obj: \
	..\..\rtxcdef\pc\cclock.c \
	..\..\rtxcdef\pc\cclock.h

$(OUTPUTDIR2)\cmbox.obj: \
	..\..\rtxcdef\pc\cmbox.c \
	..\..\rtxcdef\pc\cmbox.h

$(OUTPUTDIR2)\cpart.obj: \
	..\..\rtxcdef\pc\cpart.c \
	..\..\rtxcdef\pc\cpart.h

$(OUTPUTDIR2)\cqueue.obj: \
	..\..\rtxcdef\pc\cqueue.c \
	..\..\rtxcdef\pc\cqueue.h

$(OUTPUTDIR2)\cres.obj: \
	..\..\rtxcdef\pc\cres.c \
	..\..\rtxcdef\pc\cres.h

$(OUTPUTDIR2)\csema.obj: \
	..\..\rtxcdef\pc\csema.c \
	..\..\rtxcdef\pc\csema.h


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
 $(TARGETDIR1)\$(Target1).hex \
 $(TARGETDIR2)\$(Target2).exe 


libs: \
 $(TARGETDIR3)\$(Target3).a


