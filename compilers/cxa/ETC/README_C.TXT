-------------------------------------------------------------------------------
XA C Compiler Package

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
SOLVED PR7132: DOCUMENTATION ERRORS

PROBLEM DESCRIPTION:

1. Page 4-2 of XA Assembler User's Guide, section 4.2
"cc56" should be "ccxa"
2. Page 4-38 of XA C User's Guide, second line from the bottom
"0 to 4" should be "0 to 3"
3. Page 1-3 of XA C User's Guide, section 1.2.1
"C>set CXAINC=c:\cxa\include" should be "C>set CXAINC=c:\cxa\include\xa"
4. Page 4-5 of XA C User's Guide, section 4.1.1 regarding the use of
-Wa"arg" to -Wm"arg" in ccxa. Please provide an example on how to
use this option. My first impression was to use "-Wcg" to invoke
the "-g" in the C compiler. It turned out that it should be
"-Wc-g". Something similar to the following sentence will reduce
the number of experiments by customers.

"
The following example forces the C compiler to use -g option
(High Level Debugging information) to compile the program.
C>ccxa -Wc-g file.c
"
5. Contrary to the user's guide, the option -O (optimization switch) is
illegal for asxa.

EXAMPLE:

6. Contrary to the user's guide, the option -O3 is illegal for cxa.
However options -O0 to -O2 are fine.

-------------------------------------------------------------------------------
SOLVED PR7581: MEDIUM MODEL: DATA ACCESS IN SEGMENT !0 DEFUNCT

PROBLEM DESCRIPTION:

Medium model, suppose stack in segment 1.
MOV Rx,label  ==> MOV Rx,SOF(label)

In V1.1 the label is treated as a 24 bit address. The medium model requires
a 16 bit address.

-------------------------------------------------------------------------------
SOLVED PR7703: Subscript Strength reduction defunc

PROBLEM DESCRIPTION:

The subscript strength reduction does not work in next example

EXAMPLE:

unsigned int data_table [16][8] =  {
{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},
{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},
{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},
{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8},{1,2,3,4,5,6,7,8}, {1,2,3,4,5,6,7,8} };

unsigned int sensor_table [16][8];

void main( void )
{   int i, j;
    for ( i=0, j=0; j < 8; i++ )
    {
       sensor_table [i][j] = data_table [i][j];
       if ( i == 7 )
       {
         i = 0;
         j++;
       }
    }
}

WORKAROUND:

Turn off subscript strength reduction with compiler option -OV

-------------------------------------------------------------------------------
SOLVED PR7770: inline function _memmovc defun

PROBLEM DESCRIPTION:

the following example:
#define SIZE 512
typedef unsigned char UCHAR_t ;
_rom UCHAR_t romOne[SIZE] ;
_rom UCHAR_t romTwo[SIZE] ;
_far UCHAR_t copyOne[SIZE] ;
_far UCHAR_t copyTwo[SIZE] ;
_far void * dst ;
void Copy(void) {
  dst = (_far void *) copyOne ;
  _memmovc(dst, (_rom void *) romOne, SIZE) ;
  dst = (_far void *) copyTwo ;
  _memmovc(dst, (_rom void *) romTwo, SIZE) ;
}
compiled with -Mm -Cxag3 -s -O1 -g -A1

(continued on next page)

EXAMPLE:

(continued from previous page) results in a an error. The
SIZE constant is optimised and not reloaded for the
second _memmovc() 'call'. This can easilly be solved with
#pragma optimize C but now something else. This customer
also made an user error which was that for the pointer
assignment he used the following statement:
dst = (_far void *) &copyOne ;
Although this is taking the address of the address of an
array the compiler doesn't issue an error message. It
does seem to generate correct code surprisingly which is
propably why the customer didn't notice that something's
wrong. The compiler can be triggered to pick up on this
error when the expression is re-written like this:
dst = (_far void *) &(copyOne+0)   ; // or like this:
dst = (_far void *) &(&copyOne[0]) ;

WORKAROUND:


- as explained on the previous page -

-------------------------------------------------------------------------------
SOLVED PR7796: s560: internal error - please report

PROBLEM DESCRIPTION:

The following example causes an error in the DEMO compiler version of the
XA v1.0r0 and v1.1r0 :

typedef unsigned char BYTE_t ;
extern BYTE_t flag;
void Test(void) {
  static char anotherFlag = 1;
  if (flag!anotherFlag) {
  }
}

Using the following command line options : -Mt -O0

WORKAROUND:

--- first one ---

Use temporary variable in if-statement:
BYTE_t tmp ;
if (tmp = !anotherFlag, flag&tmp) {
}

--- second one ---

Swap if expressions:
if (!anotherFlag&flag) {
}

-------------------------------------------------------------------------------
SOLVED PR7810: Improper definitions of DestHSSEL & SrcHSSEL

PROBLEM DESCRIPTION:

The INIT.SRC file is not correct. "The problem is with
the definitions of DestHSSEL & SrcHSSEL which should be
defined as follows:

DestHSSEL EQU SSEL.0
SrcHSSEL EQU SSEL.2

This is because memory accesses using DestL (R0) requires
SSEL.0 and SrcL (R2) requires SSEL.2."

EXAMPLE:

Initialising HDATA in INIT.SRC using MODEL_MEDIUM :
SETB SrchHSSEL
SETB DestHSSEL ; result for SSEL = 0x4a, R6 initialised prior to this
_cpt_inithdata_next:
MOV.B CS, SrcHL
MOV.B ES, DestHL ; setting up CS & ES... good
MOVC.B InitLL,[SrcL+]
; => MOV.B R6L,[R2+] => src. addr. PC16-23 & [R2] => should have been CS & R2
MOV.B [DestL],InitLL
; => MOV.B [R0],R6L => dst. addr. DS & [R0] => should have been CS & [R0]

WORKAROUND:

Make the alteration as suggested.

-------------------------------------------------------------------------------
SOLVED PR7813: SSEL not initialised for jump switch

PROBLEM DESCRIPTION:

"... using the tools with the MEDIUM memory model... When a switch statement
is compiled into a jump table, the SSEL register is not set up..."

EXAMPLE:

; test.c 12 switch (aVal)
SYMB LINE, 12
MOV.W   R1,[R7]
CMP.W   R1,#01H
BG      _11
ASL.W   R1,#02H              ; SETB SSEL.1 prior to R1 usage
MOV.B   CS,#SEG(__swtable13)
ADD.W   R1,#SOF(__swtable13)
MOVC.W  R0,[R1+]
PUSH.W  R0

WORKAROUND:

Compile with -OTW to force jump chain code.

-------------------------------------------------------------------------------
SOLVED PR7910: Register allocation defunc

PROBLEM DESCRIPTION:

Internal register allocation defunc for next case

EXAMPLE:

struct ST { int dummy[5]; int y; };
struct ST array[ 2];
int i, x;

void f( void )
{
    struct ST *p;

    while( 1 )
    {
      p = &array + i;
      while( p->y ) D( p->y);
      p->y = x;
    }
}

WORKAROUND:

Turn off invariant code motion -OI

-------------------------------------------------------------------------------
SOLVED PR7911: Multiply compound operation defunc

PROBLEM DESCRIPTION:

The compiler crashes on a multiple compound operation containing post
increment or decrement.

EXAMPLE:

char *a;

void E(void)
{
    *a++ |= 1;

}

WORKAROUND:


rewrite source code to avoid this type of multiple compound operations.
e.g. (*a |= 1, a++ );

-------------------------------------------------------------------------------
SOLVED PR7988: wrong code for automatic bit variables

PROBLEM DESCRIPTION:

The following example compiled with default optimisation generates wrong
assembly code:

volatile x ;
#define EXPRESSION x
#define ASSIGNMENT(a,b) (x=(a==b))
#define CONSTANT 0

#ifdef WORKAROUND
#define STATIC static
#else
#define STATIC
#endif

extern void OpinionPoll(char) ;

void TestProcedure(void)
{
  STATIC _bit a ;
  STATIC _bit b ;

EXAMPLE:


  a = 0 ;
  b = 0 ;

  if (EXPRESSION) {
    a = 1 ;
    OpinionPoll(CONSTANT) ;
  }
  if (EXPRESSION) {
    b = 1 ;
    OpinionPoll(CONSTANT) ;
  }

  ASSIGNMENT(a,b) ;

}

WORKAROUND:

Use static bit variables instead.

-------------------------------------------------------------------------------
SOLVED CR8009: immediate values in hex while disassembling

PROBLEM DESCRIPTION:

Is there a way of telling CrossView to format its immediate values into
hexadecimal format while disassembling ?

-------------------------------------------------------------------------------
SOLVED PR8028: cxa: E175: Not an arithmetic constant

PROBLEM DESCRIPTION:

The following example will not compile. The compiler
issues error message 175 : not an arithmetic constant.

typedef struct test {
  int i ;
  struct test ** p
} TEST_t ;

void aTest(void) {
  TEST_t aStruct = {1,0} ;
}

EXAMPLE:

Work Around
-----------

#if _MODEL == 't' || _MODEL == 's' || _MODEL == 'm'
typedef _far char VOID_t
#else
typedef _huge char VOID_t
#endif

#include <stdlib.h>

typedef VOID * VOIDPOINTER_t ;
#define P2VP(x) (x.p)
#define I(x)    (((TEST_t *)*(x.p))->i)
#define P(x)    (*(x.p))

typedef struct
{
  int i ;
/*

WORKAROUND:

*/
  VOIDPOINTER_t * p ;
} TEST_t ;

void main(void)
{
  TEST_t aStruct = {0} ;
  P2VP(aStruct) = (VOIDPOINTER_t *) malloc(sizeof(VOIDPOINTER_t)) ;
  P(aStruct) = (VOIDPOINTER_t) &aStruct ;
  I(aStruct) = 1 ;
}

explanation :

- declare a struct
- allocate storage for pointer to pointer
- point to oneself
- assign 1 to aVar of pointee i.e. to aVar of aStruct

-------------------------------------------------------------------------------
SOLVED PR8074: DB initilisation failure for pointer to pointer

PROBLEM DESCRIPTION:

The following example will not generate sufficient
storage for the NULL assigned pointers:

#include <stdlib.h>

static char *vals[] = {"A", "B", "C", NULL} ;
char * kw[] = {"D"} ;
char **val[] = {vals} ;

typedef struct {char*s1;char*s2;char**sa3;char***sa4;} WHAT ;

static WHAT tbl[3] = {
  {"E", "", NULL, NULL},
  {"F", "", (char**)NULL,(char***)NULL},
  {"G", "", kw, val}
} ;

WORKAROUND:

Do not initialise the pointers with NULL. This will then
be done automatically according to the ANSI standard.

-------------------------------------------------------------------------------
SOLVED PR8089: Illegal HLL debug block info generation after errors

PROBLEM DESCRIPTION:

A register dump is generated after a specific number of syntax errors
when full debug is enable (-g) and a new block scope is started in a
function.

EXAMPLE:

wchar_t *func(int n);

int main()
{
  /* the illegal operation disappears when the brackets are removed */
  {
  int true = 1;
  }
  return;
}

WORKAROUND:

Solve syntax errors

-------------------------------------------------------------------------------
SOLVED PR8090: Stack structure member copy defunc

PROBLEM DESCRIPTION:

Determination of aggregate member offset allocated on the stack and copied
to the stack fails when aggregate member is a structure or an union and it
is not the first member.

EXAMPLE:

typedef struct point { int x,y; } point;
typedef struct rect { point pt1, pt2; } rect;
rect makerect( point p1, point p2 ) {
        rect r;
        r.pt1 = p1;
        r.pt2 = p2;
        return( r );
}
main() {
        point pts[] = { 10, 100, 1, 1, 20, 300, 500, 400 };
        rect screen = makerect( pts[0], pts[2]);
}

WORKAROUND:

Use structure pointers instead for passing and returning structures instead
of passing them by value.

-------------------------------------------------------------------------------
SOLVED PR8091: Array indexing and pointer optimization conflict

PROBLEM DESCRIPTION:

A signed array index expression can be optimized by evuating it unsigned
instead of signed, because an negative array index is not allowed. An array
access can also be optimized rewriting it to a pointer access to eliminate
the constant part of an array index epression. But it is allwoed to have
negative pointer offsets. So, it is never allowed to perform both
optimizations on a signed array index expression at the same time. The best
one need to be selected. Currently both are performed, which is wrong. This
results in an unsigned pointer arithmetic for an array with a signed index.

EXAMPLE:

int h[15];
int r;
void f( int c )
{
    if ( h[r-c+7] )
    {
        f( c );
    }
}

WORKAROUND:

rewrite array indexing to avoid both optimizations being performed by
eliminating the constant part or rearanging expression elements or make
array index an unsigned expression.

E.g. h[r+7-c]

-------------------------------------------------------------------------------
SOLVED PR8102: odd char auto array initialization defunc

PROBLEM DESCRIPTION:

Initialization of a signed/unsigned char automatic array with an odd number
of initializers fails. The last initializer is skipped.

EXAMPLE:

void main ()
{
      signed char char_array1[] = { 1, 2, 3 };
}

WORKAROUND:

Make the number of initializers even or make array declaration static.

-------------------------------------------------------------------------------
SOLVED PR8103: Dead code elimination defunc for ? :

PROBLEM DESCRIPTION:

Compilation of dead code on the right or left side of the colon operator
fails when the intermediate code peephole optimizer is enabled.

EXAMPLE:

unsigned int *p1, *p2;
int i,j,k;

int main()
{
    i = j ? k : ((*p1 ^ *p2), k);
}

WORKAROUND:

Remove dead code or disable the peephole optimizers

-------------------------------------------------------------------------------
SOLVED PR8152: stack continues in segment 1 and causes run-time errors

PROBLEM DESCRIPTION:

When there is consequtive data memory that starts in segment 0 and at least
continues in segment 1 then the locator doesn't keep into account that the
Supervisor Stack isn't allowed in segment 1 and locates it in both segments
(or more if there are). As DS is setup with the locator label __lc_es it is
initialised wrongfully and causes serious run-time errors.

WORKAROUND:

Declare a dummy integer at address 0x00fffe using the _at() operator. This
represents a physical boundry which may not be crossed by the stack.

-------------------------------------------------------------------------------
SOLVED PR8164: Temporary Bit Register allocation defunc

PROBLEM DESCRIPTION:

Temporary Bit register allocation problem evaluating
expression vbit2 = vbit1^lbit; See, example

EXAMPLE:

_bit vbit1, vbit2;
_bit func40( int a, int b )
{
    static   _bit lbit;

    lbit = !vbit1;
    vbit2 = vbit1^lbit;
    return a | b & vbit1 | vbit2 | lbit;
}

WORKAROUND:

Rewrite example to:
_bit vbit1, vbit2;
_bit func40( int a, int b )
{
    vbit2 = vbit1^(!vbit1);
    return a | b & vbit1 | vbit2 | (!vbit1);
}

-------------------------------------------------------------------------------
SOLVED PR8167: Illegal addressing mode generated for MOVX

PROBLEM DESCRIPTION:

An illegal operand combination is generated for a MOVX instruction

EXAMPLE:

extern _xdat long             long_global ;
void TestGlobalVars(void)
{
   long_global=2000000L;
}

WORKAROUND:

remove _xdat keyword

-------------------------------------------------------------------------------
SOLVED PR8188: Illegal error E 516 generated

PROBLEM DESCRIPTION:

The compiler reports that it is not allowed to use data
storage qualifier _far as a function qualifier. This is
correct, only for this case there was no _far data qualfier
used as function qualifier. So, no error should be generated.
This problem only occure when the same typdef is used for
a funtion parameter and function return and the typedef
contains a data storage qualifier in its logical type.

EXAMPLE:



static P f( _far P * l  )
{
    return *l;
}

WORKAROUND:

Expand typedef for function paramter or function return as
next example shows.

typedef _far struct p * P;

static _far struct p * f( _far P * l )
{
   return *l;
}

-------------------------------------------------------------------------------
SOLVED PR8209: unjustified stack addressing for addr. array in loop

PROBLEM DESCRIPTION:

The customer sent a case which he compiles in the large
model. Here's an extract...

_far unsigned char * _near address ; /* global scope */

for (i=0;i<2048;i++) {
  address[0] = (unsigned char) 0xff ;
  /* same initilisation for address[1] and address[2] */
}

for (i=0;i<2048;i++) {
  if (address[1] != (unsigned char) 0xff) x++ ; /*okay*/
  if (address[2] != (unsigned char) 0xff) x++ ; /*!okay*/
  if (address[3] != (unsigned char) 0xff) x++ ; /*!okay*/
}

The latter two use CMP.B [R7],#-1 for compare which is
wrong since R7 is not intialised properly.

WORKAROUND:

Use -C to disable Common Subexpression Elimination

-------------------------------------------------------------------------------
SOLVED PR8251: Intermediate code pattern match defunc

PROBLEM DESCRIPTION:

The intermediate code rewrite rule failed for a condition expression which
operands exist of structure list pointers. One of these pointers is updated
before the compare with its next member. See below, p=p->next

EXAMPLE:

typedef struct st
{
    struct st * next;
} tst;

tst * p;

void f( tst * parm )
{
    tst * cur;

    cur  = p;
    p = p->next;

    if ( p == parm )
    {
       cur->next = p->next;
    }
}

WORKAROUND:

Disable Intermediate code patern matcher (-OH), or disable expession
propagation (-OE), or disbale common subexpession elimination (-OC). Non of
these optimizations are the real problem but they can be use to bypass the
real problem. Also rewriting the code is one of the possibilities

-------------------------------------------------------------------------------
SOLVED PR8266: Some CSE problem

PROBLEM DESCRIPTION:

Compilation fails for next example

EXAMPLE:

typedef struct
{
        unsigned char * t;
        int * p;
} T;
extern        T b[];
int i;

void f( unsigned long sz, unsigned long n )
{
    register T        *  p;
    unsigned char  *     s;
    unsigned long        l;

    l = n / 4;
    if ( sz < l ) i = 2;
    s = (p = &b[i])->t;
    p->p = (int *)(s = (s - (unsigned long)(s) % 2));
    *(&s) -= 1;
}

WORKAROUND:

Disable common subexpression elimination with command line option -OC

-------------------------------------------------------------------------------
SOLVED PR8283: Internal error S 516 of the cxa compiler

PROBLEM DESCRIPTION:

The following example compiled in the medium memory model
causes the compiler to crash:

extern char _lc_bs[] ;
#define CASE(x) (*(long*)((unsigned)_lc_bs+x))
void Test(void)
{
  if (CASE(0))
    CASE(0)-- ;
}

WORKAROUND:

Remove the case (unsigned).

-------------------------------------------------------------------------------
SOLVED PR8315: The altering of SSEL by __CPNHW isn't picked up by ISRs

PROBLEM DESCRIPTION:

The run-time library functions __CP{NH/HN}W change
the value of SSEL. As ISRs assume SSEL equals 7F this
leeds to run-time errors in those cases where
__CP{NH/HN}W is interrupted.

WORKAROUND:

_interrupt(vector) _using(psw) void
isr(void)
{
#pragma asm
push.b SSEL
mov.b SSEL,07FH
#pragma endasm

/* code */

#pragma asm
pop.b SSEL
#pragma endasm
}

-------------------------------------------------------------------------------
SOLVED PR8365: internal compiler error S307

PROBLEM DESCRIPTION:

The following sample causes an internal compiler error 307. Conditions
are large memory model and CSE disabled:

EXAMPLE:

void f(_near char * far * p)
{
 *p = 0 ;
}

WORKAROUND:

Enable CSE using -Oc

-------------------------------------------------------------------------------
SOLVED PR8429: Optimization conflict for _far pointers large model

PROBLEM DESCRIPTION:

The 16-bit pointer arithmetic for 24-bit pointers in the large memory model
is conflicting with an intermediate code pattern optimization. This
optimization swaps index and address for a more optimum indirect addressing
This is not possible for pointers with size 24-bit and arithmetic 16-bit.

EXAMPLE:

compile with: -Ml
typedef struct
{
    unsigned char * p;
} ST;

ST array[1];

unsigned short y;

void r( int s )
{
        unsigned long i;

        for ( i=0;i<y; i++ )
               array[s].p[i] = 0;
}

WORKAROUND:

Disable the intermediate code pattern matcher for this case with the
optimization option -OH.

-------------------------------------------------------------------------------
SOLVED PR8438: wrong code for _UDIVL and _UMODL

PROBLEM DESCRIPTION:

Both UDIVL and UMOVL have a problem in their final stage. They
use an instruction that does a comparison and a BLE afterwards.
As an example:

CMP.W  _ty, _tx
BLE  RESHW0

The BLE instruction is a signed compare instruction. Instead
the instruction BL should be used which is an unsigned compare
instruction. The problem is that in the lower 16 bits of the
comparison, if the result has the MSB set to 1 (ie 0x9984) and
the original dividend has the MSB set to 0 (ie 0x0452), the BLE
instruction will assume that 0x9984 < 0x0452 and will
erroneously subtract one from the quotient.

EXAMPLE:

#include <stdlib.h>

unsigned long x,y;
unsigned long a = 0x80000000;
unsigned long b = 0x8000ffff;

void main( void )
{
    x = a / b;  /* x should be 0, but will be 1 */
    y = a % b;  /* y should be equal to a       */

    if ( (x!=0) || (y!=a) )
    {
        exit (1);
    }
    exit (0);
}

WORKAROUND:


Do the replacement as suggested.

-------------------------------------------------------------------------------
SOLVED PR8468: compiler crash

PROBLEM DESCRIPTION:

The following is an extract from the customer :

/* options used : -Ml -O0 */

typedef struct QueueElement1 * QueueBase1;
void que_dequeue1( QueueBase1, unsigned char );
struct QueueElement1 {
  unsigned char next;
  unsigned char prev;
};
void que_enqueue1( QueueBase1 b, unsigned char i )
{
  if( b[i].next == 0 )
  {
    b[i].next = i;
    b[i].prev = i;
  }
}

WORKAROUND:

Turn on common sub-expression using -Oc.

-------------------------------------------------------------------------------
SOLVED PR8490: Illegal optimisation

PROBLEM DESCRIPTION:

The example listed underneath ignores the assignment of res_ptr->fwlink.
Compiled in the large memory model using default optmisation.

struct _tcb { char a ; } ;
typedef struct _tcb *tcbpointer;
typedef struct cmxresource {
  tcbpointer fwlink;
  tcbpointer bwlink;
  tcbpointer owner;
} RESHDR;
extern RESHDR res_que[];
unsigned char cxrsrsv1(unsigned char res_grp) {
  RESHDR *res_ptr = &res_que[res_grp];
  res_ptr->bwlink = (tcbpointer)(res_ptr);
  res_ptr->fwlink = (tcbpointer)(res_ptr); // ignored !!!
  return(0x00);
}

EXAMPLE:



; scratch.c   11          res_ptr->bwlink = (tcbpointer)(res_ptr);
        MOV.W   R1,R3
        MOV.W   R0,R2
        MOV.B   ES,R3L
        MOV.W   [R2+4],R0
        MOV.W   [R2+6],R1
; scratch.c   12          res_ptr->fwlink = (tcbpointer)(res_ptr);
; scratch.c   13          return(0x00);
        MOV.B   R0L,#00H
; scratch.c   14        }
        RET

WORKAROUND:

Switch off the CSE optimisation using -OC.

-------------------------------------------------------------------------------
SOLVED PR8493: malloc restrictions

PROBLEM DESCRIPTION:

Malloc doesn't work as it should when allocating blocks larger than 32k and
crossing segment boundaries.

Furthermore malloc returns NULL as soon as the heap overlaps segment
boundaries.

WORKAROUND:


The heap problem can be worked around by changing the type of 'sbrk' (module
sbrk.c) from 'int' to 'unsigned'. This also requires you to adapt the proto
type of modules it is used in but most important malloc.c.

-------------------------------------------------------------------------------
SOLVED PR8512: Illegal 16-bit pointer arithmetic optimisation

PROBLEM DESCRIPTION:

The following example leeds to illegal code generation:

typedef struct {
  unsigned int  i;
  unsigned char c;
} st1;

typedef struct {
  st1 * p;
} st2;

unsigned char x;

void g( _near st2 * q, unsigned char i ) {
  st1 * p = q->p + i;
  p->c = x;
}

EXAMPLE:

--- from support ---

concerns PRs 8429 and 8627

WORKAROUND:

Disable peephole optimiser using -OH

-------------------------------------------------------------------------------
SOLVED PR8586: Subscript strength reduction causes compiler crash

PROBLEM DESCRIPTION:

The example program causes the compiler to crash when both
'subscript strength reduction' and 'common subexpression
elimination' optimisations are enabled.

EXAMPLE:

typedef struct
{
    unsigned int length;
} st;

void test( st *bus_ptr, unsigned char *data_ptr )
{
    unsigned char offset;

    for ( offset = 26; offset < 32; offset++ )
    {
        *(data_ptr+offset) = 0x00;
    }
    *(data_ptr+32) = (unsigned char)((34+bus_ptr->length)/256);
    *(data_ptr+33) = (unsigned char)(34+bus_ptr->length);
}

WORKAROUND:

Disable one of the optimisations (-OC or -OV).

-------------------------------------------------------------------------------
SOLVED PR8587: Casting 32-bit constant address to 16-bit failure

PROBLEM DESCRIPTION:

For the following example the variable 'x' is illegaly assumed to be
allocated on the stack. This problem only occures in the large memory model
casting a constant 32-bit address to 16-bit value and back again to 32-bit.

EXAMPLE:

unsigned int x;
unsigned char * p;
unsigned int i;

void main( void )
{
    p = (unsigned char*) ( (long)(unsigned int)(&x + 1) + i );
}

WORKAROUND:

Remove the cast (unsigned int).

unsigned int x;
unsigned char * p;
unsigned int i;

void main( void )
{
    p = (unsigned char*) ( (long)(&x + 1) + i );
}

-------------------------------------------------------------------------------
SOLVED PR8617: Filenames should be unique on the first 8 characters.

PROBLEM DESCRIPTION:

When filenames are not unique on the first 8 characters, Crossview
has problems with opening the correct files.

-------------------------------------------------------------------------------
SOLVED PR8621: compiler crash for compound if statement

PROBLEM DESCRIPTION:

The following example causes winNT/95 error...

void CC_drv_get_CCData( unsigned char *pBuffer )
{ unsigned char cShiftRegister;

  cShiftRegister = 1;
#ifdef BUG
  *pBuffer ^= ((*pBuffer & cShiftRegister)==0) ? 0 : 0x8;
#else
  *pBuffer ^= ((*pBuffer & 1)==0) ? 0 : 0x80;
#endif
}

Options used -Ml -O0 -DBUG

WORKAROUND:

Omit -ODBUG or use -O{1,2) instead

-------------------------------------------------------------------------------
SOLVED PR8627: segment overwrite due to peephole optimizer

PROBLEM DESCRIPTION:

Compiling the example using large memory destroys the data helt by R3 and
therefore lateron ES is initialised with a corrupt value.

struct A { unsigned   a ; struct A * b ; } ;
struct B { struct A * c ;                } ;
struct C { struct C * d ; struct D * e ; } ;
struct D { struct B * f ;                } ;
typedef struct C * C_t ;
typedef struct A * A_t ;
typedef struct B * B_t ;
extern int i ;
void out(C_t a, A_t b) {
  C_t c; B_t d;
  c=a->d;
  b->b = NULL;
  d = c->e->f + i ;
  d->c = NULL;
}

EXAMPLE:

; b.c        13   b->b = NULL;
        MOV.B   ES,R3L
        MOV.W   [R2+2],#00H
        MOV.W   [R2+4],#00H
; b.c        14   d = c->e->f + i ;
        MOV.B   ES,#SEG( _i )
        MOV.W   R2,#SOF( _i )
        MOV.W   R2,[R2]
        ASL.W   R2,#02H
        MOVS.W  R3,#00H     ; <- destroying R3
        MOV.B   ES,R1L
        MOV.W   R1,[R0+6]
        MOV.W   R0,[R0+4]
        MOV.B   ES,R1L
        ADD.W   R2,[R0]
; b.c        15   d->c = NULL;
        MOV.B   ES,R3L    ; using it but not legal
        MOV.W   [R2],#00H
        MOV.W   [R2+2],#00H

WORKAROUND:


disable peephole optimizer

-------------------------------------------------------------------------------
SOLVED PR8640: DS not saved for str(cmp/cpy)

PROBLEM DESCRIPTION:

Both strncmp.src and strcpy.src do not save and restore DS. This causes
run-time errors when direct addressing.

EXAMPLE:

_strcpy:
IF    MODEL_LARGE
        MOV.B   ES,R3L          ; s2 addressed via ES
        CLR     SSEL.0          ; s1 addressed via DS
        MOV.B   DS,R1L          ;
ENDIF
        MOV.W   R3,S1           ; os1 = s1
loop:
        MOV.B   [S1+],[S2+]     ;
        BNE     loop            ; while (*s1++ = *s2++)
        MOV.W   S1,R3           ; return os1;
IF    MODEL_LARGE
        SETB    SSEL.0          ; set back for default addressing
ENDIF
        RET

WORKAROUND:

_strcpy:
IF    MODEL_LARGE
        PUSH.B  DS
        MOV.B   ES,R3L          ; s2 addressed via ES
        CLR     SSEL.0          ; s1 addressed via DS
        MOV.B   DS,R1L          ;
ENDIF
        MOV.W   R3,S1           ; os1 = s1
loop:
        MOV.B   [S1+],[S2+]     ;
        BNE     loop            ; while (*s1++ = *s2++)
        MOV.W   S1,R3           ; return os1;
IF    MODEL_LARGE
        SETB    SSEL.0          ; set back for default addressing
        POP.B   DS
ENDIF
        RET

-------------------------------------------------------------------------------
SOLVED PR8675: compiler crash win31

PROBLEM DESCRIPTION:

The following is a simplefied case from the customer's. Compiling it with
large memory model causes it to crash on win95:

char * f(void)
{
    char * p = (char *) 0L ;

    char i = 32;

    do
    {
      *p++ &=0x7f;
    } while(i--);

    return ( p );
}

WORKAROUND:

expand the compound statement

-------------------------------------------------------------------------------
SOLVED PR8681: illegal run-time routines ___CP{HN,NH}W for large model

PROBLEM DESCRIPTION:


The compiler emits run-time routines __CPHNW and __CPNHW in the large
memory model for structure copying from stack to huge data. Instead
these should have been __CPFHW and __CPHFW which do not work with DS.
As the other two library routines do, they rely on DS having the same
value as the system mode stack pointer.

WORKAROUND:


 verify wether the __DS label has the same segment address as the system
mode stack pointer. In that case the application will run fine regardless
the use of __CP{HN,NH}W.

-------------------------------------------------------------------------------
SOLVED PR8692: S 320: (line n) internal error - please report

PROBLEM DESCRIPTION:

The following example, compiled in the large memory model, emits eternal
compiler error S320.


extern int a ;
void f(void){
  int i;
    for (i=0;i<a+1;i++);
    for (i=0;i<100;i++) a++ ;
}

WORKAROUND:

1) use -OC
2) rewrite expression 'i<a+1' to 'i<=a'

-------------------------------------------------------------------------------
SOLVED PR8693: logical negate defunc

PROBLEM DESCRIPTION:

An incorrect conditional branch is generated for a logical expression with
multiple negate operations.

EXAMPLE:

#include <stdlib.h>

unsigned char a = 0;
int s = 0;

unsigned char f( void )
{
    return !a || !( s == 2 );
}
void main( void )
{
    if ( f() == 0 )
         exit( 1 );

    exit( 0 );
}

WORKAROUND:

Rewrite the logical expression to:
#include <stdlib.h>

unsigned char a = 0;
int s = 0;

unsigned char f( void )
{
    return !(a && ( s == 2 ));
}

void main( void )
{
    if ( f() == 0 )
          exit( 1 );

    exit( 0 );
}

-------------------------------------------------------------------------------
SOLVED PR8697: -c option from assembler causes it to crash

PROBLEM DESCRIPTION:

Using the -c option of the assembler causes it to crash

WORKAROUND:

Use the NOCASE control instead

-------------------------------------------------------------------------------
SOLVED PR8731: _rom pointer arithmetic defunc

PROBLEM DESCRIPTION:


_rom pointer arithmentic should be true 24 bit arithmetic, as with _huge
data pointers for the large/medium memory model, instead of 16 bit. _rom
objects can larger than 64k and therefore _rom pointers must be capable
of handling this.

EXAMPLE:


The following example will loop infinitly (large/medium memory model) as no
ADDC is done for the pointer increment:

int f(void)
{
  _rom char * p ;
  int i ;
  for (p=(_rom char *)0x040000;p<=(_rom char *)0x05FFFF;p++)
  {
    i += *p ;
  }
  return i ;
}

WORKAROUND:


Use the following substitute...

int f(void)
{
  _huge char * p ;
  int i ;
  for (p=(_huge char *)0x040000;p<=(_huge char *)0x05FFFF;p++)
  {
    i += * ((_rom char *)p) ;
  }
  return i ;
}

-------------------------------------------------------------------------------
SOLVED PR8761: Buffer counter not maintained correctly in _filbuf

PROBLEM DESCRIPTION:

The file buffer count is not reset to zero on EOF detection.

WORKAROUND:

Reset file buffer count in _flsbuf to zero when EOF is returned.

-------------------------------------------------------------------------------
SOLVED PR8849: Illegal intermediate code pattern match

PROBLEM DESCRIPTION:

The following example shows illegal intermediate code pattern match compiling
into the large memory model:
void f(int *arg1,int arg2,char *arg3,char *arg4) {
  char aut1;
  int aut2;
  int aut3;
  aut2 = arg1[*arg3];
  for( aut1 = 0 ; aut1 < *arg4  ; aut1++ ) {
    aut3 = arg1[ aut1 ];
    arg1[ aut1 ] = aut2;
    if( aut3 == arg2 ) {
      break;
    }
    aut2 = aut3;
  }
}

EXAMPLE:

The generated assembly...
; b.c        13     aut3 = arg1[ aut1 ];
        MOV.W   R1,[R7+2]
        MOV.W   R0,[R7]
        MOV.B   ES,R1L
        MOV.W   R5,[R0]
; b.c        14     arg1[ aut1 ] = aut2;
        MOV.W   R1,[R7+2]
        MOV.W   R0,[R7]
        MOV.B   ES,R1L
        MOV.W   [R0],R4
; b.c        15     if( aut3 == arg2 ) {
        MOV.B   ES,R1L
        CMP.W   [R0],R2
        BEQ     _5
Note the final comparison; it compares [R0] which is arg1[aut1] and not aut3
as written in the C expression.

WORKAROUND:

Use -OH to disable the pattern matcher

-------------------------------------------------------------------------------
SOLVED PR8864: _huge bitfield access defunc medium model

PROBLEM DESCRIPTION:

The SSEL register is not setup correctly when setting or clearing a bitfield
which is allocated in _huge storage space for the medium memory model only.

EXAMPLE:

typedef struct
{
    int b : 1;
} S;

_huge S s;

main()
{
    s.b = 1;
}

WORKAROUND:

Use an intermediate global structure pointer


typedef struct
{
    int b : 1;
} S;

_huge S s;

_huge S *p = &s;

main()
{
    p->b = 1;
}

-------------------------------------------------------------------------------
SOLVED PR8880: _inline causing internal error S 93

PROBLEM DESCRIPTION:

The _inline keyword is causing an internal error

EXAMPLE:


extern unsigned int* pointer;

_inline
unsigned int* get_pointer(void)
{
  return(pointer);
}


void main(void)
{
  unsigned int* pMem;

  pMem = get_pointer();
}

WORKAROUND:

remove the _inline keyword

-------------------------------------------------------------------------------
SOLVED PR8889: Common subexpression elimination problem

PROBLEM DESCRIPTION:

The CSE optimization causes an illegal stack offset generation

EXAMPLE:

sample may be send at customer's request

WORKAROUND:

Disable the pattern matcher with -OH.

-------------------------------------------------------------------------------
SOLVED PR8933: SSEL selection incorrect for cprnw large memory model

PROBLEM DESCRIPTION:

The SSEL is not setup correctly for cprnw large memory model

WORKAROUND:

update source of delivered module cprnw.src
Add CLR SSEL.4 instruction at function entry for large model
Add SETB SSEL.4 instruction at befor ret for large model

-------------------------------------------------------------------------------
SOLVED PR8944: compiler wrongfully processes #if 0/#endif code.

PROBLEM DESCRIPTION:

The compiler seems to process a piece of code which is in a part
that should not be processed because it is in a false preprocessor
part.

EXAMPLE:

void main( void )
{
#if 0
#pragma asm
    orc #h'c0,ccr
#pragma endasm
#endif
}

When compiling this example, the compiler seems to process
the ' on line 5 as a character quote and starts looking
for the closing quote which it can't find.

-------------------------------------------------------------------------------
SOLVED PR8960: illegal type information generation

PROBLEM DESCRIPTION:

The following case illegally results into w112 of the linker...

//commom.h
typedef void (*WSS_Client) (int);
typedef struct { int u; } PageAddress;
typedef void ( *PageClient ) (PageAddress) ;

//aa.c
#include "common.h"
typedef struct { PageClient pc; } PageRequest;
extern void WSS_RegisterClient(WSS_Client client);
void atxt_pi_wss_turnoff( void ) {
  WSS_RegisterClient(( WSS_Client ) 0);
}

//bb.c
#include "common.h"
void WSS_RegisterClient(WSS_Client client) {}
void main(void) {}

WORKAROUND:

swicth off debug information for either module aa.c or bb.c

-------------------------------------------------------------------------------
SOLVED PR8966: cxa : s320 followup of warning 147

PROBLEM DESCRIPTION:

The compiler exits with internal error 320 for the
following sample...

void f(void *);

typedef union x_t
{
   int xa[N];         // S320 for N > 2
} X_t ;

X_t x;

void g(void)
{
   f(x);
}

WORKAROUND:

Use the '&' while passing 'x'

-------------------------------------------------------------------------------
SOLVED PR8977: cxa : storage allocation failure

PROBLEM DESCRIPTION:

Illegal overlap in storage allocation. In next example variable x is illegaly
assumed to be allocated in R0L where variable y is already being allocated.
This results in a illegal write back to variable x.

EXAMPLE:

typedef struct { int x1, x2, x3; } ST;
ST st1, st2, st3[1];
unsigned char y;
void f( void )
{
    unsigned char x;

    for( x = 0; x < 25; x++ )
         if ( y )    {
             switch( x )
             {
             default:
                if ( x % 4 )
                    st2 = st1;
             case 0:
               break;
             }
            st3[y] = st2;
        }
}

WORKAROUND:

Disable common sub-expression elimination with -OC. Common subexpression
is not the problem in this case, but it helps by-passing the problem.

-------------------------------------------------------------------------------
SOLVED PR8981: cxa : loop invariant code motion failure

PROBLEM DESCRIPTION:

The expression 'c+1' in the example below is illegaly assumed to be invariant
of the loop.

EXAMPLE:

void q( unsigned int );
typedef unsigned char AR[3];
AR *p;

void f( void )
{
    unsigned int i;
    AR *c = p;

    while( **c )
    {
        i = *(*c+1);
        q(i);
        c++;
    }
}

WORKAROUND:

Disable loop invariant motion with -OI

-------------------------------------------------------------------------------
SOLVED PR9012: Comparing _far and _huge pointers failure

PROBLEM DESCRIPTION:

Code generation for comparing _far and _huge pointers fail for medium memory
model.

EXAMPLE:

typedef struct ST { unsigned int a; } ST;
ST _huge * st1;
ST _far  * st2;

void f( signed int );

void main( void )
{
   f( st1 == st2 );
}

WORKAROUND:

Make both pointer _far or _huge.

-------------------------------------------------------------------------------
KNOWN PROBLEMS
-------------------------------------------------------------------------------
KNOWN CR7334: SYMB DEBUG INFO NON EXISTING VARIABLES (DUE TO CC OPTIM)

PROBLEM DESCRIPTION:

If the compiler does not know the address of a variable, due to the
fact that the variable does not exist as a result of optimizations, the
compiler locates the variable at address -1.

This address is placed in the HP/MRI file. It would be better if no symbolic
debug info is produced for such a variable. The compiler frontend generates
a "symb" record, this can't be removed by the compiler backend. The HP/MRI
convertor in the locator should detect "non-existing" variables and suppress
the associated debug information.

-------------------------------------------------------------------------------
KNOWN CR7574: NO HUGE ALLOC (HALLOC) SUPPORT

PROBLEM DESCRIPTION:

It's not possible to allocate memory for a data object of size >64Kb.

-------------------------------------------------------------------------------
KNOWN CR8296: extra long compile due to possible variable 'over-usage'

PROBLEM DESCRIPTION:

A particular source containing repeated lines of the following statement:

window = (( window + window ) | (unsigned char) DROPxx) & 0x7F;

where DROPxx is of the _bit (xx being 0, 1, 2 ......)
and window is of type unsigned char.

WORKAROUND:

Replacing (window+window) with 2*window does perform miracles.

-------------------------------------------------------------------------------
KNOWN CR8674: no warning issued on assigment of an _atbit declaration

PROBLEM DESCRIPTION:

No warning is emitted for the following construction...

_bitbyte y ;
_bit z _atbit(y,0) = 0 ;

User should ne informed that this is illegal and that _atbit will be ignored.

-------------------------------------------------------------------------------
KNOWN CR8722: parameters passing via stack

PROBLEM DESCRIPTION:

Make it possible to also allow parameter passing via stack-only.

-------------------------------------------------------------------------------
KNOWN CR8895: merging copy tables for duplicate strings

PROBLEM DESCRIPTION:


The compiler generates two copy tables for the following declaration:

char S[1000] = "" ;
char Q[1000] = "" ;

As "" is the same for both 'S' and 'Q' and their sizes are equal too that
could share the same copy table.
