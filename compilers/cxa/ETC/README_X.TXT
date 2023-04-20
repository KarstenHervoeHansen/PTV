-------------------------------------------------------------------------------
XA Simulator Debugger

Readme

Copyright (c) 1998 TASKING, Inc.

Document date: 8/1/98

-------------------------------------------------------------------------------
INTRODUCTION
-------------------------------------------------------------------------------
This file describes the solved and the known problems of version 2.0
revision 0 of the XA toolchain.


-------------------------------------------------------------------------------
SOLVED PROBLEMS
-------------------------------------------------------------------------------
SOLVED PR7049: LOC. DESCRIPTION FILE LOADED TOO OFTEN

-------------------------------------------------------------------------------
SOLVED PR7050: DESCRIPTION FILE NAME IS USED AS TITLE OF REGISTER WIN

-------------------------------------------------------------------------------
SOLVED PR7052: XVWDEMO: . * 7 + 1 => DATA:$74 = 120 ADDRESS SUPERFLUOUS

PROBLEM DESCRIPTION:

The address must not be printed.

-------------------------------------------------------------------------------
SOLVED PR7054: NO DERIVATIVE SPECIFIC REGISTER DEF. FILES SUPPLIED

-------------------------------------------------------------------------------
SOLVED PR7123: STARTUP OPTIONS DLG: DESCR FILE NEEDS PATH

PROBLEM DESCRIPTION:

If the startup options dialog is closed using the OK button it's verified
if the locator description file can be found. However, no search path is
used so an error is given. Although if you leave the dlg using cancel
everything will work fine afterwards

-------------------------------------------------------------------------------
SOLVED PR7130: SIMULATOR ERROR: ADDRESS WRAP-AROUND

PROBLEM DESCRIPTION:

R7==0xfff8

mov [r7+#12],r1 ==> buserror, on 1010003

Problem: result of address computation r7+#12 doesn't seem to be wrapped
properly.

-------------------------------------------------------------------------------
SOLVED PR7221: (short)123 NOT ACCEPTED

PROBLEM DESCRIPTION:

Short is a type and a memory space qualifier => conflict.

-------------------------------------------------------------------------------
SOLVED PR7240: MULTIPLE SOFTWARE INTR SIMULTANEOUS DEFUNCT

PROBLEM DESCRIPTION:

If multiple bits are written in SWR only one interrupt is serviced.

-------------------------------------------------------------------------------
SOLVED PR7376: FAR CALL DEFUNCT

PROBLEM DESCRIPTION:

A far call fails if the target address is located in a different segment.
This problem only occurs on MS-Windows, on UNIX the simulator behaves
correct.

-------------------------------------------------------------------------------
SOLVED PR7426: SIMULATOR BUGS: LSR, ASL and ASR on double word operands

PROBLEM DESCRIPTION:

This bug occurs on MS-Windows only.

When updating PSW the simulator does not look at the contents of the
high word in case of a LSR, ASL and ASR instruction.

-------------------------------------------------------------------------------
SOLVED PR7524: LIST PUSH/POP ORDER NOT CORRECTLY SIMULATED

PROBLEM DESCRIPTION:

Push and pop with a list of operands push and pop these operands in the
wrong order.

EXAMPLE:

PUSH R1,R0

is simulated as :

PUSH R0
PUSH R1

it should be :

PUSH R1
PUSH R0

-------------------------------------------------------------------------------
SOLVED PR7794: CrossView mixup with USP and SSP

PROBLEM DESCRIPTION:

Underneath a sample which demonstrates the stackpointer
confusion by the >> CrossView Simulator <<:
CLR PSWH.5     ; BANK 0
CLR PSWH.4
MOV R7,#0X8000 ; SSP = R7 = 8000H
MOV R1,USP     ; R1 = 0100H
PUSHU R1
MOV R1,USP     ; R1 = USP = 00FEH
PUSH R0        ; SSP = R7 = 7FFEH
CLR PSWH.5     ; BANK 1
SETB PSWH.4
MOV R7,0X7F90  ; SSP = R7 = 7F90H
MOV R1,USP     ; USP = 7FFE <= old SSP !

WORKAROUND:

unfortunatly none

-------------------------------------------------------------------------------
SOLVED PR7811: Improper Indirect addressing from CrossView

PROBLEM DESCRIPTION:

The following example :

typedef signed char SBYTE_t ;
_huge SBYTE_t anArray[10] = {0,1,2,3,4,5,6,7,8,9} ;
void main(void) {}

Runs OK in CrossView. I took a closer look and noticed the following. Before
copying the digits 0 through 9 from ROM to RAM CS and ES are initialised.
SSEL is also initialised: R1SEG and R3SEG are set. This all appears to
indicate that R1 and R3 will be used for the copy. However this is not the
case; R2 and R0 are used instead. As their bits are not set in SSEL the
unitialised value of DS and the current upper 8 bits of PC will be used to
generate the 24 bit addresses. In most cases for CrossView DS, ES and CS will
be 0x00. Also PC will be in the first page so its upper 8 bits will be 0x00.
This means that even if SSEL is setup wrong the copy will be successful. To
prove a copy might fail in cases I changed that values of DS, CS and ES to
point to other pages but to my surprise to copy was still successful. There
fore I can only conclude that either something's wrong with CrossView simu-
lator or I'm overlooking something.


-------------------------------------------------------------------------------
KNOWN PROBLEMS
-------------------------------------------------------------------------------
KNOWN PR7051: INITVAL/X => 0x00000011 TOO MANY ZEROS

-------------------------------------------------------------------------------
KNOWN CR7145: BREAKPOINT AT FOR STATEMENT, TRIGGER AT ITERATION

PROBLEM DESCRIPTION:

Suppose:

for (i=0; i<test; i++)
{
body;
}

At placing a breakpoint on the for() line, Crossview stops just before the
i++ action. Stopping at the i=0 point would be more appropriate.

-------------------------------------------------------------------------------
KNOWN PR7308: LIFETIME INFO NOT 100% RELIABLE

PROBLEM DESCRIPTION:

The lifetime information passed by the cxa compiler is not always
complete/correct. As result of this the debugger may access an "old"
memory location instead of the register if the variable is moved from
memory to register.

NOTE: the lifetime information that is available is handled correctly by
the debugger.

NOTE: most other toolchains don't support debugging of optimized code.

-------------------------------------------------------------------------------
KNOWN PR7309: FLOATING POINT VALUE NaN DISPLAYED AS Zero OR Inf

PROBLEM DESCRIPTION:

Floating point value "Not a Number" (NaN) is displayed sometimes as
0.0 (Zero) and sometimes as -INF or +INF (Infinite).

-------------------------------------------------------------------------------
KNOWN PR7310: DISASM OF MOV C,bit => BIT ADDRESS DEFUNCT

PROBLEM DESCRIPTION:

When disassembling MOV C,bit and MOV bit,C the bit address is not correct.

-------------------------------------------------------------------------------
KNOWN PR7336: XFW ON UNIX DOES NOT SUPPORT 16BIT COLOR

PROBLEM DESCRIPTION:

The following error message is shown when starting XFW on UNIX:

   fatal: FATAL ERROR: MSG 0x0034deb2 [CAT 3/4 STD 57010 ] Category: XVT
   release 3 assert (Signaled assert4) Function: xvt_app_create
   File: ./kpalet.c line: 480


This error indicates that you are using an unsupported visual mode in
your system. XFW supports 1-bit, 8-bit and 24-bit color.

-------------------------------------------------------------------------------
KNOWN PR7429: P3CFGA=0xE5; DISASSEMBLY: mov.b code:0x473,#-27

PROBLEM DESCRIPTION:

The disassembler puts an incorrect memory space in front of an addres.

In this case P3CFGA is located at address 0x473. This is an address in
SFR space, not in CODE space.

-------------------------------------------------------------------------------
KNOWN PR7521: EA-bit does not work properly on simulator

PROBLEM DESCRIPTION:

The EA bit should enable/disable -all- interrupts. The simulated version
of the EA-bit does not block software interrupts at all.

-------------------------------------------------------------------------------
KNOWN PR7563: ON_LINE HELP OF XVW-STARTUP DESCR FILE NOT EXPLAINED

PROBLEM DESCRIPTION:

The XVW-Startup dialog contains an edit field to specify the name of
a description file instead of an edit field to specify a CPU type.
The description file describes the memory map of the target. The XVW
simulator engine configures it's memory according the specification in
the description file.

The description file also contains a reference to the SFR file (regxag3.dat)
that will be used by XVW. In history this file name was derived from the
CPU type.

-------------------------------------------------------------------------------
KNOWN PR7565: DATA COVERAGE DIALOG MAX STACK USAGE DEFUNCT

PROBLEM DESCRIPTION:

The maximum stack usage displayed in the Data-Coverage dialog is not correct.
Also the stack's "start" and "end" labels are exchanged.

-------------------------------------------------------------------------------
KNOWN PR7566: SEARCH UP/DOWN ACCELERATOR BUTTONS EXCHANGED

PROBLEM DESCRIPTION:

The search behavior of the string search-up and search-down accelerator
buttons is different from what you would expect.

-------------------------------------------------------------------------------
KNOWN PR7577: XDAT MEMORY ACCESS DEFUNCT

PROBLEM DESCRIPTION:

The CrossView simulator does not correctly implement _xdat memory access.
Either the memory is not correcly mapped or the MOVX instruction is
not correctly simulated.

-------------------------------------------------------------------------------
KNOWN PR7580: SYSTEM/USER STACK POINTER SWITCH DEFUNCT

PROBLEM DESCRIPTION:

If the XA switches from system to user mode and vice versa the
stack pointer is not updated correctly.

-------------------------------------------------------------------------------
KNOWN PR7607: PROFILE DIALOG: GPF

PROBLEM DESCRIPTION:

The program will terminate with a GPF is the cumulative profiling window
is opened, closed, and then reopened.

-------------------------------------------------------------------------------
KNOWN PR7829: initializing code simulator error

PROBLEM DESCRIPTION:

If the 'copy' table is located outside the first code segment (>64k), the
simulator fails on the code responsible for loading the variables with
their initializors.

WORKAROUND:

Locate copy table within first code segment (0-64k)

-------------------------------------------------------------------------------
KNOWN PR7858: CrossView registers not updated

PROBLEM DESCRIPTION:

"... In CrossView Pro Debugger v1.1, I noticed that
TL0, TH0, TL1, TH1, ... registers didn't update in
the REGISTER WINDOW when modified in either assembly
language or C. I double clicked on these registers
in the REGISTER WINDOW to manual modify them. They
change value in the REGISTER WINDOW to whatever I
typed, but also change memory in the MEMORY WINDOW
(ie... while viewing DATA:0x450, etcetera).  My
program doesn't change the RAM location when I try
to modify these registers. ..."

EXAMPLE:

Using the following conventions:
RW:TL0   => contents TL0 of Register Window
DATA:450 => Data Address 450
SFR:450  => SFR Address 450
And the following program sample:
void main(void) { TL0 = 0xAA ; }
Before initilisation TL0:
RW:TL0 = 0xff, DATA:450 = 0xff, SFR:450 = 0xff
After initilisation TL0:
RW:TL0 = 0xff, DATA:450 = 0xff, SFR:450 = 0xaa
Manually changing RW:TL0 to 0 using double click method:
RW:TL0 = 0x00, DATA:450 = 0x00, SFR:450 = 0xaa

CrossView appears to retrieve the value for TL0 from DATA memory and shows
the value in the Register Window. In the program however it is retrieved from
the SFR space as can be proven when you append the following piece of code to
the program above: TL0 = TL0 == 0xaa ? 0x66 : 0x77 ;
In this case TL0 will be 0x66 and the register contents will be:
RW:TL0 = 0x00, DATA:450 = 0x00, SFR:450 = 0x66

WORKAROUND:

Use the following scheme:
#define DEBUG
#ifdef DEBUG
char peak ;
#define VIEW(val) (peak=val)
#else
#define VIEW(val)
#endif

void main(void) {
  TL0 = 0xaa ;
  VIEW(TL0) ;
}

And use the data window to show 'peak' i.e. TL0

