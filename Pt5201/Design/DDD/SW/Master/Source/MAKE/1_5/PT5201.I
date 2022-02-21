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
_X_DATA_RAM(x_ram_data0,0,07FFFFH)
_X_DATA_RAM(x_ram_data1,080000H,08FFFFH)
#endif
#define _DRESERVES
#ifdef _DRESERVED
_DRESERVED(040000H,7FFFFH)
_DRESERVED(080000H,08FFFFH)
#endif
#define _STACK_SZ 1k
#define _STACK_LC 8000H
#define _HEAP_SZ 30000H
#define _HEAP_LC 10000H
#define _RESET_VECTOR 0
