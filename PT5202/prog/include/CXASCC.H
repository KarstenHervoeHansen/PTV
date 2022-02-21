/*
 *	@(#)cxascc.h	1.2 11/26/97
 */

/**************************************************************************
**                                                                        *
**  FILE        :  cxascc.h                                               *
**                                                                        *
**  DESCRIPTION :  This include file contains the declarations for the	  *
**		   XA-SCC Memory Mapped Registers (MMR).		  *
**									  *
**		   The MMR Base address is defined with the macro 	  *
**		   _XASCC_MMRBASE. The _XASCC_MMRBASE is default set to   *
**		   0xf000 and can be overruled by redefining it.	  *
**									  *
**		   The MMR registers are defined when the macro _XASCC_DEF*
**		   is set. An external declaration of the MMR registers   *
**		   is available when the macro _XASCC_DEF is not set.	  *
**                                                                        *
**  COPYRIGHT   :  1997 TASKING, Inc.                                     *
**                                                                        *
**************************************************************************/

#ifndef _XASCC_H
#define _XASCC_H

#ifndef	_XASCC_MMRBASE
#define _XASCC_MMRBASE	0xF000
#endif

#define _XASCC_MMRADDRESS( _XASCC_MMROFFSET )	( _XASCC_MMRBASE + _XASCC_MMROFFSET )
#ifndef	_XASCC_DEF
#define _AT( x )
#define _EXTERN					extern
#else
#define _AT( x )				_at( x )
#define _EXTERN
#endif

/*
 *	DMA registers
 */
_EXTERN _far unsigned char volatile SCC0RXCON      _AT( _XASCC_MMRADDRESS( 0x100 ) );
_EXTERN _far unsigned char volatile SCC0RXSTAT     _AT( _XASCC_MMRADDRESS( 0x101 ) );
_EXTERN _far unsigned char volatile SCC0RXSEG      _AT( _XASCC_MMRADDRESS( 0x102 ) );
_EXTERN _far unsigned char volatile SCC0RXBUFBASE  _AT( _XASCC_MMRADDRESS( 0x104 ) );
_EXTERN _far unsigned int  volatile SCC0RXBUFBOUND _AT( _XASCC_MMRADDRESS( 0x106 ) );
_EXTERN _far unsigned int  volatile SCC0RXADDPTR   _AT( _XASCC_MMRADDRESS( 0x108 ) );
_EXTERN _far unsigned int  volatile SCC0RXCOUNT    _AT( _XASCC_MMRADDRESS( 0x10A ) );
_EXTERN _far unsigned int  volatile SCC0RXDATALO   _AT( _XASCC_MMRADDRESS( 0x10C ) );
_EXTERN _far unsigned int  volatile SCC0RXDATAHI   _AT( _XASCC_MMRADDRESS( 0x10E ) );
_EXTERN _far unsigned char volatile SCC1RXCON      _AT( _XASCC_MMRADDRESS( 0x110 ) );
_EXTERN _far unsigned char volatile SCC1RXSTAT     _AT( _XASCC_MMRADDRESS( 0x111 ) );
_EXTERN _far unsigned char volatile SCC1RXSEG      _AT( _XASCC_MMRADDRESS( 0x112 ) );
_EXTERN _far unsigned char volatile SCC1RXBUFBASE  _AT( _XASCC_MMRADDRESS( 0x114 ) );
_EXTERN _far unsigned int  volatile SCC1RXBUFBOUND _AT( _XASCC_MMRADDRESS( 0x116 ) );
_EXTERN _far unsigned int  volatile SCC1RXADDPTR   _AT( _XASCC_MMRADDRESS( 0x118 ) );
_EXTERN _far unsigned int  volatile SCC1RXCOUNT    _AT( _XASCC_MMRADDRESS( 0x11A ) );
_EXTERN _far unsigned int  volatile SCC1RXDATALO   _AT( _XASCC_MMRADDRESS( 0x11C ) );
_EXTERN _far unsigned int  volatile SCC1RXDATAHI   _AT( _XASCC_MMRADDRESS( 0x11E ) );
_EXTERN _far unsigned char volatile SCC2RXCON      _AT( _XASCC_MMRADDRESS( 0x120 ) );
_EXTERN _far unsigned char volatile SCC2RXSTAT     _AT( _XASCC_MMRADDRESS( 0x121 ) );
_EXTERN _far unsigned char volatile SCC2RXSEG      _AT( _XASCC_MMRADDRESS( 0x122 ) );
_EXTERN _far unsigned char volatile SCC2RXBUFBASE  _AT( _XASCC_MMRADDRESS( 0x124 ) );
_EXTERN _far unsigned int  volatile SCC2RXBUFBOUND _AT( _XASCC_MMRADDRESS( 0x126 ) );
_EXTERN _far unsigned int  volatile SCC2RXADDPTR   _AT( _XASCC_MMRADDRESS( 0x128 ) );
_EXTERN _far unsigned int  volatile SCC2RXCOUNT    _AT( _XASCC_MMRADDRESS( 0x12A ) );
_EXTERN _far unsigned int  volatile SCC2RXDATALO   _AT( _XASCC_MMRADDRESS( 0x12C ) );
_EXTERN _far unsigned int  volatile SCC2RXDATAHI   _AT( _XASCC_MMRADDRESS( 0x12E ) );
_EXTERN _far unsigned char volatile SCC3RXCON      _AT( _XASCC_MMRADDRESS( 0x130 ) );
_EXTERN _far unsigned char volatile SCC3RXSTAT     _AT( _XASCC_MMRADDRESS( 0x131 ) );
_EXTERN _far unsigned char volatile SCC3RXSEG      _AT( _XASCC_MMRADDRESS( 0x132 ) );
_EXTERN _far unsigned char volatile SCC3RXBUFBASE  _AT( _XASCC_MMRADDRESS( 0x134 ) );
_EXTERN _far unsigned int  volatile SCC3RXBUFBOUND _AT( _XASCC_MMRADDRESS( 0x136 ) );
_EXTERN _far unsigned int  volatile SCC3RXADDPTR   _AT( _XASCC_MMRADDRESS( 0x138 ) );
_EXTERN _far unsigned int  volatile SCC3RXCOUNT    _AT( _XASCC_MMRADDRESS( 0x13A ) );
_EXTERN _far unsigned int  volatile SCC3RXDATALO   _AT( _XASCC_MMRADDRESS( 0x13C ) );
_EXTERN _far unsigned int  volatile SCC3RXDATAHI   _AT( _XASCC_MMRADDRESS( 0x13E ) );
_EXTERN _far unsigned char volatile SCC0TXCON      _AT( _XASCC_MMRADDRESS( 0x140 ) ); 
_EXTERN _far unsigned char volatile SCC0TXSTAT     _AT( _XASCC_MMRADDRESS( 0x141 ) );
_EXTERN _far unsigned char volatile SCC0TXSEG      _AT( _XASCC_MMRADDRESS( 0x142 ) );
_EXTERN _far unsigned char volatile SCC0TXBUFBASE  _AT( _XASCC_MMRADDRESS( 0x144 ) );
_EXTERN _far unsigned int  volatile SCC0TXBUFBOUND _AT( _XASCC_MMRADDRESS( 0x146 ) );
_EXTERN _far unsigned int  volatile SCC0TXADDPTR   _AT( _XASCC_MMRADDRESS( 0x148 ) );
_EXTERN _far unsigned int  volatile SCC0TXCOUNT    _AT( _XASCC_MMRADDRESS( 0x14A ) );
_EXTERN _far unsigned int  volatile SCC0TXDATALO   _AT( _XASCC_MMRADDRESS( 0x14C ) );
_EXTERN _far unsigned int  volatile SCC0TXDATAHI   _AT( _XASCC_MMRADDRESS( 0x14E ) );
_EXTERN _far unsigned char volatile SCC1TXCON      _AT( _XASCC_MMRADDRESS( 0x150 ) );
_EXTERN _far unsigned char volatile SCC1TXSTAT     _AT( _XASCC_MMRADDRESS( 0x151 ) );
_EXTERN _far unsigned char volatile SCC1TXSEG      _AT( _XASCC_MMRADDRESS( 0x152 ) );
_EXTERN _far unsigned char volatile SCC1TXBUFBASE  _AT( _XASCC_MMRADDRESS( 0x154 ) );
_EXTERN _far unsigned int  volatile SCC1TXBUFBOUND _AT( _XASCC_MMRADDRESS( 0x156 ) );
_EXTERN _far unsigned int  volatile SCC1TXADDPTR   _AT( _XASCC_MMRADDRESS( 0x158 ) );
_EXTERN _far unsigned int  volatile SCC1TXCOUNT    _AT( _XASCC_MMRADDRESS( 0x15A ) );
_EXTERN _far unsigned int  volatile SCC1TXDATALO   _AT( _XASCC_MMRADDRESS( 0x15C ) );
_EXTERN _far unsigned int  volatile SCC1TXDATAHI   _AT( _XASCC_MMRADDRESS( 0x15E ) );
_EXTERN _far unsigned char volatile SCC2TXCON      _AT( _XASCC_MMRADDRESS( 0x160 ) );
_EXTERN _far unsigned char volatile SCC2TXSTAT     _AT( _XASCC_MMRADDRESS( 0x161 ) );
_EXTERN _far unsigned char volatile SCC2TXSEG      _AT( _XASCC_MMRADDRESS( 0x162 ) );
_EXTERN _far unsigned char volatile SCC2TXBUFBASE  _AT( _XASCC_MMRADDRESS( 0x164 ) );
_EXTERN _far unsigned int  volatile SCC2TXBUFBOUND _AT( _XASCC_MMRADDRESS( 0x166 ) );
_EXTERN _far unsigned int  volatile SCC2TXADDPTR   _AT( _XASCC_MMRADDRESS( 0x168 ) );
_EXTERN _far unsigned int  volatile SCC2TXCOUNT    _AT( _XASCC_MMRADDRESS( 0x16A ) );
_EXTERN _far unsigned int  volatile SCC2TXDATALO   _AT( _XASCC_MMRADDRESS( 0x16C ) );
_EXTERN _far unsigned int  volatile SCC2TXDATAHI   _AT( _XASCC_MMRADDRESS( 0x16E ) );
_EXTERN _far unsigned char volatile SCC3TXCON      _AT( _XASCC_MMRADDRESS( 0x170 ) );
_EXTERN _far unsigned char volatile SCC3TXSTAT     _AT( _XASCC_MMRADDRESS( 0x171 ) );
_EXTERN _far unsigned char volatile SCC3TXSEG      _AT( _XASCC_MMRADDRESS( 0x172 ) );
_EXTERN _far unsigned char volatile SCC3TXBUFBASE  _AT( _XASCC_MMRADDRESS( 0x174 ) );
_EXTERN _far unsigned int  volatile SCC3TXBUFBOUND _AT( _XASCC_MMRADDRESS( 0x176 ) );
_EXTERN _far unsigned int  volatile SCC3TXADDPTR   _AT( _XASCC_MMRADDRESS( 0x178 ) );
_EXTERN _far unsigned int  volatile SCC3TXCOUNT    _AT( _XASCC_MMRADDRESS( 0x17A ) );
_EXTERN _far unsigned int  volatile SCC3TXDATALO   _AT( _XASCC_MMRADDRESS( 0x17C ) );
_EXTERN _far unsigned int  volatile SCC3TXDATAHI   _AT( _XASCC_MMRADDRESS( 0x17E ) );
_EXTERN _far unsigned char volatile SCC0RXTIMEOUT  _AT( _XASCC_MMRADDRESS( 0x200 ) );
_EXTERN _far unsigned char volatile SCC1RXTIMEOUT  _AT( _XASCC_MMRADDRESS( 0x202 ) );
_EXTERN _far unsigned char volatile SCC2RXTIMEOUT  _AT( _XASCC_MMRADDRESS( 0x204 ) );
_EXTERN _far unsigned char volatile SCC3RXTIMEOUT  _AT( _XASCC_MMRADDRESS( 0x206 ) );
_EXTERN _far unsigned int  volatile G_DMA_INT      _AT( _XASCC_MMRADDRESS( 0x210 ) );		       

/*
 *	V.54 2047 Unit registers
 */
_EXTERN _far unsigned char volatile VACS       _AT( _XASCC_MMRADDRESS( 0x240 ) );
_EXTERN _far unsigned char volatile VACFG      _AT( _XASCC_MMRADDRESS( 0x241 ) );
_EXTERN _far unsigned char volatile VATCL      _AT( _XASCC_MMRADDRESS( 0x242 ) );
_EXTERN _far unsigned char volatile VATCH      _AT( _XASCC_MMRADDRESS( 0x243 ) );
_EXTERN _far unsigned char volatile VAEC       _AT( _XASCC_MMRADDRESS( 0x244 ) );
_EXTERN _far unsigned char volatile VBCS       _AT( _XASCC_MMRADDRESS( 0x248 ) );
_EXTERN _far unsigned char volatile VBCFG      _AT( _XASCC_MMRADDRESS( 0x249 ) );
_EXTERN _far unsigned char volatile VBTCL      _AT( _XASCC_MMRADDRESS( 0x24A ) );
_EXTERN _far unsigned char volatile VBTCH      _AT( _XASCC_MMRADDRESS( 0x24B ) );
_EXTERN _far unsigned char volatile VBEC       _AT( _XASCC_MMRADDRESS( 0x24C ) );

/*
 *	SCP/SPI registers
 */
_EXTERN _far unsigned char volatile SPCFG      _AT( _XASCC_MMRADDRESS( 0x260 ) );
_EXTERN _far unsigned char volatile SPD        _AT( _XASCC_MMRADDRESS( 0x262 ) );
_EXTERN _far unsigned char volatile SPCS       _AT( _XASCC_MMRADDRESS( 0x263 ) );

/*
 *	Autobaud registers
 */
_EXTERN _far unsigned char volatile BDAEE      _AT( _XASCC_MMRADDRESS( 0x270 ) );
_EXTERN _far unsigned char volatile BDCS       _AT( _XASCC_MMRADDRESS( 0x272 ) );

/*
 * MIF Registers
 */
_EXTERN _far unsigned char volatile B0CFG      _AT( _XASCC_MMRADDRESS( 0x280 ) );
_EXTERN _far unsigned char volatile B0AM       _AT( _XASCC_MMRADDRESS( 0x281 ) );
_EXTERN _far unsigned char volatile B0TMG      _AT( _XASCC_MMRADDRESS( 0x282 ) );
_EXTERN _far unsigned char volatile B1CFG      _AT( _XASCC_MMRADDRESS( 0x284 ) );
_EXTERN _far unsigned char volatile B1AM       _AT( _XASCC_MMRADDRESS( 0x285 ) );
_EXTERN _far unsigned char volatile B1TMG      _AT( _XASCC_MMRADDRESS( 0x286 ) );
_EXTERN _far unsigned char volatile B2CFG      _AT( _XASCC_MMRADDRESS( 0x288 ) );
_EXTERN _far unsigned char volatile B2AM       _AT( _XASCC_MMRADDRESS( 0x289 ) );
_EXTERN _far unsigned char volatile B2TMG      _AT( _XASCC_MMRADDRESS( 0x28A ) );
_EXTERN _far unsigned char volatile B3CFG      _AT( _XASCC_MMRADDRESS( 0x28C ) );
_EXTERN _far unsigned char volatile B3AM       _AT( _XASCC_MMRADDRESS( 0x28D ) );
_EXTERN _far unsigned char volatile B3TMG      _AT( _XASCC_MMRADDRESS( 0x28E ) );
_EXTERN _far unsigned char volatile B4CFG      _AT( _XASCC_MMRADDRESS( 0x290 ) );
_EXTERN _far unsigned char volatile B4AM       _AT( _XASCC_MMRADDRESS( 0x291 ) );
_EXTERN _far unsigned char volatile B4TMG      _AT( _XASCC_MMRADDRESS( 0x292 ) );
_EXTERN _far unsigned char volatile B5CFG      _AT( _XASCC_MMRADDRESS( 0x294 ) );
_EXTERN _far unsigned char volatile B5AM       _AT( _XASCC_MMRADDRESS( 0x295 ) );
_EXTERN _far unsigned char volatile B5TMG      _AT( _XASCC_MMRADDRESS( 0x296 ) );
_EXTERN _far unsigned char volatile MBCL       _AT( _XASCC_MMRADDRESS( 0x2BE ) );
_EXTERN _far unsigned char volatile RFSH       _AT( _XASCC_MMRADDRESS( 0x2BF ) );

/*
 *	IDL Stuff
 */
_EXTERN _far unsigned char volatile SIMODE     _AT( _XASCC_MMRADDRESS( 0x2C0 ) );
_EXTERN _far unsigned int  volatile SIMASK     _AT( _XASCC_MMRADDRESS( 0x2C2 ) );
_EXTERN _far unsigned int  volatile SOFTINTPM  _AT( _XASCC_MMRADDRESS( 0x2D0 ) );
_EXTERN _far unsigned char volatile XINT2      _AT( _XASCC_MMRADDRESS( 0x2D2 ) );

/*
 *	SCC Registers
 */
_EXTERN _far unsigned char volatile SCC0WR0    _AT( _XASCC_MMRADDRESS( 0x800 ) );
_EXTERN _far unsigned char volatile SCC0WR1    _AT( _XASCC_MMRADDRESS( 0x802 ) );
_EXTERN _far unsigned char volatile SCC0WR7P   _AT( _XASCC_MMRADDRESS( 0x804 ) );
_EXTERN _far unsigned char volatile SCC0WR3    _AT( _XASCC_MMRADDRESS( 0x806 ) );
_EXTERN _far unsigned char volatile SCC0WR4    _AT( _XASCC_MMRADDRESS( 0x808 ) );
_EXTERN _far unsigned char volatile SCC0WR5    _AT( _XASCC_MMRADDRESS( 0x80A ) );
_EXTERN _far unsigned char volatile SCC0WR6    _AT( _XASCC_MMRADDRESS( 0x80C ) );
_EXTERN _far unsigned char volatile SCC0WR7    _AT( _XASCC_MMRADDRESS( 0x80E ) );
_EXTERN _far unsigned char volatile SCC0WR8    _AT( _XASCC_MMRADDRESS( 0x810 ) );
_EXTERN _far unsigned char volatile SCC0WR9    _AT( _XASCC_MMRADDRESS( 0x812 ) );
_EXTERN _far unsigned char volatile SCC0WR10   _AT( _XASCC_MMRADDRESS( 0x814 ) );
_EXTERN _far unsigned char volatile SCC0WR11   _AT( _XASCC_MMRADDRESS( 0x816 ) );
_EXTERN _far unsigned char volatile SCC0WR12   _AT( _XASCC_MMRADDRESS( 0x818 ) );
_EXTERN _far unsigned char volatile SCC0WR13   _AT( _XASCC_MMRADDRESS( 0x81A ) );
_EXTERN _far unsigned char volatile SCC0WR14   _AT( _XASCC_MMRADDRESS( 0x81C ) );
_EXTERN _far unsigned char volatile SCC0WR15   _AT( _XASCC_MMRADDRESS( 0x81E ) );
_EXTERN _far unsigned char volatile SCC0RR0    _AT( _XASCC_MMRADDRESS( 0x820 ) );
_EXTERN _far unsigned char volatile SCC0RR1    _AT( _XASCC_MMRADDRESS( 0x822 ) );
_EXTERN _far unsigned char volatile SCC0RR6    _AT( _XASCC_MMRADDRESS( 0x82C ) );
_EXTERN _far unsigned char volatile SCC0RR7    _AT( _XASCC_MMRADDRESS( 0x82E ) );
_EXTERN _far unsigned char volatile SCC0RR8    _AT( _XASCC_MMRADDRESS( 0x830 ) );
_EXTERN _far unsigned char volatile SCC0RR10   _AT( _XASCC_MMRADDRESS( 0x834 ) );
_EXTERN _far unsigned char volatile SCC1WR0    _AT( _XASCC_MMRADDRESS( 0x840 ) );
_EXTERN _far unsigned char volatile SCC1WR1    _AT( _XASCC_MMRADDRESS( 0x842 ) );
_EXTERN _far unsigned char volatile SCC1WR7P   _AT( _XASCC_MMRADDRESS( 0x844 ) );
_EXTERN _far unsigned char volatile SCC1WR3    _AT( _XASCC_MMRADDRESS( 0x846 ) );
_EXTERN _far unsigned char volatile SCC1WR4    _AT( _XASCC_MMRADDRESS( 0x848 ) );
_EXTERN _far unsigned char volatile SCC1WR5    _AT( _XASCC_MMRADDRESS( 0x84A ) );
_EXTERN _far unsigned char volatile SCC1WR6    _AT( _XASCC_MMRADDRESS( 0x84C ) );
_EXTERN _far unsigned char volatile SCC1WR7    _AT( _XASCC_MMRADDRESS( 0x84E ) );
_EXTERN _far unsigned char volatile SCC1WR8    _AT( _XASCC_MMRADDRESS( 0x850 ) );
_EXTERN _far unsigned char volatile SCC1WR9    _AT( _XASCC_MMRADDRESS( 0x852 ) );
_EXTERN _far unsigned char volatile SCC1WR10   _AT( _XASCC_MMRADDRESS( 0x854 ) );
_EXTERN _far unsigned char volatile SCC1WR11   _AT( _XASCC_MMRADDRESS( 0x856 ) );
_EXTERN _far unsigned char volatile SCC1WR12   _AT( _XASCC_MMRADDRESS( 0x858 ) );
_EXTERN _far unsigned char volatile SCC1WR13   _AT( _XASCC_MMRADDRESS( 0x85A ) );
_EXTERN _far unsigned char volatile SCC1WR14   _AT( _XASCC_MMRADDRESS( 0x85C ) );
_EXTERN _far unsigned char volatile SCC1WR15   _AT( _XASCC_MMRADDRESS( 0x85E ) );
_EXTERN _far unsigned char volatile SCC1RR0    _AT( _XASCC_MMRADDRESS( 0x860 ) );
_EXTERN _far unsigned char volatile SCC1RR1    _AT( _XASCC_MMRADDRESS( 0x862 ) );
_EXTERN _far unsigned char volatile SCC1RR6    _AT( _XASCC_MMRADDRESS( 0x86C ) );
_EXTERN _far unsigned char volatile SCC1RR7    _AT( _XASCC_MMRADDRESS( 0x86E ) );
_EXTERN _far unsigned char volatile SCC1RR8    _AT( _XASCC_MMRADDRESS( 0x870 ) );
_EXTERN _far unsigned char volatile SCC2WR0    _AT( _XASCC_MMRADDRESS( 0x880 ) );
_EXTERN _far unsigned char volatile SCC2WR1    _AT( _XASCC_MMRADDRESS( 0x882 ) );
_EXTERN _far unsigned char volatile SCC2WR7P   _AT( _XASCC_MMRADDRESS( 0x884 ) );
_EXTERN _far unsigned char volatile SCC2WR3    _AT( _XASCC_MMRADDRESS( 0x886 ) );
_EXTERN _far unsigned char volatile SCC2WR4    _AT( _XASCC_MMRADDRESS( 0x888 ) );
_EXTERN _far unsigned char volatile SCC2WR5    _AT( _XASCC_MMRADDRESS( 0x88A ) );
_EXTERN _far unsigned char volatile SCC2WR6    _AT( _XASCC_MMRADDRESS( 0x88C ) );
_EXTERN _far unsigned char volatile SCC2WR7    _AT( _XASCC_MMRADDRESS( 0x88E ) );
_EXTERN _far unsigned char volatile SCC2WR8    _AT( _XASCC_MMRADDRESS( 0x890 ) );
_EXTERN _far unsigned char volatile SCC2WR9    _AT( _XASCC_MMRADDRESS( 0x892 ) );
_EXTERN _far unsigned char volatile SCC2WR10   _AT( _XASCC_MMRADDRESS( 0x894 ) );
_EXTERN _far unsigned char volatile SCC2WR11   _AT( _XASCC_MMRADDRESS( 0x896 ) );
_EXTERN _far unsigned char volatile SCC2WR12   _AT( _XASCC_MMRADDRESS( 0x898 ) );
_EXTERN _far unsigned char volatile SCC2WR13   _AT( _XASCC_MMRADDRESS( 0x89A ) );
_EXTERN _far unsigned char volatile SCC2WR14   _AT( _XASCC_MMRADDRESS( 0x89C ) );
_EXTERN _far unsigned char volatile SCC2WR15   _AT( _XASCC_MMRADDRESS( 0x89E ) );
_EXTERN _far unsigned char volatile SCC2RR0    _AT( _XASCC_MMRADDRESS( 0x8A0 ) );
_EXTERN _far unsigned char volatile SCC2RR1    _AT( _XASCC_MMRADDRESS( 0x8A2 ) );
_EXTERN _far unsigned char volatile SCC2RR6    _AT( _XASCC_MMRADDRESS( 0x8AC ) );
_EXTERN _far unsigned char volatile SCC2RR7    _AT( _XASCC_MMRADDRESS( 0x8AE ) );
_EXTERN _far unsigned char volatile SCC2RR8    _AT( _XASCC_MMRADDRESS( 0x8B0 ) );
_EXTERN _far unsigned char volatile SCC2RR10   _AT( _XASCC_MMRADDRESS( 0x8B4 ) );
_EXTERN _far unsigned char volatile SCC3WR0    _AT( _XASCC_MMRADDRESS( 0x8C0 ) );
_EXTERN _far unsigned char volatile SCC3WR1    _AT( _XASCC_MMRADDRESS( 0x8C2 ) );
_EXTERN _far unsigned char volatile SCC3WR7P   _AT( _XASCC_MMRADDRESS( 0x8C4 ) );
_EXTERN _far unsigned char volatile SCC3WR3    _AT( _XASCC_MMRADDRESS( 0x8C6 ) );
_EXTERN _far unsigned char volatile SCC3WR4    _AT( _XASCC_MMRADDRESS( 0x8C8 ) );
_EXTERN _far unsigned char volatile SCC3WR5    _AT( _XASCC_MMRADDRESS( 0x8CA ) );
_EXTERN _far unsigned char volatile SCC3WR6    _AT( _XASCC_MMRADDRESS( 0x8CC ) );
_EXTERN _far unsigned char volatile SCC3WR7    _AT( _XASCC_MMRADDRESS( 0x8CE ) );
_EXTERN _far unsigned char volatile SCC3WR8    _AT( _XASCC_MMRADDRESS( 0x8D0 ) );
_EXTERN _far unsigned char volatile SCC3WR9    _AT( _XASCC_MMRADDRESS( 0x8D2 ) );
_EXTERN _far unsigned char volatile SCC3WR10   _AT( _XASCC_MMRADDRESS( 0x8D4 ) );
_EXTERN _far unsigned char volatile SCC3WR11   _AT( _XASCC_MMRADDRESS( 0x8D6 ) );
_EXTERN _far unsigned char volatile SCC3WR12   _AT( _XASCC_MMRADDRESS( 0x8D8 ) );
_EXTERN _far unsigned char volatile SCC3WR13   _AT( _XASCC_MMRADDRESS( 0x8DA ) );
_EXTERN _far unsigned char volatile SCC3WR14   _AT( _XASCC_MMRADDRESS( 0x8DC ) );
_EXTERN _far unsigned char volatile SCC3WR15   _AT( _XASCC_MMRADDRESS( 0x8DE ) );
_EXTERN _far unsigned char volatile SCC3RR0    _AT( _XASCC_MMRADDRESS( 0x8E0 ) );
_EXTERN _far unsigned char volatile SCC3RR1    _AT( _XASCC_MMRADDRESS( 0x8E2 ) );
_EXTERN _far unsigned char volatile SCC3RR6    _AT( _XASCC_MMRADDRESS( 0x8EC ) );
_EXTERN _far unsigned char volatile SCC3RR7    _AT( _XASCC_MMRADDRESS( 0x8EE ) );
_EXTERN _far unsigned char volatile SCC3RR8    _AT( _XASCC_MMRADDRESS( 0x8F0 ) );
_EXTERN _far unsigned char volatile SCC3RR10   _AT( _XASCC_MMRADDRESS( 0x8F4 ) );

#endif /* _XASCC_H */
