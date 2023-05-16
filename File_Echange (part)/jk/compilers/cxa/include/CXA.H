/*
 *	@(#)cxa.h	1.5 12/16/96
 */

/**************************************************************************
**                                                                        *
**  FILE        :  cxa.h                                                  *
**                                                                        *
**  DESCRIPTION :  Include file with macros to enable or disable the      *
**                 usage of Tasking C-XA language extensions.             *
**                 cxa has the following predefined preprocessor          *
**                 symbols:                                               *
**                           _CXA     value = version of compiler         *
**                                     (e.g. value 10 for v1.0)           *
**                           _MODEL    memory model used:                 *
**                                     tiny:        value is 't'          *
**                                     small:       value is 's'          *
**                                     medium:      value is 'm'          *
**                                     large:       value is 'l'          *
**                                                                        *
**  COPYRIGHT   :  1996 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef CXA_INC

/*
 * When using the Tasking C-XA compiler, recognize language extensions
 */

#ifndef _CXA
/* 
 * When not using the Tasking C-XA compiler, define the memory type
 * specifiers away. There is no need to remove const.
 */

/* pointer type */
#define	_near
#define _far
#define _huge
#define	_data
#define	_idat
#define _pdat
#define	_xdat
#define	_bdat	
#define	_rom
	
#define	_using( X )
#define	_interrupt( X )
#define _asmfunc
#define _inline	

#define	_bit		unsigned char
#define	_sfrbit		unsigned char
#define	_sfrbyte	unsigned char

#define	_bitbyte	unsigned char
#define	_at( X )
#define	_atbit( X, Y )

/*
 * The following functions are built-in functions of the CXA compiler.
 * A prototype is given here, so other compilers will recognize
 * the function names and types. The user should make alternatives
 * for these functions to prototype the application on another host.
 */
void		_strmovc	( char _far *, _rom const char *	         );
void		_strmovx	( char _far *, _xdat char *		         );
void		_memmovc	( void _far *, _rom const void *, unsigned int   );
void		_memmovx	( void _far *, _xdat void *, unsigned int        );
_bit		_testclear	( _bit 					         );
unsigned char	_da		( unsigned char				         );
void		_nop		( void  				         );
unsigned char	_rol		( unsigned char, unsigned char 		         );
unsigned char	_ror		( unsigned char, unsigned char 		         );
unsigned char	_rol8		( unsigned char, unsigned char 		         );
unsigned char	_ror8		( unsigned char, unsigned char 		         );
unsigned char	_rolc8		( unsigned char, unsigned char 		         );
unsigned char	_rorc8		( unsigned char, unsigned char 		         );
unsigned int	_rol16		( unsigned int, unsigned char 		         );
unsigned int	_ror16		( unsigned int, unsigned char 		         );
unsigned int	_rolc16		( unsigned int, unsigned char 		         );
unsigned int	_rorc16		( unsigned int, unsigned char 		         );
void		_intxa		( unsigned int  		  		 );
int		_div32		( long, int  				         );
unsigned int	_divu32		( unsigned long, unsigned int  		         );
int		_mod32		( long, int  				         );
unsigned int	_modu32		( unsigned long, unsigned int 	 	         );
char		_div16		( int, char  				         );
unsigned char	_divu16		( unsigned int, unsigned char  		         );
char		_mod16		( int, char  				         );
unsigned char	_modu16		( unsigned int, unsigned char 	 	         );

#endif

#define	CXA_INC
#endif
