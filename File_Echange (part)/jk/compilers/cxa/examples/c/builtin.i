#define _ICODE 0x8000
#define _IRAM 0x200
#define _REGSFR regxag3.dat
#define _XMEM 1
#define _MODEL_SMALL
#define _ZPAGE 1
#ifdef _X_CODE_ROM
_X_CODE_ROM(x_code0,_ICODE,0FFFFH)
#endif
#ifdef _X_DATA_RAM
_X_DATA_RAM(x_ram_data0,0,0FFFFH)
#endif
#define _STACK_SZ 1k
#define _HEAP_SZ 2k
#define _RESET_VECTOR 0
