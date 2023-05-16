TASKING XA Family Cross-Compiler

Miscellaneous C examples

1996 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.6 1/22/97


----------------------------------------------------------------------
DESCRIPTION
----------------------------------------------------------------------

This directory contains the following examples:


1. A file showing the use of intrinsic functions and the generated
   code. The result of this example is not executable. The purpose is
   showing the generated code for each intrinsic function.
   This example is not part of the demo product.


2. A file showing the use of interrupt functions and the generated
   code. The result of this example is not executable. The purpose is
   showing the generated code.


----------------------------------------------------------------------
BUILDING THIS EXAMPLE FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE you can build this example by loading the
project file builtin.pjt or intrpt.pjt into the EDE. This project file
is configured to build the example for the 80c51XA and derivatives.

You can load a project file in the EDE using the Project | Open Menu.
After the project is opened you can press the 'make' button to
build the examples.


----------------------------------------------------------------------
BUILDING THIS EXAMPLE FROM THE COMMAND PROMPT
----------------------------------------------------------------------

This example uses a makefile for building the absolute file.  Make the
'c' directory the current working directory.  Ensure that the bin
directory of the product is included in the search path, so that the
compiler, assembler, etc. can be found. Then type:

	mkxa

To remove the generated files type:
	
	mkxa clean

The intrnsic.c and intrpt.c source files are by default translated in
the small model.

You can use the following macro definitions for mkxa:

	V		set verbose flag of ccxa
	LARGE		build for large memory model

For example:
	
	mkxa LARGE=


			       -.-.-.-
