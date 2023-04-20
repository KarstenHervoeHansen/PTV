#define _ICODE 0x0
#define _IRAM 0x200
#define _UNIFIED_BUS 1
#define _REGSFR regxag3.dat
#define _XMEM 1
#define _MODEL_LARGE
#define _ZPAGE 0
#define _XVWBUF 0
#define _STACK_SZ 1k
#define _HEAP_SZ 2k
#define _RESET_VECTOR 0
#ifdef _X_CODE_ROM
_X_CODE_ROM(x_code0,_ICODE,07FFFH)
#endif
#ifdef _X_DATA_RAM
_X_DATA_RAM(x_ram_data0,0FFH,07FFFH)
#endif
