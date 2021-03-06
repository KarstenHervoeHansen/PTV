#################################################################
# Makefile generated by Xilinx Platform Studio 
# Project:C:\MyProject\PT8614_flash\system.xmp
#################################################################

XILINX_EDK_DIR = C:/EDK

SYSTEM = system

MHSFILE = system.mhs

MSSFILE = system.mss

FPGA_ARCH = virtex2p

DEVICE = xc2vp20ff896-6

LANGUAGE = vhdl

SEARCHPATHOPT = 

SUBMODULE_OPT = 

PLATGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(SUBMODULE_OPT)

LIBGEN_OPTIONS = -mhs $(MHSFILE) -p $(DEVICE) $(SEARCHPATHOPT)

VPGEN_OPTIONS = -p $(DEVICE) $(SEARCHPATHOPT)

TESTAPP_MEMORY_OUTPUT_DIR = TestApp_Memory
TESTAPP_MEMORY_OUTPUT = $(TESTAPP_MEMORY_OUTPUT_DIR)/executable.elf

TESTAPP_PERIPHERAL_OUTPUT_DIR = TestApp_Peripheral
TESTAPP_PERIPHERAL_OUTPUT = $(TESTAPP_PERIPHERAL_OUTPUT_DIR)/executable.elf

TEST_FLASH_OUTPUT_DIR = Test_Flash
TEST_FLASH_OUTPUT = $(TEST_FLASH_OUTPUT_DIR)/executable.elf

MICROBLAZE_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/microblaze/mb_bootloop.elf
PPC405_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc405/ppc_bootloop.elf
PPC440_BOOTLOOP = $(XILINX_EDK_DIR)/sw/lib/ppc440/ppc440_bootloop.elf
BOOTLOOP_DIR = bootloops

PPC405_0_BOOTLOOP = $(BOOTLOOP_DIR)/ppc405_0.elf

PPC405_1_BOOTLOOP = $(BOOTLOOP_DIR)/ppc405_1.elf

BRAMINIT_ELF_FILES =  $(TEST_FLASH_OUTPUT) 
BRAMINIT_ELF_FILE_ARGS =   -pe ppc405_0 $(TEST_FLASH_OUTPUT) 

ALL_USER_ELF_FILES = $(TESTAPP_MEMORY_OUTPUT) $(TESTAPP_PERIPHERAL_OUTPUT) $(TEST_FLASH_OUTPUT) 

SIM_CMD = vsim

BEHAVIORAL_SIM_SCRIPT = simulation/behavioral/$(SYSTEM).do

STRUCTURAL_SIM_SCRIPT = simulation/structural/$(SYSTEM).do

TIMING_SIM_SCRIPT = simulation/timing/$(SYSTEM).do

DEFAULT_SIM_SCRIPT = $(BEHAVIORAL_SIM_SCRIPT)

MIX_LANG_SIM_OPT = -mixed yes

SIMGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(SUBMODULE_OPT) $(BRAMINIT_ELF_FILE_ARGS) $(MIX_LANG_SIM_OPT)  -s mti -X C:/MyProject/flash_test_edk12/sim/edk/ -E C:/MyProject/flash_test_edk12/sim/EDK_Lib/


LIBRARIES =  \
       ppc405_0/lib/libxil.a  \
       ppc405_1/lib/libxil.a 
VPEXEC = virtualplatform/vpexec.exe

LIBSCLEAN_TARGETS = ppc405_0_libsclean ppc405_1_libsclean 

PROGRAMCLEAN_TARGETS = TestApp_Memory_programclean TestApp_Peripheral_programclean Test_Flash_programclean 

CORE_STATE_DEVELOPMENT_FILES = 

WRAPPER_NGC_FILES = implementation/ppc405_0_wrapper.ngc \
implementation/ppc405_1_wrapper.ngc \
implementation/jtagppc_0_wrapper.ngc \
implementation/reset_block_wrapper.ngc \
implementation/iocm_wrapper.ngc \
implementation/iocm_cntlr_wrapper.ngc \
implementation/isocm_bram_wrapper.ngc \
implementation/docm_wrapper.ngc \
implementation/docm_cntlr_wrapper.ngc \
implementation/dsocm_bram_wrapper.ngc \
implementation/plb_wrapper.ngc \
implementation/generic_external_memory_wrapper.ngc \
implementation/opb_wrapper.ngc \
implementation/plb2opb_wrapper.ngc \
implementation/generic_gpio_wrapper.ngc \
implementation/leds_wrapper.ngc \
implementation/rs232_wrapper.ngc \
implementation/plb_bram_if_cntlr_1_wrapper.ngc \
implementation/plb_bram_if_cntlr_1_bram_wrapper.ngc \
implementation/dcm_0_wrapper.ngc \
implementation/util_bus_split_0_wrapper.ngc

POSTSYN_NETLIST = implementation/$(SYSTEM).ngc

SYSTEM_BIT = implementation/$(SYSTEM).bit

DOWNLOAD_BIT = implementation/download.bit

SYSTEM_ACE = implementation/$(SYSTEM).ace

UCF_FILE = data/system.ucf

BMM_FILE = implementation/$(SYSTEM).bmm

BITGEN_UT_FILE = etc/bitgen.ut

XFLOW_OPT_FILE = etc/fast_runtime.opt
XFLOW_DEPENDENCY = __xps/xpsxflow.opt $(XFLOW_OPT_FILE)

FPGA_IMP_DEPENDENCY = $(BMM_FILE) $(POSTSYN_NETLIST) $(UCF_FILE) $(BITGEN_UT_FILE) $(XFLOW_DEPENDENCY)

#################################################################
# SOFTWARE APPLICATION TESTAPP_MEMORY
#################################################################

TESTAPP_MEMORY_SOURCES = TestApp_Memory/src/TestApp_Memory.c 

TESTAPP_MEMORY_HEADERS = 

TESTAPP_MEMORY_CC = powerpc-eabi-gcc
TESTAPP_MEMORY_CC_SIZE = powerpc-eabi-size
TESTAPP_MEMORY_CC_OPT = -O2
TESTAPP_MEMORY_CFLAGS = 
TESTAPP_MEMORY_CC_SEARCH = # -B
TESTAPP_MEMORY_LIBPATH = -L./ppc405_0/lib/ # -L
TESTAPP_MEMORY_INCLUDES = -I./ppc405_0/include/ # -I
TESTAPP_MEMORY_LFLAGS = # -l
TESTAPP_MEMORY_CC_PREPROC_FLAG = # -Wp,
TESTAPP_MEMORY_CC_ASM_FLAG = # -Wa,
TESTAPP_MEMORY_CC_LINKER_FLAG = # -Wl,
TESTAPP_MEMORY_LINKER_SCRIPT = TestApp_Memory/src/TestApp_Memory_LinkScr.ld
TESTAPP_MEMORY_LINKER_SCRIPT_FLAG = -Wl,-T -Wl,$(TESTAPP_MEMORY_LINKER_SCRIPT) 
TESTAPP_MEMORY_CC_DEBUG_FLAG =  -g 
TESTAPP_MEMORY_CC_PROFILE_FLAG = # -pg
TESTAPP_MEMORY_CC_GLOBPTR_FLAG= # -msdata=eabi
TESTAPP_MEMORY_CC_START_ADDR_FLAG=  # -Wl,-defsym -Wl,_START_ADDR=
TESTAPP_MEMORY_CC_STACK_SIZE_FLAG=  # -Wl,-defsym -Wl,_STACK_SIZE=
TESTAPP_MEMORY_CC_HEAP_SIZE_FLAG=  # -Wl,-defsym -Wl,_HEAP_SIZE=
TESTAPP_MEMORY_OTHER_CC_FLAGS= $(TESTAPP_MEMORY_CC_GLOBPTR_FLAG)  \
                  $(TESTAPP_MEMORY_CC_START_ADDR_FLAG) $(TESTAPP_MEMORY_CC_STACK_SIZE_FLAG) $(TESTAPP_MEMORY_CC_HEAP_SIZE_FLAG)  \
                  $(TESTAPP_MEMORY_CC_PREPROC_FLAG) $(TESTAPP_MEMORY_CC_ASM_FLAG) $(TESTAPP_MEMORY_CC_LINKER_FLAG)  \
                  $(TESTAPP_MEMORY_LINKER_SCRIPT_FLAG) $(TESTAPP_MEMORY_CC_DEBUG_FLAG) $(TESTAPP_MEMORY_CC_PROFILE_FLAG) 

#################################################################
# SOFTWARE APPLICATION TESTAPP_PERIPHERAL
#################################################################

TESTAPP_PERIPHERAL_SOURCES = TestApp_Peripheral/src/TestApp_Peripheral.c TestApp_Peripheral/src/xgpio_tapp_example.c 

TESTAPP_PERIPHERAL_HEADERS = TestApp_Peripheral/src/gpio_header.h 

TESTAPP_PERIPHERAL_CC = powerpc-eabi-gcc
TESTAPP_PERIPHERAL_CC_SIZE = powerpc-eabi-size
TESTAPP_PERIPHERAL_CC_OPT = -O2
TESTAPP_PERIPHERAL_CFLAGS = 
TESTAPP_PERIPHERAL_CC_SEARCH = # -B
TESTAPP_PERIPHERAL_LIBPATH = -L./ppc405_0/lib/ # -L
TESTAPP_PERIPHERAL_INCLUDES = -I./ppc405_0/include/  -ITestApp_Peripheral/src/ # -I
TESTAPP_PERIPHERAL_LFLAGS = # -l
TESTAPP_PERIPHERAL_CC_PREPROC_FLAG = # -Wp,
TESTAPP_PERIPHERAL_CC_ASM_FLAG = # -Wa,
TESTAPP_PERIPHERAL_CC_LINKER_FLAG = # -Wl,
TESTAPP_PERIPHERAL_LINKER_SCRIPT = TestApp_Peripheral/src/TestApp_Peripheral_LinkScr.ld
TESTAPP_PERIPHERAL_LINKER_SCRIPT_FLAG = -Wl,-T -Wl,$(TESTAPP_PERIPHERAL_LINKER_SCRIPT) 
TESTAPP_PERIPHERAL_CC_DEBUG_FLAG =  -g 
TESTAPP_PERIPHERAL_CC_PROFILE_FLAG = # -pg
TESTAPP_PERIPHERAL_CC_GLOBPTR_FLAG= # -msdata=eabi
TESTAPP_PERIPHERAL_CC_START_ADDR_FLAG=  # -Wl,-defsym -Wl,_START_ADDR=
TESTAPP_PERIPHERAL_CC_STACK_SIZE_FLAG=  # -Wl,-defsym -Wl,_STACK_SIZE=
TESTAPP_PERIPHERAL_CC_HEAP_SIZE_FLAG=  # -Wl,-defsym -Wl,_HEAP_SIZE=
TESTAPP_PERIPHERAL_OTHER_CC_FLAGS= $(TESTAPP_PERIPHERAL_CC_GLOBPTR_FLAG)  \
                  $(TESTAPP_PERIPHERAL_CC_START_ADDR_FLAG) $(TESTAPP_PERIPHERAL_CC_STACK_SIZE_FLAG) $(TESTAPP_PERIPHERAL_CC_HEAP_SIZE_FLAG)  \
                  $(TESTAPP_PERIPHERAL_CC_PREPROC_FLAG) $(TESTAPP_PERIPHERAL_CC_ASM_FLAG) $(TESTAPP_PERIPHERAL_CC_LINKER_FLAG)  \
                  $(TESTAPP_PERIPHERAL_LINKER_SCRIPT_FLAG) $(TESTAPP_PERIPHERAL_CC_DEBUG_FLAG) $(TESTAPP_PERIPHERAL_CC_PROFILE_FLAG) 

#################################################################
# SOFTWARE APPLICATION TEST_FLASH
#################################################################

TEST_FLASH_SOURCES = TestFlash/flash_test.c TestFlash/xflash_intel_l.c TestFlash/xuartns550_l.c 

TEST_FLASH_HEADERS = TestFlash/ppc405_cache_init.h TestFlash/xflash_intel_l.h TestFlash/xgpio_l.h TestFlash/xuartns550_l.h 

TEST_FLASH_CC = powerpc-eabi-gcc
TEST_FLASH_CC_SIZE = powerpc-eabi-size
TEST_FLASH_CC_OPT = -O2
TEST_FLASH_CFLAGS = 
TEST_FLASH_CC_SEARCH = # -B
TEST_FLASH_LIBPATH = -L./ppc405_0/lib/ # -L
TEST_FLASH_INCLUDES = -I./ppc405_0/include/  -ITestFlash/ # -I
TEST_FLASH_LFLAGS = # -l
TEST_FLASH_CC_PREPROC_FLAG = # -Wp,
TEST_FLASH_CC_ASM_FLAG = # -Wa,
TEST_FLASH_CC_LINKER_FLAG = # -Wl,
TEST_FLASH_LINKER_SCRIPT = TestApp_Peripheral/src/TestApp_Peripheral_LinkScr.ld
TEST_FLASH_LINKER_SCRIPT_FLAG = -Wl,-T -Wl,$(TEST_FLASH_LINKER_SCRIPT) 
TEST_FLASH_CC_DEBUG_FLAG =  -g 
TEST_FLASH_CC_PROFILE_FLAG = # -pg
TEST_FLASH_CC_GLOBPTR_FLAG= # -msdata=eabi
TEST_FLASH_CC_START_ADDR_FLAG=  # -Wl,-defsym -Wl,_START_ADDR=
TEST_FLASH_CC_STACK_SIZE_FLAG=  # -Wl,-defsym -Wl,_STACK_SIZE=
TEST_FLASH_CC_HEAP_SIZE_FLAG=  # -Wl,-defsym -Wl,_HEAP_SIZE=
TEST_FLASH_OTHER_CC_FLAGS= $(TEST_FLASH_CC_GLOBPTR_FLAG)  \
                  $(TEST_FLASH_CC_START_ADDR_FLAG) $(TEST_FLASH_CC_STACK_SIZE_FLAG) $(TEST_FLASH_CC_HEAP_SIZE_FLAG)  \
                  $(TEST_FLASH_CC_PREPROC_FLAG) $(TEST_FLASH_CC_ASM_FLAG) $(TEST_FLASH_CC_LINKER_FLAG)  \
                  $(TEST_FLASH_LINKER_SCRIPT_FLAG) $(TEST_FLASH_CC_DEBUG_FLAG) $(TEST_FLASH_CC_PROFILE_FLAG) 
