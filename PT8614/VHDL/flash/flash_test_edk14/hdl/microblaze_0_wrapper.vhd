-------------------------------------------------------------------------------
-- microblaze_0_wrapper.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

library microblaze_v4_00_a;
use microblaze_v4_00_a.All;

entity microblaze_0_wrapper is
  port (
    CLK : in std_logic;
    RESET : in std_logic;
    INTERRUPT : in std_logic;
    DEBUG_RST : in std_logic;
    EXT_BRK : in std_logic;
    EXT_NM_BRK : in std_logic;
    DBG_STOP : in std_logic;
    INSTR : in std_logic_vector(0 to 31);
    I_ADDRTAG : out std_logic_vector(0 to 3);
    IREADY : in std_logic;
    IWAIT : in std_logic;
    INSTR_ADDR : out std_logic_vector(0 to 31);
    IFETCH : out std_logic;
    I_AS : out std_logic;
    DATA_READ : in std_logic_vector(0 to 31);
    DREADY : in std_logic;
    DWAIT : in std_logic;
    DATA_WRITE : out std_logic_vector(0 to 31);
    DATA_ADDR : out std_logic_vector(0 to 31);
    D_ADDRTAG : out std_logic_vector(0 to 3);
    D_AS : out std_logic;
    READ_STROBE : out std_logic;
    WRITE_STROBE : out std_logic;
    BYTE_ENABLE : out std_logic_vector(0 to 3);
    DM_ABUS : out std_logic_vector(0 to 31);
    DM_BE : out std_logic_vector(0 to 3);
    DM_BUSLOCK : out std_logic;
    DM_DBUS : out std_logic_vector(0 to 31);
    DM_REQUEST : out std_logic;
    DM_RNW : out std_logic;
    DM_SELECT : out std_logic;
    DM_SEQADDR : out std_logic;
    DOPB_DBUS : in std_logic_vector(0 to 31);
    DOPB_ERRACK : in std_logic;
    DOPB_MGRANT : in std_logic;
    DOPB_RETRY : in std_logic;
    DOPB_TIMEOUT : in std_logic;
    DOPB_XFERACK : in std_logic;
    IM_ABUS : out std_logic_vector(0 to 31);
    IM_BE : out std_logic_vector(0 to 3);
    IM_BUSLOCK : out std_logic;
    IM_DBUS : out std_logic_vector(0 to 31);
    IM_REQUEST : out std_logic;
    IM_RNW : out std_logic;
    IM_SELECT : out std_logic;
    IM_SEQADDR : out std_logic;
    IOPB_DBUS : in std_logic_vector(0 to 31);
    IOPB_ERRACK : in std_logic;
    IOPB_MGRANT : in std_logic;
    IOPB_RETRY : in std_logic;
    IOPB_TIMEOUT : in std_logic;
    IOPB_XFERACK : in std_logic;
    DBG_CLK : in std_logic;
    DBG_TDI : in std_logic;
    DBG_TDO : out std_logic;
    DBG_REG_EN : in std_logic_vector(0 to 4);
    DBG_CAPTURE : in std_logic;
    DBG_UPDATE : in std_logic;
    VALID_INSTR : out std_logic;
    PC_EX : out std_logic_vector(0 to 31);
    REG_WRITE : out std_logic;
    REG_ADDR : out std_logic_vector(0 to 4);
    MSR_REG : out std_logic_vector(0 to 9);
    NEW_REG_VALUE : out std_logic_vector(0 to 31);
    PIPE_RUNNING : out std_logic;
    INTERRUPT_TAKEN : out std_logic;
    JUMP_TAKEN : out std_logic;
    PREFETCH_ADDR : out std_logic_vector(0 to 3);
    MB_Halted : out std_logic;
    Trace_Branch_Instr : out std_logic;
    Trace_Delay_Slot : out std_logic;
    Trace_Data_Address : out std_logic_vector(0 to 31);
    Trace_AS : out std_logic;
    Trace_Data_Read : out std_logic;
    Trace_Data_Write : out std_logic;
    Trace_DCache_Req : out std_logic;
    Trace_DCache_Hit : out std_logic;
    Trace_ICache_Req : out std_logic;
    Trace_ICache_Hit : out std_logic;
    Trace_Instr_EX : out std_logic_vector(0 to 31);
    FSL0_S_CLK : out std_logic;
    FSL0_S_READ : out std_logic;
    FSL0_S_DATA : in std_logic_vector(0 to 31);
    FSL0_S_CONTROL : in std_logic;
    FSL0_S_EXISTS : in std_logic;
    FSL0_M_CLK : out std_logic;
    FSL0_M_WRITE : out std_logic;
    FSL0_M_DATA : out std_logic_vector(0 to 31);
    FSL0_M_CONTROL : out std_logic;
    FSL0_M_FULL : in std_logic;
    FSL1_S_CLK : out std_logic;
    FSL1_S_READ : out std_logic;
    FSL1_S_DATA : in std_logic_vector(0 to 31);
    FSL1_S_CONTROL : in std_logic;
    FSL1_S_EXISTS : in std_logic;
    FSL1_M_CLK : out std_logic;
    FSL1_M_WRITE : out std_logic;
    FSL1_M_DATA : out std_logic_vector(0 to 31);
    FSL1_M_CONTROL : out std_logic;
    FSL1_M_FULL : in std_logic;
    FSL2_S_CLK : out std_logic;
    FSL2_S_READ : out std_logic;
    FSL2_S_DATA : in std_logic_vector(0 to 31);
    FSL2_S_CONTROL : in std_logic;
    FSL2_S_EXISTS : in std_logic;
    FSL2_M_CLK : out std_logic;
    FSL2_M_WRITE : out std_logic;
    FSL2_M_DATA : out std_logic_vector(0 to 31);
    FSL2_M_CONTROL : out std_logic;
    FSL2_M_FULL : in std_logic;
    FSL3_S_CLK : out std_logic;
    FSL3_S_READ : out std_logic;
    FSL3_S_DATA : in std_logic_vector(0 to 31);
    FSL3_S_CONTROL : in std_logic;
    FSL3_S_EXISTS : in std_logic;
    FSL3_M_CLK : out std_logic;
    FSL3_M_WRITE : out std_logic;
    FSL3_M_DATA : out std_logic_vector(0 to 31);
    FSL3_M_CONTROL : out std_logic;
    FSL3_M_FULL : in std_logic;
    FSL4_S_CLK : out std_logic;
    FSL4_S_READ : out std_logic;
    FSL4_S_DATA : in std_logic_vector(0 to 31);
    FSL4_S_CONTROL : in std_logic;
    FSL4_S_EXISTS : in std_logic;
    FSL4_M_CLK : out std_logic;
    FSL4_M_WRITE : out std_logic;
    FSL4_M_DATA : out std_logic_vector(0 to 31);
    FSL4_M_CONTROL : out std_logic;
    FSL4_M_FULL : in std_logic;
    FSL5_S_CLK : out std_logic;
    FSL5_S_READ : out std_logic;
    FSL5_S_DATA : in std_logic_vector(0 to 31);
    FSL5_S_CONTROL : in std_logic;
    FSL5_S_EXISTS : in std_logic;
    FSL5_M_CLK : out std_logic;
    FSL5_M_WRITE : out std_logic;
    FSL5_M_DATA : out std_logic_vector(0 to 31);
    FSL5_M_CONTROL : out std_logic;
    FSL5_M_FULL : in std_logic;
    FSL6_S_CLK : out std_logic;
    FSL6_S_READ : out std_logic;
    FSL6_S_DATA : in std_logic_vector(0 to 31);
    FSL6_S_CONTROL : in std_logic;
    FSL6_S_EXISTS : in std_logic;
    FSL6_M_CLK : out std_logic;
    FSL6_M_WRITE : out std_logic;
    FSL6_M_DATA : out std_logic_vector(0 to 31);
    FSL6_M_CONTROL : out std_logic;
    FSL6_M_FULL : in std_logic;
    FSL7_S_CLK : out std_logic;
    FSL7_S_READ : out std_logic;
    FSL7_S_DATA : in std_logic_vector(0 to 31);
    FSL7_S_CONTROL : in std_logic;
    FSL7_S_EXISTS : in std_logic;
    FSL7_M_CLK : out std_logic;
    FSL7_M_WRITE : out std_logic;
    FSL7_M_DATA : out std_logic_vector(0 to 31);
    FSL7_M_CONTROL : out std_logic;
    FSL7_M_FULL : in std_logic;
    ICACHE_FSL_IN_CLK : out std_logic;
    ICACHE_FSL_IN_READ : out std_logic;
    ICACHE_FSL_IN_DATA : in std_logic_vector(0 to 31);
    ICACHE_FSL_IN_CONTROL : in std_logic;
    ICACHE_FSL_IN_EXISTS : in std_logic;
    ICACHE_FSL_OUT_CLK : out std_logic;
    ICACHE_FSL_OUT_WRITE : out std_logic;
    ICACHE_FSL_OUT_DATA : out std_logic_vector(0 to 31);
    ICACHE_FSL_OUT_CONTROL : out std_logic;
    ICACHE_FSL_OUT_FULL : in std_logic;
    DCACHE_FSL_IN_CLK : out std_logic;
    DCACHE_FSL_IN_READ : out std_logic;
    DCACHE_FSL_IN_DATA : in std_logic_vector(0 to 31);
    DCACHE_FSL_IN_CONTROL : in std_logic;
    DCACHE_FSL_IN_EXISTS : in std_logic;
    DCACHE_FSL_OUT_CLK : out std_logic;
    DCACHE_FSL_OUT_WRITE : out std_logic;
    DCACHE_FSL_OUT_DATA : out std_logic_vector(0 to 31);
    DCACHE_FSL_OUT_CONTROL : out std_logic;
    DCACHE_FSL_OUT_FULL : in std_logic
  );
  attribute x_core_info : STRING;
  attribute x_core_info of microblaze_0_wrapper: entity is "microblaze_v4_00_a";

end microblaze_0_wrapper;

architecture STRUCTURE of microblaze_0_wrapper is

  component microblaze is
    generic (
      C_FAMILY : string;
      C_INSTANCE : string;
      C_D_OPB : integer;
      C_D_LMB : integer;
      C_I_OPB : integer;
      C_I_LMB : integer;
      C_USE_BARREL : integer;
      C_USE_DIV : integer;
      C_USE_HW_MUL : integer;
      C_USE_FPU : integer;
      C_USE_MSR_INSTR : integer;
      C_USE_PCMP_INSTR : integer;
      C_UNALIGNED_EXCEPTIONS : integer;
      C_ILL_OPCODE_EXCEPTION : integer;
      C_IOPB_BUS_EXCEPTION : integer;
      C_DOPB_BUS_EXCEPTION : integer;
      C_DIV_ZERO_EXCEPTION : integer;
      C_FPU_EXCEPTION : integer;
      C_DEBUG_ENABLED : integer;
      C_NUMBER_OF_PC_BRK : integer;
      C_NUMBER_OF_RD_ADDR_BRK : integer;
      C_NUMBER_OF_WR_ADDR_BRK : integer;
      C_INTERRUPT_IS_EDGE : integer;
      C_EDGE_IS_POSITIVE : integer;
      C_FSL_LINKS : integer;
      C_FSL_DATA_SIZE : integer;
      C_ICACHE_BASEADDR : std_logic_vector;
      C_ICACHE_HIGHADDR : std_logic_vector;
      C_USE_ICACHE : integer;
      C_ALLOW_ICACHE_WR : integer;
      C_ADDR_TAG_BITS : integer;
      C_CACHE_BYTE_SIZE : integer;
      C_ICACHE_USE_FSL : integer;
      C_DCACHE_BASEADDR : std_logic_vector;
      C_DCACHE_HIGHADDR : std_logic_vector;
      C_USE_DCACHE : integer;
      C_ALLOW_DCACHE_WR : integer;
      C_DCACHE_ADDR_TAG : integer;
      C_DCACHE_BYTE_SIZE : integer;
      C_DCACHE_USE_FSL : integer
    );
    port (
      CLK : in std_logic;
      RESET : in std_logic;
      INTERRUPT : in std_logic;
      DEBUG_RST : in std_logic;
      EXT_BRK : in std_logic;
      EXT_NM_BRK : in std_logic;
      DBG_STOP : in std_logic;
      INSTR : in std_logic_vector(0 to 31);
      I_ADDRTAG : out std_logic_vector(0 to 3);
      IREADY : in std_logic;
      IWAIT : in std_logic;
      INSTR_ADDR : out std_logic_vector(0 to 31);
      IFETCH : out std_logic;
      I_AS : out std_logic;
      DATA_READ : in std_logic_vector(0 to 31);
      DREADY : in std_logic;
      DWAIT : in std_logic;
      DATA_WRITE : out std_logic_vector(0 to 31);
      DATA_ADDR : out std_logic_vector(0 to 31);
      D_ADDRTAG : out std_logic_vector(0 to 3);
      D_AS : out std_logic;
      READ_STROBE : out std_logic;
      WRITE_STROBE : out std_logic;
      BYTE_ENABLE : out std_logic_vector(0 to 3);
      DM_ABUS : out std_logic_vector(0 to 31);
      DM_BE : out std_logic_vector(0 to 3);
      DM_BUSLOCK : out std_logic;
      DM_DBUS : out std_logic_vector(0 to 31);
      DM_REQUEST : out std_logic;
      DM_RNW : out std_logic;
      DM_SELECT : out std_logic;
      DM_SEQADDR : out std_logic;
      DOPB_DBUS : in std_logic_vector(0 to 31);
      DOPB_ERRACK : in std_logic;
      DOPB_MGRANT : in std_logic;
      DOPB_RETRY : in std_logic;
      DOPB_TIMEOUT : in std_logic;
      DOPB_XFERACK : in std_logic;
      IM_ABUS : out std_logic_vector(0 to 31);
      IM_BE : out std_logic_vector(0 to 3);
      IM_BUSLOCK : out std_logic;
      IM_DBUS : out std_logic_vector(0 to 31);
      IM_REQUEST : out std_logic;
      IM_RNW : out std_logic;
      IM_SELECT : out std_logic;
      IM_SEQADDR : out std_logic;
      IOPB_DBUS : in std_logic_vector(0 to 31);
      IOPB_ERRACK : in std_logic;
      IOPB_MGRANT : in std_logic;
      IOPB_RETRY : in std_logic;
      IOPB_TIMEOUT : in std_logic;
      IOPB_XFERACK : in std_logic;
      DBG_CLK : in std_logic;
      DBG_TDI : in std_logic;
      DBG_TDO : out std_logic;
      DBG_REG_EN : in std_logic_vector(0 to 4);
      DBG_CAPTURE : in std_logic;
      DBG_UPDATE : in std_logic;
      VALID_INSTR : out std_logic;
      PC_EX : out std_logic_vector(0 to 31);
      REG_WRITE : out std_logic;
      REG_ADDR : out std_logic_vector(0 to 4);
      MSR_REG : out std_logic_vector(0 to 9);
      NEW_REG_VALUE : out std_logic_vector(0 to 31);
      PIPE_RUNNING : out std_logic;
      INTERRUPT_TAKEN : out std_logic;
      JUMP_TAKEN : out std_logic;
      PREFETCH_ADDR : out std_logic_vector(0 to 3);
      MB_Halted : out std_logic;
      Trace_Branch_Instr : out std_logic;
      Trace_Delay_Slot : out std_logic;
      Trace_Data_Address : out std_logic_vector(0 to 31);
      Trace_AS : out std_logic;
      Trace_Data_Read : out std_logic;
      Trace_Data_Write : out std_logic;
      Trace_DCache_Req : out std_logic;
      Trace_DCache_Hit : out std_logic;
      Trace_ICache_Req : out std_logic;
      Trace_ICache_Hit : out std_logic;
      Trace_Instr_EX : out std_logic_vector(0 to 31);
      FSL0_S_CLK : out std_logic;
      FSL0_S_READ : out std_logic;
      FSL0_S_DATA : in std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL0_S_CONTROL : in std_logic;
      FSL0_S_EXISTS : in std_logic;
      FSL0_M_CLK : out std_logic;
      FSL0_M_WRITE : out std_logic;
      FSL0_M_DATA : out std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL0_M_CONTROL : out std_logic;
      FSL0_M_FULL : in std_logic;
      FSL1_S_CLK : out std_logic;
      FSL1_S_READ : out std_logic;
      FSL1_S_DATA : in std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL1_S_CONTROL : in std_logic;
      FSL1_S_EXISTS : in std_logic;
      FSL1_M_CLK : out std_logic;
      FSL1_M_WRITE : out std_logic;
      FSL1_M_DATA : out std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL1_M_CONTROL : out std_logic;
      FSL1_M_FULL : in std_logic;
      FSL2_S_CLK : out std_logic;
      FSL2_S_READ : out std_logic;
      FSL2_S_DATA : in std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL2_S_CONTROL : in std_logic;
      FSL2_S_EXISTS : in std_logic;
      FSL2_M_CLK : out std_logic;
      FSL2_M_WRITE : out std_logic;
      FSL2_M_DATA : out std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL2_M_CONTROL : out std_logic;
      FSL2_M_FULL : in std_logic;
      FSL3_S_CLK : out std_logic;
      FSL3_S_READ : out std_logic;
      FSL3_S_DATA : in std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL3_S_CONTROL : in std_logic;
      FSL3_S_EXISTS : in std_logic;
      FSL3_M_CLK : out std_logic;
      FSL3_M_WRITE : out std_logic;
      FSL3_M_DATA : out std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL3_M_CONTROL : out std_logic;
      FSL3_M_FULL : in std_logic;
      FSL4_S_CLK : out std_logic;
      FSL4_S_READ : out std_logic;
      FSL4_S_DATA : in std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL4_S_CONTROL : in std_logic;
      FSL4_S_EXISTS : in std_logic;
      FSL4_M_CLK : out std_logic;
      FSL4_M_WRITE : out std_logic;
      FSL4_M_DATA : out std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL4_M_CONTROL : out std_logic;
      FSL4_M_FULL : in std_logic;
      FSL5_S_CLK : out std_logic;
      FSL5_S_READ : out std_logic;
      FSL5_S_DATA : in std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL5_S_CONTROL : in std_logic;
      FSL5_S_EXISTS : in std_logic;
      FSL5_M_CLK : out std_logic;
      FSL5_M_WRITE : out std_logic;
      FSL5_M_DATA : out std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL5_M_CONTROL : out std_logic;
      FSL5_M_FULL : in std_logic;
      FSL6_S_CLK : out std_logic;
      FSL6_S_READ : out std_logic;
      FSL6_S_DATA : in std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL6_S_CONTROL : in std_logic;
      FSL6_S_EXISTS : in std_logic;
      FSL6_M_CLK : out std_logic;
      FSL6_M_WRITE : out std_logic;
      FSL6_M_DATA : out std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL6_M_CONTROL : out std_logic;
      FSL6_M_FULL : in std_logic;
      FSL7_S_CLK : out std_logic;
      FSL7_S_READ : out std_logic;
      FSL7_S_DATA : in std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL7_S_CONTROL : in std_logic;
      FSL7_S_EXISTS : in std_logic;
      FSL7_M_CLK : out std_logic;
      FSL7_M_WRITE : out std_logic;
      FSL7_M_DATA : out std_logic_vector(0 to C_FSL_DATA_SIZE-1);
      FSL7_M_CONTROL : out std_logic;
      FSL7_M_FULL : in std_logic;
      ICACHE_FSL_IN_CLK : out std_logic;
      ICACHE_FSL_IN_READ : out std_logic;
      ICACHE_FSL_IN_DATA : in std_logic_vector(0 to 31);
      ICACHE_FSL_IN_CONTROL : in std_logic;
      ICACHE_FSL_IN_EXISTS : in std_logic;
      ICACHE_FSL_OUT_CLK : out std_logic;
      ICACHE_FSL_OUT_WRITE : out std_logic;
      ICACHE_FSL_OUT_DATA : out std_logic_vector(0 to 31);
      ICACHE_FSL_OUT_CONTROL : out std_logic;
      ICACHE_FSL_OUT_FULL : in std_logic;
      DCACHE_FSL_IN_CLK : out std_logic;
      DCACHE_FSL_IN_READ : out std_logic;
      DCACHE_FSL_IN_DATA : in std_logic_vector(0 to 31);
      DCACHE_FSL_IN_CONTROL : in std_logic;
      DCACHE_FSL_IN_EXISTS : in std_logic;
      DCACHE_FSL_OUT_CLK : out std_logic;
      DCACHE_FSL_OUT_WRITE : out std_logic;
      DCACHE_FSL_OUT_DATA : out std_logic_vector(0 to 31);
      DCACHE_FSL_OUT_CONTROL : out std_logic;
      DCACHE_FSL_OUT_FULL : in std_logic
    );
  end component;

begin

  microblaze_0 : microblaze
    generic map (
      C_FAMILY => "virtex2p",
      C_INSTANCE => "microblaze_0",
      C_D_OPB => 1,
      C_D_LMB => 1,
      C_I_OPB => 1,
      C_I_LMB => 1,
      C_USE_BARREL => 0,
      C_USE_DIV => 0,
      C_USE_HW_MUL => 1,
      C_USE_FPU => 0,
      C_USE_MSR_INSTR => 0,
      C_USE_PCMP_INSTR => 0,
      C_UNALIGNED_EXCEPTIONS => 0,
      C_ILL_OPCODE_EXCEPTION => 0,
      C_IOPB_BUS_EXCEPTION => 0,
      C_DOPB_BUS_EXCEPTION => 0,
      C_DIV_ZERO_EXCEPTION => 0,
      C_FPU_EXCEPTION => 0,
      C_DEBUG_ENABLED => 1,
      C_NUMBER_OF_PC_BRK => 2,
      C_NUMBER_OF_RD_ADDR_BRK => 0,
      C_NUMBER_OF_WR_ADDR_BRK => 0,
      C_INTERRUPT_IS_EDGE => 0,
      C_EDGE_IS_POSITIVE => 1,
      C_FSL_LINKS => 0,
      C_FSL_DATA_SIZE => 32,
      C_ICACHE_BASEADDR => X"00000000",
      C_ICACHE_HIGHADDR => X"3FFFFFFF",
      C_USE_ICACHE => 0,
      C_ALLOW_ICACHE_WR => 1,
      C_ADDR_TAG_BITS => 0,
      C_CACHE_BYTE_SIZE => 8192,
      C_ICACHE_USE_FSL => 0,
      C_DCACHE_BASEADDR => X"00000000",
      C_DCACHE_HIGHADDR => X"3FFFFFFF",
      C_USE_DCACHE => 0,
      C_ALLOW_DCACHE_WR => 1,
      C_DCACHE_ADDR_TAG => 0,
      C_DCACHE_BYTE_SIZE => 8192,
      C_DCACHE_USE_FSL => 0
    )
    port map (
      CLK => CLK,
      RESET => RESET,
      INTERRUPT => INTERRUPT,
      DEBUG_RST => DEBUG_RST,
      EXT_BRK => EXT_BRK,
      EXT_NM_BRK => EXT_NM_BRK,
      DBG_STOP => DBG_STOP,
      INSTR => INSTR,
      I_ADDRTAG => I_ADDRTAG,
      IREADY => IREADY,
      IWAIT => IWAIT,
      INSTR_ADDR => INSTR_ADDR,
      IFETCH => IFETCH,
      I_AS => I_AS,
      DATA_READ => DATA_READ,
      DREADY => DREADY,
      DWAIT => DWAIT,
      DATA_WRITE => DATA_WRITE,
      DATA_ADDR => DATA_ADDR,
      D_ADDRTAG => D_ADDRTAG,
      D_AS => D_AS,
      READ_STROBE => READ_STROBE,
      WRITE_STROBE => WRITE_STROBE,
      BYTE_ENABLE => BYTE_ENABLE,
      DM_ABUS => DM_ABUS,
      DM_BE => DM_BE,
      DM_BUSLOCK => DM_BUSLOCK,
      DM_DBUS => DM_DBUS,
      DM_REQUEST => DM_REQUEST,
      DM_RNW => DM_RNW,
      DM_SELECT => DM_SELECT,
      DM_SEQADDR => DM_SEQADDR,
      DOPB_DBUS => DOPB_DBUS,
      DOPB_ERRACK => DOPB_ERRACK,
      DOPB_MGRANT => DOPB_MGRANT,
      DOPB_RETRY => DOPB_RETRY,
      DOPB_TIMEOUT => DOPB_TIMEOUT,
      DOPB_XFERACK => DOPB_XFERACK,
      IM_ABUS => IM_ABUS,
      IM_BE => IM_BE,
      IM_BUSLOCK => IM_BUSLOCK,
      IM_DBUS => IM_DBUS,
      IM_REQUEST => IM_REQUEST,
      IM_RNW => IM_RNW,
      IM_SELECT => IM_SELECT,
      IM_SEQADDR => IM_SEQADDR,
      IOPB_DBUS => IOPB_DBUS,
      IOPB_ERRACK => IOPB_ERRACK,
      IOPB_MGRANT => IOPB_MGRANT,
      IOPB_RETRY => IOPB_RETRY,
      IOPB_TIMEOUT => IOPB_TIMEOUT,
      IOPB_XFERACK => IOPB_XFERACK,
      DBG_CLK => DBG_CLK,
      DBG_TDI => DBG_TDI,
      DBG_TDO => DBG_TDO,
      DBG_REG_EN => DBG_REG_EN,
      DBG_CAPTURE => DBG_CAPTURE,
      DBG_UPDATE => DBG_UPDATE,
      VALID_INSTR => VALID_INSTR,
      PC_EX => PC_EX,
      REG_WRITE => REG_WRITE,
      REG_ADDR => REG_ADDR,
      MSR_REG => MSR_REG,
      NEW_REG_VALUE => NEW_REG_VALUE,
      PIPE_RUNNING => PIPE_RUNNING,
      INTERRUPT_TAKEN => INTERRUPT_TAKEN,
      JUMP_TAKEN => JUMP_TAKEN,
      PREFETCH_ADDR => PREFETCH_ADDR,
      MB_Halted => MB_Halted,
      Trace_Branch_Instr => Trace_Branch_Instr,
      Trace_Delay_Slot => Trace_Delay_Slot,
      Trace_Data_Address => Trace_Data_Address,
      Trace_AS => Trace_AS,
      Trace_Data_Read => Trace_Data_Read,
      Trace_Data_Write => Trace_Data_Write,
      Trace_DCache_Req => Trace_DCache_Req,
      Trace_DCache_Hit => Trace_DCache_Hit,
      Trace_ICache_Req => Trace_ICache_Req,
      Trace_ICache_Hit => Trace_ICache_Hit,
      Trace_Instr_EX => Trace_Instr_EX,
      FSL0_S_CLK => FSL0_S_CLK,
      FSL0_S_READ => FSL0_S_READ,
      FSL0_S_DATA => FSL0_S_DATA,
      FSL0_S_CONTROL => FSL0_S_CONTROL,
      FSL0_S_EXISTS => FSL0_S_EXISTS,
      FSL0_M_CLK => FSL0_M_CLK,
      FSL0_M_WRITE => FSL0_M_WRITE,
      FSL0_M_DATA => FSL0_M_DATA,
      FSL0_M_CONTROL => FSL0_M_CONTROL,
      FSL0_M_FULL => FSL0_M_FULL,
      FSL1_S_CLK => FSL1_S_CLK,
      FSL1_S_READ => FSL1_S_READ,
      FSL1_S_DATA => FSL1_S_DATA,
      FSL1_S_CONTROL => FSL1_S_CONTROL,
      FSL1_S_EXISTS => FSL1_S_EXISTS,
      FSL1_M_CLK => FSL1_M_CLK,
      FSL1_M_WRITE => FSL1_M_WRITE,
      FSL1_M_DATA => FSL1_M_DATA,
      FSL1_M_CONTROL => FSL1_M_CONTROL,
      FSL1_M_FULL => FSL1_M_FULL,
      FSL2_S_CLK => FSL2_S_CLK,
      FSL2_S_READ => FSL2_S_READ,
      FSL2_S_DATA => FSL2_S_DATA,
      FSL2_S_CONTROL => FSL2_S_CONTROL,
      FSL2_S_EXISTS => FSL2_S_EXISTS,
      FSL2_M_CLK => FSL2_M_CLK,
      FSL2_M_WRITE => FSL2_M_WRITE,
      FSL2_M_DATA => FSL2_M_DATA,
      FSL2_M_CONTROL => FSL2_M_CONTROL,
      FSL2_M_FULL => FSL2_M_FULL,
      FSL3_S_CLK => FSL3_S_CLK,
      FSL3_S_READ => FSL3_S_READ,
      FSL3_S_DATA => FSL3_S_DATA,
      FSL3_S_CONTROL => FSL3_S_CONTROL,
      FSL3_S_EXISTS => FSL3_S_EXISTS,
      FSL3_M_CLK => FSL3_M_CLK,
      FSL3_M_WRITE => FSL3_M_WRITE,
      FSL3_M_DATA => FSL3_M_DATA,
      FSL3_M_CONTROL => FSL3_M_CONTROL,
      FSL3_M_FULL => FSL3_M_FULL,
      FSL4_S_CLK => FSL4_S_CLK,
      FSL4_S_READ => FSL4_S_READ,
      FSL4_S_DATA => FSL4_S_DATA,
      FSL4_S_CONTROL => FSL4_S_CONTROL,
      FSL4_S_EXISTS => FSL4_S_EXISTS,
      FSL4_M_CLK => FSL4_M_CLK,
      FSL4_M_WRITE => FSL4_M_WRITE,
      FSL4_M_DATA => FSL4_M_DATA,
      FSL4_M_CONTROL => FSL4_M_CONTROL,
      FSL4_M_FULL => FSL4_M_FULL,
      FSL5_S_CLK => FSL5_S_CLK,
      FSL5_S_READ => FSL5_S_READ,
      FSL5_S_DATA => FSL5_S_DATA,
      FSL5_S_CONTROL => FSL5_S_CONTROL,
      FSL5_S_EXISTS => FSL5_S_EXISTS,
      FSL5_M_CLK => FSL5_M_CLK,
      FSL5_M_WRITE => FSL5_M_WRITE,
      FSL5_M_DATA => FSL5_M_DATA,
      FSL5_M_CONTROL => FSL5_M_CONTROL,
      FSL5_M_FULL => FSL5_M_FULL,
      FSL6_S_CLK => FSL6_S_CLK,
      FSL6_S_READ => FSL6_S_READ,
      FSL6_S_DATA => FSL6_S_DATA,
      FSL6_S_CONTROL => FSL6_S_CONTROL,
      FSL6_S_EXISTS => FSL6_S_EXISTS,
      FSL6_M_CLK => FSL6_M_CLK,
      FSL6_M_WRITE => FSL6_M_WRITE,
      FSL6_M_DATA => FSL6_M_DATA,
      FSL6_M_CONTROL => FSL6_M_CONTROL,
      FSL6_M_FULL => FSL6_M_FULL,
      FSL7_S_CLK => FSL7_S_CLK,
      FSL7_S_READ => FSL7_S_READ,
      FSL7_S_DATA => FSL7_S_DATA,
      FSL7_S_CONTROL => FSL7_S_CONTROL,
      FSL7_S_EXISTS => FSL7_S_EXISTS,
      FSL7_M_CLK => FSL7_M_CLK,
      FSL7_M_WRITE => FSL7_M_WRITE,
      FSL7_M_DATA => FSL7_M_DATA,
      FSL7_M_CONTROL => FSL7_M_CONTROL,
      FSL7_M_FULL => FSL7_M_FULL,
      ICACHE_FSL_IN_CLK => ICACHE_FSL_IN_CLK,
      ICACHE_FSL_IN_READ => ICACHE_FSL_IN_READ,
      ICACHE_FSL_IN_DATA => ICACHE_FSL_IN_DATA,
      ICACHE_FSL_IN_CONTROL => ICACHE_FSL_IN_CONTROL,
      ICACHE_FSL_IN_EXISTS => ICACHE_FSL_IN_EXISTS,
      ICACHE_FSL_OUT_CLK => ICACHE_FSL_OUT_CLK,
      ICACHE_FSL_OUT_WRITE => ICACHE_FSL_OUT_WRITE,
      ICACHE_FSL_OUT_DATA => ICACHE_FSL_OUT_DATA,
      ICACHE_FSL_OUT_CONTROL => ICACHE_FSL_OUT_CONTROL,
      ICACHE_FSL_OUT_FULL => ICACHE_FSL_OUT_FULL,
      DCACHE_FSL_IN_CLK => DCACHE_FSL_IN_CLK,
      DCACHE_FSL_IN_READ => DCACHE_FSL_IN_READ,
      DCACHE_FSL_IN_DATA => DCACHE_FSL_IN_DATA,
      DCACHE_FSL_IN_CONTROL => DCACHE_FSL_IN_CONTROL,
      DCACHE_FSL_IN_EXISTS => DCACHE_FSL_IN_EXISTS,
      DCACHE_FSL_OUT_CLK => DCACHE_FSL_OUT_CLK,
      DCACHE_FSL_OUT_WRITE => DCACHE_FSL_OUT_WRITE,
      DCACHE_FSL_OUT_DATA => DCACHE_FSL_OUT_DATA,
      DCACHE_FSL_OUT_CONTROL => DCACHE_FSL_OUT_CONTROL,
      DCACHE_FSL_OUT_FULL => DCACHE_FSL_OUT_FULL
    );

end architecture STRUCTURE;

