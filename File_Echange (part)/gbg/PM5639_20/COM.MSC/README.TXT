The major differences between Borland C/C++ 3.0 and Microsoft C/C++ 7.0:


The File PM39IO.C:

Include files:

Borland					alloc.h
Microsoft				malloc.h

Borland					asm { .... }
Microsoft				__asm { .... }

Borland					void interrupt (*name)();
Microsoft				void (__interrupt __far *name)();

Borland					inportb(..)
Microsoft				_inp(..)

Borland					outport(..)
Microsoft				_outp(..)

Borland					intr(..)
Microsoft				_int86(..)

Borland					struct REGPACK reg
Microsoft				union _REGS reg

Borland					reg.r_ax, reg.r_bx etc.
Microsoft				reg.x.ax, reg.x.bx etc.

Borland					getvect(..)
Microsoft				_dos_getvect(..)    *)

Borland					setvect(..)
Microsoft				_dos_setvect(..)    *)


*): This definition also exists in Borland



Concerning the file DEMO.C:

Since the graphical interface are very different in Borland C/C++ and Microsoft
C/C++ 