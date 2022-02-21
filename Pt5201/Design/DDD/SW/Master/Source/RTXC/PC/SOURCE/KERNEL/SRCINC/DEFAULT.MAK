#
#   RTXC    Version 3.2
#   Copyright (c) 1986-1998.
#   Embedded System Products, Inc.
#   ALL RIGHTS RESERVED
#

#
# filename = default.mak (default to be included in other makefiles)
#
# makefile for Borland C++ v5.0x using Borland make v4.0
#

#DR = c:
DR = d:\tools

#
# Tool Directories
#
BC = d:\bc5
#BC = c:\bc5         # root path for Borland C++ distribution
		    # ** $(BC)/bin not necessary to be in PATH **
		    # typically LIB=$(BC)\lib and INCLUDE=$(BC)\include
		    # defined in environment or $(BC)\bin\turbo.cfg file
		    #

#
# Tools
#
CC = $(BC)\bin\bcc                 # C Compiler
AS = $(BC)\bin\tasm                # Assembler
AR = $(BC)\bin\tlib                # Librarian
LD = $(BC)\bin\tlink               # Linker
RM = del                           # Delete - remove intermediate files   

#
# Paths
#
INC = ..\..\Include          # Include directory
LIB = ..\..\Lib              # Library directory  
TMP = .                      # scratch files in local directory
