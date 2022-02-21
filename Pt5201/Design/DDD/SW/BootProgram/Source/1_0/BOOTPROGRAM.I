#define _ICODE 0x0
#define _IRAM 0x200
#define _REGSFR regxag3.dat
#define _XMEM 1
#define _MODEL_LARGE
#define _ZPAGE 0
#define _XVWBUF 0
#ifdef _X_CODE_ROM
_X_CODE_ROM(x_code0,_ICODE,07FFFFH)
#endif
#ifdef _X_DATA_RAM
_X_DATA_RAM(x_ram_data0,0Ea014h,0Ea016h)
#endif
#define _DRESERVES
#ifdef _DRESERVED
_DRESERVED(0Ea014h,0Ea016h)
#endif
#ifdef _X_DATA_ROM
_X_DATA_ROM(x_rom_data0,08000H,0BFFDH)
#endif
#define _STACK_SZ 128
#define _STACK_LC 0180H
#define _HEAP_SZ 0
#define _RESET_VECTOR 0
