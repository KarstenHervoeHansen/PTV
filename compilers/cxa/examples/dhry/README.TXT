TASKING XA Family Cross-Compiler

Dhrystone Benchmark programs

1996 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.5 1/28/98


----------------------------------------------------------------------
DESCRIPTION
----------------------------------------------------------------------

The Dhrystone  benchmark  is   a  standard benchmark.  The   following
description is given in the file "dhry2.h",  which you are referred to
for more information.

  The following program contains statements of a high level
  programming language (here: C) in a distribution considered 
  representative:           

    assignments                  51.0 %
    control statements           32.4 %
    procedure, function calls    16.7 %

  103 statements are dynamically executed. The program is balanced 
  with respect to the three aspects:                                             

    - statement type
    - operand type
    - operand locality
         operand global, local, parameter, or constant.                     

This example contains the following version and files of the Dhrystone
benchmark:

	version: C/2.1
	    - dhry.h
	    - dhry_1.c
	    - dhry_2.c


----------------------------------------------------------------------
BUILDING THIS BENCHMARK FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE you can build this benchmark by loading the
project file dhry_1.pjt into the EDE.  This project file is configured
to build the benchmark for the 80c51XA and derivatives.

You can load a project file in the EDE using the  Project | Open Menu.
After the  project is opened you can  press the 'make' button to build
the benchmark.    Pressing  the debugger button    will   bring up the
CrossView Simulator with the configuration appropriate to the selected
project file.


----------------------------------------------------------------------
BUILDING THIS BENCHMARK FROM THE COMMAND PROMPT
----------------------------------------------------------------------

The directory  of  the  Dhrystone benchmark  contains  a  makefile for
building the absolute files.  Make  this directory the current working
directory. Ensure that the bin directory of the product is included in
the  search  path,  so  that  the compiler,   assembler,  etc.  can be
found. Then type:

	mkxa

The programs are build for the small memory model.  The output of the
print statements is performed via simulated I/O.

To remove the generated files type:
	
	mkxa clean

You can use the following macro definitions for mkxa:

	V		set verbose flag of ccxa
	LARGE		build for large memory model
	SIZE		build for optimised size

For example:
	
	mkxa LARGE= V=

Builds the program for large memory model and sets the verbose flag
of ccxa.

Please run 'mkxa clean' before building the benchmark for an other
configuration to prevent mixing of different configurations.


			       -.-.-.-
