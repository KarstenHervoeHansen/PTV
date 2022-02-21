#define _ICODE 0x0
#define _IRAM 0x200
#define _REGSFR regxag3.dat
#define _XMEM 1
#define _MODEL_LARGE
#define _ZPAGE 0
#define _XVWBUF 0
#ifdef _X_CODE_ROM
_X_CODE_ROM(x_code0,_ICODE,0100H)
_X_CODE_ROM(x_code1,10000H,02FFFFH)
#endif
#ifdef _X_DATA_RAM
_X_DATA_RAM(x_ram_data0,010000H,02FFFFH)
_X_DATA_RAM(x_ram_data1,080000H,083FFFH)
_X_DATA_RAM(x_ram_data2,090006H,090008H)
#endif
#define _DRESERVES
#ifdef _DRESERVED
_DRESERVED(010000H,02FFFFH)
#endif
#ifdef _X_DATA_ROM
_X_DATA_ROM(x_rom_data0,030000H,07FFFFH)
#endif
#define _STACK_SZ 128
#define _STACK_LC 10080H
#define _RESET_VECTOR 0
