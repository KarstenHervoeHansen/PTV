-------------------------------------------------------------------------------
-- flash_test14.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity flash_test14 is
  port (
    fpga_0_RS232_req_to_send_pin : out std_logic;
    fpga_0_RS232_RX_pin : in std_logic;
    fpga_0_RS232_TX_pin : out std_logic;
    fpga_0_Generic_External_Memory_Mem_DQ_pin : inout std_logic_vector(0 to 15);
    fpga_0_Generic_External_Memory_Mem_A_pin : out std_logic_vector(7 to 30);
    fpga_0_Generic_External_Memory_Mem_BEN_pin : out std_logic_vector(0 to 1);
    fpga_0_Generic_External_Memory_Mem_WEN_pin : out std_logic;
    fpga_0_Generic_External_Memory_Mem_OEN_pin : out std_logic_vector(0 to 0);
    fpga_0_Generic_External_Memory_Mem_CEN_pin : out std_logic_vector(0 to 0);
    fpga_0_Generic_External_Memory_flash_csn_dummy_pin : out std_logic;
    fpga_0_Generic_External_Memory_Mem_RPN_pin : out std_logic;
    sys_clk_pin_p : in std_logic;
    sys_clk_pin_n : in std_logic;
    sys_rst_pin : in std_logic;
    FADV : out std_logic;
    FRST : out std_logic;
    FWP : out std_logic
  );
end flash_test14;

architecture STRUCTURE of flash_test14 is

  attribute box_type : STRING;

  component microblaze_0_wrapper is
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
  end component;

  attribute box_type of microblaze_0_wrapper: component is "black_box";

  component mb_opb_wrapper is
    port (
      OPB_Clk : in std_logic;
      OPB_Rst : out std_logic;
      SYS_Rst : in std_logic;
      Debug_SYS_Rst : in std_logic;
      WDT_Rst : in std_logic;
      M_ABus : in std_logic_vector(0 to 63);
      M_BE : in std_logic_vector(0 to 7);
      M_beXfer : in std_logic_vector(0 to 1);
      M_busLock : in std_logic_vector(0 to 1);
      M_DBus : in std_logic_vector(0 to 63);
      M_DBusEn : in std_logic_vector(0 to 1);
      M_DBusEn32_63 : in std_logic_vector(0 to 1);
      M_dwXfer : in std_logic_vector(0 to 1);
      M_fwXfer : in std_logic_vector(0 to 1);
      M_hwXfer : in std_logic_vector(0 to 1);
      M_request : in std_logic_vector(0 to 1);
      M_RNW : in std_logic_vector(0 to 1);
      M_select : in std_logic_vector(0 to 1);
      M_seqAddr : in std_logic_vector(0 to 1);
      Sl_beAck : in std_logic_vector(0 to 3);
      Sl_DBus : in std_logic_vector(0 to 127);
      Sl_DBusEn : in std_logic_vector(0 to 3);
      Sl_DBusEn32_63 : in std_logic_vector(0 to 3);
      Sl_errAck : in std_logic_vector(0 to 3);
      Sl_dwAck : in std_logic_vector(0 to 3);
      Sl_fwAck : in std_logic_vector(0 to 3);
      Sl_hwAck : in std_logic_vector(0 to 3);
      Sl_retry : in std_logic_vector(0 to 3);
      Sl_toutSup : in std_logic_vector(0 to 3);
      Sl_xferAck : in std_logic_vector(0 to 3);
      OPB_MRequest : out std_logic_vector(0 to 1);
      OPB_ABus : out std_logic_vector(0 to 31);
      OPB_BE : out std_logic_vector(0 to 3);
      OPB_beXfer : out std_logic;
      OPB_beAck : out std_logic;
      OPB_busLock : out std_logic;
      OPB_rdDBus : out std_logic_vector(0 to 31);
      OPB_wrDBus : out std_logic_vector(0 to 31);
      OPB_DBus : out std_logic_vector(0 to 31);
      OPB_errAck : out std_logic;
      OPB_dwAck : out std_logic;
      OPB_dwXfer : out std_logic;
      OPB_fwAck : out std_logic;
      OPB_fwXfer : out std_logic;
      OPB_hwAck : out std_logic;
      OPB_hwXfer : out std_logic;
      OPB_MGrant : out std_logic_vector(0 to 1);
      OPB_pendReq : out std_logic_vector(0 to 1);
      OPB_retry : out std_logic;
      OPB_RNW : out std_logic;
      OPB_select : out std_logic;
      OPB_seqAddr : out std_logic;
      OPB_timeout : out std_logic;
      OPB_toutSup : out std_logic;
      OPB_xferAck : out std_logic
    );
  end component;

  attribute box_type of mb_opb_wrapper: component is "black_box";

  component debug_module_wrapper is
    port (
      OPB_Clk : in std_logic;
      OPB_Rst : in std_logic;
      Interrupt : out std_logic;
      Debug_SYS_Rst : out std_logic;
      Debug_Rst : out std_logic;
      Ext_BRK : out std_logic;
      Ext_NM_BRK : out std_logic;
      OPB_ABus : in std_logic_vector(0 to 31);
      OPB_BE : in std_logic_vector(0 to 3);
      OPB_RNW : in std_logic;
      OPB_select : in std_logic;
      OPB_seqAddr : in std_logic;
      OPB_DBus : in std_logic_vector(0 to 31);
      MDM_DBus : out std_logic_vector(0 to 31);
      MDM_errAck : out std_logic;
      MDM_retry : out std_logic;
      MDM_toutSup : out std_logic;
      MDM_xferAck : out std_logic;
      Dbg_Clk_0 : out std_logic;
      Dbg_TDI_0 : out std_logic;
      Dbg_TDO_0 : in std_logic;
      Dbg_Reg_En_0 : out std_logic_vector(0 to 4);
      Dbg_Capture_0 : out std_logic;
      Dbg_Update_0 : out std_logic;
      Dbg_Clk_1 : out std_logic;
      Dbg_TDI_1 : out std_logic;
      Dbg_TDO_1 : in std_logic;
      Dbg_Reg_En_1 : out std_logic_vector(0 to 4);
      Dbg_Capture_1 : out std_logic;
      Dbg_Update_1 : out std_logic;
      Dbg_Clk_2 : out std_logic;
      Dbg_TDI_2 : out std_logic;
      Dbg_TDO_2 : in std_logic;
      Dbg_Reg_En_2 : out std_logic_vector(0 to 4);
      Dbg_Capture_2 : out std_logic;
      Dbg_Update_2 : out std_logic;
      Dbg_Clk_3 : out std_logic;
      Dbg_TDI_3 : out std_logic;
      Dbg_TDO_3 : in std_logic;
      Dbg_Reg_En_3 : out std_logic_vector(0 to 4);
      Dbg_Capture_3 : out std_logic;
      Dbg_Update_3 : out std_logic;
      Dbg_Clk_4 : out std_logic;
      Dbg_TDI_4 : out std_logic;
      Dbg_TDO_4 : in std_logic;
      Dbg_Reg_En_4 : out std_logic_vector(0 to 4);
      Dbg_Capture_4 : out std_logic;
      Dbg_Update_4 : out std_logic;
      Dbg_Clk_5 : out std_logic;
      Dbg_TDI_5 : out std_logic;
      Dbg_TDO_5 : in std_logic;
      Dbg_Reg_En_5 : out std_logic_vector(0 to 4);
      Dbg_Capture_5 : out std_logic;
      Dbg_Update_5 : out std_logic;
      Dbg_Clk_6 : out std_logic;
      Dbg_TDI_6 : out std_logic;
      Dbg_TDO_6 : in std_logic;
      Dbg_Reg_En_6 : out std_logic_vector(0 to 4);
      Dbg_Capture_6 : out std_logic;
      Dbg_Update_6 : out std_logic;
      Dbg_Clk_7 : out std_logic;
      Dbg_TDI_7 : out std_logic;
      Dbg_TDO_7 : in std_logic;
      Dbg_Reg_En_7 : out std_logic_vector(0 to 4);
      Dbg_Capture_7 : out std_logic;
      Dbg_Update_7 : out std_logic;
      bscan_tdi : out std_logic;
      bscan_reset : out std_logic;
      bscan_shift : out std_logic;
      bscan_update : out std_logic;
      bscan_capture : out std_logic;
      bscan_sel1 : out std_logic;
      bscan_drck1 : out std_logic;
      bscan_tdo1 : in std_logic;
      FSL0_S_CLK : out std_logic;
      FSL0_S_READ : out std_logic;
      FSL0_S_DATA : in std_logic_vector(0 to 31);
      FSL0_S_CONTROL : in std_logic;
      FSL0_S_EXISTS : in std_logic;
      FSL0_M_CLK : out std_logic;
      FSL0_M_WRITE : out std_logic;
      FSL0_M_DATA : out std_logic_vector(0 to 31);
      FSL0_M_CONTROL : out std_logic;
      FSL0_M_FULL : in std_logic
    );
  end component;

  attribute box_type of debug_module_wrapper: component is "black_box";

  component ilmb_wrapper is
    port (
      LMB_Clk : in std_logic;
      SYS_Rst : in std_logic;
      LMB_Rst : out std_logic;
      M_ABus : in std_logic_vector(0 to 31);
      M_ReadStrobe : in std_logic;
      M_WriteStrobe : in std_logic;
      M_AddrStrobe : in std_logic;
      M_DBus : in std_logic_vector(0 to 31);
      M_BE : in std_logic_vector(0 to 3);
      Sl_DBus : in std_logic_vector(0 to 31);
      Sl_Ready : in std_logic_vector(0 to 0);
      LMB_ABus : out std_logic_vector(0 to 31);
      LMB_ReadStrobe : out std_logic;
      LMB_WriteStrobe : out std_logic;
      LMB_AddrStrobe : out std_logic;
      LMB_ReadDBus : out std_logic_vector(0 to 31);
      LMB_WriteDBus : out std_logic_vector(0 to 31);
      LMB_Ready : out std_logic;
      LMB_BE : out std_logic_vector(0 to 3)
    );
  end component;

  attribute box_type of ilmb_wrapper: component is "black_box";

  component dlmb_wrapper is
    port (
      LMB_Clk : in std_logic;
      SYS_Rst : in std_logic;
      LMB_Rst : out std_logic;
      M_ABus : in std_logic_vector(0 to 31);
      M_ReadStrobe : in std_logic;
      M_WriteStrobe : in std_logic;
      M_AddrStrobe : in std_logic;
      M_DBus : in std_logic_vector(0 to 31);
      M_BE : in std_logic_vector(0 to 3);
      Sl_DBus : in std_logic_vector(0 to 31);
      Sl_Ready : in std_logic_vector(0 to 0);
      LMB_ABus : out std_logic_vector(0 to 31);
      LMB_ReadStrobe : out std_logic;
      LMB_WriteStrobe : out std_logic;
      LMB_AddrStrobe : out std_logic;
      LMB_ReadDBus : out std_logic_vector(0 to 31);
      LMB_WriteDBus : out std_logic_vector(0 to 31);
      LMB_Ready : out std_logic;
      LMB_BE : out std_logic_vector(0 to 3)
    );
  end component;

  attribute box_type of dlmb_wrapper: component is "black_box";

  component dlmb_cntlr_wrapper is
    port (
      LMB_Clk : in std_logic;
      LMB_Rst : in std_logic;
      LMB_ABus : in std_logic_vector(0 to 31);
      LMB_WriteDBus : in std_logic_vector(0 to 31);
      LMB_AddrStrobe : in std_logic;
      LMB_ReadStrobe : in std_logic;
      LMB_WriteStrobe : in std_logic;
      LMB_BE : in std_logic_vector(0 to 3);
      Sl_DBus : out std_logic_vector(0 to 31);
      Sl_Ready : out std_logic;
      BRAM_Rst_A : out std_logic;
      BRAM_Clk_A : out std_logic;
      BRAM_EN_A : out std_logic;
      BRAM_WEN_A : out std_logic_vector(0 to 3);
      BRAM_Addr_A : out std_logic_vector(0 to 31);
      BRAM_Din_A : in std_logic_vector(0 to 31);
      BRAM_Dout_A : out std_logic_vector(0 to 31)
    );
  end component;

  attribute box_type of dlmb_cntlr_wrapper: component is "black_box";

  component ilmb_cntlr_wrapper is
    port (
      LMB_Clk : in std_logic;
      LMB_Rst : in std_logic;
      LMB_ABus : in std_logic_vector(0 to 31);
      LMB_WriteDBus : in std_logic_vector(0 to 31);
      LMB_AddrStrobe : in std_logic;
      LMB_ReadStrobe : in std_logic;
      LMB_WriteStrobe : in std_logic;
      LMB_BE : in std_logic_vector(0 to 3);
      Sl_DBus : out std_logic_vector(0 to 31);
      Sl_Ready : out std_logic;
      BRAM_Rst_A : out std_logic;
      BRAM_Clk_A : out std_logic;
      BRAM_EN_A : out std_logic;
      BRAM_WEN_A : out std_logic_vector(0 to 3);
      BRAM_Addr_A : out std_logic_vector(0 to 31);
      BRAM_Din_A : in std_logic_vector(0 to 31);
      BRAM_Dout_A : out std_logic_vector(0 to 31)
    );
  end component;

  attribute box_type of ilmb_cntlr_wrapper: component is "black_box";

  component lmb_bram_wrapper is
    port (
      BRAM_Rst_A : in std_logic;
      BRAM_Clk_A : in std_logic;
      BRAM_EN_A : in std_logic;
      BRAM_WEN_A : in std_logic_vector(0 to 3);
      BRAM_Addr_A : in std_logic_vector(0 to 31);
      BRAM_Din_A : out std_logic_vector(0 to 31);
      BRAM_Dout_A : in std_logic_vector(0 to 31);
      BRAM_Rst_B : in std_logic;
      BRAM_Clk_B : in std_logic;
      BRAM_EN_B : in std_logic;
      BRAM_WEN_B : in std_logic_vector(0 to 3);
      BRAM_Addr_B : in std_logic_vector(0 to 31);
      BRAM_Din_B : out std_logic_vector(0 to 31);
      BRAM_Dout_B : in std_logic_vector(0 to 31)
    );
  end component;

  attribute box_type of lmb_bram_wrapper: component is "black_box";

  component generic_external_memory_wrapper is
    port (
      OPB_Clk : in std_logic;
      OPB_Rst : in std_logic;
      OPB_ABus : in std_logic_vector(0 to 31);
      OPB_DBus : in std_logic_vector(0 to 31);
      Sln_DBus : out std_logic_vector(0 to 31);
      OPB_select : in std_logic;
      OPB_RNW : in std_logic;
      OPB_seqAddr : in std_logic;
      OPB_BE : in std_logic_vector(0 to 3);
      Sln_xferAck : out std_logic;
      Sln_errAck : out std_logic;
      Sln_toutSup : out std_logic;
      Sln_retry : out std_logic;
      Mem_A : out std_logic_vector(0 to 31);
      Mem_DQ_I : in std_logic_vector(0 to 15);
      Mem_DQ_O : out std_logic_vector(0 to 15);
      Mem_DQ_T : out std_logic_vector(0 to 15);
      Mem_CEN : out std_logic_vector(0 to 0);
      Mem_OEN : out std_logic_vector(0 to 0);
      Mem_WEN : out std_logic;
      Mem_QWEN : out std_logic_vector(0 to 1);
      Mem_BEN : out std_logic_vector(0 to 1);
      Mem_RPN : out std_logic;
      Mem_CE : out std_logic_vector(0 to 0);
      Mem_ADV_LDN : out std_logic;
      Mem_LBON : out std_logic;
      Mem_CKEN : out std_logic;
      Mem_RNW : out std_logic
    );
  end component;

  attribute box_type of generic_external_memory_wrapper: component is "black_box";

  component rs232_wrapper is
    port (
      OPB_Clk : in std_logic;
      OPB_Rst : in std_logic;
      Interrupt : out std_logic;
      OPB_ABus : in std_logic_vector(0 to 31);
      OPB_BE : in std_logic_vector(0 to 3);
      OPB_RNW : in std_logic;
      OPB_select : in std_logic;
      OPB_seqAddr : in std_logic;
      OPB_DBus : in std_logic_vector(0 to 31);
      UART_DBus : out std_logic_vector(0 to 31);
      UART_errAck : out std_logic;
      UART_retry : out std_logic;
      UART_toutSup : out std_logic;
      UART_xferAck : out std_logic;
      RX : in std_logic;
      TX : out std_logic
    );
  end component;

  attribute box_type of rs232_wrapper: component is "black_box";

  component opb_bram_if_cntlr_1_wrapper is
    port (
      opb_clk : in std_logic;
      opb_rst : in std_logic;
      opb_abus : in std_logic_vector(0 to 31);
      opb_dbus : in std_logic_vector(0 to 31);
      sln_dbus : out std_logic_vector(0 to 31);
      opb_select : in std_logic;
      opb_rnw : in std_logic;
      opb_seqaddr : in std_logic;
      opb_be : in std_logic_vector(0 to 3);
      sln_xferack : out std_logic;
      sln_errack : out std_logic;
      sln_toutsup : out std_logic;
      sln_retry : out std_logic;
      bram_rst : out std_logic;
      bram_clk : out std_logic;
      bram_en : out std_logic;
      bram_wen : out std_logic_vector(0 to 3);
      bram_addr : out std_logic_vector(0 to 31);
      bram_din : in std_logic_vector(0 to 31);
      bram_dout : out std_logic_vector(0 to 31)
    );
  end component;

  attribute box_type of opb_bram_if_cntlr_1_wrapper: component is "black_box";

  component opb_bram_if_cntlr_1_bram_wrapper is
    port (
      BRAM_Rst_A : in std_logic;
      BRAM_Clk_A : in std_logic;
      BRAM_EN_A : in std_logic;
      BRAM_WEN_A : in std_logic_vector(0 to 3);
      BRAM_Addr_A : in std_logic_vector(0 to 31);
      BRAM_Din_A : out std_logic_vector(0 to 31);
      BRAM_Dout_A : in std_logic_vector(0 to 31);
      BRAM_Rst_B : in std_logic;
      BRAM_Clk_B : in std_logic;
      BRAM_EN_B : in std_logic;
      BRAM_WEN_B : in std_logic_vector(0 to 3);
      BRAM_Addr_B : in std_logic_vector(0 to 31);
      BRAM_Din_B : out std_logic_vector(0 to 31);
      BRAM_Dout_B : in std_logic_vector(0 to 31)
    );
  end component;

  attribute box_type of opb_bram_if_cntlr_1_bram_wrapper: component is "black_box";

  component dcm_0_wrapper is
    port (
      RST : in std_logic;
      CLKIN : in std_logic;
      CLKFB : in std_logic;
      PSEN : in std_logic;
      PSINCDEC : in std_logic;
      PSCLK : in std_logic;
      DSSEN : in std_logic;
      CLK0 : out std_logic;
      CLK90 : out std_logic;
      CLK180 : out std_logic;
      CLK270 : out std_logic;
      CLKDV : out std_logic;
      CLK2X : out std_logic;
      CLK2X180 : out std_logic;
      CLKFX : out std_logic;
      CLKFX180 : out std_logic;
      STATUS : out std_logic_vector(7 downto 0);
      LOCKED : out std_logic;
      PSDONE : out std_logic
    );
  end component;

  attribute box_type of dcm_0_wrapper: component is "black_box";

  component util_bus_split_0_wrapper is
    port (
      Sig : in std_logic_vector(0 to 31);
      Out1 : out std_logic_vector(7 to 30);
      Out2 : out std_logic_vector(31 to 31)
    );
  end component;

  attribute box_type of util_bus_split_0_wrapper: component is "black_box";

  component OBUF is
    port (
      I : in std_logic;
      O : out std_logic
    );
  end component;

  component IBUF is
    port (
      I : in std_logic;
      O : out std_logic
    );
  end component;

  component IOBUF is
    port (
      I : in std_logic;
      IO : inout std_logic;
      O : out std_logic;
      T : in std_logic
    );
  end component;

  component IBUFG is
    port (
      I : in std_logic;
      O : out std_logic
    );
  end component;

  -- Internal signals

  signal DBG_CAPTURE_s : std_logic;
  signal DBG_CLK_s : std_logic;
  signal DBG_REG_EN_s : std_logic_vector(0 to 4);
  signal DBG_TDI_s : std_logic;
  signal DBG_TDO_s : std_logic;
  signal DBG_UPDATE_s : std_logic;
  signal Debug_Rst : std_logic;
  signal Ext_BRK : std_logic;
  signal Ext_NM_BRK : std_logic;
  signal dcm_0_FB : std_logic;
  signal dcm_clk_s : std_logic;
  signal dlmb_LMB_ABus : std_logic_vector(0 to 31);
  signal dlmb_LMB_AddrStrobe : std_logic;
  signal dlmb_LMB_BE : std_logic_vector(0 to 3);
  signal dlmb_LMB_ReadDBus : std_logic_vector(0 to 31);
  signal dlmb_LMB_ReadStrobe : std_logic;
  signal dlmb_LMB_Ready : std_logic;
  signal dlmb_LMB_WriteDBus : std_logic_vector(0 to 31);
  signal dlmb_LMB_WriteStrobe : std_logic;
  signal dlmb_M_ABus : std_logic_vector(0 to 31);
  signal dlmb_M_AddrStrobe : std_logic;
  signal dlmb_M_BE : std_logic_vector(0 to 3);
  signal dlmb_M_DBus : std_logic_vector(0 to 31);
  signal dlmb_M_ReadStrobe : std_logic;
  signal dlmb_M_WriteStrobe : std_logic;
  signal dlmb_OPB_Rst : std_logic;
  signal dlmb_Sl_DBus : std_logic_vector(0 to 31);
  signal dlmb_Sl_Ready : std_logic_vector(0 to 0);
  signal dlmb_port_BRAM_Addr : std_logic_vector(0 to 31);
  signal dlmb_port_BRAM_Clk : std_logic;
  signal dlmb_port_BRAM_Din : std_logic_vector(0 to 31);
  signal dlmb_port_BRAM_Dout : std_logic_vector(0 to 31);
  signal dlmb_port_BRAM_EN : std_logic;
  signal dlmb_port_BRAM_Rst : std_logic;
  signal dlmb_port_BRAM_WEN : std_logic_vector(0 to 3);
  signal fpga_0_Generic_External_Memory_Mem_A : std_logic_vector(7 to 30);
  signal fpga_0_Generic_External_Memory_Mem_A_split : std_logic_vector(0 to 31);
  signal fpga_0_Generic_External_Memory_Mem_BEN : std_logic_vector(0 to 1);
  signal fpga_0_Generic_External_Memory_Mem_CEN : std_logic_vector(0 to 0);
  signal fpga_0_Generic_External_Memory_Mem_DQ_I : std_logic_vector(0 to 15);
  signal fpga_0_Generic_External_Memory_Mem_DQ_O : std_logic_vector(0 to 15);
  signal fpga_0_Generic_External_Memory_Mem_DQ_T : std_logic_vector(0 to 15);
  signal fpga_0_Generic_External_Memory_Mem_OEN : std_logic_vector(0 to 0);
  signal fpga_0_Generic_External_Memory_Mem_RPN : std_logic;
  signal fpga_0_Generic_External_Memory_Mem_WEN : std_logic;
  signal fpga_0_RS232_RX : std_logic;
  signal fpga_0_RS232_TX : std_logic;
  signal ilmb_LMB_ABus : std_logic_vector(0 to 31);
  signal ilmb_LMB_AddrStrobe : std_logic;
  signal ilmb_LMB_BE : std_logic_vector(0 to 3);
  signal ilmb_LMB_ReadDBus : std_logic_vector(0 to 31);
  signal ilmb_LMB_ReadStrobe : std_logic;
  signal ilmb_LMB_Ready : std_logic;
  signal ilmb_LMB_WriteDBus : std_logic_vector(0 to 31);
  signal ilmb_LMB_WriteStrobe : std_logic;
  signal ilmb_M_ABus : std_logic_vector(0 to 31);
  signal ilmb_M_AddrStrobe : std_logic;
  signal ilmb_M_ReadStrobe : std_logic;
  signal ilmb_OPB_Rst : std_logic;
  signal ilmb_Sl_DBus : std_logic_vector(0 to 31);
  signal ilmb_Sl_Ready : std_logic_vector(0 to 0);
  signal ilmb_port_BRAM_Addr : std_logic_vector(0 to 31);
  signal ilmb_port_BRAM_Clk : std_logic;
  signal ilmb_port_BRAM_Din : std_logic_vector(0 to 31);
  signal ilmb_port_BRAM_Dout : std_logic_vector(0 to 31);
  signal ilmb_port_BRAM_EN : std_logic;
  signal ilmb_port_BRAM_Rst : std_logic;
  signal ilmb_port_BRAM_WEN : std_logic_vector(0 to 3);
  signal mb_opb_Debug_SYS_Rst : std_logic;
  signal mb_opb_M_ABus : std_logic_vector(0 to 63);
  signal mb_opb_M_BE : std_logic_vector(0 to 7);
  signal mb_opb_M_DBus : std_logic_vector(0 to 63);
  signal mb_opb_M_RNW : std_logic_vector(0 to 1);
  signal mb_opb_M_busLock : std_logic_vector(0 to 1);
  signal mb_opb_M_request : std_logic_vector(0 to 1);
  signal mb_opb_M_select : std_logic_vector(0 to 1);
  signal mb_opb_M_seqAddr : std_logic_vector(0 to 1);
  signal mb_opb_OPB_ABus : std_logic_vector(0 to 31);
  signal mb_opb_OPB_BE : std_logic_vector(0 to 3);
  signal mb_opb_OPB_DBus : std_logic_vector(0 to 31);
  signal mb_opb_OPB_MGrant : std_logic_vector(0 to 1);
  signal mb_opb_OPB_RNW : std_logic;
  signal mb_opb_OPB_Rst : std_logic;
  signal mb_opb_OPB_errAck : std_logic;
  signal mb_opb_OPB_retry : std_logic;
  signal mb_opb_OPB_select : std_logic;
  signal mb_opb_OPB_seqAddr : std_logic;
  signal mb_opb_OPB_timeout : std_logic;
  signal mb_opb_OPB_xferAck : std_logic;
  signal mb_opb_Sl_DBus : std_logic_vector(0 to 127);
  signal mb_opb_Sl_errAck : std_logic_vector(0 to 3);
  signal mb_opb_Sl_retry : std_logic_vector(0 to 3);
  signal mb_opb_Sl_toutSup : std_logic_vector(0 to 3);
  signal mb_opb_Sl_xferAck : std_logic_vector(0 to 3);
  signal net_gnd0 : std_logic;
  signal net_gnd2 : std_logic_vector(0 to 1);
  signal net_gnd32 : std_logic_vector(0 to 31);
  signal net_gnd4 : std_logic_vector(0 to 3);
  signal net_vcc0 : std_logic;
  signal net_vcc2 : std_logic_vector(0 to 1);
  signal net_vcc4 : std_logic_vector(0 to 3);
  signal opb_bram_if_cntlr_1_port_BRAM_Addr : std_logic_vector(0 to 31);
  signal opb_bram_if_cntlr_1_port_BRAM_Clk : std_logic;
  signal opb_bram_if_cntlr_1_port_BRAM_Din : std_logic_vector(0 to 31);
  signal opb_bram_if_cntlr_1_port_BRAM_Dout : std_logic_vector(0 to 31);
  signal opb_bram_if_cntlr_1_port_BRAM_EN : std_logic;
  signal opb_bram_if_cntlr_1_port_BRAM_Rst : std_logic;
  signal opb_bram_if_cntlr_1_port_BRAM_WEN : std_logic_vector(0 to 3);
  signal sys_clk_s : std_logic;
  signal sys_rst_s : std_logic;

begin

  -- Internal assignments

  net_gnd0 <= '0';
  net_gnd2(0 to 1) <= B"00";
  net_gnd32(0 to 31) <= B"00000000000000000000000000000000";
  net_gnd4(0 to 3) <= B"0000";
  net_vcc0 <= '1';
  net_vcc2(0 to 1) <= B"11";
  net_vcc4(0 to 3) <= B"1111";

  microblaze_0 : microblaze_0_wrapper
    port map (
      CLK => sys_clk_s,
      RESET => mb_opb_OPB_Rst,
      INTERRUPT => net_gnd0,
      DEBUG_RST => Debug_Rst,
      EXT_BRK => Ext_BRK,
      EXT_NM_BRK => Ext_NM_BRK,
      DBG_STOP => net_gnd0,
      INSTR => ilmb_LMB_ReadDBus,
      I_ADDRTAG => open,
      IREADY => ilmb_LMB_Ready,
      IWAIT => net_gnd0,
      INSTR_ADDR => ilmb_M_ABus,
      IFETCH => ilmb_M_ReadStrobe,
      I_AS => ilmb_M_AddrStrobe,
      DATA_READ => dlmb_LMB_ReadDBus,
      DREADY => dlmb_LMB_Ready,
      DWAIT => net_gnd0,
      DATA_WRITE => dlmb_M_DBus,
      DATA_ADDR => dlmb_M_ABus,
      D_ADDRTAG => open,
      D_AS => dlmb_M_AddrStrobe,
      READ_STROBE => dlmb_M_ReadStrobe,
      WRITE_STROBE => dlmb_M_WriteStrobe,
      BYTE_ENABLE => dlmb_M_BE,
      DM_ABUS => mb_opb_M_ABus(0 to 31),
      DM_BE => mb_opb_M_BE(0 to 3),
      DM_BUSLOCK => mb_opb_M_busLock(0),
      DM_DBUS => mb_opb_M_DBus(0 to 31),
      DM_REQUEST => mb_opb_M_request(0),
      DM_RNW => mb_opb_M_RNW(0),
      DM_SELECT => mb_opb_M_select(0),
      DM_SEQADDR => mb_opb_M_seqAddr(0),
      DOPB_DBUS => mb_opb_OPB_DBus,
      DOPB_ERRACK => mb_opb_OPB_errAck,
      DOPB_MGRANT => mb_opb_OPB_MGrant(0),
      DOPB_RETRY => mb_opb_OPB_retry,
      DOPB_TIMEOUT => mb_opb_OPB_timeout,
      DOPB_XFERACK => mb_opb_OPB_xferAck,
      IM_ABUS => mb_opb_M_ABus(32 to 63),
      IM_BE => mb_opb_M_BE(4 to 7),
      IM_BUSLOCK => mb_opb_M_busLock(1),
      IM_DBUS => mb_opb_M_DBus(32 to 63),
      IM_REQUEST => mb_opb_M_request(1),
      IM_RNW => mb_opb_M_RNW(1),
      IM_SELECT => mb_opb_M_select(1),
      IM_SEQADDR => mb_opb_M_seqAddr(1),
      IOPB_DBUS => mb_opb_OPB_DBus,
      IOPB_ERRACK => mb_opb_OPB_errAck,
      IOPB_MGRANT => mb_opb_OPB_MGrant(1),
      IOPB_RETRY => mb_opb_OPB_retry,
      IOPB_TIMEOUT => mb_opb_OPB_timeout,
      IOPB_XFERACK => mb_opb_OPB_xferAck,
      DBG_CLK => DBG_CLK_s,
      DBG_TDI => DBG_TDI_s,
      DBG_TDO => DBG_TDO_s,
      DBG_REG_EN => DBG_REG_EN_s,
      DBG_CAPTURE => DBG_CAPTURE_s,
      DBG_UPDATE => DBG_UPDATE_s,
      VALID_INSTR => open,
      PC_EX => open,
      REG_WRITE => open,
      REG_ADDR => open,
      MSR_REG => open,
      NEW_REG_VALUE => open,
      PIPE_RUNNING => open,
      INTERRUPT_TAKEN => open,
      JUMP_TAKEN => open,
      PREFETCH_ADDR => open,
      MB_Halted => open,
      Trace_Branch_Instr => open,
      Trace_Delay_Slot => open,
      Trace_Data_Address => open,
      Trace_AS => open,
      Trace_Data_Read => open,
      Trace_Data_Write => open,
      Trace_DCache_Req => open,
      Trace_DCache_Hit => open,
      Trace_ICache_Req => open,
      Trace_ICache_Hit => open,
      Trace_Instr_EX => open,
      FSL0_S_CLK => open,
      FSL0_S_READ => open,
      FSL0_S_DATA => net_gnd32,
      FSL0_S_CONTROL => net_gnd0,
      FSL0_S_EXISTS => net_gnd0,
      FSL0_M_CLK => open,
      FSL0_M_WRITE => open,
      FSL0_M_DATA => open,
      FSL0_M_CONTROL => open,
      FSL0_M_FULL => net_gnd0,
      FSL1_S_CLK => open,
      FSL1_S_READ => open,
      FSL1_S_DATA => net_gnd32,
      FSL1_S_CONTROL => net_gnd0,
      FSL1_S_EXISTS => net_gnd0,
      FSL1_M_CLK => open,
      FSL1_M_WRITE => open,
      FSL1_M_DATA => open,
      FSL1_M_CONTROL => open,
      FSL1_M_FULL => net_gnd0,
      FSL2_S_CLK => open,
      FSL2_S_READ => open,
      FSL2_S_DATA => net_gnd32,
      FSL2_S_CONTROL => net_gnd0,
      FSL2_S_EXISTS => net_gnd0,
      FSL2_M_CLK => open,
      FSL2_M_WRITE => open,
      FSL2_M_DATA => open,
      FSL2_M_CONTROL => open,
      FSL2_M_FULL => net_gnd0,
      FSL3_S_CLK => open,
      FSL3_S_READ => open,
      FSL3_S_DATA => net_gnd32,
      FSL3_S_CONTROL => net_gnd0,
      FSL3_S_EXISTS => net_gnd0,
      FSL3_M_CLK => open,
      FSL3_M_WRITE => open,
      FSL3_M_DATA => open,
      FSL3_M_CONTROL => open,
      FSL3_M_FULL => net_gnd0,
      FSL4_S_CLK => open,
      FSL4_S_READ => open,
      FSL4_S_DATA => net_gnd32,
      FSL4_S_CONTROL => net_gnd0,
      FSL4_S_EXISTS => net_gnd0,
      FSL4_M_CLK => open,
      FSL4_M_WRITE => open,
      FSL4_M_DATA => open,
      FSL4_M_CONTROL => open,
      FSL4_M_FULL => net_gnd0,
      FSL5_S_CLK => open,
      FSL5_S_READ => open,
      FSL5_S_DATA => net_gnd32,
      FSL5_S_CONTROL => net_gnd0,
      FSL5_S_EXISTS => net_gnd0,
      FSL5_M_CLK => open,
      FSL5_M_WRITE => open,
      FSL5_M_DATA => open,
      FSL5_M_CONTROL => open,
      FSL5_M_FULL => net_gnd0,
      FSL6_S_CLK => open,
      FSL6_S_READ => open,
      FSL6_S_DATA => net_gnd32,
      FSL6_S_CONTROL => net_gnd0,
      FSL6_S_EXISTS => net_gnd0,
      FSL6_M_CLK => open,
      FSL6_M_WRITE => open,
      FSL6_M_DATA => open,
      FSL6_M_CONTROL => open,
      FSL6_M_FULL => net_gnd0,
      FSL7_S_CLK => open,
      FSL7_S_READ => open,
      FSL7_S_DATA => net_gnd32,
      FSL7_S_CONTROL => net_gnd0,
      FSL7_S_EXISTS => net_gnd0,
      FSL7_M_CLK => open,
      FSL7_M_WRITE => open,
      FSL7_M_DATA => open,
      FSL7_M_CONTROL => open,
      FSL7_M_FULL => net_gnd0,
      ICACHE_FSL_IN_CLK => open,
      ICACHE_FSL_IN_READ => open,
      ICACHE_FSL_IN_DATA => net_gnd32,
      ICACHE_FSL_IN_CONTROL => net_gnd0,
      ICACHE_FSL_IN_EXISTS => net_gnd0,
      ICACHE_FSL_OUT_CLK => open,
      ICACHE_FSL_OUT_WRITE => open,
      ICACHE_FSL_OUT_DATA => open,
      ICACHE_FSL_OUT_CONTROL => open,
      ICACHE_FSL_OUT_FULL => net_gnd0,
      DCACHE_FSL_IN_CLK => open,
      DCACHE_FSL_IN_READ => open,
      DCACHE_FSL_IN_DATA => net_gnd32,
      DCACHE_FSL_IN_CONTROL => net_gnd0,
      DCACHE_FSL_IN_EXISTS => net_gnd0,
      DCACHE_FSL_OUT_CLK => open,
      DCACHE_FSL_OUT_WRITE => open,
      DCACHE_FSL_OUT_DATA => open,
      DCACHE_FSL_OUT_CONTROL => open,
      DCACHE_FSL_OUT_FULL => net_gnd0
    );

  mb_opb : mb_opb_wrapper
    port map (
      OPB_Clk => sys_clk_s,
      OPB_Rst => mb_opb_OPB_Rst,
      SYS_Rst => sys_rst_s,
      Debug_SYS_Rst => mb_opb_Debug_SYS_Rst,
      WDT_Rst => net_gnd0,
      M_ABus => mb_opb_M_ABus,
      M_BE => mb_opb_M_BE,
      M_beXfer => net_gnd2,
      M_busLock => mb_opb_M_busLock,
      M_DBus => mb_opb_M_DBus,
      M_DBusEn => net_gnd2,
      M_DBusEn32_63 => net_vcc2,
      M_dwXfer => net_gnd2,
      M_fwXfer => net_gnd2,
      M_hwXfer => net_gnd2,
      M_request => mb_opb_M_request,
      M_RNW => mb_opb_M_RNW,
      M_select => mb_opb_M_select,
      M_seqAddr => mb_opb_M_seqAddr,
      Sl_beAck => net_gnd4,
      Sl_DBus => mb_opb_Sl_DBus,
      Sl_DBusEn => net_vcc4,
      Sl_DBusEn32_63 => net_vcc4,
      Sl_errAck => mb_opb_Sl_errAck,
      Sl_dwAck => net_gnd4,
      Sl_fwAck => net_gnd4,
      Sl_hwAck => net_gnd4,
      Sl_retry => mb_opb_Sl_retry,
      Sl_toutSup => mb_opb_Sl_toutSup,
      Sl_xferAck => mb_opb_Sl_xferAck,
      OPB_MRequest => open,
      OPB_ABus => mb_opb_OPB_ABus,
      OPB_BE => mb_opb_OPB_BE,
      OPB_beXfer => open,
      OPB_beAck => open,
      OPB_busLock => open,
      OPB_rdDBus => open,
      OPB_wrDBus => open,
      OPB_DBus => mb_opb_OPB_DBus,
      OPB_errAck => mb_opb_OPB_errAck,
      OPB_dwAck => open,
      OPB_dwXfer => open,
      OPB_fwAck => open,
      OPB_fwXfer => open,
      OPB_hwAck => open,
      OPB_hwXfer => open,
      OPB_MGrant => mb_opb_OPB_MGrant,
      OPB_pendReq => open,
      OPB_retry => mb_opb_OPB_retry,
      OPB_RNW => mb_opb_OPB_RNW,
      OPB_select => mb_opb_OPB_select,
      OPB_seqAddr => mb_opb_OPB_seqAddr,
      OPB_timeout => mb_opb_OPB_timeout,
      OPB_toutSup => open,
      OPB_xferAck => mb_opb_OPB_xferAck
    );

  debug_module : debug_module_wrapper
    port map (
      OPB_Clk => sys_clk_s,
      OPB_Rst => mb_opb_OPB_Rst,
      Interrupt => open,
      Debug_SYS_Rst => mb_opb_Debug_SYS_Rst,
      Debug_Rst => Debug_Rst,
      Ext_BRK => Ext_BRK,
      Ext_NM_BRK => Ext_NM_BRK,
      OPB_ABus => mb_opb_OPB_ABus,
      OPB_BE => mb_opb_OPB_BE,
      OPB_RNW => mb_opb_OPB_RNW,
      OPB_select => mb_opb_OPB_select,
      OPB_seqAddr => mb_opb_OPB_seqAddr,
      OPB_DBus => mb_opb_OPB_DBus,
      MDM_DBus => mb_opb_Sl_DBus(0 to 31),
      MDM_errAck => mb_opb_Sl_errAck(0),
      MDM_retry => mb_opb_Sl_retry(0),
      MDM_toutSup => mb_opb_Sl_toutSup(0),
      MDM_xferAck => mb_opb_Sl_xferAck(0),
      Dbg_Clk_0 => DBG_CLK_s,
      Dbg_TDI_0 => DBG_TDI_s,
      Dbg_TDO_0 => DBG_TDO_s,
      Dbg_Reg_En_0 => DBG_REG_EN_s,
      Dbg_Capture_0 => DBG_CAPTURE_s,
      Dbg_Update_0 => DBG_UPDATE_s,
      Dbg_Clk_1 => open,
      Dbg_TDI_1 => open,
      Dbg_TDO_1 => net_gnd0,
      Dbg_Reg_En_1 => open,
      Dbg_Capture_1 => open,
      Dbg_Update_1 => open,
      Dbg_Clk_2 => open,
      Dbg_TDI_2 => open,
      Dbg_TDO_2 => net_gnd0,
      Dbg_Reg_En_2 => open,
      Dbg_Capture_2 => open,
      Dbg_Update_2 => open,
      Dbg_Clk_3 => open,
      Dbg_TDI_3 => open,
      Dbg_TDO_3 => net_gnd0,
      Dbg_Reg_En_3 => open,
      Dbg_Capture_3 => open,
      Dbg_Update_3 => open,
      Dbg_Clk_4 => open,
      Dbg_TDI_4 => open,
      Dbg_TDO_4 => net_gnd0,
      Dbg_Reg_En_4 => open,
      Dbg_Capture_4 => open,
      Dbg_Update_4 => open,
      Dbg_Clk_5 => open,
      Dbg_TDI_5 => open,
      Dbg_TDO_5 => net_gnd0,
      Dbg_Reg_En_5 => open,
      Dbg_Capture_5 => open,
      Dbg_Update_5 => open,
      Dbg_Clk_6 => open,
      Dbg_TDI_6 => open,
      Dbg_TDO_6 => net_gnd0,
      Dbg_Reg_En_6 => open,
      Dbg_Capture_6 => open,
      Dbg_Update_6 => open,
      Dbg_Clk_7 => open,
      Dbg_TDI_7 => open,
      Dbg_TDO_7 => net_gnd0,
      Dbg_Reg_En_7 => open,
      Dbg_Capture_7 => open,
      Dbg_Update_7 => open,
      bscan_tdi => open,
      bscan_reset => open,
      bscan_shift => open,
      bscan_update => open,
      bscan_capture => open,
      bscan_sel1 => open,
      bscan_drck1 => open,
      bscan_tdo1 => net_gnd0,
      FSL0_S_CLK => open,
      FSL0_S_READ => open,
      FSL0_S_DATA => net_gnd32,
      FSL0_S_CONTROL => net_gnd0,
      FSL0_S_EXISTS => net_gnd0,
      FSL0_M_CLK => open,
      FSL0_M_WRITE => open,
      FSL0_M_DATA => open,
      FSL0_M_CONTROL => open,
      FSL0_M_FULL => net_gnd0
    );

  ilmb : ilmb_wrapper
    port map (
      LMB_Clk => sys_clk_s,
      SYS_Rst => sys_rst_s,
      LMB_Rst => ilmb_OPB_Rst,
      M_ABus => ilmb_M_ABus,
      M_ReadStrobe => ilmb_M_ReadStrobe,
      M_WriteStrobe => net_gnd0,
      M_AddrStrobe => ilmb_M_AddrStrobe,
      M_DBus => net_gnd32,
      M_BE => net_gnd4,
      Sl_DBus => ilmb_Sl_DBus,
      Sl_Ready => ilmb_Sl_Ready(0 to 0),
      LMB_ABus => ilmb_LMB_ABus,
      LMB_ReadStrobe => ilmb_LMB_ReadStrobe,
      LMB_WriteStrobe => ilmb_LMB_WriteStrobe,
      LMB_AddrStrobe => ilmb_LMB_AddrStrobe,
      LMB_ReadDBus => ilmb_LMB_ReadDBus,
      LMB_WriteDBus => ilmb_LMB_WriteDBus,
      LMB_Ready => ilmb_LMB_Ready,
      LMB_BE => ilmb_LMB_BE
    );

  dlmb : dlmb_wrapper
    port map (
      LMB_Clk => sys_clk_s,
      SYS_Rst => sys_rst_s,
      LMB_Rst => dlmb_OPB_Rst,
      M_ABus => dlmb_M_ABus,
      M_ReadStrobe => dlmb_M_ReadStrobe,
      M_WriteStrobe => dlmb_M_WriteStrobe,
      M_AddrStrobe => dlmb_M_AddrStrobe,
      M_DBus => dlmb_M_DBus,
      M_BE => dlmb_M_BE,
      Sl_DBus => dlmb_Sl_DBus,
      Sl_Ready => dlmb_Sl_Ready(0 to 0),
      LMB_ABus => dlmb_LMB_ABus,
      LMB_ReadStrobe => dlmb_LMB_ReadStrobe,
      LMB_WriteStrobe => dlmb_LMB_WriteStrobe,
      LMB_AddrStrobe => dlmb_LMB_AddrStrobe,
      LMB_ReadDBus => dlmb_LMB_ReadDBus,
      LMB_WriteDBus => dlmb_LMB_WriteDBus,
      LMB_Ready => dlmb_LMB_Ready,
      LMB_BE => dlmb_LMB_BE
    );

  dlmb_cntlr : dlmb_cntlr_wrapper
    port map (
      LMB_Clk => sys_clk_s,
      LMB_Rst => dlmb_OPB_Rst,
      LMB_ABus => dlmb_LMB_ABus,
      LMB_WriteDBus => dlmb_LMB_WriteDBus,
      LMB_AddrStrobe => dlmb_LMB_AddrStrobe,
      LMB_ReadStrobe => dlmb_LMB_ReadStrobe,
      LMB_WriteStrobe => dlmb_LMB_WriteStrobe,
      LMB_BE => dlmb_LMB_BE,
      Sl_DBus => dlmb_Sl_DBus,
      Sl_Ready => dlmb_Sl_Ready(0),
      BRAM_Rst_A => dlmb_port_BRAM_Rst,
      BRAM_Clk_A => dlmb_port_BRAM_Clk,
      BRAM_EN_A => dlmb_port_BRAM_EN,
      BRAM_WEN_A => dlmb_port_BRAM_WEN,
      BRAM_Addr_A => dlmb_port_BRAM_Addr,
      BRAM_Din_A => dlmb_port_BRAM_Din,
      BRAM_Dout_A => dlmb_port_BRAM_Dout
    );

  ilmb_cntlr : ilmb_cntlr_wrapper
    port map (
      LMB_Clk => sys_clk_s,
      LMB_Rst => ilmb_OPB_Rst,
      LMB_ABus => ilmb_LMB_ABus,
      LMB_WriteDBus => ilmb_LMB_WriteDBus,
      LMB_AddrStrobe => ilmb_LMB_AddrStrobe,
      LMB_ReadStrobe => ilmb_LMB_ReadStrobe,
      LMB_WriteStrobe => ilmb_LMB_WriteStrobe,
      LMB_BE => ilmb_LMB_BE,
      Sl_DBus => ilmb_Sl_DBus,
      Sl_Ready => ilmb_Sl_Ready(0),
      BRAM_Rst_A => ilmb_port_BRAM_Rst,
      BRAM_Clk_A => ilmb_port_BRAM_Clk,
      BRAM_EN_A => ilmb_port_BRAM_EN,
      BRAM_WEN_A => ilmb_port_BRAM_WEN,
      BRAM_Addr_A => ilmb_port_BRAM_Addr,
      BRAM_Din_A => ilmb_port_BRAM_Din,
      BRAM_Dout_A => ilmb_port_BRAM_Dout
    );

  lmb_bram : lmb_bram_wrapper
    port map (
      BRAM_Rst_A => ilmb_port_BRAM_Rst,
      BRAM_Clk_A => ilmb_port_BRAM_Clk,
      BRAM_EN_A => ilmb_port_BRAM_EN,
      BRAM_WEN_A => ilmb_port_BRAM_WEN,
      BRAM_Addr_A => ilmb_port_BRAM_Addr,
      BRAM_Din_A => ilmb_port_BRAM_Din,
      BRAM_Dout_A => ilmb_port_BRAM_Dout,
      BRAM_Rst_B => dlmb_port_BRAM_Rst,
      BRAM_Clk_B => dlmb_port_BRAM_Clk,
      BRAM_EN_B => dlmb_port_BRAM_EN,
      BRAM_WEN_B => dlmb_port_BRAM_WEN,
      BRAM_Addr_B => dlmb_port_BRAM_Addr,
      BRAM_Din_B => dlmb_port_BRAM_Din,
      BRAM_Dout_B => dlmb_port_BRAM_Dout
    );

  generic_external_memory : generic_external_memory_wrapper
    port map (
      OPB_Clk => sys_clk_s,
      OPB_Rst => mb_opb_OPB_Rst,
      OPB_ABus => mb_opb_OPB_ABus,
      OPB_DBus => mb_opb_OPB_DBus,
      Sln_DBus => mb_opb_Sl_DBus(32 to 63),
      OPB_select => mb_opb_OPB_select,
      OPB_RNW => mb_opb_OPB_RNW,
      OPB_seqAddr => mb_opb_OPB_seqAddr,
      OPB_BE => mb_opb_OPB_BE,
      Sln_xferAck => mb_opb_Sl_xferAck(1),
      Sln_errAck => mb_opb_Sl_errAck(1),
      Sln_toutSup => mb_opb_Sl_toutSup(1),
      Sln_retry => mb_opb_Sl_retry(1),
      Mem_A => fpga_0_Generic_External_Memory_Mem_A_split,
      Mem_DQ_I => fpga_0_Generic_External_Memory_Mem_DQ_I,
      Mem_DQ_O => fpga_0_Generic_External_Memory_Mem_DQ_O,
      Mem_DQ_T => fpga_0_Generic_External_Memory_Mem_DQ_T,
      Mem_CEN => fpga_0_Generic_External_Memory_Mem_CEN(0 to 0),
      Mem_OEN => fpga_0_Generic_External_Memory_Mem_OEN(0 to 0),
      Mem_WEN => fpga_0_Generic_External_Memory_Mem_WEN,
      Mem_QWEN => open,
      Mem_BEN => fpga_0_Generic_External_Memory_Mem_BEN,
      Mem_RPN => fpga_0_Generic_External_Memory_Mem_RPN,
      Mem_CE => open,
      Mem_ADV_LDN => open,
      Mem_LBON => open,
      Mem_CKEN => open,
      Mem_RNW => open
    );

  rs232 : rs232_wrapper
    port map (
      OPB_Clk => sys_clk_s,
      OPB_Rst => mb_opb_OPB_Rst,
      Interrupt => open,
      OPB_ABus => mb_opb_OPB_ABus,
      OPB_BE => mb_opb_OPB_BE,
      OPB_RNW => mb_opb_OPB_RNW,
      OPB_select => mb_opb_OPB_select,
      OPB_seqAddr => mb_opb_OPB_seqAddr,
      OPB_DBus => mb_opb_OPB_DBus,
      UART_DBus => mb_opb_Sl_DBus(64 to 95),
      UART_errAck => mb_opb_Sl_errAck(2),
      UART_retry => mb_opb_Sl_retry(2),
      UART_toutSup => mb_opb_Sl_toutSup(2),
      UART_xferAck => mb_opb_Sl_xferAck(2),
      RX => fpga_0_RS232_RX,
      TX => fpga_0_RS232_TX
    );

  opb_bram_if_cntlr_1 : opb_bram_if_cntlr_1_wrapper
    port map (
      opb_clk => sys_clk_s,
      opb_rst => mb_opb_OPB_Rst,
      opb_abus => mb_opb_OPB_ABus,
      opb_dbus => mb_opb_OPB_DBus,
      sln_dbus => mb_opb_Sl_DBus(96 to 127),
      opb_select => mb_opb_OPB_select,
      opb_rnw => mb_opb_OPB_RNW,
      opb_seqaddr => mb_opb_OPB_seqAddr,
      opb_be => mb_opb_OPB_BE,
      sln_xferack => mb_opb_Sl_xferAck(3),
      sln_errack => mb_opb_Sl_errAck(3),
      sln_toutsup => mb_opb_Sl_toutSup(3),
      sln_retry => mb_opb_Sl_retry(3),
      bram_rst => opb_bram_if_cntlr_1_port_BRAM_Rst,
      bram_clk => opb_bram_if_cntlr_1_port_BRAM_Clk,
      bram_en => opb_bram_if_cntlr_1_port_BRAM_EN,
      bram_wen => opb_bram_if_cntlr_1_port_BRAM_WEN,
      bram_addr => opb_bram_if_cntlr_1_port_BRAM_Addr,
      bram_din => opb_bram_if_cntlr_1_port_BRAM_Din,
      bram_dout => opb_bram_if_cntlr_1_port_BRAM_Dout
    );

  opb_bram_if_cntlr_1_bram : opb_bram_if_cntlr_1_bram_wrapper
    port map (
      BRAM_Rst_A => opb_bram_if_cntlr_1_port_BRAM_Rst,
      BRAM_Clk_A => opb_bram_if_cntlr_1_port_BRAM_Clk,
      BRAM_EN_A => opb_bram_if_cntlr_1_port_BRAM_EN,
      BRAM_WEN_A => opb_bram_if_cntlr_1_port_BRAM_WEN,
      BRAM_Addr_A => opb_bram_if_cntlr_1_port_BRAM_Addr,
      BRAM_Din_A => opb_bram_if_cntlr_1_port_BRAM_Din,
      BRAM_Dout_A => opb_bram_if_cntlr_1_port_BRAM_Dout,
      BRAM_Rst_B => net_gnd0,
      BRAM_Clk_B => net_gnd0,
      BRAM_EN_B => net_gnd0,
      BRAM_WEN_B => net_gnd4,
      BRAM_Addr_B => net_gnd32,
      BRAM_Din_B => open,
      BRAM_Dout_B => net_gnd32
    );

  dcm_0 : dcm_0_wrapper
    port map (
      RST => net_gnd0,
      CLKIN => dcm_clk_s,
      CLKFB => dcm_0_FB,
      PSEN => net_gnd0,
      PSINCDEC => net_gnd0,
      PSCLK => net_gnd0,
      DSSEN => net_gnd0,
      CLK0 => dcm_0_FB,
      CLK90 => open,
      CLK180 => open,
      CLK270 => open,
      CLKDV => sys_clk_s,
      CLK2X => open,
      CLK2X180 => open,
      CLKFX => open,
      CLKFX180 => open,
      STATUS => open,
      LOCKED => open,
      PSDONE => open
    );

  util_bus_split_0 : util_bus_split_0_wrapper
    port map (
      Sig => fpga_0_Generic_External_Memory_Mem_A_split,
      Out1 => fpga_0_Generic_External_Memory_Mem_A,
      Out2 => open
    );

  obuf_0 : OBUF
    port map (
      I => net_gnd0,
      O => fpga_0_RS232_req_to_send_pin
    );

  ibuf_1 : IBUF
    port map (
      I => fpga_0_RS232_RX_pin,
      O => fpga_0_RS232_RX
    );

  obuf_2 : OBUF
    port map (
      I => fpga_0_RS232_TX,
      O => fpga_0_RS232_TX_pin
    );

  iobuf_3 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(0),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(0),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(0),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(0)
    );

  iobuf_4 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(1),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(1),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(1),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(1)
    );

  iobuf_5 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(2),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(2),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(2),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(2)
    );

  iobuf_6 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(3),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(3),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(3),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(3)
    );

  iobuf_7 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(4),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(4),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(4),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(4)
    );

  iobuf_8 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(5),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(5),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(5),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(5)
    );

  iobuf_9 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(6),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(6),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(6),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(6)
    );

  iobuf_10 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(7),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(7),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(7),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(7)
    );

  iobuf_11 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(8),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(8),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(8),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(8)
    );

  iobuf_12 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(9),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(9),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(9),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(9)
    );

  iobuf_13 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(10),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(10),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(10),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(10)
    );

  iobuf_14 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(11),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(11),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(11),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(11)
    );

  iobuf_15 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(12),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(12),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(12),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(12)
    );

  iobuf_16 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(13),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(13),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(13),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(13)
    );

  iobuf_17 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(14),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(14),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(14),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(14)
    );

  iobuf_18 : IOBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_DQ_O(15),
      IO => fpga_0_Generic_External_Memory_Mem_DQ_pin(15),
      O => fpga_0_Generic_External_Memory_Mem_DQ_I(15),
      T => fpga_0_Generic_External_Memory_Mem_DQ_T(15)
    );

-- obuf_19 : OBUF
--   port map (
--     I => fpga_0_Generic_External_Memory_Mem_A(0),
--     O => fpga_0_Generic_External_Memory_Mem_A_pin(0)
--   );
--
-- obuf_20 : OBUF
--   port map (
--     I => fpga_0_Generic_External_Memory_Mem_A(1),
--     O => fpga_0_Generic_External_Memory_Mem_A_pin(1)
--   );
--
-- obuf_21 : OBUF
--   port map (
--     I => fpga_0_Generic_External_Memory_Mem_A(2),
--     O => fpga_0_Generic_External_Memory_Mem_A_pin(2)
--   );
--
-- obuf_22 : OBUF
--   port map (
--     I => fpga_0_Generic_External_Memory_Mem_A(3),
--     O => fpga_0_Generic_External_Memory_Mem_A_pin(3)
--   );
--
-- obuf_23 : OBUF
--   port map (
--     I => fpga_0_Generic_External_Memory_Mem_A(4),
--     O => fpga_0_Generic_External_Memory_Mem_A_pin(4)
--   );
--
-- obuf_24 : OBUF
--   port map (
--     I => fpga_0_Generic_External_Memory_Mem_A(5),
--     O => fpga_0_Generic_External_Memory_Mem_A_pin(5)
--   );
--
-- obuf_25 : OBUF
--   port map (
--     I => fpga_0_Generic_External_Memory_Mem_A(6),
--     O => fpga_0_Generic_External_Memory_Mem_A_pin(6)
--   );

  obuf_26 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(7),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(7)
    );

  obuf_27 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(8),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(8)
    );

  obuf_28 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(9),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(9)
    );

  obuf_29 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(10),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(10)
    );

  obuf_30 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(11),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(11)
    );

  obuf_31 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(12),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(12)
    );

  obuf_32 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(13),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(13)
    );

  obuf_33 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(14),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(14)
    );

  obuf_34 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(15),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(15)
    );

  obuf_35 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(16),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(16)
    );

  obuf_36 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(17),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(17)
    );

  obuf_37 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(18),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(18)
    );

  obuf_38 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(19),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(19)
    );

  obuf_39 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(20),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(20)
    );

  obuf_40 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(21),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(21)
    );

  obuf_41 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(22),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(22)
    );

  obuf_42 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(23),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(23)
    );

  obuf_43 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(24),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(24)
    );

  obuf_44 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(25),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(25)
    );

  obuf_45 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(26),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(26)
    );

  obuf_46 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(27),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(27)
    );

  obuf_47 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(28),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(28)
    );

  obuf_48 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(29),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(29)
    );

  obuf_49 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_A(30),
      O => fpga_0_Generic_External_Memory_Mem_A_pin(30)
    );

--  obuf_50 : OBUF
--    port map (
--      I => fpga_0_Generic_External_Memory_Mem_A(31),
--      O => fpga_0_Generic_External_Memory_Mem_A_pin(31)
--    );

  obuf_51 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_BEN(0),
      O => fpga_0_Generic_External_Memory_Mem_BEN_pin(0)
    );

  obuf_52 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_BEN(1),
      O => fpga_0_Generic_External_Memory_Mem_BEN_pin(1)
    );

  obuf_53 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_WEN,
      O => fpga_0_Generic_External_Memory_Mem_WEN_pin
    );

  obuf_54 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_OEN(0),
      O => fpga_0_Generic_External_Memory_Mem_OEN_pin(0)
    );

  obuf_55 : OBUF
    port map (
      I => fpga_0_Generic_External_Memory_Mem_CEN(0),
      O => fpga_0_Generic_External_Memory_Mem_CEN_pin(0)
    );

  obuf_56 : OBUF
    port map (
      I => net_vcc0,
      O => fpga_0_Generic_External_Memory_flash_csn_dummy_pin
    );

  obuf_57 : OBUF
    port map (
      I => net_gnd0,
      O => fpga_0_Generic_External_Memory_Mem_RPN_pin
    );

-- ibufg_58 : IBUFG
--   port map (
--     I => sys_clk_pin,
--     O => dcm_clk_s
--   );

  ibuf_59 : IBUF
    port map (
      I => sys_rst_pin,
      O => sys_rst_s
    );
  obuf_61 : OBUF
    port map (
      I => net_gnd0,
      O => FADV
    );

  obuf_62 : OBUF
    port map (
      I => net_vcc0,--fpga_0_Generic_External_Memory_Mem_RPN,
      O => FRST
    );

  obuf_63 : OBUF
    port map (
      I => net_vcc0,
      O => FWP
    );
    
diff_buf_brefclk : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT") 
port map (
  		O  => dcm_clk_s,
  		I  => sys_clk_pin_p,
  		IB => sys_clk_pin_n
);
end architecture STRUCTURE;

