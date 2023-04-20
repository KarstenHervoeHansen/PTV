TASKING XA Family Cross-Compiler

EXAMPLES

1996 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.3 12/17/96


----------------------------------------------------------------------
OVERVIEW
----------------------------------------------------------------------

This examples directory contains several subdirectories with example
programs. Each subdirectory contains a makefile which can be
processed by mkxa to build the example.

For information about the examples, see the readme.txt file in the
subdirectory.


----------------------------------------------------------------------
BUILDING THE EXAMPLES FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE you can build the examples by loading the
project files ( .pjt) into the EDE. The project files are configured
to build the examples for the 80c51XA and derivatives.

You can load a project file in the EDE using the Project | Open Menu.
After the project is opened you can press the 'make' button to
build the examples. Pressing the debugger button will bring up the
CrossView Simulator with the configuration appropriate to the
selected project file.


----------------------------------------------------------------------
BUILDING THE EXAMPLES FROM THE COMMAND PROMPT
----------------------------------------------------------------------

For building an example program, make the directory containing the 
example the current working directory.  Build the example by typing:

	mkxa

When the example has already been built before, only the parts which
are out of date are rebuilt.

In the directory of this README file you can also find a
makefile. This makefile is common for the assembler package, the C
compiler package and the CrossView package.  Which examples are
installed depends on which packages are installed. Which examples are
built is determined by the existence of certain examples.

For building all examples you should add the bin directory of the
installed product to the search path and type:
	
	mkxa

The C examples are built in the small memory model by default.  For 
screen I/O the C examples use the simulated I/O feature of CrossView.
By defining macros on the command line you can steer the way the 
examples are build. A macro is defined by <macroname>=<replacement>.
The following macros can be defined:

	LARGE=  	Translate all C examples in the large memory
			model.

	V=  		Set verbose mode of the steering program. If
			set it shows the invocations of the separate 
			tools.

Example:
	
	mkxa V= LARGE=  

When you want to re-translate the examples with other settings you
should first clean up the results of a previous translation. This can
be done by:
	
	mkxa clean

You can also use this when you just want to clean up the example
directories.


			       -.-.-.-
