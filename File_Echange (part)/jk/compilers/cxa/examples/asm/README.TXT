TASKING XA Family Cross-Assembler

Assembler sample program

1996 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.4 12/19/96


----------------------------------------------------------------------
DESCRIPTION
----------------------------------------------------------------------

The program is a function that calculates the factorial of a value.
The XA assembly language program has a startup part, an init part and
the main part. The result of this example is an executable which
shows the usage of the recursive function "factorial".


----------------------------------------------------------------------
BUILDING THIS EXAMPLE FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE you can build this example by loading the
project file asm.pjt into the EDE. This project file is configured to
build the example for the 80c51XA and derivatives.

You can load a project file in the EDE using the Project | Open Menu.
After the project is opened you can press the 'make' button to
build the examples. Pressing the debugger button will bring up the
CrossView Simulator with the configuration appropriate to the
selected project file.


----------------------------------------------------------------------
BUILDING THIS EXAMPLE FROM THE COMMAND PROMPT
----------------------------------------------------------------------

This example uses a makefile for building the absolute file.  Make the
'asm' directory the current working directory.  Ensure that the bin
directory of the product is included in the search path, so that the
compiler, assembler, etc. can be found. Then type:

	mkxa

To remove the generated files type:
	
	mkxa clean

You can use the following macro definition for mkxa:

	V		set verbose flag of ccxa

For example:
	
	mkxa V=


			       -.-.-.-

