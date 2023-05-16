TASKING XA Family Cross-Compiler

Example Program Fptrap

1996 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.3 12/17/96


----------------------------------------------------------------------
DESCRIPTION
----------------------------------------------------------------------

This small C program demonstrates how to install an IEEE-754 type
exception handler in an application program, which allows the
intercepting of run-time floating point exceptions in basic
calculations like addition. This also optionally allows to replace the
resulting value with one of your own. It therefore installs a trap
handler, which is invoked on any floating point exception and handles
the exception in any way you like. This handler is installed

For more simple cases, where only the occurrence of such an exception
needs to be intercepted, see the fpsigfpe example.

You can use this example module with your application by specifying the
command line argument -DNODEMO to the compiler command.  This will
suppress the inclusion of the main function.  You can install the
exception handler by editing the function

	IEEE_fp_exception_handler

and calling

        install_fp_exception_handler( );

at the start of your application program.
NOTE: you *must* specify -fptrap to ccxa, in order to link
      with the appropriate library.

The output from this example application is written to standard output
using Simulated I/O from CrossView. Issue for example

	1 sio o screen

to have it written into a window of CrossView.


----------------------------------------------------------------------
BUILDING THIS EXAMPLE FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE you can build this example by loading the
project file fptrap.pjt into the EDE. This project file is configured
to build the example for the 80c51XA and derivatives.

You can load a project file in the EDE using the Project | Open Menu.
After the project is opened you can press the 'make' button to
build the examples. Pressing the debugger button will bring up the
CrossView Simulator with the configuration appropriate to the
selected project file.


----------------------------------------------------------------------
BUILDING THIS EXAMPLE FROM THE COMMAND PROMPT
----------------------------------------------------------------------

The directory of the Fptrap  example contains a makefile for building
the absolute   file.    Make  this   directory  the current    working
directory. Ensure that the bin directory of the product is included in
the  search  path,  so that  the compiler,    assembler,  etc. can  be
found. Then type:

	mkxa

The program  is build for  the small memory  model.  The output of the
print statements is done via simulated I/O.

To remove the generated files type:
	
	mkxa clean

You can use the following macro definitions for mkxa:

	V		set verbose flag of ccxa
	LARGE		build for large memory model

For example:
	
	mkxa LARGE=

Builds the  program for large memory  model.

Please run 'mkxa  clean' before  building the  examples  for an other
configuration to prevent mixing of different configurations.

			       -.-.-.-

