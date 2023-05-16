TASKING XA Family Cross-Compiler

Whetstone Benchmark program

1996 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.5 12/19/96


----------------------------------------------------------------------
DESCRIPTION
----------------------------------------------------------------------

Whetstone benchmark in C.  This program is a translation of the
original Algol version in "A Synthetic Benchmark" by H.J. Curnow and
B.A. Wichman in Computer Journal, Vol 19 #1, February 1976.

Used to test compiler optimization and floating point performance.
 

----------------------------------------------------------------------
BUILDING THIS BENCHMARK FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE you can build this benchmark by loading the
project file whet.pjt into the EDE. This project file is configured to
build the benchmark for the 80c51XA and derivatives.

You can load a project file in the EDE using the Project | Open Menu.
After the project is opened you can press the 'make' button to
build the benchmark. Pressing the debugger button will bring up the
CrossView Simulator with the configuration appropriate to the
selected project file.


----------------------------------------------------------------------
BUILDING THIS BENCHMARK FROM THE COMMAND PROMPT
----------------------------------------------------------------------

The directory of the Whetstone benchmark contains a makefile for
building the absolute file.  Make this directory the current working
directory. Ensure that the bin directory of the product is included in
the search path, so that the compiler, assembler, etc. can be
found. Then type:

	mkxa

The program is built for the small memory model.  The output of the
print statements is done via simulated I/O.

To remove the generated files type:
	
	mkxa clean

You can use the following macro definitions for mkxa:

	V		set verbose flag of ccxa
	LARGE		build for large memory model
	SIZE		build for optimised size
	SINGLE		build for single precision
	RESULTS		print intermediate results


For example:
	
	mkxa LARGE= RESULTS=

Builds the program for large memory model and print intermediate
results.

Please run 'mkxa clean' before building the benchmark for another
configuration to prevent mixing of different configurations.



			       -.-.-.-
