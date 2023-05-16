# TASKING EDE (Windows based integrated Embedded Development Environment)
#
# This makefile has been generated by TASKING EDE
#
# Toolchain: TASKING C for XA 2.0 r2
#

SHELLCMD=       break call cd chcp chdir cls copy ctty date del dir echo\
                erase exit for goto if md mkdir path pause prompt rd rem\
                ren rename rmdir set shift time type ver verify vol

BINDIR = C:\cxa\bin
INCDIR = C:\cxa\include
LIBDIR = C:\cxa\lib\xa

PROJ    = sieve
PROJDIR = c:\cxa\examples\sieve
PRODDIR = C:\cxa

all         : $(PROJ).abs 

sieve.src   : sieve.c
	C:\cxa\bin\cxa.exe sieve.c -f<<EOF
  $(EMPTY)  -IC:\cxa\include -Ms -So -w -A1 -O2 -DMEASURE_TIME -DPRINT -g
  EOF

sieve.obj   : sieve.src
	C:\cxa\bin\asxa.exe sieve.src -f<<EOF
  $(EMPTY)  ZP -gs -LcDElMnPQsWXy -OHjs -DMODEL_SMALL CA
  EOF

$(PROJ).out : sieve.obj sieve.i
	C:\cxa\bin\ccxa.exe -Wlk -emEDE=\"$(PROJ).i\" -o $@ -f<<EOF
  $(separate "\n" $(match .obj $!))
  $(separate "\n" $(match .a $!)) -cl    -Wlk-w8 -Ms -LC:\cxa\lib\xa
  EOF

$(PROJ).abs : $(PROJ).out sieve.i
	C:\cxa\bin\lcxa.exe -o $@ $(match .out $!) -emEDE=\"$(PROJ).i\" -f<<EOF
  $(EMPTY)  -f1 -M -w8 -dxa.dsc
  EOF

# Copyright (c) 1998 TASKING, Inc.
# Serial:	085795-Apt
# EDE Version:	2.0 r7
