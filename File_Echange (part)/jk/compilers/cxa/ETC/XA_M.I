/**************************************************************************
**
**      FILE        :  @(#)xa_m.i	1.5
**
**      VERSION     :  97/11/21
**
**      DESCRIPTION :  Default locator control file for tiny model,
**		       this file is included from xa_t.dsc
**
**	NOTE	    :  All available locator controls are listed below
**
**      COPYRIGHT   :  1997 TASKING, Inc.
**
**************************************************************************/

/*
 *	Default memory definition
 */
#ifdef _UNIFIED_BUS
#ifdef _X_CODE_ROM
_X_CODE_ROM(x_code0,_ICODE,07FFFH)	//	assume 224k external code rom
_X_CODE_ROM(x_code1,(_ICODE+010000H),03FFFFH)
#endif
#ifdef _X_DATA_RAM
_X_DATA_RAM(x_ram_data,08000H,0FFFFH)	//	assume 32k external data ram
#endif
#else
#ifdef _X_CODE_ROM
_X_CODE_ROM(x_code,_ICODE,03FFFFH)	//	assume 224k external code rom
#endif
#ifdef _X_DATA_RAM
_X_DATA_RAM(x_ram_data,0,0FBFFH)	//	assume 63k external data ram
#endif
#ifdef _X_DATA_ROM
_X_DATA_ROM(x_rom_data,0FC00H,0FFFFH)	//	assume 1k external data rom
#endif
#endif

#ifndef _HEAPSZ
#define	_HEAP_SZ	10k		//	set default Heap size for malloc()/free()
#endif

/**************************************************************************
Locator control			Default		Description

	Compiler and derivative used:
_REGSFR				regxag3.dat	Special Function Register file for CrossView 
						Pro Debugger

	Processor configuration:
_IRAM				200H		Internal RAM size
_ICODE				32k		Internal ROM size
_ZPAGE				1		0: No Page Zero mode1: Page Zero mode

	Memory Model:		
_MODEL_TINY			False		Define if you are using the tiny memory model
_MODEL_SMALL			True		Define if you are using the small memory model
_MODEL_MEDIUM			False		Define if you are using the medium memory model
_MODEL_LARGE			False		Define if you are using the large memory model

	External memory:		
_UNIFIED_BUS			0		0: Seperate external code and data bus (Harvard)
						1: Unified external code and data bus (linear)
_XMEM				1		0: Single chip solution (no external memory)
						1:External memory areas specified by (one 
						or more) _XRAM and _XROM macros
_X_DATA_RAM(name, addr, size)	0,64k		External Data RAM area: chip-name, start-address, 
						size Must be within #ifdef _X_DATA_RAM and #endif
_X_DATA_ROM(name, addr, size)	64k,128k	External Data ROM area: chip-name, start-address, 
						size Must be within #ifdef _X_DATA_ROM and #endif
_X_CODE_ROM(name, addr, size)	32k,96k		External Code ROM area: chip-name, start-address,
						size Must be within #ifdef _X_CODE_ROM and #endif
_DRESERVES			False		Define if you want to reserve some memory area 
						on the DATA bus
_DRESERVED(startaddr,endaddr)	--		Reserve memory area on the DATA bus: start-address, 
						end-address
_CRESERVES			False		Define if you want to reserve some memory area 
						on the CODE bus
_CRESERVED(startaddr,endaddr)	--		Reserve memory area on the CODE bus: start-address,
						end-address
_CODEPAGE			0H		Map code page (64k) to this page number
_RESET_VECTOR			0H		Allocate reset vector (segment in startup code) 
						at this address

	Special segments:		
_STACK_SZ			--		System stack size
_STACK_LC			--		Location of the system stack
_HEAP_SZ			--		Heap size for malloc() and free()
_HEAP_LC			--		Location of the heap
_XVWBUF				8		CrossView Pro buffer size (strings in target memory)

	Order (and optional address) of user segments:		
_BAORDER			False		Define if you want to use the _BITADDR control
_BITADDR(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _BITADDR control.
						Must be within #ifdef _BITADDR and #endif
_BORDER				False		Define if you want to use the _BIT control
_BIT(name [addr=addr] )		--		Specify segment with name must be located at address 
						addr or before/after another _BIT control.
						Must be within #ifdef _BIT and #endif
_DORDER				False		Define if you want to use the _DATA control
_DATA(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _DATA control.
						Must be within #ifdef _DATA and #endif
_IORDER				False		Define if you want to use the _IDATA control
_IDATA(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _IDATA control.
						Must be within #ifdef _IDATA and #endif
_HORDER				False		Define if you want to use the _HDATA control
_HDATA(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _HDATA control.
						Must be within #ifdef _HDATA and #endif
_RDORDER			False		Define if you want to use the _RDATA control
_RDATA(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _RDATA control.
						Must be within #ifdef _RDATA and #endif
_RIORDER			False		Define if you want to use the _RIDATA control
_RIDATA(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _RIDATA control.
						Must be within #ifdef _RIDATA and #endif
_RHORDER			False		Define if you want to use the _RHDATA control
_RHDATA(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _RHDATA control.
						Must be within #ifdef _RHDATA and #endif
_XSORDER			False		Define if you want to use the _XSHORT control
_XSHORT(name[ addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _XSHORT control.
						Must be within #ifdef _XSHORT and #endif
_CORDER				False		Define if you want to use the _CODE or _CCOPY or 
						_CTABLE control
_CODE(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _CODE control.
						Must be within #ifdef _CODE and #endif
_CCOPY(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _CCOPY control.
						Must be within #ifdef _CCOPY and #endif
_CTABLE( addr )			--		Specify absolute address of the copy table
						Must be within #ifdef _CTABLE and #endif
_HCORDER			False		Define if you want to use the _HCODE or _HCOPY or 
						_HTABLE control
_HCODE(name [addr=addr] )	--		Specify segment with name must be located at address 
						addr or before/after another _HCODE control.
						Must be within #ifdef _HCODE and #endif
_HTABLE( addr )			--		Specify absolute address of the copy table
						Must be within #ifdef _HTABLE and #endif
**************************************************************************/
