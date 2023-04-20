TASKING XA Family CrossView Debugger

I/O routines and example program

1996 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.5 10/28/97


----------------------------------------------------------------------
DESCRIPTION
----------------------------------------------------------------------

All examples that produce 'screen' output or user keyboard input use
simulated I/O (default). The low-level I/O functions _ioread() and
_iowrite() called by the C library I/O functions use simulated I/O by
default.  Simulated I/O is done by calling the functions _simi and
_simo. When XVW is told to do simulated I/O it sets breakpoints on
these functions. The functions are defined in the C library.  When one
of the functions is called, the breakpoint is hit and XVW reads or
writes the data. XVW users can see the 'CrossView Debugger User's
Guide' for more information about simulated I/O.

When the C library I/O functions are used for I/O on the hardware
(e.g., a serial port), the _iowrite() and _ioread() functions
must be adapted to support that hardware.

This directory contains a small example program that prints some text
and reads a number and reads text. The program uses simulated I/O.


----------------------------------------------------------------------
BUILDING THIS EXAMPLE FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE you can build this example by loading the
project file io.pjt into the EDE. This project file is configured to
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
'io' directory the current working directory.  Ensure that the bin
directory of the product is included in the search path, so that the
compiler, assembler, etc. can be found. Then type:

	mkxa

The program is build for the small memory model. The output of the
printf statement is done using simulated output.

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


----------------------------------------------------------------------
RUNNING THE PROGRAM
----------------------------------------------------------------------

The CrossView debugger can be used to run the program. The output is
printed on the 'stdout' stream. By default the program uses simulated
output. So, enabling stream number 1 for simulated output on screen is
required to get output when running the programs.

In the CrossView Pro Debugger:

- load the program using the menu File | Load Application...

- reset the target board, using the menu Run | Program Reset

- enable simulated output stream 1 and simulated input stream 0 using
  the dialog that pops up with the menu Debug | Simulated I/O Setup...

- run the program with the menu Run | Run


			       -.-.-.-
