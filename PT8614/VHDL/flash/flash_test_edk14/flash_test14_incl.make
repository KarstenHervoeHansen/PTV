#################################################################
# Makefile generated by Xilinx Platform Studio 
# Project:C:\MyProject\flash_test_edk14\flash_test14.xmp
#################################################################

XILINX_EDK_DIR = C:/EDK

SYSTEM = flash_test14

MHSFILE = flash_test14.mhs

MSSFILE = flash_test14.mss

FPGA_ARCH = virtex2p

DEVICE = xc2vp20ff896-6

LANGUAGE = vhdl

SEARCHPATHOPT = 

SUBMODULE_OPT = 

PLATGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(SUBMODULE_OPT)

LIBGEN_OPTIONS = -mhs $(MHSFILE) -p $(DEVICE) $(SEARCHPATHOPT) \
                   $(MICROBLAZE_0_LIBG_OPT)

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

MICROBLAZE_0_BOOTLOOP = $(BOOTLOOP_DIR)/microblaze_0.elf
MICROBLAZE_0_XMDSTUB = microblaze_0/code/xmdstub.elf

BRAMINIT_ELF_FILES =  $(TEST_FLASH_OUTPUT) 
BRAMINIT_ELF_FILE_ARGS =   -pe microblaze_0 $(TEST_FLASH_OUTPUT) 

ALL_USER_ELF_FILES = $(TESTAPP_MEMORY_OUTPUT) $(TESTAPP_PERIPHERAL_OUTPUT) $(TEST_FLASH_OUTPUT) 

SIM_CMD = vsim

BEHAVIORAL_SIM_SCRIPT = simulation/behavioral/$(SYSTEM).do

STRUCTURAL_SIM_SCRIPT = simulation/structural/$(SYSTEM).do

TIMING_SIM_SCRIPT = simulation/timing/$(SYSTEM).do

DEFAULT_SIM_SCRIPT = $(BEHAVIORAL_SIM_SCRIPT)

MIX_LANG_SIM_OPT = -mixed yes

SIMGEN_OPTIONS = -p $(DEVICE) -lang $(LANGUAGE) $(SEARCHPATHOPT) $(SUBMODULE_OPT) $(BRAMINIT_ELF_FILE_ARGS) $(MIX_LANG_SIM_OPT)  -s mti

MICROBLAZE_0_XMDSTUB = microblaze_0/code/xmdstub.elf

LIBRARIES =  \
       microblaze_0/lib/libxil.a 
VPEXEC = virtualplatform/vpexec.exe

LIBSCLEAN_TARGETS = microblaze_0_libsclean 

PROGRAMCLEAN_TARGETS = TestApp_Memory_programclean TestApp_Peripheral_programclean Test_Flash_programclean 

CORE_STATE_DEVELOPMENT_FILES = 

WRAPPER_NGC_FILES = implementation/microblaze_0_wrapper.ngc \
implementation/mb_opb_wrapper.ngc \
implementation/debug_module_wrapper.ngc \
implementation/ilmb_wrapper.ngc \
implementation/dlmb_wrapper.ngc \
implementation/dlmb_cntlr_wrapper.ngc \
implementation/ilmb_cntlr_wrapper.ngc \
implementation/lmb_bram_wrapper.ngc \
implementation/generic_external_memory_wrapper.ngc \
implementation/rs232_wrapper.ngc \
implementation/opb_bram_if_cntlr_1_wrapper.ngc \
implementation/opb_bram_if_cntlr_1_bram_wrapper.ngc \
implementation/dcm_0_wrapper.ngc \
implementation/util_bus_split_0_wrapper.ngc

POSTSYN_NETLIST = implementation/$(SYSTEM).ngc

SYSTEM_BIT = implementation/$(SYSTEM).bit

DOWNLOAD_BIT = implementation/download.bit

SYSTEM_ACE = implementation/$(SYSTEM).ace

UCF_FILE = data/flash_test14.ucf

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

TESTAPP_MEMORY_CC = mb-gcc
TESTAPP_MEMORY_CC_SIZE = mb-size
TESTAPP_MEMORY_CC_OPT = -O2
TESTAPP_MEMORY_CFLAGS = 
TESTAPP_MEMORY_CC_SEARCH = # -B
TESTAPP_MEMORY_LIBPATH = -L./microblaze_0/lib/ # -L
TESTAPP_MEMORY_INCLUDES = -I./microblaze_0/include/ # -I
TESTAPP_MEMORY_LFLAGS = # -l
TESTAPP_MEMORY_CC_PREPROC_FLAG = # -Wp,
TESTAPP_MEMORY_CC_ASM_FLAG = # -Wa,
TESTAPP_MEMORY_CC_LINKER_FLAG = # -Wl,
TESTAPP_MEMORY_LINKER_SCRIPT = TestApp_Memory/src/TestApp_Memory_LinkScr.ld
TESTAPP_MEMORY_LINKER_SCRIPT_FLAG = -Wl,-T -Wl,$(TESTAPP_MEMORY_LINKER_SCRIPT) 
TESTAPP_MEMORY_CC_DEBUG_FLAG =  -g 
TESTAPP_MEMORY_CC_PROFILE_FLAG = # -pg
TESTAPP_MEMORY_CC_GLOBPTR_FLAG= # -mxl-gp-opt
TESTAPP_MEMORY_MODE = executable
TESTAPP_MEMORY_LIBG_OPT = -$(TESTAPP_MEMORY_MODE) microblaze_0
TESTAPP_MEMORY_CC_SOFTMUL_FLAG= -mno-xl-soft-mul 
TESTAPP_MEMORY_CC_START_ADDR_FLAG=  # -Wl,-defsym -Wl,_TEXT_START_ADDR=
TESTAPP_MEMORY_CC_STACK_SIZE_FLAG=  # -Wl,-defsym -Wl,_STACK_SIZE=
TESTAPP_MEMORY_CC_HEAP_SIZE_FLAG=  # -Wl,-defsym -Wl,_HEAP_SIZE=
TESTAPP_MEMORY_OTHER_CC_FLAGS= $(TESTAPP_MEMORY_CC_GLOBPTR_FLAG)  \
                  $(TESTAPP_MEMORY_CC_START_ADDR_FLAG) $(TESTAPP_MEMORY_CC_STACK_SIZE_FLAG) $(TESTAPP_MEMORY_CC_HEAP_SIZE_FLAG)  \
                  $(TESTAPP_MEMORY_CC_SOFTMUL_FLAG)  \
                  $(TESTAPP_MEMORY_CC_PREPROC_FLAG) $(TESTAPP_MEMORY_CC_ASM_FLAG) $(TESTAPP_MEMORY_CC_LINKER_FLAG)  \
                  $(TESTAPP_MEMORY_LINKER_SCRIPT_FLAG) $(TESTAPP_MEMORY_CC_DEBUG_FLAG) $(TESTAPP_MEMORY_CC_PROFILE_FLAG) 

#################################################################
# SOFTWARE APPLICATION TESTAPP_PERIPHERAL
#################################################################

TESTAPP_PERIPHERAL_SOURCES = TestApp_Peripheral/src/TestApp_Peripheral.c TestApp_Peripheral/src/xuartlite_selftest_example.c 

TESTAPP_PERIPHERAL_HEADERS = TestApp_Peripheral/src/uartlite_header.h 

TESTAPP_PERIPHERAL_CC = mb-gcc
TESTAPP_PERIPHERAL_CC_SIZE = mb-size
TESTAPP_PERIPHERAL_CC_OPT = -O2
TESTAPP_PERIPHERAL_CFLAGS = 
TESTAPP_PERIPHERAL_CC_SEARCH = # -B
TESTAPP_PERIPHERAL_LIBPATH = -L./microblaze_0/lib/ # -L
TESTAPP_PERIPHERAL_INCLUDES = -I./microblaze_0/include/  -ITestApp_Peripheral/src/ # -I
TESTAPP_PERIPHERAL_LFLAGS = # -l
TESTAPP_PERIPHERAL_CC_PREPROC_FLAG = # -Wp,
TESTAPP_PERIPHERAL_CC_ASM_FLAG = # -Wa,
TESTAPP_PERIPHERAL_CC_LINKER_FLAG = # -Wl,
TESTAPP_PERIPHERAL_LINKER_SCRIPT = TestApp_Peripheral/src/TestApp_Peripheral_LinkScr.ld
TESTAPP_PERIPHERAL_LINKER_SCRIPT_FLAG = -Wl,-T -Wl,$(TESTAPP_PERIPHERAL_LINKER_SCRIPT) 
TESTAPP_PERIPHERAL_CC_DEBUG_FLAG =  -g 
TESTAPP_PERIPHERAL_CC_PROFILE_FLAG = # -pg
TESTAPP_PERIPHERAL_CC_GLOBPTR_FLAG= # -mxl-gp-opt
TESTAPP_PERIPHERAL_MODE = executable
TESTAPP_PERIPHERAL_LIBG_OPT = -$(TESTAPP_PERIPHERAL_MODE) microblaze_0
TESTAPP_PERIPHERAL_CC_SOFTMUL_FLAG= -mno-xl-soft-mul 
TESTAPP_PERIPHERAL_CC_START_ADDR_FLAG=  # -Wl,-defsym -Wl,_TEXT_START_ADDR=
TESTAPP_PERIPHERAL_CC_STACK_SIZE_FLAG=  # -Wl,-defsym -Wl,_STACK_SIZE=
TESTAPP_PERIPHERAL_CC_HEAP_SIZE_FLAG=  # -Wl,-defsym -Wl,_HEAP_SIZE=
TESTAPP_PERIPHERAL_OTHER_CC_FLAGS= $(TESTAPP_PERIPHERAL_CC_GLOBPTR_FLAG)  \
                  $(TESTAPP_PERIPHERAL_CC_START_ADDR_FLAG) $(TESTAPP_PERIPHERAL_CC_STACK_SIZE_FLAG) $(TESTAPP_PERIPHERAL_CC_HEAP_SIZE_FLAG)  \
                  $(TESTAPP_PERIPHERAL_CC_SOFTMUL_FLAG)  \
                  $(TESTAPP_PERIPHERAL_CC_PREPROC_FLAG) $(TESTAPP_PERIPHERAL_CC_ASM_FLAG) $(TESTAPP_PERIPHERAL_CC_LINKER_FLAG)  \
                  $(TESTAPP_PERIPHERAL_LINKER_SCRIPT_FLAG) $(TESTAPP_PERIPHERAL_CC_DEBUG_FLAG) $(TESTAPP_PERIPHERAL_CC_PROFILE_FLAG) 

#################################################################
# SOFTWARE APPLICATION TEST_FLASH
#################################################################

TEST_FLASH_SOURCES = Test_Flash/flash_test.c Test_Flash/xflash_intel_l.c Test_Flash/xuartns550_l.c 

TEST_FLASH_HEADERS = Test_Flash/xflash_intel_l.h Test_Flash/xuartns550_l.h 

TEST_FLASH_CC = mb-gcc
TEST_FLASH_CC_SIZE = mb-size
TEST_FLASH_CC_OPT = -O2
TEST_FLASH_CFLAGS = 
TEST_FLASH_CC_SEARCH = # -B
TEST_FLASH_LIBPATH = -L./microblaze_0/lib/ # -L
TEST_FLASH_INCLUDES = -I./microblaze_0/include/  -ITest_Flash/ # -I
TEST_FLASH_LFLAGS = # -l
TEST_FLASH_CC_PREPROC_FLAG = # -Wp,
TEST_FLASH_CC_ASM_FLAG = # -Wa,
TEST_FLASH_CC_LINKER_FLAG = # -Wl,
TEST_FLASH_LINKER_SCRIPT = 
TEST_FLASH_LINKER_SCRIPT_FLAG = #-Wl,-T -Wl,$(TEST_FLASH_LINKER_SCRIPT) 
TEST_FLASH_CC_DEBUG_FLAG =  -g 
TEST_FLASH_CC_PROFILE_FLAG = # -pg
TEST_FLASH_CC_GLOBPTR_FLAG= # -mxl-gp-opt
TEST_FLASH_MODE = executable
TEST_FLASH_LIBG_OPT = -$(TEST_FLASH_MODE) microblaze_0
TEST_FLASH_CC_SOFTMUL_FLAG= -mno-xl-soft-mul 
TEST_FLASH_CC_START_ADDR_FLAG=  # -Wl,-defsym -Wl,_TEXT_START_ADDR=
TEST_FLASH_CC_STACK_SIZE_FLAG=  # -Wl,-defsym -Wl,_STACK_SIZE=
TEST_FLASH_CC_HEAP_SIZE_FLAG=  # -Wl,-defsym -Wl,_HEAP_SIZE=
TEST_FLASH_OTHER_CC_FLAGS= $(TEST_FLASH_CC_GLOBPTR_FLAG)  \
                  $(TEST_FLASH_CC_START_ADDR_FLAG) $(TEST_FLASH_CC_STACK_SIZE_FLAG) $(TEST_FLASH_CC_HEAP_SIZE_FLAG)  \
                  $(TEST_FLASH_CC_SOFTMUL_FLAG)  \
                  $(TEST_FLASH_CC_PREPROC_FLAG) $(TEST_FLASH_CC_ASM_FLAG) $(TEST_FLASH_CC_LINKER_FLAG)  \
                  $(TEST_FLASH_LINKER_SCRIPT_FLAG) $(TEST_FLASH_CC_DEBUG_FLAG) $(TEST_FLASH_CC_PROFILE_FLAG) 
