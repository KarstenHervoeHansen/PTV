-------------------------------------------------------------------------------
XA Assembler Package

Readme

Copyright (c) 1998 TASKING, Inc.

Document date: 15/07/98

-------------------------------------------------------------------------------
INTRODUCTION
-------------------------------------------------------------------------------
Thank you for purchasing a TASKING product.
This file describes the solved and the known problems of version 2.0
revision 2 of the XA toolchain.


-------------------------------------------------------------------------------
SOLVED PROBLEMS
-------------------------------------------------------------------------------
SOLVED PR7602: Assembler optimizer problem

PROBLEM DESCRIPTION:

Assembler optimizer problem with forward references to absolute
sections.

EXAMPLE:

        CSEG at 4000h
breakp:
        jmp     breakp0

        CSEG at 4020h

        ds      100h

        ALIGN   1
breakp0:
end

WORKAROUND:

Disable range checking by making the address
expression more complicated. e.g. "jmp breakp0*2/2".


        CSEG at 4000h
breakp:
        jmp     breakp0*2/2

        CSEG at 4020h

        ds      100h

        ALIGN   1
breakp0:
end

-------------------------------------------------------------------------------
SOLVED PR7603: NOT defunc

PROBLEM DESCRIPTION:

Assembler operator NOT is implemented as a logical operator and
not as a binary operator.

EXAMPLE:

MOV R1,#NOT(2) ; MOV r1,#0FDH

WORKAROUND:

Use ~ instead of NOT

-------------------------------------------------------------------------------
SOLVED PR7631: assemblers hangs after jnb <bit>,loop instruction

PROBLEM DESCRIPTION:

When the instruction jnb R2.0,loop  or  jbc R2.0,loop are
being used the assembler hangs

EXAMPLE:

(the next line suffices)
JNB R2.0,loop

-------------------------------------------------------------------------------
SOLVED PR7653: ORG directive not supported

PROBLEM DESCRIPTION:

The ORG directive is not yet supported by the current
assembler.

EXAMPLE:

CSEG
    ORG 20

WORKAROUND:

Use absolute sections instead
   ORG 20
-> CSEG AT 20

-------------------------------------------------------------------------------
SOLVED PR7654: assembler is not case insensitive by default

PROBLEM DESCRIPTION:

The assembler by default should be case insensitive,
but it is case sensitive.

EXAMPLE:

CSEG
define abc 'R0'
    MOV abc,R1

WORKAROUND:

use the $NOCASE control in the assembly source,
or add NOCASE to the command line options.

For EDE: set NOCASE in the miscellaneous tab,
because EDE assumes the assembler to be case insensitive
by default and therefor adds nothing to the command line
options.

-------------------------------------------------------------------------------
SOLVED PR7655: assembly instruction  MOVC.B A,[A+PC] not recognized

PROBLEM DESCRIPTION:

The assembler doesn't recognize the following instructions:
  MOVC.B A,[A+PC]
  MOVC.B A,[A+DPTR]

WORKAROUND:

use MOVC A,[A+PC] instead.

-------------------------------------------------------------------------------
SOLVED PR7674: NUMBER directive defunc

PROBLEM DESCRIPTION:

The directive EXTERN NUMBER(abc) is not implemented

EXAMPLE:

   CSEG
   MOV R0, abc
   EXTRN NUMBER(abc)
   END

WORKAROUND:

replace symbolic number with constant value

-------------------------------------------------------------------------------
SOLVED CR7793: ASXA doesn't support non-generic JMP instruction

PROBLEM DESCRIPTION:

Sometimes you want to specify a JMP without the
assembler optimizing it into a BRA instruction

EXAMPLE:

When programming a JMP table:

_1:     JMP _entry1
        NOP
_2:     JMP _entry2
        NOP

Because the JMP instruction is 3 bytes one
NOP instruction is added to get a 4 byte
alignment. But when the assembler optimizes
the JMP into a BRA 2 NOPs would be necessary.
In this case you want to be able to specify
the real hardware instruction (e.g. JMP.L)

-------------------------------------------------------------------------------
SOLVED PR7801: #include "xxx.h" doesn't work when added to directories

PROBLEM DESCRIPTION:

Adding an include file directory to EDE doesn't result
in a proper compilation. It still returns the error
message : cannot find include file xxx.h

WORKAROUND:

The dol file doesn't contain a %list option for the
include files. Therefore they aren't added to the
optionstring. The following needs to be added:

OPTION {
  ID = CompIncDir
  TYPE = INVISIBLE
  STATE = SELECTED
  SELECTED = {
    OPTIONTEXT = "%list(;, ,-I%incdir)"
  }
}

To the compiler TAB

-------------------------------------------------------------------------------
SOLVED PR7808: Misc options of assembler TAB doesn't work

PROBLEM DESCRIPTION:

The user options entered into the Miscellaneous tab from the assembler aren't
passed onto the makefile.

EXAMPLE:

The following has to be added to cxa.dol :

SELECTED
{
  OPTIONTEXT = "%s"
}

It should be added to the OPTION "User options" of the BLOCK "User &options"
from the assembler tool. It will the look like this :

BLOCK {
  NAME = "User &options:"
  OPTION {
    NAME = "User options:"
    ID = O1017
    TYPE = EDITBOX
    STATE = ""
    SELECTED {
      OPTIONTEXT = "%s"
    }

WORKAROUND:

  }
}

-------------------------------------------------------------------------------
SOLVED PR8024: Assembler doesn't generate listfile by default.

PROBLEM DESCRIPTION:

Although the manual states the assembler generates a listfile
by default, it doesn't.

WORKAROUND:

Use the '-l' option.

-------------------------------------------------------------------------------
SOLVED PR8287: NOPR assembler control doesn't work

PROBLEM DESCRIPTION:

Using NOPR on the assembler command line results in the following
error message :

asxa E350: (no file) (commandline) : list file already defined

-------------------------------------------------------------------------------
SOLVED PR8336: 64 bit expression evaluation fails above 29 bits

PROBLEM DESCRIPTION:

Using the following bit of assembly:

DSEG
case:
DD 16384*32768 ; step 1
DD 32768*32768 ; step 2

Fails for step 2 whilst the assembler manual states that the evaluation is
being done in 64 2's complement precision.

The error can be identified in the list file:

000000                    7 DSEG
000000                    8 case:
000000 00000020           9 DD 16384*32768 ; step 1
000004 00800000          10 DD 32768*32768 ; step 2

WORKAROUND:

- Do the evaluation yourself -

-------------------------------------------------------------------------------
SOLVED PR8410: compound external problem

PROBLEM DESCRIPTION:

Ref manual page 7-42. EXTRN code(Idle), bit(IDL_EN) doesn't work; it
produces an error about expecting section name.

EXAMPLE:

See description

WORKAROUND:

Do not use compound externals

-------------------------------------------------------------------------------
SOLVED PR8500: linker error 209

PROBLEM DESCRIPTION:

For certain object modules the linker emits e209.

-------------------------------------------------------------------------------
SOLVED PR8794: Not possible to enable ASM debug

PROBLEM DESCRIPTION:

It is not possible from EDE to select assembler source line information
generation for ASM level debugging.

WORKAROUND:

Add -ga option to the Miscellaneous option field of the assembler
and select radio button:
Assembler/Code Generation/Debug Info/Emit ASM additional...

-------------------------------------------------------------------------------
SOLVED PR8897: asxa: syntax error: Error 206: '0' unexpected

PROBLEM DESCRIPTION:

"... Long Description: "Syntax error: Error 206: '0'
unexpected." Customer gets the above when compiling
code which has at line 28 the following line:
CMP.B [R1],#0x0d ...(compare R1 with the value of
0x0d) Additionally, the customer gets a warning:
Warning 118 inserted external "x0d".
The 0x notation for hex numbers is illegal assembler syntax.
See assembler manual paragraph 5.2.1. The assembler only
gives the wrong error for 0x0d and 0x0b.

EXAMPLE:

          cseg at 100h
begin:
          cmp.b [R1],#0x06
          cmp.b [R1],#0x0b
          end

--> assembler output:

asxa E206: line 4 : syntax error: "0" unexpected
asxa W118: line 4 : inserted "external X0B"

WORKAROUND:

The 0x notation for hexidecimal numbers is illegal notation.
Use the 'H' notation: #0bH

-------------------------------------------------------------------------------
SOLVED PR8967: Floating point run-time library failure

PROBLEM DESCRIPTION:

Next floating point operations are not handled correctly by the run-time
floating point libraries

EXAMPLE:

#include <stdio.h>

#define VAL3 -0.000000000004881
#define VAL2  0.000000000137769
#define VAL0  0.624240000000000

double   a3=VAL3;
double   a2=VAL2;
double   a0=VAL0;

void main(void)
{
    printf("%0.15f=%0.15f\n", VAL3+VAL0, a3+a0);
    printf("%0.15f=%0.15f\n", VAL2+VAL0, a2+a0);
}

WORKAROUND:

use single precision floating point only ('F' compiler option)

-------------------------------------------------------------------------------
KNOWN PROBLEMS
-------------------------------------------------------------------------------
KNOWN CR7092: OPTION "-Wlc-dmy_xa.sdc" NOT HANDLED CORRECTLY

PROBLEM DESCRIPTION:

Invocation: ccxa -Wlc-dmyxa.dsc
Result: lcxa -dmyxa.dsc -dxa_t.dsc
This is correct, because all tool specific options are passed without
checking. This is not a restriction but a feature of ccxa. To overrule
the automatic generated dsc file specification a -dsc file option could be
added to ccxa.

WORKAROUND:

Store contents of myxa.dsc in xa_t.dsc, which need to be located in
project directory.

-------------------------------------------------------------------------------
KNOWN PR7134: CODE IS NOT LOCATED CORRECTLY (HOWEVER IT RUNS)

PROBLEM DESCRIPTION:

The "reserved" command placed in the "amode code" block, which reserves
space for the vector table, results in locating the code at high memory
addresses.

-------------------------------------------------------------------------------
KNOWN PR7302: ASXA CONTROL "DATE" NOT IMPLEMENTED

PROBLEM DESCRIPTION:

Assembler control "DATE" is not implemented in the XA assembler.

-------------------------------------------------------------------------------
KNOWN PR7303: ASXA CONTROL "SAVE" & "RESTORE" NOT IMPLEMENTED

PROBLEM DESCRIPTION:

Assembler controls "SAVE" and "RESTORE" are not implemented in the XA
assembler.

-------------------------------------------------------------------------------
KNOWN PR7341: EDE: INCONVENIENT BEHAVIOR

PROBLEM DESCRIPTION:

Before the make starts
- I have to be sure to have at least one file opened

WORKAROUND:

This is "standard" CodeWright behavior.

-------------------------------------------------------------------------------
KNOWN PR7723: Line number info of preprocessed .asm files not used

PROBLEM DESCRIPTION:

After preprocessing a .asm files using mppxa, the generated .src
file contains line number information referring to the original
.asm file. When the assembler finds any errors in the .src file
it should generate an error message containing the line number
of the original .asm file.
The current assembler accepts line number information but doesn't
use it in error messages.

-------------------------------------------------------------------------------
KNOWN PR7764: mppxa generates premature EOF on macro

PROBLEM DESCRIPTION:

The following macro :

%*define(get(address))
(
  %if( (%NES(%address,one))AND(%NES(%address,two)) )
  then
    (mov dptr,#%address)
  fi
)

results in the following error message from mppxa:

**** ERROR 312 (case.asm, line #9) (0): PREMATURE EOF
INSIDE EXPANSION: GET

WORKAROUND:

Allow for a least one space between the parentheses and
AND. So it ought to look like this:

%*define(get(address))
(
  %if( (%NES(%address,one)) AND (%NES(%address,two)) )
  then
    (mov dptr,#%address)
  fi
)

This works fine but I do think it should have been
accepted by mppxa.

-------------------------------------------------------------------------------
KNOWN PR7765: E362 from asxa, actually caused by mppxa $INCLUDE contro

PROBLEM DESCRIPTION:

After preprocessing the following:

$INCLUDE(myfile.hlp) ; any comments

The output looks like this:

$                  ; any comments
# 1 "myfile.hlp"
; contents of myfile.hlp
# 13 "myasm.asm"

The string "; any comments" is prematurely included and
this triggers the assembler to report error message E362.

WORKAROUND:

- 1 - Do not use a comment after $INCLUDE control.

- 2 -

Use the INCLUDE assembler directive instead. This also
avoids using mppxa. Referring to the previous example it
will now look like:

INCLUDE "myfile.hlp" ; no error from this comment

The output will of coarse be an object file since mppxa
isn't used.

-------------------------------------------------------------------------------
KNOWN PR7876: mppxa: ERROR 301 undefined macro name

PROBLEM DESCRIPTION:

When assembling "fnt_n68.asm", the following message
appears:

ERROR 301 (fnt_n68.asm, line #59) "0): UNDEFINED
MACRO NAME INSIDE CALL DB

EXAMPLE:


The following line in this source file caused the error :

DB      0               ;$25    %

WORKAROUND:

Found that mppxa.exe - which is invoked for fnt_n68.asm -
can't handle a commented line containing a % character.
It seems to assume that it dealing with a macro which
explains the error message. It can be solved by changing
% to '%'

-------------------------------------------------------------------------------
KNOWN PR8309: Locating must conform to DELFEE also for 0-sized section

PROBLEM DESCRIPTION:

The following example :

/* a.c */
extern char _lc_b_B_CLR_FA[] ;
extern char _lc_e_B_CLR_FA[] ;
int y ;
void main(void) {
  while (_lc_e_B_CLR_FA-_lc_b_B_CLR_FA) ;
}
/* b.c */
#include "b.h"
/* b.h */
#pragma asm
B_CLR_FA        SEGMENT HDATA INSEGMENT CLEAR
#pragma endasm

Will generate labels __lc_{e,b}_B_CLR_FA = 0x800 when using the following
amode control:
amode hdata { section A_CLR_FA addr=0x800 ; section B_CLR_FA ; }
These labels should have been equal to or larger than  __lc_e_A_CLR_FA

-------------------------------------------------------------------------------
KNOWN PR8419: wrong output file from linker

PROBLEM DESCRIPTION:


For the following construction...

EXTRN  HDATA( _spow_power_system_task )
EXTRN  HDATA( _user_control_task )
PUBLIC _POWER_STABLE_EVENT_HANDLER
_POWER_STABLE_EVENT_HANDLER HDATA _spow_power_system_task
PUBLIC _POWER_PROTECTION_EVENT_HANDLER
_POWER_PROTECTION_EVENT_HANDLER HDATA _user_control_task

A wrong linker output file will be generated if the above module is the first
of modules on the linker invocation line. The actual cause is that labels
aren't updated once there resolved from other modules.

WORKAROUND:

Make it the last module on the linker invocation line

-------------------------------------------------------------------------------
KNOWN PR8423: Illegal warning W132

PROBLEM DESCRIPTION:

An illegal warning is generated when IF statement is used in include file

EXAMPLE:

file: template.asm

if !@DEF( 'template_INC' )
include 'template.inc'
endif
END

file: template.inc

if !@DEF( 'template_INC' )
define template_INC ''
endif

WORKAROUND:

Ignore the warning

-------------------------------------------------------------------------------
KNOWN PR8619: unjustified E265 of assembler

PROBLEM DESCRIPTION:

The following assembly example unjustfully emits E265...

NAME GLAS
HCSEG AT 0100H
aap:
cmp.w aap,#010H
beq aap
mies:
IF ( 0100H - aap != mies )
MSG aap
ENDIF
END

EXAMPLE:

Inspecting the listfile shows...

ADDR   CODE            LINE SOURCELINE
                          1         NAME GLAS
000100                    2         HCSEG AT 0100H
000100                    2         HCSEG AT 0100H
000100                    3         aap:
000100 9E14000010         4         cmp.w aap,#010H
000105 F3FD               5         beq aap
000105 F3FD00             5         beq aap
000108                    6         mies:
                          7         IF ( 0100H - aap != mies )

Note that all labels are know to the assembler so it should be able to
calculate the expression.

WORKAROUND:

It appears that that omitting BEQ gives a succesfull assemble. For debugging
purposes it can temporarily be replaced with two NOPs in order to get proper
messages from MSG.

-------------------------------------------------------------------------------
KNOWN PR8727: duplicate lines in the assembler list file

PROBLEM DESCRIPTION:


The assembler list file contains duplicate lines

-------------------------------------------------------------------------------
KNOWN PR8843: asxa: INCLUDE versus NOLIST

PROBLEM DESCRIPTION:

Doing an include of two levels deep and using the nolist
assembler control in each of these disrupts proper
listing and basically shows everything.

EXAMPLE:

; a.src
NAME TEST
$NOLIST
INCLUDE "b.inc"
$LIST
END

; b.inc
$NOLIST
INCLUDE "c.inc"
; not this
$LIST
$LIST
; but this

; c.inc
$NOLIST
$LIST

=> 'not this' and 'but this' will both be shown in the list file

-------------------------------------------------------------------------------
KNOWN PR8879: asxa: E252 illegally being issued

PROBLEM DESCRIPTION:

Assembling the following example illegally issues E252.

        cseg
label_AA:
       bne 4n
       jmp label_aa
4:
       bne 6n
6:

label_a equ 1
label_b equ 2

       ret
       end

EXAMPLE:

see problem description

-------------------------------------------------------------------------------
KNOWN PR8911: now reload of <project.i> results in default memory map

PROBLEM DESCRIPTION:


It seems that <project>.i isn't used for a memory reload of a project from
CrossView Pro. In that case, probably, the default of xa.dsc is chosen.
It can simple be proven by locating something at 64k for the medium memory
model, starting CrossView Pro and next do a reload. It is succeeded by the
following error messages:

PutBlock: cannot write GDI ...... present at CODE:0x10000
Download failure: no memory present at CODE:0x10000
no memory prsent at CODE:0xFFFE

Same errormessages appear when you envoke it from the command line like:

xfwxa a.abs -tcfg sim.cfg -dsc xa.dsc

By neglecting -em EDE=\"a.i\" the defaults are chosen which result in the
above error messages which seems to proof the theory.

WORKAROUND:


 close CrossView Pro and re-envoke from EDE

-------------------------------------------------------------------------------
KNOWN PR8919: assembler generates debug info for non-code sections

PROBLEM DESCRIPTION:

The assembler generates line number information for all sections,
including for non-code sections. For all sections a local function
is generated. Ieee-695 can not show difference between the local funcions.
In an harvard architecture this can give problems for setting breakpoints.
Assembler options to show assembler line info: -gal

EXAMPLE:

        cseg at 100h

start:  nop
        ret

        iseg at 100h
abc:    ds 2
        end

WORKAROUND:

Do not use the assembler debug information.
Do not overlap code and data.

-------------------------------------------------------------------------------
KNOWN CR8953: treat IDATA segments as indirect addressable only

PROBLEM DESCRIPTION:

Customer would like the following....

NAME B
X IDATA 25
Y SEGMENT CODE
RSEG Y
MOV X,R0
END

to trigger an error from the assembler i.e. the
assembler should treat the label as indirect addressable
only.

-------------------------------------------------------------------------------
KNOWN PR8965: asxa: illegal generic branch for bitaddressable operand

PROBLEM DESCRIPTION:

The following assembly example...

    CSEG
    JB _x, $
    DBSEG
_x: DBIT 1
    END


Is replaced by...


000000 97Arrr02          +2 ;       JNB     _x, _LG_0
000004 D5FFFF            +2 ;       JMP.L   $
000008                   +2 _LG_0:

Causing an infinite loop at JMP.L $

WORKAROUND:

replace JB _x, $
with 1: JB _x, 1p

