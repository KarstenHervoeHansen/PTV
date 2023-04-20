TASKING 80C51XA Family CrossView Pro debugger

Crossview Pro tutorial session

1997 Copyright TASKING, Inc.

Document Id: @(#)readme.txt	1.6 


----------------------------------------------------------------------
DESCRIPTION
----------------------------------------------------------------------

This is  an example to  show how  to debug a  C  application using the
CrossView Pro XA  Debugger. Before   debugging the application   first
build the example  using EDE or  use the makefile as  described below.
For  a detailed description, see chapter  'Overview' of the TASKING XA
CrossView Pro Debugger Manual (Building Your Executable).

You can use the following as a small tutorial session:

When the program  is  loaded in the   debugger using the "File |  Load
Application" dialog, you can run the program as follows:

	First open some windows from the View menu (e.g. Source,
	Register and Stack).

	Then enable the simulated I/O streams that are used in the
	program with the "Debug | Simulated I/O" dialog. Activate
	stream 0 for input and 1 for output.
	
	Reset the program with the menu "Run | Program reset".

	Run the program with the menu "Run | Run" or with the "Run"
	button in the Source window.

	A simulated output window will pop-up with "fail" in it.

Probably something went  wrong. A debugger  is used to find  bugs. So,
the demo.abs program contains two bugs:

	The first is caused by a for loop that fills an array. However
	the loop makes  one iteration too much  and writes outside the
        array boundary. That caused the printing of the "fail" instead
	of "pass".

	The second is a infinite loop because the loop counter remains
	one.

By using   breakpoints   with an  action,  it is   possible  to make a
temporary  work-around. To make  the  definition of  these breakpoints
easier a play back  file is included  in the examples directory.   The
first break point  checks when the loop counter  in the first for loop
exceeds  the array boundary and when  this  happens it repositions the
program counter  to  the line after the  for  loop and  then continues
execution. The second breakpoint is set on  the infinite loop will set
the  loop counter  to zero  and    then continues  execution.  Do  the
following to load the play back file that sets these breakpoints:

	To break out the infinite loop first halt the program using
	the "Run | Halt" menu or with the "Halt" button in the Source
	or in the Command window.

	Load the play back file "demo.cmd" using the "Options |
	Playback" dialog.

	You can see that the breakpoints are set with the "Options |
	Breakpoints" dialog.

	Now run the program again. First reset the program and then
	run it.

	The simulated output window will pop-up with "pass" and "Do
	you want to ring the bell (y or n)? ". Then the simulated
	input dialog pops up. Enter 'y' or 'n'. A 'beep' will sound if
	entered 'y' and the text "End of demo" is printed in the
	simulated output window.


This first  run already showed  a few of the  powerful features of the
CrossView  Pro debugger. Please see  the CrossView Pro users manual or
the  on-line help  for more  information on  the features of CrossView
Pro.


----------------------------------------------------------------------
REBUILDING THE APPLICATION FROM THE TASKING EDE
----------------------------------------------------------------------

When using the Tasking EDE  you can build  this example by loading the
project file demo.pjt into the EDE. This project file is configured to
build the example for the 80c51XA and derivatives.

You can load a project file in the EDE  using the Project | Open Menu.
After the  project is opened you can  press the 'make' button to build
the examples. Pressing the debugger button will bring up the CrossView
Simulator with the  configuration appropriate to the  selected project
file.


----------------------------------------------------------------------
REBUILDING THE APPLICATION FROM THE COMMAND PROMPT
----------------------------------------------------------------------

When not using TASKING EDE you can build the example as follows.
This example uses a makefile for building the absolute file.  Make the
'xvw' directory the current working directory.  Ensure that the bin
directory of the product is included in the search path, so that the
compiler, assembler, etc. can be found. Then type:

	mkxa

The program is build for the default model (mixed model).

To remove the generated files type:
	
	mkxa clean

It might be necessary to run 'mkxa clean' before rebuilding the
application the first time because the demo.abs is already present.


			       -.-.-.-

