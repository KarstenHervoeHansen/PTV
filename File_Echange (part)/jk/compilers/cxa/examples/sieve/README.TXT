TASKING XA Family Cross-Compiler

Eratosthenes Sieve Prime Number Program in C

1996 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.4 12/19/96


----------------------------------------------------------------------
BUILDING THIS BENCHMARK FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE you can build this benchmark by loading the
project file sieve.pjt into the EDE. This project file is configured to
build the benchmark for the 80c51XA and derivatives.

You can load a project file in the EDE using the Project | Open Menu.
After the project is opened you can press the 'make' button to
build the benchmark. Pressing the debugger button will bring up the
CrossView Simulator with the configuration appropriate to the
selected project file.


----------------------------------------------------------------------
BUILDING THIS BENCHMARK FROM THE COMMAND PROMPT
----------------------------------------------------------------------

The directory of the Sieve benchmark contains a makefile for building
the absolute file.  Make this directory the current working directory.
Ensure that the bin directory of the product is included in the search
path, so that the compiler, assembler, etc. can be found. Then type:

	mkxa

The program is built for the small memory model.  The output of the
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

Please run 'mkxa clean' before building the benchmark for another
configuration to prevent mixing of different configurations.


			       -.-.-.-
